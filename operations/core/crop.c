#ifdef CHANT_SELF

chant_float (x,      -123434.0,  1234134.0,  0.0, "left most pixel coordinate")
chant_float (y,      -121212.0,  1234134.0,  0.0, "top pixel coordinate")
chant_float (width,  -123434.0,  1234134.0,  10.0, "width in pixels")
chant_float (height, -121212.0,  1234134.0,  10.0, "height in pixels")

#else

#define CHANT_FILTER
#define CHANT_NAME            crop
#define CHANT_SELF            "crop.c"
#define CHANT_DESCRIPTION     "crops the image, can be used to rectangulary" \
                              "clip buffers, as well as specifying what " \
                              "portion of a composition to render to file"
#define CHANT_CLASS_CONSTRUCT
#include "gegl-chant.h"


#include <stdio.h>

int chant_foo = 0;

/* Actual image processing code
 ************************************************************************/
static gboolean
evaluate (GeglOperation *operation,
          const gchar   *output_prop)
{
  GeglOperationFilter    *filter = GEGL_OPERATION_FILTER(operation);
  GeglBuffer  *input  = filter->input;
  ChantInstance *crop = CHANT_INSTANCE (operation);

  if(strcmp("output", output_prop))
    return FALSE;

  if (filter->output)
    g_object_unref (filter->output);

  g_assert (input);
  g_assert (gegl_buffer_get_format (input));

  filter->output = g_object_new (GEGL_TYPE_BUFFER,
                                 "source",       input,
                                 "x",     (int)crop->x,
                                 "y",     (int)crop->y,
                                 "width",  (int)crop->width,
                                 "height", (int)crop->height,
                                 NULL);
  crop = NULL;
  return  TRUE;
}

static gboolean
calc_have_rect (GeglOperation *operation)
{
  ChantInstance  *op_crop = (ChantInstance*)(operation);
  GeglRect *in_rect = gegl_operation_get_have_rect (operation, "input");
  if (!in_rect)
    return FALSE;

  gegl_operation_set_have_rect (operation,
     op_crop->x, op_crop->y,
     op_crop->width, op_crop->height);
  return TRUE;
}

static gboolean
calc_need_rect (GeglOperation *self)
{
  GeglRect *requested    = gegl_operation_need_rect (self);

  gegl_operation_set_need_rect (self, "input",
     requested->x,
     requested->y,
     requested->w, requested->h);
  return TRUE;
}

static void class_init (GeglOperationClass *operation_class)
{
  operation_class->calc_have_rect = calc_have_rect;
  operation_class->calc_need_rect = calc_need_rect;
}

#endif
