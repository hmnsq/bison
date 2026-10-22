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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ "system.h"

#അന്തര്ഭാവ <bitsetv.h>

#അന്തര്ഭാവ "getargs.h"
#അന്തര്ഭാവ "relation.h"

ശൂന്യം
relation_print (സ്ഥിരം അക്ഷരം *title,
                relation r, relation_കണ്ണി size,
                relation_കണ്ണി_print print, FILE *out)
{
  എങ്കിൽ (title)
    fprintf (out, "%s:\n", title);
  വേണ്ടി (relation_കണ്ണി i = 0; i < size; ++i)
    എങ്കിൽ (r[i])
      {
        fputs ("    ", out);
        എങ്കിൽ (print)
          print (i, out);
        അല്ലെങ്കിൽ
          fprintf (out, "%3ld", (long) i);
        fputc (':', out);
        വേണ്ടി (relation_കണ്ണി j = 0; r[i][j] != END_കണ്ണി; ++j)
          {
            fputc (' ', out);
            എങ്കിൽ (print)
              print (r[i][j], out);
            അല്ലെങ്കിൽ
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

സ്ഥിതം relation R;
സ്ഥിതം relation_nodes indexes;
സ്ഥിതം relation_nodes vertices;
സ്ഥിതം relation_കണ്ണി മുകൾ_2;
സ്ഥിതം relation_കണ്ണി infinity;
സ്ഥിതം bitsetv F;

സ്ഥിതം ശൂന്യം
traverse (relation_കണ്ണി i)
{
  vertices[++മുകൾ_2] = i;
  relation_കണ്ണി height = indexes[i] = മുകൾ_2;

  എങ്കിൽ (R[i])
    വേണ്ടി (relation_കണ്ണി j = 0; R[i][j] != END_കണ്ണി; ++j)
      {
        എങ്കിൽ (indexes[R[i][j]] == 0)
          traverse (R[i][j]);

        എങ്കിൽ (indexes[i] > indexes[R[i][j]])
          indexes[i] = indexes[R[i][j]];

        bitset_or (F[i], F[i], F[R[i][j]]);
      }

  എങ്കിൽ (indexes[i] == height)
    വേണ്ടി (;;)
      {
        relation_കണ്ണി j = vertices[മുകൾ_2--];
        indexes[j] = infinity;

        എങ്കിൽ (i == j)
          നിർത്തുക;

        bitset_copy (F[j], F[i]);
      }
}


ശൂന്യം
relation_digraph (relation r, relation_കണ്ണി size, bitsetv function)
{
  infinity = size + 2;
  indexes = xcalloc (size + 1, വലുപ്പം *indexes);
  vertices = xnmalloc (size + 1, വലുപ്പം *vertices);
  മുകൾ_2 = 0;

  R = r;
  F = function;

  വേണ്ടി (relation_കണ്ണി i = 0; i < size; i++)
    എങ്കിൽ (indexes[i] == 0 && R[i])
      traverse (i);

  സ്വതന്ത്രം (indexes);
  സ്വതന്ത്രം (vertices);

  function = F;
}


/*-------------------------------------------.
| Destructively transpose R_ARG, of size N.  |
`-------------------------------------------*/

ശൂന്യം
relation_transpose (relation *R_arg, relation_കണ്ണി size)
{
  relation r = *R_arg;

  എങ്കിൽ (trace_flag & trace_sets)
    relation_print ("relation_transpose", r, size, ശൂന്യ, stderr);

  /* Count. */
  /* NEDGES[I] -- total size of NEW_R[I]. */
  size_t *nedges = xcalloc (size, വലുപ്പം *nedges);
  വേണ്ടി (relation_കണ്ണി i = 0; i < size; i++)
    എങ്കിൽ (r[i])
      വേണ്ടി (relation_കണ്ണി j = 0; r[i][j] != END_കണ്ണി; ++j)
        ++nedges[r[i][j]];

  /* Allocate. */
  /* The result. */
  relation new_R = xnmalloc (size, വലുപ്പം *new_R);
  /* END_R[I] -- next entry of NEW_R[I]. */
  relation end_R = xnmalloc (size, വലുപ്പം *end_R);
  വേണ്ടി (relation_കണ്ണി i = 0; i < size; i++)
    {
      relation_കണ്ണി *sp = ശൂന്യ;
      എങ്കിൽ (nedges[i] > 0)
        {
          sp = xnmalloc (nedges[i] + 1, വലുപ്പം *sp);
          sp[nedges[i]] = END_കണ്ണി;
        }
      new_R[i] = sp;
      end_R[i] = sp;
    }

  /* Store. */
  വേണ്ടി (relation_കണ്ണി i = 0; i < size; i++)
    എങ്കിൽ (r[i])
      വേണ്ടി (relation_കണ്ണി j = 0; r[i][j] != END_കണ്ണി; ++j)
        *end_R[r[i][j]]++ = i;

  സ്വതന്ത്രം (nedges);
  സ്വതന്ത്രം (end_R);

  /* Free the input: it is replaced with the result. */
  വേണ്ടി (relation_കണ്ണി i = 0; i < size; i++)
    സ്വതന്ത്രം (r[i]);
  സ്വതന്ത്രം (r);

  എങ്കിൽ (trace_flag & trace_sets)
    relation_print ("relation_transpose: output", new_R, size, ശൂന്യ, stderr);

  *R_arg = new_R;
}
