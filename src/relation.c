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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ <bitsetv.h>

#ಅನ್ತರ್ಭಾವ "getargs.h"
#ಅನ್ತರ್ಭಾವ "relation.h"

ಶೂನ್ಯ
relation_print (ಧ್ರುವ ವರ್ಣ *title,
                relation r, relation_ಗಂಟು size,
                relation_ಗಂಟು_print print, FILE *out)
{
  ಯದಿ (title)
    fprintf (out, "%s:\n", title);
  ಪ್ರತಿ (relation_ಗಂಟು i = 0; i < size; ++i)
    ಯದಿ (r[i])
      {
        fputs ("    ", out);
        ಯದಿ (print)
          print (i, out);
        ಅನ್ಯಥಾ
          fprintf (out, "%3ld", (long) i);
        fputc (':', out);
        ಪ್ರತಿ (relation_ಗಂಟು j = 0; r[i][j] != END_ಗಂಟು; ++j)
          {
            fputc (' ', out);
            ಯದಿ (print)
              print (r[i][j], out);
            ಅನ್ಯಥಾ
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

ಸ್ಥಿರ relation R;
ಸ್ಥಿರ relation_nodes indexes;
ಸ್ಥಿರ relation_nodes vertices;
ಸ್ಥಿರ relation_ಗಂಟು ತುದಿ_2;
ಸ್ಥಿರ relation_ಗಂಟು infinity;
ಸ್ಥಿರ bitsetv F;

ಸ್ಥಿರ ಶೂನ್ಯ
traverse (relation_ಗಂಟು i)
{
  vertices[++ತುದಿ_2] = i;
  relation_ಗಂಟು height = indexes[i] = ತುದಿ_2;

  ಯದಿ (R[i])
    ಪ್ರತಿ (relation_ಗಂಟು j = 0; R[i][j] != END_ಗಂಟು; ++j)
      {
        ಯದಿ (indexes[R[i][j]] == 0)
          traverse (R[i][j]);

        ಯದಿ (indexes[i] > indexes[R[i][j]])
          indexes[i] = indexes[R[i][j]];

        bitset_or (F[i], F[i], F[R[i][j]]);
      }

  ಯದಿ (indexes[i] == height)
    ಪ್ರತಿ (;;)
      {
        relation_ಗಂಟು j = vertices[ತುದಿ_2--];
        indexes[j] = infinity;

        ಯದಿ (i == j)
          ಭಙ್ಗ;

        bitset_copy (F[j], F[i]);
      }
}


ಶೂನ್ಯ
relation_digraph (relation r, relation_ಗಂಟು size, bitsetv function)
{
  infinity = size + 2;
  indexes = xcalloc (size + 1, ಪರಿಮಾಣ *indexes);
  vertices = xnmalloc (size + 1, ಪರಿಮಾಣ *vertices);
  ತುದಿ_2 = 0;

  R = r;
  F = function;

  ಪ್ರತಿ (relation_ಗಂಟು i = 0; i < size; i++)
    ಯದಿ (indexes[i] == 0 && R[i])
      traverse (i);

  ಮುಕ್ತ (indexes);
  ಮುಕ್ತ (vertices);

  function = F;
}


/*-------------------------------------------.
| Destructively transpose R_ARG, of size N.  |
`-------------------------------------------*/

ಶೂನ್ಯ
relation_transpose (relation *R_arg, relation_ಗಂಟು size)
{
  relation r = *R_arg;

  ಯದಿ (trace_flag & trace_sets)
    relation_print ("relation_transpose", r, size, ರಿಕ್ತ, stderr);

  /* Count. */
  /* NEDGES[I] -- total size of NEW_R[I]. */
  size_t *nedges = xcalloc (size, ಪರಿಮಾಣ *nedges);
  ಪ್ರತಿ (relation_ಗಂಟು i = 0; i < size; i++)
    ಯದಿ (r[i])
      ಪ್ರತಿ (relation_ಗಂಟು j = 0; r[i][j] != END_ಗಂಟು; ++j)
        ++nedges[r[i][j]];

  /* Allocate. */
  /* The result. */
  relation new_R = xnmalloc (size, ಪರಿಮಾಣ *new_R);
  /* END_R[I] -- next entry of NEW_R[I]. */
  relation end_R = xnmalloc (size, ಪರಿಮಾಣ *end_R);
  ಪ್ರತಿ (relation_ಗಂಟು i = 0; i < size; i++)
    {
      relation_ಗಂಟು *sp = ರಿಕ್ತ;
      ಯದಿ (nedges[i] > 0)
        {
          sp = xnmalloc (nedges[i] + 1, ಪರಿಮಾಣ *sp);
          sp[nedges[i]] = END_ಗಂಟು;
        }
      new_R[i] = sp;
      end_R[i] = sp;
    }

  /* Store. */
  ಪ್ರತಿ (relation_ಗಂಟು i = 0; i < size; i++)
    ಯದಿ (r[i])
      ಪ್ರತಿ (relation_ಗಂಟು j = 0; r[i][j] != END_ಗಂಟು; ++j)
        *end_R[r[i][j]]++ = i;

  ಮುಕ್ತ (nedges);
  ಮುಕ್ತ (end_R);

  /* Free the input: it is replaced with the result. */
  ಪ್ರತಿ (relation_ಗಂಟು i = 0; i < size; i++)
    ಮುಕ್ತ (r[i]);
  ಮುಕ್ತ (r);

  ಯದಿ (trace_flag & trace_sets)
    relation_print ("relation_transpose: output", new_R, size, ರಿಕ್ತ, stderr);

  *R_arg = new_R;
}
