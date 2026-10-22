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
#ವ್ಯಾಖ್ಯಾನಿಸು ಸಂಕೇತ_UNDEFINED (-1)

/* Undefined symbol number.  */
#ವ್ಯಾಖ್ಯಾನಿಸು ಸಂಖ್ಯೆ_UNDEFINED (-1)


ಸ್ಥಿರ ರಚನೆ hash_ಕೋಷ್ಟಕ *ಚಿಹ್ನೆ_ಕೋಷ್ಟಕ = ಖಾಲಿ;
ಸ್ಥಿರ ರಚನೆ hash_ಕೋಷ್ಟಕ *semantic_type_ಕೋಷ್ಟಕ = ಖಾಲಿ;

/*----------------------------------------------------------------.
| Symbols sorted by tag.  Allocated by table_sort, after which no |
| more symbols should be created.                                 |
`----------------------------------------------------------------*/

ಸ್ಥಿರ ಚಿಹ್ನೆ **symbols_sorted = ಖಾಲಿ;
ಸ್ಥಿರ semantic_type **semantic_types_sorted = ಖಾಲಿ;


/*------------------------.
| Distinguished symbols.  |
`------------------------*/

ಚಿಹ್ನೆ *errtoken = ಖಾಲಿ;
ಚಿಹ್ನೆ *undeftoken = ಖಾಲಿ;
ಚಿಹ್ನೆ *eoftoken = ಖಾಲಿ;
ಚಿಹ್ನೆ *acceptsymbol = ಖಾಲಿ;

/* Precedence relation graph. */
ಸ್ಥಿರ symgraph **prec_nodes;

/* Store which associativity is used.  */
ಸ್ಥಿರ bool *ಬಳಸಿದ_assoc = ಖಾಲಿ;

bool ಟ್ಯಾಗ್_ನೋಡಿದ = false;


/* Whether SYM was defined by the user.  */

ಸ್ಥಿರ bool
ಚಿಹ್ನೆ_is_user_defined (ಚಿಹ್ನೆ *sym)
{
  ಸ್ಥಿರಾಂಕ bool eof_is_user_defined
    = !eoftoken->alias || STRNEQ (eoftoken->alias->ಟ್ಯಾಗ್, "$end");
  ಹಿಂತಿರುಗಿ sym->ಟ್ಯಾಗ್[0] != '$'
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
  sym_content *res = xmalloc (ಗಾತ್ರ *res);

  res->ಚಿಹ್ನೆ = s;

  res->type_name = ಖಾಲಿ;
  res->type_loc = ಖಾಲಿಗಳು_loc;
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಸಂಕೇತ_PROPS_ಗಾತ್ರ; ++i)
    ಸಂಕೇತ_props_none_init (&res->props[i]);

  res->ಸಂಖ್ಯೆ = ಸಂಖ್ಯೆ_UNDEFINED;
  res->prec_loc = ಖಾಲಿಗಳು_loc;
  res->prec = 0;
  res->assoc = undef_assoc;
  res->ಸಂಕೇತ_2 = ಸಂಕೇತ_UNDEFINED;

  res->class = unknown_sym;
  res->ಸ್ಥಿತಿ_2 = undeclared;

  ಹಿಂತಿರುಗಿ res;
}

/*---------------------------------.
| Create a new symbol, named TAG.  |
`---------------------------------*/

ಸ್ಥಿರ ಚಿಹ್ನೆ *
ಚಿಹ್ನೆ_new (uniqstr ಟ್ಯಾಗ್, location loc)
{
  ಚಿಹ್ನೆ *res = xmalloc (ಗಾತ್ರ *res);
  uniqstr_assert (ಟ್ಯಾಗ್);

  /* If the tag is not a string (starts with a double quote), check
     that it is valid for Yacc. */
  ಆದರೆ (ಟ್ಯಾಗ್[0] != '\"' && ಟ್ಯಾಗ್[0] != '\'' && strchr (ಟ್ಯಾಗ್, '-'))
    complain (&loc, Wyacc,
              _("POSIX Yacc forbids dashes in symbol names: %s"), ಟ್ಯಾಗ್);

  res->ಟ್ಯಾಗ್ = ಟ್ಯಾಗ್;
  res->location = loc;
  res->translatable = false;
  res->location_ದ_lhs = false;
  res->alias = ಖಾಲಿ;
  res->content = sym_content_new (res);
  res->is_alias = false;
  ಹಿಂತಿರುಗಿ res;
}

/*--------------------.
| Free a sym_content. |
`--------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
sym_content_free (sym_content *sym)
{
  ಬಿಡುಗಡೆ (sym);
}


/*---------------------------------------------------------.
| Free a symbol and its associated content if appropriate. |
`---------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
ಚಿಹ್ನೆ_free (ಶೂನ್ಯ *ptr)
{
  ಚಿಹ್ನೆ *sym = (ಚಿಹ್ನೆ *)ptr;
  ಆದರೆ (!sym->is_alias)
    sym_content_free (sym->content);
  ಬಿಡುಗಡೆ (sym);

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
symbols_ವಿಂಗಡಿಸು (ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆ **ಮೊದಲ, ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆ **ಎರಡನೇ)
{
  ಆದರೆ (0 < location_cmp ((*ಮೊದಲ)->location, (*ಎರಡನೇ)->location))
    {
      ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆ* tmp = *ಮೊದಲ;
      *ಮೊದಲ = *ಎರಡನೇ;
      *ಎರಡನೇ = tmp;
    }
}

/* Likewise, for locations.  */

ಸ್ಥಿರ ಶೂನ್ಯ
locations_ವಿಂಗಡಿಸು (location *ಮೊದಲ, location *ಎರಡನೇ)
{
  ಆದರೆ (0 < location_cmp (*ಮೊದಲ, *ಎರಡನೇ))
    {
      location tmp = *ಮೊದಲ;
      *ಮೊದಲ = *ಎರಡನೇ;
      *ಎರಡನೇ = tmp;
    }
}

ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *
ಸಂಕೇತ_props_type_string (ಸಂಕೇತ_props_type ಬಗೆ)
{
  ಆಯ್ಕೆ (ಬಗೆ)
    {
    ಸ್ಥಿತಿ destructor:
      ಹಿಂತಿರುಗಿ "%destructor";
    ಸ್ಥಿತಿ printer:
      ಹಿಂತಿರುಗಿ "%printer";
    }
  abort ();
}


/*----------------------------------------.
| Create a new semantic type, named TAG.  |
`----------------------------------------*/

ಸ್ಥಿರ semantic_type *
semantic_type_new (uniqstr ಟ್ಯಾಗ್, ಸ್ಥಿರಾಂಕ location *loc)
{
  semantic_type *res = xmalloc (ಗಾತ್ರ *res);

  uniqstr_assert (ಟ್ಯಾಗ್);
  res->ಟ್ಯಾಗ್ = ಟ್ಯಾಗ್;
  res->location = loc ? *loc : ಖಾಲಿಗಳು_loc;
  res->ಸ್ಥಿತಿ_2 = undeclared;
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಸಂಕೇತ_PROPS_ಗಾತ್ರ; ++i)
    ಸಂಕೇತ_props_none_init (&res->props[i]);

  ಹಿಂತಿರುಗಿ res;
}


/*-----------------.
| Print a symbol.  |
`-----------------*/

#ವ್ಯಾಖ್ಯಾನಿಸು ಚಿಹ್ನೆ_INT_ATTR_PRINT(Attr)                     \
  ಆದರೆ (s->content)                                       \
    fprintf (f, " %s = %d", #Attr, s->content->Attr)

#ವ್ಯಾಖ್ಯಾನಿಸು ಚಿಹ್ನೆ_STR_ATTR_PRINT(Attr)                     \
  ಆದರೆ (s->content && s->content->Attr)                   \
    fprintf (f, " %s { %s }", #Attr, s->content->Attr)

#ವ್ಯಾಖ್ಯಾನಿಸು ಚಿಹ್ನೆ_ಸಂಕೇತ_PRINT(Attr)                                         \
  ಆದರೆ (s->content && s->content->props[Attr].ಸಂಕೇತ_2)                       \
    fprintf (f, " %s { %s }", #Attr, s->content->props[Attr].ಸಂಕೇತ_2)

ಶೂನ್ಯ
ಚಿಹ್ನೆ_print (ಚಿಹ್ನೆ ಸ್ಥಿರಾಂಕ *s, FILE *f)
{
  ಆದರೆ (s)
    {
      ಚಿಹ್ನೆ_class c = s->content->class;
      fprintf (f, "%s: %s",
               c == unknown_sym    ? "unknown"
               : c == pct_type_sym ? "%type"
               : c == token_sym    ? "token"
               : c == nterm_sym    ? "nterm"
               : ಖಾಲಿ, /* abort.  */
               s->ಟ್ಯಾಗ್);
      putc (' ', f);
      location_print (s->location, f);
      ಚಿಹ್ನೆ_INT_ATTR_PRINT (ಸಂಕೇತ_2);
      ಚಿಹ್ನೆ_INT_ATTR_PRINT (ಸಂಖ್ಯೆ);
      ಚಿಹ್ನೆ_STR_ATTR_PRINT (type_name);
      ಚಿಹ್ನೆ_ಸಂಕೇತ_PRINT (destructor);
      ಚಿಹ್ನೆ_ಸಂಕೇತ_PRINT (printer);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    fputs ("<NULL>", f);
}

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು ಚಿಹ್ನೆ_ATTR_PRINT
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು ಚಿಹ್ನೆ_ಸಂಕೇತ_PRINT


/*----------------------------------.
| Whether S is a valid identifier.  |
`----------------------------------*/

ಸ್ಥಿರ bool
is_identifier (uniqstr s)
{
  ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ alphanum[26 + 26 + 1 + 10] =
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "_"
    "0123456789";
  ಆದರೆ (!s || ! memchr (alphanum, *s, ಗಾತ್ರ alphanum - 10))
    ಹಿಂತಿರುಗಿ false;
  ಗಾಗಿ (++s; *s; ++s)
    ಆದರೆ (! memchr (alphanum, *s, ಗಾತ್ರ alphanum))
      ಹಿಂತಿರುಗಿ false;
  ಹಿಂತಿರುಗಿ true;
}


/*-----------------------------------------------.
| Get the identifier associated to this symbol.  |
`-----------------------------------------------*/

uniqstr
ಚಿಹ್ನೆ_id_get (ಚಿಹ್ನೆ ಸ್ಥಿರಾಂಕ *sym)
{
  // There's one weird case: YYerror is the alias, and error is the
  // base symbol.  Return YYerror in that case.
  ಆದರೆ (sym->alias && is_identifier (sym->alias->ಟ್ಯಾಗ್))
    ಹಿಂತಿರುಗಿ sym->alias->ಟ್ಯಾಗ್;
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (is_identifier (sym->ಟ್ಯಾಗ್))
    ಹಿಂತಿರುಗಿ sym->ಟ್ಯಾಗ್;
  ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}


/*------------------------------------------------------------------.
| Complain that S's WHAT is redeclared at SECOND, and was first set |
| at FIRST.                                                         |
`------------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
complain_ಚಿಹ್ನೆ_redeclared (ಚಿಹ್ನೆ *s, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *what, location ಮೊದಲ,
                            location ಎರಡನೇ)
{
  locations_ವಿಂಗಡಿಸು (&ಮೊದಲ, &ಎರಡನೇ);
  complain (&ಎರಡನೇ, complaint, _("%s redeclaration for %s"), what, s->ಟ್ಯಾಗ್);
  subcomplain (&ಮೊದಲ, complaint, _("previous declaration"));
}

ಸ್ಥಿರ ಶೂನ್ಯ
complain_semantic_type_redeclared (semantic_type *s, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *what, location ಮೊದಲ,
                                   location ಎರಡನೇ)
{
  locations_ವಿಂಗಡಿಸು (&ಮೊದಲ, &ಎರಡನೇ);
  complain (&ಎರಡನೇ, complaint, _("%s redeclaration for <%s>"), what, s->ಟ್ಯಾಗ್);
  subcomplain (&ಮೊದಲ, complaint, _("previous declaration"));
}

ಸ್ಥಿರ ಶೂನ್ಯ
complain_class_redeclared (ಚಿಹ್ನೆ *sym, ಚಿಹ್ನೆ_class class, location ಎರಡನೇ)
{
  complain (&ಎರಡನೇ, complaint,
            class == token_sym
            ? _("symbol %s redeclared as a token")
            : _("symbol %s redeclared as a nonterminal"), sym->ಟ್ಯಾಗ್);
  ಆದರೆ (!location_ಖಾಲಿಗಳು (sym->location))
    subcomplain (&sym->location, complaint, _("previous definition"));
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆ *
ಚಿಹ್ನೆ_from_uniqstr_fuzzy (ಸ್ಥಿರಾಂಕ uniqstr ಕೀಲಿ)
{
  aver (symbols_sorted);
#ವ್ಯಾಖ್ಯಾನಿಸು FSTRCMP_ಮಿತಿ 0.6
  ದ್ವಿಗುಣ ಉತ್ತಮ_similarity = FSTRCMP_ಮಿತಿ;
  ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆ *res = ಖಾಲಿ;
  size_t ಎಣಿಕೆ_2 = hash_get_n_entries (ಚಿಹ್ನೆ_ಕೋಷ್ಟಕ);
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಎಣಿಕೆ_2; ++i)
    {
      ಚಿಹ್ನೆ *sym = symbols_sorted[i];
      ಆದರೆ (STRNEQ (ಕೀಲಿ, sym->ಟ್ಯಾಗ್)
          && (sym->content->ಸ್ಥಿತಿ_2 == declared
              || sym->content->ಸ್ಥಿತಿ_2 == undeclared))
        {
          ದ್ವಿಗುಣ similarity = fstrcmp_bounded (ಕೀಲಿ, sym->ಟ್ಯಾಗ್, ಉತ್ತಮ_similarity);
          ಆದರೆ (ಉತ್ತಮ_similarity < similarity)
            {
              res = sym;
              ಉತ್ತಮ_similarity = similarity;
            }
        }
    }
  ಹಿಂತಿರುಗಿ res;
}

ಸ್ಥಿರ ಶೂನ್ಯ
complain_ಚಿಹ್ನೆ_undeclared (ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆ *sym)
{
  ಪ್ರತಿಜ್ಞಾ (sym->content->ಸ್ಥಿತಿ_2 != declared);
  ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆ *ಉತ್ತಮ = ಚಿಹ್ನೆ_from_uniqstr_fuzzy (sym->ಟ್ಯಾಗ್);
  ಆದರೆ (ಉತ್ತಮ)
    {
      complain (&sym->location,
                sym->content->ಸ್ಥಿತಿ_2 == needed ? complaint : Wother,
                _("symbol %s is used, but is not defined as a token"
                  " and has no rules; did you mean %s?"),
                quote_n (0, sym->ಟ್ಯಾಗ್),
                quote_n (1, ಉತ್ತಮ->ಟ್ಯಾಗ್));
      ಆದರೆ (feature_flag & feature_caret)
        location_caret_suggestion (sym->location, ಉತ್ತಮ->ಟ್ಯಾಗ್, stderr);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    complain (&sym->location,
              sym->content->ಸ್ಥಿತಿ_2 == needed ? complaint : Wother,
              _("symbol %s is used, but is not defined as a token"
                " and has no rules"),
              quote (sym->ಟ್ಯಾಗ್));
}

ಶೂನ್ಯ
ಚಿಹ್ನೆ_location_as_lhs_ಗಣ (ಚಿಹ್ನೆ *sym, location loc)
{
  ಆದರೆ (!sym->location_ದ_lhs)
    {
      sym->location = loc;
      sym->location_ದ_lhs = true;
    }
}


/*-----------------------------------------------------------------.
| Set the TYPE_NAME associated with SYM.  Does nothing if passed 0 |
| as TYPE_NAME.                                                    |
`-----------------------------------------------------------------*/

ಶೂನ್ಯ
ಚಿಹ್ನೆ_type_ಗಣ (ಚಿಹ್ನೆ *sym, uniqstr type_name, location loc)
{
  ಆದರೆ (type_name)
    {
      ಟ್ಯಾಗ್_ನೋಡಿದ = true;
      ಆದರೆ (sym->content->type_name)
        complain_ಚಿಹ್ನೆ_redeclared (sym, "%type",
                                    sym->content->type_loc, loc);
      ಇಲ್ಲದಿದ್ದರೆ
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
ಚಿಹ್ನೆ_ಸಂಕೇತ_props_ಗಣ (ಚಿಹ್ನೆ *sym, ಸಂಕೇತ_props_type ಬಗೆ,
                       ಸಂಕೇತ_props ಸ್ಥಿರಾಂಕ *ಸಂಕೇತ_2)
{
  ಆದರೆ (sym->content->props[ಬಗೆ].ಸಂಕೇತ_2)
    complain_ಚಿಹ್ನೆ_redeclared (sym, ಸಂಕೇತ_props_type_string (ಬಗೆ),
                                sym->content->props[ಬಗೆ].location,
                                ಸಂಕೇತ_2->location);
  ಇಲ್ಲದಿದ್ದರೆ
    sym->content->props[ಬಗೆ] = *ಸಂಕೇತ_2;
}


/*-----------------------------------------------------.
| Set the DESTRUCTOR or PRINTER associated with TYPE.  |
`-----------------------------------------------------*/

ಶೂನ್ಯ
semantic_type_ಸಂಕೇತ_props_ಗಣ (semantic_type *type,
                              ಸಂಕೇತ_props_type ಬಗೆ,
                              ಸಂಕೇತ_props ಸ್ಥಿರಾಂಕ *ಸಂಕೇತ_2)
{
  ಆದರೆ (type->props[ಬಗೆ].ಸಂಕೇತ_2)
    complain_semantic_type_redeclared (type, ಸಂಕೇತ_props_type_string (ಬಗೆ),
                                       type->props[ಬಗೆ].location,
                                       ಸಂಕೇತ_2->location);
  ಇಲ್ಲದಿದ್ದರೆ
    type->props[ಬಗೆ] = *ಸಂಕೇತ_2;
}

/*---------------------------------------------------.
| Get the computed %destructor or %printer for SYM.  |
`---------------------------------------------------*/

ಸಂಕೇತ_props *
ಚಿಹ್ನೆ_ಸಂಕೇತ_props_get (ಚಿಹ್ನೆ *sym, ಸಂಕೇತ_props_type ಬಗೆ)
{
  /* Per-symbol code props.  */
  ಆದರೆ (sym->content->props[ಬಗೆ].ಸಂಕೇತ_2)
    ಹಿಂತಿರುಗಿ &sym->content->props[ಬಗೆ];

  /* Per-type code props.  */
  ಆದರೆ (sym->content->type_name)
    {
      ಸಂಕೇತ_props *ಸಂಕೇತ_2 =
        &semantic_type_get (sym->content->type_name, ಖಾಲಿ)->props[ಬಗೆ];
      ಆದರೆ (ಸಂಕೇತ_2->ಸಂಕೇತ_2)
        ಹಿಂತಿರುಗಿ ಸಂಕೇತ_2;
    }

  /* Apply default code props's only to user-defined symbols.  */
  ಆದರೆ (ಚಿಹ್ನೆ_is_user_defined (sym))
    {
      ಸಂಕೇತ_props *ಸಂಕೇತ_2 = &semantic_type_get (sym->content->type_name ? "*" : "",
                                             ಖಾಲಿ)->props[ಬಗೆ];
      ಆದರೆ (ಸಂಕೇತ_2->ಸಂಕೇತ_2)
        ಹಿಂತಿರುಗಿ ಸಂಕೇತ_2;
    }
  ಹಿಂತಿರುಗಿ &ಸಂಕೇತ_props_none;
}

/*-----------------------------------------------------------------.
| Set the PRECEDENCE associated with SYM.  Does nothing if invoked |
| with UNDEF_ASSOC as ASSOC.                                       |
`-----------------------------------------------------------------*/

ಶೂನ್ಯ
ಚಿಹ್ನೆ_precedence_ಗಣ (ಚಿಹ್ನೆ *sym, ಪೂರ್ಣಾಂಕ prec, assoc a, location loc)
{
  ಆದರೆ (a != undef_assoc)
    {
      sym_content *s = sym->content;
      ಆದರೆ (s->prec)
        complain_ಚಿಹ್ನೆ_redeclared (sym, assoc_ಗಮ್ಯ_string (a),
                                    s->prec_loc, loc);
      ಇಲ್ಲದಿದ್ದರೆ
        {
          s->prec = prec;
          s->assoc = a;
          s->prec_loc = loc;
        }
    }

  /* Only terminals have a precedence. */
  ಚಿಹ್ನೆ_class_ಗಣ (sym, token_sym, loc, false);
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
ಚಿಹ್ನೆ_class_ಗಣ (ಚಿಹ್ನೆ *sym, ಚಿಹ್ನೆ_class class, location loc, bool declaring)
{
  aver (class != unknown_sym);
  sym_content *s = sym->content;
  ಆದರೆ (class == pct_type_sym)
    {
      ಆದರೆ (s->class == token_sym)
        complain_pct_type_on_token (&loc);
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (s->class == unknown_sym)
        s->class = class;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (s->class != unknown_sym && s->class != pct_type_sym
           && s->class != class)
    complain_class_redeclared (sym, class, loc);
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಆದರೆ (class == token_sym && s->class == pct_type_sym)
        complain_pct_type_on_token (&sym->location);

      s->class = class;

      ಆದರೆ (declaring)
        {
          ಆದರೆ (s->ಸ್ಥಿತಿ_2 == declared)
            {
              complain (&loc, Wother,
                        _("symbol %s redeclared"), sym->ಟ್ಯಾಗ್);
              subcomplain (&sym->location, Wother,
                           _("previous declaration"));
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              sym->location = loc;
              s->ಸ್ಥಿತಿ_2 = declared;
            }
        }
    }
}


/*----------------------------.
| Set the token code of SYM.  |
`----------------------------*/

ಶೂನ್ಯ
ಚಿಹ್ನೆ_ಸಂಕೇತ_ಗಣ (ಚಿಹ್ನೆ *sym, ಪೂರ್ಣಾಂಕ ಸಂಕೇತ_2, location loc)
{
  ಪೂರ್ಣಾಂಕ *codep = &sym->content->ಸಂಕೇತ_2;
  ಆದರೆ (sym->content->class != token_sym)
    complain (&loc, complaint,
              _("nonterminals cannot be given a token code"));
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (*codep != ಸಂಕೇತ_UNDEFINED
           && *codep != ಸಂಕೇತ_2)
    complain (&loc, complaint, _("redefining code of token %s"),
              sym->ಟ್ಯಾಗ್);
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಸಂಕೇತ_2 == ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ)
    complain (&loc, complaint, _("code of token %s too large"),
              sym->ಟ್ಯಾಗ್);
  ಇಲ್ಲದಿದ್ದರೆ
    {
      *codep = ಸಂಕೇತ_2;
      /* User defined $end token? */
      ಆದರೆ (ಸಂಕೇತ_2 == 0 && !eoftoken)
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
  ಆದರೆ (s->class == unknown_sym || s->class == pct_type_sym)
    {
      complain_ಚಿಹ್ನೆ_undeclared (sym);
      s->class = nterm_sym;
    }

  ಆದರೆ (s->ಸಂಖ್ಯೆ == ಸಂಖ್ಯೆ_UNDEFINED)
    s->ಸಂಖ್ಯೆ = s->class == token_sym ? ntokens++ : nnterms++;

  ಆದರೆ (s->class == token_sym
      && sym->ಟ್ಯಾಗ್[0] == '"'
      && !sym->is_alias)
    complain (&sym->location, Wdangling_alias,
              _("string literal %s not attached to a symbol"),
              sym->ಟ್ಯಾಗ್);

  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < 2; ++i)
    ಚಿಹ್ನೆ_ಸಂಕೇತ_props_get (sym, i)->is_ಬಳಸಿದ = true;

  /* Set the semantic type status associated to the current symbol to
     'declared' so that we could check semantic types unnecessary uses. */
  ಆದರೆ (s->type_name)
    {
      semantic_type *sem_type = semantic_type_get (s->type_name, ಖಾಲಿ);
      ಆದರೆ (sem_type)
        sem_type->ಸ್ಥಿತಿ_2 = declared;
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
semantic_type_check_defined (semantic_type *sem_type)
{
  /* <*> and <> do not have to be "declared".  */
  ಆದರೆ (sem_type->ಸ್ಥಿತಿ_2 == declared
      || !*sem_type->ಟ್ಯಾಗ್
      || STREQ (sem_type->ಟ್ಯಾಗ್, "*"))
    {
      ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < 2; ++i)
        ಆದರೆ (sem_type->props[i].ಬಗೆ != ಸಂಕೇತ_PROPS_NONE
            && ! sem_type->props[i].is_ಬಳಸಿದ)
          complain (&sem_type->location, Wother,
                    _("useless %s for type <%s>"),
                    ಸಂಕೇತ_props_type_string (i), sem_type->ಟ್ಯಾಗ್);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    complain (&sem_type->location, Wother,
              _("type <%s> is used, but is not associated to any symbol"),
              sem_type->ಟ್ಯಾಗ್);
}

/*-------------------------------------------------------------------.
| Merge the properties (precedence, associativity, etc.) of SYM, and |
| its string-named alias STR; check consistency.                     |
`-------------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
ಚಿಹ್ನೆ_ವಿಲೀನ_properties (ಚಿಹ್ನೆ *sym, ಚಿಹ್ನೆ *str)
{
  ಆದರೆ (str->content->type_name != sym->content->type_name)
    {
      ಆದರೆ (str->content->type_name)
        ಚಿಹ್ನೆ_type_ಗಣ (sym,
                         str->content->type_name, str->content->type_loc);
      ಇಲ್ಲದಿದ್ದರೆ
        ಚಿಹ್ನೆ_type_ಗಣ (str,
                         sym->content->type_name, sym->content->type_loc);
    }


  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಸಂಕೇತ_PROPS_ಗಾತ್ರ; ++i)
    ಆದರೆ (str->content->props[i].ಸಂಕೇತ_2)
      ಚಿಹ್ನೆ_ಸಂಕೇತ_props_ಗಣ (sym, i, &str->content->props[i]);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (sym->content->props[i].ಸಂಕೇತ_2)
      ಚಿಹ್ನೆ_ಸಂಕೇತ_props_ಗಣ (str, i, &sym->content->props[i]);

  ಆದರೆ (sym->content->prec || str->content->prec)
    {
      ಆದರೆ (str->content->prec)
        ಚಿಹ್ನೆ_precedence_ಗಣ (sym, str->content->prec, str->content->assoc,
                               str->content->prec_loc);
      ಇಲ್ಲದಿದ್ದರೆ
        ಚಿಹ್ನೆ_precedence_ಗಣ (str, sym->content->prec, sym->content->assoc,
                               sym->content->prec_loc);
    }
}

ಶೂನ್ಯ
ಚಿಹ್ನೆ_make_alias (ಚಿಹ್ನೆ *sym, ಚಿಹ್ನೆ *str, location loc)
{
  ಆದರೆ (sym->content->class != token_sym)
    complain (&loc, complaint,
              _("nonterminals cannot be given a string alias"));
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (str->alias)
    complain (&loc, Wother,
              _("symbol %s used more than once as a literal string"), str->ಟ್ಯಾಗ್);
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (sym->alias)
    complain (&loc, Wother,
              _("symbol %s given more than one literal string"), sym->ಟ್ಯಾಗ್);
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಚಿಹ್ನೆ_ವಿಲೀನ_properties (sym, str);
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
  ಆದರೆ (sym->content->class == nterm_sym)
    sym->content->ಸಂಖ್ಯೆ += ntokens;

  symbols[sym->content->ಸಂಖ್ಯೆ] = sym->content->ಚಿಹ್ನೆ;
}

ಸ್ಥಿರ ಶೂನ್ಯ
complain_ಸಂಕೇತ_redeclared (ಪೂರ್ಣಾಂಕ ಅಂಕೆ, ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆ *ಮೊದಲ, ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆ *ಎರಡನೇ)
{
  symbols_ವಿಂಗಡಿಸು (&ಮೊದಲ, &ಎರಡನೇ);
  complain (&ಎರಡನೇ->location, complaint,
            _("code %d reassigned to token %s"),
            ಅಂಕೆ, ಎರಡನೇ->ಟ್ಯಾಗ್);
  subcomplain (&ಮೊದಲ->location, complaint,
               _("previous declaration for %s"),
               ಮೊದಲ->ಟ್ಯಾಗ್);
}

/*-------------------------------------------------.
| Put SYM in TOKEN_TRANSLATIONS if it is a token.  |
`-------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
ಚಿಹ್ನೆ_translation (ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆ *sym)
{
  ಆದರೆ (sym->content->class == token_sym && !sym->is_alias)
    {
      /* A token whose translation has already been set? */
      ಆದರೆ (token_translations[sym->content->ಸಂಕೇತ_2]
          != undeftoken->content->ಸಂಖ್ಯೆ)
        complain_ಸಂಕೇತ_redeclared
          (sym->content->ಸಂಕೇತ_2,
           symbols[token_translations[sym->content->ಸಂಕೇತ_2]], sym);
      ಇಲ್ಲದಿದ್ದರೆ
        token_translations[sym->content->ಸಂಕೇತ_2]
          = sym->content->ಸಂಖ್ಯೆ;
    }
}


/*---------------------------------------.
| Symbol and semantic type hash tables.  |
`---------------------------------------*/

/* Initial capacity of symbol and semantic type hash table.  */
#ವ್ಯಾಖ್ಯಾನಿಸು HT_INITIAL_ಸಾಮರ್ಥ್ಯ 257

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
hash_compare_ಚಿಹ್ನೆ (ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆ *m1, ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆ *m2)
{
  /* Since tags are unique, we can compare the pointers themselves.  */
  ಹಿಂತಿರುಗಿ UNIQSTR_EQ (m1->ಟ್ಯಾಗ್, m2->ಟ್ಯಾಗ್);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
hash_compare_semantic_type (ಸ್ಥಿರಾಂಕ semantic_type *m1, ಸ್ಥಿರಾಂಕ semantic_type *m2)
{
  /* Since names are unique, we can compare the pointers themselves.  */
  ಹಿಂತಿರುಗಿ UNIQSTR_EQ (m1->ಟ್ಯಾಗ್, m2->ಟ್ಯಾಗ್);
}

ಸ್ಥಿರ bool
hash_ಚಿಹ್ನೆ_comparator (ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *m1, ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *m2)
{
  ಹಿಂತಿರುಗಿ hash_compare_ಚಿಹ್ನೆ (m1, m2);
}

ಸ್ಥಿರ bool
hash_semantic_type_comparator (ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *m1, ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *m2)
{
  ಹಿಂತಿರುಗಿ hash_compare_semantic_type (m1, m2);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ size_t
hash_ಚಿಹ್ನೆ (ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆ *m, size_t tablesize)
{
  /* Since tags are unique, we can hash the pointer itself.  */
  ಹಿಂತಿರುಗಿ ((uintptr_t) m->ಟ್ಯಾಗ್) % tablesize;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ size_t
hash_semantic_type (ಸ್ಥಿರಾಂಕ semantic_type *m, size_t tablesize)
{
  /* Since names are unique, we can hash the pointer itself.  */
  ಹಿಂತಿರುಗಿ ((uintptr_t) m->ಟ್ಯಾಗ್) % tablesize;
}

ಸ್ಥಿರ size_t
hash_ಚಿಹ್ನೆ_hasher (ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *m, size_t tablesize)
{
  ಹಿಂತಿರುಗಿ hash_ಚಿಹ್ನೆ (m, tablesize);
}

ಸ್ಥಿರ size_t
hash_semantic_type_hasher (ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *m, size_t tablesize)
{
  ಹಿಂತಿರುಗಿ hash_semantic_type (m, tablesize);
}

/*-------------------------------.
| Create the symbol hash table.  |
`-------------------------------*/

ಶೂನ್ಯ
symbols_new (ಶೂನ್ಯ)
{
  ಚಿಹ್ನೆ_ಕೋಷ್ಟಕ = hash_xinitialize (HT_INITIAL_ಸಾಮರ್ಥ್ಯ,
                                   ಖಾಲಿ,
                                   hash_ಚಿಹ್ನೆ_hasher,
                                   hash_ಚಿಹ್ನೆ_comparator,
                                   ಚಿಹ್ನೆ_free);

  /* Construct the acceptsymbol symbol. */
  acceptsymbol = ಚಿಹ್ನೆ_get ("$accept", ಖಾಲಿಗಳು_loc);
  acceptsymbol->content->class = nterm_sym;
  acceptsymbol->content->ಸಂಖ್ಯೆ = nnterms++;

  /* Construct the YYerror/"error" token */
  errtoken = ಚಿಹ್ನೆ_get ("YYerror", ಖಾಲಿಗಳು_loc);
  errtoken->content->class = token_sym;
  errtoken->content->ಸಂಖ್ಯೆ = ntokens++;
  {
    ಚಿಹ್ನೆ *alias = ಚಿಹ್ನೆ_get ("error", ಖಾಲಿಗಳು_loc);
    ಚಿಹ್ನೆ_class_ಗಣ (alias, token_sym, ಖಾಲಿಗಳು_loc, false);
    ಚಿಹ್ನೆ_make_alias (errtoken, alias, ಖಾಲಿಗಳು_loc);
  }

  /* Construct the YYUNDEF/"$undefined" token that represents all
     undefined literal tokens.  It is always symbol number 2.  */
  undeftoken = ಚಿಹ್ನೆ_get ("YYUNDEF", ಖಾಲಿಗಳು_loc);
  undeftoken->content->class = token_sym;
  undeftoken->content->ಸಂಖ್ಯೆ = ntokens++;
  {
    ಚಿಹ್ನೆ *alias = ಚಿಹ್ನೆ_get ("$undefined", ಖಾಲಿಗಳು_loc);
    ಚಿಹ್ನೆ_class_ಗಣ (alias, token_sym, ಖಾಲಿಗಳು_loc, false);
    ಚಿಹ್ನೆ_make_alias (undeftoken, alias, ಖಾಲಿಗಳು_loc);
  }

  semantic_type_ಕೋಷ್ಟಕ = hash_xinitialize (HT_INITIAL_ಸಾಮರ್ಥ್ಯ,
                                          ಖಾಲಿ,
                                          hash_semantic_type_hasher,
                                          hash_semantic_type_comparator,
                                          ಬಿಡುಗಡೆ);
}


/*----------------------------------------------------------------.
| Find the symbol named KEY, and return it.  If it does not exist |
| yet, create it.                                                 |
`----------------------------------------------------------------*/

ಚಿಹ್ನೆ *
ಚಿಹ್ನೆ_from_uniqstr (ಸ್ಥಿರಾಂಕ uniqstr ಕೀಲಿ, location loc)
{
  ಚಿಹ್ನೆ probe;

  probe.ಟ್ಯಾಗ್ = ಕೀಲಿ;
  ಚಿಹ್ನೆ *res = hash_lookup (ಚಿಹ್ನೆ_ಕೋಷ್ಟಕ, &probe);

  ಆದರೆ (!res)
    {
      /* First insertion in the hash. */
      aver (!symbols_sorted);
      res = ಚಿಹ್ನೆ_new (ಕೀಲಿ, loc);
      hash_xinsert (ಚಿಹ್ನೆ_ಕೋಷ್ಟಕ, res);
    }
  ಹಿಂತಿರುಗಿ res;
}


/*-----------------------------------------------------------------------.
| Find the semantic type named KEY, and return it.  If it does not exist |
| yet, create it.                                                        |
`-----------------------------------------------------------------------*/

semantic_type *
semantic_type_from_uniqstr (ಸ್ಥಿರಾಂಕ uniqstr ಕೀಲಿ, ಸ್ಥಿರಾಂಕ location *loc)
{
  semantic_type probe;

  probe.ಟ್ಯಾಗ್ = ಕೀಲಿ;
  semantic_type *res = hash_lookup (semantic_type_ಕೋಷ್ಟಕ, &probe);

  ಆದರೆ (!res)
    {
      /* First insertion in the hash. */
      res = semantic_type_new (ಕೀಲಿ, loc);
      hash_xinsert (semantic_type_ಕೋಷ್ಟಕ, res);
    }
  ಹಿಂತಿರುಗಿ res;
}


/*----------------------------------------------------------------.
| Find the symbol named KEY, and return it.  If it does not exist |
| yet, create it.                                                 |
`----------------------------------------------------------------*/

ಚಿಹ್ನೆ *
ಚಿಹ್ನೆ_get (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ, location loc)
{
  ಹಿಂತಿರುಗಿ ಚಿಹ್ನೆ_from_uniqstr (uniqstr_new (ಕೀಲಿ), loc);
}


/*-----------------------------------------------------------------------.
| Find the semantic type named KEY, and return it.  If it does not exist |
| yet, create it.                                                        |
`-----------------------------------------------------------------------*/

semantic_type *
semantic_type_get (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ, ಸ್ಥಿರಾಂಕ location *loc)
{
  ಹಿಂತಿರುಗಿ semantic_type_from_uniqstr (uniqstr_new (ಕೀಲಿ), loc);
}


/*------------------------------------------------------------------.
| Generate a dummy nonterminal, whose name cannot conflict with the |
| user's names.                                                     |
`------------------------------------------------------------------*/

ಚಿಹ್ನೆ *
dummy_ಚಿಹ್ನೆ_get (location loc)
{
  /* Incremented for each generated symbol.  */
  ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ dummy_ಎಣಿಕೆ = 0;
  ಅಕ್ಷರ buf[32];
  ಪೂರ್ಣಾಂಕ len = snprintf (buf, ಗಾತ್ರ buf, "$@%d", ++dummy_ಎಣಿಕೆ);
  assure (len < ಗಾತ್ರ buf);
  ಚಿಹ್ನೆ *sym = ಚಿಹ್ನೆ_get (buf, loc);
  sym->content->class = nterm_sym;
  ಹಿಂತಿರುಗಿ sym;
}

bool
ಚಿಹ್ನೆ_is_dummy (ಚಿಹ್ನೆ ಸ್ಥಿರಾಂಕ *sym)
{
  ಹಿಂತಿರುಗಿ sym->ಟ್ಯಾಗ್[0] == '@' || (sym->ಟ್ಯಾಗ್[0] == '$' && sym->ಟ್ಯಾಗ್[1] == '@');
}

/*-------------------.
| Free the symbols.  |
`-------------------*/

ಶೂನ್ಯ
symbols_free (ಶೂನ್ಯ)
{
  hash_free (ಚಿಹ್ನೆ_ಕೋಷ್ಟಕ);
  hash_free (semantic_type_ಕೋಷ್ಟಕ);
  ಬಿಡುಗಡೆ (symbols);
  ಬಿಡುಗಡೆ (symbols_sorted);
  ಬಿಡುಗಡೆ (semantic_types_sorted);
}


ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ಚಿಹ್ನೆ_cmp (ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *a, ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *b)
{
  ಹಿಂತಿರುಗಿ location_cmp ((*(ಚಿಹ್ನೆ * ಸ್ಥಿರಾಂಕ *)a)->location,
                       (*(ಚಿಹ್ನೆ * ಸ್ಥಿರಾಂಕ *)b)->location);
}

/* Store in *SORTED an array of pointers to the symbols contained in
   TABLE, sorted by order of appearance (i.e., by location). */

ಸ್ಥಿರ ಶೂನ್ಯ
ಕೋಷ್ಟಕ_ವಿಂಗಡಿಸು (ರಚನೆ hash_ಕೋಷ್ಟಕ *ಕೋಷ್ಟಕ, ಚಿಹ್ನೆ ***sorted)
{
  aver (!*sorted);
  size_t ಎಣಿಕೆ_2 = hash_get_n_entries (ಕೋಷ್ಟಕ);
  *sorted = xnmalloc (ಎಣಿಕೆ_2 + 1, ಗಾತ್ರ **sorted);
  hash_get_entries (ಕೋಷ್ಟಕ, (ಶೂನ್ಯ**)*sorted, ಎಣಿಕೆ_2);
  qsort (*sorted, ಎಣಿಕೆ_2, ಗಾತ್ರ **sorted, ಚಿಹ್ನೆ_cmp);
  (*sorted)[ಎಣಿಕೆ_2] = ಖಾಲಿ;
}

/*--------------------------------------------------------------.
| Check that all the symbols are defined.  Report any undefined |
| symbols and consider them nonterminals.                       |
`--------------------------------------------------------------*/

ಶೂನ್ಯ
symbols_check_defined (ಶೂನ್ಯ)
{
  ಕೋಷ್ಟಕ_ವಿಂಗಡಿಸು (ಚಿಹ್ನೆ_ಕೋಷ್ಟಕ, &symbols_sorted);
  /* semantic_type, like symbol, starts with a 'tag' field and then a
     'location' field.  And here we only deal with arrays/hashes of
     pointers, sizeof is not an issue.

     So instead of implementing table_sort (and symbol_cmp) once for
     each type, let's lie a bit to the typing system, and treat
     'semantic_type' as if it were 'symbol'. */
  ಕೋಷ್ಟಕ_ವಿಂಗಡಿಸು (semantic_type_ಕೋಷ್ಟಕ, (ಚಿಹ್ನೆ ***) &semantic_types_sorted);

  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; symbols_sorted[i]; ++i)
    ಚಿಹ್ನೆ_check_defined (symbols_sorted[i]);
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; semantic_types_sorted[i]; ++i)
    semantic_type_check_defined (semantic_types_sorted[i]);
}

/*------------------------------------------------------------------.
| Set TOKEN_TRANSLATIONS.  Check that no two symbols share the same |
| number.                                                           |
`------------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
symbols_token_translations_init (ಶೂನ್ಯ)
{
  bool ಸಂಕೇತ_256_ಲಭ್ಯ_p = true;

  /* Find the highest token code, and whether 256, the POSIX preferred
     token code for the error token, is used.  */
  max_ಸಂಕೇತ = 0;
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ntokens; ++i)
    {
      sym_content *sym = symbols[i]->content;
      ಆದರೆ (sym->ಸಂಕೇತ_2 != ಸಂಕೇತ_UNDEFINED)
        {
          ಆದರೆ (sym->ಸಂಕೇತ_2 > max_ಸಂಕೇತ)
            max_ಸಂಕೇತ = sym->ಸಂಕೇತ_2;
          ಆದರೆ (sym->ಸಂಕೇತ_2 == 256)
            ಸಂಕೇತ_256_ಲಭ್ಯ_p = false;
        }
    }

  /* If 256 is not used, assign it to error, to follow POSIX.  */
  ಆದರೆ (ಸಂಕೇತ_256_ಲಭ್ಯ_p
      && errtoken->content->ಸಂಕೇತ_2 == ಸಂಕೇತ_UNDEFINED)
    errtoken->content->ಸಂಕೇತ_2 = 256;

  /* Set the missing codes. */
  ಆದರೆ (max_ಸಂಕೇತ < 256)
    max_ಸಂಕೇತ = 256;

  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ntokens; ++i)
    {
      sym_content *sym = symbols[i]->content;
      ಆದರೆ (sym->ಸಂಕೇತ_2 == ಸಂಕೇತ_UNDEFINED)
        {
          IGNORE_TYPE_LIMITS_BEGIN
          ಆದರೆ (INT_ಕೂಡಿಸು_WRAPV (max_ಸಂಕೇತ, 1, &max_ಸಂಕೇತ))
            complain (ಖಾಲಿ, fatal, _("token number too large"));
          IGNORE_TYPE_LIMITS_ಕೊನೆ
          sym->ಸಂಕೇತ_2 = max_ಸಂಕೇತ;
        }
      ಆದರೆ (sym->ಸಂಕೇತ_2 > max_ಸಂಕೇತ)
        max_ಸಂಕೇತ = sym->ಸಂಕೇತ_2;
    }

  token_translations = xnmalloc (max_ಸಂಕೇತ + 1,
                                 ಗಾತ್ರ *token_translations);

  /* Initialize all entries for literal tokens to the internal token
     number for $undefined, which represents all invalid inputs.  */
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < max_ಸಂಕೇತ + 1; ++i)
    token_translations[i] = undeftoken->content->ಸಂಖ್ಯೆ;
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; symbols_sorted[i]; ++i)
    ಚಿಹ್ನೆ_translation (symbols_sorted[i]);
}


/* Whether some symbol requires internationalization.  */
ಸ್ಥಿರ bool
has_translations (ಶೂನ್ಯ)
{
  ಗಾಗಿ (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ನಮೂದು = hash_get_ಮೊದಲ (ಚಿಹ್ನೆ_ಕೋಷ್ಟಕ);
       ನಮೂದು;
       ನಮೂದು = hash_get_ಮುಂದಿನ (ಚಿಹ್ನೆ_ಕೋಷ್ಟಕ, ನಮೂದು))
    {
      ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆ *sym = (ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆ *) ನಮೂದು;
      ಆದರೆ (sym->translatable)
        ಹಿಂತಿರುಗಿ true;
    }
  ಹಿಂತಿರುಗಿ false;
}

/*----------------------------------------------------------------.
| Assign symbol numbers, and write definition of token names into |
| FDEFINES.  Set up vectors SYMBOL_TABLE, TAGS of symbols.        |
`----------------------------------------------------------------*/

ಶೂನ್ಯ
symbols_pack (ಶೂನ್ಯ)
{
  symbols = xcalloc (nsyms, ಗಾತ್ರ *symbols);
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; symbols_sorted[i]; ++i)
    ಚಿಹ್ನೆ_pack (symbols_sorted[i]);

  /* Aliases leave empty slots in symbols, so remove them.  */
  {
    ಪೂರ್ಣಾಂಕ nsyms_old = nsyms;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ writei = 0, readi = 0; readi < nsyms_old; readi += 1)
      {
        ಆದರೆ (symbols[readi] == ಖಾಲಿ)
          {
            nsyms -= 1;
            ntokens -= 1;
          }
        ಇಲ್ಲದಿದ್ದರೆ
          {
            symbols[writei] = symbols[readi];
            symbols[writei]->content->ಸಂಖ್ಯೆ = writei;
            writei += 1;
          }
      }
  }
  symbols = xnrealloc (symbols, nsyms, ಗಾತ್ರ *symbols);

  symbols_token_translations_init ();

  // If some user tokens are internationalized, the internal ones
  // should be too.
  ಆದರೆ (has_translations ())
    {
      ಸ್ಥಿರಾಂಕ bool eof_is_user_defined
        = !eoftoken->alias || STRNEQ (eoftoken->alias->ಟ್ಯಾಗ್, "$end");
      ಆದರೆ (!eof_is_user_defined)
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
  prec_nodes = xcalloc (nsyms, ಗಾತ್ರ *prec_nodes);
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < nsyms; ++i)
    {
      prec_nodes[i] = xmalloc (ಗಾತ್ರ *prec_nodes[i]);
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
symgraphlink_new (graphid id, symgraphlink *ಮುಂದಿನ)
{
  symgraphlink *res = xmalloc (ಗಾತ್ರ *res);
  res->id = id;
  res->ಮುಂದಿನ = ಮುಂದಿನ;
  ಹಿಂತಿರುಗಿ res;
}


/*------------------------------------------------------------------.
| Register the second symbol of the precedence relation, and return |
| whether this relation is new.  Use only in register_precedence.   |
`------------------------------------------------------------------*/

ಸ್ಥಿರ bool
ರಿಜಿಸ್ಟರ್_precedence_ಎರಡನೇ_ಚಿಹ್ನೆ (symgraphlink **ಮೊದಲ, graphid sym)
{
  ಆದರೆ (!*ಮೊದಲ || sym < (*ಮೊದಲ)->id)
    *ಮೊದಲ = symgraphlink_new (sym, *ಮೊದಲ);
  ಇಲ್ಲದಿದ್ದರೆ
    {
      symgraphlink *slist = *ಮೊದಲ;

      ಆಗ (slist->ಮುಂದಿನ && slist->ಮುಂದಿನ->id <= sym)
        slist = slist->ಮುಂದಿನ;

      ಆದರೆ (slist->id == sym)
        /* Relation already present. */
        ಹಿಂತಿರುಗಿ false;

      slist->ಮುಂದಿನ = symgraphlink_new (sym, slist->ಮುಂದಿನ);
    }
  ಹಿಂತಿರುಗಿ true;
}

/*------------------------------------------------------------------.
| Register a new relation between symbols as used. The first symbol |
| has a greater precedence than the second one.                     |
`------------------------------------------------------------------*/

ಶೂನ್ಯ
ರಿಜಿಸ್ಟರ್_precedence (graphid ಮೊದಲ, graphid snd)
{
  ಆದರೆ (!prec_nodes)
    init_prec_nodes ();
  ರಿಜಿಸ್ಟರ್_precedence_ಎರಡನೇ_ಚಿಹ್ನೆ (&(prec_nodes[ಮೊದಲ]->succ), snd);
  ರಿಜಿಸ್ಟರ್_precedence_ಎರಡನೇ_ಚಿಹ್ನೆ (&(prec_nodes[snd]->pred), ಮೊದಲ);
}


/*---------------------------------------.
| Deep clear a linked / adjacency list). |
`---------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
linkedlist_free (symgraphlink *ಗಂಟು)
{
  ಆದರೆ (ಗಂಟು)
    {
      ಆಗ (ಗಂಟು->ಮುಂದಿನ)
        {
          symgraphlink *tmp = ಗಂಟು->ಮುಂದಿನ;
          ಬಿಡುಗಡೆ (ಗಂಟು);
          ಗಂಟು = tmp;
        }
      ಬಿಡುಗಡೆ (ಗಂಟು);
    }
}

/*----------------------------------------------.
| Clear and destroy association tracking table. |
`----------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
assoc_free (ಶೂನ್ಯ)
{
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < nsyms; ++i)
    {
      linkedlist_free (prec_nodes[i]->pred);
      linkedlist_free (prec_nodes[i]->succ);
      ಬಿಡುಗಡೆ (prec_nodes[i]);
    }
  ಬಿಡುಗಡೆ (prec_nodes);
}

/*---------------------------------------.
| Initialize association tracking table. |
`---------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
init_assoc (ಶೂನ್ಯ)
{
  ಬಳಸಿದ_assoc = xcalloc (nsyms, ಗಾತ್ರ *ಬಳಸಿದ_assoc);
  ಗಾಗಿ (graphid i = 0; i < nsyms; ++i)
    ಬಳಸಿದ_assoc[i] = false;
}

/*------------------------------------------------------------------.
| Test if the associativity for the symbols is defined and useless. |
`------------------------------------------------------------------*/

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
is_assoc_useless (ಚಿಹ್ನೆ *s)
{
  ಹಿಂತಿರುಗಿ s
      && s->content->assoc != undef_assoc
      && s->content->assoc != precedence_assoc
      && !ಬಳಸಿದ_assoc[s->content->ಸಂಖ್ಯೆ];
}

/*-------------------------------.
| Register a used associativity. |
`-------------------------------*/

ಶೂನ್ಯ
ರಿಜಿಸ್ಟರ್_assoc (graphid i, graphid j)
{
  ಆದರೆ (!ಬಳಸಿದ_assoc)
    init_assoc ();
  ಬಳಸಿದ_assoc[i] = true;
  ಬಳಸಿದ_assoc[j] = true;
}

/*--------------------------------------------------.
| Print a warning for unused precedence relations.  |
`--------------------------------------------------*/

ಶೂನ್ಯ
print_precedence_warnings (ಶೂನ್ಯ)
{
  ಆದರೆ (!prec_nodes)
    init_prec_nodes ();
  ಆದರೆ (!ಬಳಸಿದ_assoc)
    init_assoc ();
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < nsyms; ++i)
    {
      ಚಿಹ್ನೆ *s = symbols[i];
      ಆದರೆ (s
          && s->content->prec != 0
          && !prec_nodes[i]->pred
          && !prec_nodes[i]->succ)
        {
          ಆದರೆ (is_assoc_useless (s))
            complain (&s->content->prec_loc, Wprecedence,
                      _("useless precedence and associativity for %s"), s->ಟ್ಯಾಗ್);
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (s->content->assoc == precedence_assoc)
            complain (&s->content->prec_loc, Wprecedence,
                      _("useless precedence for %s"), s->ಟ್ಯಾಗ್);
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (is_assoc_useless (s))
        complain (&s->content->prec_loc, Wprecedence,
                  _("useless associativity for %s, use %%precedence"), s->ಟ್ಯಾಗ್);
    }
  ಬಿಡುಗಡೆ (ಬಳಸಿದ_assoc);
  assoc_free ();
}
