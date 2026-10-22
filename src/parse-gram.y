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
  #അന്തര്ഭാവ "symlist.h"
  #അന്തര്ഭാവ "symtab.h"
}

%code provides
{
  /* Initialize unquote.  */
  ശൂന്യം parser_init (ശൂന്യം);
  /* Deallocate storage for unquote.  */
  ശൂന്യം parser_free (ശൂന്യം);
}

%code മുകൾ_2
{
  /* On column 0 to please syntax-check.  */
#അന്തര്ഭാവ <config.h>
}

%code
{
  #അന്തര്ഭാവ "system.h"

  #അന്തര്ഭാവ <c-ctype.h>
  #അന്തര്ഭാവ <quotearg.h>
  #അന്തര്ഭാവ <vasnprintf.h>
  #അന്തര്ഭാവ <xmemdup0.h>

  #അന്തര്ഭാവ "complain.h"
  #അന്തര്ഭാവ "conflicts.h"
  #അന്തര്ഭാവ "files.h"
  #അന്തര്ഭാവ "getargs.h"
  #അന്തര്ഭാവ "gram.h"
  #അന്തര്ഭാവ "named-ref.h"
  #അന്തര്ഭാവ "reader.h"
  #അന്തര്ഭാവ "scan-code.h"
  #അന്തര്ഭാവ "scan-gram.h"
  #അന്തര്ഭാവ "strversion.h"

  /* Pretend to be at least that version, to check features published
     in that version while developping it.  */
  സ്ഥിതം സ്ഥിരം അക്ഷരം* api_version = "3.8";

  സ്ഥിതം പൂർണ്ണസംഖ്യ current_prec = 0;
  സ്ഥിതം location current_lhs_loc;
  സ്ഥിതം named_ref *current_lhs_named_ref;
  സ്ഥിതം ചിഹ്നം *current_lhs_ചിഹ്നം;
  സ്ഥിതം ചിഹ്നം_class current_class = unknown_sym;

  /** Set the new current left-hand side symbol, possibly common
   * to several right-hand side parts of rule.
   */
  സ്ഥിതം ശൂന്യം current_lhs (ചിഹ്നം *sym, location loc, named_ref *ref);

  #define YYLLOC_DEFAULT(Current, Rhs, N)         \
    (Current) = lloc_default (Rhs, N)
  സ്ഥിതം YYLTYPE lloc_default (YYLTYPE സ്ഥിരം *, പൂർണ്ണസംഖ്യ);

  #define YY_LOCATION_PRINT(File, Loc)            \
    location_print (Loc, File)

  /* Strip initial '{' and final '}' (must be first and last characters).
     Return the result.  */
  സ്ഥിതം അക്ഷരം *strip_braces (അക്ഷരം *code);

  /* Convert CODE by calling code_props_plain_init if PLAIN, otherwise
     code_props_symbol_action_init.  Calls
     gram_scanner_last_string_free to release the latest string from
     the scanner (should be CODE). */
  സ്ഥിതം അക്ഷരം സ്ഥിരം *translate_code (അക്ഷരം *code, location loc, bool plain);

  /* Convert CODE by calling code_props_plain_init after having
     stripped the first and last characters (expected to be '{', and
     '}').  Calls gram_scanner_last_string_free to release the latest
     string from the scanner (should be CODE). */
  സ്ഥിതം അക്ഷരം സ്ഥിരം *translate_code_braceless (അക്ഷരം *code, location loc);

  /* Handle a %header directive.  */
  സ്ഥിതം ശൂന്യം handle_header (അക്ഷരം സ്ഥിരം *മൂല്യം);

  /* Handle a %error-verbose directive.  */
  സ്ഥിതം ശൂന്യം handle_error_verbose (location സ്ഥിരം *loc, അക്ഷരം സ്ഥിരം *directive);

  /* Handle a %file-prefix directive.  */
  സ്ഥിതം ശൂന്യം handle_file_prefix (location സ്ഥിരം *loc,
                                  location സ്ഥിരം *dir_loc,
                                  അക്ഷരം സ്ഥിരം *directive, അക്ഷരം സ്ഥിരം *മൂല്യം);

  /* Handle a %language directive.  */
  സ്ഥിതം ശൂന്യം handle_language (location സ്ഥിരം *loc, അക്ഷരം സ്ഥിരം *lang);

  /* Handle a %name-prefix directive.  */
  സ്ഥിതം ശൂന്യം handle_name_prefix (location സ്ഥിരം *loc,
                                  അക്ഷരം സ്ഥിരം *directive, അക്ഷരം സ്ഥിരം *മൂല്യം);

  /* Handle a %pure-parser directive.  */
  സ്ഥിതം ശൂന്യം handle_pure_parser (location സ്ഥിരം *loc, അക്ഷരം സ്ഥിരം *directive);

  /* Handle a %require directive.  */
  സ്ഥിതം ശൂന്യം handle_require (location സ്ഥിരം *loc, അക്ഷരം സ്ഥിരം *version);

  /* Handle a %skeleton directive.  */
  സ്ഥിതം ശൂന്യം handle_skeleton (location സ്ഥിരം *loc, അക്ഷരം സ്ഥിരം *skel);

  /* Handle a %yacc directive.  */
  സ്ഥിതം ശൂന്യം handle_yacc (location സ്ഥിരം *loc);

  /* Implementation of yyerror.  */
  സ്ഥിതം ശൂന്യം gram_error (location സ്ഥിരം *, അക്ഷരം സ്ഥിരം *);

  /* A string that describes a char (e.g., 'a' -> "'a'").  */
  സ്ഥിതം അക്ഷരം സ്ഥിരം *char_name (അക്ഷരം);

  /* Add style to semantic values in traces.  */
  സ്ഥിതം ശൂന്യം tron (FILE *yyo);
  സ്ഥിതം ശൂന്യം troff (FILE *yyo);

  /* Interpret a quoted string (such as `"Hello, \"World\"\n\""`).
     Manages the memory of the result.  */
  സ്ഥിതം അക്ഷരം *unquote (സ്ഥിരം അക്ഷരം *str);

  /* Discard the latest unquoted string.  */
  സ്ഥിതം ശൂന്യം unquote_free (അക്ഷരം *last_string);
}

%define api.header.include {"parse-gram.h"}
%define api.prefix {gram_}
%define api.pure full
%define api.token.raw
%define api.മൂല്യം.തരം സംഘം
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

  PERCENT_ഇടത്        "%left"
  PERCENT_വലത്       "%right"
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
  PERCENT_NO_വരികൾ        "%no-lines"
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

%തരം <unsigned അക്ഷരം> CHAR_LITERAL
%printer { fputs (char_name ($$), yyo); } <unsigned അക്ഷരം>

%തരം <അക്ഷരം*> "{...}" "%?{...}" "%{...%}" EPILOGUE STRING TSTRING
%printer { fputs ($$, yyo); } <അക്ഷരം*>

%തരം <uniqstr>
  BRACKETED_ID ID ID_COLON
  PERCENT_ERROR_VERBOSE PERCENT_FILE_PREFIX PERCENT_FLAG PERCENT_NAME_PREFIX
  PERCENT_PURE_PARSER
  TAG tag tag.opt variable
%printer { fputs ($$, yyo); } <uniqstr>
%printer { fprintf (yyo, "[%s]", $$); } BRACKETED_ID
%printer { fprintf (yyo, "%s:", $$); } ID_COLON
%printer { fprintf (yyo, "%%%s", $$); } PERCENT_FLAG
%printer { fprintf (yyo, "<%s>", $$); } TAG tag

%token <പൂർണ്ണസംഖ്യ> INT_LITERAL _("integer literal")
%printer { fprintf (yyo, "%d", $$); } <പൂർണ്ണസംഖ്യ>

%തരം <ചിഹ്നം*> id id_colon string_as_id ചിഹ്നം token_decl token_decl_for_prec
%printer { fprintf (yyo, "%s", $$ ? $$->tag : "<NULL>"); } <ചിഹ്നം*>
%printer { fprintf (yyo, "%s:", $$->tag); } id_colon

%തരം <assoc> precedence_declarator

%destructor { ചിഹ്നം_list_free ($$); } <ചിഹ്നം_list*>
%printer { ചിഹ്നം_list_syms_print ($$, yyo); } <ചിഹ്നം_list*>

%തരം <named_ref*> named_ref.opt

/*---------.
| %param.  |
`---------*/
%code requires
{
  തരനിർവചനം എണ്ണൽ
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
  സ്ഥിതം ശൂന്യം add_param (param_type തരം, അക്ഷരം *decl, location loc);
  സ്ഥിതം param_type current_param = param_none;
};
%token <param_type> PERCENT_PARAM "%param";
%printer
{
  തിരഞ്ഞെടുപ്പ് ($$)
    {
#define CASE(In, Out)                                           \
      അവസ്ഥ param_ ## In: fputs ("%" #Out, yyo); നിർത്തുക
      CASE (lex,   lex-param);
      CASE (parse, parse-param);
      CASE (both,  param);
#undef CASE
      അവസ്ഥ param_none: aver (false); നിർത്തുക;
    }
} <param_type>;


                     /*==========\
                     | Grammar.  |
                     \==========*/
%%

നിവേശം:
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
| "%define" variable മൂല്യം
    {
      muscle_percent_define_തിരുകുക ($2, @$, $3.kind, $3.chars,
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
| "%no-lines"                   { no_വരികൾ_flag = true; }
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
  ചിഹ്നം_declaration
| "%start" symbols.1
    {
      grammar_start_symbols_add ($2);
    }
| code_props_type "{...}" generic_symlist
    {
      code_props code;
      code_props_ചിഹ്നം_action_init (&code, $2, @2);
      code_props_translate_code (&code);
      {
        വേണ്ടി (ചിഹ്നം_list *list = $3; list; list = list->next)
          ചിഹ്നം_list_code_props_set (list, $1, &code);
        ചിഹ്നം_list_free ($3);
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

%തരം <code_props_type> code_props_type;
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
| ID     { muscle_percent_define_തിരുകുക ("api.value.union.name",
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


%തരം <ചിഹ്നം_list*> nterm_decls ചിഹ്നം_decls symbols.1
      token_decls token_decls_for_prec
      token_decl.1 token_decl_for_prec.1;
ചിഹ്നം_declaration:
  "%nterm" { current_class = nterm_sym; } nterm_decls[syms]
    {
      current_class = unknown_sym;
      ചിഹ്നം_list_free ($syms);
    }
| "%token" { current_class = token_sym; } token_decls[syms]
    {
      current_class = unknown_sym;
      ചിഹ്നം_list_free ($syms);
    }
| "%type" { current_class = pct_type_sym; } ചിഹ്നം_decls[syms]
    {
      current_class = unknown_sym;
      ചിഹ്നം_list_free ($syms);
    }
| precedence_declarator token_decls_for_prec[syms]
    {
      ++current_prec;
      വേണ്ടി (ചിഹ്നം_list *list = $syms; list; list = list->next)
        ചിഹ്നം_precedence_set (list->content.sym, current_prec, $1, @1);
      ചിഹ്നം_list_free ($syms);
    }
;

precedence_declarator:
  "%left"       { $$ = ഇടത്_assoc; }
| "%right"      { $$ = വലത്_assoc; }
| "%nonassoc"   { $$ = non_assoc; }
| "%precedence" { $$ = precedence_assoc; }
;

%തരം <അക്ഷരം*> string.opt;
string.opt:
  %empty  { $$ = ശൂന്യ; }
| STRING  { $$ = $1; }
;

tag.opt:
  %empty { $$ = ശൂന്യ; }
| TAG    { $$ = $1; }
;

%തരം <ചിഹ്നം_list*> generic_symlist generic_symlist_item;
generic_symlist:
  generic_symlist_item
| generic_symlist generic_symlist_item   { $$ = ചിഹ്നം_list_append ($1, $2); }
;

generic_symlist_item:
  ചിഹ്നം    { $$ = ചിഹ്നം_list_sym_new ($1, @1); }
| tag       { $$ = ചിഹ്നം_list_type_new ($1, @1); }
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
      $$ = ചിഹ്നം_list_type_set ($syms, $TAG);
    }
| token_decls TAG token_decl.1[syms]
    {
      $$ = ചിഹ്നം_list_append ($1, ചിഹ്നം_list_type_set ($syms, $TAG));
    }
;

// One or more symbol declarations for %token or %nterm.
token_decl.1:
  token_decl                { $$ = ചിഹ്നം_list_sym_new ($1, @1); }
| token_decl.1 token_decl   { $$ = ചിഹ്നം_list_append ($1, ചിഹ്നം_list_sym_new ($2, @2)); }

// One symbol declaration for %token or %nterm.
token_decl:
  id പൂർണ്ണസംഖ്യ.opt[num] alias
    {
      $$ = $id;
      ചിഹ്നം_class_set ($id, current_class, @id, true);
      എങ്കിൽ (0 <= $അക്കം)
        ചിഹ്നം_code_set ($id, $അക്കം, @അക്കം);
      എങ്കിൽ ($alias)
        ചിഹ്നം_make_alias ($id, $alias, @alias);
    }
;

%തരം <പൂർണ്ണസംഖ്യ> പൂർണ്ണസംഖ്യ.opt;
പൂർണ്ണസംഖ്യ.opt:
  %empty  { $$ = -1; }
| INT_LITERAL
;

%തരം <ചിഹ്നം*> alias;
alias:
  %empty         { $$ = ശൂന്യ; }
| string_as_id   { $$ = $1; }
| TSTRING
    {
      $$ = ചിഹ്നം_get ($1, @1);
      ചിഹ്നം_class_set ($$, token_sym, @1, false);
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
      $$ = ചിഹ്നം_list_type_set ($syms, $TAG);
    }
| token_decls_for_prec TAG token_decl_for_prec.1[syms]
    {
      $$ = ചിഹ്നം_list_append ($1, ചിഹ്നം_list_type_set ($syms, $TAG));
    }
;

// One or more token declarations for precedence declaration.
token_decl_for_prec.1:
  token_decl_for_prec
    { $$ = ചിഹ്നം_list_sym_new ($1, @1); }
| token_decl_for_prec.1 token_decl_for_prec
    { $$ = ചിഹ്നം_list_append ($1, ചിഹ്നം_list_sym_new ($2, @2)); }

// One token declaration for precedence declaration.
token_decl_for_prec:
  id പൂർണ്ണസംഖ്യ.opt[num]
    {
      $$ = $id;
      ചിഹ്നം_class_set ($id, token_sym, @id, false);
      എങ്കിൽ (0 <= $അക്കം)
        ചിഹ്നം_code_set ($id, $അക്കം, @അക്കം);
    }
| string_as_id
;


/*-----------------------------------.
| symbol_decls (argument of %type).  |
`-----------------------------------*/

// A non empty list of typed symbols (for %type).
ചിഹ്നം_decls:
  symbols.1[syms]
    {
      $$ = $syms;
    }
| TAG symbols.1[syms]
    {
      $$ = ചിഹ്നം_list_type_set ($syms, $TAG);
    }
| ചിഹ്നം_decls TAG symbols.1[syms]
    {
      $$ = ചിഹ്നം_list_append ($1, ചിഹ്നം_list_type_set ($syms, $TAG));
    }
;

// One or more symbols.
symbols.1:
  ചിഹ്നം
    {
      എങ്കിൽ (current_class != unknown_sym)
        ചിഹ്നം_class_set ($ചിഹ്നം, current_class, @ചിഹ്നം, false);
      $$ = ചിഹ്നം_list_sym_new ($ചിഹ്നം, @ചിഹ്നം);
    }
  | symbols.1 ചിഹ്നം
    {
      എങ്കിൽ (current_class != unknown_sym)
        ചിഹ്നം_class_set ($ചിഹ്നം, current_class, @ചിഹ്നം, false);
      $$ = ചിഹ്നം_list_append ($1, ചിഹ്നം_list_sym_new ($ചിഹ്നം, @ചിഹ്നം));
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
    { grammar_current_rule_begin (current_lhs_ചിഹ്നം, current_lhs_loc,
                                  current_lhs_named_ref); }
| rhs ചിഹ്നം named_ref.opt
    { grammar_current_rule_ചിഹ്നം_append ($2, @2, $3); }
| rhs tag.opt "{...}"[action] named_ref.opt[name]
    { grammar_current_rule_action_append ($action, @action, $name, $[tag.opt]); }
| rhs "%?{...}"
    { grammar_current_rule_predicate_append ($2, @2); }
| rhs "%empty"
    { grammar_current_rule_empty_set (@2); }
| rhs "%prec" ചിഹ്നം
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
  %empty         { $$ = ശൂന്യ; }
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
  #അന്തര്ഭാവ "muscle-tab.h"
  തരനിർവചനം ഘടന
  {
    അക്ഷരം സ്ഥിരം *chars;
    muscle_kind kind;
  } മൂല്യം_type;
};
%തരം <മൂല്യം_type> മൂല്യം;
%printer
{
  തിരഞ്ഞെടുപ്പ് ($$.kind)
    {
    അവസ്ഥ muscle_code:    fprintf (yyo,  "{%s}",  $$.chars); നിർത്തുക;
    അവസ്ഥ muscle_keyword: fprintf (yyo,   "%s",   $$.chars); നിർത്തുക;
    അവസ്ഥ muscle_string:  fprintf (yyo, "\"%s\"", $$.chars); നിർത്തുക;
    }
} <മൂല്യം_type>;

മൂല്യം:
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
    { $$ = ചിഹ്നം_from_uniqstr ($1, @1); }
| CHAR_LITERAL
    {
      സ്ഥിരം അക്ഷരം *var = "api.token.raw";
      എങ്കിൽ (current_class == nterm_sym)
        {
          complain (&@1, complaint,
                    _("character literals cannot be nonterminals"));
          YYERROR;
        }
      എങ്കിൽ (muscle_percent_define_ifdef (var))
        {
          complain (&@1, complaint,
                    _("character literals cannot be used together"
                    " with %s"), var);
          location loc = muscle_percent_define_get_loc (var);
          subcomplain (&loc, complaint, _("definition of %s"), var);
        }
      $$ = ചിഹ്നം_get (char_name ($1), @1);
      ചിഹ്നം_class_set ($$, token_sym, @1, false);
      ചിഹ്നം_code_set ($$, $1, @1);
    }
;

id_colon:
  ID_COLON { $$ = ചിഹ്നം_from_uniqstr ($1, @1); }
;


ചിഹ്നം:
  id
| string_as_id
;

/* A string used as an ID.  */
string_as_id:
  STRING
    {
      $$ = ചിഹ്നം_get ($1, @1);
      ചിഹ്നം_class_set ($$, token_sym, @1, false);
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

പൂർണ്ണസംഖ്യ
yyreport_syntax_error (സ്ഥിരം yypcontext_t *ctx)
{
  പൂർണ്ണസംഖ്യ res = 0;
  /* Arguments of format: reported tokens (one for the "unexpected",
     one per "expected"). */
  എണ്ണൽ { ARGS_MAX = 5 };
  സ്ഥിരം അക്ഷരം *argv[ARGS_MAX];
  പൂർണ്ണസംഖ്യ argc = 0;
  yysymbol_kind_t unexpected = yypcontext_token (ctx);
  എങ്കിൽ (unexpected != YYSYMBOL_YYEMPTY)
    {
      argv[argc++] = yysymbol_name (unexpected);
      yysymbol_kind_t expected[ARGS_MAX - 1];
      പൂർണ്ണസംഖ്യ nexpected = yypcontext_expected_tokens (ctx, expected, ARGS_MAX - 1);
      എങ്കിൽ (nexpected < 0)
        res = nexpected;
      അല്ലെങ്കിൽ
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < nexpected; ++i)
          argv[argc++] = yysymbol_name (expected[i]);
    }
  syntax_error (*yypcontext_location (ctx), argc, argv);
  തിരികെ res;
}


/* Return the location of the left-hand side of a rule whose
   right-hand side is RHS[1] ... RHS[N].  Ignore empty nonterminals in
   the right-hand side, and return an empty location equal to the end
   boundary of RHS[0] if the right-hand side is empty.  */

സ്ഥിതം YYLTYPE
lloc_default (YYLTYPE സ്ഥിരം *rhs, പൂർണ്ണസംഖ്യ n)
{
  YYLTYPE loc;

  /* SGI MIPSpro 7.4.1m miscompiles "loc.start = loc.end = rhs[n].end;".
     The bug is fixed in 7.4.2m, but play it safe for now.  */
  loc.start = rhs[n].end;
  loc.end = rhs[n].end;

  /* Ignore empty nonterminals the start of the right-hand side.
     Do not bother to ignore them at the end of the right-hand side,
     since empty nonterminals have the same end as their predecessors.  */
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 1; i <= n; i++)
    എങ്കിൽ (! equal_boundaries (rhs[i].start, rhs[i].end))
      {
        loc.start = rhs[i].start;
        നിർത്തുക;
      }

  തിരികെ loc;
}

സ്ഥിതം
അക്ഷരം *strip_braces (അക്ഷരം *code)
{
  code[strlen (code) - 1] = 0;
  തിരികെ code + 1;
}

സ്ഥിതം
അക്ഷരം സ്ഥിരം *
translate_code (അക്ഷരം *code, location loc, bool plain)
{
  code_props plain_code;
  എങ്കിൽ (plain)
    code_props_plain_init (&plain_code, code, loc);
  അല്ലെങ്കിൽ
    code_props_ചിഹ്നം_action_init (&plain_code, code, loc);
  code_props_translate_code (&plain_code);
  gram_scanner_last_string_free ();
  തിരികെ plain_code.code;
}

സ്ഥിതം
അക്ഷരം സ്ഥിരം *
translate_code_braceless (അക്ഷരം *code, location loc)
{
  തിരികെ translate_code (strip_braces (code), loc, true);
}

സ്ഥിതം ശൂന്യം
add_param (param_type തരം, അക്ഷരം *decl, location loc)
{
  സ്ഥിതം അക്ഷരം സ്ഥിരം alphanum[26 + 26 + 1 + 10 + 1] =
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "_"
    "0123456789";

  അക്ഷരം സ്ഥിരം *name_start = ശൂന്യ;
  {
    അക്ഷരം *p;
    /* Stop on last actual character.  */
    വേണ്ടി (p = decl; p[1]; p++)
      എങ്കിൽ ((p == decl
           || ! memchr (alphanum, p[-1], വലുപ്പം alphanum - 1))
          && memchr (alphanum, p[0], വലുപ്പം alphanum - 10 - 1))
        name_start = p;

    /* Strip the surrounding '{' and '}', and any blanks just inside
       the braces.  */
    --p;
    ആകുമ്പോൾ (c_isspace ((unsigned അക്ഷരം) *p))
      --p;
    p[1] = '\0';
    ++decl;
    ആകുമ്പോൾ (c_isspace ((unsigned അക്ഷരം) *decl))
      ++decl;
  }

  എങ്കിൽ (! name_start)
    complain (&loc, complaint, _("missing identifier in parameter declaration"));
  അല്ലെങ്കിൽ
    {
      അക്ഷരം *name = xmemdup0 (name_start, strspn (name_start, alphanum));
      എങ്കിൽ (തരം & param_lex)
        muscle_pair_list_grow ("lex_param", decl, name);
      എങ്കിൽ (തരം & param_parse)
        muscle_pair_list_grow ("parse_param", decl, name);
      സ്വതന്ത്രം (name);
    }

  gram_scanner_last_string_free ();
}


സ്ഥിതം ശൂന്യം
handle_header (അക്ഷരം സ്ഥിരം *മൂല്യം)
{
  header_flag = true;
  എങ്കിൽ (മൂല്യം)
    {
      അക്ഷരം *file = unquote (മൂല്യം);
      spec_header_file = xstrdup (file);
      gram_scanner_last_string_free ();
      unquote_free (file);
    }
}


സ്ഥിതം ശൂന്യം
handle_error_verbose (location സ്ഥിരം *loc, അക്ഷരം സ്ഥിരം *directive)
{
  bison_directive (loc, directive);
  muscle_percent_define_തിരുകുക (directive, *loc, muscle_keyword, "",
                                MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
}


സ്ഥിതം ശൂന്യം
handle_file_prefix (location സ്ഥിരം *loc,
                    location സ്ഥിരം *dir_loc,
                    അക്ഷരം സ്ഥിരം *directive, അക്ഷരം സ്ഥിരം *മൂല്യം_quoted)
{
  അക്ഷരം *മൂല്യം = unquote (മൂല്യം_quoted);
  bison_directive (loc, directive);
  bool warned = false;

  എങ്കിൽ (location_empty (spec_file_prefix_loc))
    {
      spec_file_prefix_loc = *loc;
      spec_file_prefix = മൂല്യം;
    }
  അല്ലെങ്കിൽ
    {
      duplicate_directive (directive, spec_file_prefix_loc, *loc);
      warned = true;
    }

  എങ്കിൽ (!warned
      && STRNEQ (directive, "%file-prefix"))
    deprecated_directive (dir_loc, directive, "%file-prefix");
}

സ്ഥിതം ശൂന്യം
handle_language (location സ്ഥിരം *loc, അക്ഷരം സ്ഥിരം *lang)
{
  language_argmatch (unquote (lang), grammar_prio, *loc);
}


സ്ഥിതം ശൂന്യം
handle_name_prefix (location സ്ഥിരം *loc,
                    അക്ഷരം സ്ഥിരം *directive, അക്ഷരം സ്ഥിരം *മൂല്യം_quoted)
{
  അക്ഷരം *മൂല്യം = unquote (മൂല്യം_quoted);
  bison_directive (loc, directive);

  അക്ഷരം buf1[1024];
  size_t len1 = വലുപ്പം (buf1);
  അക്ഷരം *old = asnprintf (buf1, &len1, "%s\"%s\"", directive, മൂല്യം);
  എങ്കിൽ (!old)
    xalloc_die ();

  എങ്കിൽ (location_empty (spec_name_prefix_loc))
    {
      spec_name_prefix = മൂല്യം;
      spec_name_prefix_loc = *loc;

      അക്ഷരം buf2[1024];
      size_t len2 = വലുപ്പം (buf2);
      അക്ഷരം *new = asnprintf (buf2, &len2, "%%define api.prefix {%s}", മൂല്യം);
      എങ്കിൽ (!new)
        xalloc_die ();
      deprecated_directive (loc, old, new);
      എങ്കിൽ (new != buf2)
        സ്വതന്ത്രം (new);
    }
  അല്ലെങ്കിൽ
    duplicate_directive (old, spec_file_prefix_loc, *loc);

  എങ്കിൽ (old != buf1)
    സ്വതന്ത്രം (old);
}


സ്ഥിതം ശൂന്യം
handle_pure_parser (location സ്ഥിരം *loc, അക്ഷരം സ്ഥിരം *directive)
{
  bison_directive (loc, directive);
  deprecated_directive (loc, directive, "%define api.pure");
  muscle_percent_define_തിരുകുക ("api.pure", *loc, muscle_keyword, "",
                                MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
}


സ്ഥിതം ശൂന്യം
handle_require (location സ്ഥിരം *loc, അക്ഷരം സ്ഥിരം *version_quoted)
{
  അക്ഷരം *version = unquote (version_quoted);
  required_version = strversion_to_int (version);
  എങ്കിൽ (required_version == -1)
    {
      complain (loc, complaint, _("invalid version requirement: %s"),
                version);
      required_version = 0;
    }
  അല്ലെങ്കിൽ
    {
      സ്ഥിരം അക്ഷരം* package_version =
        0 < strverscmp (api_version, PACKAGE_VERSION)
        ? api_version : PACKAGE_VERSION;
      എങ്കിൽ (0 < strverscmp (version, package_version))
        {
          complain (loc, complaint, _("require bison %s, but have %s"),
                    version, package_version);
          exit (EX_MISMATCH);
        }
    }
  unquote_free (version);
  gram_scanner_last_string_free ();
}

സ്ഥിതം ശൂന്യം
handle_skeleton (location സ്ഥിരം *loc, അക്ഷരം സ്ഥിരം *skel_quoted)
{
  അക്ഷരം *skel = unquote (skel_quoted);
  അക്ഷരം സ്ഥിരം *skeleton_user = skel;
  എങ്കിൽ (strchr (skeleton_user, '/'))
    {
      size_t dir_length = strlen (grammar_file);
      ആകുമ്പോൾ (dir_length && grammar_file[dir_length - 1] != '/')
        --dir_length;
      ആകുമ്പോൾ (dir_length && grammar_file[dir_length - 1] == '/')
        --dir_length;
      അക്ഷരം *skeleton_build =
        xmalloc (dir_length + 1 + strlen (skeleton_user) + 1);
      എങ്കിൽ (dir_length > 0)
        {
          memcpy (skeleton_build, grammar_file, dir_length);
          skeleton_build[dir_length++] = '/';
        }
      strcpy (skeleton_build + dir_length, skeleton_user);
      skeleton_user = uniqstr_new (skeleton_build);
      സ്വതന്ത്രം (skeleton_build);
    }
  skeleton_arg (skeleton_user, grammar_prio, *loc);
}


സ്ഥിതം ശൂന്യം
handle_yacc (location സ്ഥിരം *loc)
{
  സ്ഥിരം അക്ഷരം *directive = "%yacc";
  bison_directive (loc, directive);
  എങ്കിൽ (location_empty (yacc_loc))
    set_yacc (*loc);
  അല്ലെങ്കിൽ
    duplicate_directive (directive, yacc_loc, *loc);
}


സ്ഥിതം ശൂന്യം
gram_error (location സ്ഥിരം *loc, അക്ഷരം സ്ഥിരം *സന്ദേശം)
{
  complain (loc, complaint, "%s", സന്ദേശം);
}

സ്ഥിതം അക്ഷരം സ്ഥിരം *
char_name (അക്ഷരം c)
{
  എങ്കിൽ (c == '\'')
    തിരികെ "'\\''";
  അല്ലെങ്കിൽ
    {
      അക്ഷരം buf[4];
      buf[0] = '\''; buf[1] = c; buf[2] = '\''; buf[3] = '\0';
      തിരികെ quotearg_style (escape_quoting_style, buf);
    }
}

സ്ഥിതം ശൂന്യം
current_lhs (ചിഹ്നം *sym, location loc, named_ref *ref)
{
  current_lhs_ചിഹ്നം = sym;
  current_lhs_loc = loc;
  എങ്കിൽ (sym)
    ചിഹ്നം_location_as_lhs_set (sym, loc);
  /* In order to simplify memory management, named references for lhs
     are always assigned by deep copy into the current symbol_list
     node.  This is because a single named-ref in the grammar may
     result in several uses when the user factors lhs between several
     rules using "|".  Therefore free the parser's original copy.  */
  സ്വതന്ത്രം (current_lhs_named_ref);
  current_lhs_named_ref = ref;
}

സ്ഥിതം ശൂന്യം tron (FILE *yyo)
{
  begin_use_class ("value", yyo);
}

സ്ഥിതം ശൂന്യം troff (FILE *yyo)
{
  end_use_class ("value", yyo);
}


/*----------.
| Unquote.  |
`----------*/

ഘടന obstack obstack_for_unquote;

ശൂന്യം
parser_init (ശൂന്യം)
{
  obstack_init (&obstack_for_unquote);
}

ശൂന്യം
parser_free (ശൂന്യം)
{
  obstack_free (&obstack_for_unquote, 0);
}

സ്ഥിതം ശൂന്യം
unquote_free (അക്ഷരം *last_string)
{
  obstack_free (&obstack_for_unquote, last_string);
}

സ്ഥിതം അക്ഷരം *
unquote (സ്ഥിരം അക്ഷരം *cp)
{
#define GROW(Char)                              \
  obstack_1grow (&obstack_for_unquote, Char);
  വേണ്ടി (++cp; *cp && *cp != '"'; ++cp)
    തിരഞ്ഞെടുപ്പ് (*cp)
      {
      അവസ്ഥ '"':
        നിർത്തുക;
      അവസ്ഥ '\\':
        ++cp;
        തിരഞ്ഞെടുപ്പ് (*cp)
          {
          അവസ്ഥ '0': അവസ്ഥ '1': അവസ്ഥ '2': അവസ്ഥ '3': അവസ്ഥ '4':
          അവസ്ഥ '5': അവസ്ഥ '6': അവസ്ഥ '7': അവസ്ഥ '8': അവസ്ഥ '9':
            {
              പൂർണ്ണസംഖ്യ c = cp[0] - '0';
              എങ്കിൽ (c_isdigit (cp[1]))
                {
                  ++cp;
                  c = c * 8 + cp[0] - '0';
                }
              എങ്കിൽ (c_isdigit (cp[1]))
                {
                  ++cp;
                  c = c * 8 + cp[0] - '0';
                }
              GROW (c);
            }
            നിർത്തുക;

          അവസ്ഥ 'a': GROW ('\a'); നിർത്തുക;
          അവസ്ഥ 'b': GROW ('\b'); നിർത്തുക;
          അവസ്ഥ 'f': GROW ('\f'); നിർത്തുക;
          അവസ്ഥ 'n': GROW ('\n'); നിർത്തുക;
          അവസ്ഥ 'r': GROW ('\r'); നിർത്തുക;
          അവസ്ഥ 't': GROW ('\t'); നിർത്തുക;
          അവസ്ഥ 'v': GROW ('\v'); നിർത്തുക;

          അവസ്ഥ 'x':
            {
              പൂർണ്ണസംഖ്യ c = 0;
              ആകുമ്പോൾ (c_isxdigit (cp[1]))
                {
                  ++cp;
                  c = (c * 16 + (c_isdigit (cp[0]) ? cp[0] - '0'
                                 : c_isupper (cp[0]) ? cp[0] - 'A'
                                 : cp[0] - '0'));
                }
              GROW (c);
              നിർത്തുക;
            }
          }
        നിർത്തുക;

      സ്ഥിരസ്ഥിതി:
        GROW (*cp);
        നിർത്തുക;
      }
  പ്രതിജ്ഞാ (*cp == '"');
  ++cp;
  പ്രതിജ്ഞാ (*cp == '\0');
#undef GROW
  തിരികെ obstack_finish0 (&obstack_for_unquote);
}
