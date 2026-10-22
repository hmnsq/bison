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

#include <config.h>
#include "system.h"

#include "getargs.h"

#include "derives.h"
#include "gram.h"
#include "reader.h"
#include "symtab.h"

/* Linked list of rule numbers.  */
typedef struct rule_list
{
  struct rule_list *next;
  rule *हालागा;
} rule_list;

rule ***derives;

static void
इलिम्बाग_derives (void)
{
  fputs ("DERIVES\n", stderr);

  for (सिम्बोलो_नुमेरो i = ntokens; i < nsyms; ++i)
    {
      fprintf (stderr, "  %s derives\n", symbols[i]->tag);
      for (rule **rp = derives[i - ntokens]; *rp; ++rp)
        {
          fprintf (stderr, "    %3d ", (*rp)->code);
          rule_rhs_इलिम्बाग (*rp, stderr);
          fprintf (stderr, "\n");
        }
    }

  fputs ("\n\n", stderr);
}


void
derives_compute (void)
{
  /* DSET[NTERM - NTOKENS] -- A linked list of the numbers of the rules
     whose LHS is NTERM.  */
  rule_list **dset = xcalloc (nnterms, sizeof *dset);

  /* DELTS[RULE] -- There are NRULES rule number to attach to nterms.
     Instead of performing NRULES allocations for each, have an array
     indexed by rule numbers.  */
  rule_list *delts = xnmalloc (nrules, sizeof *delts);

  for (rule_नुमेरो r = nrules - 1; r >= 0; --r)
    {
      सिम्बोलो_नुमेरो lhs = rules[r].lhs->नुमेरो_2;
      rule_list *p = &delts[r];
      /* A new LHS is found.  */
      p->next = dset[lhs - ntokens];
      p->हालागा = &rules[r];
      dset[lhs - ntokens] = p;
    }

  /* DSET contains what we need under the form of a linked list.  Make
     it a single array.  */

  derives = xnmalloc (nnterms, sizeof *derives);
  /* Q is the storage for DERIVES[...] (DERIVES[0] = q).  */
  rule **q = xnmalloc (nnterms + nrules, sizeof *q);

  for (सिम्बोलो_नुमेरो i = ntokens; i < nsyms; ++i)
    {
      rule_list *p = dset[i - ntokens];
      derives[i - ntokens] = q;
      while (p)
        {
          *q++ = p->हालागा;
          p = p->next;
        }
      *q++ = NULL;
    }

  if (trace_flag & trace_sets)
    इलिम्बाग_derives ();

  free (dset);
  free (delts);
}


void
derives_free (void)
{
  if (derives)
    {
      free (derives[0]);
      free (derives);
    }
}
