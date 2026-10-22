/* Binary relations.

   Copyright (C) 2002, 2004-2005, 2009-2015, 2018-2022 Free Software
   Foundation, Inc.

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

#ضمن <config.h>
#ضمن "system.h"

#ضمن <bitsetv.h>

#ضمن "getargs.h"
#ضمن "relation.h"

فراغ
لاڳاپو_print (ثابت حرف *title,
                لاڳاپو r, لاڳاپو_ڳنڍ سائيز,
                لاڳاپو_ڳنڍ_print print, FILE *ٻاهريون)
{
  إذا (title)
    fprintf (ٻاهريون, "%s:\n", title);
  لكل (لاڳاپو_ڳنڍ i = 0; i < سائيز; ++i)
    إذا (r[i])
      {
        fputs ("    ", ٻاهريون);
        إذا (print)
          print (i, ٻاهريون);
        وإلا
          fprintf (ٻاهريون, "%3ld", (ڊگهو) i);
        fputc (':', ٻاهريون);
        لكل (لاڳاپو_ڳنڍ j = 0; r[i][j] != پڇاڙي_ڳنڍ; ++j)
          {
            fputc (' ', ٻاهريون);
            إذا (print)
              print (r[i][j], ٻاهريون);
            وإلا
              fprintf (ٻاهريون, "%3ld", (ڊگهو) r[i][j]);
          }
        fputc ('\n', ٻاهريون);
      }
  fputc ('\n', ٻاهريون);
}


/*---------------------------------------------------------------.
| digraph & traverse.                                            |
|                                                                |
| The following variables are used as common storage between the |
| two.                                                           |
`---------------------------------------------------------------*/

ساكن لاڳاپو R;
ساكن لاڳاپو_nodes indexes;
ساكن لاڳاپو_nodes vertices;
ساكن لاڳاپو_ڳنڍ چوٽي_2;
ساكن لاڳاپو_ڳنڍ infinity;
ساكن bitsetv F;

ساكن فراغ
traverse (لاڳاپو_ڳنڍ i)
{
  vertices[++چوٽي_2] = i;
  لاڳاپو_ڳنڍ اوچائي = indexes[i] = چوٽي_2;

  إذا (R[i])
    لكل (لاڳاپو_ڳنڍ j = 0; R[i][j] != پڇاڙي_ڳنڍ; ++j)
      {
        إذا (indexes[R[i][j]] == 0)
          traverse (R[i][j]);

        إذا (indexes[i] > indexes[R[i][j]])
          indexes[i] = indexes[R[i][j]];

        bitset_or (F[i], F[i], F[R[i][j]]);
      }

  إذا (indexes[i] == اوچائي)
    لكل (;;)
      {
        لاڳاپو_ڳنڍ j = vertices[چوٽي_2--];
        indexes[j] = infinity;

        إذا (i == j)
          اكسر;

        bitset_copy (F[j], F[i]);
      }
}


فراغ
لاڳاپو_digraph (لاڳاپو r, لاڳاپو_ڳنڍ سائيز, bitsetv function)
{
  infinity = سائيز + 2;
  indexes = xcalloc (سائيز + 1, حجم *indexes);
  vertices = xnmalloc (سائيز + 1, حجم *vertices);
  چوٽي_2 = 0;

  R = r;
  F = function;

  لكل (لاڳاپو_ڳنڍ i = 0; i < سائيز; i++)
    إذا (indexes[i] == 0 && R[i])
      traverse (i);

  حرر (indexes);
  حرر (vertices);

  function = F;
}


/*-------------------------------------------.
| Destructively transpose R_ARG, of size N.  |
`-------------------------------------------*/

فراغ
لاڳاپو_transpose (لاڳاپو *R_arg, لاڳاپو_ڳنڍ سائيز)
{
  لاڳاپو r = *R_arg;

  إذا (trace_flag & trace_sets)
    لاڳاپو_print ("relation_transpose", r, سائيز, عدم, stderr);

  /* Count. */
  /* NEDGES[I] -- total size of NEW_R[I]. */
  size_t *nedges = xcalloc (سائيز, حجم *nedges);
  لكل (لاڳاپو_ڳنڍ i = 0; i < سائيز; i++)
    إذا (r[i])
      لكل (لاڳاپو_ڳنڍ j = 0; r[i][j] != پڇاڙي_ڳنڍ; ++j)
        ++nedges[r[i][j]];

  /* Allocate. */
  /* The result. */
  لاڳاپو new_R = xnmalloc (سائيز, حجم *new_R);
  /* END_R[I] -- next entry of NEW_R[I]. */
  لاڳاپو پڇاڙي_R = xnmalloc (سائيز, حجم *پڇاڙي_R);
  لكل (لاڳاپو_ڳنڍ i = 0; i < سائيز; i++)
    {
      لاڳاپو_ڳنڍ *sp = عدم;
      إذا (nedges[i] > 0)
        {
          sp = xnmalloc (nedges[i] + 1, حجم *sp);
          sp[nedges[i]] = پڇاڙي_ڳنڍ;
        }
      new_R[i] = sp;
      پڇاڙي_R[i] = sp;
    }

  /* Store. */
  لكل (لاڳاپو_ڳنڍ i = 0; i < سائيز; i++)
    إذا (r[i])
      لكل (لاڳاپو_ڳنڍ j = 0; r[i][j] != پڇاڙي_ڳنڍ; ++j)
        *پڇاڙي_R[r[i][j]]++ = i;

  حرر (nedges);
  حرر (پڇاڙي_R);

  /* Free the input: it is replaced with the result. */
  لكل (لاڳاپو_ڳنڍ i = 0; i < سائيز; i++)
    حرر (r[i]);
  حرر (r);

  إذا (trace_flag & trace_sets)
    لاڳاپو_print ("relation_transpose: output", new_R, سائيز, عدم, stderr);

  *R_arg = new_R;
}
