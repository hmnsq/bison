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


स्थिर संरचना hash_table *प्रतीक_table = रिक्त;
स्थिर संरचना hash_table *semantic_type_table = रिक्त;

/*----------------------------------------------------------------.
| Symbols sorted by tag.  Allocated by table_sort, after which no |
| more symbols should be created.                                 |
`----------------------------------------------------------------*/

स्थिर प्रतीक **symbols_sorted = रिक्त;
स्थिर semantic_type **semantic_types_sorted = रिक्त;


/*------------------------.
| Distinguished symbols.  |
`------------------------*/

प्रतीक *errtoken = रिक्त;
प्रतीक *undeftoken = रिक्त;
प्रतीक *eoftoken = रिक्त;
प्रतीक *acceptsymbol = रिक्त;

/* Precedence relation graph. */
स्थिर symgraph **prec_nodes;

/* Store which associativity is used.  */
स्थिर bool *used_assoc = रिक्त;

bool tag_seen = false;


/* Whether SYM was defined by the user.  */

स्थिर bool
प्रतीक_is_user_defined (प्रतीक *sym)
{
  ध्रुव bool eof_is_user_defined
    = !eoftoken->alias || STRNEQ (eoftoken->alias->tag, "$end");
  प्रत्यागम sym->tag[0] != '$'
    && (eof_is_user_defined || (sym != eoftoken && sym->alias != errtoken))
    && sym != errtoken && sym->alias != errtoken
    && sym != undeftoken && sym->alias != undeftoken;
}


/*--------------------------.
| Create a new sym_content. |
`--------------------------*/

स्थिर sym_content *
sym_content_new (प्रतीक *s)
{
  sym_content *res = xmalloc (परिमाण *res);

  res->प्रतीक = s;

  res->type_name = रिक्त;
  res->type_loc = empty_loc;
  प्रति (पूर्णाङ्क i = 0; i < CODE_PROPS_SIZE; ++i)
    code_props_none_init (&res->props[i]);

  res->संख्या = संख्या_UNDEFINED;
  res->prec_loc = empty_loc;
  res->prec = 0;
  res->assoc = undef_assoc;
  res->code = CODE_UNDEFINED;

  res->class = unknown_sym;
  res->status = undeclared;

  प्रत्यागम res;
}

/*---------------------------------.
| Create a new symbol, named TAG.  |
`---------------------------------*/

स्थिर प्रतीक *
प्रतीक_new (uniqstr tag, location loc)
{
  प्रतीक *res = xmalloc (परिमाण *res);
  uniqstr_assert (tag);

  /* If the tag is not a string (starts with a double quote), check
     that it is valid for Yacc. */
  यदि (tag[0] != '\"' && tag[0] != '\'' && strchr (tag, '-'))
    complain (&loc, Wyacc,
              _("POSIX Yacc forbids dashes in symbol names: %s"), tag);

  res->tag = tag;
  res->location = loc;
  res->translatable = false;
  res->location_of_lhs = false;
  res->alias = रिक्त;
  res->content = sym_content_new (res);
  res->is_alias = false;
  प्रत्यागम res;
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
प्रतीक_free (शून्य *ptr)
{
  प्रतीक *sym = (प्रतीक *)ptr;
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
symbols_sort (ध्रुव प्रतीक **first, ध्रुव प्रतीक **second)
{
  यदि (0 < location_cmp ((*first)->location, (*second)->location))
    {
      ध्रुव प्रतीक* tmp = *first;
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

वर्ण ध्रुव *
code_props_type_string (code_props_type kind)
{
  विकल्प (kind)
    {
    पक्ष destructor:
      प्रत्यागम "%destructor";
    पक्ष printer:
      प्रत्यागम "%printer";
    }
  abort ();
}


/*----------------------------------------.
| Create a new semantic type, named TAG.  |
`----------------------------------------*/

स्थिर semantic_type *
semantic_type_new (uniqstr tag, ध्रुव location *loc)
{
  semantic_type *res = xmalloc (परिमाण *res);

  uniqstr_assert (tag);
  res->tag = tag;
  res->location = loc ? *loc : empty_loc;
  res->status = undeclared;
  प्रति (पूर्णाङ्क i = 0; i < CODE_PROPS_SIZE; ++i)
    code_props_none_init (&res->props[i]);

  प्रत्यागम res;
}


/*-----------------.
| Print a symbol.  |
`-----------------*/

#define प्रतीक_INT_ATTR_PRINT(Attr)                     \
  यदि (s->content)                                       \
    fprintf (f, " %s = %d", #Attr, s->content->Attr)

#define प्रतीक_STR_ATTR_PRINT(Attr)                     \
  यदि (s->content && s->content->Attr)                   \
    fprintf (f, " %s { %s }", #Attr, s->content->Attr)

#define प्रतीक_CODE_PRINT(Attr)                                         \
  यदि (s->content && s->content->props[Attr].code)                       \
    fprintf (f, " %s { %s }", #Attr, s->content->props[Attr].code)

शून्य
प्रतीक_print (प्रतीक ध्रुव *s, FILE *f)
{
  यदि (s)
    {
      प्रतीक_class c = s->content->class;
      fprintf (f, "%s: %s",
               c == unknown_sym    ? "unknown"
               : c == pct_type_sym ? "%type"
               : c == token_sym    ? "token"
               : c == nterm_sym    ? "nterm"
               : रिक्त, /* abort.  */
               s->tag);
      putc (' ', f);
      location_print (s->location, f);
      प्रतीक_INT_ATTR_PRINT (code);
      प्रतीक_INT_ATTR_PRINT (संख्या);
      प्रतीक_STR_ATTR_PRINT (type_name);
      प्रतीक_CODE_PRINT (destructor);
      प्रतीक_CODE_PRINT (printer);
    }
  अन्यथा
    fputs ("<NULL>", f);
}

#undef प्रतीक_ATTR_PRINT
#undef प्रतीक_CODE_PRINT


/*----------------------------------.
| Whether S is a valid identifier.  |
`----------------------------------*/

स्थिर bool
is_identifier (uniqstr s)
{
  स्थिर वर्ण ध्रुव alphanum[26 + 26 + 1 + 10] =
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "_"
    "0123456789";
  यदि (!s || ! memchr (alphanum, *s, परिमाण alphanum - 10))
    प्रत्यागम false;
  प्रति (++s; *s; ++s)
    यदि (! memchr (alphanum, *s, परिमाण alphanum))
      प्रत्यागम false;
  प्रत्यागम true;
}


/*-----------------------------------------------.
| Get the identifier associated to this symbol.  |
`-----------------------------------------------*/

uniqstr
प्रतीक_id_get (प्रतीक ध्रुव *sym)
{
  // There's one weird case: YYerror is the alias, and error is the
  // base symbol.  Return YYerror in that case.
  यदि (sym->alias && is_identifier (sym->alias->tag))
    प्रत्यागम sym->alias->tag;
  अन्यथा यदि (is_identifier (sym->tag))
    प्रत्यागम sym->tag;
  अन्यथा
    प्रत्यागम रिक्त;
}


/*------------------------------------------------------------------.
| Complain that S's WHAT is redeclared at SECOND, and was first set |
| at FIRST.                                                         |
`------------------------------------------------------------------*/

स्थिर शून्य
complain_प्रतीक_redeclared (प्रतीक *s, ध्रुव वर्ण *what, location first,
                            location second)
{
  locations_sort (&first, &second);
  complain (&second, complaint, _("%s redeclaration for %s"), what, s->tag);
  subcomplain (&first, complaint, _("previous declaration"));
}

स्थिर शून्य
complain_semantic_type_redeclared (semantic_type *s, ध्रुव वर्ण *what, location first,
                                   location second)
{
  locations_sort (&first, &second);
  complain (&second, complaint, _("%s redeclaration for <%s>"), what, s->tag);
  subcomplain (&first, complaint, _("previous declaration"));
}

स्थिर शून्य
complain_class_redeclared (प्रतीक *sym, प्रतीक_class class, location second)
{
  complain (&second, complaint,
            class == token_sym
            ? _("symbol %s redeclared as a token")
            : _("symbol %s redeclared as a nonterminal"), sym->tag);
  यदि (!location_empty (sym->location))
    subcomplain (&sym->location, complaint, _("previous definition"));
}

स्थिर ध्रुव प्रतीक *
प्रतीक_from_uniqstr_fuzzy (ध्रुव uniqstr कुंजी)
{
  aver (symbols_sorted);
#define FSTRCMP_THRESHOLD 0.6
  द्विगुण best_similarity = FSTRCMP_THRESHOLD;
  ध्रुव प्रतीक *res = रिक्त;
  size_t गिनती = hash_get_n_entries (प्रतीक_table);
  प्रति (पूर्णाङ्क i = 0; i < गिनती; ++i)
    {
      प्रतीक *sym = symbols_sorted[i];
      यदि (STRNEQ (कुंजी, sym->tag)
          && (sym->content->status == declared
              || sym->content->status == undeclared))
        {
          द्विगुण similarity = fstrcmp_bounded (कुंजी, sym->tag, best_similarity);
          यदि (best_similarity < similarity)
            {
              res = sym;
              best_similarity = similarity;
            }
        }
    }
  प्रत्यागम res;
}

स्थिर शून्य
complain_प्रतीक_undeclared (ध्रुव प्रतीक *sym)
{
  प्रतिज्ञा (sym->content->status != declared);
  ध्रुव प्रतीक *best = प्रतीक_from_uniqstr_fuzzy (sym->tag);
  यदि (best)
    {
      complain (&sym->location,
                sym->content->status == needed ? complaint : Wother,
                _("symbol %s is used, but is not defined as a token"
                  " and has no rules; did you mean %s?"),
                quote_n (0, sym->tag),
                quote_n (1, best->tag));
      यदि (feature_flag & feature_caret)
        location_caret_suggestion (sym->location, best->tag, stderr);
    }
  अन्यथा
    complain (&sym->location,
              sym->content->status == needed ? complaint : Wother,
              _("symbol %s is used, but is not defined as a token"
                " and has no rules"),
              quote (sym->tag));
}

शून्य
प्रतीक_location_as_lhs_set (प्रतीक *sym, location loc)
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
प्रतीक_type_set (प्रतीक *sym, uniqstr type_name, location loc)
{
  यदि (type_name)
    {
      tag_seen = true;
      यदि (sym->content->type_name)
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

शून्य
प्रतीक_code_props_set (प्रतीक *sym, code_props_type kind,
                       code_props ध्रुव *code)
{
  यदि (sym->content->props[kind].code)
    complain_प्रतीक_redeclared (sym, code_props_type_string (kind),
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
                              code_props ध्रुव *code)
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
प्रतीक_code_props_get (प्रतीक *sym, code_props_type kind)
{
  /* Per-symbol code props.  */
  यदि (sym->content->props[kind].code)
    प्रत्यागम &sym->content->props[kind];

  /* Per-type code props.  */
  यदि (sym->content->type_name)
    {
      code_props *code =
        &semantic_type_get (sym->content->type_name, रिक्त)->props[kind];
      यदि (code->code)
        प्रत्यागम code;
    }

  /* Apply default code props's only to user-defined symbols.  */
  यदि (प्रतीक_is_user_defined (sym))
    {
      code_props *code = &semantic_type_get (sym->content->type_name ? "*" : "",
                                             रिक्त)->props[kind];
      यदि (code->code)
        प्रत्यागम code;
    }
  प्रत्यागम &code_props_none;
}

/*-----------------------------------------------------------------.
| Set the PRECEDENCE associated with SYM.  Does nothing if invoked |
| with UNDEF_ASSOC as ASSOC.                                       |
`-----------------------------------------------------------------*/

शून्य
प्रतीक_precedence_set (प्रतीक *sym, पूर्णाङ्क prec, assoc a, location loc)
{
  यदि (a != undef_assoc)
    {
      sym_content *s = sym->content;
      यदि (s->prec)
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

स्थिर शून्य
complain_pct_type_on_token (location *loc)
{
  complain (loc, Wyacc,
            _("POSIX yacc reserves %%type to nonterminals"));
}

शून्य
प्रतीक_class_set (प्रतीक *sym, प्रतीक_class class, location loc, bool declaring)
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
प्रतीक_code_set (प्रतीक *sym, पूर्णाङ्क code, location loc)
{
  पूर्णाङ्क *codep = &sym->content->code;
  यदि (sym->content->class != token_sym)
    complain (&loc, complaint,
              _("nonterminals cannot be given a token code"));
  अन्यथा यदि (*codep != CODE_UNDEFINED
           && *codep != code)
    complain (&loc, complaint, _("redefining code of token %s"),
              sym->tag);
  अन्यथा यदि (code == INT_MAX)
    complain (&loc, complaint, _("code of token %s too large"),
              sym->tag);
  अन्यथा
    {
      *codep = code;
      /* User defined $end token? */
      यदि (code == 0 && !eoftoken)
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

स्थिर शून्य
प्रतीक_check_defined (प्रतीक *sym)
{
  sym_content *s = sym->content;
  यदि (s->class == unknown_sym || s->class == pct_type_sym)
    {
      complain_प्रतीक_undeclared (sym);
      s->class = nterm_sym;
    }

  यदि (s->संख्या == संख्या_UNDEFINED)
    s->संख्या = s->class == token_sym ? ntokens++ : nnterms++;

  यदि (s->class == token_sym
      && sym->tag[0] == '"'
      && !sym->is_alias)
    complain (&sym->location, Wdangling_alias,
              _("string literal %s not attached to a symbol"),
              sym->tag);

  प्रति (पूर्णाङ्क i = 0; i < 2; ++i)
    प्रतीक_code_props_get (sym, i)->is_used = true;

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
      प्रति (पूर्णाङ्क i = 0; i < 2; ++i)
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
प्रतीक_merge_properties (प्रतीक *sym, प्रतीक *str)
{
  यदि (str->content->type_name != sym->content->type_name)
    {
      यदि (str->content->type_name)
        प्रतीक_type_set (sym,
                         str->content->type_name, str->content->type_loc);
      अन्यथा
        प्रतीक_type_set (str,
                         sym->content->type_name, sym->content->type_loc);
    }


  प्रति (पूर्णाङ्क i = 0; i < CODE_PROPS_SIZE; ++i)
    यदि (str->content->props[i].code)
      प्रतीक_code_props_set (sym, i, &str->content->props[i]);
    अन्यथा यदि (sym->content->props[i].code)
      प्रतीक_code_props_set (str, i, &sym->content->props[i]);

  यदि (sym->content->prec || str->content->prec)
    {
      यदि (str->content->prec)
        प्रतीक_precedence_set (sym, str->content->prec, str->content->assoc,
                               str->content->prec_loc);
      अन्यथा
        प्रतीक_precedence_set (str, sym->content->prec, sym->content->assoc,
                               sym->content->prec_loc);
    }
}

शून्य
प्रतीक_make_alias (प्रतीक *sym, प्रतीक *str, location loc)
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

स्थिर शून्य
प्रतीक_pack (प्रतीक *sym)
{
  aver (sym->content->संख्या != संख्या_UNDEFINED);
  यदि (sym->content->class == nterm_sym)
    sym->content->संख्या += ntokens;

  symbols[sym->content->संख्या] = sym->content->प्रतीक;
}

स्थिर शून्य
complain_code_redeclared (पूर्णाङ्क अंक, ध्रुव प्रतीक *first, ध्रुव प्रतीक *second)
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
प्रतीक_translation (ध्रुव प्रतीक *sym)
{
  यदि (sym->content->class == token_sym && !sym->is_alias)
    {
      /* A token whose translation has already been set? */
      यदि (token_translations[sym->content->code]
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

स्थिर अन्तःस्थ bool
hash_compare_प्रतीक (ध्रुव प्रतीक *m1, ध्रुव प्रतीक *m2)
{
  /* Since tags are unique, we can compare the pointers themselves.  */
  प्रत्यागम UNIQSTR_EQ (m1->tag, m2->tag);
}

स्थिर अन्तःस्थ bool
hash_compare_semantic_type (ध्रुव semantic_type *m1, ध्रुव semantic_type *m2)
{
  /* Since names are unique, we can compare the pointers themselves.  */
  प्रत्यागम UNIQSTR_EQ (m1->tag, m2->tag);
}

स्थिर bool
hash_प्रतीक_comparator (शून्य ध्रुव *m1, शून्य ध्रुव *m2)
{
  प्रत्यागम hash_compare_प्रतीक (m1, m2);
}

स्थिर bool
hash_semantic_type_comparator (शून्य ध्रुव *m1, शून्य ध्रुव *m2)
{
  प्रत्यागम hash_compare_semantic_type (m1, m2);
}

स्थिर अन्तःस्थ size_t
hash_प्रतीक (ध्रुव प्रतीक *m, size_t tablesize)
{
  /* Since tags are unique, we can hash the pointer itself.  */
  प्रत्यागम ((uintptr_t) m->tag) % tablesize;
}

स्थिर अन्तःस्थ size_t
hash_semantic_type (ध्रुव semantic_type *m, size_t tablesize)
{
  /* Since names are unique, we can hash the pointer itself.  */
  प्रत्यागम ((uintptr_t) m->tag) % tablesize;
}

स्थिर size_t
hash_प्रतीक_hasher (शून्य ध्रुव *m, size_t tablesize)
{
  प्रत्यागम hash_प्रतीक (m, tablesize);
}

स्थिर size_t
hash_semantic_type_hasher (शून्य ध्रुव *m, size_t tablesize)
{
  प्रत्यागम hash_semantic_type (m, tablesize);
}

/*-------------------------------.
| Create the symbol hash table.  |
`-------------------------------*/

शून्य
symbols_new (शून्य)
{
  प्रतीक_table = hash_xinitialize (HT_INITIAL_CAPACITY,
                                   रिक्त,
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
                                          रिक्त,
                                          hash_semantic_type_hasher,
                                          hash_semantic_type_comparator,
                                          मुक्त);
}


/*----------------------------------------------------------------.
| Find the symbol named KEY, and return it.  If it does not exist |
| yet, create it.                                                 |
`----------------------------------------------------------------*/

प्रतीक *
प्रतीक_from_uniqstr (ध्रुव uniqstr कुंजी, location loc)
{
  प्रतीक probe;

  probe.tag = कुंजी;
  प्रतीक *res = hash_lookup (प्रतीक_table, &probe);

  यदि (!res)
    {
      /* First insertion in the hash. */
      aver (!symbols_sorted);
      res = प्रतीक_new (कुंजी, loc);
      hash_xinsert (प्रतीक_table, res);
    }
  प्रत्यागम res;
}


/*-----------------------------------------------------------------------.
| Find the semantic type named KEY, and return it.  If it does not exist |
| yet, create it.                                                        |
`-----------------------------------------------------------------------*/

semantic_type *
semantic_type_from_uniqstr (ध्रुव uniqstr कुंजी, ध्रुव location *loc)
{
  semantic_type probe;

  probe.tag = कुंजी;
  semantic_type *res = hash_lookup (semantic_type_table, &probe);

  यदि (!res)
    {
      /* First insertion in the hash. */
      res = semantic_type_new (कुंजी, loc);
      hash_xinsert (semantic_type_table, res);
    }
  प्रत्यागम res;
}


/*----------------------------------------------------------------.
| Find the symbol named KEY, and return it.  If it does not exist |
| yet, create it.                                                 |
`----------------------------------------------------------------*/

प्रतीक *
प्रतीक_get (ध्रुव वर्ण *कुंजी, location loc)
{
  प्रत्यागम प्रतीक_from_uniqstr (uniqstr_new (कुंजी), loc);
}


/*-----------------------------------------------------------------------.
| Find the semantic type named KEY, and return it.  If it does not exist |
| yet, create it.                                                        |
`-----------------------------------------------------------------------*/

semantic_type *
semantic_type_get (ध्रुव वर्ण *कुंजी, ध्रुव location *loc)
{
  प्रत्यागम semantic_type_from_uniqstr (uniqstr_new (कुंजी), loc);
}


/*------------------------------------------------------------------.
| Generate a dummy nonterminal, whose name cannot conflict with the |
| user's names.                                                     |
`------------------------------------------------------------------*/

प्रतीक *
dummy_प्रतीक_get (location loc)
{
  /* Incremented for each generated symbol.  */
  स्थिर पूर्णाङ्क dummy_गिनती = 0;
  वर्ण buf[32];
  पूर्णाङ्क len = snprintf (buf, परिमाण buf, "$@%d", ++dummy_गिनती);
  assure (len < परिमाण buf);
  प्रतीक *sym = प्रतीक_get (buf, loc);
  sym->content->class = nterm_sym;
  प्रत्यागम sym;
}

bool
प्रतीक_is_dummy (प्रतीक ध्रुव *sym)
{
  प्रत्यागम sym->tag[0] == '@' || (sym->tag[0] == '$' && sym->tag[1] == '@');
}

/*-------------------.
| Free the symbols.  |
`-------------------*/

शून्य
symbols_free (शून्य)
{
  hash_free (प्रतीक_table);
  hash_free (semantic_type_table);
  मुक्त (symbols);
  मुक्त (symbols_sorted);
  मुक्त (semantic_types_sorted);
}


स्थिर पूर्णाङ्क
प्रतीक_cmp (शून्य ध्रुव *a, शून्य ध्रुव *b)
{
  प्रत्यागम location_cmp ((*(प्रतीक * ध्रुव *)a)->location,
                       (*(प्रतीक * ध्रुव *)b)->location);
}

/* Store in *SORTED an array of pointers to the symbols contained in
   TABLE, sorted by order of appearance (i.e., by location). */

स्थिर शून्य
table_sort (संरचना hash_table *table, प्रतीक ***sorted)
{
  aver (!*sorted);
  size_t गिनती = hash_get_n_entries (table);
  *sorted = xnmalloc (गिनती + 1, परिमाण **sorted);
  hash_get_entries (table, (शून्य**)*sorted, गिनती);
  qsort (*sorted, गिनती, परिमाण **sorted, प्रतीक_cmp);
  (*sorted)[गिनती] = रिक्त;
}

/*--------------------------------------------------------------.
| Check that all the symbols are defined.  Report any undefined |
| symbols and consider them nonterminals.                       |
`--------------------------------------------------------------*/

शून्य
symbols_check_defined (शून्य)
{
  table_sort (प्रतीक_table, &symbols_sorted);
  /* semantic_type, like symbol, starts with a 'tag' field and then a
     'location' field.  And here we only deal with arrays/hashes of
     pointers, sizeof is not an issue.

     So instead of implementing table_sort (and symbol_cmp) once for
     each type, let's lie a bit to the typing system, and treat
     'semantic_type' as if it were 'symbol'. */
  table_sort (semantic_type_table, (प्रतीक ***) &semantic_types_sorted);

  प्रति (पूर्णाङ्क i = 0; symbols_sorted[i]; ++i)
    प्रतीक_check_defined (symbols_sorted[i]);
  प्रति (पूर्णाङ्क i = 0; semantic_types_sorted[i]; ++i)
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
  प्रति (पूर्णाङ्क i = 0; i < ntokens; ++i)
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

  प्रति (पूर्णाङ्क i = 0; i < ntokens; ++i)
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
  प्रति (पूर्णाङ्क i = 0; i < max_code + 1; ++i)
    token_translations[i] = undeftoken->content->संख्या;
  प्रति (पूर्णाङ्क i = 0; symbols_sorted[i]; ++i)
    प्रतीक_translation (symbols_sorted[i]);
}


/* Whether some symbol requires internationalization.  */
स्थिर bool
has_translations (शून्य)
{
  प्रति (ध्रुव शून्य *entry = hash_get_first (प्रतीक_table);
       entry;
       entry = hash_get_next (प्रतीक_table, entry))
    {
      ध्रुव प्रतीक *sym = (ध्रुव प्रतीक *) entry;
      यदि (sym->translatable)
        प्रत्यागम true;
    }
  प्रत्यागम false;
}

/*----------------------------------------------------------------.
| Assign symbol numbers, and write definition of token names into |
| FDEFINES.  Set up vectors SYMBOL_TABLE, TAGS of symbols.        |
`----------------------------------------------------------------*/

शून्य
symbols_pack (शून्य)
{
  symbols = xcalloc (nsyms, परिमाण *symbols);
  प्रति (पूर्णाङ्क i = 0; symbols_sorted[i]; ++i)
    प्रतीक_pack (symbols_sorted[i]);

  /* Aliases leave empty slots in symbols, so remove them.  */
  {
    पूर्णाङ्क nsyms_old = nsyms;
    प्रति (पूर्णाङ्क writei = 0, readi = 0; readi < nsyms_old; readi += 1)
      {
        यदि (symbols[readi] == रिक्त)
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
  symbols = xnrealloc (symbols, nsyms, परिमाण *symbols);

  symbols_token_translations_init ();

  // If some user tokens are internationalized, the internal ones
  // should be too.
  यदि (has_translations ())
    {
      ध्रुव bool eof_is_user_defined
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
  प्रति (पूर्णाङ्क i = 0; i < nsyms; ++i)
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
  प्रत्यागम res;
}


/*------------------------------------------------------------------.
| Register the second symbol of the precedence relation, and return |
| whether this relation is new.  Use only in register_precedence.   |
`------------------------------------------------------------------*/

स्थिर bool
register_precedence_second_प्रतीक (symgraphlink **first, graphid sym)
{
  यदि (!*first || sym < (*first)->id)
    *first = symgraphlink_new (sym, *first);
  अन्यथा
    {
      symgraphlink *slist = *first;

      यावत् (slist->next && slist->next->id <= sym)
        slist = slist->next;

      यदि (slist->id == sym)
        /* Relation already present. */
        प्रत्यागम false;

      slist->next = symgraphlink_new (sym, slist->next);
    }
  प्रत्यागम true;
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
  register_precedence_second_प्रतीक (&(prec_nodes[first]->succ), snd);
  register_precedence_second_प्रतीक (&(prec_nodes[snd]->pred), first);
}


/*---------------------------------------.
| Deep clear a linked / adjacency list). |
`---------------------------------------*/

स्थिर शून्य
linkedlist_free (symgraphlink *गाँठ)
{
  यदि (गाँठ)
    {
      यावत् (गाँठ->next)
        {
          symgraphlink *tmp = गाँठ->next;
          मुक्त (गाँठ);
          गाँठ = tmp;
        }
      मुक्त (गाँठ);
    }
}

/*----------------------------------------------.
| Clear and destroy association tracking table. |
`----------------------------------------------*/

स्थिर शून्य
assoc_free (शून्य)
{
  प्रति (पूर्णाङ्क i = 0; i < nsyms; ++i)
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
  प्रति (graphid i = 0; i < nsyms; ++i)
    used_assoc[i] = false;
}

/*------------------------------------------------------------------.
| Test if the associativity for the symbols is defined and useless. |
`------------------------------------------------------------------*/

स्थिर अन्तःस्थ bool
is_assoc_useless (प्रतीक *s)
{
  प्रत्यागम s
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
  प्रति (पूर्णाङ्क i = 0; i < nsyms; ++i)
    {
      प्रतीक *s = symbols[i];
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
