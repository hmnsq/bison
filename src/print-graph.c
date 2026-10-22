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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ "print-graph.h"

#அந்தர்பாவ "system.h"

#அந்தர்பாவ "closure.h"
#அந்தர்பாவ "complain.h"
#அந்தர்பாவ "conflicts.h"
#அந்தர்பாவ "files.h"
#அந்தர்பாவ "getargs.h"
#அந்தர்பாவ "gram.h"
#அந்தர்பாவ "graphviz.h"
#அந்தர்பாவ "lalr.h"
#அந்தர்பாவ "lr0.h"
#அந்தர்பாவ "reader.h"
#அந்தர்பாவ "state.h"
#அந்தர்பாவ "symtab.h"


/*----------------------------.
| Construct the node labels.  |
`----------------------------*/

/* Print the lhs of a rule in such a manner that there is no vertical
   repetition, like in *.output files. */

நிலையான வெறுமை
print_core (கட்டமைப்பு obstack *oout, state *s)
{
  item_index மாறிலி *sitems = s->பொருட்கள்;
  sym_content *previous_lhs = வெற்று;
  size_t snritems = s->nitems;

  /* Output all the items of a state, not just its kernel.  */
  எனில் (report_flag & report_itemsets)
    {
      closure (sitems, snritems);
      sitems = itemset;
      snritems = nitemset;
    }

  obstack_printf (oout, _("State %d"), s->எண்);
  obstack_sgrow (oout, "\\n\\l");
  க்கு (size_t i = 0; i < snritems; ++i)
    {
      item_எண் மாறிலி *sp1 = ritem + sitems[i];
      rule மாறிலி *r = item_rule (sp1);

      obstack_printf (oout, "%3d ", r->எண்);
      எனில் (previous_lhs && UNIQSTR_EQ (previous_lhs->குறியீடு->tag,
                                      r->lhs->குறியீடு->tag))
        obstack_printf (oout, "%*s| ",
                        (முழு) strlen (previous_lhs->குறியீடு->tag), "");
      இல்லையெனில்
        {
          obstack_backslash (oout, r->lhs->குறியீடு->tag);
          obstack_printf (oout, ": ");
        }
      previous_lhs = r->lhs;

      க்கு (item_எண் மாறிலி *sp = r->rhs; sp < sp1; sp++)
        {
          obstack_backslash (oout, symbols[*sp]->tag);
          obstack_1grow (oout, ' ');
        }

      obstack_sgrow (oout, "•");

      எனில் (0 <= *r->rhs)
        க்கு (item_எண் மாறிலி *sp = sp1; 0 <= *sp; ++sp)
          {
            obstack_1grow (oout, ' ');
            obstack_backslash (oout, symbols[*sp]->tag);
          }
      இல்லையெனில்
        obstack_sgrow (oout, " %empty");

      /* Experimental feature: display the lookahead tokens. */
      எனில் (report_flag & report_lookaheads
          && item_எண்_is_rule_எண் (*sp1))
        {
          /* Find the reduction we are handling.  */
          reductions *reds = s->reductions;
          முழு redno = state_reduction_find (s, r);

          /* Print them if there are.  */
          எனில் (reds->lookaheads && redno != -1)
            {
              bitset_iterator biter;
              முழு k;
              எழுத்து மாறிலி *sep = "";
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

நிலையான வெறுமை
print_actions (state மாறிலி *s, FILE *fgraph)
{
  transitions மாறிலி *trans = s->transitions;

  எனில் (!trans->இலக்கம் && !s->reductions)
    திரும்பு;

  க்கு (முழு i = 0; i < trans->இலக்கம்; i++)
    எனில் (!TRANSITION_IS_DISABLED (trans, i))
      {
        மாறிலி state *s1 = trans->states[i];
        மாறிலி குறியீடு_எண் sym = s1->accessing_குறியீடு;

        /* Shifts are solid, gotos are dashed, and error is dotted.  */
        எழுத்து மாறிலி *style =
          (TRANSITION_IS_ERROR (trans, i) ? "dotted"
           : TRANSITION_IS_SHIFT (trans, i) ? "solid"
           : "dashed");

        எனில் (TRANSITION_IS_ERROR (trans, i)
            && STRNEQ (symbols[sym]->tag, "error"))
          abort ();
        output_edge (s->எண், s1->எண்,
                     TRANSITION_IS_ERROR (trans, i) ? வெற்று : symbols[sym]->tag,
                     style, fgraph);
      }
  /* Display reductions. */
  output_red (s, s->reductions, fgraph);
}


/*-------------------------------------------------------------.
| Output in FGRAPH the current node specifications and exiting |
| edges.                                                       |
`-------------------------------------------------------------*/

நிலையான வெறுமை
print_state (state *s, FILE *fgraph)
{
  கட்டமைப்பு obstack முனை_obstack;

  /* A node's label contains its items.  */
  obstack_init (&முனை_obstack);
  print_core (&முனை_obstack, s);
  output_முனை (s->எண், obstack_finish0 (&முனை_obstack), fgraph);
  obstack_free (&முனை_obstack, 0);

  /* Output the edges.  */
  print_actions (s, fgraph);
}


வெறுமை
print_graph (வெறுமை)
{
  FILE *fgraph = xfopen (spec_graph_file, "w");
  start_graph (fgraph);

  /* Output nodes and edges. */
  க்கு (முழு i = 0; i < nstates; i++)
    print_state (states[i], fgraph);

  finish_graph (fgraph);
  xfclose (fgraph);
}
