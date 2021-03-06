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
#include "mainwindow.h"

#define MAX_PANEL_ENTRIES 4

static char s_time_text[TIME_TEXT_LENGTH];
static char s_date_text[DATE_TEXT_LENGTH];
static char s_heart_text[PANEL_TEXT_LENGTH];
static char s_activetime_text[PANEL_TEXT_LENGTH];
static char s_steps_text[PANEL_TEXT_LENGTH];
static char s_distance_text[PANEL_TEXT_LENGTH];
static char s_sleep_text[PANEL_TEXT_LENGTH];
static char s_calories_text[PANEL_TEXT_LENGTH];
static char s_battery_text[PANEL_TEXT_LENGTH];

static Window *s_window;
static GFont s_res_font_visitor_brk_big;
static GFont s_res_font_visitor_brk_small;
static TextLayer *s_timetextlayer;
static TextLayer *s_datetextlayer;
static Layer *s_panellayer;

static TextLayer *s_entrytitlelayers[MAX_PANEL_ENTRIES];
static TextLayer *s_entrytextlayers[MAX_PANEL_ENTRIES];

static GColor s_background_color;
static GColor s_foreground_color;

static int get_entry_count() {
  int count = 0;
  for (int i = 0; i < MAX_PANEL_ENTRIES; ++i)
    if (s_entrytitlelayers[i])
      ++count;
  return count;
}

static void initialise_ui() {
  s_window = window_create();
  window_set_background_color(s_window, s_background_color);
  #ifndef PBL_SDK_3
    window_set_fullscreen(s_window, 1);
  #endif
  
#ifdef PBL_PLATFORM_EMERY
  s_res_font_visitor_brk_big = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_VISITOR_BRK_70));
  s_res_font_visitor_brk_small = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_VISITOR_BRK_30));
#else
  s_res_font_visitor_brk_big = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_VISITOR_BRK_50));
  s_res_font_visitor_brk_small = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_VISITOR_BRK_20));
#endif

  // s_timetextlayer
  s_timetextlayer = text_layer_create(GRect(0, 6, 149, 80));
  text_layer_set_background_color(s_timetextlayer, GColorClear);
  text_layer_set_text_color(s_timetextlayer, s_foreground_color);
  text_layer_set_text(s_timetextlayer, s_time_text);
  text_layer_set_text_alignment(s_timetextlayer, GTextAlignmentCenter);
  text_layer_set_font(s_timetextlayer, s_res_font_visitor_brk_big);
  layer_add_child(window_get_root_layer(s_window), text_layer_get_layer(s_timetextlayer));
  
  // s_datetextlayer
  s_datetextlayer = text_layer_create(GRect(0, 67, 144, 22));
  text_layer_set_background_color(s_datetextlayer, GColorClear);
  text_layer_set_text_color(s_datetextlayer, s_foreground_color);
  text_layer_set_text(s_datetextlayer, s_date_text);
  text_layer_set_text_alignment(s_datetextlayer, GTextAlignmentCenter);
  text_layer_set_font(s_datetextlayer, s_res_font_visitor_brk_small);
  layer_add_child(window_get_root_layer(s_window), text_layer_get_layer(s_datetextlayer));
  
  // s_panellayer
  s_panellayer = layer_create(GRect(9, 99, 125, 60));
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_panellayer);
}

static void create_entry(PanelEntry entry, int position)
{
  char *title = NULL;
  char *text = NULL;
  switch (entry) {
    case PanelEntryBattery:
      title = "Batt:";
      text = s_battery_text;
      break;
    case PanelEntryActiveTime:
#ifdef PBL_PLATFORM_EMERY
      // The font must be a multiple of 10, but the bigger font can't quite fit on emery.
      title = "Activ:";
#else
      title = "Active:";
#endif
      text = s_activetime_text;
      break;
    case PanelEntrySteps:
      title = "Steps:";
      text = s_steps_text;
      break;
    case PanelEntryDistance:
      title = "Dist:";
      text = s_distance_text;
      break;
    case PanelEntrySleep:
      title = "Sleep:";
      text = s_sleep_text;
      break;
    case PanelEntryCalories:
      title = "Cal:";
      text = s_calories_text;
      break;
    case PanelEntryHeart:
      title = "Heart:";
      text = s_heart_text;
      break;
    case PanelEntryNone:
      break;
  }

  TextLayer *titlelayer = NULL;
  TextLayer *textlayer = NULL;
  if (title) {
    titlelayer = text_layer_create(GRectZero);
    text_layer_set_background_color(titlelayer, GColorClear);
    text_layer_set_text_color(titlelayer, s_foreground_color);
    text_layer_set_text(titlelayer, title);
    text_layer_set_font(titlelayer, s_res_font_visitor_brk_small);
    layer_add_child(s_panellayer, text_layer_get_layer(titlelayer));

    textlayer = text_layer_create(GRectZero);
    text_layer_set_background_color(textlayer, GColorClear);
    text_layer_set_text_color(textlayer, s_foreground_color);
    text_layer_set_text(textlayer, text);
    text_layer_set_text_alignment(textlayer, GTextAlignmentRight);
    text_layer_set_font(textlayer, s_res_font_visitor_brk_small);
    layer_add_child(s_panellayer, text_layer_get_layer(textlayer));
  }

  s_entrytitlelayers[position] = titlelayer;
  s_entrytextlayers[position] = textlayer;
}

static void destroy_ui(void) {
  window_destroy(s_window);
  text_layer_destroy(s_timetextlayer);
  text_layer_destroy(s_datetextlayer);
  layer_destroy(s_panellayer);

  text_layer_destroy(s_entrytitlelayers[0]);
  text_layer_destroy(s_entrytitlelayers[1]);
  text_layer_destroy(s_entrytitlelayers[2]);
  text_layer_destroy(s_entrytitlelayers[3]);
  text_layer_destroy(s_entrytextlayers[0]);
  text_layer_destroy(s_entrytextlayers[1]);
  text_layer_destroy(s_entrytextlayers[2]);
  text_layer_destroy(s_entrytextlayers[3]);

  fonts_unload_custom_font(s_res_font_visitor_brk_big);
  fonts_unload_custom_font(s_res_font_visitor_brk_small);
}

#ifdef PBL_PLATFORM_EMERY
#define HEIGHT_VISITOR_BRK_BIG 70
#define HEIGHT_VISITOR_BRK_SMALL 33
#define ASCENT_VISITOR_BRK_BIG 35
#define ASCENT_VISITOR_BRK_SMALL 15
#define ORIGIN_TO_ASCENT_VISITOR_BRK_BIG (70 - ASCENT_VISITOR_BRK_BIG)
#define ORIGIN_TO_ASCENT_VISITOR_BRK_SMALL (30 - ASCENT_VISITOR_BRK_SMALL)
#else
#define HEIGHT_VISITOR_BRK_BIG 50
#define HEIGHT_VISITOR_BRK_SMALL 22
#define ASCENT_VISITOR_BRK_BIG 25
#define ASCENT_VISITOR_BRK_SMALL 10
#define ORIGIN_TO_ASCENT_VISITOR_BRK_BIG (50 - ASCENT_VISITOR_BRK_BIG)
#define ORIGIN_TO_ASCENT_VISITOR_BRK_SMALL (20 - ASCENT_VISITOR_BRK_SMALL)
#endif

#define CHAR_GAP_VISITOR_BRK_BIG 6
#define CHAR_GAP_VISITOR_BRK_SMALL 2
#define SCREEN_MARGIN 5
#define PANEL_SIDES_PADDING 5
#define PANEL_TOPBOTTOM_PADDING 7
#define PANEL_ENTRY_SPAN PANEL_TOPBOTTOM_PADDING


static void layout_panel() {
  GRect bounds = layer_get_bounds(s_panellayer);
  int width = bounds.size.w;
  int hcenter = width / 2 + 1;
  int right_width = width - hcenter - PANEL_SIDES_PADDING + CHAR_GAP_VISITOR_BRK_SMALL;

  int entry_count = get_entry_count();
  int extendable_height = bounds.size.h - PANEL_TOPBOTTOM_PADDING * 2;
  extendable_height -= ASCENT_VISITOR_BRK_SMALL * entry_count;
  int gap = extendable_height / (entry_count - 1);

  int entry_ascent_offsets[MAX_PANEL_ENTRIES];
  int next_offset = PANEL_TOPBOTTOM_PADDING;
  for (size_t i = 0; i < MAX_PANEL_ENTRIES; ++i) {
    if (s_entrytitlelayers[i]) {
      entry_ascent_offsets[i] = next_offset;
      next_offset = next_offset + ASCENT_VISITOR_BRK_SMALL + gap;
    }
  }

  if (s_entrytitlelayers[0]) {
    layer_set_frame(text_layer_get_layer(s_entrytitlelayers[0]), GRect(PANEL_SIDES_PADDING, entry_ascent_offsets[0] - ORIGIN_TO_ASCENT_VISITOR_BRK_SMALL, hcenter, HEIGHT_VISITOR_BRK_SMALL));
    layer_set_frame(text_layer_get_layer(s_entrytextlayers[0]), GRect(hcenter, entry_ascent_offsets[0] - ORIGIN_TO_ASCENT_VISITOR_BRK_SMALL, right_width, HEIGHT_VISITOR_BRK_SMALL));
  }
  if (s_entrytitlelayers[1]) {
    layer_set_frame(text_layer_get_layer(s_entrytitlelayers[1]), GRect(PANEL_SIDES_PADDING, entry_ascent_offsets[1] - ORIGIN_TO_ASCENT_VISITOR_BRK_SMALL, hcenter, HEIGHT_VISITOR_BRK_SMALL));
    layer_set_frame(text_layer_get_layer(s_entrytextlayers[1]), GRect(hcenter, entry_ascent_offsets[1] - ORIGIN_TO_ASCENT_VISITOR_BRK_SMALL, right_width, HEIGHT_VISITOR_BRK_SMALL));
  }
  if (s_entrytitlelayers[2]) {
    layer_set_frame(text_layer_get_layer(s_entrytitlelayers[2]), GRect(PANEL_SIDES_PADDING, entry_ascent_offsets[2] - ORIGIN_TO_ASCENT_VISITOR_BRK_SMALL, hcenter, HEIGHT_VISITOR_BRK_SMALL));
    layer_set_frame(text_layer_get_layer(s_entrytextlayers[2]), GRect(hcenter, entry_ascent_offsets[2] - ORIGIN_TO_ASCENT_VISITOR_BRK_SMALL, right_width, HEIGHT_VISITOR_BRK_SMALL));
  }
  if (s_entrytitlelayers[3]) {
    layer_set_frame(text_layer_get_layer(s_entrytitlelayers[3]), GRect(PANEL_SIDES_PADDING, entry_ascent_offsets[3] - ORIGIN_TO_ASCENT_VISITOR_BRK_SMALL, hcenter, HEIGHT_VISITOR_BRK_SMALL));
    layer_set_frame(text_layer_get_layer(s_entrytextlayers[3]), GRect(hcenter, entry_ascent_offsets[3] - ORIGIN_TO_ASCENT_VISITOR_BRK_SMALL, right_width, HEIGHT_VISITOR_BRK_SMALL));
  }
}

static void layout() {
  Layer *root = window_get_root_layer(s_window);
  GRect bounds = layer_get_bounds(root);
  int width = bounds.size.w;

  int entry_count = get_entry_count();
  int panel_height = entry_count ? (entry_count * ASCENT_VISITOR_BRK_SMALL + (entry_count - 1) * PANEL_ENTRY_SPAN + 2 * PANEL_TOPBOTTOM_PADDING) : 0;
  int hgap_time_date = ASCENT_VISITOR_BRK_BIG / 2;
  int extendable_height = bounds.size.h;
  extendable_height -= hgap_time_date + ASCENT_VISITOR_BRK_BIG + ASCENT_VISITOR_BRK_SMALL + panel_height + 2 * SCREEN_MARGIN;
  int hgap_1 = extendable_height / 2;
  int hgap_2 = extendable_height - hgap_1;

  int time_ascent_offset = SCREEN_MARGIN + hgap_1;
  int date_ascent_offset = time_ascent_offset + ASCENT_VISITOR_BRK_BIG + hgap_time_date;
  int panel_y_offset = date_ascent_offset + ASCENT_VISITOR_BRK_SMALL + hgap_2;

  layer_set_frame(text_layer_get_layer(s_timetextlayer), GRect(0, time_ascent_offset - ORIGIN_TO_ASCENT_VISITOR_BRK_BIG, width + CHAR_GAP_VISITOR_BRK_BIG, HEIGHT_VISITOR_BRK_BIG));
  layer_set_frame(text_layer_get_layer(s_datetextlayer), GRect(0, date_ascent_offset - ORIGIN_TO_ASCENT_VISITOR_BRK_SMALL, width + CHAR_GAP_VISITOR_BRK_SMALL, HEIGHT_VISITOR_BRK_SMALL));
  layer_set_frame((Layer*)s_panellayer, GRect(SCREEN_MARGIN, panel_y_offset, width - SCREEN_MARGIN * 2, panel_height));

  layout_panel();
}

static void framelayer_update_proc(Layer *layer, GContext *ctx) {
  graphics_context_set_stroke_color(ctx, s_foreground_color);
  graphics_draw_rect(ctx, layer_get_bounds(layer));
}

static void handle_window_unload(Window* window) {
  destroy_ui();
}

void show_mainwindow(ClaySettings *settings) {
  s_background_color = settings->BackgroundColor;
  s_foreground_color = settings->ForegroundColor;
  initialise_ui();
  create_entry(settings->Entry1, 0);
  create_entry(settings->Entry2, 1);
  create_entry(settings->Entry3, 2);
  create_entry(settings->Entry4, 3);
  layout();

  layer_set_update_proc(s_panellayer, framelayer_update_proc);

  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);
}

void hide_mainwindow(void) {
  window_stack_remove(s_window, true);
}

void mainwindow_mark_dirty()
{
  layer_mark_dirty(window_get_root_layer(s_window));
}

char *get_time_text() {
  return s_time_text;
}

char *get_date_text() {
  return s_date_text;
}

char *get_battery_text() {
  return s_battery_text;
}

char *get_activetime_text() {
  return s_activetime_text;
}

char *get_steps_text() {
  return s_steps_text;
}

char *get_distance_text() {
  return s_distance_text;
}

char *get_sleep_text() {
  return s_sleep_text;
}

char *get_calories_text() {
  return s_calories_text;
}

char *get_heart_text() {
  return s_heart_text;
}
