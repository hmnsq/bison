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
%option prefix="gram_" outfile="lex.yy.c"

%{
#অন্তর্ভাব <c-ctype.h>
#অন্তর্ভাব <errno.h>
#অন্তর্ভাব <mbswidth.h>
#অন্তর্ভাব <quote.h>
#অন্তর্ভাব <quotearg.h>

#অন্তর্ভাব "src/complain.h"
#অন্তর্ভাব "src/files.h"
#অন্তর্ভাব "src/getargs.h"
#অন্তর্ভাব "src/gram.h"
#অন্তর্ভাব "src/reader.h"
#অন্তর্ভাব "src/scan-gram.h"
#অন্তর্ভাব "src/uniqstr.h"

#define FLEX_PREFIX(Id) gram_ ## Id
#অন্তর্ভাব "src/flex-scanner.h"

/* Work around a bug in flex 2.5.31.  See Debian bug 333231
   <https://bugs.debian.org/333231>.  */
#undef gram_wrap
#define gram_wrap() 1

#define YY_DECL GRAM_LEX_DECL

/* Location of scanner cursor.  */
স্থির boundary scanner_cursor;

#define YY_USER_ACTION  location_compute (loc, &scanner_cursor, yytext, yyleng);

/* Report that yytext is an extension, and evaluate to its token kind.  */
#define BISON_DIRECTIVE(Directive)                              \
  (bison_directive (loc, yytext), PERCENT_ ## Directive)

#define RETURN_PERCENT_PARAM(মান_4)             \
  RETURN_মান(PERCENT_PARAM, param_ ## মান_4)

#define RETURN_PERCENT_FLAG(মান_4)                      \
  RETURN_মান(PERCENT_FLAG, uniqstr_new (মান_4))

#define RETURN_মান(Token, মান_4)              \
  কুরু {                                          \
    val->Token = মান_4;                         \
    প্রত্যাগম Token;                               \
  } যাবত্ (0)

#define ROLLBACK_CURRENT_TOKEN                                  \
  কুরু {                                                          \
    scanner_cursor.column -= mbsnwidth (yytext, yyleng, 0);     \
    scanner_cursor.byte -= yyleng;                              \
    yyless (0);                                                 \
  } যাবত্ (0)

#define DEPRECATED_DIRECTIVE(বার্তা_2)                               \
  কুরু {                                                          \
    deprecated_directive (loc, yytext, বার্তা_2);                    \
    scanner_cursor.column -= mbsnwidth (বার্তা_2, strlen (বার্তা_2), 0);  \
    scanner_cursor.byte -= strlen (বার্তা_2);                        \
    প্রতি (size_t i = strlen (বার্তা_2); i != 0; --i)                  \
      unput (বার্তা_2[i - 1]);                                       \
  } যাবত্ (0)


#define STRING_GROW_ESCAPE(Char)                                \
  কুরু {                                                          \
    verify (UCHAR_MAX < ULONG_MAX);                             \
    long c = Char;                                              \
    bool valid = 0 < c && c <= UCHAR_MAX;                       \
    যদি (!valid)                                                 \
      complain (loc, complaint,                                 \
                _("invalid number after \\-escape: %s"),        \
                yytext + 1);                                    \
    যদি (YY_START == SC_ESCAPED_CHARACTER)                       \
      STRING_1GROW (valid ? c : '?');                           \
    অন্যথা                                                        \
      STRING_GROW ();                                           \
  } যাবত্ (0)


/* The current file name.  Might change with #line.  */
স্থির uniqstr current_file = রিক্ত;

/* A string representing the most recently saved token.  */
স্থির বর্ণ *last_string = রিক্ত;

/* Bracketed identifier. */
স্থির uniqstr bracketed_id_str = রিক্ত;
স্থির location bracketed_id_loc;
স্থির boundary bracketed_id_start;
স্থির পূর্ণাঙ্ক bracketed_id_context_state = 0;

শূন্য
gram_scanner_last_string_free (শূন্য)
{
  STRING_FREE ();
}

স্থির শূন্য handle_syncline (বর্ণ *, location);
স্থির পূর্ণাঙ্ক scan_integer (বর্ণ ধ্রুব *p, পূর্ণাঙ্ক base, location loc);
স্থির পূর্ণাঙ্ক convert_ucn_to_byte (বর্ণ ধ্রুব *hex_text);
স্থির শূন্য unexpected_eof (boundary, বর্ণ ধ্রুব *);
স্থির শূন্য unexpected_newline (boundary, বর্ণ ধ্রুব *);

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
%x SC_TAG

 /* Four types of user code:
    - prologue (code between '%{' '%}' in the first section, before %%);
    - actions, printers, union, etc, (between braced in the middle section);
    - epilogue (everything after the second %%).
    - predicate (code between '%?{' and '{' in middle section); */
%x SC_PROLOGUE SC_BRACED_CODE SC_EPILOGUE SC_PREDICATE
 /* C and C++ comments in code. */
%x SC_COMMENT SC_সারি_COMMENT
 /* Strings and characters in code. */
%x SC_STRING SC_CHARACTER
 /* Bracketed identifiers support. */
%x SC_BRACKETED_ID SC_RETURN_BRACKETED_ID

letter    [.abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_]
id        {letter}({letter}|[-0-9])*
পূর্ণাঙ্ক       [0-9]+
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
  পূর্ণাঙ্ক nesting PACIFY_CC (= 0);

  /* Parent context state, when applicable.  */
  পূর্ণাঙ্ক context_state PACIFY_CC (= 0);

  /* Location of most recent identifier, when applicable.  */
  location id_loc PACIFY_CC (= empty_loc);

  /* Where containing code started, when applicable.  Its initial
     value is relevant only when yylex is invoked in the SC_EPILOGUE
     start condition.  */
  boundary code_start = scanner_cursor;

  /* Where containing comment or string or character literal started,
     when applicable.  */
  boundary token_start PACIFY_CC (= scanner_cursor);

  /* We cannot trust YY_USER_INIT, whose semantics changes over time
     (it moved in Flex 2.5.38).  */
  স্থির bool first = true;
  যদি (first)
    {
      scanner_cursor = loc->start;
      first = false;
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
  "//".*       অনুবর্ত;
  "/*" {
    token_start = loc->start;
    context_state = YY_START;
    BEGIN SC_YACC_COMMENT;
  }

  ^"#line "{পূর্ণাঙ্ক}(" \"".*"\"")?{eol} {
    handle_syncline (yytext + পরিমাণ "#line " - 1, *loc);
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
  "%binary"                         প্রত্যাগম BISON_DIRECTIVE (NONASSOC);
  "%code"                           প্রত্যাগম BISON_DIRECTIVE (CODE);
  "%debug"                          RETURN_PERCENT_FLAG ("parse.trace");
  "%default-prec"                   প্রত্যাগম BISON_DIRECTIVE (DEFAULT_PREC);
  "%define"                         প্রত্যাগম BISON_DIRECTIVE (DEFINE);
  "%defines"                        প্রত্যাগম BISON_DIRECTIVE (HEADER); // Deprecated in 3.8.
  "%destructor"                     প্রত্যাগম BISON_DIRECTIVE (DESTRUCTOR);
  "%dprec"                          প্রত্যাগম BISON_DIRECTIVE (DPREC);
  "%empty"                          প্রত্যাগম BISON_DIRECTIVE (EMPTY);
  "%expect"                         প্রত্যাগম BISON_DIRECTIVE (EXPECT);
  "%expect-rr"                      প্রত্যাগম BISON_DIRECTIVE (EXPECT_RR);
  "%file-prefix"                    RETURN_মান (PERCENT_FILE_PREFIX, uniqstr_new (yytext));
  "%glr-parser"                     প্রত্যাগম BISON_DIRECTIVE (GLR_PARSER);
  "%header"                         প্রত্যাগম BISON_DIRECTIVE (HEADER);
  "%initial-action"                 প্রত্যাগম BISON_DIRECTIVE (INITIAL_ACTION);
  "%language"                       প্রত্যাগম BISON_DIRECTIVE (LANGUAGE);
  "%left"                           প্রত্যাগম PERCENT_বাম;
  "%lex-param"                      RETURN_PERCENT_PARAM (lex);
  "%locations"                      RETURN_PERCENT_FLAG ("locations");
  "%merge"                          প্রত্যাগম BISON_DIRECTIVE (MERGE);
  "%no-default-prec"                প্রত্যাগম BISON_DIRECTIVE (NO_DEFAULT_PREC);
  "%no-lines"                       প্রত্যাগম BISON_DIRECTIVE (NO_সারিগুলি);
  "%nonassoc"                       প্রত্যাগম PERCENT_NONASSOC;
  "%nondeterministic-parser"        প্রত্যাগম BISON_DIRECTIVE (NONDETERMINISTIC_PARSER);
  "%nterm"                          প্রত্যাগম BISON_DIRECTIVE (NTERM);
  "%output"                         প্রত্যাগম BISON_DIRECTIVE (OUTPUT);
  "%param"                          RETURN_PERCENT_PARAM (both);
  "%parse-param"                    RETURN_PERCENT_PARAM (parse);
  "%prec"                           প্রত্যাগম PERCENT_PREC;
  "%precedence"                     প্রত্যাগম BISON_DIRECTIVE (PRECEDENCE);
  "%printer"                        প্রত্যাগম BISON_DIRECTIVE (PRINTER);
  "%require"                        প্রত্যাগম BISON_DIRECTIVE (REQUIRE);
  "%right"                          প্রত্যাগম PERCENT_ডান;
  "%skeleton"                       প্রত্যাগম BISON_DIRECTIVE (SKELETON);
  "%start"                          প্রত্যাগম PERCENT_START;
  "%term"                           প্রত্যাগম BISON_DIRECTIVE (TOKEN);
  "%token"                          প্রত্যাগম PERCENT_TOKEN;
  "%token-table"                    প্রত্যাগম BISON_DIRECTIVE (TOKEN_TABLE);
  "%type"                           প্রত্যাগম PERCENT_TYPE;
  "%union"                          প্রত্যাগম PERCENT_UNION;
  "%verbose"                        প্রত্যাগম BISON_DIRECTIVE (VERBOSE);
  "%yacc"                           প্রত্যাগম PERCENT_YACC;

  /* Deprecated since Bison 2.3b (2008-05-27), but the warning is
     issued only since Bison 3.4. */
  "%pure"[-_]"parser"               RETURN_মান (PERCENT_PURE_PARSER, uniqstr_new (yytext));

  /* Deprecated since Bison 3.0 (2013-07-25), but the warning is
     issued only since Bison 3.3. */
  "%error-verbose"                  RETURN_মান (PERCENT_ERROR_VERBOSE, uniqstr_new (yytext));

  /* Deprecated since Bison 2.6 (2012-07-19), but the warning is
     issued only since Bison 3.3. */
  "%name"[-_]"prefix"{eqopt}{sp}    RETURN_মান (PERCENT_NAME_PREFIX, uniqstr_new (yytext));

  /* Deprecated since Bison 2.7.90, 2012. */
  "%default"[-_]"prec"              DEPRECATED_DIRECTIVE ("%default-prec");
  "%error"[-_]"verbose"             RETURN_মান (PERCENT_ERROR_VERBOSE, uniqstr_new (yytext));
  "%expect"[-_]"rr"                 DEPRECATED_DIRECTIVE ("%expect-rr");
  "%file-prefix"{eqopt}             RETURN_মান (PERCENT_FILE_PREFIX, uniqstr_new (yytext));
  "%fixed"[-_]"output"[-_]"files"   DEPRECATED_DIRECTIVE ("%output \"y.tab.c\"");
  "%no"[-_]"default"[-_]"prec"      DEPRECATED_DIRECTIVE ("%no-default-prec");
  "%no"[-_]"lines"                  DEPRECATED_DIRECTIVE ("%no-lines");
  "%output"{eqopt}                  DEPRECATED_DIRECTIVE ("%output");
  "%token"[-_]"table"               DEPRECATED_DIRECTIVE ("%token-table");

  "%"{id} {
    complain (loc, complaint, _("invalid directive: %s"), quote (yytext));
    প্রত্যাগম GRAM_error;
  }

  ":"                     প্রত্যাগম COLON;
  "="                     প্রত্যাগম EQUAL;
  "|"                     প্রত্যাগম PIPE;
  ";"                     প্রত্যাগম SEMICOLON;

  {id} {
    val->ID = uniqstr_new (yytext);
    id_loc = *loc;
    bracketed_id_str = রিক্ত;
    BEGIN SC_AFTER_IDENTIFIER;
  }

  {পূর্ণাঙ্ক}      RETURN_মান (INT_LITERAL, scan_integer (yytext, 10, *loc));
  {xint}     RETURN_মান (INT_LITERAL, scan_integer (yytext, 16, *loc));

  /* Identifiers may not start with a digit.  Yet, don't silently
     accept "1FOO" as "1 FOO".  */
  {পূর্ণাঙ্ক}{id} {
    complain (loc, complaint, _("invalid identifier: %s"), quote (yytext));
    প্রত্যাগম GRAM_error;
  }

  /* Characters.  */
  "'"         token_start = loc->start; BEGIN SC_ESCAPED_CHARACTER;

  /* Strings. */
  "\""        token_start = loc->start; STRING_1GROW ('"'); BEGIN SC_ESCAPED_STRING;
  "_(\""      token_start = loc->start; STRING_1GROW ('"'); BEGIN SC_ESCAPED_TSTRING;

  /* Prologue. */
  "%{"        code_start = loc->start; BEGIN SC_PROLOGUE;

  /* Code in between braces.  */
  "{" {
    STRING_GROW ();
    nesting = 0;
    code_start = loc->start;
    BEGIN SC_BRACED_CODE;
  }

  /* Semantic predicate. */
  "%?"([ \f\t\v]|{eol})*"{" {
    nesting = 0;
    code_start = loc->start;
    BEGIN SC_PREDICATE;
  }

  /* A type. */
  "<*>"       প্রত্যাগম TAG_ANY;
  "<>"        প্রত্যাগম TAG_NONE;
  "<"         {
    nesting = 0;
    token_start = loc->start;
    BEGIN SC_TAG;
  }

  "%%" {
    স্থির পূর্ণাঙ্ক percent_percent_গুনতি;
    যদি (++percent_percent_গুনতি == 2)
      BEGIN SC_EPILOGUE;
    প্রত্যাগম PERCENT_PERCENT;
  }

  "[" {
    bracketed_id_str = রিক্ত;
    bracketed_id_start = loc->start;
    bracketed_id_context_state = YY_START;
    BEGIN SC_BRACKETED_ID;
  }

  [^\[%A-Za-z0-9_<>{}""''*;|=/, \f\r\n\t\v]+|. {
    complain (loc, complaint, "%s: %s",
              ngettext ("invalid character", "invalid characters", yyleng),
              quote_mem (yytext, yyleng));
    প্রত্যাগম GRAM_error;
  }

  <<EOF>> {
    loc->start = loc->end = scanner_cursor;
    yyterminate ();
  }
}


  /*--------------------------------------------------------------.
  | Supporting \0 complexifies our implementation for no expected |
  | added value.                                                  |
  `--------------------------------------------------------------*/

<SC_ESCAPED_CHARACTER,SC_ESCAPED_STRING,SC_ESCAPED_TSTRING,SC_TAG>
{
  \0         {
    complain (loc, complaint, _("invalid null character"));
    STRING_FINISH ();
    STRING_FREE ();
    প্রত্যাগম GRAM_error;
  }
}


  /*-----------------------------------------------------------------.
  | Scanning after an identifier, checking whether a colon is next.  |
  `-----------------------------------------------------------------*/

<SC_AFTER_IDENTIFIER>
{
  "[" {
    যদি (bracketed_id_str)
      {
        ROLLBACK_CURRENT_TOKEN;
        BEGIN SC_RETURN_BRACKETED_ID;
        *loc = id_loc;
        প্রত্যাগম ID;
      }
    অন্যথা
      {
        bracketed_id_start = loc->start;
        bracketed_id_context_state = YY_START;
        BEGIN SC_BRACKETED_ID;
      }
  }
  ":" {
    ROLLBACK_CURRENT_TOKEN;
    BEGIN (bracketed_id_str ? SC_RETURN_BRACKETED_ID : INITIAL);
    *loc = id_loc;
    প্রত্যাগম ID_COLON;
  }
  . {
    ROLLBACK_CURRENT_TOKEN;
    BEGIN (bracketed_id_str ? SC_RETURN_BRACKETED_ID : INITIAL);
    *loc = id_loc;
    প্রত্যাগম ID;
  }
  <<EOF>> {
    BEGIN (bracketed_id_str ? SC_RETURN_BRACKETED_ID : INITIAL);
    *loc = id_loc;
    প্রত্যাগম ID;
  }
}

  /*--------------------------------.
  | Scanning bracketed identifiers. |
  `--------------------------------*/

<SC_BRACKETED_ID>
{
  {id} {
    যদি (bracketed_id_str)
      {
        complain (loc, complaint,
                  _("unexpected identifier in bracketed name: %s"),
                  quote (yytext));
        প্রত্যাগম GRAM_error;
      }
    অন্যথা
      {
        bracketed_id_str = uniqstr_new (yytext);
        bracketed_id_loc = *loc;
      }
  }
  "]" {
    BEGIN bracketed_id_context_state;
    যদি (bracketed_id_str)
      {
        যদি (INITIAL == bracketed_id_context_state)
          {
            val->BRACKETED_ID = bracketed_id_str;
            bracketed_id_str = 0;
            *loc = bracketed_id_loc;
            প্রত্যাগম BRACKETED_ID;
          }
      }
    অন্যথা
      {
        complain (loc, complaint, _("an identifier expected"));
        প্রত্যাগম GRAM_error;
      }
  }

  [^\].A-Za-z0-9_/ \f\r\n\t\v]+|. {
    complain (loc, complaint, "%s: %s",
              ngettext ("invalid character in bracketed name",
                        "invalid characters in bracketed name", yyleng),
              quote_mem (yytext, yyleng));
    প্রত্যাগম GRAM_error;
  }

  <<EOF>> {
    BEGIN bracketed_id_context_state;
    unexpected_eof (bracketed_id_start, "]");
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
    প্রত্যাগম BRACKETED_ID;
  }
}


  /*---------------------------------------------------------------.
  | Scanning a Yacc comment.  The initial '/ *' is already eaten.  |
  `---------------------------------------------------------------*/

<SC_YACC_COMMENT>
{
  "*/"     BEGIN context_state;
  .|{eol}  অনুবর্ত;
  <<EOF>>  unexpected_eof (token_start, "*/"); BEGIN context_state;
}


  /*------------------------------------------------------------.
  | Scanning a C comment.  The initial '/ *' is already eaten.  |
  `------------------------------------------------------------*/

<SC_COMMENT>
{
  "*"{splice}"/"  STRING_GROW (); BEGIN context_state;
  <<EOF>>         unexpected_eof (token_start, "*/"); BEGIN context_state;
}


  /*--------------------------------------------------------------.
  | Scanning a line comment.  The initial '//' is already eaten.  |
  `--------------------------------------------------------------*/

<SC_সারি_COMMENT>
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
    STRING_FINISH ();
    BEGIN INITIAL;
    loc->start = token_start;
    complain (loc, Wyacc,
              _("POSIX Yacc does not support string literals"));
    RETURN_মান (STRING, last_string);
  }
  <<EOF>>   unexpected_eof (token_start, "\"");
  "\n"      unexpected_newline (token_start, "\"");
}

<SC_ESCAPED_TSTRING>
{
  "\")" {
    STRING_1GROW ('"');
    STRING_FINISH ();
    BEGIN INITIAL;
    loc->start = token_start;
    complain (loc, Wyacc,
              _("POSIX Yacc does not support string literals"));
    RETURN_মান (TSTRING, last_string);
  }
  <<EOF>>   unexpected_eof (token_start, "\")");
  "\n"      unexpected_newline (token_start, "\")");
}



  /*----------------------------------------------------------.
  | Scanning a Bison character literal, decoding its escapes. |
  | The initial quote is already eaten.                       |
  `----------------------------------------------------------*/

<SC_ESCAPED_CHARACTER>
{
  "'" {
    STRING_FINISH ();
    BEGIN INITIAL;
    loc->start = token_start;

    যদি (last_string[0] == '\0')
      {
        complain (loc, complaint, _("empty character literal"));
        STRING_FREE ();
        প্রত্যাগম GRAM_error;
      }
    অন্যথা যদি (last_string[1] != '\0')
      {
        complain (loc, complaint, _("extra characters in character literal"));
        STRING_FREE ();
        প্রত্যাগম GRAM_error;
      }
    অন্যথা
      {
        val->CHAR_LITERAL = last_string[0];
        STRING_FREE ();
        প্রত্যাগম CHAR_LITERAL;
      }
  }
  {eol}     unexpected_newline (token_start, "'");
  <<EOF>>   unexpected_eof (token_start, "'");
}



  /*--------------------------------------------------------------.
  | Scanning a tag.  The initial angle bracket is already eaten.  |
  `--------------------------------------------------------------*/

<SC_TAG>
{
  ">" {
    --nesting;
    যদি (nesting < 0)
      {
        STRING_FINISH ();
        loc->start = token_start;
        val->TAG = uniqstr_new (last_string);
        STRING_FREE ();
        BEGIN INITIAL;
        প্রত্যাগম TAG;
      }
    STRING_GROW ();
  }

  ([^<>]|->)+ STRING_GROW ();
  "<"+   STRING_GROW (); nesting += yyleng;

  <<EOF>>   unexpected_eof (token_start, ">");
}

  /*----------------------------.
  | Decode escaped characters.  |
  `----------------------------*/

<SC_ESCAPED_CHARACTER,SC_ESCAPED_STRING,SC_ESCAPED_TSTRING>
{
  \\[0-7]{1,3} {
    STRING_GROW_ESCAPE (strtol (yytext + 1, রিক্ত, 8));
  }

  \\x[0-9abcdefABCDEF]+ {
    STRING_GROW_ESCAPE (strtol (yytext + 2, রিক্ত, 16));
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
    STRING_GROW_ESCAPE (convert_ucn_to_byte (yytext));
  }

  \\(.|{eol})      {
    বর্ণ ধ্রুব *p = yytext + 1;
    /* Quote only if escaping won't make the character visible.  */
    যদি (c_isspace ((unsigned বর্ণ) *p) && c_isprint ((unsigned বর্ণ) *p))
      p = quote (p);
    অন্যথা
      p = quotearg_style_mem (escape_quoting_style, p, 1);
    complain (loc, complaint, _("invalid character after \\-escape: %s"),
              p);
    STRING_1GROW ('?');
  }

  "\\"             {
    // None of the other rules matched: the last character of this
    // file is "\".  But Flex does not support "\\<<EOF>>".
    unexpected_eof (token_start,
                    YY_START == SC_ESCAPED_CHARACTER ? "?'"
                    : YY_START == SC_ESCAPED_STRING ? "?\""
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
  {eol}         unexpected_newline (token_start, "'");
  <<EOF>>       unexpected_eof (token_start, "'");
}

<SC_STRING>
{
  "\""          STRING_GROW (); BEGIN context_state;
  {eol}         unexpected_newline (token_start, "\"");
  <<EOF>>       unexpected_eof (token_start, "\"");
}


  /*---------------------------------------------------.
  | Strings, comments etc. can be found in user code.  |
  `---------------------------------------------------*/

<SC_BRACED_CODE,SC_PROLOGUE,SC_EPILOGUE,SC_PREDICATE>
{
  "'" {
    STRING_GROW ();
    context_state = YY_START;
    token_start = loc->start;
    BEGIN SC_CHARACTER;
  }
  "\"" {
    STRING_GROW ();
    context_state = YY_START;
    token_start = loc->start;
    BEGIN SC_STRING;
  }
  "/"{splice}"*" {
    STRING_GROW ();
    context_state = YY_START;
    token_start = loc->start;
    BEGIN SC_COMMENT;
  }
  "/"{splice}"/" {
    STRING_GROW ();
    context_state = YY_START;
    BEGIN SC_সারি_COMMENT;
  }
}



  /*-----------------------------------------------------------.
  | Scanning some code in braces (actions, predicates). The    |
  | initial "{" is already eaten.                              |
  `-----------------------------------------------------------*/

<SC_BRACED_CODE,SC_PREDICATE>
{
  "{"|"<"{splice}"%"  STRING_GROW (); nesting++;
  "%"{splice}">"      STRING_GROW (); nesting--;

  /* Tokenize '<<%' correctly (as '<<' '%') rather than incorrectly
     (as '<' '<%').  */
  "<"{splice}"<"  STRING_GROW ();

  <<EOF>>   unexpected_eof (code_start, "}");
}

<SC_BRACED_CODE>
{
  "}" {
    STRING_1GROW ('}');

    --nesting;
    যদি (nesting < 0)
      {
        STRING_FINISH ();
        loc->start = code_start;
        BEGIN INITIAL;
        RETURN_মান (BRACED_CODE, last_string);
      }
  }
}

<SC_PREDICATE>
{
  "}" {
    --nesting;
    যদি (nesting < 0)
      {
        STRING_FINISH ();
        loc->start = code_start;
        BEGIN INITIAL;
        RETURN_মান (BRACED_PREDICATE, last_string);
      }
    অন্যথা
      STRING_1GROW ('}');
  }
}

  /*--------------------------------------------------------------.
  | Scanning some prologue: from "%{" (already scanned) to "%}".  |
  `--------------------------------------------------------------*/

<SC_PROLOGUE>
{
  "%}" {
    STRING_FINISH ();
    loc->start = code_start;
    BEGIN INITIAL;
    RETURN_মান (PROLOGUE, last_string);
  }

  <<EOF>>   unexpected_eof (code_start, "%}");
}


  /*---------------------------------------------------------------.
  | Scanning the epilogue (everything after the second "%%", which |
  | has already been eaten).                                       |
  `---------------------------------------------------------------*/

<SC_EPILOGUE>
{
  <<EOF>> {
    STRING_FINISH ();
    loc->start = code_start;
    BEGIN INITIAL;
    RETURN_মান (EPILOGUE, last_string);
  }
}


  /*-----------------------------------------------------.
  | By default, grow the string obstack with the input.  |
  `-----------------------------------------------------*/

<SC_COMMENT,SC_সারি_COMMENT,SC_BRACED_CODE,SC_PREDICATE,SC_PROLOGUE,SC_EPILOGUE,SC_STRING,SC_CHARACTER,SC_ESCAPED_CHARACTER,SC_ESCAPED_STRING,SC_ESCAPED_TSTRING>
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

স্থির পূর্ণাঙ্ক
scan_integer (বর্ণ ধ্রুব *সংখ্যা, পূর্ণাঙ্ক base, location loc)
{
  verify (INT_MAX < ULONG_MAX);
  যদি (base == 16)
    complain (&loc, Wyacc,
              _("POSIX Yacc does not support hexadecimal literals"));

  errno = 0;
  long অঙ্ক = strtol (সংখ্যা, রিক্ত, base);

  যদি (! (0 <= অঙ্ক && অঙ্ক <= INT_MAX && errno == 0))
    {
      complain (&loc, complaint, _("integer out of range: %s"),
                quote (সংখ্যা));
      অঙ্ক = INT_MAX;
    }

  প্রত্যাগম অঙ্ক;
}


/*------------------------------------------------------------------.
| Convert universal character name UCN to a single-byte character,  |
| and return that character.  Return -1 if UCN does not correspond  |
| to a single-byte character.                                       |
`------------------------------------------------------------------*/

স্থির পূর্ণাঙ্ক
convert_ucn_to_byte (বর্ণ ধ্রুব *ucn)
{
  verify (UCHAR_MAX <= INT_MAX);
  long code = strtol (ucn + 2, রিক্ত, 16);

  /* FIXME: Currently we assume Unicode-compatible unibyte characters
     on ASCII hosts (i.e., Latin-1 on hosts with 8-bit bytes).  On
     non-ASCII hosts we support only the portable C character set.
     These limitations should be removed once we add support for
     multibyte characters.  */

  যদি (! (0 <= code && code <= UCHAR_MAX))
    প্রত্যাগম -1;

#যদি ! ('$' == 0x24 && '@' == 0x40 && '`' == 0x60 && '~' == 0x7e)
  {
    /* A non-ASCII host.  Use CODE to index into a table of the C
       basic execution character set, which is guaranteed to exist on
       all Standard C platforms.  This table also includes '$', '@',
       and '`', which are not in the basic execution character set but
       which are unibyte characters on all the platforms that we know
       about.  */
    স্থির signed বর্ণ ধ্রুব table[] =
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

    code = code < পরিমাণ table ? table[code] : -1;
  }
#endif

  প্রত্যাগম code;
}


/*----------------------------------------------------------------------------.
| Handle '#line INT_LITERAL( "FILE")?\n'.  ARGS has already skipped '#line '. |
`----------------------------------------------------------------------------*/

স্থির শূন্য
handle_syncline (বর্ণ *args, location loc)
{
  বর্ণ *file;
  errno = 0;
  long lineno = strtol (args, &file, 10);
  যদি (! (0 <= lineno && lineno <= INT_MAX && errno == 0))
    {
      complain (&loc, Wother, _("line number overflow"));
      lineno = INT_MAX;
    }

  file = strchr (file, '"');
  যদি (file)
    {
      *strchr (file + 1, '"') = '\0';
      current_file = uniqstr_new (file + 1);
    }
  boundary_set (&scanner_cursor, current_file, lineno, 1, 1);
}


/*----------------------------------------------------------------.
| For a token or comment starting at START, report message MSGID, |
| which should say that an end marker was found before the        |
| expected TOKEN_END. Then, pretend that TOKEN_END was found.     |
`----------------------------------------------------------------*/

স্থির শূন্য
unexpected_end (boundary start, বর্ণ ধ্রুব *msgid, বর্ণ ধ্রুব *token_end)
{
  location loc;
  loc.start = start;
  loc.end = scanner_cursor;
  size_t i = strlen (token_end);

  /* Adjust scanner cursor so that any later message does not count
     the characters about to be inserted.  */
  scanner_cursor.column -= i;
  scanner_cursor.byte -= i;

  যাবত্ (i != 0)
    unput (token_end[--i]);

  token_end = quote (token_end);
  /* Instead of '\'', display "'".  */
  যদি (STREQ (token_end, "'\\''"))
    token_end = "\"'\"";
  complain (&loc, complaint, msgid, token_end);
}


/*------------------------------------------------------------------------.
| Report an unexpected EOF in a token or comment starting at START.       |
| An end of file was encountered and the expected TOKEN_END was missing.  |
| After reporting the problem, pretend that TOKEN_END was found.          |
`------------------------------------------------------------------------*/

স্থির শূন্য
unexpected_eof (boundary start, বর্ণ ধ্রুব *token_end)
{
  unexpected_end (start, _("missing %s at end of file"), token_end);
}


/*----------------------------------------.
| Likewise, but for unexpected newlines.  |
`----------------------------------------*/

স্থির শূন্য
unexpected_newline (boundary start, বর্ণ ধ্রুব *token_end)
{
  unexpected_end (start, _("missing %s at end of line"), token_end);
}


শূন্য
gram_scanner_open (ধ্রুব বর্ণ *gram)
{
  gram__flex_debug = trace_flag & trace_scan;
  gram_debug = trace_flag & trace_parse;
  obstack_init (&obstack_for_string);
  current_file = gram;
  gram_in = xfopen (gram, "r");
}


শূন্য
gram_scanner_close (শূন্য)
{
  xfclose (gram_in);
  /* Reclaim Flex's buffers.  */
  yylex_destroy ();
}


শূন্য
gram_scanner_free (শূন্য)
{
  obstack_free (&obstack_for_string, 0);
}
