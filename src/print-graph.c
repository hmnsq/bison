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

#अन्तर्भाव <config.h>
#अन्तर्भाव "print-graph.h"

#अन्तर्भाव "system.h"

#अन्तर्भाव "closure.h"
#अन्तर्भाव "complain.h"
#अन्तर्भाव "conflicts.h"
#अन्तर्भाव "files.h"
#अन्तर्भाव "getargs.h"
#अन्तर्भाव "gram.h"
#अन्तर्भाव "graphviz.h"
#अन्तर्भाव "lalr.h"
#अन्तर्भाव "lr0.h"
#अन्तर्भाव "reader.h"
#अन्तर्भाव "state.h"
#अन्तर्भाव "symtab.h"


/*----------------------------.
| Construct the node labels.  |
`----------------------------*/

/* Print the lhs of a rule in such a manner that there is no vertical
   repetition, like in *.output files. */

स्थिर शून्य
print_core (संरचना obstack *oout, state *s)
{
  item_index स्थिरांक *sitems = s->वस्तुसंच;
  sym_content *previous_lhs = रिक्त;
  माप_प्रकार snritems = s->nitems;

  /* Output all the items of a state, not just its kernel.  */
  जर (report_flag & report_itemsets)
    {
      closure (sitems, snritems);
      sitems = itemset;
      snritems = nitemset;
    }

  obstack_printf (oout, _("State %d"), s->संख्या);
  obstack_sgrow (oout, "\\n\\l");
  साठी (माप_प्रकार i = 0; i < snritems; ++i)
    {
      item_संख्या स्थिरांक *sp1 = ritem + sitems[i];
      rule स्थिरांक *r = item_rule (sp1);

      obstack_printf (oout, "%3d ", r->संख्या);
      जर (previous_lhs && UNIQSTR_EQ (previous_lhs->चिन्ह->tag,
                                      r->lhs->चिन्ह->tag))
        obstack_printf (oout, "%*s| ",
                        (पूर्णांक) म_माप (previous_lhs->चिन्ह->tag), "");
      अन्यथा
        {
          obstack_backslash (oout, r->lhs->चिन्ह->tag);
          obstack_printf (oout, ": ");
        }
      previous_lhs = r->lhs;

      साठी (item_संख्या स्थिरांक *sp = r->rhs; sp < sp1; sp++)
        {
          obstack_backslash (oout, symbols[*sp]->tag);
          obstack_1grow (oout, ' ');
        }

      obstack_sgrow (oout, "•");

      जर (0 <= *r->rhs)
        साठी (item_संख्या स्थिरांक *sp = sp1; 0 <= *sp; ++sp)
          {
            obstack_1grow (oout, ' ');
            obstack_backslash (oout, symbols[*sp]->tag);
          }
      अन्यथा
        obstack_sgrow (oout, " %empty");

      /* Experimental feature: display the lookahead tokens. */
      जर (report_flag & report_lookaheads
          && item_संख्या_is_rule_संख्या (*sp1))
        {
          /* Find the reduction we are handling.  */
          reductions *reds = s->reductions;
          पूर्णांक redno = state_reduction_find (s, r);

          /* Print them if there are.  */
          जर (reds->lookaheads && redno != -1)
            {
              bitset_iterator biter;
              पूर्णांक k;
              अक्षर स्थिरांक *sep = "";
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

स्थिर शून्य
print_actions (state स्थिरांक *s, खाता *fgraph)
{
  transitions स्थिरांक *trans = s->transitions;

  जर (!trans->अंक && !s->reductions)
    परत;

  साठी (पूर्णांक i = 0; i < trans->अंक; i++)
    जर (!TRANSITION_IS_DISABLED (trans, i))
      {
        स्थिरांक state *s1 = trans->states[i];
        स्थिरांक चिन्ह_संख्या sym = s1->accessing_चिन्ह;

        /* Shifts are solid, gotos are dashed, and error is dotted.  */
        अक्षर स्थिरांक *style =
          (TRANSITION_IS_ERROR (trans, i) ? "dotted"
           : TRANSITION_IS_SHIFT (trans, i) ? "solid"
           : "dashed");

        जर (TRANSITION_IS_ERROR (trans, i)
            && STRNEQ (symbols[sym]->tag, "error"))
          पात ();
        output_edge (s->संख्या, s1->संख्या,
                     TRANSITION_IS_ERROR (trans, i) ? रिक्त : symbols[sym]->tag,
                     style, fgraph);
      }
  /* Display reductions. */
  output_red (s, s->reductions, fgraph);
}


/*-------------------------------------------------------------.
| Output in FGRAPH the current node specifications and exiting |
| edges.                                                       |
`-------------------------------------------------------------*/

स्थिर शून्य
print_state (state *s, खाता *fgraph)
{
  संरचना obstack गाठ_obstack;

  /* A node's label contains its items.  */
  obstack_init (&गाठ_obstack);
  print_core (&गाठ_obstack, s);
  output_गाठ (s->संख्या, obstack_finish0 (&गाठ_obstack), fgraph);
  obstack_free (&गाठ_obstack, 0);

  /* Output the edges.  */
  print_actions (s, fgraph);
}


शून्य
print_graph (शून्य)
{
  खाता *fgraph = xfopen (spec_graph_file, "w");
  start_graph (fgraph);

  /* Output nodes and edges. */
  साठी (पूर्णांक i = 0; i < nstates; i++)
    print_state (states[i], fgraph);

  finish_graph (fgraph);
  xfclose (fgraph);
}
