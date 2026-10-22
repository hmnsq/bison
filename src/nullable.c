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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ "getargs.h"
#ಅನ್ತರ್ಭಾವ "gram.h"
#ಅನ್ತರ್ಭಾವ "nullable.h"
#ಅನ್ತರ್ಭಾವ "reduce.h"
#ಅನ್ತರ್ಭಾವ "symtab.h"

/* Linked list of rules.  */
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ rule_list
{
  ಸಂರಚನಾ rule_list *next;
  ಧ್ರುವ rule *ಮೌಲ್ಯ;
} rule_list;

bool *nullable = ರಿಕ್ತ;

ಸ್ಥಿರ ಶೂನ್ಯ
nullable_print (FILE *out)
{
  fputs ("NULLABLE\n", out);
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = ntokens; i < nsyms; i++)
    fprintf (out, "  %s: %s\n", symbols[i]->tag,
             nullable[i - ntokens] ? "yes" : "no");
  fputs ("\n\n", out);
}

ಶೂನ್ಯ
nullable_compute (ಶೂನ್ಯ)
{
  nullable = xcalloc (nnterms, ಪರಿಮಾಣ *nullable);

  size_t *rcount = xcalloc (nrules, ಪರಿಮಾಣ *rcount);
  /* RITEM contains all the rules, including useless productions.
     Hence we must allocate room for useless nonterminals too.  */
  rule_list **rsets = xcalloc (nnterms, ಪರಿಮಾಣ *rsets);
  /* This is said to be more elements than we actually use.
     Supposedly NRITEMS - NRULES is enough.  But why take the risk?  */
  rule_list *relts = xnmalloc (nritems + nnterms + 1, ಪರಿಮಾಣ *relts);

  ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ *squeue = xnmalloc (nnterms, ಪರಿಮಾಣ *squeue);
  ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ *s2 = squeue;
  {
    rule_list *p = relts;
    ಪ್ರತಿ (rule_ಸಂಖ್ಯೆ ruleno = 0; ruleno < nrules; ++ruleno)
      ಯದಿ (rules[ruleno].useful)
        {
          ಧ್ರುವ rule *r = &rules[ruleno];
          ಯದಿ (r->rhs[0] >= 0)
            {
              /* This rule has a non empty RHS. */
              bool any_tokens = false;
              ಪ್ರತಿ (item_ಸಂಖ್ಯೆ *rp = r->rhs; *rp >= 0; ++rp)
                ಯದಿ (ISTOKEN (*rp))
                  any_tokens = true;

              /* This rule has only nonterminals: schedule it for the second
                 pass.  */
              ಯದಿ (!any_tokens)
                ಪ್ರತಿ (item_ಸಂಖ್ಯೆ *rp = r->rhs; *rp >= 0; ++rp)
                  {
                    rcount[ruleno]++;
                    p->next = rsets[*rp - ntokens];
                    p->ಮೌಲ್ಯ = r;
                    rsets[*rp - ntokens] = p;
                    p++;
                  }
            }
          ಅನ್ಯಥಾ
            {
              /* This rule has an empty RHS. */
              ಯದಿ (r->useful
                  && ! nullable[r->lhs->ಸಂಖ್ಯೆ - ntokens])
                {
                  nullable[r->lhs->ಸಂಖ್ಯೆ - ntokens] = true;
                  *s2++ = r->lhs->ಸಂಖ್ಯೆ;
                }
            }
        }
  }

  ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ *s1 = squeue;
  ಯಾವತ್ (s1 < s2)
    ಪ್ರತಿ (rule_list *p = rsets[*s1++ - ntokens]; p; p = p->next)
      {
        ಧ್ರುವ rule *r = p->ಮೌಲ್ಯ;
        ಯದಿ (--rcount[r->ಸಂಖ್ಯೆ] == 0)
          ಯದಿ (r->useful && ! nullable[r->lhs->ಸಂಖ್ಯೆ - ntokens])
            {
              nullable[r->lhs->ಸಂಖ್ಯೆ - ntokens] = true;
              *s2++ = r->lhs->ಸಂಖ್ಯೆ;
            }
      }

  ಮುಕ್ತ (squeue);
  ಮುಕ್ತ (rcount);
  ಮುಕ್ತ (rsets);
  ಮುಕ್ತ (relts);

  ಯದಿ (trace_flag & trace_sets)
    nullable_print (stderr);
}


ಶೂನ್ಯ
nullable_free (ಶೂನ್ಯ)
{
  ಮುಕ್ತ (nullable);
}
