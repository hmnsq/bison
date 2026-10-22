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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ "system.h"

#அந்தர்பாவ "getargs.h"
#அந்தர்பாவ "gram.h"
#அந்தர்பாவ "nullable.h"
#அந்தர்பாவ "reduce.h"
#அந்தர்பாவ "symtab.h"

/* Linked list of rules.  */
வகைவரை கட்டமைப்பு rule_list
{
  கட்டமைப்பு rule_list *next;
  மாறிலி rule *மதிப்பு;
} rule_list;

bool *nullable = வெற்று;

நிலையான வெறுமை
nullable_print (FILE *out)
{
  fputs ("NULLABLE\n", out);
  க்கு (முழு i = ntokens; i < nsyms; i++)
    fprintf (out, "  %s: %s\n", symbols[i]->tag,
             nullable[i - ntokens] ? "yes" : "no");
  fputs ("\n\n", out);
}

வெறுமை
nullable_compute (வெறுமை)
{
  nullable = xcalloc (nnterms, அளவு *nullable);

  size_t *rcount = xcalloc (nrules, அளவு *rcount);
  /* RITEM contains all the rules, including useless productions.
     Hence we must allocate room for useless nonterminals too.  */
  rule_list **rsets = xcalloc (nnterms, அளவு *rsets);
  /* This is said to be more elements than we actually use.
     Supposedly NRITEMS - NRULES is enough.  But why take the risk?  */
  rule_list *relts = xnmalloc (nritems + nnterms + 1, அளவு *relts);

  குறியீடு_எண் *squeue = xnmalloc (nnterms, அளவு *squeue);
  குறியீடு_எண் *s2 = squeue;
  {
    rule_list *p = relts;
    க்கு (rule_எண் ruleno = 0; ruleno < nrules; ++ruleno)
      எனில் (rules[ruleno].useful)
        {
          மாறிலி rule *r = &rules[ruleno];
          எனில் (r->rhs[0] >= 0)
            {
              /* This rule has a non empty RHS. */
              bool any_tokens = false;
              க்கு (item_எண் *rp = r->rhs; *rp >= 0; ++rp)
                எனில் (ISTOKEN (*rp))
                  any_tokens = true;

              /* This rule has only nonterminals: schedule it for the second
                 pass.  */
              எனில் (!any_tokens)
                க்கு (item_எண் *rp = r->rhs; *rp >= 0; ++rp)
                  {
                    rcount[ruleno]++;
                    p->next = rsets[*rp - ntokens];
                    p->மதிப்பு = r;
                    rsets[*rp - ntokens] = p;
                    p++;
                  }
            }
          இல்லையெனில்
            {
              /* This rule has an empty RHS. */
              எனில் (r->useful
                  && ! nullable[r->lhs->எண் - ntokens])
                {
                  nullable[r->lhs->எண் - ntokens] = true;
                  *s2++ = r->lhs->எண்;
                }
            }
        }
  }

  குறியீடு_எண் *s1 = squeue;
  போது (s1 < s2)
    க்கு (rule_list *p = rsets[*s1++ - ntokens]; p; p = p->next)
      {
        மாறிலி rule *r = p->மதிப்பு;
        எனில் (--rcount[r->எண்] == 0)
          எனில் (r->useful && ! nullable[r->lhs->எண் - ntokens])
            {
              nullable[r->lhs->எண் - ntokens] = true;
              *s2++ = r->lhs->எண்;
            }
      }

  விடுவி (squeue);
  விடுவி (rcount);
  விடுவி (rsets);
  விடுவி (relts);

  எனில் (trace_flag & trace_sets)
    nullable_print (stderr);
}


வெறுமை
nullable_free (வெறுமை)
{
  விடுவி (nullable);
}
