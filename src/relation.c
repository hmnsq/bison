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
                relation r, relation_गाठ size,
                relation_गाठ_print print, FILE *out)
{
  यदि (title)
    fprintf (out, "%s:\n", title);
  प्रति (relation_गाठ i = 0; i < size; ++i)
    यदि (r[i])
      {
        fputs ("    ", out);
        यदि (print)
          print (i, out);
        अन्यथा
          fprintf (out, "%3ld", (long) i);
        fputc (':', out);
        प्रति (relation_गाठ j = 0; r[i][j] != END_गाठ; ++j)
          {
            fputc (' ', out);
            यदि (print)
              print (r[i][j], out);
            अन्यथा
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
स्थिर relation_गाठ शीर्ष_2;
स्थिर relation_गाठ infinity;
स्थिर bitsetv F;

स्थिर शून्य
traverse (relation_गाठ i)
{
  vertices[++शीर्ष_2] = i;
  relation_गाठ height = indexes[i] = शीर्ष_2;

  यदि (R[i])
    प्रति (relation_गाठ j = 0; R[i][j] != END_गाठ; ++j)
      {
        यदि (indexes[R[i][j]] == 0)
          traverse (R[i][j]);

        यदि (indexes[i] > indexes[R[i][j]])
          indexes[i] = indexes[R[i][j]];

        bitset_or (F[i], F[i], F[R[i][j]]);
      }

  यदि (indexes[i] == height)
    प्रति (;;)
      {
        relation_गाठ j = vertices[शीर्ष_2--];
        indexes[j] = infinity;

        यदि (i == j)
          भङ्ग;

        bitset_copy (F[j], F[i]);
      }
}


शून्य
relation_digraph (relation r, relation_गाठ size, bitsetv function)
{
  infinity = size + 2;
  indexes = xcalloc (size + 1, परिमाण *indexes);
  vertices = xnmalloc (size + 1, परिमाण *vertices);
  शीर्ष_2 = 0;

  R = r;
  F = function;

  प्रति (relation_गाठ i = 0; i < size; i++)
    यदि (indexes[i] == 0 && R[i])
      traverse (i);

  मुक्त (indexes);
  मुक्त (vertices);

  function = F;
}


/*-------------------------------------------.
| Destructively transpose R_ARG, of size N.  |
`-------------------------------------------*/

शून्य
relation_transpose (relation *R_arg, relation_गाठ size)
{
  relation r = *R_arg;

  यदि (trace_flag & trace_sets)
    relation_print ("relation_transpose", r, size, रिक्त, stderr);

  /* Count. */
  /* NEDGES[I] -- total size of NEW_R[I]. */
  size_t *nedges = xcalloc (size, परिमाण *nedges);
  प्रति (relation_गाठ i = 0; i < size; i++)
    यदि (r[i])
      प्रति (relation_गाठ j = 0; r[i][j] != END_गाठ; ++j)
        ++nedges[r[i][j]];

  /* Allocate. */
  /* The result. */
  relation new_R = xnmalloc (size, परिमाण *new_R);
  /* END_R[I] -- next entry of NEW_R[I]. */
  relation end_R = xnmalloc (size, परिमाण *end_R);
  प्रति (relation_गाठ i = 0; i < size; i++)
    {
      relation_गाठ *sp = रिक्त;
      यदि (nedges[i] > 0)
        {
          sp = xnmalloc (nedges[i] + 1, परिमाण *sp);
          sp[nedges[i]] = END_गाठ;
        }
      new_R[i] = sp;
      end_R[i] = sp;
    }

  /* Store. */
  प्रति (relation_गाठ i = 0; i < size; i++)
    यदि (r[i])
      प्रति (relation_गाठ j = 0; r[i][j] != END_गाठ; ++j)
        *end_R[r[i][j]]++ = i;

  मुक्त (nedges);
  मुक्त (end_R);

  /* Free the input: it is replaced with the result. */
  प्रति (relation_गाठ i = 0; i < size; i++)
    मुक्त (r[i]);
  मुक्त (r);

  यदि (trace_flag & trace_sets)
    relation_print ("relation_transpose: output", new_R, size, रिक्त, stderr);

  *R_arg = new_R;
}
