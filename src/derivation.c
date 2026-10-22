/* Counterexample derivation trees

   Copyright (C) 2020-2022 Free Software Foundation, Inc.

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

#ضمن "derivation.h"
#ضمن "glyphs.h"

#ضمن <c-ctype.h>
#ضمن <gl_linked_list.h>
#ضمن <mbswidth.h>
#ضمن <vasnprintf.h>

#ضمن "system.h"
#ضمن "complain.h"

بنية derivation
{
  نشان_عدد sym;
  derivation_list children;
  صحيح reference_ڳڻپ;
  // The rule SYM -> CHILDREN.
  ثابت قاعدو *قاعدو;
  // Color assigned for styling.  Guarantees that the derivation is
  // always displayed with the same color, independently of the order
  // in which the derivations are traversed.
  صحيح color;
};

ساكن derivation d_dot = { -1, عدم, -1, عدم, -1 };

derivation *
derivation_dot (فراغ)
{
  أرجع &d_dot;
}

فراغ
derivation_list_append (derivation_list dl, derivation *d)
{
  derivation_retain (d);
  gl_list_جوڙيو_آخري (dl, d);
}

فراغ
derivation_list_prepend (derivation_list dl, derivation *d)
{
  derivation_retain (d);
  gl_list_جوڙيو_پهريون (dl, d);
}

فراغ derivation_list_free (derivation_list dl)
{
  derivation *d = عدم;
  لكل (gl_list_iterator_t it = gl_list_iterator (dl);
       derivation_list_ايندڙ (&it, &d);
       )
    إذا (d != &d_dot)
      derivation_free (d);
  gl_list_free (dl);
}

derivation *
derivation_new (نشان_عدد sym, derivation_list children,
                ثابت قاعدو *r)
{
  derivation *res = xmalloc (حجم *res);
  res->sym = sym;
  res->children = children;
  res->reference_ڳڻپ = 0;
  res->قاعدو = r;
  res->color = -1;
  أرجع res;
}

فراغ
derivation_retain (derivation *d)
{
  ++d->reference_ڳڻپ;
}

فراغ
derivation_free (derivation *d)
{
  إذا (!d)
    أرجع;
  derivation_list free_قطار =
    gl_list_create (GL_LINKED_LIST, عدم, عدم, عدم, true,
                    1, (ثابت فراغ **)&d);
  طالما (gl_list_سائيز (free_قطار) > 0)
    {
      derivation *deriv = (derivation *) gl_list_get_at (free_قطار, 0);
      إذا (--deriv->reference_ڳڻپ == 0)
        {
          إذا (deriv->children)
            {
              derivation *ٻار = عدم;
              لكل (gl_list_iterator_t it = gl_list_iterator (deriv->children);
                   derivation_list_ايندڙ (&it, &ٻار);
                   )
                إذا (ٻار != &d_dot)
                  gl_list_جوڙيو_آخري (free_قطار, ٻار);
              gl_list_free (deriv->children);
            }
          حرر (deriv);
        }
      gl_list_remove_at (free_قطار, 0);
    }
  gl_list_free (free_قطار);
}

size_t
derivation_سائيز (ثابت derivation *deriv)
{
  إذا (!deriv->children)
    أرجع 1;
  صحيح سائيز = 1;
  derivation *ٻار = عدم;
  لكل (gl_list_iterator_t it = gl_list_iterator (deriv->children);
       derivation_list_ايندڙ (&it, &ٻار);
       )
    سائيز += derivation_سائيز (ٻار);
  أرجع سائيز;
}


// Longest distance from root to leaf.
ساكن صحيح
derivation_اونهائي (ثابت derivation *deriv)
{
  إذا (deriv->children)
    {
      // Children's depth cannot be 0, even if there are no children
      // (the case of a derivation with an empty RHS).
      صحيح res = 1;
      derivation *ٻار;
      لكل (gl_list_iterator_t it = gl_list_iterator (deriv->children);
           derivation_list_ايندڙ (&it, &ٻار);
           )
        res = max_int (res, derivation_اونهائي (ٻار));
      أرجع res + 1;
    }
  وإلا
    أرجع 1;
}

ساكن bool
all_spaces (ثابت حرف *s)
{
  طالما (c_isspace (*s))
    s++;
  أرجع *s == '\0';
}

// Printing the derivation as trees without trailing spaces is
// painful: we cannot simply pad one "column" before moving to the
// next:
//
// exp
// ↳ x1          e1  foo1                      x1
//   ↳ x2        ↳ ε ↳ foo2                    ↳ x2
//     ↳ x3            ↳ foo3                    ↳ x3
//       ↳ "X" •         ↳ x1        foo4          ↳ "X"
//                         ↳ x2      ↳ "quuux"
//                           ↳ x3
//                             ↳ "X"
//
// It's hard for a column to know that it's "last" to decide whether
// to output the right-padding or not.  So when we need to pad on the
// right to complete a column, we don't output the spaces, we
// accumulate the width of padding in *PADDING.
//
// Each time we actually print something (non space), we flush that
// padding.  When we _don't_ print something, its width is added to
// the current padding.
//
// This function implements this.
//
// When COND is true, put S on OUT, preceded by *PADDING white spaces.
// Otherwise add the width to *PADDING.  Return the width of S.
ساكن صحيح
fputs_if (bool cond, FILE *ٻاهريون, صحيح *padding, ثابت حرف *s)
{
  صحيح res = mbswidth (s, 0);
  إذا (cond && !all_spaces (s))
    {
      fprintf (ٻاهريون, "%*s%s", *padding, "", s);
      *padding = 0;
    }
  وإلا
    {
      *padding += res;
    }
  أرجع res;
}

ساكن صحيح
fprintf_if (bool cond, FILE *ٻاهريون, صحيح *padding, ثابت حرف *fmt, ...)
{
  حرف buf[256];
  size_t len = حجم (buf);
  va_list دليلون;
  va_شروعات (دليلون, fmt);
  حرف *cp = vasnprintf (buf, &len, fmt, دليلون);
  va_پڇاڙي (دليلون);
  إذا (!cp)
    xalloc_die ();
  صحيح res = fputs_if (cond, ٻاهريون, padding, cp);
  إذا (cp != buf)
    حرر (cp);
  أرجع res;
}

// The width taken to report this derivation recursively down to its
// leaves.
ساكن صحيح
derivation_ويڪر (ثابت derivation *deriv)
{
  إذا (deriv->children)
    {
      ثابت نشان *sym = symbols[deriv->sym];
      صحيح self_ويڪر = mbswidth (sym->ٽيگ, 0);

      // Arrow and space.
      صحيح children_ويڪر = هيٺ_ڏانهن_arrow_ويڪر;
      children_ويڪر += snprintf (عدم, 0, "%d: ", deriv->قاعدو->عدد);
      إذا (gl_list_سائيز (deriv->children) == 0)
        // Empty rhs.
        children_ويڪر += خالي_جايون_ويڪر;
      وإلا
        {
          إذا (gl_list_سائيز (deriv->children) == 1
              && gl_list_get_پهريون (deriv->children) == &d_dot)
            {
              children_ويڪر += خالي_جايون_ويڪر;
              children_ويڪر += derivation_separator_ويڪر;
            }

          derivation *ٻار;
          لكل (gl_list_iterator_t it = gl_list_iterator (deriv->children);
               derivation_list_ايندڙ (&it, &ٻار);
               )
            children_ويڪر
              += derivation_separator_ويڪر + derivation_ويڪر (ٻار);
          // No separator at the beginning.
          children_ويڪر -= derivation_separator_ويڪر;
        }
      أرجع max_int (self_ويڪر, children_ويڪر);
    }
  وإلا إذا (deriv == &d_dot)
    {
      أرجع dot_ويڪر;
    }
  وإلا // leaf.
    {
      ثابت نشان *sym = symbols[deriv->sym];
      أرجع mbswidth (sym->ٽيگ, 0);
    }
}


// Print DERIV for DEPTH.
//
// The tree is printed from top to bottom with DEPTH ranging from 0 to
// the total depth of the tree.  DERIV should only printed when we
// reach its depth, i.e., then DEPTH is 0.
//
// When DEPTH is 1 and we're on a subderivation, then we print the RHS
// of the derivation (in DEPTH 0 we printed its LHS).
//
// Return the "logical printed" width.  We might have not have reached
// that width, in which case the missing spaces are in *PADDING.
ساكن صحيح
derivation_print_وڻ_impl (ثابت derivation *deriv, FILE *ٻاهريون,
                            صحيح اونهائي, صحيح *padding)
{
  ثابت صحيح ويڪر = derivation_ويڪر (deriv);

  صحيح res = 0;
  إذا (deriv->children)
    {
      ثابت نشان *sym = symbols[deriv->sym];
      حرف style[20];
      snprintf (style, 20, "cex-%d", deriv->color);

      إذا (اونهائي == 0 || اونهائي == 1)
        {
          begin_use_class (style, ٻاهريون);
          begin_use_class ("cex-step", ٻاهريون);
        }
      إذا (اونهائي == 0)
        {
          res += fputs_if (true, ٻاهريون, padding, sym->ٽيگ);
        }
      وإلا
        {
          res += fputs_if (اونهائي == 1, ٻاهريون, padding, هيٺ_ڏانهن_arrow);
          res += fprintf_if (اونهائي == 1, ٻاهريون, padding, "%d: ", deriv->قاعدو->عدد);
          إذا (gl_list_سائيز (deriv->children) == 0)
            // Empty rhs.
            res += fputs_if (اونهائي == 1, ٻاهريون, padding, خالي_جايون);
          وإلا
            {
              إذا (gl_list_سائيز (deriv->children) == 1
                  && gl_list_get_پهريون (deriv->children) == &d_dot)
                {
                  res += fputs_if (اونهائي == 1, ٻاهريون, padding, خالي_جايون);
                  res += fputs_if (اونهائي == 1, ٻاهريون, padding, derivation_separator);
                }

              bool پهريون = true;
              derivation *ٻار;
              لكل (gl_list_iterator_t it = gl_list_iterator (deriv->children);
                   derivation_list_ايندڙ (&it, &ٻار);
                   )
                {
                  إذا (!پهريون)
                    res += fputs_if (اونهائي == 1, ٻاهريون, padding, derivation_separator);
                  res += derivation_print_وڻ_impl (ٻار, ٻاهريون, اونهائي - 1, padding);
                  پهريون = false;
                }
            }
        }
      إذا (اونهائي == 0 || اونهائي == 1)
        {
          پڇاڙي_use_class ("cex-step", ٻاهريون);
          پڇاڙي_use_class (style, ٻاهريون);
        }
      *padding += ويڪر - res;
      res = ويڪر;
    }
  وإلا إذا (deriv == &d_dot)
    {
      إذا (اونهائي == 0)
        begin_use_class ("cex-dot", ٻاهريون);
      res += fputs_if (اونهائي == 0, ٻاهريون, padding, dot);
      إذا (اونهائي == 0)
        پڇاڙي_use_class ("cex-dot", ٻاهريون);
    }
  وإلا // leaf.
    {
      ثابت نشان *sym = symbols[deriv->sym];
      إذا (اونهائي == 0)
        begin_use_class ("cex-leaf", ٻاهريون);
      res += fputs_if (اونهائي == 0, ٻاهريون, padding, sym->ٽيگ);
      إذا (اونهائي == 0)
        پڇاڙي_use_class ("cex-leaf", ٻاهريون);
    }
  أرجع res;
}

ساكن فراغ
derivation_print_وڻ (ثابت derivation *deriv, FILE *ٻاهريون, ثابت حرف *اڳياڙي)
{
  fputc ('\n', ٻاهريون);
  لكل (صحيح اونهائي = 0, max_اونهائي = derivation_اونهائي (deriv);
       اونهائي < max_اونهائي; ++اونهائي)
    {
      صحيح padding = 0;
      fprintf (ٻاهريون, "    %s", اڳياڙي);
      derivation_print_وڻ_impl (deriv, ٻاهريون, اونهائي, &padding);
      fputc ('\n', ٻاهريون);
    }
}


/* Print DERIV, colored according to COUNTER.
   Return false if nothing is printed.  */
ساكن bool
derivation_print_flat_impl (derivation *deriv, FILE *ٻاهريون,
                            bool leaves_only,
                            صحيح *counter, ثابت حرف *اڳياڙي)
{
  إذا (deriv->children)
    {
      ثابت نشان *sym = symbols[deriv->sym];
      deriv->color = *counter;
      ++*counter;
      حرف style[20];
      snprintf (style, 20, "cex-%d", deriv->color);
      begin_use_class (style, ٻاهريون);

      إذا (!leaves_only)
        {
          fputs (اڳياڙي, ٻاهريون);
          begin_use_class ("cex-step", ٻاهريون);
          fprintf (ٻاهريون, "%s %s [ ", sym->ٽيگ, arrow);
          پڇاڙي_use_class ("cex-step", ٻاهريون);
          اڳياڙي = "";
        }
      bool res = false;
      derivation *ٻار;
      لكل (gl_list_iterator_t it = gl_list_iterator (deriv->children);
           derivation_list_ايندڙ (&it, &ٻار);
           )
        {
          إذا (derivation_print_flat_impl (ٻار, ٻاهريون,
                                          leaves_only, counter, اڳياڙي))
            {
              اڳياڙي = " ";
              res = true;
            }
          وإلا إذا (!leaves_only)
            اڳياڙي = " ";
        }
      إذا (!leaves_only)
        {
          begin_use_class ("cex-step", ٻاهريون);
          إذا (res)
            fputs (" ]", ٻاهريون);
          وإلا
            fputs ("]", ٻاهريون);
          پڇاڙي_use_class ("cex-step", ٻاهريون);
        }
      پڇاڙي_use_class (style, ٻاهريون);
      أرجع res;
    }
  وإلا إذا (deriv == &d_dot)
    {
      fputs (اڳياڙي, ٻاهريون);
      begin_use_class ("cex-dot", ٻاهريون);
      fputs (dot, ٻاهريون);
      پڇاڙي_use_class ("cex-dot", ٻاهريون);
    }
  وإلا // leaf.
    {
      fputs (اڳياڙي, ٻاهريون);
      ثابت نشان *sym = symbols[deriv->sym];
      begin_use_class ("cex-leaf", ٻاهريون);
      fprintf (ٻاهريون, "%s", sym->ٽيگ);
      پڇاڙي_use_class ("cex-leaf", ٻاهريون);
    }
  أرجع true;
}

ساكن فراغ
derivation_print_flat (ثابت derivation *deriv, FILE *ٻاهريون, ثابت حرف *اڳياڙي)
{
  صحيح counter = 0;
  fputs (اڳياڙي, ٻاهريون);
  derivation_print_flat_impl ((derivation *)deriv, ٻاهريون, false, &counter, "");
  fputc ('\n', ٻاهريون);
}

فراغ
derivation_print_leaves (ثابت derivation *deriv, FILE *ٻاهريون)
{
  صحيح counter = 0;
  derivation_print_flat_impl ((derivation *)deriv, ٻاهريون, true, &counter, "");
  fputc ('\n', ٻاهريون);
}

فراغ
derivation_print (ثابت derivation *deriv, FILE *ٻاهريون, ثابت حرف *اڳياڙي)
{
  إذا (getenv ("YYFLAT"))
    derivation_print_flat (deriv, ٻاهريون, اڳياڙي);
  وإلا
    derivation_print_وڻ (deriv, ٻاهريون, اڳياڙي);
}
