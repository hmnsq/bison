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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব "symtab.h"

#অন্তর্ভাব "system.h"

#অন্তর্ভাব <assure.h>
#অন্তর্ভাব <fstrcmp.h>
#অন্তর্ভাব <hash.h>
#অন্তর্ভাব <quote.h>

#অন্তর্ভাব "complain.h"
#অন্তর্ভাব "getargs.h"
#অন্তর্ভাব "gram.h"
#অন্তর্ভাব "intprops.h"

/** Undefined token code.  */
#define CODE_UNDEFINED (-1)

/* Undefined symbol number.  */
#define সংখ্যা_UNDEFINED (-1)


স্থির সংরচনা hash_table *প্রতীক_table = রিক্ত;
স্থির সংরচনা hash_table *semantic_type_table = রিক্ত;

/*----------------------------------------------------------------.
| Symbols sorted by tag.  Allocated by table_sort, after which no |
| more symbols should be created.                                 |
`----------------------------------------------------------------*/

স্থির প্রতীক **symbols_sorted = রিক্ত;
স্থির semantic_type **semantic_types_sorted = রিক্ত;


/*------------------------.
| Distinguished symbols.  |
`------------------------*/

প্রতীক *errtoken = রিক্ত;
প্রতীক *undeftoken = রিক্ত;
প্রতীক *eoftoken = রিক্ত;
প্রতীক *acceptsymbol = রিক্ত;

/* Precedence relation graph. */
স্থির symgraph **prec_nodes;

/* Store which associativity is used.  */
স্থির bool *used_assoc = রিক্ত;

bool tag_seen = false;


/* Whether SYM was defined by the user.  */

স্থির bool
প্রতীক_is_user_defined (প্রতীক *sym)
{
  ধ্রুব bool eof_is_user_defined
    = !eoftoken->alias || STRNEQ (eoftoken->alias->tag, "$end");
  প্রত্যাগম sym->tag[0] != '$'
    && (eof_is_user_defined || (sym != eoftoken && sym->alias != errtoken))
    && sym != errtoken && sym->alias != errtoken
    && sym != undeftoken && sym->alias != undeftoken;
}


/*--------------------------.
| Create a new sym_content. |
`--------------------------*/

স্থির sym_content *
sym_content_new (প্রতীক *s)
{
  sym_content *res = xmalloc (পরিমাণ *res);

  res->প্রতীক = s;

  res->type_name = রিক্ত;
  res->type_loc = empty_loc;
  প্রতি (পূর্ণাঙ্ক i = 0; i < CODE_PROPS_SIZE; ++i)
    code_props_none_init (&res->props[i]);

  res->সংখ্যা = সংখ্যা_UNDEFINED;
  res->prec_loc = empty_loc;
  res->prec = 0;
  res->assoc = undef_assoc;
  res->code = CODE_UNDEFINED;

  res->class = unknown_sym;
  res->status = undeclared;

  প্রত্যাগম res;
}

/*---------------------------------.
| Create a new symbol, named TAG.  |
`---------------------------------*/

স্থির প্রতীক *
প্রতীক_new (uniqstr tag, location loc)
{
  প্রতীক *res = xmalloc (পরিমাণ *res);
  uniqstr_assert (tag);

  /* If the tag is not a string (starts with a double quote), check
     that it is valid for Yacc. */
  যদি (tag[0] != '\"' && tag[0] != '\'' && strchr (tag, '-'))
    complain (&loc, Wyacc,
              _("POSIX Yacc forbids dashes in symbol names: %s"), tag);

  res->tag = tag;
  res->location = loc;
  res->translatable = false;
  res->location_of_lhs = false;
  res->alias = রিক্ত;
  res->content = sym_content_new (res);
  res->is_alias = false;
  প্রত্যাগম res;
}

/*--------------------.
| Free a sym_content. |
`--------------------*/

স্থির শূন্য
sym_content_free (sym_content *sym)
{
  মুক্ত (sym);
}


/*---------------------------------------------------------.
| Free a symbol and its associated content if appropriate. |
`---------------------------------------------------------*/

স্থির শূন্য
প্রতীক_free (শূন্য *ptr)
{
  প্রতীক *sym = (প্রতীক *)ptr;
  যদি (!sym->is_alias)
    sym_content_free (sym->content);
  মুক্ত (sym);

}

/* If needed, swap first and second so that first has the earliest
   location (according to location_cmp).

   Many symbol features (e.g., token codes) are not assigned during
   parsing, but in a second step, via a traversal of the symbol table
   sorted on tag.

   However, error messages make more sense if we keep the first
   declaration first.
*/

স্থির শূন্য
symbols_sort (ধ্রুব প্রতীক **first, ধ্রুব প্রতীক **second)
{
  যদি (0 < location_cmp ((*first)->location, (*second)->location))
    {
      ধ্রুব প্রতীক* tmp = *first;
      *first = *second;
      *second = tmp;
    }
}

/* Likewise, for locations.  */

স্থির শূন্য
locations_sort (location *first, location *second)
{
  যদি (0 < location_cmp (*first, *second))
    {
      location tmp = *first;
      *first = *second;
      *second = tmp;
    }
}

বর্ণ ধ্রুব *
code_props_type_string (code_props_type kind)
{
  বিকল্প (kind)
    {
    পক্ষ destructor:
      প্রত্যাগম "%destructor";
    পক্ষ printer:
      প্রত্যাগম "%printer";
    }
  abort ();
}


/*----------------------------------------.
| Create a new semantic type, named TAG.  |
`----------------------------------------*/

স্থির semantic_type *
semantic_type_new (uniqstr tag, ধ্রুব location *loc)
{
  semantic_type *res = xmalloc (পরিমাণ *res);

  uniqstr_assert (tag);
  res->tag = tag;
  res->location = loc ? *loc : empty_loc;
  res->status = undeclared;
  প্রতি (পূর্ণাঙ্ক i = 0; i < CODE_PROPS_SIZE; ++i)
    code_props_none_init (&res->props[i]);

  প্রত্যাগম res;
}


/*-----------------.
| Print a symbol.  |
`-----------------*/

#define প্রতীক_INT_ATTR_PRINT(Attr)                     \
  যদি (s->content)                                       \
    fprintf (f, " %s = %d", #Attr, s->content->Attr)

#define প্রতীক_STR_ATTR_PRINT(Attr)                     \
  যদি (s->content && s->content->Attr)                   \
    fprintf (f, " %s { %s }", #Attr, s->content->Attr)

#define প্রতীক_CODE_PRINT(Attr)                                         \
  যদি (s->content && s->content->props[Attr].code)                       \
    fprintf (f, " %s { %s }", #Attr, s->content->props[Attr].code)

শূন্য
প্রতীক_print (প্রতীক ধ্রুব *s, FILE *f)
{
  যদি (s)
    {
      প্রতীক_class c = s->content->class;
      fprintf (f, "%s: %s",
               c == unknown_sym    ? "unknown"
               : c == pct_type_sym ? "%type"
               : c == token_sym    ? "token"
               : c == nterm_sym    ? "nterm"
               : রিক্ত, /* abort.  */
               s->tag);
      putc (' ', f);
      location_print (s->location, f);
      প্রতীক_INT_ATTR_PRINT (code);
      প্রতীক_INT_ATTR_PRINT (সংখ্যা);
      প্রতীক_STR_ATTR_PRINT (type_name);
      প্রতীক_CODE_PRINT (destructor);
      প্রতীক_CODE_PRINT (printer);
    }
  অন্যথা
    fputs ("<NULL>", f);
}

#undef প্রতীক_ATTR_PRINT
#undef প্রতীক_CODE_PRINT


/*----------------------------------.
| Whether S is a valid identifier.  |
`----------------------------------*/

স্থির bool
is_identifier (uniqstr s)
{
  স্থির বর্ণ ধ্রুব alphanum[26 + 26 + 1 + 10] =
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "_"
    "0123456789";
  যদি (!s || ! memchr (alphanum, *s, পরিমাণ alphanum - 10))
    প্রত্যাগম false;
  প্রতি (++s; *s; ++s)
    যদি (! memchr (alphanum, *s, পরিমাণ alphanum))
      প্রত্যাগম false;
  প্রত্যাগম true;
}


/*-----------------------------------------------.
| Get the identifier associated to this symbol.  |
`-----------------------------------------------*/

uniqstr
প্রতীক_id_get (প্রতীক ধ্রুব *sym)
{
  // There's one weird case: YYerror is the alias, and error is the
  // base symbol.  Return YYerror in that case.
  যদি (sym->alias && is_identifier (sym->alias->tag))
    প্রত্যাগম sym->alias->tag;
  অন্যথা যদি (is_identifier (sym->tag))
    প্রত্যাগম sym->tag;
  অন্যথা
    প্রত্যাগম রিক্ত;
}


/*------------------------------------------------------------------.
| Complain that S's WHAT is redeclared at SECOND, and was first set |
| at FIRST.                                                         |
`------------------------------------------------------------------*/

স্থির শূন্য
complain_প্রতীক_redeclared (প্রতীক *s, ধ্রুব বর্ণ *what, location first,
                            location second)
{
  locations_sort (&first, &second);
  complain (&second, complaint, _("%s redeclaration for %s"), what, s->tag);
  subcomplain (&first, complaint, _("previous declaration"));
}

স্থির শূন্য
complain_semantic_type_redeclared (semantic_type *s, ধ্রুব বর্ণ *what, location first,
                                   location second)
{
  locations_sort (&first, &second);
  complain (&second, complaint, _("%s redeclaration for <%s>"), what, s->tag);
  subcomplain (&first, complaint, _("previous declaration"));
}

স্থির শূন্য
complain_class_redeclared (প্রতীক *sym, প্রতীক_class class, location second)
{
  complain (&second, complaint,
            class == token_sym
            ? _("symbol %s redeclared as a token")
            : _("symbol %s redeclared as a nonterminal"), sym->tag);
  যদি (!location_empty (sym->location))
    subcomplain (&sym->location, complaint, _("previous definition"));
}

স্থির ধ্রুব প্রতীক *
প্রতীক_from_uniqstr_fuzzy (ধ্রুব uniqstr চাবি)
{
  aver (symbols_sorted);
#define FSTRCMP_THRESHOLD 0.6
  দ্বিগুণ best_similarity = FSTRCMP_THRESHOLD;
  ধ্রুব প্রতীক *res = রিক্ত;
  size_t গুনতি = hash_get_n_entries (প্রতীক_table);
  প্রতি (পূর্ণাঙ্ক i = 0; i < গুনতি; ++i)
    {
      প্রতীক *sym = symbols_sorted[i];
      যদি (STRNEQ (চাবি, sym->tag)
          && (sym->content->status == declared
              || sym->content->status == undeclared))
        {
          দ্বিগুণ similarity = fstrcmp_bounded (চাবি, sym->tag, best_similarity);
          যদি (best_similarity < similarity)
            {
              res = sym;
              best_similarity = similarity;
            }
        }
    }
  প্রত্যাগম res;
}

স্থির শূন্য
complain_প্রতীক_undeclared (ধ্রুব প্রতীক *sym)
{
  প্রতিজ্ঞা (sym->content->status != declared);
  ধ্রুব প্রতীক *best = প্রতীক_from_uniqstr_fuzzy (sym->tag);
  যদি (best)
    {
      complain (&sym->location,
                sym->content->status == needed ? complaint : Wother,
                _("symbol %s is used, but is not defined as a token"
                  " and has no rules; did you mean %s?"),
                quote_n (0, sym->tag),
                quote_n (1, best->tag));
      যদি (feature_flag & feature_caret)
        location_caret_suggestion (sym->location, best->tag, stderr);
    }
  অন্যথা
    complain (&sym->location,
              sym->content->status == needed ? complaint : Wother,
              _("symbol %s is used, but is not defined as a token"
                " and has no rules"),
              quote (sym->tag));
}

শূন্য
প্রতীক_location_as_lhs_set (প্রতীক *sym, location loc)
{
  যদি (!sym->location_of_lhs)
    {
      sym->location = loc;
      sym->location_of_lhs = true;
    }
}


/*-----------------------------------------------------------------.
| Set the TYPE_NAME associated with SYM.  Does nothing if passed 0 |
| as TYPE_NAME.                                                    |
`-----------------------------------------------------------------*/

শূন্য
প্রতীক_type_set (প্রতীক *sym, uniqstr type_name, location loc)
{
  যদি (type_name)
    {
      tag_seen = true;
      যদি (sym->content->type_name)
        complain_প্রতীক_redeclared (sym, "%type",
                                    sym->content->type_loc, loc);
      অন্যথা
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

শূন্য
প্রতীক_code_props_set (প্রতীক *sym, code_props_type kind,
                       code_props ধ্রুব *code)
{
  যদি (sym->content->props[kind].code)
    complain_প্রতীক_redeclared (sym, code_props_type_string (kind),
                                sym->content->props[kind].location,
                                code->location);
  অন্যথা
    sym->content->props[kind] = *code;
}


/*-----------------------------------------------------.
| Set the DESTRUCTOR or PRINTER associated with TYPE.  |
`-----------------------------------------------------*/

শূন্য
semantic_type_code_props_set (semantic_type *type,
                              code_props_type kind,
                              code_props ধ্রুব *code)
{
  যদি (type->props[kind].code)
    complain_semantic_type_redeclared (type, code_props_type_string (kind),
                                       type->props[kind].location,
                                       code->location);
  অন্যথা
    type->props[kind] = *code;
}

/*---------------------------------------------------.
| Get the computed %destructor or %printer for SYM.  |
`---------------------------------------------------*/

code_props *
প্রতীক_code_props_get (প্রতীক *sym, code_props_type kind)
{
  /* Per-symbol code props.  */
  যদি (sym->content->props[kind].code)
    প্রত্যাগম &sym->content->props[kind];

  /* Per-type code props.  */
  যদি (sym->content->type_name)
    {
      code_props *code =
        &semantic_type_get (sym->content->type_name, রিক্ত)->props[kind];
      যদি (code->code)
        প্রত্যাগম code;
    }

  /* Apply default code props's only to user-defined symbols.  */
  যদি (প্রতীক_is_user_defined (sym))
    {
      code_props *code = &semantic_type_get (sym->content->type_name ? "*" : "",
                                             রিক্ত)->props[kind];
      যদি (code->code)
        প্রত্যাগম code;
    }
  প্রত্যাগম &code_props_none;
}

/*-----------------------------------------------------------------.
| Set the PRECEDENCE associated with SYM.  Does nothing if invoked |
| with UNDEF_ASSOC as ASSOC.                                       |
`-----------------------------------------------------------------*/

শূন্য
প্রতীক_precedence_set (প্রতীক *sym, পূর্ণাঙ্ক prec, assoc a, location loc)
{
  যদি (a != undef_assoc)
    {
      sym_content *s = sym->content;
      যদি (s->prec)
        complain_প্রতীক_redeclared (sym, assoc_to_string (a),
                                    s->prec_loc, loc);
      অন্যথা
        {
          s->prec = prec;
          s->assoc = a;
          s->prec_loc = loc;
        }
    }

  /* Only terminals have a precedence. */
  প্রতীক_class_set (sym, token_sym, loc, false);
}


/*------------------------------------.
| Set the CLASS associated with SYM.  |
`------------------------------------*/

স্থির শূন্য
complain_pct_type_on_token (location *loc)
{
  complain (loc, Wyacc,
            _("POSIX yacc reserves %%type to nonterminals"));
}

শূন্য
প্রতীক_class_set (প্রতীক *sym, প্রতীক_class class, location loc, bool declaring)
{
  aver (class != unknown_sym);
  sym_content *s = sym->content;
  যদি (class == pct_type_sym)
    {
      যদি (s->class == token_sym)
        complain_pct_type_on_token (&loc);
      অন্যথা যদি (s->class == unknown_sym)
        s->class = class;
    }
  অন্যথা যদি (s->class != unknown_sym && s->class != pct_type_sym
           && s->class != class)
    complain_class_redeclared (sym, class, loc);
  অন্যথা
    {
      যদি (class == token_sym && s->class == pct_type_sym)
        complain_pct_type_on_token (&sym->location);

      s->class = class;

      যদি (declaring)
        {
          যদি (s->status == declared)
            {
              complain (&loc, Wother,
                        _("symbol %s redeclared"), sym->tag);
              subcomplain (&sym->location, Wother,
                           _("previous declaration"));
            }
          অন্যথা
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

শূন্য
প্রতীক_code_set (প্রতীক *sym, পূর্ণাঙ্ক code, location loc)
{
  পূর্ণাঙ্ক *codep = &sym->content->code;
  যদি (sym->content->class != token_sym)
    complain (&loc, complaint,
              _("nonterminals cannot be given a token code"));
  অন্যথা যদি (*codep != CODE_UNDEFINED
           && *codep != code)
    complain (&loc, complaint, _("redefining code of token %s"),
              sym->tag);
  অন্যথা যদি (code == INT_MAX)
    complain (&loc, complaint, _("code of token %s too large"),
              sym->tag);
  অন্যথা
    {
      *codep = code;
      /* User defined $end token? */
      যদি (code == 0 && !eoftoken)
        {
          eoftoken = sym->content->প্রতীক;
          eoftoken->content->সংখ্যা = 0;
        }
    }
}


/*----------------------------------------------------------.
| If SYM is not defined, report an error, and consider it a |
| nonterminal.                                              |
`----------------------------------------------------------*/

স্থির শূন্য
প্রতীক_check_defined (প্রতীক *sym)
{
  sym_content *s = sym->content;
  যদি (s->class == unknown_sym || s->class == pct_type_sym)
    {
      complain_প্রতীক_undeclared (sym);
      s->class = nterm_sym;
    }

  যদি (s->সংখ্যা == সংখ্যা_UNDEFINED)
    s->সংখ্যা = s->class == token_sym ? ntokens++ : nnterms++;

  যদি (s->class == token_sym
      && sym->tag[0] == '"'
      && !sym->is_alias)
    complain (&sym->location, Wdangling_alias,
              _("string literal %s not attached to a symbol"),
              sym->tag);

  প্রতি (পূর্ণাঙ্ক i = 0; i < 2; ++i)
    প্রতীক_code_props_get (sym, i)->is_used = true;

  /* Set the semantic type status associated to the current symbol to
     'declared' so that we could check semantic types unnecessary uses. */
  যদি (s->type_name)
    {
      semantic_type *sem_type = semantic_type_get (s->type_name, রিক্ত);
      যদি (sem_type)
        sem_type->status = declared;
    }
}

স্থির শূন্য
semantic_type_check_defined (semantic_type *sem_type)
{
  /* <*> and <> do not have to be "declared".  */
  যদি (sem_type->status == declared
      || !*sem_type->tag
      || STREQ (sem_type->tag, "*"))
    {
      প্রতি (পূর্ণাঙ্ক i = 0; i < 2; ++i)
        যদি (sem_type->props[i].kind != CODE_PROPS_NONE
            && ! sem_type->props[i].is_used)
          complain (&sem_type->location, Wother,
                    _("useless %s for type <%s>"),
                    code_props_type_string (i), sem_type->tag);
    }
  অন্যথা
    complain (&sem_type->location, Wother,
              _("type <%s> is used, but is not associated to any symbol"),
              sem_type->tag);
}

/*-------------------------------------------------------------------.
| Merge the properties (precedence, associativity, etc.) of SYM, and |
| its string-named alias STR; check consistency.                     |
`-------------------------------------------------------------------*/

স্থির শূন্য
প্রতীক_merge_properties (প্রতীক *sym, প্রতীক *str)
{
  যদি (str->content->type_name != sym->content->type_name)
    {
      যদি (str->content->type_name)
        প্রতীক_type_set (sym,
                         str->content->type_name, str->content->type_loc);
      অন্যথা
        প্রতীক_type_set (str,
                         sym->content->type_name, sym->content->type_loc);
    }


  প্রতি (পূর্ণাঙ্ক i = 0; i < CODE_PROPS_SIZE; ++i)
    যদি (str->content->props[i].code)
      প্রতীক_code_props_set (sym, i, &str->content->props[i]);
    অন্যথা যদি (sym->content->props[i].code)
      প্রতীক_code_props_set (str, i, &sym->content->props[i]);

  যদি (sym->content->prec || str->content->prec)
    {
      যদি (str->content->prec)
        প্রতীক_precedence_set (sym, str->content->prec, str->content->assoc,
                               str->content->prec_loc);
      অন্যথা
        প্রতীক_precedence_set (str, sym->content->prec, sym->content->assoc,
                               sym->content->prec_loc);
    }
}

শূন্য
প্রতীক_make_alias (প্রতীক *sym, প্রতীক *str, location loc)
{
  যদি (sym->content->class != token_sym)
    complain (&loc, complaint,
              _("nonterminals cannot be given a string alias"));
  অন্যথা যদি (str->alias)
    complain (&loc, Wother,
              _("symbol %s used more than once as a literal string"), str->tag);
  অন্যথা যদি (sym->alias)
    complain (&loc, Wother,
              _("symbol %s given more than one literal string"), sym->tag);
  অন্যথা
    {
      প্রতীক_merge_properties (sym, str);
      sym_content_free (str->content);
      str->content = sym->content;
      str->content->প্রতীক = str;
      str->is_alias = true;
      str->alias = sym;
      sym->alias = str;
    }
}


/*-------------------------------------------------------------------.
| Assign a symbol number, and write the definition of the token name |
| into FDEFINES.  Put in SYMBOLS.                                    |
`-------------------------------------------------------------------*/

স্থির শূন্য
প্রতীক_pack (প্রতীক *sym)
{
  aver (sym->content->সংখ্যা != সংখ্যা_UNDEFINED);
  যদি (sym->content->class == nterm_sym)
    sym->content->সংখ্যা += ntokens;

  symbols[sym->content->সংখ্যা] = sym->content->প্রতীক;
}

স্থির শূন্য
complain_code_redeclared (পূর্ণাঙ্ক অঙ্ক, ধ্রুব প্রতীক *first, ধ্রুব প্রতীক *second)
{
  symbols_sort (&first, &second);
  complain (&second->location, complaint,
            _("code %d reassigned to token %s"),
            অঙ্ক, second->tag);
  subcomplain (&first->location, complaint,
               _("previous declaration for %s"),
               first->tag);
}

/*-------------------------------------------------.
| Put SYM in TOKEN_TRANSLATIONS if it is a token.  |
`-------------------------------------------------*/

স্থির শূন্য
প্রতীক_translation (ধ্রুব প্রতীক *sym)
{
  যদি (sym->content->class == token_sym && !sym->is_alias)
    {
      /* A token whose translation has already been set? */
      যদি (token_translations[sym->content->code]
          != undeftoken->content->সংখ্যা)
        complain_code_redeclared
          (sym->content->code,
           symbols[token_translations[sym->content->code]], sym);
      অন্যথা
        token_translations[sym->content->code]
          = sym->content->সংখ্যা;
    }
}


/*---------------------------------------.
| Symbol and semantic type hash tables.  |
`---------------------------------------*/

/* Initial capacity of symbol and semantic type hash table.  */
#define HT_INITIAL_CAPACITY 257

স্থির অন্তঃস্থ bool
hash_compare_প্রতীক (ধ্রুব প্রতীক *m1, ধ্রুব প্রতীক *m2)
{
  /* Since tags are unique, we can compare the pointers themselves.  */
  প্রত্যাগম UNIQSTR_EQ (m1->tag, m2->tag);
}

স্থির অন্তঃস্থ bool
hash_compare_semantic_type (ধ্রুব semantic_type *m1, ধ্রুব semantic_type *m2)
{
  /* Since names are unique, we can compare the pointers themselves.  */
  প্রত্যাগম UNIQSTR_EQ (m1->tag, m2->tag);
}

স্থির bool
hash_প্রতীক_comparator (শূন্য ধ্রুব *m1, শূন্য ধ্রুব *m2)
{
  প্রত্যাগম hash_compare_প্রতীক (m1, m2);
}

স্থির bool
hash_semantic_type_comparator (শূন্য ধ্রুব *m1, শূন্য ধ্রুব *m2)
{
  প্রত্যাগম hash_compare_semantic_type (m1, m2);
}

স্থির অন্তঃস্থ size_t
hash_প্রতীক (ধ্রুব প্রতীক *m, size_t tablesize)
{
  /* Since tags are unique, we can hash the pointer itself.  */
  প্রত্যাগম ((uintptr_t) m->tag) % tablesize;
}

স্থির অন্তঃস্থ size_t
hash_semantic_type (ধ্রুব semantic_type *m, size_t tablesize)
{
  /* Since names are unique, we can hash the pointer itself.  */
  প্রত্যাগম ((uintptr_t) m->tag) % tablesize;
}

স্থির size_t
hash_প্রতীক_hasher (শূন্য ধ্রুব *m, size_t tablesize)
{
  প্রত্যাগম hash_প্রতীক (m, tablesize);
}

স্থির size_t
hash_semantic_type_hasher (শূন্য ধ্রুব *m, size_t tablesize)
{
  প্রত্যাগম hash_semantic_type (m, tablesize);
}

/*-------------------------------.
| Create the symbol hash table.  |
`-------------------------------*/

শূন্য
symbols_new (শূন্য)
{
  প্রতীক_table = hash_xinitialize (HT_INITIAL_CAPACITY,
                                   রিক্ত,
                                   hash_প্রতীক_hasher,
                                   hash_প্রতীক_comparator,
                                   প্রতীক_free);

  /* Construct the acceptsymbol symbol. */
  acceptsymbol = প্রতীক_get ("$accept", empty_loc);
  acceptsymbol->content->class = nterm_sym;
  acceptsymbol->content->সংখ্যা = nnterms++;

  /* Construct the YYerror/"error" token */
  errtoken = প্রতীক_get ("YYerror", empty_loc);
  errtoken->content->class = token_sym;
  errtoken->content->সংখ্যা = ntokens++;
  {
    প্রতীক *alias = প্রতীক_get ("error", empty_loc);
    প্রতীক_class_set (alias, token_sym, empty_loc, false);
    প্রতীক_make_alias (errtoken, alias, empty_loc);
  }

  /* Construct the YYUNDEF/"$undefined" token that represents all
     undefined literal tokens.  It is always symbol number 2.  */
  undeftoken = প্রতীক_get ("YYUNDEF", empty_loc);
  undeftoken->content->class = token_sym;
  undeftoken->content->সংখ্যা = ntokens++;
  {
    প্রতীক *alias = প্রতীক_get ("$undefined", empty_loc);
    প্রতীক_class_set (alias, token_sym, empty_loc, false);
    প্রতীক_make_alias (undeftoken, alias, empty_loc);
  }

  semantic_type_table = hash_xinitialize (HT_INITIAL_CAPACITY,
                                          রিক্ত,
                                          hash_semantic_type_hasher,
                                          hash_semantic_type_comparator,
                                          মুক্ত);
}


/*----------------------------------------------------------------.
| Find the symbol named KEY, and return it.  If it does not exist |
| yet, create it.                                                 |
`----------------------------------------------------------------*/

প্রতীক *
প্রতীক_from_uniqstr (ধ্রুব uniqstr চাবি, location loc)
{
  প্রতীক probe;

  probe.tag = চাবি;
  প্রতীক *res = hash_lookup (প্রতীক_table, &probe);

  যদি (!res)
    {
      /* First insertion in the hash. */
      aver (!symbols_sorted);
      res = প্রতীক_new (চাবি, loc);
      hash_xinsert (প্রতীক_table, res);
    }
  প্রত্যাগম res;
}


/*-----------------------------------------------------------------------.
| Find the semantic type named KEY, and return it.  If it does not exist |
| yet, create it.                                                        |
`-----------------------------------------------------------------------*/

semantic_type *
semantic_type_from_uniqstr (ধ্রুব uniqstr চাবি, ধ্রুব location *loc)
{
  semantic_type probe;

  probe.tag = চাবি;
  semantic_type *res = hash_lookup (semantic_type_table, &probe);

  যদি (!res)
    {
      /* First insertion in the hash. */
      res = semantic_type_new (চাবি, loc);
      hash_xinsert (semantic_type_table, res);
    }
  প্রত্যাগম res;
}


/*----------------------------------------------------------------.
| Find the symbol named KEY, and return it.  If it does not exist |
| yet, create it.                                                 |
`----------------------------------------------------------------*/

প্রতীক *
প্রতীক_get (ধ্রুব বর্ণ *চাবি, location loc)
{
  প্রত্যাগম প্রতীক_from_uniqstr (uniqstr_new (চাবি), loc);
}


/*-----------------------------------------------------------------------.
| Find the semantic type named KEY, and return it.  If it does not exist |
| yet, create it.                                                        |
`-----------------------------------------------------------------------*/

semantic_type *
semantic_type_get (ধ্রুব বর্ণ *চাবি, ধ্রুব location *loc)
{
  প্রত্যাগম semantic_type_from_uniqstr (uniqstr_new (চাবি), loc);
}


/*------------------------------------------------------------------.
| Generate a dummy nonterminal, whose name cannot conflict with the |
| user's names.                                                     |
`------------------------------------------------------------------*/

প্রতীক *
dummy_প্রতীক_get (location loc)
{
  /* Incremented for each generated symbol.  */
  স্থির পূর্ণাঙ্ক dummy_গুনতি = 0;
  বর্ণ buf[32];
  পূর্ণাঙ্ক len = snprintf (buf, পরিমাণ buf, "$@%d", ++dummy_গুনতি);
  assure (len < পরিমাণ buf);
  প্রতীক *sym = প্রতীক_get (buf, loc);
  sym->content->class = nterm_sym;
  প্রত্যাগম sym;
}

bool
প্রতীক_is_dummy (প্রতীক ধ্রুব *sym)
{
  প্রত্যাগম sym->tag[0] == '@' || (sym->tag[0] == '$' && sym->tag[1] == '@');
}

/*-------------------.
| Free the symbols.  |
`-------------------*/

শূন্য
symbols_free (শূন্য)
{
  hash_free (প্রতীক_table);
  hash_free (semantic_type_table);
  মুক্ত (symbols);
  মুক্ত (symbols_sorted);
  মুক্ত (semantic_types_sorted);
}


স্থির পূর্ণাঙ্ক
প্রতীক_cmp (শূন্য ধ্রুব *a, শূন্য ধ্রুব *b)
{
  প্রত্যাগম location_cmp ((*(প্রতীক * ধ্রুব *)a)->location,
                       (*(প্রতীক * ধ্রুব *)b)->location);
}

/* Store in *SORTED an array of pointers to the symbols contained in
   TABLE, sorted by order of appearance (i.e., by location). */

স্থির শূন্য
table_sort (সংরচনা hash_table *table, প্রতীক ***sorted)
{
  aver (!*sorted);
  size_t গুনতি = hash_get_n_entries (table);
  *sorted = xnmalloc (গুনতি + 1, পরিমাণ **sorted);
  hash_get_entries (table, (শূন্য**)*sorted, গুনতি);
  qsort (*sorted, গুনতি, পরিমাণ **sorted, প্রতীক_cmp);
  (*sorted)[গুনতি] = রিক্ত;
}

/*--------------------------------------------------------------.
| Check that all the symbols are defined.  Report any undefined |
| symbols and consider them nonterminals.                       |
`--------------------------------------------------------------*/

শূন্য
symbols_check_defined (শূন্য)
{
  table_sort (প্রতীক_table, &symbols_sorted);
  /* semantic_type, like symbol, starts with a 'tag' field and then a
     'location' field.  And here we only deal with arrays/hashes of
     pointers, sizeof is not an issue.

     So instead of implementing table_sort (and symbol_cmp) once for
     each type, let's lie a bit to the typing system, and treat
     'semantic_type' as if it were 'symbol'. */
  table_sort (semantic_type_table, (প্রতীক ***) &semantic_types_sorted);

  প্রতি (পূর্ণাঙ্ক i = 0; symbols_sorted[i]; ++i)
    প্রতীক_check_defined (symbols_sorted[i]);
  প্রতি (পূর্ণাঙ্ক i = 0; semantic_types_sorted[i]; ++i)
    semantic_type_check_defined (semantic_types_sorted[i]);
}

/*------------------------------------------------------------------.
| Set TOKEN_TRANSLATIONS.  Check that no two symbols share the same |
| number.                                                           |
`------------------------------------------------------------------*/

স্থির শূন্য
symbols_token_translations_init (শূন্য)
{
  bool code_256_available_p = true;

  /* Find the highest token code, and whether 256, the POSIX preferred
     token code for the error token, is used.  */
  max_code = 0;
  প্রতি (পূর্ণাঙ্ক i = 0; i < ntokens; ++i)
    {
      sym_content *sym = symbols[i]->content;
      যদি (sym->code != CODE_UNDEFINED)
        {
          যদি (sym->code > max_code)
            max_code = sym->code;
          যদি (sym->code == 256)
            code_256_available_p = false;
        }
    }

  /* If 256 is not used, assign it to error, to follow POSIX.  */
  যদি (code_256_available_p
      && errtoken->content->code == CODE_UNDEFINED)
    errtoken->content->code = 256;

  /* Set the missing codes. */
  যদি (max_code < 256)
    max_code = 256;

  প্রতি (পূর্ণাঙ্ক i = 0; i < ntokens; ++i)
    {
      sym_content *sym = symbols[i]->content;
      যদি (sym->code == CODE_UNDEFINED)
        {
          IGNORE_TYPE_LIMITS_BEGIN
          যদি (INT_ADD_WRAPV (max_code, 1, &max_code))
            complain (রিক্ত, fatal, _("token number too large"));
          IGNORE_TYPE_LIMITS_END
          sym->code = max_code;
        }
      যদি (sym->code > max_code)
        max_code = sym->code;
    }

  token_translations = xnmalloc (max_code + 1,
                                 পরিমাণ *token_translations);

  /* Initialize all entries for literal tokens to the internal token
     number for $undefined, which represents all invalid inputs.  */
  প্রতি (পূর্ণাঙ্ক i = 0; i < max_code + 1; ++i)
    token_translations[i] = undeftoken->content->সংখ্যা;
  প্রতি (পূর্ণাঙ্ক i = 0; symbols_sorted[i]; ++i)
    প্রতীক_translation (symbols_sorted[i]);
}


/* Whether some symbol requires internationalization.  */
স্থির bool
has_translations (শূন্য)
{
  প্রতি (ধ্রুব শূন্য *entry = hash_get_first (প্রতীক_table);
       entry;
       entry = hash_get_next (প্রতীক_table, entry))
    {
      ধ্রুব প্রতীক *sym = (ধ্রুব প্রতীক *) entry;
      যদি (sym->translatable)
        প্রত্যাগম true;
    }
  প্রত্যাগম false;
}

/*----------------------------------------------------------------.
| Assign symbol numbers, and write definition of token names into |
| FDEFINES.  Set up vectors SYMBOL_TABLE, TAGS of symbols.        |
`----------------------------------------------------------------*/

শূন্য
symbols_pack (শূন্য)
{
  symbols = xcalloc (nsyms, পরিমাণ *symbols);
  প্রতি (পূর্ণাঙ্ক i = 0; symbols_sorted[i]; ++i)
    প্রতীক_pack (symbols_sorted[i]);

  /* Aliases leave empty slots in symbols, so remove them.  */
  {
    পূর্ণাঙ্ক nsyms_old = nsyms;
    প্রতি (পূর্ণাঙ্ক writei = 0, readi = 0; readi < nsyms_old; readi += 1)
      {
        যদি (symbols[readi] == রিক্ত)
          {
            nsyms -= 1;
            ntokens -= 1;
          }
        অন্যথা
          {
            symbols[writei] = symbols[readi];
            symbols[writei]->content->সংখ্যা = writei;
            writei += 1;
          }
      }
  }
  symbols = xnrealloc (symbols, nsyms, পরিমাণ *symbols);

  symbols_token_translations_init ();

  // If some user tokens are internationalized, the internal ones
  // should be too.
  যদি (has_translations ())
    {
      ধ্রুব bool eof_is_user_defined
        = !eoftoken->alias || STRNEQ (eoftoken->alias->tag, "$end");
      যদি (!eof_is_user_defined)
        eoftoken->alias->translatable = true;
      undeftoken->alias->translatable = true;
      errtoken->alias->translatable = true;
    }
}

/*---------------------------------.
| Initialize relation graph nodes. |
`---------------------------------*/

স্থির শূন্য
init_prec_nodes (শূন্য)
{
  prec_nodes = xcalloc (nsyms, পরিমাণ *prec_nodes);
  প্রতি (পূর্ণাঙ্ক i = 0; i < nsyms; ++i)
    {
      prec_nodes[i] = xmalloc (পরিমাণ *prec_nodes[i]);
      symgraph *s = prec_nodes[i];
      s->id = i;
      s->succ = 0;
      s->pred = 0;
    }
}

/*----------------.
| Create a link.  |
`----------------*/

স্থির symgraphlink *
symgraphlink_new (graphid id, symgraphlink *next)
{
  symgraphlink *res = xmalloc (পরিমাণ *res);
  res->id = id;
  res->next = next;
  প্রত্যাগম res;
}


/*------------------------------------------------------------------.
| Register the second symbol of the precedence relation, and return |
| whether this relation is new.  Use only in register_precedence.   |
`------------------------------------------------------------------*/

স্থির bool
register_precedence_second_প্রতীক (symgraphlink **first, graphid sym)
{
  যদি (!*first || sym < (*first)->id)
    *first = symgraphlink_new (sym, *first);
  অন্যথা
    {
      symgraphlink *slist = *first;

      যাবত্ (slist->next && slist->next->id <= sym)
        slist = slist->next;

      যদি (slist->id == sym)
        /* Relation already present. */
        প্রত্যাগম false;

      slist->next = symgraphlink_new (sym, slist->next);
    }
  প্রত্যাগম true;
}

/*------------------------------------------------------------------.
| Register a new relation between symbols as used. The first symbol |
| has a greater precedence than the second one.                     |
`------------------------------------------------------------------*/

শূন্য
register_precedence (graphid first, graphid snd)
{
  যদি (!prec_nodes)
    init_prec_nodes ();
  register_precedence_second_প্রতীক (&(prec_nodes[first]->succ), snd);
  register_precedence_second_প্রতীক (&(prec_nodes[snd]->pred), first);
}


/*---------------------------------------.
| Deep clear a linked / adjacency list). |
`---------------------------------------*/

স্থির শূন্য
linkedlist_free (symgraphlink *গ্রন্থি)
{
  যদি (গ্রন্থি)
    {
      যাবত্ (গ্রন্থি->next)
        {
          symgraphlink *tmp = গ্রন্থি->next;
          মুক্ত (গ্রন্থি);
          গ্রন্থি = tmp;
        }
      মুক্ত (গ্রন্থি);
    }
}

/*----------------------------------------------.
| Clear and destroy association tracking table. |
`----------------------------------------------*/

স্থির শূন্য
assoc_free (শূন্য)
{
  প্রতি (পূর্ণাঙ্ক i = 0; i < nsyms; ++i)
    {
      linkedlist_free (prec_nodes[i]->pred);
      linkedlist_free (prec_nodes[i]->succ);
      মুক্ত (prec_nodes[i]);
    }
  মুক্ত (prec_nodes);
}

/*---------------------------------------.
| Initialize association tracking table. |
`---------------------------------------*/

স্থির শূন্য
init_assoc (শূন্য)
{
  used_assoc = xcalloc (nsyms, পরিমাণ *used_assoc);
  প্রতি (graphid i = 0; i < nsyms; ++i)
    used_assoc[i] = false;
}

/*------------------------------------------------------------------.
| Test if the associativity for the symbols is defined and useless. |
`------------------------------------------------------------------*/

স্থির অন্তঃস্থ bool
is_assoc_useless (প্রতীক *s)
{
  প্রত্যাগম s
      && s->content->assoc != undef_assoc
      && s->content->assoc != precedence_assoc
      && !used_assoc[s->content->সংখ্যা];
}

/*-------------------------------.
| Register a used associativity. |
`-------------------------------*/

শূন্য
register_assoc (graphid i, graphid j)
{
  যদি (!used_assoc)
    init_assoc ();
  used_assoc[i] = true;
  used_assoc[j] = true;
}

/*--------------------------------------------------.
| Print a warning for unused precedence relations.  |
`--------------------------------------------------*/

শূন্য
print_precedence_warnings (শূন্য)
{
  যদি (!prec_nodes)
    init_prec_nodes ();
  যদি (!used_assoc)
    init_assoc ();
  প্রতি (পূর্ণাঙ্ক i = 0; i < nsyms; ++i)
    {
      প্রতীক *s = symbols[i];
      যদি (s
          && s->content->prec != 0
          && !prec_nodes[i]->pred
          && !prec_nodes[i]->succ)
        {
          যদি (is_assoc_useless (s))
            complain (&s->content->prec_loc, Wprecedence,
                      _("useless precedence and associativity for %s"), s->tag);
          অন্যথা যদি (s->content->assoc == precedence_assoc)
            complain (&s->content->prec_loc, Wprecedence,
                      _("useless precedence for %s"), s->tag);
        }
      অন্যথা যদি (is_assoc_useless (s))
        complain (&s->content->prec_loc, Wprecedence,
                  _("useless associativity for %s, use %%precedence"), s->tag);
    }
  মুক্ত (used_assoc);
  assoc_free ();
}
