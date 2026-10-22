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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ SBITSET_H_
# define SBITSET_H_

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *Sbitset;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ size_t Sbitset_ಸೂಚ್ಯಂಕ;
# define SBITSET_ಸೂಚ್ಯಂಕ_CONVERSION_SPEC "zu"

# define Sbitset__nbytes(NBITS) \
  (((NBITS) + CHAR_ಬಿಟ್ - 1) / CHAR_ಬಿಟ್)
# define Sbitset_byte_ವಿಳಾಸ(SELF, ಸೂಚ್ಯಂಕ) \
  (((SELF) + (ಸೂಚ್ಯಂಕ) / CHAR_ಬಿಟ್))
# define Sbitset_ಬಿಟ್_ಮುಖವಾಡ(ಸೂಚ್ಯಂಕ) \
  (1 << (CHAR_ಬಿಟ್ - 1 - (ಸೂಚ್ಯಂಕ) % CHAR_ಬಿಟ್))
# define Sbitset_ಕೊನೆಯ_byte_ಮುಖವಾಡ(NBITS) \
  (UCHAR_MAX << (CHAR_ಬಿಟ್ - 1 - ((NBITS) - 1) % CHAR_ಬಿಟ್))

/* nbits must not be 0.  */
Sbitset Sbitset__new (Sbitset_ಸೂಚ್ಯಂಕ nbits);
Sbitset Sbitset__new_on_obstack (Sbitset_ಸೂಚ್ಯಂಕ nbits,
                                 ರಚನೆ obstack *obstackp);
ಶೂನ್ಯ Sbitset__delete (Sbitset self);

# define Sbitset__test(SELF, ಸೂಚ್ಯಂಕ)                                     \
  ((*Sbitset_byte_ವಿಳಾಸ ((SELF), (ಸೂಚ್ಯಂಕ)) & Sbitset_ಬಿಟ್_ಮುಖವಾಡ (ಸೂಚ್ಯಂಕ)) != 0)

bool Sbitset_is_ಖಾಲಿಗಳು (Sbitset self, Sbitset_ಸೂಚ್ಯಂಕ nbits);

ಶೂನ್ಯ Sbitset__fprint (Sbitset self, Sbitset_ಸೂಚ್ಯಂಕ nbits, FILE *file);

# define Sbitset_ಗಣ(SELF, ಸೂಚ್ಯಂಕ)                                      \
  ಮಾಡು {                                                                  \
    *Sbitset_byte_ವಿಳಾಸ ((SELF), (ಸೂಚ್ಯಂಕ)) =                           \
      *Sbitset_byte_ವಿಳಾಸ ((SELF), (ಸೂಚ್ಯಂಕ)) | Sbitset_ಬಿಟ್_ಮುಖವಾಡ (ಸೂಚ್ಯಂಕ); \
  } ಆಗ (0)

# define Sbitset__reset(SELF, ಸೂಚ್ಯಂಕ)                                    \
  ಮಾಡು {                                                                  \
    *Sbitset_byte_ವಿಳಾಸ ((SELF), (ಸೂಚ್ಯಂಕ)) =                           \
      *Sbitset_byte_ವಿಳಾಸ ((SELF), (ಸೂಚ್ಯಂಕ)) & ~Sbitset_ಬಿಟ್_ಮುಖವಾಡ (ಸೂಚ್ಯಂಕ); \
  } ಆಗ (0)

/* NBITS is the size of the bitset.  More than NBITS bits might be reset.  */
# define Sbitset__zero(SELF, NBITS)             \
  ಮಾಡು {                                          \
    memset (SELF, 0, Sbitset__nbytes (NBITS));  \
  } ಆಗ (0)

/* NBITS is the size of the bitset.  More than NBITS bits might be set.  */
# define Sbitset__ones(SELF, NBITS)                     \
  ಮಾಡು {                                                  \
    memset (SELF, UCHAR_MAX, Sbitset__nbytes (NBITS));  \
  } ಆಗ (0)

/* NBITS is the size of every bitset.  More than NBITS bits might be set.  */
# define Sbitset__or(SELF, OTHER1, OTHER2, NBITS)                       \
  ಮಾಡು {                                                                  \
    Sbitset ptr_self = (SELF);                                          \
    Sbitset ptr_other1 = (OTHER1);                                      \
    Sbitset ptr_other2 = (OTHER2);                                      \
    Sbitset ಕೊನೆ_self = ptr_self + Sbitset__nbytes (NBITS);              \
    ಗಾಗಿ (; ptr_self < ಕೊನೆ_self; ++ptr_self, ++ptr_other1, ++ptr_other2) \
      *ptr_self = *ptr_other1 | *ptr_other2;                            \
  } ಆಗ (0)

/* ATTENTION: there are *two* loops here, "break" and "continue" will
   not apply to the whole loop, just the inner one.  */
# define SBITSET__FOR_EACH(SELF, NBITS, ITER, ಸೂಚ್ಯಂಕ)                    \
  ಗಾಗಿ ((ITER) = (SELF); (ITER) < (SELF) + Sbitset__nbytes (NBITS); ++(ITER)) \
    ಆದರೆ (*(ITER) != 0)                                                   \
      ಗಾಗಿ ((ಸೂಚ್ಯಂಕ) = ((ITER)-(SELF))*CHAR_ಬಿಟ್;                          \
           (ಸೂಚ್ಯಂಕ) < (NBITS) && (SELF)+(ಸೂಚ್ಯಂಕ)/CHAR_ಬಿಟ್ < (ITER)+1;     \
           ++(ಸೂಚ್ಯಂಕ))                                                   \
        ಆದರೆ (((*ITER) & Sbitset_ಬಿಟ್_ಮುಖವಾಡ (ಸೂಚ್ಯಂಕ)) != 0)

#ಆದರೆ_ಅಂತ್ಯ /* !SBITSET_H_ */
