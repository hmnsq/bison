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
#અન્તર્ભાવ <c-ctype.h>
#અન્તર્ભાવ <get-errno.h>
#અન્તર્ભાવ <quote.h>

#અન્તર્ભાવ "src/complain.h"
#અન્તર્ભાવ "src/getargs.h"
#અન્તર્ભાવ "src/muscle-tab.h"
#અન્તર્ભાવ "src/reader.h"
#અન્તર્ભાવ "src/scan-code.h"
#અન્તર્ભાવ "src/symlist.h"

#define FLEX_PREFIX(Id) code_ ## Id
#અન્તર્ભાવ "src/flex-scanner.h"

/* Work around a bug in flex 2.5.31.  See Debian bug 333231
   <https://bugs.debian.org/333231>.  */
#undef code_wrap
#define code_wrap() 1

સંરચના obstack *obstack_for_actions = &obstack_for_string;

/* The current calling start condition: SC_RULE_ACTION or
   SC_SYMBOL_ACTION. */
# define YY_DECL સ્થિર અક્ષર *code_lex (code_props *self, પૂર્ણાંક sc_context)
YY_DECL;

#define YY_USER_ACTION  location_compute (loc, &loc->end, yytext, yyleng);

સ્થિર અક્ષર *fetch_type_name (અક્ષર *cp, અક્ષર અચલ **type_name,
                              અચલ location *dollar_loc);

સ્થિર શૂન્ય handle_action_dollar (પ્રતીક_list *rule, અક્ષર *cp,
                                  અચલ location *dollar_loc);
સ્થિર શૂન્ય handle_action_at (પ્રતીક_list *rule, અક્ષર *cp,
                              અચલ location *at_loc);

/* A string to be pushed to obstack after dollar/at has been handled. */
સ્થિર અક્ષર *ref_tail_fields;

સ્થિર location current_loc;
સ્થિર location *loc = &current_loc;

/* A string representing the most recent translation.  */
સ્થિર અક્ષર *last_string;

/* True if an untyped $$ or $n was seen.  */
સ્થિર bool untyped_var_seen;

%}
 /* C and C++ comments in code. */
%x SC_COMMENT SC_લીટી_COMMENT
 /* Strings and characters in code. */
%x SC_STRING SC_CHARACTER
 /* Whether in a rule or symbol action.  Specifies the translation
    of $ and @.  */
%x SC_RULE_ACTION SC_પ્રતીક_ACTION


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
  aver (sc_context == SC_પ્રતીક_ACTION
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

<SC_લીટી_COMMENT>
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


<SC_RULE_ACTION,SC_પ્રતીક_ACTION>
{
  "'"              STRING_GROW (); BEGIN SC_CHARACTER;
  "\""             STRING_GROW (); BEGIN SC_STRING;
  "/"{splice}"*"   STRING_GROW (); BEGIN SC_COMMENT;
  "/"{splice}"/"   STRING_GROW (); BEGIN SC_લીટી_COMMENT;

  [$@]  {
    complain (loc, Wother, _("stray '%s'"), yytext);
    obstack_escape (&obstack_for_string, yytext);
  }
}

<SC_RULE_ACTION>
{
  "$"("<"{tag}">")?{ref}  {
    ref_tail_fields = ખાલી;
    handle_action_dollar (self->rule, yytext, loc);
    જો (ref_tail_fields)
      obstack_sgrow (&obstack_for_string, ref_tail_fields);
  }
  "@"{ref} {
    ref_tail_fields = ખાલી;
    handle_action_at (self->rule, yytext, loc);
    જો (ref_tail_fields)
      obstack_sgrow (&obstack_for_string, ref_tail_fields);
  }
}

<SC_પ્રતીક_ACTION>
{
  "$"("<"{tag}">")?"$" {
    અચલ અક્ષર *type_name = ખાલી;
    fetch_type_name (yytext + 1, &type_name, loc)[-1] = 0;
    obstack_sgrow (&obstack_for_string, "]b4_dollar_dollar(");
    obstack_quote (&obstack_for_string, type_name);
    obstack_sgrow (&obstack_for_string, ")[");
    self->is_કિંમત_used = true;
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
  <<EOF>>     STRING_FINISH (); પાછા last_string;
}

%%

સ્થિર અંતઃરેખ bool
is_dot_or_dash (અક્ષર ch)
{
  પાછા ch == '.' || ch == '-';
}

સ્થિર અંતઃરેખ bool
contains_dot_or_dash (અચલ અક્ષર* p)
{
  પાછા !!strpbrk (p, ".-");
}

/* Defines a variant of a symbolic name resolution. */
પ્રકારવ્યાખ્યા સંરચના
{
  /* Index in symbol list. */
  પૂર્ણાંક પ્રતીક_index;

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

સ્થિર variant *variant_table = ખાલી;
સ્થિર પૂર્ણાંક variant_table_size = 0;
સ્થિર પૂર્ણાંક variant_ગણતરી = 0;

સ્થિર variant *
variant_table_grow (શૂન્ય)
{
  ++variant_ગણતરી;
  જો (variant_ગણતરી > variant_table_size)
    {
      જ્યારે (variant_ગણતરી > variant_table_size)
        variant_table_size = 2 * variant_table_size + 3;
      variant_table = xnrealloc (variant_table, variant_table_size,
                                 કદ *variant_table);
    }
  પાછા &variant_table[variant_count - 1];
}

સ્થિર શૂન્ય
variant_table_free (શૂન્ય)
{
  મુક્ત (variant_table);
  variant_table = ખાલી;
  variant_table_size = variant_ગણતરી = 0;
}

સ્થિર અક્ષર અચલ *
find_prefix_end (અક્ષર અચલ *prefix, અક્ષર અચલ *cp, અક્ષર અચલ *end)
{
  માટે (; *prefix && cp != end; ++prefix, ++cp)
    જો (*prefix != *cp)
      પાછા ખાલી;

  પાછા *prefix ? ખાલી : cp;
}

સ્થિર variant *
variant_add (uniqstr id, location id_loc, પૂર્ણાંક પ્રતીક_index,
             અક્ષર અચલ *cp, અક્ષર અચલ *cp_end, bool explicit_bracketing)
{
  અક્ષર અચલ *prefix_end = find_prefix_end (id, cp, cp_end);
  જો (prefix_end &&
      (prefix_end == cp_end ||
       (!explicit_bracketing && is_dot_or_dash (*prefix_end))))
    {
      variant *r = variant_table_grow ();
      r->પ્રતીક_index = પ્રતીક_index;
      r->id = id;
      r->loc = id_loc;
      r->hidden_by = ખાલી;
      r->err = 0;
      પાછા r;
    }
  નહીંતર
    પાછા ખાલી;
}

સ્થિર અચલ અક્ષર *
get_at_spec(પૂર્ણાંક પ્રતીક_index)
{
  સ્થિર અક્ષર at_buf[20];
  જો (પ્રતીક_index == 0)
    strcpy (at_buf, "$$");
  નહીંતર
    snprintf (at_buf, કદ at_buf, "$%u", પ્રતીક_index);
  પાછા at_buf;
}

સ્થિર શૂન્ય
show_sub_message (warnings warning,
                  અચલ અક્ષર* cp, bool explicit_bracketing,
                  પૂર્ણાંક midrule_rhs_index, અક્ષર dollar_or_at,
                  અચલ variant *var)
{
  અચલ અક્ષર *at_spec = get_at_spec (var->પ્રતીક_index);

  જો (var->err == 0)
    subcomplain (&var->loc, warning,
                 _("refers to: %c%s at %s"), dollar_or_at,
                 var->id, at_spec);
  નહીંતર
    {
      અચલ અક્ષર *id;
      location id_loc;

      જો (var->hidden_by)
        {
          id = var->hidden_by->id;
          id_loc = var->hidden_by->loc;
        }
      નહીંતર
        {
          id = var->id;
          id_loc = var->loc;
        }

      અચલ અક્ષર *tail = explicit_bracketing ? "" : cp + strlen (var->id);

      /* Create the explanation message. */
      સ્થિર સંરચના obstack સંદેશ_buf;
      obstack_init (&સંદેશ_buf);

      obstack_printf (&સંદેશ_buf, _("possibly meant: %c"), dollar_or_at);
      જો (contains_dot_or_dash (id))
        obstack_printf (&સંદેશ_buf, "[%s]", id);
      નહીંતર
        obstack_sgrow (&સંદેશ_buf, id);
      obstack_sgrow (&સંદેશ_buf, tail);

      જો (var->err & VARIANT_HIDDEN)
        {
          obstack_printf (&સંદેશ_buf, _(", hiding %c"), dollar_or_at);
          જો (contains_dot_or_dash (var->id))
            obstack_printf (&સંદેશ_buf, "[%s]", var->id);
          નહીંતર
            obstack_sgrow (&સંદેશ_buf, var->id);
          obstack_sgrow (&સંદેશ_buf, tail);
        }

      obstack_printf (&સંદેશ_buf, _(" at %s"), at_spec);

      જો (var->err & VARIANT_NOT_VISIBLE_FROM_MIDRULE)
        obstack_printf (&સંદેશ_buf,
                        _(", cannot be accessed from midrule action at $%d"),
                        midrule_rhs_index);

      subcomplain (&id_loc, warning, "%s",
                   obstack_finish0 (&સંદેશ_buf));
      obstack_free (&સંદેશ_buf, 0);
    }
}

સ્થિર શૂન્ય
show_sub_messages (warnings warning,
                   અચલ અક્ષર* cp, bool explicit_bracketing,
                   પૂર્ણાંક midrule_rhs_index, અક્ષર dollar_or_at)
{
  માટે (પૂર્ણાંક i = 0; i < variant_ગણતરી; ++i)
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
સ્થિર long
parse_positional_ref (અક્ષર *cp, પૂર્ણાંક rule_length,
                      અક્ષર *text, અચલ location *text_loc)
{
  long અંક = strtol (cp, &cp, 10);
  જો (1 - INT_MAX + rule_length <= અંક && અંક <= rule_length)
    પાછા અંક;
  નહીંતર
    {
      complain (text_loc, complaint, _("integer out of range: %s"),
                quote (text));
      પાછા INVALID_REF;
    }
}


/* Parse named or positional reference. In case of positional
   references, can return negative values for $-n "deep" stack
   accesses. */
સ્થિર long
parse_ref (અક્ષર *cp, પ્રતીક_list *rule, પૂર્ણાંક rule_length,
           પૂર્ણાંક midrule_rhs_index, અક્ષર *text, અચલ location *text_loc,
           અક્ષર dollar_or_at)
{
  જો ('$' == *cp)
    પાછા LHS_REF;

  જો (c_isdigit (*cp) || (*cp == '-' && c_isdigit (cp[1])))
    પાછા parse_positional_ref (cp, rule_length, text, text_loc);

  bool અચલ explicit_bracketing = *cp == '[';

  જો (explicit_bracketing)
    ++cp;
  નહીંતર
    ref_tail_fields = strpbrk (cp, ".-");

  અક્ષર અચલ *cp_end = strchr (cp, explicit_bracketing ? ']' : '\0');

  /* Add all relevant variants. */
  {
    પૂર્ણાંક પ્રતીક_index;
    પ્રતીક_list *l;
    variant_ગણતરી = 0;
    માટે (પ્રતીક_index = 0, l = rule; !પ્રતીક_list_null (l);
         ++પ્રતીક_index, l = l->next)
      {
        જો (l->content_type != SYMLIST_પ્રતીક)
          ચાલુ;

        variant *var
          = variant_add (l->content.sym->tag, l->sym_loc,
                         પ્રતીક_index, cp, cp_end, explicit_bracketing);
        જો (var && l->named_ref)
          var->hidden_by = l->named_ref;

        જો (l->named_ref)
          variant_add (l->named_ref->id, l->named_ref->loc,
                       પ્રતીક_index, cp, cp_end, explicit_bracketing);
      }
  }

  /* Check errors. */
  પૂર્ણાંક valid_variants = 0;
  પૂર્ણાંક valid_variant_index = 0;
  માટે (પૂર્ણાંક i = 0; i < variant_ગણતરી; ++i)
    {
      variant *var = &variant_table[i];
      પૂર્ણાંક પ્રતીક_index = var->પ્રતીક_index;

      /* Check visibility from midrule actions. */
      જો (midrule_rhs_index != 0
          && (પ્રતીક_index == 0 || midrule_rhs_index < પ્રતીક_index))
        var->err |= VARIANT_NOT_VISIBLE_FROM_MIDRULE;

      /* Check correct bracketing. */
      જો (!explicit_bracketing && contains_dot_or_dash (var->id))
        var->err |= VARIANT_BAD_BRACKETING;

      /* Check using of hidden symbols. */
      જો (var->hidden_by)
        var->err |= VARIANT_HIDDEN;

      જો (!var->err)
        {
          valid_variant_index = i;
          ++valid_variants;
        }
    }

  પસંદગી (valid_variants)
    {
    સ્થિતિ 0:
      {
        પૂર્ણાંક len = (explicit_bracketing || !ref_tail_fields) ?
          cp_end - cp : ref_tail_fields - cp;

        complain (text_loc, complaint,
                  _("invalid reference: %s"), quote (text));
        જો (len == 0)
          {
            location sym_loc = *text_loc;
            sym_loc.start.column += 1;
            sym_loc.end = sym_loc.start;
            subcomplain (&sym_loc, complaint,
                         _("syntax error after '%c', expecting integer, "
                         "letter, '_', '[', or '$'"),
                         dollar_or_at);
          }
        નહીંતર જો (midrule_rhs_index)
          subcomplain (&rule->rhs_loc, complaint,
                       _("symbol not found in production before $%d: "
                       "%.*s"),
                       midrule_rhs_index, len, cp);
        નહીંતર
          subcomplain (&rule->rhs_loc, complaint,
                       _("symbol not found in production: %.*s"),
                       len, cp);

        જો (variant_ગણતરી > 0)
          show_sub_messages (complaint,
                             cp, explicit_bracketing, midrule_rhs_index,
                             dollar_or_at);
        પાછા INVALID_REF;
      }
    સ્થિતિ 1:
      {
        જો (variant_ગણતરી > 1)
          {
            complain (text_loc, Wother,
                      _("misleading reference: %s"), quote (text));
            show_sub_messages (Wother,
                               cp, explicit_bracketing, midrule_rhs_index,
                               dollar_or_at);
          }
        {
          પૂર્ણાંક પ્રતીક_index =
            variant_table[valid_variant_index].પ્રતીક_index;
          પાછા (પ્રતીક_index == midrule_rhs_index) ? LHS_REF : પ્રતીક_index;
        }
      }
    સ્થિતિ 2:
    મૂળભૂત:
      {
        complain (text_loc, complaint,
                  _("ambiguous reference: %s"), quote (text));
        show_sub_messages (complaint,
                           cp, explicit_bracketing, midrule_rhs_index,
                           dollar_or_at);
        પાછા INVALID_REF;
      }
    }
}

/* Keeps track of the maximum number of semantic values to the left of
   a handle (those referenced by $0, $-1, etc.) are required by the
   semantic actions of this grammar. */
પૂર્ણાંક max_ડાબું_semantic_context = 0;


/* If CP points to a typename (i.e., <.*?>), set TYPE_NAME to its
   beginning (i.e., after the opening "<", and return the pointer
   immediately after it.  */

સ્થિર
અક્ષર *
fetch_type_name (અક્ષર *cp, અક્ષર અચલ **type_name,
                 અચલ location *dollar_loc)
{
  જો (*cp == '<')
    {
      *type_name = ++cp;
      /* Series of non-'>' or "->".  */
      જ્યારે (*cp != '>' || cp[-1] == '-')
        ++cp;

      /* The '>' symbol will be later replaced by '\0'. Original
         'text' is needed for error messages. */
      ++cp;
      જો (untyped_var_seen)
        complain (dollar_loc, complaint,
                  _("explicit type given in untyped grammar"));
      tag_seen = true;
    }
  પાછા cp;
}

/*------------------------------------------------------------------.
| TEXT is pointing to a wannabee semantic value (i.e., a '$').      |
|                                                                   |
| Possible inputs: $[<TYPENAME>]($|INTEGER)                         |
|                                                                   |
| Output to OBSTACK_FOR_STRING a reference to this semantic value.  |
`------------------------------------------------------------------*/

સ્થિર શૂન્ય
handle_action_dollar (પ્રતીક_list *rule, અક્ષર *text, અચલ location *dollar_loc)
{
  પ્રતીક_list *effective_rule;
  પૂર્ણાંક effective_rule_length;

  જો (rule->midrule_parent_rule)
    {
      effective_rule = rule->midrule_parent_rule;
      effective_rule_length = rule->midrule_parent_rhs_index - 1;
    }
  નહીંતર
    {
      effective_rule = rule;
      effective_rule_length = પ્રતીક_list_length (rule->next);
    }

  /* The type name if explicit, otherwise left null. */
  અક્ષર અચલ *type_name = ખાલી;
  અક્ષર *cp = fetch_type_name (text + 1, &type_name, dollar_loc);
  પૂર્ણાંક n = parse_ref (cp, effective_rule, effective_rule_length,
                     rule->midrule_parent_rhs_index, text, dollar_loc, '$');
  /* End type_name.  Don't do it earlier: parse_ref depends on TEXT.  */
  જો (type_name)
    cp[-1] = '\0';

  પસંદગી (n)
    {
    સ્થિતિ INVALID_REF:
      તોડો;

    સ્થિતિ LHS_REF:
      {
        પ્રતીક_list *sym = પ્રતીક_list_n_get (rule, 0);
        જો (!type_name
            && !sym->content.sym->content->type_name)
          {
            જો (union_seen || tag_seen)
              {
                જો (rule->midrule_parent_rule)
                  complain (dollar_loc, complaint,
                            _("$$ for the midrule at $%d of %s"
                              " has no declared type"),
                            rule->midrule_parent_rhs_index,
                            quote (effective_rule->content.sym->tag));
                નહીંતર
                  complain (dollar_loc, complaint,
                            _("$$ of %s has no declared type"),
                            quote (rule->content.sym->tag));
              }
            નહીંતર
              untyped_var_seen = true;
          }

        obstack_printf (&obstack_for_string, "]b4_lhs_value(orig %d, ",
                        sym->content.sym->content->સંખ્યા);
        obstack_quote (&obstack_for_string, type_name);
        obstack_sgrow (&obstack_for_string, ")[");
        rule->action_props.is_કિંમત_used = true;
      }
      તોડો;

      /* Reference to a RHS symbol.  */
    મૂળભૂત:
      {
        જો (max_ડાબું_semantic_context < 1 - n)
          max_ડાબું_semantic_context = 1 - n;
        પ્રતીક_list *sym = 0 < n ? પ્રતીક_list_n_get (effective_rule, n) : ખાલી;
        જો (!type_name
            && (!sym || !sym->content.sym->content->type_name))
          {
            જો (union_seen || tag_seen)
              complain (dollar_loc, complaint,
                        _("$%s of %s has no declared type"), cp,
                        quote (effective_rule->content.sym->tag));
            નહીંતર
              untyped_var_seen = true;
          }

        obstack_printf (&obstack_for_string,
                        "]b4_rhs_value(%d, %d, ",
                        effective_rule_length, n);
        જો (sym)
          obstack_printf (&obstack_for_string, "%s%d, ",
                          sym->content.sym->content->class == nterm_sym ? "orig " : "",
                          sym->content.sym->content->સંખ્યા);
        નહીંતર
          obstack_sgrow (&obstack_for_string, "[], ");

        obstack_quote (&obstack_for_string, type_name);
        obstack_sgrow (&obstack_for_string, ")[");
        જો (0 < n)
          {
            જો (muscle_percent_define_ifdef ("api.value.automove")
                && sym->action_props.is_કિંમત_used)
              complain (dollar_loc, Wother,
                        _("multiple occurrences of $%d with api.value.automove"),
                        n);
            sym->action_props.is_કિંમત_used = true;
          }
      }
      તોડો;
    }
}


/*------------------------------------------------------.
| TEXT is a location token (i.e., a '@...').  Output to |
| OBSTACK_FOR_STRING a reference to this location.      |
`------------------------------------------------------*/

સ્થિર શૂન્ય
handle_action_at (પ્રતીક_list *rule, અક્ષર *text, અચલ location *at_loc)
{
  પ્રતીક_list *effective_rule;
  પૂર્ણાંક effective_rule_length;

  જો (rule->midrule_parent_rule)
    {
      effective_rule = rule->midrule_parent_rule;
      effective_rule_length = rule->midrule_parent_rhs_index - 1;
    }
  નહીંતર
    {
      effective_rule = rule;
      effective_rule_length = પ્રતીક_list_length (rule->next);
    }

  muscle_percent_define_ensure ("locations", *at_loc, true);

  પૂર્ણાંક n = parse_ref (text + 1, effective_rule, effective_rule_length,
                     rule->midrule_parent_rhs_index, text, at_loc, '@');
  પસંદગી (n)
    {
    સ્થિતિ INVALID_REF:
      તોડો;

    સ્થિતિ LHS_REF:
      obstack_sgrow (&obstack_for_string, "]b4_lhs_location[");
      તોડો;

    મૂળભૂત:
      obstack_printf (&obstack_for_string, "]b4_rhs_location(%d, %d)[",
                      effective_rule_length, n);
      તોડો;
    }
}


/*-------------------------.
| Initialize the scanner.  |
`-------------------------*/

/* Translate the '$...' and '@...' in \a self, in the context \a
   sc_context (SC_RULE_ACTION, SC_SYMBOL_ACTION, INITIAL).  */

સ્થિર અક્ષર અચલ *
translate_action (code_props *self, પૂર્ણાંક sc_context)
{
  loc->start = loc->end = self->location.start;
  yy_switch_to_buffer (yy_scan_string (self->code));
  અક્ષર *res = code_lex (self, sc_context);
  yy_delete_buffer (YY_CURRENT_BUFFER);
  પાછા res;
}

/*------------------------------------------------------------------------.
| Implementation of the public interface as documented in "scan-code.h".  |
`------------------------------------------------------------------------*/

શૂન્ય
code_props_none_init (code_props *self)
{
  *self = code_props_none;
}

code_props code_props_none = CODE_PROPS_NONE_INIT;

શૂન્ય
code_props_plain_init (code_props *self, અક્ષર અચલ *code,
                       location code_loc)
{
  code_props_none_init (self);
  self->kind = CODE_PROPS_PLAIN;
  self->code = code;
  self->location = code_loc;
}

શૂન્ય
code_props_પ્રતીક_action_init (code_props *self, અક્ષર અચલ *code,
                               location code_loc)
{
  code_props_none_init (self);
  self->kind = CODE_PROPS_પ્રતીક_ACTION;
  self->code = code;
  self->location = code_loc;
}

શૂન્ય
code_props_rule_action_init (code_props *self, અક્ષર અચલ *code,
                             location code_loc, પ્રતીક_list *rule,
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

શૂન્ય
code_props_translate_code (code_props *self)
{
  પસંદગી (self->kind)
    {
    સ્થિતિ CODE_PROPS_NONE:
      તોડો;
    સ્થિતિ CODE_PROPS_PLAIN:
      self->code = translate_action (self, INITIAL);
      તોડો;
    સ્થિતિ CODE_PROPS_પ્રતીક_ACTION:
      self->code = translate_action (self, SC_પ્રતીક_ACTION);
      તોડો;
    સ્થિતિ CODE_PROPS_RULE_ACTION:
      self->code = translate_action (self, SC_RULE_ACTION);
      તોડો;
    }
}

શૂન્ય
code_scanner_last_string_free (શૂન્ય)
{
  STRING_FREE ();
}

શૂન્ય
code_scanner_init (શૂન્ય)
{
  obstack_init (&obstack_for_string);
  yy_flex_debug = 0;
}

શૂન્ય
code_scanner_free (શૂન્ય)
{
  obstack_free (&obstack_for_string, 0);
  variant_table_free ();

  /* Reclaim Flex's buffers.  */
  yylex_destroy ();
}
