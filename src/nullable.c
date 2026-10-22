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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ "system.h"

#અન્તર્ભાવ "getargs.h"
#અન્તર્ભાવ "gram.h"
#અન્તર્ભાવ "nullable.h"
#અન્તર્ભાવ "reduce.h"
#અન્તર્ભાવ "symtab.h"

/* Linked list of rules.  */
પ્રકારવ્યાખ્યા સંરચના rule_list
{
  સંરચના rule_list *next;
  અચલ rule *કિંમત;
} rule_list;

bool *nullable = ખાલી;

સ્થિર શૂન્ય
nullable_print (FILE *out)
{
  fputs ("NULLABLE\n", out);
  માટે (પૂર્ણાંક i = ntokens; i < nsyms; i++)
    fprintf (out, "  %s: %s\n", symbols[i]->tag,
             nullable[i - ntokens] ? "yes" : "no");
  fputs ("\n\n", out);
}

શૂન્ય
nullable_compute (શૂન્ય)
{
  nullable = xcalloc (nnterms, કદ *nullable);

  size_t *rcount = xcalloc (nrules, કદ *rcount);
  /* RITEM contains all the rules, including useless productions.
     Hence we must allocate room for useless nonterminals too.  */
  rule_list **rsets = xcalloc (nnterms, કદ *rsets);
  /* This is said to be more elements than we actually use.
     Supposedly NRITEMS - NRULES is enough.  But why take the risk?  */
  rule_list *relts = xnmalloc (nritems + nnterms + 1, કદ *relts);

  પ્રતીક_સંખ્યા *squeue = xnmalloc (nnterms, કદ *squeue);
  પ્રતીક_સંખ્યા *s2 = squeue;
  {
    rule_list *p = relts;
    માટે (rule_સંખ્યા ruleno = 0; ruleno < nrules; ++ruleno)
      જો (rules[ruleno].useful)
        {
          અચલ rule *r = &rules[ruleno];
          જો (r->rhs[0] >= 0)
            {
              /* This rule has a non empty RHS. */
              bool any_tokens = false;
              માટે (item_સંખ્યા *rp = r->rhs; *rp >= 0; ++rp)
                જો (ISTOKEN (*rp))
                  any_tokens = true;

              /* This rule has only nonterminals: schedule it for the second
                 pass.  */
              જો (!any_tokens)
                માટે (item_સંખ્યા *rp = r->rhs; *rp >= 0; ++rp)
                  {
                    rcount[ruleno]++;
                    p->next = rsets[*rp - ntokens];
                    p->કિંમત = r;
                    rsets[*rp - ntokens] = p;
                    p++;
                  }
            }
          નહીંતર
            {
              /* This rule has an empty RHS. */
              જો (r->useful
                  && ! nullable[r->lhs->સંખ્યા - ntokens])
                {
                  nullable[r->lhs->સંખ્યા - ntokens] = true;
                  *s2++ = r->lhs->સંખ્યા;
                }
            }
        }
  }

  પ્રતીક_સંખ્યા *s1 = squeue;
  જ્યારે (s1 < s2)
    માટે (rule_list *p = rsets[*s1++ - ntokens]; p; p = p->next)
      {
        અચલ rule *r = p->કિંમત;
        જો (--rcount[r->સંખ્યા] == 0)
          જો (r->useful && ! nullable[r->lhs->સંખ્યા - ntokens])
            {
              nullable[r->lhs->સંખ્યા - ntokens] = true;
              *s2++ = r->lhs->સંખ્યા;
            }
      }

  મુક્ત (squeue);
  મુક્ત (rcount);
  મુક્ત (rsets);
  મુક્ત (relts);

  જો (trace_flag & trace_sets)
    nullable_print (stderr);
}


શૂન્ય
nullable_free (શૂન્ય)
{
  મુક્ત (nullable);
}
