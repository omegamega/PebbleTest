#include "pebble.h"

static Window *window;

static BitmapLayer *image_layer;
static BitmapLayer *image_layer2;

static TextLayer *text_layer;

static GBitmap *image;
static GBitmap *image2;

// handle Tick, and flash bg
void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{
	if(layer_get_hidden((Layer*)text_layer) == false)
	{
		//Allocate long-lived storage (required by TextLayer)
		static char buffer[] = "00:00";

		//Write the time to the buffer in a safe manner
		strftime(buffer, sizeof(buffer), "%H:%M", tick_time);

		//Set the TextLayer to display the buffer
		text_layer_set_text(text_layer, buffer);
	}
	else
	{
		if(layer_get_hidden((Layer*)image_layer2) == false){
			layer_set_hidden((Layer*)image_layer2, true);
		}else{
			layer_set_hidden((Layer*)image_layer2, false);	
		}
	}
}

// handle tap, and show number face 
void tap_hide_text_timer_callback(void* data) {
	layer_set_hidden((Layer*)text_layer,true);
	light_enable(false);
}

void tap_handler(AccelAxisType axis, int32_t direction)
{
	layer_set_hidden((Layer*)text_layer, false);
	light_enable(true);	
	
	layer_set_hidden((Layer*)image_layer2, false);

	app_timer_register(3000, tap_hide_text_timer_callback, NULL);	
}

int main(void) {
  window = window_create();
	window_set_fullscreen(window, true);
  window_stack_push(window, true /* Animated */);

  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);

  // This needs to be deinited on app exit which is when the event loop ends
  image = gbitmap_create_with_resource(RESOURCE_ID_SAMPLE);
  image2 = gbitmap_create_with_resource(RESOURCE_ID_SAMPLE2);

  // The bitmap layer holds the image for display
  image_layer = bitmap_layer_create(bounds);
  bitmap_layer_set_bitmap(image_layer, image);
  bitmap_layer_set_alignment(image_layer, GAlignCenter);
  layer_add_child(window_layer, bitmap_layer_get_layer(image_layer));
	
  image_layer2 = bitmap_layer_create(bounds);
  bitmap_layer_set_bitmap(image_layer2, image2);
  bitmap_layer_set_alignment(image_layer2, GAlignCenter);
  layer_add_child(window_layer, bitmap_layer_get_layer(image_layer2));
	
  //Create the TextLayer
  text_layer = text_layer_create(GRect(0, 0, 144, 50));
  text_layer_set_background_color(text_layer, GColorClear);
  text_layer_set_text_color(text_layer, GColorBlack);

  text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));
  text_layer_set_text_alignment(text_layer, GTextAlignmentLeft);
	layer_set_hidden((Layer*)text_layer, true);

  layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer));
	
	// set timer
	tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
	accel_tap_service_subscribe(tap_handler);
	
  app_event_loop();

  gbitmap_destroy(image);
  gbitmap_destroy(image2);

  bitmap_layer_destroy(image_layer);
  bitmap_layer_destroy(image_layer2);
	text_layer_destroy(text_layer);
  window_destroy(window);
}
