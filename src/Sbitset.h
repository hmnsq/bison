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

#جے_بنا_تعریف SBITSET_H_
# define SBITSET_H_

تعريفنوع بنانشانی حرف *Sbitset;
تعريفنوع size_t Sbitset_اشاریہ;
# define SBITSET_اشاریہ_CONVERSION_SPEC "zu"

# define Sbitset__nbytes(NBITS) \
  (((NBITS) + CHAR_ثنائی_ہندسہ - 1) / CHAR_ثنائی_ہندسہ)
# define Sbitset_byte_پتہ(SELF, اشاریہ) \
  (((SELF) + (اشاریہ) / CHAR_ثنائی_ہندسہ))
# define Sbitset_ثنائی_ہندسہ_نقاب(اشاریہ) \
  (1 << (CHAR_ثنائی_ہندسہ - 1 - (اشاریہ) % CHAR_ثنائی_ہندسہ))
# define Sbitset_آخری_byte_نقاب(NBITS) \
  (UCHAR_MAX << (CHAR_ثنائی_ہندسہ - 1 - ((NBITS) - 1) % CHAR_ثنائی_ہندسہ))

/* nbits must not be 0.  */
Sbitset Sbitset__new (Sbitset_اشاریہ nbits);
Sbitset Sbitset__new_on_obstack (Sbitset_اشاریہ nbits,
                                 بنية obstack *obstackp);
فراغ Sbitset__delete (Sbitset self);

# define Sbitset__test(SELF, اشاریہ)                                     \
  ((*Sbitset_byte_پتہ ((SELF), (اشاریہ)) & Sbitset_ثنائی_ہندسہ_نقاب (اشاریہ)) != 0)

bool Sbitset_is_خالی_تھاواں (Sbitset self, Sbitset_اشاریہ nbits);

فراغ Sbitset__fprint (Sbitset self, Sbitset_اشاریہ nbits, FILE *file);

# define Sbitset_سیٹ(SELF, اشاریہ)                                      \
  افعل {                                                                  \
    *Sbitset_byte_پتہ ((SELF), (اشاریہ)) =                           \
      *Sbitset_byte_پتہ ((SELF), (اشاریہ)) | Sbitset_ثنائی_ہندسہ_نقاب (اشاریہ); \
  } طالما (0)

# define Sbitset__reset(SELF, اشاریہ)                                    \
  افعل {                                                                  \
    *Sbitset_byte_پتہ ((SELF), (اشاریہ)) =                           \
      *Sbitset_byte_پتہ ((SELF), (اشاریہ)) & ~Sbitset_ثنائی_ہندسہ_نقاب (اشاریہ); \
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
    Sbitset سرا_self = ptr_self + Sbitset__nbytes (NBITS);              \
    لكل (; ptr_self < سرا_self; ++ptr_self, ++ptr_other1, ++ptr_other2) \
      *ptr_self = *ptr_other1 | *ptr_other2;                            \
  } طالما (0)

/* ATTENTION: there are *two* loops here, "break" and "continue" will
   not apply to the whole loop, just the inner one.  */
# define SBITSET__FOR_EACH(SELF, NBITS, ITER, اشاریہ)                    \
  لكل ((ITER) = (SELF); (ITER) < (SELF) + Sbitset__nbytes (NBITS); ++(ITER)) \
    إذا (*(ITER) != 0)                                                   \
      لكل ((اشاریہ) = ((ITER)-(SELF))*CHAR_ثنائی_ہندسہ;                          \
           (اشاریہ) < (NBITS) && (SELF)+(اشاریہ)/CHAR_ثنائی_ہندسہ < (ITER)+1;     \
           ++(اشاریہ))                                                   \
        إذا (((*ITER) & Sbitset_ثنائی_ہندسہ_نقاب (اشاریہ)) != 0)

#جے_انت /* !SBITSET_H_ */
