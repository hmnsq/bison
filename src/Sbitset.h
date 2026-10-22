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

#यदि_अपरिभाषितम् SBITSET_H_
# define SBITSET_H_

प्रकारनिर्देश अचिह्नित वर्ण *Sbitset;
प्रकारनिर्देश size_t Sbitset_सूचकांक;
# define SBITSET_सूचकांक_CONVERSION_SPEC "zu"

# define Sbitset__nbytes(NBITS) \
  (((NBITS) + CHAR_BIT - 1) / CHAR_BIT)
# define Sbitset_byte_स्थानाङ्कः(SELF, सूचकांक) \
  (((SELF) + (सूचकांक) / CHAR_BIT))
# define Sbitset_bit_आवरणम्(सूचकांक) \
  (1 << (CHAR_BIT - 1 - (सूचकांक) % CHAR_BIT))
# define Sbitset_चरमः_byte_आवरणम्(NBITS) \
  (UCHAR_MAX << (CHAR_BIT - 1 - ((NBITS) - 1) % CHAR_BIT))

/* nbits must not be 0.  */
Sbitset Sbitset__new (Sbitset_सूचकांक nbits);
Sbitset Sbitset__new_on_obstack (Sbitset_सूचकांक nbits,
                                 संरचना obstack *obstackp);
शून्य Sbitset__delete (Sbitset self);

# define Sbitset__test(SELF, सूचकांक)                                     \
  ((*Sbitset_byte_स्थानाङ्कः ((SELF), (सूचकांक)) & Sbitset_bit_आवरणम् (सूचकांक)) != 0)

bool Sbitset_is_रिक्तानि (Sbitset self, Sbitset_सूचकांक nbits);

शून्य Sbitset__fprint (Sbitset self, Sbitset_सूचकांक nbits, FILE *file);

# define Sbitset_समुच्चयः(SELF, सूचकांक)                                      \
  कुरु {                                                                  \
    *Sbitset_byte_स्थानाङ्कः ((SELF), (सूचकांक)) =                           \
      *Sbitset_byte_स्थानाङ्कः ((SELF), (सूचकांक)) | Sbitset_bit_आवरणम् (सूचकांक); \
  } यावत् (0)

# define Sbitset__reset(SELF, सूचकांक)                                    \
  कुरु {                                                                  \
    *Sbitset_byte_स्थानाङ्कः ((SELF), (सूचकांक)) =                           \
      *Sbitset_byte_स्थानाङ्कः ((SELF), (सूचकांक)) & ~Sbitset_bit_आवरणम् (सूचकांक); \
  } यावत् (0)

/* NBITS is the size of the bitset.  More than NBITS bits might be reset.  */
# define Sbitset__zero(SELF, NBITS)             \
  कुरु {                                          \
    स्मृतिं_पूरय (SELF, 0, Sbitset__nbytes (NBITS));  \
  } यावत् (0)

/* NBITS is the size of the bitset.  More than NBITS bits might be set.  */
# define Sbitset__ones(SELF, NBITS)                     \
  कुरु {                                                  \
    स्मृतिं_पूरय (SELF, UCHAR_MAX, Sbitset__nbytes (NBITS));  \
  } यावत् (0)

/* NBITS is the size of every bitset.  More than NBITS bits might be set.  */
# define Sbitset__or(SELF, OTHER1, OTHER2, NBITS)                       \
  कुरु {                                                                  \
    Sbitset ptr_self = (SELF);                                          \
    Sbitset ptr_other1 = (OTHER1);                                      \
    Sbitset ptr_other2 = (OTHER2);                                      \
    Sbitset अन्तः_self = ptr_self + Sbitset__nbytes (NBITS);              \
    प्रति (; ptr_self < अन्तः_self; ++ptr_self, ++ptr_other1, ++ptr_other2) \
      *ptr_self = *ptr_other1 | *ptr_other2;                            \
  } यावत् (0)

/* ATTENTION: there are *two* loops here, "break" and "continue" will
   not apply to the whole loop, just the inner one.  */
# define SBITSET__FOR_EACH(SELF, NBITS, ITER, सूचकांक)                    \
  प्रति ((ITER) = (SELF); (ITER) < (SELF) + Sbitset__nbytes (NBITS); ++(ITER)) \
    यदि (*(ITER) != 0)                                                   \
      प्रति ((सूचकांक) = ((ITER)-(SELF))*CHAR_BIT;                          \
           (सूचकांक) < (NBITS) && (SELF)+(सूचकांक)/CHAR_BIT < (ITER)+1;     \
           ++(सूचकांक))                                                   \
        यदि (((*ITER) & Sbitset_bit_आवरणम् (सूचकांक)) != 0)

#यदि_अन्तः /* !SBITSET_H_ */
