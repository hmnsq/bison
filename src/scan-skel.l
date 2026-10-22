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
#അന്തര്ഭാവ <dirname.h>
#അന്തര്ഭാവ <error.h>
#അന്തര്ഭാവ <path-join.h>
#അന്തര്ഭാവ <quotearg.h>

#അന്തര്ഭാവ "src/complain.h"
#അന്തര്ഭാവ "src/files.h"
#അന്തര്ഭാവ "src/getargs.h"
#അന്തര്ഭാവ "src/scan-skel.h"

#define FLEX_PREFIX(Id) skel_ ## Id
#അന്തര്ഭാവ "src/flex-scanner.h"

/* Work around a bug in flex 2.5.31.  See Debian bug 333231
   <https://bugs.debian.org/333231>.  */
#undef skel_wrap
#define skel_wrap() 1

#define YY_DECL സ്ഥിതം പൂർണ്ണസംഖ്യ skel_lex (ശൂന്യം)
YY_DECL;

തരനിർവചനം ശൂന്യം (*at_directive)(പൂർണ്ണസംഖ്യ, അക്ഷരം**, അക്ഷരം **, പൂർണ്ണസംഖ്യ*);
സ്ഥിതം ശൂന്യം at_init (പൂർണ്ണസംഖ്യ *argc, അക്ഷരം *argv[], at_directive *at_ptr, at_directive fun);
സ്ഥിതം ശൂന്യം at_basename (പൂർണ്ണസംഖ്യ argc, അക്ഷരം *argv[], അക്ഷരം**, പൂർണ്ണസംഖ്യ*);
സ്ഥിതം ശൂന്യം at_complain (പൂർണ്ണസംഖ്യ argc, അക്ഷരം *argv[], അക്ഷരം**, പൂർണ്ണസംഖ്യ*);
സ്ഥിതം ശൂന്യം at_output (പൂർണ്ണസംഖ്യ argc, അക്ഷരം *argv[], അക്ഷരം **name, പൂർണ്ണസംഖ്യ *lineno);
സ്ഥിതം ശൂന്യം fail_for_at_directive_too_many_args (അക്ഷരം സ്ഥിരം *at_directive_name);
സ്ഥിതം ശൂന്യം fail_for_at_directive_too_few_args (അക്ഷരം സ്ഥിരം *at_directive_name);
സ്ഥിതം ശൂന്യം fail_for_invalid_at (അക്ഷരം സ്ഥിരം *at);
സ്ഥിതം ശൂന്യം output_mapped_file (അക്ഷരം സ്ഥിരം *name);
%}

/* Identifiers of our M4 macros.  */
macro      [bm]4_[a-zA-Z_0-9]*
/* Safe sequence of word-constituent characters.  */
identifier [A-Za-z_0-9]+

%x SC_AT_DIRECTIVE_ARGS
%x SC_AT_DIRECTIVE_SKIP_WS

%%

%{
  പൂർണ്ണസംഖ്യ out_lineno PACIFY_CC (= 0);
  അക്ഷരം *out_name = ശൂന്യ;

  /* Currently, only the @complain directive takes multiple arguments, and
     never more than 7, with argv[0] being the directive name and argv[1]
     being the type of complaint to dispatch. */
#define ARGC_MAX 9
  പൂർണ്ണസംഖ്യ argc = 0;
  അക്ഷരം *argv[ARGC_MAX];
  at_directive at_ptr = ശൂന്യ;
%}

"@@" fputc ('@', yyout);
"@{" fputc ('[', yyout);
"@}" fputc (']', yyout);
"@'" തുടരുക;  /* Used by b4_cat in ../data/bison.m4.  */
@\n  തുടരുക;

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
                  loc.start.വരി = out_lineno;
                  loc.end = loc.start;
                  complain (&loc, Wother,
                            "suspicious sequence in the output: %s", yytext);
                  ECHO;
                }
{identifier}    ECHO;
.               ECHO;

<INITIAL><<EOF>> {
  എങ്കിൽ (out_name)
    {
      സ്വതന്ത്രം (out_name);
      xfclose (yyout);
    }
  തിരികെ EOF;
}

<SC_AT_DIRECTIVE_ARGS>
{
  [^@]+  STRING_GROW ();

  "@@"   STRING_1GROW ('@');
  "@{"   STRING_1GROW ('[');
  "@}"   STRING_1GROW (']');
  "@'"   തുടരുക; /* For starting an argument that begins with whitespace. */
  @\n    തുടരുക;

  @[,)] {
    എങ്കിൽ (argc >= ARGC_MAX)
      fail_for_at_directive_too_many_args (argv[0]);

    argv[argc++] = obstack_finish0 (&obstack_for_string);

    /* Like M4, skip whitespace after a comma.  */
    എങ്കിൽ (yytext[1] == ',')
      BEGIN SC_AT_DIRECTIVE_SKIP_WS;
    അല്ലെങ്കിൽ
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
  [ \t\r\n]    തുടരുക;
  .            yyless (0); BEGIN SC_AT_DIRECTIVE_ARGS;
}

<SC_AT_DIRECTIVE_ARGS,SC_AT_DIRECTIVE_SKIP_WS>
{
  <<EOF>>  complain (ശൂന്യ, fatal, _("unclosed %s directive in skeleton"), argv[0]);
}

%%

സ്ഥിതം ശൂന്യം
at_init (പൂർണ്ണസംഖ്യ *argc, അക്ഷരം *argv[], at_directive *at_ptr, at_directive fun)
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

ശൂന്യം
scan_skel (FILE *in)
{
  സ്ഥിതം bool initialized = false;
  എങ്കിൽ (!initialized)
    {
      initialized = true;
      obstack_init (&obstack_for_string);
    }
  skel_in = in;
  skel__flex_debug = trace_flag & trace_skeleton;
  skel_lex ();
}

ശൂന്യം
skel_scanner_free (ശൂന്യം)
{
  obstack_free (&obstack_for_string, 0);
  /* Reclaim Flex's buffers.  */
  yylex_destroy ();
}

സ്ഥിതം വരിയിൽ warnings
flag (സ്ഥിരം അക്ഷരം *arg)
{
  /* compare with values issued from b4_error */
  എങ്കിൽ (STREQ (arg, "complain"))
    തിരികെ complaint;
  അല്ലെങ്കിൽ എങ്കിൽ (STREQ (arg, "deprecated"))
    തിരികെ Wdeprecated;
  അല്ലെങ്കിൽ എങ്കിൽ (STREQ (arg, "fatal"))
    തിരികെ fatal;
  അല്ലെങ്കിൽ എങ്കിൽ (STREQ (arg, "note"))
    തിരികെ silent | complaint | no_caret | note;
  അല്ലെങ്കിൽ എങ്കിൽ (STREQ (arg, "warn"))
    തിരികെ Wother;
  അല്ലെങ്കിൽ
    abort ();
}

സ്ഥിതം ശൂന്യം
at_basename (പൂർണ്ണസംഖ്യ argc, അക്ഷരം *argv[], അക്ഷരം **out_namep, പൂർണ്ണസംഖ്യ *out_linenop)
{
  (ശൂന്യം) out_namep;
  (ശൂന്യം) out_linenop;
  എങ്കിൽ (2 < argc)
    fail_for_at_directive_too_many_args (argv[0]);
  fputs (last_component (argv[1]), yyout);
}

സ്ഥിതം ശൂന്യം
at_complain (പൂർണ്ണസംഖ്യ argc, അക്ഷരം *argv[], അക്ഷരം **out_namep, പൂർണ്ണസംഖ്യ *out_linenop)
{
  എങ്കിൽ (argc < 4)
    fail_for_at_directive_too_few_args (argv[0]);

  (ശൂന്യം) out_namep;
  (ശൂന്യം) out_linenop;

  warnings w = flag (argv[1]);

  location loc;
  location *locp = ശൂന്യ;
  എങ്കിൽ (argv[2] && argv[2][0])
    {
      boundary_set_from_string (&loc.start, argv[2]);
      boundary_set_from_string (&loc.end, argv[3]);
      locp = &loc;
    }
  complain_args (locp, w, argc - 4, argv + 4);
}

സ്ഥിതം ശൂന്യം
at_output (പൂർണ്ണസംഖ്യ argc, അക്ഷരം *argv[], അക്ഷരം **out_namep, പൂർണ്ണസംഖ്യ *out_linenop)
{
  എങ്കിൽ (3 < argc)
    fail_for_at_directive_too_many_args (argv[0]);
  എങ്കിൽ (*out_namep)
    {
      സ്വതന്ത്രം (*out_namep);
      xfclose (yyout);
    }
  *out_namep = xpath_join (argv[1], 2 < argc ? argv[2] : ശൂന്യ);
  output_file_name_check (out_namep, true);
  /* If there were errors, do not generate the output.  */
  yyout = xfopen (complaint_status ? "/dev/null" : *out_namep, "w");
  *out_linenop = 1;
}

സ്ഥിതം ശൂന്യം
fail_for_at_directive_too_few_args (അക്ഷരം സ്ഥിരം *at_directive_name)
{
  complain (ശൂന്യ, fatal, _("too few arguments for %s directive in skeleton"),
            at_directive_name);
}

സ്ഥിതം ശൂന്യം
fail_for_at_directive_too_many_args (അക്ഷരം സ്ഥിരം *at_directive_name)
{
  complain (ശൂന്യ, fatal, _("too many arguments for %s directive in skeleton"),
            at_directive_name);
}

സ്ഥിതം ശൂന്യം
fail_for_invalid_at (അക്ഷരം സ്ഥിരം *at)
{
  complain (ശൂന്യ, fatal, "invalid @ in skeleton: %s", at);
}

സ്ഥിതം ശൂന്യം
output_mapped_file (അക്ഷരം സ്ഥിരം *name)
{
  fputs (quotearg_style (c_quoting_style, map_file_name (name)), yyout);
}
