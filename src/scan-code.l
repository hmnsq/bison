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
#شامل <c-ctype.h>
#شامل <get-errno.h>
#شامل <quote.h>

#شامل "src/complain.h"
#شامل "src/getargs.h"
#شامل "src/muscle-tab.h"
#شامل "src/reader.h"
#شامل "src/scan-code.h"
#شامل "src/symlist.h"

#define FLEX_PREFIX(Id) code_ ## Id
#شامل "src/flex-scanner.h"

/* Work around a bug in flex 2.5.31.  See Debian bug 333231
   <https://bugs.debian.org/333231>.  */
#undef code_wrap
#define code_wrap() 1

ساختار obstack *obstack_for_actions = &obstack_for_string;

/* The current calling start condition: SC_RULE_ACTION or
   SC_SYMBOL_ACTION. */
# define YY_DECL ایستا نویسه *code_lex (code_props *self, صحیح sc_context)
YY_DECL;

#define YY_USER_ACTION  location_compute (loc, &loc->end, yytext, yyleng);

ایستا نویسه *fetch_type_name (نویسه *cp, نویسه ثابت **type_name,
                              ثابت location *dollar_loc);

ایستا تهی handle_action_dollar (نماد_list *rule, نویسه *cp,
                                  ثابت location *dollar_loc);
ایستا تهی handle_action_at (نماد_list *rule, نویسه *cp,
                              ثابت location *at_loc);

/* A string to be pushed to obstack after dollar/at has been handled. */
ایستا نویسه *ref_tail_fields;

ایستا location current_loc;
ایستا location *loc = &current_loc;

/* A string representing the most recent translation.  */
ایستا نویسه *last_string;

/* True if an untyped $$ or $n was seen.  */
ایستا bool untyped_var_seen;

%}
 /* C and C++ comments in code. */
%x SC_COMMENT SC_سطر_COMMENT
 /* Strings and characters in code. */
%x SC_STRING SC_CHARACTER
 /* Whether in a rule or symbol action.  Specifies the translation
    of $ and @.  */
%x SC_RULE_ACTION SC_نماد_ACTION


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
  aver (sc_context == SC_نماد_ACTION
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


<SC_RULE_ACTION,SC_نماد_ACTION>
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

<SC_RULE_ACTION>
{
  "$"("<"{tag}">")?{ref}  {
    ref_tail_fields = پوچ;
    handle_action_dollar (self->rule, yytext, loc);
    اگر (ref_tail_fields)
      obstack_sgrow (&obstack_for_string, ref_tail_fields);
  }
  "@"{ref} {
    ref_tail_fields = پوچ;
    handle_action_at (self->rule, yytext, loc);
    اگر (ref_tail_fields)
      obstack_sgrow (&obstack_for_string, ref_tail_fields);
  }
}

<SC_نماد_ACTION>
{
  "$"("<"{tag}">")?"$" {
    ثابت نویسه *type_name = پوچ;
    fetch_type_name (yytext + 1, &type_name, loc)[-1] = 0;
    obstack_sgrow (&obstack_for_string, "]b4_dollar_dollar(");
    obstack_quote (&obstack_for_string, type_name);
    obstack_sgrow (&obstack_for_string, ")[");
    self->is_مقدار_used = true;
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
  <<EOF>>     STRING_FINISH (); بازگشت last_string;
}

%%

ایستا درونخطی bool
is_dot_or_dash (نویسه ch)
{
  بازگشت ch == '.' || ch == '-';
}

ایستا درونخطی bool
contains_dot_or_dash (ثابت نویسه* p)
{
  بازگشت !!strpbrk (p, ".-");
}

/* Defines a variant of a symbolic name resolution. */
تعریفنوع ساختار
{
  /* Index in symbol list. */
  صحیح نماد_index;

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

ایستا variant *variant_table = پوچ;
ایستا صحیح variant_table_size = 0;
ایستا صحیح variant_شمار = 0;

ایستا variant *
variant_table_grow (تهی)
{
  ++variant_شمار;
  اگر (variant_شمار > variant_table_size)
    {
      وقتی (variant_شمار > variant_table_size)
        variant_table_size = 2 * variant_table_size + 3;
      variant_table = xnrealloc (variant_table, variant_table_size,
                                 اندازه *variant_table);
    }
  بازگشت &variant_table[variant_count - 1];
}

ایستا تهی
variant_table_free (تهی)
{
  آزاد (variant_table);
  variant_table = پوچ;
  variant_table_size = variant_شمار = 0;
}

ایستا نویسه ثابت *
find_prefix_end (نویسه ثابت *prefix, نویسه ثابت *cp, نویسه ثابت *end)
{
  برای (; *prefix && cp != end; ++prefix, ++cp)
    اگر (*prefix != *cp)
      بازگشت پوچ;

  بازگشت *prefix ? پوچ : cp;
}

ایستا variant *
variant_add (uniqstr id, location id_loc, صحیح نماد_index,
             نویسه ثابت *cp, نویسه ثابت *cp_end, bool explicit_bracketing)
{
  نویسه ثابت *prefix_end = find_prefix_end (id, cp, cp_end);
  اگر (prefix_end &&
      (prefix_end == cp_end ||
       (!explicit_bracketing && is_dot_or_dash (*prefix_end))))
    {
      variant *r = variant_table_grow ();
      r->نماد_index = نماد_index;
      r->id = id;
      r->loc = id_loc;
      r->hidden_by = پوچ;
      r->err = 0;
      بازگشت r;
    }
  وگرنه
    بازگشت پوچ;
}

ایستا ثابت نویسه *
get_at_spec(صحیح نماد_index)
{
  ایستا نویسه at_buf[20];
  اگر (نماد_index == 0)
    strcpy (at_buf, "$$");
  وگرنه
    snprintf (at_buf, اندازه at_buf, "$%u", نماد_index);
  بازگشت at_buf;
}

ایستا تهی
show_sub_message (warnings warning,
                  ثابت نویسه* cp, bool explicit_bracketing,
                  صحیح midrule_rhs_index, نویسه dollar_or_at,
                  ثابت variant *var)
{
  ثابت نویسه *at_spec = get_at_spec (var->نماد_index);

  اگر (var->err == 0)
    subcomplain (&var->loc, warning,
                 _("refers to: %c%s at %s"), dollar_or_at,
                 var->id, at_spec);
  وگرنه
    {
      ثابت نویسه *id;
      location id_loc;

      اگر (var->hidden_by)
        {
          id = var->hidden_by->id;
          id_loc = var->hidden_by->loc;
        }
      وگرنه
        {
          id = var->id;
          id_loc = var->loc;
        }

      ثابت نویسه *tail = explicit_bracketing ? "" : cp + strlen (var->id);

      /* Create the explanation message. */
      ایستا ساختار obstack پیام_buf;
      obstack_init (&پیام_buf);

      obstack_printf (&پیام_buf, _("possibly meant: %c"), dollar_or_at);
      اگر (contains_dot_or_dash (id))
        obstack_printf (&پیام_buf, "[%s]", id);
      وگرنه
        obstack_sgrow (&پیام_buf, id);
      obstack_sgrow (&پیام_buf, tail);

      اگر (var->err & VARIANT_HIDDEN)
        {
          obstack_printf (&پیام_buf, _(", hiding %c"), dollar_or_at);
          اگر (contains_dot_or_dash (var->id))
            obstack_printf (&پیام_buf, "[%s]", var->id);
          وگرنه
            obstack_sgrow (&پیام_buf, var->id);
          obstack_sgrow (&پیام_buf, tail);
        }

      obstack_printf (&پیام_buf, _(" at %s"), at_spec);

      اگر (var->err & VARIANT_NOT_VISIBLE_FROM_MIDRULE)
        obstack_printf (&پیام_buf,
                        _(", cannot be accessed from midrule action at $%d"),
                        midrule_rhs_index);

      subcomplain (&id_loc, warning, "%s",
                   obstack_finish0 (&پیام_buf));
      obstack_free (&پیام_buf, 0);
    }
}

ایستا تهی
show_sub_messages (warnings warning,
                   ثابت نویسه* cp, bool explicit_bracketing,
                   صحیح midrule_rhs_index, نویسه dollar_or_at)
{
  برای (صحیح i = 0; i < variant_شمار; ++i)
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
ایستا long
parse_positional_ref (نویسه *cp, صحیح rule_length,
                      نویسه *text, ثابت location *text_loc)
{
  long رقم = strtol (cp, &cp, 10);
  اگر (1 - INT_MAX + rule_length <= رقم && رقم <= rule_length)
    بازگشت رقم;
  وگرنه
    {
      complain (text_loc, complaint, _("integer out of range: %s"),
                quote (text));
      بازگشت INVALID_REF;
    }
}


/* Parse named or positional reference. In case of positional
   references, can return negative values for $-n "deep" stack
   accesses. */
ایستا long
parse_ref (نویسه *cp, نماد_list *rule, صحیح rule_length,
           صحیح midrule_rhs_index, نویسه *text, ثابت location *text_loc,
           نویسه dollar_or_at)
{
  اگر ('$' == *cp)
    بازگشت LHS_REF;

  اگر (c_isdigit (*cp) || (*cp == '-' && c_isdigit (cp[1])))
    بازگشت parse_positional_ref (cp, rule_length, text, text_loc);

  bool ثابت explicit_bracketing = *cp == '[';

  اگر (explicit_bracketing)
    ++cp;
  وگرنه
    ref_tail_fields = strpbrk (cp, ".-");

  نویسه ثابت *cp_end = strchr (cp, explicit_bracketing ? ']' : '\0');

  /* Add all relevant variants. */
  {
    صحیح نماد_index;
    نماد_list *l;
    variant_شمار = 0;
    برای (نماد_index = 0, l = rule; !نماد_list_null (l);
         ++نماد_index, l = l->next)
      {
        اگر (l->content_type != SYMLIST_نماد)
          ادامه;

        variant *var
          = variant_add (l->content.sym->tag, l->sym_loc,
                         نماد_index, cp, cp_end, explicit_bracketing);
        اگر (var && l->named_ref)
          var->hidden_by = l->named_ref;

        اگر (l->named_ref)
          variant_add (l->named_ref->id, l->named_ref->loc,
                       نماد_index, cp, cp_end, explicit_bracketing);
      }
  }

  /* Check errors. */
  صحیح valid_variants = 0;
  صحیح valid_variant_index = 0;
  برای (صحیح i = 0; i < variant_شمار; ++i)
    {
      variant *var = &variant_table[i];
      صحیح نماد_index = var->نماد_index;

      /* Check visibility from midrule actions. */
      اگر (midrule_rhs_index != 0
          && (نماد_index == 0 || midrule_rhs_index < نماد_index))
        var->err |= VARIANT_NOT_VISIBLE_FROM_MIDRULE;

      /* Check correct bracketing. */
      اگر (!explicit_bracketing && contains_dot_or_dash (var->id))
        var->err |= VARIANT_BAD_BRACKETING;

      /* Check using of hidden symbols. */
      اگر (var->hidden_by)
        var->err |= VARIANT_HIDDEN;

      اگر (!var->err)
        {
          valid_variant_index = i;
          ++valid_variants;
        }
    }

  انتخاب (valid_variants)
    {
    حالت 0:
      {
        صحیح len = (explicit_bracketing || !ref_tail_fields) ?
          cp_end - cp : ref_tail_fields - cp;

        complain (text_loc, complaint,
                  _("invalid reference: %s"), quote (text));
        اگر (len == 0)
          {
            location sym_loc = *text_loc;
            sym_loc.start.column += 1;
            sym_loc.end = sym_loc.start;
            subcomplain (&sym_loc, complaint,
                         _("syntax error after '%c', expecting integer, "
                         "letter, '_', '[', or '$'"),
                         dollar_or_at);
          }
        وگرنه اگر (midrule_rhs_index)
          subcomplain (&rule->rhs_loc, complaint,
                       _("symbol not found in production before $%d: "
                       "%.*s"),
                       midrule_rhs_index, len, cp);
        وگرنه
          subcomplain (&rule->rhs_loc, complaint,
                       _("symbol not found in production: %.*s"),
                       len, cp);

        اگر (variant_شمار > 0)
          show_sub_messages (complaint,
                             cp, explicit_bracketing, midrule_rhs_index,
                             dollar_or_at);
        بازگشت INVALID_REF;
      }
    حالت 1:
      {
        اگر (variant_شمار > 1)
          {
            complain (text_loc, Wother,
                      _("misleading reference: %s"), quote (text));
            show_sub_messages (Wother,
                               cp, explicit_bracketing, midrule_rhs_index,
                               dollar_or_at);
          }
        {
          صحیح نماد_index =
            variant_table[valid_variant_index].نماد_index;
          بازگشت (نماد_index == midrule_rhs_index) ? LHS_REF : نماد_index;
        }
      }
    حالت 2:
    پیشفرض:
      {
        complain (text_loc, complaint,
                  _("ambiguous reference: %s"), quote (text));
        show_sub_messages (complaint,
                           cp, explicit_bracketing, midrule_rhs_index,
                           dollar_or_at);
        بازگشت INVALID_REF;
      }
    }
}

/* Keeps track of the maximum number of semantic values to the left of
   a handle (those referenced by $0, $-1, etc.) are required by the
   semantic actions of this grammar. */
صحیح max_چپ_semantic_context = 0;


/* If CP points to a typename (i.e., <.*?>), set TYPE_NAME to its
   beginning (i.e., after the opening "<", and return the pointer
   immediately after it.  */

ایستا
نویسه *
fetch_type_name (نویسه *cp, نویسه ثابت **type_name,
                 ثابت location *dollar_loc)
{
  اگر (*cp == '<')
    {
      *type_name = ++cp;
      /* Series of non-'>' or "->".  */
      وقتی (*cp != '>' || cp[-1] == '-')
        ++cp;

      /* The '>' symbol will be later replaced by '\0'. Original
         'text' is needed for error messages. */
      ++cp;
      اگر (untyped_var_seen)
        complain (dollar_loc, complaint,
                  _("explicit type given in untyped grammar"));
      tag_seen = true;
    }
  بازگشت cp;
}

/*------------------------------------------------------------------.
| TEXT is pointing to a wannabee semantic value (i.e., a '$').      |
|                                                                   |
| Possible inputs: $[<TYPENAME>]($|INTEGER)                         |
|                                                                   |
| Output to OBSTACK_FOR_STRING a reference to this semantic value.  |
`------------------------------------------------------------------*/

ایستا تهی
handle_action_dollar (نماد_list *rule, نویسه *text, ثابت location *dollar_loc)
{
  نماد_list *effective_rule;
  صحیح effective_rule_length;

  اگر (rule->midrule_parent_rule)
    {
      effective_rule = rule->midrule_parent_rule;
      effective_rule_length = rule->midrule_parent_rhs_index - 1;
    }
  وگرنه
    {
      effective_rule = rule;
      effective_rule_length = نماد_list_length (rule->next);
    }

  /* The type name if explicit, otherwise left null. */
  نویسه ثابت *type_name = پوچ;
  نویسه *cp = fetch_type_name (text + 1, &type_name, dollar_loc);
  صحیح n = parse_ref (cp, effective_rule, effective_rule_length,
                     rule->midrule_parent_rhs_index, text, dollar_loc, '$');
  /* End type_name.  Don't do it earlier: parse_ref depends on TEXT.  */
  اگر (type_name)
    cp[-1] = '\0';

  انتخاب (n)
    {
    حالت INVALID_REF:
      بشکن;

    حالت LHS_REF:
      {
        نماد_list *sym = نماد_list_n_get (rule, 0);
        اگر (!type_name
            && !sym->content.sym->content->type_name)
          {
            اگر (union_seen || tag_seen)
              {
                اگر (rule->midrule_parent_rule)
                  complain (dollar_loc, complaint,
                            _("$$ for the midrule at $%d of %s"
                              " has no declared type"),
                            rule->midrule_parent_rhs_index,
                            quote (effective_rule->content.sym->tag));
                وگرنه
                  complain (dollar_loc, complaint,
                            _("$$ of %s has no declared type"),
                            quote (rule->content.sym->tag));
              }
            وگرنه
              untyped_var_seen = true;
          }

        obstack_printf (&obstack_for_string, "]b4_lhs_value(orig %d, ",
                        sym->content.sym->content->عدد);
        obstack_quote (&obstack_for_string, type_name);
        obstack_sgrow (&obstack_for_string, ")[");
        rule->action_props.is_مقدار_used = true;
      }
      بشکن;

      /* Reference to a RHS symbol.  */
    پیشفرض:
      {
        اگر (max_چپ_semantic_context < 1 - n)
          max_چپ_semantic_context = 1 - n;
        نماد_list *sym = 0 < n ? نماد_list_n_get (effective_rule, n) : پوچ;
        اگر (!type_name
            && (!sym || !sym->content.sym->content->type_name))
          {
            اگر (union_seen || tag_seen)
              complain (dollar_loc, complaint,
                        _("$%s of %s has no declared type"), cp,
                        quote (effective_rule->content.sym->tag));
            وگرنه
              untyped_var_seen = true;
          }

        obstack_printf (&obstack_for_string,
                        "]b4_rhs_value(%d, %d, ",
                        effective_rule_length, n);
        اگر (sym)
          obstack_printf (&obstack_for_string, "%s%d, ",
                          sym->content.sym->content->class == nterm_sym ? "orig " : "",
                          sym->content.sym->content->عدد);
        وگرنه
          obstack_sgrow (&obstack_for_string, "[], ");

        obstack_quote (&obstack_for_string, type_name);
        obstack_sgrow (&obstack_for_string, ")[");
        اگر (0 < n)
          {
            اگر (muscle_percent_define_ifdef ("api.value.automove")
                && sym->action_props.is_مقدار_used)
              complain (dollar_loc, Wother,
                        _("multiple occurrences of $%d with api.value.automove"),
                        n);
            sym->action_props.is_مقدار_used = true;
          }
      }
      بشکن;
    }
}


/*------------------------------------------------------.
| TEXT is a location token (i.e., a '@...').  Output to |
| OBSTACK_FOR_STRING a reference to this location.      |
`------------------------------------------------------*/

ایستا تهی
handle_action_at (نماد_list *rule, نویسه *text, ثابت location *at_loc)
{
  نماد_list *effective_rule;
  صحیح effective_rule_length;

  اگر (rule->midrule_parent_rule)
    {
      effective_rule = rule->midrule_parent_rule;
      effective_rule_length = rule->midrule_parent_rhs_index - 1;
    }
  وگرنه
    {
      effective_rule = rule;
      effective_rule_length = نماد_list_length (rule->next);
    }

  muscle_percent_define_ensure ("locations", *at_loc, true);

  صحیح n = parse_ref (text + 1, effective_rule, effective_rule_length,
                     rule->midrule_parent_rhs_index, text, at_loc, '@');
  انتخاب (n)
    {
    حالت INVALID_REF:
      بشکن;

    حالت LHS_REF:
      obstack_sgrow (&obstack_for_string, "]b4_lhs_location[");
      بشکن;

    پیشفرض:
      obstack_printf (&obstack_for_string, "]b4_rhs_location(%d, %d)[",
                      effective_rule_length, n);
      بشکن;
    }
}


/*-------------------------.
| Initialize the scanner.  |
`-------------------------*/

/* Translate the '$...' and '@...' in \a self, in the context \a
   sc_context (SC_RULE_ACTION, SC_SYMBOL_ACTION, INITIAL).  */

ایستا نویسه ثابت *
translate_action (code_props *self, صحیح sc_context)
{
  loc->start = loc->end = self->location.start;
  yy_switch_to_buffer (yy_scan_string (self->code));
  نویسه *res = code_lex (self, sc_context);
  yy_delete_buffer (YY_CURRENT_BUFFER);
  بازگشت res;
}

/*------------------------------------------------------------------------.
| Implementation of the public interface as documented in "scan-code.h".  |
`------------------------------------------------------------------------*/

تهی
code_props_none_init (code_props *self)
{
  *self = code_props_none;
}

code_props code_props_none = CODE_PROPS_NONE_INIT;

تهی
code_props_plain_init (code_props *self, نویسه ثابت *code,
                       location code_loc)
{
  code_props_none_init (self);
  self->kind = CODE_PROPS_PLAIN;
  self->code = code;
  self->location = code_loc;
}

تهی
code_props_نماد_action_init (code_props *self, نویسه ثابت *code,
                               location code_loc)
{
  code_props_none_init (self);
  self->kind = CODE_PROPS_نماد_ACTION;
  self->code = code;
  self->location = code_loc;
}

تهی
code_props_rule_action_init (code_props *self, نویسه ثابت *code,
                             location code_loc, نماد_list *rule,
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

تهی
code_props_translate_code (code_props *self)
{
  انتخاب (self->kind)
    {
    حالت CODE_PROPS_NONE:
      بشکن;
    حالت CODE_PROPS_PLAIN:
      self->code = translate_action (self, INITIAL);
      بشکن;
    حالت CODE_PROPS_نماد_ACTION:
      self->code = translate_action (self, SC_نماد_ACTION);
      بشکن;
    حالت CODE_PROPS_RULE_ACTION:
      self->code = translate_action (self, SC_RULE_ACTION);
      بشکن;
    }
}

تهی
code_scanner_last_string_free (تهی)
{
  STRING_FREE ();
}

تهی
code_scanner_init (تهی)
{
  obstack_init (&obstack_for_string);
  yy_flex_debug = 0;
}

تهی
code_scanner_free (تهی)
{
  obstack_free (&obstack_for_string, 0);
  variant_table_free ();

  /* Reclaim Flex's buffers.  */
  yylex_destroy ();
}
