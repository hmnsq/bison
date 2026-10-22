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

ರಚನೆ derivation
{
  ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym;
  derivation_list children;
  ಪೂರ್ಣಾಂಕ reference_ಎಣಿಕೆ;
  // The rule SYM -> CHILDREN.
  ಸ್ಥಿರಾಂಕ ನಿಯಮ *ನಿಯಮ;
  // Color assigned for styling.  Guarantees that the derivation is
  // always displayed with the same color, independently of the order
  // in which the derivations are traversed.
  ಪೂರ್ಣಾಂಕ color;
};

ಸ್ಥಿರ derivation d_dot = { -1, ಖಾಲಿ, -1, ಖಾಲಿ, -1 };

derivation *
derivation_dot (ಶೂನ್ಯ)
{
  ಹಿಂತಿರುಗಿ &d_dot;
}

ಶೂನ್ಯ
derivation_list_append (derivation_list dl, derivation *d)
{
  derivation_retain (d);
  gl_list_ಕೂಡಿಸು_ಕೊನೆಯ (dl, d);
}

ಶೂನ್ಯ
derivation_list_prepend (derivation_list dl, derivation *d)
{
  derivation_retain (d);
  gl_list_ಕೂಡಿಸು_ಮೊದಲ (dl, d);
}

ಶೂನ್ಯ derivation_list_free (derivation_list dl)
{
  derivation *d = ಖಾಲಿ;
  ಗಾಗಿ (gl_list_iterator_t it = gl_list_iterator (dl);
       derivation_list_ಮುಂದಿನ (&it, &d);
       )
    ಆದರೆ (d != &d_dot)
      derivation_free (d);
  gl_list_free (dl);
}

derivation *
derivation_new (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym, derivation_list children,
                ಸ್ಥಿರಾಂಕ ನಿಯಮ *r)
{
  derivation *res = xmalloc (ಗಾತ್ರ *res);
  res->sym = sym;
  res->children = children;
  res->reference_ಎಣಿಕೆ = 0;
  res->ನಿಯಮ = r;
  res->color = -1;
  ಹಿಂತಿರುಗಿ res;
}

ಶೂನ್ಯ
derivation_retain (derivation *d)
{
  ++d->reference_ಎಣಿಕೆ;
}

ಶೂನ್ಯ
derivation_free (derivation *d)
{
  ಆದರೆ (!d)
    ಹಿಂತಿರುಗಿ;
  derivation_list free_ಸರತಿ =
    gl_list_create (GL_LINKED_LIST, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ, true,
                    1, (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ **)&d);
  ಆಗ (gl_list_ಗಾತ್ರ (free_ಸರತಿ) > 0)
    {
      derivation *deriv = (derivation *) gl_list_get_at (free_ಸರತಿ, 0);
      ಆದರೆ (--deriv->reference_ಎಣಿಕೆ == 0)
        {
          ಆದರೆ (deriv->children)
            {
              derivation *ಮಗು = ಖಾಲಿ;
              ಗಾಗಿ (gl_list_iterator_t it = gl_list_iterator (deriv->children);
                   derivation_list_ಮುಂದಿನ (&it, &ಮಗು);
                   )
                ಆದರೆ (ಮಗು != &d_dot)
                  gl_list_ಕೂಡಿಸು_ಕೊನೆಯ (free_ಸರತಿ, ಮಗು);
              gl_list_free (deriv->children);
            }
          ಬಿಡುಗಡೆ (deriv);
        }
      gl_list_remove_at (free_ಸರತಿ, 0);
    }
  gl_list_free (free_ಸರತಿ);
}

size_t
derivation_ಗಾತ್ರ (ಸ್ಥಿರಾಂಕ derivation *deriv)
{
  ಆದರೆ (!deriv->children)
    ಹಿಂತಿರುಗಿ 1;
  ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2 = 1;
  derivation *ಮಗು = ಖಾಲಿ;
  ಗಾಗಿ (gl_list_iterator_t it = gl_list_iterator (deriv->children);
       derivation_list_ಮುಂದಿನ (&it, &ಮಗು);
       )
    ಗಾತ್ರ_2 += derivation_ಗಾತ್ರ (ಮಗು);
  ಹಿಂತಿರುಗಿ ಗಾತ್ರ_2;
}


// Longest distance from root to leaf.
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
derivation_ಆಳ (ಸ್ಥಿರಾಂಕ derivation *deriv)
{
  ಆದರೆ (deriv->children)
    {
      // Children's depth cannot be 0, even if there are no children
      // (the case of a derivation with an empty RHS).
      ಪೂರ್ಣಾಂಕ res = 1;
      derivation *ಮಗು;
      ಗಾಗಿ (gl_list_iterator_t it = gl_list_iterator (deriv->children);
           derivation_list_ಮುಂದಿನ (&it, &ಮಗು);
           )
        res = max_int (res, derivation_ಆಳ (ಮಗು));
      ಹಿಂತಿರುಗಿ res + 1;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ bool
all_spaces (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s)
{
  ಆಗ (c_isspace (*s))
    s++;
  ಹಿಂತಿರುಗಿ *s == '\0';
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
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
fputs_if (bool cond, FILE *ಹೊರಹೋಗು, ಪೂರ್ಣಾಂಕ *padding, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s)
{
  ಪೂರ್ಣಾಂಕ res = mbswidth (s, 0);
  ಆದರೆ (cond && !all_spaces (s))
    {
      fprintf (ಹೊರಹೋಗು, "%*s%s", *padding, "", s);
      *padding = 0;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      *padding += res;
    }
  ಹಿಂತಿರುಗಿ res;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
fprintf_if (bool cond, FILE *ಹೊರಹೋಗು, ಪೂರ್ಣಾಂಕ *padding, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt, ...)
{
  ಅಕ್ಷರ buf[256];
  size_t len = ಗಾತ್ರ (buf);
  va_list ವಾದಗಳು;
  va_ಆರಂಭ (ವಾದಗಳು, fmt);
  ಅಕ್ಷರ *cp = vasnprintf (buf, &len, fmt, ವಾದಗಳು);
  va_ಕೊನೆ (ವಾದಗಳು);
  ಆದರೆ (!cp)
    xalloc_die ();
  ಪೂರ್ಣಾಂಕ res = fputs_if (cond, ಹೊರಹೋಗು, padding, cp);
  ಆದರೆ (cp != buf)
    ಬಿಡುಗಡೆ (cp);
  ಹಿಂತಿರುಗಿ res;
}

// The width taken to report this derivation recursively down to its
// leaves.
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
derivation_ಅಗಲ (ಸ್ಥಿರಾಂಕ derivation *deriv)
{
  ಆದರೆ (deriv->children)
    {
      ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆ *sym = symbols[deriv->sym];
      ಪೂರ್ಣಾಂಕ self_ಅಗಲ = mbswidth (sym->ಟ್ಯಾಗ್, 0);

      // Arrow and space.
      ಪೂರ್ಣಾಂಕ children_ಅಗಲ = ಕೆಳಕ್ಕೆ_arrow_ಅಗಲ;
      children_ಅಗಲ += snprintf (ಖಾಲಿ, 0, "%d: ", deriv->ನಿಯಮ->ಸಂಖ್ಯೆ);
      ಆದರೆ (gl_list_ಗಾತ್ರ (deriv->children) == 0)
        // Empty rhs.
        children_ಅಗಲ += ಖಾಲಿಗಳು_ಅಗಲ;
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಆದರೆ (gl_list_ಗಾತ್ರ (deriv->children) == 1
              && gl_list_get_ಮೊದಲ (deriv->children) == &d_dot)
            {
              children_ಅಗಲ += ಖಾಲಿಗಳು_ಅಗಲ;
              children_ಅಗಲ += derivation_separator_ಅಗಲ;
            }

          derivation *ಮಗು;
          ಗಾಗಿ (gl_list_iterator_t it = gl_list_iterator (deriv->children);
               derivation_list_ಮುಂದಿನ (&it, &ಮಗು);
               )
            children_ಅಗಲ
              += derivation_separator_ಅಗಲ + derivation_ಅಗಲ (ಮಗು);
          // No separator at the beginning.
          children_ಅಗಲ -= derivation_separator_ಅಗಲ;
        }
      ಹಿಂತಿರುಗಿ max_int (self_ಅಗಲ, children_ಅಗಲ);
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (deriv == &d_dot)
    {
      ಹಿಂತಿರುಗಿ dot_ಅಗಲ;
    }
  ಇಲ್ಲದಿದ್ದರೆ // leaf.
    {
      ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆ *sym = symbols[deriv->sym];
      ಹಿಂತಿರುಗಿ mbswidth (sym->ಟ್ಯಾಗ್, 0);
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
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
derivation_print_ವೃಕ್ಷ_impl (ಸ್ಥಿರಾಂಕ derivation *deriv, FILE *ಹೊರಹೋಗು,
                            ಪೂರ್ಣಾಂಕ ಆಳ, ಪೂರ್ಣಾಂಕ *padding)
{
  ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಅಗಲ = derivation_ಅಗಲ (deriv);

  ಪೂರ್ಣಾಂಕ res = 0;
  ಆದರೆ (deriv->children)
    {
      ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆ *sym = symbols[deriv->sym];
      ಅಕ್ಷರ style[20];
      snprintf (style, 20, "cex-%d", deriv->color);

      ಆದರೆ (ಆಳ == 0 || ಆಳ == 1)
        {
          begin_use_class (style, ಹೊರಹೋಗು);
          begin_use_class ("cex-step", ಹೊರಹೋಗು);
        }
      ಆದರೆ (ಆಳ == 0)
        {
          res += fputs_if (true, ಹೊರಹೋಗು, padding, sym->ಟ್ಯಾಗ್);
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          res += fputs_if (ಆಳ == 1, ಹೊರಹೋಗು, padding, ಕೆಳಕ್ಕೆ_arrow);
          res += fprintf_if (ಆಳ == 1, ಹೊರಹೋಗು, padding, "%d: ", deriv->ನಿಯಮ->ಸಂಖ್ಯೆ);
          ಆದರೆ (gl_list_ಗಾತ್ರ (deriv->children) == 0)
            // Empty rhs.
            res += fputs_if (ಆಳ == 1, ಹೊರಹೋಗು, padding, ಖಾಲಿಗಳು);
          ಇಲ್ಲದಿದ್ದರೆ
            {
              ಆದರೆ (gl_list_ಗಾತ್ರ (deriv->children) == 1
                  && gl_list_get_ಮೊದಲ (deriv->children) == &d_dot)
                {
                  res += fputs_if (ಆಳ == 1, ಹೊರಹೋಗು, padding, ಖಾಲಿಗಳು);
                  res += fputs_if (ಆಳ == 1, ಹೊರಹೋಗು, padding, derivation_separator);
                }

              bool ಮೊದಲ = true;
              derivation *ಮಗು;
              ಗಾಗಿ (gl_list_iterator_t it = gl_list_iterator (deriv->children);
                   derivation_list_ಮುಂದಿನ (&it, &ಮಗು);
                   )
                {
                  ಆದರೆ (!ಮೊದಲ)
                    res += fputs_if (ಆಳ == 1, ಹೊರಹೋಗು, padding, derivation_separator);
                  res += derivation_print_ವೃಕ್ಷ_impl (ಮಗು, ಹೊರಹೋಗು, ಆಳ - 1, padding);
                  ಮೊದಲ = false;
                }
            }
        }
      ಆದರೆ (ಆಳ == 0 || ಆಳ == 1)
        {
          ಕೊನೆ_use_class ("cex-step", ಹೊರಹೋಗು);
          ಕೊನೆ_use_class (style, ಹೊರಹೋಗು);
        }
      *padding += ಅಗಲ - res;
      res = ಅಗಲ;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (deriv == &d_dot)
    {
      ಆದರೆ (ಆಳ == 0)
        begin_use_class ("cex-dot", ಹೊರಹೋಗು);
      res += fputs_if (ಆಳ == 0, ಹೊರಹೋಗು, padding, dot);
      ಆದರೆ (ಆಳ == 0)
        ಕೊನೆ_use_class ("cex-dot", ಹೊರಹೋಗು);
    }
  ಇಲ್ಲದಿದ್ದರೆ // leaf.
    {
      ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆ *sym = symbols[deriv->sym];
      ಆದರೆ (ಆಳ == 0)
        begin_use_class ("cex-leaf", ಹೊರಹೋಗು);
      res += fputs_if (ಆಳ == 0, ಹೊರಹೋಗು, padding, sym->ಟ್ಯಾಗ್);
      ಆದರೆ (ಆಳ == 0)
        ಕೊನೆ_use_class ("cex-leaf", ಹೊರಹೋಗು);
    }
  ಹಿಂತಿರುಗಿ res;
}

ಸ್ಥಿರ ಶೂನ್ಯ
derivation_print_ವೃಕ್ಷ (ಸ್ಥಿರಾಂಕ derivation *deriv, FILE *ಹೊರಹೋಗು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ)
{
  fputc ('\n', ಹೊರಹೋಗು);
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ ಆಳ = 0, max_ಆಳ = derivation_ಆಳ (deriv);
       ಆಳ < max_ಆಳ; ++ಆಳ)
    {
      ಪೂರ್ಣಾಂಕ padding = 0;
      fprintf (ಹೊರಹೋಗು, "    %s", ಪೂರ್ವಪ್ರತ್ಯಯ);
      derivation_print_ವೃಕ್ಷ_impl (deriv, ಹೊರಹೋಗು, ಆಳ, &padding);
      fputc ('\n', ಹೊರಹೋಗು);
    }
}


/* Print DERIV, colored according to COUNTER.
   Return false if nothing is printed.  */
ಸ್ಥಿರ bool
derivation_print_flat_impl (derivation *deriv, FILE *ಹೊರಹೋಗು,
                            bool leaves_only,
                            ಪೂರ್ಣಾಂಕ *counter, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ)
{
  ಆದರೆ (deriv->children)
    {
      ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆ *sym = symbols[deriv->sym];
      deriv->color = *counter;
      ++*counter;
      ಅಕ್ಷರ style[20];
      snprintf (style, 20, "cex-%d", deriv->color);
      begin_use_class (style, ಹೊರಹೋಗು);

      ಆದರೆ (!leaves_only)
        {
          fputs (ಪೂರ್ವಪ್ರತ್ಯಯ, ಹೊರಹೋಗು);
          begin_use_class ("cex-step", ಹೊರಹೋಗು);
          fprintf (ಹೊರಹೋಗು, "%s %s [ ", sym->ಟ್ಯಾಗ್, arrow);
          ಕೊನೆ_use_class ("cex-step", ಹೊರಹೋಗು);
          ಪೂರ್ವಪ್ರತ್ಯಯ = "";
        }
      bool res = false;
      derivation *ಮಗು;
      ಗಾಗಿ (gl_list_iterator_t it = gl_list_iterator (deriv->children);
           derivation_list_ಮುಂದಿನ (&it, &ಮಗು);
           )
        {
          ಆದರೆ (derivation_print_flat_impl (ಮಗು, ಹೊರಹೋಗು,
                                          leaves_only, counter, ಪೂರ್ವಪ್ರತ್ಯಯ))
            {
              ಪೂರ್ವಪ್ರತ್ಯಯ = " ";
              res = true;
            }
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!leaves_only)
            ಪೂರ್ವಪ್ರತ್ಯಯ = " ";
        }
      ಆದರೆ (!leaves_only)
        {
          begin_use_class ("cex-step", ಹೊರಹೋಗು);
          ಆದರೆ (res)
            fputs (" ]", ಹೊರಹೋಗು);
          ಇಲ್ಲದಿದ್ದರೆ
            fputs ("]", ಹೊರಹೋಗು);
          ಕೊನೆ_use_class ("cex-step", ಹೊರಹೋಗು);
        }
      ಕೊನೆ_use_class (style, ಹೊರಹೋಗು);
      ಹಿಂತಿರುಗಿ res;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (deriv == &d_dot)
    {
      fputs (ಪೂರ್ವಪ್ರತ್ಯಯ, ಹೊರಹೋಗು);
      begin_use_class ("cex-dot", ಹೊರಹೋಗು);
      fputs (dot, ಹೊರಹೋಗು);
      ಕೊನೆ_use_class ("cex-dot", ಹೊರಹೋಗು);
    }
  ಇಲ್ಲದಿದ್ದರೆ // leaf.
    {
      fputs (ಪೂರ್ವಪ್ರತ್ಯಯ, ಹೊರಹೋಗು);
      ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆ *sym = symbols[deriv->sym];
      begin_use_class ("cex-leaf", ಹೊರಹೋಗು);
      fprintf (ಹೊರಹೋಗು, "%s", sym->ಟ್ಯಾಗ್);
      ಕೊನೆ_use_class ("cex-leaf", ಹೊರಹೋಗು);
    }
  ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ ಶೂನ್ಯ
derivation_print_flat (ಸ್ಥಿರಾಂಕ derivation *deriv, FILE *ಹೊರಹೋಗು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ)
{
  ಪೂರ್ಣಾಂಕ counter = 0;
  fputs (ಪೂರ್ವಪ್ರತ್ಯಯ, ಹೊರಹೋಗು);
  derivation_print_flat_impl ((derivation *)deriv, ಹೊರಹೋಗು, false, &counter, "");
  fputc ('\n', ಹೊರಹೋಗು);
}

ಶೂನ್ಯ
derivation_print_leaves (ಸ್ಥಿರಾಂಕ derivation *deriv, FILE *ಹೊರಹೋಗು)
{
  ಪೂರ್ಣಾಂಕ counter = 0;
  derivation_print_flat_impl ((derivation *)deriv, ಹೊರಹೋಗು, true, &counter, "");
  fputc ('\n', ಹೊರಹೋಗು);
}

ಶೂನ್ಯ
derivation_print (ಸ್ಥಿರಾಂಕ derivation *deriv, FILE *ಹೊರಹೋಗು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ)
{
  ಆದರೆ (getenv ("YYFLAT"))
    derivation_print_flat (deriv, ಹೊರಹೋಗು, ಪೂರ್ವಪ್ರತ್ಯಯ);
  ಇಲ್ಲದಿದ್ದರೆ
    derivation_print_ವೃಕ್ಷ (deriv, ಹೊರಹೋಗು, ಪೂರ್ವಪ್ರತ್ಯಯ);
}
