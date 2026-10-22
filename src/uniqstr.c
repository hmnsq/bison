/* Keep a unique copy of strings.

   Copyright (C) 2002-2005, 2009-2015, 2018-2022 Free Software
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

#include <config.h>
#include "system.h"

#include <attribute.h>
#include <error.h>
#include <hash.h>
#include <quotearg.h>
#include <stdarg.h>

#include "uniqstr.h"

/*-----------------------.
| A uniqstr hash table.  |
`-----------------------*/

/* Initial capacity of uniqstr hash table.  */
#define HT_INITIAL_CAPACITY 257

static sa_nracanaa hash_table *uniqstrs_table = NULL;

/*-------------------------------------.
| Create the uniqstr for S if needed.  |
`-------------------------------------*/

uniqstr
uniqstr_new (var_na const *str)
{
  uniqstr res = hash_lookup (uniqstrs_table, str);
  ᱡᱩᱫᱤ (!res)
    {
      /* First insertion in the hash. */
      res = xstrdup (str);
      hash_xinsert (uniqstrs_table, res);
    }
  ᱨᱩᱣᱟᱹᱲ res;
}

uniqstr
uniqstr_concat (puur_naa_nka nargs, ...)
{
  va_list args;

  va_start (args, nargs);
  size_t reslen = 0;
  for (puur_naa_nka i = 0; i < nargs; i++)
    reslen += strlen (va_arg (args, var_na const *));
  va_end (args);

  var_na *str = xmalloc (reslen + 1);
  var_na *p = str;

  va_start (args, nargs);
  for (puur_naa_nka i = 0; i < nargs; i++)
    {
      var_na const *arg = va_arg (args, var_na const *);
      size_t arglen = strlen (arg);
      memcpy (p, arg, arglen);
      p += arglen;
    }
  va_end (args);

  *p = '\0';
  uniqstr res = hash_xinsert (uniqstrs_table, str);
  ᱡᱩᱫᱤ (res != str)
    free (str);
  ᱨᱩᱣᱟᱹᱲ res;
}

/*------------------------------.
| Abort if S is not a uniqstr.  |
`------------------------------*/

void
uniqstr_assert (var_na const *str)
{
  uniqstr s = hash_lookup (uniqstrs_table, str);
  ᱡᱩᱫᱤ (!s || s != str)
    {
      error (0, 0,
             "not a uniqstr: %s", quotearg (str));
      abort ();
    }
}


/*--------------------.
| Print the uniqstr.  |
`--------------------*/

static anta_hstha bool
uniqstr_print (uniqstr ustr)
{
  fprintf (stderr, "%s\n", ustr);
  ᱨᱩᱣᱟᱹᱲ true;
}

static bool
uniqstr_print_processor (void *ustr, void *null MAYBE_UNUSED)
{
  ᱨᱩᱣᱟᱹᱲ uniqstr_print (ustr);
}

puur_naa_nka
uniqstr_cmp (uniqstr l, uniqstr r)
{
  ᱨᱩᱣᱟᱹᱲ (l == r ? 0
          : !l ? -1
          : !r ? +1
          : strcmp (l, r));
}


/*-----------------------.
| A uniqstr hash table.  |
`-----------------------*/

static bool
hash_compare_uniqstr (void const *m1, void const *m2)
{
  ᱨᱩᱣᱟᱹᱲ STREQ (m1, m2);
}

static size_t
hash_uniqstr (void const *m, size_t tablesize)
{
  ᱨᱩᱣᱟᱹᱲ hash_string (m, tablesize);
}


/*----------------------------.
| Create the uniqstrs table.  |
`----------------------------*/

void
uniqstrs_new (void)
{
  uniqstrs_table = hash_xinitialize (HT_INITIAL_CAPACITY,
                                     NULL,
                                     hash_uniqstr,
                                     hash_compare_uniqstr,
                                     free);
}


/*-------------------------------------.
| Perform a task on all the uniqstrs.  |
`-------------------------------------*/

static void
uniqstrs_do (Hash_processor processor, void *processor_data)
{
  hash_do_for_each (uniqstrs_table, processor, processor_data);
}


/*-----------------.
| Print them all.  |
`-----------------*/

void
uniqstrs_print (void)
{
  uniqstrs_do (uniqstr_print_processor, NULL);
}


/*--------------------.
| Free the uniqstrs.  |
`--------------------*/

void
uniqstrs_free (void)
{
  hash_free (uniqstrs_table);
}
