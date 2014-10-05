/*

   Demonstrate how to display a two color, black and white bitmap
   image with no transparency.

   The original source image is from:

      <http://openclipart.org/detail/26728/aiga-litter-disposal-by-anonymous>

   The source image was converted from an SVG into a RGB bitmap using
   Inkscape. It has no transparency and uses only black and white as
   colors.

 */

#include "pebble.h"

static Window *window;

static BitmapLayer *image_layer;

static GBitmap *image;
static GBitmap *image2;

static AppTimer *timer;
static int count = 0;

void timer_callback(void *data){
	count++;
	APP_LOG(APP_LOG_LEVEL_DEBUG, "count %d", count);
	
	if(count % 2) {
		bitmap_layer_set_bitmap(image_layer, image);
	}else{
		bitmap_layer_set_bitmap(image_layer, image2);
	}
	timer = app_timer_register(500, timer_callback, NULL);
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
	
	timer = app_timer_register(500, timer_callback, NULL);
	
  app_event_loop();

  gbitmap_destroy(image);
  gbitmap_destroy(image2);

  bitmap_layer_destroy(image_layer);
  window_destroy(window);
}
