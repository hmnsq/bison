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

#شامل <config.h>
#شامل "print-graph.h"

#شامل "system.h"

#شامل "closure.h"
#شامل "complain.h"
#شامل "conflicts.h"
#شامل "files.h"
#شامل "getargs.h"
#شامل "gram.h"
#شامل "graphviz.h"
#شامل "lalr.h"
#شامل "lr0.h"
#شامل "reader.h"
#شامل "state.h"
#شامل "symtab.h"


/*----------------------------.
| Construct the node labels.  |
`----------------------------*/

/* Print the lhs of a rule in such a manner that there is no vertical
   repetition, like in *.output files. */

ایستا تهی
print_core (ساختار obstack *oout, state *s)
{
  item_index ثابت *sitems = s->اقلام;
  sym_content *previous_lhs = پوچ;
  size_t snritems = s->nitems;

  /* Output all the items of a state, not just its kernel.  */
  اگر (report_flag & report_itemsets)
    {
      closure (sitems, snritems);
      sitems = itemset;
      snritems = nitemset;
    }

  obstack_printf (oout, _("State %d"), s->عدد);
  obstack_sgrow (oout, "\\n\\l");
  برای (size_t i = 0; i < snritems; ++i)
    {
      item_عدد ثابت *sp1 = ritem + sitems[i];
      rule ثابت *r = item_rule (sp1);

      obstack_printf (oout, "%3d ", r->عدد);
      اگر (previous_lhs && UNIQSTR_EQ (previous_lhs->نماد->tag,
                                      r->lhs->نماد->tag))
        obstack_printf (oout, "%*s| ",
                        (صحیح) strlen (previous_lhs->نماد->tag), "");
      وگرنه
        {
          obstack_backslash (oout, r->lhs->نماد->tag);
          obstack_printf (oout, ": ");
        }
      previous_lhs = r->lhs;

      برای (item_عدد ثابت *sp = r->rhs; sp < sp1; sp++)
        {
          obstack_backslash (oout, symbols[*sp]->tag);
          obstack_1grow (oout, ' ');
        }

      obstack_sgrow (oout, "•");

      اگر (0 <= *r->rhs)
        برای (item_عدد ثابت *sp = sp1; 0 <= *sp; ++sp)
          {
            obstack_1grow (oout, ' ');
            obstack_backslash (oout, symbols[*sp]->tag);
          }
      وگرنه
        obstack_sgrow (oout, " %empty");

      /* Experimental feature: display the lookahead tokens. */
      اگر (report_flag & report_lookaheads
          && item_عدد_is_rule_عدد (*sp1))
        {
          /* Find the reduction we are handling.  */
          reductions *reds = s->reductions;
          صحیح redno = state_reduction_find (s, r);

          /* Print them if there are.  */
          اگر (reds->lookaheads && redno != -1)
            {
              bitset_iterator biter;
              صحیح k;
              نویسه ثابت *sep = "";
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

ایستا تهی
print_actions (state ثابت *s, FILE *fgraph)
{
  transitions ثابت *trans = s->transitions;

  اگر (!trans->رقم && !s->reductions)
    بازگشت;

  برای (صحیح i = 0; i < trans->رقم; i++)
    اگر (!TRANSITION_IS_DISABLED (trans, i))
      {
        ثابت state *s1 = trans->states[i];
        ثابت نماد_عدد sym = s1->accessing_نماد;

        /* Shifts are solid, gotos are dashed, and error is dotted.  */
        نویسه ثابت *style =
          (TRANSITION_IS_ERROR (trans, i) ? "dotted"
           : TRANSITION_IS_SHIFT (trans, i) ? "solid"
           : "dashed");

        اگر (TRANSITION_IS_ERROR (trans, i)
            && STRNEQ (symbols[sym]->tag, "error"))
          abort ();
        output_edge (s->عدد, s1->عدد,
                     TRANSITION_IS_ERROR (trans, i) ? پوچ : symbols[sym]->tag,
                     style, fgraph);
      }
  /* Display reductions. */
  output_red (s, s->reductions, fgraph);
}


/*-------------------------------------------------------------.
| Output in FGRAPH the current node specifications and exiting |
| edges.                                                       |
`-------------------------------------------------------------*/

ایستا تهی
print_state (state *s, FILE *fgraph)
{
  ساختار obstack گره_obstack;

  /* A node's label contains its items.  */
  obstack_init (&گره_obstack);
  print_core (&گره_obstack, s);
  output_گره (s->عدد, obstack_finish0 (&گره_obstack), fgraph);
  obstack_free (&گره_obstack, 0);

  /* Output the edges.  */
  print_actions (s, fgraph);
}


تهی
print_graph (تهی)
{
  FILE *fgraph = xfopen (spec_graph_file, "w");
  start_graph (fgraph);

  /* Output nodes and edges. */
  برای (صحیح i = 0; i < nstates; i++)
    print_state (states[i], fgraph);

  finish_graph (fgraph);
  xfclose (fgraph);
}
