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
  هێما_ژمار sym;
  derivation_list children;
  صحيح reference_ژماردن;
  // The rule SYM -> CHILDREN.
  ثابت rule *rule;
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
  gl_list_add_last (dl, d);
}

فراغ
derivation_list_prepend (derivation_list dl, derivation *d)
{
  derivation_retain (d);
  gl_list_add_first (dl, d);
}

فراغ derivation_list_free (derivation_list dl)
{
  derivation *d = عدم;
  لكل (gl_list_iterator_t it = gl_list_iterator (dl);
       derivation_list_next (&it, &d);
       )
    إذا (d != &d_dot)
      derivation_free (d);
  gl_list_free (dl);
}

derivation *
derivation_new (هێما_ژمار sym, derivation_list children,
                ثابت rule *r)
{
  derivation *res = xmalloc (حجم *res);
  res->sym = sym;
  res->children = children;
  res->reference_ژماردن = 0;
  res->rule = r;
  res->color = -1;
  أرجع res;
}

فراغ
derivation_retain (derivation *d)
{
  ++d->reference_ژماردن;
}

فراغ
derivation_free (derivation *d)
{
  إذا (!d)
    أرجع;
  derivation_list free_queue =
    gl_list_create (GL_LINKED_LIST, عدم, عدم, عدم, true,
                    1, (ثابت فراغ **)&d);
  طالما (gl_list_size (free_queue) > 0)
    {
      derivation *deriv = (derivation *) gl_list_get_at (free_queue, 0);
      إذا (--deriv->reference_ژماردن == 0)
        {
          إذا (deriv->children)
            {
              derivation *child = عدم;
              لكل (gl_list_iterator_t it = gl_list_iterator (deriv->children);
                   derivation_list_next (&it, &child);
                   )
                إذا (child != &d_dot)
                  gl_list_add_last (free_queue, child);
              gl_list_free (deriv->children);
            }
          حرر (deriv);
        }
      gl_list_remove_at (free_queue, 0);
    }
  gl_list_free (free_queue);
}

size_t
derivation_size (ثابت derivation *deriv)
{
  إذا (!deriv->children)
    أرجع 1;
  صحيح size = 1;
  derivation *child = عدم;
  لكل (gl_list_iterator_t it = gl_list_iterator (deriv->children);
       derivation_list_next (&it, &child);
       )
    size += derivation_size (child);
  أرجع size;
}


// Longest distance from root to leaf.
ساكن صحيح
derivation_depth (ثابت derivation *deriv)
{
  إذا (deriv->children)
    {
      // Children's depth cannot be 0, even if there are no children
      // (the case of a derivation with an empty RHS).
      صحيح res = 1;
      derivation *child;
      لكل (gl_list_iterator_t it = gl_list_iterator (deriv->children);
           derivation_list_next (&it, &child);
           )
        res = max_int (res, derivation_depth (child));
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
fputs_if (bool cond, FILE *out, صحيح *padding, ثابت حرف *s)
{
  صحيح res = mbswidth (s, 0);
  إذا (cond && !all_spaces (s))
    {
      fprintf (out, "%*s%s", *padding, "", s);
      *padding = 0;
    }
  وإلا
    {
      *padding += res;
    }
  أرجع res;
}

ساكن صحيح
fprintf_if (bool cond, FILE *out, صحيح *padding, ثابت حرف *fmt, ...)
{
  حرف buf[256];
  size_t len = حجم (buf);
  va_list args;
  va_start (args, fmt);
  حرف *cp = vasnprintf (buf, &len, fmt, args);
  va_end (args);
  إذا (!cp)
    xalloc_die ();
  صحيح res = fputs_if (cond, out, padding, cp);
  إذا (cp != buf)
    حرر (cp);
  أرجع res;
}

// The width taken to report this derivation recursively down to its
// leaves.
ساكن صحيح
derivation_width (ثابت derivation *deriv)
{
  إذا (deriv->children)
    {
      ثابت هێما *sym = symbols[deriv->sym];
      صحيح self_width = mbswidth (sym->tag, 0);

      // Arrow and space.
      صحيح children_width = down_arrow_width;
      children_width += snprintf (عدم, 0, "%d: ", deriv->rule->ژمار);
      إذا (gl_list_size (deriv->children) == 0)
        // Empty rhs.
        children_width += empty_width;
      وإلا
        {
          إذا (gl_list_size (deriv->children) == 1
              && gl_list_get_first (deriv->children) == &d_dot)
            {
              children_width += empty_width;
              children_width += derivation_separator_width;
            }

          derivation *child;
          لكل (gl_list_iterator_t it = gl_list_iterator (deriv->children);
               derivation_list_next (&it, &child);
               )
            children_width
              += derivation_separator_width + derivation_width (child);
          // No separator at the beginning.
          children_width -= derivation_separator_width;
        }
      أرجع max_int (self_width, children_width);
    }
  وإلا إذا (deriv == &d_dot)
    {
      أرجع dot_width;
    }
  وإلا // leaf.
    {
      ثابت هێما *sym = symbols[deriv->sym];
      أرجع mbswidth (sym->tag, 0);
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
derivation_print_درەخت_impl (ثابت derivation *deriv, FILE *out,
                            صحيح depth, صحيح *padding)
{
  ثابت صحيح width = derivation_width (deriv);

  صحيح res = 0;
  إذا (deriv->children)
    {
      ثابت هێما *sym = symbols[deriv->sym];
      حرف style[20];
      snprintf (style, 20, "cex-%d", deriv->color);

      إذا (depth == 0 || depth == 1)
        {
          begin_use_class (style, out);
          begin_use_class ("cex-step", out);
        }
      إذا (depth == 0)
        {
          res += fputs_if (true, out, padding, sym->tag);
        }
      وإلا
        {
          res += fputs_if (depth == 1, out, padding, down_arrow);
          res += fprintf_if (depth == 1, out, padding, "%d: ", deriv->rule->ژمار);
          إذا (gl_list_size (deriv->children) == 0)
            // Empty rhs.
            res += fputs_if (depth == 1, out, padding, empty);
          وإلا
            {
              إذا (gl_list_size (deriv->children) == 1
                  && gl_list_get_first (deriv->children) == &d_dot)
                {
                  res += fputs_if (depth == 1, out, padding, empty);
                  res += fputs_if (depth == 1, out, padding, derivation_separator);
                }

              bool first = true;
              derivation *child;
              لكل (gl_list_iterator_t it = gl_list_iterator (deriv->children);
                   derivation_list_next (&it, &child);
                   )
                {
                  إذا (!first)
                    res += fputs_if (depth == 1, out, padding, derivation_separator);
                  res += derivation_print_درەخت_impl (child, out, depth - 1, padding);
                  first = false;
                }
            }
        }
      إذا (depth == 0 || depth == 1)
        {
          end_use_class ("cex-step", out);
          end_use_class (style, out);
        }
      *padding += width - res;
      res = width;
    }
  وإلا إذا (deriv == &d_dot)
    {
      إذا (depth == 0)
        begin_use_class ("cex-dot", out);
      res += fputs_if (depth == 0, out, padding, dot);
      إذا (depth == 0)
        end_use_class ("cex-dot", out);
    }
  وإلا // leaf.
    {
      ثابت هێما *sym = symbols[deriv->sym];
      إذا (depth == 0)
        begin_use_class ("cex-leaf", out);
      res += fputs_if (depth == 0, out, padding, sym->tag);
      إذا (depth == 0)
        end_use_class ("cex-leaf", out);
    }
  أرجع res;
}

ساكن فراغ
derivation_print_درەخت (ثابت derivation *deriv, FILE *out, ثابت حرف *prefix)
{
  fputc ('\n', out);
  لكل (صحيح depth = 0, max_depth = derivation_depth (deriv);
       depth < max_depth; ++depth)
    {
      صحيح padding = 0;
      fprintf (out, "    %s", prefix);
      derivation_print_درەخت_impl (deriv, out, depth, &padding);
      fputc ('\n', out);
    }
}


/* Print DERIV, colored according to COUNTER.
   Return false if nothing is printed.  */
ساكن bool
derivation_print_flat_impl (derivation *deriv, FILE *out,
                            bool leaves_only,
                            صحيح *counter, ثابت حرف *prefix)
{
  إذا (deriv->children)
    {
      ثابت هێما *sym = symbols[deriv->sym];
      deriv->color = *counter;
      ++*counter;
      حرف style[20];
      snprintf (style, 20, "cex-%d", deriv->color);
      begin_use_class (style, out);

      إذا (!leaves_only)
        {
          fputs (prefix, out);
          begin_use_class ("cex-step", out);
          fprintf (out, "%s %s [ ", sym->tag, arrow);
          end_use_class ("cex-step", out);
          prefix = "";
        }
      bool res = false;
      derivation *child;
      لكل (gl_list_iterator_t it = gl_list_iterator (deriv->children);
           derivation_list_next (&it, &child);
           )
        {
          إذا (derivation_print_flat_impl (child, out,
                                          leaves_only, counter, prefix))
            {
              prefix = " ";
              res = true;
            }
          وإلا إذا (!leaves_only)
            prefix = " ";
        }
      إذا (!leaves_only)
        {
          begin_use_class ("cex-step", out);
          إذا (res)
            fputs (" ]", out);
          وإلا
            fputs ("]", out);
          end_use_class ("cex-step", out);
        }
      end_use_class (style, out);
      أرجع res;
    }
  وإلا إذا (deriv == &d_dot)
    {
      fputs (prefix, out);
      begin_use_class ("cex-dot", out);
      fputs (dot, out);
      end_use_class ("cex-dot", out);
    }
  وإلا // leaf.
    {
      fputs (prefix, out);
      ثابت هێما *sym = symbols[deriv->sym];
      begin_use_class ("cex-leaf", out);
      fprintf (out, "%s", sym->tag);
      end_use_class ("cex-leaf", out);
    }
  أرجع true;
}

ساكن فراغ
derivation_print_flat (ثابت derivation *deriv, FILE *out, ثابت حرف *prefix)
{
  صحيح counter = 0;
  fputs (prefix, out);
  derivation_print_flat_impl ((derivation *)deriv, out, false, &counter, "");
  fputc ('\n', out);
}

فراغ
derivation_print_leaves (ثابت derivation *deriv, FILE *out)
{
  صحيح counter = 0;
  derivation_print_flat_impl ((derivation *)deriv, out, true, &counter, "");
  fputc ('\n', out);
}

فراغ
derivation_print (ثابت derivation *deriv, FILE *out, ثابت حرف *prefix)
{
  إذا (getenv ("YYFLAT"))
    derivation_print_flat (deriv, out, prefix);
  وإلا
    derivation_print_درەخت (deriv, out, prefix);
}
