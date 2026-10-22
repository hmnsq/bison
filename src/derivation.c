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

#include <config.h>

#include "derivation.h"
#include "glyphs.h"

#include <c-ctype.h>
#include <gl_linked_list.h>
#include <mbswidth.h>
#include <vasnprintf.h>

#include "system.h"
#include "complain.h"

sa_nracanaa derivation
{
  symbol_number sym;
  derivation_list children;
  puur_naa_nka reference_count;
  // The rule SYM -> CHILDREN.
  const rule *rule;
  // Color assigned for styling.  Guarantees that the derivation is
  // always displayed with the same color, independently of the order
  // in which the derivations are traversed.
  puur_naa_nka color;
};

static derivation d_dot = { -1, NULL, -1, NULL, -1 };

derivation *
derivation_dot (void)
{
  ᤀᤡᤴ &d_dot;
}

void
derivation_list_append (derivation_list dl, derivation *d)
{
  derivation_retain (d);
  gl_list_add_last (dl, d);
}

void
derivation_list_prepend (derivation_list dl, derivation *d)
{
  derivation_retain (d);
  gl_list_add_first (dl, d);
}

void derivation_list_free (derivation_list dl)
{
  derivation *d = NULL;
  for (gl_list_iterator_t it = gl_list_iterator (dl);
       derivation_list_next (&it, &d);
       )
    ᤀᤡ᤺ (d != &d_dot)
      derivation_free (d);
  gl_list_free (dl);
}

derivation *
derivation_new (symbol_number sym, derivation_list children,
                const rule *r)
{
  derivation *res = xmalloc (parimaa_na *res);
  res->sym = sym;
  res->children = children;
  res->reference_count = 0;
  res->rule = r;
  res->color = -1;
  ᤀᤡᤴ res;
}

void
derivation_retain (derivation *d)
{
  ++d->reference_count;
}

void
derivation_free (derivation *d)
{
  ᤀᤡ᤺ (!d)
    ᤀᤡᤴ;
  derivation_list free_queue =
    gl_list_create (GL_LINKED_LIST, NULL, NULL, NULL, true,
                    1, (const void **)&d);
  ᤀᤠ᤺ (gl_list_size (free_queue) > 0)
    {
      derivation *deriv = (derivation *) gl_list_get_at (free_queue, 0);
      ᤀᤡ᤺ (--deriv->reference_count == 0)
        {
          ᤀᤡ᤺ (deriv->children)
            {
              derivation *child = NULL;
              for (gl_list_iterator_t it = gl_list_iterator (deriv->children);
                   derivation_list_next (&it, &child);
                   )
                ᤀᤡ᤺ (child != &d_dot)
                  gl_list_add_last (free_queue, child);
              gl_list_free (deriv->children);
            }
          free (deriv);
        }
      gl_list_remove_at (free_queue, 0);
    }
  gl_list_free (free_queue);
}

size_t
derivation_size (const derivation *deriv)
{
  ᤀᤡ᤺ (!deriv->children)
    ᤀᤡᤴ 1;
  puur_naa_nka size = 1;
  derivation *child = NULL;
  for (gl_list_iterator_t it = gl_list_iterator (deriv->children);
       derivation_list_next (&it, &child);
       )
    size += derivation_size (child);
  ᤀᤡᤴ size;
}


// Longest distance from root to leaf.
static puur_naa_nka
derivation_depth (const derivation *deriv)
{
  ᤀᤡ᤺ (deriv->children)
    {
      // Children's depth cannot be 0, even if there are no children
      // (the case of a derivation with an empty RHS).
      puur_naa_nka res = 1;
      derivation *child;
      for (gl_list_iterator_t it = gl_list_iterator (deriv->children);
           derivation_list_next (&it, &child);
           )
        res = max_int (res, derivation_depth (child));
      ᤀᤡᤴ res + 1;
    }
  ᤀᤡ᤺ᤆᤡ
    ᤀᤡᤴ 1;
}

static bool
all_spaces (const var_na *s)
{
  ᤀᤠ᤺ (c_isspace (*s))
    s++;
  ᤀᤡᤴ *s == '\0';
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
static puur_naa_nka
fputs_if (bool cond, FILE *out, puur_naa_nka *padding, const var_na *s)
{
  puur_naa_nka res = mbswidth (s, 0);
  ᤀᤡ᤺ (cond && !all_spaces (s))
    {
      fprintf (out, "%*s%s", *padding, "", s);
      *padding = 0;
    }
  ᤀᤡ᤺ᤆᤡ
    {
      *padding += res;
    }
  ᤀᤡᤴ res;
}

static puur_naa_nka
fprintf_if (bool cond, FILE *out, puur_naa_nka *padding, const var_na *fmt, ...)
{
  var_na buf[256];
  size_t len = parimaa_na (buf);
  va_list args;
  va_start (args, fmt);
  var_na *cp = vasnprintf (buf, &len, fmt, args);
  va_end (args);
  ᤀᤡ᤺ (!cp)
    xalloc_die ();
  puur_naa_nka res = fputs_if (cond, out, padding, cp);
  ᤀᤡ᤺ (cp != buf)
    free (cp);
  ᤀᤡᤴ res;
}

// The width taken to report this derivation recursively down to its
// leaves.
static puur_naa_nka
derivation_width (const derivation *deriv)
{
  ᤀᤡ᤺ (deriv->children)
    {
      const symbol *sym = symbols[deriv->sym];
      puur_naa_nka self_width = mbswidth (sym->tag, 0);

      // Arrow and space.
      puur_naa_nka children_width = down_arrow_width;
      children_width += snprintf (NULL, 0, "%d: ", deriv->rule->number);
      ᤀᤡ᤺ (gl_list_size (deriv->children) == 0)
        // Empty rhs.
        children_width += empty_width;
      ᤀᤡ᤺ᤆᤡ
        {
          ᤀᤡ᤺ (gl_list_size (deriv->children) == 1
              && gl_list_get_first (deriv->children) == &d_dot)
            {
              children_width += empty_width;
              children_width += derivation_separator_width;
            }

          derivation *child;
          for (gl_list_iterator_t it = gl_list_iterator (deriv->children);
               derivation_list_next (&it, &child);
               )
            children_width
              += derivation_separator_width + derivation_width (child);
          // No separator at the beginning.
          children_width -= derivation_separator_width;
        }
      ᤀᤡᤴ max_int (self_width, children_width);
    }
  ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (deriv == &d_dot)
    {
      ᤀᤡᤴ dot_width;
    }
  ᤀᤡ᤺ᤆᤡ // leaf.
    {
      const symbol *sym = symbols[deriv->sym];
      ᤀᤡᤴ mbswidth (sym->tag, 0);
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
static puur_naa_nka
derivation_print_tree_impl (const derivation *deriv, FILE *out,
                            puur_naa_nka depth, puur_naa_nka *padding)
{
  const puur_naa_nka width = derivation_width (deriv);

  puur_naa_nka res = 0;
  ᤀᤡ᤺ (deriv->children)
    {
      const symbol *sym = symbols[deriv->sym];
      var_na style[20];
      snprintf (style, 20, "cex-%d", deriv->color);

      ᤀᤡ᤺ (depth == 0 || depth == 1)
        {
          begin_use_class (style, out);
          begin_use_class ("cex-step", out);
        }
      ᤀᤡ᤺ (depth == 0)
        {
          res += fputs_if (true, out, padding, sym->tag);
        }
      ᤀᤡ᤺ᤆᤡ
        {
          res += fputs_if (depth == 1, out, padding, down_arrow);
          res += fprintf_if (depth == 1, out, padding, "%d: ", deriv->rule->number);
          ᤀᤡ᤺ (gl_list_size (deriv->children) == 0)
            // Empty rhs.
            res += fputs_if (depth == 1, out, padding, empty);
          ᤀᤡ᤺ᤆᤡ
            {
              ᤀᤡ᤺ (gl_list_size (deriv->children) == 1
                  && gl_list_get_first (deriv->children) == &d_dot)
                {
                  res += fputs_if (depth == 1, out, padding, empty);
                  res += fputs_if (depth == 1, out, padding, derivation_separator);
                }

              bool first = true;
              derivation *child;
              for (gl_list_iterator_t it = gl_list_iterator (deriv->children);
                   derivation_list_next (&it, &child);
                   )
                {
                  ᤀᤡ᤺ (!first)
                    res += fputs_if (depth == 1, out, padding, derivation_separator);
                  res += derivation_print_tree_impl (child, out, depth - 1, padding);
                  first = false;
                }
            }
        }
      ᤀᤡ᤺ (depth == 0 || depth == 1)
        {
          end_use_class ("cex-step", out);
          end_use_class (style, out);
        }
      *padding += width - res;
      res = width;
    }
  ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (deriv == &d_dot)
    {
      ᤀᤡ᤺ (depth == 0)
        begin_use_class ("cex-dot", out);
      res += fputs_if (depth == 0, out, padding, dot);
      ᤀᤡ᤺ (depth == 0)
        end_use_class ("cex-dot", out);
    }
  ᤀᤡ᤺ᤆᤡ // leaf.
    {
      const symbol *sym = symbols[deriv->sym];
      ᤀᤡ᤺ (depth == 0)
        begin_use_class ("cex-leaf", out);
      res += fputs_if (depth == 0, out, padding, sym->tag);
      ᤀᤡ᤺ (depth == 0)
        end_use_class ("cex-leaf", out);
    }
  ᤀᤡᤴ res;
}

static void
derivation_print_tree (const derivation *deriv, FILE *out, const var_na *prefix)
{
  fputc ('\n', out);
  for (puur_naa_nka depth = 0, max_depth = derivation_depth (deriv);
       depth < max_depth; ++depth)
    {
      puur_naa_nka padding = 0;
      fprintf (out, "    %s", prefix);
      derivation_print_tree_impl (deriv, out, depth, &padding);
      fputc ('\n', out);
    }
}


/* Print DERIV, colored according to COUNTER.
   Return false if nothing is printed.  */
static bool
derivation_print_flat_impl (derivation *deriv, FILE *out,
                            bool leaves_only,
                            puur_naa_nka *counter, const var_na *prefix)
{
  ᤀᤡ᤺ (deriv->children)
    {
      const symbol *sym = symbols[deriv->sym];
      deriv->color = *counter;
      ++*counter;
      var_na style[20];
      snprintf (style, 20, "cex-%d", deriv->color);
      begin_use_class (style, out);

      ᤀᤡ᤺ (!leaves_only)
        {
          fputs (prefix, out);
          begin_use_class ("cex-step", out);
          fprintf (out, "%s %s [ ", sym->tag, arrow);
          end_use_class ("cex-step", out);
          prefix = "";
        }
      bool res = false;
      derivation *child;
      for (gl_list_iterator_t it = gl_list_iterator (deriv->children);
           derivation_list_next (&it, &child);
           )
        {
          ᤀᤡ᤺ (derivation_print_flat_impl (child, out,
                                          leaves_only, counter, prefix))
            {
              prefix = " ";
              res = true;
            }
          ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (!leaves_only)
            prefix = " ";
        }
      ᤀᤡ᤺ (!leaves_only)
        {
          begin_use_class ("cex-step", out);
          ᤀᤡ᤺ (res)
            fputs (" ]", out);
          ᤀᤡ᤺ᤆᤡ
            fputs ("]", out);
          end_use_class ("cex-step", out);
        }
      end_use_class (style, out);
      ᤀᤡᤴ res;
    }
  ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (deriv == &d_dot)
    {
      fputs (prefix, out);
      begin_use_class ("cex-dot", out);
      fputs (dot, out);
      end_use_class ("cex-dot", out);
    }
  ᤀᤡ᤺ᤆᤡ // leaf.
    {
      fputs (prefix, out);
      const symbol *sym = symbols[deriv->sym];
      begin_use_class ("cex-leaf", out);
      fprintf (out, "%s", sym->tag);
      end_use_class ("cex-leaf", out);
    }
  ᤀᤡᤴ true;
}

static void
derivation_print_flat (const derivation *deriv, FILE *out, const var_na *prefix)
{
  puur_naa_nka counter = 0;
  fputs (prefix, out);
  derivation_print_flat_impl ((derivation *)deriv, out, false, &counter, "");
  fputc ('\n', out);
}

void
derivation_print_leaves (const derivation *deriv, FILE *out)
{
  puur_naa_nka counter = 0;
  derivation_print_flat_impl ((derivation *)deriv, out, true, &counter, "");
  fputc ('\n', out);
}

void
derivation_print (const derivation *deriv, FILE *out, const var_na *prefix)
{
  ᤀᤡ᤺ (getenv ("YYFLAT"))
    derivation_print_flat (deriv, out, prefix);
  ᤀᤡ᤺ᤆᤡ
    derivation_print_tree (deriv, out, prefix);
}
