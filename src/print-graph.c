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
  شي_اشاريو ثابت *sitems = s->شيون;
  sym_content *اڳيون_lhs = عدم;
  size_t snritems = s->nitems;

  /* Output all the items of a state, not just its kernel.  */
  إذا (report_flag & report_itemsets)
    {
      گهيرو (sitems, snritems);
      sitems = itemset;
      snritems = nitemset;
    }

  obstack_printf (oout, _("State %d"), s->عدد);
  obstack_sgrow (oout, "\\n\\l");
  لكل (size_t i = 0; i < snritems; ++i)
    {
      شي_عدد ثابت *sp1 = ritem + sitems[i];
      قاعدو ثابت *r = شي_قاعدو (sp1);

      obstack_printf (oout, "%3d ", r->عدد);
      إذا (اڳيون_lhs && UNIQSTR_EQ (اڳيون_lhs->نشان->ٽيگ,
                                      r->lhs->نشان->ٽيگ))
        obstack_printf (oout, "%*s| ",
                        (صحيح) لڙي_ڊيگهه (اڳيون_lhs->نشان->ٽيگ), "");
      وإلا
        {
          obstack_backslash (oout, r->lhs->نشان->ٽيگ);
          obstack_printf (oout, ": ");
        }
      اڳيون_lhs = r->lhs;

      لكل (شي_عدد ثابت *sp = r->rhs; sp < sp1; sp++)
        {
          obstack_backslash (oout, symbols[*sp]->ٽيگ);
          obstack_1grow (oout, ' ');
        }

      obstack_sgrow (oout, "•");

      إذا (0 <= *r->rhs)
        لكل (شي_عدد ثابت *sp = sp1; 0 <= *sp; ++sp)
          {
            obstack_1grow (oout, ' ');
            obstack_backslash (oout, symbols[*sp]->ٽيگ);
          }
      وإلا
        obstack_sgrow (oout, " %empty");

      /* Experimental feature: display the lookahead tokens. */
      إذا (report_flag & report_lookaheads
          && شي_عدد_is_قاعدو_عدد (*sp1))
        {
          /* Find the reduction we are handling.  */
          reductions *reds = s->reductions;
          صحيح redno = state_reduction_ڳوليو (s, r);

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
                  obstack_backslash (oout, symbols[k]->ٽيگ);
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

  إذا (!trans->انگ && !s->reductions)
    أرجع;

  لكل (صحيح i = 0; i < trans->انگ; i++)
    إذا (!TRANSITION_IS_DISABLED (trans, i))
      {
        ثابت state *s1 = trans->حالتون[i];
        ثابت نشان_عدد sym = s1->accessing_نشان;

        /* Shifts are solid, gotos are dashed, and error is dotted.  */
        حرف ثابت *style =
          (TRANSITION_IS_ERROR (trans, i) ? "dotted"
           : TRANSITION_IS_سرڪايو (trans, i) ? "solid"
           : "dashed");

        إذا (TRANSITION_IS_ERROR (trans, i)
            && STRNEQ (symbols[sym]->ٽيگ, "error"))
          abort ();
        output_ڪنارو (s->عدد, s1->عدد,
                     TRANSITION_IS_ERROR (trans, i) ? عدم : symbols[sym]->ٽيگ,
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
  بنية obstack ڳنڍ_obstack;

  /* A node's label contains its items.  */
  obstack_init (&ڳنڍ_obstack);
  print_core (&ڳنڍ_obstack, s);
  output_ڳنڍ (s->عدد, obstack_finish0 (&ڳنڍ_obstack), fgraph);
  obstack_free (&ڳنڍ_obstack, 0);

  /* Output the edges.  */
  print_actions (s, fgraph);
}


فراغ
print_گراف (فراغ)
{
  FILE *fgraph = xfopen (spec_گراف_file, "w");
  شروعات_گراف (fgraph);

  /* Output nodes and edges. */
  لكل (صحيح i = 0; i < nstates; i++)
    print_state (حالتون[i], fgraph);

  پڄاڻي_گراف (fgraph);
  xfclose (fgraph);
}
