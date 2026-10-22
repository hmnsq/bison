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

#ضمن <config.h>
#ضمن "system.h"

#ضمن <bitsetv.h>

#ضمن "getargs.h"
#ضمن "relation.h"

فراغ
اړیکه_print (ثابت حرف *title,
                اړیکه r, اړیکه_غوټه اندازه,
                اړیکه_غوټه_print print, FILE *بهر)
{
  إذا (title)
    fprintf (بهر, "%s:\n", title);
  لكل (اړیکه_غوټه i = 0; i < اندازه; ++i)
    إذا (r[i])
      {
        fputs ("    ", بهر);
        إذا (print)
          print (i, بهر);
        وإلا
          fprintf (بهر, "%3ld", (اوږد) i);
        fputc (':', بهر);
        لكل (اړیکه_غوټه j = 0; r[i][j] != پای_ټکی_غوټه; ++j)
          {
            fputc (' ', بهر);
            إذا (print)
              print (r[i][j], بهر);
            وإلا
              fprintf (بهر, "%3ld", (اوږد) r[i][j]);
          }
        fputc ('\n', بهر);
      }
  fputc ('\n', بهر);
}


/*---------------------------------------------------------------.
| digraph & traverse.                                            |
|                                                                |
| The following variables are used as common storage between the |
| two.                                                           |
`---------------------------------------------------------------*/

ساكن اړیکه R;
ساكن اړیکه_nodes indexes;
ساكن اړیکه_nodes vertices;
ساكن اړیکه_غوټه سر_2;
ساكن اړیکه_غوټه infinity;
ساكن bitsetv F;

ساكن فراغ
traverse (اړیکه_غوټه i)
{
  vertices[++سر_2] = i;
  اړیکه_غوټه لوړوالی = indexes[i] = سر_2;

  إذا (R[i])
    لكل (اړیکه_غوټه j = 0; R[i][j] != پای_ټکی_غوټه; ++j)
      {
        إذا (indexes[R[i][j]] == 0)
          traverse (R[i][j]);

        إذا (indexes[i] > indexes[R[i][j]])
          indexes[i] = indexes[R[i][j]];

        bitset_or (F[i], F[i], F[R[i][j]]);
      }

  إذا (indexes[i] == لوړوالی)
    لكل (;;)
      {
        اړیکه_غوټه j = vertices[سر_2--];
        indexes[j] = infinity;

        إذا (i == j)
          اكسر;

        bitset_copy (F[j], F[i]);
      }
}


فراغ
اړیکه_digraph (اړیکه r, اړیکه_غوټه اندازه, bitsetv function)
{
  infinity = اندازه + 2;
  indexes = xcalloc (اندازه + 1, حجم *indexes);
  vertices = xnmalloc (اندازه + 1, حجم *vertices);
  سر_2 = 0;

  R = r;
  F = function;

  لكل (اړیکه_غوټه i = 0; i < اندازه; i++)
    إذا (indexes[i] == 0 && R[i])
      traverse (i);

  حرر (indexes);
  حرر (vertices);

  function = F;
}


/*-------------------------------------------.
| Destructively transpose R_ARG, of size N.  |
`-------------------------------------------*/

فراغ
اړیکه_transpose (اړیکه *R_arg, اړیکه_غوټه اندازه)
{
  اړیکه r = *R_arg;

  إذا (trace_flag & trace_sets)
    اړیکه_print ("relation_transpose", r, اندازه, عدم, stderr);

  /* Count. */
  /* NEDGES[I] -- total size of NEW_R[I]. */
  size_t *nedges = xcalloc (اندازه, حجم *nedges);
  لكل (اړیکه_غوټه i = 0; i < اندازه; i++)
    إذا (r[i])
      لكل (اړیکه_غوټه j = 0; r[i][j] != پای_ټکی_غوټه; ++j)
        ++nedges[r[i][j]];

  /* Allocate. */
  /* The result. */
  اړیکه new_R = xnmalloc (اندازه, حجم *new_R);
  /* END_R[I] -- next entry of NEW_R[I]. */
  اړیکه پای_ټکی_R = xnmalloc (اندازه, حجم *پای_ټکی_R);
  لكل (اړیکه_غوټه i = 0; i < اندازه; i++)
    {
      اړیکه_غوټه *sp = عدم;
      إذا (nedges[i] > 0)
        {
          sp = xnmalloc (nedges[i] + 1, حجم *sp);
          sp[nedges[i]] = پای_ټکی_غوټه;
        }
      new_R[i] = sp;
      پای_ټکی_R[i] = sp;
    }

  /* Store. */
  لكل (اړیکه_غوټه i = 0; i < اندازه; i++)
    إذا (r[i])
      لكل (اړیکه_غوټه j = 0; r[i][j] != پای_ټکی_غوټه; ++j)
        *پای_ټکی_R[r[i][j]]++ = i;

  حرر (nedges);
  حرر (پای_ټکی_R);

  /* Free the input: it is replaced with the result. */
  لكل (اړیکه_غوټه i = 0; i < اندازه; i++)
    حرر (r[i]);
  حرر (r);

  إذا (trace_flag & trace_sets)
    اړیکه_print ("relation_transpose: output", new_R, اندازه, عدم, stderr);

  *R_arg = new_R;
}
