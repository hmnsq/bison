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

#अन्तर्भाव <config.h>
#अन्तर्भाव "system.h"

#अन्तर्भाव <bitsetv.h>

#अन्तर्भाव "getargs.h"
#अन्तर्भाव "relation.h"

शून्य
relation_print (ध्रुव वर्ण *title,
                relation r, relation_ग्रन्थिः size,
                relation_ग्रन्थिः_print print, FILE *out)
{
  सचे (title)
    fprintf (out, "%s:\n", title);
  अत्थाय (relation_ग्रन्थिः i = 0; i < size; ++i)
    सचे (r[i])
      {
        fputs ("    ", out);
        सचे (print)
          print (i, out);
        अञ्ञथा
          fprintf (out, "%3ld", (long) i);
        fputc (':', out);
        अत्थाय (relation_ग्रन्थिः j = 0; r[i][j] != END_ग्रन्थिः; ++j)
          {
            fputc (' ', out);
            सचे (print)
              print (r[i][j], out);
            अञ्ञथा
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

स्थिर relation R;
स्थिर relation_nodes indexes;
स्थिर relation_nodes vertices;
स्थिर relation_ग्रन्थिः शिखरम्_2;
स्थिर relation_ग्रन्थिः infinity;
स्थिर bitsetv F;

स्थिर शून्य
traverse (relation_ग्रन्थिः i)
{
  vertices[++शिखरम्_2] = i;
  relation_ग्रन्थिः height = indexes[i] = शिखरम्_2;

  सचे (R[i])
    अत्थाय (relation_ग्रन्थिः j = 0; R[i][j] != END_ग्रन्थिः; ++j)
      {
        सचे (indexes[R[i][j]] == 0)
          traverse (R[i][j]);

        सचे (indexes[i] > indexes[R[i][j]])
          indexes[i] = indexes[R[i][j]];

        bitset_or (F[i], F[i], F[R[i][j]]);
      }

  सचे (indexes[i] == height)
    अत्थाय (;;)
      {
        relation_ग्रन्थिः j = vertices[शिखरम्_2--];
        indexes[j] = infinity;

        सचे (i == j)
          भङ्ग;

        bitset_copy (F[j], F[i]);
      }
}


शून्य
relation_digraph (relation r, relation_ग्रन्थिः size, bitsetv function)
{
  infinity = size + 2;
  indexes = xcalloc (size + 1, परिमाण *indexes);
  vertices = xnmalloc (size + 1, परिमाण *vertices);
  शिखरम्_2 = 0;

  R = r;
  F = function;

  अत्थाय (relation_ग्रन्थिः i = 0; i < size; i++)
    सचे (indexes[i] == 0 && R[i])
      traverse (i);

  मुक्त (indexes);
  मुक्त (vertices);

  function = F;
}


/*-------------------------------------------.
| Destructively transpose R_ARG, of size N.  |
`-------------------------------------------*/

शून्य
relation_transpose (relation *R_arg, relation_ग्रन्थिः size)
{
  relation r = *R_arg;

  सचे (trace_flag & trace_sets)
    relation_print ("relation_transpose", r, size, रिक्त, stderr);

  /* Count. */
  /* NEDGES[I] -- total size of NEW_R[I]. */
  size_t *nedges = xcalloc (size, परिमाण *nedges);
  अत्थाय (relation_ग्रन्थिः i = 0; i < size; i++)
    सचे (r[i])
      अत्थाय (relation_ग्रन्थिः j = 0; r[i][j] != END_ग्रन्थिः; ++j)
        ++nedges[r[i][j]];

  /* Allocate. */
  /* The result. */
  relation new_R = xnmalloc (size, परिमाण *new_R);
  /* END_R[I] -- next entry of NEW_R[I]. */
  relation end_R = xnmalloc (size, परिमाण *end_R);
  अत्थाय (relation_ग्रन्थिः i = 0; i < size; i++)
    {
      relation_ग्रन्थिः *sp = रिक्त;
      सचे (nedges[i] > 0)
        {
          sp = xnmalloc (nedges[i] + 1, परिमाण *sp);
          sp[nedges[i]] = END_ग्रन्थिः;
        }
      new_R[i] = sp;
      end_R[i] = sp;
    }

  /* Store. */
  अत्थाय (relation_ग्रन्थिः i = 0; i < size; i++)
    सचे (r[i])
      अत्थाय (relation_ग्रन्थिः j = 0; r[i][j] != END_ग्रन्थिः; ++j)
        *end_R[r[i][j]]++ = i;

  मुक्त (nedges);
  मुक्त (end_R);

  /* Free the input: it is replaced with the result. */
  अत्थाय (relation_ग्रन्थिः i = 0; i < size; i++)
    मुक्त (r[i]);
  मुक्त (r);

  सचे (trace_flag & trace_sets)
    relation_print ("relation_transpose: output", new_R, size, रिक्त, stderr);

  *R_arg = new_R;
}
