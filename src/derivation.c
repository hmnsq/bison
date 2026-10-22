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

#ଅନ୍ତର୍ଭାଵ <config.h>

#ଅନ୍ତର୍ଭାଵ "derivation.h"
#ଅନ୍ତର୍ଭାଵ "glyphs.h"

#ଅନ୍ତର୍ଭାଵ <c-ctype.h>
#ଅନ୍ତର୍ଭାଵ <gl_linked_list.h>
#ଅନ୍ତର୍ଭାଵ <mbswidth.h>
#ଅନ୍ତର୍ଭାଵ <vasnprintf.h>

#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "complain.h"

ସଂରଚନା derivation
{
  ପ୍ରତୀକ_ସଂଖ୍ୟା sym;
  derivation_list children;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା reference_ଗଣନା;
  // The rule SYM -> CHILDREN.
  ଅଚଳ rule *rule;
  // Color assigned for styling.  Guarantees that the derivation is
  // always displayed with the same color, independently of the order
  // in which the derivations are traversed.
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା color;
};

ସ୍ଥିର derivation d_dot = { -1, ଖାଲି, -1, ଖାଲି, -1 };

derivation *
derivation_dot (ଶୂନ୍ୟ)
{
  ଫେରନ୍ତୁ &d_dot;
}

ଶୂନ୍ୟ
derivation_list_append (derivation_list dl, derivation *d)
{
  derivation_retain (d);
  gl_list_add_last (dl, d);
}

ଶୂନ୍ୟ
derivation_list_prepend (derivation_list dl, derivation *d)
{
  derivation_retain (d);
  gl_list_add_first (dl, d);
}

ଶୂନ୍ୟ derivation_list_free (derivation_list dl)
{
  derivation *d = ଖାଲି;
  ପାଇଁ (gl_list_iterator_t it = gl_list_iterator (dl);
       derivation_list_next (&it, &d);
       )
    ଯଦି (d != &d_dot)
      derivation_free (d);
  gl_list_free (dl);
}

derivation *
derivation_new (ପ୍ରତୀକ_ସଂଖ୍ୟା sym, derivation_list children,
                ଅଚଳ rule *r)
{
  derivation *res = xmalloc (ଆକାର *res);
  res->sym = sym;
  res->children = children;
  res->reference_ଗଣନା = 0;
  res->rule = r;
  res->color = -1;
  ଫେରନ୍ତୁ res;
}

ଶୂନ୍ୟ
derivation_retain (derivation *d)
{
  ++d->reference_ଗଣନା;
}

ଶୂନ୍ୟ
derivation_free (derivation *d)
{
  ଯଦି (!d)
    ଫେରନ୍ତୁ;
  derivation_list free_queue =
    gl_list_create (GL_LINKED_LIST, ଖାଲି, ଖାଲି, ଖାଲି, true,
                    1, (ଅଚଳ ଶୂନ୍ୟ **)&d);
  ଯେତେବେଳେ (gl_list_size (free_queue) > 0)
    {
      derivation *deriv = (derivation *) gl_list_get_at (free_queue, 0);
      ଯଦି (--deriv->reference_ଗଣନା == 0)
        {
          ଯଦି (deriv->children)
            {
              derivation *child = ଖାଲି;
              ପାଇଁ (gl_list_iterator_t it = gl_list_iterator (deriv->children);
                   derivation_list_next (&it, &child);
                   )
                ଯଦି (child != &d_dot)
                  gl_list_add_last (free_queue, child);
              gl_list_free (deriv->children);
            }
          ମୁକ୍ତ (deriv);
        }
      gl_list_remove_at (free_queue, 0);
    }
  gl_list_free (free_queue);
}

size_t
derivation_size (ଅଚଳ derivation *deriv)
{
  ଯଦି (!deriv->children)
    ଫେରନ୍ତୁ 1;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା size = 1;
  derivation *child = ଖାଲି;
  ପାଇଁ (gl_list_iterator_t it = gl_list_iterator (deriv->children);
       derivation_list_next (&it, &child);
       )
    size += derivation_size (child);
  ଫେରନ୍ତୁ size;
}


// Longest distance from root to leaf.
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
derivation_depth (ଅଚଳ derivation *deriv)
{
  ଯଦି (deriv->children)
    {
      // Children's depth cannot be 0, even if there are no children
      // (the case of a derivation with an empty RHS).
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା res = 1;
      derivation *child;
      ପାଇଁ (gl_list_iterator_t it = gl_list_iterator (deriv->children);
           derivation_list_next (&it, &child);
           )
        res = max_int (res, derivation_depth (child));
      ଫେରନ୍ତୁ res + 1;
    }
  ଅନ୍ୟଥା
    ଫେରନ୍ତୁ 1;
}

ସ୍ଥିର bool
all_spaces (ଅଚଳ ଅକ୍ଷର *s)
{
  ଯେତେବେଳେ (c_isspace (*s))
    s++;
  ଫେରନ୍ତୁ *s == '\0';
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
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
fputs_if (bool cond, FILE *out, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *padding, ଅଚଳ ଅକ୍ଷର *s)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା res = mbswidth (s, 0);
  ଯଦି (cond && !all_spaces (s))
    {
      fprintf (out, "%*s%s", *padding, "", s);
      *padding = 0;
    }
  ଅନ୍ୟଥା
    {
      *padding += res;
    }
  ଫେରନ୍ତୁ res;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
fprintf_if (bool cond, FILE *out, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *padding, ଅଚଳ ଅକ୍ଷର *fmt, ...)
{
  ଅକ୍ଷର buf[256];
  size_t len = ଆକାର (buf);
  va_list args;
  va_start (args, fmt);
  ଅକ୍ଷର *cp = vasnprintf (buf, &len, fmt, args);
  va_end (args);
  ଯଦି (!cp)
    xalloc_die ();
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା res = fputs_if (cond, out, padding, cp);
  ଯଦି (cp != buf)
    ମୁକ୍ତ (cp);
  ଫେରନ୍ତୁ res;
}

// The width taken to report this derivation recursively down to its
// leaves.
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
derivation_width (ଅଚଳ derivation *deriv)
{
  ଯଦି (deriv->children)
    {
      ଅଚଳ ପ୍ରତୀକ *sym = symbols[deriv->sym];
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା self_width = mbswidth (sym->tag, 0);

      // Arrow and space.
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା children_width = down_arrow_width;
      children_width += snprintf (ଖାଲି, 0, "%d: ", deriv->rule->ସଂଖ୍ୟା);
      ଯଦି (gl_list_size (deriv->children) == 0)
        // Empty rhs.
        children_width += empty_width;
      ଅନ୍ୟଥା
        {
          ଯଦି (gl_list_size (deriv->children) == 1
              && gl_list_get_first (deriv->children) == &d_dot)
            {
              children_width += empty_width;
              children_width += derivation_separator_width;
            }

          derivation *child;
          ପାଇଁ (gl_list_iterator_t it = gl_list_iterator (deriv->children);
               derivation_list_next (&it, &child);
               )
            children_width
              += derivation_separator_width + derivation_width (child);
          // No separator at the beginning.
          children_width -= derivation_separator_width;
        }
      ଫେରନ୍ତୁ max_int (self_width, children_width);
    }
  ଅନ୍ୟଥା ଯଦି (deriv == &d_dot)
    {
      ଫେରନ୍ତୁ dot_width;
    }
  ଅନ୍ୟଥା // leaf.
    {
      ଅଚଳ ପ୍ରତୀକ *sym = symbols[deriv->sym];
      ଫେରନ୍ତୁ mbswidth (sym->tag, 0);
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
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
derivation_print_ବୃକ୍ଷ_impl (ଅଚଳ derivation *deriv, FILE *out,
                            ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *padding)
{
  ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା width = derivation_width (deriv);

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା res = 0;
  ଯଦି (deriv->children)
    {
      ଅଚଳ ପ୍ରତୀକ *sym = symbols[deriv->sym];
      ଅକ୍ଷର style[20];
      snprintf (style, 20, "cex-%d", deriv->color);

      ଯଦି (depth == 0 || depth == 1)
        {
          begin_use_class (style, out);
          begin_use_class ("cex-step", out);
        }
      ଯଦି (depth == 0)
        {
          res += fputs_if (true, out, padding, sym->tag);
        }
      ଅନ୍ୟଥା
        {
          res += fputs_if (depth == 1, out, padding, down_arrow);
          res += fprintf_if (depth == 1, out, padding, "%d: ", deriv->rule->ସଂଖ୍ୟା);
          ଯଦି (gl_list_size (deriv->children) == 0)
            // Empty rhs.
            res += fputs_if (depth == 1, out, padding, empty);
          ଅନ୍ୟଥା
            {
              ଯଦି (gl_list_size (deriv->children) == 1
                  && gl_list_get_first (deriv->children) == &d_dot)
                {
                  res += fputs_if (depth == 1, out, padding, empty);
                  res += fputs_if (depth == 1, out, padding, derivation_separator);
                }

              bool first = true;
              derivation *child;
              ପାଇଁ (gl_list_iterator_t it = gl_list_iterator (deriv->children);
                   derivation_list_next (&it, &child);
                   )
                {
                  ଯଦି (!first)
                    res += fputs_if (depth == 1, out, padding, derivation_separator);
                  res += derivation_print_ବୃକ୍ଷ_impl (child, out, depth - 1, padding);
                  first = false;
                }
            }
        }
      ଯଦି (depth == 0 || depth == 1)
        {
          end_use_class ("cex-step", out);
          end_use_class (style, out);
        }
      *padding += width - res;
      res = width;
    }
  ଅନ୍ୟଥା ଯଦି (deriv == &d_dot)
    {
      ଯଦି (depth == 0)
        begin_use_class ("cex-dot", out);
      res += fputs_if (depth == 0, out, padding, dot);
      ଯଦି (depth == 0)
        end_use_class ("cex-dot", out);
    }
  ଅନ୍ୟଥା // leaf.
    {
      ଅଚଳ ପ୍ରତୀକ *sym = symbols[deriv->sym];
      ଯଦି (depth == 0)
        begin_use_class ("cex-leaf", out);
      res += fputs_if (depth == 0, out, padding, sym->tag);
      ଯଦି (depth == 0)
        end_use_class ("cex-leaf", out);
    }
  ଫେରନ୍ତୁ res;
}

ସ୍ଥିର ଶୂନ୍ୟ
derivation_print_ବୃକ୍ଷ (ଅଚଳ derivation *deriv, FILE *out, ଅଚଳ ଅକ୍ଷର *prefix)
{
  fputc ('\n', out);
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth = 0, max_depth = derivation_depth (deriv);
       depth < max_depth; ++depth)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା padding = 0;
      fprintf (out, "    %s", prefix);
      derivation_print_ବୃକ୍ଷ_impl (deriv, out, depth, &padding);
      fputc ('\n', out);
    }
}


/* Print DERIV, colored according to COUNTER.
   Return false if nothing is printed.  */
ସ୍ଥିର bool
derivation_print_flat_impl (derivation *deriv, FILE *out,
                            bool leaves_only,
                            ପୂର୍ଣ୍ଣସଂଖ୍ୟା *counter, ଅଚଳ ଅକ୍ଷର *prefix)
{
  ଯଦି (deriv->children)
    {
      ଅଚଳ ପ୍ରତୀକ *sym = symbols[deriv->sym];
      deriv->color = *counter;
      ++*counter;
      ଅକ୍ଷର style[20];
      snprintf (style, 20, "cex-%d", deriv->color);
      begin_use_class (style, out);

      ଯଦି (!leaves_only)
        {
          fputs (prefix, out);
          begin_use_class ("cex-step", out);
          fprintf (out, "%s %s [ ", sym->tag, arrow);
          end_use_class ("cex-step", out);
          prefix = "";
        }
      bool res = false;
      derivation *child;
      ପାଇଁ (gl_list_iterator_t it = gl_list_iterator (deriv->children);
           derivation_list_next (&it, &child);
           )
        {
          ଯଦି (derivation_print_flat_impl (child, out,
                                          leaves_only, counter, prefix))
            {
              prefix = " ";
              res = true;
            }
          ଅନ୍ୟଥା ଯଦି (!leaves_only)
            prefix = " ";
        }
      ଯଦି (!leaves_only)
        {
          begin_use_class ("cex-step", out);
          ଯଦି (res)
            fputs (" ]", out);
          ଅନ୍ୟଥା
            fputs ("]", out);
          end_use_class ("cex-step", out);
        }
      end_use_class (style, out);
      ଫେରନ୍ତୁ res;
    }
  ଅନ୍ୟଥା ଯଦି (deriv == &d_dot)
    {
      fputs (prefix, out);
      begin_use_class ("cex-dot", out);
      fputs (dot, out);
      end_use_class ("cex-dot", out);
    }
  ଅନ୍ୟଥା // leaf.
    {
      fputs (prefix, out);
      ଅଚଳ ପ୍ରତୀକ *sym = symbols[deriv->sym];
      begin_use_class ("cex-leaf", out);
      fprintf (out, "%s", sym->tag);
      end_use_class ("cex-leaf", out);
    }
  ଫେରନ୍ତୁ true;
}

ସ୍ଥିର ଶୂନ୍ୟ
derivation_print_flat (ଅଚଳ derivation *deriv, FILE *out, ଅଚଳ ଅକ୍ଷର *prefix)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା counter = 0;
  fputs (prefix, out);
  derivation_print_flat_impl ((derivation *)deriv, out, false, &counter, "");
  fputc ('\n', out);
}

ଶୂନ୍ୟ
derivation_print_leaves (ଅଚଳ derivation *deriv, FILE *out)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା counter = 0;
  derivation_print_flat_impl ((derivation *)deriv, out, true, &counter, "");
  fputc ('\n', out);
}

ଶୂନ୍ୟ
derivation_print (ଅଚଳ derivation *deriv, FILE *out, ଅଚଳ ଅକ୍ଷର *prefix)
{
  ଯଦି (getenv ("YYFLAT"))
    derivation_print_flat (deriv, out, prefix);
  ଅନ୍ୟଥା
    derivation_print_ବୃକ୍ଷ (deriv, out, prefix);
}
