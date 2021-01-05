#include <stdio.h>
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
  int height, width;
} t_ltbl;

void ltbl_free(t_ltbl *x)
{
  led_matrix_delete(x->matrix);
}

void str_replace_char(char *s, char c, char r)
{
  size_t n = strlen(s);
  for (size_t i=0; i<n; i++) if (s[i] == c) s[i] = r;
}

void ltbl_destroy(t_ltbl *x)
{
  // do nothing without initizialisation
  if (x->matrix == NULL)
  {
    error("Matrix not initialized.");
    return;
  }
  led_matrix_delete(x->matrix);
  x->matrix = NULL;
  if (x->matrix != NULL) printf("matrix not null");
}

void ltbl_init(t_ltbl *x, t_symbol *s, int argc, t_atom *argv)
{
  // tear down old matrix before creating new
  ltbl_destroy(x);

  char **argv_char = malloc(sizeof(char*) * (argc + 1));
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
    fprintf(stderr, "%s\n", argv_char[i]);
  }

  // dont drop root priviliges after initializig, so one can initialize
  // more matrices
  argv_char[argc] = "--led-no-drop-privs";
  post(argv_char[argc]);
  argc++;

  memset(&x->options, 0, sizeof(x->options));
  x->matrix = led_matrix_create_from_options(&x->options, &argc, &argv_char);
  if (x->matrix == NULL)
  {
    error("Could not create matrix :/");
    return;
  }

  x->offscreen_canvas = led_matrix_create_offscreen_canvas(x->matrix);

  led_canvas_get_size(x->offscreen_canvas, &x->width, &x->height);

  fprintf(stderr, "Size: %dx%d. Hardware gpio mapping: %s\n",
          x->width, x->height, x->options.hardware_mapping);
}

void ltbl_list(t_ltbl *x, t_symbol *s, int argc, t_atom *argv)
{
  int with_alpha = 0;

  // do nothing without initizialisation
  if (x->matrix == NULL)
  {
    error("Matrix not initialized.");
    return;
  }

  if (argc == x->width * x->height * 4)
  {
    with_alpha = 1;
  }
  else if (argc == x->width * x->height * 3)
  {
    with_alpha = 0;
  }
  else
  {
    pd_error(x, "Wrong number of pixels: %i != (%i x %i x 4) or (%i x %i x 3)", argc, x->width, x->height, x->width, x->height);
  }

  int i;
  uint8_t r,g,b;
  for (int ix=0; ix<x->width; ix++)
  {
    for (int iy=0; iy<x->height; iy++)
    {
      i = (3 + with_alpha) * (ix + iy * x->width);

      if (with_alpha && (atom_getint(argv + i + 3) == 0)) r = g = b = 0; // how to handle transparency? right now set to black.
      else
      {
        r = atom_getint(argv + i);
	      g = atom_getint(argv + i + 1);
        b = atom_getint(argv + i + 2);
      }

      led_canvas_set_pixel(x->offscreen_canvas, ix, iy, r, g, b);
    }
  }
  x->offscreen_canvas = led_matrix_swap_on_vsync(x->matrix, x->offscreen_canvas);
}

// draw test image
void ltbl_test(t_ltbl *x)
{
    // do nothing without initizialisation
    if (x->matrix == NULL)
    {
      error("Matrix not initialized.");
      return;
    }

    led_canvas_clear(x->offscreen_canvas);

    led_canvas_set_pixel(x->offscreen_canvas, 1, 0, 255, 255, 255);
    led_canvas_set_pixel(x->offscreen_canvas, 0, 0, 255, 255, 255);
    led_canvas_set_pixel(x->offscreen_canvas, 0, 1, 255, 255, 255);

    led_canvas_set_pixel(x->offscreen_canvas, x->width - 1, 0, 255, 255, 255);
    led_canvas_set_pixel(x->offscreen_canvas, x->width - 1, 1, 255, 255, 255);
    led_canvas_set_pixel(x->offscreen_canvas, x->width - 2, 0, 255, 255, 255);

    led_canvas_set_pixel(x->offscreen_canvas, 0, x->height - 1, 255, 255, 255);
    led_canvas_set_pixel(x->offscreen_canvas, 1, x->height - 1, 255, 255, 255);
    led_canvas_set_pixel(x->offscreen_canvas, 0, x->height - 2, 255, 255, 255);

    led_canvas_set_pixel(x->offscreen_canvas, x->width - 1 , x->height - 1 , 255, 255, 255);
    led_canvas_set_pixel(x->offscreen_canvas, x->width - 2 , x->height - 1 , 255, 255, 255);
    led_canvas_set_pixel(x->offscreen_canvas, x->width - 1 , x->height - 2 , 255, 255, 255);

    x->offscreen_canvas = led_matrix_swap_on_vsync(x->matrix, x->offscreen_canvas);
}

void ltbl_clear(t_ltbl *x)
{
  // do nothing without initizialisation
  if (x->matrix == NULL)
  {
    error("Matrix not initialized.");
    return;
  }
  led_canvas_clear(x->offscreen_canvas);
  x->offscreen_canvas = led_matrix_swap_on_vsync(x->matrix, x->offscreen_canvas);
}

void *ltbl_new(t_symbol *s, int argc, t_atom *argv)
{

  t_ltbl *x = (t_ltbl *)pd_new(ltbl_class);
  x->matrix = NULL;

  ltbl_init(x, s, argc, argv);

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
  class_addmethod(ltbl_class, (t_method)ltbl_destroy, gensym("destroy"), 0);
  class_addmethod(ltbl_class, (t_method)ltbl_clear, gensym("clear"), 0);
  class_addmethod(ltbl_class, (t_method)ltbl_test, gensym("test"), 0);
}


