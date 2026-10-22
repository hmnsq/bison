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
print_core (بنية obstack *oout, state *s)
{
  item_index ثابت *sitems = s->اشیاء;
  sym_content *previous_lhs = عدم;
  size_t snritems = s->nitems;

  /* Output all the items of a state, not just its kernel.  */
  إذا (report_flag & report_itemsets)
    {
      closure (sitems, snritems);
      sitems = itemset;
      snritems = nitemset;
    }

  obstack_printf (oout, _("State %d"), s->نمبر);
  obstack_sgrow (oout, "\\n\\l");
  لكل (size_t i = 0; i < snritems; ++i)
    {
      item_نمبر ثابت *sp1 = ritem + sitems[i];
      rule ثابت *r = item_rule (sp1);

      obstack_printf (oout, "%3d ", r->نمبر);
      إذا (previous_lhs && UNIQSTR_EQ (previous_lhs->علامت->tag,
                                      r->lhs->علامت->tag))
        obstack_printf (oout, "%*s| ",
                        (صحيح) strlen (previous_lhs->علامت->tag), "");
      وإلا
        {
          obstack_backslash (oout, r->lhs->علامت->tag);
          obstack_printf (oout, ": ");
        }
      previous_lhs = r->lhs;

      لكل (item_نمبر ثابت *sp = r->rhs; sp < sp1; sp++)
        {
          obstack_backslash (oout, symbols[*sp]->tag);
          obstack_1grow (oout, ' ');
        }

      obstack_sgrow (oout, "•");

      إذا (0 <= *r->rhs)
        لكل (item_نمبر ثابت *sp = sp1; 0 <= *sp; ++sp)
          {
            obstack_1grow (oout, ' ');
            obstack_backslash (oout, symbols[*sp]->tag);
          }
      وإلا
        obstack_sgrow (oout, " %empty");

      /* Experimental feature: display the lookahead tokens. */
      إذا (report_flag & report_lookaheads
          && item_نمبر_is_rule_نمبر (*sp1))
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
print_actions (state ثابت *s, FILE *fgraph)
{
  transitions ثابت *trans = s->transitions;

  إذا (!trans->عدد && !s->reductions)
    أرجع;

  لكل (صحيح i = 0; i < trans->عدد; i++)
    إذا (!TRANSITION_IS_DISABLED (trans, i))
      {
        ثابت state *s1 = trans->states[i];
        ثابت علامت_نمبر sym = s1->accessing_علامت;

        /* Shifts are solid, gotos are dashed, and error is dotted.  */
        حرف ثابت *style =
          (TRANSITION_IS_ERROR (trans, i) ? "dotted"
           : TRANSITION_IS_SHIFT (trans, i) ? "solid"
           : "dashed");

        إذا (TRANSITION_IS_ERROR (trans, i)
            && STRNEQ (symbols[sym]->tag, "error"))
          abort ();
        output_edge (s->نمبر, s1->نمبر,
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
print_state (state *s, FILE *fgraph)
{
  بنية obstack گرہ_obstack;

  /* A node's label contains its items.  */
  obstack_init (&گرہ_obstack);
  print_core (&گرہ_obstack, s);
  output_گرہ (s->نمبر, obstack_finish0 (&گرہ_obstack), fgraph);
  obstack_free (&گرہ_obstack, 0);

  /* Output the edges.  */
  print_actions (s, fgraph);
}


فراغ
print_graph (فراغ)
{
  FILE *fgraph = xfopen (spec_graph_file, "w");
  start_graph (fgraph);

  /* Output nodes and edges. */
  لكل (صحيح i = 0; i < nstates; i++)
    print_state (states[i], fgraph);

  finish_graph (fgraph);
  xfclose (fgraph);
}
