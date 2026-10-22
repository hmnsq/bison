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
  item_index अचर *sitems = s->वस्तुहरू;
  sym_content *previous_lhs = रिक्त;
  माप_प्रकार snritems = s->nitems;

  /* Output all the items of a state, not just its kernel.  */
  यदि (report_flag & report_itemsets)
    {
      closure (sitems, snritems);
      sitems = itemset;
      snritems = nitemset;
    }

  obstack_printf (oout, _("State %d"), s->सङ्ख्या);
  obstack_sgrow (oout, "\\n\\l");
  लागि (माप_प्रकार i = 0; i < snritems; ++i)
    {
      item_सङ्ख्या अचर *sp1 = ritem + sitems[i];
      rule अचर *r = item_rule (sp1);

      obstack_printf (oout, "%3d ", r->सङ्ख्या);
      यदि (previous_lhs && UNIQSTR_EQ (previous_lhs->चिह्न->tag,
                                      r->lhs->चिह्न->tag))
        obstack_printf (oout, "%*s| ",
                        (पूर्णाङ्क) म_माप (previous_lhs->चिह्न->tag), "");
      अन्यथा
        {
          obstack_backslash (oout, r->lhs->चिह्न->tag);
          obstack_printf (oout, ": ");
        }
      previous_lhs = r->lhs;

      लागि (item_सङ्ख्या अचर *sp = r->rhs; sp < sp1; sp++)
        {
          obstack_backslash (oout, symbols[*sp]->tag);
          obstack_1grow (oout, ' ');
        }

      obstack_sgrow (oout, "•");

      यदि (0 <= *r->rhs)
        लागि (item_सङ्ख्या अचर *sp = sp1; 0 <= *sp; ++sp)
          {
            obstack_1grow (oout, ' ');
            obstack_backslash (oout, symbols[*sp]->tag);
          }
      अन्यथा
        obstack_sgrow (oout, " %empty");

      /* Experimental feature: display the lookahead tokens. */
      यदि (report_flag & report_lookaheads
          && item_सङ्ख्या_is_rule_सङ्ख्या (*sp1))
        {
          /* Find the reduction we are handling.  */
          reductions *reds = s->reductions;
          पूर्णाङ्क redno = state_reduction_find (s, r);

          /* Print them if there are.  */
          यदि (reds->lookaheads && redno != -1)
            {
              bitset_iterator biter;
              पूर्णाङ्क k;
              वर्ण अचर *sep = "";
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
print_actions (state अचर *s, खाता *fgraph)
{
  transitions अचर *trans = s->transitions;

  यदि (!trans->अङ्क && !s->reductions)
    फर्काऊ;

  लागि (पूर्णाङ्क i = 0; i < trans->अङ्क; i++)
    यदि (!TRANSITION_IS_DISABLED (trans, i))
      {
        अचर state *s1 = trans->states[i];
        अचर चिह्न_सङ्ख्या sym = s1->accessing_चिह्न;

        /* Shifts are solid, gotos are dashed, and error is dotted.  */
        वर्ण अचर *style =
          (TRANSITION_IS_ERROR (trans, i) ? "dotted"
           : TRANSITION_IS_SHIFT (trans, i) ? "solid"
           : "dashed");

        यदि (TRANSITION_IS_ERROR (trans, i)
            && STRNEQ (symbols[sym]->tag, "error"))
          पात ();
        output_edge (s->सङ्ख्या, s1->सङ्ख्या,
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
  संरचना obstack गाँठो_obstack;

  /* A node's label contains its items.  */
  obstack_init (&गाँठो_obstack);
  print_core (&गाँठो_obstack, s);
  output_गाँठो (s->सङ्ख्या, obstack_finish0 (&गाँठो_obstack), fgraph);
  obstack_free (&गाँठो_obstack, 0);

  /* Output the edges.  */
  print_actions (s, fgraph);
}


शून्य
print_graph (शून्य)
{
  खाता *fgraph = xfopen (spec_graph_file, "w");
  start_graph (fgraph);

  /* Output nodes and edges. */
  लागि (पूर्णाङ्क i = 0; i < nstates; i++)
    print_state (states[i], fgraph);

  finish_graph (fgraph);
  xfclose (fgraph);
}
