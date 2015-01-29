/* This file is an image processing operation for GEGL
 *
 * GEGL is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * GEGL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with GEGL; if not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2015 Red Hat, Inc.
 */

#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES

/* This operation has no properties */

#else

#define GEGL_OP_POINT_FILTER
#define GEGL_OP_C_FILE "insta-brannan.c"

#include "gegl-op.h"
#include <stdio.h>

static const guint8 brannan_a[] =
  {
    0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,
    27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,
    51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,
    75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,
    99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,
    117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,
    135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,
    153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,
    171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,
    189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,
    207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,
    225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,
    243,244,245,246,247,248,249,250,251,252,253,254,255,255
  };

static const guint8 brannan_r[] =
  {
    50,50,50,50,50,50,50,50,50,50,50,50,50,51,51,51,51,51,52,53,54,55,56,57,
    59,60,62,63,64,66,67,68,69,70,71,71,72,73,73,74,75,75,76,76,77,77,78,78,
    79,79,80,80,81,81,82,83,83,84,85,86,87,88,88,89,90,91,92,93,94,95,96,97,
    98,99,100,101,102,103,104,105,106,107,108,109,111,112,113,114,115,116,
    118,119,120,121,122,124,125,126,128,129,130,132,133,134,136,137,139,140,
    141,143,144,146,147,149,150,152,153,154,156,157,159,160,162,163,164,166,
    167,169,170,171,173,174,175,177,178,179,181,182,183,185,186,187,189,190,
    192,193,195,196,198,199,201,203,204,206,207,209,210,212,213,215,216,217,
    219,220,221,223,224,225,226,227,228,229,230,231,232,233,234,235,236,236,
    237,238,239,239,240,241,241,242,243,243,244,244,245,246,246,247,247,248,
    248,249,249,249,250,250,251,251,251,252,252,252,253,253,253,254,254,254,
    254,254,254,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,254,
    254,254,254,254,254
  };

static const guint8 brannan_g[] =
  {
    0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,2,2,2,3,4,4,5,6,7,8,10,11,12,13,14,16,17,
    18,19,20,21,23,24,25,26,27,28,29,30,32,33,34,35,36,38,39,40,41,43,44,45,
    47,48,50,51,53,54,56,57,59,61,62,64,66,68,70,72,74,76,78,80,82,84,87,89,
    91,93,95,97,100,102,104,106,108,110,112,114,116,118,120,122,124,126,128,
    130,132,134,136,138,140,142,144,146,148,150,152,154,156,158,160,161,163,
    165,167,168,170,172,173,175,176,178,179,181,182,183,184,186,187,188,189,
    190,191,192,193,193,194,195,196,196,197,198,198,199,200,200,201,202,202,
    203,203,204,204,205,205,206,207,207,208,208,209,210,210,211,212,212,213,
    214,214,215,216,217,217,218,219,219,220,221,221,222,222,223,224,224,225,
    225,226,226,227,228,228,229,229,229,230,230,231,231,232,232,233,233,233,
    234,234,234,235,235,236,236,236,237,237,237,238,238,239,239,239,240,240,
    240,241,241,241,242,242,242,243,243,243,244,244,244,245,245,245,246,246,
    247,247,247,248,248,249,249,250,250,251,251,252,252,252
  };

static const guint8 brannan_b[] =
  {
    48,48,48,48,48,48,48,48,49,49,49,49,49,49,49,50,50,50,51,51,51,52,52,53,
    53,54,54,54,55,55,56,56,57,57,58,58,59,60,60,61,61,62,62,63,64,64,65,66,
    66,67,68,68,69,70,71,71,72,73,74,75,75,76,77,78,79,80,81,82,83,84,85,86,
    87,88,89,90,92,93,94,95,96,98,99,100,101,102,103,105,106,107,108,109,110,
    111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,
    129,130,131,132,132,133,134,135,136,137,138,139,140,141,141,142,143,144,
    145,146,146,147,148,148,149,150,151,151,152,153,154,155,156,157,158,159,
    160,161,162,163,164,165,166,168,169,170,171,172,173,174,175,176,177,178,
    178,179,180,181,181,182,183,183,184,184,185,185,185,186,186,187,187,187,
    188,188,188,189,189,190,190,191,191,192,193,193,194,195,195,196,197,198,
    199,200,200,201,202,203,204,205,206,206,207,208,209,210,211,211,212,213,
    214,214,215,216,216,217,218,218,219,219,220,220,221,222,222,222,223,223,
    224,224,224,225,225,225,225,225,225,225,225,225,225,225,225,225,225,225,
    225,225
  };

static void
gegl_insta_brannan_prepare (GeglOperation *operation)
{
  gegl_operation_set_format (operation, "input",
                             babl_format ("R'G'B' u8"));
  gegl_operation_set_format (operation, "output",
                             babl_format ("R'G'B' u8"));
}

static gboolean
gegl_insta_brannan_process (GeglOperation       *operation,
                            void                *in_buf,
                            void                *out_buf,
                            glong                n_pixels,
                            const GeglRectangle *roi,
                            gint                 level)
{
  guint8 *dest = out_buf;
  guint8 *src = in_buf;
  glong   i;

  for (i = 0; i < n_pixels; i++)
    {
      const gfloat level = 0.1;
      const glong offset = i * 3;
      guint max;

      dest[offset + 0] = brannan_r[src[offset + 0]];
      dest[offset + 1] = brannan_g[src[offset + 1]];
      dest[offset + 2] = brannan_b[src[offset + 2]];

      dest[offset + 0] = brannan_a[dest[offset + 0]];
      dest[offset + 1] = brannan_a[dest[offset + 1]];
      dest[offset + 2] = brannan_a[dest[offset + 2]];

      max = (dest[offset + 0] > dest[offset + 1]) ? 0 : 1;
      max = (dest[offset + max] > dest[offset + 2]) ? max : 2;

      if (max != 0)
        dest[offset + 0] +=
          (guint8) ((dest[offset + max] - dest[offset + 0]) * level + 0.5);

      if (max != 1)
        dest[offset + 1] +=
          (guint8) ((dest[offset + max] - dest[offset + 1]) * level + 0.5);

      if (max != 2)
        dest[offset + 2] +=
          (guint8) ((dest[offset + max] - dest[offset + 2]) * level + 0.5);
    }

  return TRUE;
}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass            *operation_class;
  GeglOperationPointFilterClass *point_filter_class;

  operation_class    = GEGL_OPERATION_CLASS (klass);
  point_filter_class = GEGL_OPERATION_POINT_FILTER_CLASS (klass);

  operation_class->prepare    = gegl_insta_brannan_prepare;
  point_filter_class->process = gegl_insta_brannan_process;

  gegl_operation_class_set_keys (operation_class,
    "name",        "gegl:insta-brannan",
    "title",       _("Instagram's Brannan filter"),
    "categories",  "enhance",
    NULL);
}

#endif
