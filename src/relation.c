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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ "system.h"

#અન્તર્ભાવ <bitsetv.h>

#અન્તર્ભાવ "getargs.h"
#અન્તર્ભાવ "relation.h"

શૂન્ય
relation_print (અચલ અક્ષર *title,
                relation r, relation_ગાંઠ size,
                relation_ગાંઠ_print print, FILE *out)
{
  જો (title)
    fprintf (out, "%s:\n", title);
  માટે (relation_ગાંઠ i = 0; i < size; ++i)
    જો (r[i])
      {
        fputs ("    ", out);
        જો (print)
          print (i, out);
        નહીંતર
          fprintf (out, "%3ld", (long) i);
        fputc (':', out);
        માટે (relation_ગાંઠ j = 0; r[i][j] != END_ગાંઠ; ++j)
          {
            fputc (' ', out);
            જો (print)
              print (r[i][j], out);
            નહીંતર
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

સ્થિર relation R;
સ્થિર relation_nodes indexes;
સ્થિર relation_nodes vertices;
સ્થિર relation_ગાંઠ ટોચ_2;
સ્થિર relation_ગાંઠ infinity;
સ્થિર bitsetv F;

સ્થિર શૂન્ય
traverse (relation_ગાંઠ i)
{
  vertices[++ટોચ_2] = i;
  relation_ગાંઠ height = indexes[i] = ટોચ_2;

  જો (R[i])
    માટે (relation_ગાંઠ j = 0; R[i][j] != END_ગાંઠ; ++j)
      {
        જો (indexes[R[i][j]] == 0)
          traverse (R[i][j]);

        જો (indexes[i] > indexes[R[i][j]])
          indexes[i] = indexes[R[i][j]];

        bitset_or (F[i], F[i], F[R[i][j]]);
      }

  જો (indexes[i] == height)
    માટે (;;)
      {
        relation_ગાંઠ j = vertices[ટોચ_2--];
        indexes[j] = infinity;

        જો (i == j)
          તોડો;

        bitset_copy (F[j], F[i]);
      }
}


શૂન્ય
relation_digraph (relation r, relation_ગાંઠ size, bitsetv function)
{
  infinity = size + 2;
  indexes = xcalloc (size + 1, કદ *indexes);
  vertices = xnmalloc (size + 1, કદ *vertices);
  ટોચ_2 = 0;

  R = r;
  F = function;

  માટે (relation_ગાંઠ i = 0; i < size; i++)
    જો (indexes[i] == 0 && R[i])
      traverse (i);

  મુક્ત (indexes);
  મુક્ત (vertices);

  function = F;
}


/*-------------------------------------------.
| Destructively transpose R_ARG, of size N.  |
`-------------------------------------------*/

શૂન્ય
relation_transpose (relation *R_arg, relation_ગાંઠ size)
{
  relation r = *R_arg;

  જો (trace_flag & trace_sets)
    relation_print ("relation_transpose", r, size, ખાલી, stderr);

  /* Count. */
  /* NEDGES[I] -- total size of NEW_R[I]. */
  size_t *nedges = xcalloc (size, કદ *nedges);
  માટે (relation_ગાંઠ i = 0; i < size; i++)
    જો (r[i])
      માટે (relation_ગાંઠ j = 0; r[i][j] != END_ગાંઠ; ++j)
        ++nedges[r[i][j]];

  /* Allocate. */
  /* The result. */
  relation new_R = xnmalloc (size, કદ *new_R);
  /* END_R[I] -- next entry of NEW_R[I]. */
  relation end_R = xnmalloc (size, કદ *end_R);
  માટે (relation_ગાંઠ i = 0; i < size; i++)
    {
      relation_ગાંઠ *sp = ખાલી;
      જો (nedges[i] > 0)
        {
          sp = xnmalloc (nedges[i] + 1, કદ *sp);
          sp[nedges[i]] = END_ગાંઠ;
        }
      new_R[i] = sp;
      end_R[i] = sp;
    }

  /* Store. */
  માટે (relation_ગાંઠ i = 0; i < size; i++)
    જો (r[i])
      માટે (relation_ગાંઠ j = 0; r[i][j] != END_ગાંઠ; ++j)
        *end_R[r[i][j]]++ = i;

  મુક્ત (nedges);
  મુક્ત (end_R);

  /* Free the input: it is replaced with the result. */
  માટે (relation_ગાંઠ i = 0; i < size; i++)
    મુક્ત (r[i]);
  મુક્ત (r);

  જો (trace_flag & trace_sets)
    relation_print ("relation_transpose: output", new_R, size, ખાલી, stderr);

  *R_arg = new_R;
}
