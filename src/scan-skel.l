/* Scan Bison Skeletons.                                       -*- C -*-

   Copyright (C) 2001-2015, 2018-2022 Free Software Foundation, Inc.

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

%option nodefault noyywrap noinput nounput never-interactive debug
%option prefix="skel_" outfile="lex.yy.c"

%{
#شامل <dirname.h>
#شامل <error.h>
#شامل <path-join.h>
#شامل <quotearg.h>

#شامل "src/complain.h"
#شامل "src/files.h"
#شامل "src/getargs.h"
#شامل "src/scan-skel.h"

#define FLEX_PREFIX(Id) skel_ ## Id
#شامل "src/flex-scanner.h"

/* Work around a bug in flex 2.5.31.  See Debian bug 333231
   <https://bugs.debian.org/333231>.  */
#undef skel_wrap
#define skel_wrap() 1

#define YY_DECL ایستا صحیح skel_lex (تهی)
YY_DECL;

تعریفنوع تهی (*at_directive)(صحیح, نویسه**, نویسه **, صحیح*);
ایستا تهی at_init (صحیح *argc, نویسه *argv[], at_directive *at_ptr, at_directive fun);
ایستا تهی at_basename (صحیح argc, نویسه *argv[], نویسه**, صحیح*);
ایستا تهی at_complain (صحیح argc, نویسه *argv[], نویسه**, صحیح*);
ایستا تهی at_output (صحیح argc, نویسه *argv[], نویسه **name, صحیح *lineno);
ایستا تهی fail_for_at_directive_too_many_args (نویسه ثابت *at_directive_name);
ایستا تهی fail_for_at_directive_too_few_args (نویسه ثابت *at_directive_name);
ایستا تهی fail_for_invalid_at (نویسه ثابت *at);
ایستا تهی output_mapped_file (نویسه ثابت *name);
%}

/* Identifiers of our M4 macros.  */
macro      [bm]4_[a-zA-Z_0-9]*
/* Safe sequence of word-constituent characters.  */
identifier [A-Za-z_0-9]+

%x SC_AT_DIRECTIVE_ARGS
%x SC_AT_DIRECTIVE_SKIP_WS

%%

%{
  صحیح out_lineno PACIFY_CC (= 0);
  نویسه *out_name = پوچ;

  /* Currently, only the @complain directive takes multiple arguments, and
     never more than 7, with argv[0] being the directive name and argv[1]
     being the type of complaint to dispatch. */
#define ARGC_MAX 9
  صحیح argc = 0;
  نویسه *argv[ARGC_MAX];
  at_directive at_ptr = پوچ;
%}

"@@" fputc ('@', yyout);
"@{" fputc ('[', yyout);
"@}" fputc (']', yyout);
"@'" ادامه;  /* Used by b4_cat in ../data/bison.m4.  */
@\n  ادامه;

"@oline@"  fprintf (yyout, "%d", out_lineno + 1);
"@ofile@"  output_mapped_file (out_name);

"@basename("    at_init (&argc, argv, &at_ptr, &at_basename);
"@complain("    at_init (&argc, argv, &at_ptr, &at_complain);
"@output("      at_init (&argc, argv, &at_ptr, &at_output);

  /* This pattern must not match more than the previous @ patterns. */
@[^@{}''(\n]*   fail_for_invalid_at (yytext);
\n              out_lineno++; ECHO;
[^a-z@\n]+      ECHO;

  /* If there are still identifiers that look like macros, such as
     b4_synbol, this probably an error, say a typo in M4, or
     overquotation.  */
{macro}         {
                  location loc = empty_loc;
                  loc.start.file = map_file_name (out_name);
                  loc.start.سطر = out_lineno;
                  loc.end = loc.start;
                  complain (&loc, Wother,
                            "suspicious sequence in the output: %s", yytext);
                  ECHO;
                }
{identifier}    ECHO;
.               ECHO;

<INITIAL><<EOF>> {
  اگر (out_name)
    {
      آزاد (out_name);
      xfclose (yyout);
    }
  بازگشت EOF;
}

<SC_AT_DIRECTIVE_ARGS>
{
  [^@]+  STRING_GROW ();

  "@@"   STRING_1GROW ('@');
  "@{"   STRING_1GROW ('[');
  "@}"   STRING_1GROW (']');
  "@'"   ادامه; /* For starting an argument that begins with whitespace. */
  @\n    ادامه;

  @[,)] {
    اگر (argc >= ARGC_MAX)
      fail_for_at_directive_too_many_args (argv[0]);

    argv[argc++] = obstack_finish0 (&obstack_for_string);

    /* Like M4, skip whitespace after a comma.  */
    اگر (yytext[1] == ',')
      BEGIN SC_AT_DIRECTIVE_SKIP_WS;
    وگرنه
      {
        aver (at_ptr);
        at_ptr (argc, argv, &out_name, &out_lineno);
        obstack_free (&obstack_for_string, argv[0]);
        argc = 0;
        BEGIN INITIAL;
      }
  }

  @.?  fail_for_invalid_at (yytext);
}

<SC_AT_DIRECTIVE_SKIP_WS>
{
  [ \t\r\n]    ادامه;
  .            yyless (0); BEGIN SC_AT_DIRECTIVE_ARGS;
}

<SC_AT_DIRECTIVE_ARGS,SC_AT_DIRECTIVE_SKIP_WS>
{
  <<EOF>>  complain (پوچ, fatal, _("unclosed %s directive in skeleton"), argv[0]);
}

%%

ایستا تهی
at_init (صحیح *argc, نویسه *argv[], at_directive *at_ptr, at_directive fun)
{
  *at_ptr = fun;
  yytext[yyleng-1] = '\0';
  obstack_grow (&obstack_for_string, yytext, yyleng);
  argv[(*argc)++] = obstack_finish (&obstack_for_string);
  BEGIN SC_AT_DIRECTIVE_ARGS;
}

/*------------------------.
| Scan a Bison skeleton.  |
`------------------------*/

تهی
scan_skel (FILE *in)
{
  ایستا bool initialized = false;
  اگر (!initialized)
    {
      initialized = true;
      obstack_init (&obstack_for_string);
    }
  skel_in = in;
  skel__flex_debug = trace_flag & trace_skeleton;
  skel_lex ();
}

تهی
skel_scanner_free (تهی)
{
  obstack_free (&obstack_for_string, 0);
  /* Reclaim Flex's buffers.  */
  yylex_destroy ();
}

ایستا درونخطی warnings
flag (ثابت نویسه *arg)
{
  /* compare with values issued from b4_error */
  اگر (STREQ (arg, "complain"))
    بازگشت complaint;
  وگرنه اگر (STREQ (arg, "deprecated"))
    بازگشت Wdeprecated;
  وگرنه اگر (STREQ (arg, "fatal"))
    بازگشت fatal;
  وگرنه اگر (STREQ (arg, "note"))
    بازگشت silent | complaint | no_caret | note;
  وگرنه اگر (STREQ (arg, "warn"))
    بازگشت Wother;
  وگرنه
    abort ();
}

ایستا تهی
at_basename (صحیح argc, نویسه *argv[], نویسه **out_namep, صحیح *out_linenop)
{
  (تهی) out_namep;
  (تهی) out_linenop;
  اگر (2 < argc)
    fail_for_at_directive_too_many_args (argv[0]);
  fputs (last_component (argv[1]), yyout);
}

ایستا تهی
at_complain (صحیح argc, نویسه *argv[], نویسه **out_namep, صحیح *out_linenop)
{
  اگر (argc < 4)
    fail_for_at_directive_too_few_args (argv[0]);

  (تهی) out_namep;
  (تهی) out_linenop;

  warnings w = flag (argv[1]);

  location loc;
  location *locp = پوچ;
  اگر (argv[2] && argv[2][0])
    {
      boundary_set_from_string (&loc.start, argv[2]);
      boundary_set_from_string (&loc.end, argv[3]);
      locp = &loc;
    }
  complain_args (locp, w, argc - 4, argv + 4);
}

ایستا تهی
at_output (صحیح argc, نویسه *argv[], نویسه **out_namep, صحیح *out_linenop)
{
  اگر (3 < argc)
    fail_for_at_directive_too_many_args (argv[0]);
  اگر (*out_namep)
    {
      آزاد (*out_namep);
      xfclose (yyout);
    }
  *out_namep = xpath_join (argv[1], 2 < argc ? argv[2] : پوچ);
  output_file_name_check (out_namep, true);
  /* If there were errors, do not generate the output.  */
  yyout = xfopen (complaint_status ? "/dev/null" : *out_namep, "w");
  *out_linenop = 1;
}

ایستا تهی
fail_for_at_directive_too_few_args (نویسه ثابت *at_directive_name)
{
  complain (پوچ, fatal, _("too few arguments for %s directive in skeleton"),
            at_directive_name);
}

ایستا تهی
fail_for_at_directive_too_many_args (نویسه ثابت *at_directive_name)
{
  complain (پوچ, fatal, _("too many arguments for %s directive in skeleton"),
            at_directive_name);
}

ایستا تهی
fail_for_invalid_at (نویسه ثابت *at)
{
  complain (پوچ, fatal, "invalid @ in skeleton: %s", at);
}

ایستا تهی
output_mapped_file (نویسه ثابت *name)
{
  fputs (quotearg_style (c_quoting_style, map_file_name (name)), yyout);
}
