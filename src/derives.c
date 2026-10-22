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

#समावेश <config.h>
#समावेश "system.h"

#समावेश "getargs.h"

#समावेश "derives.h"
#समावेश "gram.h"
#समावेश "reader.h"
#समावेश "symtab.h"

/* Linked list of rule numbers.  */
प्रकार काष्ठा rule_list
{
  काष्ठा rule_list *अग्र;
  rule *मूल्य;
} rule_list;

rule ***derives;

अटल व्योम
print_derives (व्योम)
{
  ख_माला_दो ("DERIVES\n", मानक_त्रुटि);

  क्रम (प्रतीक_संख्या i = ntokens; i < nsyms; ++i)
    {
      ख_लिखो (मानक_त्रुटि, "  %s derives\n", symbols[i]->tag);
      क्रम (rule **rp = derives[i - ntokens]; *rp; ++rp)
        {
          ख_लिखो (मानक_त्रुटि, "    %3d ", (*rp)->code);
          rule_rhs_print (*rp, मानक_त्रुटि);
          ख_लिखो (मानक_त्रुटि, "\n");
        }
    }

  ख_माला_दो ("\n\n", मानक_त्रुटि);
}


व्योम
derives_compute (व्योम)
{
  /* DSET[NTERM - NTOKENS] -- A linked list of the numbers of the rules
     whose LHS is NTERM.  */
  rule_list **dset = xcalloc (nnterms, माप *dset);

  /* DELTS[RULE] -- There are NRULES rule number to attach to nterms.
     Instead of performing NRULES allocations for each, have an array
     indexed by rule numbers.  */
  rule_list *delts = xnmalloc (nrules, माप *delts);

  क्रम (rule_संख्या r = nrules - 1; r >= 0; --r)
    {
      प्रतीक_संख्या lhs = rules[r].lhs->संख्या;
      rule_list *p = &delts[r];
      /* A new LHS is found.  */
      p->अग्र = dset[lhs - ntokens];
      p->मूल्य = &rules[r];
      dset[lhs - ntokens] = p;
    }

  /* DSET contains what we need under the form of a linked list.  Make
     it a single array.  */

  derives = xnmalloc (nnterms, माप *derives);
  /* Q is the storage for DERIVES[...] (DERIVES[0] = q).  */
  rule **q = xnmalloc (nnterms + nrules, माप *q);

  क्रम (प्रतीक_संख्या i = ntokens; i < nsyms; ++i)
    {
      rule_list *p = dset[i - ntokens];
      derives[i - ntokens] = q;
      जबतक (p)
        {
          *q++ = p->मूल्य;
          p = p->अग्र;
        }
      *q++ = शून्य;
    }

  अगर (trace_flag & trace_sets)
    print_derives ();

  मुक्त (dset);
  मुक्त (delts);
}


व्योम
derives_free (व्योम)
{
  अगर (derives)
    {
      मुक्त (derives[0]);
      मुक्त (derives);
    }
}
