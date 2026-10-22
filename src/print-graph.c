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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ "print-graph.h"

#ଅନ୍ତର୍ଭାଵ "system.h"

#ଅନ୍ତର୍ଭାଵ "closure.h"
#ଅନ୍ତର୍ଭାଵ "complain.h"
#ଅନ୍ତର୍ଭାଵ "conflicts.h"
#ଅନ୍ତର୍ଭାଵ "files.h"
#ଅନ୍ତର୍ଭାଵ "getargs.h"
#ଅନ୍ତର୍ଭାଵ "gram.h"
#ଅନ୍ତର୍ଭାଵ "graphviz.h"
#ଅନ୍ତର୍ଭାଵ "lalr.h"
#ଅନ୍ତର୍ଭାଵ "lr0.h"
#ଅନ୍ତର୍ଭାଵ "reader.h"
#ଅନ୍ତର୍ଭାଵ "state.h"
#ଅନ୍ତର୍ଭାଵ "symtab.h"


/*----------------------------.
| Construct the node labels.  |
`----------------------------*/

/* Print the lhs of a rule in such a manner that there is no vertical
   repetition, like in *.output files. */

ସ୍ଥିର ଶୂନ୍ୟ
print_core (ସଂରଚନା obstack *oout, state *s)
{
  item_index ଅଚଳ *sitems = s->ବସ୍ତୁଗୁଡ଼ିକ;
  sym_content *previous_lhs = ଖାଲି;
  size_t snritems = s->nitems;

  /* Output all the items of a state, not just its kernel.  */
  ଯଦି (report_flag & report_itemsets)
    {
      closure (sitems, snritems);
      sitems = itemset;
      snritems = nitemset;
    }

  obstack_printf (oout, _("State %d"), s->ସଂଖ୍ୟା);
  obstack_sgrow (oout, "\\n\\l");
  ପାଇଁ (size_t i = 0; i < snritems; ++i)
    {
      item_ସଂଖ୍ୟା ଅଚଳ *sp1 = ritem + sitems[i];
      rule ଅଚଳ *r = item_rule (sp1);

      obstack_printf (oout, "%3d ", r->ସଂଖ୍ୟା);
      ଯଦି (previous_lhs && UNIQSTR_EQ (previous_lhs->ପ୍ରତୀକ->tag,
                                      r->lhs->ପ୍ରତୀକ->tag))
        obstack_printf (oout, "%*s| ",
                        (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) strlen (previous_lhs->ପ୍ରତୀକ->tag), "");
      ଅନ୍ୟଥା
        {
          obstack_backslash (oout, r->lhs->ପ୍ରତୀକ->tag);
          obstack_printf (oout, ": ");
        }
      previous_lhs = r->lhs;

      ପାଇଁ (item_ସଂଖ୍ୟା ଅଚଳ *sp = r->rhs; sp < sp1; sp++)
        {
          obstack_backslash (oout, symbols[*sp]->tag);
          obstack_1grow (oout, ' ');
        }

      obstack_sgrow (oout, "•");

      ଯଦି (0 <= *r->rhs)
        ପାଇଁ (item_ସଂଖ୍ୟା ଅଚଳ *sp = sp1; 0 <= *sp; ++sp)
          {
            obstack_1grow (oout, ' ');
            obstack_backslash (oout, symbols[*sp]->tag);
          }
      ଅନ୍ୟଥା
        obstack_sgrow (oout, " %empty");

      /* Experimental feature: display the lookahead tokens. */
      ଯଦି (report_flag & report_lookaheads
          && item_ସଂଖ୍ୟା_is_rule_ସଂଖ୍ୟା (*sp1))
        {
          /* Find the reduction we are handling.  */
          reductions *reds = s->reductions;
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା redno = state_reduction_find (s, r);

          /* Print them if there are.  */
          ଯଦି (reds->lookaheads && redno != -1)
            {
              bitset_iterator biter;
              ପୂର୍ଣ୍ଣସଂଖ୍ୟା k;
              ଅକ୍ଷର ଅଚଳ *sep = "";
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

ସ୍ଥିର ଶୂନ୍ୟ
print_actions (state ଅଚଳ *s, FILE *fgraph)
{
  transitions ଅଚଳ *trans = s->transitions;

  ଯଦି (!trans->ଅଙ୍କ && !s->reductions)
    ଫେରନ୍ତୁ;

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < trans->ଅଙ୍କ; i++)
    ଯଦି (!TRANSITION_IS_DISABLED (trans, i))
      {
        ଅଚଳ state *s1 = trans->states[i];
        ଅଚଳ ପ୍ରତୀକ_ସଂଖ୍ୟା sym = s1->accessing_ପ୍ରତୀକ;

        /* Shifts are solid, gotos are dashed, and error is dotted.  */
        ଅକ୍ଷର ଅଚଳ *style =
          (TRANSITION_IS_ERROR (trans, i) ? "dotted"
           : TRANSITION_IS_SHIFT (trans, i) ? "solid"
           : "dashed");

        ଯଦି (TRANSITION_IS_ERROR (trans, i)
            && STRNEQ (symbols[sym]->tag, "error"))
          abort ();
        output_edge (s->ସଂଖ୍ୟା, s1->ସଂଖ୍ୟା,
                     TRANSITION_IS_ERROR (trans, i) ? ଖାଲି : symbols[sym]->tag,
                     style, fgraph);
      }
  /* Display reductions. */
  output_red (s, s->reductions, fgraph);
}


/*-------------------------------------------------------------.
| Output in FGRAPH the current node specifications and exiting |
| edges.                                                       |
`-------------------------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
print_state (state *s, FILE *fgraph)
{
  ସଂରଚନା obstack ଗଣ୍ଠି_obstack;

  /* A node's label contains its items.  */
  obstack_init (&ଗଣ୍ଠି_obstack);
  print_core (&ଗଣ୍ଠି_obstack, s);
  output_ଗଣ୍ଠି (s->ସଂଖ୍ୟା, obstack_finish0 (&ଗଣ୍ଠି_obstack), fgraph);
  obstack_free (&ଗଣ୍ଠି_obstack, 0);

  /* Output the edges.  */
  print_actions (s, fgraph);
}


ଶୂନ୍ୟ
print_graph (ଶୂନ୍ୟ)
{
  FILE *fgraph = xfopen (spec_graph_file, "w");
  start_graph (fgraph);

  /* Output nodes and edges. */
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < nstates; i++)
    print_state (states[i], fgraph);

  finish_graph (fgraph);
  xfclose (fgraph);
}
