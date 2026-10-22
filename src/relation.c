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

#समावेश <config.h>
#समावेश "system.h"

#समावेश <bitsetv.h>

#समावेश "getargs.h"
#समावेश "relation.h"

व्योम
relation_print (स्थिर अक्षर *title,
                relation r, relation_गाँठ आकार,
                relation_गाँठ_print print, खाता *out)
{
  अगर (title)
    ख_लिखो (out, "%s:\n", title);
  क्रम (relation_गाँठ i = 0; i < आकार; ++i)
    अगर (r[i])
      {
        ख_माला_दो ("    ", out);
        अगर (print)
          print (i, out);
        अन्यथा
          ख_लिखो (out, "%3ld", (दीर्घ) i);
        ख_अक्षर_दो (':', out);
        क्रम (relation_गाँठ j = 0; r[i][j] != END_गाँठ; ++j)
          {
            ख_अक्षर_दो (' ', out);
            अगर (print)
              print (r[i][j], out);
            अन्यथा
              ख_लिखो (out, "%3ld", (दीर्घ) r[i][j]);
          }
        ख_अक्षर_दो ('\n', out);
      }
  ख_अक्षर_दो ('\n', out);
}


/*---------------------------------------------------------------.
| digraph & traverse.                                            |
|                                                                |
| The following variables are used as common storage between the |
| two.                                                           |
`---------------------------------------------------------------*/

अटल relation R;
अटल relation_nodes indexes;
अटल relation_nodes vertices;
अटल relation_गाँठ शीर्ष_2;
अटल relation_गाँठ infinity;
अटल bitsetv F;

अटल व्योम
traverse (relation_गाँठ i)
{
  vertices[++शीर्ष_2] = i;
  relation_गाँठ height = indexes[i] = शीर्ष_2;

  अगर (R[i])
    क्रम (relation_गाँठ j = 0; R[i][j] != END_गाँठ; ++j)
      {
        अगर (indexes[R[i][j]] == 0)
          traverse (R[i][j]);

        अगर (indexes[i] > indexes[R[i][j]])
          indexes[i] = indexes[R[i][j]];

        bitset_or (F[i], F[i], F[R[i][j]]);
      }

  अगर (indexes[i] == height)
    क्रम (;;)
      {
        relation_गाँठ j = vertices[शीर्ष_2--];
        indexes[j] = infinity;

        अगर (i == j)
          अवरोध;

        bitset_copy (F[j], F[i]);
      }
}


व्योम
relation_digraph (relation r, relation_गाँठ आकार, bitsetv function)
{
  infinity = आकार + 2;
  indexes = xcalloc (आकार + 1, माप *indexes);
  vertices = xnmalloc (आकार + 1, माप *vertices);
  शीर्ष_2 = 0;

  R = r;
  F = function;

  क्रम (relation_गाँठ i = 0; i < आकार; i++)
    अगर (indexes[i] == 0 && R[i])
      traverse (i);

  मुक्त (indexes);
  मुक्त (vertices);

  function = F;
}


/*-------------------------------------------.
| Destructively transpose R_ARG, of size N.  |
`-------------------------------------------*/

व्योम
relation_transpose (relation *R_arg, relation_गाँठ आकार)
{
  relation r = *R_arg;

  अगर (trace_flag & trace_sets)
    relation_print ("relation_transpose", r, आकार, शून्य, मानक_त्रुटि);

  /* Count. */
  /* NEDGES[I] -- total size of NEW_R[I]. */
  माप_प्रकार *nedges = xcalloc (आकार, माप *nedges);
  क्रम (relation_गाँठ i = 0; i < आकार; i++)
    अगर (r[i])
      क्रम (relation_गाँठ j = 0; r[i][j] != END_गाँठ; ++j)
        ++nedges[r[i][j]];

  /* Allocate. */
  /* The result. */
  relation new_R = xnmalloc (आकार, माप *new_R);
  /* END_R[I] -- next entry of NEW_R[I]. */
  relation end_R = xnmalloc (आकार, माप *end_R);
  क्रम (relation_गाँठ i = 0; i < आकार; i++)
    {
      relation_गाँठ *sp = शून्य;
      अगर (nedges[i] > 0)
        {
          sp = xnmalloc (nedges[i] + 1, माप *sp);
          sp[nedges[i]] = END_गाँठ;
        }
      new_R[i] = sp;
      end_R[i] = sp;
    }

  /* Store. */
  क्रम (relation_गाँठ i = 0; i < आकार; i++)
    अगर (r[i])
      क्रम (relation_गाँठ j = 0; r[i][j] != END_गाँठ; ++j)
        *end_R[r[i][j]]++ = i;

  मुक्त (nedges);
  मुक्त (end_R);

  /* Free the input: it is replaced with the result. */
  क्रम (relation_गाँठ i = 0; i < आकार; i++)
    मुक्त (r[i]);
  मुक्त (r);

  अगर (trace_flag & trace_sets)
    relation_print ("relation_transpose: output", new_R, आकार, शून्य, मानक_त्रुटि);

  *R_arg = new_R;
}
