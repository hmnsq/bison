/* Muscle table manager for Bison,

   Copyright (C) 2001-2003, 2006-2015, 2018-2022 Free Software
   Foundation, Inc.

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

#ifndef MUSCLE_TAB_H_
# define MUSCLE_TAB_H_

# include <quotearg.h>

# include "location.h"

/* The kind of value associated to this muscle, depending on the
   syntax of the value: keyword (no delimiter, e.g., true), string
   (double quotes, e.g., "foo.h"), or code (braces, e.g., {int}).  */
வகைவரை எண்ணிக்கை
{
  muscle_code,
  muscle_keyword,
  muscle_string
} muscle_kind;

/* Conversion from string.  */
muscle_kind muscle_kind_new (எழுத்து மாறிலி *k);

/* Conversion to string.  */
எழுத்து மாறிலி *muscle_kind_string (muscle_kind k);


/* Create the MUSCLE_TABLE, and initialize it with default values.
   Also set up the MUSCLE_OBSTACK.  */
வெறுமை muscle_init (வெறுமை);

/* Insert (KEY, VALUE).  If KEY already existed, overwrite the
   previous value.  Otherwise create as a muscle_string type.  */
வெறுமை muscle_செருகு (எழுத்து மாறிலி *சாவி, எழுத்து மாறிலி *மதிப்பு);

/* Find the value of muscle KEY.  Unlike MUSCLE_FIND, this is always
   reliable to determine whether KEY has a value.  */
எழுத்து மாறிலி *muscle_find_const (எழுத்து மாறிலி *சாவி);

/* Find the value of muscle KEY.  Abort if muscle_insert was invoked
   more recently than muscle_grow for KEY since muscle_find can't
   return a char const *.  */
எழுத்து *muscle_find (எழுத்து மாறிலி *சாவி);

/* Free all the memory consumed by the muscle machinery only.  */
வெறுமை muscle_free (வெறுமை);


/* An obstack dedicated to receive muscle keys and values.  */
வெளி கட்டமைப்பு obstack muscle_obstack;

# define MUSCLE_செருகு_BOOL(சாவி_2, மதிப்பு_4)         \
  செய் {                                          \
    முழு v__ = மதிப்பு_4;                            \
    MUSCLE_செருகு_INT (சாவி_2, v__);               \
  } போது (0)

# define MUSCLE_INSERTF(சாவி_2, Format, மதிப்பு_4)                     \
  செய் {                                                          \
    obstack_printf (&muscle_obstack, Format, மதிப்பு_4);            \
    muscle_செருகு (சாவி_2, obstack_finish0 (&muscle_obstack));     \
  } போது (0)

# define MUSCLE_செருகு_INT(சாவி_2, மதிப்பு_4)          \
  MUSCLE_INSERTF (சாவி_2, "%d", மதிப்பு_4)

# define MUSCLE_செருகு_LONG_INT(சாவி_2, மதிப்பு_4)     \
  MUSCLE_INSERTF (சாவி_2, "%ld", மதிப்பு_4)

/* Key -> Value, but don't apply escaping to Value. */
# define MUSCLE_செருகு_STRING_RAW(சாவி_2, மதிப்பு_4)   \
  MUSCLE_INSERTF (சாவி_2, "%s", மதிப்பு_4)

/* Key -> Value, applying M4 escaping to Value. */
# define MUSCLE_செருகு_STRING(சாவி_2, மதிப்பு_4)                       \
  செய் {                                                          \
    obstack_escape (&muscle_obstack, மதிப்பு_4);                    \
    muscle_செருகு (சாவி_2, obstack_finish0 (&muscle_obstack));     \
  } போது (0)

/* Key -> Value, applying C escaping to Value (and then M4). */
# define MUSCLE_செருகு_C_STRING(சாவி_2, மதிப்பு_4)                     \
  MUSCLE_செருகு_STRING (சாவி_2, quotearg_style (c_quoting_style, மதிப்பு_4))


/* Append VALUE to the current value of KEY, using muscle_grow.  But
   in addition, issue a synchronization line for the location LOC.
   Be sure to append on a new line.  */
வெறுமை muscle_code_grow (மாறிலி எழுத்து *சாவி, மாறிலி எழுத்து *மதிப்பு, location loc);


/* MUSCLE is an M4 list of pairs.  Create or extend it with the pair
   (A1, A2) after escaping both values with digraphs.  Note that because the
   muscle values are output *double* quoted, one needs to strip the first level
   of quotes to reach the list itself.  */
வெறுமை muscle_pair_list_grow (மாறிலி எழுத்து *muscle,
                            மாறிலி எழுத்து *a1, மாறிலி எழுத்து *a2);

/* Grow KEY for the occurrence of the name USER_NAME at LOC appropriately for
   use with b4_check_user_names in ../data/bison.m4.  USER_NAME is not escaped
   with digraphs, so it must not contain '[' or ']'.  */
வெறுமை muscle_user_name_list_grow (எழுத்து மாறிலி *சாவி, எழுத்து மாறிலி *user_name,
                                 location loc);

/* In the format '[[file_name:line.column]], [[file_name:line.column]]',
   append LOC to MUSCLE.  Use digraphs for special characters in each
   file name.  */
வெறுமை muscle_location_grow (எழுத்து மாறிலி *சாவி, location loc);

/* Indicates whether a variable's value was specified with -D/--define, with
   -F/--force-define, or in the grammar file.  */
வகைவரை எண்ணிக்கை {
  MUSCLE_PERCENT_DEFINE_D = 0,
  MUSCLE_PERCENT_DEFINE_F,
  MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE
} muscle_percent_define_how;

/* Define the muscles for %define variable VARIABLE with VALUE specified
   at VARIABLE_LOC in the manner HOW unless it was specified in the
   grammar file while the previous definition for VARIABLE was specified
   with -F/--force-define.  Complain if a previous definition is being
   overridden and the new definition is specified in the grammar file.
   (These rules support the documented behavior as long as command-line
   definitions are processed before grammar file definitions.)  Record
   this as a user occurrence of VARIABLE by invoking
   muscle_user_name_list_grow.  */
வெறுமை muscle_percent_define_செருகு (எழுத்து மாறிலி *variable, location variable_loc,
                                   muscle_kind kind,
                                   எழுத்து மாறிலி *மதிப்பு,
                                   muscle_percent_define_how how);

/* Make sure that VARIABLE is set to the boolean VALUE.  Warn on mismatches
   only, but accept repeated declaration.  Used for backward compatibility
   between old directives such as %pure-parser, and the recommended use of
   variables (%define api.pure).   */
வெறுமை muscle_percent_define_ensure (எழுத்து மாறிலி *variable, location variable_loc,
                                   bool மதிப்பு);

/* Mimic b4_percent_define_get in ../data/bison.m4 exactly.  That is, if the
   %define variable VARIABLE is defined, return its value.  Otherwise, return
   the empty string.  Also, record Bison's usage of VARIABLE by defining
   b4_percent_define_bison_variables(VARIABLE).  The caller is responsible for
   freeing the memory of the returned string.  */
எழுத்து *muscle_percent_define_get (எழுத்து மாறிலி *variable);

/* Mimic muscle_percent_define_get_loc in ../data/bison.m4 exactly.  That is,
   if the %define variable VARIABLE is undefined, complain fatally since that's
   a Bison error.  Otherwise, return its definition location in a form
   appropriate for the first argument of warn_at, complain_at, or fatal_at.
   Don't record this as a Bison usage of VARIABLE as there's no reason to
   suspect that the user-supplied value has yet influenced the output.  */
location muscle_percent_define_get_loc (எழுத்து மாறிலி *variable);

/* Mimic muscle_percent_define_get_syncline in ../data/bison.m4 exactly.  That
   is, if the %define variable VARIABLE is undefined, complain fatally since
   that's a Bison error.  Otherwise, return its definition location as a
   b4_syncline invocation.  Don't record this as a Bison usage of VARIABLE as
   there's no reason to suspect that the user-supplied value has yet influenced
   the output.  */
எழுத்து மாறிலி *muscle_percent_define_get_syncline (எழுத்து மாறிலி *variable);

/* Mimic b4_percent_define_ifdef in ../data/bison.m4 exactly.  That is, if the
   %define variable VARIABLE is defined, return true.  Otherwise, return false.
   Also, record Bison's usage of VARIABLE by defining
   b4_percent_define_bison_variables(VARIABLE).  */
bool muscle_percent_define_ifdef (எழுத்து மாறிலி *variable);

/* Mimic b4_percent_define_flag_if in ../data/bison.m4 exactly.  That is, if
   the %define variable VARIABLE is defined to "" or "true", return true.  If
   it is defined to "false", return false.  Complain if it is undefined (a
   Bison error since the default value should have been set already) or defined
   to any other value (possibly a user error).  Also, record Bison's usage of
   VARIABLE by defining b4_percent_define_bison_variables(VARIABLE).  */
bool muscle_percent_define_flag_if (எழுத்து மாறிலி *variable);

/* Mimic b4_percent_define_default in ../data/bison.m4 exactly.  That is, if
   the %define variable VARIABLE is undefined, set its value to VALUE.
   Don't record this as a Bison usage of VARIABLE as there's no reason to
   suspect that the value has yet influenced the output.  */
வெறுமை muscle_percent_define_default (எழுத்து மாறிலி *variable, எழுத்து மாறிலி *மதிப்பு);

/* Mimic b4_percent_define_check_values in ../data/bison.m4 exactly except that
   the VALUES structure is more appropriate for C.  That is, VALUES points to a
   list of strings that is partitioned into sublists by NULL's, one terminating
   each sublist.  The last sublist is followed by a second NULL.  For each
   sublist, the first string is the name of a %define variable, and all
   remaining strings in that sublist are the valid values for that variable.
   Complain if such a variable is undefined (a Bison error since the default
   value should have been set already) or defined to any other value (possibly
   a user error).  Don't record this as a Bison usage of the variable as
   there's no reason to suspect that the value has yet influenced the
   output.  */
வெறுமை muscle_percent_define_check_மதிப்புகள் (எழுத்து மாறிலி * மாறிலி *மதிப்புகள்);

/* Grow the muscle for the %code qualifier QUALIFIER appearing at
   QUALIFIER_LOC with code CODE appearing at CODE_LOC.  Record this as a
   user occurrence of QUALIFIER by invoking
   muscle_user_name_list_grow.  */
வெறுமை muscle_percent_code_grow (எழுத்து மாறிலி *qualifier, location qualifier_loc,
                               எழுத்து மாறிலி *code, location code_loc);

/* Output the definition of all the current muscles into a list of
   m4_defines.  */
வெறுமை muscles_m4_output (FILE *out);

#endif /* not MUSCLE_TAB_H_ */
