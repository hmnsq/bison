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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব "system.h"

#অন্তর্ভাব <hash.h>
#অন্তর্ভাব <quote.h>

#অন্তর্ভাব "complain.h"
#অন্তর্ভাব "files.h"
#অন্তর্ভাব "fixits.h"
#অন্তর্ভাব "getargs.h"
#অন্তর্ভাব "muscle-tab.h"

muscle_kind
muscle_kind_new (অক্ষর ধ্রুবক *k)
{
  যদি (STREQ (k, "code"))
    ফেরত muscle_code;
  অন্যথা যদি (STREQ (k, "keyword"))
    ফেরত muscle_keyword;
  অন্যথা যদি (STREQ (k, "string"))
    ফেরত muscle_string;
  abort ();
}

অক্ষর ধ্রুবক *
muscle_kind_string (muscle_kind k)
{
  নির্বাচন (k)
    {
    ক্ষেত্র muscle_code:    ফেরত "code";
    ক্ষেত্র muscle_keyword: ফেরত "keyword";
    ক্ষেত্র muscle_string:  ফেরত "string";
    }
  abort ();
}


/* A key-value pair, along with storage that can be reclaimed when
   this pair is no longer needed.  */
প্রকারলেখ কাঠামো
{
  অক্ষর ধ্রুবক *চাবি;
  অক্ষর ধ্রুবক *মান;
  অক্ষর *storage;
  muscle_kind kind;
} muscle_entry;


/* The name of muscle for the %define variable VAR (corresponding to
   FIELD, if defined).  */
স্থির uniqstr
muscle_name (অক্ষর ধ্রুবক *var, অক্ষর ধ্রুবক *field)
{
  যদি (field)
    ফেরত UNIQSTR_CONCAT ("percent_define_", field, "(", var, ")");
  অন্যথা
    ফেরত UNIQSTR_CONCAT ("percent_define(", var, ")");
}

/* An obstack used to create some entries.  */
কাঠামো obstack muscle_obstack;

/* Initial capacity of muscles hash table.  */
#define HT_INITIAL_CAPACITY 257

স্থির কাঠামো hash_table *muscle_table = ফাঁকা;

স্থির bool
hash_compare_muscles (শূন্য ধ্রুবক *x, শূন্য ধ্রুবক *y)
{
  muscle_entry ধ্রুবক *m1 = x;
  muscle_entry ধ্রুবক *m2 = y;
  ফেরত STREQ (m1->চাবি, m2->চাবি);
}

স্থির size_t
hash_muscle (ধ্রুবক শূন্য *x, size_t tablesize)
{
  muscle_entry ধ্রুবক *m = x;
  ফেরত hash_string (m->চাবি, tablesize);
}

/* Create a fresh muscle name KEY, and insert in the hash table.  */
স্থির শূন্য *
muscle_entry_new (অক্ষর ধ্রুবক *চাবি)
{
  muscle_entry *res = xmalloc (আকার *res);
  res->চাবি = চাবি;
  res->মান = ফাঁকা;
  res->storage = ফাঁকা;
  hash_xinsert (muscle_table, res);
  ফেরত res;
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

  muscle_table = hash_xinitialize (HT_INITIAL_CAPACITY, ফাঁকা, hash_muscle,
                                   hash_compare_muscles, muscle_entry_free);
  /* Avoid warnings if the user defined this variable, but did not
     actually call -Wcex.  */
  মুক্ত (muscle_percent_define_get ("cex.timeout"));
}


শূন্য
muscle_free (শূন্য)
{
  hash_free (muscle_table);
  obstack_free (&muscle_obstack, ফাঁকা);
}

/* Look for the muscle named KEY.  Return NULL if does not exist.  */
স্থির muscle_entry *
muscle_lookup (অক্ষর ধ্রুবক *চাবি)
{
  muscle_entry probe;
  probe.চাবি = চাবি;
  ফেরত hash_lookup (muscle_table, &probe);
}


শূন্য
muscle_ঢোকাও (অক্ষর ধ্রুবক *চাবি, অক্ষর ধ্রুবক *মান)
{
  muscle_entry *entry = muscle_lookup (চাবি);
  যদি (entry)
    মুক্ত (entry->storage);
  অন্যথা
    /* First insertion in the hash. */
    entry = muscle_entry_new (চাবি);
  entry->মান = মান;
  entry->storage = ফাঁকা;
}


/* Append VALUE to the current value of KEY.  If KEY did not already
   exist, create it.  Use MUSCLE_OBSTACK.  De-allocate the previously
   associated value.  Copy VALUE and SEPARATOR.  If VALUE does not end
   with TERMINATOR, append one.  */

স্থির শূন্য
muscle_grow (ধ্রুবক অক্ষর *চাবি, ধ্রুবক অক্ষর *val,
             ধ্রুবক অক্ষর *separator, ধ্রুবক অক্ষর *terminator)
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
    অক্ষর ধ্রুবক *new_val = obstack_finish0 (&muscle_obstack);
    entry->মান = entry->storage = xstrdup (new_val);
    obstack_free (&muscle_obstack, new_val);
  }
}

/*------------------------------------------------------------------.
| Using muscle_grow, append a synchronization line for the location |
| LOC to the current value of KEY.                                  |
`------------------------------------------------------------------*/

স্থির শূন্য
muscle_syncline_grow (অক্ষর ধ্রুবক *চাবি, location loc)
{
  obstack_printf (&muscle_obstack, "]b4_syncline(%d, ", loc.start.সারি);
  obstack_quote (&muscle_obstack,
                 quotearg_style (c_quoting_style, map_file_name (loc.start.file)));
  obstack_sgrow (&muscle_obstack, ")dnl\n[");
  অক্ষর ধ্রুবক *extension = obstack_finish0 (&muscle_obstack);
  muscle_grow (চাবি, extension, "", "");
  obstack_free (&muscle_obstack, extension);
}

/*------------------------------------------------------------------.
| Append VALUE to the current value of KEY, using muscle_grow.  But |
| in addition, issue a synchronization line for the location LOC    |
| using muscle_syncline_grow.                                       |
`------------------------------------------------------------------*/

শূন্য
muscle_code_grow (ধ্রুবক অক্ষর *চাবি, ধ্রুবক অক্ষর *val, location loc)
{
  muscle_syncline_grow (চাবি, loc);
  muscle_grow (চাবি, val, "", "\n");
}


শূন্য
muscle_pair_list_grow (ধ্রুবক অক্ষর *muscle,
                       ধ্রুবক অক্ষর *a1, ধ্রুবক অক্ষর *a2)
{
  obstack_sgrow (&muscle_obstack, "[");
  obstack_quote (&muscle_obstack, a1);
  obstack_sgrow (&muscle_obstack, ", ");
  obstack_quote (&muscle_obstack, a2);
  obstack_sgrow (&muscle_obstack, "]");
  অক্ষর ধ্রুবক *pair = obstack_finish0 (&muscle_obstack);
  muscle_grow (muscle, pair, ",\n", "");
  obstack_free (&muscle_obstack, pair);
}


অক্ষর ধ্রুবক *
muscle_find_const (অক্ষর ধ্রুবক *চাবি)
{
  muscle_entry *entry = muscle_lookup (চাবি);
  ফেরত entry ? entry->মান : ফাঁকা;
}


অক্ষর *
muscle_find (অক্ষর ধ্রুবক *চাবি)
{
  muscle_entry *entry = muscle_lookup (চাবি);
  যদি (entry)
    {
      aver (entry->মান == entry->storage);
      ফেরত entry->storage;
    }
  ফেরত ফাঁকা;
}


/* In the format 'file_name:line.column', append BOUND to MUSCLE.  Use
   digraphs for special characters in the file name.  */

স্থির শূন্য
muscle_boundary_grow (অক্ষর ধ্রুবক *চাবি, boundary bound)
{
  obstack_sgrow  (&muscle_obstack, "[[");
  obstack_escape (&muscle_obstack, bound.file);
  obstack_printf (&muscle_obstack, ":%d.%d@@%d]]", bound.সারি, bound.column, bound.byte);
  অক্ষর ধ্রুবক *extension = obstack_finish0 (&muscle_obstack);
  muscle_grow (চাবি, extension, "", "");
  obstack_free (&muscle_obstack, extension);
}


শূন্য
muscle_location_grow (অক্ষর ধ্রুবক *চাবি, location loc)
{
  muscle_boundary_grow (চাবি, loc.start);
  muscle_grow (চাবি, "", ", ", "");
  muscle_boundary_grow (চাবি, loc.end);
}

#define COMMON_DECODE(মান_4)                                    \
  ক্ষেত্র '$':                                                     \
    ++(মান_4); aver (*(মান_4) == ']');                          \
    ++(মান_4); aver (*(মান_4) == '[');                          \
    obstack_sgrow (&muscle_obstack, "$");                       \
    বিরতি;                                                      \
  ক্ষেত্র '@':                                                     \
    নির্বাচন (*++(মান_4))                                         \
      {                                                         \
      ক্ষেত্র '\'': /* Ignore "@'" */ বিরতি;                       \
      ক্ষেত্র '@': obstack_sgrow (&muscle_obstack, "@" ); বিরতি;   \
      ক্ষেত্র '{': obstack_sgrow (&muscle_obstack, "[" ); বিরতি;   \
      ক্ষেত্র '}': obstack_sgrow (&muscle_obstack, "]" ); বিরতি;   \
      পূর্বনির্ধারিত: aver (false); বিরতি;                             \
      }                                                         \
    বিরতি;                                                      \
  পূর্বনির্ধারিত:                                                      \
    obstack_1grow (&muscle_obstack, *(মান_4));                  \
    বিরতি;

/* Reverse of obstack_escape.  */
স্থির অক্ষর *
string_decode (অক্ষর ধ্রুবক *চাবি)
{
  অক্ষর ধ্রুবক *মান = muscle_find_const (চাবি);
  যদি (!মান)
    ফেরত ফাঁকা;
  করো {
    নির্বাচন (*মান)
      {
        COMMON_DECODE (মান)
        ক্ষেত্র '[':
        ক্ষেত্র ']':
          aver (false);
          বিরতি;
      }
  } যখন (*মান++);
  অক্ষর ধ্রুবক *মান_decoded = obstack_finish (&muscle_obstack);
  অক্ষর *res = xstrdup (মান_decoded);
  obstack_free (&muscle_obstack, মান_decoded);
  ফেরত res;
}

/* Reverse of muscle_location_grow.  */
স্থির location
location_decode (অক্ষর ধ্রুবক *মান)
{
  aver (মান);
  aver (*মান == '[');
  ++মান; aver (*মান == '[');
  location loc;
  যখন (*++মান)
    নির্বাচন (*মান)
      {
        COMMON_DECODE (মান)
        ক্ষেত্র '[':
          aver (false);
          বিরতি;
        ক্ষেত্র ']':
          ++মান; aver (*মান == ']');
          অক্ষর *boundary_str = obstack_finish0 (&muscle_obstack);
          নির্বাচন (*++মান)
            {
            ক্ষেত্র ',':
              boundary_set_from_string (&loc.start, boundary_str);
              obstack_free (&muscle_obstack, boundary_str);
              ++মান; aver (*মান == ' ');
              ++মান; aver (*মান == '[');
              ++মান; aver (*মান == '[');
              বিরতি;
            ক্ষেত্র '\0':
              boundary_set_from_string (&loc.end, boundary_str);
              obstack_free (&muscle_obstack, boundary_str);
              ফেরত loc;
              বিরতি;
            পূর্বনির্ধারিত:
              aver (false);
              বিরতি;
            }
          বিরতি;
      }
  aver (false);
  ফেরত loc;
}

শূন্য
muscle_user_name_list_grow (অক্ষর ধ্রুবক *চাবি, অক্ষর ধ্রুবক *user_name,
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
অক্ষর *
define_directive (অক্ষর ধ্রুবক *assignment,
                  muscle_kind kind,
                  অক্ষর ধ্রুবক *মান)
{
  অক্ষর *eq = strchr (assignment, '=');
  অক্ষর ধ্রুবক *fmt
    = eq || !মান || !*মান ? "%%define %s"
    : kind == muscle_code     ? "%%define %s {%s}"
    : kind == muscle_string   ? "%%define %s \"%s\""
    :                           "%%define %s %s";
  অক্ষর *res = xmalloc (strlen (fmt) + strlen (assignment)
                       + (মান ? strlen (মান) : 0));
  sprintf (res, fmt, assignment, মান);
  eq = strchr (res, '=');
  যদি (eq)
    *eq = eq[1] ? ' ' : '\0';
  ফেরত res;
}

/** If the \a variable name is obsolete, return the name to use,
 * otherwise \a variable.  If the \a value is obsolete, update it too.
 *
 * Allocates the returned value if needed, otherwise the returned
 * value is exactly \a variable.  */
স্থির
অক্ষর ধ্রুবক *
muscle_percent_variable_update (অক্ষর ধ্রুবক *variable,
                                muscle_kind kind,
                                অক্ষর ধ্রুবক **মান,
                                অক্ষর **old, অক্ষর **upd)
{
  প্রকারলেখ কাঠামো
  {
    ধ্রুবক অক্ষর *obsolete;
    ধ্রুবক অক্ষর *updated;
    muscle_kind kind;
  } conversion_type;
  ধ্রুবক conversion_type conversion[] =
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
    { ফাঁকা, ফাঁকা, -1, }
  };

  জন্য (conversion_type ধ্রুবক *c = conversion; c->obsolete; ++c)
    {
      অক্ষর ধ্রুবক *eq = strchr (c->obsolete, '=');
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
            অক্ষর *res = xstrdup (c->updated);
            অক্ষর *eq2 = strchr (res, '=');
            যদি (eq2)
              {
                *eq2 = '\0';
                *মান = eq2 + 1;
              }
            ফেরত res;
          }
        }
    }
  ফেরত variable;
}

শূন্য
muscle_percent_define_ঢোকাও (অক্ষর ধ্রুবক *var, location variable_loc,
                              muscle_kind kind,
                              অক্ষর ধ্রুবক *মান,
                              muscle_percent_define_how how)
{
  /* Backward compatibility.  */
  অক্ষর *old = ফাঁকা;
  অক্ষর *upd = ফাঁকা;
  অক্ষর ধ্রুবক *variable
    = muscle_percent_variable_update (var, kind,
                                      &মান, &old, &upd);
  uniqstr name = muscle_name (variable, ফাঁকা);
  uniqstr loc_name = muscle_name (variable, "loc");
  uniqstr syncline_name = muscle_name (variable, "syncline");
  uniqstr how_name = muscle_name (variable, "how");
  uniqstr kind_name = muscle_name (variable, "kind");

  /* Command-line options are processed before the grammar file.  */
  bool warned = false;
  যদি (how == MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE)
    {
      অক্ষর ধ্রুবক *current_মান = muscle_find_const (name);
      যদি (current_মান)
        {
          long l = strtol (muscle_find_const (how_name), ফাঁকা, 10);
          muscle_percent_define_how how_old
            = 0 <= l && l <= INT_MAX ? l : INT_MAX;
          যদি (how_old == MUSCLE_PERCENT_DEFINE_F)
            যাও end;
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
    মুক্ত ((অক্ষর *) variable);
}

/* This is used for backward compatibility, e.g., "%define api.pure"
   supersedes "%pure-parser".  */
শূন্য
muscle_percent_define_ensure (অক্ষর ধ্রুবক *variable, location loc,
                              bool মান)
{
  uniqstr name = muscle_name (variable, ফাঁকা);
  অক্ষর ধ্রুবক *val = মান ? "" : "false";

  /* Don't complain is VARIABLE is already defined, but be sure to set
     its value to VAL.  */
  যদি (!muscle_find_const (name)
      || muscle_percent_define_flag_if (variable) != মান)
    muscle_percent_define_ঢোকাও (variable, loc, muscle_keyword, val,
                                  MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
}

/* Mark %define VARIABLE as used.  */
স্থির শূন্য
muscle_percent_define_use (অক্ষর ধ্রুবক *variable)
{
  muscle_ঢোকাও (muscle_name (variable, "bison_variables"), "");
}

/* The value of %define variable VARIABLE (corresponding to FIELD, if
   defined).  Do not register as used, but diagnose unset variables.  */

স্থির
অক্ষর ধ্রুবক *
muscle_percent_define_get_raw (অক্ষর ধ্রুবক *variable, অক্ষর ধ্রুবক *field)
{
  uniqstr name = muscle_name (variable, field);
  অক্ষর ধ্রুবক *res = muscle_find_const (name);
  যদি (!res)
    complain (ফাঁকা, fatal, _("%s: undefined %%define variable %s"),
              "muscle_percent_define_get_raw", quote (variable));
  ফেরত res;
}

অক্ষর *
muscle_percent_define_get (অক্ষর ধ্রুবক *variable)
{
  uniqstr name = muscle_name (variable, ফাঁকা);
  অক্ষর *মান = string_decode (name);
  যদি (!মান)
    মান = xstrdup ("");
  muscle_percent_define_use (variable);
  ফেরত মান;
}

/* The kind of VARIABLE.  An error if undefined.  */
স্থির muscle_kind
muscle_percent_define_get_kind (অক্ষর ধ্রুবক *variable)
{
  ফেরত muscle_kind_new (muscle_percent_define_get_raw (variable, "kind"));
}

/* Check the kind of VARIABLE.  An error if undefined.  */
স্থির শূন্য
muscle_percent_define_check_kind (অক্ষর ধ্রুবক *variable, muscle_kind kind)
{
  যদি (muscle_percent_define_get_kind (variable) != kind)
    {
      location loc = muscle_percent_define_get_loc (variable);
      নির্বাচন (kind)
        {
        ক্ষেত্র muscle_code:
          complain (&loc, Wdeprecated,
                    _("%%define variable '%s' requires '{...}' values"),
                    variable);
          বিরতি;
        ক্ষেত্র muscle_keyword:
          complain (&loc, Wdeprecated,
                    _("%%define variable '%s' requires keyword values"),
                    variable);
          বিরতি;
        ক্ষেত্র muscle_string:
          complain (&loc, Wdeprecated,
                    _("%%define variable '%s' requires '\"...\"' values"),
                    variable);
          বিরতি;
        }
    }
}


location
muscle_percent_define_get_loc (অক্ষর ধ্রুবক *variable)
{
  ফেরত location_decode (muscle_percent_define_get_raw (variable, "loc"));
}

অক্ষর ধ্রুবক *
muscle_percent_define_get_syncline (অক্ষর ধ্রুবক *variable)
{
  ফেরত muscle_percent_define_get_raw (variable, "syncline");
}

bool
muscle_percent_define_ifdef (অক্ষর ধ্রুবক *variable)
{
  যদি (muscle_find_const (muscle_name (variable, ফাঁকা)))
    {
      muscle_percent_define_use (variable);
      ফেরত true;
    }
  অন্যথা
    ফেরত false;
}

bool
muscle_percent_define_flag_if (অক্ষর ধ্রুবক *variable)
{
  uniqstr invalid_boolean_name = muscle_name (variable, "invalid_boolean");
  bool res = false;

  যদি (muscle_percent_define_ifdef (variable))
    {
      অক্ষর *মান = muscle_percent_define_get (variable);
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
    complain (ফাঁকা, fatal, _("%s: undefined %%define variable %s"),
              "muscle_percent_define_flag", quote (variable));

  ফেরত res;
}

শূন্য
muscle_percent_define_default (অক্ষর ধ্রুবক *variable, অক্ষর ধ্রুবক *মান)
{
  uniqstr name = muscle_name (variable, ফাঁকা);
  যদি (!muscle_find_const (name))
    {
      MUSCLE_ঢোকাও_STRING (name, মান);
      MUSCLE_ঢোকাও_STRING (muscle_name (variable, "kind"), "keyword");
      {
        uniqstr loc_name = muscle_name (variable, "loc");
        location loc;
        loc.start.file = "<default value>";
        loc.start.সারি = -1;
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
muscle_percent_define_check_মানসমূহ (অক্ষর ধ্রুবক * ধ্রুবক *মানসমূহ)
{
  জন্য (; *মানসমূহ; ++মানসমূহ)
    {
      অক্ষর ধ্রুবক * ধ্রুবক *variablep = মানসমূহ;
      uniqstr name = muscle_name (*variablep, ফাঁকা);
      অক্ষর *মান = string_decode (name);
      muscle_percent_define_check_kind (*variablep, muscle_keyword);
      যদি (মান)
        {
          জন্য (++মানসমূহ; *মানসমূহ; ++মানসমূহ)
            যদি (STREQ (মান, *মানসমূহ))
              বিরতি;
          যদি (!*মানসমূহ)
            {
              location loc = muscle_percent_define_get_loc (*variablep);
              complain (&loc, complaint,
                        _("invalid value for %%define variable %s: %s"),
                        quote (*variablep), quote_n (1, মান));
              জন্য (মানসমূহ = variablep + 1; *মানসমূহ; ++মানসমূহ)
                subcomplain (&loc, complaint | no_caret | silent,
                             _("accepted value: %s"), quote (*মানসমূহ));
            }
          অন্যথা
            যখন (*মানসমূহ)
              ++মানসমূহ;
          মুক্ত (মান);
        }
      অন্যথা
        complain (ফাঁকা, fatal, _("%s: undefined %%define variable %s"),
                  "muscle_percent_define_check_values", quote (*variablep));
    }
}

শূন্য
muscle_percent_code_grow (অক্ষর ধ্রুবক *qualifier, location qualifier_loc,
                          অক্ষর ধ্রুবক *code, location code_loc)
{
  অক্ষর ধ্রুবক *name = UNIQSTR_CONCAT ("percent_code(", qualifier, ")");
  muscle_code_grow (name, code, code_loc);
  muscle_user_name_list_grow ("percent_code_user_qualifiers", qualifier,
                               qualifier_loc);
}


/*------------------------------------------------.
| Output the definition of ENTRY as a m4_define.  |
`------------------------------------------------*/

স্থির অন্তঃরেখ bool
muscle_m4_output (muscle_entry *entry, FILE *out)
{
  fprintf (out,
           "m4_define([b4_%s],\n"
           "[[%s]])\n\n\n", entry->চাবি, entry->মান);
  ফেরত true;
}

স্থির bool
muscle_m4_output_processor (শূন্য *entry, শূন্য *out)
{
  ফেরত muscle_m4_output (entry, out);
}


শূন্য
muscles_m4_output (FILE *out)
{
  hash_do_for_each (muscle_table, muscle_m4_output_processor, out);
}
