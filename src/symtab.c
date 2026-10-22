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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ "symtab.h"

#അന്തര്ഭാവ "system.h"

#അന്തര്ഭാവ <assure.h>
#അന്തര്ഭാവ <fstrcmp.h>
#അന്തര്ഭാവ <hash.h>
#അന്തര്ഭാവ <quote.h>

#അന്തര്ഭാവ "complain.h"
#അന്തര്ഭാവ "getargs.h"
#അന്തര്ഭാവ "gram.h"
#അന്തര്ഭാവ "intprops.h"

/** Undefined token code.  */
#define CODE_UNDEFINED (-1)

/* Undefined symbol number.  */
#define സംഖ്യ_UNDEFINED (-1)


സ്ഥിതം ഘടന hash_table *ചിഹ്നം_table = ശൂന്യ;
സ്ഥിതം ഘടന hash_table *semantic_type_table = ശൂന്യ;

/*----------------------------------------------------------------.
| Symbols sorted by tag.  Allocated by table_sort, after which no |
| more symbols should be created.                                 |
`----------------------------------------------------------------*/

സ്ഥിതം ചിഹ്നം **symbols_sorted = ശൂന്യ;
സ്ഥിതം semantic_type **semantic_types_sorted = ശൂന്യ;


/*------------------------.
| Distinguished symbols.  |
`------------------------*/

ചിഹ്നം *errtoken = ശൂന്യ;
ചിഹ്നം *undeftoken = ശൂന്യ;
ചിഹ്നം *eoftoken = ശൂന്യ;
ചിഹ്നം *acceptsymbol = ശൂന്യ;

/* Precedence relation graph. */
സ്ഥിതം symgraph **prec_nodes;

/* Store which associativity is used.  */
സ്ഥിതം bool *used_assoc = ശൂന്യ;

bool tag_seen = false;


/* Whether SYM was defined by the user.  */

സ്ഥിതം bool
ചിഹ്നം_is_user_defined (ചിഹ്നം *sym)
{
  സ്ഥിരം bool eof_is_user_defined
    = !eoftoken->alias || STRNEQ (eoftoken->alias->tag, "$end");
  തിരികെ sym->tag[0] != '$'
    && (eof_is_user_defined || (sym != eoftoken && sym->alias != errtoken))
    && sym != errtoken && sym->alias != errtoken
    && sym != undeftoken && sym->alias != undeftoken;
}


/*--------------------------.
| Create a new sym_content. |
`--------------------------*/

സ്ഥിതം sym_content *
sym_content_new (ചിഹ്നം *s)
{
  sym_content *res = xmalloc (വലുപ്പം *res);

  res->ചിഹ്നം = s;

  res->type_name = ശൂന്യ;
  res->type_loc = empty_loc;
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < CODE_PROPS_SIZE; ++i)
    code_props_none_init (&res->props[i]);

  res->സംഖ്യ = സംഖ്യ_UNDEFINED;
  res->prec_loc = empty_loc;
  res->prec = 0;
  res->assoc = undef_assoc;
  res->code = CODE_UNDEFINED;

  res->class = unknown_sym;
  res->status = undeclared;

  തിരികെ res;
}

/*---------------------------------.
| Create a new symbol, named TAG.  |
`---------------------------------*/

സ്ഥിതം ചിഹ്നം *
ചിഹ്നം_new (uniqstr tag, location loc)
{
  ചിഹ്നം *res = xmalloc (വലുപ്പം *res);
  uniqstr_assert (tag);

  /* If the tag is not a string (starts with a double quote), check
     that it is valid for Yacc. */
  എങ്കിൽ (tag[0] != '\"' && tag[0] != '\'' && strchr (tag, '-'))
    complain (&loc, Wyacc,
              _("POSIX Yacc forbids dashes in symbol names: %s"), tag);

  res->tag = tag;
  res->location = loc;
  res->translatable = false;
  res->location_of_lhs = false;
  res->alias = ശൂന്യ;
  res->content = sym_content_new (res);
  res->is_alias = false;
  തിരികെ res;
}

/*--------------------.
| Free a sym_content. |
`--------------------*/

സ്ഥിതം ശൂന്യം
sym_content_free (sym_content *sym)
{
  സ്വതന്ത്രം (sym);
}


/*---------------------------------------------------------.
| Free a symbol and its associated content if appropriate. |
`---------------------------------------------------------*/

സ്ഥിതം ശൂന്യം
ചിഹ്നം_free (ശൂന്യം *ptr)
{
  ചിഹ്നം *sym = (ചിഹ്നം *)ptr;
  എങ്കിൽ (!sym->is_alias)
    sym_content_free (sym->content);
  സ്വതന്ത്രം (sym);

}

/* If needed, swap first and second so that first has the earliest
   location (according to location_cmp).

   Many symbol features (e.g., token codes) are not assigned during
   parsing, but in a second step, via a traversal of the symbol table
   sorted on tag.

   However, error messages make more sense if we keep the first
   declaration first.
*/

സ്ഥിതം ശൂന്യം
symbols_sort (സ്ഥിരം ചിഹ്നം **first, സ്ഥിരം ചിഹ്നം **second)
{
  എങ്കിൽ (0 < location_cmp ((*first)->location, (*second)->location))
    {
      സ്ഥിരം ചിഹ്നം* tmp = *first;
      *first = *second;
      *second = tmp;
    }
}

/* Likewise, for locations.  */

സ്ഥിതം ശൂന്യം
locations_sort (location *first, location *second)
{
  എങ്കിൽ (0 < location_cmp (*first, *second))
    {
      location tmp = *first;
      *first = *second;
      *second = tmp;
    }
}

അക്ഷരം സ്ഥിരം *
code_props_type_string (code_props_type kind)
{
  തിരഞ്ഞെടുപ്പ് (kind)
    {
    അവസ്ഥ destructor:
      തിരികെ "%destructor";
    അവസ്ഥ printer:
      തിരികെ "%printer";
    }
  abort ();
}


/*----------------------------------------.
| Create a new semantic type, named TAG.  |
`----------------------------------------*/

സ്ഥിതം semantic_type *
semantic_type_new (uniqstr tag, സ്ഥിരം location *loc)
{
  semantic_type *res = xmalloc (വലുപ്പം *res);

  uniqstr_assert (tag);
  res->tag = tag;
  res->location = loc ? *loc : empty_loc;
  res->status = undeclared;
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < CODE_PROPS_SIZE; ++i)
    code_props_none_init (&res->props[i]);

  തിരികെ res;
}


/*-----------------.
| Print a symbol.  |
`-----------------*/

#define ചിഹ്നം_INT_ATTR_PRINT(Attr)                     \
  എങ്കിൽ (s->content)                                       \
    fprintf (f, " %s = %d", #Attr, s->content->Attr)

#define ചിഹ്നം_STR_ATTR_PRINT(Attr)                     \
  എങ്കിൽ (s->content && s->content->Attr)                   \
    fprintf (f, " %s { %s }", #Attr, s->content->Attr)

#define ചിഹ്നം_CODE_PRINT(Attr)                                         \
  എങ്കിൽ (s->content && s->content->props[Attr].code)                       \
    fprintf (f, " %s { %s }", #Attr, s->content->props[Attr].code)

ശൂന്യം
ചിഹ്നം_print (ചിഹ്നം സ്ഥിരം *s, FILE *f)
{
  എങ്കിൽ (s)
    {
      ചിഹ്നം_class c = s->content->class;
      fprintf (f, "%s: %s",
               c == unknown_sym    ? "unknown"
               : c == pct_type_sym ? "%type"
               : c == token_sym    ? "token"
               : c == nterm_sym    ? "nterm"
               : ശൂന്യ, /* abort.  */
               s->tag);
      putc (' ', f);
      location_print (s->location, f);
      ചിഹ്നം_INT_ATTR_PRINT (code);
      ചിഹ്നം_INT_ATTR_PRINT (സംഖ്യ);
      ചിഹ്നം_STR_ATTR_PRINT (type_name);
      ചിഹ്നം_CODE_PRINT (destructor);
      ചിഹ്നം_CODE_PRINT (printer);
    }
  അല്ലെങ്കിൽ
    fputs ("<NULL>", f);
}

#undef ചിഹ്നം_ATTR_PRINT
#undef ചിഹ്നം_CODE_PRINT


/*----------------------------------.
| Whether S is a valid identifier.  |
`----------------------------------*/

സ്ഥിതം bool
is_identifier (uniqstr s)
{
  സ്ഥിതം അക്ഷരം സ്ഥിരം alphanum[26 + 26 + 1 + 10] =
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "_"
    "0123456789";
  എങ്കിൽ (!s || ! memchr (alphanum, *s, വലുപ്പം alphanum - 10))
    തിരികെ false;
  വേണ്ടി (++s; *s; ++s)
    എങ്കിൽ (! memchr (alphanum, *s, വലുപ്പം alphanum))
      തിരികെ false;
  തിരികെ true;
}


/*-----------------------------------------------.
| Get the identifier associated to this symbol.  |
`-----------------------------------------------*/

uniqstr
ചിഹ്നം_id_get (ചിഹ്നം സ്ഥിരം *sym)
{
  // There's one weird case: YYerror is the alias, and error is the
  // base symbol.  Return YYerror in that case.
  എങ്കിൽ (sym->alias && is_identifier (sym->alias->tag))
    തിരികെ sym->alias->tag;
  അല്ലെങ്കിൽ എങ്കിൽ (is_identifier (sym->tag))
    തിരികെ sym->tag;
  അല്ലെങ്കിൽ
    തിരികെ ശൂന്യ;
}


/*------------------------------------------------------------------.
| Complain that S's WHAT is redeclared at SECOND, and was first set |
| at FIRST.                                                         |
`------------------------------------------------------------------*/

സ്ഥിതം ശൂന്യം
complain_ചിഹ്നം_redeclared (ചിഹ്നം *s, സ്ഥിരം അക്ഷരം *what, location first,
                            location second)
{
  locations_sort (&first, &second);
  complain (&second, complaint, _("%s redeclaration for %s"), what, s->tag);
  subcomplain (&first, complaint, _("previous declaration"));
}

സ്ഥിതം ശൂന്യം
complain_semantic_type_redeclared (semantic_type *s, സ്ഥിരം അക്ഷരം *what, location first,
                                   location second)
{
  locations_sort (&first, &second);
  complain (&second, complaint, _("%s redeclaration for <%s>"), what, s->tag);
  subcomplain (&first, complaint, _("previous declaration"));
}

സ്ഥിതം ശൂന്യം
complain_class_redeclared (ചിഹ്നം *sym, ചിഹ്നം_class class, location second)
{
  complain (&second, complaint,
            class == token_sym
            ? _("symbol %s redeclared as a token")
            : _("symbol %s redeclared as a nonterminal"), sym->tag);
  എങ്കിൽ (!location_empty (sym->location))
    subcomplain (&sym->location, complaint, _("previous definition"));
}

സ്ഥിതം സ്ഥിരം ചിഹ്നം *
ചിഹ്നം_from_uniqstr_fuzzy (സ്ഥിരം uniqstr താക്കോൽ)
{
  aver (symbols_sorted);
#define FSTRCMP_THRESHOLD 0.6
  ഇരട്ട best_similarity = FSTRCMP_THRESHOLD;
  സ്ഥിരം ചിഹ്നം *res = ശൂന്യ;
  size_t എണ്ണം = hash_get_n_entries (ചിഹ്നം_table);
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; ++i)
    {
      ചിഹ്നം *sym = symbols_sorted[i];
      എങ്കിൽ (STRNEQ (താക്കോൽ, sym->tag)
          && (sym->content->status == declared
              || sym->content->status == undeclared))
        {
          ഇരട്ട similarity = fstrcmp_bounded (താക്കോൽ, sym->tag, best_similarity);
          എങ്കിൽ (best_similarity < similarity)
            {
              res = sym;
              best_similarity = similarity;
            }
        }
    }
  തിരികെ res;
}

സ്ഥിതം ശൂന്യം
complain_ചിഹ്നം_undeclared (സ്ഥിരം ചിഹ്നം *sym)
{
  പ്രതിജ്ഞാ (sym->content->status != declared);
  സ്ഥിരം ചിഹ്നം *best = ചിഹ്നം_from_uniqstr_fuzzy (sym->tag);
  എങ്കിൽ (best)
    {
      complain (&sym->location,
                sym->content->status == needed ? complaint : Wother,
                _("symbol %s is used, but is not defined as a token"
                  " and has no rules; did you mean %s?"),
                quote_n (0, sym->tag),
                quote_n (1, best->tag));
      എങ്കിൽ (feature_flag & feature_caret)
        location_caret_suggestion (sym->location, best->tag, stderr);
    }
  അല്ലെങ്കിൽ
    complain (&sym->location,
              sym->content->status == needed ? complaint : Wother,
              _("symbol %s is used, but is not defined as a token"
                " and has no rules"),
              quote (sym->tag));
}

ശൂന്യം
ചിഹ്നം_location_as_lhs_set (ചിഹ്നം *sym, location loc)
{
  എങ്കിൽ (!sym->location_of_lhs)
    {
      sym->location = loc;
      sym->location_of_lhs = true;
    }
}


/*-----------------------------------------------------------------.
| Set the TYPE_NAME associated with SYM.  Does nothing if passed 0 |
| as TYPE_NAME.                                                    |
`-----------------------------------------------------------------*/

ശൂന്യം
ചിഹ്നം_type_set (ചിഹ്നം *sym, uniqstr type_name, location loc)
{
  എങ്കിൽ (type_name)
    {
      tag_seen = true;
      എങ്കിൽ (sym->content->type_name)
        complain_ചിഹ്നം_redeclared (sym, "%type",
                                    sym->content->type_loc, loc);
      അല്ലെങ്കിൽ
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

ശൂന്യം
ചിഹ്നം_code_props_set (ചിഹ്നം *sym, code_props_type kind,
                       code_props സ്ഥിരം *code)
{
  എങ്കിൽ (sym->content->props[kind].code)
    complain_ചിഹ്നം_redeclared (sym, code_props_type_string (kind),
                                sym->content->props[kind].location,
                                code->location);
  അല്ലെങ്കിൽ
    sym->content->props[kind] = *code;
}


/*-----------------------------------------------------.
| Set the DESTRUCTOR or PRINTER associated with TYPE.  |
`-----------------------------------------------------*/

ശൂന്യം
semantic_type_code_props_set (semantic_type *type,
                              code_props_type kind,
                              code_props സ്ഥിരം *code)
{
  എങ്കിൽ (type->props[kind].code)
    complain_semantic_type_redeclared (type, code_props_type_string (kind),
                                       type->props[kind].location,
                                       code->location);
  അല്ലെങ്കിൽ
    type->props[kind] = *code;
}

/*---------------------------------------------------.
| Get the computed %destructor or %printer for SYM.  |
`---------------------------------------------------*/

code_props *
ചിഹ്നം_code_props_get (ചിഹ്നം *sym, code_props_type kind)
{
  /* Per-symbol code props.  */
  എങ്കിൽ (sym->content->props[kind].code)
    തിരികെ &sym->content->props[kind];

  /* Per-type code props.  */
  എങ്കിൽ (sym->content->type_name)
    {
      code_props *code =
        &semantic_type_get (sym->content->type_name, ശൂന്യ)->props[kind];
      എങ്കിൽ (code->code)
        തിരികെ code;
    }

  /* Apply default code props's only to user-defined symbols.  */
  എങ്കിൽ (ചിഹ്നം_is_user_defined (sym))
    {
      code_props *code = &semantic_type_get (sym->content->type_name ? "*" : "",
                                             ശൂന്യ)->props[kind];
      എങ്കിൽ (code->code)
        തിരികെ code;
    }
  തിരികെ &code_props_none;
}

/*-----------------------------------------------------------------.
| Set the PRECEDENCE associated with SYM.  Does nothing if invoked |
| with UNDEF_ASSOC as ASSOC.                                       |
`-----------------------------------------------------------------*/

ശൂന്യം
ചിഹ്നം_precedence_set (ചിഹ്നം *sym, പൂർണ്ണസംഖ്യ prec, assoc a, location loc)
{
  എങ്കിൽ (a != undef_assoc)
    {
      sym_content *s = sym->content;
      എങ്കിൽ (s->prec)
        complain_ചിഹ്നം_redeclared (sym, assoc_to_string (a),
                                    s->prec_loc, loc);
      അല്ലെങ്കിൽ
        {
          s->prec = prec;
          s->assoc = a;
          s->prec_loc = loc;
        }
    }

  /* Only terminals have a precedence. */
  ചിഹ്നം_class_set (sym, token_sym, loc, false);
}


/*------------------------------------.
| Set the CLASS associated with SYM.  |
`------------------------------------*/

സ്ഥിതം ശൂന്യം
complain_pct_type_on_token (location *loc)
{
  complain (loc, Wyacc,
            _("POSIX yacc reserves %%type to nonterminals"));
}

ശൂന്യം
ചിഹ്നം_class_set (ചിഹ്നം *sym, ചിഹ്നം_class class, location loc, bool declaring)
{
  aver (class != unknown_sym);
  sym_content *s = sym->content;
  എങ്കിൽ (class == pct_type_sym)
    {
      എങ്കിൽ (s->class == token_sym)
        complain_pct_type_on_token (&loc);
      അല്ലെങ്കിൽ എങ്കിൽ (s->class == unknown_sym)
        s->class = class;
    }
  അല്ലെങ്കിൽ എങ്കിൽ (s->class != unknown_sym && s->class != pct_type_sym
           && s->class != class)
    complain_class_redeclared (sym, class, loc);
  അല്ലെങ്കിൽ
    {
      എങ്കിൽ (class == token_sym && s->class == pct_type_sym)
        complain_pct_type_on_token (&sym->location);

      s->class = class;

      എങ്കിൽ (declaring)
        {
          എങ്കിൽ (s->status == declared)
            {
              complain (&loc, Wother,
                        _("symbol %s redeclared"), sym->tag);
              subcomplain (&sym->location, Wother,
                           _("previous declaration"));
            }
          അല്ലെങ്കിൽ
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

ശൂന്യം
ചിഹ്നം_code_set (ചിഹ്നം *sym, പൂർണ്ണസംഖ്യ code, location loc)
{
  പൂർണ്ണസംഖ്യ *codep = &sym->content->code;
  എങ്കിൽ (sym->content->class != token_sym)
    complain (&loc, complaint,
              _("nonterminals cannot be given a token code"));
  അല്ലെങ്കിൽ എങ്കിൽ (*codep != CODE_UNDEFINED
           && *codep != code)
    complain (&loc, complaint, _("redefining code of token %s"),
              sym->tag);
  അല്ലെങ്കിൽ എങ്കിൽ (code == INT_MAX)
    complain (&loc, complaint, _("code of token %s too large"),
              sym->tag);
  അല്ലെങ്കിൽ
    {
      *codep = code;
      /* User defined $end token? */
      എങ്കിൽ (code == 0 && !eoftoken)
        {
          eoftoken = sym->content->ചിഹ്നം;
          eoftoken->content->സംഖ്യ = 0;
        }
    }
}


/*----------------------------------------------------------.
| If SYM is not defined, report an error, and consider it a |
| nonterminal.                                              |
`----------------------------------------------------------*/

സ്ഥിതം ശൂന്യം
ചിഹ്നം_check_defined (ചിഹ്നം *sym)
{
  sym_content *s = sym->content;
  എങ്കിൽ (s->class == unknown_sym || s->class == pct_type_sym)
    {
      complain_ചിഹ്നം_undeclared (sym);
      s->class = nterm_sym;
    }

  എങ്കിൽ (s->സംഖ്യ == സംഖ്യ_UNDEFINED)
    s->സംഖ്യ = s->class == token_sym ? ntokens++ : nnterms++;

  എങ്കിൽ (s->class == token_sym
      && sym->tag[0] == '"'
      && !sym->is_alias)
    complain (&sym->location, Wdangling_alias,
              _("string literal %s not attached to a symbol"),
              sym->tag);

  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < 2; ++i)
    ചിഹ്നം_code_props_get (sym, i)->is_used = true;

  /* Set the semantic type status associated to the current symbol to
     'declared' so that we could check semantic types unnecessary uses. */
  എങ്കിൽ (s->type_name)
    {
      semantic_type *sem_type = semantic_type_get (s->type_name, ശൂന്യ);
      എങ്കിൽ (sem_type)
        sem_type->status = declared;
    }
}

സ്ഥിതം ശൂന്യം
semantic_type_check_defined (semantic_type *sem_type)
{
  /* <*> and <> do not have to be "declared".  */
  എങ്കിൽ (sem_type->status == declared
      || !*sem_type->tag
      || STREQ (sem_type->tag, "*"))
    {
      വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < 2; ++i)
        എങ്കിൽ (sem_type->props[i].kind != CODE_PROPS_NONE
            && ! sem_type->props[i].is_used)
          complain (&sem_type->location, Wother,
                    _("useless %s for type <%s>"),
                    code_props_type_string (i), sem_type->tag);
    }
  അല്ലെങ്കിൽ
    complain (&sem_type->location, Wother,
              _("type <%s> is used, but is not associated to any symbol"),
              sem_type->tag);
}

/*-------------------------------------------------------------------.
| Merge the properties (precedence, associativity, etc.) of SYM, and |
| its string-named alias STR; check consistency.                     |
`-------------------------------------------------------------------*/

സ്ഥിതം ശൂന്യം
ചിഹ്നം_merge_properties (ചിഹ്നം *sym, ചിഹ്നം *str)
{
  എങ്കിൽ (str->content->type_name != sym->content->type_name)
    {
      എങ്കിൽ (str->content->type_name)
        ചിഹ്നം_type_set (sym,
                         str->content->type_name, str->content->type_loc);
      അല്ലെങ്കിൽ
        ചിഹ്നം_type_set (str,
                         sym->content->type_name, sym->content->type_loc);
    }


  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < CODE_PROPS_SIZE; ++i)
    എങ്കിൽ (str->content->props[i].code)
      ചിഹ്നം_code_props_set (sym, i, &str->content->props[i]);
    അല്ലെങ്കിൽ എങ്കിൽ (sym->content->props[i].code)
      ചിഹ്നം_code_props_set (str, i, &sym->content->props[i]);

  എങ്കിൽ (sym->content->prec || str->content->prec)
    {
      എങ്കിൽ (str->content->prec)
        ചിഹ്നം_precedence_set (sym, str->content->prec, str->content->assoc,
                               str->content->prec_loc);
      അല്ലെങ്കിൽ
        ചിഹ്നം_precedence_set (str, sym->content->prec, sym->content->assoc,
                               sym->content->prec_loc);
    }
}

ശൂന്യം
ചിഹ്നം_make_alias (ചിഹ്നം *sym, ചിഹ്നം *str, location loc)
{
  എങ്കിൽ (sym->content->class != token_sym)
    complain (&loc, complaint,
              _("nonterminals cannot be given a string alias"));
  അല്ലെങ്കിൽ എങ്കിൽ (str->alias)
    complain (&loc, Wother,
              _("symbol %s used more than once as a literal string"), str->tag);
  അല്ലെങ്കിൽ എങ്കിൽ (sym->alias)
    complain (&loc, Wother,
              _("symbol %s given more than one literal string"), sym->tag);
  അല്ലെങ്കിൽ
    {
      ചിഹ്നം_merge_properties (sym, str);
      sym_content_free (str->content);
      str->content = sym->content;
      str->content->ചിഹ്നം = str;
      str->is_alias = true;
      str->alias = sym;
      sym->alias = str;
    }
}


/*-------------------------------------------------------------------.
| Assign a symbol number, and write the definition of the token name |
| into FDEFINES.  Put in SYMBOLS.                                    |
`-------------------------------------------------------------------*/

സ്ഥിതം ശൂന്യം
ചിഹ്നം_pack (ചിഹ്നം *sym)
{
  aver (sym->content->സംഖ്യ != സംഖ്യ_UNDEFINED);
  എങ്കിൽ (sym->content->class == nterm_sym)
    sym->content->സംഖ്യ += ntokens;

  symbols[sym->content->സംഖ്യ] = sym->content->ചിഹ്നം;
}

സ്ഥിതം ശൂന്യം
complain_code_redeclared (പൂർണ്ണസംഖ്യ അക്കം, സ്ഥിരം ചിഹ്നം *first, സ്ഥിരം ചിഹ്നം *second)
{
  symbols_sort (&first, &second);
  complain (&second->location, complaint,
            _("code %d reassigned to token %s"),
            അക്കം, second->tag);
  subcomplain (&first->location, complaint,
               _("previous declaration for %s"),
               first->tag);
}

/*-------------------------------------------------.
| Put SYM in TOKEN_TRANSLATIONS if it is a token.  |
`-------------------------------------------------*/

സ്ഥിതം ശൂന്യം
ചിഹ്നം_translation (സ്ഥിരം ചിഹ്നം *sym)
{
  എങ്കിൽ (sym->content->class == token_sym && !sym->is_alias)
    {
      /* A token whose translation has already been set? */
      എങ്കിൽ (token_translations[sym->content->code]
          != undeftoken->content->സംഖ്യ)
        complain_code_redeclared
          (sym->content->code,
           symbols[token_translations[sym->content->code]], sym);
      അല്ലെങ്കിൽ
        token_translations[sym->content->code]
          = sym->content->സംഖ്യ;
    }
}


/*---------------------------------------.
| Symbol and semantic type hash tables.  |
`---------------------------------------*/

/* Initial capacity of symbol and semantic type hash table.  */
#define HT_INITIAL_CAPACITY 257

സ്ഥിതം വരിയിൽ bool
hash_compare_ചിഹ്നം (സ്ഥിരം ചിഹ്നം *m1, സ്ഥിരം ചിഹ്നം *m2)
{
  /* Since tags are unique, we can compare the pointers themselves.  */
  തിരികെ UNIQSTR_EQ (m1->tag, m2->tag);
}

സ്ഥിതം വരിയിൽ bool
hash_compare_semantic_type (സ്ഥിരം semantic_type *m1, സ്ഥിരം semantic_type *m2)
{
  /* Since names are unique, we can compare the pointers themselves.  */
  തിരികെ UNIQSTR_EQ (m1->tag, m2->tag);
}

സ്ഥിതം bool
hash_ചിഹ്നം_comparator (ശൂന്യം സ്ഥിരം *m1, ശൂന്യം സ്ഥിരം *m2)
{
  തിരികെ hash_compare_ചിഹ്നം (m1, m2);
}

സ്ഥിതം bool
hash_semantic_type_comparator (ശൂന്യം സ്ഥിരം *m1, ശൂന്യം സ്ഥിരം *m2)
{
  തിരികെ hash_compare_semantic_type (m1, m2);
}

സ്ഥിതം വരിയിൽ size_t
hash_ചിഹ്നം (സ്ഥിരം ചിഹ്നം *m, size_t tablesize)
{
  /* Since tags are unique, we can hash the pointer itself.  */
  തിരികെ ((uintptr_t) m->tag) % tablesize;
}

സ്ഥിതം വരിയിൽ size_t
hash_semantic_type (സ്ഥിരം semantic_type *m, size_t tablesize)
{
  /* Since names are unique, we can hash the pointer itself.  */
  തിരികെ ((uintptr_t) m->tag) % tablesize;
}

സ്ഥിതം size_t
hash_ചിഹ്നം_hasher (ശൂന്യം സ്ഥിരം *m, size_t tablesize)
{
  തിരികെ hash_ചിഹ്നം (m, tablesize);
}

സ്ഥിതം size_t
hash_semantic_type_hasher (ശൂന്യം സ്ഥിരം *m, size_t tablesize)
{
  തിരികെ hash_semantic_type (m, tablesize);
}

/*-------------------------------.
| Create the symbol hash table.  |
`-------------------------------*/

ശൂന്യം
symbols_new (ശൂന്യം)
{
  ചിഹ്നം_table = hash_xinitialize (HT_INITIAL_CAPACITY,
                                   ശൂന്യ,
                                   hash_ചിഹ്നം_hasher,
                                   hash_ചിഹ്നം_comparator,
                                   ചിഹ്നം_free);

  /* Construct the acceptsymbol symbol. */
  acceptsymbol = ചിഹ്നം_get ("$accept", empty_loc);
  acceptsymbol->content->class = nterm_sym;
  acceptsymbol->content->സംഖ്യ = nnterms++;

  /* Construct the YYerror/"error" token */
  errtoken = ചിഹ്നം_get ("YYerror", empty_loc);
  errtoken->content->class = token_sym;
  errtoken->content->സംഖ്യ = ntokens++;
  {
    ചിഹ്നം *alias = ചിഹ്നം_get ("error", empty_loc);
    ചിഹ്നം_class_set (alias, token_sym, empty_loc, false);
    ചിഹ്നം_make_alias (errtoken, alias, empty_loc);
  }

  /* Construct the YYUNDEF/"$undefined" token that represents all
     undefined literal tokens.  It is always symbol number 2.  */
  undeftoken = ചിഹ്നം_get ("YYUNDEF", empty_loc);
  undeftoken->content->class = token_sym;
  undeftoken->content->സംഖ്യ = ntokens++;
  {
    ചിഹ്നം *alias = ചിഹ്നം_get ("$undefined", empty_loc);
    ചിഹ്നം_class_set (alias, token_sym, empty_loc, false);
    ചിഹ്നം_make_alias (undeftoken, alias, empty_loc);
  }

  semantic_type_table = hash_xinitialize (HT_INITIAL_CAPACITY,
                                          ശൂന്യ,
                                          hash_semantic_type_hasher,
                                          hash_semantic_type_comparator,
                                          സ്വതന്ത്രം);
}


/*----------------------------------------------------------------.
| Find the symbol named KEY, and return it.  If it does not exist |
| yet, create it.                                                 |
`----------------------------------------------------------------*/

ചിഹ്നം *
ചിഹ്നം_from_uniqstr (സ്ഥിരം uniqstr താക്കോൽ, location loc)
{
  ചിഹ്നം probe;

  probe.tag = താക്കോൽ;
  ചിഹ്നം *res = hash_lookup (ചിഹ്നം_table, &probe);

  എങ്കിൽ (!res)
    {
      /* First insertion in the hash. */
      aver (!symbols_sorted);
      res = ചിഹ്നം_new (താക്കോൽ, loc);
      hash_xinsert (ചിഹ്നം_table, res);
    }
  തിരികെ res;
}


/*-----------------------------------------------------------------------.
| Find the semantic type named KEY, and return it.  If it does not exist |
| yet, create it.                                                        |
`-----------------------------------------------------------------------*/

semantic_type *
semantic_type_from_uniqstr (സ്ഥിരം uniqstr താക്കോൽ, സ്ഥിരം location *loc)
{
  semantic_type probe;

  probe.tag = താക്കോൽ;
  semantic_type *res = hash_lookup (semantic_type_table, &probe);

  എങ്കിൽ (!res)
    {
      /* First insertion in the hash. */
      res = semantic_type_new (താക്കോൽ, loc);
      hash_xinsert (semantic_type_table, res);
    }
  തിരികെ res;
}


/*----------------------------------------------------------------.
| Find the symbol named KEY, and return it.  If it does not exist |
| yet, create it.                                                 |
`----------------------------------------------------------------*/

ചിഹ്നം *
ചിഹ്നം_get (സ്ഥിരം അക്ഷരം *താക്കോൽ, location loc)
{
  തിരികെ ചിഹ്നം_from_uniqstr (uniqstr_new (താക്കോൽ), loc);
}


/*-----------------------------------------------------------------------.
| Find the semantic type named KEY, and return it.  If it does not exist |
| yet, create it.                                                        |
`-----------------------------------------------------------------------*/

semantic_type *
semantic_type_get (സ്ഥിരം അക്ഷരം *താക്കോൽ, സ്ഥിരം location *loc)
{
  തിരികെ semantic_type_from_uniqstr (uniqstr_new (താക്കോൽ), loc);
}


/*------------------------------------------------------------------.
| Generate a dummy nonterminal, whose name cannot conflict with the |
| user's names.                                                     |
`------------------------------------------------------------------*/

ചിഹ്നം *
dummy_ചിഹ്നം_get (location loc)
{
  /* Incremented for each generated symbol.  */
  സ്ഥിതം പൂർണ്ണസംഖ്യ dummy_എണ്ണം = 0;
  അക്ഷരം buf[32];
  പൂർണ്ണസംഖ്യ len = snprintf (buf, വലുപ്പം buf, "$@%d", ++dummy_എണ്ണം);
  assure (len < വലുപ്പം buf);
  ചിഹ്നം *sym = ചിഹ്നം_get (buf, loc);
  sym->content->class = nterm_sym;
  തിരികെ sym;
}

bool
ചിഹ്നം_is_dummy (ചിഹ്നം സ്ഥിരം *sym)
{
  തിരികെ sym->tag[0] == '@' || (sym->tag[0] == '$' && sym->tag[1] == '@');
}

/*-------------------.
| Free the symbols.  |
`-------------------*/

ശൂന്യം
symbols_free (ശൂന്യം)
{
  hash_free (ചിഹ്നം_table);
  hash_free (semantic_type_table);
  സ്വതന്ത്രം (symbols);
  സ്വതന്ത്രം (symbols_sorted);
  സ്വതന്ത്രം (semantic_types_sorted);
}


സ്ഥിതം പൂർണ്ണസംഖ്യ
ചിഹ്നം_cmp (ശൂന്യം സ്ഥിരം *a, ശൂന്യം സ്ഥിരം *b)
{
  തിരികെ location_cmp ((*(ചിഹ്നം * സ്ഥിരം *)a)->location,
                       (*(ചിഹ്നം * സ്ഥിരം *)b)->location);
}

/* Store in *SORTED an array of pointers to the symbols contained in
   TABLE, sorted by order of appearance (i.e., by location). */

സ്ഥിതം ശൂന്യം
table_sort (ഘടന hash_table *table, ചിഹ്നം ***sorted)
{
  aver (!*sorted);
  size_t എണ്ണം = hash_get_n_entries (table);
  *sorted = xnmalloc (എണ്ണം + 1, വലുപ്പം **sorted);
  hash_get_entries (table, (ശൂന്യം**)*sorted, എണ്ണം);
  qsort (*sorted, എണ്ണം, വലുപ്പം **sorted, ചിഹ്നം_cmp);
  (*sorted)[എണ്ണം] = ശൂന്യ;
}

/*--------------------------------------------------------------.
| Check that all the symbols are defined.  Report any undefined |
| symbols and consider them nonterminals.                       |
`--------------------------------------------------------------*/

ശൂന്യം
symbols_check_defined (ശൂന്യം)
{
  table_sort (ചിഹ്നം_table, &symbols_sorted);
  /* semantic_type, like symbol, starts with a 'tag' field and then a
     'location' field.  And here we only deal with arrays/hashes of
     pointers, sizeof is not an issue.

     So instead of implementing table_sort (and symbol_cmp) once for
     each type, let's lie a bit to the typing system, and treat
     'semantic_type' as if it were 'symbol'. */
  table_sort (semantic_type_table, (ചിഹ്നം ***) &semantic_types_sorted);

  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; symbols_sorted[i]; ++i)
    ചിഹ്നം_check_defined (symbols_sorted[i]);
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; semantic_types_sorted[i]; ++i)
    semantic_type_check_defined (semantic_types_sorted[i]);
}

/*------------------------------------------------------------------.
| Set TOKEN_TRANSLATIONS.  Check that no two symbols share the same |
| number.                                                           |
`------------------------------------------------------------------*/

സ്ഥിതം ശൂന്യം
symbols_token_translations_init (ശൂന്യം)
{
  bool code_256_available_p = true;

  /* Find the highest token code, and whether 256, the POSIX preferred
     token code for the error token, is used.  */
  max_code = 0;
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < ntokens; ++i)
    {
      sym_content *sym = symbols[i]->content;
      എങ്കിൽ (sym->code != CODE_UNDEFINED)
        {
          എങ്കിൽ (sym->code > max_code)
            max_code = sym->code;
          എങ്കിൽ (sym->code == 256)
            code_256_available_p = false;
        }
    }

  /* If 256 is not used, assign it to error, to follow POSIX.  */
  എങ്കിൽ (code_256_available_p
      && errtoken->content->code == CODE_UNDEFINED)
    errtoken->content->code = 256;

  /* Set the missing codes. */
  എങ്കിൽ (max_code < 256)
    max_code = 256;

  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < ntokens; ++i)
    {
      sym_content *sym = symbols[i]->content;
      എങ്കിൽ (sym->code == CODE_UNDEFINED)
        {
          IGNORE_TYPE_LIMITS_BEGIN
          എങ്കിൽ (INT_ADD_WRAPV (max_code, 1, &max_code))
            complain (ശൂന്യ, fatal, _("token number too large"));
          IGNORE_TYPE_LIMITS_END
          sym->code = max_code;
        }
      എങ്കിൽ (sym->code > max_code)
        max_code = sym->code;
    }

  token_translations = xnmalloc (max_code + 1,
                                 വലുപ്പം *token_translations);

  /* Initialize all entries for literal tokens to the internal token
     number for $undefined, which represents all invalid inputs.  */
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < max_code + 1; ++i)
    token_translations[i] = undeftoken->content->സംഖ്യ;
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; symbols_sorted[i]; ++i)
    ചിഹ്നം_translation (symbols_sorted[i]);
}


/* Whether some symbol requires internationalization.  */
സ്ഥിതം bool
has_translations (ശൂന്യം)
{
  വേണ്ടി (സ്ഥിരം ശൂന്യം *entry = hash_get_first (ചിഹ്നം_table);
       entry;
       entry = hash_get_next (ചിഹ്നം_table, entry))
    {
      സ്ഥിരം ചിഹ്നം *sym = (സ്ഥിരം ചിഹ്നം *) entry;
      എങ്കിൽ (sym->translatable)
        തിരികെ true;
    }
  തിരികെ false;
}

/*----------------------------------------------------------------.
| Assign symbol numbers, and write definition of token names into |
| FDEFINES.  Set up vectors SYMBOL_TABLE, TAGS of symbols.        |
`----------------------------------------------------------------*/

ശൂന്യം
symbols_pack (ശൂന്യം)
{
  symbols = xcalloc (nsyms, വലുപ്പം *symbols);
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; symbols_sorted[i]; ++i)
    ചിഹ്നം_pack (symbols_sorted[i]);

  /* Aliases leave empty slots in symbols, so remove them.  */
  {
    പൂർണ്ണസംഖ്യ nsyms_old = nsyms;
    വേണ്ടി (പൂർണ്ണസംഖ്യ writei = 0, readi = 0; readi < nsyms_old; readi += 1)
      {
        എങ്കിൽ (symbols[readi] == ശൂന്യ)
          {
            nsyms -= 1;
            ntokens -= 1;
          }
        അല്ലെങ്കിൽ
          {
            symbols[writei] = symbols[readi];
            symbols[writei]->content->സംഖ്യ = writei;
            writei += 1;
          }
      }
  }
  symbols = xnrealloc (symbols, nsyms, വലുപ്പം *symbols);

  symbols_token_translations_init ();

  // If some user tokens are internationalized, the internal ones
  // should be too.
  എങ്കിൽ (has_translations ())
    {
      സ്ഥിരം bool eof_is_user_defined
        = !eoftoken->alias || STRNEQ (eoftoken->alias->tag, "$end");
      എങ്കിൽ (!eof_is_user_defined)
        eoftoken->alias->translatable = true;
      undeftoken->alias->translatable = true;
      errtoken->alias->translatable = true;
    }
}

/*---------------------------------.
| Initialize relation graph nodes. |
`---------------------------------*/

സ്ഥിതം ശൂന്യം
init_prec_nodes (ശൂന്യം)
{
  prec_nodes = xcalloc (nsyms, വലുപ്പം *prec_nodes);
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < nsyms; ++i)
    {
      prec_nodes[i] = xmalloc (വലുപ്പം *prec_nodes[i]);
      symgraph *s = prec_nodes[i];
      s->id = i;
      s->succ = 0;
      s->pred = 0;
    }
}

/*----------------.
| Create a link.  |
`----------------*/

സ്ഥിതം symgraphlink *
symgraphlink_new (graphid id, symgraphlink *next)
{
  symgraphlink *res = xmalloc (വലുപ്പം *res);
  res->id = id;
  res->next = next;
  തിരികെ res;
}


/*------------------------------------------------------------------.
| Register the second symbol of the precedence relation, and return |
| whether this relation is new.  Use only in register_precedence.   |
`------------------------------------------------------------------*/

സ്ഥിതം bool
register_precedence_second_ചിഹ്നം (symgraphlink **first, graphid sym)
{
  എങ്കിൽ (!*first || sym < (*first)->id)
    *first = symgraphlink_new (sym, *first);
  അല്ലെങ്കിൽ
    {
      symgraphlink *slist = *first;

      ആകുമ്പോൾ (slist->next && slist->next->id <= sym)
        slist = slist->next;

      എങ്കിൽ (slist->id == sym)
        /* Relation already present. */
        തിരികെ false;

      slist->next = symgraphlink_new (sym, slist->next);
    }
  തിരികെ true;
}

/*------------------------------------------------------------------.
| Register a new relation between symbols as used. The first symbol |
| has a greater precedence than the second one.                     |
`------------------------------------------------------------------*/

ശൂന്യം
register_precedence (graphid first, graphid snd)
{
  എങ്കിൽ (!prec_nodes)
    init_prec_nodes ();
  register_precedence_second_ചിഹ്നം (&(prec_nodes[first]->succ), snd);
  register_precedence_second_ചിഹ്നം (&(prec_nodes[snd]->pred), first);
}


/*---------------------------------------.
| Deep clear a linked / adjacency list). |
`---------------------------------------*/

സ്ഥിതം ശൂന്യം
linkedlist_free (symgraphlink *കണ്ണി)
{
  എങ്കിൽ (കണ്ണി)
    {
      ആകുമ്പോൾ (കണ്ണി->next)
        {
          symgraphlink *tmp = കണ്ണി->next;
          സ്വതന്ത്രം (കണ്ണി);
          കണ്ണി = tmp;
        }
      സ്വതന്ത്രം (കണ്ണി);
    }
}

/*----------------------------------------------.
| Clear and destroy association tracking table. |
`----------------------------------------------*/

സ്ഥിതം ശൂന്യം
assoc_free (ശൂന്യം)
{
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < nsyms; ++i)
    {
      linkedlist_free (prec_nodes[i]->pred);
      linkedlist_free (prec_nodes[i]->succ);
      സ്വതന്ത്രം (prec_nodes[i]);
    }
  സ്വതന്ത്രം (prec_nodes);
}

/*---------------------------------------.
| Initialize association tracking table. |
`---------------------------------------*/

സ്ഥിതം ശൂന്യം
init_assoc (ശൂന്യം)
{
  used_assoc = xcalloc (nsyms, വലുപ്പം *used_assoc);
  വേണ്ടി (graphid i = 0; i < nsyms; ++i)
    used_assoc[i] = false;
}

/*------------------------------------------------------------------.
| Test if the associativity for the symbols is defined and useless. |
`------------------------------------------------------------------*/

സ്ഥിതം വരിയിൽ bool
is_assoc_useless (ചിഹ്നം *s)
{
  തിരികെ s
      && s->content->assoc != undef_assoc
      && s->content->assoc != precedence_assoc
      && !used_assoc[s->content->സംഖ്യ];
}

/*-------------------------------.
| Register a used associativity. |
`-------------------------------*/

ശൂന്യം
register_assoc (graphid i, graphid j)
{
  എങ്കിൽ (!used_assoc)
    init_assoc ();
  used_assoc[i] = true;
  used_assoc[j] = true;
}

/*--------------------------------------------------.
| Print a warning for unused precedence relations.  |
`--------------------------------------------------*/

ശൂന്യം
print_precedence_warnings (ശൂന്യം)
{
  എങ്കിൽ (!prec_nodes)
    init_prec_nodes ();
  എങ്കിൽ (!used_assoc)
    init_assoc ();
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < nsyms; ++i)
    {
      ചിഹ്നം *s = symbols[i];
      എങ്കിൽ (s
          && s->content->prec != 0
          && !prec_nodes[i]->pred
          && !prec_nodes[i]->succ)
        {
          എങ്കിൽ (is_assoc_useless (s))
            complain (&s->content->prec_loc, Wprecedence,
                      _("useless precedence and associativity for %s"), s->tag);
          അല്ലെങ്കിൽ എങ്കിൽ (s->content->assoc == precedence_assoc)
            complain (&s->content->prec_loc, Wprecedence,
                      _("useless precedence for %s"), s->tag);
        }
      അല്ലെങ്കിൽ എങ്കിൽ (is_assoc_useless (s))
        complain (&s->content->prec_loc, Wprecedence,
                  _("useless associativity for %s, use %%precedence"), s->tag);
    }
  സ്വതന്ത്രം (used_assoc);
  assoc_free ();
}
