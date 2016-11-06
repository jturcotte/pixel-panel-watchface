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

#define TIME_TEXT_LENGTH 8
#define DATE_TEXT_LENGTH 16
#define BATTERY_TEXT_LENGTH 8
#define ACTIVETIME_TEXT_LENGTH 8
#define STEPS_TEXT_LENGTH 8
#define HEART_TEXT_LENGTH 8

typedef enum {
    PanelEntryNone,
    PanelEntryBattery,
    PanelEntryActiveTime,
    PanelEntrySteps,
    PanelEntryHeart
} PanelEntry;

typedef struct ClaySettings {
  PanelEntry Entry1;
  PanelEntry Entry2;
  PanelEntry Entry3;
  PanelEntry Entry4;
  GColor BackgroundColor;
  GColor ForegroundColor;
} __attribute__((__packed__)) ClaySettings;

void show_mainwindow(ClaySettings *settings);
void hide_mainwindow(void);
void mainwindow_mark_dirty();
char *get_time_text();
char *get_date_text();
char *get_battery_text();
char *get_activetime_text();
char *get_steps_text();
char *get_heart_text();
