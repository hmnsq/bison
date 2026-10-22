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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "symtab.h"

#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ <assure.h>
#ಅನ್ತರ್ಭಾವ <fstrcmp.h>
#ಅನ್ತರ್ಭಾವ <hash.h>
#ಅನ್ತರ್ಭಾವ <quote.h>

#ಅನ್ತರ್ಭಾವ "complain.h"
#ಅನ್ತರ್ಭಾವ "getargs.h"
#ಅನ್ತರ್ಭಾವ "gram.h"
#ಅನ್ತರ್ಭಾವ "intprops.h"

/** Undefined token code.  */
#define CODE_UNDEFINED (-1)

/* Undefined symbol number.  */
#define ಸಂಖ್ಯೆ_UNDEFINED (-1)


ಸ್ಥಿರ ಸಂರಚನಾ hash_table *ಚಿಹ್ನೆ_table = ರಿಕ್ತ;
ಸ್ಥಿರ ಸಂರಚನಾ hash_table *semantic_type_table = ರಿಕ್ತ;

/*----------------------------------------------------------------.
| Symbols sorted by tag.  Allocated by table_sort, after which no |
| more symbols should be created.                                 |
`----------------------------------------------------------------*/

ಸ್ಥಿರ ಚಿಹ್ನೆ **symbols_sorted = ರಿಕ್ತ;
ಸ್ಥಿರ semantic_type **semantic_types_sorted = ರಿಕ್ತ;


/*------------------------.
| Distinguished symbols.  |
`------------------------*/

ಚಿಹ್ನೆ *errtoken = ರಿಕ್ತ;
ಚಿಹ್ನೆ *undeftoken = ರಿಕ್ತ;
ಚಿಹ್ನೆ *eoftoken = ರಿಕ್ತ;
ಚಿಹ್ನೆ *acceptsymbol = ರಿಕ್ತ;

/* Precedence relation graph. */
ಸ್ಥಿರ symgraph **prec_nodes;

/* Store which associativity is used.  */
ಸ್ಥಿರ bool *used_assoc = ರಿಕ್ತ;

bool tag_seen = false;


/* Whether SYM was defined by the user.  */

ಸ್ಥಿರ bool
ಚಿಹ್ನೆ_is_user_defined (ಚಿಹ್ನೆ *sym)
{
  ಧ್ರುವ bool eof_is_user_defined
    = !eoftoken->alias || STRNEQ (eoftoken->alias->tag, "$end");
  ಪ್ರತ್ಯಾಗಮ sym->tag[0] != '$'
    && (eof_is_user_defined || (sym != eoftoken && sym->alias != errtoken))
    && sym != errtoken && sym->alias != errtoken
    && sym != undeftoken && sym->alias != undeftoken;
}


/*--------------------------.
| Create a new sym_content. |
`--------------------------*/

ಸ್ಥಿರ sym_content *
sym_content_new (ಚಿಹ್ನೆ *s)
{
  sym_content *res = xmalloc (ಪರಿಮಾಣ *res);

  res->ಚಿಹ್ನೆ = s;

  res->type_name = ರಿಕ್ತ;
  res->type_loc = empty_loc;
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < CODE_PROPS_SIZE; ++i)
    code_props_none_init (&res->props[i]);

  res->ಸಂಖ್ಯೆ = ಸಂಖ್ಯೆ_UNDEFINED;
  res->prec_loc = empty_loc;
  res->prec = 0;
  res->assoc = undef_assoc;
  res->code = CODE_UNDEFINED;

  res->class = unknown_sym;
  res->status = undeclared;

  ಪ್ರತ್ಯಾಗಮ res;
}

/*---------------------------------.
| Create a new symbol, named TAG.  |
`---------------------------------*/

ಸ್ಥಿರ ಚಿಹ್ನೆ *
ಚಿಹ್ನೆ_new (uniqstr tag, location loc)
{
  ಚಿಹ್ನೆ *res = xmalloc (ಪರಿಮಾಣ *res);
  uniqstr_assert (tag);

  /* If the tag is not a string (starts with a double quote), check
     that it is valid for Yacc. */
  ಯದಿ (tag[0] != '\"' && tag[0] != '\'' && strchr (tag, '-'))
    complain (&loc, Wyacc,
              _("POSIX Yacc forbids dashes in symbol names: %s"), tag);

  res->tag = tag;
  res->location = loc;
  res->translatable = false;
  res->location_of_lhs = false;
  res->alias = ರಿಕ್ತ;
  res->content = sym_content_new (res);
  res->is_alias = false;
  ಪ್ರತ್ಯಾಗಮ res;
}

/*--------------------.
| Free a sym_content. |
`--------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
sym_content_free (sym_content *sym)
{
  ಮುಕ್ತ (sym);
}


/*---------------------------------------------------------.
| Free a symbol and its associated content if appropriate. |
`---------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
ಚಿಹ್ನೆ_free (ಶೂನ್ಯ *ptr)
{
  ಚಿಹ್ನೆ *sym = (ಚಿಹ್ನೆ *)ptr;
  ಯದಿ (!sym->is_alias)
    sym_content_free (sym->content);
  ಮುಕ್ತ (sym);

}

/* If needed, swap first and second so that first has the earliest
   location (according to location_cmp).

   Many symbol features (e.g., token codes) are not assigned during
   parsing, but in a second step, via a traversal of the symbol table
   sorted on tag.

   However, error messages make more sense if we keep the first
   declaration first.
*/

ಸ್ಥಿರ ಶೂನ್ಯ
symbols_sort (ಧ್ರುವ ಚಿಹ್ನೆ **first, ಧ್ರುವ ಚಿಹ್ನೆ **second)
{
  ಯದಿ (0 < location_cmp ((*first)->location, (*second)->location))
    {
      ಧ್ರುವ ಚಿಹ್ನೆ* tmp = *first;
      *first = *second;
      *second = tmp;
    }
}

/* Likewise, for locations.  */

ಸ್ಥಿರ ಶೂನ್ಯ
locations_sort (location *first, location *second)
{
  ಯದಿ (0 < location_cmp (*first, *second))
    {
      location tmp = *first;
      *first = *second;
      *second = tmp;
    }
}

ವರ್ಣ ಧ್ರುವ *
code_props_type_string (code_props_type kind)
{
  ವಿಕಲ್ಪ (kind)
    {
    ಪಕ್ಷ destructor:
      ಪ್ರತ್ಯಾಗಮ "%destructor";
    ಪಕ್ಷ printer:
      ಪ್ರತ್ಯಾಗಮ "%printer";
    }
  abort ();
}


/*----------------------------------------.
| Create a new semantic type, named TAG.  |
`----------------------------------------*/

ಸ್ಥಿರ semantic_type *
semantic_type_new (uniqstr tag, ಧ್ರುವ location *loc)
{
  semantic_type *res = xmalloc (ಪರಿಮಾಣ *res);

  uniqstr_assert (tag);
  res->tag = tag;
  res->location = loc ? *loc : empty_loc;
  res->status = undeclared;
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < CODE_PROPS_SIZE; ++i)
    code_props_none_init (&res->props[i]);

  ಪ್ರತ್ಯಾಗಮ res;
}


/*-----------------.
| Print a symbol.  |
`-----------------*/

#define ಚಿಹ್ನೆ_INT_ATTR_PRINT(Attr)                     \
  ಯದಿ (s->content)                                       \
    fprintf (f, " %s = %d", #Attr, s->content->Attr)

#define ಚಿಹ್ನೆ_STR_ATTR_PRINT(Attr)                     \
  ಯದಿ (s->content && s->content->Attr)                   \
    fprintf (f, " %s { %s }", #Attr, s->content->Attr)

#define ಚಿಹ್ನೆ_CODE_PRINT(Attr)                                         \
  ಯದಿ (s->content && s->content->props[Attr].code)                       \
    fprintf (f, " %s { %s }", #Attr, s->content->props[Attr].code)

ಶೂನ್ಯ
ಚಿಹ್ನೆ_print (ಚಿಹ್ನೆ ಧ್ರುವ *s, FILE *f)
{
  ಯದಿ (s)
    {
      ಚಿಹ್ನೆ_class c = s->content->class;
      fprintf (f, "%s: %s",
               c == unknown_sym    ? "unknown"
               : c == pct_type_sym ? "%type"
               : c == token_sym    ? "token"
               : c == nterm_sym    ? "nterm"
               : ರಿಕ್ತ, /* abort.  */
               s->tag);
      putc (' ', f);
      location_print (s->location, f);
      ಚಿಹ್ನೆ_INT_ATTR_PRINT (code);
      ಚಿಹ್ನೆ_INT_ATTR_PRINT (ಸಂಖ್ಯೆ);
      ಚಿಹ್ನೆ_STR_ATTR_PRINT (type_name);
      ಚಿಹ್ನೆ_CODE_PRINT (destructor);
      ಚಿಹ್ನೆ_CODE_PRINT (printer);
    }
  ಅನ್ಯಥಾ
    fputs ("<NULL>", f);
}

#undef ಚಿಹ್ನೆ_ATTR_PRINT
#undef ಚಿಹ್ನೆ_CODE_PRINT


/*----------------------------------.
| Whether S is a valid identifier.  |
`----------------------------------*/

ಸ್ಥಿರ bool
is_identifier (uniqstr s)
{
  ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ alphanum[26 + 26 + 1 + 10] =
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "_"
    "0123456789";
  ಯದಿ (!s || ! memchr (alphanum, *s, ಪರಿಮಾಣ alphanum - 10))
    ಪ್ರತ್ಯಾಗಮ false;
  ಪ್ರತಿ (++s; *s; ++s)
    ಯದಿ (! memchr (alphanum, *s, ಪರಿಮಾಣ alphanum))
      ಪ್ರತ್ಯಾಗಮ false;
  ಪ್ರತ್ಯಾಗಮ true;
}


/*-----------------------------------------------.
| Get the identifier associated to this symbol.  |
`-----------------------------------------------*/

uniqstr
ಚಿಹ್ನೆ_id_get (ಚಿಹ್ನೆ ಧ್ರುವ *sym)
{
  // There's one weird case: YYerror is the alias, and error is the
  // base symbol.  Return YYerror in that case.
  ಯದಿ (sym->alias && is_identifier (sym->alias->tag))
    ಪ್ರತ್ಯಾಗಮ sym->alias->tag;
  ಅನ್ಯಥಾ ಯದಿ (is_identifier (sym->tag))
    ಪ್ರತ್ಯಾಗಮ sym->tag;
  ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}


/*------------------------------------------------------------------.
| Complain that S's WHAT is redeclared at SECOND, and was first set |
| at FIRST.                                                         |
`------------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
complain_ಚಿಹ್ನೆ_redeclared (ಚಿಹ್ನೆ *s, ಧ್ರುವ ವರ್ಣ *what, location first,
                            location second)
{
  locations_sort (&first, &second);
  complain (&second, complaint, _("%s redeclaration for %s"), what, s->tag);
  subcomplain (&first, complaint, _("previous declaration"));
}

ಸ್ಥಿರ ಶೂನ್ಯ
complain_semantic_type_redeclared (semantic_type *s, ಧ್ರುವ ವರ್ಣ *what, location first,
                                   location second)
{
  locations_sort (&first, &second);
  complain (&second, complaint, _("%s redeclaration for <%s>"), what, s->tag);
  subcomplain (&first, complaint, _("previous declaration"));
}

ಸ್ಥಿರ ಶೂನ್ಯ
complain_class_redeclared (ಚಿಹ್ನೆ *sym, ಚಿಹ್ನೆ_class class, location second)
{
  complain (&second, complaint,
            class == token_sym
            ? _("symbol %s redeclared as a token")
            : _("symbol %s redeclared as a nonterminal"), sym->tag);
  ಯದಿ (!location_empty (sym->location))
    subcomplain (&sym->location, complaint, _("previous definition"));
}

ಸ್ಥಿರ ಧ್ರುವ ಚಿಹ್ನೆ *
ಚಿಹ್ನೆ_from_uniqstr_fuzzy (ಧ್ರುವ uniqstr ಕೀಲಿ)
{
  aver (symbols_sorted);
#define FSTRCMP_THRESHOLD 0.6
  ದ್ವಿಗುಣ best_similarity = FSTRCMP_THRESHOLD;
  ಧ್ರುವ ಚಿಹ್ನೆ *res = ರಿಕ್ತ;
  size_t ಎಣಿಕೆ = hash_get_n_entries (ಚಿಹ್ನೆ_table);
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; ++i)
    {
      ಚಿಹ್ನೆ *sym = symbols_sorted[i];
      ಯದಿ (STRNEQ (ಕೀಲಿ, sym->tag)
          && (sym->content->status == declared
              || sym->content->status == undeclared))
        {
          ದ್ವಿಗುಣ similarity = fstrcmp_bounded (ಕೀಲಿ, sym->tag, best_similarity);
          ಯದಿ (best_similarity < similarity)
            {
              res = sym;
              best_similarity = similarity;
            }
        }
    }
  ಪ್ರತ್ಯಾಗಮ res;
}

ಸ್ಥಿರ ಶೂನ್ಯ
complain_ಚಿಹ್ನೆ_undeclared (ಧ್ರುವ ಚಿಹ್ನೆ *sym)
{
  ಪ್ರತಿಜ್ಞಾ (sym->content->status != declared);
  ಧ್ರುವ ಚಿಹ್ನೆ *best = ಚಿಹ್ನೆ_from_uniqstr_fuzzy (sym->tag);
  ಯದಿ (best)
    {
      complain (&sym->location,
                sym->content->status == needed ? complaint : Wother,
                _("symbol %s is used, but is not defined as a token"
                  " and has no rules; did you mean %s?"),
                quote_n (0, sym->tag),
                quote_n (1, best->tag));
      ಯದಿ (feature_flag & feature_caret)
        location_caret_suggestion (sym->location, best->tag, stderr);
    }
  ಅನ್ಯಥಾ
    complain (&sym->location,
              sym->content->status == needed ? complaint : Wother,
              _("symbol %s is used, but is not defined as a token"
                " and has no rules"),
              quote (sym->tag));
}

ಶೂನ್ಯ
ಚಿಹ್ನೆ_location_as_lhs_set (ಚಿಹ್ನೆ *sym, location loc)
{
  ಯದಿ (!sym->location_of_lhs)
    {
      sym->location = loc;
      sym->location_of_lhs = true;
    }
}


/*-----------------------------------------------------------------.
| Set the TYPE_NAME associated with SYM.  Does nothing if passed 0 |
| as TYPE_NAME.                                                    |
`-----------------------------------------------------------------*/

ಶೂನ್ಯ
ಚಿಹ್ನೆ_type_set (ಚಿಹ್ನೆ *sym, uniqstr type_name, location loc)
{
  ಯದಿ (type_name)
    {
      tag_seen = true;
      ಯದಿ (sym->content->type_name)
        complain_ಚಿಹ್ನೆ_redeclared (sym, "%type",
                                    sym->content->type_loc, loc);
      ಅನ್ಯಥಾ
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

ಶೂನ್ಯ
ಚಿಹ್ನೆ_code_props_set (ಚಿಹ್ನೆ *sym, code_props_type kind,
                       code_props ಧ್ರುವ *code)
{
  ಯದಿ (sym->content->props[kind].code)
    complain_ಚಿಹ್ನೆ_redeclared (sym, code_props_type_string (kind),
                                sym->content->props[kind].location,
                                code->location);
  ಅನ್ಯಥಾ
    sym->content->props[kind] = *code;
}


/*-----------------------------------------------------.
| Set the DESTRUCTOR or PRINTER associated with TYPE.  |
`-----------------------------------------------------*/

ಶೂನ್ಯ
semantic_type_code_props_set (semantic_type *type,
                              code_props_type kind,
                              code_props ಧ್ರುವ *code)
{
  ಯದಿ (type->props[kind].code)
    complain_semantic_type_redeclared (type, code_props_type_string (kind),
                                       type->props[kind].location,
                                       code->location);
  ಅನ್ಯಥಾ
    type->props[kind] = *code;
}

/*---------------------------------------------------.
| Get the computed %destructor or %printer for SYM.  |
`---------------------------------------------------*/

code_props *
ಚಿಹ್ನೆ_code_props_get (ಚಿಹ್ನೆ *sym, code_props_type kind)
{
  /* Per-symbol code props.  */
  ಯದಿ (sym->content->props[kind].code)
    ಪ್ರತ್ಯಾಗಮ &sym->content->props[kind];

  /* Per-type code props.  */
  ಯದಿ (sym->content->type_name)
    {
      code_props *code =
        &semantic_type_get (sym->content->type_name, ರಿಕ್ತ)->props[kind];
      ಯದಿ (code->code)
        ಪ್ರತ್ಯಾಗಮ code;
    }

  /* Apply default code props's only to user-defined symbols.  */
  ಯದಿ (ಚಿಹ್ನೆ_is_user_defined (sym))
    {
      code_props *code = &semantic_type_get (sym->content->type_name ? "*" : "",
                                             ರಿಕ್ತ)->props[kind];
      ಯದಿ (code->code)
        ಪ್ರತ್ಯಾಗಮ code;
    }
  ಪ್ರತ್ಯಾಗಮ &code_props_none;
}

/*-----------------------------------------------------------------.
| Set the PRECEDENCE associated with SYM.  Does nothing if invoked |
| with UNDEF_ASSOC as ASSOC.                                       |
`-----------------------------------------------------------------*/

ಶೂನ್ಯ
ಚಿಹ್ನೆ_precedence_set (ಚಿಹ್ನೆ *sym, ಪೂರ್ಣಾಙ್ಕ prec, assoc a, location loc)
{
  ಯದಿ (a != undef_assoc)
    {
      sym_content *s = sym->content;
      ಯದಿ (s->prec)
        complain_ಚಿಹ್ನೆ_redeclared (sym, assoc_to_string (a),
                                    s->prec_loc, loc);
      ಅನ್ಯಥಾ
        {
          s->prec = prec;
          s->assoc = a;
          s->prec_loc = loc;
        }
    }

  /* Only terminals have a precedence. */
  ಚಿಹ್ನೆ_class_set (sym, token_sym, loc, false);
}


/*------------------------------------.
| Set the CLASS associated with SYM.  |
`------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
complain_pct_type_on_token (location *loc)
{
  complain (loc, Wyacc,
            _("POSIX yacc reserves %%type to nonterminals"));
}

ಶೂನ್ಯ
ಚಿಹ್ನೆ_class_set (ಚಿಹ್ನೆ *sym, ಚಿಹ್ನೆ_class class, location loc, bool declaring)
{
  aver (class != unknown_sym);
  sym_content *s = sym->content;
  ಯದಿ (class == pct_type_sym)
    {
      ಯದಿ (s->class == token_sym)
        complain_pct_type_on_token (&loc);
      ಅನ್ಯಥಾ ಯದಿ (s->class == unknown_sym)
        s->class = class;
    }
  ಅನ್ಯಥಾ ಯದಿ (s->class != unknown_sym && s->class != pct_type_sym
           && s->class != class)
    complain_class_redeclared (sym, class, loc);
  ಅನ್ಯಥಾ
    {
      ಯದಿ (class == token_sym && s->class == pct_type_sym)
        complain_pct_type_on_token (&sym->location);

      s->class = class;

      ಯದಿ (declaring)
        {
          ಯದಿ (s->status == declared)
            {
              complain (&loc, Wother,
                        _("symbol %s redeclared"), sym->tag);
              subcomplain (&sym->location, Wother,
                           _("previous declaration"));
            }
          ಅನ್ಯಥಾ
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

ಶೂನ್ಯ
ಚಿಹ್ನೆ_code_set (ಚಿಹ್ನೆ *sym, ಪೂರ್ಣಾಙ್ಕ code, location loc)
{
  ಪೂರ್ಣಾಙ್ಕ *codep = &sym->content->code;
  ಯದಿ (sym->content->class != token_sym)
    complain (&loc, complaint,
              _("nonterminals cannot be given a token code"));
  ಅನ್ಯಥಾ ಯದಿ (*codep != CODE_UNDEFINED
           && *codep != code)
    complain (&loc, complaint, _("redefining code of token %s"),
              sym->tag);
  ಅನ್ಯಥಾ ಯದಿ (code == INT_MAX)
    complain (&loc, complaint, _("code of token %s too large"),
              sym->tag);
  ಅನ್ಯಥಾ
    {
      *codep = code;
      /* User defined $end token? */
      ಯದಿ (code == 0 && !eoftoken)
        {
          eoftoken = sym->content->ಚಿಹ್ನೆ;
          eoftoken->content->ಸಂಖ್ಯೆ = 0;
        }
    }
}


/*----------------------------------------------------------.
| If SYM is not defined, report an error, and consider it a |
| nonterminal.                                              |
`----------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
ಚಿಹ್ನೆ_check_defined (ಚಿಹ್ನೆ *sym)
{
  sym_content *s = sym->content;
  ಯದಿ (s->class == unknown_sym || s->class == pct_type_sym)
    {
      complain_ಚಿಹ್ನೆ_undeclared (sym);
      s->class = nterm_sym;
    }

  ಯದಿ (s->ಸಂಖ್ಯೆ == ಸಂಖ್ಯೆ_UNDEFINED)
    s->ಸಂಖ್ಯೆ = s->class == token_sym ? ntokens++ : nnterms++;

  ಯದಿ (s->class == token_sym
      && sym->tag[0] == '"'
      && !sym->is_alias)
    complain (&sym->location, Wdangling_alias,
              _("string literal %s not attached to a symbol"),
              sym->tag);

  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < 2; ++i)
    ಚಿಹ್ನೆ_code_props_get (sym, i)->is_used = true;

  /* Set the semantic type status associated to the current symbol to
     'declared' so that we could check semantic types unnecessary uses. */
  ಯದಿ (s->type_name)
    {
      semantic_type *sem_type = semantic_type_get (s->type_name, ರಿಕ್ತ);
      ಯದಿ (sem_type)
        sem_type->status = declared;
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
semantic_type_check_defined (semantic_type *sem_type)
{
  /* <*> and <> do not have to be "declared".  */
  ಯದಿ (sem_type->status == declared
      || !*sem_type->tag
      || STREQ (sem_type->tag, "*"))
    {
      ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < 2; ++i)
        ಯದಿ (sem_type->props[i].kind != CODE_PROPS_NONE
            && ! sem_type->props[i].is_used)
          complain (&sem_type->location, Wother,
                    _("useless %s for type <%s>"),
                    code_props_type_string (i), sem_type->tag);
    }
  ಅನ್ಯಥಾ
    complain (&sem_type->location, Wother,
              _("type <%s> is used, but is not associated to any symbol"),
              sem_type->tag);
}

/*-------------------------------------------------------------------.
| Merge the properties (precedence, associativity, etc.) of SYM, and |
| its string-named alias STR; check consistency.                     |
`-------------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
ಚಿಹ್ನೆ_merge_properties (ಚಿಹ್ನೆ *sym, ಚಿಹ್ನೆ *str)
{
  ಯದಿ (str->content->type_name != sym->content->type_name)
    {
      ಯದಿ (str->content->type_name)
        ಚಿಹ್ನೆ_type_set (sym,
                         str->content->type_name, str->content->type_loc);
      ಅನ್ಯಥಾ
        ಚಿಹ್ನೆ_type_set (str,
                         sym->content->type_name, sym->content->type_loc);
    }


  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < CODE_PROPS_SIZE; ++i)
    ಯದಿ (str->content->props[i].code)
      ಚಿಹ್ನೆ_code_props_set (sym, i, &str->content->props[i]);
    ಅನ್ಯಥಾ ಯದಿ (sym->content->props[i].code)
      ಚಿಹ್ನೆ_code_props_set (str, i, &sym->content->props[i]);

  ಯದಿ (sym->content->prec || str->content->prec)
    {
      ಯದಿ (str->content->prec)
        ಚಿಹ್ನೆ_precedence_set (sym, str->content->prec, str->content->assoc,
                               str->content->prec_loc);
      ಅನ್ಯಥಾ
        ಚಿಹ್ನೆ_precedence_set (str, sym->content->prec, sym->content->assoc,
                               sym->content->prec_loc);
    }
}

ಶೂನ್ಯ
ಚಿಹ್ನೆ_make_alias (ಚಿಹ್ನೆ *sym, ಚಿಹ್ನೆ *str, location loc)
{
  ಯದಿ (sym->content->class != token_sym)
    complain (&loc, complaint,
              _("nonterminals cannot be given a string alias"));
  ಅನ್ಯಥಾ ಯದಿ (str->alias)
    complain (&loc, Wother,
              _("symbol %s used more than once as a literal string"), str->tag);
  ಅನ್ಯಥಾ ಯದಿ (sym->alias)
    complain (&loc, Wother,
              _("symbol %s given more than one literal string"), sym->tag);
  ಅನ್ಯಥಾ
    {
      ಚಿಹ್ನೆ_merge_properties (sym, str);
      sym_content_free (str->content);
      str->content = sym->content;
      str->content->ಚಿಹ್ನೆ = str;
      str->is_alias = true;
      str->alias = sym;
      sym->alias = str;
    }
}


/*-------------------------------------------------------------------.
| Assign a symbol number, and write the definition of the token name |
| into FDEFINES.  Put in SYMBOLS.                                    |
`-------------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
ಚಿಹ್ನೆ_pack (ಚಿಹ್ನೆ *sym)
{
  aver (sym->content->ಸಂಖ್ಯೆ != ಸಂಖ್ಯೆ_UNDEFINED);
  ಯದಿ (sym->content->class == nterm_sym)
    sym->content->ಸಂಖ್ಯೆ += ntokens;

  symbols[sym->content->ಸಂಖ್ಯೆ] = sym->content->ಚಿಹ್ನೆ;
}

ಸ್ಥಿರ ಶೂನ್ಯ
complain_code_redeclared (ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ, ಧ್ರುವ ಚಿಹ್ನೆ *first, ಧ್ರುವ ಚಿಹ್ನೆ *second)
{
  symbols_sort (&first, &second);
  complain (&second->location, complaint,
            _("code %d reassigned to token %s"),
            ಅಂಕೆ, second->tag);
  subcomplain (&first->location, complaint,
               _("previous declaration for %s"),
               first->tag);
}

/*-------------------------------------------------.
| Put SYM in TOKEN_TRANSLATIONS if it is a token.  |
`-------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
ಚಿಹ್ನೆ_translation (ಧ್ರುವ ಚಿಹ್ನೆ *sym)
{
  ಯದಿ (sym->content->class == token_sym && !sym->is_alias)
    {
      /* A token whose translation has already been set? */
      ಯದಿ (token_translations[sym->content->code]
          != undeftoken->content->ಸಂಖ್ಯೆ)
        complain_code_redeclared
          (sym->content->code,
           symbols[token_translations[sym->content->code]], sym);
      ಅನ್ಯಥಾ
        token_translations[sym->content->code]
          = sym->content->ಸಂಖ್ಯೆ;
    }
}


/*---------------------------------------.
| Symbol and semantic type hash tables.  |
`---------------------------------------*/

/* Initial capacity of symbol and semantic type hash table.  */
#define HT_INITIAL_CAPACITY 257

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
hash_compare_ಚಿಹ್ನೆ (ಧ್ರುವ ಚಿಹ್ನೆ *m1, ಧ್ರುವ ಚಿಹ್ನೆ *m2)
{
  /* Since tags are unique, we can compare the pointers themselves.  */
  ಪ್ರತ್ಯಾಗಮ UNIQSTR_EQ (m1->tag, m2->tag);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
hash_compare_semantic_type (ಧ್ರುವ semantic_type *m1, ಧ್ರುವ semantic_type *m2)
{
  /* Since names are unique, we can compare the pointers themselves.  */
  ಪ್ರತ್ಯಾಗಮ UNIQSTR_EQ (m1->tag, m2->tag);
}

ಸ್ಥಿರ bool
hash_ಚಿಹ್ನೆ_comparator (ಶೂನ್ಯ ಧ್ರುವ *m1, ಶೂನ್ಯ ಧ್ರುವ *m2)
{
  ಪ್ರತ್ಯಾಗಮ hash_compare_ಚಿಹ್ನೆ (m1, m2);
}

ಸ್ಥಿರ bool
hash_semantic_type_comparator (ಶೂನ್ಯ ಧ್ರುವ *m1, ಶೂನ್ಯ ಧ್ರುವ *m2)
{
  ಪ್ರತ್ಯಾಗಮ hash_compare_semantic_type (m1, m2);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ size_t
hash_ಚಿಹ್ನೆ (ಧ್ರುವ ಚಿಹ್ನೆ *m, size_t tablesize)
{
  /* Since tags are unique, we can hash the pointer itself.  */
  ಪ್ರತ್ಯಾಗಮ ((uintptr_t) m->tag) % tablesize;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ size_t
hash_semantic_type (ಧ್ರುವ semantic_type *m, size_t tablesize)
{
  /* Since names are unique, we can hash the pointer itself.  */
  ಪ್ರತ್ಯಾಗಮ ((uintptr_t) m->tag) % tablesize;
}

ಸ್ಥಿರ size_t
hash_ಚಿಹ್ನೆ_hasher (ಶೂನ್ಯ ಧ್ರುವ *m, size_t tablesize)
{
  ಪ್ರತ್ಯಾಗಮ hash_ಚಿಹ್ನೆ (m, tablesize);
}

ಸ್ಥಿರ size_t
hash_semantic_type_hasher (ಶೂನ್ಯ ಧ್ರುವ *m, size_t tablesize)
{
  ಪ್ರತ್ಯಾಗಮ hash_semantic_type (m, tablesize);
}

/*-------------------------------.
| Create the symbol hash table.  |
`-------------------------------*/

ಶೂನ್ಯ
symbols_new (ಶೂನ್ಯ)
{
  ಚಿಹ್ನೆ_table = hash_xinitialize (HT_INITIAL_CAPACITY,
                                   ರಿಕ್ತ,
                                   hash_ಚಿಹ್ನೆ_hasher,
                                   hash_ಚಿಹ್ನೆ_comparator,
                                   ಚಿಹ್ನೆ_free);

  /* Construct the acceptsymbol symbol. */
  acceptsymbol = ಚಿಹ್ನೆ_get ("$accept", empty_loc);
  acceptsymbol->content->class = nterm_sym;
  acceptsymbol->content->ಸಂಖ್ಯೆ = nnterms++;

  /* Construct the YYerror/"error" token */
  errtoken = ಚಿಹ್ನೆ_get ("YYerror", empty_loc);
  errtoken->content->class = token_sym;
  errtoken->content->ಸಂಖ್ಯೆ = ntokens++;
  {
    ಚಿಹ್ನೆ *alias = ಚಿಹ್ನೆ_get ("error", empty_loc);
    ಚಿಹ್ನೆ_class_set (alias, token_sym, empty_loc, false);
    ಚಿಹ್ನೆ_make_alias (errtoken, alias, empty_loc);
  }

  /* Construct the YYUNDEF/"$undefined" token that represents all
     undefined literal tokens.  It is always symbol number 2.  */
  undeftoken = ಚಿಹ್ನೆ_get ("YYUNDEF", empty_loc);
  undeftoken->content->class = token_sym;
  undeftoken->content->ಸಂಖ್ಯೆ = ntokens++;
  {
    ಚಿಹ್ನೆ *alias = ಚಿಹ್ನೆ_get ("$undefined", empty_loc);
    ಚಿಹ್ನೆ_class_set (alias, token_sym, empty_loc, false);
    ಚಿಹ್ನೆ_make_alias (undeftoken, alias, empty_loc);
  }

  semantic_type_table = hash_xinitialize (HT_INITIAL_CAPACITY,
                                          ರಿಕ್ತ,
                                          hash_semantic_type_hasher,
                                          hash_semantic_type_comparator,
                                          ಮುಕ್ತ);
}


/*----------------------------------------------------------------.
| Find the symbol named KEY, and return it.  If it does not exist |
| yet, create it.                                                 |
`----------------------------------------------------------------*/

ಚಿಹ್ನೆ *
ಚಿಹ್ನೆ_from_uniqstr (ಧ್ರುವ uniqstr ಕೀಲಿ, location loc)
{
  ಚಿಹ್ನೆ probe;

  probe.tag = ಕೀಲಿ;
  ಚಿಹ್ನೆ *res = hash_lookup (ಚಿಹ್ನೆ_table, &probe);

  ಯದಿ (!res)
    {
      /* First insertion in the hash. */
      aver (!symbols_sorted);
      res = ಚಿಹ್ನೆ_new (ಕೀಲಿ, loc);
      hash_xinsert (ಚಿಹ್ನೆ_table, res);
    }
  ಪ್ರತ್ಯಾಗಮ res;
}


/*-----------------------------------------------------------------------.
| Find the semantic type named KEY, and return it.  If it does not exist |
| yet, create it.                                                        |
`-----------------------------------------------------------------------*/

semantic_type *
semantic_type_from_uniqstr (ಧ್ರುವ uniqstr ಕೀಲಿ, ಧ್ರುವ location *loc)
{
  semantic_type probe;

  probe.tag = ಕೀಲಿ;
  semantic_type *res = hash_lookup (semantic_type_table, &probe);

  ಯದಿ (!res)
    {
      /* First insertion in the hash. */
      res = semantic_type_new (ಕೀಲಿ, loc);
      hash_xinsert (semantic_type_table, res);
    }
  ಪ್ರತ್ಯಾಗಮ res;
}


/*----------------------------------------------------------------.
| Find the symbol named KEY, and return it.  If it does not exist |
| yet, create it.                                                 |
`----------------------------------------------------------------*/

ಚಿಹ್ನೆ *
ಚಿಹ್ನೆ_get (ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ, location loc)
{
  ಪ್ರತ್ಯಾಗಮ ಚಿಹ್ನೆ_from_uniqstr (uniqstr_new (ಕೀಲಿ), loc);
}


/*-----------------------------------------------------------------------.
| Find the semantic type named KEY, and return it.  If it does not exist |
| yet, create it.                                                        |
`-----------------------------------------------------------------------*/

semantic_type *
semantic_type_get (ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ, ಧ್ರುವ location *loc)
{
  ಪ್ರತ್ಯಾಗಮ semantic_type_from_uniqstr (uniqstr_new (ಕೀಲಿ), loc);
}


/*------------------------------------------------------------------.
| Generate a dummy nonterminal, whose name cannot conflict with the |
| user's names.                                                     |
`------------------------------------------------------------------*/

ಚಿಹ್ನೆ *
dummy_ಚಿಹ್ನೆ_get (location loc)
{
  /* Incremented for each generated symbol.  */
  ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ dummy_ಎಣಿಕೆ = 0;
  ವರ್ಣ buf[32];
  ಪೂರ್ಣಾಙ್ಕ len = snprintf (buf, ಪರಿಮಾಣ buf, "$@%d", ++dummy_ಎಣಿಕೆ);
  assure (len < ಪರಿಮಾಣ buf);
  ಚಿಹ್ನೆ *sym = ಚಿಹ್ನೆ_get (buf, loc);
  sym->content->class = nterm_sym;
  ಪ್ರತ್ಯಾಗಮ sym;
}

bool
ಚಿಹ್ನೆ_is_dummy (ಚಿಹ್ನೆ ಧ್ರುವ *sym)
{
  ಪ್ರತ್ಯಾಗಮ sym->tag[0] == '@' || (sym->tag[0] == '$' && sym->tag[1] == '@');
}

/*-------------------.
| Free the symbols.  |
`-------------------*/

ಶೂನ್ಯ
symbols_free (ಶೂನ್ಯ)
{
  hash_free (ಚಿಹ್ನೆ_table);
  hash_free (semantic_type_table);
  ಮುಕ್ತ (symbols);
  ಮುಕ್ತ (symbols_sorted);
  ಮುಕ್ತ (semantic_types_sorted);
}


ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
ಚಿಹ್ನೆ_cmp (ಶೂನ್ಯ ಧ್ರುವ *a, ಶೂನ್ಯ ಧ್ರುವ *b)
{
  ಪ್ರತ್ಯಾಗಮ location_cmp ((*(ಚಿಹ್ನೆ * ಧ್ರುವ *)a)->location,
                       (*(ಚಿಹ್ನೆ * ಧ್ರುವ *)b)->location);
}

/* Store in *SORTED an array of pointers to the symbols contained in
   TABLE, sorted by order of appearance (i.e., by location). */

ಸ್ಥಿರ ಶೂನ್ಯ
table_sort (ಸಂರಚನಾ hash_table *table, ಚಿಹ್ನೆ ***sorted)
{
  aver (!*sorted);
  size_t ಎಣಿಕೆ = hash_get_n_entries (table);
  *sorted = xnmalloc (ಎಣಿಕೆ + 1, ಪರಿಮಾಣ **sorted);
  hash_get_entries (table, (ಶೂನ್ಯ**)*sorted, ಎಣಿಕೆ);
  qsort (*sorted, ಎಣಿಕೆ, ಪರಿಮಾಣ **sorted, ಚಿಹ್ನೆ_cmp);
  (*sorted)[ಎಣಿಕೆ] = ರಿಕ್ತ;
}

/*--------------------------------------------------------------.
| Check that all the symbols are defined.  Report any undefined |
| symbols and consider them nonterminals.                       |
`--------------------------------------------------------------*/

ಶೂನ್ಯ
symbols_check_defined (ಶೂನ್ಯ)
{
  table_sort (ಚಿಹ್ನೆ_table, &symbols_sorted);
  /* semantic_type, like symbol, starts with a 'tag' field and then a
     'location' field.  And here we only deal with arrays/hashes of
     pointers, sizeof is not an issue.

     So instead of implementing table_sort (and symbol_cmp) once for
     each type, let's lie a bit to the typing system, and treat
     'semantic_type' as if it were 'symbol'. */
  table_sort (semantic_type_table, (ಚಿಹ್ನೆ ***) &semantic_types_sorted);

  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; symbols_sorted[i]; ++i)
    ಚಿಹ್ನೆ_check_defined (symbols_sorted[i]);
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; semantic_types_sorted[i]; ++i)
    semantic_type_check_defined (semantic_types_sorted[i]);
}

/*------------------------------------------------------------------.
| Set TOKEN_TRANSLATIONS.  Check that no two symbols share the same |
| number.                                                           |
`------------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
symbols_token_translations_init (ಶೂನ್ಯ)
{
  bool code_256_available_p = true;

  /* Find the highest token code, and whether 256, the POSIX preferred
     token code for the error token, is used.  */
  max_code = 0;
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ntokens; ++i)
    {
      sym_content *sym = symbols[i]->content;
      ಯದಿ (sym->code != CODE_UNDEFINED)
        {
          ಯದಿ (sym->code > max_code)
            max_code = sym->code;
          ಯದಿ (sym->code == 256)
            code_256_available_p = false;
        }
    }

  /* If 256 is not used, assign it to error, to follow POSIX.  */
  ಯದಿ (code_256_available_p
      && errtoken->content->code == CODE_UNDEFINED)
    errtoken->content->code = 256;

  /* Set the missing codes. */
  ಯದಿ (max_code < 256)
    max_code = 256;

  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ntokens; ++i)
    {
      sym_content *sym = symbols[i]->content;
      ಯದಿ (sym->code == CODE_UNDEFINED)
        {
          IGNORE_TYPE_LIMITS_BEGIN
          ಯದಿ (INT_ADD_WRAPV (max_code, 1, &max_code))
            complain (ರಿಕ್ತ, fatal, _("token number too large"));
          IGNORE_TYPE_LIMITS_END
          sym->code = max_code;
        }
      ಯದಿ (sym->code > max_code)
        max_code = sym->code;
    }

  token_translations = xnmalloc (max_code + 1,
                                 ಪರಿಮಾಣ *token_translations);

  /* Initialize all entries for literal tokens to the internal token
     number for $undefined, which represents all invalid inputs.  */
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < max_code + 1; ++i)
    token_translations[i] = undeftoken->content->ಸಂಖ್ಯೆ;
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; symbols_sorted[i]; ++i)
    ಚಿಹ್ನೆ_translation (symbols_sorted[i]);
}


/* Whether some symbol requires internationalization.  */
ಸ್ಥಿರ bool
has_translations (ಶೂನ್ಯ)
{
  ಪ್ರತಿ (ಧ್ರುವ ಶೂನ್ಯ *entry = hash_get_first (ಚಿಹ್ನೆ_table);
       entry;
       entry = hash_get_next (ಚಿಹ್ನೆ_table, entry))
    {
      ಧ್ರುವ ಚಿಹ್ನೆ *sym = (ಧ್ರುವ ಚಿಹ್ನೆ *) entry;
      ಯದಿ (sym->translatable)
        ಪ್ರತ್ಯಾಗಮ true;
    }
  ಪ್ರತ್ಯಾಗಮ false;
}

/*----------------------------------------------------------------.
| Assign symbol numbers, and write definition of token names into |
| FDEFINES.  Set up vectors SYMBOL_TABLE, TAGS of symbols.        |
`----------------------------------------------------------------*/

ಶೂನ್ಯ
symbols_pack (ಶೂನ್ಯ)
{
  symbols = xcalloc (nsyms, ಪರಿಮಾಣ *symbols);
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; symbols_sorted[i]; ++i)
    ಚಿಹ್ನೆ_pack (symbols_sorted[i]);

  /* Aliases leave empty slots in symbols, so remove them.  */
  {
    ಪೂರ್ಣಾಙ್ಕ nsyms_old = nsyms;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ writei = 0, readi = 0; readi < nsyms_old; readi += 1)
      {
        ಯದಿ (symbols[readi] == ರಿಕ್ತ)
          {
            nsyms -= 1;
            ntokens -= 1;
          }
        ಅನ್ಯಥಾ
          {
            symbols[writei] = symbols[readi];
            symbols[writei]->content->ಸಂಖ್ಯೆ = writei;
            writei += 1;
          }
      }
  }
  symbols = xnrealloc (symbols, nsyms, ಪರಿಮಾಣ *symbols);

  symbols_token_translations_init ();

  // If some user tokens are internationalized, the internal ones
  // should be too.
  ಯದಿ (has_translations ())
    {
      ಧ್ರುವ bool eof_is_user_defined
        = !eoftoken->alias || STRNEQ (eoftoken->alias->tag, "$end");
      ಯದಿ (!eof_is_user_defined)
        eoftoken->alias->translatable = true;
      undeftoken->alias->translatable = true;
      errtoken->alias->translatable = true;
    }
}

/*---------------------------------.
| Initialize relation graph nodes. |
`---------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
init_prec_nodes (ಶೂನ್ಯ)
{
  prec_nodes = xcalloc (nsyms, ಪರಿಮಾಣ *prec_nodes);
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < nsyms; ++i)
    {
      prec_nodes[i] = xmalloc (ಪರಿಮಾಣ *prec_nodes[i]);
      symgraph *s = prec_nodes[i];
      s->id = i;
      s->succ = 0;
      s->pred = 0;
    }
}

/*----------------.
| Create a link.  |
`----------------*/

ಸ್ಥಿರ symgraphlink *
symgraphlink_new (graphid id, symgraphlink *next)
{
  symgraphlink *res = xmalloc (ಪರಿಮಾಣ *res);
  res->id = id;
  res->next = next;
  ಪ್ರತ್ಯಾಗಮ res;
}


/*------------------------------------------------------------------.
| Register the second symbol of the precedence relation, and return |
| whether this relation is new.  Use only in register_precedence.   |
`------------------------------------------------------------------*/

ಸ್ಥಿರ bool
register_precedence_second_ಚಿಹ್ನೆ (symgraphlink **first, graphid sym)
{
  ಯದಿ (!*first || sym < (*first)->id)
    *first = symgraphlink_new (sym, *first);
  ಅನ್ಯಥಾ
    {
      symgraphlink *slist = *first;

      ಯಾವತ್ (slist->next && slist->next->id <= sym)
        slist = slist->next;

      ಯದಿ (slist->id == sym)
        /* Relation already present. */
        ಪ್ರತ್ಯಾಗಮ false;

      slist->next = symgraphlink_new (sym, slist->next);
    }
  ಪ್ರತ್ಯಾಗಮ true;
}

/*------------------------------------------------------------------.
| Register a new relation between symbols as used. The first symbol |
| has a greater precedence than the second one.                     |
`------------------------------------------------------------------*/

ಶೂನ್ಯ
register_precedence (graphid first, graphid snd)
{
  ಯದಿ (!prec_nodes)
    init_prec_nodes ();
  register_precedence_second_ಚಿಹ್ನೆ (&(prec_nodes[first]->succ), snd);
  register_precedence_second_ಚಿಹ್ನೆ (&(prec_nodes[snd]->pred), first);
}


/*---------------------------------------.
| Deep clear a linked / adjacency list). |
`---------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
linkedlist_free (symgraphlink *ಗಂಟು)
{
  ಯದಿ (ಗಂಟು)
    {
      ಯಾವತ್ (ಗಂಟು->next)
        {
          symgraphlink *tmp = ಗಂಟು->next;
          ಮುಕ್ತ (ಗಂಟು);
          ಗಂಟು = tmp;
        }
      ಮುಕ್ತ (ಗಂಟು);
    }
}

/*----------------------------------------------.
| Clear and destroy association tracking table. |
`----------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
assoc_free (ಶೂನ್ಯ)
{
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < nsyms; ++i)
    {
      linkedlist_free (prec_nodes[i]->pred);
      linkedlist_free (prec_nodes[i]->succ);
      ಮುಕ್ತ (prec_nodes[i]);
    }
  ಮುಕ್ತ (prec_nodes);
}

/*---------------------------------------.
| Initialize association tracking table. |
`---------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
init_assoc (ಶೂನ್ಯ)
{
  used_assoc = xcalloc (nsyms, ಪರಿಮಾಣ *used_assoc);
  ಪ್ರತಿ (graphid i = 0; i < nsyms; ++i)
    used_assoc[i] = false;
}

/*------------------------------------------------------------------.
| Test if the associativity for the symbols is defined and useless. |
`------------------------------------------------------------------*/

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
is_assoc_useless (ಚಿಹ್ನೆ *s)
{
  ಪ್ರತ್ಯಾಗಮ s
      && s->content->assoc != undef_assoc
      && s->content->assoc != precedence_assoc
      && !used_assoc[s->content->ಸಂಖ್ಯೆ];
}

/*-------------------------------.
| Register a used associativity. |
`-------------------------------*/

ಶೂನ್ಯ
register_assoc (graphid i, graphid j)
{
  ಯದಿ (!used_assoc)
    init_assoc ();
  used_assoc[i] = true;
  used_assoc[j] = true;
}

/*--------------------------------------------------.
| Print a warning for unused precedence relations.  |
`--------------------------------------------------*/

ಶೂನ್ಯ
print_precedence_warnings (ಶೂನ್ಯ)
{
  ಯದಿ (!prec_nodes)
    init_prec_nodes ();
  ಯದಿ (!used_assoc)
    init_assoc ();
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < nsyms; ++i)
    {
      ಚಿಹ್ನೆ *s = symbols[i];
      ಯದಿ (s
          && s->content->prec != 0
          && !prec_nodes[i]->pred
          && !prec_nodes[i]->succ)
        {
          ಯದಿ (is_assoc_useless (s))
            complain (&s->content->prec_loc, Wprecedence,
                      _("useless precedence and associativity for %s"), s->tag);
          ಅನ್ಯಥಾ ಯದಿ (s->content->assoc == precedence_assoc)
            complain (&s->content->prec_loc, Wprecedence,
                      _("useless precedence for %s"), s->tag);
        }
      ಅನ್ಯಥಾ ಯದಿ (is_assoc_useless (s))
        complain (&s->content->prec_loc, Wprecedence,
                  _("useless associativity for %s, use %%precedence"), s->tag);
    }
  ಮುಕ್ತ (used_assoc);
  assoc_free ();
}
