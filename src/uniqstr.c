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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ <attribute.h>
#ಅನ್ತರ್ಭಾವ <error.h>
#ಅನ್ತರ್ಭಾವ <hash.h>
#ಅನ್ತರ್ಭಾವ <quotearg.h>
#ಅನ್ತರ್ಭಾವ <stdarg.h>

#ಅನ್ತರ್ಭಾವ "uniqstr.h"

/*-----------------------.
| A uniqstr hash table.  |
`-----------------------*/

/* Initial capacity of uniqstr hash table.  */
#ವ್ಯಾಖ್ಯಾನಿಸು HT_INITIAL_ಸಾಮರ್ಥ್ಯ 257

ಸ್ಥಿರ ರಚನೆ hash_ಕೋಷ್ಟಕ *uniqstrs_ಕೋಷ್ಟಕ = ಖಾಲಿ;

/*-------------------------------------.
| Create the uniqstr for S if needed.  |
`-------------------------------------*/

uniqstr
uniqstr_new (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *str)
{
  uniqstr res = hash_lookup (uniqstrs_ಕೋಷ್ಟಕ, str);
  ಆದರೆ (!res)
    {
      /* First insertion in the hash. */
      res = xstrdup (str);
      hash_xinsert (uniqstrs_ಕೋಷ್ಟಕ, res);
    }
  ಹಿಂತಿರುಗಿ res;
}

uniqstr
uniqstr_concat (ಪೂರ್ಣಾಂಕ nargs, ...)
{
  va_list ವಾದಗಳು;

  va_ಆರಂಭ (ವಾದಗಳು, nargs);
  size_t reslen = 0;
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < nargs; i++)
    reslen += ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (va_arg (ವಾದಗಳು, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *));
  va_ಕೊನೆ (ವಾದಗಳು);

  ಅಕ್ಷರ *str = xmalloc (reslen + 1);
  ಅಕ್ಷರ *p = str;

  va_ಆರಂಭ (ವಾದಗಳು, nargs);
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < nargs; i++)
    {
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *arg = va_arg (ವಾದಗಳು, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *);
      size_t arglen = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (arg);
      ಸ್ಮರಣೆ_ನಕಲು (p, arg, arglen);
      p += arglen;
    }
  va_ಕೊನೆ (ವಾದಗಳು);

  *p = '\0';
  uniqstr res = hash_xinsert (uniqstrs_ಕೋಷ್ಟಕ, str);
  ಆದರೆ (res != str)
    ಬಿಡುಗಡೆ (str);
  ಹಿಂತಿರುಗಿ res;
}

/*------------------------------.
| Abort if S is not a uniqstr.  |
`------------------------------*/

ಶೂನ್ಯ
uniqstr_assert (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *str)
{
  uniqstr s = hash_lookup (uniqstrs_ಕೋಷ್ಟಕ, str);
  ಆದರೆ (!s || s != str)
    {
      error (0, 0,
             "not a uniqstr: %s", quotearg (str));
      abort ();
    }
}


/*--------------------.
| Print the uniqstr.  |
`--------------------*/

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
uniqstr_print (uniqstr ustr)
{
  fprintf (stderr, "%s\n", ustr);
  ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ bool
uniqstr_print_processor (ಶೂನ್ಯ *ustr, ಶೂನ್ಯ *null MAYBE_ಬಳಸದ)
{
  ಹಿಂತಿರುಗಿ uniqstr_print (ustr);
}

ಪೂರ್ಣಾಂಕ
uniqstr_cmp (uniqstr l, uniqstr r)
{
  ಹಿಂತಿರುಗಿ (l == r ? 0
          : !l ? -1
          : !r ? +1
          : strcmp (l, r));
}


/*-----------------------.
| A uniqstr hash table.  |
`-----------------------*/

ಸ್ಥಿರ bool
hash_compare_uniqstr (ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *m1, ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *m2)
{
  ಹಿಂತಿರುಗಿ STREQ (m1, m2);
}

ಸ್ಥಿರ size_t
hash_uniqstr (ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *m, size_t tablesize)
{
  ಹಿಂತಿರುಗಿ hash_string (m, tablesize);
}


/*----------------------------.
| Create the uniqstrs table.  |
`----------------------------*/

ಶೂನ್ಯ
uniqstrs_new (ಶೂನ್ಯ)
{
  uniqstrs_ಕೋಷ್ಟಕ = hash_xinitialize (HT_INITIAL_ಸಾಮರ್ಥ್ಯ,
                                     ಖಾಲಿ,
                                     hash_uniqstr,
                                     hash_compare_uniqstr,
                                     ಬಿಡುಗಡೆ);
}


/*-------------------------------------.
| Perform a task on all the uniqstrs.  |
`-------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
uniqstrs_do (Hash_processor processor, ಶೂನ್ಯ *processor_ದತ್ತಾಂಶ)
{
  hash_do_for_each (uniqstrs_ಕೋಷ್ಟಕ, processor, processor_ದತ್ತಾಂಶ);
}


/*-----------------.
| Print them all.  |
`-----------------*/

ಶೂನ್ಯ
uniqstrs_print (ಶೂನ್ಯ)
{
  uniqstrs_do (uniqstr_print_processor, ಖಾಲಿ);
}


/*--------------------.
| Free the uniqstrs.  |
`--------------------*/

ಶೂನ್ಯ
uniqstrs_free (ಶೂನ್ಯ)
{
  hash_free (uniqstrs_ಕೋಷ್ಟಕ);
}
