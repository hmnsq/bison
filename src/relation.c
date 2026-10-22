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
relation_print (ثابت حرف *title,
                relation r, relation_node size,
                relation_node_print print, FILE *out)
{
  אם (title)
    fprintf (out, "%s:\n", title);
  עבור (relation_node i = 0; i < size; ++i)
    אם (r[i])
      {
        fputs ("    ", out);
        אם (print)
          print (i, out);
        אחרת
          fprintf (out, "%3ld", (long) i);
        fputc (':', out);
        עבור (relation_node j = 0; r[i][j] != END_NODE; ++j)
          {
            fputc (' ', out);
            אם (print)
              print (r[i][j], out);
            אחרת
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
ساكن relation_node top;
ساكن relation_node infinity;
ساكن bitsetv F;

ساكن فراغ
traverse (relation_node i)
{
  vertices[++top] = i;
  relation_node height = indexes[i] = top;

  אם (R[i])
    עבור (relation_node j = 0; R[i][j] != END_NODE; ++j)
      {
        אם (indexes[R[i][j]] == 0)
          traverse (R[i][j]);

        אם (indexes[i] > indexes[R[i][j]])
          indexes[i] = indexes[R[i][j]];

        bitset_or (F[i], F[i], F[R[i][j]]);
      }

  אם (indexes[i] == height)
    עבור (;;)
      {
        relation_node j = vertices[top--];
        indexes[j] = infinity;

        אם (i == j)
          اكسر;

        bitset_copy (F[j], F[i]);
      }
}


فراغ
relation_digraph (relation r, relation_node size, bitsetv function)
{
  infinity = size + 2;
  indexes = xcalloc (size + 1, حجم *indexes);
  vertices = xnmalloc (size + 1, حجم *vertices);
  top = 0;

  R = r;
  F = function;

  עבור (relation_node i = 0; i < size; i++)
    אם (indexes[i] == 0 && R[i])
      traverse (i);

  حرر (indexes);
  حرر (vertices);

  function = F;
}


/*-------------------------------------------.
| Destructively transpose R_ARG, of size N.  |
`-------------------------------------------*/

فراغ
relation_transpose (relation *R_arg, relation_node size)
{
  relation r = *R_arg;

  אם (trace_flag & trace_sets)
    relation_print ("relation_transpose", r, size, عدم, stderr);

  /* Count. */
  /* NEDGES[I] -- total size of NEW_R[I]. */
  size_t *nedges = xcalloc (size, حجم *nedges);
  עבור (relation_node i = 0; i < size; i++)
    אם (r[i])
      עבור (relation_node j = 0; r[i][j] != END_NODE; ++j)
        ++nedges[r[i][j]];

  /* Allocate. */
  /* The result. */
  relation new_R = xnmalloc (size, حجم *new_R);
  /* END_R[I] -- next entry of NEW_R[I]. */
  relation end_R = xnmalloc (size, حجم *end_R);
  עבור (relation_node i = 0; i < size; i++)
    {
      relation_node *sp = عدم;
      אם (nedges[i] > 0)
        {
          sp = xnmalloc (nedges[i] + 1, حجم *sp);
          sp[nedges[i]] = END_NODE;
        }
      new_R[i] = sp;
      end_R[i] = sp;
    }

  /* Store. */
  עבור (relation_node i = 0; i < size; i++)
    אם (r[i])
      עבור (relation_node j = 0; r[i][j] != END_NODE; ++j)
        *end_R[r[i][j]]++ = i;

  حرر (nedges);
  حرر (end_R);

  /* Free the input: it is replaced with the result. */
  עבור (relation_node i = 0; i < size; i++)
    حرر (r[i]);
  حرر (r);

  אם (trace_flag & trace_sets)
    relation_print ("relation_transpose: output", new_R, size, عدم, stderr);

  *R_arg = new_R;
}
