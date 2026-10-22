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
#وصف_ڪريو ڪوڊ_UNDEFINED (-1)

/* Undefined symbol number.  */
#وصف_ڪريو عدد_UNDEFINED (-1)


ساكن بنية hash_جدول *نشان_جدول = عدم;
ساكن بنية hash_جدول *semantic_type_جدول = عدم;

/*----------------------------------------------------------------.
| Symbols sorted by tag.  Allocated by table_sort, after which no |
| more symbols should be created.                                 |
`----------------------------------------------------------------*/

ساكن نشان **symbols_sorted = عدم;
ساكن semantic_type **semantic_types_sorted = عدم;


/*------------------------.
| Distinguished symbols.  |
`------------------------*/

نشان *errtoken = عدم;
نشان *undeftoken = عدم;
نشان *eoftoken = عدم;
نشان *acceptsymbol = عدم;

/* Precedence relation graph. */
ساكن symgraph **prec_nodes;

/* Store which associativity is used.  */
ساكن bool *استعمال_ٿيل_assoc = عدم;

bool ٽيگ_ڏٺل = false;


/* Whether SYM was defined by the user.  */

ساكن bool
نشان_is_user_defined (نشان *sym)
{
  ثابت bool eof_is_user_defined
    = !eoftoken->alias || STRNEQ (eoftoken->alias->ٽيگ, "$end");
  أرجع sym->ٽيگ[0] != '$'
    && (eof_is_user_defined || (sym != eoftoken && sym->alias != errtoken))
    && sym != errtoken && sym->alias != errtoken
    && sym != undeftoken && sym->alias != undeftoken;
}


/*--------------------------.
| Create a new sym_content. |
`--------------------------*/

ساكن sym_content *
sym_content_new (نشان *s)
{
  sym_content *res = xmalloc (حجم *res);

  res->نشان = s;

  res->type_name = عدم;
  res->type_loc = خالي_جايون_loc;
  لكل (صحيح i = 0; i < ڪوڊ_PROPS_سائيز; ++i)
    ڪوڊ_props_none_init (&res->props[i]);

  res->عدد = عدد_UNDEFINED;
  res->prec_loc = خالي_جايون_loc;
  res->prec = 0;
  res->assoc = undef_assoc;
  res->ڪوڊ = ڪوڊ_UNDEFINED;

  res->class = unknown_sym;
  res->حالت = undeclared;

  أرجع res;
}

/*---------------------------------.
| Create a new symbol, named TAG.  |
`---------------------------------*/

ساكن نشان *
نشان_new (uniqstr ٽيگ, location loc)
{
  نشان *res = xmalloc (حجم *res);
  uniqstr_assert (ٽيگ);

  /* If the tag is not a string (starts with a double quote), check
     that it is valid for Yacc. */
  إذا (ٽيگ[0] != '\"' && ٽيگ[0] != '\'' && strchr (ٽيگ, '-'))
    complain (&loc, Wyacc,
              _("POSIX Yacc forbids dashes in symbol names: %s"), ٽيگ);

  res->ٽيگ = ٽيگ;
  res->location = loc;
  res->translatable = false;
  res->location_جو_lhs = false;
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
نشان_free (فراغ *ptr)
{
  نشان *sym = (نشان *)ptr;
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
symbols_ترتيب_ڏيو (ثابت نشان **پهريون, ثابت نشان **ٻيو)
{
  إذا (0 < location_cmp ((*پهريون)->location, (*ٻيو)->location))
    {
      ثابت نشان* tmp = *پهريون;
      *پهريون = *ٻيو;
      *ٻيو = tmp;
    }
}

/* Likewise, for locations.  */

ساكن فراغ
locations_ترتيب_ڏيو (location *پهريون, location *ٻيو)
{
  إذا (0 < location_cmp (*پهريون, *ٻيو))
    {
      location tmp = *پهريون;
      *پهريون = *ٻيو;
      *ٻيو = tmp;
    }
}

حرف ثابت *
ڪوڊ_props_type_string (ڪوڊ_props_type قسم)
{
  بدل (قسم)
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
semantic_type_new (uniqstr ٽيگ, ثابت location *loc)
{
  semantic_type *res = xmalloc (حجم *res);

  uniqstr_assert (ٽيگ);
  res->ٽيگ = ٽيگ;
  res->location = loc ? *loc : خالي_جايون_loc;
  res->حالت = undeclared;
  لكل (صحيح i = 0; i < ڪوڊ_PROPS_سائيز; ++i)
    ڪوڊ_props_none_init (&res->props[i]);

  أرجع res;
}


/*-----------------.
| Print a symbol.  |
`-----------------*/

#وصف_ڪريو نشان_INT_ATTR_PRINT(Attr)                     \
  إذا (s->content)                                       \
    fprintf (f, " %s = %d", #Attr, s->content->Attr)

#وصف_ڪريو نشان_STR_ATTR_PRINT(Attr)                     \
  إذا (s->content && s->content->Attr)                   \
    fprintf (f, " %s { %s }", #Attr, s->content->Attr)

#وصف_ڪريو نشان_ڪوڊ_PRINT(Attr)                                         \
  إذا (s->content && s->content->props[Attr].ڪوڊ)                       \
    fprintf (f, " %s { %s }", #Attr, s->content->props[Attr].ڪوڊ)

فراغ
نشان_print (نشان ثابت *s, FILE *f)
{
  إذا (s)
    {
      نشان_class c = s->content->class;
      fprintf (f, "%s: %s",
               c == unknown_sym    ? "unknown"
               : c == pct_type_sym ? "%type"
               : c == token_sym    ? "token"
               : c == nterm_sym    ? "nterm"
               : عدم, /* abort.  */
               s->ٽيگ);
      putc (' ', f);
      location_print (s->location, f);
      نشان_INT_ATTR_PRINT (ڪوڊ);
      نشان_INT_ATTR_PRINT (عدد);
      نشان_STR_ATTR_PRINT (type_name);
      نشان_ڪوڊ_PRINT (destructor);
      نشان_ڪوڊ_PRINT (printer);
    }
  وإلا
    fputs ("<NULL>", f);
}

#وصف_رد نشان_ATTR_PRINT
#وصف_رد نشان_ڪوڊ_PRINT


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
نشان_id_get (نشان ثابت *sym)
{
  // There's one weird case: YYerror is the alias, and error is the
  // base symbol.  Return YYerror in that case.
  إذا (sym->alias && is_identifier (sym->alias->ٽيگ))
    أرجع sym->alias->ٽيگ;
  وإلا إذا (is_identifier (sym->ٽيگ))
    أرجع sym->ٽيگ;
  وإلا
    أرجع عدم;
}


/*------------------------------------------------------------------.
| Complain that S's WHAT is redeclared at SECOND, and was first set |
| at FIRST.                                                         |
`------------------------------------------------------------------*/

ساكن فراغ
complain_نشان_redeclared (نشان *s, ثابت حرف *what, location پهريون,
                            location ٻيو)
{
  locations_ترتيب_ڏيو (&پهريون, &ٻيو);
  complain (&ٻيو, complaint, _("%s redeclaration for %s"), what, s->ٽيگ);
  subcomplain (&پهريون, complaint, _("previous declaration"));
}

ساكن فراغ
complain_semantic_type_redeclared (semantic_type *s, ثابت حرف *what, location پهريون,
                                   location ٻيو)
{
  locations_ترتيب_ڏيو (&پهريون, &ٻيو);
  complain (&ٻيو, complaint, _("%s redeclaration for <%s>"), what, s->ٽيگ);
  subcomplain (&پهريون, complaint, _("previous declaration"));
}

ساكن فراغ
complain_class_redeclared (نشان *sym, نشان_class class, location ٻيو)
{
  complain (&ٻيو, complaint,
            class == token_sym
            ? _("symbol %s redeclared as a token")
            : _("symbol %s redeclared as a nonterminal"), sym->ٽيگ);
  إذا (!location_خالي_جايون (sym->location))
    subcomplain (&sym->location, complaint, _("previous definition"));
}

ساكن ثابت نشان *
نشان_from_uniqstr_fuzzy (ثابت uniqstr ڪنجي)
{
  aver (symbols_sorted);
#وصف_ڪريو FSTRCMP_حد 0.6
  مزدوج بهترين_similarity = FSTRCMP_حد;
  ثابت نشان *res = عدم;
  size_t ڳڻپ = hash_get_n_entries (نشان_جدول);
  لكل (صحيح i = 0; i < ڳڻپ; ++i)
    {
      نشان *sym = symbols_sorted[i];
      إذا (STRNEQ (ڪنجي, sym->ٽيگ)
          && (sym->content->حالت == declared
              || sym->content->حالت == undeclared))
        {
          مزدوج similarity = fstrcmp_bounded (ڪنجي, sym->ٽيگ, بهترين_similarity);
          إذا (بهترين_similarity < similarity)
            {
              res = sym;
              بهترين_similarity = similarity;
            }
        }
    }
  أرجع res;
}

ساكن فراغ
complain_نشان_undeclared (ثابت نشان *sym)
{
  أكد (sym->content->حالت != declared);
  ثابت نشان *بهترين = نشان_from_uniqstr_fuzzy (sym->ٽيگ);
  إذا (بهترين)
    {
      complain (&sym->location,
                sym->content->حالت == needed ? complaint : Wother,
                _("symbol %s is used, but is not defined as a token"
                  " and has no rules; did you mean %s?"),
                quote_n (0, sym->ٽيگ),
                quote_n (1, بهترين->ٽيگ));
      إذا (feature_flag & feature_caret)
        location_caret_suggestion (sym->location, بهترين->ٽيگ, stderr);
    }
  وإلا
    complain (&sym->location,
              sym->content->حالت == needed ? complaint : Wother,
              _("symbol %s is used, but is not defined as a token"
                " and has no rules"),
              quote (sym->ٽيگ));
}

فراغ
نشان_location_as_lhs_سيٽ (نشان *sym, location loc)
{
  إذا (!sym->location_جو_lhs)
    {
      sym->location = loc;
      sym->location_جو_lhs = true;
    }
}


/*-----------------------------------------------------------------.
| Set the TYPE_NAME associated with SYM.  Does nothing if passed 0 |
| as TYPE_NAME.                                                    |
`-----------------------------------------------------------------*/

فراغ
نشان_type_سيٽ (نشان *sym, uniqstr type_name, location loc)
{
  إذا (type_name)
    {
      ٽيگ_ڏٺل = true;
      إذا (sym->content->type_name)
        complain_نشان_redeclared (sym, "%type",
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
نشان_ڪوڊ_props_سيٽ (نشان *sym, ڪوڊ_props_type قسم,
                       ڪوڊ_props ثابت *ڪوڊ)
{
  إذا (sym->content->props[قسم].ڪوڊ)
    complain_نشان_redeclared (sym, ڪوڊ_props_type_string (قسم),
                                sym->content->props[قسم].location,
                                ڪوڊ->location);
  وإلا
    sym->content->props[قسم] = *ڪوڊ;
}


/*-----------------------------------------------------.
| Set the DESTRUCTOR or PRINTER associated with TYPE.  |
`-----------------------------------------------------*/

فراغ
semantic_type_ڪوڊ_props_سيٽ (semantic_type *type,
                              ڪوڊ_props_type قسم,
                              ڪوڊ_props ثابت *ڪوڊ)
{
  إذا (type->props[قسم].ڪوڊ)
    complain_semantic_type_redeclared (type, ڪوڊ_props_type_string (قسم),
                                       type->props[قسم].location,
                                       ڪوڊ->location);
  وإلا
    type->props[قسم] = *ڪوڊ;
}

/*---------------------------------------------------.
| Get the computed %destructor or %printer for SYM.  |
`---------------------------------------------------*/

ڪوڊ_props *
نشان_ڪوڊ_props_get (نشان *sym, ڪوڊ_props_type قسم)
{
  /* Per-symbol code props.  */
  إذا (sym->content->props[قسم].ڪوڊ)
    أرجع &sym->content->props[قسم];

  /* Per-type code props.  */
  إذا (sym->content->type_name)
    {
      ڪوڊ_props *ڪوڊ =
        &semantic_type_get (sym->content->type_name, عدم)->props[قسم];
      إذا (ڪوڊ->ڪوڊ)
        أرجع ڪوڊ;
    }

  /* Apply default code props's only to user-defined symbols.  */
  إذا (نشان_is_user_defined (sym))
    {
      ڪوڊ_props *ڪوڊ = &semantic_type_get (sym->content->type_name ? "*" : "",
                                             عدم)->props[قسم];
      إذا (ڪوڊ->ڪوڊ)
        أرجع ڪوڊ;
    }
  أرجع &ڪوڊ_props_none;
}

/*-----------------------------------------------------------------.
| Set the PRECEDENCE associated with SYM.  Does nothing if invoked |
| with UNDEF_ASSOC as ASSOC.                                       |
`-----------------------------------------------------------------*/

فراغ
نشان_precedence_سيٽ (نشان *sym, صحيح prec, assoc a, location loc)
{
  إذا (a != undef_assoc)
    {
      sym_content *s = sym->content;
      إذا (s->prec)
        complain_نشان_redeclared (sym, assoc_منزل_string (a),
                                    s->prec_loc, loc);
      وإلا
        {
          s->prec = prec;
          s->assoc = a;
          s->prec_loc = loc;
        }
    }

  /* Only terminals have a precedence. */
  نشان_class_سيٽ (sym, token_sym, loc, false);
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
نشان_class_سيٽ (نشان *sym, نشان_class class, location loc, bool declaring)
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
                        _("symbol %s redeclared"), sym->ٽيگ);
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
نشان_ڪوڊ_سيٽ (نشان *sym, صحيح ڪوڊ, location loc)
{
  صحيح *codep = &sym->content->ڪوڊ;
  إذا (sym->content->class != token_sym)
    complain (&loc, complaint,
              _("nonterminals cannot be given a token code"));
  وإلا إذا (*codep != ڪوڊ_UNDEFINED
           && *codep != ڪوڊ)
    complain (&loc, complaint, _("redefining code of token %s"),
              sym->ٽيگ);
  وإلا إذا (ڪوڊ == وڌ_کان_وڌ_عدد)
    complain (&loc, complaint, _("code of token %s too large"),
              sym->ٽيگ);
  وإلا
    {
      *codep = ڪوڊ;
      /* User defined $end token? */
      إذا (ڪوڊ == 0 && !eoftoken)
        {
          eoftoken = sym->content->نشان;
          eoftoken->content->عدد = 0;
        }
    }
}


/*----------------------------------------------------------.
| If SYM is not defined, report an error, and consider it a |
| nonterminal.                                              |
`----------------------------------------------------------*/

ساكن فراغ
نشان_check_defined (نشان *sym)
{
  sym_content *s = sym->content;
  إذا (s->class == unknown_sym || s->class == pct_type_sym)
    {
      complain_نشان_undeclared (sym);
      s->class = nterm_sym;
    }

  إذا (s->عدد == عدد_UNDEFINED)
    s->عدد = s->class == token_sym ? ntokens++ : nnterms++;

  إذا (s->class == token_sym
      && sym->ٽيگ[0] == '"'
      && !sym->is_alias)
    complain (&sym->location, Wdangling_alias,
              _("string literal %s not attached to a symbol"),
              sym->ٽيگ);

  لكل (صحيح i = 0; i < 2; ++i)
    نشان_ڪوڊ_props_get (sym, i)->is_استعمال_ٿيل = true;

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
      || !*sem_type->ٽيگ
      || STREQ (sem_type->ٽيگ, "*"))
    {
      لكل (صحيح i = 0; i < 2; ++i)
        إذا (sem_type->props[i].قسم != ڪوڊ_PROPS_NONE
            && ! sem_type->props[i].is_استعمال_ٿيل)
          complain (&sem_type->location, Wother,
                    _("useless %s for type <%s>"),
                    ڪوڊ_props_type_string (i), sem_type->ٽيگ);
    }
  وإلا
    complain (&sem_type->location, Wother,
              _("type <%s> is used, but is not associated to any symbol"),
              sem_type->ٽيگ);
}

/*-------------------------------------------------------------------.
| Merge the properties (precedence, associativity, etc.) of SYM, and |
| its string-named alias STR; check consistency.                     |
`-------------------------------------------------------------------*/

ساكن فراغ
نشان_ملايو_properties (نشان *sym, نشان *str)
{
  إذا (str->content->type_name != sym->content->type_name)
    {
      إذا (str->content->type_name)
        نشان_type_سيٽ (sym,
                         str->content->type_name, str->content->type_loc);
      وإلا
        نشان_type_سيٽ (str,
                         sym->content->type_name, sym->content->type_loc);
    }


  لكل (صحيح i = 0; i < ڪوڊ_PROPS_سائيز; ++i)
    إذا (str->content->props[i].ڪوڊ)
      نشان_ڪوڊ_props_سيٽ (sym, i, &str->content->props[i]);
    وإلا إذا (sym->content->props[i].ڪوڊ)
      نشان_ڪوڊ_props_سيٽ (str, i, &sym->content->props[i]);

  إذا (sym->content->prec || str->content->prec)
    {
      إذا (str->content->prec)
        نشان_precedence_سيٽ (sym, str->content->prec, str->content->assoc,
                               str->content->prec_loc);
      وإلا
        نشان_precedence_سيٽ (str, sym->content->prec, sym->content->assoc,
                               sym->content->prec_loc);
    }
}

فراغ
نشان_make_alias (نشان *sym, نشان *str, location loc)
{
  إذا (sym->content->class != token_sym)
    complain (&loc, complaint,
              _("nonterminals cannot be given a string alias"));
  وإلا إذا (str->alias)
    complain (&loc, Wother,
              _("symbol %s used more than once as a literal string"), str->ٽيگ);
  وإلا إذا (sym->alias)
    complain (&loc, Wother,
              _("symbol %s given more than one literal string"), sym->ٽيگ);
  وإلا
    {
      نشان_ملايو_properties (sym, str);
      sym_content_free (str->content);
      str->content = sym->content;
      str->content->نشان = str;
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
نشان_pack (نشان *sym)
{
  aver (sym->content->عدد != عدد_UNDEFINED);
  إذا (sym->content->class == nterm_sym)
    sym->content->عدد += ntokens;

  symbols[sym->content->عدد] = sym->content->نشان;
}

ساكن فراغ
complain_ڪوڊ_redeclared (صحيح انگ, ثابت نشان *پهريون, ثابت نشان *ٻيو)
{
  symbols_ترتيب_ڏيو (&پهريون, &ٻيو);
  complain (&ٻيو->location, complaint,
            _("code %d reassigned to token %s"),
            انگ, ٻيو->ٽيگ);
  subcomplain (&پهريون->location, complaint,
               _("previous declaration for %s"),
               پهريون->ٽيگ);
}

/*-------------------------------------------------.
| Put SYM in TOKEN_TRANSLATIONS if it is a token.  |
`-------------------------------------------------*/

ساكن فراغ
نشان_translation (ثابت نشان *sym)
{
  إذا (sym->content->class == token_sym && !sym->is_alias)
    {
      /* A token whose translation has already been set? */
      إذا (token_translations[sym->content->ڪوڊ]
          != undeftoken->content->عدد)
        complain_ڪوڊ_redeclared
          (sym->content->ڪوڊ,
           symbols[token_translations[sym->content->ڪوڊ]], sym);
      وإلا
        token_translations[sym->content->ڪوڊ]
          = sym->content->عدد;
    }
}


/*---------------------------------------.
| Symbol and semantic type hash tables.  |
`---------------------------------------*/

/* Initial capacity of symbol and semantic type hash table.  */
#وصف_ڪريو HT_INITIAL_صلاحيت 257

ساكن مضمن bool
hash_compare_نشان (ثابت نشان *m1, ثابت نشان *m2)
{
  /* Since tags are unique, we can compare the pointers themselves.  */
  أرجع UNIQSTR_EQ (m1->ٽيگ, m2->ٽيگ);
}

ساكن مضمن bool
hash_compare_semantic_type (ثابت semantic_type *m1, ثابت semantic_type *m2)
{
  /* Since names are unique, we can compare the pointers themselves.  */
  أرجع UNIQSTR_EQ (m1->ٽيگ, m2->ٽيگ);
}

ساكن bool
hash_نشان_comparator (فراغ ثابت *m1, فراغ ثابت *m2)
{
  أرجع hash_compare_نشان (m1, m2);
}

ساكن bool
hash_semantic_type_comparator (فراغ ثابت *m1, فراغ ثابت *m2)
{
  أرجع hash_compare_semantic_type (m1, m2);
}

ساكن مضمن size_t
hash_نشان (ثابت نشان *m, size_t tablesize)
{
  /* Since tags are unique, we can hash the pointer itself.  */
  أرجع ((uintptr_t) m->ٽيگ) % tablesize;
}

ساكن مضمن size_t
hash_semantic_type (ثابت semantic_type *m, size_t tablesize)
{
  /* Since names are unique, we can hash the pointer itself.  */
  أرجع ((uintptr_t) m->ٽيگ) % tablesize;
}

ساكن size_t
hash_نشان_hasher (فراغ ثابت *m, size_t tablesize)
{
  أرجع hash_نشان (m, tablesize);
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
  نشان_جدول = hash_xinitialize (HT_INITIAL_صلاحيت,
                                   عدم,
                                   hash_نشان_hasher,
                                   hash_نشان_comparator,
                                   نشان_free);

  /* Construct the acceptsymbol symbol. */
  acceptsymbol = نشان_get ("$accept", خالي_جايون_loc);
  acceptsymbol->content->class = nterm_sym;
  acceptsymbol->content->عدد = nnterms++;

  /* Construct the YYerror/"error" token */
  errtoken = نشان_get ("YYerror", خالي_جايون_loc);
  errtoken->content->class = token_sym;
  errtoken->content->عدد = ntokens++;
  {
    نشان *alias = نشان_get ("error", خالي_جايون_loc);
    نشان_class_سيٽ (alias, token_sym, خالي_جايون_loc, false);
    نشان_make_alias (errtoken, alias, خالي_جايون_loc);
  }

  /* Construct the YYUNDEF/"$undefined" token that represents all
     undefined literal tokens.  It is always symbol number 2.  */
  undeftoken = نشان_get ("YYUNDEF", خالي_جايون_loc);
  undeftoken->content->class = token_sym;
  undeftoken->content->عدد = ntokens++;
  {
    نشان *alias = نشان_get ("$undefined", خالي_جايون_loc);
    نشان_class_سيٽ (alias, token_sym, خالي_جايون_loc, false);
    نشان_make_alias (undeftoken, alias, خالي_جايون_loc);
  }

  semantic_type_جدول = hash_xinitialize (HT_INITIAL_صلاحيت,
                                          عدم,
                                          hash_semantic_type_hasher,
                                          hash_semantic_type_comparator,
                                          حرر);
}


/*----------------------------------------------------------------.
| Find the symbol named KEY, and return it.  If it does not exist |
| yet, create it.                                                 |
`----------------------------------------------------------------*/

نشان *
نشان_from_uniqstr (ثابت uniqstr ڪنجي, location loc)
{
  نشان probe;

  probe.ٽيگ = ڪنجي;
  نشان *res = hash_lookup (نشان_جدول, &probe);

  إذا (!res)
    {
      /* First insertion in the hash. */
      aver (!symbols_sorted);
      res = نشان_new (ڪنجي, loc);
      hash_xinsert (نشان_جدول, res);
    }
  أرجع res;
}


/*-----------------------------------------------------------------------.
| Find the semantic type named KEY, and return it.  If it does not exist |
| yet, create it.                                                        |
`-----------------------------------------------------------------------*/

semantic_type *
semantic_type_from_uniqstr (ثابت uniqstr ڪنجي, ثابت location *loc)
{
  semantic_type probe;

  probe.ٽيگ = ڪنجي;
  semantic_type *res = hash_lookup (semantic_type_جدول, &probe);

  إذا (!res)
    {
      /* First insertion in the hash. */
      res = semantic_type_new (ڪنجي, loc);
      hash_xinsert (semantic_type_جدول, res);
    }
  أرجع res;
}


/*----------------------------------------------------------------.
| Find the symbol named KEY, and return it.  If it does not exist |
| yet, create it.                                                 |
`----------------------------------------------------------------*/

نشان *
نشان_get (ثابت حرف *ڪنجي, location loc)
{
  أرجع نشان_from_uniqstr (uniqstr_new (ڪنجي), loc);
}


/*-----------------------------------------------------------------------.
| Find the semantic type named KEY, and return it.  If it does not exist |
| yet, create it.                                                        |
`-----------------------------------------------------------------------*/

semantic_type *
semantic_type_get (ثابت حرف *ڪنجي, ثابت location *loc)
{
  أرجع semantic_type_from_uniqstr (uniqstr_new (ڪنجي), loc);
}


/*------------------------------------------------------------------.
| Generate a dummy nonterminal, whose name cannot conflict with the |
| user's names.                                                     |
`------------------------------------------------------------------*/

نشان *
dummy_نشان_get (location loc)
{
  /* Incremented for each generated symbol.  */
  ساكن صحيح dummy_ڳڻپ = 0;
  حرف buf[32];
  صحيح len = snprintf (buf, حجم buf, "$@%d", ++dummy_ڳڻپ);
  assure (len < حجم buf);
  نشان *sym = نشان_get (buf, loc);
  sym->content->class = nterm_sym;
  أرجع sym;
}

bool
نشان_is_dummy (نشان ثابت *sym)
{
  أرجع sym->ٽيگ[0] == '@' || (sym->ٽيگ[0] == '$' && sym->ٽيگ[1] == '@');
}

/*-------------------.
| Free the symbols.  |
`-------------------*/

فراغ
symbols_free (فراغ)
{
  hash_free (نشان_جدول);
  hash_free (semantic_type_جدول);
  حرر (symbols);
  حرر (symbols_sorted);
  حرر (semantic_types_sorted);
}


ساكن صحيح
نشان_cmp (فراغ ثابت *a, فراغ ثابت *b)
{
  أرجع location_cmp ((*(نشان * ثابت *)a)->location,
                       (*(نشان * ثابت *)b)->location);
}

/* Store in *SORTED an array of pointers to the symbols contained in
   TABLE, sorted by order of appearance (i.e., by location). */

ساكن فراغ
جدول_ترتيب_ڏيو (بنية hash_جدول *جدول, نشان ***sorted)
{
  aver (!*sorted);
  size_t ڳڻپ = hash_get_n_entries (جدول);
  *sorted = xnmalloc (ڳڻپ + 1, حجم **sorted);
  hash_get_entries (جدول, (فراغ**)*sorted, ڳڻپ);
  qsort (*sorted, ڳڻپ, حجم **sorted, نشان_cmp);
  (*sorted)[ڳڻپ] = عدم;
}

/*--------------------------------------------------------------.
| Check that all the symbols are defined.  Report any undefined |
| symbols and consider them nonterminals.                       |
`--------------------------------------------------------------*/

فراغ
symbols_check_defined (فراغ)
{
  جدول_ترتيب_ڏيو (نشان_جدول, &symbols_sorted);
  /* semantic_type, like symbol, starts with a 'tag' field and then a
     'location' field.  And here we only deal with arrays/hashes of
     pointers, sizeof is not an issue.

     So instead of implementing table_sort (and symbol_cmp) once for
     each type, let's lie a bit to the typing system, and treat
     'semantic_type' as if it were 'symbol'. */
  جدول_ترتيب_ڏيو (semantic_type_جدول, (نشان ***) &semantic_types_sorted);

  لكل (صحيح i = 0; symbols_sorted[i]; ++i)
    نشان_check_defined (symbols_sorted[i]);
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
  bool ڪوڊ_256_موجود_p = true;

  /* Find the highest token code, and whether 256, the POSIX preferred
     token code for the error token, is used.  */
  max_ڪوڊ = 0;
  لكل (صحيح i = 0; i < ntokens; ++i)
    {
      sym_content *sym = symbols[i]->content;
      إذا (sym->ڪوڊ != ڪوڊ_UNDEFINED)
        {
          إذا (sym->ڪوڊ > max_ڪوڊ)
            max_ڪوڊ = sym->ڪوڊ;
          إذا (sym->ڪوڊ == 256)
            ڪوڊ_256_موجود_p = false;
        }
    }

  /* If 256 is not used, assign it to error, to follow POSIX.  */
  إذا (ڪوڊ_256_موجود_p
      && errtoken->content->ڪوڊ == ڪوڊ_UNDEFINED)
    errtoken->content->ڪوڊ = 256;

  /* Set the missing codes. */
  إذا (max_ڪوڊ < 256)
    max_ڪوڊ = 256;

  لكل (صحيح i = 0; i < ntokens; ++i)
    {
      sym_content *sym = symbols[i]->content;
      إذا (sym->ڪوڊ == ڪوڊ_UNDEFINED)
        {
          IGNORE_TYPE_LIMITS_BEGIN
          إذا (INT_جوڙيو_WRAPV (max_ڪوڊ, 1, &max_ڪوڊ))
            complain (عدم, fatal, _("token number too large"));
          IGNORE_TYPE_LIMITS_پڇاڙي
          sym->ڪوڊ = max_ڪوڊ;
        }
      إذا (sym->ڪوڊ > max_ڪوڊ)
        max_ڪوڊ = sym->ڪوڊ;
    }

  token_translations = xnmalloc (max_ڪوڊ + 1,
                                 حجم *token_translations);

  /* Initialize all entries for literal tokens to the internal token
     number for $undefined, which represents all invalid inputs.  */
  لكل (صحيح i = 0; i < max_ڪوڊ + 1; ++i)
    token_translations[i] = undeftoken->content->عدد;
  لكل (صحيح i = 0; symbols_sorted[i]; ++i)
    نشان_translation (symbols_sorted[i]);
}


/* Whether some symbol requires internationalization.  */
ساكن bool
has_translations (فراغ)
{
  لكل (ثابت فراغ *داخلا_2 = hash_get_پهريون (نشان_جدول);
       داخلا_2;
       داخلا_2 = hash_get_ايندڙ (نشان_جدول, داخلا_2))
    {
      ثابت نشان *sym = (ثابت نشان *) داخلا_2;
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
    نشان_pack (symbols_sorted[i]);

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
            symbols[writei]->content->عدد = writei;
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
        = !eoftoken->alias || STRNEQ (eoftoken->alias->ٽيگ, "$end");
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
symgraphlink_new (graphid id, symgraphlink *ايندڙ)
{
  symgraphlink *res = xmalloc (حجم *res);
  res->id = id;
  res->ايندڙ = ايندڙ;
  أرجع res;
}


/*------------------------------------------------------------------.
| Register the second symbol of the precedence relation, and return |
| whether this relation is new.  Use only in register_precedence.   |
`------------------------------------------------------------------*/

ساكن bool
رجسٽر_precedence_ٻيو_نشان (symgraphlink **پهريون, graphid sym)
{
  إذا (!*پهريون || sym < (*پهريون)->id)
    *پهريون = symgraphlink_new (sym, *پهريون);
  وإلا
    {
      symgraphlink *slist = *پهريون;

      طالما (slist->ايندڙ && slist->ايندڙ->id <= sym)
        slist = slist->ايندڙ;

      إذا (slist->id == sym)
        /* Relation already present. */
        أرجع false;

      slist->ايندڙ = symgraphlink_new (sym, slist->ايندڙ);
    }
  أرجع true;
}

/*------------------------------------------------------------------.
| Register a new relation between symbols as used. The first symbol |
| has a greater precedence than the second one.                     |
`------------------------------------------------------------------*/

فراغ
رجسٽر_precedence (graphid پهريون, graphid snd)
{
  إذا (!prec_nodes)
    init_prec_nodes ();
  رجسٽر_precedence_ٻيو_نشان (&(prec_nodes[پهريون]->succ), snd);
  رجسٽر_precedence_ٻيو_نشان (&(prec_nodes[snd]->pred), پهريون);
}


/*---------------------------------------.
| Deep clear a linked / adjacency list). |
`---------------------------------------*/

ساكن فراغ
linkedlist_free (symgraphlink *ڳنڍ)
{
  إذا (ڳنڍ)
    {
      طالما (ڳنڍ->ايندڙ)
        {
          symgraphlink *tmp = ڳنڍ->ايندڙ;
          حرر (ڳنڍ);
          ڳنڍ = tmp;
        }
      حرر (ڳنڍ);
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
  استعمال_ٿيل_assoc = xcalloc (nsyms, حجم *استعمال_ٿيل_assoc);
  لكل (graphid i = 0; i < nsyms; ++i)
    استعمال_ٿيل_assoc[i] = false;
}

/*------------------------------------------------------------------.
| Test if the associativity for the symbols is defined and useless. |
`------------------------------------------------------------------*/

ساكن مضمن bool
is_assoc_useless (نشان *s)
{
  أرجع s
      && s->content->assoc != undef_assoc
      && s->content->assoc != precedence_assoc
      && !استعمال_ٿيل_assoc[s->content->عدد];
}

/*-------------------------------.
| Register a used associativity. |
`-------------------------------*/

فراغ
رجسٽر_assoc (graphid i, graphid j)
{
  إذا (!استعمال_ٿيل_assoc)
    init_assoc ();
  استعمال_ٿيل_assoc[i] = true;
  استعمال_ٿيل_assoc[j] = true;
}

/*--------------------------------------------------.
| Print a warning for unused precedence relations.  |
`--------------------------------------------------*/

فراغ
print_precedence_warnings (فراغ)
{
  إذا (!prec_nodes)
    init_prec_nodes ();
  إذا (!استعمال_ٿيل_assoc)
    init_assoc ();
  لكل (صحيح i = 0; i < nsyms; ++i)
    {
      نشان *s = symbols[i];
      إذا (s
          && s->content->prec != 0
          && !prec_nodes[i]->pred
          && !prec_nodes[i]->succ)
        {
          إذا (is_assoc_useless (s))
            complain (&s->content->prec_loc, Wprecedence,
                      _("useless precedence and associativity for %s"), s->ٽيگ);
          وإلا إذا (s->content->assoc == precedence_assoc)
            complain (&s->content->prec_loc, Wprecedence,
                      _("useless precedence for %s"), s->ٽيگ);
        }
      وإلا إذا (is_assoc_useless (s))
        complain (&s->content->prec_loc, Wprecedence,
                  _("useless associativity for %s, use %%precedence"), s->ٽيگ);
    }
  حرر (استعمال_ٿيل_assoc);
  assoc_free ();
}
