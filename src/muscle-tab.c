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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ "system.h"

#ଅନ୍ତର୍ଭାଵ <hash.h>
#ଅନ୍ତର୍ଭାଵ <quote.h>

#ଅନ୍ତର୍ଭାଵ "complain.h"
#ଅନ୍ତର୍ଭାଵ "files.h"
#ଅନ୍ତର୍ଭାଵ "fixits.h"
#ଅନ୍ତର୍ଭାଵ "getargs.h"
#ଅନ୍ତର୍ଭାଵ "muscle-tab.h"

muscle_kind
muscle_kind_new (ଅକ୍ଷର ଅଚଳ *k)
{
  ଯଦି (STREQ (k, "code"))
    ଫେରନ୍ତୁ muscle_code;
  ଅନ୍ୟଥା ଯଦି (STREQ (k, "keyword"))
    ଫେରନ୍ତୁ muscle_keyword;
  ଅନ୍ୟଥା ଯଦି (STREQ (k, "string"))
    ଫେରନ୍ତୁ muscle_string;
  abort ();
}

ଅକ୍ଷର ଅଚଳ *
muscle_kind_string (muscle_kind k)
{
  ବାଛ (k)
    {
    ସ୍ଥିତି muscle_code:    ଫେରନ୍ତୁ "code";
    ସ୍ଥିତି muscle_keyword: ଫେରନ୍ତୁ "keyword";
    ସ୍ଥିତି muscle_string:  ଫେରନ୍ତୁ "string";
    }
  abort ();
}


/* A key-value pair, along with storage that can be reclaimed when
   this pair is no longer needed.  */
ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା
{
  ଅକ୍ଷର ଅଚଳ *ଚାବି;
  ଅକ୍ଷର ଅଚଳ *ମୂଲ୍ୟ;
  ଅକ୍ଷର *storage;
  muscle_kind kind;
} muscle_entry;


/* The name of muscle for the %define variable VAR (corresponding to
   FIELD, if defined).  */
ସ୍ଥିର uniqstr
muscle_name (ଅକ୍ଷର ଅଚଳ *var, ଅକ୍ଷର ଅଚଳ *field)
{
  ଯଦି (field)
    ଫେରନ୍ତୁ UNIQSTR_CONCAT ("percent_define_", field, "(", var, ")");
  ଅନ୍ୟଥା
    ଫେରନ୍ତୁ UNIQSTR_CONCAT ("percent_define(", var, ")");
}

/* An obstack used to create some entries.  */
ସଂରଚନା obstack muscle_obstack;

/* Initial capacity of muscles hash table.  */
#define HT_INITIAL_CAPACITY 257

ସ୍ଥିର ସଂରଚନା hash_table *muscle_table = ଖାଲି;

ସ୍ଥିର bool
hash_compare_muscles (ଶୂନ୍ୟ ଅଚଳ *x, ଶୂନ୍ୟ ଅଚଳ *y)
{
  muscle_entry ଅଚଳ *m1 = x;
  muscle_entry ଅଚଳ *m2 = y;
  ଫେରନ୍ତୁ STREQ (m1->ଚାବି, m2->ଚାବି);
}

ସ୍ଥିର size_t
hash_muscle (ଅଚଳ ଶୂନ୍ୟ *x, size_t tablesize)
{
  muscle_entry ଅଚଳ *m = x;
  ଫେରନ୍ତୁ hash_string (m->ଚାବି, tablesize);
}

/* Create a fresh muscle name KEY, and insert in the hash table.  */
ସ୍ଥିର ଶୂନ୍ୟ *
muscle_entry_new (ଅକ୍ଷର ଅଚଳ *ଚାବି)
{
  muscle_entry *res = xmalloc (ଆକାର *res);
  res->ଚାବି = ଚାବି;
  res->ମୂଲ୍ୟ = ଖାଲି;
  res->storage = ଖାଲି;
  hash_xinsert (muscle_table, res);
  ଫେରନ୍ତୁ res;
}

ସ୍ଥିର ଶୂନ୍ୟ
muscle_entry_free (ଶୂନ୍ୟ *entry)
{
  muscle_entry *mentry = entry;
  ମୁକ୍ତ (mentry->storage);
  ମୁକ୍ତ (mentry);
}

ଶୂନ୍ୟ
muscle_init (ଶୂନ୍ୟ)
{
  /* Initialize the muscle obstack.  */
  obstack_init (&muscle_obstack);

  muscle_table = hash_xinitialize (HT_INITIAL_CAPACITY, ଖାଲି, hash_muscle,
                                   hash_compare_muscles, muscle_entry_free);
  /* Avoid warnings if the user defined this variable, but did not
     actually call -Wcex.  */
  ମୁକ୍ତ (muscle_percent_define_get ("cex.timeout"));
}


ଶୂନ୍ୟ
muscle_free (ଶୂନ୍ୟ)
{
  hash_free (muscle_table);
  obstack_free (&muscle_obstack, ଖାଲି);
}

/* Look for the muscle named KEY.  Return NULL if does not exist.  */
ସ୍ଥିର muscle_entry *
muscle_lookup (ଅକ୍ଷର ଅଚଳ *ଚାବି)
{
  muscle_entry probe;
  probe.ଚାବି = ଚାବି;
  ଫେରନ୍ତୁ hash_lookup (muscle_table, &probe);
}


ଶୂନ୍ୟ
muscle_ଭର୍ତ୍ତି (ଅକ୍ଷର ଅଚଳ *ଚାବି, ଅକ୍ଷର ଅଚଳ *ମୂଲ୍ୟ)
{
  muscle_entry *entry = muscle_lookup (ଚାବି);
  ଯଦି (entry)
    ମୁକ୍ତ (entry->storage);
  ଅନ୍ୟଥା
    /* First insertion in the hash. */
    entry = muscle_entry_new (ଚାବି);
  entry->ମୂଲ୍ୟ = ମୂଲ୍ୟ;
  entry->storage = ଖାଲି;
}


/* Append VALUE to the current value of KEY.  If KEY did not already
   exist, create it.  Use MUSCLE_OBSTACK.  De-allocate the previously
   associated value.  Copy VALUE and SEPARATOR.  If VALUE does not end
   with TERMINATOR, append one.  */

ସ୍ଥିର ଶୂନ୍ୟ
muscle_grow (ଅଚଳ ଅକ୍ଷର *ଚାବି, ଅଚଳ ଅକ୍ଷର *val,
             ଅଚଳ ଅକ୍ଷର *separator, ଅଚଳ ଅକ୍ଷର *terminator)
{
  muscle_entry *entry = muscle_lookup (ଚାବି);
  ଯଦି (entry)
    {
      obstack_sgrow (&muscle_obstack, entry->ମୂଲ୍ୟ);
      obstack_sgrow (&muscle_obstack, separator);
      ମୁକ୍ତ (entry->storage);
    }
  ଅନ୍ୟଥା
    entry = muscle_entry_new (ଚାବି);

  obstack_sgrow (&muscle_obstack, val);

  size_t vals = strlen (val);
  size_t terms = strlen (terminator);
  ଯଦି (terms <= vals
      && STRNEQ (val + vals - terms, terminator))
    obstack_sgrow (&muscle_obstack, terminator);

  {
    ଅକ୍ଷର ଅଚଳ *new_val = obstack_finish0 (&muscle_obstack);
    entry->ମୂଲ୍ୟ = entry->storage = xstrdup (new_val);
    obstack_free (&muscle_obstack, new_val);
  }
}

/*------------------------------------------------------------------.
| Using muscle_grow, append a synchronization line for the location |
| LOC to the current value of KEY.                                  |
`------------------------------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
muscle_syncline_grow (ଅକ୍ଷର ଅଚଳ *ଚାବି, location loc)
{
  obstack_printf (&muscle_obstack, "]b4_syncline(%d, ", loc.start.ଧାଡ଼ି);
  obstack_quote (&muscle_obstack,
                 quotearg_style (c_quoting_style, map_file_name (loc.start.file)));
  obstack_sgrow (&muscle_obstack, ")dnl\n[");
  ଅକ୍ଷର ଅଚଳ *extension = obstack_finish0 (&muscle_obstack);
  muscle_grow (ଚାବି, extension, "", "");
  obstack_free (&muscle_obstack, extension);
}

/*------------------------------------------------------------------.
| Append VALUE to the current value of KEY, using muscle_grow.  But |
| in addition, issue a synchronization line for the location LOC    |
| using muscle_syncline_grow.                                       |
`------------------------------------------------------------------*/

ଶୂନ୍ୟ
muscle_code_grow (ଅଚଳ ଅକ୍ଷର *ଚାବି, ଅଚଳ ଅକ୍ଷର *val, location loc)
{
  muscle_syncline_grow (ଚାବି, loc);
  muscle_grow (ଚାବି, val, "", "\n");
}


ଶୂନ୍ୟ
muscle_pair_list_grow (ଅଚଳ ଅକ୍ଷର *muscle,
                       ଅଚଳ ଅକ୍ଷର *a1, ଅଚଳ ଅକ୍ଷର *a2)
{
  obstack_sgrow (&muscle_obstack, "[");
  obstack_quote (&muscle_obstack, a1);
  obstack_sgrow (&muscle_obstack, ", ");
  obstack_quote (&muscle_obstack, a2);
  obstack_sgrow (&muscle_obstack, "]");
  ଅକ୍ଷର ଅଚଳ *pair = obstack_finish0 (&muscle_obstack);
  muscle_grow (muscle, pair, ",\n", "");
  obstack_free (&muscle_obstack, pair);
}


ଅକ୍ଷର ଅଚଳ *
muscle_find_const (ଅକ୍ଷର ଅଚଳ *ଚାବି)
{
  muscle_entry *entry = muscle_lookup (ଚାବି);
  ଫେରନ୍ତୁ entry ? entry->ମୂଲ୍ୟ : ଖାଲି;
}


ଅକ୍ଷର *
muscle_find (ଅକ୍ଷର ଅଚଳ *ଚାବି)
{
  muscle_entry *entry = muscle_lookup (ଚାବି);
  ଯଦି (entry)
    {
      aver (entry->ମୂଲ୍ୟ == entry->storage);
      ଫେରନ୍ତୁ entry->storage;
    }
  ଫେରନ୍ତୁ ଖାଲି;
}


/* In the format 'file_name:line.column', append BOUND to MUSCLE.  Use
   digraphs for special characters in the file name.  */

ସ୍ଥିର ଶୂନ୍ୟ
muscle_boundary_grow (ଅକ୍ଷର ଅଚଳ *ଚାବି, boundary bound)
{
  obstack_sgrow  (&muscle_obstack, "[[");
  obstack_escape (&muscle_obstack, bound.file);
  obstack_printf (&muscle_obstack, ":%d.%d@@%d]]", bound.ଧାଡ଼ି, bound.column, bound.byte);
  ଅକ୍ଷର ଅଚଳ *extension = obstack_finish0 (&muscle_obstack);
  muscle_grow (ଚାବି, extension, "", "");
  obstack_free (&muscle_obstack, extension);
}


ଶୂନ୍ୟ
muscle_location_grow (ଅକ୍ଷର ଅଚଳ *ଚାବି, location loc)
{
  muscle_boundary_grow (ଚାବି, loc.start);
  muscle_grow (ଚାବି, "", ", ", "");
  muscle_boundary_grow (ଚାବି, loc.end);
}

#define COMMON_DECODE(ମୂଲ୍ୟ_4)                                    \
  ସ୍ଥିତି '$':                                                     \
    ++(ମୂଲ୍ୟ_4); aver (*(ମୂଲ୍ୟ_4) == ']');                          \
    ++(ମୂଲ୍ୟ_4); aver (*(ମୂଲ୍ୟ_4) == '[');                          \
    obstack_sgrow (&muscle_obstack, "$");                       \
    ଭାଙ୍ଗ;                                                      \
  ସ୍ଥିତି '@':                                                     \
    ବାଛ (*++(ମୂଲ୍ୟ_4))                                         \
      {                                                         \
      ସ୍ଥିତି '\'': /* Ignore "@'" */ ଭାଙ୍ଗ;                       \
      ସ୍ଥିତି '@': obstack_sgrow (&muscle_obstack, "@" ); ଭାଙ୍ଗ;   \
      ସ୍ଥିତି '{': obstack_sgrow (&muscle_obstack, "[" ); ଭାଙ୍ଗ;   \
      ସ୍ଥିତି '}': obstack_sgrow (&muscle_obstack, "]" ); ଭାଙ୍ଗ;   \
      ପୂର୍ବନିର୍ଦ୍ଧାରିତ: aver (false); ଭାଙ୍ଗ;                             \
      }                                                         \
    ଭାଙ୍ଗ;                                                      \
  ପୂର୍ବନିର୍ଦ୍ଧାରିତ:                                                      \
    obstack_1grow (&muscle_obstack, *(ମୂଲ୍ୟ_4));                  \
    ଭାଙ୍ଗ;

/* Reverse of obstack_escape.  */
ସ୍ଥିର ଅକ୍ଷର *
string_decode (ଅକ୍ଷର ଅଚଳ *ଚାବି)
{
  ଅକ୍ଷର ଅଚଳ *ମୂଲ୍ୟ = muscle_find_const (ଚାବି);
  ଯଦି (!ମୂଲ୍ୟ)
    ଫେରନ୍ତୁ ଖାଲି;
  କର {
    ବାଛ (*ମୂଲ୍ୟ)
      {
        COMMON_DECODE (ମୂଲ୍ୟ)
        ସ୍ଥିତି '[':
        ସ୍ଥିତି ']':
          aver (false);
          ଭାଙ୍ଗ;
      }
  } ଯେତେବେଳେ (*ମୂଲ୍ୟ++);
  ଅକ୍ଷର ଅଚଳ *ମୂଲ୍ୟ_decoded = obstack_finish (&muscle_obstack);
  ଅକ୍ଷର *res = xstrdup (ମୂଲ୍ୟ_decoded);
  obstack_free (&muscle_obstack, ମୂଲ୍ୟ_decoded);
  ଫେରନ୍ତୁ res;
}

/* Reverse of muscle_location_grow.  */
ସ୍ଥିର location
location_decode (ଅକ୍ଷର ଅଚଳ *ମୂଲ୍ୟ)
{
  aver (ମୂଲ୍ୟ);
  aver (*ମୂଲ୍ୟ == '[');
  ++ମୂଲ୍ୟ; aver (*ମୂଲ୍ୟ == '[');
  location loc;
  ଯେତେବେଳେ (*++ମୂଲ୍ୟ)
    ବାଛ (*ମୂଲ୍ୟ)
      {
        COMMON_DECODE (ମୂଲ୍ୟ)
        ସ୍ଥିତି '[':
          aver (false);
          ଭାଙ୍ଗ;
        ସ୍ଥିତି ']':
          ++ମୂଲ୍ୟ; aver (*ମୂଲ୍ୟ == ']');
          ଅକ୍ଷର *boundary_str = obstack_finish0 (&muscle_obstack);
          ବାଛ (*++ମୂଲ୍ୟ)
            {
            ସ୍ଥିତି ',':
              boundary_set_from_string (&loc.start, boundary_str);
              obstack_free (&muscle_obstack, boundary_str);
              ++ମୂଲ୍ୟ; aver (*ମୂଲ୍ୟ == ' ');
              ++ମୂଲ୍ୟ; aver (*ମୂଲ୍ୟ == '[');
              ++ମୂଲ୍ୟ; aver (*ମୂଲ୍ୟ == '[');
              ଭାଙ୍ଗ;
            ସ୍ଥିତି '\0':
              boundary_set_from_string (&loc.end, boundary_str);
              obstack_free (&muscle_obstack, boundary_str);
              ଫେରନ୍ତୁ loc;
              ଭାଙ୍ଗ;
            ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
              aver (false);
              ଭାଙ୍ଗ;
            }
          ଭାଙ୍ଗ;
      }
  aver (false);
  ଫେରନ୍ତୁ loc;
}

ଶୂନ୍ୟ
muscle_user_name_list_grow (ଅକ୍ଷର ଅଚଳ *ଚାବି, ଅକ୍ଷର ଅଚଳ *user_name,
                            location loc)
{
  muscle_grow (ଚାବି, "[[[[", ",", "");
  muscle_grow (ଚାବି, user_name, "", "");
  muscle_grow (ଚାବି, "]], ", "", "");
  muscle_location_grow (ଚାବି, loc);
  muscle_grow (ଚାବି, "]]", "", "");
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

ସ୍ଥିର
ଅକ୍ଷର *
define_directive (ଅକ୍ଷର ଅଚଳ *assignment,
                  muscle_kind kind,
                  ଅକ୍ଷର ଅଚଳ *ମୂଲ୍ୟ)
{
  ଅକ୍ଷର *eq = strchr (assignment, '=');
  ଅକ୍ଷର ଅଚଳ *fmt
    = eq || !ମୂଲ୍ୟ || !*ମୂଲ୍ୟ ? "%%define %s"
    : kind == muscle_code     ? "%%define %s {%s}"
    : kind == muscle_string   ? "%%define %s \"%s\""
    :                           "%%define %s %s";
  ଅକ୍ଷର *res = xmalloc (strlen (fmt) + strlen (assignment)
                       + (ମୂଲ୍ୟ ? strlen (ମୂଲ୍ୟ) : 0));
  sprintf (res, fmt, assignment, ମୂଲ୍ୟ);
  eq = strchr (res, '=');
  ଯଦି (eq)
    *eq = eq[1] ? ' ' : '\0';
  ଫେରନ୍ତୁ res;
}

/** If the \a variable name is obsolete, return the name to use,
 * otherwise \a variable.  If the \a value is obsolete, update it too.
 *
 * Allocates the returned value if needed, otherwise the returned
 * value is exactly \a variable.  */
ସ୍ଥିର
ଅକ୍ଷର ଅଚଳ *
muscle_percent_variable_update (ଅକ୍ଷର ଅଚଳ *variable,
                                muscle_kind kind,
                                ଅକ୍ଷର ଅଚଳ **ମୂଲ୍ୟ,
                                ଅକ୍ଷର **old, ଅକ୍ଷର **upd)
{
  ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା
  {
    ଅଚଳ ଅକ୍ଷର *obsolete;
    ଅଚଳ ଅକ୍ଷର *updated;
    muscle_kind kind;
  } conversion_type;
  ଅଚଳ conversion_type conversion[] =
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
    { ଖାଲି, ଖାଲି, -1, }
  };

  ପାଇଁ (conversion_type ଅଚଳ *c = conversion; c->obsolete; ++c)
    {
      ଅକ୍ଷର ଅଚଳ *eq = strchr (c->obsolete, '=');
      ଯଦି (eq
          ? (!strncmp (c->obsolete, variable, eq - c->obsolete)
             && STREQ (eq + 1, *ମୂଲ୍ୟ))
          : STREQ (c->obsolete, variable))
        {
          /* Generate the deprecation warning. */
          *old = c->obsolete[0] == '%'
            ? xstrdup (c->obsolete)
            : define_directive (c->obsolete, kind, *ମୂଲ୍ୟ);
          *upd = define_directive (c->updated, c->kind, *ମୂଲ୍ୟ);
          /* Update the variable and its value.  */
          {
            ଅକ୍ଷର *res = xstrdup (c->updated);
            ଅକ୍ଷର *eq2 = strchr (res, '=');
            ଯଦି (eq2)
              {
                *eq2 = '\0';
                *ମୂଲ୍ୟ = eq2 + 1;
              }
            ଫେରନ୍ତୁ res;
          }
        }
    }
  ଫେରନ୍ତୁ variable;
}

ଶୂନ୍ୟ
muscle_percent_define_ଭର୍ତ୍ତି (ଅକ୍ଷର ଅଚଳ *var, location variable_loc,
                              muscle_kind kind,
                              ଅକ୍ଷର ଅଚଳ *ମୂଲ୍ୟ,
                              muscle_percent_define_how how)
{
  /* Backward compatibility.  */
  ଅକ୍ଷର *old = ଖାଲି;
  ଅକ୍ଷର *upd = ଖାଲି;
  ଅକ୍ଷର ଅଚଳ *variable
    = muscle_percent_variable_update (var, kind,
                                      &ମୂଲ୍ୟ, &old, &upd);
  uniqstr name = muscle_name (variable, ଖାଲି);
  uniqstr loc_name = muscle_name (variable, "loc");
  uniqstr syncline_name = muscle_name (variable, "syncline");
  uniqstr how_name = muscle_name (variable, "how");
  uniqstr kind_name = muscle_name (variable, "kind");

  /* Command-line options are processed before the grammar file.  */
  bool warned = false;
  ଯଦି (how == MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE)
    {
      ଅକ୍ଷର ଅଚଳ *current_ମୂଲ୍ୟ = muscle_find_const (name);
      ଯଦି (current_ମୂଲ୍ୟ)
        {
          long l = strtol (muscle_find_const (how_name), ଖାଲି, 10);
          muscle_percent_define_how how_old
            = 0 <= l && l <= INT_MAX ? l : INT_MAX;
          ଯଦି (how_old == MUSCLE_PERCENT_DEFINE_F)
            ଯାଅ end;
          /* If assigning the same value, make it a warning.  */
          warnings warn = STREQ (ମୂଲ୍ୟ, current_ମୂଲ୍ୟ) ? Wother : complaint;
          complain (&variable_loc, warn,
                    _("%%define variable %s redefined"),
                    quote (variable));
          location loc = muscle_percent_define_get_loc (variable);
          subcomplain (&loc, warn, _("previous definition"));
          fixits_register (&variable_loc, "");
          warned = true;
        }
    }

  ଯଦି (!warned && old && upd)
    deprecated_directive (&variable_loc, old, upd);

  MUSCLE_ଭର୍ତ୍ତି_STRING (name, ମୂଲ୍ୟ);
  muscle_ଭର୍ତ୍ତି (loc_name, "");
  muscle_location_grow (loc_name, variable_loc);
  muscle_ଭର୍ତ୍ତି (syncline_name, "");
  muscle_syncline_grow (syncline_name, variable_loc);
  muscle_user_name_list_grow ("percent_define_user_variables", variable,
                              variable_loc);
  MUSCLE_ଭର୍ତ୍ତି_INT (how_name, how);
  MUSCLE_ଭର୍ତ୍ତି_STRING (kind_name, muscle_kind_string (kind));
 end:
  ମୁକ୍ତ (old);
  ମୁକ୍ତ (upd);
  ଯଦି (variable != var)
    ମୁକ୍ତ ((ଅକ୍ଷର *) variable);
}

/* This is used for backward compatibility, e.g., "%define api.pure"
   supersedes "%pure-parser".  */
ଶୂନ୍ୟ
muscle_percent_define_ensure (ଅକ୍ଷର ଅଚଳ *variable, location loc,
                              bool ମୂଲ୍ୟ)
{
  uniqstr name = muscle_name (variable, ଖାଲି);
  ଅକ୍ଷର ଅଚଳ *val = ମୂଲ୍ୟ ? "" : "false";

  /* Don't complain is VARIABLE is already defined, but be sure to set
     its value to VAL.  */
  ଯଦି (!muscle_find_const (name)
      || muscle_percent_define_flag_if (variable) != ମୂଲ୍ୟ)
    muscle_percent_define_ଭର୍ତ୍ତି (variable, loc, muscle_keyword, val,
                                  MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
}

/* Mark %define VARIABLE as used.  */
ସ୍ଥିର ଶୂନ୍ୟ
muscle_percent_define_use (ଅକ୍ଷର ଅଚଳ *variable)
{
  muscle_ଭର୍ତ୍ତି (muscle_name (variable, "bison_variables"), "");
}

/* The value of %define variable VARIABLE (corresponding to FIELD, if
   defined).  Do not register as used, but diagnose unset variables.  */

ସ୍ଥିର
ଅକ୍ଷର ଅଚଳ *
muscle_percent_define_get_raw (ଅକ୍ଷର ଅଚଳ *variable, ଅକ୍ଷର ଅଚଳ *field)
{
  uniqstr name = muscle_name (variable, field);
  ଅକ୍ଷର ଅଚଳ *res = muscle_find_const (name);
  ଯଦି (!res)
    complain (ଖାଲି, fatal, _("%s: undefined %%define variable %s"),
              "muscle_percent_define_get_raw", quote (variable));
  ଫେରନ୍ତୁ res;
}

ଅକ୍ଷର *
muscle_percent_define_get (ଅକ୍ଷର ଅଚଳ *variable)
{
  uniqstr name = muscle_name (variable, ଖାଲି);
  ଅକ୍ଷର *ମୂଲ୍ୟ = string_decode (name);
  ଯଦି (!ମୂଲ୍ୟ)
    ମୂଲ୍ୟ = xstrdup ("");
  muscle_percent_define_use (variable);
  ଫେରନ୍ତୁ ମୂଲ୍ୟ;
}

/* The kind of VARIABLE.  An error if undefined.  */
ସ୍ଥିର muscle_kind
muscle_percent_define_get_kind (ଅକ୍ଷର ଅଚଳ *variable)
{
  ଫେରନ୍ତୁ muscle_kind_new (muscle_percent_define_get_raw (variable, "kind"));
}

/* Check the kind of VARIABLE.  An error if undefined.  */
ସ୍ଥିର ଶୂନ୍ୟ
muscle_percent_define_check_kind (ଅକ୍ଷର ଅଚଳ *variable, muscle_kind kind)
{
  ଯଦି (muscle_percent_define_get_kind (variable) != kind)
    {
      location loc = muscle_percent_define_get_loc (variable);
      ବାଛ (kind)
        {
        ସ୍ଥିତି muscle_code:
          complain (&loc, Wdeprecated,
                    _("%%define variable '%s' requires '{...}' values"),
                    variable);
          ଭାଙ୍ଗ;
        ସ୍ଥିତି muscle_keyword:
          complain (&loc, Wdeprecated,
                    _("%%define variable '%s' requires keyword values"),
                    variable);
          ଭାଙ୍ଗ;
        ସ୍ଥିତି muscle_string:
          complain (&loc, Wdeprecated,
                    _("%%define variable '%s' requires '\"...\"' values"),
                    variable);
          ଭାଙ୍ଗ;
        }
    }
}


location
muscle_percent_define_get_loc (ଅକ୍ଷର ଅଚଳ *variable)
{
  ଫେରନ୍ତୁ location_decode (muscle_percent_define_get_raw (variable, "loc"));
}

ଅକ୍ଷର ଅଚଳ *
muscle_percent_define_get_syncline (ଅକ୍ଷର ଅଚଳ *variable)
{
  ଫେରନ୍ତୁ muscle_percent_define_get_raw (variable, "syncline");
}

bool
muscle_percent_define_ifdef (ଅକ୍ଷର ଅଚଳ *variable)
{
  ଯଦି (muscle_find_const (muscle_name (variable, ଖାଲି)))
    {
      muscle_percent_define_use (variable);
      ଫେରନ୍ତୁ true;
    }
  ଅନ୍ୟଥା
    ଫେରନ୍ତୁ false;
}

bool
muscle_percent_define_flag_if (ଅକ୍ଷର ଅଚଳ *variable)
{
  uniqstr invalid_boolean_name = muscle_name (variable, "invalid_boolean");
  bool res = false;

  ଯଦି (muscle_percent_define_ifdef (variable))
    {
      ଅକ୍ଷର *ମୂଲ୍ୟ = muscle_percent_define_get (variable);
      muscle_percent_define_check_kind (variable, muscle_keyword);
      ଯଦି (ମୂଲ୍ୟ[0] == '\0' || STREQ (ମୂଲ୍ୟ, "true"))
        res = true;
      ଅନ୍ୟଥା ଯଦି (STREQ (ମୂଲ୍ୟ, "false"))
        res = false;
      ଅନ୍ୟଥା ଯଦି (!muscle_find_const (invalid_boolean_name))
        {
          muscle_ଭର୍ତ୍ତି (invalid_boolean_name, "");
          location loc = muscle_percent_define_get_loc (variable);
          complain (&loc, complaint,
                    _("invalid value for %%define Boolean variable %s"),
                    quote (variable));
        }
      ମୁକ୍ତ (ମୂଲ୍ୟ);
    }
  ଅନ୍ୟଥା
    complain (ଖାଲି, fatal, _("%s: undefined %%define variable %s"),
              "muscle_percent_define_flag", quote (variable));

  ଫେରନ୍ତୁ res;
}

ଶୂନ୍ୟ
muscle_percent_define_default (ଅକ୍ଷର ଅଚଳ *variable, ଅକ୍ଷର ଅଚଳ *ମୂଲ୍ୟ)
{
  uniqstr name = muscle_name (variable, ଖାଲି);
  ଯଦି (!muscle_find_const (name))
    {
      MUSCLE_ଭର୍ତ୍ତି_STRING (name, ମୂଲ୍ୟ);
      MUSCLE_ଭର୍ତ୍ତି_STRING (muscle_name (variable, "kind"), "keyword");
      {
        uniqstr loc_name = muscle_name (variable, "loc");
        location loc;
        loc.start.file = "<default value>";
        loc.start.ଧାଡ଼ି = -1;
        loc.start.column = -1;
        loc.start.byte = -1;
        loc.end = loc.start;
        muscle_ଭର୍ତ୍ତି (loc_name, "");
        muscle_location_grow (loc_name, loc);
      }
      muscle_ଭର୍ତ୍ତି (muscle_name (variable, "syncline"), "");
    }
}

ଶୂନ୍ୟ
muscle_percent_define_check_ମୂଲ୍ୟଗୁଡ଼ିକ (ଅକ୍ଷର ଅଚଳ * ଅଚଳ *ମୂଲ୍ୟଗୁଡ଼ିକ)
{
  ପାଇଁ (; *ମୂଲ୍ୟଗୁଡ଼ିକ; ++ମୂଲ୍ୟଗୁଡ଼ିକ)
    {
      ଅକ୍ଷର ଅଚଳ * ଅଚଳ *variablep = ମୂଲ୍ୟଗୁଡ଼ିକ;
      uniqstr name = muscle_name (*variablep, ଖାଲି);
      ଅକ୍ଷର *ମୂଲ୍ୟ = string_decode (name);
      muscle_percent_define_check_kind (*variablep, muscle_keyword);
      ଯଦି (ମୂଲ୍ୟ)
        {
          ପାଇଁ (++ମୂଲ୍ୟଗୁଡ଼ିକ; *ମୂଲ୍ୟଗୁଡ଼ିକ; ++ମୂଲ୍ୟଗୁଡ଼ିକ)
            ଯଦି (STREQ (ମୂଲ୍ୟ, *ମୂଲ୍ୟଗୁଡ଼ିକ))
              ଭାଙ୍ଗ;
          ଯଦି (!*ମୂଲ୍ୟଗୁଡ଼ିକ)
            {
              location loc = muscle_percent_define_get_loc (*variablep);
              complain (&loc, complaint,
                        _("invalid value for %%define variable %s: %s"),
                        quote (*variablep), quote_n (1, ମୂଲ୍ୟ));
              ପାଇଁ (ମୂଲ୍ୟଗୁଡ଼ିକ = variablep + 1; *ମୂଲ୍ୟଗୁଡ଼ିକ; ++ମୂଲ୍ୟଗୁଡ଼ିକ)
                subcomplain (&loc, complaint | no_caret | silent,
                             _("accepted value: %s"), quote (*ମୂଲ୍ୟଗୁଡ଼ିକ));
            }
          ଅନ୍ୟଥା
            ଯେତେବେଳେ (*ମୂଲ୍ୟଗୁଡ଼ିକ)
              ++ମୂଲ୍ୟଗୁଡ଼ିକ;
          ମୁକ୍ତ (ମୂଲ୍ୟ);
        }
      ଅନ୍ୟଥା
        complain (ଖାଲି, fatal, _("%s: undefined %%define variable %s"),
                  "muscle_percent_define_check_values", quote (*variablep));
    }
}

ଶୂନ୍ୟ
muscle_percent_code_grow (ଅକ୍ଷର ଅଚଳ *qualifier, location qualifier_loc,
                          ଅକ୍ଷର ଅଚଳ *code, location code_loc)
{
  ଅକ୍ଷର ଅଚଳ *name = UNIQSTR_CONCAT ("percent_code(", qualifier, ")");
  muscle_code_grow (name, code, code_loc);
  muscle_user_name_list_grow ("percent_code_user_qualifiers", qualifier,
                               qualifier_loc);
}


/*------------------------------------------------.
| Output the definition of ENTRY as a m4_define.  |
`------------------------------------------------*/

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
muscle_m4_output (muscle_entry *entry, FILE *out)
{
  fprintf (out,
           "m4_define([b4_%s],\n"
           "[[%s]])\n\n\n", entry->ଚାବି, entry->ମୂଲ୍ୟ);
  ଫେରନ୍ତୁ true;
}

ସ୍ଥିର bool
muscle_m4_output_processor (ଶୂନ୍ୟ *entry, ଶୂନ୍ୟ *out)
{
  ଫେରନ୍ତୁ muscle_m4_output (entry, out);
}


ଶୂନ୍ୟ
muscles_m4_output (FILE *out)
{
  hash_do_for_each (muscle_table, muscle_m4_output_processor, out);
}
