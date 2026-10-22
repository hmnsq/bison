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
relation_print (const char *title,
                relation r, relation_node size,
                relation_node_print print, FILE *out)
{
  如果 (title)
    fprintf (out, "%s:\n", title);
  为 (relation_node i = 0; i < size; ++i)
    如果 (r[i])
      {
        fputs ("    ", out);
        如果 (print)
          print (i, out);
        否则
          fprintf (out, "%3ld", (long) i);
        fputc (':', out);
        为 (relation_node j = 0; r[i][j] != END_NODE; ++j)
          {
            fputc (' ', out);
            如果 (print)
              print (r[i][j], out);
            否则
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

  如果 (R[i])
    为 (relation_node j = 0; R[i][j] != END_NODE; ++j)
      {
        如果 (indexes[R[i][j]] == 0)
          traverse (R[i][j]);

        如果 (indexes[i] > indexes[R[i][j]])
          indexes[i] = indexes[R[i][j]];

        bitset_or (F[i], F[i], F[R[i][j]]);
      }

  如果 (indexes[i] == height)
    为 (;;)
      {
        relation_node j = vertices[top--];
        indexes[j] = infinity;

        如果 (i == j)
          break;

        bitset_copy (F[j], F[i]);
      }
}


void
relation_digraph (relation r, relation_node size, bitsetv function)
{
  infinity = size + 2;
  indexes = xcalloc (size + 1, sizeof *indexes);
  vertices = xnmalloc (size + 1, sizeof *vertices);
  top = 0;

  R = r;
  F = function;

  为 (relation_node i = 0; i < size; i++)
    如果 (indexes[i] == 0 && R[i])
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

  如果 (trace_flag & trace_sets)
    relation_print ("relation_transpose", r, size, NULL, stderr);

  /* Count. */
  /* NEDGES[I] -- total size of NEW_R[I]. */
  size_t *nedges = xcalloc (size, sizeof *nedges);
  为 (relation_node i = 0; i < size; i++)
    如果 (r[i])
      为 (relation_node j = 0; r[i][j] != END_NODE; ++j)
        ++nedges[r[i][j]];

  /* Allocate. */
  /* The result. */
  relation new_R = xnmalloc (size, sizeof *new_R);
  /* END_R[I] -- next entry of NEW_R[I]. */
  relation end_R = xnmalloc (size, sizeof *end_R);
  为 (relation_node i = 0; i < size; i++)
    {
      relation_node *sp = NULL;
      如果 (nedges[i] > 0)
        {
          sp = xnmalloc (nedges[i] + 1, sizeof *sp);
          sp[nedges[i]] = END_NODE;
        }
      new_R[i] = sp;
      end_R[i] = sp;
    }

  /* Store. */
  为 (relation_node i = 0; i < size; i++)
    如果 (r[i])
      为 (relation_node j = 0; r[i][j] != END_NODE; ++j)
        *end_R[r[i][j]]++ = i;

  free (nedges);
  free (end_R);

  /* Free the input: it is replaced with the result. */
  为 (relation_node i = 0; i < size; i++)
    free (r[i]);
  free (r);

  如果 (trace_flag & trace_sets)
    relation_print ("relation_transpose: output", new_R, size, NULL, stderr);

  *R_arg = new_R;
}
