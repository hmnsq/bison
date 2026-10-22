/* Match rules with nonterminals for bison,

   Copyright (C) 1984, 1989, 2000-2003, 2005, 2009-2015, 2018-2022 Free
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

#अन्तर्भाव <config.h>
#अन्तर्भाव "system.h"

#अन्तर्भाव "getargs.h"

#अन्तर्भाव "derives.h"
#अन्तर्भाव "gram.h"
#अन्तर्भाव "reader.h"
#अन्तर्भाव "symtab.h"

/* Linked list of rule numbers.  */
प्रकारव्याख्या संरचना rule_list
{
  संरचना rule_list *next;
  rule *मूल्य;
} rule_list;

rule ***derives;

स्थिर शून्य
print_derives (शून्य)
{
  ख_माला_दो ("DERIVES\n", मानक_त्रुटि);

  साठी (चिन्ह_संख्या i = ntokens; i < nsyms; ++i)
    {
      ख_लिखो (मानक_त्रुटि, "  %s derives\n", symbols[i]->tag);
      साठी (rule **rp = derives[i - ntokens]; *rp; ++rp)
        {
          ख_लिखो (मानक_त्रुटि, "    %3d ", (*rp)->code);
          rule_rhs_print (*rp, मानक_त्रुटि);
          ख_लिखो (मानक_त्रुटि, "\n");
        }
    }

  ख_माला_दो ("\n\n", मानक_त्रुटि);
}


शून्य
derives_compute (शून्य)
{
  /* DSET[NTERM - NTOKENS] -- A linked list of the numbers of the rules
     whose LHS is NTERM.  */
  rule_list **dset = xcalloc (nnterms, आकार *dset);

  /* DELTS[RULE] -- There are NRULES rule number to attach to nterms.
     Instead of performing NRULES allocations for each, have an array
     indexed by rule numbers.  */
  rule_list *delts = xnmalloc (nrules, आकार *delts);

  साठी (rule_संख्या r = nrules - 1; r >= 0; --r)
    {
      चिन्ह_संख्या lhs = rules[r].lhs->संख्या;
      rule_list *p = &delts[r];
      /* A new LHS is found.  */
      p->next = dset[lhs - ntokens];
      p->मूल्य = &rules[r];
      dset[lhs - ntokens] = p;
    }

  /* DSET contains what we need under the form of a linked list.  Make
     it a single array.  */

  derives = xnmalloc (nnterms, आकार *derives);
  /* Q is the storage for DERIVES[...] (DERIVES[0] = q).  */
  rule **q = xnmalloc (nnterms + nrules, आकार *q);

  साठी (चिन्ह_संख्या i = ntokens; i < nsyms; ++i)
    {
      rule_list *p = dset[i - ntokens];
      derives[i - ntokens] = q;
      पर्यंत (p)
        {
          *q++ = p->मूल्य;
          p = p->next;
        }
      *q++ = रिक्त;
    }

  जर (trace_flag & trace_sets)
    print_derives ();

  मुक्त (dset);
  मुक्त (delts);
}


शून्य
derives_free (शून्य)
{
  जर (derives)
    {
      मुक्त (derives[0]);
      मुक्त (derives);
    }
}
