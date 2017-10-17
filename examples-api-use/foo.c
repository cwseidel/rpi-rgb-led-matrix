/* -*- mode: c; c-basic-offset: 2; indent-tabs-mode: nil; -*-
 *
 * Using the C-API of this library.
 *
 */
#include "led-matrix-c.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {
  struct RGBLedMatrixOptions options;
  struct RGBLedMatrix *matrix;
  struct LedCanvas *offscreen_canvas;
  int width, height;
  int x, y, i, j, k, d;

  memset(&options, 0, sizeof(options));
  options.rows = 32;
  options.chain_length = 1;

  /* This supports all the led commandline options. Try --led-help */
  matrix = led_matrix_create_from_options(&options, &argc, &argv);
  if (matrix == NULL)
    return 1;

  /* Let's do an example with double-buffering. We create one extra
   * buffer onto which we draw, which is then swapped on each refresh.
   * This is typically a good aproach for animations and such.
   */
  offscreen_canvas = led_matrix_create_offscreen_canvas(matrix);

  led_canvas_get_size(offscreen_canvas, &width, &height);

  fprintf(stderr, "Size: %dx%d. Hardware gpio mapping: %s\n",
          width, height, options.hardware_mapping);

  j = 0;
  k = 0;
  for (i = 0; i < 1000; ++i) {
    for (y = 0; y < height; ++y) {
      for (x = 0; x < width; ++x) {
        led_canvas_set_pixel(offscreen_canvas, x, y, i & 0xff, x, y);
      }
    }

    if( d == 0 ){
      if(j > 0){
        --j;
      }
      else{
        d = change_direction(d);
      }
    }
    if( d == 1 ){ 
      if( k > 0 ){
        --k;
      }
      else{
        d = change_direction(d);       
      }
    }
    if( d == 2 ){
      if( j < height ){
        ++j; 
      }
      else{
        d = change_direction(d);
      }
    }
    if( d == 3 ){
      if( k < width ){
        ++k;
      }
      else{
        d = change_direction(d);
      }
    }
    
    led_canvas_set_pixel(offscreen_canvas, j, k, 100, 100, 100);
    /* Now, we swap the canvas. We give swap_on_vsync the buffer we
     * just have drawn into, and wait until the next vsync happens.
     * we get back the unused buffer to which we'll draw in the next
     * iteration.
     */
    offscreen_canvas = led_matrix_swap_on_vsync(matrix, offscreen_canvas);
  }

  /*
   * Make sure to always call led_matrix_delete() in the end to reset the
   * display. Installing signal handlers for defined exit is a good idea.
   */
  led_matrix_delete(matrix);

  return 0;
}
int change_direction(int current_direction){
  int n, new_direction;
  int direction_array[3];
  
  if(current_direction == 0){
    direction_array[0] = 1;
    direction_array[1] = 2;
    direction_array[2] = 3;
  }
  if(current_direction == 1){
    direction_array[0] = 0;
    direction_array[1] = 2;
    direction_array[2] = 3;
  }
  if(current_direction == 2){
    direction_array[0] = 0;
    direction_array[1] = 1;
    direction_array[2] = 3;
  }
  if(current_direction == 3){
    direction_array[0] = 0;
    direction_array[1] = 1;
    direction_array[2] = 2;
  }
  n = rand() % 3;
  new_direction = direction_array[n];
  return(new_direction);
}
