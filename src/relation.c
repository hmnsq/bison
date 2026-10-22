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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ <bitsetv.h>

#ಅನ್ತರ್ಭಾವ "getargs.h"
#ಅನ್ತರ್ಭಾವ "relation.h"

ಶೂನ್ಯ
ಸಂಬಂಧ_print (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *title,
                ಸಂಬಂಧ r, ಸಂಬಂಧ_ಗಂಟು ಗಾತ್ರ_2,
                ಸಂಬಂಧ_ಗಂಟು_print print, FILE *ಹೊರಹೋಗು)
{
  ಆದರೆ (title)
    fprintf (ಹೊರಹೋಗು, "%s:\n", title);
  ಗಾಗಿ (ಸಂಬಂಧ_ಗಂಟು i = 0; i < ಗಾತ್ರ_2; ++i)
    ಆದರೆ (r[i])
      {
        fputs ("    ", ಹೊರಹೋಗು);
        ಆದರೆ (print)
          print (i, ಹೊರಹೋಗು);
        ಇಲ್ಲದಿದ್ದರೆ
          fprintf (ಹೊರಹೋಗು, "%3ld", (ದೀರ್ಘ) i);
        fputc (':', ಹೊರಹೋಗು);
        ಗಾಗಿ (ಸಂಬಂಧ_ಗಂಟು j = 0; r[i][j] != ಕೊನೆ_ಗಂಟು; ++j)
          {
            fputc (' ', ಹೊರಹೋಗು);
            ಆದರೆ (print)
              print (r[i][j], ಹೊರಹೋಗು);
            ಇಲ್ಲದಿದ್ದರೆ
              fprintf (ಹೊರಹೋಗು, "%3ld", (ದೀರ್ಘ) r[i][j]);
          }
        fputc ('\n', ಹೊರಹೋಗು);
      }
  fputc ('\n', ಹೊರಹೋಗು);
}


/*---------------------------------------------------------------.
| digraph & traverse.                                            |
|                                                                |
| The following variables are used as common storage between the |
| two.                                                           |
`---------------------------------------------------------------*/

ಸ್ಥಿರ ಸಂಬಂಧ R;
ಸ್ಥಿರ ಸಂಬಂಧ_nodes indexes;
ಸ್ಥಿರ ಸಂಬಂಧ_nodes vertices;
ಸ್ಥಿರ ಸಂಬಂಧ_ಗಂಟು ತುದಿ_2;
ಸ್ಥಿರ ಸಂಬಂಧ_ಗಂಟು infinity;
ಸ್ಥಿರ bitsetv F;

ಸ್ಥಿರ ಶೂನ್ಯ
traverse (ಸಂಬಂಧ_ಗಂಟು i)
{
  vertices[++ತುದಿ_2] = i;
  ಸಂಬಂಧ_ಗಂಟು ಎತ್ತರ = indexes[i] = ತುದಿ_2;

  ಆದರೆ (R[i])
    ಗಾಗಿ (ಸಂಬಂಧ_ಗಂಟು j = 0; R[i][j] != ಕೊನೆ_ಗಂಟು; ++j)
      {
        ಆದರೆ (indexes[R[i][j]] == 0)
          traverse (R[i][j]);

        ಆದರೆ (indexes[i] > indexes[R[i][j]])
          indexes[i] = indexes[R[i][j]];

        bitset_or (F[i], F[i], F[R[i][j]]);
      }

  ಆದರೆ (indexes[i] == ಎತ್ತರ)
    ಗಾಗಿ (;;)
      {
        ಸಂಬಂಧ_ಗಂಟು j = vertices[ತುದಿ_2--];
        indexes[j] = infinity;

        ಆದರೆ (i == j)
          ನಿಲ್ಲಿಸು;

        bitset_copy (F[j], F[i]);
      }
}


ಶೂನ್ಯ
ಸಂಬಂಧ_digraph (ಸಂಬಂಧ r, ಸಂಬಂಧ_ಗಂಟು ಗಾತ್ರ_2, bitsetv function)
{
  infinity = ಗಾತ್ರ_2 + 2;
  indexes = xcalloc (ಗಾತ್ರ_2 + 1, ಗಾತ್ರ *indexes);
  vertices = xnmalloc (ಗಾತ್ರ_2 + 1, ಗಾತ್ರ *vertices);
  ತುದಿ_2 = 0;

  R = r;
  F = function;

  ಗಾಗಿ (ಸಂಬಂಧ_ಗಂಟು i = 0; i < ಗಾತ್ರ_2; i++)
    ಆದರೆ (indexes[i] == 0 && R[i])
      traverse (i);

  ಬಿಡುಗಡೆ (indexes);
  ಬಿಡುಗಡೆ (vertices);

  function = F;
}


/*-------------------------------------------.
| Destructively transpose R_ARG, of size N.  |
`-------------------------------------------*/

ಶೂನ್ಯ
ಸಂಬಂಧ_transpose (ಸಂಬಂಧ *R_arg, ಸಂಬಂಧ_ಗಂಟು ಗಾತ್ರ_2)
{
  ಸಂಬಂಧ r = *R_arg;

  ಆದರೆ (trace_flag & trace_sets)
    ಸಂಬಂಧ_print ("relation_transpose", r, ಗಾತ್ರ_2, ಖಾಲಿ, stderr);

  /* Count. */
  /* NEDGES[I] -- total size of NEW_R[I]. */
  size_t *nedges = xcalloc (ಗಾತ್ರ_2, ಗಾತ್ರ *nedges);
  ಗಾಗಿ (ಸಂಬಂಧ_ಗಂಟು i = 0; i < ಗಾತ್ರ_2; i++)
    ಆದರೆ (r[i])
      ಗಾಗಿ (ಸಂಬಂಧ_ಗಂಟು j = 0; r[i][j] != ಕೊನೆ_ಗಂಟು; ++j)
        ++nedges[r[i][j]];

  /* Allocate. */
  /* The result. */
  ಸಂಬಂಧ new_R = xnmalloc (ಗಾತ್ರ_2, ಗಾತ್ರ *new_R);
  /* END_R[I] -- next entry of NEW_R[I]. */
  ಸಂಬಂಧ ಕೊನೆ_R = xnmalloc (ಗಾತ್ರ_2, ಗಾತ್ರ *ಕೊನೆ_R);
  ಗಾಗಿ (ಸಂಬಂಧ_ಗಂಟು i = 0; i < ಗಾತ್ರ_2; i++)
    {
      ಸಂಬಂಧ_ಗಂಟು *sp = ಖಾಲಿ;
      ಆದರೆ (nedges[i] > 0)
        {
          sp = xnmalloc (nedges[i] + 1, ಗಾತ್ರ *sp);
          sp[nedges[i]] = ಕೊನೆ_ಗಂಟು;
        }
      new_R[i] = sp;
      ಕೊನೆ_R[i] = sp;
    }

  /* Store. */
  ಗಾಗಿ (ಸಂಬಂಧ_ಗಂಟು i = 0; i < ಗಾತ್ರ_2; i++)
    ಆದರೆ (r[i])
      ಗಾಗಿ (ಸಂಬಂಧ_ಗಂಟು j = 0; r[i][j] != ಕೊನೆ_ಗಂಟು; ++j)
        *ಕೊನೆ_R[r[i][j]]++ = i;

  ಬಿಡುಗಡೆ (nedges);
  ಬಿಡುಗಡೆ (ಕೊನೆ_R);

  /* Free the input: it is replaced with the result. */
  ಗಾಗಿ (ಸಂಬಂಧ_ಗಂಟು i = 0; i < ಗಾತ್ರ_2; i++)
    ಬಿಡುಗಡೆ (r[i]);
  ಬಿಡುಗಡೆ (r);

  ಆದರೆ (trace_flag & trace_sets)
    ಸಂಬಂಧ_print ("relation_transpose: output", new_R, ಗಾತ್ರ_2, ಖಾಲಿ, stderr);

  *R_arg = new_R;
}
