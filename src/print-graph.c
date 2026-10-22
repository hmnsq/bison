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

#ضمن <config.h>
#ضمن "print-graph.h"

#ضمن "system.h"

#ضمن "closure.h"
#ضمن "complain.h"
#ضمن "conflicts.h"
#ضمن "files.h"
#ضمن "getargs.h"
#ضمن "gram.h"
#ضمن "graphviz.h"
#ضمن "lalr.h"
#ضمن "lr0.h"
#ضمن "reader.h"
#ضمن "state.h"
#ضمن "symtab.h"


/*----------------------------.
| Construct the node labels.  |
`----------------------------*/

/* Print the lhs of a rule in such a manner that there is no vertical
   repetition, like in *.output files. */

ساكن فراغ
بغ_core (بنية obstack *oout, state *s)
{
  item_index ثابت *sitems = s->ابوبو;
  sym_content *previous_lhs = عدم;
  size_t snritems = s->nitems;

  /* Output all the items of a state, not just its kernel.  */
  إذا (report_flag & report_itemsets)
    {
      closure (sitems, snritems);
      sitems = itemset;
      snritems = nitemset;
    }

  obstack_printf (oout, _("State %d"), s->لمب_2);
  obstack_sgrow (oout, "\\n\\l");
  لكل (size_t i = 0; i < snritems; ++i)
    {
      item_لمب ثابت *sp1 = ritem + sitems[i];
      rule ثابت *r = item_rule (sp1);

      obstack_printf (oout, "%3d ", r->لمب_2);
      إذا (previous_lhs && UNIQSTR_EQ (previous_lhs->علام->tag,
                                      r->lhs->علام->tag))
        obstack_printf (oout, "%*s| ",
                        (صحيح) strlen (previous_lhs->علام->tag), "");
      وإلا
        {
          obstack_backslash (oout, r->lhs->علام->tag);
          obstack_printf (oout, ": ");
        }
      previous_lhs = r->lhs;

      لكل (item_لمب ثابت *sp = r->rhs; sp < sp1; sp++)
        {
          obstack_backslash (oout, symbols[*sp]->tag);
          obstack_1grow (oout, ' ');
        }

      obstack_sgrow (oout, "•");

      إذا (0 <= *r->rhs)
        لكل (item_لمب ثابت *sp = sp1; 0 <= *sp; ++sp)
          {
            obstack_1grow (oout, ' ');
            obstack_backslash (oout, symbols[*sp]->tag);
          }
      وإلا
        obstack_sgrow (oout, " %empty");

      /* Experimental feature: display the lookahead tokens. */
      إذا (report_flag & report_lookaheads
          && item_لمب_is_rule_لمب (*sp1))
        {
          /* Find the reduction we are handling.  */
          reductions *reds = s->reductions;
          صحيح redno = state_reduction_find (s, r);

          /* Print them if there are.  */
          إذا (reds->lookaheads && redno != -1)
            {
              bitset_iterator biter;
              صحيح k;
              حرف ثابت *sep = "";
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

ساكن فراغ
بغ_actions (state ثابت *s, FILE *fgraph)
{
  transitions ثابت *trans = s->transitions;

  إذا (!trans->لمب && !s->reductions)
    أرجع;

  لكل (صحيح i = 0; i < trans->لمب; i++)
    إذا (!TRANSITION_IS_DISABLED (trans, i))
      {
        ثابت state *s1 = trans->states[i];
        ثابت علام_لمب sym = s1->accessing_علام;

        /* Shifts are solid, gotos are dashed, and error is dotted.  */
        حرف ثابت *style =
          (TRANSITION_IS_ERROR (trans, i) ? "dotted"
           : TRANSITION_IS_SHIFT (trans, i) ? "solid"
           : "dashed");

        إذا (TRANSITION_IS_ERROR (trans, i)
            && STRNEQ (symbols[sym]->tag, "error"))
          abort ();
        output_edge (s->لمب_2, s1->لمب_2,
                     TRANSITION_IS_ERROR (trans, i) ? عدم : symbols[sym]->tag,
                     style, fgraph);
      }
  /* Display reductions. */
  output_red (s, s->reductions, fgraph);
}


/*-------------------------------------------------------------.
| Output in FGRAPH the current node specifications and exiting |
| edges.                                                       |
`-------------------------------------------------------------*/

ساكن فراغ
بغ_state (state *s, FILE *fgraph)
{
  بنية obstack كلي_obstack;

  /* A node's label contains its items.  */
  obstack_init (&كلي_obstack);
  بغ_core (&كلي_obstack, s);
  output_كلي (s->لمب_2, obstack_finish0 (&كلي_obstack), fgraph);
  obstack_free (&كلي_obstack, 0);

  /* Output the edges.  */
  بغ_actions (s, fgraph);
}


فراغ
بغ_graph (فراغ)
{
  FILE *fgraph = xfopen (spec_graph_file, "w");
  start_graph (fgraph);

  /* Output nodes and edges. */
  لكل (صحيح i = 0; i < nstates; i++)
    بغ_state (states[i], fgraph);

  finish_graph (fgraph);
  xfclose (fgraph);
}
