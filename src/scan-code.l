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
#समावेश <c-ctype.h>
#समावेश <get-errno.h>
#समावेश <quote.h>

#समावेश "src/complain.h"
#समावेश "src/getargs.h"
#समावेश "src/muscle-tab.h"
#समावेश "src/reader.h"
#समावेश "src/scan-code.h"
#समावेश "src/symlist.h"

#घोषणा FLEX_PREFIX(Id) code_ ## Id
#समावेश "src/flex-scanner.h"

/* Work around a bug in flex 2.5.31.  See Debian bug 333231
   <https://bugs.debian.org/333231>.  */
#अघोषित code_wrap
#घोषणा code_wrap() 1

काष्ठा obstack *obstack_for_actions = &obstack_for_string;

/* The current calling start condition: SC_RULE_ACTION or
   SC_SYMBOL_ACTION. */
# define YY_DECL अटल अक्षर *code_lex (code_props *self, पूर्णांक sc_context)
YY_DECL;

#घोषणा YY_USER_ACTION  location_compute (loc, &loc->end, श_माला, yyleng);

अटल अक्षर *fetch_type_name (अक्षर *cp, अक्षर स्थिर **type_name,
                              स्थिर location *dollar_loc);

अटल व्योम handle_action_dollar (प्रतीक_list *rule, अक्षर *cp,
                                  स्थिर location *dollar_loc);
अटल व्योम handle_action_at (प्रतीक_list *rule, अक्षर *cp,
                              स्थिर location *at_loc);

/* A string to be pushed to obstack after dollar/at has been handled. */
अटल अक्षर *ref_पुच्छ_fields;

अटल location current_loc;
अटल location *loc = &current_loc;

/* A string representing the most recent translation.  */
अटल अक्षर *last_string;

/* True if an untyped $$ or $n was seen.  */
अटल bool untyped_var_seen;

%}
 /* C and C++ comments in code. */
%x SC_COMMENT SC_पंक्ति_COMMENT
 /* Strings and characters in code. */
%x SC_STRING SC_CHARACTER
 /* Whether in a rule or symbol action.  Specifies the translation
    of $ and @.  */
%x SC_RULE_ACTION SC_प्रतीक_ACTION


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
  aver (sc_context == SC_प्रतीक_ACTION
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

<SC_पंक्ति_COMMENT>
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


<SC_RULE_ACTION,SC_प्रतीक_ACTION>
{
  "'"              STRING_GROW (); BEGIN SC_CHARACTER;
  "\""             STRING_GROW (); BEGIN SC_STRING;
  "/"{splice}"*"   STRING_GROW (); BEGIN SC_COMMENT;
  "/"{splice}"/"   STRING_GROW (); BEGIN SC_पंक्ति_COMMENT;

  [$@]  {
    complain (loc, Wother, _("stray '%s'"), श_माला);
    obstack_escape (&obstack_for_string, श_माला);
  }
}

<SC_RULE_ACTION>
{
  "$"("<"{tag}">")?{ref}  {
    ref_पुच्छ_fields = शून्य;
    handle_action_dollar (self->rule, श_माला, loc);
    अगर (ref_पुच्छ_fields)
      obstack_sgrow (&obstack_for_string, ref_पुच्छ_fields);
  }
  "@"{ref} {
    ref_पुच्छ_fields = शून्य;
    handle_action_at (self->rule, श_माला, loc);
    अगर (ref_पुच्छ_fields)
      obstack_sgrow (&obstack_for_string, ref_पुच्छ_fields);
  }
}

<SC_प्रतीक_ACTION>
{
  "$"("<"{tag}">")?"$" {
    स्थिर अक्षर *type_name = शून्य;
    fetch_type_name (श_माला + 1, &type_name, loc)[-1] = 0;
    obstack_sgrow (&obstack_for_string, "]b4_dollar_dollar(");
    obstack_quote (&obstack_for_string, type_name);
    obstack_sgrow (&obstack_for_string, ")[");
    self->is_मूल्य_used = true;
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
  [$@\[\]]    obstack_escape (&obstack_for_string, श_माला);

  /* By default, grow the string obstack with the input.  */
  .|\n        STRING_GROW ();

  /* End of processing. */
  <<खातापूर्ण>>     STRING_FINISH (); वापस last_string;
}

%%

अटल अंतरभूत bool
is_dot_or_dash (अक्षर ch)
{
  वापस ch == '.' || ch == '-';
}

अटल अंतरभूत bool
contains_dot_or_dash (स्थिर अक्षर* p)
{
  वापस !!strpbrk (p, ".-");
}

/* Defines a variant of a symbolic name resolution. */
प्रकार काष्ठा
{
  /* Index in symbol list. */
  पूर्णांक प्रतीक_सूचकांक;

  /* Matched symbol id and loc. */
  uniqstr id;
  location loc;

  /* Hiding named reference. */
  named_ref* hidden_by;

  /* Error flags. May contain zero (no errors) or
     a combination of VARIANT_* values. */
  अचिन्हित err;
} variant;

/* Set when the variant refers to a symbol hidden
   by an explicit symbol reference. */
#घोषणा VARIANT_HIDDEN (1 << 0)

/* Set when the variant refers to a symbol containing
   dots or dashes. Will require explicit bracketing. */
#घोषणा VARIANT_BAD_BRACKETING (1 << 1)

/* Set when the variant refers to a symbol which is
   not visible from current midrule. */
#घोषणा VARIANT_NOT_VISIBLE_FROM_MIDRULE (1 << 2)

अटल variant *variant_table = शून्य;
अटल पूर्णांक variant_table_आकार = 0;
अटल पूर्णांक variant_गिनती = 0;

अटल variant *
variant_table_grow (व्योम)
{
  ++variant_गिनती;
  अगर (variant_गिनती > variant_table_आकार)
    {
      जबतक (variant_गिनती > variant_table_आकार)
        variant_table_आकार = 2 * variant_table_आकार + 3;
      variant_table = xnrealloc (variant_table, variant_table_आकार,
                                 माप *variant_table);
    }
  वापस &variant_table[variant_count - 1];
}

अटल व्योम
variant_table_free (व्योम)
{
  मुक्त (variant_table);
  variant_table = शून्य;
  variant_table_आकार = variant_गिनती = 0;
}

अटल अक्षर स्थिर *
find_prefix_end (अक्षर स्थिर *prefix, अक्षर स्थिर *cp, अक्षर स्थिर *end)
{
  क्रम (; *prefix && cp != end; ++prefix, ++cp)
    अगर (*prefix != *cp)
      वापस शून्य;

  वापस *prefix ? शून्य : cp;
}

अटल variant *
variant_add (uniqstr id, location id_loc, पूर्णांक प्रतीक_सूचकांक,
             अक्षर स्थिर *cp, अक्षर स्थिर *cp_end, bool explicit_bracketing)
{
  अक्षर स्थिर *prefix_end = find_prefix_end (id, cp, cp_end);
  अगर (prefix_end &&
      (prefix_end == cp_end ||
       (!explicit_bracketing && is_dot_or_dash (*prefix_end))))
    {
      variant *r = variant_table_grow ();
      r->प्रतीक_सूचकांक = प्रतीक_सूचकांक;
      r->id = id;
      r->loc = id_loc;
      r->hidden_by = शून्य;
      r->err = 0;
      वापस r;
    }
  अन्यथा
    वापस शून्य;
}

अटल स्थिर अक्षर *
get_at_spec(पूर्णांक प्रतीक_सूचकांक)
{
  अटल अक्षर at_buf[20];
  अगर (प्रतीक_सूचकांक == 0)
    म_नकल (at_buf, "$$");
  अन्यथा
    snprintf (at_buf, माप at_buf, "$%u", प्रतीक_सूचकांक);
  वापस at_buf;
}

अटल व्योम
show_sub_message (warnings warning,
                  स्थिर अक्षर* cp, bool explicit_bracketing,
                  पूर्णांक midrule_rhs_सूचकांक, अक्षर dollar_or_at,
                  स्थिर variant *var)
{
  स्थिर अक्षर *at_spec = get_at_spec (var->प्रतीक_सूचकांक);

  अगर (var->err == 0)
    subcomplain (&var->loc, warning,
                 _("refers to: %c%s at %s"), dollar_or_at,
                 var->id, at_spec);
  अन्यथा
    {
      स्थिर अक्षर *id;
      location id_loc;

      अगर (var->hidden_by)
        {
          id = var->hidden_by->id;
          id_loc = var->hidden_by->loc;
        }
      अन्यथा
        {
          id = var->id;
          id_loc = var->loc;
        }

      स्थिर अक्षर *पुच्छ = explicit_bracketing ? "" : cp + म_माप (var->id);

      /* Create the explanation message. */
      अटल काष्ठा obstack संदेश_buf;
      obstack_init (&संदेश_buf);

      obstack_printf (&संदेश_buf, _("possibly meant: %c"), dollar_or_at);
      अगर (contains_dot_or_dash (id))
        obstack_printf (&संदेश_buf, "[%s]", id);
      अन्यथा
        obstack_sgrow (&संदेश_buf, id);
      obstack_sgrow (&संदेश_buf, पुच्छ);

      अगर (var->err & VARIANT_HIDDEN)
        {
          obstack_printf (&संदेश_buf, _(", hiding %c"), dollar_or_at);
          अगर (contains_dot_or_dash (var->id))
            obstack_printf (&संदेश_buf, "[%s]", var->id);
          अन्यथा
            obstack_sgrow (&संदेश_buf, var->id);
          obstack_sgrow (&संदेश_buf, पुच्छ);
        }

      obstack_printf (&संदेश_buf, _(" at %s"), at_spec);

      अगर (var->err & VARIANT_NOT_VISIBLE_FROM_MIDRULE)
        obstack_printf (&संदेश_buf,
                        _(", cannot be accessed from midrule action at $%d"),
                        midrule_rhs_सूचकांक);

      subcomplain (&id_loc, warning, "%s",
                   obstack_finish0 (&संदेश_buf));
      obstack_free (&संदेश_buf, 0);
    }
}

अटल व्योम
show_sub_messages (warnings warning,
                   स्थिर अक्षर* cp, bool explicit_bracketing,
                   पूर्णांक midrule_rhs_सूचकांक, अक्षर dollar_or_at)
{
  क्रम (पूर्णांक i = 0; i < variant_गिनती; ++i)
    show_sub_message (warning | silent,
                      cp, explicit_bracketing,
                      midrule_rhs_सूचकांक, dollar_or_at,
                      &variant_table[i]);
}

/* Returned from "parse_ref" when the reference
   is inappropriate. */
#घोषणा INVALID_REF (पूर्णांक_न्यून)

/* Returned from "parse_ref" when the reference
   points to LHS ($$) of the current rule or midrule. */
#घोषणा LHS_REF (पूर्णांक_न्यून + 1)

/* Parse a positional reference in RULE.  */
अटल दीर्घ
parse_positional_ref (अक्षर *cp, पूर्णांक rule_लंबाई,
                      अक्षर *text, स्थिर location *text_loc)
{
  दीर्घ अंक = म_से_दीर्घ (cp, &cp, 10);
  अगर (1 - पूर्णांक_उच्च + rule_लंबाई <= अंक && अंक <= rule_लंबाई)
    वापस अंक;
  अन्यथा
    {
      complain (text_loc, complaint, _("integer out of range: %s"),
                quote (text));
      वापस INVALID_REF;
    }
}


/* Parse named or positional reference. In case of positional
   references, can return negative values for $-n "deep" stack
   accesses. */
अटल दीर्घ
parse_ref (अक्षर *cp, प्रतीक_list *rule, पूर्णांक rule_लंबाई,
           पूर्णांक midrule_rhs_सूचकांक, अक्षर *text, स्थिर location *text_loc,
           अक्षर dollar_or_at)
{
  अगर ('$' == *cp)
    वापस LHS_REF;

  अगर (c_isdigit (*cp) || (*cp == '-' && c_isdigit (cp[1])))
    वापस parse_positional_ref (cp, rule_लंबाई, text, text_loc);

  bool स्थिर explicit_bracketing = *cp == '[';

  अगर (explicit_bracketing)
    ++cp;
  अन्यथा
    ref_पुच्छ_fields = strpbrk (cp, ".-");

  अक्षर स्थिर *cp_end = म_अक्षर (cp, explicit_bracketing ? ']' : '\0');

  /* Add all relevant variants. */
  {
    पूर्णांक प्रतीक_सूचकांक;
    प्रतीक_list *l;
    variant_गिनती = 0;
    क्रम (प्रतीक_सूचकांक = 0, l = rule; !प्रतीक_list_null (l);
         ++प्रतीक_सूचकांक, l = l->अग्र)
      {
        अगर (l->content_type != SYMLIST_प्रतीक)
          जारी;

        variant *var
          = variant_add (l->content.sym->tag, l->sym_loc,
                         प्रतीक_सूचकांक, cp, cp_end, explicit_bracketing);
        अगर (var && l->named_ref)
          var->hidden_by = l->named_ref;

        अगर (l->named_ref)
          variant_add (l->named_ref->id, l->named_ref->loc,
                       प्रतीक_सूचकांक, cp, cp_end, explicit_bracketing);
      }
  }

  /* Check errors. */
  पूर्णांक valid_variants = 0;
  पूर्णांक valid_variant_सूचकांक = 0;
  क्रम (पूर्णांक i = 0; i < variant_गिनती; ++i)
    {
      variant *var = &variant_table[i];
      पूर्णांक प्रतीक_सूचकांक = var->प्रतीक_सूचकांक;

      /* Check visibility from midrule actions. */
      अगर (midrule_rhs_सूचकांक != 0
          && (प्रतीक_सूचकांक == 0 || midrule_rhs_सूचकांक < प्रतीक_सूचकांक))
        var->err |= VARIANT_NOT_VISIBLE_FROM_MIDRULE;

      /* Check correct bracketing. */
      अगर (!explicit_bracketing && contains_dot_or_dash (var->id))
        var->err |= VARIANT_BAD_BRACKETING;

      /* Check using of hidden symbols. */
      अगर (var->hidden_by)
        var->err |= VARIANT_HIDDEN;

      अगर (!var->err)
        {
          valid_variant_सूचकांक = i;
          ++valid_variants;
        }
    }

  चयन (valid_variants)
    {
    हाल 0:
      {
        पूर्णांक len = (explicit_bracketing || !ref_पुच्छ_fields) ?
          cp_end - cp : ref_पुच्छ_fields - cp;

        complain (text_loc, complaint,
                  _("invalid reference: %s"), quote (text));
        अगर (len == 0)
          {
            location sym_loc = *text_loc;
            sym_loc.start.column += 1;
            sym_loc.end = sym_loc.start;
            subcomplain (&sym_loc, complaint,
                         _("syntax error after '%c', expecting integer, "
                         "letter, '_', '[', or '$'"),
                         dollar_or_at);
          }
        अन्यथा अगर (midrule_rhs_सूचकांक)
          subcomplain (&rule->rhs_loc, complaint,
                       _("symbol not found in production before $%d: "
                       "%.*s"),
                       midrule_rhs_सूचकांक, len, cp);
        अन्यथा
          subcomplain (&rule->rhs_loc, complaint,
                       _("symbol not found in production: %.*s"),
                       len, cp);

        अगर (variant_गिनती > 0)
          show_sub_messages (complaint,
                             cp, explicit_bracketing, midrule_rhs_सूचकांक,
                             dollar_or_at);
        वापस INVALID_REF;
      }
    हाल 1:
      {
        अगर (variant_गिनती > 1)
          {
            complain (text_loc, Wother,
                      _("misleading reference: %s"), quote (text));
            show_sub_messages (Wother,
                               cp, explicit_bracketing, midrule_rhs_सूचकांक,
                               dollar_or_at);
          }
        {
          पूर्णांक प्रतीक_सूचकांक =
            variant_table[valid_variant_index].प्रतीक_सूचकांक;
          वापस (प्रतीक_सूचकांक == midrule_rhs_सूचकांक) ? LHS_REF : प्रतीक_सूचकांक;
        }
      }
    हाल 2:
    शेष:
      {
        complain (text_loc, complaint,
                  _("ambiguous reference: %s"), quote (text));
        show_sub_messages (complaint,
                           cp, explicit_bracketing, midrule_rhs_सूचकांक,
                           dollar_or_at);
        वापस INVALID_REF;
      }
    }
}

/* Keeps track of the maximum number of semantic values to the left of
   a handle (those referenced by $0, $-1, etc.) are required by the
   semantic actions of this grammar. */
पूर्णांक max_बायाँ_semantic_context = 0;


/* If CP points to a typename (i.e., <.*?>), set TYPE_NAME to its
   beginning (i.e., after the opening "<", and return the pointer
   immediately after it.  */

अटल
अक्षर *
fetch_type_name (अक्षर *cp, अक्षर स्थिर **type_name,
                 स्थिर location *dollar_loc)
{
  अगर (*cp == '<')
    {
      *type_name = ++cp;
      /* Series of non-'>' or "->".  */
      जबतक (*cp != '>' || cp[-1] == '-')
        ++cp;

      /* The '>' symbol will be later replaced by '\0'. Original
         'text' is needed for error messages. */
      ++cp;
      अगर (untyped_var_seen)
        complain (dollar_loc, complaint,
                  _("explicit type given in untyped grammar"));
      tag_seen = true;
    }
  वापस cp;
}

/*------------------------------------------------------------------.
| TEXT is pointing to a wannabee semantic value (i.e., a '$').      |
|                                                                   |
| Possible inputs: $[<TYPENAME>]($|INTEGER)                         |
|                                                                   |
| Output to OBSTACK_FOR_STRING a reference to this semantic value.  |
`------------------------------------------------------------------*/

अटल व्योम
handle_action_dollar (प्रतीक_list *rule, अक्षर *text, स्थिर location *dollar_loc)
{
  प्रतीक_list *effective_rule;
  पूर्णांक effective_rule_लंबाई;

  अगर (rule->midrule_parent_rule)
    {
      effective_rule = rule->midrule_parent_rule;
      effective_rule_लंबाई = rule->midrule_parent_rhs_सूचकांक - 1;
    }
  अन्यथा
    {
      effective_rule = rule;
      effective_rule_लंबाई = प्रतीक_list_लंबाई (rule->अग्र);
    }

  /* The type name if explicit, otherwise left null. */
  अक्षर स्थिर *type_name = शून्य;
  अक्षर *cp = fetch_type_name (text + 1, &type_name, dollar_loc);
  पूर्णांक n = parse_ref (cp, effective_rule, effective_rule_लंबाई,
                     rule->midrule_parent_rhs_सूचकांक, text, dollar_loc, '$');
  /* End type_name.  Don't do it earlier: parse_ref depends on TEXT.  */
  अगर (type_name)
    cp[-1] = '\0';

  चयन (n)
    {
    हाल INVALID_REF:
      अवरोध;

    हाल LHS_REF:
      {
        प्रतीक_list *sym = प्रतीक_list_n_get (rule, 0);
        अगर (!type_name
            && !sym->content.sym->content->type_name)
          {
            अगर (union_seen || tag_seen)
              {
                अगर (rule->midrule_parent_rule)
                  complain (dollar_loc, complaint,
                            _("$$ for the midrule at $%d of %s"
                              " has no declared type"),
                            rule->midrule_parent_rhs_सूचकांक,
                            quote (effective_rule->content.sym->tag));
                अन्यथा
                  complain (dollar_loc, complaint,
                            _("$$ of %s has no declared type"),
                            quote (rule->content.sym->tag));
              }
            अन्यथा
              untyped_var_seen = true;
          }

        obstack_printf (&obstack_for_string, "]b4_lhs_value(orig %d, ",
                        sym->content.sym->content->संख्या);
        obstack_quote (&obstack_for_string, type_name);
        obstack_sgrow (&obstack_for_string, ")[");
        rule->action_props.is_मूल्य_used = true;
      }
      अवरोध;

      /* Reference to a RHS symbol.  */
    शेष:
      {
        अगर (max_बायाँ_semantic_context < 1 - n)
          max_बायाँ_semantic_context = 1 - n;
        प्रतीक_list *sym = 0 < n ? प्रतीक_list_n_get (effective_rule, n) : शून्य;
        अगर (!type_name
            && (!sym || !sym->content.sym->content->type_name))
          {
            अगर (union_seen || tag_seen)
              complain (dollar_loc, complaint,
                        _("$%s of %s has no declared type"), cp,
                        quote (effective_rule->content.sym->tag));
            अन्यथा
              untyped_var_seen = true;
          }

        obstack_printf (&obstack_for_string,
                        "]b4_rhs_value(%d, %d, ",
                        effective_rule_लंबाई, n);
        अगर (sym)
          obstack_printf (&obstack_for_string, "%s%d, ",
                          sym->content.sym->content->class == nterm_sym ? "orig " : "",
                          sym->content.sym->content->संख्या);
        अन्यथा
          obstack_sgrow (&obstack_for_string, "[], ");

        obstack_quote (&obstack_for_string, type_name);
        obstack_sgrow (&obstack_for_string, ")[");
        अगर (0 < n)
          {
            अगर (muscle_percent_define_ifdef ("api.value.automove")
                && sym->action_props.is_मूल्य_used)
              complain (dollar_loc, Wother,
                        _("multiple occurrences of $%d with api.value.automove"),
                        n);
            sym->action_props.is_मूल्य_used = true;
          }
      }
      अवरोध;
    }
}


/*------------------------------------------------------.
| TEXT is a location token (i.e., a '@...').  Output to |
| OBSTACK_FOR_STRING a reference to this location.      |
`------------------------------------------------------*/

अटल व्योम
handle_action_at (प्रतीक_list *rule, अक्षर *text, स्थिर location *at_loc)
{
  प्रतीक_list *effective_rule;
  पूर्णांक effective_rule_लंबाई;

  अगर (rule->midrule_parent_rule)
    {
      effective_rule = rule->midrule_parent_rule;
      effective_rule_लंबाई = rule->midrule_parent_rhs_सूचकांक - 1;
    }
  अन्यथा
    {
      effective_rule = rule;
      effective_rule_लंबाई = प्रतीक_list_लंबाई (rule->अग्र);
    }

  muscle_percent_define_ensure ("locations", *at_loc, true);

  पूर्णांक n = parse_ref (text + 1, effective_rule, effective_rule_लंबाई,
                     rule->midrule_parent_rhs_सूचकांक, text, at_loc, '@');
  चयन (n)
    {
    हाल INVALID_REF:
      अवरोध;

    हाल LHS_REF:
      obstack_sgrow (&obstack_for_string, "]b4_lhs_location[");
      अवरोध;

    शेष:
      obstack_printf (&obstack_for_string, "]b4_rhs_location(%d, %d)[",
                      effective_rule_लंबाई, n);
      अवरोध;
    }
}


/*-------------------------.
| Initialize the scanner.  |
`-------------------------*/

/* Translate the '$...' and '@...' in \a self, in the context \a
   sc_context (SC_RULE_ACTION, SC_SYMBOL_ACTION, INITIAL).  */

अटल अक्षर स्थिर *
translate_action (code_props *self, पूर्णांक sc_context)
{
  loc->start = loc->end = self->location.start;
  yy_switch_to_बफर (yy_scan_string (self->code));
  अक्षर *res = code_lex (self, sc_context);
  yy_delete_बफर (YY_CURRENT_बफर);
  वापस res;
}

/*------------------------------------------------------------------------.
| Implementation of the public interface as documented in "scan-code.h".  |
`------------------------------------------------------------------------*/

व्योम
code_props_none_init (code_props *self)
{
  *self = code_props_none;
}

code_props code_props_none = CODE_PROPS_NONE_INIT;

व्योम
code_props_plain_init (code_props *self, अक्षर स्थिर *code,
                       location code_loc)
{
  code_props_none_init (self);
  self->kind = CODE_PROPS_PLAIN;
  self->code = code;
  self->location = code_loc;
}

व्योम
code_props_प्रतीक_action_init (code_props *self, अक्षर स्थिर *code,
                               location code_loc)
{
  code_props_none_init (self);
  self->kind = CODE_PROPS_प्रतीक_ACTION;
  self->code = code;
  self->location = code_loc;
}

व्योम
code_props_rule_action_init (code_props *self, अक्षर स्थिर *code,
                             location code_loc, प्रतीक_list *rule,
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

व्योम
code_props_translate_code (code_props *self)
{
  चयन (self->kind)
    {
    हाल CODE_PROPS_NONE:
      अवरोध;
    हाल CODE_PROPS_PLAIN:
      self->code = translate_action (self, INITIAL);
      अवरोध;
    हाल CODE_PROPS_प्रतीक_ACTION:
      self->code = translate_action (self, SC_प्रतीक_ACTION);
      अवरोध;
    हाल CODE_PROPS_RULE_ACTION:
      self->code = translate_action (self, SC_RULE_ACTION);
      अवरोध;
    }
}

व्योम
code_scanner_last_string_free (व्योम)
{
  STRING_FREE ();
}

व्योम
code_scanner_init (व्योम)
{
  obstack_init (&obstack_for_string);
  yy_flex_debug = 0;
}

व्योम
code_scanner_free (व्योम)
{
  obstack_free (&obstack_for_string, 0);
  variant_table_free ();

  /* Reclaim Flex's buffers.  */
  yylex_destroy ();
}
