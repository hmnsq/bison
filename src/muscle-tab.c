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

#समावेश <config.h>
#समावेश "system.h"

#समावेश <hash.h>
#समावेश <quote.h>

#समावेश "complain.h"
#समावेश "files.h"
#समावेश "fixits.h"
#समावेश "getargs.h"
#समावेश "muscle-tab.h"

muscle_kind
muscle_kind_new (अक्षर स्थिर *k)
{
  अगर (STREQ (k, "code"))
    वापस muscle_code;
  अन्यथा अगर (STREQ (k, "keyword"))
    वापस muscle_keyword;
  अन्यथा अगर (STREQ (k, "string"))
    वापस muscle_string;
  पात ();
}

अक्षर स्थिर *
muscle_kind_string (muscle_kind k)
{
  चयन (k)
    {
    हाल muscle_code:    वापस "code";
    हाल muscle_keyword: वापस "keyword";
    हाल muscle_string:  वापस "string";
    }
  पात ();
}


/* A key-value pair, along with storage that can be reclaimed when
   this pair is no longer needed.  */
प्रकार काष्ठा
{
  अक्षर स्थिर *कुंजी;
  अक्षर स्थिर *मूल्य;
  अक्षर *storage;
  muscle_kind kind;
} muscle_entry;


/* The name of muscle for the %define variable VAR (corresponding to
   FIELD, if defined).  */
अटल uniqstr
muscle_name (अक्षर स्थिर *var, अक्षर स्थिर *field)
{
  अगर (field)
    वापस UNIQSTR_CONCAT ("percent_define_", field, "(", var, ")");
  अन्यथा
    वापस UNIQSTR_CONCAT ("percent_define(", var, ")");
}

/* An obstack used to create some entries.  */
काष्ठा obstack muscle_obstack;

/* Initial capacity of muscles hash table.  */
#घोषणा HT_INITIAL_CAPACITY 257

अटल काष्ठा hash_table *muscle_table = शून्य;

अटल bool
hash_compare_muscles (व्योम स्थिर *x, व्योम स्थिर *y)
{
  muscle_entry स्थिर *m1 = x;
  muscle_entry स्थिर *m2 = y;
  वापस STREQ (m1->कुंजी, m2->कुंजी);
}

अटल माप_प्रकार
hash_muscle (स्थिर व्योम *x, माप_प्रकार tablesize)
{
  muscle_entry स्थिर *m = x;
  वापस hash_string (m->कुंजी, tablesize);
}

/* Create a fresh muscle name KEY, and insert in the hash table.  */
अटल व्योम *
muscle_entry_new (अक्षर स्थिर *कुंजी)
{
  muscle_entry *res = xmalloc (माप *res);
  res->कुंजी = कुंजी;
  res->मूल्य = शून्य;
  res->storage = शून्य;
  hash_xinsert (muscle_table, res);
  वापस res;
}

अटल व्योम
muscle_entry_free (व्योम *entry)
{
  muscle_entry *mentry = entry;
  मुक्त (mentry->storage);
  मुक्त (mentry);
}

व्योम
muscle_init (व्योम)
{
  /* Initialize the muscle obstack.  */
  obstack_init (&muscle_obstack);

  muscle_table = hash_xinitialize (HT_INITIAL_CAPACITY, शून्य, hash_muscle,
                                   hash_compare_muscles, muscle_entry_free);
  /* Avoid warnings if the user defined this variable, but did not
     actually call -Wcex.  */
  मुक्त (muscle_percent_define_get ("cex.timeout"));
}


व्योम
muscle_free (व्योम)
{
  hash_free (muscle_table);
  obstack_free (&muscle_obstack, शून्य);
}

/* Look for the muscle named KEY.  Return NULL if does not exist.  */
अटल muscle_entry *
muscle_lookup (अक्षर स्थिर *कुंजी)
{
  muscle_entry probe;
  probe.कुंजी = कुंजी;
  वापस hash_lookup (muscle_table, &probe);
}


व्योम
muscle_जोड़ो (अक्षर स्थिर *कुंजी, अक्षर स्थिर *मूल्य)
{
  muscle_entry *entry = muscle_lookup (कुंजी);
  अगर (entry)
    मुक्त (entry->storage);
  अन्यथा
    /* First insertion in the hash. */
    entry = muscle_entry_new (कुंजी);
  entry->मूल्य = मूल्य;
  entry->storage = शून्य;
}


/* Append VALUE to the current value of KEY.  If KEY did not already
   exist, create it.  Use MUSCLE_OBSTACK.  De-allocate the previously
   associated value.  Copy VALUE and SEPARATOR.  If VALUE does not end
   with TERMINATOR, append one.  */

अटल व्योम
muscle_grow (स्थिर अक्षर *कुंजी, स्थिर अक्षर *val,
             स्थिर अक्षर *separator, स्थिर अक्षर *terminator)
{
  muscle_entry *entry = muscle_lookup (कुंजी);
  अगर (entry)
    {
      obstack_sgrow (&muscle_obstack, entry->मूल्य);
      obstack_sgrow (&muscle_obstack, separator);
      मुक्त (entry->storage);
    }
  अन्यथा
    entry = muscle_entry_new (कुंजी);

  obstack_sgrow (&muscle_obstack, val);

  माप_प्रकार vals = म_माप (val);
  माप_प्रकार terms = म_माप (terminator);
  अगर (terms <= vals
      && STRNEQ (val + vals - terms, terminator))
    obstack_sgrow (&muscle_obstack, terminator);

  {
    अक्षर स्थिर *new_val = obstack_finish0 (&muscle_obstack);
    entry->मूल्य = entry->storage = xstrdup (new_val);
    obstack_free (&muscle_obstack, new_val);
  }
}

/*------------------------------------------------------------------.
| Using muscle_grow, append a synchronization line for the location |
| LOC to the current value of KEY.                                  |
`------------------------------------------------------------------*/

अटल व्योम
muscle_syncline_grow (अक्षर स्थिर *कुंजी, location loc)
{
  obstack_printf (&muscle_obstack, "]b4_syncline(%d, ", loc.start.पंक्ति);
  obstack_quote (&muscle_obstack,
                 quotearg_style (c_quoting_style, map_file_name (loc.start.file)));
  obstack_sgrow (&muscle_obstack, ")dnl\n[");
  अक्षर स्थिर *extension = obstack_finish0 (&muscle_obstack);
  muscle_grow (कुंजी, extension, "", "");
  obstack_free (&muscle_obstack, extension);
}

/*------------------------------------------------------------------.
| Append VALUE to the current value of KEY, using muscle_grow.  But |
| in addition, issue a synchronization line for the location LOC    |
| using muscle_syncline_grow.                                       |
`------------------------------------------------------------------*/

व्योम
muscle_code_grow (स्थिर अक्षर *कुंजी, स्थिर अक्षर *val, location loc)
{
  muscle_syncline_grow (कुंजी, loc);
  muscle_grow (कुंजी, val, "", "\n");
}


व्योम
muscle_pair_list_grow (स्थिर अक्षर *muscle,
                       स्थिर अक्षर *a1, स्थिर अक्षर *a2)
{
  obstack_sgrow (&muscle_obstack, "[");
  obstack_quote (&muscle_obstack, a1);
  obstack_sgrow (&muscle_obstack, ", ");
  obstack_quote (&muscle_obstack, a2);
  obstack_sgrow (&muscle_obstack, "]");
  अक्षर स्थिर *pair = obstack_finish0 (&muscle_obstack);
  muscle_grow (muscle, pair, ",\n", "");
  obstack_free (&muscle_obstack, pair);
}


अक्षर स्थिर *
muscle_find_const (अक्षर स्थिर *कुंजी)
{
  muscle_entry *entry = muscle_lookup (कुंजी);
  वापस entry ? entry->मूल्य : शून्य;
}


अक्षर *
muscle_find (अक्षर स्थिर *कुंजी)
{
  muscle_entry *entry = muscle_lookup (कुंजी);
  अगर (entry)
    {
      aver (entry->मूल्य == entry->storage);
      वापस entry->storage;
    }
  वापस शून्य;
}


/* In the format 'file_name:line.column', append BOUND to MUSCLE.  Use
   digraphs for special characters in the file name.  */

अटल व्योम
muscle_boundary_grow (अक्षर स्थिर *कुंजी, boundary bound)
{
  obstack_sgrow  (&muscle_obstack, "[[");
  obstack_escape (&muscle_obstack, bound.file);
  obstack_printf (&muscle_obstack, ":%d.%d@@%d]]", bound.पंक्ति, bound.column, bound.byte);
  अक्षर स्थिर *extension = obstack_finish0 (&muscle_obstack);
  muscle_grow (कुंजी, extension, "", "");
  obstack_free (&muscle_obstack, extension);
}


व्योम
muscle_location_grow (अक्षर स्थिर *कुंजी, location loc)
{
  muscle_boundary_grow (कुंजी, loc.start);
  muscle_grow (कुंजी, "", ", ", "");
  muscle_boundary_grow (कुंजी, loc.end);
}

#घोषणा COMMON_DECODE(मूल्य_4)                                    \
  हाल '$':                                                     \
    ++(मूल्य_4); aver (*(मूल्य_4) == ']');                          \
    ++(मूल्य_4); aver (*(मूल्य_4) == '[');                          \
    obstack_sgrow (&muscle_obstack, "$");                       \
    अवरोध;                                                      \
  हाल '@':                                                     \
    चयन (*++(मूल्य_4))                                         \
      {                                                         \
      हाल '\'': /* Ignore "@'" */ अवरोध;                       \
      हाल '@': obstack_sgrow (&muscle_obstack, "@" ); अवरोध;   \
      हाल '{': obstack_sgrow (&muscle_obstack, "[" ); अवरोध;   \
      हाल '}': obstack_sgrow (&muscle_obstack, "]" ); अवरोध;   \
      शेष: aver (false); अवरोध;                             \
      }                                                         \
    अवरोध;                                                      \
  शेष:                                                      \
    obstack_1grow (&muscle_obstack, *(मूल्य_4));                  \
    अवरोध;

/* Reverse of obstack_escape.  */
अटल अक्षर *
string_decode (अक्षर स्थिर *कुंजी)
{
  अक्षर स्थिर *मूल्य = muscle_find_const (कुंजी);
  अगर (!मूल्य)
    वापस शून्य;
  करो {
    चयन (*मूल्य)
      {
        COMMON_DECODE (मूल्य)
        हाल '[':
        हाल ']':
          aver (false);
          अवरोध;
      }
  } जबतक (*मूल्य++);
  अक्षर स्थिर *मूल्य_decoded = obstack_finish (&muscle_obstack);
  अक्षर *res = xstrdup (मूल्य_decoded);
  obstack_free (&muscle_obstack, मूल्य_decoded);
  वापस res;
}

/* Reverse of muscle_location_grow.  */
अटल location
location_decode (अक्षर स्थिर *मूल्य)
{
  aver (मूल्य);
  aver (*मूल्य == '[');
  ++मूल्य; aver (*मूल्य == '[');
  location loc;
  जबतक (*++मूल्य)
    चयन (*मूल्य)
      {
        COMMON_DECODE (मूल्य)
        हाल '[':
          aver (false);
          अवरोध;
        हाल ']':
          ++मूल्य; aver (*मूल्य == ']');
          अक्षर *boundary_str = obstack_finish0 (&muscle_obstack);
          चयन (*++मूल्य)
            {
            हाल ',':
              boundary_set_from_string (&loc.start, boundary_str);
              obstack_free (&muscle_obstack, boundary_str);
              ++मूल्य; aver (*मूल्य == ' ');
              ++मूल्य; aver (*मूल्य == '[');
              ++मूल्य; aver (*मूल्य == '[');
              अवरोध;
            हाल '\0':
              boundary_set_from_string (&loc.end, boundary_str);
              obstack_free (&muscle_obstack, boundary_str);
              वापस loc;
              अवरोध;
            शेष:
              aver (false);
              अवरोध;
            }
          अवरोध;
      }
  aver (false);
  वापस loc;
}

व्योम
muscle_user_name_list_grow (अक्षर स्थिर *कुंजी, अक्षर स्थिर *user_name,
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

अटल
अक्षर *
define_directive (अक्षर स्थिर *assignment,
                  muscle_kind kind,
                  अक्षर स्थिर *मूल्य)
{
  अक्षर *eq = म_अक्षर (assignment, '=');
  अक्षर स्थिर *fmt
    = eq || !मूल्य || !*मूल्य ? "%%define %s"
    : kind == muscle_code     ? "%%define %s {%s}"
    : kind == muscle_string   ? "%%define %s \"%s\""
    :                           "%%define %s %s";
  अक्षर *res = xmalloc (म_माप (fmt) + म_माप (assignment)
                       + (मूल्य ? म_माप (मूल्य) : 0));
  प्र_लिखो (res, fmt, assignment, मूल्य);
  eq = म_अक्षर (res, '=');
  अगर (eq)
    *eq = eq[1] ? ' ' : '\0';
  वापस res;
}

/** If the \a variable name is obsolete, return the name to use,
 * otherwise \a variable.  If the \a value is obsolete, update it too.
 *
 * Allocates the returned value if needed, otherwise the returned
 * value is exactly \a variable.  */
अटल
अक्षर स्थिर *
muscle_percent_variable_update (अक्षर स्थिर *variable,
                                muscle_kind kind,
                                अक्षर स्थिर **मूल्य,
                                अक्षर **old, अक्षर **upd)
{
  प्रकार काष्ठा
  {
    स्थिर अक्षर *obsolete;
    स्थिर अक्षर *updated;
    muscle_kind kind;
  } conversion_type;
  स्थिर conversion_type conversion[] =
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
    { शून्य, शून्य, -1, }
  };

  क्रम (conversion_type स्थिर *c = conversion; c->obsolete; ++c)
    {
      अक्षर स्थिर *eq = म_अक्षर (c->obsolete, '=');
      अगर (eq
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
            अगर (eq2)
              {
                *eq2 = '\0';
                *मूल्य = eq2 + 1;
              }
            वापस res;
          }
        }
    }
  वापस variable;
}

व्योम
muscle_percent_define_जोड़ो (अक्षर स्थिर *var, location variable_loc,
                              muscle_kind kind,
                              अक्षर स्थिर *मूल्य,
                              muscle_percent_define_how how)
{
  /* Backward compatibility.  */
  अक्षर *old = शून्य;
  अक्षर *upd = शून्य;
  अक्षर स्थिर *variable
    = muscle_percent_variable_update (var, kind,
                                      &मूल्य, &old, &upd);
  uniqstr name = muscle_name (variable, शून्य);
  uniqstr loc_name = muscle_name (variable, "loc");
  uniqstr syncline_name = muscle_name (variable, "syncline");
  uniqstr how_name = muscle_name (variable, "how");
  uniqstr kind_name = muscle_name (variable, "kind");

  /* Command-line options are processed before the grammar file.  */
  bool warned = false;
  अगर (how == MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE)
    {
      अक्षर स्थिर *current_मूल्य = muscle_find_const (name);
      अगर (current_मूल्य)
        {
          दीर्घ l = म_से_दीर्घ (muscle_find_const (how_name), शून्य, 10);
          muscle_percent_define_how how_old
            = 0 <= l && l <= पूर्णांक_उच्च ? l : पूर्णांक_उच्च;
          अगर (how_old == MUSCLE_PERCENT_DEFINE_F)
            जाओ end;
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

  अगर (!warned && old && upd)
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
  अगर (variable != var)
    मुक्त ((अक्षर *) variable);
}

/* This is used for backward compatibility, e.g., "%define api.pure"
   supersedes "%pure-parser".  */
व्योम
muscle_percent_define_ensure (अक्षर स्थिर *variable, location loc,
                              bool मूल्य)
{
  uniqstr name = muscle_name (variable, शून्य);
  अक्षर स्थिर *val = मूल्य ? "" : "false";

  /* Don't complain is VARIABLE is already defined, but be sure to set
     its value to VAL.  */
  अगर (!muscle_find_const (name)
      || muscle_percent_define_flag_if (variable) != मूल्य)
    muscle_percent_define_जोड़ो (variable, loc, muscle_keyword, val,
                                  MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
}

/* Mark %define VARIABLE as used.  */
अटल व्योम
muscle_percent_define_use (अक्षर स्थिर *variable)
{
  muscle_जोड़ो (muscle_name (variable, "bison_variables"), "");
}

/* The value of %define variable VARIABLE (corresponding to FIELD, if
   defined).  Do not register as used, but diagnose unset variables.  */

अटल
अक्षर स्थिर *
muscle_percent_define_get_raw (अक्षर स्थिर *variable, अक्षर स्थिर *field)
{
  uniqstr name = muscle_name (variable, field);
  अक्षर स्थिर *res = muscle_find_const (name);
  अगर (!res)
    complain (शून्य, fatal, _("%s: undefined %%define variable %s"),
              "muscle_percent_define_get_raw", quote (variable));
  वापस res;
}

अक्षर *
muscle_percent_define_get (अक्षर स्थिर *variable)
{
  uniqstr name = muscle_name (variable, शून्य);
  अक्षर *मूल्य = string_decode (name);
  अगर (!मूल्य)
    मूल्य = xstrdup ("");
  muscle_percent_define_use (variable);
  वापस मूल्य;
}

/* The kind of VARIABLE.  An error if undefined.  */
अटल muscle_kind
muscle_percent_define_get_kind (अक्षर स्थिर *variable)
{
  वापस muscle_kind_new (muscle_percent_define_get_raw (variable, "kind"));
}

/* Check the kind of VARIABLE.  An error if undefined.  */
अटल व्योम
muscle_percent_define_check_kind (अक्षर स्थिर *variable, muscle_kind kind)
{
  अगर (muscle_percent_define_get_kind (variable) != kind)
    {
      location loc = muscle_percent_define_get_loc (variable);
      चयन (kind)
        {
        हाल muscle_code:
          complain (&loc, Wdeprecated,
                    _("%%define variable '%s' requires '{...}' values"),
                    variable);
          अवरोध;
        हाल muscle_keyword:
          complain (&loc, Wdeprecated,
                    _("%%define variable '%s' requires keyword values"),
                    variable);
          अवरोध;
        हाल muscle_string:
          complain (&loc, Wdeprecated,
                    _("%%define variable '%s' requires '\"...\"' values"),
                    variable);
          अवरोध;
        }
    }
}


location
muscle_percent_define_get_loc (अक्षर स्थिर *variable)
{
  वापस location_decode (muscle_percent_define_get_raw (variable, "loc"));
}

अक्षर स्थिर *
muscle_percent_define_get_syncline (अक्षर स्थिर *variable)
{
  वापस muscle_percent_define_get_raw (variable, "syncline");
}

bool
muscle_percent_define_ifdef (अक्षर स्थिर *variable)
{
  अगर (muscle_find_const (muscle_name (variable, शून्य)))
    {
      muscle_percent_define_use (variable);
      वापस true;
    }
  अन्यथा
    वापस false;
}

bool
muscle_percent_define_flag_if (अक्षर स्थिर *variable)
{
  uniqstr invalid_boolean_name = muscle_name (variable, "invalid_boolean");
  bool res = false;

  अगर (muscle_percent_define_ifdef (variable))
    {
      अक्षर *मूल्य = muscle_percent_define_get (variable);
      muscle_percent_define_check_kind (variable, muscle_keyword);
      अगर (मूल्य[0] == '\0' || STREQ (मूल्य, "true"))
        res = true;
      अन्यथा अगर (STREQ (मूल्य, "false"))
        res = false;
      अन्यथा अगर (!muscle_find_const (invalid_boolean_name))
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
    complain (शून्य, fatal, _("%s: undefined %%define variable %s"),
              "muscle_percent_define_flag", quote (variable));

  वापस res;
}

व्योम
muscle_percent_define_default (अक्षर स्थिर *variable, अक्षर स्थिर *मूल्य)
{
  uniqstr name = muscle_name (variable, शून्य);
  अगर (!muscle_find_const (name))
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

व्योम
muscle_percent_define_check_मूल्यावली (अक्षर स्थिर * स्थिर *मूल्यावली)
{
  क्रम (; *मूल्यावली; ++मूल्यावली)
    {
      अक्षर स्थिर * स्थिर *variablep = मूल्यावली;
      uniqstr name = muscle_name (*variablep, शून्य);
      अक्षर *मूल्य = string_decode (name);
      muscle_percent_define_check_kind (*variablep, muscle_keyword);
      अगर (मूल्य)
        {
          क्रम (++मूल्यावली; *मूल्यावली; ++मूल्यावली)
            अगर (STREQ (मूल्य, *मूल्यावली))
              अवरोध;
          अगर (!*मूल्यावली)
            {
              location loc = muscle_percent_define_get_loc (*variablep);
              complain (&loc, complaint,
                        _("invalid value for %%define variable %s: %s"),
                        quote (*variablep), quote_n (1, मूल्य));
              क्रम (मूल्यावली = variablep + 1; *मूल्यावली; ++मूल्यावली)
                subcomplain (&loc, complaint | no_caret | silent,
                             _("accepted value: %s"), quote (*मूल्यावली));
            }
          अन्यथा
            जबतक (*मूल्यावली)
              ++मूल्यावली;
          मुक्त (मूल्य);
        }
      अन्यथा
        complain (शून्य, fatal, _("%s: undefined %%define variable %s"),
                  "muscle_percent_define_check_values", quote (*variablep));
    }
}

व्योम
muscle_percent_code_grow (अक्षर स्थिर *qualifier, location qualifier_loc,
                          अक्षर स्थिर *code, location code_loc)
{
  अक्षर स्थिर *name = UNIQSTR_CONCAT ("percent_code(", qualifier, ")");
  muscle_code_grow (name, code, code_loc);
  muscle_user_name_list_grow ("percent_code_user_qualifiers", qualifier,
                               qualifier_loc);
}


/*------------------------------------------------.
| Output the definition of ENTRY as a m4_define.  |
`------------------------------------------------*/

अटल अंतरभूत bool
muscle_m4_output (muscle_entry *entry, खाता *out)
{
  ख_लिखो (out,
           "m4_define([b4_%s],\n"
           "[[%s]])\n\n\n", entry->कुंजी, entry->मूल्य);
  वापस true;
}

अटल bool
muscle_m4_output_processor (व्योम *entry, व्योम *out)
{
  वापस muscle_m4_output (entry, out);
}


व्योम
muscles_m4_output (खाता *out)
{
  hash_do_for_each (muscle_table, muscle_m4_output_processor, out);
}
