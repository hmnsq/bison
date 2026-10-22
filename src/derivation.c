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

#અન્તર્ભાવ <config.h>

#અન્તર્ભાવ "derivation.h"
#અન્તર્ભાવ "glyphs.h"

#અન્તર્ભાવ <c-ctype.h>
#અન્તર્ભાવ <gl_linked_list.h>
#અન્તર્ભાવ <mbswidth.h>
#અન્તર્ભાવ <vasnprintf.h>

#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "complain.h"

સંરચના derivation
{
  પ્રતીક_સંખ્યા sym;
  derivation_list children;
  પૂર્ણાંક reference_ગણતરી;
  // The rule SYM -> CHILDREN.
  અચલ rule *rule;
  // Color assigned for styling.  Guarantees that the derivation is
  // always displayed with the same color, independently of the order
  // in which the derivations are traversed.
  પૂર્ણાંક color;
};

સ્થિર derivation d_dot = { -1, ખાલી, -1, ખાલી, -1 };

derivation *
derivation_dot (શૂન્ય)
{
  પાછા &d_dot;
}

શૂન્ય
derivation_list_append (derivation_list dl, derivation *d)
{
  derivation_retain (d);
  gl_list_add_last (dl, d);
}

શૂન્ય
derivation_list_prepend (derivation_list dl, derivation *d)
{
  derivation_retain (d);
  gl_list_add_first (dl, d);
}

શૂન્ય derivation_list_free (derivation_list dl)
{
  derivation *d = ખાલી;
  માટે (gl_list_iterator_t it = gl_list_iterator (dl);
       derivation_list_next (&it, &d);
       )
    જો (d != &d_dot)
      derivation_free (d);
  gl_list_free (dl);
}

derivation *
derivation_new (પ્રતીક_સંખ્યા sym, derivation_list children,
                અચલ rule *r)
{
  derivation *res = xmalloc (કદ *res);
  res->sym = sym;
  res->children = children;
  res->reference_ગણતરી = 0;
  res->rule = r;
  res->color = -1;
  પાછા res;
}

શૂન્ય
derivation_retain (derivation *d)
{
  ++d->reference_ગણતરી;
}

શૂન્ય
derivation_free (derivation *d)
{
  જો (!d)
    પાછા;
  derivation_list free_queue =
    gl_list_create (GL_LINKED_LIST, ખાલી, ખાલી, ખાલી, true,
                    1, (અચલ શૂન્ય **)&d);
  જ્યારે (gl_list_size (free_queue) > 0)
    {
      derivation *deriv = (derivation *) gl_list_get_at (free_queue, 0);
      જો (--deriv->reference_ગણતરી == 0)
        {
          જો (deriv->children)
            {
              derivation *child = ખાલી;
              માટે (gl_list_iterator_t it = gl_list_iterator (deriv->children);
                   derivation_list_next (&it, &child);
                   )
                જો (child != &d_dot)
                  gl_list_add_last (free_queue, child);
              gl_list_free (deriv->children);
            }
          મુક્ત (deriv);
        }
      gl_list_remove_at (free_queue, 0);
    }
  gl_list_free (free_queue);
}

size_t
derivation_size (અચલ derivation *deriv)
{
  જો (!deriv->children)
    પાછા 1;
  પૂર્ણાંક size = 1;
  derivation *child = ખાલી;
  માટે (gl_list_iterator_t it = gl_list_iterator (deriv->children);
       derivation_list_next (&it, &child);
       )
    size += derivation_size (child);
  પાછા size;
}


// Longest distance from root to leaf.
સ્થિર પૂર્ણાંક
derivation_depth (અચલ derivation *deriv)
{
  જો (deriv->children)
    {
      // Children's depth cannot be 0, even if there are no children
      // (the case of a derivation with an empty RHS).
      પૂર્ણાંક res = 1;
      derivation *child;
      માટે (gl_list_iterator_t it = gl_list_iterator (deriv->children);
           derivation_list_next (&it, &child);
           )
        res = max_int (res, derivation_depth (child));
      પાછા res + 1;
    }
  નહીંતર
    પાછા 1;
}

સ્થિર bool
all_spaces (અચલ અક્ષર *s)
{
  જ્યારે (c_isspace (*s))
    s++;
  પાછા *s == '\0';
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
સ્થિર પૂર્ણાંક
fputs_if (bool cond, FILE *out, પૂર્ણાંક *padding, અચલ અક્ષર *s)
{
  પૂર્ણાંક res = mbswidth (s, 0);
  જો (cond && !all_spaces (s))
    {
      fprintf (out, "%*s%s", *padding, "", s);
      *padding = 0;
    }
  નહીંતર
    {
      *padding += res;
    }
  પાછા res;
}

સ્થિર પૂર્ણાંક
fprintf_if (bool cond, FILE *out, પૂર્ણાંક *padding, અચલ અક્ષર *fmt, ...)
{
  અક્ષર buf[256];
  size_t len = કદ (buf);
  va_list args;
  va_start (args, fmt);
  અક્ષર *cp = vasnprintf (buf, &len, fmt, args);
  va_end (args);
  જો (!cp)
    xalloc_die ();
  પૂર્ણાંક res = fputs_if (cond, out, padding, cp);
  જો (cp != buf)
    મુક્ત (cp);
  પાછા res;
}

// The width taken to report this derivation recursively down to its
// leaves.
સ્થિર પૂર્ણાંક
derivation_width (અચલ derivation *deriv)
{
  જો (deriv->children)
    {
      અચલ પ્રતીક *sym = symbols[deriv->sym];
      પૂર્ણાંક self_width = mbswidth (sym->tag, 0);

      // Arrow and space.
      પૂર્ણાંક children_width = down_arrow_width;
      children_width += snprintf (ખાલી, 0, "%d: ", deriv->rule->સંખ્યા);
      જો (gl_list_size (deriv->children) == 0)
        // Empty rhs.
        children_width += empty_width;
      નહીંતર
        {
          જો (gl_list_size (deriv->children) == 1
              && gl_list_get_first (deriv->children) == &d_dot)
            {
              children_width += empty_width;
              children_width += derivation_separator_width;
            }

          derivation *child;
          માટે (gl_list_iterator_t it = gl_list_iterator (deriv->children);
               derivation_list_next (&it, &child);
               )
            children_width
              += derivation_separator_width + derivation_width (child);
          // No separator at the beginning.
          children_width -= derivation_separator_width;
        }
      પાછા max_int (self_width, children_width);
    }
  નહીંતર જો (deriv == &d_dot)
    {
      પાછા dot_width;
    }
  નહીંતર // leaf.
    {
      અચલ પ્રતીક *sym = symbols[deriv->sym];
      પાછા mbswidth (sym->tag, 0);
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
સ્થિર પૂર્ણાંક
derivation_print_વૃક્ષ_impl (અચલ derivation *deriv, FILE *out,
                            પૂર્ણાંક depth, પૂર્ણાંક *padding)
{
  અચલ પૂર્ણાંક width = derivation_width (deriv);

  પૂર્ણાંક res = 0;
  જો (deriv->children)
    {
      અચલ પ્રતીક *sym = symbols[deriv->sym];
      અક્ષર style[20];
      snprintf (style, 20, "cex-%d", deriv->color);

      જો (depth == 0 || depth == 1)
        {
          begin_use_class (style, out);
          begin_use_class ("cex-step", out);
        }
      જો (depth == 0)
        {
          res += fputs_if (true, out, padding, sym->tag);
        }
      નહીંતર
        {
          res += fputs_if (depth == 1, out, padding, down_arrow);
          res += fprintf_if (depth == 1, out, padding, "%d: ", deriv->rule->સંખ્યા);
          જો (gl_list_size (deriv->children) == 0)
            // Empty rhs.
            res += fputs_if (depth == 1, out, padding, empty);
          નહીંતર
            {
              જો (gl_list_size (deriv->children) == 1
                  && gl_list_get_first (deriv->children) == &d_dot)
                {
                  res += fputs_if (depth == 1, out, padding, empty);
                  res += fputs_if (depth == 1, out, padding, derivation_separator);
                }

              bool first = true;
              derivation *child;
              માટે (gl_list_iterator_t it = gl_list_iterator (deriv->children);
                   derivation_list_next (&it, &child);
                   )
                {
                  જો (!first)
                    res += fputs_if (depth == 1, out, padding, derivation_separator);
                  res += derivation_print_વૃક્ષ_impl (child, out, depth - 1, padding);
                  first = false;
                }
            }
        }
      જો (depth == 0 || depth == 1)
        {
          end_use_class ("cex-step", out);
          end_use_class (style, out);
        }
      *padding += width - res;
      res = width;
    }
  નહીંતર જો (deriv == &d_dot)
    {
      જો (depth == 0)
        begin_use_class ("cex-dot", out);
      res += fputs_if (depth == 0, out, padding, dot);
      જો (depth == 0)
        end_use_class ("cex-dot", out);
    }
  નહીંતર // leaf.
    {
      અચલ પ્રતીક *sym = symbols[deriv->sym];
      જો (depth == 0)
        begin_use_class ("cex-leaf", out);
      res += fputs_if (depth == 0, out, padding, sym->tag);
      જો (depth == 0)
        end_use_class ("cex-leaf", out);
    }
  પાછા res;
}

સ્થિર શૂન્ય
derivation_print_વૃક્ષ (અચલ derivation *deriv, FILE *out, અચલ અક્ષર *prefix)
{
  fputc ('\n', out);
  માટે (પૂર્ણાંક depth = 0, max_depth = derivation_depth (deriv);
       depth < max_depth; ++depth)
    {
      પૂર્ણાંક padding = 0;
      fprintf (out, "    %s", prefix);
      derivation_print_વૃક્ષ_impl (deriv, out, depth, &padding);
      fputc ('\n', out);
    }
}


/* Print DERIV, colored according to COUNTER.
   Return false if nothing is printed.  */
સ્થિર bool
derivation_print_flat_impl (derivation *deriv, FILE *out,
                            bool leaves_only,
                            પૂર્ણાંક *counter, અચલ અક્ષર *prefix)
{
  જો (deriv->children)
    {
      અચલ પ્રતીક *sym = symbols[deriv->sym];
      deriv->color = *counter;
      ++*counter;
      અક્ષર style[20];
      snprintf (style, 20, "cex-%d", deriv->color);
      begin_use_class (style, out);

      જો (!leaves_only)
        {
          fputs (prefix, out);
          begin_use_class ("cex-step", out);
          fprintf (out, "%s %s [ ", sym->tag, arrow);
          end_use_class ("cex-step", out);
          prefix = "";
        }
      bool res = false;
      derivation *child;
      માટે (gl_list_iterator_t it = gl_list_iterator (deriv->children);
           derivation_list_next (&it, &child);
           )
        {
          જો (derivation_print_flat_impl (child, out,
                                          leaves_only, counter, prefix))
            {
              prefix = " ";
              res = true;
            }
          નહીંતર જો (!leaves_only)
            prefix = " ";
        }
      જો (!leaves_only)
        {
          begin_use_class ("cex-step", out);
          જો (res)
            fputs (" ]", out);
          નહીંતર
            fputs ("]", out);
          end_use_class ("cex-step", out);
        }
      end_use_class (style, out);
      પાછા res;
    }
  નહીંતર જો (deriv == &d_dot)
    {
      fputs (prefix, out);
      begin_use_class ("cex-dot", out);
      fputs (dot, out);
      end_use_class ("cex-dot", out);
    }
  નહીંતર // leaf.
    {
      fputs (prefix, out);
      અચલ પ્રતીક *sym = symbols[deriv->sym];
      begin_use_class ("cex-leaf", out);
      fprintf (out, "%s", sym->tag);
      end_use_class ("cex-leaf", out);
    }
  પાછા true;
}

સ્થિર શૂન્ય
derivation_print_flat (અચલ derivation *deriv, FILE *out, અચલ અક્ષર *prefix)
{
  પૂર્ણાંક counter = 0;
  fputs (prefix, out);
  derivation_print_flat_impl ((derivation *)deriv, out, false, &counter, "");
  fputc ('\n', out);
}

શૂન્ય
derivation_print_leaves (અચલ derivation *deriv, FILE *out)
{
  પૂર્ણાંક counter = 0;
  derivation_print_flat_impl ((derivation *)deriv, out, true, &counter, "");
  fputc ('\n', out);
}

શૂન્ય
derivation_print (અચલ derivation *deriv, FILE *out, અચલ અક્ષર *prefix)
{
  જો (getenv ("YYFLAT"))
    derivation_print_flat (deriv, out, prefix);
  નહીંતર
    derivation_print_વૃક્ષ (deriv, out, prefix);
}
