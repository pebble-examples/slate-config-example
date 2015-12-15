#include <pebble.h>

#define KEY_COLOR_RED     0
#define KEY_COLOR_GREEN   1
#define KEY_COLOR_BLUE    2
#define KEY_HIGH_CONTRAST 3

static Window *s_main_window;
static TextLayer *s_text_layer;

static bool gcolor_is_dark(GColor color) {
#if defined(PBL_BW)
  return gcolor_equal(color, GColorBlack);
#elif defined(PBL_COLOR)
  return color.r < 2 && color.g < 2 && color.b < 2;
#endif
}

static void inbox_received_handler(DictionaryIterator *iter, void *context) {
  // High contrast selected?
  Tuple *high_contrast_t = dict_find(iter, KEY_HIGH_CONTRAST);
  if(high_contrast_t && high_contrast_t->value->int8 > 0) {  // Read boolean as an integer
    // Change color scheme
    window_set_background_color(s_main_window, GColorBlack);
    text_layer_set_text_color(s_text_layer, GColorWhite);

    // Persist value
    persist_write_bool(KEY_HIGH_CONTRAST, true);
  } else {
    persist_write_bool(KEY_HIGH_CONTRAST, false);
  }

  // Color scheme?
  Tuple *color_red_t = dict_find(iter, KEY_COLOR_RED);
  Tuple *color_green_t = dict_find(iter, KEY_COLOR_GREEN);
  Tuple *color_blue_t = dict_find(iter, KEY_COLOR_BLUE);
  if(color_red_t && color_green_t && color_blue_t) {
    // Apply the color if available
#if defined(PBL_BW)
    window_set_background_color(s_main_window, GColorWhite);
    text_layer_set_text_color(s_text_layer, GColorBlack);
#elif defined(PBL_COLOR)
    int red = color_red_t->value->int32;
    int green = color_green_t->value->int32;
    int blue = color_blue_t->value->int32;

    // Persist values
    persist_write_int(KEY_COLOR_RED, red);
    persist_write_int(KEY_COLOR_GREEN, green);
    persist_write_int(KEY_COLOR_BLUE, blue);

    GColor bg_color = GColorFromRGB(red, green, blue);
    window_set_background_color(s_main_window, bg_color);
    text_layer_set_text_color(s_text_layer, gcolor_is_dark(bg_color) ? GColorWhite : GColorBlack);
#endif
  }
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_text_layer = text_layer_create(GRect(0, 50, bounds.size.w, 100));
  text_layer_set_text(s_text_layer, "App configuration choices will be reflected here!");
  text_layer_set_font(s_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(s_text_layer, GTextAlignmentCenter);
  text_layer_set_background_color(s_text_layer, GColorClear);
  layer_add_child(window_layer, text_layer_get_layer(s_text_layer));

  // Read saved config
  if(persist_read_bool(KEY_HIGH_CONTRAST)) {
    // Apply high contrast mode
    window_set_background_color(s_main_window, GColorBlack);
    text_layer_set_text_color(s_text_layer, GColorWhite);
  } else {
#if defined(PBL_BW)
    // Not available, use normal colors
#elif defined(PBL_COLOR)
    // Use background color setting
    int red = persist_read_int(KEY_COLOR_RED);
    int green = persist_read_int(KEY_COLOR_GREEN);
    int blue = persist_read_int(KEY_COLOR_BLUE);

    GColor bg_color = GColorFromRGB(red, green, blue);
    window_set_background_color(s_main_window, bg_color);
    text_layer_set_text_color(s_text_layer, gcolor_is_dark(bg_color) ? GColorWhite : GColorBlack);
#endif
  }
}

static void window_unload(Window *window) {
  text_layer_destroy(s_text_layer);
}

static void init() {
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  window_stack_push(s_main_window, true);

  app_message_register_inbox_received(inbox_received_handler);
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
}

static void deinit() {
  window_destroy(s_main_window);
}

int main() {
  init();
  app_event_loop();
  deinit();
}
