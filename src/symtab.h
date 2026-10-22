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

#यदि_अपरिभाषितम् SYMTAB_H_
# define SYMTAB_H_

# include "assoc.h"
# include "location.h"
# include "scan-code.h"
# include "uniqstr.h"

/*----------.
| Symbols.  |
`----------*/

/** Symbol classes.  */
प्रकारनिर्देश गणना
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
} चिह्नम्_class;


/** Internal token numbers. */
प्रकारनिर्देश पूर्णाङ्क चिह्नम्_सङ्ख्या;
# define चिह्नम्_सङ्ख्या_अधिकतमम् अधिकतम_पूर्णाङ्कः


प्रकारनिर्देश संरचना चिह्नम् चिह्नम्;
प्रकारनिर्देश संरचना sym_content sym_content;

/* Declaration status of a symbol.

   First, it is "undeclared".  Then, if "undeclared" and used in a
   %printer/%destructor, it is "used".  If not "declared" but used in
   a rule, it is "needed".  Finally, if declared (via a rule for
   nonterminals, or %token), it is "declared".

   When status are checked at the end, "declared" symbols are fine,
   "used" symbols trigger warnings, otherwise it's an error.  */

प्रकारनिर्देश गणना
  {
    /** Used in the input file for an unknown reason (error).  */
    undeclared,
    /** Used by %destructor/%printer but not defined (warning).  */
    प्रयुक्तम्,
    /** Used in the grammar (rules) but not defined (error).  */
    needed,
    /** Defined with %type or %token (good).  */
    declared,
  } declaration_स्थितिः;

गणना संहिता_props_type
  {
    destructor = 0,
    printer = 1,
  };
प्रकारनिर्देश गणना संहिता_props_type संहिता_props_type;

गणना { संहिता_PROPS_आकार = 2 };

संरचना चिह्नम्
{
  /** The key, name of the symbol.  */
  uniqstr चिह्नकम्;

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
  bool location_सम्बन्धि_lhs;

  /** Points to the other in the symbol-string pair for an alias. */
  चिह्नम् *alias;

  /** Whether this symbol is the alias of another or not. */
  bool is_alias;

  /** All the info about the pointed symbol is there. */
  sym_content *content;
};

संरचना sym_content
{
  /** The main symbol that denotes this content (it contains the
      possible alias). */
  चिह्नम् *चिह्नम्;

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
  संहिता_props props[संहिता_PROPS_आकार];

  चिह्नम्_सङ्ख्या सङ्ख्या;
  location prec_loc;
  पूर्णाङ्क prec;
  assoc assoc;

  /** Token code, possibly specified by the user (%token FOO 42).  */
  पूर्णाङ्क संहिता;

  चिह्नम्_class class;
  declaration_स्थितिः स्थितिः;
};

/** Fetch (or create) the symbol associated to KEY.  */
चिह्नम् *चिह्नम्_from_uniqstr (ध्रुव uniqstr कुञ्चिका, location loc);

/** Fetch (or create) the symbol associated to KEY.  */
चिह्नम् *चिह्नम्_get (ध्रुव वर्ण *कुञ्चिका, location loc);

/** Generate a dummy nonterminal.

   Its name cannot conflict with the user's names.  */
चिह्नम् *dummy_चिह्नम्_get (location loc);


/*--------------------.
| Methods on symbol.  |
`--------------------*/

/** Print a symbol (for debugging). */
शून्य चिह्नम्_print (चिह्नम् ध्रुव *s, FILE *f);

/** Is this a dummy nonterminal?  */
bool चिह्नम्_is_dummy (चिह्नम् ध्रुव *sym);

/** The name of the code_props type: "\%destructor" or "\%printer".  */
वर्ण ध्रुव *संहिता_props_type_string (संहिता_props_type विधा);

/** The name of the symbol that can be used as an identifier.
 ** Consider the alias if needed.
 ** Return 0 if there is none (e.g., the symbol is only defined as
 ** a string). */
uniqstr चिह्नम्_id_get (चिह्नम् ध्रुव *sym);

/**
 * Make \c str the literal string alias of \c sym.  Copy token number,
 * symbol number, and type from \c sym to \c str.
 */
शून्य चिह्नम्_make_alias (चिह्नम् *sym, चिह्नम् *str, location loc);

/**
 * This symbol is used as the lhs of a rule.  Record this location
 * as definition point, if not already done.
 */
शून्य चिह्नम्_location_as_lhs_समुच्चयः (चिह्नम् *sym, location loc);

/** Set the \c type_name associated with \c sym.

    Do nothing if passed 0 as \c type_name.  */
शून्य चिह्नम्_type_समुच्चयः (चिह्नम् *sym, uniqstr type_name, location loc);

/** Set the \c \%destructor or \c \%printer associated with \c sym.  */
शून्य चिह्नम्_संहिता_props_समुच्चयः (चिह्नम् *sym, संहिता_props_type विधा,
                            संहिता_props ध्रुव *destructor);

/** Get the computed \c \%destructor or \c %printer for \c sym, which was
    initialized with \c code_props_none_init if there's no \c \%destructor or
    \c %printer.  */
संहिता_props *चिह्नम्_संहिता_props_get (चिह्नम् *sym, संहिता_props_type विधा);

/** Set the \c precedence associated with \c sym.

    Ensure that \a symbol is a terminal.
    Do nothing if invoked with \c undef_assoc as \c assoc.  */
शून्य चिह्नम्_precedence_समुच्चयः (चिह्नम् *sym, पूर्णाङ्क prec, assoc a, location loc);

/** Set the \c class associated with \c sym.

    Whether \c declaring means whether this class definition comes
    from %nterm or %token (but not %type, prec/assoc, etc.).  A symbol
    can have "declaring" set only at most once.  */
शून्य चिह्नम्_class_समुच्चयः (चिह्नम् *sym, चिह्नम्_class class, location loc,
                       bool declaring);

/** Set the token \c code of \c sym, specified by the user at \c loc.  */
शून्य चिह्नम्_संहिता_समुच्चयः (चिह्नम् *sym, पूर्णाङ्क संहिता, location loc);



/*------------------.
| Special symbols.  |
`------------------*/

/** The error token. */
बाह्य चिह्नम् *errtoken;
/** The token for unknown tokens.  */
बाह्य चिह्नम् *undeftoken;
/** The end of input token.  */
बाह्य चिह्नम् *eoftoken;
/** The genuine start symbol.

   $accept: start-symbol $end */
बाह्य चिह्नम् *acceptsymbol;

/** Whether a symbol declared with a type tag.  */
बाह्य bool चिह्नकम्_आलोकितम्;


/*-------------------.
| Symbol Relations.  |
`-------------------*/

/* The symbol relations are represented by a directed graph. */

/* The id of a node */
प्रकारनिर्देश पूर्णाङ्क graphid;

प्रकारनिर्देश संरचना symgraphlink symgraphlink;

संरचना symgraphlink
{
  /** The second \c symbol or group of a precedence relation.
   * See \c symgraph. */
  graphid id;

  symgraphlink *अग्र;
};

/* Symbol precedence graph, to store the used precedence relations between
 * symbols. */

प्रकारनिर्देश संरचना symgraph symgraph;

संरचना symgraph
{
  /** Identifier for the node: equal to the number of the symbol. */
  graphid id;

  /** The list of related symbols that have a smaller precedence. */
  symgraphlink *succ;

  /** The list of related symbols that have a greater precedence. */
  symgraphlink *pred;
};

/** Register a new precedence relation as used. */

शून्य पञ्जिकाकोशः_precedence (graphid first, graphid snd);

/** Print a warning for each symbol whose precedence and/or associativity
 * is useless. */

शून्य print_precedence_warnings (शून्य);

/*----------------------.
| Symbol associativity  |
`----------------------*/

शून्य पञ्जिकाकोशः_assoc (graphid i, graphid j);

/*-----------------.
| Semantic types.  |
`-----------------*/

/** A semantic type and its associated \c \%destructor and \c \%printer.

   Access the fields of this struct only through the interface functions in
   this file.  \sa symbol::destructor  */
प्रकारनिर्देश संरचना {
  /** The key, name of the semantic type.  */
  uniqstr चिह्नकम्;

  /** The location of its first occurrence.  */
  location location;

  /** Its status : "undeclared", "used" or "declared".
      It cannot be "needed".  */
  declaration_स्थितिः स्थितिः;

  /** Any \c %destructor and %printer declared for this
      semantic type.  */
  संहिता_props props[संहिता_PROPS_आकार];

} semantic_type;

/** Fetch (or create) the semantic type associated to KEY.  */
semantic_type *semantic_type_from_uniqstr (ध्रुव uniqstr कुञ्चिका,
                                           ध्रुव location *loc);

/** Fetch (or create) the semantic type associated to KEY.  */
semantic_type *semantic_type_get (ध्रुव वर्ण *कुञ्चिका, ध्रुव location *loc);

/** Set the \c destructor or \c printer associated with \c type.  */
शून्य semantic_type_संहिता_props_समुच्चयः (semantic_type *type,
                                   संहिता_props_type विधा,
                                   संहिता_props ध्रुव *संहिता);

/*----------------------------------.
| Symbol and semantic type tables.  |
`----------------------------------*/

/** Create the symbol and semantic type tables, and the built-in
    symbols.  */
शून्य symbols_new (शून्य);

/** Free all the memory allocated for symbols and semantic types.  */
शून्य symbols_free (शून्य);

/** Check that all the symbols are defined.

    Report any undefined symbols and consider them nonterminals.  */
शून्य symbols_check_defined (शून्य);

/** Sanity checks and #token_translations construction.

   Perform various sanity checks, assign symbol numbers, and set up
   #token_translations.  */
शून्य symbols_pack (शून्य);

#यदि_अन्तः /* !SYMTAB_H_ */
