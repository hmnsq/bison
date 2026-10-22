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
%option اڳياڙي="skel_" outfile="lex.yy.c"

%{
#ضمن <dirname.h>
#ضمن <error.h>
#ضمن <path-join.h>
#ضمن <quotearg.h>

#ضمن "src/complain.h"
#ضمن "src/files.h"
#ضمن "src/getargs.h"
#ضمن "src/scan-skel.h"

#وصف_ڪريو FLEX_اڳياڙي(Id) skel_ ## Id
#ضمن "src/flex-scanner.h"

/* Work around a bug in flex 2.5.31.  See Debian bug 333231
   <https://bugs.debian.org/333231>.  */
#وصف_رد skel_wrap
#وصف_ڪريو skel_wrap() 1

#وصف_ڪريو YY_DECL ساكن صحيح skel_lex (فراغ)
YY_DECL;

تعريفنوع فراغ (*at_directive)(صحيح, حرف**, حرف **, صحيح*);
ساكن فراغ at_init (صحيح *argc, حرف *argv[], at_directive *at_ptr, at_directive fun);
ساكن فراغ at_basename (صحيح argc, حرف *argv[], حرف**, صحيح*);
ساكن فراغ at_complain (صحيح argc, حرف *argv[], حرف**, صحيح*);
ساكن فراغ at_output (صحيح argc, حرف *argv[], حرف **name, صحيح *lineno);
ساكن فراغ fail_for_at_directive_too_many_دليلون (حرف ثابت *at_directive_name);
ساكن فراغ fail_for_at_directive_too_few_دليلون (حرف ثابت *at_directive_name);
ساكن فراغ fail_for_invalid_at (حرف ثابت *at);
ساكن فراغ output_mapped_file (حرف ثابت *name);
%}

/* Identifiers of our M4 macros.  */
macro      [bm]4_[a-zA-Z_0-9]*
/* Safe sequence of word-constituent characters.  */
identifier [A-Za-z_0-9]+

%x SC_AT_DIRECTIVE_دليلون
%x SC_AT_DIRECTIVE_SKIP_WS

%%

%{
  صحيح ٻاهريون_lineno PACIFY_CC (= 0);
  حرف *ٻاهريون_name = عدم;

  /* Currently, only the @complain directive takes multiple arguments, and
     never more than 7, with argv[0] being the directive name and argv[1]
     being the type of complaint to dispatch. */
#وصف_ڪريو ARGC_MAX 9
  صحيح argc = 0;
  حرف *argv[ARGC_MAX];
  at_directive at_ptr = عدم;
%}

"@@" fputc ('@', yyout);
"@{" fputc ('[', yyout);
"@}" fputc (']', yyout);
"@'" استمر;  /* Used by b4_cat in ../data/bison.m4.  */
@\n  استمر;

"@oline@"  fprintf (yyout, "%d", ٻاهريون_lineno + 1);
"@ofile@"  output_mapped_file (ٻاهريون_name);

"@basename("    at_init (&argc, argv, &at_ptr, &at_basename);
"@complain("    at_init (&argc, argv, &at_ptr, &at_complain);
"@output("      at_init (&argc, argv, &at_ptr, &at_output);

  /* This pattern must not match more than the previous @ patterns. */
@[^@{}''(\n]*   fail_for_invalid_at (yytext);
\n              ٻاهريون_lineno++; ECHO;
[^a-z@\n]+      ECHO;

  /* If there are still identifiers that look like macros, such as
     b4_synbol, this probably an error, say a typo in M4, or
     overquotation.  */
{macro}         {
                  location loc = خالي_جايون_loc;
                  loc.شروعات.file = map_file_name (ٻاهريون_name);
                  loc.شروعات.سٽ = ٻاهريون_lineno;
                  loc.پڇاڙي = loc.شروعات;
                  complain (&loc, Wother,
                            "suspicious sequence in the output: %s", yytext);
                  ECHO;
                }
{identifier}    ECHO;
.               ECHO;

<INITIAL><<EOF>> {
  إذا (ٻاهريون_name)
    {
      حرر (ٻاهريون_name);
      xfclose (yyout);
    }
  أرجع EOF;
}

<SC_AT_DIRECTIVE_دليلون>
{
  [^@]+  STRING_GROW ();

  "@@"   STRING_1GROW ('@');
  "@{"   STRING_1GROW ('[');
  "@}"   STRING_1GROW (']');
  "@'"   استمر; /* For starting an argument that begins with whitespace. */
  @\n    استمر;

  @[,)] {
    إذا (argc >= ARGC_MAX)
      fail_for_at_directive_too_many_دليلون (argv[0]);

    argv[argc++] = obstack_finish0 (&obstack_for_string);

    /* Like M4, skip whitespace after a comma.  */
    إذا (yytext[1] == ',')
      BEGIN SC_AT_DIRECTIVE_SKIP_WS;
    وإلا
      {
        aver (at_ptr);
        at_ptr (argc, argv, &ٻاهريون_name, &ٻاهريون_lineno);
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
  .            yyless (0); BEGIN SC_AT_DIRECTIVE_دليلون;
}

<SC_AT_DIRECTIVE_دليلون,SC_AT_DIRECTIVE_SKIP_WS>
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
  argv[(*argc)++] = obstack_پڄاڻي (&obstack_for_string);
  BEGIN SC_AT_DIRECTIVE_دليلون;
}

/*------------------------.
| Scan a Bison skeleton.  |
`------------------------*/

فراغ
جاچ_skel (FILE *in)
{
  ساكن bool initialized = false;
  إذا (!initialized)
    {
      initialized = true;
      obstack_init (&obstack_for_string);
    }
  skel_اندريون = in;
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
  إذا (STREQ (arg, "complain"))
    أرجع complaint;
  وإلا إذا (STREQ (arg, "deprecated"))
    أرجع Wdeprecated;
  وإلا إذا (STREQ (arg, "fatal"))
    أرجع fatal;
  وإلا إذا (STREQ (arg, "note"))
    أرجع silent | complaint | no_caret | note;
  وإلا إذا (STREQ (arg, "warn"))
    أرجع Wother;
  وإلا
    abort ();
}

ساكن فراغ
at_basename (صحيح argc, حرف *argv[], حرف **ٻاهريون_namep, صحيح *ٻاهريون_linenop)
{
  (فراغ) ٻاهريون_namep;
  (فراغ) ٻاهريون_linenop;
  إذا (2 < argc)
    fail_for_at_directive_too_many_دليلون (argv[0]);
  fputs (آخري_component (argv[1]), yyout);
}

ساكن فراغ
at_complain (صحيح argc, حرف *argv[], حرف **ٻاهريون_namep, صحيح *ٻاهريون_linenop)
{
  إذا (argc < 4)
    fail_for_at_directive_too_few_دليلون (argv[0]);

  (فراغ) ٻاهريون_namep;
  (فراغ) ٻاهريون_linenop;

  warnings w = flag (argv[1]);

  location loc;
  location *locp = عدم;
  إذا (argv[2] && argv[2][0])
    {
      boundary_سيٽ_from_string (&loc.شروعات, argv[2]);
      boundary_سيٽ_from_string (&loc.پڇاڙي, argv[3]);
      locp = &loc;
    }
  complain_دليلون (locp, w, argc - 4, argv + 4);
}

ساكن فراغ
at_output (صحيح argc, حرف *argv[], حرف **ٻاهريون_namep, صحيح *ٻاهريون_linenop)
{
  إذا (3 < argc)
    fail_for_at_directive_too_many_دليلون (argv[0]);
  إذا (*ٻاهريون_namep)
    {
      حرر (*ٻاهريون_namep);
      xfclose (yyout);
    }
  *ٻاهريون_namep = xpath_join (argv[1], 2 < argc ? argv[2] : عدم);
  output_file_name_check (ٻاهريون_namep, true);
  /* If there were errors, do not generate the output.  */
  yyout = xfopen (complaint_حالت ? "/dev/null" : *ٻاهريون_namep, "w");
  *ٻاهريون_linenop = 1;
}

ساكن فراغ
fail_for_at_directive_too_few_دليلون (حرف ثابت *at_directive_name)
{
  complain (عدم, fatal, _("too few arguments for %s directive in skeleton"),
            at_directive_name);
}

ساكن فراغ
fail_for_at_directive_too_many_دليلون (حرف ثابت *at_directive_name)
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
