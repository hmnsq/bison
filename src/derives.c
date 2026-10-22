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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ "system.h"

#અન્તર્ભાવ "getargs.h"

#અન્તર્ભાવ "derives.h"
#અન્તર્ભાવ "gram.h"
#અન્તર્ભાવ "reader.h"
#અન્તર્ભાવ "symtab.h"

/* Linked list of rule numbers.  */
પ્રકારવ્યાખ્યા સંરચના rule_list
{
  સંરચના rule_list *next;
  rule *કિંમત;
} rule_list;

rule ***derives;

સ્થિર શૂન્ય
print_derives (શૂન્ય)
{
  fputs ("DERIVES\n", stderr);

  માટે (પ્રતીક_સંખ્યા i = ntokens; i < nsyms; ++i)
    {
      fprintf (stderr, "  %s derives\n", symbols[i]->tag);
      માટે (rule **rp = derives[i - ntokens]; *rp; ++rp)
        {
          fprintf (stderr, "    %3d ", (*rp)->code);
          rule_rhs_print (*rp, stderr);
          fprintf (stderr, "\n");
        }
    }

  fputs ("\n\n", stderr);
}


શૂન્ય
derives_compute (શૂન્ય)
{
  /* DSET[NTERM - NTOKENS] -- A linked list of the numbers of the rules
     whose LHS is NTERM.  */
  rule_list **dset = xcalloc (nnterms, કદ *dset);

  /* DELTS[RULE] -- There are NRULES rule number to attach to nterms.
     Instead of performing NRULES allocations for each, have an array
     indexed by rule numbers.  */
  rule_list *delts = xnmalloc (nrules, કદ *delts);

  માટે (rule_સંખ્યા r = nrules - 1; r >= 0; --r)
    {
      પ્રતીક_સંખ્યા lhs = rules[r].lhs->સંખ્યા;
      rule_list *p = &delts[r];
      /* A new LHS is found.  */
      p->next = dset[lhs - ntokens];
      p->કિંમત = &rules[r];
      dset[lhs - ntokens] = p;
    }

  /* DSET contains what we need under the form of a linked list.  Make
     it a single array.  */

  derives = xnmalloc (nnterms, કદ *derives);
  /* Q is the storage for DERIVES[...] (DERIVES[0] = q).  */
  rule **q = xnmalloc (nnterms + nrules, કદ *q);

  માટે (પ્રતીક_સંખ્યા i = ntokens; i < nsyms; ++i)
    {
      rule_list *p = dset[i - ntokens];
      derives[i - ntokens] = q;
      જ્યારે (p)
        {
          *q++ = p->કિંમત;
          p = p->next;
        }
      *q++ = ખાલી;
    }

  જો (trace_flag & trace_sets)
    print_derives ();

  મુક્ત (dset);
  મુક્ત (delts);
}


શૂન્ય
derives_free (શૂન્ય)
{
  જો (derives)
    {
      મુક્ત (derives[0]);
      મુક્ત (derives);
    }
}
