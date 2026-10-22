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
#ଅନ୍ତର୍ଭାଵ <c-ctype.h>
#ଅନ୍ତର୍ଭାଵ <get-errno.h>
#ଅନ୍ତର୍ଭାଵ <quote.h>

#ଅନ୍ତର୍ଭାଵ "src/complain.h"
#ଅନ୍ତର୍ଭାଵ "src/getargs.h"
#ଅନ୍ତର୍ଭାଵ "src/muscle-tab.h"
#ଅନ୍ତର୍ଭାଵ "src/reader.h"
#ଅନ୍ତର୍ଭାଵ "src/scan-code.h"
#ଅନ୍ତର୍ଭାଵ "src/symlist.h"

#define FLEX_PREFIX(Id) code_ ## Id
#ଅନ୍ତର୍ଭାଵ "src/flex-scanner.h"

/* Work around a bug in flex 2.5.31.  See Debian bug 333231
   <https://bugs.debian.org/333231>.  */
#undef code_wrap
#define code_wrap() 1

ସଂରଚନା obstack *obstack_for_actions = &obstack_for_string;

/* The current calling start condition: SC_RULE_ACTION or
   SC_SYMBOL_ACTION. */
# define YY_DECL ସ୍ଥିର ଅକ୍ଷର *code_lex (code_props *self, ପୂର୍ଣ୍ଣସଂଖ୍ୟା sc_context)
YY_DECL;

#define YY_USER_ACTION  location_compute (loc, &loc->end, yytext, yyleng);

ସ୍ଥିର ଅକ୍ଷର *fetch_type_name (ଅକ୍ଷର *cp, ଅକ୍ଷର ଅଚଳ **type_name,
                              ଅଚଳ location *dollar_loc);

ସ୍ଥିର ଶୂନ୍ୟ handle_action_dollar (ପ୍ରତୀକ_list *rule, ଅକ୍ଷର *cp,
                                  ଅଚଳ location *dollar_loc);
ସ୍ଥିର ଶୂନ୍ୟ handle_action_at (ପ୍ରତୀକ_list *rule, ଅକ୍ଷର *cp,
                              ଅଚଳ location *at_loc);

/* A string to be pushed to obstack after dollar/at has been handled. */
ସ୍ଥିର ଅକ୍ଷର *ref_tail_fields;

ସ୍ଥିର location current_loc;
ସ୍ଥିର location *loc = &current_loc;

/* A string representing the most recent translation.  */
ସ୍ଥିର ଅକ୍ଷର *last_string;

/* True if an untyped $$ or $n was seen.  */
ସ୍ଥିର bool untyped_var_seen;

%}
 /* C and C++ comments in code. */
%x SC_COMMENT SC_ଧାଡ଼ି_COMMENT
 /* Strings and characters in code. */
%x SC_STRING SC_CHARACTER
 /* Whether in a rule or symbol action.  Specifies the translation
    of $ and @.  */
%x SC_RULE_ACTION SC_ପ୍ରତୀକ_ACTION


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
  aver (sc_context == SC_ପ୍ରତୀକ_ACTION
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

<SC_ଧାଡ଼ି_COMMENT>
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


<SC_RULE_ACTION,SC_ପ୍ରତୀକ_ACTION>
{
  "'"              STRING_GROW (); BEGIN SC_CHARACTER;
  "\""             STRING_GROW (); BEGIN SC_STRING;
  "/"{splice}"*"   STRING_GROW (); BEGIN SC_COMMENT;
  "/"{splice}"/"   STRING_GROW (); BEGIN SC_ଧାଡ଼ି_COMMENT;

  [$@]  {
    complain (loc, Wother, _("stray '%s'"), yytext);
    obstack_escape (&obstack_for_string, yytext);
  }
}

<SC_RULE_ACTION>
{
  "$"("<"{tag}">")?{ref}  {
    ref_tail_fields = ଖାଲି;
    handle_action_dollar (self->rule, yytext, loc);
    ଯଦି (ref_tail_fields)
      obstack_sgrow (&obstack_for_string, ref_tail_fields);
  }
  "@"{ref} {
    ref_tail_fields = ଖାଲି;
    handle_action_at (self->rule, yytext, loc);
    ଯଦି (ref_tail_fields)
      obstack_sgrow (&obstack_for_string, ref_tail_fields);
  }
}

<SC_ପ୍ରତୀକ_ACTION>
{
  "$"("<"{tag}">")?"$" {
    ଅଚଳ ଅକ୍ଷର *type_name = ଖାଲି;
    fetch_type_name (yytext + 1, &type_name, loc)[-1] = 0;
    obstack_sgrow (&obstack_for_string, "]b4_dollar_dollar(");
    obstack_quote (&obstack_for_string, type_name);
    obstack_sgrow (&obstack_for_string, ")[");
    self->is_ମୂଲ୍ୟ_used = true;
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
  <<EOF>>     STRING_FINISH (); ଫେରନ୍ତୁ last_string;
}

%%

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
is_dot_or_dash (ଅକ୍ଷର ch)
{
  ଫେରନ୍ତୁ ch == '.' || ch == '-';
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
contains_dot_or_dash (ଅଚଳ ଅକ୍ଷର* p)
{
  ଫେରନ୍ତୁ !!strpbrk (p, ".-");
}

/* Defines a variant of a symbolic name resolution. */
ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା
{
  /* Index in symbol list. */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ପ୍ରତୀକ_index;

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

ସ୍ଥିର variant *variant_table = ଖାଲି;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା variant_table_size = 0;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା variant_ଗଣନା = 0;

ସ୍ଥିର variant *
variant_table_grow (ଶୂନ୍ୟ)
{
  ++variant_ଗଣନା;
  ଯଦି (variant_ଗଣନା > variant_table_size)
    {
      ଯେତେବେଳେ (variant_ଗଣନା > variant_table_size)
        variant_table_size = 2 * variant_table_size + 3;
      variant_table = xnrealloc (variant_table, variant_table_size,
                                 ଆକାର *variant_table);
    }
  ଫେରନ୍ତୁ &variant_table[variant_count - 1];
}

ସ୍ଥିର ଶୂନ୍ୟ
variant_table_free (ଶୂନ୍ୟ)
{
  ମୁକ୍ତ (variant_table);
  variant_table = ଖାଲି;
  variant_table_size = variant_ଗଣନା = 0;
}

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *
find_prefix_end (ଅକ୍ଷର ଅଚଳ *prefix, ଅକ୍ଷର ଅଚଳ *cp, ଅକ୍ଷର ଅଚଳ *end)
{
  ପାଇଁ (; *prefix && cp != end; ++prefix, ++cp)
    ଯଦି (*prefix != *cp)
      ଫେରନ୍ତୁ ଖାଲି;

  ଫେରନ୍ତୁ *prefix ? ଖାଲି : cp;
}

ସ୍ଥିର variant *
variant_add (uniqstr id, location id_loc, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ପ୍ରତୀକ_index,
             ଅକ୍ଷର ଅଚଳ *cp, ଅକ୍ଷର ଅଚଳ *cp_end, bool explicit_bracketing)
{
  ଅକ୍ଷର ଅଚଳ *prefix_end = find_prefix_end (id, cp, cp_end);
  ଯଦି (prefix_end &&
      (prefix_end == cp_end ||
       (!explicit_bracketing && is_dot_or_dash (*prefix_end))))
    {
      variant *r = variant_table_grow ();
      r->ପ୍ରତୀକ_index = ପ୍ରତୀକ_index;
      r->id = id;
      r->loc = id_loc;
      r->hidden_by = ଖାଲି;
      r->err = 0;
      ଫେରନ୍ତୁ r;
    }
  ଅନ୍ୟଥା
    ଫେରନ୍ତୁ ଖାଲି;
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *
get_at_spec(ପୂର୍ଣ୍ଣସଂଖ୍ୟା ପ୍ରତୀକ_index)
{
  ସ୍ଥିର ଅକ୍ଷର at_buf[20];
  ଯଦି (ପ୍ରତୀକ_index == 0)
    strcpy (at_buf, "$$");
  ଅନ୍ୟଥା
    snprintf (at_buf, ଆକାର at_buf, "$%u", ପ୍ରତୀକ_index);
  ଫେରନ୍ତୁ at_buf;
}

ସ୍ଥିର ଶୂନ୍ୟ
show_sub_message (warnings warning,
                  ଅଚଳ ଅକ୍ଷର* cp, bool explicit_bracketing,
                  ପୂର୍ଣ୍ଣସଂଖ୍ୟା midrule_rhs_index, ଅକ୍ଷର dollar_or_at,
                  ଅଚଳ variant *var)
{
  ଅଚଳ ଅକ୍ଷର *at_spec = get_at_spec (var->ପ୍ରତୀକ_index);

  ଯଦି (var->err == 0)
    subcomplain (&var->loc, warning,
                 _("refers to: %c%s at %s"), dollar_or_at,
                 var->id, at_spec);
  ଅନ୍ୟଥା
    {
      ଅଚଳ ଅକ୍ଷର *id;
      location id_loc;

      ଯଦି (var->hidden_by)
        {
          id = var->hidden_by->id;
          id_loc = var->hidden_by->loc;
        }
      ଅନ୍ୟଥା
        {
          id = var->id;
          id_loc = var->loc;
        }

      ଅଚଳ ଅକ୍ଷର *tail = explicit_bracketing ? "" : cp + strlen (var->id);

      /* Create the explanation message. */
      ସ୍ଥିର ସଂରଚନା obstack ସନ୍ଦେଶ_buf;
      obstack_init (&ସନ୍ଦେଶ_buf);

      obstack_printf (&ସନ୍ଦେଶ_buf, _("possibly meant: %c"), dollar_or_at);
      ଯଦି (contains_dot_or_dash (id))
        obstack_printf (&ସନ୍ଦେଶ_buf, "[%s]", id);
      ଅନ୍ୟଥା
        obstack_sgrow (&ସନ୍ଦେଶ_buf, id);
      obstack_sgrow (&ସନ୍ଦେଶ_buf, tail);

      ଯଦି (var->err & VARIANT_HIDDEN)
        {
          obstack_printf (&ସନ୍ଦେଶ_buf, _(", hiding %c"), dollar_or_at);
          ଯଦି (contains_dot_or_dash (var->id))
            obstack_printf (&ସନ୍ଦେଶ_buf, "[%s]", var->id);
          ଅନ୍ୟଥା
            obstack_sgrow (&ସନ୍ଦେଶ_buf, var->id);
          obstack_sgrow (&ସନ୍ଦେଶ_buf, tail);
        }

      obstack_printf (&ସନ୍ଦେଶ_buf, _(" at %s"), at_spec);

      ଯଦି (var->err & VARIANT_NOT_VISIBLE_FROM_MIDRULE)
        obstack_printf (&ସନ୍ଦେଶ_buf,
                        _(", cannot be accessed from midrule action at $%d"),
                        midrule_rhs_index);

      subcomplain (&id_loc, warning, "%s",
                   obstack_finish0 (&ସନ୍ଦେଶ_buf));
      obstack_free (&ସନ୍ଦେଶ_buf, 0);
    }
}

ସ୍ଥିର ଶୂନ୍ୟ
show_sub_messages (warnings warning,
                   ଅଚଳ ଅକ୍ଷର* cp, bool explicit_bracketing,
                   ପୂର୍ଣ୍ଣସଂଖ୍ୟା midrule_rhs_index, ଅକ୍ଷର dollar_or_at)
{
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < variant_ଗଣନା; ++i)
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
ସ୍ଥିର long
parse_positional_ref (ଅକ୍ଷର *cp, ପୂର୍ଣ୍ଣସଂଖ୍ୟା rule_length,
                      ଅକ୍ଷର *text, ଅଚଳ location *text_loc)
{
  long ଅଙ୍କ = strtol (cp, &cp, 10);
  ଯଦି (1 - INT_MAX + rule_length <= ଅଙ୍କ && ଅଙ୍କ <= rule_length)
    ଫେରନ୍ତୁ ଅଙ୍କ;
  ଅନ୍ୟଥା
    {
      complain (text_loc, complaint, _("integer out of range: %s"),
                quote (text));
      ଫେରନ୍ତୁ INVALID_REF;
    }
}


/* Parse named or positional reference. In case of positional
   references, can return negative values for $-n "deep" stack
   accesses. */
ସ୍ଥିର long
parse_ref (ଅକ୍ଷର *cp, ପ୍ରତୀକ_list *rule, ପୂର୍ଣ୍ଣସଂଖ୍ୟା rule_length,
           ପୂର୍ଣ୍ଣସଂଖ୍ୟା midrule_rhs_index, ଅକ୍ଷର *text, ଅଚଳ location *text_loc,
           ଅକ୍ଷର dollar_or_at)
{
  ଯଦି ('$' == *cp)
    ଫେରନ୍ତୁ LHS_REF;

  ଯଦି (c_isdigit (*cp) || (*cp == '-' && c_isdigit (cp[1])))
    ଫେରନ୍ତୁ parse_positional_ref (cp, rule_length, text, text_loc);

  bool ଅଚଳ explicit_bracketing = *cp == '[';

  ଯଦି (explicit_bracketing)
    ++cp;
  ଅନ୍ୟଥା
    ref_tail_fields = strpbrk (cp, ".-");

  ଅକ୍ଷର ଅଚଳ *cp_end = strchr (cp, explicit_bracketing ? ']' : '\0');

  /* Add all relevant variants. */
  {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ପ୍ରତୀକ_index;
    ପ୍ରତୀକ_list *l;
    variant_ଗଣନା = 0;
    ପାଇଁ (ପ୍ରତୀକ_index = 0, l = rule; !ପ୍ରତୀକ_list_null (l);
         ++ପ୍ରତୀକ_index, l = l->next)
      {
        ଯଦି (l->content_type != SYMLIST_ପ୍ରତୀକ)
          ଜାରି;

        variant *var
          = variant_add (l->content.sym->tag, l->sym_loc,
                         ପ୍ରତୀକ_index, cp, cp_end, explicit_bracketing);
        ଯଦି (var && l->named_ref)
          var->hidden_by = l->named_ref;

        ଯଦି (l->named_ref)
          variant_add (l->named_ref->id, l->named_ref->loc,
                       ପ୍ରତୀକ_index, cp, cp_end, explicit_bracketing);
      }
  }

  /* Check errors. */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା valid_variants = 0;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା valid_variant_index = 0;
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < variant_ଗଣନା; ++i)
    {
      variant *var = &variant_table[i];
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ପ୍ରତୀକ_index = var->ପ୍ରତୀକ_index;

      /* Check visibility from midrule actions. */
      ଯଦି (midrule_rhs_index != 0
          && (ପ୍ରତୀକ_index == 0 || midrule_rhs_index < ପ୍ରତୀକ_index))
        var->err |= VARIANT_NOT_VISIBLE_FROM_MIDRULE;

      /* Check correct bracketing. */
      ଯଦି (!explicit_bracketing && contains_dot_or_dash (var->id))
        var->err |= VARIANT_BAD_BRACKETING;

      /* Check using of hidden symbols. */
      ଯଦି (var->hidden_by)
        var->err |= VARIANT_HIDDEN;

      ଯଦି (!var->err)
        {
          valid_variant_index = i;
          ++valid_variants;
        }
    }

  ବାଛ (valid_variants)
    {
    ସ୍ଥିତି 0:
      {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା len = (explicit_bracketing || !ref_tail_fields) ?
          cp_end - cp : ref_tail_fields - cp;

        complain (text_loc, complaint,
                  _("invalid reference: %s"), quote (text));
        ଯଦି (len == 0)
          {
            location sym_loc = *text_loc;
            sym_loc.start.column += 1;
            sym_loc.end = sym_loc.start;
            subcomplain (&sym_loc, complaint,
                         _("syntax error after '%c', expecting integer, "
                         "letter, '_', '[', or '$'"),
                         dollar_or_at);
          }
        ଅନ୍ୟଥା ଯଦି (midrule_rhs_index)
          subcomplain (&rule->rhs_loc, complaint,
                       _("symbol not found in production before $%d: "
                       "%.*s"),
                       midrule_rhs_index, len, cp);
        ଅନ୍ୟଥା
          subcomplain (&rule->rhs_loc, complaint,
                       _("symbol not found in production: %.*s"),
                       len, cp);

        ଯଦି (variant_ଗଣନା > 0)
          show_sub_messages (complaint,
                             cp, explicit_bracketing, midrule_rhs_index,
                             dollar_or_at);
        ଫେରନ୍ତୁ INVALID_REF;
      }
    ସ୍ଥିତି 1:
      {
        ଯଦି (variant_ଗଣନା > 1)
          {
            complain (text_loc, Wother,
                      _("misleading reference: %s"), quote (text));
            show_sub_messages (Wother,
                               cp, explicit_bracketing, midrule_rhs_index,
                               dollar_or_at);
          }
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା ପ୍ରତୀକ_index =
            variant_table[valid_variant_index].ପ୍ରତୀକ_index;
          ଫେରନ୍ତୁ (ପ୍ରତୀକ_index == midrule_rhs_index) ? LHS_REF : ପ୍ରତୀକ_index;
        }
      }
    ସ୍ଥିତି 2:
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      {
        complain (text_loc, complaint,
                  _("ambiguous reference: %s"), quote (text));
        show_sub_messages (complaint,
                           cp, explicit_bracketing, midrule_rhs_index,
                           dollar_or_at);
        ଫେରନ୍ତୁ INVALID_REF;
      }
    }
}

/* Keeps track of the maximum number of semantic values to the left of
   a handle (those referenced by $0, $-1, etc.) are required by the
   semantic actions of this grammar. */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_ବାମ_semantic_context = 0;


/* If CP points to a typename (i.e., <.*?>), set TYPE_NAME to its
   beginning (i.e., after the opening "<", and return the pointer
   immediately after it.  */

ସ୍ଥିର
ଅକ୍ଷର *
fetch_type_name (ଅକ୍ଷର *cp, ଅକ୍ଷର ଅଚଳ **type_name,
                 ଅଚଳ location *dollar_loc)
{
  ଯଦି (*cp == '<')
    {
      *type_name = ++cp;
      /* Series of non-'>' or "->".  */
      ଯେତେବେଳେ (*cp != '>' || cp[-1] == '-')
        ++cp;

      /* The '>' symbol will be later replaced by '\0'. Original
         'text' is needed for error messages. */
      ++cp;
      ଯଦି (untyped_var_seen)
        complain (dollar_loc, complaint,
                  _("explicit type given in untyped grammar"));
      tag_seen = true;
    }
  ଫେରନ୍ତୁ cp;
}

/*------------------------------------------------------------------.
| TEXT is pointing to a wannabee semantic value (i.e., a '$').      |
|                                                                   |
| Possible inputs: $[<TYPENAME>]($|INTEGER)                         |
|                                                                   |
| Output to OBSTACK_FOR_STRING a reference to this semantic value.  |
`------------------------------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
handle_action_dollar (ପ୍ରତୀକ_list *rule, ଅକ୍ଷର *text, ଅଚଳ location *dollar_loc)
{
  ପ୍ରତୀକ_list *effective_rule;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା effective_rule_length;

  ଯଦି (rule->midrule_parent_rule)
    {
      effective_rule = rule->midrule_parent_rule;
      effective_rule_length = rule->midrule_parent_rhs_index - 1;
    }
  ଅନ୍ୟଥା
    {
      effective_rule = rule;
      effective_rule_length = ପ୍ରତୀକ_list_length (rule->next);
    }

  /* The type name if explicit, otherwise left null. */
  ଅକ୍ଷର ଅଚଳ *type_name = ଖାଲି;
  ଅକ୍ଷର *cp = fetch_type_name (text + 1, &type_name, dollar_loc);
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = parse_ref (cp, effective_rule, effective_rule_length,
                     rule->midrule_parent_rhs_index, text, dollar_loc, '$');
  /* End type_name.  Don't do it earlier: parse_ref depends on TEXT.  */
  ଯଦି (type_name)
    cp[-1] = '\0';

  ବାଛ (n)
    {
    ସ୍ଥିତି INVALID_REF:
      ଭାଙ୍ଗ;

    ସ୍ଥିତି LHS_REF:
      {
        ପ୍ରତୀକ_list *sym = ପ୍ରତୀକ_list_n_get (rule, 0);
        ଯଦି (!type_name
            && !sym->content.sym->content->type_name)
          {
            ଯଦି (union_seen || tag_seen)
              {
                ଯଦି (rule->midrule_parent_rule)
                  complain (dollar_loc, complaint,
                            _("$$ for the midrule at $%d of %s"
                              " has no declared type"),
                            rule->midrule_parent_rhs_index,
                            quote (effective_rule->content.sym->tag));
                ଅନ୍ୟଥା
                  complain (dollar_loc, complaint,
                            _("$$ of %s has no declared type"),
                            quote (rule->content.sym->tag));
              }
            ଅନ୍ୟଥା
              untyped_var_seen = true;
          }

        obstack_printf (&obstack_for_string, "]b4_lhs_value(orig %d, ",
                        sym->content.sym->content->ସଂଖ୍ୟା);
        obstack_quote (&obstack_for_string, type_name);
        obstack_sgrow (&obstack_for_string, ")[");
        rule->action_props.is_ମୂଲ୍ୟ_used = true;
      }
      ଭାଙ୍ଗ;

      /* Reference to a RHS symbol.  */
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      {
        ଯଦି (max_ବାମ_semantic_context < 1 - n)
          max_ବାମ_semantic_context = 1 - n;
        ପ୍ରତୀକ_list *sym = 0 < n ? ପ୍ରତୀକ_list_n_get (effective_rule, n) : ଖାଲି;
        ଯଦି (!type_name
            && (!sym || !sym->content.sym->content->type_name))
          {
            ଯଦି (union_seen || tag_seen)
              complain (dollar_loc, complaint,
                        _("$%s of %s has no declared type"), cp,
                        quote (effective_rule->content.sym->tag));
            ଅନ୍ୟଥା
              untyped_var_seen = true;
          }

        obstack_printf (&obstack_for_string,
                        "]b4_rhs_value(%d, %d, ",
                        effective_rule_length, n);
        ଯଦି (sym)
          obstack_printf (&obstack_for_string, "%s%d, ",
                          sym->content.sym->content->class == nterm_sym ? "orig " : "",
                          sym->content.sym->content->ସଂଖ୍ୟା);
        ଅନ୍ୟଥା
          obstack_sgrow (&obstack_for_string, "[], ");

        obstack_quote (&obstack_for_string, type_name);
        obstack_sgrow (&obstack_for_string, ")[");
        ଯଦି (0 < n)
          {
            ଯଦି (muscle_percent_define_ifdef ("api.value.automove")
                && sym->action_props.is_ମୂଲ୍ୟ_used)
              complain (dollar_loc, Wother,
                        _("multiple occurrences of $%d with api.value.automove"),
                        n);
            sym->action_props.is_ମୂଲ୍ୟ_used = true;
          }
      }
      ଭାଙ୍ଗ;
    }
}


/*------------------------------------------------------.
| TEXT is a location token (i.e., a '@...').  Output to |
| OBSTACK_FOR_STRING a reference to this location.      |
`------------------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
handle_action_at (ପ୍ରତୀକ_list *rule, ଅକ୍ଷର *text, ଅଚଳ location *at_loc)
{
  ପ୍ରତୀକ_list *effective_rule;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା effective_rule_length;

  ଯଦି (rule->midrule_parent_rule)
    {
      effective_rule = rule->midrule_parent_rule;
      effective_rule_length = rule->midrule_parent_rhs_index - 1;
    }
  ଅନ୍ୟଥା
    {
      effective_rule = rule;
      effective_rule_length = ପ୍ରତୀକ_list_length (rule->next);
    }

  muscle_percent_define_ensure ("locations", *at_loc, true);

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = parse_ref (text + 1, effective_rule, effective_rule_length,
                     rule->midrule_parent_rhs_index, text, at_loc, '@');
  ବାଛ (n)
    {
    ସ୍ଥିତି INVALID_REF:
      ଭାଙ୍ଗ;

    ସ୍ଥିତି LHS_REF:
      obstack_sgrow (&obstack_for_string, "]b4_lhs_location[");
      ଭାଙ୍ଗ;

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      obstack_printf (&obstack_for_string, "]b4_rhs_location(%d, %d)[",
                      effective_rule_length, n);
      ଭାଙ୍ଗ;
    }
}


/*-------------------------.
| Initialize the scanner.  |
`-------------------------*/

/* Translate the '$...' and '@...' in \a self, in the context \a
   sc_context (SC_RULE_ACTION, SC_SYMBOL_ACTION, INITIAL).  */

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *
translate_action (code_props *self, ପୂର୍ଣ୍ଣସଂଖ୍ୟା sc_context)
{
  loc->start = loc->end = self->location.start;
  yy_switch_to_buffer (yy_scan_string (self->code));
  ଅକ୍ଷର *res = code_lex (self, sc_context);
  yy_delete_buffer (YY_CURRENT_BUFFER);
  ଫେରନ୍ତୁ res;
}

/*------------------------------------------------------------------------.
| Implementation of the public interface as documented in "scan-code.h".  |
`------------------------------------------------------------------------*/

ଶୂନ୍ୟ
code_props_none_init (code_props *self)
{
  *self = code_props_none;
}

code_props code_props_none = CODE_PROPS_NONE_INIT;

ଶୂନ୍ୟ
code_props_plain_init (code_props *self, ଅକ୍ଷର ଅଚଳ *code,
                       location code_loc)
{
  code_props_none_init (self);
  self->kind = CODE_PROPS_PLAIN;
  self->code = code;
  self->location = code_loc;
}

ଶୂନ୍ୟ
code_props_ପ୍ରତୀକ_action_init (code_props *self, ଅକ୍ଷର ଅଚଳ *code,
                               location code_loc)
{
  code_props_none_init (self);
  self->kind = CODE_PROPS_ପ୍ରତୀକ_ACTION;
  self->code = code;
  self->location = code_loc;
}

ଶୂନ୍ୟ
code_props_rule_action_init (code_props *self, ଅକ୍ଷର ଅଚଳ *code,
                             location code_loc, ପ୍ରତୀକ_list *rule,
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

ଶୂନ୍ୟ
code_props_translate_code (code_props *self)
{
  ବାଛ (self->kind)
    {
    ସ୍ଥିତି CODE_PROPS_NONE:
      ଭାଙ୍ଗ;
    ସ୍ଥିତି CODE_PROPS_PLAIN:
      self->code = translate_action (self, INITIAL);
      ଭାଙ୍ଗ;
    ସ୍ଥିତି CODE_PROPS_ପ୍ରତୀକ_ACTION:
      self->code = translate_action (self, SC_ପ୍ରତୀକ_ACTION);
      ଭାଙ୍ଗ;
    ସ୍ଥିତି CODE_PROPS_RULE_ACTION:
      self->code = translate_action (self, SC_RULE_ACTION);
      ଭାଙ୍ଗ;
    }
}

ଶୂନ୍ୟ
code_scanner_last_string_free (ଶୂନ୍ୟ)
{
  STRING_FREE ();
}

ଶୂନ୍ୟ
code_scanner_init (ଶୂନ୍ୟ)
{
  obstack_init (&obstack_for_string);
  yy_flex_debug = 0;
}

ଶୂନ୍ୟ
code_scanner_free (ଶୂନ୍ୟ)
{
  obstack_free (&obstack_for_string, 0);
  variant_table_free ();

  /* Reclaim Flex's buffers.  */
  yylex_destroy ();
}
