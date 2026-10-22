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
تعريفنوع بنية قاعدہ_list
{
  بنية قاعدہ_list *اگلا;
  ثابت قاعدہ *قدر;
} قاعدہ_list;

bool *nullable = عدم;

ساكن فراغ
nullable_print (FILE *باہرلا)
{
  fputs ("NULLABLE\n", باہرلا);
  لكل (صحيح i = ntokens; i < nsyms; i++)
    fprintf (باہرلا, "  %s: %s\n", symbols[i]->ٹیگ,
             nullable[i - ntokens] ? "yes" : "no");
  fputs ("\n\n", باہرلا);
}

فراغ
nullable_compute (فراغ)
{
  nullable = xcalloc (nnterms, حجم *nullable);

  size_t *rcount = xcalloc (nrules, حجم *rcount);
  /* RITEM contains all the rules, including useless productions.
     Hence we must allocate room for useless nonterminals too.  */
  قاعدہ_list **rsets = xcalloc (nnterms, حجم *rsets);
  /* This is said to be more elements than we actually use.
     Supposedly NRITEMS - NRULES is enough.  But why take the risk?  */
  قاعدہ_list *relts = xnmalloc (nritems + nnterms + 1, حجم *relts);

  علامت_نمبر *squeue = xnmalloc (nnterms, حجم *squeue);
  علامت_نمبر *s2 = squeue;
  {
    قاعدہ_list *p = relts;
    لكل (قاعدہ_نمبر ruleno = 0; ruleno < nrules; ++ruleno)
      إذا (قاعدے[ruleno].useful)
        {
          ثابت قاعدہ *r = &قاعدے[ruleno];
          إذا (r->rhs[0] >= 0)
            {
              /* This rule has a non empty RHS. */
              bool any_ٹوکن = false;
              لكل (شے_نمبر *rp = r->rhs; *rp >= 0; ++rp)
                إذا (ISTOKEN (*rp))
                  any_ٹوکن = true;

              /* This rule has only nonterminals: schedule it for the second
                 pass.  */
              إذا (!any_ٹوکن)
                لكل (شے_نمبر *rp = r->rhs; *rp >= 0; ++rp)
                  {
                    rcount[ruleno]++;
                    p->اگلا = rsets[*rp - ntokens];
                    p->قدر = r;
                    rsets[*rp - ntokens] = p;
                    p++;
                  }
            }
          وإلا
            {
              /* This rule has an empty RHS. */
              إذا (r->useful
                  && ! nullable[r->lhs->نمبر - ntokens])
                {
                  nullable[r->lhs->نمبر - ntokens] = true;
                  *s2++ = r->lhs->نمبر;
                }
            }
        }
  }

  علامت_نمبر *s1 = squeue;
  طالما (s1 < s2)
    لكل (قاعدہ_list *p = rsets[*s1++ - ntokens]; p; p = p->اگلا)
      {
        ثابت قاعدہ *r = p->قدر;
        إذا (--rcount[r->نمبر] == 0)
          إذا (r->useful && ! nullable[r->lhs->نمبر - ntokens])
            {
              nullable[r->lhs->نمبر - ntokens] = true;
              *s2++ = r->lhs->نمبر;
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
