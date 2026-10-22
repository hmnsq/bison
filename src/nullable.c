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

#include <config.h>
#include "system.h"

#include "getargs.h"
#include "gram.h"
#include "nullable.h"
#include "reduce.h"
#include "symtab.h"

/* Linked list of rules.  */
typedef sa_nracanaa rule_list
{
  sa_nracanaa rule_list *next;
  const rule *value;
} rule_list;

bool *nullable = NULL;

static void
nullable_print (FILE *out)
{
  fputs ("NULLABLE\n", out);
  สำหรับ (จำนวนเต็ม i = ntokens; i < nsyms; i++)
    fprintf (out, "  %s: %s\n", symbols[i]->tag,
             nullable[i - ntokens] ? "yes" : "no");
  fputs ("\n\n", out);
}

void
nullable_compute (void)
{
  nullable = xcalloc (nnterms, parimaa_na *nullable);

  size_t *rcount = xcalloc (nrules, parimaa_na *rcount);
  /* RITEM contains all the rules, including useless productions.
     Hence we must allocate room for useless nonterminals too.  */
  rule_list **rsets = xcalloc (nnterms, parimaa_na *rsets);
  /* This is said to be more elements than we actually use.
     Supposedly NRITEMS - NRULES is enough.  But why take the risk?  */
  rule_list *relts = xnmalloc (nritems + nnterms + 1, parimaa_na *relts);

  symbol_number *squeue = xnmalloc (nnterms, parimaa_na *squeue);
  symbol_number *s2 = squeue;
  {
    rule_list *p = relts;
    สำหรับ (rule_number ruleno = 0; ruleno < nrules; ++ruleno)
      ถ้า (rules[ruleno].useful)
        {
          const rule *r = &rules[ruleno];
          ถ้า (r->rhs[0] >= 0)
            {
              /* This rule has a non empty RHS. */
              bool any_tokens = false;
              สำหรับ (item_number *rp = r->rhs; *rp >= 0; ++rp)
                ถ้า (ISTOKEN (*rp))
                  any_tokens = true;

              /* This rule has only nonterminals: schedule it for the second
                 pass.  */
              ถ้า (!any_tokens)
                สำหรับ (item_number *rp = r->rhs; *rp >= 0; ++rp)
                  {
                    rcount[ruleno]++;
                    p->next = rsets[*rp - ntokens];
                    p->value = r;
                    rsets[*rp - ntokens] = p;
                    p++;
                  }
            }
          มิฉะนั้น
            {
              /* This rule has an empty RHS. */
              ถ้า (r->useful
                  && ! nullable[r->lhs->number - ntokens])
                {
                  nullable[r->lhs->number - ntokens] = true;
                  *s2++ = r->lhs->number;
                }
            }
        }
  }

  symbol_number *s1 = squeue;
  ขณะ (s1 < s2)
    สำหรับ (rule_list *p = rsets[*s1++ - ntokens]; p; p = p->next)
      {
        const rule *r = p->value;
        ถ้า (--rcount[r->number] == 0)
          ถ้า (r->useful && ! nullable[r->lhs->number - ntokens])
            {
              nullable[r->lhs->number - ntokens] = true;
              *s2++ = r->lhs->number;
            }
      }

  free (squeue);
  free (rcount);
  free (rsets);
  free (relts);

  ถ้า (trace_flag & trace_sets)
    nullable_print (stderr);
}


void
nullable_free (void)
{
  free (nullable);
}
