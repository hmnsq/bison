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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ "system.h"

#ଅନ୍ତର୍ଭାଵ <bitsetv.h>

#ଅନ୍ତର୍ଭାଵ "getargs.h"
#ଅନ୍ତର୍ଭାଵ "relation.h"

ଶୂନ୍ୟ
relation_print (ଅଚଳ ଅକ୍ଷର *title,
                relation r, relation_ଗଣ୍ଠି size,
                relation_ଗଣ୍ଠି_print print, FILE *out)
{
  ଯଦି (title)
    fprintf (out, "%s:\n", title);
  ପାଇଁ (relation_ଗଣ୍ଠି i = 0; i < size; ++i)
    ଯଦି (r[i])
      {
        fputs ("    ", out);
        ଯଦି (print)
          print (i, out);
        ଅନ୍ୟଥା
          fprintf (out, "%3ld", (long) i);
        fputc (':', out);
        ପାଇଁ (relation_ଗଣ୍ଠି j = 0; r[i][j] != END_ଗଣ୍ଠି; ++j)
          {
            fputc (' ', out);
            ଯଦି (print)
              print (r[i][j], out);
            ଅନ୍ୟଥା
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

ସ୍ଥିର relation R;
ସ୍ଥିର relation_nodes indexes;
ସ୍ଥିର relation_nodes vertices;
ସ୍ଥିର relation_ଗଣ୍ଠି ଶୀର୍ଷ_2;
ସ୍ଥିର relation_ଗଣ୍ଠି infinity;
ସ୍ଥିର bitsetv F;

ସ୍ଥିର ଶୂନ୍ୟ
traverse (relation_ଗଣ୍ଠି i)
{
  vertices[++ଶୀର୍ଷ_2] = i;
  relation_ଗଣ୍ଠି height = indexes[i] = ଶୀର୍ଷ_2;

  ଯଦି (R[i])
    ପାଇଁ (relation_ଗଣ୍ଠି j = 0; R[i][j] != END_ଗଣ୍ଠି; ++j)
      {
        ଯଦି (indexes[R[i][j]] == 0)
          traverse (R[i][j]);

        ଯଦି (indexes[i] > indexes[R[i][j]])
          indexes[i] = indexes[R[i][j]];

        bitset_or (F[i], F[i], F[R[i][j]]);
      }

  ଯଦି (indexes[i] == height)
    ପାଇଁ (;;)
      {
        relation_ଗଣ୍ଠି j = vertices[ଶୀର୍ଷ_2--];
        indexes[j] = infinity;

        ଯଦି (i == j)
          ଭାଙ୍ଗ;

        bitset_copy (F[j], F[i]);
      }
}


ଶୂନ୍ୟ
relation_digraph (relation r, relation_ଗଣ୍ଠି size, bitsetv function)
{
  infinity = size + 2;
  indexes = xcalloc (size + 1, ଆକାର *indexes);
  vertices = xnmalloc (size + 1, ଆକାର *vertices);
  ଶୀର୍ଷ_2 = 0;

  R = r;
  F = function;

  ପାଇଁ (relation_ଗଣ୍ଠି i = 0; i < size; i++)
    ଯଦି (indexes[i] == 0 && R[i])
      traverse (i);

  ମୁକ୍ତ (indexes);
  ମୁକ୍ତ (vertices);

  function = F;
}


/*-------------------------------------------.
| Destructively transpose R_ARG, of size N.  |
`-------------------------------------------*/

ଶୂନ୍ୟ
relation_transpose (relation *R_arg, relation_ଗଣ୍ଠି size)
{
  relation r = *R_arg;

  ଯଦି (trace_flag & trace_sets)
    relation_print ("relation_transpose", r, size, ଖାଲି, stderr);

  /* Count. */
  /* NEDGES[I] -- total size of NEW_R[I]. */
  size_t *nedges = xcalloc (size, ଆକାର *nedges);
  ପାଇଁ (relation_ଗଣ୍ଠି i = 0; i < size; i++)
    ଯଦି (r[i])
      ପାଇଁ (relation_ଗଣ୍ଠି j = 0; r[i][j] != END_ଗଣ୍ଠି; ++j)
        ++nedges[r[i][j]];

  /* Allocate. */
  /* The result. */
  relation new_R = xnmalloc (size, ଆକାର *new_R);
  /* END_R[I] -- next entry of NEW_R[I]. */
  relation end_R = xnmalloc (size, ଆକାର *end_R);
  ପାଇଁ (relation_ଗଣ୍ଠି i = 0; i < size; i++)
    {
      relation_ଗଣ୍ଠି *sp = ଖାଲି;
      ଯଦି (nedges[i] > 0)
        {
          sp = xnmalloc (nedges[i] + 1, ଆକାର *sp);
          sp[nedges[i]] = END_ଗଣ୍ଠି;
        }
      new_R[i] = sp;
      end_R[i] = sp;
    }

  /* Store. */
  ପାଇଁ (relation_ଗଣ୍ଠି i = 0; i < size; i++)
    ଯଦି (r[i])
      ପାଇଁ (relation_ଗଣ୍ଠି j = 0; r[i][j] != END_ଗଣ୍ଠି; ++j)
        *end_R[r[i][j]]++ = i;

  ମୁକ୍ତ (nedges);
  ମୁକ୍ତ (end_R);

  /* Free the input: it is replaced with the result. */
  ପାଇଁ (relation_ଗଣ୍ଠି i = 0; i < size; i++)
    ମୁକ୍ତ (r[i]);
  ମୁକ୍ତ (r);

  ଯଦି (trace_flag & trace_sets)
    relation_print ("relation_transpose: output", new_R, size, ଖାଲି, stderr);

  *R_arg = new_R;
}
