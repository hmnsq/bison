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

#ضمن <config.h>
#ضمن "symtab.h"

#ضمن "system.h"

#ضمن <assure.h>
#ضمن <fstrcmp.h>
#ضمن <hash.h>
#ضمن <quote.h>

#ضمن "complain.h"
#ضمن "getargs.h"
#ضمن "gram.h"
#ضمن "intprops.h"

/** Undefined token code.  */
#define CODE_UNDEFINED (-1)

/* Undefined symbol number.  */
#define نومبور_UNDEFINED (-1)


ساكن بنية hash_table *سيمبول_table = عدم;
ساكن بنية hash_table *semantic_type_table = عدم;

/*----------------------------------------------------------------.
| Symbols sorted by tag.  Allocated by table_sort, after which no |
| more symbols should be created.                                 |
`----------------------------------------------------------------*/

ساكن سيمبول **symbols_sorted = عدم;
ساكن semantic_type **semantic_types_sorted = عدم;


/*------------------------.
| Distinguished symbols.  |
`------------------------*/

سيمبول *errtoken = عدم;
سيمبول *undeftoken = عدم;
سيمبول *eoftoken = عدم;
سيمبول *acceptsymbol = عدم;

/* Precedence relation graph. */
ساكن symgraph **prec_nodes;

/* Store which associativity is used.  */
ساكن bool *used_assoc = عدم;

bool tag_seen = false;


/* Whether SYM was defined by the user.  */

ساكن bool
سيمبول_is_user_defined (سيمبول *sym)
{
  ثابت bool eof_is_user_defined
    = !eoftoken->alias || STRNEQ (eoftoken->alias->tag, "$end");
  أرجع sym->tag[0] != '$'
    && (eof_is_user_defined || (sym != eoftoken && sym->alias != errtoken))
    && sym != errtoken && sym->alias != errtoken
    && sym != undeftoken && sym->alias != undeftoken;
}


/*--------------------------.
| Create a new sym_content. |
`--------------------------*/

ساكن sym_content *
sym_content_new (سيمبول *s)
{
  sym_content *res = xmalloc (حجم *res);

  res->سيمبول = s;

  res->type_name = عدم;
  res->type_loc = empty_loc;
  لكل (صحيح i = 0; i < CODE_PROPS_SIZE; ++i)
    code_props_none_init (&res->props[i]);

  res->نومبور = نومبور_UNDEFINED;
  res->prec_loc = empty_loc;
  res->prec = 0;
  res->assoc = undef_assoc;
  res->code = CODE_UNDEFINED;

  res->class = unknown_sym;
  res->status = undeclared;

  أرجع res;
}

/*---------------------------------.
| Create a new symbol, named TAG.  |
`---------------------------------*/

ساكن سيمبول *
سيمبول_new (uniqstr tag, location loc)
{
  سيمبول *res = xmalloc (حجم *res);
  uniqstr_assert (tag);

  /* If the tag is not a string (starts with a double quote), check
     that it is valid for Yacc. */
  إذا (tag[0] != '\"' && tag[0] != '\'' && strchr (tag, '-'))
    complain (&loc, Wyacc,
              _("POSIX Yacc forbids dashes in symbol names: %s"), tag);

  res->tag = tag;
  res->location = loc;
  res->translatable = false;
  res->location_of_lhs = false;
  res->alias = عدم;
  res->content = sym_content_new (res);
  res->is_alias = false;
  أرجع res;
}

/*--------------------.
| Free a sym_content. |
`--------------------*/

ساكن فراغ
sym_content_free (sym_content *sym)
{
  حرر (sym);
}


/*---------------------------------------------------------.
| Free a symbol and its associated content if appropriate. |
`---------------------------------------------------------*/

ساكن فراغ
سيمبول_free (فراغ *ptr)
{
  سيمبول *sym = (سيمبول *)ptr;
  إذا (!sym->is_alias)
    sym_content_free (sym->content);
  حرر (sym);

}

/* If needed, swap first and second so that first has the earliest
   location (according to location_cmp).

   Many symbol features (e.g., token codes) are not assigned during
   parsing, but in a second step, via a traversal of the symbol table
   sorted on tag.

   However, error messages make more sense if we keep the first
   declaration first.
*/

ساكن فراغ
symbols_sort (ثابت سيمبول **first, ثابت سيمبول **second)
{
  إذا (0 < location_cmp ((*first)->location, (*second)->location))
    {
      ثابت سيمبول* tmp = *first;
      *first = *second;
      *second = tmp;
    }
}

/* Likewise, for locations.  */

ساكن فراغ
locations_sort (location *first, location *second)
{
  إذا (0 < location_cmp (*first, *second))
    {
      location tmp = *first;
      *first = *second;
      *second = tmp;
    }
}

حرف ثابت *
code_props_type_string (code_props_type kind)
{
  بدل (kind)
    {
    حالة destructor:
      أرجع "%destructor";
    حالة printer:
      أرجع "%printer";
    }
  abort ();
}


/*----------------------------------------.
| Create a new semantic type, named TAG.  |
`----------------------------------------*/

ساكن semantic_type *
semantic_type_new (uniqstr tag, ثابت location *loc)
{
  semantic_type *res = xmalloc (حجم *res);

  uniqstr_assert (tag);
  res->tag = tag;
  res->location = loc ? *loc : empty_loc;
  res->status = undeclared;
  لكل (صحيح i = 0; i < CODE_PROPS_SIZE; ++i)
    code_props_none_init (&res->props[i]);

  أرجع res;
}


/*-----------------.
| Print a symbol.  |
`-----------------*/

#define سيمبول_INT_ATTR_PRINT(Attr)                     \
  إذا (s->content)                                       \
    fprintf (f, " %s = %d", #Attr, s->content->Attr)

#define سيمبول_STR_ATTR_PRINT(Attr)                     \
  إذا (s->content && s->content->Attr)                   \
    fprintf (f, " %s { %s }", #Attr, s->content->Attr)

#define سيمبول_CODE_PRINT(Attr)                                         \
  إذا (s->content && s->content->props[Attr].code)                       \
    fprintf (f, " %s { %s }", #Attr, s->content->props[Attr].code)

فراغ
سيمبول_print (سيمبول ثابت *s, FILE *f)
{
  إذا (s)
    {
      سيمبول_class c = s->content->class;
      fprintf (f, "%s: %s",
               c == unknown_sym    ? "unknown"
               : c == pct_type_sym ? "%type"
               : c == token_sym    ? "token"
               : c == nterm_sym    ? "nterm"
               : عدم, /* abort.  */
               s->tag);
      putc (' ', f);
      location_print (s->location, f);
      سيمبول_INT_ATTR_PRINT (code);
      سيمبول_INT_ATTR_PRINT (نومبور);
      سيمبول_STR_ATTR_PRINT (type_name);
      سيمبول_CODE_PRINT (destructor);
      سيمبول_CODE_PRINT (printer);
    }
  وإلا
    fputs ("<NULL>", f);
}

#undef سيمبول_ATTR_PRINT
#undef سيمبول_CODE_PRINT


/*----------------------------------.
| Whether S is a valid identifier.  |
`----------------------------------*/

ساكن bool
is_identifier (uniqstr s)
{
  ساكن حرف ثابت alphanum[26 + 26 + 1 + 10] =
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "_"
    "0123456789";
  إذا (!s || ! memchr (alphanum, *s, حجم alphanum - 10))
    أرجع false;
  لكل (++s; *s; ++s)
    إذا (! memchr (alphanum, *s, حجم alphanum))
      أرجع false;
  أرجع true;
}


/*-----------------------------------------------.
| Get the identifier associated to this symbol.  |
`-----------------------------------------------*/

uniqstr
سيمبول_id_get (سيمبول ثابت *sym)
{
  // There's one weird case: YYerror is the alias, and error is the
  // base symbol.  Return YYerror in that case.
  إذا (sym->alias && is_identifier (sym->alias->tag))
    أرجع sym->alias->tag;
  وإلا إذا (is_identifier (sym->tag))
    أرجع sym->tag;
  وإلا
    أرجع عدم;
}


/*------------------------------------------------------------------.
| Complain that S's WHAT is redeclared at SECOND, and was first set |
| at FIRST.                                                         |
`------------------------------------------------------------------*/

ساكن فراغ
complain_سيمبول_redeclared (سيمبول *s, ثابت حرف *what, location first,
                            location second)
{
  locations_sort (&first, &second);
  complain (&second, complaint, _("%s redeclaration for %s"), what, s->tag);
  subcomplain (&first, complaint, _("previous declaration"));
}

ساكن فراغ
complain_semantic_type_redeclared (semantic_type *s, ثابت حرف *what, location first,
                                   location second)
{
  locations_sort (&first, &second);
  complain (&second, complaint, _("%s redeclaration for <%s>"), what, s->tag);
  subcomplain (&first, complaint, _("previous declaration"));
}

ساكن فراغ
complain_class_redeclared (سيمبول *sym, سيمبول_class class, location second)
{
  complain (&second, complaint,
            class == token_sym
            ? _("symbol %s redeclared as a token")
            : _("symbol %s redeclared as a nonterminal"), sym->tag);
  إذا (!location_empty (sym->location))
    subcomplain (&sym->location, complaint, _("previous definition"));
}

ساكن ثابت سيمبول *
سيمبول_from_uniqstr_fuzzy (ثابت uniqstr کونچي)
{
  aver (symbols_sorted);
#define FSTRCMP_THRESHOLD 0.6
  مزدوج best_similarity = FSTRCMP_THRESHOLD;
  ثابت سيمبول *res = عدم;
  size_t کيراءن = hash_get_n_entries (سيمبول_table);
  لكل (صحيح i = 0; i < کيراءن; ++i)
    {
      سيمبول *sym = symbols_sorted[i];
      إذا (STRNEQ (کونچي, sym->tag)
          && (sym->content->status == declared
              || sym->content->status == undeclared))
        {
          مزدوج similarity = fstrcmp_bounded (کونچي, sym->tag, best_similarity);
          إذا (best_similarity < similarity)
            {
              res = sym;
              best_similarity = similarity;
            }
        }
    }
  أرجع res;
}

ساكن فراغ
complain_سيمبول_undeclared (ثابت سيمبول *sym)
{
  أكد (sym->content->status != declared);
  ثابت سيمبول *best = سيمبول_from_uniqstr_fuzzy (sym->tag);
  إذا (best)
    {
      complain (&sym->location,
                sym->content->status == needed ? complaint : Wother,
                _("symbol %s is used, but is not defined as a token"
                  " and has no rules; did you mean %s?"),
                quote_n (0, sym->tag),
                quote_n (1, best->tag));
      إذا (feature_flag & feature_caret)
        location_caret_suggestion (sym->location, best->tag, stderr);
    }
  وإلا
    complain (&sym->location,
              sym->content->status == needed ? complaint : Wother,
              _("symbol %s is used, but is not defined as a token"
                " and has no rules"),
              quote (sym->tag));
}

فراغ
سيمبول_location_as_lhs_set (سيمبول *sym, location loc)
{
  إذا (!sym->location_of_lhs)
    {
      sym->location = loc;
      sym->location_of_lhs = true;
    }
}


/*-----------------------------------------------------------------.
| Set the TYPE_NAME associated with SYM.  Does nothing if passed 0 |
| as TYPE_NAME.                                                    |
`-----------------------------------------------------------------*/

فراغ
سيمبول_type_set (سيمبول *sym, uniqstr type_name, location loc)
{
  إذا (type_name)
    {
      tag_seen = true;
      إذا (sym->content->type_name)
        complain_سيمبول_redeclared (sym, "%type",
                                    sym->content->type_loc, loc);
      وإلا
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

فراغ
سيمبول_code_props_set (سيمبول *sym, code_props_type kind,
                       code_props ثابت *code)
{
  إذا (sym->content->props[kind].code)
    complain_سيمبول_redeclared (sym, code_props_type_string (kind),
                                sym->content->props[kind].location,
                                code->location);
  وإلا
    sym->content->props[kind] = *code;
}


/*-----------------------------------------------------.
| Set the DESTRUCTOR or PRINTER associated with TYPE.  |
`-----------------------------------------------------*/

فراغ
semantic_type_code_props_set (semantic_type *type,
                              code_props_type kind,
                              code_props ثابت *code)
{
  إذا (type->props[kind].code)
    complain_semantic_type_redeclared (type, code_props_type_string (kind),
                                       type->props[kind].location,
                                       code->location);
  وإلا
    type->props[kind] = *code;
}

/*---------------------------------------------------.
| Get the computed %destructor or %printer for SYM.  |
`---------------------------------------------------*/

code_props *
سيمبول_code_props_get (سيمبول *sym, code_props_type kind)
{
  /* Per-symbol code props.  */
  إذا (sym->content->props[kind].code)
    أرجع &sym->content->props[kind];

  /* Per-type code props.  */
  إذا (sym->content->type_name)
    {
      code_props *code =
        &semantic_type_get (sym->content->type_name, عدم)->props[kind];
      إذا (code->code)
        أرجع code;
    }

  /* Apply default code props's only to user-defined symbols.  */
  إذا (سيمبول_is_user_defined (sym))
    {
      code_props *code = &semantic_type_get (sym->content->type_name ? "*" : "",
                                             عدم)->props[kind];
      إذا (code->code)
        أرجع code;
    }
  أرجع &code_props_none;
}

/*-----------------------------------------------------------------.
| Set the PRECEDENCE associated with SYM.  Does nothing if invoked |
| with UNDEF_ASSOC as ASSOC.                                       |
`-----------------------------------------------------------------*/

فراغ
سيمبول_precedence_set (سيمبول *sym, صحيح prec, assoc a, location loc)
{
  إذا (a != undef_assoc)
    {
      sym_content *s = sym->content;
      إذا (s->prec)
        complain_سيمبول_redeclared (sym, assoc_to_string (a),
                                    s->prec_loc, loc);
      وإلا
        {
          s->prec = prec;
          s->assoc = a;
          s->prec_loc = loc;
        }
    }

  /* Only terminals have a precedence. */
  سيمبول_class_set (sym, token_sym, loc, false);
}


/*------------------------------------.
| Set the CLASS associated with SYM.  |
`------------------------------------*/

ساكن فراغ
complain_pct_type_on_token (location *loc)
{
  complain (loc, Wyacc,
            _("POSIX yacc reserves %%type to nonterminals"));
}

فراغ
سيمبول_class_set (سيمبول *sym, سيمبول_class class, location loc, bool declaring)
{
  aver (class != unknown_sym);
  sym_content *s = sym->content;
  إذا (class == pct_type_sym)
    {
      إذا (s->class == token_sym)
        complain_pct_type_on_token (&loc);
      وإلا إذا (s->class == unknown_sym)
        s->class = class;
    }
  وإلا إذا (s->class != unknown_sym && s->class != pct_type_sym
           && s->class != class)
    complain_class_redeclared (sym, class, loc);
  وإلا
    {
      إذا (class == token_sym && s->class == pct_type_sym)
        complain_pct_type_on_token (&sym->location);

      s->class = class;

      إذا (declaring)
        {
          إذا (s->status == declared)
            {
              complain (&loc, Wother,
                        _("symbol %s redeclared"), sym->tag);
              subcomplain (&sym->location, Wother,
                           _("previous declaration"));
            }
          وإلا
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

فراغ
سيمبول_code_set (سيمبول *sym, صحيح code, location loc)
{
  صحيح *codep = &sym->content->code;
  إذا (sym->content->class != token_sym)
    complain (&loc, complaint,
              _("nonterminals cannot be given a token code"));
  وإلا إذا (*codep != CODE_UNDEFINED
           && *codep != code)
    complain (&loc, complaint, _("redefining code of token %s"),
              sym->tag);
  وإلا إذا (code == INT_MAX)
    complain (&loc, complaint, _("code of token %s too large"),
              sym->tag);
  وإلا
    {
      *codep = code;
      /* User defined $end token? */
      إذا (code == 0 && !eoftoken)
        {
          eoftoken = sym->content->سيمبول;
          eoftoken->content->نومبور = 0;
        }
    }
}


/*----------------------------------------------------------.
| If SYM is not defined, report an error, and consider it a |
| nonterminal.                                              |
`----------------------------------------------------------*/

ساكن فراغ
سيمبول_check_defined (سيمبول *sym)
{
  sym_content *s = sym->content;
  إذا (s->class == unknown_sym || s->class == pct_type_sym)
    {
      complain_سيمبول_undeclared (sym);
      s->class = nterm_sym;
    }

  إذا (s->نومبور == نومبور_UNDEFINED)
    s->نومبور = s->class == token_sym ? ntokens++ : nnterms++;

  إذا (s->class == token_sym
      && sym->tag[0] == '"'
      && !sym->is_alias)
    complain (&sym->location, Wdangling_alias,
              _("string literal %s not attached to a symbol"),
              sym->tag);

  لكل (صحيح i = 0; i < 2; ++i)
    سيمبول_code_props_get (sym, i)->is_used = true;

  /* Set the semantic type status associated to the current symbol to
     'declared' so that we could check semantic types unnecessary uses. */
  إذا (s->type_name)
    {
      semantic_type *sem_type = semantic_type_get (s->type_name, عدم);
      إذا (sem_type)
        sem_type->status = declared;
    }
}

ساكن فراغ
semantic_type_check_defined (semantic_type *sem_type)
{
  /* <*> and <> do not have to be "declared".  */
  إذا (sem_type->status == declared
      || !*sem_type->tag
      || STREQ (sem_type->tag, "*"))
    {
      لكل (صحيح i = 0; i < 2; ++i)
        إذا (sem_type->props[i].kind != CODE_PROPS_NONE
            && ! sem_type->props[i].is_used)
          complain (&sem_type->location, Wother,
                    _("useless %s for type <%s>"),
                    code_props_type_string (i), sem_type->tag);
    }
  وإلا
    complain (&sem_type->location, Wother,
              _("type <%s> is used, but is not associated to any symbol"),
              sem_type->tag);
}

/*-------------------------------------------------------------------.
| Merge the properties (precedence, associativity, etc.) of SYM, and |
| its string-named alias STR; check consistency.                     |
`-------------------------------------------------------------------*/

ساكن فراغ
سيمبول_merge_properties (سيمبول *sym, سيمبول *str)
{
  إذا (str->content->type_name != sym->content->type_name)
    {
      إذا (str->content->type_name)
        سيمبول_type_set (sym,
                         str->content->type_name, str->content->type_loc);
      وإلا
        سيمبول_type_set (str,
                         sym->content->type_name, sym->content->type_loc);
    }


  لكل (صحيح i = 0; i < CODE_PROPS_SIZE; ++i)
    إذا (str->content->props[i].code)
      سيمبول_code_props_set (sym, i, &str->content->props[i]);
    وإلا إذا (sym->content->props[i].code)
      سيمبول_code_props_set (str, i, &sym->content->props[i]);

  إذا (sym->content->prec || str->content->prec)
    {
      إذا (str->content->prec)
        سيمبول_precedence_set (sym, str->content->prec, str->content->assoc,
                               str->content->prec_loc);
      وإلا
        سيمبول_precedence_set (str, sym->content->prec, sym->content->assoc,
                               sym->content->prec_loc);
    }
}

فراغ
سيمبول_make_alias (سيمبول *sym, سيمبول *str, location loc)
{
  إذا (sym->content->class != token_sym)
    complain (&loc, complaint,
              _("nonterminals cannot be given a string alias"));
  وإلا إذا (str->alias)
    complain (&loc, Wother,
              _("symbol %s used more than once as a literal string"), str->tag);
  وإلا إذا (sym->alias)
    complain (&loc, Wother,
              _("symbol %s given more than one literal string"), sym->tag);
  وإلا
    {
      سيمبول_merge_properties (sym, str);
      sym_content_free (str->content);
      str->content = sym->content;
      str->content->سيمبول = str;
      str->is_alias = true;
      str->alias = sym;
      sym->alias = str;
    }
}


/*-------------------------------------------------------------------.
| Assign a symbol number, and write the definition of the token name |
| into FDEFINES.  Put in SYMBOLS.                                    |
`-------------------------------------------------------------------*/

ساكن فراغ
سيمبول_pack (سيمبول *sym)
{
  aver (sym->content->نومبور != نومبور_UNDEFINED);
  إذا (sym->content->class == nterm_sym)
    sym->content->نومبور += ntokens;

  symbols[sym->content->نومبور] = sym->content->سيمبول;
}

ساكن فراغ
complain_code_redeclared (صحيح اڠک, ثابت سيمبول *first, ثابت سيمبول *second)
{
  symbols_sort (&first, &second);
  complain (&second->location, complaint,
            _("code %d reassigned to token %s"),
            اڠک, second->tag);
  subcomplain (&first->location, complaint,
               _("previous declaration for %s"),
               first->tag);
}

/*-------------------------------------------------.
| Put SYM in TOKEN_TRANSLATIONS if it is a token.  |
`-------------------------------------------------*/

ساكن فراغ
سيمبول_translation (ثابت سيمبول *sym)
{
  إذا (sym->content->class == token_sym && !sym->is_alias)
    {
      /* A token whose translation has already been set? */
      إذا (token_translations[sym->content->code]
          != undeftoken->content->نومبور)
        complain_code_redeclared
          (sym->content->code,
           symbols[token_translations[sym->content->code]], sym);
      وإلا
        token_translations[sym->content->code]
          = sym->content->نومبور;
    }
}


/*---------------------------------------.
| Symbol and semantic type hash tables.  |
`---------------------------------------*/

/* Initial capacity of symbol and semantic type hash table.  */
#define HT_INITIAL_CAPACITY 257

ساكن مضمن bool
hash_compare_سيمبول (ثابت سيمبول *m1, ثابت سيمبول *m2)
{
  /* Since tags are unique, we can compare the pointers themselves.  */
  أرجع UNIQSTR_EQ (m1->tag, m2->tag);
}

ساكن مضمن bool
hash_compare_semantic_type (ثابت semantic_type *m1, ثابت semantic_type *m2)
{
  /* Since names are unique, we can compare the pointers themselves.  */
  أرجع UNIQSTR_EQ (m1->tag, m2->tag);
}

ساكن bool
hash_سيمبول_comparator (فراغ ثابت *m1, فراغ ثابت *m2)
{
  أرجع hash_compare_سيمبول (m1, m2);
}

ساكن bool
hash_semantic_type_comparator (فراغ ثابت *m1, فراغ ثابت *m2)
{
  أرجع hash_compare_semantic_type (m1, m2);
}

ساكن مضمن size_t
hash_سيمبول (ثابت سيمبول *m, size_t tablesize)
{
  /* Since tags are unique, we can hash the pointer itself.  */
  أرجع ((uintptr_t) m->tag) % tablesize;
}

ساكن مضمن size_t
hash_semantic_type (ثابت semantic_type *m, size_t tablesize)
{
  /* Since names are unique, we can hash the pointer itself.  */
  أرجع ((uintptr_t) m->tag) % tablesize;
}

ساكن size_t
hash_سيمبول_hasher (فراغ ثابت *m, size_t tablesize)
{
  أرجع hash_سيمبول (m, tablesize);
}

ساكن size_t
hash_semantic_type_hasher (فراغ ثابت *m, size_t tablesize)
{
  أرجع hash_semantic_type (m, tablesize);
}

/*-------------------------------.
| Create the symbol hash table.  |
`-------------------------------*/

فراغ
symbols_new (فراغ)
{
  سيمبول_table = hash_xinitialize (HT_INITIAL_CAPACITY,
                                   عدم,
                                   hash_سيمبول_hasher,
                                   hash_سيمبول_comparator,
                                   سيمبول_free);

  /* Construct the acceptsymbol symbol. */
  acceptsymbol = سيمبول_get ("$accept", empty_loc);
  acceptsymbol->content->class = nterm_sym;
  acceptsymbol->content->نومبور = nnterms++;

  /* Construct the YYerror/"error" token */
  errtoken = سيمبول_get ("YYerror", empty_loc);
  errtoken->content->class = token_sym;
  errtoken->content->نومبور = ntokens++;
  {
    سيمبول *alias = سيمبول_get ("error", empty_loc);
    سيمبول_class_set (alias, token_sym, empty_loc, false);
    سيمبول_make_alias (errtoken, alias, empty_loc);
  }

  /* Construct the YYUNDEF/"$undefined" token that represents all
     undefined literal tokens.  It is always symbol number 2.  */
  undeftoken = سيمبول_get ("YYUNDEF", empty_loc);
  undeftoken->content->class = token_sym;
  undeftoken->content->نومبور = ntokens++;
  {
    سيمبول *alias = سيمبول_get ("$undefined", empty_loc);
    سيمبول_class_set (alias, token_sym, empty_loc, false);
    سيمبول_make_alias (undeftoken, alias, empty_loc);
  }

  semantic_type_table = hash_xinitialize (HT_INITIAL_CAPACITY,
                                          عدم,
                                          hash_semantic_type_hasher,
                                          hash_semantic_type_comparator,
                                          حرر);
}


/*----------------------------------------------------------------.
| Find the symbol named KEY, and return it.  If it does not exist |
| yet, create it.                                                 |
`----------------------------------------------------------------*/

سيمبول *
سيمبول_from_uniqstr (ثابت uniqstr کونچي, location loc)
{
  سيمبول probe;

  probe.tag = کونچي;
  سيمبول *res = hash_lookup (سيمبول_table, &probe);

  إذا (!res)
    {
      /* First insertion in the hash. */
      aver (!symbols_sorted);
      res = سيمبول_new (کونچي, loc);
      hash_xinsert (سيمبول_table, res);
    }
  أرجع res;
}


/*-----------------------------------------------------------------------.
| Find the semantic type named KEY, and return it.  If it does not exist |
| yet, create it.                                                        |
`-----------------------------------------------------------------------*/

semantic_type *
semantic_type_from_uniqstr (ثابت uniqstr کونچي, ثابت location *loc)
{
  semantic_type probe;

  probe.tag = کونچي;
  semantic_type *res = hash_lookup (semantic_type_table, &probe);

  إذا (!res)
    {
      /* First insertion in the hash. */
      res = semantic_type_new (کونچي, loc);
      hash_xinsert (semantic_type_table, res);
    }
  أرجع res;
}


/*----------------------------------------------------------------.
| Find the symbol named KEY, and return it.  If it does not exist |
| yet, create it.                                                 |
`----------------------------------------------------------------*/

سيمبول *
سيمبول_get (ثابت حرف *کونچي, location loc)
{
  أرجع سيمبول_from_uniqstr (uniqstr_new (کونچي), loc);
}


/*-----------------------------------------------------------------------.
| Find the semantic type named KEY, and return it.  If it does not exist |
| yet, create it.                                                        |
`-----------------------------------------------------------------------*/

semantic_type *
semantic_type_get (ثابت حرف *کونچي, ثابت location *loc)
{
  أرجع semantic_type_from_uniqstr (uniqstr_new (کونچي), loc);
}


/*------------------------------------------------------------------.
| Generate a dummy nonterminal, whose name cannot conflict with the |
| user's names.                                                     |
`------------------------------------------------------------------*/

سيمبول *
dummy_سيمبول_get (location loc)
{
  /* Incremented for each generated symbol.  */
  ساكن صحيح dummy_کيراءن = 0;
  حرف buf[32];
  صحيح len = snprintf (buf, حجم buf, "$@%d", ++dummy_کيراءن);
  assure (len < حجم buf);
  سيمبول *sym = سيمبول_get (buf, loc);
  sym->content->class = nterm_sym;
  أرجع sym;
}

bool
سيمبول_is_dummy (سيمبول ثابت *sym)
{
  أرجع sym->tag[0] == '@' || (sym->tag[0] == '$' && sym->tag[1] == '@');
}

/*-------------------.
| Free the symbols.  |
`-------------------*/

فراغ
symbols_free (فراغ)
{
  hash_free (سيمبول_table);
  hash_free (semantic_type_table);
  حرر (symbols);
  حرر (symbols_sorted);
  حرر (semantic_types_sorted);
}


ساكن صحيح
سيمبول_cmp (فراغ ثابت *a, فراغ ثابت *b)
{
  أرجع location_cmp ((*(سيمبول * ثابت *)a)->location,
                       (*(سيمبول * ثابت *)b)->location);
}

/* Store in *SORTED an array of pointers to the symbols contained in
   TABLE, sorted by order of appearance (i.e., by location). */

ساكن فراغ
table_sort (بنية hash_table *table, سيمبول ***sorted)
{
  aver (!*sorted);
  size_t کيراءن = hash_get_n_entries (table);
  *sorted = xnmalloc (کيراءن + 1, حجم **sorted);
  hash_get_entries (table, (فراغ**)*sorted, کيراءن);
  qsort (*sorted, کيراءن, حجم **sorted, سيمبول_cmp);
  (*sorted)[کيراءن] = عدم;
}

/*--------------------------------------------------------------.
| Check that all the symbols are defined.  Report any undefined |
| symbols and consider them nonterminals.                       |
`--------------------------------------------------------------*/

فراغ
symbols_check_defined (فراغ)
{
  table_sort (سيمبول_table, &symbols_sorted);
  /* semantic_type, like symbol, starts with a 'tag' field and then a
     'location' field.  And here we only deal with arrays/hashes of
     pointers, sizeof is not an issue.

     So instead of implementing table_sort (and symbol_cmp) once for
     each type, let's lie a bit to the typing system, and treat
     'semantic_type' as if it were 'symbol'. */
  table_sort (semantic_type_table, (سيمبول ***) &semantic_types_sorted);

  لكل (صحيح i = 0; symbols_sorted[i]; ++i)
    سيمبول_check_defined (symbols_sorted[i]);
  لكل (صحيح i = 0; semantic_types_sorted[i]; ++i)
    semantic_type_check_defined (semantic_types_sorted[i]);
}

/*------------------------------------------------------------------.
| Set TOKEN_TRANSLATIONS.  Check that no two symbols share the same |
| number.                                                           |
`------------------------------------------------------------------*/

ساكن فراغ
symbols_token_translations_init (فراغ)
{
  bool code_256_available_p = true;

  /* Find the highest token code, and whether 256, the POSIX preferred
     token code for the error token, is used.  */
  max_code = 0;
  لكل (صحيح i = 0; i < ntokens; ++i)
    {
      sym_content *sym = symbols[i]->content;
      إذا (sym->code != CODE_UNDEFINED)
        {
          إذا (sym->code > max_code)
            max_code = sym->code;
          إذا (sym->code == 256)
            code_256_available_p = false;
        }
    }

  /* If 256 is not used, assign it to error, to follow POSIX.  */
  إذا (code_256_available_p
      && errtoken->content->code == CODE_UNDEFINED)
    errtoken->content->code = 256;

  /* Set the missing codes. */
  إذا (max_code < 256)
    max_code = 256;

  لكل (صحيح i = 0; i < ntokens; ++i)
    {
      sym_content *sym = symbols[i]->content;
      إذا (sym->code == CODE_UNDEFINED)
        {
          IGNORE_TYPE_LIMITS_BEGIN
          إذا (INT_ADD_WRAPV (max_code, 1, &max_code))
            complain (عدم, fatal, _("token number too large"));
          IGNORE_TYPE_LIMITS_END
          sym->code = max_code;
        }
      إذا (sym->code > max_code)
        max_code = sym->code;
    }

  token_translations = xnmalloc (max_code + 1,
                                 حجم *token_translations);

  /* Initialize all entries for literal tokens to the internal token
     number for $undefined, which represents all invalid inputs.  */
  لكل (صحيح i = 0; i < max_code + 1; ++i)
    token_translations[i] = undeftoken->content->نومبور;
  لكل (صحيح i = 0; symbols_sorted[i]; ++i)
    سيمبول_translation (symbols_sorted[i]);
}


/* Whether some symbol requires internationalization.  */
ساكن bool
has_translations (فراغ)
{
  لكل (ثابت فراغ *entry = hash_get_first (سيمبول_table);
       entry;
       entry = hash_get_next (سيمبول_table, entry))
    {
      ثابت سيمبول *sym = (ثابت سيمبول *) entry;
      إذا (sym->translatable)
        أرجع true;
    }
  أرجع false;
}

/*----------------------------------------------------------------.
| Assign symbol numbers, and write definition of token names into |
| FDEFINES.  Set up vectors SYMBOL_TABLE, TAGS of symbols.        |
`----------------------------------------------------------------*/

فراغ
symbols_pack (فراغ)
{
  symbols = xcalloc (nsyms, حجم *symbols);
  لكل (صحيح i = 0; symbols_sorted[i]; ++i)
    سيمبول_pack (symbols_sorted[i]);

  /* Aliases leave empty slots in symbols, so remove them.  */
  {
    صحيح nsyms_old = nsyms;
    لكل (صحيح writei = 0, readi = 0; readi < nsyms_old; readi += 1)
      {
        إذا (symbols[readi] == عدم)
          {
            nsyms -= 1;
            ntokens -= 1;
          }
        وإلا
          {
            symbols[writei] = symbols[readi];
            symbols[writei]->content->نومبور = writei;
            writei += 1;
          }
      }
  }
  symbols = xnrealloc (symbols, nsyms, حجم *symbols);

  symbols_token_translations_init ();

  // If some user tokens are internationalized, the internal ones
  // should be too.
  إذا (has_translations ())
    {
      ثابت bool eof_is_user_defined
        = !eoftoken->alias || STRNEQ (eoftoken->alias->tag, "$end");
      إذا (!eof_is_user_defined)
        eoftoken->alias->translatable = true;
      undeftoken->alias->translatable = true;
      errtoken->alias->translatable = true;
    }
}

/*---------------------------------.
| Initialize relation graph nodes. |
`---------------------------------*/

ساكن فراغ
init_prec_nodes (فراغ)
{
  prec_nodes = xcalloc (nsyms, حجم *prec_nodes);
  لكل (صحيح i = 0; i < nsyms; ++i)
    {
      prec_nodes[i] = xmalloc (حجم *prec_nodes[i]);
      symgraph *s = prec_nodes[i];
      s->id = i;
      s->succ = 0;
      s->pred = 0;
    }
}

/*----------------.
| Create a link.  |
`----------------*/

ساكن symgraphlink *
symgraphlink_new (graphid id, symgraphlink *next)
{
  symgraphlink *res = xmalloc (حجم *res);
  res->id = id;
  res->next = next;
  أرجع res;
}


/*------------------------------------------------------------------.
| Register the second symbol of the precedence relation, and return |
| whether this relation is new.  Use only in register_precedence.   |
`------------------------------------------------------------------*/

ساكن bool
register_precedence_second_سيمبول (symgraphlink **first, graphid sym)
{
  إذا (!*first || sym < (*first)->id)
    *first = symgraphlink_new (sym, *first);
  وإلا
    {
      symgraphlink *slist = *first;

      طالما (slist->next && slist->next->id <= sym)
        slist = slist->next;

      إذا (slist->id == sym)
        /* Relation already present. */
        أرجع false;

      slist->next = symgraphlink_new (sym, slist->next);
    }
  أرجع true;
}

/*------------------------------------------------------------------.
| Register a new relation between symbols as used. The first symbol |
| has a greater precedence than the second one.                     |
`------------------------------------------------------------------*/

فراغ
register_precedence (graphid first, graphid snd)
{
  إذا (!prec_nodes)
    init_prec_nodes ();
  register_precedence_second_سيمبول (&(prec_nodes[first]->succ), snd);
  register_precedence_second_سيمبول (&(prec_nodes[snd]->pred), first);
}


/*---------------------------------------.
| Deep clear a linked / adjacency list). |
`---------------------------------------*/

ساكن فراغ
linkedlist_free (symgraphlink *نود)
{
  إذا (نود)
    {
      طالما (نود->next)
        {
          symgraphlink *tmp = نود->next;
          حرر (نود);
          نود = tmp;
        }
      حرر (نود);
    }
}

/*----------------------------------------------.
| Clear and destroy association tracking table. |
`----------------------------------------------*/

ساكن فراغ
assoc_free (فراغ)
{
  لكل (صحيح i = 0; i < nsyms; ++i)
    {
      linkedlist_free (prec_nodes[i]->pred);
      linkedlist_free (prec_nodes[i]->succ);
      حرر (prec_nodes[i]);
    }
  حرر (prec_nodes);
}

/*---------------------------------------.
| Initialize association tracking table. |
`---------------------------------------*/

ساكن فراغ
init_assoc (فراغ)
{
  used_assoc = xcalloc (nsyms, حجم *used_assoc);
  لكل (graphid i = 0; i < nsyms; ++i)
    used_assoc[i] = false;
}

/*------------------------------------------------------------------.
| Test if the associativity for the symbols is defined and useless. |
`------------------------------------------------------------------*/

ساكن مضمن bool
is_assoc_useless (سيمبول *s)
{
  أرجع s
      && s->content->assoc != undef_assoc
      && s->content->assoc != precedence_assoc
      && !used_assoc[s->content->نومبور];
}

/*-------------------------------.
| Register a used associativity. |
`-------------------------------*/

فراغ
register_assoc (graphid i, graphid j)
{
  إذا (!used_assoc)
    init_assoc ();
  used_assoc[i] = true;
  used_assoc[j] = true;
}

/*--------------------------------------------------.
| Print a warning for unused precedence relations.  |
`--------------------------------------------------*/

فراغ
print_precedence_warnings (فراغ)
{
  إذا (!prec_nodes)
    init_prec_nodes ();
  إذا (!used_assoc)
    init_assoc ();
  لكل (صحيح i = 0; i < nsyms; ++i)
    {
      سيمبول *s = symbols[i];
      إذا (s
          && s->content->prec != 0
          && !prec_nodes[i]->pred
          && !prec_nodes[i]->succ)
        {
          إذا (is_assoc_useless (s))
            complain (&s->content->prec_loc, Wprecedence,
                      _("useless precedence and associativity for %s"), s->tag);
          وإلا إذا (s->content->assoc == precedence_assoc)
            complain (&s->content->prec_loc, Wprecedence,
                      _("useless precedence for %s"), s->tag);
        }
      وإلا إذا (is_assoc_useless (s))
        complain (&s->content->prec_loc, Wprecedence,
                  _("useless associativity for %s, use %%precedence"), s->tag);
    }
  حرر (used_assoc);
  assoc_free ();
}
