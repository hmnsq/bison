/* Calculate which nonterminals can expand into the null string for Bison.

   Copyright (C) 1984, 1989, 2000-2006, 2009-2015, 2018-2022 Free
   Software Foundation, Inc.

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


/* Set up NULLABLE, a vector saying which nonterminals can expand into
   the null string.  NULLABLE[I - NTOKENS] is nonzero if symbol I can
   do so.  */

#ضمن <config.h>
#ضمن "system.h"

#ضمن "getargs.h"
#ضمن "gram.h"
#ضمن "nullable.h"
#ضمن "reduce.h"
#ضمن "symtab.h"

/* Linked list of rules.  */
تعريفنوع بنية قاعده_list
{
  بنية قاعده_list *بل;
  ثابت قاعده *ارزښت;
} قاعده_list;

bool *nullable = عدم;

ساكن فراغ
nullable_print (FILE *بهر)
{
  fputs ("NULLABLE\n", بهر);
  لكل (صحيح i = ntokens; i < nsyms; i++)
    fprintf (بهر, "  %s: %s\n", symbols[i]->نښان,
             nullable[i - ntokens] ? "yes" : "no");
  fputs ("\n\n", بهر);
}

فراغ
nullable_compute (فراغ)
{
  nullable = xcalloc (nnterms, حجم *nullable);

  size_t *rcount = xcalloc (nrules, حجم *rcount);
  /* RITEM contains all the rules, including useless productions.
     Hence we must allocate room for useless nonterminals too.  */
  قاعده_list **rsets = xcalloc (nnterms, حجم *rsets);
  /* This is said to be more elements than we actually use.
     Supposedly NRITEMS - NRULES is enough.  But why take the risk?  */
  قاعده_list *relts = xnmalloc (nritems + nnterms + 1, حجم *relts);

  نښه_شمېره *squeue = xnmalloc (nnterms, حجم *squeue);
  نښه_شمېره *s2 = squeue;
  {
    قاعده_list *p = relts;
    لكل (قاعده_شمېره ruleno = 0; ruleno < nrules; ++ruleno)
      إذا (قواعد[ruleno].useful)
        {
          ثابت قاعده *r = &قواعد[ruleno];
          إذا (r->rhs[0] >= 0)
            {
              /* This rule has a non empty RHS. */
              bool any_نښې = false;
              لكل (توکی_شمېره *rp = r->rhs; *rp >= 0; ++rp)
                إذا (ISTOKEN (*rp))
                  any_نښې = true;

              /* This rule has only nonterminals: schedule it for the second
                 pass.  */
              إذا (!any_نښې)
                لكل (توکی_شمېره *rp = r->rhs; *rp >= 0; ++rp)
                  {
                    rcount[ruleno]++;
                    p->بل = rsets[*rp - ntokens];
                    p->ارزښت = r;
                    rsets[*rp - ntokens] = p;
                    p++;
                  }
            }
          وإلا
            {
              /* This rule has an empty RHS. */
              إذا (r->useful
                  && ! nullable[r->lhs->شمېره - ntokens])
                {
                  nullable[r->lhs->شمېره - ntokens] = true;
                  *s2++ = r->lhs->شمېره;
                }
            }
        }
  }

  نښه_شمېره *s1 = squeue;
  طالما (s1 < s2)
    لكل (قاعده_list *p = rsets[*s1++ - ntokens]; p; p = p->بل)
      {
        ثابت قاعده *r = p->ارزښت;
        إذا (--rcount[r->شمېره] == 0)
          إذا (r->useful && ! nullable[r->lhs->شمېره - ntokens])
            {
              nullable[r->lhs->شمېره - ntokens] = true;
              *s2++ = r->lhs->شمېره;
            }
      }

  حرر (squeue);
  حرر (rcount);
  حرر (rsets);
  حرر (relts);

  إذا (trace_flag & trace_sets)
    nullable_print (stderr);
}


فراغ
nullable_free (فراغ)
{
  حرر (nullable);
}
