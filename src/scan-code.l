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
#अन्तर्भाव <c-ctype.h>
#अन्तर्भाव <get-errno.h>
#अन्तर्भाव <quote.h>

#अन्तर्भाव "src/complain.h"
#अन्तर्भाव "src/getargs.h"
#अन्तर्भाव "src/muscle-tab.h"
#अन्तर्भाव "src/reader.h"
#अन्तर्भाव "src/scan-code.h"
#अन्तर्भाव "src/symlist.h"

#define FLEX_PREFIX(Id) code_ ## Id
#अन्तर्भाव "src/flex-scanner.h"

/* Work around a bug in flex 2.5.31.  See Debian bug 333231
   <https://bugs.debian.org/333231>.  */
#undef code_wrap
#define code_wrap() 1

संरचना obstack *obstack_for_actions = &obstack_for_string;

/* The current calling start condition: SC_RULE_ACTION or
   SC_SYMBOL_ACTION. */
# define YY_DECL स्थिर वर्ण *code_lex (code_props *self, पूर्णाङ्क sc_context)
YY_DECL;

#define YY_USER_ACTION  location_compute (loc, &loc->end, yytext, yyleng);

स्थिर वर्ण *fetch_type_name (वर्ण *cp, वर्ण ध्रुव **type_name,
                              ध्रुव location *dollar_loc);

स्थिर शून्य handle_action_dollar (चिह्नम्_list *rule, वर्ण *cp,
                                  ध्रुव location *dollar_loc);
स्थिर शून्य handle_action_at (चिह्नम्_list *rule, वर्ण *cp,
                              ध्रुव location *at_loc);

/* A string to be pushed to obstack after dollar/at has been handled. */
स्थिर वर्ण *ref_tail_fields;

स्थिर location current_loc;
स्थिर location *loc = &current_loc;

/* A string representing the most recent translation.  */
स्थिर वर्ण *last_string;

/* True if an untyped $$ or $n was seen.  */
स्थिर bool untyped_var_seen;

%}
 /* C and C++ comments in code. */
%x SC_COMMENT SC_पङ्क्तिः_COMMENT
 /* Strings and characters in code. */
%x SC_STRING SC_CHARACTER
 /* Whether in a rule or symbol action.  Specifies the translation
    of $ and @.  */
%x SC_RULE_ACTION SC_चिह्नम्_ACTION


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
  aver (sc_context == SC_चिह्नम्_ACTION
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

<SC_पङ्क्तिः_COMMENT>
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


<SC_RULE_ACTION,SC_चिह्नम्_ACTION>
{
  "'"              STRING_GROW (); BEGIN SC_CHARACTER;
  "\""             STRING_GROW (); BEGIN SC_STRING;
  "/"{splice}"*"   STRING_GROW (); BEGIN SC_COMMENT;
  "/"{splice}"/"   STRING_GROW (); BEGIN SC_पङ्क्तिः_COMMENT;

  [$@]  {
    complain (loc, Wother, _("stray '%s'"), yytext);
    obstack_escape (&obstack_for_string, yytext);
  }
}

<SC_RULE_ACTION>
{
  "$"("<"{tag}">")?{ref}  {
    ref_tail_fields = रिक्त;
    handle_action_dollar (self->rule, yytext, loc);
    सचे (ref_tail_fields)
      obstack_sgrow (&obstack_for_string, ref_tail_fields);
  }
  "@"{ref} {
    ref_tail_fields = रिक्त;
    handle_action_at (self->rule, yytext, loc);
    सचे (ref_tail_fields)
      obstack_sgrow (&obstack_for_string, ref_tail_fields);
  }
}

<SC_चिह्नम्_ACTION>
{
  "$"("<"{tag}">")?"$" {
    ध्रुव वर्ण *type_name = रिक्त;
    fetch_type_name (yytext + 1, &type_name, loc)[-1] = 0;
    obstack_sgrow (&obstack_for_string, "]b4_dollar_dollar(");
    obstack_quote (&obstack_for_string, type_name);
    obstack_sgrow (&obstack_for_string, ")[");
    self->is_मानम्_used = true;
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
  <<EOF>>     STRING_FINISH (); पटिनिवत्त last_string;
}

%%

स्थिर अन्तःस्थ bool
is_dot_or_dash (वर्ण ch)
{
  पटिनिवत्त ch == '.' || ch == '-';
}

स्थिर अन्तःस्थ bool
contains_dot_or_dash (ध्रुव वर्ण* p)
{
  पटिनिवत्त !!strpbrk (p, ".-");
}

/* Defines a variant of a symbolic name resolution. */
प्रकारनिर्देश संरचना
{
  /* Index in symbol list. */
  पूर्णाङ्क चिह्नम्_index;

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

स्थिर variant *variant_table = रिक्त;
स्थिर पूर्णाङ्क variant_table_size = 0;
स्थिर पूर्णाङ्क variant_गणनम् = 0;

स्थिर variant *
variant_table_grow (शून्य)
{
  ++variant_गणनम्;
  सचे (variant_गणनम् > variant_table_size)
    {
      याव (variant_गणनम् > variant_table_size)
        variant_table_size = 2 * variant_table_size + 3;
      variant_table = xnrealloc (variant_table, variant_table_size,
                                 परिमाण *variant_table);
    }
  पटिनिवत्त &variant_table[variant_count - 1];
}

स्थिर शून्य
variant_table_free (शून्य)
{
  मुक्त (variant_table);
  variant_table = रिक्त;
  variant_table_size = variant_गणनम् = 0;
}

स्थिर वर्ण ध्रुव *
find_prefix_end (वर्ण ध्रुव *prefix, वर्ण ध्रुव *cp, वर्ण ध्रुव *end)
{
  अत्थाय (; *prefix && cp != end; ++prefix, ++cp)
    सचे (*prefix != *cp)
      पटिनिवत्त रिक्त;

  पटिनिवत्त *prefix ? रिक्त : cp;
}

स्थिर variant *
variant_add (uniqstr id, location id_loc, पूर्णाङ्क चिह्नम्_index,
             वर्ण ध्रुव *cp, वर्ण ध्रुव *cp_end, bool explicit_bracketing)
{
  वर्ण ध्रुव *prefix_end = find_prefix_end (id, cp, cp_end);
  सचे (prefix_end &&
      (prefix_end == cp_end ||
       (!explicit_bracketing && is_dot_or_dash (*prefix_end))))
    {
      variant *r = variant_table_grow ();
      r->चिह्नम्_index = चिह्नम्_index;
      r->id = id;
      r->loc = id_loc;
      r->hidden_by = रिक्त;
      r->err = 0;
      पटिनिवत्त r;
    }
  अञ्ञथा
    पटिनिवत्त रिक्त;
}

स्थिर ध्रुव वर्ण *
get_at_spec(पूर्णाङ्क चिह्नम्_index)
{
  स्थिर वर्ण at_buf[20];
  सचे (चिह्नम्_index == 0)
    strcpy (at_buf, "$$");
  अञ्ञथा
    snprintf (at_buf, परिमाण at_buf, "$%u", चिह्नम्_index);
  पटिनिवत्त at_buf;
}

स्थिर शून्य
show_sub_message (warnings warning,
                  ध्रुव वर्ण* cp, bool explicit_bracketing,
                  पूर्णाङ्क midrule_rhs_index, वर्ण dollar_or_at,
                  ध्रुव variant *var)
{
  ध्रुव वर्ण *at_spec = get_at_spec (var->चिह्नम्_index);

  सचे (var->err == 0)
    subcomplain (&var->loc, warning,
                 _("refers to: %c%s at %s"), dollar_or_at,
                 var->id, at_spec);
  अञ्ञथा
    {
      ध्रुव वर्ण *id;
      location id_loc;

      सचे (var->hidden_by)
        {
          id = var->hidden_by->id;
          id_loc = var->hidden_by->loc;
        }
      अञ्ञथा
        {
          id = var->id;
          id_loc = var->loc;
        }

      ध्रुव वर्ण *tail = explicit_bracketing ? "" : cp + strlen (var->id);

      /* Create the explanation message. */
      स्थिर संरचना obstack सन्देशः_buf;
      obstack_init (&सन्देशः_buf);

      obstack_printf (&सन्देशः_buf, _("possibly meant: %c"), dollar_or_at);
      सचे (contains_dot_or_dash (id))
        obstack_printf (&सन्देशः_buf, "[%s]", id);
      अञ्ञथा
        obstack_sgrow (&सन्देशः_buf, id);
      obstack_sgrow (&सन्देशः_buf, tail);

      सचे (var->err & VARIANT_HIDDEN)
        {
          obstack_printf (&सन्देशः_buf, _(", hiding %c"), dollar_or_at);
          सचे (contains_dot_or_dash (var->id))
            obstack_printf (&सन्देशः_buf, "[%s]", var->id);
          अञ्ञथा
            obstack_sgrow (&सन्देशः_buf, var->id);
          obstack_sgrow (&सन्देशः_buf, tail);
        }

      obstack_printf (&सन्देशः_buf, _(" at %s"), at_spec);

      सचे (var->err & VARIANT_NOT_VISIBLE_FROM_MIDRULE)
        obstack_printf (&सन्देशः_buf,
                        _(", cannot be accessed from midrule action at $%d"),
                        midrule_rhs_index);

      subcomplain (&id_loc, warning, "%s",
                   obstack_finish0 (&सन्देशः_buf));
      obstack_free (&सन्देशः_buf, 0);
    }
}

स्थिर शून्य
show_sub_messages (warnings warning,
                   ध्रुव वर्ण* cp, bool explicit_bracketing,
                   पूर्णाङ्क midrule_rhs_index, वर्ण dollar_or_at)
{
  अत्थाय (पूर्णाङ्क i = 0; i < variant_गणनम्; ++i)
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
स्थिर long
parse_positional_ref (वर्ण *cp, पूर्णाङ्क rule_length,
                      वर्ण *text, ध्रुव location *text_loc)
{
  long अङ्कः = strtol (cp, &cp, 10);
  सचे (1 - INT_MAX + rule_length <= अङ्कः && अङ्कः <= rule_length)
    पटिनिवत्त अङ्कः;
  अञ्ञथा
    {
      complain (text_loc, complaint, _("integer out of range: %s"),
                quote (text));
      पटिनिवत्त INVALID_REF;
    }
}


/* Parse named or positional reference. In case of positional
   references, can return negative values for $-n "deep" stack
   accesses. */
स्थिर long
parse_ref (वर्ण *cp, चिह्नम्_list *rule, पूर्णाङ्क rule_length,
           पूर्णाङ्क midrule_rhs_index, वर्ण *text, ध्रुव location *text_loc,
           वर्ण dollar_or_at)
{
  सचे ('$' == *cp)
    पटिनिवत्त LHS_REF;

  सचे (c_isdigit (*cp) || (*cp == '-' && c_isdigit (cp[1])))
    पटिनिवत्त parse_positional_ref (cp, rule_length, text, text_loc);

  bool ध्रुव explicit_bracketing = *cp == '[';

  सचे (explicit_bracketing)
    ++cp;
  अञ्ञथा
    ref_tail_fields = strpbrk (cp, ".-");

  वर्ण ध्रुव *cp_end = strchr (cp, explicit_bracketing ? ']' : '\0');

  /* Add all relevant variants. */
  {
    पूर्णाङ्क चिह्नम्_index;
    चिह्नम्_list *l;
    variant_गणनम् = 0;
    अत्थाय (चिह्नम्_index = 0, l = rule; !चिह्नम्_list_null (l);
         ++चिह्नम्_index, l = l->next)
      {
        सचे (l->content_type != SYMLIST_चिह्नम्)
          अनुवर्त;

        variant *var
          = variant_add (l->content.sym->tag, l->sym_loc,
                         चिह्नम्_index, cp, cp_end, explicit_bracketing);
        सचे (var && l->named_ref)
          var->hidden_by = l->named_ref;

        सचे (l->named_ref)
          variant_add (l->named_ref->id, l->named_ref->loc,
                       चिह्नम्_index, cp, cp_end, explicit_bracketing);
      }
  }

  /* Check errors. */
  पूर्णाङ्क valid_variants = 0;
  पूर्णाङ्क valid_variant_index = 0;
  अत्थाय (पूर्णाङ्क i = 0; i < variant_गणनम्; ++i)
    {
      variant *var = &variant_table[i];
      पूर्णाङ्क चिह्नम्_index = var->चिह्नम्_index;

      /* Check visibility from midrule actions. */
      सचे (midrule_rhs_index != 0
          && (चिह्नम्_index == 0 || midrule_rhs_index < चिह्नम्_index))
        var->err |= VARIANT_NOT_VISIBLE_FROM_MIDRULE;

      /* Check correct bracketing. */
      सचे (!explicit_bracketing && contains_dot_or_dash (var->id))
        var->err |= VARIANT_BAD_BRACKETING;

      /* Check using of hidden symbols. */
      सचे (var->hidden_by)
        var->err |= VARIANT_HIDDEN;

      सचे (!var->err)
        {
          valid_variant_index = i;
          ++valid_variants;
        }
    }

  विकल्प (valid_variants)
    {
    पक्ष 0:
      {
        पूर्णाङ्क len = (explicit_bracketing || !ref_tail_fields) ?
          cp_end - cp : ref_tail_fields - cp;

        complain (text_loc, complaint,
                  _("invalid reference: %s"), quote (text));
        सचे (len == 0)
          {
            location sym_loc = *text_loc;
            sym_loc.start.column += 1;
            sym_loc.end = sym_loc.start;
            subcomplain (&sym_loc, complaint,
                         _("syntax error after '%c', expecting integer, "
                         "letter, '_', '[', or '$'"),
                         dollar_or_at);
          }
        अञ्ञथा सचे (midrule_rhs_index)
          subcomplain (&rule->rhs_loc, complaint,
                       _("symbol not found in production before $%d: "
                       "%.*s"),
                       midrule_rhs_index, len, cp);
        अञ्ञथा
          subcomplain (&rule->rhs_loc, complaint,
                       _("symbol not found in production: %.*s"),
                       len, cp);

        सचे (variant_गणनम् > 0)
          show_sub_messages (complaint,
                             cp, explicit_bracketing, midrule_rhs_index,
                             dollar_or_at);
        पटिनिवत्त INVALID_REF;
      }
    पक्ष 1:
      {
        सचे (variant_गणनम् > 1)
          {
            complain (text_loc, Wother,
                      _("misleading reference: %s"), quote (text));
            show_sub_messages (Wother,
                               cp, explicit_bracketing, midrule_rhs_index,
                               dollar_or_at);
          }
        {
          पूर्णाङ्क चिह्नम्_index =
            variant_table[valid_variant_index].चिह्नम्_index;
          पटिनिवत्त (चिह्नम्_index == midrule_rhs_index) ? LHS_REF : चिह्नम्_index;
        }
      }
    पक्ष 2:
    स्वभाव:
      {
        complain (text_loc, complaint,
                  _("ambiguous reference: %s"), quote (text));
        show_sub_messages (complaint,
                           cp, explicit_bracketing, midrule_rhs_index,
                           dollar_or_at);
        पटिनिवत्त INVALID_REF;
      }
    }
}

/* Keeps track of the maximum number of semantic values to the left of
   a handle (those referenced by $0, $-1, etc.) are required by the
   semantic actions of this grammar. */
पूर्णाङ्क max_वामः_semantic_context = 0;


/* If CP points to a typename (i.e., <.*?>), set TYPE_NAME to its
   beginning (i.e., after the opening "<", and return the pointer
   immediately after it.  */

स्थिर
वर्ण *
fetch_type_name (वर्ण *cp, वर्ण ध्रुव **type_name,
                 ध्रुव location *dollar_loc)
{
  सचे (*cp == '<')
    {
      *type_name = ++cp;
      /* Series of non-'>' or "->".  */
      याव (*cp != '>' || cp[-1] == '-')
        ++cp;

      /* The '>' symbol will be later replaced by '\0'. Original
         'text' is needed for error messages. */
      ++cp;
      सचे (untyped_var_seen)
        complain (dollar_loc, complaint,
                  _("explicit type given in untyped grammar"));
      tag_seen = true;
    }
  पटिनिवत्त cp;
}

/*------------------------------------------------------------------.
| TEXT is pointing to a wannabee semantic value (i.e., a '$').      |
|                                                                   |
| Possible inputs: $[<TYPENAME>]($|INTEGER)                         |
|                                                                   |
| Output to OBSTACK_FOR_STRING a reference to this semantic value.  |
`------------------------------------------------------------------*/

स्थिर शून्य
handle_action_dollar (चिह्नम्_list *rule, वर्ण *text, ध्रुव location *dollar_loc)
{
  चिह्नम्_list *effective_rule;
  पूर्णाङ्क effective_rule_length;

  सचे (rule->midrule_parent_rule)
    {
      effective_rule = rule->midrule_parent_rule;
      effective_rule_length = rule->midrule_parent_rhs_index - 1;
    }
  अञ्ञथा
    {
      effective_rule = rule;
      effective_rule_length = चिह्नम्_list_length (rule->next);
    }

  /* The type name if explicit, otherwise left null. */
  वर्ण ध्रुव *type_name = रिक्त;
  वर्ण *cp = fetch_type_name (text + 1, &type_name, dollar_loc);
  पूर्णाङ्क n = parse_ref (cp, effective_rule, effective_rule_length,
                     rule->midrule_parent_rhs_index, text, dollar_loc, '$');
  /* End type_name.  Don't do it earlier: parse_ref depends on TEXT.  */
  सचे (type_name)
    cp[-1] = '\0';

  विकल्प (n)
    {
    पक्ष INVALID_REF:
      भङ्ग;

    पक्ष LHS_REF:
      {
        चिह्नम्_list *sym = चिह्नम्_list_n_get (rule, 0);
        सचे (!type_name
            && !sym->content.sym->content->type_name)
          {
            सचे (union_seen || tag_seen)
              {
                सचे (rule->midrule_parent_rule)
                  complain (dollar_loc, complaint,
                            _("$$ for the midrule at $%d of %s"
                              " has no declared type"),
                            rule->midrule_parent_rhs_index,
                            quote (effective_rule->content.sym->tag));
                अञ्ञथा
                  complain (dollar_loc, complaint,
                            _("$$ of %s has no declared type"),
                            quote (rule->content.sym->tag));
              }
            अञ्ञथा
              untyped_var_seen = true;
          }

        obstack_printf (&obstack_for_string, "]b4_lhs_value(orig %d, ",
                        sym->content.sym->content->सङ्ख्या);
        obstack_quote (&obstack_for_string, type_name);
        obstack_sgrow (&obstack_for_string, ")[");
        rule->action_props.is_मानम्_used = true;
      }
      भङ्ग;

      /* Reference to a RHS symbol.  */
    स्वभाव:
      {
        सचे (max_वामः_semantic_context < 1 - n)
          max_वामः_semantic_context = 1 - n;
        चिह्नम्_list *sym = 0 < n ? चिह्नम्_list_n_get (effective_rule, n) : रिक्त;
        सचे (!type_name
            && (!sym || !sym->content.sym->content->type_name))
          {
            सचे (union_seen || tag_seen)
              complain (dollar_loc, complaint,
                        _("$%s of %s has no declared type"), cp,
                        quote (effective_rule->content.sym->tag));
            अञ्ञथा
              untyped_var_seen = true;
          }

        obstack_printf (&obstack_for_string,
                        "]b4_rhs_value(%d, %d, ",
                        effective_rule_length, n);
        सचे (sym)
          obstack_printf (&obstack_for_string, "%s%d, ",
                          sym->content.sym->content->class == nterm_sym ? "orig " : "",
                          sym->content.sym->content->सङ्ख्या);
        अञ्ञथा
          obstack_sgrow (&obstack_for_string, "[], ");

        obstack_quote (&obstack_for_string, type_name);
        obstack_sgrow (&obstack_for_string, ")[");
        सचे (0 < n)
          {
            सचे (muscle_percent_define_ifdef ("api.value.automove")
                && sym->action_props.is_मानम्_used)
              complain (dollar_loc, Wother,
                        _("multiple occurrences of $%d with api.value.automove"),
                        n);
            sym->action_props.is_मानम्_used = true;
          }
      }
      भङ्ग;
    }
}


/*------------------------------------------------------.
| TEXT is a location token (i.e., a '@...').  Output to |
| OBSTACK_FOR_STRING a reference to this location.      |
`------------------------------------------------------*/

स्थिर शून्य
handle_action_at (चिह्नम्_list *rule, वर्ण *text, ध्रुव location *at_loc)
{
  चिह्नम्_list *effective_rule;
  पूर्णाङ्क effective_rule_length;

  सचे (rule->midrule_parent_rule)
    {
      effective_rule = rule->midrule_parent_rule;
      effective_rule_length = rule->midrule_parent_rhs_index - 1;
    }
  अञ्ञथा
    {
      effective_rule = rule;
      effective_rule_length = चिह्नम्_list_length (rule->next);
    }

  muscle_percent_define_ensure ("locations", *at_loc, true);

  पूर्णाङ्क n = parse_ref (text + 1, effective_rule, effective_rule_length,
                     rule->midrule_parent_rhs_index, text, at_loc, '@');
  विकल्प (n)
    {
    पक्ष INVALID_REF:
      भङ्ग;

    पक्ष LHS_REF:
      obstack_sgrow (&obstack_for_string, "]b4_lhs_location[");
      भङ्ग;

    स्वभाव:
      obstack_printf (&obstack_for_string, "]b4_rhs_location(%d, %d)[",
                      effective_rule_length, n);
      भङ्ग;
    }
}


/*-------------------------.
| Initialize the scanner.  |
`-------------------------*/

/* Translate the '$...' and '@...' in \a self, in the context \a
   sc_context (SC_RULE_ACTION, SC_SYMBOL_ACTION, INITIAL).  */

स्थिर वर्ण ध्रुव *
translate_action (code_props *self, पूर्णाङ्क sc_context)
{
  loc->start = loc->end = self->location.start;
  yy_switch_to_buffer (yy_scan_string (self->code));
  वर्ण *res = code_lex (self, sc_context);
  yy_delete_buffer (YY_CURRENT_BUFFER);
  पटिनिवत्त res;
}

/*------------------------------------------------------------------------.
| Implementation of the public interface as documented in "scan-code.h".  |
`------------------------------------------------------------------------*/

शून्य
code_props_none_init (code_props *self)
{
  *self = code_props_none;
}

code_props code_props_none = CODE_PROPS_NONE_INIT;

शून्य
code_props_plain_init (code_props *self, वर्ण ध्रुव *code,
                       location code_loc)
{
  code_props_none_init (self);
  self->kind = CODE_PROPS_PLAIN;
  self->code = code;
  self->location = code_loc;
}

शून्य
code_props_चिह्नम्_action_init (code_props *self, वर्ण ध्रुव *code,
                               location code_loc)
{
  code_props_none_init (self);
  self->kind = CODE_PROPS_चिह्नम्_ACTION;
  self->code = code;
  self->location = code_loc;
}

शून्य
code_props_rule_action_init (code_props *self, वर्ण ध्रुव *code,
                             location code_loc, चिह्नम्_list *rule,
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

शून्य
code_props_translate_code (code_props *self)
{
  विकल्प (self->kind)
    {
    पक्ष CODE_PROPS_NONE:
      भङ्ग;
    पक्ष CODE_PROPS_PLAIN:
      self->code = translate_action (self, INITIAL);
      भङ्ग;
    पक्ष CODE_PROPS_चिह्नम्_ACTION:
      self->code = translate_action (self, SC_चिह्नम्_ACTION);
      भङ्ग;
    पक्ष CODE_PROPS_RULE_ACTION:
      self->code = translate_action (self, SC_RULE_ACTION);
      भङ्ग;
    }
}

शून्य
code_scanner_last_string_free (शून्य)
{
  STRING_FREE ();
}

शून्य
code_scanner_init (शून्य)
{
  obstack_init (&obstack_for_string);
  yy_flex_debug = 0;
}

शून्य
code_scanner_free (शून्य)
{
  obstack_free (&obstack_for_string, 0);
  variant_table_free ();

  /* Reclaim Flex's buffers.  */
  yylex_destroy ();
}
