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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ "system.h"

#അന്തര്ഭാവ "getargs.h"
#അന്തര്ഭാവ "gram.h"
#അന്തര്ഭാവ "nullable.h"
#അന്തര്ഭാവ "reduce.h"
#അന്തര്ഭാവ "symtab.h"

/* Linked list of rules.  */
തരനിർവചനം ഘടന rule_list
{
  ഘടന rule_list *next;
  സ്ഥിരം rule *മൂല്യം;
} rule_list;

bool *nullable = ശൂന്യ;

സ്ഥിതം ശൂന്യം
nullable_print (FILE *out)
{
  fputs ("NULLABLE\n", out);
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = ntokens; i < nsyms; i++)
    fprintf (out, "  %s: %s\n", symbols[i]->tag,
             nullable[i - ntokens] ? "yes" : "no");
  fputs ("\n\n", out);
}

ശൂന്യം
nullable_compute (ശൂന്യം)
{
  nullable = xcalloc (nnterms, വലുപ്പം *nullable);

  size_t *rcount = xcalloc (nrules, വലുപ്പം *rcount);
  /* RITEM contains all the rules, including useless productions.
     Hence we must allocate room for useless nonterminals too.  */
  rule_list **rsets = xcalloc (nnterms, വലുപ്പം *rsets);
  /* This is said to be more elements than we actually use.
     Supposedly NRITEMS - NRULES is enough.  But why take the risk?  */
  rule_list *relts = xnmalloc (nritems + nnterms + 1, വലുപ്പം *relts);

  ചിഹ്നം_സംഖ്യ *squeue = xnmalloc (nnterms, വലുപ്പം *squeue);
  ചിഹ്നം_സംഖ്യ *s2 = squeue;
  {
    rule_list *p = relts;
    വേണ്ടി (rule_സംഖ്യ ruleno = 0; ruleno < nrules; ++ruleno)
      എങ്കിൽ (rules[ruleno].useful)
        {
          സ്ഥിരം rule *r = &rules[ruleno];
          എങ്കിൽ (r->rhs[0] >= 0)
            {
              /* This rule has a non empty RHS. */
              bool any_tokens = false;
              വേണ്ടി (item_സംഖ്യ *rp = r->rhs; *rp >= 0; ++rp)
                എങ്കിൽ (ISTOKEN (*rp))
                  any_tokens = true;

              /* This rule has only nonterminals: schedule it for the second
                 pass.  */
              എങ്കിൽ (!any_tokens)
                വേണ്ടി (item_സംഖ്യ *rp = r->rhs; *rp >= 0; ++rp)
                  {
                    rcount[ruleno]++;
                    p->next = rsets[*rp - ntokens];
                    p->മൂല്യം = r;
                    rsets[*rp - ntokens] = p;
                    p++;
                  }
            }
          അല്ലെങ്കിൽ
            {
              /* This rule has an empty RHS. */
              എങ്കിൽ (r->useful
                  && ! nullable[r->lhs->സംഖ്യ - ntokens])
                {
                  nullable[r->lhs->സംഖ്യ - ntokens] = true;
                  *s2++ = r->lhs->സംഖ്യ;
                }
            }
        }
  }

  ചിഹ്നം_സംഖ്യ *s1 = squeue;
  ആകുമ്പോൾ (s1 < s2)
    വേണ്ടി (rule_list *p = rsets[*s1++ - ntokens]; p; p = p->next)
      {
        സ്ഥിരം rule *r = p->മൂല്യം;
        എങ്കിൽ (--rcount[r->സംഖ്യ] == 0)
          എങ്കിൽ (r->useful && ! nullable[r->lhs->സംഖ്യ - ntokens])
            {
              nullable[r->lhs->സംഖ്യ - ntokens] = true;
              *s2++ = r->lhs->സംഖ്യ;
            }
      }

  സ്വതന്ത്രം (squeue);
  സ്വതന്ത്രം (rcount);
  സ്വതന്ത്രം (rsets);
  സ്വതന്ത്രം (relts);

  എങ്കിൽ (trace_flag & trace_sets)
    nullable_print (stderr);
}


ശൂന്യം
nullable_free (ശൂന്യം)
{
  സ്വതന്ത്രം (nullable);
}
