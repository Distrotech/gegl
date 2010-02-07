#include <stdlib.h>
#include <glib.h>
#include <gegl.h>

static long ticks_start;

glong gegl_ticks (void);

void test_start (void)
{
  ticks_start = gegl_ticks ();
}

void test_end (const gchar *id,
               glong        bytes)
{
  long ticks = gegl_ticks ()-ticks_start;
  g_print ("@ %s: %.2f megabytes/second\n",
       id, (bytes / 1024.0 / 1024.0)  / (ticks / 1000000.0));
}

GeglBuffer *test_buffer (gint width,
                         gint height,
                         Babl *format)
{
  GeglRectangle bound = {0, 0, width, height};
  GeglBuffer *buffer;
  gfloat *buf = g_malloc0 (width * height * 16);
  gint i;
  buffer = gegl_buffer_new (&bound, format);
  for (i=0; i < width * height * 4; i++)
    buf[i] = g_random_double_range (-0.5, 2.0);
  gegl_buffer_set (buffer, NULL, babl_format ("RGBA float"), buf, 0);
  g_free (buf);
  return buffer;
}

