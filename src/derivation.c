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

#অন্তর্ভাব <config.h>

#অন্তর্ভাব "derivation.h"
#অন্তর্ভাব "glyphs.h"

#অন্তর্ভাব <c-ctype.h>
#অন্তর্ভাব <gl_linked_list.h>
#অন্তর্ভাব <mbswidth.h>
#অন্তর্ভাব <vasnprintf.h>

#অন্তর্ভাব "system.h"
#অন্তর্ভাব "complain.h"

কাঠামো derivation
{
  প্রতীক_সংখ্যা sym;
  derivation_list children;
  পূর্ণসংখ্যা reference_গুনতি;
  // The rule SYM -> CHILDREN.
  ধ্রুবক rule *rule;
  // Color assigned for styling.  Guarantees that the derivation is
  // always displayed with the same color, independently of the order
  // in which the derivations are traversed.
  পূর্ণসংখ্যা color;
};

স্থির derivation d_dot = { -1, ফাঁকা, -1, ফাঁকা, -1 };

derivation *
derivation_dot (শূন্য)
{
  ফেরত &d_dot;
}

শূন্য
derivation_list_append (derivation_list dl, derivation *d)
{
  derivation_retain (d);
  gl_list_add_last (dl, d);
}

শূন্য
derivation_list_prepend (derivation_list dl, derivation *d)
{
  derivation_retain (d);
  gl_list_add_first (dl, d);
}

শূন্য derivation_list_free (derivation_list dl)
{
  derivation *d = ফাঁকা;
  জন্য (gl_list_iterator_t it = gl_list_iterator (dl);
       derivation_list_পরবর্তী (&it, &d);
       )
    যদি (d != &d_dot)
      derivation_free (d);
  gl_list_free (dl);
}

derivation *
derivation_new (প্রতীক_সংখ্যা sym, derivation_list children,
                ধ্রুবক rule *r)
{
  derivation *res = xmalloc (আকার *res);
  res->sym = sym;
  res->children = children;
  res->reference_গুনতি = 0;
  res->rule = r;
  res->color = -1;
  ফেরত res;
}

শূন্য
derivation_retain (derivation *d)
{
  ++d->reference_গুনতি;
}

শূন্য
derivation_free (derivation *d)
{
  যদি (!d)
    ফেরত;
  derivation_list free_queue =
    gl_list_create (GL_LINKED_LIST, ফাঁকা, ফাঁকা, ফাঁকা, true,
                    1, (ধ্রুবক শূন্য **)&d);
  যখন (gl_list_আকার (free_queue) > 0)
    {
      derivation *deriv = (derivation *) gl_list_get_at (free_queue, 0);
      যদি (--deriv->reference_গুনতি == 0)
        {
          যদি (deriv->children)
            {
              derivation *child = ফাঁকা;
              জন্য (gl_list_iterator_t it = gl_list_iterator (deriv->children);
                   derivation_list_পরবর্তী (&it, &child);
                   )
                যদি (child != &d_dot)
                  gl_list_add_last (free_queue, child);
              gl_list_free (deriv->children);
            }
          মুক্ত (deriv);
        }
      gl_list_remove_at (free_queue, 0);
    }
  gl_list_free (free_queue);
}

size_t
derivation_আকার (ধ্রুবক derivation *deriv)
{
  যদি (!deriv->children)
    ফেরত 1;
  পূর্ণসংখ্যা আকার_2 = 1;
  derivation *child = ফাঁকা;
  জন্য (gl_list_iterator_t it = gl_list_iterator (deriv->children);
       derivation_list_পরবর্তী (&it, &child);
       )
    আকার_2 += derivation_আকার (child);
  ফেরত আকার_2;
}


// Longest distance from root to leaf.
স্থির পূর্ণসংখ্যা
derivation_depth (ধ্রুবক derivation *deriv)
{
  যদি (deriv->children)
    {
      // Children's depth cannot be 0, even if there are no children
      // (the case of a derivation with an empty RHS).
      পূর্ণসংখ্যা res = 1;
      derivation *child;
      জন্য (gl_list_iterator_t it = gl_list_iterator (deriv->children);
           derivation_list_পরবর্তী (&it, &child);
           )
        res = max_int (res, derivation_depth (child));
      ফেরত res + 1;
    }
  অন্যথা
    ফেরত 1;
}

স্থির bool
all_spaces (ধ্রুবক অক্ষর *s)
{
  যখন (c_isspace (*s))
    s++;
  ফেরত *s == '\0';
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
স্থির পূর্ণসংখ্যা
fputs_if (bool cond, FILE *out, পূর্ণসংখ্যা *padding, ধ্রুবক অক্ষর *s)
{
  পূর্ণসংখ্যা res = mbswidth (s, 0);
  যদি (cond && !all_spaces (s))
    {
      fprintf (out, "%*s%s", *padding, "", s);
      *padding = 0;
    }
  অন্যথা
    {
      *padding += res;
    }
  ফেরত res;
}

স্থির পূর্ণসংখ্যা
fprintf_if (bool cond, FILE *out, পূর্ণসংখ্যা *padding, ধ্রুবক অক্ষর *fmt, ...)
{
  অক্ষর buf[256];
  size_t len = আকার (buf);
  va_list args;
  va_start (args, fmt);
  অক্ষর *cp = vasnprintf (buf, &len, fmt, args);
  va_end (args);
  যদি (!cp)
    xalloc_die ();
  পূর্ণসংখ্যা res = fputs_if (cond, out, padding, cp);
  যদি (cp != buf)
    মুক্ত (cp);
  ফেরত res;
}

// The width taken to report this derivation recursively down to its
// leaves.
স্থির পূর্ণসংখ্যা
derivation_width (ধ্রুবক derivation *deriv)
{
  যদি (deriv->children)
    {
      ধ্রুবক প্রতীক *sym = symbols[deriv->sym];
      পূর্ণসংখ্যা self_width = mbswidth (sym->tag, 0);

      // Arrow and space.
      পূর্ণসংখ্যা children_width = down_arrow_width;
      children_width += snprintf (ফাঁকা, 0, "%d: ", deriv->rule->সংখ্যা);
      যদি (gl_list_আকার (deriv->children) == 0)
        // Empty rhs.
        children_width += empty_width;
      অন্যথা
        {
          যদি (gl_list_আকার (deriv->children) == 1
              && gl_list_get_first (deriv->children) == &d_dot)
            {
              children_width += empty_width;
              children_width += derivation_separator_width;
            }

          derivation *child;
          জন্য (gl_list_iterator_t it = gl_list_iterator (deriv->children);
               derivation_list_পরবর্তী (&it, &child);
               )
            children_width
              += derivation_separator_width + derivation_width (child);
          // No separator at the beginning.
          children_width -= derivation_separator_width;
        }
      ফেরত max_int (self_width, children_width);
    }
  অন্যথা যদি (deriv == &d_dot)
    {
      ফেরত dot_width;
    }
  অন্যথা // leaf.
    {
      ধ্রুবক প্রতীক *sym = symbols[deriv->sym];
      ফেরত mbswidth (sym->tag, 0);
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
স্থির পূর্ণসংখ্যা
derivation_print_বৃক্ষ_impl (ধ্রুবক derivation *deriv, FILE *out,
                            পূর্ণসংখ্যা depth, পূর্ণসংখ্যা *padding)
{
  ধ্রুবক পূর্ণসংখ্যা width = derivation_width (deriv);

  পূর্ণসংখ্যা res = 0;
  যদি (deriv->children)
    {
      ধ্রুবক প্রতীক *sym = symbols[deriv->sym];
      অক্ষর style[20];
      snprintf (style, 20, "cex-%d", deriv->color);

      যদি (depth == 0 || depth == 1)
        {
          begin_use_class (style, out);
          begin_use_class ("cex-step", out);
        }
      যদি (depth == 0)
        {
          res += fputs_if (true, out, padding, sym->tag);
        }
      অন্যথা
        {
          res += fputs_if (depth == 1, out, padding, down_arrow);
          res += fprintf_if (depth == 1, out, padding, "%d: ", deriv->rule->সংখ্যা);
          যদি (gl_list_আকার (deriv->children) == 0)
            // Empty rhs.
            res += fputs_if (depth == 1, out, padding, empty);
          অন্যথা
            {
              যদি (gl_list_আকার (deriv->children) == 1
                  && gl_list_get_first (deriv->children) == &d_dot)
                {
                  res += fputs_if (depth == 1, out, padding, empty);
                  res += fputs_if (depth == 1, out, padding, derivation_separator);
                }

              bool first = true;
              derivation *child;
              জন্য (gl_list_iterator_t it = gl_list_iterator (deriv->children);
                   derivation_list_পরবর্তী (&it, &child);
                   )
                {
                  যদি (!first)
                    res += fputs_if (depth == 1, out, padding, derivation_separator);
                  res += derivation_print_বৃক্ষ_impl (child, out, depth - 1, padding);
                  first = false;
                }
            }
        }
      যদি (depth == 0 || depth == 1)
        {
          end_use_class ("cex-step", out);
          end_use_class (style, out);
        }
      *padding += width - res;
      res = width;
    }
  অন্যথা যদি (deriv == &d_dot)
    {
      যদি (depth == 0)
        begin_use_class ("cex-dot", out);
      res += fputs_if (depth == 0, out, padding, dot);
      যদি (depth == 0)
        end_use_class ("cex-dot", out);
    }
  অন্যথা // leaf.
    {
      ধ্রুবক প্রতীক *sym = symbols[deriv->sym];
      যদি (depth == 0)
        begin_use_class ("cex-leaf", out);
      res += fputs_if (depth == 0, out, padding, sym->tag);
      যদি (depth == 0)
        end_use_class ("cex-leaf", out);
    }
  ফেরত res;
}

স্থির শূন্য
derivation_print_বৃক্ষ (ধ্রুবক derivation *deriv, FILE *out, ধ্রুবক অক্ষর *prefix)
{
  fputc ('\n', out);
  জন্য (পূর্ণসংখ্যা depth = 0, max_depth = derivation_depth (deriv);
       depth < max_depth; ++depth)
    {
      পূর্ণসংখ্যা padding = 0;
      fprintf (out, "    %s", prefix);
      derivation_print_বৃক্ষ_impl (deriv, out, depth, &padding);
      fputc ('\n', out);
    }
}


/* Print DERIV, colored according to COUNTER.
   Return false if nothing is printed.  */
স্থির bool
derivation_print_flat_impl (derivation *deriv, FILE *out,
                            bool leaves_only,
                            পূর্ণসংখ্যা *গণক, ধ্রুবক অক্ষর *prefix)
{
  যদি (deriv->children)
    {
      ধ্রুবক প্রতীক *sym = symbols[deriv->sym];
      deriv->color = *গণক;
      ++*গণক;
      অক্ষর style[20];
      snprintf (style, 20, "cex-%d", deriv->color);
      begin_use_class (style, out);

      যদি (!leaves_only)
        {
          fputs (prefix, out);
          begin_use_class ("cex-step", out);
          fprintf (out, "%s %s [ ", sym->tag, arrow);
          end_use_class ("cex-step", out);
          prefix = "";
        }
      bool res = false;
      derivation *child;
      জন্য (gl_list_iterator_t it = gl_list_iterator (deriv->children);
           derivation_list_পরবর্তী (&it, &child);
           )
        {
          যদি (derivation_print_flat_impl (child, out,
                                          leaves_only, গণক, prefix))
            {
              prefix = " ";
              res = true;
            }
          অন্যথা যদি (!leaves_only)
            prefix = " ";
        }
      যদি (!leaves_only)
        {
          begin_use_class ("cex-step", out);
          যদি (res)
            fputs (" ]", out);
          অন্যথা
            fputs ("]", out);
          end_use_class ("cex-step", out);
        }
      end_use_class (style, out);
      ফেরত res;
    }
  অন্যথা যদি (deriv == &d_dot)
    {
      fputs (prefix, out);
      begin_use_class ("cex-dot", out);
      fputs (dot, out);
      end_use_class ("cex-dot", out);
    }
  অন্যথা // leaf.
    {
      fputs (prefix, out);
      ধ্রুবক প্রতীক *sym = symbols[deriv->sym];
      begin_use_class ("cex-leaf", out);
      fprintf (out, "%s", sym->tag);
      end_use_class ("cex-leaf", out);
    }
  ফেরত true;
}

স্থির শূন্য
derivation_print_flat (ধ্রুবক derivation *deriv, FILE *out, ধ্রুবক অক্ষর *prefix)
{
  পূর্ণসংখ্যা গণক = 0;
  fputs (prefix, out);
  derivation_print_flat_impl ((derivation *)deriv, out, false, &গণক, "");
  fputc ('\n', out);
}

শূন্য
derivation_print_leaves (ধ্রুবক derivation *deriv, FILE *out)
{
  পূর্ণসংখ্যা গণক = 0;
  derivation_print_flat_impl ((derivation *)deriv, out, true, &গণক, "");
  fputc ('\n', out);
}

শূন্য
derivation_print (ধ্রুবক derivation *deriv, FILE *out, ধ্রুবক অক্ষর *prefix)
{
  যদি (getenv ("YYFLAT"))
    derivation_print_flat (deriv, out, prefix);
  অন্যথা
    derivation_print_বৃক্ষ (deriv, out, prefix);
}
