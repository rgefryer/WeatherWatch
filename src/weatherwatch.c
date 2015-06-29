#include "pebble.h"

// Things to do
//
// - Add an indicator of when the last update was
// - Persistently store the weather info
// - Even out display times when some screens are not available 
// - Make text_layer_set_text_max_size work out size of text area and set wrap mode
  
  
static AppSync s_sync;
static uint8_t s_sync_buffer[2000];

enum WeatherKey {
  SUMMARY_NOW = 10,
  SUMMARY_HOUR = 11,
  SUMMARY_DAY = 12,
  SUMMARY_WEEK = 13,
};

int secs_in_minute = 0;

#define NUM_FONTS 6
static GFont cFonts[NUM_FONTS];

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GFont s_res_bitham_34_medium_numbers;
static GFont s_res_roboto_condensed_21;
static TextLayer *s_time;
static InverterLayer *s_inverterlayer_1;
static TextLayer *s_date;
static TextLayer *s_updatetime;
static TextLayer *s_forecast1;
static TextLayer *s_forecast2;
static TextLayer *s_forecast3;
static TextLayer *s_forecast4;
static Layer *s_graphicslayer;

time_t expires1 = 0;
time_t expires2 = 0;
time_t expires3 = 0;
time_t expires4 = 0;
time_t lastupdatetime = 0;

static void draw_status_bar(Layer *this_layer, GContext *ctx) {
  // Draw things here using ctx
  if (secs_in_minute == 59) {
    GPoint p0 = GPoint(0, 0);
    GPoint p1 = GPoint(140, 0);
    graphics_context_set_stroke_color(ctx, GColorBlack);
    graphics_draw_line(ctx, p0, p1);  
  }
  
  GPoint p0 = GPoint(0, 0);
  GPoint p1 = GPoint((59 - secs_in_minute) * 140 / 60, 0);
  graphics_context_set_stroke_color(ctx, GColorWhite);
  graphics_draw_line(ctx, p0, p1);  
  
  graphics_context_set_stroke_color(ctx, GColorWhite);
  graphics_draw_pixel(ctx, GPoint(35, 1));  
  graphics_draw_pixel(ctx, GPoint(70, 1));  
  graphics_draw_pixel(ctx, GPoint(105, 1));  
}

static void initialise_ui(void) {
  s_window = window_create();
  window_set_background_color(s_window, GColorBlack);
  #ifndef PBL_SDK_3
    window_set_fullscreen(s_window, 1);
  #endif

  cFonts[0] = fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD);
  cFonts[1] = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
  cFonts[2] = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
  cFonts[3] = fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21);
  cFonts[4] = fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD);
  cFonts[5] = fonts_get_system_font(FONT_KEY_DROID_SERIF_28_BOLD);
    
  s_res_bitham_34_medium_numbers = fonts_get_system_font(FONT_KEY_BITHAM_34_MEDIUM_NUMBERS);
  s_res_roboto_condensed_21 = fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21);
  // s_time
  s_time = text_layer_create(GRect(1, 126, 143, 40));
  text_layer_set_background_color(s_time, GColorBlack);
  text_layer_set_text_color(s_time, GColorWhite);
  text_layer_set_text(s_time, "00:00");
  text_layer_set_text_alignment(s_time, GTextAlignmentCenter);
  text_layer_set_font(s_time, s_res_bitham_34_medium_numbers);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_time);
  
  // s_date
  s_date = text_layer_create(GRect(4, 107, 80, 24));
  text_layer_set_background_color(s_date, GColorBlack);
  text_layer_set_text_color(s_date, GColorWhite);
  text_layer_set_text(s_date, "");
  text_layer_set_font(s_date, s_res_roboto_condensed_21);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_date);
  
  // s_updatetime
  s_updatetime = text_layer_create(GRect(110, 113, 30, 24));
  text_layer_set_background_color(s_updatetime, GColorBlack);
  text_layer_set_text_color(s_updatetime, GColorWhite);
  text_layer_set_text(s_updatetime, "00:00");
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_updatetime);
  
  // s_forecast1
  s_forecast1 = text_layer_create(GRect(0, 0, 144, 105));
  text_layer_set_background_color(s_forecast1, GColorBlack);
  text_layer_set_text_color(s_forecast1, GColorWhite);
  text_layer_set_text(s_forecast1, "");
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_forecast1);
  
  // s_forecast2
  s_forecast2 = text_layer_create(GRect(0, 0, 144, 105));
  text_layer_set_background_color(s_forecast2, GColorBlack);
  text_layer_set_text_color(s_forecast2, GColorWhite);
  text_layer_set_text(s_forecast2, "");
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_forecast2);
  layer_set_hidden(text_layer_get_layer(s_forecast2), true);
  
  // s_forecast3
  s_forecast3 = text_layer_create(GRect(0, 0, 144, 105));
  text_layer_set_background_color(s_forecast3, GColorBlack);
  text_layer_set_text_color(s_forecast3, GColorWhite);
  text_layer_set_text(s_forecast3, "");
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_forecast3);
  layer_set_hidden(text_layer_get_layer(s_forecast3), true);
  
  // s_forecast4
  s_forecast4 = text_layer_create(GRect(0, 0, 144, 105));
  text_layer_set_background_color(s_forecast4, GColorBlack);
  text_layer_set_text_color(s_forecast4, GColorWhite);
  text_layer_set_text(s_forecast4, "");
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_forecast4);
  layer_set_hidden(text_layer_get_layer(s_forecast4), true);

    // s_inverterlayer_1
  s_inverterlayer_1 = inverter_layer_create(GRect(2, 132, 140, 1));
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_inverterlayer_1);
  
  // s_graphicslayer
  s_graphicslayer = layer_create(GRect(0, 105, 144, 2));
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_graphicslayer);
  layer_set_update_proc(s_graphicslayer, draw_status_bar);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  text_layer_destroy(s_time);
  inverter_layer_destroy(s_inverterlayer_1);
  text_layer_destroy(s_date);
  text_layer_destroy(s_forecast1);
  text_layer_destroy(s_forecast2);
  text_layer_destroy(s_forecast3);
  text_layer_destroy(s_forecast4);
}
// END AUTO-GENERATED UI CODE

static void handle_window_unload(Window* window) {
  destroy_ui();
}

static void sync_error_handler(DictionaryResult dict_error, AppMessageResult app_message_error, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Sync Error: %d", app_message_error);
}

void text_layer_set_text_max_size(TextLayer * xiTextLayer, const char * xiText, GSize xiSize) {

  int lOK = 0;
  int ii;
  
  text_layer_set_text(xiTextLayer, xiText);
  text_layer_set_overflow_mode(xiTextLayer, GTextOverflowModeWordWrap);
  for (ii = 0; ii < NUM_FONTS; ii++)
  {
    text_layer_set_font(xiTextLayer, cFonts[ii]);
    GSize lSize = text_layer_get_content_size(xiTextLayer);
    if (lSize.h <= xiSize.h)
    {
      lOK = ii;
    }
    else
    {
      break;
    }
  }
  text_layer_set_font(xiTextLayer, cFonts[lOK]);
  
}

static void sync_changed_handler(const uint32_t key, const Tuple* new_tuple, const Tuple* old_tuple, void* context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message success: %ld", key);
  lastupdatetime = 0;
  switch (key) {
    
    case SUMMARY_NOW:
      // App Sync keeps new_tuple in s_sync_buffer, so we may use it directly
      if (old_tuple != NULL) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Old Value: %s", old_tuple->value->cstring);
      }
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Value: %s", new_tuple->value->cstring);
      text_layer_set_text_max_size(s_forecast1, new_tuple->value->cstring, GSize(144, 105));
      expires1 = time(NULL) + (10 * 60);
      break;
    
    case SUMMARY_HOUR:
      // App Sync keeps new_tuple in s_sync_buffer, so we may use it directly
      if (old_tuple != NULL) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Old Value: %s", old_tuple->value->cstring);
      }
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Value: %s", new_tuple->value->cstring);
      text_layer_set_text_max_size(s_forecast2, new_tuple->value->cstring, GSize(144, 105));
      expires2 = time(NULL) + (15 * 60);
      break;
    
    case SUMMARY_DAY:
      // App Sync keeps new_tuple in s_sync_buffer, so we may use it directly
      if (old_tuple != NULL) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Old Value: %s", old_tuple->value->cstring);
      }
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Value: %s", new_tuple->value->cstring);
      text_layer_set_text_max_size(s_forecast3, new_tuple->value->cstring, GSize(144, 105));
      expires3 = time(NULL) + (3 * 60 * 60);
      break;
    
    case SUMMARY_WEEK:
      // App Sync keeps new_tuple in s_sync_buffer, so we may use it directly
      if (old_tuple != NULL) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Old Value: %s", old_tuple->value->cstring);
      }
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Value: %s", new_tuple->value->cstring);
      text_layer_set_text_max_size(s_forecast4, new_tuple->value->cstring, GSize(144, 105));
      expires4 = time(NULL) + (24 * 60 * 60);
      break;
    
    
  }  
}

time_t recent_time = 0;
static void update_time() {
  // Get a tm structure
  recent_time = time(NULL); 
  struct tm *tick_time = localtime(&recent_time);

  // Create a long-lived buffer
  static char buffer[] = "00:00";

  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    // Use 24 hour format
    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
  } else {
    // Use 12 hour format
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
  }

  // Create a long-lived buffer
  static char buffer2[] = "September 31";
  strftime(buffer2, sizeof("September 31"), "%B %e", tick_time);

  // Display this time on the TextLayer
  text_layer_set_text(s_time, buffer);
  text_layer_set_text(s_date, buffer2);
  
  // Create a long-lived buffer
  static char buffer3[] = "00:00";
  struct tm *tick_time2 = localtime(&lastupdatetime);
  strftime(buffer3, sizeof("00:00"), "%H:%M", tick_time2);
  text_layer_set_text(s_updatetime, buffer3);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  lastupdatetime += 1;
  if (secs_in_minute == 0) {
    update_time();    
    secs_in_minute = 60;
  }
  secs_in_minute--;
  if ((secs_in_minute == 59) && (recent_time < expires1)) {
    layer_set_hidden(text_layer_get_layer(s_forecast1), false);    
    layer_set_hidden(text_layer_get_layer(s_forecast2), true);    
    layer_set_hidden(text_layer_get_layer(s_forecast3), true);    
    layer_set_hidden(text_layer_get_layer(s_forecast4), true);    
    
 
  }
  else if ((secs_in_minute == 44) && (recent_time < expires2)) {
    layer_set_hidden(text_layer_get_layer(s_forecast1), true);    
    layer_set_hidden(text_layer_get_layer(s_forecast2), false);    
    layer_set_hidden(text_layer_get_layer(s_forecast3), true);    
    layer_set_hidden(text_layer_get_layer(s_forecast4), true);    
    
  
    
  }
  else if ((secs_in_minute == 29) && (recent_time < expires3)) {
    layer_set_hidden(text_layer_get_layer(s_forecast1), true);    
    layer_set_hidden(text_layer_get_layer(s_forecast2), true);    
    layer_set_hidden(text_layer_get_layer(s_forecast3), false);    
    layer_set_hidden(text_layer_get_layer(s_forecast4), true);    
    
 
  }
  else if ((secs_in_minute == 14) && (recent_time < expires4)) {
    layer_set_hidden(text_layer_get_layer(s_forecast1), true);    
    layer_set_hidden(text_layer_get_layer(s_forecast2), true);    
    layer_set_hidden(text_layer_get_layer(s_forecast3), true);    
    layer_set_hidden(text_layer_get_layer(s_forecast4), false);        
    
   
    
  } 
  layer_mark_dirty(s_graphicslayer);
}

void show_face(void) {
  initialise_ui();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);
}

void hide_face(void) {
  window_stack_remove(s_window, true);
}

static void init(void) {
  show_face();

  // Setup AppSync
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());

  // Setup initial values
  Tuplet initial_values[] = {
    TupletCString(SUMMARY_NOW, "Loading..."),
    TupletCString(SUMMARY_HOUR, "Loading..."),
    TupletCString(SUMMARY_DAY, "Loading..."),
    TupletCString(SUMMARY_WEEK, "Loading..."),
  };

  // Begin using AppSync
  app_sync_init(&s_sync, s_sync_buffer, sizeof(s_sync_buffer), initial_values, ARRAY_LENGTH(initial_values), sync_changed_handler, sync_error_handler, NULL);  
  
  // Register with TickTimerService
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);  
  
  secs_in_minute = (time(NULL) % 60);
  
  // Make sure the time is displayed from the start
  update_time();    
}

static void deinit(void) {
  hide_face();

  app_sync_deinit(&s_sync);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}