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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ BISON_SYSTEM_H
# define BISON_SYSTEM_H

/* flex 2.5.31 gratuitously defines macros like INT8_MIN.  But this
   runs afoul of pre-C99 compilers that have <inttypes.h> or
   <stdint.h>, which are included below if available.  It also runs
   afoul of pre-C99 compilers that define these macros in <limits.h>.  */
# ಆದರೆ ! defined __STDC_VERSION__ || __STDC_VERSION__ < 199901
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

# define ARRAY_CARDINALITY(Array) (ಗಾತ್ರ (Array) / ಗಾತ್ರ *(Array))
# define STREQ(L, R)  (strcmp(L, R) == 0)
# define STRNEQ(L, R) (!STREQ(L, R))

/* Just like strncmp, but the second argument must be a literal string
   and you don't specify the length.  */
# define STRNCMP_LIT(S, Literal)                        \
  ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ (S, "" Literal "", ಗಾತ್ರ (Literal) - 1)

/* Whether Literal is a prefix of S.  */
# define STRPREFIX_LIT(Literal, S)              \
  (STRNCMP_LIT (S, Literal) == 0)

# include <unistd.h>
# include <inttypes.h>

# ifndef UINTPTR_MAX
/* This isn't perfect, but it's good enough for Bison, which needs
   only to hash pointers.  */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ size_t uintptr_t;
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
# ಆದರೆ defined __GNUC__ && !defined __clang__ && !defined __ICC
#  define GCC_ಆವೃತ್ತಿ (__GNUC__ * 100 + __GNUC_MINOR__)
# endif

// See https://lists.gnu.org/r/bug-bison/2019-10/msg00061.html
// and https://trac.macports.org/ticket/59927.
# ಆದರೆ defined GCC_ಆವೃತ್ತಿ && 405 <= GCC_ಆವೃತ್ತಿ
#  define IGNORE_TYPE_LIMITS_BEGIN \
     _Pragma ("GCC diagnostic push") \
     _Pragma ("GCC diagnostic ignored \"-Wtype-limits\"")
#  define IGNORE_TYPE_LIMITS_ಕೊನೆ \
     _Pragma ("GCC diagnostic pop")
# ಇಲ್ಲದಿದ್ದರೆ
#  define IGNORE_TYPE_LIMITS_BEGIN
#  define IGNORE_TYPE_LIMITS_ಕೊನೆ
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
# define PACIFY_CC(ಸಂಕೇತ_5) ಸಂಕೇತ_5

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

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
min_int (ಪೂರ್ಣಾಂಕ a, ಪೂರ್ಣಾಂಕ b)
{
  ಹಿಂತಿರುಗಿ a < b ? a : b;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
max_int (ಪೂರ್ಣಾಂಕ a, ಪೂರ್ಣಾಂಕ b)
{
  ಹಿಂತಿರುಗಿ a >= b ? a : b;
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
# define aver ಪ್ರತಿಜ್ಞಾ


/*-----------.
| Obstacks.  |
`-----------*/

# define obstack_chunk_alloc xmalloc
# define obstack_chunk_free  ಬಿಡುಗಡೆ
# include <obstack.h>

/* String-grow: append Str to Obs.  */

# define obstack_sgrow(Obs, Str) \
  obstack_grow (Obs, Str, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (Str))

/* Output Str escaped to be a string.

   For instance "\"foo\"" -> "\\\"foo\\\"".  */

# define obstack_backslash(Obs, Str)                    \
  ಮಾಡು {                                                  \
    ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *p__;                                    \
    ಗಾಗಿ (p__ = Str; *p__; p__++)                        \
      ಆಯ್ಕೆ (*p__)                                     \
        {                                               \
        ಸ್ಥಿತಿ '"':  obstack_sgrow (Obs, "\\\""); ನಿಲ್ಲಿಸು;  \
        ಸ್ಥಿತಿ '\\': obstack_sgrow (Obs, "\\\\"); ನಿಲ್ಲಿಸು;  \
        ಪೂರ್ವನಿಯೋಜಿತ:   obstack_1grow (Obs, *p__);   ನಿಲ್ಲಿಸು;  \
        }                                               \
  } ಆಗ (0)


/* Output CP escaped for our postprocessing (i.e., escape M4 special
   characters).

   For instance "[foo]" -> "@{foo@}", "$$" -> "$][$][".

   All the user's input passed to m4 is processed by this function or
   by output_escaped, except user actions.  Because of the specific
   handling of @$, @1, etc., user actions are processed (and escaped)
   by scan-code.l.

   Keep output_escaped and obstack_escape sync'ed.
 */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
obstack_escape (ರಚನೆ obstack* obs, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cp)
{
  ಗಾಗಿ (; *cp; ++cp)
    ಆಯ್ಕೆ (*cp)
      {
      ಸ್ಥಿತಿ '$': obstack_sgrow (obs, "$]["); ನಿಲ್ಲಿಸು;
      ಸ್ಥಿತಿ '@': obstack_sgrow (obs, "@@" ); ನಿಲ್ಲಿಸು;
      ಸ್ಥಿತಿ '[': obstack_sgrow (obs, "@{" ); ನಿಲ್ಲಿಸು;
      ಸ್ಥಿತಿ ']': obstack_sgrow (obs, "@}" ); ನಿಲ್ಲಿಸು;
      ಸ್ಥಿತಿ 'b':
        ಆದರೆ (STRPREFIX_LIT ("b4_", cp))
          {
            obstack_sgrow (obs, "b4@'_");
            cp += ಅಕ್ಷರಸರಣಿ_ಉದ್ದ ("b4_") - 1;
            ನಿಲ್ಲಿಸು;
          }
        ಇಲ್ಲದಿದ್ದರೆ
          ಹೋಗು append;

      ಸ್ಥಿತಿ 'm':
        ಆದರೆ (STRPREFIX_LIT ("m4_", cp))
          {
            obstack_sgrow (obs, "m4@'_");
            cp += ಅಕ್ಷರಸರಣಿ_ಉದ್ದ ("m4_") - 1;
            ನಿಲ್ಲಿಸು;
          }
        ಇಲ್ಲದಿದ್ದರೆ
          ಹೋಗು append;

      append:
      ಪೂರ್ವನಿಯೋಜಿತ:  obstack_1grow (obs, *cp); ನಿಲ್ಲಿಸು;
      }
}


/* Output Str both quoted for M4 (i.e., embed in [[...]]), and escaped
   for our postprocessing (i.e., escape M4 special characters).  If
   Str is empty (or NULL), output "[]" instead of "[[]]" as it makes
   M4 programming easier (m4_ifval can be used).

   For instance "[foo]" -> "[[@{foo@}]]", "$$" -> "[[$][$][]]".  */

# define obstack_quote(Obs, Str)                \
  ಮಾಡು {                                          \
    ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ* obstack_quote_p = Str;          \
    ಆದರೆ (obstack_quote_p && obstack_quote_p[0])  \
      {                                         \
        obstack_sgrow (Obs, "[[");              \
        obstack_escape (Obs, obstack_quote_p);  \
        obstack_sgrow (Obs, "]]");              \
      }                                         \
    ಇಲ್ಲದಿದ್ದರೆ                                        \
      obstack_sgrow (Obs, "[]");                \
  } ಆಗ (0)


/* Append the ending 0, finish Obs, and return the string.  */

# define obstack_finish0(Obs)                                   \
  (obstack_1grow (Obs, '\0'), (ಅಕ್ಷರ *) obstack_ಮುಕ್ತಾಯ (Obs))


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
  ಮಾಡು {                                          \
    Type *ಗಂಟು_2, *ಮುಂದಿನ_2;                        \
    ಗಾಗಿ (ಗಂಟು_2 = List; ಗಂಟು_2; ಗಂಟು_2 = ಮುಂದಿನ_2)    \
      {                                         \
        ಮುಂದಿನ_2 = ಗಂಟು_2->ಮುಂದಿನ;                    \
        ಬಿಡುಗಡೆ (ಗಂಟು_2);                           \
      }                                         \
  } ಆಗ (0)

#ಆದರೆ_ಅಂತ್ಯ  /* ! BISON_SYSTEM_H */
