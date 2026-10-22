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
प्रकारनिर्देश संरचना rule_list
{
  संरचना rule_list *next;
  ध्रुव rule *मानम्;
} rule_list;

bool *nullable = रिक्त;

स्थिर शून्य
nullable_print (FILE *out)
{
  fputs ("NULLABLE\n", out);
  अत्थाय (पूर्णाङ्क i = ntokens; i < nsyms; i++)
    fprintf (out, "  %s: %s\n", symbols[i]->tag,
             nullable[i - ntokens] ? "yes" : "no");
  fputs ("\n\n", out);
}

शून्य
nullable_compute (शून्य)
{
  nullable = xcalloc (nnterms, परिमाण *nullable);

  size_t *rcount = xcalloc (nrules, परिमाण *rcount);
  /* RITEM contains all the rules, including useless productions.
     Hence we must allocate room for useless nonterminals too.  */
  rule_list **rsets = xcalloc (nnterms, परिमाण *rsets);
  /* This is said to be more elements than we actually use.
     Supposedly NRITEMS - NRULES is enough.  But why take the risk?  */
  rule_list *relts = xnmalloc (nritems + nnterms + 1, परिमाण *relts);

  चिह्नम्_सङ्ख्या *squeue = xnmalloc (nnterms, परिमाण *squeue);
  चिह्नम्_सङ्ख्या *s2 = squeue;
  {
    rule_list *p = relts;
    अत्थाय (rule_सङ्ख्या ruleno = 0; ruleno < nrules; ++ruleno)
      सचे (rules[ruleno].useful)
        {
          ध्रुव rule *r = &rules[ruleno];
          सचे (r->rhs[0] >= 0)
            {
              /* This rule has a non empty RHS. */
              bool any_tokens = false;
              अत्थाय (item_सङ्ख्या *rp = r->rhs; *rp >= 0; ++rp)
                सचे (ISTOKEN (*rp))
                  any_tokens = true;

              /* This rule has only nonterminals: schedule it for the second
                 pass.  */
              सचे (!any_tokens)
                अत्थाय (item_सङ्ख्या *rp = r->rhs; *rp >= 0; ++rp)
                  {
                    rcount[ruleno]++;
                    p->next = rsets[*rp - ntokens];
                    p->मानम् = r;
                    rsets[*rp - ntokens] = p;
                    p++;
                  }
            }
          अञ्ञथा
            {
              /* This rule has an empty RHS. */
              सचे (r->useful
                  && ! nullable[r->lhs->सङ्ख्या - ntokens])
                {
                  nullable[r->lhs->सङ्ख्या - ntokens] = true;
                  *s2++ = r->lhs->सङ्ख्या;
                }
            }
        }
  }

  चिह्नम्_सङ्ख्या *s1 = squeue;
  याव (s1 < s2)
    अत्थाय (rule_list *p = rsets[*s1++ - ntokens]; p; p = p->next)
      {
        ध्रुव rule *r = p->मानम्;
        सचे (--rcount[r->सङ्ख्या] == 0)
          सचे (r->useful && ! nullable[r->lhs->सङ्ख्या - ntokens])
            {
              nullable[r->lhs->सङ्ख्या - ntokens] = true;
              *s2++ = r->lhs->सङ्ख्या;
            }
      }

  मुक्त (squeue);
  मुक्त (rcount);
  मुक्त (rsets);
  मुक्त (relts);

  सचे (trace_flag & trace_sets)
    nullable_print (stderr);
}


शून्य
nullable_free (शून्य)
{
  मुक्त (nullable);
}
