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
  #ଅନ୍ତର୍ଭାଵ "symlist.h"
  #ଅନ୍ତର୍ଭାଵ "symtab.h"
}

%code provides
{
  /* Initialize unquote.  */
  ଶୂନ୍ୟ parser_init (ଶୂନ୍ୟ);
  /* Deallocate storage for unquote.  */
  ଶୂନ୍ୟ parser_free (ଶୂନ୍ୟ);
}

%code ଶୀର୍ଷ_2
{
  /* On column 0 to please syntax-check.  */
#ଅନ୍ତର୍ଭାଵ <config.h>
}

%code
{
  #ଅନ୍ତର୍ଭାଵ "system.h"

  #ଅନ୍ତର୍ଭାଵ <c-ctype.h>
  #ଅନ୍ତର୍ଭାଵ <quotearg.h>
  #ଅନ୍ତର୍ଭାଵ <vasnprintf.h>
  #ଅନ୍ତର୍ଭାଵ <xmemdup0.h>

  #ଅନ୍ତର୍ଭାଵ "complain.h"
  #ଅନ୍ତର୍ଭାଵ "conflicts.h"
  #ଅନ୍ତର୍ଭାଵ "files.h"
  #ଅନ୍ତର୍ଭାଵ "getargs.h"
  #ଅନ୍ତର୍ଭାଵ "gram.h"
  #ଅନ୍ତର୍ଭାଵ "named-ref.h"
  #ଅନ୍ତର୍ଭାଵ "reader.h"
  #ଅନ୍ତର୍ଭାଵ "scan-code.h"
  #ଅନ୍ତର୍ଭାଵ "scan-gram.h"
  #ଅନ୍ତର୍ଭାଵ "strversion.h"

  /* Pretend to be at least that version, to check features published
     in that version while developping it.  */
  ସ୍ଥିର ଅଚଳ ଅକ୍ଷର* api_version = "3.8";

  ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା current_prec = 0;
  ସ୍ଥିର location current_lhs_loc;
  ସ୍ଥିର named_ref *current_lhs_named_ref;
  ସ୍ଥିର ପ୍ରତୀକ *current_lhs_ପ୍ରତୀକ;
  ସ୍ଥିର ପ୍ରତୀକ_class current_class = unknown_sym;

  /** Set the new current left-hand side symbol, possibly common
   * to several right-hand side parts of rule.
   */
  ସ୍ଥିର ଶୂନ୍ୟ current_lhs (ପ୍ରତୀକ *sym, location loc, named_ref *ref);

  #define YYLLOC_DEFAULT(Current, Rhs, N)         \
    (Current) = lloc_default (Rhs, N)
  ସ୍ଥିର YYLTYPE lloc_default (YYLTYPE ଅଚଳ *, ପୂର୍ଣ୍ଣସଂଖ୍ୟା);

  #define YY_LOCATION_PRINT(File, Loc)            \
    location_print (Loc, File)

  /* Strip initial '{' and final '}' (must be first and last characters).
     Return the result.  */
  ସ୍ଥିର ଅକ୍ଷର *strip_braces (ଅକ୍ଷର *code);

  /* Convert CODE by calling code_props_plain_init if PLAIN, otherwise
     code_props_symbol_action_init.  Calls
     gram_scanner_last_string_free to release the latest string from
     the scanner (should be CODE). */
  ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *translate_code (ଅକ୍ଷର *code, location loc, bool plain);

  /* Convert CODE by calling code_props_plain_init after having
     stripped the first and last characters (expected to be '{', and
     '}').  Calls gram_scanner_last_string_free to release the latest
     string from the scanner (should be CODE). */
  ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *translate_code_braceless (ଅକ୍ଷର *code, location loc);

  /* Handle a %header directive.  */
  ସ୍ଥିର ଶୂନ୍ୟ handle_header (ଅକ୍ଷର ଅଚଳ *ମୂଲ୍ୟ);

  /* Handle a %error-verbose directive.  */
  ସ୍ଥିର ଶୂନ୍ୟ handle_error_verbose (location ଅଚଳ *loc, ଅକ୍ଷର ଅଚଳ *directive);

  /* Handle a %file-prefix directive.  */
  ସ୍ଥିର ଶୂନ୍ୟ handle_file_prefix (location ଅଚଳ *loc,
                                  location ଅଚଳ *dir_loc,
                                  ଅକ୍ଷର ଅଚଳ *directive, ଅକ୍ଷର ଅଚଳ *ମୂଲ୍ୟ);

  /* Handle a %language directive.  */
  ସ୍ଥିର ଶୂନ୍ୟ handle_language (location ଅଚଳ *loc, ଅକ୍ଷର ଅଚଳ *lang);

  /* Handle a %name-prefix directive.  */
  ସ୍ଥିର ଶୂନ୍ୟ handle_name_prefix (location ଅଚଳ *loc,
                                  ଅକ୍ଷର ଅଚଳ *directive, ଅକ୍ଷର ଅଚଳ *ମୂଲ୍ୟ);

  /* Handle a %pure-parser directive.  */
  ସ୍ଥିର ଶୂନ୍ୟ handle_pure_parser (location ଅଚଳ *loc, ଅକ୍ଷର ଅଚଳ *directive);

  /* Handle a %require directive.  */
  ସ୍ଥିର ଶୂନ୍ୟ handle_require (location ଅଚଳ *loc, ଅକ୍ଷର ଅଚଳ *version);

  /* Handle a %skeleton directive.  */
  ସ୍ଥିର ଶୂନ୍ୟ handle_skeleton (location ଅଚଳ *loc, ଅକ୍ଷର ଅଚଳ *skel);

  /* Handle a %yacc directive.  */
  ସ୍ଥିର ଶୂନ୍ୟ handle_yacc (location ଅଚଳ *loc);

  /* Implementation of yyerror.  */
  ସ୍ଥିର ଶୂନ୍ୟ gram_error (location ଅଚଳ *, ଅକ୍ଷର ଅଚଳ *);

  /* A string that describes a char (e.g., 'a' -> "'a'").  */
  ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *char_name (ଅକ୍ଷର);

  /* Add style to semantic values in traces.  */
  ସ୍ଥିର ଶୂନ୍ୟ tron (FILE *yyo);
  ସ୍ଥିର ଶୂନ୍ୟ troff (FILE *yyo);

  /* Interpret a quoted string (such as `"Hello, \"World\"\n\""`).
     Manages the memory of the result.  */
  ସ୍ଥିର ଅକ୍ଷର *unquote (ଅଚଳ ଅକ୍ଷର *str);

  /* Discard the latest unquoted string.  */
  ସ୍ଥିର ଶୂନ୍ୟ unquote_free (ଅକ୍ଷର *last_string);
}

%define api.header.include {"parse-gram.h"}
%define api.prefix {gram_}
%define api.pure full
%define api.token.raw
%define api.ମୂଲ୍ୟ.ପ୍ରକାର ସଂଘ
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

  PERCENT_ବାମ        "%left"
  PERCENT_ଡାହାଣ       "%right"
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
  PERCENT_NO_ଧାଡ଼ିଗୁଡ଼ିକ        "%no-lines"
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

%ପ୍ରକାର <unsigned ଅକ୍ଷର> CHAR_LITERAL
%printer { fputs (char_name ($$), yyo); } <unsigned ଅକ୍ଷର>

%ପ୍ରକାର <ଅକ୍ଷର*> "{...}" "%?{...}" "%{...%}" EPILOGUE STRING TSTRING
%printer { fputs ($$, yyo); } <ଅକ୍ଷର*>

%ପ୍ରକାର <uniqstr>
  BRACKETED_ID ID ID_COLON
  PERCENT_ERROR_VERBOSE PERCENT_FILE_PREFIX PERCENT_FLAG PERCENT_NAME_PREFIX
  PERCENT_PURE_PARSER
  TAG tag tag.opt variable
%printer { fputs ($$, yyo); } <uniqstr>
%printer { fprintf (yyo, "[%s]", $$); } BRACKETED_ID
%printer { fprintf (yyo, "%s:", $$); } ID_COLON
%printer { fprintf (yyo, "%%%s", $$); } PERCENT_FLAG
%printer { fprintf (yyo, "<%s>", $$); } TAG tag

%token <ପୂର୍ଣ୍ଣସଂଖ୍ୟା> INT_LITERAL _("integer literal")
%printer { fprintf (yyo, "%d", $$); } <ପୂର୍ଣ୍ଣସଂଖ୍ୟା>

%ପ୍ରକାର <ପ୍ରତୀକ*> id id_colon string_as_id ପ୍ରତୀକ token_decl token_decl_for_prec
%printer { fprintf (yyo, "%s", $$ ? $$->tag : "<NULL>"); } <ପ୍ରତୀକ*>
%printer { fprintf (yyo, "%s:", $$->tag); } id_colon

%ପ୍ରକାର <assoc> precedence_declarator

%destructor { ପ୍ରତୀକ_list_free ($$); } <ପ୍ରତୀକ_list*>
%printer { ପ୍ରତୀକ_list_syms_print ($$, yyo); } <ପ୍ରତୀକ_list*>

%ପ୍ରକାର <named_ref*> named_ref.opt

/*---------.
| %param.  |
`---------*/
%code requires
{
  ପ୍ରକାରବ୍ୟାଖ୍ୟା ଗଣନା
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
  ସ୍ଥିର ଶୂନ୍ୟ add_param (param_type ପ୍ରକାର, ଅକ୍ଷର *decl, location loc);
  ସ୍ଥିର param_type current_param = param_none;
};
%token <param_type> PERCENT_PARAM "%param";
%printer
{
  ବାଛ ($$)
    {
#define CASE(In, Out)                                           \
      ସ୍ଥିତି param_ ## In: fputs ("%" #Out, yyo); ଭାଙ୍ଗ
      CASE (lex,   lex-param);
      CASE (parse, parse-param);
      CASE (both,  param);
#undef CASE
      ସ୍ଥିତି param_none: aver (false); ଭାଙ୍ଗ;
    }
} <param_type>;


                     /*==========\
                     | Grammar.  |
                     \==========*/
%%

ନିବେଶ:
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
| "%define" variable ମୂଲ୍ୟ
    {
      muscle_percent_define_ଭର୍ତ୍ତି ($2, @$, $3.kind, $3.chars,
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
| "%no-lines"                   { no_ଧାଡ଼ିଗୁଡ଼ିକ_flag = true; }
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
  ପ୍ରତୀକ_declaration
| "%start" symbols.1
    {
      grammar_start_symbols_add ($2);
    }
| code_props_type "{...}" generic_symlist
    {
      code_props code;
      code_props_ପ୍ରତୀକ_action_init (&code, $2, @2);
      code_props_translate_code (&code);
      {
        ପାଇଁ (ପ୍ରତୀକ_list *list = $3; list; list = list->next)
          ପ୍ରତୀକ_list_code_props_set (list, $1, &code);
        ପ୍ରତୀକ_list_free ($3);
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

%ପ୍ରକାର <code_props_type> code_props_type;
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
| ID     { muscle_percent_define_ଭର୍ତ୍ତି ("api.value.union.name",
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


%ପ୍ରକାର <ପ୍ରତୀକ_list*> nterm_decls ପ୍ରତୀକ_decls symbols.1
      token_decls token_decls_for_prec
      token_decl.1 token_decl_for_prec.1;
ପ୍ରତୀକ_declaration:
  "%nterm" { current_class = nterm_sym; } nterm_decls[syms]
    {
      current_class = unknown_sym;
      ପ୍ରତୀକ_list_free ($syms);
    }
| "%token" { current_class = token_sym; } token_decls[syms]
    {
      current_class = unknown_sym;
      ପ୍ରତୀକ_list_free ($syms);
    }
| "%type" { current_class = pct_type_sym; } ପ୍ରତୀକ_decls[syms]
    {
      current_class = unknown_sym;
      ପ୍ରତୀକ_list_free ($syms);
    }
| precedence_declarator token_decls_for_prec[syms]
    {
      ++current_prec;
      ପାଇଁ (ପ୍ରତୀକ_list *list = $syms; list; list = list->next)
        ପ୍ରତୀକ_precedence_set (list->content.sym, current_prec, $1, @1);
      ପ୍ରତୀକ_list_free ($syms);
    }
;

precedence_declarator:
  "%left"       { $$ = ବାମ_assoc; }
| "%right"      { $$ = ଡାହାଣ_assoc; }
| "%nonassoc"   { $$ = non_assoc; }
| "%precedence" { $$ = precedence_assoc; }
;

%ପ୍ରକାର <ଅକ୍ଷର*> string.opt;
string.opt:
  %empty  { $$ = ଖାଲି; }
| STRING  { $$ = $1; }
;

tag.opt:
  %empty { $$ = ଖାଲି; }
| TAG    { $$ = $1; }
;

%ପ୍ରକାର <ପ୍ରତୀକ_list*> generic_symlist generic_symlist_item;
generic_symlist:
  generic_symlist_item
| generic_symlist generic_symlist_item   { $$ = ପ୍ରତୀକ_list_append ($1, $2); }
;

generic_symlist_item:
  ପ୍ରତୀକ    { $$ = ପ୍ରତୀକ_list_sym_new ($1, @1); }
| tag       { $$ = ପ୍ରତୀକ_list_type_new ($1, @1); }
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
      $$ = ପ୍ରତୀକ_list_type_set ($syms, $TAG);
    }
| token_decls TAG token_decl.1[syms]
    {
      $$ = ପ୍ରତୀକ_list_append ($1, ପ୍ରତୀକ_list_type_set ($syms, $TAG));
    }
;

// One or more symbol declarations for %token or %nterm.
token_decl.1:
  token_decl                { $$ = ପ୍ରତୀକ_list_sym_new ($1, @1); }
| token_decl.1 token_decl   { $$ = ପ୍ରତୀକ_list_append ($1, ପ୍ରତୀକ_list_sym_new ($2, @2)); }

// One symbol declaration for %token or %nterm.
token_decl:
  id ପୂର୍ଣ୍ଣସଂଖ୍ୟା.opt[num] alias
    {
      $$ = $id;
      ପ୍ରତୀକ_class_set ($id, current_class, @id, true);
      ଯଦି (0 <= $ଅଙ୍କ)
        ପ୍ରତୀକ_code_set ($id, $ଅଙ୍କ, @ଅଙ୍କ);
      ଯଦି ($alias)
        ପ୍ରତୀକ_make_alias ($id, $alias, @alias);
    }
;

%ପ୍ରକାର <ପୂର୍ଣ୍ଣସଂଖ୍ୟା> ପୂର୍ଣ୍ଣସଂଖ୍ୟା.opt;
ପୂର୍ଣ୍ଣସଂଖ୍ୟା.opt:
  %empty  { $$ = -1; }
| INT_LITERAL
;

%ପ୍ରକାର <ପ୍ରତୀକ*> alias;
alias:
  %empty         { $$ = ଖାଲି; }
| string_as_id   { $$ = $1; }
| TSTRING
    {
      $$ = ପ୍ରତୀକ_get ($1, @1);
      ପ୍ରତୀକ_class_set ($$, token_sym, @1, false);
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
      $$ = ପ୍ରତୀକ_list_type_set ($syms, $TAG);
    }
| token_decls_for_prec TAG token_decl_for_prec.1[syms]
    {
      $$ = ପ୍ରତୀକ_list_append ($1, ପ୍ରତୀକ_list_type_set ($syms, $TAG));
    }
;

// One or more token declarations for precedence declaration.
token_decl_for_prec.1:
  token_decl_for_prec
    { $$ = ପ୍ରତୀକ_list_sym_new ($1, @1); }
| token_decl_for_prec.1 token_decl_for_prec
    { $$ = ପ୍ରତୀକ_list_append ($1, ପ୍ରତୀକ_list_sym_new ($2, @2)); }

// One token declaration for precedence declaration.
token_decl_for_prec:
  id ପୂର୍ଣ୍ଣସଂଖ୍ୟା.opt[num]
    {
      $$ = $id;
      ପ୍ରତୀକ_class_set ($id, token_sym, @id, false);
      ଯଦି (0 <= $ଅଙ୍କ)
        ପ୍ରତୀକ_code_set ($id, $ଅଙ୍କ, @ଅଙ୍କ);
    }
| string_as_id
;


/*-----------------------------------.
| symbol_decls (argument of %type).  |
`-----------------------------------*/

// A non empty list of typed symbols (for %type).
ପ୍ରତୀକ_decls:
  symbols.1[syms]
    {
      $$ = $syms;
    }
| TAG symbols.1[syms]
    {
      $$ = ପ୍ରତୀକ_list_type_set ($syms, $TAG);
    }
| ପ୍ରତୀକ_decls TAG symbols.1[syms]
    {
      $$ = ପ୍ରତୀକ_list_append ($1, ପ୍ରତୀକ_list_type_set ($syms, $TAG));
    }
;

// One or more symbols.
symbols.1:
  ପ୍ରତୀକ
    {
      ଯଦି (current_class != unknown_sym)
        ପ୍ରତୀକ_class_set ($ପ୍ରତୀକ, current_class, @ପ୍ରତୀକ, false);
      $$ = ପ୍ରତୀକ_list_sym_new ($ପ୍ରତୀକ, @ପ୍ରତୀକ);
    }
  | symbols.1 ପ୍ରତୀକ
    {
      ଯଦି (current_class != unknown_sym)
        ପ୍ରତୀକ_class_set ($ପ୍ରତୀକ, current_class, @ପ୍ରତୀକ, false);
      $$ = ପ୍ରତୀକ_list_append ($1, ପ୍ରତୀକ_list_sym_new ($ପ୍ରତୀକ, @ପ୍ରତୀକ));
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
    { grammar_current_rule_begin (current_lhs_ପ୍ରତୀକ, current_lhs_loc,
                                  current_lhs_named_ref); }
| rhs ପ୍ରତୀକ named_ref.opt
    { grammar_current_rule_ପ୍ରତୀକ_append ($2, @2, $3); }
| rhs tag.opt "{...}"[action] named_ref.opt[name]
    { grammar_current_rule_action_append ($action, @action, $name, $[tag.opt]); }
| rhs "%?{...}"
    { grammar_current_rule_predicate_append ($2, @2); }
| rhs "%empty"
    { grammar_current_rule_empty_set (@2); }
| rhs "%prec" ପ୍ରତୀକ
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
  %empty         { $$ = ଖାଲି; }
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
  #ଅନ୍ତର୍ଭାଵ "muscle-tab.h"
  ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା
  {
    ଅକ୍ଷର ଅଚଳ *chars;
    muscle_kind kind;
  } ମୂଲ୍ୟ_type;
};
%ପ୍ରକାର <ମୂଲ୍ୟ_type> ମୂଲ୍ୟ;
%printer
{
  ବାଛ ($$.kind)
    {
    ସ୍ଥିତି muscle_code:    fprintf (yyo,  "{%s}",  $$.chars); ଭାଙ୍ଗ;
    ସ୍ଥିତି muscle_keyword: fprintf (yyo,   "%s",   $$.chars); ଭାଙ୍ଗ;
    ସ୍ଥିତି muscle_string:  fprintf (yyo, "\"%s\"", $$.chars); ଭାଙ୍ଗ;
    }
} <ମୂଲ୍ୟ_type>;

ମୂଲ୍ୟ:
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
    { $$ = ପ୍ରତୀକ_from_uniqstr ($1, @1); }
| CHAR_LITERAL
    {
      ଅଚଳ ଅକ୍ଷର *var = "api.token.raw";
      ଯଦି (current_class == nterm_sym)
        {
          complain (&@1, complaint,
                    _("character literals cannot be nonterminals"));
          YYERROR;
        }
      ଯଦି (muscle_percent_define_ifdef (var))
        {
          complain (&@1, complaint,
                    _("character literals cannot be used together"
                    " with %s"), var);
          location loc = muscle_percent_define_get_loc (var);
          subcomplain (&loc, complaint, _("definition of %s"), var);
        }
      $$ = ପ୍ରତୀକ_get (char_name ($1), @1);
      ପ୍ରତୀକ_class_set ($$, token_sym, @1, false);
      ପ୍ରତୀକ_code_set ($$, $1, @1);
    }
;

id_colon:
  ID_COLON { $$ = ପ୍ରତୀକ_from_uniqstr ($1, @1); }
;


ପ୍ରତୀକ:
  id
| string_as_id
;

/* A string used as an ID.  */
string_as_id:
  STRING
    {
      $$ = ପ୍ରତୀକ_get ($1, @1);
      ପ୍ରତୀକ_class_set ($$, token_sym, @1, false);
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

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
yyreport_syntax_error (ଅଚଳ yypcontext_t *ctx)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା res = 0;
  /* Arguments of format: reported tokens (one for the "unexpected",
     one per "expected"). */
  ଗଣନା { ARGS_MAX = 5 };
  ଅଚଳ ଅକ୍ଷର *argv[ARGS_MAX];
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc = 0;
  yysymbol_kind_t unexpected = yypcontext_token (ctx);
  ଯଦି (unexpected != YYSYMBOL_YYEMPTY)
    {
      argv[argc++] = yysymbol_name (unexpected);
      yysymbol_kind_t expected[ARGS_MAX - 1];
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା nexpected = yypcontext_expected_tokens (ctx, expected, ARGS_MAX - 1);
      ଯଦି (nexpected < 0)
        res = nexpected;
      ଅନ୍ୟଥା
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < nexpected; ++i)
          argv[argc++] = yysymbol_name (expected[i]);
    }
  syntax_error (*yypcontext_location (ctx), argc, argv);
  ଫେରନ୍ତୁ res;
}


/* Return the location of the left-hand side of a rule whose
   right-hand side is RHS[1] ... RHS[N].  Ignore empty nonterminals in
   the right-hand side, and return an empty location equal to the end
   boundary of RHS[0] if the right-hand side is empty.  */

ସ୍ଥିର YYLTYPE
lloc_default (YYLTYPE ଅଚଳ *rhs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
  YYLTYPE loc;

  /* SGI MIPSpro 7.4.1m miscompiles "loc.start = loc.end = rhs[n].end;".
     The bug is fixed in 7.4.2m, but play it safe for now.  */
  loc.start = rhs[n].end;
  loc.end = rhs[n].end;

  /* Ignore empty nonterminals the start of the right-hand side.
     Do not bother to ignore them at the end of the right-hand side,
     since empty nonterminals have the same end as their predecessors.  */
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 1; i <= n; i++)
    ଯଦି (! equal_boundaries (rhs[i].start, rhs[i].end))
      {
        loc.start = rhs[i].start;
        ଭାଙ୍ଗ;
      }

  ଫେରନ୍ତୁ loc;
}

ସ୍ଥିର
ଅକ୍ଷର *strip_braces (ଅକ୍ଷର *code)
{
  code[strlen (code) - 1] = 0;
  ଫେରନ୍ତୁ code + 1;
}

ସ୍ଥିର
ଅକ୍ଷର ଅଚଳ *
translate_code (ଅକ୍ଷର *code, location loc, bool plain)
{
  code_props plain_code;
  ଯଦି (plain)
    code_props_plain_init (&plain_code, code, loc);
  ଅନ୍ୟଥା
    code_props_ପ୍ରତୀକ_action_init (&plain_code, code, loc);
  code_props_translate_code (&plain_code);
  gram_scanner_last_string_free ();
  ଫେରନ୍ତୁ plain_code.code;
}

ସ୍ଥିର
ଅକ୍ଷର ଅଚଳ *
translate_code_braceless (ଅକ୍ଷର *code, location loc)
{
  ଫେରନ୍ତୁ translate_code (strip_braces (code), loc, true);
}

ସ୍ଥିର ଶୂନ୍ୟ
add_param (param_type ପ୍ରକାର, ଅକ୍ଷର *decl, location loc)
{
  ସ୍ଥିର ଅକ୍ଷର ଅଚଳ alphanum[26 + 26 + 1 + 10 + 1] =
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "_"
    "0123456789";

  ଅକ୍ଷର ଅଚଳ *name_start = ଖାଲି;
  {
    ଅକ୍ଷର *p;
    /* Stop on last actual character.  */
    ପାଇଁ (p = decl; p[1]; p++)
      ଯଦି ((p == decl
           || ! memchr (alphanum, p[-1], ଆକାର alphanum - 1))
          && memchr (alphanum, p[0], ଆକାର alphanum - 10 - 1))
        name_start = p;

    /* Strip the surrounding '{' and '}', and any blanks just inside
       the braces.  */
    --p;
    ଯେତେବେଳେ (c_isspace ((unsigned ଅକ୍ଷର) *p))
      --p;
    p[1] = '\0';
    ++decl;
    ଯେତେବେଳେ (c_isspace ((unsigned ଅକ୍ଷର) *decl))
      ++decl;
  }

  ଯଦି (! name_start)
    complain (&loc, complaint, _("missing identifier in parameter declaration"));
  ଅନ୍ୟଥା
    {
      ଅକ୍ଷର *name = xmemdup0 (name_start, strspn (name_start, alphanum));
      ଯଦି (ପ୍ରକାର & param_lex)
        muscle_pair_list_grow ("lex_param", decl, name);
      ଯଦି (ପ୍ରକାର & param_parse)
        muscle_pair_list_grow ("parse_param", decl, name);
      ମୁକ୍ତ (name);
    }

  gram_scanner_last_string_free ();
}


ସ୍ଥିର ଶୂନ୍ୟ
handle_header (ଅକ୍ଷର ଅଚଳ *ମୂଲ୍ୟ)
{
  header_flag = true;
  ଯଦି (ମୂଲ୍ୟ)
    {
      ଅକ୍ଷର *file = unquote (ମୂଲ୍ୟ);
      spec_header_file = xstrdup (file);
      gram_scanner_last_string_free ();
      unquote_free (file);
    }
}


ସ୍ଥିର ଶୂନ୍ୟ
handle_error_verbose (location ଅଚଳ *loc, ଅକ୍ଷର ଅଚଳ *directive)
{
  bison_directive (loc, directive);
  muscle_percent_define_ଭର୍ତ୍ତି (directive, *loc, muscle_keyword, "",
                                MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
}


ସ୍ଥିର ଶୂନ୍ୟ
handle_file_prefix (location ଅଚଳ *loc,
                    location ଅଚଳ *dir_loc,
                    ଅକ୍ଷର ଅଚଳ *directive, ଅକ୍ଷର ଅଚଳ *ମୂଲ୍ୟ_quoted)
{
  ଅକ୍ଷର *ମୂଲ୍ୟ = unquote (ମୂଲ୍ୟ_quoted);
  bison_directive (loc, directive);
  bool warned = false;

  ଯଦି (location_empty (spec_file_prefix_loc))
    {
      spec_file_prefix_loc = *loc;
      spec_file_prefix = ମୂଲ୍ୟ;
    }
  ଅନ୍ୟଥା
    {
      duplicate_directive (directive, spec_file_prefix_loc, *loc);
      warned = true;
    }

  ଯଦି (!warned
      && STRNEQ (directive, "%file-prefix"))
    deprecated_directive (dir_loc, directive, "%file-prefix");
}

ସ୍ଥିର ଶୂନ୍ୟ
handle_language (location ଅଚଳ *loc, ଅକ୍ଷର ଅଚଳ *lang)
{
  language_argmatch (unquote (lang), grammar_prio, *loc);
}


ସ୍ଥିର ଶୂନ୍ୟ
handle_name_prefix (location ଅଚଳ *loc,
                    ଅକ୍ଷର ଅଚଳ *directive, ଅକ୍ଷର ଅଚଳ *ମୂଲ୍ୟ_quoted)
{
  ଅକ୍ଷର *ମୂଲ୍ୟ = unquote (ମୂଲ୍ୟ_quoted);
  bison_directive (loc, directive);

  ଅକ୍ଷର buf1[1024];
  size_t len1 = ଆକାର (buf1);
  ଅକ୍ଷର *old = asnprintf (buf1, &len1, "%s\"%s\"", directive, ମୂଲ୍ୟ);
  ଯଦି (!old)
    xalloc_die ();

  ଯଦି (location_empty (spec_name_prefix_loc))
    {
      spec_name_prefix = ମୂଲ୍ୟ;
      spec_name_prefix_loc = *loc;

      ଅକ୍ଷର buf2[1024];
      size_t len2 = ଆକାର (buf2);
      ଅକ୍ଷର *new = asnprintf (buf2, &len2, "%%define api.prefix {%s}", ମୂଲ୍ୟ);
      ଯଦି (!new)
        xalloc_die ();
      deprecated_directive (loc, old, new);
      ଯଦି (new != buf2)
        ମୁକ୍ତ (new);
    }
  ଅନ୍ୟଥା
    duplicate_directive (old, spec_file_prefix_loc, *loc);

  ଯଦି (old != buf1)
    ମୁକ୍ତ (old);
}


ସ୍ଥିର ଶୂନ୍ୟ
handle_pure_parser (location ଅଚଳ *loc, ଅକ୍ଷର ଅଚଳ *directive)
{
  bison_directive (loc, directive);
  deprecated_directive (loc, directive, "%define api.pure");
  muscle_percent_define_ଭର୍ତ୍ତି ("api.pure", *loc, muscle_keyword, "",
                                MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
}


ସ୍ଥିର ଶୂନ୍ୟ
handle_require (location ଅଚଳ *loc, ଅକ୍ଷର ଅଚଳ *version_quoted)
{
  ଅକ୍ଷର *version = unquote (version_quoted);
  required_version = strversion_to_int (version);
  ଯଦି (required_version == -1)
    {
      complain (loc, complaint, _("invalid version requirement: %s"),
                version);
      required_version = 0;
    }
  ଅନ୍ୟଥା
    {
      ଅଚଳ ଅକ୍ଷର* package_version =
        0 < strverscmp (api_version, PACKAGE_VERSION)
        ? api_version : PACKAGE_VERSION;
      ଯଦି (0 < strverscmp (version, package_version))
        {
          complain (loc, complaint, _("require bison %s, but have %s"),
                    version, package_version);
          exit (EX_MISMATCH);
        }
    }
  unquote_free (version);
  gram_scanner_last_string_free ();
}

ସ୍ଥିର ଶୂନ୍ୟ
handle_skeleton (location ଅଚଳ *loc, ଅକ୍ଷର ଅଚଳ *skel_quoted)
{
  ଅକ୍ଷର *skel = unquote (skel_quoted);
  ଅକ୍ଷର ଅଚଳ *skeleton_user = skel;
  ଯଦି (strchr (skeleton_user, '/'))
    {
      size_t dir_length = strlen (grammar_file);
      ଯେତେବେଳେ (dir_length && grammar_file[dir_length - 1] != '/')
        --dir_length;
      ଯେତେବେଳେ (dir_length && grammar_file[dir_length - 1] == '/')
        --dir_length;
      ଅକ୍ଷର *skeleton_build =
        xmalloc (dir_length + 1 + strlen (skeleton_user) + 1);
      ଯଦି (dir_length > 0)
        {
          memcpy (skeleton_build, grammar_file, dir_length);
          skeleton_build[dir_length++] = '/';
        }
      strcpy (skeleton_build + dir_length, skeleton_user);
      skeleton_user = uniqstr_new (skeleton_build);
      ମୁକ୍ତ (skeleton_build);
    }
  skeleton_arg (skeleton_user, grammar_prio, *loc);
}


ସ୍ଥିର ଶୂନ୍ୟ
handle_yacc (location ଅଚଳ *loc)
{
  ଅଚଳ ଅକ୍ଷର *directive = "%yacc";
  bison_directive (loc, directive);
  ଯଦି (location_empty (yacc_loc))
    set_yacc (*loc);
  ଅନ୍ୟଥା
    duplicate_directive (directive, yacc_loc, *loc);
}


ସ୍ଥିର ଶୂନ୍ୟ
gram_error (location ଅଚଳ *loc, ଅକ୍ଷର ଅଚଳ *ସନ୍ଦେଶ)
{
  complain (loc, complaint, "%s", ସନ୍ଦେଶ);
}

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *
char_name (ଅକ୍ଷର c)
{
  ଯଦି (c == '\'')
    ଫେରନ୍ତୁ "'\\''";
  ଅନ୍ୟଥା
    {
      ଅକ୍ଷର buf[4];
      buf[0] = '\''; buf[1] = c; buf[2] = '\''; buf[3] = '\0';
      ଫେରନ୍ତୁ quotearg_style (escape_quoting_style, buf);
    }
}

ସ୍ଥିର ଶୂନ୍ୟ
current_lhs (ପ୍ରତୀକ *sym, location loc, named_ref *ref)
{
  current_lhs_ପ୍ରତୀକ = sym;
  current_lhs_loc = loc;
  ଯଦି (sym)
    ପ୍ରତୀକ_location_as_lhs_set (sym, loc);
  /* In order to simplify memory management, named references for lhs
     are always assigned by deep copy into the current symbol_list
     node.  This is because a single named-ref in the grammar may
     result in several uses when the user factors lhs between several
     rules using "|".  Therefore free the parser's original copy.  */
  ମୁକ୍ତ (current_lhs_named_ref);
  current_lhs_named_ref = ref;
}

ସ୍ଥିର ଶୂନ୍ୟ tron (FILE *yyo)
{
  begin_use_class ("value", yyo);
}

ସ୍ଥିର ଶୂନ୍ୟ troff (FILE *yyo)
{
  end_use_class ("value", yyo);
}


/*----------.
| Unquote.  |
`----------*/

ସଂରଚନା obstack obstack_for_unquote;

ଶୂନ୍ୟ
parser_init (ଶୂନ୍ୟ)
{
  obstack_init (&obstack_for_unquote);
}

ଶୂନ୍ୟ
parser_free (ଶୂନ୍ୟ)
{
  obstack_free (&obstack_for_unquote, 0);
}

ସ୍ଥିର ଶୂନ୍ୟ
unquote_free (ଅକ୍ଷର *last_string)
{
  obstack_free (&obstack_for_unquote, last_string);
}

ସ୍ଥିର ଅକ୍ଷର *
unquote (ଅଚଳ ଅକ୍ଷର *cp)
{
#define GROW(Char)                              \
  obstack_1grow (&obstack_for_unquote, Char);
  ପାଇଁ (++cp; *cp && *cp != '"'; ++cp)
    ବାଛ (*cp)
      {
      ସ୍ଥିତି '"':
        ଭାଙ୍ଗ;
      ସ୍ଥିତି '\\':
        ++cp;
        ବାଛ (*cp)
          {
          ସ୍ଥିତି '0': ସ୍ଥିତି '1': ସ୍ଥିତି '2': ସ୍ଥିତି '3': ସ୍ଥିତି '4':
          ସ୍ଥିତି '5': ସ୍ଥିତି '6': ସ୍ଥିତି '7': ସ୍ଥିତି '8': ସ୍ଥିତି '9':
            {
              ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = cp[0] - '0';
              ଯଦି (c_isdigit (cp[1]))
                {
                  ++cp;
                  c = c * 8 + cp[0] - '0';
                }
              ଯଦି (c_isdigit (cp[1]))
                {
                  ++cp;
                  c = c * 8 + cp[0] - '0';
                }
              GROW (c);
            }
            ଭାଙ୍ଗ;

          ସ୍ଥିତି 'a': GROW ('\a'); ଭାଙ୍ଗ;
          ସ୍ଥିତି 'b': GROW ('\b'); ଭାଙ୍ଗ;
          ସ୍ଥିତି 'f': GROW ('\f'); ଭାଙ୍ଗ;
          ସ୍ଥିତି 'n': GROW ('\n'); ଭାଙ୍ଗ;
          ସ୍ଥିତି 'r': GROW ('\r'); ଭାଙ୍ଗ;
          ସ୍ଥିତି 't': GROW ('\t'); ଭାଙ୍ଗ;
          ସ୍ଥିତି 'v': GROW ('\v'); ଭାଙ୍ଗ;

          ସ୍ଥିତି 'x':
            {
              ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = 0;
              ଯେତେବେଳେ (c_isxdigit (cp[1]))
                {
                  ++cp;
                  c = (c * 16 + (c_isdigit (cp[0]) ? cp[0] - '0'
                                 : c_isupper (cp[0]) ? cp[0] - 'A'
                                 : cp[0] - '0'));
                }
              GROW (c);
              ଭାଙ୍ଗ;
            }
          }
        ଭାଙ୍ଗ;

      ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        GROW (*cp);
        ଭାଙ୍ଗ;
      }
  ପ୍ରତିଜ୍ଞା (*cp == '"');
  ++cp;
  ପ୍ରତିଜ୍ଞା (*cp == '\0');
#undef GROW
  ଫେରନ୍ତୁ obstack_finish0 (&obstack_for_unquote);
}
