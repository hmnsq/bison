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
  توکی_شاخص ثابت *sitems = s->توکي;
  sym_content *پخوانی_lhs = عدم;
  size_t snritems = s->nitems;

  /* Output all the items of a state, not just its kernel.  */
  إذا (report_flag & report_itemsets)
    {
      تړون (sitems, snritems);
      sitems = itemset;
      snritems = nitemset;
    }

  obstack_printf (oout, _("State %d"), s->شمېره);
  obstack_sgrow (oout, "\\n\\l");
  لكل (size_t i = 0; i < snritems; ++i)
    {
      توکی_شمېره ثابت *sp1 = ritem + sitems[i];
      قاعده ثابت *r = توکی_قاعده (sp1);

      obstack_printf (oout, "%3d ", r->شمېره);
      إذا (پخوانی_lhs && UNIQSTR_EQ (پخوانی_lhs->نښه->نښان,
                                      r->lhs->نښه->نښان))
        obstack_printf (oout, "%*s| ",
                        (صحيح) مزي_اوږدوالی (پخوانی_lhs->نښه->نښان), "");
      وإلا
        {
          obstack_backslash (oout, r->lhs->نښه->نښان);
          obstack_printf (oout, ": ");
        }
      پخوانی_lhs = r->lhs;

      لكل (توکی_شمېره ثابت *sp = r->rhs; sp < sp1; sp++)
        {
          obstack_backslash (oout, symbols[*sp]->نښان);
          obstack_1grow (oout, ' ');
        }

      obstack_sgrow (oout, "•");

      إذا (0 <= *r->rhs)
        لكل (توکی_شمېره ثابت *sp = sp1; 0 <= *sp; ++sp)
          {
            obstack_1grow (oout, ' ');
            obstack_backslash (oout, symbols[*sp]->نښان);
          }
      وإلا
        obstack_sgrow (oout, " %empty");

      /* Experimental feature: display the lookahead tokens. */
      إذا (report_flag & report_lookaheads
          && توکی_شمېره_is_قاعده_شمېره (*sp1))
        {
          /* Find the reduction we are handling.  */
          reductions *reds = s->reductions;
          صحيح redno = state_reduction_پیدا_کړه (s, r);

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
                  obstack_backslash (oout, symbols[k]->نښان);
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
        ثابت state *s1 = trans->حالتونه[i];
        ثابت نښه_شمېره sym = s1->accessing_نښه;

        /* Shifts are solid, gotos are dashed, and error is dotted.  */
        حرف ثابت *style =
          (TRANSITION_IS_ERROR (trans, i) ? "dotted"
           : TRANSITION_IS_ښويدنه (trans, i) ? "solid"
           : "dashed");

        إذا (TRANSITION_IS_ERROR (trans, i)
            && STRNEQ (symbols[sym]->نښان, "error"))
          abort ();
        output_څنډه (s->شمېره, s1->شمېره,
                     TRANSITION_IS_ERROR (trans, i) ? عدم : symbols[sym]->نښان,
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
  بنية obstack غوټه_obstack;

  /* A node's label contains its items.  */
  obstack_init (&غوټه_obstack);
  print_core (&غوټه_obstack, s);
  output_غوټه (s->شمېره, obstack_finish0 (&غوټه_obstack), fgraph);
  obstack_free (&غوټه_obstack, 0);

  /* Output the edges.  */
  print_actions (s, fgraph);
}


فراغ
print_ګراف (فراغ)
{
  FILE *fgraph = xfopen (spec_ګراف_file, "w");
  پیل_ګراف (fgraph);

  /* Output nodes and edges. */
  لكل (صحيح i = 0; i < nstates; i++)
    print_state (حالتونه[i], fgraph);

  پای_ګراف (fgraph);
  xfclose (fgraph);
}
