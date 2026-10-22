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
تعريفنوع بنية rule_list
{
  بنية rule_list *next;
  ثابت rule *قيمة;
} rule_list;

bool *nullable = عدم;

ساكن فراغ
nullable_اطبع (FILE *out)
{
  fputs ("NULLABLE\n", out);
  لكل (صحيح i = ntokens; i < nsyms; i++)
    fprintf (out, "  %s: %s\n", symbols[i]->tag,
             nullable[i - ntokens] ? "yes" : "no");
  fputs ("\n\n", out);
}

فراغ
nullable_compute (فراغ)
{
  nullable = xcalloc (nnterms, حجم *nullable);

  size_t *rcount = xcalloc (nrules, حجم *rcount);
  /* RITEM contains all the rules, including useless productions.
     Hence we must allocate room for useless nonterminals too.  */
  rule_list **rsets = xcalloc (nnterms, حجم *rsets);
  /* This is said to be more elements than we actually use.
     Supposedly NRITEMS - NRULES is enough.  But why take the risk?  */
  rule_list *relts = xnmalloc (nritems + nnterms + 1, حجم *relts);

  رمز_رقم *squeue = xnmalloc (nnterms, حجم *squeue);
  رمز_رقم *s2 = squeue;
  {
    rule_list *p = relts;
    لكل (rule_رقم ruleno = 0; ruleno < nrules; ++ruleno)
      إذا (rules[ruleno].useful)
        {
          ثابت rule *r = &rules[ruleno];
          إذا (r->rhs[0] >= 0)
            {
              /* This rule has a non empty RHS. */
              bool any_tokens = false;
              لكل (item_رقم *rp = r->rhs; *rp >= 0; ++rp)
                إذا (ISTOKEN (*rp))
                  any_tokens = true;

              /* This rule has only nonterminals: schedule it for the second
                 pass.  */
              إذا (!any_tokens)
                لكل (item_رقم *rp = r->rhs; *rp >= 0; ++rp)
                  {
                    rcount[ruleno]++;
                    p->next = rsets[*rp - ntokens];
                    p->قيمة = r;
                    rsets[*rp - ntokens] = p;
                    p++;
                  }
            }
          وإلا
            {
              /* This rule has an empty RHS. */
              إذا (r->useful
                  && ! nullable[r->lhs->رقم_2 - ntokens])
                {
                  nullable[r->lhs->رقم_2 - ntokens] = true;
                  *s2++ = r->lhs->رقم_2;
                }
            }
        }
  }

  رمز_رقم *s1 = squeue;
  طالما (s1 < s2)
    لكل (rule_list *p = rsets[*s1++ - ntokens]; p; p = p->next)
      {
        ثابت rule *r = p->قيمة;
        إذا (--rcount[r->رقم_2] == 0)
          إذا (r->useful && ! nullable[r->lhs->رقم_2 - ntokens])
            {
              nullable[r->lhs->رقم_2 - ntokens] = true;
              *s2++ = r->lhs->رقم_2;
            }
      }

  حرر (squeue);
  حرر (rcount);
  حرر (rsets);
  حرر (relts);

  إذا (trace_flag & trace_sets)
    nullable_اطبع (stderr);
}


فراغ
nullable_free (فراغ)
{
  حرر (nullable);
}
