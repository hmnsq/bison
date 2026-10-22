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
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2.30-82269"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* "%code top" blocks.  */

  /* On column 0 to please syntax-check.  */
#ಅನ್ತರ್ಭಾವ <config.h>

/* Substitute the type names.  */
#define YYSTYPE         GRAM_STYPE
#define YYLTYPE         GRAM_LTYPE
/* Substitute the variable and function names.  */
#define yyparse         gram_parse
#define yylex           gram_lex
#define yyerror         gram_error
#define yydebug         gram_debug
#define yynerrs         gram_nerrs


# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  ಅನ್ಯಥಾ
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  ಯದಿ defined __cplusplus
#   ಯದಿ 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   ಅನ್ಯಥಾ
#    define YY_NULLPTR 0
#   endif
#  ಅನ್ಯಥಾ
#   define YY_NULLPTR ((ಶೂನ್ಯ*)0)
#  endif
# endif

#ಅನ್ತರ್ಭಾವ "src/parse-gram.h"
/* Symbol kind.  */
ಗಣನಾ yysymbol_kind_t
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
  YYSYMBOL_PERCENT_MERGE = 16,             /* "%merge"  */
  YYSYMBOL_PERCENT_CODE = 17,              /* "%code"  */
  YYSYMBOL_PERCENT_DEFAULT_PREC = 18,      /* "%default-prec"  */
  YYSYMBOL_PERCENT_DEFINE = 19,            /* "%define"  */
  YYSYMBOL_PERCENT_ERROR_VERBOSE = 20,     /* "%error-verbose"  */
  YYSYMBOL_PERCENT_EXPECT = 21,            /* "%expect"  */
  YYSYMBOL_PERCENT_EXPECT_RR = 22,         /* "%expect-rr"  */
  YYSYMBOL_PERCENT_FILE_PREFIX = 23,       /* "%file-prefix"  */
  YYSYMBOL_PERCENT_FLAG = 24,              /* "%<flag>"  */
  YYSYMBOL_PERCENT_GLR_PARSER = 25,        /* "%glr-parser"  */
  YYSYMBOL_PERCENT_HEADER = 26,            /* "%header"  */
  YYSYMBOL_PERCENT_INITIAL_ACTION = 27,    /* "%initial-action"  */
  YYSYMBOL_PERCENT_LANGUAGE = 28,          /* "%language"  */
  YYSYMBOL_PERCENT_NAME_PREFIX = 29,       /* "%name-prefix"  */
  YYSYMBOL_PERCENT_NO_DEFAULT_PREC = 30,   /* "%no-default-prec"  */
  YYSYMBOL_PERCENT_NO_ಸಾಲುಗಳು = 31,          /* "%no-lines"  */
  YYSYMBOL_PERCENT_NONDETERMINISTIC_PARSER = 32, /* "%nondeterministic-parser"  */
  YYSYMBOL_PERCENT_OUTPUT = 33,            /* "%output"  */
  YYSYMBOL_PERCENT_PURE_PARSER = 34,       /* "%pure-parser"  */
  YYSYMBOL_PERCENT_REQUIRE = 35,           /* "%require"  */
  YYSYMBOL_PERCENT_SKELETON = 36,          /* "%skeleton"  */
  YYSYMBOL_PERCENT_START = 37,             /* "%start"  */
  YYSYMBOL_PERCENT_TOKEN_TABLE = 38,       /* "%token-table"  */
  YYSYMBOL_PERCENT_VERBOSE = 39,           /* "%verbose"  */
  YYSYMBOL_PERCENT_YACC = 40,              /* "%yacc"  */
  YYSYMBOL_BRACED_CODE = 41,               /* "{...}"  */
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
  YYSYMBOL_TAG = 54,                       /* "<tag>"  */
  YYSYMBOL_TAG_ANY = 55,                   /* "<*>"  */
  YYSYMBOL_TAG_NONE = 56,                  /* "<>"  */
  YYSYMBOL_INT_LITERAL = 57,               /* "integer literal"  */
  YYSYMBOL_PERCENT_PARAM = 58,             /* "%param"  */
  YYSYMBOL_PERCENT_UNION = 59,             /* "%union"  */
  YYSYMBOL_PERCENT_EMPTY = 60,             /* "%empty"  */
  YYSYMBOL_YYACCEPT = 61,                  /* $accept  */
  YYSYMBOL_ಒಳಹರಿವು = 62,                     /*ಒಳಹರಿವು*/
  YYSYMBOL_prologue_declarations = 63,     /* prologue_declarations  */
  YYSYMBOL_prologue_declaration = 64,      /* prologue_declaration  */
  YYSYMBOL_65_1 = 65,                      /* $@1  */
  YYSYMBOL_params = 66,                    /* params  */
  YYSYMBOL_grammar_declaration = 67,       /* grammar_declaration  */
  YYSYMBOL_code_props_type = 68,           /* code_props_type  */
  YYSYMBOL_union_name = 69,                /* union_name  */
  YYSYMBOL_ಚಿಹ್ನೆ_declaration = 70,        /* symbol_declaration  */
  YYSYMBOL_71_2 = 71,                      /* $@2  */
  YYSYMBOL_72_3 = 72,                      /* $@3  */
  YYSYMBOL_73_4 = 73,                      /* $@4  */
  YYSYMBOL_precedence_declarator = 74,     /* precedence_declarator  */
  YYSYMBOL_75_string_opt = 75,             /* string.opt  */
  YYSYMBOL_76_tag_opt = 76,                /* tag.opt  */
  YYSYMBOL_generic_symlist = 77,           /* generic_symlist  */
  YYSYMBOL_generic_symlist_item = 78,      /* generic_symlist_item  */
  YYSYMBOL_tag = 79,                       /* tag  */
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
  YYSYMBOL_rules_or_grammar_declaration = 92, /* rules_or_grammar_declaration  */
  YYSYMBOL_rules = 93,                     /* rules  */
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
ಪ್ರಕಾರನಿರ್ದೇಶ ಗಣನಾ yysymbol_kind_t yysymbol_kind_t;



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
  ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ* api_version = "3.8";

  ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ current_prec = 0;
  ಸ್ಥಿರ location current_lhs_loc;
  ಸ್ಥಿರ named_ref *current_lhs_named_ref;
  ಸ್ಥಿರ ಚಿಹ್ನೆ *current_lhs_ಚಿಹ್ನೆ;
  ಸ್ಥಿರ ಚಿಹ್ನೆ_class current_class = unknown_sym;

  /** Set the new current left-hand side symbol, possibly common
   * to several right-hand side parts of rule.
   */
  ಸ್ಥಿರ ಶೂನ್ಯ current_lhs (ಚಿಹ್ನೆ *sym, location loc, named_ref *ref);

  #define YYLLOC_DEFAULT(Current, Rhs, N)         \
    (Current) = lloc_default (Rhs, N)
  ಸ್ಥಿರ YYLTYPE lloc_default (YYLTYPE ಧ್ರುವ *, ಪೂರ್ಣಾಙ್ಕ);

  #define YY_LOCATION_PRINT(File, Loc)            \
    location_print (Loc, File)

  /* Strip initial '{' and final '}' (must be first and last characters).
     Return the result.  */
  ಸ್ಥಿರ ವರ್ಣ *strip_braces (ವರ್ಣ *code);

  /* Convert CODE by calling code_props_plain_init if PLAIN, otherwise
     code_props_symbol_action_init.  Calls
     gram_scanner_last_string_free to release the latest string from
     the scanner (should be CODE). */
  ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *translate_code (ವರ್ಣ *code, location loc, bool plain);

  /* Convert CODE by calling code_props_plain_init after having
     stripped the first and last characters (expected to be '{', and
     '}').  Calls gram_scanner_last_string_free to release the latest
     string from the scanner (should be CODE). */
  ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *translate_code_braceless (ವರ್ಣ *code, location loc);

  /* Handle a %header directive.  */
  ಸ್ಥಿರ ಶೂನ್ಯ handle_header (ವರ್ಣ ಧ್ರುವ *ಮೌಲ್ಯ);

  /* Handle a %error-verbose directive.  */
  ಸ್ಥಿರ ಶೂನ್ಯ handle_error_verbose (location ಧ್ರುವ *loc, ವರ್ಣ ಧ್ರುವ *directive);

  /* Handle a %file-prefix directive.  */
  ಸ್ಥಿರ ಶೂನ್ಯ handle_file_prefix (location ಧ್ರುವ *loc,
                                  location ಧ್ರುವ *dir_loc,
                                  ವರ್ಣ ಧ್ರುವ *directive, ವರ್ಣ ಧ್ರುವ *ಮೌಲ್ಯ);

  /* Handle a %language directive.  */
  ಸ್ಥಿರ ಶೂನ್ಯ handle_language (location ಧ್ರುವ *loc, ವರ್ಣ ಧ್ರುವ *lang);

  /* Handle a %name-prefix directive.  */
  ಸ್ಥಿರ ಶೂನ್ಯ handle_name_prefix (location ಧ್ರುವ *loc,
                                  ವರ್ಣ ಧ್ರುವ *directive, ವರ್ಣ ಧ್ರುವ *ಮೌಲ್ಯ);

  /* Handle a %pure-parser directive.  */
  ಸ್ಥಿರ ಶೂನ್ಯ handle_pure_parser (location ಧ್ರುವ *loc, ವರ್ಣ ಧ್ರುವ *directive);

  /* Handle a %require directive.  */
  ಸ್ಥಿರ ಶೂನ್ಯ handle_require (location ಧ್ರುವ *loc, ವರ್ಣ ಧ್ರುವ *version);

  /* Handle a %skeleton directive.  */
  ಸ್ಥಿರ ಶೂನ್ಯ handle_skeleton (location ಧ್ರುವ *loc, ವರ್ಣ ಧ್ರುವ *skel);

  /* Handle a %yacc directive.  */
  ಸ್ಥಿರ ಶೂನ್ಯ handle_yacc (location ಧ್ರುವ *loc);

  /* Implementation of yyerror.  */
  ಸ್ಥಿರ ಶೂನ್ಯ gram_error (location ಧ್ರುವ *, ವರ್ಣ ಧ್ರುವ *);

  /* A string that describes a char (e.g., 'a' -> "'a'").  */
  ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *char_name (ವರ್ಣ);

  /* Add style to semantic values in traces.  */
  ಸ್ಥಿರ ಶೂನ್ಯ tron (FILE *yyo);
  ಸ್ಥಿರ ಶೂನ್ಯ troff (FILE *yyo);

  /* Interpret a quoted string (such as `"Hello, \"World\"\n\""`).
     Manages the memory of the result.  */
  ಸ್ಥಿರ ವರ್ಣ *unquote (ಧ್ರುವ ವರ್ಣ *str);

  /* Discard the latest unquoted string.  */
  ಸ್ಥಿರ ಶೂನ್ಯ unquote_free (ವರ್ಣ *last_string);

  /** Add a lex-param and/or a parse-param.
   *
   * \param type  where to push this formal argument.
   * \param decl  the formal argument.  Destroyed.
   * \param loc   the location in the source.
   */
  ಸ್ಥಿರ ಶೂನ್ಯ add_param (param_type type, ವರ್ಣ *decl, location loc);
  ಸ್ಥಿರ param_type current_param = param_none;


#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# ಯದಿ defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
ಪ್ರಕಾರನಿರ್ದೇಶ __INT_LEAST8_TYPE__ yytype_int8;
#ನೋಚೇದ್ಯದಿ defined YY_STDINT_H
ಪ್ರಕಾರನಿರ್ದೇಶ int_least8_t yytype_int8;
#ಅನ್ಯಥಾ
ಪ್ರಕಾರನಿರ್ದೇಶ signed ವರ್ಣ yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
ಪ್ರಕಾರನಿರ್ದೇಶ __INT_LEAST16_TYPE__ yytype_int16;
#ನೋಚೇದ್ಯದಿ defined YY_STDINT_H
ಪ್ರಕಾರನಿರ್ದೇಶ int_least16_t yytype_int16;
#ಅನ್ಯಥಾ
ಪ್ರಕಾರನಿರ್ದೇಶ short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#ಯದಿ defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
ಪ್ರಕಾರನಿರ್ದೇಶ __UINT_LEAST8_TYPE__ yytype_uint8;
#ನೋಚೇದ್ಯದಿ (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
ಪ್ರಕಾರನಿರ್ದೇಶ uint_least8_t yytype_uint8;
#ನೋಚೇದ್ಯದಿ !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
ಪ್ರಕಾರನಿರ್ದೇಶ unsigned ವರ್ಣ yytype_uint8;
#ಅನ್ಯಥಾ
ಪ್ರಕಾರನಿರ್ದೇಶ short yytype_uint8;
#endif

#ಯದಿ defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
ಪ್ರಕಾರನಿರ್ದೇಶ __UINT_LEAST16_TYPE__ yytype_uint16;
#ನೋಚೇದ್ಯದಿ (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
ಪ್ರಕಾರನಿರ್ದೇಶ uint_least16_t yytype_uint16;
#ನೋಚೇದ್ಯದಿ !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
ಪ್ರಕಾರನಿರ್ದೇಶ unsigned short yytype_uint16;
#ಅನ್ಯಥಾ
ಪ್ರಕಾರನಿರ್ದೇಶ ಪೂರ್ಣಾಙ್ಕ yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# ಯದಿ defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# ಅನ್ಯಥಾ
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# ಅನ್ಯಥಾ
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, ಪರಿಮಾಣ (X))


/* Stored state numbers (used for stacks). */
ಪ್ರಕಾರನಿರ್ದೇಶ yytype_uint8 yy_state_t;

/* State numbers in computations.  */
ಪ್ರಕಾರನಿರ್ದೇಶ ಪೂರ್ಣಾಙ್ಕ yy_state_fast_t;

#ifndef YY_
# ಯದಿ defined YYENABLE_NLS && YYENABLE_NLS
#  ಯದಿ ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef N_
# define N_(Msgid) Msgid
#endif


#ifndef YY_ATTRIBUTE_PURE
# ಯದಿ defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# ಅನ್ಯಥಾ
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# ಯದಿ defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# ಅನ್ಯಥಾ
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#ಯದಿ ! defined lint || defined __GNUC__
# define YY_USE(E) ((ಶೂನ್ಯ) (E))
#ಅನ್ಯಥಾ
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#ಯದಿ defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# ಯದಿ __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# ಅನ್ಯಥಾ
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#ಅನ್ಯಥಾ
# define YY_INITIAL_ಮೌಲ್ಯ(ಮೌಲ್ಯ_4) ಮೌಲ್ಯ_4
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_ಮೌಲ್ಯ
# define YY_INITIAL_ಮೌಲ್ಯ(ಮೌಲ್ಯ_4) /* Nothing. */
#endif

#ಯದಿ defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((ಶೂನ್ಯ) (0 && (E)))

#ಯದಿ 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) ಕುರು { /* empty */; } ಯಾವತ್ (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# ಅನ್ಯಥಾ
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  ಯದಿ (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined ಮುಕ್ತ)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   ಯದಿ ! defined malloc && ! defined EXIT_SUCCESS
ಶೂನ್ಯ *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE ಮುಕ್ತ
#   ಯದಿ ! defined ಮುಕ್ತ && ! defined EXIT_SUCCESS
ಶೂನ್ಯ ಮುಕ್ತ (ಶೂನ್ಯ *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
# define YYCOPY_NEEDED 1
#endif /* 1 */

#ಯದಿ (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined GRAM_LTYPE_IS_TRIVIAL && GRAM_LTYPE_IS_TRIVIAL \
             && defined GRAM_STYPE_IS_TRIVIAL && GRAM_STYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
ಸಙ್ಘ yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (ಸಙ್ಘ yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(ರಾಶಿ_alloc, ರಾಶಿ)                           \
    ಕುರು                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->ರಾಶಿ_alloc, ರಾಶಿ, yysize);                    \
        ರಾಶಿ = &yyptr->ರಾಶಿ_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*ರಾಶಿ) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    ಯಾವತ್ (0)

#endif

#ಯದಿ defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  ಯದಿ defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, ಎಣಿಕೆ_2) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (ಎಣಿಕೆ_2)) * ಪರಿಮಾಣ (*(Src)))
#  ಅನ್ಯಥಾ
#   define YYCOPY(Dst, Src, ಎಣಿಕೆ_2)              \
      ಕುರು                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          ಪ್ರತಿ (yyi = 0; yyi < (ಎಣಿಕೆ_2); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      ಯಾವತ್ (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   240

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  61
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  44
/* YYNRULES -- Number of rules.  */
#define YYNRULES  125
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  169

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   315


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX) YY_CAST (yysymbol_kind_t, YYX)

#ಯದಿ GRAM_DEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
ಸ್ಥಿರ ಧ್ರುವ yytype_int16 yyrline[] =
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
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_ಚಿಹ್ನೆ(State) YY_CAST (yysymbol_kind_t, yystos[State])

#ಯದಿ 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ yy_sname[] =
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
  ಪ್ರತ್ಯಾಗಮ (yysymbol < YYNTOKENS && yytranslatable[yysymbol]
          ? _(yy_sname[yysymbol])
          : yy_sname[yysymbol]);
}
#endif

#define YYPACT_NINF (-148)

#define yypact_ಮೌಲ್ಯ_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-125)

#define yytable_ಮೌಲ್ಯ_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
ಸ್ಥಿರ ಧ್ರುವ yytype_int16 yypact[] =
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
ಸ್ಥಿರ ಧ್ರುವ yytype_int8 yydefact[] =
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
ಸ್ಥಿರ ಧ್ರುವ yytype_int16 yypgoto[] =
{
    -148,  -148,  -148,  -148,  -148,  -148,   109,  -148,  -148,  -148,
    -148,  -148,  -148,  -148,  -148,  -148,  -148,    -7,  -148,  -148,
      66,   -73,   -87,    60,  -148,  -148,   -71,   -83,  -148,   -50,
    -148,    51,  -148,  -148,  -148,   -19,  -147,  -148,  -148,   -47,
    -148,   -48,   -38,  -148
};

/* YYDEFGOTO[NTERM-NUM].  */
ಸ್ಥಿರ ಧ್ರುವ yytype_uint8 yydefgoto[] =
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
ಸ್ಥಿರ ಧ್ರುವ yytype_int16 yytable[] =
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

ಸ್ಥಿರ ಧ್ರುವ yytype_int16 yycheck[] =
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
ಸ್ಥಿರ ಧ್ರುವ yytype_int8 yystos[] =
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
ಸ್ಥಿರ ಧ್ರುವ yytype_int8 yyr1[] =
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
ಸ್ಥಿರ ಧ್ರುವ yytype_int8 yyr2[] =
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


ಗಣನಾ { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = GRAM_EMPTY)

#define YYACCEPT        ಗಚ್ಛ yyacceptlab
#define YYABORT         ಗಚ್ಛ yyabortlab
#define YYERROR         ಗಚ್ಛ yyerrorlab
#define YYNOMEM         ಗಚ್ಛ yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, ಮೌಲ್ಯ_4)                                    \
  ಕುರು                                                              \
    ಯದಿ (yychar == GRAM_EMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (ಮೌಲ್ಯ_4);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        YY_LAC_DISCARD ("YYBACKUP");                              \
        ಗಚ್ಛ yybackup;                                            \
      }                                                           \
    ಅನ್ಯಥಾ                                                          \
      {                                                           \
        yyerror (&yylloc, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  ಯಾವತ್ (0)

/* Backward compatibility with an undocumented macro.
   Use GRAM_error or GRAM_UNDEF. */
#define YYERRCODE GRAM_UNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    ಕುರು                                                                  \
      ಯದಿ (N)                                                            \
        {                                                               \
          (Current).first_ಸಾಲು   = YYRHSLOC (Rhs, 1).first_ಸಾಲು;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_ಸಾಲು    = YYRHSLOC (Rhs, N).last_ಸಾಲು;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      ಅನ್ಯಥಾ                                                              \
        {                                                               \
          (Current).first_ಸಾಲು   = (Current).last_ಸಾಲು   =              \
            YYRHSLOC (Rhs, 0).last_ಸಾಲು;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    ಯಾವತ್ (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#ಯದಿ GRAM_DEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
ಕುರು {                                            \
  ಯದಿ (yydebug)                                  \
    YYFPRINTF Args;                             \
} ಯಾವತ್ (0)


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  ಯದಿ defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined GRAM_LTYPE_IS_TRIVIAL && GRAM_LTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
yy_location_print_ (FILE *yyo, YYLTYPE ಧ್ರುವ * ಧ್ರುವ yylocp)
{
  ಪೂರ್ಣಾಙ್ಕ res = 0;
  ಪೂರ್ಣಾಙ್ಕ end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  ಯದಿ (0 <= yylocp->first_ಸಾಲು)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_ಸಾಲು);
      ಯದಿ (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  ಯದಿ (0 <= yylocp->last_ಸಾಲು)
    {
      ಯದಿ (yylocp->first_ಸಾಲು < yylocp->last_ಸಾಲು)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_ಸಾಲು);
          ಯದಿ (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      ಅನ್ಯಥಾ ಯದಿ (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  ಪ್ರತ್ಯಾಗಮ res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  ಅನ್ಯಥಾ

#   define YYLOCATION_PRINT(File, Loc) ((ಶೂನ್ಯ) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_ಚಿಹ್ನೆ_PRINT(Title, Kind, ಮೌಲ್ಯ_4, Location)                    \
ಕುರು {                                                                      \
  ಯದಿ (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_ಚಿಹ್ನೆ_print (stderr,                                            \
                  Kind, ಮೌಲ್ಯ_4, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} ಯಾವತ್ (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
yy_ಚಿಹ್ನೆ_ಮೌಲ್ಯ_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE ಧ್ರುವ * ಧ್ರುವ yyvaluep, YYLTYPE ಧ್ರುವ * ಧ್ರುವ yylocationp)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  ಯದಿ (!yyvaluep)
    ಪ್ರತ್ಯಾಗಮ;
/* "%code pre-printer" blocks.  */
tron (yyo);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  ವಿಕಲ್ಪ (yykind)
    {
    ಪಕ್ಷ YYSYMBOL_STRING: /* "string"  */
         { fputs (((*yyvaluep).STRING), yyo); }
        ಭಙ್ಗ;

    ಪಕ್ಷ YYSYMBOL_TSTRING: /* "translatable string"  */
         { fputs (((*yyvaluep).TSTRING), yyo); }
        ಭಙ್ಗ;

    ಪಕ್ಷ YYSYMBOL_PERCENT_ERROR_VERBOSE: /* "%error-verbose"  */
         { fputs (((*yyvaluep).PERCENT_ERROR_VERBOSE), yyo); }
        ಭಙ್ಗ;

    ಪಕ್ಷ YYSYMBOL_PERCENT_FILE_PREFIX: /* "%file-prefix"  */
         { fputs (((*yyvaluep).PERCENT_FILE_PREFIX), yyo); }
        ಭಙ್ಗ;

    ಪಕ್ಷ YYSYMBOL_PERCENT_FLAG: /* "%<flag>"  */
         { fprintf (yyo, "%%%s", ((*yyvaluep).PERCENT_FLAG)); }
        ಭಙ್ಗ;

    ಪಕ್ಷ YYSYMBOL_PERCENT_NAME_PREFIX: /* "%name-prefix"  */
         { fputs (((*yyvaluep).PERCENT_NAME_PREFIX), yyo); }
        ಭಙ್ಗ;

    ಪಕ್ಷ YYSYMBOL_PERCENT_PURE_PARSER: /* "%pure-parser"  */
         { fputs (((*yyvaluep).PERCENT_PURE_PARSER), yyo); }
        ಭಙ್ಗ;

    ಪಕ್ಷ YYSYMBOL_BRACED_CODE: /* "{...}"  */
         { fputs (((*yyvaluep).BRACED_CODE), yyo); }
        ಭಙ್ಗ;

    ಪಕ್ಷ YYSYMBOL_BRACED_PREDICATE: /* "%?{...}"  */
         { fputs (((*yyvaluep).BRACED_PREDICATE), yyo); }
        ಭಙ್ಗ;

    ಪಕ್ಷ YYSYMBOL_BRACKETED_ID: /* "[identifier]"  */
         { fprintf (yyo, "[%s]", ((*yyvaluep).BRACKETED_ID)); }
        ಭಙ್ಗ;

    ಪಕ್ಷ YYSYMBOL_CHAR_LITERAL: /* "character literal"  */
         { fputs (char_name (((*yyvaluep).CHAR_LITERAL)), yyo); }
        ಭಙ್ಗ;

    ಪಕ್ಷ YYSYMBOL_EPILOGUE: /* "epilogue"  */
         { fputs (((*yyvaluep).EPILOGUE), yyo); }
        ಭಙ್ಗ;

    ಪಕ್ಷ YYSYMBOL_ID: /* "identifier"  */
         { fputs (((*yyvaluep).ID), yyo); }
        ಭಙ್ಗ;

    ಪಕ್ಷ YYSYMBOL_ID_COLON: /* "identifier:"  */
         { fprintf (yyo, "%s:", ((*yyvaluep).ID_COLON)); }
        ಭಙ್ಗ;

    ಪಕ್ಷ YYSYMBOL_PROLOGUE: /* "%{...%}"  */
         { fputs (((*yyvaluep).PROLOGUE), yyo); }
        ಭಙ್ಗ;

    ಪಕ್ಷ YYSYMBOL_TAG: /* "<tag>"  */
         { fprintf (yyo, "<%s>", ((*yyvaluep).TAG)); }
        ಭಙ್ಗ;

    ಪಕ್ಷ YYSYMBOL_INT_LITERAL: /* "integer literal"  */
         { fprintf (yyo, "%d", ((*yyvaluep).INT_LITERAL)); }
        ಭಙ್ಗ;

    ಪಕ್ಷ YYSYMBOL_PERCENT_PARAM: /* "%param"  */
{
  ವಿಕಲ್ಪ (((*yyvaluep).PERCENT_PARAM))
    {
#define CASE(In, Out)                                           \
      ಪಕ್ಷ param_ ## In: fputs ("%" #Out, yyo); ಭಙ್ಗ
      CASE (lex,   lex-param);
      CASE (parse, parse-param);
      CASE (both,  param);
#undef CASE
      ಪಕ್ಷ param_none: aver (false); ಭಙ್ಗ;
    }
}
        ಭಙ್ಗ;

    ಪಕ್ಷ YYSYMBOL_code_props_type: /* code_props_type  */
         { fprintf (yyo, "%s", code_props_type_string (((*yyvaluep).code_props_type))); }
        ಭಙ್ಗ;

    ಪಕ್ಷ YYSYMBOL_75_string_opt: /* string.opt  */
         { fputs (((*yyvaluep).yykind_75), yyo); }
        ಭಙ್ಗ;

    ಪಕ್ಷ YYSYMBOL_76_tag_opt: /* tag.opt  */
         { fputs (((*yyvaluep).yykind_76), yyo); }
        ಭಙ್ಗ;

    ಪಕ್ಷ YYSYMBOL_generic_symlist: /* generic_symlist  */
         { ಚಿಹ್ನೆ_list_syms_print (((*yyvaluep).generic_symlist), yyo); }
        ಭಙ್ಗ;

    ಪಕ್ಷ YYSYMBOL_generic_symlist_item: /* generic_symlist_item  */
         { ಚಿಹ್ನೆ_list_syms_print (((*yyvaluep).generic_symlist_item), yyo); }
        ಭಙ್ಗ;

    ಪಕ್ಷ YYSYMBOL_tag: /* tag  */
         { fprintf (yyo, "<%s>", ((*yyvaluep).tag)); }
        ಭಙ್ಗ;

    ಪಕ್ಷ YYSYMBOL_nterm_decls: /* nterm_decls  */
         { ಚಿಹ್ನೆ_list_syms_print (((*yyvaluep).nterm_decls), yyo); }
        ಭಙ್ಗ;

    ಪಕ್ಷ YYSYMBOL_token_decls: /* token_decls  */
         { ಚಿಹ್ನೆ_list_syms_print (((*yyvaluep).token_decls), yyo); }
        ಭಙ್ಗ;

    ಪಕ್ಷ YYSYMBOL_82_token_decl_1: /* token_decl.1  */
         { ಚಿಹ್ನೆ_list_syms_print (((*yyvaluep).yykind_82), yyo); }
        ಭಙ್ಗ;

    ಪಕ್ಷ YYSYMBOL_token_decl: /* token_decl  */
         { fprintf (yyo, "%s", ((*yyvaluep).token_decl) ? ((*yyvaluep).token_decl)->tag : "<NULL>"); }
        ಭಙ್ಗ;

    ಪಕ್ಷ YYSYMBOL_84_int_opt: /* int.opt  */
         { fprintf (yyo, "%d", ((*yyvaluep).yykind_84)); }
        ಭಙ್ಗ;

    ಪಕ್ಷ YYSYMBOL_alias: /* alias  */
         { fprintf (yyo, "%s", ((*yyvaluep).alias) ? ((*yyvaluep).alias)->tag : "<NULL>"); }
        ಭಙ್ಗ;

    ಪಕ್ಷ YYSYMBOL_token_decls_for_prec: /* token_decls_for_prec  */
         { ಚಿಹ್ನೆ_list_syms_print (((*yyvaluep).token_decls_for_prec), yyo); }
        ಭಙ್ಗ;

    ಪಕ್ಷ YYSYMBOL_87_token_decl_for_prec_1: /* token_decl_for_prec.1  */
         { ಚಿಹ್ನೆ_list_syms_print (((*yyvaluep).yykind_87), yyo); }
        ಭಙ್ಗ;

    ಪಕ್ಷ YYSYMBOL_token_decl_for_prec: /* token_decl_for_prec  */
         { fprintf (yyo, "%s", ((*yyvaluep).token_decl_for_prec) ? ((*yyvaluep).token_decl_for_prec)->tag : "<NULL>"); }
        ಭಙ್ಗ;

    ಪಕ್ಷ YYSYMBOL_ಚಿಹ್ನೆ_decls: /* symbol_decls  */
         { ಚಿಹ್ನೆ_list_syms_print (((*yyvaluep).ಚಿಹ್ನೆ_decls), yyo); }
        ಭಙ್ಗ;

    ಪಕ್ಷ YYSYMBOL_90_symbols_1: /* symbols.1  */
         { ಚಿಹ್ನೆ_list_syms_print (((*yyvaluep).yykind_90), yyo); }
        ಭಙ್ಗ;

    ಪಕ್ಷ YYSYMBOL_variable: /* variable  */
         { fputs (((*yyvaluep).variable), yyo); }
        ಭಙ್ಗ;

    ಪಕ್ಷ YYSYMBOL_ಮೌಲ್ಯ: /*ಮೌಲ್ಯ*/
{
  ವಿಕಲ್ಪ (((*yyvaluep).ಮೌಲ್ಯ).kind)
    {
    ಪಕ್ಷ muscle_code:    fprintf (yyo,  "{%s}",  ((*yyvaluep).ಮೌಲ್ಯ).chars); ಭಙ್ಗ;
    ಪಕ್ಷ muscle_keyword: fprintf (yyo,   "%s",   ((*yyvaluep).ಮೌಲ್ಯ).chars); ಭಙ್ಗ;
    ಪಕ್ಷ muscle_string:  fprintf (yyo, "\"%s\"", ((*yyvaluep).ಮೌಲ್ಯ).chars); ಭಙ್ಗ;
    }
}
        ಭಙ್ಗ;

    ಪಕ್ಷ YYSYMBOL_id: /* id  */
         { fprintf (yyo, "%s", ((*yyvaluep).id) ? ((*yyvaluep).id)->tag : "<NULL>"); }
        ಭಙ್ಗ;

    ಪಕ್ಷ YYSYMBOL_id_colon: /* id_colon  */
         { fprintf (yyo, "%s:", ((*yyvaluep).id_colon)->tag); }
        ಭಙ್ಗ;

    ಪಕ್ಷ YYSYMBOL_ಚಿಹ್ನೆ: /*ಚಿಹ್ನೆ*/
         { fprintf (yyo, "%s", ((*yyvaluep).ಚಿಹ್ನೆ) ? ((*yyvaluep).ಚಿಹ್ನೆ)->tag : "<NULL>"); }
        ಭಙ್ಗ;

    ಪಕ್ಷ YYSYMBOL_string_as_id: /* string_as_id  */
         { fprintf (yyo, "%s", ((*yyvaluep).string_as_id) ? ((*yyvaluep).string_as_id)->tag : "<NULL>"); }
        ಭಙ್ಗ;

      ಸ್ವಭಾವ:
        ಭಙ್ಗ;
    }
  YY_IGNORE_MAYBE_UNINITIALIZED_END
/* "%code post-printer" blocks.  */
troff (yyo);

}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
yy_ಚಿಹ್ನೆ_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE ಧ್ರುವ * ಧ್ರುವ yyvaluep, YYLTYPE ಧ್ರುವ * ಧ್ರುವ yylocationp)
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
  ಪ್ರತಿ (; yybottom <= yytop; yybottom++)
    {
      ಪೂರ್ಣಾಙ್ಕ yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_ರಾಶಿ_PRINT(Bottom, ತುದಿ)                            \
ಕುರು {                                                            \
  ಯದಿ (yydebug)                                                  \
    yy_ರಾಶಿ_print ((Bottom), (ತುದಿ));                           \
} ಯಾವತ್ (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 ಪೂರ್ಣಾಙ್ಕ yyrule)
{
  ಪೂರ್ಣಾಙ್ಕ yylno = yyrline[yyrule];
  ಪೂರ್ಣಾಙ್ಕ yynrhs = yyr2[yyrule];
  ಪೂರ್ಣಾಙ್ಕ yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  ಪ್ರತಿ (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_ಚಿಹ್ನೆ_print (stderr,
                       YY_ACCESSING_ಚಿಹ್ನೆ (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]));
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
ಕುರು {                                    \
  ಯದಿ (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
} ಯಾವತ್ (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
ಪೂರ್ಣಾಙ್ಕ yydebug;
#ಅನ್ಯಥಾ /* !GRAM_DEBUG */
# define YYDPRINTF(Args) ((ಶೂನ್ಯ) 0)
# define YY_ಚಿಹ್ನೆ_PRINT(Title, Kind, ಮೌಲ್ಯ_4, Location)
# define YY_ರಾಶಿ_PRINT(Bottom, ತುದಿ)
# define YY_REDUCE_PRINT(Rule)
#endif /* !GRAM_DEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


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
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
yy_lac_ರಾಶಿ_realloc (YYPTRDIFF_T *yycapacity, YYPTRDIFF_T yyadd,
#ಯದಿ GRAM_DEBUG
                      ವರ್ಣ ಧ್ರುವ *yydebug_prefix,
                      ವರ್ಣ ಧ್ರುವ *yydebug_suffix,
#endif
                      yy_state_t **yybottom,
                      yy_state_t *yybottom_no_free,
                      yy_state_t **yytop, yy_state_t *yytop_empty)
{
  YYPTRDIFF_T yysize_old =
    *yytop == yytop_empty ? 0 : *yytop - *yybottom + 1;
  YYPTRDIFF_T yysize_new = yysize_old + yyadd;
  ಯದಿ (*yycapacity < yysize_new)
    {
      YYPTRDIFF_T yyalloc = 2 * yysize_new;
      yy_state_t *yybottom_new;
      /* Use YYMAXDEPTH for maximum stack size given that the stack
         should never need to grow larger than the main state stack
         needs to grow without LAC.  */
      ಯದಿ (YYMAXDEPTH < yysize_new)
        {
          YYDPRINTF ((stderr, "%smax size exceeded%s", yydebug_prefix,
                      yydebug_suffix));
          ಪ್ರತ್ಯಾಗಮ YYENOMEM;
        }
      ಯದಿ (YYMAXDEPTH < yyalloc)
        yyalloc = YYMAXDEPTH;
      yybottom_new =
        YY_CAST (yy_state_t *,
                 YYSTACK_ALLOC (YY_CAST (YYSIZE_T,
                                         yyalloc * YYSIZEOF (*yybottom_new))));
      ಯದಿ (!yybottom_new)
        {
          YYDPRINTF ((stderr, "%srealloc failed%s", yydebug_prefix,
                      yydebug_suffix));
          ಪ್ರತ್ಯಾಗಮ YYENOMEM;
        }
      ಯದಿ (*yytop != yytop_empty)
        {
          YYCOPY (yybottom_new, *yybottom, yysize_old);
          *yytop = yybottom_new + (yysize_old - 1);
        }
      ಯದಿ (*yybottom != yybottom_no_free)
        YYSTACK_FREE (*yybottom);
      *yybottom = yybottom_new;
      *yycapacity = yyalloc;
    }
  ಪ್ರತ್ಯಾಗಮ 0;
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
#define YY_LAC_ESTABLISH                                                \
ಕುರು {                                                                    \
  ಯದಿ (!yy_lac_established)                                              \
    {                                                                   \
      YYDPRINTF ((stderr,                                               \
                  "LAC: initial context established for %s\n",          \
                  yysymbol_name (yytoken)));                            \
      yy_lac_established = 1;                                           \
      ವಿಕಲ್ಪ (yy_lac (yyesa, &yyes, &yyes_capacity, yyssp, yytoken))    \
        {                                                               \
        ಪಕ್ಷ YYENOMEM:                                                  \
          YYNOMEM;                                                      \
        ಪಕ್ಷ 1:                                                         \
          ಗಚ್ಛ yyerrlab;                                                \
        }                                                               \
    }                                                                   \
} ಯಾವತ್ (0)

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
#ಯದಿ GRAM_DEBUG
# define YY_LAC_DISCARD(Event)                                           \
ಕುರು {                                                                     \
  ಯದಿ (yy_lac_established)                                                \
    {                                                                    \
      YYDPRINTF ((stderr, "LAC: initial context discarded due to "       \
                  Event "\n"));                                          \
      yy_lac_established = 0;                                            \
    }                                                                    \
} ಯಾವತ್ (0)
#ಅನ್ಯಥಾ
# define YY_LAC_DISCARD(Event) yy_lac_established = 0
#endif

/* Given the stack whose top is *YYSSP, return 0 iff YYTOKEN can
   eventually (after perhaps some reductions) be shifted, return 1 if
   not, or return YYENOMEM if memory is exhausted.  As preconditions and
   postconditions: *YYES_CAPACITY is the allocated size of the array to
   which *YYES points, and either *YYES = YYESA or *YYES points to an
   array allocated with YYSTACK_ALLOC.  yy_lac may overwrite the
   contents of either array, alter *YYES and *YYES_CAPACITY, and free
   any old *YYES other than YYESA.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
yy_lac (yy_state_t *yyesa, yy_state_t **yyes,
        YYPTRDIFF_T *yyes_capacity, yy_state_t *yyssp, yysymbol_kind_t yytoken)
{
  yy_state_t *yyes_prev = yyssp;
  yy_state_t *yyesp = yyes_prev;
  /* Reduce until we encounter a shift and thereby accept the token.  */
  YYDPRINTF ((stderr, "LAC: checking lookahead %s:", yysymbol_name (yytoken)));
  ಯದಿ (yytoken == YYSYMBOL_YYUNDEF)
    {
      YYDPRINTF ((stderr, " Always Err\n"));
      ಪ್ರತ್ಯಾಗಮ 1;
    }
  ಯಾವತ್ (1)
    {
      ಪೂರ್ಣಾಙ್ಕ yyrule = yypact[+*yyesp];
      ಯದಿ (yypact_ಮೌಲ್ಯ_is_default (yyrule)
          || (yyrule += yytoken) < 0 || YYLAST < yyrule
          || yycheck[yyrule] != yytoken)
        {
          /* Use the default action.  */
          yyrule = yydefact[+*yyesp];
          ಯದಿ (yyrule == 0)
            {
              YYDPRINTF ((stderr, " Err\n"));
              ಪ್ರತ್ಯಾಗಮ 1;
            }
        }
      ಅನ್ಯಥಾ
        {
          /* Use the action from yytable.  */
          yyrule = yytable[yyrule];
          ಯದಿ (yytable_ಮೌಲ್ಯ_is_error (yyrule))
            {
              YYDPRINTF ((stderr, " Err\n"));
              ಪ್ರತ್ಯಾಗಮ 1;
            }
          ಯದಿ (0 < yyrule)
            {
              YYDPRINTF ((stderr, " S%d\n", yyrule));
              ಪ್ರತ್ಯಾಗಮ 0;
            }
          yyrule = -yyrule;
        }
      /* By now we know we have to simulate a reduce.  */
      YYDPRINTF ((stderr, " R%d", yyrule - 1));
      {
        /* Pop the corresponding number of values from the stack.  */
        YYPTRDIFF_T yylen = yyr2[yyrule];
        /* First pop from the LAC stack as many tokens as possible.  */
        ಯದಿ (yyesp != yyes_prev)
          {
            YYPTRDIFF_T yysize = yyesp - *yyes + 1;
            ಯದಿ (yylen < yysize)
              {
                yyesp -= yylen;
                yylen = 0;
              }
            ಅನ್ಯಥಾ
              {
                yyesp = yyes_prev;
                yylen -= yysize;
              }
          }
        /* Only afterwards look at the main stack.  */
        ಯದಿ (yylen)
          yyesp = yyes_prev -= yylen;
      }
      /* Push the resulting state of the reduction.  */
      {
        yy_state_fast_t yystate;
        {
          ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ yylhs = yyr1[yyrule] - YYNTOKENS;
          ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ yyi = yypgoto[yylhs] + *yyesp;
          yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyesp
                     ? yytable[yyi]
                     : yydefgoto[yylhs]);
        }
        ಯದಿ (yyesp == yyes_prev)
          {
            yyesp = *yyes;
            YY_IGNORE_USELESS_CAST_BEGIN
            *yyesp = YY_CAST (yy_state_t, yystate);
            YY_IGNORE_USELESS_CAST_END
          }
        ಅನ್ಯಥಾ
          {
            ಯದಿ (yy_lac_ರಾಶಿ_realloc (yyes_capacity, 1,
#ಯದಿ GRAM_DEBUG
                                      " (", ")",
#endif
                                      yyes, yyesa, &yyesp, yyes_prev))
              {
                YYDPRINTF ((stderr, "\n"));
                ಪ್ರತ್ಯಾಗಮ YYENOMEM;
              }
            YY_IGNORE_USELESS_CAST_BEGIN
            *++yyesp = YY_CAST (yy_state_t, yystate);
            YY_IGNORE_USELESS_CAST_END
          }
        YYDPRINTF ((stderr, " G%d", yystate));
      }
    }
}

/* Context of a parse error.  */
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ
{
  yy_state_t *yyssp;
  yy_state_t *yyesa;
  yy_state_t **yyes;
  YYPTRDIFF_T *yyes_capacity;
  yysymbol_kind_t yytoken;
  YYLTYPE *yylloc;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
yypcontext_expected_tokens (ಧ್ರುವ yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], ಪೂರ್ಣಾಙ್ಕ yyargn)
{
  /* Actual size of YYARG. */
  ಪೂರ್ಣಾಙ್ಕ yycount = 0;

  ಪೂರ್ಣಾಙ್ಕ yyx;
  ಪ್ರತಿ (yyx = 0; yyx < YYNTOKENS; ++yyx)
    {
      yysymbol_kind_t yysym = YY_CAST (yysymbol_kind_t, yyx);
      ಯದಿ (yysym != YYSYMBOL_YYerror && yysym != YYSYMBOL_YYUNDEF)
        ವಿಕಲ್ಪ (yy_lac (yyctx->yyesa, yyctx->yyes, yyctx->yyes_capacity, yyctx->yyssp, yysym))
          {
          ಪಕ್ಷ YYENOMEM:
            ಪ್ರತ್ಯಾಗಮ YYENOMEM;
          ಪಕ್ಷ 1:
            ಅನುವರ್ತ;
          ಸ್ವಭಾವ:
            ಯದಿ (!yyarg)
              ++yycount;
            ಅನ್ಯಥಾ ಯದಿ (yycount == yyargn)
              ಪ್ರತ್ಯಾಗಮ 0;
            ಅನ್ಯಥಾ
              yyarg[yycount++] = yysym;
          }
    }
  ಯದಿ (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  ಪ್ರತ್ಯಾಗಮ yycount;
}




/* The kind of the lookahead of this context.  */
ಸ್ಥಿರ yysymbol_kind_t
yypcontext_token (ಧ್ರುವ yypcontext_t *yyctx) YY_ATTRIBUTE_UNUSED;

ಸ್ಥಿರ yysymbol_kind_t
yypcontext_token (ಧ್ರುವ yypcontext_t *yyctx)
{
  ಪ್ರತ್ಯಾಗಮ yyctx->yytoken;
}

/* The location of the lookahead of this context.  */
ಸ್ಥಿರ YYLTYPE *
yypcontext_location (ಧ್ರುವ yypcontext_t *yyctx) YY_ATTRIBUTE_UNUSED;

ಸ್ಥಿರ YYLTYPE *
yypcontext_location (ಧ್ರುವ yypcontext_t *yyctx)
{
  ಪ್ರತ್ಯಾಗಮ yyctx->yylloc;
}

/* User defined function to report a syntax error.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
yyreport_syntax_error (ಧ್ರುವ yypcontext_t *yyctx);

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
yydestruct (ಧ್ರುವ ವರ್ಣ *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  ಯದಿ (!yymsg)
    yymsg = "Deleting";
  YY_ಚಿಹ್ನೆ_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  ವಿಕಲ್ಪ (yykind)
    {
    ಪಕ್ಷ YYSYMBOL_generic_symlist: /* generic_symlist  */
            { ಚಿಹ್ನೆ_list_free (((*yyvaluep).generic_symlist)); }
        ಭಙ್ಗ;

    ಪಕ್ಷ YYSYMBOL_generic_symlist_item: /* generic_symlist_item  */
            { ಚಿಹ್ನೆ_list_free (((*yyvaluep).generic_symlist_item)); }
        ಭಙ್ಗ;

    ಪಕ್ಷ YYSYMBOL_nterm_decls: /* nterm_decls  */
            { ಚಿಹ್ನೆ_list_free (((*yyvaluep).nterm_decls)); }
        ಭಙ್ಗ;

    ಪಕ್ಷ YYSYMBOL_token_decls: /* token_decls  */
            { ಚಿಹ್ನೆ_list_free (((*yyvaluep).token_decls)); }
        ಭಙ್ಗ;

    ಪಕ್ಷ YYSYMBOL_82_token_decl_1: /* token_decl.1  */
            { ಚಿಹ್ನೆ_list_free (((*yyvaluep).yykind_82)); }
        ಭಙ್ಗ;

    ಪಕ್ಷ YYSYMBOL_token_decls_for_prec: /* token_decls_for_prec  */
            { ಚಿಹ್ನೆ_list_free (((*yyvaluep).token_decls_for_prec)); }
        ಭಙ್ಗ;

    ಪಕ್ಷ YYSYMBOL_87_token_decl_for_prec_1: /* token_decl_for_prec.1  */
            { ಚಿಹ್ನೆ_list_free (((*yyvaluep).yykind_87)); }
        ಭಙ್ಗ;

    ಪಕ್ಷ YYSYMBOL_ಚಿಹ್ನೆ_decls: /* symbol_decls  */
            { ಚಿಹ್ನೆ_list_free (((*yyvaluep).ಚಿಹ್ನೆ_decls)); }
        ಭಙ್ಗ;

    ಪಕ್ಷ YYSYMBOL_90_symbols_1: /* symbols.1  */
            { ಚಿಹ್ನೆ_list_free (((*yyvaluep).yykind_90)); }
        ಭಙ್ಗ;

      ಸ್ವಭಾವ:
        ಭಙ್ಗ;
    }
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}






/*----------.
| yyparse.  |
`----------*/

ಪೂರ್ಣಾಙ್ಕ
yyparse (ಶೂನ್ಯ)
{
/* Lookahead token kind.  */
ಪೂರ್ಣಾಙ್ಕ yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_ಮೌಲ್ಯ (ಸ್ಥಿರ YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_ಮೌಲ್ಯ (= yyval_default);

/* Location data for the lookahead symbol.  */
ಸ್ಥಿರ YYLTYPE yyloc_default
# ಯದಿ defined GRAM_LTYPE_IS_TRIVIAL && GRAM_LTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

    /* Number of syntax errors so far.  */
    ಪೂರ್ಣಾಙ್ಕ yynerrs YY_ATTRIBUTE_UNUSED = 0;

    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    ಪೂರ್ಣಾಙ್ಕ yyerrstatus = 0;

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
    YYPTRDIFF_T yyes_capacity = 20 < YYMAXDEPTH ? 20 : YYMAXDEPTH;

  /* Whether LAC context is established.  A Boolean.  */
  ಪೂರ್ಣಾಙ್ಕ yy_lac_established = 0;
  ಪೂರ್ಣಾಙ್ಕ yyn;
  /* The return value of yyparse.  */
  ಪೂರ್ಣಾಙ್ಕ yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  ಪೂರ್ಣಾಙ್ಕ yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = GRAM_EMPTY; /* Cause a token to be read.  */


/* User initialization code.  */
{
  /* Bison's grammar can initial empty locations, hence a default
     location is needed. */
  boundary_set (&yylloc.start, grammar_file, 1, 1, 1);
  boundary_set (&yylloc.end, grammar_file, 1, 1, 1);
}


  yylsp[0] = yylloc;
  ಗಚ್ಛ yysetstate;


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
  YY_IGNORE_USELESS_CAST_END
  YY_ರಾಶಿ_PRINT (yyss, yyssp);

  ಯದಿ (yyss + yystacksize - 1 <= yyssp)
#ಯದಿ !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#ಅನ್ಯಥಾ
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# ಯದಿ defined yyoverflow
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
# ಅನ್ಯಥಾ /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      ಯದಿ (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      ಯದಿ (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        ಸಙ್ಘ yyalloc *yyptr =
          YY_CAST (ಸಙ್ಘ yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        ಯದಿ (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        ಯದಿ (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      ಯದಿ (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  ಯದಿ (yystate == YYFINAL)
    YYACCEPT;

  ಗಚ್ಛ yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  ಯದಿ (yypact_ಮೌಲ್ಯ_is_default (yyn))
    ಗಚ್ಛ yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  ಯದಿ (yychar == GRAM_EMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex (&yylval, &yylloc);
    }

  ಯದಿ (yychar <= GRAM_EOF)
    {
      yychar = GRAM_EOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  ಅನ್ಯಥಾ ಯದಿ (yychar == GRAM_error)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = GRAM_UNDEF;
      yytoken = YYSYMBOL_YYerror;
      yyerror_range[1] = yylloc;
      ಗಚ್ಛ yyerrlab1;
    }
  ಅನ್ಯಥಾ
    {
      yytoken = YYTRANSLATE (yychar);
      YY_ಚಿಹ್ನೆ_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  ಯದಿ (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    {
      YY_LAC_ESTABLISH;
      ಗಚ್ಛ yydefault;
    }
  yyn = yytable[yyn];
  ಯದಿ (yyn <= 0)
    {
      ಯದಿ (yytable_ಮೌಲ್ಯ_is_error (yyn))
        ಗಚ್ಛ yyerrlab;
      yyn = -yyn;
      YY_LAC_ESTABLISH;
      ಗಚ್ಛ yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  ಯದಿ (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_ಚಿಹ್ನೆ_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = GRAM_EMPTY;
  YY_LAC_DISCARD ("shift");
  ಗಚ್ಛ yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  ಯದಿ (yyn == 0)
    ಗಚ್ಛ yyerrlab;
  ಗಚ್ಛ yyreduce;


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
    ಪೂರ್ಣಾಙ್ಕ yychar_backup = yychar;
    ವಿಕಲ್ಪ (yyn)
      {
  ಪಕ್ಷ 6: /* prologue_declaration: "%{...%}"  */
    {
      muscle_code_grow (union_seen ? "post_prologue" : "pre_prologue",
                        translate_code ((yyvsp[0].PROLOGUE), (yylsp[0]), true), (yylsp[0]));
      code_scanner_last_string_free ();
    }
    ಭಙ್ಗ;

  ಪಕ್ಷ 7: /* prologue_declaration: "%<flag>"  */
    {
      muscle_percent_define_ensure ((yyvsp[0].PERCENT_FLAG), (yylsp[0]), true);
    }
    ಭಙ್ಗ;

  ಪಕ್ಷ 8: /* prologue_declaration: "%define" variable value  */
    {
      muscle_percent_define_ಸೇರಿಸು ((yyvsp[-1].variable), (yyloc), (yyvsp[0].ಮೌಲ್ಯ).kind, (yyvsp[0].ಮೌಲ್ಯ).chars,
                                    MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
    }
    ಭಙ್ಗ;

  ಪಕ್ಷ 9: /* prologue_declaration: "%header" string.opt  */
                                   { handle_header ((yyvsp[0].yykind_75)); }
    ಭಙ್ಗ;

  ಪಕ್ಷ 10: /* prologue_declaration: "%error-verbose"  */
                                   { handle_error_verbose (&(yyloc), (yyvsp[0].PERCENT_ERROR_VERBOSE)); }
    ಭಙ್ಗ;

  ಪಕ್ಷ 11: /* prologue_declaration: "%expect" "integer literal"  */
                                   { expected_sr_conflicts = (yyvsp[0].INT_LITERAL); }
    ಭಙ್ಗ;

  ಪಕ್ಷ 12: /* prologue_declaration: "%expect-rr" "integer literal"  */
                                   { expected_rr_conflicts = (yyvsp[0].INT_LITERAL); }
    ಭಙ್ಗ;

  ಪಕ್ಷ 13: /* prologue_declaration: "%file-prefix" "string"  */
                                   { handle_file_prefix (&(yyloc), &(yylsp[-1]), (yyvsp[-1].PERCENT_FILE_PREFIX), (yyvsp[0].STRING)); }
    ಭಙ್ಗ;

  ಪಕ್ಷ 14: /* prologue_declaration: "%glr-parser"  */
    {
      nondeterministic_parser = true;
      glr_parser = true;
    }
    ಭಙ್ಗ;

  ಪಕ್ಷ 15: /* prologue_declaration: "%initial-action" "{...}"  */
    {
      muscle_code_grow ("initial_action", translate_code ((yyvsp[0].BRACED_CODE), (yylsp[0]), false), (yylsp[0]));
      code_scanner_last_string_free ();
    }
    ಭಙ್ಗ;

  ಪಕ್ಷ 16: /* prologue_declaration: "%language" "string"  */
                                { handle_language (&(yylsp[-1]), (yyvsp[0].STRING)); }
    ಭಙ್ಗ;

  ಪಕ್ಷ 17: /* prologue_declaration: "%name-prefix" "string"  */
                                { handle_name_prefix (&(yyloc), (yyvsp[-1].PERCENT_NAME_PREFIX), (yyvsp[0].STRING)); }
    ಭಙ್ಗ;

  ಪಕ್ಷ 18: /* prologue_declaration: "%no-lines"  */
                                { no_ಸಾಲುಗಳು_flag = true; }
    ಭಙ್ಗ;

  ಪಕ್ಷ 19: /* prologue_declaration: "%nondeterministic-parser"  */
                                { nondeterministic_parser = true; }
    ಭಙ್ಗ;

  ಪಕ್ಷ 20: /* prologue_declaration: "%output" "string"  */
                                { spec_outfile = unquote ((yyvsp[0].STRING)); gram_scanner_last_string_free (); }
    ಭಙ್ಗ;

  ಪಕ್ಷ 21: /* $@1: %empty  */
           { current_param = (yyvsp[0].PERCENT_PARAM); }
    ಭಙ್ಗ;

  ಪಕ್ಷ 22: /* prologue_declaration: "%param" $@1 params  */
                                          { current_param = param_none; }
    ಭಙ್ಗ;

  ಪಕ್ಷ 23: /* prologue_declaration: "%pure-parser"  */
                                { handle_pure_parser (&(yyloc), (yyvsp[0].PERCENT_PURE_PARSER)); }
    ಭಙ್ಗ;

  ಪಕ್ಷ 24: /* prologue_declaration: "%require" "string"  */
                                { handle_require (&(yylsp[0]), (yyvsp[0].STRING)); }
    ಭಙ್ಗ;

  ಪಕ್ಷ 25: /* prologue_declaration: "%skeleton" "string"  */
                                { handle_skeleton (&(yylsp[0]), (yyvsp[0].STRING)); }
    ಭಙ್ಗ;

  ಪಕ್ಷ 26: /* prologue_declaration: "%token-table"  */
                                { token_table_flag = true; }
    ಭಙ್ಗ;

  ಪಕ್ಷ 27: /* prologue_declaration: "%verbose"  */
                                { report_flag |= report_states; }
    ಭಙ್ಗ;

  ಪಕ್ಷ 28: /* prologue_declaration: "%yacc"  */
                                { handle_yacc (&(yyloc)); }
    ಭಙ್ಗ;

  ಪಕ್ಷ 29: /* prologue_declaration: error ";"  */
                                { current_class = unknown_sym; yyerrok; }
    ಭಙ್ಗ;

  ಪಕ್ಷ 31: /* params: params "{...}"  */
                   { add_param (current_param, (yyvsp[0].BRACED_CODE), (yylsp[0])); }
    ಭಙ್ಗ;

  ಪಕ್ಷ 32: /* params: "{...}"  */
                   { add_param (current_param, (yyvsp[0].BRACED_CODE), (yylsp[0])); }
    ಭಙ್ಗ;

  ಪಕ್ಷ 34: /* grammar_declaration: "%start" symbols.1  */
    {
      grammar_start_symbols_add ((yyvsp[0].yykind_90));
    }
    ಭಙ್ಗ;

  ಪಕ್ಷ 35: /* grammar_declaration: code_props_type "{...}" generic_symlist  */
    {
      code_props code;
      code_props_ಚಿಹ್ನೆ_action_init (&code, (yyvsp[-1].BRACED_CODE), (yylsp[-1]));
      code_props_translate_code (&code);
      {
        ಪ್ರತಿ (ಚಿಹ್ನೆ_list *list = (yyvsp[0].generic_symlist); list; list = list->next)
          ಚಿಹ್ನೆ_list_code_props_set (list, (yyvsp[-2].code_props_type), &code);
        ಚಿಹ್ನೆ_list_free ((yyvsp[0].generic_symlist));
      }
    }
    ಭಙ್ಗ;

  ಪಕ್ಷ 36: /* grammar_declaration: "%default-prec"  */
    {
      default_prec = true;
    }
    ಭಙ್ಗ;

  ಪಕ್ಷ 37: /* grammar_declaration: "%no-default-prec"  */
    {
      default_prec = false;
    }
    ಭಙ್ಗ;

  ಪಕ್ಷ 38: /* grammar_declaration: "%code" "{...}"  */
    {
      /* Do not invoke muscle_percent_code_grow here since it invokes
         muscle_user_name_list_grow.  */
      muscle_code_grow ("percent_code()",
                        translate_code_braceless ((yyvsp[0].BRACED_CODE), (yylsp[0])), (yylsp[0]));
      code_scanner_last_string_free ();
    }
    ಭಙ್ಗ;

  ಪಕ್ಷ 39: /* grammar_declaration: "%code" "identifier" "{...}"  */
    {
      muscle_percent_code_grow ((yyvsp[-1].ID), (yylsp[-1]), translate_code_braceless ((yyvsp[0].BRACED_CODE), (yylsp[0])), (yylsp[0]));
      code_scanner_last_string_free ();
    }
    ಭಙ್ಗ;

  ಪಕ್ಷ 40: /* code_props_type: "%destructor"  */
                 { (yyval.code_props_type) = destructor; }
    ಭಙ್ಗ;

  ಪಕ್ಷ 41: /* code_props_type: "%printer"  */
                 { (yyval.code_props_type) = printer; }
    ಭಙ್ಗ;

  ಪಕ್ಷ 42: /* union_name: %empty  */
         {}
    ಭಙ್ಗ;

  ಪಕ್ಷ 43: /* union_name: "identifier"  */
         { muscle_percent_define_ಸೇರಿಸು ("api.value.union.name",
                                         (yylsp[0]), muscle_keyword, (yyvsp[0].ID),
                                         MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE); }
    ಭಙ್ಗ;

  ಪಕ್ಷ 44: /* grammar_declaration: "%union" union_name "{...}"  */
    {
      union_seen = true;
      muscle_code_grow ("union_members", translate_code_braceless ((yyvsp[0].BRACED_CODE), (yylsp[0])), (yylsp[0]));
      code_scanner_last_string_free ();
    }
    ಭಙ್ಗ;

  ಪಕ್ಷ 45: /* $@2: %empty  */
           { current_class = nterm_sym; }
    ಭಙ್ಗ;

  ಪಕ್ಷ 46: /* symbol_declaration: "%nterm" $@2 nterm_decls  */
    {
      current_class = unknown_sym;
      ಚಿಹ್ನೆ_list_free ((yyvsp[0].nterm_decls));
    }
    ಭಙ್ಗ;

  ಪಕ್ಷ 47: /* $@3: %empty  */
           { current_class = token_sym; }
    ಭಙ್ಗ;

  ಪಕ್ಷ 48: /* symbol_declaration: "%token" $@3 token_decls  */
    {
      current_class = unknown_sym;
      ಚಿಹ್ನೆ_list_free ((yyvsp[0].token_decls));
    }
    ಭಙ್ಗ;

  ಪಕ್ಷ 49: /* $@4: %empty  */
          { current_class = pct_type_sym; }
    ಭಙ್ಗ;

  ಪಕ್ಷ 50: /* symbol_declaration: "%type" $@4 symbol_decls  */
    {
      current_class = unknown_sym;
      ಚಿಹ್ನೆ_list_free ((yyvsp[0].ಚಿಹ್ನೆ_decls));
    }
    ಭಙ್ಗ;

  ಪಕ್ಷ 51: /* symbol_declaration: precedence_declarator token_decls_for_prec  */
    {
      ++current_prec;
      ಪ್ರತಿ (ಚಿಹ್ನೆ_list *list = (yyvsp[0].token_decls_for_prec); list; list = list->next)
        ಚಿಹ್ನೆ_precedence_set (list->content.sym, current_prec, (yyvsp[-1].precedence_declarator), (yylsp[-1]));
      ಚಿಹ್ನೆ_list_free ((yyvsp[0].token_decls_for_prec));
    }
    ಭಙ್ಗ;

  ಪಕ್ಷ 52: /* precedence_declarator: "%left"  */
                { (yyval.precedence_declarator) = ಎಡ_assoc; }
    ಭಙ್ಗ;

  ಪಕ್ಷ 53: /* precedence_declarator: "%right"  */
                { (yyval.precedence_declarator) = ಬಲ_assoc; }
    ಭಙ್ಗ;

  ಪಕ್ಷ 54: /* precedence_declarator: "%nonassoc"  */
                { (yyval.precedence_declarator) = non_assoc; }
    ಭಙ್ಗ;

  ಪಕ್ಷ 55: /* precedence_declarator: "%precedence"  */
                { (yyval.precedence_declarator) = precedence_assoc; }
    ಭಙ್ಗ;

  ಪಕ್ಷ 56: /* string.opt: %empty  */
          { (yyval.yykind_75) = ರಿಕ್ತ; }
    ಭಙ್ಗ;

  ಪಕ್ಷ 57: /* string.opt: "string"  */
          { (yyval.yykind_75) = (yyvsp[0].STRING); }
    ಭಙ್ಗ;

  ಪಕ್ಷ 58: /* tag.opt: %empty  */
         { (yyval.yykind_76) = ರಿಕ್ತ; }
    ಭಙ್ಗ;

  ಪಕ್ಷ 59: /* tag.opt: "<tag>"  */
         { (yyval.yykind_76) = (yyvsp[0].TAG); }
    ಭಙ್ಗ;

  ಪಕ್ಷ 61: /* generic_symlist: generic_symlist generic_symlist_item  */
                                         { (yyval.generic_symlist) = ಚಿಹ್ನೆ_list_append ((yyvsp[-1].generic_symlist), (yyvsp[0].generic_symlist_item)); }
    ಭಙ್ಗ;

  ಪಕ್ಷ 62: /* generic_symlist_item: symbol  */
            { (yyval.generic_symlist_item) = ಚಿಹ್ನೆ_list_sym_new ((yyvsp[0].ಚಿಹ್ನೆ), (yylsp[0])); }
    ಭಙ್ಗ;

  ಪಕ್ಷ 63: /* generic_symlist_item: tag  */
            { (yyval.generic_symlist_item) = ಚಿಹ್ನೆ_list_type_new ((yyvsp[0].tag), (yylsp[0])); }
    ಭಙ್ಗ;

  ಪಕ್ಷ 65: /* tag: "<*>"  */
        { (yyval.tag) = uniqstr_new ("*"); }
    ಭಙ್ಗ;

  ಪಕ್ಷ 66: /* tag: "<>"  */
        { (yyval.tag) = uniqstr_new (""); }
    ಭಙ್ಗ;

  ಪಕ್ಷ 68: /* token_decls: token_decl.1  */
    {
      (yyval.token_decls) = (yyvsp[0].yykind_82);
    }
    ಭಙ್ಗ;

  ಪಕ್ಷ 69: /* token_decls: "<tag>" token_decl.1  */
    {
      (yyval.token_decls) = ಚಿಹ್ನೆ_list_type_set ((yyvsp[0].yykind_82), (yyvsp[-1].TAG));
    }
    ಭಙ್ಗ;

  ಪಕ್ಷ 70: /* token_decls: token_decls "<tag>" token_decl.1  */
    {
      (yyval.token_decls) = ಚಿಹ್ನೆ_list_append ((yyvsp[-2].token_decls), ಚಿಹ್ನೆ_list_type_set ((yyvsp[0].yykind_82), (yyvsp[-1].TAG)));
    }
    ಭಙ್ಗ;

  ಪಕ್ಷ 71: /* token_decl.1: token_decl  */
                            { (yyval.yykind_82) = ಚಿಹ್ನೆ_list_sym_new ((yyvsp[0].token_decl), (yylsp[0])); }
    ಭಙ್ಗ;

  ಪಕ್ಷ 72: /* token_decl.1: token_decl.1 token_decl  */
                            { (yyval.yykind_82) = ಚಿಹ್ನೆ_list_append ((yyvsp[-1].yykind_82), ಚಿಹ್ನೆ_list_sym_new ((yyvsp[0].token_decl), (yylsp[0]))); }
    ಭಙ್ಗ;

  ಪಕ್ಷ 73: /* token_decl: id int.opt alias  */
    {
      (yyval.token_decl) = (yyvsp[-2].id);
      ಚಿಹ್ನೆ_class_set ((yyvsp[-2].id), current_class, (yylsp[-2]), true);
      ಯದಿ (0 <= (yyvsp[-1].yykind_84))
        ಚಿಹ್ನೆ_code_set ((yyvsp[-2].id), (yyvsp[-1].yykind_84), (yylsp[-1]));
      ಯದಿ ((yyvsp[0].alias))
        ಚಿಹ್ನೆ_make_alias ((yyvsp[-2].id), (yyvsp[0].alias), (yylsp[0]));
    }
    ಭಙ್ಗ;

  ಪಕ್ಷ 74: /* int.opt: %empty  */
          { (yyval.yykind_84) = -1; }
    ಭಙ್ಗ;

  ಪಕ್ಷ 76: /* alias: %empty  */
                 { (yyval.alias) = ರಿಕ್ತ; }
    ಭಙ್ಗ;

  ಪಕ್ಷ 77: /* alias: string_as_id  */
                 { (yyval.alias) = (yyvsp[0].string_as_id); }
    ಭಙ್ಗ;

  ಪಕ್ಷ 78: /* alias: "translatable string"  */
    {
      (yyval.alias) = ಚಿಹ್ನೆ_get ((yyvsp[0].TSTRING), (yylsp[0]));
      ಚಿಹ್ನೆ_class_set ((yyval.alias), token_sym, (yylsp[0]), false);
      (yyval.alias)->translatable = true;
    }
    ಭಙ್ಗ;

  ಪಕ್ಷ 79: /* token_decls_for_prec: token_decl_for_prec.1  */
    {
      (yyval.token_decls_for_prec) = (yyvsp[0].yykind_87);
    }
    ಭಙ್ಗ;

  ಪಕ್ಷ 80: /* token_decls_for_prec: "<tag>" token_decl_for_prec.1  */
    {
      (yyval.token_decls_for_prec) = ಚಿಹ್ನೆ_list_type_set ((yyvsp[0].yykind_87), (yyvsp[-1].TAG));
    }
    ಭಙ್ಗ;

  ಪಕ್ಷ 81: /* token_decls_for_prec: token_decls_for_prec "<tag>" token_decl_for_prec.1  */
    {
      (yyval.token_decls_for_prec) = ಚಿಹ್ನೆ_list_append ((yyvsp[-2].token_decls_for_prec), ಚಿಹ್ನೆ_list_type_set ((yyvsp[0].yykind_87), (yyvsp[-1].TAG)));
    }
    ಭಙ್ಗ;

  ಪಕ್ಷ 82: /* token_decl_for_prec.1: token_decl_for_prec  */
    { (yyval.yykind_87) = ಚಿಹ್ನೆ_list_sym_new ((yyvsp[0].token_decl_for_prec), (yylsp[0])); }
    ಭಙ್ಗ;

  ಪಕ್ಷ 83: /* token_decl_for_prec.1: token_decl_for_prec.1 token_decl_for_prec  */
    { (yyval.yykind_87) = ಚಿಹ್ನೆ_list_append ((yyvsp[-1].yykind_87), ಚಿಹ್ನೆ_list_sym_new ((yyvsp[0].token_decl_for_prec), (yylsp[0]))); }
    ಭಙ್ಗ;

  ಪಕ್ಷ 84: /* token_decl_for_prec: id int.opt  */
    {
      (yyval.token_decl_for_prec) = (yyvsp[-1].id);
      ಚಿಹ್ನೆ_class_set ((yyvsp[-1].id), token_sym, (yylsp[-1]), false);
      ಯದಿ (0 <= (yyvsp[0].yykind_84))
        ಚಿಹ್ನೆ_code_set ((yyvsp[-1].id), (yyvsp[0].yykind_84), (yylsp[0]));
    }
    ಭಙ್ಗ;

  ಪಕ್ಷ 86: /* symbol_decls: symbols.1  */
    {
      (yyval.ಚಿಹ್ನೆ_decls) = (yyvsp[0].yykind_90);
    }
    ಭಙ್ಗ;

  ಪಕ್ಷ 87: /* symbol_decls: "<tag>" symbols.1  */
    {
      (yyval.ಚಿಹ್ನೆ_decls) = ಚಿಹ್ನೆ_list_type_set ((yyvsp[0].yykind_90), (yyvsp[-1].TAG));
    }
    ಭಙ್ಗ;

  ಪಕ್ಷ 88: /* symbol_decls: symbol_decls "<tag>" symbols.1  */
    {
      (yyval.ಚಿಹ್ನೆ_decls) = ಚಿಹ್ನೆ_list_append ((yyvsp[-2].ಚಿಹ್ನೆ_decls), ಚಿಹ್ನೆ_list_type_set ((yyvsp[0].yykind_90), (yyvsp[-1].TAG)));
    }
    ಭಙ್ಗ;

  ಪಕ್ಷ 89: /* symbols.1: symbol  */
    {
      ಯದಿ (current_class != unknown_sym)
        ಚಿಹ್ನೆ_class_set ((yyvsp[0].ಚಿಹ್ನೆ), current_class, (yylsp[0]), false);
      (yyval.yykind_90) = ಚಿಹ್ನೆ_list_sym_new ((yyvsp[0].ಚಿಹ್ನೆ), (yylsp[0]));
    }
    ಭಙ್ಗ;

  ಪಕ್ಷ 90: /* symbols.1: symbols.1 symbol  */
    {
      ಯದಿ (current_class != unknown_sym)
        ಚಿಹ್ನೆ_class_set ((yyvsp[0].ಚಿಹ್ನೆ), current_class, (yylsp[0]), false);
      (yyval.yykind_90) = ಚಿಹ್ನೆ_list_append ((yyvsp[-1].yykind_90), ಚಿಹ್ನೆ_list_sym_new ((yyvsp[0].ಚಿಹ್ನೆ), (yylsp[0])));
    }
    ಭಙ್ಗ;

  ಪಕ್ಷ 95: /* rules_or_grammar_declaration: error ";"  */
    {
      yyerrok;
    }
    ಭಙ್ಗ;

  ಪಕ್ಷ 96: /* $@5: %empty  */
                         { current_lhs ((yyvsp[-1].id_colon), (yylsp[-1]), (yyvsp[0].yykind_97)); }
    ಭಙ್ಗ;

  ಪಕ್ಷ 97: /* rules: id_colon named_ref.opt $@5 ":" rhses.1  */
    {
      /* Free the current lhs. */
      current_lhs (0, (yylsp[-4]), 0);
    }
    ಭಙ್ಗ;

  ಪಕ್ಷ 98: /* rhses.1: rhs  */
                     { grammar_current_rule_end ((yylsp[0])); }
    ಭಙ್ಗ;

  ಪಕ್ಷ 99: /* rhses.1: rhses.1 "|" rhs  */
                     { grammar_current_rule_end ((yylsp[0])); }
    ಭಙ್ಗ;

  ಪಕ್ಷ 101: /* rhs: %empty  */
    { grammar_current_rule_begin (current_lhs_ಚಿಹ್ನೆ, current_lhs_loc,
                                  current_lhs_named_ref); }
    ಭಙ್ಗ;

  ಪಕ್ಷ 102: /* rhs: rhs symbol named_ref.opt  */
    { grammar_current_rule_ಚಿಹ್ನೆ_append ((yyvsp[-1].ಚಿಹ್ನೆ), (yylsp[-1]), (yyvsp[0].yykind_97)); }
    ಭಙ್ಗ;

  ಪಕ್ಷ 103: /* rhs: rhs tag.opt "{...}" named_ref.opt  */
    { grammar_current_rule_action_append ((yyvsp[-1].BRACED_CODE), (yylsp[-1]), (yyvsp[0].yykind_97), (yyvsp[-2].yykind_76)); }
    ಭಙ್ಗ;

  ಪಕ್ಷ 104: /* rhs: rhs "%?{...}"  */
    { grammar_current_rule_predicate_append ((yyvsp[0].BRACED_PREDICATE), (yylsp[0])); }
    ಭಙ್ಗ;

  ಪಕ್ಷ 105: /* rhs: rhs "%empty"  */
    { grammar_current_rule_empty_set ((yylsp[0])); }
    ಭಙ್ಗ;

  ಪಕ್ಷ 106: /* rhs: rhs "%prec" symbol  */
    { grammar_current_rule_prec_set ((yyvsp[0].ಚಿಹ್ನೆ), (yylsp[0])); }
    ಭಙ್ಗ;

  ಪಕ್ಷ 107: /* rhs: rhs "%dprec" "integer literal"  */
    { grammar_current_rule_dprec_set ((yyvsp[0].INT_LITERAL), (yylsp[0])); }
    ಭಙ್ಗ;

  ಪಕ್ಷ 108: /* rhs: rhs "%merge" "<tag>"  */
    { grammar_current_rule_merge_set ((yyvsp[0].TAG), (yylsp[0])); }
    ಭಙ್ಗ;

  ಪಕ್ಷ 109: /* rhs: rhs "%expect" "integer literal"  */
    { grammar_current_rule_expect_sr ((yyvsp[0].INT_LITERAL), (yylsp[0])); }
    ಭಙ್ಗ;

  ಪಕ್ಷ 110: /* rhs: rhs "%expect-rr" "integer literal"  */
    { grammar_current_rule_expect_rr ((yyvsp[0].INT_LITERAL), (yylsp[0])); }
    ಭಙ್ಗ;

  ಪಕ್ಷ 111: /* named_ref.opt: %empty  */
                 { (yyval.yykind_97) = ರಿಕ್ತ; }
    ಭಙ್ಗ;

  ಪಕ್ಷ 112: /* named_ref.opt: "[identifier]"  */
                 { (yyval.yykind_97) = named_ref_new ((yyvsp[0].BRACKETED_ID), (yylsp[0])); }
    ಭಙ್ಗ;

  ಪಕ್ಷ 114: /* value: %empty  */
          { (yyval.ಮೌಲ್ಯ).kind = muscle_keyword; (yyval.ಮೌಲ್ಯ).chars = ""; }
    ಭಙ್ಗ;

  ಪಕ್ಷ 115: /* value: "identifier"  */
          { (yyval.ಮೌಲ್ಯ).kind = muscle_keyword; (yyval.ಮೌಲ್ಯ).chars = (yyvsp[0].ID); }
    ಭಙ್ಗ;

  ಪಕ್ಷ 116: /* value: "string"  */
          { (yyval.ಮೌಲ್ಯ).kind = muscle_string;  (yyval.ಮೌಲ್ಯ).chars = unquote ((yyvsp[0].STRING)); gram_scanner_last_string_free ();}
    ಭಙ್ಗ;

  ಪಕ್ಷ 117: /* value: "{...}"  */
          { (yyval.ಮೌಲ್ಯ).kind = muscle_code;    (yyval.ಮೌಲ್ಯ).chars = strip_braces ((yyvsp[0].BRACED_CODE)); gram_scanner_last_string_free (); }
    ಭಙ್ಗ;

  ಪಕ್ಷ 118: /* id: "identifier"  */
    { (yyval.id) = ಚಿಹ್ನೆ_from_uniqstr ((yyvsp[0].ID), (yylsp[0])); }
    ಭಙ್ಗ;

  ಪಕ್ಷ 119: /* id: "character literal"  */
    {
      ಧ್ರುವ ವರ್ಣ *var = "api.token.raw";
      ಯದಿ (current_class == nterm_sym)
        {
          complain (&(yylsp[0]), complaint,
                    _("character literals cannot be nonterminals"));
          YYERROR;
        }
      ಯದಿ (muscle_percent_define_ifdef (var))
        {
          complain (&(yylsp[0]), complaint,
                    _("character literals cannot be used together"
                    " with %s"), var);
          location loc = muscle_percent_define_get_loc (var);
          subcomplain (&loc, complaint, _("definition of %s"), var);
        }
      (yyval.id) = ಚಿಹ್ನೆ_get (char_name ((yyvsp[0].CHAR_LITERAL)), (yylsp[0]));
      ಚಿಹ್ನೆ_class_set ((yyval.id), token_sym, (yylsp[0]), false);
      ಚಿಹ್ನೆ_code_set ((yyval.id), (yyvsp[0].CHAR_LITERAL), (yylsp[0]));
    }
    ಭಙ್ಗ;

  ಪಕ್ಷ 120: /* id_colon: "identifier:"  */
           { (yyval.id_colon) = ಚಿಹ್ನೆ_from_uniqstr ((yyvsp[0].ID_COLON), (yylsp[0])); }
    ಭಙ್ಗ;

  ಪಕ್ಷ 123: /* string_as_id: "string"  */
    {
      (yyval.string_as_id) = ಚಿಹ್ನೆ_get ((yyvsp[0].STRING), (yylsp[0]));
      ಚಿಹ್ನೆ_class_set ((yyval.string_as_id), token_sym, (yylsp[0]), false);
    }
    ಭಙ್ಗ;

  ಪಕ್ಷ 125: /* epilogue.opt: "%%" "epilogue"  */
    {
      muscle_code_grow ("epilogue", translate_code ((yyvsp[0].EPILOGUE), (yylsp[0]), true), (yylsp[0]));
      code_scanner_last_string_free ();
    }
    ಭಙ್ಗ;



        ಸ್ವಭಾವ: ಭಙ್ಗ;
      }
    ಯದಿ (yychar_backup != yychar)
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
  YY_ಚಿಹ್ನೆ_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ yylhs = yyr1[yyn] - YYNTOKENS;
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  ಗಚ್ಛ yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == GRAM_EMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  ಯದಿ (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yyesa, &yyes, &yyes_capacity, yytoken, &yylloc};
        ಯದಿ (yychar != GRAM_EMPTY)
          YY_LAC_ESTABLISH;
        ಯದಿ (yyreport_syntax_error (&yyctx) == 2)
          YYNOMEM;
      }
    }

  yyerror_range[1] = yylloc;
  ಯದಿ (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      ಯದಿ (yychar <= GRAM_EOF)
        {
          /* Return failure if at end of input.  */
          ಯದಿ (yychar == GRAM_EOF)
            YYABORT;
        }
      ಅನ್ಯಥಾ
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc);
          yychar = GRAM_EMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  ಗಚ್ಛ yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  ಯದಿ (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_ರಾಶಿ_PRINT (yyss, yyssp);
  yystate = *yyssp;
  ಗಚ್ಛ yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  ಪ್ರತಿ (;;)
    {
      yyn = yypact[yystate];
      ಯದಿ (!yypact_ಮೌಲ್ಯ_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          ಯದಿ (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              ಯದಿ (0 < yyn)
                ಭಙ್ಗ;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      ಯದಿ (yyssp == yyss)
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
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

  /* Shift the error token.  */
  YY_ಚಿಹ್ನೆ_PRINT ("Shifting", YY_ACCESSING_ಚಿಹ್ನೆ (yyn), yyvsp, yylsp);

  yystate = yyn;
  ಗಚ್ಛ yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  ಗಚ್ಛ yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  ಗಚ್ಛ yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, YY_("memory exhausted"));
  yyresult = 2;
  ಗಚ್ಛ yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  ಯದಿ (yychar != GRAM_EMPTY)
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
  ಯಾವತ್ (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_ಚಿಹ್ನೆ (+*yyssp), yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  ಯದಿ (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  ಯದಿ (yyes != yyesa)
    YYSTACK_FREE (yyes);

  ಪ್ರತ್ಯಾಗಮ yyresult;
}



ಪೂರ್ಣಾಙ್ಕ
yyreport_syntax_error (ಧ್ರುವ yypcontext_t *ctx)
{
  ಪೂರ್ಣಾಙ್ಕ res = 0;
  /* Arguments of format: reported tokens (one for the "unexpected",
     one per "expected"). */
  ಗಣನಾ { ARGS_MAX = 5 };
  ಧ್ರುವ ವರ್ಣ *argv[ARGS_MAX];
  ಪೂರ್ಣಾಙ್ಕ argc = 0;
  yysymbol_kind_t unexpected = yypcontext_token (ctx);
  ಯದಿ (unexpected != YYSYMBOL_YYEMPTY)
    {
      argv[argc++] = yysymbol_name (unexpected);
      yysymbol_kind_t expected[ARGS_MAX - 1];
      ಪೂರ್ಣಾಙ್ಕ nexpected = yypcontext_expected_tokens (ctx, expected, ARGS_MAX - 1);
      ಯದಿ (nexpected < 0)
        res = nexpected;
      ಅನ್ಯಥಾ
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < nexpected; ++i)
          argv[argc++] = yysymbol_name (expected[i]);
    }
  syntax_error (*yypcontext_location (ctx), argc, argv);
  ಪ್ರತ್ಯಾಗಮ res;
}


/* Return the location of the left-hand side of a rule whose
   right-hand side is RHS[1] ... RHS[N].  Ignore empty nonterminals in
   the right-hand side, and return an empty location equal to the end
   boundary of RHS[0] if the right-hand side is empty.  */

ಸ್ಥಿರ YYLTYPE
lloc_default (YYLTYPE ಧ್ರುವ *rhs, ಪೂರ್ಣಾಙ್ಕ n)
{
  YYLTYPE loc;

  /* SGI MIPSpro 7.4.1m miscompiles "loc.start = loc.end = rhs[n].end;".
     The bug is fixed in 7.4.2m, but play it safe for now.  */
  loc.start = rhs[n].end;
  loc.end = rhs[n].end;

  /* Ignore empty nonterminals the start of the right-hand side.
     Do not bother to ignore them at the end of the right-hand side,
     since empty nonterminals have the same end as their predecessors.  */
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 1; i <= n; i++)
    ಯದಿ (! equal_boundaries (rhs[i].start, rhs[i].end))
      {
        loc.start = rhs[i].start;
        ಭಙ್ಗ;
      }

  ಪ್ರತ್ಯಾಗಮ loc;
}

ಸ್ಥಿರ
ವರ್ಣ *strip_braces (ವರ್ಣ *code)
{
  code[strlen (code) - 1] = 0;
  ಪ್ರತ್ಯಾಗಮ code + 1;
}

ಸ್ಥಿರ
ವರ್ಣ ಧ್ರುವ *
translate_code (ವರ್ಣ *code, location loc, bool plain)
{
  code_props plain_code;
  ಯದಿ (plain)
    code_props_plain_init (&plain_code, code, loc);
  ಅನ್ಯಥಾ
    code_props_ಚಿಹ್ನೆ_action_init (&plain_code, code, loc);
  code_props_translate_code (&plain_code);
  gram_scanner_last_string_free ();
  ಪ್ರತ್ಯಾಗಮ plain_code.code;
}

ಸ್ಥಿರ
ವರ್ಣ ಧ್ರುವ *
translate_code_braceless (ವರ್ಣ *code, location loc)
{
  ಪ್ರತ್ಯಾಗಮ translate_code (strip_braces (code), loc, true);
}

ಸ್ಥಿರ ಶೂನ್ಯ
add_param (param_type type, ವರ್ಣ *decl, location loc)
{
  ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ alphanum[26 + 26 + 1 + 10 + 1] =
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "_"
    "0123456789";

  ವರ್ಣ ಧ್ರುವ *name_start = ರಿಕ್ತ;
  {
    ವರ್ಣ *p;
    /* Stop on last actual character.  */
    ಪ್ರತಿ (p = decl; p[1]; p++)
      ಯದಿ ((p == decl
           || ! memchr (alphanum, p[-1], ಪರಿಮಾಣ alphanum - 1))
          && memchr (alphanum, p[0], ಪರಿಮಾಣ alphanum - 10 - 1))
        name_start = p;

    /* Strip the surrounding '{' and '}', and any blanks just inside
       the braces.  */
    --p;
    ಯಾವತ್ (c_isspace ((unsigned ವರ್ಣ) *p))
      --p;
    p[1] = '\0';
    ++decl;
    ಯಾವತ್ (c_isspace ((unsigned ವರ್ಣ) *decl))
      ++decl;
  }

  ಯದಿ (! name_start)
    complain (&loc, complaint, _("missing identifier in parameter declaration"));
  ಅನ್ಯಥಾ
    {
      ವರ್ಣ *name = xmemdup0 (name_start, strspn (name_start, alphanum));
      ಯದಿ (type & param_lex)
        muscle_pair_list_grow ("lex_param", decl, name);
      ಯದಿ (type & param_parse)
        muscle_pair_list_grow ("parse_param", decl, name);
      ಮುಕ್ತ (name);
    }

  gram_scanner_last_string_free ();
}


ಸ್ಥಿರ ಶೂನ್ಯ
handle_header (ವರ್ಣ ಧ್ರುವ *ಮೌಲ್ಯ)
{
  header_flag = true;
  ಯದಿ (ಮೌಲ್ಯ)
    {
      ವರ್ಣ *file = unquote (ಮೌಲ್ಯ);
      spec_header_file = xstrdup (file);
      gram_scanner_last_string_free ();
      unquote_free (file);
    }
}


ಸ್ಥಿರ ಶೂನ್ಯ
handle_error_verbose (location ಧ್ರುವ *loc, ವರ್ಣ ಧ್ರುವ *directive)
{
  bison_directive (loc, directive);
  muscle_percent_define_ಸೇರಿಸು (directive, *loc, muscle_keyword, "",
                                MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
}


ಸ್ಥಿರ ಶೂನ್ಯ
handle_file_prefix (location ಧ್ರುವ *loc,
                    location ಧ್ರುವ *dir_loc,
                    ವರ್ಣ ಧ್ರುವ *directive, ವರ್ಣ ಧ್ರುವ *ಮೌಲ್ಯ_quoted)
{
  ವರ್ಣ *ಮೌಲ್ಯ = unquote (ಮೌಲ್ಯ_quoted);
  bison_directive (loc, directive);
  bool warned = false;

  ಯದಿ (location_empty (spec_file_prefix_loc))
    {
      spec_file_prefix_loc = *loc;
      spec_file_prefix = ಮೌಲ್ಯ;
    }
  ಅನ್ಯಥಾ
    {
      duplicate_directive (directive, spec_file_prefix_loc, *loc);
      warned = true;
    }

  ಯದಿ (!warned
      && STRNEQ (directive, "%file-prefix"))
    deprecated_directive (dir_loc, directive, "%file-prefix");
}

ಸ್ಥಿರ ಶೂನ್ಯ
handle_language (location ಧ್ರುವ *loc, ವರ್ಣ ಧ್ರುವ *lang)
{
  language_argmatch (unquote (lang), grammar_prio, *loc);
}


ಸ್ಥಿರ ಶೂನ್ಯ
handle_name_prefix (location ಧ್ರುವ *loc,
                    ವರ್ಣ ಧ್ರುವ *directive, ವರ್ಣ ಧ್ರುವ *ಮೌಲ್ಯ_quoted)
{
  ವರ್ಣ *ಮೌಲ್ಯ = unquote (ಮೌಲ್ಯ_quoted);
  bison_directive (loc, directive);

  ವರ್ಣ buf1[1024];
  size_t len1 = ಪರಿಮಾಣ (buf1);
  ವರ್ಣ *old = asnprintf (buf1, &len1, "%s\"%s\"", directive, ಮೌಲ್ಯ);
  ಯದಿ (!old)
    xalloc_die ();

  ಯದಿ (location_empty (spec_name_prefix_loc))
    {
      spec_name_prefix = ಮೌಲ್ಯ;
      spec_name_prefix_loc = *loc;

      ವರ್ಣ buf2[1024];
      size_t len2 = ಪರಿಮಾಣ (buf2);
      ವರ್ಣ *new = asnprintf (buf2, &len2, "%%define api.prefix {%s}", ಮೌಲ್ಯ);
      ಯದಿ (!new)
        xalloc_die ();
      deprecated_directive (loc, old, new);
      ಯದಿ (new != buf2)
        ಮುಕ್ತ (new);
    }
  ಅನ್ಯಥಾ
    duplicate_directive (old, spec_file_prefix_loc, *loc);

  ಯದಿ (old != buf1)
    ಮುಕ್ತ (old);
}


ಸ್ಥಿರ ಶೂನ್ಯ
handle_pure_parser (location ಧ್ರುವ *loc, ವರ್ಣ ಧ್ರುವ *directive)
{
  bison_directive (loc, directive);
  deprecated_directive (loc, directive, "%define api.pure");
  muscle_percent_define_ಸೇರಿಸು ("api.pure", *loc, muscle_keyword, "",
                                MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
}


ಸ್ಥಿರ ಶೂನ್ಯ
handle_require (location ಧ್ರುವ *loc, ವರ್ಣ ಧ್ರುವ *version_quoted)
{
  ವರ್ಣ *version = unquote (version_quoted);
  required_version = strversion_to_int (version);
  ಯದಿ (required_version == -1)
    {
      complain (loc, complaint, _("invalid version requirement: %s"),
                version);
      required_version = 0;
    }
  ಅನ್ಯಥಾ
    {
      ಧ್ರುವ ವರ್ಣ* package_version =
        0 < strverscmp (api_version, PACKAGE_VERSION)
        ? api_version : PACKAGE_VERSION;
      ಯದಿ (0 < strverscmp (version, package_version))
        {
          complain (loc, complaint, _("require bison %s, but have %s"),
                    version, package_version);
          exit (EX_MISMATCH);
        }
    }
  unquote_free (version);
  gram_scanner_last_string_free ();
}

ಸ್ಥಿರ ಶೂನ್ಯ
handle_skeleton (location ಧ್ರುವ *loc, ವರ್ಣ ಧ್ರುವ *skel_quoted)
{
  ವರ್ಣ *skel = unquote (skel_quoted);
  ವರ್ಣ ಧ್ರುವ *skeleton_user = skel;
  ಯದಿ (strchr (skeleton_user, '/'))
    {
      size_t dir_length = strlen (grammar_file);
      ಯಾವತ್ (dir_length && grammar_file[dir_length - 1] != '/')
        --dir_length;
      ಯಾವತ್ (dir_length && grammar_file[dir_length - 1] == '/')
        --dir_length;
      ವರ್ಣ *skeleton_build =
        xmalloc (dir_length + 1 + strlen (skeleton_user) + 1);
      ಯದಿ (dir_length > 0)
        {
          memcpy (skeleton_build, grammar_file, dir_length);
          skeleton_build[dir_length++] = '/';
        }
      strcpy (skeleton_build + dir_length, skeleton_user);
      skeleton_user = uniqstr_new (skeleton_build);
      ಮುಕ್ತ (skeleton_build);
    }
  skeleton_arg (skeleton_user, grammar_prio, *loc);
}


ಸ್ಥಿರ ಶೂನ್ಯ
handle_yacc (location ಧ್ರುವ *loc)
{
  ಧ್ರುವ ವರ್ಣ *directive = "%yacc";
  bison_directive (loc, directive);
  ಯದಿ (location_empty (yacc_loc))
    set_yacc (*loc);
  ಅನ್ಯಥಾ
    duplicate_directive (directive, yacc_loc, *loc);
}


ಸ್ಥಿರ ಶೂನ್ಯ
gram_error (location ಧ್ರುವ *loc, ವರ್ಣ ಧ್ರುವ *ಸಂದೇಶ)
{
  complain (loc, complaint, "%s", ಸಂದೇಶ);
}

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *
char_name (ವರ್ಣ c)
{
  ಯದಿ (c == '\'')
    ಪ್ರತ್ಯಾಗಮ "'\\''";
  ಅನ್ಯಥಾ
    {
      ವರ್ಣ buf[4];
      buf[0] = '\''; buf[1] = c; buf[2] = '\''; buf[3] = '\0';
      ಪ್ರತ್ಯಾಗಮ quotearg_style (escape_quoting_style, buf);
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
current_lhs (ಚಿಹ್ನೆ *sym, location loc, named_ref *ref)
{
  current_lhs_ಚಿಹ್ನೆ = sym;
  current_lhs_loc = loc;
  ಯದಿ (sym)
    ಚಿಹ್ನೆ_location_as_lhs_set (sym, loc);
  /* In order to simplify memory management, named references for lhs
     are always assigned by deep copy into the current symbol_list
     node.  This is because a single named-ref in the grammar may
     result in several uses when the user factors lhs between several
     rules using "|".  Therefore free the parser's original copy.  */
  ಮುಕ್ತ (current_lhs_named_ref);
  current_lhs_named_ref = ref;
}

ಸ್ಥಿರ ಶೂನ್ಯ tron (FILE *yyo)
{
  begin_use_class ("value", yyo);
}

ಸ್ಥಿರ ಶೂನ್ಯ troff (FILE *yyo)
{
  end_use_class ("value", yyo);
}


/*----------.
| Unquote.  |
`----------*/

ಸಂರಚನಾ obstack obstack_for_unquote;

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
unquote_free (ವರ್ಣ *last_string)
{
  obstack_free (&obstack_for_unquote, last_string);
}

ಸ್ಥಿರ ವರ್ಣ *
unquote (ಧ್ರುವ ವರ್ಣ *cp)
{
#define GROW(Char)                              \
  obstack_1grow (&obstack_for_unquote, Char);
  ಪ್ರತಿ (++cp; *cp && *cp != '"'; ++cp)
    ವಿಕಲ್ಪ (*cp)
      {
      ಪಕ್ಷ '"':
        ಭಙ್ಗ;
      ಪಕ್ಷ '\\':
        ++cp;
        ವಿಕಲ್ಪ (*cp)
          {
          ಪಕ್ಷ '0': ಪಕ್ಷ '1': ಪಕ್ಷ '2': ಪಕ್ಷ '3': ಪಕ್ಷ '4':
          ಪಕ್ಷ '5': ಪಕ್ಷ '6': ಪಕ್ಷ '7': ಪಕ್ಷ '8': ಪಕ್ಷ '9':
            {
              ಪೂರ್ಣಾಙ್ಕ c = cp[0] - '0';
              ಯದಿ (c_isdigit (cp[1]))
                {
                  ++cp;
                  c = c * 8 + cp[0] - '0';
                }
              ಯದಿ (c_isdigit (cp[1]))
                {
                  ++cp;
                  c = c * 8 + cp[0] - '0';
                }
              GROW (c);
            }
            ಭಙ್ಗ;

          ಪಕ್ಷ 'a': GROW ('\a'); ಭಙ್ಗ;
          ಪಕ್ಷ 'b': GROW ('\b'); ಭಙ್ಗ;
          ಪಕ್ಷ 'f': GROW ('\f'); ಭಙ್ಗ;
          ಪಕ್ಷ 'n': GROW ('\n'); ಭಙ್ಗ;
          ಪಕ್ಷ 'r': GROW ('\r'); ಭಙ್ಗ;
          ಪಕ್ಷ 't': GROW ('\t'); ಭಙ್ಗ;
          ಪಕ್ಷ 'v': GROW ('\v'); ಭಙ್ಗ;

          ಪಕ್ಷ 'x':
            {
              ಪೂರ್ಣಾಙ್ಕ c = 0;
              ಯಾವತ್ (c_isxdigit (cp[1]))
                {
                  ++cp;
                  c = (c * 16 + (c_isdigit (cp[0]) ? cp[0] - '0'
                                 : c_isupper (cp[0]) ? cp[0] - 'A'
                                 : cp[0] - '0'));
                }
              GROW (c);
              ಭಙ್ಗ;
            }
          }
        ಭಙ್ಗ;

      ಸ್ವಭಾವ:
        GROW (*cp);
        ಭಙ್ಗ;
      }
  ಪ್ರತಿಜ್ಞಾ (*cp == '"');
  ++cp;
  ಪ್ರತಿಜ್ಞಾ (*cp == '\0');
#undef GROW
  ಪ್ರತ್ಯಾಗಮ obstack_finish0 (&obstack_for_unquote);
}
