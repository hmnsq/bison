/* Output a graph of the generated parser, for Bison.

   Copyright (C) 2001-2007, 2009-2015, 2018-2022 Free Software
   Foundation, Inc.

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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "print-graph.h"

#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ "closure.h"
#ಅನ್ತರ್ಭಾವ "complain.h"
#ಅನ್ತರ್ಭಾವ "conflicts.h"
#ಅನ್ತರ್ಭಾವ "files.h"
#ಅನ್ತರ್ಭಾವ "getargs.h"
#ಅನ್ತರ್ಭಾವ "gram.h"
#ಅನ್ತರ್ಭಾವ "graphviz.h"
#ಅನ್ತರ್ಭಾವ "lalr.h"
#ಅನ್ತರ್ಭಾವ "lr0.h"
#ಅನ್ತರ್ಭಾವ "reader.h"
#ಅನ್ತರ್ಭಾವ "state.h"
#ಅನ್ತರ್ಭಾವ "symtab.h"


/*----------------------------.
| Construct the node labels.  |
`----------------------------*/

/* Print the lhs of a rule in such a manner that there is no vertical
   repetition, like in *.output files. */

ಸ್ಥಿರ ಶೂನ್ಯ
print_core (ರಚನೆ obstack *oout, state *s)
{
  ಅಂಶ_ಸೂಚ್ಯಂಕ ಸ್ಥಿರಾಂಕ *sitems = s->ವಸ್ತುಗಳು;
  sym_content *ಹಿಂದಿನ_lhs = ಖಾಲಿ;
  size_t snritems = s->nitems;

  /* Output all the items of a state, not just its kernel.  */
  ಆದರೆ (report_flag & report_itemsets)
    {
      ಆವರಣ (sitems, snritems);
      sitems = itemset;
      snritems = nitemset;
    }

  obstack_printf (oout, _("State %d"), s->ಸಂಖ್ಯೆ);
  obstack_sgrow (oout, "\\n\\l");
  ಗಾಗಿ (size_t i = 0; i < snritems; ++i)
    {
      ಅಂಶ_ಸಂಖ್ಯೆ ಸ್ಥಿರಾಂಕ *sp1 = ritem + sitems[i];
      ನಿಯಮ ಸ್ಥಿರಾಂಕ *r = ಅಂಶ_ನಿಯಮ (sp1);

      obstack_printf (oout, "%3d ", r->ಸಂಖ್ಯೆ);
      ಆದರೆ (ಹಿಂದಿನ_lhs && UNIQSTR_EQ (ಹಿಂದಿನ_lhs->ಚಿಹ್ನೆ->ಟ್ಯಾಗ್,
                                      r->lhs->ಚಿಹ್ನೆ->ಟ್ಯಾಗ್))
        obstack_printf (oout, "%*s| ",
                        (ಪೂರ್ಣಾಂಕ) ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ಹಿಂದಿನ_lhs->ಚಿಹ್ನೆ->ಟ್ಯಾಗ್), "");
      ಇಲ್ಲದಿದ್ದರೆ
        {
          obstack_backslash (oout, r->lhs->ಚಿಹ್ನೆ->ಟ್ಯಾಗ್);
          obstack_printf (oout, ": ");
        }
      ಹಿಂದಿನ_lhs = r->lhs;

      ಗಾಗಿ (ಅಂಶ_ಸಂಖ್ಯೆ ಸ್ಥಿರಾಂಕ *sp = r->rhs; sp < sp1; sp++)
        {
          obstack_backslash (oout, symbols[*sp]->ಟ್ಯಾಗ್);
          obstack_1grow (oout, ' ');
        }

      obstack_sgrow (oout, "•");

      ಆದರೆ (0 <= *r->rhs)
        ಗಾಗಿ (ಅಂಶ_ಸಂಖ್ಯೆ ಸ್ಥಿರಾಂಕ *sp = sp1; 0 <= *sp; ++sp)
          {
            obstack_1grow (oout, ' ');
            obstack_backslash (oout, symbols[*sp]->ಟ್ಯಾಗ್);
          }
      ಇಲ್ಲದಿದ್ದರೆ
        obstack_sgrow (oout, " %empty");

      /* Experimental feature: display the lookahead tokens. */
      ಆದರೆ (report_flag & report_lookaheads
          && ಅಂಶ_ಸಂಖ್ಯೆ_is_ನಿಯಮ_ಸಂಖ್ಯೆ (*sp1))
        {
          /* Find the reduction we are handling.  */
          reductions *reds = s->reductions;
          ಪೂರ್ಣಾಂಕ redno = state_reduction_ಕಂಡುಹಿಡಿ (s, r);

          /* Print them if there are.  */
          ಆದರೆ (reds->lookaheads && redno != -1)
            {
              bitset_iterator biter;
              ಪೂರ್ಣಾಂಕ k;
              ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *sep = "";
              obstack_sgrow (oout, "  [");
              BITSET_FOR_EACH (biter, reds->lookaheads[redno], k, 0)
                {
                  obstack_sgrow (oout, sep);
                  obstack_backslash (oout, symbols[k]->ಟ್ಯಾಗ್);
                  sep = ", ";
                }
              obstack_1grow (oout, ']');
            }
        }
      obstack_sgrow (oout, "\\l");
    }
}


/*---------------------------------------------------------------.
| Output in graph_obstack edges specifications in incidence with |
| current node.                                                  |
`---------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
print_actions (state ಸ್ಥಿರಾಂಕ *s, FILE *fgraph)
{
  transitions ಸ್ಥಿರಾಂಕ *trans = s->transitions;

  ಆದರೆ (!trans->ಅಂಕೆ && !s->reductions)
    ಹಿಂತಿರುಗಿ;

  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < trans->ಅಂಕೆ; i++)
    ಆದರೆ (!TRANSITION_IS_DISABLED (trans, i))
      {
        ಸ್ಥಿರಾಂಕ state *s1 = trans->ಸ್ಥಿತಿಗಳು[i];
        ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym = s1->accessing_ಚಿಹ್ನೆ;

        /* Shifts are solid, gotos are dashed, and error is dotted.  */
        ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *style =
          (TRANSITION_IS_ERROR (trans, i) ? "dotted"
           : TRANSITION_IS_ಸರಿಸು (trans, i) ? "solid"
           : "dashed");

        ಆದರೆ (TRANSITION_IS_ERROR (trans, i)
            && STRNEQ (symbols[sym]->ಟ್ಯಾಗ್, "error"))
          abort ();
        output_ಅಂಚು (s->ಸಂಖ್ಯೆ, s1->ಸಂಖ್ಯೆ,
                     TRANSITION_IS_ERROR (trans, i) ? ಖಾಲಿ : symbols[sym]->ಟ್ಯಾಗ್,
                     style, fgraph);
      }
  /* Display reductions. */
  output_red (s, s->reductions, fgraph);
}


/*-------------------------------------------------------------.
| Output in FGRAPH the current node specifications and exiting |
| edges.                                                       |
`-------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
print_state (state *s, FILE *fgraph)
{
  ರಚನೆ obstack ಗಂಟು_obstack;

  /* A node's label contains its items.  */
  obstack_init (&ಗಂಟು_obstack);
  print_core (&ಗಂಟು_obstack, s);
  output_ಗಂಟು (s->ಸಂಖ್ಯೆ, obstack_finish0 (&ಗಂಟು_obstack), fgraph);
  obstack_free (&ಗಂಟು_obstack, 0);

  /* Output the edges.  */
  print_actions (s, fgraph);
}


ಶೂನ್ಯ
print_ನಕ್ಷೆ (ಶೂನ್ಯ)
{
  FILE *fgraph = xfopen (spec_ನಕ್ಷೆ_file, "w");
  ಆರಂಭ_ನಕ್ಷೆ (fgraph);

  /* Output nodes and edges. */
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < nstates; i++)
    print_state (ಸ್ಥಿತಿಗಳು[i], fgraph);

  ಮುಕ್ತಾಯ_ನಕ್ಷೆ (fgraph);
  xfclose (fgraph);
}
