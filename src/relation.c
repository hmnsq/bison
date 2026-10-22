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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব "system.h"

#অন্তর্ভাব <bitsetv.h>

#অন্তর্ভাব "getargs.h"
#অন্তর্ভাব "relation.h"

শূন্য
relation_print (ধ্রুব বর্ণ *title,
                relation r, relation_গ্রন্থি size,
                relation_গ্রন্থি_print print, FILE *out)
{
  যদি (title)
    fprintf (out, "%s:\n", title);
  প্রতি (relation_গ্রন্থি i = 0; i < size; ++i)
    যদি (r[i])
      {
        fputs ("    ", out);
        যদি (print)
          print (i, out);
        অন্যথা
          fprintf (out, "%3ld", (long) i);
        fputc (':', out);
        প্রতি (relation_গ্রন্থি j = 0; r[i][j] != END_গ্রন্থি; ++j)
          {
            fputc (' ', out);
            যদি (print)
              print (r[i][j], out);
            অন্যথা
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

স্থির relation R;
স্থির relation_nodes indexes;
স্থির relation_nodes vertices;
স্থির relation_গ্রন্থি শীর্ষ_2;
স্থির relation_গ্রন্থি infinity;
স্থির bitsetv F;

স্থির শূন্য
traverse (relation_গ্রন্থি i)
{
  vertices[++শীর্ষ_2] = i;
  relation_গ্রন্থি height = indexes[i] = শীর্ষ_2;

  যদি (R[i])
    প্রতি (relation_গ্রন্থি j = 0; R[i][j] != END_গ্রন্থি; ++j)
      {
        যদি (indexes[R[i][j]] == 0)
          traverse (R[i][j]);

        যদি (indexes[i] > indexes[R[i][j]])
          indexes[i] = indexes[R[i][j]];

        bitset_or (F[i], F[i], F[R[i][j]]);
      }

  যদি (indexes[i] == height)
    প্রতি (;;)
      {
        relation_গ্রন্থি j = vertices[শীর্ষ_2--];
        indexes[j] = infinity;

        যদি (i == j)
          ভঙ্গ;

        bitset_copy (F[j], F[i]);
      }
}


শূন্য
relation_digraph (relation r, relation_গ্রন্থি size, bitsetv function)
{
  infinity = size + 2;
  indexes = xcalloc (size + 1, পরিমাণ *indexes);
  vertices = xnmalloc (size + 1, পরিমাণ *vertices);
  শীর্ষ_2 = 0;

  R = r;
  F = function;

  প্রতি (relation_গ্রন্থি i = 0; i < size; i++)
    যদি (indexes[i] == 0 && R[i])
      traverse (i);

  মুক্ত (indexes);
  মুক্ত (vertices);

  function = F;
}


/*-------------------------------------------.
| Destructively transpose R_ARG, of size N.  |
`-------------------------------------------*/

শূন্য
relation_transpose (relation *R_arg, relation_গ্রন্থি size)
{
  relation r = *R_arg;

  যদি (trace_flag & trace_sets)
    relation_print ("relation_transpose", r, size, রিক্ত, stderr);

  /* Count. */
  /* NEDGES[I] -- total size of NEW_R[I]. */
  size_t *nedges = xcalloc (size, পরিমাণ *nedges);
  প্রতি (relation_গ্রন্থি i = 0; i < size; i++)
    যদি (r[i])
      প্রতি (relation_গ্রন্থি j = 0; r[i][j] != END_গ্রন্থি; ++j)
        ++nedges[r[i][j]];

  /* Allocate. */
  /* The result. */
  relation new_R = xnmalloc (size, পরিমাণ *new_R);
  /* END_R[I] -- next entry of NEW_R[I]. */
  relation end_R = xnmalloc (size, পরিমাণ *end_R);
  প্রতি (relation_গ্রন্থি i = 0; i < size; i++)
    {
      relation_গ্রন্থি *sp = রিক্ত;
      যদি (nedges[i] > 0)
        {
          sp = xnmalloc (nedges[i] + 1, পরিমাণ *sp);
          sp[nedges[i]] = END_গ্রন্থি;
        }
      new_R[i] = sp;
      end_R[i] = sp;
    }

  /* Store. */
  প্রতি (relation_গ্রন্থি i = 0; i < size; i++)
    যদি (r[i])
      প্রতি (relation_গ্রন্থি j = 0; r[i][j] != END_গ্রন্থি; ++j)
        *end_R[r[i][j]]++ = i;

  মুক্ত (nedges);
  মুক্ত (end_R);

  /* Free the input: it is replaced with the result. */
  প্রতি (relation_গ্রন্থি i = 0; i < size; i++)
    মুক্ত (r[i]);
  মুক্ত (r);

  যদি (trace_flag & trace_sets)
    relation_print ("relation_transpose: output", new_R, size, রিক্ত, stderr);

  *R_arg = new_R;
}
