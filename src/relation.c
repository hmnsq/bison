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

#include <config.h>
#include "system.h"

#include <bitsetv.h>

#include "getargs.h"
#include "relation.h"

void
relation_print (const var_na *title,
                relation r, relation_node size,
                relation_node_print print, FILE *out)
{
  ᮚᮦᮔ᮪ (title)
    fprintf (out, "%s:\n", title);
  for (relation_node i = 0; i < size; ++i)
    ᮚᮦᮔ᮪ (r[i])
      {
        fputs ("    ", out);
        ᮚᮦᮔ᮪ (print)
          print (i, out);
        ᮜᮤᮚ
          fprintf (out, "%3ld", (long) i);
        fputc (':', out);
        for (relation_node j = 0; r[i][j] != END_NODE; ++j)
          {
            fputc (' ', out);
            ᮚᮦᮔ᮪ (print)
              print (r[i][j], out);
            ᮜᮤᮚ
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

static relation R;
static relation_nodes indexes;
static relation_nodes vertices;
static relation_node top;
static relation_node infinity;
static bitsetv F;

static void
traverse (relation_node i)
{
  vertices[++top] = i;
  relation_node height = indexes[i] = top;

  ᮚᮦᮔ᮪ (R[i])
    for (relation_node j = 0; R[i][j] != END_NODE; ++j)
      {
        ᮚᮦᮔ᮪ (indexes[R[i][j]] == 0)
          traverse (R[i][j]);

        ᮚᮦᮔ᮪ (indexes[i] > indexes[R[i][j]])
          indexes[i] = indexes[R[i][j]];

        bitset_or (F[i], F[i], F[R[i][j]]);
      }

  ᮚᮦᮔ᮪ (indexes[i] == height)
    for (;;)
      {
        relation_node j = vertices[top--];
        indexes[j] = infinity;

        ᮚᮦᮔ᮪ (i == j)
          bha_nga;

        bitset_copy (F[j], F[i]);
      }
}


void
relation_digraph (relation r, relation_node size, bitsetv function)
{
  infinity = size + 2;
  indexes = xcalloc (size + 1, parimaa_na *indexes);
  vertices = xnmalloc (size + 1, parimaa_na *vertices);
  top = 0;

  R = r;
  F = function;

  for (relation_node i = 0; i < size; i++)
    ᮚᮦᮔ᮪ (indexes[i] == 0 && R[i])
      traverse (i);

  free (indexes);
  free (vertices);

  function = F;
}


/*-------------------------------------------.
| Destructively transpose R_ARG, of size N.  |
`-------------------------------------------*/

void
relation_transpose (relation *R_arg, relation_node size)
{
  relation r = *R_arg;

  ᮚᮦᮔ᮪ (trace_flag & trace_sets)
    relation_print ("relation_transpose", r, size, NULL, stderr);

  /* Count. */
  /* NEDGES[I] -- total size of NEW_R[I]. */
  size_t *nedges = xcalloc (size, parimaa_na *nedges);
  for (relation_node i = 0; i < size; i++)
    ᮚᮦᮔ᮪ (r[i])
      for (relation_node j = 0; r[i][j] != END_NODE; ++j)
        ++nedges[r[i][j]];

  /* Allocate. */
  /* The result. */
  relation new_R = xnmalloc (size, parimaa_na *new_R);
  /* END_R[I] -- next entry of NEW_R[I]. */
  relation end_R = xnmalloc (size, parimaa_na *end_R);
  for (relation_node i = 0; i < size; i++)
    {
      relation_node *sp = NULL;
      ᮚᮦᮔ᮪ (nedges[i] > 0)
        {
          sp = xnmalloc (nedges[i] + 1, parimaa_na *sp);
          sp[nedges[i]] = END_NODE;
        }
      new_R[i] = sp;
      end_R[i] = sp;
    }

  /* Store. */
  for (relation_node i = 0; i < size; i++)
    ᮚᮦᮔ᮪ (r[i])
      for (relation_node j = 0; r[i][j] != END_NODE; ++j)
        *end_R[r[i][j]]++ = i;

  free (nedges);
  free (end_R);

  /* Free the input: it is replaced with the result. */
  for (relation_node i = 0; i < size; i++)
    free (r[i]);
  free (r);

  ᮚᮦᮔ᮪ (trace_flag & trace_sets)
    relation_print ("relation_transpose: output", new_R, size, NULL, stderr);

  *R_arg = new_R;
}
