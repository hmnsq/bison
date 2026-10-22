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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব "system.h"

#অন্তর্ভাব "getargs.h"
#অন্তর্ভাব "gram.h"
#অন্তর্ভাব "nullable.h"
#অন্তর্ভাব "reduce.h"
#অন্তর্ভাব "symtab.h"

/* Linked list of rules.  */
প্রকারলেখ কাঠামো rule_list
{
  কাঠামো rule_list *পরবর্তী;
  ধ্রুবক rule *মান;
} rule_list;

bool *nullable = ফাঁকা;

স্থির শূন্য
nullable_print (FILE *out)
{
  fputs ("NULLABLE\n", out);
  জন্য (পূর্ণসংখ্যা i = ntokens; i < nsyms; i++)
    fprintf (out, "  %s: %s\n", symbols[i]->tag,
             nullable[i - ntokens] ? "yes" : "no");
  fputs ("\n\n", out);
}

শূন্য
nullable_compute (শূন্য)
{
  nullable = xcalloc (nnterms, আকার *nullable);

  size_t *rcount = xcalloc (nrules, আকার *rcount);
  /* RITEM contains all the rules, including useless productions.
     Hence we must allocate room for useless nonterminals too.  */
  rule_list **rsets = xcalloc (nnterms, আকার *rsets);
  /* This is said to be more elements than we actually use.
     Supposedly NRITEMS - NRULES is enough.  But why take the risk?  */
  rule_list *relts = xnmalloc (nritems + nnterms + 1, আকার *relts);

  প্রতীক_সংখ্যা *squeue = xnmalloc (nnterms, আকার *squeue);
  প্রতীক_সংখ্যা *s2 = squeue;
  {
    rule_list *p = relts;
    জন্য (rule_সংখ্যা ruleno = 0; ruleno < nrules; ++ruleno)
      যদি (rules[ruleno].useful)
        {
          ধ্রুবক rule *r = &rules[ruleno];
          যদি (r->rhs[0] >= 0)
            {
              /* This rule has a non empty RHS. */
              bool any_tokens = false;
              জন্য (item_সংখ্যা *rp = r->rhs; *rp >= 0; ++rp)
                যদি (ISTOKEN (*rp))
                  any_tokens = true;

              /* This rule has only nonterminals: schedule it for the second
                 pass.  */
              যদি (!any_tokens)
                জন্য (item_সংখ্যা *rp = r->rhs; *rp >= 0; ++rp)
                  {
                    rcount[ruleno]++;
                    p->পরবর্তী = rsets[*rp - ntokens];
                    p->মান = r;
                    rsets[*rp - ntokens] = p;
                    p++;
                  }
            }
          অন্যথা
            {
              /* This rule has an empty RHS. */
              যদি (r->useful
                  && ! nullable[r->lhs->সংখ্যা - ntokens])
                {
                  nullable[r->lhs->সংখ্যা - ntokens] = true;
                  *s2++ = r->lhs->সংখ্যা;
                }
            }
        }
  }

  প্রতীক_সংখ্যা *s1 = squeue;
  যখন (s1 < s2)
    জন্য (rule_list *p = rsets[*s1++ - ntokens]; p; p = p->পরবর্তী)
      {
        ধ্রুবক rule *r = p->মান;
        যদি (--rcount[r->সংখ্যা] == 0)
          যদি (r->useful && ! nullable[r->lhs->সংখ্যা - ntokens])
            {
              nullable[r->lhs->সংখ্যা - ntokens] = true;
              *s2++ = r->lhs->সংখ্যা;
            }
      }

  মুক্ত (squeue);
  মুক্ত (rcount);
  মুক্ত (rsets);
  মুক্ত (relts);

  যদি (trace_flag & trace_sets)
    nullable_print (stderr);
}


শূন্য
nullable_free (শূন্য)
{
  মুক্ত (nullable);
}
