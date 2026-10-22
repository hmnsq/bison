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

#অন্তর্ভাৱ <config.h>
#অন্তর্ভাৱ "print-graph.h"

#অন্তর্ভাৱ "system.h"

#অন্তর্ভাৱ "closure.h"
#অন্তর্ভাৱ "complain.h"
#অন্তর্ভাৱ "conflicts.h"
#অন্তর্ভাৱ "files.h"
#অন্তর্ভাৱ "getargs.h"
#অন্তর্ভাৱ "gram.h"
#অন্তর্ভাৱ "graphviz.h"
#অন্তর্ভাৱ "lalr.h"
#অন্তর্ভাৱ "lr0.h"
#অন্তর্ভাৱ "reader.h"
#অন্তর্ভাৱ "state.h"
#অন্তর্ভাৱ "symtab.h"


/*----------------------------.
| Construct the node labels.  |
`----------------------------*/

/* Print the lhs of a rule in such a manner that there is no vertical
   repetition, like in *.output files. */

স্থির শূন্য
print_core (সংরচনা obstack *oout, state *s)
{
  item_index ধ্রুৱ *sitems = s->বস্তুসমূহ;
  sym_content *previous_lhs = রিক্ত;
  size_t snritems = s->nitems;

  /* Output all the items of a state, not just its kernel.  */
  যদি (report_flag & report_itemsets)
    {
      closure (sitems, snritems);
      sitems = itemset;
      snritems = nitemset;
    }

  obstack_printf (oout, _("State %d"), s->সংখ্যা);
  obstack_sgrow (oout, "\\n\\l");
  বাবে (size_t i = 0; i < snritems; ++i)
    {
      item_সংখ্যা ধ্রুৱ *sp1 = ritem + sitems[i];
      rule ধ্রুৱ *r = item_rule (sp1);

      obstack_printf (oout, "%3d ", r->সংখ্যা);
      যদি (previous_lhs && UNIQSTR_EQ (previous_lhs->প্ৰতীক->tag,
                                      r->lhs->প্ৰতীক->tag))
        obstack_printf (oout, "%*s| ",
                        (পূর্ণাঙ্ক) strlen (previous_lhs->প্ৰতীক->tag), "");
      অন্যথা
        {
          obstack_backslash (oout, r->lhs->প্ৰতীক->tag);
          obstack_printf (oout, ": ");
        }
      previous_lhs = r->lhs;

      বাবে (item_সংখ্যা ধ্রুৱ *sp = r->rhs; sp < sp1; sp++)
        {
          obstack_backslash (oout, symbols[*sp]->tag);
          obstack_1grow (oout, ' ');
        }

      obstack_sgrow (oout, "•");

      যদি (0 <= *r->rhs)
        বাবে (item_সংখ্যা ধ্রুৱ *sp = sp1; 0 <= *sp; ++sp)
          {
            obstack_1grow (oout, ' ');
            obstack_backslash (oout, symbols[*sp]->tag);
          }
      অন্যথা
        obstack_sgrow (oout, " %empty");

      /* Experimental feature: display the lookahead tokens. */
      যদি (report_flag & report_lookaheads
          && item_সংখ্যা_is_rule_সংখ্যা (*sp1))
        {
          /* Find the reduction we are handling.  */
          reductions *reds = s->reductions;
          পূর্ণাঙ্ক redno = state_reduction_find (s, r);

          /* Print them if there are.  */
          যদি (reds->lookaheads && redno != -1)
            {
              bitset_iterator biter;
              পূর্ণাঙ্ক k;
              ৱর্ণ ধ্রুৱ *sep = "";
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

স্থির শূন্য
print_actions (state ধ্রুৱ *s, FILE *fgraph)
{
  transitions ধ্রুৱ *trans = s->transitions;

  যদি (!trans->অঙ্ক && !s->reductions)
    ঘূৰাই;

  বাবে (পূর্ণাঙ্ক i = 0; i < trans->অঙ্ক; i++)
    যদি (!TRANSITION_IS_DISABLED (trans, i))
      {
        ধ্রুৱ state *s1 = trans->states[i];
        ধ্রুৱ প্ৰতীক_সংখ্যা sym = s1->accessing_প্ৰতীক;

        /* Shifts are solid, gotos are dashed, and error is dotted.  */
        ৱর্ণ ধ্রুৱ *style =
          (TRANSITION_IS_ERROR (trans, i) ? "dotted"
           : TRANSITION_IS_SHIFT (trans, i) ? "solid"
           : "dashed");

        যদি (TRANSITION_IS_ERROR (trans, i)
            && STRNEQ (symbols[sym]->tag, "error"))
          abort ();
        output_edge (s->সংখ্যা, s1->সংখ্যা,
                     TRANSITION_IS_ERROR (trans, i) ? রিক্ত : symbols[sym]->tag,
                     style, fgraph);
      }
  /* Display reductions. */
  output_red (s, s->reductions, fgraph);
}


/*-------------------------------------------------------------.
| Output in FGRAPH the current node specifications and exiting |
| edges.                                                       |
`-------------------------------------------------------------*/

স্থির শূন্য
print_state (state *s, FILE *fgraph)
{
  সংরচনা obstack গ্ৰন্থি_obstack;

  /* A node's label contains its items.  */
  obstack_init (&গ্ৰন্থি_obstack);
  print_core (&গ্ৰন্থি_obstack, s);
  output_গ্ৰন্থি (s->সংখ্যা, obstack_finish0 (&গ্ৰন্থি_obstack), fgraph);
  obstack_free (&গ্ৰন্থি_obstack, 0);

  /* Output the edges.  */
  print_actions (s, fgraph);
}


শূন্য
print_graph (শূন্য)
{
  FILE *fgraph = xfopen (spec_graph_file, "w");
  start_graph (fgraph);

  /* Output nodes and edges. */
  বাবে (পূর্ণাঙ্ক i = 0; i < nstates; i++)
    print_state (states[i], fgraph);

  finish_graph (fgraph);
  xfclose (fgraph);
}
