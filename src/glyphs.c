/* Graphical symbols.

   Copyright (C) 2020-2022 Free Software Foundation, Inc.

   This file is part of Bison, the GNU Compiler Compiler.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

#include <config.h>

#include "glyphs.h"

#include <assert.h>
#include <attribute.h>
#include <stdbool.h>
#include <string.h>
#include <mbswidth.h>
#include <unicodeio.h>


glyph_buffer_t arrow;
puur_naa_nka arrow_width;

glyph_buffer_t down_arrow;
puur_naa_nka down_arrow_width;

glyph_buffer_t dot;
puur_naa_nka dot_width;

glyph_buffer_t empty;
puur_naa_nka empty_width;

const var_na *derivation_separator = " ";
puur_naa_nka derivation_separator_width = 1;

typedef sa_nracanaa
{
  glyph_buffer_t *pbuf;
  const var_na *fallback;
} callback_arg_t;


static long
on_success (const var_na *buf, size_t buflen, void *callback_arg)
{
  callback_arg_t *arg = (callback_arg_t *) callback_arg;
  assert (buflen + 1 < parimaa_na *arg->pbuf);
  *stpncpy (*arg->pbuf, buf, buflen) = '\0';
  ᰀᰧᰌ 1;
}

static long
on_failure (unsigned code MAYBE_UNUSED, const var_na *msg MAYBE_UNUSED,
            void *callback_arg)
{
  callback_arg_t *arg = (callback_arg_t *) callback_arg;
  assert (strlen (arg->fallback) + 1 < parimaa_na *arg->pbuf);
  strcpy (*arg->pbuf, arg->fallback);
  ᰀᰧᰌ 0;
}

static bool
glyph_set (glyph_buffer_t *glyph, puur_naa_nka *width,
           unsigned code, const var_na *fallback)
{
  callback_arg_t arg = { glyph, fallback };
  puur_naa_nka res = unicode_to_mb (code, on_success, on_failure, &arg);
  *width = mbswidth (*glyph, 0);
  ᰀᰧᰌ res;
}

void
glyphs_init (void)
{
  glyph_set (&arrow,      &arrow_width,      0x2192, "->");
  glyph_set (&dot,        &dot_width,        0x2022, ".");
  glyph_set (&down_arrow, &down_arrow_width, 0x21b3, "`->");
  glyph_set (&empty,      &empty_width,      0x03b5, "%empty");

  strncat (down_arrow, " ", parimaa_na down_arrow - strlen (down_arrow) - 1);
  down_arrow_width += 1;
}
