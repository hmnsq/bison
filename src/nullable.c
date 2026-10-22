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

#شامل <config.h>
#شامل "system.h"

#شامل "getargs.h"
#شامل "gram.h"
#شامل "nullable.h"
#شامل "reduce.h"
#شامل "symtab.h"

/* Linked list of rules.  */
تعریفنوع ساختار rule_list
{
  ساختار rule_list *next;
  ثابت rule *مقدار;
} rule_list;

bool *nullable = پوچ;

ایستا تهی
nullable_print (FILE *out)
{
  fputs ("NULLABLE\n", out);
  برای (صحیح i = ntokens; i < nsyms; i++)
    fprintf (out, "  %s: %s\n", symbols[i]->tag,
             nullable[i - ntokens] ? "yes" : "no");
  fputs ("\n\n", out);
}

تهی
nullable_compute (تهی)
{
  nullable = xcalloc (nnterms, اندازه *nullable);

  size_t *rcount = xcalloc (nrules, اندازه *rcount);
  /* RITEM contains all the rules, including useless productions.
     Hence we must allocate room for useless nonterminals too.  */
  rule_list **rsets = xcalloc (nnterms, اندازه *rsets);
  /* This is said to be more elements than we actually use.
     Supposedly NRITEMS - NRULES is enough.  But why take the risk?  */
  rule_list *relts = xnmalloc (nritems + nnterms + 1, اندازه *relts);

  نماد_عدد *squeue = xnmalloc (nnterms, اندازه *squeue);
  نماد_عدد *s2 = squeue;
  {
    rule_list *p = relts;
    برای (rule_عدد ruleno = 0; ruleno < nrules; ++ruleno)
      اگر (rules[ruleno].useful)
        {
          ثابت rule *r = &rules[ruleno];
          اگر (r->rhs[0] >= 0)
            {
              /* This rule has a non empty RHS. */
              bool any_tokens = false;
              برای (item_عدد *rp = r->rhs; *rp >= 0; ++rp)
                اگر (ISTOKEN (*rp))
                  any_tokens = true;

              /* This rule has only nonterminals: schedule it for the second
                 pass.  */
              اگر (!any_tokens)
                برای (item_عدد *rp = r->rhs; *rp >= 0; ++rp)
                  {
                    rcount[ruleno]++;
                    p->next = rsets[*rp - ntokens];
                    p->مقدار = r;
                    rsets[*rp - ntokens] = p;
                    p++;
                  }
            }
          وگرنه
            {
              /* This rule has an empty RHS. */
              اگر (r->useful
                  && ! nullable[r->lhs->عدد - ntokens])
                {
                  nullable[r->lhs->عدد - ntokens] = true;
                  *s2++ = r->lhs->عدد;
                }
            }
        }
  }

  نماد_عدد *s1 = squeue;
  وقتی (s1 < s2)
    برای (rule_list *p = rsets[*s1++ - ntokens]; p; p = p->next)
      {
        ثابت rule *r = p->مقدار;
        اگر (--rcount[r->عدد] == 0)
          اگر (r->useful && ! nullable[r->lhs->عدد - ntokens])
            {
              nullable[r->lhs->عدد - ntokens] = true;
              *s2++ = r->lhs->عدد;
            }
      }

  آزاد (squeue);
  آزاد (rcount);
  آزاد (rsets);
  آزاد (relts);

  اگر (trace_flag & trace_sets)
    nullable_print (stderr);
}


تهی
nullable_free (تهی)
{
  آزاد (nullable);
}
