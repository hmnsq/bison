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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ "system.h"

#அந்தர்பாவ "getargs.h"

#அந்தர்பாவ "derives.h"
#அந்தர்பாவ "gram.h"
#அந்தர்பாவ "reader.h"
#அந்தர்பாவ "symtab.h"

/* Linked list of rule numbers.  */
வகைவரை கட்டமைப்பு rule_list
{
  கட்டமைப்பு rule_list *next;
  rule *மதிப்பு;
} rule_list;

rule ***derives;

நிலையான வெறுமை
print_derives (வெறுமை)
{
  fputs ("DERIVES\n", stderr);

  க்கு (குறியீடு_எண் i = ntokens; i < nsyms; ++i)
    {
      fprintf (stderr, "  %s derives\n", symbols[i]->tag);
      க்கு (rule **rp = derives[i - ntokens]; *rp; ++rp)
        {
          fprintf (stderr, "    %3d ", (*rp)->code);
          rule_rhs_print (*rp, stderr);
          fprintf (stderr, "\n");
        }
    }

  fputs ("\n\n", stderr);
}


வெறுமை
derives_compute (வெறுமை)
{
  /* DSET[NTERM - NTOKENS] -- A linked list of the numbers of the rules
     whose LHS is NTERM.  */
  rule_list **dset = xcalloc (nnterms, அளவு *dset);

  /* DELTS[RULE] -- There are NRULES rule number to attach to nterms.
     Instead of performing NRULES allocations for each, have an array
     indexed by rule numbers.  */
  rule_list *delts = xnmalloc (nrules, அளவு *delts);

  க்கு (rule_எண் r = nrules - 1; r >= 0; --r)
    {
      குறியீடு_எண் lhs = rules[r].lhs->எண்;
      rule_list *p = &delts[r];
      /* A new LHS is found.  */
      p->next = dset[lhs - ntokens];
      p->மதிப்பு = &rules[r];
      dset[lhs - ntokens] = p;
    }

  /* DSET contains what we need under the form of a linked list.  Make
     it a single array.  */

  derives = xnmalloc (nnterms, அளவு *derives);
  /* Q is the storage for DERIVES[...] (DERIVES[0] = q).  */
  rule **q = xnmalloc (nnterms + nrules, அளவு *q);

  க்கு (குறியீடு_எண் i = ntokens; i < nsyms; ++i)
    {
      rule_list *p = dset[i - ntokens];
      derives[i - ntokens] = q;
      போது (p)
        {
          *q++ = p->மதிப்பு;
          p = p->next;
        }
      *q++ = வெற்று;
    }

  எனில் (trace_flag & trace_sets)
    print_derives ();

  விடுவி (dset);
  விடுவி (delts);
}


வெறுமை
derives_free (வெறுமை)
{
  எனில் (derives)
    {
      விடுவி (derives[0]);
      விடுவி (derives);
    }
}
