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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ "print-graph.h"

#അന്തര്ഭാവ "system.h"

#അന്തര്ഭാവ "closure.h"
#അന്തര്ഭാവ "complain.h"
#അന്തര്ഭാവ "conflicts.h"
#അന്തര്ഭാവ "files.h"
#അന്തര്ഭാവ "getargs.h"
#അന്തര്ഭാവ "gram.h"
#അന്തര്ഭാവ "graphviz.h"
#അന്തര്ഭാവ "lalr.h"
#അന്തര്ഭാവ "lr0.h"
#അന്തര്ഭാവ "reader.h"
#അന്തര്ഭാവ "state.h"
#അന്തര്ഭാവ "symtab.h"


/*----------------------------.
| Construct the node labels.  |
`----------------------------*/

/* Print the lhs of a rule in such a manner that there is no vertical
   repetition, like in *.output files. */

സ്ഥിതം ശൂന്യം
print_core (ഘടന obstack *oout, state *s)
{
  item_index സ്ഥിരം *sitems = s->ഇനങ്ങൾ;
  sym_content *previous_lhs = ശൂന്യ;
  size_t snritems = s->nitems;

  /* Output all the items of a state, not just its kernel.  */
  എങ്കിൽ (report_flag & report_itemsets)
    {
      closure (sitems, snritems);
      sitems = itemset;
      snritems = nitemset;
    }

  obstack_printf (oout, _("State %d"), s->സംഖ്യ);
  obstack_sgrow (oout, "\\n\\l");
  വേണ്ടി (size_t i = 0; i < snritems; ++i)
    {
      item_സംഖ്യ സ്ഥിരം *sp1 = ritem + sitems[i];
      rule സ്ഥിരം *r = item_rule (sp1);

      obstack_printf (oout, "%3d ", r->സംഖ്യ);
      എങ്കിൽ (previous_lhs && UNIQSTR_EQ (previous_lhs->ചിഹ്നം->tag,
                                      r->lhs->ചിഹ്നം->tag))
        obstack_printf (oout, "%*s| ",
                        (പൂർണ്ണസംഖ്യ) strlen (previous_lhs->ചിഹ്നം->tag), "");
      അല്ലെങ്കിൽ
        {
          obstack_backslash (oout, r->lhs->ചിഹ്നം->tag);
          obstack_printf (oout, ": ");
        }
      previous_lhs = r->lhs;

      വേണ്ടി (item_സംഖ്യ സ്ഥിരം *sp = r->rhs; sp < sp1; sp++)
        {
          obstack_backslash (oout, symbols[*sp]->tag);
          obstack_1grow (oout, ' ');
        }

      obstack_sgrow (oout, "•");

      എങ്കിൽ (0 <= *r->rhs)
        വേണ്ടി (item_സംഖ്യ സ്ഥിരം *sp = sp1; 0 <= *sp; ++sp)
          {
            obstack_1grow (oout, ' ');
            obstack_backslash (oout, symbols[*sp]->tag);
          }
      അല്ലെങ്കിൽ
        obstack_sgrow (oout, " %empty");

      /* Experimental feature: display the lookahead tokens. */
      എങ്കിൽ (report_flag & report_lookaheads
          && item_സംഖ്യ_is_rule_സംഖ്യ (*sp1))
        {
          /* Find the reduction we are handling.  */
          reductions *reds = s->reductions;
          പൂർണ്ണസംഖ്യ redno = state_reduction_find (s, r);

          /* Print them if there are.  */
          എങ്കിൽ (reds->lookaheads && redno != -1)
            {
              bitset_iterator biter;
              പൂർണ്ണസംഖ്യ k;
              അക്ഷരം സ്ഥിരം *sep = "";
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

സ്ഥിതം ശൂന്യം
print_actions (state സ്ഥിരം *s, FILE *fgraph)
{
  transitions സ്ഥിരം *trans = s->transitions;

  എങ്കിൽ (!trans->അക്കം && !s->reductions)
    തിരികെ;

  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < trans->അക്കം; i++)
    എങ്കിൽ (!TRANSITION_IS_DISABLED (trans, i))
      {
        സ്ഥിരം state *s1 = trans->states[i];
        സ്ഥിരം ചിഹ്നം_സംഖ്യ sym = s1->accessing_ചിഹ്നം;

        /* Shifts are solid, gotos are dashed, and error is dotted.  */
        അക്ഷരം സ്ഥിരം *style =
          (TRANSITION_IS_ERROR (trans, i) ? "dotted"
           : TRANSITION_IS_SHIFT (trans, i) ? "solid"
           : "dashed");

        എങ്കിൽ (TRANSITION_IS_ERROR (trans, i)
            && STRNEQ (symbols[sym]->tag, "error"))
          abort ();
        output_edge (s->സംഖ്യ, s1->സംഖ്യ,
                     TRANSITION_IS_ERROR (trans, i) ? ശൂന്യ : symbols[sym]->tag,
                     style, fgraph);
      }
  /* Display reductions. */
  output_red (s, s->reductions, fgraph);
}


/*-------------------------------------------------------------.
| Output in FGRAPH the current node specifications and exiting |
| edges.                                                       |
`-------------------------------------------------------------*/

സ്ഥിതം ശൂന്യം
print_state (state *s, FILE *fgraph)
{
  ഘടന obstack കണ്ണി_obstack;

  /* A node's label contains its items.  */
  obstack_init (&കണ്ണി_obstack);
  print_core (&കണ്ണി_obstack, s);
  output_കണ്ണി (s->സംഖ്യ, obstack_finish0 (&കണ്ണി_obstack), fgraph);
  obstack_free (&കണ്ണി_obstack, 0);

  /* Output the edges.  */
  print_actions (s, fgraph);
}


ശൂന്യം
print_graph (ശൂന്യം)
{
  FILE *fgraph = xfopen (spec_graph_file, "w");
  start_graph (fgraph);

  /* Output nodes and edges. */
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < nstates; i++)
    print_state (states[i], fgraph);

  finish_graph (fgraph);
  xfclose (fgraph);
}
