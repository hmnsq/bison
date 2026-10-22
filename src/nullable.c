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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ "system.h"

#ଅନ୍ତର୍ଭାଵ "getargs.h"
#ଅନ୍ତର୍ଭାଵ "gram.h"
#ଅନ୍ତର୍ଭାଵ "nullable.h"
#ଅନ୍ତର୍ଭାଵ "reduce.h"
#ଅନ୍ତର୍ଭାଵ "symtab.h"

/* Linked list of rules.  */
ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା rule_list
{
  ସଂରଚନା rule_list *next;
  ଅଚଳ rule *ମୂଲ୍ୟ;
} rule_list;

bool *nullable = ଖାଲି;

ସ୍ଥିର ଶୂନ୍ୟ
nullable_print (FILE *out)
{
  fputs ("NULLABLE\n", out);
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = ntokens; i < nsyms; i++)
    fprintf (out, "  %s: %s\n", symbols[i]->tag,
             nullable[i - ntokens] ? "yes" : "no");
  fputs ("\n\n", out);
}

ଶୂନ୍ୟ
nullable_compute (ଶୂନ୍ୟ)
{
  nullable = xcalloc (nnterms, ଆକାର *nullable);

  size_t *rcount = xcalloc (nrules, ଆକାର *rcount);
  /* RITEM contains all the rules, including useless productions.
     Hence we must allocate room for useless nonterminals too.  */
  rule_list **rsets = xcalloc (nnterms, ଆକାର *rsets);
  /* This is said to be more elements than we actually use.
     Supposedly NRITEMS - NRULES is enough.  But why take the risk?  */
  rule_list *relts = xnmalloc (nritems + nnterms + 1, ଆକାର *relts);

  ପ୍ରତୀକ_ସଂଖ୍ୟା *squeue = xnmalloc (nnterms, ଆକାର *squeue);
  ପ୍ରତୀକ_ସଂଖ୍ୟା *s2 = squeue;
  {
    rule_list *p = relts;
    ପାଇଁ (rule_ସଂଖ୍ୟା ruleno = 0; ruleno < nrules; ++ruleno)
      ଯଦି (rules[ruleno].useful)
        {
          ଅଚଳ rule *r = &rules[ruleno];
          ଯଦି (r->rhs[0] >= 0)
            {
              /* This rule has a non empty RHS. */
              bool any_tokens = false;
              ପାଇଁ (item_ସଂଖ୍ୟା *rp = r->rhs; *rp >= 0; ++rp)
                ଯଦି (ISTOKEN (*rp))
                  any_tokens = true;

              /* This rule has only nonterminals: schedule it for the second
                 pass.  */
              ଯଦି (!any_tokens)
                ପାଇଁ (item_ସଂଖ୍ୟା *rp = r->rhs; *rp >= 0; ++rp)
                  {
                    rcount[ruleno]++;
                    p->next = rsets[*rp - ntokens];
                    p->ମୂଲ୍ୟ = r;
                    rsets[*rp - ntokens] = p;
                    p++;
                  }
            }
          ଅନ୍ୟଥା
            {
              /* This rule has an empty RHS. */
              ଯଦି (r->useful
                  && ! nullable[r->lhs->ସଂଖ୍ୟା - ntokens])
                {
                  nullable[r->lhs->ସଂଖ୍ୟା - ntokens] = true;
                  *s2++ = r->lhs->ସଂଖ୍ୟା;
                }
            }
        }
  }

  ପ୍ରତୀକ_ସଂଖ୍ୟା *s1 = squeue;
  ଯେତେବେଳେ (s1 < s2)
    ପାଇଁ (rule_list *p = rsets[*s1++ - ntokens]; p; p = p->next)
      {
        ଅଚଳ rule *r = p->ମୂଲ୍ୟ;
        ଯଦି (--rcount[r->ସଂଖ୍ୟା] == 0)
          ଯଦି (r->useful && ! nullable[r->lhs->ସଂଖ୍ୟା - ntokens])
            {
              nullable[r->lhs->ସଂଖ୍ୟା - ntokens] = true;
              *s2++ = r->lhs->ସଂଖ୍ୟା;
            }
      }

  ମୁକ୍ତ (squeue);
  ମୁକ୍ତ (rcount);
  ମୁକ୍ତ (rsets);
  ମୁକ୍ତ (relts);

  ଯଦି (trace_flag & trace_sets)
    nullable_print (stderr);
}


ଶୂନ୍ୟ
nullable_free (ଶୂନ୍ୟ)
{
  ମୁକ୍ତ (nullable);
}
