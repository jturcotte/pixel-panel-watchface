/*
  Copyright (C) 2016 Jocelyn Turcotte <turcotte.j@gmail.com>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
*/

#include <pebble.h>
#include "src/c/mainwindow.h"

#define SETTINGS_KEY 1

ClaySettings settings;

static PanelEntry tuple_to_panelentry(Tuple *tuple) {
  if (tuple->type != TUPLE_CSTRING)
    return PanelEntryNone;

  char *str = tuple->value->cstring;
  if (!strncmp("Battery", str, tuple->length))
    return PanelEntryBattery;
  if (!strncmp("ActiveTime", str, tuple->length))
    return PanelEntryActiveTime;
  if (!strncmp("Steps", str, tuple->length))
    return PanelEntrySteps;
  if (!strncmp("Distance", str, tuple->length))
    return PanelEntryDistance;
  if (!strncmp("Sleep", str, tuple->length))
    return PanelEntrySleep;
  if (!strncmp("Calories", str, tuple->length))
    return PanelEntryCalories;
  if (!strncmp("Heart", str, tuple->length))
    return PanelEntryHeart;
  return PanelEntryNone;
}

static void convert_count_to_string(int count, char *output, size_t length) {
  if (count >= 100000) {
    snprintf(output, length, "%dk", count / 1000);
  } else if (count >= 1000) {
    int kilo = count / 1000;
    snprintf(output, length, "%d.%dk", kilo, (count - kilo * 1000) / 100);
  } else {
    snprintf(output, length, "%d", count);
  }
}

static void convert_seconds_to_string(int seconds, char *output, size_t length) {
  int hours = seconds / 3600;
  int minutes = seconds / 60 - hours * 60;
  if (hours >= 10)
    snprintf(output, length, "%dh", hours);
  else if (hours >= 1)
    snprintf(output, length, "%d:%d", hours, minutes);
  else
    snprintf(output, length, "%dm", minutes);
}

static void default_settings() {
  settings.Entry1 = PanelEntryBattery;
  settings.Entry2 = PanelEntryActiveTime;
  settings.Entry3 = PanelEntrySteps;
  settings.Entry4 = PanelEntryHeart;
  settings.BackgroundColor = GColorBlack;
  settings.ForegroundColor = GColorWhite;
}

static void load_settings() {
  default_settings();
  persist_read_data(SETTINGS_KEY, &settings, sizeof(settings));
}

static void save_settings() {
  persist_write_data(SETTINGS_KEY, &settings, sizeof(settings));
}

static void inbox_received_handler(DictionaryIterator *iter, void *context) {
  Tuple *bg_color_t = dict_find(iter, MESSAGE_KEY_BackgroundColor);
  if (bg_color_t)
    settings.BackgroundColor = GColorFromHEX(bg_color_t->value->int32);

  Tuple *fg_color_t = dict_find(iter, MESSAGE_KEY_ForegroundColor);
  if (fg_color_t)
    settings.ForegroundColor = GColorFromHEX(fg_color_t->value->int32);

  Tuple *entry1_t = dict_find(iter, MESSAGE_KEY_Entry1);
  if (entry1_t)
    settings.Entry1 = tuple_to_panelentry(entry1_t);

  Tuple *entry2_t = dict_find(iter, MESSAGE_KEY_Entry2);
  if (entry2_t)
    settings.Entry2 = tuple_to_panelentry(entry2_t);

  Tuple *entry3_t = dict_find(iter, MESSAGE_KEY_Entry3);
  if (entry3_t)
    settings.Entry3 = tuple_to_panelentry(entry3_t);

  Tuple *entry4_t = dict_find(iter, MESSAGE_KEY_Entry4);
  if (entry4_t)
    settings.Entry4 = tuple_to_panelentry(entry4_t);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Config: %d %s", settings.Entry1, entry1_t->value->cstring);

  save_settings();
  hide_mainwindow();
  show_mainwindow(&settings);
}

static void tick_event_handler(struct tm *tick_time, TimeUnits units_changed) {
  if (!tick_time) {
    time_t temp = time(NULL); 
    tick_time = localtime(&temp);
  }

  if (units_changed & MINUTE_UNIT) {
    // Write the current hours and minutes into a buffer
    char *time_text = get_time_text();
    strftime(time_text, TIME_TEXT_LENGTH, clock_is_24h_style() ? "%H:%M" : "%I:%M", tick_time);
    mainwindow_mark_dirty();
  }
  if (units_changed & DAY_UNIT) {
    char *date_text = get_date_text();
    strftime(date_text, DATE_TEXT_LENGTH, "%a, %b %d", tick_time);
    mainwindow_mark_dirty();
  }
}

static void battery_event_handler(BatteryChargeState state) {
  char *battery_text = get_battery_text();
  snprintf(battery_text, PANEL_TEXT_LENGTH, "%d%%", state.charge_percent);
  mainwindow_mark_dirty();
}

static void health_data_event_handler(HealthEventType type, void *context) {
  if (type == HealthEventHeartRateUpdate || type == HealthEventSignificantUpdate) {
    int bpm = health_service_peek_current_value(HealthMetricHeartRateBPM);
    char *heart_text = get_heart_text();
    snprintf(heart_text, PANEL_TEXT_LENGTH, "%d", bpm);
    mainwindow_mark_dirty();
  }

  if (type == HealthEventMovementUpdate || type == HealthEventSignificantUpdate) {
    int seconds = health_service_sum_today(HealthMetricActiveSeconds);
    char *activetime_text = get_activetime_text();
    convert_seconds_to_string(seconds, activetime_text, PANEL_TEXT_LENGTH);

    int steps = health_service_sum_today(HealthMetricStepCount);
    char *steps_text = get_steps_text();
    convert_count_to_string(steps, steps_text, PANEL_TEXT_LENGTH);

    int calories = health_service_sum_today(HealthMetricActiveKCalories)
      + health_service_sum_today(HealthMetricRestingKCalories);
    char *calories_text = get_calories_text();
    convert_count_to_string(calories, calories_text, PANEL_TEXT_LENGTH);

    int meters = health_service_sum_today(HealthMetricWalkedDistanceMeters);
    char *distance_text = get_distance_text();
    if (meters >= 100000) {
      snprintf(distance_text, PANEL_TEXT_LENGTH, "%dkm", meters / 1000);
    } else if (meters >= 1000) {
      int kilo = meters / 1000;
      snprintf(distance_text, PANEL_TEXT_LENGTH, "%d.%dkm", kilo, (meters - kilo * 1000) / 100);
    } else {
      snprintf(distance_text, PANEL_TEXT_LENGTH, "%d", meters);
    }
    mainwindow_mark_dirty();
  }

  if (type == HealthEventSleepUpdate || type == HealthEventSignificantUpdate) {
    int seconds = health_service_sum_today(HealthMetricSleepSeconds);
    char *sleep_text = get_sleep_text();
    convert_seconds_to_string(seconds, sleep_text, PANEL_TEXT_LENGTH);
    mainwindow_mark_dirty();
  }
}

static void init() {
  load_settings();
  show_mainwindow(&settings);

  // Listen for AppMessages
  app_message_register_inbox_received(inbox_received_handler);
  app_message_open(128, 128);

  // Make sure the panel is updated from the start
  tick_event_handler(NULL, MINUTE_UNIT | DAY_UNIT);
  battery_event_handler(battery_state_service_peek());
  health_data_event_handler(HealthEventSignificantUpdate, NULL);

  // Register event handlers
  tick_timer_service_subscribe(MINUTE_UNIT | DAY_UNIT, tick_event_handler);
  battery_state_service_subscribe(battery_event_handler);
  health_service_events_subscribe(health_data_event_handler, NULL);
}

static void deinit() {
  hide_mainwindow();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}