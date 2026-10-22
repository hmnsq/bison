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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ "symtab.h"

#அந்தர்பாவ "system.h"

#அந்தர்பாவ <assure.h>
#அந்தர்பாவ <fstrcmp.h>
#அந்தர்பாவ <hash.h>
#அந்தர்பாவ <quote.h>

#அந்தர்பாவ "complain.h"
#அந்தர்பாவ "getargs.h"
#அந்தர்பாவ "gram.h"
#அந்தர்பாவ "intprops.h"

/** Undefined token code.  */
#define CODE_UNDEFINED (-1)

/* Undefined symbol number.  */
#define எண்_UNDEFINED (-1)


நிலையான கட்டமைப்பு hash_table *குறியீடு_table = வெற்று;
நிலையான கட்டமைப்பு hash_table *semantic_type_table = வெற்று;

/*----------------------------------------------------------------.
| Symbols sorted by tag.  Allocated by table_sort, after which no |
| more symbols should be created.                                 |
`----------------------------------------------------------------*/

நிலையான குறியீடு **symbols_sorted = வெற்று;
நிலையான semantic_type **semantic_types_sorted = வெற்று;


/*------------------------.
| Distinguished symbols.  |
`------------------------*/

குறியீடு *errtoken = வெற்று;
குறியீடு *undeftoken = வெற்று;
குறியீடு *eoftoken = வெற்று;
குறியீடு *acceptsymbol = வெற்று;

/* Precedence relation graph. */
நிலையான symgraph **prec_nodes;

/* Store which associativity is used.  */
நிலையான bool *used_assoc = வெற்று;

bool tag_seen = false;


/* Whether SYM was defined by the user.  */

நிலையான bool
குறியீடு_is_user_defined (குறியீடு *sym)
{
  மாறிலி bool eof_is_user_defined
    = !eoftoken->alias || STRNEQ (eoftoken->alias->tag, "$end");
  திரும்பு sym->tag[0] != '$'
    && (eof_is_user_defined || (sym != eoftoken && sym->alias != errtoken))
    && sym != errtoken && sym->alias != errtoken
    && sym != undeftoken && sym->alias != undeftoken;
}


/*--------------------------.
| Create a new sym_content. |
`--------------------------*/

நிலையான sym_content *
sym_content_new (குறியீடு *s)
{
  sym_content *res = xmalloc (அளவு *res);

  res->குறியீடு = s;

  res->type_name = வெற்று;
  res->type_loc = empty_loc;
  க்கு (முழு i = 0; i < CODE_PROPS_SIZE; ++i)
    code_props_none_init (&res->props[i]);

  res->எண் = எண்_UNDEFINED;
  res->prec_loc = empty_loc;
  res->prec = 0;
  res->assoc = undef_assoc;
  res->code = CODE_UNDEFINED;

  res->class = unknown_sym;
  res->status = undeclared;

  திரும்பு res;
}

/*---------------------------------.
| Create a new symbol, named TAG.  |
`---------------------------------*/

நிலையான குறியீடு *
குறியீடு_new (uniqstr tag, location loc)
{
  குறியீடு *res = xmalloc (அளவு *res);
  uniqstr_assert (tag);

  /* If the tag is not a string (starts with a double quote), check
     that it is valid for Yacc. */
  எனில் (tag[0] != '\"' && tag[0] != '\'' && strchr (tag, '-'))
    complain (&loc, Wyacc,
              _("POSIX Yacc forbids dashes in symbol names: %s"), tag);

  res->tag = tag;
  res->location = loc;
  res->translatable = false;
  res->location_of_lhs = false;
  res->alias = வெற்று;
  res->content = sym_content_new (res);
  res->is_alias = false;
  திரும்பு res;
}

/*--------------------.
| Free a sym_content. |
`--------------------*/

நிலையான வெறுமை
sym_content_free (sym_content *sym)
{
  விடுவி (sym);
}


/*---------------------------------------------------------.
| Free a symbol and its associated content if appropriate. |
`---------------------------------------------------------*/

நிலையான வெறுமை
குறியீடு_free (வெறுமை *ptr)
{
  குறியீடு *sym = (குறியீடு *)ptr;
  எனில் (!sym->is_alias)
    sym_content_free (sym->content);
  விடுவி (sym);

}

/* If needed, swap first and second so that first has the earliest
   location (according to location_cmp).

   Many symbol features (e.g., token codes) are not assigned during
   parsing, but in a second step, via a traversal of the symbol table
   sorted on tag.

   However, error messages make more sense if we keep the first
   declaration first.
*/

நிலையான வெறுமை
symbols_sort (மாறிலி குறியீடு **first, மாறிலி குறியீடு **second)
{
  எனில் (0 < location_cmp ((*first)->location, (*second)->location))
    {
      மாறிலி குறியீடு* tmp = *first;
      *first = *second;
      *second = tmp;
    }
}

/* Likewise, for locations.  */

நிலையான வெறுமை
locations_sort (location *first, location *second)
{
  எனில் (0 < location_cmp (*first, *second))
    {
      location tmp = *first;
      *first = *second;
      *second = tmp;
    }
}

எழுத்து மாறிலி *
code_props_type_string (code_props_type kind)
{
  தேர்வு (kind)
    {
    நிலை destructor:
      திரும்பு "%destructor";
    நிலை printer:
      திரும்பு "%printer";
    }
  abort ();
}


/*----------------------------------------.
| Create a new semantic type, named TAG.  |
`----------------------------------------*/

நிலையான semantic_type *
semantic_type_new (uniqstr tag, மாறிலி location *loc)
{
  semantic_type *res = xmalloc (அளவு *res);

  uniqstr_assert (tag);
  res->tag = tag;
  res->location = loc ? *loc : empty_loc;
  res->status = undeclared;
  க்கு (முழு i = 0; i < CODE_PROPS_SIZE; ++i)
    code_props_none_init (&res->props[i]);

  திரும்பு res;
}


/*-----------------.
| Print a symbol.  |
`-----------------*/

#define குறியீடு_INT_ATTR_PRINT(Attr)                     \
  எனில் (s->content)                                       \
    fprintf (f, " %s = %d", #Attr, s->content->Attr)

#define குறியீடு_STR_ATTR_PRINT(Attr)                     \
  எனில் (s->content && s->content->Attr)                   \
    fprintf (f, " %s { %s }", #Attr, s->content->Attr)

#define குறியீடு_CODE_PRINT(Attr)                                         \
  எனில் (s->content && s->content->props[Attr].code)                       \
    fprintf (f, " %s { %s }", #Attr, s->content->props[Attr].code)

வெறுமை
குறியீடு_print (குறியீடு மாறிலி *s, FILE *f)
{
  எனில் (s)
    {
      குறியீடு_class c = s->content->class;
      fprintf (f, "%s: %s",
               c == unknown_sym    ? "unknown"
               : c == pct_type_sym ? "%type"
               : c == token_sym    ? "token"
               : c == nterm_sym    ? "nterm"
               : வெற்று, /* abort.  */
               s->tag);
      putc (' ', f);
      location_print (s->location, f);
      குறியீடு_INT_ATTR_PRINT (code);
      குறியீடு_INT_ATTR_PRINT (எண்);
      குறியீடு_STR_ATTR_PRINT (type_name);
      குறியீடு_CODE_PRINT (destructor);
      குறியீடு_CODE_PRINT (printer);
    }
  இல்லையெனில்
    fputs ("<NULL>", f);
}

#undef குறியீடு_ATTR_PRINT
#undef குறியீடு_CODE_PRINT


/*----------------------------------.
| Whether S is a valid identifier.  |
`----------------------------------*/

நிலையான bool
is_identifier (uniqstr s)
{
  நிலையான எழுத்து மாறிலி alphanum[26 + 26 + 1 + 10] =
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "_"
    "0123456789";
  எனில் (!s || ! memchr (alphanum, *s, அளவு alphanum - 10))
    திரும்பு false;
  க்கு (++s; *s; ++s)
    எனில் (! memchr (alphanum, *s, அளவு alphanum))
      திரும்பு false;
  திரும்பு true;
}


/*-----------------------------------------------.
| Get the identifier associated to this symbol.  |
`-----------------------------------------------*/

uniqstr
குறியீடு_id_get (குறியீடு மாறிலி *sym)
{
  // There's one weird case: YYerror is the alias, and error is the
  // base symbol.  Return YYerror in that case.
  எனில் (sym->alias && is_identifier (sym->alias->tag))
    திரும்பு sym->alias->tag;
  இல்லையெனில் எனில் (is_identifier (sym->tag))
    திரும்பு sym->tag;
  இல்லையெனில்
    திரும்பு வெற்று;
}


/*------------------------------------------------------------------.
| Complain that S's WHAT is redeclared at SECOND, and was first set |
| at FIRST.                                                         |
`------------------------------------------------------------------*/

நிலையான வெறுமை
complain_குறியீடு_redeclared (குறியீடு *s, மாறிலி எழுத்து *what, location first,
                            location second)
{
  locations_sort (&first, &second);
  complain (&second, complaint, _("%s redeclaration for %s"), what, s->tag);
  subcomplain (&first, complaint, _("previous declaration"));
}

நிலையான வெறுமை
complain_semantic_type_redeclared (semantic_type *s, மாறிலி எழுத்து *what, location first,
                                   location second)
{
  locations_sort (&first, &second);
  complain (&second, complaint, _("%s redeclaration for <%s>"), what, s->tag);
  subcomplain (&first, complaint, _("previous declaration"));
}

நிலையான வெறுமை
complain_class_redeclared (குறியீடு *sym, குறியீடு_class class, location second)
{
  complain (&second, complaint,
            class == token_sym
            ? _("symbol %s redeclared as a token")
            : _("symbol %s redeclared as a nonterminal"), sym->tag);
  எனில் (!location_empty (sym->location))
    subcomplain (&sym->location, complaint, _("previous definition"));
}

நிலையான மாறிலி குறியீடு *
குறியீடு_from_uniqstr_fuzzy (மாறிலி uniqstr சாவி)
{
  aver (symbols_sorted);
#define FSTRCMP_THRESHOLD 0.6
  இரட்டை best_similarity = FSTRCMP_THRESHOLD;
  மாறிலி குறியீடு *res = வெற்று;
  size_t மொத்தம் = hash_get_n_entries (குறியீடு_table);
  க்கு (முழு i = 0; i < மொத்தம்; ++i)
    {
      குறியீடு *sym = symbols_sorted[i];
      எனில் (STRNEQ (சாவி, sym->tag)
          && (sym->content->status == declared
              || sym->content->status == undeclared))
        {
          இரட்டை similarity = fstrcmp_bounded (சாவி, sym->tag, best_similarity);
          எனில் (best_similarity < similarity)
            {
              res = sym;
              best_similarity = similarity;
            }
        }
    }
  திரும்பு res;
}

நிலையான வெறுமை
complain_குறியீடு_undeclared (மாறிலி குறியீடு *sym)
{
  ப்ரதிஜ்ஞா (sym->content->status != declared);
  மாறிலி குறியீடு *best = குறியீடு_from_uniqstr_fuzzy (sym->tag);
  எனில் (best)
    {
      complain (&sym->location,
                sym->content->status == needed ? complaint : Wother,
                _("symbol %s is used, but is not defined as a token"
                  " and has no rules; did you mean %s?"),
                quote_n (0, sym->tag),
                quote_n (1, best->tag));
      எனில் (feature_flag & feature_caret)
        location_caret_suggestion (sym->location, best->tag, stderr);
    }
  இல்லையெனில்
    complain (&sym->location,
              sym->content->status == needed ? complaint : Wother,
              _("symbol %s is used, but is not defined as a token"
                " and has no rules"),
              quote (sym->tag));
}

வெறுமை
குறியீடு_location_as_lhs_set (குறியீடு *sym, location loc)
{
  எனில் (!sym->location_of_lhs)
    {
      sym->location = loc;
      sym->location_of_lhs = true;
    }
}


/*-----------------------------------------------------------------.
| Set the TYPE_NAME associated with SYM.  Does nothing if passed 0 |
| as TYPE_NAME.                                                    |
`-----------------------------------------------------------------*/

வெறுமை
குறியீடு_type_set (குறியீடு *sym, uniqstr type_name, location loc)
{
  எனில் (type_name)
    {
      tag_seen = true;
      எனில் (sym->content->type_name)
        complain_குறியீடு_redeclared (sym, "%type",
                                    sym->content->type_loc, loc);
      இல்லையெனில்
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

வெறுமை
குறியீடு_code_props_set (குறியீடு *sym, code_props_type kind,
                       code_props மாறிலி *code)
{
  எனில் (sym->content->props[kind].code)
    complain_குறியீடு_redeclared (sym, code_props_type_string (kind),
                                sym->content->props[kind].location,
                                code->location);
  இல்லையெனில்
    sym->content->props[kind] = *code;
}


/*-----------------------------------------------------.
| Set the DESTRUCTOR or PRINTER associated with TYPE.  |
`-----------------------------------------------------*/

வெறுமை
semantic_type_code_props_set (semantic_type *type,
                              code_props_type kind,
                              code_props மாறிலி *code)
{
  எனில் (type->props[kind].code)
    complain_semantic_type_redeclared (type, code_props_type_string (kind),
                                       type->props[kind].location,
                                       code->location);
  இல்லையெனில்
    type->props[kind] = *code;
}

/*---------------------------------------------------.
| Get the computed %destructor or %printer for SYM.  |
`---------------------------------------------------*/

code_props *
குறியீடு_code_props_get (குறியீடு *sym, code_props_type kind)
{
  /* Per-symbol code props.  */
  எனில் (sym->content->props[kind].code)
    திரும்பு &sym->content->props[kind];

  /* Per-type code props.  */
  எனில் (sym->content->type_name)
    {
      code_props *code =
        &semantic_type_get (sym->content->type_name, வெற்று)->props[kind];
      எனில் (code->code)
        திரும்பு code;
    }

  /* Apply default code props's only to user-defined symbols.  */
  எனில் (குறியீடு_is_user_defined (sym))
    {
      code_props *code = &semantic_type_get (sym->content->type_name ? "*" : "",
                                             வெற்று)->props[kind];
      எனில் (code->code)
        திரும்பு code;
    }
  திரும்பு &code_props_none;
}

/*-----------------------------------------------------------------.
| Set the PRECEDENCE associated with SYM.  Does nothing if invoked |
| with UNDEF_ASSOC as ASSOC.                                       |
`-----------------------------------------------------------------*/

வெறுமை
குறியீடு_precedence_set (குறியீடு *sym, முழு prec, assoc a, location loc)
{
  எனில் (a != undef_assoc)
    {
      sym_content *s = sym->content;
      எனில் (s->prec)
        complain_குறியீடு_redeclared (sym, assoc_to_string (a),
                                    s->prec_loc, loc);
      இல்லையெனில்
        {
          s->prec = prec;
          s->assoc = a;
          s->prec_loc = loc;
        }
    }

  /* Only terminals have a precedence. */
  குறியீடு_class_set (sym, token_sym, loc, false);
}


/*------------------------------------.
| Set the CLASS associated with SYM.  |
`------------------------------------*/

நிலையான வெறுமை
complain_pct_type_on_token (location *loc)
{
  complain (loc, Wyacc,
            _("POSIX yacc reserves %%type to nonterminals"));
}

வெறுமை
குறியீடு_class_set (குறியீடு *sym, குறியீடு_class class, location loc, bool declaring)
{
  aver (class != unknown_sym);
  sym_content *s = sym->content;
  எனில் (class == pct_type_sym)
    {
      எனில் (s->class == token_sym)
        complain_pct_type_on_token (&loc);
      இல்லையெனில் எனில் (s->class == unknown_sym)
        s->class = class;
    }
  இல்லையெனில் எனில் (s->class != unknown_sym && s->class != pct_type_sym
           && s->class != class)
    complain_class_redeclared (sym, class, loc);
  இல்லையெனில்
    {
      எனில் (class == token_sym && s->class == pct_type_sym)
        complain_pct_type_on_token (&sym->location);

      s->class = class;

      எனில் (declaring)
        {
          எனில் (s->status == declared)
            {
              complain (&loc, Wother,
                        _("symbol %s redeclared"), sym->tag);
              subcomplain (&sym->location, Wother,
                           _("previous declaration"));
            }
          இல்லையெனில்
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

வெறுமை
குறியீடு_code_set (குறியீடு *sym, முழு code, location loc)
{
  முழு *codep = &sym->content->code;
  எனில் (sym->content->class != token_sym)
    complain (&loc, complaint,
              _("nonterminals cannot be given a token code"));
  இல்லையெனில் எனில் (*codep != CODE_UNDEFINED
           && *codep != code)
    complain (&loc, complaint, _("redefining code of token %s"),
              sym->tag);
  இல்லையெனில் எனில் (code == INT_MAX)
    complain (&loc, complaint, _("code of token %s too large"),
              sym->tag);
  இல்லையெனில்
    {
      *codep = code;
      /* User defined $end token? */
      எனில் (code == 0 && !eoftoken)
        {
          eoftoken = sym->content->குறியீடு;
          eoftoken->content->எண் = 0;
        }
    }
}


/*----------------------------------------------------------.
| If SYM is not defined, report an error, and consider it a |
| nonterminal.                                              |
`----------------------------------------------------------*/

நிலையான வெறுமை
குறியீடு_check_defined (குறியீடு *sym)
{
  sym_content *s = sym->content;
  எனில் (s->class == unknown_sym || s->class == pct_type_sym)
    {
      complain_குறியீடு_undeclared (sym);
      s->class = nterm_sym;
    }

  எனில் (s->எண் == எண்_UNDEFINED)
    s->எண் = s->class == token_sym ? ntokens++ : nnterms++;

  எனில் (s->class == token_sym
      && sym->tag[0] == '"'
      && !sym->is_alias)
    complain (&sym->location, Wdangling_alias,
              _("string literal %s not attached to a symbol"),
              sym->tag);

  க்கு (முழு i = 0; i < 2; ++i)
    குறியீடு_code_props_get (sym, i)->is_used = true;

  /* Set the semantic type status associated to the current symbol to
     'declared' so that we could check semantic types unnecessary uses. */
  எனில் (s->type_name)
    {
      semantic_type *sem_type = semantic_type_get (s->type_name, வெற்று);
      எனில் (sem_type)
        sem_type->status = declared;
    }
}

நிலையான வெறுமை
semantic_type_check_defined (semantic_type *sem_type)
{
  /* <*> and <> do not have to be "declared".  */
  எனில் (sem_type->status == declared
      || !*sem_type->tag
      || STREQ (sem_type->tag, "*"))
    {
      க்கு (முழு i = 0; i < 2; ++i)
        எனில் (sem_type->props[i].kind != CODE_PROPS_NONE
            && ! sem_type->props[i].is_used)
          complain (&sem_type->location, Wother,
                    _("useless %s for type <%s>"),
                    code_props_type_string (i), sem_type->tag);
    }
  இல்லையெனில்
    complain (&sem_type->location, Wother,
              _("type <%s> is used, but is not associated to any symbol"),
              sem_type->tag);
}

/*-------------------------------------------------------------------.
| Merge the properties (precedence, associativity, etc.) of SYM, and |
| its string-named alias STR; check consistency.                     |
`-------------------------------------------------------------------*/

நிலையான வெறுமை
குறியீடு_merge_properties (குறியீடு *sym, குறியீடு *str)
{
  எனில் (str->content->type_name != sym->content->type_name)
    {
      எனில் (str->content->type_name)
        குறியீடு_type_set (sym,
                         str->content->type_name, str->content->type_loc);
      இல்லையெனில்
        குறியீடு_type_set (str,
                         sym->content->type_name, sym->content->type_loc);
    }


  க்கு (முழு i = 0; i < CODE_PROPS_SIZE; ++i)
    எனில் (str->content->props[i].code)
      குறியீடு_code_props_set (sym, i, &str->content->props[i]);
    இல்லையெனில் எனில் (sym->content->props[i].code)
      குறியீடு_code_props_set (str, i, &sym->content->props[i]);

  எனில் (sym->content->prec || str->content->prec)
    {
      எனில் (str->content->prec)
        குறியீடு_precedence_set (sym, str->content->prec, str->content->assoc,
                               str->content->prec_loc);
      இல்லையெனில்
        குறியீடு_precedence_set (str, sym->content->prec, sym->content->assoc,
                               sym->content->prec_loc);
    }
}

வெறுமை
குறியீடு_make_alias (குறியீடு *sym, குறியீடு *str, location loc)
{
  எனில் (sym->content->class != token_sym)
    complain (&loc, complaint,
              _("nonterminals cannot be given a string alias"));
  இல்லையெனில் எனில் (str->alias)
    complain (&loc, Wother,
              _("symbol %s used more than once as a literal string"), str->tag);
  இல்லையெனில் எனில் (sym->alias)
    complain (&loc, Wother,
              _("symbol %s given more than one literal string"), sym->tag);
  இல்லையெனில்
    {
      குறியீடு_merge_properties (sym, str);
      sym_content_free (str->content);
      str->content = sym->content;
      str->content->குறியீடு = str;
      str->is_alias = true;
      str->alias = sym;
      sym->alias = str;
    }
}


/*-------------------------------------------------------------------.
| Assign a symbol number, and write the definition of the token name |
| into FDEFINES.  Put in SYMBOLS.                                    |
`-------------------------------------------------------------------*/

நிலையான வெறுமை
குறியீடு_pack (குறியீடு *sym)
{
  aver (sym->content->எண் != எண்_UNDEFINED);
  எனில் (sym->content->class == nterm_sym)
    sym->content->எண் += ntokens;

  symbols[sym->content->எண்] = sym->content->குறியீடு;
}

நிலையான வெறுமை
complain_code_redeclared (முழு இலக்கம், மாறிலி குறியீடு *first, மாறிலி குறியீடு *second)
{
  symbols_sort (&first, &second);
  complain (&second->location, complaint,
            _("code %d reassigned to token %s"),
            இலக்கம், second->tag);
  subcomplain (&first->location, complaint,
               _("previous declaration for %s"),
               first->tag);
}

/*-------------------------------------------------.
| Put SYM in TOKEN_TRANSLATIONS if it is a token.  |
`-------------------------------------------------*/

நிலையான வெறுமை
குறியீடு_translation (மாறிலி குறியீடு *sym)
{
  எனில் (sym->content->class == token_sym && !sym->is_alias)
    {
      /* A token whose translation has already been set? */
      எனில் (token_translations[sym->content->code]
          != undeftoken->content->எண்)
        complain_code_redeclared
          (sym->content->code,
           symbols[token_translations[sym->content->code]], sym);
      இல்லையெனில்
        token_translations[sym->content->code]
          = sym->content->எண்;
    }
}


/*---------------------------------------.
| Symbol and semantic type hash tables.  |
`---------------------------------------*/

/* Initial capacity of symbol and semantic type hash table.  */
#define HT_INITIAL_CAPACITY 257

நிலையான உள்வரி bool
hash_compare_குறியீடு (மாறிலி குறியீடு *m1, மாறிலி குறியீடு *m2)
{
  /* Since tags are unique, we can compare the pointers themselves.  */
  திரும்பு UNIQSTR_EQ (m1->tag, m2->tag);
}

நிலையான உள்வரி bool
hash_compare_semantic_type (மாறிலி semantic_type *m1, மாறிலி semantic_type *m2)
{
  /* Since names are unique, we can compare the pointers themselves.  */
  திரும்பு UNIQSTR_EQ (m1->tag, m2->tag);
}

நிலையான bool
hash_குறியீடு_comparator (வெறுமை மாறிலி *m1, வெறுமை மாறிலி *m2)
{
  திரும்பு hash_compare_குறியீடு (m1, m2);
}

நிலையான bool
hash_semantic_type_comparator (வெறுமை மாறிலி *m1, வெறுமை மாறிலி *m2)
{
  திரும்பு hash_compare_semantic_type (m1, m2);
}

நிலையான உள்வரி size_t
hash_குறியீடு (மாறிலி குறியீடு *m, size_t tablesize)
{
  /* Since tags are unique, we can hash the pointer itself.  */
  திரும்பு ((uintptr_t) m->tag) % tablesize;
}

நிலையான உள்வரி size_t
hash_semantic_type (மாறிலி semantic_type *m, size_t tablesize)
{
  /* Since names are unique, we can hash the pointer itself.  */
  திரும்பு ((uintptr_t) m->tag) % tablesize;
}

நிலையான size_t
hash_குறியீடு_hasher (வெறுமை மாறிலி *m, size_t tablesize)
{
  திரும்பு hash_குறியீடு (m, tablesize);
}

நிலையான size_t
hash_semantic_type_hasher (வெறுமை மாறிலி *m, size_t tablesize)
{
  திரும்பு hash_semantic_type (m, tablesize);
}

/*-------------------------------.
| Create the symbol hash table.  |
`-------------------------------*/

வெறுமை
symbols_new (வெறுமை)
{
  குறியீடு_table = hash_xinitialize (HT_INITIAL_CAPACITY,
                                   வெற்று,
                                   hash_குறியீடு_hasher,
                                   hash_குறியீடு_comparator,
                                   குறியீடு_free);

  /* Construct the acceptsymbol symbol. */
  acceptsymbol = குறியீடு_get ("$accept", empty_loc);
  acceptsymbol->content->class = nterm_sym;
  acceptsymbol->content->எண் = nnterms++;

  /* Construct the YYerror/"error" token */
  errtoken = குறியீடு_get ("YYerror", empty_loc);
  errtoken->content->class = token_sym;
  errtoken->content->எண் = ntokens++;
  {
    குறியீடு *alias = குறியீடு_get ("error", empty_loc);
    குறியீடு_class_set (alias, token_sym, empty_loc, false);
    குறியீடு_make_alias (errtoken, alias, empty_loc);
  }

  /* Construct the YYUNDEF/"$undefined" token that represents all
     undefined literal tokens.  It is always symbol number 2.  */
  undeftoken = குறியீடு_get ("YYUNDEF", empty_loc);
  undeftoken->content->class = token_sym;
  undeftoken->content->எண் = ntokens++;
  {
    குறியீடு *alias = குறியீடு_get ("$undefined", empty_loc);
    குறியீடு_class_set (alias, token_sym, empty_loc, false);
    குறியீடு_make_alias (undeftoken, alias, empty_loc);
  }

  semantic_type_table = hash_xinitialize (HT_INITIAL_CAPACITY,
                                          வெற்று,
                                          hash_semantic_type_hasher,
                                          hash_semantic_type_comparator,
                                          விடுவி);
}


/*----------------------------------------------------------------.
| Find the symbol named KEY, and return it.  If it does not exist |
| yet, create it.                                                 |
`----------------------------------------------------------------*/

குறியீடு *
குறியீடு_from_uniqstr (மாறிலி uniqstr சாவி, location loc)
{
  குறியீடு probe;

  probe.tag = சாவி;
  குறியீடு *res = hash_lookup (குறியீடு_table, &probe);

  எனில் (!res)
    {
      /* First insertion in the hash. */
      aver (!symbols_sorted);
      res = குறியீடு_new (சாவி, loc);
      hash_xinsert (குறியீடு_table, res);
    }
  திரும்பு res;
}


/*-----------------------------------------------------------------------.
| Find the semantic type named KEY, and return it.  If it does not exist |
| yet, create it.                                                        |
`-----------------------------------------------------------------------*/

semantic_type *
semantic_type_from_uniqstr (மாறிலி uniqstr சாவி, மாறிலி location *loc)
{
  semantic_type probe;

  probe.tag = சாவி;
  semantic_type *res = hash_lookup (semantic_type_table, &probe);

  எனில் (!res)
    {
      /* First insertion in the hash. */
      res = semantic_type_new (சாவி, loc);
      hash_xinsert (semantic_type_table, res);
    }
  திரும்பு res;
}


/*----------------------------------------------------------------.
| Find the symbol named KEY, and return it.  If it does not exist |
| yet, create it.                                                 |
`----------------------------------------------------------------*/

குறியீடு *
குறியீடு_get (மாறிலி எழுத்து *சாவி, location loc)
{
  திரும்பு குறியீடு_from_uniqstr (uniqstr_new (சாவி), loc);
}


/*-----------------------------------------------------------------------.
| Find the semantic type named KEY, and return it.  If it does not exist |
| yet, create it.                                                        |
`-----------------------------------------------------------------------*/

semantic_type *
semantic_type_get (மாறிலி எழுத்து *சாவி, மாறிலி location *loc)
{
  திரும்பு semantic_type_from_uniqstr (uniqstr_new (சாவி), loc);
}


/*------------------------------------------------------------------.
| Generate a dummy nonterminal, whose name cannot conflict with the |
| user's names.                                                     |
`------------------------------------------------------------------*/

குறியீடு *
dummy_குறியீடு_get (location loc)
{
  /* Incremented for each generated symbol.  */
  நிலையான முழு dummy_மொத்தம் = 0;
  எழுத்து buf[32];
  முழு len = snprintf (buf, அளவு buf, "$@%d", ++dummy_மொத்தம்);
  assure (len < அளவு buf);
  குறியீடு *sym = குறியீடு_get (buf, loc);
  sym->content->class = nterm_sym;
  திரும்பு sym;
}

bool
குறியீடு_is_dummy (குறியீடு மாறிலி *sym)
{
  திரும்பு sym->tag[0] == '@' || (sym->tag[0] == '$' && sym->tag[1] == '@');
}

/*-------------------.
| Free the symbols.  |
`-------------------*/

வெறுமை
symbols_free (வெறுமை)
{
  hash_free (குறியீடு_table);
  hash_free (semantic_type_table);
  விடுவி (symbols);
  விடுவி (symbols_sorted);
  விடுவி (semantic_types_sorted);
}


நிலையான முழு
குறியீடு_cmp (வெறுமை மாறிலி *a, வெறுமை மாறிலி *b)
{
  திரும்பு location_cmp ((*(குறியீடு * மாறிலி *)a)->location,
                       (*(குறியீடு * மாறிலி *)b)->location);
}

/* Store in *SORTED an array of pointers to the symbols contained in
   TABLE, sorted by order of appearance (i.e., by location). */

நிலையான வெறுமை
table_sort (கட்டமைப்பு hash_table *table, குறியீடு ***sorted)
{
  aver (!*sorted);
  size_t மொத்தம் = hash_get_n_entries (table);
  *sorted = xnmalloc (மொத்தம் + 1, அளவு **sorted);
  hash_get_entries (table, (வெறுமை**)*sorted, மொத்தம்);
  qsort (*sorted, மொத்தம், அளவு **sorted, குறியீடு_cmp);
  (*sorted)[மொத்தம்] = வெற்று;
}

/*--------------------------------------------------------------.
| Check that all the symbols are defined.  Report any undefined |
| symbols and consider them nonterminals.                       |
`--------------------------------------------------------------*/

வெறுமை
symbols_check_defined (வெறுமை)
{
  table_sort (குறியீடு_table, &symbols_sorted);
  /* semantic_type, like symbol, starts with a 'tag' field and then a
     'location' field.  And here we only deal with arrays/hashes of
     pointers, sizeof is not an issue.

     So instead of implementing table_sort (and symbol_cmp) once for
     each type, let's lie a bit to the typing system, and treat
     'semantic_type' as if it were 'symbol'. */
  table_sort (semantic_type_table, (குறியீடு ***) &semantic_types_sorted);

  க்கு (முழு i = 0; symbols_sorted[i]; ++i)
    குறியீடு_check_defined (symbols_sorted[i]);
  க்கு (முழு i = 0; semantic_types_sorted[i]; ++i)
    semantic_type_check_defined (semantic_types_sorted[i]);
}

/*------------------------------------------------------------------.
| Set TOKEN_TRANSLATIONS.  Check that no two symbols share the same |
| number.                                                           |
`------------------------------------------------------------------*/

நிலையான வெறுமை
symbols_token_translations_init (வெறுமை)
{
  bool code_256_available_p = true;

  /* Find the highest token code, and whether 256, the POSIX preferred
     token code for the error token, is used.  */
  max_code = 0;
  க்கு (முழு i = 0; i < ntokens; ++i)
    {
      sym_content *sym = symbols[i]->content;
      எனில் (sym->code != CODE_UNDEFINED)
        {
          எனில் (sym->code > max_code)
            max_code = sym->code;
          எனில் (sym->code == 256)
            code_256_available_p = false;
        }
    }

  /* If 256 is not used, assign it to error, to follow POSIX.  */
  எனில் (code_256_available_p
      && errtoken->content->code == CODE_UNDEFINED)
    errtoken->content->code = 256;

  /* Set the missing codes. */
  எனில் (max_code < 256)
    max_code = 256;

  க்கு (முழு i = 0; i < ntokens; ++i)
    {
      sym_content *sym = symbols[i]->content;
      எனில் (sym->code == CODE_UNDEFINED)
        {
          IGNORE_TYPE_LIMITS_BEGIN
          எனில் (INT_ADD_WRAPV (max_code, 1, &max_code))
            complain (வெற்று, fatal, _("token number too large"));
          IGNORE_TYPE_LIMITS_END
          sym->code = max_code;
        }
      எனில் (sym->code > max_code)
        max_code = sym->code;
    }

  token_translations = xnmalloc (max_code + 1,
                                 அளவு *token_translations);

  /* Initialize all entries for literal tokens to the internal token
     number for $undefined, which represents all invalid inputs.  */
  க்கு (முழு i = 0; i < max_code + 1; ++i)
    token_translations[i] = undeftoken->content->எண்;
  க்கு (முழு i = 0; symbols_sorted[i]; ++i)
    குறியீடு_translation (symbols_sorted[i]);
}


/* Whether some symbol requires internationalization.  */
நிலையான bool
has_translations (வெறுமை)
{
  க்கு (மாறிலி வெறுமை *entry = hash_get_first (குறியீடு_table);
       entry;
       entry = hash_get_next (குறியீடு_table, entry))
    {
      மாறிலி குறியீடு *sym = (மாறிலி குறியீடு *) entry;
      எனில் (sym->translatable)
        திரும்பு true;
    }
  திரும்பு false;
}

/*----------------------------------------------------------------.
| Assign symbol numbers, and write definition of token names into |
| FDEFINES.  Set up vectors SYMBOL_TABLE, TAGS of symbols.        |
`----------------------------------------------------------------*/

வெறுமை
symbols_pack (வெறுமை)
{
  symbols = xcalloc (nsyms, அளவு *symbols);
  க்கு (முழு i = 0; symbols_sorted[i]; ++i)
    குறியீடு_pack (symbols_sorted[i]);

  /* Aliases leave empty slots in symbols, so remove them.  */
  {
    முழு nsyms_old = nsyms;
    க்கு (முழு writei = 0, readi = 0; readi < nsyms_old; readi += 1)
      {
        எனில் (symbols[readi] == வெற்று)
          {
            nsyms -= 1;
            ntokens -= 1;
          }
        இல்லையெனில்
          {
            symbols[writei] = symbols[readi];
            symbols[writei]->content->எண் = writei;
            writei += 1;
          }
      }
  }
  symbols = xnrealloc (symbols, nsyms, அளவு *symbols);

  symbols_token_translations_init ();

  // If some user tokens are internationalized, the internal ones
  // should be too.
  எனில் (has_translations ())
    {
      மாறிலி bool eof_is_user_defined
        = !eoftoken->alias || STRNEQ (eoftoken->alias->tag, "$end");
      எனில் (!eof_is_user_defined)
        eoftoken->alias->translatable = true;
      undeftoken->alias->translatable = true;
      errtoken->alias->translatable = true;
    }
}

/*---------------------------------.
| Initialize relation graph nodes. |
`---------------------------------*/

நிலையான வெறுமை
init_prec_nodes (வெறுமை)
{
  prec_nodes = xcalloc (nsyms, அளவு *prec_nodes);
  க்கு (முழு i = 0; i < nsyms; ++i)
    {
      prec_nodes[i] = xmalloc (அளவு *prec_nodes[i]);
      symgraph *s = prec_nodes[i];
      s->id = i;
      s->succ = 0;
      s->pred = 0;
    }
}

/*----------------.
| Create a link.  |
`----------------*/

நிலையான symgraphlink *
symgraphlink_new (graphid id, symgraphlink *next)
{
  symgraphlink *res = xmalloc (அளவு *res);
  res->id = id;
  res->next = next;
  திரும்பு res;
}


/*------------------------------------------------------------------.
| Register the second symbol of the precedence relation, and return |
| whether this relation is new.  Use only in register_precedence.   |
`------------------------------------------------------------------*/

நிலையான bool
register_precedence_second_குறியீடு (symgraphlink **first, graphid sym)
{
  எனில் (!*first || sym < (*first)->id)
    *first = symgraphlink_new (sym, *first);
  இல்லையெனில்
    {
      symgraphlink *slist = *first;

      போது (slist->next && slist->next->id <= sym)
        slist = slist->next;

      எனில் (slist->id == sym)
        /* Relation already present. */
        திரும்பு false;

      slist->next = symgraphlink_new (sym, slist->next);
    }
  திரும்பு true;
}

/*------------------------------------------------------------------.
| Register a new relation between symbols as used. The first symbol |
| has a greater precedence than the second one.                     |
`------------------------------------------------------------------*/

வெறுமை
register_precedence (graphid first, graphid snd)
{
  எனில் (!prec_nodes)
    init_prec_nodes ();
  register_precedence_second_குறியீடு (&(prec_nodes[first]->succ), snd);
  register_precedence_second_குறியீடு (&(prec_nodes[snd]->pred), first);
}


/*---------------------------------------.
| Deep clear a linked / adjacency list). |
`---------------------------------------*/

நிலையான வெறுமை
linkedlist_free (symgraphlink *முனை)
{
  எனில் (முனை)
    {
      போது (முனை->next)
        {
          symgraphlink *tmp = முனை->next;
          விடுவி (முனை);
          முனை = tmp;
        }
      விடுவி (முனை);
    }
}

/*----------------------------------------------.
| Clear and destroy association tracking table. |
`----------------------------------------------*/

நிலையான வெறுமை
assoc_free (வெறுமை)
{
  க்கு (முழு i = 0; i < nsyms; ++i)
    {
      linkedlist_free (prec_nodes[i]->pred);
      linkedlist_free (prec_nodes[i]->succ);
      விடுவி (prec_nodes[i]);
    }
  விடுவி (prec_nodes);
}

/*---------------------------------------.
| Initialize association tracking table. |
`---------------------------------------*/

நிலையான வெறுமை
init_assoc (வெறுமை)
{
  used_assoc = xcalloc (nsyms, அளவு *used_assoc);
  க்கு (graphid i = 0; i < nsyms; ++i)
    used_assoc[i] = false;
}

/*------------------------------------------------------------------.
| Test if the associativity for the symbols is defined and useless. |
`------------------------------------------------------------------*/

நிலையான உள்வரி bool
is_assoc_useless (குறியீடு *s)
{
  திரும்பு s
      && s->content->assoc != undef_assoc
      && s->content->assoc != precedence_assoc
      && !used_assoc[s->content->எண்];
}

/*-------------------------------.
| Register a used associativity. |
`-------------------------------*/

வெறுமை
register_assoc (graphid i, graphid j)
{
  எனில் (!used_assoc)
    init_assoc ();
  used_assoc[i] = true;
  used_assoc[j] = true;
}

/*--------------------------------------------------.
| Print a warning for unused precedence relations.  |
`--------------------------------------------------*/

வெறுமை
print_precedence_warnings (வெறுமை)
{
  எனில் (!prec_nodes)
    init_prec_nodes ();
  எனில் (!used_assoc)
    init_assoc ();
  க்கு (முழு i = 0; i < nsyms; ++i)
    {
      குறியீடு *s = symbols[i];
      எனில் (s
          && s->content->prec != 0
          && !prec_nodes[i]->pred
          && !prec_nodes[i]->succ)
        {
          எனில் (is_assoc_useless (s))
            complain (&s->content->prec_loc, Wprecedence,
                      _("useless precedence and associativity for %s"), s->tag);
          இல்லையெனில் எனில் (s->content->assoc == precedence_assoc)
            complain (&s->content->prec_loc, Wprecedence,
                      _("useless precedence for %s"), s->tag);
        }
      இல்லையெனில் எனில் (is_assoc_useless (s))
        complain (&s->content->prec_loc, Wprecedence,
                  _("useless associativity for %s, use %%precedence"), s->tag);
    }
  விடுவி (used_assoc);
  assoc_free ();
}
