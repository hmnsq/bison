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
  #समावेश "symlist.h"
  #समावेश "symtab.h"
}

%code provides
{
  /* Initialize unquote.  */
  व्योम parser_init (व्योम);
  /* Deallocate storage for unquote.  */
  व्योम parser_free (व्योम);
}

%code शीर्ष_2
{
  /* On column 0 to please syntax-check.  */
#समावेश <config.h>
}

%code
{
  #समावेश "system.h"

  #समावेश <c-ctype.h>
  #समावेश <quotearg.h>
  #समावेश <vasnprintf.h>
  #समावेश <xmemdup0.h>

  #समावेश "complain.h"
  #समावेश "conflicts.h"
  #समावेश "files.h"
  #समावेश "getargs.h"
  #समावेश "gram.h"
  #समावेश "named-ref.h"
  #समावेश "reader.h"
  #समावेश "scan-code.h"
  #समावेश "scan-gram.h"
  #समावेश "strversion.h"

  /* Pretend to be at least that version, to check features published
     in that version while developping it.  */
  अटल स्थिर अक्षर* api_version = "3.8";

  अटल पूर्णांक current_prec = 0;
  अटल location current_lhs_loc;
  अटल named_ref *current_lhs_named_ref;
  अटल प्रतीक *current_lhs_प्रतीक;
  अटल प्रतीक_class current_class = unknown_sym;

  /** Set the new current left-hand side symbol, possibly common
   * to several right-hand side parts of rule.
   */
  अटल व्योम current_lhs (प्रतीक *sym, location loc, named_ref *ref);

  #घोषणा YYLLOC_DEFAULT(Current, Rhs, N)         \
    (Current) = lloc_default (Rhs, N)
  अटल YYLTYPE lloc_default (YYLTYPE स्थिर *, पूर्णांक);

  #घोषणा YY_LOCATION_PRINT(File, Loc)            \
    location_print (Loc, File)

  /* Strip initial '{' and final '}' (must be first and last characters).
     Return the result.  */
  अटल अक्षर *strip_braces (अक्षर *code);

  /* Convert CODE by calling code_props_plain_init if PLAIN, otherwise
     code_props_symbol_action_init.  Calls
     gram_scanner_last_string_free to release the latest string from
     the scanner (should be CODE). */
  अटल अक्षर स्थिर *translate_code (अक्षर *code, location loc, bool plain);

  /* Convert CODE by calling code_props_plain_init after having
     stripped the first and last characters (expected to be '{', and
     '}').  Calls gram_scanner_last_string_free to release the latest
     string from the scanner (should be CODE). */
  अटल अक्षर स्थिर *translate_code_braceless (अक्षर *code, location loc);

  /* Handle a %header directive.  */
  अटल व्योम handle_header (अक्षर स्थिर *मूल्य);

  /* Handle a %error-verbose directive.  */
  अटल व्योम handle_error_verbose (location स्थिर *loc, अक्षर स्थिर *directive);

  /* Handle a %file-prefix directive.  */
  अटल व्योम handle_file_prefix (location स्थिर *loc,
                                  location स्थिर *dir_loc,
                                  अक्षर स्थिर *directive, अक्षर स्थिर *मूल्य);

  /* Handle a %language directive.  */
  अटल व्योम handle_language (location स्थिर *loc, अक्षर स्थिर *lang);

  /* Handle a %name-prefix directive.  */
  अटल व्योम handle_name_prefix (location स्थिर *loc,
                                  अक्षर स्थिर *directive, अक्षर स्थिर *मूल्य);

  /* Handle a %pure-parser directive.  */
  अटल व्योम handle_pure_parser (location स्थिर *loc, अक्षर स्थिर *directive);

  /* Handle a %require directive.  */
  अटल व्योम handle_require (location स्थिर *loc, अक्षर स्थिर *version);

  /* Handle a %skeleton directive.  */
  अटल व्योम handle_skeleton (location स्थिर *loc, अक्षर स्थिर *skel);

  /* Handle a %yacc directive.  */
  अटल व्योम handle_yacc (location स्थिर *loc);

  /* Implementation of yyerror.  */
  अटल व्योम gram_error (location स्थिर *, अक्षर स्थिर *);

  /* A string that describes a char (e.g., 'a' -> "'a'").  */
  अटल अक्षर स्थिर *char_name (अक्षर);

  /* Add style to semantic values in traces.  */
  अटल व्योम tron (खाता *yyo);
  अटल व्योम troff (खाता *yyo);

  /* Interpret a quoted string (such as `"Hello, \"World\"\n\""`).
     Manages the memory of the result.  */
  अटल अक्षर *unquote (स्थिर अक्षर *str);

  /* Discard the latest unquoted string.  */
  अटल व्योम unquote_free (अक्षर *last_string);
}

%define api.header.include {"parse-gram.h"}
%define api.prefix {gram_}
%define api.pure full
%define api.token.raw
%define api.मूल्य.type जोड़
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

  PERCENT_बायाँ        "%left"
  PERCENT_दायाँ       "%right"
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
  PERCENT_NO_पंक्तियाँ        "%no-lines"
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

%type <अचिन्हित अक्षर> CHAR_LITERAL
%printer { ख_माला_दो (char_name ($$), yyo); } <अचिन्हित अक्षर>

%type <अक्षर*> "{...}" "%?{...}" "%{...%}" EPILOGUE STRING TSTRING
%printer { ख_माला_दो ($$, yyo); } <अक्षर*>

%type <uniqstr>
  BRACKETED_ID ID ID_COLON
  PERCENT_ERROR_VERBOSE PERCENT_FILE_PREFIX PERCENT_FLAG PERCENT_NAME_PREFIX
  PERCENT_PURE_PARSER
  TAG tag tag.opt variable
%printer { ख_माला_दो ($$, yyo); } <uniqstr>
%printer { ख_लिखो (yyo, "[%s]", $$); } BRACKETED_ID
%printer { ख_लिखो (yyo, "%s:", $$); } ID_COLON
%printer { ख_लिखो (yyo, "%%%s", $$); } PERCENT_FLAG
%printer { ख_लिखो (yyo, "<%s>", $$); } TAG tag

%token <पूर्णांक> INT_LITERAL _("integer literal")
%printer { ख_लिखो (yyo, "%d", $$); } <पूर्णांक>

%type <प्रतीक*> id id_colon string_as_id प्रतीक token_decl token_decl_for_prec
%printer { ख_लिखो (yyo, "%s", $$ ? $$->tag : "<NULL>"); } <प्रतीक*>
%printer { ख_लिखो (yyo, "%s:", $$->tag); } id_colon

%type <assoc> precedence_declarator

%destructor { प्रतीक_list_free ($$); } <प्रतीक_list*>
%printer { प्रतीक_list_syms_print ($$, yyo); } <प्रतीक_list*>

%type <named_ref*> named_ref.opt

/*---------.
| %param.  |
`---------*/
%code requires
{
  प्रकार क्रमागत
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
  अटल व्योम add_param (param_type type, अक्षर *decl, location loc);
  अटल param_type current_param = param_none;
};
%token <param_type> PERCENT_PARAM "%param";
%printer
{
  चयन ($$)
    {
#घोषणा CASE(In, Out)                                           \
      हाल param_ ## In: ख_माला_दो ("%" #Out, yyo); अवरोध
      CASE (lex,   lex-param);
      CASE (parse, parse-param);
      CASE (both,  param);
#अघोषित CASE
      हाल param_none: aver (false); अवरोध;
    }
} <param_type>;


                     /*==========\
                     | Grammar.  |
                     \==========*/
%%

निवेश:
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
| "%define" variable मूल्य
    {
      muscle_percent_define_जोड़ो ($2, @$, $3.kind, $3.chars,
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
| "%no-lines"                   { no_पंक्तियाँ_flag = true; }
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
  प्रतीक_declaration
| "%start" symbols.1
    {
      grammar_start_symbols_add ($2);
    }
| code_props_type "{...}" generic_symlist
    {
      code_props code;
      code_props_प्रतीक_action_init (&code, $2, @2);
      code_props_translate_code (&code);
      {
        क्रम (प्रतीक_list *list = $3; list; list = list->अग्र)
          प्रतीक_list_code_props_set (list, $1, &code);
        प्रतीक_list_free ($3);
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

%type <code_props_type> code_props_type;
%printer { ख_लिखो (yyo, "%s", code_props_type_string ($$)); } <code_props_type>;
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
| ID     { muscle_percent_define_जोड़ो ("api.value.union.name",
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


%type <प्रतीक_list*> nterm_decls प्रतीक_decls symbols.1
      token_decls token_decls_for_prec
      token_decl.1 token_decl_for_prec.1;
प्रतीक_declaration:
  "%nterm" { current_class = nterm_sym; } nterm_decls[syms]
    {
      current_class = unknown_sym;
      प्रतीक_list_free ($syms);
    }
| "%token" { current_class = token_sym; } token_decls[syms]
    {
      current_class = unknown_sym;
      प्रतीक_list_free ($syms);
    }
| "%type" { current_class = pct_type_sym; } प्रतीक_decls[syms]
    {
      current_class = unknown_sym;
      प्रतीक_list_free ($syms);
    }
| precedence_declarator token_decls_for_prec[syms]
    {
      ++current_prec;
      क्रम (प्रतीक_list *list = $syms; list; list = list->अग्र)
        प्रतीक_precedence_set (list->content.sym, current_prec, $1, @1);
      प्रतीक_list_free ($syms);
    }
;

precedence_declarator:
  "%left"       { $$ = बायाँ_assoc; }
| "%right"      { $$ = दायाँ_assoc; }
| "%nonassoc"   { $$ = non_assoc; }
| "%precedence" { $$ = precedence_assoc; }
;

%type <अक्षर*> string.opt;
string.opt:
  %empty  { $$ = शून्य; }
| STRING  { $$ = $1; }
;

tag.opt:
  %empty { $$ = शून्य; }
| TAG    { $$ = $1; }
;

%type <प्रतीक_list*> generic_symlist generic_symlist_item;
generic_symlist:
  generic_symlist_item
| generic_symlist generic_symlist_item   { $$ = प्रतीक_list_append ($1, $2); }
;

generic_symlist_item:
  प्रतीक    { $$ = प्रतीक_list_sym_new ($1, @1); }
| tag       { $$ = प्रतीक_list_type_new ($1, @1); }
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
      $$ = प्रतीक_list_type_set ($syms, $TAG);
    }
| token_decls TAG token_decl.1[syms]
    {
      $$ = प्रतीक_list_append ($1, प्रतीक_list_type_set ($syms, $TAG));
    }
;

// One or more symbol declarations for %token or %nterm.
token_decl.1:
  token_decl                { $$ = प्रतीक_list_sym_new ($1, @1); }
| token_decl.1 token_decl   { $$ = प्रतीक_list_append ($1, प्रतीक_list_sym_new ($2, @2)); }

// One symbol declaration for %token or %nterm.
token_decl:
  id पूर्णांक.opt[num] alias
    {
      $$ = $id;
      प्रतीक_class_set ($id, current_class, @id, true);
      अगर (0 <= $अंक)
        प्रतीक_code_set ($id, $अंक, @अंक);
      अगर ($alias)
        प्रतीक_make_alias ($id, $alias, @alias);
    }
;

%type <पूर्णांक> पूर्णांक.opt;
पूर्णांक.opt:
  %empty  { $$ = -1; }
| INT_LITERAL
;

%type <प्रतीक*> alias;
alias:
  %empty         { $$ = शून्य; }
| string_as_id   { $$ = $1; }
| TSTRING
    {
      $$ = प्रतीक_get ($1, @1);
      प्रतीक_class_set ($$, token_sym, @1, false);
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
      $$ = प्रतीक_list_type_set ($syms, $TAG);
    }
| token_decls_for_prec TAG token_decl_for_prec.1[syms]
    {
      $$ = प्रतीक_list_append ($1, प्रतीक_list_type_set ($syms, $TAG));
    }
;

// One or more token declarations for precedence declaration.
token_decl_for_prec.1:
  token_decl_for_prec
    { $$ = प्रतीक_list_sym_new ($1, @1); }
| token_decl_for_prec.1 token_decl_for_prec
    { $$ = प्रतीक_list_append ($1, प्रतीक_list_sym_new ($2, @2)); }

// One token declaration for precedence declaration.
token_decl_for_prec:
  id पूर्णांक.opt[num]
    {
      $$ = $id;
      प्रतीक_class_set ($id, token_sym, @id, false);
      अगर (0 <= $अंक)
        प्रतीक_code_set ($id, $अंक, @अंक);
    }
| string_as_id
;


/*-----------------------------------.
| symbol_decls (argument of %type).  |
`-----------------------------------*/

// A non empty list of typed symbols (for %type).
प्रतीक_decls:
  symbols.1[syms]
    {
      $$ = $syms;
    }
| TAG symbols.1[syms]
    {
      $$ = प्रतीक_list_type_set ($syms, $TAG);
    }
| प्रतीक_decls TAG symbols.1[syms]
    {
      $$ = प्रतीक_list_append ($1, प्रतीक_list_type_set ($syms, $TAG));
    }
;

// One or more symbols.
symbols.1:
  प्रतीक
    {
      अगर (current_class != unknown_sym)
        प्रतीक_class_set ($प्रतीक, current_class, @प्रतीक, false);
      $$ = प्रतीक_list_sym_new ($प्रतीक, @प्रतीक);
    }
  | symbols.1 प्रतीक
    {
      अगर (current_class != unknown_sym)
        प्रतीक_class_set ($प्रतीक, current_class, @प्रतीक, false);
      $$ = प्रतीक_list_append ($1, प्रतीक_list_sym_new ($प्रतीक, @प्रतीक));
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
    { grammar_current_rule_begin (current_lhs_प्रतीक, current_lhs_loc,
                                  current_lhs_named_ref); }
| rhs प्रतीक named_ref.opt
    { grammar_current_rule_प्रतीक_append ($2, @2, $3); }
| rhs tag.opt "{...}"[action] named_ref.opt[name]
    { grammar_current_rule_action_append ($action, @action, $name, $[tag.opt]); }
| rhs "%?{...}"
    { grammar_current_rule_predicate_append ($2, @2); }
| rhs "%empty"
    { grammar_current_rule_empty_set (@2); }
| rhs "%prec" प्रतीक
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
  %empty         { $$ = शून्य; }
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
  #समावेश "muscle-tab.h"
  प्रकार काष्ठा
  {
    अक्षर स्थिर *chars;
    muscle_kind kind;
  } मूल्य_type;
};
%type <मूल्य_type> मूल्य;
%printer
{
  चयन ($$.kind)
    {
    हाल muscle_code:    ख_लिखो (yyo,  "{%s}",  $$.chars); अवरोध;
    हाल muscle_keyword: ख_लिखो (yyo,   "%s",   $$.chars); अवरोध;
    हाल muscle_string:  ख_लिखो (yyo, "\"%s\"", $$.chars); अवरोध;
    }
} <मूल्य_type>;

मूल्य:
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
    { $$ = प्रतीक_from_uniqstr ($1, @1); }
| CHAR_LITERAL
    {
      स्थिर अक्षर *var = "api.token.raw";
      अगर (current_class == nterm_sym)
        {
          complain (&@1, complaint,
                    _("character literals cannot be nonterminals"));
          YYERROR;
        }
      अगर (muscle_percent_define_ifdef (var))
        {
          complain (&@1, complaint,
                    _("character literals cannot be used together"
                    " with %s"), var);
          location loc = muscle_percent_define_get_loc (var);
          subcomplain (&loc, complaint, _("definition of %s"), var);
        }
      $$ = प्रतीक_get (char_name ($1), @1);
      प्रतीक_class_set ($$, token_sym, @1, false);
      प्रतीक_code_set ($$, $1, @1);
    }
;

id_colon:
  ID_COLON { $$ = प्रतीक_from_uniqstr ($1, @1); }
;


प्रतीक:
  id
| string_as_id
;

/* A string used as an ID.  */
string_as_id:
  STRING
    {
      $$ = प्रतीक_get ($1, @1);
      प्रतीक_class_set ($$, token_sym, @1, false);
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

पूर्णांक
yyreport_syntax_error (स्थिर yypcontext_t *ctx)
{
  पूर्णांक res = 0;
  /* Arguments of format: reported tokens (one for the "unexpected",
     one per "expected"). */
  क्रमागत { ARGS_MAX = 5 };
  स्थिर अक्षर *argv[ARGS_MAX];
  पूर्णांक argc = 0;
  yysymbol_kind_t unexpected = yypcontext_token (ctx);
  अगर (unexpected != YYSYMBOL_YYEMPTY)
    {
      argv[argc++] = yysymbol_name (unexpected);
      yysymbol_kind_t expected[ARGS_MAX - 1];
      पूर्णांक nexpected = yypcontext_expected_tokens (ctx, expected, ARGS_MAX - 1);
      अगर (nexpected < 0)
        res = nexpected;
      अन्यथा
        क्रम (पूर्णांक i = 0; i < nexpected; ++i)
          argv[argc++] = yysymbol_name (expected[i]);
    }
  syntax_error (*yypcontext_location (ctx), argc, argv);
  वापस res;
}


/* Return the location of the left-hand side of a rule whose
   right-hand side is RHS[1] ... RHS[N].  Ignore empty nonterminals in
   the right-hand side, and return an empty location equal to the end
   boundary of RHS[0] if the right-hand side is empty.  */

अटल YYLTYPE
lloc_default (YYLTYPE स्थिर *rhs, पूर्णांक n)
{
  YYLTYPE loc;

  /* SGI MIPSpro 7.4.1m miscompiles "loc.start = loc.end = rhs[n].end;".
     The bug is fixed in 7.4.2m, but play it safe for now.  */
  loc.start = rhs[n].end;
  loc.end = rhs[n].end;

  /* Ignore empty nonterminals the start of the right-hand side.
     Do not bother to ignore them at the end of the right-hand side,
     since empty nonterminals have the same end as their predecessors.  */
  क्रम (पूर्णांक i = 1; i <= n; i++)
    अगर (! equal_boundaries (rhs[i].start, rhs[i].end))
      {
        loc.start = rhs[i].start;
        अवरोध;
      }

  वापस loc;
}

अटल
अक्षर *strip_braces (अक्षर *code)
{
  code[म_माप (code) - 1] = 0;
  वापस code + 1;
}

अटल
अक्षर स्थिर *
translate_code (अक्षर *code, location loc, bool plain)
{
  code_props plain_code;
  अगर (plain)
    code_props_plain_init (&plain_code, code, loc);
  अन्यथा
    code_props_प्रतीक_action_init (&plain_code, code, loc);
  code_props_translate_code (&plain_code);
  gram_scanner_last_string_free ();
  वापस plain_code.code;
}

अटल
अक्षर स्थिर *
translate_code_braceless (अक्षर *code, location loc)
{
  वापस translate_code (strip_braces (code), loc, true);
}

अटल व्योम
add_param (param_type type, अक्षर *decl, location loc)
{
  अटल अक्षर स्थिर alphanum[26 + 26 + 1 + 10 + 1] =
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "_"
    "0123456789";

  अक्षर स्थिर *name_start = शून्य;
  {
    अक्षर *p;
    /* Stop on last actual character.  */
    क्रम (p = decl; p[1]; p++)
      अगर ((p == decl
           || ! स_प्रथम (alphanum, p[-1], माप alphanum - 1))
          && स_प्रथम (alphanum, p[0], माप alphanum - 10 - 1))
        name_start = p;

    /* Strip the surrounding '{' and '}', and any blanks just inside
       the braces.  */
    --p;
    जबतक (c_isspace ((अचिन्हित अक्षर) *p))
      --p;
    p[1] = '\0';
    ++decl;
    जबतक (c_isspace ((अचिन्हित अक्षर) *decl))
      ++decl;
  }

  अगर (! name_start)
    complain (&loc, complaint, _("missing identifier in parameter declaration"));
  अन्यथा
    {
      अक्षर *name = xmemdup0 (name_start, म_अखोज (name_start, alphanum));
      अगर (type & param_lex)
        muscle_pair_list_grow ("lex_param", decl, name);
      अगर (type & param_parse)
        muscle_pair_list_grow ("parse_param", decl, name);
      मुक्त (name);
    }

  gram_scanner_last_string_free ();
}


अटल व्योम
handle_header (अक्षर स्थिर *मूल्य)
{
  header_flag = true;
  अगर (मूल्य)
    {
      अक्षर *file = unquote (मूल्य);
      spec_header_file = xstrdup (file);
      gram_scanner_last_string_free ();
      unquote_free (file);
    }
}


अटल व्योम
handle_error_verbose (location स्थिर *loc, अक्षर स्थिर *directive)
{
  bison_directive (loc, directive);
  muscle_percent_define_जोड़ो (directive, *loc, muscle_keyword, "",
                                MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
}


अटल व्योम
handle_file_prefix (location स्थिर *loc,
                    location स्थिर *dir_loc,
                    अक्षर स्थिर *directive, अक्षर स्थिर *मूल्य_quoted)
{
  अक्षर *मूल्य = unquote (मूल्य_quoted);
  bison_directive (loc, directive);
  bool warned = false;

  अगर (location_empty (spec_file_prefix_loc))
    {
      spec_file_prefix_loc = *loc;
      spec_file_prefix = मूल्य;
    }
  अन्यथा
    {
      duplicate_directive (directive, spec_file_prefix_loc, *loc);
      warned = true;
    }

  अगर (!warned
      && STRNEQ (directive, "%file-prefix"))
    deprecated_directive (dir_loc, directive, "%file-prefix");
}

अटल व्योम
handle_language (location स्थिर *loc, अक्षर स्थिर *lang)
{
  language_argmatch (unquote (lang), grammar_prio, *loc);
}


अटल व्योम
handle_name_prefix (location स्थिर *loc,
                    अक्षर स्थिर *directive, अक्षर स्थिर *मूल्य_quoted)
{
  अक्षर *मूल्य = unquote (मूल्य_quoted);
  bison_directive (loc, directive);

  अक्षर buf1[1024];
  माप_प्रकार len1 = माप (buf1);
  अक्षर *old = asnprintf (buf1, &len1, "%s\"%s\"", directive, मूल्य);
  अगर (!old)
    xalloc_die ();

  अगर (location_empty (spec_name_prefix_loc))
    {
      spec_name_prefix = मूल्य;
      spec_name_prefix_loc = *loc;

      अक्षर buf2[1024];
      माप_प्रकार len2 = माप (buf2);
      अक्षर *new = asnprintf (buf2, &len2, "%%define api.prefix {%s}", मूल्य);
      अगर (!new)
        xalloc_die ();
      deprecated_directive (loc, old, new);
      अगर (new != buf2)
        मुक्त (new);
    }
  अन्यथा
    duplicate_directive (old, spec_file_prefix_loc, *loc);

  अगर (old != buf1)
    मुक्त (old);
}


अटल व्योम
handle_pure_parser (location स्थिर *loc, अक्षर स्थिर *directive)
{
  bison_directive (loc, directive);
  deprecated_directive (loc, directive, "%define api.pure");
  muscle_percent_define_जोड़ो ("api.pure", *loc, muscle_keyword, "",
                                MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
}


अटल व्योम
handle_require (location स्थिर *loc, अक्षर स्थिर *version_quoted)
{
  अक्षर *version = unquote (version_quoted);
  required_version = strversion_to_int (version);
  अगर (required_version == -1)
    {
      complain (loc, complaint, _("invalid version requirement: %s"),
                version);
      required_version = 0;
    }
  अन्यथा
    {
      स्थिर अक्षर* package_version =
        0 < strverscmp (api_version, PACKAGE_VERSION)
        ? api_version : PACKAGE_VERSION;
      अगर (0 < strverscmp (version, package_version))
        {
          complain (loc, complaint, _("require bison %s, but have %s"),
                    version, package_version);
          निकास (EX_MISMATCH);
        }
    }
  unquote_free (version);
  gram_scanner_last_string_free ();
}

अटल व्योम
handle_skeleton (location स्थिर *loc, अक्षर स्थिर *skel_quoted)
{
  अक्षर *skel = unquote (skel_quoted);
  अक्षर स्थिर *skeleton_user = skel;
  अगर (म_अक्षर (skeleton_user, '/'))
    {
      माप_प्रकार dir_लंबाई = म_माप (grammar_file);
      जबतक (dir_लंबाई && grammar_file[dir_length - 1] != '/')
        --dir_length;
      जबतक (dir_लंबाई && grammar_file[dir_length - 1] == '/')
        --dir_length;
      अक्षर *skeleton_build =
        xmalloc (dir_लंबाई + 1 + म_माप (skeleton_user) + 1);
      अगर (dir_लंबाई > 0)
        {
          स_नकल (skeleton_build, grammar_file, dir_लंबाई);
          skeleton_build[dir_length++] = '/';
        }
      म_नकल (skeleton_build + dir_लंबाई, skeleton_user);
      skeleton_user = uniqstr_new (skeleton_build);
      मुक्त (skeleton_build);
    }
  skeleton_arg (skeleton_user, grammar_prio, *loc);
}


अटल व्योम
handle_yacc (location स्थिर *loc)
{
  स्थिर अक्षर *directive = "%yacc";
  bison_directive (loc, directive);
  अगर (location_empty (yacc_loc))
    set_yacc (*loc);
  अन्यथा
    duplicate_directive (directive, yacc_loc, *loc);
}


अटल व्योम
gram_error (location स्थिर *loc, अक्षर स्थिर *संदेश)
{
  complain (loc, complaint, "%s", संदेश);
}

अटल अक्षर स्थिर *
char_name (अक्षर c)
{
  अगर (c == '\'')
    वापस "'\\''";
  अन्यथा
    {
      अक्षर buf[4];
      buf[0] = '\''; buf[1] = c; buf[2] = '\''; buf[3] = '\0';
      वापस quotearg_style (escape_quoting_style, buf);
    }
}

अटल व्योम
current_lhs (प्रतीक *sym, location loc, named_ref *ref)
{
  current_lhs_प्रतीक = sym;
  current_lhs_loc = loc;
  अगर (sym)
    प्रतीक_location_as_lhs_set (sym, loc);
  /* In order to simplify memory management, named references for lhs
     are always assigned by deep copy into the current symbol_list
     node.  This is because a single named-ref in the grammar may
     result in several uses when the user factors lhs between several
     rules using "|".  Therefore free the parser's original copy.  */
  मुक्त (current_lhs_named_ref);
  current_lhs_named_ref = ref;
}

अटल व्योम tron (खाता *yyo)
{
  begin_use_class ("value", yyo);
}

अटल व्योम troff (खाता *yyo)
{
  end_use_class ("value", yyo);
}


/*----------.
| Unquote.  |
`----------*/

काष्ठा obstack obstack_for_unquote;

व्योम
parser_init (व्योम)
{
  obstack_init (&obstack_for_unquote);
}

व्योम
parser_free (व्योम)
{
  obstack_free (&obstack_for_unquote, 0);
}

अटल व्योम
unquote_free (अक्षर *last_string)
{
  obstack_free (&obstack_for_unquote, last_string);
}

अटल अक्षर *
unquote (स्थिर अक्षर *cp)
{
#घोषणा GROW(Char)                              \
  obstack_1grow (&obstack_for_unquote, Char);
  क्रम (++cp; *cp && *cp != '"'; ++cp)
    चयन (*cp)
      {
      हाल '"':
        अवरोध;
      हाल '\\':
        ++cp;
        चयन (*cp)
          {
          हाल '0': हाल '1': हाल '2': हाल '3': हाल '4':
          हाल '5': हाल '6': हाल '7': हाल '8': हाल '9':
            {
              पूर्णांक c = cp[0] - '0';
              अगर (c_isdigit (cp[1]))
                {
                  ++cp;
                  c = c * 8 + cp[0] - '0';
                }
              अगर (c_isdigit (cp[1]))
                {
                  ++cp;
                  c = c * 8 + cp[0] - '0';
                }
              GROW (c);
            }
            अवरोध;

          हाल 'a': GROW ('\a'); अवरोध;
          हाल 'b': GROW ('\b'); अवरोध;
          हाल 'f': GROW ('\f'); अवरोध;
          हाल 'n': GROW ('\n'); अवरोध;
          हाल 'r': GROW ('\r'); अवरोध;
          हाल 't': GROW ('\t'); अवरोध;
          हाल 'v': GROW ('\v'); अवरोध;

          हाल 'x':
            {
              पूर्णांक c = 0;
              जबतक (c_isxdigit (cp[1]))
                {
                  ++cp;
                  c = (c * 16 + (c_isdigit (cp[0]) ? cp[0] - '0'
                                 : c_isupper (cp[0]) ? cp[0] - 'A'
                                 : cp[0] - '0'));
                }
              GROW (c);
              अवरोध;
            }
          }
        अवरोध;

      शेष:
        GROW (*cp);
        अवरोध;
      }
  निश्चित (*cp == '"');
  ++cp;
  निश्चित (*cp == '\0');
#अघोषित GROW
  वापस obstack_finish0 (&obstack_for_unquote);
}
