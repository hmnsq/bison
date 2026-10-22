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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ "system.h"

#અન્તર્ભાવ <hash.h>
#અન્તર્ભાવ <quote.h>

#અન્તર્ભાવ "complain.h"
#અન્તર્ભાવ "files.h"
#અન્તર્ભાવ "fixits.h"
#અન્તર્ભાવ "getargs.h"
#અન્તર્ભાવ "muscle-tab.h"

muscle_kind
muscle_kind_new (અક્ષર અચલ *k)
{
  જો (STREQ (k, "code"))
    પાછા muscle_code;
  નહીંતર જો (STREQ (k, "keyword"))
    પાછા muscle_keyword;
  નહીંતર જો (STREQ (k, "string"))
    પાછા muscle_string;
  abort ();
}

અક્ષર અચલ *
muscle_kind_string (muscle_kind k)
{
  પસંદગી (k)
    {
    સ્થિતિ muscle_code:    પાછા "code";
    સ્થિતિ muscle_keyword: પાછા "keyword";
    સ્થિતિ muscle_string:  પાછા "string";
    }
  abort ();
}


/* A key-value pair, along with storage that can be reclaimed when
   this pair is no longer needed.  */
પ્રકારવ્યાખ્યા સંરચના
{
  અક્ષર અચલ *ચાવી;
  અક્ષર અચલ *કિંમત;
  અક્ષર *storage;
  muscle_kind kind;
} muscle_entry;


/* The name of muscle for the %define variable VAR (corresponding to
   FIELD, if defined).  */
સ્થિર uniqstr
muscle_name (અક્ષર અચલ *var, અક્ષર અચલ *field)
{
  જો (field)
    પાછા UNIQSTR_CONCAT ("percent_define_", field, "(", var, ")");
  નહીંતર
    પાછા UNIQSTR_CONCAT ("percent_define(", var, ")");
}

/* An obstack used to create some entries.  */
સંરચના obstack muscle_obstack;

/* Initial capacity of muscles hash table.  */
#define HT_INITIAL_CAPACITY 257

સ્થિર સંરચના hash_table *muscle_table = ખાલી;

સ્થિર bool
hash_compare_muscles (શૂન્ય અચલ *x, શૂન્ય અચલ *y)
{
  muscle_entry અચલ *m1 = x;
  muscle_entry અચલ *m2 = y;
  પાછા STREQ (m1->ચાવી, m2->ચાવી);
}

સ્થિર size_t
hash_muscle (અચલ શૂન્ય *x, size_t tablesize)
{
  muscle_entry અચલ *m = x;
  પાછા hash_string (m->ચાવી, tablesize);
}

/* Create a fresh muscle name KEY, and insert in the hash table.  */
સ્થિર શૂન્ય *
muscle_entry_new (અક્ષર અચલ *ચાવી)
{
  muscle_entry *res = xmalloc (કદ *res);
  res->ચાવી = ચાવી;
  res->કિંમત = ખાલી;
  res->storage = ખાલી;
  hash_xinsert (muscle_table, res);
  પાછા res;
}

સ્થિર શૂન્ય
muscle_entry_free (શૂન્ય *entry)
{
  muscle_entry *mentry = entry;
  મુક્ત (mentry->storage);
  મુક્ત (mentry);
}

શૂન્ય
muscle_init (શૂન્ય)
{
  /* Initialize the muscle obstack.  */
  obstack_init (&muscle_obstack);

  muscle_table = hash_xinitialize (HT_INITIAL_CAPACITY, ખાલી, hash_muscle,
                                   hash_compare_muscles, muscle_entry_free);
  /* Avoid warnings if the user defined this variable, but did not
     actually call -Wcex.  */
  મુક્ત (muscle_percent_define_get ("cex.timeout"));
}


શૂન્ય
muscle_free (શૂન્ય)
{
  hash_free (muscle_table);
  obstack_free (&muscle_obstack, ખાલી);
}

/* Look for the muscle named KEY.  Return NULL if does not exist.  */
સ્થિર muscle_entry *
muscle_lookup (અક્ષર અચલ *ચાવી)
{
  muscle_entry probe;
  probe.ચાવી = ચાવી;
  પાછા hash_lookup (muscle_table, &probe);
}


શૂન્ય
muscle_ઉમેરો (અક્ષર અચલ *ચાવી, અક્ષર અચલ *કિંમત)
{
  muscle_entry *entry = muscle_lookup (ચાવી);
  જો (entry)
    મુક્ત (entry->storage);
  નહીંતર
    /* First insertion in the hash. */
    entry = muscle_entry_new (ચાવી);
  entry->કિંમત = કિંમત;
  entry->storage = ખાલી;
}


/* Append VALUE to the current value of KEY.  If KEY did not already
   exist, create it.  Use MUSCLE_OBSTACK.  De-allocate the previously
   associated value.  Copy VALUE and SEPARATOR.  If VALUE does not end
   with TERMINATOR, append one.  */

સ્થિર શૂન્ય
muscle_grow (અચલ અક્ષર *ચાવી, અચલ અક્ષર *val,
             અચલ અક્ષર *separator, અચલ અક્ષર *terminator)
{
  muscle_entry *entry = muscle_lookup (ચાવી);
  જો (entry)
    {
      obstack_sgrow (&muscle_obstack, entry->કિંમત);
      obstack_sgrow (&muscle_obstack, separator);
      મુક્ત (entry->storage);
    }
  નહીંતર
    entry = muscle_entry_new (ચાવી);

  obstack_sgrow (&muscle_obstack, val);

  size_t vals = strlen (val);
  size_t terms = strlen (terminator);
  જો (terms <= vals
      && STRNEQ (val + vals - terms, terminator))
    obstack_sgrow (&muscle_obstack, terminator);

  {
    અક્ષર અચલ *new_val = obstack_finish0 (&muscle_obstack);
    entry->કિંમત = entry->storage = xstrdup (new_val);
    obstack_free (&muscle_obstack, new_val);
  }
}

/*------------------------------------------------------------------.
| Using muscle_grow, append a synchronization line for the location |
| LOC to the current value of KEY.                                  |
`------------------------------------------------------------------*/

સ્થિર શૂન્ય
muscle_syncline_grow (અક્ષર અચલ *ચાવી, location loc)
{
  obstack_printf (&muscle_obstack, "]b4_syncline(%d, ", loc.start.લીટી);
  obstack_quote (&muscle_obstack,
                 quotearg_style (c_quoting_style, map_file_name (loc.start.file)));
  obstack_sgrow (&muscle_obstack, ")dnl\n[");
  અક્ષર અચલ *extension = obstack_finish0 (&muscle_obstack);
  muscle_grow (ચાવી, extension, "", "");
  obstack_free (&muscle_obstack, extension);
}

/*------------------------------------------------------------------.
| Append VALUE to the current value of KEY, using muscle_grow.  But |
| in addition, issue a synchronization line for the location LOC    |
| using muscle_syncline_grow.                                       |
`------------------------------------------------------------------*/

શૂન્ય
muscle_code_grow (અચલ અક્ષર *ચાવી, અચલ અક્ષર *val, location loc)
{
  muscle_syncline_grow (ચાવી, loc);
  muscle_grow (ચાવી, val, "", "\n");
}


શૂન્ય
muscle_pair_list_grow (અચલ અક્ષર *muscle,
                       અચલ અક્ષર *a1, અચલ અક્ષર *a2)
{
  obstack_sgrow (&muscle_obstack, "[");
  obstack_quote (&muscle_obstack, a1);
  obstack_sgrow (&muscle_obstack, ", ");
  obstack_quote (&muscle_obstack, a2);
  obstack_sgrow (&muscle_obstack, "]");
  અક્ષર અચલ *pair = obstack_finish0 (&muscle_obstack);
  muscle_grow (muscle, pair, ",\n", "");
  obstack_free (&muscle_obstack, pair);
}


અક્ષર અચલ *
muscle_find_const (અક્ષર અચલ *ચાવી)
{
  muscle_entry *entry = muscle_lookup (ચાવી);
  પાછા entry ? entry->કિંમત : ખાલી;
}


અક્ષર *
muscle_find (અક્ષર અચલ *ચાવી)
{
  muscle_entry *entry = muscle_lookup (ચાવી);
  જો (entry)
    {
      aver (entry->કિંમત == entry->storage);
      પાછા entry->storage;
    }
  પાછા ખાલી;
}


/* In the format 'file_name:line.column', append BOUND to MUSCLE.  Use
   digraphs for special characters in the file name.  */

સ્થિર શૂન્ય
muscle_boundary_grow (અક્ષર અચલ *ચાવી, boundary bound)
{
  obstack_sgrow  (&muscle_obstack, "[[");
  obstack_escape (&muscle_obstack, bound.file);
  obstack_printf (&muscle_obstack, ":%d.%d@@%d]]", bound.લીટી, bound.column, bound.byte);
  અક્ષર અચલ *extension = obstack_finish0 (&muscle_obstack);
  muscle_grow (ચાવી, extension, "", "");
  obstack_free (&muscle_obstack, extension);
}


શૂન્ય
muscle_location_grow (અક્ષર અચલ *ચાવી, location loc)
{
  muscle_boundary_grow (ચાવી, loc.start);
  muscle_grow (ચાવી, "", ", ", "");
  muscle_boundary_grow (ચાવી, loc.end);
}

#define COMMON_DECODE(કિંમત_4)                                    \
  સ્થિતિ '$':                                                     \
    ++(કિંમત_4); aver (*(કિંમત_4) == ']');                          \
    ++(કિંમત_4); aver (*(કિંમત_4) == '[');                          \
    obstack_sgrow (&muscle_obstack, "$");                       \
    તોડો;                                                      \
  સ્થિતિ '@':                                                     \
    પસંદગી (*++(કિંમત_4))                                         \
      {                                                         \
      સ્થિતિ '\'': /* Ignore "@'" */ તોડો;                       \
      સ્થિતિ '@': obstack_sgrow (&muscle_obstack, "@" ); તોડો;   \
      સ્થિતિ '{': obstack_sgrow (&muscle_obstack, "[" ); તોડો;   \
      સ્થિતિ '}': obstack_sgrow (&muscle_obstack, "]" ); તોડો;   \
      મૂળભૂત: aver (false); તોડો;                             \
      }                                                         \
    તોડો;                                                      \
  મૂળભૂત:                                                      \
    obstack_1grow (&muscle_obstack, *(કિંમત_4));                  \
    તોડો;

/* Reverse of obstack_escape.  */
સ્થિર અક્ષર *
string_decode (અક્ષર અચલ *ચાવી)
{
  અક્ષર અચલ *કિંમત = muscle_find_const (ચાવી);
  જો (!કિંમત)
    પાછા ખાલી;
  કરો {
    પસંદગી (*કિંમત)
      {
        COMMON_DECODE (કિંમત)
        સ્થિતિ '[':
        સ્થિતિ ']':
          aver (false);
          તોડો;
      }
  } જ્યારે (*કિંમત++);
  અક્ષર અચલ *કિંમત_decoded = obstack_finish (&muscle_obstack);
  અક્ષર *res = xstrdup (કિંમત_decoded);
  obstack_free (&muscle_obstack, કિંમત_decoded);
  પાછા res;
}

/* Reverse of muscle_location_grow.  */
સ્થિર location
location_decode (અક્ષર અચલ *કિંમત)
{
  aver (કિંમત);
  aver (*કિંમત == '[');
  ++કિંમત; aver (*કિંમત == '[');
  location loc;
  જ્યારે (*++કિંમત)
    પસંદગી (*કિંમત)
      {
        COMMON_DECODE (કિંમત)
        સ્થિતિ '[':
          aver (false);
          તોડો;
        સ્થિતિ ']':
          ++કિંમત; aver (*કિંમત == ']');
          અક્ષર *boundary_str = obstack_finish0 (&muscle_obstack);
          પસંદગી (*++કિંમત)
            {
            સ્થિતિ ',':
              boundary_set_from_string (&loc.start, boundary_str);
              obstack_free (&muscle_obstack, boundary_str);
              ++કિંમત; aver (*કિંમત == ' ');
              ++કિંમત; aver (*કિંમત == '[');
              ++કિંમત; aver (*કિંમત == '[');
              તોડો;
            સ્થિતિ '\0':
              boundary_set_from_string (&loc.end, boundary_str);
              obstack_free (&muscle_obstack, boundary_str);
              પાછા loc;
              તોડો;
            મૂળભૂત:
              aver (false);
              તોડો;
            }
          તોડો;
      }
  aver (false);
  પાછા loc;
}

શૂન્ય
muscle_user_name_list_grow (અક્ષર અચલ *ચાવી, અક્ષર અચલ *user_name,
                            location loc)
{
  muscle_grow (ચાવી, "[[[[", ",", "");
  muscle_grow (ચાવી, user_name, "", "");
  muscle_grow (ચાવી, "]], ", "", "");
  muscle_location_grow (ચાવી, loc);
  muscle_grow (ચાવી, "]]", "", "");
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

સ્થિર
અક્ષર *
define_directive (અક્ષર અચલ *assignment,
                  muscle_kind kind,
                  અક્ષર અચલ *કિંમત)
{
  અક્ષર *eq = strchr (assignment, '=');
  અક્ષર અચલ *fmt
    = eq || !કિંમત || !*કિંમત ? "%%define %s"
    : kind == muscle_code     ? "%%define %s {%s}"
    : kind == muscle_string   ? "%%define %s \"%s\""
    :                           "%%define %s %s";
  અક્ષર *res = xmalloc (strlen (fmt) + strlen (assignment)
                       + (કિંમત ? strlen (કિંમત) : 0));
  sprintf (res, fmt, assignment, કિંમત);
  eq = strchr (res, '=');
  જો (eq)
    *eq = eq[1] ? ' ' : '\0';
  પાછા res;
}

/** If the \a variable name is obsolete, return the name to use,
 * otherwise \a variable.  If the \a value is obsolete, update it too.
 *
 * Allocates the returned value if needed, otherwise the returned
 * value is exactly \a variable.  */
સ્થિર
અક્ષર અચલ *
muscle_percent_variable_update (અક્ષર અચલ *variable,
                                muscle_kind kind,
                                અક્ષર અચલ **કિંમત,
                                અક્ષર **old, અક્ષર **upd)
{
  પ્રકારવ્યાખ્યા સંરચના
  {
    અચલ અક્ષર *obsolete;
    અચલ અક્ષર *updated;
    muscle_kind kind;
  } conversion_type;
  અચલ conversion_type conversion[] =
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
    { ખાલી, ખાલી, -1, }
  };

  માટે (conversion_type અચલ *c = conversion; c->obsolete; ++c)
    {
      અક્ષર અચલ *eq = strchr (c->obsolete, '=');
      જો (eq
          ? (!strncmp (c->obsolete, variable, eq - c->obsolete)
             && STREQ (eq + 1, *કિંમત))
          : STREQ (c->obsolete, variable))
        {
          /* Generate the deprecation warning. */
          *old = c->obsolete[0] == '%'
            ? xstrdup (c->obsolete)
            : define_directive (c->obsolete, kind, *કિંમત);
          *upd = define_directive (c->updated, c->kind, *કિંમત);
          /* Update the variable and its value.  */
          {
            અક્ષર *res = xstrdup (c->updated);
            અક્ષર *eq2 = strchr (res, '=');
            જો (eq2)
              {
                *eq2 = '\0';
                *કિંમત = eq2 + 1;
              }
            પાછા res;
          }
        }
    }
  પાછા variable;
}

શૂન્ય
muscle_percent_define_ઉમેરો (અક્ષર અચલ *var, location variable_loc,
                              muscle_kind kind,
                              અક્ષર અચલ *કિંમત,
                              muscle_percent_define_how how)
{
  /* Backward compatibility.  */
  અક્ષર *old = ખાલી;
  અક્ષર *upd = ખાલી;
  અક્ષર અચલ *variable
    = muscle_percent_variable_update (var, kind,
                                      &કિંમત, &old, &upd);
  uniqstr name = muscle_name (variable, ખાલી);
  uniqstr loc_name = muscle_name (variable, "loc");
  uniqstr syncline_name = muscle_name (variable, "syncline");
  uniqstr how_name = muscle_name (variable, "how");
  uniqstr kind_name = muscle_name (variable, "kind");

  /* Command-line options are processed before the grammar file.  */
  bool warned = false;
  જો (how == MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE)
    {
      અક્ષર અચલ *current_કિંમત = muscle_find_const (name);
      જો (current_કિંમત)
        {
          long l = strtol (muscle_find_const (how_name), ખાલી, 10);
          muscle_percent_define_how how_old
            = 0 <= l && l <= INT_MAX ? l : INT_MAX;
          જો (how_old == MUSCLE_PERCENT_DEFINE_F)
            જાઓ end;
          /* If assigning the same value, make it a warning.  */
          warnings warn = STREQ (કિંમત, current_કિંમત) ? Wother : complaint;
          complain (&variable_loc, warn,
                    _("%%define variable %s redefined"),
                    quote (variable));
          location loc = muscle_percent_define_get_loc (variable);
          subcomplain (&loc, warn, _("previous definition"));
          fixits_register (&variable_loc, "");
          warned = true;
        }
    }

  જો (!warned && old && upd)
    deprecated_directive (&variable_loc, old, upd);

  MUSCLE_ઉમેરો_STRING (name, કિંમત);
  muscle_ઉમેરો (loc_name, "");
  muscle_location_grow (loc_name, variable_loc);
  muscle_ઉમેરો (syncline_name, "");
  muscle_syncline_grow (syncline_name, variable_loc);
  muscle_user_name_list_grow ("percent_define_user_variables", variable,
                              variable_loc);
  MUSCLE_ઉમેરો_INT (how_name, how);
  MUSCLE_ઉમેરો_STRING (kind_name, muscle_kind_string (kind));
 end:
  મુક્ત (old);
  મુક્ત (upd);
  જો (variable != var)
    મુક્ત ((અક્ષર *) variable);
}

/* This is used for backward compatibility, e.g., "%define api.pure"
   supersedes "%pure-parser".  */
શૂન્ય
muscle_percent_define_ensure (અક્ષર અચલ *variable, location loc,
                              bool કિંમત)
{
  uniqstr name = muscle_name (variable, ખાલી);
  અક્ષર અચલ *val = કિંમત ? "" : "false";

  /* Don't complain is VARIABLE is already defined, but be sure to set
     its value to VAL.  */
  જો (!muscle_find_const (name)
      || muscle_percent_define_flag_if (variable) != કિંમત)
    muscle_percent_define_ઉમેરો (variable, loc, muscle_keyword, val,
                                  MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
}

/* Mark %define VARIABLE as used.  */
સ્થિર શૂન્ય
muscle_percent_define_use (અક્ષર અચલ *variable)
{
  muscle_ઉમેરો (muscle_name (variable, "bison_variables"), "");
}

/* The value of %define variable VARIABLE (corresponding to FIELD, if
   defined).  Do not register as used, but diagnose unset variables.  */

સ્થિર
અક્ષર અચલ *
muscle_percent_define_get_raw (અક્ષર અચલ *variable, અક્ષર અચલ *field)
{
  uniqstr name = muscle_name (variable, field);
  અક્ષર અચલ *res = muscle_find_const (name);
  જો (!res)
    complain (ખાલી, fatal, _("%s: undefined %%define variable %s"),
              "muscle_percent_define_get_raw", quote (variable));
  પાછા res;
}

અક્ષર *
muscle_percent_define_get (અક્ષર અચલ *variable)
{
  uniqstr name = muscle_name (variable, ખાલી);
  અક્ષર *કિંમત = string_decode (name);
  જો (!કિંમત)
    કિંમત = xstrdup ("");
  muscle_percent_define_use (variable);
  પાછા કિંમત;
}

/* The kind of VARIABLE.  An error if undefined.  */
સ્થિર muscle_kind
muscle_percent_define_get_kind (અક્ષર અચલ *variable)
{
  પાછા muscle_kind_new (muscle_percent_define_get_raw (variable, "kind"));
}

/* Check the kind of VARIABLE.  An error if undefined.  */
સ્થિર શૂન્ય
muscle_percent_define_check_kind (અક્ષર અચલ *variable, muscle_kind kind)
{
  જો (muscle_percent_define_get_kind (variable) != kind)
    {
      location loc = muscle_percent_define_get_loc (variable);
      પસંદગી (kind)
        {
        સ્થિતિ muscle_code:
          complain (&loc, Wdeprecated,
                    _("%%define variable '%s' requires '{...}' values"),
                    variable);
          તોડો;
        સ્થિતિ muscle_keyword:
          complain (&loc, Wdeprecated,
                    _("%%define variable '%s' requires keyword values"),
                    variable);
          તોડો;
        સ્થિતિ muscle_string:
          complain (&loc, Wdeprecated,
                    _("%%define variable '%s' requires '\"...\"' values"),
                    variable);
          તોડો;
        }
    }
}


location
muscle_percent_define_get_loc (અક્ષર અચલ *variable)
{
  પાછા location_decode (muscle_percent_define_get_raw (variable, "loc"));
}

અક્ષર અચલ *
muscle_percent_define_get_syncline (અક્ષર અચલ *variable)
{
  પાછા muscle_percent_define_get_raw (variable, "syncline");
}

bool
muscle_percent_define_ifdef (અક્ષર અચલ *variable)
{
  જો (muscle_find_const (muscle_name (variable, ખાલી)))
    {
      muscle_percent_define_use (variable);
      પાછા true;
    }
  નહીંતર
    પાછા false;
}

bool
muscle_percent_define_flag_if (અક્ષર અચલ *variable)
{
  uniqstr invalid_boolean_name = muscle_name (variable, "invalid_boolean");
  bool res = false;

  જો (muscle_percent_define_ifdef (variable))
    {
      અક્ષર *કિંમત = muscle_percent_define_get (variable);
      muscle_percent_define_check_kind (variable, muscle_keyword);
      જો (કિંમત[0] == '\0' || STREQ (કિંમત, "true"))
        res = true;
      નહીંતર જો (STREQ (કિંમત, "false"))
        res = false;
      નહીંતર જો (!muscle_find_const (invalid_boolean_name))
        {
          muscle_ઉમેરો (invalid_boolean_name, "");
          location loc = muscle_percent_define_get_loc (variable);
          complain (&loc, complaint,
                    _("invalid value for %%define Boolean variable %s"),
                    quote (variable));
        }
      મુક્ત (કિંમત);
    }
  નહીંતર
    complain (ખાલી, fatal, _("%s: undefined %%define variable %s"),
              "muscle_percent_define_flag", quote (variable));

  પાછા res;
}

શૂન્ય
muscle_percent_define_default (અક્ષર અચલ *variable, અક્ષર અચલ *કિંમત)
{
  uniqstr name = muscle_name (variable, ખાલી);
  જો (!muscle_find_const (name))
    {
      MUSCLE_ઉમેરો_STRING (name, કિંમત);
      MUSCLE_ઉમેરો_STRING (muscle_name (variable, "kind"), "keyword");
      {
        uniqstr loc_name = muscle_name (variable, "loc");
        location loc;
        loc.start.file = "<default value>";
        loc.start.લીટી = -1;
        loc.start.column = -1;
        loc.start.byte = -1;
        loc.end = loc.start;
        muscle_ઉમેરો (loc_name, "");
        muscle_location_grow (loc_name, loc);
      }
      muscle_ઉમેરો (muscle_name (variable, "syncline"), "");
    }
}

શૂન્ય
muscle_percent_define_check_કિંમતો (અક્ષર અચલ * અચલ *કિંમતો)
{
  માટે (; *કિંમતો; ++કિંમતો)
    {
      અક્ષર અચલ * અચલ *variablep = કિંમતો;
      uniqstr name = muscle_name (*variablep, ખાલી);
      અક્ષર *કિંમત = string_decode (name);
      muscle_percent_define_check_kind (*variablep, muscle_keyword);
      જો (કિંમત)
        {
          માટે (++કિંમતો; *કિંમતો; ++કિંમતો)
            જો (STREQ (કિંમત, *કિંમતો))
              તોડો;
          જો (!*કિંમતો)
            {
              location loc = muscle_percent_define_get_loc (*variablep);
              complain (&loc, complaint,
                        _("invalid value for %%define variable %s: %s"),
                        quote (*variablep), quote_n (1, કિંમત));
              માટે (કિંમતો = variablep + 1; *કિંમતો; ++કિંમતો)
                subcomplain (&loc, complaint | no_caret | silent,
                             _("accepted value: %s"), quote (*કિંમતો));
            }
          નહીંતર
            જ્યારે (*કિંમતો)
              ++કિંમતો;
          મુક્ત (કિંમત);
        }
      નહીંતર
        complain (ખાલી, fatal, _("%s: undefined %%define variable %s"),
                  "muscle_percent_define_check_values", quote (*variablep));
    }
}

શૂન્ય
muscle_percent_code_grow (અક્ષર અચલ *qualifier, location qualifier_loc,
                          અક્ષર અચલ *code, location code_loc)
{
  અક્ષર અચલ *name = UNIQSTR_CONCAT ("percent_code(", qualifier, ")");
  muscle_code_grow (name, code, code_loc);
  muscle_user_name_list_grow ("percent_code_user_qualifiers", qualifier,
                               qualifier_loc);
}


/*------------------------------------------------.
| Output the definition of ENTRY as a m4_define.  |
`------------------------------------------------*/

સ્થિર અંતઃરેખ bool
muscle_m4_output (muscle_entry *entry, FILE *out)
{
  fprintf (out,
           "m4_define([b4_%s],\n"
           "[[%s]])\n\n\n", entry->ચાવી, entry->કિંમત);
  પાછા true;
}

સ્થિર bool
muscle_m4_output_processor (શૂન્ય *entry, શૂન્ય *out)
{
  પાછા muscle_m4_output (entry, out);
}


શૂન્ય
muscles_m4_output (FILE *out)
{
  hash_do_for_each (muscle_table, muscle_m4_output_processor, out);
}
