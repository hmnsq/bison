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
%option prefix="code_" outfile="lex.yy.c"

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

#define FLEX_PREFIX(Id) code_ ## Id
#ضمن "src/flex-scanner.h"

/* Work around a bug in flex 2.5.31.  See Debian bug 333231
   <https://bugs.debian.org/333231>.  */
#undef code_wrap
#define code_wrap() 1

بنية obstack *obstack_for_actions = &obstack_for_string;

/* The current calling start condition: SC_RULE_ACTION or
   SC_SYMBOL_ACTION. */
# define YY_DECL ساكن حرف *code_lex (code_props *self, صحيح sc_context)
YY_DECL;

#define YY_USER_ACTION  location_compute (loc, &loc->end, yytext, yyleng);

ساكن حرف *fetch_type_name (حرف *cp, حرف ثابت **type_name,
                              ثابت location *dollar_loc);

ساكن فراغ handle_action_dollar (علام_list *rule, حرف *cp,
                                  ثابت location *dollar_loc);
ساكن فراغ handle_action_at (علام_list *rule, حرف *cp,
                              ثابت location *at_loc);

/* A string to be pushed to obstack after dollar/at has been handled. */
ساكن حرف *ref_tail_fields;

ساكن location current_loc;
ساكن location *loc = &current_loc;

/* A string representing the most recent translation.  */
ساكن حرف *last_string;

/* True if an untyped $$ or $n was seen.  */
ساكن bool untyped_var_seen;

%}
 /* C and C++ comments in code. */
%x SC_COMMENT SC_لي_COMMENT
 /* Strings and characters in code. */
%x SC_STRING SC_CHARACTER
 /* Whether in a rule or symbol action.  Specifies the translation
    of $ and @.  */
%x SC_RULE_ACTION SC_علام_ACTION


/* POSIX says that a tag must be both an id and a C union member, but
   historically almost any character is allowed in a tag.  We disallow
   NUL and newline, as this simplifies our implementation.  We allow
   "->" as a means to dereference a pointer.  */
tag      ([^\0\n>]|->)*[^-]

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
  aver (sc_context == SC_علام_ACTION
        || sc_context == SC_RULE_ACTION
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

<SC_لي_COMMENT>
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


<SC_RULE_ACTION,SC_علام_ACTION>
{
  "'"              STRING_GROW (); BEGIN SC_CHARACTER;
  "\""             STRING_GROW (); BEGIN SC_STRING;
  "/"{splice}"*"   STRING_GROW (); BEGIN SC_COMMENT;
  "/"{splice}"/"   STRING_GROW (); BEGIN SC_لي_COMMENT;

  [$@]  {
    complain (loc, Wother, _("stray '%s'"), yytext);
    obstack_escape (&obstack_for_string, yytext);
  }
}

<SC_RULE_ACTION>
{
  "$"("<"{tag}">")?{ref}  {
    ref_tail_fields = عدم;
    handle_action_dollar (self->rule, yytext, loc);
    إذا (ref_tail_fields)
      obstack_sgrow (&obstack_for_string, ref_tail_fields);
  }
  "@"{ref} {
    ref_tail_fields = عدم;
    handle_action_at (self->rule, yytext, loc);
    إذا (ref_tail_fields)
      obstack_sgrow (&obstack_for_string, ref_tail_fields);
  }
}

<SC_علام_ACTION>
{
  "$"("<"{tag}">")?"$" {
    ثابت حرف *type_name = عدم;
    fetch_type_name (yytext + 1, &type_name, loc)[-1] = 0;
    obstack_sgrow (&obstack_for_string, "]b4_dollar_dollar(");
    obstack_quote (&obstack_for_string, type_name);
    obstack_sgrow (&obstack_for_string, ")[");
    self->is_درج_used = true;
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
  <<EOF>>     STRING_FINISH (); أرجع last_string;
}

%%

ساكن مضمن bool
is_dot_or_dash (حرف ch)
{
  أرجع ch == '.' || ch == '-';
}

ساكن مضمن bool
contains_dot_or_dash (ثابت حرف* p)
{
  أرجع !!strpbrk (p, ".-");
}

/* Defines a variant of a symbolic name resolution. */
تعريفنوع بنية
{
  /* Index in symbol list. */
  صحيح علام_index;

  /* Matched symbol id and loc. */
  uniqstr id;
  location loc;

  /* Hiding named reference. */
  named_ref* hidden_by;

  /* Error flags. May contain zero (no errors) or
     a combination of VARIANT_* values. */
  unsigned err;
} variant;

/* Set when the variant refers to a symbol hidden
   by an explicit symbol reference. */
#define VARIANT_HIDDEN (1 << 0)

/* Set when the variant refers to a symbol containing
   dots or dashes. Will require explicit bracketing. */
#define VARIANT_BAD_BRACKETING (1 << 1)

/* Set when the variant refers to a symbol which is
   not visible from current midrule. */
#define VARIANT_NOT_VISIBLE_FROM_MIDRULE (1 << 2)

ساكن variant *variant_table = عدم;
ساكن صحيح variant_table_size = 0;
ساكن صحيح variant_كدي = 0;

ساكن variant *
variant_table_grow (فراغ)
{
  ++variant_كدي;
  إذا (variant_كدي > variant_table_size)
    {
      طالما (variant_كدي > variant_table_size)
        variant_table_size = 2 * variant_table_size + 3;
      variant_table = xnrealloc (variant_table, variant_table_size,
                                 حجم *variant_table);
    }
  أرجع &variant_table[variant_count - 1];
}

ساكن فراغ
variant_table_free (فراغ)
{
  حرر (variant_table);
  variant_table = عدم;
  variant_table_size = variant_كدي = 0;
}

ساكن حرف ثابت *
find_prefix_end (حرف ثابت *prefix, حرف ثابت *cp, حرف ثابت *end)
{
  لكل (; *prefix && cp != end; ++prefix, ++cp)
    إذا (*prefix != *cp)
      أرجع عدم;

  أرجع *prefix ? عدم : cp;
}

ساكن variant *
variant_add (uniqstr id, location id_loc, صحيح علام_index,
             حرف ثابت *cp, حرف ثابت *cp_end, bool explicit_bracketing)
{
  حرف ثابت *prefix_end = find_prefix_end (id, cp, cp_end);
  إذا (prefix_end &&
      (prefix_end == cp_end ||
       (!explicit_bracketing && is_dot_or_dash (*prefix_end))))
    {
      variant *r = variant_table_grow ();
      r->علام_index = علام_index;
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
get_at_spec(صحيح علام_index)
{
  ساكن حرف at_buf[20];
  إذا (علام_index == 0)
    strcpy (at_buf, "$$");
  وإلا
    snprintf (at_buf, حجم at_buf, "$%u", علام_index);
  أرجع at_buf;
}

ساكن فراغ
show_sub_message (warnings warning,
                  ثابت حرف* cp, bool explicit_bracketing,
                  صحيح midrule_rhs_index, حرف dollar_or_at,
                  ثابت variant *var)
{
  ثابت حرف *at_spec = get_at_spec (var->علام_index);

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

      ثابت حرف *tail = explicit_bracketing ? "" : cp + strlen (var->id);

      /* Create the explanation message. */
      ساكن بنية obstack سكو_buf;
      obstack_init (&سكو_buf);

      obstack_printf (&سكو_buf, _("possibly meant: %c"), dollar_or_at);
      إذا (contains_dot_or_dash (id))
        obstack_printf (&سكو_buf, "[%s]", id);
      وإلا
        obstack_sgrow (&سكو_buf, id);
      obstack_sgrow (&سكو_buf, tail);

      إذا (var->err & VARIANT_HIDDEN)
        {
          obstack_printf (&سكو_buf, _(", hiding %c"), dollar_or_at);
          إذا (contains_dot_or_dash (var->id))
            obstack_printf (&سكو_buf, "[%s]", var->id);
          وإلا
            obstack_sgrow (&سكو_buf, var->id);
          obstack_sgrow (&سكو_buf, tail);
        }

      obstack_printf (&سكو_buf, _(" at %s"), at_spec);

      إذا (var->err & VARIANT_NOT_VISIBLE_FROM_MIDRULE)
        obstack_printf (&سكو_buf,
                        _(", cannot be accessed from midrule action at $%d"),
                        midrule_rhs_index);

      subcomplain (&id_loc, warning, "%s",
                   obstack_finish0 (&سكو_buf));
      obstack_free (&سكو_buf, 0);
    }
}

ساكن فراغ
show_sub_messages (warnings warning,
                   ثابت حرف* cp, bool explicit_bracketing,
                   صحيح midrule_rhs_index, حرف dollar_or_at)
{
  لكل (صحيح i = 0; i < variant_كدي; ++i)
    show_sub_message (warning | silent,
                      cp, explicit_bracketing,
                      midrule_rhs_index, dollar_or_at,
                      &variant_table[i]);
}

/* Returned from "parse_ref" when the reference
   is inappropriate. */
#define INVALID_REF (INT_MIN)

/* Returned from "parse_ref" when the reference
   points to LHS ($$) of the current rule or midrule. */
#define LHS_REF (INT_MIN + 1)

/* Parse a positional reference in RULE.  */
ساكن long
parse_positional_ref (حرف *cp, صحيح rule_length,
                      حرف *text, ثابت location *text_loc)
{
  long لمب = strtol (cp, &cp, 10);
  إذا (1 - INT_MAX + rule_length <= لمب && لمب <= rule_length)
    أرجع لمب;
  وإلا
    {
      complain (text_loc, complaint, _("integer out of range: %s"),
                quote (text));
      أرجع INVALID_REF;
    }
}


/* Parse named or positional reference. In case of positional
   references, can return negative values for $-n "deep" stack
   accesses. */
ساكن long
parse_ref (حرف *cp, علام_list *rule, صحيح rule_length,
           صحيح midrule_rhs_index, حرف *text, ثابت location *text_loc,
           حرف dollar_or_at)
{
  إذا ('$' == *cp)
    أرجع LHS_REF;

  إذا (c_isdigit (*cp) || (*cp == '-' && c_isdigit (cp[1])))
    أرجع parse_positional_ref (cp, rule_length, text, text_loc);

  bool ثابت explicit_bracketing = *cp == '[';

  إذا (explicit_bracketing)
    ++cp;
  وإلا
    ref_tail_fields = strpbrk (cp, ".-");

  حرف ثابت *cp_end = strchr (cp, explicit_bracketing ? ']' : '\0');

  /* Add all relevant variants. */
  {
    صحيح علام_index;
    علام_list *l;
    variant_كدي = 0;
    لكل (علام_index = 0, l = rule; !علام_list_null (l);
         ++علام_index, l = l->next)
      {
        إذا (l->content_type != SYMLIST_علام)
          استمر;

        variant *var
          = variant_add (l->content.sym->tag, l->sym_loc,
                         علام_index, cp, cp_end, explicit_bracketing);
        إذا (var && l->named_ref)
          var->hidden_by = l->named_ref;

        إذا (l->named_ref)
          variant_add (l->named_ref->id, l->named_ref->loc,
                       علام_index, cp, cp_end, explicit_bracketing);
      }
  }

  /* Check errors. */
  صحيح valid_variants = 0;
  صحيح valid_variant_index = 0;
  لكل (صحيح i = 0; i < variant_كدي; ++i)
    {
      variant *var = &variant_table[i];
      صحيح علام_index = var->علام_index;

      /* Check visibility from midrule actions. */
      إذا (midrule_rhs_index != 0
          && (علام_index == 0 || midrule_rhs_index < علام_index))
        var->err |= VARIANT_NOT_VISIBLE_FROM_MIDRULE;

      /* Check correct bracketing. */
      إذا (!explicit_bracketing && contains_dot_or_dash (var->id))
        var->err |= VARIANT_BAD_BRACKETING;

      /* Check using of hidden symbols. */
      إذا (var->hidden_by)
        var->err |= VARIANT_HIDDEN;

      إذا (!var->err)
        {
          valid_variant_index = i;
          ++valid_variants;
        }
    }

  بدل (valid_variants)
    {
    حالة 0:
      {
        صحيح len = (explicit_bracketing || !ref_tail_fields) ?
          cp_end - cp : ref_tail_fields - cp;

        complain (text_loc, complaint,
                  _("invalid reference: %s"), quote (text));
        إذا (len == 0)
          {
            location sym_loc = *text_loc;
            sym_loc.start.column += 1;
            sym_loc.end = sym_loc.start;
            subcomplain (&sym_loc, complaint,
                         _("syntax error after '%c', expecting integer, "
                         "letter, '_', '[', or '$'"),
                         dollar_or_at);
          }
        وإلا إذا (midrule_rhs_index)
          subcomplain (&rule->rhs_loc, complaint,
                       _("symbol not found in production before $%d: "
                       "%.*s"),
                       midrule_rhs_index, len, cp);
        وإلا
          subcomplain (&rule->rhs_loc, complaint,
                       _("symbol not found in production: %.*s"),
                       len, cp);

        إذا (variant_كدي > 0)
          show_sub_messages (complaint,
                             cp, explicit_bracketing, midrule_rhs_index,
                             dollar_or_at);
        أرجع INVALID_REF;
      }
    حالة 1:
      {
        إذا (variant_كدي > 1)
          {
            complain (text_loc, Wother,
                      _("misleading reference: %s"), quote (text));
            show_sub_messages (Wother,
                               cp, explicit_bracketing, midrule_rhs_index,
                               dollar_or_at);
          }
        {
          صحيح علام_index =
            variant_table[valid_variant_index].علام_index;
          أرجع (علام_index == midrule_rhs_index) ? LHS_REF : علام_index;
        }
      }
    حالة 2:
    افتراضي:
      {
        complain (text_loc, complaint,
                  _("ambiguous reference: %s"), quote (text));
        show_sub_messages (complaint,
                           cp, explicit_bracketing, midrule_rhs_index,
                           dollar_or_at);
        أرجع INVALID_REF;
      }
    }
}

/* Keeps track of the maximum number of semantic values to the left of
   a handle (those referenced by $0, $-1, etc.) are required by the
   semantic actions of this grammar. */
صحيح max_هغو_semantic_context = 0;


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
      إذا (untyped_var_seen)
        complain (dollar_loc, complaint,
                  _("explicit type given in untyped grammar"));
      tag_seen = true;
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
handle_action_dollar (علام_list *rule, حرف *text, ثابت location *dollar_loc)
{
  علام_list *effective_rule;
  صحيح effective_rule_length;

  إذا (rule->midrule_parent_rule)
    {
      effective_rule = rule->midrule_parent_rule;
      effective_rule_length = rule->midrule_parent_rhs_index - 1;
    }
  وإلا
    {
      effective_rule = rule;
      effective_rule_length = علام_list_length (rule->next);
    }

  /* The type name if explicit, otherwise left null. */
  حرف ثابت *type_name = عدم;
  حرف *cp = fetch_type_name (text + 1, &type_name, dollar_loc);
  صحيح n = parse_ref (cp, effective_rule, effective_rule_length,
                     rule->midrule_parent_rhs_index, text, dollar_loc, '$');
  /* End type_name.  Don't do it earlier: parse_ref depends on TEXT.  */
  إذا (type_name)
    cp[-1] = '\0';

  بدل (n)
    {
    حالة INVALID_REF:
      اكسر;

    حالة LHS_REF:
      {
        علام_list *sym = علام_list_n_get (rule, 0);
        إذا (!type_name
            && !sym->content.sym->content->type_name)
          {
            إذا (union_seen || tag_seen)
              {
                إذا (rule->midrule_parent_rule)
                  complain (dollar_loc, complaint,
                            _("$$ for the midrule at $%d of %s"
                              " has no declared type"),
                            rule->midrule_parent_rhs_index,
                            quote (effective_rule->content.sym->tag));
                وإلا
                  complain (dollar_loc, complaint,
                            _("$$ of %s has no declared type"),
                            quote (rule->content.sym->tag));
              }
            وإلا
              untyped_var_seen = true;
          }

        obstack_printf (&obstack_for_string, "]b4_lhs_value(orig %d, ",
                        sym->content.sym->content->لمب_2);
        obstack_quote (&obstack_for_string, type_name);
        obstack_sgrow (&obstack_for_string, ")[");
        rule->action_props.is_درج_used = true;
      }
      اكسر;

      /* Reference to a RHS symbol.  */
    افتراضي:
      {
        إذا (max_هغو_semantic_context < 1 - n)
          max_هغو_semantic_context = 1 - n;
        علام_list *sym = 0 < n ? علام_list_n_get (effective_rule, n) : عدم;
        إذا (!type_name
            && (!sym || !sym->content.sym->content->type_name))
          {
            إذا (union_seen || tag_seen)
              complain (dollar_loc, complaint,
                        _("$%s of %s has no declared type"), cp,
                        quote (effective_rule->content.sym->tag));
            وإلا
              untyped_var_seen = true;
          }

        obstack_printf (&obstack_for_string,
                        "]b4_rhs_value(%d, %d, ",
                        effective_rule_length, n);
        إذا (sym)
          obstack_printf (&obstack_for_string, "%s%d, ",
                          sym->content.sym->content->class == nterm_sym ? "orig " : "",
                          sym->content.sym->content->لمب_2);
        وإلا
          obstack_sgrow (&obstack_for_string, "[], ");

        obstack_quote (&obstack_for_string, type_name);
        obstack_sgrow (&obstack_for_string, ")[");
        إذا (0 < n)
          {
            إذا (muscle_percent_define_ifdef ("api.value.automove")
                && sym->action_props.is_درج_used)
              complain (dollar_loc, Wother,
                        _("multiple occurrences of $%d with api.value.automove"),
                        n);
            sym->action_props.is_درج_used = true;
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
handle_action_at (علام_list *rule, حرف *text, ثابت location *at_loc)
{
  علام_list *effective_rule;
  صحيح effective_rule_length;

  إذا (rule->midrule_parent_rule)
    {
      effective_rule = rule->midrule_parent_rule;
      effective_rule_length = rule->midrule_parent_rhs_index - 1;
    }
  وإلا
    {
      effective_rule = rule;
      effective_rule_length = علام_list_length (rule->next);
    }

  muscle_percent_define_ensure ("locations", *at_loc, true);

  صحيح n = parse_ref (text + 1, effective_rule, effective_rule_length,
                     rule->midrule_parent_rhs_index, text, at_loc, '@');
  بدل (n)
    {
    حالة INVALID_REF:
      اكسر;

    حالة LHS_REF:
      obstack_sgrow (&obstack_for_string, "]b4_lhs_location[");
      اكسر;

    افتراضي:
      obstack_printf (&obstack_for_string, "]b4_rhs_location(%d, %d)[",
                      effective_rule_length, n);
      اكسر;
    }
}


/*-------------------------.
| Initialize the scanner.  |
`-------------------------*/

/* Translate the '$...' and '@...' in \a self, in the context \a
   sc_context (SC_RULE_ACTION, SC_SYMBOL_ACTION, INITIAL).  */

ساكن حرف ثابت *
translate_action (code_props *self, صحيح sc_context)
{
  loc->start = loc->end = self->location.start;
  yy_switch_to_buffer (yy_scan_string (self->code));
  حرف *res = code_lex (self, sc_context);
  yy_delete_buffer (YY_CURRENT_BUFFER);
  أرجع res;
}

/*------------------------------------------------------------------------.
| Implementation of the public interface as documented in "scan-code.h".  |
`------------------------------------------------------------------------*/

فراغ
code_props_none_init (code_props *self)
{
  *self = code_props_none;
}

code_props code_props_none = CODE_PROPS_NONE_INIT;

فراغ
code_props_plain_init (code_props *self, حرف ثابت *code,
                       location code_loc)
{
  code_props_none_init (self);
  self->kind = CODE_PROPS_PLAIN;
  self->code = code;
  self->location = code_loc;
}

فراغ
code_props_علام_action_init (code_props *self, حرف ثابت *code,
                               location code_loc)
{
  code_props_none_init (self);
  self->kind = CODE_PROPS_علام_ACTION;
  self->code = code;
  self->location = code_loc;
}

فراغ
code_props_rule_action_init (code_props *self, حرف ثابت *code,
                             location code_loc, علام_list *rule,
                             named_ref *name, uniqstr type,
                             bool is_predicate)
{
  code_props_none_init (self);
  self->kind = CODE_PROPS_RULE_ACTION;
  self->code = code;
  self->location = code_loc;
  self->rule = rule;
  self->named_ref = name;
  self->type = type;
  self->is_predicate = is_predicate;
}

فراغ
code_props_translate_code (code_props *self)
{
  بدل (self->kind)
    {
    حالة CODE_PROPS_NONE:
      اكسر;
    حالة CODE_PROPS_PLAIN:
      self->code = translate_action (self, INITIAL);
      اكسر;
    حالة CODE_PROPS_علام_ACTION:
      self->code = translate_action (self, SC_علام_ACTION);
      اكسر;
    حالة CODE_PROPS_RULE_ACTION:
      self->code = translate_action (self, SC_RULE_ACTION);
      اكسر;
    }
}

فراغ
code_scanner_last_string_free (فراغ)
{
  STRING_FREE ();
}

فراغ
code_scanner_init (فراغ)
{
  obstack_init (&obstack_for_string);
  yy_flex_debug = 0;
}

فراغ
code_scanner_free (فراغ)
{
  obstack_free (&obstack_for_string, 0);
  variant_table_free ();

  /* Reclaim Flex's buffers.  */
  yylex_destroy ();
}
