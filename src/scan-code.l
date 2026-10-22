/* Bison Action Scanner                             -*- C -*-

   Copyright (C) 2006-2015, 2018-2022 Free Software Foundation, Inc.

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

%option debug nodefault noinput nounput noyywrap never-interactive
%option اڳياڙي="code_" outfile="lex.yy.c"

%{
#ضمن <c-ctype.h>
#ضمن <get-errno.h>
#ضمن <quote.h>

#ضمن "src/complain.h"
#ضمن "src/getargs.h"
#ضمن "src/muscle-tab.h"
#ضمن "src/reader.h"
#ضمن "src/scan-code.h"
#ضمن "src/symlist.h"

#وصف_ڪريو FLEX_اڳياڙي(Id) ڪوڊ_2 ## Id
#ضمن "src/flex-scanner.h"

/* Work around a bug in flex 2.5.31.  See Debian bug 333231
   <https://bugs.debian.org/333231>.  */
#وصف_رد ڪوڊ_wrap
#وصف_ڪريو ڪوڊ_wrap() 1

بنية obstack *obstack_for_actions = &obstack_for_string;

/* The current calling start condition: SC_RULE_ACTION or
   SC_SYMBOL_ACTION. */
# define YY_DECL ساكن حرف *ڪوڊ_lex (ڪوڊ_props *self, صحيح sc_context)
YY_DECL;

#وصف_ڪريو YY_USER_ACTION  location_compute (loc, &loc->پڇاڙي, yytext, yyleng);

ساكن حرف *fetch_type_name (حرف *cp, حرف ثابت **type_name,
                              ثابت location *dollar_loc);

ساكن فراغ handle_action_dollar (نشان_list *قاعدو, حرف *cp,
                                  ثابت location *dollar_loc);
ساكن فراغ handle_action_at (نشان_list *قاعدو, حرف *cp,
                              ثابت location *at_loc);

/* A string to be pushed to obstack after dollar/at has been handled. */
ساكن حرف *ref_پڇ_fields;

ساكن location current_loc;
ساكن location *loc = &current_loc;

/* A string representing the most recent translation.  */
ساكن حرف *آخري_string;

/* True if an untyped $$ or $n was seen.  */
ساكن bool untyped_var_ڏٺل;

%}
 /* C and C++ comments in code. */
%x SC_COMMENT SC_سٽ_COMMENT
 /* Strings and characters in code. */
%x SC_STRING SC_CHARACTER
 /* Whether in a rule or symbol action.  Specifies the translation
    of $ and @.  */
%x SC_قاعدو_ACTION SC_نشان_ACTION


/* POSIX says that a tag must be both an id and a C union member, but
   historically almost any character is allowed in a tag.  We disallow
   NUL and newline, as this simplifies our implementation.  We allow
   "->" as a means to dereference a pointer.  */
ٽيگ      ([^\0\n>]|->)*[^-]

/* Zero or more instances of backslash-newline.  Following GCC, allow
   white space between the backslash and the newline.  */
splice   (\\[ \f\t\v]*\n)*

/* C style identifier. Must start with letter. Will be used for
   named symbol references. Shall be kept synchronized with
   scan-gram.l "letter" and "id". */
letter   [.abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_]
id       {letter}({letter}|[-0-9])*
ref      -?[0-9]+|{id}|"["{id}"]"|"$"

%%

%{
  /* This scanner is special: it is invoked only once, henceforth
     is expected to return only once.  This initialization is
     therefore done once per action to translate. */
  aver (sc_context == SC_نشان_ACTION
        || sc_context == SC_قاعدو_ACTION
        || sc_context == INITIAL);
  BEGIN sc_context;
%}

  /*------------------------------------------------------------.
  | Scanning a C comment.  The initial '/ *' is already eaten.  |
  `------------------------------------------------------------*/

<SC_COMMENT>
{
  "*"{splice}"/"  STRING_GROW (); BEGIN sc_context;
}


  /*--------------------------------------------------------------.
  | Scanning a line comment.  The initial '//' is already eaten.  |
  `--------------------------------------------------------------*/

<SC_سٽ_COMMENT>
{
  "\n"           STRING_GROW (); BEGIN sc_context;
  {splice}       STRING_GROW ();
}


  /*--------------------------------------------.
  | Scanning user-code characters and strings.  |
  `--------------------------------------------*/

<SC_CHARACTER,SC_STRING>
{
  {splice}|\\{splice}.  STRING_GROW ();
}

<SC_CHARACTER>
{
  "'"           STRING_GROW (); BEGIN sc_context;
}

<SC_STRING>
{
  "\""          STRING_GROW (); BEGIN sc_context;
}


<SC_قاعدو_ACTION,SC_نشان_ACTION>
{
  "'"              STRING_GROW (); BEGIN SC_CHARACTER;
  "\""             STRING_GROW (); BEGIN SC_STRING;
  "/"{splice}"*"   STRING_GROW (); BEGIN SC_COMMENT;
  "/"{splice}"/"   STRING_GROW (); BEGIN SC_سٽ_COMMENT;

  [$@]  {
    complain (loc, Wother, _("stray '%s'"), yytext);
    obstack_escape (&obstack_for_string, yytext);
  }
}

<SC_قاعدو_ACTION>
{
  "$"("<"{ٽيگ}">")?{ref}  {
    ref_پڇ_fields = عدم;
    handle_action_dollar (self->قاعدو, yytext, loc);
    إذا (ref_پڇ_fields)
      obstack_sgrow (&obstack_for_string, ref_پڇ_fields);
  }
  "@"{ref} {
    ref_پڇ_fields = عدم;
    handle_action_at (self->قاعدو, yytext, loc);
    إذا (ref_پڇ_fields)
      obstack_sgrow (&obstack_for_string, ref_پڇ_fields);
  }
}

<SC_نشان_ACTION>
{
  "$"("<"{ٽيگ}">")?"$" {
    ثابت حرف *type_name = عدم;
    fetch_type_name (yytext + 1, &type_name, loc)[-1] = 0;
    obstack_sgrow (&obstack_for_string, "]b4_dollar_dollar(");
    obstack_quote (&obstack_for_string, type_name);
    obstack_sgrow (&obstack_for_string, ")[");
    self->is_قدر_استعمال_ٿيل = true;
  }
  "@$" {
    obstack_sgrow (&obstack_for_string, "]b4_at_dollar[");
    muscle_percent_define_ensure("locations", *loc, true);
  }
}


<*>
{
  "b4_"   STRING_SGROW ("b4@'_");
  "m4_"   STRING_SGROW ("m4@'_");

  /* Escape M4 quoting characters in C code.  */
  [$@\[\]]    obstack_escape (&obstack_for_string, yytext);

  /* By default, grow the string obstack with the input.  */
  .|\n        STRING_GROW ();

  /* End of processing. */
  <<EOF>>     STRING_پڄاڻي (); أرجع آخري_string;
}

%%

ساكن مضمن bool
is_dot_or_dash (حرف ch)
{
  أرجع ch == '.' || ch == '-';
}

ساكن مضمن bool
شامل_آهي_dot_or_dash (ثابت حرف* p)
{
  أرجع !!strpbrk (p, ".-");
}

/* Defines a variant of a symbolic name resolution. */
تعريفنوع بنية
{
  /* Index in symbol list. */
  صحيح نشان_اشاريو;

  /* Matched symbol id and loc. */
  uniqstr id;
  location loc;

  /* Hiding named reference. */
  named_ref* hidden_by;

  /* Error flags. May contain zero (no errors) or
     a combination of VARIANT_* values. */
  بنانشان err;
} variant;

/* Set when the variant refers to a symbol hidden
   by an explicit symbol reference. */
#وصف_ڪريو VARIANT_HIDDEN (1 << 0)

/* Set when the variant refers to a symbol containing
   dots or dashes. Will require explicit bracketing. */
#وصف_ڪريو VARIANT_BAD_BRACKETING (1 << 1)

/* Set when the variant refers to a symbol which is
   not visible from current midrule. */
#وصف_ڪريو VARIANT_NOT_VISIBLE_FROM_MIDRULE (1 << 2)

ساكن variant *variant_جدول = عدم;
ساكن صحيح variant_جدول_سائيز = 0;
ساكن صحيح variant_ڳڻپ = 0;

ساكن variant *
variant_جدول_grow (فراغ)
{
  ++variant_ڳڻپ;
  إذا (variant_ڳڻپ > variant_جدول_سائيز)
    {
      طالما (variant_ڳڻپ > variant_جدول_سائيز)
        variant_جدول_سائيز = 2 * variant_جدول_سائيز + 3;
      variant_جدول = xnrealloc (variant_جدول, variant_جدول_سائيز,
                                 حجم *variant_جدول);
    }
  أرجع &variant_جدول[variant_count - 1];
}

ساكن فراغ
variant_جدول_free (فراغ)
{
  حرر (variant_جدول);
  variant_جدول = عدم;
  variant_جدول_سائيز = variant_ڳڻپ = 0;
}

ساكن حرف ثابت *
ڳوليو_اڳياڙي_پڇاڙي (حرف ثابت *اڳياڙي, حرف ثابت *cp, حرف ثابت *پڇاڙي)
{
  لكل (; *اڳياڙي && cp != پڇاڙي; ++اڳياڙي, ++cp)
    إذا (*اڳياڙي != *cp)
      أرجع عدم;

  أرجع *اڳياڙي ? عدم : cp;
}

ساكن variant *
variant_جوڙيو (uniqstr id, location id_loc, صحيح نشان_اشاريو,
             حرف ثابت *cp, حرف ثابت *cp_پڇاڙي, bool explicit_bracketing)
{
  حرف ثابت *اڳياڙي_پڇاڙي = ڳوليو_اڳياڙي_پڇاڙي (id, cp, cp_پڇاڙي);
  إذا (اڳياڙي_پڇاڙي &&
      (اڳياڙي_پڇاڙي == cp_پڇاڙي ||
       (!explicit_bracketing && is_dot_or_dash (*اڳياڙي_پڇاڙي))))
    {
      variant *r = variant_جدول_grow ();
      r->نشان_اشاريو = نشان_اشاريو;
      r->id = id;
      r->loc = id_loc;
      r->hidden_by = عدم;
      r->err = 0;
      أرجع r;
    }
  وإلا
    أرجع عدم;
}

ساكن ثابت حرف *
get_at_spec(صحيح نشان_اشاريو)
{
  ساكن حرف at_buf[20];
  إذا (نشان_اشاريو == 0)
    strcpy (at_buf, "$$");
  وإلا
    snprintf (at_buf, حجم at_buf, "$%u", نشان_اشاريو);
  أرجع at_buf;
}

ساكن فراغ
show_sub_پيغام (warnings warning,
                  ثابت حرف* cp, bool explicit_bracketing,
                  صحيح midrule_rhs_اشاريو, حرف dollar_or_at,
                  ثابت variant *var)
{
  ثابت حرف *at_spec = get_at_spec (var->نشان_اشاريو);

  إذا (var->err == 0)
    subcomplain (&var->loc, warning,
                 _("refers to: %c%s at %s"), dollar_or_at,
                 var->id, at_spec);
  وإلا
    {
      ثابت حرف *id;
      location id_loc;

      إذا (var->hidden_by)
        {
          id = var->hidden_by->id;
          id_loc = var->hidden_by->loc;
        }
      وإلا
        {
          id = var->id;
          id_loc = var->loc;
        }

      ثابت حرف *پڇ = explicit_bracketing ? "" : cp + لڙي_ڊيگهه (var->id);

      /* Create the explanation message. */
      ساكن بنية obstack نياپو_buf;
      obstack_init (&نياپو_buf);

      obstack_printf (&نياپو_buf, _("possibly meant: %c"), dollar_or_at);
      إذا (شامل_آهي_dot_or_dash (id))
        obstack_printf (&نياپو_buf, "[%s]", id);
      وإلا
        obstack_sgrow (&نياپو_buf, id);
      obstack_sgrow (&نياپو_buf, پڇ);

      إذا (var->err & VARIANT_HIDDEN)
        {
          obstack_printf (&نياپو_buf, _(", hiding %c"), dollar_or_at);
          إذا (شامل_آهي_dot_or_dash (var->id))
            obstack_printf (&نياپو_buf, "[%s]", var->id);
          وإلا
            obstack_sgrow (&نياپو_buf, var->id);
          obstack_sgrow (&نياپو_buf, پڇ);
        }

      obstack_printf (&نياپو_buf, _(" at %s"), at_spec);

      إذا (var->err & VARIANT_NOT_VISIBLE_FROM_MIDRULE)
        obstack_printf (&نياپو_buf,
                        _(", cannot be accessed from midrule action at $%d"),
                        midrule_rhs_اشاريو);

      subcomplain (&id_loc, warning, "%s",
                   obstack_finish0 (&نياپو_buf));
      obstack_free (&نياپو_buf, 0);
    }
}

ساكن فراغ
show_sub_messages (warnings warning,
                   ثابت حرف* cp, bool explicit_bracketing,
                   صحيح midrule_rhs_اشاريو, حرف dollar_or_at)
{
  لكل (صحيح i = 0; i < variant_ڳڻپ; ++i)
    show_sub_پيغام (warning | silent,
                      cp, explicit_bracketing,
                      midrule_rhs_اشاريو, dollar_or_at,
                      &variant_جدول[i]);
}

/* Returned from "parse_ref" when the reference
   is inappropriate. */
#وصف_ڪريو INVALID_REF (INT_MIN)

/* Returned from "parse_ref" when the reference
   points to LHS ($$) of the current rule or midrule. */
#وصف_ڪريو LHS_REF (INT_MIN + 1)

/* Parse a positional reference in RULE.  */
ساكن ڊگهو
parse_positional_ref (حرف *cp, صحيح قاعدو_ڊيگهائي,
                      حرف *متن, ثابت location *متن_loc)
{
  ڊگهو انگ = strtol (cp, &cp, 10);
  إذا (1 - وڌ_کان_وڌ_عدد + قاعدو_ڊيگهائي <= انگ && انگ <= قاعدو_ڊيگهائي)
    أرجع انگ;
  وإلا
    {
      complain (متن_loc, complaint, _("integer out of range: %s"),
                quote (متن));
      أرجع INVALID_REF;
    }
}


/* Parse named or positional reference. In case of positional
   references, can return negative values for $-n "deep" stack
   accesses. */
ساكن ڊگهو
parse_ref (حرف *cp, نشان_list *قاعدو, صحيح قاعدو_ڊيگهائي,
           صحيح midrule_rhs_اشاريو, حرف *متن, ثابت location *متن_loc,
           حرف dollar_or_at)
{
  إذا ('$' == *cp)
    أرجع LHS_REF;

  إذا (c_isdigit (*cp) || (*cp == '-' && c_isdigit (cp[1])))
    أرجع parse_positional_ref (cp, قاعدو_ڊيگهائي, متن, متن_loc);

  bool ثابت explicit_bracketing = *cp == '[';

  إذا (explicit_bracketing)
    ++cp;
  وإلا
    ref_پڇ_fields = strpbrk (cp, ".-");

  حرف ثابت *cp_پڇاڙي = strchr (cp, explicit_bracketing ? ']' : '\0');

  /* Add all relevant variants. */
  {
    صحيح نشان_اشاريو;
    نشان_list *l;
    variant_ڳڻپ = 0;
    لكل (نشان_اشاريو = 0, l = قاعدو; !نشان_list_null (l);
         ++نشان_اشاريو, l = l->ايندڙ)
      {
        إذا (l->content_type != SYMLIST_نشان)
          استمر;

        variant *var
          = variant_جوڙيو (l->content.sym->ٽيگ, l->sym_loc,
                         نشان_اشاريو, cp, cp_پڇاڙي, explicit_bracketing);
        إذا (var && l->named_ref)
          var->hidden_by = l->named_ref;

        إذا (l->named_ref)
          variant_جوڙيو (l->named_ref->id, l->named_ref->loc,
                       نشان_اشاريو, cp, cp_پڇاڙي, explicit_bracketing);
      }
  }

  /* Check errors. */
  صحيح valid_variants = 0;
  صحيح valid_variant_اشاريو = 0;
  لكل (صحيح i = 0; i < variant_ڳڻپ; ++i)
    {
      variant *var = &variant_جدول[i];
      صحيح نشان_اشاريو = var->نشان_اشاريو;

      /* Check visibility from midrule actions. */
      إذا (midrule_rhs_اشاريو != 0
          && (نشان_اشاريو == 0 || midrule_rhs_اشاريو < نشان_اشاريو))
        var->err |= VARIANT_NOT_VISIBLE_FROM_MIDRULE;

      /* Check correct bracketing. */
      إذا (!explicit_bracketing && شامل_آهي_dot_or_dash (var->id))
        var->err |= VARIANT_BAD_BRACKETING;

      /* Check using of hidden symbols. */
      إذا (var->hidden_by)
        var->err |= VARIANT_HIDDEN;

      إذا (!var->err)
        {
          valid_variant_اشاريو = i;
          ++valid_variants;
        }
    }

  بدل (valid_variants)
    {
    حالة 0:
      {
        صحيح len = (explicit_bracketing || !ref_پڇ_fields) ?
          cp_پڇاڙي - cp : ref_پڇ_fields - cp;

        complain (متن_loc, complaint,
                  _("invalid reference: %s"), quote (متن));
        إذا (len == 0)
          {
            location sym_loc = *متن_loc;
            sym_loc.شروعات.ڪالم += 1;
            sym_loc.پڇاڙي = sym_loc.شروعات;
            subcomplain (&sym_loc, complaint,
                         _("syntax error after '%c', expecting integer, "
                         "letter, '_', '[', or '$'"),
                         dollar_or_at);
          }
        وإلا إذا (midrule_rhs_اشاريو)
          subcomplain (&قاعدو->rhs_loc, complaint,
                       _("symbol not found in production before $%d: "
                       "%.*s"),
                       midrule_rhs_اشاريو, len, cp);
        وإلا
          subcomplain (&قاعدو->rhs_loc, complaint,
                       _("symbol not found in production: %.*s"),
                       len, cp);

        إذا (variant_ڳڻپ > 0)
          show_sub_messages (complaint,
                             cp, explicit_bracketing, midrule_rhs_اشاريو,
                             dollar_or_at);
        أرجع INVALID_REF;
      }
    حالة 1:
      {
        إذا (variant_ڳڻپ > 1)
          {
            complain (متن_loc, Wother,
                      _("misleading reference: %s"), quote (متن));
            show_sub_messages (Wother,
                               cp, explicit_bracketing, midrule_rhs_اشاريو,
                               dollar_or_at);
          }
        {
          صحيح نشان_اشاريو =
            variant_جدول[valid_variant_index].نشان_اشاريو;
          أرجع (نشان_اشاريو == midrule_rhs_اشاريو) ? LHS_REF : نشان_اشاريو;
        }
      }
    حالة 2:
    افتراضي:
      {
        complain (متن_loc, complaint,
                  _("ambiguous reference: %s"), quote (متن));
        show_sub_messages (complaint,
                           cp, explicit_bracketing, midrule_rhs_اشاريو,
                           dollar_or_at);
        أرجع INVALID_REF;
      }
    }
}

/* Keeps track of the maximum number of semantic values to the left of
   a handle (those referenced by $0, $-1, etc.) are required by the
   semantic actions of this grammar. */
صحيح max_کاٻو_semantic_context = 0;


/* If CP points to a typename (i.e., <.*?>), set TYPE_NAME to its
   beginning (i.e., after the opening "<", and return the pointer
   immediately after it.  */

ساكن
حرف *
fetch_type_name (حرف *cp, حرف ثابت **type_name,
                 ثابت location *dollar_loc)
{
  إذا (*cp == '<')
    {
      *type_name = ++cp;
      /* Series of non-'>' or "->".  */
      طالما (*cp != '>' || cp[-1] == '-')
        ++cp;

      /* The '>' symbol will be later replaced by '\0'. Original
         'text' is needed for error messages. */
      ++cp;
      إذا (untyped_var_ڏٺل)
        complain (dollar_loc, complaint,
                  _("explicit type given in untyped grammar"));
      ٽيگ_ڏٺل = true;
    }
  أرجع cp;
}

/*------------------------------------------------------------------.
| TEXT is pointing to a wannabee semantic value (i.e., a '$').      |
|                                                                   |
| Possible inputs: $[<TYPENAME>]($|INTEGER)                         |
|                                                                   |
| Output to OBSTACK_FOR_STRING a reference to this semantic value.  |
`------------------------------------------------------------------*/

ساكن فراغ
handle_action_dollar (نشان_list *قاعدو, حرف *متن, ثابت location *dollar_loc)
{
  نشان_list *effective_قاعدو;
  صحيح effective_قاعدو_ڊيگهائي;

  إذا (قاعدو->midrule_والد_قاعدو)
    {
      effective_قاعدو = قاعدو->midrule_والد_قاعدو;
      effective_قاعدو_ڊيگهائي = قاعدو->midrule_والد_rhs_اشاريو - 1;
    }
  وإلا
    {
      effective_قاعدو = قاعدو;
      effective_قاعدو_ڊيگهائي = نشان_list_ڊيگهائي (قاعدو->ايندڙ);
    }

  /* The type name if explicit, otherwise left null. */
  حرف ثابت *type_name = عدم;
  حرف *cp = fetch_type_name (متن + 1, &type_name, dollar_loc);
  صحيح n = parse_ref (cp, effective_قاعدو, effective_قاعدو_ڊيگهائي,
                     قاعدو->midrule_والد_rhs_اشاريو, متن, dollar_loc, '$');
  /* End type_name.  Don't do it earlier: parse_ref depends on TEXT.  */
  إذا (type_name)
    cp[-1] = '\0';

  بدل (n)
    {
    حالة INVALID_REF:
      اكسر;

    حالة LHS_REF:
      {
        نشان_list *sym = نشان_list_n_get (قاعدو, 0);
        إذا (!type_name
            && !sym->content.sym->content->type_name)
          {
            إذا (union_ڏٺل || ٽيگ_ڏٺل)
              {
                إذا (قاعدو->midrule_والد_قاعدو)
                  complain (dollar_loc, complaint,
                            _("$$ for the midrule at $%d of %s"
                              " has no declared type"),
                            قاعدو->midrule_والد_rhs_اشاريو,
                            quote (effective_قاعدو->content.sym->ٽيگ));
                وإلا
                  complain (dollar_loc, complaint,
                            _("$$ of %s has no declared type"),
                            quote (قاعدو->content.sym->ٽيگ));
              }
            وإلا
              untyped_var_ڏٺل = true;
          }

        obstack_printf (&obstack_for_string, "]b4_lhs_value(orig %d, ",
                        sym->content.sym->content->عدد);
        obstack_quote (&obstack_for_string, type_name);
        obstack_sgrow (&obstack_for_string, ")[");
        قاعدو->action_props.is_قدر_استعمال_ٿيل = true;
      }
      اكسر;

      /* Reference to a RHS symbol.  */
    افتراضي:
      {
        إذا (max_کاٻو_semantic_context < 1 - n)
          max_کاٻو_semantic_context = 1 - n;
        نشان_list *sym = 0 < n ? نشان_list_n_get (effective_قاعدو, n) : عدم;
        إذا (!type_name
            && (!sym || !sym->content.sym->content->type_name))
          {
            إذا (union_ڏٺل || ٽيگ_ڏٺل)
              complain (dollar_loc, complaint,
                        _("$%s of %s has no declared type"), cp,
                        quote (effective_قاعدو->content.sym->ٽيگ));
            وإلا
              untyped_var_ڏٺل = true;
          }

        obstack_printf (&obstack_for_string,
                        "]b4_rhs_value(%d, %d, ",
                        effective_قاعدو_ڊيگهائي, n);
        إذا (sym)
          obstack_printf (&obstack_for_string, "%s%d, ",
                          sym->content.sym->content->class == nterm_sym ? "orig " : "",
                          sym->content.sym->content->عدد);
        وإلا
          obstack_sgrow (&obstack_for_string, "[], ");

        obstack_quote (&obstack_for_string, type_name);
        obstack_sgrow (&obstack_for_string, ")[");
        إذا (0 < n)
          {
            إذا (muscle_percent_define_ifdef ("api.value.automove")
                && sym->action_props.is_قدر_استعمال_ٿيل)
              complain (dollar_loc, Wother,
                        _("multiple occurrences of $%d with api.value.automove"),
                        n);
            sym->action_props.is_قدر_استعمال_ٿيل = true;
          }
      }
      اكسر;
    }
}


/*------------------------------------------------------.
| TEXT is a location token (i.e., a '@...').  Output to |
| OBSTACK_FOR_STRING a reference to this location.      |
`------------------------------------------------------*/

ساكن فراغ
handle_action_at (نشان_list *قاعدو, حرف *متن, ثابت location *at_loc)
{
  نشان_list *effective_قاعدو;
  صحيح effective_قاعدو_ڊيگهائي;

  إذا (قاعدو->midrule_والد_قاعدو)
    {
      effective_قاعدو = قاعدو->midrule_والد_قاعدو;
      effective_قاعدو_ڊيگهائي = قاعدو->midrule_والد_rhs_اشاريو - 1;
    }
  وإلا
    {
      effective_قاعدو = قاعدو;
      effective_قاعدو_ڊيگهائي = نشان_list_ڊيگهائي (قاعدو->ايندڙ);
    }

  muscle_percent_define_ensure ("locations", *at_loc, true);

  صحيح n = parse_ref (متن + 1, effective_قاعدو, effective_قاعدو_ڊيگهائي,
                     قاعدو->midrule_والد_rhs_اشاريو, متن, at_loc, '@');
  بدل (n)
    {
    حالة INVALID_REF:
      اكسر;

    حالة LHS_REF:
      obstack_sgrow (&obstack_for_string, "]b4_lhs_location[");
      اكسر;

    افتراضي:
      obstack_printf (&obstack_for_string, "]b4_rhs_location(%d, %d)[",
                      effective_قاعدو_ڊيگهائي, n);
      اكسر;
    }
}


/*-------------------------.
| Initialize the scanner.  |
`-------------------------*/

/* Translate the '$...' and '@...' in \a self, in the context \a
   sc_context (SC_RULE_ACTION, SC_SYMBOL_ACTION, INITIAL).  */

ساكن حرف ثابت *
translate_action (ڪوڊ_props *self, صحيح sc_context)
{
  loc->شروعات = loc->پڇاڙي = self->location.شروعات;
  yy_switch_منزل_بفر (yy_جاچ_string (self->ڪوڊ));
  حرف *res = ڪوڊ_lex (self, sc_context);
  yy_delete_بفر (YY_CURRENT_بفر);
  أرجع res;
}

/*------------------------------------------------------------------------.
| Implementation of the public interface as documented in "scan-code.h".  |
`------------------------------------------------------------------------*/

فراغ
ڪوڊ_props_none_init (ڪوڊ_props *self)
{
  *self = ڪوڊ_props_none;
}

ڪوڊ_props ڪوڊ_props_none = ڪوڊ_PROPS_NONE_INIT;

فراغ
ڪوڊ_props_plain_init (ڪوڊ_props *self, حرف ثابت *ڪوڊ,
                       location ڪوڊ_loc)
{
  ڪوڊ_props_none_init (self);
  self->قسم = ڪوڊ_PROPS_PLAIN;
  self->ڪوڊ = ڪوڊ;
  self->location = ڪوڊ_loc;
}

فراغ
ڪوڊ_props_نشان_action_init (ڪوڊ_props *self, حرف ثابت *ڪوڊ,
                               location ڪوڊ_loc)
{
  ڪوڊ_props_none_init (self);
  self->قسم = ڪوڊ_PROPS_نشان_ACTION;
  self->ڪوڊ = ڪوڊ;
  self->location = ڪوڊ_loc;
}

فراغ
ڪوڊ_props_قاعدو_action_init (ڪوڊ_props *self, حرف ثابت *ڪوڊ,
                             location ڪوڊ_loc, نشان_list *قاعدو,
                             named_ref *name, uniqstr type,
                             bool is_predicate)
{
  ڪوڊ_props_none_init (self);
  self->قسم = ڪوڊ_PROPS_قاعدو_ACTION;
  self->ڪوڊ = ڪوڊ;
  self->location = ڪوڊ_loc;
  self->قاعدو = قاعدو;
  self->named_ref = name;
  self->type = type;
  self->is_predicate = is_predicate;
}

فراغ
ڪوڊ_props_translate_ڪوڊ (ڪوڊ_props *self)
{
  بدل (self->قسم)
    {
    حالة ڪوڊ_PROPS_NONE:
      اكسر;
    حالة ڪوڊ_PROPS_PLAIN:
      self->ڪوڊ = translate_action (self, INITIAL);
      اكسر;
    حالة ڪوڊ_PROPS_نشان_ACTION:
      self->ڪوڊ = translate_action (self, SC_نشان_ACTION);
      اكسر;
    حالة ڪوڊ_PROPS_قاعدو_ACTION:
      self->ڪوڊ = translate_action (self, SC_قاعدو_ACTION);
      اكسر;
    }
}

فراغ
ڪوڊ_scanner_آخري_string_free (فراغ)
{
  STRING_FREE ();
}

فراغ
ڪوڊ_scanner_init (فراغ)
{
  obstack_init (&obstack_for_string);
  yy_flex_debug = 0;
}

فراغ
ڪوڊ_scanner_free (فراغ)
{
  obstack_free (&obstack_for_string, 0);
  variant_جدول_free ();

  /* Reclaim Flex's buffers.  */
  yylex_destroy ();
}
