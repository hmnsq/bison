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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ "system.h"

#അന്തര്ഭാവ <hash.h>
#അന്തര്ഭാവ <quote.h>

#അന്തര്ഭാവ "complain.h"
#അന്തര്ഭാവ "files.h"
#അന്തര്ഭാവ "fixits.h"
#അന്തര്ഭാവ "getargs.h"
#അന്തര്ഭാവ "muscle-tab.h"

muscle_kind
muscle_kind_new (അക്ഷരം സ്ഥിരം *k)
{
  എങ്കിൽ (STREQ (k, "code"))
    തിരികെ muscle_code;
  അല്ലെങ്കിൽ എങ്കിൽ (STREQ (k, "keyword"))
    തിരികെ muscle_keyword;
  അല്ലെങ്കിൽ എങ്കിൽ (STREQ (k, "string"))
    തിരികെ muscle_string;
  abort ();
}

അക്ഷരം സ്ഥിരം *
muscle_kind_string (muscle_kind k)
{
  തിരഞ്ഞെടുപ്പ് (k)
    {
    അവസ്ഥ muscle_code:    തിരികെ "code";
    അവസ്ഥ muscle_keyword: തിരികെ "keyword";
    അവസ്ഥ muscle_string:  തിരികെ "string";
    }
  abort ();
}


/* A key-value pair, along with storage that can be reclaimed when
   this pair is no longer needed.  */
തരനിർവചനം ഘടന
{
  അക്ഷരം സ്ഥിരം *താക്കോൽ;
  അക്ഷരം സ്ഥിരം *മൂല്യം;
  അക്ഷരം *storage;
  muscle_kind kind;
} muscle_entry;


/* The name of muscle for the %define variable VAR (corresponding to
   FIELD, if defined).  */
സ്ഥിതം uniqstr
muscle_name (അക്ഷരം സ്ഥിരം *var, അക്ഷരം സ്ഥിരം *field)
{
  എങ്കിൽ (field)
    തിരികെ UNIQSTR_CONCAT ("percent_define_", field, "(", var, ")");
  അല്ലെങ്കിൽ
    തിരികെ UNIQSTR_CONCAT ("percent_define(", var, ")");
}

/* An obstack used to create some entries.  */
ഘടന obstack muscle_obstack;

/* Initial capacity of muscles hash table.  */
#define HT_INITIAL_CAPACITY 257

സ്ഥിതം ഘടന hash_table *muscle_table = ശൂന്യ;

സ്ഥിതം bool
hash_compare_muscles (ശൂന്യം സ്ഥിരം *x, ശൂന്യം സ്ഥിരം *y)
{
  muscle_entry സ്ഥിരം *m1 = x;
  muscle_entry സ്ഥിരം *m2 = y;
  തിരികെ STREQ (m1->താക്കോൽ, m2->താക്കോൽ);
}

സ്ഥിതം size_t
hash_muscle (സ്ഥിരം ശൂന്യം *x, size_t tablesize)
{
  muscle_entry സ്ഥിരം *m = x;
  തിരികെ hash_string (m->താക്കോൽ, tablesize);
}

/* Create a fresh muscle name KEY, and insert in the hash table.  */
സ്ഥിതം ശൂന്യം *
muscle_entry_new (അക്ഷരം സ്ഥിരം *താക്കോൽ)
{
  muscle_entry *res = xmalloc (വലുപ്പം *res);
  res->താക്കോൽ = താക്കോൽ;
  res->മൂല്യം = ശൂന്യ;
  res->storage = ശൂന്യ;
  hash_xinsert (muscle_table, res);
  തിരികെ res;
}

സ്ഥിതം ശൂന്യം
muscle_entry_free (ശൂന്യം *entry)
{
  muscle_entry *mentry = entry;
  സ്വതന്ത്രം (mentry->storage);
  സ്വതന്ത്രം (mentry);
}

ശൂന്യം
muscle_init (ശൂന്യം)
{
  /* Initialize the muscle obstack.  */
  obstack_init (&muscle_obstack);

  muscle_table = hash_xinitialize (HT_INITIAL_CAPACITY, ശൂന്യ, hash_muscle,
                                   hash_compare_muscles, muscle_entry_free);
  /* Avoid warnings if the user defined this variable, but did not
     actually call -Wcex.  */
  സ്വതന്ത്രം (muscle_percent_define_get ("cex.timeout"));
}


ശൂന്യം
muscle_free (ശൂന്യം)
{
  hash_free (muscle_table);
  obstack_free (&muscle_obstack, ശൂന്യ);
}

/* Look for the muscle named KEY.  Return NULL if does not exist.  */
സ്ഥിതം muscle_entry *
muscle_lookup (അക്ഷരം സ്ഥിരം *താക്കോൽ)
{
  muscle_entry probe;
  probe.താക്കോൽ = താക്കോൽ;
  തിരികെ hash_lookup (muscle_table, &probe);
}


ശൂന്യം
muscle_തിരുകുക (അക്ഷരം സ്ഥിരം *താക്കോൽ, അക്ഷരം സ്ഥിരം *മൂല്യം)
{
  muscle_entry *entry = muscle_lookup (താക്കോൽ);
  എങ്കിൽ (entry)
    സ്വതന്ത്രം (entry->storage);
  അല്ലെങ്കിൽ
    /* First insertion in the hash. */
    entry = muscle_entry_new (താക്കോൽ);
  entry->മൂല്യം = മൂല്യം;
  entry->storage = ശൂന്യ;
}


/* Append VALUE to the current value of KEY.  If KEY did not already
   exist, create it.  Use MUSCLE_OBSTACK.  De-allocate the previously
   associated value.  Copy VALUE and SEPARATOR.  If VALUE does not end
   with TERMINATOR, append one.  */

സ്ഥിതം ശൂന്യം
muscle_grow (സ്ഥിരം അക്ഷരം *താക്കോൽ, സ്ഥിരം അക്ഷരം *val,
             സ്ഥിരം അക്ഷരം *separator, സ്ഥിരം അക്ഷരം *terminator)
{
  muscle_entry *entry = muscle_lookup (താക്കോൽ);
  എങ്കിൽ (entry)
    {
      obstack_sgrow (&muscle_obstack, entry->മൂല്യം);
      obstack_sgrow (&muscle_obstack, separator);
      സ്വതന്ത്രം (entry->storage);
    }
  അല്ലെങ്കിൽ
    entry = muscle_entry_new (താക്കോൽ);

  obstack_sgrow (&muscle_obstack, val);

  size_t vals = strlen (val);
  size_t terms = strlen (terminator);
  എങ്കിൽ (terms <= vals
      && STRNEQ (val + vals - terms, terminator))
    obstack_sgrow (&muscle_obstack, terminator);

  {
    അക്ഷരം സ്ഥിരം *new_val = obstack_finish0 (&muscle_obstack);
    entry->മൂല്യം = entry->storage = xstrdup (new_val);
    obstack_free (&muscle_obstack, new_val);
  }
}

/*------------------------------------------------------------------.
| Using muscle_grow, append a synchronization line for the location |
| LOC to the current value of KEY.                                  |
`------------------------------------------------------------------*/

സ്ഥിതം ശൂന്യം
muscle_syncline_grow (അക്ഷരം സ്ഥിരം *താക്കോൽ, location loc)
{
  obstack_printf (&muscle_obstack, "]b4_syncline(%d, ", loc.start.വരി);
  obstack_quote (&muscle_obstack,
                 quotearg_style (c_quoting_style, map_file_name (loc.start.file)));
  obstack_sgrow (&muscle_obstack, ")dnl\n[");
  അക്ഷരം സ്ഥിരം *extension = obstack_finish0 (&muscle_obstack);
  muscle_grow (താക്കോൽ, extension, "", "");
  obstack_free (&muscle_obstack, extension);
}

/*------------------------------------------------------------------.
| Append VALUE to the current value of KEY, using muscle_grow.  But |
| in addition, issue a synchronization line for the location LOC    |
| using muscle_syncline_grow.                                       |
`------------------------------------------------------------------*/

ശൂന്യം
muscle_code_grow (സ്ഥിരം അക്ഷരം *താക്കോൽ, സ്ഥിരം അക്ഷരം *val, location loc)
{
  muscle_syncline_grow (താക്കോൽ, loc);
  muscle_grow (താക്കോൽ, val, "", "\n");
}


ശൂന്യം
muscle_pair_list_grow (സ്ഥിരം അക്ഷരം *muscle,
                       സ്ഥിരം അക്ഷരം *a1, സ്ഥിരം അക്ഷരം *a2)
{
  obstack_sgrow (&muscle_obstack, "[");
  obstack_quote (&muscle_obstack, a1);
  obstack_sgrow (&muscle_obstack, ", ");
  obstack_quote (&muscle_obstack, a2);
  obstack_sgrow (&muscle_obstack, "]");
  അക്ഷരം സ്ഥിരം *pair = obstack_finish0 (&muscle_obstack);
  muscle_grow (muscle, pair, ",\n", "");
  obstack_free (&muscle_obstack, pair);
}


അക്ഷരം സ്ഥിരം *
muscle_find_const (അക്ഷരം സ്ഥിരം *താക്കോൽ)
{
  muscle_entry *entry = muscle_lookup (താക്കോൽ);
  തിരികെ entry ? entry->മൂല്യം : ശൂന്യ;
}


അക്ഷരം *
muscle_find (അക്ഷരം സ്ഥിരം *താക്കോൽ)
{
  muscle_entry *entry = muscle_lookup (താക്കോൽ);
  എങ്കിൽ (entry)
    {
      aver (entry->മൂല്യം == entry->storage);
      തിരികെ entry->storage;
    }
  തിരികെ ശൂന്യ;
}


/* In the format 'file_name:line.column', append BOUND to MUSCLE.  Use
   digraphs for special characters in the file name.  */

സ്ഥിതം ശൂന്യം
muscle_boundary_grow (അക്ഷരം സ്ഥിരം *താക്കോൽ, boundary bound)
{
  obstack_sgrow  (&muscle_obstack, "[[");
  obstack_escape (&muscle_obstack, bound.file);
  obstack_printf (&muscle_obstack, ":%d.%d@@%d]]", bound.വരി, bound.column, bound.byte);
  അക്ഷരം സ്ഥിരം *extension = obstack_finish0 (&muscle_obstack);
  muscle_grow (താക്കോൽ, extension, "", "");
  obstack_free (&muscle_obstack, extension);
}


ശൂന്യം
muscle_location_grow (അക്ഷരം സ്ഥിരം *താക്കോൽ, location loc)
{
  muscle_boundary_grow (താക്കോൽ, loc.start);
  muscle_grow (താക്കോൽ, "", ", ", "");
  muscle_boundary_grow (താക്കോൽ, loc.end);
}

#define COMMON_DECODE(മൂല്യം_4)                                    \
  അവസ്ഥ '$':                                                     \
    ++(മൂല്യം_4); aver (*(മൂല്യം_4) == ']');                          \
    ++(മൂല്യം_4); aver (*(മൂല്യം_4) == '[');                          \
    obstack_sgrow (&muscle_obstack, "$");                       \
    നിർത്തുക;                                                      \
  അവസ്ഥ '@':                                                     \
    തിരഞ്ഞെടുപ്പ് (*++(മൂല്യം_4))                                         \
      {                                                         \
      അവസ്ഥ '\'': /* Ignore "@'" */ നിർത്തുക;                       \
      അവസ്ഥ '@': obstack_sgrow (&muscle_obstack, "@" ); നിർത്തുക;   \
      അവസ്ഥ '{': obstack_sgrow (&muscle_obstack, "[" ); നിർത്തുക;   \
      അവസ്ഥ '}': obstack_sgrow (&muscle_obstack, "]" ); നിർത്തുക;   \
      സ്ഥിരസ്ഥിതി: aver (false); നിർത്തുക;                             \
      }                                                         \
    നിർത്തുക;                                                      \
  സ്ഥിരസ്ഥിതി:                                                      \
    obstack_1grow (&muscle_obstack, *(മൂല്യം_4));                  \
    നിർത്തുക;

/* Reverse of obstack_escape.  */
സ്ഥിതം അക്ഷരം *
string_decode (അക്ഷരം സ്ഥിരം *താക്കോൽ)
{
  അക്ഷരം സ്ഥിരം *മൂല്യം = muscle_find_const (താക്കോൽ);
  എങ്കിൽ (!മൂല്യം)
    തിരികെ ശൂന്യ;
  ചെയ്യുക {
    തിരഞ്ഞെടുപ്പ് (*മൂല്യം)
      {
        COMMON_DECODE (മൂല്യം)
        അവസ്ഥ '[':
        അവസ്ഥ ']':
          aver (false);
          നിർത്തുക;
      }
  } ആകുമ്പോൾ (*മൂല്യം++);
  അക്ഷരം സ്ഥിരം *മൂല്യം_decoded = obstack_finish (&muscle_obstack);
  അക്ഷരം *res = xstrdup (മൂല്യം_decoded);
  obstack_free (&muscle_obstack, മൂല്യം_decoded);
  തിരികെ res;
}

/* Reverse of muscle_location_grow.  */
സ്ഥിതം location
location_decode (അക്ഷരം സ്ഥിരം *മൂല്യം)
{
  aver (മൂല്യം);
  aver (*മൂല്യം == '[');
  ++മൂല്യം; aver (*മൂല്യം == '[');
  location loc;
  ആകുമ്പോൾ (*++മൂല്യം)
    തിരഞ്ഞെടുപ്പ് (*മൂല്യം)
      {
        COMMON_DECODE (മൂല്യം)
        അവസ്ഥ '[':
          aver (false);
          നിർത്തുക;
        അവസ്ഥ ']':
          ++മൂല്യം; aver (*മൂല്യം == ']');
          അക്ഷരം *boundary_str = obstack_finish0 (&muscle_obstack);
          തിരഞ്ഞെടുപ്പ് (*++മൂല്യം)
            {
            അവസ്ഥ ',':
              boundary_set_from_string (&loc.start, boundary_str);
              obstack_free (&muscle_obstack, boundary_str);
              ++മൂല്യം; aver (*മൂല്യം == ' ');
              ++മൂല്യം; aver (*മൂല്യം == '[');
              ++മൂല്യം; aver (*മൂല്യം == '[');
              നിർത്തുക;
            അവസ്ഥ '\0':
              boundary_set_from_string (&loc.end, boundary_str);
              obstack_free (&muscle_obstack, boundary_str);
              തിരികെ loc;
              നിർത്തുക;
            സ്ഥിരസ്ഥിതി:
              aver (false);
              നിർത്തുക;
            }
          നിർത്തുക;
      }
  aver (false);
  തിരികെ loc;
}

ശൂന്യം
muscle_user_name_list_grow (അക്ഷരം സ്ഥിരം *താക്കോൽ, അക്ഷരം സ്ഥിരം *user_name,
                            location loc)
{
  muscle_grow (താക്കോൽ, "[[[[", ",", "");
  muscle_grow (താക്കോൽ, user_name, "", "");
  muscle_grow (താക്കോൽ, "]], ", "", "");
  muscle_location_grow (താക്കോൽ, loc);
  muscle_grow (താക്കോൽ, "]]", "", "");
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

സ്ഥിതം
അക്ഷരം *
define_directive (അക്ഷരം സ്ഥിരം *assignment,
                  muscle_kind kind,
                  അക്ഷരം സ്ഥിരം *മൂല്യം)
{
  അക്ഷരം *eq = strchr (assignment, '=');
  അക്ഷരം സ്ഥിരം *fmt
    = eq || !മൂല്യം || !*മൂല്യം ? "%%define %s"
    : kind == muscle_code     ? "%%define %s {%s}"
    : kind == muscle_string   ? "%%define %s \"%s\""
    :                           "%%define %s %s";
  അക്ഷരം *res = xmalloc (strlen (fmt) + strlen (assignment)
                       + (മൂല്യം ? strlen (മൂല്യം) : 0));
  sprintf (res, fmt, assignment, മൂല്യം);
  eq = strchr (res, '=');
  എങ്കിൽ (eq)
    *eq = eq[1] ? ' ' : '\0';
  തിരികെ res;
}

/** If the \a variable name is obsolete, return the name to use,
 * otherwise \a variable.  If the \a value is obsolete, update it too.
 *
 * Allocates the returned value if needed, otherwise the returned
 * value is exactly \a variable.  */
സ്ഥിതം
അക്ഷരം സ്ഥിരം *
muscle_percent_variable_update (അക്ഷരം സ്ഥിരം *variable,
                                muscle_kind kind,
                                അക്ഷരം സ്ഥിരം **മൂല്യം,
                                അക്ഷരം **old, അക്ഷരം **upd)
{
  തരനിർവചനം ഘടന
  {
    സ്ഥിരം അക്ഷരം *obsolete;
    സ്ഥിരം അക്ഷരം *updated;
    muscle_kind kind;
  } conversion_type;
  സ്ഥിരം conversion_type conversion[] =
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
    { ശൂന്യ, ശൂന്യ, -1, }
  };

  വേണ്ടി (conversion_type സ്ഥിരം *c = conversion; c->obsolete; ++c)
    {
      അക്ഷരം സ്ഥിരം *eq = strchr (c->obsolete, '=');
      എങ്കിൽ (eq
          ? (!strncmp (c->obsolete, variable, eq - c->obsolete)
             && STREQ (eq + 1, *മൂല്യം))
          : STREQ (c->obsolete, variable))
        {
          /* Generate the deprecation warning. */
          *old = c->obsolete[0] == '%'
            ? xstrdup (c->obsolete)
            : define_directive (c->obsolete, kind, *മൂല്യം);
          *upd = define_directive (c->updated, c->kind, *മൂല്യം);
          /* Update the variable and its value.  */
          {
            അക്ഷരം *res = xstrdup (c->updated);
            അക്ഷരം *eq2 = strchr (res, '=');
            എങ്കിൽ (eq2)
              {
                *eq2 = '\0';
                *മൂല്യം = eq2 + 1;
              }
            തിരികെ res;
          }
        }
    }
  തിരികെ variable;
}

ശൂന്യം
muscle_percent_define_തിരുകുക (അക്ഷരം സ്ഥിരം *var, location variable_loc,
                              muscle_kind kind,
                              അക്ഷരം സ്ഥിരം *മൂല്യം,
                              muscle_percent_define_how how)
{
  /* Backward compatibility.  */
  അക്ഷരം *old = ശൂന്യ;
  അക്ഷരം *upd = ശൂന്യ;
  അക്ഷരം സ്ഥിരം *variable
    = muscle_percent_variable_update (var, kind,
                                      &മൂല്യം, &old, &upd);
  uniqstr name = muscle_name (variable, ശൂന്യ);
  uniqstr loc_name = muscle_name (variable, "loc");
  uniqstr syncline_name = muscle_name (variable, "syncline");
  uniqstr how_name = muscle_name (variable, "how");
  uniqstr kind_name = muscle_name (variable, "kind");

  /* Command-line options are processed before the grammar file.  */
  bool warned = false;
  എങ്കിൽ (how == MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE)
    {
      അക്ഷരം സ്ഥിരം *current_മൂല്യം = muscle_find_const (name);
      എങ്കിൽ (current_മൂല്യം)
        {
          long l = strtol (muscle_find_const (how_name), ശൂന്യ, 10);
          muscle_percent_define_how how_old
            = 0 <= l && l <= INT_MAX ? l : INT_MAX;
          എങ്കിൽ (how_old == MUSCLE_PERCENT_DEFINE_F)
            പോകുക end;
          /* If assigning the same value, make it a warning.  */
          warnings warn = STREQ (മൂല്യം, current_മൂല്യം) ? Wother : complaint;
          complain (&variable_loc, warn,
                    _("%%define variable %s redefined"),
                    quote (variable));
          location loc = muscle_percent_define_get_loc (variable);
          subcomplain (&loc, warn, _("previous definition"));
          fixits_register (&variable_loc, "");
          warned = true;
        }
    }

  എങ്കിൽ (!warned && old && upd)
    deprecated_directive (&variable_loc, old, upd);

  MUSCLE_തിരുകുക_STRING (name, മൂല്യം);
  muscle_തിരുകുക (loc_name, "");
  muscle_location_grow (loc_name, variable_loc);
  muscle_തിരുകുക (syncline_name, "");
  muscle_syncline_grow (syncline_name, variable_loc);
  muscle_user_name_list_grow ("percent_define_user_variables", variable,
                              variable_loc);
  MUSCLE_തിരുകുക_INT (how_name, how);
  MUSCLE_തിരുകുക_STRING (kind_name, muscle_kind_string (kind));
 end:
  സ്വതന്ത്രം (old);
  സ്വതന്ത്രം (upd);
  എങ്കിൽ (variable != var)
    സ്വതന്ത്രം ((അക്ഷരം *) variable);
}

/* This is used for backward compatibility, e.g., "%define api.pure"
   supersedes "%pure-parser".  */
ശൂന്യം
muscle_percent_define_ensure (അക്ഷരം സ്ഥിരം *variable, location loc,
                              bool മൂല്യം)
{
  uniqstr name = muscle_name (variable, ശൂന്യ);
  അക്ഷരം സ്ഥിരം *val = മൂല്യം ? "" : "false";

  /* Don't complain is VARIABLE is already defined, but be sure to set
     its value to VAL.  */
  എങ്കിൽ (!muscle_find_const (name)
      || muscle_percent_define_flag_if (variable) != മൂല്യം)
    muscle_percent_define_തിരുകുക (variable, loc, muscle_keyword, val,
                                  MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
}

/* Mark %define VARIABLE as used.  */
സ്ഥിതം ശൂന്യം
muscle_percent_define_use (അക്ഷരം സ്ഥിരം *variable)
{
  muscle_തിരുകുക (muscle_name (variable, "bison_variables"), "");
}

/* The value of %define variable VARIABLE (corresponding to FIELD, if
   defined).  Do not register as used, but diagnose unset variables.  */

സ്ഥിതം
അക്ഷരം സ്ഥിരം *
muscle_percent_define_get_raw (അക്ഷരം സ്ഥിരം *variable, അക്ഷരം സ്ഥിരം *field)
{
  uniqstr name = muscle_name (variable, field);
  അക്ഷരം സ്ഥിരം *res = muscle_find_const (name);
  എങ്കിൽ (!res)
    complain (ശൂന്യ, fatal, _("%s: undefined %%define variable %s"),
              "muscle_percent_define_get_raw", quote (variable));
  തിരികെ res;
}

അക്ഷരം *
muscle_percent_define_get (അക്ഷരം സ്ഥിരം *variable)
{
  uniqstr name = muscle_name (variable, ശൂന്യ);
  അക്ഷരം *മൂല്യം = string_decode (name);
  എങ്കിൽ (!മൂല്യം)
    മൂല്യം = xstrdup ("");
  muscle_percent_define_use (variable);
  തിരികെ മൂല്യം;
}

/* The kind of VARIABLE.  An error if undefined.  */
സ്ഥിതം muscle_kind
muscle_percent_define_get_kind (അക്ഷരം സ്ഥിരം *variable)
{
  തിരികെ muscle_kind_new (muscle_percent_define_get_raw (variable, "kind"));
}

/* Check the kind of VARIABLE.  An error if undefined.  */
സ്ഥിതം ശൂന്യം
muscle_percent_define_check_kind (അക്ഷരം സ്ഥിരം *variable, muscle_kind kind)
{
  എങ്കിൽ (muscle_percent_define_get_kind (variable) != kind)
    {
      location loc = muscle_percent_define_get_loc (variable);
      തിരഞ്ഞെടുപ്പ് (kind)
        {
        അവസ്ഥ muscle_code:
          complain (&loc, Wdeprecated,
                    _("%%define variable '%s' requires '{...}' values"),
                    variable);
          നിർത്തുക;
        അവസ്ഥ muscle_keyword:
          complain (&loc, Wdeprecated,
                    _("%%define variable '%s' requires keyword values"),
                    variable);
          നിർത്തുക;
        അവസ്ഥ muscle_string:
          complain (&loc, Wdeprecated,
                    _("%%define variable '%s' requires '\"...\"' values"),
                    variable);
          നിർത്തുക;
        }
    }
}


location
muscle_percent_define_get_loc (അക്ഷരം സ്ഥിരം *variable)
{
  തിരികെ location_decode (muscle_percent_define_get_raw (variable, "loc"));
}

അക്ഷരം സ്ഥിരം *
muscle_percent_define_get_syncline (അക്ഷരം സ്ഥിരം *variable)
{
  തിരികെ muscle_percent_define_get_raw (variable, "syncline");
}

bool
muscle_percent_define_ifdef (അക്ഷരം സ്ഥിരം *variable)
{
  എങ്കിൽ (muscle_find_const (muscle_name (variable, ശൂന്യ)))
    {
      muscle_percent_define_use (variable);
      തിരികെ true;
    }
  അല്ലെങ്കിൽ
    തിരികെ false;
}

bool
muscle_percent_define_flag_if (അക്ഷരം സ്ഥിരം *variable)
{
  uniqstr invalid_boolean_name = muscle_name (variable, "invalid_boolean");
  bool res = false;

  എങ്കിൽ (muscle_percent_define_ifdef (variable))
    {
      അക്ഷരം *മൂല്യം = muscle_percent_define_get (variable);
      muscle_percent_define_check_kind (variable, muscle_keyword);
      എങ്കിൽ (മൂല്യം[0] == '\0' || STREQ (മൂല്യം, "true"))
        res = true;
      അല്ലെങ്കിൽ എങ്കിൽ (STREQ (മൂല്യം, "false"))
        res = false;
      അല്ലെങ്കിൽ എങ്കിൽ (!muscle_find_const (invalid_boolean_name))
        {
          muscle_തിരുകുക (invalid_boolean_name, "");
          location loc = muscle_percent_define_get_loc (variable);
          complain (&loc, complaint,
                    _("invalid value for %%define Boolean variable %s"),
                    quote (variable));
        }
      സ്വതന്ത്രം (മൂല്യം);
    }
  അല്ലെങ്കിൽ
    complain (ശൂന്യ, fatal, _("%s: undefined %%define variable %s"),
              "muscle_percent_define_flag", quote (variable));

  തിരികെ res;
}

ശൂന്യം
muscle_percent_define_default (അക്ഷരം സ്ഥിരം *variable, അക്ഷരം സ്ഥിരം *മൂല്യം)
{
  uniqstr name = muscle_name (variable, ശൂന്യ);
  എങ്കിൽ (!muscle_find_const (name))
    {
      MUSCLE_തിരുകുക_STRING (name, മൂല്യം);
      MUSCLE_തിരുകുക_STRING (muscle_name (variable, "kind"), "keyword");
      {
        uniqstr loc_name = muscle_name (variable, "loc");
        location loc;
        loc.start.file = "<default value>";
        loc.start.വരി = -1;
        loc.start.column = -1;
        loc.start.byte = -1;
        loc.end = loc.start;
        muscle_തിരുകുക (loc_name, "");
        muscle_location_grow (loc_name, loc);
      }
      muscle_തിരുകുക (muscle_name (variable, "syncline"), "");
    }
}

ശൂന്യം
muscle_percent_define_check_മൂല്യങ്ങൾ (അക്ഷരം സ്ഥിരം * സ്ഥിരം *മൂല്യങ്ങൾ)
{
  വേണ്ടി (; *മൂല്യങ്ങൾ; ++മൂല്യങ്ങൾ)
    {
      അക്ഷരം സ്ഥിരം * സ്ഥിരം *variablep = മൂല്യങ്ങൾ;
      uniqstr name = muscle_name (*variablep, ശൂന്യ);
      അക്ഷരം *മൂല്യം = string_decode (name);
      muscle_percent_define_check_kind (*variablep, muscle_keyword);
      എങ്കിൽ (മൂല്യം)
        {
          വേണ്ടി (++മൂല്യങ്ങൾ; *മൂല്യങ്ങൾ; ++മൂല്യങ്ങൾ)
            എങ്കിൽ (STREQ (മൂല്യം, *മൂല്യങ്ങൾ))
              നിർത്തുക;
          എങ്കിൽ (!*മൂല്യങ്ങൾ)
            {
              location loc = muscle_percent_define_get_loc (*variablep);
              complain (&loc, complaint,
                        _("invalid value for %%define variable %s: %s"),
                        quote (*variablep), quote_n (1, മൂല്യം));
              വേണ്ടി (മൂല്യങ്ങൾ = variablep + 1; *മൂല്യങ്ങൾ; ++മൂല്യങ്ങൾ)
                subcomplain (&loc, complaint | no_caret | silent,
                             _("accepted value: %s"), quote (*മൂല്യങ്ങൾ));
            }
          അല്ലെങ്കിൽ
            ആകുമ്പോൾ (*മൂല്യങ്ങൾ)
              ++മൂല്യങ്ങൾ;
          സ്വതന്ത്രം (മൂല്യം);
        }
      അല്ലെങ്കിൽ
        complain (ശൂന്യ, fatal, _("%s: undefined %%define variable %s"),
                  "muscle_percent_define_check_values", quote (*variablep));
    }
}

ശൂന്യം
muscle_percent_code_grow (അക്ഷരം സ്ഥിരം *qualifier, location qualifier_loc,
                          അക്ഷരം സ്ഥിരം *code, location code_loc)
{
  അക്ഷരം സ്ഥിരം *name = UNIQSTR_CONCAT ("percent_code(", qualifier, ")");
  muscle_code_grow (name, code, code_loc);
  muscle_user_name_list_grow ("percent_code_user_qualifiers", qualifier,
                               qualifier_loc);
}


/*------------------------------------------------.
| Output the definition of ENTRY as a m4_define.  |
`------------------------------------------------*/

സ്ഥിതം വരിയിൽ bool
muscle_m4_output (muscle_entry *entry, FILE *out)
{
  fprintf (out,
           "m4_define([b4_%s],\n"
           "[[%s]])\n\n\n", entry->താക്കോൽ, entry->മൂല്യം);
  തിരികെ true;
}

സ്ഥിതം bool
muscle_m4_output_processor (ശൂന്യം *entry, ശൂന്യം *out)
{
  തിരികെ muscle_m4_output (entry, out);
}


ശൂന്യം
muscles_m4_output (FILE *out)
{
  hash_do_for_each (muscle_table, muscle_m4_output_processor, out);
}
