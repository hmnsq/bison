/* A simple, memory-efficient bitset implementation.

   Copyright (C) 2009-2015, 2018-2022 Free Software Foundation, Inc.

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

#ifndef SBITSET_H_
# define SBITSET_H_

प्रकारनिर्देश अचिन्हित वर्ण *Sbitset;
प्रकारनिर्देश माप_प्रकार Sbitset__Index;
# define SBITSET__INDEX__CONVERSION_SPEC "zu"

# define Sbitset__nbytes(NBITS) \
  (((NBITS) + अक्षर_बिट - 1) / अक्षर_बिट)
# define Sbitset__byteAddress(SELF, INDEX) \
  (((SELF) + (INDEX) / अक्षर_बिट))
# define Sbitset__bit_mask(INDEX) \
  (1 << (अक्षर_बिट - 1 - (INDEX) % अक्षर_बिट))
# define Sbitset__last_byte_mask(NBITS) \
  (अच_अक्षर_उच्च << (अक्षर_बिट - 1 - ((NBITS) - 1) % अक्षर_बिट))

/* nbits must not be 0.  */
Sbitset Sbitset__new (Sbitset__Index nbits);
Sbitset Sbitset__new_on_obstack (Sbitset__Index nbits,
                                 संरचना obstack *obstackp);
शून्य Sbitset__delete (Sbitset self);

# define Sbitset__test(SELF, INDEX)                                     \
  ((*Sbitset__byteAddress ((SELF), (INDEX)) & Sbitset__bit_mask (INDEX)) != 0)

bool Sbitset__isEmpty (Sbitset self, Sbitset__Index nbits);

शून्य Sbitset__fprint (Sbitset self, Sbitset__Index nbits, खाता *file);

# define Sbitset__set(SELF, INDEX)                                      \
  गर {                                                                  \
    *Sbitset__byteAddress ((SELF), (INDEX)) =                           \
      *Sbitset__byteAddress ((SELF), (INDEX)) | Sbitset__bit_mask (INDEX); \
  } जबसम्म (0)

# define Sbitset__reset(SELF, INDEX)                                    \
  गर {                                                                  \
    *Sbitset__byteAddress ((SELF), (INDEX)) =                           \
      *Sbitset__byteAddress ((SELF), (INDEX)) & ~Sbitset__bit_mask (INDEX); \
  } जबसम्म (0)

/* NBITS is the size of the bitset.  More than NBITS bits might be reset.  */
# define Sbitset__zero(SELF, NBITS)             \
  गर {                                          \
    स_रखो (SELF, 0, Sbitset__nbytes (NBITS));  \
  } जबसम्म (0)

/* NBITS is the size of the bitset.  More than NBITS bits might be set.  */
# define Sbitset__ones(SELF, NBITS)                     \
  गर {                                                  \
    स_रखो (SELF, अच_अक्षर_उच्च, Sbitset__nbytes (NBITS));  \
  } जबसम्म (0)

/* NBITS is the size of every bitset.  More than NBITS bits might be set.  */
# define Sbitset__or(SELF, OTHER1, OTHER2, NBITS)                       \
  गर {                                                                  \
    Sbitset ptr_self = (SELF);                                          \
    Sbitset ptr_other1 = (OTHER1);                                      \
    Sbitset ptr_other2 = (OTHER2);                                      \
    Sbitset end_self = ptr_self + Sbitset__nbytes (NBITS);              \
    लागि (; ptr_self < end_self; ++ptr_self, ++ptr_other1, ++ptr_other2) \
      *ptr_self = *ptr_other1 | *ptr_other2;                            \
  } जबसम्म (0)

/* ATTENTION: there are *two* loops here, "break" and "continue" will
   not apply to the whole loop, just the inner one.  */
# define SBITSET__FOR_EACH(SELF, NBITS, ITER, INDEX)                    \
  लागि ((ITER) = (SELF); (ITER) < (SELF) + Sbitset__nbytes (NBITS); ++(ITER)) \
    यदि (*(ITER) != 0)                                                   \
      लागि ((INDEX) = ((ITER)-(SELF))*अक्षर_बिट;                          \
           (INDEX) < (NBITS) && (SELF)+(INDEX)/अक्षर_बिट < (ITER)+1;     \
           ++(INDEX))                                                   \
        यदि (((*ITER) & Sbitset__bit_mask (INDEX)) != 0)

#endif /* !SBITSET_H_ */
