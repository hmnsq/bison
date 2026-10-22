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

#جيڪڏهن_وصف_نه_ٿيل SYMTAB_H_
# define SYMTAB_H_

# include "assoc.h"
# include "location.h"
# include "scan-code.h"
# include "uniqstr.h"

/*----------.
| Symbols.  |
`----------*/

/** Symbol classes.  */
تعريفنوع تعداد
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
} نشان_class;


/** Internal token numbers. */
تعريفنوع صحيح نشان_عدد;
# define نشان_عدد_وڌ_کان_وڌ وڌ_کان_وڌ_عدد


تعريفنوع بنية نشان نشان;
تعريفنوع بنية sym_content sym_content;

/* Declaration status of a symbol.

   First, it is "undeclared".  Then, if "undeclared" and used in a
   %printer/%destructor, it is "used".  If not "declared" but used in
   a rule, it is "needed".  Finally, if declared (via a rule for
   nonterminals, or %token), it is "declared".

   When status are checked at the end, "declared" symbols are fine,
   "used" symbols trigger warnings, otherwise it's an error.  */

تعريفنوع تعداد
  {
    /** Used in the input file for an unknown reason (error).  */
    undeclared,
    /** Used by %destructor/%printer but not defined (warning).  */
    استعمال_ٿيل,
    /** Used in the grammar (rules) but not defined (error).  */
    needed,
    /** Defined with %type or %token (good).  */
    declared,
  } declaration_حالت;

تعداد ڪوڊ_props_type
  {
    destructor = 0,
    printer = 1,
  };
تعريفنوع تعداد ڪوڊ_props_type ڪوڊ_props_type;

تعداد { ڪوڊ_PROPS_سائيز = 2 };

بنية نشان
{
  /** The key, name of the symbol.  */
  uniqstr ٽيگ;

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
  bool location_جو_lhs;

  /** Points to the other in the symbol-string pair for an alias. */
  نشان *alias;

  /** Whether this symbol is the alias of another or not. */
  bool is_alias;

  /** All the info about the pointed symbol is there. */
  sym_content *content;
};

بنية sym_content
{
  /** The main symbol that denotes this content (it contains the
      possible alias). */
  نشان *نشان;

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
  ڪوڊ_props props[ڪوڊ_PROPS_سائيز];

  نشان_عدد عدد;
  location prec_loc;
  صحيح prec;
  assoc assoc;

  /** Token code, possibly specified by the user (%token FOO 42).  */
  صحيح ڪوڊ;

  نشان_class class;
  declaration_حالت حالت;
};

/** Fetch (or create) the symbol associated to KEY.  */
نشان *نشان_from_uniqstr (ثابت uniqstr ڪنجي, location loc);

/** Fetch (or create) the symbol associated to KEY.  */
نشان *نشان_get (ثابت حرف *ڪنجي, location loc);

/** Generate a dummy nonterminal.

   Its name cannot conflict with the user's names.  */
نشان *dummy_نشان_get (location loc);


/*--------------------.
| Methods on symbol.  |
`--------------------*/

/** Print a symbol (for debugging). */
فراغ نشان_print (نشان ثابت *s, FILE *f);

/** Is this a dummy nonterminal?  */
bool نشان_is_dummy (نشان ثابت *sym);

/** The name of the code_props type: "\%destructor" or "\%printer".  */
حرف ثابت *ڪوڊ_props_type_string (ڪوڊ_props_type قسم);

/** The name of the symbol that can be used as an identifier.
 ** Consider the alias if needed.
 ** Return 0 if there is none (e.g., the symbol is only defined as
 ** a string). */
uniqstr نشان_id_get (نشان ثابت *sym);

/**
 * Make \c str the literal string alias of \c sym.  Copy token number,
 * symbol number, and type from \c sym to \c str.
 */
فراغ نشان_make_alias (نشان *sym, نشان *str, location loc);

/**
 * This symbol is used as the lhs of a rule.  Record this location
 * as definition point, if not already done.
 */
فراغ نشان_location_as_lhs_سيٽ (نشان *sym, location loc);

/** Set the \c type_name associated with \c sym.

    Do nothing if passed 0 as \c type_name.  */
فراغ نشان_type_سيٽ (نشان *sym, uniqstr type_name, location loc);

/** Set the \c \%destructor or \c \%printer associated with \c sym.  */
فراغ نشان_ڪوڊ_props_سيٽ (نشان *sym, ڪوڊ_props_type قسم,
                            ڪوڊ_props ثابت *destructor);

/** Get the computed \c \%destructor or \c %printer for \c sym, which was
    initialized with \c code_props_none_init if there's no \c \%destructor or
    \c %printer.  */
ڪوڊ_props *نشان_ڪوڊ_props_get (نشان *sym, ڪوڊ_props_type قسم);

/** Set the \c precedence associated with \c sym.

    Ensure that \a symbol is a terminal.
    Do nothing if invoked with \c undef_assoc as \c assoc.  */
فراغ نشان_precedence_سيٽ (نشان *sym, صحيح prec, assoc a, location loc);

/** Set the \c class associated with \c sym.

    Whether \c declaring means whether this class definition comes
    from %nterm or %token (but not %type, prec/assoc, etc.).  A symbol
    can have "declaring" set only at most once.  */
فراغ نشان_class_سيٽ (نشان *sym, نشان_class class, location loc,
                       bool declaring);

/** Set the token \c code of \c sym, specified by the user at \c loc.  */
فراغ نشان_ڪوڊ_سيٽ (نشان *sym, صحيح ڪوڊ, location loc);



/*------------------.
| Special symbols.  |
`------------------*/

/** The error token. */
خارجي نشان *errtoken;
/** The token for unknown tokens.  */
خارجي نشان *undeftoken;
/** The end of input token.  */
خارجي نشان *eoftoken;
/** The genuine start symbol.

   $accept: start-symbol $end */
خارجي نشان *acceptsymbol;

/** Whether a symbol declared with a type tag.  */
خارجي bool ٽيگ_ڏٺل;


/*-------------------.
| Symbol Relations.  |
`-------------------*/

/* The symbol relations are represented by a directed graph. */

/* The id of a node */
تعريفنوع صحيح graphid;

تعريفنوع بنية symgraphlink symgraphlink;

بنية symgraphlink
{
  /** The second \c symbol or group of a precedence relation.
   * See \c symgraph. */
  graphid id;

  symgraphlink *ايندڙ;
};

/* Symbol precedence graph, to store the used precedence relations between
 * symbols. */

تعريفنوع بنية symgraph symgraph;

بنية symgraph
{
  /** Identifier for the node: equal to the number of the symbol. */
  graphid id;

  /** The list of related symbols that have a smaller precedence. */
  symgraphlink *succ;

  /** The list of related symbols that have a greater precedence. */
  symgraphlink *pred;
};

/** Register a new precedence relation as used. */

فراغ رجسٽر_precedence (graphid پهريون, graphid snd);

/** Print a warning for each symbol whose precedence and/or associativity
 * is useless. */

فراغ print_precedence_warnings (فراغ);

/*----------------------.
| Symbol associativity  |
`----------------------*/

فراغ رجسٽر_assoc (graphid i, graphid j);

/*-----------------.
| Semantic types.  |
`-----------------*/

/** A semantic type and its associated \c \%destructor and \c \%printer.

   Access the fields of this struct only through the interface functions in
   this file.  \sa symbol::destructor  */
تعريفنوع بنية {
  /** The key, name of the semantic type.  */
  uniqstr ٽيگ;

  /** The location of its first occurrence.  */
  location location;

  /** Its status : "undeclared", "used" or "declared".
      It cannot be "needed".  */
  declaration_حالت حالت;

  /** Any \c %destructor and %printer declared for this
      semantic type.  */
  ڪوڊ_props props[ڪوڊ_PROPS_سائيز];

} semantic_type;

/** Fetch (or create) the semantic type associated to KEY.  */
semantic_type *semantic_type_from_uniqstr (ثابت uniqstr ڪنجي,
                                           ثابت location *loc);

/** Fetch (or create) the semantic type associated to KEY.  */
semantic_type *semantic_type_get (ثابت حرف *ڪنجي, ثابت location *loc);

/** Set the \c destructor or \c printer associated with \c type.  */
فراغ semantic_type_ڪوڊ_props_سيٽ (semantic_type *type,
                                   ڪوڊ_props_type قسم,
                                   ڪوڊ_props ثابت *ڪوڊ);

/*----------------------------------.
| Symbol and semantic type tables.  |
`----------------------------------*/

/** Create the symbol and semantic type tables, and the built-in
    symbols.  */
فراغ symbols_new (فراغ);

/** Free all the memory allocated for symbols and semantic types.  */
فراغ symbols_free (فراغ);

/** Check that all the symbols are defined.

    Report any undefined symbols and consider them nonterminals.  */
فراغ symbols_check_defined (فراغ);

/** Sanity checks and #token_translations construction.

   Perform various sanity checks, assign symbol numbers, and set up
   #token_translations.  */
فراغ symbols_pack (فراغ);

#جيڪڏهن_پورو /* !SYMTAB_H_ */
