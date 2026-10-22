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

#ifndef BISON_SYSTEM_H
# define BISON_SYSTEM_H

/* flex 2.5.31 gratuitously defines macros like INT8_MIN.  But this
   runs afoul of pre-C99 compilers that have <inttypes.h> or
   <stdint.h>, which are included below if available.  It also runs
   afoul of pre-C99 compilers that define these macros in <limits.h>.  */
# যদি ! defined __STDC_VERSION__ || __STDC_VERSION__ < 199901
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

# define ARRAY_CARDINALITY(Array) (পরিমাণ (Array) / পরিমাণ *(Array))
# define STREQ(L, R)  (strcmp(L, R) == 0)
# define STRNEQ(L, R) (!STREQ(L, R))

/* Just like strncmp, but the second argument must be a literal string
   and you don't specify the length.  */
# define STRNCMP_LIT(S, Literal)                        \
  strncmp (S, "" Literal "", পরিমাণ (Literal) - 1)

/* Whether Literal is a prefix of S.  */
# define STRPREFIX_LIT(Literal, S)              \
  (STRNCMP_LIT (S, Literal) == 0)

# include <unistd.h>
# include <inttypes.h>

# ifndef UINTPTR_MAX
/* This isn't perfect, but it's good enough for Bison, which needs
   only to hash pointers.  */
প্রকারনির্দেশ size_t uintptr_t;
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
# যদি defined __GNUC__ && !defined __clang__ && !defined __ICC
#  define GCC_VERSION (__GNUC__ * 100 + __GNUC_MINOR__)
# endif

// See https://lists.gnu.org/r/bug-bison/2019-10/msg00061.html
// and https://trac.macports.org/ticket/59927.
# যদি defined GCC_VERSION && 405 <= GCC_VERSION
#  define IGNORE_TYPE_LIMITS_BEGIN \
     _Pragma ("GCC diagnostic push") \
     _Pragma ("GCC diagnostic ignored \"-Wtype-limits\"")
#  define IGNORE_TYPE_LIMITS_END \
     _Pragma ("GCC diagnostic pop")
# অন্যথা
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

স্থির অন্তঃস্থ পূর্ণাঙ্ক
min_int (পূর্ণাঙ্ক a, পূর্ণাঙ্ক b)
{
  ঘূৰাই a < b ? a : b;
}

স্থির অন্তঃস্থ পূর্ণাঙ্ক
max_int (পূর্ণাঙ্ক a, পূর্ণাঙ্ক b)
{
  ঘূৰাই a >= b ? a : b;
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
# define aver প্রতিজ্ঞা


/*-----------.
| Obstacks.  |
`-----------*/

# define obstack_chunk_alloc xmalloc
# define obstack_chunk_free  মুক্ত
# include <obstack.h>

/* String-grow: append Str to Obs.  */

# define obstack_sgrow(Obs, Str) \
  obstack_grow (Obs, Str, strlen (Str))

/* Output Str escaped to be a string.

   For instance "\"foo\"" -> "\\\"foo\\\"".  */

# define obstack_backslash(Obs, Str)                    \
  কৰা {                                                  \
    ৱর্ণ ধ্রুৱ *p__;                                    \
    বাবে (p__ = Str; *p__; p__++)                        \
      ৱিকল্প (*p__)                                     \
        {                                               \
        পক্ষ '"':  obstack_sgrow (Obs, "\\\""); ভঙ্গ;  \
        পক্ষ '\\': obstack_sgrow (Obs, "\\\\"); ভঙ্গ;  \
        স্ৱভাৱ:   obstack_1grow (Obs, *p__);   ভঙ্গ;  \
        }                                               \
  } যেতিয়ালৈ (0)


/* Output CP escaped for our postprocessing (i.e., escape M4 special
   characters).

   For instance "[foo]" -> "@{foo@}", "$$" -> "$][$][".

   All the user's input passed to m4 is processed by this function or
   by output_escaped, except user actions.  Because of the specific
   handling of @$, @1, etc., user actions are processed (and escaped)
   by scan-code.l.

   Keep output_escaped and obstack_escape sync'ed.
 */

স্থির অন্তঃস্থ শূন্য
obstack_escape (সংরচনা obstack* obs, ধ্রুৱ ৱর্ণ *cp)
{
  বাবে (; *cp; ++cp)
    ৱিকল্প (*cp)
      {
      পক্ষ '$': obstack_sgrow (obs, "$]["); ভঙ্গ;
      পক্ষ '@': obstack_sgrow (obs, "@@" ); ভঙ্গ;
      পক্ষ '[': obstack_sgrow (obs, "@{" ); ভঙ্গ;
      পক্ষ ']': obstack_sgrow (obs, "@}" ); ভঙ্গ;
      পক্ষ 'b':
        যদি (STRPREFIX_LIT ("b4_", cp))
          {
            obstack_sgrow (obs, "b4@'_");
            cp += strlen ("b4_") - 1;
            ভঙ্গ;
          }
        অন্যথা
          গচ্ছ append;

      পক্ষ 'm':
        যদি (STRPREFIX_LIT ("m4_", cp))
          {
            obstack_sgrow (obs, "m4@'_");
            cp += strlen ("m4_") - 1;
            ভঙ্গ;
          }
        অন্যথা
          গচ্ছ append;

      append:
      স্ৱভাৱ:  obstack_1grow (obs, *cp); ভঙ্গ;
      }
}


/* Output Str both quoted for M4 (i.e., embed in [[...]]), and escaped
   for our postprocessing (i.e., escape M4 special characters).  If
   Str is empty (or NULL), output "[]" instead of "[[]]" as it makes
   M4 programming easier (m4_ifval can be used).

   For instance "[foo]" -> "[[@{foo@}]]", "$$" -> "[[$][$][]]".  */

# define obstack_quote(Obs, Str)                \
  কৰা {                                          \
    ৱর্ণ ধ্রুৱ* obstack_quote_p = Str;          \
    যদি (obstack_quote_p && obstack_quote_p[0])  \
      {                                         \
        obstack_sgrow (Obs, "[[");              \
        obstack_escape (Obs, obstack_quote_p);  \
        obstack_sgrow (Obs, "]]");              \
      }                                         \
    অন্যথা                                        \
      obstack_sgrow (Obs, "[]");                \
  } যেতিয়ালৈ (0)


/* Append the ending 0, finish Obs, and return the string.  */

# define obstack_finish0(Obs)                                   \
  (obstack_1grow (Obs, '\0'), (ৱর্ণ *) obstack_finish (Obs))


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
  কৰা {                                          \
    Type *গ্ৰন্থি_2, *_next;                        \
    বাবে (গ্ৰন্থি_2 = List; গ্ৰন্থি_2; গ্ৰন্থি_2 = _next)    \
      {                                         \
        _next = গ্ৰন্থি_2->next;                    \
        মুক্ত (গ্ৰন্থি_2);                           \
      }                                         \
  } যেতিয়ালৈ (0)

#endif  /* ! BISON_SYSTEM_H */
