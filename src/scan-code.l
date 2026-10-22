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
#அந்தர்பாவ <c-ctype.h>
#அந்தர்பாவ <get-errno.h>
#அந்தர்பாவ <quote.h>

#அந்தர்பாவ "src/complain.h"
#அந்தர்பாவ "src/getargs.h"
#அந்தர்பாவ "src/muscle-tab.h"
#அந்தர்பாவ "src/reader.h"
#அந்தர்பாவ "src/scan-code.h"
#அந்தர்பாவ "src/symlist.h"

#define FLEX_PREFIX(Id) code_ ## Id
#அந்தர்பாவ "src/flex-scanner.h"

/* Work around a bug in flex 2.5.31.  See Debian bug 333231
   <https://bugs.debian.org/333231>.  */
#undef code_wrap
#define code_wrap() 1

கட்டமைப்பு obstack *obstack_for_actions = &obstack_for_string;

/* The current calling start condition: SC_RULE_ACTION or
   SC_SYMBOL_ACTION. */
# define YY_DECL நிலையான எழுத்து *code_lex (code_props *self, முழு sc_context)
YY_DECL;

#define YY_USER_ACTION  location_compute (loc, &loc->end, yytext, yyleng);

நிலையான எழுத்து *fetch_type_name (எழுத்து *cp, எழுத்து மாறிலி **type_name,
                              மாறிலி location *dollar_loc);

நிலையான வெறுமை handle_action_dollar (குறியீடு_list *rule, எழுத்து *cp,
                                  மாறிலி location *dollar_loc);
நிலையான வெறுமை handle_action_at (குறியீடு_list *rule, எழுத்து *cp,
                              மாறிலி location *at_loc);

/* A string to be pushed to obstack after dollar/at has been handled. */
நிலையான எழுத்து *ref_tail_fields;

நிலையான location current_loc;
நிலையான location *loc = &current_loc;

/* A string representing the most recent translation.  */
நிலையான எழுத்து *last_string;

/* True if an untyped $$ or $n was seen.  */
நிலையான bool untyped_var_seen;

%}
 /* C and C++ comments in code. */
%x SC_COMMENT SC_வரி_COMMENT
 /* Strings and characters in code. */
%x SC_STRING SC_CHARACTER
 /* Whether in a rule or symbol action.  Specifies the translation
    of $ and @.  */
%x SC_RULE_ACTION SC_குறியீடு_ACTION


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
  aver (sc_context == SC_குறியீடு_ACTION
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

<SC_வரி_COMMENT>
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


<SC_RULE_ACTION,SC_குறியீடு_ACTION>
{
  "'"              STRING_GROW (); BEGIN SC_CHARACTER;
  "\""             STRING_GROW (); BEGIN SC_STRING;
  "/"{splice}"*"   STRING_GROW (); BEGIN SC_COMMENT;
  "/"{splice}"/"   STRING_GROW (); BEGIN SC_வரி_COMMENT;

  [$@]  {
    complain (loc, Wother, _("stray '%s'"), yytext);
    obstack_escape (&obstack_for_string, yytext);
  }
}

<SC_RULE_ACTION>
{
  "$"("<"{tag}">")?{ref}  {
    ref_tail_fields = வெற்று;
    handle_action_dollar (self->rule, yytext, loc);
    எனில் (ref_tail_fields)
      obstack_sgrow (&obstack_for_string, ref_tail_fields);
  }
  "@"{ref} {
    ref_tail_fields = வெற்று;
    handle_action_at (self->rule, yytext, loc);
    எனில் (ref_tail_fields)
      obstack_sgrow (&obstack_for_string, ref_tail_fields);
  }
}

<SC_குறியீடு_ACTION>
{
  "$"("<"{tag}">")?"$" {
    மாறிலி எழுத்து *type_name = வெற்று;
    fetch_type_name (yytext + 1, &type_name, loc)[-1] = 0;
    obstack_sgrow (&obstack_for_string, "]b4_dollar_dollar(");
    obstack_quote (&obstack_for_string, type_name);
    obstack_sgrow (&obstack_for_string, ")[");
    self->is_மதிப்பு_used = true;
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
  <<EOF>>     STRING_FINISH (); திரும்பு last_string;
}

%%

நிலையான உள்வரி bool
is_dot_or_dash (எழுத்து ch)
{
  திரும்பு ch == '.' || ch == '-';
}

நிலையான உள்வரி bool
contains_dot_or_dash (மாறிலி எழுத்து* p)
{
  திரும்பு !!strpbrk (p, ".-");
}

/* Defines a variant of a symbolic name resolution. */
வகைவரை கட்டமைப்பு
{
  /* Index in symbol list. */
  முழு குறியீடு_index;

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

நிலையான variant *variant_table = வெற்று;
நிலையான முழு variant_table_size = 0;
நிலையான முழு variant_மொத்தம் = 0;

நிலையான variant *
variant_table_grow (வெறுமை)
{
  ++variant_மொத்தம்;
  எனில் (variant_மொத்தம் > variant_table_size)
    {
      போது (variant_மொத்தம் > variant_table_size)
        variant_table_size = 2 * variant_table_size + 3;
      variant_table = xnrealloc (variant_table, variant_table_size,
                                 அளவு *variant_table);
    }
  திரும்பு &variant_table[variant_count - 1];
}

நிலையான வெறுமை
variant_table_free (வெறுமை)
{
  விடுவி (variant_table);
  variant_table = வெற்று;
  variant_table_size = variant_மொத்தம் = 0;
}

நிலையான எழுத்து மாறிலி *
find_prefix_end (எழுத்து மாறிலி *prefix, எழுத்து மாறிலி *cp, எழுத்து மாறிலி *end)
{
  க்கு (; *prefix && cp != end; ++prefix, ++cp)
    எனில் (*prefix != *cp)
      திரும்பு வெற்று;

  திரும்பு *prefix ? வெற்று : cp;
}

நிலையான variant *
variant_add (uniqstr id, location id_loc, முழு குறியீடு_index,
             எழுத்து மாறிலி *cp, எழுத்து மாறிலி *cp_end, bool explicit_bracketing)
{
  எழுத்து மாறிலி *prefix_end = find_prefix_end (id, cp, cp_end);
  எனில் (prefix_end &&
      (prefix_end == cp_end ||
       (!explicit_bracketing && is_dot_or_dash (*prefix_end))))
    {
      variant *r = variant_table_grow ();
      r->குறியீடு_index = குறியீடு_index;
      r->id = id;
      r->loc = id_loc;
      r->hidden_by = வெற்று;
      r->err = 0;
      திரும்பு r;
    }
  இல்லையெனில்
    திரும்பு வெற்று;
}

நிலையான மாறிலி எழுத்து *
get_at_spec(முழு குறியீடு_index)
{
  நிலையான எழுத்து at_buf[20];
  எனில் (குறியீடு_index == 0)
    strcpy (at_buf, "$$");
  இல்லையெனில்
    snprintf (at_buf, அளவு at_buf, "$%u", குறியீடு_index);
  திரும்பு at_buf;
}

நிலையான வெறுமை
show_sub_message (warnings warning,
                  மாறிலி எழுத்து* cp, bool explicit_bracketing,
                  முழு midrule_rhs_index, எழுத்து dollar_or_at,
                  மாறிலி variant *var)
{
  மாறிலி எழுத்து *at_spec = get_at_spec (var->குறியீடு_index);

  எனில் (var->err == 0)
    subcomplain (&var->loc, warning,
                 _("refers to: %c%s at %s"), dollar_or_at,
                 var->id, at_spec);
  இல்லையெனில்
    {
      மாறிலி எழுத்து *id;
      location id_loc;

      எனில் (var->hidden_by)
        {
          id = var->hidden_by->id;
          id_loc = var->hidden_by->loc;
        }
      இல்லையெனில்
        {
          id = var->id;
          id_loc = var->loc;
        }

      மாறிலி எழுத்து *tail = explicit_bracketing ? "" : cp + strlen (var->id);

      /* Create the explanation message. */
      நிலையான கட்டமைப்பு obstack செய்தி_buf;
      obstack_init (&செய்தி_buf);

      obstack_printf (&செய்தி_buf, _("possibly meant: %c"), dollar_or_at);
      எனில் (contains_dot_or_dash (id))
        obstack_printf (&செய்தி_buf, "[%s]", id);
      இல்லையெனில்
        obstack_sgrow (&செய்தி_buf, id);
      obstack_sgrow (&செய்தி_buf, tail);

      எனில் (var->err & VARIANT_HIDDEN)
        {
          obstack_printf (&செய்தி_buf, _(", hiding %c"), dollar_or_at);
          எனில் (contains_dot_or_dash (var->id))
            obstack_printf (&செய்தி_buf, "[%s]", var->id);
          இல்லையெனில்
            obstack_sgrow (&செய்தி_buf, var->id);
          obstack_sgrow (&செய்தி_buf, tail);
        }

      obstack_printf (&செய்தி_buf, _(" at %s"), at_spec);

      எனில் (var->err & VARIANT_NOT_VISIBLE_FROM_MIDRULE)
        obstack_printf (&செய்தி_buf,
                        _(", cannot be accessed from midrule action at $%d"),
                        midrule_rhs_index);

      subcomplain (&id_loc, warning, "%s",
                   obstack_finish0 (&செய்தி_buf));
      obstack_free (&செய்தி_buf, 0);
    }
}

நிலையான வெறுமை
show_sub_messages (warnings warning,
                   மாறிலி எழுத்து* cp, bool explicit_bracketing,
                   முழு midrule_rhs_index, எழுத்து dollar_or_at)
{
  க்கு (முழு i = 0; i < variant_மொத்தம்; ++i)
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
நிலையான long
parse_positional_ref (எழுத்து *cp, முழு rule_length,
                      எழுத்து *text, மாறிலி location *text_loc)
{
  long இலக்கம் = strtol (cp, &cp, 10);
  எனில் (1 - INT_MAX + rule_length <= இலக்கம் && இலக்கம் <= rule_length)
    திரும்பு இலக்கம்;
  இல்லையெனில்
    {
      complain (text_loc, complaint, _("integer out of range: %s"),
                quote (text));
      திரும்பு INVALID_REF;
    }
}


/* Parse named or positional reference. In case of positional
   references, can return negative values for $-n "deep" stack
   accesses. */
நிலையான long
parse_ref (எழுத்து *cp, குறியீடு_list *rule, முழு rule_length,
           முழு midrule_rhs_index, எழுத்து *text, மாறிலி location *text_loc,
           எழுத்து dollar_or_at)
{
  எனில் ('$' == *cp)
    திரும்பு LHS_REF;

  எனில் (c_isdigit (*cp) || (*cp == '-' && c_isdigit (cp[1])))
    திரும்பு parse_positional_ref (cp, rule_length, text, text_loc);

  bool மாறிலி explicit_bracketing = *cp == '[';

  எனில் (explicit_bracketing)
    ++cp;
  இல்லையெனில்
    ref_tail_fields = strpbrk (cp, ".-");

  எழுத்து மாறிலி *cp_end = strchr (cp, explicit_bracketing ? ']' : '\0');

  /* Add all relevant variants. */
  {
    முழு குறியீடு_index;
    குறியீடு_list *l;
    variant_மொத்தம் = 0;
    க்கு (குறியீடு_index = 0, l = rule; !குறியீடு_list_null (l);
         ++குறியீடு_index, l = l->next)
      {
        எனில் (l->content_type != SYMLIST_குறியீடு)
          தொடர்;

        variant *var
          = variant_add (l->content.sym->tag, l->sym_loc,
                         குறியீடு_index, cp, cp_end, explicit_bracketing);
        எனில் (var && l->named_ref)
          var->hidden_by = l->named_ref;

        எனில் (l->named_ref)
          variant_add (l->named_ref->id, l->named_ref->loc,
                       குறியீடு_index, cp, cp_end, explicit_bracketing);
      }
  }

  /* Check errors. */
  முழு valid_variants = 0;
  முழு valid_variant_index = 0;
  க்கு (முழு i = 0; i < variant_மொத்தம்; ++i)
    {
      variant *var = &variant_table[i];
      முழு குறியீடு_index = var->குறியீடு_index;

      /* Check visibility from midrule actions. */
      எனில் (midrule_rhs_index != 0
          && (குறியீடு_index == 0 || midrule_rhs_index < குறியீடு_index))
        var->err |= VARIANT_NOT_VISIBLE_FROM_MIDRULE;

      /* Check correct bracketing. */
      எனில் (!explicit_bracketing && contains_dot_or_dash (var->id))
        var->err |= VARIANT_BAD_BRACKETING;

      /* Check using of hidden symbols. */
      எனில் (var->hidden_by)
        var->err |= VARIANT_HIDDEN;

      எனில் (!var->err)
        {
          valid_variant_index = i;
          ++valid_variants;
        }
    }

  தேர்வு (valid_variants)
    {
    நிலை 0:
      {
        முழு len = (explicit_bracketing || !ref_tail_fields) ?
          cp_end - cp : ref_tail_fields - cp;

        complain (text_loc, complaint,
                  _("invalid reference: %s"), quote (text));
        எனில் (len == 0)
          {
            location sym_loc = *text_loc;
            sym_loc.start.column += 1;
            sym_loc.end = sym_loc.start;
            subcomplain (&sym_loc, complaint,
                         _("syntax error after '%c', expecting integer, "
                         "letter, '_', '[', or '$'"),
                         dollar_or_at);
          }
        இல்லையெனில் எனில் (midrule_rhs_index)
          subcomplain (&rule->rhs_loc, complaint,
                       _("symbol not found in production before $%d: "
                       "%.*s"),
                       midrule_rhs_index, len, cp);
        இல்லையெனில்
          subcomplain (&rule->rhs_loc, complaint,
                       _("symbol not found in production: %.*s"),
                       len, cp);

        எனில் (variant_மொத்தம் > 0)
          show_sub_messages (complaint,
                             cp, explicit_bracketing, midrule_rhs_index,
                             dollar_or_at);
        திரும்பு INVALID_REF;
      }
    நிலை 1:
      {
        எனில் (variant_மொத்தம் > 1)
          {
            complain (text_loc, Wother,
                      _("misleading reference: %s"), quote (text));
            show_sub_messages (Wother,
                               cp, explicit_bracketing, midrule_rhs_index,
                               dollar_or_at);
          }
        {
          முழு குறியீடு_index =
            variant_table[valid_variant_index].குறியீடு_index;
          திரும்பு (குறியீடு_index == midrule_rhs_index) ? LHS_REF : குறியீடு_index;
        }
      }
    நிலை 2:
    இயல்பு:
      {
        complain (text_loc, complaint,
                  _("ambiguous reference: %s"), quote (text));
        show_sub_messages (complaint,
                           cp, explicit_bracketing, midrule_rhs_index,
                           dollar_or_at);
        திரும்பு INVALID_REF;
      }
    }
}

/* Keeps track of the maximum number of semantic values to the left of
   a handle (those referenced by $0, $-1, etc.) are required by the
   semantic actions of this grammar. */
முழு max_இடது_semantic_context = 0;


/* If CP points to a typename (i.e., <.*?>), set TYPE_NAME to its
   beginning (i.e., after the opening "<", and return the pointer
   immediately after it.  */

நிலையான
எழுத்து *
fetch_type_name (எழுத்து *cp, எழுத்து மாறிலி **type_name,
                 மாறிலி location *dollar_loc)
{
  எனில் (*cp == '<')
    {
      *type_name = ++cp;
      /* Series of non-'>' or "->".  */
      போது (*cp != '>' || cp[-1] == '-')
        ++cp;

      /* The '>' symbol will be later replaced by '\0'. Original
         'text' is needed for error messages. */
      ++cp;
      எனில் (untyped_var_seen)
        complain (dollar_loc, complaint,
                  _("explicit type given in untyped grammar"));
      tag_seen = true;
    }
  திரும்பு cp;
}

/*------------------------------------------------------------------.
| TEXT is pointing to a wannabee semantic value (i.e., a '$').      |
|                                                                   |
| Possible inputs: $[<TYPENAME>]($|INTEGER)                         |
|                                                                   |
| Output to OBSTACK_FOR_STRING a reference to this semantic value.  |
`------------------------------------------------------------------*/

நிலையான வெறுமை
handle_action_dollar (குறியீடு_list *rule, எழுத்து *text, மாறிலி location *dollar_loc)
{
  குறியீடு_list *effective_rule;
  முழு effective_rule_length;

  எனில் (rule->midrule_parent_rule)
    {
      effective_rule = rule->midrule_parent_rule;
      effective_rule_length = rule->midrule_parent_rhs_index - 1;
    }
  இல்லையெனில்
    {
      effective_rule = rule;
      effective_rule_length = குறியீடு_list_length (rule->next);
    }

  /* The type name if explicit, otherwise left null. */
  எழுத்து மாறிலி *type_name = வெற்று;
  எழுத்து *cp = fetch_type_name (text + 1, &type_name, dollar_loc);
  முழு n = parse_ref (cp, effective_rule, effective_rule_length,
                     rule->midrule_parent_rhs_index, text, dollar_loc, '$');
  /* End type_name.  Don't do it earlier: parse_ref depends on TEXT.  */
  எனில் (type_name)
    cp[-1] = '\0';

  தேர்வு (n)
    {
    நிலை INVALID_REF:
      நிறுத்து;

    நிலை LHS_REF:
      {
        குறியீடு_list *sym = குறியீடு_list_n_get (rule, 0);
        எனில் (!type_name
            && !sym->content.sym->content->type_name)
          {
            எனில் (union_seen || tag_seen)
              {
                எனில் (rule->midrule_parent_rule)
                  complain (dollar_loc, complaint,
                            _("$$ for the midrule at $%d of %s"
                              " has no declared type"),
                            rule->midrule_parent_rhs_index,
                            quote (effective_rule->content.sym->tag));
                இல்லையெனில்
                  complain (dollar_loc, complaint,
                            _("$$ of %s has no declared type"),
                            quote (rule->content.sym->tag));
              }
            இல்லையெனில்
              untyped_var_seen = true;
          }

        obstack_printf (&obstack_for_string, "]b4_lhs_value(orig %d, ",
                        sym->content.sym->content->எண்);
        obstack_quote (&obstack_for_string, type_name);
        obstack_sgrow (&obstack_for_string, ")[");
        rule->action_props.is_மதிப்பு_used = true;
      }
      நிறுத்து;

      /* Reference to a RHS symbol.  */
    இயல்பு:
      {
        எனில் (max_இடது_semantic_context < 1 - n)
          max_இடது_semantic_context = 1 - n;
        குறியீடு_list *sym = 0 < n ? குறியீடு_list_n_get (effective_rule, n) : வெற்று;
        எனில் (!type_name
            && (!sym || !sym->content.sym->content->type_name))
          {
            எனில் (union_seen || tag_seen)
              complain (dollar_loc, complaint,
                        _("$%s of %s has no declared type"), cp,
                        quote (effective_rule->content.sym->tag));
            இல்லையெனில்
              untyped_var_seen = true;
          }

        obstack_printf (&obstack_for_string,
                        "]b4_rhs_value(%d, %d, ",
                        effective_rule_length, n);
        எனில் (sym)
          obstack_printf (&obstack_for_string, "%s%d, ",
                          sym->content.sym->content->class == nterm_sym ? "orig " : "",
                          sym->content.sym->content->எண்);
        இல்லையெனில்
          obstack_sgrow (&obstack_for_string, "[], ");

        obstack_quote (&obstack_for_string, type_name);
        obstack_sgrow (&obstack_for_string, ")[");
        எனில் (0 < n)
          {
            எனில் (muscle_percent_define_ifdef ("api.value.automove")
                && sym->action_props.is_மதிப்பு_used)
              complain (dollar_loc, Wother,
                        _("multiple occurrences of $%d with api.value.automove"),
                        n);
            sym->action_props.is_மதிப்பு_used = true;
          }
      }
      நிறுத்து;
    }
}


/*------------------------------------------------------.
| TEXT is a location token (i.e., a '@...').  Output to |
| OBSTACK_FOR_STRING a reference to this location.      |
`------------------------------------------------------*/

நிலையான வெறுமை
handle_action_at (குறியீடு_list *rule, எழுத்து *text, மாறிலி location *at_loc)
{
  குறியீடு_list *effective_rule;
  முழு effective_rule_length;

  எனில் (rule->midrule_parent_rule)
    {
      effective_rule = rule->midrule_parent_rule;
      effective_rule_length = rule->midrule_parent_rhs_index - 1;
    }
  இல்லையெனில்
    {
      effective_rule = rule;
      effective_rule_length = குறியீடு_list_length (rule->next);
    }

  muscle_percent_define_ensure ("locations", *at_loc, true);

  முழு n = parse_ref (text + 1, effective_rule, effective_rule_length,
                     rule->midrule_parent_rhs_index, text, at_loc, '@');
  தேர்வு (n)
    {
    நிலை INVALID_REF:
      நிறுத்து;

    நிலை LHS_REF:
      obstack_sgrow (&obstack_for_string, "]b4_lhs_location[");
      நிறுத்து;

    இயல்பு:
      obstack_printf (&obstack_for_string, "]b4_rhs_location(%d, %d)[",
                      effective_rule_length, n);
      நிறுத்து;
    }
}


/*-------------------------.
| Initialize the scanner.  |
`-------------------------*/

/* Translate the '$...' and '@...' in \a self, in the context \a
   sc_context (SC_RULE_ACTION, SC_SYMBOL_ACTION, INITIAL).  */

நிலையான எழுத்து மாறிலி *
translate_action (code_props *self, முழு sc_context)
{
  loc->start = loc->end = self->location.start;
  yy_switch_to_buffer (yy_scan_string (self->code));
  எழுத்து *res = code_lex (self, sc_context);
  yy_delete_buffer (YY_CURRENT_BUFFER);
  திரும்பு res;
}

/*------------------------------------------------------------------------.
| Implementation of the public interface as documented in "scan-code.h".  |
`------------------------------------------------------------------------*/

வெறுமை
code_props_none_init (code_props *self)
{
  *self = code_props_none;
}

code_props code_props_none = CODE_PROPS_NONE_INIT;

வெறுமை
code_props_plain_init (code_props *self, எழுத்து மாறிலி *code,
                       location code_loc)
{
  code_props_none_init (self);
  self->kind = CODE_PROPS_PLAIN;
  self->code = code;
  self->location = code_loc;
}

வெறுமை
code_props_குறியீடு_action_init (code_props *self, எழுத்து மாறிலி *code,
                               location code_loc)
{
  code_props_none_init (self);
  self->kind = CODE_PROPS_குறியீடு_ACTION;
  self->code = code;
  self->location = code_loc;
}

வெறுமை
code_props_rule_action_init (code_props *self, எழுத்து மாறிலி *code,
                             location code_loc, குறியீடு_list *rule,
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

வெறுமை
code_props_translate_code (code_props *self)
{
  தேர்வு (self->kind)
    {
    நிலை CODE_PROPS_NONE:
      நிறுத்து;
    நிலை CODE_PROPS_PLAIN:
      self->code = translate_action (self, INITIAL);
      நிறுத்து;
    நிலை CODE_PROPS_குறியீடு_ACTION:
      self->code = translate_action (self, SC_குறியீடு_ACTION);
      நிறுத்து;
    நிலை CODE_PROPS_RULE_ACTION:
      self->code = translate_action (self, SC_RULE_ACTION);
      நிறுத்து;
    }
}

வெறுமை
code_scanner_last_string_free (வெறுமை)
{
  STRING_FREE ();
}

வெறுமை
code_scanner_init (வெறுமை)
{
  obstack_init (&obstack_for_string);
  yy_flex_debug = 0;
}

வெறுமை
code_scanner_free (வெறுமை)
{
  obstack_free (&obstack_for_string, 0);
  variant_table_free ();

  /* Reclaim Flex's buffers.  */
  yylex_destroy ();
}
