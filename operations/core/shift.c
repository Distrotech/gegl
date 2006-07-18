#ifdef CHANT_SELF
 
chant_float (x,   -123434.0,  1234134.0,  0.0, "x coordinate of new origin")
chant_float (y,   -121212.0,  1234134.0,  0.0, "y coordinate of new origin")

#else

#define CHANT_FILTER
#define CHANT_NAME            shift
#define CHANT_DESCRIPTION     "Translate the buffer, an integer amount of pixels."
#define CHANT_SELF            "shift.c"
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
  GeglBuffer    *input  = filter->input;
  ChantInstance *translate = (ChantInstance*)filter;

  if(strcmp("output", output_prop))
    return FALSE;

  if (filter->output)
    g_object_unref (filter->output);

  g_assert (input);
  g_assert (gegl_buffer_get_format (input));

  filter->output = g_object_new (GEGL_TYPE_BUFFER,
                                 "source",       input,
                                 "shift-x",     (int)-translate->x,
                                 "shift-y",     (int)-translate->y,
                                 "abyss-width", -1,  /* turn of abyss (relying
                                                        on abyss of source) */
                                 NULL);
  translate = NULL;
  return  TRUE;
}

static gboolean
calc_have_rect (GeglOperation *operation)
{
  ChantInstance  *op_shift = (ChantInstance*)(operation);
  GeglRect *in_rect = gegl_operation_get_have_rect (operation, "input");
  if (!in_rect)
    return FALSE;

  gegl_operation_set_have_rect (operation, 
     in_rect->x + op_shift->x,
     in_rect->y + op_shift->y,
     in_rect->w, in_rect->h);
  return TRUE;
}

static gboolean
calc_need_rect (GeglOperation *self)
{
  ChantInstance  *op_shift = (ChantInstance*)(self);
  GeglRect *requested    = gegl_operation_need_rect (self);

  gegl_operation_set_need_rect (self, "input",
     requested->x - op_shift->x,
     requested->y - op_shift->y,
     requested->w, requested->h);
  return TRUE;
}

static void class_init (GeglOperationClass *operation_class)
{
  operation_class->calc_have_rect = calc_have_rect;
  operation_class->calc_need_rect = calc_need_rect;
}

#endif
