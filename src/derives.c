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

#ضمن <config.h>
#ضمن "system.h"

#ضمن "getargs.h"

#ضمن "derives.h"
#ضمن "gram.h"
#ضمن "reader.h"
#ضمن "symtab.h"

/* Linked list of rule numbers.  */
تعريفنوع بنية قاعده_list
{
  بنية قاعده_list *بل;
  قاعده *ارزښت;
} قاعده_list;

قاعده ***derives;

ساكن فراغ
print_derives (فراغ)
{
  fputs ("DERIVES\n", stderr);

  لكل (نښه_شمېره i = ntokens; i < nsyms; ++i)
    {
      fprintf (stderr, "  %s derives\n", symbols[i]->نښان);
      لكل (قاعده **rp = derives[i - ntokens]; *rp; ++rp)
        {
          fprintf (stderr, "    %3d ", (*rp)->کوډ);
          قاعده_rhs_print (*rp, stderr);
          fprintf (stderr, "\n");
        }
    }

  fputs ("\n\n", stderr);
}


فراغ
derives_compute (فراغ)
{
  /* DSET[NTERM - NTOKENS] -- A linked list of the numbers of the rules
     whose LHS is NTERM.  */
  قاعده_list **dset = xcalloc (nnterms, حجم *dset);

  /* DELTS[RULE] -- There are NRULES rule number to attach to nterms.
     Instead of performing NRULES allocations for each, have an array
     indexed by rule numbers.  */
  قاعده_list *delts = xnmalloc (nrules, حجم *delts);

  لكل (قاعده_شمېره r = nrules - 1; r >= 0; --r)
    {
      نښه_شمېره lhs = قواعد[r].lhs->شمېره;
      قاعده_list *p = &delts[r];
      /* A new LHS is found.  */
      p->بل = dset[lhs - ntokens];
      p->ارزښت = &قواعد[r];
      dset[lhs - ntokens] = p;
    }

  /* DSET contains what we need under the form of a linked list.  Make
     it a single array.  */

  derives = xnmalloc (nnterms, حجم *derives);
  /* Q is the storage for DERIVES[...] (DERIVES[0] = q).  */
  قاعده **q = xnmalloc (nnterms + nrules, حجم *q);

  لكل (نښه_شمېره i = ntokens; i < nsyms; ++i)
    {
      قاعده_list *p = dset[i - ntokens];
      derives[i - ntokens] = q;
      طالما (p)
        {
          *q++ = p->ارزښت;
          p = p->بل;
        }
      *q++ = عدم;
    }

  إذا (trace_flag & trace_sets)
    print_derives ();

  حرر (dset);
  حرر (delts);
}


فراغ
derives_free (فراغ)
{
  إذا (derives)
    {
      حرر (derives[0]);
      حرر (derives);
    }
}
