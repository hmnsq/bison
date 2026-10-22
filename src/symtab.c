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

#شامل <config.h>
#شامل "symtab.h"

#شامل "system.h"

#شامل <assure.h>
#شامل <fstrcmp.h>
#شامل <hash.h>
#شامل <quote.h>

#شامل "complain.h"
#شامل "getargs.h"
#شامل "gram.h"
#شامل "intprops.h"

/** Undefined token code.  */
#define CODE_UNDEFINED (-1)

/* Undefined symbol number.  */
#define عدد_UNDEFINED (-1)


ایستا ساختار hash_table *نماد_table = پوچ;
ایستا ساختار hash_table *semantic_type_table = پوچ;

/*----------------------------------------------------------------.
| Symbols sorted by tag.  Allocated by table_sort, after which no |
| more symbols should be created.                                 |
`----------------------------------------------------------------*/

ایستا نماد **symbols_sorted = پوچ;
ایستا semantic_type **semantic_types_sorted = پوچ;


/*------------------------.
| Distinguished symbols.  |
`------------------------*/

نماد *errtoken = پوچ;
نماد *undeftoken = پوچ;
نماد *eoftoken = پوچ;
نماد *acceptsymbol = پوچ;

/* Precedence relation graph. */
ایستا symgraph **prec_nodes;

/* Store which associativity is used.  */
ایستا bool *used_assoc = پوچ;

bool tag_seen = false;


/* Whether SYM was defined by the user.  */

ایستا bool
نماد_is_user_defined (نماد *sym)
{
  ثابت bool eof_is_user_defined
    = !eoftoken->alias || STRNEQ (eoftoken->alias->tag, "$end");
  بازگشت sym->tag[0] != '$'
    && (eof_is_user_defined || (sym != eoftoken && sym->alias != errtoken))
    && sym != errtoken && sym->alias != errtoken
    && sym != undeftoken && sym->alias != undeftoken;
}


/*--------------------------.
| Create a new sym_content. |
`--------------------------*/

ایستا sym_content *
sym_content_new (نماد *s)
{
  sym_content *res = xmalloc (اندازه *res);

  res->نماد = s;

  res->type_name = پوچ;
  res->type_loc = empty_loc;
  برای (صحیح i = 0; i < CODE_PROPS_SIZE; ++i)
    code_props_none_init (&res->props[i]);

  res->عدد = عدد_UNDEFINED;
  res->prec_loc = empty_loc;
  res->prec = 0;
  res->assoc = undef_assoc;
  res->code = CODE_UNDEFINED;

  res->class = unknown_sym;
  res->status = undeclared;

  بازگشت res;
}

/*---------------------------------.
| Create a new symbol, named TAG.  |
`---------------------------------*/

ایستا نماد *
نماد_new (uniqstr tag, location loc)
{
  نماد *res = xmalloc (اندازه *res);
  uniqstr_assert (tag);

  /* If the tag is not a string (starts with a double quote), check
     that it is valid for Yacc. */
  اگر (tag[0] != '\"' && tag[0] != '\'' && strchr (tag, '-'))
    complain (&loc, Wyacc,
              _("POSIX Yacc forbids dashes in symbol names: %s"), tag);

  res->tag = tag;
  res->location = loc;
  res->translatable = false;
  res->location_of_lhs = false;
  res->alias = پوچ;
  res->content = sym_content_new (res);
  res->is_alias = false;
  بازگشت res;
}

/*--------------------.
| Free a sym_content. |
`--------------------*/

ایستا تهی
sym_content_free (sym_content *sym)
{
  آزاد (sym);
}


/*---------------------------------------------------------.
| Free a symbol and its associated content if appropriate. |
`---------------------------------------------------------*/

ایستا تهی
نماد_free (تهی *ptr)
{
  نماد *sym = (نماد *)ptr;
  اگر (!sym->is_alias)
    sym_content_free (sym->content);
  آزاد (sym);

}

/* If needed, swap first and second so that first has the earliest
   location (according to location_cmp).

   Many symbol features (e.g., token codes) are not assigned during
   parsing, but in a second step, via a traversal of the symbol table
   sorted on tag.

   However, error messages make more sense if we keep the first
   declaration first.
*/

ایستا تهی
symbols_sort (ثابت نماد **first, ثابت نماد **second)
{
  اگر (0 < location_cmp ((*first)->location, (*second)->location))
    {
      ثابت نماد* tmp = *first;
      *first = *second;
      *second = tmp;
    }
}

/* Likewise, for locations.  */

ایستا تهی
locations_sort (location *first, location *second)
{
  اگر (0 < location_cmp (*first, *second))
    {
      location tmp = *first;
      *first = *second;
      *second = tmp;
    }
}

نویسه ثابت *
code_props_type_string (code_props_type kind)
{
  انتخاب (kind)
    {
    حالت destructor:
      بازگشت "%destructor";
    حالت printer:
      بازگشت "%printer";
    }
  abort ();
}


/*----------------------------------------.
| Create a new semantic type, named TAG.  |
`----------------------------------------*/

ایستا semantic_type *
semantic_type_new (uniqstr tag, ثابت location *loc)
{
  semantic_type *res = xmalloc (اندازه *res);

  uniqstr_assert (tag);
  res->tag = tag;
  res->location = loc ? *loc : empty_loc;
  res->status = undeclared;
  برای (صحیح i = 0; i < CODE_PROPS_SIZE; ++i)
    code_props_none_init (&res->props[i]);

  بازگشت res;
}


/*-----------------.
| Print a symbol.  |
`-----------------*/

#define نماد_INT_ATTR_PRINT(Attr)                     \
  اگر (s->content)                                       \
    fprintf (f, " %s = %d", #Attr, s->content->Attr)

#define نماد_STR_ATTR_PRINT(Attr)                     \
  اگر (s->content && s->content->Attr)                   \
    fprintf (f, " %s { %s }", #Attr, s->content->Attr)

#define نماد_CODE_PRINT(Attr)                                         \
  اگر (s->content && s->content->props[Attr].code)                       \
    fprintf (f, " %s { %s }", #Attr, s->content->props[Attr].code)

تهی
نماد_print (نماد ثابت *s, FILE *f)
{
  اگر (s)
    {
      نماد_class c = s->content->class;
      fprintf (f, "%s: %s",
               c == unknown_sym    ? "unknown"
               : c == pct_type_sym ? "%type"
               : c == token_sym    ? "token"
               : c == nterm_sym    ? "nterm"
               : پوچ, /* abort.  */
               s->tag);
      putc (' ', f);
      location_print (s->location, f);
      نماد_INT_ATTR_PRINT (code);
      نماد_INT_ATTR_PRINT (عدد);
      نماد_STR_ATTR_PRINT (type_name);
      نماد_CODE_PRINT (destructor);
      نماد_CODE_PRINT (printer);
    }
  وگرنه
    fputs ("<NULL>", f);
}

#undef نماد_ATTR_PRINT
#undef نماد_CODE_PRINT


/*----------------------------------.
| Whether S is a valid identifier.  |
`----------------------------------*/

ایستا bool
is_identifier (uniqstr s)
{
  ایستا نویسه ثابت alphanum[26 + 26 + 1 + 10] =
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "_"
    "0123456789";
  اگر (!s || ! memchr (alphanum, *s, اندازه alphanum - 10))
    بازگشت false;
  برای (++s; *s; ++s)
    اگر (! memchr (alphanum, *s, اندازه alphanum))
      بازگشت false;
  بازگشت true;
}


/*-----------------------------------------------.
| Get the identifier associated to this symbol.  |
`-----------------------------------------------*/

uniqstr
نماد_id_get (نماد ثابت *sym)
{
  // There's one weird case: YYerror is the alias, and error is the
  // base symbol.  Return YYerror in that case.
  اگر (sym->alias && is_identifier (sym->alias->tag))
    بازگشت sym->alias->tag;
  وگرنه اگر (is_identifier (sym->tag))
    بازگشت sym->tag;
  وگرنه
    بازگشت پوچ;
}


/*------------------------------------------------------------------.
| Complain that S's WHAT is redeclared at SECOND, and was first set |
| at FIRST.                                                         |
`------------------------------------------------------------------*/

ایستا تهی
complain_نماد_redeclared (نماد *s, ثابت نویسه *what, location first,
                            location second)
{
  locations_sort (&first, &second);
  complain (&second, complaint, _("%s redeclaration for %s"), what, s->tag);
  subcomplain (&first, complaint, _("previous declaration"));
}

ایستا تهی
complain_semantic_type_redeclared (semantic_type *s, ثابت نویسه *what, location first,
                                   location second)
{
  locations_sort (&first, &second);
  complain (&second, complaint, _("%s redeclaration for <%s>"), what, s->tag);
  subcomplain (&first, complaint, _("previous declaration"));
}

ایستا تهی
complain_class_redeclared (نماد *sym, نماد_class class, location second)
{
  complain (&second, complaint,
            class == token_sym
            ? _("symbol %s redeclared as a token")
            : _("symbol %s redeclared as a nonterminal"), sym->tag);
  اگر (!location_empty (sym->location))
    subcomplain (&sym->location, complaint, _("previous definition"));
}

ایستا ثابت نماد *
نماد_from_uniqstr_fuzzy (ثابت uniqstr کلید)
{
  aver (symbols_sorted);
#define FSTRCMP_THRESHOLD 0.6
  دوگانه best_similarity = FSTRCMP_THRESHOLD;
  ثابت نماد *res = پوچ;
  size_t شمار = hash_get_n_entries (نماد_table);
  برای (صحیح i = 0; i < شمار; ++i)
    {
      نماد *sym = symbols_sorted[i];
      اگر (STRNEQ (کلید, sym->tag)
          && (sym->content->status == declared
              || sym->content->status == undeclared))
        {
          دوگانه similarity = fstrcmp_bounded (کلید, sym->tag, best_similarity);
          اگر (best_similarity < similarity)
            {
              res = sym;
              best_similarity = similarity;
            }
        }
    }
  بازگشت res;
}

ایستا تهی
complain_نماد_undeclared (ثابت نماد *sym)
{
  أكد (sym->content->status != declared);
  ثابت نماد *best = نماد_from_uniqstr_fuzzy (sym->tag);
  اگر (best)
    {
      complain (&sym->location,
                sym->content->status == needed ? complaint : Wother,
                _("symbol %s is used, but is not defined as a token"
                  " and has no rules; did you mean %s?"),
                quote_n (0, sym->tag),
                quote_n (1, best->tag));
      اگر (feature_flag & feature_caret)
        location_caret_suggestion (sym->location, best->tag, stderr);
    }
  وگرنه
    complain (&sym->location,
              sym->content->status == needed ? complaint : Wother,
              _("symbol %s is used, but is not defined as a token"
                " and has no rules"),
              quote (sym->tag));
}

تهی
نماد_location_as_lhs_set (نماد *sym, location loc)
{
  اگر (!sym->location_of_lhs)
    {
      sym->location = loc;
      sym->location_of_lhs = true;
    }
}


/*-----------------------------------------------------------------.
| Set the TYPE_NAME associated with SYM.  Does nothing if passed 0 |
| as TYPE_NAME.                                                    |
`-----------------------------------------------------------------*/

تهی
نماد_type_set (نماد *sym, uniqstr type_name, location loc)
{
  اگر (type_name)
    {
      tag_seen = true;
      اگر (sym->content->type_name)
        complain_نماد_redeclared (sym, "%type",
                                    sym->content->type_loc, loc);
      وگرنه
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

تهی
نماد_code_props_set (نماد *sym, code_props_type kind,
                       code_props ثابت *code)
{
  اگر (sym->content->props[kind].code)
    complain_نماد_redeclared (sym, code_props_type_string (kind),
                                sym->content->props[kind].location,
                                code->location);
  وگرنه
    sym->content->props[kind] = *code;
}


/*-----------------------------------------------------.
| Set the DESTRUCTOR or PRINTER associated with TYPE.  |
`-----------------------------------------------------*/

تهی
semantic_type_code_props_set (semantic_type *type,
                              code_props_type kind,
                              code_props ثابت *code)
{
  اگر (type->props[kind].code)
    complain_semantic_type_redeclared (type, code_props_type_string (kind),
                                       type->props[kind].location,
                                       code->location);
  وگرنه
    type->props[kind] = *code;
}

/*---------------------------------------------------.
| Get the computed %destructor or %printer for SYM.  |
`---------------------------------------------------*/

code_props *
نماد_code_props_get (نماد *sym, code_props_type kind)
{
  /* Per-symbol code props.  */
  اگر (sym->content->props[kind].code)
    بازگشت &sym->content->props[kind];

  /* Per-type code props.  */
  اگر (sym->content->type_name)
    {
      code_props *code =
        &semantic_type_get (sym->content->type_name, پوچ)->props[kind];
      اگر (code->code)
        بازگشت code;
    }

  /* Apply default code props's only to user-defined symbols.  */
  اگر (نماد_is_user_defined (sym))
    {
      code_props *code = &semantic_type_get (sym->content->type_name ? "*" : "",
                                             پوچ)->props[kind];
      اگر (code->code)
        بازگشت code;
    }
  بازگشت &code_props_none;
}

/*-----------------------------------------------------------------.
| Set the PRECEDENCE associated with SYM.  Does nothing if invoked |
| with UNDEF_ASSOC as ASSOC.                                       |
`-----------------------------------------------------------------*/

تهی
نماد_precedence_set (نماد *sym, صحیح prec, assoc a, location loc)
{
  اگر (a != undef_assoc)
    {
      sym_content *s = sym->content;
      اگر (s->prec)
        complain_نماد_redeclared (sym, assoc_to_string (a),
                                    s->prec_loc, loc);
      وگرنه
        {
          s->prec = prec;
          s->assoc = a;
          s->prec_loc = loc;
        }
    }

  /* Only terminals have a precedence. */
  نماد_class_set (sym, token_sym, loc, false);
}


/*------------------------------------.
| Set the CLASS associated with SYM.  |
`------------------------------------*/

ایستا تهی
complain_pct_type_on_token (location *loc)
{
  complain (loc, Wyacc,
            _("POSIX yacc reserves %%type to nonterminals"));
}

تهی
نماد_class_set (نماد *sym, نماد_class class, location loc, bool declaring)
{
  aver (class != unknown_sym);
  sym_content *s = sym->content;
  اگر (class == pct_type_sym)
    {
      اگر (s->class == token_sym)
        complain_pct_type_on_token (&loc);
      وگرنه اگر (s->class == unknown_sym)
        s->class = class;
    }
  وگرنه اگر (s->class != unknown_sym && s->class != pct_type_sym
           && s->class != class)
    complain_class_redeclared (sym, class, loc);
  وگرنه
    {
      اگر (class == token_sym && s->class == pct_type_sym)
        complain_pct_type_on_token (&sym->location);

      s->class = class;

      اگر (declaring)
        {
          اگر (s->status == declared)
            {
              complain (&loc, Wother,
                        _("symbol %s redeclared"), sym->tag);
              subcomplain (&sym->location, Wother,
                           _("previous declaration"));
            }
          وگرنه
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

تهی
نماد_code_set (نماد *sym, صحیح code, location loc)
{
  صحیح *codep = &sym->content->code;
  اگر (sym->content->class != token_sym)
    complain (&loc, complaint,
              _("nonterminals cannot be given a token code"));
  وگرنه اگر (*codep != CODE_UNDEFINED
           && *codep != code)
    complain (&loc, complaint, _("redefining code of token %s"),
              sym->tag);
  وگرنه اگر (code == INT_MAX)
    complain (&loc, complaint, _("code of token %s too large"),
              sym->tag);
  وگرنه
    {
      *codep = code;
      /* User defined $end token? */
      اگر (code == 0 && !eoftoken)
        {
          eoftoken = sym->content->نماد;
          eoftoken->content->عدد = 0;
        }
    }
}


/*----------------------------------------------------------.
| If SYM is not defined, report an error, and consider it a |
| nonterminal.                                              |
`----------------------------------------------------------*/

ایستا تهی
نماد_check_defined (نماد *sym)
{
  sym_content *s = sym->content;
  اگر (s->class == unknown_sym || s->class == pct_type_sym)
    {
      complain_نماد_undeclared (sym);
      s->class = nterm_sym;
    }

  اگر (s->عدد == عدد_UNDEFINED)
    s->عدد = s->class == token_sym ? ntokens++ : nnterms++;

  اگر (s->class == token_sym
      && sym->tag[0] == '"'
      && !sym->is_alias)
    complain (&sym->location, Wdangling_alias,
              _("string literal %s not attached to a symbol"),
              sym->tag);

  برای (صحیح i = 0; i < 2; ++i)
    نماد_code_props_get (sym, i)->is_used = true;

  /* Set the semantic type status associated to the current symbol to
     'declared' so that we could check semantic types unnecessary uses. */
  اگر (s->type_name)
    {
      semantic_type *sem_type = semantic_type_get (s->type_name, پوچ);
      اگر (sem_type)
        sem_type->status = declared;
    }
}

ایستا تهی
semantic_type_check_defined (semantic_type *sem_type)
{
  /* <*> and <> do not have to be "declared".  */
  اگر (sem_type->status == declared
      || !*sem_type->tag
      || STREQ (sem_type->tag, "*"))
    {
      برای (صحیح i = 0; i < 2; ++i)
        اگر (sem_type->props[i].kind != CODE_PROPS_NONE
            && ! sem_type->props[i].is_used)
          complain (&sem_type->location, Wother,
                    _("useless %s for type <%s>"),
                    code_props_type_string (i), sem_type->tag);
    }
  وگرنه
    complain (&sem_type->location, Wother,
              _("type <%s> is used, but is not associated to any symbol"),
              sem_type->tag);
}

/*-------------------------------------------------------------------.
| Merge the properties (precedence, associativity, etc.) of SYM, and |
| its string-named alias STR; check consistency.                     |
`-------------------------------------------------------------------*/

ایستا تهی
نماد_merge_properties (نماد *sym, نماد *str)
{
  اگر (str->content->type_name != sym->content->type_name)
    {
      اگر (str->content->type_name)
        نماد_type_set (sym,
                         str->content->type_name, str->content->type_loc);
      وگرنه
        نماد_type_set (str,
                         sym->content->type_name, sym->content->type_loc);
    }


  برای (صحیح i = 0; i < CODE_PROPS_SIZE; ++i)
    اگر (str->content->props[i].code)
      نماد_code_props_set (sym, i, &str->content->props[i]);
    وگرنه اگر (sym->content->props[i].code)
      نماد_code_props_set (str, i, &sym->content->props[i]);

  اگر (sym->content->prec || str->content->prec)
    {
      اگر (str->content->prec)
        نماد_precedence_set (sym, str->content->prec, str->content->assoc,
                               str->content->prec_loc);
      وگرنه
        نماد_precedence_set (str, sym->content->prec, sym->content->assoc,
                               sym->content->prec_loc);
    }
}

تهی
نماد_make_alias (نماد *sym, نماد *str, location loc)
{
  اگر (sym->content->class != token_sym)
    complain (&loc, complaint,
              _("nonterminals cannot be given a string alias"));
  وگرنه اگر (str->alias)
    complain (&loc, Wother,
              _("symbol %s used more than once as a literal string"), str->tag);
  وگرنه اگر (sym->alias)
    complain (&loc, Wother,
              _("symbol %s given more than one literal string"), sym->tag);
  وگرنه
    {
      نماد_merge_properties (sym, str);
      sym_content_free (str->content);
      str->content = sym->content;
      str->content->نماد = str;
      str->is_alias = true;
      str->alias = sym;
      sym->alias = str;
    }
}


/*-------------------------------------------------------------------.
| Assign a symbol number, and write the definition of the token name |
| into FDEFINES.  Put in SYMBOLS.                                    |
`-------------------------------------------------------------------*/

ایستا تهی
نماد_pack (نماد *sym)
{
  aver (sym->content->عدد != عدد_UNDEFINED);
  اگر (sym->content->class == nterm_sym)
    sym->content->عدد += ntokens;

  symbols[sym->content->عدد] = sym->content->نماد;
}

ایستا تهی
complain_code_redeclared (صحیح رقم, ثابت نماد *first, ثابت نماد *second)
{
  symbols_sort (&first, &second);
  complain (&second->location, complaint,
            _("code %d reassigned to token %s"),
            رقم, second->tag);
  subcomplain (&first->location, complaint,
               _("previous declaration for %s"),
               first->tag);
}

/*-------------------------------------------------.
| Put SYM in TOKEN_TRANSLATIONS if it is a token.  |
`-------------------------------------------------*/

ایستا تهی
نماد_translation (ثابت نماد *sym)
{
  اگر (sym->content->class == token_sym && !sym->is_alias)
    {
      /* A token whose translation has already been set? */
      اگر (token_translations[sym->content->code]
          != undeftoken->content->عدد)
        complain_code_redeclared
          (sym->content->code,
           symbols[token_translations[sym->content->code]], sym);
      وگرنه
        token_translations[sym->content->code]
          = sym->content->عدد;
    }
}


/*---------------------------------------.
| Symbol and semantic type hash tables.  |
`---------------------------------------*/

/* Initial capacity of symbol and semantic type hash table.  */
#define HT_INITIAL_CAPACITY 257

ایستا درونخطی bool
hash_compare_نماد (ثابت نماد *m1, ثابت نماد *m2)
{
  /* Since tags are unique, we can compare the pointers themselves.  */
  بازگشت UNIQSTR_EQ (m1->tag, m2->tag);
}

ایستا درونخطی bool
hash_compare_semantic_type (ثابت semantic_type *m1, ثابت semantic_type *m2)
{
  /* Since names are unique, we can compare the pointers themselves.  */
  بازگشت UNIQSTR_EQ (m1->tag, m2->tag);
}

ایستا bool
hash_نماد_comparator (تهی ثابت *m1, تهی ثابت *m2)
{
  بازگشت hash_compare_نماد (m1, m2);
}

ایستا bool
hash_semantic_type_comparator (تهی ثابت *m1, تهی ثابت *m2)
{
  بازگشت hash_compare_semantic_type (m1, m2);
}

ایستا درونخطی size_t
hash_نماد (ثابت نماد *m, size_t tablesize)
{
  /* Since tags are unique, we can hash the pointer itself.  */
  بازگشت ((uintptr_t) m->tag) % tablesize;
}

ایستا درونخطی size_t
hash_semantic_type (ثابت semantic_type *m, size_t tablesize)
{
  /* Since names are unique, we can hash the pointer itself.  */
  بازگشت ((uintptr_t) m->tag) % tablesize;
}

ایستا size_t
hash_نماد_hasher (تهی ثابت *m, size_t tablesize)
{
  بازگشت hash_نماد (m, tablesize);
}

ایستا size_t
hash_semantic_type_hasher (تهی ثابت *m, size_t tablesize)
{
  بازگشت hash_semantic_type (m, tablesize);
}

/*-------------------------------.
| Create the symbol hash table.  |
`-------------------------------*/

تهی
symbols_new (تهی)
{
  نماد_table = hash_xinitialize (HT_INITIAL_CAPACITY,
                                   پوچ,
                                   hash_نماد_hasher,
                                   hash_نماد_comparator,
                                   نماد_free);

  /* Construct the acceptsymbol symbol. */
  acceptsymbol = نماد_get ("$accept", empty_loc);
  acceptsymbol->content->class = nterm_sym;
  acceptsymbol->content->عدد = nnterms++;

  /* Construct the YYerror/"error" token */
  errtoken = نماد_get ("YYerror", empty_loc);
  errtoken->content->class = token_sym;
  errtoken->content->عدد = ntokens++;
  {
    نماد *alias = نماد_get ("error", empty_loc);
    نماد_class_set (alias, token_sym, empty_loc, false);
    نماد_make_alias (errtoken, alias, empty_loc);
  }

  /* Construct the YYUNDEF/"$undefined" token that represents all
     undefined literal tokens.  It is always symbol number 2.  */
  undeftoken = نماد_get ("YYUNDEF", empty_loc);
  undeftoken->content->class = token_sym;
  undeftoken->content->عدد = ntokens++;
  {
    نماد *alias = نماد_get ("$undefined", empty_loc);
    نماد_class_set (alias, token_sym, empty_loc, false);
    نماد_make_alias (undeftoken, alias, empty_loc);
  }

  semantic_type_table = hash_xinitialize (HT_INITIAL_CAPACITY,
                                          پوچ,
                                          hash_semantic_type_hasher,
                                          hash_semantic_type_comparator,
                                          آزاد);
}


/*----------------------------------------------------------------.
| Find the symbol named KEY, and return it.  If it does not exist |
| yet, create it.                                                 |
`----------------------------------------------------------------*/

نماد *
نماد_from_uniqstr (ثابت uniqstr کلید, location loc)
{
  نماد probe;

  probe.tag = کلید;
  نماد *res = hash_lookup (نماد_table, &probe);

  اگر (!res)
    {
      /* First insertion in the hash. */
      aver (!symbols_sorted);
      res = نماد_new (کلید, loc);
      hash_xinsert (نماد_table, res);
    }
  بازگشت res;
}


/*-----------------------------------------------------------------------.
| Find the semantic type named KEY, and return it.  If it does not exist |
| yet, create it.                                                        |
`-----------------------------------------------------------------------*/

semantic_type *
semantic_type_from_uniqstr (ثابت uniqstr کلید, ثابت location *loc)
{
  semantic_type probe;

  probe.tag = کلید;
  semantic_type *res = hash_lookup (semantic_type_table, &probe);

  اگر (!res)
    {
      /* First insertion in the hash. */
      res = semantic_type_new (کلید, loc);
      hash_xinsert (semantic_type_table, res);
    }
  بازگشت res;
}


/*----------------------------------------------------------------.
| Find the symbol named KEY, and return it.  If it does not exist |
| yet, create it.                                                 |
`----------------------------------------------------------------*/

نماد *
نماد_get (ثابت نویسه *کلید, location loc)
{
  بازگشت نماد_from_uniqstr (uniqstr_new (کلید), loc);
}


/*-----------------------------------------------------------------------.
| Find the semantic type named KEY, and return it.  If it does not exist |
| yet, create it.                                                        |
`-----------------------------------------------------------------------*/

semantic_type *
semantic_type_get (ثابت نویسه *کلید, ثابت location *loc)
{
  بازگشت semantic_type_from_uniqstr (uniqstr_new (کلید), loc);
}


/*------------------------------------------------------------------.
| Generate a dummy nonterminal, whose name cannot conflict with the |
| user's names.                                                     |
`------------------------------------------------------------------*/

نماد *
dummy_نماد_get (location loc)
{
  /* Incremented for each generated symbol.  */
  ایستا صحیح dummy_شمار = 0;
  نویسه buf[32];
  صحیح len = snprintf (buf, اندازه buf, "$@%d", ++dummy_شمار);
  assure (len < اندازه buf);
  نماد *sym = نماد_get (buf, loc);
  sym->content->class = nterm_sym;
  بازگشت sym;
}

bool
نماد_is_dummy (نماد ثابت *sym)
{
  بازگشت sym->tag[0] == '@' || (sym->tag[0] == '$' && sym->tag[1] == '@');
}

/*-------------------.
| Free the symbols.  |
`-------------------*/

تهی
symbols_free (تهی)
{
  hash_free (نماد_table);
  hash_free (semantic_type_table);
  آزاد (symbols);
  آزاد (symbols_sorted);
  آزاد (semantic_types_sorted);
}


ایستا صحیح
نماد_cmp (تهی ثابت *a, تهی ثابت *b)
{
  بازگشت location_cmp ((*(نماد * ثابت *)a)->location,
                       (*(نماد * ثابت *)b)->location);
}

/* Store in *SORTED an array of pointers to the symbols contained in
   TABLE, sorted by order of appearance (i.e., by location). */

ایستا تهی
table_sort (ساختار hash_table *table, نماد ***sorted)
{
  aver (!*sorted);
  size_t شمار = hash_get_n_entries (table);
  *sorted = xnmalloc (شمار + 1, اندازه **sorted);
  hash_get_entries (table, (تهی**)*sorted, شمار);
  qsort (*sorted, شمار, اندازه **sorted, نماد_cmp);
  (*sorted)[شمار] = پوچ;
}

/*--------------------------------------------------------------.
| Check that all the symbols are defined.  Report any undefined |
| symbols and consider them nonterminals.                       |
`--------------------------------------------------------------*/

تهی
symbols_check_defined (تهی)
{
  table_sort (نماد_table, &symbols_sorted);
  /* semantic_type, like symbol, starts with a 'tag' field and then a
     'location' field.  And here we only deal with arrays/hashes of
     pointers, sizeof is not an issue.

     So instead of implementing table_sort (and symbol_cmp) once for
     each type, let's lie a bit to the typing system, and treat
     'semantic_type' as if it were 'symbol'. */
  table_sort (semantic_type_table, (نماد ***) &semantic_types_sorted);

  برای (صحیح i = 0; symbols_sorted[i]; ++i)
    نماد_check_defined (symbols_sorted[i]);
  برای (صحیح i = 0; semantic_types_sorted[i]; ++i)
    semantic_type_check_defined (semantic_types_sorted[i]);
}

/*------------------------------------------------------------------.
| Set TOKEN_TRANSLATIONS.  Check that no two symbols share the same |
| number.                                                           |
`------------------------------------------------------------------*/

ایستا تهی
symbols_token_translations_init (تهی)
{
  bool code_256_available_p = true;

  /* Find the highest token code, and whether 256, the POSIX preferred
     token code for the error token, is used.  */
  max_code = 0;
  برای (صحیح i = 0; i < ntokens; ++i)
    {
      sym_content *sym = symbols[i]->content;
      اگر (sym->code != CODE_UNDEFINED)
        {
          اگر (sym->code > max_code)
            max_code = sym->code;
          اگر (sym->code == 256)
            code_256_available_p = false;
        }
    }

  /* If 256 is not used, assign it to error, to follow POSIX.  */
  اگر (code_256_available_p
      && errtoken->content->code == CODE_UNDEFINED)
    errtoken->content->code = 256;

  /* Set the missing codes. */
  اگر (max_code < 256)
    max_code = 256;

  برای (صحیح i = 0; i < ntokens; ++i)
    {
      sym_content *sym = symbols[i]->content;
      اگر (sym->code == CODE_UNDEFINED)
        {
          IGNORE_TYPE_LIMITS_BEGIN
          اگر (INT_ADD_WRAPV (max_code, 1, &max_code))
            complain (پوچ, fatal, _("token number too large"));
          IGNORE_TYPE_LIMITS_END
          sym->code = max_code;
        }
      اگر (sym->code > max_code)
        max_code = sym->code;
    }

  token_translations = xnmalloc (max_code + 1,
                                 اندازه *token_translations);

  /* Initialize all entries for literal tokens to the internal token
     number for $undefined, which represents all invalid inputs.  */
  برای (صحیح i = 0; i < max_code + 1; ++i)
    token_translations[i] = undeftoken->content->عدد;
  برای (صحیح i = 0; symbols_sorted[i]; ++i)
    نماد_translation (symbols_sorted[i]);
}


/* Whether some symbol requires internationalization.  */
ایستا bool
has_translations (تهی)
{
  برای (ثابت تهی *entry = hash_get_first (نماد_table);
       entry;
       entry = hash_get_next (نماد_table, entry))
    {
      ثابت نماد *sym = (ثابت نماد *) entry;
      اگر (sym->translatable)
        بازگشت true;
    }
  بازگشت false;
}

/*----------------------------------------------------------------.
| Assign symbol numbers, and write definition of token names into |
| FDEFINES.  Set up vectors SYMBOL_TABLE, TAGS of symbols.        |
`----------------------------------------------------------------*/

تهی
symbols_pack (تهی)
{
  symbols = xcalloc (nsyms, اندازه *symbols);
  برای (صحیح i = 0; symbols_sorted[i]; ++i)
    نماد_pack (symbols_sorted[i]);

  /* Aliases leave empty slots in symbols, so remove them.  */
  {
    صحیح nsyms_old = nsyms;
    برای (صحیح writei = 0, readi = 0; readi < nsyms_old; readi += 1)
      {
        اگر (symbols[readi] == پوچ)
          {
            nsyms -= 1;
            ntokens -= 1;
          }
        وگرنه
          {
            symbols[writei] = symbols[readi];
            symbols[writei]->content->عدد = writei;
            writei += 1;
          }
      }
  }
  symbols = xnrealloc (symbols, nsyms, اندازه *symbols);

  symbols_token_translations_init ();

  // If some user tokens are internationalized, the internal ones
  // should be too.
  اگر (has_translations ())
    {
      ثابت bool eof_is_user_defined
        = !eoftoken->alias || STRNEQ (eoftoken->alias->tag, "$end");
      اگر (!eof_is_user_defined)
        eoftoken->alias->translatable = true;
      undeftoken->alias->translatable = true;
      errtoken->alias->translatable = true;
    }
}

/*---------------------------------.
| Initialize relation graph nodes. |
`---------------------------------*/

ایستا تهی
init_prec_nodes (تهی)
{
  prec_nodes = xcalloc (nsyms, اندازه *prec_nodes);
  برای (صحیح i = 0; i < nsyms; ++i)
    {
      prec_nodes[i] = xmalloc (اندازه *prec_nodes[i]);
      symgraph *s = prec_nodes[i];
      s->id = i;
      s->succ = 0;
      s->pred = 0;
    }
}

/*----------------.
| Create a link.  |
`----------------*/

ایستا symgraphlink *
symgraphlink_new (graphid id, symgraphlink *next)
{
  symgraphlink *res = xmalloc (اندازه *res);
  res->id = id;
  res->next = next;
  بازگشت res;
}


/*------------------------------------------------------------------.
| Register the second symbol of the precedence relation, and return |
| whether this relation is new.  Use only in register_precedence.   |
`------------------------------------------------------------------*/

ایستا bool
register_precedence_second_نماد (symgraphlink **first, graphid sym)
{
  اگر (!*first || sym < (*first)->id)
    *first = symgraphlink_new (sym, *first);
  وگرنه
    {
      symgraphlink *slist = *first;

      وقتی (slist->next && slist->next->id <= sym)
        slist = slist->next;

      اگر (slist->id == sym)
        /* Relation already present. */
        بازگشت false;

      slist->next = symgraphlink_new (sym, slist->next);
    }
  بازگشت true;
}

/*------------------------------------------------------------------.
| Register a new relation between symbols as used. The first symbol |
| has a greater precedence than the second one.                     |
`------------------------------------------------------------------*/

تهی
register_precedence (graphid first, graphid snd)
{
  اگر (!prec_nodes)
    init_prec_nodes ();
  register_precedence_second_نماد (&(prec_nodes[first]->succ), snd);
  register_precedence_second_نماد (&(prec_nodes[snd]->pred), first);
}


/*---------------------------------------.
| Deep clear a linked / adjacency list). |
`---------------------------------------*/

ایستا تهی
linkedlist_free (symgraphlink *گره)
{
  اگر (گره)
    {
      وقتی (گره->next)
        {
          symgraphlink *tmp = گره->next;
          آزاد (گره);
          گره = tmp;
        }
      آزاد (گره);
    }
}

/*----------------------------------------------.
| Clear and destroy association tracking table. |
`----------------------------------------------*/

ایستا تهی
assoc_free (تهی)
{
  برای (صحیح i = 0; i < nsyms; ++i)
    {
      linkedlist_free (prec_nodes[i]->pred);
      linkedlist_free (prec_nodes[i]->succ);
      آزاد (prec_nodes[i]);
    }
  آزاد (prec_nodes);
}

/*---------------------------------------.
| Initialize association tracking table. |
`---------------------------------------*/

ایستا تهی
init_assoc (تهی)
{
  used_assoc = xcalloc (nsyms, اندازه *used_assoc);
  برای (graphid i = 0; i < nsyms; ++i)
    used_assoc[i] = false;
}

/*------------------------------------------------------------------.
| Test if the associativity for the symbols is defined and useless. |
`------------------------------------------------------------------*/

ایستا درونخطی bool
is_assoc_useless (نماد *s)
{
  بازگشت s
      && s->content->assoc != undef_assoc
      && s->content->assoc != precedence_assoc
      && !used_assoc[s->content->عدد];
}

/*-------------------------------.
| Register a used associativity. |
`-------------------------------*/

تهی
register_assoc (graphid i, graphid j)
{
  اگر (!used_assoc)
    init_assoc ();
  used_assoc[i] = true;
  used_assoc[j] = true;
}

/*--------------------------------------------------.
| Print a warning for unused precedence relations.  |
`--------------------------------------------------*/

تهی
print_precedence_warnings (تهی)
{
  اگر (!prec_nodes)
    init_prec_nodes ();
  اگر (!used_assoc)
    init_assoc ();
  برای (صحیح i = 0; i < nsyms; ++i)
    {
      نماد *s = symbols[i];
      اگر (s
          && s->content->prec != 0
          && !prec_nodes[i]->pred
          && !prec_nodes[i]->succ)
        {
          اگر (is_assoc_useless (s))
            complain (&s->content->prec_loc, Wprecedence,
                      _("useless precedence and associativity for %s"), s->tag);
          وگرنه اگر (s->content->assoc == precedence_assoc)
            complain (&s->content->prec_loc, Wprecedence,
                      _("useless precedence for %s"), s->tag);
        }
      وگرنه اگر (is_assoc_useless (s))
        complain (&s->content->prec_loc, Wprecedence,
                  _("useless associativity for %s, use %%precedence"), s->tag);
    }
  آزاد (used_assoc);
  assoc_free ();
}
