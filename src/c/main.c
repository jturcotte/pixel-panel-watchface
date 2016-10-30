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

static char s_battery_text[8];
static char s_time_text[8];
static char s_date_text[16];

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

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void battery_callback(BatteryChargeState state) {
  snprintf(s_battery_text, sizeof(s_battery_text), "%d%%", state.charge_percent);
  set_battery_text(s_battery_text);
}

static void init() {
  show_mainwindow();

  // Make sure the time is displayed from the start
  update_time();
  battery_callback(battery_state_service_peek());

  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  battery_state_service_subscribe(battery_callback);
}

static void deinit() {
  hide_mainwindow();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}