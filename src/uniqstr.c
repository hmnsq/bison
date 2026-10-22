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

#समावेश <config.h>
#समावेश "system.h"

#समावेश <attribute.h>
#समावेश <error.h>
#समावेश <hash.h>
#समावेश <quotearg.h>
#समावेश <stdarg.h>

#समावेश "uniqstr.h"

/*-----------------------.
| A uniqstr hash table.  |
`-----------------------*/

/* Initial capacity of uniqstr hash table.  */
#घोषणा HT_INITIAL_CAPACITY 257

अटल काष्ठा hash_table *uniqstrs_table = शून्य;

/*-------------------------------------.
| Create the uniqstr for S if needed.  |
`-------------------------------------*/

uniqstr
uniqstr_new (अक्षर स्थिर *str)
{
  uniqstr res = hash_lookup (uniqstrs_table, str);
  अगर (!res)
    {
      /* First insertion in the hash. */
      res = xstrdup (str);
      hash_xinsert (uniqstrs_table, res);
    }
  वापस res;
}

uniqstr
uniqstr_concat (पूर्णांक nargs, ...)
{
  बहु_सूची args;

  बहु_शुरू (args, nargs);
  माप_प्रकार reslen = 0;
  क्रम (पूर्णांक i = 0; i < nargs; i++)
    reslen += म_माप (बहु_तर्क (args, अक्षर स्थिर *));
  बहु_पूर्ण (args);

  अक्षर *str = xmalloc (reslen + 1);
  अक्षर *p = str;

  बहु_शुरू (args, nargs);
  क्रम (पूर्णांक i = 0; i < nargs; i++)
    {
      अक्षर स्थिर *arg = बहु_तर्क (args, अक्षर स्थिर *);
      माप_प्रकार arglen = म_माप (arg);
      स_नकल (p, arg, arglen);
      p += arglen;
    }
  बहु_पूर्ण (args);

  *p = '\0';
  uniqstr res = hash_xinsert (uniqstrs_table, str);
  अगर (res != str)
    मुक्त (str);
  वापस res;
}

/*------------------------------.
| Abort if S is not a uniqstr.  |
`------------------------------*/

व्योम
uniqstr_assert (अक्षर स्थिर *str)
{
  uniqstr s = hash_lookup (uniqstrs_table, str);
  अगर (!s || s != str)
    {
      error (0, 0,
             "not a uniqstr: %s", quotearg (str));
      पात ();
    }
}


/*--------------------.
| Print the uniqstr.  |
`--------------------*/

अटल अंतरभूत bool
uniqstr_print (uniqstr ustr)
{
  ख_लिखो (मानक_त्रुटि, "%s\n", ustr);
  वापस true;
}

अटल bool
uniqstr_print_processor (व्योम *ustr, व्योम *null MAYBE_UNUSED)
{
  वापस uniqstr_print (ustr);
}

पूर्णांक
uniqstr_cmp (uniqstr l, uniqstr r)
{
  वापस (l == r ? 0
          : !l ? -1
          : !r ? +1
          : म_भेद (l, r));
}


/*-----------------------.
| A uniqstr hash table.  |
`-----------------------*/

अटल bool
hash_compare_uniqstr (व्योम स्थिर *m1, व्योम स्थिर *m2)
{
  वापस STREQ (m1, m2);
}

अटल माप_प्रकार
hash_uniqstr (व्योम स्थिर *m, माप_प्रकार tablesize)
{
  वापस hash_string (m, tablesize);
}


/*----------------------------.
| Create the uniqstrs table.  |
`----------------------------*/

व्योम
uniqstrs_new (व्योम)
{
  uniqstrs_table = hash_xinitialize (HT_INITIAL_CAPACITY,
                                     शून्य,
                                     hash_uniqstr,
                                     hash_compare_uniqstr,
                                     मुक्त);
}


/*-------------------------------------.
| Perform a task on all the uniqstrs.  |
`-------------------------------------*/

अटल व्योम
uniqstrs_do (Hash_processor processor, व्योम *processor_तथ्य)
{
  hash_do_for_each (uniqstrs_table, processor, processor_तथ्य);
}


/*-----------------.
| Print them all.  |
`-----------------*/

व्योम
uniqstrs_print (व्योम)
{
  uniqstrs_do (uniqstr_print_processor, शून्य);
}


/*--------------------.
| Free the uniqstrs.  |
`--------------------*/

व्योम
uniqstrs_free (व्योम)
{
  hash_free (uniqstrs_table);
}
