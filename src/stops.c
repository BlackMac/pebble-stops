#include <pebble.h>

static Window *window;
static TextLayer *text_layer;

enum {
            AKEY_NUMBER,
            AKEY_TEXT,
        };

// static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
//   text_layer_set_text(text_layer, "Select");
// }

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  DictionaryIterator *iter;
 app_message_outbox_begin(&iter);
 Tuplet value = TupletInteger(1, -1);
 dict_write_tuplet(iter, &value);
 app_message_outbox_send();
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
 DictionaryIterator *iter;
 app_message_outbox_begin(&iter);
 Tuplet value = TupletInteger(1, 1);
 dict_write_tuplet(iter, &value);
 app_message_outbox_send();
}

static void click_config_provider(void *context) {
  // window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

 void in_received_handler(DictionaryIterator *received, void *context) {
  Tuple *text_tuple = dict_find(received, AKEY_NUMBER);
  if (text_tuple) {
    text_layer_set_text(text_layer, text_tuple->value->cstring);
    //APP_LOG(APP_LOG_LEVEL_DEBUG, "Text: %s", text_tuple->value->cstring);
  }
   
 }

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  text_layer = text_layer_create((GRect) { .origin = { 0, 6 }, .size = { bounds.size.w, 60 } });
  text_layer_set_text(text_layer, "Haltestelle suchen...");
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
}

static void window_unload(Window *window) {
  text_layer_destroy(text_layer);
}

static void init(void) {
  app_message_register_inbox_received(in_received_handler); 

  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);

  const uint32_t inbound_size = 64;
  const uint32_t outbound_size = 64;
  app_message_open(inbound_size, outbound_size);

}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();
}
