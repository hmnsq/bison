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

#समावेश <config.h>

#समावेश "derivation.h"
#समावेश "glyphs.h"

#समावेश <c-ctype.h>
#समावेश <gl_linked_list.h>
#समावेश <mbswidth.h>
#समावेश <vasnprintf.h>

#समावेश "system.h"
#समावेश "complain.h"

काष्ठा derivation
{
  प्रतीक_संख्या sym;
  derivation_list children;
  पूर्णांक reference_गिनती;
  // The rule SYM -> CHILDREN.
  स्थिर rule *rule;
  // Color assigned for styling.  Guarantees that the derivation is
  // always displayed with the same color, independently of the order
  // in which the derivations are traversed.
  पूर्णांक color;
};

अटल derivation d_dot = { -1, शून्य, -1, शून्य, -1 };

derivation *
derivation_dot (व्योम)
{
  वापस &d_dot;
}

व्योम
derivation_list_append (derivation_list dl, derivation *d)
{
  derivation_retain (d);
  gl_list_add_last (dl, d);
}

व्योम
derivation_list_prepend (derivation_list dl, derivation *d)
{
  derivation_retain (d);
  gl_list_add_first (dl, d);
}

व्योम derivation_list_free (derivation_list dl)
{
  derivation *d = शून्य;
  क्रम (gl_list_iterator_t it = gl_list_iterator (dl);
       derivation_list_अग्र (&it, &d);
       )
    अगर (d != &d_dot)
      derivation_free (d);
  gl_list_free (dl);
}

derivation *
derivation_new (प्रतीक_संख्या sym, derivation_list children,
                स्थिर rule *r)
{
  derivation *res = xmalloc (माप *res);
  res->sym = sym;
  res->children = children;
  res->reference_गिनती = 0;
  res->rule = r;
  res->color = -1;
  वापस res;
}

व्योम
derivation_retain (derivation *d)
{
  ++d->reference_गिनती;
}

व्योम
derivation_free (derivation *d)
{
  अगर (!d)
    वापस;
  derivation_list free_queue =
    gl_list_create (GL_LINKED_LIST, शून्य, शून्य, शून्य, true,
                    1, (स्थिर व्योम **)&d);
  जबतक (gl_list_आकार (free_queue) > 0)
    {
      derivation *deriv = (derivation *) gl_list_get_at (free_queue, 0);
      अगर (--deriv->reference_गिनती == 0)
        {
          अगर (deriv->children)
            {
              derivation *child = शून्य;
              क्रम (gl_list_iterator_t it = gl_list_iterator (deriv->children);
                   derivation_list_अग्र (&it, &child);
                   )
                अगर (child != &d_dot)
                  gl_list_add_last (free_queue, child);
              gl_list_free (deriv->children);
            }
          मुक्त (deriv);
        }
      gl_list_remove_at (free_queue, 0);
    }
  gl_list_free (free_queue);
}

माप_प्रकार
derivation_आकार (स्थिर derivation *deriv)
{
  अगर (!deriv->children)
    वापस 1;
  पूर्णांक आकार = 1;
  derivation *child = शून्य;
  क्रम (gl_list_iterator_t it = gl_list_iterator (deriv->children);
       derivation_list_अग्र (&it, &child);
       )
    आकार += derivation_आकार (child);
  वापस आकार;
}


// Longest distance from root to leaf.
अटल पूर्णांक
derivation_depth (स्थिर derivation *deriv)
{
  अगर (deriv->children)
    {
      // Children's depth cannot be 0, even if there are no children
      // (the case of a derivation with an empty RHS).
      पूर्णांक res = 1;
      derivation *child;
      क्रम (gl_list_iterator_t it = gl_list_iterator (deriv->children);
           derivation_list_अग्र (&it, &child);
           )
        res = max_int (res, derivation_depth (child));
      वापस res + 1;
    }
  अन्यथा
    वापस 1;
}

अटल bool
all_spaces (स्थिर अक्षर *s)
{
  जबतक (c_isspace (*s))
    s++;
  वापस *s == '\0';
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
अटल पूर्णांक
fputs_if (bool cond, खाता *out, पूर्णांक *padding, स्थिर अक्षर *s)
{
  पूर्णांक res = mbswidth (s, 0);
  अगर (cond && !all_spaces (s))
    {
      ख_लिखो (out, "%*s%s", *padding, "", s);
      *padding = 0;
    }
  अन्यथा
    {
      *padding += res;
    }
  वापस res;
}

अटल पूर्णांक
fprintf_if (bool cond, खाता *out, पूर्णांक *padding, स्थिर अक्षर *fmt, ...)
{
  अक्षर buf[256];
  माप_प्रकार len = माप (buf);
  बहु_सूची args;
  बहु_शुरू (args, fmt);
  अक्षर *cp = vasnprintf (buf, &len, fmt, args);
  बहु_पूर्ण (args);
  अगर (!cp)
    xalloc_die ();
  पूर्णांक res = fputs_if (cond, out, padding, cp);
  अगर (cp != buf)
    मुक्त (cp);
  वापस res;
}

// The width taken to report this derivation recursively down to its
// leaves.
अटल पूर्णांक
derivation_width (स्थिर derivation *deriv)
{
  अगर (deriv->children)
    {
      स्थिर प्रतीक *sym = symbols[deriv->sym];
      पूर्णांक self_width = mbswidth (sym->tag, 0);

      // Arrow and space.
      पूर्णांक children_width = down_arrow_width;
      children_width += snprintf (शून्य, 0, "%d: ", deriv->rule->संख्या);
      अगर (gl_list_आकार (deriv->children) == 0)
        // Empty rhs.
        children_width += empty_width;
      अन्यथा
        {
          अगर (gl_list_आकार (deriv->children) == 1
              && gl_list_get_first (deriv->children) == &d_dot)
            {
              children_width += empty_width;
              children_width += derivation_separator_width;
            }

          derivation *child;
          क्रम (gl_list_iterator_t it = gl_list_iterator (deriv->children);
               derivation_list_अग्र (&it, &child);
               )
            children_width
              += derivation_separator_width + derivation_width (child);
          // No separator at the beginning.
          children_width -= derivation_separator_width;
        }
      वापस max_int (self_width, children_width);
    }
  अन्यथा अगर (deriv == &d_dot)
    {
      वापस dot_width;
    }
  अन्यथा // leaf.
    {
      स्थिर प्रतीक *sym = symbols[deriv->sym];
      वापस mbswidth (sym->tag, 0);
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
अटल पूर्णांक
derivation_print_वृक्ष_impl (स्थिर derivation *deriv, खाता *out,
                            पूर्णांक depth, पूर्णांक *padding)
{
  स्थिर पूर्णांक width = derivation_width (deriv);

  पूर्णांक res = 0;
  अगर (deriv->children)
    {
      स्थिर प्रतीक *sym = symbols[deriv->sym];
      अक्षर style[20];
      snprintf (style, 20, "cex-%d", deriv->color);

      अगर (depth == 0 || depth == 1)
        {
          begin_use_class (style, out);
          begin_use_class ("cex-step", out);
        }
      अगर (depth == 0)
        {
          res += fputs_if (true, out, padding, sym->tag);
        }
      अन्यथा
        {
          res += fputs_if (depth == 1, out, padding, down_arrow);
          res += fprintf_if (depth == 1, out, padding, "%d: ", deriv->rule->संख्या);
          अगर (gl_list_आकार (deriv->children) == 0)
            // Empty rhs.
            res += fputs_if (depth == 1, out, padding, empty);
          अन्यथा
            {
              अगर (gl_list_आकार (deriv->children) == 1
                  && gl_list_get_first (deriv->children) == &d_dot)
                {
                  res += fputs_if (depth == 1, out, padding, empty);
                  res += fputs_if (depth == 1, out, padding, derivation_separator);
                }

              bool first = true;
              derivation *child;
              क्रम (gl_list_iterator_t it = gl_list_iterator (deriv->children);
                   derivation_list_अग्र (&it, &child);
                   )
                {
                  अगर (!first)
                    res += fputs_if (depth == 1, out, padding, derivation_separator);
                  res += derivation_print_वृक्ष_impl (child, out, depth - 1, padding);
                  first = false;
                }
            }
        }
      अगर (depth == 0 || depth == 1)
        {
          end_use_class ("cex-step", out);
          end_use_class (style, out);
        }
      *padding += width - res;
      res = width;
    }
  अन्यथा अगर (deriv == &d_dot)
    {
      अगर (depth == 0)
        begin_use_class ("cex-dot", out);
      res += fputs_if (depth == 0, out, padding, dot);
      अगर (depth == 0)
        end_use_class ("cex-dot", out);
    }
  अन्यथा // leaf.
    {
      स्थिर प्रतीक *sym = symbols[deriv->sym];
      अगर (depth == 0)
        begin_use_class ("cex-leaf", out);
      res += fputs_if (depth == 0, out, padding, sym->tag);
      अगर (depth == 0)
        end_use_class ("cex-leaf", out);
    }
  वापस res;
}

अटल व्योम
derivation_print_वृक्ष (स्थिर derivation *deriv, खाता *out, स्थिर अक्षर *prefix)
{
  ख_अक्षर_दो ('\n', out);
  क्रम (पूर्णांक depth = 0, max_depth = derivation_depth (deriv);
       depth < max_depth; ++depth)
    {
      पूर्णांक padding = 0;
      ख_लिखो (out, "    %s", prefix);
      derivation_print_वृक्ष_impl (deriv, out, depth, &padding);
      ख_अक्षर_दो ('\n', out);
    }
}


/* Print DERIV, colored according to COUNTER.
   Return false if nothing is printed.  */
अटल bool
derivation_print_flat_impl (derivation *deriv, खाता *out,
                            bool leaves_only,
                            पूर्णांक *गणक, स्थिर अक्षर *prefix)
{
  अगर (deriv->children)
    {
      स्थिर प्रतीक *sym = symbols[deriv->sym];
      deriv->color = *गणक;
      ++*गणक;
      अक्षर style[20];
      snprintf (style, 20, "cex-%d", deriv->color);
      begin_use_class (style, out);

      अगर (!leaves_only)
        {
          ख_माला_दो (prefix, out);
          begin_use_class ("cex-step", out);
          ख_लिखो (out, "%s %s [ ", sym->tag, arrow);
          end_use_class ("cex-step", out);
          prefix = "";
        }
      bool res = false;
      derivation *child;
      क्रम (gl_list_iterator_t it = gl_list_iterator (deriv->children);
           derivation_list_अग्र (&it, &child);
           )
        {
          अगर (derivation_print_flat_impl (child, out,
                                          leaves_only, गणक, prefix))
            {
              prefix = " ";
              res = true;
            }
          अन्यथा अगर (!leaves_only)
            prefix = " ";
        }
      अगर (!leaves_only)
        {
          begin_use_class ("cex-step", out);
          अगर (res)
            ख_माला_दो (" ]", out);
          अन्यथा
            ख_माला_दो ("]", out);
          end_use_class ("cex-step", out);
        }
      end_use_class (style, out);
      वापस res;
    }
  अन्यथा अगर (deriv == &d_dot)
    {
      ख_माला_दो (prefix, out);
      begin_use_class ("cex-dot", out);
      ख_माला_दो (dot, out);
      end_use_class ("cex-dot", out);
    }
  अन्यथा // leaf.
    {
      ख_माला_दो (prefix, out);
      स्थिर प्रतीक *sym = symbols[deriv->sym];
      begin_use_class ("cex-leaf", out);
      ख_लिखो (out, "%s", sym->tag);
      end_use_class ("cex-leaf", out);
    }
  वापस true;
}

अटल व्योम
derivation_print_flat (स्थिर derivation *deriv, खाता *out, स्थिर अक्षर *prefix)
{
  पूर्णांक गणक = 0;
  ख_माला_दो (prefix, out);
  derivation_print_flat_impl ((derivation *)deriv, out, false, &गणक, "");
  ख_अक्षर_दो ('\n', out);
}

व्योम
derivation_print_leaves (स्थिर derivation *deriv, खाता *out)
{
  पूर्णांक गणक = 0;
  derivation_print_flat_impl ((derivation *)deriv, out, true, &गणक, "");
  ख_अक्षर_दो ('\n', out);
}

व्योम
derivation_print (स्थिर derivation *deriv, खाता *out, स्थिर अक्षर *prefix)
{
  अगर (दो_पर्या ("YYFLAT"))
    derivation_print_flat (deriv, out, prefix);
  अन्यथा
    derivation_print_वृक्ष (deriv, out, prefix);
}
