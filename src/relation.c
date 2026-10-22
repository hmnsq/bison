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

#شامل <config.h>
#شامل "system.h"

#شامل <bitsetv.h>

#شامل "getargs.h"
#شامل "relation.h"

تهی
relation_print (ثابت نویسه *title,
                relation r, relation_گره size,
                relation_گره_print print, FILE *out)
{
  اگر (title)
    fprintf (out, "%s:\n", title);
  برای (relation_گره i = 0; i < size; ++i)
    اگر (r[i])
      {
        fputs ("    ", out);
        اگر (print)
          print (i, out);
        وگرنه
          fprintf (out, "%3ld", (long) i);
        fputc (':', out);
        برای (relation_گره j = 0; r[i][j] != END_گره; ++j)
          {
            fputc (' ', out);
            اگر (print)
              print (r[i][j], out);
            وگرنه
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

ایستا relation R;
ایستا relation_nodes indexes;
ایستا relation_nodes vertices;
ایستا relation_گره بالاترین_2;
ایستا relation_گره infinity;
ایستا bitsetv F;

ایستا تهی
traverse (relation_گره i)
{
  vertices[++بالاترین_2] = i;
  relation_گره height = indexes[i] = بالاترین_2;

  اگر (R[i])
    برای (relation_گره j = 0; R[i][j] != END_گره; ++j)
      {
        اگر (indexes[R[i][j]] == 0)
          traverse (R[i][j]);

        اگر (indexes[i] > indexes[R[i][j]])
          indexes[i] = indexes[R[i][j]];

        bitset_or (F[i], F[i], F[R[i][j]]);
      }

  اگر (indexes[i] == height)
    برای (;;)
      {
        relation_گره j = vertices[بالاترین_2--];
        indexes[j] = infinity;

        اگر (i == j)
          بشکن;

        bitset_copy (F[j], F[i]);
      }
}


تهی
relation_digraph (relation r, relation_گره size, bitsetv function)
{
  infinity = size + 2;
  indexes = xcalloc (size + 1, اندازه *indexes);
  vertices = xnmalloc (size + 1, اندازه *vertices);
  بالاترین_2 = 0;

  R = r;
  F = function;

  برای (relation_گره i = 0; i < size; i++)
    اگر (indexes[i] == 0 && R[i])
      traverse (i);

  آزاد (indexes);
  آزاد (vertices);

  function = F;
}


/*-------------------------------------------.
| Destructively transpose R_ARG, of size N.  |
`-------------------------------------------*/

تهی
relation_transpose (relation *R_arg, relation_گره size)
{
  relation r = *R_arg;

  اگر (trace_flag & trace_sets)
    relation_print ("relation_transpose", r, size, پوچ, stderr);

  /* Count. */
  /* NEDGES[I] -- total size of NEW_R[I]. */
  size_t *nedges = xcalloc (size, اندازه *nedges);
  برای (relation_گره i = 0; i < size; i++)
    اگر (r[i])
      برای (relation_گره j = 0; r[i][j] != END_گره; ++j)
        ++nedges[r[i][j]];

  /* Allocate. */
  /* The result. */
  relation new_R = xnmalloc (size, اندازه *new_R);
  /* END_R[I] -- next entry of NEW_R[I]. */
  relation end_R = xnmalloc (size, اندازه *end_R);
  برای (relation_گره i = 0; i < size; i++)
    {
      relation_گره *sp = پوچ;
      اگر (nedges[i] > 0)
        {
          sp = xnmalloc (nedges[i] + 1, اندازه *sp);
          sp[nedges[i]] = END_گره;
        }
      new_R[i] = sp;
      end_R[i] = sp;
    }

  /* Store. */
  برای (relation_گره i = 0; i < size; i++)
    اگر (r[i])
      برای (relation_گره j = 0; r[i][j] != END_گره; ++j)
        *end_R[r[i][j]]++ = i;

  آزاد (nedges);
  آزاد (end_R);

  /* Free the input: it is replaced with the result. */
  برای (relation_گره i = 0; i < size; i++)
    آزاد (r[i]);
  آزاد (r);

  اگر (trace_flag & trace_sets)
    relation_print ("relation_transpose: output", new_R, size, پوچ, stderr);

  *R_arg = new_R;
}
