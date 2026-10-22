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

#यदि_अपरिभाषितम् YY_GRAM_SRC_PARSE_GRAM_H_INCLUDED
# define YY_GRAM_SRC_PARSE_GRAM_H_INCLUDED
/* Debug traces.  */
#यदि_अपरिभाषितम् GRAM_DEBUG
# यदि defined YYDEBUG
#यदि YYDEBUG
#   define GRAM_DEBUG 1
#  अन्यथा
#   define GRAM_DEBUG 0
#  endif
# अन्यथा /* ! defined YYDEBUG */
#  define GRAM_DEBUG 1
# endif /* ! defined YYDEBUG */
#यदि_अन्तः  /* ! defined GRAM_DEBUG */
#यदि GRAM_DEBUG
बाह्य पूर्णाङ्क gram_debug;
#यदि_अन्तः
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
    वर्ण ध्रुव *chars;
    muscle_विधा विधा;
  } मानम्_type;


/* Token kinds.  */
#यदि_अपरिभाषितम् GRAM_TOKENTYPE
# define GRAM_TOKENTYPE
  गणना gram_tokentype
  {
    GRAM_रिक्तानि = -2,
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
    PERCENT_वामः = 10,             /* "%left"  */
    PERCENT_दक्षिणः = 11,            /* "%right"  */
    PERCENT_NONASSOC = 12,         /* "%nonassoc"  */
    PERCENT_PRECEDENCE = 13,       /* "%precedence"  */
    PERCENT_PREC = 14,             /* "%prec"  */
    PERCENT_DPREC = 15,            /* "%dprec"  */
    PERCENT_MERGE = 16,            /* "%merge"  */
    PERCENT_संहिता = 17,             /* "%code"  */
    PERCENT_DEFAULT_PREC = 18,     /* "%default-prec"  */
    PERCENT_DEFINE = 19,           /* "%define"  */
    PERCENT_ERROR_VERBOSE = 20,    /* "%error-verbose"  */
    PERCENT_EXPECT = 21,           /* "%expect"  */
    PERCENT_EXPECT_RR = 22,        /* "%expect-rr"  */
    PERCENT_FILE_उपसर्गः = 23,      /* "%file-prefix"  */
    PERCENT_FLAG = 24,             /* "%<flag>"  */
    PERCENT_GLR_PARSER = 25,       /* "%glr-parser"  */
    PERCENT_शीर्षकम् = 26,           /* "%header"  */
    PERCENT_INITIAL_ACTION = 27,   /* "%initial-action"  */
    PERCENT_LANGUAGE = 28,         /* "%language"  */
    PERCENT_NAME_उपसर्गः = 29,      /* "%name-prefix"  */
    PERCENT_NO_DEFAULT_PREC = 30,  /* "%no-default-prec"  */
    PERCENT_NO_पङ्क्तयः = 31,         /* "%no-lines"  */
    PERCENT_NONDETERMINISTIC_PARSER = 32, /* "%nondeterministic-parser"  */
    PERCENT_OUTPUT = 33,           /* "%output"  */
    PERCENT_विशुद्धम्_PARSER = 34,      /* "%pure-parser"  */
    PERCENT_REQUIRE = 35,          /* "%require"  */
    PERCENT_SKELETON = 36,         /* "%skeleton"  */
    PERCENT_START = 37,            /* "%start"  */
    PERCENT_TOKEN_सारणी = 38,      /* "%token-table"  */
    PERCENT_VERBOSE = 39,          /* "%verbose"  */
    PERCENT_YACC = 40,             /* "%yacc"  */
    BRACED_संहिता = 41,              /* "{...}"  */
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
    चिह्नकम्_2 = 54,                      /* "<tag>"  */
    चिह्नकम्_ANY = 55,                  /* "<*>"  */
    चिह्नकम्_NONE = 56,                 /* "<>"  */
    INT_LITERAL = 57,              /* "integer literal"  */
    PERCENT_PARAM = 58,            /* "%param"  */
    PERCENT_UNION = 59,            /* "%union"  */
    PERCENT_रिक्तानि = 60             /* "%empty"  */
  };
  प्रकारनिर्देश गणना gram_tokentype gram_token_विधा_t;
#यदि_अन्तः

/* Value type.  */
#यदि ! defined GRAM_STYPE && ! defined GRAM_STYPE_IS_DECLARED
सङ्घ GRAM_STYPE
{
  assoc precedence_declarator;             /* precedence_declarator  */
  वर्ण* STRING;                            /* "string"  */
  वर्ण* TSTRING;                           /* "translatable string"  */
  वर्ण* BRACED_संहिता;                       /* "{...}"  */
  वर्ण* BRACED_PREDICATE;                  /* "%?{...}"  */
  वर्ण* EPILOGUE;                          /* "epilogue"  */
  वर्ण* PROLOGUE;                          /* "%{...%}"  */
  वर्ण* yykind_75;                         /* string.opt  */
  संहिता_props_type संहिता_props_type;         /* code_props_type  */
  पूर्णाङ्क INT_LITERAL;                         /* "integer literal"  */
  पूर्णाङ्क yykind_84;                           /* int.opt  */
  named_ref* yykind_97;                    /* named_ref.opt  */
  param_type PERCENT_PARAM;                /* "%param"  */
  चिह्नम्* token_decl;                      /* token_decl  */
  चिह्नम्* alias;                           /* alias  */
  चिह्नम्* token_decl_for_prec;             /* token_decl_for_prec  */
  चिह्नम्* id;                              /* id  */
  चिह्नम्* id_colon;                        /* id_colon  */
  चिह्नम्* चिह्नम्;                          /*चिह्नम्*/
  चिह्नम्* string_as_id;                    /* string_as_id  */
  चिह्नम्_list* generic_symlist;            /* generic_symlist  */
  चिह्नम्_list* generic_symlist_item;       /* generic_symlist_item  */
  चिह्नम्_list* nterm_decls;                /* nterm_decls  */
  चिह्नम्_list* token_decls;                /* token_decls  */
  चिह्नम्_list* yykind_82;                  /* token_decl.1  */
  चिह्नम्_list* token_decls_for_prec;       /* token_decls_for_prec  */
  चिह्नम्_list* yykind_87;                  /* token_decl_for_prec.1  */
  चिह्नम्_list* चिह्नम्_decls;               /* symbol_decls  */
  चिह्नम्_list* yykind_90;                  /* symbols.1  */
  uniqstr PERCENT_ERROR_VERBOSE;           /* "%error-verbose"  */
  uniqstr PERCENT_FILE_उपसर्गः;             /* "%file-prefix"  */
  uniqstr PERCENT_FLAG;                    /* "%<flag>"  */
  uniqstr PERCENT_NAME_उपसर्गः;             /* "%name-prefix"  */
  uniqstr PERCENT_विशुद्धम्_PARSER;             /* "%pure-parser"  */
  uniqstr BRACKETED_ID;                    /* "[identifier]"  */
  uniqstr ID;                              /* "identifier"  */
  uniqstr ID_COLON;                        /* "identifier:"  */
  uniqstr चिह्नकम्_2;                             /* "<tag>"  */
  uniqstr yykind_76;                       /* tag.opt  */
  uniqstr चिह्नकम्;                             /*चिह्नकम्*/
  uniqstr variable;                        /* variable  */
  अचिह्नित वर्ण CHAR_LITERAL;              /* "character literal"  */
  मानम्_type मानम्;                        /*मानम्*/


};
प्रकारनिर्देश सङ्घ GRAM_STYPE GRAM_STYPE;
# define GRAM_STYPE_IS_TRIVIAL 1
# define GRAM_STYPE_IS_DECLARED 1
#यदि_अन्तः

/* Location type.  */
#यदि ! defined GRAM_LTYPE && ! defined GRAM_LTYPE_IS_DECLARED
प्रकारनिर्देश संरचना GRAM_LTYPE GRAM_LTYPE;
संरचना GRAM_LTYPE
{
  पूर्णाङ्क first_पङ्क्तिः;
  पूर्णाङ्क first_column;
  पूर्णाङ्क चरमः_पङ्क्तिः;
  पूर्णाङ्क चरमः_column;
};
# define GRAM_LTYPE_IS_DECLARED 1
# define GRAM_LTYPE_IS_TRIVIAL 1
#यदि_अन्तः




पूर्णाङ्क gram_parse (शून्य);

/* "%code provides" blocks.  */

  /* Initialize unquote.  */
  शून्य parser_init (शून्य);
  /* Deallocate storage for unquote.  */
  शून्य parser_free (शून्य);


#यदि_अन्तः /* !YY_GRAM_SRC_PARSE_GRAM_H_INCLUDED  */
