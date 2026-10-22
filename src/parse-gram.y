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
  ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ* api_version = "3.8";

  ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ current_prec = 0;
  ಸ್ಥಿರ location current_lhs_loc;
  ಸ್ಥಿರ named_ref *current_lhs_named_ref;
  ಸ್ಥಿರ ಚಿಹ್ನೆ *current_lhs_ಚಿಹ್ನೆ;
  ಸ್ಥಿರ ಚಿಹ್ನೆ_class current_class = unknown_sym;

  /** Set the new current left-hand side symbol, possibly common
   * to several right-hand side parts of rule.
   */
  ಸ್ಥಿರ ಶೂನ್ಯ current_lhs (ಚಿಹ್ನೆ *sym, location loc, named_ref *ref);

  #define YYLLOC_DEFAULT(Current, Rhs, N)         \
    (Current) = lloc_default (Rhs, N)
  ಸ್ಥಿರ YYLTYPE lloc_default (YYLTYPE ಧ್ರುವ *, ಪೂರ್ಣಾಙ್ಕ);

  #define YY_LOCATION_PRINT(File, Loc)            \
    location_print (Loc, File)

  /* Strip initial '{' and final '}' (must be first and last characters).
     Return the result.  */
  ಸ್ಥಿರ ವರ್ಣ *strip_braces (ವರ್ಣ *code);

  /* Convert CODE by calling code_props_plain_init if PLAIN, otherwise
     code_props_symbol_action_init.  Calls
     gram_scanner_last_string_free to release the latest string from
     the scanner (should be CODE). */
  ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *translate_code (ವರ್ಣ *code, location loc, bool plain);

  /* Convert CODE by calling code_props_plain_init after having
     stripped the first and last characters (expected to be '{', and
     '}').  Calls gram_scanner_last_string_free to release the latest
     string from the scanner (should be CODE). */
  ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *translate_code_braceless (ವರ್ಣ *code, location loc);

  /* Handle a %header directive.  */
  ಸ್ಥಿರ ಶೂನ್ಯ handle_header (ವರ್ಣ ಧ್ರುವ *ಮೌಲ್ಯ);

  /* Handle a %error-verbose directive.  */
  ಸ್ಥಿರ ಶೂನ್ಯ handle_error_verbose (location ಧ್ರುವ *loc, ವರ್ಣ ಧ್ರುವ *directive);

  /* Handle a %file-prefix directive.  */
  ಸ್ಥಿರ ಶೂನ್ಯ handle_file_prefix (location ಧ್ರುವ *loc,
                                  location ಧ್ರುವ *dir_loc,
                                  ವರ್ಣ ಧ್ರುವ *directive, ವರ್ಣ ಧ್ರುವ *ಮೌಲ್ಯ);

  /* Handle a %language directive.  */
  ಸ್ಥಿರ ಶೂನ್ಯ handle_language (location ಧ್ರುವ *loc, ವರ್ಣ ಧ್ರುವ *lang);

  /* Handle a %name-prefix directive.  */
  ಸ್ಥಿರ ಶೂನ್ಯ handle_name_prefix (location ಧ್ರುವ *loc,
                                  ವರ್ಣ ಧ್ರುವ *directive, ವರ್ಣ ಧ್ರುವ *ಮೌಲ್ಯ);

  /* Handle a %pure-parser directive.  */
  ಸ್ಥಿರ ಶೂನ್ಯ handle_pure_parser (location ಧ್ರುವ *loc, ವರ್ಣ ಧ್ರುವ *directive);

  /* Handle a %require directive.  */
  ಸ್ಥಿರ ಶೂನ್ಯ handle_require (location ಧ್ರುವ *loc, ವರ್ಣ ಧ್ರುವ *version);

  /* Handle a %skeleton directive.  */
  ಸ್ಥಿರ ಶೂನ್ಯ handle_skeleton (location ಧ್ರುವ *loc, ವರ್ಣ ಧ್ರುವ *skel);

  /* Handle a %yacc directive.  */
  ಸ್ಥಿರ ಶೂನ್ಯ handle_yacc (location ಧ್ರುವ *loc);

  /* Implementation of yyerror.  */
  ಸ್ಥಿರ ಶೂನ್ಯ gram_error (location ಧ್ರುವ *, ವರ್ಣ ಧ್ರುವ *);

  /* A string that describes a char (e.g., 'a' -> "'a'").  */
  ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *char_name (ವರ್ಣ);

  /* Add style to semantic values in traces.  */
  ಸ್ಥಿರ ಶೂನ್ಯ tron (FILE *yyo);
  ಸ್ಥಿರ ಶೂನ್ಯ troff (FILE *yyo);

  /* Interpret a quoted string (such as `"Hello, \"World\"\n\""`).
     Manages the memory of the result.  */
  ಸ್ಥಿರ ವರ್ಣ *unquote (ಧ್ರುವ ವರ್ಣ *str);

  /* Discard the latest unquoted string.  */
  ಸ್ಥಿರ ಶೂನ್ಯ unquote_free (ವರ್ಣ *last_string);
}

%define api.header.include {"parse-gram.h"}
%define api.prefix {gram_}
%define api.pure full
%define api.token.raw
%define api.ಮೌಲ್ಯ.ಪ್ರಕಾರ ಸಙ್ಘ
%define locations
%define parse.error custom
%define parse.lac full
%define parse.trace
%header
%expect 0
%verbose

%initial-action
{
  /* Bison's grammar can initial empty locations, hence a default
     location is needed. */
  boundary_set (&@$.start, grammar_file, 1, 1, 1);
  boundary_set (&@$.end, grammar_file, 1, 1, 1);
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
  PERCENT_MERGE       "%merge"

  PERCENT_CODE            "%code"
  PERCENT_DEFAULT_PREC    "%default-prec"
  PERCENT_DEFINE          "%define"
  PERCENT_ERROR_VERBOSE   "%error-verbose"
  PERCENT_EXPECT          "%expect"
  PERCENT_EXPECT_RR       "%expect-rr"
  PERCENT_FILE_PREFIX     "%file-prefix"
  PERCENT_FLAG            "%<flag>"
  PERCENT_GLR_PARSER      "%glr-parser"
  PERCENT_HEADER          "%header"
  PERCENT_INITIAL_ACTION  "%initial-action"
  PERCENT_LANGUAGE        "%language"
  PERCENT_NAME_PREFIX     "%name-prefix"
  PERCENT_NO_DEFAULT_PREC "%no-default-prec"
  PERCENT_NO_ಸಾಲುಗಳು        "%no-lines"
  PERCENT_NONDETERMINISTIC_PARSER "%nondeterministic-parser"
  PERCENT_OUTPUT          "%output"
  PERCENT_PURE_PARSER     "%pure-parser"
  PERCENT_REQUIRE         "%require"
  PERCENT_SKELETON        "%skeleton"
  PERCENT_START           "%start"
  PERCENT_TOKEN_TABLE     "%token-table"
  PERCENT_VERBOSE         "%verbose"
  PERCENT_YACC            "%yacc"

  BRACED_CODE       "{...}"
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
  TAG               _("<tag>")
  TAG_ANY           "<*>"
  TAG_NONE          "<>"

 /* Experimental feature, don't rely on it.  */
%code pre-printer  {tron (yyo);}
%code post-printer {troff (yyo);}

%ಪ್ರಕಾರ <unsigned ವರ್ಣ> CHAR_LITERAL
%printer { fputs (char_name ($$), yyo); } <unsigned ವರ್ಣ>

%ಪ್ರಕಾರ <ವರ್ಣ*> "{...}" "%?{...}" "%{...%}" EPILOGUE STRING TSTRING
%printer { fputs ($$, yyo); } <ವರ್ಣ*>

%ಪ್ರಕಾರ <uniqstr>
  BRACKETED_ID ID ID_COLON
  PERCENT_ERROR_VERBOSE PERCENT_FILE_PREFIX PERCENT_FLAG PERCENT_NAME_PREFIX
  PERCENT_PURE_PARSER
  TAG tag tag.opt variable
%printer { fputs ($$, yyo); } <uniqstr>
%printer { fprintf (yyo, "[%s]", $$); } BRACKETED_ID
%printer { fprintf (yyo, "%s:", $$); } ID_COLON
%printer { fprintf (yyo, "%%%s", $$); } PERCENT_FLAG
%printer { fprintf (yyo, "<%s>", $$); } TAG tag

%token <ಪೂರ್ಣಾಙ್ಕ> INT_LITERAL _("integer literal")
%printer { fprintf (yyo, "%d", $$); } <ಪೂರ್ಣಾಙ್ಕ>

%ಪ್ರಕಾರ <ಚಿಹ್ನೆ*> id id_colon string_as_id ಚಿಹ್ನೆ token_decl token_decl_for_prec
%printer { fprintf (yyo, "%s", $$ ? $$->tag : "<NULL>"); } <ಚಿಹ್ನೆ*>
%printer { fprintf (yyo, "%s:", $$->tag); } id_colon

%ಪ್ರಕಾರ <assoc> precedence_declarator

%destructor { ಚಿಹ್ನೆ_list_free ($$); } <ಚಿಹ್ನೆ_list*>
%printer { ಚಿಹ್ನೆ_list_syms_print ($$, yyo); } <ಚಿಹ್ನೆ_list*>

%ಪ್ರಕಾರ <named_ref*> named_ref.opt

/*---------.
| %param.  |
`---------*/
%code requires
{
  ಪ್ರಕಾರನಿರ್ದೇಶ ಗಣನಾ
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
  ಸ್ಥಿರ ಶೂನ್ಯ add_param (param_type ಪ್ರಕಾರ, ವರ್ಣ *decl, location loc);
  ಸ್ಥಿರ param_type current_param = param_none;
};
%token <param_type> PERCENT_PARAM "%param";
%printer
{
  ವಿಕಲ್ಪ ($$)
    {
#define CASE(In, Out)                                           \
      ಪಕ್ಷ param_ ## In: fputs ("%" #Out, yyo); ಭಙ್ಗ
      CASE (lex,   lex-param);
      CASE (parse, parse-param);
      CASE (both,  param);
#undef CASE
      ಪಕ್ಷ param_none: aver (false); ಭಙ್ಗ;
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
      muscle_code_grow (union_seen ? "post_prologue" : "pre_prologue",
                        translate_code ($1, @1, true), @1);
      code_scanner_last_string_free ();
    }
| "%<flag>"
    {
      muscle_percent_define_ensure ($1, @1, true);
    }
| "%define" variable ಮೌಲ್ಯ
    {
      muscle_percent_define_ಸೇರಿಸು ($2, @$, $3.kind, $3.chars,
                                    MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
    }
| "%header" string.opt             { handle_header ($2); }
| "%error-verbose"                 { handle_error_verbose (&@$, $1); }
| "%expect" INT_LITERAL            { expected_sr_conflicts = $2; }
| "%expect-rr" INT_LITERAL         { expected_rr_conflicts = $2; }
| "%file-prefix" STRING            { handle_file_prefix (&@$, &@1, $1, $2); }
| "%glr-parser"
    {
      nondeterministic_parser = true;
      glr_parser = true;
    }
| "%initial-action" "{...}"
    {
      muscle_code_grow ("initial_action", translate_code ($2, @2, false), @2);
      code_scanner_last_string_free ();
    }
| "%language" STRING            { handle_language (&@1, $2); }
| "%name-prefix" STRING         { handle_name_prefix (&@$, $1, $2); }
| "%no-lines"                   { no_ಸಾಲುಗಳು_flag = true; }
| "%nondeterministic-parser"    { nondeterministic_parser = true; }
| "%output" STRING              { spec_outfile = unquote ($2); gram_scanner_last_string_free (); }
| "%param" { current_param = $1; } params { current_param = param_none; }
| "%pure-parser"                { handle_pure_parser (&@$, $1); }
| "%require" STRING             { handle_require (&@2, $2); }
| "%skeleton" STRING            { handle_skeleton (&@2, $2); }
| "%token-table"                { token_table_flag = true; }
| "%verbose"                    { report_flag |= report_states; }
| "%yacc"                       { handle_yacc (&@$); }
| error ";"                     { current_class = unknown_sym; yyerrok; }
| /*FIXME: Err?  What is this horror doing here? */ ";"
;

params:
   params "{...}"  { add_param (current_param, $2, @2); }
| "{...}"          { add_param (current_param, $1, @1); }
;


/*----------------------.
| grammar_declaration.  |
`----------------------*/

grammar_declaration:
  ಚಿಹ್ನೆ_declaration
| "%start" symbols.1
    {
      grammar_start_symbols_add ($2);
    }
| code_props_type "{...}" generic_symlist
    {
      code_props code;
      code_props_ಚಿಹ್ನೆ_action_init (&code, $2, @2);
      code_props_translate_code (&code);
      {
        ಪ್ರತಿ (ಚಿಹ್ನೆ_list *list = $3; list; list = list->next)
          ಚಿಹ್ನೆ_list_code_props_set (list, $1, &code);
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
      muscle_code_grow ("percent_code()",
                        translate_code_braceless ($2, @2), @2);
      code_scanner_last_string_free ();
    }
| "%code" ID "{...}"
    {
      muscle_percent_code_grow ($2, @2, translate_code_braceless ($3, @3), @3);
      code_scanner_last_string_free ();
    }
;

%ಪ್ರಕಾರ <code_props_type> code_props_type;
%printer { fprintf (yyo, "%s", code_props_type_string ($$)); } <code_props_type>;
code_props_type:
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
      union_seen = true;
      muscle_code_grow ("union_members", translate_code_braceless ($3, @3), @3);
      code_scanner_last_string_free ();
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
      ಪ್ರತಿ (ಚಿಹ್ನೆ_list *list = $syms; list; list = list->next)
        ಚಿಹ್ನೆ_precedence_set (list->content.sym, current_prec, $1, @1);
      ಚಿಹ್ನೆ_list_free ($syms);
    }
;

precedence_declarator:
  "%left"       { $$ = ಎಡ_assoc; }
| "%right"      { $$ = ಬಲ_assoc; }
| "%nonassoc"   { $$ = non_assoc; }
| "%precedence" { $$ = precedence_assoc; }
;

%ಪ್ರಕಾರ <ವರ್ಣ*> string.opt;
string.opt:
  %empty  { $$ = ರಿಕ್ತ; }
| STRING  { $$ = $1; }
;

tag.opt:
  %empty { $$ = ರಿಕ್ತ; }
| TAG    { $$ = $1; }
;

%ಪ್ರಕಾರ <ಚಿಹ್ನೆ_list*> generic_symlist generic_symlist_item;
generic_symlist:
  generic_symlist_item
| generic_symlist generic_symlist_item   { $$ = ಚಿಹ್ನೆ_list_append ($1, $2); }
;

generic_symlist_item:
  ಚಿಹ್ನೆ    { $$ = ಚಿಹ್ನೆ_list_sym_new ($1, @1); }
| tag       { $$ = ಚಿಹ್ನೆ_list_type_new ($1, @1); }
;

tag:
  TAG
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
| TAG token_decl.1[syms]
    {
      $$ = ಚಿಹ್ನೆ_list_type_set ($syms, $TAG);
    }
| token_decls TAG token_decl.1[syms]
    {
      $$ = ಚಿಹ್ನೆ_list_append ($1, ಚಿಹ್ನೆ_list_type_set ($syms, $TAG));
    }
;

// One or more symbol declarations for %token or %nterm.
token_decl.1:
  token_decl                { $$ = ಚಿಹ್ನೆ_list_sym_new ($1, @1); }
| token_decl.1 token_decl   { $$ = ಚಿಹ್ನೆ_list_append ($1, ಚಿಹ್ನೆ_list_sym_new ($2, @2)); }

// One symbol declaration for %token or %nterm.
token_decl:
  id ಪೂರ್ಣಾಙ್ಕ.opt[num] alias
    {
      $$ = $id;
      ಚಿಹ್ನೆ_class_set ($id, current_class, @id, true);
      ಯದಿ (0 <= $ಅಂಕೆ)
        ಚಿಹ್ನೆ_code_set ($id, $ಅಂಕೆ, @ಅಂಕೆ);
      ಯದಿ ($alias)
        ಚಿಹ್ನೆ_make_alias ($id, $alias, @alias);
    }
;

%ಪ್ರಕಾರ <ಪೂರ್ಣಾಙ್ಕ> ಪೂರ್ಣಾಙ್ಕ.opt;
ಪೂರ್ಣಾಙ್ಕ.opt:
  %empty  { $$ = -1; }
| INT_LITERAL
;

%ಪ್ರಕಾರ <ಚಿಹ್ನೆ*> alias;
alias:
  %empty         { $$ = ರಿಕ್ತ; }
| string_as_id   { $$ = $1; }
| TSTRING
    {
      $$ = ಚಿಹ್ನೆ_get ($1, @1);
      ಚಿಹ್ನೆ_class_set ($$, token_sym, @1, false);
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
| TAG token_decl_for_prec.1[syms]
    {
      $$ = ಚಿಹ್ನೆ_list_type_set ($syms, $TAG);
    }
| token_decls_for_prec TAG token_decl_for_prec.1[syms]
    {
      $$ = ಚಿಹ್ನೆ_list_append ($1, ಚಿಹ್ನೆ_list_type_set ($syms, $TAG));
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
  id ಪೂರ್ಣಾಙ್ಕ.opt[num]
    {
      $$ = $id;
      ಚಿಹ್ನೆ_class_set ($id, token_sym, @id, false);
      ಯದಿ (0 <= $ಅಂಕೆ)
        ಚಿಹ್ನೆ_code_set ($id, $ಅಂಕೆ, @ಅಂಕೆ);
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
| TAG symbols.1[syms]
    {
      $$ = ಚಿಹ್ನೆ_list_type_set ($syms, $TAG);
    }
| ಚಿಹ್ನೆ_decls TAG symbols.1[syms]
    {
      $$ = ಚಿಹ್ನೆ_list_append ($1, ಚಿಹ್ನೆ_list_type_set ($syms, $TAG));
    }
;

// One or more symbols.
symbols.1:
  ಚಿಹ್ನೆ
    {
      ಯದಿ (current_class != unknown_sym)
        ಚಿಹ್ನೆ_class_set ($ಚಿಹ್ನೆ, current_class, @ಚಿಹ್ನೆ, false);
      $$ = ಚಿಹ್ನೆ_list_sym_new ($ಚಿಹ್ನೆ, @ಚಿಹ್ನೆ);
    }
  | symbols.1 ಚಿಹ್ನೆ
    {
      ಯದಿ (current_class != unknown_sym)
        ಚಿಹ್ನೆ_class_set ($ಚಿಹ್ನೆ, current_class, @ಚಿಹ್ನೆ, false);
      $$ = ಚಿಹ್ನೆ_list_append ($1, ಚಿಹ್ನೆ_list_sym_new ($ಚಿಹ್ನೆ, @ಚಿಹ್ನೆ));
    }
;

        /*------------------------------------------.
        | The grammar section: between the two %%.  |
        `------------------------------------------*/

grammar:
  rules_or_grammar_declaration
| grammar rules_or_grammar_declaration
;

/* As a Bison extension, one can use the grammar declarations in the
   body of the grammar.  */
rules_or_grammar_declaration:
  rules
| grammar_declaration ";"
| error ";"
    {
      yyerrok;
    }
;

rules:
  id_colon named_ref.opt { current_lhs ($1, @1, $2); } ":" rhses.1
    {
      /* Free the current lhs. */
      current_lhs (0, @1, 0);
    }
;

rhses.1:
  rhs                { grammar_current_rule_end (@rhs); }
| rhses.1 "|" rhs    { grammar_current_rule_end (@rhs); }
| rhses.1 ";"
;

%token PERCENT_EMPTY "%empty";
rhs:
  %empty
    { grammar_current_rule_begin (current_lhs_ಚಿಹ್ನೆ, current_lhs_loc,
                                  current_lhs_named_ref); }
| rhs ಚಿಹ್ನೆ named_ref.opt
    { grammar_current_rule_ಚಿಹ್ನೆ_append ($2, @2, $3); }
| rhs tag.opt "{...}"[action] named_ref.opt[name]
    { grammar_current_rule_action_append ($action, @action, $name, $[tag.opt]); }
| rhs "%?{...}"
    { grammar_current_rule_predicate_append ($2, @2); }
| rhs "%empty"
    { grammar_current_rule_empty_set (@2); }
| rhs "%prec" ಚಿಹ್ನೆ
    { grammar_current_rule_prec_set ($3, @3); }
| rhs "%dprec" INT_LITERAL
    { grammar_current_rule_dprec_set ($3, @3); }
| rhs "%merge" TAG
    { grammar_current_rule_merge_set ($3, @3); }
| rhs "%expect" INT_LITERAL
    { grammar_current_rule_expect_sr ($3, @3); }
| rhs "%expect-rr" INT_LITERAL
    { grammar_current_rule_expect_rr ($3, @3); }
;

named_ref.opt:
  %empty         { $$ = ರಿಕ್ತ; }
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
  ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ
  {
    ವರ್ಣ ಧ್ರುವ *chars;
    muscle_kind kind;
  } ಮೌಲ್ಯ_type;
};
%ಪ್ರಕಾರ <ಮೌಲ್ಯ_type> ಮೌಲ್ಯ;
%printer
{
  ವಿಕಲ್ಪ ($$.kind)
    {
    ಪಕ್ಷ muscle_code:    fprintf (yyo,  "{%s}",  $$.chars); ಭಙ್ಗ;
    ಪಕ್ಷ muscle_keyword: fprintf (yyo,   "%s",   $$.chars); ಭಙ್ಗ;
    ಪಕ್ಷ muscle_string:  fprintf (yyo, "\"%s\"", $$.chars); ಭಙ್ಗ;
    }
} <ಮೌಲ್ಯ_type>;

ಮೌಲ್ಯ:
  %empty  { $$.kind = muscle_keyword; $$.chars = ""; }
| ID      { $$.kind = muscle_keyword; $$.chars = $1; }
| STRING  { $$.kind = muscle_string;  $$.chars = unquote ($1); gram_scanner_last_string_free ();}
| "{...}" { $$.kind = muscle_code;    $$.chars = strip_braces ($1); gram_scanner_last_string_free (); }
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
      ಧ್ರುವ ವರ್ಣ *var = "api.token.raw";
      ಯದಿ (current_class == nterm_sym)
        {
          complain (&@1, complaint,
                    _("character literals cannot be nonterminals"));
          YYERROR;
        }
      ಯದಿ (muscle_percent_define_ifdef (var))
        {
          complain (&@1, complaint,
                    _("character literals cannot be used together"
                    " with %s"), var);
          location loc = muscle_percent_define_get_loc (var);
          subcomplain (&loc, complaint, _("definition of %s"), var);
        }
      $$ = ಚಿಹ್ನೆ_get (char_name ($1), @1);
      ಚಿಹ್ನೆ_class_set ($$, token_sym, @1, false);
      ಚಿಹ್ನೆ_code_set ($$, $1, @1);
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
      ಚಿಹ್ನೆ_class_set ($$, token_sym, @1, false);
    }
;

epilogue.opt:
  %empty
| "%%" EPILOGUE
    {
      muscle_code_grow ("epilogue", translate_code ($2, @2, true), @2);
      code_scanner_last_string_free ();
    }
;

%%

ಪೂರ್ಣಾಙ್ಕ
yyreport_syntax_error (ಧ್ರುವ yypcontext_t *ctx)
{
  ಪೂರ್ಣಾಙ್ಕ res = 0;
  /* Arguments of format: reported tokens (one for the "unexpected",
     one per "expected"). */
  ಗಣನಾ { ARGS_MAX = 5 };
  ಧ್ರುವ ವರ್ಣ *argv[ARGS_MAX];
  ಪೂರ್ಣಾಙ್ಕ argc = 0;
  yysymbol_kind_t unexpected = yypcontext_token (ctx);
  ಯದಿ (unexpected != YYSYMBOL_YYEMPTY)
    {
      argv[argc++] = yysymbol_name (unexpected);
      yysymbol_kind_t expected[ARGS_MAX - 1];
      ಪೂರ್ಣಾಙ್ಕ nexpected = yypcontext_expected_tokens (ctx, expected, ARGS_MAX - 1);
      ಯದಿ (nexpected < 0)
        res = nexpected;
      ಅನ್ಯಥಾ
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < nexpected; ++i)
          argv[argc++] = yysymbol_name (expected[i]);
    }
  syntax_error (*yypcontext_location (ctx), argc, argv);
  ಪ್ರತ್ಯಾಗಮ res;
}


/* Return the location of the left-hand side of a rule whose
   right-hand side is RHS[1] ... RHS[N].  Ignore empty nonterminals in
   the right-hand side, and return an empty location equal to the end
   boundary of RHS[0] if the right-hand side is empty.  */

ಸ್ಥಿರ YYLTYPE
lloc_default (YYLTYPE ಧ್ರುವ *rhs, ಪೂರ್ಣಾಙ್ಕ n)
{
  YYLTYPE loc;

  /* SGI MIPSpro 7.4.1m miscompiles "loc.start = loc.end = rhs[n].end;".
     The bug is fixed in 7.4.2m, but play it safe for now.  */
  loc.start = rhs[n].end;
  loc.end = rhs[n].end;

  /* Ignore empty nonterminals the start of the right-hand side.
     Do not bother to ignore them at the end of the right-hand side,
     since empty nonterminals have the same end as their predecessors.  */
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 1; i <= n; i++)
    ಯದಿ (! equal_boundaries (rhs[i].start, rhs[i].end))
      {
        loc.start = rhs[i].start;
        ಭಙ್ಗ;
      }

  ಪ್ರತ್ಯಾಗಮ loc;
}

ಸ್ಥಿರ
ವರ್ಣ *strip_braces (ವರ್ಣ *code)
{
  code[strlen (code) - 1] = 0;
  ಪ್ರತ್ಯಾಗಮ code + 1;
}

ಸ್ಥಿರ
ವರ್ಣ ಧ್ರುವ *
translate_code (ವರ್ಣ *code, location loc, bool plain)
{
  code_props plain_code;
  ಯದಿ (plain)
    code_props_plain_init (&plain_code, code, loc);
  ಅನ್ಯಥಾ
    code_props_ಚಿಹ್ನೆ_action_init (&plain_code, code, loc);
  code_props_translate_code (&plain_code);
  gram_scanner_last_string_free ();
  ಪ್ರತ್ಯಾಗಮ plain_code.code;
}

ಸ್ಥಿರ
ವರ್ಣ ಧ್ರುವ *
translate_code_braceless (ವರ್ಣ *code, location loc)
{
  ಪ್ರತ್ಯಾಗಮ translate_code (strip_braces (code), loc, true);
}

ಸ್ಥಿರ ಶೂನ್ಯ
add_param (param_type ಪ್ರಕಾರ, ವರ್ಣ *decl, location loc)
{
  ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ alphanum[26 + 26 + 1 + 10 + 1] =
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "_"
    "0123456789";

  ವರ್ಣ ಧ್ರುವ *name_start = ರಿಕ್ತ;
  {
    ವರ್ಣ *p;
    /* Stop on last actual character.  */
    ಪ್ರತಿ (p = decl; p[1]; p++)
      ಯದಿ ((p == decl
           || ! memchr (alphanum, p[-1], ಪರಿಮಾಣ alphanum - 1))
          && memchr (alphanum, p[0], ಪರಿಮಾಣ alphanum - 10 - 1))
        name_start = p;

    /* Strip the surrounding '{' and '}', and any blanks just inside
       the braces.  */
    --p;
    ಯಾವತ್ (c_isspace ((unsigned ವರ್ಣ) *p))
      --p;
    p[1] = '\0';
    ++decl;
    ಯಾವತ್ (c_isspace ((unsigned ವರ್ಣ) *decl))
      ++decl;
  }

  ಯದಿ (! name_start)
    complain (&loc, complaint, _("missing identifier in parameter declaration"));
  ಅನ್ಯಥಾ
    {
      ವರ್ಣ *name = xmemdup0 (name_start, strspn (name_start, alphanum));
      ಯದಿ (ಪ್ರಕಾರ & param_lex)
        muscle_pair_list_grow ("lex_param", decl, name);
      ಯದಿ (ಪ್ರಕಾರ & param_parse)
        muscle_pair_list_grow ("parse_param", decl, name);
      ಮುಕ್ತ (name);
    }

  gram_scanner_last_string_free ();
}


ಸ್ಥಿರ ಶೂನ್ಯ
handle_header (ವರ್ಣ ಧ್ರುವ *ಮೌಲ್ಯ)
{
  header_flag = true;
  ಯದಿ (ಮೌಲ್ಯ)
    {
      ವರ್ಣ *file = unquote (ಮೌಲ್ಯ);
      spec_header_file = xstrdup (file);
      gram_scanner_last_string_free ();
      unquote_free (file);
    }
}


ಸ್ಥಿರ ಶೂನ್ಯ
handle_error_verbose (location ಧ್ರುವ *loc, ವರ್ಣ ಧ್ರುವ *directive)
{
  bison_directive (loc, directive);
  muscle_percent_define_ಸೇರಿಸು (directive, *loc, muscle_keyword, "",
                                MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
}


ಸ್ಥಿರ ಶೂನ್ಯ
handle_file_prefix (location ಧ್ರುವ *loc,
                    location ಧ್ರುವ *dir_loc,
                    ವರ್ಣ ಧ್ರುವ *directive, ವರ್ಣ ಧ್ರುವ *ಮೌಲ್ಯ_quoted)
{
  ವರ್ಣ *ಮೌಲ್ಯ = unquote (ಮೌಲ್ಯ_quoted);
  bison_directive (loc, directive);
  bool warned = false;

  ಯದಿ (location_empty (spec_file_prefix_loc))
    {
      spec_file_prefix_loc = *loc;
      spec_file_prefix = ಮೌಲ್ಯ;
    }
  ಅನ್ಯಥಾ
    {
      duplicate_directive (directive, spec_file_prefix_loc, *loc);
      warned = true;
    }

  ಯದಿ (!warned
      && STRNEQ (directive, "%file-prefix"))
    deprecated_directive (dir_loc, directive, "%file-prefix");
}

ಸ್ಥಿರ ಶೂನ್ಯ
handle_language (location ಧ್ರುವ *loc, ವರ್ಣ ಧ್ರುವ *lang)
{
  language_argmatch (unquote (lang), grammar_prio, *loc);
}


ಸ್ಥಿರ ಶೂನ್ಯ
handle_name_prefix (location ಧ್ರುವ *loc,
                    ವರ್ಣ ಧ್ರುವ *directive, ವರ್ಣ ಧ್ರುವ *ಮೌಲ್ಯ_quoted)
{
  ವರ್ಣ *ಮೌಲ್ಯ = unquote (ಮೌಲ್ಯ_quoted);
  bison_directive (loc, directive);

  ವರ್ಣ buf1[1024];
  size_t len1 = ಪರಿಮಾಣ (buf1);
  ವರ್ಣ *old = asnprintf (buf1, &len1, "%s\"%s\"", directive, ಮೌಲ್ಯ);
  ಯದಿ (!old)
    xalloc_die ();

  ಯದಿ (location_empty (spec_name_prefix_loc))
    {
      spec_name_prefix = ಮೌಲ್ಯ;
      spec_name_prefix_loc = *loc;

      ವರ್ಣ buf2[1024];
      size_t len2 = ಪರಿಮಾಣ (buf2);
      ವರ್ಣ *new = asnprintf (buf2, &len2, "%%define api.prefix {%s}", ಮೌಲ್ಯ);
      ಯದಿ (!new)
        xalloc_die ();
      deprecated_directive (loc, old, new);
      ಯದಿ (new != buf2)
        ಮುಕ್ತ (new);
    }
  ಅನ್ಯಥಾ
    duplicate_directive (old, spec_file_prefix_loc, *loc);

  ಯದಿ (old != buf1)
    ಮುಕ್ತ (old);
}


ಸ್ಥಿರ ಶೂನ್ಯ
handle_pure_parser (location ಧ್ರುವ *loc, ವರ್ಣ ಧ್ರುವ *directive)
{
  bison_directive (loc, directive);
  deprecated_directive (loc, directive, "%define api.pure");
  muscle_percent_define_ಸೇರಿಸು ("api.pure", *loc, muscle_keyword, "",
                                MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
}


ಸ್ಥಿರ ಶೂನ್ಯ
handle_require (location ಧ್ರುವ *loc, ವರ್ಣ ಧ್ರುವ *version_quoted)
{
  ವರ್ಣ *version = unquote (version_quoted);
  required_version = strversion_to_int (version);
  ಯದಿ (required_version == -1)
    {
      complain (loc, complaint, _("invalid version requirement: %s"),
                version);
      required_version = 0;
    }
  ಅನ್ಯಥಾ
    {
      ಧ್ರುವ ವರ್ಣ* package_version =
        0 < strverscmp (api_version, PACKAGE_VERSION)
        ? api_version : PACKAGE_VERSION;
      ಯದಿ (0 < strverscmp (version, package_version))
        {
          complain (loc, complaint, _("require bison %s, but have %s"),
                    version, package_version);
          exit (EX_MISMATCH);
        }
    }
  unquote_free (version);
  gram_scanner_last_string_free ();
}

ಸ್ಥಿರ ಶೂನ್ಯ
handle_skeleton (location ಧ್ರುವ *loc, ವರ್ಣ ಧ್ರುವ *skel_quoted)
{
  ವರ್ಣ *skel = unquote (skel_quoted);
  ವರ್ಣ ಧ್ರುವ *skeleton_user = skel;
  ಯದಿ (strchr (skeleton_user, '/'))
    {
      size_t dir_length = strlen (grammar_file);
      ಯಾವತ್ (dir_length && grammar_file[dir_length - 1] != '/')
        --dir_length;
      ಯಾವತ್ (dir_length && grammar_file[dir_length - 1] == '/')
        --dir_length;
      ವರ್ಣ *skeleton_build =
        xmalloc (dir_length + 1 + strlen (skeleton_user) + 1);
      ಯದಿ (dir_length > 0)
        {
          memcpy (skeleton_build, grammar_file, dir_length);
          skeleton_build[dir_length++] = '/';
        }
      strcpy (skeleton_build + dir_length, skeleton_user);
      skeleton_user = uniqstr_new (skeleton_build);
      ಮುಕ್ತ (skeleton_build);
    }
  skeleton_arg (skeleton_user, grammar_prio, *loc);
}


ಸ್ಥಿರ ಶೂನ್ಯ
handle_yacc (location ಧ್ರುವ *loc)
{
  ಧ್ರುವ ವರ್ಣ *directive = "%yacc";
  bison_directive (loc, directive);
  ಯದಿ (location_empty (yacc_loc))
    set_yacc (*loc);
  ಅನ್ಯಥಾ
    duplicate_directive (directive, yacc_loc, *loc);
}


ಸ್ಥಿರ ಶೂನ್ಯ
gram_error (location ಧ್ರುವ *loc, ವರ್ಣ ಧ್ರುವ *ಸಂದೇಶ)
{
  complain (loc, complaint, "%s", ಸಂದೇಶ);
}

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *
char_name (ವರ್ಣ c)
{
  ಯದಿ (c == '\'')
    ಪ್ರತ್ಯಾಗಮ "'\\''";
  ಅನ್ಯಥಾ
    {
      ವರ್ಣ buf[4];
      buf[0] = '\''; buf[1] = c; buf[2] = '\''; buf[3] = '\0';
      ಪ್ರತ್ಯಾಗಮ quotearg_style (escape_quoting_style, buf);
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
current_lhs (ಚಿಹ್ನೆ *sym, location loc, named_ref *ref)
{
  current_lhs_ಚಿಹ್ನೆ = sym;
  current_lhs_loc = loc;
  ಯದಿ (sym)
    ಚಿಹ್ನೆ_location_as_lhs_set (sym, loc);
  /* In order to simplify memory management, named references for lhs
     are always assigned by deep copy into the current symbol_list
     node.  This is because a single named-ref in the grammar may
     result in several uses when the user factors lhs between several
     rules using "|".  Therefore free the parser's original copy.  */
  ಮುಕ್ತ (current_lhs_named_ref);
  current_lhs_named_ref = ref;
}

ಸ್ಥಿರ ಶೂನ್ಯ tron (FILE *yyo)
{
  begin_use_class ("value", yyo);
}

ಸ್ಥಿರ ಶೂನ್ಯ troff (FILE *yyo)
{
  end_use_class ("value", yyo);
}


/*----------.
| Unquote.  |
`----------*/

ಸಂರಚನಾ obstack obstack_for_unquote;

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
unquote_free (ವರ್ಣ *last_string)
{
  obstack_free (&obstack_for_unquote, last_string);
}

ಸ್ಥಿರ ವರ್ಣ *
unquote (ಧ್ರುವ ವರ್ಣ *cp)
{
#define GROW(Char)                              \
  obstack_1grow (&obstack_for_unquote, Char);
  ಪ್ರತಿ (++cp; *cp && *cp != '"'; ++cp)
    ವಿಕಲ್ಪ (*cp)
      {
      ಪಕ್ಷ '"':
        ಭಙ್ಗ;
      ಪಕ್ಷ '\\':
        ++cp;
        ವಿಕಲ್ಪ (*cp)
          {
          ಪಕ್ಷ '0': ಪಕ್ಷ '1': ಪಕ್ಷ '2': ಪಕ್ಷ '3': ಪಕ್ಷ '4':
          ಪಕ್ಷ '5': ಪಕ್ಷ '6': ಪಕ್ಷ '7': ಪಕ್ಷ '8': ಪಕ್ಷ '9':
            {
              ಪೂರ್ಣಾಙ್ಕ c = cp[0] - '0';
              ಯದಿ (c_isdigit (cp[1]))
                {
                  ++cp;
                  c = c * 8 + cp[0] - '0';
                }
              ಯದಿ (c_isdigit (cp[1]))
                {
                  ++cp;
                  c = c * 8 + cp[0] - '0';
                }
              GROW (c);
            }
            ಭಙ್ಗ;

          ಪಕ್ಷ 'a': GROW ('\a'); ಭಙ್ಗ;
          ಪಕ್ಷ 'b': GROW ('\b'); ಭಙ್ಗ;
          ಪಕ್ಷ 'f': GROW ('\f'); ಭಙ್ಗ;
          ಪಕ್ಷ 'n': GROW ('\n'); ಭಙ್ಗ;
          ಪಕ್ಷ 'r': GROW ('\r'); ಭಙ್ಗ;
          ಪಕ್ಷ 't': GROW ('\t'); ಭಙ್ಗ;
          ಪಕ್ಷ 'v': GROW ('\v'); ಭಙ್ಗ;

          ಪಕ್ಷ 'x':
            {
              ಪೂರ್ಣಾಙ್ಕ c = 0;
              ಯಾವತ್ (c_isxdigit (cp[1]))
                {
                  ++cp;
                  c = (c * 16 + (c_isdigit (cp[0]) ? cp[0] - '0'
                                 : c_isupper (cp[0]) ? cp[0] - 'A'
                                 : cp[0] - '0'));
                }
              GROW (c);
              ಭಙ್ಗ;
            }
          }
        ಭಙ್ಗ;

      ಸ್ವಭಾವ:
        GROW (*cp);
        ಭಙ್ಗ;
      }
  ಪ್ರತಿಜ್ಞಾ (*cp == '"');
  ++cp;
  ಪ್ರತಿಜ್ಞಾ (*cp == '\0');
#undef GROW
  ಪ್ರತ್ಯಾಗಮ obstack_finish0 (&obstack_for_unquote);
}
