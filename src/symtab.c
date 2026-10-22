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
#تعریف_کړه کوډ_UNDEFINED (-1)

/* Undefined symbol number.  */
#تعریف_کړه شمېره_UNDEFINED (-1)


ساكن بنية hash_جدول *نښه_جدول = عدم;
ساكن بنية hash_جدول *semantic_type_جدول = عدم;

/*----------------------------------------------------------------.
| Symbols sorted by tag.  Allocated by table_sort, after which no |
| more symbols should be created.                                 |
`----------------------------------------------------------------*/

ساكن نښه **symbols_sorted = عدم;
ساكن semantic_type **semantic_types_sorted = عدم;


/*------------------------.
| Distinguished symbols.  |
`------------------------*/

نښه *errtoken = عدم;
نښه *undeftoken = عدم;
نښه *eoftoken = عدم;
نښه *acceptsymbol = عدم;

/* Precedence relation graph. */
ساكن symgraph **prec_nodes;

/* Store which associativity is used.  */
ساكن bool *کارول_شوی_assoc = عدم;

bool نښان_کتل_شوی = false;


/* Whether SYM was defined by the user.  */

ساكن bool
نښه_is_user_defined (نښه *sym)
{
  ثابت bool eof_is_user_defined
    = !eoftoken->alias || STRNEQ (eoftoken->alias->نښان, "$end");
  أرجع sym->نښان[0] != '$'
    && (eof_is_user_defined || (sym != eoftoken && sym->alias != errtoken))
    && sym != errtoken && sym->alias != errtoken
    && sym != undeftoken && sym->alias != undeftoken;
}


/*--------------------------.
| Create a new sym_content. |
`--------------------------*/

ساكن sym_content *
sym_content_new (نښه *s)
{
  sym_content *res = xmalloc (حجم *res);

  res->نښه = s;

  res->type_name = عدم;
  res->type_loc = تشه_loc;
  لكل (صحيح i = 0; i < کوډ_PROPS_اندازه; ++i)
    کوډ_props_none_init (&res->props[i]);

  res->شمېره = شمېره_UNDEFINED;
  res->prec_loc = تشه_loc;
  res->prec = 0;
  res->assoc = undef_assoc;
  res->کوډ = کوډ_UNDEFINED;

  res->class = unknown_sym;
  res->حالت = undeclared;

  أرجع res;
}

/*---------------------------------.
| Create a new symbol, named TAG.  |
`---------------------------------*/

ساكن نښه *
نښه_new (uniqstr نښان, location loc)
{
  نښه *res = xmalloc (حجم *res);
  uniqstr_assert (نښان);

  /* If the tag is not a string (starts with a double quote), check
     that it is valid for Yacc. */
  إذا (نښان[0] != '\"' && نښان[0] != '\'' && strchr (نښان, '-'))
    complain (&loc, Wyacc,
              _("POSIX Yacc forbids dashes in symbol names: %s"), نښان);

  res->نښان = نښان;
  res->location = loc;
  res->translatable = false;
  res->location_د_lhs = false;
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
نښه_free (فراغ *ptr)
{
  نښه *sym = (نښه *)ptr;
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
symbols_ترتیبول (ثابت نښه **لومړی, ثابت نښه **دوهم)
{
  إذا (0 < location_cmp ((*لومړی)->location, (*دوهم)->location))
    {
      ثابت نښه* tmp = *لومړی;
      *لومړی = *دوهم;
      *دوهم = tmp;
    }
}

/* Likewise, for locations.  */

ساكن فراغ
locations_ترتیبول (location *لومړی, location *دوهم)
{
  إذا (0 < location_cmp (*لومړی, *دوهم))
    {
      location tmp = *لومړی;
      *لومړی = *دوهم;
      *دوهم = tmp;
    }
}

حرف ثابت *
کوډ_props_type_string (کوډ_props_type ډول)
{
  بدل (ډول)
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
semantic_type_new (uniqstr نښان, ثابت location *loc)
{
  semantic_type *res = xmalloc (حجم *res);

  uniqstr_assert (نښان);
  res->نښان = نښان;
  res->location = loc ? *loc : تشه_loc;
  res->حالت = undeclared;
  لكل (صحيح i = 0; i < کوډ_PROPS_اندازه; ++i)
    کوډ_props_none_init (&res->props[i]);

  أرجع res;
}


/*-----------------.
| Print a symbol.  |
`-----------------*/

#تعریف_کړه نښه_INT_ATTR_PRINT(Attr)                     \
  إذا (s->content)                                       \
    fprintf (f, " %s = %d", #Attr, s->content->Attr)

#تعریف_کړه نښه_STR_ATTR_PRINT(Attr)                     \
  إذا (s->content && s->content->Attr)                   \
    fprintf (f, " %s { %s }", #Attr, s->content->Attr)

#تعریف_کړه نښه_کوډ_PRINT(Attr)                                         \
  إذا (s->content && s->content->props[Attr].کوډ)                       \
    fprintf (f, " %s { %s }", #Attr, s->content->props[Attr].کوډ)

فراغ
نښه_print (نښه ثابت *s, FILE *f)
{
  إذا (s)
    {
      نښه_class c = s->content->class;
      fprintf (f, "%s: %s",
               c == unknown_sym    ? "unknown"
               : c == pct_type_sym ? "%type"
               : c == token_sym    ? "token"
               : c == nterm_sym    ? "nterm"
               : عدم, /* abort.  */
               s->نښان);
      putc (' ', f);
      location_print (s->location, f);
      نښه_INT_ATTR_PRINT (کوډ);
      نښه_INT_ATTR_PRINT (شمېره);
      نښه_STR_ATTR_PRINT (type_name);
      نښه_کوډ_PRINT (destructor);
      نښه_کوډ_PRINT (printer);
    }
  وإلا
    fputs ("<NULL>", f);
}

#تعریف_لغوه نښه_ATTR_PRINT
#تعریف_لغوه نښه_کوډ_PRINT


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
نښه_id_get (نښه ثابت *sym)
{
  // There's one weird case: YYerror is the alias, and error is the
  // base symbol.  Return YYerror in that case.
  إذا (sym->alias && is_identifier (sym->alias->نښان))
    أرجع sym->alias->نښان;
  وإلا إذا (is_identifier (sym->نښان))
    أرجع sym->نښان;
  وإلا
    أرجع عدم;
}


/*------------------------------------------------------------------.
| Complain that S's WHAT is redeclared at SECOND, and was first set |
| at FIRST.                                                         |
`------------------------------------------------------------------*/

ساكن فراغ
complain_نښه_redeclared (نښه *s, ثابت حرف *what, location لومړی,
                            location دوهم)
{
  locations_ترتیبول (&لومړی, &دوهم);
  complain (&دوهم, complaint, _("%s redeclaration for %s"), what, s->نښان);
  subcomplain (&لومړی, complaint, _("previous declaration"));
}

ساكن فراغ
complain_semantic_type_redeclared (semantic_type *s, ثابت حرف *what, location لومړی,
                                   location دوهم)
{
  locations_ترتیبول (&لومړی, &دوهم);
  complain (&دوهم, complaint, _("%s redeclaration for <%s>"), what, s->نښان);
  subcomplain (&لومړی, complaint, _("previous declaration"));
}

ساكن فراغ
complain_class_redeclared (نښه *sym, نښه_class class, location دوهم)
{
  complain (&دوهم, complaint,
            class == token_sym
            ? _("symbol %s redeclared as a token")
            : _("symbol %s redeclared as a nonterminal"), sym->نښان);
  إذا (!location_تشه (sym->location))
    subcomplain (&sym->location, complaint, _("previous definition"));
}

ساكن ثابت نښه *
نښه_from_uniqstr_fuzzy (ثابت uniqstr کلی)
{
  aver (symbols_sorted);
#تعریف_کړه FSTRCMP_پوله 0.6
  مزدوج غوره_similarity = FSTRCMP_پوله;
  ثابت نښه *res = عدم;
  size_t شمېر = hash_get_n_entries (نښه_جدول);
  لكل (صحيح i = 0; i < شمېر; ++i)
    {
      نښه *sym = symbols_sorted[i];
      إذا (STRNEQ (کلی, sym->نښان)
          && (sym->content->حالت == declared
              || sym->content->حالت == undeclared))
        {
          مزدوج similarity = fstrcmp_bounded (کلی, sym->نښان, غوره_similarity);
          إذا (غوره_similarity < similarity)
            {
              res = sym;
              غوره_similarity = similarity;
            }
        }
    }
  أرجع res;
}

ساكن فراغ
complain_نښه_undeclared (ثابت نښه *sym)
{
  أكد (sym->content->حالت != declared);
  ثابت نښه *غوره = نښه_from_uniqstr_fuzzy (sym->نښان);
  إذا (غوره)
    {
      complain (&sym->location,
                sym->content->حالت == needed ? complaint : Wother,
                _("symbol %s is used, but is not defined as a token"
                  " and has no rules; did you mean %s?"),
                quote_n (0, sym->نښان),
                quote_n (1, غوره->نښان));
      إذا (feature_flag & feature_caret)
        location_caret_suggestion (sym->location, غوره->نښان, stderr);
    }
  وإلا
    complain (&sym->location,
              sym->content->حالت == needed ? complaint : Wother,
              _("symbol %s is used, but is not defined as a token"
                " and has no rules"),
              quote (sym->نښان));
}

فراغ
نښه_location_as_lhs_سېټ (نښه *sym, location loc)
{
  إذا (!sym->location_د_lhs)
    {
      sym->location = loc;
      sym->location_د_lhs = true;
    }
}


/*-----------------------------------------------------------------.
| Set the TYPE_NAME associated with SYM.  Does nothing if passed 0 |
| as TYPE_NAME.                                                    |
`-----------------------------------------------------------------*/

فراغ
نښه_type_سېټ (نښه *sym, uniqstr type_name, location loc)
{
  إذا (type_name)
    {
      نښان_کتل_شوی = true;
      إذا (sym->content->type_name)
        complain_نښه_redeclared (sym, "%type",
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
نښه_کوډ_props_سېټ (نښه *sym, کوډ_props_type ډول,
                       کوډ_props ثابت *کوډ)
{
  إذا (sym->content->props[ډول].کوډ)
    complain_نښه_redeclared (sym, کوډ_props_type_string (ډول),
                                sym->content->props[ډول].location,
                                کوډ->location);
  وإلا
    sym->content->props[ډول] = *کوډ;
}


/*-----------------------------------------------------.
| Set the DESTRUCTOR or PRINTER associated with TYPE.  |
`-----------------------------------------------------*/

فراغ
semantic_type_کوډ_props_سېټ (semantic_type *type,
                              کوډ_props_type ډول,
                              کوډ_props ثابت *کوډ)
{
  إذا (type->props[ډول].کوډ)
    complain_semantic_type_redeclared (type, کوډ_props_type_string (ډول),
                                       type->props[ډول].location,
                                       کوډ->location);
  وإلا
    type->props[ډول] = *کوډ;
}

/*---------------------------------------------------.
| Get the computed %destructor or %printer for SYM.  |
`---------------------------------------------------*/

کوډ_props *
نښه_کوډ_props_get (نښه *sym, کوډ_props_type ډول)
{
  /* Per-symbol code props.  */
  إذا (sym->content->props[ډول].کوډ)
    أرجع &sym->content->props[ډول];

  /* Per-type code props.  */
  إذا (sym->content->type_name)
    {
      کوډ_props *کوډ =
        &semantic_type_get (sym->content->type_name, عدم)->props[ډول];
      إذا (کوډ->کوډ)
        أرجع کوډ;
    }

  /* Apply default code props's only to user-defined symbols.  */
  إذا (نښه_is_user_defined (sym))
    {
      کوډ_props *کوډ = &semantic_type_get (sym->content->type_name ? "*" : "",
                                             عدم)->props[ډول];
      إذا (کوډ->کوډ)
        أرجع کوډ;
    }
  أرجع &کوډ_props_none;
}

/*-----------------------------------------------------------------.
| Set the PRECEDENCE associated with SYM.  Does nothing if invoked |
| with UNDEF_ASSOC as ASSOC.                                       |
`-----------------------------------------------------------------*/

فراغ
نښه_precedence_سېټ (نښه *sym, صحيح prec, assoc a, location loc)
{
  إذا (a != undef_assoc)
    {
      sym_content *s = sym->content;
      إذا (s->prec)
        complain_نښه_redeclared (sym, assoc_منزل_string (a),
                                    s->prec_loc, loc);
      وإلا
        {
          s->prec = prec;
          s->assoc = a;
          s->prec_loc = loc;
        }
    }

  /* Only terminals have a precedence. */
  نښه_class_سېټ (sym, token_sym, loc, false);
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
نښه_class_سېټ (نښه *sym, نښه_class class, location loc, bool declaring)
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
          إذا (s->حالت == declared)
            {
              complain (&loc, Wother,
                        _("symbol %s redeclared"), sym->نښان);
              subcomplain (&sym->location, Wother,
                           _("previous declaration"));
            }
          وإلا
            {
              sym->location = loc;
              s->حالت = declared;
            }
        }
    }
}


/*----------------------------.
| Set the token code of SYM.  |
`----------------------------*/

فراغ
نښه_کوډ_سېټ (نښه *sym, صحيح کوډ, location loc)
{
  صحيح *codep = &sym->content->کوډ;
  إذا (sym->content->class != token_sym)
    complain (&loc, complaint,
              _("nonterminals cannot be given a token code"));
  وإلا إذا (*codep != کوډ_UNDEFINED
           && *codep != کوډ)
    complain (&loc, complaint, _("redefining code of token %s"),
              sym->نښان);
  وإلا إذا (کوډ == لوی_عدد)
    complain (&loc, complaint, _("code of token %s too large"),
              sym->نښان);
  وإلا
    {
      *codep = کوډ;
      /* User defined $end token? */
      إذا (کوډ == 0 && !eoftoken)
        {
          eoftoken = sym->content->نښه;
          eoftoken->content->شمېره = 0;
        }
    }
}


/*----------------------------------------------------------.
| If SYM is not defined, report an error, and consider it a |
| nonterminal.                                              |
`----------------------------------------------------------*/

ساكن فراغ
نښه_check_defined (نښه *sym)
{
  sym_content *s = sym->content;
  إذا (s->class == unknown_sym || s->class == pct_type_sym)
    {
      complain_نښه_undeclared (sym);
      s->class = nterm_sym;
    }

  إذا (s->شمېره == شمېره_UNDEFINED)
    s->شمېره = s->class == token_sym ? ntokens++ : nnterms++;

  إذا (s->class == token_sym
      && sym->نښان[0] == '"'
      && !sym->is_alias)
    complain (&sym->location, Wdangling_alias,
              _("string literal %s not attached to a symbol"),
              sym->نښان);

  لكل (صحيح i = 0; i < 2; ++i)
    نښه_کوډ_props_get (sym, i)->is_کارول_شوی = true;

  /* Set the semantic type status associated to the current symbol to
     'declared' so that we could check semantic types unnecessary uses. */
  إذا (s->type_name)
    {
      semantic_type *sem_type = semantic_type_get (s->type_name, عدم);
      إذا (sem_type)
        sem_type->حالت = declared;
    }
}

ساكن فراغ
semantic_type_check_defined (semantic_type *sem_type)
{
  /* <*> and <> do not have to be "declared".  */
  إذا (sem_type->حالت == declared
      || !*sem_type->نښان
      || STREQ (sem_type->نښان, "*"))
    {
      لكل (صحيح i = 0; i < 2; ++i)
        إذا (sem_type->props[i].ډول != کوډ_PROPS_NONE
            && ! sem_type->props[i].is_کارول_شوی)
          complain (&sem_type->location, Wother,
                    _("useless %s for type <%s>"),
                    کوډ_props_type_string (i), sem_type->نښان);
    }
  وإلا
    complain (&sem_type->location, Wother,
              _("type <%s> is used, but is not associated to any symbol"),
              sem_type->نښان);
}

/*-------------------------------------------------------------------.
| Merge the properties (precedence, associativity, etc.) of SYM, and |
| its string-named alias STR; check consistency.                     |
`-------------------------------------------------------------------*/

ساكن فراغ
نښه_یوځای_properties (نښه *sym, نښه *str)
{
  إذا (str->content->type_name != sym->content->type_name)
    {
      إذا (str->content->type_name)
        نښه_type_سېټ (sym,
                         str->content->type_name, str->content->type_loc);
      وإلا
        نښه_type_سېټ (str,
                         sym->content->type_name, sym->content->type_loc);
    }


  لكل (صحيح i = 0; i < کوډ_PROPS_اندازه; ++i)
    إذا (str->content->props[i].کوډ)
      نښه_کوډ_props_سېټ (sym, i, &str->content->props[i]);
    وإلا إذا (sym->content->props[i].کوډ)
      نښه_کوډ_props_سېټ (str, i, &sym->content->props[i]);

  إذا (sym->content->prec || str->content->prec)
    {
      إذا (str->content->prec)
        نښه_precedence_سېټ (sym, str->content->prec, str->content->assoc,
                               str->content->prec_loc);
      وإلا
        نښه_precedence_سېټ (str, sym->content->prec, sym->content->assoc,
                               sym->content->prec_loc);
    }
}

فراغ
نښه_make_alias (نښه *sym, نښه *str, location loc)
{
  إذا (sym->content->class != token_sym)
    complain (&loc, complaint,
              _("nonterminals cannot be given a string alias"));
  وإلا إذا (str->alias)
    complain (&loc, Wother,
              _("symbol %s used more than once as a literal string"), str->نښان);
  وإلا إذا (sym->alias)
    complain (&loc, Wother,
              _("symbol %s given more than one literal string"), sym->نښان);
  وإلا
    {
      نښه_یوځای_properties (sym, str);
      sym_content_free (str->content);
      str->content = sym->content;
      str->content->نښه = str;
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
نښه_pack (نښه *sym)
{
  aver (sym->content->شمېره != شمېره_UNDEFINED);
  إذا (sym->content->class == nterm_sym)
    sym->content->شمېره += ntokens;

  symbols[sym->content->شمېره] = sym->content->نښه;
}

ساكن فراغ
complain_کوډ_redeclared (صحيح عدد, ثابت نښه *لومړی, ثابت نښه *دوهم)
{
  symbols_ترتیبول (&لومړی, &دوهم);
  complain (&دوهم->location, complaint,
            _("code %d reassigned to token %s"),
            عدد, دوهم->نښان);
  subcomplain (&لومړی->location, complaint,
               _("previous declaration for %s"),
               لومړی->نښان);
}

/*-------------------------------------------------.
| Put SYM in TOKEN_TRANSLATIONS if it is a token.  |
`-------------------------------------------------*/

ساكن فراغ
نښه_translation (ثابت نښه *sym)
{
  إذا (sym->content->class == token_sym && !sym->is_alias)
    {
      /* A token whose translation has already been set? */
      إذا (token_translations[sym->content->کوډ]
          != undeftoken->content->شمېره)
        complain_کوډ_redeclared
          (sym->content->کوډ,
           symbols[token_translations[sym->content->کوډ]], sym);
      وإلا
        token_translations[sym->content->کوډ]
          = sym->content->شمېره;
    }
}


/*---------------------------------------.
| Symbol and semantic type hash tables.  |
`---------------------------------------*/

/* Initial capacity of symbol and semantic type hash table.  */
#تعریف_کړه HT_INITIAL_ظرفیت 257

ساكن مضمن bool
hash_compare_نښه (ثابت نښه *m1, ثابت نښه *m2)
{
  /* Since tags are unique, we can compare the pointers themselves.  */
  أرجع UNIQSTR_EQ (m1->نښان, m2->نښان);
}

ساكن مضمن bool
hash_compare_semantic_type (ثابت semantic_type *m1, ثابت semantic_type *m2)
{
  /* Since names are unique, we can compare the pointers themselves.  */
  أرجع UNIQSTR_EQ (m1->نښان, m2->نښان);
}

ساكن bool
hash_نښه_comparator (فراغ ثابت *m1, فراغ ثابت *m2)
{
  أرجع hash_compare_نښه (m1, m2);
}

ساكن bool
hash_semantic_type_comparator (فراغ ثابت *m1, فراغ ثابت *m2)
{
  أرجع hash_compare_semantic_type (m1, m2);
}

ساكن مضمن size_t
hash_نښه (ثابت نښه *m, size_t tablesize)
{
  /* Since tags are unique, we can hash the pointer itself.  */
  أرجع ((uintptr_t) m->نښان) % tablesize;
}

ساكن مضمن size_t
hash_semantic_type (ثابت semantic_type *m, size_t tablesize)
{
  /* Since names are unique, we can hash the pointer itself.  */
  أرجع ((uintptr_t) m->نښان) % tablesize;
}

ساكن size_t
hash_نښه_hasher (فراغ ثابت *m, size_t tablesize)
{
  أرجع hash_نښه (m, tablesize);
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
  نښه_جدول = hash_xinitialize (HT_INITIAL_ظرفیت,
                                   عدم,
                                   hash_نښه_hasher,
                                   hash_نښه_comparator,
                                   نښه_free);

  /* Construct the acceptsymbol symbol. */
  acceptsymbol = نښه_get ("$accept", تشه_loc);
  acceptsymbol->content->class = nterm_sym;
  acceptsymbol->content->شمېره = nnterms++;

  /* Construct the YYerror/"error" token */
  errtoken = نښه_get ("YYerror", تشه_loc);
  errtoken->content->class = token_sym;
  errtoken->content->شمېره = ntokens++;
  {
    نښه *alias = نښه_get ("error", تشه_loc);
    نښه_class_سېټ (alias, token_sym, تشه_loc, false);
    نښه_make_alias (errtoken, alias, تشه_loc);
  }

  /* Construct the YYUNDEF/"$undefined" token that represents all
     undefined literal tokens.  It is always symbol number 2.  */
  undeftoken = نښه_get ("YYUNDEF", تشه_loc);
  undeftoken->content->class = token_sym;
  undeftoken->content->شمېره = ntokens++;
  {
    نښه *alias = نښه_get ("$undefined", تشه_loc);
    نښه_class_سېټ (alias, token_sym, تشه_loc, false);
    نښه_make_alias (undeftoken, alias, تشه_loc);
  }

  semantic_type_جدول = hash_xinitialize (HT_INITIAL_ظرفیت,
                                          عدم,
                                          hash_semantic_type_hasher,
                                          hash_semantic_type_comparator,
                                          حرر);
}


/*----------------------------------------------------------------.
| Find the symbol named KEY, and return it.  If it does not exist |
| yet, create it.                                                 |
`----------------------------------------------------------------*/

نښه *
نښه_from_uniqstr (ثابت uniqstr کلی, location loc)
{
  نښه probe;

  probe.نښان = کلی;
  نښه *res = hash_lookup (نښه_جدول, &probe);

  إذا (!res)
    {
      /* First insertion in the hash. */
      aver (!symbols_sorted);
      res = نښه_new (کلی, loc);
      hash_xinsert (نښه_جدول, res);
    }
  أرجع res;
}


/*-----------------------------------------------------------------------.
| Find the semantic type named KEY, and return it.  If it does not exist |
| yet, create it.                                                        |
`-----------------------------------------------------------------------*/

semantic_type *
semantic_type_from_uniqstr (ثابت uniqstr کلی, ثابت location *loc)
{
  semantic_type probe;

  probe.نښان = کلی;
  semantic_type *res = hash_lookup (semantic_type_جدول, &probe);

  إذا (!res)
    {
      /* First insertion in the hash. */
      res = semantic_type_new (کلی, loc);
      hash_xinsert (semantic_type_جدول, res);
    }
  أرجع res;
}


/*----------------------------------------------------------------.
| Find the symbol named KEY, and return it.  If it does not exist |
| yet, create it.                                                 |
`----------------------------------------------------------------*/

نښه *
نښه_get (ثابت حرف *کلی, location loc)
{
  أرجع نښه_from_uniqstr (uniqstr_new (کلی), loc);
}


/*-----------------------------------------------------------------------.
| Find the semantic type named KEY, and return it.  If it does not exist |
| yet, create it.                                                        |
`-----------------------------------------------------------------------*/

semantic_type *
semantic_type_get (ثابت حرف *کلی, ثابت location *loc)
{
  أرجع semantic_type_from_uniqstr (uniqstr_new (کلی), loc);
}


/*------------------------------------------------------------------.
| Generate a dummy nonterminal, whose name cannot conflict with the |
| user's names.                                                     |
`------------------------------------------------------------------*/

نښه *
dummy_نښه_get (location loc)
{
  /* Incremented for each generated symbol.  */
  ساكن صحيح dummy_شمېر = 0;
  حرف buf[32];
  صحيح len = snprintf (buf, حجم buf, "$@%d", ++dummy_شمېر);
  assure (len < حجم buf);
  نښه *sym = نښه_get (buf, loc);
  sym->content->class = nterm_sym;
  أرجع sym;
}

bool
نښه_is_dummy (نښه ثابت *sym)
{
  أرجع sym->نښان[0] == '@' || (sym->نښان[0] == '$' && sym->نښان[1] == '@');
}

/*-------------------.
| Free the symbols.  |
`-------------------*/

فراغ
symbols_free (فراغ)
{
  hash_free (نښه_جدول);
  hash_free (semantic_type_جدول);
  حرر (symbols);
  حرر (symbols_sorted);
  حرر (semantic_types_sorted);
}


ساكن صحيح
نښه_cmp (فراغ ثابت *a, فراغ ثابت *b)
{
  أرجع location_cmp ((*(نښه * ثابت *)a)->location,
                       (*(نښه * ثابت *)b)->location);
}

/* Store in *SORTED an array of pointers to the symbols contained in
   TABLE, sorted by order of appearance (i.e., by location). */

ساكن فراغ
جدول_ترتیبول (بنية hash_جدول *جدول, نښه ***sorted)
{
  aver (!*sorted);
  size_t شمېر = hash_get_n_entries (جدول);
  *sorted = xnmalloc (شمېر + 1, حجم **sorted);
  hash_get_entries (جدول, (فراغ**)*sorted, شمېر);
  qsort (*sorted, شمېر, حجم **sorted, نښه_cmp);
  (*sorted)[شمېر] = عدم;
}

/*--------------------------------------------------------------.
| Check that all the symbols are defined.  Report any undefined |
| symbols and consider them nonterminals.                       |
`--------------------------------------------------------------*/

فراغ
symbols_check_defined (فراغ)
{
  جدول_ترتیبول (نښه_جدول, &symbols_sorted);
  /* semantic_type, like symbol, starts with a 'tag' field and then a
     'location' field.  And here we only deal with arrays/hashes of
     pointers, sizeof is not an issue.

     So instead of implementing table_sort (and symbol_cmp) once for
     each type, let's lie a bit to the typing system, and treat
     'semantic_type' as if it were 'symbol'. */
  جدول_ترتیبول (semantic_type_جدول, (نښه ***) &semantic_types_sorted);

  لكل (صحيح i = 0; symbols_sorted[i]; ++i)
    نښه_check_defined (symbols_sorted[i]);
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
  bool کوډ_256_شته_p = true;

  /* Find the highest token code, and whether 256, the POSIX preferred
     token code for the error token, is used.  */
  max_کوډ = 0;
  لكل (صحيح i = 0; i < ntokens; ++i)
    {
      sym_content *sym = symbols[i]->content;
      إذا (sym->کوډ != کوډ_UNDEFINED)
        {
          إذا (sym->کوډ > max_کوډ)
            max_کوډ = sym->کوډ;
          إذا (sym->کوډ == 256)
            کوډ_256_شته_p = false;
        }
    }

  /* If 256 is not used, assign it to error, to follow POSIX.  */
  إذا (کوډ_256_شته_p
      && errtoken->content->کوډ == کوډ_UNDEFINED)
    errtoken->content->کوډ = 256;

  /* Set the missing codes. */
  إذا (max_کوډ < 256)
    max_کوډ = 256;

  لكل (صحيح i = 0; i < ntokens; ++i)
    {
      sym_content *sym = symbols[i]->content;
      إذا (sym->کوډ == کوډ_UNDEFINED)
        {
          IGNORE_TYPE_LIMITS_BEGIN
          إذا (INT_جمع_کړه_WRAPV (max_کوډ, 1, &max_کوډ))
            complain (عدم, fatal, _("token number too large"));
          IGNORE_TYPE_LIMITS_پای_ټکی
          sym->کوډ = max_کوډ;
        }
      إذا (sym->کوډ > max_کوډ)
        max_کوډ = sym->کوډ;
    }

  token_translations = xnmalloc (max_کوډ + 1,
                                 حجم *token_translations);

  /* Initialize all entries for literal tokens to the internal token
     number for $undefined, which represents all invalid inputs.  */
  لكل (صحيح i = 0; i < max_کوډ + 1; ++i)
    token_translations[i] = undeftoken->content->شمېره;
  لكل (صحيح i = 0; symbols_sorted[i]; ++i)
    نښه_translation (symbols_sorted[i]);
}


/* Whether some symbol requires internationalization.  */
ساكن bool
has_translations (فراغ)
{
  لكل (ثابت فراغ *ثبت = hash_get_لومړی (نښه_جدول);
       ثبت;
       ثبت = hash_get_بل (نښه_جدول, ثبت))
    {
      ثابت نښه *sym = (ثابت نښه *) ثبت;
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
    نښه_pack (symbols_sorted[i]);

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
            symbols[writei]->content->شمېره = writei;
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
        = !eoftoken->alias || STRNEQ (eoftoken->alias->نښان, "$end");
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
symgraphlink_new (graphid id, symgraphlink *بل)
{
  symgraphlink *res = xmalloc (حجم *res);
  res->id = id;
  res->بل = بل;
  أرجع res;
}


/*------------------------------------------------------------------.
| Register the second symbol of the precedence relation, and return |
| whether this relation is new.  Use only in register_precedence.   |
`------------------------------------------------------------------*/

ساكن bool
ثبتوونکی_precedence_دوهم_نښه (symgraphlink **لومړی, graphid sym)
{
  إذا (!*لومړی || sym < (*لومړی)->id)
    *لومړی = symgraphlink_new (sym, *لومړی);
  وإلا
    {
      symgraphlink *slist = *لومړی;

      طالما (slist->بل && slist->بل->id <= sym)
        slist = slist->بل;

      إذا (slist->id == sym)
        /* Relation already present. */
        أرجع false;

      slist->بل = symgraphlink_new (sym, slist->بل);
    }
  أرجع true;
}

/*------------------------------------------------------------------.
| Register a new relation between symbols as used. The first symbol |
| has a greater precedence than the second one.                     |
`------------------------------------------------------------------*/

فراغ
ثبتوونکی_precedence (graphid لومړی, graphid snd)
{
  إذا (!prec_nodes)
    init_prec_nodes ();
  ثبتوونکی_precedence_دوهم_نښه (&(prec_nodes[لومړی]->succ), snd);
  ثبتوونکی_precedence_دوهم_نښه (&(prec_nodes[snd]->pred), لومړی);
}


/*---------------------------------------.
| Deep clear a linked / adjacency list). |
`---------------------------------------*/

ساكن فراغ
linkedlist_free (symgraphlink *غوټه)
{
  إذا (غوټه)
    {
      طالما (غوټه->بل)
        {
          symgraphlink *tmp = غوټه->بل;
          حرر (غوټه);
          غوټه = tmp;
        }
      حرر (غوټه);
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
  کارول_شوی_assoc = xcalloc (nsyms, حجم *کارول_شوی_assoc);
  لكل (graphid i = 0; i < nsyms; ++i)
    کارول_شوی_assoc[i] = false;
}

/*------------------------------------------------------------------.
| Test if the associativity for the symbols is defined and useless. |
`------------------------------------------------------------------*/

ساكن مضمن bool
is_assoc_useless (نښه *s)
{
  أرجع s
      && s->content->assoc != undef_assoc
      && s->content->assoc != precedence_assoc
      && !کارول_شوی_assoc[s->content->شمېره];
}

/*-------------------------------.
| Register a used associativity. |
`-------------------------------*/

فراغ
ثبتوونکی_assoc (graphid i, graphid j)
{
  إذا (!کارول_شوی_assoc)
    init_assoc ();
  کارول_شوی_assoc[i] = true;
  کارول_شوی_assoc[j] = true;
}

/*--------------------------------------------------.
| Print a warning for unused precedence relations.  |
`--------------------------------------------------*/

فراغ
print_precedence_warnings (فراغ)
{
  إذا (!prec_nodes)
    init_prec_nodes ();
  إذا (!کارول_شوی_assoc)
    init_assoc ();
  لكل (صحيح i = 0; i < nsyms; ++i)
    {
      نښه *s = symbols[i];
      إذا (s
          && s->content->prec != 0
          && !prec_nodes[i]->pred
          && !prec_nodes[i]->succ)
        {
          إذا (is_assoc_useless (s))
            complain (&s->content->prec_loc, Wprecedence,
                      _("useless precedence and associativity for %s"), s->نښان);
          وإلا إذا (s->content->assoc == precedence_assoc)
            complain (&s->content->prec_loc, Wprecedence,
                      _("useless precedence for %s"), s->نښان);
        }
      وإلا إذا (is_assoc_useless (s))
        complain (&s->content->prec_loc, Wprecedence,
                  _("useless associativity for %s, use %%precedence"), s->نښان);
    }
  حرر (کارول_شوی_assoc);
  assoc_free ();
}
