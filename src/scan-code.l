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

%विकल्पः debug nodefault noinput nounput noyywrap never-interactive
%विकल्पः उपसर्गः="code_" outfile="lex.yy.c"

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

#परिभाषय FLEX_उपसर्गः(Id) संहिता_2 ## Id
#अन्तर्भाव "src/flex-scanner.h"

/* Work around a bug in flex 2.5.31.  See Debian bug 333231
   <https://bugs.debian.org/333231>.  */
#अपरिभाषय संहिता_wrap
#परिभाषय संहिता_wrap() 1

संरचना obstack *obstack_for_actions = &obstack_for_string;

/* The current calling start condition: SC_RULE_ACTION or
   SC_SYMBOL_ACTION. */
# define YY_DECL स्थिर वर्ण *संहिता_lex (संहिता_props *self, पूर्णाङ्क sc_context)
YY_DECL;

#परिभाषय YY_USER_ACTION  location_compute (loc, &loc->अन्तः, yytext, yyleng);

स्थिर वर्ण *fetch_type_name (वर्ण *cp, वर्ण ध्रुव **type_name,
                              ध्रुव location *dollar_loc);

स्थिर शून्य handle_action_dollar (चिह्नम्_list *नियमः, वर्ण *cp,
                                  ध्रुव location *dollar_loc);
स्थिर शून्य handle_action_at (चिह्नम्_list *नियमः, वर्ण *cp,
                              ध्रुव location *at_loc);

/* A string to be pushed to obstack after dollar/at has been handled. */
स्थिर वर्ण *ref_पुच्छ_fields;

स्थिर location current_loc;
स्थिर location *loc = &current_loc;

/* A string representing the most recent translation.  */
स्थिर वर्ण *चरमः_string;

/* True if an untyped $$ or $n was seen.  */
स्थिर bool untyped_var_आलोकितम्;

%}
 /* C and C++ comments in code. */
%x SC_COMMENT SC_पङ्क्तिः_COMMENT
 /* Strings and characters in code. */
%x SC_STRING SC_CHARACTER
 /* Whether in a rule or symbol action.  Specifies the translation
    of $ and @.  */
%x SC_नियमः_ACTION SC_चिह्नम्_ACTION


/* POSIX says that a tag must be both an id and a C union member, but
   historically almost any character is allowed in a tag.  We disallow
   NUL and newline, as this simplifies our implementation.  We allow
   "->" as a means to dereference a pointer.  */
चिह्नकम्      ([^\0\n>]|->)*[^-]

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
        || sc_context == SC_नियमः_ACTION
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


<SC_नियमः_ACTION,SC_चिह्नम्_ACTION>
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

<SC_नियमः_ACTION>
{
  "$"("<"{चिह्नकम्}">")?{ref}  {
    ref_पुच्छ_fields = रिक्त;
    handle_action_dollar (self->नियमः, yytext, loc);
    यदि (ref_पुच्छ_fields)
      obstack_sgrow (&obstack_for_string, ref_पुच्छ_fields);
  }
  "@"{ref} {
    ref_पुच्छ_fields = रिक्त;
    handle_action_at (self->नियमः, yytext, loc);
    यदि (ref_पुच्छ_fields)
      obstack_sgrow (&obstack_for_string, ref_पुच्छ_fields);
  }
}

<SC_चिह्नम्_ACTION>
{
  "$"("<"{चिह्नकम्}">")?"$" {
    ध्रुव वर्ण *type_name = रिक्त;
    fetch_type_name (yytext + 1, &type_name, loc)[-1] = 0;
    obstack_sgrow (&obstack_for_string, "]b4_dollar_dollar(");
    obstack_quote (&obstack_for_string, type_name);
    obstack_sgrow (&obstack_for_string, ")[");
    self->is_मानम्_प्रयुक्तम् = true;
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
  <<EOF>>     STRING_FINISH (); प्रत्यागम चरमः_string;
}

%%

स्थिर अन्तःस्थ bool
is_dot_or_dash (वर्ण ch)
{
  प्रत्यागम ch == '.' || ch == '-';
}

स्थिर अन्तःस्थ bool
अन्तर्भवति_dot_or_dash (ध्रुव वर्ण* p)
{
  प्रत्यागम !!strpbrk (p, ".-");
}

/* Defines a variant of a symbolic name resolution. */
प्रकारनिर्देश संरचना
{
  /* Index in symbol list. */
  पूर्णाङ्क चिह्नम्_सूचकांक;

  /* Matched symbol id and loc. */
  uniqstr id;
  location loc;

  /* Hiding named reference. */
  named_ref* hidden_by;

  /* Error flags. May contain zero (no errors) or
     a combination of VARIANT_* values. */
  अचिह्नित err;
} variant;

/* Set when the variant refers to a symbol hidden
   by an explicit symbol reference. */
#परिभाषय VARIANT_HIDDEN (1 << 0)

/* Set when the variant refers to a symbol containing
   dots or dashes. Will require explicit bracketing. */
#परिभाषय VARIANT_BAD_BRACKETING (1 << 1)

/* Set when the variant refers to a symbol which is
   not visible from current midrule. */
#परिभाषय VARIANT_NOT_VISIBLE_FROM_MIDRULE (1 << 2)

स्थिर variant *variant_सारणी = रिक्त;
स्थिर पूर्णाङ्क variant_सारणी_आकार = 0;
स्थिर पूर्णाङ्क variant_गणनम् = 0;

स्थिर variant *
variant_सारणी_grow (शून्य)
{
  ++variant_गणनम्;
  यदि (variant_गणनम् > variant_सारणी_आकार)
    {
      यावत् (variant_गणनम् > variant_सारणी_आकार)
        variant_सारणी_आकार = 2 * variant_सारणी_आकार + 3;
      variant_सारणी = xnrealloc (variant_सारणी, variant_सारणी_आकार,
                                 परिमाण *variant_सारणी);
    }
  प्रत्यागम &variant_सारणी[variant_count - 1];
}

स्थिर शून्य
variant_सारणी_free (शून्य)
{
  मुक्त (variant_सारणी);
  variant_सारणी = रिक्त;
  variant_सारणी_आकार = variant_गणनम् = 0;
}

स्थिर वर्ण ध्रुव *
find_उपसर्गः_अन्तः (वर्ण ध्रुव *उपसर्गः, वर्ण ध्रुव *cp, वर्ण ध्रुव *अन्तः)
{
  प्रति (; *उपसर्गः && cp != अन्तः; ++उपसर्गः, ++cp)
    यदि (*उपसर्गः != *cp)
      प्रत्यागम रिक्त;

  प्रत्यागम *उपसर्गः ? रिक्त : cp;
}

स्थिर variant *
variant_सङ्कलय (uniqstr id, location id_loc, पूर्णाङ्क चिह्नम्_सूचकांक,
             वर्ण ध्रुव *cp, वर्ण ध्रुव *cp_अन्तः, bool explicit_bracketing)
{
  वर्ण ध्रुव *उपसर्गः_अन्तः = find_उपसर्गः_अन्तः (id, cp, cp_अन्तः);
  यदि (उपसर्गः_अन्तः &&
      (उपसर्गः_अन्तः == cp_अन्तः ||
       (!explicit_bracketing && is_dot_or_dash (*उपसर्गः_अन्तः))))
    {
      variant *r = variant_सारणी_grow ();
      r->चिह्नम्_सूचकांक = चिह्नम्_सूचकांक;
      r->id = id;
      r->loc = id_loc;
      r->hidden_by = रिक्त;
      r->err = 0;
      प्रत्यागम r;
    }
  अन्यथा
    प्रत्यागम रिक्त;
}

स्थिर ध्रुव वर्ण *
get_at_spec(पूर्णाङ्क चिह्नम्_सूचकांक)
{
  स्थिर वर्ण at_buf[20];
  यदि (चिह्नम्_सूचकांक == 0)
    वाक्यं_प्रतिलिख (at_buf, "$$");
  अन्यथा
    snprintf (at_buf, परिमाण at_buf, "$%u", चिह्नम्_सूचकांक);
  प्रत्यागम at_buf;
}

स्थिर शून्य
show_sub_सन्देशः (warnings warning,
                  ध्रुव वर्ण* cp, bool explicit_bracketing,
                  पूर्णाङ्क midrule_rhs_सूचकांक, वर्ण dollar_or_at,
                  ध्रुव variant *var)
{
  ध्रुव वर्ण *at_spec = get_at_spec (var->चिह्नम्_सूचकांक);

  यदि (var->err == 0)
    subcomplain (&var->loc, warning,
                 _("refers to: %c%s at %s"), dollar_or_at,
                 var->id, at_spec);
  अन्यथा
    {
      ध्रुव वर्ण *id;
      location id_loc;

      यदि (var->hidden_by)
        {
          id = var->hidden_by->id;
          id_loc = var->hidden_by->loc;
        }
      अन्यथा
        {
          id = var->id;
          id_loc = var->loc;
        }

      ध्रुव वर्ण *पुच्छ = explicit_bracketing ? "" : cp + वाक्यदैर्घ्यम् (var->id);

      /* Create the explanation message. */
      स्थिर संरचना obstack सन्देशः_buf;
      obstack_init (&सन्देशः_buf);

      obstack_printf (&सन्देशः_buf, _("possibly meant: %c"), dollar_or_at);
      यदि (अन्तर्भवति_dot_or_dash (id))
        obstack_printf (&सन्देशः_buf, "[%s]", id);
      अन्यथा
        obstack_sgrow (&सन्देशः_buf, id);
      obstack_sgrow (&सन्देशः_buf, पुच्छ);

      यदि (var->err & VARIANT_HIDDEN)
        {
          obstack_printf (&सन्देशः_buf, _(", hiding %c"), dollar_or_at);
          यदि (अन्तर्भवति_dot_or_dash (var->id))
            obstack_printf (&सन्देशः_buf, "[%s]", var->id);
          अन्यथा
            obstack_sgrow (&सन्देशः_buf, var->id);
          obstack_sgrow (&सन्देशः_buf, पुच्छ);
        }

      obstack_printf (&सन्देशः_buf, _(" at %s"), at_spec);

      यदि (var->err & VARIANT_NOT_VISIBLE_FROM_MIDRULE)
        obstack_printf (&सन्देशः_buf,
                        _(", cannot be accessed from midrule action at $%d"),
                        midrule_rhs_सूचकांक);

      subcomplain (&id_loc, warning, "%s",
                   obstack_finish0 (&सन्देशः_buf));
      obstack_free (&सन्देशः_buf, 0);
    }
}

स्थिर शून्य
show_sub_messages (warnings warning,
                   ध्रुव वर्ण* cp, bool explicit_bracketing,
                   पूर्णाङ्क midrule_rhs_सूचकांक, वर्ण dollar_or_at)
{
  प्रति (पूर्णाङ्क i = 0; i < variant_गणनम्; ++i)
    show_sub_सन्देशः (warning | silent,
                      cp, explicit_bracketing,
                      midrule_rhs_सूचकांक, dollar_or_at,
                      &variant_सारणी[i]);
}

/* Returned from "parse_ref" when the reference
   is inappropriate. */
#परिभाषय INVALID_REF (INT_MIN)

/* Returned from "parse_ref" when the reference
   points to LHS ($$) of the current rule or midrule. */
#परिभाषय LHS_REF (INT_MIN + 1)

/* Parse a positional reference in RULE.  */
स्थिर दीर्घ
parse_positional_ref (वर्ण *cp, पूर्णाङ्क नियमः_दीर्घता,
                      वर्ण *text, ध्रुव location *text_loc)
{
  दीर्घ अङ्कः = strtol (cp, &cp, 10);
  यदि (1 - अधिकतम_पूर्णाङ्कः + नियमः_दीर्घता <= अङ्कः && अङ्कः <= नियमः_दीर्घता)
    प्रत्यागम अङ्कः;
  अन्यथा
    {
      complain (text_loc, complaint, _("integer out of range: %s"),
                quote (text));
      प्रत्यागम INVALID_REF;
    }
}


/* Parse named or positional reference. In case of positional
   references, can return negative values for $-n "deep" stack
   accesses. */
स्थिर दीर्घ
parse_ref (वर्ण *cp, चिह्नम्_list *नियमः, पूर्णाङ्क नियमः_दीर्घता,
           पूर्णाङ्क midrule_rhs_सूचकांक, वर्ण *text, ध्रुव location *text_loc,
           वर्ण dollar_or_at)
{
  यदि ('$' == *cp)
    प्रत्यागम LHS_REF;

  यदि (c_isdigit (*cp) || (*cp == '-' && c_isdigit (cp[1])))
    प्रत्यागम parse_positional_ref (cp, नियमः_दीर्घता, text, text_loc);

  bool ध्रुव explicit_bracketing = *cp == '[';

  यदि (explicit_bracketing)
    ++cp;
  अन्यथा
    ref_पुच्छ_fields = strpbrk (cp, ".-");

  वर्ण ध्रुव *cp_अन्तः = strchr (cp, explicit_bracketing ? ']' : '\0');

  /* Add all relevant variants. */
  {
    पूर्णाङ्क चिह्नम्_सूचकांक;
    चिह्नम्_list *l;
    variant_गणनम् = 0;
    प्रति (चिह्नम्_सूचकांक = 0, l = नियमः; !चिह्नम्_list_null (l);
         ++चिह्नम्_सूचकांक, l = l->अग्र)
      {
        यदि (l->content_type != SYMLIST_चिह्नम्)
          अनुवर्त;

        variant *var
          = variant_सङ्कलय (l->content.sym->चिह्नकम्, l->sym_loc,
                         चिह्नम्_सूचकांक, cp, cp_अन्तः, explicit_bracketing);
        यदि (var && l->named_ref)
          var->hidden_by = l->named_ref;

        यदि (l->named_ref)
          variant_सङ्कलय (l->named_ref->id, l->named_ref->loc,
                       चिह्नम्_सूचकांक, cp, cp_अन्तः, explicit_bracketing);
      }
  }

  /* Check errors. */
  पूर्णाङ्क valid_variants = 0;
  पूर्णाङ्क valid_variant_सूचकांक = 0;
  प्रति (पूर्णाङ्क i = 0; i < variant_गणनम्; ++i)
    {
      variant *var = &variant_सारणी[i];
      पूर्णाङ्क चिह्नम्_सूचकांक = var->चिह्नम्_सूचकांक;

      /* Check visibility from midrule actions. */
      यदि (midrule_rhs_सूचकांक != 0
          && (चिह्नम्_सूचकांक == 0 || midrule_rhs_सूचकांक < चिह्नम्_सूचकांक))
        var->err |= VARIANT_NOT_VISIBLE_FROM_MIDRULE;

      /* Check correct bracketing. */
      यदि (!explicit_bracketing && अन्तर्भवति_dot_or_dash (var->id))
        var->err |= VARIANT_BAD_BRACKETING;

      /* Check using of hidden symbols. */
      यदि (var->hidden_by)
        var->err |= VARIANT_HIDDEN;

      यदि (!var->err)
        {
          valid_variant_सूचकांक = i;
          ++valid_variants;
        }
    }

  विकल्प (valid_variants)
    {
    पक्ष 0:
      {
        पूर्णाङ्क len = (explicit_bracketing || !ref_पुच्छ_fields) ?
          cp_अन्तः - cp : ref_पुच्छ_fields - cp;

        complain (text_loc, complaint,
                  _("invalid reference: %s"), quote (text));
        यदि (len == 0)
          {
            location sym_loc = *text_loc;
            sym_loc.start.column += 1;
            sym_loc.अन्तः = sym_loc.start;
            subcomplain (&sym_loc, complaint,
                         _("syntax error after '%c', expecting integer, "
                         "letter, '_', '[', or '$'"),
                         dollar_or_at);
          }
        अन्यथा यदि (midrule_rhs_सूचकांक)
          subcomplain (&नियमः->rhs_loc, complaint,
                       _("symbol not found in production before $%d: "
                       "%.*s"),
                       midrule_rhs_सूचकांक, len, cp);
        अन्यथा
          subcomplain (&नियमः->rhs_loc, complaint,
                       _("symbol not found in production: %.*s"),
                       len, cp);

        यदि (variant_गणनम् > 0)
          show_sub_messages (complaint,
                             cp, explicit_bracketing, midrule_rhs_सूचकांक,
                             dollar_or_at);
        प्रत्यागम INVALID_REF;
      }
    पक्ष 1:
      {
        यदि (variant_गणनम् > 1)
          {
            complain (text_loc, Wother,
                      _("misleading reference: %s"), quote (text));
            show_sub_messages (Wother,
                               cp, explicit_bracketing, midrule_rhs_सूचकांक,
                               dollar_or_at);
          }
        {
          पूर्णाङ्क चिह्नम्_सूचकांक =
            variant_सारणी[valid_variant_index].चिह्नम्_सूचकांक;
          प्रत्यागम (चिह्नम्_सूचकांक == midrule_rhs_सूचकांक) ? LHS_REF : चिह्नम्_सूचकांक;
        }
      }
    पक्ष 2:
    स्वभाव:
      {
        complain (text_loc, complaint,
                  _("ambiguous reference: %s"), quote (text));
        show_sub_messages (complaint,
                           cp, explicit_bracketing, midrule_rhs_सूचकांक,
                           dollar_or_at);
        प्रत्यागम INVALID_REF;
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
  यदि (*cp == '<')
    {
      *type_name = ++cp;
      /* Series of non-'>' or "->".  */
      यावत् (*cp != '>' || cp[-1] == '-')
        ++cp;

      /* The '>' symbol will be later replaced by '\0'. Original
         'text' is needed for error messages. */
      ++cp;
      यदि (untyped_var_आलोकितम्)
        complain (dollar_loc, complaint,
                  _("explicit type given in untyped grammar"));
      चिह्नकम्_आलोकितम् = true;
    }
  प्रत्यागम cp;
}

/*------------------------------------------------------------------.
| TEXT is pointing to a wannabee semantic value (i.e., a '$').      |
|                                                                   |
| Possible inputs: $[<TYPENAME>]($|INTEGER)                         |
|                                                                   |
| Output to OBSTACK_FOR_STRING a reference to this semantic value.  |
`------------------------------------------------------------------*/

स्थिर शून्य
handle_action_dollar (चिह्नम्_list *नियमः, वर्ण *text, ध्रुव location *dollar_loc)
{
  चिह्नम्_list *effective_नियमः;
  पूर्णाङ्क effective_नियमः_दीर्घता;

  यदि (नियमः->midrule_parent_नियमः)
    {
      effective_नियमः = नियमः->midrule_parent_नियमः;
      effective_नियमः_दीर्घता = नियमः->midrule_parent_rhs_सूचकांक - 1;
    }
  अन्यथा
    {
      effective_नियमः = नियमः;
      effective_नियमः_दीर्घता = चिह्नम्_list_दीर्घता (नियमः->अग्र);
    }

  /* The type name if explicit, otherwise left null. */
  वर्ण ध्रुव *type_name = रिक्त;
  वर्ण *cp = fetch_type_name (text + 1, &type_name, dollar_loc);
  पूर्णाङ्क n = parse_ref (cp, effective_नियमः, effective_नियमः_दीर्घता,
                     नियमः->midrule_parent_rhs_सूचकांक, text, dollar_loc, '$');
  /* End type_name.  Don't do it earlier: parse_ref depends on TEXT.  */
  यदि (type_name)
    cp[-1] = '\0';

  विकल्प (n)
    {
    पक्ष INVALID_REF:
      भङ्ग;

    पक्ष LHS_REF:
      {
        चिह्नम्_list *sym = चिह्नम्_list_n_get (नियमः, 0);
        यदि (!type_name
            && !sym->content.sym->content->type_name)
          {
            यदि (union_आलोकितम् || चिह्नकम्_आलोकितम्)
              {
                यदि (नियमः->midrule_parent_नियमः)
                  complain (dollar_loc, complaint,
                            _("$$ for the midrule at $%d of %s"
                              " has no declared type"),
                            नियमः->midrule_parent_rhs_सूचकांक,
                            quote (effective_नियमः->content.sym->चिह्नकम्));
                अन्यथा
                  complain (dollar_loc, complaint,
                            _("$$ of %s has no declared type"),
                            quote (नियमः->content.sym->चिह्नकम्));
              }
            अन्यथा
              untyped_var_आलोकितम् = true;
          }

        obstack_printf (&obstack_for_string, "]b4_lhs_value(orig %d, ",
                        sym->content.sym->content->सङ्ख्या);
        obstack_quote (&obstack_for_string, type_name);
        obstack_sgrow (&obstack_for_string, ")[");
        नियमः->action_props.is_मानम्_प्रयुक्तम् = true;
      }
      भङ्ग;

      /* Reference to a RHS symbol.  */
    स्वभाव:
      {
        यदि (max_वामः_semantic_context < 1 - n)
          max_वामः_semantic_context = 1 - n;
        चिह्नम्_list *sym = 0 < n ? चिह्नम्_list_n_get (effective_नियमः, n) : रिक्त;
        यदि (!type_name
            && (!sym || !sym->content.sym->content->type_name))
          {
            यदि (union_आलोकितम् || चिह्नकम्_आलोकितम्)
              complain (dollar_loc, complaint,
                        _("$%s of %s has no declared type"), cp,
                        quote (effective_नियमः->content.sym->चिह्नकम्));
            अन्यथा
              untyped_var_आलोकितम् = true;
          }

        obstack_printf (&obstack_for_string,
                        "]b4_rhs_value(%d, %d, ",
                        effective_नियमः_दीर्घता, n);
        यदि (sym)
          obstack_printf (&obstack_for_string, "%s%d, ",
                          sym->content.sym->content->class == nterm_sym ? "orig " : "",
                          sym->content.sym->content->सङ्ख्या);
        अन्यथा
          obstack_sgrow (&obstack_for_string, "[], ");

        obstack_quote (&obstack_for_string, type_name);
        obstack_sgrow (&obstack_for_string, ")[");
        यदि (0 < n)
          {
            यदि (muscle_percent_define_ifdef ("api.value.automove")
                && sym->action_props.is_मानम्_प्रयुक्तम्)
              complain (dollar_loc, Wother,
                        _("multiple occurrences of $%d with api.value.automove"),
                        n);
            sym->action_props.is_मानम्_प्रयुक्तम् = true;
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
handle_action_at (चिह्नम्_list *नियमः, वर्ण *text, ध्रुव location *at_loc)
{
  चिह्नम्_list *effective_नियमः;
  पूर्णाङ्क effective_नियमः_दीर्घता;

  यदि (नियमः->midrule_parent_नियमः)
    {
      effective_नियमः = नियमः->midrule_parent_नियमः;
      effective_नियमः_दीर्घता = नियमः->midrule_parent_rhs_सूचकांक - 1;
    }
  अन्यथा
    {
      effective_नियमः = नियमः;
      effective_नियमः_दीर्घता = चिह्नम्_list_दीर्घता (नियमः->अग्र);
    }

  muscle_percent_define_ensure ("locations", *at_loc, true);

  पूर्णाङ्क n = parse_ref (text + 1, effective_नियमः, effective_नियमः_दीर्घता,
                     नियमः->midrule_parent_rhs_सूचकांक, text, at_loc, '@');
  विकल्प (n)
    {
    पक्ष INVALID_REF:
      भङ्ग;

    पक्ष LHS_REF:
      obstack_sgrow (&obstack_for_string, "]b4_lhs_location[");
      भङ्ग;

    स्वभाव:
      obstack_printf (&obstack_for_string, "]b4_rhs_location(%d, %d)[",
                      effective_नियमः_दीर्घता, n);
      भङ्ग;
    }
}


/*-------------------------.
| Initialize the scanner.  |
`-------------------------*/

/* Translate the '$...' and '@...' in \a self, in the context \a
   sc_context (SC_RULE_ACTION, SC_SYMBOL_ACTION, INITIAL).  */

स्थिर वर्ण ध्रुव *
translate_action (संहिता_props *self, पूर्णाङ्क sc_context)
{
  loc->start = loc->अन्तः = self->location.start;
  yy_switch_गन्तव्यम्_बफर (yy_अवलोकनम्_string (self->संहिता));
  वर्ण *res = संहिता_lex (self, sc_context);
  yy_delete_बफर (YY_CURRENT_बफर);
  प्रत्यागम res;
}

/*------------------------------------------------------------------------.
| Implementation of the public interface as documented in "scan-code.h".  |
`------------------------------------------------------------------------*/

शून्य
संहिता_props_none_init (संहिता_props *self)
{
  *self = संहिता_props_none;
}

संहिता_props संहिता_props_none = संहिता_PROPS_NONE_INIT;

शून्य
संहिता_props_plain_init (संहिता_props *self, वर्ण ध्रुव *संहिता,
                       location संहिता_loc)
{
  संहिता_props_none_init (self);
  self->विधा = संहिता_PROPS_PLAIN;
  self->संहिता = संहिता;
  self->location = संहिता_loc;
}

शून्य
संहिता_props_चिह्नम्_action_init (संहिता_props *self, वर्ण ध्रुव *संहिता,
                               location संहिता_loc)
{
  संहिता_props_none_init (self);
  self->विधा = संहिता_PROPS_चिह्नम्_ACTION;
  self->संहिता = संहिता;
  self->location = संहिता_loc;
}

शून्य
संहिता_props_नियमः_action_init (संहिता_props *self, वर्ण ध्रुव *संहिता,
                             location संहिता_loc, चिह्नम्_list *नियमः,
                             named_ref *name, uniqstr type,
                             bool is_predicate)
{
  संहिता_props_none_init (self);
  self->विधा = संहिता_PROPS_नियमः_ACTION;
  self->संहिता = संहिता;
  self->location = संहिता_loc;
  self->नियमः = नियमः;
  self->named_ref = name;
  self->type = type;
  self->is_predicate = is_predicate;
}

शून्य
संहिता_props_translate_संहिता (संहिता_props *self)
{
  विकल्प (self->विधा)
    {
    पक्ष संहिता_PROPS_NONE:
      भङ्ग;
    पक्ष संहिता_PROPS_PLAIN:
      self->संहिता = translate_action (self, INITIAL);
      भङ्ग;
    पक्ष संहिता_PROPS_चिह्नम्_ACTION:
      self->संहिता = translate_action (self, SC_चिह्नम्_ACTION);
      भङ्ग;
    पक्ष संहिता_PROPS_नियमः_ACTION:
      self->संहिता = translate_action (self, SC_नियमः_ACTION);
      भङ्ग;
    }
}

शून्य
संहिता_scanner_चरमः_string_free (शून्य)
{
  STRING_FREE ();
}

शून्य
संहिता_scanner_init (शून्य)
{
  obstack_init (&obstack_for_string);
  yy_flex_debug = 0;
}

शून्य
संहिता_scanner_free (शून्य)
{
  obstack_free (&obstack_for_string, 0);
  variant_सारणी_free ();

  /* Reclaim Flex's buffers.  */
  yylex_destroy ();
}
