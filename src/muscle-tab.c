/* Muscle table manager for Bison.

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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ <hash.h>
#ಅನ್ತರ್ಭಾವ <quote.h>

#ಅನ್ತರ್ಭಾವ "complain.h"
#ಅನ್ತರ್ಭಾವ "files.h"
#ಅನ್ತರ್ಭಾವ "fixits.h"
#ಅನ್ತರ್ಭಾವ "getargs.h"
#ಅನ್ತರ್ಭಾವ "muscle-tab.h"

muscle_ಬಗೆ
muscle_ಬಗೆ_new (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *k)
{
  ಆದರೆ (STREQ (k, "code"))
    ಹಿಂತಿರುಗಿ muscle_ಸಂಕೇತ;
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (STREQ (k, "keyword"))
    ಹಿಂತಿರುಗಿ muscle_keyword;
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (STREQ (k, "string"))
    ಹಿಂತಿರುಗಿ muscle_string;
  abort ();
}

ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *
muscle_ಬಗೆ_string (muscle_ಬಗೆ k)
{
  ಆಯ್ಕೆ (k)
    {
    ಸ್ಥಿತಿ muscle_ಸಂಕೇತ:    ಹಿಂತಿರುಗಿ "code";
    ಸ್ಥಿತಿ muscle_keyword: ಹಿಂತಿರುಗಿ "keyword";
    ಸ್ಥಿತಿ muscle_string:  ಹಿಂತಿರುಗಿ "string";
    }
  abort ();
}


/* A key-value pair, along with storage that can be reclaimed when
   this pair is no longer needed.  */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ
{
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಕೀಲಿ;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಮೌಲ್ಯ;
  ಅಕ್ಷರ *storage;
  muscle_ಬಗೆ ಬಗೆ;
} muscle_ನಮೂದು;


/* The name of muscle for the %define variable VAR (corresponding to
   FIELD, if defined).  */
ಸ್ಥಿರ uniqstr
muscle_name (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *var, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಕ್ಷೇತ್ರ)
{
  ಆದರೆ (ಕ್ಷೇತ್ರ)
    ಹಿಂತಿರುಗಿ UNIQSTR_CONCAT ("percent_define_", ಕ್ಷೇತ್ರ, "(", var, ")");
  ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ UNIQSTR_CONCAT ("percent_define(", var, ")");
}

/* An obstack used to create some entries.  */
ರಚನೆ obstack muscle_obstack;

/* Initial capacity of muscles hash table.  */
#ವ್ಯಾಖ್ಯಾನಿಸು HT_INITIAL_ಸಾಮರ್ಥ್ಯ 257

ಸ್ಥಿರ ರಚನೆ hash_ಕೋಷ್ಟಕ *muscle_ಕೋಷ್ಟಕ = ಖಾಲಿ;

ಸ್ಥಿರ bool
hash_compare_muscles (ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *x, ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *y)
{
  muscle_ನಮೂದು ಸ್ಥಿರಾಂಕ *m1 = x;
  muscle_ನಮೂದು ಸ್ಥಿರಾಂಕ *m2 = y;
  ಹಿಂತಿರುಗಿ STREQ (m1->ಕೀಲಿ, m2->ಕೀಲಿ);
}

ಸ್ಥಿರ size_t
hash_muscle (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *x, size_t tablesize)
{
  muscle_ನಮೂದು ಸ್ಥಿರಾಂಕ *m = x;
  ಹಿಂತಿರುಗಿ hash_string (m->ಕೀಲಿ, tablesize);
}

/* Create a fresh muscle name KEY, and insert in the hash table.  */
ಸ್ಥಿರ ಶೂನ್ಯ *
muscle_ನಮೂದು_new (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಕೀಲಿ)
{
  muscle_ನಮೂದು *res = xmalloc (ಗಾತ್ರ *res);
  res->ಕೀಲಿ = ಕೀಲಿ;
  res->ಮೌಲ್ಯ = ಖಾಲಿ;
  res->storage = ಖಾಲಿ;
  hash_xinsert (muscle_ಕೋಷ್ಟಕ, res);
  ಹಿಂತಿರುಗಿ res;
}

ಸ್ಥಿರ ಶೂನ್ಯ
muscle_ನಮೂದು_free (ಶೂನ್ಯ *ನಮೂದು)
{
  muscle_ನಮೂದು *mentry = ನಮೂದು;
  ಬಿಡುಗಡೆ (mentry->storage);
  ಬಿಡುಗಡೆ (mentry);
}

ಶೂನ್ಯ
muscle_init (ಶೂನ್ಯ)
{
  /* Initialize the muscle obstack.  */
  obstack_init (&muscle_obstack);

  muscle_ಕೋಷ್ಟಕ = hash_xinitialize (HT_INITIAL_ಸಾಮರ್ಥ್ಯ, ಖಾಲಿ, hash_muscle,
                                   hash_compare_muscles, muscle_ನಮೂದು_free);
  /* Avoid warnings if the user defined this variable, but did not
     actually call -Wcex.  */
  ಬಿಡುಗಡೆ (muscle_percent_define_get ("cex.timeout"));
}


ಶೂನ್ಯ
muscle_free (ಶೂನ್ಯ)
{
  hash_free (muscle_ಕೋಷ್ಟಕ);
  obstack_free (&muscle_obstack, ಖಾಲಿ);
}

/* Look for the muscle named KEY.  Return NULL if does not exist.  */
ಸ್ಥಿರ muscle_ನಮೂದು *
muscle_lookup (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಕೀಲಿ)
{
  muscle_ನಮೂದು probe;
  probe.ಕೀಲಿ = ಕೀಲಿ;
  ಹಿಂತಿರುಗಿ hash_lookup (muscle_ಕೋಷ್ಟಕ, &probe);
}


ಶೂನ್ಯ
muscle_ಸೇರಿಸು (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಕೀಲಿ, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಮೌಲ್ಯ)
{
  muscle_ನಮೂದು *ನಮೂದು = muscle_lookup (ಕೀಲಿ);
  ಆದರೆ (ನಮೂದು)
    ಬಿಡುಗಡೆ (ನಮೂದು->storage);
  ಇಲ್ಲದಿದ್ದರೆ
    /* First insertion in the hash. */
    ನಮೂದು = muscle_ನಮೂದು_new (ಕೀಲಿ);
  ನಮೂದು->ಮೌಲ್ಯ = ಮೌಲ್ಯ;
  ನಮೂದು->storage = ಖಾಲಿ;
}


/* Append VALUE to the current value of KEY.  If KEY did not already
   exist, create it.  Use MUSCLE_OBSTACK.  De-allocate the previously
   associated value.  Copy VALUE and SEPARATOR.  If VALUE does not end
   with TERMINATOR, append one.  */

ಸ್ಥಿರ ಶೂನ್ಯ
muscle_grow (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *val,
             ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *separator, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *terminator)
{
  muscle_ನಮೂದು *ನಮೂದು = muscle_lookup (ಕೀಲಿ);
  ಆದರೆ (ನಮೂದು)
    {
      obstack_sgrow (&muscle_obstack, ನಮೂದು->ಮೌಲ್ಯ);
      obstack_sgrow (&muscle_obstack, separator);
      ಬಿಡುಗಡೆ (ನಮೂದು->storage);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    ನಮೂದು = muscle_ನಮೂದು_new (ಕೀಲಿ);

  obstack_sgrow (&muscle_obstack, val);

  size_t vals = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (val);
  size_t terms = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (terminator);
  ಆದರೆ (terms <= vals
      && STRNEQ (val + vals - terms, terminator))
    obstack_sgrow (&muscle_obstack, terminator);

  {
    ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *new_val = obstack_finish0 (&muscle_obstack);
    ನಮೂದು->ಮೌಲ್ಯ = ನಮೂದು->storage = xstrdup (new_val);
    obstack_free (&muscle_obstack, new_val);
  }
}

/*------------------------------------------------------------------.
| Using muscle_grow, append a synchronization line for the location |
| LOC to the current value of KEY.                                  |
`------------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
muscle_syncline_grow (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಕೀಲಿ, location loc)
{
  obstack_printf (&muscle_obstack, "]b4_syncline(%d, ", loc.ಆರಂಭ.ಸಾಲು);
  obstack_quote (&muscle_obstack,
                 quotearg_style (c_quoting_style, map_file_name (loc.ಆರಂಭ.file)));
  obstack_sgrow (&muscle_obstack, ")dnl\n[");
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *extension = obstack_finish0 (&muscle_obstack);
  muscle_grow (ಕೀಲಿ, extension, "", "");
  obstack_free (&muscle_obstack, extension);
}

/*------------------------------------------------------------------.
| Append VALUE to the current value of KEY, using muscle_grow.  But |
| in addition, issue a synchronization line for the location LOC    |
| using muscle_syncline_grow.                                       |
`------------------------------------------------------------------*/

ಶೂನ್ಯ
muscle_ಸಂಕೇತ_grow (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *val, location loc)
{
  muscle_syncline_grow (ಕೀಲಿ, loc);
  muscle_grow (ಕೀಲಿ, val, "", "\n");
}


ಶೂನ್ಯ
muscle_pair_list_grow (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *muscle,
                       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *a1, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *a2)
{
  obstack_sgrow (&muscle_obstack, "[");
  obstack_quote (&muscle_obstack, a1);
  obstack_sgrow (&muscle_obstack, ", ");
  obstack_quote (&muscle_obstack, a2);
  obstack_sgrow (&muscle_obstack, "]");
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *pair = obstack_finish0 (&muscle_obstack);
  muscle_grow (muscle, pair, ",\n", "");
  obstack_free (&muscle_obstack, pair);
}


ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *
muscle_ಕಂಡುಹಿಡಿ_const (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಕೀಲಿ)
{
  muscle_ನಮೂದು *ನಮೂದು = muscle_lookup (ಕೀಲಿ);
  ಹಿಂತಿರುಗಿ ನಮೂದು ? ನಮೂದು->ಮೌಲ್ಯ : ಖಾಲಿ;
}


ಅಕ್ಷರ *
muscle_ಕಂಡುಹಿಡಿ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಕೀಲಿ)
{
  muscle_ನಮೂದು *ನಮೂದು = muscle_lookup (ಕೀಲಿ);
  ಆದರೆ (ನಮೂದು)
    {
      aver (ನಮೂದು->ಮೌಲ್ಯ == ನಮೂದು->storage);
      ಹಿಂತಿರುಗಿ ನಮೂದು->storage;
    }
  ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}


/* In the format 'file_name:line.column', append BOUND to MUSCLE.  Use
   digraphs for special characters in the file name.  */

ಸ್ಥಿರ ಶೂನ್ಯ
muscle_boundary_grow (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಕೀಲಿ, boundary bound)
{
  obstack_sgrow  (&muscle_obstack, "[[");
  obstack_escape (&muscle_obstack, bound.file);
  obstack_printf (&muscle_obstack, ":%d.%d@@%d]]", bound.ಸಾಲು, bound.ಸ್ತಂಭ, bound.byte);
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *extension = obstack_finish0 (&muscle_obstack);
  muscle_grow (ಕೀಲಿ, extension, "", "");
  obstack_free (&muscle_obstack, extension);
}


ಶೂನ್ಯ
muscle_location_grow (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಕೀಲಿ, location loc)
{
  muscle_boundary_grow (ಕೀಲಿ, loc.ಆರಂಭ);
  muscle_grow (ಕೀಲಿ, "", ", ", "");
  muscle_boundary_grow (ಕೀಲಿ, loc.ಕೊನೆ);
}

#ವ್ಯಾಖ್ಯಾನಿಸು COMMON_DECODE(ಮೌಲ್ಯ_4)                                    \
  ಸ್ಥಿತಿ '$':                                                     \
    ++(ಮೌಲ್ಯ_4); aver (*(ಮೌಲ್ಯ_4) == ']');                          \
    ++(ಮೌಲ್ಯ_4); aver (*(ಮೌಲ್ಯ_4) == '[');                          \
    obstack_sgrow (&muscle_obstack, "$");                       \
    ನಿಲ್ಲಿಸು;                                                      \
  ಸ್ಥಿತಿ '@':                                                     \
    ಆಯ್ಕೆ (*++(ಮೌಲ್ಯ_4))                                         \
      {                                                         \
      ಸ್ಥಿತಿ '\'': /* Ignore "@'" */ ನಿಲ್ಲಿಸು;                       \
      ಸ್ಥಿತಿ '@': obstack_sgrow (&muscle_obstack, "@" ); ನಿಲ್ಲಿಸು;   \
      ಸ್ಥಿತಿ '{': obstack_sgrow (&muscle_obstack, "[" ); ನಿಲ್ಲಿಸು;   \
      ಸ್ಥಿತಿ '}': obstack_sgrow (&muscle_obstack, "]" ); ನಿಲ್ಲಿಸು;   \
      ಪೂರ್ವನಿಯೋಜಿತ: aver (false); ನಿಲ್ಲಿಸು;                             \
      }                                                         \
    ನಿಲ್ಲಿಸು;                                                      \
  ಪೂರ್ವನಿಯೋಜಿತ:                                                      \
    obstack_1grow (&muscle_obstack, *(ಮೌಲ್ಯ_4));                  \
    ನಿಲ್ಲಿಸು;

/* Reverse of obstack_escape.  */
ಸ್ಥಿರ ಅಕ್ಷರ *
string_decode (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಕೀಲಿ)
{
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಮೌಲ್ಯ = muscle_ಕಂಡುಹಿಡಿ_const (ಕೀಲಿ);
  ಆದರೆ (!ಮೌಲ್ಯ)
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
  ಮಾಡು {
    ಆಯ್ಕೆ (*ಮೌಲ್ಯ)
      {
        COMMON_DECODE (ಮೌಲ್ಯ)
        ಸ್ಥಿತಿ '[':
        ಸ್ಥಿತಿ ']':
          aver (false);
          ನಿಲ್ಲಿಸು;
      }
  } ಆಗ (*ಮೌಲ್ಯ++);
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಮೌಲ್ಯ_decoded = obstack_ಮುಕ್ತಾಯ (&muscle_obstack);
  ಅಕ್ಷರ *res = xstrdup (ಮೌಲ್ಯ_decoded);
  obstack_free (&muscle_obstack, ಮೌಲ್ಯ_decoded);
  ಹಿಂತಿರುಗಿ res;
}

/* Reverse of muscle_location_grow.  */
ಸ್ಥಿರ location
location_decode (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಮೌಲ್ಯ)
{
  aver (ಮೌಲ್ಯ);
  aver (*ಮೌಲ್ಯ == '[');
  ++ಮೌಲ್ಯ; aver (*ಮೌಲ್ಯ == '[');
  location loc;
  ಆಗ (*++ಮೌಲ್ಯ)
    ಆಯ್ಕೆ (*ಮೌಲ್ಯ)
      {
        COMMON_DECODE (ಮೌಲ್ಯ)
        ಸ್ಥಿತಿ '[':
          aver (false);
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ ']':
          ++ಮೌಲ್ಯ; aver (*ಮೌಲ್ಯ == ']');
          ಅಕ್ಷರ *boundary_str = obstack_finish0 (&muscle_obstack);
          ಆಯ್ಕೆ (*++ಮೌಲ್ಯ)
            {
            ಸ್ಥಿತಿ ',':
              boundary_ಗಣ_from_string (&loc.ಆರಂಭ, boundary_str);
              obstack_free (&muscle_obstack, boundary_str);
              ++ಮೌಲ್ಯ; aver (*ಮೌಲ್ಯ == ' ');
              ++ಮೌಲ್ಯ; aver (*ಮೌಲ್ಯ == '[');
              ++ಮೌಲ್ಯ; aver (*ಮೌಲ್ಯ == '[');
              ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ '\0':
              boundary_ಗಣ_from_string (&loc.ಕೊನೆ, boundary_str);
              obstack_free (&muscle_obstack, boundary_str);
              ಹಿಂತಿರುಗಿ loc;
              ನಿಲ್ಲಿಸು;
            ಪೂರ್ವನಿಯೋಜಿತ:
              aver (false);
              ನಿಲ್ಲಿಸು;
            }
          ನಿಲ್ಲಿಸು;
      }
  aver (false);
  ಹಿಂತಿರುಗಿ loc;
}

ಶೂನ್ಯ
muscle_user_name_list_grow (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಕೀಲಿ, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *user_name,
                            location loc)
{
  muscle_grow (ಕೀಲಿ, "[[[[", ",", "");
  muscle_grow (ಕೀಲಿ, user_name, "", "");
  muscle_grow (ಕೀಲಿ, "]], ", "", "");
  muscle_location_grow (ಕೀಲಿ, loc);
  muscle_grow (ಕೀಲಿ, "]]", "", "");
}


/** Return an allocated string that represents the %define directive
    that performs the assignment.

    @param assignment "VAR", or "VAR=VAL".
    @param value      default value if VAL \a assignment has no '='.

    For instance:
    "foo", NULL      => "%define foo"
    "foo", "baz"     => "%define foo baz"
    "foo=bar", NULL  => "%define foo bar"
    "foo=bar", "baz" => "%define foo bar"
    "foo=", NULL     => "%define foo"
    "foo=", "baz"    => "%define foo"
 */

ಸ್ಥಿರ
ಅಕ್ಷರ *
define_directive (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *assignment,
                  muscle_ಬಗೆ ಬಗೆ,
                  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಮೌಲ್ಯ)
{
  ಅಕ್ಷರ *eq = strchr (assignment, '=');
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *fmt
    = eq || !ಮೌಲ್ಯ || !*ಮೌಲ್ಯ ? "%%define %s"
    : ಬಗೆ == muscle_ಸಂಕೇತ     ? "%%define %s {%s}"
    : ಬಗೆ == muscle_string   ? "%%define %s \"%s\""
    :                           "%%define %s %s";
  ಅಕ್ಷರ *res = xmalloc (ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (fmt) + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (assignment)
                       + (ಮೌಲ್ಯ ? ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ಮೌಲ್ಯ) : 0));
  sprintf (res, fmt, assignment, ಮೌಲ್ಯ);
  eq = strchr (res, '=');
  ಆದರೆ (eq)
    *eq = eq[1] ? ' ' : '\0';
  ಹಿಂತಿರುಗಿ res;
}

/** If the \a variable name is obsolete, return the name to use,
 * otherwise \a variable.  If the \a value is obsolete, update it too.
 *
 * Allocates the returned value if needed, otherwise the returned
 * value is exactly \a variable.  */
ಸ್ಥಿರ
ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *
muscle_percent_variable_update (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *variable,
                                muscle_ಬಗೆ ಬಗೆ,
                                ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ **ಮೌಲ್ಯ,
                                ಅಕ್ಷರ **old, ಅಕ್ಷರ **upd)
{
  ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ
  {
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *obsolete;
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *updated;
    muscle_ಬಗೆ ಬಗೆ;
  } conversion_type;
  ಸ್ಥಿರಾಂಕ conversion_type conversion[] =
  {
    { "%error-verbose",             "parse.error=verbose",       muscle_keyword },
    { "%error_verbose",             "parse.error=verbose",       muscle_keyword },
    { "abstract",                   "api.parser.abstract",       muscle_keyword },
    { "annotations",                "api.parser.annotations",    muscle_ಸಂಕೇತ },
    { "api.push_pull",              "api.push-pull",             muscle_keyword },
    { "api.tokens.prefix",          "api.token.prefix",          muscle_ಸಂಕೇತ },
    { "extends",                    "api.parser.extends",        muscle_keyword },
    { "filename_type",              "api.filename.type",         muscle_ಸಂಕೇತ },
    { "final",                      "api.parser.final",          muscle_keyword },
    { "implements",                 "api.parser.implements",     muscle_keyword },
    { "lex_symbol",                 "api.token.constructor",     -1 },
    { "location_type",              "api.location.type",         muscle_ಸಂಕೇತ },
    { "lr.default-reductions",      "lr.default-reduction",      muscle_keyword },
    { "lr.keep-unreachable-states", "lr.keep-unreachable-state", muscle_keyword },
    { "lr.keep_unreachable_states", "lr.keep-unreachable-state", muscle_keyword },
    { "namespace",                  "api.namespace",             muscle_ಸಂಕೇತ },
    { "package",                    "api.package",               muscle_ಸಂಕೇತ },
    { "parser_class_name",          "api.parser.class",          muscle_ಸಂಕೇತ },
    { "public",                     "api.parser.public",         muscle_keyword },
    { "strictfp",                   "api.parser.strictfp",       muscle_keyword },
    { "stype",                      "api.value.type",            -1 },
    { "variant=",                   "api.value.type=variant",    -1 },
    { "variant=true",               "api.value.type=variant",    -1 },
    { ಖಾಲಿ, ಖಾಲಿ, -1, }
  };

  ಗಾಗಿ (conversion_type ಸ್ಥಿರಾಂಕ *c = conversion; c->obsolete; ++c)
    {
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *eq = strchr (c->obsolete, '=');
      ಆದರೆ (eq
          ? (!ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ (c->obsolete, variable, eq - c->obsolete)
             && STREQ (eq + 1, *ಮೌಲ್ಯ))
          : STREQ (c->obsolete, variable))
        {
          /* Generate the deprecation warning. */
          *old = c->obsolete[0] == '%'
            ? xstrdup (c->obsolete)
            : define_directive (c->obsolete, ಬಗೆ, *ಮೌಲ್ಯ);
          *upd = define_directive (c->updated, c->ಬಗೆ, *ಮೌಲ್ಯ);
          /* Update the variable and its value.  */
          {
            ಅಕ್ಷರ *res = xstrdup (c->updated);
            ಅಕ್ಷರ *eq2 = strchr (res, '=');
            ಆದರೆ (eq2)
              {
                *eq2 = '\0';
                *ಮೌಲ್ಯ = eq2 + 1;
              }
            ಹಿಂತಿರುಗಿ res;
          }
        }
    }
  ಹಿಂತಿರುಗಿ variable;
}

ಶೂನ್ಯ
muscle_percent_define_ಸೇರಿಸು (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *var, location variable_loc,
                              muscle_ಬಗೆ ಬಗೆ,
                              ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಮೌಲ್ಯ,
                              muscle_percent_define_how how)
{
  /* Backward compatibility.  */
  ಅಕ್ಷರ *old = ಖಾಲಿ;
  ಅಕ್ಷರ *upd = ಖಾಲಿ;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *variable
    = muscle_percent_variable_update (var, ಬಗೆ,
                                      &ಮೌಲ್ಯ, &old, &upd);
  uniqstr name = muscle_name (variable, ಖಾಲಿ);
  uniqstr loc_name = muscle_name (variable, "loc");
  uniqstr syncline_name = muscle_name (variable, "syncline");
  uniqstr how_name = muscle_name (variable, "how");
  uniqstr ಬಗೆ_name = muscle_name (variable, "kind");

  /* Command-line options are processed before the grammar file.  */
  bool warned = false;
  ಆದರೆ (how == MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE)
    {
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *current_ಮೌಲ್ಯ = muscle_ಕಂಡುಹಿಡಿ_const (name);
      ಆದರೆ (current_ಮೌಲ್ಯ)
        {
          ದೀರ್ಘ l = strtol (muscle_ಕಂಡುಹಿಡಿ_const (how_name), ಖಾಲಿ, 10);
          muscle_percent_define_how how_old
            = 0 <= l && l <= ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ ? l : ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ;
          ಆದರೆ (how_old == MUSCLE_PERCENT_DEFINE_F)
            ಹೋಗು ಕೊನೆ;
          /* If assigning the same value, make it a warning.  */
          warnings warn = STREQ (ಮೌಲ್ಯ, current_ಮೌಲ್ಯ) ? Wother : complaint;
          complain (&variable_loc, warn,
                    _("%%define variable %s redefined"),
                    quote (variable));
          location loc = muscle_percent_define_get_loc (variable);
          subcomplain (&loc, warn, _("previous definition"));
          fixits_ರಿಜಿಸ್ಟರ್ (&variable_loc, "");
          warned = true;
        }
    }

  ಆದರೆ (!warned && old && upd)
    deprecated_directive (&variable_loc, old, upd);

  MUSCLE_ಸೇರಿಸು_STRING (name, ಮೌಲ್ಯ);
  muscle_ಸೇರಿಸು (loc_name, "");
  muscle_location_grow (loc_name, variable_loc);
  muscle_ಸೇರಿಸು (syncline_name, "");
  muscle_syncline_grow (syncline_name, variable_loc);
  muscle_user_name_list_grow ("percent_define_user_variables", variable,
                              variable_loc);
  MUSCLE_ಸೇರಿಸು_INT (how_name, how);
  MUSCLE_ಸೇರಿಸು_STRING (ಬಗೆ_name, muscle_ಬಗೆ_string (ಬಗೆ));
 ಕೊನೆ:
  ಬಿಡುಗಡೆ (old);
  ಬಿಡುಗಡೆ (upd);
  ಆದರೆ (variable != var)
    ಬಿಡುಗಡೆ ((ಅಕ್ಷರ *) variable);
}

/* This is used for backward compatibility, e.g., "%define api.pure"
   supersedes "%pure-parser".  */
ಶೂನ್ಯ
muscle_percent_define_ensure (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *variable, location loc,
                              bool ಮೌಲ್ಯ)
{
  uniqstr name = muscle_name (variable, ಖಾಲಿ);
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *val = ಮೌಲ್ಯ ? "" : "false";

  /* Don't complain is VARIABLE is already defined, but be sure to set
     its value to VAL.  */
  ಆದರೆ (!muscle_ಕಂಡುಹಿಡಿ_const (name)
      || muscle_percent_define_flag_if (variable) != ಮೌಲ್ಯ)
    muscle_percent_define_ಸೇರಿಸು (variable, loc, muscle_keyword, val,
                                  MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
}

/* Mark %define VARIABLE as used.  */
ಸ್ಥಿರ ಶೂನ್ಯ
muscle_percent_define_use (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *variable)
{
  muscle_ಸೇರಿಸು (muscle_name (variable, "bison_variables"), "");
}

/* The value of %define variable VARIABLE (corresponding to FIELD, if
   defined).  Do not register as used, but diagnose unset variables.  */

ಸ್ಥಿರ
ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *
muscle_percent_define_get_raw (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *variable, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಕ್ಷೇತ್ರ)
{
  uniqstr name = muscle_name (variable, ಕ್ಷೇತ್ರ);
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *res = muscle_ಕಂಡುಹಿಡಿ_const (name);
  ಆದರೆ (!res)
    complain (ಖಾಲಿ, fatal, _("%s: undefined %%define variable %s"),
              "muscle_percent_define_get_raw", quote (variable));
  ಹಿಂತಿರುಗಿ res;
}

ಅಕ್ಷರ *
muscle_percent_define_get (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *variable)
{
  uniqstr name = muscle_name (variable, ಖಾಲಿ);
  ಅಕ್ಷರ *ಮೌಲ್ಯ = string_decode (name);
  ಆದರೆ (!ಮೌಲ್ಯ)
    ಮೌಲ್ಯ = xstrdup ("");
  muscle_percent_define_use (variable);
  ಹಿಂತಿರುಗಿ ಮೌಲ್ಯ;
}

/* The kind of VARIABLE.  An error if undefined.  */
ಸ್ಥಿರ muscle_ಬಗೆ
muscle_percent_define_get_ಬಗೆ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *variable)
{
  ಹಿಂತಿರುಗಿ muscle_ಬಗೆ_new (muscle_percent_define_get_raw (variable, "kind"));
}

/* Check the kind of VARIABLE.  An error if undefined.  */
ಸ್ಥಿರ ಶೂನ್ಯ
muscle_percent_define_check_ಬಗೆ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *variable, muscle_ಬಗೆ ಬಗೆ)
{
  ಆದರೆ (muscle_percent_define_get_ಬಗೆ (variable) != ಬಗೆ)
    {
      location loc = muscle_percent_define_get_loc (variable);
      ಆಯ್ಕೆ (ಬಗೆ)
        {
        ಸ್ಥಿತಿ muscle_ಸಂಕೇತ:
          complain (&loc, Wdeprecated,
                    _("%%define variable '%s' requires '{...}' values"),
                    variable);
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ muscle_keyword:
          complain (&loc, Wdeprecated,
                    _("%%define variable '%s' requires keyword values"),
                    variable);
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ muscle_string:
          complain (&loc, Wdeprecated,
                    _("%%define variable '%s' requires '\"...\"' values"),
                    variable);
          ನಿಲ್ಲಿಸು;
        }
    }
}


location
muscle_percent_define_get_loc (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *variable)
{
  ಹಿಂತಿರುಗಿ location_decode (muscle_percent_define_get_raw (variable, "loc"));
}

ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *
muscle_percent_define_get_syncline (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *variable)
{
  ಹಿಂತಿರುಗಿ muscle_percent_define_get_raw (variable, "syncline");
}

bool
muscle_percent_define_ifdef (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *variable)
{
  ಆದರೆ (muscle_ಕಂಡುಹಿಡಿ_const (muscle_name (variable, ಖಾಲಿ)))
    {
      muscle_percent_define_use (variable);
      ಹಿಂತಿರುಗಿ true;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ false;
}

bool
muscle_percent_define_flag_if (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *variable)
{
  uniqstr invalid_boolean_name = muscle_name (variable, "invalid_boolean");
  bool res = false;

  ಆದರೆ (muscle_percent_define_ifdef (variable))
    {
      ಅಕ್ಷರ *ಮೌಲ್ಯ = muscle_percent_define_get (variable);
      muscle_percent_define_check_ಬಗೆ (variable, muscle_keyword);
      ಆದರೆ (ಮೌಲ್ಯ[0] == '\0' || STREQ (ಮೌಲ್ಯ, "true"))
        res = true;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (STREQ (ಮೌಲ್ಯ, "false"))
        res = false;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!muscle_ಕಂಡುಹಿಡಿ_const (invalid_boolean_name))
        {
          muscle_ಸೇರಿಸು (invalid_boolean_name, "");
          location loc = muscle_percent_define_get_loc (variable);
          complain (&loc, complaint,
                    _("invalid value for %%define Boolean variable %s"),
                    quote (variable));
        }
      ಬಿಡುಗಡೆ (ಮೌಲ್ಯ);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    complain (ಖಾಲಿ, fatal, _("%s: undefined %%define variable %s"),
              "muscle_percent_define_flag", quote (variable));

  ಹಿಂತಿರುಗಿ res;
}

ಶೂನ್ಯ
muscle_percent_define_default (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *variable, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಮೌಲ್ಯ)
{
  uniqstr name = muscle_name (variable, ಖಾಲಿ);
  ಆದರೆ (!muscle_ಕಂಡುಹಿಡಿ_const (name))
    {
      MUSCLE_ಸೇರಿಸು_STRING (name, ಮೌಲ್ಯ);
      MUSCLE_ಸೇರಿಸು_STRING (muscle_name (variable, "kind"), "keyword");
      {
        uniqstr loc_name = muscle_name (variable, "loc");
        location loc;
        loc.ಆರಂಭ.file = "<default value>";
        loc.ಆರಂಭ.ಸಾಲು = -1;
        loc.ಆರಂಭ.ಸ್ತಂಭ = -1;
        loc.ಆರಂಭ.byte = -1;
        loc.ಕೊನೆ = loc.ಆರಂಭ;
        muscle_ಸೇರಿಸು (loc_name, "");
        muscle_location_grow (loc_name, loc);
      }
      muscle_ಸೇರಿಸು (muscle_name (variable, "syncline"), "");
    }
}

ಶೂನ್ಯ
muscle_percent_define_check_ಮೌಲ್ಯಗಳು (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ * ಸ್ಥಿರಾಂಕ *ಮೌಲ್ಯಗಳು)
{
  ಗಾಗಿ (; *ಮೌಲ್ಯಗಳು; ++ಮೌಲ್ಯಗಳು)
    {
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ * ಸ್ಥಿರಾಂಕ *variablep = ಮೌಲ್ಯಗಳು;
      uniqstr name = muscle_name (*variablep, ಖಾಲಿ);
      ಅಕ್ಷರ *ಮೌಲ್ಯ = string_decode (name);
      muscle_percent_define_check_ಬಗೆ (*variablep, muscle_keyword);
      ಆದರೆ (ಮೌಲ್ಯ)
        {
          ಗಾಗಿ (++ಮೌಲ್ಯಗಳು; *ಮೌಲ್ಯಗಳು; ++ಮೌಲ್ಯಗಳು)
            ಆದರೆ (STREQ (ಮೌಲ್ಯ, *ಮೌಲ್ಯಗಳು))
              ನಿಲ್ಲಿಸು;
          ಆದರೆ (!*ಮೌಲ್ಯಗಳು)
            {
              location loc = muscle_percent_define_get_loc (*variablep);
              complain (&loc, complaint,
                        _("invalid value for %%define variable %s: %s"),
                        quote (*variablep), quote_n (1, ಮೌಲ್ಯ));
              ಗಾಗಿ (ಮೌಲ್ಯಗಳು = variablep + 1; *ಮೌಲ್ಯಗಳು; ++ಮೌಲ್ಯಗಳು)
                subcomplain (&loc, complaint | no_caret | silent,
                             _("accepted value: %s"), quote (*ಮೌಲ್ಯಗಳು));
            }
          ಇಲ್ಲದಿದ್ದರೆ
            ಆಗ (*ಮೌಲ್ಯಗಳು)
              ++ಮೌಲ್ಯಗಳು;
          ಬಿಡುಗಡೆ (ಮೌಲ್ಯ);
        }
      ಇಲ್ಲದಿದ್ದರೆ
        complain (ಖಾಲಿ, fatal, _("%s: undefined %%define variable %s"),
                  "muscle_percent_define_check_values", quote (*variablep));
    }
}

ಶೂನ್ಯ
muscle_percent_ಸಂಕೇತ_grow (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *qualifier, location qualifier_loc,
                          ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸಂಕೇತ_2, location ಸಂಕೇತ_loc)
{
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name = UNIQSTR_CONCAT ("percent_code(", qualifier, ")");
  muscle_ಸಂಕೇತ_grow (name, ಸಂಕೇತ_2, ಸಂಕೇತ_loc);
  muscle_user_name_list_grow ("percent_code_user_qualifiers", qualifier,
                               qualifier_loc);
}


/*------------------------------------------------.
| Output the definition of ENTRY as a m4_define.  |
`------------------------------------------------*/

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
muscle_m4_output (muscle_ನಮೂದು *ನಮೂದು, FILE *ಹೊರಹೋಗು)
{
  fprintf (ಹೊರಹೋಗು,
           "m4_define([b4_%s],\n"
           "[[%s]])\n\n\n", ನಮೂದು->ಕೀಲಿ, ನಮೂದು->ಮೌಲ್ಯ);
  ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ bool
muscle_m4_output_processor (ಶೂನ್ಯ *ನಮೂದು, ಶೂನ್ಯ *ಹೊರಹೋಗು)
{
  ಹಿಂತಿರುಗಿ muscle_m4_output (ನಮೂದು, ಹೊರಹೋಗು);
}


ಶೂನ್ಯ
muscles_m4_output (FILE *ಹೊರಹೋಗು)
{
  hash_do_for_each (muscle_ಕೋಷ್ಟಕ, muscle_m4_output_processor, ಹೊರಹೋಗು);
}
