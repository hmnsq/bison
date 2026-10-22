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
#അന്തര്ഭാവ <c-ctype.h>
#അന്തര്ഭാവ <get-errno.h>
#അന്തര്ഭാവ <quote.h>

#അന്തര്ഭാവ "src/complain.h"
#അന്തര്ഭാവ "src/getargs.h"
#അന്തര്ഭാവ "src/muscle-tab.h"
#അന്തര്ഭാവ "src/reader.h"
#അന്തര്ഭാവ "src/scan-code.h"
#അന്തര്ഭാവ "src/symlist.h"

#define FLEX_PREFIX(Id) code_ ## Id
#അന്തര്ഭാവ "src/flex-scanner.h"

/* Work around a bug in flex 2.5.31.  See Debian bug 333231
   <https://bugs.debian.org/333231>.  */
#undef code_wrap
#define code_wrap() 1

ഘടന obstack *obstack_for_actions = &obstack_for_string;

/* The current calling start condition: SC_RULE_ACTION or
   SC_SYMBOL_ACTION. */
# define YY_DECL സ്ഥിതം അക്ഷരം *code_lex (code_props *self, പൂർണ്ണസംഖ്യ sc_context)
YY_DECL;

#define YY_USER_ACTION  location_compute (loc, &loc->end, yytext, yyleng);

സ്ഥിതം അക്ഷരം *fetch_type_name (അക്ഷരം *cp, അക്ഷരം സ്ഥിരം **type_name,
                              സ്ഥിരം location *dollar_loc);

സ്ഥിതം ശൂന്യം handle_action_dollar (ചിഹ്നം_list *rule, അക്ഷരം *cp,
                                  സ്ഥിരം location *dollar_loc);
സ്ഥിതം ശൂന്യം handle_action_at (ചിഹ്നം_list *rule, അക്ഷരം *cp,
                              സ്ഥിരം location *at_loc);

/* A string to be pushed to obstack after dollar/at has been handled. */
സ്ഥിതം അക്ഷരം *ref_tail_fields;

സ്ഥിതം location current_loc;
സ്ഥിതം location *loc = &current_loc;

/* A string representing the most recent translation.  */
സ്ഥിതം അക്ഷരം *last_string;

/* True if an untyped $$ or $n was seen.  */
സ്ഥിതം bool untyped_var_seen;

%}
 /* C and C++ comments in code. */
%x SC_COMMENT SC_വരി_COMMENT
 /* Strings and characters in code. */
%x SC_STRING SC_CHARACTER
 /* Whether in a rule or symbol action.  Specifies the translation
    of $ and @.  */
%x SC_RULE_ACTION SC_ചിഹ്നം_ACTION


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
  aver (sc_context == SC_ചിഹ്നം_ACTION
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

<SC_വരി_COMMENT>
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


<SC_RULE_ACTION,SC_ചിഹ്നം_ACTION>
{
  "'"              STRING_GROW (); BEGIN SC_CHARACTER;
  "\""             STRING_GROW (); BEGIN SC_STRING;
  "/"{splice}"*"   STRING_GROW (); BEGIN SC_COMMENT;
  "/"{splice}"/"   STRING_GROW (); BEGIN SC_വരി_COMMENT;

  [$@]  {
    complain (loc, Wother, _("stray '%s'"), yytext);
    obstack_escape (&obstack_for_string, yytext);
  }
}

<SC_RULE_ACTION>
{
  "$"("<"{tag}">")?{ref}  {
    ref_tail_fields = ശൂന്യ;
    handle_action_dollar (self->rule, yytext, loc);
    എങ്കിൽ (ref_tail_fields)
      obstack_sgrow (&obstack_for_string, ref_tail_fields);
  }
  "@"{ref} {
    ref_tail_fields = ശൂന്യ;
    handle_action_at (self->rule, yytext, loc);
    എങ്കിൽ (ref_tail_fields)
      obstack_sgrow (&obstack_for_string, ref_tail_fields);
  }
}

<SC_ചിഹ്നം_ACTION>
{
  "$"("<"{tag}">")?"$" {
    സ്ഥിരം അക്ഷരം *type_name = ശൂന്യ;
    fetch_type_name (yytext + 1, &type_name, loc)[-1] = 0;
    obstack_sgrow (&obstack_for_string, "]b4_dollar_dollar(");
    obstack_quote (&obstack_for_string, type_name);
    obstack_sgrow (&obstack_for_string, ")[");
    self->is_മൂല്യം_used = true;
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
  <<EOF>>     STRING_FINISH (); തിരികെ last_string;
}

%%

സ്ഥിതം വരിയിൽ bool
is_dot_or_dash (അക്ഷരം ch)
{
  തിരികെ ch == '.' || ch == '-';
}

സ്ഥിതം വരിയിൽ bool
contains_dot_or_dash (സ്ഥിരം അക്ഷരം* p)
{
  തിരികെ !!strpbrk (p, ".-");
}

/* Defines a variant of a symbolic name resolution. */
തരനിർവചനം ഘടന
{
  /* Index in symbol list. */
  പൂർണ്ണസംഖ്യ ചിഹ്നം_index;

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

സ്ഥിതം variant *variant_table = ശൂന്യ;
സ്ഥിതം പൂർണ്ണസംഖ്യ variant_table_size = 0;
സ്ഥിതം പൂർണ്ണസംഖ്യ variant_എണ്ണം = 0;

സ്ഥിതം variant *
variant_table_grow (ശൂന്യം)
{
  ++variant_എണ്ണം;
  എങ്കിൽ (variant_എണ്ണം > variant_table_size)
    {
      ആകുമ്പോൾ (variant_എണ്ണം > variant_table_size)
        variant_table_size = 2 * variant_table_size + 3;
      variant_table = xnrealloc (variant_table, variant_table_size,
                                 വലുപ്പം *variant_table);
    }
  തിരികെ &variant_table[variant_count - 1];
}

സ്ഥിതം ശൂന്യം
variant_table_free (ശൂന്യം)
{
  സ്വതന്ത്രം (variant_table);
  variant_table = ശൂന്യ;
  variant_table_size = variant_എണ്ണം = 0;
}

സ്ഥിതം അക്ഷരം സ്ഥിരം *
find_prefix_end (അക്ഷരം സ്ഥിരം *prefix, അക്ഷരം സ്ഥിരം *cp, അക്ഷരം സ്ഥിരം *end)
{
  വേണ്ടി (; *prefix && cp != end; ++prefix, ++cp)
    എങ്കിൽ (*prefix != *cp)
      തിരികെ ശൂന്യ;

  തിരികെ *prefix ? ശൂന്യ : cp;
}

സ്ഥിതം variant *
variant_add (uniqstr id, location id_loc, പൂർണ്ണസംഖ്യ ചിഹ്നം_index,
             അക്ഷരം സ്ഥിരം *cp, അക്ഷരം സ്ഥിരം *cp_end, bool explicit_bracketing)
{
  അക്ഷരം സ്ഥിരം *prefix_end = find_prefix_end (id, cp, cp_end);
  എങ്കിൽ (prefix_end &&
      (prefix_end == cp_end ||
       (!explicit_bracketing && is_dot_or_dash (*prefix_end))))
    {
      variant *r = variant_table_grow ();
      r->ചിഹ്നം_index = ചിഹ്നം_index;
      r->id = id;
      r->loc = id_loc;
      r->hidden_by = ശൂന്യ;
      r->err = 0;
      തിരികെ r;
    }
  അല്ലെങ്കിൽ
    തിരികെ ശൂന്യ;
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *
get_at_spec(പൂർണ്ണസംഖ്യ ചിഹ്നം_index)
{
  സ്ഥിതം അക്ഷരം at_buf[20];
  എങ്കിൽ (ചിഹ്നം_index == 0)
    strcpy (at_buf, "$$");
  അല്ലെങ്കിൽ
    snprintf (at_buf, വലുപ്പം at_buf, "$%u", ചിഹ്നം_index);
  തിരികെ at_buf;
}

സ്ഥിതം ശൂന്യം
show_sub_message (warnings warning,
                  സ്ഥിരം അക്ഷരം* cp, bool explicit_bracketing,
                  പൂർണ്ണസംഖ്യ midrule_rhs_index, അക്ഷരം dollar_or_at,
                  സ്ഥിരം variant *var)
{
  സ്ഥിരം അക്ഷരം *at_spec = get_at_spec (var->ചിഹ്നം_index);

  എങ്കിൽ (var->err == 0)
    subcomplain (&var->loc, warning,
                 _("refers to: %c%s at %s"), dollar_or_at,
                 var->id, at_spec);
  അല്ലെങ്കിൽ
    {
      സ്ഥിരം അക്ഷരം *id;
      location id_loc;

      എങ്കിൽ (var->hidden_by)
        {
          id = var->hidden_by->id;
          id_loc = var->hidden_by->loc;
        }
      അല്ലെങ്കിൽ
        {
          id = var->id;
          id_loc = var->loc;
        }

      സ്ഥിരം അക്ഷരം *tail = explicit_bracketing ? "" : cp + strlen (var->id);

      /* Create the explanation message. */
      സ്ഥിതം ഘടന obstack സന്ദേശം_buf;
      obstack_init (&സന്ദേശം_buf);

      obstack_printf (&സന്ദേശം_buf, _("possibly meant: %c"), dollar_or_at);
      എങ്കിൽ (contains_dot_or_dash (id))
        obstack_printf (&സന്ദേശം_buf, "[%s]", id);
      അല്ലെങ്കിൽ
        obstack_sgrow (&സന്ദേശം_buf, id);
      obstack_sgrow (&സന്ദേശം_buf, tail);

      എങ്കിൽ (var->err & VARIANT_HIDDEN)
        {
          obstack_printf (&സന്ദേശം_buf, _(", hiding %c"), dollar_or_at);
          എങ്കിൽ (contains_dot_or_dash (var->id))
            obstack_printf (&സന്ദേശം_buf, "[%s]", var->id);
          അല്ലെങ്കിൽ
            obstack_sgrow (&സന്ദേശം_buf, var->id);
          obstack_sgrow (&സന്ദേശം_buf, tail);
        }

      obstack_printf (&സന്ദേശം_buf, _(" at %s"), at_spec);

      എങ്കിൽ (var->err & VARIANT_NOT_VISIBLE_FROM_MIDRULE)
        obstack_printf (&സന്ദേശം_buf,
                        _(", cannot be accessed from midrule action at $%d"),
                        midrule_rhs_index);

      subcomplain (&id_loc, warning, "%s",
                   obstack_finish0 (&സന്ദേശം_buf));
      obstack_free (&സന്ദേശം_buf, 0);
    }
}

സ്ഥിതം ശൂന്യം
show_sub_messages (warnings warning,
                   സ്ഥിരം അക്ഷരം* cp, bool explicit_bracketing,
                   പൂർണ്ണസംഖ്യ midrule_rhs_index, അക്ഷരം dollar_or_at)
{
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < variant_എണ്ണം; ++i)
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
സ്ഥിതം long
parse_positional_ref (അക്ഷരം *cp, പൂർണ്ണസംഖ്യ rule_length,
                      അക്ഷരം *text, സ്ഥിരം location *text_loc)
{
  long അക്കം = strtol (cp, &cp, 10);
  എങ്കിൽ (1 - INT_MAX + rule_length <= അക്കം && അക്കം <= rule_length)
    തിരികെ അക്കം;
  അല്ലെങ്കിൽ
    {
      complain (text_loc, complaint, _("integer out of range: %s"),
                quote (text));
      തിരികെ INVALID_REF;
    }
}


/* Parse named or positional reference. In case of positional
   references, can return negative values for $-n "deep" stack
   accesses. */
സ്ഥിതം long
parse_ref (അക്ഷരം *cp, ചിഹ്നം_list *rule, പൂർണ്ണസംഖ്യ rule_length,
           പൂർണ്ണസംഖ്യ midrule_rhs_index, അക്ഷരം *text, സ്ഥിരം location *text_loc,
           അക്ഷരം dollar_or_at)
{
  എങ്കിൽ ('$' == *cp)
    തിരികെ LHS_REF;

  എങ്കിൽ (c_isdigit (*cp) || (*cp == '-' && c_isdigit (cp[1])))
    തിരികെ parse_positional_ref (cp, rule_length, text, text_loc);

  bool സ്ഥിരം explicit_bracketing = *cp == '[';

  എങ്കിൽ (explicit_bracketing)
    ++cp;
  അല്ലെങ്കിൽ
    ref_tail_fields = strpbrk (cp, ".-");

  അക്ഷരം സ്ഥിരം *cp_end = strchr (cp, explicit_bracketing ? ']' : '\0');

  /* Add all relevant variants. */
  {
    പൂർണ്ണസംഖ്യ ചിഹ്നം_index;
    ചിഹ്നം_list *l;
    variant_എണ്ണം = 0;
    വേണ്ടി (ചിഹ്നം_index = 0, l = rule; !ചിഹ്നം_list_null (l);
         ++ചിഹ്നം_index, l = l->next)
      {
        എങ്കിൽ (l->content_type != SYMLIST_ചിഹ്നം)
          തുടരുക;

        variant *var
          = variant_add (l->content.sym->tag, l->sym_loc,
                         ചിഹ്നം_index, cp, cp_end, explicit_bracketing);
        എങ്കിൽ (var && l->named_ref)
          var->hidden_by = l->named_ref;

        എങ്കിൽ (l->named_ref)
          variant_add (l->named_ref->id, l->named_ref->loc,
                       ചിഹ്നം_index, cp, cp_end, explicit_bracketing);
      }
  }

  /* Check errors. */
  പൂർണ്ണസംഖ്യ valid_variants = 0;
  പൂർണ്ണസംഖ്യ valid_variant_index = 0;
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < variant_എണ്ണം; ++i)
    {
      variant *var = &variant_table[i];
      പൂർണ്ണസംഖ്യ ചിഹ്നം_index = var->ചിഹ്നം_index;

      /* Check visibility from midrule actions. */
      എങ്കിൽ (midrule_rhs_index != 0
          && (ചിഹ്നം_index == 0 || midrule_rhs_index < ചിഹ്നം_index))
        var->err |= VARIANT_NOT_VISIBLE_FROM_MIDRULE;

      /* Check correct bracketing. */
      എങ്കിൽ (!explicit_bracketing && contains_dot_or_dash (var->id))
        var->err |= VARIANT_BAD_BRACKETING;

      /* Check using of hidden symbols. */
      എങ്കിൽ (var->hidden_by)
        var->err |= VARIANT_HIDDEN;

      എങ്കിൽ (!var->err)
        {
          valid_variant_index = i;
          ++valid_variants;
        }
    }

  തിരഞ്ഞെടുപ്പ് (valid_variants)
    {
    അവസ്ഥ 0:
      {
        പൂർണ്ണസംഖ്യ len = (explicit_bracketing || !ref_tail_fields) ?
          cp_end - cp : ref_tail_fields - cp;

        complain (text_loc, complaint,
                  _("invalid reference: %s"), quote (text));
        എങ്കിൽ (len == 0)
          {
            location sym_loc = *text_loc;
            sym_loc.start.column += 1;
            sym_loc.end = sym_loc.start;
            subcomplain (&sym_loc, complaint,
                         _("syntax error after '%c', expecting integer, "
                         "letter, '_', '[', or '$'"),
                         dollar_or_at);
          }
        അല്ലെങ്കിൽ എങ്കിൽ (midrule_rhs_index)
          subcomplain (&rule->rhs_loc, complaint,
                       _("symbol not found in production before $%d: "
                       "%.*s"),
                       midrule_rhs_index, len, cp);
        അല്ലെങ്കിൽ
          subcomplain (&rule->rhs_loc, complaint,
                       _("symbol not found in production: %.*s"),
                       len, cp);

        എങ്കിൽ (variant_എണ്ണം > 0)
          show_sub_messages (complaint,
                             cp, explicit_bracketing, midrule_rhs_index,
                             dollar_or_at);
        തിരികെ INVALID_REF;
      }
    അവസ്ഥ 1:
      {
        എങ്കിൽ (variant_എണ്ണം > 1)
          {
            complain (text_loc, Wother,
                      _("misleading reference: %s"), quote (text));
            show_sub_messages (Wother,
                               cp, explicit_bracketing, midrule_rhs_index,
                               dollar_or_at);
          }
        {
          പൂർണ്ണസംഖ്യ ചിഹ്നം_index =
            variant_table[valid_variant_index].ചിഹ്നം_index;
          തിരികെ (ചിഹ്നം_index == midrule_rhs_index) ? LHS_REF : ചിഹ്നം_index;
        }
      }
    അവസ്ഥ 2:
    സ്ഥിരസ്ഥിതി:
      {
        complain (text_loc, complaint,
                  _("ambiguous reference: %s"), quote (text));
        show_sub_messages (complaint,
                           cp, explicit_bracketing, midrule_rhs_index,
                           dollar_or_at);
        തിരികെ INVALID_REF;
      }
    }
}

/* Keeps track of the maximum number of semantic values to the left of
   a handle (those referenced by $0, $-1, etc.) are required by the
   semantic actions of this grammar. */
പൂർണ്ണസംഖ്യ max_ഇടത്_semantic_context = 0;


/* If CP points to a typename (i.e., <.*?>), set TYPE_NAME to its
   beginning (i.e., after the opening "<", and return the pointer
   immediately after it.  */

സ്ഥിതം
അക്ഷരം *
fetch_type_name (അക്ഷരം *cp, അക്ഷരം സ്ഥിരം **type_name,
                 സ്ഥിരം location *dollar_loc)
{
  എങ്കിൽ (*cp == '<')
    {
      *type_name = ++cp;
      /* Series of non-'>' or "->".  */
      ആകുമ്പോൾ (*cp != '>' || cp[-1] == '-')
        ++cp;

      /* The '>' symbol will be later replaced by '\0'. Original
         'text' is needed for error messages. */
      ++cp;
      എങ്കിൽ (untyped_var_seen)
        complain (dollar_loc, complaint,
                  _("explicit type given in untyped grammar"));
      tag_seen = true;
    }
  തിരികെ cp;
}

/*------------------------------------------------------------------.
| TEXT is pointing to a wannabee semantic value (i.e., a '$').      |
|                                                                   |
| Possible inputs: $[<TYPENAME>]($|INTEGER)                         |
|                                                                   |
| Output to OBSTACK_FOR_STRING a reference to this semantic value.  |
`------------------------------------------------------------------*/

സ്ഥിതം ശൂന്യം
handle_action_dollar (ചിഹ്നം_list *rule, അക്ഷരം *text, സ്ഥിരം location *dollar_loc)
{
  ചിഹ്നം_list *effective_rule;
  പൂർണ്ണസംഖ്യ effective_rule_length;

  എങ്കിൽ (rule->midrule_parent_rule)
    {
      effective_rule = rule->midrule_parent_rule;
      effective_rule_length = rule->midrule_parent_rhs_index - 1;
    }
  അല്ലെങ്കിൽ
    {
      effective_rule = rule;
      effective_rule_length = ചിഹ്നം_list_length (rule->next);
    }

  /* The type name if explicit, otherwise left null. */
  അക്ഷരം സ്ഥിരം *type_name = ശൂന്യ;
  അക്ഷരം *cp = fetch_type_name (text + 1, &type_name, dollar_loc);
  പൂർണ്ണസംഖ്യ n = parse_ref (cp, effective_rule, effective_rule_length,
                     rule->midrule_parent_rhs_index, text, dollar_loc, '$');
  /* End type_name.  Don't do it earlier: parse_ref depends on TEXT.  */
  എങ്കിൽ (type_name)
    cp[-1] = '\0';

  തിരഞ്ഞെടുപ്പ് (n)
    {
    അവസ്ഥ INVALID_REF:
      നിർത്തുക;

    അവസ്ഥ LHS_REF:
      {
        ചിഹ്നം_list *sym = ചിഹ്നം_list_n_get (rule, 0);
        എങ്കിൽ (!type_name
            && !sym->content.sym->content->type_name)
          {
            എങ്കിൽ (union_seen || tag_seen)
              {
                എങ്കിൽ (rule->midrule_parent_rule)
                  complain (dollar_loc, complaint,
                            _("$$ for the midrule at $%d of %s"
                              " has no declared type"),
                            rule->midrule_parent_rhs_index,
                            quote (effective_rule->content.sym->tag));
                അല്ലെങ്കിൽ
                  complain (dollar_loc, complaint,
                            _("$$ of %s has no declared type"),
                            quote (rule->content.sym->tag));
              }
            അല്ലെങ്കിൽ
              untyped_var_seen = true;
          }

        obstack_printf (&obstack_for_string, "]b4_lhs_value(orig %d, ",
                        sym->content.sym->content->സംഖ്യ);
        obstack_quote (&obstack_for_string, type_name);
        obstack_sgrow (&obstack_for_string, ")[");
        rule->action_props.is_മൂല്യം_used = true;
      }
      നിർത്തുക;

      /* Reference to a RHS symbol.  */
    സ്ഥിരസ്ഥിതി:
      {
        എങ്കിൽ (max_ഇടത്_semantic_context < 1 - n)
          max_ഇടത്_semantic_context = 1 - n;
        ചിഹ്നം_list *sym = 0 < n ? ചിഹ്നം_list_n_get (effective_rule, n) : ശൂന്യ;
        എങ്കിൽ (!type_name
            && (!sym || !sym->content.sym->content->type_name))
          {
            എങ്കിൽ (union_seen || tag_seen)
              complain (dollar_loc, complaint,
                        _("$%s of %s has no declared type"), cp,
                        quote (effective_rule->content.sym->tag));
            അല്ലെങ്കിൽ
              untyped_var_seen = true;
          }

        obstack_printf (&obstack_for_string,
                        "]b4_rhs_value(%d, %d, ",
                        effective_rule_length, n);
        എങ്കിൽ (sym)
          obstack_printf (&obstack_for_string, "%s%d, ",
                          sym->content.sym->content->class == nterm_sym ? "orig " : "",
                          sym->content.sym->content->സംഖ്യ);
        അല്ലെങ്കിൽ
          obstack_sgrow (&obstack_for_string, "[], ");

        obstack_quote (&obstack_for_string, type_name);
        obstack_sgrow (&obstack_for_string, ")[");
        എങ്കിൽ (0 < n)
          {
            എങ്കിൽ (muscle_percent_define_ifdef ("api.value.automove")
                && sym->action_props.is_മൂല്യം_used)
              complain (dollar_loc, Wother,
                        _("multiple occurrences of $%d with api.value.automove"),
                        n);
            sym->action_props.is_മൂല്യം_used = true;
          }
      }
      നിർത്തുക;
    }
}


/*------------------------------------------------------.
| TEXT is a location token (i.e., a '@...').  Output to |
| OBSTACK_FOR_STRING a reference to this location.      |
`------------------------------------------------------*/

സ്ഥിതം ശൂന്യം
handle_action_at (ചിഹ്നം_list *rule, അക്ഷരം *text, സ്ഥിരം location *at_loc)
{
  ചിഹ്നം_list *effective_rule;
  പൂർണ്ണസംഖ്യ effective_rule_length;

  എങ്കിൽ (rule->midrule_parent_rule)
    {
      effective_rule = rule->midrule_parent_rule;
      effective_rule_length = rule->midrule_parent_rhs_index - 1;
    }
  അല്ലെങ്കിൽ
    {
      effective_rule = rule;
      effective_rule_length = ചിഹ്നം_list_length (rule->next);
    }

  muscle_percent_define_ensure ("locations", *at_loc, true);

  പൂർണ്ണസംഖ്യ n = parse_ref (text + 1, effective_rule, effective_rule_length,
                     rule->midrule_parent_rhs_index, text, at_loc, '@');
  തിരഞ്ഞെടുപ്പ് (n)
    {
    അവസ്ഥ INVALID_REF:
      നിർത്തുക;

    അവസ്ഥ LHS_REF:
      obstack_sgrow (&obstack_for_string, "]b4_lhs_location[");
      നിർത്തുക;

    സ്ഥിരസ്ഥിതി:
      obstack_printf (&obstack_for_string, "]b4_rhs_location(%d, %d)[",
                      effective_rule_length, n);
      നിർത്തുക;
    }
}


/*-------------------------.
| Initialize the scanner.  |
`-------------------------*/

/* Translate the '$...' and '@...' in \a self, in the context \a
   sc_context (SC_RULE_ACTION, SC_SYMBOL_ACTION, INITIAL).  */

സ്ഥിതം അക്ഷരം സ്ഥിരം *
translate_action (code_props *self, പൂർണ്ണസംഖ്യ sc_context)
{
  loc->start = loc->end = self->location.start;
  yy_switch_to_buffer (yy_scan_string (self->code));
  അക്ഷരം *res = code_lex (self, sc_context);
  yy_delete_buffer (YY_CURRENT_BUFFER);
  തിരികെ res;
}

/*------------------------------------------------------------------------.
| Implementation of the public interface as documented in "scan-code.h".  |
`------------------------------------------------------------------------*/

ശൂന്യം
code_props_none_init (code_props *self)
{
  *self = code_props_none;
}

code_props code_props_none = CODE_PROPS_NONE_INIT;

ശൂന്യം
code_props_plain_init (code_props *self, അക്ഷരം സ്ഥിരം *code,
                       location code_loc)
{
  code_props_none_init (self);
  self->kind = CODE_PROPS_PLAIN;
  self->code = code;
  self->location = code_loc;
}

ശൂന്യം
code_props_ചിഹ്നം_action_init (code_props *self, അക്ഷരം സ്ഥിരം *code,
                               location code_loc)
{
  code_props_none_init (self);
  self->kind = CODE_PROPS_ചിഹ്നം_ACTION;
  self->code = code;
  self->location = code_loc;
}

ശൂന്യം
code_props_rule_action_init (code_props *self, അക്ഷരം സ്ഥിരം *code,
                             location code_loc, ചിഹ്നം_list *rule,
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

ശൂന്യം
code_props_translate_code (code_props *self)
{
  തിരഞ്ഞെടുപ്പ് (self->kind)
    {
    അവസ്ഥ CODE_PROPS_NONE:
      നിർത്തുക;
    അവസ്ഥ CODE_PROPS_PLAIN:
      self->code = translate_action (self, INITIAL);
      നിർത്തുക;
    അവസ്ഥ CODE_PROPS_ചിഹ്നം_ACTION:
      self->code = translate_action (self, SC_ചിഹ്നം_ACTION);
      നിർത്തുക;
    അവസ്ഥ CODE_PROPS_RULE_ACTION:
      self->code = translate_action (self, SC_RULE_ACTION);
      നിർത്തുക;
    }
}

ശൂന്യം
code_scanner_last_string_free (ശൂന്യം)
{
  STRING_FREE ();
}

ശൂന്യം
code_scanner_init (ശൂന്യം)
{
  obstack_init (&obstack_for_string);
  yy_flex_debug = 0;
}

ശൂന്യം
code_scanner_free (ശൂന്യം)
{
  obstack_free (&obstack_for_string, 0);
  variant_table_free ();

  /* Reclaim Flex's buffers.  */
  yylex_destroy ();
}
