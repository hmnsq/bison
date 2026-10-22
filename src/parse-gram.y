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

%code چوٽي_2
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
  ساكن ثابت حرف* api_نسخو = "3.8";

  ساكن صحيح current_prec = 0;
  ساكن location current_lhs_loc;
  ساكن named_ref *current_lhs_named_ref;
  ساكن نشان *current_lhs_نشان;
  ساكن نشان_class current_class = unknown_sym;

  /** Set the new current left-hand side symbol, possibly common
   * to several right-hand side parts of rule.
   */
  ساكن فراغ current_lhs (نشان *sym, location loc, named_ref *ref);

  #وصف_ڪريو YYLLOC_DEFAULT(Current, Rhs, N)         \
    (Current) = lloc_default (Rhs, N)
  ساكن YYLTYPE lloc_default (YYLTYPE ثابت *, صحيح);

  #وصف_ڪريو YY_LOCATION_PRINT(File, Loc)            \
    location_print (Loc, File)

  /* Strip initial '{' and final '}' (must be first and last characters).
     Return the result.  */
  ساكن حرف *strip_braces (حرف *ڪوڊ);

  /* Convert CODE by calling code_props_plain_init if PLAIN, otherwise
     code_props_symbol_action_init.  Calls
     gram_scanner_last_string_free to release the latest string from
     the scanner (should be CODE). */
  ساكن حرف ثابت *translate_ڪوڊ (حرف *ڪوڊ, location loc, bool plain);

  /* Convert CODE by calling code_props_plain_init after having
     stripped the first and last characters (expected to be '{', and
     '}').  Calls gram_scanner_last_string_free to release the latest
     string from the scanner (should be CODE). */
  ساكن حرف ثابت *translate_ڪوڊ_braceless (حرف *ڪوڊ, location loc);

  /* Handle a %header directive.  */
  ساكن فراغ handle_سرو (حرف ثابت *قدر);

  /* Handle a %error-verbose directive.  */
  ساكن فراغ handle_error_verbose (location ثابت *loc, حرف ثابت *directive);

  /* Handle a %file-prefix directive.  */
  ساكن فراغ handle_file_اڳياڙي (location ثابت *loc,
                                  location ثابت *dir_loc,
                                  حرف ثابت *directive, حرف ثابت *قدر);

  /* Handle a %language directive.  */
  ساكن فراغ handle_language (location ثابت *loc, حرف ثابت *lang);

  /* Handle a %name-prefix directive.  */
  ساكن فراغ handle_name_اڳياڙي (location ثابت *loc,
                                  حرف ثابت *directive, حرف ثابت *قدر);

  /* Handle a %pure-parser directive.  */
  ساكن فراغ handle_خالص_parser (location ثابت *loc, حرف ثابت *directive);

  /* Handle a %require directive.  */
  ساكن فراغ handle_require (location ثابت *loc, حرف ثابت *نسخو);

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
  ساكن فراغ unquote_free (حرف *آخري_string);
}

%define api.سرو.include {"parse-gram.h"}
%define api.اڳياڙي {gram_}
%define api.خالص ڀريل
%define api.token.raw
%define api.قدر.نوع اتحاد
%define locations
%define parse.error custom
%define parse.lac ڀريل
%define parse.trace
%header
%expect 0
%verbose

%initial-action
{
  /* Bison's grammar can initial empty locations, hence a default
     location is needed. */
  boundary_سيٽ (&@$.شروعات, grammar_file, 1, 1, 1);
  boundary_سيٽ (&@$.پڇاڙي, grammar_file, 1, 1, 1);
}

%token
  STRING              _("string")
  TSTRING             _("translatable string")

  PERCENT_TOKEN       "%token"
  PERCENT_NTERM       "%nterm"

  PERCENT_TYPE        "%type"
  PERCENT_DESTRUCTOR  "%destructor"
  PERCENT_PRINTER     "%printer"

  PERCENT_کاٻو        "%left"
  PERCENT_ساڄو       "%right"
  PERCENT_NONASSOC    "%nonassoc"
  PERCENT_PRECEDENCE  "%precedence"

  PERCENT_PREC        "%prec"
  PERCENT_DPREC       "%dprec"
  PERCENT_ملايو       "%merge"

  PERCENT_ڪوڊ            "%code"
  PERCENT_DEFAULT_PREC    "%default-prec"
  PERCENT_DEFINE          "%define"
  PERCENT_ERROR_VERBOSE   "%error-verbose"
  PERCENT_EXPECT          "%expect"
  PERCENT_EXPECT_RR       "%expect-rr"
  PERCENT_FILE_اڳياڙي     "%file-prefix"
  PERCENT_FLAG            "%<flag>"
  PERCENT_GLR_PARSER      "%glr-parser"
  PERCENT_سرو          "%header"
  PERCENT_INITIAL_ACTION  "%initial-action"
  PERCENT_LANGUAGE        "%language"
  PERCENT_NAME_اڳياڙي     "%name-prefix"
  PERCENT_NO_DEFAULT_PREC "%no-default-prec"
  PERCENT_NO_سٽون        "%no-lines"
  PERCENT_NONDETERMINISTIC_PARSER "%nondeterministic-parser"
  PERCENT_OUTPUT          "%output"
  PERCENT_خالص_PARSER     "%pure-parser"
  PERCENT_REQUIRE         "%require"
  PERCENT_SKELETON        "%skeleton"
  PERCENT_شروعات           "%start"
  PERCENT_TOKEN_جدول     "%token-table"
  PERCENT_VERBOSE         "%verbose"
  PERCENT_YACC            "%yacc"

  BRACED_ڪوڊ       "{...}"
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
  ٽيگ_2               _("<tag>")
  ٽيگ_ANY           "<*>"
  ٽيگ_NONE          "<>"

 /* Experimental feature, don't rely on it.  */
%code pre-printer  {tron (yyo);}
%code post-printer {troff (yyo);}

%نوع <بنانشان حرف> CHAR_LITERAL
%printer { fputs (char_name ($$), yyo); } <بنانشان حرف>

%نوع <حرف*> "{...}" "%?{...}" "%{...%}" EPILOGUE STRING TSTRING
%printer { fputs ($$, yyo); } <حرف*>

%نوع <uniqstr>
  BRACKETED_ID ID ID_COLON
  PERCENT_ERROR_VERBOSE PERCENT_FILE_اڳياڙي PERCENT_FLAG PERCENT_NAME_اڳياڙي
  PERCENT_خالص_PARSER
  ٽيگ_2 ٽيگ ٽيگ.opt variable
%printer { fputs ($$, yyo); } <uniqstr>
%printer { fprintf (yyo, "[%s]", $$); } BRACKETED_ID
%printer { fprintf (yyo, "%s:", $$); } ID_COLON
%printer { fprintf (yyo, "%%%s", $$); } PERCENT_FLAG
%printer { fprintf (yyo, "<%s>", $$); } ٽيگ_2 ٽيگ

%token <صحيح> INT_LITERAL _("integer literal")
%printer { fprintf (yyo, "%d", $$); } <صحيح>

%نوع <نشان*> id id_colon string_as_id نشان token_decl token_decl_for_prec
%printer { fprintf (yyo, "%s", $$ ? $$->ٽيگ : "<NULL>"); } <نشان*>
%printer { fprintf (yyo, "%s:", $$->ٽيگ); } id_colon

%نوع <assoc> precedence_declarator

%destructor { نشان_list_free ($$); } <نشان_list*>
%printer { نشان_list_syms_print ($$, yyo); } <نشان_list*>

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
  ساكن فراغ جوڙيو_param (param_type نوع, حرف *decl, location loc);
  ساكن param_type current_param = param_none;
};
%token <param_type> PERCENT_PARAM "%param";
%printer
{
  بدل ($$)
    {
#وصف_ڪريو CASE(اندريون_2, ٻاهريون_2)                                           \
      حالة param_ ## اندريون_2: fputs ("%" #Out, yyo); اكسر
      CASE (lex,   lex-param);
      CASE (parse, parse-param);
      CASE (both,  param);
#وصف_رد CASE
      حالة param_none: aver (false); اكسر;
    }
} <param_type>;


                     /*==========\
                     | Grammar.  |
                     \==========*/
%%

داخلا:
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
      muscle_ڪوڊ_grow (union_ڏٺل ? "post_prologue" : "pre_prologue",
                        translate_ڪوڊ ($1, @1, true), @1);
      ڪوڊ_scanner_آخري_string_free ();
    }
| "%<flag>"
    {
      muscle_percent_define_ensure ($1, @1, true);
    }
| "%define" variable قدر
    {
      muscle_percent_define_وجهو ($2, @$, $3.قسم, $3.chars,
                                    MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
    }
| "%header" string.opt             { handle_سرو ($2); }
| "%error-verbose"                 { handle_error_verbose (&@$, $1); }
| "%expect" INT_LITERAL            { expected_sr_conflicts = $2; }
| "%expect-rr" INT_LITERAL         { expected_rr_conflicts = $2; }
| "%file-prefix" STRING            { handle_file_اڳياڙي (&@$, &@1, $1, $2); }
| "%glr-parser"
    {
      nondeterministic_parser = true;
      glr_parser = true;
    }
| "%initial-action" "{...}"
    {
      muscle_ڪوڊ_grow ("initial_action", translate_ڪوڊ ($2, @2, false), @2);
      ڪوڊ_scanner_آخري_string_free ();
    }
| "%language" STRING            { handle_language (&@1, $2); }
| "%name-prefix" STRING         { handle_name_اڳياڙي (&@$, $1, $2); }
| "%no-lines"                   { no_سٽون_flag = true; }
| "%nondeterministic-parser"    { nondeterministic_parser = true; }
| "%output" STRING              { spec_outfile = unquote ($2); gram_scanner_آخري_string_free (); }
| "%param" { current_param = $1; } params { current_param = param_none; }
| "%pure-parser"                { handle_خالص_parser (&@$, $1); }
| "%require" STRING             { handle_require (&@2, $2); }
| "%skeleton" STRING            { handle_skeleton (&@2, $2); }
| "%token-table"                { token_جدول_flag = true; }
| "%verbose"                    { report_flag |= report_حالتون; }
| "%yacc"                       { handle_yacc (&@$); }
| error ";"                     { current_class = unknown_sym; yyerrok; }
| /*FIXME: Err?  What is this horror doing here? */ ";"
;

params:
   params "{...}"  { جوڙيو_param (current_param, $2, @2); }
| "{...}"          { جوڙيو_param (current_param, $1, @1); }
;


/*----------------------.
| grammar_declaration.  |
`----------------------*/

grammar_declaration:
  نشان_declaration
| "%start" symbols.1
    {
      grammar_شروعات_symbols_جوڙيو ($2);
    }
| ڪوڊ_props_type "{...}" generic_symlist
    {
      ڪوڊ_props ڪوڊ;
      ڪوڊ_props_نشان_action_init (&ڪوڊ, $2, @2);
      ڪوڊ_props_translate_ڪوڊ (&ڪوڊ);
      {
        لكل (نشان_list *list = $3; list; list = list->ايندڙ)
          نشان_list_ڪوڊ_props_سيٽ (list, $1, &ڪوڊ);
        نشان_list_free ($3);
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
      muscle_ڪوڊ_grow ("percent_code()",
                        translate_ڪوڊ_braceless ($2, @2), @2);
      ڪوڊ_scanner_آخري_string_free ();
    }
| "%code" ID "{...}"
    {
      muscle_percent_ڪوڊ_grow ($2, @2, translate_ڪوڊ_braceless ($3, @3), @3);
      ڪوڊ_scanner_آخري_string_free ();
    }
;

%نوع <ڪوڊ_props_type> ڪوڊ_props_type;
%printer { fprintf (yyo, "%s", ڪوڊ_props_type_string ($$)); } <ڪوڊ_props_type>;
ڪوڊ_props_type:
  "%destructor"  { $$ = destructor; }
| "%printer"     { $$ = printer; }
;

/*---------.
| %union.  |
`---------*/

%token PERCENT_UNION "%union";

union_name:
  %empty {}
| ID     { muscle_percent_define_وجهو ("api.value.union.name",
                                         @1, muscle_keyword, $1,
                                         MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE); }
;

grammar_declaration:
  "%union" union_name "{...}"
    {
      union_ڏٺل = true;
      muscle_ڪوڊ_grow ("union_members", translate_ڪوڊ_braceless ($3, @3), @3);
      ڪوڊ_scanner_آخري_string_free ();
    }
;


%نوع <نشان_list*> nterm_decls نشان_decls symbols.1
      token_decls token_decls_for_prec
      token_decl.1 token_decl_for_prec.1;
نشان_declaration:
  "%nterm" { current_class = nterm_sym; } nterm_decls[syms]
    {
      current_class = unknown_sym;
      نشان_list_free ($syms);
    }
| "%token" { current_class = token_sym; } token_decls[syms]
    {
      current_class = unknown_sym;
      نشان_list_free ($syms);
    }
| "%type" { current_class = pct_type_sym; } نشان_decls[syms]
    {
      current_class = unknown_sym;
      نشان_list_free ($syms);
    }
| precedence_declarator token_decls_for_prec[syms]
    {
      ++current_prec;
      لكل (نشان_list *list = $syms; list; list = list->ايندڙ)
        نشان_precedence_سيٽ (list->content.sym, current_prec, $1, @1);
      نشان_list_free ($syms);
    }
;

precedence_declarator:
  "%left"       { $$ = کاٻو_assoc; }
| "%right"      { $$ = ساڄو_assoc; }
| "%nonassoc"   { $$ = non_assoc; }
| "%precedence" { $$ = precedence_assoc; }
;

%نوع <حرف*> string.opt;
string.opt:
  %empty  { $$ = عدم; }
| STRING  { $$ = $1; }
;

ٽيگ.opt:
  %empty { $$ = عدم; }
| ٽيگ_2    { $$ = $1; }
;

%نوع <نشان_list*> generic_symlist generic_symlist_شي;
generic_symlist:
  generic_symlist_شي
| generic_symlist generic_symlist_شي   { $$ = نشان_list_append ($1, $2); }
;

generic_symlist_شي:
  نشان    { $$ = نشان_list_sym_new ($1, @1); }
| ٽيگ       { $$ = نشان_list_type_new ($1, @1); }
;

ٽيگ:
  ٽيگ_2
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
| ٽيگ_2 token_decl.1[syms]
    {
      $$ = نشان_list_type_سيٽ ($syms, $ٽيگ_2);
    }
| token_decls ٽيگ_2 token_decl.1[syms]
    {
      $$ = نشان_list_append ($1, نشان_list_type_سيٽ ($syms, $ٽيگ_2));
    }
;

// One or more symbol declarations for %token or %nterm.
token_decl.1:
  token_decl                { $$ = نشان_list_sym_new ($1, @1); }
| token_decl.1 token_decl   { $$ = نشان_list_append ($1, نشان_list_sym_new ($2, @2)); }

// One symbol declaration for %token or %nterm.
token_decl:
  id صحيح.opt[num] alias
    {
      $$ = $id;
      نشان_class_سيٽ ($id, current_class, @id, true);
      إذا (0 <= $انگ)
        نشان_ڪوڊ_سيٽ ($id, $انگ, @انگ);
      إذا ($alias)
        نشان_make_alias ($id, $alias, @alias);
    }
;

%نوع <صحيح> صحيح.opt;
صحيح.opt:
  %empty  { $$ = -1; }
| INT_LITERAL
;

%نوع <نشان*> alias;
alias:
  %empty         { $$ = عدم; }
| string_as_id   { $$ = $1; }
| TSTRING
    {
      $$ = نشان_get ($1, @1);
      نشان_class_سيٽ ($$, token_sym, @1, false);
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
| ٽيگ_2 token_decl_for_prec.1[syms]
    {
      $$ = نشان_list_type_سيٽ ($syms, $ٽيگ_2);
    }
| token_decls_for_prec ٽيگ_2 token_decl_for_prec.1[syms]
    {
      $$ = نشان_list_append ($1, نشان_list_type_سيٽ ($syms, $ٽيگ_2));
    }
;

// One or more token declarations for precedence declaration.
token_decl_for_prec.1:
  token_decl_for_prec
    { $$ = نشان_list_sym_new ($1, @1); }
| token_decl_for_prec.1 token_decl_for_prec
    { $$ = نشان_list_append ($1, نشان_list_sym_new ($2, @2)); }

// One token declaration for precedence declaration.
token_decl_for_prec:
  id صحيح.opt[num]
    {
      $$ = $id;
      نشان_class_سيٽ ($id, token_sym, @id, false);
      إذا (0 <= $انگ)
        نشان_ڪوڊ_سيٽ ($id, $انگ, @انگ);
    }
| string_as_id
;


/*-----------------------------------.
| symbol_decls (argument of %type).  |
`-----------------------------------*/

// A non empty list of typed symbols (for %type).
نشان_decls:
  symbols.1[syms]
    {
      $$ = $syms;
    }
| ٽيگ_2 symbols.1[syms]
    {
      $$ = نشان_list_type_سيٽ ($syms, $ٽيگ_2);
    }
| نشان_decls ٽيگ_2 symbols.1[syms]
    {
      $$ = نشان_list_append ($1, نشان_list_type_سيٽ ($syms, $ٽيگ_2));
    }
;

// One or more symbols.
symbols.1:
  نشان
    {
      إذا (current_class != unknown_sym)
        نشان_class_سيٽ ($نشان, current_class, @نشان, false);
      $$ = نشان_list_sym_new ($نشان, @نشان);
    }
  | symbols.1 نشان
    {
      إذا (current_class != unknown_sym)
        نشان_class_سيٽ ($نشان, current_class, @نشان, false);
      $$ = نشان_list_append ($1, نشان_list_sym_new ($نشان, @نشان));
    }
;

        /*------------------------------------------.
        | The grammar section: between the two %%.  |
        `------------------------------------------*/

grammar:
  قاعدا_or_grammar_declaration
| grammar قاعدا_or_grammar_declaration
;

/* As a Bison extension, one can use the grammar declarations in the
   body of the grammar.  */
قاعدا_or_grammar_declaration:
  قاعدا
| grammar_declaration ";"
| error ";"
    {
      yyerrok;
    }
;

قاعدا:
  id_colon named_ref.opt { current_lhs ($1, @1, $2); } ":" rhses.1
    {
      /* Free the current lhs. */
      current_lhs (0, @1, 0);
    }
;

rhses.1:
  rhs                { grammar_current_قاعدو_پڇاڙي (@rhs); }
| rhses.1 "|" rhs    { grammar_current_قاعدو_پڇاڙي (@rhs); }
| rhses.1 ";"
;

%token PERCENT_خالي_جايون "%empty";
rhs:
  %empty
    { grammar_current_قاعدو_begin (current_lhs_نشان, current_lhs_loc,
                                  current_lhs_named_ref); }
| rhs نشان named_ref.opt
    { grammar_current_قاعدو_نشان_append ($2, @2, $3); }
| rhs ٽيگ.opt "{...}"[action] named_ref.opt[name]
    { grammar_current_قاعدو_action_append ($action, @action, $name, $[tag.opt]); }
| rhs "%?{...}"
    { grammar_current_قاعدو_predicate_append ($2, @2); }
| rhs "%empty"
    { grammar_current_قاعدو_خالي_جايون_سيٽ (@2); }
| rhs "%prec" نشان
    { grammar_current_قاعدو_prec_سيٽ ($3, @3); }
| rhs "%dprec" INT_LITERAL
    { grammar_current_قاعدو_dprec_سيٽ ($3, @3); }
| rhs "%merge" ٽيگ_2
    { grammar_current_قاعدو_ملايو_سيٽ ($3, @3); }
| rhs "%expect" INT_LITERAL
    { grammar_current_قاعدو_expect_sr ($3, @3); }
| rhs "%expect-rr" INT_LITERAL
    { grammar_current_قاعدو_expect_rr ($3, @3); }
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
    حالة muscle_ڪوڊ:    fprintf (yyo,  "{%s}",  $$.chars); اكسر;
    حالة muscle_keyword: fprintf (yyo,   "%s",   $$.chars); اكسر;
    حالة muscle_string:  fprintf (yyo, "\"%s\"", $$.chars); اكسر;
    }
} <قدر_type>;

قدر:
  %empty  { $$.قسم = muscle_keyword; $$.chars = ""; }
| ID      { $$.قسم = muscle_keyword; $$.chars = $1; }
| STRING  { $$.قسم = muscle_string;  $$.chars = unquote ($1); gram_scanner_آخري_string_free ();}
| "{...}" { $$.قسم = muscle_ڪوڊ;    $$.chars = strip_braces ($1); gram_scanner_آخري_string_free (); }
;


/*--------------.
| Identifiers.  |
`--------------*/

/* Identifiers are returned as uniqstr values by the scanner.
   Depending on their use, we may need to make them genuine symbols.  */

id:
  ID
    { $$ = نشان_from_uniqstr ($1, @1); }
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
      $$ = نشان_get (char_name ($1), @1);
      نشان_class_سيٽ ($$, token_sym, @1, false);
      نشان_ڪوڊ_سيٽ ($$, $1, @1);
    }
;

id_colon:
  ID_COLON { $$ = نشان_from_uniqstr ($1, @1); }
;


نشان:
  id
| string_as_id
;

/* A string used as an ID.  */
string_as_id:
  STRING
    {
      $$ = نشان_get ($1, @1);
      نشان_class_سيٽ ($$, token_sym, @1, false);
    }
;

epilogue.opt:
  %empty
| "%%" EPILOGUE
    {
      muscle_ڪوڊ_grow ("epilogue", translate_ڪوڊ ($2, @2, true), @2);
      ڪوڊ_scanner_آخري_string_free ();
    }
;

%%

صحيح
yyreport_syntax_error (ثابت yypcontext_t *ctx)
{
  صحيح res = 0;
  /* Arguments of format: reported tokens (one for the "unexpected",
     one per "expected"). */
  تعداد { دليلون_MAX = 5 };
  ثابت حرف *argv[ARGS_MAX];
  صحيح argc = 0;
  yysymbol_قسم_t unexpected = yypcontext_token (ctx);
  إذا (unexpected != YYSYMBOL_YYEMPTY)
    {
      argv[argc++] = yysymbol_name (unexpected);
      yysymbol_قسم_t expected[ARGS_MAX - 1];
      صحيح nexpected = yypcontext_expected_ٽوڪن (ctx, expected, دليلون_MAX - 1);
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
  loc.شروعات = rhs[n].پڇاڙي;
  loc.پڇاڙي = rhs[n].پڇاڙي;

  /* Ignore empty nonterminals the start of the right-hand side.
     Do not bother to ignore them at the end of the right-hand side,
     since empty nonterminals have the same end as their predecessors.  */
  لكل (صحيح i = 1; i <= n; i++)
    إذا (! equal_boundaries (rhs[i].شروعات, rhs[i].پڇاڙي))
      {
        loc.شروعات = rhs[i].شروعات;
        اكسر;
      }

  أرجع loc;
}

ساكن
حرف *strip_braces (حرف *ڪوڊ)
{
  ڪوڊ[لڙي_ڊيگهه (ڪوڊ) - 1] = 0;
  أرجع ڪوڊ + 1;
}

ساكن
حرف ثابت *
translate_ڪوڊ (حرف *ڪوڊ, location loc, bool plain)
{
  ڪوڊ_props plain_ڪوڊ;
  إذا (plain)
    ڪوڊ_props_plain_init (&plain_ڪوڊ, ڪوڊ, loc);
  وإلا
    ڪوڊ_props_نشان_action_init (&plain_ڪوڊ, ڪوڊ, loc);
  ڪوڊ_props_translate_ڪوڊ (&plain_ڪوڊ);
  gram_scanner_آخري_string_free ();
  أرجع plain_ڪوڊ.ڪوڊ;
}

ساكن
حرف ثابت *
translate_ڪوڊ_braceless (حرف *ڪوڊ, location loc)
{
  أرجع translate_ڪوڊ (strip_braces (ڪوڊ), loc, true);
}

ساكن فراغ
جوڙيو_param (param_type نوع, حرف *decl, location loc)
{
  ساكن حرف ثابت alphanum[26 + 26 + 1 + 10 + 1] =
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "_"
    "0123456789";

  حرف ثابت *name_شروعات = عدم;
  {
    حرف *p;
    /* Stop on last actual character.  */
    لكل (p = decl; p[1]; p++)
      إذا ((p == decl
           || ! memchr (alphanum, p[-1], حجم alphanum - 1))
          && memchr (alphanum, p[0], حجم alphanum - 10 - 1))
        name_شروعات = p;

    /* Strip the surrounding '{' and '}', and any blanks just inside
       the braces.  */
    --p;
    طالما (c_isspace ((بنانشان حرف) *p))
      --p;
    p[1] = '\0';
    ++decl;
    طالما (c_isspace ((بنانشان حرف) *decl))
      ++decl;
  }

  إذا (! name_شروعات)
    complain (&loc, complaint, _("missing identifier in parameter declaration"));
  وإلا
    {
      حرف *name = xmemdup0 (name_شروعات, strspn (name_شروعات, alphanum));
      إذا (نوع & param_lex)
        muscle_pair_list_grow ("lex_param", decl, name);
      إذا (نوع & param_parse)
        muscle_pair_list_grow ("parse_param", decl, name);
      حرر (name);
    }

  gram_scanner_آخري_string_free ();
}


ساكن فراغ
handle_سرو (حرف ثابت *قدر)
{
  سرو_flag = true;
  إذا (قدر)
    {
      حرف *file = unquote (قدر);
      spec_سرو_file = xstrdup (file);
      gram_scanner_آخري_string_free ();
      unquote_free (file);
    }
}


ساكن فراغ
handle_error_verbose (location ثابت *loc, حرف ثابت *directive)
{
  bison_directive (loc, directive);
  muscle_percent_define_وجهو (directive, *loc, muscle_keyword, "",
                                MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
}


ساكن فراغ
handle_file_اڳياڙي (location ثابت *loc,
                    location ثابت *dir_loc,
                    حرف ثابت *directive, حرف ثابت *قدر_quoted)
{
  حرف *قدر = unquote (قدر_quoted);
  bison_directive (loc, directive);
  bool warned = false;

  إذا (location_خالي_جايون (spec_file_اڳياڙي_loc))
    {
      spec_file_اڳياڙي_loc = *loc;
      spec_file_اڳياڙي = قدر;
    }
  وإلا
    {
      duplicate_directive (directive, spec_file_اڳياڙي_loc, *loc);
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
handle_name_اڳياڙي (location ثابت *loc,
                    حرف ثابت *directive, حرف ثابت *قدر_quoted)
{
  حرف *قدر = unquote (قدر_quoted);
  bison_directive (loc, directive);

  حرف buf1[1024];
  size_t len1 = حجم (buf1);
  حرف *old = asnprintf (buf1, &len1, "%s\"%s\"", directive, قدر);
  إذا (!old)
    xalloc_die ();

  إذا (location_خالي_جايون (spec_name_اڳياڙي_loc))
    {
      spec_name_اڳياڙي = قدر;
      spec_name_اڳياڙي_loc = *loc;

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
    duplicate_directive (old, spec_file_اڳياڙي_loc, *loc);

  إذا (old != buf1)
    حرر (old);
}


ساكن فراغ
handle_خالص_parser (location ثابت *loc, حرف ثابت *directive)
{
  bison_directive (loc, directive);
  deprecated_directive (loc, directive, "%define api.pure");
  muscle_percent_define_وجهو ("api.pure", *loc, muscle_keyword, "",
                                MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
}


ساكن فراغ
handle_require (location ثابت *loc, حرف ثابت *نسخو_quoted)
{
  حرف *نسخو = unquote (نسخو_quoted);
  required_نسخو = strversion_منزل_int (نسخو);
  إذا (required_نسخو == -1)
    {
      complain (loc, complaint, _("invalid version requirement: %s"),
                نسخو);
      required_نسخو = 0;
    }
  وإلا
    {
      ثابت حرف* package_نسخو =
        0 < strverscmp (api_نسخو, PACKAGE_نسخو)
        ? api_نسخو : PACKAGE_نسخو;
      إذا (0 < strverscmp (نسخو, package_نسخو))
        {
          complain (loc, complaint, _("require bison %s, but have %s"),
                    نسخو, package_نسخو);
          ٻاهر_نڪرو (EX_MISMATCH);
        }
    }
  unquote_free (نسخو);
  gram_scanner_آخري_string_free ();
}

ساكن فراغ
handle_skeleton (location ثابت *loc, حرف ثابت *skel_quoted)
{
  حرف *skel = unquote (skel_quoted);
  حرف ثابت *skeleton_user = skel;
  إذا (strchr (skeleton_user, '/'))
    {
      size_t dir_ڊيگهائي = لڙي_ڊيگهه (grammar_file);
      طالما (dir_ڊيگهائي && grammar_file[dir_length - 1] != '/')
        --dir_length;
      طالما (dir_ڊيگهائي && grammar_file[dir_length - 1] == '/')
        --dir_length;
      حرف *skeleton_build =
        xmalloc (dir_ڊيگهائي + 1 + لڙي_ڊيگهه (skeleton_user) + 1);
      إذا (dir_ڊيگهائي > 0)
        {
          يادگيري_نقل (skeleton_build, grammar_file, dir_ڊيگهائي);
          skeleton_build[dir_length++] = '/';
        }
      strcpy (skeleton_build + dir_ڊيگهائي, skeleton_user);
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
  إذا (location_خالي_جايون (yacc_loc))
    سيٽ_yacc (*loc);
  وإلا
    duplicate_directive (directive, yacc_loc, *loc);
}


ساكن فراغ
gram_error (location ثابت *loc, حرف ثابت *نياپو)
{
  complain (loc, complaint, "%s", نياپو);
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
current_lhs (نشان *sym, location loc, named_ref *ref)
{
  current_lhs_نشان = sym;
  current_lhs_loc = loc;
  إذا (sym)
    نشان_location_as_lhs_سيٽ (sym, loc);
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
  پڇاڙي_use_class ("value", yyo);
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
unquote_free (حرف *آخري_string)
{
  obstack_free (&obstack_for_unquote, آخري_string);
}

ساكن حرف *
unquote (ثابت حرف *cp)
{
#وصف_ڪريو GROW(Char)                              \
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
#وصف_رد GROW
  أرجع obstack_finish0 (&obstack_for_unquote);
}
