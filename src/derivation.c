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
  نښه_شمېره sym;
  derivation_list children;
  صحيح reference_شمېر;
  // The rule SYM -> CHILDREN.
  ثابت قاعده *قاعده;
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
  gl_list_جمع_کړه_وروستی (dl, d);
}

فراغ
derivation_list_prepend (derivation_list dl, derivation *d)
{
  derivation_retain (d);
  gl_list_جمع_کړه_لومړی (dl, d);
}

فراغ derivation_list_free (derivation_list dl)
{
  derivation *d = عدم;
  لكل (gl_list_iterator_t it = gl_list_iterator (dl);
       derivation_list_بل (&it, &d);
       )
    إذا (d != &d_dot)
      derivation_free (d);
  gl_list_free (dl);
}

derivation *
derivation_new (نښه_شمېره sym, derivation_list children,
                ثابت قاعده *r)
{
  derivation *res = xmalloc (حجم *res);
  res->sym = sym;
  res->children = children;
  res->reference_شمېر = 0;
  res->قاعده = r;
  res->color = -1;
  أرجع res;
}

فراغ
derivation_retain (derivation *d)
{
  ++d->reference_شمېر;
}

فراغ
derivation_free (derivation *d)
{
  إذا (!d)
    أرجع;
  derivation_list free_کتار =
    gl_list_create (GL_LINKED_LIST, عدم, عدم, عدم, true,
                    1, (ثابت فراغ **)&d);
  طالما (gl_list_اندازه (free_کتار) > 0)
    {
      derivation *deriv = (derivation *) gl_list_get_at (free_کتار, 0);
      إذا (--deriv->reference_شمېر == 0)
        {
          إذا (deriv->children)
            {
              derivation *ماشوم = عدم;
              لكل (gl_list_iterator_t it = gl_list_iterator (deriv->children);
                   derivation_list_بل (&it, &ماشوم);
                   )
                إذا (ماشوم != &d_dot)
                  gl_list_جمع_کړه_وروستی (free_کتار, ماشوم);
              gl_list_free (deriv->children);
            }
          حرر (deriv);
        }
      gl_list_remove_at (free_کتار, 0);
    }
  gl_list_free (free_کتار);
}

size_t
derivation_اندازه (ثابت derivation *deriv)
{
  إذا (!deriv->children)
    أرجع 1;
  صحيح اندازه = 1;
  derivation *ماشوم = عدم;
  لكل (gl_list_iterator_t it = gl_list_iterator (deriv->children);
       derivation_list_بل (&it, &ماشوم);
       )
    اندازه += derivation_اندازه (ماشوم);
  أرجع اندازه;
}


// Longest distance from root to leaf.
ساكن صحيح
derivation_ژوروالی (ثابت derivation *deriv)
{
  إذا (deriv->children)
    {
      // Children's depth cannot be 0, even if there are no children
      // (the case of a derivation with an empty RHS).
      صحيح res = 1;
      derivation *ماشوم;
      لكل (gl_list_iterator_t it = gl_list_iterator (deriv->children);
           derivation_list_بل (&it, &ماشوم);
           )
        res = max_int (res, derivation_ژوروالی (ماشوم));
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
fputs_if (bool cond, FILE *بهر, صحيح *padding, ثابت حرف *s)
{
  صحيح res = mbswidth (s, 0);
  إذا (cond && !all_spaces (s))
    {
      fprintf (بهر, "%*s%s", *padding, "", s);
      *padding = 0;
    }
  وإلا
    {
      *padding += res;
    }
  أرجع res;
}

ساكن صحيح
fprintf_if (bool cond, FILE *بهر, صحيح *padding, ثابت حرف *fmt, ...)
{
  حرف buf[256];
  size_t len = حجم (buf);
  va_list دلیلونه;
  va_پیل (دلیلونه, fmt);
  حرف *cp = vasnprintf (buf, &len, fmt, دلیلونه);
  va_پای_ټکی (دلیلونه);
  إذا (!cp)
    xalloc_die ();
  صحيح res = fputs_if (cond, بهر, padding, cp);
  إذا (cp != buf)
    حرر (cp);
  أرجع res;
}

// The width taken to report this derivation recursively down to its
// leaves.
ساكن صحيح
derivation_پلنوالی (ثابت derivation *deriv)
{
  إذا (deriv->children)
    {
      ثابت نښه *sym = symbols[deriv->sym];
      صحيح self_پلنوالی = mbswidth (sym->نښان, 0);

      // Arrow and space.
      صحيح children_پلنوالی = ښکته_arrow_پلنوالی;
      children_پلنوالی += snprintf (عدم, 0, "%d: ", deriv->قاعده->شمېره);
      إذا (gl_list_اندازه (deriv->children) == 0)
        // Empty rhs.
        children_پلنوالی += تشه_پلنوالی;
      وإلا
        {
          إذا (gl_list_اندازه (deriv->children) == 1
              && gl_list_get_لومړی (deriv->children) == &d_dot)
            {
              children_پلنوالی += تشه_پلنوالی;
              children_پلنوالی += derivation_separator_پلنوالی;
            }

          derivation *ماشوم;
          لكل (gl_list_iterator_t it = gl_list_iterator (deriv->children);
               derivation_list_بل (&it, &ماشوم);
               )
            children_پلنوالی
              += derivation_separator_پلنوالی + derivation_پلنوالی (ماشوم);
          // No separator at the beginning.
          children_پلنوالی -= derivation_separator_پلنوالی;
        }
      أرجع max_int (self_پلنوالی, children_پلنوالی);
    }
  وإلا إذا (deriv == &d_dot)
    {
      أرجع dot_پلنوالی;
    }
  وإلا // leaf.
    {
      ثابت نښه *sym = symbols[deriv->sym];
      أرجع mbswidth (sym->نښان, 0);
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
derivation_print_ونه_impl (ثابت derivation *deriv, FILE *بهر,
                            صحيح ژوروالی, صحيح *padding)
{
  ثابت صحيح پلنوالی = derivation_پلنوالی (deriv);

  صحيح res = 0;
  إذا (deriv->children)
    {
      ثابت نښه *sym = symbols[deriv->sym];
      حرف style[20];
      snprintf (style, 20, "cex-%d", deriv->color);

      إذا (ژوروالی == 0 || ژوروالی == 1)
        {
          begin_use_class (style, بهر);
          begin_use_class ("cex-step", بهر);
        }
      إذا (ژوروالی == 0)
        {
          res += fputs_if (true, بهر, padding, sym->نښان);
        }
      وإلا
        {
          res += fputs_if (ژوروالی == 1, بهر, padding, ښکته_arrow);
          res += fprintf_if (ژوروالی == 1, بهر, padding, "%d: ", deriv->قاعده->شمېره);
          إذا (gl_list_اندازه (deriv->children) == 0)
            // Empty rhs.
            res += fputs_if (ژوروالی == 1, بهر, padding, تشه);
          وإلا
            {
              إذا (gl_list_اندازه (deriv->children) == 1
                  && gl_list_get_لومړی (deriv->children) == &d_dot)
                {
                  res += fputs_if (ژوروالی == 1, بهر, padding, تشه);
                  res += fputs_if (ژوروالی == 1, بهر, padding, derivation_separator);
                }

              bool لومړی = true;
              derivation *ماشوم;
              لكل (gl_list_iterator_t it = gl_list_iterator (deriv->children);
                   derivation_list_بل (&it, &ماشوم);
                   )
                {
                  إذا (!لومړی)
                    res += fputs_if (ژوروالی == 1, بهر, padding, derivation_separator);
                  res += derivation_print_ونه_impl (ماشوم, بهر, ژوروالی - 1, padding);
                  لومړی = false;
                }
            }
        }
      إذا (ژوروالی == 0 || ژوروالی == 1)
        {
          پای_ټکی_use_class ("cex-step", بهر);
          پای_ټکی_use_class (style, بهر);
        }
      *padding += پلنوالی - res;
      res = پلنوالی;
    }
  وإلا إذا (deriv == &d_dot)
    {
      إذا (ژوروالی == 0)
        begin_use_class ("cex-dot", بهر);
      res += fputs_if (ژوروالی == 0, بهر, padding, dot);
      إذا (ژوروالی == 0)
        پای_ټکی_use_class ("cex-dot", بهر);
    }
  وإلا // leaf.
    {
      ثابت نښه *sym = symbols[deriv->sym];
      إذا (ژوروالی == 0)
        begin_use_class ("cex-leaf", بهر);
      res += fputs_if (ژوروالی == 0, بهر, padding, sym->نښان);
      إذا (ژوروالی == 0)
        پای_ټکی_use_class ("cex-leaf", بهر);
    }
  أرجع res;
}

ساكن فراغ
derivation_print_ونه (ثابت derivation *deriv, FILE *بهر, ثابت حرف *مختاړی)
{
  fputc ('\n', بهر);
  لكل (صحيح ژوروالی = 0, max_ژوروالی = derivation_ژوروالی (deriv);
       ژوروالی < max_ژوروالی; ++ژوروالی)
    {
      صحيح padding = 0;
      fprintf (بهر, "    %s", مختاړی);
      derivation_print_ونه_impl (deriv, بهر, ژوروالی, &padding);
      fputc ('\n', بهر);
    }
}


/* Print DERIV, colored according to COUNTER.
   Return false if nothing is printed.  */
ساكن bool
derivation_print_flat_impl (derivation *deriv, FILE *بهر,
                            bool leaves_only,
                            صحيح *counter, ثابت حرف *مختاړی)
{
  إذا (deriv->children)
    {
      ثابت نښه *sym = symbols[deriv->sym];
      deriv->color = *counter;
      ++*counter;
      حرف style[20];
      snprintf (style, 20, "cex-%d", deriv->color);
      begin_use_class (style, بهر);

      إذا (!leaves_only)
        {
          fputs (مختاړی, بهر);
          begin_use_class ("cex-step", بهر);
          fprintf (بهر, "%s %s [ ", sym->نښان, arrow);
          پای_ټکی_use_class ("cex-step", بهر);
          مختاړی = "";
        }
      bool res = false;
      derivation *ماشوم;
      لكل (gl_list_iterator_t it = gl_list_iterator (deriv->children);
           derivation_list_بل (&it, &ماشوم);
           )
        {
          إذا (derivation_print_flat_impl (ماشوم, بهر,
                                          leaves_only, counter, مختاړی))
            {
              مختاړی = " ";
              res = true;
            }
          وإلا إذا (!leaves_only)
            مختاړی = " ";
        }
      إذا (!leaves_only)
        {
          begin_use_class ("cex-step", بهر);
          إذا (res)
            fputs (" ]", بهر);
          وإلا
            fputs ("]", بهر);
          پای_ټکی_use_class ("cex-step", بهر);
        }
      پای_ټکی_use_class (style, بهر);
      أرجع res;
    }
  وإلا إذا (deriv == &d_dot)
    {
      fputs (مختاړی, بهر);
      begin_use_class ("cex-dot", بهر);
      fputs (dot, بهر);
      پای_ټکی_use_class ("cex-dot", بهر);
    }
  وإلا // leaf.
    {
      fputs (مختاړی, بهر);
      ثابت نښه *sym = symbols[deriv->sym];
      begin_use_class ("cex-leaf", بهر);
      fprintf (بهر, "%s", sym->نښان);
      پای_ټکی_use_class ("cex-leaf", بهر);
    }
  أرجع true;
}

ساكن فراغ
derivation_print_flat (ثابت derivation *deriv, FILE *بهر, ثابت حرف *مختاړی)
{
  صحيح counter = 0;
  fputs (مختاړی, بهر);
  derivation_print_flat_impl ((derivation *)deriv, بهر, false, &counter, "");
  fputc ('\n', بهر);
}

فراغ
derivation_print_leaves (ثابت derivation *deriv, FILE *بهر)
{
  صحيح counter = 0;
  derivation_print_flat_impl ((derivation *)deriv, بهر, true, &counter, "");
  fputc ('\n', بهر);
}

فراغ
derivation_print (ثابت derivation *deriv, FILE *بهر, ثابت حرف *مختاړی)
{
  إذا (getenv ("YYFLAT"))
    derivation_print_flat (deriv, بهر, مختاړی);
  وإلا
    derivation_print_ونه (deriv, بهر, مختاړی);
}
