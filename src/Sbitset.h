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

#که_نه_تعریف_شوی SBITSET_H_
# define SBITSET_H_

تعريفنوع بینښه حرف *Sbitset;
تعريفنوع size_t Sbitset_شاخص;
# define SBITSET_شاخص_CONVERSION_SPEC "zu"

# define Sbitset__nbytes(NBITS) \
  (((NBITS) + CHAR_بټ - 1) / CHAR_بټ)
# define Sbitset_byte_پته(SELF, شاخص) \
  (((SELF) + (شاخص) / CHAR_بټ))
# define Sbitset_بټ_ماسک(شاخص) \
  (1 << (CHAR_بټ - 1 - (شاخص) % CHAR_بټ))
# define Sbitset_وروستی_byte_ماسک(NBITS) \
  (UCHAR_MAX << (CHAR_بټ - 1 - ((NBITS) - 1) % CHAR_بټ))

/* nbits must not be 0.  */
Sbitset Sbitset__new (Sbitset_شاخص nbits);
Sbitset Sbitset__new_on_obstack (Sbitset_شاخص nbits,
                                 بنية obstack *obstackp);
فراغ Sbitset__delete (Sbitset self);

# define Sbitset__test(SELF, شاخص)                                     \
  ((*Sbitset_byte_پته ((SELF), (شاخص)) & Sbitset_بټ_ماسک (شاخص)) != 0)

bool Sbitset_is_تشه (Sbitset self, Sbitset_شاخص nbits);

فراغ Sbitset__fprint (Sbitset self, Sbitset_شاخص nbits, FILE *file);

# define Sbitset_سېټ(SELF, شاخص)                                      \
  افعل {                                                                  \
    *Sbitset_byte_پته ((SELF), (شاخص)) =                           \
      *Sbitset_byte_پته ((SELF), (شاخص)) | Sbitset_بټ_ماسک (شاخص); \
  } طالما (0)

# define Sbitset__reset(SELF, شاخص)                                    \
  افعل {                                                                  \
    *Sbitset_byte_پته ((SELF), (شاخص)) =                           \
      *Sbitset_byte_پته ((SELF), (شاخص)) & ~Sbitset_بټ_ماسک (شاخص); \
  } طالما (0)

/* NBITS is the size of the bitset.  More than NBITS bits might be reset.  */
# define Sbitset__zero(SELF, NBITS)             \
  افعل {                                          \
    memset (SELF, 0, Sbitset__nbytes (NBITS));  \
  } طالما (0)

/* NBITS is the size of the bitset.  More than NBITS bits might be set.  */
# define Sbitset__ones(SELF, NBITS)                     \
  افعل {                                                  \
    memset (SELF, UCHAR_MAX, Sbitset__nbytes (NBITS));  \
  } طالما (0)

/* NBITS is the size of every bitset.  More than NBITS bits might be set.  */
# define Sbitset__or(SELF, OTHER1, OTHER2, NBITS)                       \
  افعل {                                                                  \
    Sbitset ptr_self = (SELF);                                          \
    Sbitset ptr_other1 = (OTHER1);                                      \
    Sbitset ptr_other2 = (OTHER2);                                      \
    Sbitset پای_ټکی_self = ptr_self + Sbitset__nbytes (NBITS);              \
    لكل (; ptr_self < پای_ټکی_self; ++ptr_self, ++ptr_other1, ++ptr_other2) \
      *ptr_self = *ptr_other1 | *ptr_other2;                            \
  } طالما (0)

/* ATTENTION: there are *two* loops here, "break" and "continue" will
   not apply to the whole loop, just the inner one.  */
# define SBITSET__FOR_EACH(SELF, NBITS, ITER, شاخص)                    \
  لكل ((ITER) = (SELF); (ITER) < (SELF) + Sbitset__nbytes (NBITS); ++(ITER)) \
    إذا (*(ITER) != 0)                                                   \
      لكل ((شاخص) = ((ITER)-(SELF))*CHAR_بټ;                          \
           (شاخص) < (NBITS) && (SELF)+(شاخص)/CHAR_بټ < (ITER)+1;     \
           ++(شاخص))                                                   \
        إذا (((*ITER) & Sbitset_بټ_ماسک (شاخص)) != 0)

#د_که_پای /* !SBITSET_H_ */
