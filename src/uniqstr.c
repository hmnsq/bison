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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ "system.h"

#അന്തര്ഭാവ <attribute.h>
#അന്തര്ഭാവ <error.h>
#അന്തര്ഭാവ <hash.h>
#അന്തര്ഭാവ <quotearg.h>
#അന്തര്ഭാവ <stdarg.h>

#അന്തര്ഭാവ "uniqstr.h"

/*-----------------------.
| A uniqstr hash table.  |
`-----------------------*/

/* Initial capacity of uniqstr hash table.  */
#define HT_INITIAL_CAPACITY 257

സ്ഥിതം ഘടന hash_table *uniqstrs_table = ശൂന്യ;

/*-------------------------------------.
| Create the uniqstr for S if needed.  |
`-------------------------------------*/

uniqstr
uniqstr_new (അക്ഷരം സ്ഥിരം *str)
{
  uniqstr res = hash_lookup (uniqstrs_table, str);
  എങ്കിൽ (!res)
    {
      /* First insertion in the hash. */
      res = xstrdup (str);
      hash_xinsert (uniqstrs_table, res);
    }
  തിരികെ res;
}

uniqstr
uniqstr_concat (പൂർണ്ണസംഖ്യ nargs, ...)
{
  va_list args;

  va_start (args, nargs);
  size_t reslen = 0;
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < nargs; i++)
    reslen += strlen (va_arg (args, അക്ഷരം സ്ഥിരം *));
  va_end (args);

  അക്ഷരം *str = xmalloc (reslen + 1);
  അക്ഷരം *p = str;

  va_start (args, nargs);
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < nargs; i++)
    {
      അക്ഷരം സ്ഥിരം *arg = va_arg (args, അക്ഷരം സ്ഥിരം *);
      size_t arglen = strlen (arg);
      memcpy (p, arg, arglen);
      p += arglen;
    }
  va_end (args);

  *p = '\0';
  uniqstr res = hash_xinsert (uniqstrs_table, str);
  എങ്കിൽ (res != str)
    സ്വതന്ത്രം (str);
  തിരികെ res;
}

/*------------------------------.
| Abort if S is not a uniqstr.  |
`------------------------------*/

ശൂന്യം
uniqstr_assert (അക്ഷരം സ്ഥിരം *str)
{
  uniqstr s = hash_lookup (uniqstrs_table, str);
  എങ്കിൽ (!s || s != str)
    {
      error (0, 0,
             "not a uniqstr: %s", quotearg (str));
      abort ();
    }
}


/*--------------------.
| Print the uniqstr.  |
`--------------------*/

സ്ഥിതം വരിയിൽ bool
uniqstr_print (uniqstr ustr)
{
  fprintf (stderr, "%s\n", ustr);
  തിരികെ true;
}

സ്ഥിതം bool
uniqstr_print_processor (ശൂന്യം *ustr, ശൂന്യം *null MAYBE_UNUSED)
{
  തിരികെ uniqstr_print (ustr);
}

പൂർണ്ണസംഖ്യ
uniqstr_cmp (uniqstr l, uniqstr r)
{
  തിരികെ (l == r ? 0
          : !l ? -1
          : !r ? +1
          : strcmp (l, r));
}


/*-----------------------.
| A uniqstr hash table.  |
`-----------------------*/

സ്ഥിതം bool
hash_compare_uniqstr (ശൂന്യം സ്ഥിരം *m1, ശൂന്യം സ്ഥിരം *m2)
{
  തിരികെ STREQ (m1, m2);
}

സ്ഥിതം size_t
hash_uniqstr (ശൂന്യം സ്ഥിരം *m, size_t tablesize)
{
  തിരികെ hash_string (m, tablesize);
}


/*----------------------------.
| Create the uniqstrs table.  |
`----------------------------*/

ശൂന്യം
uniqstrs_new (ശൂന്യം)
{
  uniqstrs_table = hash_xinitialize (HT_INITIAL_CAPACITY,
                                     ശൂന്യ,
                                     hash_uniqstr,
                                     hash_compare_uniqstr,
                                     സ്വതന്ത്രം);
}


/*-------------------------------------.
| Perform a task on all the uniqstrs.  |
`-------------------------------------*/

സ്ഥിതം ശൂന്യം
uniqstrs_do (Hash_processor processor, ശൂന്യം *processor_data)
{
  hash_do_for_each (uniqstrs_table, processor, processor_data);
}


/*-----------------.
| Print them all.  |
`-----------------*/

ശൂന്യം
uniqstrs_print (ശൂന്യം)
{
  uniqstrs_do (uniqstr_print_processor, ശൂന്യ);
}


/*--------------------.
| Free the uniqstrs.  |
`--------------------*/

ശൂന്യം
uniqstrs_free (ശൂന്യം)
{
  hash_free (uniqstrs_table);
}
