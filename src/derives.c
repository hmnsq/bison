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

#شامل <config.h>
#شامل "system.h"

#شامل "getargs.h"

#شامل "derives.h"
#شامل "gram.h"
#شامل "reader.h"
#شامل "symtab.h"

/* Linked list of rule numbers.  */
تعریفنوع ساختار rule_list
{
  ساختار rule_list *next;
  rule *مقدار;
} rule_list;

rule ***derives;

ایستا تهی
print_derives (تهی)
{
  fputs ("DERIVES\n", stderr);

  برای (نماد_عدد i = ntokens; i < nsyms; ++i)
    {
      fprintf (stderr, "  %s derives\n", symbols[i]->tag);
      برای (rule **rp = derives[i - ntokens]; *rp; ++rp)
        {
          fprintf (stderr, "    %3d ", (*rp)->code);
          rule_rhs_print (*rp, stderr);
          fprintf (stderr, "\n");
        }
    }

  fputs ("\n\n", stderr);
}


تهی
derives_compute (تهی)
{
  /* DSET[NTERM - NTOKENS] -- A linked list of the numbers of the rules
     whose LHS is NTERM.  */
  rule_list **dset = xcalloc (nnterms, اندازه *dset);

  /* DELTS[RULE] -- There are NRULES rule number to attach to nterms.
     Instead of performing NRULES allocations for each, have an array
     indexed by rule numbers.  */
  rule_list *delts = xnmalloc (nrules, اندازه *delts);

  برای (rule_عدد r = nrules - 1; r >= 0; --r)
    {
      نماد_عدد lhs = rules[r].lhs->عدد;
      rule_list *p = &delts[r];
      /* A new LHS is found.  */
      p->next = dset[lhs - ntokens];
      p->مقدار = &rules[r];
      dset[lhs - ntokens] = p;
    }

  /* DSET contains what we need under the form of a linked list.  Make
     it a single array.  */

  derives = xnmalloc (nnterms, اندازه *derives);
  /* Q is the storage for DERIVES[...] (DERIVES[0] = q).  */
  rule **q = xnmalloc (nnterms + nrules, اندازه *q);

  برای (نماد_عدد i = ntokens; i < nsyms; ++i)
    {
      rule_list *p = dset[i - ntokens];
      derives[i - ntokens] = q;
      وقتی (p)
        {
          *q++ = p->مقدار;
          p = p->next;
        }
      *q++ = پوچ;
    }

  اگر (trace_flag & trace_sets)
    print_derives ();

  آزاد (dset);
  آزاد (delts);
}


تهی
derives_free (تهی)
{
  اگر (derives)
    {
      آزاد (derives[0]);
      آزاد (derives);
    }
}
