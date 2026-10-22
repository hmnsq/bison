/* A Bison parser, made by GNU Bison 3.8.2.30-82269.  */

/* Bison interface for Yacc-like parsers in C

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ YY_GRAM_SRC_PARSE_GRAM_H_INCLUDED
# define YY_GRAM_SRC_PARSE_GRAM_H_INCLUDED
/* Debug traces.  */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ GRAM_DEBUG
# ಆದರೆ defined YYDEBUG
#ಆದರೆ YYDEBUG
#   define GRAM_DEBUG 1
#  ಇಲ್ಲದಿದ್ದರೆ
#   define GRAM_DEBUG 0
#  endif
# ಇಲ್ಲದಿದ್ದರೆ /* ! defined YYDEBUG */
#  define GRAM_DEBUG 1
# endif /* ! defined YYDEBUG */
#ಆದರೆ_ಅಂತ್ಯ  /* ! defined GRAM_DEBUG */
#ಆದರೆ GRAM_DEBUG
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ gram_debug;
#ಆದರೆ_ಅಂತ್ಯ
/* "%code requires" blocks.  */

  #ಅನ್ತರ್ಭಾವ "symlist.h"
  #ಅನ್ತರ್ಭಾವ "symtab.h"

  ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಎಣಿಕೆ
  {
    param_none   = 0,
    param_lex    = 1 << 0,
    param_parse  = 1 << 1,
    param_both   = param_lex | param_parse
  } param_type;

  #ಅನ್ತರ್ಭಾವ "muscle-tab.h"
  ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ
  {
    ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *chars;
    muscle_ಬಗೆ ಬಗೆ;
  } ಮೌಲ್ಯ_type;


/* Token kinds.  */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ GRAM_TOKENTYPE
# define GRAM_TOKENTYPE
  ಎಣಿಕೆ gram_tokentype
  {
    GRAM_ಖಾಲಿಗಳು = -2,
    GRAM_EOF = 0,                  /* "end of file"  */
    GRAM_error = 1,                /* error  */
    GRAM_UNDEF = 2,                /* "invalid token"  */
    STRING = 3,                    /* "string"  */
    TSTRING = 4,                   /* "translatable string"  */
    PERCENT_TOKEN = 5,             /* "%token"  */
    PERCENT_NTERM = 6,             /* "%nterm"  */
    PERCENT_TYPE = 7,              /* "%type"  */
    PERCENT_DESTRUCTOR = 8,        /* "%destructor"  */
    PERCENT_PRINTER = 9,           /* "%printer"  */
    PERCENT_ಎಡ = 10,             /* "%left"  */
    PERCENT_ಬಲ = 11,            /* "%right"  */
    PERCENT_NONASSOC = 12,         /* "%nonassoc"  */
    PERCENT_PRECEDENCE = 13,       /* "%precedence"  */
    PERCENT_PREC = 14,             /* "%prec"  */
    PERCENT_DPREC = 15,            /* "%dprec"  */
    PERCENT_ವಿಲೀನ = 16,            /* "%merge"  */
    PERCENT_ಸಂಕೇತ = 17,             /* "%code"  */
    PERCENT_DEFAULT_PREC = 18,     /* "%default-prec"  */
    PERCENT_DEFINE = 19,           /* "%define"  */
    PERCENT_ERROR_VERBOSE = 20,    /* "%error-verbose"  */
    PERCENT_EXPECT = 21,           /* "%expect"  */
    PERCENT_EXPECT_RR = 22,        /* "%expect-rr"  */
    PERCENT_FILE_ಪೂರ್ವಪ್ರತ್ಯಯ = 23,      /* "%file-prefix"  */
    PERCENT_FLAG = 24,             /* "%<flag>"  */
    PERCENT_GLR_PARSER = 25,       /* "%glr-parser"  */
    PERCENT_ಶೀರ್ಷಿಕೆ = 26,           /* "%header"  */
    PERCENT_INITIAL_ACTION = 27,   /* "%initial-action"  */
    PERCENT_LANGUAGE = 28,         /* "%language"  */
    PERCENT_NAME_ಪೂರ್ವಪ್ರತ್ಯಯ = 29,      /* "%name-prefix"  */
    PERCENT_NO_DEFAULT_PREC = 30,  /* "%no-default-prec"  */
    PERCENT_NO_ಸಾಲುಗಳು = 31,         /* "%no-lines"  */
    PERCENT_NONDETERMINISTIC_PARSER = 32, /* "%nondeterministic-parser"  */
    PERCENT_OUTPUT = 33,           /* "%output"  */
    PERCENT_ಶುದ್ಧ_PARSER = 34,      /* "%pure-parser"  */
    PERCENT_REQUIRE = 35,          /* "%require"  */
    PERCENT_SKELETON = 36,         /* "%skeleton"  */
    PERCENT_ಆರಂಭ = 37,            /* "%start"  */
    PERCENT_TOKEN_ಕೋಷ್ಟಕ = 38,      /* "%token-table"  */
    PERCENT_VERBOSE = 39,          /* "%verbose"  */
    PERCENT_YACC = 40,             /* "%yacc"  */
    BRACED_ಸಂಕೇತ = 41,              /* "{...}"  */
    BRACED_PREDICATE = 42,         /* "%?{...}"  */
    BRACKETED_ID = 43,             /* "[identifier]"  */
    CHAR_LITERAL = 44,             /* "character literal"  */
    COLON = 45,                    /* ":"  */
    EPILOGUE = 46,                 /* "epilogue"  */
    EQUAL = 47,                    /* "="  */
    ID = 48,                       /* "identifier"  */
    ID_COLON = 49,                 /* "identifier:"  */
    PERCENT_PERCENT = 50,          /* "%%"  */
    PIPE = 51,                     /* "|"  */
    PROLOGUE = 52,                 /* "%{...%}"  */
    SEMICOLON = 53,                /* ";"  */
    ಟ್ಯಾಗ್_2 = 54,                      /* "<tag>"  */
    ಟ್ಯಾಗ್_ANY = 55,                  /* "<*>"  */
    ಟ್ಯಾಗ್_NONE = 56,                 /* "<>"  */
    INT_LITERAL = 57,              /* "integer literal"  */
    PERCENT_PARAM = 58,            /* "%param"  */
    PERCENT_UNION = 59,            /* "%union"  */
    PERCENT_ಖಾಲಿಗಳು = 60             /* "%empty"  */
  };
  ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಎಣಿಕೆ gram_tokentype gram_token_ಬಗೆ_t;
#ಆದರೆ_ಅಂತ್ಯ

/* Value type.  */
#ಆದರೆ ! defined GRAM_STYPE && ! defined GRAM_STYPE_IS_DECLARED
ಒಕ್ಕೂಟ GRAM_STYPE
{
  assoc precedence_declarator;             /* precedence_declarator  */
  ಅಕ್ಷರ* STRING;                            /* "string"  */
  ಅಕ್ಷರ* TSTRING;                           /* "translatable string"  */
  ಅಕ್ಷರ* BRACED_ಸಂಕೇತ;                       /* "{...}"  */
  ಅಕ್ಷರ* BRACED_PREDICATE;                  /* "%?{...}"  */
  ಅಕ್ಷರ* EPILOGUE;                          /* "epilogue"  */
  ಅಕ್ಷರ* PROLOGUE;                          /* "%{...%}"  */
  ಅಕ್ಷರ* yykind_75;                         /* string.opt  */
  ಸಂಕೇತ_props_type ಸಂಕೇತ_props_type;         /* code_props_type  */
  ಪೂರ್ಣಾಂಕ INT_LITERAL;                         /* "integer literal"  */
  ಪೂರ್ಣಾಂಕ yykind_84;                           /* int.opt  */
  named_ref* yykind_97;                    /* named_ref.opt  */
  param_type PERCENT_PARAM;                /* "%param"  */
  ಚಿಹ್ನೆ* token_decl;                      /* token_decl  */
  ಚಿಹ್ನೆ* alias;                           /* alias  */
  ಚಿಹ್ನೆ* token_decl_for_prec;             /* token_decl_for_prec  */
  ಚಿಹ್ನೆ* id;                              /* id  */
  ಚಿಹ್ನೆ* id_colon;                        /* id_colon  */
  ಚಿಹ್ನೆ* ಚಿಹ್ನೆ;                          /*ಚಿಹ್ನೆ*/
  ಚಿಹ್ನೆ* string_as_id;                    /* string_as_id  */
  ಚಿಹ್ನೆ_list* generic_symlist;            /* generic_symlist  */
  ಚಿಹ್ನೆ_list* generic_symlist_ಅಂಶ;       /* generic_symlist_item  */
  ಚಿಹ್ನೆ_list* nterm_decls;                /* nterm_decls  */
  ಚಿಹ್ನೆ_list* token_decls;                /* token_decls  */
  ಚಿಹ್ನೆ_list* yykind_82;                  /* token_decl.1  */
  ಚಿಹ್ನೆ_list* token_decls_for_prec;       /* token_decls_for_prec  */
  ಚಿಹ್ನೆ_list* yykind_87;                  /* token_decl_for_prec.1  */
  ಚಿಹ್ನೆ_list* ಚಿಹ್ನೆ_decls;               /* symbol_decls  */
  ಚಿಹ್ನೆ_list* yykind_90;                  /* symbols.1  */
  uniqstr PERCENT_ERROR_VERBOSE;           /* "%error-verbose"  */
  uniqstr PERCENT_FILE_ಪೂರ್ವಪ್ರತ್ಯಯ;             /* "%file-prefix"  */
  uniqstr PERCENT_FLAG;                    /* "%<flag>"  */
  uniqstr PERCENT_NAME_ಪೂರ್ವಪ್ರತ್ಯಯ;             /* "%name-prefix"  */
  uniqstr PERCENT_ಶುದ್ಧ_PARSER;             /* "%pure-parser"  */
  uniqstr BRACKETED_ID;                    /* "[identifier]"  */
  uniqstr ID;                              /* "identifier"  */
  uniqstr ID_COLON;                        /* "identifier:"  */
  uniqstr ಟ್ಯಾಗ್_2;                             /* "<tag>"  */
  uniqstr yykind_76;                       /* tag.opt  */
  uniqstr ಟ್ಯಾಗ್;                             /*ಟ್ಯಾಗ್*/
  uniqstr variable;                        /* variable  */
  ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ CHAR_LITERAL;              /* "character literal"  */
  ಮೌಲ್ಯ_type ಮೌಲ್ಯ;                        /*ಮೌಲ್ಯ*/


};
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಒಕ್ಕೂಟ GRAM_STYPE GRAM_STYPE;
# define GRAM_STYPE_IS_TRIVIAL 1
# define GRAM_STYPE_IS_DECLARED 1
#ಆದರೆ_ಅಂತ್ಯ

/* Location type.  */
#ಆದರೆ ! defined GRAM_LTYPE && ! defined GRAM_LTYPE_IS_DECLARED
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ GRAM_LTYPE GRAM_LTYPE;
ರಚನೆ GRAM_LTYPE
{
  ಪೂರ್ಣಾಂಕ ಮೊದಲ_ಸಾಲು;
  ಪೂರ್ಣಾಂಕ ಮೊದಲ_ಸ್ತಂಭ;
  ಪೂರ್ಣಾಂಕ ಕೊನೆಯ_ಸಾಲು;
  ಪೂರ್ಣಾಂಕ ಕೊನೆಯ_ಸ್ತಂಭ;
};
# define GRAM_LTYPE_IS_DECLARED 1
# define GRAM_LTYPE_IS_TRIVIAL 1
#ಆದರೆ_ಅಂತ್ಯ




ಪೂರ್ಣಾಂಕ gram_parse (ಶೂನ್ಯ);

/* "%code provides" blocks.  */

  /* Initialize unquote.  */
  ಶೂನ್ಯ parser_init (ಶೂನ್ಯ);
  /* Deallocate storage for unquote.  */
  ಶೂನ್ಯ parser_free (ಶೂನ್ಯ);


#ಆದರೆ_ಅಂತ್ಯ /* !YY_GRAM_SRC_PARSE_GRAM_H_INCLUDED  */
