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
سمبندھ_print (ثابت حرف *title,
                سمبندھ r, سمبندھ_گرہ ناپ,
                سمبندھ_گرہ_print print, FILE *باہرلا)
{
  إذا (title)
    fprintf (باہرلا, "%s:\n", title);
  لكل (سمبندھ_گرہ i = 0; i < ناپ; ++i)
    إذا (r[i])
      {
        fputs ("    ", باہرلا);
        إذا (print)
          print (i, باہرلا);
        وإلا
          fprintf (باہرلا, "%3ld", (لما) i);
        fputc (':', باہرلا);
        لكل (سمبندھ_گرہ j = 0; r[i][j] != سرا_گرہ; ++j)
          {
            fputc (' ', باہرلا);
            إذا (print)
              print (r[i][j], باہرلا);
            وإلا
              fprintf (باہرلا, "%3ld", (لما) r[i][j]);
          }
        fputc ('\n', باہرلا);
      }
  fputc ('\n', باہرلا);
}


/*---------------------------------------------------------------.
| digraph & traverse.                                            |
|                                                                |
| The following variables are used as common storage between the |
| two.                                                           |
`---------------------------------------------------------------*/

ساكن سمبندھ R;
ساكن سمبندھ_nodes indexes;
ساكن سمبندھ_nodes vertices;
ساكن سمبندھ_گرہ چوٹی_2;
ساكن سمبندھ_گرہ infinity;
ساكن bitsetv F;

ساكن فراغ
traverse (سمبندھ_گرہ i)
{
  vertices[++چوٹی_2] = i;
  سمبندھ_گرہ اچائی = indexes[i] = چوٹی_2;

  إذا (R[i])
    لكل (سمبندھ_گرہ j = 0; R[i][j] != سرا_گرہ; ++j)
      {
        إذا (indexes[R[i][j]] == 0)
          traverse (R[i][j]);

        إذا (indexes[i] > indexes[R[i][j]])
          indexes[i] = indexes[R[i][j]];

        bitset_or (F[i], F[i], F[R[i][j]]);
      }

  إذا (indexes[i] == اچائی)
    لكل (;;)
      {
        سمبندھ_گرہ j = vertices[چوٹی_2--];
        indexes[j] = infinity;

        إذا (i == j)
          اكسر;

        bitset_copy (F[j], F[i]);
      }
}


فراغ
سمبندھ_digraph (سمبندھ r, سمبندھ_گرہ ناپ, bitsetv function)
{
  infinity = ناپ + 2;
  indexes = xcalloc (ناپ + 1, حجم *indexes);
  vertices = xnmalloc (ناپ + 1, حجم *vertices);
  چوٹی_2 = 0;

  R = r;
  F = function;

  لكل (سمبندھ_گرہ i = 0; i < ناپ; i++)
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
سمبندھ_transpose (سمبندھ *R_arg, سمبندھ_گرہ ناپ)
{
  سمبندھ r = *R_arg;

  إذا (trace_flag & trace_sets)
    سمبندھ_print ("relation_transpose", r, ناپ, عدم, stderr);

  /* Count. */
  /* NEDGES[I] -- total size of NEW_R[I]. */
  size_t *nedges = xcalloc (ناپ, حجم *nedges);
  لكل (سمبندھ_گرہ i = 0; i < ناپ; i++)
    إذا (r[i])
      لكل (سمبندھ_گرہ j = 0; r[i][j] != سرا_گرہ; ++j)
        ++nedges[r[i][j]];

  /* Allocate. */
  /* The result. */
  سمبندھ new_R = xnmalloc (ناپ, حجم *new_R);
  /* END_R[I] -- next entry of NEW_R[I]. */
  سمبندھ سرا_R = xnmalloc (ناپ, حجم *سرا_R);
  لكل (سمبندھ_گرہ i = 0; i < ناپ; i++)
    {
      سمبندھ_گرہ *sp = عدم;
      إذا (nedges[i] > 0)
        {
          sp = xnmalloc (nedges[i] + 1, حجم *sp);
          sp[nedges[i]] = سرا_گرہ;
        }
      new_R[i] = sp;
      سرا_R[i] = sp;
    }

  /* Store. */
  لكل (سمبندھ_گرہ i = 0; i < ناپ; i++)
    إذا (r[i])
      لكل (سمبندھ_گرہ j = 0; r[i][j] != سرا_گرہ; ++j)
        *سرا_R[r[i][j]]++ = i;

  حرر (nedges);
  حرر (سرا_R);

  /* Free the input: it is replaced with the result. */
  لكل (سمبندھ_گرہ i = 0; i < ناپ; i++)
    حرر (r[i]);
  حرر (r);

  إذا (trace_flag & trace_sets)
    سمبندھ_print ("relation_transpose: output", new_R, ناپ, عدم, stderr);

  *R_arg = new_R;
}
