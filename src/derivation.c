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

#अन्तर्भाव <config.h>

#अन्तर्भाव "derivation.h"
#अन्तर्भाव "glyphs.h"

#अन्तर्भाव <c-ctype.h>
#अन्तर्भाव <gl_linked_list.h>
#अन्तर्भाव <mbswidth.h>
#अन्तर्भाव <vasnprintf.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "complain.h"

संरचना derivation
{
  प्रतीक_संख्या sym;
  derivation_list children;
  पूर्णाङ्क reference_गिनती;
  // The rule SYM -> CHILDREN.
  ध्रुव rule *rule;
  // Color assigned for styling.  Guarantees that the derivation is
  // always displayed with the same color, independently of the order
  // in which the derivations are traversed.
  पूर्णाङ्क color;
};

स्थिर derivation d_dot = { -1, रिक्त, -1, रिक्त, -1 };

derivation *
derivation_dot (शून्य)
{
  प्रत्यागम &d_dot;
}

शून्य
derivation_list_append (derivation_list dl, derivation *d)
{
  derivation_retain (d);
  gl_list_add_last (dl, d);
}

शून्य
derivation_list_prepend (derivation_list dl, derivation *d)
{
  derivation_retain (d);
  gl_list_add_first (dl, d);
}

शून्य derivation_list_free (derivation_list dl)
{
  derivation *d = रिक्त;
  प्रति (gl_list_iterator_t it = gl_list_iterator (dl);
       derivation_list_next (&it, &d);
       )
    यदि (d != &d_dot)
      derivation_free (d);
  gl_list_free (dl);
}

derivation *
derivation_new (प्रतीक_संख्या sym, derivation_list children,
                ध्रुव rule *r)
{
  derivation *res = xmalloc (परिमाण *res);
  res->sym = sym;
  res->children = children;
  res->reference_गिनती = 0;
  res->rule = r;
  res->color = -1;
  प्रत्यागम res;
}

शून्य
derivation_retain (derivation *d)
{
  ++d->reference_गिनती;
}

शून्य
derivation_free (derivation *d)
{
  यदि (!d)
    प्रत्यागम;
  derivation_list free_queue =
    gl_list_create (GL_LINKED_LIST, रिक्त, रिक्त, रिक्त, true,
                    1, (ध्रुव शून्य **)&d);
  यावत् (gl_list_size (free_queue) > 0)
    {
      derivation *deriv = (derivation *) gl_list_get_at (free_queue, 0);
      यदि (--deriv->reference_गिनती == 0)
        {
          यदि (deriv->children)
            {
              derivation *child = रिक्त;
              प्रति (gl_list_iterator_t it = gl_list_iterator (deriv->children);
                   derivation_list_next (&it, &child);
                   )
                यदि (child != &d_dot)
                  gl_list_add_last (free_queue, child);
              gl_list_free (deriv->children);
            }
          मुक्त (deriv);
        }
      gl_list_remove_at (free_queue, 0);
    }
  gl_list_free (free_queue);
}

size_t
derivation_size (ध्रुव derivation *deriv)
{
  यदि (!deriv->children)
    प्रत्यागम 1;
  पूर्णाङ्क size = 1;
  derivation *child = रिक्त;
  प्रति (gl_list_iterator_t it = gl_list_iterator (deriv->children);
       derivation_list_next (&it, &child);
       )
    size += derivation_size (child);
  प्रत्यागम size;
}


// Longest distance from root to leaf.
स्थिर पूर्णाङ्क
derivation_depth (ध्रुव derivation *deriv)
{
  यदि (deriv->children)
    {
      // Children's depth cannot be 0, even if there are no children
      // (the case of a derivation with an empty RHS).
      पूर्णाङ्क res = 1;
      derivation *child;
      प्रति (gl_list_iterator_t it = gl_list_iterator (deriv->children);
           derivation_list_next (&it, &child);
           )
        res = max_int (res, derivation_depth (child));
      प्रत्यागम res + 1;
    }
  अन्यथा
    प्रत्यागम 1;
}

स्थिर bool
all_spaces (ध्रुव वर्ण *s)
{
  यावत् (c_isspace (*s))
    s++;
  प्रत्यागम *s == '\0';
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
स्थिर पूर्णाङ्क
fputs_if (bool cond, FILE *out, पूर्णाङ्क *padding, ध्रुव वर्ण *s)
{
  पूर्णाङ्क res = mbswidth (s, 0);
  यदि (cond && !all_spaces (s))
    {
      fprintf (out, "%*s%s", *padding, "", s);
      *padding = 0;
    }
  अन्यथा
    {
      *padding += res;
    }
  प्रत्यागम res;
}

स्थिर पूर्णाङ्क
fprintf_if (bool cond, FILE *out, पूर्णाङ्क *padding, ध्रुव वर्ण *fmt, ...)
{
  वर्ण buf[256];
  size_t len = परिमाण (buf);
  va_list args;
  va_start (args, fmt);
  वर्ण *cp = vasnprintf (buf, &len, fmt, args);
  va_end (args);
  यदि (!cp)
    xalloc_die ();
  पूर्णाङ्क res = fputs_if (cond, out, padding, cp);
  यदि (cp != buf)
    मुक्त (cp);
  प्रत्यागम res;
}

// The width taken to report this derivation recursively down to its
// leaves.
स्थिर पूर्णाङ्क
derivation_width (ध्रुव derivation *deriv)
{
  यदि (deriv->children)
    {
      ध्रुव प्रतीक *sym = symbols[deriv->sym];
      पूर्णाङ्क self_width = mbswidth (sym->tag, 0);

      // Arrow and space.
      पूर्णाङ्क children_width = down_arrow_width;
      children_width += snprintf (रिक्त, 0, "%d: ", deriv->rule->संख्या);
      यदि (gl_list_size (deriv->children) == 0)
        // Empty rhs.
        children_width += empty_width;
      अन्यथा
        {
          यदि (gl_list_size (deriv->children) == 1
              && gl_list_get_first (deriv->children) == &d_dot)
            {
              children_width += empty_width;
              children_width += derivation_separator_width;
            }

          derivation *child;
          प्रति (gl_list_iterator_t it = gl_list_iterator (deriv->children);
               derivation_list_next (&it, &child);
               )
            children_width
              += derivation_separator_width + derivation_width (child);
          // No separator at the beginning.
          children_width -= derivation_separator_width;
        }
      प्रत्यागम max_int (self_width, children_width);
    }
  अन्यथा यदि (deriv == &d_dot)
    {
      प्रत्यागम dot_width;
    }
  अन्यथा // leaf.
    {
      ध्रुव प्रतीक *sym = symbols[deriv->sym];
      प्रत्यागम mbswidth (sym->tag, 0);
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
स्थिर पूर्णाङ्क
derivation_print_वृक्ष_impl (ध्रुव derivation *deriv, FILE *out,
                            पूर्णाङ्क depth, पूर्णाङ्क *padding)
{
  ध्रुव पूर्णाङ्क width = derivation_width (deriv);

  पूर्णाङ्क res = 0;
  यदि (deriv->children)
    {
      ध्रुव प्रतीक *sym = symbols[deriv->sym];
      वर्ण style[20];
      snprintf (style, 20, "cex-%d", deriv->color);

      यदि (depth == 0 || depth == 1)
        {
          begin_use_class (style, out);
          begin_use_class ("cex-step", out);
        }
      यदि (depth == 0)
        {
          res += fputs_if (true, out, padding, sym->tag);
        }
      अन्यथा
        {
          res += fputs_if (depth == 1, out, padding, down_arrow);
          res += fprintf_if (depth == 1, out, padding, "%d: ", deriv->rule->संख्या);
          यदि (gl_list_size (deriv->children) == 0)
            // Empty rhs.
            res += fputs_if (depth == 1, out, padding, empty);
          अन्यथा
            {
              यदि (gl_list_size (deriv->children) == 1
                  && gl_list_get_first (deriv->children) == &d_dot)
                {
                  res += fputs_if (depth == 1, out, padding, empty);
                  res += fputs_if (depth == 1, out, padding, derivation_separator);
                }

              bool first = true;
              derivation *child;
              प्रति (gl_list_iterator_t it = gl_list_iterator (deriv->children);
                   derivation_list_next (&it, &child);
                   )
                {
                  यदि (!first)
                    res += fputs_if (depth == 1, out, padding, derivation_separator);
                  res += derivation_print_वृक्ष_impl (child, out, depth - 1, padding);
                  first = false;
                }
            }
        }
      यदि (depth == 0 || depth == 1)
        {
          end_use_class ("cex-step", out);
          end_use_class (style, out);
        }
      *padding += width - res;
      res = width;
    }
  अन्यथा यदि (deriv == &d_dot)
    {
      यदि (depth == 0)
        begin_use_class ("cex-dot", out);
      res += fputs_if (depth == 0, out, padding, dot);
      यदि (depth == 0)
        end_use_class ("cex-dot", out);
    }
  अन्यथा // leaf.
    {
      ध्रुव प्रतीक *sym = symbols[deriv->sym];
      यदि (depth == 0)
        begin_use_class ("cex-leaf", out);
      res += fputs_if (depth == 0, out, padding, sym->tag);
      यदि (depth == 0)
        end_use_class ("cex-leaf", out);
    }
  प्रत्यागम res;
}

स्थिर शून्य
derivation_print_वृक्ष (ध्रुव derivation *deriv, FILE *out, ध्रुव वर्ण *prefix)
{
  fputc ('\n', out);
  प्रति (पूर्णाङ्क depth = 0, max_depth = derivation_depth (deriv);
       depth < max_depth; ++depth)
    {
      पूर्णाङ्क padding = 0;
      fprintf (out, "    %s", prefix);
      derivation_print_वृक्ष_impl (deriv, out, depth, &padding);
      fputc ('\n', out);
    }
}


/* Print DERIV, colored according to COUNTER.
   Return false if nothing is printed.  */
स्थिर bool
derivation_print_flat_impl (derivation *deriv, FILE *out,
                            bool leaves_only,
                            पूर्णाङ्क *counter, ध्रुव वर्ण *prefix)
{
  यदि (deriv->children)
    {
      ध्रुव प्रतीक *sym = symbols[deriv->sym];
      deriv->color = *counter;
      ++*counter;
      वर्ण style[20];
      snprintf (style, 20, "cex-%d", deriv->color);
      begin_use_class (style, out);

      यदि (!leaves_only)
        {
          fputs (prefix, out);
          begin_use_class ("cex-step", out);
          fprintf (out, "%s %s [ ", sym->tag, arrow);
          end_use_class ("cex-step", out);
          prefix = "";
        }
      bool res = false;
      derivation *child;
      प्रति (gl_list_iterator_t it = gl_list_iterator (deriv->children);
           derivation_list_next (&it, &child);
           )
        {
          यदि (derivation_print_flat_impl (child, out,
                                          leaves_only, counter, prefix))
            {
              prefix = " ";
              res = true;
            }
          अन्यथा यदि (!leaves_only)
            prefix = " ";
        }
      यदि (!leaves_only)
        {
          begin_use_class ("cex-step", out);
          यदि (res)
            fputs (" ]", out);
          अन्यथा
            fputs ("]", out);
          end_use_class ("cex-step", out);
        }
      end_use_class (style, out);
      प्रत्यागम res;
    }
  अन्यथा यदि (deriv == &d_dot)
    {
      fputs (prefix, out);
      begin_use_class ("cex-dot", out);
      fputs (dot, out);
      end_use_class ("cex-dot", out);
    }
  अन्यथा // leaf.
    {
      fputs (prefix, out);
      ध्रुव प्रतीक *sym = symbols[deriv->sym];
      begin_use_class ("cex-leaf", out);
      fprintf (out, "%s", sym->tag);
      end_use_class ("cex-leaf", out);
    }
  प्रत्यागम true;
}

स्थिर शून्य
derivation_print_flat (ध्रुव derivation *deriv, FILE *out, ध्रुव वर्ण *prefix)
{
  पूर्णाङ्क counter = 0;
  fputs (prefix, out);
  derivation_print_flat_impl ((derivation *)deriv, out, false, &counter, "");
  fputc ('\n', out);
}

शून्य
derivation_print_leaves (ध्रुव derivation *deriv, FILE *out)
{
  पूर्णाङ्क counter = 0;
  derivation_print_flat_impl ((derivation *)deriv, out, true, &counter, "");
  fputc ('\n', out);
}

शून्य
derivation_print (ध्रुव derivation *deriv, FILE *out, ध्रुव वर्ण *prefix)
{
  यदि (getenv ("YYFLAT"))
    derivation_print_flat (deriv, out, prefix);
  अन्यथा
    derivation_print_वृक्ष (deriv, out, prefix);
}
