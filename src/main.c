#include <pebble.h>

#define KEY_COLOR_RED     0
#define KEY_COLOR_GREEN   1
#define KEY_COLOR_BLUE    2
#define KEY_HIGH_CONTRAST 3

static Window *s_main_window;
static TextLayer *s_text_layer;

static void inbox_received_handler(DictionaryIterator *iter, void *context) {
  // Animated?
  Tuple *high_contrast_t = dict_find(iter, KEY_HIGH_CONTRAST);
  if(high_contrast_t && t->value->int32 > 0) {
    // Change color scheme
    window_set_background_color(s_main_window, GColorWhite);
    text_layer_set_text_color(s_text_layer, GColorBlack);

    // Persist value
    persist_write_bool(KEY_HIGH_CONTRAST, true);
  }

  // Color scheme?
  Tuple *color_red_t = dict_find(iter, KEY_COLOR_RED);
  if(color_red_t) {
    
  }
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_text_layer = text_layer_create(GRect(0, 50, bounds.size.w, 100));
  text_layer_set_text(s_text_layer, "This is some example text!");
  text_layer_set_font(s_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(s_text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_text_layer));
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
