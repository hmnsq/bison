/* Definitions for symtab.c and callers, part of Bison.

   Copyright (C) 1984, 1989, 1992, 2000-2002, 2004-2015, 2018-2022 Free
   Software Foundation, Inc.

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

/**
 * \file symtab.h
 * \brief Manipulating ::symbol.
 */

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ SYMTAB_H_
# define SYMTAB_H_

# include "assoc.h"
# include "location.h"
# include "scan-code.h"
# include "uniqstr.h"

/*----------.
| Symbols.  |
`----------*/

/** Symbol classes.  */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಎಣಿಕೆ
{
  /** Undefined.  */
  unknown_sym,
  /** Declared with %type: same as Undefined, but triggered a Wyacc if
      applied to a terminal. */
  pct_type_sym,
  /** Terminal. */
  token_sym,
  /** Nonterminal. */
  nterm_sym
} ಚಿಹ್ನೆ_class;


/** Internal token numbers. */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಪೂರ್ಣಾಂಕ ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ;
# define ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ_ಗರಿಷ್ಠ ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ ಚಿಹ್ನೆ ಚಿಹ್ನೆ;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ sym_content sym_content;

/* Declaration status of a symbol.

   First, it is "undeclared".  Then, if "undeclared" and used in a
   %printer/%destructor, it is "used".  If not "declared" but used in
   a rule, it is "needed".  Finally, if declared (via a rule for
   nonterminals, or %token), it is "declared".

   When status are checked at the end, "declared" symbols are fine,
   "used" symbols trigger warnings, otherwise it's an error.  */

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಎಣಿಕೆ
  {
    /** Used in the input file for an unknown reason (error).  */
    undeclared,
    /** Used by %destructor/%printer but not defined (warning).  */
    ಬಳಸಿದ,
    /** Used in the grammar (rules) but not defined (error).  */
    needed,
    /** Defined with %type or %token (good).  */
    declared,
  } declaration_ಸ್ಥಿತಿ;

ಎಣಿಕೆ ಸಂಕೇತ_props_type
  {
    destructor = 0,
    printer = 1,
  };
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಎಣಿಕೆ ಸಂಕೇತ_props_type ಸಂಕೇತ_props_type;

ಎಣಿಕೆ { ಸಂಕೇತ_PROPS_ಗಾತ್ರ = 2 };

ರಚನೆ ಚಿಹ್ನೆ
{
  /** The key, name of the symbol.  */
  uniqstr ಟ್ಯಾಗ್;

  /** The "defining" location.  */
  location location;

  /** Whether this symbol is translatable. */
  bool translatable;

  /** Whether \a location is about the first uses as left-hand side
      symbol of a rule (true), or simply the first occurrence (e.g.,
      in a %type, or as a rhs symbol of a rule).  The former type of
      location is more natural in error messages.  This Boolean helps
      moving from location of the first occurrence to first use as
      lhs. */
  bool location_ದ_lhs;

  /** Points to the other in the symbol-string pair for an alias. */
  ಚಿಹ್ನೆ *alias;

  /** Whether this symbol is the alias of another or not. */
  bool is_alias;

  /** All the info about the pointed symbol is there. */
  sym_content *content;
};

ರಚನೆ sym_content
{
  /** The main symbol that denotes this content (it contains the
      possible alias). */
  ಚಿಹ್ನೆ *ಚಿಹ್ನೆ;

  /** Its \c \%type.

      Beware that this is the type_name as was entered by the user,
      including silly things such as "]" if she entered "%token <]> t".
      Therefore, when outputting type_name to M4, be sure to escape it
      into "@}".  See quoted_output for instance.  */
  uniqstr type_name;

  /** Its \c \%type's location.  */
  location type_loc;

  /** Any \c \%destructor (resp. \%printer) declared specifically for this
      symbol.

      Access this field only through <tt>symbol</tt>'s interface functions. For
      example, if <tt>symbol::destructor = NULL</tt> (resp. <tt>symbol::printer
      = NULL</tt>), a default \c \%destructor (resp. \%printer) or a per-type
      \c symbol_destructor_printer_get will compute the correct one. */
  ಸಂಕೇತ_props props[ಸಂಕೇತ_PROPS_ಗಾತ್ರ];

  ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ ಸಂಖ್ಯೆ;
  location prec_loc;
  ಪೂರ್ಣಾಂಕ prec;
  assoc assoc;

  /** Token code, possibly specified by the user (%token FOO 42).  */
  ಪೂರ್ಣಾಂಕ ಸಂಕೇತ_2;

  ಚಿಹ್ನೆ_class class;
  declaration_ಸ್ಥಿತಿ ಸ್ಥಿತಿ_2;
};

/** Fetch (or create) the symbol associated to KEY.  */
ಚಿಹ್ನೆ *ಚಿಹ್ನೆ_from_uniqstr (ಸ್ಥಿರಾಂಕ uniqstr ಕೀಲಿ, location loc);

/** Fetch (or create) the symbol associated to KEY.  */
ಚಿಹ್ನೆ *ಚಿಹ್ನೆ_get (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ, location loc);

/** Generate a dummy nonterminal.

   Its name cannot conflict with the user's names.  */
ಚಿಹ್ನೆ *dummy_ಚಿಹ್ನೆ_get (location loc);


/*--------------------.
| Methods on symbol.  |
`--------------------*/

/** Print a symbol (for debugging). */
ಶೂನ್ಯ ಚಿಹ್ನೆ_print (ಚಿಹ್ನೆ ಸ್ಥಿರಾಂಕ *s, FILE *f);

/** Is this a dummy nonterminal?  */
bool ಚಿಹ್ನೆ_is_dummy (ಚಿಹ್ನೆ ಸ್ಥಿರಾಂಕ *sym);

/** The name of the code_props type: "\%destructor" or "\%printer".  */
ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸಂಕೇತ_props_type_string (ಸಂಕೇತ_props_type ಬಗೆ);

/** The name of the symbol that can be used as an identifier.
 ** Consider the alias if needed.
 ** Return 0 if there is none (e.g., the symbol is only defined as
 ** a string). */
uniqstr ಚಿಹ್ನೆ_id_get (ಚಿಹ್ನೆ ಸ್ಥಿರಾಂಕ *sym);

/**
 * Make \c str the literal string alias of \c sym.  Copy token number,
 * symbol number, and type from \c sym to \c str.
 */
ಶೂನ್ಯ ಚಿಹ್ನೆ_make_alias (ಚಿಹ್ನೆ *sym, ಚಿಹ್ನೆ *str, location loc);

/**
 * This symbol is used as the lhs of a rule.  Record this location
 * as definition point, if not already done.
 */
ಶೂನ್ಯ ಚಿಹ್ನೆ_location_as_lhs_ಗಣ (ಚಿಹ್ನೆ *sym, location loc);

/** Set the \c type_name associated with \c sym.

    Do nothing if passed 0 as \c type_name.  */
ಶೂನ್ಯ ಚಿಹ್ನೆ_type_ಗಣ (ಚಿಹ್ನೆ *sym, uniqstr type_name, location loc);

/** Set the \c \%destructor or \c \%printer associated with \c sym.  */
ಶೂನ್ಯ ಚಿಹ್ನೆ_ಸಂಕೇತ_props_ಗಣ (ಚಿಹ್ನೆ *sym, ಸಂಕೇತ_props_type ಬಗೆ,
                            ಸಂಕೇತ_props ಸ್ಥಿರಾಂಕ *destructor);

/** Get the computed \c \%destructor or \c %printer for \c sym, which was
    initialized with \c code_props_none_init if there's no \c \%destructor or
    \c %printer.  */
ಸಂಕೇತ_props *ಚಿಹ್ನೆ_ಸಂಕೇತ_props_get (ಚಿಹ್ನೆ *sym, ಸಂಕೇತ_props_type ಬಗೆ);

/** Set the \c precedence associated with \c sym.

    Ensure that \a symbol is a terminal.
    Do nothing if invoked with \c undef_assoc as \c assoc.  */
ಶೂನ್ಯ ಚಿಹ್ನೆ_precedence_ಗಣ (ಚಿಹ್ನೆ *sym, ಪೂರ್ಣಾಂಕ prec, assoc a, location loc);

/** Set the \c class associated with \c sym.

    Whether \c declaring means whether this class definition comes
    from %nterm or %token (but not %type, prec/assoc, etc.).  A symbol
    can have "declaring" set only at most once.  */
ಶೂನ್ಯ ಚಿಹ್ನೆ_class_ಗಣ (ಚಿಹ್ನೆ *sym, ಚಿಹ್ನೆ_class class, location loc,
                       bool declaring);

/** Set the token \c code of \c sym, specified by the user at \c loc.  */
ಶೂನ್ಯ ಚಿಹ್ನೆ_ಸಂಕೇತ_ಗಣ (ಚಿಹ್ನೆ *sym, ಪೂರ್ಣಾಂಕ ಸಂಕೇತ_2, location loc);



/*------------------.
| Special symbols.  |
`------------------*/

/** The error token. */
ಬಾಹ್ಯ ಚಿಹ್ನೆ *errtoken;
/** The token for unknown tokens.  */
ಬಾಹ್ಯ ಚಿಹ್ನೆ *undeftoken;
/** The end of input token.  */
ಬಾಹ್ಯ ಚಿಹ್ನೆ *eoftoken;
/** The genuine start symbol.

   $accept: start-symbol $end */
ಬಾಹ್ಯ ಚಿಹ್ನೆ *acceptsymbol;

/** Whether a symbol declared with a type tag.  */
ಬಾಹ್ಯ bool ಟ್ಯಾಗ್_ನೋಡಿದ;


/*-------------------.
| Symbol Relations.  |
`-------------------*/

/* The symbol relations are represented by a directed graph. */

/* The id of a node */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಪೂರ್ಣಾಂಕ graphid;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ symgraphlink symgraphlink;

ರಚನೆ symgraphlink
{
  /** The second \c symbol or group of a precedence relation.
   * See \c symgraph. */
  graphid id;

  symgraphlink *ಮುಂದಿನ;
};

/* Symbol precedence graph, to store the used precedence relations between
 * symbols. */

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ symgraph symgraph;

ರಚನೆ symgraph
{
  /** Identifier for the node: equal to the number of the symbol. */
  graphid id;

  /** The list of related symbols that have a smaller precedence. */
  symgraphlink *succ;

  /** The list of related symbols that have a greater precedence. */
  symgraphlink *pred;
};

/** Register a new precedence relation as used. */

ಶೂನ್ಯ ರಿಜಿಸ್ಟರ್_precedence (graphid ಮೊದಲ, graphid snd);

/** Print a warning for each symbol whose precedence and/or associativity
 * is useless. */

ಶೂನ್ಯ print_precedence_warnings (ಶೂನ್ಯ);

/*----------------------.
| Symbol associativity  |
`----------------------*/

ಶೂನ್ಯ ರಿಜಿಸ್ಟರ್_assoc (graphid i, graphid j);

/*-----------------.
| Semantic types.  |
`-----------------*/

/** A semantic type and its associated \c \%destructor and \c \%printer.

   Access the fields of this struct only through the interface functions in
   this file.  \sa symbol::destructor  */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
  /** The key, name of the semantic type.  */
  uniqstr ಟ್ಯಾಗ್;

  /** The location of its first occurrence.  */
  location location;

  /** Its status : "undeclared", "used" or "declared".
      It cannot be "needed".  */
  declaration_ಸ್ಥಿತಿ ಸ್ಥಿತಿ_2;

  /** Any \c %destructor and %printer declared for this
      semantic type.  */
  ಸಂಕೇತ_props props[ಸಂಕೇತ_PROPS_ಗಾತ್ರ];

} semantic_type;

/** Fetch (or create) the semantic type associated to KEY.  */
semantic_type *semantic_type_from_uniqstr (ಸ್ಥಿರಾಂಕ uniqstr ಕೀಲಿ,
                                           ಸ್ಥಿರಾಂಕ location *loc);

/** Fetch (or create) the semantic type associated to KEY.  */
semantic_type *semantic_type_get (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ, ಸ್ಥಿರಾಂಕ location *loc);

/** Set the \c destructor or \c printer associated with \c type.  */
ಶೂನ್ಯ semantic_type_ಸಂಕೇತ_props_ಗಣ (semantic_type *type,
                                   ಸಂಕೇತ_props_type ಬಗೆ,
                                   ಸಂಕೇತ_props ಸ್ಥಿರಾಂಕ *ಸಂಕೇತ_2);

/*----------------------------------.
| Symbol and semantic type tables.  |
`----------------------------------*/

/** Create the symbol and semantic type tables, and the built-in
    symbols.  */
ಶೂನ್ಯ symbols_new (ಶೂನ್ಯ);

/** Free all the memory allocated for symbols and semantic types.  */
ಶೂನ್ಯ symbols_free (ಶೂನ್ಯ);

/** Check that all the symbols are defined.

    Report any undefined symbols and consider them nonterminals.  */
ಶೂನ್ಯ symbols_check_defined (ಶೂನ್ಯ);

/** Sanity checks and #token_translations construction.

   Perform various sanity checks, assign symbol numbers, and set up
   #token_translations.  */
ಶೂನ್ಯ symbols_pack (ಶೂನ್ಯ);

#ಆದರೆ_ಅಂತ್ಯ /* !SYMTAB_H_ */
