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

static char s_time_text[8];
static char s_date_text[16];
static char s_heart_text[8];
static char s_steps_text[8];
static char s_battery_text[8];

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Write the current hours and minutes into a buffer
  strftime(s_time_text, sizeof(s_time_text), clock_is_24h_style() ?
                                          "%H:%M" : "%I:%M", tick_time);
  strftime(s_date_text, sizeof(s_date_text), "%a, %b %d", tick_time);

  set_time_text(s_time_text);
  set_date_text(s_date_text);
}

static void tick_event_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void battery_event_handler(BatteryChargeState state) {
  snprintf(s_battery_text, sizeof(s_battery_text), "%d%%", state.charge_percent);
  set_battery_text(s_battery_text);
}

static void health_data_event_handler(HealthEventType type, void *context) {
  if (type == HealthEventHeartRateUpdate) {
    int bpm = health_service_peek_current_value(HealthMetricHeartRateBPM);
    snprintf(s_heart_text, sizeof(s_heart_text), "%d", bpm);
    set_heart_text(s_heart_text);
  } else if (type == HealthEventMovementUpdate) {
    int steps = health_service_sum_today(HealthMetricStepCount);
    if (steps >= 100000) {
      int kilo = steps / 1000;
      snprintf(s_steps_text, sizeof(s_steps_text), "%dk", steps / 1000);
    } else if (steps >= 1000) {
      int kilo = steps / 1000;
      snprintf(s_steps_text, sizeof(s_steps_text), "%d.%dk", kilo, (steps - kilo * 1000) / 100);
    } else {
      snprintf(s_steps_text, sizeof(s_steps_text), "%d", steps);
    }
    set_steps_text(s_steps_text);
  }
}

static void init() {
  show_mainwindow();

  // Make sure the panel is updated from the start
  update_time();
  battery_event_handler(battery_state_service_peek());
  health_data_event_handler(HealthEventHeartRateUpdate, NULL);
  health_data_event_handler(HealthEventMovementUpdate, NULL);

  // Register event handlers
  tick_timer_service_subscribe(MINUTE_UNIT, tick_event_handler);
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