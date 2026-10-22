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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ "print-graph.h"

#અન્તર્ભાવ "system.h"

#અન્તર્ભાવ "closure.h"
#અન્તર્ભાવ "complain.h"
#અન્તર્ભાવ "conflicts.h"
#અન્તર્ભાવ "files.h"
#અન્તર્ભાવ "getargs.h"
#અન્તર્ભાવ "gram.h"
#અન્તર્ભાવ "graphviz.h"
#અન્તર્ભાવ "lalr.h"
#અન્તર્ભાવ "lr0.h"
#અન્તર્ભાવ "reader.h"
#અન્તર્ભાવ "state.h"
#અન્તર્ભાવ "symtab.h"


/*----------------------------.
| Construct the node labels.  |
`----------------------------*/

/* Print the lhs of a rule in such a manner that there is no vertical
   repetition, like in *.output files. */

સ્થિર શૂન્ય
print_core (સંરચના obstack *oout, state *s)
{
  item_index અચલ *sitems = s->વસ્તુઓ;
  sym_content *previous_lhs = ખાલી;
  size_t snritems = s->nitems;

  /* Output all the items of a state, not just its kernel.  */
  જો (report_flag & report_itemsets)
    {
      closure (sitems, snritems);
      sitems = itemset;
      snritems = nitemset;
    }

  obstack_printf (oout, _("State %d"), s->સંખ્યા);
  obstack_sgrow (oout, "\\n\\l");
  માટે (size_t i = 0; i < snritems; ++i)
    {
      item_સંખ્યા અચલ *sp1 = ritem + sitems[i];
      rule અચલ *r = item_rule (sp1);

      obstack_printf (oout, "%3d ", r->સંખ્યા);
      જો (previous_lhs && UNIQSTR_EQ (previous_lhs->પ્રતીક->tag,
                                      r->lhs->પ્રતીક->tag))
        obstack_printf (oout, "%*s| ",
                        (પૂર્ણાંક) strlen (previous_lhs->પ્રતીક->tag), "");
      નહીંતર
        {
          obstack_backslash (oout, r->lhs->પ્રતીક->tag);
          obstack_printf (oout, ": ");
        }
      previous_lhs = r->lhs;

      માટે (item_સંખ્યા અચલ *sp = r->rhs; sp < sp1; sp++)
        {
          obstack_backslash (oout, symbols[*sp]->tag);
          obstack_1grow (oout, ' ');
        }

      obstack_sgrow (oout, "•");

      જો (0 <= *r->rhs)
        માટે (item_સંખ્યા અચલ *sp = sp1; 0 <= *sp; ++sp)
          {
            obstack_1grow (oout, ' ');
            obstack_backslash (oout, symbols[*sp]->tag);
          }
      નહીંતર
        obstack_sgrow (oout, " %empty");

      /* Experimental feature: display the lookahead tokens. */
      જો (report_flag & report_lookaheads
          && item_સંખ્યા_is_rule_સંખ્યા (*sp1))
        {
          /* Find the reduction we are handling.  */
          reductions *reds = s->reductions;
          પૂર્ણાંક redno = state_reduction_find (s, r);

          /* Print them if there are.  */
          જો (reds->lookaheads && redno != -1)
            {
              bitset_iterator biter;
              પૂર્ણાંક k;
              અક્ષર અચલ *sep = "";
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

સ્થિર શૂન્ય
print_actions (state અચલ *s, FILE *fgraph)
{
  transitions અચલ *trans = s->transitions;

  જો (!trans->અંક && !s->reductions)
    પાછા;

  માટે (પૂર્ણાંક i = 0; i < trans->અંક; i++)
    જો (!TRANSITION_IS_DISABLED (trans, i))
      {
        અચલ state *s1 = trans->states[i];
        અચલ પ્રતીક_સંખ્યા sym = s1->accessing_પ્રતીક;

        /* Shifts are solid, gotos are dashed, and error is dotted.  */
        અક્ષર અચલ *style =
          (TRANSITION_IS_ERROR (trans, i) ? "dotted"
           : TRANSITION_IS_SHIFT (trans, i) ? "solid"
           : "dashed");

        જો (TRANSITION_IS_ERROR (trans, i)
            && STRNEQ (symbols[sym]->tag, "error"))
          abort ();
        output_edge (s->સંખ્યા, s1->સંખ્યા,
                     TRANSITION_IS_ERROR (trans, i) ? ખાલી : symbols[sym]->tag,
                     style, fgraph);
      }
  /* Display reductions. */
  output_red (s, s->reductions, fgraph);
}


/*-------------------------------------------------------------.
| Output in FGRAPH the current node specifications and exiting |
| edges.                                                       |
`-------------------------------------------------------------*/

સ્થિર શૂન્ય
print_state (state *s, FILE *fgraph)
{
  સંરચના obstack ગાંઠ_obstack;

  /* A node's label contains its items.  */
  obstack_init (&ગાંઠ_obstack);
  print_core (&ગાંઠ_obstack, s);
  output_ગાંઠ (s->સંખ્યા, obstack_finish0 (&ગાંઠ_obstack), fgraph);
  obstack_free (&ગાંઠ_obstack, 0);

  /* Output the edges.  */
  print_actions (s, fgraph);
}


શૂન્ય
print_graph (શૂન્ય)
{
  FILE *fgraph = xfopen (spec_graph_file, "w");
  start_graph (fgraph);

  /* Output nodes and edges. */
  માટે (પૂર્ણાંક i = 0; i < nstates; i++)
    print_state (states[i], fgraph);

  finish_graph (fgraph);
  xfclose (fgraph);
}
