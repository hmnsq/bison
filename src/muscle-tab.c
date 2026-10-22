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

#ضمن <config.h>
#ضمن "system.h"

#ضمن <hash.h>
#ضمن <quote.h>

#ضمن "complain.h"
#ضمن "files.h"
#ضمن "fixits.h"
#ضمن "getargs.h"
#ضمن "muscle-tab.h"

muscle_kind
muscle_kind_new (حرف ثابت *k)
{
  إذا (STREQ (k, "code"))
    أرجع muscle_code;
  وإلا إذا (STREQ (k, "keyword"))
    أرجع muscle_keyword;
  وإلا إذا (STREQ (k, "string"))
    أرجع muscle_string;
  abort ();
}

حرف ثابت *
muscle_kind_string (muscle_kind k)
{
  بدل (k)
    {
    حالة muscle_code:    أرجع "code";
    حالة muscle_keyword: أرجع "keyword";
    حالة muscle_string:  أرجع "string";
    }
  abort ();
}


/* A key-value pair, along with storage that can be reclaimed when
   this pair is no longer needed.  */
تعريفنوع بنية
{
  حرف ثابت *کلیل;
  حرف ثابت *بەها;
  حرف *storage;
  muscle_kind kind;
} muscle_entry;


/* The name of muscle for the %define variable VAR (corresponding to
   FIELD, if defined).  */
ساكن uniqstr
muscle_name (حرف ثابت *var, حرف ثابت *field)
{
  إذا (field)
    أرجع UNIQSTR_CONCAT ("percent_define_", field, "(", var, ")");
  وإلا
    أرجع UNIQSTR_CONCAT ("percent_define(", var, ")");
}

/* An obstack used to create some entries.  */
بنية obstack muscle_obstack;

/* Initial capacity of muscles hash table.  */
#define HT_INITIAL_CAPACITY 257

ساكن بنية hash_table *muscle_table = عدم;

ساكن bool
hash_compare_muscles (فراغ ثابت *x, فراغ ثابت *y)
{
  muscle_entry ثابت *m1 = x;
  muscle_entry ثابت *m2 = y;
  أرجع STREQ (m1->کلیل, m2->کلیل);
}

ساكن size_t
hash_muscle (ثابت فراغ *x, size_t tablesize)
{
  muscle_entry ثابت *m = x;
  أرجع hash_string (m->کلیل, tablesize);
}

/* Create a fresh muscle name KEY, and insert in the hash table.  */
ساكن فراغ *
muscle_entry_new (حرف ثابت *کلیل)
{
  muscle_entry *res = xmalloc (حجم *res);
  res->کلیل = کلیل;
  res->بەها = عدم;
  res->storage = عدم;
  hash_xinsert (muscle_table, res);
  أرجع res;
}

ساكن فراغ
muscle_entry_free (فراغ *entry)
{
  muscle_entry *mentry = entry;
  حرر (mentry->storage);
  حرر (mentry);
}

فراغ
muscle_init (فراغ)
{
  /* Initialize the muscle obstack.  */
  obstack_init (&muscle_obstack);

  muscle_table = hash_xinitialize (HT_INITIAL_CAPACITY, عدم, hash_muscle,
                                   hash_compare_muscles, muscle_entry_free);
  /* Avoid warnings if the user defined this variable, but did not
     actually call -Wcex.  */
  حرر (muscle_percent_define_get ("cex.timeout"));
}


فراغ
muscle_free (فراغ)
{
  hash_free (muscle_table);
  obstack_free (&muscle_obstack, عدم);
}

/* Look for the muscle named KEY.  Return NULL if does not exist.  */
ساكن muscle_entry *
muscle_lookup (حرف ثابت *کلیل)
{
  muscle_entry probe;
  probe.کلیل = کلیل;
  أرجع hash_lookup (muscle_table, &probe);
}


فراغ
muscle_زیادکردن (حرف ثابت *کلیل, حرف ثابت *بەها)
{
  muscle_entry *entry = muscle_lookup (کلیل);
  إذا (entry)
    حرر (entry->storage);
  وإلا
    /* First insertion in the hash. */
    entry = muscle_entry_new (کلیل);
  entry->بەها = بەها;
  entry->storage = عدم;
}


/* Append VALUE to the current value of KEY.  If KEY did not already
   exist, create it.  Use MUSCLE_OBSTACK.  De-allocate the previously
   associated value.  Copy VALUE and SEPARATOR.  If VALUE does not end
   with TERMINATOR, append one.  */

ساكن فراغ
muscle_grow (ثابت حرف *کلیل, ثابت حرف *val,
             ثابت حرف *separator, ثابت حرف *terminator)
{
  muscle_entry *entry = muscle_lookup (کلیل);
  إذا (entry)
    {
      obstack_sgrow (&muscle_obstack, entry->بەها);
      obstack_sgrow (&muscle_obstack, separator);
      حرر (entry->storage);
    }
  وإلا
    entry = muscle_entry_new (کلیل);

  obstack_sgrow (&muscle_obstack, val);

  size_t vals = strlen (val);
  size_t terms = strlen (terminator);
  إذا (terms <= vals
      && STRNEQ (val + vals - terms, terminator))
    obstack_sgrow (&muscle_obstack, terminator);

  {
    حرف ثابت *new_val = obstack_finish0 (&muscle_obstack);
    entry->بەها = entry->storage = xstrdup (new_val);
    obstack_free (&muscle_obstack, new_val);
  }
}

/*------------------------------------------------------------------.
| Using muscle_grow, append a synchronization line for the location |
| LOC to the current value of KEY.                                  |
`------------------------------------------------------------------*/

ساكن فراغ
muscle_syncline_grow (حرف ثابت *کلیل, location loc)
{
  obstack_printf (&muscle_obstack, "]b4_syncline(%d, ", loc.start.هێڵ);
  obstack_quote (&muscle_obstack,
                 quotearg_style (c_quoting_style, map_file_name (loc.start.file)));
  obstack_sgrow (&muscle_obstack, ")dnl\n[");
  حرف ثابت *extension = obstack_finish0 (&muscle_obstack);
  muscle_grow (کلیل, extension, "", "");
  obstack_free (&muscle_obstack, extension);
}

/*------------------------------------------------------------------.
| Append VALUE to the current value of KEY, using muscle_grow.  But |
| in addition, issue a synchronization line for the location LOC    |
| using muscle_syncline_grow.                                       |
`------------------------------------------------------------------*/

فراغ
muscle_code_grow (ثابت حرف *کلیل, ثابت حرف *val, location loc)
{
  muscle_syncline_grow (کلیل, loc);
  muscle_grow (کلیل, val, "", "\n");
}


فراغ
muscle_pair_list_grow (ثابت حرف *muscle,
                       ثابت حرف *a1, ثابت حرف *a2)
{
  obstack_sgrow (&muscle_obstack, "[");
  obstack_quote (&muscle_obstack, a1);
  obstack_sgrow (&muscle_obstack, ", ");
  obstack_quote (&muscle_obstack, a2);
  obstack_sgrow (&muscle_obstack, "]");
  حرف ثابت *pair = obstack_finish0 (&muscle_obstack);
  muscle_grow (muscle, pair, ",\n", "");
  obstack_free (&muscle_obstack, pair);
}


حرف ثابت *
muscle_find_const (حرف ثابت *کلیل)
{
  muscle_entry *entry = muscle_lookup (کلیل);
  أرجع entry ? entry->بەها : عدم;
}


حرف *
muscle_find (حرف ثابت *کلیل)
{
  muscle_entry *entry = muscle_lookup (کلیل);
  إذا (entry)
    {
      aver (entry->بەها == entry->storage);
      أرجع entry->storage;
    }
  أرجع عدم;
}


/* In the format 'file_name:line.column', append BOUND to MUSCLE.  Use
   digraphs for special characters in the file name.  */

ساكن فراغ
muscle_boundary_grow (حرف ثابت *کلیل, boundary bound)
{
  obstack_sgrow  (&muscle_obstack, "[[");
  obstack_escape (&muscle_obstack, bound.file);
  obstack_printf (&muscle_obstack, ":%d.%d@@%d]]", bound.هێڵ, bound.column, bound.byte);
  حرف ثابت *extension = obstack_finish0 (&muscle_obstack);
  muscle_grow (کلیل, extension, "", "");
  obstack_free (&muscle_obstack, extension);
}


فراغ
muscle_location_grow (حرف ثابت *کلیل, location loc)
{
  muscle_boundary_grow (کلیل, loc.start);
  muscle_grow (کلیل, "", ", ", "");
  muscle_boundary_grow (کلیل, loc.end);
}

#define COMMON_DECODE(بەها_4)                                    \
  حالة '$':                                                     \
    ++(بەها_4); aver (*(بەها_4) == ']');                          \
    ++(بەها_4); aver (*(بەها_4) == '[');                          \
    obstack_sgrow (&muscle_obstack, "$");                       \
    اكسر;                                                      \
  حالة '@':                                                     \
    بدل (*++(بەها_4))                                         \
      {                                                         \
      حالة '\'': /* Ignore "@'" */ اكسر;                       \
      حالة '@': obstack_sgrow (&muscle_obstack, "@" ); اكسر;   \
      حالة '{': obstack_sgrow (&muscle_obstack, "[" ); اكسر;   \
      حالة '}': obstack_sgrow (&muscle_obstack, "]" ); اكسر;   \
      افتراضي: aver (false); اكسر;                             \
      }                                                         \
    اكسر;                                                      \
  افتراضي:                                                      \
    obstack_1grow (&muscle_obstack, *(بەها_4));                  \
    اكسر;

/* Reverse of obstack_escape.  */
ساكن حرف *
string_decode (حرف ثابت *کلیل)
{
  حرف ثابت *بەها = muscle_find_const (کلیل);
  إذا (!بەها)
    أرجع عدم;
  افعل {
    بدل (*بەها)
      {
        COMMON_DECODE (بەها)
        حالة '[':
        حالة ']':
          aver (false);
          اكسر;
      }
  } طالما (*بەها++);
  حرف ثابت *بەها_decoded = obstack_finish (&muscle_obstack);
  حرف *res = xstrdup (بەها_decoded);
  obstack_free (&muscle_obstack, بەها_decoded);
  أرجع res;
}

/* Reverse of muscle_location_grow.  */
ساكن location
location_decode (حرف ثابت *بەها)
{
  aver (بەها);
  aver (*بەها == '[');
  ++بەها; aver (*بەها == '[');
  location loc;
  طالما (*++بەها)
    بدل (*بەها)
      {
        COMMON_DECODE (بەها)
        حالة '[':
          aver (false);
          اكسر;
        حالة ']':
          ++بەها; aver (*بەها == ']');
          حرف *boundary_str = obstack_finish0 (&muscle_obstack);
          بدل (*++بەها)
            {
            حالة ',':
              boundary_set_from_string (&loc.start, boundary_str);
              obstack_free (&muscle_obstack, boundary_str);
              ++بەها; aver (*بەها == ' ');
              ++بەها; aver (*بەها == '[');
              ++بەها; aver (*بەها == '[');
              اكسر;
            حالة '\0':
              boundary_set_from_string (&loc.end, boundary_str);
              obstack_free (&muscle_obstack, boundary_str);
              أرجع loc;
              اكسر;
            افتراضي:
              aver (false);
              اكسر;
            }
          اكسر;
      }
  aver (false);
  أرجع loc;
}

فراغ
muscle_user_name_list_grow (حرف ثابت *کلیل, حرف ثابت *user_name,
                            location loc)
{
  muscle_grow (کلیل, "[[[[", ",", "");
  muscle_grow (کلیل, user_name, "", "");
  muscle_grow (کلیل, "]], ", "", "");
  muscle_location_grow (کلیل, loc);
  muscle_grow (کلیل, "]]", "", "");
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

ساكن
حرف *
define_directive (حرف ثابت *assignment,
                  muscle_kind kind,
                  حرف ثابت *بەها)
{
  حرف *eq = strchr (assignment, '=');
  حرف ثابت *fmt
    = eq || !بەها || !*بەها ? "%%define %s"
    : kind == muscle_code     ? "%%define %s {%s}"
    : kind == muscle_string   ? "%%define %s \"%s\""
    :                           "%%define %s %s";
  حرف *res = xmalloc (strlen (fmt) + strlen (assignment)
                       + (بەها ? strlen (بەها) : 0));
  sprintf (res, fmt, assignment, بەها);
  eq = strchr (res, '=');
  إذا (eq)
    *eq = eq[1] ? ' ' : '\0';
  أرجع res;
}

/** If the \a variable name is obsolete, return the name to use,
 * otherwise \a variable.  If the \a value is obsolete, update it too.
 *
 * Allocates the returned value if needed, otherwise the returned
 * value is exactly \a variable.  */
ساكن
حرف ثابت *
muscle_percent_variable_update (حرف ثابت *variable,
                                muscle_kind kind,
                                حرف ثابت **بەها,
                                حرف **old, حرف **upd)
{
  تعريفنوع بنية
  {
    ثابت حرف *obsolete;
    ثابت حرف *updated;
    muscle_kind kind;
  } conversion_type;
  ثابت conversion_type conversion[] =
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
    { عدم, عدم, -1, }
  };

  لكل (conversion_type ثابت *c = conversion; c->obsolete; ++c)
    {
      حرف ثابت *eq = strchr (c->obsolete, '=');
      إذا (eq
          ? (!strncmp (c->obsolete, variable, eq - c->obsolete)
             && STREQ (eq + 1, *بەها))
          : STREQ (c->obsolete, variable))
        {
          /* Generate the deprecation warning. */
          *old = c->obsolete[0] == '%'
            ? xstrdup (c->obsolete)
            : define_directive (c->obsolete, kind, *بەها);
          *upd = define_directive (c->updated, c->kind, *بەها);
          /* Update the variable and its value.  */
          {
            حرف *res = xstrdup (c->updated);
            حرف *eq2 = strchr (res, '=');
            إذا (eq2)
              {
                *eq2 = '\0';
                *بەها = eq2 + 1;
              }
            أرجع res;
          }
        }
    }
  أرجع variable;
}

فراغ
muscle_percent_define_زیادکردن (حرف ثابت *var, location variable_loc,
                              muscle_kind kind,
                              حرف ثابت *بەها,
                              muscle_percent_define_how how)
{
  /* Backward compatibility.  */
  حرف *old = عدم;
  حرف *upd = عدم;
  حرف ثابت *variable
    = muscle_percent_variable_update (var, kind,
                                      &بەها, &old, &upd);
  uniqstr name = muscle_name (variable, عدم);
  uniqstr loc_name = muscle_name (variable, "loc");
  uniqstr syncline_name = muscle_name (variable, "syncline");
  uniqstr how_name = muscle_name (variable, "how");
  uniqstr kind_name = muscle_name (variable, "kind");

  /* Command-line options are processed before the grammar file.  */
  bool warned = false;
  إذا (how == MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE)
    {
      حرف ثابت *current_بەها = muscle_find_const (name);
      إذا (current_بەها)
        {
          long l = strtol (muscle_find_const (how_name), عدم, 10);
          muscle_percent_define_how how_old
            = 0 <= l && l <= INT_MAX ? l : INT_MAX;
          إذا (how_old == MUSCLE_PERCENT_DEFINE_F)
            اذهب end;
          /* If assigning the same value, make it a warning.  */
          warnings warn = STREQ (بەها, current_بەها) ? Wother : complaint;
          complain (&variable_loc, warn,
                    _("%%define variable %s redefined"),
                    quote (variable));
          location loc = muscle_percent_define_get_loc (variable);
          subcomplain (&loc, warn, _("previous definition"));
          fixits_register (&variable_loc, "");
          warned = true;
        }
    }

  إذا (!warned && old && upd)
    deprecated_directive (&variable_loc, old, upd);

  MUSCLE_زیادکردن_STRING (name, بەها);
  muscle_زیادکردن (loc_name, "");
  muscle_location_grow (loc_name, variable_loc);
  muscle_زیادکردن (syncline_name, "");
  muscle_syncline_grow (syncline_name, variable_loc);
  muscle_user_name_list_grow ("percent_define_user_variables", variable,
                              variable_loc);
  MUSCLE_زیادکردن_INT (how_name, how);
  MUSCLE_زیادکردن_STRING (kind_name, muscle_kind_string (kind));
 end:
  حرر (old);
  حرر (upd);
  إذا (variable != var)
    حرر ((حرف *) variable);
}

/* This is used for backward compatibility, e.g., "%define api.pure"
   supersedes "%pure-parser".  */
فراغ
muscle_percent_define_ensure (حرف ثابت *variable, location loc,
                              bool بەها)
{
  uniqstr name = muscle_name (variable, عدم);
  حرف ثابت *val = بەها ? "" : "false";

  /* Don't complain is VARIABLE is already defined, but be sure to set
     its value to VAL.  */
  إذا (!muscle_find_const (name)
      || muscle_percent_define_flag_if (variable) != بەها)
    muscle_percent_define_زیادکردن (variable, loc, muscle_keyword, val,
                                  MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
}

/* Mark %define VARIABLE as used.  */
ساكن فراغ
muscle_percent_define_use (حرف ثابت *variable)
{
  muscle_زیادکردن (muscle_name (variable, "bison_variables"), "");
}

/* The value of %define variable VARIABLE (corresponding to FIELD, if
   defined).  Do not register as used, but diagnose unset variables.  */

ساكن
حرف ثابت *
muscle_percent_define_get_raw (حرف ثابت *variable, حرف ثابت *field)
{
  uniqstr name = muscle_name (variable, field);
  حرف ثابت *res = muscle_find_const (name);
  إذا (!res)
    complain (عدم, fatal, _("%s: undefined %%define variable %s"),
              "muscle_percent_define_get_raw", quote (variable));
  أرجع res;
}

حرف *
muscle_percent_define_get (حرف ثابت *variable)
{
  uniqstr name = muscle_name (variable, عدم);
  حرف *بەها = string_decode (name);
  إذا (!بەها)
    بەها = xstrdup ("");
  muscle_percent_define_use (variable);
  أرجع بەها;
}

/* The kind of VARIABLE.  An error if undefined.  */
ساكن muscle_kind
muscle_percent_define_get_kind (حرف ثابت *variable)
{
  أرجع muscle_kind_new (muscle_percent_define_get_raw (variable, "kind"));
}

/* Check the kind of VARIABLE.  An error if undefined.  */
ساكن فراغ
muscle_percent_define_check_kind (حرف ثابت *variable, muscle_kind kind)
{
  إذا (muscle_percent_define_get_kind (variable) != kind)
    {
      location loc = muscle_percent_define_get_loc (variable);
      بدل (kind)
        {
        حالة muscle_code:
          complain (&loc, Wdeprecated,
                    _("%%define variable '%s' requires '{...}' values"),
                    variable);
          اكسر;
        حالة muscle_keyword:
          complain (&loc, Wdeprecated,
                    _("%%define variable '%s' requires keyword values"),
                    variable);
          اكسر;
        حالة muscle_string:
          complain (&loc, Wdeprecated,
                    _("%%define variable '%s' requires '\"...\"' values"),
                    variable);
          اكسر;
        }
    }
}


location
muscle_percent_define_get_loc (حرف ثابت *variable)
{
  أرجع location_decode (muscle_percent_define_get_raw (variable, "loc"));
}

حرف ثابت *
muscle_percent_define_get_syncline (حرف ثابت *variable)
{
  أرجع muscle_percent_define_get_raw (variable, "syncline");
}

bool
muscle_percent_define_ifdef (حرف ثابت *variable)
{
  إذا (muscle_find_const (muscle_name (variable, عدم)))
    {
      muscle_percent_define_use (variable);
      أرجع true;
    }
  وإلا
    أرجع false;
}

bool
muscle_percent_define_flag_if (حرف ثابت *variable)
{
  uniqstr invalid_boolean_name = muscle_name (variable, "invalid_boolean");
  bool res = false;

  إذا (muscle_percent_define_ifdef (variable))
    {
      حرف *بەها = muscle_percent_define_get (variable);
      muscle_percent_define_check_kind (variable, muscle_keyword);
      إذا (بەها[0] == '\0' || STREQ (بەها, "true"))
        res = true;
      وإلا إذا (STREQ (بەها, "false"))
        res = false;
      وإلا إذا (!muscle_find_const (invalid_boolean_name))
        {
          muscle_زیادکردن (invalid_boolean_name, "");
          location loc = muscle_percent_define_get_loc (variable);
          complain (&loc, complaint,
                    _("invalid value for %%define Boolean variable %s"),
                    quote (variable));
        }
      حرر (بەها);
    }
  وإلا
    complain (عدم, fatal, _("%s: undefined %%define variable %s"),
              "muscle_percent_define_flag", quote (variable));

  أرجع res;
}

فراغ
muscle_percent_define_default (حرف ثابت *variable, حرف ثابت *بەها)
{
  uniqstr name = muscle_name (variable, عدم);
  إذا (!muscle_find_const (name))
    {
      MUSCLE_زیادکردن_STRING (name, بەها);
      MUSCLE_زیادکردن_STRING (muscle_name (variable, "kind"), "keyword");
      {
        uniqstr loc_name = muscle_name (variable, "loc");
        location loc;
        loc.start.file = "<default value>";
        loc.start.هێڵ = -1;
        loc.start.column = -1;
        loc.start.byte = -1;
        loc.end = loc.start;
        muscle_زیادکردن (loc_name, "");
        muscle_location_grow (loc_name, loc);
      }
      muscle_زیادکردن (muscle_name (variable, "syncline"), "");
    }
}

فراغ
muscle_percent_define_check_بەهاکان (حرف ثابت * ثابت *بەهاکان)
{
  لكل (; *بەهاکان; ++بەهاکان)
    {
      حرف ثابت * ثابت *variablep = بەهاکان;
      uniqstr name = muscle_name (*variablep, عدم);
      حرف *بەها = string_decode (name);
      muscle_percent_define_check_kind (*variablep, muscle_keyword);
      إذا (بەها)
        {
          لكل (++بەهاکان; *بەهاکان; ++بەهاکان)
            إذا (STREQ (بەها, *بەهاکان))
              اكسر;
          إذا (!*بەهاکان)
            {
              location loc = muscle_percent_define_get_loc (*variablep);
              complain (&loc, complaint,
                        _("invalid value for %%define variable %s: %s"),
                        quote (*variablep), quote_n (1, بەها));
              لكل (بەهاکان = variablep + 1; *بەهاکان; ++بەهاکان)
                subcomplain (&loc, complaint | no_caret | silent,
                             _("accepted value: %s"), quote (*بەهاکان));
            }
          وإلا
            طالما (*بەهاکان)
              ++بەهاکان;
          حرر (بەها);
        }
      وإلا
        complain (عدم, fatal, _("%s: undefined %%define variable %s"),
                  "muscle_percent_define_check_values", quote (*variablep));
    }
}

فراغ
muscle_percent_code_grow (حرف ثابت *qualifier, location qualifier_loc,
                          حرف ثابت *code, location code_loc)
{
  حرف ثابت *name = UNIQSTR_CONCAT ("percent_code(", qualifier, ")");
  muscle_code_grow (name, code, code_loc);
  muscle_user_name_list_grow ("percent_code_user_qualifiers", qualifier,
                               qualifier_loc);
}


/*------------------------------------------------.
| Output the definition of ENTRY as a m4_define.  |
`------------------------------------------------*/

ساكن مضمن bool
muscle_m4_output (muscle_entry *entry, FILE *out)
{
  fprintf (out,
           "m4_define([b4_%s],\n"
           "[[%s]])\n\n\n", entry->کلیل, entry->بەها);
  أرجع true;
}

ساكن bool
muscle_m4_output_processor (فراغ *entry, فراغ *out)
{
  أرجع muscle_m4_output (entry, out);
}


فراغ
muscles_m4_output (FILE *out)
{
  hash_do_for_each (muscle_table, muscle_m4_output_processor, out);
}
