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
#ಅನ್ತರ್ಭಾವ <dirname.h>
#ಅನ್ತರ್ಭಾವ <error.h>
#ಅನ್ತರ್ಭಾವ <path-join.h>
#ಅನ್ತರ್ಭಾವ <quotearg.h>

#ಅನ್ತರ್ಭಾವ "src/complain.h"
#ಅನ್ತರ್ಭಾವ "src/files.h"
#ಅನ್ತರ್ಭಾವ "src/getargs.h"
#ಅನ್ತರ್ಭಾವ "src/scan-skel.h"

#define FLEX_PREFIX(Id) skel_ ## Id
#ಅನ್ತರ್ಭಾವ "src/flex-scanner.h"

/* Work around a bug in flex 2.5.31.  See Debian bug 333231
   <https://bugs.debian.org/333231>.  */
#undef skel_wrap
#define skel_wrap() 1

#define YY_DECL ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ skel_lex (ಶೂನ್ಯ)
YY_DECL;

ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ (*at_directive)(ಪೂರ್ಣಾಙ್ಕ, ವರ್ಣ**, ವರ್ಣ **, ಪೂರ್ಣಾಙ್ಕ*);
ಸ್ಥಿರ ಶೂನ್ಯ at_init (ಪೂರ್ಣಾಙ್ಕ *argc, ವರ್ಣ *argv[], at_directive *at_ptr, at_directive fun);
ಸ್ಥಿರ ಶೂನ್ಯ at_basename (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ *argv[], ವರ್ಣ**, ಪೂರ್ಣಾಙ್ಕ*);
ಸ್ಥಿರ ಶೂನ್ಯ at_complain (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ *argv[], ವರ್ಣ**, ಪೂರ್ಣಾಙ್ಕ*);
ಸ್ಥಿರ ಶೂನ್ಯ at_output (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ *argv[], ವರ್ಣ **name, ಪೂರ್ಣಾಙ್ಕ *lineno);
ಸ್ಥಿರ ಶೂನ್ಯ fail_for_at_directive_too_many_args (ವರ್ಣ ಧ್ರುವ *at_directive_name);
ಸ್ಥಿರ ಶೂನ್ಯ fail_for_at_directive_too_few_args (ವರ್ಣ ಧ್ರುವ *at_directive_name);
ಸ್ಥಿರ ಶೂನ್ಯ fail_for_invalid_at (ವರ್ಣ ಧ್ರುವ *at);
ಸ್ಥಿರ ಶೂನ್ಯ output_mapped_file (ವರ್ಣ ಧ್ರುವ *name);
%}

/* Identifiers of our M4 macros.  */
macro      [bm]4_[a-zA-Z_0-9]*
/* Safe sequence of word-constituent characters.  */
identifier [A-Za-z_0-9]+

%x SC_AT_DIRECTIVE_ARGS
%x SC_AT_DIRECTIVE_SKIP_WS

%%

%{
  ಪೂರ್ಣಾಙ್ಕ out_lineno PACIFY_CC (= 0);
  ವರ್ಣ *out_name = ರಿಕ್ತ;

  /* Currently, only the @complain directive takes multiple arguments, and
     never more than 7, with argv[0] being the directive name and argv[1]
     being the type of complaint to dispatch. */
#define ARGC_MAX 9
  ಪೂರ್ಣಾಙ್ಕ argc = 0;
  ವರ್ಣ *argv[ARGC_MAX];
  at_directive at_ptr = ರಿಕ್ತ;
%}

"@@" fputc ('@', yyout);
"@{" fputc ('[', yyout);
"@}" fputc (']', yyout);
"@'" ಅನುವರ್ತ;  /* Used by b4_cat in ../data/bison.m4.  */
@\n  ಅನುವರ್ತ;

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
                  loc.start.ಸಾಲು = out_lineno;
                  loc.end = loc.start;
                  complain (&loc, Wother,
                            "suspicious sequence in the output: %s", yytext);
                  ECHO;
                }
{identifier}    ECHO;
.               ECHO;

<INITIAL><<EOF>> {
  ಯದಿ (out_name)
    {
      ಮುಕ್ತ (out_name);
      xfclose (yyout);
    }
  ಪ್ರತ್ಯಾಗಮ EOF;
}

<SC_AT_DIRECTIVE_ARGS>
{
  [^@]+  STRING_GROW ();

  "@@"   STRING_1GROW ('@');
  "@{"   STRING_1GROW ('[');
  "@}"   STRING_1GROW (']');
  "@'"   ಅನುವರ್ತ; /* For starting an argument that begins with whitespace. */
  @\n    ಅನುವರ್ತ;

  @[,)] {
    ಯದಿ (argc >= ARGC_MAX)
      fail_for_at_directive_too_many_args (argv[0]);

    argv[argc++] = obstack_finish0 (&obstack_for_string);

    /* Like M4, skip whitespace after a comma.  */
    ಯದಿ (yytext[1] == ',')
      BEGIN SC_AT_DIRECTIVE_SKIP_WS;
    ಅನ್ಯಥಾ
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
  [ \t\r\n]    ಅನುವರ್ತ;
  .            yyless (0); BEGIN SC_AT_DIRECTIVE_ARGS;
}

<SC_AT_DIRECTIVE_ARGS,SC_AT_DIRECTIVE_SKIP_WS>
{
  <<EOF>>  complain (ರಿಕ್ತ, fatal, _("unclosed %s directive in skeleton"), argv[0]);
}

%%

ಸ್ಥಿರ ಶೂನ್ಯ
at_init (ಪೂರ್ಣಾಙ್ಕ *argc, ವರ್ಣ *argv[], at_directive *at_ptr, at_directive fun)
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

ಶೂನ್ಯ
scan_skel (FILE *in)
{
  ಸ್ಥಿರ bool initialized = false;
  ಯದಿ (!initialized)
    {
      initialized = true;
      obstack_init (&obstack_for_string);
    }
  skel_in = in;
  skel__flex_debug = trace_flag & trace_skeleton;
  skel_lex ();
}

ಶೂನ್ಯ
skel_scanner_free (ಶೂನ್ಯ)
{
  obstack_free (&obstack_for_string, 0);
  /* Reclaim Flex's buffers.  */
  yylex_destroy ();
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ warnings
flag (ಧ್ರುವ ವರ್ಣ *arg)
{
  /* compare with values issued from b4_error */
  ಯದಿ (STREQ (arg, "complain"))
    ಪ್ರತ್ಯಾಗಮ complaint;
  ಅನ್ಯಥಾ ಯದಿ (STREQ (arg, "deprecated"))
    ಪ್ರತ್ಯಾಗಮ Wdeprecated;
  ಅನ್ಯಥಾ ಯದಿ (STREQ (arg, "fatal"))
    ಪ್ರತ್ಯಾಗಮ fatal;
  ಅನ್ಯಥಾ ಯದಿ (STREQ (arg, "note"))
    ಪ್ರತ್ಯಾಗಮ silent | complaint | no_caret | note;
  ಅನ್ಯಥಾ ಯದಿ (STREQ (arg, "warn"))
    ಪ್ರತ್ಯಾಗಮ Wother;
  ಅನ್ಯಥಾ
    abort ();
}

ಸ್ಥಿರ ಶೂನ್ಯ
at_basename (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ *argv[], ವರ್ಣ **out_namep, ಪೂರ್ಣಾಙ್ಕ *out_linenop)
{
  (ಶೂನ್ಯ) out_namep;
  (ಶೂನ್ಯ) out_linenop;
  ಯದಿ (2 < argc)
    fail_for_at_directive_too_many_args (argv[0]);
  fputs (last_component (argv[1]), yyout);
}

ಸ್ಥಿರ ಶೂನ್ಯ
at_complain (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ *argv[], ವರ್ಣ **out_namep, ಪೂರ್ಣಾಙ್ಕ *out_linenop)
{
  ಯದಿ (argc < 4)
    fail_for_at_directive_too_few_args (argv[0]);

  (ಶೂನ್ಯ) out_namep;
  (ಶೂನ್ಯ) out_linenop;

  warnings w = flag (argv[1]);

  location loc;
  location *locp = ರಿಕ್ತ;
  ಯದಿ (argv[2] && argv[2][0])
    {
      boundary_set_from_string (&loc.start, argv[2]);
      boundary_set_from_string (&loc.end, argv[3]);
      locp = &loc;
    }
  complain_args (locp, w, argc - 4, argv + 4);
}

ಸ್ಥಿರ ಶೂನ್ಯ
at_output (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ *argv[], ವರ್ಣ **out_namep, ಪೂರ್ಣಾಙ್ಕ *out_linenop)
{
  ಯದಿ (3 < argc)
    fail_for_at_directive_too_many_args (argv[0]);
  ಯದಿ (*out_namep)
    {
      ಮುಕ್ತ (*out_namep);
      xfclose (yyout);
    }
  *out_namep = xpath_join (argv[1], 2 < argc ? argv[2] : ರಿಕ್ತ);
  output_file_name_check (out_namep, true);
  /* If there were errors, do not generate the output.  */
  yyout = xfopen (complaint_status ? "/dev/null" : *out_namep, "w");
  *out_linenop = 1;
}

ಸ್ಥಿರ ಶೂನ್ಯ
fail_for_at_directive_too_few_args (ವರ್ಣ ಧ್ರುವ *at_directive_name)
{
  complain (ರಿಕ್ತ, fatal, _("too few arguments for %s directive in skeleton"),
            at_directive_name);
}

ಸ್ಥಿರ ಶೂನ್ಯ
fail_for_at_directive_too_many_args (ವರ್ಣ ಧ್ರುವ *at_directive_name)
{
  complain (ರಿಕ್ತ, fatal, _("too many arguments for %s directive in skeleton"),
            at_directive_name);
}

ಸ್ಥಿರ ಶೂನ್ಯ
fail_for_invalid_at (ವರ್ಣ ಧ್ರುವ *at)
{
  complain (ರಿಕ್ತ, fatal, "invalid @ in skeleton: %s", at);
}

ಸ್ಥಿರ ಶೂನ್ಯ
output_mapped_file (ವರ್ಣ ಧ್ರುವ *name)
{
  fputs (quotearg_style (c_quoting_style, map_file_name (name)), yyout);
}
