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
  #ضمن "symlist.h"
  #ضمن "symtab.h"
}

%code provides
{
  /* Initialize unquote.  */
  فراغ parser_init (فراغ);
  /* Deallocate storage for unquote.  */
  فراغ parser_free (فراغ);
}

%code چوٹی_2
{
  /* On column 0 to please syntax-check.  */
#ضمن <config.h>
}

%code
{
  #ضمن "system.h"

  #ضمن <c-ctype.h>
  #ضمن <quotearg.h>
  #ضمن <vasnprintf.h>
  #ضمن <xmemdup0.h>

  #ضمن "complain.h"
  #ضمن "conflicts.h"
  #ضمن "files.h"
  #ضمن "getargs.h"
  #ضمن "gram.h"
  #ضمن "named-ref.h"
  #ضمن "reader.h"
  #ضمن "scan-code.h"
  #ضمن "scan-gram.h"
  #ضمن "strversion.h"

  /* Pretend to be at least that version, to check features published
     in that version while developping it.  */
  ساكن ثابت حرف* api_version = "3.8";

  ساكن صحيح current_prec = 0;
  ساكن location current_lhs_loc;
  ساكن named_ref *current_lhs_named_ref;
  ساكن علامت *current_lhs_علامت;
  ساكن علامت_class current_class = unknown_sym;

  /** Set the new current left-hand side symbol, possibly common
   * to several right-hand side parts of rule.
   */
  ساكن فراغ current_lhs (علامت *sym, location loc, named_ref *ref);

  #define YYLLOC_DEFAULT(Current, Rhs, N)         \
    (Current) = lloc_default (Rhs, N)
  ساكن YYLTYPE lloc_default (YYLTYPE ثابت *, صحيح);

  #define YY_LOCATION_PRINT(File, Loc)            \
    location_print (Loc, File)

  /* Strip initial '{' and final '}' (must be first and last characters).
     Return the result.  */
  ساكن حرف *strip_braces (حرف *code);

  /* Convert CODE by calling code_props_plain_init if PLAIN, otherwise
     code_props_symbol_action_init.  Calls
     gram_scanner_last_string_free to release the latest string from
     the scanner (should be CODE). */
  ساكن حرف ثابت *translate_code (حرف *code, location loc, bool plain);

  /* Convert CODE by calling code_props_plain_init after having
     stripped the first and last characters (expected to be '{', and
     '}').  Calls gram_scanner_last_string_free to release the latest
     string from the scanner (should be CODE). */
  ساكن حرف ثابت *translate_code_braceless (حرف *code, location loc);

  /* Handle a %header directive.  */
  ساكن فراغ handle_header (حرف ثابت *قدر);

  /* Handle a %error-verbose directive.  */
  ساكن فراغ handle_error_verbose (location ثابت *loc, حرف ثابت *directive);

  /* Handle a %file-prefix directive.  */
  ساكن فراغ handle_file_prefix (location ثابت *loc,
                                  location ثابت *dir_loc,
                                  حرف ثابت *directive, حرف ثابت *قدر);

  /* Handle a %language directive.  */
  ساكن فراغ handle_language (location ثابت *loc, حرف ثابت *lang);

  /* Handle a %name-prefix directive.  */
  ساكن فراغ handle_name_prefix (location ثابت *loc,
                                  حرف ثابت *directive, حرف ثابت *قدر);

  /* Handle a %pure-parser directive.  */
  ساكن فراغ handle_pure_parser (location ثابت *loc, حرف ثابت *directive);

  /* Handle a %require directive.  */
  ساكن فراغ handle_require (location ثابت *loc, حرف ثابت *version);

  /* Handle a %skeleton directive.  */
  ساكن فراغ handle_skeleton (location ثابت *loc, حرف ثابت *skel);

  /* Handle a %yacc directive.  */
  ساكن فراغ handle_yacc (location ثابت *loc);

  /* Implementation of yyerror.  */
  ساكن فراغ gram_error (location ثابت *, حرف ثابت *);

  /* A string that describes a char (e.g., 'a' -> "'a'").  */
  ساكن حرف ثابت *char_name (حرف);

  /* Add style to semantic values in traces.  */
  ساكن فراغ tron (FILE *yyo);
  ساكن فراغ troff (FILE *yyo);

  /* Interpret a quoted string (such as `"Hello, \"World\"\n\""`).
     Manages the memory of the result.  */
  ساكن حرف *unquote (ثابت حرف *str);

  /* Discard the latest unquoted string.  */
  ساكن فراغ unquote_free (حرف *last_string);
}

%define api.header.include {"parse-gram.h"}
%define api.prefix {gram_}
%define api.pure full
%define api.token.raw
%define api.قدر.نوع اتحاد
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

  PERCENT_بایاں        "%left"
  PERCENT_دایاں       "%right"
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
  PERCENT_NO_سطریں        "%no-lines"
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

%نوع <unsigned حرف> CHAR_LITERAL
%printer { fputs (char_name ($$), yyo); } <unsigned حرف>

%نوع <حرف*> "{...}" "%?{...}" "%{...%}" EPILOGUE STRING TSTRING
%printer { fputs ($$, yyo); } <حرف*>

%نوع <uniqstr>
  BRACKETED_ID ID ID_COLON
  PERCENT_ERROR_VERBOSE PERCENT_FILE_PREFIX PERCENT_FLAG PERCENT_NAME_PREFIX
  PERCENT_PURE_PARSER
  TAG tag tag.opt variable
%printer { fputs ($$, yyo); } <uniqstr>
%printer { fprintf (yyo, "[%s]", $$); } BRACKETED_ID
%printer { fprintf (yyo, "%s:", $$); } ID_COLON
%printer { fprintf (yyo, "%%%s", $$); } PERCENT_FLAG
%printer { fprintf (yyo, "<%s>", $$); } TAG tag

%token <صحيح> INT_LITERAL _("integer literal")
%printer { fprintf (yyo, "%d", $$); } <صحيح>

%نوع <علامت*> id id_colon string_as_id علامت token_decl token_decl_for_prec
%printer { fprintf (yyo, "%s", $$ ? $$->tag : "<NULL>"); } <علامت*>
%printer { fprintf (yyo, "%s:", $$->tag); } id_colon

%نوع <assoc> precedence_declarator

%destructor { علامت_list_free ($$); } <علامت_list*>
%printer { علامت_list_syms_print ($$, yyo); } <علامت_list*>

%نوع <named_ref*> named_ref.opt

/*---------.
| %param.  |
`---------*/
%code requires
{
  تعريفنوع تعداد
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
  ساكن فراغ add_param (param_type نوع, حرف *decl, location loc);
  ساكن param_type current_param = param_none;
};
%token <param_type> PERCENT_PARAM "%param";
%printer
{
  بدل ($$)
    {
#define CASE(In, Out)                                           \
      حالة param_ ## In: fputs ("%" #Out, yyo); اكسر
      CASE (lex,   lex-param);
      CASE (parse, parse-param);
      CASE (both,  param);
#undef CASE
      حالة param_none: aver (false); اكسر;
    }
} <param_type>;


                     /*==========\
                     | Grammar.  |
                     \==========*/
%%

داخلہ:
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
| "%define" variable قدر
    {
      muscle_percent_define_داخل_کرو ($2, @$, $3.kind, $3.chars,
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
| "%no-lines"                   { no_سطریں_flag = true; }
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
  علامت_declaration
| "%start" symbols.1
    {
      grammar_start_symbols_add ($2);
    }
| code_props_type "{...}" generic_symlist
    {
      code_props code;
      code_props_علامت_action_init (&code, $2, @2);
      code_props_translate_code (&code);
      {
        لكل (علامت_list *list = $3; list; list = list->next)
          علامت_list_code_props_set (list, $1, &code);
        علامت_list_free ($3);
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

%نوع <code_props_type> code_props_type;
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
| ID     { muscle_percent_define_داخل_کرو ("api.value.union.name",
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


%نوع <علامت_list*> nterm_decls علامت_decls symbols.1
      token_decls token_decls_for_prec
      token_decl.1 token_decl_for_prec.1;
علامت_declaration:
  "%nterm" { current_class = nterm_sym; } nterm_decls[syms]
    {
      current_class = unknown_sym;
      علامت_list_free ($syms);
    }
| "%token" { current_class = token_sym; } token_decls[syms]
    {
      current_class = unknown_sym;
      علامت_list_free ($syms);
    }
| "%type" { current_class = pct_type_sym; } علامت_decls[syms]
    {
      current_class = unknown_sym;
      علامت_list_free ($syms);
    }
| precedence_declarator token_decls_for_prec[syms]
    {
      ++current_prec;
      لكل (علامت_list *list = $syms; list; list = list->next)
        علامت_precedence_set (list->content.sym, current_prec, $1, @1);
      علامت_list_free ($syms);
    }
;

precedence_declarator:
  "%left"       { $$ = بایاں_assoc; }
| "%right"      { $$ = دایاں_assoc; }
| "%nonassoc"   { $$ = non_assoc; }
| "%precedence" { $$ = precedence_assoc; }
;

%نوع <حرف*> string.opt;
string.opt:
  %empty  { $$ = عدم; }
| STRING  { $$ = $1; }
;

tag.opt:
  %empty { $$ = عدم; }
| TAG    { $$ = $1; }
;

%نوع <علامت_list*> generic_symlist generic_symlist_item;
generic_symlist:
  generic_symlist_item
| generic_symlist generic_symlist_item   { $$ = علامت_list_append ($1, $2); }
;

generic_symlist_item:
  علامت    { $$ = علامت_list_sym_new ($1, @1); }
| tag       { $$ = علامت_list_type_new ($1, @1); }
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
      $$ = علامت_list_type_set ($syms, $TAG);
    }
| token_decls TAG token_decl.1[syms]
    {
      $$ = علامت_list_append ($1, علامت_list_type_set ($syms, $TAG));
    }
;

// One or more symbol declarations for %token or %nterm.
token_decl.1:
  token_decl                { $$ = علامت_list_sym_new ($1, @1); }
| token_decl.1 token_decl   { $$ = علامت_list_append ($1, علامت_list_sym_new ($2, @2)); }

// One symbol declaration for %token or %nterm.
token_decl:
  id صحيح.opt[num] alias
    {
      $$ = $id;
      علامت_class_set ($id, current_class, @id, true);
      إذا (0 <= $عدد)
        علامت_code_set ($id, $عدد, @عدد);
      إذا ($alias)
        علامت_make_alias ($id, $alias, @alias);
    }
;

%نوع <صحيح> صحيح.opt;
صحيح.opt:
  %empty  { $$ = -1; }
| INT_LITERAL
;

%نوع <علامت*> alias;
alias:
  %empty         { $$ = عدم; }
| string_as_id   { $$ = $1; }
| TSTRING
    {
      $$ = علامت_get ($1, @1);
      علامت_class_set ($$, token_sym, @1, false);
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
      $$ = علامت_list_type_set ($syms, $TAG);
    }
| token_decls_for_prec TAG token_decl_for_prec.1[syms]
    {
      $$ = علامت_list_append ($1, علامت_list_type_set ($syms, $TAG));
    }
;

// One or more token declarations for precedence declaration.
token_decl_for_prec.1:
  token_decl_for_prec
    { $$ = علامت_list_sym_new ($1, @1); }
| token_decl_for_prec.1 token_decl_for_prec
    { $$ = علامت_list_append ($1, علامت_list_sym_new ($2, @2)); }

// One token declaration for precedence declaration.
token_decl_for_prec:
  id صحيح.opt[num]
    {
      $$ = $id;
      علامت_class_set ($id, token_sym, @id, false);
      إذا (0 <= $عدد)
        علامت_code_set ($id, $عدد, @عدد);
    }
| string_as_id
;


/*-----------------------------------.
| symbol_decls (argument of %type).  |
`-----------------------------------*/

// A non empty list of typed symbols (for %type).
علامت_decls:
  symbols.1[syms]
    {
      $$ = $syms;
    }
| TAG symbols.1[syms]
    {
      $$ = علامت_list_type_set ($syms, $TAG);
    }
| علامت_decls TAG symbols.1[syms]
    {
      $$ = علامت_list_append ($1, علامت_list_type_set ($syms, $TAG));
    }
;

// One or more symbols.
symbols.1:
  علامت
    {
      إذا (current_class != unknown_sym)
        علامت_class_set ($علامت, current_class, @علامت, false);
      $$ = علامت_list_sym_new ($علامت, @علامت);
    }
  | symbols.1 علامت
    {
      إذا (current_class != unknown_sym)
        علامت_class_set ($علامت, current_class, @علامت, false);
      $$ = علامت_list_append ($1, علامت_list_sym_new ($علامت, @علامت));
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
    { grammar_current_rule_begin (current_lhs_علامت, current_lhs_loc,
                                  current_lhs_named_ref); }
| rhs علامت named_ref.opt
    { grammar_current_rule_علامت_append ($2, @2, $3); }
| rhs tag.opt "{...}"[action] named_ref.opt[name]
    { grammar_current_rule_action_append ($action, @action, $name, $[tag.opt]); }
| rhs "%?{...}"
    { grammar_current_rule_predicate_append ($2, @2); }
| rhs "%empty"
    { grammar_current_rule_empty_set (@2); }
| rhs "%prec" علامت
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
  %empty         { $$ = عدم; }
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
  #ضمن "muscle-tab.h"
  تعريفنوع بنية
  {
    حرف ثابت *chars;
    muscle_kind kind;
  } قدر_type;
};
%نوع <قدر_type> قدر;
%printer
{
  بدل ($$.kind)
    {
    حالة muscle_code:    fprintf (yyo,  "{%s}",  $$.chars); اكسر;
    حالة muscle_keyword: fprintf (yyo,   "%s",   $$.chars); اكسر;
    حالة muscle_string:  fprintf (yyo, "\"%s\"", $$.chars); اكسر;
    }
} <قدر_type>;

قدر:
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
    { $$ = علامت_from_uniqstr ($1, @1); }
| CHAR_LITERAL
    {
      ثابت حرف *var = "api.token.raw";
      إذا (current_class == nterm_sym)
        {
          complain (&@1, complaint,
                    _("character literals cannot be nonterminals"));
          YYERROR;
        }
      إذا (muscle_percent_define_ifdef (var))
        {
          complain (&@1, complaint,
                    _("character literals cannot be used together"
                    " with %s"), var);
          location loc = muscle_percent_define_get_loc (var);
          subcomplain (&loc, complaint, _("definition of %s"), var);
        }
      $$ = علامت_get (char_name ($1), @1);
      علامت_class_set ($$, token_sym, @1, false);
      علامت_code_set ($$, $1, @1);
    }
;

id_colon:
  ID_COLON { $$ = علامت_from_uniqstr ($1, @1); }
;


علامت:
  id
| string_as_id
;

/* A string used as an ID.  */
string_as_id:
  STRING
    {
      $$ = علامت_get ($1, @1);
      علامت_class_set ($$, token_sym, @1, false);
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

صحيح
yyreport_syntax_error (ثابت yypcontext_t *ctx)
{
  صحيح res = 0;
  /* Arguments of format: reported tokens (one for the "unexpected",
     one per "expected"). */
  تعداد { ARGS_MAX = 5 };
  ثابت حرف *argv[ARGS_MAX];
  صحيح argc = 0;
  yysymbol_kind_t unexpected = yypcontext_token (ctx);
  إذا (unexpected != YYSYMBOL_YYEMPTY)
    {
      argv[argc++] = yysymbol_name (unexpected);
      yysymbol_kind_t expected[ARGS_MAX - 1];
      صحيح nexpected = yypcontext_expected_tokens (ctx, expected, ARGS_MAX - 1);
      إذا (nexpected < 0)
        res = nexpected;
      وإلا
        لكل (صحيح i = 0; i < nexpected; ++i)
          argv[argc++] = yysymbol_name (expected[i]);
    }
  syntax_error (*yypcontext_location (ctx), argc, argv);
  أرجع res;
}


/* Return the location of the left-hand side of a rule whose
   right-hand side is RHS[1] ... RHS[N].  Ignore empty nonterminals in
   the right-hand side, and return an empty location equal to the end
   boundary of RHS[0] if the right-hand side is empty.  */

ساكن YYLTYPE
lloc_default (YYLTYPE ثابت *rhs, صحيح n)
{
  YYLTYPE loc;

  /* SGI MIPSpro 7.4.1m miscompiles "loc.start = loc.end = rhs[n].end;".
     The bug is fixed in 7.4.2m, but play it safe for now.  */
  loc.start = rhs[n].end;
  loc.end = rhs[n].end;

  /* Ignore empty nonterminals the start of the right-hand side.
     Do not bother to ignore them at the end of the right-hand side,
     since empty nonterminals have the same end as their predecessors.  */
  لكل (صحيح i = 1; i <= n; i++)
    إذا (! equal_boundaries (rhs[i].start, rhs[i].end))
      {
        loc.start = rhs[i].start;
        اكسر;
      }

  أرجع loc;
}

ساكن
حرف *strip_braces (حرف *code)
{
  code[strlen (code) - 1] = 0;
  أرجع code + 1;
}

ساكن
حرف ثابت *
translate_code (حرف *code, location loc, bool plain)
{
  code_props plain_code;
  إذا (plain)
    code_props_plain_init (&plain_code, code, loc);
  وإلا
    code_props_علامت_action_init (&plain_code, code, loc);
  code_props_translate_code (&plain_code);
  gram_scanner_last_string_free ();
  أرجع plain_code.code;
}

ساكن
حرف ثابت *
translate_code_braceless (حرف *code, location loc)
{
  أرجع translate_code (strip_braces (code), loc, true);
}

ساكن فراغ
add_param (param_type نوع, حرف *decl, location loc)
{
  ساكن حرف ثابت alphanum[26 + 26 + 1 + 10 + 1] =
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "_"
    "0123456789";

  حرف ثابت *name_start = عدم;
  {
    حرف *p;
    /* Stop on last actual character.  */
    لكل (p = decl; p[1]; p++)
      إذا ((p == decl
           || ! memchr (alphanum, p[-1], حجم alphanum - 1))
          && memchr (alphanum, p[0], حجم alphanum - 10 - 1))
        name_start = p;

    /* Strip the surrounding '{' and '}', and any blanks just inside
       the braces.  */
    --p;
    طالما (c_isspace ((unsigned حرف) *p))
      --p;
    p[1] = '\0';
    ++decl;
    طالما (c_isspace ((unsigned حرف) *decl))
      ++decl;
  }

  إذا (! name_start)
    complain (&loc, complaint, _("missing identifier in parameter declaration"));
  وإلا
    {
      حرف *name = xmemdup0 (name_start, strspn (name_start, alphanum));
      إذا (نوع & param_lex)
        muscle_pair_list_grow ("lex_param", decl, name);
      إذا (نوع & param_parse)
        muscle_pair_list_grow ("parse_param", decl, name);
      حرر (name);
    }

  gram_scanner_last_string_free ();
}


ساكن فراغ
handle_header (حرف ثابت *قدر)
{
  header_flag = true;
  إذا (قدر)
    {
      حرف *file = unquote (قدر);
      spec_header_file = xstrdup (file);
      gram_scanner_last_string_free ();
      unquote_free (file);
    }
}


ساكن فراغ
handle_error_verbose (location ثابت *loc, حرف ثابت *directive)
{
  bison_directive (loc, directive);
  muscle_percent_define_داخل_کرو (directive, *loc, muscle_keyword, "",
                                MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
}


ساكن فراغ
handle_file_prefix (location ثابت *loc,
                    location ثابت *dir_loc,
                    حرف ثابت *directive, حرف ثابت *قدر_quoted)
{
  حرف *قدر = unquote (قدر_quoted);
  bison_directive (loc, directive);
  bool warned = false;

  إذا (location_empty (spec_file_prefix_loc))
    {
      spec_file_prefix_loc = *loc;
      spec_file_prefix = قدر;
    }
  وإلا
    {
      duplicate_directive (directive, spec_file_prefix_loc, *loc);
      warned = true;
    }

  إذا (!warned
      && STRNEQ (directive, "%file-prefix"))
    deprecated_directive (dir_loc, directive, "%file-prefix");
}

ساكن فراغ
handle_language (location ثابت *loc, حرف ثابت *lang)
{
  language_argmatch (unquote (lang), grammar_prio, *loc);
}


ساكن فراغ
handle_name_prefix (location ثابت *loc,
                    حرف ثابت *directive, حرف ثابت *قدر_quoted)
{
  حرف *قدر = unquote (قدر_quoted);
  bison_directive (loc, directive);

  حرف buf1[1024];
  size_t len1 = حجم (buf1);
  حرف *old = asnprintf (buf1, &len1, "%s\"%s\"", directive, قدر);
  إذا (!old)
    xalloc_die ();

  إذا (location_empty (spec_name_prefix_loc))
    {
      spec_name_prefix = قدر;
      spec_name_prefix_loc = *loc;

      حرف buf2[1024];
      size_t len2 = حجم (buf2);
      حرف *new = asnprintf (buf2, &len2, "%%define api.prefix {%s}", قدر);
      إذا (!new)
        xalloc_die ();
      deprecated_directive (loc, old, new);
      إذا (new != buf2)
        حرر (new);
    }
  وإلا
    duplicate_directive (old, spec_file_prefix_loc, *loc);

  إذا (old != buf1)
    حرر (old);
}


ساكن فراغ
handle_pure_parser (location ثابت *loc, حرف ثابت *directive)
{
  bison_directive (loc, directive);
  deprecated_directive (loc, directive, "%define api.pure");
  muscle_percent_define_داخل_کرو ("api.pure", *loc, muscle_keyword, "",
                                MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
}


ساكن فراغ
handle_require (location ثابت *loc, حرف ثابت *version_quoted)
{
  حرف *version = unquote (version_quoted);
  required_version = strversion_to_int (version);
  إذا (required_version == -1)
    {
      complain (loc, complaint, _("invalid version requirement: %s"),
                version);
      required_version = 0;
    }
  وإلا
    {
      ثابت حرف* package_version =
        0 < strverscmp (api_version, PACKAGE_VERSION)
        ? api_version : PACKAGE_VERSION;
      إذا (0 < strverscmp (version, package_version))
        {
          complain (loc, complaint, _("require bison %s, but have %s"),
                    version, package_version);
          exit (EX_MISMATCH);
        }
    }
  unquote_free (version);
  gram_scanner_last_string_free ();
}

ساكن فراغ
handle_skeleton (location ثابت *loc, حرف ثابت *skel_quoted)
{
  حرف *skel = unquote (skel_quoted);
  حرف ثابت *skeleton_user = skel;
  إذا (strchr (skeleton_user, '/'))
    {
      size_t dir_length = strlen (grammar_file);
      طالما (dir_length && grammar_file[dir_length - 1] != '/')
        --dir_length;
      طالما (dir_length && grammar_file[dir_length - 1] == '/')
        --dir_length;
      حرف *skeleton_build =
        xmalloc (dir_length + 1 + strlen (skeleton_user) + 1);
      إذا (dir_length > 0)
        {
          memcpy (skeleton_build, grammar_file, dir_length);
          skeleton_build[dir_length++] = '/';
        }
      strcpy (skeleton_build + dir_length, skeleton_user);
      skeleton_user = uniqstr_new (skeleton_build);
      حرر (skeleton_build);
    }
  skeleton_arg (skeleton_user, grammar_prio, *loc);
}


ساكن فراغ
handle_yacc (location ثابت *loc)
{
  ثابت حرف *directive = "%yacc";
  bison_directive (loc, directive);
  إذا (location_empty (yacc_loc))
    set_yacc (*loc);
  وإلا
    duplicate_directive (directive, yacc_loc, *loc);
}


ساكن فراغ
gram_error (location ثابت *loc, حرف ثابت *پیغام)
{
  complain (loc, complaint, "%s", پیغام);
}

ساكن حرف ثابت *
char_name (حرف c)
{
  إذا (c == '\'')
    أرجع "'\\''";
  وإلا
    {
      حرف buf[4];
      buf[0] = '\''; buf[1] = c; buf[2] = '\''; buf[3] = '\0';
      أرجع quotearg_style (escape_quoting_style, buf);
    }
}

ساكن فراغ
current_lhs (علامت *sym, location loc, named_ref *ref)
{
  current_lhs_علامت = sym;
  current_lhs_loc = loc;
  إذا (sym)
    علامت_location_as_lhs_set (sym, loc);
  /* In order to simplify memory management, named references for lhs
     are always assigned by deep copy into the current symbol_list
     node.  This is because a single named-ref in the grammar may
     result in several uses when the user factors lhs between several
     rules using "|".  Therefore free the parser's original copy.  */
  حرر (current_lhs_named_ref);
  current_lhs_named_ref = ref;
}

ساكن فراغ tron (FILE *yyo)
{
  begin_use_class ("value", yyo);
}

ساكن فراغ troff (FILE *yyo)
{
  end_use_class ("value", yyo);
}


/*----------.
| Unquote.  |
`----------*/

بنية obstack obstack_for_unquote;

فراغ
parser_init (فراغ)
{
  obstack_init (&obstack_for_unquote);
}

فراغ
parser_free (فراغ)
{
  obstack_free (&obstack_for_unquote, 0);
}

ساكن فراغ
unquote_free (حرف *last_string)
{
  obstack_free (&obstack_for_unquote, last_string);
}

ساكن حرف *
unquote (ثابت حرف *cp)
{
#define GROW(Char)                              \
  obstack_1grow (&obstack_for_unquote, Char);
  لكل (++cp; *cp && *cp != '"'; ++cp)
    بدل (*cp)
      {
      حالة '"':
        اكسر;
      حالة '\\':
        ++cp;
        بدل (*cp)
          {
          حالة '0': حالة '1': حالة '2': حالة '3': حالة '4':
          حالة '5': حالة '6': حالة '7': حالة '8': حالة '9':
            {
              صحيح c = cp[0] - '0';
              إذا (c_isdigit (cp[1]))
                {
                  ++cp;
                  c = c * 8 + cp[0] - '0';
                }
              إذا (c_isdigit (cp[1]))
                {
                  ++cp;
                  c = c * 8 + cp[0] - '0';
                }
              GROW (c);
            }
            اكسر;

          حالة 'a': GROW ('\a'); اكسر;
          حالة 'b': GROW ('\b'); اكسر;
          حالة 'f': GROW ('\f'); اكسر;
          حالة 'n': GROW ('\n'); اكسر;
          حالة 'r': GROW ('\r'); اكسر;
          حالة 't': GROW ('\t'); اكسر;
          حالة 'v': GROW ('\v'); اكسر;

          حالة 'x':
            {
              صحيح c = 0;
              طالما (c_isxdigit (cp[1]))
                {
                  ++cp;
                  c = (c * 16 + (c_isdigit (cp[0]) ? cp[0] - '0'
                                 : c_isupper (cp[0]) ? cp[0] - 'A'
                                 : cp[0] - '0'));
                }
              GROW (c);
              اكسر;
            }
          }
        اكسر;

      افتراضي:
        GROW (*cp);
        اكسر;
      }
  أكد (*cp == '"');
  ++cp;
  أكد (*cp == '\0');
#undef GROW
  أرجع obstack_finish0 (&obstack_for_unquote);
}
