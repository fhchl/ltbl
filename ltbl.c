#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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
  led_matrix_delete(x->matrix);
}


void *ltbl_new(t_symbol *s, int argc, t_atom *argv)
{
  char **argv_char;
  t_ltbl *x = (t_ltbl *)pd_new(ltbl_class);

  memset(&x->options, 0, sizeof(x->options));
  x->options.rows = 32;
  x->options.chain_length = 1;

  for (int i=0; i<argc; i++)
  {
    argv_char[i] = strdup(atom_getsymbol(argv+i)->s_name);
    post(argv_char[i]);
  }

  /* This supports all the led commandline options. Try --led-help */
  x->matrix = led_matrix_create_from_options(&x->options, &argc, &argv_char);
  if (x->matrix == NULL)
    error("Could not create matrix :/");

  /* Let's do an example with double-buffering. We create one extra
   * buffer onto which we draw, which is then swapped on each refresh.
   * This is typically a good aproach for animations and such.
   */
  x->offscreen_canvas = led_matrix_create_offscreen_canvas(x->matrix);

  led_canvas_get_size(x->offscreen_canvas, &x->width, &x->height);

  fprintf(stderr, "Size: %dx%d. Hardware gpio mapping: %s\n",
          x->width, x->height, x->options.hardware_mapping);

  return (void *)x;
}


void ltbl_setup(void) {
  ltbl_class = class_new(gensym("ltbl"),
        (t_newmethod)ltbl_new,
        (t_method)ltbl_free,
        sizeof(t_ltbl),
        CLASS_DEFAULT,
        A_GIMME,
        0); 
}
