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
uniqstr_new (वर्ण अचर *str)
{
  uniqstr res = hash_lookup (uniqstrs_table, str);
  यदि (!res)
    {
      /* First insertion in the hash. */
      res = xstrdup (str);
      hash_xinsert (uniqstrs_table, res);
    }
  फर्काऊ res;
}

uniqstr
uniqstr_concat (पूर्णाङ्क nargs, ...)
{
  बहु_सूची args;

  बहु_शुरू (args, nargs);
  माप_प्रकार reslen = 0;
  लागि (पूर्णाङ्क i = 0; i < nargs; i++)
    reslen += म_माप (बहु_तर्क (args, वर्ण अचर *));
  बहु_पूर्ण (args);

  वर्ण *str = xmalloc (reslen + 1);
  वर्ण *p = str;

  बहु_शुरू (args, nargs);
  लागि (पूर्णाङ्क i = 0; i < nargs; i++)
    {
      वर्ण अचर *arg = बहु_तर्क (args, वर्ण अचर *);
      माप_प्रकार arglen = म_माप (arg);
      स_नकल (p, arg, arglen);
      p += arglen;
    }
  बहु_पूर्ण (args);

  *p = '\0';
  uniqstr res = hash_xinsert (uniqstrs_table, str);
  यदि (res != str)
    मुक्त (str);
  फर्काऊ res;
}

/*------------------------------.
| Abort if S is not a uniqstr.  |
`------------------------------*/

शून्य
uniqstr_assert (वर्ण अचर *str)
{
  uniqstr s = hash_lookup (uniqstrs_table, str);
  यदि (!s || s != str)
    {
      error (0, 0,
             "not a uniqstr: %s", quotearg (str));
      पात ();
    }
}


/*--------------------.
| Print the uniqstr.  |
`--------------------*/

स्थिर अन्तःस्थ bool
uniqstr_print (uniqstr ustr)
{
  ख_लिखो (मानक_त्रुटि, "%s\n", ustr);
  फर्काऊ true;
}

स्थिर bool
uniqstr_print_processor (शून्य *ustr, शून्य *null MAYBE_UNUSED)
{
  फर्काऊ uniqstr_print (ustr);
}

पूर्णाङ्क
uniqstr_cmp (uniqstr l, uniqstr r)
{
  फर्काऊ (l == r ? 0
          : !l ? -1
          : !r ? +1
          : म_भेद (l, r));
}


/*-----------------------.
| A uniqstr hash table.  |
`-----------------------*/

स्थिर bool
hash_compare_uniqstr (शून्य अचर *m1, शून्य अचर *m2)
{
  फर्काऊ STREQ (m1, m2);
}

स्थिर माप_प्रकार
hash_uniqstr (शून्य अचर *m, माप_प्रकार tablesize)
{
  फर्काऊ hash_string (m, tablesize);
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
