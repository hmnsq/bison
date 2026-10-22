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
प्रकारनिर्देश संरचना नियमः_list
{
  संरचना नियमः_list *अग्र;
  ध्रुव नियमः *मानम्;
} नियमः_list;

bool *nullable = रिक्त;

स्थिर शून्य
nullable_print (FILE *बहिर्गमनम्)
{
  fputs ("NULLABLE\n", बहिर्गमनम्);
  प्रति (पूर्णाङ्क i = ntokens; i < nsyms; i++)
    fprintf (बहिर्गमनम्, "  %s: %s\n", symbols[i]->चिह्नकम्,
             nullable[i - ntokens] ? "yes" : "no");
  fputs ("\n\n", बहिर्गमनम्);
}

शून्य
nullable_compute (शून्य)
{
  nullable = xcalloc (nnterms, परिमाण *nullable);

  size_t *rcount = xcalloc (nrules, परिमाण *rcount);
  /* RITEM contains all the rules, including useless productions.
     Hence we must allocate room for useless nonterminals too.  */
  नियमः_list **rsets = xcalloc (nnterms, परिमाण *rsets);
  /* This is said to be more elements than we actually use.
     Supposedly NRITEMS - NRULES is enough.  But why take the risk?  */
  नियमः_list *relts = xnmalloc (nritems + nnterms + 1, परिमाण *relts);

  चिह्नम्_सङ्ख्या *squeue = xnmalloc (nnterms, परिमाण *squeue);
  चिह्नम्_सङ्ख्या *s2 = squeue;
  {
    नियमः_list *p = relts;
    प्रति (नियमः_सङ्ख्या ruleno = 0; ruleno < nrules; ++ruleno)
      यदि (नियमाः[ruleno].useful)
        {
          ध्रुव नियमः *r = &नियमाः[ruleno];
          यदि (r->rhs[0] >= 0)
            {
              /* This rule has a non empty RHS. */
              bool any_tokens = false;
              प्रति (item_सङ्ख्या *rp = r->rhs; *rp >= 0; ++rp)
                यदि (ISTOKEN (*rp))
                  any_tokens = true;

              /* This rule has only nonterminals: schedule it for the second
                 pass.  */
              यदि (!any_tokens)
                प्रति (item_सङ्ख्या *rp = r->rhs; *rp >= 0; ++rp)
                  {
                    rcount[ruleno]++;
                    p->अग्र = rsets[*rp - ntokens];
                    p->मानम् = r;
                    rsets[*rp - ntokens] = p;
                    p++;
                  }
            }
          अन्यथा
            {
              /* This rule has an empty RHS. */
              यदि (r->useful
                  && ! nullable[r->lhs->सङ्ख्या - ntokens])
                {
                  nullable[r->lhs->सङ्ख्या - ntokens] = true;
                  *s2++ = r->lhs->सङ्ख्या;
                }
            }
        }
  }

  चिह्नम्_सङ्ख्या *s1 = squeue;
  यावत् (s1 < s2)
    प्रति (नियमः_list *p = rsets[*s1++ - ntokens]; p; p = p->अग्र)
      {
        ध्रुव नियमः *r = p->मानम्;
        यदि (--rcount[r->सङ्ख्या] == 0)
          यदि (r->useful && ! nullable[r->lhs->सङ्ख्या - ntokens])
            {
              nullable[r->lhs->सङ्ख्या - ntokens] = true;
              *s2++ = r->lhs->सङ्ख्या;
            }
      }

  मुक्त (squeue);
  मुक्त (rcount);
  मुक्त (rsets);
  मुक्त (relts);

  यदि (trace_flag & trace_sets)
    nullable_print (stderr);
}


शून्य
nullable_free (शून्य)
{
  मुक्त (nullable);
}
