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
#ضمن <dirname.h>
#ضمن <error.h>
#ضمن <path-join.h>
#ضمن <quotearg.h>

#ضمن "src/complain.h"
#ضمن "src/files.h"
#ضمن "src/getargs.h"
#ضمن "src/scan-skel.h"

#define FLEX_PREFIX(Id) skel_ ## Id
#ضمن "src/flex-scanner.h"

/* Work around a bug in flex 2.5.31.  See Debian bug 333231
   <https://bugs.debian.org/333231>.  */
#undef skel_wrap
#define skel_wrap() 1

#define YY_DECL ساكن صحيح skel_lex (فراغ)
YY_DECL;

تعريفنوع فراغ (*at_directive)(صحيح, حرف**, حرف **, صحيح*);
ساكن فراغ at_init (صحيح *argc, حرف *argv[], at_directive *at_ptr, at_directive fun);
ساكن فراغ at_basename (صحيح argc, حرف *argv[], حرف**, صحيح*);
ساكن فراغ at_complain (صحيح argc, حرف *argv[], حرف**, صحيح*);
ساكن فراغ at_output (صحيح argc, حرف *argv[], حرف **name, صحيح *lineno);
ساكن فراغ fail_for_at_directive_too_many_args (حرف ثابت *at_directive_name);
ساكن فراغ fail_for_at_directive_too_few_args (حرف ثابت *at_directive_name);
ساكن فراغ fail_for_invalid_at (حرف ثابت *at);
ساكن فراغ output_mapped_file (حرف ثابت *name);
%}

/* Identifiers of our M4 macros.  */
macro      [bm]4_[a-zA-Z_0-9]*
/* Safe sequence of word-constituent characters.  */
identifier [A-Za-z_0-9]+

%x SC_AT_DIRECTIVE_ARGS
%x SC_AT_DIRECTIVE_SKIP_WS

%%

%{
  صحيح out_lineno PACIFY_CC (= 0);
  حرف *out_name = عدم;

  /* Currently, only the @complain directive takes multiple arguments, and
     never more than 7, with argv[0] being the directive name and argv[1]
     being the type of complaint to dispatch. */
#define ARGC_MAX 9
  صحيح argc = 0;
  حرف *argv[ARGC_MAX];
  at_directive at_ptr = عدم;
%}

"@@" fputc ('@', yyout);
"@{" fputc ('[', yyout);
"@}" fputc (']', yyout);
"@'" استمر;  /* Used by b4_cat in ../data/bison.m4.  */
@\n  استمر;

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
                  loc.start.line = out_lineno;
                  loc.end = loc.start;
                  complain (&loc, Wother,
                            "suspicious sequence in the output: %s", yytext);
                  ECHO;
                }
{identifier}    ECHO;
.               ECHO;

<INITIAL><<EOF>> {
  אם (out_name)
    {
      حرر (out_name);
      xfclose (yyout);
    }
  החזר EOF;
}

<SC_AT_DIRECTIVE_ARGS>
{
  [^@]+  STRING_GROW ();

  "@@"   STRING_1GROW ('@');
  "@{"   STRING_1GROW ('[');
  "@}"   STRING_1GROW (']');
  "@'"   استمر; /* For starting an argument that begins with whitespace. */
  @\n    استمر;

  @[,)] {
    אם (argc >= ARGC_MAX)
      fail_for_at_directive_too_many_args (argv[0]);

    argv[argc++] = obstack_finish0 (&obstack_for_string);

    /* Like M4, skip whitespace after a comma.  */
    אם (yytext[1] == ',')
      BEGIN SC_AT_DIRECTIVE_SKIP_WS;
    אחרת
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
  [ \t\r\n]    استمر;
  .            yyless (0); BEGIN SC_AT_DIRECTIVE_ARGS;
}

<SC_AT_DIRECTIVE_ARGS,SC_AT_DIRECTIVE_SKIP_WS>
{
  <<EOF>>  complain (عدم, fatal, _("unclosed %s directive in skeleton"), argv[0]);
}

%%

ساكن فراغ
at_init (صحيح *argc, حرف *argv[], at_directive *at_ptr, at_directive fun)
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

فراغ
scan_skel (FILE *in)
{
  ساكن bool initialized = false;
  אם (!initialized)
    {
      initialized = true;
      obstack_init (&obstack_for_string);
    }
  skel_in = in;
  skel__flex_debug = trace_flag & trace_skeleton;
  skel_lex ();
}

فراغ
skel_scanner_free (فراغ)
{
  obstack_free (&obstack_for_string, 0);
  /* Reclaim Flex's buffers.  */
  yylex_destroy ();
}

ساكن مضمن warnings
flag (ثابت حرف *arg)
{
  /* compare with values issued from b4_error */
  אם (STREQ (arg, "complain"))
    החזר complaint;
  אחרת אם (STREQ (arg, "deprecated"))
    החזר Wdeprecated;
  אחרת אם (STREQ (arg, "fatal"))
    החזר fatal;
  אחרת אם (STREQ (arg, "note"))
    החזר silent | complaint | no_caret | note;
  אחרת אם (STREQ (arg, "warn"))
    החזר Wother;
  אחרת
    abort ();
}

ساكن فراغ
at_basename (صحيح argc, حرف *argv[], حرف **out_namep, صحيح *out_linenop)
{
  (فراغ) out_namep;
  (فراغ) out_linenop;
  אם (2 < argc)
    fail_for_at_directive_too_many_args (argv[0]);
  fputs (last_component (argv[1]), yyout);
}

ساكن فراغ
at_complain (صحيح argc, حرف *argv[], حرف **out_namep, صحيح *out_linenop)
{
  אם (argc < 4)
    fail_for_at_directive_too_few_args (argv[0]);

  (فراغ) out_namep;
  (فراغ) out_linenop;

  warnings w = flag (argv[1]);

  location loc;
  location *locp = عدم;
  אם (argv[2] && argv[2][0])
    {
      boundary_set_from_string (&loc.start, argv[2]);
      boundary_set_from_string (&loc.end, argv[3]);
      locp = &loc;
    }
  complain_args (locp, w, argc - 4, argv + 4);
}

ساكن فراغ
at_output (صحيح argc, حرف *argv[], حرف **out_namep, صحيح *out_linenop)
{
  אם (3 < argc)
    fail_for_at_directive_too_many_args (argv[0]);
  אם (*out_namep)
    {
      حرر (*out_namep);
      xfclose (yyout);
    }
  *out_namep = xpath_join (argv[1], 2 < argc ? argv[2] : عدم);
  output_file_name_check (out_namep, true);
  /* If there were errors, do not generate the output.  */
  yyout = xfopen (complaint_status ? "/dev/null" : *out_namep, "w");
  *out_linenop = 1;
}

ساكن فراغ
fail_for_at_directive_too_few_args (حرف ثابت *at_directive_name)
{
  complain (عدم, fatal, _("too few arguments for %s directive in skeleton"),
            at_directive_name);
}

ساكن فراغ
fail_for_at_directive_too_many_args (حرف ثابت *at_directive_name)
{
  complain (عدم, fatal, _("too many arguments for %s directive in skeleton"),
            at_directive_name);
}

ساكن فراغ
fail_for_invalid_at (حرف ثابت *at)
{
  complain (عدم, fatal, "invalid @ in skeleton: %s", at);
}

ساكن فراغ
output_mapped_file (حرف ثابت *name)
{
  fputs (quotearg_style (c_quoting_style, map_file_name (name)), yyout);
}
