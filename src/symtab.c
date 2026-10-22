/* Symbol table manager for Bison.

   Copyright (C) 1984, 1989, 2000-2002, 2004-2015, 2018-2022 Free
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

#अन्तर्भाव <config.h>
#अन्तर्भाव "symtab.h"

#अन्तर्भाव "system.h"

#अन्तर्भाव <assure.h>
#अन्तर्भाव <fstrcmp.h>
#अन्तर्भाव <hash.h>
#अन्तर्भाव <quote.h>

#अन्तर्भाव "complain.h"
#अन्तर्भाव "getargs.h"
#अन्तर्भाव "gram.h"
#अन्तर्भाव "intprops.h"

/** Undefined token code.  */
#define CODE_UNDEFINED (-1)

/* Undefined symbol number.  */
#define संख्या_UNDEFINED (-1)


स्थिर संरचना hash_table *चिन्ह_table = रिक्त;
स्थिर संरचना hash_table *semantic_type_table = रिक्त;

/*----------------------------------------------------------------.
| Symbols sorted by tag.  Allocated by table_sort, after which no |
| more symbols should be created.                                 |
`----------------------------------------------------------------*/

स्थिर चिन्ह **symbols_sorted = रिक्त;
स्थिर semantic_type **semantic_types_sorted = रिक्त;


/*------------------------.
| Distinguished symbols.  |
`------------------------*/

चिन्ह *errtoken = रिक्त;
चिन्ह *undeftoken = रिक्त;
चिन्ह *eoftoken = रिक्त;
चिन्ह *acceptsymbol = रिक्त;

/* Precedence relation graph. */
स्थिर symgraph **prec_nodes;

/* Store which associativity is used.  */
स्थिर bool *used_assoc = रिक्त;

bool tag_seen = false;


/* Whether SYM was defined by the user.  */

स्थिर bool
चिन्ह_is_user_defined (चिन्ह *sym)
{
  स्थिरांक bool eof_is_user_defined
    = !eoftoken->alias || STRNEQ (eoftoken->alias->tag, "$end");
  परत sym->tag[0] != '$'
    && (eof_is_user_defined || (sym != eoftoken && sym->alias != errtoken))
    && sym != errtoken && sym->alias != errtoken
    && sym != undeftoken && sym->alias != undeftoken;
}


/*--------------------------.
| Create a new sym_content. |
`--------------------------*/

स्थिर sym_content *
sym_content_new (चिन्ह *s)
{
  sym_content *res = xmalloc (आकार *res);

  res->चिन्ह = s;

  res->type_name = रिक्त;
  res->type_loc = empty_loc;
  साठी (पूर्णांक i = 0; i < CODE_PROPS_SIZE; ++i)
    code_props_none_init (&res->props[i]);

  res->संख्या = संख्या_UNDEFINED;
  res->prec_loc = empty_loc;
  res->prec = 0;
  res->assoc = undef_assoc;
  res->code = CODE_UNDEFINED;

  res->class = unknown_sym;
  res->status = undeclared;

  परत res;
}

/*---------------------------------.
| Create a new symbol, named TAG.  |
`---------------------------------*/

स्थिर चिन्ह *
चिन्ह_new (uniqstr tag, location loc)
{
  चिन्ह *res = xmalloc (आकार *res);
  uniqstr_assert (tag);

  /* If the tag is not a string (starts with a double quote), check
     that it is valid for Yacc. */
  जर (tag[0] != '\"' && tag[0] != '\'' && म_अक्षर (tag, '-'))
    complain (&loc, Wyacc,
              _("POSIX Yacc forbids dashes in symbol names: %s"), tag);

  res->tag = tag;
  res->location = loc;
  res->translatable = false;
  res->location_of_lhs = false;
  res->alias = रिक्त;
  res->content = sym_content_new (res);
  res->is_alias = false;
  परत res;
}

/*--------------------.
| Free a sym_content. |
`--------------------*/

स्थिर शून्य
sym_content_free (sym_content *sym)
{
  मुक्त (sym);
}


/*---------------------------------------------------------.
| Free a symbol and its associated content if appropriate. |
`---------------------------------------------------------*/

स्थिर शून्य
चिन्ह_free (शून्य *ptr)
{
  चिन्ह *sym = (चिन्ह *)ptr;
  जर (!sym->is_alias)
    sym_content_free (sym->content);
  मुक्त (sym);

}

/* If needed, swap first and second so that first has the earliest
   location (according to location_cmp).

   Many symbol features (e.g., token codes) are not assigned during
   parsing, but in a second step, via a traversal of the symbol table
   sorted on tag.

   However, error messages make more sense if we keep the first
   declaration first.
*/

स्थिर शून्य
symbols_sort (स्थिरांक चिन्ह **first, स्थिरांक चिन्ह **second)
{
  जर (0 < location_cmp ((*first)->location, (*second)->location))
    {
      स्थिरांक चिन्ह* tmp = *first;
      *first = *second;
      *second = tmp;
    }
}

/* Likewise, for locations.  */

स्थिर शून्य
locations_sort (location *first, location *second)
{
  जर (0 < location_cmp (*first, *second))
    {
      location tmp = *first;
      *first = *second;
      *second = tmp;
    }
}

अक्षर स्थिरांक *
code_props_type_string (code_props_type kind)
{
  निवड (kind)
    {
    स्थिती destructor:
      परत "%destructor";
    स्थिती printer:
      परत "%printer";
    }
  पात ();
}


/*----------------------------------------.
| Create a new semantic type, named TAG.  |
`----------------------------------------*/

स्थिर semantic_type *
semantic_type_new (uniqstr tag, स्थिरांक location *loc)
{
  semantic_type *res = xmalloc (आकार *res);

  uniqstr_assert (tag);
  res->tag = tag;
  res->location = loc ? *loc : empty_loc;
  res->status = undeclared;
  साठी (पूर्णांक i = 0; i < CODE_PROPS_SIZE; ++i)
    code_props_none_init (&res->props[i]);

  परत res;
}


/*-----------------.
| Print a symbol.  |
`-----------------*/

#define चिन्ह_INT_ATTR_PRINT(Attr)                     \
  जर (s->content)                                       \
    ख_लिखो (f, " %s = %d", #Attr, s->content->Attr)

#define चिन्ह_STR_ATTR_PRINT(Attr)                     \
  जर (s->content && s->content->Attr)                   \
    ख_लिखो (f, " %s { %s }", #Attr, s->content->Attr)

#define चिन्ह_CODE_PRINT(Attr)                                         \
  जर (s->content && s->content->props[Attr].code)                       \
    ख_लिखो (f, " %s { %s }", #Attr, s->content->props[Attr].code)

शून्य
चिन्ह_print (चिन्ह स्थिरांक *s, खाता *f)
{
  जर (s)
    {
      चिन्ह_class c = s->content->class;
      ख_लिखो (f, "%s: %s",
               c == unknown_sym    ? "unknown"
               : c == pct_type_sym ? "%type"
               : c == token_sym    ? "token"
               : c == nterm_sym    ? "nterm"
               : रिक्त, /* abort.  */
               s->tag);
      अ_दो (' ', f);
      location_print (s->location, f);
      चिन्ह_INT_ATTR_PRINT (code);
      चिन्ह_INT_ATTR_PRINT (संख्या);
      चिन्ह_STR_ATTR_PRINT (type_name);
      चिन्ह_CODE_PRINT (destructor);
      चिन्ह_CODE_PRINT (printer);
    }
  अन्यथा
    ख_माला_दो ("<NULL>", f);
}

#undef चिन्ह_ATTR_PRINT
#undef चिन्ह_CODE_PRINT


/*----------------------------------.
| Whether S is a valid identifier.  |
`----------------------------------*/

स्थिर bool
is_identifier (uniqstr s)
{
  स्थिर अक्षर स्थिरांक alphanum[26 + 26 + 1 + 10] =
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "_"
    "0123456789";
  जर (!s || ! स_प्रथम (alphanum, *s, आकार alphanum - 10))
    परत false;
  साठी (++s; *s; ++s)
    जर (! स_प्रथम (alphanum, *s, आकार alphanum))
      परत false;
  परत true;
}


/*-----------------------------------------------.
| Get the identifier associated to this symbol.  |
`-----------------------------------------------*/

uniqstr
चिन्ह_id_get (चिन्ह स्थिरांक *sym)
{
  // There's one weird case: YYerror is the alias, and error is the
  // base symbol.  Return YYerror in that case.
  जर (sym->alias && is_identifier (sym->alias->tag))
    परत sym->alias->tag;
  अन्यथा जर (is_identifier (sym->tag))
    परत sym->tag;
  अन्यथा
    परत रिक्त;
}


/*------------------------------------------------------------------.
| Complain that S's WHAT is redeclared at SECOND, and was first set |
| at FIRST.                                                         |
`------------------------------------------------------------------*/

स्थिर शून्य
complain_चिन्ह_redeclared (चिन्ह *s, स्थिरांक अक्षर *what, location first,
                            location second)
{
  locations_sort (&first, &second);
  complain (&second, complaint, _("%s redeclaration for %s"), what, s->tag);
  subcomplain (&first, complaint, _("previous declaration"));
}

स्थिर शून्य
complain_semantic_type_redeclared (semantic_type *s, स्थिरांक अक्षर *what, location first,
                                   location second)
{
  locations_sort (&first, &second);
  complain (&second, complaint, _("%s redeclaration for <%s>"), what, s->tag);
  subcomplain (&first, complaint, _("previous declaration"));
}

स्थिर शून्य
complain_class_redeclared (चिन्ह *sym, चिन्ह_class class, location second)
{
  complain (&second, complaint,
            class == token_sym
            ? _("symbol %s redeclared as a token")
            : _("symbol %s redeclared as a nonterminal"), sym->tag);
  जर (!location_empty (sym->location))
    subcomplain (&sym->location, complaint, _("previous definition"));
}

स्थिर स्थिरांक चिन्ह *
चिन्ह_from_uniqstr_fuzzy (स्थिरांक uniqstr किल्ली)
{
  aver (symbols_sorted);
#define FSTRCMP_THRESHOLD 0.6
  द्विगुण best_similarity = FSTRCMP_THRESHOLD;
  स्थिरांक चिन्ह *res = रिक्त;
  माप_प्रकार मोजणी = hash_get_n_entries (चिन्ह_table);
  साठी (पूर्णांक i = 0; i < मोजणी; ++i)
    {
      चिन्ह *sym = symbols_sorted[i];
      जर (STRNEQ (किल्ली, sym->tag)
          && (sym->content->status == declared
              || sym->content->status == undeclared))
        {
          द्विगुण similarity = fstrcmp_bounded (किल्ली, sym->tag, best_similarity);
          जर (best_similarity < similarity)
            {
              res = sym;
              best_similarity = similarity;
            }
        }
    }
  परत res;
}

स्थिर शून्य
complain_चिन्ह_undeclared (स्थिरांक चिन्ह *sym)
{
  प्रतिज्ञा (sym->content->status != declared);
  स्थिरांक चिन्ह *best = चिन्ह_from_uniqstr_fuzzy (sym->tag);
  जर (best)
    {
      complain (&sym->location,
                sym->content->status == needed ? complaint : Wother,
                _("symbol %s is used, but is not defined as a token"
                  " and has no rules; did you mean %s?"),
                quote_n (0, sym->tag),
                quote_n (1, best->tag));
      जर (feature_flag & feature_caret)
        location_caret_suggestion (sym->location, best->tag, मानक_त्रुटि);
    }
  अन्यथा
    complain (&sym->location,
              sym->content->status == needed ? complaint : Wother,
              _("symbol %s is used, but is not defined as a token"
                " and has no rules"),
              quote (sym->tag));
}

शून्य
चिन्ह_location_as_lhs_set (चिन्ह *sym, location loc)
{
  जर (!sym->location_of_lhs)
    {
      sym->location = loc;
      sym->location_of_lhs = true;
    }
}


/*-----------------------------------------------------------------.
| Set the TYPE_NAME associated with SYM.  Does nothing if passed 0 |
| as TYPE_NAME.                                                    |
`-----------------------------------------------------------------*/

शून्य
चिन्ह_type_set (चिन्ह *sym, uniqstr type_name, location loc)
{
  जर (type_name)
    {
      tag_seen = true;
      जर (sym->content->type_name)
        complain_चिन्ह_redeclared (sym, "%type",
                                    sym->content->type_loc, loc);
      अन्यथा
        {
          uniqstr_assert (type_name);
          sym->content->type_name = type_name;
          sym->content->type_loc = loc;
        }
    }
}

/*--------------------------------------------------------.
| Set the DESTRUCTOR or PRINTER associated with the SYM.  |
`--------------------------------------------------------*/

शून्य
चिन्ह_code_props_set (चिन्ह *sym, code_props_type kind,
                       code_props स्थिरांक *code)
{
  जर (sym->content->props[kind].code)
    complain_चिन्ह_redeclared (sym, code_props_type_string (kind),
                                sym->content->props[kind].location,
                                code->location);
  अन्यथा
    sym->content->props[kind] = *code;
}


/*-----------------------------------------------------.
| Set the DESTRUCTOR or PRINTER associated with TYPE.  |
`-----------------------------------------------------*/

शून्य
semantic_type_code_props_set (semantic_type *type,
                              code_props_type kind,
                              code_props स्थिरांक *code)
{
  जर (type->props[kind].code)
    complain_semantic_type_redeclared (type, code_props_type_string (kind),
                                       type->props[kind].location,
                                       code->location);
  अन्यथा
    type->props[kind] = *code;
}

/*---------------------------------------------------.
| Get the computed %destructor or %printer for SYM.  |
`---------------------------------------------------*/

code_props *
चिन्ह_code_props_get (चिन्ह *sym, code_props_type kind)
{
  /* Per-symbol code props.  */
  जर (sym->content->props[kind].code)
    परत &sym->content->props[kind];

  /* Per-type code props.  */
  जर (sym->content->type_name)
    {
      code_props *code =
        &semantic_type_get (sym->content->type_name, रिक्त)->props[kind];
      जर (code->code)
        परत code;
    }

  /* Apply default code props's only to user-defined symbols.  */
  जर (चिन्ह_is_user_defined (sym))
    {
      code_props *code = &semantic_type_get (sym->content->type_name ? "*" : "",
                                             रिक्त)->props[kind];
      जर (code->code)
        परत code;
    }
  परत &code_props_none;
}

/*-----------------------------------------------------------------.
| Set the PRECEDENCE associated with SYM.  Does nothing if invoked |
| with UNDEF_ASSOC as ASSOC.                                       |
`-----------------------------------------------------------------*/

शून्य
चिन्ह_precedence_set (चिन्ह *sym, पूर्णांक prec, assoc a, location loc)
{
  जर (a != undef_assoc)
    {
      sym_content *s = sym->content;
      जर (s->prec)
        complain_चिन्ह_redeclared (sym, assoc_to_string (a),
                                    s->prec_loc, loc);
      अन्यथा
        {
          s->prec = prec;
          s->assoc = a;
          s->prec_loc = loc;
        }
    }

  /* Only terminals have a precedence. */
  चिन्ह_class_set (sym, token_sym, loc, false);
}


/*------------------------------------.
| Set the CLASS associated with SYM.  |
`------------------------------------*/

स्थिर शून्य
complain_pct_type_on_token (location *loc)
{
  complain (loc, Wyacc,
            _("POSIX yacc reserves %%type to nonterminals"));
}

शून्य
चिन्ह_class_set (चिन्ह *sym, चिन्ह_class class, location loc, bool declaring)
{
  aver (class != unknown_sym);
  sym_content *s = sym->content;
  जर (class == pct_type_sym)
    {
      जर (s->class == token_sym)
        complain_pct_type_on_token (&loc);
      अन्यथा जर (s->class == unknown_sym)
        s->class = class;
    }
  अन्यथा जर (s->class != unknown_sym && s->class != pct_type_sym
           && s->class != class)
    complain_class_redeclared (sym, class, loc);
  अन्यथा
    {
      जर (class == token_sym && s->class == pct_type_sym)
        complain_pct_type_on_token (&sym->location);

      s->class = class;

      जर (declaring)
        {
          जर (s->status == declared)
            {
              complain (&loc, Wother,
                        _("symbol %s redeclared"), sym->tag);
              subcomplain (&sym->location, Wother,
                           _("previous declaration"));
            }
          अन्यथा
            {
              sym->location = loc;
              s->status = declared;
            }
        }
    }
}


/*----------------------------.
| Set the token code of SYM.  |
`----------------------------*/

शून्य
चिन्ह_code_set (चिन्ह *sym, पूर्णांक code, location loc)
{
  पूर्णांक *codep = &sym->content->code;
  जर (sym->content->class != token_sym)
    complain (&loc, complaint,
              _("nonterminals cannot be given a token code"));
  अन्यथा जर (*codep != CODE_UNDEFINED
           && *codep != code)
    complain (&loc, complaint, _("redefining code of token %s"),
              sym->tag);
  अन्यथा जर (code == पूर्णांक_उच्च)
    complain (&loc, complaint, _("code of token %s too large"),
              sym->tag);
  अन्यथा
    {
      *codep = code;
      /* User defined $end token? */
      जर (code == 0 && !eoftoken)
        {
          eoftoken = sym->content->चिन्ह;
          eoftoken->content->संख्या = 0;
        }
    }
}


/*----------------------------------------------------------.
| If SYM is not defined, report an error, and consider it a |
| nonterminal.                                              |
`----------------------------------------------------------*/

स्थिर शून्य
चिन्ह_check_defined (चिन्ह *sym)
{
  sym_content *s = sym->content;
  जर (s->class == unknown_sym || s->class == pct_type_sym)
    {
      complain_चिन्ह_undeclared (sym);
      s->class = nterm_sym;
    }

  जर (s->संख्या == संख्या_UNDEFINED)
    s->संख्या = s->class == token_sym ? ntokens++ : nnterms++;

  जर (s->class == token_sym
      && sym->tag[0] == '"'
      && !sym->is_alias)
    complain (&sym->location, Wdangling_alias,
              _("string literal %s not attached to a symbol"),
              sym->tag);

  साठी (पूर्णांक i = 0; i < 2; ++i)
    चिन्ह_code_props_get (sym, i)->is_used = true;

  /* Set the semantic type status associated to the current symbol to
     'declared' so that we could check semantic types unnecessary uses. */
  जर (s->type_name)
    {
      semantic_type *sem_type = semantic_type_get (s->type_name, रिक्त);
      जर (sem_type)
        sem_type->status = declared;
    }
}

स्थिर शून्य
semantic_type_check_defined (semantic_type *sem_type)
{
  /* <*> and <> do not have to be "declared".  */
  जर (sem_type->status == declared
      || !*sem_type->tag
      || STREQ (sem_type->tag, "*"))
    {
      साठी (पूर्णांक i = 0; i < 2; ++i)
        जर (sem_type->props[i].kind != CODE_PROPS_NONE
            && ! sem_type->props[i].is_used)
          complain (&sem_type->location, Wother,
                    _("useless %s for type <%s>"),
                    code_props_type_string (i), sem_type->tag);
    }
  अन्यथा
    complain (&sem_type->location, Wother,
              _("type <%s> is used, but is not associated to any symbol"),
              sem_type->tag);
}

/*-------------------------------------------------------------------.
| Merge the properties (precedence, associativity, etc.) of SYM, and |
| its string-named alias STR; check consistency.                     |
`-------------------------------------------------------------------*/

स्थिर शून्य
चिन्ह_merge_properties (चिन्ह *sym, चिन्ह *str)
{
  जर (str->content->type_name != sym->content->type_name)
    {
      जर (str->content->type_name)
        चिन्ह_type_set (sym,
                         str->content->type_name, str->content->type_loc);
      अन्यथा
        चिन्ह_type_set (str,
                         sym->content->type_name, sym->content->type_loc);
    }


  साठी (पूर्णांक i = 0; i < CODE_PROPS_SIZE; ++i)
    जर (str->content->props[i].code)
      चिन्ह_code_props_set (sym, i, &str->content->props[i]);
    अन्यथा जर (sym->content->props[i].code)
      चिन्ह_code_props_set (str, i, &sym->content->props[i]);

  जर (sym->content->prec || str->content->prec)
    {
      जर (str->content->prec)
        चिन्ह_precedence_set (sym, str->content->prec, str->content->assoc,
                               str->content->prec_loc);
      अन्यथा
        चिन्ह_precedence_set (str, sym->content->prec, sym->content->assoc,
                               sym->content->prec_loc);
    }
}

शून्य
चिन्ह_make_alias (चिन्ह *sym, चिन्ह *str, location loc)
{
  जर (sym->content->class != token_sym)
    complain (&loc, complaint,
              _("nonterminals cannot be given a string alias"));
  अन्यथा जर (str->alias)
    complain (&loc, Wother,
              _("symbol %s used more than once as a literal string"), str->tag);
  अन्यथा जर (sym->alias)
    complain (&loc, Wother,
              _("symbol %s given more than one literal string"), sym->tag);
  अन्यथा
    {
      चिन्ह_merge_properties (sym, str);
      sym_content_free (str->content);
      str->content = sym->content;
      str->content->चिन्ह = str;
      str->is_alias = true;
      str->alias = sym;
      sym->alias = str;
    }
}


/*-------------------------------------------------------------------.
| Assign a symbol number, and write the definition of the token name |
| into FDEFINES.  Put in SYMBOLS.                                    |
`-------------------------------------------------------------------*/

स्थिर शून्य
चिन्ह_pack (चिन्ह *sym)
{
  aver (sym->content->संख्या != संख्या_UNDEFINED);
  जर (sym->content->class == nterm_sym)
    sym->content->संख्या += ntokens;

  symbols[sym->content->संख्या] = sym->content->चिन्ह;
}

स्थिर शून्य
complain_code_redeclared (पूर्णांक अंक, स्थिरांक चिन्ह *first, स्थिरांक चिन्ह *second)
{
  symbols_sort (&first, &second);
  complain (&second->location, complaint,
            _("code %d reassigned to token %s"),
            अंक, second->tag);
  subcomplain (&first->location, complaint,
               _("previous declaration for %s"),
               first->tag);
}

/*-------------------------------------------------.
| Put SYM in TOKEN_TRANSLATIONS if it is a token.  |
`-------------------------------------------------*/

स्थिर शून्य
चिन्ह_translation (स्थिरांक चिन्ह *sym)
{
  जर (sym->content->class == token_sym && !sym->is_alias)
    {
      /* A token whose translation has already been set? */
      जर (token_translations[sym->content->code]
          != undeftoken->content->संख्या)
        complain_code_redeclared
          (sym->content->code,
           symbols[token_translations[sym->content->code]], sym);
      अन्यथा
        token_translations[sym->content->code]
          = sym->content->संख्या;
    }
}


/*---------------------------------------.
| Symbol and semantic type hash tables.  |
`---------------------------------------*/

/* Initial capacity of symbol and semantic type hash table.  */
#define HT_INITIAL_CAPACITY 257

स्थिर ओळीत bool
hash_compare_चिन्ह (स्थिरांक चिन्ह *m1, स्थिरांक चिन्ह *m2)
{
  /* Since tags are unique, we can compare the pointers themselves.  */
  परत UNIQSTR_EQ (m1->tag, m2->tag);
}

स्थिर ओळीत bool
hash_compare_semantic_type (स्थिरांक semantic_type *m1, स्थिरांक semantic_type *m2)
{
  /* Since names are unique, we can compare the pointers themselves.  */
  परत UNIQSTR_EQ (m1->tag, m2->tag);
}

स्थिर bool
hash_चिन्ह_comparator (शून्य स्थिरांक *m1, शून्य स्थिरांक *m2)
{
  परत hash_compare_चिन्ह (m1, m2);
}

स्थिर bool
hash_semantic_type_comparator (शून्य स्थिरांक *m1, शून्य स्थिरांक *m2)
{
  परत hash_compare_semantic_type (m1, m2);
}

स्थिर ओळीत माप_प्रकार
hash_चिन्ह (स्थिरांक चिन्ह *m, माप_प्रकार tablesize)
{
  /* Since tags are unique, we can hash the pointer itself.  */
  परत ((uintptr_t) m->tag) % tablesize;
}

स्थिर ओळीत माप_प्रकार
hash_semantic_type (स्थिरांक semantic_type *m, माप_प्रकार tablesize)
{
  /* Since names are unique, we can hash the pointer itself.  */
  परत ((uintptr_t) m->tag) % tablesize;
}

स्थिर माप_प्रकार
hash_चिन्ह_hasher (शून्य स्थिरांक *m, माप_प्रकार tablesize)
{
  परत hash_चिन्ह (m, tablesize);
}

स्थिर माप_प्रकार
hash_semantic_type_hasher (शून्य स्थिरांक *m, माप_प्रकार tablesize)
{
  परत hash_semantic_type (m, tablesize);
}

/*-------------------------------.
| Create the symbol hash table.  |
`-------------------------------*/

शून्य
symbols_new (शून्य)
{
  चिन्ह_table = hash_xinitialize (HT_INITIAL_CAPACITY,
                                   रिक्त,
                                   hash_चिन्ह_hasher,
                                   hash_चिन्ह_comparator,
                                   चिन्ह_free);

  /* Construct the acceptsymbol symbol. */
  acceptsymbol = चिन्ह_get ("$accept", empty_loc);
  acceptsymbol->content->class = nterm_sym;
  acceptsymbol->content->संख्या = nnterms++;

  /* Construct the YYerror/"error" token */
  errtoken = चिन्ह_get ("YYerror", empty_loc);
  errtoken->content->class = token_sym;
  errtoken->content->संख्या = ntokens++;
  {
    चिन्ह *alias = चिन्ह_get ("error", empty_loc);
    चिन्ह_class_set (alias, token_sym, empty_loc, false);
    चिन्ह_make_alias (errtoken, alias, empty_loc);
  }

  /* Construct the YYUNDEF/"$undefined" token that represents all
     undefined literal tokens.  It is always symbol number 2.  */
  undeftoken = चिन्ह_get ("YYUNDEF", empty_loc);
  undeftoken->content->class = token_sym;
  undeftoken->content->संख्या = ntokens++;
  {
    चिन्ह *alias = चिन्ह_get ("$undefined", empty_loc);
    चिन्ह_class_set (alias, token_sym, empty_loc, false);
    चिन्ह_make_alias (undeftoken, alias, empty_loc);
  }

  semantic_type_table = hash_xinitialize (HT_INITIAL_CAPACITY,
                                          रिक्त,
                                          hash_semantic_type_hasher,
                                          hash_semantic_type_comparator,
                                          मुक्त);
}


/*----------------------------------------------------------------.
| Find the symbol named KEY, and return it.  If it does not exist |
| yet, create it.                                                 |
`----------------------------------------------------------------*/

चिन्ह *
चिन्ह_from_uniqstr (स्थिरांक uniqstr किल्ली, location loc)
{
  चिन्ह probe;

  probe.tag = किल्ली;
  चिन्ह *res = hash_lookup (चिन्ह_table, &probe);

  जर (!res)
    {
      /* First insertion in the hash. */
      aver (!symbols_sorted);
      res = चिन्ह_new (किल्ली, loc);
      hash_xinsert (चिन्ह_table, res);
    }
  परत res;
}


/*-----------------------------------------------------------------------.
| Find the semantic type named KEY, and return it.  If it does not exist |
| yet, create it.                                                        |
`-----------------------------------------------------------------------*/

semantic_type *
semantic_type_from_uniqstr (स्थिरांक uniqstr किल्ली, स्थिरांक location *loc)
{
  semantic_type probe;

  probe.tag = किल्ली;
  semantic_type *res = hash_lookup (semantic_type_table, &probe);

  जर (!res)
    {
      /* First insertion in the hash. */
      res = semantic_type_new (किल्ली, loc);
      hash_xinsert (semantic_type_table, res);
    }
  परत res;
}


/*----------------------------------------------------------------.
| Find the symbol named KEY, and return it.  If it does not exist |
| yet, create it.                                                 |
`----------------------------------------------------------------*/

चिन्ह *
चिन्ह_get (स्थिरांक अक्षर *किल्ली, location loc)
{
  परत चिन्ह_from_uniqstr (uniqstr_new (किल्ली), loc);
}


/*-----------------------------------------------------------------------.
| Find the semantic type named KEY, and return it.  If it does not exist |
| yet, create it.                                                        |
`-----------------------------------------------------------------------*/

semantic_type *
semantic_type_get (स्थिरांक अक्षर *किल्ली, स्थिरांक location *loc)
{
  परत semantic_type_from_uniqstr (uniqstr_new (किल्ली), loc);
}


/*------------------------------------------------------------------.
| Generate a dummy nonterminal, whose name cannot conflict with the |
| user's names.                                                     |
`------------------------------------------------------------------*/

चिन्ह *
dummy_चिन्ह_get (location loc)
{
  /* Incremented for each generated symbol.  */
  स्थिर पूर्णांक dummy_मोजणी = 0;
  अक्षर buf[32];
  पूर्णांक len = snprintf (buf, आकार buf, "$@%d", ++dummy_मोजणी);
  assure (len < आकार buf);
  चिन्ह *sym = चिन्ह_get (buf, loc);
  sym->content->class = nterm_sym;
  परत sym;
}

bool
चिन्ह_is_dummy (चिन्ह स्थिरांक *sym)
{
  परत sym->tag[0] == '@' || (sym->tag[0] == '$' && sym->tag[1] == '@');
}

/*-------------------.
| Free the symbols.  |
`-------------------*/

शून्य
symbols_free (शून्य)
{
  hash_free (चिन्ह_table);
  hash_free (semantic_type_table);
  मुक्त (symbols);
  मुक्त (symbols_sorted);
  मुक्त (semantic_types_sorted);
}


स्थिर पूर्णांक
चिन्ह_cmp (शून्य स्थिरांक *a, शून्य स्थिरांक *b)
{
  परत location_cmp ((*(चिन्ह * स्थिरांक *)a)->location,
                       (*(चिन्ह * स्थिरांक *)b)->location);
}

/* Store in *SORTED an array of pointers to the symbols contained in
   TABLE, sorted by order of appearance (i.e., by location). */

स्थिर शून्य
table_sort (संरचना hash_table *table, चिन्ह ***sorted)
{
  aver (!*sorted);
  माप_प्रकार मोजणी = hash_get_n_entries (table);
  *sorted = xnmalloc (मोजणी + 1, आकार **sorted);
  hash_get_entries (table, (शून्य**)*sorted, मोजणी);
  क्विक (*sorted, मोजणी, आकार **sorted, चिन्ह_cmp);
  (*sorted)[मोजणी] = रिक्त;
}

/*--------------------------------------------------------------.
| Check that all the symbols are defined.  Report any undefined |
| symbols and consider them nonterminals.                       |
`--------------------------------------------------------------*/

शून्य
symbols_check_defined (शून्य)
{
  table_sort (चिन्ह_table, &symbols_sorted);
  /* semantic_type, like symbol, starts with a 'tag' field and then a
     'location' field.  And here we only deal with arrays/hashes of
     pointers, sizeof is not an issue.

     So instead of implementing table_sort (and symbol_cmp) once for
     each type, let's lie a bit to the typing system, and treat
     'semantic_type' as if it were 'symbol'. */
  table_sort (semantic_type_table, (चिन्ह ***) &semantic_types_sorted);

  साठी (पूर्णांक i = 0; symbols_sorted[i]; ++i)
    चिन्ह_check_defined (symbols_sorted[i]);
  साठी (पूर्णांक i = 0; semantic_types_sorted[i]; ++i)
    semantic_type_check_defined (semantic_types_sorted[i]);
}

/*------------------------------------------------------------------.
| Set TOKEN_TRANSLATIONS.  Check that no two symbols share the same |
| number.                                                           |
`------------------------------------------------------------------*/

स्थिर शून्य
symbols_token_translations_init (शून्य)
{
  bool code_256_available_p = true;

  /* Find the highest token code, and whether 256, the POSIX preferred
     token code for the error token, is used.  */
  max_code = 0;
  साठी (पूर्णांक i = 0; i < ntokens; ++i)
    {
      sym_content *sym = symbols[i]->content;
      जर (sym->code != CODE_UNDEFINED)
        {
          जर (sym->code > max_code)
            max_code = sym->code;
          जर (sym->code == 256)
            code_256_available_p = false;
        }
    }

  /* If 256 is not used, assign it to error, to follow POSIX.  */
  जर (code_256_available_p
      && errtoken->content->code == CODE_UNDEFINED)
    errtoken->content->code = 256;

  /* Set the missing codes. */
  जर (max_code < 256)
    max_code = 256;

  साठी (पूर्णांक i = 0; i < ntokens; ++i)
    {
      sym_content *sym = symbols[i]->content;
      जर (sym->code == CODE_UNDEFINED)
        {
          IGNORE_TYPE_LIMITS_BEGIN
          जर (INT_ADD_WRAPV (max_code, 1, &max_code))
            complain (रिक्त, fatal, _("token number too large"));
          IGNORE_TYPE_LIMITS_END
          sym->code = max_code;
        }
      जर (sym->code > max_code)
        max_code = sym->code;
    }

  token_translations = xnmalloc (max_code + 1,
                                 आकार *token_translations);

  /* Initialize all entries for literal tokens to the internal token
     number for $undefined, which represents all invalid inputs.  */
  साठी (पूर्णांक i = 0; i < max_code + 1; ++i)
    token_translations[i] = undeftoken->content->संख्या;
  साठी (पूर्णांक i = 0; symbols_sorted[i]; ++i)
    चिन्ह_translation (symbols_sorted[i]);
}


/* Whether some symbol requires internationalization.  */
स्थिर bool
has_translations (शून्य)
{
  साठी (स्थिरांक शून्य *entry = hash_get_first (चिन्ह_table);
       entry;
       entry = hash_get_next (चिन्ह_table, entry))
    {
      स्थिरांक चिन्ह *sym = (स्थिरांक चिन्ह *) entry;
      जर (sym->translatable)
        परत true;
    }
  परत false;
}

/*----------------------------------------------------------------.
| Assign symbol numbers, and write definition of token names into |
| FDEFINES.  Set up vectors SYMBOL_TABLE, TAGS of symbols.        |
`----------------------------------------------------------------*/

शून्य
symbols_pack (शून्य)
{
  symbols = xcalloc (nsyms, आकार *symbols);
  साठी (पूर्णांक i = 0; symbols_sorted[i]; ++i)
    चिन्ह_pack (symbols_sorted[i]);

  /* Aliases leave empty slots in symbols, so remove them.  */
  {
    पूर्णांक nsyms_old = nsyms;
    साठी (पूर्णांक writei = 0, readi = 0; readi < nsyms_old; readi += 1)
      {
        जर (symbols[readi] == रिक्त)
          {
            nsyms -= 1;
            ntokens -= 1;
          }
        अन्यथा
          {
            symbols[writei] = symbols[readi];
            symbols[writei]->content->संख्या = writei;
            writei += 1;
          }
      }
  }
  symbols = xnrealloc (symbols, nsyms, आकार *symbols);

  symbols_token_translations_init ();

  // If some user tokens are internationalized, the internal ones
  // should be too.
  जर (has_translations ())
    {
      स्थिरांक bool eof_is_user_defined
        = !eoftoken->alias || STRNEQ (eoftoken->alias->tag, "$end");
      जर (!eof_is_user_defined)
        eoftoken->alias->translatable = true;
      undeftoken->alias->translatable = true;
      errtoken->alias->translatable = true;
    }
}

/*---------------------------------.
| Initialize relation graph nodes. |
`---------------------------------*/

स्थिर शून्य
init_prec_nodes (शून्य)
{
  prec_nodes = xcalloc (nsyms, आकार *prec_nodes);
  साठी (पूर्णांक i = 0; i < nsyms; ++i)
    {
      prec_nodes[i] = xmalloc (आकार *prec_nodes[i]);
      symgraph *s = prec_nodes[i];
      s->id = i;
      s->succ = 0;
      s->pred = 0;
    }
}

/*----------------.
| Create a link.  |
`----------------*/

स्थिर symgraphlink *
symgraphlink_new (graphid id, symgraphlink *next)
{
  symgraphlink *res = xmalloc (आकार *res);
  res->id = id;
  res->next = next;
  परत res;
}


/*------------------------------------------------------------------.
| Register the second symbol of the precedence relation, and return |
| whether this relation is new.  Use only in register_precedence.   |
`------------------------------------------------------------------*/

स्थिर bool
register_precedence_second_चिन्ह (symgraphlink **first, graphid sym)
{
  जर (!*first || sym < (*first)->id)
    *first = symgraphlink_new (sym, *first);
  अन्यथा
    {
      symgraphlink *slist = *first;

      पर्यंत (slist->next && slist->next->id <= sym)
        slist = slist->next;

      जर (slist->id == sym)
        /* Relation already present. */
        परत false;

      slist->next = symgraphlink_new (sym, slist->next);
    }
  परत true;
}

/*------------------------------------------------------------------.
| Register a new relation between symbols as used. The first symbol |
| has a greater precedence than the second one.                     |
`------------------------------------------------------------------*/

शून्य
register_precedence (graphid first, graphid snd)
{
  जर (!prec_nodes)
    init_prec_nodes ();
  register_precedence_second_चिन्ह (&(prec_nodes[first]->succ), snd);
  register_precedence_second_चिन्ह (&(prec_nodes[snd]->pred), first);
}


/*---------------------------------------.
| Deep clear a linked / adjacency list). |
`---------------------------------------*/

स्थिर शून्य
linkedlist_free (symgraphlink *गाठ)
{
  जर (गाठ)
    {
      पर्यंत (गाठ->next)
        {
          symgraphlink *tmp = गाठ->next;
          मुक्त (गाठ);
          गाठ = tmp;
        }
      मुक्त (गाठ);
    }
}

/*----------------------------------------------.
| Clear and destroy association tracking table. |
`----------------------------------------------*/

स्थिर शून्य
assoc_free (शून्य)
{
  साठी (पूर्णांक i = 0; i < nsyms; ++i)
    {
      linkedlist_free (prec_nodes[i]->pred);
      linkedlist_free (prec_nodes[i]->succ);
      मुक्त (prec_nodes[i]);
    }
  मुक्त (prec_nodes);
}

/*---------------------------------------.
| Initialize association tracking table. |
`---------------------------------------*/

स्थिर शून्य
init_assoc (शून्य)
{
  used_assoc = xcalloc (nsyms, आकार *used_assoc);
  साठी (graphid i = 0; i < nsyms; ++i)
    used_assoc[i] = false;
}

/*------------------------------------------------------------------.
| Test if the associativity for the symbols is defined and useless. |
`------------------------------------------------------------------*/

स्थिर ओळीत bool
is_assoc_useless (चिन्ह *s)
{
  परत s
      && s->content->assoc != undef_assoc
      && s->content->assoc != precedence_assoc
      && !used_assoc[s->content->संख्या];
}

/*-------------------------------.
| Register a used associativity. |
`-------------------------------*/

शून्य
register_assoc (graphid i, graphid j)
{
  जर (!used_assoc)
    init_assoc ();
  used_assoc[i] = true;
  used_assoc[j] = true;
}

/*--------------------------------------------------.
| Print a warning for unused precedence relations.  |
`--------------------------------------------------*/

शून्य
print_precedence_warnings (शून्य)
{
  जर (!prec_nodes)
    init_prec_nodes ();
  जर (!used_assoc)
    init_assoc ();
  साठी (पूर्णांक i = 0; i < nsyms; ++i)
    {
      चिन्ह *s = symbols[i];
      जर (s
          && s->content->prec != 0
          && !prec_nodes[i]->pred
          && !prec_nodes[i]->succ)
        {
          जर (is_assoc_useless (s))
            complain (&s->content->prec_loc, Wprecedence,
                      _("useless precedence and associativity for %s"), s->tag);
          अन्यथा जर (s->content->assoc == precedence_assoc)
            complain (&s->content->prec_loc, Wprecedence,
                      _("useless precedence for %s"), s->tag);
        }
      अन्यथा जर (is_assoc_useless (s))
        complain (&s->content->prec_loc, Wprecedence,
                  _("useless associativity for %s, use %%precedence"), s->tag);
    }
  मुक्त (used_assoc);
  assoc_free ();
}
