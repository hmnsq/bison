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
#घोषणा YYBISON 30802

/* Bison version string.  */
#घोषणा YYBISON_VERSION "3.8.2.30-82269"

/* Skeleton name.  */
#घोषणा YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#घोषणा YYPURE 2

/* Push parsers.  */
#घोषणा YYPUSH 0

/* Pull parsers.  */
#घोषणा YYPULL 1

/* "%code top" blocks.  */

  /* On column 0 to please syntax-check.  */
#समावेश <config.h>

/* Substitute the type names.  */
#घोषणा YYSTYPE         GRAM_STYPE
#घोषणा YYLTYPE         GRAM_LTYPE
/* Substitute the variable and function names.  */
#घोषणा yyparse         gram_parse
#घोषणा yylex           gram_lex
#घोषणा yyerror         gram_error
#घोषणा yydebug         gram_debug
#घोषणा yynerrs         gram_nerrs


# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  अन्यथा
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  अगर defined __cplusplus
#   अगर 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   अन्यथा
#    define YY_NULLPTR 0
#   endif
#  अन्यथा
#   define YY_NULLPTR ((व्योम*)0)
#  endif
# endif

#समावेश "src/parse-gram.h"
/* Symbol kind.  */
क्रमागत yysymbol_kind_t
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
  YYSYMBOL_PERCENT_बायाँ = 10,              /* "%left"  */
  YYSYMBOL_PERCENT_दायाँ = 11,             /* "%right"  */
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
  YYSYMBOL_PERCENT_NO_पंक्तियाँ = 31,          /* "%no-lines"  */
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
  YYSYMBOL_निवेश = 62,                     /*निवेश*/
  YYSYMBOL_prologue_declarations = 63,     /* prologue_declarations  */
  YYSYMBOL_prologue_declaration = 64,      /* prologue_declaration  */
  YYSYMBOL_65_1 = 65,                      /* $@1  */
  YYSYMBOL_params = 66,                    /* params  */
  YYSYMBOL_grammar_declaration = 67,       /* grammar_declaration  */
  YYSYMBOL_code_props_type = 68,           /* code_props_type  */
  YYSYMBOL_union_name = 69,                /* union_name  */
  YYSYMBOL_प्रतीक_declaration = 70,        /* symbol_declaration  */
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
  YYSYMBOL_प्रतीक_decls = 89,              /* symbol_decls  */
  YYSYMBOL_90_symbols_1 = 90,              /* symbols.1  */
  YYSYMBOL_grammar = 91,                   /* grammar  */
  YYSYMBOL_rules_or_grammar_declaration = 92, /* rules_or_grammar_declaration  */
  YYSYMBOL_rules = 93,                     /* rules  */
  YYSYMBOL_94_5 = 94,                      /* $@5  */
  YYSYMBOL_95_rhses_1 = 95,                /* rhses.1  */
  YYSYMBOL_rhs = 96,                       /* rhs  */
  YYSYMBOL_97_named_ref_opt = 97,          /* named_ref.opt  */
  YYSYMBOL_variable = 98,                  /* variable  */
  YYSYMBOL_मूल्य = 99,                     /*मूल्य*/
  YYSYMBOL_id = 100,                       /* id  */
  YYSYMBOL_id_colon = 101,                 /* id_colon  */
  YYSYMBOL_प्रतीक = 102,                   /*प्रतीक*/
  YYSYMBOL_string_as_id = 103,             /* string_as_id  */
  YYSYMBOL_104_epilogue_opt = 104          /* epilogue.opt  */
};
प्रकार क्रमागत yysymbol_kind_t yysymbol_kind_t;



/* Unqualified %code blocks.  */

  #समावेश "system.h"

  #समावेश <c-ctype.h>
  #समावेश <quotearg.h>
  #समावेश <vasnprintf.h>
  #समावेश <xmemdup0.h>

  #समावेश "complain.h"
  #समावेश "conflicts.h"
  #समावेश "files.h"
  #समावेश "getargs.h"
  #समावेश "gram.h"
  #समावेश "named-ref.h"
  #समावेश "reader.h"
  #समावेश "scan-code.h"
  #समावेश "scan-gram.h"
  #समावेश "strversion.h"

  /* Pretend to be at least that version, to check features published
     in that version while developping it.  */
  अटल स्थिर अक्षर* api_version = "3.8";

  अटल पूर्णांक current_prec = 0;
  अटल location current_lhs_loc;
  अटल named_ref *current_lhs_named_ref;
  अटल प्रतीक *current_lhs_प्रतीक;
  अटल प्रतीक_class current_class = unknown_sym;

  /** Set the new current left-hand side symbol, possibly common
   * to several right-hand side parts of rule.
   */
  अटल व्योम current_lhs (प्रतीक *sym, location loc, named_ref *ref);

  #घोषणा YYLLOC_DEFAULT(Current, Rhs, N)         \
    (Current) = lloc_default (Rhs, N)
  अटल YYLTYPE lloc_default (YYLTYPE स्थिर *, पूर्णांक);

  #घोषणा YY_LOCATION_PRINT(File, Loc)            \
    location_print (Loc, File)

  /* Strip initial '{' and final '}' (must be first and last characters).
     Return the result.  */
  अटल अक्षर *strip_braces (अक्षर *code);

  /* Convert CODE by calling code_props_plain_init if PLAIN, otherwise
     code_props_symbol_action_init.  Calls
     gram_scanner_last_string_free to release the latest string from
     the scanner (should be CODE). */
  अटल अक्षर स्थिर *translate_code (अक्षर *code, location loc, bool plain);

  /* Convert CODE by calling code_props_plain_init after having
     stripped the first and last characters (expected to be '{', and
     '}').  Calls gram_scanner_last_string_free to release the latest
     string from the scanner (should be CODE). */
  अटल अक्षर स्थिर *translate_code_braceless (अक्षर *code, location loc);

  /* Handle a %header directive.  */
  अटल व्योम handle_header (अक्षर स्थिर *मूल्य);

  /* Handle a %error-verbose directive.  */
  अटल व्योम handle_error_verbose (location स्थिर *loc, अक्षर स्थिर *directive);

  /* Handle a %file-prefix directive.  */
  अटल व्योम handle_file_prefix (location स्थिर *loc,
                                  location स्थिर *dir_loc,
                                  अक्षर स्थिर *directive, अक्षर स्थिर *मूल्य);

  /* Handle a %language directive.  */
  अटल व्योम handle_language (location स्थिर *loc, अक्षर स्थिर *lang);

  /* Handle a %name-prefix directive.  */
  अटल व्योम handle_name_prefix (location स्थिर *loc,
                                  अक्षर स्थिर *directive, अक्षर स्थिर *मूल्य);

  /* Handle a %pure-parser directive.  */
  अटल व्योम handle_pure_parser (location स्थिर *loc, अक्षर स्थिर *directive);

  /* Handle a %require directive.  */
  अटल व्योम handle_require (location स्थिर *loc, अक्षर स्थिर *version);

  /* Handle a %skeleton directive.  */
  अटल व्योम handle_skeleton (location स्थिर *loc, अक्षर स्थिर *skel);

  /* Handle a %yacc directive.  */
  अटल व्योम handle_yacc (location स्थिर *loc);

  /* Implementation of yyerror.  */
  अटल व्योम gram_error (location स्थिर *, अक्षर स्थिर *);

  /* A string that describes a char (e.g., 'a' -> "'a'").  */
  अटल अक्षर स्थिर *char_name (अक्षर);

  /* Add style to semantic values in traces.  */
  अटल व्योम tron (खाता *yyo);
  अटल व्योम troff (खाता *yyo);

  /* Interpret a quoted string (such as `"Hello, \"World\"\n\""`).
     Manages the memory of the result.  */
  अटल अक्षर *unquote (स्थिर अक्षर *str);

  /* Discard the latest unquoted string.  */
  अटल व्योम unquote_free (अक्षर *last_string);

  /** Add a lex-param and/or a parse-param.
   *
   * \param type  where to push this formal argument.
   * \param decl  the formal argument.  Destroyed.
   * \param loc   the location in the source.
   */
  अटल व्योम add_param (param_type type, अक्षर *decl, location loc);
  अटल param_type current_param = param_none;


#अगर_घोषित लघु
# undef लघु
#पूर्ण_अगर

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#अगर_अघोषित __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# अगर defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#पूर्ण_अगर

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#अगर_घोषित __INT_LEAST8_MAX__
प्रकार __INT_LEAST8_TYPE__ yytype_int8;
#या_अगर defined YY_STDINT_H
प्रकार int_least8_t yytype_int8;
#अन्यथा
प्रकार चिन्हित अक्षर yytype_int8;
#पूर्ण_अगर

#अगर_घोषित __INT_LEAST16_MAX__
प्रकार __INT_LEAST16_TYPE__ yytype_int16;
#या_अगर defined YY_STDINT_H
प्रकार int_least16_t yytype_int16;
#अन्यथा
प्रकार लघु yytype_int16;
#पूर्ण_अगर

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#अगर_घोषित __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#पूर्ण_अगर

#अगर defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
प्रकार __UINT_LEAST8_TYPE__ yytype_uint8;
#या_अगर (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= पूर्णांक_उच्च)
प्रकार uint_least8_t yytype_uint8;
#या_अगर !defined __UINT_LEAST8_MAX__ && अच_अक्षर_उच्च <= पूर्णांक_उच्च
प्रकार अचिन्हित अक्षर yytype_uint8;
#अन्यथा
प्रकार लघु yytype_uint8;
#पूर्ण_अगर

#अगर defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
प्रकार __UINT_LEAST16_TYPE__ yytype_uint16;
#या_अगर (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= पूर्णांक_उच्च)
प्रकार uint_least16_t yytype_uint16;
#या_अगर !defined __UINT_LEAST16_MAX__ && अच_लघु_उच्च <= पूर्णांक_उच्च
प्रकार अचिन्हित लघु yytype_uint16;
#अन्यथा
प्रकार पूर्णांक yytype_uint16;
#पूर्ण_अगर

#अगर_अघोषित YYPTRDIFF_T
# अगर defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef सूचक_भेद_प्रकार
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T सूचक_भेद_प्रकार
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# अन्यथा
#  define YYPTRDIFF_T दीर्घ
#  define YYPTRDIFF_MAXIMUM दीर्घ_उच्च
# endif
#पूर्ण_अगर

#अगर_अघोषित YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined माप_प्रकार
#  define YYSIZE_T माप_प्रकार
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T माप_प्रकार
# अन्यथा
#  define YYSIZE_T अचिन्हित
# endif
#पूर्ण_अगर

#घोषणा YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#घोषणा YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, माप (X))


/* Stored state numbers (used for stacks). */
प्रकार yytype_uint8 yy_state_t;

/* State numbers in computations.  */
प्रकार पूर्णांक yy_state_fast_t;

#अगर_अघोषित YY_
# अगर defined YYENABLE_NLS && YYENABLE_NLS
#  अगर ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#पूर्ण_अगर

#अगर_अघोषित N_
# define N_(Msgid) Msgid
#पूर्ण_अगर


#अगर_अघोषित YY_ATTRIBUTE_PURE
# अगर defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# अन्यथा
#  define YY_ATTRIBUTE_PURE
# endif
#पूर्ण_अगर

#अगर_अघोषित YY_ATTRIBUTE_UNUSED
# अगर defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# अन्यथा
#  define YY_ATTRIBUTE_UNUSED
# endif
#पूर्ण_अगर

/* Suppress unused-variable warnings by "using" E.  */
#अगर ! defined lint || defined __GNUC__
# define YY_USE(E) ((व्योम) (E))
#अन्यथा
# define YY_USE(E) /* empty */
#पूर्ण_अगर

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#अगर defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# अगर __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# अन्यथा
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#अन्यथा
# define YY_INITIAL_मूल्य(मूल्य_4) मूल्य_4
#पूर्ण_अगर
#अगर_अघोषित YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#पूर्ण_अगर
#अगर_अघोषित YY_INITIAL_मूल्य
# define YY_INITIAL_मूल्य(मूल्य_4) /* Nothing. */
#पूर्ण_अगर

#अगर defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#पूर्ण_अगर
#अगर_अघोषित YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#पूर्ण_अगर


#घोषणा YY_ASSERT(E) ((व्योम) (0 && (E)))

#अगर 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) करो { /* empty */; } जबतक (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# अन्यथा
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  अगर (defined __cplusplus && ! defined निकास_सफल \
       && ! ((defined YYMALLOC || defined दो_स्मृति) \
             && (defined YYFREE || defined मुक्त)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef निकास_सफल
#    define निकास_सफल 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC दो_स्मृति
#   अगर ! defined दो_स्मृति && ! defined निकास_सफल
व्योम *दो_स्मृति (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE मुक्त
#   अगर ! defined मुक्त && ! defined निकास_सफल
व्योम मुक्त (व्योम *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
# define YYCOPY_NEEDED 1
#पूर्ण_अगर /* 1 */

#अगर (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined GRAM_LTYPE_IS_TRIVIAL && GRAM_LTYPE_IS_TRIVIAL \
             && defined GRAM_STYPE_IS_TRIVIAL && GRAM_STYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
जोड़ yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (जोड़ yyalloc) - 1)

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
# define YYSTACK_RELOCATE(ढेर_alloc, ढेर)                           \
    करो                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->ढेर_alloc, ढेर, yysize);                    \
        ढेर = &yyptr->ढेर_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*ढेर) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    जबतक (0)

#पूर्ण_अगर

#अगर defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  अगर defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, गिनती_2) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (गिनती_2)) * माप (*(Src)))
#  अन्यथा
#   define YYCOPY(Dst, Src, गिनती_2)              \
      करो                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          क्रम (yyi = 0; yyi < (गिनती_2); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      जबतक (0)
#  endif
# endif
#पूर्ण_अगर /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#घोषणा YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#घोषणा YYLAST   240

/* YYNTOKENS -- Number of terminals.  */
#घोषणा YYNTOKENS  61
/* YYNNTS -- Number of nonterminals.  */
#घोषणा YYNNTS  44
/* YYNRULES -- Number of rules.  */
#घोषणा YYNRULES  125
/* YYNSTATES -- Number of states.  */
#घोषणा YYNSTATES  169

/* YYMAXUTOK -- Last valid token kind.  */
#घोषणा YYMAXUTOK   315


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#घोषणा YYTRANSLATE(YYX) YY_CAST (yysymbol_kind_t, YYX)

#अगर GRAM_DEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
अटल स्थिर yytype_int16 yyrline[] =
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
#पूर्ण_अगर

/** Accessing symbol of state STATE.  */
#घोषणा YY_ACCESSING_प्रतीक(State) YY_CAST (yysymbol_kind_t, yystos[State])

#अगर 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
अटल स्थिर अक्षर *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

अटल स्थिर अक्षर *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  अटल स्थिर अक्षर *स्थिर yy_sname[] =
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
  अटल yytype_int8 yytranslatable[] =
  {
       1,     1,     1,     1,     1,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     1,     1,     0,     1,     0,     1,     1,
       0,     0,     0,     0,     1,     0,     0,     1,     0,     0,
       0
  };
  वापस (yysymbol < YYNTOKENS && yytranslatable[yysymbol]
          ? _(yy_sname[yysymbol])
          : yy_sname[yysymbol]);
}
#पूर्ण_अगर

#घोषणा YYPACT_NINF (-148)

#घोषणा yypact_मूल्य_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#घोषणा YYTABLE_NINF (-125)

#घोषणा yytable_मूल्य_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
अटल स्थिर yytype_int16 yypact[] =
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
अटल स्थिर yytype_int8 yydefact[] =
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
अटल स्थिर yytype_int16 yypgoto[] =
{
    -148,  -148,  -148,  -148,  -148,  -148,   109,  -148,  -148,  -148,
    -148,  -148,  -148,  -148,  -148,  -148,  -148,    -7,  -148,  -148,
      66,   -73,   -87,    60,  -148,  -148,   -71,   -83,  -148,   -50,
    -148,    51,  -148,  -148,  -148,   -19,  -147,  -148,  -148,   -47,
    -148,   -48,   -38,  -148
};

/* YYDEFGOTO[NTERM-NUM].  */
अटल स्थिर yytype_uint8 yydefgoto[] =
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
अटल स्थिर yytype_int16 yytable[] =
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

अटल स्थिर yytype_int16 yycheck[] =
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
अटल स्थिर yytype_int8 yystos[] =
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
अटल स्थिर yytype_int8 yyr1[] =
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
अटल स्थिर yytype_int8 yyr2[] =
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


क्रमागत { YYENOMEM = -2 };

#घोषणा yyerrok         (yyerrstatus = 0)
#घोषणा yyclearin       (yychar = GRAM_EMPTY)

#घोषणा YYACCEPT        जाओ yyacceptlab
#घोषणा YYABORT         जाओ yyabortlab
#घोषणा YYERROR         जाओ yyerrorlab
#घोषणा YYNOMEM         जाओ yyexhaustedlab


#घोषणा YYRECOVERING()  (!!yyerrstatus)

#घोषणा YYBACKUP(Token, मूल्य_4)                                    \
  करो                                                              \
    अगर (yychar == GRAM_EMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (मूल्य_4);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        YY_LAC_DISCARD ("YYBACKUP");                              \
        जाओ yybackup;                                            \
      }                                                           \
    अन्यथा                                                          \
      {                                                           \
        yyerror (&yylloc, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  जबतक (0)

/* Backward compatibility with an undocumented macro.
   Use GRAM_error or GRAM_UNDEF. */
#घोषणा YYERRCODE GRAM_UNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#अगर_अघोषित YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    करो                                                                  \
      अगर (N)                                                            \
        {                                                               \
          (Current).first_पंक्ति   = YYRHSLOC (Rhs, 1).first_पंक्ति;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_पंक्ति    = YYRHSLOC (Rhs, N).last_पंक्ति;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      अन्यथा                                                              \
        {                                                               \
          (Current).first_पंक्ति   = (Current).last_पंक्ति   =              \
            YYRHSLOC (Rhs, 0).last_पंक्ति;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    जबतक (0)
#पूर्ण_अगर

#घोषणा YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#अगर GRAM_DEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF ख_लिखो
# endif

# define YYDPRINTF(Args)                        \
करो {                                            \
  अगर (yydebug)                                  \
    YYFPRINTF Args;                             \
} जबतक (0)


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  अगर defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined GRAM_LTYPE_IS_TRIVIAL && GRAM_LTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
अटल पूर्णांक
yy_location_print_ (खाता *yyo, YYLTYPE स्थिर * स्थिर yylocp)
{
  पूर्णांक res = 0;
  पूर्णांक end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  अगर (0 <= yylocp->first_पंक्ति)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_पंक्ति);
      अगर (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  अगर (0 <= yylocp->last_पंक्ति)
    {
      अगर (yylocp->first_पंक्ति < yylocp->last_पंक्ति)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_पंक्ति);
          अगर (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      अन्यथा अगर (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  वापस res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  अन्यथा

#   define YYLOCATION_PRINT(File, Loc) ((व्योम) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_प्रतीक_PRINT(Title, Kind, मूल्य_4, Location)                    \
करो {                                                                      \
  अगर (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (मानक_त्रुटि, "%s ", Title);                                   \
      yy_प्रतीक_print (मानक_त्रुटि,                                            \
                  Kind, मूल्य_4, Location); \
      YYFPRINTF (मानक_त्रुटि, "\n");                                           \
    }                                                                     \
} जबतक (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

अटल व्योम
yy_प्रतीक_मूल्य_print (खाता *yyo,
                       yysymbol_kind_t yykind, YYSTYPE स्थिर * स्थिर yyvaluep, YYLTYPE स्थिर * स्थिर yylocationp)
{
  खाता *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  अगर (!yyvaluep)
    वापस;
/* "%code pre-printer" blocks.  */
tron (yyo);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  चयन (yykind)
    {
    हाल YYSYMBOL_STRING: /* "string"  */
         { ख_माला_दो (((*yyvaluep).STRING), yyo); }
        अवरोध;

    हाल YYSYMBOL_TSTRING: /* "translatable string"  */
         { ख_माला_दो (((*yyvaluep).TSTRING), yyo); }
        अवरोध;

    हाल YYSYMBOL_PERCENT_ERROR_VERBOSE: /* "%error-verbose"  */
         { ख_माला_दो (((*yyvaluep).PERCENT_ERROR_VERBOSE), yyo); }
        अवरोध;

    हाल YYSYMBOL_PERCENT_FILE_PREFIX: /* "%file-prefix"  */
         { ख_माला_दो (((*yyvaluep).PERCENT_FILE_PREFIX), yyo); }
        अवरोध;

    हाल YYSYMBOL_PERCENT_FLAG: /* "%<flag>"  */
         { ख_लिखो (yyo, "%%%s", ((*yyvaluep).PERCENT_FLAG)); }
        अवरोध;

    हाल YYSYMBOL_PERCENT_NAME_PREFIX: /* "%name-prefix"  */
         { ख_माला_दो (((*yyvaluep).PERCENT_NAME_PREFIX), yyo); }
        अवरोध;

    हाल YYSYMBOL_PERCENT_PURE_PARSER: /* "%pure-parser"  */
         { ख_माला_दो (((*yyvaluep).PERCENT_PURE_PARSER), yyo); }
        अवरोध;

    हाल YYSYMBOL_BRACED_CODE: /* "{...}"  */
         { ख_माला_दो (((*yyvaluep).BRACED_CODE), yyo); }
        अवरोध;

    हाल YYSYMBOL_BRACED_PREDICATE: /* "%?{...}"  */
         { ख_माला_दो (((*yyvaluep).BRACED_PREDICATE), yyo); }
        अवरोध;

    हाल YYSYMBOL_BRACKETED_ID: /* "[identifier]"  */
         { ख_लिखो (yyo, "[%s]", ((*yyvaluep).BRACKETED_ID)); }
        अवरोध;

    हाल YYSYMBOL_CHAR_LITERAL: /* "character literal"  */
         { ख_माला_दो (char_name (((*yyvaluep).CHAR_LITERAL)), yyo); }
        अवरोध;

    हाल YYSYMBOL_EPILOGUE: /* "epilogue"  */
         { ख_माला_दो (((*yyvaluep).EPILOGUE), yyo); }
        अवरोध;

    हाल YYSYMBOL_ID: /* "identifier"  */
         { ख_माला_दो (((*yyvaluep).ID), yyo); }
        अवरोध;

    हाल YYSYMBOL_ID_COLON: /* "identifier:"  */
         { ख_लिखो (yyo, "%s:", ((*yyvaluep).ID_COLON)); }
        अवरोध;

    हाल YYSYMBOL_PROLOGUE: /* "%{...%}"  */
         { ख_माला_दो (((*yyvaluep).PROLOGUE), yyo); }
        अवरोध;

    हाल YYSYMBOL_TAG: /* "<tag>"  */
         { ख_लिखो (yyo, "<%s>", ((*yyvaluep).TAG)); }
        अवरोध;

    हाल YYSYMBOL_INT_LITERAL: /* "integer literal"  */
         { ख_लिखो (yyo, "%d", ((*yyvaluep).INT_LITERAL)); }
        अवरोध;

    हाल YYSYMBOL_PERCENT_PARAM: /* "%param"  */
{
  चयन (((*yyvaluep).PERCENT_PARAM))
    {
#घोषणा CASE(In, Out)                                           \
      हाल param_ ## In: ख_माला_दो ("%" #Out, yyo); अवरोध
      CASE (lex,   lex-param);
      CASE (parse, parse-param);
      CASE (both,  param);
#अघोषित CASE
      हाल param_none: aver (false); अवरोध;
    }
}
        अवरोध;

    हाल YYSYMBOL_code_props_type: /* code_props_type  */
         { ख_लिखो (yyo, "%s", code_props_type_string (((*yyvaluep).code_props_type))); }
        अवरोध;

    हाल YYSYMBOL_75_string_opt: /* string.opt  */
         { ख_माला_दो (((*yyvaluep).yykind_75), yyo); }
        अवरोध;

    हाल YYSYMBOL_76_tag_opt: /* tag.opt  */
         { ख_माला_दो (((*yyvaluep).yykind_76), yyo); }
        अवरोध;

    हाल YYSYMBOL_generic_symlist: /* generic_symlist  */
         { प्रतीक_list_syms_print (((*yyvaluep).generic_symlist), yyo); }
        अवरोध;

    हाल YYSYMBOL_generic_symlist_item: /* generic_symlist_item  */
         { प्रतीक_list_syms_print (((*yyvaluep).generic_symlist_item), yyo); }
        अवरोध;

    हाल YYSYMBOL_tag: /* tag  */
         { ख_लिखो (yyo, "<%s>", ((*yyvaluep).tag)); }
        अवरोध;

    हाल YYSYMBOL_nterm_decls: /* nterm_decls  */
         { प्रतीक_list_syms_print (((*yyvaluep).nterm_decls), yyo); }
        अवरोध;

    हाल YYSYMBOL_token_decls: /* token_decls  */
         { प्रतीक_list_syms_print (((*yyvaluep).token_decls), yyo); }
        अवरोध;

    हाल YYSYMBOL_82_token_decl_1: /* token_decl.1  */
         { प्रतीक_list_syms_print (((*yyvaluep).yykind_82), yyo); }
        अवरोध;

    हाल YYSYMBOL_token_decl: /* token_decl  */
         { ख_लिखो (yyo, "%s", ((*yyvaluep).token_decl) ? ((*yyvaluep).token_decl)->tag : "<NULL>"); }
        अवरोध;

    हाल YYSYMBOL_84_int_opt: /* int.opt  */
         { ख_लिखो (yyo, "%d", ((*yyvaluep).yykind_84)); }
        अवरोध;

    हाल YYSYMBOL_alias: /* alias  */
         { ख_लिखो (yyo, "%s", ((*yyvaluep).alias) ? ((*yyvaluep).alias)->tag : "<NULL>"); }
        अवरोध;

    हाल YYSYMBOL_token_decls_for_prec: /* token_decls_for_prec  */
         { प्रतीक_list_syms_print (((*yyvaluep).token_decls_for_prec), yyo); }
        अवरोध;

    हाल YYSYMBOL_87_token_decl_for_prec_1: /* token_decl_for_prec.1  */
         { प्रतीक_list_syms_print (((*yyvaluep).yykind_87), yyo); }
        अवरोध;

    हाल YYSYMBOL_token_decl_for_prec: /* token_decl_for_prec  */
         { ख_लिखो (yyo, "%s", ((*yyvaluep).token_decl_for_prec) ? ((*yyvaluep).token_decl_for_prec)->tag : "<NULL>"); }
        अवरोध;

    हाल YYSYMBOL_प्रतीक_decls: /* symbol_decls  */
         { प्रतीक_list_syms_print (((*yyvaluep).प्रतीक_decls), yyo); }
        अवरोध;

    हाल YYSYMBOL_90_symbols_1: /* symbols.1  */
         { प्रतीक_list_syms_print (((*yyvaluep).yykind_90), yyo); }
        अवरोध;

    हाल YYSYMBOL_variable: /* variable  */
         { ख_माला_दो (((*yyvaluep).variable), yyo); }
        अवरोध;

    हाल YYSYMBOL_मूल्य: /*मूल्य*/
{
  चयन (((*yyvaluep).मूल्य).kind)
    {
    हाल muscle_code:    ख_लिखो (yyo,  "{%s}",  ((*yyvaluep).मूल्य).chars); अवरोध;
    हाल muscle_keyword: ख_लिखो (yyo,   "%s",   ((*yyvaluep).मूल्य).chars); अवरोध;
    हाल muscle_string:  ख_लिखो (yyo, "\"%s\"", ((*yyvaluep).मूल्य).chars); अवरोध;
    }
}
        अवरोध;

    हाल YYSYMBOL_id: /* id  */
         { ख_लिखो (yyo, "%s", ((*yyvaluep).id) ? ((*yyvaluep).id)->tag : "<NULL>"); }
        अवरोध;

    हाल YYSYMBOL_id_colon: /* id_colon  */
         { ख_लिखो (yyo, "%s:", ((*yyvaluep).id_colon)->tag); }
        अवरोध;

    हाल YYSYMBOL_प्रतीक: /*प्रतीक*/
         { ख_लिखो (yyo, "%s", ((*yyvaluep).प्रतीक) ? ((*yyvaluep).प्रतीक)->tag : "<NULL>"); }
        अवरोध;

    हाल YYSYMBOL_string_as_id: /* string_as_id  */
         { ख_लिखो (yyo, "%s", ((*yyvaluep).string_as_id) ? ((*yyvaluep).string_as_id)->tag : "<NULL>"); }
        अवरोध;

      शेष:
        अवरोध;
    }
  YY_IGNORE_MAYBE_UNINITIALIZED_END
/* "%code post-printer" blocks.  */
troff (yyo);

}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

अटल व्योम
yy_प्रतीक_print (खाता *yyo,
                 yysymbol_kind_t yykind, YYSTYPE स्थिर * स्थिर yyvaluep, YYLTYPE स्थिर * स्थिर yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_प्रतीक_मूल्य_print (yyo, yykind, yyvaluep, yylocationp);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

अटल व्योम
yy_ढेर_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (मानक_त्रुटि, "Stack now");
  क्रम (; yybottom <= yytop; yybottom++)
    {
      पूर्णांक yybot = *yybottom;
      YYFPRINTF (मानक_त्रुटि, " %d", yybot);
    }
  YYFPRINTF (मानक_त्रुटि, "\n");
}

# define YY_ढेर_PRINT(Bottom, शीर्ष)                            \
करो {                                                            \
  अगर (yydebug)                                                  \
    yy_ढेर_print ((Bottom), (शीर्ष));                           \
} जबतक (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

अटल व्योम
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 पूर्णांक yyrule)
{
  पूर्णांक yylno = yyrline[yyrule];
  पूर्णांक yynrhs = yyr2[yyrule];
  पूर्णांक yyi;
  YYFPRINTF (मानक_त्रुटि, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  क्रम (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (मानक_त्रुटि, "   $%d = ", yyi + 1);
      yy_प्रतीक_print (मानक_त्रुटि,
                       YY_ACCESSING_प्रतीक (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]));
      YYFPRINTF (मानक_त्रुटि, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
करो {                                    \
  अगर (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
} जबतक (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
पूर्णांक yydebug;
#अन्यथा /* !GRAM_DEBUG */
# define YYDPRINTF(Args) ((व्योम) 0)
# define YY_प्रतीक_PRINT(Title, Kind, मूल्य_4, Location)
# define YY_ढेर_PRINT(Bottom, शीर्ष)
# define YY_REDUCE_PRINT(Rule)
#पूर्ण_अगर /* !GRAM_DEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#अगर_अघोषित YYINITDEPTH
# define YYINITDEPTH 200
#पूर्ण_अगर

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#अगर_अघोषित YYMAXDEPTH
# define YYMAXDEPTH 10000
#पूर्ण_अगर


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
अटल पूर्णांक
yy_lac_ढेर_realloc (YYPTRDIFF_T *yycapacity, YYPTRDIFF_T yyadd,
#अगर GRAM_DEBUG
                      अक्षर स्थिर *yydebug_prefix,
                      अक्षर स्थिर *yydebug_suffix,
#पूर्ण_अगर
                      yy_state_t **yybottom,
                      yy_state_t *yybottom_no_free,
                      yy_state_t **yytop, yy_state_t *yytop_empty)
{
  YYPTRDIFF_T yysize_old =
    *yytop == yytop_empty ? 0 : *yytop - *yybottom + 1;
  YYPTRDIFF_T yysize_new = yysize_old + yyadd;
  अगर (*yycapacity < yysize_new)
    {
      YYPTRDIFF_T yyalloc = 2 * yysize_new;
      yy_state_t *yybottom_new;
      /* Use YYMAXDEPTH for maximum stack size given that the stack
         should never need to grow larger than the main state stack
         needs to grow without LAC.  */
      अगर (YYMAXDEPTH < yysize_new)
        {
          YYDPRINTF ((मानक_त्रुटि, "%smax size exceeded%s", yydebug_prefix,
                      yydebug_suffix));
          वापस YYENOMEM;
        }
      अगर (YYMAXDEPTH < yyalloc)
        yyalloc = YYMAXDEPTH;
      yybottom_new =
        YY_CAST (yy_state_t *,
                 YYSTACK_ALLOC (YY_CAST (YYSIZE_T,
                                         yyalloc * YYSIZEOF (*yybottom_new))));
      अगर (!yybottom_new)
        {
          YYDPRINTF ((मानक_त्रुटि, "%srealloc failed%s", yydebug_prefix,
                      yydebug_suffix));
          वापस YYENOMEM;
        }
      अगर (*yytop != yytop_empty)
        {
          YYCOPY (yybottom_new, *yybottom, yysize_old);
          *yytop = yybottom_new + (yysize_old - 1);
        }
      अगर (*yybottom != yybottom_no_free)
        YYSTACK_FREE (*yybottom);
      *yybottom = yybottom_new;
      *yycapacity = yyalloc;
    }
  वापस 0;
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
#घोषणा YY_LAC_ESTABLISH                                                \
करो {                                                                    \
  अगर (!yy_lac_established)                                              \
    {                                                                   \
      YYDPRINTF ((मानक_त्रुटि,                                               \
                  "LAC: initial context established for %s\n",          \
                  yysymbol_name (yytoken)));                            \
      yy_lac_established = 1;                                           \
      चयन (yy_lac (yyesa, &yyes, &yyes_capacity, yyssp, yytoken))    \
        {                                                               \
        हाल YYENOMEM:                                                  \
          YYNOMEM;                                                      \
        हाल 1:                                                         \
          जाओ yyerrlab;                                                \
        }                                                               \
    }                                                                   \
} जबतक (0)

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
#अगर GRAM_DEBUG
# define YY_LAC_DISCARD(Event)                                           \
करो {                                                                     \
  अगर (yy_lac_established)                                                \
    {                                                                    \
      YYDPRINTF ((मानक_त्रुटि, "LAC: initial context discarded due to "       \
                  Event "\n"));                                          \
      yy_lac_established = 0;                                            \
    }                                                                    \
} जबतक (0)
#अन्यथा
# define YY_LAC_DISCARD(Event) yy_lac_established = 0
#पूर्ण_अगर

/* Given the stack whose top is *YYSSP, return 0 iff YYTOKEN can
   eventually (after perhaps some reductions) be shifted, return 1 if
   not, or return YYENOMEM if memory is exhausted.  As preconditions and
   postconditions: *YYES_CAPACITY is the allocated size of the array to
   which *YYES points, and either *YYES = YYESA or *YYES points to an
   array allocated with YYSTACK_ALLOC.  yy_lac may overwrite the
   contents of either array, alter *YYES and *YYES_CAPACITY, and free
   any old *YYES other than YYESA.  */
अटल पूर्णांक
yy_lac (yy_state_t *yyesa, yy_state_t **yyes,
        YYPTRDIFF_T *yyes_capacity, yy_state_t *yyssp, yysymbol_kind_t yytoken)
{
  yy_state_t *yyes_पूर्व = yyssp;
  yy_state_t *yyesp = yyes_पूर्व;
  /* Reduce until we encounter a shift and thereby accept the token.  */
  YYDPRINTF ((मानक_त्रुटि, "LAC: checking lookahead %s:", yysymbol_name (yytoken)));
  अगर (yytoken == YYSYMBOL_YYUNDEF)
    {
      YYDPRINTF ((मानक_त्रुटि, " Always Err\n"));
      वापस 1;
    }
  जबतक (1)
    {
      पूर्णांक yyrule = yypact[+*yyesp];
      अगर (yypact_मूल्य_is_default (yyrule)
          || (yyrule += yytoken) < 0 || YYLAST < yyrule
          || yycheck[yyrule] != yytoken)
        {
          /* Use the default action.  */
          yyrule = yydefact[+*yyesp];
          अगर (yyrule == 0)
            {
              YYDPRINTF ((मानक_त्रुटि, " Err\n"));
              वापस 1;
            }
        }
      अन्यथा
        {
          /* Use the action from yytable.  */
          yyrule = yytable[yyrule];
          अगर (yytable_मूल्य_is_error (yyrule))
            {
              YYDPRINTF ((मानक_त्रुटि, " Err\n"));
              वापस 1;
            }
          अगर (0 < yyrule)
            {
              YYDPRINTF ((मानक_त्रुटि, " S%d\n", yyrule));
              वापस 0;
            }
          yyrule = -yyrule;
        }
      /* By now we know we have to simulate a reduce.  */
      YYDPRINTF ((मानक_त्रुटि, " R%d", yyrule - 1));
      {
        /* Pop the corresponding number of values from the stack.  */
        YYPTRDIFF_T yylen = yyr2[yyrule];
        /* First pop from the LAC stack as many tokens as possible.  */
        अगर (yyesp != yyes_पूर्व)
          {
            YYPTRDIFF_T yysize = yyesp - *yyes + 1;
            अगर (yylen < yysize)
              {
                yyesp -= yylen;
                yylen = 0;
              }
            अन्यथा
              {
                yyesp = yyes_पूर्व;
                yylen -= yysize;
              }
          }
        /* Only afterwards look at the main stack.  */
        अगर (yylen)
          yyesp = yyes_पूर्व -= yylen;
      }
      /* Push the resulting state of the reduction.  */
      {
        yy_state_fast_t yystate;
        {
          स्थिर पूर्णांक yylhs = yyr1[yyrule] - YYNTOKENS;
          स्थिर पूर्णांक yyi = yypgoto[yylhs] + *yyesp;
          yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyesp
                     ? yytable[yyi]
                     : yydefgoto[yylhs]);
        }
        अगर (yyesp == yyes_पूर्व)
          {
            yyesp = *yyes;
            YY_IGNORE_USELESS_CAST_BEGIN
            *yyesp = YY_CAST (yy_state_t, yystate);
            YY_IGNORE_USELESS_CAST_END
          }
        अन्यथा
          {
            अगर (yy_lac_ढेर_realloc (yyes_capacity, 1,
#अगर GRAM_DEBUG
                                      " (", ")",
#पूर्ण_अगर
                                      yyes, yyesa, &yyesp, yyes_पूर्व))
              {
                YYDPRINTF ((मानक_त्रुटि, "\n"));
                वापस YYENOMEM;
              }
            YY_IGNORE_USELESS_CAST_BEGIN
            *++yyesp = YY_CAST (yy_state_t, yystate);
            YY_IGNORE_USELESS_CAST_END
          }
        YYDPRINTF ((मानक_त्रुटि, " G%d", yystate));
      }
    }
}

/* Context of a parse error.  */
प्रकार काष्ठा
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
अटल पूर्णांक
yypcontext_expected_tokens (स्थिर yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], पूर्णांक yyargn)
{
  /* Actual size of YYARG. */
  पूर्णांक yycount = 0;

  पूर्णांक yyx;
  क्रम (yyx = 0; yyx < YYNTOKENS; ++yyx)
    {
      yysymbol_kind_t yysym = YY_CAST (yysymbol_kind_t, yyx);
      अगर (yysym != YYSYMBOL_YYerror && yysym != YYSYMBOL_YYUNDEF)
        चयन (yy_lac (yyctx->yyesa, yyctx->yyes, yyctx->yyes_capacity, yyctx->yyssp, yysym))
          {
          हाल YYENOMEM:
            वापस YYENOMEM;
          हाल 1:
            जारी;
          शेष:
            अगर (!yyarg)
              ++yycount;
            अन्यथा अगर (yycount == yyargn)
              वापस 0;
            अन्यथा
              yyarg[yycount++] = yysym;
          }
    }
  अगर (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  वापस yycount;
}




/* The kind of the lookahead of this context.  */
अटल yysymbol_kind_t
yypcontext_token (स्थिर yypcontext_t *yyctx) YY_ATTRIBUTE_UNUSED;

अटल yysymbol_kind_t
yypcontext_token (स्थिर yypcontext_t *yyctx)
{
  वापस yyctx->yytoken;
}

/* The location of the lookahead of this context.  */
अटल YYLTYPE *
yypcontext_location (स्थिर yypcontext_t *yyctx) YY_ATTRIBUTE_UNUSED;

अटल YYLTYPE *
yypcontext_location (स्थिर yypcontext_t *yyctx)
{
  वापस yyctx->yylloc;
}

/* User defined function to report a syntax error.  */
अटल पूर्णांक
yyreport_syntax_error (स्थिर yypcontext_t *yyctx);

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

अटल व्योम
yydestruct (स्थिर अक्षर *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  अगर (!yymsg)
    yymsg = "Deleting";
  YY_प्रतीक_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  चयन (yykind)
    {
    हाल YYSYMBOL_generic_symlist: /* generic_symlist  */
            { प्रतीक_list_free (((*yyvaluep).generic_symlist)); }
        अवरोध;

    हाल YYSYMBOL_generic_symlist_item: /* generic_symlist_item  */
            { प्रतीक_list_free (((*yyvaluep).generic_symlist_item)); }
        अवरोध;

    हाल YYSYMBOL_nterm_decls: /* nterm_decls  */
            { प्रतीक_list_free (((*yyvaluep).nterm_decls)); }
        अवरोध;

    हाल YYSYMBOL_token_decls: /* token_decls  */
            { प्रतीक_list_free (((*yyvaluep).token_decls)); }
        अवरोध;

    हाल YYSYMBOL_82_token_decl_1: /* token_decl.1  */
            { प्रतीक_list_free (((*yyvaluep).yykind_82)); }
        अवरोध;

    हाल YYSYMBOL_token_decls_for_prec: /* token_decls_for_prec  */
            { प्रतीक_list_free (((*yyvaluep).token_decls_for_prec)); }
        अवरोध;

    हाल YYSYMBOL_87_token_decl_for_prec_1: /* token_decl_for_prec.1  */
            { प्रतीक_list_free (((*yyvaluep).yykind_87)); }
        अवरोध;

    हाल YYSYMBOL_प्रतीक_decls: /* symbol_decls  */
            { प्रतीक_list_free (((*yyvaluep).प्रतीक_decls)); }
        अवरोध;

    हाल YYSYMBOL_90_symbols_1: /* symbols.1  */
            { प्रतीक_list_free (((*yyvaluep).yykind_90)); }
        अवरोध;

      शेष:
        अवरोध;
    }
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}






/*----------.
| yyparse.  |
`----------*/

पूर्णांक
yyparse (व्योम)
{
/* Lookahead token kind.  */
पूर्णांक yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_मूल्य (अटल YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_मूल्य (= yyval_default);

/* Location data for the lookahead symbol.  */
अटल YYLTYPE yyloc_default
# अगर defined GRAM_LTYPE_IS_TRIVIAL && GRAM_LTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

    /* Number of syntax errors so far.  */
    पूर्णांक yynerrs YY_ATTRIBUTE_UNUSED = 0;

    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    पूर्णांक yyerrstatus = 0;

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
  पूर्णांक yy_lac_established = 0;
  पूर्णांक yyn;
  /* The return value of yyparse.  */
  पूर्णांक yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];



#घोषणा YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  पूर्णांक yylen = 0;

  YYDPRINTF ((मानक_त्रुटि, "Starting parse\n"));

  yychar = GRAM_EMPTY; /* Cause a token to be read.  */


/* User initialization code.  */
{
  /* Bison's grammar can initial empty locations, hence a default
     location is needed. */
  boundary_set (&yylloc.start, grammar_file, 1, 1, 1);
  boundary_set (&yylloc.end, grammar_file, 1, 1, 1);
}


  yylsp[0] = yylloc;
  जाओ yysetstate;


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
  YYDPRINTF ((मानक_त्रुटि, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_ढेर_PRINT (yyss, yyssp);

  अगर (yyss + yystacksize - 1 <= yyssp)
#अगर !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#अन्यथा
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# अगर defined yyoverflow
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
# अन्यथा /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      अगर (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      अगर (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        जोड़ yyalloc *yyptr =
          YY_CAST (जोड़ yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        अगर (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        अगर (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((मानक_त्रुटि, "Stack size increased to %ld\n",
                  YY_CAST (दीर्घ, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      अगर (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#पूर्ण_अगर /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  अगर (yystate == YYFINAL)
    YYACCEPT;

  जाओ yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  अगर (yypact_मूल्य_is_default (yyn))
    जाओ yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  अगर (yychar == GRAM_EMPTY)
    {
      YYDPRINTF ((मानक_त्रुटि, "Reading a token\n"));
      yychar = yylex (&yylval, &yylloc);
    }

  अगर (yychar <= GRAM_EOF)
    {
      yychar = GRAM_EOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((मानक_त्रुटि, "Now at end of input.\n"));
    }
  अन्यथा अगर (yychar == GRAM_error)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = GRAM_UNDEF;
      yytoken = YYSYMBOL_YYerror;
      yyerror_range[1] = yylloc;
      जाओ yyerrlab1;
    }
  अन्यथा
    {
      yytoken = YYTRANSLATE (yychar);
      YY_प्रतीक_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  अगर (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    {
      YY_LAC_ESTABLISH;
      जाओ yydefault;
    }
  yyn = yytable[yyn];
  अगर (yyn <= 0)
    {
      अगर (yytable_मूल्य_is_error (yyn))
        जाओ yyerrlab;
      yyn = -yyn;
      YY_LAC_ESTABLISH;
      जाओ yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  अगर (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_प्रतीक_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = GRAM_EMPTY;
  YY_LAC_DISCARD ("shift");
  जाओ yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  अगर (yyn == 0)
    जाओ yyerrlab;
  जाओ yyreduce;


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
    पूर्णांक yychar_backup = yychar;
    चयन (yyn)
      {
  हाल 6: /* prologue_declaration: "%{...%}"  */
    {
      muscle_code_grow (union_seen ? "post_prologue" : "pre_prologue",
                        translate_code ((yyvsp[0].PROLOGUE), (yylsp[0]), true), (yylsp[0]));
      code_scanner_last_string_free ();
    }
    अवरोध;

  हाल 7: /* prologue_declaration: "%<flag>"  */
    {
      muscle_percent_define_ensure ((yyvsp[0].PERCENT_FLAG), (yylsp[0]), true);
    }
    अवरोध;

  हाल 8: /* prologue_declaration: "%define" variable value  */
    {
      muscle_percent_define_जोड़ो ((yyvsp[-1].variable), (yyloc), (yyvsp[0].मूल्य).kind, (yyvsp[0].मूल्य).chars,
                                    MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
    }
    अवरोध;

  हाल 9: /* prologue_declaration: "%header" string.opt  */
                                   { handle_header ((yyvsp[0].yykind_75)); }
    अवरोध;

  हाल 10: /* prologue_declaration: "%error-verbose"  */
                                   { handle_error_verbose (&(yyloc), (yyvsp[0].PERCENT_ERROR_VERBOSE)); }
    अवरोध;

  हाल 11: /* prologue_declaration: "%expect" "integer literal"  */
                                   { expected_sr_conflicts = (yyvsp[0].INT_LITERAL); }
    अवरोध;

  हाल 12: /* prologue_declaration: "%expect-rr" "integer literal"  */
                                   { expected_rr_conflicts = (yyvsp[0].INT_LITERAL); }
    अवरोध;

  हाल 13: /* prologue_declaration: "%file-prefix" "string"  */
                                   { handle_file_prefix (&(yyloc), &(yylsp[-1]), (yyvsp[-1].PERCENT_FILE_PREFIX), (yyvsp[0].STRING)); }
    अवरोध;

  हाल 14: /* prologue_declaration: "%glr-parser"  */
    {
      nondeterministic_parser = true;
      glr_parser = true;
    }
    अवरोध;

  हाल 15: /* prologue_declaration: "%initial-action" "{...}"  */
    {
      muscle_code_grow ("initial_action", translate_code ((yyvsp[0].BRACED_CODE), (yylsp[0]), false), (yylsp[0]));
      code_scanner_last_string_free ();
    }
    अवरोध;

  हाल 16: /* prologue_declaration: "%language" "string"  */
                                { handle_language (&(yylsp[-1]), (yyvsp[0].STRING)); }
    अवरोध;

  हाल 17: /* prologue_declaration: "%name-prefix" "string"  */
                                { handle_name_prefix (&(yyloc), (yyvsp[-1].PERCENT_NAME_PREFIX), (yyvsp[0].STRING)); }
    अवरोध;

  हाल 18: /* prologue_declaration: "%no-lines"  */
                                { no_पंक्तियाँ_flag = true; }
    अवरोध;

  हाल 19: /* prologue_declaration: "%nondeterministic-parser"  */
                                { nondeterministic_parser = true; }
    अवरोध;

  हाल 20: /* prologue_declaration: "%output" "string"  */
                                { spec_outfile = unquote ((yyvsp[0].STRING)); gram_scanner_last_string_free (); }
    अवरोध;

  हाल 21: /* $@1: %empty  */
           { current_param = (yyvsp[0].PERCENT_PARAM); }
    अवरोध;

  हाल 22: /* prologue_declaration: "%param" $@1 params  */
                                          { current_param = param_none; }
    अवरोध;

  हाल 23: /* prologue_declaration: "%pure-parser"  */
                                { handle_pure_parser (&(yyloc), (yyvsp[0].PERCENT_PURE_PARSER)); }
    अवरोध;

  हाल 24: /* prologue_declaration: "%require" "string"  */
                                { handle_require (&(yylsp[0]), (yyvsp[0].STRING)); }
    अवरोध;

  हाल 25: /* prologue_declaration: "%skeleton" "string"  */
                                { handle_skeleton (&(yylsp[0]), (yyvsp[0].STRING)); }
    अवरोध;

  हाल 26: /* prologue_declaration: "%token-table"  */
                                { token_table_flag = true; }
    अवरोध;

  हाल 27: /* prologue_declaration: "%verbose"  */
                                { report_flag |= report_states; }
    अवरोध;

  हाल 28: /* prologue_declaration: "%yacc"  */
                                { handle_yacc (&(yyloc)); }
    अवरोध;

  हाल 29: /* prologue_declaration: error ";"  */
                                { current_class = unknown_sym; yyerrok; }
    अवरोध;

  हाल 31: /* params: params "{...}"  */
                   { add_param (current_param, (yyvsp[0].BRACED_CODE), (yylsp[0])); }
    अवरोध;

  हाल 32: /* params: "{...}"  */
                   { add_param (current_param, (yyvsp[0].BRACED_CODE), (yylsp[0])); }
    अवरोध;

  हाल 34: /* grammar_declaration: "%start" symbols.1  */
    {
      grammar_start_symbols_add ((yyvsp[0].yykind_90));
    }
    अवरोध;

  हाल 35: /* grammar_declaration: code_props_type "{...}" generic_symlist  */
    {
      code_props code;
      code_props_प्रतीक_action_init (&code, (yyvsp[-1].BRACED_CODE), (yylsp[-1]));
      code_props_translate_code (&code);
      {
        क्रम (प्रतीक_list *list = (yyvsp[0].generic_symlist); list; list = list->अग्र)
          प्रतीक_list_code_props_set (list, (yyvsp[-2].code_props_type), &code);
        प्रतीक_list_free ((yyvsp[0].generic_symlist));
      }
    }
    अवरोध;

  हाल 36: /* grammar_declaration: "%default-prec"  */
    {
      default_prec = true;
    }
    अवरोध;

  हाल 37: /* grammar_declaration: "%no-default-prec"  */
    {
      default_prec = false;
    }
    अवरोध;

  हाल 38: /* grammar_declaration: "%code" "{...}"  */
    {
      /* Do not invoke muscle_percent_code_grow here since it invokes
         muscle_user_name_list_grow.  */
      muscle_code_grow ("percent_code()",
                        translate_code_braceless ((yyvsp[0].BRACED_CODE), (yylsp[0])), (yylsp[0]));
      code_scanner_last_string_free ();
    }
    अवरोध;

  हाल 39: /* grammar_declaration: "%code" "identifier" "{...}"  */
    {
      muscle_percent_code_grow ((yyvsp[-1].ID), (yylsp[-1]), translate_code_braceless ((yyvsp[0].BRACED_CODE), (yylsp[0])), (yylsp[0]));
      code_scanner_last_string_free ();
    }
    अवरोध;

  हाल 40: /* code_props_type: "%destructor"  */
                 { (yyval.code_props_type) = destructor; }
    अवरोध;

  हाल 41: /* code_props_type: "%printer"  */
                 { (yyval.code_props_type) = printer; }
    अवरोध;

  हाल 42: /* union_name: %empty  */
         {}
    अवरोध;

  हाल 43: /* union_name: "identifier"  */
         { muscle_percent_define_जोड़ो ("api.value.union.name",
                                         (yylsp[0]), muscle_keyword, (yyvsp[0].ID),
                                         MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE); }
    अवरोध;

  हाल 44: /* grammar_declaration: "%union" union_name "{...}"  */
    {
      union_seen = true;
      muscle_code_grow ("union_members", translate_code_braceless ((yyvsp[0].BRACED_CODE), (yylsp[0])), (yylsp[0]));
      code_scanner_last_string_free ();
    }
    अवरोध;

  हाल 45: /* $@2: %empty  */
           { current_class = nterm_sym; }
    अवरोध;

  हाल 46: /* symbol_declaration: "%nterm" $@2 nterm_decls  */
    {
      current_class = unknown_sym;
      प्रतीक_list_free ((yyvsp[0].nterm_decls));
    }
    अवरोध;

  हाल 47: /* $@3: %empty  */
           { current_class = token_sym; }
    अवरोध;

  हाल 48: /* symbol_declaration: "%token" $@3 token_decls  */
    {
      current_class = unknown_sym;
      प्रतीक_list_free ((yyvsp[0].token_decls));
    }
    अवरोध;

  हाल 49: /* $@4: %empty  */
          { current_class = pct_type_sym; }
    अवरोध;

  हाल 50: /* symbol_declaration: "%type" $@4 symbol_decls  */
    {
      current_class = unknown_sym;
      प्रतीक_list_free ((yyvsp[0].प्रतीक_decls));
    }
    अवरोध;

  हाल 51: /* symbol_declaration: precedence_declarator token_decls_for_prec  */
    {
      ++current_prec;
      क्रम (प्रतीक_list *list = (yyvsp[0].token_decls_for_prec); list; list = list->अग्र)
        प्रतीक_precedence_set (list->content.sym, current_prec, (yyvsp[-1].precedence_declarator), (yylsp[-1]));
      प्रतीक_list_free ((yyvsp[0].token_decls_for_prec));
    }
    अवरोध;

  हाल 52: /* precedence_declarator: "%left"  */
                { (yyval.precedence_declarator) = बायाँ_assoc; }
    अवरोध;

  हाल 53: /* precedence_declarator: "%right"  */
                { (yyval.precedence_declarator) = दायाँ_assoc; }
    अवरोध;

  हाल 54: /* precedence_declarator: "%nonassoc"  */
                { (yyval.precedence_declarator) = non_assoc; }
    अवरोध;

  हाल 55: /* precedence_declarator: "%precedence"  */
                { (yyval.precedence_declarator) = precedence_assoc; }
    अवरोध;

  हाल 56: /* string.opt: %empty  */
          { (yyval.yykind_75) = शून्य; }
    अवरोध;

  हाल 57: /* string.opt: "string"  */
          { (yyval.yykind_75) = (yyvsp[0].STRING); }
    अवरोध;

  हाल 58: /* tag.opt: %empty  */
         { (yyval.yykind_76) = शून्य; }
    अवरोध;

  हाल 59: /* tag.opt: "<tag>"  */
         { (yyval.yykind_76) = (yyvsp[0].TAG); }
    अवरोध;

  हाल 61: /* generic_symlist: generic_symlist generic_symlist_item  */
                                         { (yyval.generic_symlist) = प्रतीक_list_append ((yyvsp[-1].generic_symlist), (yyvsp[0].generic_symlist_item)); }
    अवरोध;

  हाल 62: /* generic_symlist_item: symbol  */
            { (yyval.generic_symlist_item) = प्रतीक_list_sym_new ((yyvsp[0].प्रतीक), (yylsp[0])); }
    अवरोध;

  हाल 63: /* generic_symlist_item: tag  */
            { (yyval.generic_symlist_item) = प्रतीक_list_type_new ((yyvsp[0].tag), (yylsp[0])); }
    अवरोध;

  हाल 65: /* tag: "<*>"  */
        { (yyval.tag) = uniqstr_new ("*"); }
    अवरोध;

  हाल 66: /* tag: "<>"  */
        { (yyval.tag) = uniqstr_new (""); }
    अवरोध;

  हाल 68: /* token_decls: token_decl.1  */
    {
      (yyval.token_decls) = (yyvsp[0].yykind_82);
    }
    अवरोध;

  हाल 69: /* token_decls: "<tag>" token_decl.1  */
    {
      (yyval.token_decls) = प्रतीक_list_type_set ((yyvsp[0].yykind_82), (yyvsp[-1].TAG));
    }
    अवरोध;

  हाल 70: /* token_decls: token_decls "<tag>" token_decl.1  */
    {
      (yyval.token_decls) = प्रतीक_list_append ((yyvsp[-2].token_decls), प्रतीक_list_type_set ((yyvsp[0].yykind_82), (yyvsp[-1].TAG)));
    }
    अवरोध;

  हाल 71: /* token_decl.1: token_decl  */
                            { (yyval.yykind_82) = प्रतीक_list_sym_new ((yyvsp[0].token_decl), (yylsp[0])); }
    अवरोध;

  हाल 72: /* token_decl.1: token_decl.1 token_decl  */
                            { (yyval.yykind_82) = प्रतीक_list_append ((yyvsp[-1].yykind_82), प्रतीक_list_sym_new ((yyvsp[0].token_decl), (yylsp[0]))); }
    अवरोध;

  हाल 73: /* token_decl: id int.opt alias  */
    {
      (yyval.token_decl) = (yyvsp[-2].id);
      प्रतीक_class_set ((yyvsp[-2].id), current_class, (yylsp[-2]), true);
      अगर (0 <= (yyvsp[-1].yykind_84))
        प्रतीक_code_set ((yyvsp[-2].id), (yyvsp[-1].yykind_84), (yylsp[-1]));
      अगर ((yyvsp[0].alias))
        प्रतीक_make_alias ((yyvsp[-2].id), (yyvsp[0].alias), (yylsp[0]));
    }
    अवरोध;

  हाल 74: /* int.opt: %empty  */
          { (yyval.yykind_84) = -1; }
    अवरोध;

  हाल 76: /* alias: %empty  */
                 { (yyval.alias) = शून्य; }
    अवरोध;

  हाल 77: /* alias: string_as_id  */
                 { (yyval.alias) = (yyvsp[0].string_as_id); }
    अवरोध;

  हाल 78: /* alias: "translatable string"  */
    {
      (yyval.alias) = प्रतीक_get ((yyvsp[0].TSTRING), (yylsp[0]));
      प्रतीक_class_set ((yyval.alias), token_sym, (yylsp[0]), false);
      (yyval.alias)->translatable = true;
    }
    अवरोध;

  हाल 79: /* token_decls_for_prec: token_decl_for_prec.1  */
    {
      (yyval.token_decls_for_prec) = (yyvsp[0].yykind_87);
    }
    अवरोध;

  हाल 80: /* token_decls_for_prec: "<tag>" token_decl_for_prec.1  */
    {
      (yyval.token_decls_for_prec) = प्रतीक_list_type_set ((yyvsp[0].yykind_87), (yyvsp[-1].TAG));
    }
    अवरोध;

  हाल 81: /* token_decls_for_prec: token_decls_for_prec "<tag>" token_decl_for_prec.1  */
    {
      (yyval.token_decls_for_prec) = प्रतीक_list_append ((yyvsp[-2].token_decls_for_prec), प्रतीक_list_type_set ((yyvsp[0].yykind_87), (yyvsp[-1].TAG)));
    }
    अवरोध;

  हाल 82: /* token_decl_for_prec.1: token_decl_for_prec  */
    { (yyval.yykind_87) = प्रतीक_list_sym_new ((yyvsp[0].token_decl_for_prec), (yylsp[0])); }
    अवरोध;

  हाल 83: /* token_decl_for_prec.1: token_decl_for_prec.1 token_decl_for_prec  */
    { (yyval.yykind_87) = प्रतीक_list_append ((yyvsp[-1].yykind_87), प्रतीक_list_sym_new ((yyvsp[0].token_decl_for_prec), (yylsp[0]))); }
    अवरोध;

  हाल 84: /* token_decl_for_prec: id int.opt  */
    {
      (yyval.token_decl_for_prec) = (yyvsp[-1].id);
      प्रतीक_class_set ((yyvsp[-1].id), token_sym, (yylsp[-1]), false);
      अगर (0 <= (yyvsp[0].yykind_84))
        प्रतीक_code_set ((yyvsp[-1].id), (yyvsp[0].yykind_84), (yylsp[0]));
    }
    अवरोध;

  हाल 86: /* symbol_decls: symbols.1  */
    {
      (yyval.प्रतीक_decls) = (yyvsp[0].yykind_90);
    }
    अवरोध;

  हाल 87: /* symbol_decls: "<tag>" symbols.1  */
    {
      (yyval.प्रतीक_decls) = प्रतीक_list_type_set ((yyvsp[0].yykind_90), (yyvsp[-1].TAG));
    }
    अवरोध;

  हाल 88: /* symbol_decls: symbol_decls "<tag>" symbols.1  */
    {
      (yyval.प्रतीक_decls) = प्रतीक_list_append ((yyvsp[-2].प्रतीक_decls), प्रतीक_list_type_set ((yyvsp[0].yykind_90), (yyvsp[-1].TAG)));
    }
    अवरोध;

  हाल 89: /* symbols.1: symbol  */
    {
      अगर (current_class != unknown_sym)
        प्रतीक_class_set ((yyvsp[0].प्रतीक), current_class, (yylsp[0]), false);
      (yyval.yykind_90) = प्रतीक_list_sym_new ((yyvsp[0].प्रतीक), (yylsp[0]));
    }
    अवरोध;

  हाल 90: /* symbols.1: symbols.1 symbol  */
    {
      अगर (current_class != unknown_sym)
        प्रतीक_class_set ((yyvsp[0].प्रतीक), current_class, (yylsp[0]), false);
      (yyval.yykind_90) = प्रतीक_list_append ((yyvsp[-1].yykind_90), प्रतीक_list_sym_new ((yyvsp[0].प्रतीक), (yylsp[0])));
    }
    अवरोध;

  हाल 95: /* rules_or_grammar_declaration: error ";"  */
    {
      yyerrok;
    }
    अवरोध;

  हाल 96: /* $@5: %empty  */
                         { current_lhs ((yyvsp[-1].id_colon), (yylsp[-1]), (yyvsp[0].yykind_97)); }
    अवरोध;

  हाल 97: /* rules: id_colon named_ref.opt $@5 ":" rhses.1  */
    {
      /* Free the current lhs. */
      current_lhs (0, (yylsp[-4]), 0);
    }
    अवरोध;

  हाल 98: /* rhses.1: rhs  */
                     { grammar_current_rule_end ((yylsp[0])); }
    अवरोध;

  हाल 99: /* rhses.1: rhses.1 "|" rhs  */
                     { grammar_current_rule_end ((yylsp[0])); }
    अवरोध;

  हाल 101: /* rhs: %empty  */
    { grammar_current_rule_begin (current_lhs_प्रतीक, current_lhs_loc,
                                  current_lhs_named_ref); }
    अवरोध;

  हाल 102: /* rhs: rhs symbol named_ref.opt  */
    { grammar_current_rule_प्रतीक_append ((yyvsp[-1].प्रतीक), (yylsp[-1]), (yyvsp[0].yykind_97)); }
    अवरोध;

  हाल 103: /* rhs: rhs tag.opt "{...}" named_ref.opt  */
    { grammar_current_rule_action_append ((yyvsp[-1].BRACED_CODE), (yylsp[-1]), (yyvsp[0].yykind_97), (yyvsp[-2].yykind_76)); }
    अवरोध;

  हाल 104: /* rhs: rhs "%?{...}"  */
    { grammar_current_rule_predicate_append ((yyvsp[0].BRACED_PREDICATE), (yylsp[0])); }
    अवरोध;

  हाल 105: /* rhs: rhs "%empty"  */
    { grammar_current_rule_empty_set ((yylsp[0])); }
    अवरोध;

  हाल 106: /* rhs: rhs "%prec" symbol  */
    { grammar_current_rule_prec_set ((yyvsp[0].प्रतीक), (yylsp[0])); }
    अवरोध;

  हाल 107: /* rhs: rhs "%dprec" "integer literal"  */
    { grammar_current_rule_dprec_set ((yyvsp[0].INT_LITERAL), (yylsp[0])); }
    अवरोध;

  हाल 108: /* rhs: rhs "%merge" "<tag>"  */
    { grammar_current_rule_merge_set ((yyvsp[0].TAG), (yylsp[0])); }
    अवरोध;

  हाल 109: /* rhs: rhs "%expect" "integer literal"  */
    { grammar_current_rule_expect_sr ((yyvsp[0].INT_LITERAL), (yylsp[0])); }
    अवरोध;

  हाल 110: /* rhs: rhs "%expect-rr" "integer literal"  */
    { grammar_current_rule_expect_rr ((yyvsp[0].INT_LITERAL), (yylsp[0])); }
    अवरोध;

  हाल 111: /* named_ref.opt: %empty  */
                 { (yyval.yykind_97) = शून्य; }
    अवरोध;

  हाल 112: /* named_ref.opt: "[identifier]"  */
                 { (yyval.yykind_97) = named_ref_new ((yyvsp[0].BRACKETED_ID), (yylsp[0])); }
    अवरोध;

  हाल 114: /* value: %empty  */
          { (yyval.मूल्य).kind = muscle_keyword; (yyval.मूल्य).chars = ""; }
    अवरोध;

  हाल 115: /* value: "identifier"  */
          { (yyval.मूल्य).kind = muscle_keyword; (yyval.मूल्य).chars = (yyvsp[0].ID); }
    अवरोध;

  हाल 116: /* value: "string"  */
          { (yyval.मूल्य).kind = muscle_string;  (yyval.मूल्य).chars = unquote ((yyvsp[0].STRING)); gram_scanner_last_string_free ();}
    अवरोध;

  हाल 117: /* value: "{...}"  */
          { (yyval.मूल्य).kind = muscle_code;    (yyval.मूल्य).chars = strip_braces ((yyvsp[0].BRACED_CODE)); gram_scanner_last_string_free (); }
    अवरोध;

  हाल 118: /* id: "identifier"  */
    { (yyval.id) = प्रतीक_from_uniqstr ((yyvsp[0].ID), (yylsp[0])); }
    अवरोध;

  हाल 119: /* id: "character literal"  */
    {
      स्थिर अक्षर *var = "api.token.raw";
      अगर (current_class == nterm_sym)
        {
          complain (&(yylsp[0]), complaint,
                    _("character literals cannot be nonterminals"));
          YYERROR;
        }
      अगर (muscle_percent_define_ifdef (var))
        {
          complain (&(yylsp[0]), complaint,
                    _("character literals cannot be used together"
                    " with %s"), var);
          location loc = muscle_percent_define_get_loc (var);
          subcomplain (&loc, complaint, _("definition of %s"), var);
        }
      (yyval.id) = प्रतीक_get (char_name ((yyvsp[0].CHAR_LITERAL)), (yylsp[0]));
      प्रतीक_class_set ((yyval.id), token_sym, (yylsp[0]), false);
      प्रतीक_code_set ((yyval.id), (yyvsp[0].CHAR_LITERAL), (yylsp[0]));
    }
    अवरोध;

  हाल 120: /* id_colon: "identifier:"  */
           { (yyval.id_colon) = प्रतीक_from_uniqstr ((yyvsp[0].ID_COLON), (yylsp[0])); }
    अवरोध;

  हाल 123: /* string_as_id: "string"  */
    {
      (yyval.string_as_id) = प्रतीक_get ((yyvsp[0].STRING), (yylsp[0]));
      प्रतीक_class_set ((yyval.string_as_id), token_sym, (yylsp[0]), false);
    }
    अवरोध;

  हाल 125: /* epilogue.opt: "%%" "epilogue"  */
    {
      muscle_code_grow ("epilogue", translate_code ((yyvsp[0].EPILOGUE), (yylsp[0]), true), (yylsp[0]));
      code_scanner_last_string_free ();
    }
    अवरोध;



        शेष: अवरोध;
      }
    अगर (yychar_backup != yychar)
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
  YY_प्रतीक_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    स्थिर पूर्णांक yylhs = yyr1[yyn] - YYNTOKENS;
    स्थिर पूर्णांक yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  जाओ yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == GRAM_EMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  अगर (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yyesa, &yyes, &yyes_capacity, yytoken, &yylloc};
        अगर (yychar != GRAM_EMPTY)
          YY_LAC_ESTABLISH;
        अगर (yyreport_syntax_error (&yyctx) == 2)
          YYNOMEM;
      }
    }

  yyerror_range[1] = yylloc;
  अगर (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      अगर (yychar <= GRAM_EOF)
        {
          /* Return failure if at end of input.  */
          अगर (yychar == GRAM_EOF)
            YYABORT;
        }
      अन्यथा
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc);
          yychar = GRAM_EMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  जाओ yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  अगर (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_ढेर_PRINT (yyss, yyssp);
  yystate = *yyssp;
  जाओ yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  क्रम (;;)
    {
      yyn = yypact[yystate];
      अगर (!yypact_मूल्य_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          अगर (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              अगर (0 < yyn)
                अवरोध;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      अगर (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_प्रतीक (yystate), yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_ढेर_PRINT (yyss, yyssp);
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
  YY_प्रतीक_PRINT ("Shifting", YY_ACCESSING_प्रतीक (yyn), yyvsp, yylsp);

  yystate = yyn;
  जाओ yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  जाओ yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  जाओ yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, YY_("memory exhausted"));
  yyresult = 2;
  जाओ yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  अगर (yychar != GRAM_EMPTY)
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
  YY_ढेर_PRINT (yyss, yyssp);
  जबतक (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_प्रतीक (+*yyssp), yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#अगर_अघोषित yyoverflow
  अगर (yyss != yyssa)
    YYSTACK_FREE (yyss);
#पूर्ण_अगर
  अगर (yyes != yyesa)
    YYSTACK_FREE (yyes);

  वापस yyresult;
}



पूर्णांक
yyreport_syntax_error (स्थिर yypcontext_t *ctx)
{
  पूर्णांक res = 0;
  /* Arguments of format: reported tokens (one for the "unexpected",
     one per "expected"). */
  क्रमागत { ARGS_MAX = 5 };
  स्थिर अक्षर *argv[ARGS_MAX];
  पूर्णांक argc = 0;
  yysymbol_kind_t unexpected = yypcontext_token (ctx);
  अगर (unexpected != YYSYMBOL_YYEMPTY)
    {
      argv[argc++] = yysymbol_name (unexpected);
      yysymbol_kind_t expected[ARGS_MAX - 1];
      पूर्णांक nexpected = yypcontext_expected_tokens (ctx, expected, ARGS_MAX - 1);
      अगर (nexpected < 0)
        res = nexpected;
      अन्यथा
        क्रम (पूर्णांक i = 0; i < nexpected; ++i)
          argv[argc++] = yysymbol_name (expected[i]);
    }
  syntax_error (*yypcontext_location (ctx), argc, argv);
  वापस res;
}


/* Return the location of the left-hand side of a rule whose
   right-hand side is RHS[1] ... RHS[N].  Ignore empty nonterminals in
   the right-hand side, and return an empty location equal to the end
   boundary of RHS[0] if the right-hand side is empty.  */

अटल YYLTYPE
lloc_default (YYLTYPE स्थिर *rhs, पूर्णांक n)
{
  YYLTYPE loc;

  /* SGI MIPSpro 7.4.1m miscompiles "loc.start = loc.end = rhs[n].end;".
     The bug is fixed in 7.4.2m, but play it safe for now.  */
  loc.start = rhs[n].end;
  loc.end = rhs[n].end;

  /* Ignore empty nonterminals the start of the right-hand side.
     Do not bother to ignore them at the end of the right-hand side,
     since empty nonterminals have the same end as their predecessors.  */
  क्रम (पूर्णांक i = 1; i <= n; i++)
    अगर (! equal_boundaries (rhs[i].start, rhs[i].end))
      {
        loc.start = rhs[i].start;
        अवरोध;
      }

  वापस loc;
}

अटल
अक्षर *strip_braces (अक्षर *code)
{
  code[म_माप (code) - 1] = 0;
  वापस code + 1;
}

अटल
अक्षर स्थिर *
translate_code (अक्षर *code, location loc, bool plain)
{
  code_props plain_code;
  अगर (plain)
    code_props_plain_init (&plain_code, code, loc);
  अन्यथा
    code_props_प्रतीक_action_init (&plain_code, code, loc);
  code_props_translate_code (&plain_code);
  gram_scanner_last_string_free ();
  वापस plain_code.code;
}

अटल
अक्षर स्थिर *
translate_code_braceless (अक्षर *code, location loc)
{
  वापस translate_code (strip_braces (code), loc, true);
}

अटल व्योम
add_param (param_type type, अक्षर *decl, location loc)
{
  अटल अक्षर स्थिर alphanum[26 + 26 + 1 + 10 + 1] =
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "_"
    "0123456789";

  अक्षर स्थिर *name_start = शून्य;
  {
    अक्षर *p;
    /* Stop on last actual character.  */
    क्रम (p = decl; p[1]; p++)
      अगर ((p == decl
           || ! स_प्रथम (alphanum, p[-1], माप alphanum - 1))
          && स_प्रथम (alphanum, p[0], माप alphanum - 10 - 1))
        name_start = p;

    /* Strip the surrounding '{' and '}', and any blanks just inside
       the braces.  */
    --p;
    जबतक (c_isspace ((अचिन्हित अक्षर) *p))
      --p;
    p[1] = '\0';
    ++decl;
    जबतक (c_isspace ((अचिन्हित अक्षर) *decl))
      ++decl;
  }

  अगर (! name_start)
    complain (&loc, complaint, _("missing identifier in parameter declaration"));
  अन्यथा
    {
      अक्षर *name = xmemdup0 (name_start, म_अखोज (name_start, alphanum));
      अगर (type & param_lex)
        muscle_pair_list_grow ("lex_param", decl, name);
      अगर (type & param_parse)
        muscle_pair_list_grow ("parse_param", decl, name);
      मुक्त (name);
    }

  gram_scanner_last_string_free ();
}


अटल व्योम
handle_header (अक्षर स्थिर *मूल्य)
{
  header_flag = true;
  अगर (मूल्य)
    {
      अक्षर *file = unquote (मूल्य);
      spec_header_file = xstrdup (file);
      gram_scanner_last_string_free ();
      unquote_free (file);
    }
}


अटल व्योम
handle_error_verbose (location स्थिर *loc, अक्षर स्थिर *directive)
{
  bison_directive (loc, directive);
  muscle_percent_define_जोड़ो (directive, *loc, muscle_keyword, "",
                                MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
}


अटल व्योम
handle_file_prefix (location स्थिर *loc,
                    location स्थिर *dir_loc,
                    अक्षर स्थिर *directive, अक्षर स्थिर *मूल्य_quoted)
{
  अक्षर *मूल्य = unquote (मूल्य_quoted);
  bison_directive (loc, directive);
  bool warned = false;

  अगर (location_empty (spec_file_prefix_loc))
    {
      spec_file_prefix_loc = *loc;
      spec_file_prefix = मूल्य;
    }
  अन्यथा
    {
      duplicate_directive (directive, spec_file_prefix_loc, *loc);
      warned = true;
    }

  अगर (!warned
      && STRNEQ (directive, "%file-prefix"))
    deprecated_directive (dir_loc, directive, "%file-prefix");
}

अटल व्योम
handle_language (location स्थिर *loc, अक्षर स्थिर *lang)
{
  language_argmatch (unquote (lang), grammar_prio, *loc);
}


अटल व्योम
handle_name_prefix (location स्थिर *loc,
                    अक्षर स्थिर *directive, अक्षर स्थिर *मूल्य_quoted)
{
  अक्षर *मूल्य = unquote (मूल्य_quoted);
  bison_directive (loc, directive);

  अक्षर buf1[1024];
  माप_प्रकार len1 = माप (buf1);
  अक्षर *old = asnprintf (buf1, &len1, "%s\"%s\"", directive, मूल्य);
  अगर (!old)
    xalloc_die ();

  अगर (location_empty (spec_name_prefix_loc))
    {
      spec_name_prefix = मूल्य;
      spec_name_prefix_loc = *loc;

      अक्षर buf2[1024];
      माप_प्रकार len2 = माप (buf2);
      अक्षर *new = asnprintf (buf2, &len2, "%%define api.prefix {%s}", मूल्य);
      अगर (!new)
        xalloc_die ();
      deprecated_directive (loc, old, new);
      अगर (new != buf2)
        मुक्त (new);
    }
  अन्यथा
    duplicate_directive (old, spec_file_prefix_loc, *loc);

  अगर (old != buf1)
    मुक्त (old);
}


अटल व्योम
handle_pure_parser (location स्थिर *loc, अक्षर स्थिर *directive)
{
  bison_directive (loc, directive);
  deprecated_directive (loc, directive, "%define api.pure");
  muscle_percent_define_जोड़ो ("api.pure", *loc, muscle_keyword, "",
                                MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
}


अटल व्योम
handle_require (location स्थिर *loc, अक्षर स्थिर *version_quoted)
{
  अक्षर *version = unquote (version_quoted);
  required_version = strversion_to_int (version);
  अगर (required_version == -1)
    {
      complain (loc, complaint, _("invalid version requirement: %s"),
                version);
      required_version = 0;
    }
  अन्यथा
    {
      स्थिर अक्षर* package_version =
        0 < strverscmp (api_version, PACKAGE_VERSION)
        ? api_version : PACKAGE_VERSION;
      अगर (0 < strverscmp (version, package_version))
        {
          complain (loc, complaint, _("require bison %s, but have %s"),
                    version, package_version);
          निकास (EX_MISMATCH);
        }
    }
  unquote_free (version);
  gram_scanner_last_string_free ();
}

अटल व्योम
handle_skeleton (location स्थिर *loc, अक्षर स्थिर *skel_quoted)
{
  अक्षर *skel = unquote (skel_quoted);
  अक्षर स्थिर *skeleton_user = skel;
  अगर (म_अक्षर (skeleton_user, '/'))
    {
      माप_प्रकार dir_लंबाई = म_माप (grammar_file);
      जबतक (dir_लंबाई && grammar_file[dir_लंबाई - 1] != '/')
        --dir_लंबाई;
      जबतक (dir_लंबाई && grammar_file[dir_लंबाई - 1] == '/')
        --dir_लंबाई;
      अक्षर *skeleton_build =
        xmalloc (dir_लंबाई + 1 + म_माप (skeleton_user) + 1);
      अगर (dir_लंबाई > 0)
        {
          स_नकल (skeleton_build, grammar_file, dir_लंबाई);
          skeleton_build[dir_लंबाई++] = '/';
        }
      म_नकल (skeleton_build + dir_लंबाई, skeleton_user);
      skeleton_user = uniqstr_new (skeleton_build);
      मुक्त (skeleton_build);
    }
  skeleton_arg (skeleton_user, grammar_prio, *loc);
}


अटल व्योम
handle_yacc (location स्थिर *loc)
{
  स्थिर अक्षर *directive = "%yacc";
  bison_directive (loc, directive);
  अगर (location_empty (yacc_loc))
    set_yacc (*loc);
  अन्यथा
    duplicate_directive (directive, yacc_loc, *loc);
}


अटल व्योम
gram_error (location स्थिर *loc, अक्षर स्थिर *संदेश)
{
  complain (loc, complaint, "%s", संदेश);
}

अटल अक्षर स्थिर *
char_name (अक्षर c)
{
  अगर (c == '\'')
    वापस "'\\''";
  अन्यथा
    {
      अक्षर buf[4];
      buf[0] = '\''; buf[1] = c; buf[2] = '\''; buf[3] = '\0';
      वापस quotearg_style (escape_quoting_style, buf);
    }
}

अटल व्योम
current_lhs (प्रतीक *sym, location loc, named_ref *ref)
{
  current_lhs_प्रतीक = sym;
  current_lhs_loc = loc;
  अगर (sym)
    प्रतीक_location_as_lhs_set (sym, loc);
  /* In order to simplify memory management, named references for lhs
     are always assigned by deep copy into the current symbol_list
     node.  This is because a single named-ref in the grammar may
     result in several uses when the user factors lhs between several
     rules using "|".  Therefore free the parser's original copy.  */
  मुक्त (current_lhs_named_ref);
  current_lhs_named_ref = ref;
}

अटल व्योम tron (खाता *yyo)
{
  begin_use_class ("value", yyo);
}

अटल व्योम troff (खाता *yyo)
{
  end_use_class ("value", yyo);
}


/*----------.
| Unquote.  |
`----------*/

काष्ठा obstack obstack_for_unquote;

व्योम
parser_init (व्योम)
{
  obstack_init (&obstack_for_unquote);
}

व्योम
parser_free (व्योम)
{
  obstack_free (&obstack_for_unquote, 0);
}

अटल व्योम
unquote_free (अक्षर *last_string)
{
  obstack_free (&obstack_for_unquote, last_string);
}

अटल अक्षर *
unquote (स्थिर अक्षर *cp)
{
#घोषणा GROW(Char)                              \
  obstack_1grow (&obstack_for_unquote, Char);
  क्रम (++cp; *cp && *cp != '"'; ++cp)
    चयन (*cp)
      {
      हाल '"':
        अवरोध;
      हाल '\\':
        ++cp;
        चयन (*cp)
          {
          हाल '0': हाल '1': हाल '2': हाल '3': हाल '4':
          हाल '5': हाल '6': हाल '7': हाल '8': हाल '9':
            {
              पूर्णांक c = cp[0] - '0';
              अगर (c_isdigit (cp[1]))
                {
                  ++cp;
                  c = c * 8 + cp[0] - '0';
                }
              अगर (c_isdigit (cp[1]))
                {
                  ++cp;
                  c = c * 8 + cp[0] - '0';
                }
              GROW (c);
            }
            अवरोध;

          हाल 'a': GROW ('\a'); अवरोध;
          हाल 'b': GROW ('\b'); अवरोध;
          हाल 'f': GROW ('\f'); अवरोध;
          हाल 'n': GROW ('\n'); अवरोध;
          हाल 'r': GROW ('\r'); अवरोध;
          हाल 't': GROW ('\t'); अवरोध;
          हाल 'v': GROW ('\v'); अवरोध;

          हाल 'x':
            {
              पूर्णांक c = 0;
              जबतक (c_isxdigit (cp[1]))
                {
                  ++cp;
                  c = (c * 16 + (c_isdigit (cp[0]) ? cp[0] - '0'
                                 : c_isupper (cp[0]) ? cp[0] - 'A'
                                 : cp[0] - '0'));
                }
              GROW (c);
              अवरोध;
            }
          }
        अवरोध;

      शेष:
        GROW (*cp);
        अवरोध;
      }
  निश्चित (*cp == '"');
  ++cp;
  निश्चित (*cp == '\0');
#अघोषित GROW
  वापस obstack_finish0 (&obstack_for_unquote);
}
