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
muscle_kind_new (वर्ण अचर *k)
{
  यदि (STREQ (k, "code"))
    फर्काऊ muscle_code;
  अन्यथा यदि (STREQ (k, "keyword"))
    फर्काऊ muscle_keyword;
  अन्यथा यदि (STREQ (k, "string"))
    फर्काऊ muscle_string;
  पात ();
}

वर्ण अचर *
muscle_kind_string (muscle_kind k)
{
  विकल्प (k)
    {
    पक्ष muscle_code:    फर्काऊ "code";
    पक्ष muscle_keyword: फर्काऊ "keyword";
    पक्ष muscle_string:  फर्काऊ "string";
    }
  पात ();
}


/* A key-value pair, along with storage that can be reclaimed when
   this pair is no longer needed.  */
प्रकारनिर्देश संरचना
{
  वर्ण अचर *साँचो;
  वर्ण अचर *मान;
  वर्ण *storage;
  muscle_kind kind;
} muscle_entry;


/* The name of muscle for the %define variable VAR (corresponding to
   FIELD, if defined).  */
स्थिर uniqstr
muscle_name (वर्ण अचर *var, वर्ण अचर *field)
{
  यदि (field)
    फर्काऊ UNIQSTR_CONCAT ("percent_define_", field, "(", var, ")");
  अन्यथा
    फर्काऊ UNIQSTR_CONCAT ("percent_define(", var, ")");
}

/* An obstack used to create some entries.  */
संरचना obstack muscle_obstack;

/* Initial capacity of muscles hash table.  */
#define HT_INITIAL_CAPACITY 257

स्थिर संरचना hash_table *muscle_table = रिक्त;

स्थिर bool
hash_compare_muscles (शून्य अचर *x, शून्य अचर *y)
{
  muscle_entry अचर *m1 = x;
  muscle_entry अचर *m2 = y;
  फर्काऊ STREQ (m1->साँचो, m2->साँचो);
}

स्थिर माप_प्रकार
hash_muscle (अचर शून्य *x, माप_प्रकार tablesize)
{
  muscle_entry अचर *m = x;
  फर्काऊ hash_string (m->साँचो, tablesize);
}

/* Create a fresh muscle name KEY, and insert in the hash table.  */
स्थिर शून्य *
muscle_entry_new (वर्ण अचर *साँचो)
{
  muscle_entry *res = xmalloc (परिमाण *res);
  res->साँचो = साँचो;
  res->मान = रिक्त;
  res->storage = रिक्त;
  hash_xinsert (muscle_table, res);
  फर्काऊ res;
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
muscle_lookup (वर्ण अचर *साँचो)
{
  muscle_entry probe;
  probe.साँचो = साँचो;
  फर्काऊ hash_lookup (muscle_table, &probe);
}


शून्य
muscle_घुसाउ (वर्ण अचर *साँचो, वर्ण अचर *मान)
{
  muscle_entry *entry = muscle_lookup (साँचो);
  यदि (entry)
    मुक्त (entry->storage);
  अन्यथा
    /* First insertion in the hash. */
    entry = muscle_entry_new (साँचो);
  entry->मान = मान;
  entry->storage = रिक्त;
}


/* Append VALUE to the current value of KEY.  If KEY did not already
   exist, create it.  Use MUSCLE_OBSTACK.  De-allocate the previously
   associated value.  Copy VALUE and SEPARATOR.  If VALUE does not end
   with TERMINATOR, append one.  */

स्थिर शून्य
muscle_grow (अचर वर्ण *साँचो, अचर वर्ण *val,
             अचर वर्ण *separator, अचर वर्ण *terminator)
{
  muscle_entry *entry = muscle_lookup (साँचो);
  यदि (entry)
    {
      obstack_sgrow (&muscle_obstack, entry->मान);
      obstack_sgrow (&muscle_obstack, separator);
      मुक्त (entry->storage);
    }
  अन्यथा
    entry = muscle_entry_new (साँचो);

  obstack_sgrow (&muscle_obstack, val);

  माप_प्रकार vals = म_माप (val);
  माप_प्रकार terms = म_माप (terminator);
  यदि (terms <= vals
      && STRNEQ (val + vals - terms, terminator))
    obstack_sgrow (&muscle_obstack, terminator);

  {
    वर्ण अचर *new_val = obstack_finish0 (&muscle_obstack);
    entry->मान = entry->storage = xstrdup (new_val);
    obstack_free (&muscle_obstack, new_val);
  }
}

/*------------------------------------------------------------------.
| Using muscle_grow, append a synchronization line for the location |
| LOC to the current value of KEY.                                  |
`------------------------------------------------------------------*/

स्थिर शून्य
muscle_syncline_grow (वर्ण अचर *साँचो, location loc)
{
  obstack_printf (&muscle_obstack, "]b4_syncline(%d, ", loc.start.पङ्क्ति);
  obstack_quote (&muscle_obstack,
                 quotearg_style (c_quoting_style, map_file_name (loc.start.file)));
  obstack_sgrow (&muscle_obstack, ")dnl\n[");
  वर्ण अचर *extension = obstack_finish0 (&muscle_obstack);
  muscle_grow (साँचो, extension, "", "");
  obstack_free (&muscle_obstack, extension);
}

/*------------------------------------------------------------------.
| Append VALUE to the current value of KEY, using muscle_grow.  But |
| in addition, issue a synchronization line for the location LOC    |
| using muscle_syncline_grow.                                       |
`------------------------------------------------------------------*/

शून्य
muscle_code_grow (अचर वर्ण *साँचो, अचर वर्ण *val, location loc)
{
  muscle_syncline_grow (साँचो, loc);
  muscle_grow (साँचो, val, "", "\n");
}


शून्य
muscle_pair_list_grow (अचर वर्ण *muscle,
                       अचर वर्ण *a1, अचर वर्ण *a2)
{
  obstack_sgrow (&muscle_obstack, "[");
  obstack_quote (&muscle_obstack, a1);
  obstack_sgrow (&muscle_obstack, ", ");
  obstack_quote (&muscle_obstack, a2);
  obstack_sgrow (&muscle_obstack, "]");
  वर्ण अचर *pair = obstack_finish0 (&muscle_obstack);
  muscle_grow (muscle, pair, ",\n", "");
  obstack_free (&muscle_obstack, pair);
}


वर्ण अचर *
muscle_find_const (वर्ण अचर *साँचो)
{
  muscle_entry *entry = muscle_lookup (साँचो);
  फर्काऊ entry ? entry->मान : रिक्त;
}


वर्ण *
muscle_find (वर्ण अचर *साँचो)
{
  muscle_entry *entry = muscle_lookup (साँचो);
  यदि (entry)
    {
      aver (entry->मान == entry->storage);
      फर्काऊ entry->storage;
    }
  फर्काऊ रिक्त;
}


/* In the format 'file_name:line.column', append BOUND to MUSCLE.  Use
   digraphs for special characters in the file name.  */

स्थिर शून्य
muscle_boundary_grow (वर्ण अचर *साँचो, boundary bound)
{
  obstack_sgrow  (&muscle_obstack, "[[");
  obstack_escape (&muscle_obstack, bound.file);
  obstack_printf (&muscle_obstack, ":%d.%d@@%d]]", bound.पङ्क्ति, bound.column, bound.byte);
  वर्ण अचर *extension = obstack_finish0 (&muscle_obstack);
  muscle_grow (साँचो, extension, "", "");
  obstack_free (&muscle_obstack, extension);
}


शून्य
muscle_location_grow (वर्ण अचर *साँचो, location loc)
{
  muscle_boundary_grow (साँचो, loc.start);
  muscle_grow (साँचो, "", ", ", "");
  muscle_boundary_grow (साँचो, loc.end);
}

#define COMMON_DECODE(मान_4)                                    \
  पक्ष '$':                                                     \
    ++(मान_4); aver (*(मान_4) == ']');                          \
    ++(मान_4); aver (*(मान_4) == '[');                          \
    obstack_sgrow (&muscle_obstack, "$");                       \
    रोक;                                                      \
  पक्ष '@':                                                     \
    विकल्प (*++(मान_4))                                         \
      {                                                         \
      पक्ष '\'': /* Ignore "@'" */ रोक;                       \
      पक्ष '@': obstack_sgrow (&muscle_obstack, "@" ); रोक;   \
      पक्ष '{': obstack_sgrow (&muscle_obstack, "[" ); रोक;   \
      पक्ष '}': obstack_sgrow (&muscle_obstack, "]" ); रोक;   \
      स्वभाव: aver (false); रोक;                             \
      }                                                         \
    रोक;                                                      \
  स्वभाव:                                                      \
    obstack_1grow (&muscle_obstack, *(मान_4));                  \
    रोक;

/* Reverse of obstack_escape.  */
स्थिर वर्ण *
string_decode (वर्ण अचर *साँचो)
{
  वर्ण अचर *मान = muscle_find_const (साँचो);
  यदि (!मान)
    फर्काऊ रिक्त;
  गर {
    विकल्प (*मान)
      {
        COMMON_DECODE (मान)
        पक्ष '[':
        पक्ष ']':
          aver (false);
          रोक;
      }
  } जबसम्म (*मान++);
  वर्ण अचर *मान_decoded = obstack_finish (&muscle_obstack);
  वर्ण *res = xstrdup (मान_decoded);
  obstack_free (&muscle_obstack, मान_decoded);
  फर्काऊ res;
}

/* Reverse of muscle_location_grow.  */
स्थिर location
location_decode (वर्ण अचर *मान)
{
  aver (मान);
  aver (*मान == '[');
  ++मान; aver (*मान == '[');
  location loc;
  जबसम्म (*++मान)
    विकल्प (*मान)
      {
        COMMON_DECODE (मान)
        पक्ष '[':
          aver (false);
          रोक;
        पक्ष ']':
          ++मान; aver (*मान == ']');
          वर्ण *boundary_str = obstack_finish0 (&muscle_obstack);
          विकल्प (*++मान)
            {
            पक्ष ',':
              boundary_set_from_string (&loc.start, boundary_str);
              obstack_free (&muscle_obstack, boundary_str);
              ++मान; aver (*मान == ' ');
              ++मान; aver (*मान == '[');
              ++मान; aver (*मान == '[');
              रोक;
            पक्ष '\0':
              boundary_set_from_string (&loc.end, boundary_str);
              obstack_free (&muscle_obstack, boundary_str);
              फर्काऊ loc;
              रोक;
            स्वभाव:
              aver (false);
              रोक;
            }
          रोक;
      }
  aver (false);
  फर्काऊ loc;
}

शून्य
muscle_user_name_list_grow (वर्ण अचर *साँचो, वर्ण अचर *user_name,
                            location loc)
{
  muscle_grow (साँचो, "[[[[", ",", "");
  muscle_grow (साँचो, user_name, "", "");
  muscle_grow (साँचो, "]], ", "", "");
  muscle_location_grow (साँचो, loc);
  muscle_grow (साँचो, "]]", "", "");
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
define_directive (वर्ण अचर *assignment,
                  muscle_kind kind,
                  वर्ण अचर *मान)
{
  वर्ण *eq = म_अक्षर (assignment, '=');
  वर्ण अचर *fmt
    = eq || !मान || !*मान ? "%%define %s"
    : kind == muscle_code     ? "%%define %s {%s}"
    : kind == muscle_string   ? "%%define %s \"%s\""
    :                           "%%define %s %s";
  वर्ण *res = xmalloc (म_माप (fmt) + म_माप (assignment)
                       + (मान ? म_माप (मान) : 0));
  प्र_लिखो (res, fmt, assignment, मान);
  eq = म_अक्षर (res, '=');
  यदि (eq)
    *eq = eq[1] ? ' ' : '\0';
  फर्काऊ res;
}

/** If the \a variable name is obsolete, return the name to use,
 * otherwise \a variable.  If the \a value is obsolete, update it too.
 *
 * Allocates the returned value if needed, otherwise the returned
 * value is exactly \a variable.  */
स्थिर
वर्ण अचर *
muscle_percent_variable_update (वर्ण अचर *variable,
                                muscle_kind kind,
                                वर्ण अचर **मान,
                                वर्ण **old, वर्ण **upd)
{
  प्रकारनिर्देश संरचना
  {
    अचर वर्ण *obsolete;
    अचर वर्ण *updated;
    muscle_kind kind;
  } conversion_type;
  अचर conversion_type conversion[] =
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

  लागि (conversion_type अचर *c = conversion; c->obsolete; ++c)
    {
      वर्ण अचर *eq = म_अक्षर (c->obsolete, '=');
      यदि (eq
          ? (!म_भेदन (c->obsolete, variable, eq - c->obsolete)
             && STREQ (eq + 1, *मान))
          : STREQ (c->obsolete, variable))
        {
          /* Generate the deprecation warning. */
          *old = c->obsolete[0] == '%'
            ? xstrdup (c->obsolete)
            : define_directive (c->obsolete, kind, *मान);
          *upd = define_directive (c->updated, c->kind, *मान);
          /* Update the variable and its value.  */
          {
            वर्ण *res = xstrdup (c->updated);
            वर्ण *eq2 = म_अक्षर (res, '=');
            यदि (eq2)
              {
                *eq2 = '\0';
                *मान = eq2 + 1;
              }
            फर्काऊ res;
          }
        }
    }
  फर्काऊ variable;
}

शून्य
muscle_percent_define_घुसाउ (वर्ण अचर *var, location variable_loc,
                              muscle_kind kind,
                              वर्ण अचर *मान,
                              muscle_percent_define_how how)
{
  /* Backward compatibility.  */
  वर्ण *old = रिक्त;
  वर्ण *upd = रिक्त;
  वर्ण अचर *variable
    = muscle_percent_variable_update (var, kind,
                                      &मान, &old, &upd);
  uniqstr name = muscle_name (variable, रिक्त);
  uniqstr loc_name = muscle_name (variable, "loc");
  uniqstr syncline_name = muscle_name (variable, "syncline");
  uniqstr how_name = muscle_name (variable, "how");
  uniqstr kind_name = muscle_name (variable, "kind");

  /* Command-line options are processed before the grammar file.  */
  bool warned = false;
  यदि (how == MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE)
    {
      वर्ण अचर *current_मान = muscle_find_const (name);
      यदि (current_मान)
        {
          दीर्घ l = म_से_दीर्घ (muscle_find_const (how_name), रिक्त, 10);
          muscle_percent_define_how how_old
            = 0 <= l && l <= पूर्णांक_उच्च ? l : पूर्णांक_उच्च;
          यदि (how_old == MUSCLE_PERCENT_DEFINE_F)
            गच्छ end;
          /* If assigning the same value, make it a warning.  */
          warnings warn = STREQ (मान, current_मान) ? Wother : complaint;
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

  MUSCLE_घुसाउ_STRING (name, मान);
  muscle_घुसाउ (loc_name, "");
  muscle_location_grow (loc_name, variable_loc);
  muscle_घुसाउ (syncline_name, "");
  muscle_syncline_grow (syncline_name, variable_loc);
  muscle_user_name_list_grow ("percent_define_user_variables", variable,
                              variable_loc);
  MUSCLE_घुसाउ_INT (how_name, how);
  MUSCLE_घुसाउ_STRING (kind_name, muscle_kind_string (kind));
 end:
  मुक्त (old);
  मुक्त (upd);
  यदि (variable != var)
    मुक्त ((वर्ण *) variable);
}

/* This is used for backward compatibility, e.g., "%define api.pure"
   supersedes "%pure-parser".  */
शून्य
muscle_percent_define_ensure (वर्ण अचर *variable, location loc,
                              bool मान)
{
  uniqstr name = muscle_name (variable, रिक्त);
  वर्ण अचर *val = मान ? "" : "false";

  /* Don't complain is VARIABLE is already defined, but be sure to set
     its value to VAL.  */
  यदि (!muscle_find_const (name)
      || muscle_percent_define_flag_if (variable) != मान)
    muscle_percent_define_घुसाउ (variable, loc, muscle_keyword, val,
                                  MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
}

/* Mark %define VARIABLE as used.  */
स्थिर शून्य
muscle_percent_define_use (वर्ण अचर *variable)
{
  muscle_घुसाउ (muscle_name (variable, "bison_variables"), "");
}

/* The value of %define variable VARIABLE (corresponding to FIELD, if
   defined).  Do not register as used, but diagnose unset variables.  */

स्थिर
वर्ण अचर *
muscle_percent_define_get_raw (वर्ण अचर *variable, वर्ण अचर *field)
{
  uniqstr name = muscle_name (variable, field);
  वर्ण अचर *res = muscle_find_const (name);
  यदि (!res)
    complain (रिक्त, fatal, _("%s: undefined %%define variable %s"),
              "muscle_percent_define_get_raw", quote (variable));
  फर्काऊ res;
}

वर्ण *
muscle_percent_define_get (वर्ण अचर *variable)
{
  uniqstr name = muscle_name (variable, रिक्त);
  वर्ण *मान = string_decode (name);
  यदि (!मान)
    मान = xstrdup ("");
  muscle_percent_define_use (variable);
  फर्काऊ मान;
}

/* The kind of VARIABLE.  An error if undefined.  */
स्थिर muscle_kind
muscle_percent_define_get_kind (वर्ण अचर *variable)
{
  फर्काऊ muscle_kind_new (muscle_percent_define_get_raw (variable, "kind"));
}

/* Check the kind of VARIABLE.  An error if undefined.  */
स्थिर शून्य
muscle_percent_define_check_kind (वर्ण अचर *variable, muscle_kind kind)
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
          रोक;
        पक्ष muscle_keyword:
          complain (&loc, Wdeprecated,
                    _("%%define variable '%s' requires keyword values"),
                    variable);
          रोक;
        पक्ष muscle_string:
          complain (&loc, Wdeprecated,
                    _("%%define variable '%s' requires '\"...\"' values"),
                    variable);
          रोक;
        }
    }
}


location
muscle_percent_define_get_loc (वर्ण अचर *variable)
{
  फर्काऊ location_decode (muscle_percent_define_get_raw (variable, "loc"));
}

वर्ण अचर *
muscle_percent_define_get_syncline (वर्ण अचर *variable)
{
  फर्काऊ muscle_percent_define_get_raw (variable, "syncline");
}

bool
muscle_percent_define_ifdef (वर्ण अचर *variable)
{
  यदि (muscle_find_const (muscle_name (variable, रिक्त)))
    {
      muscle_percent_define_use (variable);
      फर्काऊ true;
    }
  अन्यथा
    फर्काऊ false;
}

bool
muscle_percent_define_flag_if (वर्ण अचर *variable)
{
  uniqstr invalid_boolean_name = muscle_name (variable, "invalid_boolean");
  bool res = false;

  यदि (muscle_percent_define_ifdef (variable))
    {
      वर्ण *मान = muscle_percent_define_get (variable);
      muscle_percent_define_check_kind (variable, muscle_keyword);
      यदि (मान[0] == '\0' || STREQ (मान, "true"))
        res = true;
      अन्यथा यदि (STREQ (मान, "false"))
        res = false;
      अन्यथा यदि (!muscle_find_const (invalid_boolean_name))
        {
          muscle_घुसाउ (invalid_boolean_name, "");
          location loc = muscle_percent_define_get_loc (variable);
          complain (&loc, complaint,
                    _("invalid value for %%define Boolean variable %s"),
                    quote (variable));
        }
      मुक्त (मान);
    }
  अन्यथा
    complain (रिक्त, fatal, _("%s: undefined %%define variable %s"),
              "muscle_percent_define_flag", quote (variable));

  फर्काऊ res;
}

शून्य
muscle_percent_define_default (वर्ण अचर *variable, वर्ण अचर *मान)
{
  uniqstr name = muscle_name (variable, रिक्त);
  यदि (!muscle_find_const (name))
    {
      MUSCLE_घुसाउ_STRING (name, मान);
      MUSCLE_घुसाउ_STRING (muscle_name (variable, "kind"), "keyword");
      {
        uniqstr loc_name = muscle_name (variable, "loc");
        location loc;
        loc.start.file = "<default value>";
        loc.start.पङ्क्ति = -1;
        loc.start.column = -1;
        loc.start.byte = -1;
        loc.end = loc.start;
        muscle_घुसाउ (loc_name, "");
        muscle_location_grow (loc_name, loc);
      }
      muscle_घुसाउ (muscle_name (variable, "syncline"), "");
    }
}

शून्य
muscle_percent_define_check_मानहरू (वर्ण अचर * अचर *मानहरू)
{
  लागि (; *मानहरू; ++मानहरू)
    {
      वर्ण अचर * अचर *variablep = मानहरू;
      uniqstr name = muscle_name (*variablep, रिक्त);
      वर्ण *मान = string_decode (name);
      muscle_percent_define_check_kind (*variablep, muscle_keyword);
      यदि (मान)
        {
          लागि (++मानहरू; *मानहरू; ++मानहरू)
            यदि (STREQ (मान, *मानहरू))
              रोक;
          यदि (!*मानहरू)
            {
              location loc = muscle_percent_define_get_loc (*variablep);
              complain (&loc, complaint,
                        _("invalid value for %%define variable %s: %s"),
                        quote (*variablep), quote_n (1, मान));
              लागि (मानहरू = variablep + 1; *मानहरू; ++मानहरू)
                subcomplain (&loc, complaint | no_caret | silent,
                             _("accepted value: %s"), quote (*मानहरू));
            }
          अन्यथा
            जबसम्म (*मानहरू)
              ++मानहरू;
          मुक्त (मान);
        }
      अन्यथा
        complain (रिक्त, fatal, _("%s: undefined %%define variable %s"),
                  "muscle_percent_define_check_values", quote (*variablep));
    }
}

शून्य
muscle_percent_code_grow (वर्ण अचर *qualifier, location qualifier_loc,
                          वर्ण अचर *code, location code_loc)
{
  वर्ण अचर *name = UNIQSTR_CONCAT ("percent_code(", qualifier, ")");
  muscle_code_grow (name, code, code_loc);
  muscle_user_name_list_grow ("percent_code_user_qualifiers", qualifier,
                               qualifier_loc);
}


/*------------------------------------------------.
| Output the definition of ENTRY as a m4_define.  |
`------------------------------------------------*/

स्थिर अन्तःस्थ bool
muscle_m4_output (muscle_entry *entry, खाता *out)
{
  ख_लिखो (out,
           "m4_define([b4_%s],\n"
           "[[%s]])\n\n\n", entry->साँचो, entry->मान);
  फर्काऊ true;
}

स्थिर bool
muscle_m4_output_processor (शून्य *entry, शून्य *out)
{
  फर्काऊ muscle_m4_output (entry, out);
}


शून्य
muscles_m4_output (खाता *out)
{
  hash_do_for_each (muscle_table, muscle_m4_output_processor, out);
}
