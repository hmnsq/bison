/* Calculate which nonterminals can expand into the null string for Bison.

   Copyright (C) 1984, 1989, 2000-2006, 2009-2015, 2018-2022 Free
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


/* Set up NULLABLE, a vector saying which nonterminals can expand into
   the null string.  NULLABLE[I - NTOKENS] is nonzero if symbol I can
   do so.  */

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ "getargs.h"
#ಅನ್ತರ್ಭಾವ "gram.h"
#ಅನ್ತರ್ಭಾವ "nullable.h"
#ಅನ್ತರ್ಭಾವ "reduce.h"
#ಅನ್ತರ್ಭಾವ "symtab.h"

/* Linked list of rules.  */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ ನಿಯಮ_list
{
  ರಚನೆ ನಿಯಮ_list *ಮುಂದಿನ;
  ಸ್ಥಿರಾಂಕ ನಿಯಮ *ಮೌಲ್ಯ;
} ನಿಯಮ_list;

bool *nullable = ಖಾಲಿ;

ಸ್ಥಿರ ಶೂನ್ಯ
nullable_print (FILE *ಹೊರಹೋಗು)
{
  fputs ("NULLABLE\n", ಹೊರಹೋಗು);
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = ntokens; i < nsyms; i++)
    fprintf (ಹೊರಹೋಗು, "  %s: %s\n", symbols[i]->ಟ್ಯಾಗ್,
             nullable[i - ntokens] ? "yes" : "no");
  fputs ("\n\n", ಹೊರಹೋಗು);
}

ಶೂನ್ಯ
nullable_compute (ಶೂನ್ಯ)
{
  nullable = xcalloc (nnterms, ಗಾತ್ರ *nullable);

  size_t *rcount = xcalloc (nrules, ಗಾತ್ರ *rcount);
  /* RITEM contains all the rules, including useless productions.
     Hence we must allocate room for useless nonterminals too.  */
  ನಿಯಮ_list **rsets = xcalloc (nnterms, ಗಾತ್ರ *rsets);
  /* This is said to be more elements than we actually use.
     Supposedly NRITEMS - NRULES is enough.  But why take the risk?  */
  ನಿಯಮ_list *relts = xnmalloc (nritems + nnterms + 1, ಗಾತ್ರ *relts);

  ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ *squeue = xnmalloc (nnterms, ಗಾತ್ರ *squeue);
  ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ *s2 = squeue;
  {
    ನಿಯಮ_list *p = relts;
    ಗಾಗಿ (ನಿಯಮ_ಸಂಖ್ಯೆ ruleno = 0; ruleno < nrules; ++ruleno)
      ಆದರೆ (ನಿಯಮಗಳು[ruleno].useful)
        {
          ಸ್ಥಿರಾಂಕ ನಿಯಮ *r = &ನಿಯಮಗಳು[ruleno];
          ಆದರೆ (r->rhs[0] >= 0)
            {
              /* This rule has a non empty RHS. */
              bool any_ಟೋಕನುಗಳು = false;
              ಗಾಗಿ (ಅಂಶ_ಸಂಖ್ಯೆ *rp = r->rhs; *rp >= 0; ++rp)
                ಆದರೆ (ISTOKEN (*rp))
                  any_ಟೋಕನುಗಳು = true;

              /* This rule has only nonterminals: schedule it for the second
                 pass.  */
              ಆದರೆ (!any_ಟೋಕನುಗಳು)
                ಗಾಗಿ (ಅಂಶ_ಸಂಖ್ಯೆ *rp = r->rhs; *rp >= 0; ++rp)
                  {
                    rcount[ruleno]++;
                    p->ಮುಂದಿನ = rsets[*rp - ntokens];
                    p->ಮೌಲ್ಯ = r;
                    rsets[*rp - ntokens] = p;
                    p++;
                  }
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              /* This rule has an empty RHS. */
              ಆದರೆ (r->useful
                  && ! nullable[r->lhs->ಸಂಖ್ಯೆ - ntokens])
                {
                  nullable[r->lhs->ಸಂಖ್ಯೆ - ntokens] = true;
                  *s2++ = r->lhs->ಸಂಖ್ಯೆ;
                }
            }
        }
  }

  ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ *s1 = squeue;
  ಆಗ (s1 < s2)
    ಗಾಗಿ (ನಿಯಮ_list *p = rsets[*s1++ - ntokens]; p; p = p->ಮುಂದಿನ)
      {
        ಸ್ಥಿರಾಂಕ ನಿಯಮ *r = p->ಮೌಲ್ಯ;
        ಆದರೆ (--rcount[r->ಸಂಖ್ಯೆ] == 0)
          ಆದರೆ (r->useful && ! nullable[r->lhs->ಸಂಖ್ಯೆ - ntokens])
            {
              nullable[r->lhs->ಸಂಖ್ಯೆ - ntokens] = true;
              *s2++ = r->lhs->ಸಂಖ್ಯೆ;
            }
      }

  ಬಿಡುಗಡೆ (squeue);
  ಬಿಡುಗಡೆ (rcount);
  ಬಿಡುಗಡೆ (rsets);
  ಬಿಡುಗಡೆ (relts);

  ಆದರೆ (trace_flag & trace_sets)
    nullable_print (stderr);
}


ಶೂನ್ಯ
nullable_free (ಶೂನ್ಯ)
{
  ಬಿಡುಗಡೆ (nullable);
}
