/* Bison Grammar Scanner                             -*- C -*-

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

%option debug nodefault noinput noyywrap never-interactive
%option مختاړی="gram_" outfile="lex.yy.c"

%{
#ضمن <c-ctype.h>
#ضمن <errno.h>
#ضمن <mbswidth.h>
#ضمن <quote.h>
#ضمن <quotearg.h>

#ضمن "src/complain.h"
#ضمن "src/files.h"
#ضمن "src/getargs.h"
#ضمن "src/gram.h"
#ضمن "src/reader.h"
#ضمن "src/scan-gram.h"
#ضمن "src/uniqstr.h"

#تعریف_کړه FLEX_مختاړی(Id) gram_ ## Id
#ضمن "src/flex-scanner.h"

/* Work around a bug in flex 2.5.31.  See Debian bug 333231
   <https://bugs.debian.org/333231>.  */
#تعریف_لغوه gram_wrap
#تعریف_کړه gram_wrap() 1

#تعریف_کړه YY_DECL GRAM_LEX_DECL

/* Location of scanner cursor.  */
ساكن boundary scanner_cursor;

#تعریف_کړه YY_USER_ACTION  location_compute (loc, &scanner_cursor, yytext, yyleng);

/* Report that yytext is an extension, and evaluate to its token kind.  */
#تعریف_کړه BISON_DIRECTIVE(Directive)                              \
  (bison_directive (loc, yytext), PERCENT_ ## Directive)

#تعریف_کړه RETURN_PERCENT_PARAM(ارزښت_4)             \
  RETURN_ارزښت(PERCENT_PARAM, param_ ## ارزښت_4)

#تعریف_کړه RETURN_PERCENT_FLAG(ارزښت_4)                      \
  RETURN_ارزښت(PERCENT_FLAG, uniqstr_new (ارزښت_4))

#تعریف_کړه RETURN_ارزښت(Token, ارزښت_4)              \
  افعل {                                          \
    val->Token = ارزښت_4;                         \
    أرجع Token;                               \
  } طالما (0)

#تعریف_کړه ROLLBACK_CURRENT_TOKEN                                  \
  افعل {                                                          \
    scanner_cursor.ستنه -= mbsnwidth (yytext, yyleng, 0);     \
    scanner_cursor.byte -= yyleng;                              \
    yyless (0);                                                 \
  } طالما (0)

#تعریف_کړه DEPRECATED_DIRECTIVE(پیغام_3)                               \
  افعل {                                                          \
    deprecated_directive (loc, yytext, پیغام_3);                    \
    scanner_cursor.ستنه -= mbsnwidth (پیغام_3, مزي_اوږدوالی (پیغام_3), 0);  \
    scanner_cursor.byte -= مزي_اوږدوالی (پیغام_3);                        \
    لكل (size_t i = مزي_اوږدوالی (پیغام_3); i != 0; --i)                  \
      unput (پیغام_3[i - 1]);                                       \
  } طالما (0)


#تعریف_کړه STRING_GROW_ESCAPE(Char)                                \
  افعل {                                                          \
    verify (UCHAR_MAX < ULONG_MAX);                             \
    اوږد c = Char;                                              \
    bool valid = 0 < c && c <= UCHAR_MAX;                       \
    إذا (!valid)                                                 \
      complain (loc, complaint,                                 \
                _("invalid number after \\-escape: %s"),        \
                yytext + 1);                                    \
    إذا (YY_پیل == SC_ESCAPED_CHARACTER)                       \
      STRING_1GROW (valid ? c : '?');                           \
    وإلا                                                        \
      STRING_GROW ();                                           \
  } طالما (0)


/* The current file name.  Might change with #line.  */
ساكن uniqstr current_file = عدم;

/* A string representing the most recently saved token.  */
ساكن حرف *وروستی_string = عدم;

/* Bracketed identifier. */
ساكن uniqstr bracketed_id_str = عدم;
ساكن location bracketed_id_loc;
ساكن boundary bracketed_id_پیل;
ساكن صحيح bracketed_id_context_state = 0;

فراغ
gram_scanner_وروستی_string_free (فراغ)
{
  STRING_FREE ();
}

ساكن فراغ handle_syncline (حرف *, location);
ساكن صحيح لټون_integer (حرف ثابت *p, صحيح بنسټ, location loc);
ساكن صحيح convert_ucn_منزل_byte (حرف ثابت *hex_متن);
ساكن فراغ unexpected_eof (boundary, حرف ثابت *);
ساكن فراغ unexpected_newline (boundary, حرف ثابت *);

%}
 /* A C-like comment in directives/rules. */
%x SC_YACC_COMMENT
 /* Characters and strings in directives/rules. */
%x SC_ESCAPED_CHARACTER SC_ESCAPED_STRING SC_ESCAPED_TSTRING
 /* A identifier was just read in directives/rules.  Special state
    to capture the sequence 'identifier :'. */
%x SC_AFTER_IDENTIFIER

 /* POSIX says that a tag must be both an id and a C union member, but
    historically almost any character is allowed in a tag.  We
    disallow NUL, as this simplifies our implementation.  We match
    angle brackets in nested pairs: several languages use them for
    generics/template types.  */
%x SC_نښان

 /* Four types of user code:
    - prologue (code between '%{' '%}' in the first section, before %%);
    - actions, printers, union, etc, (between braced in the middle section);
    - epilogue (everything after the second %%).
    - predicate (code between '%?{' and '{' in middle section); */
%x SC_PROLOGUE SC_BRACED_کوډ SC_EPILOGUE SC_PREDICATE
 /* C and C++ comments in code. */
%x SC_COMMENT SC_کرښه_COMMENT
 /* Strings and characters in code. */
%x SC_STRING SC_CHARACTER
 /* Bracketed identifiers support. */
%x SC_BRACKETED_ID SC_RETURN_BRACKETED_ID

letter    [.abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_]
id        {letter}({letter}|[-0-9])*
صحيح       [0-9]+
xint      0[xX][0-9abcdefABCDEF]+

eol       \n|\r\n

 /* UTF-8 Encoded Unicode Code Point, from Flex's documentation. */
mbchar    [\x09\x0A\x0D\x20-\x7E]|[\xC2-\xDF][\x80-\xBF]|\xE0[\xA0-\xBF][\x80-\xBF]|[\xE1-\xEC\xEE\xEF]([\x80-\xBF]{2})|\xED[\x80-\x9F][\x80-\xBF]|\xF0[\x90-\xBF]([\x80-\xBF]{2})|[\xF1-\xF3]([\x80-\xBF]{3})|\xF4[\x80-\x8F]([\x80-\xBF]{2})

/* Zero or more instances of backslash-newline.  Following GCC, allow
   white space between the backslash and the newline.  */
splice   (\\[ \f\t\v]*{eol})*

/* An equal sign, with optional leading whitespaces. This is used in some
   deprecated constructs. */
sp       [[:space:]]*
eqopt    ({sp}=)?

%%
%{
  /* Nesting level.  Either for nested braces, or nested angle brackets
     (but not mixed).  */
  صحيح nesting PACIFY_CC (= 0);

  /* Parent context state, when applicable.  */
  صحيح context_state PACIFY_CC (= 0);

  /* Location of most recent identifier, when applicable.  */
  location id_loc PACIFY_CC (= تشه_loc);

  /* Where containing code started, when applicable.  Its initial
     value is relevant only when yylex is invoked in the SC_EPILOGUE
     start condition.  */
  boundary کوډ_پیل = scanner_cursor;

  /* Where containing comment or string or character literal started,
     when applicable.  */
  boundary token_پیل PACIFY_CC (= scanner_cursor);

  /* We cannot trust YY_USER_INIT, whose semantics changes over time
     (it moved in Flex 2.5.38).  */
  ساكن bool لومړی = true;
  إذا (لومړی)
    {
      scanner_cursor = loc->پیل;
      لومړی = false;
    }
%}


  /*-----------------------.
  | Scanning white space.  |
  `-----------------------*/

<INITIAL,SC_AFTER_IDENTIFIER,SC_BRACKETED_ID,SC_RETURN_BRACKETED_ID>
{
  /* Comments and white space.  */
  "," {
     complain (loc, Wother, _("stray ',' treated as white space"));
  }
  [ \f\t\v\r]|{eol}  |
  "//".*       استمر;
  "/*" {
    token_پیل = loc->پیل;
    context_state = YY_پیل;
    BEGIN SC_YACC_COMMENT;
  }

  ^"#line "{صحيح}(" \"".*"\"")?{eol} {
    handle_syncline (yytext + حجم "#line " - 1, *loc);
  }
}


  /*----------------------------.
  | Scanning Bison directives.  |
  `----------------------------*/

  /* For directives that are also command line options, the regex must be
        "%..."
     after "[-_]"s are removed, and the directive must match the --long
     option name, with a single string argument.  Otherwise, add exceptions
     to ../build-aux/cross-options.pl.  */

<INITIAL>
{
  "%binary"                         أرجع BISON_DIRECTIVE (NONASSOC);
  "%code"                           أرجع BISON_DIRECTIVE (کوډ_3);
  "%debug"                          RETURN_PERCENT_FLAG ("parse.trace");
  "%default-prec"                   أرجع BISON_DIRECTIVE (DEFAULT_PREC);
  "%define"                         أرجع BISON_DIRECTIVE (DEFINE);
  "%defines"                        أرجع BISON_DIRECTIVE (سرلیک_2); // Deprecated in 3.8.
  "%destructor"                     أرجع BISON_DIRECTIVE (DESTRUCTOR);
  "%dprec"                          أرجع BISON_DIRECTIVE (DPREC);
  "%empty"                          أرجع BISON_DIRECTIVE (تشه_2);
  "%expect"                         أرجع BISON_DIRECTIVE (EXPECT);
  "%expect-rr"                      أرجع BISON_DIRECTIVE (EXPECT_RR);
  "%file-prefix"                    RETURN_ارزښت (PERCENT_FILE_مختاړی, uniqstr_new (yytext));
  "%glr-parser"                     أرجع BISON_DIRECTIVE (GLR_PARSER);
  "%header"                         أرجع BISON_DIRECTIVE (سرلیک_2);
  "%initial-action"                 أرجع BISON_DIRECTIVE (INITIAL_ACTION);
  "%language"                       أرجع BISON_DIRECTIVE (LANGUAGE);
  "%left"                           أرجع PERCENT_کیڼ;
  "%lex-param"                      RETURN_PERCENT_PARAM (lex);
  "%locations"                      RETURN_PERCENT_FLAG ("locations");
  "%merge"                          أرجع BISON_DIRECTIVE (یوځای);
  "%no-default-prec"                أرجع BISON_DIRECTIVE (NO_DEFAULT_PREC);
  "%no-lines"                       أرجع BISON_DIRECTIVE (NO_کرښې);
  "%nonassoc"                       أرجع PERCENT_NONASSOC;
  "%nondeterministic-parser"        أرجع BISON_DIRECTIVE (NONDETERMINISTIC_PARSER);
  "%nterm"                          أرجع BISON_DIRECTIVE (NTERM);
  "%output"                         أرجع BISON_DIRECTIVE (OUTPUT);
  "%param"                          RETURN_PERCENT_PARAM (both);
  "%parse-param"                    RETURN_PERCENT_PARAM (parse);
  "%prec"                           أرجع PERCENT_PREC;
  "%precedence"                     أرجع BISON_DIRECTIVE (PRECEDENCE);
  "%printer"                        أرجع BISON_DIRECTIVE (PRINTER);
  "%require"                        أرجع BISON_DIRECTIVE (REQUIRE);
  "%right"                          أرجع PERCENT_ښی;
  "%skeleton"                       أرجع BISON_DIRECTIVE (SKELETON);
  "%start"                          أرجع PERCENT_پیل;
  "%term"                           أرجع BISON_DIRECTIVE (TOKEN);
  "%token"                          أرجع PERCENT_TOKEN;
  "%token-table"                    أرجع BISON_DIRECTIVE (TOKEN_جدول);
  "%type"                           أرجع PERCENT_TYPE;
  "%union"                          أرجع PERCENT_UNION;
  "%verbose"                        أرجع BISON_DIRECTIVE (VERBOSE);
  "%yacc"                           أرجع PERCENT_YACC;

  /* Deprecated since Bison 2.3b (2008-05-27), but the warning is
     issued only since Bison 3.4. */
  "%pure"[-_]"parser"               RETURN_ارزښت (PERCENT_خالص_PARSER, uniqstr_new (yytext));

  /* Deprecated since Bison 3.0 (2013-07-25), but the warning is
     issued only since Bison 3.3. */
  "%error-verbose"                  RETURN_ارزښت (PERCENT_ERROR_VERBOSE, uniqstr_new (yytext));

  /* Deprecated since Bison 2.6 (2012-07-19), but the warning is
     issued only since Bison 3.3. */
  "%name"[-_]"prefix"{eqopt}{sp}    RETURN_ارزښت (PERCENT_NAME_مختاړی, uniqstr_new (yytext));

  /* Deprecated since Bison 2.7.90, 2012. */
  "%default"[-_]"prec"              DEPRECATED_DIRECTIVE ("%default-prec");
  "%error"[-_]"verbose"             RETURN_ارزښت (PERCENT_ERROR_VERBOSE, uniqstr_new (yytext));
  "%expect"[-_]"rr"                 DEPRECATED_DIRECTIVE ("%expect-rr");
  "%file-prefix"{eqopt}             RETURN_ارزښت (PERCENT_FILE_مختاړی, uniqstr_new (yytext));
  "%fixed"[-_]"output"[-_]"files"   DEPRECATED_DIRECTIVE ("%output \"y.tab.c\"");
  "%no"[-_]"default"[-_]"prec"      DEPRECATED_DIRECTIVE ("%no-default-prec");
  "%no"[-_]"lines"                  DEPRECATED_DIRECTIVE ("%no-lines");
  "%output"{eqopt}                  DEPRECATED_DIRECTIVE ("%output");
  "%token"[-_]"table"               DEPRECATED_DIRECTIVE ("%token-table");

  "%"{id} {
    complain (loc, complaint, _("invalid directive: %s"), quote (yytext));
    أرجع GRAM_error;
  }

  ":"                     أرجع COLON;
  "="                     أرجع EQUAL;
  "|"                     أرجع PIPE;
  ";"                     أرجع SEMICOLON;

  {id} {
    val->ID = uniqstr_new (yytext);
    id_loc = *loc;
    bracketed_id_str = عدم;
    BEGIN SC_AFTER_IDENTIFIER;
  }

  {صحيح}      RETURN_ارزښت (INT_LITERAL, لټون_integer (yytext, 10, *loc));
  {xint}     RETURN_ارزښت (INT_LITERAL, لټون_integer (yytext, 16, *loc));

  /* Identifiers may not start with a digit.  Yet, don't silently
     accept "1FOO" as "1 FOO".  */
  {صحيح}{id} {
    complain (loc, complaint, _("invalid identifier: %s"), quote (yytext));
    أرجع GRAM_error;
  }

  /* Characters.  */
  "'"         token_پیل = loc->پیل; BEGIN SC_ESCAPED_CHARACTER;

  /* Strings. */
  "\""        token_پیل = loc->پیل; STRING_1GROW ('"'); BEGIN SC_ESCAPED_STRING;
  "_(\""      token_پیل = loc->پیل; STRING_1GROW ('"'); BEGIN SC_ESCAPED_TSTRING;

  /* Prologue. */
  "%{"        کوډ_پیل = loc->پیل; BEGIN SC_PROLOGUE;

  /* Code in between braces.  */
  "{" {
    STRING_GROW ();
    nesting = 0;
    کوډ_پیل = loc->پیل;
    BEGIN SC_BRACED_کوډ;
  }

  /* Semantic predicate. */
  "%?"([ \f\t\v]|{eol})*"{" {
    nesting = 0;
    کوډ_پیل = loc->پیل;
    BEGIN SC_PREDICATE;
  }

  /* A type. */
  "<*>"       أرجع نښان_ANY;
  "<>"        أرجع نښان_NONE;
  "<"         {
    nesting = 0;
    token_پیل = loc->پیل;
    BEGIN SC_نښان;
  }

  "%%" {
    ساكن صحيح percent_percent_شمېر;
    إذا (++percent_percent_شمېر == 2)
      BEGIN SC_EPILOGUE;
    أرجع PERCENT_PERCENT;
  }

  "[" {
    bracketed_id_str = عدم;
    bracketed_id_پیل = loc->پیل;
    bracketed_id_context_state = YY_پیل;
    BEGIN SC_BRACKETED_ID;
  }

  [^\[%A-Za-z0-9_<>{}""''*;|=/, \f\r\n\t\v]+|. {
    complain (loc, complaint, "%s: %s",
              ngettext ("invalid character", "invalid characters", yyleng),
              quote_mem (yytext, yyleng));
    أرجع GRAM_error;
  }

  <<EOF>> {
    loc->پیل = loc->پای_ټکی = scanner_cursor;
    yyterminate ();
  }
}


  /*--------------------------------------------------------------.
  | Supporting \0 complexifies our implementation for no expected |
  | added value.                                                  |
  `--------------------------------------------------------------*/

<SC_ESCAPED_CHARACTER,SC_ESCAPED_STRING,SC_ESCAPED_TSTRING,SC_نښان>
{
  \0         {
    complain (loc, complaint, _("invalid null character"));
    STRING_پای ();
    STRING_FREE ();
    أرجع GRAM_error;
  }
}


  /*-----------------------------------------------------------------.
  | Scanning after an identifier, checking whether a colon is next.  |
  `-----------------------------------------------------------------*/

<SC_AFTER_IDENTIFIER>
{
  "[" {
    إذا (bracketed_id_str)
      {
        ROLLBACK_CURRENT_TOKEN;
        BEGIN SC_RETURN_BRACKETED_ID;
        *loc = id_loc;
        أرجع ID;
      }
    وإلا
      {
        bracketed_id_پیل = loc->پیل;
        bracketed_id_context_state = YY_پیل;
        BEGIN SC_BRACKETED_ID;
      }
  }
  ":" {
    ROLLBACK_CURRENT_TOKEN;
    BEGIN (bracketed_id_str ? SC_RETURN_BRACKETED_ID : INITIAL);
    *loc = id_loc;
    أرجع ID_COLON;
  }
  . {
    ROLLBACK_CURRENT_TOKEN;
    BEGIN (bracketed_id_str ? SC_RETURN_BRACKETED_ID : INITIAL);
    *loc = id_loc;
    أرجع ID;
  }
  <<EOF>> {
    BEGIN (bracketed_id_str ? SC_RETURN_BRACKETED_ID : INITIAL);
    *loc = id_loc;
    أرجع ID;
  }
}

  /*--------------------------------.
  | Scanning bracketed identifiers. |
  `--------------------------------*/

<SC_BRACKETED_ID>
{
  {id} {
    إذا (bracketed_id_str)
      {
        complain (loc, complaint,
                  _("unexpected identifier in bracketed name: %s"),
                  quote (yytext));
        أرجع GRAM_error;
      }
    وإلا
      {
        bracketed_id_str = uniqstr_new (yytext);
        bracketed_id_loc = *loc;
      }
  }
  "]" {
    BEGIN bracketed_id_context_state;
    إذا (bracketed_id_str)
      {
        إذا (INITIAL == bracketed_id_context_state)
          {
            val->BRACKETED_ID = bracketed_id_str;
            bracketed_id_str = 0;
            *loc = bracketed_id_loc;
            أرجع BRACKETED_ID;
          }
      }
    وإلا
      {
        complain (loc, complaint, _("an identifier expected"));
        أرجع GRAM_error;
      }
  }

  [^\].A-Za-z0-9_/ \f\r\n\t\v]+|. {
    complain (loc, complaint, "%s: %s",
              ngettext ("invalid character in bracketed name",
                        "invalid characters in bracketed name", yyleng),
              quote_mem (yytext, yyleng));
    أرجع GRAM_error;
  }

  <<EOF>> {
    BEGIN bracketed_id_context_state;
    unexpected_eof (bracketed_id_پیل, "]");
  }
}

<SC_RETURN_BRACKETED_ID>
{
  . {
    ROLLBACK_CURRENT_TOKEN;
    val->BRACKETED_ID = bracketed_id_str;
    bracketed_id_str = 0;
    *loc = bracketed_id_loc;
    BEGIN INITIAL;
    أرجع BRACKETED_ID;
  }
}


  /*---------------------------------------------------------------.
  | Scanning a Yacc comment.  The initial '/ *' is already eaten.  |
  `---------------------------------------------------------------*/

<SC_YACC_COMMENT>
{
  "*/"     BEGIN context_state;
  .|{eol}  استمر;
  <<EOF>>  unexpected_eof (token_پیل, "*/"); BEGIN context_state;
}


  /*------------------------------------------------------------.
  | Scanning a C comment.  The initial '/ *' is already eaten.  |
  `------------------------------------------------------------*/

<SC_COMMENT>
{
  "*"{splice}"/"  STRING_GROW (); BEGIN context_state;
  <<EOF>>         unexpected_eof (token_پیل, "*/"); BEGIN context_state;
}


  /*--------------------------------------------------------------.
  | Scanning a line comment.  The initial '//' is already eaten.  |
  `--------------------------------------------------------------*/

<SC_کرښه_COMMENT>
{
  {eol}          STRING_GROW (); BEGIN context_state;
  {splice}       STRING_GROW ();
  <<EOF>>        BEGIN context_state;
}


  /*------------------------------------------------.
  | Scanning a Bison string, including its escapes. |
  | The initial quote is already eaten.             |
  `------------------------------------------------*/

<SC_ESCAPED_STRING>
{
  "\"" {
    STRING_1GROW ('"');
    STRING_پای ();
    BEGIN INITIAL;
    loc->پیل = token_پیل;
    complain (loc, Wyacc,
              _("POSIX Yacc does not support string literals"));
    RETURN_ارزښت (STRING, وروستی_string);
  }
  <<EOF>>   unexpected_eof (token_پیل, "\"");
  "\n"      unexpected_newline (token_پیل, "\"");
}

<SC_ESCAPED_TSTRING>
{
  "\")" {
    STRING_1GROW ('"');
    STRING_پای ();
    BEGIN INITIAL;
    loc->پیل = token_پیل;
    complain (loc, Wyacc,
              _("POSIX Yacc does not support string literals"));
    RETURN_ارزښت (TSTRING, وروستی_string);
  }
  <<EOF>>   unexpected_eof (token_پیل, "\")");
  "\n"      unexpected_newline (token_پیل, "\")");
}



  /*----------------------------------------------------------.
  | Scanning a Bison character literal, decoding its escapes. |
  | The initial quote is already eaten.                       |
  `----------------------------------------------------------*/

<SC_ESCAPED_CHARACTER>
{
  "'" {
    STRING_پای ();
    BEGIN INITIAL;
    loc->پیل = token_پیل;

    إذا (وروستی_string[0] == '\0')
      {
        complain (loc, complaint, _("empty character literal"));
        STRING_FREE ();
        أرجع GRAM_error;
      }
    وإلا إذا (وروستی_string[1] != '\0')
      {
        complain (loc, complaint, _("extra characters in character literal"));
        STRING_FREE ();
        أرجع GRAM_error;
      }
    وإلا
      {
        val->CHAR_LITERAL = وروستی_string[0];
        STRING_FREE ();
        أرجع CHAR_LITERAL;
      }
  }
  {eol}     unexpected_newline (token_پیل, "'");
  <<EOF>>   unexpected_eof (token_پیل, "'");
}



  /*--------------------------------------------------------------.
  | Scanning a tag.  The initial angle bracket is already eaten.  |
  `--------------------------------------------------------------*/

<SC_نښان>
{
  ">" {
    --nesting;
    إذا (nesting < 0)
      {
        STRING_پای ();
        loc->پیل = token_پیل;
        val->نښان_2 = uniqstr_new (وروستی_string);
        STRING_FREE ();
        BEGIN INITIAL;
        أرجع نښان_2;
      }
    STRING_GROW ();
  }

  ([^<>]|->)+ STRING_GROW ();
  "<"+   STRING_GROW (); nesting += yyleng;

  <<EOF>>   unexpected_eof (token_پیل, ">");
}

  /*----------------------------.
  | Decode escaped characters.  |
  `----------------------------*/

<SC_ESCAPED_CHARACTER,SC_ESCAPED_STRING,SC_ESCAPED_TSTRING>
{
  \\[0-7]{1,3} {
    STRING_GROW_ESCAPE (strtol (yytext + 1, عدم, 8));
  }

  \\x[0-9abcdefABCDEF]+ {
    STRING_GROW_ESCAPE (strtol (yytext + 2, عدم, 16));
  }

  \\a   STRING_GROW_ESCAPE ('\a');
  \\b   STRING_GROW_ESCAPE ('\b');
  \\f   STRING_GROW_ESCAPE ('\f');
  \\n   STRING_GROW_ESCAPE ('\n');
  \\r   STRING_GROW_ESCAPE ('\r');
  \\t   STRING_GROW_ESCAPE ('\t');
  \\v   STRING_GROW_ESCAPE ('\v');

  /* \\[\"\'?\\] would be shorter, but it confuses xgettext.  */
  \\("\""|"'"|"?"|"\\")  STRING_GROW_ESCAPE (yytext[1]);

  \\(u|U[0-9abcdefABCDEF]{4})[0-9abcdefABCDEF]{4} {
    STRING_GROW_ESCAPE (convert_ucn_منزل_byte (yytext));
  }

  \\(.|{eol})      {
    حرف ثابت *p = yytext + 1;
    /* Quote only if escaping won't make the character visible.  */
    إذا (c_isspace ((بینښه حرف) *p) && c_isprint ((بینښه حرف) *p))
      p = quote (p);
    وإلا
      p = quotearg_style_mem (escape_quoting_style, p, 1);
    complain (loc, complaint, _("invalid character after \\-escape: %s"),
              p);
    STRING_1GROW ('?');
  }

  "\\"             {
    // None of the other rules matched: the last character of this
    // file is "\".  But Flex does not support "\\<<EOF>>".
    unexpected_eof (token_پیل,
                    YY_پیل == SC_ESCAPED_CHARACTER ? "?'"
                    : YY_پیل == SC_ESCAPED_STRING ? "?\""
                    : "?\")");
  }
}

  /*--------------------------------------------.
  | Scanning user-code characters and strings.  |
  `--------------------------------------------*/

<SC_CHARACTER,SC_STRING>
{
  {splice}|\\{splice}[^\n\[\]]  STRING_GROW ();
}

<SC_CHARACTER>
{
  "'"           STRING_GROW (); BEGIN context_state;
  {eol}         unexpected_newline (token_پیل, "'");
  <<EOF>>       unexpected_eof (token_پیل, "'");
}

<SC_STRING>
{
  "\""          STRING_GROW (); BEGIN context_state;
  {eol}         unexpected_newline (token_پیل, "\"");
  <<EOF>>       unexpected_eof (token_پیل, "\"");
}


  /*---------------------------------------------------.
  | Strings, comments etc. can be found in user code.  |
  `---------------------------------------------------*/

<SC_BRACED_کوډ,SC_PROLOGUE,SC_EPILOGUE,SC_PREDICATE>
{
  "'" {
    STRING_GROW ();
    context_state = YY_پیل;
    token_پیل = loc->پیل;
    BEGIN SC_CHARACTER;
  }
  "\"" {
    STRING_GROW ();
    context_state = YY_پیل;
    token_پیل = loc->پیل;
    BEGIN SC_STRING;
  }
  "/"{splice}"*" {
    STRING_GROW ();
    context_state = YY_پیل;
    token_پیل = loc->پیل;
    BEGIN SC_COMMENT;
  }
  "/"{splice}"/" {
    STRING_GROW ();
    context_state = YY_پیل;
    BEGIN SC_کرښه_COMMENT;
  }
}



  /*-----------------------------------------------------------.
  | Scanning some code in braces (actions, predicates). The    |
  | initial "{" is already eaten.                              |
  `-----------------------------------------------------------*/

<SC_BRACED_کوډ,SC_PREDICATE>
{
  "{"|"<"{splice}"%"  STRING_GROW (); nesting++;
  "%"{splice}">"      STRING_GROW (); nesting--;

  /* Tokenize '<<%' correctly (as '<<' '%') rather than incorrectly
     (as '<' '<%').  */
  "<"{splice}"<"  STRING_GROW ();

  <<EOF>>   unexpected_eof (کوډ_پیل, "}");
}

<SC_BRACED_کوډ>
{
  "}" {
    STRING_1GROW ('}');

    --nesting;
    إذا (nesting < 0)
      {
        STRING_پای ();
        loc->پیل = کوډ_پیل;
        BEGIN INITIAL;
        RETURN_ارزښت (BRACED_کوډ, وروستی_string);
      }
  }
}

<SC_PREDICATE>
{
  "}" {
    --nesting;
    إذا (nesting < 0)
      {
        STRING_پای ();
        loc->پیل = کوډ_پیل;
        BEGIN INITIAL;
        RETURN_ارزښت (BRACED_PREDICATE, وروستی_string);
      }
    وإلا
      STRING_1GROW ('}');
  }
}

  /*--------------------------------------------------------------.
  | Scanning some prologue: from "%{" (already scanned) to "%}".  |
  `--------------------------------------------------------------*/

<SC_PROLOGUE>
{
  "%}" {
    STRING_پای ();
    loc->پیل = کوډ_پیل;
    BEGIN INITIAL;
    RETURN_ارزښت (PROLOGUE, وروستی_string);
  }

  <<EOF>>   unexpected_eof (کوډ_پیل, "%}");
}


  /*---------------------------------------------------------------.
  | Scanning the epilogue (everything after the second "%%", which |
  | has already been eaten).                                       |
  `---------------------------------------------------------------*/

<SC_EPILOGUE>
{
  <<EOF>> {
    STRING_پای ();
    loc->پیل = کوډ_پیل;
    BEGIN INITIAL;
    RETURN_ارزښت (EPILOGUE, وروستی_string);
  }
}


  /*-----------------------------------------------------.
  | By default, grow the string obstack with the input.  |
  `-----------------------------------------------------*/

<SC_COMMENT,SC_کرښه_COMMENT,SC_BRACED_کوډ,SC_PREDICATE,SC_PROLOGUE,SC_EPILOGUE,SC_STRING,SC_CHARACTER,SC_ESCAPED_CHARACTER,SC_ESCAPED_STRING,SC_ESCAPED_TSTRING>
{
  /* Accept multibyte characters in one block instead of byte after
     byte, so that add_column_width and mbsnwidth can compute correct
     screen width.

     Add a fallthrough "|." so that non UTF-8 input is still accepted
     and does not jam the scanner.  */
  {mbchar}|.   STRING_GROW ();
}

%%


/*------------------------------------------------------.
| Scan NUMBER for a base-BASE integer at location LOC.  |
`------------------------------------------------------*/

ساكن صحيح
لټون_integer (حرف ثابت *شمېره, صحيح بنسټ, location loc)
{
  verify (لوی_عدد < ULONG_MAX);
  إذا (بنسټ == 16)
    complain (&loc, Wyacc,
              _("POSIX Yacc does not support hexadecimal literals"));

  errno = 0;
  اوږد عدد = strtol (شمېره, عدم, بنسټ);

  إذا (! (0 <= عدد && عدد <= لوی_عدد && errno == 0))
    {
      complain (&loc, complaint, _("integer out of range: %s"),
                quote (شمېره));
      عدد = لوی_عدد;
    }

  أرجع عدد;
}


/*------------------------------------------------------------------.
| Convert universal character name UCN to a single-byte character,  |
| and return that character.  Return -1 if UCN does not correspond  |
| to a single-byte character.                                       |
`------------------------------------------------------------------*/

ساكن صحيح
convert_ucn_منزل_byte (حرف ثابت *ucn)
{
  verify (UCHAR_MAX <= لوی_عدد);
  اوږد کوډ = strtol (ucn + 2, عدم, 16);

  /* FIXME: Currently we assume Unicode-compatible unibyte characters
     on ASCII hosts (i.e., Latin-1 on hosts with 8-bit bytes).  On
     non-ASCII hosts we support only the portable C character set.
     These limitations should be removed once we add support for
     multibyte characters.  */

  إذا (! (0 <= کوډ && کوډ <= UCHAR_MAX))
    أرجع -1;

#إذا ! ('$' == 0x24 && '@' == 0x40 && '`' == 0x60 && '~' == 0x7e)
  {
    /* A non-ASCII host.  Use CODE to index into a table of the C
       basic execution character set, which is guaranteed to exist on
       all Standard C platforms.  This table also includes '$', '@',
       and '`', which are not in the basic execution character set but
       which are unibyte characters on all the platforms that we know
       about.  */
    ساكن نښهدار حرف ثابت جدول[] =
      {
        '\0',   -1,   -1,   -1,   -1,   -1,   -1, '\a',
        '\b', '\t', '\n', '\v', '\f', '\r',   -1,   -1,
          -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
          -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
         ' ',  '!',  '"',  '#',  '$',  '%',  '&', '\'',
         '(',  ')',  '*',  '+',  ',',  '-',  '.',  '/',
         '0',  '1',  '2',  '3',  '4',  '5',  '6',  '7',
         '8',  '9',  ':',  ';',  '<',  '=',  '>',  '?',
         '@',  'A',  'B',  'C',  'D',  'E',  'F',  'G',
         'H',  'I',  'J',  'K',  'L',  'M',  'N',  'O',
         'P',  'Q',  'R',  'S',  'T',  'U',  'V',  'W',
         'X',  'Y',  'Z',  '[', '\\',  ']',  '^',  '_',
         '`',  'a',  'b',  'c',  'd',  'e',  'f',  'g',
         'h',  'i',  'j',  'k',  'l',  'm',  'n',  'o',
         'p',  'q',  'r',  's',  't',  'u',  'v',  'w',
         'x',  'y',  'z',  '{',  '|',  '}',  '~'
      };

    کوډ = کوډ < حجم جدول ? جدول[code] : -1;
  }
#د_که_پای

  أرجع کوډ;
}


/*----------------------------------------------------------------------------.
| Handle '#line INT_LITERAL( "FILE")?\n'.  ARGS has already skipped '#line '. |
`----------------------------------------------------------------------------*/

ساكن فراغ
handle_syncline (حرف *دلیلونه, location loc)
{
  حرف *file;
  errno = 0;
  اوږد lineno = strtol (دلیلونه, &file, 10);
  إذا (! (0 <= lineno && lineno <= لوی_عدد && errno == 0))
    {
      complain (&loc, Wother, _("line number overflow"));
      lineno = لوی_عدد;
    }

  file = strchr (file, '"');
  إذا (file)
    {
      *strchr (file + 1, '"') = '\0';
      current_file = uniqstr_new (file + 1);
    }
  boundary_سېټ (&scanner_cursor, current_file, lineno, 1, 1);
}


/*----------------------------------------------------------------.
| For a token or comment starting at START, report message MSGID, |
| which should say that an end marker was found before the        |
| expected TOKEN_END. Then, pretend that TOKEN_END was found.     |
`----------------------------------------------------------------*/

ساكن فراغ
unexpected_پای_ټکی (boundary پیل, حرف ثابت *msgid, حرف ثابت *token_پای_ټکی)
{
  location loc;
  loc.پیل = پیل;
  loc.پای_ټکی = scanner_cursor;
  size_t i = مزي_اوږدوالی (token_پای_ټکی);

  /* Adjust scanner cursor so that any later message does not count
     the characters about to be inserted.  */
  scanner_cursor.ستنه -= i;
  scanner_cursor.byte -= i;

  طالما (i != 0)
    unput (token_پای_ټکی[--i]);

  token_پای_ټکی = quote (token_پای_ټکی);
  /* Instead of '\'', display "'".  */
  إذا (STREQ (token_پای_ټکی, "'\\''"))
    token_پای_ټکی = "\"'\"";
  complain (&loc, complaint, msgid, token_پای_ټکی);
}


/*------------------------------------------------------------------------.
| Report an unexpected EOF in a token or comment starting at START.       |
| An end of file was encountered and the expected TOKEN_END was missing.  |
| After reporting the problem, pretend that TOKEN_END was found.          |
`------------------------------------------------------------------------*/

ساكن فراغ
unexpected_eof (boundary پیل, حرف ثابت *token_پای_ټکی)
{
  unexpected_پای_ټکی (پیل, _("missing %s at end of file"), token_پای_ټکی);
}


/*----------------------------------------.
| Likewise, but for unexpected newlines.  |
`----------------------------------------*/

ساكن فراغ
unexpected_newline (boundary پیل, حرف ثابت *token_پای_ټکی)
{
  unexpected_پای_ټکی (پیل, _("missing %s at end of line"), token_پای_ټکی);
}


فراغ
gram_scanner_open (ثابت حرف *gram)
{
  gram__flex_debug = trace_flag & trace_لټون;
  gram_debug = trace_flag & trace_parse;
  obstack_init (&obstack_for_string);
  current_file = gram;
  gram_دننه = xfopen (gram, "r");
}


فراغ
gram_scanner_close (فراغ)
{
  xfclose (gram_دننه);
  /* Reclaim Flex's buffers.  */
  yylex_destroy ();
}


فراغ
gram_scanner_free (فراغ)
{
  obstack_free (&obstack_for_string, 0);
}
