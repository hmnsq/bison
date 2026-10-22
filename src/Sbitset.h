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

#अगर_अघोषित SBITSET_H_
# define SBITSET_H_

प्रकार अचिन्हित अक्षर *Sbitset;
प्रकार माप_प्रकार Sbitset_सूचकांक;
# define SBITSET_सूचकांक_CONVERSION_SPEC "zu"

# define Sbitset__nbytes(NBITS) \
  (((NBITS) + अक्षर_बिट - 1) / अक्षर_बिट)
# define Sbitset__byteAddress(SELF, सूचकांक) \
  (((SELF) + (सूचकांक) / अक्षर_बिट))
# define Sbitset__bit_mask(सूचकांक) \
  (1 << (अक्षर_बिट - 1 - (सूचकांक) % अक्षर_बिट))
# define Sbitset__last_byte_mask(NBITS) \
  (अच_अक्षर_उच्च << (अक्षर_बिट - 1 - ((NBITS) - 1) % अक्षर_बिट))

/* nbits must not be 0.  */
Sbitset Sbitset__new (Sbitset_सूचकांक nbits);
Sbitset Sbitset__new_on_obstack (Sbitset_सूचकांक nbits,
                                 काष्ठा obstack *obstackp);
व्योम Sbitset__delete (Sbitset self);

# define Sbitset__test(SELF, सूचकांक)                                     \
  ((*Sbitset__byteAddress ((SELF), (सूचकांक)) & Sbitset__bit_mask (सूचकांक)) != 0)

bool Sbitset__isEmpty (Sbitset self, Sbitset_सूचकांक nbits);

व्योम Sbitset__fprint (Sbitset self, Sbitset_सूचकांक nbits, खाता *file);

# define Sbitset__set(SELF, सूचकांक)                                      \
  करो {                                                                  \
    *Sbitset__byteAddress ((SELF), (सूचकांक)) =                           \
      *Sbitset__byteAddress ((SELF), (सूचकांक)) | Sbitset__bit_mask (सूचकांक); \
  } जबतक (0)

# define Sbitset__reset(SELF, सूचकांक)                                    \
  करो {                                                                  \
    *Sbitset__byteAddress ((SELF), (सूचकांक)) =                           \
      *Sbitset__byteAddress ((SELF), (सूचकांक)) & ~Sbitset__bit_mask (सूचकांक); \
  } जबतक (0)

/* NBITS is the size of the bitset.  More than NBITS bits might be reset.  */
# define Sbitset__zero(SELF, NBITS)             \
  करो {                                          \
    स_रखो (SELF, 0, Sbitset__nbytes (NBITS));  \
  } जबतक (0)

/* NBITS is the size of the bitset.  More than NBITS bits might be set.  */
# define Sbitset__ones(SELF, NBITS)                     \
  करो {                                                  \
    स_रखो (SELF, अच_अक्षर_उच्च, Sbitset__nbytes (NBITS));  \
  } जबतक (0)

/* NBITS is the size of every bitset.  More than NBITS bits might be set.  */
# define Sbitset__or(SELF, OTHER1, OTHER2, NBITS)                       \
  करो {                                                                  \
    Sbitset ptr_self = (SELF);                                          \
    Sbitset ptr_other1 = (OTHER1);                                      \
    Sbitset ptr_other2 = (OTHER2);                                      \
    Sbitset end_self = ptr_self + Sbitset__nbytes (NBITS);              \
    क्रम (; ptr_self < end_self; ++ptr_self, ++ptr_other1, ++ptr_other2) \
      *ptr_self = *ptr_other1 | *ptr_other2;                            \
  } जबतक (0)

/* ATTENTION: there are *two* loops here, "break" and "continue" will
   not apply to the whole loop, just the inner one.  */
# define SBITSET__FOR_EACH(SELF, NBITS, ITER, सूचकांक)                    \
  क्रम ((ITER) = (SELF); (ITER) < (SELF) + Sbitset__nbytes (NBITS); ++(ITER)) \
    अगर (*(ITER) != 0)                                                   \
      क्रम ((सूचकांक) = ((ITER)-(SELF))*अक्षर_बिट;                          \
           (सूचकांक) < (NBITS) && (SELF)+(सूचकांक)/अक्षर_बिट < (ITER)+1;     \
           ++(सूचकांक))                                                   \
        अगर (((*ITER) & Sbitset__bit_mask (सूचकांक)) != 0)

#पूर्ण_अगर /* !SBITSET_H_ */
