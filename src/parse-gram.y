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
  #અન્તર્ભાવ "symlist.h"
  #અન્તર્ભાવ "symtab.h"
}

%code provides
{
  /* Initialize unquote.  */
  શૂન્ય parser_init (શૂન્ય);
  /* Deallocate storage for unquote.  */
  શૂન્ય parser_free (શૂન્ય);
}

%code ટોચ_2
{
  /* On column 0 to please syntax-check.  */
#અન્તર્ભાવ <config.h>
}

%code
{
  #અન્તર્ભાવ "system.h"

  #અન્તર્ભાવ <c-ctype.h>
  #અન્તર્ભાવ <quotearg.h>
  #અન્તર્ભાવ <vasnprintf.h>
  #અન્તર્ભાવ <xmemdup0.h>

  #અન્તર્ભાવ "complain.h"
  #અન્તર્ભાવ "conflicts.h"
  #અન્તર્ભાવ "files.h"
  #અન્તર્ભાવ "getargs.h"
  #અન્તર્ભાવ "gram.h"
  #અન્તર્ભાવ "named-ref.h"
  #અન્તર્ભાવ "reader.h"
  #અન્તર્ભાવ "scan-code.h"
  #અન્તર્ભાવ "scan-gram.h"
  #અન્તર્ભાવ "strversion.h"

  /* Pretend to be at least that version, to check features published
     in that version while developping it.  */
  સ્થિર અચલ અક્ષર* api_version = "3.8";

  સ્થિર પૂર્ણાંક current_prec = 0;
  સ્થિર location current_lhs_loc;
  સ્થિર named_ref *current_lhs_named_ref;
  સ્થિર પ્રતીક *current_lhs_પ્રતીક;
  સ્થિર પ્રતીક_class current_class = unknown_sym;

  /** Set the new current left-hand side symbol, possibly common
   * to several right-hand side parts of rule.
   */
  સ્થિર શૂન્ય current_lhs (પ્રતીક *sym, location loc, named_ref *ref);

  #define YYLLOC_DEFAULT(Current, Rhs, N)         \
    (Current) = lloc_default (Rhs, N)
  સ્થિર YYLTYPE lloc_default (YYLTYPE અચલ *, પૂર્ણાંક);

  #define YY_LOCATION_PRINT(File, Loc)            \
    location_print (Loc, File)

  /* Strip initial '{' and final '}' (must be first and last characters).
     Return the result.  */
  સ્થિર અક્ષર *strip_braces (અક્ષર *code);

  /* Convert CODE by calling code_props_plain_init if PLAIN, otherwise
     code_props_symbol_action_init.  Calls
     gram_scanner_last_string_free to release the latest string from
     the scanner (should be CODE). */
  સ્થિર અક્ષર અચલ *translate_code (અક્ષર *code, location loc, bool plain);

  /* Convert CODE by calling code_props_plain_init after having
     stripped the first and last characters (expected to be '{', and
     '}').  Calls gram_scanner_last_string_free to release the latest
     string from the scanner (should be CODE). */
  સ્થિર અક્ષર અચલ *translate_code_braceless (અક્ષર *code, location loc);

  /* Handle a %header directive.  */
  સ્થિર શૂન્ય handle_header (અક્ષર અચલ *કિંમત);

  /* Handle a %error-verbose directive.  */
  સ્થિર શૂન્ય handle_error_verbose (location અચલ *loc, અક્ષર અચલ *directive);

  /* Handle a %file-prefix directive.  */
  સ્થિર શૂન્ય handle_file_prefix (location અચલ *loc,
                                  location અચલ *dir_loc,
                                  અક્ષર અચલ *directive, અક્ષર અચલ *કિંમત);

  /* Handle a %language directive.  */
  સ્થિર શૂન્ય handle_language (location અચલ *loc, અક્ષર અચલ *lang);

  /* Handle a %name-prefix directive.  */
  સ્થિર શૂન્ય handle_name_prefix (location અચલ *loc,
                                  અક્ષર અચલ *directive, અક્ષર અચલ *કિંમત);

  /* Handle a %pure-parser directive.  */
  સ્થિર શૂન્ય handle_pure_parser (location અચલ *loc, અક્ષર અચલ *directive);

  /* Handle a %require directive.  */
  સ્થિર શૂન્ય handle_require (location અચલ *loc, અક્ષર અચલ *version);

  /* Handle a %skeleton directive.  */
  સ્થિર શૂન્ય handle_skeleton (location અચલ *loc, અક્ષર અચલ *skel);

  /* Handle a %yacc directive.  */
  સ્થિર શૂન્ય handle_yacc (location અચલ *loc);

  /* Implementation of yyerror.  */
  સ્થિર શૂન્ય gram_error (location અચલ *, અક્ષર અચલ *);

  /* A string that describes a char (e.g., 'a' -> "'a'").  */
  સ્થિર અક્ષર અચલ *char_name (અક્ષર);

  /* Add style to semantic values in traces.  */
  સ્થિર શૂન્ય tron (FILE *yyo);
  સ્થિર શૂન્ય troff (FILE *yyo);

  /* Interpret a quoted string (such as `"Hello, \"World\"\n\""`).
     Manages the memory of the result.  */
  સ્થિર અક્ષર *unquote (અચલ અક્ષર *str);

  /* Discard the latest unquoted string.  */
  સ્થિર શૂન્ય unquote_free (અક્ષર *last_string);
}

%define api.header.include {"parse-gram.h"}
%define api.prefix {gram_}
%define api.pure full
%define api.token.raw
%define api.કિંમત.પ્રકાર સંઘ
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

  PERCENT_ડાબું        "%left"
  PERCENT_જમણું       "%right"
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
  PERCENT_NO_લીટીઓ        "%no-lines"
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

%પ્રકાર <unsigned અક્ષર> CHAR_LITERAL
%printer { fputs (char_name ($$), yyo); } <unsigned અક્ષર>

%પ્રકાર <અક્ષર*> "{...}" "%?{...}" "%{...%}" EPILOGUE STRING TSTRING
%printer { fputs ($$, yyo); } <અક્ષર*>

%પ્રકાર <uniqstr>
  BRACKETED_ID ID ID_COLON
  PERCENT_ERROR_VERBOSE PERCENT_FILE_PREFIX PERCENT_FLAG PERCENT_NAME_PREFIX
  PERCENT_PURE_PARSER
  TAG tag tag.opt variable
%printer { fputs ($$, yyo); } <uniqstr>
%printer { fprintf (yyo, "[%s]", $$); } BRACKETED_ID
%printer { fprintf (yyo, "%s:", $$); } ID_COLON
%printer { fprintf (yyo, "%%%s", $$); } PERCENT_FLAG
%printer { fprintf (yyo, "<%s>", $$); } TAG tag

%token <પૂર્ણાંક> INT_LITERAL _("integer literal")
%printer { fprintf (yyo, "%d", $$); } <પૂર્ણાંક>

%પ્રકાર <પ્રતીક*> id id_colon string_as_id પ્રતીક token_decl token_decl_for_prec
%printer { fprintf (yyo, "%s", $$ ? $$->tag : "<NULL>"); } <પ્રતીક*>
%printer { fprintf (yyo, "%s:", $$->tag); } id_colon

%પ્રકાર <assoc> precedence_declarator

%destructor { પ્રતીક_list_free ($$); } <પ્રતીક_list*>
%printer { પ્રતીક_list_syms_print ($$, yyo); } <પ્રતીક_list*>

%પ્રકાર <named_ref*> named_ref.opt

/*---------.
| %param.  |
`---------*/
%code requires
{
  પ્રકારવ્યાખ્યા ગણના
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
  સ્થિર શૂન્ય add_param (param_type પ્રકાર, અક્ષર *decl, location loc);
  સ્થિર param_type current_param = param_none;
};
%token <param_type> PERCENT_PARAM "%param";
%printer
{
  પસંદગી ($$)
    {
#define CASE(In, Out)                                           \
      સ્થિતિ param_ ## In: fputs ("%" #Out, yyo); તોડો
      CASE (lex,   lex-param);
      CASE (parse, parse-param);
      CASE (both,  param);
#undef CASE
      સ્થિતિ param_none: aver (false); તોડો;
    }
} <param_type>;


                     /*==========\
                     | Grammar.  |
                     \==========*/
%%

આગત:
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
| "%define" variable કિંમત
    {
      muscle_percent_define_ઉમેરો ($2, @$, $3.kind, $3.chars,
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
| "%no-lines"                   { no_લીટીઓ_flag = true; }
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
  પ્રતીક_declaration
| "%start" symbols.1
    {
      grammar_start_symbols_add ($2);
    }
| code_props_type "{...}" generic_symlist
    {
      code_props code;
      code_props_પ્રતીક_action_init (&code, $2, @2);
      code_props_translate_code (&code);
      {
        માટે (પ્રતીક_list *list = $3; list; list = list->next)
          પ્રતીક_list_code_props_set (list, $1, &code);
        પ્રતીક_list_free ($3);
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

%પ્રકાર <code_props_type> code_props_type;
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
| ID     { muscle_percent_define_ઉમેરો ("api.value.union.name",
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


%પ્રકાર <પ્રતીક_list*> nterm_decls પ્રતીક_decls symbols.1
      token_decls token_decls_for_prec
      token_decl.1 token_decl_for_prec.1;
પ્રતીક_declaration:
  "%nterm" { current_class = nterm_sym; } nterm_decls[syms]
    {
      current_class = unknown_sym;
      પ્રતીક_list_free ($syms);
    }
| "%token" { current_class = token_sym; } token_decls[syms]
    {
      current_class = unknown_sym;
      પ્રતીક_list_free ($syms);
    }
| "%type" { current_class = pct_type_sym; } પ્રતીક_decls[syms]
    {
      current_class = unknown_sym;
      પ્રતીક_list_free ($syms);
    }
| precedence_declarator token_decls_for_prec[syms]
    {
      ++current_prec;
      માટે (પ્રતીક_list *list = $syms; list; list = list->next)
        પ્રતીક_precedence_set (list->content.sym, current_prec, $1, @1);
      પ્રતીક_list_free ($syms);
    }
;

precedence_declarator:
  "%left"       { $$ = ડાબું_assoc; }
| "%right"      { $$ = જમણું_assoc; }
| "%nonassoc"   { $$ = non_assoc; }
| "%precedence" { $$ = precedence_assoc; }
;

%પ્રકાર <અક્ષર*> string.opt;
string.opt:
  %empty  { $$ = ખાલી; }
| STRING  { $$ = $1; }
;

tag.opt:
  %empty { $$ = ખાલી; }
| TAG    { $$ = $1; }
;

%પ્રકાર <પ્રતીક_list*> generic_symlist generic_symlist_item;
generic_symlist:
  generic_symlist_item
| generic_symlist generic_symlist_item   { $$ = પ્રતીક_list_append ($1, $2); }
;

generic_symlist_item:
  પ્રતીક    { $$ = પ્રતીક_list_sym_new ($1, @1); }
| tag       { $$ = પ્રતીક_list_type_new ($1, @1); }
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
      $$ = પ્રતીક_list_type_set ($syms, $TAG);
    }
| token_decls TAG token_decl.1[syms]
    {
      $$ = પ્રતીક_list_append ($1, પ્રતીક_list_type_set ($syms, $TAG));
    }
;

// One or more symbol declarations for %token or %nterm.
token_decl.1:
  token_decl                { $$ = પ્રતીક_list_sym_new ($1, @1); }
| token_decl.1 token_decl   { $$ = પ્રતીક_list_append ($1, પ્રતીક_list_sym_new ($2, @2)); }

// One symbol declaration for %token or %nterm.
token_decl:
  id પૂર્ણાંક.opt[num] alias
    {
      $$ = $id;
      પ્રતીક_class_set ($id, current_class, @id, true);
      જો (0 <= $અંક)
        પ્રતીક_code_set ($id, $અંક, @અંક);
      જો ($alias)
        પ્રતીક_make_alias ($id, $alias, @alias);
    }
;

%પ્રકાર <પૂર્ણાંક> પૂર્ણાંક.opt;
પૂર્ણાંક.opt:
  %empty  { $$ = -1; }
| INT_LITERAL
;

%પ્રકાર <પ્રતીક*> alias;
alias:
  %empty         { $$ = ખાલી; }
| string_as_id   { $$ = $1; }
| TSTRING
    {
      $$ = પ્રતીક_get ($1, @1);
      પ્રતીક_class_set ($$, token_sym, @1, false);
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
      $$ = પ્રતીક_list_type_set ($syms, $TAG);
    }
| token_decls_for_prec TAG token_decl_for_prec.1[syms]
    {
      $$ = પ્રતીક_list_append ($1, પ્રતીક_list_type_set ($syms, $TAG));
    }
;

// One or more token declarations for precedence declaration.
token_decl_for_prec.1:
  token_decl_for_prec
    { $$ = પ્રતીક_list_sym_new ($1, @1); }
| token_decl_for_prec.1 token_decl_for_prec
    { $$ = પ્રતીક_list_append ($1, પ્રતીક_list_sym_new ($2, @2)); }

// One token declaration for precedence declaration.
token_decl_for_prec:
  id પૂર્ણાંક.opt[num]
    {
      $$ = $id;
      પ્રતીક_class_set ($id, token_sym, @id, false);
      જો (0 <= $અંક)
        પ્રતીક_code_set ($id, $અંક, @અંક);
    }
| string_as_id
;


/*-----------------------------------.
| symbol_decls (argument of %type).  |
`-----------------------------------*/

// A non empty list of typed symbols (for %type).
પ્રતીક_decls:
  symbols.1[syms]
    {
      $$ = $syms;
    }
| TAG symbols.1[syms]
    {
      $$ = પ્રતીક_list_type_set ($syms, $TAG);
    }
| પ્રતીક_decls TAG symbols.1[syms]
    {
      $$ = પ્રતીક_list_append ($1, પ્રતીક_list_type_set ($syms, $TAG));
    }
;

// One or more symbols.
symbols.1:
  પ્રતીક
    {
      જો (current_class != unknown_sym)
        પ્રતીક_class_set ($પ્રતીક, current_class, @પ્રતીક, false);
      $$ = પ્રતીક_list_sym_new ($પ્રતીક, @પ્રતીક);
    }
  | symbols.1 પ્રતીક
    {
      જો (current_class != unknown_sym)
        પ્રતીક_class_set ($પ્રતીક, current_class, @પ્રતીક, false);
      $$ = પ્રતીક_list_append ($1, પ્રતીક_list_sym_new ($પ્રતીક, @પ્રતીક));
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
    { grammar_current_rule_begin (current_lhs_પ્રતીક, current_lhs_loc,
                                  current_lhs_named_ref); }
| rhs પ્રતીક named_ref.opt
    { grammar_current_rule_પ્રતીક_append ($2, @2, $3); }
| rhs tag.opt "{...}"[action] named_ref.opt[name]
    { grammar_current_rule_action_append ($action, @action, $name, $[tag.opt]); }
| rhs "%?{...}"
    { grammar_current_rule_predicate_append ($2, @2); }
| rhs "%empty"
    { grammar_current_rule_empty_set (@2); }
| rhs "%prec" પ્રતીક
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
  %empty         { $$ = ખાલી; }
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
  #અન્તર્ભાવ "muscle-tab.h"
  પ્રકારવ્યાખ્યા સંરચના
  {
    અક્ષર અચલ *chars;
    muscle_kind kind;
  } કિંમત_type;
};
%પ્રકાર <કિંમત_type> કિંમત;
%printer
{
  પસંદગી ($$.kind)
    {
    સ્થિતિ muscle_code:    fprintf (yyo,  "{%s}",  $$.chars); તોડો;
    સ્થિતિ muscle_keyword: fprintf (yyo,   "%s",   $$.chars); તોડો;
    સ્થિતિ muscle_string:  fprintf (yyo, "\"%s\"", $$.chars); તોડો;
    }
} <કિંમત_type>;

કિંમત:
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
    { $$ = પ્રતીક_from_uniqstr ($1, @1); }
| CHAR_LITERAL
    {
      અચલ અક્ષર *var = "api.token.raw";
      જો (current_class == nterm_sym)
        {
          complain (&@1, complaint,
                    _("character literals cannot be nonterminals"));
          YYERROR;
        }
      જો (muscle_percent_define_ifdef (var))
        {
          complain (&@1, complaint,
                    _("character literals cannot be used together"
                    " with %s"), var);
          location loc = muscle_percent_define_get_loc (var);
          subcomplain (&loc, complaint, _("definition of %s"), var);
        }
      $$ = પ્રતીક_get (char_name ($1), @1);
      પ્રતીક_class_set ($$, token_sym, @1, false);
      પ્રતીક_code_set ($$, $1, @1);
    }
;

id_colon:
  ID_COLON { $$ = પ્રતીક_from_uniqstr ($1, @1); }
;


પ્રતીક:
  id
| string_as_id
;

/* A string used as an ID.  */
string_as_id:
  STRING
    {
      $$ = પ્રતીક_get ($1, @1);
      પ્રતીક_class_set ($$, token_sym, @1, false);
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

પૂર્ણાંક
yyreport_syntax_error (અચલ yypcontext_t *ctx)
{
  પૂર્ણાંક res = 0;
  /* Arguments of format: reported tokens (one for the "unexpected",
     one per "expected"). */
  ગણના { ARGS_MAX = 5 };
  અચલ અક્ષર *argv[ARGS_MAX];
  પૂર્ણાંક argc = 0;
  yysymbol_kind_t unexpected = yypcontext_token (ctx);
  જો (unexpected != YYSYMBOL_YYEMPTY)
    {
      argv[argc++] = yysymbol_name (unexpected);
      yysymbol_kind_t expected[ARGS_MAX - 1];
      પૂર્ણાંક nexpected = yypcontext_expected_tokens (ctx, expected, ARGS_MAX - 1);
      જો (nexpected < 0)
        res = nexpected;
      નહીંતર
        માટે (પૂર્ણાંક i = 0; i < nexpected; ++i)
          argv[argc++] = yysymbol_name (expected[i]);
    }
  syntax_error (*yypcontext_location (ctx), argc, argv);
  પાછા res;
}


/* Return the location of the left-hand side of a rule whose
   right-hand side is RHS[1] ... RHS[N].  Ignore empty nonterminals in
   the right-hand side, and return an empty location equal to the end
   boundary of RHS[0] if the right-hand side is empty.  */

સ્થિર YYLTYPE
lloc_default (YYLTYPE અચલ *rhs, પૂર્ણાંક n)
{
  YYLTYPE loc;

  /* SGI MIPSpro 7.4.1m miscompiles "loc.start = loc.end = rhs[n].end;".
     The bug is fixed in 7.4.2m, but play it safe for now.  */
  loc.start = rhs[n].end;
  loc.end = rhs[n].end;

  /* Ignore empty nonterminals the start of the right-hand side.
     Do not bother to ignore them at the end of the right-hand side,
     since empty nonterminals have the same end as their predecessors.  */
  માટે (પૂર્ણાંક i = 1; i <= n; i++)
    જો (! equal_boundaries (rhs[i].start, rhs[i].end))
      {
        loc.start = rhs[i].start;
        તોડો;
      }

  પાછા loc;
}

સ્થિર
અક્ષર *strip_braces (અક્ષર *code)
{
  code[strlen (code) - 1] = 0;
  પાછા code + 1;
}

સ્થિર
અક્ષર અચલ *
translate_code (અક્ષર *code, location loc, bool plain)
{
  code_props plain_code;
  જો (plain)
    code_props_plain_init (&plain_code, code, loc);
  નહીંતર
    code_props_પ્રતીક_action_init (&plain_code, code, loc);
  code_props_translate_code (&plain_code);
  gram_scanner_last_string_free ();
  પાછા plain_code.code;
}

સ્થિર
અક્ષર અચલ *
translate_code_braceless (અક્ષર *code, location loc)
{
  પાછા translate_code (strip_braces (code), loc, true);
}

સ્થિર શૂન્ય
add_param (param_type પ્રકાર, અક્ષર *decl, location loc)
{
  સ્થિર અક્ષર અચલ alphanum[26 + 26 + 1 + 10 + 1] =
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "_"
    "0123456789";

  અક્ષર અચલ *name_start = ખાલી;
  {
    અક્ષર *p;
    /* Stop on last actual character.  */
    માટે (p = decl; p[1]; p++)
      જો ((p == decl
           || ! memchr (alphanum, p[-1], કદ alphanum - 1))
          && memchr (alphanum, p[0], કદ alphanum - 10 - 1))
        name_start = p;

    /* Strip the surrounding '{' and '}', and any blanks just inside
       the braces.  */
    --p;
    જ્યારે (c_isspace ((unsigned અક્ષર) *p))
      --p;
    p[1] = '\0';
    ++decl;
    જ્યારે (c_isspace ((unsigned અક્ષર) *decl))
      ++decl;
  }

  જો (! name_start)
    complain (&loc, complaint, _("missing identifier in parameter declaration"));
  નહીંતર
    {
      અક્ષર *name = xmemdup0 (name_start, strspn (name_start, alphanum));
      જો (પ્રકાર & param_lex)
        muscle_pair_list_grow ("lex_param", decl, name);
      જો (પ્રકાર & param_parse)
        muscle_pair_list_grow ("parse_param", decl, name);
      મુક્ત (name);
    }

  gram_scanner_last_string_free ();
}


સ્થિર શૂન્ય
handle_header (અક્ષર અચલ *કિંમત)
{
  header_flag = true;
  જો (કિંમત)
    {
      અક્ષર *file = unquote (કિંમત);
      spec_header_file = xstrdup (file);
      gram_scanner_last_string_free ();
      unquote_free (file);
    }
}


સ્થિર શૂન્ય
handle_error_verbose (location અચલ *loc, અક્ષર અચલ *directive)
{
  bison_directive (loc, directive);
  muscle_percent_define_ઉમેરો (directive, *loc, muscle_keyword, "",
                                MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
}


સ્થિર શૂન્ય
handle_file_prefix (location અચલ *loc,
                    location અચલ *dir_loc,
                    અક્ષર અચલ *directive, અક્ષર અચલ *કિંમત_quoted)
{
  અક્ષર *કિંમત = unquote (કિંમત_quoted);
  bison_directive (loc, directive);
  bool warned = false;

  જો (location_empty (spec_file_prefix_loc))
    {
      spec_file_prefix_loc = *loc;
      spec_file_prefix = કિંમત;
    }
  નહીંતર
    {
      duplicate_directive (directive, spec_file_prefix_loc, *loc);
      warned = true;
    }

  જો (!warned
      && STRNEQ (directive, "%file-prefix"))
    deprecated_directive (dir_loc, directive, "%file-prefix");
}

સ્થિર શૂન્ય
handle_language (location અચલ *loc, અક્ષર અચલ *lang)
{
  language_argmatch (unquote (lang), grammar_prio, *loc);
}


સ્થિર શૂન્ય
handle_name_prefix (location અચલ *loc,
                    અક્ષર અચલ *directive, અક્ષર અચલ *કિંમત_quoted)
{
  અક્ષર *કિંમત = unquote (કિંમત_quoted);
  bison_directive (loc, directive);

  અક્ષર buf1[1024];
  size_t len1 = કદ (buf1);
  અક્ષર *old = asnprintf (buf1, &len1, "%s\"%s\"", directive, કિંમત);
  જો (!old)
    xalloc_die ();

  જો (location_empty (spec_name_prefix_loc))
    {
      spec_name_prefix = કિંમત;
      spec_name_prefix_loc = *loc;

      અક્ષર buf2[1024];
      size_t len2 = કદ (buf2);
      અક્ષર *new = asnprintf (buf2, &len2, "%%define api.prefix {%s}", કિંમત);
      જો (!new)
        xalloc_die ();
      deprecated_directive (loc, old, new);
      જો (new != buf2)
        મુક્ત (new);
    }
  નહીંતર
    duplicate_directive (old, spec_file_prefix_loc, *loc);

  જો (old != buf1)
    મુક્ત (old);
}


સ્થિર શૂન્ય
handle_pure_parser (location અચલ *loc, અક્ષર અચલ *directive)
{
  bison_directive (loc, directive);
  deprecated_directive (loc, directive, "%define api.pure");
  muscle_percent_define_ઉમેરો ("api.pure", *loc, muscle_keyword, "",
                                MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
}


સ્થિર શૂન્ય
handle_require (location અચલ *loc, અક્ષર અચલ *version_quoted)
{
  અક્ષર *version = unquote (version_quoted);
  required_version = strversion_to_int (version);
  જો (required_version == -1)
    {
      complain (loc, complaint, _("invalid version requirement: %s"),
                version);
      required_version = 0;
    }
  નહીંતર
    {
      અચલ અક્ષર* package_version =
        0 < strverscmp (api_version, PACKAGE_VERSION)
        ? api_version : PACKAGE_VERSION;
      જો (0 < strverscmp (version, package_version))
        {
          complain (loc, complaint, _("require bison %s, but have %s"),
                    version, package_version);
          exit (EX_MISMATCH);
        }
    }
  unquote_free (version);
  gram_scanner_last_string_free ();
}

સ્થિર શૂન્ય
handle_skeleton (location અચલ *loc, અક્ષર અચલ *skel_quoted)
{
  અક્ષર *skel = unquote (skel_quoted);
  અક્ષર અચલ *skeleton_user = skel;
  જો (strchr (skeleton_user, '/'))
    {
      size_t dir_length = strlen (grammar_file);
      જ્યારે (dir_length && grammar_file[dir_length - 1] != '/')
        --dir_length;
      જ્યારે (dir_length && grammar_file[dir_length - 1] == '/')
        --dir_length;
      અક્ષર *skeleton_build =
        xmalloc (dir_length + 1 + strlen (skeleton_user) + 1);
      જો (dir_length > 0)
        {
          memcpy (skeleton_build, grammar_file, dir_length);
          skeleton_build[dir_length++] = '/';
        }
      strcpy (skeleton_build + dir_length, skeleton_user);
      skeleton_user = uniqstr_new (skeleton_build);
      મુક્ત (skeleton_build);
    }
  skeleton_arg (skeleton_user, grammar_prio, *loc);
}


સ્થિર શૂન્ય
handle_yacc (location અચલ *loc)
{
  અચલ અક્ષર *directive = "%yacc";
  bison_directive (loc, directive);
  જો (location_empty (yacc_loc))
    set_yacc (*loc);
  નહીંતર
    duplicate_directive (directive, yacc_loc, *loc);
}


સ્થિર શૂન્ય
gram_error (location અચલ *loc, અક્ષર અચલ *સંદેશ)
{
  complain (loc, complaint, "%s", સંદેશ);
}

સ્થિર અક્ષર અચલ *
char_name (અક્ષર c)
{
  જો (c == '\'')
    પાછા "'\\''";
  નહીંતર
    {
      અક્ષર buf[4];
      buf[0] = '\''; buf[1] = c; buf[2] = '\''; buf[3] = '\0';
      પાછા quotearg_style (escape_quoting_style, buf);
    }
}

સ્થિર શૂન્ય
current_lhs (પ્રતીક *sym, location loc, named_ref *ref)
{
  current_lhs_પ્રતીક = sym;
  current_lhs_loc = loc;
  જો (sym)
    પ્રતીક_location_as_lhs_set (sym, loc);
  /* In order to simplify memory management, named references for lhs
     are always assigned by deep copy into the current symbol_list
     node.  This is because a single named-ref in the grammar may
     result in several uses when the user factors lhs between several
     rules using "|".  Therefore free the parser's original copy.  */
  મુક્ત (current_lhs_named_ref);
  current_lhs_named_ref = ref;
}

સ્થિર શૂન્ય tron (FILE *yyo)
{
  begin_use_class ("value", yyo);
}

સ્થિર શૂન્ય troff (FILE *yyo)
{
  end_use_class ("value", yyo);
}


/*----------.
| Unquote.  |
`----------*/

સંરચના obstack obstack_for_unquote;

શૂન્ય
parser_init (શૂન્ય)
{
  obstack_init (&obstack_for_unquote);
}

શૂન્ય
parser_free (શૂન્ય)
{
  obstack_free (&obstack_for_unquote, 0);
}

સ્થિર શૂન્ય
unquote_free (અક્ષર *last_string)
{
  obstack_free (&obstack_for_unquote, last_string);
}

સ્થિર અક્ષર *
unquote (અચલ અક્ષર *cp)
{
#define GROW(Char)                              \
  obstack_1grow (&obstack_for_unquote, Char);
  માટે (++cp; *cp && *cp != '"'; ++cp)
    પસંદગી (*cp)
      {
      સ્થિતિ '"':
        તોડો;
      સ્થિતિ '\\':
        ++cp;
        પસંદગી (*cp)
          {
          સ્થિતિ '0': સ્થિતિ '1': સ્થિતિ '2': સ્થિતિ '3': સ્થિતિ '4':
          સ્થિતિ '5': સ્થિતિ '6': સ્થિતિ '7': સ્થિતિ '8': સ્થિતિ '9':
            {
              પૂર્ણાંક c = cp[0] - '0';
              જો (c_isdigit (cp[1]))
                {
                  ++cp;
                  c = c * 8 + cp[0] - '0';
                }
              જો (c_isdigit (cp[1]))
                {
                  ++cp;
                  c = c * 8 + cp[0] - '0';
                }
              GROW (c);
            }
            તોડો;

          સ્થિતિ 'a': GROW ('\a'); તોડો;
          સ્થિતિ 'b': GROW ('\b'); તોડો;
          સ્થિતિ 'f': GROW ('\f'); તોડો;
          સ્થિતિ 'n': GROW ('\n'); તોડો;
          સ્થિતિ 'r': GROW ('\r'); તોડો;
          સ્થિતિ 't': GROW ('\t'); તોડો;
          સ્થિતિ 'v': GROW ('\v'); તોડો;

          સ્થિતિ 'x':
            {
              પૂર્ણાંક c = 0;
              જ્યારે (c_isxdigit (cp[1]))
                {
                  ++cp;
                  c = (c * 16 + (c_isdigit (cp[0]) ? cp[0] - '0'
                                 : c_isupper (cp[0]) ? cp[0] - 'A'
                                 : cp[0] - '0'));
                }
              GROW (c);
              તોડો;
            }
          }
        તોડો;

      મૂળભૂત:
        GROW (*cp);
        તોડો;
      }
  પ્રતિજ્ઞા (*cp == '"');
  ++cp;
  પ્રતિજ્ઞા (*cp == '\0');
#undef GROW
  પાછા obstack_finish0 (&obstack_for_unquote);
}
