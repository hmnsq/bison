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
#define सङ्ख्या_UNDEFINED (-1)


स्थिर संरचना hash_table *चिह्न_table = रिक्त;
स्थिर संरचना hash_table *semantic_type_table = रिक्त;

/*----------------------------------------------------------------.
| Symbols sorted by tag.  Allocated by table_sort, after which no |
| more symbols should be created.                                 |
`----------------------------------------------------------------*/

स्थिर चिह्न **symbols_sorted = रिक्त;
स्थिर semantic_type **semantic_types_sorted = रिक्त;


/*------------------------.
| Distinguished symbols.  |
`------------------------*/

चिह्न *errtoken = रिक्त;
चिह्न *undeftoken = रिक्त;
चिह्न *eoftoken = रिक्त;
चिह्न *acceptsymbol = रिक्त;

/* Precedence relation graph. */
स्थिर symgraph **prec_nodes;

/* Store which associativity is used.  */
स्थिर bool *used_assoc = रिक्त;

bool tag_seen = false;


/* Whether SYM was defined by the user.  */

स्थिर bool
चिह्न_is_user_defined (चिह्न *sym)
{
  अचर bool eof_is_user_defined
    = !eoftoken->alias || STRNEQ (eoftoken->alias->tag, "$end");
  फर्काऊ sym->tag[0] != '$'
    && (eof_is_user_defined || (sym != eoftoken && sym->alias != errtoken))
    && sym != errtoken && sym->alias != errtoken
    && sym != undeftoken && sym->alias != undeftoken;
}


/*--------------------------.
| Create a new sym_content. |
`--------------------------*/

स्थिर sym_content *
sym_content_new (चिह्न *s)
{
  sym_content *res = xmalloc (परिमाण *res);

  res->चिह्न = s;

  res->type_name = रिक्त;
  res->type_loc = empty_loc;
  लागि (पूर्णाङ्क i = 0; i < CODE_PROPS_SIZE; ++i)
    code_props_none_init (&res->props[i]);

  res->सङ्ख्या = सङ्ख्या_UNDEFINED;
  res->prec_loc = empty_loc;
  res->prec = 0;
  res->assoc = undef_assoc;
  res->code = CODE_UNDEFINED;

  res->class = unknown_sym;
  res->status = undeclared;

  फर्काऊ res;
}

/*---------------------------------.
| Create a new symbol, named TAG.  |
`---------------------------------*/

स्थिर चिह्न *
चिह्न_new (uniqstr tag, location loc)
{
  चिह्न *res = xmalloc (परिमाण *res);
  uniqstr_assert (tag);

  /* If the tag is not a string (starts with a double quote), check
     that it is valid for Yacc. */
  यदि (tag[0] != '\"' && tag[0] != '\'' && म_अक्षर (tag, '-'))
    complain (&loc, Wyacc,
              _("POSIX Yacc forbids dashes in symbol names: %s"), tag);

  res->tag = tag;
  res->location = loc;
  res->translatable = false;
  res->location_of_lhs = false;
  res->alias = रिक्त;
  res->content = sym_content_new (res);
  res->is_alias = false;
  फर्काऊ res;
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
चिह्न_free (शून्य *ptr)
{
  चिह्न *sym = (चिह्न *)ptr;
  यदि (!sym->is_alias)
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
symbols_sort (अचर चिह्न **first, अचर चिह्न **second)
{
  यदि (0 < location_cmp ((*first)->location, (*second)->location))
    {
      अचर चिह्न* tmp = *first;
      *first = *second;
      *second = tmp;
    }
}

/* Likewise, for locations.  */

स्थिर शून्य
locations_sort (location *first, location *second)
{
  यदि (0 < location_cmp (*first, *second))
    {
      location tmp = *first;
      *first = *second;
      *second = tmp;
    }
}

वर्ण अचर *
code_props_type_string (code_props_type kind)
{
  विकल्प (kind)
    {
    पक्ष destructor:
      फर्काऊ "%destructor";
    पक्ष printer:
      फर्काऊ "%printer";
    }
  पात ();
}


/*----------------------------------------.
| Create a new semantic type, named TAG.  |
`----------------------------------------*/

स्थिर semantic_type *
semantic_type_new (uniqstr tag, अचर location *loc)
{
  semantic_type *res = xmalloc (परिमाण *res);

  uniqstr_assert (tag);
  res->tag = tag;
  res->location = loc ? *loc : empty_loc;
  res->status = undeclared;
  लागि (पूर्णाङ्क i = 0; i < CODE_PROPS_SIZE; ++i)
    code_props_none_init (&res->props[i]);

  फर्काऊ res;
}


/*-----------------.
| Print a symbol.  |
`-----------------*/

#define चिह्न_INT_ATTR_PRINT(Attr)                     \
  यदि (s->content)                                       \
    ख_लिखो (f, " %s = %d", #Attr, s->content->Attr)

#define चिह्न_STR_ATTR_PRINT(Attr)                     \
  यदि (s->content && s->content->Attr)                   \
    ख_लिखो (f, " %s { %s }", #Attr, s->content->Attr)

#define चिह्न_CODE_PRINT(Attr)                                         \
  यदि (s->content && s->content->props[Attr].code)                       \
    ख_लिखो (f, " %s { %s }", #Attr, s->content->props[Attr].code)

शून्य
चिह्न_print (चिह्न अचर *s, खाता *f)
{
  यदि (s)
    {
      चिह्न_class c = s->content->class;
      ख_लिखो (f, "%s: %s",
               c == unknown_sym    ? "unknown"
               : c == pct_type_sym ? "%type"
               : c == token_sym    ? "token"
               : c == nterm_sym    ? "nterm"
               : रिक्त, /* abort.  */
               s->tag);
      अ_दो (' ', f);
      location_print (s->location, f);
      चिह्न_INT_ATTR_PRINT (code);
      चिह्न_INT_ATTR_PRINT (सङ्ख्या);
      चिह्न_STR_ATTR_PRINT (type_name);
      चिह्न_CODE_PRINT (destructor);
      चिह्न_CODE_PRINT (printer);
    }
  अन्यथा
    ख_माला_दो ("<NULL>", f);
}

#undef चिह्न_ATTR_PRINT
#undef चिह्न_CODE_PRINT


/*----------------------------------.
| Whether S is a valid identifier.  |
`----------------------------------*/

स्थिर bool
is_identifier (uniqstr s)
{
  स्थिर वर्ण अचर alphanum[26 + 26 + 1 + 10] =
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "_"
    "0123456789";
  यदि (!s || ! स_प्रथम (alphanum, *s, परिमाण alphanum - 10))
    फर्काऊ false;
  लागि (++s; *s; ++s)
    यदि (! स_प्रथम (alphanum, *s, परिमाण alphanum))
      फर्काऊ false;
  फर्काऊ true;
}


/*-----------------------------------------------.
| Get the identifier associated to this symbol.  |
`-----------------------------------------------*/

uniqstr
चिह्न_id_get (चिह्न अचर *sym)
{
  // There's one weird case: YYerror is the alias, and error is the
  // base symbol.  Return YYerror in that case.
  यदि (sym->alias && is_identifier (sym->alias->tag))
    फर्काऊ sym->alias->tag;
  अन्यथा यदि (is_identifier (sym->tag))
    फर्काऊ sym->tag;
  अन्यथा
    फर्काऊ रिक्त;
}


/*------------------------------------------------------------------.
| Complain that S's WHAT is redeclared at SECOND, and was first set |
| at FIRST.                                                         |
`------------------------------------------------------------------*/

स्थिर शून्य
complain_चिह्न_redeclared (चिह्न *s, अचर वर्ण *what, location first,
                            location second)
{
  locations_sort (&first, &second);
  complain (&second, complaint, _("%s redeclaration for %s"), what, s->tag);
  subcomplain (&first, complaint, _("previous declaration"));
}

स्थिर शून्य
complain_semantic_type_redeclared (semantic_type *s, अचर वर्ण *what, location first,
                                   location second)
{
  locations_sort (&first, &second);
  complain (&second, complaint, _("%s redeclaration for <%s>"), what, s->tag);
  subcomplain (&first, complaint, _("previous declaration"));
}

स्थिर शून्य
complain_class_redeclared (चिह्न *sym, चिह्न_class class, location second)
{
  complain (&second, complaint,
            class == token_sym
            ? _("symbol %s redeclared as a token")
            : _("symbol %s redeclared as a nonterminal"), sym->tag);
  यदि (!location_empty (sym->location))
    subcomplain (&sym->location, complaint, _("previous definition"));
}

स्थिर अचर चिह्न *
चिह्न_from_uniqstr_fuzzy (अचर uniqstr साँचो)
{
  aver (symbols_sorted);
#define FSTRCMP_THRESHOLD 0.6
  द्विगुण best_similarity = FSTRCMP_THRESHOLD;
  अचर चिह्न *res = रिक्त;
  माप_प्रकार गन्ती = hash_get_n_entries (चिह्न_table);
  लागि (पूर्णाङ्क i = 0; i < गन्ती; ++i)
    {
      चिह्न *sym = symbols_sorted[i];
      यदि (STRNEQ (साँचो, sym->tag)
          && (sym->content->status == declared
              || sym->content->status == undeclared))
        {
          द्विगुण similarity = fstrcmp_bounded (साँचो, sym->tag, best_similarity);
          यदि (best_similarity < similarity)
            {
              res = sym;
              best_similarity = similarity;
            }
        }
    }
  फर्काऊ res;
}

स्थिर शून्य
complain_चिह्न_undeclared (अचर चिह्न *sym)
{
  प्रतिज्ञा (sym->content->status != declared);
  अचर चिह्न *best = चिह्न_from_uniqstr_fuzzy (sym->tag);
  यदि (best)
    {
      complain (&sym->location,
                sym->content->status == needed ? complaint : Wother,
                _("symbol %s is used, but is not defined as a token"
                  " and has no rules; did you mean %s?"),
                quote_n (0, sym->tag),
                quote_n (1, best->tag));
      यदि (feature_flag & feature_caret)
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
चिह्न_location_as_lhs_set (चिह्न *sym, location loc)
{
  यदि (!sym->location_of_lhs)
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
चिह्न_type_set (चिह्न *sym, uniqstr type_name, location loc)
{
  यदि (type_name)
    {
      tag_seen = true;
      यदि (sym->content->type_name)
        complain_चिह्न_redeclared (sym, "%type",
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
चिह्न_code_props_set (चिह्न *sym, code_props_type kind,
                       code_props अचर *code)
{
  यदि (sym->content->props[kind].code)
    complain_चिह्न_redeclared (sym, code_props_type_string (kind),
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
                              code_props अचर *code)
{
  यदि (type->props[kind].code)
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
चिह्न_code_props_get (चिह्न *sym, code_props_type kind)
{
  /* Per-symbol code props.  */
  यदि (sym->content->props[kind].code)
    फर्काऊ &sym->content->props[kind];

  /* Per-type code props.  */
  यदि (sym->content->type_name)
    {
      code_props *code =
        &semantic_type_get (sym->content->type_name, रिक्त)->props[kind];
      यदि (code->code)
        फर्काऊ code;
    }

  /* Apply default code props's only to user-defined symbols.  */
  यदि (चिह्न_is_user_defined (sym))
    {
      code_props *code = &semantic_type_get (sym->content->type_name ? "*" : "",
                                             रिक्त)->props[kind];
      यदि (code->code)
        फर्काऊ code;
    }
  फर्काऊ &code_props_none;
}

/*-----------------------------------------------------------------.
| Set the PRECEDENCE associated with SYM.  Does nothing if invoked |
| with UNDEF_ASSOC as ASSOC.                                       |
`-----------------------------------------------------------------*/

शून्य
चिह्न_precedence_set (चिह्न *sym, पूर्णाङ्क prec, assoc a, location loc)
{
  यदि (a != undef_assoc)
    {
      sym_content *s = sym->content;
      यदि (s->prec)
        complain_चिह्न_redeclared (sym, assoc_to_string (a),
                                    s->prec_loc, loc);
      अन्यथा
        {
          s->prec = prec;
          s->assoc = a;
          s->prec_loc = loc;
        }
    }

  /* Only terminals have a precedence. */
  चिह्न_class_set (sym, token_sym, loc, false);
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
चिह्न_class_set (चिह्न *sym, चिह्न_class class, location loc, bool declaring)
{
  aver (class != unknown_sym);
  sym_content *s = sym->content;
  यदि (class == pct_type_sym)
    {
      यदि (s->class == token_sym)
        complain_pct_type_on_token (&loc);
      अन्यथा यदि (s->class == unknown_sym)
        s->class = class;
    }
  अन्यथा यदि (s->class != unknown_sym && s->class != pct_type_sym
           && s->class != class)
    complain_class_redeclared (sym, class, loc);
  अन्यथा
    {
      यदि (class == token_sym && s->class == pct_type_sym)
        complain_pct_type_on_token (&sym->location);

      s->class = class;

      यदि (declaring)
        {
          यदि (s->status == declared)
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
चिह्न_code_set (चिह्न *sym, पूर्णाङ्क code, location loc)
{
  पूर्णाङ्क *codep = &sym->content->code;
  यदि (sym->content->class != token_sym)
    complain (&loc, complaint,
              _("nonterminals cannot be given a token code"));
  अन्यथा यदि (*codep != CODE_UNDEFINED
           && *codep != code)
    complain (&loc, complaint, _("redefining code of token %s"),
              sym->tag);
  अन्यथा यदि (code == पूर्णांक_उच्च)
    complain (&loc, complaint, _("code of token %s too large"),
              sym->tag);
  अन्यथा
    {
      *codep = code;
      /* User defined $end token? */
      यदि (code == 0 && !eoftoken)
        {
          eoftoken = sym->content->चिह्न;
          eoftoken->content->सङ्ख्या = 0;
        }
    }
}


/*----------------------------------------------------------.
| If SYM is not defined, report an error, and consider it a |
| nonterminal.                                              |
`----------------------------------------------------------*/

स्थिर शून्य
चिह्न_check_defined (चिह्न *sym)
{
  sym_content *s = sym->content;
  यदि (s->class == unknown_sym || s->class == pct_type_sym)
    {
      complain_चिह्न_undeclared (sym);
      s->class = nterm_sym;
    }

  यदि (s->सङ्ख्या == सङ्ख्या_UNDEFINED)
    s->सङ्ख्या = s->class == token_sym ? ntokens++ : nnterms++;

  यदि (s->class == token_sym
      && sym->tag[0] == '"'
      && !sym->is_alias)
    complain (&sym->location, Wdangling_alias,
              _("string literal %s not attached to a symbol"),
              sym->tag);

  लागि (पूर्णाङ्क i = 0; i < 2; ++i)
    चिह्न_code_props_get (sym, i)->is_used = true;

  /* Set the semantic type status associated to the current symbol to
     'declared' so that we could check semantic types unnecessary uses. */
  यदि (s->type_name)
    {
      semantic_type *sem_type = semantic_type_get (s->type_name, रिक्त);
      यदि (sem_type)
        sem_type->status = declared;
    }
}

स्थिर शून्य
semantic_type_check_defined (semantic_type *sem_type)
{
  /* <*> and <> do not have to be "declared".  */
  यदि (sem_type->status == declared
      || !*sem_type->tag
      || STREQ (sem_type->tag, "*"))
    {
      लागि (पूर्णाङ्क i = 0; i < 2; ++i)
        यदि (sem_type->props[i].kind != CODE_PROPS_NONE
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
चिह्न_merge_properties (चिह्न *sym, चिह्न *str)
{
  यदि (str->content->type_name != sym->content->type_name)
    {
      यदि (str->content->type_name)
        चिह्न_type_set (sym,
                         str->content->type_name, str->content->type_loc);
      अन्यथा
        चिह्न_type_set (str,
                         sym->content->type_name, sym->content->type_loc);
    }


  लागि (पूर्णाङ्क i = 0; i < CODE_PROPS_SIZE; ++i)
    यदि (str->content->props[i].code)
      चिह्न_code_props_set (sym, i, &str->content->props[i]);
    अन्यथा यदि (sym->content->props[i].code)
      चिह्न_code_props_set (str, i, &sym->content->props[i]);

  यदि (sym->content->prec || str->content->prec)
    {
      यदि (str->content->prec)
        चिह्न_precedence_set (sym, str->content->prec, str->content->assoc,
                               str->content->prec_loc);
      अन्यथा
        चिह्न_precedence_set (str, sym->content->prec, sym->content->assoc,
                               sym->content->prec_loc);
    }
}

शून्य
चिह्न_make_alias (चिह्न *sym, चिह्न *str, location loc)
{
  यदि (sym->content->class != token_sym)
    complain (&loc, complaint,
              _("nonterminals cannot be given a string alias"));
  अन्यथा यदि (str->alias)
    complain (&loc, Wother,
              _("symbol %s used more than once as a literal string"), str->tag);
  अन्यथा यदि (sym->alias)
    complain (&loc, Wother,
              _("symbol %s given more than one literal string"), sym->tag);
  अन्यथा
    {
      चिह्न_merge_properties (sym, str);
      sym_content_free (str->content);
      str->content = sym->content;
      str->content->चिह्न = str;
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
चिह्न_pack (चिह्न *sym)
{
  aver (sym->content->सङ्ख्या != सङ्ख्या_UNDEFINED);
  यदि (sym->content->class == nterm_sym)
    sym->content->सङ्ख्या += ntokens;

  symbols[sym->content->सङ्ख्या] = sym->content->चिह्न;
}

स्थिर शून्य
complain_code_redeclared (पूर्णाङ्क अङ्क, अचर चिह्न *first, अचर चिह्न *second)
{
  symbols_sort (&first, &second);
  complain (&second->location, complaint,
            _("code %d reassigned to token %s"),
            अङ्क, second->tag);
  subcomplain (&first->location, complaint,
               _("previous declaration for %s"),
               first->tag);
}

/*-------------------------------------------------.
| Put SYM in TOKEN_TRANSLATIONS if it is a token.  |
`-------------------------------------------------*/

स्थिर शून्य
चिह्न_translation (अचर चिह्न *sym)
{
  यदि (sym->content->class == token_sym && !sym->is_alias)
    {
      /* A token whose translation has already been set? */
      यदि (token_translations[sym->content->code]
          != undeftoken->content->सङ्ख्या)
        complain_code_redeclared
          (sym->content->code,
           symbols[token_translations[sym->content->code]], sym);
      अन्यथा
        token_translations[sym->content->code]
          = sym->content->सङ्ख्या;
    }
}


/*---------------------------------------.
| Symbol and semantic type hash tables.  |
`---------------------------------------*/

/* Initial capacity of symbol and semantic type hash table.  */
#define HT_INITIAL_CAPACITY 257

स्थिर अन्तःस्थ bool
hash_compare_चिह्न (अचर चिह्न *m1, अचर चिह्न *m2)
{
  /* Since tags are unique, we can compare the pointers themselves.  */
  फर्काऊ UNIQSTR_EQ (m1->tag, m2->tag);
}

स्थिर अन्तःस्थ bool
hash_compare_semantic_type (अचर semantic_type *m1, अचर semantic_type *m2)
{
  /* Since names are unique, we can compare the pointers themselves.  */
  फर्काऊ UNIQSTR_EQ (m1->tag, m2->tag);
}

स्थिर bool
hash_चिह्न_comparator (शून्य अचर *m1, शून्य अचर *m2)
{
  फर्काऊ hash_compare_चिह्न (m1, m2);
}

स्थिर bool
hash_semantic_type_comparator (शून्य अचर *m1, शून्य अचर *m2)
{
  फर्काऊ hash_compare_semantic_type (m1, m2);
}

स्थिर अन्तःस्थ माप_प्रकार
hash_चिह्न (अचर चिह्न *m, माप_प्रकार tablesize)
{
  /* Since tags are unique, we can hash the pointer itself.  */
  फर्काऊ ((uintptr_t) m->tag) % tablesize;
}

स्थिर अन्तःस्थ माप_प्रकार
hash_semantic_type (अचर semantic_type *m, माप_प्रकार tablesize)
{
  /* Since names are unique, we can hash the pointer itself.  */
  फर्काऊ ((uintptr_t) m->tag) % tablesize;
}

स्थिर माप_प्रकार
hash_चिह्न_hasher (शून्य अचर *m, माप_प्रकार tablesize)
{
  फर्काऊ hash_चिह्न (m, tablesize);
}

स्थिर माप_प्रकार
hash_semantic_type_hasher (शून्य अचर *m, माप_प्रकार tablesize)
{
  फर्काऊ hash_semantic_type (m, tablesize);
}

/*-------------------------------.
| Create the symbol hash table.  |
`-------------------------------*/

शून्य
symbols_new (शून्य)
{
  चिह्न_table = hash_xinitialize (HT_INITIAL_CAPACITY,
                                   रिक्त,
                                   hash_चिह्न_hasher,
                                   hash_चिह्न_comparator,
                                   चिह्न_free);

  /* Construct the acceptsymbol symbol. */
  acceptsymbol = चिह्न_get ("$accept", empty_loc);
  acceptsymbol->content->class = nterm_sym;
  acceptsymbol->content->सङ्ख्या = nnterms++;

  /* Construct the YYerror/"error" token */
  errtoken = चिह्न_get ("YYerror", empty_loc);
  errtoken->content->class = token_sym;
  errtoken->content->सङ्ख्या = ntokens++;
  {
    चिह्न *alias = चिह्न_get ("error", empty_loc);
    चिह्न_class_set (alias, token_sym, empty_loc, false);
    चिह्न_make_alias (errtoken, alias, empty_loc);
  }

  /* Construct the YYUNDEF/"$undefined" token that represents all
     undefined literal tokens.  It is always symbol number 2.  */
  undeftoken = चिह्न_get ("YYUNDEF", empty_loc);
  undeftoken->content->class = token_sym;
  undeftoken->content->सङ्ख्या = ntokens++;
  {
    चिह्न *alias = चिह्न_get ("$undefined", empty_loc);
    चिह्न_class_set (alias, token_sym, empty_loc, false);
    चिह्न_make_alias (undeftoken, alias, empty_loc);
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

चिह्न *
चिह्न_from_uniqstr (अचर uniqstr साँचो, location loc)
{
  चिह्न probe;

  probe.tag = साँचो;
  चिह्न *res = hash_lookup (चिह्न_table, &probe);

  यदि (!res)
    {
      /* First insertion in the hash. */
      aver (!symbols_sorted);
      res = चिह्न_new (साँचो, loc);
      hash_xinsert (चिह्न_table, res);
    }
  फर्काऊ res;
}


/*-----------------------------------------------------------------------.
| Find the semantic type named KEY, and return it.  If it does not exist |
| yet, create it.                                                        |
`-----------------------------------------------------------------------*/

semantic_type *
semantic_type_from_uniqstr (अचर uniqstr साँचो, अचर location *loc)
{
  semantic_type probe;

  probe.tag = साँचो;
  semantic_type *res = hash_lookup (semantic_type_table, &probe);

  यदि (!res)
    {
      /* First insertion in the hash. */
      res = semantic_type_new (साँचो, loc);
      hash_xinsert (semantic_type_table, res);
    }
  फर्काऊ res;
}


/*----------------------------------------------------------------.
| Find the symbol named KEY, and return it.  If it does not exist |
| yet, create it.                                                 |
`----------------------------------------------------------------*/

चिह्न *
चिह्न_get (अचर वर्ण *साँचो, location loc)
{
  फर्काऊ चिह्न_from_uniqstr (uniqstr_new (साँचो), loc);
}


/*-----------------------------------------------------------------------.
| Find the semantic type named KEY, and return it.  If it does not exist |
| yet, create it.                                                        |
`-----------------------------------------------------------------------*/

semantic_type *
semantic_type_get (अचर वर्ण *साँचो, अचर location *loc)
{
  फर्काऊ semantic_type_from_uniqstr (uniqstr_new (साँचो), loc);
}


/*------------------------------------------------------------------.
| Generate a dummy nonterminal, whose name cannot conflict with the |
| user's names.                                                     |
`------------------------------------------------------------------*/

चिह्न *
dummy_चिह्न_get (location loc)
{
  /* Incremented for each generated symbol.  */
  स्थिर पूर्णाङ्क dummy_गन्ती = 0;
  वर्ण buf[32];
  पूर्णाङ्क len = snprintf (buf, परिमाण buf, "$@%d", ++dummy_गन्ती);
  assure (len < परिमाण buf);
  चिह्न *sym = चिह्न_get (buf, loc);
  sym->content->class = nterm_sym;
  फर्काऊ sym;
}

bool
चिह्न_is_dummy (चिह्न अचर *sym)
{
  फर्काऊ sym->tag[0] == '@' || (sym->tag[0] == '$' && sym->tag[1] == '@');
}

/*-------------------.
| Free the symbols.  |
`-------------------*/

शून्य
symbols_free (शून्य)
{
  hash_free (चिह्न_table);
  hash_free (semantic_type_table);
  मुक्त (symbols);
  मुक्त (symbols_sorted);
  मुक्त (semantic_types_sorted);
}


स्थिर पूर्णाङ्क
चिह्न_cmp (शून्य अचर *a, शून्य अचर *b)
{
  फर्काऊ location_cmp ((*(चिह्न * अचर *)a)->location,
                       (*(चिह्न * अचर *)b)->location);
}

/* Store in *SORTED an array of pointers to the symbols contained in
   TABLE, sorted by order of appearance (i.e., by location). */

स्थिर शून्य
table_sort (संरचना hash_table *table, चिह्न ***sorted)
{
  aver (!*sorted);
  माप_प्रकार गन्ती = hash_get_n_entries (table);
  *sorted = xnmalloc (गन्ती + 1, परिमाण **sorted);
  hash_get_entries (table, (शून्य**)*sorted, गन्ती);
  क्विक (*sorted, गन्ती, परिमाण **sorted, चिह्न_cmp);
  (*sorted)[गन्ती] = रिक्त;
}

/*--------------------------------------------------------------.
| Check that all the symbols are defined.  Report any undefined |
| symbols and consider them nonterminals.                       |
`--------------------------------------------------------------*/

शून्य
symbols_check_defined (शून्य)
{
  table_sort (चिह्न_table, &symbols_sorted);
  /* semantic_type, like symbol, starts with a 'tag' field and then a
     'location' field.  And here we only deal with arrays/hashes of
     pointers, sizeof is not an issue.

     So instead of implementing table_sort (and symbol_cmp) once for
     each type, let's lie a bit to the typing system, and treat
     'semantic_type' as if it were 'symbol'. */
  table_sort (semantic_type_table, (चिह्न ***) &semantic_types_sorted);

  लागि (पूर्णाङ्क i = 0; symbols_sorted[i]; ++i)
    चिह्न_check_defined (symbols_sorted[i]);
  लागि (पूर्णाङ्क i = 0; semantic_types_sorted[i]; ++i)
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
  लागि (पूर्णाङ्क i = 0; i < ntokens; ++i)
    {
      sym_content *sym = symbols[i]->content;
      यदि (sym->code != CODE_UNDEFINED)
        {
          यदि (sym->code > max_code)
            max_code = sym->code;
          यदि (sym->code == 256)
            code_256_available_p = false;
        }
    }

  /* If 256 is not used, assign it to error, to follow POSIX.  */
  यदि (code_256_available_p
      && errtoken->content->code == CODE_UNDEFINED)
    errtoken->content->code = 256;

  /* Set the missing codes. */
  यदि (max_code < 256)
    max_code = 256;

  लागि (पूर्णाङ्क i = 0; i < ntokens; ++i)
    {
      sym_content *sym = symbols[i]->content;
      यदि (sym->code == CODE_UNDEFINED)
        {
          IGNORE_TYPE_LIMITS_BEGIN
          यदि (INT_ADD_WRAPV (max_code, 1, &max_code))
            complain (रिक्त, fatal, _("token number too large"));
          IGNORE_TYPE_LIMITS_END
          sym->code = max_code;
        }
      यदि (sym->code > max_code)
        max_code = sym->code;
    }

  token_translations = xnmalloc (max_code + 1,
                                 परिमाण *token_translations);

  /* Initialize all entries for literal tokens to the internal token
     number for $undefined, which represents all invalid inputs.  */
  लागि (पूर्णाङ्क i = 0; i < max_code + 1; ++i)
    token_translations[i] = undeftoken->content->सङ्ख्या;
  लागि (पूर्णाङ्क i = 0; symbols_sorted[i]; ++i)
    चिह्न_translation (symbols_sorted[i]);
}


/* Whether some symbol requires internationalization.  */
स्थिर bool
has_translations (शून्य)
{
  लागि (अचर शून्य *entry = hash_get_first (चिह्न_table);
       entry;
       entry = hash_get_next (चिह्न_table, entry))
    {
      अचर चिह्न *sym = (अचर चिह्न *) entry;
      यदि (sym->translatable)
        फर्काऊ true;
    }
  फर्काऊ false;
}

/*----------------------------------------------------------------.
| Assign symbol numbers, and write definition of token names into |
| FDEFINES.  Set up vectors SYMBOL_TABLE, TAGS of symbols.        |
`----------------------------------------------------------------*/

शून्य
symbols_pack (शून्य)
{
  symbols = xcalloc (nsyms, परिमाण *symbols);
  लागि (पूर्णाङ्क i = 0; symbols_sorted[i]; ++i)
    चिह्न_pack (symbols_sorted[i]);

  /* Aliases leave empty slots in symbols, so remove them.  */
  {
    पूर्णाङ्क nsyms_old = nsyms;
    लागि (पूर्णाङ्क writei = 0, readi = 0; readi < nsyms_old; readi += 1)
      {
        यदि (symbols[readi] == रिक्त)
          {
            nsyms -= 1;
            ntokens -= 1;
          }
        अन्यथा
          {
            symbols[writei] = symbols[readi];
            symbols[writei]->content->सङ्ख्या = writei;
            writei += 1;
          }
      }
  }
  symbols = xnrealloc (symbols, nsyms, परिमाण *symbols);

  symbols_token_translations_init ();

  // If some user tokens are internationalized, the internal ones
  // should be too.
  यदि (has_translations ())
    {
      अचर bool eof_is_user_defined
        = !eoftoken->alias || STRNEQ (eoftoken->alias->tag, "$end");
      यदि (!eof_is_user_defined)
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
  prec_nodes = xcalloc (nsyms, परिमाण *prec_nodes);
  लागि (पूर्णाङ्क i = 0; i < nsyms; ++i)
    {
      prec_nodes[i] = xmalloc (परिमाण *prec_nodes[i]);
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
  symgraphlink *res = xmalloc (परिमाण *res);
  res->id = id;
  res->next = next;
  फर्काऊ res;
}


/*------------------------------------------------------------------.
| Register the second symbol of the precedence relation, and return |
| whether this relation is new.  Use only in register_precedence.   |
`------------------------------------------------------------------*/

स्थिर bool
register_precedence_second_चिह्न (symgraphlink **first, graphid sym)
{
  यदि (!*first || sym < (*first)->id)
    *first = symgraphlink_new (sym, *first);
  अन्यथा
    {
      symgraphlink *slist = *first;

      जबसम्म (slist->next && slist->next->id <= sym)
        slist = slist->next;

      यदि (slist->id == sym)
        /* Relation already present. */
        फर्काऊ false;

      slist->next = symgraphlink_new (sym, slist->next);
    }
  फर्काऊ true;
}

/*------------------------------------------------------------------.
| Register a new relation between symbols as used. The first symbol |
| has a greater precedence than the second one.                     |
`------------------------------------------------------------------*/

शून्य
register_precedence (graphid first, graphid snd)
{
  यदि (!prec_nodes)
    init_prec_nodes ();
  register_precedence_second_चिह्न (&(prec_nodes[first]->succ), snd);
  register_precedence_second_चिह्न (&(prec_nodes[snd]->pred), first);
}


/*---------------------------------------.
| Deep clear a linked / adjacency list). |
`---------------------------------------*/

स्थिर शून्य
linkedlist_free (symgraphlink *गाँठो)
{
  यदि (गाँठो)
    {
      जबसम्म (गाँठो->next)
        {
          symgraphlink *tmp = गाँठो->next;
          मुक्त (गाँठो);
          गाँठो = tmp;
        }
      मुक्त (गाँठो);
    }
}

/*----------------------------------------------.
| Clear and destroy association tracking table. |
`----------------------------------------------*/

स्थिर शून्य
assoc_free (शून्य)
{
  लागि (पूर्णाङ्क i = 0; i < nsyms; ++i)
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
  used_assoc = xcalloc (nsyms, परिमाण *used_assoc);
  लागि (graphid i = 0; i < nsyms; ++i)
    used_assoc[i] = false;
}

/*------------------------------------------------------------------.
| Test if the associativity for the symbols is defined and useless. |
`------------------------------------------------------------------*/

स्थिर अन्तःस्थ bool
is_assoc_useless (चिह्न *s)
{
  फर्काऊ s
      && s->content->assoc != undef_assoc
      && s->content->assoc != precedence_assoc
      && !used_assoc[s->content->सङ्ख्या];
}

/*-------------------------------.
| Register a used associativity. |
`-------------------------------*/

शून्य
register_assoc (graphid i, graphid j)
{
  यदि (!used_assoc)
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
  यदि (!prec_nodes)
    init_prec_nodes ();
  यदि (!used_assoc)
    init_assoc ();
  लागि (पूर्णाङ्क i = 0; i < nsyms; ++i)
    {
      चिह्न *s = symbols[i];
      यदि (s
          && s->content->prec != 0
          && !prec_nodes[i]->pred
          && !prec_nodes[i]->succ)
        {
          यदि (is_assoc_useless (s))
            complain (&s->content->prec_loc, Wprecedence,
                      _("useless precedence and associativity for %s"), s->tag);
          अन्यथा यदि (s->content->assoc == precedence_assoc)
            complain (&s->content->prec_loc, Wprecedence,
                      _("useless precedence for %s"), s->tag);
        }
      अन्यथा यदि (is_assoc_useless (s))
        complain (&s->content->prec_loc, Wprecedence,
                  _("useless associativity for %s, use %%precedence"), s->tag);
    }
  मुक्त (used_assoc);
  assoc_free ();
}
