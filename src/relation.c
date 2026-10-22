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
relation_بغ (ثابت حرف *title,
                relation r, relation_كلي size,
                relation_كلي_بغ بغ, FILE *out)
{
  إذا (title)
    fprintf (out, "%s:\n", title);
  لكل (relation_كلي i = 0; i < size; ++i)
    إذا (r[i])
      {
        fputs ("    ", out);
        إذا (بغ)
          بغ (i, out);
        وإلا
          fprintf (out, "%3ld", (long) i);
        fputc (':', out);
        لكل (relation_كلي j = 0; r[i][j] != END_كلي; ++j)
          {
            fputc (' ', out);
            إذا (بغ)
              بغ (r[i][j], out);
            وإلا
              fprintf (out, "%3ld", (long) r[i][j]);
          }
        fputc ('\n', out);
      }
  fputc ('\n', out);
}


/*---------------------------------------------------------------.
| digraph & traverse.                                            |
|                                                                |
| The following variables are used as common storage between the |
| two.                                                           |
`---------------------------------------------------------------*/

ساكن relation R;
ساكن relation_nodes indexes;
ساكن relation_nodes vertices;
ساكن relation_كلي كولي_2;
ساكن relation_كلي infinity;
ساكن bitsetv F;

ساكن فراغ
traverse (relation_كلي i)
{
  vertices[++كولي_2] = i;
  relation_كلي height = indexes[i] = كولي_2;

  إذا (R[i])
    لكل (relation_كلي j = 0; R[i][j] != END_كلي; ++j)
      {
        إذا (indexes[R[i][j]] == 0)
          traverse (R[i][j]);

        إذا (indexes[i] > indexes[R[i][j]])
          indexes[i] = indexes[R[i][j]];

        bitset_or (F[i], F[i], F[R[i][j]]);
      }

  إذا (indexes[i] == height)
    لكل (;;)
      {
        relation_كلي j = vertices[كولي_2--];
        indexes[j] = infinity;

        إذا (i == j)
          اكسر;

        bitset_copy (F[j], F[i]);
      }
}


فراغ
relation_digraph (relation r, relation_كلي size, bitsetv function)
{
  infinity = size + 2;
  indexes = xcalloc (size + 1, حجم *indexes);
  vertices = xnmalloc (size + 1, حجم *vertices);
  كولي_2 = 0;

  R = r;
  F = function;

  لكل (relation_كلي i = 0; i < size; i++)
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
relation_transpose (relation *R_arg, relation_كلي size)
{
  relation r = *R_arg;

  إذا (trace_flag & trace_sets)
    relation_بغ ("relation_transpose", r, size, عدم, stderr);

  /* Count. */
  /* NEDGES[I] -- total size of NEW_R[I]. */
  size_t *nedges = xcalloc (size, حجم *nedges);
  لكل (relation_كلي i = 0; i < size; i++)
    إذا (r[i])
      لكل (relation_كلي j = 0; r[i][j] != END_كلي; ++j)
        ++nedges[r[i][j]];

  /* Allocate. */
  /* The result. */
  relation new_R = xnmalloc (size, حجم *new_R);
  /* END_R[I] -- next entry of NEW_R[I]. */
  relation end_R = xnmalloc (size, حجم *end_R);
  لكل (relation_كلي i = 0; i < size; i++)
    {
      relation_كلي *sp = عدم;
      إذا (nedges[i] > 0)
        {
          sp = xnmalloc (nedges[i] + 1, حجم *sp);
          sp[nedges[i]] = END_كلي;
        }
      new_R[i] = sp;
      end_R[i] = sp;
    }

  /* Store. */
  لكل (relation_كلي i = 0; i < size; i++)
    إذا (r[i])
      لكل (relation_كلي j = 0; r[i][j] != END_كلي; ++j)
        *end_R[r[i][j]]++ = i;

  حرر (nedges);
  حرر (end_R);

  /* Free the input: it is replaced with the result. */
  لكل (relation_كلي i = 0; i < size; i++)
    حرر (r[i]);
  حرر (r);

  إذا (trace_flag & trace_sets)
    relation_بغ ("relation_transpose: output", new_R, size, عدم, stderr);

  *R_arg = new_R;
}
