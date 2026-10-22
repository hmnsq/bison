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

#ifndef YY_GRAM_SRC_PARSE_GRAM_H_INCLUDED
# define YY_GRAM_SRC_PARSE_GRAM_H_INCLUDED
/* Debug traces.  */
#ifndef GRAM_DEBUG
# यदि defined YYDEBUG
#यदि YYDEBUG
#   define GRAM_DEBUG 1
#  अन्यथा
#   define GRAM_DEBUG 0
#  endif
# अन्यथा /* ! defined YYDEBUG */
#  define GRAM_DEBUG 1
# endif /* ! defined YYDEBUG */
#endif  /* ! defined GRAM_DEBUG */
#यदि GRAM_DEBUG
बाह्य पूर्णाङ्क gram_debug;
#endif
/* "%code requires" blocks.  */

  #अन्तर्भाव "symlist.h"
  #अन्तर्भाव "symtab.h"

  प्रकारनिर्देश गणना
  {
    param_none   = 0,
    param_lex    = 1 << 0,
    param_parse  = 1 << 1,
    param_both   = param_lex | param_parse
  } param_type;

  #अन्तर्भाव "muscle-tab.h"
  प्रकारनिर्देश संरचना
  {
    वर्ण अचर *chars;
    muscle_kind kind;
  } मान_type;


/* Token kinds.  */
#ifndef GRAM_TOKENTYPE
# define GRAM_TOKENTYPE
  गणना gram_tokentype
  {
    GRAM_EMPTY = -2,
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
    PERCENT_बायाँ = 10,             /* "%left"  */
    PERCENT_दायाँ = 11,            /* "%right"  */
    PERCENT_NONASSOC = 12,         /* "%nonassoc"  */
    PERCENT_PRECEDENCE = 13,       /* "%precedence"  */
    PERCENT_PREC = 14,             /* "%prec"  */
    PERCENT_DPREC = 15,            /* "%dprec"  */
    PERCENT_MERGE = 16,            /* "%merge"  */
    PERCENT_CODE = 17,             /* "%code"  */
    PERCENT_DEFAULT_PREC = 18,     /* "%default-prec"  */
    PERCENT_DEFINE = 19,           /* "%define"  */
    PERCENT_ERROR_VERBOSE = 20,    /* "%error-verbose"  */
    PERCENT_EXPECT = 21,           /* "%expect"  */
    PERCENT_EXPECT_RR = 22,        /* "%expect-rr"  */
    PERCENT_FILE_PREFIX = 23,      /* "%file-prefix"  */
    PERCENT_FLAG = 24,             /* "%<flag>"  */
    PERCENT_GLR_PARSER = 25,       /* "%glr-parser"  */
    PERCENT_HEADER = 26,           /* "%header"  */
    PERCENT_INITIAL_ACTION = 27,   /* "%initial-action"  */
    PERCENT_LANGUAGE = 28,         /* "%language"  */
    PERCENT_NAME_PREFIX = 29,      /* "%name-prefix"  */
    PERCENT_NO_DEFAULT_PREC = 30,  /* "%no-default-prec"  */
    PERCENT_NO_पङ्क्तिहरू = 31,         /* "%no-lines"  */
    PERCENT_NONDETERMINISTIC_PARSER = 32, /* "%nondeterministic-parser"  */
    PERCENT_OUTPUT = 33,           /* "%output"  */
    PERCENT_PURE_PARSER = 34,      /* "%pure-parser"  */
    PERCENT_REQUIRE = 35,          /* "%require"  */
    PERCENT_SKELETON = 36,         /* "%skeleton"  */
    PERCENT_START = 37,            /* "%start"  */
    PERCENT_TOKEN_TABLE = 38,      /* "%token-table"  */
    PERCENT_VERBOSE = 39,          /* "%verbose"  */
    PERCENT_YACC = 40,             /* "%yacc"  */
    BRACED_CODE = 41,              /* "{...}"  */
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
    TAG = 54,                      /* "<tag>"  */
    TAG_ANY = 55,                  /* "<*>"  */
    TAG_NONE = 56,                 /* "<>"  */
    INT_LITERAL = 57,              /* "integer literal"  */
    PERCENT_PARAM = 58,            /* "%param"  */
    PERCENT_UNION = 59,            /* "%union"  */
    PERCENT_EMPTY = 60             /* "%empty"  */
  };
  प्रकारनिर्देश गणना gram_tokentype gram_token_kind_t;
#endif

/* Value type.  */
#यदि ! defined GRAM_STYPE && ! defined GRAM_STYPE_IS_DECLARED
सङ्घ GRAM_STYPE
{
  assoc precedence_declarator;             /* precedence_declarator  */
  वर्ण* STRING;                            /* "string"  */
  वर्ण* TSTRING;                           /* "translatable string"  */
  वर्ण* BRACED_CODE;                       /* "{...}"  */
  वर्ण* BRACED_PREDICATE;                  /* "%?{...}"  */
  वर्ण* EPILOGUE;                          /* "epilogue"  */
  वर्ण* PROLOGUE;                          /* "%{...%}"  */
  वर्ण* yykind_75;                         /* string.opt  */
  code_props_type code_props_type;         /* code_props_type  */
  पूर्णाङ्क INT_LITERAL;                         /* "integer literal"  */
  पूर्णाङ्क yykind_84;                           /* int.opt  */
  named_ref* yykind_97;                    /* named_ref.opt  */
  param_type PERCENT_PARAM;                /* "%param"  */
  चिह्न* token_decl;                      /* token_decl  */
  चिह्न* alias;                           /* alias  */
  चिह्न* token_decl_for_prec;             /* token_decl_for_prec  */
  चिह्न* id;                              /* id  */
  चिह्न* id_colon;                        /* id_colon  */
  चिह्न* चिह्न;                          /*चिह्न*/
  चिह्न* string_as_id;                    /* string_as_id  */
  चिह्न_list* generic_symlist;            /* generic_symlist  */
  चिह्न_list* generic_symlist_item;       /* generic_symlist_item  */
  चिह्न_list* nterm_decls;                /* nterm_decls  */
  चिह्न_list* token_decls;                /* token_decls  */
  चिह्न_list* yykind_82;                  /* token_decl.1  */
  चिह्न_list* token_decls_for_prec;       /* token_decls_for_prec  */
  चिह्न_list* yykind_87;                  /* token_decl_for_prec.1  */
  चिह्न_list* चिह्न_decls;               /* symbol_decls  */
  चिह्न_list* yykind_90;                  /* symbols.1  */
  uniqstr PERCENT_ERROR_VERBOSE;           /* "%error-verbose"  */
  uniqstr PERCENT_FILE_PREFIX;             /* "%file-prefix"  */
  uniqstr PERCENT_FLAG;                    /* "%<flag>"  */
  uniqstr PERCENT_NAME_PREFIX;             /* "%name-prefix"  */
  uniqstr PERCENT_PURE_PARSER;             /* "%pure-parser"  */
  uniqstr BRACKETED_ID;                    /* "[identifier]"  */
  uniqstr ID;                              /* "identifier"  */
  uniqstr ID_COLON;                        /* "identifier:"  */
  uniqstr TAG;                             /* "<tag>"  */
  uniqstr yykind_76;                       /* tag.opt  */
  uniqstr tag;                             /* tag  */
  uniqstr variable;                        /* variable  */
  अचिन्हित वर्ण CHAR_LITERAL;              /* "character literal"  */
  मान_type मान;                        /*मान*/


};
प्रकारनिर्देश सङ्घ GRAM_STYPE GRAM_STYPE;
# define GRAM_STYPE_IS_TRIVIAL 1
# define GRAM_STYPE_IS_DECLARED 1
#endif

/* Location type.  */
#यदि ! defined GRAM_LTYPE && ! defined GRAM_LTYPE_IS_DECLARED
प्रकारनिर्देश संरचना GRAM_LTYPE GRAM_LTYPE;
संरचना GRAM_LTYPE
{
  पूर्णाङ्क first_पङ्क्ति;
  पूर्णाङ्क first_column;
  पूर्णाङ्क last_पङ्क्ति;
  पूर्णाङ्क last_column;
};
# define GRAM_LTYPE_IS_DECLARED 1
# define GRAM_LTYPE_IS_TRIVIAL 1
#endif




पूर्णाङ्क gram_parse (शून्य);

/* "%code provides" blocks.  */

  /* Initialize unquote.  */
  शून्य parser_init (शून्य);
  /* Deallocate storage for unquote.  */
  शून्य parser_free (शून्य);


#endif /* !YY_GRAM_SRC_PARSE_GRAM_H_INCLUDED  */
