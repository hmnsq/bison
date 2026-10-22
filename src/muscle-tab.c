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

#অন্তর্ভাৱ <config.h>
#অন্তর্ভাৱ "system.h"

#অন্তর্ভাৱ <hash.h>
#অন্তর্ভাৱ <quote.h>

#অন্তর্ভাৱ "complain.h"
#অন্তর্ভাৱ "files.h"
#অন্তর্ভাৱ "fixits.h"
#অন্তর্ভাৱ "getargs.h"
#অন্তর্ভাৱ "muscle-tab.h"

muscle_kind
muscle_kind_new (ৱর্ণ ধ্রুৱ *k)
{
  যদি (STREQ (k, "code"))
    ঘূৰাই muscle_code;
  অন্যথা যদি (STREQ (k, "keyword"))
    ঘূৰাই muscle_keyword;
  অন্যথা যদি (STREQ (k, "string"))
    ঘূৰাই muscle_string;
  abort ();
}

ৱর্ণ ধ্রুৱ *
muscle_kind_string (muscle_kind k)
{
  ৱিকল্প (k)
    {
    পক্ষ muscle_code:    ঘূৰাই "code";
    পক্ষ muscle_keyword: ঘূৰাই "keyword";
    পক্ষ muscle_string:  ঘূৰাই "string";
    }
  abort ();
}


/* A key-value pair, along with storage that can be reclaimed when
   this pair is no longer needed.  */
প্রকারনির্দেশ সংরচনা
{
  ৱর্ণ ধ্রুৱ *চাবি;
  ৱর্ণ ধ্রুৱ *মান;
  ৱর্ণ *storage;
  muscle_kind kind;
} muscle_entry;


/* The name of muscle for the %define variable VAR (corresponding to
   FIELD, if defined).  */
স্থির uniqstr
muscle_name (ৱর্ণ ধ্রুৱ *var, ৱর্ণ ধ্রুৱ *field)
{
  যদি (field)
    ঘূৰাই UNIQSTR_CONCAT ("percent_define_", field, "(", var, ")");
  অন্যথা
    ঘূৰাই UNIQSTR_CONCAT ("percent_define(", var, ")");
}

/* An obstack used to create some entries.  */
সংরচনা obstack muscle_obstack;

/* Initial capacity of muscles hash table.  */
#define HT_INITIAL_CAPACITY 257

স্থির সংরচনা hash_table *muscle_table = রিক্ত;

স্থির bool
hash_compare_muscles (শূন্য ধ্রুৱ *x, শূন্য ধ্রুৱ *y)
{
  muscle_entry ধ্রুৱ *m1 = x;
  muscle_entry ধ্রুৱ *m2 = y;
  ঘূৰাই STREQ (m1->চাবি, m2->চাবি);
}

স্থির size_t
hash_muscle (ধ্রুৱ শূন্য *x, size_t tablesize)
{
  muscle_entry ধ্রুৱ *m = x;
  ঘূৰাই hash_string (m->চাবি, tablesize);
}

/* Create a fresh muscle name KEY, and insert in the hash table.  */
স্থির শূন্য *
muscle_entry_new (ৱর্ণ ধ্রুৱ *চাবি)
{
  muscle_entry *res = xmalloc (পরিমাণ *res);
  res->চাবি = চাবি;
  res->মান = রিক্ত;
  res->storage = রিক্ত;
  hash_xinsert (muscle_table, res);
  ঘূৰাই res;
}

স্থির শূন্য
muscle_entry_free (শূন্য *entry)
{
  muscle_entry *mentry = entry;
  মুক্ত (mentry->storage);
  মুক্ত (mentry);
}

শূন্য
muscle_init (শূন্য)
{
  /* Initialize the muscle obstack.  */
  obstack_init (&muscle_obstack);

  muscle_table = hash_xinitialize (HT_INITIAL_CAPACITY, রিক্ত, hash_muscle,
                                   hash_compare_muscles, muscle_entry_free);
  /* Avoid warnings if the user defined this variable, but did not
     actually call -Wcex.  */
  মুক্ত (muscle_percent_define_get ("cex.timeout"));
}


শূন্য
muscle_free (শূন্য)
{
  hash_free (muscle_table);
  obstack_free (&muscle_obstack, রিক্ত);
}

/* Look for the muscle named KEY.  Return NULL if does not exist.  */
স্থির muscle_entry *
muscle_lookup (ৱর্ণ ধ্রুৱ *চাবি)
{
  muscle_entry probe;
  probe.চাবি = চাবি;
  ঘূৰাই hash_lookup (muscle_table, &probe);
}


শূন্য
muscle_ঢোকাও (ৱর্ণ ধ্রুৱ *চাবি, ৱর্ণ ধ্রুৱ *মান)
{
  muscle_entry *entry = muscle_lookup (চাবি);
  যদি (entry)
    মুক্ত (entry->storage);
  অন্যথা
    /* First insertion in the hash. */
    entry = muscle_entry_new (চাবি);
  entry->মান = মান;
  entry->storage = রিক্ত;
}


/* Append VALUE to the current value of KEY.  If KEY did not already
   exist, create it.  Use MUSCLE_OBSTACK.  De-allocate the previously
   associated value.  Copy VALUE and SEPARATOR.  If VALUE does not end
   with TERMINATOR, append one.  */

স্থির শূন্য
muscle_grow (ধ্রুৱ ৱর্ণ *চাবি, ধ্রুৱ ৱর্ণ *val,
             ধ্রুৱ ৱর্ণ *separator, ধ্রুৱ ৱর্ণ *terminator)
{
  muscle_entry *entry = muscle_lookup (চাবি);
  যদি (entry)
    {
      obstack_sgrow (&muscle_obstack, entry->মান);
      obstack_sgrow (&muscle_obstack, separator);
      মুক্ত (entry->storage);
    }
  অন্যথা
    entry = muscle_entry_new (চাবি);

  obstack_sgrow (&muscle_obstack, val);

  size_t vals = strlen (val);
  size_t terms = strlen (terminator);
  যদি (terms <= vals
      && STRNEQ (val + vals - terms, terminator))
    obstack_sgrow (&muscle_obstack, terminator);

  {
    ৱর্ণ ধ্রুৱ *new_val = obstack_finish0 (&muscle_obstack);
    entry->মান = entry->storage = xstrdup (new_val);
    obstack_free (&muscle_obstack, new_val);
  }
}

/*------------------------------------------------------------------.
| Using muscle_grow, append a synchronization line for the location |
| LOC to the current value of KEY.                                  |
`------------------------------------------------------------------*/

স্থির শূন্য
muscle_syncline_grow (ৱর্ণ ধ্রুৱ *চাবি, location loc)
{
  obstack_printf (&muscle_obstack, "]b4_syncline(%d, ", loc.start.সাৰি);
  obstack_quote (&muscle_obstack,
                 quotearg_style (c_quoting_style, map_file_name (loc.start.file)));
  obstack_sgrow (&muscle_obstack, ")dnl\n[");
  ৱর্ণ ধ্রুৱ *extension = obstack_finish0 (&muscle_obstack);
  muscle_grow (চাবি, extension, "", "");
  obstack_free (&muscle_obstack, extension);
}

/*------------------------------------------------------------------.
| Append VALUE to the current value of KEY, using muscle_grow.  But |
| in addition, issue a synchronization line for the location LOC    |
| using muscle_syncline_grow.                                       |
`------------------------------------------------------------------*/

শূন্য
muscle_code_grow (ধ্রুৱ ৱর্ণ *চাবি, ধ্রুৱ ৱর্ণ *val, location loc)
{
  muscle_syncline_grow (চাবি, loc);
  muscle_grow (চাবি, val, "", "\n");
}


শূন্য
muscle_pair_list_grow (ধ্রুৱ ৱর্ণ *muscle,
                       ধ্রুৱ ৱর্ণ *a1, ধ্রুৱ ৱর্ণ *a2)
{
  obstack_sgrow (&muscle_obstack, "[");
  obstack_quote (&muscle_obstack, a1);
  obstack_sgrow (&muscle_obstack, ", ");
  obstack_quote (&muscle_obstack, a2);
  obstack_sgrow (&muscle_obstack, "]");
  ৱর্ণ ধ্রুৱ *pair = obstack_finish0 (&muscle_obstack);
  muscle_grow (muscle, pair, ",\n", "");
  obstack_free (&muscle_obstack, pair);
}


ৱর্ণ ধ্রুৱ *
muscle_find_const (ৱর্ণ ধ্রুৱ *চাবি)
{
  muscle_entry *entry = muscle_lookup (চাবি);
  ঘূৰাই entry ? entry->মান : রিক্ত;
}


ৱর্ণ *
muscle_find (ৱর্ণ ধ্রুৱ *চাবি)
{
  muscle_entry *entry = muscle_lookup (চাবি);
  যদি (entry)
    {
      aver (entry->মান == entry->storage);
      ঘূৰাই entry->storage;
    }
  ঘূৰাই রিক্ত;
}


/* In the format 'file_name:line.column', append BOUND to MUSCLE.  Use
   digraphs for special characters in the file name.  */

স্থির শূন্য
muscle_boundary_grow (ৱর্ণ ধ্রুৱ *চাবি, boundary bound)
{
  obstack_sgrow  (&muscle_obstack, "[[");
  obstack_escape (&muscle_obstack, bound.file);
  obstack_printf (&muscle_obstack, ":%d.%d@@%d]]", bound.সাৰি, bound.column, bound.byte);
  ৱর্ণ ধ্রুৱ *extension = obstack_finish0 (&muscle_obstack);
  muscle_grow (চাবি, extension, "", "");
  obstack_free (&muscle_obstack, extension);
}


শূন্য
muscle_location_grow (ৱর্ণ ধ্রুৱ *চাবি, location loc)
{
  muscle_boundary_grow (চাবি, loc.start);
  muscle_grow (চাবি, "", ", ", "");
  muscle_boundary_grow (চাবি, loc.end);
}

#define COMMON_DECODE(মান_4)                                    \
  পক্ষ '$':                                                     \
    ++(মান_4); aver (*(মান_4) == ']');                          \
    ++(মান_4); aver (*(মান_4) == '[');                          \
    obstack_sgrow (&muscle_obstack, "$");                       \
    ভঙ্গ;                                                      \
  পক্ষ '@':                                                     \
    ৱিকল্প (*++(মান_4))                                         \
      {                                                         \
      পক্ষ '\'': /* Ignore "@'" */ ভঙ্গ;                       \
      পক্ষ '@': obstack_sgrow (&muscle_obstack, "@" ); ভঙ্গ;   \
      পক্ষ '{': obstack_sgrow (&muscle_obstack, "[" ); ভঙ্গ;   \
      পক্ষ '}': obstack_sgrow (&muscle_obstack, "]" ); ভঙ্গ;   \
      স্ৱভাৱ: aver (false); ভঙ্গ;                             \
      }                                                         \
    ভঙ্গ;                                                      \
  স্ৱভাৱ:                                                      \
    obstack_1grow (&muscle_obstack, *(মান_4));                  \
    ভঙ্গ;

/* Reverse of obstack_escape.  */
স্থির ৱর্ণ *
string_decode (ৱর্ণ ধ্রুৱ *চাবি)
{
  ৱর্ণ ধ্রুৱ *মান = muscle_find_const (চাবি);
  যদি (!মান)
    ঘূৰাই রিক্ত;
  কৰা {
    ৱিকল্প (*মান)
      {
        COMMON_DECODE (মান)
        পক্ষ '[':
        পক্ষ ']':
          aver (false);
          ভঙ্গ;
      }
  } যেতিয়ালৈ (*মান++);
  ৱর্ণ ধ্রুৱ *মান_decoded = obstack_finish (&muscle_obstack);
  ৱর্ণ *res = xstrdup (মান_decoded);
  obstack_free (&muscle_obstack, মান_decoded);
  ঘূৰাই res;
}

/* Reverse of muscle_location_grow.  */
স্থির location
location_decode (ৱর্ণ ধ্রুৱ *মান)
{
  aver (মান);
  aver (*মান == '[');
  ++মান; aver (*মান == '[');
  location loc;
  যেতিয়ালৈ (*++মান)
    ৱিকল্প (*মান)
      {
        COMMON_DECODE (মান)
        পক্ষ '[':
          aver (false);
          ভঙ্গ;
        পক্ষ ']':
          ++মান; aver (*মান == ']');
          ৱর্ণ *boundary_str = obstack_finish0 (&muscle_obstack);
          ৱিকল্প (*++মান)
            {
            পক্ষ ',':
              boundary_set_from_string (&loc.start, boundary_str);
              obstack_free (&muscle_obstack, boundary_str);
              ++মান; aver (*মান == ' ');
              ++মান; aver (*মান == '[');
              ++মান; aver (*মান == '[');
              ভঙ্গ;
            পক্ষ '\0':
              boundary_set_from_string (&loc.end, boundary_str);
              obstack_free (&muscle_obstack, boundary_str);
              ঘূৰাই loc;
              ভঙ্গ;
            স্ৱভাৱ:
              aver (false);
              ভঙ্গ;
            }
          ভঙ্গ;
      }
  aver (false);
  ঘূৰাই loc;
}

শূন্য
muscle_user_name_list_grow (ৱর্ণ ধ্রুৱ *চাবি, ৱর্ণ ধ্রুৱ *user_name,
                            location loc)
{
  muscle_grow (চাবি, "[[[[", ",", "");
  muscle_grow (চাবি, user_name, "", "");
  muscle_grow (চাবি, "]], ", "", "");
  muscle_location_grow (চাবি, loc);
  muscle_grow (চাবি, "]]", "", "");
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

স্থির
ৱর্ণ *
define_directive (ৱর্ণ ধ্রুৱ *assignment,
                  muscle_kind kind,
                  ৱর্ণ ধ্রুৱ *মান)
{
  ৱর্ণ *eq = strchr (assignment, '=');
  ৱর্ণ ধ্রুৱ *fmt
    = eq || !মান || !*মান ? "%%define %s"
    : kind == muscle_code     ? "%%define %s {%s}"
    : kind == muscle_string   ? "%%define %s \"%s\""
    :                           "%%define %s %s";
  ৱর্ণ *res = xmalloc (strlen (fmt) + strlen (assignment)
                       + (মান ? strlen (মান) : 0));
  sprintf (res, fmt, assignment, মান);
  eq = strchr (res, '=');
  যদি (eq)
    *eq = eq[1] ? ' ' : '\0';
  ঘূৰাই res;
}

/** If the \a variable name is obsolete, return the name to use,
 * otherwise \a variable.  If the \a value is obsolete, update it too.
 *
 * Allocates the returned value if needed, otherwise the returned
 * value is exactly \a variable.  */
স্থির
ৱর্ণ ধ্রুৱ *
muscle_percent_variable_update (ৱর্ণ ধ্রুৱ *variable,
                                muscle_kind kind,
                                ৱর্ণ ধ্রুৱ **মান,
                                ৱর্ণ **old, ৱর্ণ **upd)
{
  প্রকারনির্দেশ সংরচনা
  {
    ধ্রুৱ ৱর্ণ *obsolete;
    ধ্রুৱ ৱর্ণ *updated;
    muscle_kind kind;
  } conversion_type;
  ধ্রুৱ conversion_type conversion[] =
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
    { রিক্ত, রিক্ত, -1, }
  };

  বাবে (conversion_type ধ্রুৱ *c = conversion; c->obsolete; ++c)
    {
      ৱর্ণ ধ্রুৱ *eq = strchr (c->obsolete, '=');
      যদি (eq
          ? (!strncmp (c->obsolete, variable, eq - c->obsolete)
             && STREQ (eq + 1, *মান))
          : STREQ (c->obsolete, variable))
        {
          /* Generate the deprecation warning. */
          *old = c->obsolete[0] == '%'
            ? xstrdup (c->obsolete)
            : define_directive (c->obsolete, kind, *মান);
          *upd = define_directive (c->updated, c->kind, *মান);
          /* Update the variable and its value.  */
          {
            ৱর্ণ *res = xstrdup (c->updated);
            ৱর্ণ *eq2 = strchr (res, '=');
            যদি (eq2)
              {
                *eq2 = '\0';
                *মান = eq2 + 1;
              }
            ঘূৰাই res;
          }
        }
    }
  ঘূৰাই variable;
}

শূন্য
muscle_percent_define_ঢোকাও (ৱর্ণ ধ্রুৱ *var, location variable_loc,
                              muscle_kind kind,
                              ৱর্ণ ধ্রুৱ *মান,
                              muscle_percent_define_how how)
{
  /* Backward compatibility.  */
  ৱর্ণ *old = রিক্ত;
  ৱর্ণ *upd = রিক্ত;
  ৱর্ণ ধ্রুৱ *variable
    = muscle_percent_variable_update (var, kind,
                                      &মান, &old, &upd);
  uniqstr name = muscle_name (variable, রিক্ত);
  uniqstr loc_name = muscle_name (variable, "loc");
  uniqstr syncline_name = muscle_name (variable, "syncline");
  uniqstr how_name = muscle_name (variable, "how");
  uniqstr kind_name = muscle_name (variable, "kind");

  /* Command-line options are processed before the grammar file.  */
  bool warned = false;
  যদি (how == MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE)
    {
      ৱর্ণ ধ্রুৱ *current_মান = muscle_find_const (name);
      যদি (current_মান)
        {
          long l = strtol (muscle_find_const (how_name), রিক্ত, 10);
          muscle_percent_define_how how_old
            = 0 <= l && l <= INT_MAX ? l : INT_MAX;
          যদি (how_old == MUSCLE_PERCENT_DEFINE_F)
            গচ্ছ end;
          /* If assigning the same value, make it a warning.  */
          warnings warn = STREQ (মান, current_মান) ? Wother : complaint;
          complain (&variable_loc, warn,
                    _("%%define variable %s redefined"),
                    quote (variable));
          location loc = muscle_percent_define_get_loc (variable);
          subcomplain (&loc, warn, _("previous definition"));
          fixits_register (&variable_loc, "");
          warned = true;
        }
    }

  যদি (!warned && old && upd)
    deprecated_directive (&variable_loc, old, upd);

  MUSCLE_ঢোকাও_STRING (name, মান);
  muscle_ঢোকাও (loc_name, "");
  muscle_location_grow (loc_name, variable_loc);
  muscle_ঢোকাও (syncline_name, "");
  muscle_syncline_grow (syncline_name, variable_loc);
  muscle_user_name_list_grow ("percent_define_user_variables", variable,
                              variable_loc);
  MUSCLE_ঢোকাও_INT (how_name, how);
  MUSCLE_ঢোকাও_STRING (kind_name, muscle_kind_string (kind));
 end:
  মুক্ত (old);
  মুক্ত (upd);
  যদি (variable != var)
    মুক্ত ((ৱর্ণ *) variable);
}

/* This is used for backward compatibility, e.g., "%define api.pure"
   supersedes "%pure-parser".  */
শূন্য
muscle_percent_define_ensure (ৱর্ণ ধ্রুৱ *variable, location loc,
                              bool মান)
{
  uniqstr name = muscle_name (variable, রিক্ত);
  ৱর্ণ ধ্রুৱ *val = মান ? "" : "false";

  /* Don't complain is VARIABLE is already defined, but be sure to set
     its value to VAL.  */
  যদি (!muscle_find_const (name)
      || muscle_percent_define_flag_if (variable) != মান)
    muscle_percent_define_ঢোকাও (variable, loc, muscle_keyword, val,
                                  MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
}

/* Mark %define VARIABLE as used.  */
স্থির শূন্য
muscle_percent_define_use (ৱর্ণ ধ্রুৱ *variable)
{
  muscle_ঢোকাও (muscle_name (variable, "bison_variables"), "");
}

/* The value of %define variable VARIABLE (corresponding to FIELD, if
   defined).  Do not register as used, but diagnose unset variables.  */

স্থির
ৱর্ণ ধ্রুৱ *
muscle_percent_define_get_raw (ৱর্ণ ধ্রুৱ *variable, ৱর্ণ ধ্রুৱ *field)
{
  uniqstr name = muscle_name (variable, field);
  ৱর্ণ ধ্রুৱ *res = muscle_find_const (name);
  যদি (!res)
    complain (রিক্ত, fatal, _("%s: undefined %%define variable %s"),
              "muscle_percent_define_get_raw", quote (variable));
  ঘূৰাই res;
}

ৱর্ণ *
muscle_percent_define_get (ৱর্ণ ধ্রুৱ *variable)
{
  uniqstr name = muscle_name (variable, রিক্ত);
  ৱর্ণ *মান = string_decode (name);
  যদি (!মান)
    মান = xstrdup ("");
  muscle_percent_define_use (variable);
  ঘূৰাই মান;
}

/* The kind of VARIABLE.  An error if undefined.  */
স্থির muscle_kind
muscle_percent_define_get_kind (ৱর্ণ ধ্রুৱ *variable)
{
  ঘূৰাই muscle_kind_new (muscle_percent_define_get_raw (variable, "kind"));
}

/* Check the kind of VARIABLE.  An error if undefined.  */
স্থির শূন্য
muscle_percent_define_check_kind (ৱর্ণ ধ্রুৱ *variable, muscle_kind kind)
{
  যদি (muscle_percent_define_get_kind (variable) != kind)
    {
      location loc = muscle_percent_define_get_loc (variable);
      ৱিকল্প (kind)
        {
        পক্ষ muscle_code:
          complain (&loc, Wdeprecated,
                    _("%%define variable '%s' requires '{...}' values"),
                    variable);
          ভঙ্গ;
        পক্ষ muscle_keyword:
          complain (&loc, Wdeprecated,
                    _("%%define variable '%s' requires keyword values"),
                    variable);
          ভঙ্গ;
        পক্ষ muscle_string:
          complain (&loc, Wdeprecated,
                    _("%%define variable '%s' requires '\"...\"' values"),
                    variable);
          ভঙ্গ;
        }
    }
}


location
muscle_percent_define_get_loc (ৱর্ণ ধ্রুৱ *variable)
{
  ঘূৰাই location_decode (muscle_percent_define_get_raw (variable, "loc"));
}

ৱর্ণ ধ্রুৱ *
muscle_percent_define_get_syncline (ৱর্ণ ধ্রুৱ *variable)
{
  ঘূৰাই muscle_percent_define_get_raw (variable, "syncline");
}

bool
muscle_percent_define_ifdef (ৱর্ণ ধ্রুৱ *variable)
{
  যদি (muscle_find_const (muscle_name (variable, রিক্ত)))
    {
      muscle_percent_define_use (variable);
      ঘূৰাই true;
    }
  অন্যথা
    ঘূৰাই false;
}

bool
muscle_percent_define_flag_if (ৱর্ণ ধ্রুৱ *variable)
{
  uniqstr invalid_boolean_name = muscle_name (variable, "invalid_boolean");
  bool res = false;

  যদি (muscle_percent_define_ifdef (variable))
    {
      ৱর্ণ *মান = muscle_percent_define_get (variable);
      muscle_percent_define_check_kind (variable, muscle_keyword);
      যদি (মান[0] == '\0' || STREQ (মান, "true"))
        res = true;
      অন্যথা যদি (STREQ (মান, "false"))
        res = false;
      অন্যথা যদি (!muscle_find_const (invalid_boolean_name))
        {
          muscle_ঢোকাও (invalid_boolean_name, "");
          location loc = muscle_percent_define_get_loc (variable);
          complain (&loc, complaint,
                    _("invalid value for %%define Boolean variable %s"),
                    quote (variable));
        }
      মুক্ত (মান);
    }
  অন্যথা
    complain (রিক্ত, fatal, _("%s: undefined %%define variable %s"),
              "muscle_percent_define_flag", quote (variable));

  ঘূৰাই res;
}

শূন্য
muscle_percent_define_default (ৱর্ণ ধ্রুৱ *variable, ৱর্ণ ধ্রুৱ *মান)
{
  uniqstr name = muscle_name (variable, রিক্ত);
  যদি (!muscle_find_const (name))
    {
      MUSCLE_ঢোকাও_STRING (name, মান);
      MUSCLE_ঢোকাও_STRING (muscle_name (variable, "kind"), "keyword");
      {
        uniqstr loc_name = muscle_name (variable, "loc");
        location loc;
        loc.start.file = "<default value>";
        loc.start.সাৰি = -1;
        loc.start.column = -1;
        loc.start.byte = -1;
        loc.end = loc.start;
        muscle_ঢোকাও (loc_name, "");
        muscle_location_grow (loc_name, loc);
      }
      muscle_ঢোকাও (muscle_name (variable, "syncline"), "");
    }
}

শূন্য
muscle_percent_define_check_মানসমূহ (ৱর্ণ ধ্রুৱ * ধ্রুৱ *মানসমূহ)
{
  বাবে (; *মানসমূহ; ++মানসমূহ)
    {
      ৱর্ণ ধ্রুৱ * ধ্রুৱ *variablep = মানসমূহ;
      uniqstr name = muscle_name (*variablep, রিক্ত);
      ৱর্ণ *মান = string_decode (name);
      muscle_percent_define_check_kind (*variablep, muscle_keyword);
      যদি (মান)
        {
          বাবে (++মানসমূহ; *মানসমূহ; ++মানসমূহ)
            যদি (STREQ (মান, *মানসমূহ))
              ভঙ্গ;
          যদি (!*মানসমূহ)
            {
              location loc = muscle_percent_define_get_loc (*variablep);
              complain (&loc, complaint,
                        _("invalid value for %%define variable %s: %s"),
                        quote (*variablep), quote_n (1, মান));
              বাবে (মানসমূহ = variablep + 1; *মানসমূহ; ++মানসমূহ)
                subcomplain (&loc, complaint | no_caret | silent,
                             _("accepted value: %s"), quote (*মানসমূহ));
            }
          অন্যথা
            যেতিয়ালৈ (*মানসমূহ)
              ++মানসমূহ;
          মুক্ত (মান);
        }
      অন্যথা
        complain (রিক্ত, fatal, _("%s: undefined %%define variable %s"),
                  "muscle_percent_define_check_values", quote (*variablep));
    }
}

শূন্য
muscle_percent_code_grow (ৱর্ণ ধ্রুৱ *qualifier, location qualifier_loc,
                          ৱর্ণ ধ্রুৱ *code, location code_loc)
{
  ৱর্ণ ধ্রুৱ *name = UNIQSTR_CONCAT ("percent_code(", qualifier, ")");
  muscle_code_grow (name, code, code_loc);
  muscle_user_name_list_grow ("percent_code_user_qualifiers", qualifier,
                               qualifier_loc);
}


/*------------------------------------------------.
| Output the definition of ENTRY as a m4_define.  |
`------------------------------------------------*/

স্থির অন্তঃস্থ bool
muscle_m4_output (muscle_entry *entry, FILE *out)
{
  fprintf (out,
           "m4_define([b4_%s],\n"
           "[[%s]])\n\n\n", entry->চাবি, entry->মান);
  ঘূৰাই true;
}

স্থির bool
muscle_m4_output_processor (শূন্য *entry, শূন্য *out)
{
  ঘূৰাই muscle_m4_output (entry, out);
}


শূন্য
muscles_m4_output (FILE *out)
{
  hash_do_for_each (muscle_table, muscle_m4_output_processor, out);
}
