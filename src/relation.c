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
सम्बन्धः_print (ध्रुव वर्ण *title,
                सम्बन्धः r, सम्बन्धः_ग्रन्थिः आकार,
                सम्बन्धः_ग्रन्थिः_print print, FILE *बहिर्गमनम्)
{
  यदि (title)
    fprintf (बहिर्गमनम्, "%s:\n", title);
  प्रति (सम्बन्धः_ग्रन्थिः i = 0; i < आकार; ++i)
    यदि (r[i])
      {
        fputs ("    ", बहिर्गमनम्);
        यदि (print)
          print (i, बहिर्गमनम्);
        अन्यथा
          fprintf (बहिर्गमनम्, "%3ld", (दीर्घ) i);
        fputc (':', बहिर्गमनम्);
        प्रति (सम्बन्धः_ग्रन्थिः j = 0; r[i][j] != अन्तः_ग्रन्थिः; ++j)
          {
            fputc (' ', बहिर्गमनम्);
            यदि (print)
              print (r[i][j], बहिर्गमनम्);
            अन्यथा
              fprintf (बहिर्गमनम्, "%3ld", (दीर्घ) r[i][j]);
          }
        fputc ('\n', बहिर्गमनम्);
      }
  fputc ('\n', बहिर्गमनम्);
}


/*---------------------------------------------------------------.
| digraph & traverse.                                            |
|                                                                |
| The following variables are used as common storage between the |
| two.                                                           |
`---------------------------------------------------------------*/

स्थिर सम्बन्धः R;
स्थिर सम्बन्धः_nodes indexes;
स्थिर सम्बन्धः_nodes vertices;
स्थिर सम्बन्धः_ग्रन्थिः शिखरम्_2;
स्थिर सम्बन्धः_ग्रन्थिः infinity;
स्थिर bitsetv F;

स्थिर शून्य
traverse (सम्बन्धः_ग्रन्थिः i)
{
  vertices[++शिखरम्_2] = i;
  सम्बन्धः_ग्रन्थिः height = indexes[i] = शिखरम्_2;

  यदि (R[i])
    प्रति (सम्बन्धः_ग्रन्थिः j = 0; R[i][j] != अन्तः_ग्रन्थिः; ++j)
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
        सम्बन्धः_ग्रन्थिः j = vertices[शिखरम्_2--];
        indexes[j] = infinity;

        यदि (i == j)
          भङ्ग;

        bitset_copy (F[j], F[i]);
      }
}


शून्य
सम्बन्धः_digraph (सम्बन्धः r, सम्बन्धः_ग्रन्थिः आकार, bitsetv function)
{
  infinity = आकार + 2;
  indexes = xcalloc (आकार + 1, परिमाण *indexes);
  vertices = xnmalloc (आकार + 1, परिमाण *vertices);
  शिखरम्_2 = 0;

  R = r;
  F = function;

  प्रति (सम्बन्धः_ग्रन्थिः i = 0; i < आकार; i++)
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
सम्बन्धः_transpose (सम्बन्धः *R_arg, सम्बन्धः_ग्रन्थिः आकार)
{
  सम्बन्धः r = *R_arg;

  यदि (trace_flag & trace_sets)
    सम्बन्धः_print ("relation_transpose", r, आकार, रिक्त, stderr);

  /* Count. */
  /* NEDGES[I] -- total size of NEW_R[I]. */
  size_t *nedges = xcalloc (आकार, परिमाण *nedges);
  प्रति (सम्बन्धः_ग्रन्थिः i = 0; i < आकार; i++)
    यदि (r[i])
      प्रति (सम्बन्धः_ग्रन्थिः j = 0; r[i][j] != अन्तः_ग्रन्थिः; ++j)
        ++nedges[r[i][j]];

  /* Allocate. */
  /* The result. */
  सम्बन्धः new_R = xnmalloc (आकार, परिमाण *new_R);
  /* END_R[I] -- next entry of NEW_R[I]. */
  सम्बन्धः अन्तः_R = xnmalloc (आकार, परिमाण *अन्तः_R);
  प्रति (सम्बन्धः_ग्रन्थिः i = 0; i < आकार; i++)
    {
      सम्बन्धः_ग्रन्थिः *sp = रिक्त;
      यदि (nedges[i] > 0)
        {
          sp = xnmalloc (nedges[i] + 1, परिमाण *sp);
          sp[nedges[i]] = अन्तः_ग्रन्थिः;
        }
      new_R[i] = sp;
      अन्तः_R[i] = sp;
    }

  /* Store. */
  प्रति (सम्बन्धः_ग्रन्थिः i = 0; i < आकार; i++)
    यदि (r[i])
      प्रति (सम्बन्धः_ग्रन्थिः j = 0; r[i][j] != अन्तः_ग्रन्थिः; ++j)
        *अन्तः_R[r[i][j]]++ = i;

  मुक्त (nedges);
  मुक्त (अन्तः_R);

  /* Free the input: it is replaced with the result. */
  प्रति (सम्बन्धः_ग्रन्थिः i = 0; i < आकार; i++)
    मुक्त (r[i]);
  मुक्त (r);

  यदि (trace_flag & trace_sets)
    सम्बन्धः_print ("relation_transpose: output", new_R, आकार, रिक्त, stderr);

  *R_arg = new_R;
}
