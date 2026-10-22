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

muscle_kind
muscle_kind_new (ವರ್ಣ ಧ್ರುವ *k)
{
  ಯದಿ (STREQ (k, "code"))
    ಪ್ರತ್ಯಾಗಮ muscle_code;
  ಅನ್ಯಥಾ ಯದಿ (STREQ (k, "keyword"))
    ಪ್ರತ್ಯಾಗಮ muscle_keyword;
  ಅನ್ಯಥಾ ಯದಿ (STREQ (k, "string"))
    ಪ್ರತ್ಯಾಗಮ muscle_string;
  abort ();
}

ವರ್ಣ ಧ್ರುವ *
muscle_kind_string (muscle_kind k)
{
  ವಿಕಲ್ಪ (k)
    {
    ಪಕ್ಷ muscle_code:    ಪ್ರತ್ಯಾಗಮ "code";
    ಪಕ್ಷ muscle_keyword: ಪ್ರತ್ಯಾಗಮ "keyword";
    ಪಕ್ಷ muscle_string:  ಪ್ರತ್ಯಾಗಮ "string";
    }
  abort ();
}


/* A key-value pair, along with storage that can be reclaimed when
   this pair is no longer needed.  */
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ
{
  ವರ್ಣ ಧ್ರುವ *ಕೀಲಿ;
  ವರ್ಣ ಧ್ರುವ *ಮೌಲ್ಯ;
  ವರ್ಣ *storage;
  muscle_kind kind;
} muscle_entry;


/* The name of muscle for the %define variable VAR (corresponding to
   FIELD, if defined).  */
ಸ್ಥಿರ uniqstr
muscle_name (ವರ್ಣ ಧ್ರುವ *var, ವರ್ಣ ಧ್ರುವ *field)
{
  ಯದಿ (field)
    ಪ್ರತ್ಯಾಗಮ UNIQSTR_CONCAT ("percent_define_", field, "(", var, ")");
  ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ UNIQSTR_CONCAT ("percent_define(", var, ")");
}

/* An obstack used to create some entries.  */
ಸಂರಚನಾ obstack muscle_obstack;

/* Initial capacity of muscles hash table.  */
#define HT_INITIAL_CAPACITY 257

ಸ್ಥಿರ ಸಂರಚನಾ hash_table *muscle_table = ರಿಕ್ತ;

ಸ್ಥಿರ bool
hash_compare_muscles (ಶೂನ್ಯ ಧ್ರುವ *x, ಶೂನ್ಯ ಧ್ರುವ *y)
{
  muscle_entry ಧ್ರುವ *m1 = x;
  muscle_entry ಧ್ರುವ *m2 = y;
  ಪ್ರತ್ಯಾಗಮ STREQ (m1->ಕೀಲಿ, m2->ಕೀಲಿ);
}

ಸ್ಥಿರ size_t
hash_muscle (ಧ್ರುವ ಶೂನ್ಯ *x, size_t tablesize)
{
  muscle_entry ಧ್ರುವ *m = x;
  ಪ್ರತ್ಯಾಗಮ hash_string (m->ಕೀಲಿ, tablesize);
}

/* Create a fresh muscle name KEY, and insert in the hash table.  */
ಸ್ಥಿರ ಶೂನ್ಯ *
muscle_entry_new (ವರ್ಣ ಧ್ರುವ *ಕೀಲಿ)
{
  muscle_entry *res = xmalloc (ಪರಿಮಾಣ *res);
  res->ಕೀಲಿ = ಕೀಲಿ;
  res->ಮೌಲ್ಯ = ರಿಕ್ತ;
  res->storage = ರಿಕ್ತ;
  hash_xinsert (muscle_table, res);
  ಪ್ರತ್ಯಾಗಮ res;
}

ಸ್ಥಿರ ಶೂನ್ಯ
muscle_entry_free (ಶೂನ್ಯ *entry)
{
  muscle_entry *mentry = entry;
  ಮುಕ್ತ (mentry->storage);
  ಮುಕ್ತ (mentry);
}

ಶೂನ್ಯ
muscle_init (ಶೂನ್ಯ)
{
  /* Initialize the muscle obstack.  */
  obstack_init (&muscle_obstack);

  muscle_table = hash_xinitialize (HT_INITIAL_CAPACITY, ರಿಕ್ತ, hash_muscle,
                                   hash_compare_muscles, muscle_entry_free);
  /* Avoid warnings if the user defined this variable, but did not
     actually call -Wcex.  */
  ಮುಕ್ತ (muscle_percent_define_get ("cex.timeout"));
}


ಶೂನ್ಯ
muscle_free (ಶೂನ್ಯ)
{
  hash_free (muscle_table);
  obstack_free (&muscle_obstack, ರಿಕ್ತ);
}

/* Look for the muscle named KEY.  Return NULL if does not exist.  */
ಸ್ಥಿರ muscle_entry *
muscle_lookup (ವರ್ಣ ಧ್ರುವ *ಕೀಲಿ)
{
  muscle_entry probe;
  probe.ಕೀಲಿ = ಕೀಲಿ;
  ಪ್ರತ್ಯಾಗಮ hash_lookup (muscle_table, &probe);
}


ಶೂನ್ಯ
muscle_ಸೇರಿಸು (ವರ್ಣ ಧ್ರುವ *ಕೀಲಿ, ವರ್ಣ ಧ್ರುವ *ಮೌಲ್ಯ)
{
  muscle_entry *entry = muscle_lookup (ಕೀಲಿ);
  ಯದಿ (entry)
    ಮುಕ್ತ (entry->storage);
  ಅನ್ಯಥಾ
    /* First insertion in the hash. */
    entry = muscle_entry_new (ಕೀಲಿ);
  entry->ಮೌಲ್ಯ = ಮೌಲ್ಯ;
  entry->storage = ರಿಕ್ತ;
}


/* Append VALUE to the current value of KEY.  If KEY did not already
   exist, create it.  Use MUSCLE_OBSTACK.  De-allocate the previously
   associated value.  Copy VALUE and SEPARATOR.  If VALUE does not end
   with TERMINATOR, append one.  */

ಸ್ಥಿರ ಶೂನ್ಯ
muscle_grow (ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ, ಧ್ರುವ ವರ್ಣ *val,
             ಧ್ರುವ ವರ್ಣ *separator, ಧ್ರುವ ವರ್ಣ *terminator)
{
  muscle_entry *entry = muscle_lookup (ಕೀಲಿ);
  ಯದಿ (entry)
    {
      obstack_sgrow (&muscle_obstack, entry->ಮೌಲ್ಯ);
      obstack_sgrow (&muscle_obstack, separator);
      ಮುಕ್ತ (entry->storage);
    }
  ಅನ್ಯಥಾ
    entry = muscle_entry_new (ಕೀಲಿ);

  obstack_sgrow (&muscle_obstack, val);

  size_t vals = strlen (val);
  size_t terms = strlen (terminator);
  ಯದಿ (terms <= vals
      && STRNEQ (val + vals - terms, terminator))
    obstack_sgrow (&muscle_obstack, terminator);

  {
    ವರ್ಣ ಧ್ರುವ *new_val = obstack_finish0 (&muscle_obstack);
    entry->ಮೌಲ್ಯ = entry->storage = xstrdup (new_val);
    obstack_free (&muscle_obstack, new_val);
  }
}

/*------------------------------------------------------------------.
| Using muscle_grow, append a synchronization line for the location |
| LOC to the current value of KEY.                                  |
`------------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
muscle_syncline_grow (ವರ್ಣ ಧ್ರುವ *ಕೀಲಿ, location loc)
{
  obstack_printf (&muscle_obstack, "]b4_syncline(%d, ", loc.start.ಸಾಲು);
  obstack_quote (&muscle_obstack,
                 quotearg_style (c_quoting_style, map_file_name (loc.start.file)));
  obstack_sgrow (&muscle_obstack, ")dnl\n[");
  ವರ್ಣ ಧ್ರುವ *extension = obstack_finish0 (&muscle_obstack);
  muscle_grow (ಕೀಲಿ, extension, "", "");
  obstack_free (&muscle_obstack, extension);
}

/*------------------------------------------------------------------.
| Append VALUE to the current value of KEY, using muscle_grow.  But |
| in addition, issue a synchronization line for the location LOC    |
| using muscle_syncline_grow.                                       |
`------------------------------------------------------------------*/

ಶೂನ್ಯ
muscle_code_grow (ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ, ಧ್ರುವ ವರ್ಣ *val, location loc)
{
  muscle_syncline_grow (ಕೀಲಿ, loc);
  muscle_grow (ಕೀಲಿ, val, "", "\n");
}


ಶೂನ್ಯ
muscle_pair_list_grow (ಧ್ರುವ ವರ್ಣ *muscle,
                       ಧ್ರುವ ವರ್ಣ *a1, ಧ್ರುವ ವರ್ಣ *a2)
{
  obstack_sgrow (&muscle_obstack, "[");
  obstack_quote (&muscle_obstack, a1);
  obstack_sgrow (&muscle_obstack, ", ");
  obstack_quote (&muscle_obstack, a2);
  obstack_sgrow (&muscle_obstack, "]");
  ವರ್ಣ ಧ್ರುವ *pair = obstack_finish0 (&muscle_obstack);
  muscle_grow (muscle, pair, ",\n", "");
  obstack_free (&muscle_obstack, pair);
}


ವರ್ಣ ಧ್ರುವ *
muscle_find_const (ವರ್ಣ ಧ್ರುವ *ಕೀಲಿ)
{
  muscle_entry *entry = muscle_lookup (ಕೀಲಿ);
  ಪ್ರತ್ಯಾಗಮ entry ? entry->ಮೌಲ್ಯ : ರಿಕ್ತ;
}


ವರ್ಣ *
muscle_find (ವರ್ಣ ಧ್ರುವ *ಕೀಲಿ)
{
  muscle_entry *entry = muscle_lookup (ಕೀಲಿ);
  ಯದಿ (entry)
    {
      aver (entry->ಮೌಲ್ಯ == entry->storage);
      ಪ್ರತ್ಯಾಗಮ entry->storage;
    }
  ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}


/* In the format 'file_name:line.column', append BOUND to MUSCLE.  Use
   digraphs for special characters in the file name.  */

ಸ್ಥಿರ ಶೂನ್ಯ
muscle_boundary_grow (ವರ್ಣ ಧ್ರುವ *ಕೀಲಿ, boundary bound)
{
  obstack_sgrow  (&muscle_obstack, "[[");
  obstack_escape (&muscle_obstack, bound.file);
  obstack_printf (&muscle_obstack, ":%d.%d@@%d]]", bound.ಸಾಲು, bound.column, bound.byte);
  ವರ್ಣ ಧ್ರುವ *extension = obstack_finish0 (&muscle_obstack);
  muscle_grow (ಕೀಲಿ, extension, "", "");
  obstack_free (&muscle_obstack, extension);
}


ಶೂನ್ಯ
muscle_location_grow (ವರ್ಣ ಧ್ರುವ *ಕೀಲಿ, location loc)
{
  muscle_boundary_grow (ಕೀಲಿ, loc.start);
  muscle_grow (ಕೀಲಿ, "", ", ", "");
  muscle_boundary_grow (ಕೀಲಿ, loc.end);
}

#define COMMON_DECODE(ಮೌಲ್ಯ_4)                                    \
  ಪಕ್ಷ '$':                                                     \
    ++(ಮೌಲ್ಯ_4); aver (*(ಮೌಲ್ಯ_4) == ']');                          \
    ++(ಮೌಲ್ಯ_4); aver (*(ಮೌಲ್ಯ_4) == '[');                          \
    obstack_sgrow (&muscle_obstack, "$");                       \
    ಭಙ್ಗ;                                                      \
  ಪಕ್ಷ '@':                                                     \
    ವಿಕಲ್ಪ (*++(ಮೌಲ್ಯ_4))                                         \
      {                                                         \
      ಪಕ್ಷ '\'': /* Ignore "@'" */ ಭಙ್ಗ;                       \
      ಪಕ್ಷ '@': obstack_sgrow (&muscle_obstack, "@" ); ಭಙ್ಗ;   \
      ಪಕ್ಷ '{': obstack_sgrow (&muscle_obstack, "[" ); ಭಙ್ಗ;   \
      ಪಕ್ಷ '}': obstack_sgrow (&muscle_obstack, "]" ); ಭಙ್ಗ;   \
      ಸ್ವಭಾವ: aver (false); ಭಙ್ಗ;                             \
      }                                                         \
    ಭಙ್ಗ;                                                      \
  ಸ್ವಭಾವ:                                                      \
    obstack_1grow (&muscle_obstack, *(ಮೌಲ್ಯ_4));                  \
    ಭಙ್ಗ;

/* Reverse of obstack_escape.  */
ಸ್ಥಿರ ವರ್ಣ *
string_decode (ವರ್ಣ ಧ್ರುವ *ಕೀಲಿ)
{
  ವರ್ಣ ಧ್ರುವ *ಮೌಲ್ಯ = muscle_find_const (ಕೀಲಿ);
  ಯದಿ (!ಮೌಲ್ಯ)
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
  ಕುರು {
    ವಿಕಲ್ಪ (*ಮೌಲ್ಯ)
      {
        COMMON_DECODE (ಮೌಲ್ಯ)
        ಪಕ್ಷ '[':
        ಪಕ್ಷ ']':
          aver (false);
          ಭಙ್ಗ;
      }
  } ಯಾವತ್ (*ಮೌಲ್ಯ++);
  ವರ್ಣ ಧ್ರುವ *ಮೌಲ್ಯ_decoded = obstack_finish (&muscle_obstack);
  ವರ್ಣ *res = xstrdup (ಮೌಲ್ಯ_decoded);
  obstack_free (&muscle_obstack, ಮೌಲ್ಯ_decoded);
  ಪ್ರತ್ಯಾಗಮ res;
}

/* Reverse of muscle_location_grow.  */
ಸ್ಥಿರ location
location_decode (ವರ್ಣ ಧ್ರುವ *ಮೌಲ್ಯ)
{
  aver (ಮೌಲ್ಯ);
  aver (*ಮೌಲ್ಯ == '[');
  ++ಮೌಲ್ಯ; aver (*ಮೌಲ್ಯ == '[');
  location loc;
  ಯಾವತ್ (*++ಮೌಲ್ಯ)
    ವಿಕಲ್ಪ (*ಮೌಲ್ಯ)
      {
        COMMON_DECODE (ಮೌಲ್ಯ)
        ಪಕ್ಷ '[':
          aver (false);
          ಭಙ್ಗ;
        ಪಕ್ಷ ']':
          ++ಮೌಲ್ಯ; aver (*ಮೌಲ್ಯ == ']');
          ವರ್ಣ *boundary_str = obstack_finish0 (&muscle_obstack);
          ವಿಕಲ್ಪ (*++ಮೌಲ್ಯ)
            {
            ಪಕ್ಷ ',':
              boundary_set_from_string (&loc.start, boundary_str);
              obstack_free (&muscle_obstack, boundary_str);
              ++ಮೌಲ್ಯ; aver (*ಮೌಲ್ಯ == ' ');
              ++ಮೌಲ್ಯ; aver (*ಮೌಲ್ಯ == '[');
              ++ಮೌಲ್ಯ; aver (*ಮೌಲ್ಯ == '[');
              ಭಙ್ಗ;
            ಪಕ್ಷ '\0':
              boundary_set_from_string (&loc.end, boundary_str);
              obstack_free (&muscle_obstack, boundary_str);
              ಪ್ರತ್ಯಾಗಮ loc;
              ಭಙ್ಗ;
            ಸ್ವಭಾವ:
              aver (false);
              ಭಙ್ಗ;
            }
          ಭಙ್ಗ;
      }
  aver (false);
  ಪ್ರತ್ಯಾಗಮ loc;
}

ಶೂನ್ಯ
muscle_user_name_list_grow (ವರ್ಣ ಧ್ರುವ *ಕೀಲಿ, ವರ್ಣ ಧ್ರುವ *user_name,
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
ವರ್ಣ *
define_directive (ವರ್ಣ ಧ್ರುವ *assignment,
                  muscle_kind kind,
                  ವರ್ಣ ಧ್ರುವ *ಮೌಲ್ಯ)
{
  ವರ್ಣ *eq = strchr (assignment, '=');
  ವರ್ಣ ಧ್ರುವ *fmt
    = eq || !ಮೌಲ್ಯ || !*ಮೌಲ್ಯ ? "%%define %s"
    : kind == muscle_code     ? "%%define %s {%s}"
    : kind == muscle_string   ? "%%define %s \"%s\""
    :                           "%%define %s %s";
  ವರ್ಣ *res = xmalloc (strlen (fmt) + strlen (assignment)
                       + (ಮೌಲ್ಯ ? strlen (ಮೌಲ್ಯ) : 0));
  sprintf (res, fmt, assignment, ಮೌಲ್ಯ);
  eq = strchr (res, '=');
  ಯದಿ (eq)
    *eq = eq[1] ? ' ' : '\0';
  ಪ್ರತ್ಯಾಗಮ res;
}

/** If the \a variable name is obsolete, return the name to use,
 * otherwise \a variable.  If the \a value is obsolete, update it too.
 *
 * Allocates the returned value if needed, otherwise the returned
 * value is exactly \a variable.  */
ಸ್ಥಿರ
ವರ್ಣ ಧ್ರುವ *
muscle_percent_variable_update (ವರ್ಣ ಧ್ರುವ *variable,
                                muscle_kind kind,
                                ವರ್ಣ ಧ್ರುವ **ಮೌಲ್ಯ,
                                ವರ್ಣ **old, ವರ್ಣ **upd)
{
  ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ
  {
    ಧ್ರುವ ವರ್ಣ *obsolete;
    ಧ್ರುವ ವರ್ಣ *updated;
    muscle_kind kind;
  } conversion_type;
  ಧ್ರುವ conversion_type conversion[] =
  {
    { "%error-verbose",             "parse.error=verbose",       muscle_keyword },
    { "%error_verbose",             "parse.error=verbose",       muscle_keyword },
    { "abstract",                   "api.parser.abstract",       muscle_keyword },
    { "annotations",                "api.parser.annotations",    muscle_code },
    { "api.push_pull",              "api.push-pull",             muscle_keyword },
    { "api.tokens.prefix",          "api.token.prefix",          muscle_code },
    { "extends",                    "api.parser.extends",        muscle_keyword },
    { "filename_type",              "api.filename.type",         muscle_code },
    { "final",                      "api.parser.final",          muscle_keyword },
    { "implements",                 "api.parser.implements",     muscle_keyword },
    { "lex_symbol",                 "api.token.constructor",     -1 },
    { "location_type",              "api.location.type",         muscle_code },
    { "lr.default-reductions",      "lr.default-reduction",      muscle_keyword },
    { "lr.keep-unreachable-states", "lr.keep-unreachable-state", muscle_keyword },
    { "lr.keep_unreachable_states", "lr.keep-unreachable-state", muscle_keyword },
    { "namespace",                  "api.namespace",             muscle_code },
    { "package",                    "api.package",               muscle_code },
    { "parser_class_name",          "api.parser.class",          muscle_code },
    { "public",                     "api.parser.public",         muscle_keyword },
    { "strictfp",                   "api.parser.strictfp",       muscle_keyword },
    { "stype",                      "api.value.type",            -1 },
    { "variant=",                   "api.value.type=variant",    -1 },
    { "variant=true",               "api.value.type=variant",    -1 },
    { ರಿಕ್ತ, ರಿಕ್ತ, -1, }
  };

  ಪ್ರತಿ (conversion_type ಧ್ರುವ *c = conversion; c->obsolete; ++c)
    {
      ವರ್ಣ ಧ್ರುವ *eq = strchr (c->obsolete, '=');
      ಯದಿ (eq
          ? (!strncmp (c->obsolete, variable, eq - c->obsolete)
             && STREQ (eq + 1, *ಮೌಲ್ಯ))
          : STREQ (c->obsolete, variable))
        {
          /* Generate the deprecation warning. */
          *old = c->obsolete[0] == '%'
            ? xstrdup (c->obsolete)
            : define_directive (c->obsolete, kind, *ಮೌಲ್ಯ);
          *upd = define_directive (c->updated, c->kind, *ಮೌಲ್ಯ);
          /* Update the variable and its value.  */
          {
            ವರ್ಣ *res = xstrdup (c->updated);
            ವರ್ಣ *eq2 = strchr (res, '=');
            ಯದಿ (eq2)
              {
                *eq2 = '\0';
                *ಮೌಲ್ಯ = eq2 + 1;
              }
            ಪ್ರತ್ಯಾಗಮ res;
          }
        }
    }
  ಪ್ರತ್ಯಾಗಮ variable;
}

ಶೂನ್ಯ
muscle_percent_define_ಸೇರಿಸು (ವರ್ಣ ಧ್ರುವ *var, location variable_loc,
                              muscle_kind kind,
                              ವರ್ಣ ಧ್ರುವ *ಮೌಲ್ಯ,
                              muscle_percent_define_how how)
{
  /* Backward compatibility.  */
  ವರ್ಣ *old = ರಿಕ್ತ;
  ವರ್ಣ *upd = ರಿಕ್ತ;
  ವರ್ಣ ಧ್ರುವ *variable
    = muscle_percent_variable_update (var, kind,
                                      &ಮೌಲ್ಯ, &old, &upd);
  uniqstr name = muscle_name (variable, ರಿಕ್ತ);
  uniqstr loc_name = muscle_name (variable, "loc");
  uniqstr syncline_name = muscle_name (variable, "syncline");
  uniqstr how_name = muscle_name (variable, "how");
  uniqstr kind_name = muscle_name (variable, "kind");

  /* Command-line options are processed before the grammar file.  */
  bool warned = false;
  ಯದಿ (how == MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE)
    {
      ವರ್ಣ ಧ್ರುವ *current_ಮೌಲ್ಯ = muscle_find_const (name);
      ಯದಿ (current_ಮೌಲ್ಯ)
        {
          long l = strtol (muscle_find_const (how_name), ರಿಕ್ತ, 10);
          muscle_percent_define_how how_old
            = 0 <= l && l <= INT_MAX ? l : INT_MAX;
          ಯದಿ (how_old == MUSCLE_PERCENT_DEFINE_F)
            ಗಚ್ಛ end;
          /* If assigning the same value, make it a warning.  */
          warnings warn = STREQ (ಮೌಲ್ಯ, current_ಮೌಲ್ಯ) ? Wother : complaint;
          complain (&variable_loc, warn,
                    _("%%define variable %s redefined"),
                    quote (variable));
          location loc = muscle_percent_define_get_loc (variable);
          subcomplain (&loc, warn, _("previous definition"));
          fixits_register (&variable_loc, "");
          warned = true;
        }
    }

  ಯದಿ (!warned && old && upd)
    deprecated_directive (&variable_loc, old, upd);

  MUSCLE_ಸೇರಿಸು_STRING (name, ಮೌಲ್ಯ);
  muscle_ಸೇರಿಸು (loc_name, "");
  muscle_location_grow (loc_name, variable_loc);
  muscle_ಸೇರಿಸು (syncline_name, "");
  muscle_syncline_grow (syncline_name, variable_loc);
  muscle_user_name_list_grow ("percent_define_user_variables", variable,
                              variable_loc);
  MUSCLE_ಸೇರಿಸು_INT (how_name, how);
  MUSCLE_ಸೇರಿಸು_STRING (kind_name, muscle_kind_string (kind));
 end:
  ಮುಕ್ತ (old);
  ಮುಕ್ತ (upd);
  ಯದಿ (variable != var)
    ಮುಕ್ತ ((ವರ್ಣ *) variable);
}

/* This is used for backward compatibility, e.g., "%define api.pure"
   supersedes "%pure-parser".  */
ಶೂನ್ಯ
muscle_percent_define_ensure (ವರ್ಣ ಧ್ರುವ *variable, location loc,
                              bool ಮೌಲ್ಯ)
{
  uniqstr name = muscle_name (variable, ರಿಕ್ತ);
  ವರ್ಣ ಧ್ರುವ *val = ಮೌಲ್ಯ ? "" : "false";

  /* Don't complain is VARIABLE is already defined, but be sure to set
     its value to VAL.  */
  ಯದಿ (!muscle_find_const (name)
      || muscle_percent_define_flag_if (variable) != ಮೌಲ್ಯ)
    muscle_percent_define_ಸೇರಿಸು (variable, loc, muscle_keyword, val,
                                  MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
}

/* Mark %define VARIABLE as used.  */
ಸ್ಥಿರ ಶೂನ್ಯ
muscle_percent_define_use (ವರ್ಣ ಧ್ರುವ *variable)
{
  muscle_ಸೇರಿಸು (muscle_name (variable, "bison_variables"), "");
}

/* The value of %define variable VARIABLE (corresponding to FIELD, if
   defined).  Do not register as used, but diagnose unset variables.  */

ಸ್ಥಿರ
ವರ್ಣ ಧ್ರುವ *
muscle_percent_define_get_raw (ವರ್ಣ ಧ್ರುವ *variable, ವರ್ಣ ಧ್ರುವ *field)
{
  uniqstr name = muscle_name (variable, field);
  ವರ್ಣ ಧ್ರುವ *res = muscle_find_const (name);
  ಯದಿ (!res)
    complain (ರಿಕ್ತ, fatal, _("%s: undefined %%define variable %s"),
              "muscle_percent_define_get_raw", quote (variable));
  ಪ್ರತ್ಯಾಗಮ res;
}

ವರ್ಣ *
muscle_percent_define_get (ವರ್ಣ ಧ್ರುವ *variable)
{
  uniqstr name = muscle_name (variable, ರಿಕ್ತ);
  ವರ್ಣ *ಮೌಲ್ಯ = string_decode (name);
  ಯದಿ (!ಮೌಲ್ಯ)
    ಮೌಲ್ಯ = xstrdup ("");
  muscle_percent_define_use (variable);
  ಪ್ರತ್ಯಾಗಮ ಮೌಲ್ಯ;
}

/* The kind of VARIABLE.  An error if undefined.  */
ಸ್ಥಿರ muscle_kind
muscle_percent_define_get_kind (ವರ್ಣ ಧ್ರುವ *variable)
{
  ಪ್ರತ್ಯಾಗಮ muscle_kind_new (muscle_percent_define_get_raw (variable, "kind"));
}

/* Check the kind of VARIABLE.  An error if undefined.  */
ಸ್ಥಿರ ಶೂನ್ಯ
muscle_percent_define_check_kind (ವರ್ಣ ಧ್ರುವ *variable, muscle_kind kind)
{
  ಯದಿ (muscle_percent_define_get_kind (variable) != kind)
    {
      location loc = muscle_percent_define_get_loc (variable);
      ವಿಕಲ್ಪ (kind)
        {
        ಪಕ್ಷ muscle_code:
          complain (&loc, Wdeprecated,
                    _("%%define variable '%s' requires '{...}' values"),
                    variable);
          ಭಙ್ಗ;
        ಪಕ್ಷ muscle_keyword:
          complain (&loc, Wdeprecated,
                    _("%%define variable '%s' requires keyword values"),
                    variable);
          ಭಙ್ಗ;
        ಪಕ್ಷ muscle_string:
          complain (&loc, Wdeprecated,
                    _("%%define variable '%s' requires '\"...\"' values"),
                    variable);
          ಭಙ್ಗ;
        }
    }
}


location
muscle_percent_define_get_loc (ವರ್ಣ ಧ್ರುವ *variable)
{
  ಪ್ರತ್ಯಾಗಮ location_decode (muscle_percent_define_get_raw (variable, "loc"));
}

ವರ್ಣ ಧ್ರುವ *
muscle_percent_define_get_syncline (ವರ್ಣ ಧ್ರುವ *variable)
{
  ಪ್ರತ್ಯಾಗಮ muscle_percent_define_get_raw (variable, "syncline");
}

bool
muscle_percent_define_ifdef (ವರ್ಣ ಧ್ರುವ *variable)
{
  ಯದಿ (muscle_find_const (muscle_name (variable, ರಿಕ್ತ)))
    {
      muscle_percent_define_use (variable);
      ಪ್ರತ್ಯಾಗಮ true;
    }
  ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ false;
}

bool
muscle_percent_define_flag_if (ವರ್ಣ ಧ್ರುವ *variable)
{
  uniqstr invalid_boolean_name = muscle_name (variable, "invalid_boolean");
  bool res = false;

  ಯದಿ (muscle_percent_define_ifdef (variable))
    {
      ವರ್ಣ *ಮೌಲ್ಯ = muscle_percent_define_get (variable);
      muscle_percent_define_check_kind (variable, muscle_keyword);
      ಯದಿ (ಮೌಲ್ಯ[0] == '\0' || STREQ (ಮೌಲ್ಯ, "true"))
        res = true;
      ಅನ್ಯಥಾ ಯದಿ (STREQ (ಮೌಲ್ಯ, "false"))
        res = false;
      ಅನ್ಯಥಾ ಯದಿ (!muscle_find_const (invalid_boolean_name))
        {
          muscle_ಸೇರಿಸು (invalid_boolean_name, "");
          location loc = muscle_percent_define_get_loc (variable);
          complain (&loc, complaint,
                    _("invalid value for %%define Boolean variable %s"),
                    quote (variable));
        }
      ಮುಕ್ತ (ಮೌಲ್ಯ);
    }
  ಅನ್ಯಥಾ
    complain (ರಿಕ್ತ, fatal, _("%s: undefined %%define variable %s"),
              "muscle_percent_define_flag", quote (variable));

  ಪ್ರತ್ಯಾಗಮ res;
}

ಶೂನ್ಯ
muscle_percent_define_default (ವರ್ಣ ಧ್ರುವ *variable, ವರ್ಣ ಧ್ರುವ *ಮೌಲ್ಯ)
{
  uniqstr name = muscle_name (variable, ರಿಕ್ತ);
  ಯದಿ (!muscle_find_const (name))
    {
      MUSCLE_ಸೇರಿಸು_STRING (name, ಮೌಲ್ಯ);
      MUSCLE_ಸೇರಿಸು_STRING (muscle_name (variable, "kind"), "keyword");
      {
        uniqstr loc_name = muscle_name (variable, "loc");
        location loc;
        loc.start.file = "<default value>";
        loc.start.ಸಾಲು = -1;
        loc.start.column = -1;
        loc.start.byte = -1;
        loc.end = loc.start;
        muscle_ಸೇರಿಸು (loc_name, "");
        muscle_location_grow (loc_name, loc);
      }
      muscle_ಸೇರಿಸು (muscle_name (variable, "syncline"), "");
    }
}

ಶೂನ್ಯ
muscle_percent_define_check_ಮೌಲ್ಯಗಳು (ವರ್ಣ ಧ್ರುವ * ಧ್ರುವ *ಮೌಲ್ಯಗಳು)
{
  ಪ್ರತಿ (; *ಮೌಲ್ಯಗಳು; ++ಮೌಲ್ಯಗಳು)
    {
      ವರ್ಣ ಧ್ರುವ * ಧ್ರುವ *variablep = ಮೌಲ್ಯಗಳು;
      uniqstr name = muscle_name (*variablep, ರಿಕ್ತ);
      ವರ್ಣ *ಮೌಲ್ಯ = string_decode (name);
      muscle_percent_define_check_kind (*variablep, muscle_keyword);
      ಯದಿ (ಮೌಲ್ಯ)
        {
          ಪ್ರತಿ (++ಮೌಲ್ಯಗಳು; *ಮೌಲ್ಯಗಳು; ++ಮೌಲ್ಯಗಳು)
            ಯದಿ (STREQ (ಮೌಲ್ಯ, *ಮೌಲ್ಯಗಳು))
              ಭಙ್ಗ;
          ಯದಿ (!*ಮೌಲ್ಯಗಳು)
            {
              location loc = muscle_percent_define_get_loc (*variablep);
              complain (&loc, complaint,
                        _("invalid value for %%define variable %s: %s"),
                        quote (*variablep), quote_n (1, ಮೌಲ್ಯ));
              ಪ್ರತಿ (ಮೌಲ್ಯಗಳು = variablep + 1; *ಮೌಲ್ಯಗಳು; ++ಮೌಲ್ಯಗಳು)
                subcomplain (&loc, complaint | no_caret | silent,
                             _("accepted value: %s"), quote (*ಮೌಲ್ಯಗಳು));
            }
          ಅನ್ಯಥಾ
            ಯಾವತ್ (*ಮೌಲ್ಯಗಳು)
              ++ಮೌಲ್ಯಗಳು;
          ಮುಕ್ತ (ಮೌಲ್ಯ);
        }
      ಅನ್ಯಥಾ
        complain (ರಿಕ್ತ, fatal, _("%s: undefined %%define variable %s"),
                  "muscle_percent_define_check_values", quote (*variablep));
    }
}

ಶೂನ್ಯ
muscle_percent_code_grow (ವರ್ಣ ಧ್ರುವ *qualifier, location qualifier_loc,
                          ವರ್ಣ ಧ್ರುವ *code, location code_loc)
{
  ವರ್ಣ ಧ್ರುವ *name = UNIQSTR_CONCAT ("percent_code(", qualifier, ")");
  muscle_code_grow (name, code, code_loc);
  muscle_user_name_list_grow ("percent_code_user_qualifiers", qualifier,
                               qualifier_loc);
}


/*------------------------------------------------.
| Output the definition of ENTRY as a m4_define.  |
`------------------------------------------------*/

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
muscle_m4_output (muscle_entry *entry, FILE *out)
{
  fprintf (out,
           "m4_define([b4_%s],\n"
           "[[%s]])\n\n\n", entry->ಕೀಲಿ, entry->ಮೌಲ್ಯ);
  ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ bool
muscle_m4_output_processor (ಶೂನ್ಯ *entry, ಶೂನ್ಯ *out)
{
  ಪ್ರತ್ಯಾಗಮ muscle_m4_output (entry, out);
}


ಶೂನ್ಯ
muscles_m4_output (FILE *out)
{
  hash_do_for_each (muscle_table, muscle_m4_output_processor, out);
}
