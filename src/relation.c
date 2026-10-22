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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ "system.h"

#அந்தர்பாவ <bitsetv.h>

#அந்தர்பாவ "getargs.h"
#அந்தர்பாவ "relation.h"

வெறுமை
relation_print (மாறிலி எழுத்து *title,
                relation r, relation_முனை size,
                relation_முனை_print print, FILE *out)
{
  எனில் (title)
    fprintf (out, "%s:\n", title);
  க்கு (relation_முனை i = 0; i < size; ++i)
    எனில் (r[i])
      {
        fputs ("    ", out);
        எனில் (print)
          print (i, out);
        இல்லையெனில்
          fprintf (out, "%3ld", (long) i);
        fputc (':', out);
        க்கு (relation_முனை j = 0; r[i][j] != END_முனை; ++j)
          {
            fputc (' ', out);
            எனில் (print)
              print (r[i][j], out);
            இல்லையெனில்
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

நிலையான relation R;
நிலையான relation_nodes indexes;
நிலையான relation_nodes vertices;
நிலையான relation_முனை உச்சி_2;
நிலையான relation_முனை infinity;
நிலையான bitsetv F;

நிலையான வெறுமை
traverse (relation_முனை i)
{
  vertices[++உச்சி_2] = i;
  relation_முனை height = indexes[i] = உச்சி_2;

  எனில் (R[i])
    க்கு (relation_முனை j = 0; R[i][j] != END_முனை; ++j)
      {
        எனில் (indexes[R[i][j]] == 0)
          traverse (R[i][j]);

        எனில் (indexes[i] > indexes[R[i][j]])
          indexes[i] = indexes[R[i][j]];

        bitset_or (F[i], F[i], F[R[i][j]]);
      }

  எனில் (indexes[i] == height)
    க்கு (;;)
      {
        relation_முனை j = vertices[உச்சி_2--];
        indexes[j] = infinity;

        எனில் (i == j)
          நிறுத்து;

        bitset_copy (F[j], F[i]);
      }
}


வெறுமை
relation_digraph (relation r, relation_முனை size, bitsetv function)
{
  infinity = size + 2;
  indexes = xcalloc (size + 1, அளவு *indexes);
  vertices = xnmalloc (size + 1, அளவு *vertices);
  உச்சி_2 = 0;

  R = r;
  F = function;

  க்கு (relation_முனை i = 0; i < size; i++)
    எனில் (indexes[i] == 0 && R[i])
      traverse (i);

  விடுவி (indexes);
  விடுவி (vertices);

  function = F;
}


/*-------------------------------------------.
| Destructively transpose R_ARG, of size N.  |
`-------------------------------------------*/

வெறுமை
relation_transpose (relation *R_arg, relation_முனை size)
{
  relation r = *R_arg;

  எனில் (trace_flag & trace_sets)
    relation_print ("relation_transpose", r, size, வெற்று, stderr);

  /* Count. */
  /* NEDGES[I] -- total size of NEW_R[I]. */
  size_t *nedges = xcalloc (size, அளவு *nedges);
  க்கு (relation_முனை i = 0; i < size; i++)
    எனில் (r[i])
      க்கு (relation_முனை j = 0; r[i][j] != END_முனை; ++j)
        ++nedges[r[i][j]];

  /* Allocate. */
  /* The result. */
  relation new_R = xnmalloc (size, அளவு *new_R);
  /* END_R[I] -- next entry of NEW_R[I]. */
  relation end_R = xnmalloc (size, அளவு *end_R);
  க்கு (relation_முனை i = 0; i < size; i++)
    {
      relation_முனை *sp = வெற்று;
      எனில் (nedges[i] > 0)
        {
          sp = xnmalloc (nedges[i] + 1, அளவு *sp);
          sp[nedges[i]] = END_முனை;
        }
      new_R[i] = sp;
      end_R[i] = sp;
    }

  /* Store. */
  க்கு (relation_முனை i = 0; i < size; i++)
    எனில் (r[i])
      க்கு (relation_முனை j = 0; r[i][j] != END_முனை; ++j)
        *end_R[r[i][j]]++ = i;

  விடுவி (nedges);
  விடுவி (end_R);

  /* Free the input: it is replaced with the result. */
  க்கு (relation_முனை i = 0; i < size; i++)
    விடுவி (r[i]);
  விடுவி (r);

  எனில் (trace_flag & trace_sets)
    relation_print ("relation_transpose: output", new_R, size, வெற்று, stderr);

  *R_arg = new_R;
}
