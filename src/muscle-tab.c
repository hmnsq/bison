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

muscle_قسم
muscle_قسم_new (حرف ثابت *k)
{
  إذا (STREQ (k, "code"))
    أرجع muscle_ڪوڊ;
  وإلا إذا (STREQ (k, "keyword"))
    أرجع muscle_keyword;
  وإلا إذا (STREQ (k, "string"))
    أرجع muscle_string;
  abort ();
}

حرف ثابت *
muscle_قسم_string (muscle_قسم k)
{
  بدل (k)
    {
    حالة muscle_ڪوڊ:    أرجع "code";
    حالة muscle_keyword: أرجع "keyword";
    حالة muscle_string:  أرجع "string";
    }
  abort ();
}


/* A key-value pair, along with storage that can be reclaimed when
   this pair is no longer needed.  */
تعريفنوع بنية
{
  حرف ثابت *ڪنجي;
  حرف ثابت *قدر;
  حرف *storage;
  muscle_قسم قسم;
} muscle_داخلا;


/* The name of muscle for the %define variable VAR (corresponding to
   FIELD, if defined).  */
ساكن uniqstr
muscle_name (حرف ثابت *var, حرف ثابت *ميدان)
{
  إذا (ميدان)
    أرجع UNIQSTR_CONCAT ("percent_define_", ميدان, "(", var, ")");
  وإلا
    أرجع UNIQSTR_CONCAT ("percent_define(", var, ")");
}

/* An obstack used to create some entries.  */
بنية obstack muscle_obstack;

/* Initial capacity of muscles hash table.  */
#وصف_ڪريو HT_INITIAL_صلاحيت 257

ساكن بنية hash_جدول *muscle_جدول = عدم;

ساكن bool
hash_compare_muscles (فراغ ثابت *x, فراغ ثابت *y)
{
  muscle_داخلا ثابت *m1 = x;
  muscle_داخلا ثابت *m2 = y;
  أرجع STREQ (m1->ڪنجي, m2->ڪنجي);
}

ساكن size_t
hash_muscle (ثابت فراغ *x, size_t tablesize)
{
  muscle_داخلا ثابت *m = x;
  أرجع hash_string (m->ڪنجي, tablesize);
}

/* Create a fresh muscle name KEY, and insert in the hash table.  */
ساكن فراغ *
muscle_داخلا_new (حرف ثابت *ڪنجي)
{
  muscle_داخلا *res = xmalloc (حجم *res);
  res->ڪنجي = ڪنجي;
  res->قدر = عدم;
  res->storage = عدم;
  hash_xinsert (muscle_جدول, res);
  أرجع res;
}

ساكن فراغ
muscle_داخلا_free (فراغ *داخلا_2)
{
  muscle_داخلا *mentry = داخلا_2;
  حرر (mentry->storage);
  حرر (mentry);
}

فراغ
muscle_init (فراغ)
{
  /* Initialize the muscle obstack.  */
  obstack_init (&muscle_obstack);

  muscle_جدول = hash_xinitialize (HT_INITIAL_صلاحيت, عدم, hash_muscle,
                                   hash_compare_muscles, muscle_داخلا_free);
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
ساكن muscle_داخلا *
muscle_lookup (حرف ثابت *ڪنجي)
{
  muscle_داخلا probe;
  probe.ڪنجي = ڪنجي;
  أرجع hash_lookup (muscle_جدول, &probe);
}


فراغ
muscle_وجهو (حرف ثابت *ڪنجي, حرف ثابت *قدر)
{
  muscle_داخلا *داخلا_2 = muscle_lookup (ڪنجي);
  إذا (داخلا_2)
    حرر (داخلا_2->storage);
  وإلا
    /* First insertion in the hash. */
    داخلا_2 = muscle_داخلا_new (ڪنجي);
  داخلا_2->قدر = قدر;
  داخلا_2->storage = عدم;
}


/* Append VALUE to the current value of KEY.  If KEY did not already
   exist, create it.  Use MUSCLE_OBSTACK.  De-allocate the previously
   associated value.  Copy VALUE and SEPARATOR.  If VALUE does not end
   with TERMINATOR, append one.  */

ساكن فراغ
muscle_grow (ثابت حرف *ڪنجي, ثابت حرف *val,
             ثابت حرف *separator, ثابت حرف *terminator)
{
  muscle_داخلا *داخلا_2 = muscle_lookup (ڪنجي);
  إذا (داخلا_2)
    {
      obstack_sgrow (&muscle_obstack, داخلا_2->قدر);
      obstack_sgrow (&muscle_obstack, separator);
      حرر (داخلا_2->storage);
    }
  وإلا
    داخلا_2 = muscle_داخلا_new (ڪنجي);

  obstack_sgrow (&muscle_obstack, val);

  size_t vals = لڙي_ڊيگهه (val);
  size_t terms = لڙي_ڊيگهه (terminator);
  إذا (terms <= vals
      && STRNEQ (val + vals - terms, terminator))
    obstack_sgrow (&muscle_obstack, terminator);

  {
    حرف ثابت *new_val = obstack_finish0 (&muscle_obstack);
    داخلا_2->قدر = داخلا_2->storage = xstrdup (new_val);
    obstack_free (&muscle_obstack, new_val);
  }
}

/*------------------------------------------------------------------.
| Using muscle_grow, append a synchronization line for the location |
| LOC to the current value of KEY.                                  |
`------------------------------------------------------------------*/

ساكن فراغ
muscle_syncline_grow (حرف ثابت *ڪنجي, location loc)
{
  obstack_printf (&muscle_obstack, "]b4_syncline(%d, ", loc.شروعات.سٽ);
  obstack_quote (&muscle_obstack,
                 quotearg_style (c_quoting_style, map_file_name (loc.شروعات.file)));
  obstack_sgrow (&muscle_obstack, ")dnl\n[");
  حرف ثابت *extension = obstack_finish0 (&muscle_obstack);
  muscle_grow (ڪنجي, extension, "", "");
  obstack_free (&muscle_obstack, extension);
}

/*------------------------------------------------------------------.
| Append VALUE to the current value of KEY, using muscle_grow.  But |
| in addition, issue a synchronization line for the location LOC    |
| using muscle_syncline_grow.                                       |
`------------------------------------------------------------------*/

فراغ
muscle_ڪوڊ_grow (ثابت حرف *ڪنجي, ثابت حرف *val, location loc)
{
  muscle_syncline_grow (ڪنجي, loc);
  muscle_grow (ڪنجي, val, "", "\n");
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
muscle_ڳوليو_const (حرف ثابت *ڪنجي)
{
  muscle_داخلا *داخلا_2 = muscle_lookup (ڪنجي);
  أرجع داخلا_2 ? داخلا_2->قدر : عدم;
}


حرف *
muscle_ڳوليو (حرف ثابت *ڪنجي)
{
  muscle_داخلا *داخلا_2 = muscle_lookup (ڪنجي);
  إذا (داخلا_2)
    {
      aver (داخلا_2->قدر == داخلا_2->storage);
      أرجع داخلا_2->storage;
    }
  أرجع عدم;
}


/* In the format 'file_name:line.column', append BOUND to MUSCLE.  Use
   digraphs for special characters in the file name.  */

ساكن فراغ
muscle_boundary_grow (حرف ثابت *ڪنجي, boundary bound)
{
  obstack_sgrow  (&muscle_obstack, "[[");
  obstack_escape (&muscle_obstack, bound.file);
  obstack_printf (&muscle_obstack, ":%d.%d@@%d]]", bound.سٽ, bound.ڪالم, bound.byte);
  حرف ثابت *extension = obstack_finish0 (&muscle_obstack);
  muscle_grow (ڪنجي, extension, "", "");
  obstack_free (&muscle_obstack, extension);
}


فراغ
muscle_location_grow (حرف ثابت *ڪنجي, location loc)
{
  muscle_boundary_grow (ڪنجي, loc.شروعات);
  muscle_grow (ڪنجي, "", ", ", "");
  muscle_boundary_grow (ڪنجي, loc.پڇاڙي);
}

#وصف_ڪريو COMMON_DECODE(قدر_4)                                    \
  حالة '$':                                                     \
    ++(قدر_4); aver (*(قدر_4) == ']');                          \
    ++(قدر_4); aver (*(قدر_4) == '[');                          \
    obstack_sgrow (&muscle_obstack, "$");                       \
    اكسر;                                                      \
  حالة '@':                                                     \
    بدل (*++(قدر_4))                                         \
      {                                                         \
      حالة '\'': /* Ignore "@'" */ اكسر;                       \
      حالة '@': obstack_sgrow (&muscle_obstack, "@" ); اكسر;   \
      حالة '{': obstack_sgrow (&muscle_obstack, "[" ); اكسر;   \
      حالة '}': obstack_sgrow (&muscle_obstack, "]" ); اكسر;   \
      افتراضي: aver (false); اكسر;                             \
      }                                                         \
    اكسر;                                                      \
  افتراضي:                                                      \
    obstack_1grow (&muscle_obstack, *(قدر_4));                  \
    اكسر;

/* Reverse of obstack_escape.  */
ساكن حرف *
string_decode (حرف ثابت *ڪنجي)
{
  حرف ثابت *قدر = muscle_ڳوليو_const (ڪنجي);
  إذا (!قدر)
    أرجع عدم;
  افعل {
    بدل (*قدر)
      {
        COMMON_DECODE (قدر)
        حالة '[':
        حالة ']':
          aver (false);
          اكسر;
      }
  } طالما (*قدر++);
  حرف ثابت *قدر_decoded = obstack_پڄاڻي (&muscle_obstack);
  حرف *res = xstrdup (قدر_decoded);
  obstack_free (&muscle_obstack, قدر_decoded);
  أرجع res;
}

/* Reverse of muscle_location_grow.  */
ساكن location
location_decode (حرف ثابت *قدر)
{
  aver (قدر);
  aver (*قدر == '[');
  ++قدر; aver (*قدر == '[');
  location loc;
  طالما (*++قدر)
    بدل (*قدر)
      {
        COMMON_DECODE (قدر)
        حالة '[':
          aver (false);
          اكسر;
        حالة ']':
          ++قدر; aver (*قدر == ']');
          حرف *boundary_str = obstack_finish0 (&muscle_obstack);
          بدل (*++قدر)
            {
            حالة ',':
              boundary_سيٽ_from_string (&loc.شروعات, boundary_str);
              obstack_free (&muscle_obstack, boundary_str);
              ++قدر; aver (*قدر == ' ');
              ++قدر; aver (*قدر == '[');
              ++قدر; aver (*قدر == '[');
              اكسر;
            حالة '\0':
              boundary_سيٽ_from_string (&loc.پڇاڙي, boundary_str);
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
muscle_user_name_list_grow (حرف ثابت *ڪنجي, حرف ثابت *user_name,
                            location loc)
{
  muscle_grow (ڪنجي, "[[[[", ",", "");
  muscle_grow (ڪنجي, user_name, "", "");
  muscle_grow (ڪنجي, "]], ", "", "");
  muscle_location_grow (ڪنجي, loc);
  muscle_grow (ڪنجي, "]]", "", "");
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
                  muscle_قسم قسم,
                  حرف ثابت *قدر)
{
  حرف *eq = strchr (assignment, '=');
  حرف ثابت *fmt
    = eq || !قدر || !*قدر ? "%%define %s"
    : قسم == muscle_ڪوڊ     ? "%%define %s {%s}"
    : قسم == muscle_string   ? "%%define %s \"%s\""
    :                           "%%define %s %s";
  حرف *res = xmalloc (لڙي_ڊيگهه (fmt) + لڙي_ڊيگهه (assignment)
                       + (قدر ? لڙي_ڊيگهه (قدر) : 0));
  sprintf (res, fmt, assignment, قدر);
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
                                muscle_قسم قسم,
                                حرف ثابت **قدر,
                                حرف **old, حرف **upd)
{
  تعريفنوع بنية
  {
    ثابت حرف *obsolete;
    ثابت حرف *updated;
    muscle_قسم قسم;
  } conversion_type;
  ثابت conversion_type conversion[] =
  {
    { "%error-verbose",             "parse.error=verbose",       muscle_keyword },
    { "%error_verbose",             "parse.error=verbose",       muscle_keyword },
    { "abstract",                   "api.parser.abstract",       muscle_keyword },
    { "annotations",                "api.parser.annotations",    muscle_ڪوڊ },
    { "api.push_pull",              "api.push-pull",             muscle_keyword },
    { "api.tokens.prefix",          "api.token.prefix",          muscle_ڪوڊ },
    { "extends",                    "api.parser.extends",        muscle_keyword },
    { "filename_type",              "api.filename.type",         muscle_ڪوڊ },
    { "final",                      "api.parser.final",          muscle_keyword },
    { "implements",                 "api.parser.implements",     muscle_keyword },
    { "lex_symbol",                 "api.token.constructor",     -1 },
    { "location_type",              "api.location.type",         muscle_ڪوڊ },
    { "lr.default-reductions",      "lr.default-reduction",      muscle_keyword },
    { "lr.keep-unreachable-states", "lr.keep-unreachable-state", muscle_keyword },
    { "lr.keep_unreachable_states", "lr.keep-unreachable-state", muscle_keyword },
    { "namespace",                  "api.namespace",             muscle_ڪوڊ },
    { "package",                    "api.package",               muscle_ڪوڊ },
    { "parser_class_name",          "api.parser.class",          muscle_ڪوڊ },
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
          ? (!لڙي_ڀيٽ (c->obsolete, variable, eq - c->obsolete)
             && STREQ (eq + 1, *قدر))
          : STREQ (c->obsolete, variable))
        {
          /* Generate the deprecation warning. */
          *old = c->obsolete[0] == '%'
            ? xstrdup (c->obsolete)
            : define_directive (c->obsolete, قسم, *قدر);
          *upd = define_directive (c->updated, c->قسم, *قدر);
          /* Update the variable and its value.  */
          {
            حرف *res = xstrdup (c->updated);
            حرف *eq2 = strchr (res, '=');
            إذا (eq2)
              {
                *eq2 = '\0';
                *قدر = eq2 + 1;
              }
            أرجع res;
          }
        }
    }
  أرجع variable;
}

فراغ
muscle_percent_define_وجهو (حرف ثابت *var, location variable_loc,
                              muscle_قسم قسم,
                              حرف ثابت *قدر,
                              muscle_percent_define_how how)
{
  /* Backward compatibility.  */
  حرف *old = عدم;
  حرف *upd = عدم;
  حرف ثابت *variable
    = muscle_percent_variable_update (var, قسم,
                                      &قدر, &old, &upd);
  uniqstr name = muscle_name (variable, عدم);
  uniqstr loc_name = muscle_name (variable, "loc");
  uniqstr syncline_name = muscle_name (variable, "syncline");
  uniqstr how_name = muscle_name (variable, "how");
  uniqstr قسم_name = muscle_name (variable, "kind");

  /* Command-line options are processed before the grammar file.  */
  bool warned = false;
  إذا (how == MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE)
    {
      حرف ثابت *current_قدر = muscle_ڳوليو_const (name);
      إذا (current_قدر)
        {
          ڊگهو l = strtol (muscle_ڳوليو_const (how_name), عدم, 10);
          muscle_percent_define_how how_old
            = 0 <= l && l <= وڌ_کان_وڌ_عدد ? l : وڌ_کان_وڌ_عدد;
          إذا (how_old == MUSCLE_PERCENT_DEFINE_F)
            اذهب پڇاڙي;
          /* If assigning the same value, make it a warning.  */
          warnings warn = STREQ (قدر, current_قدر) ? Wother : complaint;
          complain (&variable_loc, warn,
                    _("%%define variable %s redefined"),
                    quote (variable));
          location loc = muscle_percent_define_get_loc (variable);
          subcomplain (&loc, warn, _("previous definition"));
          fixits_رجسٽر (&variable_loc, "");
          warned = true;
        }
    }

  إذا (!warned && old && upd)
    deprecated_directive (&variable_loc, old, upd);

  MUSCLE_وجهو_STRING (name, قدر);
  muscle_وجهو (loc_name, "");
  muscle_location_grow (loc_name, variable_loc);
  muscle_وجهو (syncline_name, "");
  muscle_syncline_grow (syncline_name, variable_loc);
  muscle_user_name_list_grow ("percent_define_user_variables", variable,
                              variable_loc);
  MUSCLE_وجهو_INT (how_name, how);
  MUSCLE_وجهو_STRING (قسم_name, muscle_قسم_string (قسم));
 پڇاڙي:
  حرر (old);
  حرر (upd);
  إذا (variable != var)
    حرر ((حرف *) variable);
}

/* This is used for backward compatibility, e.g., "%define api.pure"
   supersedes "%pure-parser".  */
فراغ
muscle_percent_define_ensure (حرف ثابت *variable, location loc,
                              bool قدر)
{
  uniqstr name = muscle_name (variable, عدم);
  حرف ثابت *val = قدر ? "" : "false";

  /* Don't complain is VARIABLE is already defined, but be sure to set
     its value to VAL.  */
  إذا (!muscle_ڳوليو_const (name)
      || muscle_percent_define_flag_if (variable) != قدر)
    muscle_percent_define_وجهو (variable, loc, muscle_keyword, val,
                                  MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
}

/* Mark %define VARIABLE as used.  */
ساكن فراغ
muscle_percent_define_use (حرف ثابت *variable)
{
  muscle_وجهو (muscle_name (variable, "bison_variables"), "");
}

/* The value of %define variable VARIABLE (corresponding to FIELD, if
   defined).  Do not register as used, but diagnose unset variables.  */

ساكن
حرف ثابت *
muscle_percent_define_get_raw (حرف ثابت *variable, حرف ثابت *ميدان)
{
  uniqstr name = muscle_name (variable, ميدان);
  حرف ثابت *res = muscle_ڳوليو_const (name);
  إذا (!res)
    complain (عدم, fatal, _("%s: undefined %%define variable %s"),
              "muscle_percent_define_get_raw", quote (variable));
  أرجع res;
}

حرف *
muscle_percent_define_get (حرف ثابت *variable)
{
  uniqstr name = muscle_name (variable, عدم);
  حرف *قدر = string_decode (name);
  إذا (!قدر)
    قدر = xstrdup ("");
  muscle_percent_define_use (variable);
  أرجع قدر;
}

/* The kind of VARIABLE.  An error if undefined.  */
ساكن muscle_قسم
muscle_percent_define_get_قسم (حرف ثابت *variable)
{
  أرجع muscle_قسم_new (muscle_percent_define_get_raw (variable, "kind"));
}

/* Check the kind of VARIABLE.  An error if undefined.  */
ساكن فراغ
muscle_percent_define_check_قسم (حرف ثابت *variable, muscle_قسم قسم)
{
  إذا (muscle_percent_define_get_قسم (variable) != قسم)
    {
      location loc = muscle_percent_define_get_loc (variable);
      بدل (قسم)
        {
        حالة muscle_ڪوڊ:
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
  إذا (muscle_ڳوليو_const (muscle_name (variable, عدم)))
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
      حرف *قدر = muscle_percent_define_get (variable);
      muscle_percent_define_check_قسم (variable, muscle_keyword);
      إذا (قدر[0] == '\0' || STREQ (قدر, "true"))
        res = true;
      وإلا إذا (STREQ (قدر, "false"))
        res = false;
      وإلا إذا (!muscle_ڳوليو_const (invalid_boolean_name))
        {
          muscle_وجهو (invalid_boolean_name, "");
          location loc = muscle_percent_define_get_loc (variable);
          complain (&loc, complaint,
                    _("invalid value for %%define Boolean variable %s"),
                    quote (variable));
        }
      حرر (قدر);
    }
  وإلا
    complain (عدم, fatal, _("%s: undefined %%define variable %s"),
              "muscle_percent_define_flag", quote (variable));

  أرجع res;
}

فراغ
muscle_percent_define_default (حرف ثابت *variable, حرف ثابت *قدر)
{
  uniqstr name = muscle_name (variable, عدم);
  إذا (!muscle_ڳوليو_const (name))
    {
      MUSCLE_وجهو_STRING (name, قدر);
      MUSCLE_وجهو_STRING (muscle_name (variable, "kind"), "keyword");
      {
        uniqstr loc_name = muscle_name (variable, "loc");
        location loc;
        loc.شروعات.file = "<default value>";
        loc.شروعات.سٽ = -1;
        loc.شروعات.ڪالم = -1;
        loc.شروعات.byte = -1;
        loc.پڇاڙي = loc.شروعات;
        muscle_وجهو (loc_name, "");
        muscle_location_grow (loc_name, loc);
      }
      muscle_وجهو (muscle_name (variable, "syncline"), "");
    }
}

فراغ
muscle_percent_define_check_قدرون (حرف ثابت * ثابت *قدرون)
{
  لكل (; *قدرون; ++قدرون)
    {
      حرف ثابت * ثابت *variablep = قدرون;
      uniqstr name = muscle_name (*variablep, عدم);
      حرف *قدر = string_decode (name);
      muscle_percent_define_check_قسم (*variablep, muscle_keyword);
      إذا (قدر)
        {
          لكل (++قدرون; *قدرون; ++قدرون)
            إذا (STREQ (قدر, *قدرون))
              اكسر;
          إذا (!*قدرون)
            {
              location loc = muscle_percent_define_get_loc (*variablep);
              complain (&loc, complaint,
                        _("invalid value for %%define variable %s: %s"),
                        quote (*variablep), quote_n (1, قدر));
              لكل (قدرون = variablep + 1; *قدرون; ++قدرون)
                subcomplain (&loc, complaint | no_caret | silent,
                             _("accepted value: %s"), quote (*قدرون));
            }
          وإلا
            طالما (*قدرون)
              ++قدرون;
          حرر (قدر);
        }
      وإلا
        complain (عدم, fatal, _("%s: undefined %%define variable %s"),
                  "muscle_percent_define_check_values", quote (*variablep));
    }
}

فراغ
muscle_percent_ڪوڊ_grow (حرف ثابت *qualifier, location qualifier_loc,
                          حرف ثابت *ڪوڊ, location ڪوڊ_loc)
{
  حرف ثابت *name = UNIQSTR_CONCAT ("percent_code(", qualifier, ")");
  muscle_ڪوڊ_grow (name, ڪوڊ, ڪوڊ_loc);
  muscle_user_name_list_grow ("percent_code_user_qualifiers", qualifier,
                               qualifier_loc);
}


/*------------------------------------------------.
| Output the definition of ENTRY as a m4_define.  |
`------------------------------------------------*/

ساكن مضمن bool
muscle_m4_output (muscle_داخلا *داخلا_2, FILE *ٻاهريون)
{
  fprintf (ٻاهريون,
           "m4_define([b4_%s],\n"
           "[[%s]])\n\n\n", داخلا_2->ڪنجي, داخلا_2->قدر);
  أرجع true;
}

ساكن bool
muscle_m4_output_processor (فراغ *داخلا_2, فراغ *ٻاهريون)
{
  أرجع muscle_m4_output (داخلا_2, ٻاهريون);
}


فراغ
muscles_m4_output (FILE *ٻاهريون)
{
  hash_do_for_each (muscle_جدول, muscle_m4_output_processor, ٻاهريون);
}
