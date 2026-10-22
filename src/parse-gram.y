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
  #अन्तर्भाव "symlist.h"
  #अन्तर्भाव "symtab.h"
}

%code provides
{
  /* Initialize unquote.  */
  शून्य parser_init (शून्य);
  /* Deallocate storage for unquote.  */
  शून्य parser_free (शून्य);
}

%code शिखरम्_2
{
  /* On column 0 to please syntax-check.  */
#अन्तर्भाव <config.h>
}

%code
{
  #अन्तर्भाव "system.h"

  #अन्तर्भाव <c-ctype.h>
  #अन्तर्भाव <quotearg.h>
  #अन्तर्भाव <vasnprintf.h>
  #अन्तर्भाव <xmemdup0.h>

  #अन्तर्भाव "complain.h"
  #अन्तर्भाव "conflicts.h"
  #अन्तर्भाव "files.h"
  #अन्तर्भाव "getargs.h"
  #अन्तर्भाव "gram.h"
  #अन्तर्भाव "named-ref.h"
  #अन्तर्भाव "reader.h"
  #अन्तर्भाव "scan-code.h"
  #अन्तर्भाव "scan-gram.h"
  #अन्तर्भाव "strversion.h"

  /* Pretend to be at least that version, to check features published
     in that version while developping it.  */
  स्थिर ध्रुव वर्ण* api_संस्करणम् = "3.8";

  स्थिर पूर्णाङ्क current_prec = 0;
  स्थिर location current_lhs_loc;
  स्थिर named_ref *current_lhs_named_ref;
  स्थिर चिह्नम् *current_lhs_चिह्नम्;
  स्थिर चिह्नम्_class current_class = unknown_sym;

  /** Set the new current left-hand side symbol, possibly common
   * to several right-hand side parts of rule.
   */
  स्थिर शून्य current_lhs (चिह्नम् *sym, location loc, named_ref *ref);

  #परिभाषय YYLLOC_DEFAULT(Current, Rhs, N)         \
    (Current) = lloc_default (Rhs, N)
  स्थिर YYLTYPE lloc_default (YYLTYPE ध्रुव *, पूर्णाङ्क);

  #परिभाषय YY_LOCATION_PRINT(File, Loc)            \
    location_print (Loc, File)

  /* Strip initial '{' and final '}' (must be first and last characters).
     Return the result.  */
  स्थिर वर्ण *strip_braces (वर्ण *संहिता);

  /* Convert CODE by calling code_props_plain_init if PLAIN, otherwise
     code_props_symbol_action_init.  Calls
     gram_scanner_last_string_free to release the latest string from
     the scanner (should be CODE). */
  स्थिर वर्ण ध्रुव *translate_संहिता (वर्ण *संहिता, location loc, bool plain);

  /* Convert CODE by calling code_props_plain_init after having
     stripped the first and last characters (expected to be '{', and
     '}').  Calls gram_scanner_last_string_free to release the latest
     string from the scanner (should be CODE). */
  स्थिर वर्ण ध्रुव *translate_संहिता_braceless (वर्ण *संहिता, location loc);

  /* Handle a %header directive.  */
  स्थिर शून्य handle_शीर्षकम् (वर्ण ध्रुव *मानम्);

  /* Handle a %error-verbose directive.  */
  स्थिर शून्य handle_error_verbose (location ध्रुव *loc, वर्ण ध्रुव *directive);

  /* Handle a %file-prefix directive.  */
  स्थिर शून्य handle_file_उपसर्गः (location ध्रुव *loc,
                                  location ध्रुव *dir_loc,
                                  वर्ण ध्रुव *directive, वर्ण ध्रुव *मानम्);

  /* Handle a %language directive.  */
  स्थिर शून्य handle_language (location ध्रुव *loc, वर्ण ध्रुव *lang);

  /* Handle a %name-prefix directive.  */
  स्थिर शून्य handle_name_उपसर्गः (location ध्रुव *loc,
                                  वर्ण ध्रुव *directive, वर्ण ध्रुव *मानम्);

  /* Handle a %pure-parser directive.  */
  स्थिर शून्य handle_विशुद्धम्_parser (location ध्रुव *loc, वर्ण ध्रुव *directive);

  /* Handle a %require directive.  */
  स्थिर शून्य handle_require (location ध्रुव *loc, वर्ण ध्रुव *संस्करणम्);

  /* Handle a %skeleton directive.  */
  स्थिर शून्य handle_skeleton (location ध्रुव *loc, वर्ण ध्रुव *skel);

  /* Handle a %yacc directive.  */
  स्थिर शून्य handle_yacc (location ध्रुव *loc);

  /* Implementation of yyerror.  */
  स्थिर शून्य gram_error (location ध्रुव *, वर्ण ध्रुव *);

  /* A string that describes a char (e.g., 'a' -> "'a'").  */
  स्थिर वर्ण ध्रुव *char_name (वर्ण);

  /* Add style to semantic values in traces.  */
  स्थिर शून्य tron (FILE *yyo);
  स्थिर शून्य troff (FILE *yyo);

  /* Interpret a quoted string (such as `"Hello, \"World\"\n\""`).
     Manages the memory of the result.  */
  स्थिर वर्ण *unquote (ध्रुव वर्ण *str);

  /* Discard the latest unquoted string.  */
  स्थिर शून्य unquote_free (वर्ण *चरमः_string);
}

%define api.शीर्षकम्.include {"parse-gram.h"}
%define api.उपसर्गः {gram_}
%define api.विशुद्धम् पूर्णम्
%define api.शब्दांशः.raw
%define api.मानम्.प्रकार सङ्घ
%define locations
%define parse.error custom
%define parse.lac पूर्णम्
%define parse.trace
%header
%expect 0
%verbose

%initial-action
{
  /* Bison's grammar can initial empty locations, hence a default
     location is needed. */
  boundary_समुच्चयः (&@$.आरम्भः, grammar_file, 1, 1, 1);
  boundary_समुच्चयः (&@$.अन्तः, grammar_file, 1, 1, 1);
}

%शब्दांशः
  STRING              _("string")
  TSTRING             _("translatable string")

  PERCENT_TOKEN       "%token"
  PERCENT_NTERM       "%nterm"

  PERCENT_TYPE        "%type"
  PERCENT_DESTRUCTOR  "%destructor"
  PERCENT_PRINTER     "%printer"

  PERCENT_वामः        "%left"
  PERCENT_दक्षिणः       "%right"
  PERCENT_NONASSOC    "%nonassoc"
  PERCENT_PRECEDENCE  "%precedence"

  PERCENT_PREC        "%prec"
  PERCENT_DPREC       "%dprec"
  PERCENT_MERGE       "%merge"

  PERCENT_संहिता            "%code"
  PERCENT_DEFAULT_PREC    "%default-prec"
  PERCENT_DEFINE          "%define"
  PERCENT_ERROR_VERBOSE   "%error-verbose"
  PERCENT_EXPECT          "%expect"
  PERCENT_EXPECT_RR       "%expect-rr"
  PERCENT_FILE_उपसर्गः     "%file-prefix"
  PERCENT_FLAG            "%<flag>"
  PERCENT_GLR_PARSER      "%glr-parser"
  PERCENT_शीर्षकम्          "%header"
  PERCENT_INITIAL_ACTION  "%initial-action"
  PERCENT_LANGUAGE        "%language"
  PERCENT_NAME_उपसर्गः     "%name-prefix"
  PERCENT_NO_DEFAULT_PREC "%no-default-prec"
  PERCENT_NO_पङ्क्तयः        "%no-lines"
  PERCENT_NONDETERMINISTIC_PARSER "%nondeterministic-parser"
  PERCENT_OUTPUT          "%output"
  PERCENT_विशुद्धम्_PARSER     "%pure-parser"
  PERCENT_REQUIRE         "%require"
  PERCENT_SKELETON        "%skeleton"
  PERCENT_START           "%start"
  PERCENT_TOKEN_सारणी     "%token-table"
  PERCENT_VERBOSE         "%verbose"
  PERCENT_YACC            "%yacc"

  BRACED_संहिता       "{...}"
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
  चिह्नकम्_2               _("<tag>")
  चिह्नकम्_ANY           "<*>"
  चिह्नकम्_NONE          "<>"

 /* Experimental feature, don't rely on it.  */
%code pre-printer  {tron (yyo);}
%code post-printer {troff (yyo);}

%प्रकार <अचिह्नित वर्ण> CHAR_LITERAL
%printer { fputs (char_name ($$), yyo); } <अचिह्नित वर्ण>

%प्रकार <वर्ण*> "{...}" "%?{...}" "%{...%}" EPILOGUE STRING TSTRING
%printer { fputs ($$, yyo); } <वर्ण*>

%प्रकार <uniqstr>
  BRACKETED_ID ID ID_COLON
  PERCENT_ERROR_VERBOSE PERCENT_FILE_उपसर्गः PERCENT_FLAG PERCENT_NAME_उपसर्गः
  PERCENT_विशुद्धम्_PARSER
  चिह्नकम्_2 चिह्नकम् चिह्नकम्.opt variable
%printer { fputs ($$, yyo); } <uniqstr>
%printer { fprintf (yyo, "[%s]", $$); } BRACKETED_ID
%printer { fprintf (yyo, "%s:", $$); } ID_COLON
%printer { fprintf (yyo, "%%%s", $$); } PERCENT_FLAG
%printer { fprintf (yyo, "<%s>", $$); } चिह्नकम्_2 चिह्नकम्

%शब्दांशः <पूर्णाङ्क> INT_LITERAL _("integer literal")
%printer { fprintf (yyo, "%d", $$); } <पूर्णाङ्क>

%प्रकार <चिह्नम्*> id id_colon string_as_id चिह्नम् token_decl token_decl_for_prec
%printer { fprintf (yyo, "%s", $$ ? $$->चिह्नकम् : "<NULL>"); } <चिह्नम्*>
%printer { fprintf (yyo, "%s:", $$->चिह्नकम्); } id_colon

%प्रकार <assoc> precedence_declarator

%destructor { चिह्नम्_list_free ($$); } <चिह्नम्_list*>
%printer { चिह्नम्_list_syms_print ($$, yyo); } <चिह्नम्_list*>

%प्रकार <named_ref*> named_ref.opt

/*---------.
| %param.  |
`---------*/
%code requires
{
  प्रकारनिर्देश गणना
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
  स्थिर शून्य सङ्कलय_param (param_type प्रकार, वर्ण *decl, location loc);
  स्थिर param_type current_param = param_none;
};
%शब्दांशः <param_type> PERCENT_PARAM "%param";
%printer
{
  विकल्प ($$)
    {
#परिभाषय CASE(अन्तर्_2, बहिर्गमनम्_2)                                           \
      पक्ष param_ ## अन्तर्_2: fputs ("%" #Out, yyo); भङ्ग
      CASE (lex,   lex-param);
      CASE (parse, parse-param);
      CASE (both,  param);
#अपरिभाषय CASE
      पक्ष param_none: aver (false); भङ्ग;
    }
} <param_type>;


                     /*==========\
                     | Grammar.  |
                     \==========*/
%%

निवेशः:
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
      muscle_संहिता_grow (union_आलोकितम् ? "post_prologue" : "pre_prologue",
                        translate_संहिता ($1, @1, true), @1);
      संहिता_scanner_चरमः_string_free ();
    }
| "%<flag>"
    {
      muscle_percent_define_ensure ($1, @1, true);
    }
| "%define" variable मानम्
    {
      muscle_percent_define_निवेशय ($2, @$, $3.विधा, $3.chars,
                                    MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
    }
| "%header" string.opt             { handle_शीर्षकम् ($2); }
| "%error-verbose"                 { handle_error_verbose (&@$, $1); }
| "%expect" INT_LITERAL            { expected_sr_conflicts = $2; }
| "%expect-rr" INT_LITERAL         { expected_rr_conflicts = $2; }
| "%file-prefix" STRING            { handle_file_उपसर्गः (&@$, &@1, $1, $2); }
| "%glr-parser"
    {
      nondeterministic_parser = true;
      glr_parser = true;
    }
| "%initial-action" "{...}"
    {
      muscle_संहिता_grow ("initial_action", translate_संहिता ($2, @2, false), @2);
      संहिता_scanner_चरमः_string_free ();
    }
| "%language" STRING            { handle_language (&@1, $2); }
| "%name-prefix" STRING         { handle_name_उपसर्गः (&@$, $1, $2); }
| "%no-lines"                   { no_पङ्क्तयः_flag = true; }
| "%nondeterministic-parser"    { nondeterministic_parser = true; }
| "%output" STRING              { spec_outfile = unquote ($2); gram_scanner_चरमः_string_free (); }
| "%param" { current_param = $1; } params { current_param = param_none; }
| "%pure-parser"                { handle_विशुद्धम्_parser (&@$, $1); }
| "%require" STRING             { handle_require (&@2, $2); }
| "%skeleton" STRING            { handle_skeleton (&@2, $2); }
| "%token-table"                { token_सारणी_flag = true; }
| "%verbose"                    { report_flag |= report_अवस्थाः; }
| "%yacc"                       { handle_yacc (&@$); }
| error ";"                     { current_class = unknown_sym; yyerrok; }
| /*FIXME: Err?  What is this horror doing here? */ ";"
;

params:
   params "{...}"  { सङ्कलय_param (current_param, $2, @2); }
| "{...}"          { सङ्कलय_param (current_param, $1, @1); }
;


/*----------------------.
| grammar_declaration.  |
`----------------------*/

grammar_declaration:
  चिह्नम्_declaration
| "%start" symbols.1
    {
      grammar_start_symbols_सङ्कलय ($2);
    }
| संहिता_props_type "{...}" generic_symlist
    {
      संहिता_props संहिता;
      संहिता_props_चिह्नम्_action_init (&संहिता, $2, @2);
      संहिता_props_translate_संहिता (&संहिता);
      {
        प्रति (चिह्नम्_list *list = $3; list; list = list->अग्र)
          चिह्नम्_list_संहिता_props_समुच्चयः (list, $1, &संहिता);
        चिह्नम्_list_free ($3);
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
      muscle_संहिता_grow ("percent_code()",
                        translate_संहिता_braceless ($2, @2), @2);
      संहिता_scanner_चरमः_string_free ();
    }
| "%code" ID "{...}"
    {
      muscle_percent_संहिता_grow ($2, @2, translate_संहिता_braceless ($3, @3), @3);
      संहिता_scanner_चरमः_string_free ();
    }
;

%प्रकार <संहिता_props_type> संहिता_props_type;
%printer { fprintf (yyo, "%s", संहिता_props_type_string ($$)); } <संहिता_props_type>;
संहिता_props_type:
  "%destructor"  { $$ = destructor; }
| "%printer"     { $$ = printer; }
;

/*---------.
| %union.  |
`---------*/

%शब्दांशः PERCENT_UNION "%union";

union_name:
  %empty {}
| ID     { muscle_percent_define_निवेशय ("api.value.union.name",
                                         @1, muscle_keyword, $1,
                                         MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE); }
;

grammar_declaration:
  "%union" union_name "{...}"
    {
      union_आलोकितम् = true;
      muscle_संहिता_grow ("union_members", translate_संहिता_braceless ($3, @3), @3);
      संहिता_scanner_चरमः_string_free ();
    }
;


%प्रकार <चिह्नम्_list*> nterm_decls चिह्नम्_decls symbols.1
      token_decls token_decls_for_prec
      token_decl.1 token_decl_for_prec.1;
चिह्नम्_declaration:
  "%nterm" { current_class = nterm_sym; } nterm_decls[syms]
    {
      current_class = unknown_sym;
      चिह्नम्_list_free ($syms);
    }
| "%token" { current_class = token_sym; } token_decls[syms]
    {
      current_class = unknown_sym;
      चिह्नम्_list_free ($syms);
    }
| "%type" { current_class = pct_type_sym; } चिह्नम्_decls[syms]
    {
      current_class = unknown_sym;
      चिह्नम्_list_free ($syms);
    }
| precedence_declarator token_decls_for_prec[syms]
    {
      ++current_prec;
      प्रति (चिह्नम्_list *list = $syms; list; list = list->अग्र)
        चिह्नम्_precedence_समुच्चयः (list->content.sym, current_prec, $1, @1);
      चिह्नम्_list_free ($syms);
    }
;

precedence_declarator:
  "%left"       { $$ = वामः_assoc; }
| "%right"      { $$ = दक्षिणः_assoc; }
| "%nonassoc"   { $$ = non_assoc; }
| "%precedence" { $$ = precedence_assoc; }
;

%प्रकार <वर्ण*> string.opt;
string.opt:
  %empty  { $$ = रिक्त; }
| STRING  { $$ = $1; }
;

चिह्नकम्.opt:
  %empty { $$ = रिक्त; }
| चिह्नकम्_2    { $$ = $1; }
;

%प्रकार <चिह्नम्_list*> generic_symlist generic_symlist_item;
generic_symlist:
  generic_symlist_item
| generic_symlist generic_symlist_item   { $$ = चिह्नम्_list_append ($1, $2); }
;

generic_symlist_item:
  चिह्नम्    { $$ = चिह्नम्_list_sym_new ($1, @1); }
| चिह्नकम्       { $$ = चिह्नम्_list_type_new ($1, @1); }
;

चिह्नकम्:
  चिह्नकम्_2
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
| चिह्नकम्_2 token_decl.1[syms]
    {
      $$ = चिह्नम्_list_type_समुच्चयः ($syms, $चिह्नकम्_2);
    }
| token_decls चिह्नकम्_2 token_decl.1[syms]
    {
      $$ = चिह्नम्_list_append ($1, चिह्नम्_list_type_समुच्चयः ($syms, $चिह्नकम्_2));
    }
;

// One or more symbol declarations for %token or %nterm.
token_decl.1:
  token_decl                { $$ = चिह्नम्_list_sym_new ($1, @1); }
| token_decl.1 token_decl   { $$ = चिह्नम्_list_append ($1, चिह्नम्_list_sym_new ($2, @2)); }

// One symbol declaration for %token or %nterm.
token_decl:
  id पूर्णाङ्क.opt[num] alias
    {
      $$ = $id;
      चिह्नम्_class_समुच्चयः ($id, current_class, @id, true);
      यदि (0 <= $अङ्कः)
        चिह्नम्_संहिता_समुच्चयः ($id, $अङ्कः, @अङ्कः);
      यदि ($alias)
        चिह्नम्_make_alias ($id, $alias, @alias);
    }
;

%प्रकार <पूर्णाङ्क> पूर्णाङ्क.opt;
पूर्णाङ्क.opt:
  %empty  { $$ = -1; }
| INT_LITERAL
;

%प्रकार <चिह्नम्*> alias;
alias:
  %empty         { $$ = रिक्त; }
| string_as_id   { $$ = $1; }
| TSTRING
    {
      $$ = चिह्नम्_get ($1, @1);
      चिह्नम्_class_समुच्चयः ($$, token_sym, @1, false);
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
| चिह्नकम्_2 token_decl_for_prec.1[syms]
    {
      $$ = चिह्नम्_list_type_समुच्चयः ($syms, $चिह्नकम्_2);
    }
| token_decls_for_prec चिह्नकम्_2 token_decl_for_prec.1[syms]
    {
      $$ = चिह्नम्_list_append ($1, चिह्नम्_list_type_समुच्चयः ($syms, $चिह्नकम्_2));
    }
;

// One or more token declarations for precedence declaration.
token_decl_for_prec.1:
  token_decl_for_prec
    { $$ = चिह्नम्_list_sym_new ($1, @1); }
| token_decl_for_prec.1 token_decl_for_prec
    { $$ = चिह्नम्_list_append ($1, चिह्नम्_list_sym_new ($2, @2)); }

// One token declaration for precedence declaration.
token_decl_for_prec:
  id पूर्णाङ्क.opt[num]
    {
      $$ = $id;
      चिह्नम्_class_समुच्चयः ($id, token_sym, @id, false);
      यदि (0 <= $अङ्कः)
        चिह्नम्_संहिता_समुच्चयः ($id, $अङ्कः, @अङ्कः);
    }
| string_as_id
;


/*-----------------------------------.
| symbol_decls (argument of %type).  |
`-----------------------------------*/

// A non empty list of typed symbols (for %type).
चिह्नम्_decls:
  symbols.1[syms]
    {
      $$ = $syms;
    }
| चिह्नकम्_2 symbols.1[syms]
    {
      $$ = चिह्नम्_list_type_समुच्चयः ($syms, $चिह्नकम्_2);
    }
| चिह्नम्_decls चिह्नकम्_2 symbols.1[syms]
    {
      $$ = चिह्नम्_list_append ($1, चिह्नम्_list_type_समुच्चयः ($syms, $चिह्नकम्_2));
    }
;

// One or more symbols.
symbols.1:
  चिह्नम्
    {
      यदि (current_class != unknown_sym)
        चिह्नम्_class_समुच्चयः ($चिह्नम्, current_class, @चिह्नम्, false);
      $$ = चिह्नम्_list_sym_new ($चिह्नम्, @चिह्नम्);
    }
  | symbols.1 चिह्नम्
    {
      यदि (current_class != unknown_sym)
        चिह्नम्_class_समुच्चयः ($चिह्नम्, current_class, @चिह्नम्, false);
      $$ = चिह्नम्_list_append ($1, चिह्नम्_list_sym_new ($चिह्नम्, @चिह्नम्));
    }
;

        /*------------------------------------------.
        | The grammar section: between the two %%.  |
        `------------------------------------------*/

grammar:
  नियमाः_or_grammar_declaration
| grammar नियमाः_or_grammar_declaration
;

/* As a Bison extension, one can use the grammar declarations in the
   body of the grammar.  */
नियमाः_or_grammar_declaration:
  नियमाः
| grammar_declaration ";"
| error ";"
    {
      yyerrok;
    }
;

नियमाः:
  id_colon named_ref.opt { current_lhs ($1, @1, $2); } ":" rhses.1
    {
      /* Free the current lhs. */
      current_lhs (0, @1, 0);
    }
;

rhses.1:
  rhs                { grammar_current_नियमः_अन्तः (@rhs); }
| rhses.1 "|" rhs    { grammar_current_नियमः_अन्तः (@rhs); }
| rhses.1 ";"
;

%शब्दांशः PERCENT_रिक्तानि "%empty";
rhs:
  %empty
    { grammar_current_नियमः_begin (current_lhs_चिह्नम्, current_lhs_loc,
                                  current_lhs_named_ref); }
| rhs चिह्नम् named_ref.opt
    { grammar_current_नियमः_चिह्नम्_append ($2, @2, $3); }
| rhs चिह्नकम्.opt "{...}"[action] named_ref.opt[name]
    { grammar_current_नियमः_action_append ($action, @action, $name, $[tag.opt]); }
| rhs "%?{...}"
    { grammar_current_नियमः_predicate_append ($2, @2); }
| rhs "%empty"
    { grammar_current_नियमः_रिक्तानि_समुच्चयः (@2); }
| rhs "%prec" चिह्नम्
    { grammar_current_नियमः_prec_समुच्चयः ($3, @3); }
| rhs "%dprec" INT_LITERAL
    { grammar_current_नियमः_dprec_समुच्चयः ($3, @3); }
| rhs "%merge" चिह्नकम्_2
    { grammar_current_नियमः_merge_समुच्चयः ($3, @3); }
| rhs "%expect" INT_LITERAL
    { grammar_current_नियमः_expect_sr ($3, @3); }
| rhs "%expect-rr" INT_LITERAL
    { grammar_current_नियमः_expect_rr ($3, @3); }
;

named_ref.opt:
  %empty         { $$ = रिक्त; }
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
  #अन्तर्भाव "muscle-tab.h"
  प्रकारनिर्देश संरचना
  {
    वर्ण ध्रुव *chars;
    muscle_विधा विधा;
  } मानम्_type;
};
%प्रकार <मानम्_type> मानम्;
%printer
{
  विकल्प ($$.विधा)
    {
    पक्ष muscle_संहिता:    fprintf (yyo,  "{%s}",  $$.chars); भङ्ग;
    पक्ष muscle_keyword: fprintf (yyo,   "%s",   $$.chars); भङ्ग;
    पक्ष muscle_string:  fprintf (yyo, "\"%s\"", $$.chars); भङ्ग;
    }
} <मानम्_type>;

मानम्:
  %empty  { $$.विधा = muscle_keyword; $$.chars = ""; }
| ID      { $$.विधा = muscle_keyword; $$.chars = $1; }
| STRING  { $$.विधा = muscle_string;  $$.chars = unquote ($1); gram_scanner_चरमः_string_free ();}
| "{...}" { $$.विधा = muscle_संहिता;    $$.chars = strip_braces ($1); gram_scanner_चरमः_string_free (); }
;


/*--------------.
| Identifiers.  |
`--------------*/

/* Identifiers are returned as uniqstr values by the scanner.
   Depending on their use, we may need to make them genuine symbols.  */

id:
  ID
    { $$ = चिह्नम्_from_uniqstr ($1, @1); }
| CHAR_LITERAL
    {
      ध्रुव वर्ण *var = "api.token.raw";
      यदि (current_class == nterm_sym)
        {
          complain (&@1, complaint,
                    _("character literals cannot be nonterminals"));
          YYERROR;
        }
      यदि (muscle_percent_define_ifdef (var))
        {
          complain (&@1, complaint,
                    _("character literals cannot be used together"
                    " with %s"), var);
          location loc = muscle_percent_define_get_loc (var);
          subcomplain (&loc, complaint, _("definition of %s"), var);
        }
      $$ = चिह्नम्_get (char_name ($1), @1);
      चिह्नम्_class_समुच्चयः ($$, token_sym, @1, false);
      चिह्नम्_संहिता_समुच्चयः ($$, $1, @1);
    }
;

id_colon:
  ID_COLON { $$ = चिह्नम्_from_uniqstr ($1, @1); }
;


चिह्नम्:
  id
| string_as_id
;

/* A string used as an ID.  */
string_as_id:
  STRING
    {
      $$ = चिह्नम्_get ($1, @1);
      चिह्नम्_class_समुच्चयः ($$, token_sym, @1, false);
    }
;

epilogue.opt:
  %empty
| "%%" EPILOGUE
    {
      muscle_संहिता_grow ("epilogue", translate_संहिता ($2, @2, true), @2);
      संहिता_scanner_चरमः_string_free ();
    }
;

%%

पूर्णाङ्क
yyreport_syntax_error (ध्रुव yypcontext_t *ctx)
{
  पूर्णाङ्क res = 0;
  /* Arguments of format: reported tokens (one for the "unexpected",
     one per "expected"). */
  गणना { तर्काः_MAX = 5 };
  ध्रुव वर्ण *argv[ARGS_MAX];
  पूर्णाङ्क argc = 0;
  yysymbol_विधा_t unexpected = yypcontext_token (ctx);
  यदि (unexpected != YYSYMBOL_YYEMPTY)
    {
      argv[argc++] = yysymbol_name (unexpected);
      yysymbol_विधा_t expected[ARGS_MAX - 1];
      पूर्णाङ्क nexpected = yypcontext_expected_tokens (ctx, expected, तर्काः_MAX - 1);
      यदि (nexpected < 0)
        res = nexpected;
      अन्यथा
        प्रति (पूर्णाङ्क i = 0; i < nexpected; ++i)
          argv[argc++] = yysymbol_name (expected[i]);
    }
  syntax_error (*yypcontext_location (ctx), argc, argv);
  प्रत्यागम res;
}


/* Return the location of the left-hand side of a rule whose
   right-hand side is RHS[1] ... RHS[N].  Ignore empty nonterminals in
   the right-hand side, and return an empty location equal to the end
   boundary of RHS[0] if the right-hand side is empty.  */

स्थिर YYLTYPE
lloc_default (YYLTYPE ध्रुव *rhs, पूर्णाङ्क n)
{
  YYLTYPE loc;

  /* SGI MIPSpro 7.4.1m miscompiles "loc.start = loc.end = rhs[n].end;".
     The bug is fixed in 7.4.2m, but play it safe for now.  */
  loc.आरम्भः = rhs[n].अन्तः;
  loc.अन्तः = rhs[n].अन्तः;

  /* Ignore empty nonterminals the start of the right-hand side.
     Do not bother to ignore them at the end of the right-hand side,
     since empty nonterminals have the same end as their predecessors.  */
  प्रति (पूर्णाङ्क i = 1; i <= n; i++)
    यदि (! equal_boundaries (rhs[i].आरम्भः, rhs[i].अन्तः))
      {
        loc.आरम्भः = rhs[i].आरम्भः;
        भङ्ग;
      }

  प्रत्यागम loc;
}

स्थिर
वर्ण *strip_braces (वर्ण *संहिता)
{
  संहिता[वाक्यदैर्घ्यम् (संहिता) - 1] = 0;
  प्रत्यागम संहिता + 1;
}

स्थिर
वर्ण ध्रुव *
translate_संहिता (वर्ण *संहिता, location loc, bool plain)
{
  संहिता_props plain_संहिता;
  यदि (plain)
    संहिता_props_plain_init (&plain_संहिता, संहिता, loc);
  अन्यथा
    संहिता_props_चिह्नम्_action_init (&plain_संहिता, संहिता, loc);
  संहिता_props_translate_संहिता (&plain_संहिता);
  gram_scanner_चरमः_string_free ();
  प्रत्यागम plain_संहिता.संहिता;
}

स्थिर
वर्ण ध्रुव *
translate_संहिता_braceless (वर्ण *संहिता, location loc)
{
  प्रत्यागम translate_संहिता (strip_braces (संहिता), loc, true);
}

स्थिर शून्य
सङ्कलय_param (param_type प्रकार, वर्ण *decl, location loc)
{
  स्थिर वर्ण ध्रुव alphanum[26 + 26 + 1 + 10 + 1] =
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "_"
    "0123456789";

  वर्ण ध्रुव *name_start = रिक्त;
  {
    वर्ण *p;
    /* Stop on last actual character.  */
    प्रति (p = decl; p[1]; p++)
      यदि ((p == decl
           || ! memchr (alphanum, p[-1], परिमाण alphanum - 1))
          && memchr (alphanum, p[0], परिमाण alphanum - 10 - 1))
        name_start = p;

    /* Strip the surrounding '{' and '}', and any blanks just inside
       the braces.  */
    --p;
    यावत् (c_isspace ((अचिह्नित वर्ण) *p))
      --p;
    p[1] = '\0';
    ++decl;
    यावत् (c_isspace ((अचिह्नित वर्ण) *decl))
      ++decl;
  }

  यदि (! name_start)
    complain (&loc, complaint, _("missing identifier in parameter declaration"));
  अन्यथा
    {
      वर्ण *name = xmemdup0 (name_start, strspn (name_start, alphanum));
      यदि (प्रकार & param_lex)
        muscle_pair_list_grow ("lex_param", decl, name);
      यदि (प्रकार & param_parse)
        muscle_pair_list_grow ("parse_param", decl, name);
      मुक्त (name);
    }

  gram_scanner_चरमः_string_free ();
}


स्थिर शून्य
handle_शीर्षकम् (वर्ण ध्रुव *मानम्)
{
  शीर्षकम्_flag = true;
  यदि (मानम्)
    {
      वर्ण *file = unquote (मानम्);
      spec_शीर्षकम्_file = xstrdup (file);
      gram_scanner_चरमः_string_free ();
      unquote_free (file);
    }
}


स्थिर शून्य
handle_error_verbose (location ध्रुव *loc, वर्ण ध्रुव *directive)
{
  bison_directive (loc, directive);
  muscle_percent_define_निवेशय (directive, *loc, muscle_keyword, "",
                                MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
}


स्थिर शून्य
handle_file_उपसर्गः (location ध्रुव *loc,
                    location ध्रुव *dir_loc,
                    वर्ण ध्रुव *directive, वर्ण ध्रुव *मानम्_quoted)
{
  वर्ण *मानम् = unquote (मानम्_quoted);
  bison_directive (loc, directive);
  bool warned = false;

  यदि (location_रिक्तानि (spec_file_उपसर्गः_loc))
    {
      spec_file_उपसर्गः_loc = *loc;
      spec_file_उपसर्गः = मानम्;
    }
  अन्यथा
    {
      duplicate_directive (directive, spec_file_उपसर्गः_loc, *loc);
      warned = true;
    }

  यदि (!warned
      && STRNEQ (directive, "%file-prefix"))
    deprecated_directive (dir_loc, directive, "%file-prefix");
}

स्थिर शून्य
handle_language (location ध्रुव *loc, वर्ण ध्रुव *lang)
{
  language_argmatch (unquote (lang), grammar_prio, *loc);
}


स्थिर शून्य
handle_name_उपसर्गः (location ध्रुव *loc,
                    वर्ण ध्रुव *directive, वर्ण ध्रुव *मानम्_quoted)
{
  वर्ण *मानम् = unquote (मानम्_quoted);
  bison_directive (loc, directive);

  वर्ण buf1[1024];
  size_t len1 = परिमाण (buf1);
  वर्ण *old = asnprintf (buf1, &len1, "%s\"%s\"", directive, मानम्);
  यदि (!old)
    xalloc_die ();

  यदि (location_रिक्तानि (spec_name_उपसर्गः_loc))
    {
      spec_name_उपसर्गः = मानम्;
      spec_name_उपसर्गः_loc = *loc;

      वर्ण buf2[1024];
      size_t len2 = परिमाण (buf2);
      वर्ण *new = asnprintf (buf2, &len2, "%%define api.prefix {%s}", मानम्);
      यदि (!new)
        xalloc_die ();
      deprecated_directive (loc, old, new);
      यदि (new != buf2)
        मुक्त (new);
    }
  अन्यथा
    duplicate_directive (old, spec_file_उपसर्गः_loc, *loc);

  यदि (old != buf1)
    मुक्त (old);
}


स्थिर शून्य
handle_विशुद्धम्_parser (location ध्रुव *loc, वर्ण ध्रुव *directive)
{
  bison_directive (loc, directive);
  deprecated_directive (loc, directive, "%define api.pure");
  muscle_percent_define_निवेशय ("api.pure", *loc, muscle_keyword, "",
                                MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
}


स्थिर शून्य
handle_require (location ध्रुव *loc, वर्ण ध्रुव *संस्करणम्_quoted)
{
  वर्ण *संस्करणम् = unquote (संस्करणम्_quoted);
  required_संस्करणम् = strversion_गन्तव्यम्_int (संस्करणम्);
  यदि (required_संस्करणम् == -1)
    {
      complain (loc, complaint, _("invalid version requirement: %s"),
                संस्करणम्);
      required_संस्करणम् = 0;
    }
  अन्यथा
    {
      ध्रुव वर्ण* package_संस्करणम् =
        0 < strverscmp (api_संस्करणम्, PACKAGE_संस्करणम्)
        ? api_संस्करणम् : PACKAGE_संस्करणम्;
      यदि (0 < strverscmp (संस्करणम्, package_संस्करणम्))
        {
          complain (loc, complaint, _("require bison %s, but have %s"),
                    संस्करणम्, package_संस्करणम्);
          निर्गच्छ (EX_MISMATCH);
        }
    }
  unquote_free (संस्करणम्);
  gram_scanner_चरमः_string_free ();
}

स्थिर शून्य
handle_skeleton (location ध्रुव *loc, वर्ण ध्रुव *skel_quoted)
{
  वर्ण *skel = unquote (skel_quoted);
  वर्ण ध्रुव *skeleton_user = skel;
  यदि (strchr (skeleton_user, '/'))
    {
      size_t dir_दीर्घता = वाक्यदैर्घ्यम् (grammar_file);
      यावत् (dir_दीर्घता && grammar_file[dir_length - 1] != '/')
        --dir_length;
      यावत् (dir_दीर्घता && grammar_file[dir_length - 1] == '/')
        --dir_length;
      वर्ण *skeleton_build =
        xmalloc (dir_दीर्घता + 1 + वाक्यदैर्घ्यम् (skeleton_user) + 1);
      यदि (dir_दीर्घता > 0)
        {
          स्मृतिं_प्रतिलिख (skeleton_build, grammar_file, dir_दीर्घता);
          skeleton_build[dir_length++] = '/';
        }
      वाक्यं_प्रतिलिख (skeleton_build + dir_दीर्घता, skeleton_user);
      skeleton_user = uniqstr_new (skeleton_build);
      मुक्त (skeleton_build);
    }
  skeleton_arg (skeleton_user, grammar_prio, *loc);
}


स्थिर शून्य
handle_yacc (location ध्रुव *loc)
{
  ध्रुव वर्ण *directive = "%yacc";
  bison_directive (loc, directive);
  यदि (location_रिक्तानि (yacc_loc))
    समुच्चयः_yacc (*loc);
  अन्यथा
    duplicate_directive (directive, yacc_loc, *loc);
}


स्थिर शून्य
gram_error (location ध्रुव *loc, वर्ण ध्रुव *सन्देशः_2)
{
  complain (loc, complaint, "%s", सन्देशः_2);
}

स्थिर वर्ण ध्रुव *
char_name (वर्ण c)
{
  यदि (c == '\'')
    प्रत्यागम "'\\''";
  अन्यथा
    {
      वर्ण buf[4];
      buf[0] = '\''; buf[1] = c; buf[2] = '\''; buf[3] = '\0';
      प्रत्यागम quotearg_style (escape_quoting_style, buf);
    }
}

स्थिर शून्य
current_lhs (चिह्नम् *sym, location loc, named_ref *ref)
{
  current_lhs_चिह्नम् = sym;
  current_lhs_loc = loc;
  यदि (sym)
    चिह्नम्_location_as_lhs_समुच्चयः (sym, loc);
  /* In order to simplify memory management, named references for lhs
     are always assigned by deep copy into the current symbol_list
     node.  This is because a single named-ref in the grammar may
     result in several uses when the user factors lhs between several
     rules using "|".  Therefore free the parser's original copy.  */
  मुक्त (current_lhs_named_ref);
  current_lhs_named_ref = ref;
}

स्थिर शून्य tron (FILE *yyo)
{
  begin_use_class ("value", yyo);
}

स्थिर शून्य troff (FILE *yyo)
{
  अन्तः_use_class ("value", yyo);
}


/*----------.
| Unquote.  |
`----------*/

संरचना obstack obstack_for_unquote;

शून्य
parser_init (शून्य)
{
  obstack_init (&obstack_for_unquote);
}

शून्य
parser_free (शून्य)
{
  obstack_free (&obstack_for_unquote, 0);
}

स्थिर शून्य
unquote_free (वर्ण *चरमः_string)
{
  obstack_free (&obstack_for_unquote, चरमः_string);
}

स्थिर वर्ण *
unquote (ध्रुव वर्ण *cp)
{
#परिभाषय GROW(Char)                              \
  obstack_1grow (&obstack_for_unquote, Char);
  प्रति (++cp; *cp && *cp != '"'; ++cp)
    विकल्प (*cp)
      {
      पक्ष '"':
        भङ्ग;
      पक्ष '\\':
        ++cp;
        विकल्प (*cp)
          {
          पक्ष '0': पक्ष '1': पक्ष '2': पक्ष '3': पक्ष '4':
          पक्ष '5': पक्ष '6': पक्ष '7': पक्ष '8': पक्ष '9':
            {
              पूर्णाङ्क c = cp[0] - '0';
              यदि (c_isdigit (cp[1]))
                {
                  ++cp;
                  c = c * 8 + cp[0] - '0';
                }
              यदि (c_isdigit (cp[1]))
                {
                  ++cp;
                  c = c * 8 + cp[0] - '0';
                }
              GROW (c);
            }
            भङ्ग;

          पक्ष 'a': GROW ('\a'); भङ्ग;
          पक्ष 'b': GROW ('\b'); भङ्ग;
          पक्ष 'f': GROW ('\f'); भङ्ग;
          पक्ष 'n': GROW ('\n'); भङ्ग;
          पक्ष 'r': GROW ('\r'); भङ्ग;
          पक्ष 't': GROW ('\t'); भङ्ग;
          पक्ष 'v': GROW ('\v'); भङ्ग;

          पक्ष 'x':
            {
              पूर्णाङ्क c = 0;
              यावत् (c_isxdigit (cp[1]))
                {
                  ++cp;
                  c = (c * 16 + (c_isdigit (cp[0]) ? cp[0] - '0'
                                 : c_isupper (cp[0]) ? cp[0] - 'A'
                                 : cp[0] - '0'));
                }
              GROW (c);
              भङ्ग;
            }
          }
        भङ्ग;

      स्वभाव:
        GROW (*cp);
        भङ्ग;
      }
  प्रतिज्ञा (*cp == '"');
  ++cp;
  प्रतिज्ञा (*cp == '\0');
#अपरिभाषय GROW
  प्रत्यागम obstack_finish0 (&obstack_for_unquote);
}
