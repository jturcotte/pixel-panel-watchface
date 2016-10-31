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

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GFont s_res_font_visitor_brk_50;
static GFont s_res_font_visitor_brk_20;
static TextLayer *s_timetextlayer;
static TextLayer *s_datetextlayer;
static TextLayer *s_batttextlayer;
static TextLayer *s_stepstextlayer;
static TextLayer *s_hearttextlayer;
static Layer *s_panellayer;
static TextLayer *s_titlebatt;
static TextLayer *s_titlesteps;
static TextLayer *s_titleheart;

static void initialise_ui(void) {
  s_window = window_create();
  window_set_background_color(s_window, GColorBlack);
  #ifndef PBL_SDK_3
    window_set_fullscreen(s_window, 1);
  #endif
  
  s_res_font_visitor_brk_50 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_VISITOR_BRK_50));
  s_res_font_visitor_brk_20 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_VISITOR_BRK_20));
  // s_timetextlayer
  s_timetextlayer = text_layer_create(GRect(0, 6, 149, 80));
  text_layer_set_background_color(s_timetextlayer, GColorClear);
  text_layer_set_text_color(s_timetextlayer, GColorWhite);
  text_layer_set_text(s_timetextlayer, "22:56");
  text_layer_set_text_alignment(s_timetextlayer, GTextAlignmentCenter);
  text_layer_set_font(s_timetextlayer, s_res_font_visitor_brk_50);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_timetextlayer);
  
  // s_datetextlayer
  s_datetextlayer = text_layer_create(GRect(0, 67, 144, 22));
  text_layer_set_background_color(s_datetextlayer, GColorClear);
  text_layer_set_text_color(s_datetextlayer, GColorWhite);
  text_layer_set_text(s_datetextlayer, "Tue, Oct 26");
  text_layer_set_text_alignment(s_datetextlayer, GTextAlignmentCenter);
  text_layer_set_font(s_datetextlayer, s_res_font_visitor_brk_20);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_datetextlayer);
  
  // s_panellayer
  s_panellayer = layer_create(GRect(9, 99, 125, 60));
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_panellayer);
  
  // s_batttextlayer
  s_batttextlayer = text_layer_create(GRect(78, 96, 53, 22));
  text_layer_set_background_color(s_batttextlayer, GColorClear);
  text_layer_set_text_color(s_batttextlayer, GColorWhite);
  text_layer_set_text_alignment(s_batttextlayer, GTextAlignmentRight);
  text_layer_set_font(s_batttextlayer, s_res_font_visitor_brk_20);
  layer_add_child(s_panellayer, (Layer *)s_batttextlayer);
  
  // s_stepstextlayer
  s_stepstextlayer = text_layer_create(GRect(78, 114, 53, 22));
  text_layer_set_background_color(s_stepstextlayer, GColorClear);
  text_layer_set_text_color(s_stepstextlayer, GColorWhite);
  text_layer_set_text_alignment(s_stepstextlayer, GTextAlignmentRight);
  text_layer_set_font(s_stepstextlayer, s_res_font_visitor_brk_20);
  layer_add_child(s_panellayer, (Layer *)s_stepstextlayer);
  
  // s_hearttextlayer
  s_hearttextlayer = text_layer_create(GRect(78, 132, 53, 22));
  text_layer_set_background_color(s_hearttextlayer, GColorClear);
  text_layer_set_text_color(s_hearttextlayer, GColorWhite);
  text_layer_set_text_alignment(s_hearttextlayer, GTextAlignmentRight);
  text_layer_set_font(s_hearttextlayer, s_res_font_visitor_brk_20);
  layer_add_child(s_panellayer, (Layer *)s_hearttextlayer);
  
  // s_titlebatt
  s_titlebatt = text_layer_create(GRect(14, 96, 64, 22));
  text_layer_set_background_color(s_titlebatt, GColorClear);
  text_layer_set_text_color(s_titlebatt, GColorWhite);
  text_layer_set_text(s_titlebatt, "Batt:");
  text_layer_set_font(s_titlebatt, s_res_font_visitor_brk_20);
  layer_add_child(s_panellayer, (Layer *)s_titlebatt);

  // s_titlesteps
  s_titlesteps = text_layer_create(GRect(14, 114, 64, 22));
  text_layer_set_background_color(s_titlesteps, GColorClear);
  text_layer_set_text_color(s_titlesteps, GColorWhite);
  text_layer_set_text(s_titlesteps, "Steps:");
  text_layer_set_font(s_titlesteps, s_res_font_visitor_brk_20);
  layer_add_child(s_panellayer, (Layer *)s_titlesteps);
  
  // s_titleheart
  s_titleheart = text_layer_create(GRect(14, 132, 64, 22));
  text_layer_set_background_color(s_titleheart, GColorClear);
  text_layer_set_text_color(s_titleheart, GColorWhite);
  text_layer_set_text(s_titleheart, "Heart:");
  text_layer_set_font(s_titleheart, s_res_font_visitor_brk_20);
  layer_add_child(s_panellayer, (Layer *)s_titleheart);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  text_layer_destroy(s_timetextlayer);
  text_layer_destroy(s_datetextlayer);
  text_layer_destroy(s_batttextlayer);
  text_layer_destroy(s_stepstextlayer);
  text_layer_destroy(s_hearttextlayer);
  layer_destroy(s_panellayer);
  text_layer_destroy(s_titleheart);
  text_layer_destroy(s_titlesteps);
  text_layer_destroy(s_titlebatt);
  fonts_unload_custom_font(s_res_font_visitor_brk_50);
  fonts_unload_custom_font(s_res_font_visitor_brk_20);
}
// END AUTO-GENERATED UI CODE

#define HEIGHT_VISITOR_BRK_50 50
#define HEIGHT_VISITOR_BRK_20 22
#define ASCENT_VISITOR_BRK_50 25
#define ASCENT_VISITOR_BRK_20 10
#define ORIGIN_TO_ASCENT_VISITOR_BRK_50 (50 - ASCENT_VISITOR_BRK_50)
#define ORIGIN_TO_ASCENT_VISITOR_BRK_20 (20 - ASCENT_VISITOR_BRK_20)
#define CHAR_GAP_VISITOR_BRK_50 6
#define CHAR_GAP_VISITOR_BRK_20 2
#define PANEL_HEIGHT 60
#define PANEL_BOTTOM_MARGIN 9
#define PANEL_SIDES_MARGIN PANEL_BOTTOM_MARGIN
#define PANEL_SIDES_PADDING 5
#define PANEL_TOPBOTTOM_PADDING 7


static void layout_panel() {
  GRect bounds = layer_get_bounds(s_panellayer);
  int width = bounds.size.w;
  int hcenter = width / 2 + 1;
  int right_width = width - hcenter - PANEL_SIDES_PADDING + CHAR_GAP_VISITOR_BRK_20;

  int extendable_height = bounds.size.h - PANEL_TOPBOTTOM_PADDING * 2;
  extendable_height -= ASCENT_VISITOR_BRK_20 * 3;
  int gap = extendable_height / 2;
  int batt_ascent_offset = PANEL_TOPBOTTOM_PADDING;
  int steps_ascent_offset = batt_ascent_offset + ASCENT_VISITOR_BRK_20 + gap;
  int heart_ascent_offset = steps_ascent_offset + ASCENT_VISITOR_BRK_20 + gap;

  layer_set_frame((Layer*)s_batttextlayer, GRect(hcenter, batt_ascent_offset - ORIGIN_TO_ASCENT_VISITOR_BRK_20, right_width, HEIGHT_VISITOR_BRK_20));
  layer_set_frame((Layer*)s_stepstextlayer, GRect(hcenter, steps_ascent_offset - ORIGIN_TO_ASCENT_VISITOR_BRK_20, right_width, HEIGHT_VISITOR_BRK_20));
  layer_set_frame((Layer*)s_hearttextlayer, GRect(hcenter, heart_ascent_offset - ORIGIN_TO_ASCENT_VISITOR_BRK_20, right_width, HEIGHT_VISITOR_BRK_20));
  layer_set_frame((Layer*)s_titlebatt, GRect(PANEL_SIDES_PADDING, batt_ascent_offset - ORIGIN_TO_ASCENT_VISITOR_BRK_20, hcenter, HEIGHT_VISITOR_BRK_20));
  layer_set_frame((Layer*)s_titlesteps, GRect(PANEL_SIDES_PADDING, steps_ascent_offset - ORIGIN_TO_ASCENT_VISITOR_BRK_20, hcenter, HEIGHT_VISITOR_BRK_20));
  layer_set_frame((Layer*)s_titleheart, GRect(PANEL_SIDES_PADDING, heart_ascent_offset - ORIGIN_TO_ASCENT_VISITOR_BRK_20, hcenter, HEIGHT_VISITOR_BRK_20));
}

static void layout() {
  Layer *root = window_get_root_layer(s_window);
  GRect bounds = layer_get_bounds(root);
  // int x = bounds.origin.x;
  int width = bounds.size.w;
  GSize s = text_layer_get_content_size(s_timetextlayer);

  int extendable_height = bounds.size.h;
  extendable_height -= ASCENT_VISITOR_BRK_50 + ASCENT_VISITOR_BRK_20 + PANEL_HEIGHT + PANEL_BOTTOM_MARGIN;
  int hgap_1_2 = extendable_height * 2 / 5;
  int hgap_2 = extendable_height * 1 / 5;
  int hgap_3 = extendable_height - hgap_1_2 - hgap_1_2;

  int time_ascent_offset = hgap_1_2;
  int date_ascent_offset = time_ascent_offset + ASCENT_VISITOR_BRK_50 + hgap_1_2;
  int panel_y_offset = date_ascent_offset + ASCENT_VISITOR_BRK_20 + hgap_3;

  layer_set_frame((Layer*)s_timetextlayer, GRect(0, time_ascent_offset - ORIGIN_TO_ASCENT_VISITOR_BRK_50, width + CHAR_GAP_VISITOR_BRK_50, HEIGHT_VISITOR_BRK_50));
  layer_set_frame((Layer*)s_datetextlayer, GRect(0, date_ascent_offset - ORIGIN_TO_ASCENT_VISITOR_BRK_20, width, HEIGHT_VISITOR_BRK_20));
  layer_set_frame((Layer*)s_panellayer, GRect(PANEL_SIDES_MARGIN, panel_y_offset, width - PANEL_SIDES_MARGIN * 2, 60));

  layout_panel();
}

static void framelayer_update_proc(Layer *layer, GContext *ctx) {
  graphics_context_set_stroke_color(ctx, GColorWhite);
  graphics_draw_rect(ctx, layer_get_bounds(layer));
}

static void handle_window_unload(Window* window) {
  destroy_ui();
}

void show_mainwindow(void) {
  initialise_ui();
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

void set_time_text(char *text)
{
  text_layer_set_text(s_timetextlayer, text);
}

void set_date_text(char *text)
{
  text_layer_set_text(s_datetextlayer, text);
}

void set_battery_text(char *text)
{
  text_layer_set_text(s_batttextlayer, text);
}

void set_steps_text(char *text)
{
  text_layer_set_text(s_stepstextlayer, text);
}

void set_heart_text(char *text)
{
  text_layer_set_text(s_hearttextlayer, text);
}
