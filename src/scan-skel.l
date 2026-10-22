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
#அந்தர்பாவ <dirname.h>
#அந்தர்பாவ <error.h>
#அந்தர்பாவ <path-join.h>
#அந்தர்பாவ <quotearg.h>

#அந்தர்பாவ "src/complain.h"
#அந்தர்பாவ "src/files.h"
#அந்தர்பாவ "src/getargs.h"
#அந்தர்பாவ "src/scan-skel.h"

#define FLEX_PREFIX(Id) skel_ ## Id
#அந்தர்பாவ "src/flex-scanner.h"

/* Work around a bug in flex 2.5.31.  See Debian bug 333231
   <https://bugs.debian.org/333231>.  */
#undef skel_wrap
#define skel_wrap() 1

#define YY_DECL நிலையான முழு skel_lex (வெறுமை)
YY_DECL;

வகைவரை வெறுமை (*at_directive)(முழு, எழுத்து**, எழுத்து **, முழு*);
நிலையான வெறுமை at_init (முழு *argc, எழுத்து *argv[], at_directive *at_ptr, at_directive fun);
நிலையான வெறுமை at_basename (முழு argc, எழுத்து *argv[], எழுத்து**, முழு*);
நிலையான வெறுமை at_complain (முழு argc, எழுத்து *argv[], எழுத்து**, முழு*);
நிலையான வெறுமை at_output (முழு argc, எழுத்து *argv[], எழுத்து **name, முழு *lineno);
நிலையான வெறுமை fail_for_at_directive_too_many_args (எழுத்து மாறிலி *at_directive_name);
நிலையான வெறுமை fail_for_at_directive_too_few_args (எழுத்து மாறிலி *at_directive_name);
நிலையான வெறுமை fail_for_invalid_at (எழுத்து மாறிலி *at);
நிலையான வெறுமை output_mapped_file (எழுத்து மாறிலி *name);
%}

/* Identifiers of our M4 macros.  */
macro      [bm]4_[a-zA-Z_0-9]*
/* Safe sequence of word-constituent characters.  */
identifier [A-Za-z_0-9]+

%x SC_AT_DIRECTIVE_ARGS
%x SC_AT_DIRECTIVE_SKIP_WS

%%

%{
  முழு out_lineno PACIFY_CC (= 0);
  எழுத்து *out_name = வெற்று;

  /* Currently, only the @complain directive takes multiple arguments, and
     never more than 7, with argv[0] being the directive name and argv[1]
     being the type of complaint to dispatch. */
#define ARGC_MAX 9
  முழு argc = 0;
  எழுத்து *argv[ARGC_MAX];
  at_directive at_ptr = வெற்று;
%}

"@@" fputc ('@', yyout);
"@{" fputc ('[', yyout);
"@}" fputc (']', yyout);
"@'" தொடர்;  /* Used by b4_cat in ../data/bison.m4.  */
@\n  தொடர்;

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
                  loc.start.வரி = out_lineno;
                  loc.end = loc.start;
                  complain (&loc, Wother,
                            "suspicious sequence in the output: %s", yytext);
                  ECHO;
                }
{identifier}    ECHO;
.               ECHO;

<INITIAL><<EOF>> {
  எனில் (out_name)
    {
      விடுவி (out_name);
      xfclose (yyout);
    }
  திரும்பு EOF;
}

<SC_AT_DIRECTIVE_ARGS>
{
  [^@]+  STRING_GROW ();

  "@@"   STRING_1GROW ('@');
  "@{"   STRING_1GROW ('[');
  "@}"   STRING_1GROW (']');
  "@'"   தொடர்; /* For starting an argument that begins with whitespace. */
  @\n    தொடர்;

  @[,)] {
    எனில் (argc >= ARGC_MAX)
      fail_for_at_directive_too_many_args (argv[0]);

    argv[argc++] = obstack_finish0 (&obstack_for_string);

    /* Like M4, skip whitespace after a comma.  */
    எனில் (yytext[1] == ',')
      BEGIN SC_AT_DIRECTIVE_SKIP_WS;
    இல்லையெனில்
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
  [ \t\r\n]    தொடர்;
  .            yyless (0); BEGIN SC_AT_DIRECTIVE_ARGS;
}

<SC_AT_DIRECTIVE_ARGS,SC_AT_DIRECTIVE_SKIP_WS>
{
  <<EOF>>  complain (வெற்று, fatal, _("unclosed %s directive in skeleton"), argv[0]);
}

%%

நிலையான வெறுமை
at_init (முழு *argc, எழுத்து *argv[], at_directive *at_ptr, at_directive fun)
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

வெறுமை
scan_skel (FILE *in)
{
  நிலையான bool initialized = false;
  எனில் (!initialized)
    {
      initialized = true;
      obstack_init (&obstack_for_string);
    }
  skel_in = in;
  skel__flex_debug = trace_flag & trace_skeleton;
  skel_lex ();
}

வெறுமை
skel_scanner_free (வெறுமை)
{
  obstack_free (&obstack_for_string, 0);
  /* Reclaim Flex's buffers.  */
  yylex_destroy ();
}

நிலையான உள்வரி warnings
flag (மாறிலி எழுத்து *arg)
{
  /* compare with values issued from b4_error */
  எனில் (STREQ (arg, "complain"))
    திரும்பு complaint;
  இல்லையெனில் எனில் (STREQ (arg, "deprecated"))
    திரும்பு Wdeprecated;
  இல்லையெனில் எனில் (STREQ (arg, "fatal"))
    திரும்பு fatal;
  இல்லையெனில் எனில் (STREQ (arg, "note"))
    திரும்பு silent | complaint | no_caret | note;
  இல்லையெனில் எனில் (STREQ (arg, "warn"))
    திரும்பு Wother;
  இல்லையெனில்
    abort ();
}

நிலையான வெறுமை
at_basename (முழு argc, எழுத்து *argv[], எழுத்து **out_namep, முழு *out_linenop)
{
  (வெறுமை) out_namep;
  (வெறுமை) out_linenop;
  எனில் (2 < argc)
    fail_for_at_directive_too_many_args (argv[0]);
  fputs (last_component (argv[1]), yyout);
}

நிலையான வெறுமை
at_complain (முழு argc, எழுத்து *argv[], எழுத்து **out_namep, முழு *out_linenop)
{
  எனில் (argc < 4)
    fail_for_at_directive_too_few_args (argv[0]);

  (வெறுமை) out_namep;
  (வெறுமை) out_linenop;

  warnings w = flag (argv[1]);

  location loc;
  location *locp = வெற்று;
  எனில் (argv[2] && argv[2][0])
    {
      boundary_set_from_string (&loc.start, argv[2]);
      boundary_set_from_string (&loc.end, argv[3]);
      locp = &loc;
    }
  complain_args (locp, w, argc - 4, argv + 4);
}

நிலையான வெறுமை
at_output (முழு argc, எழுத்து *argv[], எழுத்து **out_namep, முழு *out_linenop)
{
  எனில் (3 < argc)
    fail_for_at_directive_too_many_args (argv[0]);
  எனில் (*out_namep)
    {
      விடுவி (*out_namep);
      xfclose (yyout);
    }
  *out_namep = xpath_join (argv[1], 2 < argc ? argv[2] : வெற்று);
  output_file_name_check (out_namep, true);
  /* If there were errors, do not generate the output.  */
  yyout = xfopen (complaint_status ? "/dev/null" : *out_namep, "w");
  *out_linenop = 1;
}

நிலையான வெறுமை
fail_for_at_directive_too_few_args (எழுத்து மாறிலி *at_directive_name)
{
  complain (வெற்று, fatal, _("too few arguments for %s directive in skeleton"),
            at_directive_name);
}

நிலையான வெறுமை
fail_for_at_directive_too_many_args (எழுத்து மாறிலி *at_directive_name)
{
  complain (வெற்று, fatal, _("too many arguments for %s directive in skeleton"),
            at_directive_name);
}

நிலையான வெறுமை
fail_for_invalid_at (எழுத்து மாறிலி *at)
{
  complain (வெற்று, fatal, "invalid @ in skeleton: %s", at);
}

நிலையான வெறுமை
output_mapped_file (எழுத்து மாறிலி *name)
{
  fputs (quotearg_style (c_quoting_style, map_file_name (name)), yyout);
}
