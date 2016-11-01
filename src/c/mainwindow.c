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
static Layer *s_separatorlayer;
static Layer *s_framelayer;
static TextLayer *s_titleheart;
static TextLayer *s_titlesteps;
static TextLayer *s_titlebatt;

static void initialise_ui(void) {
  s_window = window_create();
  window_set_background_color(s_window, GColorBlack);
  #ifndef PBL_SDK_3
    window_set_fullscreen(s_window, 1);
  #endif
  
  s_res_font_visitor_brk_50 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_VISITOR_BRK_50));
  s_res_font_visitor_brk_20 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_VISITOR_BRK_20));
  // s_timetextlayer
  s_timetextlayer = text_layer_create(GRect(0, -6, 149, 50));
  text_layer_set_background_color(s_timetextlayer, GColorClear);
  text_layer_set_text_color(s_timetextlayer, GColorWhite);
  text_layer_set_text(s_timetextlayer, "22:56");
  text_layer_set_text_alignment(s_timetextlayer, GTextAlignmentCenter);
  text_layer_set_font(s_timetextlayer, s_res_font_visitor_brk_50);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_timetextlayer);
  
  // s_datetextlayer
  s_datetextlayer = text_layer_create(GRect(0, 67, 144, 20));
  text_layer_set_background_color(s_datetextlayer, GColorClear);
  text_layer_set_text_color(s_datetextlayer, GColorWhite);
  text_layer_set_text(s_datetextlayer, "Tue, Oct 26");
  text_layer_set_text_alignment(s_datetextlayer, GTextAlignmentCenter);
  text_layer_set_font(s_datetextlayer, s_res_font_visitor_brk_20);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_datetextlayer);
  
  // s_separatorlayer
  s_separatorlayer = layer_create(GRect(9, 63, 125, 2));
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_separatorlayer);
  
  // s_framelayer
  s_framelayer = layer_create(GRect(9, 99, 125, 60));
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_framelayer);
  
  // s_titleheart
  s_titleheart = text_layer_create(GRect(14, 96, 64, 20));
  text_layer_set_background_color(s_titleheart, GColorClear);
  text_layer_set_text_color(s_titleheart, GColorWhite);
  text_layer_set_text(s_titleheart, "Heart:");
  text_layer_set_font(s_titleheart, s_res_font_visitor_brk_20);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_titleheart);
  
  // s_titlesteps
  s_titlesteps = text_layer_create(GRect(14, 114, 64, 20));
  text_layer_set_background_color(s_titlesteps, GColorClear);
  text_layer_set_text_color(s_titlesteps, GColorWhite);
  text_layer_set_text(s_titlesteps, "Steps:");
  text_layer_set_font(s_titlesteps, s_res_font_visitor_brk_20);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_titlesteps);
  
  // s_titlebatt
  s_titlebatt = text_layer_create(GRect(14, 132, 64, 20));
  text_layer_set_background_color(s_titlebatt, GColorClear);
  text_layer_set_text_color(s_titlebatt, GColorWhite);
  text_layer_set_text(s_titlebatt, "Batt:");
  text_layer_set_font(s_titlebatt, s_res_font_visitor_brk_20);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_titlebatt);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  text_layer_destroy(s_timetextlayer);
  text_layer_destroy(s_datetextlayer);
  layer_destroy(s_separatorlayer);
  layer_destroy(s_framelayer);
  text_layer_destroy(s_titleheart);
  text_layer_destroy(s_titlesteps);
  text_layer_destroy(s_titlebatt);
  fonts_unload_custom_font(s_res_font_visitor_brk_50);
  fonts_unload_custom_font(s_res_font_visitor_brk_20);
}
// END AUTO-GENERATED UI CODE

static void separatorlayer_update_proc(Layer *layer, GContext *ctx) {
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_fill_rect(ctx, layer_get_bounds(layer), 0, GCornerNone);
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

  layer_set_update_proc(s_separatorlayer, separatorlayer_update_proc);
  layer_set_update_proc(s_framelayer, framelayer_update_proc);

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