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

#समावेश <config.h>
#समावेश "system.h"

#समावेश "getargs.h"
#समावेश "gram.h"
#समावेश "nullable.h"
#समावेश "reduce.h"
#समावेश "symtab.h"

/* Linked list of rules.  */
प्रकार काष्ठा rule_list
{
  काष्ठा rule_list *अग्र;
  स्थिर rule *मूल्य;
} rule_list;

bool *nullable = शून्य;

अटल व्योम
nullable_print (खाता *out)
{
  ख_माला_दो ("NULLABLE\n", out);
  क्रम (पूर्णांक i = ntokens; i < nsyms; i++)
    ख_लिखो (out, "  %s: %s\n", symbols[i]->tag,
             nullable[i - ntokens] ? "yes" : "no");
  ख_माला_दो ("\n\n", out);
}

व्योम
nullable_compute (व्योम)
{
  nullable = xcalloc (nnterms, माप *nullable);

  माप_प्रकार *rcount = xcalloc (nrules, माप *rcount);
  /* RITEM contains all the rules, including useless productions.
     Hence we must allocate room for useless nonterminals too.  */
  rule_list **rsets = xcalloc (nnterms, माप *rsets);
  /* This is said to be more elements than we actually use.
     Supposedly NRITEMS - NRULES is enough.  But why take the risk?  */
  rule_list *relts = xnmalloc (nritems + nnterms + 1, माप *relts);

  प्रतीक_संख्या *squeue = xnmalloc (nnterms, माप *squeue);
  प्रतीक_संख्या *s2 = squeue;
  {
    rule_list *p = relts;
    क्रम (rule_संख्या ruleno = 0; ruleno < nrules; ++ruleno)
      अगर (rules[ruleno].useful)
        {
          स्थिर rule *r = &rules[ruleno];
          अगर (r->rhs[0] >= 0)
            {
              /* This rule has a non empty RHS. */
              bool any_tokens = false;
              क्रम (item_संख्या *rp = r->rhs; *rp >= 0; ++rp)
                अगर (ISTOKEN (*rp))
                  any_tokens = true;

              /* This rule has only nonterminals: schedule it for the second
                 pass.  */
              अगर (!any_tokens)
                क्रम (item_संख्या *rp = r->rhs; *rp >= 0; ++rp)
                  {
                    rcount[ruleno]++;
                    p->अग्र = rsets[*rp - ntokens];
                    p->मूल्य = r;
                    rsets[*rp - ntokens] = p;
                    p++;
                  }
            }
          अन्यथा
            {
              /* This rule has an empty RHS. */
              अगर (r->useful
                  && ! nullable[r->lhs->संख्या - ntokens])
                {
                  nullable[r->lhs->संख्या - ntokens] = true;
                  *s2++ = r->lhs->संख्या;
                }
            }
        }
  }

  प्रतीक_संख्या *s1 = squeue;
  जबतक (s1 < s2)
    क्रम (rule_list *p = rsets[*s1++ - ntokens]; p; p = p->अग्र)
      {
        स्थिर rule *r = p->मूल्य;
        अगर (--rcount[r->संख्या] == 0)
          अगर (r->useful && ! nullable[r->lhs->संख्या - ntokens])
            {
              nullable[r->lhs->संख्या - ntokens] = true;
              *s2++ = r->lhs->संख्या;
            }
      }

  मुक्त (squeue);
  मुक्त (rcount);
  मुक्त (rsets);
  मुक्त (relts);

  अगर (trace_flag & trace_sets)
    nullable_print (मानक_त्रुटि);
}


व्योम
nullable_free (व्योम)
{
  मुक्त (nullable);
}
