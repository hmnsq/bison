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

#समावेश <config.h>
#समावेश "symtab.h"

#समावेश "system.h"

#समावेश <assure.h>
#समावेश <fstrcmp.h>
#समावेश <hash.h>
#समावेश <quote.h>

#समावेश "complain.h"
#समावेश "getargs.h"
#समावेश "gram.h"
#समावेश "intprops.h"

/** Undefined token code.  */
#घोषणा CODE_UNDEFINED (-1)

/* Undefined symbol number.  */
#घोषणा संख्या_UNDEFINED (-1)


अटल काष्ठा hash_table *प्रतीक_table = शून्य;
अटल काष्ठा hash_table *semantic_type_table = शून्य;

/*----------------------------------------------------------------.
| Symbols sorted by tag.  Allocated by table_sort, after which no |
| more symbols should be created.                                 |
`----------------------------------------------------------------*/

अटल प्रतीक **symbols_sorted = शून्य;
अटल semantic_type **semantic_types_sorted = शून्य;


/*------------------------.
| Distinguished symbols.  |
`------------------------*/

प्रतीक *errtoken = शून्य;
प्रतीक *undeftoken = शून्य;
प्रतीक *eoftoken = शून्य;
प्रतीक *acceptsymbol = शून्य;

/* Precedence relation graph. */
अटल symgraph **prec_nodes;

/* Store which associativity is used.  */
अटल bool *used_assoc = शून्य;

bool tag_seen = false;


/* Whether SYM was defined by the user.  */

अटल bool
प्रतीक_is_user_defined (प्रतीक *sym)
{
  स्थिर bool eof_is_user_defined
    = !eoftoken->alias || STRNEQ (eoftoken->alias->tag, "$end");
  वापस sym->tag[0] != '$'
    && (eof_is_user_defined || (sym != eoftoken && sym->alias != errtoken))
    && sym != errtoken && sym->alias != errtoken
    && sym != undeftoken && sym->alias != undeftoken;
}


/*--------------------------.
| Create a new sym_content. |
`--------------------------*/

अटल sym_content *
sym_content_new (प्रतीक *s)
{
  sym_content *res = xmalloc (माप *res);

  res->प्रतीक = s;

  res->type_name = शून्य;
  res->type_loc = empty_loc;
  क्रम (पूर्णांक i = 0; i < CODE_PROPS_आकार; ++i)
    code_props_none_init (&res->props[i]);

  res->संख्या = संख्या_UNDEFINED;
  res->prec_loc = empty_loc;
  res->prec = 0;
  res->assoc = undef_assoc;
  res->code = CODE_UNDEFINED;

  res->class = unknown_sym;
  res->status = undeclared;

  वापस res;
}

/*---------------------------------.
| Create a new symbol, named TAG.  |
`---------------------------------*/

अटल प्रतीक *
प्रतीक_new (uniqstr tag, location loc)
{
  प्रतीक *res = xmalloc (माप *res);
  uniqstr_assert (tag);

  /* If the tag is not a string (starts with a double quote), check
     that it is valid for Yacc. */
  अगर (tag[0] != '\"' && tag[0] != '\'' && म_अक्षर (tag, '-'))
    complain (&loc, Wyacc,
              _("POSIX Yacc forbids dashes in symbol names: %s"), tag);

  res->tag = tag;
  res->location = loc;
  res->translatable = false;
  res->location_of_lhs = false;
  res->alias = शून्य;
  res->content = sym_content_new (res);
  res->is_alias = false;
  वापस res;
}

/*--------------------.
| Free a sym_content. |
`--------------------*/

अटल व्योम
sym_content_free (sym_content *sym)
{
  मुक्त (sym);
}


/*---------------------------------------------------------.
| Free a symbol and its associated content if appropriate. |
`---------------------------------------------------------*/

अटल व्योम
प्रतीक_free (व्योम *ptr)
{
  प्रतीक *sym = (प्रतीक *)ptr;
  अगर (!sym->is_alias)
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

अटल व्योम
symbols_sort (स्थिर प्रतीक **first, स्थिर प्रतीक **second)
{
  अगर (0 < location_cmp ((*first)->location, (*second)->location))
    {
      स्थिर प्रतीक* tmp = *first;
      *first = *second;
      *second = tmp;
    }
}

/* Likewise, for locations.  */

अटल व्योम
locations_sort (location *first, location *second)
{
  अगर (0 < location_cmp (*first, *second))
    {
      location tmp = *first;
      *first = *second;
      *second = tmp;
    }
}

अक्षर स्थिर *
code_props_type_string (code_props_type kind)
{
  चयन (kind)
    {
    हाल destructor:
      वापस "%destructor";
    हाल printer:
      वापस "%printer";
    }
  पात ();
}


/*----------------------------------------.
| Create a new semantic type, named TAG.  |
`----------------------------------------*/

अटल semantic_type *
semantic_type_new (uniqstr tag, स्थिर location *loc)
{
  semantic_type *res = xmalloc (माप *res);

  uniqstr_assert (tag);
  res->tag = tag;
  res->location = loc ? *loc : empty_loc;
  res->status = undeclared;
  क्रम (पूर्णांक i = 0; i < CODE_PROPS_आकार; ++i)
    code_props_none_init (&res->props[i]);

  वापस res;
}


/*-----------------.
| Print a symbol.  |
`-----------------*/

#घोषणा प्रतीक_INT_ATTR_PRINT(Attr)                     \
  अगर (s->content)                                       \
    ख_लिखो (f, " %s = %d", #Attr, s->content->Attr)

#घोषणा प्रतीक_STR_ATTR_PRINT(Attr)                     \
  अगर (s->content && s->content->Attr)                   \
    ख_लिखो (f, " %s { %s }", #Attr, s->content->Attr)

#घोषणा प्रतीक_CODE_PRINT(Attr)                                         \
  अगर (s->content && s->content->props[Attr].code)                       \
    ख_लिखो (f, " %s { %s }", #Attr, s->content->props[Attr].code)

व्योम
प्रतीक_print (प्रतीक स्थिर *s, खाता *f)
{
  अगर (s)
    {
      प्रतीक_class c = s->content->class;
      ख_लिखो (f, "%s: %s",
               c == unknown_sym    ? "unknown"
               : c == pct_type_sym ? "%type"
               : c == token_sym    ? "token"
               : c == nterm_sym    ? "nterm"
               : शून्य, /* abort.  */
               s->tag);
      अ_दो (' ', f);
      location_print (s->location, f);
      प्रतीक_INT_ATTR_PRINT (code);
      प्रतीक_INT_ATTR_PRINT (संख्या);
      प्रतीक_STR_ATTR_PRINT (type_name);
      प्रतीक_CODE_PRINT (destructor);
      प्रतीक_CODE_PRINT (printer);
    }
  अन्यथा
    ख_माला_दो ("<NULL>", f);
}

#अघोषित प्रतीक_ATTR_PRINT
#अघोषित प्रतीक_CODE_PRINT


/*----------------------------------.
| Whether S is a valid identifier.  |
`----------------------------------*/

अटल bool
is_identifier (uniqstr s)
{
  अटल अक्षर स्थिर alphanum[26 + 26 + 1 + 10] =
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "_"
    "0123456789";
  अगर (!s || ! स_प्रथम (alphanum, *s, माप alphanum - 10))
    वापस false;
  क्रम (++s; *s; ++s)
    अगर (! स_प्रथम (alphanum, *s, माप alphanum))
      वापस false;
  वापस true;
}


/*-----------------------------------------------.
| Get the identifier associated to this symbol.  |
`-----------------------------------------------*/

uniqstr
प्रतीक_id_get (प्रतीक स्थिर *sym)
{
  // There's one weird case: YYerror is the alias, and error is the
  // base symbol.  Return YYerror in that case.
  अगर (sym->alias && is_identifier (sym->alias->tag))
    वापस sym->alias->tag;
  अन्यथा अगर (is_identifier (sym->tag))
    वापस sym->tag;
  अन्यथा
    वापस शून्य;
}


/*------------------------------------------------------------------.
| Complain that S's WHAT is redeclared at SECOND, and was first set |
| at FIRST.                                                         |
`------------------------------------------------------------------*/

अटल व्योम
complain_प्रतीक_redeclared (प्रतीक *s, स्थिर अक्षर *what, location first,
                            location second)
{
  locations_sort (&first, &second);
  complain (&second, complaint, _("%s redeclaration for %s"), what, s->tag);
  subcomplain (&first, complaint, _("previous declaration"));
}

अटल व्योम
complain_semantic_type_redeclared (semantic_type *s, स्थिर अक्षर *what, location first,
                                   location second)
{
  locations_sort (&first, &second);
  complain (&second, complaint, _("%s redeclaration for <%s>"), what, s->tag);
  subcomplain (&first, complaint, _("previous declaration"));
}

अटल व्योम
complain_class_redeclared (प्रतीक *sym, प्रतीक_class class, location second)
{
  complain (&second, complaint,
            class == token_sym
            ? _("symbol %s redeclared as a token")
            : _("symbol %s redeclared as a nonterminal"), sym->tag);
  अगर (!location_empty (sym->location))
    subcomplain (&sym->location, complaint, _("previous definition"));
}

अटल स्थिर प्रतीक *
प्रतीक_from_uniqstr_fuzzy (स्थिर uniqstr कुंजी)
{
  aver (symbols_sorted);
#घोषणा FSTRCMP_THRESHOLD 0.6
  द्विगुन best_similarity = FSTRCMP_THRESHOLD;
  स्थिर प्रतीक *res = शून्य;
  माप_प्रकार गिनती = hash_get_n_entries (प्रतीक_table);
  क्रम (पूर्णांक i = 0; i < गिनती; ++i)
    {
      प्रतीक *sym = symbols_sorted[i];
      अगर (STRNEQ (कुंजी, sym->tag)
          && (sym->content->status == declared
              || sym->content->status == undeclared))
        {
          द्विगुन similarity = fstrcmp_bounded (कुंजी, sym->tag, best_similarity);
          अगर (best_similarity < similarity)
            {
              res = sym;
              best_similarity = similarity;
            }
        }
    }
  वापस res;
}

अटल व्योम
complain_प्रतीक_undeclared (स्थिर प्रतीक *sym)
{
  निश्चित (sym->content->status != declared);
  स्थिर प्रतीक *best = प्रतीक_from_uniqstr_fuzzy (sym->tag);
  अगर (best)
    {
      complain (&sym->location,
                sym->content->status == needed ? complaint : Wother,
                _("symbol %s is used, but is not defined as a token"
                  " and has no rules; did you mean %s?"),
                quote_n (0, sym->tag),
                quote_n (1, best->tag));
      अगर (feature_flag & feature_caret)
        location_caret_suggestion (sym->location, best->tag, मानक_त्रुटि);
    }
  अन्यथा
    complain (&sym->location,
              sym->content->status == needed ? complaint : Wother,
              _("symbol %s is used, but is not defined as a token"
                " and has no rules"),
              quote (sym->tag));
}

व्योम
प्रतीक_location_as_lhs_set (प्रतीक *sym, location loc)
{
  अगर (!sym->location_of_lhs)
    {
      sym->location = loc;
      sym->location_of_lhs = true;
    }
}


/*-----------------------------------------------------------------.
| Set the TYPE_NAME associated with SYM.  Does nothing if passed 0 |
| as TYPE_NAME.                                                    |
`-----------------------------------------------------------------*/

व्योम
प्रतीक_type_set (प्रतीक *sym, uniqstr type_name, location loc)
{
  अगर (type_name)
    {
      tag_seen = true;
      अगर (sym->content->type_name)
        complain_प्रतीक_redeclared (sym, "%type",
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

व्योम
प्रतीक_code_props_set (प्रतीक *sym, code_props_type kind,
                       code_props स्थिर *code)
{
  अगर (sym->content->props[kind].code)
    complain_प्रतीक_redeclared (sym, code_props_type_string (kind),
                                sym->content->props[kind].location,
                                code->location);
  अन्यथा
    sym->content->props[kind] = *code;
}


/*-----------------------------------------------------.
| Set the DESTRUCTOR or PRINTER associated with TYPE.  |
`-----------------------------------------------------*/

व्योम
semantic_type_code_props_set (semantic_type *type,
                              code_props_type kind,
                              code_props स्थिर *code)
{
  अगर (type->props[kind].code)
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
प्रतीक_code_props_get (प्रतीक *sym, code_props_type kind)
{
  /* Per-symbol code props.  */
  अगर (sym->content->props[kind].code)
    वापस &sym->content->props[kind];

  /* Per-type code props.  */
  अगर (sym->content->type_name)
    {
      code_props *code =
        &semantic_type_get (sym->content->type_name, शून्य)->props[kind];
      अगर (code->code)
        वापस code;
    }

  /* Apply default code props's only to user-defined symbols.  */
  अगर (प्रतीक_is_user_defined (sym))
    {
      code_props *code = &semantic_type_get (sym->content->type_name ? "*" : "",
                                             शून्य)->props[kind];
      अगर (code->code)
        वापस code;
    }
  वापस &code_props_none;
}

/*-----------------------------------------------------------------.
| Set the PRECEDENCE associated with SYM.  Does nothing if invoked |
| with UNDEF_ASSOC as ASSOC.                                       |
`-----------------------------------------------------------------*/

व्योम
प्रतीक_precedence_set (प्रतीक *sym, पूर्णांक prec, assoc a, location loc)
{
  अगर (a != undef_assoc)
    {
      sym_content *s = sym->content;
      अगर (s->prec)
        complain_प्रतीक_redeclared (sym, assoc_to_string (a),
                                    s->prec_loc, loc);
      अन्यथा
        {
          s->prec = prec;
          s->assoc = a;
          s->prec_loc = loc;
        }
    }

  /* Only terminals have a precedence. */
  प्रतीक_class_set (sym, token_sym, loc, false);
}


/*------------------------------------.
| Set the CLASS associated with SYM.  |
`------------------------------------*/

अटल व्योम
complain_pct_type_on_token (location *loc)
{
  complain (loc, Wyacc,
            _("POSIX yacc reserves %%type to nonterminals"));
}

व्योम
प्रतीक_class_set (प्रतीक *sym, प्रतीक_class class, location loc, bool declaring)
{
  aver (class != unknown_sym);
  sym_content *s = sym->content;
  अगर (class == pct_type_sym)
    {
      अगर (s->class == token_sym)
        complain_pct_type_on_token (&loc);
      अन्यथा अगर (s->class == unknown_sym)
        s->class = class;
    }
  अन्यथा अगर (s->class != unknown_sym && s->class != pct_type_sym
           && s->class != class)
    complain_class_redeclared (sym, class, loc);
  अन्यथा
    {
      अगर (class == token_sym && s->class == pct_type_sym)
        complain_pct_type_on_token (&sym->location);

      s->class = class;

      अगर (declaring)
        {
          अगर (s->status == declared)
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

व्योम
प्रतीक_code_set (प्रतीक *sym, पूर्णांक code, location loc)
{
  पूर्णांक *codep = &sym->content->code;
  अगर (sym->content->class != token_sym)
    complain (&loc, complaint,
              _("nonterminals cannot be given a token code"));
  अन्यथा अगर (*codep != CODE_UNDEFINED
           && *codep != code)
    complain (&loc, complaint, _("redefining code of token %s"),
              sym->tag);
  अन्यथा अगर (code == पूर्णांक_उच्च)
    complain (&loc, complaint, _("code of token %s too large"),
              sym->tag);
  अन्यथा
    {
      *codep = code;
      /* User defined $end token? */
      अगर (code == 0 && !eoftoken)
        {
          eoftoken = sym->content->प्रतीक;
          eoftoken->content->संख्या = 0;
        }
    }
}


/*----------------------------------------------------------.
| If SYM is not defined, report an error, and consider it a |
| nonterminal.                                              |
`----------------------------------------------------------*/

अटल व्योम
प्रतीक_check_defined (प्रतीक *sym)
{
  sym_content *s = sym->content;
  अगर (s->class == unknown_sym || s->class == pct_type_sym)
    {
      complain_प्रतीक_undeclared (sym);
      s->class = nterm_sym;
    }

  अगर (s->संख्या == संख्या_UNDEFINED)
    s->संख्या = s->class == token_sym ? ntokens++ : nnterms++;

  अगर (s->class == token_sym
      && sym->tag[0] == '"'
      && !sym->is_alias)
    complain (&sym->location, Wdangling_alias,
              _("string literal %s not attached to a symbol"),
              sym->tag);

  क्रम (पूर्णांक i = 0; i < 2; ++i)
    प्रतीक_code_props_get (sym, i)->is_used = true;

  /* Set the semantic type status associated to the current symbol to
     'declared' so that we could check semantic types unnecessary uses. */
  अगर (s->type_name)
    {
      semantic_type *sem_type = semantic_type_get (s->type_name, शून्य);
      अगर (sem_type)
        sem_type->status = declared;
    }
}

अटल व्योम
semantic_type_check_defined (semantic_type *sem_type)
{
  /* <*> and <> do not have to be "declared".  */
  अगर (sem_type->status == declared
      || !*sem_type->tag
      || STREQ (sem_type->tag, "*"))
    {
      क्रम (पूर्णांक i = 0; i < 2; ++i)
        अगर (sem_type->props[i].kind != CODE_PROPS_NONE
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

अटल व्योम
प्रतीक_merge_properties (प्रतीक *sym, प्रतीक *str)
{
  अगर (str->content->type_name != sym->content->type_name)
    {
      अगर (str->content->type_name)
        प्रतीक_type_set (sym,
                         str->content->type_name, str->content->type_loc);
      अन्यथा
        प्रतीक_type_set (str,
                         sym->content->type_name, sym->content->type_loc);
    }


  क्रम (पूर्णांक i = 0; i < CODE_PROPS_आकार; ++i)
    अगर (str->content->props[i].code)
      प्रतीक_code_props_set (sym, i, &str->content->props[i]);
    अन्यथा अगर (sym->content->props[i].code)
      प्रतीक_code_props_set (str, i, &sym->content->props[i]);

  अगर (sym->content->prec || str->content->prec)
    {
      अगर (str->content->prec)
        प्रतीक_precedence_set (sym, str->content->prec, str->content->assoc,
                               str->content->prec_loc);
      अन्यथा
        प्रतीक_precedence_set (str, sym->content->prec, sym->content->assoc,
                               sym->content->prec_loc);
    }
}

व्योम
प्रतीक_make_alias (प्रतीक *sym, प्रतीक *str, location loc)
{
  अगर (sym->content->class != token_sym)
    complain (&loc, complaint,
              _("nonterminals cannot be given a string alias"));
  अन्यथा अगर (str->alias)
    complain (&loc, Wother,
              _("symbol %s used more than once as a literal string"), str->tag);
  अन्यथा अगर (sym->alias)
    complain (&loc, Wother,
              _("symbol %s given more than one literal string"), sym->tag);
  अन्यथा
    {
      प्रतीक_merge_properties (sym, str);
      sym_content_free (str->content);
      str->content = sym->content;
      str->content->प्रतीक = str;
      str->is_alias = true;
      str->alias = sym;
      sym->alias = str;
    }
}


/*-------------------------------------------------------------------.
| Assign a symbol number, and write the definition of the token name |
| into FDEFINES.  Put in SYMBOLS.                                    |
`-------------------------------------------------------------------*/

अटल व्योम
प्रतीक_pack (प्रतीक *sym)
{
  aver (sym->content->संख्या != संख्या_UNDEFINED);
  अगर (sym->content->class == nterm_sym)
    sym->content->संख्या += ntokens;

  symbols[sym->content->संख्या] = sym->content->प्रतीक;
}

अटल व्योम
complain_code_redeclared (पूर्णांक अंक, स्थिर प्रतीक *first, स्थिर प्रतीक *second)
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

अटल व्योम
प्रतीक_translation (स्थिर प्रतीक *sym)
{
  अगर (sym->content->class == token_sym && !sym->is_alias)
    {
      /* A token whose translation has already been set? */
      अगर (token_translations[sym->content->code]
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
#घोषणा HT_INITIAL_CAPACITY 257

अटल अंतरभूत bool
hash_compare_प्रतीक (स्थिर प्रतीक *m1, स्थिर प्रतीक *m2)
{
  /* Since tags are unique, we can compare the pointers themselves.  */
  वापस UNIQSTR_EQ (m1->tag, m2->tag);
}

अटल अंतरभूत bool
hash_compare_semantic_type (स्थिर semantic_type *m1, स्थिर semantic_type *m2)
{
  /* Since names are unique, we can compare the pointers themselves.  */
  वापस UNIQSTR_EQ (m1->tag, m2->tag);
}

अटल bool
hash_प्रतीक_comparator (व्योम स्थिर *m1, व्योम स्थिर *m2)
{
  वापस hash_compare_प्रतीक (m1, m2);
}

अटल bool
hash_semantic_type_comparator (व्योम स्थिर *m1, व्योम स्थिर *m2)
{
  वापस hash_compare_semantic_type (m1, m2);
}

अटल अंतरभूत माप_प्रकार
hash_प्रतीक (स्थिर प्रतीक *m, माप_प्रकार tablesize)
{
  /* Since tags are unique, we can hash the pointer itself.  */
  वापस ((uintptr_t) m->tag) % tablesize;
}

अटल अंतरभूत माप_प्रकार
hash_semantic_type (स्थिर semantic_type *m, माप_प्रकार tablesize)
{
  /* Since names are unique, we can hash the pointer itself.  */
  वापस ((uintptr_t) m->tag) % tablesize;
}

अटल माप_प्रकार
hash_प्रतीक_hasher (व्योम स्थिर *m, माप_प्रकार tablesize)
{
  वापस hash_प्रतीक (m, tablesize);
}

अटल माप_प्रकार
hash_semantic_type_hasher (व्योम स्थिर *m, माप_प्रकार tablesize)
{
  वापस hash_semantic_type (m, tablesize);
}

/*-------------------------------.
| Create the symbol hash table.  |
`-------------------------------*/

व्योम
symbols_new (व्योम)
{
  प्रतीक_table = hash_xinitialize (HT_INITIAL_CAPACITY,
                                   शून्य,
                                   hash_प्रतीक_hasher,
                                   hash_प्रतीक_comparator,
                                   प्रतीक_free);

  /* Construct the acceptsymbol symbol. */
  acceptsymbol = प्रतीक_get ("$accept", empty_loc);
  acceptsymbol->content->class = nterm_sym;
  acceptsymbol->content->संख्या = nnterms++;

  /* Construct the YYerror/"error" token */
  errtoken = प्रतीक_get ("YYerror", empty_loc);
  errtoken->content->class = token_sym;
  errtoken->content->संख्या = ntokens++;
  {
    प्रतीक *alias = प्रतीक_get ("error", empty_loc);
    प्रतीक_class_set (alias, token_sym, empty_loc, false);
    प्रतीक_make_alias (errtoken, alias, empty_loc);
  }

  /* Construct the YYUNDEF/"$undefined" token that represents all
     undefined literal tokens.  It is always symbol number 2.  */
  undeftoken = प्रतीक_get ("YYUNDEF", empty_loc);
  undeftoken->content->class = token_sym;
  undeftoken->content->संख्या = ntokens++;
  {
    प्रतीक *alias = प्रतीक_get ("$undefined", empty_loc);
    प्रतीक_class_set (alias, token_sym, empty_loc, false);
    प्रतीक_make_alias (undeftoken, alias, empty_loc);
  }

  semantic_type_table = hash_xinitialize (HT_INITIAL_CAPACITY,
                                          शून्य,
                                          hash_semantic_type_hasher,
                                          hash_semantic_type_comparator,
                                          मुक्त);
}


/*----------------------------------------------------------------.
| Find the symbol named KEY, and return it.  If it does not exist |
| yet, create it.                                                 |
`----------------------------------------------------------------*/

प्रतीक *
प्रतीक_from_uniqstr (स्थिर uniqstr कुंजी, location loc)
{
  प्रतीक probe;

  probe.tag = कुंजी;
  प्रतीक *res = hash_lookup (प्रतीक_table, &probe);

  अगर (!res)
    {
      /* First insertion in the hash. */
      aver (!symbols_sorted);
      res = प्रतीक_new (कुंजी, loc);
      hash_xinsert (प्रतीक_table, res);
    }
  वापस res;
}


/*-----------------------------------------------------------------------.
| Find the semantic type named KEY, and return it.  If it does not exist |
| yet, create it.                                                        |
`-----------------------------------------------------------------------*/

semantic_type *
semantic_type_from_uniqstr (स्थिर uniqstr कुंजी, स्थिर location *loc)
{
  semantic_type probe;

  probe.tag = कुंजी;
  semantic_type *res = hash_lookup (semantic_type_table, &probe);

  अगर (!res)
    {
      /* First insertion in the hash. */
      res = semantic_type_new (कुंजी, loc);
      hash_xinsert (semantic_type_table, res);
    }
  वापस res;
}


/*----------------------------------------------------------------.
| Find the symbol named KEY, and return it.  If it does not exist |
| yet, create it.                                                 |
`----------------------------------------------------------------*/

प्रतीक *
प्रतीक_get (स्थिर अक्षर *कुंजी, location loc)
{
  वापस प्रतीक_from_uniqstr (uniqstr_new (कुंजी), loc);
}


/*-----------------------------------------------------------------------.
| Find the semantic type named KEY, and return it.  If it does not exist |
| yet, create it.                                                        |
`-----------------------------------------------------------------------*/

semantic_type *
semantic_type_get (स्थिर अक्षर *कुंजी, स्थिर location *loc)
{
  वापस semantic_type_from_uniqstr (uniqstr_new (कुंजी), loc);
}


/*------------------------------------------------------------------.
| Generate a dummy nonterminal, whose name cannot conflict with the |
| user's names.                                                     |
`------------------------------------------------------------------*/

प्रतीक *
dummy_प्रतीक_get (location loc)
{
  /* Incremented for each generated symbol.  */
  अटल पूर्णांक dummy_गिनती = 0;
  अक्षर buf[32];
  पूर्णांक len = snprintf (buf, माप buf, "$@%d", ++dummy_गिनती);
  assure (len < माप buf);
  प्रतीक *sym = प्रतीक_get (buf, loc);
  sym->content->class = nterm_sym;
  वापस sym;
}

bool
प्रतीक_is_dummy (प्रतीक स्थिर *sym)
{
  वापस sym->tag[0] == '@' || (sym->tag[0] == '$' && sym->tag[1] == '@');
}

/*-------------------.
| Free the symbols.  |
`-------------------*/

व्योम
symbols_free (व्योम)
{
  hash_free (प्रतीक_table);
  hash_free (semantic_type_table);
  मुक्त (symbols);
  मुक्त (symbols_sorted);
  मुक्त (semantic_types_sorted);
}


अटल पूर्णांक
प्रतीक_cmp (व्योम स्थिर *a, व्योम स्थिर *b)
{
  वापस location_cmp ((*(प्रतीक * स्थिर *)a)->location,
                       (*(प्रतीक * स्थिर *)b)->location);
}

/* Store in *SORTED an array of pointers to the symbols contained in
   TABLE, sorted by order of appearance (i.e., by location). */

अटल व्योम
table_sort (काष्ठा hash_table *table, प्रतीक ***sorted)
{
  aver (!*sorted);
  माप_प्रकार गिनती = hash_get_n_entries (table);
  *sorted = xnmalloc (गिनती + 1, माप **sorted);
  hash_get_entries (table, (व्योम**)*sorted, गिनती);
  क्विक (*sorted, गिनती, माप **sorted, प्रतीक_cmp);
  (*sorted)[गिनती] = शून्य;
}

/*--------------------------------------------------------------.
| Check that all the symbols are defined.  Report any undefined |
| symbols and consider them nonterminals.                       |
`--------------------------------------------------------------*/

व्योम
symbols_check_defined (व्योम)
{
  table_sort (प्रतीक_table, &symbols_sorted);
  /* semantic_type, like symbol, starts with a 'tag' field and then a
     'location' field.  And here we only deal with arrays/hashes of
     pointers, sizeof is not an issue.

     So instead of implementing table_sort (and symbol_cmp) once for
     each type, let's lie a bit to the typing system, and treat
     'semantic_type' as if it were 'symbol'. */
  table_sort (semantic_type_table, (प्रतीक ***) &semantic_types_sorted);

  क्रम (पूर्णांक i = 0; symbols_sorted[i]; ++i)
    प्रतीक_check_defined (symbols_sorted[i]);
  क्रम (पूर्णांक i = 0; semantic_types_sorted[i]; ++i)
    semantic_type_check_defined (semantic_types_sorted[i]);
}

/*------------------------------------------------------------------.
| Set TOKEN_TRANSLATIONS.  Check that no two symbols share the same |
| number.                                                           |
`------------------------------------------------------------------*/

अटल व्योम
symbols_token_translations_init (व्योम)
{
  bool code_256_available_p = true;

  /* Find the highest token code, and whether 256, the POSIX preferred
     token code for the error token, is used.  */
  max_code = 0;
  क्रम (पूर्णांक i = 0; i < ntokens; ++i)
    {
      sym_content *sym = symbols[i]->content;
      अगर (sym->code != CODE_UNDEFINED)
        {
          अगर (sym->code > max_code)
            max_code = sym->code;
          अगर (sym->code == 256)
            code_256_available_p = false;
        }
    }

  /* If 256 is not used, assign it to error, to follow POSIX.  */
  अगर (code_256_available_p
      && errtoken->content->code == CODE_UNDEFINED)
    errtoken->content->code = 256;

  /* Set the missing codes. */
  अगर (max_code < 256)
    max_code = 256;

  क्रम (पूर्णांक i = 0; i < ntokens; ++i)
    {
      sym_content *sym = symbols[i]->content;
      अगर (sym->code == CODE_UNDEFINED)
        {
          IGNORE_TYPE_LIMITS_BEGIN
          अगर (INT_ADD_WRAPV (max_code, 1, &max_code))
            complain (शून्य, fatal, _("token number too large"));
          IGNORE_TYPE_LIMITS_END
          sym->code = max_code;
        }
      अगर (sym->code > max_code)
        max_code = sym->code;
    }

  token_translations = xnmalloc (max_code + 1,
                                 माप *token_translations);

  /* Initialize all entries for literal tokens to the internal token
     number for $undefined, which represents all invalid inputs.  */
  क्रम (पूर्णांक i = 0; i < max_code + 1; ++i)
    token_translations[i] = undeftoken->content->संख्या;
  क्रम (पूर्णांक i = 0; symbols_sorted[i]; ++i)
    प्रतीक_translation (symbols_sorted[i]);
}


/* Whether some symbol requires internationalization.  */
अटल bool
has_translations (व्योम)
{
  क्रम (स्थिर व्योम *entry = hash_get_first (प्रतीक_table);
       entry;
       entry = hash_get_अग्र (प्रतीक_table, entry))
    {
      स्थिर प्रतीक *sym = (स्थिर प्रतीक *) entry;
      अगर (sym->translatable)
        वापस true;
    }
  वापस false;
}

/*----------------------------------------------------------------.
| Assign symbol numbers, and write definition of token names into |
| FDEFINES.  Set up vectors SYMBOL_TABLE, TAGS of symbols.        |
`----------------------------------------------------------------*/

व्योम
symbols_pack (व्योम)
{
  symbols = xcalloc (nsyms, माप *symbols);
  क्रम (पूर्णांक i = 0; symbols_sorted[i]; ++i)
    प्रतीक_pack (symbols_sorted[i]);

  /* Aliases leave empty slots in symbols, so remove them.  */
  {
    पूर्णांक nsyms_old = nsyms;
    क्रम (पूर्णांक writei = 0, readi = 0; readi < nsyms_old; readi += 1)
      {
        अगर (symbols[readi] == शून्य)
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
  symbols = xnrealloc (symbols, nsyms, माप *symbols);

  symbols_token_translations_init ();

  // If some user tokens are internationalized, the internal ones
  // should be too.
  अगर (has_translations ())
    {
      स्थिर bool eof_is_user_defined
        = !eoftoken->alias || STRNEQ (eoftoken->alias->tag, "$end");
      अगर (!eof_is_user_defined)
        eoftoken->alias->translatable = true;
      undeftoken->alias->translatable = true;
      errtoken->alias->translatable = true;
    }
}

/*---------------------------------.
| Initialize relation graph nodes. |
`---------------------------------*/

अटल व्योम
init_prec_nodes (व्योम)
{
  prec_nodes = xcalloc (nsyms, माप *prec_nodes);
  क्रम (पूर्णांक i = 0; i < nsyms; ++i)
    {
      prec_nodes[i] = xmalloc (माप *prec_nodes[i]);
      symgraph *s = prec_nodes[i];
      s->id = i;
      s->succ = 0;
      s->pred = 0;
    }
}

/*----------------.
| Create a link.  |
`----------------*/

अटल symgraphlink *
symgraphlink_new (graphid id, symgraphlink *अग्र)
{
  symgraphlink *res = xmalloc (माप *res);
  res->id = id;
  res->अग्र = अग्र;
  वापस res;
}


/*------------------------------------------------------------------.
| Register the second symbol of the precedence relation, and return |
| whether this relation is new.  Use only in register_precedence.   |
`------------------------------------------------------------------*/

अटल bool
register_precedence_second_प्रतीक (symgraphlink **first, graphid sym)
{
  अगर (!*first || sym < (*first)->id)
    *first = symgraphlink_new (sym, *first);
  अन्यथा
    {
      symgraphlink *slist = *first;

      जबतक (slist->अग्र && slist->अग्र->id <= sym)
        slist = slist->अग्र;

      अगर (slist->id == sym)
        /* Relation already present. */
        वापस false;

      slist->अग्र = symgraphlink_new (sym, slist->अग्र);
    }
  वापस true;
}

/*------------------------------------------------------------------.
| Register a new relation between symbols as used. The first symbol |
| has a greater precedence than the second one.                     |
`------------------------------------------------------------------*/

व्योम
register_precedence (graphid first, graphid snd)
{
  अगर (!prec_nodes)
    init_prec_nodes ();
  register_precedence_second_प्रतीक (&(prec_nodes[first]->succ), snd);
  register_precedence_second_प्रतीक (&(prec_nodes[snd]->pred), first);
}


/*---------------------------------------.
| Deep clear a linked / adjacency list). |
`---------------------------------------*/

अटल व्योम
linkedlist_free (symgraphlink *गाँठ)
{
  अगर (गाँठ)
    {
      जबतक (गाँठ->अग्र)
        {
          symgraphlink *tmp = गाँठ->अग्र;
          मुक्त (गाँठ);
          गाँठ = tmp;
        }
      मुक्त (गाँठ);
    }
}

/*----------------------------------------------.
| Clear and destroy association tracking table. |
`----------------------------------------------*/

अटल व्योम
assoc_free (व्योम)
{
  क्रम (पूर्णांक i = 0; i < nsyms; ++i)
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

अटल व्योम
init_assoc (व्योम)
{
  used_assoc = xcalloc (nsyms, माप *used_assoc);
  क्रम (graphid i = 0; i < nsyms; ++i)
    used_assoc[i] = false;
}

/*------------------------------------------------------------------.
| Test if the associativity for the symbols is defined and useless. |
`------------------------------------------------------------------*/

अटल अंतरभूत bool
is_assoc_useless (प्रतीक *s)
{
  वापस s
      && s->content->assoc != undef_assoc
      && s->content->assoc != precedence_assoc
      && !used_assoc[s->content->संख्या];
}

/*-------------------------------.
| Register a used associativity. |
`-------------------------------*/

व्योम
register_assoc (graphid i, graphid j)
{
  अगर (!used_assoc)
    init_assoc ();
  used_assoc[i] = true;
  used_assoc[j] = true;
}

/*--------------------------------------------------.
| Print a warning for unused precedence relations.  |
`--------------------------------------------------*/

व्योम
print_precedence_warnings (व्योम)
{
  अगर (!prec_nodes)
    init_prec_nodes ();
  अगर (!used_assoc)
    init_assoc ();
  क्रम (पूर्णांक i = 0; i < nsyms; ++i)
    {
      प्रतीक *s = symbols[i];
      अगर (s
          && s->content->prec != 0
          && !prec_nodes[i]->pred
          && !prec_nodes[i]->succ)
        {
          अगर (is_assoc_useless (s))
            complain (&s->content->prec_loc, Wprecedence,
                      _("useless precedence and associativity for %s"), s->tag);
          अन्यथा अगर (s->content->assoc == precedence_assoc)
            complain (&s->content->prec_loc, Wprecedence,
                      _("useless precedence for %s"), s->tag);
        }
      अन्यथा अगर (is_assoc_useless (s))
        complain (&s->content->prec_loc, Wprecedence,
                  _("useless associativity for %s, use %%precedence"), s->tag);
    }
  मुक्त (used_assoc);
  assoc_free ();
}
