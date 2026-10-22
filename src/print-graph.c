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

#include <config.h>
#include "print-graph.h"

#include "system.h"

#include "closure.h"
#include "complain.h"
#include "conflicts.h"
#include "files.h"
#include "getargs.h"
#include "gram.h"
#include "graphviz.h"
#include "lalr.h"
#include "lr0.h"
#include "reader.h"
#include "state.h"
#include "symtab.h"


/*----------------------------.
| Construct the node labels.  |
`----------------------------*/

/* Print the lhs of a rule in such a manner that there is no vertical
   repetition, like in *.output files. */

static void
print_core (struct obstack *oout, state *s)
{
  item_index const *sitems = s->items;
  sym_content *previous_lhs = NULL;
  size_t snritems = s->nitems;

  /* Output all the items of a state, not just its kernel.  */
  nếu (report_flag & report_itemsets)
    {
      closure (sitems, snritems);
      sitems = itemset;
      snritems = nitemset;
    }

  obstack_printf (oout, _("State %d"), s->number);
  obstack_sgrow (oout, "\\n\\l");
  cho (size_t i = 0; i < snritems; ++i)
    {
      item_number const *sp1 = ritem + sitems[i];
      rule const *r = item_rule (sp1);

      obstack_printf (oout, "%3d ", r->number);
      nếu (previous_lhs && UNIQSTR_EQ (previous_lhs->symbol->tag,
                                      r->lhs->symbol->tag))
        obstack_printf (oout, "%*s| ",
                        (số_nguyên) strlen (previous_lhs->symbol->tag), "");
      không_thì
        {
          obstack_backslash (oout, r->lhs->symbol->tag);
          obstack_printf (oout, ": ");
        }
      previous_lhs = r->lhs;

      cho (item_number const *sp = r->rhs; sp < sp1; sp++)
        {
          obstack_backslash (oout, symbols[*sp]->tag);
          obstack_1grow (oout, ' ');
        }

      obstack_sgrow (oout, "•");

      nếu (0 <= *r->rhs)
        cho (item_number const *sp = sp1; 0 <= *sp; ++sp)
          {
            obstack_1grow (oout, ' ');
            obstack_backslash (oout, symbols[*sp]->tag);
          }
      không_thì
        obstack_sgrow (oout, " %empty");

      /* Experimental feature: display the lookahead tokens. */
      nếu (report_flag & report_lookaheads
          && item_number_is_rule_number (*sp1))
        {
          /* Find the reduction we are handling.  */
          reductions *reds = s->reductions;
          số_nguyên redno = state_reduction_find (s, r);

          /* Print them if there are.  */
          nếu (reds->lookaheads && redno != -1)
            {
              bitset_iterator biter;
              số_nguyên k;
              char const *sep = "";
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

static void
print_actions (state const *s, FILE *fgraph)
{
  transitions const *trans = s->transitions;

  nếu (!trans->num && !s->reductions)
    trả_về;

  cho (số_nguyên i = 0; i < trans->num; i++)
    nếu (!TRANSITION_IS_DISABLED (trans, i))
      {
        const state *s1 = trans->states[i];
        const symbol_number sym = s1->accessing_symbol;

        /* Shifts are solid, gotos are dashed, and error is dotted.  */
        char const *style =
          (TRANSITION_IS_ERROR (trans, i) ? "dotted"
           : TRANSITION_IS_SHIFT (trans, i) ? "solid"
           : "dashed");

        nếu (TRANSITION_IS_ERROR (trans, i)
            && STRNEQ (symbols[sym]->tag, "error"))
          abort ();
        output_edge (s->number, s1->number,
                     TRANSITION_IS_ERROR (trans, i) ? NULL : symbols[sym]->tag,
                     style, fgraph);
      }
  /* Display reductions. */
  output_red (s, s->reductions, fgraph);
}


/*-------------------------------------------------------------.
| Output in FGRAPH the current node specifications and exiting |
| edges.                                                       |
`-------------------------------------------------------------*/

static void
print_state (state *s, FILE *fgraph)
{
  struct obstack node_obstack;

  /* A node's label contains its items.  */
  obstack_init (&node_obstack);
  print_core (&node_obstack, s);
  output_node (s->number, obstack_finish0 (&node_obstack), fgraph);
  obstack_free (&node_obstack, 0);

  /* Output the edges.  */
  print_actions (s, fgraph);
}


void
print_graph (void)
{
  FILE *fgraph = xfopen (spec_graph_file, "w");
  start_graph (fgraph);

  /* Output nodes and edges. */
  cho (số_nguyên i = 0; i < nstates; i++)
    print_state (states[i], fgraph);

  finish_graph (fgraph);
  xfclose (fgraph);
}
