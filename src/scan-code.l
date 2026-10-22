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
#অন্তর্ভাব <c-ctype.h>
#অন্তর্ভাব <get-errno.h>
#অন্তর্ভাব <quote.h>

#অন্তর্ভাব "src/complain.h"
#অন্তর্ভাব "src/getargs.h"
#অন্তর্ভাব "src/muscle-tab.h"
#অন্তর্ভাব "src/reader.h"
#অন্তর্ভাব "src/scan-code.h"
#অন্তর্ভাব "src/symlist.h"

#define FLEX_PREFIX(Id) code_ ## Id
#অন্তর্ভাব "src/flex-scanner.h"

/* Work around a bug in flex 2.5.31.  See Debian bug 333231
   <https://bugs.debian.org/333231>.  */
#undef code_wrap
#define code_wrap() 1

সংরচনা obstack *obstack_for_actions = &obstack_for_string;

/* The current calling start condition: SC_RULE_ACTION or
   SC_SYMBOL_ACTION. */
# define YY_DECL স্থির বর্ণ *code_lex (code_props *self, পূর্ণাঙ্ক sc_context)
YY_DECL;

#define YY_USER_ACTION  location_compute (loc, &loc->end, yytext, yyleng);

স্থির বর্ণ *fetch_type_name (বর্ণ *cp, বর্ণ ধ্রুব **type_name,
                              ধ্রুব location *dollar_loc);

স্থির শূন্য handle_action_dollar (প্রতীক_list *rule, বর্ণ *cp,
                                  ধ্রুব location *dollar_loc);
স্থির শূন্য handle_action_at (প্রতীক_list *rule, বর্ণ *cp,
                              ধ্রুব location *at_loc);

/* A string to be pushed to obstack after dollar/at has been handled. */
স্থির বর্ণ *ref_tail_fields;

স্থির location current_loc;
স্থির location *loc = &current_loc;

/* A string representing the most recent translation.  */
স্থির বর্ণ *last_string;

/* True if an untyped $$ or $n was seen.  */
স্থির bool untyped_var_seen;

%}
 /* C and C++ comments in code. */
%x SC_COMMENT SC_সারি_COMMENT
 /* Strings and characters in code. */
%x SC_STRING SC_CHARACTER
 /* Whether in a rule or symbol action.  Specifies the translation
    of $ and @.  */
%x SC_RULE_ACTION SC_প্রতীক_ACTION


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
  aver (sc_context == SC_প্রতীক_ACTION
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

<SC_সারি_COMMENT>
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


<SC_RULE_ACTION,SC_প্রতীক_ACTION>
{
  "'"              STRING_GROW (); BEGIN SC_CHARACTER;
  "\""             STRING_GROW (); BEGIN SC_STRING;
  "/"{splice}"*"   STRING_GROW (); BEGIN SC_COMMENT;
  "/"{splice}"/"   STRING_GROW (); BEGIN SC_সারি_COMMENT;

  [$@]  {
    complain (loc, Wother, _("stray '%s'"), yytext);
    obstack_escape (&obstack_for_string, yytext);
  }
}

<SC_RULE_ACTION>
{
  "$"("<"{tag}">")?{ref}  {
    ref_tail_fields = রিক্ত;
    handle_action_dollar (self->rule, yytext, loc);
    যদি (ref_tail_fields)
      obstack_sgrow (&obstack_for_string, ref_tail_fields);
  }
  "@"{ref} {
    ref_tail_fields = রিক্ত;
    handle_action_at (self->rule, yytext, loc);
    যদি (ref_tail_fields)
      obstack_sgrow (&obstack_for_string, ref_tail_fields);
  }
}

<SC_প্রতীক_ACTION>
{
  "$"("<"{tag}">")?"$" {
    ধ্রুব বর্ণ *type_name = রিক্ত;
    fetch_type_name (yytext + 1, &type_name, loc)[-1] = 0;
    obstack_sgrow (&obstack_for_string, "]b4_dollar_dollar(");
    obstack_quote (&obstack_for_string, type_name);
    obstack_sgrow (&obstack_for_string, ")[");
    self->is_মান_used = true;
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
  <<EOF>>     STRING_FINISH (); প্রত্যাগম last_string;
}

%%

স্থির অন্তঃস্থ bool
is_dot_or_dash (বর্ণ ch)
{
  প্রত্যাগম ch == '.' || ch == '-';
}

স্থির অন্তঃস্থ bool
contains_dot_or_dash (ধ্রুব বর্ণ* p)
{
  প্রত্যাগম !!strpbrk (p, ".-");
}

/* Defines a variant of a symbolic name resolution. */
প্রকারনির্দেশ সংরচনা
{
  /* Index in symbol list. */
  পূর্ণাঙ্ক প্রতীক_index;

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

স্থির variant *variant_table = রিক্ত;
স্থির পূর্ণাঙ্ক variant_table_size = 0;
স্থির পূর্ণাঙ্ক variant_গুনতি = 0;

স্থির variant *
variant_table_grow (শূন্য)
{
  ++variant_গুনতি;
  যদি (variant_গুনতি > variant_table_size)
    {
      যাবত্ (variant_গুনতি > variant_table_size)
        variant_table_size = 2 * variant_table_size + 3;
      variant_table = xnrealloc (variant_table, variant_table_size,
                                 পরিমাণ *variant_table);
    }
  প্রত্যাগম &variant_table[variant_count - 1];
}

স্থির শূন্য
variant_table_free (শূন্য)
{
  মুক্ত (variant_table);
  variant_table = রিক্ত;
  variant_table_size = variant_গুনতি = 0;
}

স্থির বর্ণ ধ্রুব *
find_prefix_end (বর্ণ ধ্রুব *prefix, বর্ণ ধ্রুব *cp, বর্ণ ধ্রুব *end)
{
  প্রতি (; *prefix && cp != end; ++prefix, ++cp)
    যদি (*prefix != *cp)
      প্রত্যাগম রিক্ত;

  প্রত্যাগম *prefix ? রিক্ত : cp;
}

স্থির variant *
variant_add (uniqstr id, location id_loc, পূর্ণাঙ্ক প্রতীক_index,
             বর্ণ ধ্রুব *cp, বর্ণ ধ্রুব *cp_end, bool explicit_bracketing)
{
  বর্ণ ধ্রুব *prefix_end = find_prefix_end (id, cp, cp_end);
  যদি (prefix_end &&
      (prefix_end == cp_end ||
       (!explicit_bracketing && is_dot_or_dash (*prefix_end))))
    {
      variant *r = variant_table_grow ();
      r->প্রতীক_index = প্রতীক_index;
      r->id = id;
      r->loc = id_loc;
      r->hidden_by = রিক্ত;
      r->err = 0;
      প্রত্যাগম r;
    }
  অন্যথা
    প্রত্যাগম রিক্ত;
}

স্থির ধ্রুব বর্ণ *
get_at_spec(পূর্ণাঙ্ক প্রতীক_index)
{
  স্থির বর্ণ at_buf[20];
  যদি (প্রতীক_index == 0)
    strcpy (at_buf, "$$");
  অন্যথা
    snprintf (at_buf, পরিমাণ at_buf, "$%u", প্রতীক_index);
  প্রত্যাগম at_buf;
}

স্থির শূন্য
show_sub_message (warnings warning,
                  ধ্রুব বর্ণ* cp, bool explicit_bracketing,
                  পূর্ণাঙ্ক midrule_rhs_index, বর্ণ dollar_or_at,
                  ধ্রুব variant *var)
{
  ধ্রুব বর্ণ *at_spec = get_at_spec (var->প্রতীক_index);

  যদি (var->err == 0)
    subcomplain (&var->loc, warning,
                 _("refers to: %c%s at %s"), dollar_or_at,
                 var->id, at_spec);
  অন্যথা
    {
      ধ্রুব বর্ণ *id;
      location id_loc;

      যদি (var->hidden_by)
        {
          id = var->hidden_by->id;
          id_loc = var->hidden_by->loc;
        }
      অন্যথা
        {
          id = var->id;
          id_loc = var->loc;
        }

      ধ্রুব বর্ণ *tail = explicit_bracketing ? "" : cp + strlen (var->id);

      /* Create the explanation message. */
      স্থির সংরচনা obstack বার্তা_buf;
      obstack_init (&বার্তা_buf);

      obstack_printf (&বার্তা_buf, _("possibly meant: %c"), dollar_or_at);
      যদি (contains_dot_or_dash (id))
        obstack_printf (&বার্তা_buf, "[%s]", id);
      অন্যথা
        obstack_sgrow (&বার্তা_buf, id);
      obstack_sgrow (&বার্তা_buf, tail);

      যদি (var->err & VARIANT_HIDDEN)
        {
          obstack_printf (&বার্তা_buf, _(", hiding %c"), dollar_or_at);
          যদি (contains_dot_or_dash (var->id))
            obstack_printf (&বার্তা_buf, "[%s]", var->id);
          অন্যথা
            obstack_sgrow (&বার্তা_buf, var->id);
          obstack_sgrow (&বার্তা_buf, tail);
        }

      obstack_printf (&বার্তা_buf, _(" at %s"), at_spec);

      যদি (var->err & VARIANT_NOT_VISIBLE_FROM_MIDRULE)
        obstack_printf (&বার্তা_buf,
                        _(", cannot be accessed from midrule action at $%d"),
                        midrule_rhs_index);

      subcomplain (&id_loc, warning, "%s",
                   obstack_finish0 (&বার্তা_buf));
      obstack_free (&বার্তা_buf, 0);
    }
}

স্থির শূন্য
show_sub_messages (warnings warning,
                   ধ্রুব বর্ণ* cp, bool explicit_bracketing,
                   পূর্ণাঙ্ক midrule_rhs_index, বর্ণ dollar_or_at)
{
  প্রতি (পূর্ণাঙ্ক i = 0; i < variant_গুনতি; ++i)
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
স্থির long
parse_positional_ref (বর্ণ *cp, পূর্ণাঙ্ক rule_length,
                      বর্ণ *text, ধ্রুব location *text_loc)
{
  long অঙ্ক = strtol (cp, &cp, 10);
  যদি (1 - INT_MAX + rule_length <= অঙ্ক && অঙ্ক <= rule_length)
    প্রত্যাগম অঙ্ক;
  অন্যথা
    {
      complain (text_loc, complaint, _("integer out of range: %s"),
                quote (text));
      প্রত্যাগম INVALID_REF;
    }
}


/* Parse named or positional reference. In case of positional
   references, can return negative values for $-n "deep" stack
   accesses. */
স্থির long
parse_ref (বর্ণ *cp, প্রতীক_list *rule, পূর্ণাঙ্ক rule_length,
           পূর্ণাঙ্ক midrule_rhs_index, বর্ণ *text, ধ্রুব location *text_loc,
           বর্ণ dollar_or_at)
{
  যদি ('$' == *cp)
    প্রত্যাগম LHS_REF;

  যদি (c_isdigit (*cp) || (*cp == '-' && c_isdigit (cp[1])))
    প্রত্যাগম parse_positional_ref (cp, rule_length, text, text_loc);

  bool ধ্রুব explicit_bracketing = *cp == '[';

  যদি (explicit_bracketing)
    ++cp;
  অন্যথা
    ref_tail_fields = strpbrk (cp, ".-");

  বর্ণ ধ্রুব *cp_end = strchr (cp, explicit_bracketing ? ']' : '\0');

  /* Add all relevant variants. */
  {
    পূর্ণাঙ্ক প্রতীক_index;
    প্রতীক_list *l;
    variant_গুনতি = 0;
    প্রতি (প্রতীক_index = 0, l = rule; !প্রতীক_list_null (l);
         ++প্রতীক_index, l = l->next)
      {
        যদি (l->content_type != SYMLIST_প্রতীক)
          অনুবর্ত;

        variant *var
          = variant_add (l->content.sym->tag, l->sym_loc,
                         প্রতীক_index, cp, cp_end, explicit_bracketing);
        যদি (var && l->named_ref)
          var->hidden_by = l->named_ref;

        যদি (l->named_ref)
          variant_add (l->named_ref->id, l->named_ref->loc,
                       প্রতীক_index, cp, cp_end, explicit_bracketing);
      }
  }

  /* Check errors. */
  পূর্ণাঙ্ক valid_variants = 0;
  পূর্ণাঙ্ক valid_variant_index = 0;
  প্রতি (পূর্ণাঙ্ক i = 0; i < variant_গুনতি; ++i)
    {
      variant *var = &variant_table[i];
      পূর্ণাঙ্ক প্রতীক_index = var->প্রতীক_index;

      /* Check visibility from midrule actions. */
      যদি (midrule_rhs_index != 0
          && (প্রতীক_index == 0 || midrule_rhs_index < প্রতীক_index))
        var->err |= VARIANT_NOT_VISIBLE_FROM_MIDRULE;

      /* Check correct bracketing. */
      যদি (!explicit_bracketing && contains_dot_or_dash (var->id))
        var->err |= VARIANT_BAD_BRACKETING;

      /* Check using of hidden symbols. */
      যদি (var->hidden_by)
        var->err |= VARIANT_HIDDEN;

      যদি (!var->err)
        {
          valid_variant_index = i;
          ++valid_variants;
        }
    }

  বিকল্প (valid_variants)
    {
    পক্ষ 0:
      {
        পূর্ণাঙ্ক len = (explicit_bracketing || !ref_tail_fields) ?
          cp_end - cp : ref_tail_fields - cp;

        complain (text_loc, complaint,
                  _("invalid reference: %s"), quote (text));
        যদি (len == 0)
          {
            location sym_loc = *text_loc;
            sym_loc.start.column += 1;
            sym_loc.end = sym_loc.start;
            subcomplain (&sym_loc, complaint,
                         _("syntax error after '%c', expecting integer, "
                         "letter, '_', '[', or '$'"),
                         dollar_or_at);
          }
        অন্যথা যদি (midrule_rhs_index)
          subcomplain (&rule->rhs_loc, complaint,
                       _("symbol not found in production before $%d: "
                       "%.*s"),
                       midrule_rhs_index, len, cp);
        অন্যথা
          subcomplain (&rule->rhs_loc, complaint,
                       _("symbol not found in production: %.*s"),
                       len, cp);

        যদি (variant_গুনতি > 0)
          show_sub_messages (complaint,
                             cp, explicit_bracketing, midrule_rhs_index,
                             dollar_or_at);
        প্রত্যাগম INVALID_REF;
      }
    পক্ষ 1:
      {
        যদি (variant_গুনতি > 1)
          {
            complain (text_loc, Wother,
                      _("misleading reference: %s"), quote (text));
            show_sub_messages (Wother,
                               cp, explicit_bracketing, midrule_rhs_index,
                               dollar_or_at);
          }
        {
          পূর্ণাঙ্ক প্রতীক_index =
            variant_table[valid_variant_index].প্রতীক_index;
          প্রত্যাগম (প্রতীক_index == midrule_rhs_index) ? LHS_REF : প্রতীক_index;
        }
      }
    পক্ষ 2:
    স্বভাব:
      {
        complain (text_loc, complaint,
                  _("ambiguous reference: %s"), quote (text));
        show_sub_messages (complaint,
                           cp, explicit_bracketing, midrule_rhs_index,
                           dollar_or_at);
        প্রত্যাগম INVALID_REF;
      }
    }
}

/* Keeps track of the maximum number of semantic values to the left of
   a handle (those referenced by $0, $-1, etc.) are required by the
   semantic actions of this grammar. */
পূর্ণাঙ্ক max_বাম_semantic_context = 0;


/* If CP points to a typename (i.e., <.*?>), set TYPE_NAME to its
   beginning (i.e., after the opening "<", and return the pointer
   immediately after it.  */

স্থির
বর্ণ *
fetch_type_name (বর্ণ *cp, বর্ণ ধ্রুব **type_name,
                 ধ্রুব location *dollar_loc)
{
  যদি (*cp == '<')
    {
      *type_name = ++cp;
      /* Series of non-'>' or "->".  */
      যাবত্ (*cp != '>' || cp[-1] == '-')
        ++cp;

      /* The '>' symbol will be later replaced by '\0'. Original
         'text' is needed for error messages. */
      ++cp;
      যদি (untyped_var_seen)
        complain (dollar_loc, complaint,
                  _("explicit type given in untyped grammar"));
      tag_seen = true;
    }
  প্রত্যাগম cp;
}

/*------------------------------------------------------------------.
| TEXT is pointing to a wannabee semantic value (i.e., a '$').      |
|                                                                   |
| Possible inputs: $[<TYPENAME>]($|INTEGER)                         |
|                                                                   |
| Output to OBSTACK_FOR_STRING a reference to this semantic value.  |
`------------------------------------------------------------------*/

স্থির শূন্য
handle_action_dollar (প্রতীক_list *rule, বর্ণ *text, ধ্রুব location *dollar_loc)
{
  প্রতীক_list *effective_rule;
  পূর্ণাঙ্ক effective_rule_length;

  যদি (rule->midrule_parent_rule)
    {
      effective_rule = rule->midrule_parent_rule;
      effective_rule_length = rule->midrule_parent_rhs_index - 1;
    }
  অন্যথা
    {
      effective_rule = rule;
      effective_rule_length = প্রতীক_list_length (rule->next);
    }

  /* The type name if explicit, otherwise left null. */
  বর্ণ ধ্রুব *type_name = রিক্ত;
  বর্ণ *cp = fetch_type_name (text + 1, &type_name, dollar_loc);
  পূর্ণাঙ্ক n = parse_ref (cp, effective_rule, effective_rule_length,
                     rule->midrule_parent_rhs_index, text, dollar_loc, '$');
  /* End type_name.  Don't do it earlier: parse_ref depends on TEXT.  */
  যদি (type_name)
    cp[-1] = '\0';

  বিকল্প (n)
    {
    পক্ষ INVALID_REF:
      ভঙ্গ;

    পক্ষ LHS_REF:
      {
        প্রতীক_list *sym = প্রতীক_list_n_get (rule, 0);
        যদি (!type_name
            && !sym->content.sym->content->type_name)
          {
            যদি (union_seen || tag_seen)
              {
                যদি (rule->midrule_parent_rule)
                  complain (dollar_loc, complaint,
                            _("$$ for the midrule at $%d of %s"
                              " has no declared type"),
                            rule->midrule_parent_rhs_index,
                            quote (effective_rule->content.sym->tag));
                অন্যথা
                  complain (dollar_loc, complaint,
                            _("$$ of %s has no declared type"),
                            quote (rule->content.sym->tag));
              }
            অন্যথা
              untyped_var_seen = true;
          }

        obstack_printf (&obstack_for_string, "]b4_lhs_value(orig %d, ",
                        sym->content.sym->content->সংখ্যা);
        obstack_quote (&obstack_for_string, type_name);
        obstack_sgrow (&obstack_for_string, ")[");
        rule->action_props.is_মান_used = true;
      }
      ভঙ্গ;

      /* Reference to a RHS symbol.  */
    স্বভাব:
      {
        যদি (max_বাম_semantic_context < 1 - n)
          max_বাম_semantic_context = 1 - n;
        প্রতীক_list *sym = 0 < n ? প্রতীক_list_n_get (effective_rule, n) : রিক্ত;
        যদি (!type_name
            && (!sym || !sym->content.sym->content->type_name))
          {
            যদি (union_seen || tag_seen)
              complain (dollar_loc, complaint,
                        _("$%s of %s has no declared type"), cp,
                        quote (effective_rule->content.sym->tag));
            অন্যথা
              untyped_var_seen = true;
          }

        obstack_printf (&obstack_for_string,
                        "]b4_rhs_value(%d, %d, ",
                        effective_rule_length, n);
        যদি (sym)
          obstack_printf (&obstack_for_string, "%s%d, ",
                          sym->content.sym->content->class == nterm_sym ? "orig " : "",
                          sym->content.sym->content->সংখ্যা);
        অন্যথা
          obstack_sgrow (&obstack_for_string, "[], ");

        obstack_quote (&obstack_for_string, type_name);
        obstack_sgrow (&obstack_for_string, ")[");
        যদি (0 < n)
          {
            যদি (muscle_percent_define_ifdef ("api.value.automove")
                && sym->action_props.is_মান_used)
              complain (dollar_loc, Wother,
                        _("multiple occurrences of $%d with api.value.automove"),
                        n);
            sym->action_props.is_মান_used = true;
          }
      }
      ভঙ্গ;
    }
}


/*------------------------------------------------------.
| TEXT is a location token (i.e., a '@...').  Output to |
| OBSTACK_FOR_STRING a reference to this location.      |
`------------------------------------------------------*/

স্থির শূন্য
handle_action_at (প্রতীক_list *rule, বর্ণ *text, ধ্রুব location *at_loc)
{
  প্রতীক_list *effective_rule;
  পূর্ণাঙ্ক effective_rule_length;

  যদি (rule->midrule_parent_rule)
    {
      effective_rule = rule->midrule_parent_rule;
      effective_rule_length = rule->midrule_parent_rhs_index - 1;
    }
  অন্যথা
    {
      effective_rule = rule;
      effective_rule_length = প্রতীক_list_length (rule->next);
    }

  muscle_percent_define_ensure ("locations", *at_loc, true);

  পূর্ণাঙ্ক n = parse_ref (text + 1, effective_rule, effective_rule_length,
                     rule->midrule_parent_rhs_index, text, at_loc, '@');
  বিকল্প (n)
    {
    পক্ষ INVALID_REF:
      ভঙ্গ;

    পক্ষ LHS_REF:
      obstack_sgrow (&obstack_for_string, "]b4_lhs_location[");
      ভঙ্গ;

    স্বভাব:
      obstack_printf (&obstack_for_string, "]b4_rhs_location(%d, %d)[",
                      effective_rule_length, n);
      ভঙ্গ;
    }
}


/*-------------------------.
| Initialize the scanner.  |
`-------------------------*/

/* Translate the '$...' and '@...' in \a self, in the context \a
   sc_context (SC_RULE_ACTION, SC_SYMBOL_ACTION, INITIAL).  */

স্থির বর্ণ ধ্রুব *
translate_action (code_props *self, পূর্ণাঙ্ক sc_context)
{
  loc->start = loc->end = self->location.start;
  yy_switch_to_buffer (yy_scan_string (self->code));
  বর্ণ *res = code_lex (self, sc_context);
  yy_delete_buffer (YY_CURRENT_BUFFER);
  প্রত্যাগম res;
}

/*------------------------------------------------------------------------.
| Implementation of the public interface as documented in "scan-code.h".  |
`------------------------------------------------------------------------*/

শূন্য
code_props_none_init (code_props *self)
{
  *self = code_props_none;
}

code_props code_props_none = CODE_PROPS_NONE_INIT;

শূন্য
code_props_plain_init (code_props *self, বর্ণ ধ্রুব *code,
                       location code_loc)
{
  code_props_none_init (self);
  self->kind = CODE_PROPS_PLAIN;
  self->code = code;
  self->location = code_loc;
}

শূন্য
code_props_প্রতীক_action_init (code_props *self, বর্ণ ধ্রুব *code,
                               location code_loc)
{
  code_props_none_init (self);
  self->kind = CODE_PROPS_প্রতীক_ACTION;
  self->code = code;
  self->location = code_loc;
}

শূন্য
code_props_rule_action_init (code_props *self, বর্ণ ধ্রুব *code,
                             location code_loc, প্রতীক_list *rule,
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

শূন্য
code_props_translate_code (code_props *self)
{
  বিকল্প (self->kind)
    {
    পক্ষ CODE_PROPS_NONE:
      ভঙ্গ;
    পক্ষ CODE_PROPS_PLAIN:
      self->code = translate_action (self, INITIAL);
      ভঙ্গ;
    পক্ষ CODE_PROPS_প্রতীক_ACTION:
      self->code = translate_action (self, SC_প্রতীক_ACTION);
      ভঙ্গ;
    পক্ষ CODE_PROPS_RULE_ACTION:
      self->code = translate_action (self, SC_RULE_ACTION);
      ভঙ্গ;
    }
}

শূন্য
code_scanner_last_string_free (শূন্য)
{
  STRING_FREE ();
}

শূন্য
code_scanner_init (শূন্য)
{
  obstack_init (&obstack_for_string);
  yy_flex_debug = 0;
}

শূন্য
code_scanner_free (শূন্য)
{
  obstack_free (&obstack_for_string, 0);
  variant_table_free ();

  /* Reclaim Flex's buffers.  */
  yylex_destroy ();
}
