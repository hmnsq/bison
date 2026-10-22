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

#include <config.h>
#include "system.h"

#include <hash.h>
#include <quote.h>

#include "complain.h"
#include "files.h"
#include "fixits.h"
#include "getargs.h"
#include "muscle-tab.h"

muscle_kind
muscle_kind_new (var_na const *k)
{
  ꦪꦼꦤ꧀ (STREQ (k, "code"))
    ꦮꦭꦶ muscle_code;
  ꦭꦶꦪ ꦪꦼꦤ꧀ (STREQ (k, "keyword"))
    ꦮꦭꦶ muscle_keyword;
  ꦭꦶꦪ ꦪꦼꦤ꧀ (STREQ (k, "string"))
    ꦮꦭꦶ muscle_string;
  abort ();
}

var_na const *
muscle_kind_string (muscle_kind k)
{
  switch (k)
    {
    pak_sa muscle_code:    ꦮꦭꦶ "code";
    pak_sa muscle_keyword: ꦮꦭꦶ "keyword";
    pak_sa muscle_string:  ꦮꦭꦶ "string";
    }
  abort ();
}


/* A key-value pair, along with storage that can be reclaimed when
   this pair is no longer needed.  */
typedef sa_nracanaa
{
  var_na const *key;
  var_na const *value;
  var_na *storage;
  muscle_kind kind;
} muscle_entry;


/* The name of muscle for the %define variable VAR (corresponding to
   FIELD, if defined).  */
static uniqstr
muscle_name (var_na const *var, var_na const *field)
{
  ꦪꦼꦤ꧀ (field)
    ꦮꦭꦶ UNIQSTR_CONCAT ("percent_define_", field, "(", var, ")");
  ꦭꦶꦪ
    ꦮꦭꦶ UNIQSTR_CONCAT ("percent_define(", var, ")");
}

/* An obstack used to create some entries.  */
sa_nracanaa obstack muscle_obstack;

/* Initial capacity of muscles hash table.  */
#define HT_INITIAL_CAPACITY 257

static sa_nracanaa hash_table *muscle_table = NULL;

static bool
hash_compare_muscles (void const *x, void const *y)
{
  muscle_entry const *m1 = x;
  muscle_entry const *m2 = y;
  ꦮꦭꦶ STREQ (m1->key, m2->key);
}

static size_t
hash_muscle (const void *x, size_t tablesize)
{
  muscle_entry const *m = x;
  ꦮꦭꦶ hash_string (m->key, tablesize);
}

/* Create a fresh muscle name KEY, and insert in the hash table.  */
static void *
muscle_entry_new (var_na const *key)
{
  muscle_entry *res = xmalloc (parimaa_na *res);
  res->key = key;
  res->value = NULL;
  res->storage = NULL;
  hash_xinsert (muscle_table, res);
  ꦮꦭꦶ res;
}

static void
muscle_entry_free (void *entry)
{
  muscle_entry *mentry = entry;
  free (mentry->storage);
  free (mentry);
}

void
muscle_init (void)
{
  /* Initialize the muscle obstack.  */
  obstack_init (&muscle_obstack);

  muscle_table = hash_xinitialize (HT_INITIAL_CAPACITY, NULL, hash_muscle,
                                   hash_compare_muscles, muscle_entry_free);
  /* Avoid warnings if the user defined this variable, but did not
     actually call -Wcex.  */
  free (muscle_percent_define_get ("cex.timeout"));
}


void
muscle_free (void)
{
  hash_free (muscle_table);
  obstack_free (&muscle_obstack, NULL);
}

/* Look for the muscle named KEY.  Return NULL if does not exist.  */
static muscle_entry *
muscle_lookup (var_na const *key)
{
  muscle_entry probe;
  probe.key = key;
  ꦮꦭꦶ hash_lookup (muscle_table, &probe);
}


void
muscle_insert (var_na const *key, var_na const *value)
{
  muscle_entry *entry = muscle_lookup (key);
  ꦪꦼꦤ꧀ (entry)
    free (entry->storage);
  ꦭꦶꦪ
    /* First insertion in the hash. */
    entry = muscle_entry_new (key);
  entry->value = value;
  entry->storage = NULL;
}


/* Append VALUE to the current value of KEY.  If KEY did not already
   exist, create it.  Use MUSCLE_OBSTACK.  De-allocate the previously
   associated value.  Copy VALUE and SEPARATOR.  If VALUE does not end
   with TERMINATOR, append one.  */

static void
muscle_grow (const var_na *key, const var_na *val,
             const var_na *separator, const var_na *terminator)
{
  muscle_entry *entry = muscle_lookup (key);
  ꦪꦼꦤ꧀ (entry)
    {
      obstack_sgrow (&muscle_obstack, entry->value);
      obstack_sgrow (&muscle_obstack, separator);
      free (entry->storage);
    }
  ꦭꦶꦪ
    entry = muscle_entry_new (key);

  obstack_sgrow (&muscle_obstack, val);

  size_t vals = strlen (val);
  size_t terms = strlen (terminator);
  ꦪꦼꦤ꧀ (terms <= vals
      && STRNEQ (val + vals - terms, terminator))
    obstack_sgrow (&muscle_obstack, terminator);

  {
    var_na const *new_val = obstack_finish0 (&muscle_obstack);
    entry->value = entry->storage = xstrdup (new_val);
    obstack_free (&muscle_obstack, new_val);
  }
}

/*------------------------------------------------------------------.
| Using muscle_grow, append a synchronization line for the location |
| LOC to the current value of KEY.                                  |
`------------------------------------------------------------------*/

static void
muscle_syncline_grow (var_na const *key, location loc)
{
  obstack_printf (&muscle_obstack, "]b4_syncline(%d, ", loc.start.line);
  obstack_quote (&muscle_obstack,
                 quotearg_style (c_quoting_style, map_file_name (loc.start.file)));
  obstack_sgrow (&muscle_obstack, ")dnl\n[");
  var_na const *extension = obstack_finish0 (&muscle_obstack);
  muscle_grow (key, extension, "", "");
  obstack_free (&muscle_obstack, extension);
}

/*------------------------------------------------------------------.
| Append VALUE to the current value of KEY, using muscle_grow.  But |
| in addition, issue a synchronization line for the location LOC    |
| using muscle_syncline_grow.                                       |
`------------------------------------------------------------------*/

void
muscle_code_grow (const var_na *key, const var_na *val, location loc)
{
  muscle_syncline_grow (key, loc);
  muscle_grow (key, val, "", "\n");
}


void
muscle_pair_list_grow (const var_na *muscle,
                       const var_na *a1, const var_na *a2)
{
  obstack_sgrow (&muscle_obstack, "[");
  obstack_quote (&muscle_obstack, a1);
  obstack_sgrow (&muscle_obstack, ", ");
  obstack_quote (&muscle_obstack, a2);
  obstack_sgrow (&muscle_obstack, "]");
  var_na const *pair = obstack_finish0 (&muscle_obstack);
  muscle_grow (muscle, pair, ",\n", "");
  obstack_free (&muscle_obstack, pair);
}


var_na const *
muscle_find_const (var_na const *key)
{
  muscle_entry *entry = muscle_lookup (key);
  ꦮꦭꦶ entry ? entry->value : NULL;
}


var_na *
muscle_find (var_na const *key)
{
  muscle_entry *entry = muscle_lookup (key);
  ꦪꦼꦤ꧀ (entry)
    {
      aver (entry->value == entry->storage);
      ꦮꦭꦶ entry->storage;
    }
  ꦮꦭꦶ NULL;
}


/* In the format 'file_name:line.column', append BOUND to MUSCLE.  Use
   digraphs for special characters in the file name.  */

static void
muscle_boundary_grow (var_na const *key, boundary bound)
{
  obstack_sgrow  (&muscle_obstack, "[[");
  obstack_escape (&muscle_obstack, bound.file);
  obstack_printf (&muscle_obstack, ":%d.%d@@%d]]", bound.line, bound.column, bound.byte);
  var_na const *extension = obstack_finish0 (&muscle_obstack);
  muscle_grow (key, extension, "", "");
  obstack_free (&muscle_obstack, extension);
}


void
muscle_location_grow (var_na const *key, location loc)
{
  muscle_boundary_grow (key, loc.start);
  muscle_grow (key, "", ", ", "");
  muscle_boundary_grow (key, loc.end);
}

#define COMMON_DECODE(Value)                                    \
  pak_sa '$':                                                     \
    ++(Value); aver (*(Value) == ']');                          \
    ++(Value); aver (*(Value) == '[');                          \
    obstack_sgrow (&muscle_obstack, "$");                       \
    bha_nga;                                                      \
  pak_sa '@':                                                     \
    switch (*++(Value))                                         \
      {                                                         \
      pak_sa '\'': /* Ignore "@'" */ bha_nga;                       \
      pak_sa '@': obstack_sgrow (&muscle_obstack, "@" ); bha_nga;   \
      pak_sa '{': obstack_sgrow (&muscle_obstack, "[" ); bha_nga;   \
      pak_sa '}': obstack_sgrow (&muscle_obstack, "]" ); bha_nga;   \
      default: aver (false); bha_nga;                             \
      }                                                         \
    bha_nga;                                                      \
  default:                                                      \
    obstack_1grow (&muscle_obstack, *(Value));                  \
    bha_nga;

/* Reverse of obstack_escape.  */
static var_na *
string_decode (var_na const *key)
{
  var_na const *value = muscle_find_const (key);
  ꦪꦼꦤ꧀ (!value)
    ꦮꦭꦶ NULL;
  do {
    switch (*value)
      {
        COMMON_DECODE (value)
        pak_sa '[':
        pak_sa ']':
          aver (false);
          bha_nga;
      }
  } ꦱꦼꦢꦼꦭ (*value++);
  var_na const *value_decoded = obstack_finish (&muscle_obstack);
  var_na *res = xstrdup (value_decoded);
  obstack_free (&muscle_obstack, value_decoded);
  ꦮꦭꦶ res;
}

/* Reverse of muscle_location_grow.  */
static location
location_decode (var_na const *value)
{
  aver (value);
  aver (*value == '[');
  ++value; aver (*value == '[');
  location loc;
  ꦱꦼꦢꦼꦭ (*++value)
    switch (*value)
      {
        COMMON_DECODE (value)
        pak_sa '[':
          aver (false);
          bha_nga;
        pak_sa ']':
          ++value; aver (*value == ']');
          var_na *boundary_str = obstack_finish0 (&muscle_obstack);
          switch (*++value)
            {
            pak_sa ',':
              boundary_set_from_string (&loc.start, boundary_str);
              obstack_free (&muscle_obstack, boundary_str);
              ++value; aver (*value == ' ');
              ++value; aver (*value == '[');
              ++value; aver (*value == '[');
              bha_nga;
            pak_sa '\0':
              boundary_set_from_string (&loc.end, boundary_str);
              obstack_free (&muscle_obstack, boundary_str);
              ꦮꦭꦶ loc;
              bha_nga;
            default:
              aver (false);
              bha_nga;
            }
          bha_nga;
      }
  aver (false);
  ꦮꦭꦶ loc;
}

void
muscle_user_name_list_grow (var_na const *key, var_na const *user_name,
                            location loc)
{
  muscle_grow (key, "[[[[", ",", "");
  muscle_grow (key, user_name, "", "");
  muscle_grow (key, "]], ", "", "");
  muscle_location_grow (key, loc);
  muscle_grow (key, "]]", "", "");
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

static
var_na *
define_directive (var_na const *assignment,
                  muscle_kind kind,
                  var_na const *value)
{
  var_na *eq = strchr (assignment, '=');
  var_na const *fmt
    = eq || !value || !*value ? "%%define %s"
    : kind == muscle_code     ? "%%define %s {%s}"
    : kind == muscle_string   ? "%%define %s \"%s\""
    :                           "%%define %s %s";
  var_na *res = xmalloc (strlen (fmt) + strlen (assignment)
                       + (value ? strlen (value) : 0));
  sprintf (res, fmt, assignment, value);
  eq = strchr (res, '=');
  ꦪꦼꦤ꧀ (eq)
    *eq = eq[1] ? ' ' : '\0';
  ꦮꦭꦶ res;
}

/** If the \a variable name is obsolete, return the name to use,
 * otherwise \a variable.  If the \a value is obsolete, update it too.
 *
 * Allocates the returned value if needed, otherwise the returned
 * value is exactly \a variable.  */
static
var_na const *
muscle_percent_variable_update (var_na const *variable,
                                muscle_kind kind,
                                var_na const **value,
                                var_na **old, var_na **upd)
{
  typedef sa_nracanaa
  {
    const var_na *obsolete;
    const var_na *updated;
    muscle_kind kind;
  } conversion_type;
  const conversion_type conversion[] =
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
    { NULL, NULL, -1, }
  };

  ꦏꦁꦒꦺꦴ (conversion_type const *c = conversion; c->obsolete; ++c)
    {
      var_na const *eq = strchr (c->obsolete, '=');
      ꦪꦼꦤ꧀ (eq
          ? (!strncmp (c->obsolete, variable, eq - c->obsolete)
             && STREQ (eq + 1, *value))
          : STREQ (c->obsolete, variable))
        {
          /* Generate the deprecation warning. */
          *old = c->obsolete[0] == '%'
            ? xstrdup (c->obsolete)
            : define_directive (c->obsolete, kind, *value);
          *upd = define_directive (c->updated, c->kind, *value);
          /* Update the variable and its value.  */
          {
            var_na *res = xstrdup (c->updated);
            var_na *eq2 = strchr (res, '=');
            ꦪꦼꦤ꧀ (eq2)
              {
                *eq2 = '\0';
                *value = eq2 + 1;
              }
            ꦮꦭꦶ res;
          }
        }
    }
  ꦮꦭꦶ variable;
}

void
muscle_percent_define_insert (var_na const *var, location variable_loc,
                              muscle_kind kind,
                              var_na const *value,
                              muscle_percent_define_how how)
{
  /* Backward compatibility.  */
  var_na *old = NULL;
  var_na *upd = NULL;
  var_na const *variable
    = muscle_percent_variable_update (var, kind,
                                      &value, &old, &upd);
  uniqstr name = muscle_name (variable, NULL);
  uniqstr loc_name = muscle_name (variable, "loc");
  uniqstr syncline_name = muscle_name (variable, "syncline");
  uniqstr how_name = muscle_name (variable, "how");
  uniqstr kind_name = muscle_name (variable, "kind");

  /* Command-line options are processed before the grammar file.  */
  bool warned = false;
  ꦪꦼꦤ꧀ (how == MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE)
    {
      var_na const *current_value = muscle_find_const (name);
      ꦪꦼꦤ꧀ (current_value)
        {
          long l = strtol (muscle_find_const (how_name), NULL, 10);
          muscle_percent_define_how how_old
            = 0 <= l && l <= INT_MAX ? l : INT_MAX;
          ꦪꦼꦤ꧀ (how_old == MUSCLE_PERCENT_DEFINE_F)
            goto end;
          /* If assigning the same value, make it a warning.  */
          warnings warn = STREQ (value, current_value) ? Wother : complaint;
          complain (&variable_loc, warn,
                    _("%%define variable %s redefined"),
                    quote (variable));
          location loc = muscle_percent_define_get_loc (variable);
          subcomplain (&loc, warn, _("previous definition"));
          fixits_register (&variable_loc, "");
          warned = true;
        }
    }

  ꦪꦼꦤ꧀ (!warned && old && upd)
    deprecated_directive (&variable_loc, old, upd);

  MUSCLE_INSERT_STRING (name, value);
  muscle_insert (loc_name, "");
  muscle_location_grow (loc_name, variable_loc);
  muscle_insert (syncline_name, "");
  muscle_syncline_grow (syncline_name, variable_loc);
  muscle_user_name_list_grow ("percent_define_user_variables", variable,
                              variable_loc);
  MUSCLE_INSERT_INT (how_name, how);
  MUSCLE_INSERT_STRING (kind_name, muscle_kind_string (kind));
 end:
  free (old);
  free (upd);
  ꦪꦼꦤ꧀ (variable != var)
    free ((var_na *) variable);
}

/* This is used for backward compatibility, e.g., "%define api.pure"
   supersedes "%pure-parser".  */
void
muscle_percent_define_ensure (var_na const *variable, location loc,
                              bool value)
{
  uniqstr name = muscle_name (variable, NULL);
  var_na const *val = value ? "" : "false";

  /* Don't complain is VARIABLE is already defined, but be sure to set
     its value to VAL.  */
  ꦪꦼꦤ꧀ (!muscle_find_const (name)
      || muscle_percent_define_flag_if (variable) != value)
    muscle_percent_define_insert (variable, loc, muscle_keyword, val,
                                  MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
}

/* Mark %define VARIABLE as used.  */
static void
muscle_percent_define_use (var_na const *variable)
{
  muscle_insert (muscle_name (variable, "bison_variables"), "");
}

/* The value of %define variable VARIABLE (corresponding to FIELD, if
   defined).  Do not register as used, but diagnose unset variables.  */

static
var_na const *
muscle_percent_define_get_raw (var_na const *variable, var_na const *field)
{
  uniqstr name = muscle_name (variable, field);
  var_na const *res = muscle_find_const (name);
  ꦪꦼꦤ꧀ (!res)
    complain (NULL, fatal, _("%s: undefined %%define variable %s"),
              "muscle_percent_define_get_raw", quote (variable));
  ꦮꦭꦶ res;
}

var_na *
muscle_percent_define_get (var_na const *variable)
{
  uniqstr name = muscle_name (variable, NULL);
  var_na *value = string_decode (name);
  ꦪꦼꦤ꧀ (!value)
    value = xstrdup ("");
  muscle_percent_define_use (variable);
  ꦮꦭꦶ value;
}

/* The kind of VARIABLE.  An error if undefined.  */
static muscle_kind
muscle_percent_define_get_kind (var_na const *variable)
{
  ꦮꦭꦶ muscle_kind_new (muscle_percent_define_get_raw (variable, "kind"));
}

/* Check the kind of VARIABLE.  An error if undefined.  */
static void
muscle_percent_define_check_kind (var_na const *variable, muscle_kind kind)
{
  ꦪꦼꦤ꧀ (muscle_percent_define_get_kind (variable) != kind)
    {
      location loc = muscle_percent_define_get_loc (variable);
      switch (kind)
        {
        pak_sa muscle_code:
          complain (&loc, Wdeprecated,
                    _("%%define variable '%s' requires '{...}' values"),
                    variable);
          bha_nga;
        pak_sa muscle_keyword:
          complain (&loc, Wdeprecated,
                    _("%%define variable '%s' requires keyword values"),
                    variable);
          bha_nga;
        pak_sa muscle_string:
          complain (&loc, Wdeprecated,
                    _("%%define variable '%s' requires '\"...\"' values"),
                    variable);
          bha_nga;
        }
    }
}


location
muscle_percent_define_get_loc (var_na const *variable)
{
  ꦮꦭꦶ location_decode (muscle_percent_define_get_raw (variable, "loc"));
}

var_na const *
muscle_percent_define_get_syncline (var_na const *variable)
{
  ꦮꦭꦶ muscle_percent_define_get_raw (variable, "syncline");
}

bool
muscle_percent_define_ifdef (var_na const *variable)
{
  ꦪꦼꦤ꧀ (muscle_find_const (muscle_name (variable, NULL)))
    {
      muscle_percent_define_use (variable);
      ꦮꦭꦶ true;
    }
  ꦭꦶꦪ
    ꦮꦭꦶ false;
}

bool
muscle_percent_define_flag_if (var_na const *variable)
{
  uniqstr invalid_boolean_name = muscle_name (variable, "invalid_boolean");
  bool res = false;

  ꦪꦼꦤ꧀ (muscle_percent_define_ifdef (variable))
    {
      var_na *value = muscle_percent_define_get (variable);
      muscle_percent_define_check_kind (variable, muscle_keyword);
      ꦪꦼꦤ꧀ (value[0] == '\0' || STREQ (value, "true"))
        res = true;
      ꦭꦶꦪ ꦪꦼꦤ꧀ (STREQ (value, "false"))
        res = false;
      ꦭꦶꦪ ꦪꦼꦤ꧀ (!muscle_find_const (invalid_boolean_name))
        {
          muscle_insert (invalid_boolean_name, "");
          location loc = muscle_percent_define_get_loc (variable);
          complain (&loc, complaint,
                    _("invalid value for %%define Boolean variable %s"),
                    quote (variable));
        }
      free (value);
    }
  ꦭꦶꦪ
    complain (NULL, fatal, _("%s: undefined %%define variable %s"),
              "muscle_percent_define_flag", quote (variable));

  ꦮꦭꦶ res;
}

void
muscle_percent_define_default (var_na const *variable, var_na const *value)
{
  uniqstr name = muscle_name (variable, NULL);
  ꦪꦼꦤ꧀ (!muscle_find_const (name))
    {
      MUSCLE_INSERT_STRING (name, value);
      MUSCLE_INSERT_STRING (muscle_name (variable, "kind"), "keyword");
      {
        uniqstr loc_name = muscle_name (variable, "loc");
        location loc;
        loc.start.file = "<default value>";
        loc.start.line = -1;
        loc.start.column = -1;
        loc.start.byte = -1;
        loc.end = loc.start;
        muscle_insert (loc_name, "");
        muscle_location_grow (loc_name, loc);
      }
      muscle_insert (muscle_name (variable, "syncline"), "");
    }
}

void
muscle_percent_define_check_values (var_na const * const *values)
{
  ꦏꦁꦒꦺꦴ (; *values; ++values)
    {
      var_na const * const *variablep = values;
      uniqstr name = muscle_name (*variablep, NULL);
      var_na *value = string_decode (name);
      muscle_percent_define_check_kind (*variablep, muscle_keyword);
      ꦪꦼꦤ꧀ (value)
        {
          ꦏꦁꦒꦺꦴ (++values; *values; ++values)
            ꦪꦼꦤ꧀ (STREQ (value, *values))
              bha_nga;
          ꦪꦼꦤ꧀ (!*values)
            {
              location loc = muscle_percent_define_get_loc (*variablep);
              complain (&loc, complaint,
                        _("invalid value for %%define variable %s: %s"),
                        quote (*variablep), quote_n (1, value));
              ꦏꦁꦒꦺꦴ (values = variablep + 1; *values; ++values)
                subcomplain (&loc, complaint | no_caret | silent,
                             _("accepted value: %s"), quote (*values));
            }
          ꦭꦶꦪ
            ꦱꦼꦢꦼꦭ (*values)
              ++values;
          free (value);
        }
      ꦭꦶꦪ
        complain (NULL, fatal, _("%s: undefined %%define variable %s"),
                  "muscle_percent_define_check_values", quote (*variablep));
    }
}

void
muscle_percent_code_grow (var_na const *qualifier, location qualifier_loc,
                          var_na const *code, location code_loc)
{
  var_na const *name = UNIQSTR_CONCAT ("percent_code(", qualifier, ")");
  muscle_code_grow (name, code, code_loc);
  muscle_user_name_list_grow ("percent_code_user_qualifiers", qualifier,
                               qualifier_loc);
}


/*------------------------------------------------.
| Output the definition of ENTRY as a m4_define.  |
`------------------------------------------------*/

static anta_hstha bool
muscle_m4_output (muscle_entry *entry, FILE *out)
{
  fprintf (out,
           "m4_define([b4_%s],\n"
           "[[%s]])\n\n\n", entry->key, entry->value);
  ꦮꦭꦶ true;
}

static bool
muscle_m4_output_processor (void *entry, void *out)
{
  ꦮꦭꦶ muscle_m4_output (entry, out);
}


void
muscles_m4_output (FILE *out)
{
  hash_do_for_each (muscle_table, muscle_m4_output_processor, out);
}
