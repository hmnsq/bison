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

#अन्तर्भाव <config.h>
#अन्तर्भाव "system.h"

#अन्तर्भाव <attribute.h>
#अन्तर्भाव <error.h>
#अन्तर्भाव <hash.h>
#अन्तर्भाव <quotearg.h>
#अन्तर्भाव <stdarg.h>

#अन्तर्भाव "uniqstr.h"

/*-----------------------.
| A uniqstr hash table.  |
`-----------------------*/

/* Initial capacity of uniqstr hash table.  */
#define HT_INITIAL_CAPACITY 257

स्थिर संरचना hash_table *uniqstrs_table = रिक्त;

/*-------------------------------------.
| Create the uniqstr for S if needed.  |
`-------------------------------------*/

uniqstr
uniqstr_new (अक्षर स्थिरांक *str)
{
  uniqstr res = hash_lookup (uniqstrs_table, str);
  जर (!res)
    {
      /* First insertion in the hash. */
      res = xstrdup (str);
      hash_xinsert (uniqstrs_table, res);
    }
  परत res;
}

uniqstr
uniqstr_concat (पूर्णांक nargs, ...)
{
  बहु_सूची args;

  बहु_शुरू (args, nargs);
  माप_प्रकार reslen = 0;
  साठी (पूर्णांक i = 0; i < nargs; i++)
    reslen += म_माप (बहु_तर्क (args, अक्षर स्थिरांक *));
  बहु_पूर्ण (args);

  अक्षर *str = xmalloc (reslen + 1);
  अक्षर *p = str;

  बहु_शुरू (args, nargs);
  साठी (पूर्णांक i = 0; i < nargs; i++)
    {
      अक्षर स्थिरांक *arg = बहु_तर्क (args, अक्षर स्थिरांक *);
      माप_प्रकार arglen = म_माप (arg);
      स_नकल (p, arg, arglen);
      p += arglen;
    }
  बहु_पूर्ण (args);

  *p = '\0';
  uniqstr res = hash_xinsert (uniqstrs_table, str);
  जर (res != str)
    मुक्त (str);
  परत res;
}

/*------------------------------.
| Abort if S is not a uniqstr.  |
`------------------------------*/

शून्य
uniqstr_assert (अक्षर स्थिरांक *str)
{
  uniqstr s = hash_lookup (uniqstrs_table, str);
  जर (!s || s != str)
    {
      error (0, 0,
             "not a uniqstr: %s", quotearg (str));
      पात ();
    }
}


/*--------------------.
| Print the uniqstr.  |
`--------------------*/

स्थिर ओळीत bool
uniqstr_print (uniqstr ustr)
{
  ख_लिखो (मानक_त्रुटि, "%s\n", ustr);
  परत true;
}

स्थिर bool
uniqstr_print_processor (शून्य *ustr, शून्य *null MAYBE_UNUSED)
{
  परत uniqstr_print (ustr);
}

पूर्णांक
uniqstr_cmp (uniqstr l, uniqstr r)
{
  परत (l == r ? 0
          : !l ? -1
          : !r ? +1
          : म_भेद (l, r));
}


/*-----------------------.
| A uniqstr hash table.  |
`-----------------------*/

स्थिर bool
hash_compare_uniqstr (शून्य स्थिरांक *m1, शून्य स्थिरांक *m2)
{
  परत STREQ (m1, m2);
}

स्थिर माप_प्रकार
hash_uniqstr (शून्य स्थिरांक *m, माप_प्रकार tablesize)
{
  परत hash_string (m, tablesize);
}


/*----------------------------.
| Create the uniqstrs table.  |
`----------------------------*/

शून्य
uniqstrs_new (शून्य)
{
  uniqstrs_table = hash_xinitialize (HT_INITIAL_CAPACITY,
                                     रिक्त,
                                     hash_uniqstr,
                                     hash_compare_uniqstr,
                                     मुक्त);
}


/*-------------------------------------.
| Perform a task on all the uniqstrs.  |
`-------------------------------------*/

स्थिर शून्य
uniqstrs_do (Hash_processor processor, शून्य *processor_data)
{
  hash_do_for_each (uniqstrs_table, processor, processor_data);
}


/*-----------------.
| Print them all.  |
`-----------------*/

शून्य
uniqstrs_print (शून्य)
{
  uniqstrs_do (uniqstr_print_processor, रिक्त);
}


/*--------------------.
| Free the uniqstrs.  |
`--------------------*/

शून्य
uniqstrs_free (शून्य)
{
  hash_free (uniqstrs_table);
}
