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
  ساكن ثابت حرف* api_نسخہ = "3.8";

  ساكن صحيح current_prec = 0;
  ساكن location current_lhs_loc;
  ساكن named_ref *current_lhs_named_ref;
  ساكن علامت *current_lhs_علامت;
  ساكن علامت_class current_class = unknown_sym;

  /** Set the new current left-hand side symbol, possibly common
   * to several right-hand side parts of rule.
   */
  ساكن فراغ current_lhs (علامت *sym, location loc, named_ref *ref);

  #تعریف_کرو YYLLOC_DEFAULT(Current, Rhs, N)         \
    (Current) = lloc_default (Rhs, N)
  ساكن YYLTYPE lloc_default (YYLTYPE ثابت *, صحيح);

  #تعریف_کرو YY_LOCATION_PRINT(File, Loc)            \
    location_print (Loc, File)

  /* Strip initial '{' and final '}' (must be first and last characters).
     Return the result.  */
  ساكن حرف *strip_braces (حرف *کوڈ);

  /* Convert CODE by calling code_props_plain_init if PLAIN, otherwise
     code_props_symbol_action_init.  Calls
     gram_scanner_last_string_free to release the latest string from
     the scanner (should be CODE). */
  ساكن حرف ثابت *translate_کوڈ (حرف *کوڈ, location loc, bool plain);

  /* Convert CODE by calling code_props_plain_init after having
     stripped the first and last characters (expected to be '{', and
     '}').  Calls gram_scanner_last_string_free to release the latest
     string from the scanner (should be CODE). */
  ساكن حرف ثابت *translate_کوڈ_braceless (حرف *کوڈ, location loc);

  /* Handle a %header directive.  */
  ساكن فراغ handle_سرخی (حرف ثابت *قدر);

  /* Handle a %error-verbose directive.  */
  ساكن فراغ handle_error_verbose (location ثابت *loc, حرف ثابت *directive);

  /* Handle a %file-prefix directive.  */
  ساكن فراغ handle_file_اگیتر (location ثابت *loc,
                                  location ثابت *dir_loc,
                                  حرف ثابت *directive, حرف ثابت *قدر);

  /* Handle a %language directive.  */
  ساكن فراغ handle_language (location ثابت *loc, حرف ثابت *lang);

  /* Handle a %name-prefix directive.  */
  ساكن فراغ handle_name_اگیتر (location ثابت *loc,
                                  حرف ثابت *directive, حرف ثابت *قدر);

  /* Handle a %pure-parser directive.  */
  ساكن فراغ handle_خالص_parser (location ثابت *loc, حرف ثابت *directive);

  /* Handle a %require directive.  */
  ساكن فراغ handle_require (location ثابت *loc, حرف ثابت *نسخہ);

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
  ساكن فراغ unquote_free (حرف *آخری_string);
}

%define api.سرخی.include {"parse-gram.h"}
%define api.اگیتر {gram_}
%define api.خالص بھریا
%define api.token.raw
%define api.قدر.نوع اتحاد
%define locations
%define parse.error custom
%define parse.lac بھریا
%define parse.trace
%header
%expect 0
%verbose

%initial-action
{
  /* Bison's grammar can initial empty locations, hence a default
     location is needed. */
  boundary_سیٹ (&@$.شروع, grammar_file, 1, 1, 1);
  boundary_سیٹ (&@$.سرا, grammar_file, 1, 1, 1);
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
  PERCENT_ملاؤ       "%merge"

  PERCENT_کوڈ            "%code"
  PERCENT_DEFAULT_PREC    "%default-prec"
  PERCENT_DEFINE          "%define"
  PERCENT_ERROR_VERBOSE   "%error-verbose"
  PERCENT_EXPECT          "%expect"
  PERCENT_EXPECT_RR       "%expect-rr"
  PERCENT_FILE_اگیتر     "%file-prefix"
  PERCENT_FLAG            "%<flag>"
  PERCENT_GLR_PARSER      "%glr-parser"
  PERCENT_سرخی          "%header"
  PERCENT_INITIAL_ACTION  "%initial-action"
  PERCENT_LANGUAGE        "%language"
  PERCENT_NAME_اگیتر     "%name-prefix"
  PERCENT_NO_DEFAULT_PREC "%no-default-prec"
  PERCENT_NO_سطریں        "%no-lines"
  PERCENT_NONDETERMINISTIC_PARSER "%nondeterministic-parser"
  PERCENT_OUTPUT          "%output"
  PERCENT_خالص_PARSER     "%pure-parser"
  PERCENT_REQUIRE         "%require"
  PERCENT_SKELETON        "%skeleton"
  PERCENT_شروع           "%start"
  PERCENT_TOKEN_جدول     "%token-table"
  PERCENT_VERBOSE         "%verbose"
  PERCENT_YACC            "%yacc"

  BRACED_کوڈ       "{...}"
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
  ٹیگ_2               _("<tag>")
  ٹیگ_ANY           "<*>"
  ٹیگ_NONE          "<>"

 /* Experimental feature, don't rely on it.  */
%code pre-printer  {tron (yyo);}
%code post-printer {troff (yyo);}

%نوع <بنانشانی حرف> CHAR_LITERAL
%printer { fputs (char_name ($$), yyo); } <بنانشانی حرف>

%نوع <حرف*> "{...}" "%?{...}" "%{...%}" EPILOGUE STRING TSTRING
%printer { fputs ($$, yyo); } <حرف*>

%نوع <uniqstr>
  BRACKETED_ID ID ID_COLON
  PERCENT_ERROR_VERBOSE PERCENT_FILE_اگیتر PERCENT_FLAG PERCENT_NAME_اگیتر
  PERCENT_خالص_PARSER
  ٹیگ_2 ٹیگ ٹیگ.opt variable
%printer { fputs ($$, yyo); } <uniqstr>
%printer { fprintf (yyo, "[%s]", $$); } BRACKETED_ID
%printer { fprintf (yyo, "%s:", $$); } ID_COLON
%printer { fprintf (yyo, "%%%s", $$); } PERCENT_FLAG
%printer { fprintf (yyo, "<%s>", $$); } ٹیگ_2 ٹیگ

%token <صحيح> INT_LITERAL _("integer literal")
%printer { fprintf (yyo, "%d", $$); } <صحيح>

%نوع <علامت*> id id_colon string_as_id علامت token_decl token_decl_for_prec
%printer { fprintf (yyo, "%s", $$ ? $$->ٹیگ : "<NULL>"); } <علامت*>
%printer { fprintf (yyo, "%s:", $$->ٹیگ); } id_colon

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
  ساكن فراغ جوڑو_param (param_type نوع, حرف *decl, location loc);
  ساكن param_type current_param = param_none;
};
%token <param_type> PERCENT_PARAM "%param";
%printer
{
  بدل ($$)
    {
#تعریف_کرو CASE(وچ_2, باہرلا_2)                                           \
      حالة param_ ## وچ_2: fputs ("%" #Out, yyo); اكسر
      CASE (lex,   lex-param);
      CASE (parse, parse-param);
      CASE (both,  param);
#تعریف_رد CASE
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
      muscle_کوڈ_grow (union_تکیا ? "post_prologue" : "pre_prologue",
                        translate_کوڈ ($1, @1, true), @1);
      کوڈ_scanner_آخری_string_free ();
    }
| "%<flag>"
    {
      muscle_percent_define_ensure ($1, @1, true);
    }
| "%define" variable قدر
    {
      muscle_percent_define_داخل_کرو ($2, @$, $3.قسم, $3.chars,
                                    MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
    }
| "%header" string.opt             { handle_سرخی ($2); }
| "%error-verbose"                 { handle_error_verbose (&@$, $1); }
| "%expect" INT_LITERAL            { expected_sr_conflicts = $2; }
| "%expect-rr" INT_LITERAL         { expected_rr_conflicts = $2; }
| "%file-prefix" STRING            { handle_file_اگیتر (&@$, &@1, $1, $2); }
| "%glr-parser"
    {
      nondeterministic_parser = true;
      glr_parser = true;
    }
| "%initial-action" "{...}"
    {
      muscle_کوڈ_grow ("initial_action", translate_کوڈ ($2, @2, false), @2);
      کوڈ_scanner_آخری_string_free ();
    }
| "%language" STRING            { handle_language (&@1, $2); }
| "%name-prefix" STRING         { handle_name_اگیتر (&@$, $1, $2); }
| "%no-lines"                   { no_سطریں_flag = true; }
| "%nondeterministic-parser"    { nondeterministic_parser = true; }
| "%output" STRING              { spec_outfile = unquote ($2); gram_scanner_آخری_string_free (); }
| "%param" { current_param = $1; } params { current_param = param_none; }
| "%pure-parser"                { handle_خالص_parser (&@$, $1); }
| "%require" STRING             { handle_require (&@2, $2); }
| "%skeleton" STRING            { handle_skeleton (&@2, $2); }
| "%token-table"                { token_جدول_flag = true; }
| "%verbose"                    { report_flag |= report_حالتاں; }
| "%yacc"                       { handle_yacc (&@$); }
| error ";"                     { current_class = unknown_sym; yyerrok; }
| /*FIXME: Err?  What is this horror doing here? */ ";"
;

params:
   params "{...}"  { جوڑو_param (current_param, $2, @2); }
| "{...}"          { جوڑو_param (current_param, $1, @1); }
;


/*----------------------.
| grammar_declaration.  |
`----------------------*/

grammar_declaration:
  علامت_declaration
| "%start" symbols.1
    {
      grammar_شروع_symbols_جوڑو ($2);
    }
| کوڈ_props_type "{...}" generic_symlist
    {
      کوڈ_props کوڈ;
      کوڈ_props_علامت_action_init (&کوڈ, $2, @2);
      کوڈ_props_translate_کوڈ (&کوڈ);
      {
        لكل (علامت_list *list = $3; list; list = list->اگلا)
          علامت_list_کوڈ_props_سیٹ (list, $1, &کوڈ);
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
      muscle_کوڈ_grow ("percent_code()",
                        translate_کوڈ_braceless ($2, @2), @2);
      کوڈ_scanner_آخری_string_free ();
    }
| "%code" ID "{...}"
    {
      muscle_percent_کوڈ_grow ($2, @2, translate_کوڈ_braceless ($3, @3), @3);
      کوڈ_scanner_آخری_string_free ();
    }
;

%نوع <کوڈ_props_type> کوڈ_props_type;
%printer { fprintf (yyo, "%s", کوڈ_props_type_string ($$)); } <کوڈ_props_type>;
کوڈ_props_type:
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
      union_تکیا = true;
      muscle_کوڈ_grow ("union_members", translate_کوڈ_braceless ($3, @3), @3);
      کوڈ_scanner_آخری_string_free ();
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
      لكل (علامت_list *list = $syms; list; list = list->اگلا)
        علامت_precedence_سیٹ (list->content.sym, current_prec, $1, @1);
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

ٹیگ.opt:
  %empty { $$ = عدم; }
| ٹیگ_2    { $$ = $1; }
;

%نوع <علامت_list*> generic_symlist generic_symlist_شے;
generic_symlist:
  generic_symlist_شے
| generic_symlist generic_symlist_شے   { $$ = علامت_list_append ($1, $2); }
;

generic_symlist_شے:
  علامت    { $$ = علامت_list_sym_new ($1, @1); }
| ٹیگ       { $$ = علامت_list_type_new ($1, @1); }
;

ٹیگ:
  ٹیگ_2
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
| ٹیگ_2 token_decl.1[syms]
    {
      $$ = علامت_list_type_سیٹ ($syms, $ٹیگ_2);
    }
| token_decls ٹیگ_2 token_decl.1[syms]
    {
      $$ = علامت_list_append ($1, علامت_list_type_سیٹ ($syms, $ٹیگ_2));
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
      علامت_class_سیٹ ($id, current_class, @id, true);
      إذا (0 <= $عدد)
        علامت_کوڈ_سیٹ ($id, $عدد, @عدد);
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
      علامت_class_سیٹ ($$, token_sym, @1, false);
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
| ٹیگ_2 token_decl_for_prec.1[syms]
    {
      $$ = علامت_list_type_سیٹ ($syms, $ٹیگ_2);
    }
| token_decls_for_prec ٹیگ_2 token_decl_for_prec.1[syms]
    {
      $$ = علامت_list_append ($1, علامت_list_type_سیٹ ($syms, $ٹیگ_2));
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
      علامت_class_سیٹ ($id, token_sym, @id, false);
      إذا (0 <= $عدد)
        علامت_کوڈ_سیٹ ($id, $عدد, @عدد);
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
| ٹیگ_2 symbols.1[syms]
    {
      $$ = علامت_list_type_سیٹ ($syms, $ٹیگ_2);
    }
| علامت_decls ٹیگ_2 symbols.1[syms]
    {
      $$ = علامت_list_append ($1, علامت_list_type_سیٹ ($syms, $ٹیگ_2));
    }
;

// One or more symbols.
symbols.1:
  علامت
    {
      إذا (current_class != unknown_sym)
        علامت_class_سیٹ ($علامت, current_class, @علامت, false);
      $$ = علامت_list_sym_new ($علامت, @علامت);
    }
  | symbols.1 علامت
    {
      إذا (current_class != unknown_sym)
        علامت_class_سیٹ ($علامت, current_class, @علامت, false);
      $$ = علامت_list_append ($1, علامت_list_sym_new ($علامت, @علامت));
    }
;

        /*------------------------------------------.
        | The grammar section: between the two %%.  |
        `------------------------------------------*/

grammar:
  قاعدے_or_grammar_declaration
| grammar قاعدے_or_grammar_declaration
;

/* As a Bison extension, one can use the grammar declarations in the
   body of the grammar.  */
قاعدے_or_grammar_declaration:
  قاعدے
| grammar_declaration ";"
| error ";"
    {
      yyerrok;
    }
;

قاعدے:
  id_colon named_ref.opt { current_lhs ($1, @1, $2); } ":" rhses.1
    {
      /* Free the current lhs. */
      current_lhs (0, @1, 0);
    }
;

rhses.1:
  rhs                { grammar_current_قاعدہ_سرا (@rhs); }
| rhses.1 "|" rhs    { grammar_current_قاعدہ_سرا (@rhs); }
| rhses.1 ";"
;

%token PERCENT_خالی_تھاواں "%empty";
rhs:
  %empty
    { grammar_current_قاعدہ_begin (current_lhs_علامت, current_lhs_loc,
                                  current_lhs_named_ref); }
| rhs علامت named_ref.opt
    { grammar_current_قاعدہ_علامت_append ($2, @2, $3); }
| rhs ٹیگ.opt "{...}"[action] named_ref.opt[name]
    { grammar_current_قاعدہ_action_append ($action, @action, $name, $[tag.opt]); }
| rhs "%?{...}"
    { grammar_current_قاعدہ_predicate_append ($2, @2); }
| rhs "%empty"
    { grammar_current_قاعدہ_خالی_تھاواں_سیٹ (@2); }
| rhs "%prec" علامت
    { grammar_current_قاعدہ_prec_سیٹ ($3, @3); }
| rhs "%dprec" INT_LITERAL
    { grammar_current_قاعدہ_dprec_سیٹ ($3, @3); }
| rhs "%merge" ٹیگ_2
    { grammar_current_قاعدہ_ملاؤ_سیٹ ($3, @3); }
| rhs "%expect" INT_LITERAL
    { grammar_current_قاعدہ_expect_sr ($3, @3); }
| rhs "%expect-rr" INT_LITERAL
    { grammar_current_قاعدہ_expect_rr ($3, @3); }
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
    muscle_قسم قسم;
  } قدر_type;
};
%نوع <قدر_type> قدر;
%printer
{
  بدل ($$.قسم)
    {
    حالة muscle_کوڈ:    fprintf (yyo,  "{%s}",  $$.chars); اكسر;
    حالة muscle_keyword: fprintf (yyo,   "%s",   $$.chars); اكسر;
    حالة muscle_string:  fprintf (yyo, "\"%s\"", $$.chars); اكسر;
    }
} <قدر_type>;

قدر:
  %empty  { $$.قسم = muscle_keyword; $$.chars = ""; }
| ID      { $$.قسم = muscle_keyword; $$.chars = $1; }
| STRING  { $$.قسم = muscle_string;  $$.chars = unquote ($1); gram_scanner_آخری_string_free ();}
| "{...}" { $$.قسم = muscle_کوڈ;    $$.chars = strip_braces ($1); gram_scanner_آخری_string_free (); }
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
      علامت_class_سیٹ ($$, token_sym, @1, false);
      علامت_کوڈ_سیٹ ($$, $1, @1);
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
      علامت_class_سیٹ ($$, token_sym, @1, false);
    }
;

epilogue.opt:
  %empty
| "%%" EPILOGUE
    {
      muscle_کوڈ_grow ("epilogue", translate_کوڈ ($2, @2, true), @2);
      کوڈ_scanner_آخری_string_free ();
    }
;

%%

صحيح
yyreport_syntax_error (ثابت yypcontext_t *ctx)
{
  صحيح res = 0;
  /* Arguments of format: reported tokens (one for the "unexpected",
     one per "expected"). */
  تعداد { دلیلاں_MAX = 5 };
  ثابت حرف *argv[ARGS_MAX];
  صحيح argc = 0;
  yysymbol_قسم_t unexpected = yypcontext_token (ctx);
  إذا (unexpected != YYSYMBOL_YYEMPTY)
    {
      argv[argc++] = yysymbol_name (unexpected);
      yysymbol_قسم_t expected[ARGS_MAX - 1];
      صحيح nexpected = yypcontext_expected_ٹوکن (ctx, expected, دلیلاں_MAX - 1);
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
  loc.شروع = rhs[n].سرا;
  loc.سرا = rhs[n].سرا;

  /* Ignore empty nonterminals the start of the right-hand side.
     Do not bother to ignore them at the end of the right-hand side,
     since empty nonterminals have the same end as their predecessors.  */
  لكل (صحيح i = 1; i <= n; i++)
    إذا (! equal_boundaries (rhs[i].شروع, rhs[i].سرا))
      {
        loc.شروع = rhs[i].شروع;
        اكسر;
      }

  أرجع loc;
}

ساكن
حرف *strip_braces (حرف *کوڈ)
{
  کوڈ[لڑی_لمبائی (کوڈ) - 1] = 0;
  أرجع کوڈ + 1;
}

ساكن
حرف ثابت *
translate_کوڈ (حرف *کوڈ, location loc, bool plain)
{
  کوڈ_props plain_کوڈ;
  إذا (plain)
    کوڈ_props_plain_init (&plain_کوڈ, کوڈ, loc);
  وإلا
    کوڈ_props_علامت_action_init (&plain_کوڈ, کوڈ, loc);
  کوڈ_props_translate_کوڈ (&plain_کوڈ);
  gram_scanner_آخری_string_free ();
  أرجع plain_کوڈ.کوڈ;
}

ساكن
حرف ثابت *
translate_کوڈ_braceless (حرف *کوڈ, location loc)
{
  أرجع translate_کوڈ (strip_braces (کوڈ), loc, true);
}

ساكن فراغ
جوڑو_param (param_type نوع, حرف *decl, location loc)
{
  ساكن حرف ثابت alphanum[26 + 26 + 1 + 10 + 1] =
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "_"
    "0123456789";

  حرف ثابت *name_شروع = عدم;
  {
    حرف *p;
    /* Stop on last actual character.  */
    لكل (p = decl; p[1]; p++)
      إذا ((p == decl
           || ! memchr (alphanum, p[-1], حجم alphanum - 1))
          && memchr (alphanum, p[0], حجم alphanum - 10 - 1))
        name_شروع = p;

    /* Strip the surrounding '{' and '}', and any blanks just inside
       the braces.  */
    --p;
    طالما (c_isspace ((بنانشانی حرف) *p))
      --p;
    p[1] = '\0';
    ++decl;
    طالما (c_isspace ((بنانشانی حرف) *decl))
      ++decl;
  }

  إذا (! name_شروع)
    complain (&loc, complaint, _("missing identifier in parameter declaration"));
  وإلا
    {
      حرف *name = xmemdup0 (name_شروع, strspn (name_شروع, alphanum));
      إذا (نوع & param_lex)
        muscle_pair_list_grow ("lex_param", decl, name);
      إذا (نوع & param_parse)
        muscle_pair_list_grow ("parse_param", decl, name);
      حرر (name);
    }

  gram_scanner_آخری_string_free ();
}


ساكن فراغ
handle_سرخی (حرف ثابت *قدر)
{
  سرخی_flag = true;
  إذا (قدر)
    {
      حرف *file = unquote (قدر);
      spec_سرخی_file = xstrdup (file);
      gram_scanner_آخری_string_free ();
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
handle_file_اگیتر (location ثابت *loc,
                    location ثابت *dir_loc,
                    حرف ثابت *directive, حرف ثابت *قدر_quoted)
{
  حرف *قدر = unquote (قدر_quoted);
  bison_directive (loc, directive);
  bool warned = false;

  إذا (location_خالی_تھاواں (spec_file_اگیتر_loc))
    {
      spec_file_اگیتر_loc = *loc;
      spec_file_اگیتر = قدر;
    }
  وإلا
    {
      duplicate_directive (directive, spec_file_اگیتر_loc, *loc);
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
handle_name_اگیتر (location ثابت *loc,
                    حرف ثابت *directive, حرف ثابت *قدر_quoted)
{
  حرف *قدر = unquote (قدر_quoted);
  bison_directive (loc, directive);

  حرف buf1[1024];
  size_t len1 = حجم (buf1);
  حرف *old = asnprintf (buf1, &len1, "%s\"%s\"", directive, قدر);
  إذا (!old)
    xalloc_die ();

  إذا (location_خالی_تھاواں (spec_name_اگیتر_loc))
    {
      spec_name_اگیتر = قدر;
      spec_name_اگیتر_loc = *loc;

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
    duplicate_directive (old, spec_file_اگیتر_loc, *loc);

  إذا (old != buf1)
    حرر (old);
}


ساكن فراغ
handle_خالص_parser (location ثابت *loc, حرف ثابت *directive)
{
  bison_directive (loc, directive);
  deprecated_directive (loc, directive, "%define api.pure");
  muscle_percent_define_داخل_کرو ("api.pure", *loc, muscle_keyword, "",
                                MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
}


ساكن فراغ
handle_require (location ثابت *loc, حرف ثابت *نسخہ_quoted)
{
  حرف *نسخہ = unquote (نسخہ_quoted);
  required_نسخہ = strversion_منزل_int (نسخہ);
  إذا (required_نسخہ == -1)
    {
      complain (loc, complaint, _("invalid version requirement: %s"),
                نسخہ);
      required_نسخہ = 0;
    }
  وإلا
    {
      ثابت حرف* package_نسخہ =
        0 < strverscmp (api_نسخہ, PACKAGE_نسخہ)
        ? api_نسخہ : PACKAGE_نسخہ;
      إذا (0 < strverscmp (نسخہ, package_نسخہ))
        {
          complain (loc, complaint, _("require bison %s, but have %s"),
                    نسخہ, package_نسخہ);
          باہر_نکلو (EX_MISMATCH);
        }
    }
  unquote_free (نسخہ);
  gram_scanner_آخری_string_free ();
}

ساكن فراغ
handle_skeleton (location ثابت *loc, حرف ثابت *skel_quoted)
{
  حرف *skel = unquote (skel_quoted);
  حرف ثابت *skeleton_user = skel;
  إذا (strchr (skeleton_user, '/'))
    {
      size_t dir_لمائی = لڑی_لمبائی (grammar_file);
      طالما (dir_لمائی && grammar_file[dir_length - 1] != '/')
        --dir_length;
      طالما (dir_لمائی && grammar_file[dir_length - 1] == '/')
        --dir_length;
      حرف *skeleton_build =
        xmalloc (dir_لمائی + 1 + لڑی_لمبائی (skeleton_user) + 1);
      إذا (dir_لمائی > 0)
        {
          حافظہ_نقل (skeleton_build, grammar_file, dir_لمائی);
          skeleton_build[dir_length++] = '/';
        }
      strcpy (skeleton_build + dir_لمائی, skeleton_user);
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
  إذا (location_خالی_تھاواں (yacc_loc))
    سیٹ_yacc (*loc);
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
    علامت_location_as_lhs_سیٹ (sym, loc);
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
  سرا_use_class ("value", yyo);
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
unquote_free (حرف *آخری_string)
{
  obstack_free (&obstack_for_unquote, آخری_string);
}

ساكن حرف *
unquote (ثابت حرف *cp)
{
#تعریف_کرو GROW(Char)                              \
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
#تعریف_رد GROW
  أرجع obstack_finish0 (&obstack_for_unquote);
}
