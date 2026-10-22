/* Conflict counterexample generation

   Copyright (C) 2020-2022 Free Software Foundation, Inc.

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

#ifndef COUNTEREXAMPLE_H
# define COUNTEREXAMPLE_H

# include "state.h"

// Init/deinit this module.
शून्य counterexample_init (शून्य);
शून्य counterexample_free (शून्य);

// Print the counterexamples for the conflicts of state S.
//
// Used both for the warnings on the terminal (OUT = stderr, PREFIX =
// ""), and for the reports (OUT != stderr, PREFIX != "").
शून्य
counterexample_report_state (स्थिरांक state *s, खाता *out, स्थिरांक अक्षर *prefix);

#endif /* COUNTEREXAMPLE_H */
