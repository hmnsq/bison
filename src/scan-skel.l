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
#અન્તર્ભાવ <dirname.h>
#અન્તર્ભાવ <error.h>
#અન્તર્ભાવ <path-join.h>
#અન્તર્ભાવ <quotearg.h>

#અન્તર્ભાવ "src/complain.h"
#અન્તર્ભાવ "src/files.h"
#અન્તર્ભાવ "src/getargs.h"
#અન્તર્ભાવ "src/scan-skel.h"

#define FLEX_PREFIX(Id) skel_ ## Id
#અન્તર્ભાવ "src/flex-scanner.h"

/* Work around a bug in flex 2.5.31.  See Debian bug 333231
   <https://bugs.debian.org/333231>.  */
#undef skel_wrap
#define skel_wrap() 1

#define YY_DECL સ્થિર પૂર્ણાંક skel_lex (શૂન્ય)
YY_DECL;

પ્રકારવ્યાખ્યા શૂન્ય (*at_directive)(પૂર્ણાંક, અક્ષર**, અક્ષર **, પૂર્ણાંક*);
સ્થિર શૂન્ય at_init (પૂર્ણાંક *argc, અક્ષર *argv[], at_directive *at_ptr, at_directive fun);
સ્થિર શૂન્ય at_basename (પૂર્ણાંક argc, અક્ષર *argv[], અક્ષર**, પૂર્ણાંક*);
સ્થિર શૂન્ય at_complain (પૂર્ણાંક argc, અક્ષર *argv[], અક્ષર**, પૂર્ણાંક*);
સ્થિર શૂન્ય at_output (પૂર્ણાંક argc, અક્ષર *argv[], અક્ષર **name, પૂર્ણાંક *lineno);
સ્થિર શૂન્ય fail_for_at_directive_too_many_args (અક્ષર અચલ *at_directive_name);
સ્થિર શૂન્ય fail_for_at_directive_too_few_args (અક્ષર અચલ *at_directive_name);
સ્થિર શૂન્ય fail_for_invalid_at (અક્ષર અચલ *at);
સ્થિર શૂન્ય output_mapped_file (અક્ષર અચલ *name);
%}

/* Identifiers of our M4 macros.  */
macro      [bm]4_[a-zA-Z_0-9]*
/* Safe sequence of word-constituent characters.  */
identifier [A-Za-z_0-9]+

%x SC_AT_DIRECTIVE_ARGS
%x SC_AT_DIRECTIVE_SKIP_WS

%%

%{
  પૂર્ણાંક out_lineno PACIFY_CC (= 0);
  અક્ષર *out_name = ખાલી;

  /* Currently, only the @complain directive takes multiple arguments, and
     never more than 7, with argv[0] being the directive name and argv[1]
     being the type of complaint to dispatch. */
#define ARGC_MAX 9
  પૂર્ણાંક argc = 0;
  અક્ષર *argv[ARGC_MAX];
  at_directive at_ptr = ખાલી;
%}

"@@" fputc ('@', yyout);
"@{" fputc ('[', yyout);
"@}" fputc (']', yyout);
"@'" ચાલુ;  /* Used by b4_cat in ../data/bison.m4.  */
@\n  ચાલુ;

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
                  loc.start.લીટી = out_lineno;
                  loc.end = loc.start;
                  complain (&loc, Wother,
                            "suspicious sequence in the output: %s", yytext);
                  ECHO;
                }
{identifier}    ECHO;
.               ECHO;

<INITIAL><<EOF>> {
  જો (out_name)
    {
      મુક્ત (out_name);
      xfclose (yyout);
    }
  પાછા EOF;
}

<SC_AT_DIRECTIVE_ARGS>
{
  [^@]+  STRING_GROW ();

  "@@"   STRING_1GROW ('@');
  "@{"   STRING_1GROW ('[');
  "@}"   STRING_1GROW (']');
  "@'"   ચાલુ; /* For starting an argument that begins with whitespace. */
  @\n    ચાલુ;

  @[,)] {
    જો (argc >= ARGC_MAX)
      fail_for_at_directive_too_many_args (argv[0]);

    argv[argc++] = obstack_finish0 (&obstack_for_string);

    /* Like M4, skip whitespace after a comma.  */
    જો (yytext[1] == ',')
      BEGIN SC_AT_DIRECTIVE_SKIP_WS;
    નહીંતર
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
  [ \t\r\n]    ચાલુ;
  .            yyless (0); BEGIN SC_AT_DIRECTIVE_ARGS;
}

<SC_AT_DIRECTIVE_ARGS,SC_AT_DIRECTIVE_SKIP_WS>
{
  <<EOF>>  complain (ખાલી, fatal, _("unclosed %s directive in skeleton"), argv[0]);
}

%%

સ્થિર શૂન્ય
at_init (પૂર્ણાંક *argc, અક્ષર *argv[], at_directive *at_ptr, at_directive fun)
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

શૂન્ય
scan_skel (FILE *in)
{
  સ્થિર bool initialized = false;
  જો (!initialized)
    {
      initialized = true;
      obstack_init (&obstack_for_string);
    }
  skel_in = in;
  skel__flex_debug = trace_flag & trace_skeleton;
  skel_lex ();
}

શૂન્ય
skel_scanner_free (શૂન્ય)
{
  obstack_free (&obstack_for_string, 0);
  /* Reclaim Flex's buffers.  */
  yylex_destroy ();
}

સ્થિર અંતઃરેખ warnings
flag (અચલ અક્ષર *arg)
{
  /* compare with values issued from b4_error */
  જો (STREQ (arg, "complain"))
    પાછા complaint;
  નહીંતર જો (STREQ (arg, "deprecated"))
    પાછા Wdeprecated;
  નહીંતર જો (STREQ (arg, "fatal"))
    પાછા fatal;
  નહીંતર જો (STREQ (arg, "note"))
    પાછા silent | complaint | no_caret | note;
  નહીંતર જો (STREQ (arg, "warn"))
    પાછા Wother;
  નહીંતર
    abort ();
}

સ્થિર શૂન્ય
at_basename (પૂર્ણાંક argc, અક્ષર *argv[], અક્ષર **out_namep, પૂર્ણાંક *out_linenop)
{
  (શૂન્ય) out_namep;
  (શૂન્ય) out_linenop;
  જો (2 < argc)
    fail_for_at_directive_too_many_args (argv[0]);
  fputs (last_component (argv[1]), yyout);
}

સ્થિર શૂન્ય
at_complain (પૂર્ણાંક argc, અક્ષર *argv[], અક્ષર **out_namep, પૂર્ણાંક *out_linenop)
{
  જો (argc < 4)
    fail_for_at_directive_too_few_args (argv[0]);

  (શૂન્ય) out_namep;
  (શૂન્ય) out_linenop;

  warnings w = flag (argv[1]);

  location loc;
  location *locp = ખાલી;
  જો (argv[2] && argv[2][0])
    {
      boundary_set_from_string (&loc.start, argv[2]);
      boundary_set_from_string (&loc.end, argv[3]);
      locp = &loc;
    }
  complain_args (locp, w, argc - 4, argv + 4);
}

સ્થિર શૂન્ય
at_output (પૂર્ણાંક argc, અક્ષર *argv[], અક્ષર **out_namep, પૂર્ણાંક *out_linenop)
{
  જો (3 < argc)
    fail_for_at_directive_too_many_args (argv[0]);
  જો (*out_namep)
    {
      મુક્ત (*out_namep);
      xfclose (yyout);
    }
  *out_namep = xpath_join (argv[1], 2 < argc ? argv[2] : ખાલી);
  output_file_name_check (out_namep, true);
  /* If there were errors, do not generate the output.  */
  yyout = xfopen (complaint_status ? "/dev/null" : *out_namep, "w");
  *out_linenop = 1;
}

સ્થિર શૂન્ય
fail_for_at_directive_too_few_args (અક્ષર અચલ *at_directive_name)
{
  complain (ખાલી, fatal, _("too few arguments for %s directive in skeleton"),
            at_directive_name);
}

સ્થિર શૂન્ય
fail_for_at_directive_too_many_args (અક્ષર અચલ *at_directive_name)
{
  complain (ખાલી, fatal, _("too many arguments for %s directive in skeleton"),
            at_directive_name);
}

સ્થિર શૂન્ય
fail_for_invalid_at (અક્ષર અચલ *at)
{
  complain (ખાલી, fatal, "invalid @ in skeleton: %s", at);
}

સ્થિર શૂન્ય
output_mapped_file (અક્ષર અચલ *name)
{
  fputs (quotearg_style (c_quoting_style, map_file_name (name)), yyout);
}
