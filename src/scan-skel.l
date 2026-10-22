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
#समावेश <dirname.h>
#समावेश <error.h>
#समावेश <path-join.h>
#समावेश <quotearg.h>

#समावेश "src/complain.h"
#समावेश "src/files.h"
#समावेश "src/getargs.h"
#समावेश "src/scan-skel.h"

#घोषणा FLEX_PREFIX(Id) skel_ ## Id
#समावेश "src/flex-scanner.h"

/* Work around a bug in flex 2.5.31.  See Debian bug 333231
   <https://bugs.debian.org/333231>.  */
#अघोषित skel_wrap
#घोषणा skel_wrap() 1

#घोषणा YY_DECL अटल पूर्णांक skel_lex (व्योम)
YY_DECL;

प्रकार व्योम (*at_directive)(पूर्णांक, अक्षर**, अक्षर **, पूर्णांक*);
अटल व्योम at_init (पूर्णांक *argc, अक्षर *argv[], at_directive *at_ptr, at_directive fun);
अटल व्योम at_basename (पूर्णांक argc, अक्षर *argv[], अक्षर**, पूर्णांक*);
अटल व्योम at_complain (पूर्णांक argc, अक्षर *argv[], अक्षर**, पूर्णांक*);
अटल व्योम at_output (पूर्णांक argc, अक्षर *argv[], अक्षर **name, पूर्णांक *lineno);
अटल व्योम fail_for_at_directive_too_many_args (अक्षर स्थिर *at_directive_name);
अटल व्योम fail_for_at_directive_too_few_args (अक्षर स्थिर *at_directive_name);
अटल व्योम fail_for_invalid_at (अक्षर स्थिर *at);
अटल व्योम output_mapped_file (अक्षर स्थिर *name);
%}

/* Identifiers of our M4 macros.  */
macro      [bm]4_[a-zA-Z_0-9]*
/* Safe sequence of word-constituent characters.  */
identifier [A-Za-z_0-9]+

%x SC_AT_DIRECTIVE_ARGS
%x SC_AT_DIRECTIVE_SKIP_WS

%%

%{
  पूर्णांक out_lineno PACIFY_CC (= 0);
  अक्षर *out_name = शून्य;

  /* Currently, only the @complain directive takes multiple arguments, and
     never more than 7, with argv[0] being the directive name and argv[1]
     being the type of complaint to dispatch. */
#घोषणा ARGC_MAX 9
  पूर्णांक argc = 0;
  अक्षर *argv[ARGC_MAX];
  at_directive at_ptr = शून्य;
%}

"@@" ख_अक्षर_दो ('@', yyout);
"@{" ख_अक्षर_दो ('[', yyout);
"@}" ख_अक्षर_दो (']', yyout);
"@'" जारी;  /* Used by b4_cat in ../data/bison.m4.  */
@\n  जारी;

"@oline@"  ख_लिखो (yyout, "%d", out_lineno + 1);
"@ofile@"  output_mapped_file (out_name);

"@basename("    at_init (&argc, argv, &at_ptr, &at_basename);
"@complain("    at_init (&argc, argv, &at_ptr, &at_complain);
"@output("      at_init (&argc, argv, &at_ptr, &at_output);

  /* This pattern must not match more than the previous @ patterns. */
@[^@{}''(\n]*   fail_for_invalid_at (श_माला);
\n              out_lineno++; ECHO;
[^a-z@\n]+      ECHO;

  /* If there are still identifiers that look like macros, such as
     b4_synbol, this probably an error, say a typo in M4, or
     overquotation.  */
{macro}         {
                  location loc = empty_loc;
                  loc.start.file = map_file_name (out_name);
                  loc.start.पंक्ति = out_lineno;
                  loc.end = loc.start;
                  complain (&loc, Wother,
                            "suspicious sequence in the output: %s", श_माला);
                  ECHO;
                }
{identifier}    ECHO;
.               ECHO;

<INITIAL><<खातापूर्ण>> {
  अगर (out_name)
    {
      मुक्त (out_name);
      xfclose (yyout);
    }
  वापस खातापूर्ण;
}

<SC_AT_DIRECTIVE_ARGS>
{
  [^@]+  STRING_GROW ();

  "@@"   STRING_1GROW ('@');
  "@{"   STRING_1GROW ('[');
  "@}"   STRING_1GROW (']');
  "@'"   जारी; /* For starting an argument that begins with whitespace. */
  @\n    जारी;

  @[,)] {
    अगर (argc >= ARGC_MAX)
      fail_for_at_directive_too_many_args (argv[0]);

    argv[argc++] = obstack_finish0 (&obstack_for_string);

    /* Like M4, skip whitespace after a comma.  */
    अगर (श_माला[1] == ',')
      BEGIN SC_AT_DIRECTIVE_SKIP_WS;
    अन्यथा
      {
        aver (at_ptr);
        at_ptr (argc, argv, &out_name, &out_lineno);
        obstack_free (&obstack_for_string, argv[0]);
        argc = 0;
        BEGIN INITIAL;
      }
  }

  @.?  fail_for_invalid_at (श_माला);
}

<SC_AT_DIRECTIVE_SKIP_WS>
{
  [ \t\r\n]    जारी;
  .            yyless (0); BEGIN SC_AT_DIRECTIVE_ARGS;
}

<SC_AT_DIRECTIVE_ARGS,SC_AT_DIRECTIVE_SKIP_WS>
{
  <<खातापूर्ण>>  complain (शून्य, fatal, _("unclosed %s directive in skeleton"), argv[0]);
}

%%

अटल व्योम
at_init (पूर्णांक *argc, अक्षर *argv[], at_directive *at_ptr, at_directive fun)
{
  *at_ptr = fun;
  श_माला[yyleng-1] = '\0';
  obstack_grow (&obstack_for_string, श_माला, yyleng);
  argv[(*argc)++] = obstack_finish (&obstack_for_string);
  BEGIN SC_AT_DIRECTIVE_ARGS;
}

/*------------------------.
| Scan a Bison skeleton.  |
`------------------------*/

व्योम
scan_skel (खाता *in)
{
  अटल bool initialized = false;
  अगर (!initialized)
    {
      initialized = true;
      obstack_init (&obstack_for_string);
    }
  skel_in = in;
  skel__flex_debug = trace_flag & trace_skeleton;
  skel_lex ();
}

व्योम
skel_scanner_free (व्योम)
{
  obstack_free (&obstack_for_string, 0);
  /* Reclaim Flex's buffers.  */
  yylex_destroy ();
}

अटल अंतरभूत warnings
flag (स्थिर अक्षर *arg)
{
  /* compare with values issued from b4_error */
  अगर (STREQ (arg, "complain"))
    वापस complaint;
  अन्यथा अगर (STREQ (arg, "deprecated"))
    वापस Wdeprecated;
  अन्यथा अगर (STREQ (arg, "fatal"))
    वापस fatal;
  अन्यथा अगर (STREQ (arg, "note"))
    वापस silent | complaint | no_caret | note;
  अन्यथा अगर (STREQ (arg, "warn"))
    वापस Wother;
  अन्यथा
    पात ();
}

अटल व्योम
at_basename (पूर्णांक argc, अक्षर *argv[], अक्षर **out_namep, पूर्णांक *out_linenop)
{
  (व्योम) out_namep;
  (व्योम) out_linenop;
  अगर (2 < argc)
    fail_for_at_directive_too_many_args (argv[0]);
  ख_माला_दो (last_component (argv[1]), yyout);
}

अटल व्योम
at_complain (पूर्णांक argc, अक्षर *argv[], अक्षर **out_namep, पूर्णांक *out_linenop)
{
  अगर (argc < 4)
    fail_for_at_directive_too_few_args (argv[0]);

  (व्योम) out_namep;
  (व्योम) out_linenop;

  warnings w = flag (argv[1]);

  location loc;
  location *locp = शून्य;
  अगर (argv[2] && argv[2][0])
    {
      boundary_set_from_string (&loc.start, argv[2]);
      boundary_set_from_string (&loc.end, argv[3]);
      locp = &loc;
    }
  complain_args (locp, w, argc - 4, argv + 4);
}

अटल व्योम
at_output (पूर्णांक argc, अक्षर *argv[], अक्षर **out_namep, पूर्णांक *out_linenop)
{
  अगर (3 < argc)
    fail_for_at_directive_too_many_args (argv[0]);
  अगर (*out_namep)
    {
      मुक्त (*out_namep);
      xfclose (yyout);
    }
  *out_namep = xpath_join (argv[1], 2 < argc ? argv[2] : शून्य);
  output_file_name_check (out_namep, true);
  /* If there were errors, do not generate the output.  */
  yyout = xfopen (complaint_status ? "/dev/null" : *out_namep, "w");
  *out_linenop = 1;
}

अटल व्योम
fail_for_at_directive_too_few_args (अक्षर स्थिर *at_directive_name)
{
  complain (शून्य, fatal, _("too few arguments for %s directive in skeleton"),
            at_directive_name);
}

अटल व्योम
fail_for_at_directive_too_many_args (अक्षर स्थिर *at_directive_name)
{
  complain (शून्य, fatal, _("too many arguments for %s directive in skeleton"),
            at_directive_name);
}

अटल व्योम
fail_for_invalid_at (अक्षर स्थिर *at)
{
  complain (शून्य, fatal, "invalid @ in skeleton: %s", at);
}

अटल व्योम
output_mapped_file (अक्षर स्थिर *name)
{
  ख_माला_दो (quotearg_style (c_quoting_style, map_file_name (name)), yyout);
}
