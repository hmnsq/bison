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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ "symtab.h"

#ଅନ୍ତର୍ଭାଵ "system.h"

#ଅନ୍ତର୍ଭାଵ <assure.h>
#ଅନ୍ତର୍ଭାଵ <fstrcmp.h>
#ଅନ୍ତର୍ଭାଵ <hash.h>
#ଅନ୍ତର୍ଭାଵ <quote.h>

#ଅନ୍ତର୍ଭାଵ "complain.h"
#ଅନ୍ତର୍ଭାଵ "getargs.h"
#ଅନ୍ତର୍ଭାଵ "gram.h"
#ଅନ୍ତର୍ଭାଵ "intprops.h"

/** Undefined token code.  */
#define CODE_UNDEFINED (-1)

/* Undefined symbol number.  */
#define ସଂଖ୍ୟା_UNDEFINED (-1)


ସ୍ଥିର ସଂରଚନା hash_table *ପ୍ରତୀକ_table = ଖାଲି;
ସ୍ଥିର ସଂରଚନା hash_table *semantic_type_table = ଖାଲି;

/*----------------------------------------------------------------.
| Symbols sorted by tag.  Allocated by table_sort, after which no |
| more symbols should be created.                                 |
`----------------------------------------------------------------*/

ସ୍ଥିର ପ୍ରତୀକ **symbols_sorted = ଖାଲି;
ସ୍ଥିର semantic_type **semantic_types_sorted = ଖାଲି;


/*------------------------.
| Distinguished symbols.  |
`------------------------*/

ପ୍ରତୀକ *errtoken = ଖାଲି;
ପ୍ରତୀକ *undeftoken = ଖାଲି;
ପ୍ରତୀକ *eoftoken = ଖାଲି;
ପ୍ରତୀକ *acceptsymbol = ଖାଲି;

/* Precedence relation graph. */
ସ୍ଥିର symgraph **prec_nodes;

/* Store which associativity is used.  */
ସ୍ଥିର bool *used_assoc = ଖାଲି;

bool tag_seen = false;


/* Whether SYM was defined by the user.  */

ସ୍ଥିର bool
ପ୍ରତୀକ_is_user_defined (ପ୍ରତୀକ *sym)
{
  ଅଚଳ bool eof_is_user_defined
    = !eoftoken->alias || STRNEQ (eoftoken->alias->tag, "$end");
  ଫେରନ୍ତୁ sym->tag[0] != '$'
    && (eof_is_user_defined || (sym != eoftoken && sym->alias != errtoken))
    && sym != errtoken && sym->alias != errtoken
    && sym != undeftoken && sym->alias != undeftoken;
}


/*--------------------------.
| Create a new sym_content. |
`--------------------------*/

ସ୍ଥିର sym_content *
sym_content_new (ପ୍ରତୀକ *s)
{
  sym_content *res = xmalloc (ଆକାର *res);

  res->ପ୍ରତୀକ = s;

  res->type_name = ଖାଲି;
  res->type_loc = empty_loc;
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < CODE_PROPS_SIZE; ++i)
    code_props_none_init (&res->props[i]);

  res->ସଂଖ୍ୟା = ସଂଖ୍ୟା_UNDEFINED;
  res->prec_loc = empty_loc;
  res->prec = 0;
  res->assoc = undef_assoc;
  res->code = CODE_UNDEFINED;

  res->class = unknown_sym;
  res->status = undeclared;

  ଫେରନ୍ତୁ res;
}

/*---------------------------------.
| Create a new symbol, named TAG.  |
`---------------------------------*/

ସ୍ଥିର ପ୍ରତୀକ *
ପ୍ରତୀକ_new (uniqstr tag, location loc)
{
  ପ୍ରତୀକ *res = xmalloc (ଆକାର *res);
  uniqstr_assert (tag);

  /* If the tag is not a string (starts with a double quote), check
     that it is valid for Yacc. */
  ଯଦି (tag[0] != '\"' && tag[0] != '\'' && strchr (tag, '-'))
    complain (&loc, Wyacc,
              _("POSIX Yacc forbids dashes in symbol names: %s"), tag);

  res->tag = tag;
  res->location = loc;
  res->translatable = false;
  res->location_of_lhs = false;
  res->alias = ଖାଲି;
  res->content = sym_content_new (res);
  res->is_alias = false;
  ଫେରନ୍ତୁ res;
}

/*--------------------.
| Free a sym_content. |
`--------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
sym_content_free (sym_content *sym)
{
  ମୁକ୍ତ (sym);
}


/*---------------------------------------------------------.
| Free a symbol and its associated content if appropriate. |
`---------------------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
ପ୍ରତୀକ_free (ଶୂନ୍ୟ *ptr)
{
  ପ୍ରତୀକ *sym = (ପ୍ରତୀକ *)ptr;
  ଯଦି (!sym->is_alias)
    sym_content_free (sym->content);
  ମୁକ୍ତ (sym);

}

/* If needed, swap first and second so that first has the earliest
   location (according to location_cmp).

   Many symbol features (e.g., token codes) are not assigned during
   parsing, but in a second step, via a traversal of the symbol table
   sorted on tag.

   However, error messages make more sense if we keep the first
   declaration first.
*/

ସ୍ଥିର ଶୂନ୍ୟ
symbols_sort (ଅଚଳ ପ୍ରତୀକ **first, ଅଚଳ ପ୍ରତୀକ **second)
{
  ଯଦି (0 < location_cmp ((*first)->location, (*second)->location))
    {
      ଅଚଳ ପ୍ରତୀକ* tmp = *first;
      *first = *second;
      *second = tmp;
    }
}

/* Likewise, for locations.  */

ସ୍ଥିର ଶୂନ୍ୟ
locations_sort (location *first, location *second)
{
  ଯଦି (0 < location_cmp (*first, *second))
    {
      location tmp = *first;
      *first = *second;
      *second = tmp;
    }
}

ଅକ୍ଷର ଅଚଳ *
code_props_type_string (code_props_type kind)
{
  ବାଛ (kind)
    {
    ସ୍ଥିତି destructor:
      ଫେରନ୍ତୁ "%destructor";
    ସ୍ଥିତି printer:
      ଫେରନ୍ତୁ "%printer";
    }
  abort ();
}


/*----------------------------------------.
| Create a new semantic type, named TAG.  |
`----------------------------------------*/

ସ୍ଥିର semantic_type *
semantic_type_new (uniqstr tag, ଅଚଳ location *loc)
{
  semantic_type *res = xmalloc (ଆକାର *res);

  uniqstr_assert (tag);
  res->tag = tag;
  res->location = loc ? *loc : empty_loc;
  res->status = undeclared;
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < CODE_PROPS_SIZE; ++i)
    code_props_none_init (&res->props[i]);

  ଫେରନ୍ତୁ res;
}


/*-----------------.
| Print a symbol.  |
`-----------------*/

#define ପ୍ରତୀକ_INT_ATTR_PRINT(Attr)                     \
  ଯଦି (s->content)                                       \
    fprintf (f, " %s = %d", #Attr, s->content->Attr)

#define ପ୍ରତୀକ_STR_ATTR_PRINT(Attr)                     \
  ଯଦି (s->content && s->content->Attr)                   \
    fprintf (f, " %s { %s }", #Attr, s->content->Attr)

#define ପ୍ରତୀକ_CODE_PRINT(Attr)                                         \
  ଯଦି (s->content && s->content->props[Attr].code)                       \
    fprintf (f, " %s { %s }", #Attr, s->content->props[Attr].code)

ଶୂନ୍ୟ
ପ୍ରତୀକ_print (ପ୍ରତୀକ ଅଚଳ *s, FILE *f)
{
  ଯଦି (s)
    {
      ପ୍ରତୀକ_class c = s->content->class;
      fprintf (f, "%s: %s",
               c == unknown_sym    ? "unknown"
               : c == pct_type_sym ? "%type"
               : c == token_sym    ? "token"
               : c == nterm_sym    ? "nterm"
               : ଖାଲି, /* abort.  */
               s->tag);
      putc (' ', f);
      location_print (s->location, f);
      ପ୍ରତୀକ_INT_ATTR_PRINT (code);
      ପ୍ରତୀକ_INT_ATTR_PRINT (ସଂଖ୍ୟା);
      ପ୍ରତୀକ_STR_ATTR_PRINT (type_name);
      ପ୍ରତୀକ_CODE_PRINT (destructor);
      ପ୍ରତୀକ_CODE_PRINT (printer);
    }
  ଅନ୍ୟଥା
    fputs ("<NULL>", f);
}

#undef ପ୍ରତୀକ_ATTR_PRINT
#undef ପ୍ରତୀକ_CODE_PRINT


/*----------------------------------.
| Whether S is a valid identifier.  |
`----------------------------------*/

ସ୍ଥିର bool
is_identifier (uniqstr s)
{
  ସ୍ଥିର ଅକ୍ଷର ଅଚଳ alphanum[26 + 26 + 1 + 10] =
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "_"
    "0123456789";
  ଯଦି (!s || ! memchr (alphanum, *s, ଆକାର alphanum - 10))
    ଫେରନ୍ତୁ false;
  ପାଇଁ (++s; *s; ++s)
    ଯଦି (! memchr (alphanum, *s, ଆକାର alphanum))
      ଫେରନ୍ତୁ false;
  ଫେରନ୍ତୁ true;
}


/*-----------------------------------------------.
| Get the identifier associated to this symbol.  |
`-----------------------------------------------*/

uniqstr
ପ୍ରତୀକ_id_get (ପ୍ରତୀକ ଅଚଳ *sym)
{
  // There's one weird case: YYerror is the alias, and error is the
  // base symbol.  Return YYerror in that case.
  ଯଦି (sym->alias && is_identifier (sym->alias->tag))
    ଫେରନ୍ତୁ sym->alias->tag;
  ଅନ୍ୟଥା ଯଦି (is_identifier (sym->tag))
    ଫେରନ୍ତୁ sym->tag;
  ଅନ୍ୟଥା
    ଫେରନ୍ତୁ ଖାଲି;
}


/*------------------------------------------------------------------.
| Complain that S's WHAT is redeclared at SECOND, and was first set |
| at FIRST.                                                         |
`------------------------------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
complain_ପ୍ରତୀକ_redeclared (ପ୍ରତୀକ *s, ଅଚଳ ଅକ୍ଷର *what, location first,
                            location second)
{
  locations_sort (&first, &second);
  complain (&second, complaint, _("%s redeclaration for %s"), what, s->tag);
  subcomplain (&first, complaint, _("previous declaration"));
}

ସ୍ଥିର ଶୂନ୍ୟ
complain_semantic_type_redeclared (semantic_type *s, ଅଚଳ ଅକ୍ଷର *what, location first,
                                   location second)
{
  locations_sort (&first, &second);
  complain (&second, complaint, _("%s redeclaration for <%s>"), what, s->tag);
  subcomplain (&first, complaint, _("previous declaration"));
}

ସ୍ଥିର ଶୂନ୍ୟ
complain_class_redeclared (ପ୍ରତୀକ *sym, ପ୍ରତୀକ_class class, location second)
{
  complain (&second, complaint,
            class == token_sym
            ? _("symbol %s redeclared as a token")
            : _("symbol %s redeclared as a nonterminal"), sym->tag);
  ଯଦି (!location_empty (sym->location))
    subcomplain (&sym->location, complaint, _("previous definition"));
}

ସ୍ଥିର ଅଚଳ ପ୍ରତୀକ *
ପ୍ରତୀକ_from_uniqstr_fuzzy (ଅଚଳ uniqstr ଚାବି)
{
  aver (symbols_sorted);
#define FSTRCMP_THRESHOLD 0.6
  ଦ୍ୱିଗୁଣ best_similarity = FSTRCMP_THRESHOLD;
  ଅଚଳ ପ୍ରତୀକ *res = ଖାଲି;
  size_t ଗଣନା_2 = hash_get_n_entries (ପ୍ରତୀକ_table);
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; ++i)
    {
      ପ୍ରତୀକ *sym = symbols_sorted[i];
      ଯଦି (STRNEQ (ଚାବି, sym->tag)
          && (sym->content->status == declared
              || sym->content->status == undeclared))
        {
          ଦ୍ୱିଗୁଣ similarity = fstrcmp_bounded (ଚାବି, sym->tag, best_similarity);
          ଯଦି (best_similarity < similarity)
            {
              res = sym;
              best_similarity = similarity;
            }
        }
    }
  ଫେରନ୍ତୁ res;
}

ସ୍ଥିର ଶୂନ୍ୟ
complain_ପ୍ରତୀକ_undeclared (ଅଚଳ ପ୍ରତୀକ *sym)
{
  ପ୍ରତିଜ୍ଞା (sym->content->status != declared);
  ଅଚଳ ପ୍ରତୀକ *best = ପ୍ରତୀକ_from_uniqstr_fuzzy (sym->tag);
  ଯଦି (best)
    {
      complain (&sym->location,
                sym->content->status == needed ? complaint : Wother,
                _("symbol %s is used, but is not defined as a token"
                  " and has no rules; did you mean %s?"),
                quote_n (0, sym->tag),
                quote_n (1, best->tag));
      ଯଦି (feature_flag & feature_caret)
        location_caret_suggestion (sym->location, best->tag, stderr);
    }
  ଅନ୍ୟଥା
    complain (&sym->location,
              sym->content->status == needed ? complaint : Wother,
              _("symbol %s is used, but is not defined as a token"
                " and has no rules"),
              quote (sym->tag));
}

ଶୂନ୍ୟ
ପ୍ରତୀକ_location_as_lhs_set (ପ୍ରତୀକ *sym, location loc)
{
  ଯଦି (!sym->location_of_lhs)
    {
      sym->location = loc;
      sym->location_of_lhs = true;
    }
}


/*-----------------------------------------------------------------.
| Set the TYPE_NAME associated with SYM.  Does nothing if passed 0 |
| as TYPE_NAME.                                                    |
`-----------------------------------------------------------------*/

ଶୂନ୍ୟ
ପ୍ରତୀକ_type_set (ପ୍ରତୀକ *sym, uniqstr type_name, location loc)
{
  ଯଦି (type_name)
    {
      tag_seen = true;
      ଯଦି (sym->content->type_name)
        complain_ପ୍ରତୀକ_redeclared (sym, "%type",
                                    sym->content->type_loc, loc);
      ଅନ୍ୟଥା
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

ଶୂନ୍ୟ
ପ୍ରତୀକ_code_props_set (ପ୍ରତୀକ *sym, code_props_type kind,
                       code_props ଅଚଳ *code)
{
  ଯଦି (sym->content->props[kind].code)
    complain_ପ୍ରତୀକ_redeclared (sym, code_props_type_string (kind),
                                sym->content->props[kind].location,
                                code->location);
  ଅନ୍ୟଥା
    sym->content->props[kind] = *code;
}


/*-----------------------------------------------------.
| Set the DESTRUCTOR or PRINTER associated with TYPE.  |
`-----------------------------------------------------*/

ଶୂନ୍ୟ
semantic_type_code_props_set (semantic_type *type,
                              code_props_type kind,
                              code_props ଅଚଳ *code)
{
  ଯଦି (type->props[kind].code)
    complain_semantic_type_redeclared (type, code_props_type_string (kind),
                                       type->props[kind].location,
                                       code->location);
  ଅନ୍ୟଥା
    type->props[kind] = *code;
}

/*---------------------------------------------------.
| Get the computed %destructor or %printer for SYM.  |
`---------------------------------------------------*/

code_props *
ପ୍ରତୀକ_code_props_get (ପ୍ରତୀକ *sym, code_props_type kind)
{
  /* Per-symbol code props.  */
  ଯଦି (sym->content->props[kind].code)
    ଫେରନ୍ତୁ &sym->content->props[kind];

  /* Per-type code props.  */
  ଯଦି (sym->content->type_name)
    {
      code_props *code =
        &semantic_type_get (sym->content->type_name, ଖାଲି)->props[kind];
      ଯଦି (code->code)
        ଫେରନ୍ତୁ code;
    }

  /* Apply default code props's only to user-defined symbols.  */
  ଯଦି (ପ୍ରତୀକ_is_user_defined (sym))
    {
      code_props *code = &semantic_type_get (sym->content->type_name ? "*" : "",
                                             ଖାଲି)->props[kind];
      ଯଦି (code->code)
        ଫେରନ୍ତୁ code;
    }
  ଫେରନ୍ତୁ &code_props_none;
}

/*-----------------------------------------------------------------.
| Set the PRECEDENCE associated with SYM.  Does nothing if invoked |
| with UNDEF_ASSOC as ASSOC.                                       |
`-----------------------------------------------------------------*/

ଶୂନ୍ୟ
ପ୍ରତୀକ_precedence_set (ପ୍ରତୀକ *sym, ପୂର୍ଣ୍ଣସଂଖ୍ୟା prec, assoc a, location loc)
{
  ଯଦି (a != undef_assoc)
    {
      sym_content *s = sym->content;
      ଯଦି (s->prec)
        complain_ପ୍ରତୀକ_redeclared (sym, assoc_to_string (a),
                                    s->prec_loc, loc);
      ଅନ୍ୟଥା
        {
          s->prec = prec;
          s->assoc = a;
          s->prec_loc = loc;
        }
    }

  /* Only terminals have a precedence. */
  ପ୍ରତୀକ_class_set (sym, token_sym, loc, false);
}


/*------------------------------------.
| Set the CLASS associated with SYM.  |
`------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
complain_pct_type_on_token (location *loc)
{
  complain (loc, Wyacc,
            _("POSIX yacc reserves %%type to nonterminals"));
}

ଶୂନ୍ୟ
ପ୍ରତୀକ_class_set (ପ୍ରତୀକ *sym, ପ୍ରତୀକ_class class, location loc, bool declaring)
{
  aver (class != unknown_sym);
  sym_content *s = sym->content;
  ଯଦି (class == pct_type_sym)
    {
      ଯଦି (s->class == token_sym)
        complain_pct_type_on_token (&loc);
      ଅନ୍ୟଥା ଯଦି (s->class == unknown_sym)
        s->class = class;
    }
  ଅନ୍ୟଥା ଯଦି (s->class != unknown_sym && s->class != pct_type_sym
           && s->class != class)
    complain_class_redeclared (sym, class, loc);
  ଅନ୍ୟଥା
    {
      ଯଦି (class == token_sym && s->class == pct_type_sym)
        complain_pct_type_on_token (&sym->location);

      s->class = class;

      ଯଦି (declaring)
        {
          ଯଦି (s->status == declared)
            {
              complain (&loc, Wother,
                        _("symbol %s redeclared"), sym->tag);
              subcomplain (&sym->location, Wother,
                           _("previous declaration"));
            }
          ଅନ୍ୟଥା
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

ଶୂନ୍ୟ
ପ୍ରତୀକ_code_set (ପ୍ରତୀକ *sym, ପୂର୍ଣ୍ଣସଂଖ୍ୟା code, location loc)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା *codep = &sym->content->code;
  ଯଦି (sym->content->class != token_sym)
    complain (&loc, complaint,
              _("nonterminals cannot be given a token code"));
  ଅନ୍ୟଥା ଯଦି (*codep != CODE_UNDEFINED
           && *codep != code)
    complain (&loc, complaint, _("redefining code of token %s"),
              sym->tag);
  ଅନ୍ୟଥା ଯଦି (code == INT_MAX)
    complain (&loc, complaint, _("code of token %s too large"),
              sym->tag);
  ଅନ୍ୟଥା
    {
      *codep = code;
      /* User defined $end token? */
      ଯଦି (code == 0 && !eoftoken)
        {
          eoftoken = sym->content->ପ୍ରତୀକ;
          eoftoken->content->ସଂଖ୍ୟା = 0;
        }
    }
}


/*----------------------------------------------------------.
| If SYM is not defined, report an error, and consider it a |
| nonterminal.                                              |
`----------------------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
ପ୍ରତୀକ_check_defined (ପ୍ରତୀକ *sym)
{
  sym_content *s = sym->content;
  ଯଦି (s->class == unknown_sym || s->class == pct_type_sym)
    {
      complain_ପ୍ରତୀକ_undeclared (sym);
      s->class = nterm_sym;
    }

  ଯଦି (s->ସଂଖ୍ୟା == ସଂଖ୍ୟା_UNDEFINED)
    s->ସଂଖ୍ୟା = s->class == token_sym ? ntokens++ : nnterms++;

  ଯଦି (s->class == token_sym
      && sym->tag[0] == '"'
      && !sym->is_alias)
    complain (&sym->location, Wdangling_alias,
              _("string literal %s not attached to a symbol"),
              sym->tag);

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < 2; ++i)
    ପ୍ରତୀକ_code_props_get (sym, i)->is_used = true;

  /* Set the semantic type status associated to the current symbol to
     'declared' so that we could check semantic types unnecessary uses. */
  ଯଦି (s->type_name)
    {
      semantic_type *sem_type = semantic_type_get (s->type_name, ଖାଲି);
      ଯଦି (sem_type)
        sem_type->status = declared;
    }
}

ସ୍ଥିର ଶୂନ୍ୟ
semantic_type_check_defined (semantic_type *sem_type)
{
  /* <*> and <> do not have to be "declared".  */
  ଯଦି (sem_type->status == declared
      || !*sem_type->tag
      || STREQ (sem_type->tag, "*"))
    {
      ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < 2; ++i)
        ଯଦି (sem_type->props[i].kind != CODE_PROPS_NONE
            && ! sem_type->props[i].is_used)
          complain (&sem_type->location, Wother,
                    _("useless %s for type <%s>"),
                    code_props_type_string (i), sem_type->tag);
    }
  ଅନ୍ୟଥା
    complain (&sem_type->location, Wother,
              _("type <%s> is used, but is not associated to any symbol"),
              sem_type->tag);
}

/*-------------------------------------------------------------------.
| Merge the properties (precedence, associativity, etc.) of SYM, and |
| its string-named alias STR; check consistency.                     |
`-------------------------------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
ପ୍ରତୀକ_merge_properties (ପ୍ରତୀକ *sym, ପ୍ରତୀକ *str)
{
  ଯଦି (str->content->type_name != sym->content->type_name)
    {
      ଯଦି (str->content->type_name)
        ପ୍ରତୀକ_type_set (sym,
                         str->content->type_name, str->content->type_loc);
      ଅନ୍ୟଥା
        ପ୍ରତୀକ_type_set (str,
                         sym->content->type_name, sym->content->type_loc);
    }


  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < CODE_PROPS_SIZE; ++i)
    ଯଦି (str->content->props[i].code)
      ପ୍ରତୀକ_code_props_set (sym, i, &str->content->props[i]);
    ଅନ୍ୟଥା ଯଦି (sym->content->props[i].code)
      ପ୍ରତୀକ_code_props_set (str, i, &sym->content->props[i]);

  ଯଦି (sym->content->prec || str->content->prec)
    {
      ଯଦି (str->content->prec)
        ପ୍ରତୀକ_precedence_set (sym, str->content->prec, str->content->assoc,
                               str->content->prec_loc);
      ଅନ୍ୟଥା
        ପ୍ରତୀକ_precedence_set (str, sym->content->prec, sym->content->assoc,
                               sym->content->prec_loc);
    }
}

ଶୂନ୍ୟ
ପ୍ରତୀକ_make_alias (ପ୍ରତୀକ *sym, ପ୍ରତୀକ *str, location loc)
{
  ଯଦି (sym->content->class != token_sym)
    complain (&loc, complaint,
              _("nonterminals cannot be given a string alias"));
  ଅନ୍ୟଥା ଯଦି (str->alias)
    complain (&loc, Wother,
              _("symbol %s used more than once as a literal string"), str->tag);
  ଅନ୍ୟଥା ଯଦି (sym->alias)
    complain (&loc, Wother,
              _("symbol %s given more than one literal string"), sym->tag);
  ଅନ୍ୟଥା
    {
      ପ୍ରତୀକ_merge_properties (sym, str);
      sym_content_free (str->content);
      str->content = sym->content;
      str->content->ପ୍ରତୀକ = str;
      str->is_alias = true;
      str->alias = sym;
      sym->alias = str;
    }
}


/*-------------------------------------------------------------------.
| Assign a symbol number, and write the definition of the token name |
| into FDEFINES.  Put in SYMBOLS.                                    |
`-------------------------------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
ପ୍ରତୀକ_pack (ପ୍ରତୀକ *sym)
{
  aver (sym->content->ସଂଖ୍ୟା != ସଂଖ୍ୟା_UNDEFINED);
  ଯଦି (sym->content->class == nterm_sym)
    sym->content->ସଂଖ୍ୟା += ntokens;

  symbols[sym->content->ସଂଖ୍ୟା] = sym->content->ପ୍ରତୀକ;
}

ସ୍ଥିର ଶୂନ୍ୟ
complain_code_redeclared (ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ, ଅଚଳ ପ୍ରତୀକ *first, ଅଚଳ ପ୍ରତୀକ *second)
{
  symbols_sort (&first, &second);
  complain (&second->location, complaint,
            _("code %d reassigned to token %s"),
            ଅଙ୍କ, second->tag);
  subcomplain (&first->location, complaint,
               _("previous declaration for %s"),
               first->tag);
}

/*-------------------------------------------------.
| Put SYM in TOKEN_TRANSLATIONS if it is a token.  |
`-------------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
ପ୍ରତୀକ_translation (ଅଚଳ ପ୍ରତୀକ *sym)
{
  ଯଦି (sym->content->class == token_sym && !sym->is_alias)
    {
      /* A token whose translation has already been set? */
      ଯଦି (token_translations[sym->content->code]
          != undeftoken->content->ସଂଖ୍ୟା)
        complain_code_redeclared
          (sym->content->code,
           symbols[token_translations[sym->content->code]], sym);
      ଅନ୍ୟଥା
        token_translations[sym->content->code]
          = sym->content->ସଂଖ୍ୟା;
    }
}


/*---------------------------------------.
| Symbol and semantic type hash tables.  |
`---------------------------------------*/

/* Initial capacity of symbol and semantic type hash table.  */
#define HT_INITIAL_CAPACITY 257

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
hash_compare_ପ୍ରତୀକ (ଅଚଳ ପ୍ରତୀକ *m1, ଅଚଳ ପ୍ରତୀକ *m2)
{
  /* Since tags are unique, we can compare the pointers themselves.  */
  ଫେରନ୍ତୁ UNIQSTR_EQ (m1->tag, m2->tag);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
hash_compare_semantic_type (ଅଚଳ semantic_type *m1, ଅଚଳ semantic_type *m2)
{
  /* Since names are unique, we can compare the pointers themselves.  */
  ଫେରନ୍ତୁ UNIQSTR_EQ (m1->tag, m2->tag);
}

ସ୍ଥିର bool
hash_ପ୍ରତୀକ_comparator (ଶୂନ୍ୟ ଅଚଳ *m1, ଶୂନ୍ୟ ଅଚଳ *m2)
{
  ଫେରନ୍ତୁ hash_compare_ପ୍ରତୀକ (m1, m2);
}

ସ୍ଥିର bool
hash_semantic_type_comparator (ଶୂନ୍ୟ ଅଚଳ *m1, ଶୂନ୍ୟ ଅଚଳ *m2)
{
  ଫେରନ୍ତୁ hash_compare_semantic_type (m1, m2);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ size_t
hash_ପ୍ରତୀକ (ଅଚଳ ପ୍ରତୀକ *m, size_t tablesize)
{
  /* Since tags are unique, we can hash the pointer itself.  */
  ଫେରନ୍ତୁ ((uintptr_t) m->tag) % tablesize;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ size_t
hash_semantic_type (ଅଚଳ semantic_type *m, size_t tablesize)
{
  /* Since names are unique, we can hash the pointer itself.  */
  ଫେରନ୍ତୁ ((uintptr_t) m->tag) % tablesize;
}

ସ୍ଥିର size_t
hash_ପ୍ରତୀକ_hasher (ଶୂନ୍ୟ ଅଚଳ *m, size_t tablesize)
{
  ଫେରନ୍ତୁ hash_ପ୍ରତୀକ (m, tablesize);
}

ସ୍ଥିର size_t
hash_semantic_type_hasher (ଶୂନ୍ୟ ଅଚଳ *m, size_t tablesize)
{
  ଫେରନ୍ତୁ hash_semantic_type (m, tablesize);
}

/*-------------------------------.
| Create the symbol hash table.  |
`-------------------------------*/

ଶୂନ୍ୟ
symbols_new (ଶୂନ୍ୟ)
{
  ପ୍ରତୀକ_table = hash_xinitialize (HT_INITIAL_CAPACITY,
                                   ଖାଲି,
                                   hash_ପ୍ରତୀକ_hasher,
                                   hash_ପ୍ରତୀକ_comparator,
                                   ପ୍ରତୀକ_free);

  /* Construct the acceptsymbol symbol. */
  acceptsymbol = ପ୍ରତୀକ_get ("$accept", empty_loc);
  acceptsymbol->content->class = nterm_sym;
  acceptsymbol->content->ସଂଖ୍ୟା = nnterms++;

  /* Construct the YYerror/"error" token */
  errtoken = ପ୍ରତୀକ_get ("YYerror", empty_loc);
  errtoken->content->class = token_sym;
  errtoken->content->ସଂଖ୍ୟା = ntokens++;
  {
    ପ୍ରତୀକ *alias = ପ୍ରତୀକ_get ("error", empty_loc);
    ପ୍ରତୀକ_class_set (alias, token_sym, empty_loc, false);
    ପ୍ରତୀକ_make_alias (errtoken, alias, empty_loc);
  }

  /* Construct the YYUNDEF/"$undefined" token that represents all
     undefined literal tokens.  It is always symbol number 2.  */
  undeftoken = ପ୍ରତୀକ_get ("YYUNDEF", empty_loc);
  undeftoken->content->class = token_sym;
  undeftoken->content->ସଂଖ୍ୟା = ntokens++;
  {
    ପ୍ରତୀକ *alias = ପ୍ରତୀକ_get ("$undefined", empty_loc);
    ପ୍ରତୀକ_class_set (alias, token_sym, empty_loc, false);
    ପ୍ରତୀକ_make_alias (undeftoken, alias, empty_loc);
  }

  semantic_type_table = hash_xinitialize (HT_INITIAL_CAPACITY,
                                          ଖାଲି,
                                          hash_semantic_type_hasher,
                                          hash_semantic_type_comparator,
                                          ମୁକ୍ତ);
}


/*----------------------------------------------------------------.
| Find the symbol named KEY, and return it.  If it does not exist |
| yet, create it.                                                 |
`----------------------------------------------------------------*/

ପ୍ରତୀକ *
ପ୍ରତୀକ_from_uniqstr (ଅଚଳ uniqstr ଚାବି, location loc)
{
  ପ୍ରତୀକ probe;

  probe.tag = ଚାବି;
  ପ୍ରତୀକ *res = hash_lookup (ପ୍ରତୀକ_table, &probe);

  ଯଦି (!res)
    {
      /* First insertion in the hash. */
      aver (!symbols_sorted);
      res = ପ୍ରତୀକ_new (ଚାବି, loc);
      hash_xinsert (ପ୍ରତୀକ_table, res);
    }
  ଫେରନ୍ତୁ res;
}


/*-----------------------------------------------------------------------.
| Find the semantic type named KEY, and return it.  If it does not exist |
| yet, create it.                                                        |
`-----------------------------------------------------------------------*/

semantic_type *
semantic_type_from_uniqstr (ଅଚଳ uniqstr ଚାବି, ଅଚଳ location *loc)
{
  semantic_type probe;

  probe.tag = ଚାବି;
  semantic_type *res = hash_lookup (semantic_type_table, &probe);

  ଯଦି (!res)
    {
      /* First insertion in the hash. */
      res = semantic_type_new (ଚାବି, loc);
      hash_xinsert (semantic_type_table, res);
    }
  ଫେରନ୍ତୁ res;
}


/*----------------------------------------------------------------.
| Find the symbol named KEY, and return it.  If it does not exist |
| yet, create it.                                                 |
`----------------------------------------------------------------*/

ପ୍ରତୀକ *
ପ୍ରତୀକ_get (ଅଚଳ ଅକ୍ଷର *ଚାବି, location loc)
{
  ଫେରନ୍ତୁ ପ୍ରତୀକ_from_uniqstr (uniqstr_new (ଚାବି), loc);
}


/*-----------------------------------------------------------------------.
| Find the semantic type named KEY, and return it.  If it does not exist |
| yet, create it.                                                        |
`-----------------------------------------------------------------------*/

semantic_type *
semantic_type_get (ଅଚଳ ଅକ୍ଷର *ଚାବି, ଅଚଳ location *loc)
{
  ଫେରନ୍ତୁ semantic_type_from_uniqstr (uniqstr_new (ଚାବି), loc);
}


/*------------------------------------------------------------------.
| Generate a dummy nonterminal, whose name cannot conflict with the |
| user's names.                                                     |
`------------------------------------------------------------------*/

ପ୍ରତୀକ *
dummy_ପ୍ରତୀକ_get (location loc)
{
  /* Incremented for each generated symbol.  */
  ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା dummy_ଗଣନା = 0;
  ଅକ୍ଷର buf[32];
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା len = snprintf (buf, ଆକାର buf, "$@%d", ++dummy_ଗଣନା);
  assure (len < ଆକାର buf);
  ପ୍ରତୀକ *sym = ପ୍ରତୀକ_get (buf, loc);
  sym->content->class = nterm_sym;
  ଫେରନ୍ତୁ sym;
}

bool
ପ୍ରତୀକ_is_dummy (ପ୍ରତୀକ ଅଚଳ *sym)
{
  ଫେରନ୍ତୁ sym->tag[0] == '@' || (sym->tag[0] == '$' && sym->tag[1] == '@');
}

/*-------------------.
| Free the symbols.  |
`-------------------*/

ଶୂନ୍ୟ
symbols_free (ଶୂନ୍ୟ)
{
  hash_free (ପ୍ରତୀକ_table);
  hash_free (semantic_type_table);
  ମୁକ୍ତ (symbols);
  ମୁକ୍ତ (symbols_sorted);
  ମୁକ୍ତ (semantic_types_sorted);
}


ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ପ୍ରତୀକ_cmp (ଶୂନ୍ୟ ଅଚଳ *a, ଶୂନ୍ୟ ଅଚଳ *b)
{
  ଫେରନ୍ତୁ location_cmp ((*(ପ୍ରତୀକ * ଅଚଳ *)a)->location,
                       (*(ପ୍ରତୀକ * ଅଚଳ *)b)->location);
}

/* Store in *SORTED an array of pointers to the symbols contained in
   TABLE, sorted by order of appearance (i.e., by location). */

ସ୍ଥିର ଶୂନ୍ୟ
table_sort (ସଂରଚନା hash_table *table, ପ୍ରତୀକ ***sorted)
{
  aver (!*sorted);
  size_t ଗଣନା_2 = hash_get_n_entries (table);
  *sorted = xnmalloc (ଗଣନା_2 + 1, ଆକାର **sorted);
  hash_get_entries (table, (ଶୂନ୍ୟ**)*sorted, ଗଣନା_2);
  qsort (*sorted, ଗଣନା_2, ଆକାର **sorted, ପ୍ରତୀକ_cmp);
  (*sorted)[ଗଣନା_2] = ଖାଲି;
}

/*--------------------------------------------------------------.
| Check that all the symbols are defined.  Report any undefined |
| symbols and consider them nonterminals.                       |
`--------------------------------------------------------------*/

ଶୂନ୍ୟ
symbols_check_defined (ଶୂନ୍ୟ)
{
  table_sort (ପ୍ରତୀକ_table, &symbols_sorted);
  /* semantic_type, like symbol, starts with a 'tag' field and then a
     'location' field.  And here we only deal with arrays/hashes of
     pointers, sizeof is not an issue.

     So instead of implementing table_sort (and symbol_cmp) once for
     each type, let's lie a bit to the typing system, and treat
     'semantic_type' as if it were 'symbol'. */
  table_sort (semantic_type_table, (ପ୍ରତୀକ ***) &semantic_types_sorted);

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; symbols_sorted[i]; ++i)
    ପ୍ରତୀକ_check_defined (symbols_sorted[i]);
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; semantic_types_sorted[i]; ++i)
    semantic_type_check_defined (semantic_types_sorted[i]);
}

/*------------------------------------------------------------------.
| Set TOKEN_TRANSLATIONS.  Check that no two symbols share the same |
| number.                                                           |
`------------------------------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
symbols_token_translations_init (ଶୂନ୍ୟ)
{
  bool code_256_available_p = true;

  /* Find the highest token code, and whether 256, the POSIX preferred
     token code for the error token, is used.  */
  max_code = 0;
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ntokens; ++i)
    {
      sym_content *sym = symbols[i]->content;
      ଯଦି (sym->code != CODE_UNDEFINED)
        {
          ଯଦି (sym->code > max_code)
            max_code = sym->code;
          ଯଦି (sym->code == 256)
            code_256_available_p = false;
        }
    }

  /* If 256 is not used, assign it to error, to follow POSIX.  */
  ଯଦି (code_256_available_p
      && errtoken->content->code == CODE_UNDEFINED)
    errtoken->content->code = 256;

  /* Set the missing codes. */
  ଯଦି (max_code < 256)
    max_code = 256;

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ntokens; ++i)
    {
      sym_content *sym = symbols[i]->content;
      ଯଦି (sym->code == CODE_UNDEFINED)
        {
          IGNORE_TYPE_LIMITS_BEGIN
          ଯଦି (INT_ADD_WRAPV (max_code, 1, &max_code))
            complain (ଖାଲି, fatal, _("token number too large"));
          IGNORE_TYPE_LIMITS_END
          sym->code = max_code;
        }
      ଯଦି (sym->code > max_code)
        max_code = sym->code;
    }

  token_translations = xnmalloc (max_code + 1,
                                 ଆକାର *token_translations);

  /* Initialize all entries for literal tokens to the internal token
     number for $undefined, which represents all invalid inputs.  */
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < max_code + 1; ++i)
    token_translations[i] = undeftoken->content->ସଂଖ୍ୟା;
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; symbols_sorted[i]; ++i)
    ପ୍ରତୀକ_translation (symbols_sorted[i]);
}


/* Whether some symbol requires internationalization.  */
ସ୍ଥିର bool
has_translations (ଶୂନ୍ୟ)
{
  ପାଇଁ (ଅଚଳ ଶୂନ୍ୟ *entry = hash_get_first (ପ୍ରତୀକ_table);
       entry;
       entry = hash_get_next (ପ୍ରତୀକ_table, entry))
    {
      ଅଚଳ ପ୍ରତୀକ *sym = (ଅଚଳ ପ୍ରତୀକ *) entry;
      ଯଦି (sym->translatable)
        ଫେରନ୍ତୁ true;
    }
  ଫେରନ୍ତୁ false;
}

/*----------------------------------------------------------------.
| Assign symbol numbers, and write definition of token names into |
| FDEFINES.  Set up vectors SYMBOL_TABLE, TAGS of symbols.        |
`----------------------------------------------------------------*/

ଶୂନ୍ୟ
symbols_pack (ଶୂନ୍ୟ)
{
  symbols = xcalloc (nsyms, ଆକାର *symbols);
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; symbols_sorted[i]; ++i)
    ପ୍ରତୀକ_pack (symbols_sorted[i]);

  /* Aliases leave empty slots in symbols, so remove them.  */
  {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା nsyms_old = nsyms;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା writei = 0, readi = 0; readi < nsyms_old; readi += 1)
      {
        ଯଦି (symbols[readi] == ଖାଲି)
          {
            nsyms -= 1;
            ntokens -= 1;
          }
        ଅନ୍ୟଥା
          {
            symbols[writei] = symbols[readi];
            symbols[writei]->content->ସଂଖ୍ୟା = writei;
            writei += 1;
          }
      }
  }
  symbols = xnrealloc (symbols, nsyms, ଆକାର *symbols);

  symbols_token_translations_init ();

  // If some user tokens are internationalized, the internal ones
  // should be too.
  ଯଦି (has_translations ())
    {
      ଅଚଳ bool eof_is_user_defined
        = !eoftoken->alias || STRNEQ (eoftoken->alias->tag, "$end");
      ଯଦି (!eof_is_user_defined)
        eoftoken->alias->translatable = true;
      undeftoken->alias->translatable = true;
      errtoken->alias->translatable = true;
    }
}

/*---------------------------------.
| Initialize relation graph nodes. |
`---------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
init_prec_nodes (ଶୂନ୍ୟ)
{
  prec_nodes = xcalloc (nsyms, ଆକାର *prec_nodes);
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < nsyms; ++i)
    {
      prec_nodes[i] = xmalloc (ଆକାର *prec_nodes[i]);
      symgraph *s = prec_nodes[i];
      s->id = i;
      s->succ = 0;
      s->pred = 0;
    }
}

/*----------------.
| Create a link.  |
`----------------*/

ସ୍ଥିର symgraphlink *
symgraphlink_new (graphid id, symgraphlink *next)
{
  symgraphlink *res = xmalloc (ଆକାର *res);
  res->id = id;
  res->next = next;
  ଫେରନ୍ତୁ res;
}


/*------------------------------------------------------------------.
| Register the second symbol of the precedence relation, and return |
| whether this relation is new.  Use only in register_precedence.   |
`------------------------------------------------------------------*/

ସ୍ଥିର bool
register_precedence_second_ପ୍ରତୀକ (symgraphlink **first, graphid sym)
{
  ଯଦି (!*first || sym < (*first)->id)
    *first = symgraphlink_new (sym, *first);
  ଅନ୍ୟଥା
    {
      symgraphlink *slist = *first;

      ଯେତେବେଳେ (slist->next && slist->next->id <= sym)
        slist = slist->next;

      ଯଦି (slist->id == sym)
        /* Relation already present. */
        ଫେରନ୍ତୁ false;

      slist->next = symgraphlink_new (sym, slist->next);
    }
  ଫେରନ୍ତୁ true;
}

/*------------------------------------------------------------------.
| Register a new relation between symbols as used. The first symbol |
| has a greater precedence than the second one.                     |
`------------------------------------------------------------------*/

ଶୂନ୍ୟ
register_precedence (graphid first, graphid snd)
{
  ଯଦି (!prec_nodes)
    init_prec_nodes ();
  register_precedence_second_ପ୍ରତୀକ (&(prec_nodes[first]->succ), snd);
  register_precedence_second_ପ୍ରତୀକ (&(prec_nodes[snd]->pred), first);
}


/*---------------------------------------.
| Deep clear a linked / adjacency list). |
`---------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
linkedlist_free (symgraphlink *ଗଣ୍ଠି)
{
  ଯଦି (ଗଣ୍ଠି)
    {
      ଯେତେବେଳେ (ଗଣ୍ଠି->next)
        {
          symgraphlink *tmp = ଗଣ୍ଠି->next;
          ମୁକ୍ତ (ଗଣ୍ଠି);
          ଗଣ୍ଠି = tmp;
        }
      ମୁକ୍ତ (ଗଣ୍ଠି);
    }
}

/*----------------------------------------------.
| Clear and destroy association tracking table. |
`----------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
assoc_free (ଶୂନ୍ୟ)
{
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < nsyms; ++i)
    {
      linkedlist_free (prec_nodes[i]->pred);
      linkedlist_free (prec_nodes[i]->succ);
      ମୁକ୍ତ (prec_nodes[i]);
    }
  ମୁକ୍ତ (prec_nodes);
}

/*---------------------------------------.
| Initialize association tracking table. |
`---------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
init_assoc (ଶୂନ୍ୟ)
{
  used_assoc = xcalloc (nsyms, ଆକାର *used_assoc);
  ପାଇଁ (graphid i = 0; i < nsyms; ++i)
    used_assoc[i] = false;
}

/*------------------------------------------------------------------.
| Test if the associativity for the symbols is defined and useless. |
`------------------------------------------------------------------*/

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
is_assoc_useless (ପ୍ରତୀକ *s)
{
  ଫେରନ୍ତୁ s
      && s->content->assoc != undef_assoc
      && s->content->assoc != precedence_assoc
      && !used_assoc[s->content->ସଂଖ୍ୟା];
}

/*-------------------------------.
| Register a used associativity. |
`-------------------------------*/

ଶୂନ୍ୟ
register_assoc (graphid i, graphid j)
{
  ଯଦି (!used_assoc)
    init_assoc ();
  used_assoc[i] = true;
  used_assoc[j] = true;
}

/*--------------------------------------------------.
| Print a warning for unused precedence relations.  |
`--------------------------------------------------*/

ଶୂନ୍ୟ
print_precedence_warnings (ଶୂନ୍ୟ)
{
  ଯଦି (!prec_nodes)
    init_prec_nodes ();
  ଯଦି (!used_assoc)
    init_assoc ();
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < nsyms; ++i)
    {
      ପ୍ରତୀକ *s = symbols[i];
      ଯଦି (s
          && s->content->prec != 0
          && !prec_nodes[i]->pred
          && !prec_nodes[i]->succ)
        {
          ଯଦି (is_assoc_useless (s))
            complain (&s->content->prec_loc, Wprecedence,
                      _("useless precedence and associativity for %s"), s->tag);
          ଅନ୍ୟଥା ଯଦି (s->content->assoc == precedence_assoc)
            complain (&s->content->prec_loc, Wprecedence,
                      _("useless precedence for %s"), s->tag);
        }
      ଅନ୍ୟଥା ଯଦି (is_assoc_useless (s))
        complain (&s->content->prec_loc, Wprecedence,
                  _("useless associativity for %s, use %%precedence"), s->tag);
    }
  ମୁକ୍ତ (used_assoc);
  assoc_free ();
}
