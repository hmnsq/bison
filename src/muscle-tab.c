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

#شامل <config.h>
#شامل "system.h"

#شامل <hash.h>
#شامل <quote.h>

#شامل "complain.h"
#شامل "files.h"
#شامل "fixits.h"
#شامل "getargs.h"
#شامل "muscle-tab.h"

muscle_kind
muscle_kind_new (نویسه ثابت *k)
{
  اگر (STREQ (k, "code"))
    بازگشت muscle_code;
  وگرنه اگر (STREQ (k, "keyword"))
    بازگشت muscle_keyword;
  وگرنه اگر (STREQ (k, "string"))
    بازگشت muscle_string;
  abort ();
}

نویسه ثابت *
muscle_kind_string (muscle_kind k)
{
  انتخاب (k)
    {
    حالت muscle_code:    بازگشت "code";
    حالت muscle_keyword: بازگشت "keyword";
    حالت muscle_string:  بازگشت "string";
    }
  abort ();
}


/* A key-value pair, along with storage that can be reclaimed when
   this pair is no longer needed.  */
تعریفنوع ساختار
{
  نویسه ثابت *کلید;
  نویسه ثابت *مقدار;
  نویسه *storage;
  muscle_kind kind;
} muscle_entry;


/* The name of muscle for the %define variable VAR (corresponding to
   FIELD, if defined).  */
ایستا uniqstr
muscle_name (نویسه ثابت *var, نویسه ثابت *field)
{
  اگر (field)
    بازگشت UNIQSTR_CONCAT ("percent_define_", field, "(", var, ")");
  وگرنه
    بازگشت UNIQSTR_CONCAT ("percent_define(", var, ")");
}

/* An obstack used to create some entries.  */
ساختار obstack muscle_obstack;

/* Initial capacity of muscles hash table.  */
#define HT_INITIAL_CAPACITY 257

ایستا ساختار hash_table *muscle_table = پوچ;

ایستا bool
hash_compare_muscles (تهی ثابت *x, تهی ثابت *y)
{
  muscle_entry ثابت *m1 = x;
  muscle_entry ثابت *m2 = y;
  بازگشت STREQ (m1->کلید, m2->کلید);
}

ایستا size_t
hash_muscle (ثابت تهی *x, size_t tablesize)
{
  muscle_entry ثابت *m = x;
  بازگشت hash_string (m->کلید, tablesize);
}

/* Create a fresh muscle name KEY, and insert in the hash table.  */
ایستا تهی *
muscle_entry_new (نویسه ثابت *کلید)
{
  muscle_entry *res = xmalloc (اندازه *res);
  res->کلید = کلید;
  res->مقدار = پوچ;
  res->storage = پوچ;
  hash_xinsert (muscle_table, res);
  بازگشت res;
}

ایستا تهی
muscle_entry_free (تهی *entry)
{
  muscle_entry *mentry = entry;
  آزاد (mentry->storage);
  آزاد (mentry);
}

تهی
muscle_init (تهی)
{
  /* Initialize the muscle obstack.  */
  obstack_init (&muscle_obstack);

  muscle_table = hash_xinitialize (HT_INITIAL_CAPACITY, پوچ, hash_muscle,
                                   hash_compare_muscles, muscle_entry_free);
  /* Avoid warnings if the user defined this variable, but did not
     actually call -Wcex.  */
  آزاد (muscle_percent_define_get ("cex.timeout"));
}


تهی
muscle_free (تهی)
{
  hash_free (muscle_table);
  obstack_free (&muscle_obstack, پوچ);
}

/* Look for the muscle named KEY.  Return NULL if does not exist.  */
ایستا muscle_entry *
muscle_lookup (نویسه ثابت *کلید)
{
  muscle_entry probe;
  probe.کلید = کلید;
  بازگشت hash_lookup (muscle_table, &probe);
}


تهی
muscle_درج (نویسه ثابت *کلید, نویسه ثابت *مقدار)
{
  muscle_entry *entry = muscle_lookup (کلید);
  اگر (entry)
    آزاد (entry->storage);
  وگرنه
    /* First insertion in the hash. */
    entry = muscle_entry_new (کلید);
  entry->مقدار = مقدار;
  entry->storage = پوچ;
}


/* Append VALUE to the current value of KEY.  If KEY did not already
   exist, create it.  Use MUSCLE_OBSTACK.  De-allocate the previously
   associated value.  Copy VALUE and SEPARATOR.  If VALUE does not end
   with TERMINATOR, append one.  */

ایستا تهی
muscle_grow (ثابت نویسه *کلید, ثابت نویسه *val,
             ثابت نویسه *separator, ثابت نویسه *terminator)
{
  muscle_entry *entry = muscle_lookup (کلید);
  اگر (entry)
    {
      obstack_sgrow (&muscle_obstack, entry->مقدار);
      obstack_sgrow (&muscle_obstack, separator);
      آزاد (entry->storage);
    }
  وگرنه
    entry = muscle_entry_new (کلید);

  obstack_sgrow (&muscle_obstack, val);

  size_t vals = strlen (val);
  size_t terms = strlen (terminator);
  اگر (terms <= vals
      && STRNEQ (val + vals - terms, terminator))
    obstack_sgrow (&muscle_obstack, terminator);

  {
    نویسه ثابت *new_val = obstack_finish0 (&muscle_obstack);
    entry->مقدار = entry->storage = xstrdup (new_val);
    obstack_free (&muscle_obstack, new_val);
  }
}

/*------------------------------------------------------------------.
| Using muscle_grow, append a synchronization line for the location |
| LOC to the current value of KEY.                                  |
`------------------------------------------------------------------*/

ایستا تهی
muscle_syncline_grow (نویسه ثابت *کلید, location loc)
{
  obstack_printf (&muscle_obstack, "]b4_syncline(%d, ", loc.start.سطر);
  obstack_quote (&muscle_obstack,
                 quotearg_style (c_quoting_style, map_file_name (loc.start.file)));
  obstack_sgrow (&muscle_obstack, ")dnl\n[");
  نویسه ثابت *extension = obstack_finish0 (&muscle_obstack);
  muscle_grow (کلید, extension, "", "");
  obstack_free (&muscle_obstack, extension);
}

/*------------------------------------------------------------------.
| Append VALUE to the current value of KEY, using muscle_grow.  But |
| in addition, issue a synchronization line for the location LOC    |
| using muscle_syncline_grow.                                       |
`------------------------------------------------------------------*/

تهی
muscle_code_grow (ثابت نویسه *کلید, ثابت نویسه *val, location loc)
{
  muscle_syncline_grow (کلید, loc);
  muscle_grow (کلید, val, "", "\n");
}


تهی
muscle_pair_list_grow (ثابت نویسه *muscle,
                       ثابت نویسه *a1, ثابت نویسه *a2)
{
  obstack_sgrow (&muscle_obstack, "[");
  obstack_quote (&muscle_obstack, a1);
  obstack_sgrow (&muscle_obstack, ", ");
  obstack_quote (&muscle_obstack, a2);
  obstack_sgrow (&muscle_obstack, "]");
  نویسه ثابت *pair = obstack_finish0 (&muscle_obstack);
  muscle_grow (muscle, pair, ",\n", "");
  obstack_free (&muscle_obstack, pair);
}


نویسه ثابت *
muscle_find_const (نویسه ثابت *کلید)
{
  muscle_entry *entry = muscle_lookup (کلید);
  بازگشت entry ? entry->مقدار : پوچ;
}


نویسه *
muscle_find (نویسه ثابت *کلید)
{
  muscle_entry *entry = muscle_lookup (کلید);
  اگر (entry)
    {
      aver (entry->مقدار == entry->storage);
      بازگشت entry->storage;
    }
  بازگشت پوچ;
}


/* In the format 'file_name:line.column', append BOUND to MUSCLE.  Use
   digraphs for special characters in the file name.  */

ایستا تهی
muscle_boundary_grow (نویسه ثابت *کلید, boundary bound)
{
  obstack_sgrow  (&muscle_obstack, "[[");
  obstack_escape (&muscle_obstack, bound.file);
  obstack_printf (&muscle_obstack, ":%d.%d@@%d]]", bound.سطر, bound.column, bound.byte);
  نویسه ثابت *extension = obstack_finish0 (&muscle_obstack);
  muscle_grow (کلید, extension, "", "");
  obstack_free (&muscle_obstack, extension);
}


تهی
muscle_location_grow (نویسه ثابت *کلید, location loc)
{
  muscle_boundary_grow (کلید, loc.start);
  muscle_grow (کلید, "", ", ", "");
  muscle_boundary_grow (کلید, loc.end);
}

#define COMMON_DECODE(مقدار_4)                                    \
  حالت '$':                                                     \
    ++(مقدار_4); aver (*(مقدار_4) == ']');                          \
    ++(مقدار_4); aver (*(مقدار_4) == '[');                          \
    obstack_sgrow (&muscle_obstack, "$");                       \
    بشکن;                                                      \
  حالت '@':                                                     \
    انتخاب (*++(مقدار_4))                                         \
      {                                                         \
      حالت '\'': /* Ignore "@'" */ بشکن;                       \
      حالت '@': obstack_sgrow (&muscle_obstack, "@" ); بشکن;   \
      حالت '{': obstack_sgrow (&muscle_obstack, "[" ); بشکن;   \
      حالت '}': obstack_sgrow (&muscle_obstack, "]" ); بشکن;   \
      پیشفرض: aver (false); بشکن;                             \
      }                                                         \
    بشکن;                                                      \
  پیشفرض:                                                      \
    obstack_1grow (&muscle_obstack, *(مقدار_4));                  \
    بشکن;

/* Reverse of obstack_escape.  */
ایستا نویسه *
string_decode (نویسه ثابت *کلید)
{
  نویسه ثابت *مقدار = muscle_find_const (کلید);
  اگر (!مقدار)
    بازگشت پوچ;
  انجامبده {
    انتخاب (*مقدار)
      {
        COMMON_DECODE (مقدار)
        حالت '[':
        حالت ']':
          aver (false);
          بشکن;
      }
  } وقتی (*مقدار++);
  نویسه ثابت *مقدار_decoded = obstack_finish (&muscle_obstack);
  نویسه *res = xstrdup (مقدار_decoded);
  obstack_free (&muscle_obstack, مقدار_decoded);
  بازگشت res;
}

/* Reverse of muscle_location_grow.  */
ایستا location
location_decode (نویسه ثابت *مقدار)
{
  aver (مقدار);
  aver (*مقدار == '[');
  ++مقدار; aver (*مقدار == '[');
  location loc;
  وقتی (*++مقدار)
    انتخاب (*مقدار)
      {
        COMMON_DECODE (مقدار)
        حالت '[':
          aver (false);
          بشکن;
        حالت ']':
          ++مقدار; aver (*مقدار == ']');
          نویسه *boundary_str = obstack_finish0 (&muscle_obstack);
          انتخاب (*++مقدار)
            {
            حالت ',':
              boundary_set_from_string (&loc.start, boundary_str);
              obstack_free (&muscle_obstack, boundary_str);
              ++مقدار; aver (*مقدار == ' ');
              ++مقدار; aver (*مقدار == '[');
              ++مقدار; aver (*مقدار == '[');
              بشکن;
            حالت '\0':
              boundary_set_from_string (&loc.end, boundary_str);
              obstack_free (&muscle_obstack, boundary_str);
              بازگشت loc;
              بشکن;
            پیشفرض:
              aver (false);
              بشکن;
            }
          بشکن;
      }
  aver (false);
  بازگشت loc;
}

تهی
muscle_user_name_list_grow (نویسه ثابت *کلید, نویسه ثابت *user_name,
                            location loc)
{
  muscle_grow (کلید, "[[[[", ",", "");
  muscle_grow (کلید, user_name, "", "");
  muscle_grow (کلید, "]], ", "", "");
  muscle_location_grow (کلید, loc);
  muscle_grow (کلید, "]]", "", "");
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

ایستا
نویسه *
define_directive (نویسه ثابت *assignment,
                  muscle_kind kind,
                  نویسه ثابت *مقدار)
{
  نویسه *eq = strchr (assignment, '=');
  نویسه ثابت *fmt
    = eq || !مقدار || !*مقدار ? "%%define %s"
    : kind == muscle_code     ? "%%define %s {%s}"
    : kind == muscle_string   ? "%%define %s \"%s\""
    :                           "%%define %s %s";
  نویسه *res = xmalloc (strlen (fmt) + strlen (assignment)
                       + (مقدار ? strlen (مقدار) : 0));
  sprintf (res, fmt, assignment, مقدار);
  eq = strchr (res, '=');
  اگر (eq)
    *eq = eq[1] ? ' ' : '\0';
  بازگشت res;
}

/** If the \a variable name is obsolete, return the name to use,
 * otherwise \a variable.  If the \a value is obsolete, update it too.
 *
 * Allocates the returned value if needed, otherwise the returned
 * value is exactly \a variable.  */
ایستا
نویسه ثابت *
muscle_percent_variable_update (نویسه ثابت *variable,
                                muscle_kind kind,
                                نویسه ثابت **مقدار,
                                نویسه **old, نویسه **upd)
{
  تعریفنوع ساختار
  {
    ثابت نویسه *obsolete;
    ثابت نویسه *updated;
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
    { پوچ, پوچ, -1, }
  };

  برای (conversion_type ثابت *c = conversion; c->obsolete; ++c)
    {
      نویسه ثابت *eq = strchr (c->obsolete, '=');
      اگر (eq
          ? (!strncmp (c->obsolete, variable, eq - c->obsolete)
             && STREQ (eq + 1, *مقدار))
          : STREQ (c->obsolete, variable))
        {
          /* Generate the deprecation warning. */
          *old = c->obsolete[0] == '%'
            ? xstrdup (c->obsolete)
            : define_directive (c->obsolete, kind, *مقدار);
          *upd = define_directive (c->updated, c->kind, *مقدار);
          /* Update the variable and its value.  */
          {
            نویسه *res = xstrdup (c->updated);
            نویسه *eq2 = strchr (res, '=');
            اگر (eq2)
              {
                *eq2 = '\0';
                *مقدار = eq2 + 1;
              }
            بازگشت res;
          }
        }
    }
  بازگشت variable;
}

تهی
muscle_percent_define_درج (نویسه ثابت *var, location variable_loc,
                              muscle_kind kind,
                              نویسه ثابت *مقدار,
                              muscle_percent_define_how how)
{
  /* Backward compatibility.  */
  نویسه *old = پوچ;
  نویسه *upd = پوچ;
  نویسه ثابت *variable
    = muscle_percent_variable_update (var, kind,
                                      &مقدار, &old, &upd);
  uniqstr name = muscle_name (variable, پوچ);
  uniqstr loc_name = muscle_name (variable, "loc");
  uniqstr syncline_name = muscle_name (variable, "syncline");
  uniqstr how_name = muscle_name (variable, "how");
  uniqstr kind_name = muscle_name (variable, "kind");

  /* Command-line options are processed before the grammar file.  */
  bool warned = false;
  اگر (how == MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE)
    {
      نویسه ثابت *current_مقدار = muscle_find_const (name);
      اگر (current_مقدار)
        {
          long l = strtol (muscle_find_const (how_name), پوچ, 10);
          muscle_percent_define_how how_old
            = 0 <= l && l <= INT_MAX ? l : INT_MAX;
          اگر (how_old == MUSCLE_PERCENT_DEFINE_F)
            برو end;
          /* If assigning the same value, make it a warning.  */
          warnings warn = STREQ (مقدار, current_مقدار) ? Wother : complaint;
          complain (&variable_loc, warn,
                    _("%%define variable %s redefined"),
                    quote (variable));
          location loc = muscle_percent_define_get_loc (variable);
          subcomplain (&loc, warn, _("previous definition"));
          fixits_register (&variable_loc, "");
          warned = true;
        }
    }

  اگر (!warned && old && upd)
    deprecated_directive (&variable_loc, old, upd);

  MUSCLE_درج_STRING (name, مقدار);
  muscle_درج (loc_name, "");
  muscle_location_grow (loc_name, variable_loc);
  muscle_درج (syncline_name, "");
  muscle_syncline_grow (syncline_name, variable_loc);
  muscle_user_name_list_grow ("percent_define_user_variables", variable,
                              variable_loc);
  MUSCLE_درج_INT (how_name, how);
  MUSCLE_درج_STRING (kind_name, muscle_kind_string (kind));
 end:
  آزاد (old);
  آزاد (upd);
  اگر (variable != var)
    آزاد ((نویسه *) variable);
}

/* This is used for backward compatibility, e.g., "%define api.pure"
   supersedes "%pure-parser".  */
تهی
muscle_percent_define_ensure (نویسه ثابت *variable, location loc,
                              bool مقدار)
{
  uniqstr name = muscle_name (variable, پوچ);
  نویسه ثابت *val = مقدار ? "" : "false";

  /* Don't complain is VARIABLE is already defined, but be sure to set
     its value to VAL.  */
  اگر (!muscle_find_const (name)
      || muscle_percent_define_flag_if (variable) != مقدار)
    muscle_percent_define_درج (variable, loc, muscle_keyword, val,
                                  MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
}

/* Mark %define VARIABLE as used.  */
ایستا تهی
muscle_percent_define_use (نویسه ثابت *variable)
{
  muscle_درج (muscle_name (variable, "bison_variables"), "");
}

/* The value of %define variable VARIABLE (corresponding to FIELD, if
   defined).  Do not register as used, but diagnose unset variables.  */

ایستا
نویسه ثابت *
muscle_percent_define_get_raw (نویسه ثابت *variable, نویسه ثابت *field)
{
  uniqstr name = muscle_name (variable, field);
  نویسه ثابت *res = muscle_find_const (name);
  اگر (!res)
    complain (پوچ, fatal, _("%s: undefined %%define variable %s"),
              "muscle_percent_define_get_raw", quote (variable));
  بازگشت res;
}

نویسه *
muscle_percent_define_get (نویسه ثابت *variable)
{
  uniqstr name = muscle_name (variable, پوچ);
  نویسه *مقدار = string_decode (name);
  اگر (!مقدار)
    مقدار = xstrdup ("");
  muscle_percent_define_use (variable);
  بازگشت مقدار;
}

/* The kind of VARIABLE.  An error if undefined.  */
ایستا muscle_kind
muscle_percent_define_get_kind (نویسه ثابت *variable)
{
  بازگشت muscle_kind_new (muscle_percent_define_get_raw (variable, "kind"));
}

/* Check the kind of VARIABLE.  An error if undefined.  */
ایستا تهی
muscle_percent_define_check_kind (نویسه ثابت *variable, muscle_kind kind)
{
  اگر (muscle_percent_define_get_kind (variable) != kind)
    {
      location loc = muscle_percent_define_get_loc (variable);
      انتخاب (kind)
        {
        حالت muscle_code:
          complain (&loc, Wdeprecated,
                    _("%%define variable '%s' requires '{...}' values"),
                    variable);
          بشکن;
        حالت muscle_keyword:
          complain (&loc, Wdeprecated,
                    _("%%define variable '%s' requires keyword values"),
                    variable);
          بشکن;
        حالت muscle_string:
          complain (&loc, Wdeprecated,
                    _("%%define variable '%s' requires '\"...\"' values"),
                    variable);
          بشکن;
        }
    }
}


location
muscle_percent_define_get_loc (نویسه ثابت *variable)
{
  بازگشت location_decode (muscle_percent_define_get_raw (variable, "loc"));
}

نویسه ثابت *
muscle_percent_define_get_syncline (نویسه ثابت *variable)
{
  بازگشت muscle_percent_define_get_raw (variable, "syncline");
}

bool
muscle_percent_define_ifdef (نویسه ثابت *variable)
{
  اگر (muscle_find_const (muscle_name (variable, پوچ)))
    {
      muscle_percent_define_use (variable);
      بازگشت true;
    }
  وگرنه
    بازگشت false;
}

bool
muscle_percent_define_flag_if (نویسه ثابت *variable)
{
  uniqstr invalid_boolean_name = muscle_name (variable, "invalid_boolean");
  bool res = false;

  اگر (muscle_percent_define_ifdef (variable))
    {
      نویسه *مقدار = muscle_percent_define_get (variable);
      muscle_percent_define_check_kind (variable, muscle_keyword);
      اگر (مقدار[0] == '\0' || STREQ (مقدار, "true"))
        res = true;
      وگرنه اگر (STREQ (مقدار, "false"))
        res = false;
      وگرنه اگر (!muscle_find_const (invalid_boolean_name))
        {
          muscle_درج (invalid_boolean_name, "");
          location loc = muscle_percent_define_get_loc (variable);
          complain (&loc, complaint,
                    _("invalid value for %%define Boolean variable %s"),
                    quote (variable));
        }
      آزاد (مقدار);
    }
  وگرنه
    complain (پوچ, fatal, _("%s: undefined %%define variable %s"),
              "muscle_percent_define_flag", quote (variable));

  بازگشت res;
}

تهی
muscle_percent_define_default (نویسه ثابت *variable, نویسه ثابت *مقدار)
{
  uniqstr name = muscle_name (variable, پوچ);
  اگر (!muscle_find_const (name))
    {
      MUSCLE_درج_STRING (name, مقدار);
      MUSCLE_درج_STRING (muscle_name (variable, "kind"), "keyword");
      {
        uniqstr loc_name = muscle_name (variable, "loc");
        location loc;
        loc.start.file = "<default value>";
        loc.start.سطر = -1;
        loc.start.column = -1;
        loc.start.byte = -1;
        loc.end = loc.start;
        muscle_درج (loc_name, "");
        muscle_location_grow (loc_name, loc);
      }
      muscle_درج (muscle_name (variable, "syncline"), "");
    }
}

تهی
muscle_percent_define_check_مقادیر (نویسه ثابت * ثابت *مقادیر)
{
  برای (; *مقادیر; ++مقادیر)
    {
      نویسه ثابت * ثابت *variablep = مقادیر;
      uniqstr name = muscle_name (*variablep, پوچ);
      نویسه *مقدار = string_decode (name);
      muscle_percent_define_check_kind (*variablep, muscle_keyword);
      اگر (مقدار)
        {
          برای (++مقادیر; *مقادیر; ++مقادیر)
            اگر (STREQ (مقدار, *مقادیر))
              بشکن;
          اگر (!*مقادیر)
            {
              location loc = muscle_percent_define_get_loc (*variablep);
              complain (&loc, complaint,
                        _("invalid value for %%define variable %s: %s"),
                        quote (*variablep), quote_n (1, مقدار));
              برای (مقادیر = variablep + 1; *مقادیر; ++مقادیر)
                subcomplain (&loc, complaint | no_caret | silent,
                             _("accepted value: %s"), quote (*مقادیر));
            }
          وگرنه
            وقتی (*مقادیر)
              ++مقادیر;
          آزاد (مقدار);
        }
      وگرنه
        complain (پوچ, fatal, _("%s: undefined %%define variable %s"),
                  "muscle_percent_define_check_values", quote (*variablep));
    }
}

تهی
muscle_percent_code_grow (نویسه ثابت *qualifier, location qualifier_loc,
                          نویسه ثابت *code, location code_loc)
{
  نویسه ثابت *name = UNIQSTR_CONCAT ("percent_code(", qualifier, ")");
  muscle_code_grow (name, code, code_loc);
  muscle_user_name_list_grow ("percent_code_user_qualifiers", qualifier,
                               qualifier_loc);
}


/*------------------------------------------------.
| Output the definition of ENTRY as a m4_define.  |
`------------------------------------------------*/

ایستا درونخطی bool
muscle_m4_output (muscle_entry *entry, FILE *out)
{
  fprintf (out,
           "m4_define([b4_%s],\n"
           "[[%s]])\n\n\n", entry->کلید, entry->مقدار);
  بازگشت true;
}

ایستا bool
muscle_m4_output_processor (تهی *entry, تهی *out)
{
  بازگشت muscle_m4_output (entry, out);
}


تهی
muscles_m4_output (FILE *out)
{
  hash_do_for_each (muscle_table, muscle_m4_output_processor, out);
}
