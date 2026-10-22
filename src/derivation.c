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
  चिह्नम्_सङ्ख्या sym;
  derivation_list children;
  पूर्णाङ्क reference_गणनम्;
  // The rule SYM -> CHILDREN.
  ध्रुव नियमः *नियमः;
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
  gl_list_सङ्कलय_चरमः (dl, d);
}

शून्य
derivation_list_prepend (derivation_list dl, derivation *d)
{
  derivation_retain (d);
  gl_list_सङ्कलय_first (dl, d);
}

शून्य derivation_list_free (derivation_list dl)
{
  derivation *d = रिक्त;
  प्रति (gl_list_iterator_t it = gl_list_iterator (dl);
       derivation_list_अग्र (&it, &d);
       )
    यदि (d != &d_dot)
      derivation_free (d);
  gl_list_free (dl);
}

derivation *
derivation_new (चिह्नम्_सङ्ख्या sym, derivation_list children,
                ध्रुव नियमः *r)
{
  derivation *res = xmalloc (परिमाण *res);
  res->sym = sym;
  res->children = children;
  res->reference_गणनम् = 0;
  res->नियमः = r;
  res->color = -1;
  प्रत्यागम res;
}

शून्य
derivation_retain (derivation *d)
{
  ++d->reference_गणनम्;
}

शून्य
derivation_free (derivation *d)
{
  यदि (!d)
    प्रत्यागम;
  derivation_list free_queue =
    gl_list_create (GL_LINKED_LIST, रिक्त, रिक्त, रिक्त, true,
                    1, (ध्रुव शून्य **)&d);
  यावत् (gl_list_आकार (free_queue) > 0)
    {
      derivation *deriv = (derivation *) gl_list_get_at (free_queue, 0);
      यदि (--deriv->reference_गणनम् == 0)
        {
          यदि (deriv->children)
            {
              derivation *child = रिक्त;
              प्रति (gl_list_iterator_t it = gl_list_iterator (deriv->children);
                   derivation_list_अग्र (&it, &child);
                   )
                यदि (child != &d_dot)
                  gl_list_सङ्कलय_चरमः (free_queue, child);
              gl_list_free (deriv->children);
            }
          मुक्त (deriv);
        }
      gl_list_remove_at (free_queue, 0);
    }
  gl_list_free (free_queue);
}

size_t
derivation_आकार (ध्रुव derivation *deriv)
{
  यदि (!deriv->children)
    प्रत्यागम 1;
  पूर्णाङ्क आकार = 1;
  derivation *child = रिक्त;
  प्रति (gl_list_iterator_t it = gl_list_iterator (deriv->children);
       derivation_list_अग्र (&it, &child);
       )
    आकार += derivation_आकार (child);
  प्रत्यागम आकार;
}


// Longest distance from root to leaf.
स्थिर पूर्णाङ्क
derivation_गभीरता (ध्रुव derivation *deriv)
{
  यदि (deriv->children)
    {
      // Children's depth cannot be 0, even if there are no children
      // (the case of a derivation with an empty RHS).
      पूर्णाङ्क res = 1;
      derivation *child;
      प्रति (gl_list_iterator_t it = gl_list_iterator (deriv->children);
           derivation_list_अग्र (&it, &child);
           )
        res = max_int (res, derivation_गभीरता (child));
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
fputs_if (bool cond, FILE *बहिर्गमनम्, पूर्णाङ्क *padding, ध्रुव वर्ण *s)
{
  पूर्णाङ्क res = mbswidth (s, 0);
  यदि (cond && !all_spaces (s))
    {
      fprintf (बहिर्गमनम्, "%*s%s", *padding, "", s);
      *padding = 0;
    }
  अन्यथा
    {
      *padding += res;
    }
  प्रत्यागम res;
}

स्थिर पूर्णाङ्क
fprintf_if (bool cond, FILE *बहिर्गमनम्, पूर्णाङ्क *padding, ध्रुव वर्ण *fmt, ...)
{
  वर्ण buf[256];
  size_t len = परिमाण (buf);
  va_list तर्काः;
  va_start (तर्काः, fmt);
  वर्ण *cp = vasnprintf (buf, &len, fmt, तर्काः);
  va_अन्तः (तर्काः);
  यदि (!cp)
    xalloc_die ();
  पूर्णाङ्क res = fputs_if (cond, बहिर्गमनम्, padding, cp);
  यदि (cp != buf)
    मुक्त (cp);
  प्रत्यागम res;
}

// The width taken to report this derivation recursively down to its
// leaves.
स्थिर पूर्णाङ्क
derivation_विस्तारः (ध्रुव derivation *deriv)
{
  यदि (deriv->children)
    {
      ध्रुव चिह्नम् *sym = symbols[deriv->sym];
      पूर्णाङ्क self_विस्तारः = mbswidth (sym->चिह्नकम्, 0);

      // Arrow and space.
      पूर्णाङ्क children_विस्तारः = अधस्तात्_arrow_विस्तारः;
      children_विस्तारः += snprintf (रिक्त, 0, "%d: ", deriv->नियमः->सङ्ख्या);
      यदि (gl_list_आकार (deriv->children) == 0)
        // Empty rhs.
        children_विस्तारः += रिक्तानि_विस्तारः;
      अन्यथा
        {
          यदि (gl_list_आकार (deriv->children) == 1
              && gl_list_get_first (deriv->children) == &d_dot)
            {
              children_विस्तारः += रिक्तानि_विस्तारः;
              children_विस्तारः += derivation_separator_विस्तारः;
            }

          derivation *child;
          प्रति (gl_list_iterator_t it = gl_list_iterator (deriv->children);
               derivation_list_अग्र (&it, &child);
               )
            children_विस्तारः
              += derivation_separator_विस्तारः + derivation_विस्तारः (child);
          // No separator at the beginning.
          children_विस्तारः -= derivation_separator_विस्तारः;
        }
      प्रत्यागम max_int (self_विस्तारः, children_विस्तारः);
    }
  अन्यथा यदि (deriv == &d_dot)
    {
      प्रत्यागम dot_विस्तारः;
    }
  अन्यथा // leaf.
    {
      ध्रुव चिह्नम् *sym = symbols[deriv->sym];
      प्रत्यागम mbswidth (sym->चिह्नकम्, 0);
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
derivation_print_वृक्षः_impl (ध्रुव derivation *deriv, FILE *बहिर्गमनम्,
                            पूर्णाङ्क गभीरता, पूर्णाङ्क *padding)
{
  ध्रुव पूर्णाङ्क विस्तारः = derivation_विस्तारः (deriv);

  पूर्णाङ्क res = 0;
  यदि (deriv->children)
    {
      ध्रुव चिह्नम् *sym = symbols[deriv->sym];
      वर्ण style[20];
      snprintf (style, 20, "cex-%d", deriv->color);

      यदि (गभीरता == 0 || गभीरता == 1)
        {
          begin_use_class (style, बहिर्गमनम्);
          begin_use_class ("cex-step", बहिर्गमनम्);
        }
      यदि (गभीरता == 0)
        {
          res += fputs_if (true, बहिर्गमनम्, padding, sym->चिह्नकम्);
        }
      अन्यथा
        {
          res += fputs_if (गभीरता == 1, बहिर्गमनम्, padding, अधस्तात्_arrow);
          res += fprintf_if (गभीरता == 1, बहिर्गमनम्, padding, "%d: ", deriv->नियमः->सङ्ख्या);
          यदि (gl_list_आकार (deriv->children) == 0)
            // Empty rhs.
            res += fputs_if (गभीरता == 1, बहिर्गमनम्, padding, रिक्तानि);
          अन्यथा
            {
              यदि (gl_list_आकार (deriv->children) == 1
                  && gl_list_get_first (deriv->children) == &d_dot)
                {
                  res += fputs_if (गभीरता == 1, बहिर्गमनम्, padding, रिक्तानि);
                  res += fputs_if (गभीरता == 1, बहिर्गमनम्, padding, derivation_separator);
                }

              bool first = true;
              derivation *child;
              प्रति (gl_list_iterator_t it = gl_list_iterator (deriv->children);
                   derivation_list_अग्र (&it, &child);
                   )
                {
                  यदि (!first)
                    res += fputs_if (गभीरता == 1, बहिर्गमनम्, padding, derivation_separator);
                  res += derivation_print_वृक्षः_impl (child, बहिर्गमनम्, गभीरता - 1, padding);
                  first = false;
                }
            }
        }
      यदि (गभीरता == 0 || गभीरता == 1)
        {
          अन्तः_use_class ("cex-step", बहिर्गमनम्);
          अन्तः_use_class (style, बहिर्गमनम्);
        }
      *padding += विस्तारः - res;
      res = विस्तारः;
    }
  अन्यथा यदि (deriv == &d_dot)
    {
      यदि (गभीरता == 0)
        begin_use_class ("cex-dot", बहिर्गमनम्);
      res += fputs_if (गभीरता == 0, बहिर्गमनम्, padding, dot);
      यदि (गभीरता == 0)
        अन्तः_use_class ("cex-dot", बहिर्गमनम्);
    }
  अन्यथा // leaf.
    {
      ध्रुव चिह्नम् *sym = symbols[deriv->sym];
      यदि (गभीरता == 0)
        begin_use_class ("cex-leaf", बहिर्गमनम्);
      res += fputs_if (गभीरता == 0, बहिर्गमनम्, padding, sym->चिह्नकम्);
      यदि (गभीरता == 0)
        अन्तः_use_class ("cex-leaf", बहिर्गमनम्);
    }
  प्रत्यागम res;
}

स्थिर शून्य
derivation_print_वृक्षः (ध्रुव derivation *deriv, FILE *बहिर्गमनम्, ध्रुव वर्ण *उपसर्गः)
{
  fputc ('\n', बहिर्गमनम्);
  प्रति (पूर्णाङ्क गभीरता = 0, max_गभीरता = derivation_गभीरता (deriv);
       गभीरता < max_गभीरता; ++गभीरता)
    {
      पूर्णाङ्क padding = 0;
      fprintf (बहिर्गमनम्, "    %s", उपसर्गः);
      derivation_print_वृक्षः_impl (deriv, बहिर्गमनम्, गभीरता, &padding);
      fputc ('\n', बहिर्गमनम्);
    }
}


/* Print DERIV, colored according to COUNTER.
   Return false if nothing is printed.  */
स्थिर bool
derivation_print_flat_impl (derivation *deriv, FILE *बहिर्गमनम्,
                            bool leaves_only,
                            पूर्णाङ्क *गणक, ध्रुव वर्ण *उपसर्गः)
{
  यदि (deriv->children)
    {
      ध्रुव चिह्नम् *sym = symbols[deriv->sym];
      deriv->color = *गणक;
      ++*गणक;
      वर्ण style[20];
      snprintf (style, 20, "cex-%d", deriv->color);
      begin_use_class (style, बहिर्गमनम्);

      यदि (!leaves_only)
        {
          fputs (उपसर्गः, बहिर्गमनम्);
          begin_use_class ("cex-step", बहिर्गमनम्);
          fprintf (बहिर्गमनम्, "%s %s [ ", sym->चिह्नकम्, arrow);
          अन्तः_use_class ("cex-step", बहिर्गमनम्);
          उपसर्गः = "";
        }
      bool res = false;
      derivation *child;
      प्रति (gl_list_iterator_t it = gl_list_iterator (deriv->children);
           derivation_list_अग्र (&it, &child);
           )
        {
          यदि (derivation_print_flat_impl (child, बहिर्गमनम्,
                                          leaves_only, गणक, उपसर्गः))
            {
              उपसर्गः = " ";
              res = true;
            }
          अन्यथा यदि (!leaves_only)
            उपसर्गः = " ";
        }
      यदि (!leaves_only)
        {
          begin_use_class ("cex-step", बहिर्गमनम्);
          यदि (res)
            fputs (" ]", बहिर्गमनम्);
          अन्यथा
            fputs ("]", बहिर्गमनम्);
          अन्तः_use_class ("cex-step", बहिर्गमनम्);
        }
      अन्तः_use_class (style, बहिर्गमनम्);
      प्रत्यागम res;
    }
  अन्यथा यदि (deriv == &d_dot)
    {
      fputs (उपसर्गः, बहिर्गमनम्);
      begin_use_class ("cex-dot", बहिर्गमनम्);
      fputs (dot, बहिर्गमनम्);
      अन्तः_use_class ("cex-dot", बहिर्गमनम्);
    }
  अन्यथा // leaf.
    {
      fputs (उपसर्गः, बहिर्गमनम्);
      ध्रुव चिह्नम् *sym = symbols[deriv->sym];
      begin_use_class ("cex-leaf", बहिर्गमनम्);
      fprintf (बहिर्गमनम्, "%s", sym->चिह्नकम्);
      अन्तः_use_class ("cex-leaf", बहिर्गमनम्);
    }
  प्रत्यागम true;
}

स्थिर शून्य
derivation_print_flat (ध्रुव derivation *deriv, FILE *बहिर्गमनम्, ध्रुव वर्ण *उपसर्गः)
{
  पूर्णाङ्क गणक = 0;
  fputs (उपसर्गः, बहिर्गमनम्);
  derivation_print_flat_impl ((derivation *)deriv, बहिर्गमनम्, false, &गणक, "");
  fputc ('\n', बहिर्गमनम्);
}

शून्य
derivation_print_leaves (ध्रुव derivation *deriv, FILE *बहिर्गमनम्)
{
  पूर्णाङ्क गणक = 0;
  derivation_print_flat_impl ((derivation *)deriv, बहिर्गमनम्, true, &गणक, "");
  fputc ('\n', बहिर्गमनम्);
}

शून्य
derivation_print (ध्रुव derivation *deriv, FILE *बहिर्गमनम्, ध्रुव वर्ण *उपसर्गः)
{
  यदि (getenv ("YYFLAT"))
    derivation_print_flat (deriv, बहिर्गमनम्, उपसर्गः);
  अन्यथा
    derivation_print_वृक्षः (deriv, बहिर्गमनम्, उपसर्गः);
}
