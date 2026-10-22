/* A Bison parser, made by GNU Bison 3.8.2.30-82269.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2022 Free Software Foundation,
   Inc.

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

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#ವ್ಯಾಖ್ಯಾನಿಸು YYBISON 30802

/* Bison version string.  */
#ವ್ಯಾಖ್ಯಾನಿಸು YYBISON_ಆವೃತ್ತಿ "3.8.2.30-82269"

/* Skeleton name.  */
#ವ್ಯಾಖ್ಯಾನಿಸು YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#ವ್ಯಾಖ್ಯಾನಿಸು YYPURE 2

/* Push parsers.  */
#ವ್ಯಾಖ್ಯಾನಿಸು YYPUSH 0

/* Pull parsers.  */
#ವ್ಯಾಖ್ಯಾನಿಸು YYPULL 1

/* "%code top" blocks.  */

  /* On column 0 to please syntax-check.  */
#ಅನ್ತರ್ಭಾವ <config.h>

/* Substitute the type names.  */
#ವ್ಯಾಖ್ಯಾನಿಸು YYSTYPE         GRAM_STYPE
#ವ್ಯಾಖ್ಯಾನಿಸು YYLTYPE         GRAM_LTYPE
/* Substitute the variable and function names.  */
#ವ್ಯಾಖ್ಯಾನಿಸು yyparse         gram_parse
#ವ್ಯಾಖ್ಯಾನಿಸು yylex           gram_lex
#ವ್ಯಾಖ್ಯಾನಿಸು yyerror         gram_error
#ವ್ಯಾಖ್ಯಾನಿಸು yydebug         gram_debug
#ವ್ಯಾಖ್ಯಾನಿಸು yynerrs         gram_nerrs


# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  ಇಲ್ಲದಿದ್ದರೆ
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  ಆದರೆ defined __cplusplus
#   ಆದರೆ 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   ಇಲ್ಲದಿದ್ದರೆ
#    define YY_NULLPTR 0
#   endif
#  ಇಲ್ಲದಿದ್ದರೆ
#   define YY_NULLPTR ((ಶೂನ್ಯ*)0)
#  endif
# endif

#ಅನ್ತರ್ಭಾವ "src/parse-gram.h"
/* Symbol kind.  */
ಎಣಿಕೆ yysymbol_ಬಗೆ_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_STRING = 3,                     /* "string"  */
  YYSYMBOL_TSTRING = 4,                    /* "translatable string"  */
  YYSYMBOL_PERCENT_TOKEN = 5,              /* "%token"  */
  YYSYMBOL_PERCENT_NTERM = 6,              /* "%nterm"  */
  YYSYMBOL_PERCENT_TYPE = 7,               /* "%type"  */
  YYSYMBOL_PERCENT_DESTRUCTOR = 8,         /* "%destructor"  */
  YYSYMBOL_PERCENT_PRINTER = 9,            /* "%printer"  */
  YYSYMBOL_PERCENT_ಎಡ = 10,              /* "%left"  */
  YYSYMBOL_PERCENT_ಬಲ = 11,             /* "%right"  */
  YYSYMBOL_PERCENT_NONASSOC = 12,          /* "%nonassoc"  */
  YYSYMBOL_PERCENT_PRECEDENCE = 13,        /* "%precedence"  */
  YYSYMBOL_PERCENT_PREC = 14,              /* "%prec"  */
  YYSYMBOL_PERCENT_DPREC = 15,             /* "%dprec"  */
  YYSYMBOL_PERCENT_ವಿಲೀನ = 16,             /* "%merge"  */
  YYSYMBOL_PERCENT_ಸಂಕೇತ = 17,              /* "%code"  */
  YYSYMBOL_PERCENT_DEFAULT_PREC = 18,      /* "%default-prec"  */
  YYSYMBOL_PERCENT_DEFINE = 19,            /* "%define"  */
  YYSYMBOL_PERCENT_ERROR_VERBOSE = 20,     /* "%error-verbose"  */
  YYSYMBOL_PERCENT_EXPECT = 21,            /* "%expect"  */
  YYSYMBOL_PERCENT_EXPECT_RR = 22,         /* "%expect-rr"  */
  YYSYMBOL_PERCENT_FILE_ಪೂರ್ವಪ್ರತ್ಯಯ = 23,       /* "%file-prefix"  */
  YYSYMBOL_PERCENT_FLAG = 24,              /* "%<flag>"  */
  YYSYMBOL_PERCENT_GLR_PARSER = 25,        /* "%glr-parser"  */
  YYSYMBOL_PERCENT_ಶೀರ್ಷಿಕೆ = 26,            /* "%header"  */
  YYSYMBOL_PERCENT_INITIAL_ACTION = 27,    /* "%initial-action"  */
  YYSYMBOL_PERCENT_LANGUAGE = 28,          /* "%language"  */
  YYSYMBOL_PERCENT_NAME_ಪೂರ್ವಪ್ರತ್ಯಯ = 29,       /* "%name-prefix"  */
  YYSYMBOL_PERCENT_NO_DEFAULT_PREC = 30,   /* "%no-default-prec"  */
  YYSYMBOL_PERCENT_NO_ಸಾಲುಗಳು = 31,          /* "%no-lines"  */
  YYSYMBOL_PERCENT_NONDETERMINISTIC_PARSER = 32, /* "%nondeterministic-parser"  */
  YYSYMBOL_PERCENT_OUTPUT = 33,            /* "%output"  */
  YYSYMBOL_PERCENT_ಶುದ್ಧ_PARSER = 34,       /* "%pure-parser"  */
  YYSYMBOL_PERCENT_REQUIRE = 35,           /* "%require"  */
  YYSYMBOL_PERCENT_SKELETON = 36,          /* "%skeleton"  */
  YYSYMBOL_PERCENT_ಆರಂಭ = 37,             /* "%start"  */
  YYSYMBOL_PERCENT_TOKEN_ಕೋಷ್ಟಕ = 38,       /* "%token-table"  */
  YYSYMBOL_PERCENT_VERBOSE = 39,           /* "%verbose"  */
  YYSYMBOL_PERCENT_YACC = 40,              /* "%yacc"  */
  YYSYMBOL_BRACED_ಸಂಕೇತ = 41,               /* "{...}"  */
  YYSYMBOL_BRACED_PREDICATE = 42,          /* "%?{...}"  */
  YYSYMBOL_BRACKETED_ID = 43,              /* "[identifier]"  */
  YYSYMBOL_CHAR_LITERAL = 44,              /* "character literal"  */
  YYSYMBOL_COLON = 45,                     /* ":"  */
  YYSYMBOL_EPILOGUE = 46,                  /* "epilogue"  */
  YYSYMBOL_EQUAL = 47,                     /* "="  */
  YYSYMBOL_ID = 48,                        /* "identifier"  */
  YYSYMBOL_ID_COLON = 49,                  /* "identifier:"  */
  YYSYMBOL_PERCENT_PERCENT = 50,           /* "%%"  */
  YYSYMBOL_PIPE = 51,                      /* "|"  */
  YYSYMBOL_PROLOGUE = 52,                  /* "%{...%}"  */
  YYSYMBOL_SEMICOLON = 53,                 /* ";"  */
  YYSYMBOL_ಟ್ಯಾಗ್ = 54,                       /* "<tag>"  */
  YYSYMBOL_ಟ್ಯಾಗ್_ANY = 55,                   /* "<*>"  */
  YYSYMBOL_ಟ್ಯಾಗ್_NONE = 56,                  /* "<>"  */
  YYSYMBOL_INT_LITERAL = 57,               /* "integer literal"  */
  YYSYMBOL_PERCENT_PARAM = 58,             /* "%param"  */
  YYSYMBOL_PERCENT_UNION = 59,             /* "%union"  */
  YYSYMBOL_PERCENT_ಖಾಲಿಗಳು = 60,             /* "%empty"  */
  YYSYMBOL_YYACCEPT = 61,                  /* $accept  */
  YYSYMBOL_ಒಳಹರಿವು = 62,                     /*ಒಳಹರಿವು*/
  YYSYMBOL_prologue_declarations = 63,     /* prologue_declarations  */
  YYSYMBOL_prologue_declaration = 64,      /* prologue_declaration  */
  YYSYMBOL_65_1 = 65,                      /* $@1  */
  YYSYMBOL_params = 66,                    /* params  */
  YYSYMBOL_grammar_declaration = 67,       /* grammar_declaration  */
  YYSYMBOL_ಸಂಕೇತ_props_type = 68,           /* code_props_type  */
  YYSYMBOL_union_name = 69,                /* union_name  */
  YYSYMBOL_ಚಿಹ್ನೆ_declaration = 70,        /* symbol_declaration  */
  YYSYMBOL_71_2 = 71,                      /* $@2  */
  YYSYMBOL_72_3 = 72,                      /* $@3  */
  YYSYMBOL_73_4 = 73,                      /* $@4  */
  YYSYMBOL_precedence_declarator = 74,     /* precedence_declarator  */
  YYSYMBOL_75_string_opt = 75,             /* string.opt  */
  YYSYMBOL_76_ಟ್ಯಾಗ್_opt = 76,                /* tag.opt  */
  YYSYMBOL_generic_symlist = 77,           /* generic_symlist  */
  YYSYMBOL_generic_symlist_ಅಂಶ = 78,      /* generic_symlist_item  */
  YYSYMBOL_ಟ್ಯಾಗ್_2 = 79,                       /*ಟ್ಯಾಗ್*/
  YYSYMBOL_nterm_decls = 80,               /* nterm_decls  */
  YYSYMBOL_token_decls = 81,               /* token_decls  */
  YYSYMBOL_82_token_decl_1 = 82,           /* token_decl.1  */
  YYSYMBOL_token_decl = 83,                /* token_decl  */
  YYSYMBOL_84_int_opt = 84,                /* int.opt  */
  YYSYMBOL_alias = 85,                     /* alias  */
  YYSYMBOL_token_decls_for_prec = 86,      /* token_decls_for_prec  */
  YYSYMBOL_87_token_decl_for_prec_1 = 87,  /* token_decl_for_prec.1  */
  YYSYMBOL_token_decl_for_prec = 88,       /* token_decl_for_prec  */
  YYSYMBOL_ಚಿಹ್ನೆ_decls = 89,              /* symbol_decls  */
  YYSYMBOL_90_symbols_1 = 90,              /* symbols.1  */
  YYSYMBOL_grammar = 91,                   /* grammar  */
  YYSYMBOL_ನಿಯಮಗಳು_or_grammar_declaration = 92, /* rules_or_grammar_declaration  */
  YYSYMBOL_ನಿಯಮಗಳು = 93,                     /*ನಿಯಮಗಳು*/
  YYSYMBOL_94_5 = 94,                      /* $@5  */
  YYSYMBOL_95_rhses_1 = 95,                /* rhses.1  */
  YYSYMBOL_rhs = 96,                       /* rhs  */
  YYSYMBOL_97_named_ref_opt = 97,          /* named_ref.opt  */
  YYSYMBOL_variable = 98,                  /* variable  */
  YYSYMBOL_ಮೌಲ್ಯ = 99,                     /*ಮೌಲ್ಯ*/
  YYSYMBOL_id = 100,                       /* id  */
  YYSYMBOL_id_colon = 101,                 /* id_colon  */
  YYSYMBOL_ಚಿಹ್ನೆ = 102,                   /*ಚಿಹ್ನೆ*/
  YYSYMBOL_string_as_id = 103,             /* string_as_id  */
  YYSYMBOL_104_epilogue_opt = 104          /* epilogue.opt  */
};
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಎಣಿಕೆ yysymbol_ಬಗೆ_t yysymbol_ಬಗೆ_t;



/* Unqualified %code blocks.  */

  #ಅನ್ತರ್ಭಾವ "system.h"

  #ಅನ್ತರ್ಭಾವ <c-ctype.h>
  #ಅನ್ತರ್ಭಾವ <quotearg.h>
  #ಅನ್ತರ್ಭಾವ <vasnprintf.h>
  #ಅನ್ತರ್ಭಾವ <xmemdup0.h>

  #ಅನ್ತರ್ಭಾವ "complain.h"
  #ಅನ್ತರ್ಭಾವ "conflicts.h"
  #ಅನ್ತರ್ಭಾವ "files.h"
  #ಅನ್ತರ್ಭಾವ "getargs.h"
  #ಅನ್ತರ್ಭಾವ "gram.h"
  #ಅನ್ತರ್ಭಾವ "named-ref.h"
  #ಅನ್ತರ್ಭಾವ "reader.h"
  #ಅನ್ತರ್ಭಾವ "scan-code.h"
  #ಅನ್ತರ್ಭಾವ "scan-gram.h"
  #ಅನ್ತರ್ಭಾವ "strversion.h"

  /* Pretend to be at least that version, to check features published
     in that version while developping it.  */
  ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* api_ಆವೃತ್ತಿ = "3.8";

  ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ current_prec = 0;
  ಸ್ಥಿರ location current_lhs_loc;
  ಸ್ಥಿರ named_ref *current_lhs_named_ref;
  ಸ್ಥಿರ ಚಿಹ್ನೆ *current_lhs_ಚಿಹ್ನೆ;
  ಸ್ಥಿರ ಚಿಹ್ನೆ_class current_class = unknown_sym;

  /** Set the new current left-hand side symbol, possibly common
   * to several right-hand side parts of rule.
   */
  ಸ್ಥಿರ ಶೂನ್ಯ current_lhs (ಚಿಹ್ನೆ *sym, location loc, named_ref *ref);

  #ವ್ಯಾಖ್ಯಾನಿಸು YYLLOC_DEFAULT(Current, Rhs, N)         \
    (Current) = lloc_default (Rhs, N)
  ಸ್ಥಿರ YYLTYPE lloc_default (YYLTYPE ಸ್ಥಿರಾಂಕ *, ಪೂರ್ಣಾಂಕ);

  #ವ್ಯಾಖ್ಯಾನಿಸು YY_LOCATION_PRINT(File, Loc)            \
    location_print (Loc, File)

  /* Strip initial '{' and final '}' (must be first and last characters).
     Return the result.  */
  ಸ್ಥಿರ ಅಕ್ಷರ *strip_braces (ಅಕ್ಷರ *ಸಂಕೇತ_2);

  /* Convert CODE by calling code_props_plain_init if PLAIN, otherwise
     code_props_symbol_action_init.  Calls
     gram_scanner_last_string_free to release the latest string from
     the scanner (should be CODE). */
  ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *translate_ಸಂಕೇತ (ಅಕ್ಷರ *ಸಂಕೇತ_2, location loc, bool plain);

  /* Convert CODE by calling code_props_plain_init after having
     stripped the first and last characters (expected to be '{', and
     '}').  Calls gram_scanner_last_string_free to release the latest
     string from the scanner (should be CODE). */
  ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *translate_ಸಂಕೇತ_braceless (ಅಕ್ಷರ *ಸಂಕೇತ_2, location loc);

  /* Handle a %header directive.  */
  ಸ್ಥಿರ ಶೂನ್ಯ handle_ಶೀರ್ಷಿಕೆ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಮೌಲ್ಯ);

  /* Handle a %error-verbose directive.  */
  ಸ್ಥಿರ ಶೂನ್ಯ handle_error_verbose (location ಸ್ಥಿರಾಂಕ *loc, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *directive);

  /* Handle a %file-prefix directive.  */
  ಸ್ಥಿರ ಶೂನ್ಯ handle_file_ಪೂರ್ವಪ್ರತ್ಯಯ (location ಸ್ಥಿರಾಂಕ *loc,
                                  location ಸ್ಥಿರಾಂಕ *dir_loc,
                                  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *directive, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಮೌಲ್ಯ);

  /* Handle a %language directive.  */
  ಸ್ಥಿರ ಶೂನ್ಯ handle_language (location ಸ್ಥಿರಾಂಕ *loc, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *lang);

  /* Handle a %name-prefix directive.  */
  ಸ್ಥಿರ ಶೂನ್ಯ handle_name_ಪೂರ್ವಪ್ರತ್ಯಯ (location ಸ್ಥಿರಾಂಕ *loc,
                                  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *directive, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಮೌಲ್ಯ);

  /* Handle a %pure-parser directive.  */
  ಸ್ಥಿರ ಶೂನ್ಯ handle_ಶುದ್ಧ_parser (location ಸ್ಥಿರಾಂಕ *loc, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *directive);

  /* Handle a %require directive.  */
  ಸ್ಥಿರ ಶೂನ್ಯ handle_require (location ಸ್ಥಿರಾಂಕ *loc, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಆವೃತ್ತಿ);

  /* Handle a %skeleton directive.  */
  ಸ್ಥಿರ ಶೂನ್ಯ handle_skeleton (location ಸ್ಥಿರಾಂಕ *loc, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *skel);

  /* Handle a %yacc directive.  */
  ಸ್ಥಿರ ಶೂನ್ಯ handle_yacc (location ಸ್ಥಿರಾಂಕ *loc);

  /* Implementation of yyerror.  */
  ಸ್ಥಿರ ಶೂನ್ಯ gram_error (location ಸ್ಥಿರಾಂಕ *, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *);

  /* A string that describes a char (e.g., 'a' -> "'a'").  */
  ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *char_name (ಅಕ್ಷರ);

  /* Add style to semantic values in traces.  */
  ಸ್ಥಿರ ಶೂನ್ಯ tron (FILE *yyo);
  ಸ್ಥಿರ ಶೂನ್ಯ troff (FILE *yyo);

  /* Interpret a quoted string (such as `"Hello, \"World\"\n\""`).
     Manages the memory of the result.  */
  ಸ್ಥಿರ ಅಕ್ಷರ *unquote (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str);

  /* Discard the latest unquoted string.  */
  ಸ್ಥಿರ ಶೂನ್ಯ unquote_free (ಅಕ್ಷರ *ಕೊನೆಯ_string);

  /** Add a lex-param and/or a parse-param.
   *
   * \param type  where to push this formal argument.
   * \param decl  the formal argument.  Destroyed.
   * \param loc   the location in the source.
   */
  ಸ್ಥಿರ ಶೂನ್ಯ ಕೂಡಿಸು_param (param_type type, ಅಕ್ಷರ *decl, location loc);
  ಸ್ಥಿರ param_type current_param = param_none;


#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ಗಿಡ್ಡ
# undef ಗಿಡ್ಡ
#ಆದರೆ_ಅಂತ್ಯ

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# ಆದರೆ defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#ಆದರೆ_ಅಂತ್ಯ

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __INT_LEAST8_MAX__
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ __INT_LEAST8_TYPE__ yytype_int8;
#ಅಥವಾಆದರೆ defined YY_STDINT_H
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ int_least8_t yytype_int8;
#ಇಲ್ಲದಿದ್ದರೆ
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಚಿಹ್ನೆಯುಳ್ಳ ಅಕ್ಷರ yytype_int8;
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __INT_LEAST16_MAX__
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ __INT_LEAST16_TYPE__ yytype_int16;
#ಅಥವಾಆದರೆ defined YY_STDINT_H
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ int_least16_t yytype_int16;
#ಇಲ್ಲದಿದ್ದರೆ
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಗಿಡ್ಡ yytype_int16;
#ಆದರೆ_ಅಂತ್ಯ

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ __UINT_LEAST8_TYPE__ yytype_uint8;
#ಅಥವಾಆದರೆ (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ)
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ uint_least8_t yytype_uint8;
#ಅಥವಾಆದರೆ !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ yytype_uint8;
#ಇಲ್ಲದಿದ್ದರೆ
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಗಿಡ್ಡ yytype_uint8;
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ __UINT_LEAST16_TYPE__ yytype_uint16;
#ಅಥವಾಆದರೆ (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ)
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ uint_least16_t yytype_uint16;
#ಅಥವಾಆದರೆ !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಚಿಹ್ನೆರಹಿತ ಗಿಡ್ಡ yytype_uint16;
#ಇಲ್ಲದಿದ್ದರೆ
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಪೂರ್ಣಾಂಕ yytype_uint16;
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ YYPTRDIFF_T
# ಆದರೆ defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_ಗರಿಷ್ಠ __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_ಗರಿಷ್ಠ PTRDIFF_MAX
# ಇಲ್ಲದಿದ್ದರೆ
#  define YYPTRDIFF_T ದೀರ್ಘ
#  define YYPTRDIFF_ಗರಿಷ್ಠ LONG_MAX
# endif
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# ಇಲ್ಲದಿದ್ದರೆ
#  define YYSIZE_T ಚಿಹ್ನೆರಹಿತ
# endif
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು YYSIZE_ಗರಿಷ್ಠ                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_ಗರಿಷ್ಠ < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_ಗರಿಷ್ಠ                         \
            : YY_CAST (YYSIZE_T, -1)))

#ವ್ಯಾಖ್ಯಾನಿಸು YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, ಗಾತ್ರ (X))


/* Stored state numbers (used for stacks). */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ yytype_uint8 yy_state_t;

/* State numbers in computations.  */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಪೂರ್ಣಾಂಕ yy_state_fast_t;

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ YY_
# ಆದರೆ defined YYENABLE_NLS && YYENABLE_NLS
#  ಆದರೆ ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ N_
# define N_(Msgid) Msgid
#ಆದರೆ_ಅಂತ್ಯ


#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ YY_ATTRIBUTE_ಶುದ್ಧ
# ಆದರೆ defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_ಶುದ್ಧ __attribute__ ((__pure__))
# ಇಲ್ಲದಿದ್ದರೆ
#  define YY_ATTRIBUTE_ಶುದ್ಧ
# endif
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ YY_ATTRIBUTE_ಬಳಸದ
# ಆದರೆ defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_ಬಳಸದ __attribute__ ((__unused__))
# ಇಲ್ಲದಿದ್ದರೆ
#  define YY_ATTRIBUTE_ಬಳಸದ
# endif
#ಆದರೆ_ಅಂತ್ಯ

/* Suppress unused-variable warnings by "using" E.  */
#ಆದರೆ ! defined lint || defined __GNUC__
# define YY_USE(E) ((ಶೂನ್ಯ) (E))
#ಇಲ್ಲದಿದ್ದರೆ
# define YY_USE(E) /*ಖಾಲಿಗಳು*/
#ಆದರೆ_ಅಂತ್ಯ

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#ಆದರೆ defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# ಆದರೆ __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# ಇಲ್ಲದಿದ್ದರೆ
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_ಕೊನೆ      \
    _Pragma ("GCC diagnostic pop")
#ಇಲ್ಲದಿದ್ದರೆ
# define YY_INITIAL_ಮೌಲ್ಯ(ಮೌಲ್ಯ_4) ಮೌಲ್ಯ_4
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_ಕೊನೆ
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ YY_INITIAL_ಮೌಲ್ಯ
# define YY_INITIAL_ಮೌಲ್ಯ(ಮೌಲ್ಯ_4) /* Nothing. */
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_ಕೊನೆ            \
    _Pragma ("GCC diagnostic pop")
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_ಕೊನೆ
#ಆದರೆ_ಅಂತ್ಯ


#ವ್ಯಾಖ್ಯಾನಿಸು YY_ASSERT(E) ((ಶೂನ್ಯ) (0 && (E)))

#ಆದರೆ 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) ಮಾಡು { /*ಖಾಲಿಗಳು*/; } ಆಗ (0)
#  ifndef YYSTACK_ALLOC_ಗರಿಷ್ಠ
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_ಗರಿಷ್ಠ 4032 /* reasonable circa 2006 */
#  endif
# ಇಲ್ಲದಿದ್ದರೆ
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_ಗರಿಷ್ಠ
#   define YYSTACK_ALLOC_ಗರಿಷ್ಠ YYSIZE_ಗರಿಷ್ಠ
#  endif
#  ಆದರೆ (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined ಸ್ಮರಣೆ_ಕೊಡು) \
             && (defined YYFREE || defined ಬಿಡುಗಡೆ)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC ಸ್ಮರಣೆ_ಕೊಡು
#   ಆದರೆ ! defined ಸ್ಮರಣೆ_ಕೊಡು && ! defined EXIT_SUCCESS
ಶೂನ್ಯ *ಸ್ಮರಣೆ_ಕೊಡು (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE ಬಿಡುಗಡೆ
#   ಆದರೆ ! defined ಬಿಡುಗಡೆ && ! defined EXIT_SUCCESS
ಶೂನ್ಯ ಬಿಡುಗಡೆ (ಶೂನ್ಯ *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
# define YYCOPY_NEEDED 1
#ಆದರೆ_ಅಂತ್ಯ /* 1 */

#ಆದರೆ (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined GRAM_LTYPE_IS_TRIVIAL && GRAM_LTYPE_IS_TRIVIAL \
             && defined GRAM_STYPE_IS_TRIVIAL && GRAM_STYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
ಒಕ್ಕೂಟ yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_ಗರಿಷ್ಠ (YYSIZEOF (ಒಕ್ಕೂಟ yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_ಬೈಟುಗಳು(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_ಗರಿಷ್ಠ)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(ರಾಶಿ_alloc, ರಾಶಿ)                           \
    ಮಾಡು                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->ರಾಶಿ_alloc, ರಾಶಿ, yysize);                    \
        ರಾಶಿ = &yyptr->ರಾಶಿ_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*ರಾಶಿ) + YYSTACK_GAP_ಗರಿಷ್ಠ; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    ಆಗ (0)

#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  ಆದರೆ defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, ಎಣಿಕೆ_3) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (ಎಣಿಕೆ_3)) * ಗಾತ್ರ (*(Src)))
#  ಇಲ್ಲದಿದ್ದರೆ
#   define YYCOPY(Dst, Src, ಎಣಿಕೆ_3)              \
      ಮಾಡು                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          ಗಾಗಿ (yyi = 0; yyi < (ಎಣಿಕೆ_3); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      ಆಗ (0)
#  endif
# endif
#ಆದರೆ_ಅಂತ್ಯ /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#ವ್ಯಾಖ್ಯಾನಿಸು YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#ವ್ಯಾಖ್ಯಾನಿಸು YYLAST   240

/* YYNTOKENS -- Number of terminals.  */
#ವ್ಯಾಖ್ಯಾನಿಸು YYNTOKENS  61
/* YYNNTS -- Number of nonterminals.  */
#ವ್ಯಾಖ್ಯಾನಿಸು YYNNTS  44
/* YYNRULES -- Number of rules.  */
#ವ್ಯಾಖ್ಯಾನಿಸು YYNRULES  125
/* YYNSTATES -- Number of states.  */
#ವ್ಯಾಖ್ಯಾನಿಸು YYNSTATES  169

/* YYMAXUTOK -- Last valid token kind.  */
#ವ್ಯಾಖ್ಯಾನಿಸು YYMAXUTOK   315


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#ವ್ಯಾಖ್ಯಾನಿಸು YYTRANSLATE(YYX) YY_CAST (yysymbol_ಬಗೆ_t, YYX)

#ಆದರೆ GRAM_DEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ yytype_int16 yyrline[] =
{
       0,   310,   310,   319,   320,   324,   325,   331,   335,   340,
     341,   342,   343,   344,   345,   350,   355,   356,   357,   358,
     359,   360,   360,   361,   362,   363,   364,   365,   366,   367,
     368,   372,   373,   382,   383,   387,   398,   402,   406,   414,
     424,   425,   435,   436,   442,   455,   455,   460,   460,   465,
     465,   470,   480,   481,   482,   483,   488,   489,   493,   494,
     499,   500,   504,   505,   509,   510,   511,   524,   533,   537,
     541,   549,   550,   554,   567,   568,   573,   574,   575,   593,
     597,   601,   609,   611,   616,   623,   633,   637,   641,   649,
     655,   668,   669,   675,   676,   677,   684,   684,   692,   693,
     694,   699,   702,   704,   706,   708,   710,   712,   714,   716,
     718,   723,   724,   733,   757,   758,   759,   760,   772,   774,
     798,   803,   804,   809,   817,   818
};
#ಆದರೆ_ಅಂತ್ಯ

/** Accessing symbol of state STATE.  */
#ವ್ಯಾಖ್ಯಾನಿಸು YY_ACCESSING_ಚಿಹ್ನೆ(State) YY_CAST (yysymbol_ಬಗೆ_t, yystos[State])

#ಆದರೆ 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *yysymbol_name (yysymbol_ಬಗೆ_t yysymbol) YY_ATTRIBUTE_ಬಳಸದ;

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
yysymbol_name (yysymbol_ಬಗೆ_t yysymbol)
{
  ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ yy_sname[] =
  {
  N_("end of file"), N_("error"), N_("invalid token"), N_("string"),
  N_("translatable string"), "%token", "%nterm", "%type", "%destructor",
  "%printer", "%left", "%right", "%nonassoc", "%precedence", "%prec",
  "%dprec", "%merge", "%code", "%default-prec", "%define",
  "%error-verbose", "%expect", "%expect-rr", "%file-prefix", "%<flag>",
  "%glr-parser", "%header", "%initial-action", "%language", "%name-prefix",
  "%no-default-prec", "%no-lines", "%nondeterministic-parser", "%output",
  "%pure-parser", "%require", "%skeleton", "%start", "%token-table",
  "%verbose", "%yacc", "{...}", "%?{...}", N_("[identifier]"),
  N_("character literal"), ":", N_("epilogue"), "=", N_("identifier"),
  N_("identifier:"), "%%", "|", "%{...%}", ";", N_("<tag>"), "<*>", "<>",
  N_("integer literal"), "%param", "%union", "%empty", "$accept", "input",
  "prologue_declarations", "prologue_declaration", "$@1", "params",
  "grammar_declaration", "code_props_type", "union_name",
  "symbol_declaration", "$@2", "$@3", "$@4", "precedence_declarator",
  "string.opt", "tag.opt", "generic_symlist", "generic_symlist_item",
  "tag", "nterm_decls", "token_decls", "token_decl.1", "token_decl",
  "int.opt", "alias", "token_decls_for_prec", "token_decl_for_prec.1",
  "token_decl_for_prec", "symbol_decls", "symbols.1", "grammar",
  "rules_or_grammar_declaration", "rules", "$@5", "rhses.1", "rhs",
  "named_ref.opt", "variable", "value", "id", "id_colon", "symbol",
  "string_as_id", "epilogue.opt", YY_NULLPTR
  };
  /* YYTRANSLATABLE[SYMBOL-NUM] -- Whether YY_SNAME[SYMBOL-NUM] is
     internationalizable.  */
  ಸ್ಥಿರ yytype_int8 yytranslatable[] =
  {
       1,     1,     1,     1,     1,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     1,     1,     0,     1,     0,     1,     1,
       0,     0,     0,     0,     1,     0,     0,     1,     0,     0,
       0
  };
  ಹಿಂತಿರುಗಿ (yysymbol < YYNTOKENS && yytranslatable[yysymbol]
          ? _(yy_sname[yysymbol])
          : yy_sname[yysymbol]);
}
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು YYPACT_NINF (-148)

#ವ್ಯಾಖ್ಯಾನಿಸು yypact_ಮೌಲ್ಯ_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#ವ್ಯಾಖ್ಯಾನಿಸು YYTABLE_NINF (-125)

#ವ್ಯಾಖ್ಯಾನಿಸು yytable_ಮೌಲ್ಯ_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ yytype_int16 yypact[] =
{
    -148,    13,   113,  -148,   -27,  -148,  -148,  -148,  -148,  -148,
    -148,  -148,  -148,  -148,   -33,  -148,   -21,  -148,   -29,   -26,
      26,  -148,  -148,    27,    -9,    30,    31,  -148,  -148,  -148,
      32,  -148,    34,    36,   112,  -148,  -148,  -148,   168,  -148,
    -148,  -148,    -3,  -148,  -148,    19,  -148,     7,  -148,   -37,
     -37,   164,  -148,    33,  -148,     2,  -148,  -148,  -148,  -148,
    -148,  -148,  -148,  -148,  -148,  -148,  -148,  -148,  -148,  -148,
     112,  -148,  -148,  -148,     9,  -148,    20,    58,  -148,  -148,
      37,    38,  -148,    40,   155,   112,    35,   112,  -148,    41,
    -148,   -28,    43,   -28,  -148,    41,  -148,    43,   112,    46,
     112,  -148,  -148,  -148,  -148,  -148,  -148,  -148,  -148,    44,
    -148,  -148,  -148,  -148,  -148,    50,  -148,  -148,  -148,  -148,
     155,  -148,  -148,  -148,   112,   112,  -148,  -148,  -148,   -28,
     -28,  -148,    21,   112,   112,  -148,    59,  -148,  -148,   112,
     -28,  -148,  -148,  -148,   112,  -148,   -30,   180,  -148,  -148,
     112,    48,    49,    52,    53,  -148,  -148,  -148,    65,    37,
     180,  -148,  -148,  -148,  -148,  -148,    37,  -148,  -148
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ yytype_int8 yydefact[] =
{
       3,     0,     0,     1,     0,    47,    45,    49,    40,    41,
      52,    53,    54,    55,     0,    36,     0,    10,     0,     0,
       0,     7,    14,    56,     0,     0,     0,    37,    18,    19,
       0,    23,     0,     0,     0,    26,    27,    28,     0,     6,
      30,    21,    42,     4,     5,     0,    33,     0,    29,     0,
       0,     0,    38,     0,   113,   114,    11,    12,    13,    57,
       9,    15,    16,    17,    20,    24,    25,   123,   119,   118,
      34,   121,    89,   122,     0,   120,     0,     0,    91,    93,
     111,     0,    43,     0,     0,     0,    51,    79,    82,    74,
      85,     0,    48,    68,    71,    74,    46,    67,     0,    50,
      86,    39,   116,   117,   115,     8,    90,    95,    94,     0,
      92,     2,   112,    96,    32,    22,    44,    64,    65,    66,
      35,    60,    63,    62,    80,     0,    83,    75,    84,    69,
       0,    72,    76,    87,     0,   125,     0,    31,    61,    81,
      70,    78,    73,    77,    88,   101,    97,    98,   101,   100,
       0,     0,     0,     0,     0,   104,    59,   105,     0,   111,
      99,   106,   107,   108,   109,   110,   111,   102,   103
};

/* YYPGOTO[NTERM-NUM].  */
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ yytype_int16 yypgoto[] =
{
    -148,  -148,  -148,  -148,  -148,  -148,   109,  -148,  -148,  -148,
    -148,  -148,  -148,  -148,  -148,  -148,  -148,    -7,  -148,  -148,
      66,   -73,   -87,    60,  -148,  -148,   -71,   -83,  -148,   -50,
    -148,    51,  -148,  -148,  -148,   -19,  -147,  -148,  -148,   -47,
    -148,   -48,   -38,  -148
};

/* YYDEFGOTO[NTERM-NUM].  */
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ yytype_uint8 yydefgoto[] =
{
       0,     1,     2,    43,    81,   115,    76,    45,    83,    46,
      50,    49,    51,    47,    60,   158,   120,   121,   122,    96,
      92,    93,    94,   128,   142,    86,    87,    88,    99,    70,
      77,    78,    79,   136,   146,   147,   113,    55,   105,    71,
      80,    72,    73,   111
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ yytype_int16 yytable[] =
{
      89,   100,    95,    95,   126,   102,   131,    68,    52,    90,
      67,    69,   167,     3,   124,    53,    68,    91,   129,   168,
      69,   148,   106,   149,    67,   141,    48,    54,    56,    58,
      59,    57,    61,    62,    63,    64,   123,    65,    89,    66,
      89,   126,   131,   103,    95,    82,    95,    90,   133,    90,
     104,    68,   106,   131,   139,    69,   126,   140,  -124,    74,
      84,    85,   107,     5,     6,     7,     8,     9,    10,    11,
      12,    13,   123,   108,   101,    14,    15,    89,    89,   114,
     112,   116,    95,    95,   144,   106,    90,    90,    27,   125,
     135,   137,    89,    95,   143,    34,   106,   130,   127,   159,
     134,    90,   161,   163,   145,   162,   166,    75,   109,   164,
     165,    44,   159,   138,     4,    67,    97,    42,     5,     6,
       7,     8,     9,    10,    11,    12,    13,     0,   110,   160,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,     0,   132,    68,     0,    67,     0,
      69,     0,     0,    38,     0,    39,    40,    67,     0,    74,
       0,    41,    42,     5,     6,     7,     8,     9,    10,    11,
      12,    13,     0,    67,     0,    14,    15,     0,     0,     0,
       0,     0,     0,     0,   150,   151,   152,     0,    27,    68,
       0,   153,   154,    69,     0,    34,     0,     0,    68,   117,
     118,   119,    69,     0,     0,     0,     0,    75,    98,     0,
       0,   -58,   155,     0,    68,     0,     0,    42,    69,     0,
       0,     0,     0,     0,   156,     0,     0,     0,     0,     0,
     157
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ yytype_int16 yycheck[] =
{
      47,    51,    49,    50,    87,     3,    93,    44,    41,    47,
       3,    48,   159,     0,    85,    48,    44,    54,    91,   166,
      48,    51,    70,    53,     3,     4,    53,    48,    57,     3,
       3,    57,    41,     3,     3,     3,    84,     3,    85,     3,
      87,   124,   129,    41,    91,    48,    93,    85,    98,    87,
      48,    44,   100,   140,   125,    48,   139,   130,     0,     1,
      41,    54,    53,     5,     6,     7,     8,     9,    10,    11,
      12,    13,   120,    53,    41,    17,    18,   124,   125,    41,
      43,    41,   129,   130,   134,   133,   124,   125,    30,    54,
      46,    41,   139,   140,   132,    37,   144,    54,    57,   147,
      54,   139,   150,    54,    45,    57,    41,    49,    50,    57,
      57,     2,   160,   120,     1,     3,    50,    59,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    -1,    77,   148,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    -1,    95,    44,    -1,     3,    -1,
      48,    -1,    -1,    50,    -1,    52,    53,     3,    -1,     1,
      -1,    58,    59,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    -1,     3,    -1,    17,    18,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    14,    15,    16,    -1,    30,    44,
      -1,    21,    22,    48,    -1,    37,    -1,    -1,    44,    54,
      55,    56,    48,    -1,    -1,    -1,    -1,    49,    54,    -1,
      -1,    41,    42,    -1,    44,    -1,    -1,    59,    48,    -1,
      -1,    -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,
      60
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ yytype_int8 yystos[] =
{
       0,    62,    63,     0,     1,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    50,    52,
      53,    58,    59,    64,    67,    68,    70,    74,    53,    72,
      71,    73,    41,    48,    48,    98,    57,    57,     3,     3,
      75,    41,     3,     3,     3,     3,     3,     3,    44,    48,
      90,   100,   102,   103,     1,    49,    67,    91,    92,    93,
     101,    65,    48,    69,    41,    54,    86,    87,    88,   100,
     103,    54,    81,    82,    83,   100,    80,    81,    54,    89,
      90,    41,     3,    41,    48,    99,   102,    53,    53,    50,
      92,   104,    43,    97,    41,    66,    41,    54,    55,    56,
      77,    78,    79,   102,    87,    54,    88,    57,    84,    82,
      54,    83,    84,    90,    54,    46,    94,    41,    78,    87,
      82,     4,    85,   103,    90,    45,    95,    96,    51,    53,
      14,    15,    16,    21,    22,    42,    54,    60,    76,   102,
      96,   102,    57,    54,    57,    57,    41,    97,    97
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ yytype_int8 yyr1[] =
{
       0,    61,    62,    63,    63,    64,    64,    64,    64,    64,
      64,    64,    64,    64,    64,    64,    64,    64,    64,    64,
      64,    65,    64,    64,    64,    64,    64,    64,    64,    64,
      64,    66,    66,    67,    67,    67,    67,    67,    67,    67,
      68,    68,    69,    69,    67,    71,    70,    72,    70,    73,
      70,    70,    74,    74,    74,    74,    75,    75,    76,    76,
      77,    77,    78,    78,    79,    79,    79,    80,    81,    81,
      81,    82,    82,    83,    84,    84,    85,    85,    85,    86,
      86,    86,    87,    87,    88,    88,    89,    89,    89,    90,
      90,    91,    91,    92,    92,    92,    94,    93,    95,    95,
      95,    96,    96,    96,    96,    96,    96,    96,    96,    96,
      96,    97,    97,    98,    99,    99,    99,    99,   100,   100,
     101,   102,   102,   103,   104,   104
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ yytype_int8 yyr2[] =
{
       0,     2,     4,     0,     2,     1,     1,     1,     3,     2,
       1,     2,     2,     2,     1,     2,     2,     2,     1,     1,
       2,     0,     3,     1,     2,     2,     1,     1,     1,     2,
       1,     2,     1,     1,     2,     3,     1,     1,     2,     3,
       1,     1,     0,     1,     3,     0,     3,     0,     3,     0,
       3,     2,     1,     1,     1,     1,     0,     1,     0,     1,
       1,     2,     1,     1,     1,     1,     1,     1,     1,     2,
       3,     1,     2,     3,     0,     1,     0,     1,     1,     1,
       2,     3,     1,     2,     2,     1,     1,     2,     3,     1,
       2,     1,     2,     1,     2,     2,     0,     5,     1,     3,
       2,     0,     3,     4,     2,     2,     3,     3,     3,     3,
       3,     0,     1,     1,     0,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     0,     2
};


ಎಣಿಕೆ { YYENOMEM = -2 };

#ವ್ಯಾಖ್ಯಾನಿಸು yyerrok         (yyerrstatus = 0)
#ವ್ಯಾಖ್ಯಾನಿಸು yyclearin       (yychar = GRAM_ಖಾಲಿಗಳು)

#ವ್ಯಾಖ್ಯಾನಿಸು YYACCEPT        ಹೋಗು yyacceptlab
#ವ್ಯಾಖ್ಯಾನಿಸು YYABORT         ಹೋಗು yyabortlab
#ವ್ಯಾಖ್ಯಾನಿಸು YYERROR         ಹೋಗು yyerrorlab
#ವ್ಯಾಖ್ಯಾನಿಸು YYNOMEM         ಹೋಗು yyexhaustedlab


#ವ್ಯಾಖ್ಯಾನಿಸು YYRECOVERING()  (!!yyerrstatus)

#ವ್ಯಾಖ್ಯಾನಿಸು YYBACKUP(Token, ಮೌಲ್ಯ_4)                                    \
  ಮಾಡು                                                              \
    ಆದರೆ (yychar == GRAM_ಖಾಲಿಗಳು)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (ಮೌಲ್ಯ_4);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        YY_LAC_DISCARD ("YYBACKUP");                              \
        ಹೋಗು yybackup;                                            \
      }                                                           \
    ಇಲ್ಲದಿದ್ದರೆ                                                          \
      {                                                           \
        yyerror (&yylloc, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  ಆಗ (0)

/* Backward compatibility with an undocumented macro.
   Use GRAM_error or GRAM_UNDEF. */
#ವ್ಯಾಖ್ಯಾನಿಸು YYERRCODE GRAM_UNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    ಮಾಡು                                                                  \
      ಆದರೆ (N)                                                            \
        {                                                               \
          (Current).ಮೊದಲ_ಸಾಲು   = YYRHSLOC (Rhs, 1).ಮೊದಲ_ಸಾಲು;        \
          (Current).ಮೊದಲ_ಸ್ತಂಭ = YYRHSLOC (Rhs, 1).ಮೊದಲ_ಸ್ತಂಭ;      \
          (Current).ಕೊನೆಯ_ಸಾಲು    = YYRHSLOC (Rhs, N).ಕೊನೆಯ_ಸಾಲು;         \
          (Current).ಕೊನೆಯ_ಸ್ತಂಭ  = YYRHSLOC (Rhs, N).ಕೊನೆಯ_ಸ್ತಂಭ;       \
        }                                                               \
      ಇಲ್ಲದಿದ್ದರೆ                                                              \
        {                                                               \
          (Current).ಮೊದಲ_ಸಾಲು   = (Current).ಕೊನೆಯ_ಸಾಲು   =              \
            YYRHSLOC (Rhs, 0).ಕೊನೆಯ_ಸಾಲು;                                \
          (Current).ಮೊದಲ_ಸ್ತಂಭ = (Current).ಕೊನೆಯ_ಸ್ತಂಭ =              \
            YYRHSLOC (Rhs, 0).ಕೊನೆಯ_ಸ್ತಂಭ;                              \
        }                                                               \
    ಆಗ (0)
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#ಆದರೆ GRAM_DEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(ವಾದಗಳು_2)                        \
ಮಾಡು {                                            \
  ಆದರೆ (yydebug)                                  \
    YYFPRINTF ವಾದಗಳು_2;                             \
} ಆಗ (0)


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  ಆದರೆ defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined GRAM_LTYPE_IS_TRIVIAL && GRAM_LTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_ಬಳಸದ
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
yy_location_print_ (FILE *yyo, YYLTYPE ಸ್ಥಿರಾಂಕ * ಸ್ಥಿರಾಂಕ yylocp)
{
  ಪೂರ್ಣಾಂಕ res = 0;
  ಪೂರ್ಣಾಂಕ ಕೊನೆ_ಲಂಬಸಾಲು = 0 != yylocp->ಕೊನೆಯ_ಸ್ತಂಭ ? yylocp->ಕೊನೆಯ_ಸ್ತಂಭ - 1 : 0;
  ಆದರೆ (0 <= yylocp->ಮೊದಲ_ಸಾಲು)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->ಮೊದಲ_ಸಾಲು);
      ಆದರೆ (0 <= yylocp->ಮೊದಲ_ಸ್ತಂಭ)
        res += YYFPRINTF (yyo, ".%d", yylocp->ಮೊದಲ_ಸ್ತಂಭ);
    }
  ಆದರೆ (0 <= yylocp->ಕೊನೆಯ_ಸಾಲು)
    {
      ಆದರೆ (yylocp->ಮೊದಲ_ಸಾಲು < yylocp->ಕೊನೆಯ_ಸಾಲು)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->ಕೊನೆಯ_ಸಾಲು);
          ಆದರೆ (0 <= ಕೊನೆ_ಲಂಬಸಾಲು)
            res += YYFPRINTF (yyo, ".%d", ಕೊನೆ_ಲಂಬಸಾಲು);
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (0 <= ಕೊನೆ_ಲಂಬಸಾಲು && yylocp->ಮೊದಲ_ಸ್ತಂಭ < ಕೊನೆ_ಲಂಬಸಾಲು)
        res += YYFPRINTF (yyo, "-%d", ಕೊನೆ_ಲಂಬಸಾಲು);
    }
  ಹಿಂತಿರುಗಿ res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  ಇಲ್ಲದಿದ್ದರೆ

#   define YYLOCATION_PRINT(File, Loc) ((ಶೂನ್ಯ) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_ಚಿಹ್ನೆ_PRINT(Title, ಬಗೆ_2, ಮೌಲ್ಯ_4, Location)                    \
ಮಾಡು {                                                                      \
  ಆದರೆ (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_ಚಿಹ್ನೆ_print (stderr,                                            \
                  ಬಗೆ_2, ಮೌಲ್ಯ_4, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} ಆಗ (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
yy_ಚಿಹ್ನೆ_ಮೌಲ್ಯ_print (FILE *yyo,
                       yysymbol_ಬಗೆ_t yykind, YYSTYPE ಸ್ಥಿರಾಂಕ * ಸ್ಥಿರಾಂಕ yyvaluep, YYLTYPE ಸ್ಥಿರಾಂಕ * ಸ್ಥಿರಾಂಕ yylocationp)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  ಆದರೆ (!yyvaluep)
    ಹಿಂತಿರುಗಿ;
/* "%code pre-printer" blocks.  */
tron (yyo);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  ಆಯ್ಕೆ (yykind)
    {
    ಸ್ಥಿತಿ YYSYMBOL_STRING: /* "string"  */
         { fputs (((*yyvaluep).STRING), yyo); }
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_TSTRING: /* "translatable string"  */
         { fputs (((*yyvaluep).TSTRING), yyo); }
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_PERCENT_ERROR_VERBOSE: /* "%error-verbose"  */
         { fputs (((*yyvaluep).PERCENT_ERROR_VERBOSE), yyo); }
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_PERCENT_FILE_ಪೂರ್ವಪ್ರತ್ಯಯ: /* "%file-prefix"  */
         { fputs (((*yyvaluep).PERCENT_FILE_ಪೂರ್ವಪ್ರತ್ಯಯ), yyo); }
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_PERCENT_FLAG: /* "%<flag>"  */
         { fprintf (yyo, "%%%s", ((*yyvaluep).PERCENT_FLAG)); }
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_PERCENT_NAME_ಪೂರ್ವಪ್ರತ್ಯಯ: /* "%name-prefix"  */
         { fputs (((*yyvaluep).PERCENT_NAME_ಪೂರ್ವಪ್ರತ್ಯಯ), yyo); }
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_PERCENT_ಶುದ್ಧ_PARSER: /* "%pure-parser"  */
         { fputs (((*yyvaluep).PERCENT_ಶುದ್ಧ_PARSER), yyo); }
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_BRACED_ಸಂಕೇತ: /* "{...}"  */
         { fputs (((*yyvaluep).BRACED_ಸಂಕೇತ), yyo); }
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_BRACED_PREDICATE: /* "%?{...}"  */
         { fputs (((*yyvaluep).BRACED_PREDICATE), yyo); }
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_BRACKETED_ID: /* "[identifier]"  */
         { fprintf (yyo, "[%s]", ((*yyvaluep).BRACKETED_ID)); }
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_CHAR_LITERAL: /* "character literal"  */
         { fputs (char_name (((*yyvaluep).CHAR_LITERAL)), yyo); }
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_EPILOGUE: /* "epilogue"  */
         { fputs (((*yyvaluep).EPILOGUE), yyo); }
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_ID: /* "identifier"  */
         { fputs (((*yyvaluep).ID), yyo); }
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_ID_COLON: /* "identifier:"  */
         { fprintf (yyo, "%s:", ((*yyvaluep).ID_COLON)); }
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_PROLOGUE: /* "%{...%}"  */
         { fputs (((*yyvaluep).PROLOGUE), yyo); }
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_ಟ್ಯಾಗ್: /* "<tag>"  */
         { fprintf (yyo, "<%s>", ((*yyvaluep).ಟ್ಯಾಗ್_2)); }
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_INT_LITERAL: /* "integer literal"  */
         { fprintf (yyo, "%d", ((*yyvaluep).INT_LITERAL)); }
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_PERCENT_PARAM: /* "%param"  */
{
  ಆಯ್ಕೆ (((*yyvaluep).PERCENT_PARAM))
    {
#ವ್ಯಾಖ್ಯಾನಿಸು CASE(ಒಳ_2, ಹೊರಹೋಗು_2)                                           \
      ಸ್ಥಿತಿ param_ ## ಒಳ_2: fputs ("%" #Out, yyo); ನಿಲ್ಲಿಸು
      CASE (lex,   lex-param);
      CASE (parse, parse-param);
      CASE (both,  param);
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು CASE
      ಸ್ಥಿತಿ param_none: aver (false); ನಿಲ್ಲಿಸು;
    }
}
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_ಸಂಕೇತ_props_type: /* code_props_type  */
         { fprintf (yyo, "%s", ಸಂಕೇತ_props_type_string (((*yyvaluep).ಸಂಕೇತ_props_type))); }
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_75_string_opt: /* string.opt  */
         { fputs (((*yyvaluep).yykind_75), yyo); }
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_76_ಟ್ಯಾಗ್_opt: /* tag.opt  */
         { fputs (((*yyvaluep).yykind_76), yyo); }
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_generic_symlist: /* generic_symlist  */
         { ಚಿಹ್ನೆ_list_syms_print (((*yyvaluep).generic_symlist), yyo); }
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_generic_symlist_ಅಂಶ: /* generic_symlist_item  */
         { ಚಿಹ್ನೆ_list_syms_print (((*yyvaluep).generic_symlist_ಅಂಶ), yyo); }
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_ಟ್ಯಾಗ್_2: /*ಟ್ಯಾಗ್*/
         { fprintf (yyo, "<%s>", ((*yyvaluep).ಟ್ಯಾಗ್)); }
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_nterm_decls: /* nterm_decls  */
         { ಚಿಹ್ನೆ_list_syms_print (((*yyvaluep).nterm_decls), yyo); }
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_token_decls: /* token_decls  */
         { ಚಿಹ್ನೆ_list_syms_print (((*yyvaluep).token_decls), yyo); }
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_82_token_decl_1: /* token_decl.1  */
         { ಚಿಹ್ನೆ_list_syms_print (((*yyvaluep).yykind_82), yyo); }
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_token_decl: /* token_decl  */
         { fprintf (yyo, "%s", ((*yyvaluep).token_decl) ? ((*yyvaluep).token_decl)->ಟ್ಯಾಗ್ : "<NULL>"); }
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_84_int_opt: /* int.opt  */
         { fprintf (yyo, "%d", ((*yyvaluep).yykind_84)); }
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_alias: /* alias  */
         { fprintf (yyo, "%s", ((*yyvaluep).alias) ? ((*yyvaluep).alias)->ಟ್ಯಾಗ್ : "<NULL>"); }
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_token_decls_for_prec: /* token_decls_for_prec  */
         { ಚಿಹ್ನೆ_list_syms_print (((*yyvaluep).token_decls_for_prec), yyo); }
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_87_token_decl_for_prec_1: /* token_decl_for_prec.1  */
         { ಚಿಹ್ನೆ_list_syms_print (((*yyvaluep).yykind_87), yyo); }
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_token_decl_for_prec: /* token_decl_for_prec  */
         { fprintf (yyo, "%s", ((*yyvaluep).token_decl_for_prec) ? ((*yyvaluep).token_decl_for_prec)->ಟ್ಯಾಗ್ : "<NULL>"); }
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_ಚಿಹ್ನೆ_decls: /* symbol_decls  */
         { ಚಿಹ್ನೆ_list_syms_print (((*yyvaluep).ಚಿಹ್ನೆ_decls), yyo); }
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_90_symbols_1: /* symbols.1  */
         { ಚಿಹ್ನೆ_list_syms_print (((*yyvaluep).yykind_90), yyo); }
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_variable: /* variable  */
         { fputs (((*yyvaluep).variable), yyo); }
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_ಮೌಲ್ಯ: /*ಮೌಲ್ಯ*/
{
  ಆಯ್ಕೆ (((*yyvaluep).ಮೌಲ್ಯ).ಬಗೆ)
    {
    ಸ್ಥಿತಿ muscle_ಸಂಕೇತ:    fprintf (yyo,  "{%s}",  ((*yyvaluep).ಮೌಲ್ಯ).chars); ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ muscle_keyword: fprintf (yyo,   "%s",   ((*yyvaluep).ಮೌಲ್ಯ).chars); ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ muscle_string:  fprintf (yyo, "\"%s\"", ((*yyvaluep).ಮೌಲ್ಯ).chars); ನಿಲ್ಲಿಸು;
    }
}
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_id: /* id  */
         { fprintf (yyo, "%s", ((*yyvaluep).id) ? ((*yyvaluep).id)->ಟ್ಯಾಗ್ : "<NULL>"); }
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_id_colon: /* id_colon  */
         { fprintf (yyo, "%s:", ((*yyvaluep).id_colon)->ಟ್ಯಾಗ್); }
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_ಚಿಹ್ನೆ: /*ಚಿಹ್ನೆ*/
         { fprintf (yyo, "%s", ((*yyvaluep).ಚಿಹ್ನೆ) ? ((*yyvaluep).ಚಿಹ್ನೆ)->ಟ್ಯಾಗ್ : "<NULL>"); }
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_string_as_id: /* string_as_id  */
         { fprintf (yyo, "%s", ((*yyvaluep).string_as_id) ? ((*yyvaluep).string_as_id)->ಟ್ಯಾಗ್ : "<NULL>"); }
        ನಿಲ್ಲಿಸು;

      ಪೂರ್ವನಿಯೋಜಿತ:
        ನಿಲ್ಲಿಸು;
    }
  YY_IGNORE_MAYBE_UNINITIALIZED_ಕೊನೆ
/* "%code post-printer" blocks.  */
troff (yyo);

}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
yy_ಚಿಹ್ನೆ_print (FILE *yyo,
                 yysymbol_ಬಗೆ_t yykind, YYSTYPE ಸ್ಥಿರಾಂಕ * ಸ್ಥಿರಾಂಕ yyvaluep, YYLTYPE ಸ್ಥಿರಾಂಕ * ಸ್ಥಿರಾಂಕ yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_ಚಿಹ್ನೆ_ಮೌಲ್ಯ_print (yyo, yykind, yyvaluep, yylocationp);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
yy_ರಾಶಿ_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  ಗಾಗಿ (; yybottom <= yytop; yybottom++)
    {
      ಪೂರ್ಣಾಂಕ yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_ರಾಶಿ_PRINT(Bottom, ತುದಿ)                            \
ಮಾಡು {                                                            \
  ಆದರೆ (yydebug)                                                  \
    yy_ರಾಶಿ_print ((Bottom), (ತುದಿ));                           \
} ಆಗ (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 ಪೂರ್ಣಾಂಕ yyrule)
{
  ಪೂರ್ಣಾಂಕ yylno = yyrline[yyrule];
  ಪೂರ್ಣಾಂಕ yynrhs = yyr2[yyrule];
  ಪೂರ್ಣಾಂಕ yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  ಗಾಗಿ (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_ಚಿಹ್ನೆ_print (stderr,
                       YY_ACCESSING_ಚಿಹ್ನೆ (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]));
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(ನಿಯಮ_2)          \
ಮಾಡು {                                    \
  ಆದರೆ (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, ನಿಯಮ_2); \
} ಆಗ (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
ಪೂರ್ಣಾಂಕ yydebug;
#ಇಲ್ಲದಿದ್ದರೆ /* !GRAM_DEBUG */
# define YYDPRINTF(ವಾದಗಳು_2) ((ಶೂನ್ಯ) 0)
# define YY_ಚಿಹ್ನೆ_PRINT(Title, ಬಗೆ_2, ಮೌಲ್ಯ_4, Location)
# define YY_ರಾಶಿ_PRINT(Bottom, ತುದಿ)
# define YY_REDUCE_PRINT(ನಿಯಮ_2)
#ಆದರೆ_ಅಂತ್ಯ /* !GRAM_DEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ YYINITDEPTH
# define YYINITDEPTH 200
#ಆದರೆ_ಅಂತ್ಯ

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ YYMAXDEPTH
# define YYMAXDEPTH 10000
#ಆದರೆ_ಅಂತ್ಯ


/* Given a state stack such that *YYBOTTOM is its bottom, such that
   *YYTOP is either its top or is YYTOP_EMPTY to indicate an empty
   stack, and such that *YYCAPACITY is the maximum number of elements it
   can hold without a reallocation, make sure there is enough room to
   store YYADD more elements.  If not, allocate a new stack using
   YYSTACK_ALLOC, copy the existing elements, and adjust *YYBOTTOM,
   *YYTOP, and *YYCAPACITY to reflect the new capacity and memory
   location.  If *YYBOTTOM != YYBOTTOM_NO_FREE, then free the old stack
   using YYSTACK_FREE.  Return 0 if successful or if no reallocation is
   required.  Return YYENOMEM if memory is exhausted.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
yy_lac_ರಾಶಿ_realloc (YYPTRDIFF_T *yycapacity, YYPTRDIFF_T yyadd,
#ಆದರೆ GRAM_DEBUG
                      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *yydebug_ಪೂರ್ವಪ್ರತ್ಯಯ,
                      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *yydebug_suffix,
#ಆದರೆ_ಅಂತ್ಯ
                      yy_state_t **yybottom,
                      yy_state_t *yybottom_no_free,
                      yy_state_t **yytop, yy_state_t *yytop_ಖಾಲಿಗಳು)
{
  YYPTRDIFF_T yysize_old =
    *yytop == yytop_ಖಾಲಿಗಳು ? 0 : *yytop - *yybottom + 1;
  YYPTRDIFF_T yysize_new = yysize_old + yyadd;
  ಆದರೆ (*yycapacity < yysize_new)
    {
      YYPTRDIFF_T yyalloc = 2 * yysize_new;
      yy_state_t *yybottom_new;
      /* Use YYMAXDEPTH for maximum stack size given that the stack
         should never need to grow larger than the main state stack
         needs to grow without LAC.  */
      ಆದರೆ (YYMAXDEPTH < yysize_new)
        {
          YYDPRINTF ((stderr, "%smax size exceeded%s", yydebug_ಪೂರ್ವಪ್ರತ್ಯಯ,
                      yydebug_suffix));
          ಹಿಂತಿರುಗಿ YYENOMEM;
        }
      ಆದರೆ (YYMAXDEPTH < yyalloc)
        yyalloc = YYMAXDEPTH;
      yybottom_new =
        YY_CAST (yy_state_t *,
                 YYSTACK_ALLOC (YY_CAST (YYSIZE_T,
                                         yyalloc * YYSIZEOF (*yybottom_new))));
      ಆದರೆ (!yybottom_new)
        {
          YYDPRINTF ((stderr, "%srealloc failed%s", yydebug_ಪೂರ್ವಪ್ರತ್ಯಯ,
                      yydebug_suffix));
          ಹಿಂತಿರುಗಿ YYENOMEM;
        }
      ಆದರೆ (*yytop != yytop_ಖಾಲಿಗಳು)
        {
          YYCOPY (yybottom_new, *yybottom, yysize_old);
          *yytop = yybottom_new + (yysize_old - 1);
        }
      ಆದರೆ (*yybottom != yybottom_no_free)
        YYSTACK_FREE (*yybottom);
      *yybottom = yybottom_new;
      *yycapacity = yyalloc;
    }
  ಹಿಂತಿರುಗಿ 0;
}

/* Establish the initial context for the current lookahead if no initial
   context is currently established.

   We define a context as a snapshot of the parser stacks.  We define
   the initial context for a lookahead as the context in which the
   parser initially examines that lookahead in order to select a
   syntactic action.  Thus, if the lookahead eventually proves
   syntactically unacceptable (possibly in a later context reached via a
   series of reductions), the initial context can be used to determine
   the exact set of tokens that would be syntactically acceptable in the
   lookahead's place.  Moreover, it is the context after which any
   further semantic actions would be erroneous because they would be
   determined by a syntactically unacceptable token.

   YY_LAC_ESTABLISH should be invoked when a reduction is about to be
   performed in an inconsistent state (which, for the purposes of LAC,
   includes consistent states that don't know they're consistent because
   their default reductions have been disabled).  Iff there is a
   lookahead token, it should also be invoked before reporting a syntax
   error.  This latter case is for the sake of the debugging output.

   For parse.lac=full, the implementation of YY_LAC_ESTABLISH is as
   follows.  If no initial context is currently established for the
   current lookahead, then check if that lookahead can eventually be
   shifted if syntactic actions continue from the current context.
   Report a syntax error if it cannot.  */
#ವ್ಯಾಖ್ಯಾನಿಸು YY_LAC_ESTABLISH                                                \
ಮಾಡು {                                                                    \
  ಆದರೆ (!yy_lac_established)                                              \
    {                                                                   \
      YYDPRINTF ((stderr,                                               \
                  "LAC: initial context established for %s\n",          \
                  yysymbol_name (yytoken)));                            \
      yy_lac_established = 1;                                           \
      ಆಯ್ಕೆ (yy_lac (yyesa, &yyes, &yyes_ಸಾಮರ್ಥ್ಯ, yyssp, yytoken))    \
        {                                                               \
        ಸ್ಥಿತಿ YYENOMEM:                                                  \
          YYNOMEM;                                                      \
        ಸ್ಥಿತಿ 1:                                                         \
          ಹೋಗು yyerrlab;                                                \
        }                                                               \
    }                                                                   \
} ಆಗ (0)

/* Discard any previous initial lookahead context because of Event,
   which may be a lookahead change or an invalidation of the currently
   established initial context for the current lookahead.

   The most common example of a lookahead change is a shift.  An example
   of both cases is syntax error recovery.  That is, a syntax error
   occurs when the lookahead is syntactically erroneous for the
   currently established initial context, so error recovery manipulates
   the parser stacks to try to find a new initial context in which the
   current lookahead is syntactically acceptable.  If it fails to find
   such a context, it discards the lookahead.  */
#ಆದರೆ GRAM_DEBUG
# define YY_LAC_DISCARD(Event)                                           \
ಮಾಡು {                                                                     \
  ಆದರೆ (yy_lac_established)                                                \
    {                                                                    \
      YYDPRINTF ((stderr, "LAC: initial context discarded due to "       \
                  Event "\n"));                                          \
      yy_lac_established = 0;                                            \
    }                                                                    \
} ಆಗ (0)
#ಇಲ್ಲದಿದ್ದರೆ
# define YY_LAC_DISCARD(Event) yy_lac_established = 0
#ಆದರೆ_ಅಂತ್ಯ

/* Given the stack whose top is *YYSSP, return 0 iff YYTOKEN can
   eventually (after perhaps some reductions) be shifted, return 1 if
   not, or return YYENOMEM if memory is exhausted.  As preconditions and
   postconditions: *YYES_CAPACITY is the allocated size of the array to
   which *YYES points, and either *YYES = YYESA or *YYES points to an
   array allocated with YYSTACK_ALLOC.  yy_lac may overwrite the
   contents of either array, alter *YYES and *YYES_CAPACITY, and free
   any old *YYES other than YYESA.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
yy_lac (yy_state_t *yyesa, yy_state_t **yyes,
        YYPTRDIFF_T *yyes_ಸಾಮರ್ಥ್ಯ, yy_state_t *yyssp, yysymbol_ಬಗೆ_t yytoken)
{
  yy_state_t *yyes_prev = yyssp;
  yy_state_t *yyesp = yyes_prev;
  /* Reduce until we encounter a shift and thereby accept the token.  */
  YYDPRINTF ((stderr, "LAC: checking lookahead %s:", yysymbol_name (yytoken)));
  ಆದರೆ (yytoken == YYSYMBOL_YYUNDEF)
    {
      YYDPRINTF ((stderr, " Always Err\n"));
      ಹಿಂತಿರುಗಿ 1;
    }
  ಆಗ (1)
    {
      ಪೂರ್ಣಾಂಕ yyrule = yypact[+*yyesp];
      ಆದರೆ (yypact_ಮೌಲ್ಯ_is_default (yyrule)
          || (yyrule += yytoken) < 0 || YYLAST < yyrule
          || yycheck[yyrule] != yytoken)
        {
          /* Use the default action.  */
          yyrule = yydefact[+*yyesp];
          ಆದರೆ (yyrule == 0)
            {
              YYDPRINTF ((stderr, " Err\n"));
              ಹಿಂತಿರುಗಿ 1;
            }
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          /* Use the action from yytable.  */
          yyrule = yytable[yyrule];
          ಆದರೆ (yytable_ಮೌಲ್ಯ_is_error (yyrule))
            {
              YYDPRINTF ((stderr, " Err\n"));
              ಹಿಂತಿರುಗಿ 1;
            }
          ಆದರೆ (0 < yyrule)
            {
              YYDPRINTF ((stderr, " S%d\n", yyrule));
              ಹಿಂತಿರುಗಿ 0;
            }
          yyrule = -yyrule;
        }
      /* By now we know we have to simulate a reduce.  */
      YYDPRINTF ((stderr, " R%d", yyrule - 1));
      {
        /* Pop the corresponding number of values from the stack.  */
        YYPTRDIFF_T yylen = yyr2[yyrule];
        /* First pop from the LAC stack as many tokens as possible.  */
        ಆದರೆ (yyesp != yyes_prev)
          {
            YYPTRDIFF_T yysize = yyesp - *yyes + 1;
            ಆದರೆ (yylen < yysize)
              {
                yyesp -= yylen;
                yylen = 0;
              }
            ಇಲ್ಲದಿದ್ದರೆ
              {
                yyesp = yyes_prev;
                yylen -= yysize;
              }
          }
        /* Only afterwards look at the main stack.  */
        ಆದರೆ (yylen)
          yyesp = yyes_prev -= yylen;
      }
      /* Push the resulting state of the reduction.  */
      {
        yy_state_fast_t yystate;
        {
          ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ yylhs = yyr1[yyrule] - YYNTOKENS;
          ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ yyi = yypgoto[yylhs] + *yyesp;
          yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyesp
                     ? yytable[yyi]
                     : yydefgoto[yylhs]);
        }
        ಆದರೆ (yyesp == yyes_prev)
          {
            yyesp = *yyes;
            YY_IGNORE_USELESS_CAST_BEGIN
            *yyesp = YY_CAST (yy_state_t, yystate);
            YY_IGNORE_USELESS_CAST_ಕೊನೆ
          }
        ಇಲ್ಲದಿದ್ದರೆ
          {
            ಆದರೆ (yy_lac_ರಾಶಿ_realloc (yyes_ಸಾಮರ್ಥ್ಯ, 1,
#ಆದರೆ GRAM_DEBUG
                                      " (", ")",
#ಆದರೆ_ಅಂತ್ಯ
                                      yyes, yyesa, &yyesp, yyes_prev))
              {
                YYDPRINTF ((stderr, "\n"));
                ಹಿಂತಿರುಗಿ YYENOMEM;
              }
            YY_IGNORE_USELESS_CAST_BEGIN
            *++yyesp = YY_CAST (yy_state_t, yystate);
            YY_IGNORE_USELESS_CAST_ಕೊನೆ
          }
        YYDPRINTF ((stderr, " G%d", yystate));
      }
    }
}

/* Context of a parse error.  */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ
{
  yy_state_t *yyssp;
  yy_state_t *yyesa;
  yy_state_t **yyes;
  YYPTRDIFF_T *yyes_ಸಾಮರ್ಥ್ಯ;
  yysymbol_ಬಗೆ_t yytoken;
  YYLTYPE *yylloc;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
yypcontext_expected_ಟೋಕನುಗಳು (ಸ್ಥಿರಾಂಕ yypcontext_t *yyctx,
                            yysymbol_ಬಗೆ_t yyarg[], ಪೂರ್ಣಾಂಕ yyargn)
{
  /* Actual size of YYARG. */
  ಪೂರ್ಣಾಂಕ yycount = 0;

  ಪೂರ್ಣಾಂಕ yyx;
  ಗಾಗಿ (yyx = 0; yyx < YYNTOKENS; ++yyx)
    {
      yysymbol_ಬಗೆ_t yysym = YY_CAST (yysymbol_ಬಗೆ_t, yyx);
      ಆದರೆ (yysym != YYSYMBOL_YYerror && yysym != YYSYMBOL_YYUNDEF)
        ಆಯ್ಕೆ (yy_lac (yyctx->yyesa, yyctx->yyes, yyctx->yyes_ಸಾಮರ್ಥ್ಯ, yyctx->yyssp, yysym))
          {
          ಸ್ಥಿತಿ YYENOMEM:
            ಹಿಂತಿರುಗಿ YYENOMEM;
          ಸ್ಥಿತಿ 1:
            ಮುಂದುವರಿ;
          ಪೂರ್ವನಿಯೋಜಿತ:
            ಆದರೆ (!yyarg)
              ++yycount;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (yycount == yyargn)
              ಹಿಂತಿರುಗಿ 0;
            ಇಲ್ಲದಿದ್ದರೆ
              yyarg[yycount++] = yysym;
          }
    }
  ಆದರೆ (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  ಹಿಂತಿರುಗಿ yycount;
}




/* The kind of the lookahead of this context.  */
ಸ್ಥಿರ yysymbol_ಬಗೆ_t
yypcontext_token (ಸ್ಥಿರಾಂಕ yypcontext_t *yyctx) YY_ATTRIBUTE_ಬಳಸದ;

ಸ್ಥಿರ yysymbol_ಬಗೆ_t
yypcontext_token (ಸ್ಥಿರಾಂಕ yypcontext_t *yyctx)
{
  ಹಿಂತಿರುಗಿ yyctx->yytoken;
}

/* The location of the lookahead of this context.  */
ಸ್ಥಿರ YYLTYPE *
yypcontext_location (ಸ್ಥಿರಾಂಕ yypcontext_t *yyctx) YY_ATTRIBUTE_ಬಳಸದ;

ಸ್ಥಿರ YYLTYPE *
yypcontext_location (ಸ್ಥಿರಾಂಕ yypcontext_t *yyctx)
{
  ಹಿಂತಿರುಗಿ yyctx->yylloc;
}

/* User defined function to report a syntax error.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
yyreport_syntax_error (ಸ್ಥಿರಾಂಕ yypcontext_t *yyctx);

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
yydestruct (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *yymsg,
            yysymbol_ಬಗೆ_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  ಆದರೆ (!yymsg)
    yymsg = "Deleting";
  YY_ಚಿಹ್ನೆ_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  ಆಯ್ಕೆ (yykind)
    {
    ಸ್ಥಿತಿ YYSYMBOL_generic_symlist: /* generic_symlist  */
            { ಚಿಹ್ನೆ_list_free (((*yyvaluep).generic_symlist)); }
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_generic_symlist_ಅಂಶ: /* generic_symlist_item  */
            { ಚಿಹ್ನೆ_list_free (((*yyvaluep).generic_symlist_ಅಂಶ)); }
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_nterm_decls: /* nterm_decls  */
            { ಚಿಹ್ನೆ_list_free (((*yyvaluep).nterm_decls)); }
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_token_decls: /* token_decls  */
            { ಚಿಹ್ನೆ_list_free (((*yyvaluep).token_decls)); }
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_82_token_decl_1: /* token_decl.1  */
            { ಚಿಹ್ನೆ_list_free (((*yyvaluep).yykind_82)); }
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_token_decls_for_prec: /* token_decls_for_prec  */
            { ಚಿಹ್ನೆ_list_free (((*yyvaluep).token_decls_for_prec)); }
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_87_token_decl_for_prec_1: /* token_decl_for_prec.1  */
            { ಚಿಹ್ನೆ_list_free (((*yyvaluep).yykind_87)); }
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_ಚಿಹ್ನೆ_decls: /* symbol_decls  */
            { ಚಿಹ್ನೆ_list_free (((*yyvaluep).ಚಿಹ್ನೆ_decls)); }
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_90_symbols_1: /* symbols.1  */
            { ಚಿಹ್ನೆ_list_free (((*yyvaluep).yykind_90)); }
        ನಿಲ್ಲಿಸು;

      ಪೂರ್ವನಿಯೋಜಿತ:
        ನಿಲ್ಲಿಸು;
    }
  YY_IGNORE_MAYBE_UNINITIALIZED_ಕೊನೆ
}






/*----------.
| yyparse.  |
`----------*/

ಪೂರ್ಣಾಂಕ
yyparse (ಶೂನ್ಯ)
{
/* Lookahead token kind.  */
ಪೂರ್ಣಾಂಕ yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_ಮೌಲ್ಯ (ಸ್ಥಿರ YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_ಮೌಲ್ಯ (= yyval_default);

/* Location data for the lookahead symbol.  */
ಸ್ಥಿರ YYLTYPE yyloc_default
# ಆದರೆ defined GRAM_LTYPE_IS_TRIVIAL && GRAM_LTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

    /* Number of syntax errors so far.  */
    ಪೂರ್ಣಾಂಕ yynerrs YY_ATTRIBUTE_ಬಳಸದ = 0;

    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    ಪೂರ್ಣಾಂಕ yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

    yy_state_t yyesa[20];
    yy_state_t *yyes = yyesa;
    YYPTRDIFF_T yyes_ಸಾಮರ್ಥ್ಯ = 20 < YYMAXDEPTH ? 20 : YYMAXDEPTH;

  /* Whether LAC context is established.  A Boolean.  */
  ಪೂರ್ಣಾಂಕ yy_lac_established = 0;
  ಪೂರ್ಣಾಂಕ yyn;
  /* The return value of yyparse.  */
  ಪೂರ್ಣಾಂಕ yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_ಬಗೆ_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];



#ವ್ಯಾಖ್ಯಾನಿಸು YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  ಪೂರ್ಣಾಂಕ yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = GRAM_ಖಾಲಿಗಳು; /* Cause a token to be read.  */


/* User initialization code.  */
{
  /* Bison's grammar can initial empty locations, hence a default
     location is needed. */
  boundary_ಗಣ (&yylloc.ಆರಂಭ, grammar_file, 1, 1, 1);
  boundary_ಗಣ (&yylloc.ಕೊನೆ, grammar_file, 1, 1, 1);
}


  yylsp[0] = yylloc;
  ಹೋಗು yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_ಕೊನೆ
  YY_ರಾಶಿ_PRINT (yyss, yyssp);

  ಆದರೆ (yyss + yystacksize - 1 <= yyssp)
#ಆದರೆ !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#ಇಲ್ಲದಿದ್ದರೆ
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# ಆದರೆ defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# ಇಲ್ಲದಿದ್ದರೆ /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      ಆದರೆ (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      ಆದರೆ (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        ಒಕ್ಕೂಟ yyalloc *yyptr =
          YY_CAST (ಒಕ್ಕೂಟ yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_ಬೈಟುಗಳು (yystacksize))));
        ಆದರೆ (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        ಆದರೆ (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (ದೀರ್ಘ, yystacksize)));
      YY_IGNORE_USELESS_CAST_ಕೊನೆ

      ಆದರೆ (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#ಆದರೆ_ಅಂತ್ಯ /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  ಆದರೆ (yystate == YYFINAL)
    YYACCEPT;

  ಹೋಗು yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  ಆದರೆ (yypact_ಮೌಲ್ಯ_is_default (yyn))
    ಹೋಗು yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  ಆದರೆ (yychar == GRAM_ಖಾಲಿಗಳು)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex (&yylval, &yylloc);
    }

  ಆದರೆ (yychar <= GRAM_EOF)
    {
      yychar = GRAM_EOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (yychar == GRAM_error)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = GRAM_UNDEF;
      yytoken = YYSYMBOL_YYerror;
      yyerror_range[1] = yylloc;
      ಹೋಗು yyerrlab1;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      yytoken = YYTRANSLATE (yychar);
      YY_ಚಿಹ್ನೆ_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  ಆದರೆ (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    {
      YY_LAC_ESTABLISH;
      ಹೋಗು yydefault;
    }
  yyn = yytable[yyn];
  ಆದರೆ (yyn <= 0)
    {
      ಆದರೆ (yytable_ಮೌಲ್ಯ_is_error (yyn))
        ಹೋಗು yyerrlab;
      yyn = -yyn;
      YY_LAC_ESTABLISH;
      ಹೋಗು yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  ಆದರೆ (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_ಚಿಹ್ನೆ_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_ಕೊನೆ
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = GRAM_ಖಾಲಿಗಳು;
  YY_LAC_DISCARD ("shift");
  ಹೋಗು yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  ಆದರೆ (yyn == 0)
    ಹೋಗು yyerrlab;
  ಹೋಗು yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  {
    ಪೂರ್ಣಾಂಕ yychar_backup = yychar;
    ಆಯ್ಕೆ (yyn)
      {
  ಸ್ಥಿತಿ 6: /* prologue_declaration: "%{...%}"  */
    {
      muscle_ಸಂಕೇತ_grow (union_ನೋಡಿದ ? "post_prologue" : "pre_prologue",
                        translate_ಸಂಕೇತ ((yyvsp[0].PROLOGUE), (yylsp[0]), true), (yylsp[0]));
      ಸಂಕೇತ_scanner_ಕೊನೆಯ_string_free ();
    }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 7: /* prologue_declaration: "%<flag>"  */
    {
      muscle_percent_define_ensure ((yyvsp[0].PERCENT_FLAG), (yylsp[0]), true);
    }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 8: /* prologue_declaration: "%define" variable value  */
    {
      muscle_percent_define_ಸೇರಿಸು ((yyvsp[-1].variable), (yyloc), (yyvsp[0].ಮೌಲ್ಯ).ಬಗೆ, (yyvsp[0].ಮೌಲ್ಯ).chars,
                                    MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
    }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 9: /* prologue_declaration: "%header" string.opt  */
                                   { handle_ಶೀರ್ಷಿಕೆ ((yyvsp[0].yykind_75)); }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 10: /* prologue_declaration: "%error-verbose"  */
                                   { handle_error_verbose (&(yyloc), (yyvsp[0].PERCENT_ERROR_VERBOSE)); }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 11: /* prologue_declaration: "%expect" "integer literal"  */
                                   { expected_sr_conflicts = (yyvsp[0].INT_LITERAL); }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 12: /* prologue_declaration: "%expect-rr" "integer literal"  */
                                   { expected_rr_conflicts = (yyvsp[0].INT_LITERAL); }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 13: /* prologue_declaration: "%file-prefix" "string"  */
                                   { handle_file_ಪೂರ್ವಪ್ರತ್ಯಯ (&(yyloc), &(yylsp[-1]), (yyvsp[-1].PERCENT_FILE_ಪೂರ್ವಪ್ರತ್ಯಯ), (yyvsp[0].STRING)); }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 14: /* prologue_declaration: "%glr-parser"  */
    {
      nondeterministic_parser = true;
      glr_parser = true;
    }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 15: /* prologue_declaration: "%initial-action" "{...}"  */
    {
      muscle_ಸಂಕೇತ_grow ("initial_action", translate_ಸಂಕೇತ ((yyvsp[0].BRACED_ಸಂಕೇತ), (yylsp[0]), false), (yylsp[0]));
      ಸಂಕೇತ_scanner_ಕೊನೆಯ_string_free ();
    }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 16: /* prologue_declaration: "%language" "string"  */
                                { handle_language (&(yylsp[-1]), (yyvsp[0].STRING)); }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 17: /* prologue_declaration: "%name-prefix" "string"  */
                                { handle_name_ಪೂರ್ವಪ್ರತ್ಯಯ (&(yyloc), (yyvsp[-1].PERCENT_NAME_ಪೂರ್ವಪ್ರತ್ಯಯ), (yyvsp[0].STRING)); }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 18: /* prologue_declaration: "%no-lines"  */
                                { no_ಸಾಲುಗಳು_flag = true; }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 19: /* prologue_declaration: "%nondeterministic-parser"  */
                                { nondeterministic_parser = true; }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 20: /* prologue_declaration: "%output" "string"  */
                                { spec_outfile = unquote ((yyvsp[0].STRING)); gram_scanner_ಕೊನೆಯ_string_free (); }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 21: /* $@1: %empty  */
           { current_param = (yyvsp[0].PERCENT_PARAM); }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 22: /* prologue_declaration: "%param" $@1 params  */
                                          { current_param = param_none; }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 23: /* prologue_declaration: "%pure-parser"  */
                                { handle_ಶುದ್ಧ_parser (&(yyloc), (yyvsp[0].PERCENT_ಶುದ್ಧ_PARSER)); }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 24: /* prologue_declaration: "%require" "string"  */
                                { handle_require (&(yylsp[0]), (yyvsp[0].STRING)); }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 25: /* prologue_declaration: "%skeleton" "string"  */
                                { handle_skeleton (&(yylsp[0]), (yyvsp[0].STRING)); }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 26: /* prologue_declaration: "%token-table"  */
                                { token_ಕೋಷ್ಟಕ_flag = true; }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 27: /* prologue_declaration: "%verbose"  */
                                { report_flag |= report_ಸ್ಥಿತಿಗಳು; }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 28: /* prologue_declaration: "%yacc"  */
                                { handle_yacc (&(yyloc)); }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 29: /* prologue_declaration: error ";"  */
                                { current_class = unknown_sym; yyerrok; }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 31: /* params: params "{...}"  */
                   { ಕೂಡಿಸು_param (current_param, (yyvsp[0].BRACED_ಸಂಕೇತ), (yylsp[0])); }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 32: /* params: "{...}"  */
                   { ಕೂಡಿಸು_param (current_param, (yyvsp[0].BRACED_ಸಂಕೇತ), (yylsp[0])); }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 34: /* grammar_declaration: "%start" symbols.1  */
    {
      grammar_ಆರಂಭ_symbols_ಕೂಡಿಸು ((yyvsp[0].yykind_90));
    }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 35: /* grammar_declaration: code_props_type "{...}" generic_symlist  */
    {
      ಸಂಕೇತ_props ಸಂಕೇತ_2;
      ಸಂಕೇತ_props_ಚಿಹ್ನೆ_action_init (&ಸಂಕೇತ_2, (yyvsp[-1].BRACED_ಸಂಕೇತ), (yylsp[-1]));
      ಸಂಕೇತ_props_translate_ಸಂಕೇತ (&ಸಂಕೇತ_2);
      {
        ಗಾಗಿ (ಚಿಹ್ನೆ_list *list = (yyvsp[0].generic_symlist); list; list = list->ಮುಂದಿನ)
          ಚಿಹ್ನೆ_list_ಸಂಕೇತ_props_ಗಣ (list, (yyvsp[-2].ಸಂಕೇತ_props_type), &ಸಂಕೇತ_2);
        ಚಿಹ್ನೆ_list_free ((yyvsp[0].generic_symlist));
      }
    }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 36: /* grammar_declaration: "%default-prec"  */
    {
      default_prec = true;
    }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 37: /* grammar_declaration: "%no-default-prec"  */
    {
      default_prec = false;
    }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 38: /* grammar_declaration: "%code" "{...}"  */
    {
      /* Do not invoke muscle_percent_code_grow here since it invokes
         muscle_user_name_list_grow.  */
      muscle_ಸಂಕೇತ_grow ("percent_code()",
                        translate_ಸಂಕೇತ_braceless ((yyvsp[0].BRACED_ಸಂಕೇತ), (yylsp[0])), (yylsp[0]));
      ಸಂಕೇತ_scanner_ಕೊನೆಯ_string_free ();
    }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 39: /* grammar_declaration: "%code" "identifier" "{...}"  */
    {
      muscle_percent_ಸಂಕೇತ_grow ((yyvsp[-1].ID), (yylsp[-1]), translate_ಸಂಕೇತ_braceless ((yyvsp[0].BRACED_ಸಂಕೇತ), (yylsp[0])), (yylsp[0]));
      ಸಂಕೇತ_scanner_ಕೊನೆಯ_string_free ();
    }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 40: /* code_props_type: "%destructor"  */
                 { (yyval.ಸಂಕೇತ_props_type) = destructor; }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 41: /* code_props_type: "%printer"  */
                 { (yyval.ಸಂಕೇತ_props_type) = printer; }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 42: /* union_name: %empty  */
         {}
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 43: /* union_name: "identifier"  */
         { muscle_percent_define_ಸೇರಿಸು ("api.value.union.name",
                                         (yylsp[0]), muscle_keyword, (yyvsp[0].ID),
                                         MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE); }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 44: /* grammar_declaration: "%union" union_name "{...}"  */
    {
      union_ನೋಡಿದ = true;
      muscle_ಸಂಕೇತ_grow ("union_members", translate_ಸಂಕೇತ_braceless ((yyvsp[0].BRACED_ಸಂಕೇತ), (yylsp[0])), (yylsp[0]));
      ಸಂಕೇತ_scanner_ಕೊನೆಯ_string_free ();
    }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 45: /* $@2: %empty  */
           { current_class = nterm_sym; }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 46: /* symbol_declaration: "%nterm" $@2 nterm_decls  */
    {
      current_class = unknown_sym;
      ಚಿಹ್ನೆ_list_free ((yyvsp[0].nterm_decls));
    }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 47: /* $@3: %empty  */
           { current_class = token_sym; }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 48: /* symbol_declaration: "%token" $@3 token_decls  */
    {
      current_class = unknown_sym;
      ಚಿಹ್ನೆ_list_free ((yyvsp[0].token_decls));
    }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 49: /* $@4: %empty  */
          { current_class = pct_type_sym; }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 50: /* symbol_declaration: "%type" $@4 symbol_decls  */
    {
      current_class = unknown_sym;
      ಚಿಹ್ನೆ_list_free ((yyvsp[0].ಚಿಹ್ನೆ_decls));
    }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 51: /* symbol_declaration: precedence_declarator token_decls_for_prec  */
    {
      ++current_prec;
      ಗಾಗಿ (ಚಿಹ್ನೆ_list *list = (yyvsp[0].token_decls_for_prec); list; list = list->ಮುಂದಿನ)
        ಚಿಹ್ನೆ_precedence_ಗಣ (list->content.sym, current_prec, (yyvsp[-1].precedence_declarator), (yylsp[-1]));
      ಚಿಹ್ನೆ_list_free ((yyvsp[0].token_decls_for_prec));
    }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 52: /* precedence_declarator: "%left"  */
                { (yyval.precedence_declarator) = ಎಡ_assoc; }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 53: /* precedence_declarator: "%right"  */
                { (yyval.precedence_declarator) = ಬಲ_assoc; }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 54: /* precedence_declarator: "%nonassoc"  */
                { (yyval.precedence_declarator) = non_assoc; }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 55: /* precedence_declarator: "%precedence"  */
                { (yyval.precedence_declarator) = precedence_assoc; }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 56: /* string.opt: %empty  */
          { (yyval.yykind_75) = ಖಾಲಿ; }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 57: /* string.opt: "string"  */
          { (yyval.yykind_75) = (yyvsp[0].STRING); }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 58: /* tag.opt: %empty  */
         { (yyval.yykind_76) = ಖಾಲಿ; }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 59: /* tag.opt: "<tag>"  */
         { (yyval.yykind_76) = (yyvsp[0].ಟ್ಯಾಗ್_2); }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 61: /* generic_symlist: generic_symlist generic_symlist_item  */
                                         { (yyval.generic_symlist) = ಚಿಹ್ನೆ_list_append ((yyvsp[-1].generic_symlist), (yyvsp[0].generic_symlist_ಅಂಶ)); }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 62: /* generic_symlist_item: symbol  */
            { (yyval.generic_symlist_ಅಂಶ) = ಚಿಹ್ನೆ_list_sym_new ((yyvsp[0].ಚಿಹ್ನೆ), (yylsp[0])); }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 63: /* generic_symlist_item: tag  */
            { (yyval.generic_symlist_ಅಂಶ) = ಚಿಹ್ನೆ_list_type_new ((yyvsp[0].ಟ್ಯಾಗ್), (yylsp[0])); }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 65: /* tag: "<*>"  */
        { (yyval.ಟ್ಯಾಗ್) = uniqstr_new ("*"); }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 66: /* tag: "<>"  */
        { (yyval.ಟ್ಯಾಗ್) = uniqstr_new (""); }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 68: /* token_decls: token_decl.1  */
    {
      (yyval.token_decls) = (yyvsp[0].yykind_82);
    }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 69: /* token_decls: "<tag>" token_decl.1  */
    {
      (yyval.token_decls) = ಚಿಹ್ನೆ_list_type_ಗಣ ((yyvsp[0].yykind_82), (yyvsp[-1].ಟ್ಯಾಗ್_2));
    }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 70: /* token_decls: token_decls "<tag>" token_decl.1  */
    {
      (yyval.token_decls) = ಚಿಹ್ನೆ_list_append ((yyvsp[-2].token_decls), ಚಿಹ್ನೆ_list_type_ಗಣ ((yyvsp[0].yykind_82), (yyvsp[-1].ಟ್ಯಾಗ್_2)));
    }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 71: /* token_decl.1: token_decl  */
                            { (yyval.yykind_82) = ಚಿಹ್ನೆ_list_sym_new ((yyvsp[0].token_decl), (yylsp[0])); }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 72: /* token_decl.1: token_decl.1 token_decl  */
                            { (yyval.yykind_82) = ಚಿಹ್ನೆ_list_append ((yyvsp[-1].yykind_82), ಚಿಹ್ನೆ_list_sym_new ((yyvsp[0].token_decl), (yylsp[0]))); }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 73: /* token_decl: id int.opt alias  */
    {
      (yyval.token_decl) = (yyvsp[-2].id);
      ಚಿಹ್ನೆ_class_ಗಣ ((yyvsp[-2].id), current_class, (yylsp[-2]), true);
      ಆದರೆ (0 <= (yyvsp[-1].yykind_84))
        ಚಿಹ್ನೆ_ಸಂಕೇತ_ಗಣ ((yyvsp[-2].id), (yyvsp[-1].yykind_84), (yylsp[-1]));
      ಆದರೆ ((yyvsp[0].alias))
        ಚಿಹ್ನೆ_make_alias ((yyvsp[-2].id), (yyvsp[0].alias), (yylsp[0]));
    }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 74: /* int.opt: %empty  */
          { (yyval.yykind_84) = -1; }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 76: /* alias: %empty  */
                 { (yyval.alias) = ಖಾಲಿ; }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 77: /* alias: string_as_id  */
                 { (yyval.alias) = (yyvsp[0].string_as_id); }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 78: /* alias: "translatable string"  */
    {
      (yyval.alias) = ಚಿಹ್ನೆ_get ((yyvsp[0].TSTRING), (yylsp[0]));
      ಚಿಹ್ನೆ_class_ಗಣ ((yyval.alias), token_sym, (yylsp[0]), false);
      (yyval.alias)->translatable = true;
    }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 79: /* token_decls_for_prec: token_decl_for_prec.1  */
    {
      (yyval.token_decls_for_prec) = (yyvsp[0].yykind_87);
    }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 80: /* token_decls_for_prec: "<tag>" token_decl_for_prec.1  */
    {
      (yyval.token_decls_for_prec) = ಚಿಹ್ನೆ_list_type_ಗಣ ((yyvsp[0].yykind_87), (yyvsp[-1].ಟ್ಯಾಗ್_2));
    }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 81: /* token_decls_for_prec: token_decls_for_prec "<tag>" token_decl_for_prec.1  */
    {
      (yyval.token_decls_for_prec) = ಚಿಹ್ನೆ_list_append ((yyvsp[-2].token_decls_for_prec), ಚಿಹ್ನೆ_list_type_ಗಣ ((yyvsp[0].yykind_87), (yyvsp[-1].ಟ್ಯಾಗ್_2)));
    }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 82: /* token_decl_for_prec.1: token_decl_for_prec  */
    { (yyval.yykind_87) = ಚಿಹ್ನೆ_list_sym_new ((yyvsp[0].token_decl_for_prec), (yylsp[0])); }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 83: /* token_decl_for_prec.1: token_decl_for_prec.1 token_decl_for_prec  */
    { (yyval.yykind_87) = ಚಿಹ್ನೆ_list_append ((yyvsp[-1].yykind_87), ಚಿಹ್ನೆ_list_sym_new ((yyvsp[0].token_decl_for_prec), (yylsp[0]))); }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 84: /* token_decl_for_prec: id int.opt  */
    {
      (yyval.token_decl_for_prec) = (yyvsp[-1].id);
      ಚಿಹ್ನೆ_class_ಗಣ ((yyvsp[-1].id), token_sym, (yylsp[-1]), false);
      ಆದರೆ (0 <= (yyvsp[0].yykind_84))
        ಚಿಹ್ನೆ_ಸಂಕೇತ_ಗಣ ((yyvsp[-1].id), (yyvsp[0].yykind_84), (yylsp[0]));
    }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 86: /* symbol_decls: symbols.1  */
    {
      (yyval.ಚಿಹ್ನೆ_decls) = (yyvsp[0].yykind_90);
    }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 87: /* symbol_decls: "<tag>" symbols.1  */
    {
      (yyval.ಚಿಹ್ನೆ_decls) = ಚಿಹ್ನೆ_list_type_ಗಣ ((yyvsp[0].yykind_90), (yyvsp[-1].ಟ್ಯಾಗ್_2));
    }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 88: /* symbol_decls: symbol_decls "<tag>" symbols.1  */
    {
      (yyval.ಚಿಹ್ನೆ_decls) = ಚಿಹ್ನೆ_list_append ((yyvsp[-2].ಚಿಹ್ನೆ_decls), ಚಿಹ್ನೆ_list_type_ಗಣ ((yyvsp[0].yykind_90), (yyvsp[-1].ಟ್ಯಾಗ್_2)));
    }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 89: /* symbols.1: symbol  */
    {
      ಆದರೆ (current_class != unknown_sym)
        ಚಿಹ್ನೆ_class_ಗಣ ((yyvsp[0].ಚಿಹ್ನೆ), current_class, (yylsp[0]), false);
      (yyval.yykind_90) = ಚಿಹ್ನೆ_list_sym_new ((yyvsp[0].ಚಿಹ್ನೆ), (yylsp[0]));
    }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 90: /* symbols.1: symbols.1 symbol  */
    {
      ಆದರೆ (current_class != unknown_sym)
        ಚಿಹ್ನೆ_class_ಗಣ ((yyvsp[0].ಚಿಹ್ನೆ), current_class, (yylsp[0]), false);
      (yyval.yykind_90) = ಚಿಹ್ನೆ_list_append ((yyvsp[-1].yykind_90), ಚಿಹ್ನೆ_list_sym_new ((yyvsp[0].ಚಿಹ್ನೆ), (yylsp[0])));
    }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 95: /* rules_or_grammar_declaration: error ";"  */
    {
      yyerrok;
    }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 96: /* $@5: %empty  */
                         { current_lhs ((yyvsp[-1].id_colon), (yylsp[-1]), (yyvsp[0].yykind_97)); }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 97: /* rules: id_colon named_ref.opt $@5 ":" rhses.1  */
    {
      /* Free the current lhs. */
      current_lhs (0, (yylsp[-4]), 0);
    }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 98: /* rhses.1: rhs  */
                     { grammar_current_ನಿಯಮ_ಕೊನೆ ((yylsp[0])); }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 99: /* rhses.1: rhses.1 "|" rhs  */
                     { grammar_current_ನಿಯಮ_ಕೊನೆ ((yylsp[0])); }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 101: /* rhs: %empty  */
    { grammar_current_ನಿಯಮ_begin (current_lhs_ಚಿಹ್ನೆ, current_lhs_loc,
                                  current_lhs_named_ref); }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 102: /* rhs: rhs symbol named_ref.opt  */
    { grammar_current_ನಿಯಮ_ಚಿಹ್ನೆ_append ((yyvsp[-1].ಚಿಹ್ನೆ), (yylsp[-1]), (yyvsp[0].yykind_97)); }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 103: /* rhs: rhs tag.opt "{...}" named_ref.opt  */
    { grammar_current_ನಿಯಮ_action_append ((yyvsp[-1].BRACED_ಸಂಕೇತ), (yylsp[-1]), (yyvsp[0].yykind_97), (yyvsp[-2].yykind_76)); }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 104: /* rhs: rhs "%?{...}"  */
    { grammar_current_ನಿಯಮ_predicate_append ((yyvsp[0].BRACED_PREDICATE), (yylsp[0])); }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 105: /* rhs: rhs "%empty"  */
    { grammar_current_ನಿಯಮ_ಖಾಲಿಗಳು_ಗಣ ((yylsp[0])); }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 106: /* rhs: rhs "%prec" symbol  */
    { grammar_current_ನಿಯಮ_prec_ಗಣ ((yyvsp[0].ಚಿಹ್ನೆ), (yylsp[0])); }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 107: /* rhs: rhs "%dprec" "integer literal"  */
    { grammar_current_ನಿಯಮ_dprec_ಗಣ ((yyvsp[0].INT_LITERAL), (yylsp[0])); }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 108: /* rhs: rhs "%merge" "<tag>"  */
    { grammar_current_ನಿಯಮ_ವಿಲೀನ_ಗಣ ((yyvsp[0].ಟ್ಯಾಗ್_2), (yylsp[0])); }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 109: /* rhs: rhs "%expect" "integer literal"  */
    { grammar_current_ನಿಯಮ_expect_sr ((yyvsp[0].INT_LITERAL), (yylsp[0])); }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 110: /* rhs: rhs "%expect-rr" "integer literal"  */
    { grammar_current_ನಿಯಮ_expect_rr ((yyvsp[0].INT_LITERAL), (yylsp[0])); }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 111: /* named_ref.opt: %empty  */
                 { (yyval.yykind_97) = ಖಾಲಿ; }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 112: /* named_ref.opt: "[identifier]"  */
                 { (yyval.yykind_97) = named_ref_new ((yyvsp[0].BRACKETED_ID), (yylsp[0])); }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 114: /* value: %empty  */
          { (yyval.ಮೌಲ್ಯ).ಬಗೆ = muscle_keyword; (yyval.ಮೌಲ್ಯ).chars = ""; }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 115: /* value: "identifier"  */
          { (yyval.ಮೌಲ್ಯ).ಬಗೆ = muscle_keyword; (yyval.ಮೌಲ್ಯ).chars = (yyvsp[0].ID); }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 116: /* value: "string"  */
          { (yyval.ಮೌಲ್ಯ).ಬಗೆ = muscle_string;  (yyval.ಮೌಲ್ಯ).chars = unquote ((yyvsp[0].STRING)); gram_scanner_ಕೊನೆಯ_string_free ();}
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 117: /* value: "{...}"  */
          { (yyval.ಮೌಲ್ಯ).ಬಗೆ = muscle_ಸಂಕೇತ;    (yyval.ಮೌಲ್ಯ).chars = strip_braces ((yyvsp[0].BRACED_ಸಂಕೇತ)); gram_scanner_ಕೊನೆಯ_string_free (); }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 118: /* id: "identifier"  */
    { (yyval.id) = ಚಿಹ್ನೆ_from_uniqstr ((yyvsp[0].ID), (yylsp[0])); }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 119: /* id: "character literal"  */
    {
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *var = "api.token.raw";
      ಆದರೆ (current_class == nterm_sym)
        {
          complain (&(yylsp[0]), complaint,
                    _("character literals cannot be nonterminals"));
          YYERROR;
        }
      ಆದರೆ (muscle_percent_define_ifdef (var))
        {
          complain (&(yylsp[0]), complaint,
                    _("character literals cannot be used together"
                    " with %s"), var);
          location loc = muscle_percent_define_get_loc (var);
          subcomplain (&loc, complaint, _("definition of %s"), var);
        }
      (yyval.id) = ಚಿಹ್ನೆ_get (char_name ((yyvsp[0].CHAR_LITERAL)), (yylsp[0]));
      ಚಿಹ್ನೆ_class_ಗಣ ((yyval.id), token_sym, (yylsp[0]), false);
      ಚಿಹ್ನೆ_ಸಂಕೇತ_ಗಣ ((yyval.id), (yyvsp[0].CHAR_LITERAL), (yylsp[0]));
    }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 120: /* id_colon: "identifier:"  */
           { (yyval.id_colon) = ಚಿಹ್ನೆ_from_uniqstr ((yyvsp[0].ID_COLON), (yylsp[0])); }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 123: /* string_as_id: "string"  */
    {
      (yyval.string_as_id) = ಚಿಹ್ನೆ_get ((yyvsp[0].STRING), (yylsp[0]));
      ಚಿಹ್ನೆ_class_ಗಣ ((yyval.string_as_id), token_sym, (yylsp[0]), false);
    }
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 125: /* epilogue.opt: "%%" "epilogue"  */
    {
      muscle_ಸಂಕೇತ_grow ("epilogue", translate_ಸಂಕೇತ ((yyvsp[0].EPILOGUE), (yylsp[0]), true), (yylsp[0]));
      ಸಂಕೇತ_scanner_ಕೊನೆಯ_string_free ();
    }
    ನಿಲ್ಲಿಸು;



        ಪೂರ್ವನಿಯೋಜಿತ: ನಿಲ್ಲಿಸು;
      }
    ಆದರೆ (yychar_backup != yychar)
      YY_LAC_DISCARD ("yychar change");
  }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_ಚಿಹ್ನೆ_PRINT ("-> $$ =", YY_CAST (yysymbol_ಬಗೆ_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ yylhs = yyr1[yyn] - YYNTOKENS;
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  ಹೋಗು yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == GRAM_ಖಾಲಿಗಳು ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  ಆದರೆ (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yyesa, &yyes, &yyes_ಸಾಮರ್ಥ್ಯ, yytoken, &yylloc};
        ಆದರೆ (yychar != GRAM_ಖಾಲಿಗಳು)
          YY_LAC_ESTABLISH;
        ಆದರೆ (yyreport_syntax_error (&yyctx) == 2)
          YYNOMEM;
      }
    }

  yyerror_range[1] = yylloc;
  ಆದರೆ (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      ಆದರೆ (yychar <= GRAM_EOF)
        {
          /* Return failure if at end of input.  */
          ಆದರೆ (yychar == GRAM_EOF)
            YYABORT;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc);
          yychar = GRAM_ಖಾಲಿಗಳು;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  ಹೋಗು yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  ಆದರೆ (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_ರಾಶಿ_PRINT (yyss, yyssp);
  yystate = *yyssp;
  ಹೋಗು yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  ಗಾಗಿ (;;)
    {
      yyn = yypact[yystate];
      ಆದರೆ (!yypact_ಮೌಲ್ಯ_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          ಆದರೆ (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              ಆದರೆ (0 < yyn)
                ನಿಲ್ಲಿಸು;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      ಆದರೆ (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_ಚಿಹ್ನೆ (yystate), yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_ರಾಶಿ_PRINT (yyss, yyssp);
    }

  /* If the stack popping above didn't lose the initial context for the
     current lookahead token, the shift below will for sure.  */
  YY_LAC_DISCARD ("error recovery");

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_ಕೊನೆ

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

  /* Shift the error token.  */
  YY_ಚಿಹ್ನೆ_PRINT ("Shifting", YY_ACCESSING_ಚಿಹ್ನೆ (yyn), yyvsp, yylsp);

  yystate = yyn;
  ಹೋಗು yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  ಹೋಗು yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  ಹೋಗು yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, YY_("memory exhausted"));
  yyresult = 2;
  ಹೋಗು yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  ಆದರೆ (yychar != GRAM_ಖಾಲಿಗಳು)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_ರಾಶಿ_PRINT (yyss, yyssp);
  ಆಗ (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_ಚಿಹ್ನೆ (+*yyssp), yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ yyoverflow
  ಆದರೆ (yyss != yyssa)
    YYSTACK_FREE (yyss);
#ಆದರೆ_ಅಂತ್ಯ
  ಆದರೆ (yyes != yyesa)
    YYSTACK_FREE (yyes);

  ಹಿಂತಿರುಗಿ yyresult;
}



ಪೂರ್ಣಾಂಕ
yyreport_syntax_error (ಸ್ಥಿರಾಂಕ yypcontext_t *ctx)
{
  ಪೂರ್ಣಾಂಕ res = 0;
  /* Arguments of format: reported tokens (one for the "unexpected",
     one per "expected"). */
  ಎಣಿಕೆ { ವಾದಗಳು_MAX = 5 };
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *argv[ವಾದಗಳು_MAX];
  ಪೂರ್ಣಾಂಕ argc = 0;
  yysymbol_ಬಗೆ_t unexpected = yypcontext_token (ctx);
  ಆದರೆ (unexpected != YYSYMBOL_YYEMPTY)
    {
      argv[argc++] = yysymbol_name (unexpected);
      yysymbol_ಬಗೆ_t expected[ವಾದಗಳು_MAX - 1];
      ಪೂರ್ಣಾಂಕ nexpected = yypcontext_expected_ಟೋಕನುಗಳು (ctx, expected, ವಾದಗಳು_MAX - 1);
      ಆದರೆ (nexpected < 0)
        res = nexpected;
      ಇಲ್ಲದಿದ್ದರೆ
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < nexpected; ++i)
          argv[argc++] = yysymbol_name (expected[i]);
    }
  syntax_error (*yypcontext_location (ctx), argc, argv);
  ಹಿಂತಿರುಗಿ res;
}


/* Return the location of the left-hand side of a rule whose
   right-hand side is RHS[1] ... RHS[N].  Ignore empty nonterminals in
   the right-hand side, and return an empty location equal to the end
   boundary of RHS[0] if the right-hand side is empty.  */

ಸ್ಥಿರ YYLTYPE
lloc_default (YYLTYPE ಸ್ಥಿರಾಂಕ *rhs, ಪೂರ್ಣಾಂಕ n)
{
  YYLTYPE loc;

  /* SGI MIPSpro 7.4.1m miscompiles "loc.start = loc.end = rhs[n].end;".
     The bug is fixed in 7.4.2m, but play it safe for now.  */
  loc.ಆರಂಭ = rhs[n].ಕೊನೆ;
  loc.ಕೊನೆ = rhs[n].ಕೊನೆ;

  /* Ignore empty nonterminals the start of the right-hand side.
     Do not bother to ignore them at the end of the right-hand side,
     since empty nonterminals have the same end as their predecessors.  */
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 1; i <= n; i++)
    ಆದರೆ (! equal_boundaries (rhs[i].ಆರಂಭ, rhs[i].ಕೊನೆ))
      {
        loc.ಆರಂಭ = rhs[i].ಆರಂಭ;
        ನಿಲ್ಲಿಸು;
      }

  ಹಿಂತಿರುಗಿ loc;
}

ಸ್ಥಿರ
ಅಕ್ಷರ *strip_braces (ಅಕ್ಷರ *ಸಂಕೇತ_2)
{
  ಸಂಕೇತ_2[ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ಸಂಕೇತ_2) - 1] = 0;
  ಹಿಂತಿರುಗಿ ಸಂಕೇತ_2 + 1;
}

ಸ್ಥಿರ
ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *
translate_ಸಂಕೇತ (ಅಕ್ಷರ *ಸಂಕೇತ_2, location loc, bool plain)
{
  ಸಂಕೇತ_props plain_ಸಂಕೇತ;
  ಆದರೆ (plain)
    ಸಂಕೇತ_props_plain_init (&plain_ಸಂಕೇತ, ಸಂಕೇತ_2, loc);
  ಇಲ್ಲದಿದ್ದರೆ
    ಸಂಕೇತ_props_ಚಿಹ್ನೆ_action_init (&plain_ಸಂಕೇತ, ಸಂಕೇತ_2, loc);
  ಸಂಕೇತ_props_translate_ಸಂಕೇತ (&plain_ಸಂಕೇತ);
  gram_scanner_ಕೊನೆಯ_string_free ();
  ಹಿಂತಿರುಗಿ plain_ಸಂಕೇತ.ಸಂಕೇತ_2;
}

ಸ್ಥಿರ
ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *
translate_ಸಂಕೇತ_braceless (ಅಕ್ಷರ *ಸಂಕೇತ_2, location loc)
{
  ಹಿಂತಿರುಗಿ translate_ಸಂಕೇತ (strip_braces (ಸಂಕೇತ_2), loc, true);
}

ಸ್ಥಿರ ಶೂನ್ಯ
ಕೂಡಿಸು_param (param_type type, ಅಕ್ಷರ *decl, location loc)
{
  ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ alphanum[26 + 26 + 1 + 10 + 1] =
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "_"
    "0123456789";

  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name_ಆರಂಭ = ಖಾಲಿ;
  {
    ಅಕ್ಷರ *p;
    /* Stop on last actual character.  */
    ಗಾಗಿ (p = decl; p[1]; p++)
      ಆದರೆ ((p == decl
           || ! memchr (alphanum, p[-1], ಗಾತ್ರ alphanum - 1))
          && memchr (alphanum, p[0], ಗಾತ್ರ alphanum - 10 - 1))
        name_ಆರಂಭ = p;

    /* Strip the surrounding '{' and '}', and any blanks just inside
       the braces.  */
    --p;
    ಆಗ (c_isspace ((ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ) *p))
      --p;
    p[1] = '\0';
    ++decl;
    ಆಗ (c_isspace ((ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ) *decl))
      ++decl;
  }

  ಆದರೆ (! name_ಆರಂಭ)
    complain (&loc, complaint, _("missing identifier in parameter declaration"));
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಅಕ್ಷರ *name = xmemdup0 (name_ಆರಂಭ, strspn (name_ಆರಂಭ, alphanum));
      ಆದರೆ (type & param_lex)
        muscle_pair_list_grow ("lex_param", decl, name);
      ಆದರೆ (type & param_parse)
        muscle_pair_list_grow ("parse_param", decl, name);
      ಬಿಡುಗಡೆ (name);
    }

  gram_scanner_ಕೊನೆಯ_string_free ();
}


ಸ್ಥಿರ ಶೂನ್ಯ
handle_ಶೀರ್ಷಿಕೆ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಮೌಲ್ಯ)
{
  ಶೀರ್ಷಿಕೆ_flag = true;
  ಆದರೆ (ಮೌಲ್ಯ)
    {
      ಅಕ್ಷರ *file = unquote (ಮೌಲ್ಯ);
      spec_ಶೀರ್ಷಿಕೆ_file = xstrdup (file);
      gram_scanner_ಕೊನೆಯ_string_free ();
      unquote_free (file);
    }
}


ಸ್ಥಿರ ಶೂನ್ಯ
handle_error_verbose (location ಸ್ಥಿರಾಂಕ *loc, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *directive)
{
  bison_directive (loc, directive);
  muscle_percent_define_ಸೇರಿಸು (directive, *loc, muscle_keyword, "",
                                MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
}


ಸ್ಥಿರ ಶೂನ್ಯ
handle_file_ಪೂರ್ವಪ್ರತ್ಯಯ (location ಸ್ಥಿರಾಂಕ *loc,
                    location ಸ್ಥಿರಾಂಕ *dir_loc,
                    ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *directive, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಮೌಲ್ಯ_quoted)
{
  ಅಕ್ಷರ *ಮೌಲ್ಯ = unquote (ಮೌಲ್ಯ_quoted);
  bison_directive (loc, directive);
  bool warned = false;

  ಆದರೆ (location_ಖಾಲಿಗಳು (spec_file_ಪೂರ್ವಪ್ರತ್ಯಯ_loc))
    {
      spec_file_ಪೂರ್ವಪ್ರತ್ಯಯ_loc = *loc;
      spec_file_ಪೂರ್ವಪ್ರತ್ಯಯ = ಮೌಲ್ಯ;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      duplicate_directive (directive, spec_file_ಪೂರ್ವಪ್ರತ್ಯಯ_loc, *loc);
      warned = true;
    }

  ಆದರೆ (!warned
      && STRNEQ (directive, "%file-prefix"))
    deprecated_directive (dir_loc, directive, "%file-prefix");
}

ಸ್ಥಿರ ಶೂನ್ಯ
handle_language (location ಸ್ಥಿರಾಂಕ *loc, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *lang)
{
  language_argmatch (unquote (lang), grammar_prio, *loc);
}


ಸ್ಥಿರ ಶೂನ್ಯ
handle_name_ಪೂರ್ವಪ್ರತ್ಯಯ (location ಸ್ಥಿರಾಂಕ *loc,
                    ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *directive, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಮೌಲ್ಯ_quoted)
{
  ಅಕ್ಷರ *ಮೌಲ್ಯ = unquote (ಮೌಲ್ಯ_quoted);
  bison_directive (loc, directive);

  ಅಕ್ಷರ buf1[1024];
  size_t len1 = ಗಾತ್ರ (buf1);
  ಅಕ್ಷರ *old = asnprintf (buf1, &len1, "%s\"%s\"", directive, ಮೌಲ್ಯ);
  ಆದರೆ (!old)
    xalloc_die ();

  ಆದರೆ (location_ಖಾಲಿಗಳು (spec_name_ಪೂರ್ವಪ್ರತ್ಯಯ_loc))
    {
      spec_name_ಪೂರ್ವಪ್ರತ್ಯಯ = ಮೌಲ್ಯ;
      spec_name_ಪೂರ್ವಪ್ರತ್ಯಯ_loc = *loc;

      ಅಕ್ಷರ buf2[1024];
      size_t len2 = ಗಾತ್ರ (buf2);
      ಅಕ್ಷರ *new = asnprintf (buf2, &len2, "%%define api.prefix {%s}", ಮೌಲ್ಯ);
      ಆದರೆ (!new)
        xalloc_die ();
      deprecated_directive (loc, old, new);
      ಆದರೆ (new != buf2)
        ಬಿಡುಗಡೆ (new);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    duplicate_directive (old, spec_file_ಪೂರ್ವಪ್ರತ್ಯಯ_loc, *loc);

  ಆದರೆ (old != buf1)
    ಬಿಡುಗಡೆ (old);
}


ಸ್ಥಿರ ಶೂನ್ಯ
handle_ಶುದ್ಧ_parser (location ಸ್ಥಿರಾಂಕ *loc, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *directive)
{
  bison_directive (loc, directive);
  deprecated_directive (loc, directive, "%define api.pure");
  muscle_percent_define_ಸೇರಿಸು ("api.pure", *loc, muscle_keyword, "",
                                MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
}


ಸ್ಥಿರ ಶೂನ್ಯ
handle_require (location ಸ್ಥಿರಾಂಕ *loc, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಆವೃತ್ತಿ_quoted)
{
  ಅಕ್ಷರ *ಆವೃತ್ತಿ = unquote (ಆವೃತ್ತಿ_quoted);
  required_ಆವೃತ್ತಿ = strversion_ಗಮ್ಯ_int (ಆವೃತ್ತಿ);
  ಆದರೆ (required_ಆವೃತ್ತಿ == -1)
    {
      complain (loc, complaint, _("invalid version requirement: %s"),
                ಆವೃತ್ತಿ);
      required_ಆವೃತ್ತಿ = 0;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* package_ಆವೃತ್ತಿ =
        0 < strverscmp (api_ಆವೃತ್ತಿ, PACKAGE_ಆವೃತ್ತಿ)
        ? api_ಆವೃತ್ತಿ : PACKAGE_ಆವೃತ್ತಿ;
      ಆದರೆ (0 < strverscmp (ಆವೃತ್ತಿ, package_ಆವೃತ್ತಿ))
        {
          complain (loc, complaint, _("require bison %s, but have %s"),
                    ಆವೃತ್ತಿ, package_ಆವೃತ್ತಿ);
          ನಿರ್ಗಮಿಸು (EX_MISMATCH);
        }
    }
  unquote_free (ಆವೃತ್ತಿ);
  gram_scanner_ಕೊನೆಯ_string_free ();
}

ಸ್ಥಿರ ಶೂನ್ಯ
handle_skeleton (location ಸ್ಥಿರಾಂಕ *loc, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *skel_quoted)
{
  ಅಕ್ಷರ *skel = unquote (skel_quoted);
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *skeleton_user = skel;
  ಆದರೆ (strchr (skeleton_user, '/'))
    {
      size_t dir_ಉದ್ದಳತೆ = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (grammar_file);
      ಆಗ (dir_ಉದ್ದಳತೆ && grammar_file[dir_ಉದ್ದಳತೆ - 1] != '/')
        --dir_ಉದ್ದಳತೆ;
      ಆಗ (dir_ಉದ್ದಳತೆ && grammar_file[dir_ಉದ್ದಳತೆ - 1] == '/')
        --dir_ಉದ್ದಳತೆ;
      ಅಕ್ಷರ *skeleton_build =
        xmalloc (dir_ಉದ್ದಳತೆ + 1 + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (skeleton_user) + 1);
      ಆದರೆ (dir_ಉದ್ದಳತೆ > 0)
        {
          ಸ್ಮರಣೆ_ನಕಲು (skeleton_build, grammar_file, dir_ಉದ್ದಳತೆ);
          skeleton_build[dir_ಉದ್ದಳತೆ++] = '/';
        }
      strcpy (skeleton_build + dir_ಉದ್ದಳತೆ, skeleton_user);
      skeleton_user = uniqstr_new (skeleton_build);
      ಬಿಡುಗಡೆ (skeleton_build);
    }
  skeleton_arg (skeleton_user, grammar_prio, *loc);
}


ಸ್ಥಿರ ಶೂನ್ಯ
handle_yacc (location ಸ್ಥಿರಾಂಕ *loc)
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *directive = "%yacc";
  bison_directive (loc, directive);
  ಆದರೆ (location_ಖಾಲಿಗಳು (yacc_loc))
    ಗಣ_yacc (*loc);
  ಇಲ್ಲದಿದ್ದರೆ
    duplicate_directive (directive, yacc_loc, *loc);
}


ಸ್ಥಿರ ಶೂನ್ಯ
gram_error (location ಸ್ಥಿರಾಂಕ *loc, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸಂದೇಶ_2)
{
  complain (loc, complaint, "%s", ಸಂದೇಶ_2);
}

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *
char_name (ಅಕ್ಷರ c)
{
  ಆದರೆ (c == '\'')
    ಹಿಂತಿರುಗಿ "'\\''";
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಅಕ್ಷರ buf[4];
      buf[0] = '\''; buf[1] = c; buf[2] = '\''; buf[3] = '\0';
      ಹಿಂತಿರುಗಿ quotearg_style (escape_quoting_style, buf);
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
current_lhs (ಚಿಹ್ನೆ *sym, location loc, named_ref *ref)
{
  current_lhs_ಚಿಹ್ನೆ = sym;
  current_lhs_loc = loc;
  ಆದರೆ (sym)
    ಚಿಹ್ನೆ_location_as_lhs_ಗಣ (sym, loc);
  /* In order to simplify memory management, named references for lhs
     are always assigned by deep copy into the current symbol_list
     node.  This is because a single named-ref in the grammar may
     result in several uses when the user factors lhs between several
     rules using "|".  Therefore free the parser's original copy.  */
  ಬಿಡುಗಡೆ (current_lhs_named_ref);
  current_lhs_named_ref = ref;
}

ಸ್ಥಿರ ಶೂನ್ಯ tron (FILE *yyo)
{
  begin_use_class ("value", yyo);
}

ಸ್ಥಿರ ಶೂನ್ಯ troff (FILE *yyo)
{
  ಕೊನೆ_use_class ("value", yyo);
}


/*----------.
| Unquote.  |
`----------*/

ರಚನೆ obstack obstack_for_unquote;

ಶೂನ್ಯ
parser_init (ಶೂನ್ಯ)
{
  obstack_init (&obstack_for_unquote);
}

ಶೂನ್ಯ
parser_free (ಶೂನ್ಯ)
{
  obstack_free (&obstack_for_unquote, 0);
}

ಸ್ಥಿರ ಶೂನ್ಯ
unquote_free (ಅಕ್ಷರ *ಕೊನೆಯ_string)
{
  obstack_free (&obstack_for_unquote, ಕೊನೆಯ_string);
}

ಸ್ಥಿರ ಅಕ್ಷರ *
unquote (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cp)
{
#ವ್ಯಾಖ್ಯಾನಿಸು GROW(Char)                              \
  obstack_1grow (&obstack_for_unquote, Char);
  ಗಾಗಿ (++cp; *cp && *cp != '"'; ++cp)
    ಆಯ್ಕೆ (*cp)
      {
      ಸ್ಥಿತಿ '"':
        ನಿಲ್ಲಿಸು;
      ಸ್ಥಿತಿ '\\':
        ++cp;
        ಆಯ್ಕೆ (*cp)
          {
          ಸ್ಥಿತಿ '0': ಸ್ಥಿತಿ '1': ಸ್ಥಿತಿ '2': ಸ್ಥಿತಿ '3': ಸ್ಥಿತಿ '4':
          ಸ್ಥಿತಿ '5': ಸ್ಥಿತಿ '6': ಸ್ಥಿತಿ '7': ಸ್ಥಿತಿ '8': ಸ್ಥಿತಿ '9':
            {
              ಪೂರ್ಣಾಂಕ c = cp[0] - '0';
              ಆದರೆ (c_isdigit (cp[1]))
                {
                  ++cp;
                  c = c * 8 + cp[0] - '0';
                }
              ಆದರೆ (c_isdigit (cp[1]))
                {
                  ++cp;
                  c = c * 8 + cp[0] - '0';
                }
              GROW (c);
            }
            ನಿಲ್ಲಿಸು;

          ಸ್ಥಿತಿ 'a': GROW ('\a'); ನಿಲ್ಲಿಸು;
          ಸ್ಥಿತಿ 'b': GROW ('\b'); ನಿಲ್ಲಿಸು;
          ಸ್ಥಿತಿ 'f': GROW ('\f'); ನಿಲ್ಲಿಸು;
          ಸ್ಥಿತಿ 'n': GROW ('\n'); ನಿಲ್ಲಿಸು;
          ಸ್ಥಿತಿ 'r': GROW ('\r'); ನಿಲ್ಲಿಸು;
          ಸ್ಥಿತಿ 't': GROW ('\t'); ನಿಲ್ಲಿಸು;
          ಸ್ಥಿತಿ 'v': GROW ('\v'); ನಿಲ್ಲಿಸು;

          ಸ್ಥಿತಿ 'x':
            {
              ಪೂರ್ಣಾಂಕ c = 0;
              ಆಗ (c_isxdigit (cp[1]))
                {
                  ++cp;
                  c = (c * 16 + (c_isdigit (cp[0]) ? cp[0] - '0'
                                 : c_isupper (cp[0]) ? cp[0] - 'A'
                                 : cp[0] - '0'));
                }
              GROW (c);
              ನಿಲ್ಲಿಸು;
            }
          }
        ನಿಲ್ಲಿಸು;

      ಪೂರ್ವನಿಯೋಜಿತ:
        GROW (*cp);
        ನಿಲ್ಲಿಸು;
      }
  ಪ್ರತಿಜ್ಞಾ (*cp == '"');
  ++cp;
  ಪ್ರತಿಜ್ಞಾ (*cp == '\0');
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು GROW
  ಹಿಂತಿರುಗಿ obstack_finish0 (&obstack_for_unquote);
}
