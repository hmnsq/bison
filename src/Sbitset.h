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

প্রকারলেখ unsigned অক্ষর *Sbitset;
প্রকারলেখ size_t Sbitset_সূচক;
# define SBITSET_সূচক_CONVERSION_SPEC "zu"

# define Sbitset__nbytes(NBITS) \
  (((NBITS) + CHAR_BIT - 1) / CHAR_BIT)
# define Sbitset__byteAddress(SELF, সূচক) \
  (((SELF) + (সূচক) / CHAR_BIT))
# define Sbitset__bit_mask(সূচক) \
  (1 << (CHAR_BIT - 1 - (সূচক) % CHAR_BIT))
# define Sbitset__last_byte_mask(NBITS) \
  (UCHAR_MAX << (CHAR_BIT - 1 - ((NBITS) - 1) % CHAR_BIT))

/* nbits must not be 0.  */
Sbitset Sbitset__new (Sbitset_সূচক nbits);
Sbitset Sbitset__new_on_obstack (Sbitset_সূচক nbits,
                                 কাঠামো obstack *obstackp);
শূন্য Sbitset__delete (Sbitset self);

# define Sbitset__test(SELF, সূচক)                                     \
  ((*Sbitset__byteAddress ((SELF), (সূচক)) & Sbitset__bit_mask (সূচক)) != 0)

bool Sbitset__isEmpty (Sbitset self, Sbitset_সূচক nbits);

শূন্য Sbitset__fprint (Sbitset self, Sbitset_সূচক nbits, FILE *file);

# define Sbitset__set(SELF, সূচক)                                      \
  করো {                                                                  \
    *Sbitset__byteAddress ((SELF), (সূচক)) =                           \
      *Sbitset__byteAddress ((SELF), (সূচক)) | Sbitset__bit_mask (সূচক); \
  } যখন (0)

# define Sbitset__reset(SELF, সূচক)                                    \
  করো {                                                                  \
    *Sbitset__byteAddress ((SELF), (সূচক)) =                           \
      *Sbitset__byteAddress ((SELF), (সূচক)) & ~Sbitset__bit_mask (সূচক); \
  } যখন (0)

/* NBITS is the size of the bitset.  More than NBITS bits might be reset.  */
# define Sbitset__zero(SELF, NBITS)             \
  করো {                                          \
    memset (SELF, 0, Sbitset__nbytes (NBITS));  \
  } যখন (0)

/* NBITS is the size of the bitset.  More than NBITS bits might be set.  */
# define Sbitset__ones(SELF, NBITS)                     \
  করো {                                                  \
    memset (SELF, UCHAR_MAX, Sbitset__nbytes (NBITS));  \
  } যখন (0)

/* NBITS is the size of every bitset.  More than NBITS bits might be set.  */
# define Sbitset__or(SELF, OTHER1, OTHER2, NBITS)                       \
  করো {                                                                  \
    Sbitset ptr_self = (SELF);                                          \
    Sbitset ptr_other1 = (OTHER1);                                      \
    Sbitset ptr_other2 = (OTHER2);                                      \
    Sbitset end_self = ptr_self + Sbitset__nbytes (NBITS);              \
    জন্য (; ptr_self < end_self; ++ptr_self, ++ptr_other1, ++ptr_other2) \
      *ptr_self = *ptr_other1 | *ptr_other2;                            \
  } যখন (0)

/* ATTENTION: there are *two* loops here, "break" and "continue" will
   not apply to the whole loop, just the inner one.  */
# define SBITSET__FOR_EACH(SELF, NBITS, ITER, সূচক)                    \
  জন্য ((ITER) = (SELF); (ITER) < (SELF) + Sbitset__nbytes (NBITS); ++(ITER)) \
    যদি (*(ITER) != 0)                                                   \
      জন্য ((সূচক) = ((ITER)-(SELF))*CHAR_BIT;                          \
           (সূচক) < (NBITS) && (SELF)+(সূচক)/CHAR_BIT < (ITER)+1;     \
           ++(সূচক))                                                   \
        যদি (((*ITER) & Sbitset__bit_mask (সূচক)) != 0)

#endif /* !SBITSET_H_ */
