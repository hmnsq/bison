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
%option اگیتر="gram_" outfile="lex.yy.c"

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

#تعریف_کرو FLEX_اگیتر(Id) gram_ ## Id
#ضمن "src/flex-scanner.h"

/* Work around a bug in flex 2.5.31.  See Debian bug 333231
   <https://bugs.debian.org/333231>.  */
#تعریف_رد gram_wrap
#تعریف_کرو gram_wrap() 1

#تعریف_کرو YY_DECL GRAM_LEX_DECL

/* Location of scanner cursor.  */
ساكن boundary scanner_cursor;

#تعریف_کرو YY_USER_ACTION  location_compute (loc, &scanner_cursor, yytext, yyleng);

/* Report that yytext is an extension, and evaluate to its token kind.  */
#تعریف_کرو BISON_DIRECTIVE(Directive)                              \
  (bison_directive (loc, yytext), PERCENT_ ## Directive)

#تعریف_کرو RETURN_PERCENT_PARAM(قدر_4)             \
  RETURN_قدر(PERCENT_PARAM, param_ ## قدر_4)

#تعریف_کرو RETURN_PERCENT_FLAG(قدر_4)                      \
  RETURN_قدر(PERCENT_FLAG, uniqstr_new (قدر_4))

#تعریف_کرو RETURN_قدر(Token, قدر_4)              \
  افعل {                                          \
    val->Token = قدر_4;                         \
    أرجع Token;                               \
  } طالما (0)

#تعریف_کرو ROLLBACK_CURRENT_TOKEN                                  \
  افعل {                                                          \
    scanner_cursor.کالم -= mbsnwidth (yytext, yyleng, 0);     \
    scanner_cursor.byte -= yyleng;                              \
    yyless (0);                                                 \
  } طالما (0)

#تعریف_کرو DEPRECATED_DIRECTIVE(پیغام_2)                               \
  افعل {                                                          \
    deprecated_directive (loc, yytext, پیغام_2);                    \
    scanner_cursor.کالم -= mbsnwidth (پیغام_2, لڑی_لمبائی (پیغام_2), 0);  \
    scanner_cursor.byte -= لڑی_لمبائی (پیغام_2);                        \
    لكل (size_t i = لڑی_لمبائی (پیغام_2); i != 0; --i)                  \
      unput (پیغام_2[i - 1]);                                       \
  } طالما (0)


#تعریف_کرو STRING_GROW_ESCAPE(Char)                                \
  افعل {                                                          \
    verify (UCHAR_MAX < ULONG_MAX);                             \
    لما c = Char;                                              \
    bool valid = 0 < c && c <= UCHAR_MAX;                       \
    إذا (!valid)                                                 \
      complain (loc, complaint,                                 \
                _("invalid number after \\-escape: %s"),        \
                yytext + 1);                                    \
    إذا (YY_شروع == SC_ESCAPED_CHARACTER)                       \
      STRING_1GROW (valid ? c : '?');                           \
    وإلا                                                        \
      STRING_GROW ();                                           \
  } طالما (0)


/* The current file name.  Might change with #line.  */
ساكن uniqstr current_file = عدم;

/* A string representing the most recently saved token.  */
ساكن حرف *آخری_string = عدم;

/* Bracketed identifier. */
ساكن uniqstr bracketed_id_str = عدم;
ساكن location bracketed_id_loc;
ساكن boundary bracketed_id_شروع;
ساكن صحيح bracketed_id_context_state = 0;

فراغ
gram_scanner_آخری_string_free (فراغ)
{
  STRING_FREE ();
}

ساكن فراغ handle_syncline (حرف *, location);
ساكن صحيح جانچ_integer (حرف ثابت *p, صحيح ادھار, location loc);
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
%x SC_ٹیگ

 /* Four types of user code:
    - prologue (code between '%{' '%}' in the first section, before %%);
    - actions, printers, union, etc, (between braced in the middle section);
    - epilogue (everything after the second %%).
    - predicate (code between '%?{' and '{' in middle section); */
%x SC_PROLOGUE SC_BRACED_کوڈ SC_EPILOGUE SC_PREDICATE
 /* C and C++ comments in code. */
%x SC_COMMENT SC_سطر_COMMENT
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
  location id_loc PACIFY_CC (= خالی_تھاواں_loc);

  /* Where containing code started, when applicable.  Its initial
     value is relevant only when yylex is invoked in the SC_EPILOGUE
     start condition.  */
  boundary کوڈ_شروع = scanner_cursor;

  /* Where containing comment or string or character literal started,
     when applicable.  */
  boundary token_شروع PACIFY_CC (= scanner_cursor);

  /* We cannot trust YY_USER_INIT, whose semantics changes over time
     (it moved in Flex 2.5.38).  */
  ساكن bool پہلا = true;
  إذا (پہلا)
    {
      scanner_cursor = loc->شروع;
      پہلا = false;
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
    token_شروع = loc->شروع;
    context_state = YY_شروع;
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
  "%code"                           أرجع BISON_DIRECTIVE (کوڈ_3);
  "%debug"                          RETURN_PERCENT_FLAG ("parse.trace");
  "%default-prec"                   أرجع BISON_DIRECTIVE (DEFAULT_PREC);
  "%define"                         أرجع BISON_DIRECTIVE (DEFINE);
  "%defines"                        أرجع BISON_DIRECTIVE (سرخی_2); // Deprecated in 3.8.
  "%destructor"                     أرجع BISON_DIRECTIVE (DESTRUCTOR);
  "%dprec"                          أرجع BISON_DIRECTIVE (DPREC);
  "%empty"                          أرجع BISON_DIRECTIVE (خالی_تھاواں_2);
  "%expect"                         أرجع BISON_DIRECTIVE (EXPECT);
  "%expect-rr"                      أرجع BISON_DIRECTIVE (EXPECT_RR);
  "%file-prefix"                    RETURN_قدر (PERCENT_FILE_اگیتر, uniqstr_new (yytext));
  "%glr-parser"                     أرجع BISON_DIRECTIVE (GLR_PARSER);
  "%header"                         أرجع BISON_DIRECTIVE (سرخی_2);
  "%initial-action"                 أرجع BISON_DIRECTIVE (INITIAL_ACTION);
  "%language"                       أرجع BISON_DIRECTIVE (LANGUAGE);
  "%left"                           أرجع PERCENT_بایاں;
  "%lex-param"                      RETURN_PERCENT_PARAM (lex);
  "%locations"                      RETURN_PERCENT_FLAG ("locations");
  "%merge"                          أرجع BISON_DIRECTIVE (ملاؤ);
  "%no-default-prec"                أرجع BISON_DIRECTIVE (NO_DEFAULT_PREC);
  "%no-lines"                       أرجع BISON_DIRECTIVE (NO_سطریں);
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
  "%right"                          أرجع PERCENT_دایاں;
  "%skeleton"                       أرجع BISON_DIRECTIVE (SKELETON);
  "%start"                          أرجع PERCENT_شروع;
  "%term"                           أرجع BISON_DIRECTIVE (TOKEN);
  "%token"                          أرجع PERCENT_TOKEN;
  "%token-table"                    أرجع BISON_DIRECTIVE (TOKEN_جدول);
  "%type"                           أرجع PERCENT_TYPE;
  "%union"                          أرجع PERCENT_UNION;
  "%verbose"                        أرجع BISON_DIRECTIVE (VERBOSE);
  "%yacc"                           أرجع PERCENT_YACC;

  /* Deprecated since Bison 2.3b (2008-05-27), but the warning is
     issued only since Bison 3.4. */
  "%pure"[-_]"parser"               RETURN_قدر (PERCENT_خالص_PARSER, uniqstr_new (yytext));

  /* Deprecated since Bison 3.0 (2013-07-25), but the warning is
     issued only since Bison 3.3. */
  "%error-verbose"                  RETURN_قدر (PERCENT_ERROR_VERBOSE, uniqstr_new (yytext));

  /* Deprecated since Bison 2.6 (2012-07-19), but the warning is
     issued only since Bison 3.3. */
  "%name"[-_]"prefix"{eqopt}{sp}    RETURN_قدر (PERCENT_NAME_اگیتر, uniqstr_new (yytext));

  /* Deprecated since Bison 2.7.90, 2012. */
  "%default"[-_]"prec"              DEPRECATED_DIRECTIVE ("%default-prec");
  "%error"[-_]"verbose"             RETURN_قدر (PERCENT_ERROR_VERBOSE, uniqstr_new (yytext));
  "%expect"[-_]"rr"                 DEPRECATED_DIRECTIVE ("%expect-rr");
  "%file-prefix"{eqopt}             RETURN_قدر (PERCENT_FILE_اگیتر, uniqstr_new (yytext));
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

  {صحيح}      RETURN_قدر (INT_LITERAL, جانچ_integer (yytext, 10, *loc));
  {xint}     RETURN_قدر (INT_LITERAL, جانچ_integer (yytext, 16, *loc));

  /* Identifiers may not start with a digit.  Yet, don't silently
     accept "1FOO" as "1 FOO".  */
  {صحيح}{id} {
    complain (loc, complaint, _("invalid identifier: %s"), quote (yytext));
    أرجع GRAM_error;
  }

  /* Characters.  */
  "'"         token_شروع = loc->شروع; BEGIN SC_ESCAPED_CHARACTER;

  /* Strings. */
  "\""        token_شروع = loc->شروع; STRING_1GROW ('"'); BEGIN SC_ESCAPED_STRING;
  "_(\""      token_شروع = loc->شروع; STRING_1GROW ('"'); BEGIN SC_ESCAPED_TSTRING;

  /* Prologue. */
  "%{"        کوڈ_شروع = loc->شروع; BEGIN SC_PROLOGUE;

  /* Code in between braces.  */
  "{" {
    STRING_GROW ();
    nesting = 0;
    کوڈ_شروع = loc->شروع;
    BEGIN SC_BRACED_کوڈ;
  }

  /* Semantic predicate. */
  "%?"([ \f\t\v]|{eol})*"{" {
    nesting = 0;
    کوڈ_شروع = loc->شروع;
    BEGIN SC_PREDICATE;
  }

  /* A type. */
  "<*>"       أرجع ٹیگ_ANY;
  "<>"        أرجع ٹیگ_NONE;
  "<"         {
    nesting = 0;
    token_شروع = loc->شروع;
    BEGIN SC_ٹیگ;
  }

  "%%" {
    ساكن صحيح percent_percent_گنتی;
    إذا (++percent_percent_گنتی == 2)
      BEGIN SC_EPILOGUE;
    أرجع PERCENT_PERCENT;
  }

  "[" {
    bracketed_id_str = عدم;
    bracketed_id_شروع = loc->شروع;
    bracketed_id_context_state = YY_شروع;
    BEGIN SC_BRACKETED_ID;
  }

  [^\[%A-Za-z0-9_<>{}""''*;|=/, \f\r\n\t\v]+|. {
    complain (loc, complaint, "%s: %s",
              ngettext ("invalid character", "invalid characters", yyleng),
              quote_mem (yytext, yyleng));
    أرجع GRAM_error;
  }

  <<EOF>> {
    loc->شروع = loc->سرا = scanner_cursor;
    yyterminate ();
  }
}


  /*--------------------------------------------------------------.
  | Supporting \0 complexifies our implementation for no expected |
  | added value.                                                  |
  `--------------------------------------------------------------*/

<SC_ESCAPED_CHARACTER,SC_ESCAPED_STRING,SC_ESCAPED_TSTRING,SC_ٹیگ>
{
  \0         {
    complain (loc, complaint, _("invalid null character"));
    STRING_انت ();
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
        bracketed_id_شروع = loc->شروع;
        bracketed_id_context_state = YY_شروع;
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
    unexpected_eof (bracketed_id_شروع, "]");
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
  <<EOF>>  unexpected_eof (token_شروع, "*/"); BEGIN context_state;
}


  /*------------------------------------------------------------.
  | Scanning a C comment.  The initial '/ *' is already eaten.  |
  `------------------------------------------------------------*/

<SC_COMMENT>
{
  "*"{splice}"/"  STRING_GROW (); BEGIN context_state;
  <<EOF>>         unexpected_eof (token_شروع, "*/"); BEGIN context_state;
}


  /*--------------------------------------------------------------.
  | Scanning a line comment.  The initial '//' is already eaten.  |
  `--------------------------------------------------------------*/

<SC_سطر_COMMENT>
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
    STRING_انت ();
    BEGIN INITIAL;
    loc->شروع = token_شروع;
    complain (loc, Wyacc,
              _("POSIX Yacc does not support string literals"));
    RETURN_قدر (STRING, آخری_string);
  }
  <<EOF>>   unexpected_eof (token_شروع, "\"");
  "\n"      unexpected_newline (token_شروع, "\"");
}

<SC_ESCAPED_TSTRING>
{
  "\")" {
    STRING_1GROW ('"');
    STRING_انت ();
    BEGIN INITIAL;
    loc->شروع = token_شروع;
    complain (loc, Wyacc,
              _("POSIX Yacc does not support string literals"));
    RETURN_قدر (TSTRING, آخری_string);
  }
  <<EOF>>   unexpected_eof (token_شروع, "\")");
  "\n"      unexpected_newline (token_شروع, "\")");
}



  /*----------------------------------------------------------.
  | Scanning a Bison character literal, decoding its escapes. |
  | The initial quote is already eaten.                       |
  `----------------------------------------------------------*/

<SC_ESCAPED_CHARACTER>
{
  "'" {
    STRING_انت ();
    BEGIN INITIAL;
    loc->شروع = token_شروع;

    إذا (آخری_string[0] == '\0')
      {
        complain (loc, complaint, _("empty character literal"));
        STRING_FREE ();
        أرجع GRAM_error;
      }
    وإلا إذا (آخری_string[1] != '\0')
      {
        complain (loc, complaint, _("extra characters in character literal"));
        STRING_FREE ();
        أرجع GRAM_error;
      }
    وإلا
      {
        val->CHAR_LITERAL = آخری_string[0];
        STRING_FREE ();
        أرجع CHAR_LITERAL;
      }
  }
  {eol}     unexpected_newline (token_شروع, "'");
  <<EOF>>   unexpected_eof (token_شروع, "'");
}



  /*--------------------------------------------------------------.
  | Scanning a tag.  The initial angle bracket is already eaten.  |
  `--------------------------------------------------------------*/

<SC_ٹیگ>
{
  ">" {
    --nesting;
    إذا (nesting < 0)
      {
        STRING_انت ();
        loc->شروع = token_شروع;
        val->ٹیگ_2 = uniqstr_new (آخری_string);
        STRING_FREE ();
        BEGIN INITIAL;
        أرجع ٹیگ_2;
      }
    STRING_GROW ();
  }

  ([^<>]|->)+ STRING_GROW ();
  "<"+   STRING_GROW (); nesting += yyleng;

  <<EOF>>   unexpected_eof (token_شروع, ">");
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
    إذا (c_isspace ((بنانشانی حرف) *p) && c_isprint ((بنانشانی حرف) *p))
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
    unexpected_eof (token_شروع,
                    YY_شروع == SC_ESCAPED_CHARACTER ? "?'"
                    : YY_شروع == SC_ESCAPED_STRING ? "?\""
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
  {eol}         unexpected_newline (token_شروع, "'");
  <<EOF>>       unexpected_eof (token_شروع, "'");
}

<SC_STRING>
{
  "\""          STRING_GROW (); BEGIN context_state;
  {eol}         unexpected_newline (token_شروع, "\"");
  <<EOF>>       unexpected_eof (token_شروع, "\"");
}


  /*---------------------------------------------------.
  | Strings, comments etc. can be found in user code.  |
  `---------------------------------------------------*/

<SC_BRACED_کوڈ,SC_PROLOGUE,SC_EPILOGUE,SC_PREDICATE>
{
  "'" {
    STRING_GROW ();
    context_state = YY_شروع;
    token_شروع = loc->شروع;
    BEGIN SC_CHARACTER;
  }
  "\"" {
    STRING_GROW ();
    context_state = YY_شروع;
    token_شروع = loc->شروع;
    BEGIN SC_STRING;
  }
  "/"{splice}"*" {
    STRING_GROW ();
    context_state = YY_شروع;
    token_شروع = loc->شروع;
    BEGIN SC_COMMENT;
  }
  "/"{splice}"/" {
    STRING_GROW ();
    context_state = YY_شروع;
    BEGIN SC_سطر_COMMENT;
  }
}



  /*-----------------------------------------------------------.
  | Scanning some code in braces (actions, predicates). The    |
  | initial "{" is already eaten.                              |
  `-----------------------------------------------------------*/

<SC_BRACED_کوڈ,SC_PREDICATE>
{
  "{"|"<"{splice}"%"  STRING_GROW (); nesting++;
  "%"{splice}">"      STRING_GROW (); nesting--;

  /* Tokenize '<<%' correctly (as '<<' '%') rather than incorrectly
     (as '<' '<%').  */
  "<"{splice}"<"  STRING_GROW ();

  <<EOF>>   unexpected_eof (کوڈ_شروع, "}");
}

<SC_BRACED_کوڈ>
{
  "}" {
    STRING_1GROW ('}');

    --nesting;
    إذا (nesting < 0)
      {
        STRING_انت ();
        loc->شروع = کوڈ_شروع;
        BEGIN INITIAL;
        RETURN_قدر (BRACED_کوڈ, آخری_string);
      }
  }
}

<SC_PREDICATE>
{
  "}" {
    --nesting;
    إذا (nesting < 0)
      {
        STRING_انت ();
        loc->شروع = کوڈ_شروع;
        BEGIN INITIAL;
        RETURN_قدر (BRACED_PREDICATE, آخری_string);
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
    STRING_انت ();
    loc->شروع = کوڈ_شروع;
    BEGIN INITIAL;
    RETURN_قدر (PROLOGUE, آخری_string);
  }

  <<EOF>>   unexpected_eof (کوڈ_شروع, "%}");
}


  /*---------------------------------------------------------------.
  | Scanning the epilogue (everything after the second "%%", which |
  | has already been eaten).                                       |
  `---------------------------------------------------------------*/

<SC_EPILOGUE>
{
  <<EOF>> {
    STRING_انت ();
    loc->شروع = کوڈ_شروع;
    BEGIN INITIAL;
    RETURN_قدر (EPILOGUE, آخری_string);
  }
}


  /*-----------------------------------------------------.
  | By default, grow the string obstack with the input.  |
  `-----------------------------------------------------*/

<SC_COMMENT,SC_سطر_COMMENT,SC_BRACED_کوڈ,SC_PREDICATE,SC_PROLOGUE,SC_EPILOGUE,SC_STRING,SC_CHARACTER,SC_ESCAPED_CHARACTER,SC_ESCAPED_STRING,SC_ESCAPED_TSTRING>
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
جانچ_integer (حرف ثابت *نمبر, صحيح ادھار, location loc)
{
  verify (ودھ_توں_ودھ_عدد < ULONG_MAX);
  إذا (ادھار == 16)
    complain (&loc, Wyacc,
              _("POSIX Yacc does not support hexadecimal literals"));

  errno = 0;
  لما عدد = strtol (نمبر, عدم, ادھار);

  إذا (! (0 <= عدد && عدد <= ودھ_توں_ودھ_عدد && errno == 0))
    {
      complain (&loc, complaint, _("integer out of range: %s"),
                quote (نمبر));
      عدد = ودھ_توں_ودھ_عدد;
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
  verify (UCHAR_MAX <= ودھ_توں_ودھ_عدد);
  لما کوڈ = strtol (ucn + 2, عدم, 16);

  /* FIXME: Currently we assume Unicode-compatible unibyte characters
     on ASCII hosts (i.e., Latin-1 on hosts with 8-bit bytes).  On
     non-ASCII hosts we support only the portable C character set.
     These limitations should be removed once we add support for
     multibyte characters.  */

  إذا (! (0 <= کوڈ && کوڈ <= UCHAR_MAX))
    أرجع -1;

#إذا ! ('$' == 0x24 && '@' == 0x40 && '`' == 0x60 && '~' == 0x7e)
  {
    /* A non-ASCII host.  Use CODE to index into a table of the C
       basic execution character set, which is guaranteed to exist on
       all Standard C platforms.  This table also includes '$', '@',
       and '`', which are not in the basic execution character set but
       which are unibyte characters on all the platforms that we know
       about.  */
    ساكن نشانیوالا حرف ثابت جدول[] =
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

    کوڈ = کوڈ < حجم جدول ? جدول[code] : -1;
  }
#جے_انت

  أرجع کوڈ;
}


/*----------------------------------------------------------------------------.
| Handle '#line INT_LITERAL( "FILE")?\n'.  ARGS has already skipped '#line '. |
`----------------------------------------------------------------------------*/

ساكن فراغ
handle_syncline (حرف *دلیلاں, location loc)
{
  حرف *file;
  errno = 0;
  لما lineno = strtol (دلیلاں, &file, 10);
  إذا (! (0 <= lineno && lineno <= ودھ_توں_ودھ_عدد && errno == 0))
    {
      complain (&loc, Wother, _("line number overflow"));
      lineno = ودھ_توں_ودھ_عدد;
    }

  file = strchr (file, '"');
  إذا (file)
    {
      *strchr (file + 1, '"') = '\0';
      current_file = uniqstr_new (file + 1);
    }
  boundary_سیٹ (&scanner_cursor, current_file, lineno, 1, 1);
}


/*----------------------------------------------------------------.
| For a token or comment starting at START, report message MSGID, |
| which should say that an end marker was found before the        |
| expected TOKEN_END. Then, pretend that TOKEN_END was found.     |
`----------------------------------------------------------------*/

ساكن فراغ
unexpected_سرا (boundary شروع, حرف ثابت *msgid, حرف ثابت *token_سرا)
{
  location loc;
  loc.شروع = شروع;
  loc.سرا = scanner_cursor;
  size_t i = لڑی_لمبائی (token_سرا);

  /* Adjust scanner cursor so that any later message does not count
     the characters about to be inserted.  */
  scanner_cursor.کالم -= i;
  scanner_cursor.byte -= i;

  طالما (i != 0)
    unput (token_سرا[--i]);

  token_سرا = quote (token_سرا);
  /* Instead of '\'', display "'".  */
  إذا (STREQ (token_سرا, "'\\''"))
    token_سرا = "\"'\"";
  complain (&loc, complaint, msgid, token_سرا);
}


/*------------------------------------------------------------------------.
| Report an unexpected EOF in a token or comment starting at START.       |
| An end of file was encountered and the expected TOKEN_END was missing.  |
| After reporting the problem, pretend that TOKEN_END was found.          |
`------------------------------------------------------------------------*/

ساكن فراغ
unexpected_eof (boundary شروع, حرف ثابت *token_سرا)
{
  unexpected_سرا (شروع, _("missing %s at end of file"), token_سرا);
}


/*----------------------------------------.
| Likewise, but for unexpected newlines.  |
`----------------------------------------*/

ساكن فراغ
unexpected_newline (boundary شروع, حرف ثابت *token_سرا)
{
  unexpected_سرا (شروع, _("missing %s at end of line"), token_سرا);
}


فراغ
gram_scanner_open (ثابت حرف *gram)
{
  gram__flex_debug = trace_flag & trace_جانچ;
  gram_debug = trace_flag & trace_parse;
  obstack_init (&obstack_for_string);
  current_file = gram;
  gram_وچ = xfopen (gram, "r");
}


فراغ
gram_scanner_close (فراغ)
{
  xfclose (gram_وچ);
  /* Reclaim Flex's buffers.  */
  yylex_destroy ();
}


فراغ
gram_scanner_free (فراغ)
{
  obstack_free (&obstack_for_string, 0);
}
