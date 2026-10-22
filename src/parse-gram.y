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

%code سر_2
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
  ساكن ثابت حرف* api_نسخه = "3.8";

  ساكن صحيح current_prec = 0;
  ساكن location current_lhs_loc;
  ساكن named_ref *current_lhs_named_ref;
  ساكن نښه *current_lhs_نښه;
  ساكن نښه_class current_class = unknown_sym;

  /** Set the new current left-hand side symbol, possibly common
   * to several right-hand side parts of rule.
   */
  ساكن فراغ current_lhs (نښه *sym, location loc, named_ref *ref);

  #تعریف_کړه YYLLOC_DEFAULT(Current, Rhs, N)         \
    (Current) = lloc_default (Rhs, N)
  ساكن YYLTYPE lloc_default (YYLTYPE ثابت *, صحيح);

  #تعریف_کړه YY_LOCATION_PRINT(File, Loc)            \
    location_print (Loc, File)

  /* Strip initial '{' and final '}' (must be first and last characters).
     Return the result.  */
  ساكن حرف *strip_braces (حرف *کوډ);

  /* Convert CODE by calling code_props_plain_init if PLAIN, otherwise
     code_props_symbol_action_init.  Calls
     gram_scanner_last_string_free to release the latest string from
     the scanner (should be CODE). */
  ساكن حرف ثابت *translate_کوډ (حرف *کوډ, location loc, bool plain);

  /* Convert CODE by calling code_props_plain_init after having
     stripped the first and last characters (expected to be '{', and
     '}').  Calls gram_scanner_last_string_free to release the latest
     string from the scanner (should be CODE). */
  ساكن حرف ثابت *translate_کوډ_braceless (حرف *کوډ, location loc);

  /* Handle a %header directive.  */
  ساكن فراغ handle_سرلیک (حرف ثابت *ارزښت);

  /* Handle a %error-verbose directive.  */
  ساكن فراغ handle_error_verbose (location ثابت *loc, حرف ثابت *directive);

  /* Handle a %file-prefix directive.  */
  ساكن فراغ handle_file_مختاړی (location ثابت *loc,
                                  location ثابت *dir_loc,
                                  حرف ثابت *directive, حرف ثابت *ارزښت);

  /* Handle a %language directive.  */
  ساكن فراغ handle_language (location ثابت *loc, حرف ثابت *lang);

  /* Handle a %name-prefix directive.  */
  ساكن فراغ handle_name_مختاړی (location ثابت *loc,
                                  حرف ثابت *directive, حرف ثابت *ارزښت);

  /* Handle a %pure-parser directive.  */
  ساكن فراغ handle_خالص_parser (location ثابت *loc, حرف ثابت *directive);

  /* Handle a %require directive.  */
  ساكن فراغ handle_require (location ثابت *loc, حرف ثابت *نسخه);

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
  ساكن فراغ unquote_free (حرف *وروستی_string);
}

%define api.سرلیک.include {"parse-gram.h"}
%define api.مختاړی {gram_}
%define api.خالص ډک
%define api.token.raw
%define api.ارزښت.نوع اتحاد
%define locations
%define parse.error custom
%define parse.lac ډک
%define parse.trace
%header
%expect 0
%verbose

%initial-action
{
  /* Bison's grammar can initial empty locations, hence a default
     location is needed. */
  boundary_سېټ (&@$.پیل, grammar_file, 1, 1, 1);
  boundary_سېټ (&@$.پای_ټکی, grammar_file, 1, 1, 1);
}

%token
  STRING              _("string")
  TSTRING             _("translatable string")

  PERCENT_TOKEN       "%token"
  PERCENT_NTERM       "%nterm"

  PERCENT_TYPE        "%type"
  PERCENT_DESTRUCTOR  "%destructor"
  PERCENT_PRINTER     "%printer"

  PERCENT_کیڼ        "%left"
  PERCENT_ښی       "%right"
  PERCENT_NONASSOC    "%nonassoc"
  PERCENT_PRECEDENCE  "%precedence"

  PERCENT_PREC        "%prec"
  PERCENT_DPREC       "%dprec"
  PERCENT_یوځای       "%merge"

  PERCENT_کوډ            "%code"
  PERCENT_DEFAULT_PREC    "%default-prec"
  PERCENT_DEFINE          "%define"
  PERCENT_ERROR_VERBOSE   "%error-verbose"
  PERCENT_EXPECT          "%expect"
  PERCENT_EXPECT_RR       "%expect-rr"
  PERCENT_FILE_مختاړی     "%file-prefix"
  PERCENT_FLAG            "%<flag>"
  PERCENT_GLR_PARSER      "%glr-parser"
  PERCENT_سرلیک          "%header"
  PERCENT_INITIAL_ACTION  "%initial-action"
  PERCENT_LANGUAGE        "%language"
  PERCENT_NAME_مختاړی     "%name-prefix"
  PERCENT_NO_DEFAULT_PREC "%no-default-prec"
  PERCENT_NO_کرښې        "%no-lines"
  PERCENT_NONDETERMINISTIC_PARSER "%nondeterministic-parser"
  PERCENT_OUTPUT          "%output"
  PERCENT_خالص_PARSER     "%pure-parser"
  PERCENT_REQUIRE         "%require"
  PERCENT_SKELETON        "%skeleton"
  PERCENT_پیل           "%start"
  PERCENT_TOKEN_جدول     "%token-table"
  PERCENT_VERBOSE         "%verbose"
  PERCENT_YACC            "%yacc"

  BRACED_کوډ       "{...}"
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
  نښان_2               _("<tag>")
  نښان_ANY           "<*>"
  نښان_NONE          "<>"

 /* Experimental feature, don't rely on it.  */
%code pre-printer  {tron (yyo);}
%code post-printer {troff (yyo);}

%نوع <بینښه حرف> CHAR_LITERAL
%printer { fputs (char_name ($$), yyo); } <بینښه حرف>

%نوع <حرف*> "{...}" "%?{...}" "%{...%}" EPILOGUE STRING TSTRING
%printer { fputs ($$, yyo); } <حرف*>

%نوع <uniqstr>
  BRACKETED_ID ID ID_COLON
  PERCENT_ERROR_VERBOSE PERCENT_FILE_مختاړی PERCENT_FLAG PERCENT_NAME_مختاړی
  PERCENT_خالص_PARSER
  نښان_2 نښان نښان.opt variable
%printer { fputs ($$, yyo); } <uniqstr>
%printer { fprintf (yyo, "[%s]", $$); } BRACKETED_ID
%printer { fprintf (yyo, "%s:", $$); } ID_COLON
%printer { fprintf (yyo, "%%%s", $$); } PERCENT_FLAG
%printer { fprintf (yyo, "<%s>", $$); } نښان_2 نښان

%token <صحيح> INT_LITERAL _("integer literal")
%printer { fprintf (yyo, "%d", $$); } <صحيح>

%نوع <نښه*> id id_colon string_as_id نښه token_decl token_decl_for_prec
%printer { fprintf (yyo, "%s", $$ ? $$->نښان : "<NULL>"); } <نښه*>
%printer { fprintf (yyo, "%s:", $$->نښان); } id_colon

%نوع <assoc> precedence_declarator

%destructor { نښه_list_free ($$); } <نښه_list*>
%printer { نښه_list_syms_print ($$, yyo); } <نښه_list*>

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
  ساكن فراغ جمع_کړه_param (param_type نوع, حرف *decl, location loc);
  ساكن param_type current_param = param_none;
};
%token <param_type> PERCENT_PARAM "%param";
%printer
{
  بدل ($$)
    {
#تعریف_کړه CASE(دننه_2, بهر_2)                                           \
      حالة param_ ## دننه_2: fputs ("%" #Out, yyo); اكسر
      CASE (lex,   lex-param);
      CASE (parse, parse-param);
      CASE (both,  param);
#تعریف_لغوه CASE
      حالة param_none: aver (false); اكسر;
    }
} <param_type>;


                     /*==========\
                     | Grammar.  |
                     \==========*/
%%

ننوت:
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
      muscle_کوډ_grow (union_کتل_شوی ? "post_prologue" : "pre_prologue",
                        translate_کوډ ($1, @1, true), @1);
      کوډ_scanner_وروستی_string_free ();
    }
| "%<flag>"
    {
      muscle_percent_define_ensure ($1, @1, true);
    }
| "%define" variable ارزښت
    {
      muscle_percent_define_ورننباسه ($2, @$, $3.ډول, $3.chars,
                                    MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
    }
| "%header" string.opt             { handle_سرلیک ($2); }
| "%error-verbose"                 { handle_error_verbose (&@$, $1); }
| "%expect" INT_LITERAL            { expected_sr_conflicts = $2; }
| "%expect-rr" INT_LITERAL         { expected_rr_conflicts = $2; }
| "%file-prefix" STRING            { handle_file_مختاړی (&@$, &@1, $1, $2); }
| "%glr-parser"
    {
      nondeterministic_parser = true;
      glr_parser = true;
    }
| "%initial-action" "{...}"
    {
      muscle_کوډ_grow ("initial_action", translate_کوډ ($2, @2, false), @2);
      کوډ_scanner_وروستی_string_free ();
    }
| "%language" STRING            { handle_language (&@1, $2); }
| "%name-prefix" STRING         { handle_name_مختاړی (&@$, $1, $2); }
| "%no-lines"                   { no_کرښې_flag = true; }
| "%nondeterministic-parser"    { nondeterministic_parser = true; }
| "%output" STRING              { spec_outfile = unquote ($2); gram_scanner_وروستی_string_free (); }
| "%param" { current_param = $1; } params { current_param = param_none; }
| "%pure-parser"                { handle_خالص_parser (&@$, $1); }
| "%require" STRING             { handle_require (&@2, $2); }
| "%skeleton" STRING            { handle_skeleton (&@2, $2); }
| "%token-table"                { token_جدول_flag = true; }
| "%verbose"                    { report_flag |= report_حالتونه; }
| "%yacc"                       { handle_yacc (&@$); }
| error ";"                     { current_class = unknown_sym; yyerrok; }
| /*FIXME: Err?  What is this horror doing here? */ ";"
;

params:
   params "{...}"  { جمع_کړه_param (current_param, $2, @2); }
| "{...}"          { جمع_کړه_param (current_param, $1, @1); }
;


/*----------------------.
| grammar_declaration.  |
`----------------------*/

grammar_declaration:
  نښه_declaration
| "%start" symbols.1
    {
      grammar_پیل_symbols_جمع_کړه ($2);
    }
| کوډ_props_type "{...}" generic_symlist
    {
      کوډ_props کوډ;
      کوډ_props_نښه_action_init (&کوډ, $2, @2);
      کوډ_props_translate_کوډ (&کوډ);
      {
        لكل (نښه_list *list = $3; list; list = list->بل)
          نښه_list_کوډ_props_سېټ (list, $1, &کوډ);
        نښه_list_free ($3);
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
      muscle_کوډ_grow ("percent_code()",
                        translate_کوډ_braceless ($2, @2), @2);
      کوډ_scanner_وروستی_string_free ();
    }
| "%code" ID "{...}"
    {
      muscle_percent_کوډ_grow ($2, @2, translate_کوډ_braceless ($3, @3), @3);
      کوډ_scanner_وروستی_string_free ();
    }
;

%نوع <کوډ_props_type> کوډ_props_type;
%printer { fprintf (yyo, "%s", کوډ_props_type_string ($$)); } <کوډ_props_type>;
کوډ_props_type:
  "%destructor"  { $$ = destructor; }
| "%printer"     { $$ = printer; }
;

/*---------.
| %union.  |
`---------*/

%token PERCENT_UNION "%union";

union_name:
  %empty {}
| ID     { muscle_percent_define_ورننباسه ("api.value.union.name",
                                         @1, muscle_keyword, $1,
                                         MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE); }
;

grammar_declaration:
  "%union" union_name "{...}"
    {
      union_کتل_شوی = true;
      muscle_کوډ_grow ("union_members", translate_کوډ_braceless ($3, @3), @3);
      کوډ_scanner_وروستی_string_free ();
    }
;


%نوع <نښه_list*> nterm_decls نښه_decls symbols.1
      token_decls token_decls_for_prec
      token_decl.1 token_decl_for_prec.1;
نښه_declaration:
  "%nterm" { current_class = nterm_sym; } nterm_decls[syms]
    {
      current_class = unknown_sym;
      نښه_list_free ($syms);
    }
| "%token" { current_class = token_sym; } token_decls[syms]
    {
      current_class = unknown_sym;
      نښه_list_free ($syms);
    }
| "%type" { current_class = pct_type_sym; } نښه_decls[syms]
    {
      current_class = unknown_sym;
      نښه_list_free ($syms);
    }
| precedence_declarator token_decls_for_prec[syms]
    {
      ++current_prec;
      لكل (نښه_list *list = $syms; list; list = list->بل)
        نښه_precedence_سېټ (list->content.sym, current_prec, $1, @1);
      نښه_list_free ($syms);
    }
;

precedence_declarator:
  "%left"       { $$ = کیڼ_assoc; }
| "%right"      { $$ = ښی_assoc; }
| "%nonassoc"   { $$ = non_assoc; }
| "%precedence" { $$ = precedence_assoc; }
;

%نوع <حرف*> string.opt;
string.opt:
  %empty  { $$ = عدم; }
| STRING  { $$ = $1; }
;

نښان.opt:
  %empty { $$ = عدم; }
| نښان_2    { $$ = $1; }
;

%نوع <نښه_list*> generic_symlist generic_symlist_توکی;
generic_symlist:
  generic_symlist_توکی
| generic_symlist generic_symlist_توکی   { $$ = نښه_list_append ($1, $2); }
;

generic_symlist_توکی:
  نښه    { $$ = نښه_list_sym_new ($1, @1); }
| نښان       { $$ = نښه_list_type_new ($1, @1); }
;

نښان:
  نښان_2
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
| نښان_2 token_decl.1[syms]
    {
      $$ = نښه_list_type_سېټ ($syms, $نښان_2);
    }
| token_decls نښان_2 token_decl.1[syms]
    {
      $$ = نښه_list_append ($1, نښه_list_type_سېټ ($syms, $نښان_2));
    }
;

// One or more symbol declarations for %token or %nterm.
token_decl.1:
  token_decl                { $$ = نښه_list_sym_new ($1, @1); }
| token_decl.1 token_decl   { $$ = نښه_list_append ($1, نښه_list_sym_new ($2, @2)); }

// One symbol declaration for %token or %nterm.
token_decl:
  id صحيح.opt[num] alias
    {
      $$ = $id;
      نښه_class_سېټ ($id, current_class, @id, true);
      إذا (0 <= $عدد)
        نښه_کوډ_سېټ ($id, $عدد, @عدد);
      إذا ($alias)
        نښه_make_alias ($id, $alias, @alias);
    }
;

%نوع <صحيح> صحيح.opt;
صحيح.opt:
  %empty  { $$ = -1; }
| INT_LITERAL
;

%نوع <نښه*> alias;
alias:
  %empty         { $$ = عدم; }
| string_as_id   { $$ = $1; }
| TSTRING
    {
      $$ = نښه_get ($1, @1);
      نښه_class_سېټ ($$, token_sym, @1, false);
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
| نښان_2 token_decl_for_prec.1[syms]
    {
      $$ = نښه_list_type_سېټ ($syms, $نښان_2);
    }
| token_decls_for_prec نښان_2 token_decl_for_prec.1[syms]
    {
      $$ = نښه_list_append ($1, نښه_list_type_سېټ ($syms, $نښان_2));
    }
;

// One or more token declarations for precedence declaration.
token_decl_for_prec.1:
  token_decl_for_prec
    { $$ = نښه_list_sym_new ($1, @1); }
| token_decl_for_prec.1 token_decl_for_prec
    { $$ = نښه_list_append ($1, نښه_list_sym_new ($2, @2)); }

// One token declaration for precedence declaration.
token_decl_for_prec:
  id صحيح.opt[num]
    {
      $$ = $id;
      نښه_class_سېټ ($id, token_sym, @id, false);
      إذا (0 <= $عدد)
        نښه_کوډ_سېټ ($id, $عدد, @عدد);
    }
| string_as_id
;


/*-----------------------------------.
| symbol_decls (argument of %type).  |
`-----------------------------------*/

// A non empty list of typed symbols (for %type).
نښه_decls:
  symbols.1[syms]
    {
      $$ = $syms;
    }
| نښان_2 symbols.1[syms]
    {
      $$ = نښه_list_type_سېټ ($syms, $نښان_2);
    }
| نښه_decls نښان_2 symbols.1[syms]
    {
      $$ = نښه_list_append ($1, نښه_list_type_سېټ ($syms, $نښان_2));
    }
;

// One or more symbols.
symbols.1:
  نښه
    {
      إذا (current_class != unknown_sym)
        نښه_class_سېټ ($نښه, current_class, @نښه, false);
      $$ = نښه_list_sym_new ($نښه, @نښه);
    }
  | symbols.1 نښه
    {
      إذا (current_class != unknown_sym)
        نښه_class_سېټ ($نښه, current_class, @نښه, false);
      $$ = نښه_list_append ($1, نښه_list_sym_new ($نښه, @نښه));
    }
;

        /*------------------------------------------.
        | The grammar section: between the two %%.  |
        `------------------------------------------*/

grammar:
  قواعد_or_grammar_declaration
| grammar قواعد_or_grammar_declaration
;

/* As a Bison extension, one can use the grammar declarations in the
   body of the grammar.  */
قواعد_or_grammar_declaration:
  قواعد
| grammar_declaration ";"
| error ";"
    {
      yyerrok;
    }
;

قواعد:
  id_colon named_ref.opt { current_lhs ($1, @1, $2); } ":" rhses.1
    {
      /* Free the current lhs. */
      current_lhs (0, @1, 0);
    }
;

rhses.1:
  rhs                { grammar_current_قاعده_پای_ټکی (@rhs); }
| rhses.1 "|" rhs    { grammar_current_قاعده_پای_ټکی (@rhs); }
| rhses.1 ";"
;

%token PERCENT_تشه "%empty";
rhs:
  %empty
    { grammar_current_قاعده_begin (current_lhs_نښه, current_lhs_loc,
                                  current_lhs_named_ref); }
| rhs نښه named_ref.opt
    { grammar_current_قاعده_نښه_append ($2, @2, $3); }
| rhs نښان.opt "{...}"[action] named_ref.opt[name]
    { grammar_current_قاعده_action_append ($action, @action, $name, $[tag.opt]); }
| rhs "%?{...}"
    { grammar_current_قاعده_predicate_append ($2, @2); }
| rhs "%empty"
    { grammar_current_قاعده_تشه_سېټ (@2); }
| rhs "%prec" نښه
    { grammar_current_قاعده_prec_سېټ ($3, @3); }
| rhs "%dprec" INT_LITERAL
    { grammar_current_قاعده_dprec_سېټ ($3, @3); }
| rhs "%merge" نښان_2
    { grammar_current_قاعده_یوځای_سېټ ($3, @3); }
| rhs "%expect" INT_LITERAL
    { grammar_current_قاعده_expect_sr ($3, @3); }
| rhs "%expect-rr" INT_LITERAL
    { grammar_current_قاعده_expect_rr ($3, @3); }
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
    muscle_ډول ډول;
  } ارزښت_type;
};
%نوع <ارزښت_type> ارزښت;
%printer
{
  بدل ($$.ډول)
    {
    حالة muscle_کوډ:    fprintf (yyo,  "{%s}",  $$.chars); اكسر;
    حالة muscle_keyword: fprintf (yyo,   "%s",   $$.chars); اكسر;
    حالة muscle_string:  fprintf (yyo, "\"%s\"", $$.chars); اكسر;
    }
} <ارزښت_type>;

ارزښت:
  %empty  { $$.ډول = muscle_keyword; $$.chars = ""; }
| ID      { $$.ډول = muscle_keyword; $$.chars = $1; }
| STRING  { $$.ډول = muscle_string;  $$.chars = unquote ($1); gram_scanner_وروستی_string_free ();}
| "{...}" { $$.ډول = muscle_کوډ;    $$.chars = strip_braces ($1); gram_scanner_وروستی_string_free (); }
;


/*--------------.
| Identifiers.  |
`--------------*/

/* Identifiers are returned as uniqstr values by the scanner.
   Depending on their use, we may need to make them genuine symbols.  */

id:
  ID
    { $$ = نښه_from_uniqstr ($1, @1); }
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
      $$ = نښه_get (char_name ($1), @1);
      نښه_class_سېټ ($$, token_sym, @1, false);
      نښه_کوډ_سېټ ($$, $1, @1);
    }
;

id_colon:
  ID_COLON { $$ = نښه_from_uniqstr ($1, @1); }
;


نښه:
  id
| string_as_id
;

/* A string used as an ID.  */
string_as_id:
  STRING
    {
      $$ = نښه_get ($1, @1);
      نښه_class_سېټ ($$, token_sym, @1, false);
    }
;

epilogue.opt:
  %empty
| "%%" EPILOGUE
    {
      muscle_کوډ_grow ("epilogue", translate_کوډ ($2, @2, true), @2);
      کوډ_scanner_وروستی_string_free ();
    }
;

%%

صحيح
yyreport_syntax_error (ثابت yypcontext_t *ctx)
{
  صحيح res = 0;
  /* Arguments of format: reported tokens (one for the "unexpected",
     one per "expected"). */
  تعداد { دلیلونه_MAX = 5 };
  ثابت حرف *argv[ARGS_MAX];
  صحيح argc = 0;
  yysymbol_ډول_t unexpected = yypcontext_token (ctx);
  إذا (unexpected != YYSYMBOL_YYEMPTY)
    {
      argv[argc++] = yysymbol_name (unexpected);
      yysymbol_ډول_t expected[ARGS_MAX - 1];
      صحيح nexpected = yypcontext_expected_نښې (ctx, expected, دلیلونه_MAX - 1);
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
  loc.پیل = rhs[n].پای_ټکی;
  loc.پای_ټکی = rhs[n].پای_ټکی;

  /* Ignore empty nonterminals the start of the right-hand side.
     Do not bother to ignore them at the end of the right-hand side,
     since empty nonterminals have the same end as their predecessors.  */
  لكل (صحيح i = 1; i <= n; i++)
    إذا (! equal_boundaries (rhs[i].پیل, rhs[i].پای_ټکی))
      {
        loc.پیل = rhs[i].پیل;
        اكسر;
      }

  أرجع loc;
}

ساكن
حرف *strip_braces (حرف *کوډ)
{
  کوډ[مزي_اوږدوالی (کوډ) - 1] = 0;
  أرجع کوډ + 1;
}

ساكن
حرف ثابت *
translate_کوډ (حرف *کوډ, location loc, bool plain)
{
  کوډ_props plain_کوډ;
  إذا (plain)
    کوډ_props_plain_init (&plain_کوډ, کوډ, loc);
  وإلا
    کوډ_props_نښه_action_init (&plain_کوډ, کوډ, loc);
  کوډ_props_translate_کوډ (&plain_کوډ);
  gram_scanner_وروستی_string_free ();
  أرجع plain_کوډ.کوډ;
}

ساكن
حرف ثابت *
translate_کوډ_braceless (حرف *کوډ, location loc)
{
  أرجع translate_کوډ (strip_braces (کوډ), loc, true);
}

ساكن فراغ
جمع_کړه_param (param_type نوع, حرف *decl, location loc)
{
  ساكن حرف ثابت alphanum[26 + 26 + 1 + 10 + 1] =
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "_"
    "0123456789";

  حرف ثابت *name_پیل = عدم;
  {
    حرف *p;
    /* Stop on last actual character.  */
    لكل (p = decl; p[1]; p++)
      إذا ((p == decl
           || ! memchr (alphanum, p[-1], حجم alphanum - 1))
          && memchr (alphanum, p[0], حجم alphanum - 10 - 1))
        name_پیل = p;

    /* Strip the surrounding '{' and '}', and any blanks just inside
       the braces.  */
    --p;
    طالما (c_isspace ((بینښه حرف) *p))
      --p;
    p[1] = '\0';
    ++decl;
    طالما (c_isspace ((بینښه حرف) *decl))
      ++decl;
  }

  إذا (! name_پیل)
    complain (&loc, complaint, _("missing identifier in parameter declaration"));
  وإلا
    {
      حرف *name = xmemdup0 (name_پیل, strspn (name_پیل, alphanum));
      إذا (نوع & param_lex)
        muscle_pair_list_grow ("lex_param", decl, name);
      إذا (نوع & param_parse)
        muscle_pair_list_grow ("parse_param", decl, name);
      حرر (name);
    }

  gram_scanner_وروستی_string_free ();
}


ساكن فراغ
handle_سرلیک (حرف ثابت *ارزښت)
{
  سرلیک_flag = true;
  إذا (ارزښت)
    {
      حرف *file = unquote (ارزښت);
      spec_سرلیک_file = xstrdup (file);
      gram_scanner_وروستی_string_free ();
      unquote_free (file);
    }
}


ساكن فراغ
handle_error_verbose (location ثابت *loc, حرف ثابت *directive)
{
  bison_directive (loc, directive);
  muscle_percent_define_ورننباسه (directive, *loc, muscle_keyword, "",
                                MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
}


ساكن فراغ
handle_file_مختاړی (location ثابت *loc,
                    location ثابت *dir_loc,
                    حرف ثابت *directive, حرف ثابت *ارزښت_quoted)
{
  حرف *ارزښت = unquote (ارزښت_quoted);
  bison_directive (loc, directive);
  bool warned = false;

  إذا (location_تشه (spec_file_مختاړی_loc))
    {
      spec_file_مختاړی_loc = *loc;
      spec_file_مختاړی = ارزښت;
    }
  وإلا
    {
      duplicate_directive (directive, spec_file_مختاړی_loc, *loc);
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
handle_name_مختاړی (location ثابت *loc,
                    حرف ثابت *directive, حرف ثابت *ارزښت_quoted)
{
  حرف *ارزښت = unquote (ارزښت_quoted);
  bison_directive (loc, directive);

  حرف buf1[1024];
  size_t len1 = حجم (buf1);
  حرف *old = asnprintf (buf1, &len1, "%s\"%s\"", directive, ارزښت);
  إذا (!old)
    xalloc_die ();

  إذا (location_تشه (spec_name_مختاړی_loc))
    {
      spec_name_مختاړی = ارزښت;
      spec_name_مختاړی_loc = *loc;

      حرف buf2[1024];
      size_t len2 = حجم (buf2);
      حرف *new = asnprintf (buf2, &len2, "%%define api.prefix {%s}", ارزښت);
      إذا (!new)
        xalloc_die ();
      deprecated_directive (loc, old, new);
      إذا (new != buf2)
        حرر (new);
    }
  وإلا
    duplicate_directive (old, spec_file_مختاړی_loc, *loc);

  إذا (old != buf1)
    حرر (old);
}


ساكن فراغ
handle_خالص_parser (location ثابت *loc, حرف ثابت *directive)
{
  bison_directive (loc, directive);
  deprecated_directive (loc, directive, "%define api.pure");
  muscle_percent_define_ورننباسه ("api.pure", *loc, muscle_keyword, "",
                                MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
}


ساكن فراغ
handle_require (location ثابت *loc, حرف ثابت *نسخه_quoted)
{
  حرف *نسخه = unquote (نسخه_quoted);
  required_نسخه = strversion_منزل_int (نسخه);
  إذا (required_نسخه == -1)
    {
      complain (loc, complaint, _("invalid version requirement: %s"),
                نسخه);
      required_نسخه = 0;
    }
  وإلا
    {
      ثابت حرف* package_نسخه =
        0 < strverscmp (api_نسخه, PACKAGE_نسخه)
        ? api_نسخه : PACKAGE_نسخه;
      إذا (0 < strverscmp (نسخه, package_نسخه))
        {
          complain (loc, complaint, _("require bison %s, but have %s"),
                    نسخه, package_نسخه);
          وتل (EX_MISMATCH);
        }
    }
  unquote_free (نسخه);
  gram_scanner_وروستی_string_free ();
}

ساكن فراغ
handle_skeleton (location ثابت *loc, حرف ثابت *skel_quoted)
{
  حرف *skel = unquote (skel_quoted);
  حرف ثابت *skeleton_user = skel;
  إذا (strchr (skeleton_user, '/'))
    {
      size_t dir_اوږدتیا = مزي_اوږدوالی (grammar_file);
      طالما (dir_اوږدتیا && grammar_file[dir_length - 1] != '/')
        --dir_length;
      طالما (dir_اوږدتیا && grammar_file[dir_length - 1] == '/')
        --dir_length;
      حرف *skeleton_build =
        xmalloc (dir_اوږدتیا + 1 + مزي_اوږدوالی (skeleton_user) + 1);
      إذا (dir_اوږدتیا > 0)
        {
          حافظه_نقل (skeleton_build, grammar_file, dir_اوږدتیا);
          skeleton_build[dir_length++] = '/';
        }
      strcpy (skeleton_build + dir_اوږدتیا, skeleton_user);
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
  إذا (location_تشه (yacc_loc))
    سېټ_yacc (*loc);
  وإلا
    duplicate_directive (directive, yacc_loc, *loc);
}


ساكن فراغ
gram_error (location ثابت *loc, حرف ثابت *پیغام_2)
{
  complain (loc, complaint, "%s", پیغام_2);
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
current_lhs (نښه *sym, location loc, named_ref *ref)
{
  current_lhs_نښه = sym;
  current_lhs_loc = loc;
  إذا (sym)
    نښه_location_as_lhs_سېټ (sym, loc);
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
  پای_ټکی_use_class ("value", yyo);
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
unquote_free (حرف *وروستی_string)
{
  obstack_free (&obstack_for_unquote, وروستی_string);
}

ساكن حرف *
unquote (ثابت حرف *cp)
{
#تعریف_کړه GROW(Char)                              \
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
#تعریف_لغوه GROW
  أرجع obstack_finish0 (&obstack_for_unquote);
}
