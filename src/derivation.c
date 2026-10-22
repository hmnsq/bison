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

#ಅನ್ತರ್ಭಾವ <config.h>

#ಅನ್ತರ್ಭಾವ "derivation.h"
#ಅನ್ತರ್ಭಾವ "glyphs.h"

#ಅನ್ತರ್ಭಾವ <c-ctype.h>
#ಅನ್ತರ್ಭಾವ <gl_linked_list.h>
#ಅನ್ತರ್ಭಾವ <mbswidth.h>
#ಅನ್ತರ್ಭಾವ <vasnprintf.h>

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "complain.h"

ಸಂರಚನಾ derivation
{
  ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym;
  derivation_list children;
  ಪೂರ್ಣಾಙ್ಕ reference_ಎಣಿಕೆ;
  // The rule SYM -> CHILDREN.
  ಧ್ರುವ rule *rule;
  // Color assigned for styling.  Guarantees that the derivation is
  // always displayed with the same color, independently of the order
  // in which the derivations are traversed.
  ಪೂರ್ಣಾಙ್ಕ color;
};

ಸ್ಥಿರ derivation d_dot = { -1, ರಿಕ್ತ, -1, ರಿಕ್ತ, -1 };

derivation *
derivation_dot (ಶೂನ್ಯ)
{
  ಪ್ರತ್ಯಾಗಮ &d_dot;
}

ಶೂನ್ಯ
derivation_list_append (derivation_list dl, derivation *d)
{
  derivation_retain (d);
  gl_list_add_last (dl, d);
}

ಶೂನ್ಯ
derivation_list_prepend (derivation_list dl, derivation *d)
{
  derivation_retain (d);
  gl_list_add_first (dl, d);
}

ಶೂನ್ಯ derivation_list_free (derivation_list dl)
{
  derivation *d = ರಿಕ್ತ;
  ಪ್ರತಿ (gl_list_iterator_t it = gl_list_iterator (dl);
       derivation_list_next (&it, &d);
       )
    ಯದಿ (d != &d_dot)
      derivation_free (d);
  gl_list_free (dl);
}

derivation *
derivation_new (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym, derivation_list children,
                ಧ್ರುವ rule *r)
{
  derivation *res = xmalloc (ಪರಿಮಾಣ *res);
  res->sym = sym;
  res->children = children;
  res->reference_ಎಣಿಕೆ = 0;
  res->rule = r;
  res->color = -1;
  ಪ್ರತ್ಯಾಗಮ res;
}

ಶೂನ್ಯ
derivation_retain (derivation *d)
{
  ++d->reference_ಎಣಿಕೆ;
}

ಶೂನ್ಯ
derivation_free (derivation *d)
{
  ಯದಿ (!d)
    ಪ್ರತ್ಯಾಗಮ;
  derivation_list free_queue =
    gl_list_create (GL_LINKED_LIST, ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ, true,
                    1, (ಧ್ರುವ ಶೂನ್ಯ **)&d);
  ಯಾವತ್ (gl_list_size (free_queue) > 0)
    {
      derivation *deriv = (derivation *) gl_list_get_at (free_queue, 0);
      ಯದಿ (--deriv->reference_ಎಣಿಕೆ == 0)
        {
          ಯದಿ (deriv->children)
            {
              derivation *child = ರಿಕ್ತ;
              ಪ್ರತಿ (gl_list_iterator_t it = gl_list_iterator (deriv->children);
                   derivation_list_next (&it, &child);
                   )
                ಯದಿ (child != &d_dot)
                  gl_list_add_last (free_queue, child);
              gl_list_free (deriv->children);
            }
          ಮುಕ್ತ (deriv);
        }
      gl_list_remove_at (free_queue, 0);
    }
  gl_list_free (free_queue);
}

size_t
derivation_size (ಧ್ರುವ derivation *deriv)
{
  ಯದಿ (!deriv->children)
    ಪ್ರತ್ಯಾಗಮ 1;
  ಪೂರ್ಣಾಙ್ಕ size = 1;
  derivation *child = ರಿಕ್ತ;
  ಪ್ರತಿ (gl_list_iterator_t it = gl_list_iterator (deriv->children);
       derivation_list_next (&it, &child);
       )
    size += derivation_size (child);
  ಪ್ರತ್ಯಾಗಮ size;
}


// Longest distance from root to leaf.
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
derivation_depth (ಧ್ರುವ derivation *deriv)
{
  ಯದಿ (deriv->children)
    {
      // Children's depth cannot be 0, even if there are no children
      // (the case of a derivation with an empty RHS).
      ಪೂರ್ಣಾಙ್ಕ res = 1;
      derivation *child;
      ಪ್ರತಿ (gl_list_iterator_t it = gl_list_iterator (deriv->children);
           derivation_list_next (&it, &child);
           )
        res = max_int (res, derivation_depth (child));
      ಪ್ರತ್ಯಾಗಮ res + 1;
    }
  ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ 1;
}

ಸ್ಥಿರ bool
all_spaces (ಧ್ರುವ ವರ್ಣ *s)
{
  ಯಾವತ್ (c_isspace (*s))
    s++;
  ಪ್ರತ್ಯಾಗಮ *s == '\0';
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
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
fputs_if (bool cond, FILE *out, ಪೂರ್ಣಾಙ್ಕ *padding, ಧ್ರುವ ವರ್ಣ *s)
{
  ಪೂರ್ಣಾಙ್ಕ res = mbswidth (s, 0);
  ಯದಿ (cond && !all_spaces (s))
    {
      fprintf (out, "%*s%s", *padding, "", s);
      *padding = 0;
    }
  ಅನ್ಯಥಾ
    {
      *padding += res;
    }
  ಪ್ರತ್ಯಾಗಮ res;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
fprintf_if (bool cond, FILE *out, ಪೂರ್ಣಾಙ್ಕ *padding, ಧ್ರುವ ವರ್ಣ *fmt, ...)
{
  ವರ್ಣ buf[256];
  size_t len = ಪರಿಮಾಣ (buf);
  va_list args;
  va_start (args, fmt);
  ವರ್ಣ *cp = vasnprintf (buf, &len, fmt, args);
  va_end (args);
  ಯದಿ (!cp)
    xalloc_die ();
  ಪೂರ್ಣಾಙ್ಕ res = fputs_if (cond, out, padding, cp);
  ಯದಿ (cp != buf)
    ಮುಕ್ತ (cp);
  ಪ್ರತ್ಯಾಗಮ res;
}

// The width taken to report this derivation recursively down to its
// leaves.
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
derivation_width (ಧ್ರುವ derivation *deriv)
{
  ಯದಿ (deriv->children)
    {
      ಧ್ರುವ ಚಿಹ್ನೆ *sym = symbols[deriv->sym];
      ಪೂರ್ಣಾಙ್ಕ self_width = mbswidth (sym->tag, 0);

      // Arrow and space.
      ಪೂರ್ಣಾಙ್ಕ children_width = down_arrow_width;
      children_width += snprintf (ರಿಕ್ತ, 0, "%d: ", deriv->rule->ಸಂಖ್ಯೆ);
      ಯದಿ (gl_list_size (deriv->children) == 0)
        // Empty rhs.
        children_width += empty_width;
      ಅನ್ಯಥಾ
        {
          ಯದಿ (gl_list_size (deriv->children) == 1
              && gl_list_get_first (deriv->children) == &d_dot)
            {
              children_width += empty_width;
              children_width += derivation_separator_width;
            }

          derivation *child;
          ಪ್ರತಿ (gl_list_iterator_t it = gl_list_iterator (deriv->children);
               derivation_list_next (&it, &child);
               )
            children_width
              += derivation_separator_width + derivation_width (child);
          // No separator at the beginning.
          children_width -= derivation_separator_width;
        }
      ಪ್ರತ್ಯಾಗಮ max_int (self_width, children_width);
    }
  ಅನ್ಯಥಾ ಯದಿ (deriv == &d_dot)
    {
      ಪ್ರತ್ಯಾಗಮ dot_width;
    }
  ಅನ್ಯಥಾ // leaf.
    {
      ಧ್ರುವ ಚಿಹ್ನೆ *sym = symbols[deriv->sym];
      ಪ್ರತ್ಯಾಗಮ mbswidth (sym->tag, 0);
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
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
derivation_print_ವೃಕ್ಷ_impl (ಧ್ರುವ derivation *deriv, FILE *out,
                            ಪೂರ್ಣಾಙ್ಕ depth, ಪೂರ್ಣಾಙ್ಕ *padding)
{
  ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ width = derivation_width (deriv);

  ಪೂರ್ಣಾಙ್ಕ res = 0;
  ಯದಿ (deriv->children)
    {
      ಧ್ರುವ ಚಿಹ್ನೆ *sym = symbols[deriv->sym];
      ವರ್ಣ style[20];
      snprintf (style, 20, "cex-%d", deriv->color);

      ಯದಿ (depth == 0 || depth == 1)
        {
          begin_use_class (style, out);
          begin_use_class ("cex-step", out);
        }
      ಯದಿ (depth == 0)
        {
          res += fputs_if (true, out, padding, sym->tag);
        }
      ಅನ್ಯಥಾ
        {
          res += fputs_if (depth == 1, out, padding, down_arrow);
          res += fprintf_if (depth == 1, out, padding, "%d: ", deriv->rule->ಸಂಖ್ಯೆ);
          ಯದಿ (gl_list_size (deriv->children) == 0)
            // Empty rhs.
            res += fputs_if (depth == 1, out, padding, empty);
          ಅನ್ಯಥಾ
            {
              ಯದಿ (gl_list_size (deriv->children) == 1
                  && gl_list_get_first (deriv->children) == &d_dot)
                {
                  res += fputs_if (depth == 1, out, padding, empty);
                  res += fputs_if (depth == 1, out, padding, derivation_separator);
                }

              bool first = true;
              derivation *child;
              ಪ್ರತಿ (gl_list_iterator_t it = gl_list_iterator (deriv->children);
                   derivation_list_next (&it, &child);
                   )
                {
                  ಯದಿ (!first)
                    res += fputs_if (depth == 1, out, padding, derivation_separator);
                  res += derivation_print_ವೃಕ್ಷ_impl (child, out, depth - 1, padding);
                  first = false;
                }
            }
        }
      ಯದಿ (depth == 0 || depth == 1)
        {
          end_use_class ("cex-step", out);
          end_use_class (style, out);
        }
      *padding += width - res;
      res = width;
    }
  ಅನ್ಯಥಾ ಯದಿ (deriv == &d_dot)
    {
      ಯದಿ (depth == 0)
        begin_use_class ("cex-dot", out);
      res += fputs_if (depth == 0, out, padding, dot);
      ಯದಿ (depth == 0)
        end_use_class ("cex-dot", out);
    }
  ಅನ್ಯಥಾ // leaf.
    {
      ಧ್ರುವ ಚಿಹ್ನೆ *sym = symbols[deriv->sym];
      ಯದಿ (depth == 0)
        begin_use_class ("cex-leaf", out);
      res += fputs_if (depth == 0, out, padding, sym->tag);
      ಯದಿ (depth == 0)
        end_use_class ("cex-leaf", out);
    }
  ಪ್ರತ್ಯಾಗಮ res;
}

ಸ್ಥಿರ ಶೂನ್ಯ
derivation_print_ವೃಕ್ಷ (ಧ್ರುವ derivation *deriv, FILE *out, ಧ್ರುವ ವರ್ಣ *prefix)
{
  fputc ('\n', out);
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ depth = 0, max_depth = derivation_depth (deriv);
       depth < max_depth; ++depth)
    {
      ಪೂರ್ಣಾಙ್ಕ padding = 0;
      fprintf (out, "    %s", prefix);
      derivation_print_ವೃಕ್ಷ_impl (deriv, out, depth, &padding);
      fputc ('\n', out);
    }
}


/* Print DERIV, colored according to COUNTER.
   Return false if nothing is printed.  */
ಸ್ಥಿರ bool
derivation_print_flat_impl (derivation *deriv, FILE *out,
                            bool leaves_only,
                            ಪೂರ್ಣಾಙ್ಕ *counter, ಧ್ರುವ ವರ್ಣ *prefix)
{
  ಯದಿ (deriv->children)
    {
      ಧ್ರುವ ಚಿಹ್ನೆ *sym = symbols[deriv->sym];
      deriv->color = *counter;
      ++*counter;
      ವರ್ಣ style[20];
      snprintf (style, 20, "cex-%d", deriv->color);
      begin_use_class (style, out);

      ಯದಿ (!leaves_only)
        {
          fputs (prefix, out);
          begin_use_class ("cex-step", out);
          fprintf (out, "%s %s [ ", sym->tag, arrow);
          end_use_class ("cex-step", out);
          prefix = "";
        }
      bool res = false;
      derivation *child;
      ಪ್ರತಿ (gl_list_iterator_t it = gl_list_iterator (deriv->children);
           derivation_list_next (&it, &child);
           )
        {
          ಯದಿ (derivation_print_flat_impl (child, out,
                                          leaves_only, counter, prefix))
            {
              prefix = " ";
              res = true;
            }
          ಅನ್ಯಥಾ ಯದಿ (!leaves_only)
            prefix = " ";
        }
      ಯದಿ (!leaves_only)
        {
          begin_use_class ("cex-step", out);
          ಯದಿ (res)
            fputs (" ]", out);
          ಅನ್ಯಥಾ
            fputs ("]", out);
          end_use_class ("cex-step", out);
        }
      end_use_class (style, out);
      ಪ್ರತ್ಯಾಗಮ res;
    }
  ಅನ್ಯಥಾ ಯದಿ (deriv == &d_dot)
    {
      fputs (prefix, out);
      begin_use_class ("cex-dot", out);
      fputs (dot, out);
      end_use_class ("cex-dot", out);
    }
  ಅನ್ಯಥಾ // leaf.
    {
      fputs (prefix, out);
      ಧ್ರುವ ಚಿಹ್ನೆ *sym = symbols[deriv->sym];
      begin_use_class ("cex-leaf", out);
      fprintf (out, "%s", sym->tag);
      end_use_class ("cex-leaf", out);
    }
  ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ ಶೂನ್ಯ
derivation_print_flat (ಧ್ರುವ derivation *deriv, FILE *out, ಧ್ರುವ ವರ್ಣ *prefix)
{
  ಪೂರ್ಣಾಙ್ಕ counter = 0;
  fputs (prefix, out);
  derivation_print_flat_impl ((derivation *)deriv, out, false, &counter, "");
  fputc ('\n', out);
}

ಶೂನ್ಯ
derivation_print_leaves (ಧ್ರುವ derivation *deriv, FILE *out)
{
  ಪೂರ್ಣಾಙ್ಕ counter = 0;
  derivation_print_flat_impl ((derivation *)deriv, out, true, &counter, "");
  fputc ('\n', out);
}

ಶೂನ್ಯ
derivation_print (ಧ್ರುವ derivation *deriv, FILE *out, ಧ್ರುವ ವರ್ಣ *prefix)
{
  ಯದಿ (getenv ("YYFLAT"))
    derivation_print_flat (deriv, out, prefix);
  ಅನ್ಯಥಾ
    derivation_print_ವೃಕ್ಷ (deriv, out, prefix);
}
