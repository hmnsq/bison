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

#अन्तर्भाव <config.h>
#अन्तर्भाव "system.h"

#अन्तर्भाव <hash.h>
#अन्तर्भाव <quote.h>

#अन्तर्भाव "complain.h"
#अन्तर्भाव "files.h"
#अन्तर्भाव "fixits.h"
#अन्तर्भाव "getargs.h"
#अन्तर्भाव "muscle-tab.h"

muscle_kind
muscle_kind_new (वर्ण ध्रुव *k)
{
  यदि (STREQ (k, "code"))
    प्रत्यागम muscle_code;
  अन्यथा यदि (STREQ (k, "keyword"))
    प्रत्यागम muscle_keyword;
  अन्यथा यदि (STREQ (k, "string"))
    प्रत्यागम muscle_string;
  abort ();
}

वर्ण ध्रुव *
muscle_kind_string (muscle_kind k)
{
  विकल्प (k)
    {
    पक्ष muscle_code:    प्रत्यागम "code";
    पक्ष muscle_keyword: प्रत्यागम "keyword";
    पक्ष muscle_string:  प्रत्यागम "string";
    }
  abort ();
}


/* A key-value pair, along with storage that can be reclaimed when
   this pair is no longer needed.  */
प्रकारनिर्देश संरचना
{
  वर्ण ध्रुव *कुंजी;
  वर्ण ध्रुव *मूल्य;
  वर्ण *storage;
  muscle_kind kind;
} muscle_entry;


/* The name of muscle for the %define variable VAR (corresponding to
   FIELD, if defined).  */
स्थिर uniqstr
muscle_name (वर्ण ध्रुव *var, वर्ण ध्रुव *field)
{
  यदि (field)
    प्रत्यागम UNIQSTR_CONCAT ("percent_define_", field, "(", var, ")");
  अन्यथा
    प्रत्यागम UNIQSTR_CONCAT ("percent_define(", var, ")");
}

/* An obstack used to create some entries.  */
संरचना obstack muscle_obstack;

/* Initial capacity of muscles hash table.  */
#define HT_INITIAL_CAPACITY 257

स्थिर संरचना hash_table *muscle_table = रिक्त;

स्थिर bool
hash_compare_muscles (शून्य ध्रुव *x, शून्य ध्रुव *y)
{
  muscle_entry ध्रुव *m1 = x;
  muscle_entry ध्रुव *m2 = y;
  प्रत्यागम STREQ (m1->कुंजी, m2->कुंजी);
}

स्थिर size_t
hash_muscle (ध्रुव शून्य *x, size_t tablesize)
{
  muscle_entry ध्रुव *m = x;
  प्रत्यागम hash_string (m->कुंजी, tablesize);
}

/* Create a fresh muscle name KEY, and insert in the hash table.  */
स्थिर शून्य *
muscle_entry_new (वर्ण ध्रुव *कुंजी)
{
  muscle_entry *res = xmalloc (परिमाण *res);
  res->कुंजी = कुंजी;
  res->मूल्य = रिक्त;
  res->storage = रिक्त;
  hash_xinsert (muscle_table, res);
  प्रत्यागम res;
}

स्थिर शून्य
muscle_entry_free (शून्य *entry)
{
  muscle_entry *mentry = entry;
  मुक्त (mentry->storage);
  मुक्त (mentry);
}

शून्य
muscle_init (शून्य)
{
  /* Initialize the muscle obstack.  */
  obstack_init (&muscle_obstack);

  muscle_table = hash_xinitialize (HT_INITIAL_CAPACITY, रिक्त, hash_muscle,
                                   hash_compare_muscles, muscle_entry_free);
  /* Avoid warnings if the user defined this variable, but did not
     actually call -Wcex.  */
  मुक्त (muscle_percent_define_get ("cex.timeout"));
}


शून्य
muscle_free (शून्य)
{
  hash_free (muscle_table);
  obstack_free (&muscle_obstack, रिक्त);
}

/* Look for the muscle named KEY.  Return NULL if does not exist.  */
स्थिर muscle_entry *
muscle_lookup (वर्ण ध्रुव *कुंजी)
{
  muscle_entry probe;
  probe.कुंजी = कुंजी;
  प्रत्यागम hash_lookup (muscle_table, &probe);
}


शून्य
muscle_जोड़ो (वर्ण ध्रुव *कुंजी, वर्ण ध्रुव *मूल्य)
{
  muscle_entry *entry = muscle_lookup (कुंजी);
  यदि (entry)
    मुक्त (entry->storage);
  अन्यथा
    /* First insertion in the hash. */
    entry = muscle_entry_new (कुंजी);
  entry->मूल्य = मूल्य;
  entry->storage = रिक्त;
}


/* Append VALUE to the current value of KEY.  If KEY did not already
   exist, create it.  Use MUSCLE_OBSTACK.  De-allocate the previously
   associated value.  Copy VALUE and SEPARATOR.  If VALUE does not end
   with TERMINATOR, append one.  */

स्थिर शून्य
muscle_grow (ध्रुव वर्ण *कुंजी, ध्रुव वर्ण *val,
             ध्रुव वर्ण *separator, ध्रुव वर्ण *terminator)
{
  muscle_entry *entry = muscle_lookup (कुंजी);
  यदि (entry)
    {
      obstack_sgrow (&muscle_obstack, entry->मूल्य);
      obstack_sgrow (&muscle_obstack, separator);
      मुक्त (entry->storage);
    }
  अन्यथा
    entry = muscle_entry_new (कुंजी);

  obstack_sgrow (&muscle_obstack, val);

  size_t vals = strlen (val);
  size_t terms = strlen (terminator);
  यदि (terms <= vals
      && STRNEQ (val + vals - terms, terminator))
    obstack_sgrow (&muscle_obstack, terminator);

  {
    वर्ण ध्रुव *new_val = obstack_finish0 (&muscle_obstack);
    entry->मूल्य = entry->storage = xstrdup (new_val);
    obstack_free (&muscle_obstack, new_val);
  }
}

/*------------------------------------------------------------------.
| Using muscle_grow, append a synchronization line for the location |
| LOC to the current value of KEY.                                  |
`------------------------------------------------------------------*/

स्थिर शून्य
muscle_syncline_grow (वर्ण ध्रुव *कुंजी, location loc)
{
  obstack_printf (&muscle_obstack, "]b4_syncline(%d, ", loc.start.पंक्ति);
  obstack_quote (&muscle_obstack,
                 quotearg_style (c_quoting_style, map_file_name (loc.start.file)));
  obstack_sgrow (&muscle_obstack, ")dnl\n[");
  वर्ण ध्रुव *extension = obstack_finish0 (&muscle_obstack);
  muscle_grow (कुंजी, extension, "", "");
  obstack_free (&muscle_obstack, extension);
}

/*------------------------------------------------------------------.
| Append VALUE to the current value of KEY, using muscle_grow.  But |
| in addition, issue a synchronization line for the location LOC    |
| using muscle_syncline_grow.                                       |
`------------------------------------------------------------------*/

शून्य
muscle_code_grow (ध्रुव वर्ण *कुंजी, ध्रुव वर्ण *val, location loc)
{
  muscle_syncline_grow (कुंजी, loc);
  muscle_grow (कुंजी, val, "", "\n");
}


शून्य
muscle_pair_list_grow (ध्रुव वर्ण *muscle,
                       ध्रुव वर्ण *a1, ध्रुव वर्ण *a2)
{
  obstack_sgrow (&muscle_obstack, "[");
  obstack_quote (&muscle_obstack, a1);
  obstack_sgrow (&muscle_obstack, ", ");
  obstack_quote (&muscle_obstack, a2);
  obstack_sgrow (&muscle_obstack, "]");
  वर्ण ध्रुव *pair = obstack_finish0 (&muscle_obstack);
  muscle_grow (muscle, pair, ",\n", "");
  obstack_free (&muscle_obstack, pair);
}


वर्ण ध्रुव *
muscle_find_const (वर्ण ध्रुव *कुंजी)
{
  muscle_entry *entry = muscle_lookup (कुंजी);
  प्रत्यागम entry ? entry->मूल्य : रिक्त;
}


वर्ण *
muscle_find (वर्ण ध्रुव *कुंजी)
{
  muscle_entry *entry = muscle_lookup (कुंजी);
  यदि (entry)
    {
      aver (entry->मूल्य == entry->storage);
      प्रत्यागम entry->storage;
    }
  प्रत्यागम रिक्त;
}


/* In the format 'file_name:line.column', append BOUND to MUSCLE.  Use
   digraphs for special characters in the file name.  */

स्थिर शून्य
muscle_boundary_grow (वर्ण ध्रुव *कुंजी, boundary bound)
{
  obstack_sgrow  (&muscle_obstack, "[[");
  obstack_escape (&muscle_obstack, bound.file);
  obstack_printf (&muscle_obstack, ":%d.%d@@%d]]", bound.पंक्ति, bound.column, bound.byte);
  वर्ण ध्रुव *extension = obstack_finish0 (&muscle_obstack);
  muscle_grow (कुंजी, extension, "", "");
  obstack_free (&muscle_obstack, extension);
}


शून्य
muscle_location_grow (वर्ण ध्रुव *कुंजी, location loc)
{
  muscle_boundary_grow (कुंजी, loc.start);
  muscle_grow (कुंजी, "", ", ", "");
  muscle_boundary_grow (कुंजी, loc.end);
}

#define COMMON_DECODE(मूल्य_4)                                    \
  पक्ष '$':                                                     \
    ++(मूल्य_4); aver (*(मूल्य_4) == ']');                          \
    ++(मूल्य_4); aver (*(मूल्य_4) == '[');                          \
    obstack_sgrow (&muscle_obstack, "$");                       \
    भङ्ग;                                                      \
  पक्ष '@':                                                     \
    विकल्प (*++(मूल्य_4))                                         \
      {                                                         \
      पक्ष '\'': /* Ignore "@'" */ भङ्ग;                       \
      पक्ष '@': obstack_sgrow (&muscle_obstack, "@" ); भङ्ग;   \
      पक्ष '{': obstack_sgrow (&muscle_obstack, "[" ); भङ्ग;   \
      पक्ष '}': obstack_sgrow (&muscle_obstack, "]" ); भङ्ग;   \
      स्वभाव: aver (false); भङ्ग;                             \
      }                                                         \
    भङ्ग;                                                      \
  स्वभाव:                                                      \
    obstack_1grow (&muscle_obstack, *(मूल्य_4));                  \
    भङ्ग;

/* Reverse of obstack_escape.  */
स्थिर वर्ण *
string_decode (वर्ण ध्रुव *कुंजी)
{
  वर्ण ध्रुव *मूल्य = muscle_find_const (कुंजी);
  यदि (!मूल्य)
    प्रत्यागम रिक्त;
  कुरु {
    विकल्प (*मूल्य)
      {
        COMMON_DECODE (मूल्य)
        पक्ष '[':
        पक्ष ']':
          aver (false);
          भङ्ग;
      }
  } यावत् (*मूल्य++);
  वर्ण ध्रुव *मूल्य_decoded = obstack_finish (&muscle_obstack);
  वर्ण *res = xstrdup (मूल्य_decoded);
  obstack_free (&muscle_obstack, मूल्य_decoded);
  प्रत्यागम res;
}

/* Reverse of muscle_location_grow.  */
स्थिर location
location_decode (वर्ण ध्रुव *मूल्य)
{
  aver (मूल्य);
  aver (*मूल्य == '[');
  ++मूल्य; aver (*मूल्य == '[');
  location loc;
  यावत् (*++मूल्य)
    विकल्प (*मूल्य)
      {
        COMMON_DECODE (मूल्य)
        पक्ष '[':
          aver (false);
          भङ्ग;
        पक्ष ']':
          ++मूल्य; aver (*मूल्य == ']');
          वर्ण *boundary_str = obstack_finish0 (&muscle_obstack);
          विकल्प (*++मूल्य)
            {
            पक्ष ',':
              boundary_set_from_string (&loc.start, boundary_str);
              obstack_free (&muscle_obstack, boundary_str);
              ++मूल्य; aver (*मूल्य == ' ');
              ++मूल्य; aver (*मूल्य == '[');
              ++मूल्य; aver (*मूल्य == '[');
              भङ्ग;
            पक्ष '\0':
              boundary_set_from_string (&loc.end, boundary_str);
              obstack_free (&muscle_obstack, boundary_str);
              प्रत्यागम loc;
              भङ्ग;
            स्वभाव:
              aver (false);
              भङ्ग;
            }
          भङ्ग;
      }
  aver (false);
  प्रत्यागम loc;
}

शून्य
muscle_user_name_list_grow (वर्ण ध्रुव *कुंजी, वर्ण ध्रुव *user_name,
                            location loc)
{
  muscle_grow (कुंजी, "[[[[", ",", "");
  muscle_grow (कुंजी, user_name, "", "");
  muscle_grow (कुंजी, "]], ", "", "");
  muscle_location_grow (कुंजी, loc);
  muscle_grow (कुंजी, "]]", "", "");
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

स्थिर
वर्ण *
define_directive (वर्ण ध्रुव *assignment,
                  muscle_kind kind,
                  वर्ण ध्रुव *मूल्य)
{
  वर्ण *eq = strchr (assignment, '=');
  वर्ण ध्रुव *fmt
    = eq || !मूल्य || !*मूल्य ? "%%define %s"
    : kind == muscle_code     ? "%%define %s {%s}"
    : kind == muscle_string   ? "%%define %s \"%s\""
    :                           "%%define %s %s";
  वर्ण *res = xmalloc (strlen (fmt) + strlen (assignment)
                       + (मूल्य ? strlen (मूल्य) : 0));
  sprintf (res, fmt, assignment, मूल्य);
  eq = strchr (res, '=');
  यदि (eq)
    *eq = eq[1] ? ' ' : '\0';
  प्रत्यागम res;
}

/** If the \a variable name is obsolete, return the name to use,
 * otherwise \a variable.  If the \a value is obsolete, update it too.
 *
 * Allocates the returned value if needed, otherwise the returned
 * value is exactly \a variable.  */
स्थिर
वर्ण ध्रुव *
muscle_percent_variable_update (वर्ण ध्रुव *variable,
                                muscle_kind kind,
                                वर्ण ध्रुव **मूल्य,
                                वर्ण **old, वर्ण **upd)
{
  प्रकारनिर्देश संरचना
  {
    ध्रुव वर्ण *obsolete;
    ध्रुव वर्ण *updated;
    muscle_kind kind;
  } conversion_type;
  ध्रुव conversion_type conversion[] =
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
    { रिक्त, रिक्त, -1, }
  };

  प्रति (conversion_type ध्रुव *c = conversion; c->obsolete; ++c)
    {
      वर्ण ध्रुव *eq = strchr (c->obsolete, '=');
      यदि (eq
          ? (!strncmp (c->obsolete, variable, eq - c->obsolete)
             && STREQ (eq + 1, *मूल्य))
          : STREQ (c->obsolete, variable))
        {
          /* Generate the deprecation warning. */
          *old = c->obsolete[0] == '%'
            ? xstrdup (c->obsolete)
            : define_directive (c->obsolete, kind, *मूल्य);
          *upd = define_directive (c->updated, c->kind, *मूल्य);
          /* Update the variable and its value.  */
          {
            वर्ण *res = xstrdup (c->updated);
            वर्ण *eq2 = strchr (res, '=');
            यदि (eq2)
              {
                *eq2 = '\0';
                *मूल्य = eq2 + 1;
              }
            प्रत्यागम res;
          }
        }
    }
  प्रत्यागम variable;
}

शून्य
muscle_percent_define_जोड़ो (वर्ण ध्रुव *var, location variable_loc,
                              muscle_kind kind,
                              वर्ण ध्रुव *मूल्य,
                              muscle_percent_define_how how)
{
  /* Backward compatibility.  */
  वर्ण *old = रिक्त;
  वर्ण *upd = रिक्त;
  वर्ण ध्रुव *variable
    = muscle_percent_variable_update (var, kind,
                                      &मूल्य, &old, &upd);
  uniqstr name = muscle_name (variable, रिक्त);
  uniqstr loc_name = muscle_name (variable, "loc");
  uniqstr syncline_name = muscle_name (variable, "syncline");
  uniqstr how_name = muscle_name (variable, "how");
  uniqstr kind_name = muscle_name (variable, "kind");

  /* Command-line options are processed before the grammar file.  */
  bool warned = false;
  यदि (how == MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE)
    {
      वर्ण ध्रुव *current_मूल्य = muscle_find_const (name);
      यदि (current_मूल्य)
        {
          long l = strtol (muscle_find_const (how_name), रिक्त, 10);
          muscle_percent_define_how how_old
            = 0 <= l && l <= INT_MAX ? l : INT_MAX;
          यदि (how_old == MUSCLE_PERCENT_DEFINE_F)
            गच्छ end;
          /* If assigning the same value, make it a warning.  */
          warnings warn = STREQ (मूल्य, current_मूल्य) ? Wother : complaint;
          complain (&variable_loc, warn,
                    _("%%define variable %s redefined"),
                    quote (variable));
          location loc = muscle_percent_define_get_loc (variable);
          subcomplain (&loc, warn, _("previous definition"));
          fixits_register (&variable_loc, "");
          warned = true;
        }
    }

  यदि (!warned && old && upd)
    deprecated_directive (&variable_loc, old, upd);

  MUSCLE_जोड़ो_STRING (name, मूल्य);
  muscle_जोड़ो (loc_name, "");
  muscle_location_grow (loc_name, variable_loc);
  muscle_जोड़ो (syncline_name, "");
  muscle_syncline_grow (syncline_name, variable_loc);
  muscle_user_name_list_grow ("percent_define_user_variables", variable,
                              variable_loc);
  MUSCLE_जोड़ो_INT (how_name, how);
  MUSCLE_जोड़ो_STRING (kind_name, muscle_kind_string (kind));
 end:
  मुक्त (old);
  मुक्त (upd);
  यदि (variable != var)
    मुक्त ((वर्ण *) variable);
}

/* This is used for backward compatibility, e.g., "%define api.pure"
   supersedes "%pure-parser".  */
शून्य
muscle_percent_define_ensure (वर्ण ध्रुव *variable, location loc,
                              bool मूल्य)
{
  uniqstr name = muscle_name (variable, रिक्त);
  वर्ण ध्रुव *val = मूल्य ? "" : "false";

  /* Don't complain is VARIABLE is already defined, but be sure to set
     its value to VAL.  */
  यदि (!muscle_find_const (name)
      || muscle_percent_define_flag_if (variable) != मूल्य)
    muscle_percent_define_जोड़ो (variable, loc, muscle_keyword, val,
                                  MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
}

/* Mark %define VARIABLE as used.  */
स्थिर शून्य
muscle_percent_define_use (वर्ण ध्रुव *variable)
{
  muscle_जोड़ो (muscle_name (variable, "bison_variables"), "");
}

/* The value of %define variable VARIABLE (corresponding to FIELD, if
   defined).  Do not register as used, but diagnose unset variables.  */

स्थिर
वर्ण ध्रुव *
muscle_percent_define_get_raw (वर्ण ध्रुव *variable, वर्ण ध्रुव *field)
{
  uniqstr name = muscle_name (variable, field);
  वर्ण ध्रुव *res = muscle_find_const (name);
  यदि (!res)
    complain (रिक्त, fatal, _("%s: undefined %%define variable %s"),
              "muscle_percent_define_get_raw", quote (variable));
  प्रत्यागम res;
}

वर्ण *
muscle_percent_define_get (वर्ण ध्रुव *variable)
{
  uniqstr name = muscle_name (variable, रिक्त);
  वर्ण *मूल्य = string_decode (name);
  यदि (!मूल्य)
    मूल्य = xstrdup ("");
  muscle_percent_define_use (variable);
  प्रत्यागम मूल्य;
}

/* The kind of VARIABLE.  An error if undefined.  */
स्थिर muscle_kind
muscle_percent_define_get_kind (वर्ण ध्रुव *variable)
{
  प्रत्यागम muscle_kind_new (muscle_percent_define_get_raw (variable, "kind"));
}

/* Check the kind of VARIABLE.  An error if undefined.  */
स्थिर शून्य
muscle_percent_define_check_kind (वर्ण ध्रुव *variable, muscle_kind kind)
{
  यदि (muscle_percent_define_get_kind (variable) != kind)
    {
      location loc = muscle_percent_define_get_loc (variable);
      विकल्प (kind)
        {
        पक्ष muscle_code:
          complain (&loc, Wdeprecated,
                    _("%%define variable '%s' requires '{...}' values"),
                    variable);
          भङ्ग;
        पक्ष muscle_keyword:
          complain (&loc, Wdeprecated,
                    _("%%define variable '%s' requires keyword values"),
                    variable);
          भङ्ग;
        पक्ष muscle_string:
          complain (&loc, Wdeprecated,
                    _("%%define variable '%s' requires '\"...\"' values"),
                    variable);
          भङ्ग;
        }
    }
}


location
muscle_percent_define_get_loc (वर्ण ध्रुव *variable)
{
  प्रत्यागम location_decode (muscle_percent_define_get_raw (variable, "loc"));
}

वर्ण ध्रुव *
muscle_percent_define_get_syncline (वर्ण ध्रुव *variable)
{
  प्रत्यागम muscle_percent_define_get_raw (variable, "syncline");
}

bool
muscle_percent_define_ifdef (वर्ण ध्रुव *variable)
{
  यदि (muscle_find_const (muscle_name (variable, रिक्त)))
    {
      muscle_percent_define_use (variable);
      प्रत्यागम true;
    }
  अन्यथा
    प्रत्यागम false;
}

bool
muscle_percent_define_flag_if (वर्ण ध्रुव *variable)
{
  uniqstr invalid_boolean_name = muscle_name (variable, "invalid_boolean");
  bool res = false;

  यदि (muscle_percent_define_ifdef (variable))
    {
      वर्ण *मूल्य = muscle_percent_define_get (variable);
      muscle_percent_define_check_kind (variable, muscle_keyword);
      यदि (मूल्य[0] == '\0' || STREQ (मूल्य, "true"))
        res = true;
      अन्यथा यदि (STREQ (मूल्य, "false"))
        res = false;
      अन्यथा यदि (!muscle_find_const (invalid_boolean_name))
        {
          muscle_जोड़ो (invalid_boolean_name, "");
          location loc = muscle_percent_define_get_loc (variable);
          complain (&loc, complaint,
                    _("invalid value for %%define Boolean variable %s"),
                    quote (variable));
        }
      मुक्त (मूल्य);
    }
  अन्यथा
    complain (रिक्त, fatal, _("%s: undefined %%define variable %s"),
              "muscle_percent_define_flag", quote (variable));

  प्रत्यागम res;
}

शून्य
muscle_percent_define_default (वर्ण ध्रुव *variable, वर्ण ध्रुव *मूल्य)
{
  uniqstr name = muscle_name (variable, रिक्त);
  यदि (!muscle_find_const (name))
    {
      MUSCLE_जोड़ो_STRING (name, मूल्य);
      MUSCLE_जोड़ो_STRING (muscle_name (variable, "kind"), "keyword");
      {
        uniqstr loc_name = muscle_name (variable, "loc");
        location loc;
        loc.start.file = "<default value>";
        loc.start.पंक्ति = -1;
        loc.start.column = -1;
        loc.start.byte = -1;
        loc.end = loc.start;
        muscle_जोड़ो (loc_name, "");
        muscle_location_grow (loc_name, loc);
      }
      muscle_जोड़ो (muscle_name (variable, "syncline"), "");
    }
}

शून्य
muscle_percent_define_check_मूल्यावली (वर्ण ध्रुव * ध्रुव *मूल्यावली)
{
  प्रति (; *मूल्यावली; ++मूल्यावली)
    {
      वर्ण ध्रुव * ध्रुव *variablep = मूल्यावली;
      uniqstr name = muscle_name (*variablep, रिक्त);
      वर्ण *मूल्य = string_decode (name);
      muscle_percent_define_check_kind (*variablep, muscle_keyword);
      यदि (मूल्य)
        {
          प्रति (++मूल्यावली; *मूल्यावली; ++मूल्यावली)
            यदि (STREQ (मूल्य, *मूल्यावली))
              भङ्ग;
          यदि (!*मूल्यावली)
            {
              location loc = muscle_percent_define_get_loc (*variablep);
              complain (&loc, complaint,
                        _("invalid value for %%define variable %s: %s"),
                        quote (*variablep), quote_n (1, मूल्य));
              प्रति (मूल्यावली = variablep + 1; *मूल्यावली; ++मूल्यावली)
                subcomplain (&loc, complaint | no_caret | silent,
                             _("accepted value: %s"), quote (*मूल्यावली));
            }
          अन्यथा
            यावत् (*मूल्यावली)
              ++मूल्यावली;
          मुक्त (मूल्य);
        }
      अन्यथा
        complain (रिक्त, fatal, _("%s: undefined %%define variable %s"),
                  "muscle_percent_define_check_values", quote (*variablep));
    }
}

शून्य
muscle_percent_code_grow (वर्ण ध्रुव *qualifier, location qualifier_loc,
                          वर्ण ध्रुव *code, location code_loc)
{
  वर्ण ध्रुव *name = UNIQSTR_CONCAT ("percent_code(", qualifier, ")");
  muscle_code_grow (name, code, code_loc);
  muscle_user_name_list_grow ("percent_code_user_qualifiers", qualifier,
                               qualifier_loc);
}


/*------------------------------------------------.
| Output the definition of ENTRY as a m4_define.  |
`------------------------------------------------*/

स्थिर अन्तःस्थ bool
muscle_m4_output (muscle_entry *entry, FILE *out)
{
  fprintf (out,
           "m4_define([b4_%s],\n"
           "[[%s]])\n\n\n", entry->कुंजी, entry->मूल्य);
  प्रत्यागम true;
}

स्थिर bool
muscle_m4_output_processor (शून्य *entry, शून्य *out)
{
  प्रत्यागम muscle_m4_output (entry, out);
}


शून्य
muscles_m4_output (FILE *out)
{
  hash_do_for_each (muscle_table, muscle_m4_output_processor, out);
}
