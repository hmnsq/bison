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
%option اگیتر="code_" outfile="lex.yy.c"

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

#تعریف_کرو FLEX_اگیتر(Id) کوڈ_2 ## Id
#ضمن "src/flex-scanner.h"

/* Work around a bug in flex 2.5.31.  See Debian bug 333231
   <https://bugs.debian.org/333231>.  */
#تعریف_رد کوڈ_wrap
#تعریف_کرو کوڈ_wrap() 1

بنية obstack *obstack_for_actions = &obstack_for_string;

/* The current calling start condition: SC_RULE_ACTION or
   SC_SYMBOL_ACTION. */
# define YY_DECL ساكن حرف *کوڈ_lex (کوڈ_props *self, صحيح sc_context)
YY_DECL;

#تعریف_کرو YY_USER_ACTION  location_compute (loc, &loc->سرا, yytext, yyleng);

ساكن حرف *fetch_type_name (حرف *cp, حرف ثابت **type_name,
                              ثابت location *dollar_loc);

ساكن فراغ handle_action_dollar (علامت_list *قاعدہ, حرف *cp,
                                  ثابت location *dollar_loc);
ساكن فراغ handle_action_at (علامت_list *قاعدہ, حرف *cp,
                              ثابت location *at_loc);

/* A string to be pushed to obstack after dollar/at has been handled. */
ساكن حرف *ref_پوچھ_fields;

ساكن location current_loc;
ساكن location *loc = &current_loc;

/* A string representing the most recent translation.  */
ساكن حرف *آخری_string;

/* True if an untyped $$ or $n was seen.  */
ساكن bool untyped_var_تکیا;

%}
 /* C and C++ comments in code. */
%x SC_COMMENT SC_سطر_COMMENT
 /* Strings and characters in code. */
%x SC_STRING SC_CHARACTER
 /* Whether in a rule or symbol action.  Specifies the translation
    of $ and @.  */
%x SC_قاعدہ_ACTION SC_علامت_ACTION


/* POSIX says that a tag must be both an id and a C union member, but
   historically almost any character is allowed in a tag.  We disallow
   NUL and newline, as this simplifies our implementation.  We allow
   "->" as a means to dereference a pointer.  */
ٹیگ      ([^\0\n>]|->)*[^-]

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
  aver (sc_context == SC_علامت_ACTION
        || sc_context == SC_قاعدہ_ACTION
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

<SC_سطر_COMMENT>
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


<SC_قاعدہ_ACTION,SC_علامت_ACTION>
{
  "'"              STRING_GROW (); BEGIN SC_CHARACTER;
  "\""             STRING_GROW (); BEGIN SC_STRING;
  "/"{splice}"*"   STRING_GROW (); BEGIN SC_COMMENT;
  "/"{splice}"/"   STRING_GROW (); BEGIN SC_سطر_COMMENT;

  [$@]  {
    complain (loc, Wother, _("stray '%s'"), yytext);
    obstack_escape (&obstack_for_string, yytext);
  }
}

<SC_قاعدہ_ACTION>
{
  "$"("<"{ٹیگ}">")?{ref}  {
    ref_پوچھ_fields = عدم;
    handle_action_dollar (self->قاعدہ, yytext, loc);
    إذا (ref_پوچھ_fields)
      obstack_sgrow (&obstack_for_string, ref_پوچھ_fields);
  }
  "@"{ref} {
    ref_پوچھ_fields = عدم;
    handle_action_at (self->قاعدہ, yytext, loc);
    إذا (ref_پوچھ_fields)
      obstack_sgrow (&obstack_for_string, ref_پوچھ_fields);
  }
}

<SC_علامت_ACTION>
{
  "$"("<"{ٹیگ}">")?"$" {
    ثابت حرف *type_name = عدم;
    fetch_type_name (yytext + 1, &type_name, loc)[-1] = 0;
    obstack_sgrow (&obstack_for_string, "]b4_dollar_dollar(");
    obstack_quote (&obstack_for_string, type_name);
    obstack_sgrow (&obstack_for_string, ")[");
    self->is_قدر_ورتیا = true;
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
  <<EOF>>     STRING_انت (); أرجع آخری_string;
}

%%

ساكن مضمن bool
is_dot_or_dash (حرف ch)
{
  أرجع ch == '.' || ch == '-';
}

ساكن مضمن bool
شامل_اے_dot_or_dash (ثابت حرف* p)
{
  أرجع !!strpbrk (p, ".-");
}

/* Defines a variant of a symbolic name resolution. */
تعريفنوع بنية
{
  /* Index in symbol list. */
  صحيح علامت_اشاریہ;

  /* Matched symbol id and loc. */
  uniqstr id;
  location loc;

  /* Hiding named reference. */
  named_ref* hidden_by;

  /* Error flags. May contain zero (no errors) or
     a combination of VARIANT_* values. */
  بنانشانی err;
} variant;

/* Set when the variant refers to a symbol hidden
   by an explicit symbol reference. */
#تعریف_کرو VARIANT_HIDDEN (1 << 0)

/* Set when the variant refers to a symbol containing
   dots or dashes. Will require explicit bracketing. */
#تعریف_کرو VARIANT_BAD_BRACKETING (1 << 1)

/* Set when the variant refers to a symbol which is
   not visible from current midrule. */
#تعریف_کرو VARIANT_NOT_VISIBLE_FROM_MIDRULE (1 << 2)

ساكن variant *variant_جدول = عدم;
ساكن صحيح variant_جدول_ناپ = 0;
ساكن صحيح variant_گنتی = 0;

ساكن variant *
variant_جدول_grow (فراغ)
{
  ++variant_گنتی;
  إذا (variant_گنتی > variant_جدول_ناپ)
    {
      طالما (variant_گنتی > variant_جدول_ناپ)
        variant_جدول_ناپ = 2 * variant_جدول_ناپ + 3;
      variant_جدول = xnrealloc (variant_جدول, variant_جدول_ناپ,
                                 حجم *variant_جدول);
    }
  أرجع &variant_جدول[variant_count - 1];
}

ساكن فراغ
variant_جدول_free (فراغ)
{
  حرر (variant_جدول);
  variant_جدول = عدم;
  variant_جدول_ناپ = variant_گنتی = 0;
}

ساكن حرف ثابت *
لبھو_اگیتر_سرا (حرف ثابت *اگیتر, حرف ثابت *cp, حرف ثابت *سرا)
{
  لكل (; *اگیتر && cp != سرا; ++اگیتر, ++cp)
    إذا (*اگیتر != *cp)
      أرجع عدم;

  أرجع *اگیتر ? عدم : cp;
}

ساكن variant *
variant_جوڑو (uniqstr id, location id_loc, صحيح علامت_اشاریہ,
             حرف ثابت *cp, حرف ثابت *cp_سرا, bool explicit_bracketing)
{
  حرف ثابت *اگیتر_سرا = لبھو_اگیتر_سرا (id, cp, cp_سرا);
  إذا (اگیتر_سرا &&
      (اگیتر_سرا == cp_سرا ||
       (!explicit_bracketing && is_dot_or_dash (*اگیتر_سرا))))
    {
      variant *r = variant_جدول_grow ();
      r->علامت_اشاریہ = علامت_اشاریہ;
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
get_at_spec(صحيح علامت_اشاریہ)
{
  ساكن حرف at_buf[20];
  إذا (علامت_اشاریہ == 0)
    strcpy (at_buf, "$$");
  وإلا
    snprintf (at_buf, حجم at_buf, "$%u", علامت_اشاریہ);
  أرجع at_buf;
}

ساكن فراغ
show_sub_سنیہا (warnings warning,
                  ثابت حرف* cp, bool explicit_bracketing,
                  صحيح midrule_rhs_اشاریہ, حرف dollar_or_at,
                  ثابت variant *var)
{
  ثابت حرف *at_spec = get_at_spec (var->علامت_اشاریہ);

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

      ثابت حرف *پوچھ = explicit_bracketing ? "" : cp + لڑی_لمبائی (var->id);

      /* Create the explanation message. */
      ساكن بنية obstack پیغام_buf;
      obstack_init (&پیغام_buf);

      obstack_printf (&پیغام_buf, _("possibly meant: %c"), dollar_or_at);
      إذا (شامل_اے_dot_or_dash (id))
        obstack_printf (&پیغام_buf, "[%s]", id);
      وإلا
        obstack_sgrow (&پیغام_buf, id);
      obstack_sgrow (&پیغام_buf, پوچھ);

      إذا (var->err & VARIANT_HIDDEN)
        {
          obstack_printf (&پیغام_buf, _(", hiding %c"), dollar_or_at);
          إذا (شامل_اے_dot_or_dash (var->id))
            obstack_printf (&پیغام_buf, "[%s]", var->id);
          وإلا
            obstack_sgrow (&پیغام_buf, var->id);
          obstack_sgrow (&پیغام_buf, پوچھ);
        }

      obstack_printf (&پیغام_buf, _(" at %s"), at_spec);

      إذا (var->err & VARIANT_NOT_VISIBLE_FROM_MIDRULE)
        obstack_printf (&پیغام_buf,
                        _(", cannot be accessed from midrule action at $%d"),
                        midrule_rhs_اشاریہ);

      subcomplain (&id_loc, warning, "%s",
                   obstack_finish0 (&پیغام_buf));
      obstack_free (&پیغام_buf, 0);
    }
}

ساكن فراغ
show_sub_messages (warnings warning,
                   ثابت حرف* cp, bool explicit_bracketing,
                   صحيح midrule_rhs_اشاریہ, حرف dollar_or_at)
{
  لكل (صحيح i = 0; i < variant_گنتی; ++i)
    show_sub_سنیہا (warning | silent,
                      cp, explicit_bracketing,
                      midrule_rhs_اشاریہ, dollar_or_at,
                      &variant_جدول[i]);
}

/* Returned from "parse_ref" when the reference
   is inappropriate. */
#تعریف_کرو INVALID_REF (INT_MIN)

/* Returned from "parse_ref" when the reference
   points to LHS ($$) of the current rule or midrule. */
#تعریف_کرو LHS_REF (INT_MIN + 1)

/* Parse a positional reference in RULE.  */
ساكن لما
parse_positional_ref (حرف *cp, صحيح قاعدہ_لمائی,
                      حرف *متن, ثابت location *متن_loc)
{
  لما عدد = strtol (cp, &cp, 10);
  إذا (1 - ودھ_توں_ودھ_عدد + قاعدہ_لمائی <= عدد && عدد <= قاعدہ_لمائی)
    أرجع عدد;
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
ساكن لما
parse_ref (حرف *cp, علامت_list *قاعدہ, صحيح قاعدہ_لمائی,
           صحيح midrule_rhs_اشاریہ, حرف *متن, ثابت location *متن_loc,
           حرف dollar_or_at)
{
  إذا ('$' == *cp)
    أرجع LHS_REF;

  إذا (c_isdigit (*cp) || (*cp == '-' && c_isdigit (cp[1])))
    أرجع parse_positional_ref (cp, قاعدہ_لمائی, متن, متن_loc);

  bool ثابت explicit_bracketing = *cp == '[';

  إذا (explicit_bracketing)
    ++cp;
  وإلا
    ref_پوچھ_fields = strpbrk (cp, ".-");

  حرف ثابت *cp_سرا = strchr (cp, explicit_bracketing ? ']' : '\0');

  /* Add all relevant variants. */
  {
    صحيح علامت_اشاریہ;
    علامت_list *l;
    variant_گنتی = 0;
    لكل (علامت_اشاریہ = 0, l = قاعدہ; !علامت_list_null (l);
         ++علامت_اشاریہ, l = l->اگلا)
      {
        إذا (l->content_type != SYMLIST_علامت)
          استمر;

        variant *var
          = variant_جوڑو (l->content.sym->ٹیگ, l->sym_loc,
                         علامت_اشاریہ, cp, cp_سرا, explicit_bracketing);
        إذا (var && l->named_ref)
          var->hidden_by = l->named_ref;

        إذا (l->named_ref)
          variant_جوڑو (l->named_ref->id, l->named_ref->loc,
                       علامت_اشاریہ, cp, cp_سرا, explicit_bracketing);
      }
  }

  /* Check errors. */
  صحيح valid_variants = 0;
  صحيح valid_variant_اشاریہ = 0;
  لكل (صحيح i = 0; i < variant_گنتی; ++i)
    {
      variant *var = &variant_جدول[i];
      صحيح علامت_اشاریہ = var->علامت_اشاریہ;

      /* Check visibility from midrule actions. */
      إذا (midrule_rhs_اشاریہ != 0
          && (علامت_اشاریہ == 0 || midrule_rhs_اشاریہ < علامت_اشاریہ))
        var->err |= VARIANT_NOT_VISIBLE_FROM_MIDRULE;

      /* Check correct bracketing. */
      إذا (!explicit_bracketing && شامل_اے_dot_or_dash (var->id))
        var->err |= VARIANT_BAD_BRACKETING;

      /* Check using of hidden symbols. */
      إذا (var->hidden_by)
        var->err |= VARIANT_HIDDEN;

      إذا (!var->err)
        {
          valid_variant_اشاریہ = i;
          ++valid_variants;
        }
    }

  بدل (valid_variants)
    {
    حالة 0:
      {
        صحيح len = (explicit_bracketing || !ref_پوچھ_fields) ?
          cp_سرا - cp : ref_پوچھ_fields - cp;

        complain (متن_loc, complaint,
                  _("invalid reference: %s"), quote (متن));
        إذا (len == 0)
          {
            location sym_loc = *متن_loc;
            sym_loc.شروع.کالم += 1;
            sym_loc.سرا = sym_loc.شروع;
            subcomplain (&sym_loc, complaint,
                         _("syntax error after '%c', expecting integer, "
                         "letter, '_', '[', or '$'"),
                         dollar_or_at);
          }
        وإلا إذا (midrule_rhs_اشاریہ)
          subcomplain (&قاعدہ->rhs_loc, complaint,
                       _("symbol not found in production before $%d: "
                       "%.*s"),
                       midrule_rhs_اشاریہ, len, cp);
        وإلا
          subcomplain (&قاعدہ->rhs_loc, complaint,
                       _("symbol not found in production: %.*s"),
                       len, cp);

        إذا (variant_گنتی > 0)
          show_sub_messages (complaint,
                             cp, explicit_bracketing, midrule_rhs_اشاریہ,
                             dollar_or_at);
        أرجع INVALID_REF;
      }
    حالة 1:
      {
        إذا (variant_گنتی > 1)
          {
            complain (متن_loc, Wother,
                      _("misleading reference: %s"), quote (متن));
            show_sub_messages (Wother,
                               cp, explicit_bracketing, midrule_rhs_اشاریہ,
                               dollar_or_at);
          }
        {
          صحيح علامت_اشاریہ =
            variant_جدول[valid_variant_index].علامت_اشاریہ;
          أرجع (علامت_اشاریہ == midrule_rhs_اشاریہ) ? LHS_REF : علامت_اشاریہ;
        }
      }
    حالة 2:
    افتراضي:
      {
        complain (متن_loc, complaint,
                  _("ambiguous reference: %s"), quote (متن));
        show_sub_messages (complaint,
                           cp, explicit_bracketing, midrule_rhs_اشاریہ,
                           dollar_or_at);
        أرجع INVALID_REF;
      }
    }
}

/* Keeps track of the maximum number of semantic values to the left of
   a handle (those referenced by $0, $-1, etc.) are required by the
   semantic actions of this grammar. */
صحيح max_بایاں_semantic_context = 0;


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
      إذا (untyped_var_تکیا)
        complain (dollar_loc, complaint,
                  _("explicit type given in untyped grammar"));
      ٹیگ_تکیا = true;
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
handle_action_dollar (علامت_list *قاعدہ, حرف *متن, ثابت location *dollar_loc)
{
  علامت_list *effective_قاعدہ;
  صحيح effective_قاعدہ_لمائی;

  إذا (قاعدہ->midrule_ماپا_قاعدہ)
    {
      effective_قاعدہ = قاعدہ->midrule_ماپا_قاعدہ;
      effective_قاعدہ_لمائی = قاعدہ->midrule_ماپا_rhs_اشاریہ - 1;
    }
  وإلا
    {
      effective_قاعدہ = قاعدہ;
      effective_قاعدہ_لمائی = علامت_list_لمائی (قاعدہ->اگلا);
    }

  /* The type name if explicit, otherwise left null. */
  حرف ثابت *type_name = عدم;
  حرف *cp = fetch_type_name (متن + 1, &type_name, dollar_loc);
  صحيح n = parse_ref (cp, effective_قاعدہ, effective_قاعدہ_لمائی,
                     قاعدہ->midrule_ماپا_rhs_اشاریہ, متن, dollar_loc, '$');
  /* End type_name.  Don't do it earlier: parse_ref depends on TEXT.  */
  إذا (type_name)
    cp[-1] = '\0';

  بدل (n)
    {
    حالة INVALID_REF:
      اكسر;

    حالة LHS_REF:
      {
        علامت_list *sym = علامت_list_n_get (قاعدہ, 0);
        إذا (!type_name
            && !sym->content.sym->content->type_name)
          {
            إذا (union_تکیا || ٹیگ_تکیا)
              {
                إذا (قاعدہ->midrule_ماپا_قاعدہ)
                  complain (dollar_loc, complaint,
                            _("$$ for the midrule at $%d of %s"
                              " has no declared type"),
                            قاعدہ->midrule_ماپا_rhs_اشاریہ,
                            quote (effective_قاعدہ->content.sym->ٹیگ));
                وإلا
                  complain (dollar_loc, complaint,
                            _("$$ of %s has no declared type"),
                            quote (قاعدہ->content.sym->ٹیگ));
              }
            وإلا
              untyped_var_تکیا = true;
          }

        obstack_printf (&obstack_for_string, "]b4_lhs_value(orig %d, ",
                        sym->content.sym->content->نمبر);
        obstack_quote (&obstack_for_string, type_name);
        obstack_sgrow (&obstack_for_string, ")[");
        قاعدہ->action_props.is_قدر_ورتیا = true;
      }
      اكسر;

      /* Reference to a RHS symbol.  */
    افتراضي:
      {
        إذا (max_بایاں_semantic_context < 1 - n)
          max_بایاں_semantic_context = 1 - n;
        علامت_list *sym = 0 < n ? علامت_list_n_get (effective_قاعدہ, n) : عدم;
        إذا (!type_name
            && (!sym || !sym->content.sym->content->type_name))
          {
            إذا (union_تکیا || ٹیگ_تکیا)
              complain (dollar_loc, complaint,
                        _("$%s of %s has no declared type"), cp,
                        quote (effective_قاعدہ->content.sym->ٹیگ));
            وإلا
              untyped_var_تکیا = true;
          }

        obstack_printf (&obstack_for_string,
                        "]b4_rhs_value(%d, %d, ",
                        effective_قاعدہ_لمائی, n);
        إذا (sym)
          obstack_printf (&obstack_for_string, "%s%d, ",
                          sym->content.sym->content->class == nterm_sym ? "orig " : "",
                          sym->content.sym->content->نمبر);
        وإلا
          obstack_sgrow (&obstack_for_string, "[], ");

        obstack_quote (&obstack_for_string, type_name);
        obstack_sgrow (&obstack_for_string, ")[");
        إذا (0 < n)
          {
            إذا (muscle_percent_define_ifdef ("api.value.automove")
                && sym->action_props.is_قدر_ورتیا)
              complain (dollar_loc, Wother,
                        _("multiple occurrences of $%d with api.value.automove"),
                        n);
            sym->action_props.is_قدر_ورتیا = true;
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
handle_action_at (علامت_list *قاعدہ, حرف *متن, ثابت location *at_loc)
{
  علامت_list *effective_قاعدہ;
  صحيح effective_قاعدہ_لمائی;

  إذا (قاعدہ->midrule_ماپا_قاعدہ)
    {
      effective_قاعدہ = قاعدہ->midrule_ماپا_قاعدہ;
      effective_قاعدہ_لمائی = قاعدہ->midrule_ماپا_rhs_اشاریہ - 1;
    }
  وإلا
    {
      effective_قاعدہ = قاعدہ;
      effective_قاعدہ_لمائی = علامت_list_لمائی (قاعدہ->اگلا);
    }

  muscle_percent_define_ensure ("locations", *at_loc, true);

  صحيح n = parse_ref (متن + 1, effective_قاعدہ, effective_قاعدہ_لمائی,
                     قاعدہ->midrule_ماپا_rhs_اشاریہ, متن, at_loc, '@');
  بدل (n)
    {
    حالة INVALID_REF:
      اكسر;

    حالة LHS_REF:
      obstack_sgrow (&obstack_for_string, "]b4_lhs_location[");
      اكسر;

    افتراضي:
      obstack_printf (&obstack_for_string, "]b4_rhs_location(%d, %d)[",
                      effective_قاعدہ_لمائی, n);
      اكسر;
    }
}


/*-------------------------.
| Initialize the scanner.  |
`-------------------------*/

/* Translate the '$...' and '@...' in \a self, in the context \a
   sc_context (SC_RULE_ACTION, SC_SYMBOL_ACTION, INITIAL).  */

ساكن حرف ثابت *
translate_action (کوڈ_props *self, صحيح sc_context)
{
  loc->شروع = loc->سرا = self->location.شروع;
  yy_switch_منزل_بفر (yy_جانچ_string (self->کوڈ));
  حرف *res = کوڈ_lex (self, sc_context);
  yy_delete_بفر (YY_CURRENT_بفر);
  أرجع res;
}

/*------------------------------------------------------------------------.
| Implementation of the public interface as documented in "scan-code.h".  |
`------------------------------------------------------------------------*/

فراغ
کوڈ_props_none_init (کوڈ_props *self)
{
  *self = کوڈ_props_none;
}

کوڈ_props کوڈ_props_none = کوڈ_PROPS_NONE_INIT;

فراغ
کوڈ_props_plain_init (کوڈ_props *self, حرف ثابت *کوڈ,
                       location کوڈ_loc)
{
  کوڈ_props_none_init (self);
  self->قسم = کوڈ_PROPS_PLAIN;
  self->کوڈ = کوڈ;
  self->location = کوڈ_loc;
}

فراغ
کوڈ_props_علامت_action_init (کوڈ_props *self, حرف ثابت *کوڈ,
                               location کوڈ_loc)
{
  کوڈ_props_none_init (self);
  self->قسم = کوڈ_PROPS_علامت_ACTION;
  self->کوڈ = کوڈ;
  self->location = کوڈ_loc;
}

فراغ
کوڈ_props_قاعدہ_action_init (کوڈ_props *self, حرف ثابت *کوڈ,
                             location کوڈ_loc, علامت_list *قاعدہ,
                             named_ref *name, uniqstr type,
                             bool is_predicate)
{
  کوڈ_props_none_init (self);
  self->قسم = کوڈ_PROPS_قاعدہ_ACTION;
  self->کوڈ = کوڈ;
  self->location = کوڈ_loc;
  self->قاعدہ = قاعدہ;
  self->named_ref = name;
  self->type = type;
  self->is_predicate = is_predicate;
}

فراغ
کوڈ_props_translate_کوڈ (کوڈ_props *self)
{
  بدل (self->قسم)
    {
    حالة کوڈ_PROPS_NONE:
      اكسر;
    حالة کوڈ_PROPS_PLAIN:
      self->کوڈ = translate_action (self, INITIAL);
      اكسر;
    حالة کوڈ_PROPS_علامت_ACTION:
      self->کوڈ = translate_action (self, SC_علامت_ACTION);
      اكسر;
    حالة کوڈ_PROPS_قاعدہ_ACTION:
      self->کوڈ = translate_action (self, SC_قاعدہ_ACTION);
      اكسر;
    }
}

فراغ
کوڈ_scanner_آخری_string_free (فراغ)
{
  STRING_FREE ();
}

فراغ
کوڈ_scanner_init (فراغ)
{
  obstack_init (&obstack_for_string);
  yy_flex_debug = 0;
}

فراغ
کوڈ_scanner_free (فراغ)
{
  obstack_free (&obstack_for_string, 0);
  variant_جدول_free ();

  /* Reclaim Flex's buffers.  */
  yylex_destroy ();
}
