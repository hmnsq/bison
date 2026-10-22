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

struct derivation
{
  symbol_number sym;
  derivation_list children;
  정수 reference_count;
  // The rule SYM -> CHILDREN.
  const rule *rule;
  // Color assigned for styling.  Guarantees that the derivation is
  // always displayed with the same color, independently of the order
  // in which the derivations are traversed.
  정수 color;
};

static derivation d_dot = { -1, NULL, -1, NULL, -1 };

derivation *
derivation_dot (void)
{
  반환 &d_dot;
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
  위해 (gl_list_iterator_t it = gl_list_iterator (dl);
       derivation_list_next (&it, &d);
       )
    만약 (d != &d_dot)
      derivation_free (d);
  gl_list_free (dl);
}

derivation *
derivation_new (symbol_number sym, derivation_list children,
                const rule *r)
{
  derivation *res = xmalloc (sizeof *res);
  res->sym = sym;
  res->children = children;
  res->reference_count = 0;
  res->rule = r;
  res->color = -1;
  반환 res;
}

void
derivation_retain (derivation *d)
{
  ++d->reference_count;
}

void
derivation_free (derivation *d)
{
  만약 (!d)
    반환;
  derivation_list free_queue =
    gl_list_create (GL_LINKED_LIST, NULL, NULL, NULL, true,
                    1, (const void **)&d);
  동안 (gl_list_size (free_queue) > 0)
    {
      derivation *deriv = (derivation *) gl_list_get_at (free_queue, 0);
      만약 (--deriv->reference_count == 0)
        {
          만약 (deriv->children)
            {
              derivation *child = NULL;
              위해 (gl_list_iterator_t it = gl_list_iterator (deriv->children);
                   derivation_list_next (&it, &child);
                   )
                만약 (child != &d_dot)
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
  만약 (!deriv->children)
    반환 1;
  정수 size = 1;
  derivation *child = NULL;
  위해 (gl_list_iterator_t it = gl_list_iterator (deriv->children);
       derivation_list_next (&it, &child);
       )
    size += derivation_size (child);
  반환 size;
}


// Longest distance from root to leaf.
static 정수
derivation_depth (const derivation *deriv)
{
  만약 (deriv->children)
    {
      // Children's depth cannot be 0, even if there are no children
      // (the case of a derivation with an empty RHS).
      정수 res = 1;
      derivation *child;
      위해 (gl_list_iterator_t it = gl_list_iterator (deriv->children);
           derivation_list_next (&it, &child);
           )
        res = max_int (res, derivation_depth (child));
      반환 res + 1;
    }
  아니면
    반환 1;
}

static bool
all_spaces (const char *s)
{
  동안 (c_isspace (*s))
    s++;
  반환 *s == '\0';
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
static 정수
fputs_if (bool cond, FILE *out, 정수 *padding, const char *s)
{
  정수 res = mbswidth (s, 0);
  만약 (cond && !all_spaces (s))
    {
      fprintf (out, "%*s%s", *padding, "", s);
      *padding = 0;
    }
  아니면
    {
      *padding += res;
    }
  반환 res;
}

static 정수
fprintf_if (bool cond, FILE *out, 정수 *padding, const char *fmt, ...)
{
  char buf[256];
  size_t len = sizeof (buf);
  va_list args;
  va_start (args, fmt);
  char *cp = vasnprintf (buf, &len, fmt, args);
  va_end (args);
  만약 (!cp)
    xalloc_die ();
  정수 res = fputs_if (cond, out, padding, cp);
  만약 (cp != buf)
    free (cp);
  반환 res;
}

// The width taken to report this derivation recursively down to its
// leaves.
static 정수
derivation_width (const derivation *deriv)
{
  만약 (deriv->children)
    {
      const symbol *sym = symbols[deriv->sym];
      정수 self_width = mbswidth (sym->tag, 0);

      // Arrow and space.
      정수 children_width = down_arrow_width;
      children_width += snprintf (NULL, 0, "%d: ", deriv->rule->number);
      만약 (gl_list_size (deriv->children) == 0)
        // Empty rhs.
        children_width += empty_width;
      아니면
        {
          만약 (gl_list_size (deriv->children) == 1
              && gl_list_get_first (deriv->children) == &d_dot)
            {
              children_width += empty_width;
              children_width += derivation_separator_width;
            }

          derivation *child;
          위해 (gl_list_iterator_t it = gl_list_iterator (deriv->children);
               derivation_list_next (&it, &child);
               )
            children_width
              += derivation_separator_width + derivation_width (child);
          // No separator at the beginning.
          children_width -= derivation_separator_width;
        }
      반환 max_int (self_width, children_width);
    }
  아니면 만약 (deriv == &d_dot)
    {
      반환 dot_width;
    }
  아니면 // leaf.
    {
      const symbol *sym = symbols[deriv->sym];
      반환 mbswidth (sym->tag, 0);
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
static 정수
derivation_print_tree_impl (const derivation *deriv, FILE *out,
                            정수 depth, 정수 *padding)
{
  const 정수 width = derivation_width (deriv);

  정수 res = 0;
  만약 (deriv->children)
    {
      const symbol *sym = symbols[deriv->sym];
      char style[20];
      snprintf (style, 20, "cex-%d", deriv->color);

      만약 (depth == 0 || depth == 1)
        {
          begin_use_class (style, out);
          begin_use_class ("cex-step", out);
        }
      만약 (depth == 0)
        {
          res += fputs_if (true, out, padding, sym->tag);
        }
      아니면
        {
          res += fputs_if (depth == 1, out, padding, down_arrow);
          res += fprintf_if (depth == 1, out, padding, "%d: ", deriv->rule->number);
          만약 (gl_list_size (deriv->children) == 0)
            // Empty rhs.
            res += fputs_if (depth == 1, out, padding, empty);
          아니면
            {
              만약 (gl_list_size (deriv->children) == 1
                  && gl_list_get_first (deriv->children) == &d_dot)
                {
                  res += fputs_if (depth == 1, out, padding, empty);
                  res += fputs_if (depth == 1, out, padding, derivation_separator);
                }

              bool first = true;
              derivation *child;
              위해 (gl_list_iterator_t it = gl_list_iterator (deriv->children);
                   derivation_list_next (&it, &child);
                   )
                {
                  만약 (!first)
                    res += fputs_if (depth == 1, out, padding, derivation_separator);
                  res += derivation_print_tree_impl (child, out, depth - 1, padding);
                  first = false;
                }
            }
        }
      만약 (depth == 0 || depth == 1)
        {
          end_use_class ("cex-step", out);
          end_use_class (style, out);
        }
      *padding += width - res;
      res = width;
    }
  아니면 만약 (deriv == &d_dot)
    {
      만약 (depth == 0)
        begin_use_class ("cex-dot", out);
      res += fputs_if (depth == 0, out, padding, dot);
      만약 (depth == 0)
        end_use_class ("cex-dot", out);
    }
  아니면 // leaf.
    {
      const symbol *sym = symbols[deriv->sym];
      만약 (depth == 0)
        begin_use_class ("cex-leaf", out);
      res += fputs_if (depth == 0, out, padding, sym->tag);
      만약 (depth == 0)
        end_use_class ("cex-leaf", out);
    }
  반환 res;
}

static void
derivation_print_tree (const derivation *deriv, FILE *out, const char *prefix)
{
  fputc ('\n', out);
  위해 (정수 depth = 0, max_depth = derivation_depth (deriv);
       depth < max_depth; ++depth)
    {
      정수 padding = 0;
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
                            정수 *counter, const char *prefix)
{
  만약 (deriv->children)
    {
      const symbol *sym = symbols[deriv->sym];
      deriv->color = *counter;
      ++*counter;
      char style[20];
      snprintf (style, 20, "cex-%d", deriv->color);
      begin_use_class (style, out);

      만약 (!leaves_only)
        {
          fputs (prefix, out);
          begin_use_class ("cex-step", out);
          fprintf (out, "%s %s [ ", sym->tag, arrow);
          end_use_class ("cex-step", out);
          prefix = "";
        }
      bool res = false;
      derivation *child;
      위해 (gl_list_iterator_t it = gl_list_iterator (deriv->children);
           derivation_list_next (&it, &child);
           )
        {
          만약 (derivation_print_flat_impl (child, out,
                                          leaves_only, counter, prefix))
            {
              prefix = " ";
              res = true;
            }
          아니면 만약 (!leaves_only)
            prefix = " ";
        }
      만약 (!leaves_only)
        {
          begin_use_class ("cex-step", out);
          만약 (res)
            fputs (" ]", out);
          아니면
            fputs ("]", out);
          end_use_class ("cex-step", out);
        }
      end_use_class (style, out);
      반환 res;
    }
  아니면 만약 (deriv == &d_dot)
    {
      fputs (prefix, out);
      begin_use_class ("cex-dot", out);
      fputs (dot, out);
      end_use_class ("cex-dot", out);
    }
  아니면 // leaf.
    {
      fputs (prefix, out);
      const symbol *sym = symbols[deriv->sym];
      begin_use_class ("cex-leaf", out);
      fprintf (out, "%s", sym->tag);
      end_use_class ("cex-leaf", out);
    }
  반환 true;
}

static void
derivation_print_flat (const derivation *deriv, FILE *out, const char *prefix)
{
  정수 counter = 0;
  fputs (prefix, out);
  derivation_print_flat_impl ((derivation *)deriv, out, false, &counter, "");
  fputc ('\n', out);
}

void
derivation_print_leaves (const derivation *deriv, FILE *out)
{
  정수 counter = 0;
  derivation_print_flat_impl ((derivation *)deriv, out, true, &counter, "");
  fputc ('\n', out);
}

void
derivation_print (const derivation *deriv, FILE *out, const char *prefix)
{
  만약 (getenv ("YYFLAT"))
    derivation_print_flat (deriv, out, prefix);
  아니면
    derivation_print_tree (deriv, out, prefix);
}
