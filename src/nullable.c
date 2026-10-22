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
  ثابت rule *value;
} rule_list;

bool *nullable = عدم;

ساكن فراغ
nullable_print (FILE *out)
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

  symbol_number *squeue = xnmalloc (nnterms, حجم *squeue);
  symbol_number *s2 = squeue;
  {
    rule_list *p = relts;
    لكل (rule_number ruleno = 0; ruleno < nrules; ++ruleno)
      ܐܢ (rules[ruleno].useful)
        {
          ثابت rule *r = &rules[ruleno];
          ܐܢ (r->rhs[0] >= 0)
            {
              /* This rule has a non empty RHS. */
              bool any_tokens = false;
              لكل (item_number *rp = r->rhs; *rp >= 0; ++rp)
                ܐܢ (ISTOKEN (*rp))
                  any_tokens = true;

              /* This rule has only nonterminals: schedule it for the second
                 pass.  */
              ܐܢ (!any_tokens)
                لكل (item_number *rp = r->rhs; *rp >= 0; ++rp)
                  {
                    rcount[ruleno]++;
                    p->next = rsets[*rp - ntokens];
                    p->value = r;
                    rsets[*rp - ntokens] = p;
                    p++;
                  }
            }
          ܐܚܪܢܐ
            {
              /* This rule has an empty RHS. */
              ܐܢ (r->useful
                  && ! nullable[r->lhs->number - ntokens])
                {
                  nullable[r->lhs->number - ntokens] = true;
                  *s2++ = r->lhs->number;
                }
            }
        }
  }

  symbol_number *s1 = squeue;
  ܟܡܐ (s1 < s2)
    لكل (rule_list *p = rsets[*s1++ - ntokens]; p; p = p->next)
      {
        ثابت rule *r = p->value;
        ܐܢ (--rcount[r->number] == 0)
          ܐܢ (r->useful && ! nullable[r->lhs->number - ntokens])
            {
              nullable[r->lhs->number - ntokens] = true;
              *s2++ = r->lhs->number;
            }
      }

  حرر (squeue);
  حرر (rcount);
  حرر (rsets);
  حرر (relts);

  ܐܢ (trace_flag & trace_sets)
    nullable_print (stderr);
}


فراغ
nullable_free (فراغ)
{
  حرر (nullable);
}
