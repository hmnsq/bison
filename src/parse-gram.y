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
  #অন্তর্ভাব "symlist.h"
  #অন্তর্ভাব "symtab.h"
}

%code provides
{
  /* Initialize unquote.  */
  শূন্য parser_init (শূন্য);
  /* Deallocate storage for unquote.  */
  শূন্য parser_free (শূন্য);
}

%code শীর্ষ_2
{
  /* On column 0 to please syntax-check.  */
#অন্তর্ভাব <config.h>
}

%code
{
  #অন্তর্ভাব "system.h"

  #অন্তর্ভাব <c-ctype.h>
  #অন্তর্ভাব <quotearg.h>
  #অন্তর্ভাব <vasnprintf.h>
  #অন্তর্ভাব <xmemdup0.h>

  #অন্তর্ভাব "complain.h"
  #অন্তর্ভাব "conflicts.h"
  #অন্তর্ভাব "files.h"
  #অন্তর্ভাব "getargs.h"
  #অন্তর্ভাব "gram.h"
  #অন্তর্ভাব "named-ref.h"
  #অন্তর্ভাব "reader.h"
  #অন্তর্ভাব "scan-code.h"
  #অন্তর্ভাব "scan-gram.h"
  #অন্তর্ভাব "strversion.h"

  /* Pretend to be at least that version, to check features published
     in that version while developping it.  */
  স্থির ধ্রুবক অক্ষর* api_version = "3.8";

  স্থির পূর্ণসংখ্যা current_prec = 0;
  স্থির location current_lhs_loc;
  স্থির named_ref *current_lhs_named_ref;
  স্থির প্রতীক *current_lhs_প্রতীক;
  স্থির প্রতীক_class current_class = unknown_sym;

  /** Set the new current left-hand side symbol, possibly common
   * to several right-hand side parts of rule.
   */
  স্থির শূন্য current_lhs (প্রতীক *sym, location loc, named_ref *ref);

  #define YYLLOC_DEFAULT(Current, Rhs, N)         \
    (Current) = lloc_default (Rhs, N)
  স্থির YYLTYPE lloc_default (YYLTYPE ধ্রুবক *, পূর্ণসংখ্যা);

  #define YY_LOCATION_PRINT(File, Loc)            \
    location_print (Loc, File)

  /* Strip initial '{' and final '}' (must be first and last characters).
     Return the result.  */
  স্থির অক্ষর *strip_braces (অক্ষর *code);

  /* Convert CODE by calling code_props_plain_init if PLAIN, otherwise
     code_props_symbol_action_init.  Calls
     gram_scanner_last_string_free to release the latest string from
     the scanner (should be CODE). */
  স্থির অক্ষর ধ্রুবক *translate_code (অক্ষর *code, location loc, bool plain);

  /* Convert CODE by calling code_props_plain_init after having
     stripped the first and last characters (expected to be '{', and
     '}').  Calls gram_scanner_last_string_free to release the latest
     string from the scanner (should be CODE). */
  স্থির অক্ষর ধ্রুবক *translate_code_braceless (অক্ষর *code, location loc);

  /* Handle a %header directive.  */
  স্থির শূন্য handle_header (অক্ষর ধ্রুবক *মান);

  /* Handle a %error-verbose directive.  */
  স্থির শূন্য handle_error_verbose (location ধ্রুবক *loc, অক্ষর ধ্রুবক *directive);

  /* Handle a %file-prefix directive.  */
  স্থির শূন্য handle_file_prefix (location ধ্রুবক *loc,
                                  location ধ্রুবক *dir_loc,
                                  অক্ষর ধ্রুবক *directive, অক্ষর ধ্রুবক *মান);

  /* Handle a %language directive.  */
  স্থির শূন্য handle_language (location ধ্রুবক *loc, অক্ষর ধ্রুবক *lang);

  /* Handle a %name-prefix directive.  */
  স্থির শূন্য handle_name_prefix (location ধ্রুবক *loc,
                                  অক্ষর ধ্রুবক *directive, অক্ষর ধ্রুবক *মান);

  /* Handle a %pure-parser directive.  */
  স্থির শূন্য handle_pure_parser (location ধ্রুবক *loc, অক্ষর ধ্রুবক *directive);

  /* Handle a %require directive.  */
  স্থির শূন্য handle_require (location ধ্রুবক *loc, অক্ষর ধ্রুবক *version);

  /* Handle a %skeleton directive.  */
  স্থির শূন্য handle_skeleton (location ধ্রুবক *loc, অক্ষর ধ্রুবক *skel);

  /* Handle a %yacc directive.  */
  স্থির শূন্য handle_yacc (location ধ্রুবক *loc);

  /* Implementation of yyerror.  */
  স্থির শূন্য gram_error (location ধ্রুবক *, অক্ষর ধ্রুবক *);

  /* A string that describes a char (e.g., 'a' -> "'a'").  */
  স্থির অক্ষর ধ্রুবক *char_name (অক্ষর);

  /* Add style to semantic values in traces.  */
  স্থির শূন্য tron (FILE *yyo);
  স্থির শূন্য troff (FILE *yyo);

  /* Interpret a quoted string (such as `"Hello, \"World\"\n\""`).
     Manages the memory of the result.  */
  স্থির অক্ষর *unquote (ধ্রুবক অক্ষর *str);

  /* Discard the latest unquoted string.  */
  স্থির শূন্য unquote_free (অক্ষর *last_string);
}

%define api.header.include {"parse-gram.h"}
%define api.prefix {gram_}
%define api.pure full
%define api.token.raw
%define api.মান.প্রকার সংঘ
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

  PERCENT_বাম        "%left"
  PERCENT_ডান       "%right"
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
  PERCENT_NO_সারিগুলি        "%no-lines"
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

%প্রকার <unsigned অক্ষর> CHAR_LITERAL
%printer { fputs (char_name ($$), yyo); } <unsigned অক্ষর>

%প্রকার <অক্ষর*> "{...}" "%?{...}" "%{...%}" EPILOGUE STRING TSTRING
%printer { fputs ($$, yyo); } <অক্ষর*>

%প্রকার <uniqstr>
  BRACKETED_ID ID ID_COLON
  PERCENT_ERROR_VERBOSE PERCENT_FILE_PREFIX PERCENT_FLAG PERCENT_NAME_PREFIX
  PERCENT_PURE_PARSER
  TAG tag tag.opt variable
%printer { fputs ($$, yyo); } <uniqstr>
%printer { fprintf (yyo, "[%s]", $$); } BRACKETED_ID
%printer { fprintf (yyo, "%s:", $$); } ID_COLON
%printer { fprintf (yyo, "%%%s", $$); } PERCENT_FLAG
%printer { fprintf (yyo, "<%s>", $$); } TAG tag

%token <পূর্ণসংখ্যা> INT_LITERAL _("integer literal")
%printer { fprintf (yyo, "%d", $$); } <পূর্ণসংখ্যা>

%প্রকার <প্রতীক*> id id_colon string_as_id প্রতীক token_decl token_decl_for_prec
%printer { fprintf (yyo, "%s", $$ ? $$->tag : "<NULL>"); } <প্রতীক*>
%printer { fprintf (yyo, "%s:", $$->tag); } id_colon

%প্রকার <assoc> precedence_declarator

%destructor { প্রতীক_list_free ($$); } <প্রতীক_list*>
%printer { প্রতীক_list_syms_print ($$, yyo); } <প্রতীক_list*>

%প্রকার <named_ref*> named_ref.opt

/*---------.
| %param.  |
`---------*/
%code requires
{
  প্রকারলেখ গণনা
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
  স্থির শূন্য add_param (param_type প্রকার, অক্ষর *decl, location loc);
  স্থির param_type current_param = param_none;
};
%token <param_type> PERCENT_PARAM "%param";
%printer
{
  নির্বাচন ($$)
    {
#define CASE(In, Out)                                           \
      ক্ষেত্র param_ ## In: fputs ("%" #Out, yyo); বিরতি
      CASE (lex,   lex-param);
      CASE (parse, parse-param);
      CASE (both,  param);
#undef CASE
      ক্ষেত্র param_none: aver (false); বিরতি;
    }
} <param_type>;


                     /*==========\
                     | Grammar.  |
                     \==========*/
%%

প্রবেশ:
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
| "%define" variable মান
    {
      muscle_percent_define_ঢোকাও ($2, @$, $3.kind, $3.chars,
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
| "%no-lines"                   { no_সারিগুলি_flag = true; }
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
  প্রতীক_declaration
| "%start" symbols.1
    {
      grammar_start_symbols_add ($2);
    }
| code_props_type "{...}" generic_symlist
    {
      code_props code;
      code_props_প্রতীক_action_init (&code, $2, @2);
      code_props_translate_code (&code);
      {
        জন্য (প্রতীক_list *list = $3; list; list = list->পরবর্তী)
          প্রতীক_list_code_props_set (list, $1, &code);
        প্রতীক_list_free ($3);
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

%প্রকার <code_props_type> code_props_type;
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
| ID     { muscle_percent_define_ঢোকাও ("api.value.union.name",
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


%প্রকার <প্রতীক_list*> nterm_decls প্রতীক_decls symbols.1
      token_decls token_decls_for_prec
      token_decl.1 token_decl_for_prec.1;
প্রতীক_declaration:
  "%nterm" { current_class = nterm_sym; } nterm_decls[syms]
    {
      current_class = unknown_sym;
      প্রতীক_list_free ($syms);
    }
| "%token" { current_class = token_sym; } token_decls[syms]
    {
      current_class = unknown_sym;
      প্রতীক_list_free ($syms);
    }
| "%type" { current_class = pct_type_sym; } প্রতীক_decls[syms]
    {
      current_class = unknown_sym;
      প্রতীক_list_free ($syms);
    }
| precedence_declarator token_decls_for_prec[syms]
    {
      ++current_prec;
      জন্য (প্রতীক_list *list = $syms; list; list = list->পরবর্তী)
        প্রতীক_precedence_set (list->content.sym, current_prec, $1, @1);
      প্রতীক_list_free ($syms);
    }
;

precedence_declarator:
  "%left"       { $$ = বাম_assoc; }
| "%right"      { $$ = ডান_assoc; }
| "%nonassoc"   { $$ = non_assoc; }
| "%precedence" { $$ = precedence_assoc; }
;

%প্রকার <অক্ষর*> string.opt;
string.opt:
  %empty  { $$ = ফাঁকা; }
| STRING  { $$ = $1; }
;

tag.opt:
  %empty { $$ = ফাঁকা; }
| TAG    { $$ = $1; }
;

%প্রকার <প্রতীক_list*> generic_symlist generic_symlist_item;
generic_symlist:
  generic_symlist_item
| generic_symlist generic_symlist_item   { $$ = প্রতীক_list_append ($1, $2); }
;

generic_symlist_item:
  প্রতীক    { $$ = প্রতীক_list_sym_new ($1, @1); }
| tag       { $$ = প্রতীক_list_type_new ($1, @1); }
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
      $$ = প্রতীক_list_type_set ($syms, $TAG);
    }
| token_decls TAG token_decl.1[syms]
    {
      $$ = প্রতীক_list_append ($1, প্রতীক_list_type_set ($syms, $TAG));
    }
;

// One or more symbol declarations for %token or %nterm.
token_decl.1:
  token_decl                { $$ = প্রতীক_list_sym_new ($1, @1); }
| token_decl.1 token_decl   { $$ = প্রতীক_list_append ($1, প্রতীক_list_sym_new ($2, @2)); }

// One symbol declaration for %token or %nterm.
token_decl:
  id পূর্ণসংখ্যা.opt[num] alias
    {
      $$ = $id;
      প্রতীক_class_set ($id, current_class, @id, true);
      যদি (0 <= $অঙ্ক)
        প্রতীক_code_set ($id, $অঙ্ক, @অঙ্ক);
      যদি ($alias)
        প্রতীক_make_alias ($id, $alias, @alias);
    }
;

%প্রকার <পূর্ণসংখ্যা> পূর্ণসংখ্যা.opt;
পূর্ণসংখ্যা.opt:
  %empty  { $$ = -1; }
| INT_LITERAL
;

%প্রকার <প্রতীক*> alias;
alias:
  %empty         { $$ = ফাঁকা; }
| string_as_id   { $$ = $1; }
| TSTRING
    {
      $$ = প্রতীক_get ($1, @1);
      প্রতীক_class_set ($$, token_sym, @1, false);
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
      $$ = প্রতীক_list_type_set ($syms, $TAG);
    }
| token_decls_for_prec TAG token_decl_for_prec.1[syms]
    {
      $$ = প্রতীক_list_append ($1, প্রতীক_list_type_set ($syms, $TAG));
    }
;

// One or more token declarations for precedence declaration.
token_decl_for_prec.1:
  token_decl_for_prec
    { $$ = প্রতীক_list_sym_new ($1, @1); }
| token_decl_for_prec.1 token_decl_for_prec
    { $$ = প্রতীক_list_append ($1, প্রতীক_list_sym_new ($2, @2)); }

// One token declaration for precedence declaration.
token_decl_for_prec:
  id পূর্ণসংখ্যা.opt[num]
    {
      $$ = $id;
      প্রতীক_class_set ($id, token_sym, @id, false);
      যদি (0 <= $অঙ্ক)
        প্রতীক_code_set ($id, $অঙ্ক, @অঙ্ক);
    }
| string_as_id
;


/*-----------------------------------.
| symbol_decls (argument of %type).  |
`-----------------------------------*/

// A non empty list of typed symbols (for %type).
প্রতীক_decls:
  symbols.1[syms]
    {
      $$ = $syms;
    }
| TAG symbols.1[syms]
    {
      $$ = প্রতীক_list_type_set ($syms, $TAG);
    }
| প্রতীক_decls TAG symbols.1[syms]
    {
      $$ = প্রতীক_list_append ($1, প্রতীক_list_type_set ($syms, $TAG));
    }
;

// One or more symbols.
symbols.1:
  প্রতীক
    {
      যদি (current_class != unknown_sym)
        প্রতীক_class_set ($প্রতীক, current_class, @প্রতীক, false);
      $$ = প্রতীক_list_sym_new ($প্রতীক, @প্রতীক);
    }
  | symbols.1 প্রতীক
    {
      যদি (current_class != unknown_sym)
        প্রতীক_class_set ($প্রতীক, current_class, @প্রতীক, false);
      $$ = প্রতীক_list_append ($1, প্রতীক_list_sym_new ($প্রতীক, @প্রতীক));
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
    { grammar_current_rule_begin (current_lhs_প্রতীক, current_lhs_loc,
                                  current_lhs_named_ref); }
| rhs প্রতীক named_ref.opt
    { grammar_current_rule_প্রতীক_append ($2, @2, $3); }
| rhs tag.opt "{...}"[action] named_ref.opt[name]
    { grammar_current_rule_action_append ($action, @action, $name, $[tag.opt]); }
| rhs "%?{...}"
    { grammar_current_rule_predicate_append ($2, @2); }
| rhs "%empty"
    { grammar_current_rule_empty_set (@2); }
| rhs "%prec" প্রতীক
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
  %empty         { $$ = ফাঁকা; }
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
  #অন্তর্ভাব "muscle-tab.h"
  প্রকারলেখ কাঠামো
  {
    অক্ষর ধ্রুবক *chars;
    muscle_kind kind;
  } মান_type;
};
%প্রকার <মান_type> মান;
%printer
{
  নির্বাচন ($$.kind)
    {
    ক্ষেত্র muscle_code:    fprintf (yyo,  "{%s}",  $$.chars); বিরতি;
    ক্ষেত্র muscle_keyword: fprintf (yyo,   "%s",   $$.chars); বিরতি;
    ক্ষেত্র muscle_string:  fprintf (yyo, "\"%s\"", $$.chars); বিরতি;
    }
} <মান_type>;

মান:
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
    { $$ = প্রতীক_from_uniqstr ($1, @1); }
| CHAR_LITERAL
    {
      ধ্রুবক অক্ষর *var = "api.token.raw";
      যদি (current_class == nterm_sym)
        {
          complain (&@1, complaint,
                    _("character literals cannot be nonterminals"));
          YYERROR;
        }
      যদি (muscle_percent_define_ifdef (var))
        {
          complain (&@1, complaint,
                    _("character literals cannot be used together"
                    " with %s"), var);
          location loc = muscle_percent_define_get_loc (var);
          subcomplain (&loc, complaint, _("definition of %s"), var);
        }
      $$ = প্রতীক_get (char_name ($1), @1);
      প্রতীক_class_set ($$, token_sym, @1, false);
      প্রতীক_code_set ($$, $1, @1);
    }
;

id_colon:
  ID_COLON { $$ = প্রতীক_from_uniqstr ($1, @1); }
;


প্রতীক:
  id
| string_as_id
;

/* A string used as an ID.  */
string_as_id:
  STRING
    {
      $$ = প্রতীক_get ($1, @1);
      প্রতীক_class_set ($$, token_sym, @1, false);
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

পূর্ণসংখ্যা
yyreport_syntax_error (ধ্রুবক yypcontext_t *ctx)
{
  পূর্ণসংখ্যা res = 0;
  /* Arguments of format: reported tokens (one for the "unexpected",
     one per "expected"). */
  গণনা { ARGS_MAX = 5 };
  ধ্রুবক অক্ষর *argv[ARGS_MAX];
  পূর্ণসংখ্যা argc = 0;
  yysymbol_kind_t unexpected = yypcontext_token (ctx);
  যদি (unexpected != YYSYMBOL_YYEMPTY)
    {
      argv[argc++] = yysymbol_name (unexpected);
      yysymbol_kind_t expected[ARGS_MAX - 1];
      পূর্ণসংখ্যা nexpected = yypcontext_expected_tokens (ctx, expected, ARGS_MAX - 1);
      যদি (nexpected < 0)
        res = nexpected;
      অন্যথা
        জন্য (পূর্ণসংখ্যা i = 0; i < nexpected; ++i)
          argv[argc++] = yysymbol_name (expected[i]);
    }
  syntax_error (*yypcontext_location (ctx), argc, argv);
  ফেরত res;
}


/* Return the location of the left-hand side of a rule whose
   right-hand side is RHS[1] ... RHS[N].  Ignore empty nonterminals in
   the right-hand side, and return an empty location equal to the end
   boundary of RHS[0] if the right-hand side is empty.  */

স্থির YYLTYPE
lloc_default (YYLTYPE ধ্রুবক *rhs, পূর্ণসংখ্যা n)
{
  YYLTYPE loc;

  /* SGI MIPSpro 7.4.1m miscompiles "loc.start = loc.end = rhs[n].end;".
     The bug is fixed in 7.4.2m, but play it safe for now.  */
  loc.start = rhs[n].end;
  loc.end = rhs[n].end;

  /* Ignore empty nonterminals the start of the right-hand side.
     Do not bother to ignore them at the end of the right-hand side,
     since empty nonterminals have the same end as their predecessors.  */
  জন্য (পূর্ণসংখ্যা i = 1; i <= n; i++)
    যদি (! equal_boundaries (rhs[i].start, rhs[i].end))
      {
        loc.start = rhs[i].start;
        বিরতি;
      }

  ফেরত loc;
}

স্থির
অক্ষর *strip_braces (অক্ষর *code)
{
  code[strlen (code) - 1] = 0;
  ফেরত code + 1;
}

স্থির
অক্ষর ধ্রুবক *
translate_code (অক্ষর *code, location loc, bool plain)
{
  code_props plain_code;
  যদি (plain)
    code_props_plain_init (&plain_code, code, loc);
  অন্যথা
    code_props_প্রতীক_action_init (&plain_code, code, loc);
  code_props_translate_code (&plain_code);
  gram_scanner_last_string_free ();
  ফেরত plain_code.code;
}

স্থির
অক্ষর ধ্রুবক *
translate_code_braceless (অক্ষর *code, location loc)
{
  ফেরত translate_code (strip_braces (code), loc, true);
}

স্থির শূন্য
add_param (param_type প্রকার, অক্ষর *decl, location loc)
{
  স্থির অক্ষর ধ্রুবক alphanum[26 + 26 + 1 + 10 + 1] =
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "_"
    "0123456789";

  অক্ষর ধ্রুবক *name_start = ফাঁকা;
  {
    অক্ষর *p;
    /* Stop on last actual character.  */
    জন্য (p = decl; p[1]; p++)
      যদি ((p == decl
           || ! memchr (alphanum, p[-1], আকার alphanum - 1))
          && memchr (alphanum, p[0], আকার alphanum - 10 - 1))
        name_start = p;

    /* Strip the surrounding '{' and '}', and any blanks just inside
       the braces.  */
    --p;
    যখন (c_isspace ((unsigned অক্ষর) *p))
      --p;
    p[1] = '\0';
    ++decl;
    যখন (c_isspace ((unsigned অক্ষর) *decl))
      ++decl;
  }

  যদি (! name_start)
    complain (&loc, complaint, _("missing identifier in parameter declaration"));
  অন্যথা
    {
      অক্ষর *name = xmemdup0 (name_start, strspn (name_start, alphanum));
      যদি (প্রকার & param_lex)
        muscle_pair_list_grow ("lex_param", decl, name);
      যদি (প্রকার & param_parse)
        muscle_pair_list_grow ("parse_param", decl, name);
      মুক্ত (name);
    }

  gram_scanner_last_string_free ();
}


স্থির শূন্য
handle_header (অক্ষর ধ্রুবক *মান)
{
  header_flag = true;
  যদি (মান)
    {
      অক্ষর *file = unquote (মান);
      spec_header_file = xstrdup (file);
      gram_scanner_last_string_free ();
      unquote_free (file);
    }
}


স্থির শূন্য
handle_error_verbose (location ধ্রুবক *loc, অক্ষর ধ্রুবক *directive)
{
  bison_directive (loc, directive);
  muscle_percent_define_ঢোকাও (directive, *loc, muscle_keyword, "",
                                MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
}


স্থির শূন্য
handle_file_prefix (location ধ্রুবক *loc,
                    location ধ্রুবক *dir_loc,
                    অক্ষর ধ্রুবক *directive, অক্ষর ধ্রুবক *মান_quoted)
{
  অক্ষর *মান = unquote (মান_quoted);
  bison_directive (loc, directive);
  bool warned = false;

  যদি (location_empty (spec_file_prefix_loc))
    {
      spec_file_prefix_loc = *loc;
      spec_file_prefix = মান;
    }
  অন্যথা
    {
      duplicate_directive (directive, spec_file_prefix_loc, *loc);
      warned = true;
    }

  যদি (!warned
      && STRNEQ (directive, "%file-prefix"))
    deprecated_directive (dir_loc, directive, "%file-prefix");
}

স্থির শূন্য
handle_language (location ধ্রুবক *loc, অক্ষর ধ্রুবক *lang)
{
  language_argmatch (unquote (lang), grammar_prio, *loc);
}


স্থির শূন্য
handle_name_prefix (location ধ্রুবক *loc,
                    অক্ষর ধ্রুবক *directive, অক্ষর ধ্রুবক *মান_quoted)
{
  অক্ষর *মান = unquote (মান_quoted);
  bison_directive (loc, directive);

  অক্ষর buf1[1024];
  size_t len1 = আকার (buf1);
  অক্ষর *old = asnprintf (buf1, &len1, "%s\"%s\"", directive, মান);
  যদি (!old)
    xalloc_die ();

  যদি (location_empty (spec_name_prefix_loc))
    {
      spec_name_prefix = মান;
      spec_name_prefix_loc = *loc;

      অক্ষর buf2[1024];
      size_t len2 = আকার (buf2);
      অক্ষর *new = asnprintf (buf2, &len2, "%%define api.prefix {%s}", মান);
      যদি (!new)
        xalloc_die ();
      deprecated_directive (loc, old, new);
      যদি (new != buf2)
        মুক্ত (new);
    }
  অন্যথা
    duplicate_directive (old, spec_file_prefix_loc, *loc);

  যদি (old != buf1)
    মুক্ত (old);
}


স্থির শূন্য
handle_pure_parser (location ধ্রুবক *loc, অক্ষর ধ্রুবক *directive)
{
  bison_directive (loc, directive);
  deprecated_directive (loc, directive, "%define api.pure");
  muscle_percent_define_ঢোকাও ("api.pure", *loc, muscle_keyword, "",
                                MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
}


স্থির শূন্য
handle_require (location ধ্রুবক *loc, অক্ষর ধ্রুবক *version_quoted)
{
  অক্ষর *version = unquote (version_quoted);
  required_version = strversion_to_int (version);
  যদি (required_version == -1)
    {
      complain (loc, complaint, _("invalid version requirement: %s"),
                version);
      required_version = 0;
    }
  অন্যথা
    {
      ধ্রুবক অক্ষর* package_version =
        0 < strverscmp (api_version, PACKAGE_VERSION)
        ? api_version : PACKAGE_VERSION;
      যদি (0 < strverscmp (version, package_version))
        {
          complain (loc, complaint, _("require bison %s, but have %s"),
                    version, package_version);
          exit (EX_MISMATCH);
        }
    }
  unquote_free (version);
  gram_scanner_last_string_free ();
}

স্থির শূন্য
handle_skeleton (location ধ্রুবক *loc, অক্ষর ধ্রুবক *skel_quoted)
{
  অক্ষর *skel = unquote (skel_quoted);
  অক্ষর ধ্রুবক *skeleton_user = skel;
  যদি (strchr (skeleton_user, '/'))
    {
      size_t dir_দৈর্ঘ্য = strlen (grammar_file);
      যখন (dir_দৈর্ঘ্য && grammar_file[dir_length - 1] != '/')
        --dir_length;
      যখন (dir_দৈর্ঘ্য && grammar_file[dir_length - 1] == '/')
        --dir_length;
      অক্ষর *skeleton_build =
        xmalloc (dir_দৈর্ঘ্য + 1 + strlen (skeleton_user) + 1);
      যদি (dir_দৈর্ঘ্য > 0)
        {
          memcpy (skeleton_build, grammar_file, dir_দৈর্ঘ্য);
          skeleton_build[dir_length++] = '/';
        }
      strcpy (skeleton_build + dir_দৈর্ঘ্য, skeleton_user);
      skeleton_user = uniqstr_new (skeleton_build);
      মুক্ত (skeleton_build);
    }
  skeleton_arg (skeleton_user, grammar_prio, *loc);
}


স্থির শূন্য
handle_yacc (location ধ্রুবক *loc)
{
  ধ্রুবক অক্ষর *directive = "%yacc";
  bison_directive (loc, directive);
  যদি (location_empty (yacc_loc))
    set_yacc (*loc);
  অন্যথা
    duplicate_directive (directive, yacc_loc, *loc);
}


স্থির শূন্য
gram_error (location ধ্রুবক *loc, অক্ষর ধ্রুবক *বার্তা)
{
  complain (loc, complaint, "%s", বার্তা);
}

স্থির অক্ষর ধ্রুবক *
char_name (অক্ষর c)
{
  যদি (c == '\'')
    ফেরত "'\\''";
  অন্যথা
    {
      অক্ষর buf[4];
      buf[0] = '\''; buf[1] = c; buf[2] = '\''; buf[3] = '\0';
      ফেরত quotearg_style (escape_quoting_style, buf);
    }
}

স্থির শূন্য
current_lhs (প্রতীক *sym, location loc, named_ref *ref)
{
  current_lhs_প্রতীক = sym;
  current_lhs_loc = loc;
  যদি (sym)
    প্রতীক_location_as_lhs_set (sym, loc);
  /* In order to simplify memory management, named references for lhs
     are always assigned by deep copy into the current symbol_list
     node.  This is because a single named-ref in the grammar may
     result in several uses when the user factors lhs between several
     rules using "|".  Therefore free the parser's original copy.  */
  মুক্ত (current_lhs_named_ref);
  current_lhs_named_ref = ref;
}

স্থির শূন্য tron (FILE *yyo)
{
  begin_use_class ("value", yyo);
}

স্থির শূন্য troff (FILE *yyo)
{
  end_use_class ("value", yyo);
}


/*----------.
| Unquote.  |
`----------*/

কাঠামো obstack obstack_for_unquote;

শূন্য
parser_init (শূন্য)
{
  obstack_init (&obstack_for_unquote);
}

শূন্য
parser_free (শূন্য)
{
  obstack_free (&obstack_for_unquote, 0);
}

স্থির শূন্য
unquote_free (অক্ষর *last_string)
{
  obstack_free (&obstack_for_unquote, last_string);
}

স্থির অক্ষর *
unquote (ধ্রুবক অক্ষর *cp)
{
#define GROW(Char)                              \
  obstack_1grow (&obstack_for_unquote, Char);
  জন্য (++cp; *cp && *cp != '"'; ++cp)
    নির্বাচন (*cp)
      {
      ক্ষেত্র '"':
        বিরতি;
      ক্ষেত্র '\\':
        ++cp;
        নির্বাচন (*cp)
          {
          ক্ষেত্র '0': ক্ষেত্র '1': ক্ষেত্র '2': ক্ষেত্র '3': ক্ষেত্র '4':
          ক্ষেত্র '5': ক্ষেত্র '6': ক্ষেত্র '7': ক্ষেত্র '8': ক্ষেত্র '9':
            {
              পূর্ণসংখ্যা c = cp[0] - '0';
              যদি (c_isdigit (cp[1]))
                {
                  ++cp;
                  c = c * 8 + cp[0] - '0';
                }
              যদি (c_isdigit (cp[1]))
                {
                  ++cp;
                  c = c * 8 + cp[0] - '0';
                }
              GROW (c);
            }
            বিরতি;

          ক্ষেত্র 'a': GROW ('\a'); বিরতি;
          ক্ষেত্র 'b': GROW ('\b'); বিরতি;
          ক্ষেত্র 'f': GROW ('\f'); বিরতি;
          ক্ষেত্র 'n': GROW ('\n'); বিরতি;
          ক্ষেত্র 'r': GROW ('\r'); বিরতি;
          ক্ষেত্র 't': GROW ('\t'); বিরতি;
          ক্ষেত্র 'v': GROW ('\v'); বিরতি;

          ক্ষেত্র 'x':
            {
              পূর্ণসংখ্যা c = 0;
              যখন (c_isxdigit (cp[1]))
                {
                  ++cp;
                  c = (c * 16 + (c_isdigit (cp[0]) ? cp[0] - '0'
                                 : c_isupper (cp[0]) ? cp[0] - 'A'
                                 : cp[0] - '0'));
                }
              GROW (c);
              বিরতি;
            }
          }
        বিরতি;

      পূর্বনির্ধারিত:
        GROW (*cp);
        বিরতি;
      }
  প্রতিজ্ঞা (*cp == '"');
  ++cp;
  প্রতিজ্ঞা (*cp == '\0');
#undef GROW
  ফেরত obstack_finish0 (&obstack_for_unquote);
}
