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

#جيڪڏهن_وصف_نه_ٿيل SBITSET_H_
# define SBITSET_H_

تعريفنوع بنانشان حرف *Sbitset;
تعريفنوع size_t Sbitset_اشاريو;
# define SBITSET_اشاريو_CONVERSION_SPEC "zu"

# define Sbitset__nbytes(NBITS) \
  (((NBITS) + CHAR_ٻٽو_انگ - 1) / CHAR_ٻٽو_انگ)
# define Sbitset_byte_پتو(SELF, اشاريو) \
  (((SELF) + (اشاريو) / CHAR_ٻٽو_انگ))
# define Sbitset_ٻٽو_انگ_نقاب(اشاريو) \
  (1 << (CHAR_ٻٽو_انگ - 1 - (اشاريو) % CHAR_ٻٽو_انگ))
# define Sbitset_آخري_byte_نقاب(NBITS) \
  (UCHAR_MAX << (CHAR_ٻٽو_انگ - 1 - ((NBITS) - 1) % CHAR_ٻٽو_انگ))

/* nbits must not be 0.  */
Sbitset Sbitset__new (Sbitset_اشاريو nbits);
Sbitset Sbitset__new_on_obstack (Sbitset_اشاريو nbits,
                                 بنية obstack *obstackp);
فراغ Sbitset__delete (Sbitset self);

# define Sbitset__test(SELF, اشاريو)                                     \
  ((*Sbitset_byte_پتو ((SELF), (اشاريو)) & Sbitset_ٻٽو_انگ_نقاب (اشاريو)) != 0)

bool Sbitset_is_خالي_جايون (Sbitset self, Sbitset_اشاريو nbits);

فراغ Sbitset__fprint (Sbitset self, Sbitset_اشاريو nbits, FILE *file);

# define Sbitset_سيٽ(SELF, اشاريو)                                      \
  افعل {                                                                  \
    *Sbitset_byte_پتو ((SELF), (اشاريو)) =                           \
      *Sbitset_byte_پتو ((SELF), (اشاريو)) | Sbitset_ٻٽو_انگ_نقاب (اشاريو); \
  } طالما (0)

# define Sbitset__reset(SELF, اشاريو)                                    \
  افعل {                                                                  \
    *Sbitset_byte_پتو ((SELF), (اشاريو)) =                           \
      *Sbitset_byte_پتو ((SELF), (اشاريو)) & ~Sbitset_ٻٽو_انگ_نقاب (اشاريو); \
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
    Sbitset پڇاڙي_self = ptr_self + Sbitset__nbytes (NBITS);              \
    لكل (; ptr_self < پڇاڙي_self; ++ptr_self, ++ptr_other1, ++ptr_other2) \
      *ptr_self = *ptr_other1 | *ptr_other2;                            \
  } طالما (0)

/* ATTENTION: there are *two* loops here, "break" and "continue" will
   not apply to the whole loop, just the inner one.  */
# define SBITSET__FOR_EACH(SELF, NBITS, ITER, اشاريو)                    \
  لكل ((ITER) = (SELF); (ITER) < (SELF) + Sbitset__nbytes (NBITS); ++(ITER)) \
    إذا (*(ITER) != 0)                                                   \
      لكل ((اشاريو) = ((ITER)-(SELF))*CHAR_ٻٽو_انگ;                          \
           (اشاريو) < (NBITS) && (SELF)+(اشاريو)/CHAR_ٻٽو_انگ < (ITER)+1;     \
           ++(اشاريو))                                                   \
        إذا (((*ITER) & Sbitset_ٻٽو_انگ_نقاب (اشاريو)) != 0)

#جيڪڏهن_پورو /* !SBITSET_H_ */
