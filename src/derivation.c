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

#அந்தர்பாவ <config.h>

#அந்தர்பாவ "derivation.h"
#அந்தர்பாவ "glyphs.h"

#அந்தர்பாவ <c-ctype.h>
#அந்தர்பாவ <gl_linked_list.h>
#அந்தர்பாவ <mbswidth.h>
#அந்தர்பாவ <vasnprintf.h>

#அந்தர்பாவ "system.h"
#அந்தர்பாவ "complain.h"

கட்டமைப்பு derivation
{
  குறியீடு_எண் sym;
  derivation_list children;
  முழு reference_மொத்தம்;
  // The rule SYM -> CHILDREN.
  மாறிலி rule *rule;
  // Color assigned for styling.  Guarantees that the derivation is
  // always displayed with the same color, independently of the order
  // in which the derivations are traversed.
  முழு color;
};

நிலையான derivation d_dot = { -1, வெற்று, -1, வெற்று, -1 };

derivation *
derivation_dot (வெறுமை)
{
  திரும்பு &d_dot;
}

வெறுமை
derivation_list_append (derivation_list dl, derivation *d)
{
  derivation_retain (d);
  gl_list_add_last (dl, d);
}

வெறுமை
derivation_list_prepend (derivation_list dl, derivation *d)
{
  derivation_retain (d);
  gl_list_add_first (dl, d);
}

வெறுமை derivation_list_free (derivation_list dl)
{
  derivation *d = வெற்று;
  க்கு (gl_list_iterator_t it = gl_list_iterator (dl);
       derivation_list_next (&it, &d);
       )
    எனில் (d != &d_dot)
      derivation_free (d);
  gl_list_free (dl);
}

derivation *
derivation_new (குறியீடு_எண் sym, derivation_list children,
                மாறிலி rule *r)
{
  derivation *res = xmalloc (அளவு *res);
  res->sym = sym;
  res->children = children;
  res->reference_மொத்தம் = 0;
  res->rule = r;
  res->color = -1;
  திரும்பு res;
}

வெறுமை
derivation_retain (derivation *d)
{
  ++d->reference_மொத்தம்;
}

வெறுமை
derivation_free (derivation *d)
{
  எனில் (!d)
    திரும்பு;
  derivation_list free_queue =
    gl_list_create (GL_LINKED_LIST, வெற்று, வெற்று, வெற்று, true,
                    1, (மாறிலி வெறுமை **)&d);
  போது (gl_list_size (free_queue) > 0)
    {
      derivation *deriv = (derivation *) gl_list_get_at (free_queue, 0);
      எனில் (--deriv->reference_மொத்தம் == 0)
        {
          எனில் (deriv->children)
            {
              derivation *child = வெற்று;
              க்கு (gl_list_iterator_t it = gl_list_iterator (deriv->children);
                   derivation_list_next (&it, &child);
                   )
                எனில் (child != &d_dot)
                  gl_list_add_last (free_queue, child);
              gl_list_free (deriv->children);
            }
          விடுவி (deriv);
        }
      gl_list_remove_at (free_queue, 0);
    }
  gl_list_free (free_queue);
}

size_t
derivation_size (மாறிலி derivation *deriv)
{
  எனில் (!deriv->children)
    திரும்பு 1;
  முழு size = 1;
  derivation *child = வெற்று;
  க்கு (gl_list_iterator_t it = gl_list_iterator (deriv->children);
       derivation_list_next (&it, &child);
       )
    size += derivation_size (child);
  திரும்பு size;
}


// Longest distance from root to leaf.
நிலையான முழு
derivation_depth (மாறிலி derivation *deriv)
{
  எனில் (deriv->children)
    {
      // Children's depth cannot be 0, even if there are no children
      // (the case of a derivation with an empty RHS).
      முழு res = 1;
      derivation *child;
      க்கு (gl_list_iterator_t it = gl_list_iterator (deriv->children);
           derivation_list_next (&it, &child);
           )
        res = max_int (res, derivation_depth (child));
      திரும்பு res + 1;
    }
  இல்லையெனில்
    திரும்பு 1;
}

நிலையான bool
all_spaces (மாறிலி எழுத்து *s)
{
  போது (c_isspace (*s))
    s++;
  திரும்பு *s == '\0';
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
நிலையான முழு
fputs_if (bool cond, FILE *out, முழு *padding, மாறிலி எழுத்து *s)
{
  முழு res = mbswidth (s, 0);
  எனில் (cond && !all_spaces (s))
    {
      fprintf (out, "%*s%s", *padding, "", s);
      *padding = 0;
    }
  இல்லையெனில்
    {
      *padding += res;
    }
  திரும்பு res;
}

நிலையான முழு
fprintf_if (bool cond, FILE *out, முழு *padding, மாறிலி எழுத்து *fmt, ...)
{
  எழுத்து buf[256];
  size_t len = அளவு (buf);
  va_list args;
  va_start (args, fmt);
  எழுத்து *cp = vasnprintf (buf, &len, fmt, args);
  va_end (args);
  எனில் (!cp)
    xalloc_die ();
  முழு res = fputs_if (cond, out, padding, cp);
  எனில் (cp != buf)
    விடுவி (cp);
  திரும்பு res;
}

// The width taken to report this derivation recursively down to its
// leaves.
நிலையான முழு
derivation_width (மாறிலி derivation *deriv)
{
  எனில் (deriv->children)
    {
      மாறிலி குறியீடு *sym = symbols[deriv->sym];
      முழு self_width = mbswidth (sym->tag, 0);

      // Arrow and space.
      முழு children_width = down_arrow_width;
      children_width += snprintf (வெற்று, 0, "%d: ", deriv->rule->எண்);
      எனில் (gl_list_size (deriv->children) == 0)
        // Empty rhs.
        children_width += empty_width;
      இல்லையெனில்
        {
          எனில் (gl_list_size (deriv->children) == 1
              && gl_list_get_first (deriv->children) == &d_dot)
            {
              children_width += empty_width;
              children_width += derivation_separator_width;
            }

          derivation *child;
          க்கு (gl_list_iterator_t it = gl_list_iterator (deriv->children);
               derivation_list_next (&it, &child);
               )
            children_width
              += derivation_separator_width + derivation_width (child);
          // No separator at the beginning.
          children_width -= derivation_separator_width;
        }
      திரும்பு max_int (self_width, children_width);
    }
  இல்லையெனில் எனில் (deriv == &d_dot)
    {
      திரும்பு dot_width;
    }
  இல்லையெனில் // leaf.
    {
      மாறிலி குறியீடு *sym = symbols[deriv->sym];
      திரும்பு mbswidth (sym->tag, 0);
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
நிலையான முழு
derivation_print_மரம்_impl (மாறிலி derivation *deriv, FILE *out,
                            முழு depth, முழு *padding)
{
  மாறிலி முழு width = derivation_width (deriv);

  முழு res = 0;
  எனில் (deriv->children)
    {
      மாறிலி குறியீடு *sym = symbols[deriv->sym];
      எழுத்து style[20];
      snprintf (style, 20, "cex-%d", deriv->color);

      எனில் (depth == 0 || depth == 1)
        {
          begin_use_class (style, out);
          begin_use_class ("cex-step", out);
        }
      எனில் (depth == 0)
        {
          res += fputs_if (true, out, padding, sym->tag);
        }
      இல்லையெனில்
        {
          res += fputs_if (depth == 1, out, padding, down_arrow);
          res += fprintf_if (depth == 1, out, padding, "%d: ", deriv->rule->எண்);
          எனில் (gl_list_size (deriv->children) == 0)
            // Empty rhs.
            res += fputs_if (depth == 1, out, padding, empty);
          இல்லையெனில்
            {
              எனில் (gl_list_size (deriv->children) == 1
                  && gl_list_get_first (deriv->children) == &d_dot)
                {
                  res += fputs_if (depth == 1, out, padding, empty);
                  res += fputs_if (depth == 1, out, padding, derivation_separator);
                }

              bool first = true;
              derivation *child;
              க்கு (gl_list_iterator_t it = gl_list_iterator (deriv->children);
                   derivation_list_next (&it, &child);
                   )
                {
                  எனில் (!first)
                    res += fputs_if (depth == 1, out, padding, derivation_separator);
                  res += derivation_print_மரம்_impl (child, out, depth - 1, padding);
                  first = false;
                }
            }
        }
      எனில் (depth == 0 || depth == 1)
        {
          end_use_class ("cex-step", out);
          end_use_class (style, out);
        }
      *padding += width - res;
      res = width;
    }
  இல்லையெனில் எனில் (deriv == &d_dot)
    {
      எனில் (depth == 0)
        begin_use_class ("cex-dot", out);
      res += fputs_if (depth == 0, out, padding, dot);
      எனில் (depth == 0)
        end_use_class ("cex-dot", out);
    }
  இல்லையெனில் // leaf.
    {
      மாறிலி குறியீடு *sym = symbols[deriv->sym];
      எனில் (depth == 0)
        begin_use_class ("cex-leaf", out);
      res += fputs_if (depth == 0, out, padding, sym->tag);
      எனில் (depth == 0)
        end_use_class ("cex-leaf", out);
    }
  திரும்பு res;
}

நிலையான வெறுமை
derivation_print_மரம் (மாறிலி derivation *deriv, FILE *out, மாறிலி எழுத்து *prefix)
{
  fputc ('\n', out);
  க்கு (முழு depth = 0, max_depth = derivation_depth (deriv);
       depth < max_depth; ++depth)
    {
      முழு padding = 0;
      fprintf (out, "    %s", prefix);
      derivation_print_மரம்_impl (deriv, out, depth, &padding);
      fputc ('\n', out);
    }
}


/* Print DERIV, colored according to COUNTER.
   Return false if nothing is printed.  */
நிலையான bool
derivation_print_flat_impl (derivation *deriv, FILE *out,
                            bool leaves_only,
                            முழு *counter, மாறிலி எழுத்து *prefix)
{
  எனில் (deriv->children)
    {
      மாறிலி குறியீடு *sym = symbols[deriv->sym];
      deriv->color = *counter;
      ++*counter;
      எழுத்து style[20];
      snprintf (style, 20, "cex-%d", deriv->color);
      begin_use_class (style, out);

      எனில் (!leaves_only)
        {
          fputs (prefix, out);
          begin_use_class ("cex-step", out);
          fprintf (out, "%s %s [ ", sym->tag, arrow);
          end_use_class ("cex-step", out);
          prefix = "";
        }
      bool res = false;
      derivation *child;
      க்கு (gl_list_iterator_t it = gl_list_iterator (deriv->children);
           derivation_list_next (&it, &child);
           )
        {
          எனில் (derivation_print_flat_impl (child, out,
                                          leaves_only, counter, prefix))
            {
              prefix = " ";
              res = true;
            }
          இல்லையெனில் எனில் (!leaves_only)
            prefix = " ";
        }
      எனில் (!leaves_only)
        {
          begin_use_class ("cex-step", out);
          எனில் (res)
            fputs (" ]", out);
          இல்லையெனில்
            fputs ("]", out);
          end_use_class ("cex-step", out);
        }
      end_use_class (style, out);
      திரும்பு res;
    }
  இல்லையெனில் எனில் (deriv == &d_dot)
    {
      fputs (prefix, out);
      begin_use_class ("cex-dot", out);
      fputs (dot, out);
      end_use_class ("cex-dot", out);
    }
  இல்லையெனில் // leaf.
    {
      fputs (prefix, out);
      மாறிலி குறியீடு *sym = symbols[deriv->sym];
      begin_use_class ("cex-leaf", out);
      fprintf (out, "%s", sym->tag);
      end_use_class ("cex-leaf", out);
    }
  திரும்பு true;
}

நிலையான வெறுமை
derivation_print_flat (மாறிலி derivation *deriv, FILE *out, மாறிலி எழுத்து *prefix)
{
  முழு counter = 0;
  fputs (prefix, out);
  derivation_print_flat_impl ((derivation *)deriv, out, false, &counter, "");
  fputc ('\n', out);
}

வெறுமை
derivation_print_leaves (மாறிலி derivation *deriv, FILE *out)
{
  முழு counter = 0;
  derivation_print_flat_impl ((derivation *)deriv, out, true, &counter, "");
  fputc ('\n', out);
}

வெறுமை
derivation_print (மாறிலி derivation *deriv, FILE *out, மாறிலி எழுத்து *prefix)
{
  எனில் (getenv ("YYFLAT"))
    derivation_print_flat (deriv, out, prefix);
  இல்லையெனில்
    derivation_print_மரம் (deriv, out, prefix);
}
