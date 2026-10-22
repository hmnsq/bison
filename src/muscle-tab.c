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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ "system.h"

#அந்தர்பாவ <hash.h>
#அந்தர்பாவ <quote.h>

#அந்தர்பாவ "complain.h"
#அந்தர்பாவ "files.h"
#அந்தர்பாவ "fixits.h"
#அந்தர்பாவ "getargs.h"
#அந்தர்பாவ "muscle-tab.h"

muscle_kind
muscle_kind_new (எழுத்து மாறிலி *k)
{
  எனில் (STREQ (k, "code"))
    திரும்பு muscle_code;
  இல்லையெனில் எனில் (STREQ (k, "keyword"))
    திரும்பு muscle_keyword;
  இல்லையெனில் எனில் (STREQ (k, "string"))
    திரும்பு muscle_string;
  abort ();
}

எழுத்து மாறிலி *
muscle_kind_string (muscle_kind k)
{
  தேர்வு (k)
    {
    நிலை muscle_code:    திரும்பு "code";
    நிலை muscle_keyword: திரும்பு "keyword";
    நிலை muscle_string:  திரும்பு "string";
    }
  abort ();
}


/* A key-value pair, along with storage that can be reclaimed when
   this pair is no longer needed.  */
வகைவரை கட்டமைப்பு
{
  எழுத்து மாறிலி *சாவி;
  எழுத்து மாறிலி *மதிப்பு;
  எழுத்து *storage;
  muscle_kind kind;
} muscle_entry;


/* The name of muscle for the %define variable VAR (corresponding to
   FIELD, if defined).  */
நிலையான uniqstr
muscle_name (எழுத்து மாறிலி *var, எழுத்து மாறிலி *field)
{
  எனில் (field)
    திரும்பு UNIQSTR_CONCAT ("percent_define_", field, "(", var, ")");
  இல்லையெனில்
    திரும்பு UNIQSTR_CONCAT ("percent_define(", var, ")");
}

/* An obstack used to create some entries.  */
கட்டமைப்பு obstack muscle_obstack;

/* Initial capacity of muscles hash table.  */
#define HT_INITIAL_CAPACITY 257

நிலையான கட்டமைப்பு hash_table *muscle_table = வெற்று;

நிலையான bool
hash_compare_muscles (வெறுமை மாறிலி *x, வெறுமை மாறிலி *y)
{
  muscle_entry மாறிலி *m1 = x;
  muscle_entry மாறிலி *m2 = y;
  திரும்பு STREQ (m1->சாவி, m2->சாவி);
}

நிலையான size_t
hash_muscle (மாறிலி வெறுமை *x, size_t tablesize)
{
  muscle_entry மாறிலி *m = x;
  திரும்பு hash_string (m->சாவி, tablesize);
}

/* Create a fresh muscle name KEY, and insert in the hash table.  */
நிலையான வெறுமை *
muscle_entry_new (எழுத்து மாறிலி *சாவி)
{
  muscle_entry *res = xmalloc (அளவு *res);
  res->சாவி = சாவி;
  res->மதிப்பு = வெற்று;
  res->storage = வெற்று;
  hash_xinsert (muscle_table, res);
  திரும்பு res;
}

நிலையான வெறுமை
muscle_entry_free (வெறுமை *entry)
{
  muscle_entry *mentry = entry;
  விடுவி (mentry->storage);
  விடுவி (mentry);
}

வெறுமை
muscle_init (வெறுமை)
{
  /* Initialize the muscle obstack.  */
  obstack_init (&muscle_obstack);

  muscle_table = hash_xinitialize (HT_INITIAL_CAPACITY, வெற்று, hash_muscle,
                                   hash_compare_muscles, muscle_entry_free);
  /* Avoid warnings if the user defined this variable, but did not
     actually call -Wcex.  */
  விடுவி (muscle_percent_define_get ("cex.timeout"));
}


வெறுமை
muscle_free (வெறுமை)
{
  hash_free (muscle_table);
  obstack_free (&muscle_obstack, வெற்று);
}

/* Look for the muscle named KEY.  Return NULL if does not exist.  */
நிலையான muscle_entry *
muscle_lookup (எழுத்து மாறிலி *சாவி)
{
  muscle_entry probe;
  probe.சாவி = சாவி;
  திரும்பு hash_lookup (muscle_table, &probe);
}


வெறுமை
muscle_செருகு (எழுத்து மாறிலி *சாவி, எழுத்து மாறிலி *மதிப்பு)
{
  muscle_entry *entry = muscle_lookup (சாவி);
  எனில் (entry)
    விடுவி (entry->storage);
  இல்லையெனில்
    /* First insertion in the hash. */
    entry = muscle_entry_new (சாவி);
  entry->மதிப்பு = மதிப்பு;
  entry->storage = வெற்று;
}


/* Append VALUE to the current value of KEY.  If KEY did not already
   exist, create it.  Use MUSCLE_OBSTACK.  De-allocate the previously
   associated value.  Copy VALUE and SEPARATOR.  If VALUE does not end
   with TERMINATOR, append one.  */

நிலையான வெறுமை
muscle_grow (மாறிலி எழுத்து *சாவி, மாறிலி எழுத்து *val,
             மாறிலி எழுத்து *separator, மாறிலி எழுத்து *terminator)
{
  muscle_entry *entry = muscle_lookup (சாவி);
  எனில் (entry)
    {
      obstack_sgrow (&muscle_obstack, entry->மதிப்பு);
      obstack_sgrow (&muscle_obstack, separator);
      விடுவி (entry->storage);
    }
  இல்லையெனில்
    entry = muscle_entry_new (சாவி);

  obstack_sgrow (&muscle_obstack, val);

  size_t vals = strlen (val);
  size_t terms = strlen (terminator);
  எனில் (terms <= vals
      && STRNEQ (val + vals - terms, terminator))
    obstack_sgrow (&muscle_obstack, terminator);

  {
    எழுத்து மாறிலி *new_val = obstack_finish0 (&muscle_obstack);
    entry->மதிப்பு = entry->storage = xstrdup (new_val);
    obstack_free (&muscle_obstack, new_val);
  }
}

/*------------------------------------------------------------------.
| Using muscle_grow, append a synchronization line for the location |
| LOC to the current value of KEY.                                  |
`------------------------------------------------------------------*/

நிலையான வெறுமை
muscle_syncline_grow (எழுத்து மாறிலி *சாவி, location loc)
{
  obstack_printf (&muscle_obstack, "]b4_syncline(%d, ", loc.start.வரி);
  obstack_quote (&muscle_obstack,
                 quotearg_style (c_quoting_style, map_file_name (loc.start.file)));
  obstack_sgrow (&muscle_obstack, ")dnl\n[");
  எழுத்து மாறிலி *extension = obstack_finish0 (&muscle_obstack);
  muscle_grow (சாவி, extension, "", "");
  obstack_free (&muscle_obstack, extension);
}

/*------------------------------------------------------------------.
| Append VALUE to the current value of KEY, using muscle_grow.  But |
| in addition, issue a synchronization line for the location LOC    |
| using muscle_syncline_grow.                                       |
`------------------------------------------------------------------*/

வெறுமை
muscle_code_grow (மாறிலி எழுத்து *சாவி, மாறிலி எழுத்து *val, location loc)
{
  muscle_syncline_grow (சாவி, loc);
  muscle_grow (சாவி, val, "", "\n");
}


வெறுமை
muscle_pair_list_grow (மாறிலி எழுத்து *muscle,
                       மாறிலி எழுத்து *a1, மாறிலி எழுத்து *a2)
{
  obstack_sgrow (&muscle_obstack, "[");
  obstack_quote (&muscle_obstack, a1);
  obstack_sgrow (&muscle_obstack, ", ");
  obstack_quote (&muscle_obstack, a2);
  obstack_sgrow (&muscle_obstack, "]");
  எழுத்து மாறிலி *pair = obstack_finish0 (&muscle_obstack);
  muscle_grow (muscle, pair, ",\n", "");
  obstack_free (&muscle_obstack, pair);
}


எழுத்து மாறிலி *
muscle_find_const (எழுத்து மாறிலி *சாவி)
{
  muscle_entry *entry = muscle_lookup (சாவி);
  திரும்பு entry ? entry->மதிப்பு : வெற்று;
}


எழுத்து *
muscle_find (எழுத்து மாறிலி *சாவி)
{
  muscle_entry *entry = muscle_lookup (சாவி);
  எனில் (entry)
    {
      aver (entry->மதிப்பு == entry->storage);
      திரும்பு entry->storage;
    }
  திரும்பு வெற்று;
}


/* In the format 'file_name:line.column', append BOUND to MUSCLE.  Use
   digraphs for special characters in the file name.  */

நிலையான வெறுமை
muscle_boundary_grow (எழுத்து மாறிலி *சாவி, boundary bound)
{
  obstack_sgrow  (&muscle_obstack, "[[");
  obstack_escape (&muscle_obstack, bound.file);
  obstack_printf (&muscle_obstack, ":%d.%d@@%d]]", bound.வரி, bound.column, bound.byte);
  எழுத்து மாறிலி *extension = obstack_finish0 (&muscle_obstack);
  muscle_grow (சாவி, extension, "", "");
  obstack_free (&muscle_obstack, extension);
}


வெறுமை
muscle_location_grow (எழுத்து மாறிலி *சாவி, location loc)
{
  muscle_boundary_grow (சாவி, loc.start);
  muscle_grow (சாவி, "", ", ", "");
  muscle_boundary_grow (சாவி, loc.end);
}

#define COMMON_DECODE(மதிப்பு_4)                                    \
  நிலை '$':                                                     \
    ++(மதிப்பு_4); aver (*(மதிப்பு_4) == ']');                          \
    ++(மதிப்பு_4); aver (*(மதிப்பு_4) == '[');                          \
    obstack_sgrow (&muscle_obstack, "$");                       \
    நிறுத்து;                                                      \
  நிலை '@':                                                     \
    தேர்வு (*++(மதிப்பு_4))                                         \
      {                                                         \
      நிலை '\'': /* Ignore "@'" */ நிறுத்து;                       \
      நிலை '@': obstack_sgrow (&muscle_obstack, "@" ); நிறுத்து;   \
      நிலை '{': obstack_sgrow (&muscle_obstack, "[" ); நிறுத்து;   \
      நிலை '}': obstack_sgrow (&muscle_obstack, "]" ); நிறுத்து;   \
      இயல்பு: aver (false); நிறுத்து;                             \
      }                                                         \
    நிறுத்து;                                                      \
  இயல்பு:                                                      \
    obstack_1grow (&muscle_obstack, *(மதிப்பு_4));                  \
    நிறுத்து;

/* Reverse of obstack_escape.  */
நிலையான எழுத்து *
string_decode (எழுத்து மாறிலி *சாவி)
{
  எழுத்து மாறிலி *மதிப்பு = muscle_find_const (சாவி);
  எனில் (!மதிப்பு)
    திரும்பு வெற்று;
  செய் {
    தேர்வு (*மதிப்பு)
      {
        COMMON_DECODE (மதிப்பு)
        நிலை '[':
        நிலை ']':
          aver (false);
          நிறுத்து;
      }
  } போது (*மதிப்பு++);
  எழுத்து மாறிலி *மதிப்பு_decoded = obstack_finish (&muscle_obstack);
  எழுத்து *res = xstrdup (மதிப்பு_decoded);
  obstack_free (&muscle_obstack, மதிப்பு_decoded);
  திரும்பு res;
}

/* Reverse of muscle_location_grow.  */
நிலையான location
location_decode (எழுத்து மாறிலி *மதிப்பு)
{
  aver (மதிப்பு);
  aver (*மதிப்பு == '[');
  ++மதிப்பு; aver (*மதிப்பு == '[');
  location loc;
  போது (*++மதிப்பு)
    தேர்வு (*மதிப்பு)
      {
        COMMON_DECODE (மதிப்பு)
        நிலை '[':
          aver (false);
          நிறுத்து;
        நிலை ']':
          ++மதிப்பு; aver (*மதிப்பு == ']');
          எழுத்து *boundary_str = obstack_finish0 (&muscle_obstack);
          தேர்வு (*++மதிப்பு)
            {
            நிலை ',':
              boundary_set_from_string (&loc.start, boundary_str);
              obstack_free (&muscle_obstack, boundary_str);
              ++மதிப்பு; aver (*மதிப்பு == ' ');
              ++மதிப்பு; aver (*மதிப்பு == '[');
              ++மதிப்பு; aver (*மதிப்பு == '[');
              நிறுத்து;
            நிலை '\0':
              boundary_set_from_string (&loc.end, boundary_str);
              obstack_free (&muscle_obstack, boundary_str);
              திரும்பு loc;
              நிறுத்து;
            இயல்பு:
              aver (false);
              நிறுத்து;
            }
          நிறுத்து;
      }
  aver (false);
  திரும்பு loc;
}

வெறுமை
muscle_user_name_list_grow (எழுத்து மாறிலி *சாவி, எழுத்து மாறிலி *user_name,
                            location loc)
{
  muscle_grow (சாவி, "[[[[", ",", "");
  muscle_grow (சாவி, user_name, "", "");
  muscle_grow (சாவி, "]], ", "", "");
  muscle_location_grow (சாவி, loc);
  muscle_grow (சாவி, "]]", "", "");
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

நிலையான
எழுத்து *
define_directive (எழுத்து மாறிலி *assignment,
                  muscle_kind kind,
                  எழுத்து மாறிலி *மதிப்பு)
{
  எழுத்து *eq = strchr (assignment, '=');
  எழுத்து மாறிலி *fmt
    = eq || !மதிப்பு || !*மதிப்பு ? "%%define %s"
    : kind == muscle_code     ? "%%define %s {%s}"
    : kind == muscle_string   ? "%%define %s \"%s\""
    :                           "%%define %s %s";
  எழுத்து *res = xmalloc (strlen (fmt) + strlen (assignment)
                       + (மதிப்பு ? strlen (மதிப்பு) : 0));
  sprintf (res, fmt, assignment, மதிப்பு);
  eq = strchr (res, '=');
  எனில் (eq)
    *eq = eq[1] ? ' ' : '\0';
  திரும்பு res;
}

/** If the \a variable name is obsolete, return the name to use,
 * otherwise \a variable.  If the \a value is obsolete, update it too.
 *
 * Allocates the returned value if needed, otherwise the returned
 * value is exactly \a variable.  */
நிலையான
எழுத்து மாறிலி *
muscle_percent_variable_update (எழுத்து மாறிலி *variable,
                                muscle_kind kind,
                                எழுத்து மாறிலி **மதிப்பு,
                                எழுத்து **old, எழுத்து **upd)
{
  வகைவரை கட்டமைப்பு
  {
    மாறிலி எழுத்து *obsolete;
    மாறிலி எழுத்து *updated;
    muscle_kind kind;
  } conversion_type;
  மாறிலி conversion_type conversion[] =
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
    { வெற்று, வெற்று, -1, }
  };

  க்கு (conversion_type மாறிலி *c = conversion; c->obsolete; ++c)
    {
      எழுத்து மாறிலி *eq = strchr (c->obsolete, '=');
      எனில் (eq
          ? (!strncmp (c->obsolete, variable, eq - c->obsolete)
             && STREQ (eq + 1, *மதிப்பு))
          : STREQ (c->obsolete, variable))
        {
          /* Generate the deprecation warning. */
          *old = c->obsolete[0] == '%'
            ? xstrdup (c->obsolete)
            : define_directive (c->obsolete, kind, *மதிப்பு);
          *upd = define_directive (c->updated, c->kind, *மதிப்பு);
          /* Update the variable and its value.  */
          {
            எழுத்து *res = xstrdup (c->updated);
            எழுத்து *eq2 = strchr (res, '=');
            எனில் (eq2)
              {
                *eq2 = '\0';
                *மதிப்பு = eq2 + 1;
              }
            திரும்பு res;
          }
        }
    }
  திரும்பு variable;
}

வெறுமை
muscle_percent_define_செருகு (எழுத்து மாறிலி *var, location variable_loc,
                              muscle_kind kind,
                              எழுத்து மாறிலி *மதிப்பு,
                              muscle_percent_define_how how)
{
  /* Backward compatibility.  */
  எழுத்து *old = வெற்று;
  எழுத்து *upd = வெற்று;
  எழுத்து மாறிலி *variable
    = muscle_percent_variable_update (var, kind,
                                      &மதிப்பு, &old, &upd);
  uniqstr name = muscle_name (variable, வெற்று);
  uniqstr loc_name = muscle_name (variable, "loc");
  uniqstr syncline_name = muscle_name (variable, "syncline");
  uniqstr how_name = muscle_name (variable, "how");
  uniqstr kind_name = muscle_name (variable, "kind");

  /* Command-line options are processed before the grammar file.  */
  bool warned = false;
  எனில் (how == MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE)
    {
      எழுத்து மாறிலி *current_மதிப்பு = muscle_find_const (name);
      எனில் (current_மதிப்பு)
        {
          long l = strtol (muscle_find_const (how_name), வெற்று, 10);
          muscle_percent_define_how how_old
            = 0 <= l && l <= INT_MAX ? l : INT_MAX;
          எனில் (how_old == MUSCLE_PERCENT_DEFINE_F)
            செல் end;
          /* If assigning the same value, make it a warning.  */
          warnings warn = STREQ (மதிப்பு, current_மதிப்பு) ? Wother : complaint;
          complain (&variable_loc, warn,
                    _("%%define variable %s redefined"),
                    quote (variable));
          location loc = muscle_percent_define_get_loc (variable);
          subcomplain (&loc, warn, _("previous definition"));
          fixits_register (&variable_loc, "");
          warned = true;
        }
    }

  எனில் (!warned && old && upd)
    deprecated_directive (&variable_loc, old, upd);

  MUSCLE_செருகு_STRING (name, மதிப்பு);
  muscle_செருகு (loc_name, "");
  muscle_location_grow (loc_name, variable_loc);
  muscle_செருகு (syncline_name, "");
  muscle_syncline_grow (syncline_name, variable_loc);
  muscle_user_name_list_grow ("percent_define_user_variables", variable,
                              variable_loc);
  MUSCLE_செருகு_INT (how_name, how);
  MUSCLE_செருகு_STRING (kind_name, muscle_kind_string (kind));
 end:
  விடுவி (old);
  விடுவி (upd);
  எனில் (variable != var)
    விடுவி ((எழுத்து *) variable);
}

/* This is used for backward compatibility, e.g., "%define api.pure"
   supersedes "%pure-parser".  */
வெறுமை
muscle_percent_define_ensure (எழுத்து மாறிலி *variable, location loc,
                              bool மதிப்பு)
{
  uniqstr name = muscle_name (variable, வெற்று);
  எழுத்து மாறிலி *val = மதிப்பு ? "" : "false";

  /* Don't complain is VARIABLE is already defined, but be sure to set
     its value to VAL.  */
  எனில் (!muscle_find_const (name)
      || muscle_percent_define_flag_if (variable) != மதிப்பு)
    muscle_percent_define_செருகு (variable, loc, muscle_keyword, val,
                                  MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
}

/* Mark %define VARIABLE as used.  */
நிலையான வெறுமை
muscle_percent_define_use (எழுத்து மாறிலி *variable)
{
  muscle_செருகு (muscle_name (variable, "bison_variables"), "");
}

/* The value of %define variable VARIABLE (corresponding to FIELD, if
   defined).  Do not register as used, but diagnose unset variables.  */

நிலையான
எழுத்து மாறிலி *
muscle_percent_define_get_raw (எழுத்து மாறிலி *variable, எழுத்து மாறிலி *field)
{
  uniqstr name = muscle_name (variable, field);
  எழுத்து மாறிலி *res = muscle_find_const (name);
  எனில் (!res)
    complain (வெற்று, fatal, _("%s: undefined %%define variable %s"),
              "muscle_percent_define_get_raw", quote (variable));
  திரும்பு res;
}

எழுத்து *
muscle_percent_define_get (எழுத்து மாறிலி *variable)
{
  uniqstr name = muscle_name (variable, வெற்று);
  எழுத்து *மதிப்பு = string_decode (name);
  எனில் (!மதிப்பு)
    மதிப்பு = xstrdup ("");
  muscle_percent_define_use (variable);
  திரும்பு மதிப்பு;
}

/* The kind of VARIABLE.  An error if undefined.  */
நிலையான muscle_kind
muscle_percent_define_get_kind (எழுத்து மாறிலி *variable)
{
  திரும்பு muscle_kind_new (muscle_percent_define_get_raw (variable, "kind"));
}

/* Check the kind of VARIABLE.  An error if undefined.  */
நிலையான வெறுமை
muscle_percent_define_check_kind (எழுத்து மாறிலி *variable, muscle_kind kind)
{
  எனில் (muscle_percent_define_get_kind (variable) != kind)
    {
      location loc = muscle_percent_define_get_loc (variable);
      தேர்வு (kind)
        {
        நிலை muscle_code:
          complain (&loc, Wdeprecated,
                    _("%%define variable '%s' requires '{...}' values"),
                    variable);
          நிறுத்து;
        நிலை muscle_keyword:
          complain (&loc, Wdeprecated,
                    _("%%define variable '%s' requires keyword values"),
                    variable);
          நிறுத்து;
        நிலை muscle_string:
          complain (&loc, Wdeprecated,
                    _("%%define variable '%s' requires '\"...\"' values"),
                    variable);
          நிறுத்து;
        }
    }
}


location
muscle_percent_define_get_loc (எழுத்து மாறிலி *variable)
{
  திரும்பு location_decode (muscle_percent_define_get_raw (variable, "loc"));
}

எழுத்து மாறிலி *
muscle_percent_define_get_syncline (எழுத்து மாறிலி *variable)
{
  திரும்பு muscle_percent_define_get_raw (variable, "syncline");
}

bool
muscle_percent_define_ifdef (எழுத்து மாறிலி *variable)
{
  எனில் (muscle_find_const (muscle_name (variable, வெற்று)))
    {
      muscle_percent_define_use (variable);
      திரும்பு true;
    }
  இல்லையெனில்
    திரும்பு false;
}

bool
muscle_percent_define_flag_if (எழுத்து மாறிலி *variable)
{
  uniqstr invalid_boolean_name = muscle_name (variable, "invalid_boolean");
  bool res = false;

  எனில் (muscle_percent_define_ifdef (variable))
    {
      எழுத்து *மதிப்பு = muscle_percent_define_get (variable);
      muscle_percent_define_check_kind (variable, muscle_keyword);
      எனில் (மதிப்பு[0] == '\0' || STREQ (மதிப்பு, "true"))
        res = true;
      இல்லையெனில் எனில் (STREQ (மதிப்பு, "false"))
        res = false;
      இல்லையெனில் எனில் (!muscle_find_const (invalid_boolean_name))
        {
          muscle_செருகு (invalid_boolean_name, "");
          location loc = muscle_percent_define_get_loc (variable);
          complain (&loc, complaint,
                    _("invalid value for %%define Boolean variable %s"),
                    quote (variable));
        }
      விடுவி (மதிப்பு);
    }
  இல்லையெனில்
    complain (வெற்று, fatal, _("%s: undefined %%define variable %s"),
              "muscle_percent_define_flag", quote (variable));

  திரும்பு res;
}

வெறுமை
muscle_percent_define_default (எழுத்து மாறிலி *variable, எழுத்து மாறிலி *மதிப்பு)
{
  uniqstr name = muscle_name (variable, வெற்று);
  எனில் (!muscle_find_const (name))
    {
      MUSCLE_செருகு_STRING (name, மதிப்பு);
      MUSCLE_செருகு_STRING (muscle_name (variable, "kind"), "keyword");
      {
        uniqstr loc_name = muscle_name (variable, "loc");
        location loc;
        loc.start.file = "<default value>";
        loc.start.வரி = -1;
        loc.start.column = -1;
        loc.start.byte = -1;
        loc.end = loc.start;
        muscle_செருகு (loc_name, "");
        muscle_location_grow (loc_name, loc);
      }
      muscle_செருகு (muscle_name (variable, "syncline"), "");
    }
}

வெறுமை
muscle_percent_define_check_மதிப்புகள் (எழுத்து மாறிலி * மாறிலி *மதிப்புகள்)
{
  க்கு (; *மதிப்புகள்; ++மதிப்புகள்)
    {
      எழுத்து மாறிலி * மாறிலி *variablep = மதிப்புகள்;
      uniqstr name = muscle_name (*variablep, வெற்று);
      எழுத்து *மதிப்பு = string_decode (name);
      muscle_percent_define_check_kind (*variablep, muscle_keyword);
      எனில் (மதிப்பு)
        {
          க்கு (++மதிப்புகள்; *மதிப்புகள்; ++மதிப்புகள்)
            எனில் (STREQ (மதிப்பு, *மதிப்புகள்))
              நிறுத்து;
          எனில் (!*மதிப்புகள்)
            {
              location loc = muscle_percent_define_get_loc (*variablep);
              complain (&loc, complaint,
                        _("invalid value for %%define variable %s: %s"),
                        quote (*variablep), quote_n (1, மதிப்பு));
              க்கு (மதிப்புகள் = variablep + 1; *மதிப்புகள்; ++மதிப்புகள்)
                subcomplain (&loc, complaint | no_caret | silent,
                             _("accepted value: %s"), quote (*மதிப்புகள்));
            }
          இல்லையெனில்
            போது (*மதிப்புகள்)
              ++மதிப்புகள்;
          விடுவி (மதிப்பு);
        }
      இல்லையெனில்
        complain (வெற்று, fatal, _("%s: undefined %%define variable %s"),
                  "muscle_percent_define_check_values", quote (*variablep));
    }
}

வெறுமை
muscle_percent_code_grow (எழுத்து மாறிலி *qualifier, location qualifier_loc,
                          எழுத்து மாறிலி *code, location code_loc)
{
  எழுத்து மாறிலி *name = UNIQSTR_CONCAT ("percent_code(", qualifier, ")");
  muscle_code_grow (name, code, code_loc);
  muscle_user_name_list_grow ("percent_code_user_qualifiers", qualifier,
                               qualifier_loc);
}


/*------------------------------------------------.
| Output the definition of ENTRY as a m4_define.  |
`------------------------------------------------*/

நிலையான உள்வரி bool
muscle_m4_output (muscle_entry *entry, FILE *out)
{
  fprintf (out,
           "m4_define([b4_%s],\n"
           "[[%s]])\n\n\n", entry->சாவி, entry->மதிப்பு);
  திரும்பு true;
}

நிலையான bool
muscle_m4_output_processor (வெறுமை *entry, வெறுமை *out)
{
  திரும்பு muscle_m4_output (entry, out);
}


வெறுமை
muscles_m4_output (FILE *out)
{
  hash_do_for_each (muscle_table, muscle_m4_output_processor, out);
}
