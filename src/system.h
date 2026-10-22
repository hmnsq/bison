/* System-dependent definitions for Bison.

   Copyright (C) 2000-2007, 2009-2015, 2018-2022 Free Software
   Foundation, Inc.

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

#अगर_अघोषित BISON_SYSTEM_H
# define BISON_SYSTEM_H

/* flex 2.5.31 gratuitously defines macros like INT8_MIN.  But this
   runs afoul of pre-C99 compilers that have <inttypes.h> or
   <stdint.h>, which are included below if available.  It also runs
   afoul of pre-C99 compilers that define these macros in <limits.h>.  */
# अगर ! defined __STDC_VERSION__ || __STDC_VERSION__ < 199901
#  undef INT8_MIN
#  undef INT16_MIN
#  undef INT32_MIN
#  undef INT8_MAX
#  undef INT16_MAX
#  undef UINT8_MAX
#  undef INT32_MAX
#  undef UINT16_MAX
#  undef UINT32_MAX
# endif

# include <limits.h>
# include <stddef.h>
# include <stdlib.h>
# include <string.h>

# define ARRAY_CARDINALITY(Array) (माप (Array) / माप *(Array))
# define STREQ(L, R)  (म_भेद(L, R) == 0)
# define STRNEQ(L, R) (!STREQ(L, R))

/* Just like strncmp, but the second argument must be a literal string
   and you don't specify the length.  */
# define STRNCMP_LIT(S, Literal)                        \
  म_भेदन (S, "" Literal "", माप (Literal) - 1)

/* Whether Literal is a prefix of S.  */
# define STRPREFIX_LIT(Literal, S)              \
  (STRNCMP_LIT (S, Literal) == 0)

# include <unistd.h>
# include <inttypes.h>

# ifndef UINTPTR_MAX
/* This isn't perfect, but it's good enough for Bison, which needs
   only to hash pointers.  */
प्रकार माप_प्रकार uintptr_t;
# endif

/* Version mismatch. */
# define EX_MISMATCH 63

/*---------.
| Gnulib.  |
`---------*/

# include <unlocked-io.h>
# include <verify.h>
# include <xalloc.h>

// Clang and ICC like to pretend they are GCC.
# अगर defined __GNUC__ && !defined __clang__ && !defined __ICC
#  define GCC_VERSION (__GNUC__ * 100 + __GNUC_MINOR__)
# endif

// See https://lists.gnu.org/r/bug-bison/2019-10/msg00061.html
// and https://trac.macports.org/ticket/59927.
# अगर defined GCC_VERSION && 405 <= GCC_VERSION
#  define IGNORE_TYPE_LIMITS_BEGIN \
     _Pragma ("GCC diagnostic push") \
     _Pragma ("GCC diagnostic ignored \"-Wtype-limits\"")
#  define IGNORE_TYPE_LIMITS_END \
     _Pragma ("GCC diagnostic pop")
# अन्यथा
#  define IGNORE_TYPE_LIMITS_BEGIN
#  define IGNORE_TYPE_LIMITS_END
# endif


/*-----------------.
| GCC extensions.  |
`-----------------*/

/* Use PACIFY_CC to indicate that Code is unimportant to the logic of Bison
   but that it is necessary for suppressing compiler warnings.  For example,
   Code might be a variable initializer that's always overwritten before the
   variable is used.

   PACIFY_CC is intended to be useful only as a comment as it does not alter
   Code.  It is tempting to redefine PACIFY_CC so that it will suppress Code
   when configuring without --enable-gcc-warnings.  However, that would mean
   that, for maintainers, Bison would compile with potentially less warnings
   and safer logic than it would for users.  Due to the overhead of M4,
   suppressing Code is unlikely to offer any significant improvement in
   Bison's performance anyway.  */
# define PACIFY_CC(Code) Code

# include <attribute.h>


/*------.
| NLS.  |
`------*/

# include <locale.h>

# include <gettext.h>
# define _(Msgid)  gettext (Msgid)
# define N_(Msgid) (Msgid)


/*-----------.
| Booleans.  |
`-----------*/

# include <stdbool.h>


/*-----------.
| Integers.  |
`-----------*/

अटल अंतरभूत पूर्णांक
min_int (पूर्णांक a, पूर्णांक b)
{
  वापस a < b ? a : b;
}

अटल अंतरभूत पूर्णांक
max_int (पूर्णांक a, पूर्णांक b)
{
  वापस a >= b ? a : b;
}


/*-------------.
| Assertions.  |
`-------------*/

/* In the past, Bison defined aver to simply invoke abort in the case of
   a failed assertion.  The rationale was that <assert.h>'s assertions
   were too heavyweight and could be disabled too easily.  See
   discussions at
   <https://lists.gnu.org/r/bison-patches/2006-01/msg00080.html>
   <https://lists.gnu.org/r/bison-patches/2006-09/msg00111.html>.

   However, normal assert output can be helpful during development and
   in bug reports from users.  Moreover, it's not clear now that
   <assert.h>'s assertions are significantly heavyweight.  Finally, if
   users want to experiment with disabling assertions, it's debatable
   whether it's our responsibility to stop them.  See discussion
   starting at
   <https://lists.gnu.org/r/bison-patches/2009-09/msg00013.html>.

   For now, we use assert but we call it aver throughout Bison in case
   we later wish to try another scheme.
*/
# include <assert.h>
# define aver निश्चित


/*-----------.
| Obstacks.  |
`-----------*/

# define obstack_chunk_alloc xmalloc
# define obstack_chunk_free  मुक्त
# include <obstack.h>

/* String-grow: append Str to Obs.  */

# define obstack_sgrow(Obs, Str) \
  obstack_grow (Obs, Str, म_माप (Str))

/* Output Str escaped to be a string.

   For instance "\"foo\"" -> "\\\"foo\\\"".  */

# define obstack_backslash(Obs, Str)                    \
  करो {                                                  \
    अक्षर स्थिर *p__;                                    \
    क्रम (p__ = Str; *p__; p__++)                        \
      चयन (*p__)                                     \
        {                                               \
        हाल '"':  obstack_sgrow (Obs, "\\\""); अवरोध;  \
        हाल '\\': obstack_sgrow (Obs, "\\\\"); अवरोध;  \
        शेष:   obstack_1grow (Obs, *p__);   अवरोध;  \
        }                                               \
  } जबतक (0)


/* Output CP escaped for our postprocessing (i.e., escape M4 special
   characters).

   For instance "[foo]" -> "@{foo@}", "$$" -> "$][$][".

   All the user's input passed to m4 is processed by this function or
   by output_escaped, except user actions.  Because of the specific
   handling of @$, @1, etc., user actions are processed (and escaped)
   by scan-code.l.

   Keep output_escaped and obstack_escape sync'ed.
 */

अटल अंतरभूत व्योम
obstack_escape (काष्ठा obstack* obs, स्थिर अक्षर *cp)
{
  क्रम (; *cp; ++cp)
    चयन (*cp)
      {
      हाल '$': obstack_sgrow (obs, "$]["); अवरोध;
      हाल '@': obstack_sgrow (obs, "@@" ); अवरोध;
      हाल '[': obstack_sgrow (obs, "@{" ); अवरोध;
      हाल ']': obstack_sgrow (obs, "@}" ); अवरोध;
      हाल 'b':
        अगर (STRPREFIX_LIT ("b4_", cp))
          {
            obstack_sgrow (obs, "b4@'_");
            cp += म_माप ("b4_") - 1;
            अवरोध;
          }
        अन्यथा
          जाओ append;

      हाल 'm':
        अगर (STRPREFIX_LIT ("m4_", cp))
          {
            obstack_sgrow (obs, "m4@'_");
            cp += म_माप ("m4_") - 1;
            अवरोध;
          }
        अन्यथा
          जाओ append;

      append:
      शेष:  obstack_1grow (obs, *cp); अवरोध;
      }
}


/* Output Str both quoted for M4 (i.e., embed in [[...]]), and escaped
   for our postprocessing (i.e., escape M4 special characters).  If
   Str is empty (or NULL), output "[]" instead of "[[]]" as it makes
   M4 programming easier (m4_ifval can be used).

   For instance "[foo]" -> "[[@{foo@}]]", "$$" -> "[[$][$][]]".  */

# define obstack_quote(Obs, Str)                \
  करो {                                          \
    अक्षर स्थिर* obstack_quote_p = Str;          \
    अगर (obstack_quote_p && obstack_quote_p[0])  \
      {                                         \
        obstack_sgrow (Obs, "[[");              \
        obstack_escape (Obs, obstack_quote_p);  \
        obstack_sgrow (Obs, "]]");              \
      }                                         \
    अन्यथा                                        \
      obstack_sgrow (Obs, "[]");                \
  } जबतक (0)


/* Append the ending 0, finish Obs, and return the string.  */

# define obstack_finish0(Obs)                                   \
  (obstack_1grow (Obs, '\0'), (अक्षर *) obstack_finish (Obs))


/*-----------------------------------------.
| Extensions to use for the output files.  |
`-----------------------------------------*/

# ifndef OUTPUT_EXT
#  define OUTPUT_EXT ".output"
# endif

# ifndef TAB_EXT
#  define TAB_EXT ".tab"
# endif



/*---------------------.
| Free a linked list.  |
`---------------------*/

# define LIST_FREE(Type, List)                  \
  करो {                                          \
    Type *गाँठ_2, *अग्र_2;                        \
    क्रम (गाँठ_2 = List; गाँठ_2; गाँठ_2 = अग्र_2)    \
      {                                         \
        अग्र_2 = गाँठ_2->अग्र;                    \
        मुक्त (गाँठ_2);                           \
      }                                         \
  } जबतक (0)

#पूर्ण_अगर  /* ! BISON_SYSTEM_H */
