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
#ଅନ୍ତର୍ଭାଵ <dirname.h>
#ଅନ୍ତର୍ଭାଵ <error.h>
#ଅନ୍ତର୍ଭାଵ <path-join.h>
#ଅନ୍ତର୍ଭାଵ <quotearg.h>

#ଅନ୍ତର୍ଭାଵ "src/complain.h"
#ଅନ୍ତର୍ଭାଵ "src/files.h"
#ଅନ୍ତର୍ଭାଵ "src/getargs.h"
#ଅନ୍ତର୍ଭାଵ "src/scan-skel.h"

#define FLEX_PREFIX(Id) skel_ ## Id
#ଅନ୍ତର୍ଭାଵ "src/flex-scanner.h"

/* Work around a bug in flex 2.5.31.  See Debian bug 333231
   <https://bugs.debian.org/333231>.  */
#undef skel_wrap
#define skel_wrap() 1

#define YY_DECL ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା skel_lex (ଶୂନ୍ୟ)
YY_DECL;

ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*at_directive)(ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ଅକ୍ଷର**, ଅକ୍ଷର **, ପୂର୍ଣ୍ଣସଂଖ୍ୟା*);
ସ୍ଥିର ଶୂନ୍ୟ at_init (ପୂର୍ଣ୍ଣସଂଖ୍ୟା *argc, ଅକ୍ଷର *argv[], at_directive *at_ptr, at_directive fun);
ସ୍ଥିର ଶୂନ୍ୟ at_basename (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର *argv[], ଅକ୍ଷର**, ପୂର୍ଣ୍ଣସଂଖ୍ୟା*);
ସ୍ଥିର ଶୂନ୍ୟ at_complain (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର *argv[], ଅକ୍ଷର**, ପୂର୍ଣ୍ଣସଂଖ୍ୟା*);
ସ୍ଥିର ଶୂନ୍ୟ at_output (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର *argv[], ଅକ୍ଷର **name, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *lineno);
ସ୍ଥିର ଶୂନ୍ୟ fail_for_at_directive_too_many_args (ଅକ୍ଷର ଅଚଳ *at_directive_name);
ସ୍ଥିର ଶୂନ୍ୟ fail_for_at_directive_too_few_args (ଅକ୍ଷର ଅଚଳ *at_directive_name);
ସ୍ଥିର ଶୂନ୍ୟ fail_for_invalid_at (ଅକ୍ଷର ଅଚଳ *at);
ସ୍ଥିର ଶୂନ୍ୟ output_mapped_file (ଅକ୍ଷର ଅଚଳ *name);
%}

/* Identifiers of our M4 macros.  */
macro      [bm]4_[a-zA-Z_0-9]*
/* Safe sequence of word-constituent characters.  */
identifier [A-Za-z_0-9]+

%x SC_AT_DIRECTIVE_ARGS
%x SC_AT_DIRECTIVE_SKIP_WS

%%

%{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା out_lineno PACIFY_CC (= 0);
  ଅକ୍ଷର *out_name = ଖାଲି;

  /* Currently, only the @complain directive takes multiple arguments, and
     never more than 7, with argv[0] being the directive name and argv[1]
     being the type of complaint to dispatch. */
#define ARGC_MAX 9
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc = 0;
  ଅକ୍ଷର *argv[ARGC_MAX];
  at_directive at_ptr = ଖାଲି;
%}

"@@" fputc ('@', yyout);
"@{" fputc ('[', yyout);
"@}" fputc (']', yyout);
"@'" ଜାରି;  /* Used by b4_cat in ../data/bison.m4.  */
@\n  ଜାରି;

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
                  loc.start.ଧାଡ଼ି = out_lineno;
                  loc.end = loc.start;
                  complain (&loc, Wother,
                            "suspicious sequence in the output: %s", yytext);
                  ECHO;
                }
{identifier}    ECHO;
.               ECHO;

<INITIAL><<EOF>> {
  ଯଦି (out_name)
    {
      ମୁକ୍ତ (out_name);
      xfclose (yyout);
    }
  ଫେରନ୍ତୁ EOF;
}

<SC_AT_DIRECTIVE_ARGS>
{
  [^@]+  STRING_GROW ();

  "@@"   STRING_1GROW ('@');
  "@{"   STRING_1GROW ('[');
  "@}"   STRING_1GROW (']');
  "@'"   ଜାରି; /* For starting an argument that begins with whitespace. */
  @\n    ଜାରି;

  @[,)] {
    ଯଦି (argc >= ARGC_MAX)
      fail_for_at_directive_too_many_args (argv[0]);

    argv[argc++] = obstack_finish0 (&obstack_for_string);

    /* Like M4, skip whitespace after a comma.  */
    ଯଦି (yytext[1] == ',')
      BEGIN SC_AT_DIRECTIVE_SKIP_WS;
    ଅନ୍ୟଥା
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
  [ \t\r\n]    ଜାରି;
  .            yyless (0); BEGIN SC_AT_DIRECTIVE_ARGS;
}

<SC_AT_DIRECTIVE_ARGS,SC_AT_DIRECTIVE_SKIP_WS>
{
  <<EOF>>  complain (ଖାଲି, fatal, _("unclosed %s directive in skeleton"), argv[0]);
}

%%

ସ୍ଥିର ଶୂନ୍ୟ
at_init (ପୂର୍ଣ୍ଣସଂଖ୍ୟା *argc, ଅକ୍ଷର *argv[], at_directive *at_ptr, at_directive fun)
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

ଶୂନ୍ୟ
scan_skel (FILE *in)
{
  ସ୍ଥିର bool initialized = false;
  ଯଦି (!initialized)
    {
      initialized = true;
      obstack_init (&obstack_for_string);
    }
  skel_in = in;
  skel__flex_debug = trace_flag & trace_skeleton;
  skel_lex ();
}

ଶୂନ୍ୟ
skel_scanner_free (ଶୂନ୍ୟ)
{
  obstack_free (&obstack_for_string, 0);
  /* Reclaim Flex's buffers.  */
  yylex_destroy ();
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ warnings
flag (ଅଚଳ ଅକ୍ଷର *arg)
{
  /* compare with values issued from b4_error */
  ଯଦି (STREQ (arg, "complain"))
    ଫେରନ୍ତୁ complaint;
  ଅନ୍ୟଥା ଯଦି (STREQ (arg, "deprecated"))
    ଫେରନ୍ତୁ Wdeprecated;
  ଅନ୍ୟଥା ଯଦି (STREQ (arg, "fatal"))
    ଫେରନ୍ତୁ fatal;
  ଅନ୍ୟଥା ଯଦି (STREQ (arg, "note"))
    ଫେରନ୍ତୁ silent | complaint | no_caret | note;
  ଅନ୍ୟଥା ଯଦି (STREQ (arg, "warn"))
    ଫେରନ୍ତୁ Wother;
  ଅନ୍ୟଥା
    abort ();
}

ସ୍ଥିର ଶୂନ୍ୟ
at_basename (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର *argv[], ଅକ୍ଷର **out_namep, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *out_linenop)
{
  (ଶୂନ୍ୟ) out_namep;
  (ଶୂନ୍ୟ) out_linenop;
  ଯଦି (2 < argc)
    fail_for_at_directive_too_many_args (argv[0]);
  fputs (last_component (argv[1]), yyout);
}

ସ୍ଥିର ଶୂନ୍ୟ
at_complain (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର *argv[], ଅକ୍ଷର **out_namep, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *out_linenop)
{
  ଯଦି (argc < 4)
    fail_for_at_directive_too_few_args (argv[0]);

  (ଶୂନ୍ୟ) out_namep;
  (ଶୂନ୍ୟ) out_linenop;

  warnings w = flag (argv[1]);

  location loc;
  location *locp = ଖାଲି;
  ଯଦି (argv[2] && argv[2][0])
    {
      boundary_set_from_string (&loc.start, argv[2]);
      boundary_set_from_string (&loc.end, argv[3]);
      locp = &loc;
    }
  complain_args (locp, w, argc - 4, argv + 4);
}

ସ୍ଥିର ଶୂନ୍ୟ
at_output (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର *argv[], ଅକ୍ଷର **out_namep, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *out_linenop)
{
  ଯଦି (3 < argc)
    fail_for_at_directive_too_many_args (argv[0]);
  ଯଦି (*out_namep)
    {
      ମୁକ୍ତ (*out_namep);
      xfclose (yyout);
    }
  *out_namep = xpath_join (argv[1], 2 < argc ? argv[2] : ଖାଲି);
  output_file_name_check (out_namep, true);
  /* If there were errors, do not generate the output.  */
  yyout = xfopen (complaint_status ? "/dev/null" : *out_namep, "w");
  *out_linenop = 1;
}

ସ୍ଥିର ଶୂନ୍ୟ
fail_for_at_directive_too_few_args (ଅକ୍ଷର ଅଚଳ *at_directive_name)
{
  complain (ଖାଲି, fatal, _("too few arguments for %s directive in skeleton"),
            at_directive_name);
}

ସ୍ଥିର ଶୂନ୍ୟ
fail_for_at_directive_too_many_args (ଅକ୍ଷର ଅଚଳ *at_directive_name)
{
  complain (ଖାଲି, fatal, _("too many arguments for %s directive in skeleton"),
            at_directive_name);
}

ସ୍ଥିର ଶୂନ୍ୟ
fail_for_invalid_at (ଅକ୍ଷର ଅଚଳ *at)
{
  complain (ଖାଲି, fatal, "invalid @ in skeleton: %s", at);
}

ସ୍ଥିର ଶୂନ୍ୟ
output_mapped_file (ଅକ୍ଷର ଅଚଳ *name)
{
  fputs (quotearg_style (c_quoting_style, map_file_name (name)), yyout);
}
