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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব "system.h"

#অন্তর্ভাব "getargs.h"

#অন্তর্ভাব "derives.h"
#অন্তর্ভাব "gram.h"
#অন্তর্ভাব "reader.h"
#অন্তর্ভাব "symtab.h"

/* Linked list of rule numbers.  */
প্রকারনির্দেশ সংরচনা rule_list
{
  সংরচনা rule_list *next;
  rule *মান;
} rule_list;

rule ***derives;

স্থির শূন্য
print_derives (শূন্য)
{
  fputs ("DERIVES\n", stderr);

  প্রতি (প্রতীক_সংখ্যা i = ntokens; i < nsyms; ++i)
    {
      fprintf (stderr, "  %s derives\n", symbols[i]->tag);
      প্রতি (rule **rp = derives[i - ntokens]; *rp; ++rp)
        {
          fprintf (stderr, "    %3d ", (*rp)->code);
          rule_rhs_print (*rp, stderr);
          fprintf (stderr, "\n");
        }
    }

  fputs ("\n\n", stderr);
}


শূন্য
derives_compute (শূন্য)
{
  /* DSET[NTERM - NTOKENS] -- A linked list of the numbers of the rules
     whose LHS is NTERM.  */
  rule_list **dset = xcalloc (nnterms, পরিমাণ *dset);

  /* DELTS[RULE] -- There are NRULES rule number to attach to nterms.
     Instead of performing NRULES allocations for each, have an array
     indexed by rule numbers.  */
  rule_list *delts = xnmalloc (nrules, পরিমাণ *delts);

  প্রতি (rule_সংখ্যা r = nrules - 1; r >= 0; --r)
    {
      প্রতীক_সংখ্যা lhs = rules[r].lhs->সংখ্যা;
      rule_list *p = &delts[r];
      /* A new LHS is found.  */
      p->next = dset[lhs - ntokens];
      p->মান = &rules[r];
      dset[lhs - ntokens] = p;
    }

  /* DSET contains what we need under the form of a linked list.  Make
     it a single array.  */

  derives = xnmalloc (nnterms, পরিমাণ *derives);
  /* Q is the storage for DERIVES[...] (DERIVES[0] = q).  */
  rule **q = xnmalloc (nnterms + nrules, পরিমাণ *q);

  প্রতি (প্রতীক_সংখ্যা i = ntokens; i < nsyms; ++i)
    {
      rule_list *p = dset[i - ntokens];
      derives[i - ntokens] = q;
      যাবত্ (p)
        {
          *q++ = p->মান;
          p = p->next;
        }
      *q++ = রিক্ত;
    }

  যদি (trace_flag & trace_sets)
    print_derives ();

  মুক্ত (dset);
  মুক্ত (delts);
}


শূন্য
derives_free (শূন্য)
{
  যদি (derives)
    {
      মুক্ত (derives[0]);
      মুক্ত (derives);
    }
}
