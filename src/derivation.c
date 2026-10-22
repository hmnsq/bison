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

#അന്തര്ഭാവ <config.h>

#അന്തര്ഭാവ "derivation.h"
#അന്തര്ഭാവ "glyphs.h"

#അന്തര്ഭാവ <c-ctype.h>
#അന്തര്ഭാവ <gl_linked_list.h>
#അന്തര്ഭാവ <mbswidth.h>
#അന്തര്ഭാവ <vasnprintf.h>

#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "complain.h"

ഘടന derivation
{
  ചിഹ്നം_സംഖ്യ sym;
  derivation_list children;
  പൂർണ്ണസംഖ്യ reference_എണ്ണം;
  // The rule SYM -> CHILDREN.
  സ്ഥിരം rule *rule;
  // Color assigned for styling.  Guarantees that the derivation is
  // always displayed with the same color, independently of the order
  // in which the derivations are traversed.
  പൂർണ്ണസംഖ്യ color;
};

സ്ഥിതം derivation d_dot = { -1, ശൂന്യ, -1, ശൂന്യ, -1 };

derivation *
derivation_dot (ശൂന്യം)
{
  തിരികെ &d_dot;
}

ശൂന്യം
derivation_list_append (derivation_list dl, derivation *d)
{
  derivation_retain (d);
  gl_list_add_last (dl, d);
}

ശൂന്യം
derivation_list_prepend (derivation_list dl, derivation *d)
{
  derivation_retain (d);
  gl_list_add_first (dl, d);
}

ശൂന്യം derivation_list_free (derivation_list dl)
{
  derivation *d = ശൂന്യ;
  വേണ്ടി (gl_list_iterator_t it = gl_list_iterator (dl);
       derivation_list_next (&it, &d);
       )
    എങ്കിൽ (d != &d_dot)
      derivation_free (d);
  gl_list_free (dl);
}

derivation *
derivation_new (ചിഹ്നം_സംഖ്യ sym, derivation_list children,
                സ്ഥിരം rule *r)
{
  derivation *res = xmalloc (വലുപ്പം *res);
  res->sym = sym;
  res->children = children;
  res->reference_എണ്ണം = 0;
  res->rule = r;
  res->color = -1;
  തിരികെ res;
}

ശൂന്യം
derivation_retain (derivation *d)
{
  ++d->reference_എണ്ണം;
}

ശൂന്യം
derivation_free (derivation *d)
{
  എങ്കിൽ (!d)
    തിരികെ;
  derivation_list free_queue =
    gl_list_create (GL_LINKED_LIST, ശൂന്യ, ശൂന്യ, ശൂന്യ, true,
                    1, (സ്ഥിരം ശൂന്യം **)&d);
  ആകുമ്പോൾ (gl_list_size (free_queue) > 0)
    {
      derivation *deriv = (derivation *) gl_list_get_at (free_queue, 0);
      എങ്കിൽ (--deriv->reference_എണ്ണം == 0)
        {
          എങ്കിൽ (deriv->children)
            {
              derivation *child = ശൂന്യ;
              വേണ്ടി (gl_list_iterator_t it = gl_list_iterator (deriv->children);
                   derivation_list_next (&it, &child);
                   )
                എങ്കിൽ (child != &d_dot)
                  gl_list_add_last (free_queue, child);
              gl_list_free (deriv->children);
            }
          സ്വതന്ത്രം (deriv);
        }
      gl_list_remove_at (free_queue, 0);
    }
  gl_list_free (free_queue);
}

size_t
derivation_size (സ്ഥിരം derivation *deriv)
{
  എങ്കിൽ (!deriv->children)
    തിരികെ 1;
  പൂർണ്ണസംഖ്യ size = 1;
  derivation *child = ശൂന്യ;
  വേണ്ടി (gl_list_iterator_t it = gl_list_iterator (deriv->children);
       derivation_list_next (&it, &child);
       )
    size += derivation_size (child);
  തിരികെ size;
}


// Longest distance from root to leaf.
സ്ഥിതം പൂർണ്ണസംഖ്യ
derivation_depth (സ്ഥിരം derivation *deriv)
{
  എങ്കിൽ (deriv->children)
    {
      // Children's depth cannot be 0, even if there are no children
      // (the case of a derivation with an empty RHS).
      പൂർണ്ണസംഖ്യ res = 1;
      derivation *child;
      വേണ്ടി (gl_list_iterator_t it = gl_list_iterator (deriv->children);
           derivation_list_next (&it, &child);
           )
        res = max_int (res, derivation_depth (child));
      തിരികെ res + 1;
    }
  അല്ലെങ്കിൽ
    തിരികെ 1;
}

സ്ഥിതം bool
all_spaces (സ്ഥിരം അക്ഷരം *s)
{
  ആകുമ്പോൾ (c_isspace (*s))
    s++;
  തിരികെ *s == '\0';
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
സ്ഥിതം പൂർണ്ണസംഖ്യ
fputs_if (bool cond, FILE *out, പൂർണ്ണസംഖ്യ *padding, സ്ഥിരം അക്ഷരം *s)
{
  പൂർണ്ണസംഖ്യ res = mbswidth (s, 0);
  എങ്കിൽ (cond && !all_spaces (s))
    {
      fprintf (out, "%*s%s", *padding, "", s);
      *padding = 0;
    }
  അല്ലെങ്കിൽ
    {
      *padding += res;
    }
  തിരികെ res;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
fprintf_if (bool cond, FILE *out, പൂർണ്ണസംഖ്യ *padding, സ്ഥിരം അക്ഷരം *fmt, ...)
{
  അക്ഷരം buf[256];
  size_t len = വലുപ്പം (buf);
  va_list args;
  va_start (args, fmt);
  അക്ഷരം *cp = vasnprintf (buf, &len, fmt, args);
  va_end (args);
  എങ്കിൽ (!cp)
    xalloc_die ();
  പൂർണ്ണസംഖ്യ res = fputs_if (cond, out, padding, cp);
  എങ്കിൽ (cp != buf)
    സ്വതന്ത്രം (cp);
  തിരികെ res;
}

// The width taken to report this derivation recursively down to its
// leaves.
സ്ഥിതം പൂർണ്ണസംഖ്യ
derivation_width (സ്ഥിരം derivation *deriv)
{
  എങ്കിൽ (deriv->children)
    {
      സ്ഥിരം ചിഹ്നം *sym = symbols[deriv->sym];
      പൂർണ്ണസംഖ്യ self_width = mbswidth (sym->tag, 0);

      // Arrow and space.
      പൂർണ്ണസംഖ്യ children_width = down_arrow_width;
      children_width += snprintf (ശൂന്യ, 0, "%d: ", deriv->rule->സംഖ്യ);
      എങ്കിൽ (gl_list_size (deriv->children) == 0)
        // Empty rhs.
        children_width += empty_width;
      അല്ലെങ്കിൽ
        {
          എങ്കിൽ (gl_list_size (deriv->children) == 1
              && gl_list_get_first (deriv->children) == &d_dot)
            {
              children_width += empty_width;
              children_width += derivation_separator_width;
            }

          derivation *child;
          വേണ്ടി (gl_list_iterator_t it = gl_list_iterator (deriv->children);
               derivation_list_next (&it, &child);
               )
            children_width
              += derivation_separator_width + derivation_width (child);
          // No separator at the beginning.
          children_width -= derivation_separator_width;
        }
      തിരികെ max_int (self_width, children_width);
    }
  അല്ലെങ്കിൽ എങ്കിൽ (deriv == &d_dot)
    {
      തിരികെ dot_width;
    }
  അല്ലെങ്കിൽ // leaf.
    {
      സ്ഥിരം ചിഹ്നം *sym = symbols[deriv->sym];
      തിരികെ mbswidth (sym->tag, 0);
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
സ്ഥിതം പൂർണ്ണസംഖ്യ
derivation_print_വൃക്ഷം_impl (സ്ഥിരം derivation *deriv, FILE *out,
                            പൂർണ്ണസംഖ്യ depth, പൂർണ്ണസംഖ്യ *padding)
{
  സ്ഥിരം പൂർണ്ണസംഖ്യ width = derivation_width (deriv);

  പൂർണ്ണസംഖ്യ res = 0;
  എങ്കിൽ (deriv->children)
    {
      സ്ഥിരം ചിഹ്നം *sym = symbols[deriv->sym];
      അക്ഷരം style[20];
      snprintf (style, 20, "cex-%d", deriv->color);

      എങ്കിൽ (depth == 0 || depth == 1)
        {
          begin_use_class (style, out);
          begin_use_class ("cex-step", out);
        }
      എങ്കിൽ (depth == 0)
        {
          res += fputs_if (true, out, padding, sym->tag);
        }
      അല്ലെങ്കിൽ
        {
          res += fputs_if (depth == 1, out, padding, down_arrow);
          res += fprintf_if (depth == 1, out, padding, "%d: ", deriv->rule->സംഖ്യ);
          എങ്കിൽ (gl_list_size (deriv->children) == 0)
            // Empty rhs.
            res += fputs_if (depth == 1, out, padding, empty);
          അല്ലെങ്കിൽ
            {
              എങ്കിൽ (gl_list_size (deriv->children) == 1
                  && gl_list_get_first (deriv->children) == &d_dot)
                {
                  res += fputs_if (depth == 1, out, padding, empty);
                  res += fputs_if (depth == 1, out, padding, derivation_separator);
                }

              bool first = true;
              derivation *child;
              വേണ്ടി (gl_list_iterator_t it = gl_list_iterator (deriv->children);
                   derivation_list_next (&it, &child);
                   )
                {
                  എങ്കിൽ (!first)
                    res += fputs_if (depth == 1, out, padding, derivation_separator);
                  res += derivation_print_വൃക്ഷം_impl (child, out, depth - 1, padding);
                  first = false;
                }
            }
        }
      എങ്കിൽ (depth == 0 || depth == 1)
        {
          end_use_class ("cex-step", out);
          end_use_class (style, out);
        }
      *padding += width - res;
      res = width;
    }
  അല്ലെങ്കിൽ എങ്കിൽ (deriv == &d_dot)
    {
      എങ്കിൽ (depth == 0)
        begin_use_class ("cex-dot", out);
      res += fputs_if (depth == 0, out, padding, dot);
      എങ്കിൽ (depth == 0)
        end_use_class ("cex-dot", out);
    }
  അല്ലെങ്കിൽ // leaf.
    {
      സ്ഥിരം ചിഹ്നം *sym = symbols[deriv->sym];
      എങ്കിൽ (depth == 0)
        begin_use_class ("cex-leaf", out);
      res += fputs_if (depth == 0, out, padding, sym->tag);
      എങ്കിൽ (depth == 0)
        end_use_class ("cex-leaf", out);
    }
  തിരികെ res;
}

സ്ഥിതം ശൂന്യം
derivation_print_വൃക്ഷം (സ്ഥിരം derivation *deriv, FILE *out, സ്ഥിരം അക്ഷരം *prefix)
{
  fputc ('\n', out);
  വേണ്ടി (പൂർണ്ണസംഖ്യ depth = 0, max_depth = derivation_depth (deriv);
       depth < max_depth; ++depth)
    {
      പൂർണ്ണസംഖ്യ padding = 0;
      fprintf (out, "    %s", prefix);
      derivation_print_വൃക്ഷം_impl (deriv, out, depth, &padding);
      fputc ('\n', out);
    }
}


/* Print DERIV, colored according to COUNTER.
   Return false if nothing is printed.  */
സ്ഥിതം bool
derivation_print_flat_impl (derivation *deriv, FILE *out,
                            bool leaves_only,
                            പൂർണ്ണസംഖ്യ *counter, സ്ഥിരം അക്ഷരം *prefix)
{
  എങ്കിൽ (deriv->children)
    {
      സ്ഥിരം ചിഹ്നം *sym = symbols[deriv->sym];
      deriv->color = *counter;
      ++*counter;
      അക്ഷരം style[20];
      snprintf (style, 20, "cex-%d", deriv->color);
      begin_use_class (style, out);

      എങ്കിൽ (!leaves_only)
        {
          fputs (prefix, out);
          begin_use_class ("cex-step", out);
          fprintf (out, "%s %s [ ", sym->tag, arrow);
          end_use_class ("cex-step", out);
          prefix = "";
        }
      bool res = false;
      derivation *child;
      വേണ്ടി (gl_list_iterator_t it = gl_list_iterator (deriv->children);
           derivation_list_next (&it, &child);
           )
        {
          എങ്കിൽ (derivation_print_flat_impl (child, out,
                                          leaves_only, counter, prefix))
            {
              prefix = " ";
              res = true;
            }
          അല്ലെങ്കിൽ എങ്കിൽ (!leaves_only)
            prefix = " ";
        }
      എങ്കിൽ (!leaves_only)
        {
          begin_use_class ("cex-step", out);
          എങ്കിൽ (res)
            fputs (" ]", out);
          അല്ലെങ്കിൽ
            fputs ("]", out);
          end_use_class ("cex-step", out);
        }
      end_use_class (style, out);
      തിരികെ res;
    }
  അല്ലെങ്കിൽ എങ്കിൽ (deriv == &d_dot)
    {
      fputs (prefix, out);
      begin_use_class ("cex-dot", out);
      fputs (dot, out);
      end_use_class ("cex-dot", out);
    }
  അല്ലെങ്കിൽ // leaf.
    {
      fputs (prefix, out);
      സ്ഥിരം ചിഹ്നം *sym = symbols[deriv->sym];
      begin_use_class ("cex-leaf", out);
      fprintf (out, "%s", sym->tag);
      end_use_class ("cex-leaf", out);
    }
  തിരികെ true;
}

സ്ഥിതം ശൂന്യം
derivation_print_flat (സ്ഥിരം derivation *deriv, FILE *out, സ്ഥിരം അക്ഷരം *prefix)
{
  പൂർണ്ണസംഖ്യ counter = 0;
  fputs (prefix, out);
  derivation_print_flat_impl ((derivation *)deriv, out, false, &counter, "");
  fputc ('\n', out);
}

ശൂന്യം
derivation_print_leaves (സ്ഥിരം derivation *deriv, FILE *out)
{
  പൂർണ്ണസംഖ്യ counter = 0;
  derivation_print_flat_impl ((derivation *)deriv, out, true, &counter, "");
  fputc ('\n', out);
}

ശൂന്യം
derivation_print (സ്ഥിരം derivation *deriv, FILE *out, സ്ഥിരം അക്ഷരം *prefix)
{
  എങ്കിൽ (getenv ("YYFLAT"))
    derivation_print_flat (deriv, out, prefix);
  അല്ലെങ്കിൽ
    derivation_print_വൃക്ഷം (deriv, out, prefix);
}
