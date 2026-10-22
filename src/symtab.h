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

#ifndef SYMTAB_H_
# define SYMTAB_H_

# include "assoc.h"
# include "location.h"
# include "scan-code.h"
# include "uniqstr.h"

/*----------.
| Symbols.  |
`----------*/

/** Symbol classes.  */
প্রকারনির্দেশ গণনা
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
} প্ৰতীক_class;


/** Internal token numbers. */
প্রকারনির্দেশ পূর্ণাঙ্ক প্ৰতীক_সংখ্যা;
# define প্ৰতীক_সংখ্যা_MAXIMUM INT_MAX


প্রকারনির্দেশ সংরচনা প্ৰতীক প্ৰতীক;
প্রকারনির্দেশ সংরচনা sym_content sym_content;

/* Declaration status of a symbol.

   First, it is "undeclared".  Then, if "undeclared" and used in a
   %printer/%destructor, it is "used".  If not "declared" but used in
   a rule, it is "needed".  Finally, if declared (via a rule for
   nonterminals, or %token), it is "declared".

   When status are checked at the end, "declared" symbols are fine,
   "used" symbols trigger warnings, otherwise it's an error.  */

প্রকারনির্দেশ গণনা
  {
    /** Used in the input file for an unknown reason (error).  */
    undeclared,
    /** Used by %destructor/%printer but not defined (warning).  */
    used,
    /** Used in the grammar (rules) but not defined (error).  */
    needed,
    /** Defined with %type or %token (good).  */
    declared,
  } declaration_status;

গণনা code_props_type
  {
    destructor = 0,
    printer = 1,
  };
প্রকারনির্দেশ গণনা code_props_type code_props_type;

গণনা { CODE_PROPS_SIZE = 2 };

সংরচনা প্ৰতীক
{
  /** The key, name of the symbol.  */
  uniqstr tag;

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
  bool location_of_lhs;

  /** Points to the other in the symbol-string pair for an alias. */
  প্ৰতীক *alias;

  /** Whether this symbol is the alias of another or not. */
  bool is_alias;

  /** All the info about the pointed symbol is there. */
  sym_content *content;
};

সংরচনা sym_content
{
  /** The main symbol that denotes this content (it contains the
      possible alias). */
  প্ৰতীক *প্ৰতীক;

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
  code_props props[CODE_PROPS_SIZE];

  প্ৰতীক_সংখ্যা সংখ্যা;
  location prec_loc;
  পূর্ণাঙ্ক prec;
  assoc assoc;

  /** Token code, possibly specified by the user (%token FOO 42).  */
  পূর্ণাঙ্ক code;

  প্ৰতীক_class class;
  declaration_status status;
};

/** Fetch (or create) the symbol associated to KEY.  */
প্ৰতীক *প্ৰতীক_from_uniqstr (ধ্রুৱ uniqstr চাবি, location loc);

/** Fetch (or create) the symbol associated to KEY.  */
প্ৰতীক *প্ৰতীক_get (ধ্রুৱ ৱর্ণ *চাবি, location loc);

/** Generate a dummy nonterminal.

   Its name cannot conflict with the user's names.  */
প্ৰতীক *dummy_প্ৰতীক_get (location loc);


/*--------------------.
| Methods on symbol.  |
`--------------------*/

/** Print a symbol (for debugging). */
শূন্য প্ৰতীক_print (প্ৰতীক ধ্রুৱ *s, FILE *f);

/** Is this a dummy nonterminal?  */
bool প্ৰতীক_is_dummy (প্ৰতীক ধ্রুৱ *sym);

/** The name of the code_props type: "\%destructor" or "\%printer".  */
ৱর্ণ ধ্রুৱ *code_props_type_string (code_props_type kind);

/** The name of the symbol that can be used as an identifier.
 ** Consider the alias if needed.
 ** Return 0 if there is none (e.g., the symbol is only defined as
 ** a string). */
uniqstr প্ৰতীক_id_get (প্ৰতীক ধ্রুৱ *sym);

/**
 * Make \c str the literal string alias of \c sym.  Copy token number,
 * symbol number, and type from \c sym to \c str.
 */
শূন্য প্ৰতীক_make_alias (প্ৰতীক *sym, প্ৰতীক *str, location loc);

/**
 * This symbol is used as the lhs of a rule.  Record this location
 * as definition point, if not already done.
 */
শূন্য প্ৰতীক_location_as_lhs_set (প্ৰতীক *sym, location loc);

/** Set the \c type_name associated with \c sym.

    Do nothing if passed 0 as \c type_name.  */
শূন্য প্ৰতীক_type_set (প্ৰতীক *sym, uniqstr type_name, location loc);

/** Set the \c \%destructor or \c \%printer associated with \c sym.  */
শূন্য প্ৰতীক_code_props_set (প্ৰতীক *sym, code_props_type kind,
                            code_props ধ্রুৱ *destructor);

/** Get the computed \c \%destructor or \c %printer for \c sym, which was
    initialized with \c code_props_none_init if there's no \c \%destructor or
    \c %printer.  */
code_props *প্ৰতীক_code_props_get (প্ৰতীক *sym, code_props_type kind);

/** Set the \c precedence associated with \c sym.

    Ensure that \a symbol is a terminal.
    Do nothing if invoked with \c undef_assoc as \c assoc.  */
শূন্য প্ৰতীক_precedence_set (প্ৰতীক *sym, পূর্ণাঙ্ক prec, assoc a, location loc);

/** Set the \c class associated with \c sym.

    Whether \c declaring means whether this class definition comes
    from %nterm or %token (but not %type, prec/assoc, etc.).  A symbol
    can have "declaring" set only at most once.  */
শূন্য প্ৰতীক_class_set (প্ৰতীক *sym, প্ৰতীক_class class, location loc,
                       bool declaring);

/** Set the token \c code of \c sym, specified by the user at \c loc.  */
শূন্য প্ৰতীক_code_set (প্ৰতীক *sym, পূর্ণাঙ্ক code, location loc);



/*------------------.
| Special symbols.  |
`------------------*/

/** The error token. */
বাহ্য প্ৰতীক *errtoken;
/** The token for unknown tokens.  */
বাহ্য প্ৰতীক *undeftoken;
/** The end of input token.  */
বাহ্য প্ৰতীক *eoftoken;
/** The genuine start symbol.

   $accept: start-symbol $end */
বাহ্য প্ৰতীক *acceptsymbol;

/** Whether a symbol declared with a type tag.  */
বাহ্য bool tag_seen;


/*-------------------.
| Symbol Relations.  |
`-------------------*/

/* The symbol relations are represented by a directed graph. */

/* The id of a node */
প্রকারনির্দেশ পূর্ণাঙ্ক graphid;

প্রকারনির্দেশ সংরচনা symgraphlink symgraphlink;

সংরচনা symgraphlink
{
  /** The second \c symbol or group of a precedence relation.
   * See \c symgraph. */
  graphid id;

  symgraphlink *next;
};

/* Symbol precedence graph, to store the used precedence relations between
 * symbols. */

প্রকারনির্দেশ সংরচনা symgraph symgraph;

সংরচনা symgraph
{
  /** Identifier for the node: equal to the number of the symbol. */
  graphid id;

  /** The list of related symbols that have a smaller precedence. */
  symgraphlink *succ;

  /** The list of related symbols that have a greater precedence. */
  symgraphlink *pred;
};

/** Register a new precedence relation as used. */

শূন্য register_precedence (graphid first, graphid snd);

/** Print a warning for each symbol whose precedence and/or associativity
 * is useless. */

শূন্য print_precedence_warnings (শূন্য);

/*----------------------.
| Symbol associativity  |
`----------------------*/

শূন্য register_assoc (graphid i, graphid j);

/*-----------------.
| Semantic types.  |
`-----------------*/

/** A semantic type and its associated \c \%destructor and \c \%printer.

   Access the fields of this struct only through the interface functions in
   this file.  \sa symbol::destructor  */
প্রকারনির্দেশ সংরচনা {
  /** The key, name of the semantic type.  */
  uniqstr tag;

  /** The location of its first occurrence.  */
  location location;

  /** Its status : "undeclared", "used" or "declared".
      It cannot be "needed".  */
  declaration_status status;

  /** Any \c %destructor and %printer declared for this
      semantic type.  */
  code_props props[CODE_PROPS_SIZE];

} semantic_type;

/** Fetch (or create) the semantic type associated to KEY.  */
semantic_type *semantic_type_from_uniqstr (ধ্রুৱ uniqstr চাবি,
                                           ধ্রুৱ location *loc);

/** Fetch (or create) the semantic type associated to KEY.  */
semantic_type *semantic_type_get (ধ্রুৱ ৱর্ণ *চাবি, ধ্রুৱ location *loc);

/** Set the \c destructor or \c printer associated with \c type.  */
শূন্য semantic_type_code_props_set (semantic_type *type,
                                   code_props_type kind,
                                   code_props ধ্রুৱ *code);

/*----------------------------------.
| Symbol and semantic type tables.  |
`----------------------------------*/

/** Create the symbol and semantic type tables, and the built-in
    symbols.  */
শূন্য symbols_new (শূন্য);

/** Free all the memory allocated for symbols and semantic types.  */
শূন্য symbols_free (শূন্য);

/** Check that all the symbols are defined.

    Report any undefined symbols and consider them nonterminals.  */
শূন্য symbols_check_defined (শূন্য);

/** Sanity checks and #token_translations construction.

   Perform various sanity checks, assign symbol numbers, and set up
   #token_translations.  */
শূন্য symbols_pack (শূন্য);

#endif /* !SYMTAB_H_ */
