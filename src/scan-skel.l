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
#अन्तर्भाव <dirname.h>
#अन्तर्भाव <error.h>
#अन्तर्भाव <path-join.h>
#अन्तर्भाव <quotearg.h>

#अन्तर्भाव "src/complain.h"
#अन्तर्भाव "src/files.h"
#अन्तर्भाव "src/getargs.h"
#अन्तर्भाव "src/scan-skel.h"

#define FLEX_PREFIX(Id) skel_ ## Id
#अन्तर्भाव "src/flex-scanner.h"

/* Work around a bug in flex 2.5.31.  See Debian bug 333231
   <https://bugs.debian.org/333231>.  */
#undef skel_wrap
#define skel_wrap() 1

#define YY_DECL स्थिर पूर्णाङ्क skel_lex (शून्य)
YY_DECL;

प्रकारनिर्देश शून्य (*at_directive)(पूर्णाङ्क, वर्ण**, वर्ण **, पूर्णाङ्क*);
स्थिर शून्य at_init (पूर्णाङ्क *argc, वर्ण *argv[], at_directive *at_ptr, at_directive fun);
स्थिर शून्य at_basename (पूर्णाङ्क argc, वर्ण *argv[], वर्ण**, पूर्णाङ्क*);
स्थिर शून्य at_complain (पूर्णाङ्क argc, वर्ण *argv[], वर्ण**, पूर्णाङ्क*);
स्थिर शून्य at_output (पूर्णाङ्क argc, वर्ण *argv[], वर्ण **name, पूर्णाङ्क *lineno);
स्थिर शून्य fail_for_at_directive_too_many_args (वर्ण ध्रुव *at_directive_name);
स्थिर शून्य fail_for_at_directive_too_few_args (वर्ण ध्रुव *at_directive_name);
स्थिर शून्य fail_for_invalid_at (वर्ण ध्रुव *at);
स्थिर शून्य output_mapped_file (वर्ण ध्रुव *name);
%}

/* Identifiers of our M4 macros.  */
macro      [bm]4_[a-zA-Z_0-9]*
/* Safe sequence of word-constituent characters.  */
identifier [A-Za-z_0-9]+

%x SC_AT_DIRECTIVE_ARGS
%x SC_AT_DIRECTIVE_SKIP_WS

%%

%{
  पूर्णाङ्क out_lineno PACIFY_CC (= 0);
  वर्ण *out_name = रिक्त;

  /* Currently, only the @complain directive takes multiple arguments, and
     never more than 7, with argv[0] being the directive name and argv[1]
     being the type of complaint to dispatch. */
#define ARGC_MAX 9
  पूर्णाङ्क argc = 0;
  वर्ण *argv[ARGC_MAX];
  at_directive at_ptr = रिक्त;
%}

"@@" ख_अक्षर_दो ('@', yyout);
"@{" ख_अक्षर_दो ('[', yyout);
"@}" ख_अक्षर_दो (']', yyout);
"@'" अनुवर्त;  /* Used by b4_cat in ../data/bison.m4.  */
@\n  अनुवर्त;

"@oline@"  ख_लिखो (yyout, "%d", out_lineno + 1);
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
                  loc.start.पंक्ति = out_lineno;
                  loc.end = loc.start;
                  complain (&loc, Wother,
                            "suspicious sequence in the output: %s", yytext);
                  ECHO;
                }
{identifier}    ECHO;
.               ECHO;

<INITIAL><<खातापूर्ण>> {
  यदि (out_name)
    {
      मुक्त (out_name);
      xfclose (yyout);
    }
  प्रत्यागम खातापूर्ण;
}

<SC_AT_DIRECTIVE_ARGS>
{
  [^@]+  STRING_GROW ();

  "@@"   STRING_1GROW ('@');
  "@{"   STRING_1GROW ('[');
  "@}"   STRING_1GROW (']');
  "@'"   अनुवर्त; /* For starting an argument that begins with whitespace. */
  @\n    अनुवर्त;

  @[,)] {
    यदि (argc >= ARGC_MAX)
      fail_for_at_directive_too_many_args (argv[0]);

    argv[argc++] = obstack_finish0 (&obstack_for_string);

    /* Like M4, skip whitespace after a comma.  */
    यदि (yytext[1] == ',')
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

  @.?  fail_for_invalid_at (yytext);
}

<SC_AT_DIRECTIVE_SKIP_WS>
{
  [ \t\r\n]    अनुवर्त;
  .            yyless (0); BEGIN SC_AT_DIRECTIVE_ARGS;
}

<SC_AT_DIRECTIVE_ARGS,SC_AT_DIRECTIVE_SKIP_WS>
{
  <<खातापूर्ण>>  complain (रिक्त, fatal, _("unclosed %s directive in skeleton"), argv[0]);
}

%%

स्थिर शून्य
at_init (पूर्णाङ्क *argc, वर्ण *argv[], at_directive *at_ptr, at_directive fun)
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

शून्य
scan_skel (खाता *in)
{
  स्थिर bool initialized = false;
  यदि (!initialized)
    {
      initialized = true;
      obstack_init (&obstack_for_string);
    }
  skel_in = in;
  skel__flex_debug = trace_flag & trace_skeleton;
  skel_lex ();
}

शून्य
skel_scanner_free (शून्य)
{
  obstack_free (&obstack_for_string, 0);
  /* Reclaim Flex's buffers.  */
  yylex_destroy ();
}

स्थिर अन्तःस्थ warnings
flag (ध्रुव वर्ण *arg)
{
  /* compare with values issued from b4_error */
  यदि (STREQ (arg, "complain"))
    प्रत्यागम complaint;
  अन्यथा यदि (STREQ (arg, "deprecated"))
    प्रत्यागम Wdeprecated;
  अन्यथा यदि (STREQ (arg, "fatal"))
    प्रत्यागम fatal;
  अन्यथा यदि (STREQ (arg, "note"))
    प्रत्यागम silent | complaint | no_caret | note;
  अन्यथा यदि (STREQ (arg, "warn"))
    प्रत्यागम Wother;
  अन्यथा
    पात ();
}

स्थिर शून्य
at_basename (पूर्णाङ्क argc, वर्ण *argv[], वर्ण **out_namep, पूर्णाङ्क *out_linenop)
{
  (शून्य) out_namep;
  (शून्य) out_linenop;
  यदि (2 < argc)
    fail_for_at_directive_too_many_args (argv[0]);
  ख_माला_दो (last_component (argv[1]), yyout);
}

स्थिर शून्य
at_complain (पूर्णाङ्क argc, वर्ण *argv[], वर्ण **out_namep, पूर्णाङ्क *out_linenop)
{
  यदि (argc < 4)
    fail_for_at_directive_too_few_args (argv[0]);

  (शून्य) out_namep;
  (शून्य) out_linenop;

  warnings w = flag (argv[1]);

  location loc;
  location *locp = रिक्त;
  यदि (argv[2] && argv[2][0])
    {
      boundary_set_from_string (&loc.start, argv[2]);
      boundary_set_from_string (&loc.end, argv[3]);
      locp = &loc;
    }
  complain_args (locp, w, argc - 4, argv + 4);
}

स्थिर शून्य
at_output (पूर्णाङ्क argc, वर्ण *argv[], वर्ण **out_namep, पूर्णाङ्क *out_linenop)
{
  यदि (3 < argc)
    fail_for_at_directive_too_many_args (argv[0]);
  यदि (*out_namep)
    {
      मुक्त (*out_namep);
      xfclose (yyout);
    }
  *out_namep = xpath_join (argv[1], 2 < argc ? argv[2] : रिक्त);
  output_file_name_check (out_namep, true);
  /* If there were errors, do not generate the output.  */
  yyout = xfopen (complaint_status ? "/dev/null" : *out_namep, "w");
  *out_linenop = 1;
}

स्थिर शून्य
fail_for_at_directive_too_few_args (वर्ण ध्रुव *at_directive_name)
{
  complain (रिक्त, fatal, _("too few arguments for %s directive in skeleton"),
            at_directive_name);
}

स्थिर शून्य
fail_for_at_directive_too_many_args (वर्ण ध्रुव *at_directive_name)
{
  complain (रिक्त, fatal, _("too many arguments for %s directive in skeleton"),
            at_directive_name);
}

स्थिर शून्य
fail_for_invalid_at (वर्ण ध्रुव *at)
{
  complain (रिक्त, fatal, "invalid @ in skeleton: %s", at);
}

स्थिर शून्य
output_mapped_file (वर्ण ध्रुव *name)
{
  ख_माला_दो (quotearg_style (c_quoting_style, map_file_name (name)), yyout);
}
