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
# എങ്കിൽ defined YYDEBUG
#എങ്കിൽ YYDEBUG
#   define GRAM_DEBUG 1
#  അല്ലെങ്കിൽ
#   define GRAM_DEBUG 0
#  endif
# അല്ലെങ്കിൽ /* ! defined YYDEBUG */
#  define GRAM_DEBUG 1
# endif /* ! defined YYDEBUG */
#endif  /* ! defined GRAM_DEBUG */
#എങ്കിൽ GRAM_DEBUG
ബാഹ്യ പൂർണ്ണസംഖ്യ gram_debug;
#endif
/* "%code requires" blocks.  */

  #അന്തര്ഭാവ "symlist.h"
  #അന്തര്ഭാവ "symtab.h"

  തരനിർവചനം എണ്ണൽ
  {
    param_none   = 0,
    param_lex    = 1 << 0,
    param_parse  = 1 << 1,
    param_both   = param_lex | param_parse
  } param_type;

  #അന്തര്ഭാവ "muscle-tab.h"
  തരനിർവചനം ഘടന
  {
    അക്ഷരം സ്ഥിരം *chars;
    muscle_kind kind;
  } മൂല്യം_type;


/* Token kinds.  */
#ifndef GRAM_TOKENTYPE
# define GRAM_TOKENTYPE
  എണ്ണൽ gram_tokentype
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
    PERCENT_ഇടത് = 10,             /* "%left"  */
    PERCENT_വലത് = 11,            /* "%right"  */
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
    PERCENT_NO_വരികൾ = 31,         /* "%no-lines"  */
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
  തരനിർവചനം എണ്ണൽ gram_tokentype gram_token_kind_t;
#endif

/* Value type.  */
#എങ്കിൽ ! defined GRAM_STYPE && ! defined GRAM_STYPE_IS_DECLARED
സംഘം GRAM_STYPE
{
  assoc precedence_declarator;             /* precedence_declarator  */
  അക്ഷരം* STRING;                            /* "string"  */
  അക്ഷരം* TSTRING;                           /* "translatable string"  */
  അക്ഷരം* BRACED_CODE;                       /* "{...}"  */
  അക്ഷരം* BRACED_PREDICATE;                  /* "%?{...}"  */
  അക്ഷരം* EPILOGUE;                          /* "epilogue"  */
  അക്ഷരം* PROLOGUE;                          /* "%{...%}"  */
  അക്ഷരം* yykind_75;                         /* string.opt  */
  code_props_type code_props_type;         /* code_props_type  */
  പൂർണ്ണസംഖ്യ INT_LITERAL;                         /* "integer literal"  */
  പൂർണ്ണസംഖ്യ yykind_84;                           /* int.opt  */
  named_ref* yykind_97;                    /* named_ref.opt  */
  param_type PERCENT_PARAM;                /* "%param"  */
  ചിഹ്നം* token_decl;                      /* token_decl  */
  ചിഹ്നം* alias;                           /* alias  */
  ചിഹ്നം* token_decl_for_prec;             /* token_decl_for_prec  */
  ചിഹ്നം* id;                              /* id  */
  ചിഹ്നം* id_colon;                        /* id_colon  */
  ചിഹ്നം* ചിഹ്നം;                          /*ചിഹ്നം*/
  ചിഹ്നം* string_as_id;                    /* string_as_id  */
  ചിഹ്നം_list* generic_symlist;            /* generic_symlist  */
  ചിഹ്നം_list* generic_symlist_item;       /* generic_symlist_item  */
  ചിഹ്നം_list* nterm_decls;                /* nterm_decls  */
  ചിഹ്നം_list* token_decls;                /* token_decls  */
  ചിഹ്നം_list* yykind_82;                  /* token_decl.1  */
  ചിഹ്നം_list* token_decls_for_prec;       /* token_decls_for_prec  */
  ചിഹ്നം_list* yykind_87;                  /* token_decl_for_prec.1  */
  ചിഹ്നം_list* ചിഹ്നം_decls;               /* symbol_decls  */
  ചിഹ്നം_list* yykind_90;                  /* symbols.1  */
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
  unsigned അക്ഷരം CHAR_LITERAL;              /* "character literal"  */
  മൂല്യം_type മൂല്യം;                        /*മൂല്യം*/


};
തരനിർവചനം സംഘം GRAM_STYPE GRAM_STYPE;
# define GRAM_STYPE_IS_TRIVIAL 1
# define GRAM_STYPE_IS_DECLARED 1
#endif

/* Location type.  */
#എങ്കിൽ ! defined GRAM_LTYPE && ! defined GRAM_LTYPE_IS_DECLARED
തരനിർവചനം ഘടന GRAM_LTYPE GRAM_LTYPE;
ഘടന GRAM_LTYPE
{
  പൂർണ്ണസംഖ്യ first_വരി;
  പൂർണ്ണസംഖ്യ first_column;
  പൂർണ്ണസംഖ്യ last_വരി;
  പൂർണ്ണസംഖ്യ last_column;
};
# define GRAM_LTYPE_IS_DECLARED 1
# define GRAM_LTYPE_IS_TRIVIAL 1
#endif




പൂർണ്ണസംഖ്യ gram_parse (ശൂന്യം);

/* "%code provides" blocks.  */

  /* Initialize unquote.  */
  ശൂന്യം parser_init (ശൂന്യം);
  /* Deallocate storage for unquote.  */
  ശൂന്യം parser_free (ശൂന്യം);


#endif /* !YY_GRAM_SRC_PARSE_GRAM_H_INCLUDED  */
