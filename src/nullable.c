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

#अन्तर्भाव <config.h>
#अन्तर्भाव "system.h"

#अन्तर्भाव "getargs.h"
#अन्तर्भाव "gram.h"
#अन्तर्भाव "nullable.h"
#अन्तर्भाव "reduce.h"
#अन्तर्भाव "symtab.h"

/* Linked list of rules.  */
प्रकारव्याख्या संरचना rule_list
{
  संरचना rule_list *next;
  स्थिरांक rule *मूल्य;
} rule_list;

bool *nullable = रिक्त;

स्थिर शून्य
nullable_print (खाता *out)
{
  ख_माला_दो ("NULLABLE\n", out);
  साठी (पूर्णांक i = ntokens; i < nsyms; i++)
    ख_लिखो (out, "  %s: %s\n", symbols[i]->tag,
             nullable[i - ntokens] ? "yes" : "no");
  ख_माला_दो ("\n\n", out);
}

शून्य
nullable_compute (शून्य)
{
  nullable = xcalloc (nnterms, आकार *nullable);

  माप_प्रकार *rcount = xcalloc (nrules, आकार *rcount);
  /* RITEM contains all the rules, including useless productions.
     Hence we must allocate room for useless nonterminals too.  */
  rule_list **rsets = xcalloc (nnterms, आकार *rsets);
  /* This is said to be more elements than we actually use.
     Supposedly NRITEMS - NRULES is enough.  But why take the risk?  */
  rule_list *relts = xnmalloc (nritems + nnterms + 1, आकार *relts);

  चिन्ह_संख्या *squeue = xnmalloc (nnterms, आकार *squeue);
  चिन्ह_संख्या *s2 = squeue;
  {
    rule_list *p = relts;
    साठी (rule_संख्या ruleno = 0; ruleno < nrules; ++ruleno)
      जर (rules[ruleno].useful)
        {
          स्थिरांक rule *r = &rules[ruleno];
          जर (r->rhs[0] >= 0)
            {
              /* This rule has a non empty RHS. */
              bool any_tokens = false;
              साठी (item_संख्या *rp = r->rhs; *rp >= 0; ++rp)
                जर (ISTOKEN (*rp))
                  any_tokens = true;

              /* This rule has only nonterminals: schedule it for the second
                 pass.  */
              जर (!any_tokens)
                साठी (item_संख्या *rp = r->rhs; *rp >= 0; ++rp)
                  {
                    rcount[ruleno]++;
                    p->next = rsets[*rp - ntokens];
                    p->मूल्य = r;
                    rsets[*rp - ntokens] = p;
                    p++;
                  }
            }
          अन्यथा
            {
              /* This rule has an empty RHS. */
              जर (r->useful
                  && ! nullable[r->lhs->संख्या - ntokens])
                {
                  nullable[r->lhs->संख्या - ntokens] = true;
                  *s2++ = r->lhs->संख्या;
                }
            }
        }
  }

  चिन्ह_संख्या *s1 = squeue;
  पर्यंत (s1 < s2)
    साठी (rule_list *p = rsets[*s1++ - ntokens]; p; p = p->next)
      {
        स्थिरांक rule *r = p->मूल्य;
        जर (--rcount[r->संख्या] == 0)
          जर (r->useful && ! nullable[r->lhs->संख्या - ntokens])
            {
              nullable[r->lhs->संख्या - ntokens] = true;
              *s2++ = r->lhs->संख्या;
            }
      }

  मुक्त (squeue);
  मुक्त (rcount);
  मुक्त (rsets);
  मुक्त (relts);

  जर (trace_flag & trace_sets)
    nullable_print (मानक_त्रुटि);
}


शून्य
nullable_free (शून्य)
{
  मुक्त (nullable);
}
