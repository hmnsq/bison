/* Bison Grammar Parser                             -*- C -*-

   Copyright (C) 2002-2015, 2018-2022 Free Software Foundation, Inc.

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

%code requires
{
  #ಅನ್ತರ್ಭಾವ "symlist.h"
  #ಅನ್ತರ್ಭಾವ "symtab.h"
}

%code provides
{
  /* Initialize unquote.  */
  ಶೂನ್ಯ parser_init (ಶೂನ್ಯ);
  /* Deallocate storage for unquote.  */
  ಶೂನ್ಯ parser_free (ಶೂನ್ಯ);
}

%code ತುದಿ_2
{
  /* On column 0 to please syntax-check.  */
#ಅನ್ತರ್ಭಾವ <config.h>
}

%code
{
  #ಅನ್ತರ್ಭಾವ "system.h"

  #ಅನ್ತರ್ಭಾವ <c-ctype.h>
  #ಅನ್ತರ್ಭಾವ <quotearg.h>
  #ಅನ್ತರ್ಭಾವ <vasnprintf.h>
  #ಅನ್ತರ್ಭಾವ <xmemdup0.h>

  #ಅನ್ತರ್ಭಾವ "complain.h"
  #ಅನ್ತರ್ಭಾವ "conflicts.h"
  #ಅನ್ತರ್ಭಾವ "files.h"
  #ಅನ್ತರ್ಭಾವ "getargs.h"
  #ಅನ್ತರ್ಭಾವ "gram.h"
  #ಅನ್ತರ್ಭಾವ "named-ref.h"
  #ಅನ್ತರ್ಭಾವ "reader.h"
  #ಅನ್ತರ್ಭಾವ "scan-code.h"
  #ಅನ್ತರ್ಭಾವ "scan-gram.h"
  #ಅನ್ತರ್ಭಾವ "strversion.h"

  /* Pretend to be at least that version, to check features published
     in that version while developping it.  */
  ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* api_ಆವೃತ್ತಿ = "3.8";

  ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ current_prec = 0;
  ಸ್ಥಿರ location current_lhs_loc;
  ಸ್ಥಿರ named_ref *current_lhs_named_ref;
  ಸ್ಥಿರ ಚಿಹ್ನೆ *current_lhs_ಚಿಹ್ನೆ;
  ಸ್ಥಿರ ಚಿಹ್ನೆ_class current_class = unknown_sym;

  /** Set the new current left-hand side symbol, possibly common
   * to several right-hand side parts of rule.
   */
  ಸ್ಥಿರ ಶೂನ್ಯ current_lhs (ಚಿಹ್ನೆ *sym, location loc, named_ref *ref);

  #ವ್ಯಾಖ್ಯಾನಿಸು YYLLOC_DEFAULT(Current, Rhs, N)         \
    (Current) = lloc_default (Rhs, N)
  ಸ್ಥಿರ YYLTYPE lloc_default (YYLTYPE ಸ್ಥಿರಾಂಕ *, ಪೂರ್ಣಾಂಕ);

  #ವ್ಯಾಖ್ಯಾನಿಸು YY_LOCATION_PRINT(File, Loc)            \
    location_print (Loc, File)

  /* Strip initial '{' and final '}' (must be first and last characters).
     Return the result.  */
  ಸ್ಥಿರ ಅಕ್ಷರ *strip_braces (ಅಕ್ಷರ *ಸಂಕೇತ_2);

  /* Convert CODE by calling code_props_plain_init if PLAIN, otherwise
     code_props_symbol_action_init.  Calls
     gram_scanner_last_string_free to release the latest string from
     the scanner (should be CODE). */
  ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *translate_ಸಂಕೇತ (ಅಕ್ಷರ *ಸಂಕೇತ_2, location loc, bool plain);

  /* Convert CODE by calling code_props_plain_init after having
     stripped the first and last characters (expected to be '{', and
     '}').  Calls gram_scanner_last_string_free to release the latest
     string from the scanner (should be CODE). */
  ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *translate_ಸಂಕೇತ_braceless (ಅಕ್ಷರ *ಸಂಕೇತ_2, location loc);

  /* Handle a %header directive.  */
  ಸ್ಥಿರ ಶೂನ್ಯ handle_ಶೀರ್ಷಿಕೆ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಮೌಲ್ಯ);

  /* Handle a %error-verbose directive.  */
  ಸ್ಥಿರ ಶೂನ್ಯ handle_error_verbose (location ಸ್ಥಿರಾಂಕ *loc, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *directive);

  /* Handle a %file-prefix directive.  */
  ಸ್ಥಿರ ಶೂನ್ಯ handle_file_ಪೂರ್ವಪ್ರತ್ಯಯ (location ಸ್ಥಿರಾಂಕ *loc,
                                  location ಸ್ಥಿರಾಂಕ *dir_loc,
                                  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *directive, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಮೌಲ್ಯ);

  /* Handle a %language directive.  */
  ಸ್ಥಿರ ಶೂನ್ಯ handle_language (location ಸ್ಥಿರಾಂಕ *loc, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *lang);

  /* Handle a %name-prefix directive.  */
  ಸ್ಥಿರ ಶೂನ್ಯ handle_name_ಪೂರ್ವಪ್ರತ್ಯಯ (location ಸ್ಥಿರಾಂಕ *loc,
                                  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *directive, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಮೌಲ್ಯ);

  /* Handle a %pure-parser directive.  */
  ಸ್ಥಿರ ಶೂನ್ಯ handle_ಶುದ್ಧ_parser (location ಸ್ಥಿರಾಂಕ *loc, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *directive);

  /* Handle a %require directive.  */
  ಸ್ಥಿರ ಶೂನ್ಯ handle_require (location ಸ್ಥಿರಾಂಕ *loc, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಆವೃತ್ತಿ);

  /* Handle a %skeleton directive.  */
  ಸ್ಥಿರ ಶೂನ್ಯ handle_skeleton (location ಸ್ಥಿರಾಂಕ *loc, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *skel);

  /* Handle a %yacc directive.  */
  ಸ್ಥಿರ ಶೂನ್ಯ handle_yacc (location ಸ್ಥಿರಾಂಕ *loc);

  /* Implementation of yyerror.  */
  ಸ್ಥಿರ ಶೂನ್ಯ gram_error (location ಸ್ಥಿರಾಂಕ *, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *);

  /* A string that describes a char (e.g., 'a' -> "'a'").  */
  ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *char_name (ಅಕ್ಷರ);

  /* Add style to semantic values in traces.  */
  ಸ್ಥಿರ ಶೂನ್ಯ tron (FILE *yyo);
  ಸ್ಥಿರ ಶೂನ್ಯ troff (FILE *yyo);

  /* Interpret a quoted string (such as `"Hello, \"World\"\n\""`).
     Manages the memory of the result.  */
  ಸ್ಥಿರ ಅಕ್ಷರ *unquote (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str);

  /* Discard the latest unquoted string.  */
  ಸ್ಥಿರ ಶೂನ್ಯ unquote_free (ಅಕ್ಷರ *ಕೊನೆಯ_string);
}

%define api.ಶೀರ್ಷಿಕೆ.include {"parse-gram.h"}
%define api.ಪೂರ್ವಪ್ರತ್ಯಯ {gram_}
%define api.ಶುದ್ಧ ತುಂಬಿದ
%define api.token.raw
%define api.ಮೌಲ್ಯ.ಪ್ರಕಾರ ಒಕ್ಕೂಟ
%define locations
%define parse.error custom
%define parse.lac ತುಂಬಿದ
%define parse.trace
%header
%expect 0
%verbose

%initial-action
{
  /* Bison's grammar can initial empty locations, hence a default
     location is needed. */
  boundary_ಗಣ (&@$.ಆರಂಭ, grammar_file, 1, 1, 1);
  boundary_ಗಣ (&@$.ಕೊನೆ, grammar_file, 1, 1, 1);
}

%token
  STRING              _("string")
  TSTRING             _("translatable string")

  PERCENT_TOKEN       "%token"
  PERCENT_NTERM       "%nterm"

  PERCENT_TYPE        "%type"
  PERCENT_DESTRUCTOR  "%destructor"
  PERCENT_PRINTER     "%printer"

  PERCENT_ಎಡ        "%left"
  PERCENT_ಬಲ       "%right"
  PERCENT_NONASSOC    "%nonassoc"
  PERCENT_PRECEDENCE  "%precedence"

  PERCENT_PREC        "%prec"
  PERCENT_DPREC       "%dprec"
  PERCENT_ವಿಲೀನ       "%merge"

  PERCENT_ಸಂಕೇತ            "%code"
  PERCENT_DEFAULT_PREC    "%default-prec"
  PERCENT_DEFINE          "%define"
  PERCENT_ERROR_VERBOSE   "%error-verbose"
  PERCENT_EXPECT          "%expect"
  PERCENT_EXPECT_RR       "%expect-rr"
  PERCENT_FILE_ಪೂರ್ವಪ್ರತ್ಯಯ     "%file-prefix"
  PERCENT_FLAG            "%<flag>"
  PERCENT_GLR_PARSER      "%glr-parser"
  PERCENT_ಶೀರ್ಷಿಕೆ          "%header"
  PERCENT_INITIAL_ACTION  "%initial-action"
  PERCENT_LANGUAGE        "%language"
  PERCENT_NAME_ಪೂರ್ವಪ್ರತ್ಯಯ     "%name-prefix"
  PERCENT_NO_DEFAULT_PREC "%no-default-prec"
  PERCENT_NO_ಸಾಲುಗಳು        "%no-lines"
  PERCENT_NONDETERMINISTIC_PARSER "%nondeterministic-parser"
  PERCENT_OUTPUT          "%output"
  PERCENT_ಶುದ್ಧ_PARSER     "%pure-parser"
  PERCENT_REQUIRE         "%require"
  PERCENT_SKELETON        "%skeleton"
  PERCENT_ಆರಂಭ           "%start"
  PERCENT_TOKEN_ಕೋಷ್ಟಕ     "%token-table"
  PERCENT_VERBOSE         "%verbose"
  PERCENT_YACC            "%yacc"

  BRACED_ಸಂಕೇತ       "{...}"
  BRACED_PREDICATE  "%?{...}"
  BRACKETED_ID      _("[identifier]")
  CHAR_LITERAL      _("character literal")
  COLON             ":"
  EPILOGUE          _("epilogue")
  EQUAL             "="
  ID                _("identifier")
  ID_COLON          _("identifier:")
  PERCENT_PERCENT   "%%"
  PIPE              "|"
  PROLOGUE          "%{...%}"
  SEMICOLON         ";"
  ಟ್ಯಾಗ್_2               _("<tag>")
  ಟ್ಯಾಗ್_ANY           "<*>"
  ಟ್ಯಾಗ್_NONE          "<>"

 /* Experimental feature, don't rely on it.  */
%code pre-printer  {tron (yyo);}
%code post-printer {troff (yyo);}

%ಪ್ರಕಾರ <ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ> CHAR_LITERAL
%printer { fputs (char_name ($$), yyo); } <ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ>

%ಪ್ರಕಾರ <ಅಕ್ಷರ*> "{...}" "%?{...}" "%{...%}" EPILOGUE STRING TSTRING
%printer { fputs ($$, yyo); } <ಅಕ್ಷರ*>

%ಪ್ರಕಾರ <uniqstr>
  BRACKETED_ID ID ID_COLON
  PERCENT_ERROR_VERBOSE PERCENT_FILE_ಪೂರ್ವಪ್ರತ್ಯಯ PERCENT_FLAG PERCENT_NAME_ಪೂರ್ವಪ್ರತ್ಯಯ
  PERCENT_ಶುದ್ಧ_PARSER
  ಟ್ಯಾಗ್_2 ಟ್ಯಾಗ್ ಟ್ಯಾಗ್.opt variable
%printer { fputs ($$, yyo); } <uniqstr>
%printer { fprintf (yyo, "[%s]", $$); } BRACKETED_ID
%printer { fprintf (yyo, "%s:", $$); } ID_COLON
%printer { fprintf (yyo, "%%%s", $$); } PERCENT_FLAG
%printer { fprintf (yyo, "<%s>", $$); } ಟ್ಯಾಗ್_2 ಟ್ಯಾಗ್

%token <ಪೂರ್ಣಾಂಕ> INT_LITERAL _("integer literal")
%printer { fprintf (yyo, "%d", $$); } <ಪೂರ್ಣಾಂಕ>

%ಪ್ರಕಾರ <ಚಿಹ್ನೆ*> id id_colon string_as_id ಚಿಹ್ನೆ token_decl token_decl_for_prec
%printer { fprintf (yyo, "%s", $$ ? $$->ಟ್ಯಾಗ್ : "<NULL>"); } <ಚಿಹ್ನೆ*>
%printer { fprintf (yyo, "%s:", $$->ಟ್ಯಾಗ್); } id_colon

%ಪ್ರಕಾರ <assoc> precedence_declarator

%destructor { ಚಿಹ್ನೆ_list_free ($$); } <ಚಿಹ್ನೆ_list*>
%printer { ಚಿಹ್ನೆ_list_syms_print ($$, yyo); } <ಚಿಹ್ನೆ_list*>

%ಪ್ರಕಾರ <named_ref*> named_ref.opt

/*---------.
| %param.  |
`---------*/
%code requires
{
  ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಎಣಿಕೆ
  {
    param_none   = 0,
    param_lex    = 1 << 0,
    param_parse  = 1 << 1,
    param_both   = param_lex | param_parse
  } param_type;
};
%code
{
  /** Add a lex-param and/or a parse-param.
   *
   * \param type  where to push this formal argument.
   * \param decl  the formal argument.  Destroyed.
   * \param loc   the location in the source.
   */
  ಸ್ಥಿರ ಶೂನ್ಯ ಕೂಡಿಸು_param (param_type ಪ್ರಕಾರ, ಅಕ್ಷರ *decl, location loc);
  ಸ್ಥಿರ param_type current_param = param_none;
};
%token <param_type> PERCENT_PARAM "%param";
%printer
{
  ಆಯ್ಕೆ ($$)
    {
#ವ್ಯಾಖ್ಯಾನಿಸು CASE(ಒಳ_2, ಹೊರಹೋಗು_2)                                           \
      ಸ್ಥಿತಿ param_ ## ಒಳ_2: fputs ("%" #Out, yyo); ನಿಲ್ಲಿಸು
      CASE (lex,   lex-param);
      CASE (parse, parse-param);
      CASE (both,  param);
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು CASE
      ಸ್ಥಿತಿ param_none: aver (false); ನಿಲ್ಲಿಸು;
    }
} <param_type>;


                     /*==========\
                     | Grammar.  |
                     \==========*/
%%

ಒಳಹರಿವು:
  prologue_declarations "%%" grammar epilogue.opt
;


        /*------------------------------------.
        | Declarations: before the first %%.  |
        `------------------------------------*/

prologue_declarations:
  %empty
| prologue_declarations prologue_declaration
;

prologue_declaration:
  grammar_declaration
| "%{...%}"
    {
      muscle_ಸಂಕೇತ_grow (union_ನೋಡಿದ ? "post_prologue" : "pre_prologue",
                        translate_ಸಂಕೇತ ($1, @1, true), @1);
      ಸಂಕೇತ_scanner_ಕೊನೆಯ_string_free ();
    }
| "%<flag>"
    {
      muscle_percent_define_ensure ($1, @1, true);
    }
| "%define" variable ಮೌಲ್ಯ
    {
      muscle_percent_define_ಸೇರಿಸು ($2, @$, $3.ಬಗೆ, $3.chars,
                                    MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
    }
| "%header" string.opt             { handle_ಶೀರ್ಷಿಕೆ ($2); }
| "%error-verbose"                 { handle_error_verbose (&@$, $1); }
| "%expect" INT_LITERAL            { expected_sr_conflicts = $2; }
| "%expect-rr" INT_LITERAL         { expected_rr_conflicts = $2; }
| "%file-prefix" STRING            { handle_file_ಪೂರ್ವಪ್ರತ್ಯಯ (&@$, &@1, $1, $2); }
| "%glr-parser"
    {
      nondeterministic_parser = true;
      glr_parser = true;
    }
| "%initial-action" "{...}"
    {
      muscle_ಸಂಕೇತ_grow ("initial_action", translate_ಸಂಕೇತ ($2, @2, false), @2);
      ಸಂಕೇತ_scanner_ಕೊನೆಯ_string_free ();
    }
| "%language" STRING            { handle_language (&@1, $2); }
| "%name-prefix" STRING         { handle_name_ಪೂರ್ವಪ್ರತ್ಯಯ (&@$, $1, $2); }
| "%no-lines"                   { no_ಸಾಲುಗಳು_flag = true; }
| "%nondeterministic-parser"    { nondeterministic_parser = true; }
| "%output" STRING              { spec_outfile = unquote ($2); gram_scanner_ಕೊನೆಯ_string_free (); }
| "%param" { current_param = $1; } params { current_param = param_none; }
| "%pure-parser"                { handle_ಶುದ್ಧ_parser (&@$, $1); }
| "%require" STRING             { handle_require (&@2, $2); }
| "%skeleton" STRING            { handle_skeleton (&@2, $2); }
| "%token-table"                { token_ಕೋಷ್ಟಕ_flag = true; }
| "%verbose"                    { report_flag |= report_ಸ್ಥಿತಿಗಳು; }
| "%yacc"                       { handle_yacc (&@$); }
| error ";"                     { current_class = unknown_sym; yyerrok; }
| /*FIXME: Err?  What is this horror doing here? */ ";"
;

params:
   params "{...}"  { ಕೂಡಿಸು_param (current_param, $2, @2); }
| "{...}"          { ಕೂಡಿಸು_param (current_param, $1, @1); }
;


/*----------------------.
| grammar_declaration.  |
`----------------------*/

grammar_declaration:
  ಚಿಹ್ನೆ_declaration
| "%start" symbols.1
    {
      grammar_ಆರಂಭ_symbols_ಕೂಡಿಸು ($2);
    }
| ಸಂಕೇತ_props_type "{...}" generic_symlist
    {
      ಸಂಕೇತ_props ಸಂಕೇತ_2;
      ಸಂಕೇತ_props_ಚಿಹ್ನೆ_action_init (&ಸಂಕೇತ_2, $2, @2);
      ಸಂಕೇತ_props_translate_ಸಂಕೇತ (&ಸಂಕೇತ_2);
      {
        ಗಾಗಿ (ಚಿಹ್ನೆ_list *list = $3; list; list = list->ಮುಂದಿನ)
          ಚಿಹ್ನೆ_list_ಸಂಕೇತ_props_ಗಣ (list, $1, &ಸಂಕೇತ_2);
        ಚಿಹ್ನೆ_list_free ($3);
      }
    }
| "%default-prec"
    {
      default_prec = true;
    }
| "%no-default-prec"
    {
      default_prec = false;
    }
| "%code" "{...}"
    {
      /* Do not invoke muscle_percent_code_grow here since it invokes
         muscle_user_name_list_grow.  */
      muscle_ಸಂಕೇತ_grow ("percent_code()",
                        translate_ಸಂಕೇತ_braceless ($2, @2), @2);
      ಸಂಕೇತ_scanner_ಕೊನೆಯ_string_free ();
    }
| "%code" ID "{...}"
    {
      muscle_percent_ಸಂಕೇತ_grow ($2, @2, translate_ಸಂಕೇತ_braceless ($3, @3), @3);
      ಸಂಕೇತ_scanner_ಕೊನೆಯ_string_free ();
    }
;

%ಪ್ರಕಾರ <ಸಂಕೇತ_props_type> ಸಂಕೇತ_props_type;
%printer { fprintf (yyo, "%s", ಸಂಕೇತ_props_type_string ($$)); } <ಸಂಕೇತ_props_type>;
ಸಂಕೇತ_props_type:
  "%destructor"  { $$ = destructor; }
| "%printer"     { $$ = printer; }
;

/*---------.
| %union.  |
`---------*/

%token PERCENT_UNION "%union";

union_name:
  %empty {}
| ID     { muscle_percent_define_ಸೇರಿಸು ("api.value.union.name",
                                         @1, muscle_keyword, $1,
                                         MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE); }
;

grammar_declaration:
  "%union" union_name "{...}"
    {
      union_ನೋಡಿದ = true;
      muscle_ಸಂಕೇತ_grow ("union_members", translate_ಸಂಕೇತ_braceless ($3, @3), @3);
      ಸಂಕೇತ_scanner_ಕೊನೆಯ_string_free ();
    }
;


%ಪ್ರಕಾರ <ಚಿಹ್ನೆ_list*> nterm_decls ಚಿಹ್ನೆ_decls symbols.1
      token_decls token_decls_for_prec
      token_decl.1 token_decl_for_prec.1;
ಚಿಹ್ನೆ_declaration:
  "%nterm" { current_class = nterm_sym; } nterm_decls[syms]
    {
      current_class = unknown_sym;
      ಚಿಹ್ನೆ_list_free ($syms);
    }
| "%token" { current_class = token_sym; } token_decls[syms]
    {
      current_class = unknown_sym;
      ಚಿಹ್ನೆ_list_free ($syms);
    }
| "%type" { current_class = pct_type_sym; } ಚಿಹ್ನೆ_decls[syms]
    {
      current_class = unknown_sym;
      ಚಿಹ್ನೆ_list_free ($syms);
    }
| precedence_declarator token_decls_for_prec[syms]
    {
      ++current_prec;
      ಗಾಗಿ (ಚಿಹ್ನೆ_list *list = $syms; list; list = list->ಮುಂದಿನ)
        ಚಿಹ್ನೆ_precedence_ಗಣ (list->content.sym, current_prec, $1, @1);
      ಚಿಹ್ನೆ_list_free ($syms);
    }
;

precedence_declarator:
  "%left"       { $$ = ಎಡ_assoc; }
| "%right"      { $$ = ಬಲ_assoc; }
| "%nonassoc"   { $$ = non_assoc; }
| "%precedence" { $$ = precedence_assoc; }
;

%ಪ್ರಕಾರ <ಅಕ್ಷರ*> string.opt;
string.opt:
  %empty  { $$ = ಖಾಲಿ; }
| STRING  { $$ = $1; }
;

ಟ್ಯಾಗ್.opt:
  %empty { $$ = ಖಾಲಿ; }
| ಟ್ಯಾಗ್_2    { $$ = $1; }
;

%ಪ್ರಕಾರ <ಚಿಹ್ನೆ_list*> generic_symlist generic_symlist_ಅಂಶ;
generic_symlist:
  generic_symlist_ಅಂಶ
| generic_symlist generic_symlist_ಅಂಶ   { $$ = ಚಿಹ್ನೆ_list_append ($1, $2); }
;

generic_symlist_ಅಂಶ:
  ಚಿಹ್ನೆ    { $$ = ಚಿಹ್ನೆ_list_sym_new ($1, @1); }
| ಟ್ಯಾಗ್       { $$ = ಚಿಹ್ನೆ_list_type_new ($1, @1); }
;

ಟ್ಯಾಗ್:
  ಟ್ಯಾಗ್_2
| "<*>" { $$ = uniqstr_new ("*"); }
| "<>"  { $$ = uniqstr_new (""); }
;

/*-----------------------.
| nterm_decls (%nterm).  |
`-----------------------*/

// A non empty list of possibly tagged symbols for %nterm.
//
// Can easily be defined like symbol_decls but restricted to ID, but
// using token_decls allows to reduce the number of rules, and also to
// make nicer error messages on "%nterm 'a'" or '%nterm FOO "foo"'.
nterm_decls:
  token_decls
;

/*-----------------------------------.
| token_decls (%token, and %nterm).  |
`-----------------------------------*/

// A non empty list of possibly tagged symbols for %token or %nterm.
token_decls:
  token_decl.1[syms]
    {
      $$ = $syms;
    }
| ಟ್ಯಾಗ್_2 token_decl.1[syms]
    {
      $$ = ಚಿಹ್ನೆ_list_type_ಗಣ ($syms, $ಟ್ಯಾಗ್_2);
    }
| token_decls ಟ್ಯಾಗ್_2 token_decl.1[syms]
    {
      $$ = ಚಿಹ್ನೆ_list_append ($1, ಚಿಹ್ನೆ_list_type_ಗಣ ($syms, $ಟ್ಯಾಗ್_2));
    }
;

// One or more symbol declarations for %token or %nterm.
token_decl.1:
  token_decl                { $$ = ಚಿಹ್ನೆ_list_sym_new ($1, @1); }
| token_decl.1 token_decl   { $$ = ಚಿಹ್ನೆ_list_append ($1, ಚಿಹ್ನೆ_list_sym_new ($2, @2)); }

// One symbol declaration for %token or %nterm.
token_decl:
  id ಪೂರ್ಣಾಂಕ.opt[num] alias
    {
      $$ = $id;
      ಚಿಹ್ನೆ_class_ಗಣ ($id, current_class, @id, true);
      ಆದರೆ (0 <= $ಅಂಕೆ)
        ಚಿಹ್ನೆ_ಸಂಕೇತ_ಗಣ ($id, $ಅಂಕೆ, @ಅಂಕೆ);
      ಆದರೆ ($alias)
        ಚಿಹ್ನೆ_make_alias ($id, $alias, @alias);
    }
;

%ಪ್ರಕಾರ <ಪೂರ್ಣಾಂಕ> ಪೂರ್ಣಾಂಕ.opt;
ಪೂರ್ಣಾಂಕ.opt:
  %empty  { $$ = -1; }
| INT_LITERAL
;

%ಪ್ರಕಾರ <ಚಿಹ್ನೆ*> alias;
alias:
  %empty         { $$ = ಖಾಲಿ; }
| string_as_id   { $$ = $1; }
| TSTRING
    {
      $$ = ಚಿಹ್ನೆ_get ($1, @1);
      ಚಿಹ್ನೆ_class_ಗಣ ($$, token_sym, @1, false);
      $$->translatable = true;
    }
;


/*-------------------------------------.
| token_decls_for_prec (%left, etc.).  |
`-------------------------------------*/

// A non empty list of possibly tagged tokens for precedence declaration.
//
// Similar to %token (token_decls), but in '%left FOO 1 "foo"', it treats
// FOO and "foo" as two different symbols instead of aliasing them.
token_decls_for_prec:
  token_decl_for_prec.1[syms]
    {
      $$ = $syms;
    }
| ಟ್ಯಾಗ್_2 token_decl_for_prec.1[syms]
    {
      $$ = ಚಿಹ್ನೆ_list_type_ಗಣ ($syms, $ಟ್ಯಾಗ್_2);
    }
| token_decls_for_prec ಟ್ಯಾಗ್_2 token_decl_for_prec.1[syms]
    {
      $$ = ಚಿಹ್ನೆ_list_append ($1, ಚಿಹ್ನೆ_list_type_ಗಣ ($syms, $ಟ್ಯಾಗ್_2));
    }
;

// One or more token declarations for precedence declaration.
token_decl_for_prec.1:
  token_decl_for_prec
    { $$ = ಚಿಹ್ನೆ_list_sym_new ($1, @1); }
| token_decl_for_prec.1 token_decl_for_prec
    { $$ = ಚಿಹ್ನೆ_list_append ($1, ಚಿಹ್ನೆ_list_sym_new ($2, @2)); }

// One token declaration for precedence declaration.
token_decl_for_prec:
  id ಪೂರ್ಣಾಂಕ.opt[num]
    {
      $$ = $id;
      ಚಿಹ್ನೆ_class_ಗಣ ($id, token_sym, @id, false);
      ಆದರೆ (0 <= $ಅಂಕೆ)
        ಚಿಹ್ನೆ_ಸಂಕೇತ_ಗಣ ($id, $ಅಂಕೆ, @ಅಂಕೆ);
    }
| string_as_id
;


/*-----------------------------------.
| symbol_decls (argument of %type).  |
`-----------------------------------*/

// A non empty list of typed symbols (for %type).
ಚಿಹ್ನೆ_decls:
  symbols.1[syms]
    {
      $$ = $syms;
    }
| ಟ್ಯಾಗ್_2 symbols.1[syms]
    {
      $$ = ಚಿಹ್ನೆ_list_type_ಗಣ ($syms, $ಟ್ಯಾಗ್_2);
    }
| ಚಿಹ್ನೆ_decls ಟ್ಯಾಗ್_2 symbols.1[syms]
    {
      $$ = ಚಿಹ್ನೆ_list_append ($1, ಚಿಹ್ನೆ_list_type_ಗಣ ($syms, $ಟ್ಯಾಗ್_2));
    }
;

// One or more symbols.
symbols.1:
  ಚಿಹ್ನೆ
    {
      ಆದರೆ (current_class != unknown_sym)
        ಚಿಹ್ನೆ_class_ಗಣ ($ಚಿಹ್ನೆ, current_class, @ಚಿಹ್ನೆ, false);
      $$ = ಚಿಹ್ನೆ_list_sym_new ($ಚಿಹ್ನೆ, @ಚಿಹ್ನೆ);
    }
  | symbols.1 ಚಿಹ್ನೆ
    {
      ಆದರೆ (current_class != unknown_sym)
        ಚಿಹ್ನೆ_class_ಗಣ ($ಚಿಹ್ನೆ, current_class, @ಚಿಹ್ನೆ, false);
      $$ = ಚಿಹ್ನೆ_list_append ($1, ಚಿಹ್ನೆ_list_sym_new ($ಚಿಹ್ನೆ, @ಚಿಹ್ನೆ));
    }
;

        /*------------------------------------------.
        | The grammar section: between the two %%.  |
        `------------------------------------------*/

grammar:
  ನಿಯಮಗಳು_or_grammar_declaration
| grammar ನಿಯಮಗಳು_or_grammar_declaration
;

/* As a Bison extension, one can use the grammar declarations in the
   body of the grammar.  */
ನಿಯಮಗಳು_or_grammar_declaration:
  ನಿಯಮಗಳು
| grammar_declaration ";"
| error ";"
    {
      yyerrok;
    }
;

ನಿಯಮಗಳು:
  id_colon named_ref.opt { current_lhs ($1, @1, $2); } ":" rhses.1
    {
      /* Free the current lhs. */
      current_lhs (0, @1, 0);
    }
;

rhses.1:
  rhs                { grammar_current_ನಿಯಮ_ಕೊನೆ (@rhs); }
| rhses.1 "|" rhs    { grammar_current_ನಿಯಮ_ಕೊನೆ (@rhs); }
| rhses.1 ";"
;

%token PERCENT_ಖಾಲಿಗಳು "%empty";
rhs:
  %empty
    { grammar_current_ನಿಯಮ_begin (current_lhs_ಚಿಹ್ನೆ, current_lhs_loc,
                                  current_lhs_named_ref); }
| rhs ಚಿಹ್ನೆ named_ref.opt
    { grammar_current_ನಿಯಮ_ಚಿಹ್ನೆ_append ($2, @2, $3); }
| rhs ಟ್ಯಾಗ್.opt "{...}"[action] named_ref.opt[name]
    { grammar_current_ನಿಯಮ_action_append ($action, @action, $name, $[tag.opt]); }
| rhs "%?{...}"
    { grammar_current_ನಿಯಮ_predicate_append ($2, @2); }
| rhs "%empty"
    { grammar_current_ನಿಯಮ_ಖಾಲಿಗಳು_ಗಣ (@2); }
| rhs "%prec" ಚಿಹ್ನೆ
    { grammar_current_ನಿಯಮ_prec_ಗಣ ($3, @3); }
| rhs "%dprec" INT_LITERAL
    { grammar_current_ನಿಯಮ_dprec_ಗಣ ($3, @3); }
| rhs "%merge" ಟ್ಯಾಗ್_2
    { grammar_current_ನಿಯಮ_ವಿಲೀನ_ಗಣ ($3, @3); }
| rhs "%expect" INT_LITERAL
    { grammar_current_ನಿಯಮ_expect_sr ($3, @3); }
| rhs "%expect-rr" INT_LITERAL
    { grammar_current_ನಿಯಮ_expect_rr ($3, @3); }
;

named_ref.opt:
  %empty         { $$ = ಖಾಲಿ; }
| BRACKETED_ID   { $$ = named_ref_new ($1, @1); }
;


/*---------------------.
| variable and value.  |
`---------------------*/

variable:
  ID
;

/* Some content or empty by default. */
%code requires {
  #ಅನ್ತರ್ಭಾವ "muscle-tab.h"
  ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ
  {
    ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *chars;
    muscle_ಬಗೆ ಬಗೆ;
  } ಮೌಲ್ಯ_type;
};
%ಪ್ರಕಾರ <ಮೌಲ್ಯ_type> ಮೌಲ್ಯ;
%printer
{
  ಆಯ್ಕೆ ($$.ಬಗೆ)
    {
    ಸ್ಥಿತಿ muscle_ಸಂಕೇತ:    fprintf (yyo,  "{%s}",  $$.chars); ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ muscle_keyword: fprintf (yyo,   "%s",   $$.chars); ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ muscle_string:  fprintf (yyo, "\"%s\"", $$.chars); ನಿಲ್ಲಿಸು;
    }
} <ಮೌಲ್ಯ_type>;

ಮೌಲ್ಯ:
  %empty  { $$.ಬಗೆ = muscle_keyword; $$.chars = ""; }
| ID      { $$.ಬಗೆ = muscle_keyword; $$.chars = $1; }
| STRING  { $$.ಬಗೆ = muscle_string;  $$.chars = unquote ($1); gram_scanner_ಕೊನೆಯ_string_free ();}
| "{...}" { $$.ಬಗೆ = muscle_ಸಂಕೇತ;    $$.chars = strip_braces ($1); gram_scanner_ಕೊನೆಯ_string_free (); }
;


/*--------------.
| Identifiers.  |
`--------------*/

/* Identifiers are returned as uniqstr values by the scanner.
   Depending on their use, we may need to make them genuine symbols.  */

id:
  ID
    { $$ = ಚಿಹ್ನೆ_from_uniqstr ($1, @1); }
| CHAR_LITERAL
    {
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *var = "api.token.raw";
      ಆದರೆ (current_class == nterm_sym)
        {
          complain (&@1, complaint,
                    _("character literals cannot be nonterminals"));
          YYERROR;
        }
      ಆದರೆ (muscle_percent_define_ifdef (var))
        {
          complain (&@1, complaint,
                    _("character literals cannot be used together"
                    " with %s"), var);
          location loc = muscle_percent_define_get_loc (var);
          subcomplain (&loc, complaint, _("definition of %s"), var);
        }
      $$ = ಚಿಹ್ನೆ_get (char_name ($1), @1);
      ಚಿಹ್ನೆ_class_ಗಣ ($$, token_sym, @1, false);
      ಚಿಹ್ನೆ_ಸಂಕೇತ_ಗಣ ($$, $1, @1);
    }
;

id_colon:
  ID_COLON { $$ = ಚಿಹ್ನೆ_from_uniqstr ($1, @1); }
;


ಚಿಹ್ನೆ:
  id
| string_as_id
;

/* A string used as an ID.  */
string_as_id:
  STRING
    {
      $$ = ಚಿಹ್ನೆ_get ($1, @1);
      ಚಿಹ್ನೆ_class_ಗಣ ($$, token_sym, @1, false);
    }
;

epilogue.opt:
  %empty
| "%%" EPILOGUE
    {
      muscle_ಸಂಕೇತ_grow ("epilogue", translate_ಸಂಕೇತ ($2, @2, true), @2);
      ಸಂಕೇತ_scanner_ಕೊನೆಯ_string_free ();
    }
;

%%

ಪೂರ್ಣಾಂಕ
yyreport_syntax_error (ಸ್ಥಿರಾಂಕ yypcontext_t *ctx)
{
  ಪೂರ್ಣಾಂಕ res = 0;
  /* Arguments of format: reported tokens (one for the "unexpected",
     one per "expected"). */
  ಎಣಿಕೆ { ವಾದಗಳು_MAX = 5 };
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *argv[ARGS_MAX];
  ಪೂರ್ಣಾಂಕ argc = 0;
  yysymbol_ಬಗೆ_t unexpected = yypcontext_token (ctx);
  ಆದರೆ (unexpected != YYSYMBOL_YYEMPTY)
    {
      argv[argc++] = yysymbol_name (unexpected);
      yysymbol_ಬಗೆ_t expected[ARGS_MAX - 1];
      ಪೂರ್ಣಾಂಕ nexpected = yypcontext_expected_ಟೋಕನುಗಳು (ctx, expected, ವಾದಗಳು_MAX - 1);
      ಆದರೆ (nexpected < 0)
        res = nexpected;
      ಇಲ್ಲದಿದ್ದರೆ
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < nexpected; ++i)
          argv[argc++] = yysymbol_name (expected[i]);
    }
  syntax_error (*yypcontext_location (ctx), argc, argv);
  ಹಿಂತಿರುಗಿ res;
}


/* Return the location of the left-hand side of a rule whose
   right-hand side is RHS[1] ... RHS[N].  Ignore empty nonterminals in
   the right-hand side, and return an empty location equal to the end
   boundary of RHS[0] if the right-hand side is empty.  */

ಸ್ಥಿರ YYLTYPE
lloc_default (YYLTYPE ಸ್ಥಿರಾಂಕ *rhs, ಪೂರ್ಣಾಂಕ n)
{
  YYLTYPE loc;

  /* SGI MIPSpro 7.4.1m miscompiles "loc.start = loc.end = rhs[n].end;".
     The bug is fixed in 7.4.2m, but play it safe for now.  */
  loc.ಆರಂಭ = rhs[n].ಕೊನೆ;
  loc.ಕೊನೆ = rhs[n].ಕೊನೆ;

  /* Ignore empty nonterminals the start of the right-hand side.
     Do not bother to ignore them at the end of the right-hand side,
     since empty nonterminals have the same end as their predecessors.  */
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 1; i <= n; i++)
    ಆದರೆ (! equal_boundaries (rhs[i].ಆರಂಭ, rhs[i].ಕೊನೆ))
      {
        loc.ಆರಂಭ = rhs[i].ಆರಂಭ;
        ನಿಲ್ಲಿಸು;
      }

  ಹಿಂತಿರುಗಿ loc;
}

ಸ್ಥಿರ
ಅಕ್ಷರ *strip_braces (ಅಕ್ಷರ *ಸಂಕೇತ_2)
{
  ಸಂಕೇತ_2[ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ಸಂಕೇತ_2) - 1] = 0;
  ಹಿಂತಿರುಗಿ ಸಂಕೇತ_2 + 1;
}

ಸ್ಥಿರ
ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *
translate_ಸಂಕೇತ (ಅಕ್ಷರ *ಸಂಕೇತ_2, location loc, bool plain)
{
  ಸಂಕೇತ_props plain_ಸಂಕೇತ;
  ಆದರೆ (plain)
    ಸಂಕೇತ_props_plain_init (&plain_ಸಂಕೇತ, ಸಂಕೇತ_2, loc);
  ಇಲ್ಲದಿದ್ದರೆ
    ಸಂಕೇತ_props_ಚಿಹ್ನೆ_action_init (&plain_ಸಂಕೇತ, ಸಂಕೇತ_2, loc);
  ಸಂಕೇತ_props_translate_ಸಂಕೇತ (&plain_ಸಂಕೇತ);
  gram_scanner_ಕೊನೆಯ_string_free ();
  ಹಿಂತಿರುಗಿ plain_ಸಂಕೇತ.ಸಂಕೇತ_2;
}

ಸ್ಥಿರ
ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *
translate_ಸಂಕೇತ_braceless (ಅಕ್ಷರ *ಸಂಕೇತ_2, location loc)
{
  ಹಿಂತಿರುಗಿ translate_ಸಂಕೇತ (strip_braces (ಸಂಕೇತ_2), loc, true);
}

ಸ್ಥಿರ ಶೂನ್ಯ
ಕೂಡಿಸು_param (param_type ಪ್ರಕಾರ, ಅಕ್ಷರ *decl, location loc)
{
  ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ alphanum[26 + 26 + 1 + 10 + 1] =
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "_"
    "0123456789";

  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name_ಆರಂಭ = ಖಾಲಿ;
  {
    ಅಕ್ಷರ *p;
    /* Stop on last actual character.  */
    ಗಾಗಿ (p = decl; p[1]; p++)
      ಆದರೆ ((p == decl
           || ! memchr (alphanum, p[-1], ಗಾತ್ರ alphanum - 1))
          && memchr (alphanum, p[0], ಗಾತ್ರ alphanum - 10 - 1))
        name_ಆರಂಭ = p;

    /* Strip the surrounding '{' and '}', and any blanks just inside
       the braces.  */
    --p;
    ಆಗ (c_isspace ((ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ) *p))
      --p;
    p[1] = '\0';
    ++decl;
    ಆಗ (c_isspace ((ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ) *decl))
      ++decl;
  }

  ಆದರೆ (! name_ಆರಂಭ)
    complain (&loc, complaint, _("missing identifier in parameter declaration"));
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಅಕ್ಷರ *name = xmemdup0 (name_ಆರಂಭ, strspn (name_ಆರಂಭ, alphanum));
      ಆದರೆ (ಪ್ರಕಾರ & param_lex)
        muscle_pair_list_grow ("lex_param", decl, name);
      ಆದರೆ (ಪ್ರಕಾರ & param_parse)
        muscle_pair_list_grow ("parse_param", decl, name);
      ಬಿಡುಗಡೆ (name);
    }

  gram_scanner_ಕೊನೆಯ_string_free ();
}


ಸ್ಥಿರ ಶೂನ್ಯ
handle_ಶೀರ್ಷಿಕೆ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಮೌಲ್ಯ)
{
  ಶೀರ್ಷಿಕೆ_flag = true;
  ಆದರೆ (ಮೌಲ್ಯ)
    {
      ಅಕ್ಷರ *file = unquote (ಮೌಲ್ಯ);
      spec_ಶೀರ್ಷಿಕೆ_file = xstrdup (file);
      gram_scanner_ಕೊನೆಯ_string_free ();
      unquote_free (file);
    }
}


ಸ್ಥಿರ ಶೂನ್ಯ
handle_error_verbose (location ಸ್ಥಿರಾಂಕ *loc, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *directive)
{
  bison_directive (loc, directive);
  muscle_percent_define_ಸೇರಿಸು (directive, *loc, muscle_keyword, "",
                                MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
}


ಸ್ಥಿರ ಶೂನ್ಯ
handle_file_ಪೂರ್ವಪ್ರತ್ಯಯ (location ಸ್ಥಿರಾಂಕ *loc,
                    location ಸ್ಥಿರಾಂಕ *dir_loc,
                    ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *directive, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಮೌಲ್ಯ_quoted)
{
  ಅಕ್ಷರ *ಮೌಲ್ಯ = unquote (ಮೌಲ್ಯ_quoted);
  bison_directive (loc, directive);
  bool warned = false;

  ಆದರೆ (location_ಖಾಲಿಗಳು (spec_file_ಪೂರ್ವಪ್ರತ್ಯಯ_loc))
    {
      spec_file_ಪೂರ್ವಪ್ರತ್ಯಯ_loc = *loc;
      spec_file_ಪೂರ್ವಪ್ರತ್ಯಯ = ಮೌಲ್ಯ;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      duplicate_directive (directive, spec_file_ಪೂರ್ವಪ್ರತ್ಯಯ_loc, *loc);
      warned = true;
    }

  ಆದರೆ (!warned
      && STRNEQ (directive, "%file-prefix"))
    deprecated_directive (dir_loc, directive, "%file-prefix");
}

ಸ್ಥಿರ ಶೂನ್ಯ
handle_language (location ಸ್ಥಿರಾಂಕ *loc, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *lang)
{
  language_argmatch (unquote (lang), grammar_prio, *loc);
}


ಸ್ಥಿರ ಶೂನ್ಯ
handle_name_ಪೂರ್ವಪ್ರತ್ಯಯ (location ಸ್ಥಿರಾಂಕ *loc,
                    ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *directive, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಮೌಲ್ಯ_quoted)
{
  ಅಕ್ಷರ *ಮೌಲ್ಯ = unquote (ಮೌಲ್ಯ_quoted);
  bison_directive (loc, directive);

  ಅಕ್ಷರ buf1[1024];
  size_t len1 = ಗಾತ್ರ (buf1);
  ಅಕ್ಷರ *old = asnprintf (buf1, &len1, "%s\"%s\"", directive, ಮೌಲ್ಯ);
  ಆದರೆ (!old)
    xalloc_die ();

  ಆದರೆ (location_ಖಾಲಿಗಳು (spec_name_ಪೂರ್ವಪ್ರತ್ಯಯ_loc))
    {
      spec_name_ಪೂರ್ವಪ್ರತ್ಯಯ = ಮೌಲ್ಯ;
      spec_name_ಪೂರ್ವಪ್ರತ್ಯಯ_loc = *loc;

      ಅಕ್ಷರ buf2[1024];
      size_t len2 = ಗಾತ್ರ (buf2);
      ಅಕ್ಷರ *new = asnprintf (buf2, &len2, "%%define api.prefix {%s}", ಮೌಲ್ಯ);
      ಆದರೆ (!new)
        xalloc_die ();
      deprecated_directive (loc, old, new);
      ಆದರೆ (new != buf2)
        ಬಿಡುಗಡೆ (new);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    duplicate_directive (old, spec_file_ಪೂರ್ವಪ್ರತ್ಯಯ_loc, *loc);

  ಆದರೆ (old != buf1)
    ಬಿಡುಗಡೆ (old);
}


ಸ್ಥಿರ ಶೂನ್ಯ
handle_ಶುದ್ಧ_parser (location ಸ್ಥಿರಾಂಕ *loc, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *directive)
{
  bison_directive (loc, directive);
  deprecated_directive (loc, directive, "%define api.pure");
  muscle_percent_define_ಸೇರಿಸು ("api.pure", *loc, muscle_keyword, "",
                                MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
}


ಸ್ಥಿರ ಶೂನ್ಯ
handle_require (location ಸ್ಥಿರಾಂಕ *loc, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಆವೃತ್ತಿ_quoted)
{
  ಅಕ್ಷರ *ಆವೃತ್ತಿ = unquote (ಆವೃತ್ತಿ_quoted);
  required_ಆವೃತ್ತಿ = strversion_ಗಮ್ಯ_int (ಆವೃತ್ತಿ);
  ಆದರೆ (required_ಆವೃತ್ತಿ == -1)
    {
      complain (loc, complaint, _("invalid version requirement: %s"),
                ಆವೃತ್ತಿ);
      required_ಆವೃತ್ತಿ = 0;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* package_ಆವೃತ್ತಿ =
        0 < strverscmp (api_ಆವೃತ್ತಿ, PACKAGE_ಆವೃತ್ತಿ)
        ? api_ಆವೃತ್ತಿ : PACKAGE_ಆವೃತ್ತಿ;
      ಆದರೆ (0 < strverscmp (ಆವೃತ್ತಿ, package_ಆವೃತ್ತಿ))
        {
          complain (loc, complaint, _("require bison %s, but have %s"),
                    ಆವೃತ್ತಿ, package_ಆವೃತ್ತಿ);
          ನಿರ್ಗಮಿಸು (EX_MISMATCH);
        }
    }
  unquote_free (ಆವೃತ್ತಿ);
  gram_scanner_ಕೊನೆಯ_string_free ();
}

ಸ್ಥಿರ ಶೂನ್ಯ
handle_skeleton (location ಸ್ಥಿರಾಂಕ *loc, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *skel_quoted)
{
  ಅಕ್ಷರ *skel = unquote (skel_quoted);
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *skeleton_user = skel;
  ಆದರೆ (strchr (skeleton_user, '/'))
    {
      size_t dir_ಉದ್ದಳತೆ = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (grammar_file);
      ಆಗ (dir_ಉದ್ದಳತೆ && grammar_file[dir_length - 1] != '/')
        --dir_length;
      ಆಗ (dir_ಉದ್ದಳತೆ && grammar_file[dir_length - 1] == '/')
        --dir_length;
      ಅಕ್ಷರ *skeleton_build =
        xmalloc (dir_ಉದ್ದಳತೆ + 1 + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (skeleton_user) + 1);
      ಆದರೆ (dir_ಉದ್ದಳತೆ > 0)
        {
          ಸ್ಮರಣೆ_ನಕಲು (skeleton_build, grammar_file, dir_ಉದ್ದಳತೆ);
          skeleton_build[dir_length++] = '/';
        }
      strcpy (skeleton_build + dir_ಉದ್ದಳತೆ, skeleton_user);
      skeleton_user = uniqstr_new (skeleton_build);
      ಬಿಡುಗಡೆ (skeleton_build);
    }
  skeleton_arg (skeleton_user, grammar_prio, *loc);
}


ಸ್ಥಿರ ಶೂನ್ಯ
handle_yacc (location ಸ್ಥಿರಾಂಕ *loc)
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *directive = "%yacc";
  bison_directive (loc, directive);
  ಆದರೆ (location_ಖಾಲಿಗಳು (yacc_loc))
    ಗಣ_yacc (*loc);
  ಇಲ್ಲದಿದ್ದರೆ
    duplicate_directive (directive, yacc_loc, *loc);
}


ಸ್ಥಿರ ಶೂನ್ಯ
gram_error (location ಸ್ಥಿರಾಂಕ *loc, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸಂದೇಶ_2)
{
  complain (loc, complaint, "%s", ಸಂದೇಶ_2);
}

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *
char_name (ಅಕ್ಷರ c)
{
  ಆದರೆ (c == '\'')
    ಹಿಂತಿರುಗಿ "'\\''";
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಅಕ್ಷರ buf[4];
      buf[0] = '\''; buf[1] = c; buf[2] = '\''; buf[3] = '\0';
      ಹಿಂತಿರುಗಿ quotearg_style (escape_quoting_style, buf);
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
current_lhs (ಚಿಹ್ನೆ *sym, location loc, named_ref *ref)
{
  current_lhs_ಚಿಹ್ನೆ = sym;
  current_lhs_loc = loc;
  ಆದರೆ (sym)
    ಚಿಹ್ನೆ_location_as_lhs_ಗಣ (sym, loc);
  /* In order to simplify memory management, named references for lhs
     are always assigned by deep copy into the current symbol_list
     node.  This is because a single named-ref in the grammar may
     result in several uses when the user factors lhs between several
     rules using "|".  Therefore free the parser's original copy.  */
  ಬಿಡುಗಡೆ (current_lhs_named_ref);
  current_lhs_named_ref = ref;
}

ಸ್ಥಿರ ಶೂನ್ಯ tron (FILE *yyo)
{
  begin_use_class ("value", yyo);
}

ಸ್ಥಿರ ಶೂನ್ಯ troff (FILE *yyo)
{
  ಕೊನೆ_use_class ("value", yyo);
}


/*----------.
| Unquote.  |
`----------*/

ರಚನೆ obstack obstack_for_unquote;

ಶೂನ್ಯ
parser_init (ಶೂನ್ಯ)
{
  obstack_init (&obstack_for_unquote);
}

ಶೂನ್ಯ
parser_free (ಶೂನ್ಯ)
{
  obstack_free (&obstack_for_unquote, 0);
}

ಸ್ಥಿರ ಶೂನ್ಯ
unquote_free (ಅಕ್ಷರ *ಕೊನೆಯ_string)
{
  obstack_free (&obstack_for_unquote, ಕೊನೆಯ_string);
}

ಸ್ಥಿರ ಅಕ್ಷರ *
unquote (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cp)
{
#ವ್ಯಾಖ್ಯಾನಿಸು GROW(Char)                              \
  obstack_1grow (&obstack_for_unquote, Char);
  ಗಾಗಿ (++cp; *cp && *cp != '"'; ++cp)
    ಆಯ್ಕೆ (*cp)
      {
      ಸ್ಥಿತಿ '"':
        ನಿಲ್ಲಿಸು;
      ಸ್ಥಿತಿ '\\':
        ++cp;
        ಆಯ್ಕೆ (*cp)
          {
          ಸ್ಥಿತಿ '0': ಸ್ಥಿತಿ '1': ಸ್ಥಿತಿ '2': ಸ್ಥಿತಿ '3': ಸ್ಥಿತಿ '4':
          ಸ್ಥಿತಿ '5': ಸ್ಥಿತಿ '6': ಸ್ಥಿತಿ '7': ಸ್ಥಿತಿ '8': ಸ್ಥಿತಿ '9':
            {
              ಪೂರ್ಣಾಂಕ c = cp[0] - '0';
              ಆದರೆ (c_isdigit (cp[1]))
                {
                  ++cp;
                  c = c * 8 + cp[0] - '0';
                }
              ಆದರೆ (c_isdigit (cp[1]))
                {
                  ++cp;
                  c = c * 8 + cp[0] - '0';
                }
              GROW (c);
            }
            ನಿಲ್ಲಿಸು;

          ಸ್ಥಿತಿ 'a': GROW ('\a'); ನಿಲ್ಲಿಸು;
          ಸ್ಥಿತಿ 'b': GROW ('\b'); ನಿಲ್ಲಿಸು;
          ಸ್ಥಿತಿ 'f': GROW ('\f'); ನಿಲ್ಲಿಸು;
          ಸ್ಥಿತಿ 'n': GROW ('\n'); ನಿಲ್ಲಿಸು;
          ಸ್ಥಿತಿ 'r': GROW ('\r'); ನಿಲ್ಲಿಸು;
          ಸ್ಥಿತಿ 't': GROW ('\t'); ನಿಲ್ಲಿಸು;
          ಸ್ಥಿತಿ 'v': GROW ('\v'); ನಿಲ್ಲಿಸು;

          ಸ್ಥಿತಿ 'x':
            {
              ಪೂರ್ಣಾಂಕ c = 0;
              ಆಗ (c_isxdigit (cp[1]))
                {
                  ++cp;
                  c = (c * 16 + (c_isdigit (cp[0]) ? cp[0] - '0'
                                 : c_isupper (cp[0]) ? cp[0] - 'A'
                                 : cp[0] - '0'));
                }
              GROW (c);
              ನಿಲ್ಲಿಸು;
            }
          }
        ನಿಲ್ಲಿಸು;

      ಪೂರ್ವನಿಯೋಜಿತ:
        GROW (*cp);
        ನಿಲ್ಲಿಸು;
      }
  ಪ್ರತಿಜ್ಞಾ (*cp == '"');
  ++cp;
  ಪ್ರತಿಜ್ಞಾ (*cp == '\0');
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು GROW
  ಹಿಂತಿರುಗಿ obstack_finish0 (&obstack_for_unquote);
}
