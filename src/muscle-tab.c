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
muscle_kind_new (अक्षर स्थिरांक *k)
{
  जर (STREQ (k, "code"))
    परत muscle_code;
  अन्यथा जर (STREQ (k, "keyword"))
    परत muscle_keyword;
  अन्यथा जर (STREQ (k, "string"))
    परत muscle_string;
  पात ();
}

अक्षर स्थिरांक *
muscle_kind_string (muscle_kind k)
{
  निवड (k)
    {
    स्थिती muscle_code:    परत "code";
    स्थिती muscle_keyword: परत "keyword";
    स्थिती muscle_string:  परत "string";
    }
  पात ();
}


/* A key-value pair, along with storage that can be reclaimed when
   this pair is no longer needed.  */
प्रकारव्याख्या संरचना
{
  अक्षर स्थिरांक *किल्ली;
  अक्षर स्थिरांक *मूल्य;
  अक्षर *storage;
  muscle_kind kind;
} muscle_entry;


/* The name of muscle for the %define variable VAR (corresponding to
   FIELD, if defined).  */
स्थिर uniqstr
muscle_name (अक्षर स्थिरांक *var, अक्षर स्थिरांक *field)
{
  जर (field)
    परत UNIQSTR_CONCAT ("percent_define_", field, "(", var, ")");
  अन्यथा
    परत UNIQSTR_CONCAT ("percent_define(", var, ")");
}

/* An obstack used to create some entries.  */
संरचना obstack muscle_obstack;

/* Initial capacity of muscles hash table.  */
#define HT_INITIAL_CAPACITY 257

स्थिर संरचना hash_table *muscle_table = रिक्त;

स्थिर bool
hash_compare_muscles (शून्य स्थिरांक *x, शून्य स्थिरांक *y)
{
  muscle_entry स्थिरांक *m1 = x;
  muscle_entry स्थिरांक *m2 = y;
  परत STREQ (m1->किल्ली, m2->किल्ली);
}

स्थिर माप_प्रकार
hash_muscle (स्थिरांक शून्य *x, माप_प्रकार tablesize)
{
  muscle_entry स्थिरांक *m = x;
  परत hash_string (m->किल्ली, tablesize);
}

/* Create a fresh muscle name KEY, and insert in the hash table.  */
स्थिर शून्य *
muscle_entry_new (अक्षर स्थिरांक *किल्ली)
{
  muscle_entry *res = xmalloc (आकार *res);
  res->किल्ली = किल्ली;
  res->मूल्य = रिक्त;
  res->storage = रिक्त;
  hash_xinsert (muscle_table, res);
  परत res;
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
muscle_lookup (अक्षर स्थिरांक *किल्ली)
{
  muscle_entry probe;
  probe.किल्ली = किल्ली;
  परत hash_lookup (muscle_table, &probe);
}


शून्य
muscle_घाला (अक्षर स्थिरांक *किल्ली, अक्षर स्थिरांक *मूल्य)
{
  muscle_entry *entry = muscle_lookup (किल्ली);
  जर (entry)
    मुक्त (entry->storage);
  अन्यथा
    /* First insertion in the hash. */
    entry = muscle_entry_new (किल्ली);
  entry->मूल्य = मूल्य;
  entry->storage = रिक्त;
}


/* Append VALUE to the current value of KEY.  If KEY did not already
   exist, create it.  Use MUSCLE_OBSTACK.  De-allocate the previously
   associated value.  Copy VALUE and SEPARATOR.  If VALUE does not end
   with TERMINATOR, append one.  */

स्थिर शून्य
muscle_grow (स्थिरांक अक्षर *किल्ली, स्थिरांक अक्षर *val,
             स्थिरांक अक्षर *separator, स्थिरांक अक्षर *terminator)
{
  muscle_entry *entry = muscle_lookup (किल्ली);
  जर (entry)
    {
      obstack_sgrow (&muscle_obstack, entry->मूल्य);
      obstack_sgrow (&muscle_obstack, separator);
      मुक्त (entry->storage);
    }
  अन्यथा
    entry = muscle_entry_new (किल्ली);

  obstack_sgrow (&muscle_obstack, val);

  माप_प्रकार vals = म_माप (val);
  माप_प्रकार terms = म_माप (terminator);
  जर (terms <= vals
      && STRNEQ (val + vals - terms, terminator))
    obstack_sgrow (&muscle_obstack, terminator);

  {
    अक्षर स्थिरांक *new_val = obstack_finish0 (&muscle_obstack);
    entry->मूल्य = entry->storage = xstrdup (new_val);
    obstack_free (&muscle_obstack, new_val);
  }
}

/*------------------------------------------------------------------.
| Using muscle_grow, append a synchronization line for the location |
| LOC to the current value of KEY.                                  |
`------------------------------------------------------------------*/

स्थिर शून्य
muscle_syncline_grow (अक्षर स्थिरांक *किल्ली, location loc)
{
  obstack_printf (&muscle_obstack, "]b4_syncline(%d, ", loc.start.ओळ);
  obstack_quote (&muscle_obstack,
                 quotearg_style (c_quoting_style, map_file_name (loc.start.file)));
  obstack_sgrow (&muscle_obstack, ")dnl\n[");
  अक्षर स्थिरांक *extension = obstack_finish0 (&muscle_obstack);
  muscle_grow (किल्ली, extension, "", "");
  obstack_free (&muscle_obstack, extension);
}

/*------------------------------------------------------------------.
| Append VALUE to the current value of KEY, using muscle_grow.  But |
| in addition, issue a synchronization line for the location LOC    |
| using muscle_syncline_grow.                                       |
`------------------------------------------------------------------*/

शून्य
muscle_code_grow (स्थिरांक अक्षर *किल्ली, स्थिरांक अक्षर *val, location loc)
{
  muscle_syncline_grow (किल्ली, loc);
  muscle_grow (किल्ली, val, "", "\n");
}


शून्य
muscle_pair_list_grow (स्थिरांक अक्षर *muscle,
                       स्थिरांक अक्षर *a1, स्थिरांक अक्षर *a2)
{
  obstack_sgrow (&muscle_obstack, "[");
  obstack_quote (&muscle_obstack, a1);
  obstack_sgrow (&muscle_obstack, ", ");
  obstack_quote (&muscle_obstack, a2);
  obstack_sgrow (&muscle_obstack, "]");
  अक्षर स्थिरांक *pair = obstack_finish0 (&muscle_obstack);
  muscle_grow (muscle, pair, ",\n", "");
  obstack_free (&muscle_obstack, pair);
}


अक्षर स्थिरांक *
muscle_find_const (अक्षर स्थिरांक *किल्ली)
{
  muscle_entry *entry = muscle_lookup (किल्ली);
  परत entry ? entry->मूल्य : रिक्त;
}


अक्षर *
muscle_find (अक्षर स्थिरांक *किल्ली)
{
  muscle_entry *entry = muscle_lookup (किल्ली);
  जर (entry)
    {
      aver (entry->मूल्य == entry->storage);
      परत entry->storage;
    }
  परत रिक्त;
}


/* In the format 'file_name:line.column', append BOUND to MUSCLE.  Use
   digraphs for special characters in the file name.  */

स्थिर शून्य
muscle_boundary_grow (अक्षर स्थिरांक *किल्ली, boundary bound)
{
  obstack_sgrow  (&muscle_obstack, "[[");
  obstack_escape (&muscle_obstack, bound.file);
  obstack_printf (&muscle_obstack, ":%d.%d@@%d]]", bound.ओळ, bound.column, bound.byte);
  अक्षर स्थिरांक *extension = obstack_finish0 (&muscle_obstack);
  muscle_grow (किल्ली, extension, "", "");
  obstack_free (&muscle_obstack, extension);
}


शून्य
muscle_location_grow (अक्षर स्थिरांक *किल्ली, location loc)
{
  muscle_boundary_grow (किल्ली, loc.start);
  muscle_grow (किल्ली, "", ", ", "");
  muscle_boundary_grow (किल्ली, loc.end);
}

#define COMMON_DECODE(मूल्य_4)                                    \
  स्थिती '$':                                                     \
    ++(मूल्य_4); aver (*(मूल्य_4) == ']');                          \
    ++(मूल्य_4); aver (*(मूल्य_4) == '[');                          \
    obstack_sgrow (&muscle_obstack, "$");                       \
    थांबा;                                                      \
  स्थिती '@':                                                     \
    निवड (*++(मूल्य_4))                                         \
      {                                                         \
      स्थिती '\'': /* Ignore "@'" */ थांबा;                       \
      स्थिती '@': obstack_sgrow (&muscle_obstack, "@" ); थांबा;   \
      स्थिती '{': obstack_sgrow (&muscle_obstack, "[" ); थांबा;   \
      स्थिती '}': obstack_sgrow (&muscle_obstack, "]" ); थांबा;   \
      पूर्वनिर्धारित: aver (false); थांबा;                             \
      }                                                         \
    थांबा;                                                      \
  पूर्वनिर्धारित:                                                      \
    obstack_1grow (&muscle_obstack, *(मूल्य_4));                  \
    थांबा;

/* Reverse of obstack_escape.  */
स्थिर अक्षर *
string_decode (अक्षर स्थिरांक *किल्ली)
{
  अक्षर स्थिरांक *मूल्य = muscle_find_const (किल्ली);
  जर (!मूल्य)
    परत रिक्त;
  करा {
    निवड (*मूल्य)
      {
        COMMON_DECODE (मूल्य)
        स्थिती '[':
        स्थिती ']':
          aver (false);
          थांबा;
      }
  } पर्यंत (*मूल्य++);
  अक्षर स्थिरांक *मूल्य_decoded = obstack_finish (&muscle_obstack);
  अक्षर *res = xstrdup (मूल्य_decoded);
  obstack_free (&muscle_obstack, मूल्य_decoded);
  परत res;
}

/* Reverse of muscle_location_grow.  */
स्थिर location
location_decode (अक्षर स्थिरांक *मूल्य)
{
  aver (मूल्य);
  aver (*मूल्य == '[');
  ++मूल्य; aver (*मूल्य == '[');
  location loc;
  पर्यंत (*++मूल्य)
    निवड (*मूल्य)
      {
        COMMON_DECODE (मूल्य)
        स्थिती '[':
          aver (false);
          थांबा;
        स्थिती ']':
          ++मूल्य; aver (*मूल्य == ']');
          अक्षर *boundary_str = obstack_finish0 (&muscle_obstack);
          निवड (*++मूल्य)
            {
            स्थिती ',':
              boundary_set_from_string (&loc.start, boundary_str);
              obstack_free (&muscle_obstack, boundary_str);
              ++मूल्य; aver (*मूल्य == ' ');
              ++मूल्य; aver (*मूल्य == '[');
              ++मूल्य; aver (*मूल्य == '[');
              थांबा;
            स्थिती '\0':
              boundary_set_from_string (&loc.end, boundary_str);
              obstack_free (&muscle_obstack, boundary_str);
              परत loc;
              थांबा;
            पूर्वनिर्धारित:
              aver (false);
              थांबा;
            }
          थांबा;
      }
  aver (false);
  परत loc;
}

शून्य
muscle_user_name_list_grow (अक्षर स्थिरांक *किल्ली, अक्षर स्थिरांक *user_name,
                            location loc)
{
  muscle_grow (किल्ली, "[[[[", ",", "");
  muscle_grow (किल्ली, user_name, "", "");
  muscle_grow (किल्ली, "]], ", "", "");
  muscle_location_grow (किल्ली, loc);
  muscle_grow (किल्ली, "]]", "", "");
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
अक्षर *
define_directive (अक्षर स्थिरांक *assignment,
                  muscle_kind kind,
                  अक्षर स्थिरांक *मूल्य)
{
  अक्षर *eq = म_अक्षर (assignment, '=');
  अक्षर स्थिरांक *fmt
    = eq || !मूल्य || !*मूल्य ? "%%define %s"
    : kind == muscle_code     ? "%%define %s {%s}"
    : kind == muscle_string   ? "%%define %s \"%s\""
    :                           "%%define %s %s";
  अक्षर *res = xmalloc (म_माप (fmt) + म_माप (assignment)
                       + (मूल्य ? म_माप (मूल्य) : 0));
  प्र_लिखो (res, fmt, assignment, मूल्य);
  eq = म_अक्षर (res, '=');
  जर (eq)
    *eq = eq[1] ? ' ' : '\0';
  परत res;
}

/** If the \a variable name is obsolete, return the name to use,
 * otherwise \a variable.  If the \a value is obsolete, update it too.
 *
 * Allocates the returned value if needed, otherwise the returned
 * value is exactly \a variable.  */
स्थिर
अक्षर स्थिरांक *
muscle_percent_variable_update (अक्षर स्थिरांक *variable,
                                muscle_kind kind,
                                अक्षर स्थिरांक **मूल्य,
                                अक्षर **old, अक्षर **upd)
{
  प्रकारव्याख्या संरचना
  {
    स्थिरांक अक्षर *obsolete;
    स्थिरांक अक्षर *updated;
    muscle_kind kind;
  } conversion_type;
  स्थिरांक conversion_type conversion[] =
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

  साठी (conversion_type स्थिरांक *c = conversion; c->obsolete; ++c)
    {
      अक्षर स्थिरांक *eq = म_अक्षर (c->obsolete, '=');
      जर (eq
          ? (!म_भेदन (c->obsolete, variable, eq - c->obsolete)
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
            अक्षर *res = xstrdup (c->updated);
            अक्षर *eq2 = म_अक्षर (res, '=');
            जर (eq2)
              {
                *eq2 = '\0';
                *मूल्य = eq2 + 1;
              }
            परत res;
          }
        }
    }
  परत variable;
}

शून्य
muscle_percent_define_घाला (अक्षर स्थिरांक *var, location variable_loc,
                              muscle_kind kind,
                              अक्षर स्थिरांक *मूल्य,
                              muscle_percent_define_how how)
{
  /* Backward compatibility.  */
  अक्षर *old = रिक्त;
  अक्षर *upd = रिक्त;
  अक्षर स्थिरांक *variable
    = muscle_percent_variable_update (var, kind,
                                      &मूल्य, &old, &upd);
  uniqstr name = muscle_name (variable, रिक्त);
  uniqstr loc_name = muscle_name (variable, "loc");
  uniqstr syncline_name = muscle_name (variable, "syncline");
  uniqstr how_name = muscle_name (variable, "how");
  uniqstr kind_name = muscle_name (variable, "kind");

  /* Command-line options are processed before the grammar file.  */
  bool warned = false;
  जर (how == MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE)
    {
      अक्षर स्थिरांक *current_मूल्य = muscle_find_const (name);
      जर (current_मूल्य)
        {
          दीर्घ l = म_से_दीर्घ (muscle_find_const (how_name), रिक्त, 10);
          muscle_percent_define_how how_old
            = 0 <= l && l <= पूर्णांक_उच्च ? l : पूर्णांक_उच्च;
          जर (how_old == MUSCLE_PERCENT_DEFINE_F)
            जा end;
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

  जर (!warned && old && upd)
    deprecated_directive (&variable_loc, old, upd);

  MUSCLE_घाला_STRING (name, मूल्य);
  muscle_घाला (loc_name, "");
  muscle_location_grow (loc_name, variable_loc);
  muscle_घाला (syncline_name, "");
  muscle_syncline_grow (syncline_name, variable_loc);
  muscle_user_name_list_grow ("percent_define_user_variables", variable,
                              variable_loc);
  MUSCLE_घाला_INT (how_name, how);
  MUSCLE_घाला_STRING (kind_name, muscle_kind_string (kind));
 end:
  मुक्त (old);
  मुक्त (upd);
  जर (variable != var)
    मुक्त ((अक्षर *) variable);
}

/* This is used for backward compatibility, e.g., "%define api.pure"
   supersedes "%pure-parser".  */
शून्य
muscle_percent_define_ensure (अक्षर स्थिरांक *variable, location loc,
                              bool मूल्य)
{
  uniqstr name = muscle_name (variable, रिक्त);
  अक्षर स्थिरांक *val = मूल्य ? "" : "false";

  /* Don't complain is VARIABLE is already defined, but be sure to set
     its value to VAL.  */
  जर (!muscle_find_const (name)
      || muscle_percent_define_flag_if (variable) != मूल्य)
    muscle_percent_define_घाला (variable, loc, muscle_keyword, val,
                                  MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
}

/* Mark %define VARIABLE as used.  */
स्थिर शून्य
muscle_percent_define_use (अक्षर स्थिरांक *variable)
{
  muscle_घाला (muscle_name (variable, "bison_variables"), "");
}

/* The value of %define variable VARIABLE (corresponding to FIELD, if
   defined).  Do not register as used, but diagnose unset variables.  */

स्थिर
अक्षर स्थिरांक *
muscle_percent_define_get_raw (अक्षर स्थिरांक *variable, अक्षर स्थिरांक *field)
{
  uniqstr name = muscle_name (variable, field);
  अक्षर स्थिरांक *res = muscle_find_const (name);
  जर (!res)
    complain (रिक्त, fatal, _("%s: undefined %%define variable %s"),
              "muscle_percent_define_get_raw", quote (variable));
  परत res;
}

अक्षर *
muscle_percent_define_get (अक्षर स्थिरांक *variable)
{
  uniqstr name = muscle_name (variable, रिक्त);
  अक्षर *मूल्य = string_decode (name);
  जर (!मूल्य)
    मूल्य = xstrdup ("");
  muscle_percent_define_use (variable);
  परत मूल्य;
}

/* The kind of VARIABLE.  An error if undefined.  */
स्थिर muscle_kind
muscle_percent_define_get_kind (अक्षर स्थिरांक *variable)
{
  परत muscle_kind_new (muscle_percent_define_get_raw (variable, "kind"));
}

/* Check the kind of VARIABLE.  An error if undefined.  */
स्थिर शून्य
muscle_percent_define_check_kind (अक्षर स्थिरांक *variable, muscle_kind kind)
{
  जर (muscle_percent_define_get_kind (variable) != kind)
    {
      location loc = muscle_percent_define_get_loc (variable);
      निवड (kind)
        {
        स्थिती muscle_code:
          complain (&loc, Wdeprecated,
                    _("%%define variable '%s' requires '{...}' values"),
                    variable);
          थांबा;
        स्थिती muscle_keyword:
          complain (&loc, Wdeprecated,
                    _("%%define variable '%s' requires keyword values"),
                    variable);
          थांबा;
        स्थिती muscle_string:
          complain (&loc, Wdeprecated,
                    _("%%define variable '%s' requires '\"...\"' values"),
                    variable);
          थांबा;
        }
    }
}


location
muscle_percent_define_get_loc (अक्षर स्थिरांक *variable)
{
  परत location_decode (muscle_percent_define_get_raw (variable, "loc"));
}

अक्षर स्थिरांक *
muscle_percent_define_get_syncline (अक्षर स्थिरांक *variable)
{
  परत muscle_percent_define_get_raw (variable, "syncline");
}

bool
muscle_percent_define_ifdef (अक्षर स्थिरांक *variable)
{
  जर (muscle_find_const (muscle_name (variable, रिक्त)))
    {
      muscle_percent_define_use (variable);
      परत true;
    }
  अन्यथा
    परत false;
}

bool
muscle_percent_define_flag_if (अक्षर स्थिरांक *variable)
{
  uniqstr invalid_boolean_name = muscle_name (variable, "invalid_boolean");
  bool res = false;

  जर (muscle_percent_define_ifdef (variable))
    {
      अक्षर *मूल्य = muscle_percent_define_get (variable);
      muscle_percent_define_check_kind (variable, muscle_keyword);
      जर (मूल्य[0] == '\0' || STREQ (मूल्य, "true"))
        res = true;
      अन्यथा जर (STREQ (मूल्य, "false"))
        res = false;
      अन्यथा जर (!muscle_find_const (invalid_boolean_name))
        {
          muscle_घाला (invalid_boolean_name, "");
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

  परत res;
}

शून्य
muscle_percent_define_default (अक्षर स्थिरांक *variable, अक्षर स्थिरांक *मूल्य)
{
  uniqstr name = muscle_name (variable, रिक्त);
  जर (!muscle_find_const (name))
    {
      MUSCLE_घाला_STRING (name, मूल्य);
      MUSCLE_घाला_STRING (muscle_name (variable, "kind"), "keyword");
      {
        uniqstr loc_name = muscle_name (variable, "loc");
        location loc;
        loc.start.file = "<default value>";
        loc.start.ओळ = -1;
        loc.start.column = -1;
        loc.start.byte = -1;
        loc.end = loc.start;
        muscle_घाला (loc_name, "");
        muscle_location_grow (loc_name, loc);
      }
      muscle_घाला (muscle_name (variable, "syncline"), "");
    }
}

शून्य
muscle_percent_define_check_मूल्ये (अक्षर स्थिरांक * स्थिरांक *मूल्ये)
{
  साठी (; *मूल्ये; ++मूल्ये)
    {
      अक्षर स्थिरांक * स्थिरांक *variablep = मूल्ये;
      uniqstr name = muscle_name (*variablep, रिक्त);
      अक्षर *मूल्य = string_decode (name);
      muscle_percent_define_check_kind (*variablep, muscle_keyword);
      जर (मूल्य)
        {
          साठी (++मूल्ये; *मूल्ये; ++मूल्ये)
            जर (STREQ (मूल्य, *मूल्ये))
              थांबा;
          जर (!*मूल्ये)
            {
              location loc = muscle_percent_define_get_loc (*variablep);
              complain (&loc, complaint,
                        _("invalid value for %%define variable %s: %s"),
                        quote (*variablep), quote_n (1, मूल्य));
              साठी (मूल्ये = variablep + 1; *मूल्ये; ++मूल्ये)
                subcomplain (&loc, complaint | no_caret | silent,
                             _("accepted value: %s"), quote (*मूल्ये));
            }
          अन्यथा
            पर्यंत (*मूल्ये)
              ++मूल्ये;
          मुक्त (मूल्य);
        }
      अन्यथा
        complain (रिक्त, fatal, _("%s: undefined %%define variable %s"),
                  "muscle_percent_define_check_values", quote (*variablep));
    }
}

शून्य
muscle_percent_code_grow (अक्षर स्थिरांक *qualifier, location qualifier_loc,
                          अक्षर स्थिरांक *code, location code_loc)
{
  अक्षर स्थिरांक *name = UNIQSTR_CONCAT ("percent_code(", qualifier, ")");
  muscle_code_grow (name, code, code_loc);
  muscle_user_name_list_grow ("percent_code_user_qualifiers", qualifier,
                               qualifier_loc);
}


/*------------------------------------------------.
| Output the definition of ENTRY as a m4_define.  |
`------------------------------------------------*/

स्थिर ओळीत bool
muscle_m4_output (muscle_entry *entry, खाता *out)
{
  ख_लिखो (out,
           "m4_define([b4_%s],\n"
           "[[%s]])\n\n\n", entry->किल्ली, entry->मूल्य);
  परत true;
}

स्थिर bool
muscle_m4_output_processor (शून्य *entry, शून्य *out)
{
  परत muscle_m4_output (entry, out);
}


शून्य
muscles_m4_output (खाता *out)
{
  hash_do_for_each (muscle_table, muscle_m4_output_processor, out);
}
