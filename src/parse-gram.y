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
  #அந்தர்பாவ "symlist.h"
  #அந்தர்பாவ "symtab.h"
}

%code provides
{
  /* Initialize unquote.  */
  வெறுமை parser_init (வெறுமை);
  /* Deallocate storage for unquote.  */
  வெறுமை parser_free (வெறுமை);
}

%code உச்சி_2
{
  /* On column 0 to please syntax-check.  */
#அந்தர்பாவ <config.h>
}

%code
{
  #அந்தர்பாவ "system.h"

  #அந்தர்பாவ <c-ctype.h>
  #அந்தர்பாவ <quotearg.h>
  #அந்தர்பாவ <vasnprintf.h>
  #அந்தர்பாவ <xmemdup0.h>

  #அந்தர்பாவ "complain.h"
  #அந்தர்பாவ "conflicts.h"
  #அந்தர்பாவ "files.h"
  #அந்தர்பாவ "getargs.h"
  #அந்தர்பாவ "gram.h"
  #அந்தர்பாவ "named-ref.h"
  #அந்தர்பாவ "reader.h"
  #அந்தர்பாவ "scan-code.h"
  #அந்தர்பாவ "scan-gram.h"
  #அந்தர்பாவ "strversion.h"

  /* Pretend to be at least that version, to check features published
     in that version while developping it.  */
  நிலையான மாறிலி எழுத்து* api_version = "3.8";

  நிலையான முழு current_prec = 0;
  நிலையான location current_lhs_loc;
  நிலையான named_ref *current_lhs_named_ref;
  நிலையான குறியீடு *current_lhs_குறியீடு;
  நிலையான குறியீடு_class current_class = unknown_sym;

  /** Set the new current left-hand side symbol, possibly common
   * to several right-hand side parts of rule.
   */
  நிலையான வெறுமை current_lhs (குறியீடு *sym, location loc, named_ref *ref);

  #define YYLLOC_DEFAULT(Current, Rhs, N)         \
    (Current) = lloc_default (Rhs, N)
  நிலையான YYLTYPE lloc_default (YYLTYPE மாறிலி *, முழு);

  #define YY_LOCATION_PRINT(File, Loc)            \
    location_print (Loc, File)

  /* Strip initial '{' and final '}' (must be first and last characters).
     Return the result.  */
  நிலையான எழுத்து *strip_braces (எழுத்து *code);

  /* Convert CODE by calling code_props_plain_init if PLAIN, otherwise
     code_props_symbol_action_init.  Calls
     gram_scanner_last_string_free to release the latest string from
     the scanner (should be CODE). */
  நிலையான எழுத்து மாறிலி *translate_code (எழுத்து *code, location loc, bool plain);

  /* Convert CODE by calling code_props_plain_init after having
     stripped the first and last characters (expected to be '{', and
     '}').  Calls gram_scanner_last_string_free to release the latest
     string from the scanner (should be CODE). */
  நிலையான எழுத்து மாறிலி *translate_code_braceless (எழுத்து *code, location loc);

  /* Handle a %header directive.  */
  நிலையான வெறுமை handle_header (எழுத்து மாறிலி *மதிப்பு);

  /* Handle a %error-verbose directive.  */
  நிலையான வெறுமை handle_error_verbose (location மாறிலி *loc, எழுத்து மாறிலி *directive);

  /* Handle a %file-prefix directive.  */
  நிலையான வெறுமை handle_file_prefix (location மாறிலி *loc,
                                  location மாறிலி *dir_loc,
                                  எழுத்து மாறிலி *directive, எழுத்து மாறிலி *மதிப்பு);

  /* Handle a %language directive.  */
  நிலையான வெறுமை handle_language (location மாறிலி *loc, எழுத்து மாறிலி *lang);

  /* Handle a %name-prefix directive.  */
  நிலையான வெறுமை handle_name_prefix (location மாறிலி *loc,
                                  எழுத்து மாறிலி *directive, எழுத்து மாறிலி *மதிப்பு);

  /* Handle a %pure-parser directive.  */
  நிலையான வெறுமை handle_pure_parser (location மாறிலி *loc, எழுத்து மாறிலி *directive);

  /* Handle a %require directive.  */
  நிலையான வெறுமை handle_require (location மாறிலி *loc, எழுத்து மாறிலி *version);

  /* Handle a %skeleton directive.  */
  நிலையான வெறுமை handle_skeleton (location மாறிலி *loc, எழுத்து மாறிலி *skel);

  /* Handle a %yacc directive.  */
  நிலையான வெறுமை handle_yacc (location மாறிலி *loc);

  /* Implementation of yyerror.  */
  நிலையான வெறுமை gram_error (location மாறிலி *, எழுத்து மாறிலி *);

  /* A string that describes a char (e.g., 'a' -> "'a'").  */
  நிலையான எழுத்து மாறிலி *char_name (எழுத்து);

  /* Add style to semantic values in traces.  */
  நிலையான வெறுமை tron (FILE *yyo);
  நிலையான வெறுமை troff (FILE *yyo);

  /* Interpret a quoted string (such as `"Hello, \"World\"\n\""`).
     Manages the memory of the result.  */
  நிலையான எழுத்து *unquote (மாறிலி எழுத்து *str);

  /* Discard the latest unquoted string.  */
  நிலையான வெறுமை unquote_free (எழுத்து *last_string);
}

%define api.header.include {"parse-gram.h"}
%define api.prefix {gram_}
%define api.pure full
%define api.token.raw
%define api.மதிப்பு.வகை ஒன்றிப்பு
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

  PERCENT_இடது        "%left"
  PERCENT_வலது       "%right"
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
  PERCENT_NO_வரிகள்        "%no-lines"
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

%வகை <unsigned எழுத்து> CHAR_LITERAL
%printer { fputs (char_name ($$), yyo); } <unsigned எழுத்து>

%வகை <எழுத்து*> "{...}" "%?{...}" "%{...%}" EPILOGUE STRING TSTRING
%printer { fputs ($$, yyo); } <எழுத்து*>

%வகை <uniqstr>
  BRACKETED_ID ID ID_COLON
  PERCENT_ERROR_VERBOSE PERCENT_FILE_PREFIX PERCENT_FLAG PERCENT_NAME_PREFIX
  PERCENT_PURE_PARSER
  TAG tag tag.opt variable
%printer { fputs ($$, yyo); } <uniqstr>
%printer { fprintf (yyo, "[%s]", $$); } BRACKETED_ID
%printer { fprintf (yyo, "%s:", $$); } ID_COLON
%printer { fprintf (yyo, "%%%s", $$); } PERCENT_FLAG
%printer { fprintf (yyo, "<%s>", $$); } TAG tag

%token <முழு> INT_LITERAL _("integer literal")
%printer { fprintf (yyo, "%d", $$); } <முழு>

%வகை <குறியீடு*> id id_colon string_as_id குறியீடு token_decl token_decl_for_prec
%printer { fprintf (yyo, "%s", $$ ? $$->tag : "<NULL>"); } <குறியீடு*>
%printer { fprintf (yyo, "%s:", $$->tag); } id_colon

%வகை <assoc> precedence_declarator

%destructor { குறியீடு_list_free ($$); } <குறியீடு_list*>
%printer { குறியீடு_list_syms_print ($$, yyo); } <குறியீடு_list*>

%வகை <named_ref*> named_ref.opt

/*---------.
| %param.  |
`---------*/
%code requires
{
  வகைவரை எண்ணிக்கை
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
  நிலையான வெறுமை add_param (param_type வகை, எழுத்து *decl, location loc);
  நிலையான param_type current_param = param_none;
};
%token <param_type> PERCENT_PARAM "%param";
%printer
{
  தேர்வு ($$)
    {
#define CASE(In, Out)                                           \
      நிலை param_ ## In: fputs ("%" #Out, yyo); நிறுத்து
      CASE (lex,   lex-param);
      CASE (parse, parse-param);
      CASE (both,  param);
#undef CASE
      நிலை param_none: aver (false); நிறுத்து;
    }
} <param_type>;


                     /*==========\
                     | Grammar.  |
                     \==========*/
%%

உள்ளீடு:
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
| "%define" variable மதிப்பு
    {
      muscle_percent_define_செருகு ($2, @$, $3.kind, $3.chars,
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
| "%no-lines"                   { no_வரிகள்_flag = true; }
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
  குறியீடு_declaration
| "%start" symbols.1
    {
      grammar_start_symbols_add ($2);
    }
| code_props_type "{...}" generic_symlist
    {
      code_props code;
      code_props_குறியீடு_action_init (&code, $2, @2);
      code_props_translate_code (&code);
      {
        க்கு (குறியீடு_list *list = $3; list; list = list->next)
          குறியீடு_list_code_props_set (list, $1, &code);
        குறியீடு_list_free ($3);
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

%வகை <code_props_type> code_props_type;
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
| ID     { muscle_percent_define_செருகு ("api.value.union.name",
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


%வகை <குறியீடு_list*> nterm_decls குறியீடு_decls symbols.1
      token_decls token_decls_for_prec
      token_decl.1 token_decl_for_prec.1;
குறியீடு_declaration:
  "%nterm" { current_class = nterm_sym; } nterm_decls[syms]
    {
      current_class = unknown_sym;
      குறியீடு_list_free ($syms);
    }
| "%token" { current_class = token_sym; } token_decls[syms]
    {
      current_class = unknown_sym;
      குறியீடு_list_free ($syms);
    }
| "%type" { current_class = pct_type_sym; } குறியீடு_decls[syms]
    {
      current_class = unknown_sym;
      குறியீடு_list_free ($syms);
    }
| precedence_declarator token_decls_for_prec[syms]
    {
      ++current_prec;
      க்கு (குறியீடு_list *list = $syms; list; list = list->next)
        குறியீடு_precedence_set (list->content.sym, current_prec, $1, @1);
      குறியீடு_list_free ($syms);
    }
;

precedence_declarator:
  "%left"       { $$ = இடது_assoc; }
| "%right"      { $$ = வலது_assoc; }
| "%nonassoc"   { $$ = non_assoc; }
| "%precedence" { $$ = precedence_assoc; }
;

%வகை <எழுத்து*> string.opt;
string.opt:
  %empty  { $$ = வெற்று; }
| STRING  { $$ = $1; }
;

tag.opt:
  %empty { $$ = வெற்று; }
| TAG    { $$ = $1; }
;

%வகை <குறியீடு_list*> generic_symlist generic_symlist_item;
generic_symlist:
  generic_symlist_item
| generic_symlist generic_symlist_item   { $$ = குறியீடு_list_append ($1, $2); }
;

generic_symlist_item:
  குறியீடு    { $$ = குறியீடு_list_sym_new ($1, @1); }
| tag       { $$ = குறியீடு_list_type_new ($1, @1); }
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
      $$ = குறியீடு_list_type_set ($syms, $TAG);
    }
| token_decls TAG token_decl.1[syms]
    {
      $$ = குறியீடு_list_append ($1, குறியீடு_list_type_set ($syms, $TAG));
    }
;

// One or more symbol declarations for %token or %nterm.
token_decl.1:
  token_decl                { $$ = குறியீடு_list_sym_new ($1, @1); }
| token_decl.1 token_decl   { $$ = குறியீடு_list_append ($1, குறியீடு_list_sym_new ($2, @2)); }

// One symbol declaration for %token or %nterm.
token_decl:
  id முழு.opt[num] alias
    {
      $$ = $id;
      குறியீடு_class_set ($id, current_class, @id, true);
      எனில் (0 <= $இலக்கம்)
        குறியீடு_code_set ($id, $இலக்கம், @இலக்கம்);
      எனில் ($alias)
        குறியீடு_make_alias ($id, $alias, @alias);
    }
;

%வகை <முழு> முழு.opt;
முழு.opt:
  %empty  { $$ = -1; }
| INT_LITERAL
;

%வகை <குறியீடு*> alias;
alias:
  %empty         { $$ = வெற்று; }
| string_as_id   { $$ = $1; }
| TSTRING
    {
      $$ = குறியீடு_get ($1, @1);
      குறியீடு_class_set ($$, token_sym, @1, false);
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
      $$ = குறியீடு_list_type_set ($syms, $TAG);
    }
| token_decls_for_prec TAG token_decl_for_prec.1[syms]
    {
      $$ = குறியீடு_list_append ($1, குறியீடு_list_type_set ($syms, $TAG));
    }
;

// One or more token declarations for precedence declaration.
token_decl_for_prec.1:
  token_decl_for_prec
    { $$ = குறியீடு_list_sym_new ($1, @1); }
| token_decl_for_prec.1 token_decl_for_prec
    { $$ = குறியீடு_list_append ($1, குறியீடு_list_sym_new ($2, @2)); }

// One token declaration for precedence declaration.
token_decl_for_prec:
  id முழு.opt[num]
    {
      $$ = $id;
      குறியீடு_class_set ($id, token_sym, @id, false);
      எனில் (0 <= $இலக்கம்)
        குறியீடு_code_set ($id, $இலக்கம், @இலக்கம்);
    }
| string_as_id
;


/*-----------------------------------.
| symbol_decls (argument of %type).  |
`-----------------------------------*/

// A non empty list of typed symbols (for %type).
குறியீடு_decls:
  symbols.1[syms]
    {
      $$ = $syms;
    }
| TAG symbols.1[syms]
    {
      $$ = குறியீடு_list_type_set ($syms, $TAG);
    }
| குறியீடு_decls TAG symbols.1[syms]
    {
      $$ = குறியீடு_list_append ($1, குறியீடு_list_type_set ($syms, $TAG));
    }
;

// One or more symbols.
symbols.1:
  குறியீடு
    {
      எனில் (current_class != unknown_sym)
        குறியீடு_class_set ($குறியீடு, current_class, @குறியீடு, false);
      $$ = குறியீடு_list_sym_new ($குறியீடு, @குறியீடு);
    }
  | symbols.1 குறியீடு
    {
      எனில் (current_class != unknown_sym)
        குறியீடு_class_set ($குறியீடு, current_class, @குறியீடு, false);
      $$ = குறியீடு_list_append ($1, குறியீடு_list_sym_new ($குறியீடு, @குறியீடு));
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
    { grammar_current_rule_begin (current_lhs_குறியீடு, current_lhs_loc,
                                  current_lhs_named_ref); }
| rhs குறியீடு named_ref.opt
    { grammar_current_rule_குறியீடு_append ($2, @2, $3); }
| rhs tag.opt "{...}"[action] named_ref.opt[name]
    { grammar_current_rule_action_append ($action, @action, $name, $[tag.opt]); }
| rhs "%?{...}"
    { grammar_current_rule_predicate_append ($2, @2); }
| rhs "%empty"
    { grammar_current_rule_empty_set (@2); }
| rhs "%prec" குறியீடு
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
  %empty         { $$ = வெற்று; }
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
  #அந்தர்பாவ "muscle-tab.h"
  வகைவரை கட்டமைப்பு
  {
    எழுத்து மாறிலி *chars;
    muscle_kind kind;
  } மதிப்பு_type;
};
%வகை <மதிப்பு_type> மதிப்பு;
%printer
{
  தேர்வு ($$.kind)
    {
    நிலை muscle_code:    fprintf (yyo,  "{%s}",  $$.chars); நிறுத்து;
    நிலை muscle_keyword: fprintf (yyo,   "%s",   $$.chars); நிறுத்து;
    நிலை muscle_string:  fprintf (yyo, "\"%s\"", $$.chars); நிறுத்து;
    }
} <மதிப்பு_type>;

மதிப்பு:
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
    { $$ = குறியீடு_from_uniqstr ($1, @1); }
| CHAR_LITERAL
    {
      மாறிலி எழுத்து *var = "api.token.raw";
      எனில் (current_class == nterm_sym)
        {
          complain (&@1, complaint,
                    _("character literals cannot be nonterminals"));
          YYERROR;
        }
      எனில் (muscle_percent_define_ifdef (var))
        {
          complain (&@1, complaint,
                    _("character literals cannot be used together"
                    " with %s"), var);
          location loc = muscle_percent_define_get_loc (var);
          subcomplain (&loc, complaint, _("definition of %s"), var);
        }
      $$ = குறியீடு_get (char_name ($1), @1);
      குறியீடு_class_set ($$, token_sym, @1, false);
      குறியீடு_code_set ($$, $1, @1);
    }
;

id_colon:
  ID_COLON { $$ = குறியீடு_from_uniqstr ($1, @1); }
;


குறியீடு:
  id
| string_as_id
;

/* A string used as an ID.  */
string_as_id:
  STRING
    {
      $$ = குறியீடு_get ($1, @1);
      குறியீடு_class_set ($$, token_sym, @1, false);
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

முழு
yyreport_syntax_error (மாறிலி yypcontext_t *ctx)
{
  முழு res = 0;
  /* Arguments of format: reported tokens (one for the "unexpected",
     one per "expected"). */
  எண்ணிக்கை { ARGS_MAX = 5 };
  மாறிலி எழுத்து *argv[ARGS_MAX];
  முழு argc = 0;
  yysymbol_kind_t unexpected = yypcontext_token (ctx);
  எனில் (unexpected != YYSYMBOL_YYEMPTY)
    {
      argv[argc++] = yysymbol_name (unexpected);
      yysymbol_kind_t expected[ARGS_MAX - 1];
      முழு nexpected = yypcontext_expected_tokens (ctx, expected, ARGS_MAX - 1);
      எனில் (nexpected < 0)
        res = nexpected;
      இல்லையெனில்
        க்கு (முழு i = 0; i < nexpected; ++i)
          argv[argc++] = yysymbol_name (expected[i]);
    }
  syntax_error (*yypcontext_location (ctx), argc, argv);
  திரும்பு res;
}


/* Return the location of the left-hand side of a rule whose
   right-hand side is RHS[1] ... RHS[N].  Ignore empty nonterminals in
   the right-hand side, and return an empty location equal to the end
   boundary of RHS[0] if the right-hand side is empty.  */

நிலையான YYLTYPE
lloc_default (YYLTYPE மாறிலி *rhs, முழு n)
{
  YYLTYPE loc;

  /* SGI MIPSpro 7.4.1m miscompiles "loc.start = loc.end = rhs[n].end;".
     The bug is fixed in 7.4.2m, but play it safe for now.  */
  loc.start = rhs[n].end;
  loc.end = rhs[n].end;

  /* Ignore empty nonterminals the start of the right-hand side.
     Do not bother to ignore them at the end of the right-hand side,
     since empty nonterminals have the same end as their predecessors.  */
  க்கு (முழு i = 1; i <= n; i++)
    எனில் (! equal_boundaries (rhs[i].start, rhs[i].end))
      {
        loc.start = rhs[i].start;
        நிறுத்து;
      }

  திரும்பு loc;
}

நிலையான
எழுத்து *strip_braces (எழுத்து *code)
{
  code[strlen (code) - 1] = 0;
  திரும்பு code + 1;
}

நிலையான
எழுத்து மாறிலி *
translate_code (எழுத்து *code, location loc, bool plain)
{
  code_props plain_code;
  எனில் (plain)
    code_props_plain_init (&plain_code, code, loc);
  இல்லையெனில்
    code_props_குறியீடு_action_init (&plain_code, code, loc);
  code_props_translate_code (&plain_code);
  gram_scanner_last_string_free ();
  திரும்பு plain_code.code;
}

நிலையான
எழுத்து மாறிலி *
translate_code_braceless (எழுத்து *code, location loc)
{
  திரும்பு translate_code (strip_braces (code), loc, true);
}

நிலையான வெறுமை
add_param (param_type வகை, எழுத்து *decl, location loc)
{
  நிலையான எழுத்து மாறிலி alphanum[26 + 26 + 1 + 10 + 1] =
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "_"
    "0123456789";

  எழுத்து மாறிலி *name_start = வெற்று;
  {
    எழுத்து *p;
    /* Stop on last actual character.  */
    க்கு (p = decl; p[1]; p++)
      எனில் ((p == decl
           || ! memchr (alphanum, p[-1], அளவு alphanum - 1))
          && memchr (alphanum, p[0], அளவு alphanum - 10 - 1))
        name_start = p;

    /* Strip the surrounding '{' and '}', and any blanks just inside
       the braces.  */
    --p;
    போது (c_isspace ((unsigned எழுத்து) *p))
      --p;
    p[1] = '\0';
    ++decl;
    போது (c_isspace ((unsigned எழுத்து) *decl))
      ++decl;
  }

  எனில் (! name_start)
    complain (&loc, complaint, _("missing identifier in parameter declaration"));
  இல்லையெனில்
    {
      எழுத்து *name = xmemdup0 (name_start, strspn (name_start, alphanum));
      எனில் (வகை & param_lex)
        muscle_pair_list_grow ("lex_param", decl, name);
      எனில் (வகை & param_parse)
        muscle_pair_list_grow ("parse_param", decl, name);
      விடுவி (name);
    }

  gram_scanner_last_string_free ();
}


நிலையான வெறுமை
handle_header (எழுத்து மாறிலி *மதிப்பு)
{
  header_flag = true;
  எனில் (மதிப்பு)
    {
      எழுத்து *file = unquote (மதிப்பு);
      spec_header_file = xstrdup (file);
      gram_scanner_last_string_free ();
      unquote_free (file);
    }
}


நிலையான வெறுமை
handle_error_verbose (location மாறிலி *loc, எழுத்து மாறிலி *directive)
{
  bison_directive (loc, directive);
  muscle_percent_define_செருகு (directive, *loc, muscle_keyword, "",
                                MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
}


நிலையான வெறுமை
handle_file_prefix (location மாறிலி *loc,
                    location மாறிலி *dir_loc,
                    எழுத்து மாறிலி *directive, எழுத்து மாறிலி *மதிப்பு_quoted)
{
  எழுத்து *மதிப்பு = unquote (மதிப்பு_quoted);
  bison_directive (loc, directive);
  bool warned = false;

  எனில் (location_empty (spec_file_prefix_loc))
    {
      spec_file_prefix_loc = *loc;
      spec_file_prefix = மதிப்பு;
    }
  இல்லையெனில்
    {
      duplicate_directive (directive, spec_file_prefix_loc, *loc);
      warned = true;
    }

  எனில் (!warned
      && STRNEQ (directive, "%file-prefix"))
    deprecated_directive (dir_loc, directive, "%file-prefix");
}

நிலையான வெறுமை
handle_language (location மாறிலி *loc, எழுத்து மாறிலி *lang)
{
  language_argmatch (unquote (lang), grammar_prio, *loc);
}


நிலையான வெறுமை
handle_name_prefix (location மாறிலி *loc,
                    எழுத்து மாறிலி *directive, எழுத்து மாறிலி *மதிப்பு_quoted)
{
  எழுத்து *மதிப்பு = unquote (மதிப்பு_quoted);
  bison_directive (loc, directive);

  எழுத்து buf1[1024];
  size_t len1 = அளவு (buf1);
  எழுத்து *old = asnprintf (buf1, &len1, "%s\"%s\"", directive, மதிப்பு);
  எனில் (!old)
    xalloc_die ();

  எனில் (location_empty (spec_name_prefix_loc))
    {
      spec_name_prefix = மதிப்பு;
      spec_name_prefix_loc = *loc;

      எழுத்து buf2[1024];
      size_t len2 = அளவு (buf2);
      எழுத்து *new = asnprintf (buf2, &len2, "%%define api.prefix {%s}", மதிப்பு);
      எனில் (!new)
        xalloc_die ();
      deprecated_directive (loc, old, new);
      எனில் (new != buf2)
        விடுவி (new);
    }
  இல்லையெனில்
    duplicate_directive (old, spec_file_prefix_loc, *loc);

  எனில் (old != buf1)
    விடுவி (old);
}


நிலையான வெறுமை
handle_pure_parser (location மாறிலி *loc, எழுத்து மாறிலி *directive)
{
  bison_directive (loc, directive);
  deprecated_directive (loc, directive, "%define api.pure");
  muscle_percent_define_செருகு ("api.pure", *loc, muscle_keyword, "",
                                MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
}


நிலையான வெறுமை
handle_require (location மாறிலி *loc, எழுத்து மாறிலி *version_quoted)
{
  எழுத்து *version = unquote (version_quoted);
  required_version = strversion_to_int (version);
  எனில் (required_version == -1)
    {
      complain (loc, complaint, _("invalid version requirement: %s"),
                version);
      required_version = 0;
    }
  இல்லையெனில்
    {
      மாறிலி எழுத்து* package_version =
        0 < strverscmp (api_version, PACKAGE_VERSION)
        ? api_version : PACKAGE_VERSION;
      எனில் (0 < strverscmp (version, package_version))
        {
          complain (loc, complaint, _("require bison %s, but have %s"),
                    version, package_version);
          exit (EX_MISMATCH);
        }
    }
  unquote_free (version);
  gram_scanner_last_string_free ();
}

நிலையான வெறுமை
handle_skeleton (location மாறிலி *loc, எழுத்து மாறிலி *skel_quoted)
{
  எழுத்து *skel = unquote (skel_quoted);
  எழுத்து மாறிலி *skeleton_user = skel;
  எனில் (strchr (skeleton_user, '/'))
    {
      size_t dir_length = strlen (grammar_file);
      போது (dir_length && grammar_file[dir_length - 1] != '/')
        --dir_length;
      போது (dir_length && grammar_file[dir_length - 1] == '/')
        --dir_length;
      எழுத்து *skeleton_build =
        xmalloc (dir_length + 1 + strlen (skeleton_user) + 1);
      எனில் (dir_length > 0)
        {
          memcpy (skeleton_build, grammar_file, dir_length);
          skeleton_build[dir_length++] = '/';
        }
      strcpy (skeleton_build + dir_length, skeleton_user);
      skeleton_user = uniqstr_new (skeleton_build);
      விடுவி (skeleton_build);
    }
  skeleton_arg (skeleton_user, grammar_prio, *loc);
}


நிலையான வெறுமை
handle_yacc (location மாறிலி *loc)
{
  மாறிலி எழுத்து *directive = "%yacc";
  bison_directive (loc, directive);
  எனில் (location_empty (yacc_loc))
    set_yacc (*loc);
  இல்லையெனில்
    duplicate_directive (directive, yacc_loc, *loc);
}


நிலையான வெறுமை
gram_error (location மாறிலி *loc, எழுத்து மாறிலி *செய்தி)
{
  complain (loc, complaint, "%s", செய்தி);
}

நிலையான எழுத்து மாறிலி *
char_name (எழுத்து c)
{
  எனில் (c == '\'')
    திரும்பு "'\\''";
  இல்லையெனில்
    {
      எழுத்து buf[4];
      buf[0] = '\''; buf[1] = c; buf[2] = '\''; buf[3] = '\0';
      திரும்பு quotearg_style (escape_quoting_style, buf);
    }
}

நிலையான வெறுமை
current_lhs (குறியீடு *sym, location loc, named_ref *ref)
{
  current_lhs_குறியீடு = sym;
  current_lhs_loc = loc;
  எனில் (sym)
    குறியீடு_location_as_lhs_set (sym, loc);
  /* In order to simplify memory management, named references for lhs
     are always assigned by deep copy into the current symbol_list
     node.  This is because a single named-ref in the grammar may
     result in several uses when the user factors lhs between several
     rules using "|".  Therefore free the parser's original copy.  */
  விடுவி (current_lhs_named_ref);
  current_lhs_named_ref = ref;
}

நிலையான வெறுமை tron (FILE *yyo)
{
  begin_use_class ("value", yyo);
}

நிலையான வெறுமை troff (FILE *yyo)
{
  end_use_class ("value", yyo);
}


/*----------.
| Unquote.  |
`----------*/

கட்டமைப்பு obstack obstack_for_unquote;

வெறுமை
parser_init (வெறுமை)
{
  obstack_init (&obstack_for_unquote);
}

வெறுமை
parser_free (வெறுமை)
{
  obstack_free (&obstack_for_unquote, 0);
}

நிலையான வெறுமை
unquote_free (எழுத்து *last_string)
{
  obstack_free (&obstack_for_unquote, last_string);
}

நிலையான எழுத்து *
unquote (மாறிலி எழுத்து *cp)
{
#define GROW(Char)                              \
  obstack_1grow (&obstack_for_unquote, Char);
  க்கு (++cp; *cp && *cp != '"'; ++cp)
    தேர்வு (*cp)
      {
      நிலை '"':
        நிறுத்து;
      நிலை '\\':
        ++cp;
        தேர்வு (*cp)
          {
          நிலை '0': நிலை '1': நிலை '2': நிலை '3': நிலை '4':
          நிலை '5': நிலை '6': நிலை '7': நிலை '8': நிலை '9':
            {
              முழு c = cp[0] - '0';
              எனில் (c_isdigit (cp[1]))
                {
                  ++cp;
                  c = c * 8 + cp[0] - '0';
                }
              எனில் (c_isdigit (cp[1]))
                {
                  ++cp;
                  c = c * 8 + cp[0] - '0';
                }
              GROW (c);
            }
            நிறுத்து;

          நிலை 'a': GROW ('\a'); நிறுத்து;
          நிலை 'b': GROW ('\b'); நிறுத்து;
          நிலை 'f': GROW ('\f'); நிறுத்து;
          நிலை 'n': GROW ('\n'); நிறுத்து;
          நிலை 'r': GROW ('\r'); நிறுத்து;
          நிலை 't': GROW ('\t'); நிறுத்து;
          நிலை 'v': GROW ('\v'); நிறுத்து;

          நிலை 'x':
            {
              முழு c = 0;
              போது (c_isxdigit (cp[1]))
                {
                  ++cp;
                  c = (c * 16 + (c_isdigit (cp[0]) ? cp[0] - '0'
                                 : c_isupper (cp[0]) ? cp[0] - 'A'
                                 : cp[0] - '0'));
                }
              GROW (c);
              நிறுத்து;
            }
          }
        நிறுத்து;

      இயல்பு:
        GROW (*cp);
        நிறுத்து;
      }
  ப்ரதிஜ்ஞா (*cp == '"');
  ++cp;
  ப்ரதிஜ்ஞா (*cp == '\0');
#undef GROW
  திரும்பு obstack_finish0 (&obstack_for_unquote);
}
