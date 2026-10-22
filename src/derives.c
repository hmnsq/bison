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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ "system.h"

#അന്തര്ഭാവ "getargs.h"

#അന്തര്ഭാവ "derives.h"
#അന്തര്ഭാവ "gram.h"
#അന്തര്ഭാവ "reader.h"
#അന്തര്ഭാവ "symtab.h"

/* Linked list of rule numbers.  */
തരനിർവചനം ഘടന rule_list
{
  ഘടന rule_list *next;
  rule *മൂല്യം;
} rule_list;

rule ***derives;

സ്ഥിതം ശൂന്യം
print_derives (ശൂന്യം)
{
  fputs ("DERIVES\n", stderr);

  വേണ്ടി (ചിഹ്നം_സംഖ്യ i = ntokens; i < nsyms; ++i)
    {
      fprintf (stderr, "  %s derives\n", symbols[i]->tag);
      വേണ്ടി (rule **rp = derives[i - ntokens]; *rp; ++rp)
        {
          fprintf (stderr, "    %3d ", (*rp)->code);
          rule_rhs_print (*rp, stderr);
          fprintf (stderr, "\n");
        }
    }

  fputs ("\n\n", stderr);
}


ശൂന്യം
derives_compute (ശൂന്യം)
{
  /* DSET[NTERM - NTOKENS] -- A linked list of the numbers of the rules
     whose LHS is NTERM.  */
  rule_list **dset = xcalloc (nnterms, വലുപ്പം *dset);

  /* DELTS[RULE] -- There are NRULES rule number to attach to nterms.
     Instead of performing NRULES allocations for each, have an array
     indexed by rule numbers.  */
  rule_list *delts = xnmalloc (nrules, വലുപ്പം *delts);

  വേണ്ടി (rule_സംഖ്യ r = nrules - 1; r >= 0; --r)
    {
      ചിഹ്നം_സംഖ്യ lhs = rules[r].lhs->സംഖ്യ;
      rule_list *p = &delts[r];
      /* A new LHS is found.  */
      p->next = dset[lhs - ntokens];
      p->മൂല്യം = &rules[r];
      dset[lhs - ntokens] = p;
    }

  /* DSET contains what we need under the form of a linked list.  Make
     it a single array.  */

  derives = xnmalloc (nnterms, വലുപ്പം *derives);
  /* Q is the storage for DERIVES[...] (DERIVES[0] = q).  */
  rule **q = xnmalloc (nnterms + nrules, വലുപ്പം *q);

  വേണ്ടി (ചിഹ്നം_സംഖ്യ i = ntokens; i < nsyms; ++i)
    {
      rule_list *p = dset[i - ntokens];
      derives[i - ntokens] = q;
      ആകുമ്പോൾ (p)
        {
          *q++ = p->മൂല്യം;
          p = p->next;
        }
      *q++ = ശൂന്യ;
    }

  എങ്കിൽ (trace_flag & trace_sets)
    print_derives ();

  സ്വതന്ത്രം (dset);
  സ്വതന്ത്രം (delts);
}


ശൂന്യം
derives_free (ശൂന്യം)
{
  എങ്കിൽ (derives)
    {
      സ്വതന്ത്രം (derives[0]);
      സ്വതന്ത്രം (derives);
    }
}
