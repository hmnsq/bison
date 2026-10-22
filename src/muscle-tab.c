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

muscle_ډول
muscle_ډول_new (حرف ثابت *k)
{
  إذا (STREQ (k, "code"))
    أرجع muscle_کوډ;
  وإلا إذا (STREQ (k, "keyword"))
    أرجع muscle_keyword;
  وإلا إذا (STREQ (k, "string"))
    أرجع muscle_string;
  abort ();
}

حرف ثابت *
muscle_ډول_string (muscle_ډول k)
{
  بدل (k)
    {
    حالة muscle_کوډ:    أرجع "code";
    حالة muscle_keyword: أرجع "keyword";
    حالة muscle_string:  أرجع "string";
    }
  abort ();
}


/* A key-value pair, along with storage that can be reclaimed when
   this pair is no longer needed.  */
تعريفنوع بنية
{
  حرف ثابت *کلی;
  حرف ثابت *ارزښت;
  حرف *storage;
  muscle_ډول ډول;
} muscle_ثبت;


/* The name of muscle for the %define variable VAR (corresponding to
   FIELD, if defined).  */
ساكن uniqstr
muscle_name (حرف ثابت *var, حرف ثابت *ډګر)
{
  إذا (ډګر)
    أرجع UNIQSTR_CONCAT ("percent_define_", ډګر, "(", var, ")");
  وإلا
    أرجع UNIQSTR_CONCAT ("percent_define(", var, ")");
}

/* An obstack used to create some entries.  */
بنية obstack muscle_obstack;

/* Initial capacity of muscles hash table.  */
#تعریف_کړه HT_INITIAL_ظرفیت 257

ساكن بنية hash_جدول *muscle_جدول = عدم;

ساكن bool
hash_compare_muscles (فراغ ثابت *x, فراغ ثابت *y)
{
  muscle_ثبت ثابت *m1 = x;
  muscle_ثبت ثابت *m2 = y;
  أرجع STREQ (m1->کلی, m2->کلی);
}

ساكن size_t
hash_muscle (ثابت فراغ *x, size_t tablesize)
{
  muscle_ثبت ثابت *m = x;
  أرجع hash_string (m->کلی, tablesize);
}

/* Create a fresh muscle name KEY, and insert in the hash table.  */
ساكن فراغ *
muscle_ثبت_new (حرف ثابت *کلی)
{
  muscle_ثبت *res = xmalloc (حجم *res);
  res->کلی = کلی;
  res->ارزښت = عدم;
  res->storage = عدم;
  hash_xinsert (muscle_جدول, res);
  أرجع res;
}

ساكن فراغ
muscle_ثبت_free (فراغ *ثبت)
{
  muscle_ثبت *mentry = ثبت;
  حرر (mentry->storage);
  حرر (mentry);
}

فراغ
muscle_init (فراغ)
{
  /* Initialize the muscle obstack.  */
  obstack_init (&muscle_obstack);

  muscle_جدول = hash_xinitialize (HT_INITIAL_ظرفیت, عدم, hash_muscle,
                                   hash_compare_muscles, muscle_ثبت_free);
  /* Avoid warnings if the user defined this variable, but did not
     actually call -Wcex.  */
  حرر (muscle_percent_define_get ("cex.timeout"));
}


فراغ
muscle_free (فراغ)
{
  hash_free (muscle_جدول);
  obstack_free (&muscle_obstack, عدم);
}

/* Look for the muscle named KEY.  Return NULL if does not exist.  */
ساكن muscle_ثبت *
muscle_lookup (حرف ثابت *کلی)
{
  muscle_ثبت probe;
  probe.کلی = کلی;
  أرجع hash_lookup (muscle_جدول, &probe);
}


فراغ
muscle_ورننباسه (حرف ثابت *کلی, حرف ثابت *ارزښت)
{
  muscle_ثبت *ثبت = muscle_lookup (کلی);
  إذا (ثبت)
    حرر (ثبت->storage);
  وإلا
    /* First insertion in the hash. */
    ثبت = muscle_ثبت_new (کلی);
  ثبت->ارزښت = ارزښت;
  ثبت->storage = عدم;
}


/* Append VALUE to the current value of KEY.  If KEY did not already
   exist, create it.  Use MUSCLE_OBSTACK.  De-allocate the previously
   associated value.  Copy VALUE and SEPARATOR.  If VALUE does not end
   with TERMINATOR, append one.  */

ساكن فراغ
muscle_grow (ثابت حرف *کلی, ثابت حرف *val,
             ثابت حرف *separator, ثابت حرف *terminator)
{
  muscle_ثبت *ثبت = muscle_lookup (کلی);
  إذا (ثبت)
    {
      obstack_sgrow (&muscle_obstack, ثبت->ارزښت);
      obstack_sgrow (&muscle_obstack, separator);
      حرر (ثبت->storage);
    }
  وإلا
    ثبت = muscle_ثبت_new (کلی);

  obstack_sgrow (&muscle_obstack, val);

  size_t vals = مزي_اوږدوالی (val);
  size_t terms = مزي_اوږدوالی (terminator);
  إذا (terms <= vals
      && STRNEQ (val + vals - terms, terminator))
    obstack_sgrow (&muscle_obstack, terminator);

  {
    حرف ثابت *new_val = obstack_finish0 (&muscle_obstack);
    ثبت->ارزښت = ثبت->storage = xstrdup (new_val);
    obstack_free (&muscle_obstack, new_val);
  }
}

/*------------------------------------------------------------------.
| Using muscle_grow, append a synchronization line for the location |
| LOC to the current value of KEY.                                  |
`------------------------------------------------------------------*/

ساكن فراغ
muscle_syncline_grow (حرف ثابت *کلی, location loc)
{
  obstack_printf (&muscle_obstack, "]b4_syncline(%d, ", loc.پیل.کرښه);
  obstack_quote (&muscle_obstack,
                 quotearg_style (c_quoting_style, map_file_name (loc.پیل.file)));
  obstack_sgrow (&muscle_obstack, ")dnl\n[");
  حرف ثابت *extension = obstack_finish0 (&muscle_obstack);
  muscle_grow (کلی, extension, "", "");
  obstack_free (&muscle_obstack, extension);
}

/*------------------------------------------------------------------.
| Append VALUE to the current value of KEY, using muscle_grow.  But |
| in addition, issue a synchronization line for the location LOC    |
| using muscle_syncline_grow.                                       |
`------------------------------------------------------------------*/

فراغ
muscle_کوډ_grow (ثابت حرف *کلی, ثابت حرف *val, location loc)
{
  muscle_syncline_grow (کلی, loc);
  muscle_grow (کلی, val, "", "\n");
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
muscle_پیدا_کړه_const (حرف ثابت *کلی)
{
  muscle_ثبت *ثبت = muscle_lookup (کلی);
  أرجع ثبت ? ثبت->ارزښت : عدم;
}


حرف *
muscle_پیدا_کړه (حرف ثابت *کلی)
{
  muscle_ثبت *ثبت = muscle_lookup (کلی);
  إذا (ثبت)
    {
      aver (ثبت->ارزښت == ثبت->storage);
      أرجع ثبت->storage;
    }
  أرجع عدم;
}


/* In the format 'file_name:line.column', append BOUND to MUSCLE.  Use
   digraphs for special characters in the file name.  */

ساكن فراغ
muscle_boundary_grow (حرف ثابت *کلی, boundary bound)
{
  obstack_sgrow  (&muscle_obstack, "[[");
  obstack_escape (&muscle_obstack, bound.file);
  obstack_printf (&muscle_obstack, ":%d.%d@@%d]]", bound.کرښه, bound.ستنه, bound.byte);
  حرف ثابت *extension = obstack_finish0 (&muscle_obstack);
  muscle_grow (کلی, extension, "", "");
  obstack_free (&muscle_obstack, extension);
}


فراغ
muscle_location_grow (حرف ثابت *کلی, location loc)
{
  muscle_boundary_grow (کلی, loc.پیل);
  muscle_grow (کلی, "", ", ", "");
  muscle_boundary_grow (کلی, loc.پای_ټکی);
}

#تعریف_کړه COMMON_DECODE(ارزښت_4)                                    \
  حالة '$':                                                     \
    ++(ارزښت_4); aver (*(ارزښت_4) == ']');                          \
    ++(ارزښت_4); aver (*(ارزښت_4) == '[');                          \
    obstack_sgrow (&muscle_obstack, "$");                       \
    اكسر;                                                      \
  حالة '@':                                                     \
    بدل (*++(ارزښت_4))                                         \
      {                                                         \
      حالة '\'': /* Ignore "@'" */ اكسر;                       \
      حالة '@': obstack_sgrow (&muscle_obstack, "@" ); اكسر;   \
      حالة '{': obstack_sgrow (&muscle_obstack, "[" ); اكسر;   \
      حالة '}': obstack_sgrow (&muscle_obstack, "]" ); اكسر;   \
      افتراضي: aver (false); اكسر;                             \
      }                                                         \
    اكسر;                                                      \
  افتراضي:                                                      \
    obstack_1grow (&muscle_obstack, *(ارزښت_4));                  \
    اكسر;

/* Reverse of obstack_escape.  */
ساكن حرف *
string_decode (حرف ثابت *کلی)
{
  حرف ثابت *ارزښت = muscle_پیدا_کړه_const (کلی);
  إذا (!ارزښت)
    أرجع عدم;
  افعل {
    بدل (*ارزښت)
      {
        COMMON_DECODE (ارزښت)
        حالة '[':
        حالة ']':
          aver (false);
          اكسر;
      }
  } طالما (*ارزښت++);
  حرف ثابت *ارزښت_decoded = obstack_پای (&muscle_obstack);
  حرف *res = xstrdup (ارزښت_decoded);
  obstack_free (&muscle_obstack, ارزښت_decoded);
  أرجع res;
}

/* Reverse of muscle_location_grow.  */
ساكن location
location_decode (حرف ثابت *ارزښت)
{
  aver (ارزښت);
  aver (*ارزښت == '[');
  ++ارزښت; aver (*ارزښت == '[');
  location loc;
  طالما (*++ارزښت)
    بدل (*ارزښت)
      {
        COMMON_DECODE (ارزښت)
        حالة '[':
          aver (false);
          اكسر;
        حالة ']':
          ++ارزښت; aver (*ارزښت == ']');
          حرف *boundary_str = obstack_finish0 (&muscle_obstack);
          بدل (*++ارزښت)
            {
            حالة ',':
              boundary_سېټ_from_string (&loc.پیل, boundary_str);
              obstack_free (&muscle_obstack, boundary_str);
              ++ارزښت; aver (*ارزښت == ' ');
              ++ارزښت; aver (*ارزښت == '[');
              ++ارزښت; aver (*ارزښت == '[');
              اكسر;
            حالة '\0':
              boundary_سېټ_from_string (&loc.پای_ټکی, boundary_str);
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
muscle_user_name_list_grow (حرف ثابت *کلی, حرف ثابت *user_name,
                            location loc)
{
  muscle_grow (کلی, "[[[[", ",", "");
  muscle_grow (کلی, user_name, "", "");
  muscle_grow (کلی, "]], ", "", "");
  muscle_location_grow (کلی, loc);
  muscle_grow (کلی, "]]", "", "");
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
                  muscle_ډول ډول,
                  حرف ثابت *ارزښت)
{
  حرف *eq = strchr (assignment, '=');
  حرف ثابت *fmt
    = eq || !ارزښت || !*ارزښت ? "%%define %s"
    : ډول == muscle_کوډ     ? "%%define %s {%s}"
    : ډول == muscle_string   ? "%%define %s \"%s\""
    :                           "%%define %s %s";
  حرف *res = xmalloc (مزي_اوږدوالی (fmt) + مزي_اوږدوالی (assignment)
                       + (ارزښت ? مزي_اوږدوالی (ارزښت) : 0));
  sprintf (res, fmt, assignment, ارزښت);
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
                                muscle_ډول ډول,
                                حرف ثابت **ارزښت,
                                حرف **old, حرف **upd)
{
  تعريفنوع بنية
  {
    ثابت حرف *obsolete;
    ثابت حرف *updated;
    muscle_ډول ډول;
  } conversion_type;
  ثابت conversion_type conversion[] =
  {
    { "%error-verbose",             "parse.error=verbose",       muscle_keyword },
    { "%error_verbose",             "parse.error=verbose",       muscle_keyword },
    { "abstract",                   "api.parser.abstract",       muscle_keyword },
    { "annotations",                "api.parser.annotations",    muscle_کوډ },
    { "api.push_pull",              "api.push-pull",             muscle_keyword },
    { "api.tokens.prefix",          "api.token.prefix",          muscle_کوډ },
    { "extends",                    "api.parser.extends",        muscle_keyword },
    { "filename_type",              "api.filename.type",         muscle_کوډ },
    { "final",                      "api.parser.final",          muscle_keyword },
    { "implements",                 "api.parser.implements",     muscle_keyword },
    { "lex_symbol",                 "api.token.constructor",     -1 },
    { "location_type",              "api.location.type",         muscle_کوډ },
    { "lr.default-reductions",      "lr.default-reduction",      muscle_keyword },
    { "lr.keep-unreachable-states", "lr.keep-unreachable-state", muscle_keyword },
    { "lr.keep_unreachable_states", "lr.keep-unreachable-state", muscle_keyword },
    { "namespace",                  "api.namespace",             muscle_کوډ },
    { "package",                    "api.package",               muscle_کوډ },
    { "parser_class_name",          "api.parser.class",          muscle_کوډ },
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
          ? (!مزي_پرتله (c->obsolete, variable, eq - c->obsolete)
             && STREQ (eq + 1, *ارزښت))
          : STREQ (c->obsolete, variable))
        {
          /* Generate the deprecation warning. */
          *old = c->obsolete[0] == '%'
            ? xstrdup (c->obsolete)
            : define_directive (c->obsolete, ډول, *ارزښت);
          *upd = define_directive (c->updated, c->ډول, *ارزښت);
          /* Update the variable and its value.  */
          {
            حرف *res = xstrdup (c->updated);
            حرف *eq2 = strchr (res, '=');
            إذا (eq2)
              {
                *eq2 = '\0';
                *ارزښت = eq2 + 1;
              }
            أرجع res;
          }
        }
    }
  أرجع variable;
}

فراغ
muscle_percent_define_ورننباسه (حرف ثابت *var, location variable_loc,
                              muscle_ډول ډول,
                              حرف ثابت *ارزښت,
                              muscle_percent_define_how how)
{
  /* Backward compatibility.  */
  حرف *old = عدم;
  حرف *upd = عدم;
  حرف ثابت *variable
    = muscle_percent_variable_update (var, ډول,
                                      &ارزښت, &old, &upd);
  uniqstr name = muscle_name (variable, عدم);
  uniqstr loc_name = muscle_name (variable, "loc");
  uniqstr syncline_name = muscle_name (variable, "syncline");
  uniqstr how_name = muscle_name (variable, "how");
  uniqstr ډول_name = muscle_name (variable, "kind");

  /* Command-line options are processed before the grammar file.  */
  bool warned = false;
  إذا (how == MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE)
    {
      حرف ثابت *current_ارزښت = muscle_پیدا_کړه_const (name);
      إذا (current_ارزښت)
        {
          اوږد l = strtol (muscle_پیدا_کړه_const (how_name), عدم, 10);
          muscle_percent_define_how how_old
            = 0 <= l && l <= لوی_عدد ? l : لوی_عدد;
          إذا (how_old == MUSCLE_PERCENT_DEFINE_F)
            اذهب پای_ټکی;
          /* If assigning the same value, make it a warning.  */
          warnings warn = STREQ (ارزښت, current_ارزښت) ? Wother : complaint;
          complain (&variable_loc, warn,
                    _("%%define variable %s redefined"),
                    quote (variable));
          location loc = muscle_percent_define_get_loc (variable);
          subcomplain (&loc, warn, _("previous definition"));
          fixits_ثبتوونکی (&variable_loc, "");
          warned = true;
        }
    }

  إذا (!warned && old && upd)
    deprecated_directive (&variable_loc, old, upd);

  MUSCLE_ورننباسه_STRING (name, ارزښت);
  muscle_ورننباسه (loc_name, "");
  muscle_location_grow (loc_name, variable_loc);
  muscle_ورننباسه (syncline_name, "");
  muscle_syncline_grow (syncline_name, variable_loc);
  muscle_user_name_list_grow ("percent_define_user_variables", variable,
                              variable_loc);
  MUSCLE_ورننباسه_INT (how_name, how);
  MUSCLE_ورننباسه_STRING (ډول_name, muscle_ډول_string (ډول));
 پای_ټکی:
  حرر (old);
  حرر (upd);
  إذا (variable != var)
    حرر ((حرف *) variable);
}

/* This is used for backward compatibility, e.g., "%define api.pure"
   supersedes "%pure-parser".  */
فراغ
muscle_percent_define_ensure (حرف ثابت *variable, location loc,
                              bool ارزښت)
{
  uniqstr name = muscle_name (variable, عدم);
  حرف ثابت *val = ارزښت ? "" : "false";

  /* Don't complain is VARIABLE is already defined, but be sure to set
     its value to VAL.  */
  إذا (!muscle_پیدا_کړه_const (name)
      || muscle_percent_define_flag_if (variable) != ارزښت)
    muscle_percent_define_ورننباسه (variable, loc, muscle_keyword, val,
                                  MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
}

/* Mark %define VARIABLE as used.  */
ساكن فراغ
muscle_percent_define_use (حرف ثابت *variable)
{
  muscle_ورننباسه (muscle_name (variable, "bison_variables"), "");
}

/* The value of %define variable VARIABLE (corresponding to FIELD, if
   defined).  Do not register as used, but diagnose unset variables.  */

ساكن
حرف ثابت *
muscle_percent_define_get_raw (حرف ثابت *variable, حرف ثابت *ډګر)
{
  uniqstr name = muscle_name (variable, ډګر);
  حرف ثابت *res = muscle_پیدا_کړه_const (name);
  إذا (!res)
    complain (عدم, fatal, _("%s: undefined %%define variable %s"),
              "muscle_percent_define_get_raw", quote (variable));
  أرجع res;
}

حرف *
muscle_percent_define_get (حرف ثابت *variable)
{
  uniqstr name = muscle_name (variable, عدم);
  حرف *ارزښت = string_decode (name);
  إذا (!ارزښت)
    ارزښت = xstrdup ("");
  muscle_percent_define_use (variable);
  أرجع ارزښت;
}

/* The kind of VARIABLE.  An error if undefined.  */
ساكن muscle_ډول
muscle_percent_define_get_ډول (حرف ثابت *variable)
{
  أرجع muscle_ډول_new (muscle_percent_define_get_raw (variable, "kind"));
}

/* Check the kind of VARIABLE.  An error if undefined.  */
ساكن فراغ
muscle_percent_define_check_ډول (حرف ثابت *variable, muscle_ډول ډول)
{
  إذا (muscle_percent_define_get_ډول (variable) != ډول)
    {
      location loc = muscle_percent_define_get_loc (variable);
      بدل (ډول)
        {
        حالة muscle_کوډ:
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
  إذا (muscle_پیدا_کړه_const (muscle_name (variable, عدم)))
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
      حرف *ارزښت = muscle_percent_define_get (variable);
      muscle_percent_define_check_ډول (variable, muscle_keyword);
      إذا (ارزښت[0] == '\0' || STREQ (ارزښت, "true"))
        res = true;
      وإلا إذا (STREQ (ارزښت, "false"))
        res = false;
      وإلا إذا (!muscle_پیدا_کړه_const (invalid_boolean_name))
        {
          muscle_ورننباسه (invalid_boolean_name, "");
          location loc = muscle_percent_define_get_loc (variable);
          complain (&loc, complaint,
                    _("invalid value for %%define Boolean variable %s"),
                    quote (variable));
        }
      حرر (ارزښت);
    }
  وإلا
    complain (عدم, fatal, _("%s: undefined %%define variable %s"),
              "muscle_percent_define_flag", quote (variable));

  أرجع res;
}

فراغ
muscle_percent_define_default (حرف ثابت *variable, حرف ثابت *ارزښت)
{
  uniqstr name = muscle_name (variable, عدم);
  إذا (!muscle_پیدا_کړه_const (name))
    {
      MUSCLE_ورننباسه_STRING (name, ارزښت);
      MUSCLE_ورننباسه_STRING (muscle_name (variable, "kind"), "keyword");
      {
        uniqstr loc_name = muscle_name (variable, "loc");
        location loc;
        loc.پیل.file = "<default value>";
        loc.پیل.کرښه = -1;
        loc.پیل.ستنه = -1;
        loc.پیل.byte = -1;
        loc.پای_ټکی = loc.پیل;
        muscle_ورننباسه (loc_name, "");
        muscle_location_grow (loc_name, loc);
      }
      muscle_ورننباسه (muscle_name (variable, "syncline"), "");
    }
}

فراغ
muscle_percent_define_check_ارزښتونه (حرف ثابت * ثابت *ارزښتونه)
{
  لكل (; *ارزښتونه; ++ارزښتونه)
    {
      حرف ثابت * ثابت *variablep = ارزښتونه;
      uniqstr name = muscle_name (*variablep, عدم);
      حرف *ارزښت = string_decode (name);
      muscle_percent_define_check_ډول (*variablep, muscle_keyword);
      إذا (ارزښت)
        {
          لكل (++ارزښتونه; *ارزښتونه; ++ارزښتونه)
            إذا (STREQ (ارزښت, *ارزښتونه))
              اكسر;
          إذا (!*ارزښتونه)
            {
              location loc = muscle_percent_define_get_loc (*variablep);
              complain (&loc, complaint,
                        _("invalid value for %%define variable %s: %s"),
                        quote (*variablep), quote_n (1, ارزښت));
              لكل (ارزښتونه = variablep + 1; *ارزښتونه; ++ارزښتونه)
                subcomplain (&loc, complaint | no_caret | silent,
                             _("accepted value: %s"), quote (*ارزښتونه));
            }
          وإلا
            طالما (*ارزښتونه)
              ++ارزښتونه;
          حرر (ارزښت);
        }
      وإلا
        complain (عدم, fatal, _("%s: undefined %%define variable %s"),
                  "muscle_percent_define_check_values", quote (*variablep));
    }
}

فراغ
muscle_percent_کوډ_grow (حرف ثابت *qualifier, location qualifier_loc,
                          حرف ثابت *کوډ, location کوډ_loc)
{
  حرف ثابت *name = UNIQSTR_CONCAT ("percent_code(", qualifier, ")");
  muscle_کوډ_grow (name, کوډ, کوډ_loc);
  muscle_user_name_list_grow ("percent_code_user_qualifiers", qualifier,
                               qualifier_loc);
}


/*------------------------------------------------.
| Output the definition of ENTRY as a m4_define.  |
`------------------------------------------------*/

ساكن مضمن bool
muscle_m4_output (muscle_ثبت *ثبت, FILE *بهر)
{
  fprintf (بهر,
           "m4_define([b4_%s],\n"
           "[[%s]])\n\n\n", ثبت->کلی, ثبت->ارزښت);
  أرجع true;
}

ساكن bool
muscle_m4_output_processor (فراغ *ثبت, فراغ *بهر)
{
  أرجع muscle_m4_output (ثبت, بهر);
}


فراغ
muscles_m4_output (FILE *بهر)
{
  hash_do_for_each (muscle_جدول, muscle_m4_output_processor, بهر);
}
