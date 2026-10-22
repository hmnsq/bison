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
%option ಪೂರ್ವಪ್ರತ್ಯಯ="gram_" outfile="lex.yy.c"

%{
#ಅನ್ತರ್ಭಾವ <c-ctype.h>
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಅನ್ತರ್ಭಾವ <mbswidth.h>
#ಅನ್ತರ್ಭಾವ <quote.h>
#ಅನ್ತರ್ಭಾವ <quotearg.h>

#ಅನ್ತರ್ಭಾವ "src/complain.h"
#ಅನ್ತರ್ಭಾವ "src/files.h"
#ಅನ್ತರ್ಭಾವ "src/getargs.h"
#ಅನ್ತರ್ಭಾವ "src/gram.h"
#ಅನ್ತರ್ಭಾವ "src/reader.h"
#ಅನ್ತರ್ಭಾವ "src/scan-gram.h"
#ಅನ್ತರ್ಭಾವ "src/uniqstr.h"

#ವ್ಯಾಖ್ಯಾನಿಸು FLEX_ಪೂರ್ವಪ್ರತ್ಯಯ(Id) gram_ ## Id
#ಅನ್ತರ್ಭಾವ "src/flex-scanner.h"

/* Work around a bug in flex 2.5.31.  See Debian bug 333231
   <https://bugs.debian.org/333231>.  */
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು gram_wrap
#ವ್ಯಾಖ್ಯಾನಿಸು gram_wrap() 1

#ವ್ಯಾಖ್ಯಾನಿಸು YY_DECL GRAM_LEX_DECL

/* Location of scanner cursor.  */
ಸ್ಥಿರ boundary scanner_cursor;

#ವ್ಯಾಖ್ಯಾನಿಸು YY_USER_ACTION  location_compute (loc, &scanner_cursor, yytext, yyleng);

/* Report that yytext is an extension, and evaluate to its token kind.  */
#ವ್ಯಾಖ್ಯಾನಿಸು BISON_DIRECTIVE(Directive)                              \
  (bison_directive (loc, yytext), PERCENT_ ## Directive)

#ವ್ಯಾಖ್ಯಾನಿಸು RETURN_PERCENT_PARAM(ಮೌಲ್ಯ_4)             \
  RETURN_ಮೌಲ್ಯ(PERCENT_PARAM, param_ ## ಮೌಲ್ಯ_4)

#ವ್ಯಾಖ್ಯಾನಿಸು RETURN_PERCENT_FLAG(ಮೌಲ್ಯ_4)                      \
  RETURN_ಮೌಲ್ಯ(PERCENT_FLAG, uniqstr_new (ಮೌಲ್ಯ_4))

#ವ್ಯಾಖ್ಯಾನಿಸು RETURN_ಮೌಲ್ಯ(Token, ಮೌಲ್ಯ_4)              \
  ಮಾಡು {                                          \
    val->Token = ಮೌಲ್ಯ_4;                         \
    ಹಿಂತಿರುಗಿ Token;                               \
  } ಆಗ (0)

#ವ್ಯಾಖ್ಯಾನಿಸು ROLLBACK_CURRENT_TOKEN                                  \
  ಮಾಡು {                                                          \
    scanner_cursor.ಸ್ತಂಭ -= mbsnwidth (yytext, yyleng, 0);     \
    scanner_cursor.byte -= yyleng;                              \
    yyless (0);                                                 \
  } ಆಗ (0)

#ವ್ಯಾಖ್ಯಾನಿಸು DEPRECATED_DIRECTIVE(ಸಂದೇಶ_3)                               \
  ಮಾಡು {                                                          \
    deprecated_directive (loc, yytext, ಸಂದೇಶ_3);                    \
    scanner_cursor.ಸ್ತಂಭ -= mbsnwidth (ಸಂದೇಶ_3, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ಸಂದೇಶ_3), 0);  \
    scanner_cursor.byte -= ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ಸಂದೇಶ_3);                        \
    ಗಾಗಿ (size_t i = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ಸಂದೇಶ_3); i != 0; --i)                  \
      unput (ಸಂದೇಶ_3[i - 1]);                                       \
  } ಆಗ (0)


#ವ್ಯಾಖ್ಯಾನಿಸು STRING_GROW_ESCAPE(Char)                                \
  ಮಾಡು {                                                          \
    verify (UCHAR_MAX < ULONG_MAX);                             \
    ದೀರ್ಘ c = Char;                                              \
    bool valid = 0 < c && c <= UCHAR_MAX;                       \
    ಆದರೆ (!valid)                                                 \
      complain (loc, complaint,                                 \
                _("invalid number after \\-escape: %s"),        \
                yytext + 1);                                    \
    ಆದರೆ (YY_ಆರಂಭ == SC_ESCAPED_CHARACTER)                       \
      STRING_1GROW (valid ? c : '?');                           \
    ಇಲ್ಲದಿದ್ದರೆ                                                        \
      STRING_GROW ();                                           \
  } ಆಗ (0)


/* The current file name.  Might change with #line.  */
ಸ್ಥಿರ uniqstr current_file = ಖಾಲಿ;

/* A string representing the most recently saved token.  */
ಸ್ಥಿರ ಅಕ್ಷರ *ಕೊನೆಯ_string = ಖಾಲಿ;

/* Bracketed identifier. */
ಸ್ಥಿರ uniqstr bracketed_id_str = ಖಾಲಿ;
ಸ್ಥಿರ location bracketed_id_loc;
ಸ್ಥಿರ boundary bracketed_id_ಆರಂಭ;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ bracketed_id_context_state = 0;

ಶೂನ್ಯ
gram_scanner_ಕೊನೆಯ_string_free (ಶೂನ್ಯ)
{
  STRING_FREE ();
}

ಸ್ಥಿರ ಶೂನ್ಯ handle_syncline (ಅಕ್ಷರ *, location);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಪರಿಶೀಲನೆ_integer (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *p, ಪೂರ್ಣಾಂಕ ತಳ, location loc);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ convert_ucn_ಗಮ್ಯ_byte (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *hex_ಪಠ್ಯ);
ಸ್ಥಿರ ಶೂನ್ಯ unexpected_eof (boundary, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *);
ಸ್ಥಿರ ಶೂನ್ಯ unexpected_newline (boundary, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *);

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
%x SC_ಟ್ಯಾಗ್

 /* Four types of user code:
    - prologue (code between '%{' '%}' in the first section, before %%);
    - actions, printers, union, etc, (between braced in the middle section);
    - epilogue (everything after the second %%).
    - predicate (code between '%?{' and '{' in middle section); */
%x SC_PROLOGUE SC_BRACED_ಸಂಕೇತ SC_EPILOGUE SC_PREDICATE
 /* C and C++ comments in code. */
%x SC_COMMENT SC_ಸಾಲು_COMMENT
 /* Strings and characters in code. */
%x SC_STRING SC_CHARACTER
 /* Bracketed identifiers support. */
%x SC_BRACKETED_ID SC_RETURN_BRACKETED_ID

letter    [.abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_]
id        {letter}({letter}|[-0-9])*
ಪೂರ್ಣಾಂಕ       [0-9]+
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
  ಪೂರ್ಣಾಂಕ nesting PACIFY_CC (= 0);

  /* Parent context state, when applicable.  */
  ಪೂರ್ಣಾಂಕ context_state PACIFY_CC (= 0);

  /* Location of most recent identifier, when applicable.  */
  location id_loc PACIFY_CC (= ಖಾಲಿಗಳು_loc);

  /* Where containing code started, when applicable.  Its initial
     value is relevant only when yylex is invoked in the SC_EPILOGUE
     start condition.  */
  boundary ಸಂಕೇತ_ಆರಂಭ = scanner_cursor;

  /* Where containing comment or string or character literal started,
     when applicable.  */
  boundary token_ಆರಂಭ PACIFY_CC (= scanner_cursor);

  /* We cannot trust YY_USER_INIT, whose semantics changes over time
     (it moved in Flex 2.5.38).  */
  ಸ್ಥಿರ bool ಮೊದಲ = true;
  ಆದರೆ (ಮೊದಲ)
    {
      scanner_cursor = loc->ಆರಂಭ;
      ಮೊದಲ = false;
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
  "//".*       ಮುಂದುವರಿ;
  "/*" {
    token_ಆರಂಭ = loc->ಆರಂಭ;
    context_state = YY_ಆರಂಭ;
    BEGIN SC_YACC_COMMENT;
  }

  ^"#line "{ಪೂರ್ಣಾಂಕ}(" \"".*"\"")?{eol} {
    handle_syncline (yytext + ಗಾತ್ರ "#line " - 1, *loc);
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
  "%binary"                         ಹಿಂತಿರುಗಿ BISON_DIRECTIVE (NONASSOC);
  "%code"                           ಹಿಂತಿರುಗಿ BISON_DIRECTIVE (ಸಂಕೇತ_4);
  "%debug"                          RETURN_PERCENT_FLAG ("parse.trace");
  "%default-prec"                   ಹಿಂತಿರುಗಿ BISON_DIRECTIVE (DEFAULT_PREC);
  "%define"                         ಹಿಂತಿರುಗಿ BISON_DIRECTIVE (DEFINE);
  "%defines"                        ಹಿಂತಿರುಗಿ BISON_DIRECTIVE (ಶೀರ್ಷಿಕೆ_2); // Deprecated in 3.8.
  "%destructor"                     ಹಿಂತಿರುಗಿ BISON_DIRECTIVE (DESTRUCTOR);
  "%dprec"                          ಹಿಂತಿರುಗಿ BISON_DIRECTIVE (DPREC);
  "%empty"                          ಹಿಂತಿರುಗಿ BISON_DIRECTIVE (ಖಾಲಿಗಳು_2);
  "%expect"                         ಹಿಂತಿರುಗಿ BISON_DIRECTIVE (EXPECT);
  "%expect-rr"                      ಹಿಂತಿರುಗಿ BISON_DIRECTIVE (EXPECT_RR);
  "%file-prefix"                    RETURN_ಮೌಲ್ಯ (PERCENT_FILE_ಪೂರ್ವಪ್ರತ್ಯಯ, uniqstr_new (yytext));
  "%glr-parser"                     ಹಿಂತಿರುಗಿ BISON_DIRECTIVE (GLR_PARSER);
  "%header"                         ಹಿಂತಿರುಗಿ BISON_DIRECTIVE (ಶೀರ್ಷಿಕೆ_2);
  "%initial-action"                 ಹಿಂತಿರುಗಿ BISON_DIRECTIVE (INITIAL_ACTION);
  "%language"                       ಹಿಂತಿರುಗಿ BISON_DIRECTIVE (LANGUAGE);
  "%left"                           ಹಿಂತಿರುಗಿ PERCENT_ಎಡ;
  "%lex-param"                      RETURN_PERCENT_PARAM (lex);
  "%locations"                      RETURN_PERCENT_FLAG ("locations");
  "%merge"                          ಹಿಂತಿರುಗಿ BISON_DIRECTIVE (ವಿಲೀನ);
  "%no-default-prec"                ಹಿಂತಿರುಗಿ BISON_DIRECTIVE (NO_DEFAULT_PREC);
  "%no-lines"                       ಹಿಂತಿರುಗಿ BISON_DIRECTIVE (NO_ಸಾಲುಗಳು);
  "%nonassoc"                       ಹಿಂತಿರುಗಿ PERCENT_NONASSOC;
  "%nondeterministic-parser"        ಹಿಂತಿರುಗಿ BISON_DIRECTIVE (NONDETERMINISTIC_PARSER);
  "%nterm"                          ಹಿಂತಿರುಗಿ BISON_DIRECTIVE (NTERM);
  "%output"                         ಹಿಂತಿರುಗಿ BISON_DIRECTIVE (OUTPUT);
  "%param"                          RETURN_PERCENT_PARAM (both);
  "%parse-param"                    RETURN_PERCENT_PARAM (parse);
  "%prec"                           ಹಿಂತಿರುಗಿ PERCENT_PREC;
  "%precedence"                     ಹಿಂತಿರುಗಿ BISON_DIRECTIVE (PRECEDENCE);
  "%printer"                        ಹಿಂತಿರುಗಿ BISON_DIRECTIVE (PRINTER);
  "%require"                        ಹಿಂತಿರುಗಿ BISON_DIRECTIVE (REQUIRE);
  "%right"                          ಹಿಂತಿರುಗಿ PERCENT_ಬಲ;
  "%skeleton"                       ಹಿಂತಿರುಗಿ BISON_DIRECTIVE (SKELETON);
  "%start"                          ಹಿಂತಿರುಗಿ PERCENT_ಆರಂಭ;
  "%term"                           ಹಿಂತಿರುಗಿ BISON_DIRECTIVE (TOKEN);
  "%token"                          ಹಿಂತಿರುಗಿ PERCENT_TOKEN;
  "%token-table"                    ಹಿಂತಿರುಗಿ BISON_DIRECTIVE (TOKEN_ಕೋಷ್ಟಕ);
  "%type"                           ಹಿಂತಿರುಗಿ PERCENT_TYPE;
  "%union"                          ಹಿಂತಿರುಗಿ PERCENT_UNION;
  "%verbose"                        ಹಿಂತಿರುಗಿ BISON_DIRECTIVE (VERBOSE);
  "%yacc"                           ಹಿಂತಿರುಗಿ PERCENT_YACC;

  /* Deprecated since Bison 2.3b (2008-05-27), but the warning is
     issued only since Bison 3.4. */
  "%pure"[-_]"parser"               RETURN_ಮೌಲ್ಯ (PERCENT_ಶುದ್ಧ_PARSER, uniqstr_new (yytext));

  /* Deprecated since Bison 3.0 (2013-07-25), but the warning is
     issued only since Bison 3.3. */
  "%error-verbose"                  RETURN_ಮೌಲ್ಯ (PERCENT_ERROR_VERBOSE, uniqstr_new (yytext));

  /* Deprecated since Bison 2.6 (2012-07-19), but the warning is
     issued only since Bison 3.3. */
  "%name"[-_]"prefix"{eqopt}{sp}    RETURN_ಮೌಲ್ಯ (PERCENT_NAME_ಪೂರ್ವಪ್ರತ್ಯಯ, uniqstr_new (yytext));

  /* Deprecated since Bison 2.7.90, 2012. */
  "%default"[-_]"prec"              DEPRECATED_DIRECTIVE ("%default-prec");
  "%error"[-_]"verbose"             RETURN_ಮೌಲ್ಯ (PERCENT_ERROR_VERBOSE, uniqstr_new (yytext));
  "%expect"[-_]"rr"                 DEPRECATED_DIRECTIVE ("%expect-rr");
  "%file-prefix"{eqopt}             RETURN_ಮೌಲ್ಯ (PERCENT_FILE_ಪೂರ್ವಪ್ರತ್ಯಯ, uniqstr_new (yytext));
  "%fixed"[-_]"output"[-_]"files"   DEPRECATED_DIRECTIVE ("%output \"y.tab.c\"");
  "%no"[-_]"default"[-_]"prec"      DEPRECATED_DIRECTIVE ("%no-default-prec");
  "%no"[-_]"lines"                  DEPRECATED_DIRECTIVE ("%no-lines");
  "%output"{eqopt}                  DEPRECATED_DIRECTIVE ("%output");
  "%token"[-_]"table"               DEPRECATED_DIRECTIVE ("%token-table");

  "%"{id} {
    complain (loc, complaint, _("invalid directive: %s"), quote (yytext));
    ಹಿಂತಿರುಗಿ GRAM_error;
  }

  ":"                     ಹಿಂತಿರುಗಿ COLON;
  "="                     ಹಿಂತಿರುಗಿ EQUAL;
  "|"                     ಹಿಂತಿರುಗಿ PIPE;
  ";"                     ಹಿಂತಿರುಗಿ SEMICOLON;

  {id} {
    val->ID = uniqstr_new (yytext);
    id_loc = *loc;
    bracketed_id_str = ಖಾಲಿ;
    BEGIN SC_AFTER_IDENTIFIER;
  }

  {ಪೂರ್ಣಾಂಕ}      RETURN_ಮೌಲ್ಯ (INT_LITERAL, ಪರಿಶೀಲನೆ_integer (yytext, 10, *loc));
  {xint}     RETURN_ಮೌಲ್ಯ (INT_LITERAL, ಪರಿಶೀಲನೆ_integer (yytext, 16, *loc));

  /* Identifiers may not start with a digit.  Yet, don't silently
     accept "1FOO" as "1 FOO".  */
  {ಪೂರ್ಣಾಂಕ}{id} {
    complain (loc, complaint, _("invalid identifier: %s"), quote (yytext));
    ಹಿಂತಿರುಗಿ GRAM_error;
  }

  /* Characters.  */
  "'"         token_ಆರಂಭ = loc->ಆರಂಭ; BEGIN SC_ESCAPED_CHARACTER;

  /* Strings. */
  "\""        token_ಆರಂಭ = loc->ಆರಂಭ; STRING_1GROW ('"'); BEGIN SC_ESCAPED_STRING;
  "_(\""      token_ಆರಂಭ = loc->ಆರಂಭ; STRING_1GROW ('"'); BEGIN SC_ESCAPED_TSTRING;

  /* Prologue. */
  "%{"        ಸಂಕೇತ_ಆರಂಭ = loc->ಆರಂಭ; BEGIN SC_PROLOGUE;

  /* Code in between braces.  */
  "{" {
    STRING_GROW ();
    nesting = 0;
    ಸಂಕೇತ_ಆರಂಭ = loc->ಆರಂಭ;
    BEGIN SC_BRACED_ಸಂಕೇತ;
  }

  /* Semantic predicate. */
  "%?"([ \f\t\v]|{eol})*"{" {
    nesting = 0;
    ಸಂಕೇತ_ಆರಂಭ = loc->ಆರಂಭ;
    BEGIN SC_PREDICATE;
  }

  /* A type. */
  "<*>"       ಹಿಂತಿರುಗಿ ಟ್ಯಾಗ್_ANY;
  "<>"        ಹಿಂತಿರುಗಿ ಟ್ಯಾಗ್_NONE;
  "<"         {
    nesting = 0;
    token_ಆರಂಭ = loc->ಆರಂಭ;
    BEGIN SC_ಟ್ಯಾಗ್;
  }

  "%%" {
    ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ percent_percent_ಎಣಿಕೆ;
    ಆದರೆ (++percent_percent_ಎಣಿಕೆ == 2)
      BEGIN SC_EPILOGUE;
    ಹಿಂತಿರುಗಿ PERCENT_PERCENT;
  }

  "[" {
    bracketed_id_str = ಖಾಲಿ;
    bracketed_id_ಆರಂಭ = loc->ಆರಂಭ;
    bracketed_id_context_state = YY_ಆರಂಭ;
    BEGIN SC_BRACKETED_ID;
  }

  [^\[%A-Za-z0-9_<>{}""''*;|=/, \f\r\n\t\v]+|. {
    complain (loc, complaint, "%s: %s",
              ngettext ("invalid character", "invalid characters", yyleng),
              quote_mem (yytext, yyleng));
    ಹಿಂತಿರುಗಿ GRAM_error;
  }

  <<EOF>> {
    loc->ಆರಂಭ = loc->ಕೊನೆ = scanner_cursor;
    yyterminate ();
  }
}


  /*--------------------------------------------------------------.
  | Supporting \0 complexifies our implementation for no expected |
  | added value.                                                  |
  `--------------------------------------------------------------*/

<SC_ESCAPED_CHARACTER,SC_ESCAPED_STRING,SC_ESCAPED_TSTRING,SC_ಟ್ಯಾಗ್>
{
  \0         {
    complain (loc, complaint, _("invalid null character"));
    STRING_ಮುಕ್ತಾಯ ();
    STRING_FREE ();
    ಹಿಂತಿರುಗಿ GRAM_error;
  }
}


  /*-----------------------------------------------------------------.
  | Scanning after an identifier, checking whether a colon is next.  |
  `-----------------------------------------------------------------*/

<SC_AFTER_IDENTIFIER>
{
  "[" {
    ಆದರೆ (bracketed_id_str)
      {
        ROLLBACK_CURRENT_TOKEN;
        BEGIN SC_RETURN_BRACKETED_ID;
        *loc = id_loc;
        ಹಿಂತಿರುಗಿ ID;
      }
    ಇಲ್ಲದಿದ್ದರೆ
      {
        bracketed_id_ಆರಂಭ = loc->ಆರಂಭ;
        bracketed_id_context_state = YY_ಆರಂಭ;
        BEGIN SC_BRACKETED_ID;
      }
  }
  ":" {
    ROLLBACK_CURRENT_TOKEN;
    BEGIN (bracketed_id_str ? SC_RETURN_BRACKETED_ID : INITIAL);
    *loc = id_loc;
    ಹಿಂತಿರುಗಿ ID_COLON;
  }
  . {
    ROLLBACK_CURRENT_TOKEN;
    BEGIN (bracketed_id_str ? SC_RETURN_BRACKETED_ID : INITIAL);
    *loc = id_loc;
    ಹಿಂತಿರುಗಿ ID;
  }
  <<EOF>> {
    BEGIN (bracketed_id_str ? SC_RETURN_BRACKETED_ID : INITIAL);
    *loc = id_loc;
    ಹಿಂತಿರುಗಿ ID;
  }
}

  /*--------------------------------.
  | Scanning bracketed identifiers. |
  `--------------------------------*/

<SC_BRACKETED_ID>
{
  {id} {
    ಆದರೆ (bracketed_id_str)
      {
        complain (loc, complaint,
                  _("unexpected identifier in bracketed name: %s"),
                  quote (yytext));
        ಹಿಂತಿರುಗಿ GRAM_error;
      }
    ಇಲ್ಲದಿದ್ದರೆ
      {
        bracketed_id_str = uniqstr_new (yytext);
        bracketed_id_loc = *loc;
      }
  }
  "]" {
    BEGIN bracketed_id_context_state;
    ಆದರೆ (bracketed_id_str)
      {
        ಆದರೆ (INITIAL == bracketed_id_context_state)
          {
            val->BRACKETED_ID = bracketed_id_str;
            bracketed_id_str = 0;
            *loc = bracketed_id_loc;
            ಹಿಂತಿರುಗಿ BRACKETED_ID;
          }
      }
    ಇಲ್ಲದಿದ್ದರೆ
      {
        complain (loc, complaint, _("an identifier expected"));
        ಹಿಂತಿರುಗಿ GRAM_error;
      }
  }

  [^\].A-Za-z0-9_/ \f\r\n\t\v]+|. {
    complain (loc, complaint, "%s: %s",
              ngettext ("invalid character in bracketed name",
                        "invalid characters in bracketed name", yyleng),
              quote_mem (yytext, yyleng));
    ಹಿಂತಿರುಗಿ GRAM_error;
  }

  <<EOF>> {
    BEGIN bracketed_id_context_state;
    unexpected_eof (bracketed_id_ಆರಂಭ, "]");
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
    ಹಿಂತಿರುಗಿ BRACKETED_ID;
  }
}


  /*---------------------------------------------------------------.
  | Scanning a Yacc comment.  The initial '/ *' is already eaten.  |
  `---------------------------------------------------------------*/

<SC_YACC_COMMENT>
{
  "*/"     BEGIN context_state;
  .|{eol}  ಮುಂದುವರಿ;
  <<EOF>>  unexpected_eof (token_ಆರಂಭ, "*/"); BEGIN context_state;
}


  /*------------------------------------------------------------.
  | Scanning a C comment.  The initial '/ *' is already eaten.  |
  `------------------------------------------------------------*/

<SC_COMMENT>
{
  "*"{splice}"/"  STRING_GROW (); BEGIN context_state;
  <<EOF>>         unexpected_eof (token_ಆರಂಭ, "*/"); BEGIN context_state;
}


  /*--------------------------------------------------------------.
  | Scanning a line comment.  The initial '//' is already eaten.  |
  `--------------------------------------------------------------*/

<SC_ಸಾಲು_COMMENT>
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
    STRING_ಮುಕ್ತಾಯ ();
    BEGIN INITIAL;
    loc->ಆರಂಭ = token_ಆರಂಭ;
    complain (loc, Wyacc,
              _("POSIX Yacc does not support string literals"));
    RETURN_ಮೌಲ್ಯ (STRING, ಕೊನೆಯ_string);
  }
  <<EOF>>   unexpected_eof (token_ಆರಂಭ, "\"");
  "\n"      unexpected_newline (token_ಆರಂಭ, "\"");
}

<SC_ESCAPED_TSTRING>
{
  "\")" {
    STRING_1GROW ('"');
    STRING_ಮುಕ್ತಾಯ ();
    BEGIN INITIAL;
    loc->ಆರಂಭ = token_ಆರಂಭ;
    complain (loc, Wyacc,
              _("POSIX Yacc does not support string literals"));
    RETURN_ಮೌಲ್ಯ (TSTRING, ಕೊನೆಯ_string);
  }
  <<EOF>>   unexpected_eof (token_ಆರಂಭ, "\")");
  "\n"      unexpected_newline (token_ಆರಂಭ, "\")");
}



  /*----------------------------------------------------------.
  | Scanning a Bison character literal, decoding its escapes. |
  | The initial quote is already eaten.                       |
  `----------------------------------------------------------*/

<SC_ESCAPED_CHARACTER>
{
  "'" {
    STRING_ಮುಕ್ತಾಯ ();
    BEGIN INITIAL;
    loc->ಆರಂಭ = token_ಆರಂಭ;

    ಆದರೆ (ಕೊನೆಯ_string[0] == '\0')
      {
        complain (loc, complaint, _("empty character literal"));
        STRING_FREE ();
        ಹಿಂತಿರುಗಿ GRAM_error;
      }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಕೊನೆಯ_string[1] != '\0')
      {
        complain (loc, complaint, _("extra characters in character literal"));
        STRING_FREE ();
        ಹಿಂತಿರುಗಿ GRAM_error;
      }
    ಇಲ್ಲದಿದ್ದರೆ
      {
        val->CHAR_LITERAL = ಕೊನೆಯ_string[0];
        STRING_FREE ();
        ಹಿಂತಿರುಗಿ CHAR_LITERAL;
      }
  }
  {eol}     unexpected_newline (token_ಆರಂಭ, "'");
  <<EOF>>   unexpected_eof (token_ಆರಂಭ, "'");
}



  /*--------------------------------------------------------------.
  | Scanning a tag.  The initial angle bracket is already eaten.  |
  `--------------------------------------------------------------*/

<SC_ಟ್ಯಾಗ್>
{
  ">" {
    --nesting;
    ಆದರೆ (nesting < 0)
      {
        STRING_ಮುಕ್ತಾಯ ();
        loc->ಆರಂಭ = token_ಆರಂಭ;
        val->ಟ್ಯಾಗ್_2 = uniqstr_new (ಕೊನೆಯ_string);
        STRING_FREE ();
        BEGIN INITIAL;
        ಹಿಂತಿರುಗಿ ಟ್ಯಾಗ್_2;
      }
    STRING_GROW ();
  }

  ([^<>]|->)+ STRING_GROW ();
  "<"+   STRING_GROW (); nesting += yyleng;

  <<EOF>>   unexpected_eof (token_ಆರಂಭ, ">");
}

  /*----------------------------.
  | Decode escaped characters.  |
  `----------------------------*/

<SC_ESCAPED_CHARACTER,SC_ESCAPED_STRING,SC_ESCAPED_TSTRING>
{
  \\[0-7]{1,3} {
    STRING_GROW_ESCAPE (strtol (yytext + 1, ಖಾಲಿ, 8));
  }

  \\x[0-9abcdefABCDEF]+ {
    STRING_GROW_ESCAPE (strtol (yytext + 2, ಖಾಲಿ, 16));
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
    STRING_GROW_ESCAPE (convert_ucn_ಗಮ್ಯ_byte (yytext));
  }

  \\(.|{eol})      {
    ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *p = yytext + 1;
    /* Quote only if escaping won't make the character visible.  */
    ಆದರೆ (c_isspace ((ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ) *p) && c_isprint ((ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ) *p))
      p = quote (p);
    ಇಲ್ಲದಿದ್ದರೆ
      p = quotearg_style_mem (escape_quoting_style, p, 1);
    complain (loc, complaint, _("invalid character after \\-escape: %s"),
              p);
    STRING_1GROW ('?');
  }

  "\\"             {
    // None of the other rules matched: the last character of this
    // file is "\".  But Flex does not support "\\<<EOF>>".
    unexpected_eof (token_ಆರಂಭ,
                    YY_ಆರಂಭ == SC_ESCAPED_CHARACTER ? "?'"
                    : YY_ಆರಂಭ == SC_ESCAPED_STRING ? "?\""
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
  {eol}         unexpected_newline (token_ಆರಂಭ, "'");
  <<EOF>>       unexpected_eof (token_ಆರಂಭ, "'");
}

<SC_STRING>
{
  "\""          STRING_GROW (); BEGIN context_state;
  {eol}         unexpected_newline (token_ಆರಂಭ, "\"");
  <<EOF>>       unexpected_eof (token_ಆರಂಭ, "\"");
}


  /*---------------------------------------------------.
  | Strings, comments etc. can be found in user code.  |
  `---------------------------------------------------*/

<SC_BRACED_ಸಂಕೇತ,SC_PROLOGUE,SC_EPILOGUE,SC_PREDICATE>
{
  "'" {
    STRING_GROW ();
    context_state = YY_ಆರಂಭ;
    token_ಆರಂಭ = loc->ಆರಂಭ;
    BEGIN SC_CHARACTER;
  }
  "\"" {
    STRING_GROW ();
    context_state = YY_ಆರಂಭ;
    token_ಆರಂಭ = loc->ಆರಂಭ;
    BEGIN SC_STRING;
  }
  "/"{splice}"*" {
    STRING_GROW ();
    context_state = YY_ಆರಂಭ;
    token_ಆರಂಭ = loc->ಆರಂಭ;
    BEGIN SC_COMMENT;
  }
  "/"{splice}"/" {
    STRING_GROW ();
    context_state = YY_ಆರಂಭ;
    BEGIN SC_ಸಾಲು_COMMENT;
  }
}



  /*-----------------------------------------------------------.
  | Scanning some code in braces (actions, predicates). The    |
  | initial "{" is already eaten.                              |
  `-----------------------------------------------------------*/

<SC_BRACED_ಸಂಕೇತ,SC_PREDICATE>
{
  "{"|"<"{splice}"%"  STRING_GROW (); nesting++;
  "%"{splice}">"      STRING_GROW (); nesting--;

  /* Tokenize '<<%' correctly (as '<<' '%') rather than incorrectly
     (as '<' '<%').  */
  "<"{splice}"<"  STRING_GROW ();

  <<EOF>>   unexpected_eof (ಸಂಕೇತ_ಆರಂಭ, "}");
}

<SC_BRACED_ಸಂಕೇತ>
{
  "}" {
    STRING_1GROW ('}');

    --nesting;
    ಆದರೆ (nesting < 0)
      {
        STRING_ಮುಕ್ತಾಯ ();
        loc->ಆರಂಭ = ಸಂಕೇತ_ಆರಂಭ;
        BEGIN INITIAL;
        RETURN_ಮೌಲ್ಯ (BRACED_ಸಂಕೇತ, ಕೊನೆಯ_string);
      }
  }
}

<SC_PREDICATE>
{
  "}" {
    --nesting;
    ಆದರೆ (nesting < 0)
      {
        STRING_ಮುಕ್ತಾಯ ();
        loc->ಆರಂಭ = ಸಂಕೇತ_ಆರಂಭ;
        BEGIN INITIAL;
        RETURN_ಮೌಲ್ಯ (BRACED_PREDICATE, ಕೊನೆಯ_string);
      }
    ಇಲ್ಲದಿದ್ದರೆ
      STRING_1GROW ('}');
  }
}

  /*--------------------------------------------------------------.
  | Scanning some prologue: from "%{" (already scanned) to "%}".  |
  `--------------------------------------------------------------*/

<SC_PROLOGUE>
{
  "%}" {
    STRING_ಮುಕ್ತಾಯ ();
    loc->ಆರಂಭ = ಸಂಕೇತ_ಆರಂಭ;
    BEGIN INITIAL;
    RETURN_ಮೌಲ್ಯ (PROLOGUE, ಕೊನೆಯ_string);
  }

  <<EOF>>   unexpected_eof (ಸಂಕೇತ_ಆರಂಭ, "%}");
}


  /*---------------------------------------------------------------.
  | Scanning the epilogue (everything after the second "%%", which |
  | has already been eaten).                                       |
  `---------------------------------------------------------------*/

<SC_EPILOGUE>
{
  <<EOF>> {
    STRING_ಮುಕ್ತಾಯ ();
    loc->ಆರಂಭ = ಸಂಕೇತ_ಆರಂಭ;
    BEGIN INITIAL;
    RETURN_ಮೌಲ್ಯ (EPILOGUE, ಕೊನೆಯ_string);
  }
}


  /*-----------------------------------------------------.
  | By default, grow the string obstack with the input.  |
  `-----------------------------------------------------*/

<SC_COMMENT,SC_ಸಾಲು_COMMENT,SC_BRACED_ಸಂಕೇತ,SC_PREDICATE,SC_PROLOGUE,SC_EPILOGUE,SC_STRING,SC_CHARACTER,SC_ESCAPED_CHARACTER,SC_ESCAPED_STRING,SC_ESCAPED_TSTRING>
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

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ಪರಿಶೀಲನೆ_integer (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸಂಖ್ಯೆ, ಪೂರ್ಣಾಂಕ ತಳ, location loc)
{
  verify (ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ < ULONG_MAX);
  ಆದರೆ (ತಳ == 16)
    complain (&loc, Wyacc,
              _("POSIX Yacc does not support hexadecimal literals"));

  errno = 0;
  ದೀರ್ಘ ಅಂಕೆ = strtol (ಸಂಖ್ಯೆ, ಖಾಲಿ, ತಳ);

  ಆದರೆ (! (0 <= ಅಂಕೆ && ಅಂಕೆ <= ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ && errno == 0))
    {
      complain (&loc, complaint, _("integer out of range: %s"),
                quote (ಸಂಖ್ಯೆ));
      ಅಂಕೆ = ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ;
    }

  ಹಿಂತಿರುಗಿ ಅಂಕೆ;
}


/*------------------------------------------------------------------.
| Convert universal character name UCN to a single-byte character,  |
| and return that character.  Return -1 if UCN does not correspond  |
| to a single-byte character.                                       |
`------------------------------------------------------------------*/

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
convert_ucn_ಗಮ್ಯ_byte (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ucn)
{
  verify (UCHAR_MAX <= ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ);
  ದೀರ್ಘ ಸಂಕೇತ_2 = strtol (ucn + 2, ಖಾಲಿ, 16);

  /* FIXME: Currently we assume Unicode-compatible unibyte characters
     on ASCII hosts (i.e., Latin-1 on hosts with 8-bit bytes).  On
     non-ASCII hosts we support only the portable C character set.
     These limitations should be removed once we add support for
     multibyte characters.  */

  ಆದರೆ (! (0 <= ಸಂಕೇತ_2 && ಸಂಕೇತ_2 <= UCHAR_MAX))
    ಹಿಂತಿರುಗಿ -1;

#ಆದರೆ ! ('$' == 0x24 && '@' == 0x40 && '`' == 0x60 && '~' == 0x7e)
  {
    /* A non-ASCII host.  Use CODE to index into a table of the C
       basic execution character set, which is guaranteed to exist on
       all Standard C platforms.  This table also includes '$', '@',
       and '`', which are not in the basic execution character set but
       which are unibyte characters on all the platforms that we know
       about.  */
    ಸ್ಥಿರ ಚಿಹ್ನೆಯುಳ್ಳ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ ಕೋಷ್ಟಕ[] =
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

    ಸಂಕೇತ_2 = ಸಂಕೇತ_2 < ಗಾತ್ರ ಕೋಷ್ಟಕ ? ಕೋಷ್ಟಕ[code] : -1;
  }
#ಆದರೆ_ಅಂತ್ಯ

  ಹಿಂತಿರುಗಿ ಸಂಕೇತ_2;
}


/*----------------------------------------------------------------------------.
| Handle '#line INT_LITERAL( "FILE")?\n'.  ARGS has already skipped '#line '. |
`----------------------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
handle_syncline (ಅಕ್ಷರ *ವಾದಗಳು, location loc)
{
  ಅಕ್ಷರ *file;
  errno = 0;
  ದೀರ್ಘ lineno = strtol (ವಾದಗಳು, &file, 10);
  ಆದರೆ (! (0 <= lineno && lineno <= ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ && errno == 0))
    {
      complain (&loc, Wother, _("line number overflow"));
      lineno = ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ;
    }

  file = strchr (file, '"');
  ಆದರೆ (file)
    {
      *strchr (file + 1, '"') = '\0';
      current_file = uniqstr_new (file + 1);
    }
  boundary_ಗಣ (&scanner_cursor, current_file, lineno, 1, 1);
}


/*----------------------------------------------------------------.
| For a token or comment starting at START, report message MSGID, |
| which should say that an end marker was found before the        |
| expected TOKEN_END. Then, pretend that TOKEN_END was found.     |
`----------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
unexpected_ಕೊನೆ (boundary ಆರಂಭ, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *msgid, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *token_ಕೊನೆ)
{
  location loc;
  loc.ಆರಂಭ = ಆರಂಭ;
  loc.ಕೊನೆ = scanner_cursor;
  size_t i = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (token_ಕೊನೆ);

  /* Adjust scanner cursor so that any later message does not count
     the characters about to be inserted.  */
  scanner_cursor.ಸ್ತಂಭ -= i;
  scanner_cursor.byte -= i;

  ಆಗ (i != 0)
    unput (token_ಕೊನೆ[--i]);

  token_ಕೊನೆ = quote (token_ಕೊನೆ);
  /* Instead of '\'', display "'".  */
  ಆದರೆ (STREQ (token_ಕೊನೆ, "'\\''"))
    token_ಕೊನೆ = "\"'\"";
  complain (&loc, complaint, msgid, token_ಕೊನೆ);
}


/*------------------------------------------------------------------------.
| Report an unexpected EOF in a token or comment starting at START.       |
| An end of file was encountered and the expected TOKEN_END was missing.  |
| After reporting the problem, pretend that TOKEN_END was found.          |
`------------------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
unexpected_eof (boundary ಆರಂಭ, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *token_ಕೊನೆ)
{
  unexpected_ಕೊನೆ (ಆರಂಭ, _("missing %s at end of file"), token_ಕೊನೆ);
}


/*----------------------------------------.
| Likewise, but for unexpected newlines.  |
`----------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
unexpected_newline (boundary ಆರಂಭ, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *token_ಕೊನೆ)
{
  unexpected_ಕೊನೆ (ಆರಂಭ, _("missing %s at end of line"), token_ಕೊನೆ);
}


ಶೂನ್ಯ
gram_scanner_open (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *gram)
{
  gram__flex_debug = trace_flag & trace_ಪರಿಶೀಲನೆ;
  gram_debug = trace_flag & trace_parse;
  obstack_init (&obstack_for_string);
  current_file = gram;
  gram_ಒಳ = xfopen (gram, "r");
}


ಶೂನ್ಯ
gram_scanner_close (ಶೂನ್ಯ)
{
  xfclose (gram_ಒಳ);
  /* Reclaim Flex's buffers.  */
  yylex_destroy ();
}


ಶೂನ್ಯ
gram_scanner_free (ಶೂನ್ಯ)
{
  obstack_free (&obstack_for_string, 0);
}
