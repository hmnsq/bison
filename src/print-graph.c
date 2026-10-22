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
print_core (ಸಂರಚನಾ obstack *oout, state *s)
{
  item_index ಧ್ರುವ *sitems = s->ವಸ್ತುಗಳು;
  sym_content *previous_lhs = ರಿಕ್ತ;
  size_t snritems = s->nitems;

  /* Output all the items of a state, not just its kernel.  */
  ಯದಿ (report_flag & report_itemsets)
    {
      closure (sitems, snritems);
      sitems = itemset;
      snritems = nitemset;
    }

  obstack_printf (oout, _("State %d"), s->ಸಂಖ್ಯೆ);
  obstack_sgrow (oout, "\\n\\l");
  ಪ್ರತಿ (size_t i = 0; i < snritems; ++i)
    {
      item_ಸಂಖ್ಯೆ ಧ್ರುವ *sp1 = ritem + sitems[i];
      rule ಧ್ರುವ *r = item_rule (sp1);

      obstack_printf (oout, "%3d ", r->ಸಂಖ್ಯೆ);
      ಯದಿ (previous_lhs && UNIQSTR_EQ (previous_lhs->ಚಿಹ್ನೆ->tag,
                                      r->lhs->ಚಿಹ್ನೆ->tag))
        obstack_printf (oout, "%*s| ",
                        (ಪೂರ್ಣಾಙ್ಕ) strlen (previous_lhs->ಚಿಹ್ನೆ->tag), "");
      ಅನ್ಯಥಾ
        {
          obstack_backslash (oout, r->lhs->ಚಿಹ್ನೆ->tag);
          obstack_printf (oout, ": ");
        }
      previous_lhs = r->lhs;

      ಪ್ರತಿ (item_ಸಂಖ್ಯೆ ಧ್ರುವ *sp = r->rhs; sp < sp1; sp++)
        {
          obstack_backslash (oout, symbols[*sp]->tag);
          obstack_1grow (oout, ' ');
        }

      obstack_sgrow (oout, "•");

      ಯದಿ (0 <= *r->rhs)
        ಪ್ರತಿ (item_ಸಂಖ್ಯೆ ಧ್ರುವ *sp = sp1; 0 <= *sp; ++sp)
          {
            obstack_1grow (oout, ' ');
            obstack_backslash (oout, symbols[*sp]->tag);
          }
      ಅನ್ಯಥಾ
        obstack_sgrow (oout, " %empty");

      /* Experimental feature: display the lookahead tokens. */
      ಯದಿ (report_flag & report_lookaheads
          && item_ಸಂಖ್ಯೆ_is_rule_ಸಂಖ್ಯೆ (*sp1))
        {
          /* Find the reduction we are handling.  */
          reductions *reds = s->reductions;
          ಪೂರ್ಣಾಙ್ಕ redno = state_reduction_find (s, r);

          /* Print them if there are.  */
          ಯದಿ (reds->lookaheads && redno != -1)
            {
              bitset_iterator biter;
              ಪೂರ್ಣಾಙ್ಕ k;
              ವರ್ಣ ಧ್ರುವ *sep = "";
              obstack_sgrow (oout, "  [");
              BITSET_FOR_EACH (biter, reds->lookaheads[redno], k, 0)
                {
                  obstack_sgrow (oout, sep);
                  obstack_backslash (oout, symbols[k]->tag);
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
print_actions (state ಧ್ರುವ *s, FILE *fgraph)
{
  transitions ಧ್ರುವ *trans = s->transitions;

  ಯದಿ (!trans->ಅಂಕೆ && !s->reductions)
    ಪ್ರತ್ಯಾಗಮ;

  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < trans->ಅಂಕೆ; i++)
    ಯದಿ (!TRANSITION_IS_DISABLED (trans, i))
      {
        ಧ್ರುವ state *s1 = trans->states[i];
        ಧ್ರುವ ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym = s1->accessing_ಚಿಹ್ನೆ;

        /* Shifts are solid, gotos are dashed, and error is dotted.  */
        ವರ್ಣ ಧ್ರುವ *style =
          (TRANSITION_IS_ERROR (trans, i) ? "dotted"
           : TRANSITION_IS_SHIFT (trans, i) ? "solid"
           : "dashed");

        ಯದಿ (TRANSITION_IS_ERROR (trans, i)
            && STRNEQ (symbols[sym]->tag, "error"))
          abort ();
        output_edge (s->ಸಂಖ್ಯೆ, s1->ಸಂಖ್ಯೆ,
                     TRANSITION_IS_ERROR (trans, i) ? ರಿಕ್ತ : symbols[sym]->tag,
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
  ಸಂರಚನಾ obstack ಗಂಟು_obstack;

  /* A node's label contains its items.  */
  obstack_init (&ಗಂಟು_obstack);
  print_core (&ಗಂಟು_obstack, s);
  output_ಗಂಟು (s->ಸಂಖ್ಯೆ, obstack_finish0 (&ಗಂಟು_obstack), fgraph);
  obstack_free (&ಗಂಟು_obstack, 0);

  /* Output the edges.  */
  print_actions (s, fgraph);
}


ಶೂನ್ಯ
print_graph (ಶೂನ್ಯ)
{
  FILE *fgraph = xfopen (spec_graph_file, "w");
  start_graph (fgraph);

  /* Output nodes and edges. */
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < nstates; i++)
    print_state (states[i], fgraph);

  finish_graph (fgraph);
  xfclose (fgraph);
}
