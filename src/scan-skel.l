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
%option ಪೂರ್ವಪ್ರತ್ಯಯ="skel_" outfile="lex.yy.c"

%{
#ಅನ್ತರ್ಭಾವ <dirname.h>
#ಅನ್ತರ್ಭಾವ <error.h>
#ಅನ್ತರ್ಭಾವ <path-join.h>
#ಅನ್ತರ್ಭಾವ <quotearg.h>

#ಅನ್ತರ್ಭಾವ "src/complain.h"
#ಅನ್ತರ್ಭಾವ "src/files.h"
#ಅನ್ತರ್ಭಾವ "src/getargs.h"
#ಅನ್ತರ್ಭಾವ "src/scan-skel.h"

#ವ್ಯಾಖ್ಯಾನಿಸು FLEX_ಪೂರ್ವಪ್ರತ್ಯಯ(Id) skel_ ## Id
#ಅನ್ತರ್ಭಾವ "src/flex-scanner.h"

/* Work around a bug in flex 2.5.31.  See Debian bug 333231
   <https://bugs.debian.org/333231>.  */
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು skel_wrap
#ವ್ಯಾಖ್ಯಾನಿಸು skel_wrap() 1

#ವ್ಯಾಖ್ಯಾನಿಸು YY_DECL ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ skel_lex (ಶೂನ್ಯ)
YY_DECL;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*at_directive)(ಪೂರ್ಣಾಂಕ, ಅಕ್ಷರ**, ಅಕ್ಷರ **, ಪೂರ್ಣಾಂಕ*);
ಸ್ಥಿರ ಶೂನ್ಯ at_init (ಪೂರ್ಣಾಂಕ *argc, ಅಕ್ಷರ *argv[], at_directive *at_ptr, at_directive fun);
ಸ್ಥಿರ ಶೂನ್ಯ at_basename (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ *argv[], ಅಕ್ಷರ**, ಪೂರ್ಣಾಂಕ*);
ಸ್ಥಿರ ಶೂನ್ಯ at_complain (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ *argv[], ಅಕ್ಷರ**, ಪೂರ್ಣಾಂಕ*);
ಸ್ಥಿರ ಶೂನ್ಯ at_output (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ *argv[], ಅಕ್ಷರ **name, ಪೂರ್ಣಾಂಕ *lineno);
ಸ್ಥಿರ ಶೂನ್ಯ fail_for_at_directive_too_many_ವಾದಗಳು (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *at_directive_name);
ಸ್ಥಿರ ಶೂನ್ಯ fail_for_at_directive_too_few_ವಾದಗಳು (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *at_directive_name);
ಸ್ಥಿರ ಶೂನ್ಯ fail_for_invalid_at (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *at);
ಸ್ಥಿರ ಶೂನ್ಯ output_mapped_file (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name);
%}

/* Identifiers of our M4 macros.  */
macro      [bm]4_[a-zA-Z_0-9]*
/* Safe sequence of word-constituent characters.  */
identifier [A-Za-z_0-9]+

%x SC_AT_DIRECTIVE_ವಾದಗಳು
%x SC_AT_DIRECTIVE_SKIP_WS

%%

%{
  ಪೂರ್ಣಾಂಕ ಹೊರಹೋಗು_lineno PACIFY_CC (= 0);
  ಅಕ್ಷರ *ಹೊರಹೋಗು_name = ಖಾಲಿ;

  /* Currently, only the @complain directive takes multiple arguments, and
     never more than 7, with argv[0] being the directive name and argv[1]
     being the type of complaint to dispatch. */
#ವ್ಯಾಖ್ಯಾನಿಸು ARGC_MAX 9
  ಪೂರ್ಣಾಂಕ argc = 0;
  ಅಕ್ಷರ *argv[ARGC_MAX];
  at_directive at_ptr = ಖಾಲಿ;
%}

"@@" fputc ('@', yyout);
"@{" fputc ('[', yyout);
"@}" fputc (']', yyout);
"@'" ಮುಂದುವರಿ;  /* Used by b4_cat in ../data/bison.m4.  */
@\n  ಮುಂದುವರಿ;

"@oline@"  fprintf (yyout, "%d", ಹೊರಹೋಗು_lineno + 1);
"@ofile@"  output_mapped_file (ಹೊರಹೋಗು_name);

"@basename("    at_init (&argc, argv, &at_ptr, &at_basename);
"@complain("    at_init (&argc, argv, &at_ptr, &at_complain);
"@output("      at_init (&argc, argv, &at_ptr, &at_output);

  /* This pattern must not match more than the previous @ patterns. */
@[^@{}''(\n]*   fail_for_invalid_at (yytext);
\n              ಹೊರಹೋಗು_lineno++; ECHO;
[^a-z@\n]+      ECHO;

  /* If there are still identifiers that look like macros, such as
     b4_synbol, this probably an error, say a typo in M4, or
     overquotation.  */
{macro}         {
                  location loc = ಖಾಲಿಗಳು_loc;
                  loc.ಆರಂಭ.file = map_file_name (ಹೊರಹೋಗು_name);
                  loc.ಆರಂಭ.ಸಾಲು = ಹೊರಹೋಗು_lineno;
                  loc.ಕೊನೆ = loc.ಆರಂಭ;
                  complain (&loc, Wother,
                            "suspicious sequence in the output: %s", yytext);
                  ECHO;
                }
{identifier}    ECHO;
.               ECHO;

<INITIAL><<EOF>> {
  ಆದರೆ (ಹೊರಹೋಗು_name)
    {
      ಬಿಡುಗಡೆ (ಹೊರಹೋಗು_name);
      xfclose (yyout);
    }
  ಹಿಂತಿರುಗಿ EOF;
}

<SC_AT_DIRECTIVE_ವಾದಗಳು>
{
  [^@]+  STRING_GROW ();

  "@@"   STRING_1GROW ('@');
  "@{"   STRING_1GROW ('[');
  "@}"   STRING_1GROW (']');
  "@'"   ಮುಂದುವರಿ; /* For starting an argument that begins with whitespace. */
  @\n    ಮುಂದುವರಿ;

  @[,)] {
    ಆದರೆ (argc >= ARGC_MAX)
      fail_for_at_directive_too_many_ವಾದಗಳು (argv[0]);

    argv[argc++] = obstack_finish0 (&obstack_for_string);

    /* Like M4, skip whitespace after a comma.  */
    ಆದರೆ (yytext[1] == ',')
      BEGIN SC_AT_DIRECTIVE_SKIP_WS;
    ಇಲ್ಲದಿದ್ದರೆ
      {
        aver (at_ptr);
        at_ptr (argc, argv, &ಹೊರಹೋಗು_name, &ಹೊರಹೋಗು_lineno);
        obstack_free (&obstack_for_string, argv[0]);
        argc = 0;
        BEGIN INITIAL;
      }
  }

  @.?  fail_for_invalid_at (yytext);
}

<SC_AT_DIRECTIVE_SKIP_WS>
{
  [ \t\r\n]    ಮುಂದುವರಿ;
  .            yyless (0); BEGIN SC_AT_DIRECTIVE_ವಾದಗಳು;
}

<SC_AT_DIRECTIVE_ವಾದಗಳು,SC_AT_DIRECTIVE_SKIP_WS>
{
  <<EOF>>  complain (ಖಾಲಿ, fatal, _("unclosed %s directive in skeleton"), argv[0]);
}

%%

ಸ್ಥಿರ ಶೂನ್ಯ
at_init (ಪೂರ್ಣಾಂಕ *argc, ಅಕ್ಷರ *argv[], at_directive *at_ptr, at_directive fun)
{
  *at_ptr = fun;
  yytext[yyleng-1] = '\0';
  obstack_grow (&obstack_for_string, yytext, yyleng);
  argv[(*argc)++] = obstack_ಮುಕ್ತಾಯ (&obstack_for_string);
  BEGIN SC_AT_DIRECTIVE_ವಾದಗಳು;
}

/*------------------------.
| Scan a Bison skeleton.  |
`------------------------*/

ಶೂನ್ಯ
ಪರಿಶೀಲನೆ_skel (FILE *in)
{
  ಸ್ಥಿರ bool initialized = false;
  ಆದರೆ (!initialized)
    {
      initialized = true;
      obstack_init (&obstack_for_string);
    }
  skel_ಒಳ = in;
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

ಸ್ಥಿರ ಸಾಲೊಳಗೆ warnings
flag (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg)
{
  /* compare with values issued from b4_error */
  ಆದರೆ (STREQ (arg, "complain"))
    ಹಿಂತಿರುಗಿ complaint;
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (STREQ (arg, "deprecated"))
    ಹಿಂತಿರುಗಿ Wdeprecated;
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (STREQ (arg, "fatal"))
    ಹಿಂತಿರುಗಿ fatal;
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (STREQ (arg, "note"))
    ಹಿಂತಿರುಗಿ silent | complaint | no_caret | note;
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (STREQ (arg, "warn"))
    ಹಿಂತಿರುಗಿ Wother;
  ಇಲ್ಲದಿದ್ದರೆ
    abort ();
}

ಸ್ಥಿರ ಶೂನ್ಯ
at_basename (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ *argv[], ಅಕ್ಷರ **ಹೊರಹೋಗು_namep, ಪೂರ್ಣಾಂಕ *ಹೊರಹೋಗು_linenop)
{
  (ಶೂನ್ಯ) ಹೊರಹೋಗು_namep;
  (ಶೂನ್ಯ) ಹೊರಹೋಗು_linenop;
  ಆದರೆ (2 < argc)
    fail_for_at_directive_too_many_ವಾದಗಳು (argv[0]);
  fputs (ಕೊನೆಯ_component (argv[1]), yyout);
}

ಸ್ಥಿರ ಶೂನ್ಯ
at_complain (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ *argv[], ಅಕ್ಷರ **ಹೊರಹೋಗು_namep, ಪೂರ್ಣಾಂಕ *ಹೊರಹೋಗು_linenop)
{
  ಆದರೆ (argc < 4)
    fail_for_at_directive_too_few_ವಾದಗಳು (argv[0]);

  (ಶೂನ್ಯ) ಹೊರಹೋಗು_namep;
  (ಶೂನ್ಯ) ಹೊರಹೋಗು_linenop;

  warnings w = flag (argv[1]);

  location loc;
  location *locp = ಖಾಲಿ;
  ಆದರೆ (argv[2] && argv[2][0])
    {
      boundary_ಗಣ_from_string (&loc.ಆರಂಭ, argv[2]);
      boundary_ಗಣ_from_string (&loc.ಕೊನೆ, argv[3]);
      locp = &loc;
    }
  complain_ವಾದಗಳು (locp, w, argc - 4, argv + 4);
}

ಸ್ಥಿರ ಶೂನ್ಯ
at_output (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ *argv[], ಅಕ್ಷರ **ಹೊರಹೋಗು_namep, ಪೂರ್ಣಾಂಕ *ಹೊರಹೋಗು_linenop)
{
  ಆದರೆ (3 < argc)
    fail_for_at_directive_too_many_ವಾದಗಳು (argv[0]);
  ಆದರೆ (*ಹೊರಹೋಗು_namep)
    {
      ಬಿಡುಗಡೆ (*ಹೊರಹೋಗು_namep);
      xfclose (yyout);
    }
  *ಹೊರಹೋಗು_namep = xpath_join (argv[1], 2 < argc ? argv[2] : ಖಾಲಿ);
  output_file_name_check (ಹೊರಹೋಗು_namep, true);
  /* If there were errors, do not generate the output.  */
  yyout = xfopen (complaint_ಸ್ಥಿತಿ ? "/dev/null" : *ಹೊರಹೋಗು_namep, "w");
  *ಹೊರಹೋಗು_linenop = 1;
}

ಸ್ಥಿರ ಶೂನ್ಯ
fail_for_at_directive_too_few_ವಾದಗಳು (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *at_directive_name)
{
  complain (ಖಾಲಿ, fatal, _("too few arguments for %s directive in skeleton"),
            at_directive_name);
}

ಸ್ಥಿರ ಶೂನ್ಯ
fail_for_at_directive_too_many_ವಾದಗಳು (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *at_directive_name)
{
  complain (ಖಾಲಿ, fatal, _("too many arguments for %s directive in skeleton"),
            at_directive_name);
}

ಸ್ಥಿರ ಶೂನ್ಯ
fail_for_invalid_at (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *at)
{
  complain (ಖಾಲಿ, fatal, "invalid @ in skeleton: %s", at);
}

ಸ್ಥಿರ ಶೂನ್ಯ
output_mapped_file (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name)
{
  fputs (quotearg_style (c_quoting_style, map_file_name (name)), yyout);
}
