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
%option ಪೂರ್ವಪ್ರತ್ಯಯ="code_" outfile="lex.yy.c"

%{
#ಅನ್ತರ್ಭಾವ <c-ctype.h>
#ಅನ್ತರ್ಭಾವ <get-errno.h>
#ಅನ್ತರ್ಭಾವ <quote.h>

#ಅನ್ತರ್ಭಾವ "src/complain.h"
#ಅನ್ತರ್ಭಾವ "src/getargs.h"
#ಅನ್ತರ್ಭಾವ "src/muscle-tab.h"
#ಅನ್ತರ್ಭಾವ "src/reader.h"
#ಅನ್ತರ್ಭಾವ "src/scan-code.h"
#ಅನ್ತರ್ಭಾವ "src/symlist.h"

#ವ್ಯಾಖ್ಯಾನಿಸು FLEX_ಪೂರ್ವಪ್ರತ್ಯಯ(Id) ಸಂಕೇತ_3 ## Id
#ಅನ್ತರ್ಭಾವ "src/flex-scanner.h"

/* Work around a bug in flex 2.5.31.  See Debian bug 333231
   <https://bugs.debian.org/333231>.  */
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು ಸಂಕೇತ_wrap
#ವ್ಯಾಖ್ಯಾನಿಸು ಸಂಕೇತ_wrap() 1

ರಚನೆ obstack *obstack_for_actions = &obstack_for_string;

/* The current calling start condition: SC_RULE_ACTION or
   SC_SYMBOL_ACTION. */
# define YY_DECL ಸ್ಥಿರ ಅಕ್ಷರ *ಸಂಕೇತ_lex (ಸಂಕೇತ_props *self, ಪೂರ್ಣಾಂಕ sc_context)
YY_DECL;

#ವ್ಯಾಖ್ಯಾನಿಸು YY_USER_ACTION  location_compute (loc, &loc->ಕೊನೆ, yytext, yyleng);

ಸ್ಥಿರ ಅಕ್ಷರ *fetch_type_name (ಅಕ್ಷರ *cp, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ **type_name,
                              ಸ್ಥಿರಾಂಕ location *dollar_loc);

ಸ್ಥಿರ ಶೂನ್ಯ handle_action_dollar (ಚಿಹ್ನೆ_list *ನಿಯಮ, ಅಕ್ಷರ *cp,
                                  ಸ್ಥಿರಾಂಕ location *dollar_loc);
ಸ್ಥಿರ ಶೂನ್ಯ handle_action_at (ಚಿಹ್ನೆ_list *ನಿಯಮ, ಅಕ್ಷರ *cp,
                              ಸ್ಥಿರಾಂಕ location *at_loc);

/* A string to be pushed to obstack after dollar/at has been handled. */
ಸ್ಥಿರ ಅಕ್ಷರ *ref_ಬಾಲ_fields;

ಸ್ಥಿರ location current_loc;
ಸ್ಥಿರ location *loc = &current_loc;

/* A string representing the most recent translation.  */
ಸ್ಥಿರ ಅಕ್ಷರ *ಕೊನೆಯ_string;

/* True if an untyped $$ or $n was seen.  */
ಸ್ಥಿರ bool untyped_var_ನೋಡಿದ;

%}
 /* C and C++ comments in code. */
%x SC_COMMENT SC_ಸಾಲು_COMMENT
 /* Strings and characters in code. */
%x SC_STRING SC_CHARACTER
 /* Whether in a rule or symbol action.  Specifies the translation
    of $ and @.  */
%x SC_ನಿಯಮ_ACTION SC_ಚಿಹ್ನೆ_ACTION


/* POSIX says that a tag must be both an id and a C union member, but
   historically almost any character is allowed in a tag.  We disallow
   NUL and newline, as this simplifies our implementation.  We allow
   "->" as a means to dereference a pointer.  */
ಟ್ಯಾಗ್      ([^\0\n>]|->)*[^-]

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
  aver (sc_context == SC_ಚಿಹ್ನೆ_ACTION
        || sc_context == SC_ನಿಯಮ_ACTION
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

<SC_ಸಾಲು_COMMENT>
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


<SC_ನಿಯಮ_ACTION,SC_ಚಿಹ್ನೆ_ACTION>
{
  "'"              STRING_GROW (); BEGIN SC_CHARACTER;
  "\""             STRING_GROW (); BEGIN SC_STRING;
  "/"{splice}"*"   STRING_GROW (); BEGIN SC_COMMENT;
  "/"{splice}"/"   STRING_GROW (); BEGIN SC_ಸಾಲು_COMMENT;

  [$@]  {
    complain (loc, Wother, _("stray '%s'"), yytext);
    obstack_escape (&obstack_for_string, yytext);
  }
}

<SC_ನಿಯಮ_ACTION>
{
  "$"("<"{ಟ್ಯಾಗ್}">")?{ref}  {
    ref_ಬಾಲ_fields = ಖಾಲಿ;
    handle_action_dollar (self->ನಿಯಮ, yytext, loc);
    ಆದರೆ (ref_ಬಾಲ_fields)
      obstack_sgrow (&obstack_for_string, ref_ಬಾಲ_fields);
  }
  "@"{ref} {
    ref_ಬಾಲ_fields = ಖಾಲಿ;
    handle_action_at (self->ನಿಯಮ, yytext, loc);
    ಆದರೆ (ref_ಬಾಲ_fields)
      obstack_sgrow (&obstack_for_string, ref_ಬಾಲ_fields);
  }
}

<SC_ಚಿಹ್ನೆ_ACTION>
{
  "$"("<"{ಟ್ಯಾಗ್}">")?"$" {
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *type_name = ಖಾಲಿ;
    fetch_type_name (yytext + 1, &type_name, loc)[-1] = 0;
    obstack_sgrow (&obstack_for_string, "]b4_dollar_dollar(");
    obstack_quote (&obstack_for_string, type_name);
    obstack_sgrow (&obstack_for_string, ")[");
    self->is_ಮೌಲ್ಯ_ಬಳಸಿದ = true;
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
  <<EOF>>     STRING_ಮುಕ್ತಾಯ (); ಹಿಂತಿರುಗಿ ಕೊನೆಯ_string;
}

%%

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
is_dot_or_dash (ಅಕ್ಷರ ch)
{
  ಹಿಂತಿರುಗಿ ch == '.' || ch == '-';
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
ಒಳಗೊಂಡಿದೆ_dot_or_dash (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* p)
{
  ಹಿಂತಿರುಗಿ !!strpbrk (p, ".-");
}

/* Defines a variant of a symbolic name resolution. */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ
{
  /* Index in symbol list. */
  ಪೂರ್ಣಾಂಕ ಚಿಹ್ನೆ_ಸೂಚ್ಯಂಕ;

  /* Matched symbol id and loc. */
  uniqstr id;
  location loc;

  /* Hiding named reference. */
  named_ref* hidden_by;

  /* Error flags. May contain zero (no errors) or
     a combination of VARIANT_* values. */
  ಚಿಹ್ನೆರಹಿತ err;
} variant;

/* Set when the variant refers to a symbol hidden
   by an explicit symbol reference. */
#ವ್ಯಾಖ್ಯಾನಿಸು VARIANT_HIDDEN (1 << 0)

/* Set when the variant refers to a symbol containing
   dots or dashes. Will require explicit bracketing. */
#ವ್ಯಾಖ್ಯಾನಿಸು VARIANT_BAD_BRACKETING (1 << 1)

/* Set when the variant refers to a symbol which is
   not visible from current midrule. */
#ವ್ಯಾಖ್ಯಾನಿಸು VARIANT_NOT_VISIBLE_FROM_MIDRULE (1 << 2)

ಸ್ಥಿರ variant *variant_ಕೋಷ್ಟಕ = ಖಾಲಿ;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ variant_ಕೋಷ್ಟಕ_ಗಾತ್ರ = 0;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ variant_ಎಣಿಕೆ = 0;

ಸ್ಥಿರ variant *
variant_ಕೋಷ್ಟಕ_grow (ಶೂನ್ಯ)
{
  ++variant_ಎಣಿಕೆ;
  ಆದರೆ (variant_ಎಣಿಕೆ > variant_ಕೋಷ್ಟಕ_ಗಾತ್ರ)
    {
      ಆಗ (variant_ಎಣಿಕೆ > variant_ಕೋಷ್ಟಕ_ಗಾತ್ರ)
        variant_ಕೋಷ್ಟಕ_ಗಾತ್ರ = 2 * variant_ಕೋಷ್ಟಕ_ಗಾತ್ರ + 3;
      variant_ಕೋಷ್ಟಕ = xnrealloc (variant_ಕೋಷ್ಟಕ, variant_ಕೋಷ್ಟಕ_ಗಾತ್ರ,
                                 ಗಾತ್ರ *variant_ಕೋಷ್ಟಕ);
    }
  ಹಿಂತಿರುಗಿ &variant_ಕೋಷ್ಟಕ[variant_count - 1];
}

ಸ್ಥಿರ ಶೂನ್ಯ
variant_ಕೋಷ್ಟಕ_free (ಶೂನ್ಯ)
{
  ಬಿಡುಗಡೆ (variant_ಕೋಷ್ಟಕ);
  variant_ಕೋಷ್ಟಕ = ಖಾಲಿ;
  variant_ಕೋಷ್ಟಕ_ಗಾತ್ರ = variant_ಎಣಿಕೆ = 0;
}

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *
ಕಂಡುಹಿಡಿ_ಪೂರ್ವಪ್ರತ್ಯಯ_ಕೊನೆ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಪೂರ್ವಪ್ರತ್ಯಯ, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *cp, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಕೊನೆ)
{
  ಗಾಗಿ (; *ಪೂರ್ವಪ್ರತ್ಯಯ && cp != ಕೊನೆ; ++ಪೂರ್ವಪ್ರತ್ಯಯ, ++cp)
    ಆದರೆ (*ಪೂರ್ವಪ್ರತ್ಯಯ != *cp)
      ಹಿಂತಿರುಗಿ ಖಾಲಿ;

  ಹಿಂತಿರುಗಿ *ಪೂರ್ವಪ್ರತ್ಯಯ ? ಖಾಲಿ : cp;
}

ಸ್ಥಿರ variant *
variant_ಕೂಡಿಸು (uniqstr id, location id_loc, ಪೂರ್ಣಾಂಕ ಚಿಹ್ನೆ_ಸೂಚ್ಯಂಕ,
             ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *cp, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *cp_ಕೊನೆ, bool explicit_bracketing)
{
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಪೂರ್ವಪ್ರತ್ಯಯ_ಕೊನೆ = ಕಂಡುಹಿಡಿ_ಪೂರ್ವಪ್ರತ್ಯಯ_ಕೊನೆ (id, cp, cp_ಕೊನೆ);
  ಆದರೆ (ಪೂರ್ವಪ್ರತ್ಯಯ_ಕೊನೆ &&
      (ಪೂರ್ವಪ್ರತ್ಯಯ_ಕೊನೆ == cp_ಕೊನೆ ||
       (!explicit_bracketing && is_dot_or_dash (*ಪೂರ್ವಪ್ರತ್ಯಯ_ಕೊನೆ))))
    {
      variant *r = variant_ಕೋಷ್ಟಕ_grow ();
      r->ಚಿಹ್ನೆ_ಸೂಚ್ಯಂಕ = ಚಿಹ್ನೆ_ಸೂಚ್ಯಂಕ;
      r->id = id;
      r->loc = id_loc;
      r->hidden_by = ಖಾಲಿ;
      r->err = 0;
      ಹಿಂತಿರುಗಿ r;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
get_at_spec(ಪೂರ್ಣಾಂಕ ಚಿಹ್ನೆ_ಸೂಚ್ಯಂಕ)
{
  ಸ್ಥಿರ ಅಕ್ಷರ at_buf[20];
  ಆದರೆ (ಚಿಹ್ನೆ_ಸೂಚ್ಯಂಕ == 0)
    strcpy (at_buf, "$$");
  ಇಲ್ಲದಿದ್ದರೆ
    snprintf (at_buf, ಗಾತ್ರ at_buf, "$%u", ಚಿಹ್ನೆ_ಸೂಚ್ಯಂಕ);
  ಹಿಂತಿರುಗಿ at_buf;
}

ಸ್ಥಿರ ಶೂನ್ಯ
show_sub_ಸಂದೇಶ (warnings warning,
                  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* cp, bool explicit_bracketing,
                  ಪೂರ್ಣಾಂಕ midrule_rhs_ಸೂಚ್ಯಂಕ, ಅಕ್ಷರ dollar_or_at,
                  ಸ್ಥಿರಾಂಕ variant *var)
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *at_spec = get_at_spec (var->ಚಿಹ್ನೆ_ಸೂಚ್ಯಂಕ);

  ಆದರೆ (var->err == 0)
    subcomplain (&var->loc, warning,
                 _("refers to: %c%s at %s"), dollar_or_at,
                 var->id, at_spec);
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id;
      location id_loc;

      ಆದರೆ (var->hidden_by)
        {
          id = var->hidden_by->id;
          id_loc = var->hidden_by->loc;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          id = var->id;
          id_loc = var->loc;
        }

      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಬಾಲ = explicit_bracketing ? "" : cp + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (var->id);

      /* Create the explanation message. */
      ಸ್ಥಿರ ರಚನೆ obstack ಸಂದೇಶ_buf;
      obstack_init (&ಸಂದೇಶ_buf);

      obstack_printf (&ಸಂದೇಶ_buf, _("possibly meant: %c"), dollar_or_at);
      ಆದರೆ (ಒಳಗೊಂಡಿದೆ_dot_or_dash (id))
        obstack_printf (&ಸಂದೇಶ_buf, "[%s]", id);
      ಇಲ್ಲದಿದ್ದರೆ
        obstack_sgrow (&ಸಂದೇಶ_buf, id);
      obstack_sgrow (&ಸಂದೇಶ_buf, ಬಾಲ);

      ಆದರೆ (var->err & VARIANT_HIDDEN)
        {
          obstack_printf (&ಸಂದೇಶ_buf, _(", hiding %c"), dollar_or_at);
          ಆದರೆ (ಒಳಗೊಂಡಿದೆ_dot_or_dash (var->id))
            obstack_printf (&ಸಂದೇಶ_buf, "[%s]", var->id);
          ಇಲ್ಲದಿದ್ದರೆ
            obstack_sgrow (&ಸಂದೇಶ_buf, var->id);
          obstack_sgrow (&ಸಂದೇಶ_buf, ಬಾಲ);
        }

      obstack_printf (&ಸಂದೇಶ_buf, _(" at %s"), at_spec);

      ಆದರೆ (var->err & VARIANT_NOT_VISIBLE_FROM_MIDRULE)
        obstack_printf (&ಸಂದೇಶ_buf,
                        _(", cannot be accessed from midrule action at $%d"),
                        midrule_rhs_ಸೂಚ್ಯಂಕ);

      subcomplain (&id_loc, warning, "%s",
                   obstack_finish0 (&ಸಂದೇಶ_buf));
      obstack_free (&ಸಂದೇಶ_buf, 0);
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
show_sub_messages (warnings warning,
                   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* cp, bool explicit_bracketing,
                   ಪೂರ್ಣಾಂಕ midrule_rhs_ಸೂಚ್ಯಂಕ, ಅಕ್ಷರ dollar_or_at)
{
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < variant_ಎಣಿಕೆ; ++i)
    show_sub_ಸಂದೇಶ (warning | silent,
                      cp, explicit_bracketing,
                      midrule_rhs_ಸೂಚ್ಯಂಕ, dollar_or_at,
                      &variant_ಕೋಷ್ಟಕ[i]);
}

/* Returned from "parse_ref" when the reference
   is inappropriate. */
#ವ್ಯಾಖ್ಯಾನಿಸು INVALID_REF (INT_MIN)

/* Returned from "parse_ref" when the reference
   points to LHS ($$) of the current rule or midrule. */
#ವ್ಯಾಖ್ಯಾನಿಸು LHS_REF (INT_MIN + 1)

/* Parse a positional reference in RULE.  */
ಸ್ಥಿರ ದೀರ್ಘ
parse_positional_ref (ಅಕ್ಷರ *cp, ಪೂರ್ಣಾಂಕ ನಿಯಮ_ಉದ್ದಳತೆ,
                      ಅಕ್ಷರ *ಪಠ್ಯ, ಸ್ಥಿರಾಂಕ location *ಪಠ್ಯ_loc)
{
  ದೀರ್ಘ ಅಂಕೆ = strtol (cp, &cp, 10);
  ಆದರೆ (1 - ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ + ನಿಯಮ_ಉದ್ದಳತೆ <= ಅಂಕೆ && ಅಂಕೆ <= ನಿಯಮ_ಉದ್ದಳತೆ)
    ಹಿಂತಿರುಗಿ ಅಂಕೆ;
  ಇಲ್ಲದಿದ್ದರೆ
    {
      complain (ಪಠ್ಯ_loc, complaint, _("integer out of range: %s"),
                quote (ಪಠ್ಯ));
      ಹಿಂತಿರುಗಿ INVALID_REF;
    }
}


/* Parse named or positional reference. In case of positional
   references, can return negative values for $-n "deep" stack
   accesses. */
ಸ್ಥಿರ ದೀರ್ಘ
parse_ref (ಅಕ್ಷರ *cp, ಚಿಹ್ನೆ_list *ನಿಯಮ, ಪೂರ್ಣಾಂಕ ನಿಯಮ_ಉದ್ದಳತೆ,
           ಪೂರ್ಣಾಂಕ midrule_rhs_ಸೂಚ್ಯಂಕ, ಅಕ್ಷರ *ಪಠ್ಯ, ಸ್ಥಿರಾಂಕ location *ಪಠ್ಯ_loc,
           ಅಕ್ಷರ dollar_or_at)
{
  ಆದರೆ ('$' == *cp)
    ಹಿಂತಿರುಗಿ LHS_REF;

  ಆದರೆ (c_isdigit (*cp) || (*cp == '-' && c_isdigit (cp[1])))
    ಹಿಂತಿರುಗಿ parse_positional_ref (cp, ನಿಯಮ_ಉದ್ದಳತೆ, ಪಠ್ಯ, ಪಠ್ಯ_loc);

  bool ಸ್ಥಿರಾಂಕ explicit_bracketing = *cp == '[';

  ಆದರೆ (explicit_bracketing)
    ++cp;
  ಇಲ್ಲದಿದ್ದರೆ
    ref_ಬಾಲ_fields = strpbrk (cp, ".-");

  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *cp_ಕೊನೆ = strchr (cp, explicit_bracketing ? ']' : '\0');

  /* Add all relevant variants. */
  {
    ಪೂರ್ಣಾಂಕ ಚಿಹ್ನೆ_ಸೂಚ್ಯಂಕ;
    ಚಿಹ್ನೆ_list *l;
    variant_ಎಣಿಕೆ = 0;
    ಗಾಗಿ (ಚಿಹ್ನೆ_ಸೂಚ್ಯಂಕ = 0, l = ನಿಯಮ; !ಚಿಹ್ನೆ_list_null (l);
         ++ಚಿಹ್ನೆ_ಸೂಚ್ಯಂಕ, l = l->ಮುಂದಿನ)
      {
        ಆದರೆ (l->content_type != SYMLIST_ಚಿಹ್ನೆ)
          ಮುಂದುವರಿ;

        variant *var
          = variant_ಕೂಡಿಸು (l->content.sym->ಟ್ಯಾಗ್, l->sym_loc,
                         ಚಿಹ್ನೆ_ಸೂಚ್ಯಂಕ, cp, cp_ಕೊನೆ, explicit_bracketing);
        ಆದರೆ (var && l->named_ref)
          var->hidden_by = l->named_ref;

        ಆದರೆ (l->named_ref)
          variant_ಕೂಡಿಸು (l->named_ref->id, l->named_ref->loc,
                       ಚಿಹ್ನೆ_ಸೂಚ್ಯಂಕ, cp, cp_ಕೊನೆ, explicit_bracketing);
      }
  }

  /* Check errors. */
  ಪೂರ್ಣಾಂಕ valid_variants = 0;
  ಪೂರ್ಣಾಂಕ valid_variant_ಸೂಚ್ಯಂಕ = 0;
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < variant_ಎಣಿಕೆ; ++i)
    {
      variant *var = &variant_ಕೋಷ್ಟಕ[i];
      ಪೂರ್ಣಾಂಕ ಚಿಹ್ನೆ_ಸೂಚ್ಯಂಕ = var->ಚಿಹ್ನೆ_ಸೂಚ್ಯಂಕ;

      /* Check visibility from midrule actions. */
      ಆದರೆ (midrule_rhs_ಸೂಚ್ಯಂಕ != 0
          && (ಚಿಹ್ನೆ_ಸೂಚ್ಯಂಕ == 0 || midrule_rhs_ಸೂಚ್ಯಂಕ < ಚಿಹ್ನೆ_ಸೂಚ್ಯಂಕ))
        var->err |= VARIANT_NOT_VISIBLE_FROM_MIDRULE;

      /* Check correct bracketing. */
      ಆದರೆ (!explicit_bracketing && ಒಳಗೊಂಡಿದೆ_dot_or_dash (var->id))
        var->err |= VARIANT_BAD_BRACKETING;

      /* Check using of hidden symbols. */
      ಆದರೆ (var->hidden_by)
        var->err |= VARIANT_HIDDEN;

      ಆದರೆ (!var->err)
        {
          valid_variant_ಸೂಚ್ಯಂಕ = i;
          ++valid_variants;
        }
    }

  ಆಯ್ಕೆ (valid_variants)
    {
    ಸ್ಥಿತಿ 0:
      {
        ಪೂರ್ಣಾಂಕ len = (explicit_bracketing || !ref_ಬಾಲ_fields) ?
          cp_ಕೊನೆ - cp : ref_ಬಾಲ_fields - cp;

        complain (ಪಠ್ಯ_loc, complaint,
                  _("invalid reference: %s"), quote (ಪಠ್ಯ));
        ಆದರೆ (len == 0)
          {
            location sym_loc = *ಪಠ್ಯ_loc;
            sym_loc.ಆರಂಭ.ಸ್ತಂಭ += 1;
            sym_loc.ಕೊನೆ = sym_loc.ಆರಂಭ;
            subcomplain (&sym_loc, complaint,
                         _("syntax error after '%c', expecting integer, "
                         "letter, '_', '[', or '$'"),
                         dollar_or_at);
          }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (midrule_rhs_ಸೂಚ್ಯಂಕ)
          subcomplain (&ನಿಯಮ->rhs_loc, complaint,
                       _("symbol not found in production before $%d: "
                       "%.*s"),
                       midrule_rhs_ಸೂಚ್ಯಂಕ, len, cp);
        ಇಲ್ಲದಿದ್ದರೆ
          subcomplain (&ನಿಯಮ->rhs_loc, complaint,
                       _("symbol not found in production: %.*s"),
                       len, cp);

        ಆದರೆ (variant_ಎಣಿಕೆ > 0)
          show_sub_messages (complaint,
                             cp, explicit_bracketing, midrule_rhs_ಸೂಚ್ಯಂಕ,
                             dollar_or_at);
        ಹಿಂತಿರುಗಿ INVALID_REF;
      }
    ಸ್ಥಿತಿ 1:
      {
        ಆದರೆ (variant_ಎಣಿಕೆ > 1)
          {
            complain (ಪಠ್ಯ_loc, Wother,
                      _("misleading reference: %s"), quote (ಪಠ್ಯ));
            show_sub_messages (Wother,
                               cp, explicit_bracketing, midrule_rhs_ಸೂಚ್ಯಂಕ,
                               dollar_or_at);
          }
        {
          ಪೂರ್ಣಾಂಕ ಚಿಹ್ನೆ_ಸೂಚ್ಯಂಕ =
            variant_ಕೋಷ್ಟಕ[valid_variant_index].ಚಿಹ್ನೆ_ಸೂಚ್ಯಂಕ;
          ಹಿಂತಿರುಗಿ (ಚಿಹ್ನೆ_ಸೂಚ್ಯಂಕ == midrule_rhs_ಸೂಚ್ಯಂಕ) ? LHS_REF : ಚಿಹ್ನೆ_ಸೂಚ್ಯಂಕ;
        }
      }
    ಸ್ಥಿತಿ 2:
    ಪೂರ್ವನಿಯೋಜಿತ:
      {
        complain (ಪಠ್ಯ_loc, complaint,
                  _("ambiguous reference: %s"), quote (ಪಠ್ಯ));
        show_sub_messages (complaint,
                           cp, explicit_bracketing, midrule_rhs_ಸೂಚ್ಯಂಕ,
                           dollar_or_at);
        ಹಿಂತಿರುಗಿ INVALID_REF;
      }
    }
}

/* Keeps track of the maximum number of semantic values to the left of
   a handle (those referenced by $0, $-1, etc.) are required by the
   semantic actions of this grammar. */
ಪೂರ್ಣಾಂಕ max_ಎಡ_semantic_context = 0;


/* If CP points to a typename (i.e., <.*?>), set TYPE_NAME to its
   beginning (i.e., after the opening "<", and return the pointer
   immediately after it.  */

ಸ್ಥಿರ
ಅಕ್ಷರ *
fetch_type_name (ಅಕ್ಷರ *cp, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ **type_name,
                 ಸ್ಥಿರಾಂಕ location *dollar_loc)
{
  ಆದರೆ (*cp == '<')
    {
      *type_name = ++cp;
      /* Series of non-'>' or "->".  */
      ಆಗ (*cp != '>' || cp[-1] == '-')
        ++cp;

      /* The '>' symbol will be later replaced by '\0'. Original
         'text' is needed for error messages. */
      ++cp;
      ಆದರೆ (untyped_var_ನೋಡಿದ)
        complain (dollar_loc, complaint,
                  _("explicit type given in untyped grammar"));
      ಟ್ಯಾಗ್_ನೋಡಿದ = true;
    }
  ಹಿಂತಿರುಗಿ cp;
}

/*------------------------------------------------------------------.
| TEXT is pointing to a wannabee semantic value (i.e., a '$').      |
|                                                                   |
| Possible inputs: $[<TYPENAME>]($|INTEGER)                         |
|                                                                   |
| Output to OBSTACK_FOR_STRING a reference to this semantic value.  |
`------------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
handle_action_dollar (ಚಿಹ್ನೆ_list *ನಿಯಮ, ಅಕ್ಷರ *ಪಠ್ಯ, ಸ್ಥಿರಾಂಕ location *dollar_loc)
{
  ಚಿಹ್ನೆ_list *effective_ನಿಯಮ;
  ಪೂರ್ಣಾಂಕ effective_ನಿಯಮ_ಉದ್ದಳತೆ;

  ಆದರೆ (ನಿಯಮ->midrule_ಪೋಷಕ_ನಿಯಮ)
    {
      effective_ನಿಯಮ = ನಿಯಮ->midrule_ಪೋಷಕ_ನಿಯಮ;
      effective_ನಿಯಮ_ಉದ್ದಳತೆ = ನಿಯಮ->midrule_ಪೋಷಕ_rhs_ಸೂಚ್ಯಂಕ - 1;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      effective_ನಿಯಮ = ನಿಯಮ;
      effective_ನಿಯಮ_ಉದ್ದಳತೆ = ಚಿಹ್ನೆ_list_ಉದ್ದಳತೆ (ನಿಯಮ->ಮುಂದಿನ);
    }

  /* The type name if explicit, otherwise left null. */
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *type_name = ಖಾಲಿ;
  ಅಕ್ಷರ *cp = fetch_type_name (ಪಠ್ಯ + 1, &type_name, dollar_loc);
  ಪೂರ್ಣಾಂಕ n = parse_ref (cp, effective_ನಿಯಮ, effective_ನಿಯಮ_ಉದ್ದಳತೆ,
                     ನಿಯಮ->midrule_ಪೋಷಕ_rhs_ಸೂಚ್ಯಂಕ, ಪಠ್ಯ, dollar_loc, '$');
  /* End type_name.  Don't do it earlier: parse_ref depends on TEXT.  */
  ಆದರೆ (type_name)
    cp[-1] = '\0';

  ಆಯ್ಕೆ (n)
    {
    ಸ್ಥಿತಿ INVALID_REF:
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ LHS_REF:
      {
        ಚಿಹ್ನೆ_list *sym = ಚಿಹ್ನೆ_list_n_get (ನಿಯಮ, 0);
        ಆದರೆ (!type_name
            && !sym->content.sym->content->type_name)
          {
            ಆದರೆ (union_ನೋಡಿದ || ಟ್ಯಾಗ್_ನೋಡಿದ)
              {
                ಆದರೆ (ನಿಯಮ->midrule_ಪೋಷಕ_ನಿಯಮ)
                  complain (dollar_loc, complaint,
                            _("$$ for the midrule at $%d of %s"
                              " has no declared type"),
                            ನಿಯಮ->midrule_ಪೋಷಕ_rhs_ಸೂಚ್ಯಂಕ,
                            quote (effective_ನಿಯಮ->content.sym->ಟ್ಯಾಗ್));
                ಇಲ್ಲದಿದ್ದರೆ
                  complain (dollar_loc, complaint,
                            _("$$ of %s has no declared type"),
                            quote (ನಿಯಮ->content.sym->ಟ್ಯಾಗ್));
              }
            ಇಲ್ಲದಿದ್ದರೆ
              untyped_var_ನೋಡಿದ = true;
          }

        obstack_printf (&obstack_for_string, "]b4_lhs_value(orig %d, ",
                        sym->content.sym->content->ಸಂಖ್ಯೆ);
        obstack_quote (&obstack_for_string, type_name);
        obstack_sgrow (&obstack_for_string, ")[");
        ನಿಯಮ->action_props.is_ಮೌಲ್ಯ_ಬಳಸಿದ = true;
      }
      ನಿಲ್ಲಿಸು;

      /* Reference to a RHS symbol.  */
    ಪೂರ್ವನಿಯೋಜಿತ:
      {
        ಆದರೆ (max_ಎಡ_semantic_context < 1 - n)
          max_ಎಡ_semantic_context = 1 - n;
        ಚಿಹ್ನೆ_list *sym = 0 < n ? ಚಿಹ್ನೆ_list_n_get (effective_ನಿಯಮ, n) : ಖಾಲಿ;
        ಆದರೆ (!type_name
            && (!sym || !sym->content.sym->content->type_name))
          {
            ಆದರೆ (union_ನೋಡಿದ || ಟ್ಯಾಗ್_ನೋಡಿದ)
              complain (dollar_loc, complaint,
                        _("$%s of %s has no declared type"), cp,
                        quote (effective_ನಿಯಮ->content.sym->ಟ್ಯಾಗ್));
            ಇಲ್ಲದಿದ್ದರೆ
              untyped_var_ನೋಡಿದ = true;
          }

        obstack_printf (&obstack_for_string,
                        "]b4_rhs_value(%d, %d, ",
                        effective_ನಿಯಮ_ಉದ್ದಳತೆ, n);
        ಆದರೆ (sym)
          obstack_printf (&obstack_for_string, "%s%d, ",
                          sym->content.sym->content->class == nterm_sym ? "orig " : "",
                          sym->content.sym->content->ಸಂಖ್ಯೆ);
        ಇಲ್ಲದಿದ್ದರೆ
          obstack_sgrow (&obstack_for_string, "[], ");

        obstack_quote (&obstack_for_string, type_name);
        obstack_sgrow (&obstack_for_string, ")[");
        ಆದರೆ (0 < n)
          {
            ಆದರೆ (muscle_percent_define_ifdef ("api.value.automove")
                && sym->action_props.is_ಮೌಲ್ಯ_ಬಳಸಿದ)
              complain (dollar_loc, Wother,
                        _("multiple occurrences of $%d with api.value.automove"),
                        n);
            sym->action_props.is_ಮೌಲ್ಯ_ಬಳಸಿದ = true;
          }
      }
      ನಿಲ್ಲಿಸು;
    }
}


/*------------------------------------------------------.
| TEXT is a location token (i.e., a '@...').  Output to |
| OBSTACK_FOR_STRING a reference to this location.      |
`------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
handle_action_at (ಚಿಹ್ನೆ_list *ನಿಯಮ, ಅಕ್ಷರ *ಪಠ್ಯ, ಸ್ಥಿರಾಂಕ location *at_loc)
{
  ಚಿಹ್ನೆ_list *effective_ನಿಯಮ;
  ಪೂರ್ಣಾಂಕ effective_ನಿಯಮ_ಉದ್ದಳತೆ;

  ಆದರೆ (ನಿಯಮ->midrule_ಪೋಷಕ_ನಿಯಮ)
    {
      effective_ನಿಯಮ = ನಿಯಮ->midrule_ಪೋಷಕ_ನಿಯಮ;
      effective_ನಿಯಮ_ಉದ್ದಳತೆ = ನಿಯಮ->midrule_ಪೋಷಕ_rhs_ಸೂಚ್ಯಂಕ - 1;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      effective_ನಿಯಮ = ನಿಯಮ;
      effective_ನಿಯಮ_ಉದ್ದಳತೆ = ಚಿಹ್ನೆ_list_ಉದ್ದಳತೆ (ನಿಯಮ->ಮುಂದಿನ);
    }

  muscle_percent_define_ensure ("locations", *at_loc, true);

  ಪೂರ್ಣಾಂಕ n = parse_ref (ಪಠ್ಯ + 1, effective_ನಿಯಮ, effective_ನಿಯಮ_ಉದ್ದಳತೆ,
                     ನಿಯಮ->midrule_ಪೋಷಕ_rhs_ಸೂಚ್ಯಂಕ, ಪಠ್ಯ, at_loc, '@');
  ಆಯ್ಕೆ (n)
    {
    ಸ್ಥಿತಿ INVALID_REF:
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ LHS_REF:
      obstack_sgrow (&obstack_for_string, "]b4_lhs_location[");
      ನಿಲ್ಲಿಸು;

    ಪೂರ್ವನಿಯೋಜಿತ:
      obstack_printf (&obstack_for_string, "]b4_rhs_location(%d, %d)[",
                      effective_ನಿಯಮ_ಉದ್ದಳತೆ, n);
      ನಿಲ್ಲಿಸು;
    }
}


/*-------------------------.
| Initialize the scanner.  |
`-------------------------*/

/* Translate the '$...' and '@...' in \a self, in the context \a
   sc_context (SC_RULE_ACTION, SC_SYMBOL_ACTION, INITIAL).  */

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *
translate_action (ಸಂಕೇತ_props *self, ಪೂರ್ಣಾಂಕ sc_context)
{
  loc->ಆರಂಭ = loc->ಕೊನೆ = self->location.ಆರಂಭ;
  yy_switch_ಗಮ್ಯ_ಬಫರ್ (yy_ಪರಿಶೀಲನೆ_string (self->ಸಂಕೇತ_2));
  ಅಕ್ಷರ *res = ಸಂಕೇತ_lex (self, sc_context);
  yy_delete_ಬಫರ್ (YY_CURRENT_ಬಫರ್);
  ಹಿಂತಿರುಗಿ res;
}

/*------------------------------------------------------------------------.
| Implementation of the public interface as documented in "scan-code.h".  |
`------------------------------------------------------------------------*/

ಶೂನ್ಯ
ಸಂಕೇತ_props_none_init (ಸಂಕೇತ_props *self)
{
  *self = ಸಂಕೇತ_props_none;
}

ಸಂಕೇತ_props ಸಂಕೇತ_props_none = ಸಂಕೇತ_PROPS_NONE_INIT;

ಶೂನ್ಯ
ಸಂಕೇತ_props_plain_init (ಸಂಕೇತ_props *self, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸಂಕೇತ_2,
                       location ಸಂಕೇತ_loc)
{
  ಸಂಕೇತ_props_none_init (self);
  self->ಬಗೆ = ಸಂಕೇತ_PROPS_PLAIN;
  self->ಸಂಕೇತ_2 = ಸಂಕೇತ_2;
  self->location = ಸಂಕೇತ_loc;
}

ಶೂನ್ಯ
ಸಂಕೇತ_props_ಚಿಹ್ನೆ_action_init (ಸಂಕೇತ_props *self, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸಂಕೇತ_2,
                               location ಸಂಕೇತ_loc)
{
  ಸಂಕೇತ_props_none_init (self);
  self->ಬಗೆ = ಸಂಕೇತ_PROPS_ಚಿಹ್ನೆ_ACTION;
  self->ಸಂಕೇತ_2 = ಸಂಕೇತ_2;
  self->location = ಸಂಕೇತ_loc;
}

ಶೂನ್ಯ
ಸಂಕೇತ_props_ನಿಯಮ_action_init (ಸಂಕೇತ_props *self, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸಂಕೇತ_2,
                             location ಸಂಕೇತ_loc, ಚಿಹ್ನೆ_list *ನಿಯಮ,
                             named_ref *name, uniqstr type,
                             bool is_predicate)
{
  ಸಂಕೇತ_props_none_init (self);
  self->ಬಗೆ = ಸಂಕೇತ_PROPS_ನಿಯಮ_ACTION;
  self->ಸಂಕೇತ_2 = ಸಂಕೇತ_2;
  self->location = ಸಂಕೇತ_loc;
  self->ನಿಯಮ = ನಿಯಮ;
  self->named_ref = name;
  self->type = type;
  self->is_predicate = is_predicate;
}

ಶೂನ್ಯ
ಸಂಕೇತ_props_translate_ಸಂಕೇತ (ಸಂಕೇತ_props *self)
{
  ಆಯ್ಕೆ (self->ಬಗೆ)
    {
    ಸ್ಥಿತಿ ಸಂಕೇತ_PROPS_NONE:
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ ಸಂಕೇತ_PROPS_PLAIN:
      self->ಸಂಕೇತ_2 = translate_action (self, INITIAL);
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ ಸಂಕೇತ_PROPS_ಚಿಹ್ನೆ_ACTION:
      self->ಸಂಕೇತ_2 = translate_action (self, SC_ಚಿಹ್ನೆ_ACTION);
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ ಸಂಕೇತ_PROPS_ನಿಯಮ_ACTION:
      self->ಸಂಕೇತ_2 = translate_action (self, SC_ನಿಯಮ_ACTION);
      ನಿಲ್ಲಿಸು;
    }
}

ಶೂನ್ಯ
ಸಂಕೇತ_scanner_ಕೊನೆಯ_string_free (ಶೂನ್ಯ)
{
  STRING_FREE ();
}

ಶೂನ್ಯ
ಸಂಕೇತ_scanner_init (ಶೂನ್ಯ)
{
  obstack_init (&obstack_for_string);
  yy_flex_debug = 0;
}

ಶೂನ್ಯ
ಸಂಕೇತ_scanner_free (ಶೂನ್ಯ)
{
  obstack_free (&obstack_for_string, 0);
  variant_ಕೋಷ್ಟಕ_free ();

  /* Reclaim Flex's buffers.  */
  yylex_destroy ();
}
