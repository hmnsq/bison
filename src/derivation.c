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
  علامت_نمبر sym;
  derivation_list children;
  صحيح reference_گنتی;
  // The rule SYM -> CHILDREN.
  ثابت قاعدہ *قاعدہ;
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
  gl_list_جوڑو_آخری (dl, d);
}

فراغ
derivation_list_prepend (derivation_list dl, derivation *d)
{
  derivation_retain (d);
  gl_list_جوڑو_پہلا (dl, d);
}

فراغ derivation_list_free (derivation_list dl)
{
  derivation *d = عدم;
  لكل (gl_list_iterator_t it = gl_list_iterator (dl);
       derivation_list_اگلا (&it, &d);
       )
    إذا (d != &d_dot)
      derivation_free (d);
  gl_list_free (dl);
}

derivation *
derivation_new (علامت_نمبر sym, derivation_list children,
                ثابت قاعدہ *r)
{
  derivation *res = xmalloc (حجم *res);
  res->sym = sym;
  res->children = children;
  res->reference_گنتی = 0;
  res->قاعدہ = r;
  res->color = -1;
  أرجع res;
}

فراغ
derivation_retain (derivation *d)
{
  ++d->reference_گنتی;
}

فراغ
derivation_free (derivation *d)
{
  إذا (!d)
    أرجع;
  derivation_list free_قطار =
    gl_list_create (GL_LINKED_LIST, عدم, عدم, عدم, true,
                    1, (ثابت فراغ **)&d);
  طالما (gl_list_ناپ (free_قطار) > 0)
    {
      derivation *deriv = (derivation *) gl_list_get_at (free_قطار, 0);
      إذا (--deriv->reference_گنتی == 0)
        {
          إذا (deriv->children)
            {
              derivation *بچہ = عدم;
              لكل (gl_list_iterator_t it = gl_list_iterator (deriv->children);
                   derivation_list_اگلا (&it, &بچہ);
                   )
                إذا (بچہ != &d_dot)
                  gl_list_جوڑو_آخری (free_قطار, بچہ);
              gl_list_free (deriv->children);
            }
          حرر (deriv);
        }
      gl_list_remove_at (free_قطار, 0);
    }
  gl_list_free (free_قطار);
}

size_t
derivation_ناپ (ثابت derivation *deriv)
{
  إذا (!deriv->children)
    أرجع 1;
  صحيح ناپ = 1;
  derivation *بچہ = عدم;
  لكل (gl_list_iterator_t it = gl_list_iterator (deriv->children);
       derivation_list_اگلا (&it, &بچہ);
       )
    ناپ += derivation_ناپ (بچہ);
  أرجع ناپ;
}


// Longest distance from root to leaf.
ساكن صحيح
derivation_ڈونگھائی (ثابت derivation *deriv)
{
  إذا (deriv->children)
    {
      // Children's depth cannot be 0, even if there are no children
      // (the case of a derivation with an empty RHS).
      صحيح res = 1;
      derivation *بچہ;
      لكل (gl_list_iterator_t it = gl_list_iterator (deriv->children);
           derivation_list_اگلا (&it, &بچہ);
           )
        res = max_int (res, derivation_ڈونگھائی (بچہ));
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
fputs_if (bool cond, FILE *باہرلا, صحيح *padding, ثابت حرف *s)
{
  صحيح res = mbswidth (s, 0);
  إذا (cond && !all_spaces (s))
    {
      fprintf (باہرلا, "%*s%s", *padding, "", s);
      *padding = 0;
    }
  وإلا
    {
      *padding += res;
    }
  أرجع res;
}

ساكن صحيح
fprintf_if (bool cond, FILE *باہرلا, صحيح *padding, ثابت حرف *fmt, ...)
{
  حرف buf[256];
  size_t len = حجم (buf);
  va_list دلیلاں;
  va_شروع (دلیلاں, fmt);
  حرف *cp = vasnprintf (buf, &len, fmt, دلیلاں);
  va_سرا (دلیلاں);
  إذا (!cp)
    xalloc_die ();
  صحيح res = fputs_if (cond, باہرلا, padding, cp);
  إذا (cp != buf)
    حرر (cp);
  أرجع res;
}

// The width taken to report this derivation recursively down to its
// leaves.
ساكن صحيح
derivation_چوڑائی (ثابت derivation *deriv)
{
  إذا (deriv->children)
    {
      ثابت علامت *sym = symbols[deriv->sym];
      صحيح self_چوڑائی = mbswidth (sym->ٹیگ, 0);

      // Arrow and space.
      صحيح children_چوڑائی = تھلے_arrow_چوڑائی;
      children_چوڑائی += snprintf (عدم, 0, "%d: ", deriv->قاعدہ->نمبر);
      إذا (gl_list_ناپ (deriv->children) == 0)
        // Empty rhs.
        children_چوڑائی += خالی_تھاواں_چوڑائی;
      وإلا
        {
          إذا (gl_list_ناپ (deriv->children) == 1
              && gl_list_get_پہلا (deriv->children) == &d_dot)
            {
              children_چوڑائی += خالی_تھاواں_چوڑائی;
              children_چوڑائی += derivation_separator_چوڑائی;
            }

          derivation *بچہ;
          لكل (gl_list_iterator_t it = gl_list_iterator (deriv->children);
               derivation_list_اگلا (&it, &بچہ);
               )
            children_چوڑائی
              += derivation_separator_چوڑائی + derivation_چوڑائی (بچہ);
          // No separator at the beginning.
          children_چوڑائی -= derivation_separator_چوڑائی;
        }
      أرجع max_int (self_چوڑائی, children_چوڑائی);
    }
  وإلا إذا (deriv == &d_dot)
    {
      أرجع dot_چوڑائی;
    }
  وإلا // leaf.
    {
      ثابت علامت *sym = symbols[deriv->sym];
      أرجع mbswidth (sym->ٹیگ, 0);
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
derivation_print_درخت_impl (ثابت derivation *deriv, FILE *باہرلا,
                            صحيح ڈونگھائی, صحيح *padding)
{
  ثابت صحيح چوڑائی = derivation_چوڑائی (deriv);

  صحيح res = 0;
  إذا (deriv->children)
    {
      ثابت علامت *sym = symbols[deriv->sym];
      حرف style[20];
      snprintf (style, 20, "cex-%d", deriv->color);

      إذا (ڈونگھائی == 0 || ڈونگھائی == 1)
        {
          begin_use_class (style, باہرلا);
          begin_use_class ("cex-step", باہرلا);
        }
      إذا (ڈونگھائی == 0)
        {
          res += fputs_if (true, باہرلا, padding, sym->ٹیگ);
        }
      وإلا
        {
          res += fputs_if (ڈونگھائی == 1, باہرلا, padding, تھلے_arrow);
          res += fprintf_if (ڈونگھائی == 1, باہرلا, padding, "%d: ", deriv->قاعدہ->نمبر);
          إذا (gl_list_ناپ (deriv->children) == 0)
            // Empty rhs.
            res += fputs_if (ڈونگھائی == 1, باہرلا, padding, خالی_تھاواں);
          وإلا
            {
              إذا (gl_list_ناپ (deriv->children) == 1
                  && gl_list_get_پہلا (deriv->children) == &d_dot)
                {
                  res += fputs_if (ڈونگھائی == 1, باہرلا, padding, خالی_تھاواں);
                  res += fputs_if (ڈونگھائی == 1, باہرلا, padding, derivation_separator);
                }

              bool پہلا = true;
              derivation *بچہ;
              لكل (gl_list_iterator_t it = gl_list_iterator (deriv->children);
                   derivation_list_اگلا (&it, &بچہ);
                   )
                {
                  إذا (!پہلا)
                    res += fputs_if (ڈونگھائی == 1, باہرلا, padding, derivation_separator);
                  res += derivation_print_درخت_impl (بچہ, باہرلا, ڈونگھائی - 1, padding);
                  پہلا = false;
                }
            }
        }
      إذا (ڈونگھائی == 0 || ڈونگھائی == 1)
        {
          سرا_use_class ("cex-step", باہرلا);
          سرا_use_class (style, باہرلا);
        }
      *padding += چوڑائی - res;
      res = چوڑائی;
    }
  وإلا إذا (deriv == &d_dot)
    {
      إذا (ڈونگھائی == 0)
        begin_use_class ("cex-dot", باہرلا);
      res += fputs_if (ڈونگھائی == 0, باہرلا, padding, dot);
      إذا (ڈونگھائی == 0)
        سرا_use_class ("cex-dot", باہرلا);
    }
  وإلا // leaf.
    {
      ثابت علامت *sym = symbols[deriv->sym];
      إذا (ڈونگھائی == 0)
        begin_use_class ("cex-leaf", باہرلا);
      res += fputs_if (ڈونگھائی == 0, باہرلا, padding, sym->ٹیگ);
      إذا (ڈونگھائی == 0)
        سرا_use_class ("cex-leaf", باہرلا);
    }
  أرجع res;
}

ساكن فراغ
derivation_print_درخت (ثابت derivation *deriv, FILE *باہرلا, ثابت حرف *اگیتر)
{
  fputc ('\n', باہرلا);
  لكل (صحيح ڈونگھائی = 0, max_ڈونگھائی = derivation_ڈونگھائی (deriv);
       ڈونگھائی < max_ڈونگھائی; ++ڈونگھائی)
    {
      صحيح padding = 0;
      fprintf (باہرلا, "    %s", اگیتر);
      derivation_print_درخت_impl (deriv, باہرلا, ڈونگھائی, &padding);
      fputc ('\n', باہرلا);
    }
}


/* Print DERIV, colored according to COUNTER.
   Return false if nothing is printed.  */
ساكن bool
derivation_print_flat_impl (derivation *deriv, FILE *باہرلا,
                            bool leaves_only,
                            صحيح *counter, ثابت حرف *اگیتر)
{
  إذا (deriv->children)
    {
      ثابت علامت *sym = symbols[deriv->sym];
      deriv->color = *counter;
      ++*counter;
      حرف style[20];
      snprintf (style, 20, "cex-%d", deriv->color);
      begin_use_class (style, باہرلا);

      إذا (!leaves_only)
        {
          fputs (اگیتر, باہرلا);
          begin_use_class ("cex-step", باہرلا);
          fprintf (باہرلا, "%s %s [ ", sym->ٹیگ, arrow);
          سرا_use_class ("cex-step", باہرلا);
          اگیتر = "";
        }
      bool res = false;
      derivation *بچہ;
      لكل (gl_list_iterator_t it = gl_list_iterator (deriv->children);
           derivation_list_اگلا (&it, &بچہ);
           )
        {
          إذا (derivation_print_flat_impl (بچہ, باہرلا,
                                          leaves_only, counter, اگیتر))
            {
              اگیتر = " ";
              res = true;
            }
          وإلا إذا (!leaves_only)
            اگیتر = " ";
        }
      إذا (!leaves_only)
        {
          begin_use_class ("cex-step", باہرلا);
          إذا (res)
            fputs (" ]", باہرلا);
          وإلا
            fputs ("]", باہرلا);
          سرا_use_class ("cex-step", باہرلا);
        }
      سرا_use_class (style, باہرلا);
      أرجع res;
    }
  وإلا إذا (deriv == &d_dot)
    {
      fputs (اگیتر, باہرلا);
      begin_use_class ("cex-dot", باہرلا);
      fputs (dot, باہرلا);
      سرا_use_class ("cex-dot", باہرلا);
    }
  وإلا // leaf.
    {
      fputs (اگیتر, باہرلا);
      ثابت علامت *sym = symbols[deriv->sym];
      begin_use_class ("cex-leaf", باہرلا);
      fprintf (باہرلا, "%s", sym->ٹیگ);
      سرا_use_class ("cex-leaf", باہرلا);
    }
  أرجع true;
}

ساكن فراغ
derivation_print_flat (ثابت derivation *deriv, FILE *باہرلا, ثابت حرف *اگیتر)
{
  صحيح counter = 0;
  fputs (اگیتر, باہرلا);
  derivation_print_flat_impl ((derivation *)deriv, باہرلا, false, &counter, "");
  fputc ('\n', باہرلا);
}

فراغ
derivation_print_leaves (ثابت derivation *deriv, FILE *باہرلا)
{
  صحيح counter = 0;
  derivation_print_flat_impl ((derivation *)deriv, باہرلا, true, &counter, "");
  fputc ('\n', باہرلا);
}

فراغ
derivation_print (ثابت derivation *deriv, FILE *باہرلا, ثابت حرف *اگیتر)
{
  إذا (getenv ("YYFLAT"))
    derivation_print_flat (deriv, باہرلا, اگیتر);
  وإلا
    derivation_print_درخت (deriv, باہرلا, اگیتر);
}
