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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ "system.h"

#ଅନ୍ତର୍ଭାଵ "getargs.h"

#ଅନ୍ତର୍ଭାଵ "derives.h"
#ଅନ୍ତର୍ଭାଵ "gram.h"
#ଅନ୍ତର୍ଭାଵ "reader.h"
#ଅନ୍ତର୍ଭାଵ "symtab.h"

/* Linked list of rule numbers.  */
ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା rule_list
{
  ସଂରଚନା rule_list *next;
  rule *ମୂଲ୍ୟ;
} rule_list;

rule ***derives;

ସ୍ଥିର ଶୂନ୍ୟ
print_derives (ଶୂନ୍ୟ)
{
  fputs ("DERIVES\n", stderr);

  ପାଇଁ (ପ୍ରତୀକ_ସଂଖ୍ୟା i = ntokens; i < nsyms; ++i)
    {
      fprintf (stderr, "  %s derives\n", symbols[i]->tag);
      ପାଇଁ (rule **rp = derives[i - ntokens]; *rp; ++rp)
        {
          fprintf (stderr, "    %3d ", (*rp)->code);
          rule_rhs_print (*rp, stderr);
          fprintf (stderr, "\n");
        }
    }

  fputs ("\n\n", stderr);
}


ଶୂନ୍ୟ
derives_compute (ଶୂନ୍ୟ)
{
  /* DSET[NTERM - NTOKENS] -- A linked list of the numbers of the rules
     whose LHS is NTERM.  */
  rule_list **dset = xcalloc (nnterms, ଆକାର *dset);

  /* DELTS[RULE] -- There are NRULES rule number to attach to nterms.
     Instead of performing NRULES allocations for each, have an array
     indexed by rule numbers.  */
  rule_list *delts = xnmalloc (nrules, ଆକାର *delts);

  ପାଇଁ (rule_ସଂଖ୍ୟା r = nrules - 1; r >= 0; --r)
    {
      ପ୍ରତୀକ_ସଂଖ୍ୟା lhs = rules[r].lhs->ସଂଖ୍ୟା;
      rule_list *p = &delts[r];
      /* A new LHS is found.  */
      p->next = dset[lhs - ntokens];
      p->ମୂଲ୍ୟ = &rules[r];
      dset[lhs - ntokens] = p;
    }

  /* DSET contains what we need under the form of a linked list.  Make
     it a single array.  */

  derives = xnmalloc (nnterms, ଆକାର *derives);
  /* Q is the storage for DERIVES[...] (DERIVES[0] = q).  */
  rule **q = xnmalloc (nnterms + nrules, ଆକାର *q);

  ପାଇଁ (ପ୍ରତୀକ_ସଂଖ୍ୟା i = ntokens; i < nsyms; ++i)
    {
      rule_list *p = dset[i - ntokens];
      derives[i - ntokens] = q;
      ଯେତେବେଳେ (p)
        {
          *q++ = p->ମୂଲ୍ୟ;
          p = p->next;
        }
      *q++ = ଖାଲି;
    }

  ଯଦି (trace_flag & trace_sets)
    print_derives ();

  ମୁକ୍ତ (dset);
  ମୁକ୍ତ (delts);
}


ଶୂନ୍ୟ
derives_free (ଶୂନ୍ୟ)
{
  ଯଦି (derives)
    {
      ମୁକ୍ତ (derives[0]);
      ମୁକ୍ତ (derives);
    }
}
