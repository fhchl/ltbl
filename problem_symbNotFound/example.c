/* -*- mode: c; c-basic-offset: 2; indent-tabs-mode: nil; -*-
 *
 * Using the C-API of this library.
 *
 */
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#include "m_pd.h"
#include "led-matrix-c.h"

static t_class *ltbl_class;

typedef struct _ltbl {   
  t_object  x_obj;
  struct RGBLedMatrixOptions options;
  struct RGBLedMatrix *matrix;
  struct LedCanvas *offscreen_canvas;
  int width, height;
  int x, y, i;
} t_ltbl;


void ltbl_free(t_ltbl *x)
{
  //led_matrix_delete(x->matrix); 
}

void str_replace_char(char *s, char c, char r)
{
  size_t n = strlen(s);
  for (size_t i=0; i<n; i++) if (s[i] == c) s[i] = r;
}


void ltbl_init(t_ltbl *x, t_symbol *s, int argc, t_atom *argv)
{
  char **argv_char = malloc(sizeof(char*) * argc);
  
  for (int i=0; i<argc; i++)
  {
    //~ post(strdup(atom_getsymbol(argv+i)->s_name));
    argv_char[i] = strdup(atom_getsymbol(argv+i)->s_name);
    if (argv_char[i] == NULL)
    {
      error("Could not parse symbols :/");
      return;
    }
    str_replace_char(argv_char[i], '!', ';') ;
    post(argv_char[i]);
  }
  
  memset(&x->options, 0, sizeof(x->options));
   
  /* This supports all the led commandline options. Try --led-help */
  x->matrix = led_matrix_create_from_options(&x->options, &argc, &argv_char);
  //~ if (x->matrix == NULL) 
  //~ {
    //~ error("Could not create matrix :/");
    //~ return;
  //~ }

  //~ /* Let's do an example with double-buffering. We create one extra
   //~ * buffer onto which we draw, which is then swapped on each refresh.
   //~ * This is typically a good aproach for animations and such.
   //~ */
  //~ x->offscreen_canvas = led_matrix_create_offscreen_canvas(x->matrix);

  //~ led_canvas_get_size(x->offscreen_canvas, &x->width, &x->height);

  //~ fprintf(stderr, "Size: %dx%d. Hardware gpio mapping: %s\n",
          //~ x->width, x->height, x->options.hardware_mapping);
}

void ltbl_list(t_ltbl *x, t_symbol *s, int argc, t_atom *argv) {}

void *ltbl_new(t_symbol *s, int argc, t_atom *argv)
{
  
  t_ltbl *x = (t_ltbl *)pd_new(ltbl_class);
  
  // ltbl_init(x, s, argc, argv);
  
  return (void *)x;
}

void ltbl_setup(void) {

  if (geteuid() != 0)
  {
    error("ltbl: must run as super user to access rpi-rgb-led-matrix API.");
    return;
  }
  else post("ltbl: running as super user");
  
  ltbl_class = class_new(gensym("ltbl"),
        (t_newmethod)ltbl_new,
        (t_method)ltbl_free,
        sizeof(t_ltbl),
        CLASS_DEFAULT,
        A_GIMME,
        0);
  class_addmethod(ltbl_class, (t_method)ltbl_init, gensym("init"), A_GIMME, 0);
  class_addlist(ltbl_class, (t_method)ltbl_list);
}

int main(int argc, char **argv) {
  printf("lubb");
  struct RGBLedMatrixOptions options;
  struct RGBLedMatrix *matrix;
  struct LedCanvas *offscreen_canvas;
  int width, height;
  int x, y, i;

  memset(&options, 0, sizeof(options));
  options.rows = 32;
  options.chain_length = 1;

  /* This supports all the led commandline options. Try --led-help */
  matrix = led_matrix_create_from_options(&options, &argc, &argv);
  if (matrix == NULL)
    return 1;

  //~ /* Let's do an example with double-buffering. We create one extra
   //~ * buffer onto which we draw, which is then swapped on each refresh.
   //~ * This is typically a good aproach for animations and such.
   //~ */
  //~ offscreen_canvas = led_matrix_create_offscreen_canvas(matrix);

  //~ led_canvas_get_size(offscreen_canvas, &width, &height);

  //~ fprintf(stderr, "Size: %dx%d. Hardware gpio mapping: %s\n",
          //~ width, height, options.hardware_mapping);

  //~ for (i = 0; i < 1000; ++i) {
    //~ for (y = 0; y < height; ++y) {
      //~ for (x = 0; x < width; ++x) {
        //~ led_canvas_set_pixel(offscreen_canvas, x, y, i & 0xff, x, y);
      //~ }
    //~ }

    //~ /* Now, we swap the canvas. We give swap_on_vsync the buffer we
     //~ * just have drawn into, and wait until the next vsync happens.
     //~ * we get back the unused buffer to which we'll draw in the next
     //~ * iteration.
     //~ */
    //~ offscreen_canvas = led_matrix_swap_on_vsync(matrix, offscreen_canvas);
  //~ }

  //~ /*
   //~ * Make sure to always call led_matrix_delete() in the end to reset the
   //~ * display. Installing signal handlers for defined exit is a good idea.
   //~ */
  //~ led_matrix_delete(matrix);

  //~ return 0;
}
