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
#परिभाषय YYBISON 30802

/* Bison version string.  */
#परिभाषय YYBISON_संस्करणम् "3.8.2.30-82269"

/* Skeleton name.  */
#परिभाषय YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#परिभाषय YYPURE 2

/* Push parsers.  */
#परिभाषय YYPUSH 0

/* Pull parsers.  */
#परिभाषय YYPULL 1

/* "%code top" blocks.  */

  /* On column 0 to please syntax-check.  */
#अन्तर्भाव <config.h>

/* Substitute the type names.  */
#परिभाषय YYSTYPE         GRAM_STYPE
#परिभाषय YYLTYPE         GRAM_LTYPE
/* Substitute the variable and function names.  */
#परिभाषय yyparse         gram_parse
#परिभाषय yylex           gram_lex
#परिभाषय yyerror         gram_error
#परिभाषय yydebug         gram_debug
#परिभाषय yynerrs         gram_nerrs


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
#  यदि defined __cplusplus
#   यदि 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   अन्यथा
#    define YY_NULLPTR 0
#   endif
#  अन्यथा
#   define YY_NULLPTR ((शून्य*)0)
#  endif
# endif

#अन्तर्भाव "src/parse-gram.h"
/* Symbol kind.  */
गणना yysymbol_विधा_t
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
  YYSYMBOL_PERCENT_वामः = 10,              /* "%left"  */
  YYSYMBOL_PERCENT_दक्षिणः = 11,             /* "%right"  */
  YYSYMBOL_PERCENT_NONASSOC = 12,          /* "%nonassoc"  */
  YYSYMBOL_PERCENT_PRECEDENCE = 13,        /* "%precedence"  */
  YYSYMBOL_PERCENT_PREC = 14,              /* "%prec"  */
  YYSYMBOL_PERCENT_DPREC = 15,             /* "%dprec"  */
  YYSYMBOL_PERCENT_MERGE = 16,             /* "%merge"  */
  YYSYMBOL_PERCENT_संहिता = 17,              /* "%code"  */
  YYSYMBOL_PERCENT_DEFAULT_PREC = 18,      /* "%default-prec"  */
  YYSYMBOL_PERCENT_DEFINE = 19,            /* "%define"  */
  YYSYMBOL_PERCENT_ERROR_VERBOSE = 20,     /* "%error-verbose"  */
  YYSYMBOL_PERCENT_EXPECT = 21,            /* "%expect"  */
  YYSYMBOL_PERCENT_EXPECT_RR = 22,         /* "%expect-rr"  */
  YYSYMBOL_PERCENT_FILE_उपसर्गः = 23,       /* "%file-prefix"  */
  YYSYMBOL_PERCENT_FLAG = 24,              /* "%<flag>"  */
  YYSYMBOL_PERCENT_GLR_PARSER = 25,        /* "%glr-parser"  */
  YYSYMBOL_PERCENT_शीर्षकम् = 26,            /* "%header"  */
  YYSYMBOL_PERCENT_INITIAL_ACTION = 27,    /* "%initial-action"  */
  YYSYMBOL_PERCENT_LANGUAGE = 28,          /* "%language"  */
  YYSYMBOL_PERCENT_NAME_उपसर्गः = 29,       /* "%name-prefix"  */
  YYSYMBOL_PERCENT_NO_DEFAULT_PREC = 30,   /* "%no-default-prec"  */
  YYSYMBOL_PERCENT_NO_पङ्क्तयः = 31,          /* "%no-lines"  */
  YYSYMBOL_PERCENT_NONDETERMINISTIC_PARSER = 32, /* "%nondeterministic-parser"  */
  YYSYMBOL_PERCENT_OUTPUT = 33,            /* "%output"  */
  YYSYMBOL_PERCENT_विशुद्धम्_PARSER = 34,       /* "%pure-parser"  */
  YYSYMBOL_PERCENT_REQUIRE = 35,           /* "%require"  */
  YYSYMBOL_PERCENT_SKELETON = 36,          /* "%skeleton"  */
  YYSYMBOL_PERCENT_START = 37,             /* "%start"  */
  YYSYMBOL_PERCENT_TOKEN_सारणी = 38,       /* "%token-table"  */
  YYSYMBOL_PERCENT_VERBOSE = 39,           /* "%verbose"  */
  YYSYMBOL_PERCENT_YACC = 40,              /* "%yacc"  */
  YYSYMBOL_BRACED_संहिता = 41,               /* "{...}"  */
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
  YYSYMBOL_चिह्नकम् = 54,                       /* "<tag>"  */
  YYSYMBOL_चिह्नकम्_ANY = 55,                   /* "<*>"  */
  YYSYMBOL_चिह्नकम्_NONE = 56,                  /* "<>"  */
  YYSYMBOL_INT_LITERAL = 57,               /* "integer literal"  */
  YYSYMBOL_PERCENT_PARAM = 58,             /* "%param"  */
  YYSYMBOL_PERCENT_UNION = 59,             /* "%union"  */
  YYSYMBOL_PERCENT_रिक्तानि = 60,             /* "%empty"  */
  YYSYMBOL_YYACCEPT = 61,                  /* $accept  */
  YYSYMBOL_निवेशः = 62,                     /*निवेशः*/
  YYSYMBOL_prologue_declarations = 63,     /* prologue_declarations  */
  YYSYMBOL_prologue_declaration = 64,      /* prologue_declaration  */
  YYSYMBOL_65_1 = 65,                      /* $@1  */
  YYSYMBOL_params = 66,                    /* params  */
  YYSYMBOL_grammar_declaration = 67,       /* grammar_declaration  */
  YYSYMBOL_संहिता_props_type = 68,           /* code_props_type  */
  YYSYMBOL_union_name = 69,                /* union_name  */
  YYSYMBOL_चिह्नम्_declaration = 70,        /* symbol_declaration  */
  YYSYMBOL_71_2 = 71,                      /* $@2  */
  YYSYMBOL_72_3 = 72,                      /* $@3  */
  YYSYMBOL_73_4 = 73,                      /* $@4  */
  YYSYMBOL_precedence_declarator = 74,     /* precedence_declarator  */
  YYSYMBOL_75_string_opt = 75,             /* string.opt  */
  YYSYMBOL_76_चिह्नकम्_opt = 76,                /* tag.opt  */
  YYSYMBOL_generic_symlist = 77,           /* generic_symlist  */
  YYSYMBOL_generic_symlist_item = 78,      /* generic_symlist_item  */
  YYSYMBOL_चिह्नकम्_2 = 79,                       /*चिह्नकम्*/
  YYSYMBOL_nterm_decls = 80,               /* nterm_decls  */
  YYSYMBOL_token_decls = 81,               /* token_decls  */
  YYSYMBOL_82_token_decl_1 = 82,           /* token_decl.1  */
  YYSYMBOL_token_decl = 83,                /* token_decl  */
  YYSYMBOL_84_int_opt = 84,                /* int.opt  */
  YYSYMBOL_alias = 85,                     /* alias  */
  YYSYMBOL_token_decls_for_prec = 86,      /* token_decls_for_prec  */
  YYSYMBOL_87_token_decl_for_prec_1 = 87,  /* token_decl_for_prec.1  */
  YYSYMBOL_token_decl_for_prec = 88,       /* token_decl_for_prec  */
  YYSYMBOL_चिह्नम्_decls = 89,              /* symbol_decls  */
  YYSYMBOL_90_symbols_1 = 90,              /* symbols.1  */
  YYSYMBOL_grammar = 91,                   /* grammar  */
  YYSYMBOL_नियमाः_or_grammar_declaration = 92, /* rules_or_grammar_declaration  */
  YYSYMBOL_नियमाः = 93,                     /*नियमाः*/
  YYSYMBOL_94_5 = 94,                      /* $@5  */
  YYSYMBOL_95_rhses_1 = 95,                /* rhses.1  */
  YYSYMBOL_rhs = 96,                       /* rhs  */
  YYSYMBOL_97_named_ref_opt = 97,          /* named_ref.opt  */
  YYSYMBOL_variable = 98,                  /* variable  */
  YYSYMBOL_मानम् = 99,                     /*मानम्*/
  YYSYMBOL_id = 100,                       /* id  */
  YYSYMBOL_id_colon = 101,                 /* id_colon  */
  YYSYMBOL_चिह्नम् = 102,                   /*चिह्नम्*/
  YYSYMBOL_string_as_id = 103,             /* string_as_id  */
  YYSYMBOL_104_epilogue_opt = 104          /* epilogue.opt  */
};
प्रकारनिर्देश गणना yysymbol_विधा_t yysymbol_विधा_t;



/* Unqualified %code blocks.  */

  #अन्तर्भाव "system.h"

  #अन्तर्भाव <c-ctype.h>
  #अन्तर्भाव <quotearg.h>
  #अन्तर्भाव <vasnprintf.h>
  #अन्तर्भाव <xmemdup0.h>

  #अन्तर्भाव "complain.h"
  #अन्तर्भाव "conflicts.h"
  #अन्तर्भाव "files.h"
  #अन्तर्भाव "getargs.h"
  #अन्तर्भाव "gram.h"
  #अन्तर्भाव "named-ref.h"
  #अन्तर्भाव "reader.h"
  #अन्तर्भाव "scan-code.h"
  #अन्तर्भाव "scan-gram.h"
  #अन्तर्भाव "strversion.h"

  /* Pretend to be at least that version, to check features published
     in that version while developping it.  */
  स्थिर ध्रुव वर्ण* api_संस्करणम् = "3.8";

  स्थिर पूर्णाङ्क current_prec = 0;
  स्थिर location current_lhs_loc;
  स्थिर named_ref *current_lhs_named_ref;
  स्थिर चिह्नम् *current_lhs_चिह्नम्;
  स्थिर चिह्नम्_class current_class = unknown_sym;

  /** Set the new current left-hand side symbol, possibly common
   * to several right-hand side parts of rule.
   */
  स्थिर शून्य current_lhs (चिह्नम् *sym, location loc, named_ref *ref);

  #परिभाषय YYLLOC_DEFAULT(Current, Rhs, N)         \
    (Current) = lloc_default (Rhs, N)
  स्थिर YYLTYPE lloc_default (YYLTYPE ध्रुव *, पूर्णाङ्क);

  #परिभाषय YY_LOCATION_PRINT(File, Loc)            \
    location_print (Loc, File)

  /* Strip initial '{' and final '}' (must be first and last characters).
     Return the result.  */
  स्थिर वर्ण *strip_braces (वर्ण *संहिता);

  /* Convert CODE by calling code_props_plain_init if PLAIN, otherwise
     code_props_symbol_action_init.  Calls
     gram_scanner_last_string_free to release the latest string from
     the scanner (should be CODE). */
  स्थिर वर्ण ध्रुव *translate_संहिता (वर्ण *संहिता, location loc, bool plain);

  /* Convert CODE by calling code_props_plain_init after having
     stripped the first and last characters (expected to be '{', and
     '}').  Calls gram_scanner_last_string_free to release the latest
     string from the scanner (should be CODE). */
  स्थिर वर्ण ध्रुव *translate_संहिता_braceless (वर्ण *संहिता, location loc);

  /* Handle a %header directive.  */
  स्थिर शून्य handle_शीर्षकम् (वर्ण ध्रुव *मानम्);

  /* Handle a %error-verbose directive.  */
  स्थिर शून्य handle_error_verbose (location ध्रुव *loc, वर्ण ध्रुव *directive);

  /* Handle a %file-prefix directive.  */
  स्थिर शून्य handle_file_उपसर्गः (location ध्रुव *loc,
                                  location ध्रुव *dir_loc,
                                  वर्ण ध्रुव *directive, वर्ण ध्रुव *मानम्);

  /* Handle a %language directive.  */
  स्थिर शून्य handle_language (location ध्रुव *loc, वर्ण ध्रुव *lang);

  /* Handle a %name-prefix directive.  */
  स्थिर शून्य handle_name_उपसर्गः (location ध्रुव *loc,
                                  वर्ण ध्रुव *directive, वर्ण ध्रुव *मानम्);

  /* Handle a %pure-parser directive.  */
  स्थिर शून्य handle_विशुद्धम्_parser (location ध्रुव *loc, वर्ण ध्रुव *directive);

  /* Handle a %require directive.  */
  स्थिर शून्य handle_require (location ध्रुव *loc, वर्ण ध्रुव *संस्करणम्);

  /* Handle a %skeleton directive.  */
  स्थिर शून्य handle_skeleton (location ध्रुव *loc, वर्ण ध्रुव *skel);

  /* Handle a %yacc directive.  */
  स्थिर शून्य handle_yacc (location ध्रुव *loc);

  /* Implementation of yyerror.  */
  स्थिर शून्य gram_error (location ध्रुव *, वर्ण ध्रुव *);

  /* A string that describes a char (e.g., 'a' -> "'a'").  */
  स्थिर वर्ण ध्रुव *char_name (वर्ण);

  /* Add style to semantic values in traces.  */
  स्थिर शून्य tron (FILE *yyo);
  स्थिर शून्य troff (FILE *yyo);

  /* Interpret a quoted string (such as `"Hello, \"World\"\n\""`).
     Manages the memory of the result.  */
  स्थिर वर्ण *unquote (ध्रुव वर्ण *str);

  /* Discard the latest unquoted string.  */
  स्थिर शून्य unquote_free (वर्ण *चरमः_string);

  /** Add a lex-param and/or a parse-param.
   *
   * \param type  where to push this formal argument.
   * \param decl  the formal argument.  Destroyed.
   * \param loc   the location in the source.
   */
  स्थिर शून्य सङ्कलय_param (param_type type, वर्ण *decl, location loc);
  स्थिर param_type current_param = param_none;


#यदि_परिभाषितम् ह्रस्व
# undef ह्रस्व
#यदि_अन्तः

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#यदि_अपरिभाषितम् __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# यदि defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#यदि_अन्तः

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#यदि_परिभाषितम् __INT_LEAST8_MAX__
प्रकारनिर्देश __INT_LEAST8_TYPE__ yytype_int8;
#नोचेद्यदि defined YY_STDINT_H
प्रकारनिर्देश int_least8_t yytype_int8;
#अन्यथा
प्रकारनिर्देश चिह्नित वर्ण yytype_int8;
#यदि_अन्तः

#यदि_परिभाषितम् __INT_LEAST16_MAX__
प्रकारनिर्देश __INT_LEAST16_TYPE__ yytype_int16;
#नोचेद्यदि defined YY_STDINT_H
प्रकारनिर्देश int_least16_t yytype_int16;
#अन्यथा
प्रकारनिर्देश ह्रस्व yytype_int16;
#यदि_अन्तः

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#यदि_परिभाषितम् __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#यदि_अन्तः

#यदि defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
प्रकारनिर्देश __UINT_LEAST8_TYPE__ yytype_uint8;
#नोचेद्यदि (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= अधिकतम_पूर्णाङ्कः)
प्रकारनिर्देश uint_least8_t yytype_uint8;
#नोचेद्यदि !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= अधिकतम_पूर्णाङ्कः
प्रकारनिर्देश अचिह्नित वर्ण yytype_uint8;
#अन्यथा
प्रकारनिर्देश ह्रस्व yytype_uint8;
#यदि_अन्तः

#यदि defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
प्रकारनिर्देश __UINT_LEAST16_TYPE__ yytype_uint16;
#नोचेद्यदि (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= अधिकतम_पूर्णाङ्कः)
प्रकारनिर्देश uint_least16_t yytype_uint16;
#नोचेद्यदि !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= अधिकतम_पूर्णाङ्कः
प्रकारनिर्देश अचिह्नित ह्रस्व yytype_uint16;
#अन्यथा
प्रकारनिर्देश पूर्णाङ्क yytype_uint16;
#यदि_अन्तः

#यदि_अपरिभाषितम् YYPTRDIFF_T
# यदि defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_अधिकतमम् __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_अधिकतमम् PTRDIFF_MAX
# अन्यथा
#  define YYPTRDIFF_T दीर्घ
#  define YYPTRDIFF_अधिकतमम् LONG_MAX
# endif
#यदि_अन्तः

#यदि_अपरिभाषितम् YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# अन्यथा
#  define YYSIZE_T अचिह्नित
# endif
#यदि_अन्तः

#परिभाषय YYSIZE_अधिकतमम्                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_अधिकतमम् < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_अधिकतमम्                         \
            : YY_CAST (YYSIZE_T, -1)))

#परिभाषय YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, परिमाण (X))


/* Stored state numbers (used for stacks). */
प्रकारनिर्देश yytype_uint8 yy_state_t;

/* State numbers in computations.  */
प्रकारनिर्देश पूर्णाङ्क yy_state_fast_t;

#यदि_अपरिभाषितम् YY_
# यदि defined YYENABLE_NLS && YYENABLE_NLS
#  यदि ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#यदि_अन्तः

#यदि_अपरिभाषितम् N_
# define N_(Msgid) Msgid
#यदि_अन्तः


#यदि_अपरिभाषितम् YY_ATTRIBUTE_विशुद्धम्
# यदि defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_विशुद्धम् __attribute__ ((__pure__))
# अन्यथा
#  define YY_ATTRIBUTE_विशुद्धम्
# endif
#यदि_अन्तः

#यदि_अपरिभाषितम् YY_ATTRIBUTE_अप्रयुक्तम्
# यदि defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_अप्रयुक्तम् __attribute__ ((__unused__))
# अन्यथा
#  define YY_ATTRIBUTE_अप्रयुक्तम्
# endif
#यदि_अन्तः

/* Suppress unused-variable warnings by "using" E.  */
#यदि ! defined lint || defined __GNUC__
# define YY_USE(E) ((शून्य) (E))
#अन्यथा
# define YY_USE(E) /*रिक्तानि*/
#यदि_अन्तः

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#यदि defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# यदि __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# अन्यथा
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_अन्तः      \
    _Pragma ("GCC diagnostic pop")
#अन्यथा
# define YY_INITIAL_मानम्(मानम्_4) मानम्_4
#यदि_अन्तः
#यदि_अपरिभाषितम् YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_अन्तः
#यदि_अन्तः
#यदि_अपरिभाषितम् YY_INITIAL_मानम्
# define YY_INITIAL_मानम्(मानम्_4) /* Nothing. */
#यदि_अन्तः

#यदि defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_अन्तः            \
    _Pragma ("GCC diagnostic pop")
#यदि_अन्तः
#यदि_अपरिभाषितम् YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_अन्तः
#यदि_अन्तः


#परिभाषय YY_ASSERT(E) ((शून्य) (0 && (E)))

#यदि 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) कुरु { /*रिक्तानि*/; } यावत् (0)
#  ifndef YYSTACK_ALLOC_अधिकतमम्
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_अधिकतमम् 4032 /* reasonable circa 2006 */
#  endif
# अन्यथा
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_अधिकतमम्
#   define YYSTACK_ALLOC_अधिकतमम् YYSIZE_अधिकतमम्
#  endif
#  यदि (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined स्मृतिं_देहि) \
             && (defined YYFREE || defined मुक्त)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC स्मृतिं_देहि
#   यदि ! defined स्मृतिं_देहि && ! defined EXIT_SUCCESS
शून्य *स्मृतिं_देहि (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE मुक्त
#   यदि ! defined मुक्त && ! defined EXIT_SUCCESS
शून्य मुक्त (शून्य *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
# define YYCOPY_NEEDED 1
#यदि_अन्तः /* 1 */

#यदि (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined GRAM_LTYPE_IS_TRIVIAL && GRAM_LTYPE_IS_TRIVIAL \
             && defined GRAM_STYPE_IS_TRIVIAL && GRAM_STYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
सङ्घ yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_अधिकतमम् (YYSIZEOF (सङ्घ yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_अष्टकानि(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_अधिकतमम्)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(राशिः_alloc, राशिः)                           \
    कुरु                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->राशिः_alloc, राशिः, yysize);                    \
        राशिः = &yyptr->राशिः_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*राशिः) + YYSTACK_GAP_अधिकतमम्; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    यावत् (0)

#यदि_अन्तः

#यदि defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  यदि defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, गणनम्_2) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (गणनम्_2)) * परिमाण (*(Src)))
#  अन्यथा
#   define YYCOPY(Dst, Src, गणनम्_2)              \
      कुरु                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          प्रति (yyi = 0; yyi < (गणनम्_2); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      यावत् (0)
#  endif
# endif
#यदि_अन्तः /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#परिभाषय YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#परिभाषय YYLAST   240

/* YYNTOKENS -- Number of terminals.  */
#परिभाषय YYNTOKENS  61
/* YYNNTS -- Number of nonterminals.  */
#परिभाषय YYNNTS  44
/* YYNRULES -- Number of rules.  */
#परिभाषय YYNRULES  125
/* YYNSTATES -- Number of states.  */
#परिभाषय YYNSTATES  169

/* YYMAXUTOK -- Last valid token kind.  */
#परिभाषय YYMAXUTOK   315


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#परिभाषय YYTRANSLATE(YYX) YY_CAST (yysymbol_विधा_t, YYX)

#यदि GRAM_DEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
स्थिर ध्रुव yytype_int16 yyrline[] =
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
#यदि_अन्तः

/** Accessing symbol of state STATE.  */
#परिभाषय YY_ACCESSING_चिह्नम्(State) YY_CAST (yysymbol_विधा_t, yystos[State])

#यदि 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
स्थिर ध्रुव वर्ण *yysymbol_name (yysymbol_विधा_t yysymbol) YY_ATTRIBUTE_अप्रयुक्तम्;

स्थिर ध्रुव वर्ण *
yysymbol_name (yysymbol_विधा_t yysymbol)
{
  स्थिर ध्रुव वर्ण *ध्रुव yy_sname[] =
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
  स्थिर yytype_int8 yytranslatable[] =
  {
       1,     1,     1,     1,     1,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     1,     1,     0,     1,     0,     1,     1,
       0,     0,     0,     0,     1,     0,     0,     1,     0,     0,
       0
  };
  प्रत्यागम (yysymbol < YYNTOKENS && yytranslatable[yysymbol]
          ? _(yy_sname[yysymbol])
          : yy_sname[yysymbol]);
}
#यदि_अन्तः

#परिभाषय YYPACT_NINF (-148)

#परिभाषय yypact_मानम्_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#परिभाषय YYTABLE_NINF (-125)

#परिभाषय yytable_मानम्_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
स्थिर ध्रुव yytype_int16 yypact[] =
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
स्थिर ध्रुव yytype_int8 yydefact[] =
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
स्थिर ध्रुव yytype_int16 yypgoto[] =
{
    -148,  -148,  -148,  -148,  -148,  -148,   109,  -148,  -148,  -148,
    -148,  -148,  -148,  -148,  -148,  -148,  -148,    -7,  -148,  -148,
      66,   -73,   -87,    60,  -148,  -148,   -71,   -83,  -148,   -50,
    -148,    51,  -148,  -148,  -148,   -19,  -147,  -148,  -148,   -47,
    -148,   -48,   -38,  -148
};

/* YYDEFGOTO[NTERM-NUM].  */
स्थिर ध्रुव yytype_uint8 yydefgoto[] =
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
स्थिर ध्रुव yytype_int16 yytable[] =
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

स्थिर ध्रुव yytype_int16 yycheck[] =
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
स्थिर ध्रुव yytype_int8 yystos[] =
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
स्थिर ध्रुव yytype_int8 yyr1[] =
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
स्थिर ध्रुव yytype_int8 yyr2[] =
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


गणना { YYENOMEM = -2 };

#परिभाषय yyerrok         (yyerrstatus = 0)
#परिभाषय yyclearin       (yychar = GRAM_रिक्तानि)

#परिभाषय YYACCEPT        गच्छ yyacceptlab
#परिभाषय YYABORT         गच्छ yyabortlab
#परिभाषय YYERROR         गच्छ yyerrorlab
#परिभाषय YYNOMEM         गच्छ yyexhaustedlab


#परिभाषय YYRECOVERING()  (!!yyerrstatus)

#परिभाषय YYBACKUP(Token, मानम्_4)                                    \
  कुरु                                                              \
    यदि (yychar == GRAM_रिक्तानि)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (मानम्_4);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        YY_LAC_DISCARD ("YYBACKUP");                              \
        गच्छ yybackup;                                            \
      }                                                           \
    अन्यथा                                                          \
      {                                                           \
        yyerror (&yylloc, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  यावत् (0)

/* Backward compatibility with an undocumented macro.
   Use GRAM_error or GRAM_UNDEF. */
#परिभाषय YYERRCODE GRAM_UNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#यदि_अपरिभाषितम् YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    कुरु                                                                  \
      यदि (N)                                                            \
        {                                                               \
          (Current).first_पङ्क्तिः   = YYRHSLOC (Rhs, 1).first_पङ्क्तिः;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).चरमः_पङ्क्तिः    = YYRHSLOC (Rhs, N).चरमः_पङ्क्तिः;         \
          (Current).चरमः_column  = YYRHSLOC (Rhs, N).चरमः_column;       \
        }                                                               \
      अन्यथा                                                              \
        {                                                               \
          (Current).first_पङ्क्तिः   = (Current).चरमः_पङ्क्तिः   =              \
            YYRHSLOC (Rhs, 0).चरमः_पङ्क्तिः;                                \
          (Current).first_column = (Current).चरमः_column =              \
            YYRHSLOC (Rhs, 0).चरमः_column;                              \
        }                                                               \
    यावत् (0)
#यदि_अन्तः

#परिभाषय YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#यदि GRAM_DEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(तर्काः_2)                        \
कुरु {                                            \
  यदि (yydebug)                                  \
    YYFPRINTF तर्काः_2;                             \
} यावत् (0)


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  यदि defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined GRAM_LTYPE_IS_TRIVIAL && GRAM_LTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_अप्रयुक्तम्
स्थिर पूर्णाङ्क
yy_location_print_ (FILE *yyo, YYLTYPE ध्रुव * ध्रुव yylocp)
{
  पूर्णाङ्क res = 0;
  पूर्णाङ्क अन्तः_col = 0 != yylocp->चरमः_column ? yylocp->चरमः_column - 1 : 0;
  यदि (0 <= yylocp->first_पङ्क्तिः)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_पङ्क्तिः);
      यदि (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  यदि (0 <= yylocp->चरमः_पङ्क्तिः)
    {
      यदि (yylocp->first_पङ्क्तिः < yylocp->चरमः_पङ्क्तिः)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->चरमः_पङ्क्तिः);
          यदि (0 <= अन्तः_col)
            res += YYFPRINTF (yyo, ".%d", अन्तः_col);
        }
      अन्यथा यदि (0 <= अन्तः_col && yylocp->first_column < अन्तः_col)
        res += YYFPRINTF (yyo, "-%d", अन्तः_col);
    }
  प्रत्यागम res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  अन्यथा

#   define YYLOCATION_PRINT(File, Loc) ((शून्य) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_चिह्नम्_PRINT(Title, विधा_2, मानम्_4, Location)                    \
कुरु {                                                                      \
  यदि (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_चिह्नम्_print (stderr,                                            \
                  विधा_2, मानम्_4, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} यावत् (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

स्थिर शून्य
yy_चिह्नम्_मानम्_print (FILE *yyo,
                       yysymbol_विधा_t yykind, YYSTYPE ध्रुव * ध्रुव yyvaluep, YYLTYPE ध्रुव * ध्रुव yylocationp)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  यदि (!yyvaluep)
    प्रत्यागम;
/* "%code pre-printer" blocks.  */
tron (yyo);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  विकल्प (yykind)
    {
    पक्ष YYSYMBOL_STRING: /* "string"  */
         { fputs (((*yyvaluep).STRING), yyo); }
        भङ्ग;

    पक्ष YYSYMBOL_TSTRING: /* "translatable string"  */
         { fputs (((*yyvaluep).TSTRING), yyo); }
        भङ्ग;

    पक्ष YYSYMBOL_PERCENT_ERROR_VERBOSE: /* "%error-verbose"  */
         { fputs (((*yyvaluep).PERCENT_ERROR_VERBOSE), yyo); }
        भङ्ग;

    पक्ष YYSYMBOL_PERCENT_FILE_उपसर्गः: /* "%file-prefix"  */
         { fputs (((*yyvaluep).PERCENT_FILE_उपसर्गः), yyo); }
        भङ्ग;

    पक्ष YYSYMBOL_PERCENT_FLAG: /* "%<flag>"  */
         { fprintf (yyo, "%%%s", ((*yyvaluep).PERCENT_FLAG)); }
        भङ्ग;

    पक्ष YYSYMBOL_PERCENT_NAME_उपसर्गः: /* "%name-prefix"  */
         { fputs (((*yyvaluep).PERCENT_NAME_उपसर्गः), yyo); }
        भङ्ग;

    पक्ष YYSYMBOL_PERCENT_विशुद्धम्_PARSER: /* "%pure-parser"  */
         { fputs (((*yyvaluep).PERCENT_विशुद्धम्_PARSER), yyo); }
        भङ्ग;

    पक्ष YYSYMBOL_BRACED_संहिता: /* "{...}"  */
         { fputs (((*yyvaluep).BRACED_संहिता), yyo); }
        भङ्ग;

    पक्ष YYSYMBOL_BRACED_PREDICATE: /* "%?{...}"  */
         { fputs (((*yyvaluep).BRACED_PREDICATE), yyo); }
        भङ्ग;

    पक्ष YYSYMBOL_BRACKETED_ID: /* "[identifier]"  */
         { fprintf (yyo, "[%s]", ((*yyvaluep).BRACKETED_ID)); }
        भङ्ग;

    पक्ष YYSYMBOL_CHAR_LITERAL: /* "character literal"  */
         { fputs (char_name (((*yyvaluep).CHAR_LITERAL)), yyo); }
        भङ्ग;

    पक्ष YYSYMBOL_EPILOGUE: /* "epilogue"  */
         { fputs (((*yyvaluep).EPILOGUE), yyo); }
        भङ्ग;

    पक्ष YYSYMBOL_ID: /* "identifier"  */
         { fputs (((*yyvaluep).ID), yyo); }
        भङ्ग;

    पक्ष YYSYMBOL_ID_COLON: /* "identifier:"  */
         { fprintf (yyo, "%s:", ((*yyvaluep).ID_COLON)); }
        भङ्ग;

    पक्ष YYSYMBOL_PROLOGUE: /* "%{...%}"  */
         { fputs (((*yyvaluep).PROLOGUE), yyo); }
        भङ्ग;

    पक्ष YYSYMBOL_चिह्नकम्: /* "<tag>"  */
         { fprintf (yyo, "<%s>", ((*yyvaluep).चिह्नकम्_2)); }
        भङ्ग;

    पक्ष YYSYMBOL_INT_LITERAL: /* "integer literal"  */
         { fprintf (yyo, "%d", ((*yyvaluep).INT_LITERAL)); }
        भङ्ग;

    पक्ष YYSYMBOL_PERCENT_PARAM: /* "%param"  */
{
  विकल्प (((*yyvaluep).PERCENT_PARAM))
    {
#परिभाषय CASE(अन्तर्_2, बहिर्गमनम्_2)                                           \
      पक्ष param_ ## अन्तर्_2: fputs ("%" #Out, yyo); भङ्ग
      CASE (lex,   lex-param);
      CASE (parse, parse-param);
      CASE (both,  param);
#अपरिभाषय CASE
      पक्ष param_none: aver (false); भङ्ग;
    }
}
        भङ्ग;

    पक्ष YYSYMBOL_संहिता_props_type: /* code_props_type  */
         { fprintf (yyo, "%s", संहिता_props_type_string (((*yyvaluep).संहिता_props_type))); }
        भङ्ग;

    पक्ष YYSYMBOL_75_string_opt: /* string.opt  */
         { fputs (((*yyvaluep).yykind_75), yyo); }
        भङ्ग;

    पक्ष YYSYMBOL_76_चिह्नकम्_opt: /* tag.opt  */
         { fputs (((*yyvaluep).yykind_76), yyo); }
        भङ्ग;

    पक्ष YYSYMBOL_generic_symlist: /* generic_symlist  */
         { चिह्नम्_list_syms_print (((*yyvaluep).generic_symlist), yyo); }
        भङ्ग;

    पक्ष YYSYMBOL_generic_symlist_item: /* generic_symlist_item  */
         { चिह्नम्_list_syms_print (((*yyvaluep).generic_symlist_item), yyo); }
        भङ्ग;

    पक्ष YYSYMBOL_चिह्नकम्_2: /*चिह्नकम्*/
         { fprintf (yyo, "<%s>", ((*yyvaluep).चिह्नकम्)); }
        भङ्ग;

    पक्ष YYSYMBOL_nterm_decls: /* nterm_decls  */
         { चिह्नम्_list_syms_print (((*yyvaluep).nterm_decls), yyo); }
        भङ्ग;

    पक्ष YYSYMBOL_token_decls: /* token_decls  */
         { चिह्नम्_list_syms_print (((*yyvaluep).token_decls), yyo); }
        भङ्ग;

    पक्ष YYSYMBOL_82_token_decl_1: /* token_decl.1  */
         { चिह्नम्_list_syms_print (((*yyvaluep).yykind_82), yyo); }
        भङ्ग;

    पक्ष YYSYMBOL_token_decl: /* token_decl  */
         { fprintf (yyo, "%s", ((*yyvaluep).token_decl) ? ((*yyvaluep).token_decl)->चिह्नकम् : "<NULL>"); }
        भङ्ग;

    पक्ष YYSYMBOL_84_int_opt: /* int.opt  */
         { fprintf (yyo, "%d", ((*yyvaluep).yykind_84)); }
        भङ्ग;

    पक्ष YYSYMBOL_alias: /* alias  */
         { fprintf (yyo, "%s", ((*yyvaluep).alias) ? ((*yyvaluep).alias)->चिह्नकम् : "<NULL>"); }
        भङ्ग;

    पक्ष YYSYMBOL_token_decls_for_prec: /* token_decls_for_prec  */
         { चिह्नम्_list_syms_print (((*yyvaluep).token_decls_for_prec), yyo); }
        भङ्ग;

    पक्ष YYSYMBOL_87_token_decl_for_prec_1: /* token_decl_for_prec.1  */
         { चिह्नम्_list_syms_print (((*yyvaluep).yykind_87), yyo); }
        भङ्ग;

    पक्ष YYSYMBOL_token_decl_for_prec: /* token_decl_for_prec  */
         { fprintf (yyo, "%s", ((*yyvaluep).token_decl_for_prec) ? ((*yyvaluep).token_decl_for_prec)->चिह्नकम् : "<NULL>"); }
        भङ्ग;

    पक्ष YYSYMBOL_चिह्नम्_decls: /* symbol_decls  */
         { चिह्नम्_list_syms_print (((*yyvaluep).चिह्नम्_decls), yyo); }
        भङ्ग;

    पक्ष YYSYMBOL_90_symbols_1: /* symbols.1  */
         { चिह्नम्_list_syms_print (((*yyvaluep).yykind_90), yyo); }
        भङ्ग;

    पक्ष YYSYMBOL_variable: /* variable  */
         { fputs (((*yyvaluep).variable), yyo); }
        भङ्ग;

    पक्ष YYSYMBOL_मानम्: /*मानम्*/
{
  विकल्प (((*yyvaluep).मानम्).विधा)
    {
    पक्ष muscle_संहिता:    fprintf (yyo,  "{%s}",  ((*yyvaluep).मानम्).chars); भङ्ग;
    पक्ष muscle_keyword: fprintf (yyo,   "%s",   ((*yyvaluep).मानम्).chars); भङ्ग;
    पक्ष muscle_string:  fprintf (yyo, "\"%s\"", ((*yyvaluep).मानम्).chars); भङ्ग;
    }
}
        भङ्ग;

    पक्ष YYSYMBOL_id: /* id  */
         { fprintf (yyo, "%s", ((*yyvaluep).id) ? ((*yyvaluep).id)->चिह्नकम् : "<NULL>"); }
        भङ्ग;

    पक्ष YYSYMBOL_id_colon: /* id_colon  */
         { fprintf (yyo, "%s:", ((*yyvaluep).id_colon)->चिह्नकम्); }
        भङ्ग;

    पक्ष YYSYMBOL_चिह्नम्: /*चिह्नम्*/
         { fprintf (yyo, "%s", ((*yyvaluep).चिह्नम्) ? ((*yyvaluep).चिह्नम्)->चिह्नकम् : "<NULL>"); }
        भङ्ग;

    पक्ष YYSYMBOL_string_as_id: /* string_as_id  */
         { fprintf (yyo, "%s", ((*yyvaluep).string_as_id) ? ((*yyvaluep).string_as_id)->चिह्नकम् : "<NULL>"); }
        भङ्ग;

      स्वभाव:
        भङ्ग;
    }
  YY_IGNORE_MAYBE_UNINITIALIZED_अन्तः
/* "%code post-printer" blocks.  */
troff (yyo);

}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

स्थिर शून्य
yy_चिह्नम्_print (FILE *yyo,
                 yysymbol_विधा_t yykind, YYSTYPE ध्रुव * ध्रुव yyvaluep, YYLTYPE ध्रुव * ध्रुव yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_चिह्नम्_मानम्_print (yyo, yykind, yyvaluep, yylocationp);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

स्थिर शून्य
yy_राशिः_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  प्रति (; yybottom <= yytop; yybottom++)
    {
      पूर्णाङ्क yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_राशिः_PRINT(Bottom, शिखरम्)                            \
कुरु {                                                            \
  यदि (yydebug)                                                  \
    yy_राशिः_print ((Bottom), (शिखरम्));                           \
} यावत् (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

स्थिर शून्य
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 पूर्णाङ्क yyrule)
{
  पूर्णाङ्क yylno = yyrline[yyrule];
  पूर्णाङ्क yynrhs = yyr2[yyrule];
  पूर्णाङ्क yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  प्रति (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_चिह्नम्_print (stderr,
                       YY_ACCESSING_चिह्नम् (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]));
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(नियमः_2)          \
कुरु {                                    \
  यदि (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, नियमः_2); \
} यावत् (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
पूर्णाङ्क yydebug;
#अन्यथा /* !GRAM_DEBUG */
# define YYDPRINTF(तर्काः_2) ((शून्य) 0)
# define YY_चिह्नम्_PRINT(Title, विधा_2, मानम्_4, Location)
# define YY_राशिः_PRINT(Bottom, शिखरम्)
# define YY_REDUCE_PRINT(नियमः_2)
#यदि_अन्तः /* !GRAM_DEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#यदि_अपरिभाषितम् YYINITDEPTH
# define YYINITDEPTH 200
#यदि_अन्तः

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#यदि_अपरिभाषितम् YYMAXDEPTH
# define YYMAXDEPTH 10000
#यदि_अन्तः


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
स्थिर पूर्णाङ्क
yy_lac_राशिः_realloc (YYPTRDIFF_T *yycapacity, YYPTRDIFF_T yyadd,
#यदि GRAM_DEBUG
                      वर्ण ध्रुव *yydebug_उपसर्गः,
                      वर्ण ध्रुव *yydebug_suffix,
#यदि_अन्तः
                      yy_state_t **yybottom,
                      yy_state_t *yybottom_no_free,
                      yy_state_t **yytop, yy_state_t *yytop_रिक्तानि)
{
  YYPTRDIFF_T yysize_old =
    *yytop == yytop_रिक्तानि ? 0 : *yytop - *yybottom + 1;
  YYPTRDIFF_T yysize_new = yysize_old + yyadd;
  यदि (*yycapacity < yysize_new)
    {
      YYPTRDIFF_T yyalloc = 2 * yysize_new;
      yy_state_t *yybottom_new;
      /* Use YYMAXDEPTH for maximum stack size given that the stack
         should never need to grow larger than the main state stack
         needs to grow without LAC.  */
      यदि (YYMAXDEPTH < yysize_new)
        {
          YYDPRINTF ((stderr, "%smax size exceeded%s", yydebug_उपसर्गः,
                      yydebug_suffix));
          प्रत्यागम YYENOMEM;
        }
      यदि (YYMAXDEPTH < yyalloc)
        yyalloc = YYMAXDEPTH;
      yybottom_new =
        YY_CAST (yy_state_t *,
                 YYSTACK_ALLOC (YY_CAST (YYSIZE_T,
                                         yyalloc * YYSIZEOF (*yybottom_new))));
      यदि (!yybottom_new)
        {
          YYDPRINTF ((stderr, "%srealloc failed%s", yydebug_उपसर्गः,
                      yydebug_suffix));
          प्रत्यागम YYENOMEM;
        }
      यदि (*yytop != yytop_रिक्तानि)
        {
          YYCOPY (yybottom_new, *yybottom, yysize_old);
          *yytop = yybottom_new + (yysize_old - 1);
        }
      यदि (*yybottom != yybottom_no_free)
        YYSTACK_FREE (*yybottom);
      *yybottom = yybottom_new;
      *yycapacity = yyalloc;
    }
  प्रत्यागम 0;
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
#परिभाषय YY_LAC_ESTABLISH                                                \
कुरु {                                                                    \
  यदि (!yy_lac_established)                                              \
    {                                                                   \
      YYDPRINTF ((stderr,                                               \
                  "LAC: initial context established for %s\n",          \
                  yysymbol_name (yytoken)));                            \
      yy_lac_established = 1;                                           \
      विकल्प (yy_lac (yyesa, &yyes, &yyes_क्षमता, yyssp, yytoken))    \
        {                                                               \
        पक्ष YYENOMEM:                                                  \
          YYNOMEM;                                                      \
        पक्ष 1:                                                         \
          गच्छ yyerrlab;                                                \
        }                                                               \
    }                                                                   \
} यावत् (0)

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
#यदि GRAM_DEBUG
# define YY_LAC_DISCARD(Event)                                           \
कुरु {                                                                     \
  यदि (yy_lac_established)                                                \
    {                                                                    \
      YYDPRINTF ((stderr, "LAC: initial context discarded due to "       \
                  Event "\n"));                                          \
      yy_lac_established = 0;                                            \
    }                                                                    \
} यावत् (0)
#अन्यथा
# define YY_LAC_DISCARD(Event) yy_lac_established = 0
#यदि_अन्तः

/* Given the stack whose top is *YYSSP, return 0 iff YYTOKEN can
   eventually (after perhaps some reductions) be shifted, return 1 if
   not, or return YYENOMEM if memory is exhausted.  As preconditions and
   postconditions: *YYES_CAPACITY is the allocated size of the array to
   which *YYES points, and either *YYES = YYESA or *YYES points to an
   array allocated with YYSTACK_ALLOC.  yy_lac may overwrite the
   contents of either array, alter *YYES and *YYES_CAPACITY, and free
   any old *YYES other than YYESA.  */
स्थिर पूर्णाङ्क
yy_lac (yy_state_t *yyesa, yy_state_t **yyes,
        YYPTRDIFF_T *yyes_क्षमता, yy_state_t *yyssp, yysymbol_विधा_t yytoken)
{
  yy_state_t *yyes_पूर्व = yyssp;
  yy_state_t *yyesp = yyes_पूर्व;
  /* Reduce until we encounter a shift and thereby accept the token.  */
  YYDPRINTF ((stderr, "LAC: checking lookahead %s:", yysymbol_name (yytoken)));
  यदि (yytoken == YYSYMBOL_YYUNDEF)
    {
      YYDPRINTF ((stderr, " Always Err\n"));
      प्रत्यागम 1;
    }
  यावत् (1)
    {
      पूर्णाङ्क yyrule = yypact[+*yyesp];
      यदि (yypact_मानम्_is_default (yyrule)
          || (yyrule += yytoken) < 0 || YYLAST < yyrule
          || yycheck[yyrule] != yytoken)
        {
          /* Use the default action.  */
          yyrule = yydefact[+*yyesp];
          यदि (yyrule == 0)
            {
              YYDPRINTF ((stderr, " Err\n"));
              प्रत्यागम 1;
            }
        }
      अन्यथा
        {
          /* Use the action from yytable.  */
          yyrule = yytable[yyrule];
          यदि (yytable_मानम्_is_error (yyrule))
            {
              YYDPRINTF ((stderr, " Err\n"));
              प्रत्यागम 1;
            }
          यदि (0 < yyrule)
            {
              YYDPRINTF ((stderr, " S%d\n", yyrule));
              प्रत्यागम 0;
            }
          yyrule = -yyrule;
        }
      /* By now we know we have to simulate a reduce.  */
      YYDPRINTF ((stderr, " R%d", yyrule - 1));
      {
        /* Pop the corresponding number of values from the stack.  */
        YYPTRDIFF_T yylen = yyr2[yyrule];
        /* First pop from the LAC stack as many tokens as possible.  */
        यदि (yyesp != yyes_पूर्व)
          {
            YYPTRDIFF_T yysize = yyesp - *yyes + 1;
            यदि (yylen < yysize)
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
        यदि (yylen)
          yyesp = yyes_पूर्व -= yylen;
      }
      /* Push the resulting state of the reduction.  */
      {
        yy_state_fast_t yystate;
        {
          ध्रुव पूर्णाङ्क yylhs = yyr1[yyrule] - YYNTOKENS;
          ध्रुव पूर्णाङ्क yyi = yypgoto[yylhs] + *yyesp;
          yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyesp
                     ? yytable[yyi]
                     : yydefgoto[yylhs]);
        }
        यदि (yyesp == yyes_पूर्व)
          {
            yyesp = *yyes;
            YY_IGNORE_USELESS_CAST_BEGIN
            *yyesp = YY_CAST (yy_state_t, yystate);
            YY_IGNORE_USELESS_CAST_अन्तः
          }
        अन्यथा
          {
            यदि (yy_lac_राशिः_realloc (yyes_क्षमता, 1,
#यदि GRAM_DEBUG
                                      " (", ")",
#यदि_अन्तः
                                      yyes, yyesa, &yyesp, yyes_पूर्व))
              {
                YYDPRINTF ((stderr, "\n"));
                प्रत्यागम YYENOMEM;
              }
            YY_IGNORE_USELESS_CAST_BEGIN
            *++yyesp = YY_CAST (yy_state_t, yystate);
            YY_IGNORE_USELESS_CAST_अन्तः
          }
        YYDPRINTF ((stderr, " G%d", yystate));
      }
    }
}

/* Context of a parse error.  */
प्रकारनिर्देश संरचना
{
  yy_state_t *yyssp;
  yy_state_t *yyesa;
  yy_state_t **yyes;
  YYPTRDIFF_T *yyes_क्षमता;
  yysymbol_विधा_t yytoken;
  YYLTYPE *yylloc;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
स्थिर पूर्णाङ्क
yypcontext_expected_tokens (ध्रुव yypcontext_t *yyctx,
                            yysymbol_विधा_t yyarg[], पूर्णाङ्क yyargn)
{
  /* Actual size of YYARG. */
  पूर्णाङ्क yycount = 0;

  पूर्णाङ्क yyx;
  प्रति (yyx = 0; yyx < YYNTOKENS; ++yyx)
    {
      yysymbol_विधा_t yysym = YY_CAST (yysymbol_विधा_t, yyx);
      यदि (yysym != YYSYMBOL_YYerror && yysym != YYSYMBOL_YYUNDEF)
        विकल्प (yy_lac (yyctx->yyesa, yyctx->yyes, yyctx->yyes_क्षमता, yyctx->yyssp, yysym))
          {
          पक्ष YYENOMEM:
            प्रत्यागम YYENOMEM;
          पक्ष 1:
            अनुवर्त;
          स्वभाव:
            यदि (!yyarg)
              ++yycount;
            अन्यथा यदि (yycount == yyargn)
              प्रत्यागम 0;
            अन्यथा
              yyarg[yycount++] = yysym;
          }
    }
  यदि (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  प्रत्यागम yycount;
}




/* The kind of the lookahead of this context.  */
स्थिर yysymbol_विधा_t
yypcontext_token (ध्रुव yypcontext_t *yyctx) YY_ATTRIBUTE_अप्रयुक्तम्;

स्थिर yysymbol_विधा_t
yypcontext_token (ध्रुव yypcontext_t *yyctx)
{
  प्रत्यागम yyctx->yytoken;
}

/* The location of the lookahead of this context.  */
स्थिर YYLTYPE *
yypcontext_location (ध्रुव yypcontext_t *yyctx) YY_ATTRIBUTE_अप्रयुक्तम्;

स्थिर YYLTYPE *
yypcontext_location (ध्रुव yypcontext_t *yyctx)
{
  प्रत्यागम yyctx->yylloc;
}

/* User defined function to report a syntax error.  */
स्थिर पूर्णाङ्क
yyreport_syntax_error (ध्रुव yypcontext_t *yyctx);

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

स्थिर शून्य
yydestruct (ध्रुव वर्ण *yymsg,
            yysymbol_विधा_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  यदि (!yymsg)
    yymsg = "Deleting";
  YY_चिह्नम्_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  विकल्प (yykind)
    {
    पक्ष YYSYMBOL_generic_symlist: /* generic_symlist  */
            { चिह्नम्_list_free (((*yyvaluep).generic_symlist)); }
        भङ्ग;

    पक्ष YYSYMBOL_generic_symlist_item: /* generic_symlist_item  */
            { चिह्नम्_list_free (((*yyvaluep).generic_symlist_item)); }
        भङ्ग;

    पक्ष YYSYMBOL_nterm_decls: /* nterm_decls  */
            { चिह्नम्_list_free (((*yyvaluep).nterm_decls)); }
        भङ्ग;

    पक्ष YYSYMBOL_token_decls: /* token_decls  */
            { चिह्नम्_list_free (((*yyvaluep).token_decls)); }
        भङ्ग;

    पक्ष YYSYMBOL_82_token_decl_1: /* token_decl.1  */
            { चिह्नम्_list_free (((*yyvaluep).yykind_82)); }
        भङ्ग;

    पक्ष YYSYMBOL_token_decls_for_prec: /* token_decls_for_prec  */
            { चिह्नम्_list_free (((*yyvaluep).token_decls_for_prec)); }
        भङ्ग;

    पक्ष YYSYMBOL_87_token_decl_for_prec_1: /* token_decl_for_prec.1  */
            { चिह्नम्_list_free (((*yyvaluep).yykind_87)); }
        भङ्ग;

    पक्ष YYSYMBOL_चिह्नम्_decls: /* symbol_decls  */
            { चिह्नम्_list_free (((*yyvaluep).चिह्नम्_decls)); }
        भङ्ग;

    पक्ष YYSYMBOL_90_symbols_1: /* symbols.1  */
            { चिह्नम्_list_free (((*yyvaluep).yykind_90)); }
        भङ्ग;

      स्वभाव:
        भङ्ग;
    }
  YY_IGNORE_MAYBE_UNINITIALIZED_अन्तः
}






/*----------.
| yyparse.  |
`----------*/

पूर्णाङ्क
yyparse (शून्य)
{
/* Lookahead token kind.  */
पूर्णाङ्क yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_मानम् (स्थिर YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_मानम् (= yyval_default);

/* Location data for the lookahead symbol.  */
स्थिर YYLTYPE yyloc_default
# यदि defined GRAM_LTYPE_IS_TRIVIAL && GRAM_LTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

    /* Number of syntax errors so far.  */
    पूर्णाङ्क yynerrs YY_ATTRIBUTE_अप्रयुक्तम् = 0;

    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    पूर्णाङ्क yyerrstatus = 0;

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
    YYPTRDIFF_T yyes_क्षमता = 20 < YYMAXDEPTH ? 20 : YYMAXDEPTH;

  /* Whether LAC context is established.  A Boolean.  */
  पूर्णाङ्क yy_lac_established = 0;
  पूर्णाङ्क yyn;
  /* The return value of yyparse.  */
  पूर्णाङ्क yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_विधा_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];



#परिभाषय YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  पूर्णाङ्क yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = GRAM_रिक्तानि; /* Cause a token to be read.  */


/* User initialization code.  */
{
  /* Bison's grammar can initial empty locations, hence a default
     location is needed. */
  boundary_समुच्चयः (&yylloc.start, grammar_file, 1, 1, 1);
  boundary_समुच्चयः (&yylloc.अन्तः, grammar_file, 1, 1, 1);
}


  yylsp[0] = yylloc;
  गच्छ yysetstate;


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
  YY_IGNORE_USELESS_CAST_अन्तः
  YY_राशिः_PRINT (yyss, yyssp);

  यदि (yyss + yystacksize - 1 <= yyssp)
#यदि !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#अन्यथा
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# यदि defined yyoverflow
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
      यदि (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      यदि (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        सङ्घ yyalloc *yyptr =
          YY_CAST (सङ्घ yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_अष्टकानि (yystacksize))));
        यदि (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        यदि (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (दीर्घ, yystacksize)));
      YY_IGNORE_USELESS_CAST_अन्तः

      यदि (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#यदि_अन्तः /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  यदि (yystate == YYFINAL)
    YYACCEPT;

  गच्छ yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  यदि (yypact_मानम्_is_default (yyn))
    गच्छ yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  यदि (yychar == GRAM_रिक्तानि)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex (&yylval, &yylloc);
    }

  यदि (yychar <= GRAM_EOF)
    {
      yychar = GRAM_EOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  अन्यथा यदि (yychar == GRAM_error)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = GRAM_UNDEF;
      yytoken = YYSYMBOL_YYerror;
      yyerror_range[1] = yylloc;
      गच्छ yyerrlab1;
    }
  अन्यथा
    {
      yytoken = YYTRANSLATE (yychar);
      YY_चिह्नम्_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  यदि (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    {
      YY_LAC_ESTABLISH;
      गच्छ yydefault;
    }
  yyn = yytable[yyn];
  यदि (yyn <= 0)
    {
      यदि (yytable_मानम्_is_error (yyn))
        गच्छ yyerrlab;
      yyn = -yyn;
      YY_LAC_ESTABLISH;
      गच्छ yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  यदि (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_चिह्नम्_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_अन्तः
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = GRAM_रिक्तानि;
  YY_LAC_DISCARD ("shift");
  गच्छ yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  यदि (yyn == 0)
    गच्छ yyerrlab;
  गच्छ yyreduce;


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
    पूर्णाङ्क yychar_backup = yychar;
    विकल्प (yyn)
      {
  पक्ष 6: /* prologue_declaration: "%{...%}"  */
    {
      muscle_संहिता_grow (union_आलोकितम् ? "post_prologue" : "pre_prologue",
                        translate_संहिता ((yyvsp[0].PROLOGUE), (yylsp[0]), true), (yylsp[0]));
      संहिता_scanner_चरमः_string_free ();
    }
    भङ्ग;

  पक्ष 7: /* prologue_declaration: "%<flag>"  */
    {
      muscle_percent_define_ensure ((yyvsp[0].PERCENT_FLAG), (yylsp[0]), true);
    }
    भङ्ग;

  पक्ष 8: /* prologue_declaration: "%define" variable value  */
    {
      muscle_percent_define_निवेशय ((yyvsp[-1].variable), (yyloc), (yyvsp[0].मानम्).विधा, (yyvsp[0].मानम्).chars,
                                    MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
    }
    भङ्ग;

  पक्ष 9: /* prologue_declaration: "%header" string.opt  */
                                   { handle_शीर्षकम् ((yyvsp[0].yykind_75)); }
    भङ्ग;

  पक्ष 10: /* prologue_declaration: "%error-verbose"  */
                                   { handle_error_verbose (&(yyloc), (yyvsp[0].PERCENT_ERROR_VERBOSE)); }
    भङ्ग;

  पक्ष 11: /* prologue_declaration: "%expect" "integer literal"  */
                                   { expected_sr_conflicts = (yyvsp[0].INT_LITERAL); }
    भङ्ग;

  पक्ष 12: /* prologue_declaration: "%expect-rr" "integer literal"  */
                                   { expected_rr_conflicts = (yyvsp[0].INT_LITERAL); }
    भङ्ग;

  पक्ष 13: /* prologue_declaration: "%file-prefix" "string"  */
                                   { handle_file_उपसर्गः (&(yyloc), &(yylsp[-1]), (yyvsp[-1].PERCENT_FILE_उपसर्गः), (yyvsp[0].STRING)); }
    भङ्ग;

  पक्ष 14: /* prologue_declaration: "%glr-parser"  */
    {
      nondeterministic_parser = true;
      glr_parser = true;
    }
    भङ्ग;

  पक्ष 15: /* prologue_declaration: "%initial-action" "{...}"  */
    {
      muscle_संहिता_grow ("initial_action", translate_संहिता ((yyvsp[0].BRACED_संहिता), (yylsp[0]), false), (yylsp[0]));
      संहिता_scanner_चरमः_string_free ();
    }
    भङ्ग;

  पक्ष 16: /* prologue_declaration: "%language" "string"  */
                                { handle_language (&(yylsp[-1]), (yyvsp[0].STRING)); }
    भङ्ग;

  पक्ष 17: /* prologue_declaration: "%name-prefix" "string"  */
                                { handle_name_उपसर्गः (&(yyloc), (yyvsp[-1].PERCENT_NAME_उपसर्गः), (yyvsp[0].STRING)); }
    भङ्ग;

  पक्ष 18: /* prologue_declaration: "%no-lines"  */
                                { no_पङ्क्तयः_flag = true; }
    भङ्ग;

  पक्ष 19: /* prologue_declaration: "%nondeterministic-parser"  */
                                { nondeterministic_parser = true; }
    भङ्ग;

  पक्ष 20: /* prologue_declaration: "%output" "string"  */
                                { spec_outfile = unquote ((yyvsp[0].STRING)); gram_scanner_चरमः_string_free (); }
    भङ्ग;

  पक्ष 21: /* $@1: %empty  */
           { current_param = (yyvsp[0].PERCENT_PARAM); }
    भङ्ग;

  पक्ष 22: /* prologue_declaration: "%param" $@1 params  */
                                          { current_param = param_none; }
    भङ्ग;

  पक्ष 23: /* prologue_declaration: "%pure-parser"  */
                                { handle_विशुद्धम्_parser (&(yyloc), (yyvsp[0].PERCENT_विशुद्धम्_PARSER)); }
    भङ्ग;

  पक्ष 24: /* prologue_declaration: "%require" "string"  */
                                { handle_require (&(yylsp[0]), (yyvsp[0].STRING)); }
    भङ्ग;

  पक्ष 25: /* prologue_declaration: "%skeleton" "string"  */
                                { handle_skeleton (&(yylsp[0]), (yyvsp[0].STRING)); }
    भङ्ग;

  पक्ष 26: /* prologue_declaration: "%token-table"  */
                                { token_सारणी_flag = true; }
    भङ्ग;

  पक्ष 27: /* prologue_declaration: "%verbose"  */
                                { report_flag |= report_अवस्थाः; }
    भङ्ग;

  पक्ष 28: /* prologue_declaration: "%yacc"  */
                                { handle_yacc (&(yyloc)); }
    भङ्ग;

  पक्ष 29: /* prologue_declaration: error ";"  */
                                { current_class = unknown_sym; yyerrok; }
    भङ्ग;

  पक्ष 31: /* params: params "{...}"  */
                   { सङ्कलय_param (current_param, (yyvsp[0].BRACED_संहिता), (yylsp[0])); }
    भङ्ग;

  पक्ष 32: /* params: "{...}"  */
                   { सङ्कलय_param (current_param, (yyvsp[0].BRACED_संहिता), (yylsp[0])); }
    भङ्ग;

  पक्ष 34: /* grammar_declaration: "%start" symbols.1  */
    {
      grammar_start_symbols_सङ्कलय ((yyvsp[0].yykind_90));
    }
    भङ्ग;

  पक्ष 35: /* grammar_declaration: code_props_type "{...}" generic_symlist  */
    {
      संहिता_props संहिता;
      संहिता_props_चिह्नम्_action_init (&संहिता, (yyvsp[-1].BRACED_संहिता), (yylsp[-1]));
      संहिता_props_translate_संहिता (&संहिता);
      {
        प्रति (चिह्नम्_list *list = (yyvsp[0].generic_symlist); list; list = list->अग्र)
          चिह्नम्_list_संहिता_props_समुच्चयः (list, (yyvsp[-2].संहिता_props_type), &संहिता);
        चिह्नम्_list_free ((yyvsp[0].generic_symlist));
      }
    }
    भङ्ग;

  पक्ष 36: /* grammar_declaration: "%default-prec"  */
    {
      default_prec = true;
    }
    भङ्ग;

  पक्ष 37: /* grammar_declaration: "%no-default-prec"  */
    {
      default_prec = false;
    }
    भङ्ग;

  पक्ष 38: /* grammar_declaration: "%code" "{...}"  */
    {
      /* Do not invoke muscle_percent_code_grow here since it invokes
         muscle_user_name_list_grow.  */
      muscle_संहिता_grow ("percent_code()",
                        translate_संहिता_braceless ((yyvsp[0].BRACED_संहिता), (yylsp[0])), (yylsp[0]));
      संहिता_scanner_चरमः_string_free ();
    }
    भङ्ग;

  पक्ष 39: /* grammar_declaration: "%code" "identifier" "{...}"  */
    {
      muscle_percent_संहिता_grow ((yyvsp[-1].ID), (yylsp[-1]), translate_संहिता_braceless ((yyvsp[0].BRACED_संहिता), (yylsp[0])), (yylsp[0]));
      संहिता_scanner_चरमः_string_free ();
    }
    भङ्ग;

  पक्ष 40: /* code_props_type: "%destructor"  */
                 { (yyval.संहिता_props_type) = destructor; }
    भङ्ग;

  पक्ष 41: /* code_props_type: "%printer"  */
                 { (yyval.संहिता_props_type) = printer; }
    भङ्ग;

  पक्ष 42: /* union_name: %empty  */
         {}
    भङ्ग;

  पक्ष 43: /* union_name: "identifier"  */
         { muscle_percent_define_निवेशय ("api.value.union.name",
                                         (yylsp[0]), muscle_keyword, (yyvsp[0].ID),
                                         MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE); }
    भङ्ग;

  पक्ष 44: /* grammar_declaration: "%union" union_name "{...}"  */
    {
      union_आलोकितम् = true;
      muscle_संहिता_grow ("union_members", translate_संहिता_braceless ((yyvsp[0].BRACED_संहिता), (yylsp[0])), (yylsp[0]));
      संहिता_scanner_चरमः_string_free ();
    }
    भङ्ग;

  पक्ष 45: /* $@2: %empty  */
           { current_class = nterm_sym; }
    भङ्ग;

  पक्ष 46: /* symbol_declaration: "%nterm" $@2 nterm_decls  */
    {
      current_class = unknown_sym;
      चिह्नम्_list_free ((yyvsp[0].nterm_decls));
    }
    भङ्ग;

  पक्ष 47: /* $@3: %empty  */
           { current_class = token_sym; }
    भङ्ग;

  पक्ष 48: /* symbol_declaration: "%token" $@3 token_decls  */
    {
      current_class = unknown_sym;
      चिह्नम्_list_free ((yyvsp[0].token_decls));
    }
    भङ्ग;

  पक्ष 49: /* $@4: %empty  */
          { current_class = pct_type_sym; }
    भङ्ग;

  पक्ष 50: /* symbol_declaration: "%type" $@4 symbol_decls  */
    {
      current_class = unknown_sym;
      चिह्नम्_list_free ((yyvsp[0].चिह्नम्_decls));
    }
    भङ्ग;

  पक्ष 51: /* symbol_declaration: precedence_declarator token_decls_for_prec  */
    {
      ++current_prec;
      प्रति (चिह्नम्_list *list = (yyvsp[0].token_decls_for_prec); list; list = list->अग्र)
        चिह्नम्_precedence_समुच्चयः (list->content.sym, current_prec, (yyvsp[-1].precedence_declarator), (yylsp[-1]));
      चिह्नम्_list_free ((yyvsp[0].token_decls_for_prec));
    }
    भङ्ग;

  पक्ष 52: /* precedence_declarator: "%left"  */
                { (yyval.precedence_declarator) = वामः_assoc; }
    भङ्ग;

  पक्ष 53: /* precedence_declarator: "%right"  */
                { (yyval.precedence_declarator) = दक्षिणः_assoc; }
    भङ्ग;

  पक्ष 54: /* precedence_declarator: "%nonassoc"  */
                { (yyval.precedence_declarator) = non_assoc; }
    भङ्ग;

  पक्ष 55: /* precedence_declarator: "%precedence"  */
                { (yyval.precedence_declarator) = precedence_assoc; }
    भङ्ग;

  पक्ष 56: /* string.opt: %empty  */
          { (yyval.yykind_75) = रिक्त; }
    भङ्ग;

  पक्ष 57: /* string.opt: "string"  */
          { (yyval.yykind_75) = (yyvsp[0].STRING); }
    भङ्ग;

  पक्ष 58: /* tag.opt: %empty  */
         { (yyval.yykind_76) = रिक्त; }
    भङ्ग;

  पक्ष 59: /* tag.opt: "<tag>"  */
         { (yyval.yykind_76) = (yyvsp[0].चिह्नकम्_2); }
    भङ्ग;

  पक्ष 61: /* generic_symlist: generic_symlist generic_symlist_item  */
                                         { (yyval.generic_symlist) = चिह्नम्_list_append ((yyvsp[-1].generic_symlist), (yyvsp[0].generic_symlist_item)); }
    भङ्ग;

  पक्ष 62: /* generic_symlist_item: symbol  */
            { (yyval.generic_symlist_item) = चिह्नम्_list_sym_new ((yyvsp[0].चिह्नम्), (yylsp[0])); }
    भङ्ग;

  पक्ष 63: /* generic_symlist_item: tag  */
            { (yyval.generic_symlist_item) = चिह्नम्_list_type_new ((yyvsp[0].चिह्नकम्), (yylsp[0])); }
    भङ्ग;

  पक्ष 65: /* tag: "<*>"  */
        { (yyval.चिह्नकम्) = uniqstr_new ("*"); }
    भङ्ग;

  पक्ष 66: /* tag: "<>"  */
        { (yyval.चिह्नकम्) = uniqstr_new (""); }
    भङ्ग;

  पक्ष 68: /* token_decls: token_decl.1  */
    {
      (yyval.token_decls) = (yyvsp[0].yykind_82);
    }
    भङ्ग;

  पक्ष 69: /* token_decls: "<tag>" token_decl.1  */
    {
      (yyval.token_decls) = चिह्नम्_list_type_समुच्चयः ((yyvsp[0].yykind_82), (yyvsp[-1].चिह्नकम्_2));
    }
    भङ्ग;

  पक्ष 70: /* token_decls: token_decls "<tag>" token_decl.1  */
    {
      (yyval.token_decls) = चिह्नम्_list_append ((yyvsp[-2].token_decls), चिह्नम्_list_type_समुच्चयः ((yyvsp[0].yykind_82), (yyvsp[-1].चिह्नकम्_2)));
    }
    भङ्ग;

  पक्ष 71: /* token_decl.1: token_decl  */
                            { (yyval.yykind_82) = चिह्नम्_list_sym_new ((yyvsp[0].token_decl), (yylsp[0])); }
    भङ्ग;

  पक्ष 72: /* token_decl.1: token_decl.1 token_decl  */
                            { (yyval.yykind_82) = चिह्नम्_list_append ((yyvsp[-1].yykind_82), चिह्नम्_list_sym_new ((yyvsp[0].token_decl), (yylsp[0]))); }
    भङ्ग;

  पक्ष 73: /* token_decl: id int.opt alias  */
    {
      (yyval.token_decl) = (yyvsp[-2].id);
      चिह्नम्_class_समुच्चयः ((yyvsp[-2].id), current_class, (yylsp[-2]), true);
      यदि (0 <= (yyvsp[-1].yykind_84))
        चिह्नम्_संहिता_समुच्चयः ((yyvsp[-2].id), (yyvsp[-1].yykind_84), (yylsp[-1]));
      यदि ((yyvsp[0].alias))
        चिह्नम्_make_alias ((yyvsp[-2].id), (yyvsp[0].alias), (yylsp[0]));
    }
    भङ्ग;

  पक्ष 74: /* int.opt: %empty  */
          { (yyval.yykind_84) = -1; }
    भङ्ग;

  पक्ष 76: /* alias: %empty  */
                 { (yyval.alias) = रिक्त; }
    भङ्ग;

  पक्ष 77: /* alias: string_as_id  */
                 { (yyval.alias) = (yyvsp[0].string_as_id); }
    भङ्ग;

  पक्ष 78: /* alias: "translatable string"  */
    {
      (yyval.alias) = चिह्नम्_get ((yyvsp[0].TSTRING), (yylsp[0]));
      चिह्नम्_class_समुच्चयः ((yyval.alias), token_sym, (yylsp[0]), false);
      (yyval.alias)->translatable = true;
    }
    भङ्ग;

  पक्ष 79: /* token_decls_for_prec: token_decl_for_prec.1  */
    {
      (yyval.token_decls_for_prec) = (yyvsp[0].yykind_87);
    }
    भङ्ग;

  पक्ष 80: /* token_decls_for_prec: "<tag>" token_decl_for_prec.1  */
    {
      (yyval.token_decls_for_prec) = चिह्नम्_list_type_समुच्चयः ((yyvsp[0].yykind_87), (yyvsp[-1].चिह्नकम्_2));
    }
    भङ्ग;

  पक्ष 81: /* token_decls_for_prec: token_decls_for_prec "<tag>" token_decl_for_prec.1  */
    {
      (yyval.token_decls_for_prec) = चिह्नम्_list_append ((yyvsp[-2].token_decls_for_prec), चिह्नम्_list_type_समुच्चयः ((yyvsp[0].yykind_87), (yyvsp[-1].चिह्नकम्_2)));
    }
    भङ्ग;

  पक्ष 82: /* token_decl_for_prec.1: token_decl_for_prec  */
    { (yyval.yykind_87) = चिह्नम्_list_sym_new ((yyvsp[0].token_decl_for_prec), (yylsp[0])); }
    भङ्ग;

  पक्ष 83: /* token_decl_for_prec.1: token_decl_for_prec.1 token_decl_for_prec  */
    { (yyval.yykind_87) = चिह्नम्_list_append ((yyvsp[-1].yykind_87), चिह्नम्_list_sym_new ((yyvsp[0].token_decl_for_prec), (yylsp[0]))); }
    भङ्ग;

  पक्ष 84: /* token_decl_for_prec: id int.opt  */
    {
      (yyval.token_decl_for_prec) = (yyvsp[-1].id);
      चिह्नम्_class_समुच्चयः ((yyvsp[-1].id), token_sym, (yylsp[-1]), false);
      यदि (0 <= (yyvsp[0].yykind_84))
        चिह्नम्_संहिता_समुच्चयः ((yyvsp[-1].id), (yyvsp[0].yykind_84), (yylsp[0]));
    }
    भङ्ग;

  पक्ष 86: /* symbol_decls: symbols.1  */
    {
      (yyval.चिह्नम्_decls) = (yyvsp[0].yykind_90);
    }
    भङ्ग;

  पक्ष 87: /* symbol_decls: "<tag>" symbols.1  */
    {
      (yyval.चिह्नम्_decls) = चिह्नम्_list_type_समुच्चयः ((yyvsp[0].yykind_90), (yyvsp[-1].चिह्नकम्_2));
    }
    भङ्ग;

  पक्ष 88: /* symbol_decls: symbol_decls "<tag>" symbols.1  */
    {
      (yyval.चिह्नम्_decls) = चिह्नम्_list_append ((yyvsp[-2].चिह्नम्_decls), चिह्नम्_list_type_समुच्चयः ((yyvsp[0].yykind_90), (yyvsp[-1].चिह्नकम्_2)));
    }
    भङ्ग;

  पक्ष 89: /* symbols.1: symbol  */
    {
      यदि (current_class != unknown_sym)
        चिह्नम्_class_समुच्चयः ((yyvsp[0].चिह्नम्), current_class, (yylsp[0]), false);
      (yyval.yykind_90) = चिह्नम्_list_sym_new ((yyvsp[0].चिह्नम्), (yylsp[0]));
    }
    भङ्ग;

  पक्ष 90: /* symbols.1: symbols.1 symbol  */
    {
      यदि (current_class != unknown_sym)
        चिह्नम्_class_समुच्चयः ((yyvsp[0].चिह्नम्), current_class, (yylsp[0]), false);
      (yyval.yykind_90) = चिह्नम्_list_append ((yyvsp[-1].yykind_90), चिह्नम्_list_sym_new ((yyvsp[0].चिह्नम्), (yylsp[0])));
    }
    भङ्ग;

  पक्ष 95: /* rules_or_grammar_declaration: error ";"  */
    {
      yyerrok;
    }
    भङ्ग;

  पक्ष 96: /* $@5: %empty  */
                         { current_lhs ((yyvsp[-1].id_colon), (yylsp[-1]), (yyvsp[0].yykind_97)); }
    भङ्ग;

  पक्ष 97: /* rules: id_colon named_ref.opt $@5 ":" rhses.1  */
    {
      /* Free the current lhs. */
      current_lhs (0, (yylsp[-4]), 0);
    }
    भङ्ग;

  पक्ष 98: /* rhses.1: rhs  */
                     { grammar_current_नियमः_अन्तः ((yylsp[0])); }
    भङ्ग;

  पक्ष 99: /* rhses.1: rhses.1 "|" rhs  */
                     { grammar_current_नियमः_अन्तः ((yylsp[0])); }
    भङ्ग;

  पक्ष 101: /* rhs: %empty  */
    { grammar_current_नियमः_begin (current_lhs_चिह्नम्, current_lhs_loc,
                                  current_lhs_named_ref); }
    भङ्ग;

  पक्ष 102: /* rhs: rhs symbol named_ref.opt  */
    { grammar_current_नियमः_चिह्नम्_append ((yyvsp[-1].चिह्नम्), (yylsp[-1]), (yyvsp[0].yykind_97)); }
    भङ्ग;

  पक्ष 103: /* rhs: rhs tag.opt "{...}" named_ref.opt  */
    { grammar_current_नियमः_action_append ((yyvsp[-1].BRACED_संहिता), (yylsp[-1]), (yyvsp[0].yykind_97), (yyvsp[-2].yykind_76)); }
    भङ्ग;

  पक्ष 104: /* rhs: rhs "%?{...}"  */
    { grammar_current_नियमः_predicate_append ((yyvsp[0].BRACED_PREDICATE), (yylsp[0])); }
    भङ्ग;

  पक्ष 105: /* rhs: rhs "%empty"  */
    { grammar_current_नियमः_रिक्तानि_समुच्चयः ((yylsp[0])); }
    भङ्ग;

  पक्ष 106: /* rhs: rhs "%prec" symbol  */
    { grammar_current_नियमः_prec_समुच्चयः ((yyvsp[0].चिह्नम्), (yylsp[0])); }
    भङ्ग;

  पक्ष 107: /* rhs: rhs "%dprec" "integer literal"  */
    { grammar_current_नियमः_dprec_समुच्चयः ((yyvsp[0].INT_LITERAL), (yylsp[0])); }
    भङ्ग;

  पक्ष 108: /* rhs: rhs "%merge" "<tag>"  */
    { grammar_current_नियमः_merge_समुच्चयः ((yyvsp[0].चिह्नकम्_2), (yylsp[0])); }
    भङ्ग;

  पक्ष 109: /* rhs: rhs "%expect" "integer literal"  */
    { grammar_current_नियमः_expect_sr ((yyvsp[0].INT_LITERAL), (yylsp[0])); }
    भङ्ग;

  पक्ष 110: /* rhs: rhs "%expect-rr" "integer literal"  */
    { grammar_current_नियमः_expect_rr ((yyvsp[0].INT_LITERAL), (yylsp[0])); }
    भङ्ग;

  पक्ष 111: /* named_ref.opt: %empty  */
                 { (yyval.yykind_97) = रिक्त; }
    भङ्ग;

  पक्ष 112: /* named_ref.opt: "[identifier]"  */
                 { (yyval.yykind_97) = named_ref_new ((yyvsp[0].BRACKETED_ID), (yylsp[0])); }
    भङ्ग;

  पक्ष 114: /* value: %empty  */
          { (yyval.मानम्).विधा = muscle_keyword; (yyval.मानम्).chars = ""; }
    भङ्ग;

  पक्ष 115: /* value: "identifier"  */
          { (yyval.मानम्).विधा = muscle_keyword; (yyval.मानम्).chars = (yyvsp[0].ID); }
    भङ्ग;

  पक्ष 116: /* value: "string"  */
          { (yyval.मानम्).विधा = muscle_string;  (yyval.मानम्).chars = unquote ((yyvsp[0].STRING)); gram_scanner_चरमः_string_free ();}
    भङ्ग;

  पक्ष 117: /* value: "{...}"  */
          { (yyval.मानम्).विधा = muscle_संहिता;    (yyval.मानम्).chars = strip_braces ((yyvsp[0].BRACED_संहिता)); gram_scanner_चरमः_string_free (); }
    भङ्ग;

  पक्ष 118: /* id: "identifier"  */
    { (yyval.id) = चिह्नम्_from_uniqstr ((yyvsp[0].ID), (yylsp[0])); }
    भङ्ग;

  पक्ष 119: /* id: "character literal"  */
    {
      ध्रुव वर्ण *var = "api.token.raw";
      यदि (current_class == nterm_sym)
        {
          complain (&(yylsp[0]), complaint,
                    _("character literals cannot be nonterminals"));
          YYERROR;
        }
      यदि (muscle_percent_define_ifdef (var))
        {
          complain (&(yylsp[0]), complaint,
                    _("character literals cannot be used together"
                    " with %s"), var);
          location loc = muscle_percent_define_get_loc (var);
          subcomplain (&loc, complaint, _("definition of %s"), var);
        }
      (yyval.id) = चिह्नम्_get (char_name ((yyvsp[0].CHAR_LITERAL)), (yylsp[0]));
      चिह्नम्_class_समुच्चयः ((yyval.id), token_sym, (yylsp[0]), false);
      चिह्नम्_संहिता_समुच्चयः ((yyval.id), (yyvsp[0].CHAR_LITERAL), (yylsp[0]));
    }
    भङ्ग;

  पक्ष 120: /* id_colon: "identifier:"  */
           { (yyval.id_colon) = चिह्नम्_from_uniqstr ((yyvsp[0].ID_COLON), (yylsp[0])); }
    भङ्ग;

  पक्ष 123: /* string_as_id: "string"  */
    {
      (yyval.string_as_id) = चिह्नम्_get ((yyvsp[0].STRING), (yylsp[0]));
      चिह्नम्_class_समुच्चयः ((yyval.string_as_id), token_sym, (yylsp[0]), false);
    }
    भङ्ग;

  पक्ष 125: /* epilogue.opt: "%%" "epilogue"  */
    {
      muscle_संहिता_grow ("epilogue", translate_संहिता ((yyvsp[0].EPILOGUE), (yylsp[0]), true), (yylsp[0]));
      संहिता_scanner_चरमः_string_free ();
    }
    भङ्ग;



        स्वभाव: भङ्ग;
      }
    यदि (yychar_backup != yychar)
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
  YY_चिह्नम्_PRINT ("-> $$ =", YY_CAST (yysymbol_विधा_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    ध्रुव पूर्णाङ्क yylhs = yyr1[yyn] - YYNTOKENS;
    ध्रुव पूर्णाङ्क yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  गच्छ yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == GRAM_रिक्तानि ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  यदि (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yyesa, &yyes, &yyes_क्षमता, yytoken, &yylloc};
        यदि (yychar != GRAM_रिक्तानि)
          YY_LAC_ESTABLISH;
        यदि (yyreport_syntax_error (&yyctx) == 2)
          YYNOMEM;
      }
    }

  yyerror_range[1] = yylloc;
  यदि (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      यदि (yychar <= GRAM_EOF)
        {
          /* Return failure if at end of input.  */
          यदि (yychar == GRAM_EOF)
            YYABORT;
        }
      अन्यथा
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc);
          yychar = GRAM_रिक्तानि;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  गच्छ yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  यदि (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_राशिः_PRINT (yyss, yyssp);
  yystate = *yyssp;
  गच्छ yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  प्रति (;;)
    {
      yyn = yypact[yystate];
      यदि (!yypact_मानम्_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          यदि (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              यदि (0 < yyn)
                भङ्ग;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      यदि (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_चिह्नम् (yystate), yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_राशिः_PRINT (yyss, yyssp);
    }

  /* If the stack popping above didn't lose the initial context for the
     current lookahead token, the shift below will for sure.  */
  YY_LAC_DISCARD ("error recovery");

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_अन्तः

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

  /* Shift the error token.  */
  YY_चिह्नम्_PRINT ("Shifting", YY_ACCESSING_चिह्नम् (yyn), yyvsp, yylsp);

  yystate = yyn;
  गच्छ yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  गच्छ yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  गच्छ yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, YY_("memory exhausted"));
  yyresult = 2;
  गच्छ yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  यदि (yychar != GRAM_रिक्तानि)
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
  YY_राशिः_PRINT (yyss, yyssp);
  यावत् (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_चिह्नम् (+*yyssp), yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#यदि_अपरिभाषितम् yyoverflow
  यदि (yyss != yyssa)
    YYSTACK_FREE (yyss);
#यदि_अन्तः
  यदि (yyes != yyesa)
    YYSTACK_FREE (yyes);

  प्रत्यागम yyresult;
}



पूर्णाङ्क
yyreport_syntax_error (ध्रुव yypcontext_t *ctx)
{
  पूर्णाङ्क res = 0;
  /* Arguments of format: reported tokens (one for the "unexpected",
     one per "expected"). */
  गणना { तर्काः_MAX = 5 };
  ध्रुव वर्ण *argv[तर्काः_MAX];
  पूर्णाङ्क argc = 0;
  yysymbol_विधा_t unexpected = yypcontext_token (ctx);
  यदि (unexpected != YYSYMBOL_YYEMPTY)
    {
      argv[argc++] = yysymbol_name (unexpected);
      yysymbol_विधा_t expected[तर्काः_MAX - 1];
      पूर्णाङ्क nexpected = yypcontext_expected_tokens (ctx, expected, तर्काः_MAX - 1);
      यदि (nexpected < 0)
        res = nexpected;
      अन्यथा
        प्रति (पूर्णाङ्क i = 0; i < nexpected; ++i)
          argv[argc++] = yysymbol_name (expected[i]);
    }
  syntax_error (*yypcontext_location (ctx), argc, argv);
  प्रत्यागम res;
}


/* Return the location of the left-hand side of a rule whose
   right-hand side is RHS[1] ... RHS[N].  Ignore empty nonterminals in
   the right-hand side, and return an empty location equal to the end
   boundary of RHS[0] if the right-hand side is empty.  */

स्थिर YYLTYPE
lloc_default (YYLTYPE ध्रुव *rhs, पूर्णाङ्क n)
{
  YYLTYPE loc;

  /* SGI MIPSpro 7.4.1m miscompiles "loc.start = loc.end = rhs[n].end;".
     The bug is fixed in 7.4.2m, but play it safe for now.  */
  loc.start = rhs[n].अन्तः;
  loc.अन्तः = rhs[n].अन्तः;

  /* Ignore empty nonterminals the start of the right-hand side.
     Do not bother to ignore them at the end of the right-hand side,
     since empty nonterminals have the same end as their predecessors.  */
  प्रति (पूर्णाङ्क i = 1; i <= n; i++)
    यदि (! equal_boundaries (rhs[i].start, rhs[i].अन्तः))
      {
        loc.start = rhs[i].start;
        भङ्ग;
      }

  प्रत्यागम loc;
}

स्थिर
वर्ण *strip_braces (वर्ण *संहिता)
{
  संहिता[वाक्यदैर्घ्यम् (संहिता) - 1] = 0;
  प्रत्यागम संहिता + 1;
}

स्थिर
वर्ण ध्रुव *
translate_संहिता (वर्ण *संहिता, location loc, bool plain)
{
  संहिता_props plain_संहिता;
  यदि (plain)
    संहिता_props_plain_init (&plain_संहिता, संहिता, loc);
  अन्यथा
    संहिता_props_चिह्नम्_action_init (&plain_संहिता, संहिता, loc);
  संहिता_props_translate_संहिता (&plain_संहिता);
  gram_scanner_चरमः_string_free ();
  प्रत्यागम plain_संहिता.संहिता;
}

स्थिर
वर्ण ध्रुव *
translate_संहिता_braceless (वर्ण *संहिता, location loc)
{
  प्रत्यागम translate_संहिता (strip_braces (संहिता), loc, true);
}

स्थिर शून्य
सङ्कलय_param (param_type type, वर्ण *decl, location loc)
{
  स्थिर वर्ण ध्रुव alphanum[26 + 26 + 1 + 10 + 1] =
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "_"
    "0123456789";

  वर्ण ध्रुव *name_start = रिक्त;
  {
    वर्ण *p;
    /* Stop on last actual character.  */
    प्रति (p = decl; p[1]; p++)
      यदि ((p == decl
           || ! memchr (alphanum, p[-1], परिमाण alphanum - 1))
          && memchr (alphanum, p[0], परिमाण alphanum - 10 - 1))
        name_start = p;

    /* Strip the surrounding '{' and '}', and any blanks just inside
       the braces.  */
    --p;
    यावत् (c_isspace ((अचिह्नित वर्ण) *p))
      --p;
    p[1] = '\0';
    ++decl;
    यावत् (c_isspace ((अचिह्नित वर्ण) *decl))
      ++decl;
  }

  यदि (! name_start)
    complain (&loc, complaint, _("missing identifier in parameter declaration"));
  अन्यथा
    {
      वर्ण *name = xmemdup0 (name_start, strspn (name_start, alphanum));
      यदि (type & param_lex)
        muscle_pair_list_grow ("lex_param", decl, name);
      यदि (type & param_parse)
        muscle_pair_list_grow ("parse_param", decl, name);
      मुक्त (name);
    }

  gram_scanner_चरमः_string_free ();
}


स्थिर शून्य
handle_शीर्षकम् (वर्ण ध्रुव *मानम्)
{
  शीर्षकम्_flag = true;
  यदि (मानम्)
    {
      वर्ण *file = unquote (मानम्);
      spec_शीर्षकम्_file = xstrdup (file);
      gram_scanner_चरमः_string_free ();
      unquote_free (file);
    }
}


स्थिर शून्य
handle_error_verbose (location ध्रुव *loc, वर्ण ध्रुव *directive)
{
  bison_directive (loc, directive);
  muscle_percent_define_निवेशय (directive, *loc, muscle_keyword, "",
                                MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
}


स्थिर शून्य
handle_file_उपसर्गः (location ध्रुव *loc,
                    location ध्रुव *dir_loc,
                    वर्ण ध्रुव *directive, वर्ण ध्रुव *मानम्_quoted)
{
  वर्ण *मानम् = unquote (मानम्_quoted);
  bison_directive (loc, directive);
  bool warned = false;

  यदि (location_रिक्तानि (spec_file_उपसर्गः_loc))
    {
      spec_file_उपसर्गः_loc = *loc;
      spec_file_उपसर्गः = मानम्;
    }
  अन्यथा
    {
      duplicate_directive (directive, spec_file_उपसर्गः_loc, *loc);
      warned = true;
    }

  यदि (!warned
      && STRNEQ (directive, "%file-prefix"))
    deprecated_directive (dir_loc, directive, "%file-prefix");
}

स्थिर शून्य
handle_language (location ध्रुव *loc, वर्ण ध्रुव *lang)
{
  language_argmatch (unquote (lang), grammar_prio, *loc);
}


स्थिर शून्य
handle_name_उपसर्गः (location ध्रुव *loc,
                    वर्ण ध्रुव *directive, वर्ण ध्रुव *मानम्_quoted)
{
  वर्ण *मानम् = unquote (मानम्_quoted);
  bison_directive (loc, directive);

  वर्ण buf1[1024];
  size_t len1 = परिमाण (buf1);
  वर्ण *old = asnprintf (buf1, &len1, "%s\"%s\"", directive, मानम्);
  यदि (!old)
    xalloc_die ();

  यदि (location_रिक्तानि (spec_name_उपसर्गः_loc))
    {
      spec_name_उपसर्गः = मानम्;
      spec_name_उपसर्गः_loc = *loc;

      वर्ण buf2[1024];
      size_t len2 = परिमाण (buf2);
      वर्ण *new = asnprintf (buf2, &len2, "%%define api.prefix {%s}", मानम्);
      यदि (!new)
        xalloc_die ();
      deprecated_directive (loc, old, new);
      यदि (new != buf2)
        मुक्त (new);
    }
  अन्यथा
    duplicate_directive (old, spec_file_उपसर्गः_loc, *loc);

  यदि (old != buf1)
    मुक्त (old);
}


स्थिर शून्य
handle_विशुद्धम्_parser (location ध्रुव *loc, वर्ण ध्रुव *directive)
{
  bison_directive (loc, directive);
  deprecated_directive (loc, directive, "%define api.pure");
  muscle_percent_define_निवेशय ("api.pure", *loc, muscle_keyword, "",
                                MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
}


स्थिर शून्य
handle_require (location ध्रुव *loc, वर्ण ध्रुव *संस्करणम्_quoted)
{
  वर्ण *संस्करणम् = unquote (संस्करणम्_quoted);
  required_संस्करणम् = strversion_गन्तव्यम्_int (संस्करणम्);
  यदि (required_संस्करणम् == -1)
    {
      complain (loc, complaint, _("invalid version requirement: %s"),
                संस्करणम्);
      required_संस्करणम् = 0;
    }
  अन्यथा
    {
      ध्रुव वर्ण* package_संस्करणम् =
        0 < strverscmp (api_संस्करणम्, PACKAGE_संस्करणम्)
        ? api_संस्करणम् : PACKAGE_संस्करणम्;
      यदि (0 < strverscmp (संस्करणम्, package_संस्करणम्))
        {
          complain (loc, complaint, _("require bison %s, but have %s"),
                    संस्करणम्, package_संस्करणम्);
          निर्गच्छ (EX_MISMATCH);
        }
    }
  unquote_free (संस्करणम्);
  gram_scanner_चरमः_string_free ();
}

स्थिर शून्य
handle_skeleton (location ध्रुव *loc, वर्ण ध्रुव *skel_quoted)
{
  वर्ण *skel = unquote (skel_quoted);
  वर्ण ध्रुव *skeleton_user = skel;
  यदि (strchr (skeleton_user, '/'))
    {
      size_t dir_दीर्घता = वाक्यदैर्घ्यम् (grammar_file);
      यावत् (dir_दीर्घता && grammar_file[dir_दीर्घता - 1] != '/')
        --dir_दीर्घता;
      यावत् (dir_दीर्घता && grammar_file[dir_दीर्घता - 1] == '/')
        --dir_दीर्घता;
      वर्ण *skeleton_build =
        xmalloc (dir_दीर्घता + 1 + वाक्यदैर्घ्यम् (skeleton_user) + 1);
      यदि (dir_दीर्घता > 0)
        {
          स्मृतिं_प्रतिलिख (skeleton_build, grammar_file, dir_दीर्घता);
          skeleton_build[dir_दीर्घता++] = '/';
        }
      वाक्यं_प्रतिलिख (skeleton_build + dir_दीर्घता, skeleton_user);
      skeleton_user = uniqstr_new (skeleton_build);
      मुक्त (skeleton_build);
    }
  skeleton_arg (skeleton_user, grammar_prio, *loc);
}


स्थिर शून्य
handle_yacc (location ध्रुव *loc)
{
  ध्रुव वर्ण *directive = "%yacc";
  bison_directive (loc, directive);
  यदि (location_रिक्तानि (yacc_loc))
    समुच्चयः_yacc (*loc);
  अन्यथा
    duplicate_directive (directive, yacc_loc, *loc);
}


स्थिर शून्य
gram_error (location ध्रुव *loc, वर्ण ध्रुव *सन्देशः_2)
{
  complain (loc, complaint, "%s", सन्देशः_2);
}

स्थिर वर्ण ध्रुव *
char_name (वर्ण c)
{
  यदि (c == '\'')
    प्रत्यागम "'\\''";
  अन्यथा
    {
      वर्ण buf[4];
      buf[0] = '\''; buf[1] = c; buf[2] = '\''; buf[3] = '\0';
      प्रत्यागम quotearg_style (escape_quoting_style, buf);
    }
}

स्थिर शून्य
current_lhs (चिह्नम् *sym, location loc, named_ref *ref)
{
  current_lhs_चिह्नम् = sym;
  current_lhs_loc = loc;
  यदि (sym)
    चिह्नम्_location_as_lhs_समुच्चयः (sym, loc);
  /* In order to simplify memory management, named references for lhs
     are always assigned by deep copy into the current symbol_list
     node.  This is because a single named-ref in the grammar may
     result in several uses when the user factors lhs between several
     rules using "|".  Therefore free the parser's original copy.  */
  मुक्त (current_lhs_named_ref);
  current_lhs_named_ref = ref;
}

स्थिर शून्य tron (FILE *yyo)
{
  begin_use_class ("value", yyo);
}

स्थिर शून्य troff (FILE *yyo)
{
  अन्तः_use_class ("value", yyo);
}


/*----------.
| Unquote.  |
`----------*/

संरचना obstack obstack_for_unquote;

शून्य
parser_init (शून्य)
{
  obstack_init (&obstack_for_unquote);
}

शून्य
parser_free (शून्य)
{
  obstack_free (&obstack_for_unquote, 0);
}

स्थिर शून्य
unquote_free (वर्ण *चरमः_string)
{
  obstack_free (&obstack_for_unquote, चरमः_string);
}

स्थिर वर्ण *
unquote (ध्रुव वर्ण *cp)
{
#परिभाषय GROW(Char)                              \
  obstack_1grow (&obstack_for_unquote, Char);
  प्रति (++cp; *cp && *cp != '"'; ++cp)
    विकल्प (*cp)
      {
      पक्ष '"':
        भङ्ग;
      पक्ष '\\':
        ++cp;
        विकल्प (*cp)
          {
          पक्ष '0': पक्ष '1': पक्ष '2': पक्ष '3': पक्ष '4':
          पक्ष '5': पक्ष '6': पक्ष '7': पक्ष '8': पक्ष '9':
            {
              पूर्णाङ्क c = cp[0] - '0';
              यदि (c_isdigit (cp[1]))
                {
                  ++cp;
                  c = c * 8 + cp[0] - '0';
                }
              यदि (c_isdigit (cp[1]))
                {
                  ++cp;
                  c = c * 8 + cp[0] - '0';
                }
              GROW (c);
            }
            भङ्ग;

          पक्ष 'a': GROW ('\a'); भङ्ग;
          पक्ष 'b': GROW ('\b'); भङ्ग;
          पक्ष 'f': GROW ('\f'); भङ्ग;
          पक्ष 'n': GROW ('\n'); भङ्ग;
          पक्ष 'r': GROW ('\r'); भङ्ग;
          पक्ष 't': GROW ('\t'); भङ्ग;
          पक्ष 'v': GROW ('\v'); भङ्ग;

          पक्ष 'x':
            {
              पूर्णाङ्क c = 0;
              यावत् (c_isxdigit (cp[1]))
                {
                  ++cp;
                  c = (c * 16 + (c_isdigit (cp[0]) ? cp[0] - '0'
                                 : c_isupper (cp[0]) ? cp[0] - 'A'
                                 : cp[0] - '0'));
                }
              GROW (c);
              भङ्ग;
            }
          }
        भङ्ग;

      स्वभाव:
        GROW (*cp);
        भङ्ग;
      }
  प्रतिज्ञा (*cp == '"');
  ++cp;
  प्रतिज्ञा (*cp == '\0');
#अपरिभाषय GROW
  प्रत्यागम obstack_finish0 (&obstack_for_unquote);
}
