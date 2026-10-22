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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ "symtab.h"

#અન્તર્ભાવ "system.h"

#અન્તર્ભાવ <assure.h>
#અન્તર્ભાવ <fstrcmp.h>
#અન્તર્ભાવ <hash.h>
#અન્તર્ભાવ <quote.h>

#અન્તર્ભાવ "complain.h"
#અન્તર્ભાવ "getargs.h"
#અન્તર્ભાવ "gram.h"
#અન્તર્ભાવ "intprops.h"

/** Undefined token code.  */
#define CODE_UNDEFINED (-1)

/* Undefined symbol number.  */
#define સંખ્યા_UNDEFINED (-1)


સ્થિર સંરચના hash_table *પ્રતીક_table = ખાલી;
સ્થિર સંરચના hash_table *semantic_type_table = ખાલી;

/*----------------------------------------------------------------.
| Symbols sorted by tag.  Allocated by table_sort, after which no |
| more symbols should be created.                                 |
`----------------------------------------------------------------*/

સ્થિર પ્રતીક **symbols_sorted = ખાલી;
સ્થિર semantic_type **semantic_types_sorted = ખાલી;


/*------------------------.
| Distinguished symbols.  |
`------------------------*/

પ્રતીક *errtoken = ખાલી;
પ્રતીક *undeftoken = ખાલી;
પ્રતીક *eoftoken = ખાલી;
પ્રતીક *acceptsymbol = ખાલી;

/* Precedence relation graph. */
સ્થિર symgraph **prec_nodes;

/* Store which associativity is used.  */
સ્થિર bool *used_assoc = ખાલી;

bool tag_seen = false;


/* Whether SYM was defined by the user.  */

સ્થિર bool
પ્રતીક_is_user_defined (પ્રતીક *sym)
{
  અચલ bool eof_is_user_defined
    = !eoftoken->alias || STRNEQ (eoftoken->alias->tag, "$end");
  પાછા sym->tag[0] != '$'
    && (eof_is_user_defined || (sym != eoftoken && sym->alias != errtoken))
    && sym != errtoken && sym->alias != errtoken
    && sym != undeftoken && sym->alias != undeftoken;
}


/*--------------------------.
| Create a new sym_content. |
`--------------------------*/

સ્થિર sym_content *
sym_content_new (પ્રતીક *s)
{
  sym_content *res = xmalloc (કદ *res);

  res->પ્રતીક = s;

  res->type_name = ખાલી;
  res->type_loc = empty_loc;
  માટે (પૂર્ણાંક i = 0; i < CODE_PROPS_SIZE; ++i)
    code_props_none_init (&res->props[i]);

  res->સંખ્યા = સંખ્યા_UNDEFINED;
  res->prec_loc = empty_loc;
  res->prec = 0;
  res->assoc = undef_assoc;
  res->code = CODE_UNDEFINED;

  res->class = unknown_sym;
  res->status = undeclared;

  પાછા res;
}

/*---------------------------------.
| Create a new symbol, named TAG.  |
`---------------------------------*/

સ્થિર પ્રતીક *
પ્રતીક_new (uniqstr tag, location loc)
{
  પ્રતીક *res = xmalloc (કદ *res);
  uniqstr_assert (tag);

  /* If the tag is not a string (starts with a double quote), check
     that it is valid for Yacc. */
  જો (tag[0] != '\"' && tag[0] != '\'' && strchr (tag, '-'))
    complain (&loc, Wyacc,
              _("POSIX Yacc forbids dashes in symbol names: %s"), tag);

  res->tag = tag;
  res->location = loc;
  res->translatable = false;
  res->location_of_lhs = false;
  res->alias = ખાલી;
  res->content = sym_content_new (res);
  res->is_alias = false;
  પાછા res;
}

/*--------------------.
| Free a sym_content. |
`--------------------*/

સ્થિર શૂન્ય
sym_content_free (sym_content *sym)
{
  મુક્ત (sym);
}


/*---------------------------------------------------------.
| Free a symbol and its associated content if appropriate. |
`---------------------------------------------------------*/

સ્થિર શૂન્ય
પ્રતીક_free (શૂન્ય *ptr)
{
  પ્રતીક *sym = (પ્રતીક *)ptr;
  જો (!sym->is_alias)
    sym_content_free (sym->content);
  મુક્ત (sym);

}

/* If needed, swap first and second so that first has the earliest
   location (according to location_cmp).

   Many symbol features (e.g., token codes) are not assigned during
   parsing, but in a second step, via a traversal of the symbol table
   sorted on tag.

   However, error messages make more sense if we keep the first
   declaration first.
*/

સ્થિર શૂન્ય
symbols_sort (અચલ પ્રતીક **first, અચલ પ્રતીક **second)
{
  જો (0 < location_cmp ((*first)->location, (*second)->location))
    {
      અચલ પ્રતીક* tmp = *first;
      *first = *second;
      *second = tmp;
    }
}

/* Likewise, for locations.  */

સ્થિર શૂન્ય
locations_sort (location *first, location *second)
{
  જો (0 < location_cmp (*first, *second))
    {
      location tmp = *first;
      *first = *second;
      *second = tmp;
    }
}

અક્ષર અચલ *
code_props_type_string (code_props_type kind)
{
  પસંદગી (kind)
    {
    સ્થિતિ destructor:
      પાછા "%destructor";
    સ્થિતિ printer:
      પાછા "%printer";
    }
  abort ();
}


/*----------------------------------------.
| Create a new semantic type, named TAG.  |
`----------------------------------------*/

સ્થિર semantic_type *
semantic_type_new (uniqstr tag, અચલ location *loc)
{
  semantic_type *res = xmalloc (કદ *res);

  uniqstr_assert (tag);
  res->tag = tag;
  res->location = loc ? *loc : empty_loc;
  res->status = undeclared;
  માટે (પૂર્ણાંક i = 0; i < CODE_PROPS_SIZE; ++i)
    code_props_none_init (&res->props[i]);

  પાછા res;
}


/*-----------------.
| Print a symbol.  |
`-----------------*/

#define પ્રતીક_INT_ATTR_PRINT(Attr)                     \
  જો (s->content)                                       \
    fprintf (f, " %s = %d", #Attr, s->content->Attr)

#define પ્રતીક_STR_ATTR_PRINT(Attr)                     \
  જો (s->content && s->content->Attr)                   \
    fprintf (f, " %s { %s }", #Attr, s->content->Attr)

#define પ્રતીક_CODE_PRINT(Attr)                                         \
  જો (s->content && s->content->props[Attr].code)                       \
    fprintf (f, " %s { %s }", #Attr, s->content->props[Attr].code)

શૂન્ય
પ્રતીક_print (પ્રતીક અચલ *s, FILE *f)
{
  જો (s)
    {
      પ્રતીક_class c = s->content->class;
      fprintf (f, "%s: %s",
               c == unknown_sym    ? "unknown"
               : c == pct_type_sym ? "%type"
               : c == token_sym    ? "token"
               : c == nterm_sym    ? "nterm"
               : ખાલી, /* abort.  */
               s->tag);
      putc (' ', f);
      location_print (s->location, f);
      પ્રતીક_INT_ATTR_PRINT (code);
      પ્રતીક_INT_ATTR_PRINT (સંખ્યા);
      પ્રતીક_STR_ATTR_PRINT (type_name);
      પ્રતીક_CODE_PRINT (destructor);
      પ્રતીક_CODE_PRINT (printer);
    }
  નહીંતર
    fputs ("<NULL>", f);
}

#undef પ્રતીક_ATTR_PRINT
#undef પ્રતીક_CODE_PRINT


/*----------------------------------.
| Whether S is a valid identifier.  |
`----------------------------------*/

સ્થિર bool
is_identifier (uniqstr s)
{
  સ્થિર અક્ષર અચલ alphanum[26 + 26 + 1 + 10] =
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "_"
    "0123456789";
  જો (!s || ! memchr (alphanum, *s, કદ alphanum - 10))
    પાછા false;
  માટે (++s; *s; ++s)
    જો (! memchr (alphanum, *s, કદ alphanum))
      પાછા false;
  પાછા true;
}


/*-----------------------------------------------.
| Get the identifier associated to this symbol.  |
`-----------------------------------------------*/

uniqstr
પ્રતીક_id_get (પ્રતીક અચલ *sym)
{
  // There's one weird case: YYerror is the alias, and error is the
  // base symbol.  Return YYerror in that case.
  જો (sym->alias && is_identifier (sym->alias->tag))
    પાછા sym->alias->tag;
  નહીંતર જો (is_identifier (sym->tag))
    પાછા sym->tag;
  નહીંતર
    પાછા ખાલી;
}


/*------------------------------------------------------------------.
| Complain that S's WHAT is redeclared at SECOND, and was first set |
| at FIRST.                                                         |
`------------------------------------------------------------------*/

સ્થિર શૂન્ય
complain_પ્રતીક_redeclared (પ્રતીક *s, અચલ અક્ષર *what, location first,
                            location second)
{
  locations_sort (&first, &second);
  complain (&second, complaint, _("%s redeclaration for %s"), what, s->tag);
  subcomplain (&first, complaint, _("previous declaration"));
}

સ્થિર શૂન્ય
complain_semantic_type_redeclared (semantic_type *s, અચલ અક્ષર *what, location first,
                                   location second)
{
  locations_sort (&first, &second);
  complain (&second, complaint, _("%s redeclaration for <%s>"), what, s->tag);
  subcomplain (&first, complaint, _("previous declaration"));
}

સ્થિર શૂન્ય
complain_class_redeclared (પ્રતીક *sym, પ્રતીક_class class, location second)
{
  complain (&second, complaint,
            class == token_sym
            ? _("symbol %s redeclared as a token")
            : _("symbol %s redeclared as a nonterminal"), sym->tag);
  જો (!location_empty (sym->location))
    subcomplain (&sym->location, complaint, _("previous definition"));
}

સ્થિર અચલ પ્રતીક *
પ્રતીક_from_uniqstr_fuzzy (અચલ uniqstr ચાવી)
{
  aver (symbols_sorted);
#define FSTRCMP_THRESHOLD 0.6
  દ્વિગુણ best_similarity = FSTRCMP_THRESHOLD;
  અચલ પ્રતીક *res = ખાલી;
  size_t ગણતરી = hash_get_n_entries (પ્રતીક_table);
  માટે (પૂર્ણાંક i = 0; i < ગણતરી; ++i)
    {
      પ્રતીક *sym = symbols_sorted[i];
      જો (STRNEQ (ચાવી, sym->tag)
          && (sym->content->status == declared
              || sym->content->status == undeclared))
        {
          દ્વિગુણ similarity = fstrcmp_bounded (ચાવી, sym->tag, best_similarity);
          જો (best_similarity < similarity)
            {
              res = sym;
              best_similarity = similarity;
            }
        }
    }
  પાછા res;
}

સ્થિર શૂન્ય
complain_પ્રતીક_undeclared (અચલ પ્રતીક *sym)
{
  પ્રતિજ્ઞા (sym->content->status != declared);
  અચલ પ્રતીક *best = પ્રતીક_from_uniqstr_fuzzy (sym->tag);
  જો (best)
    {
      complain (&sym->location,
                sym->content->status == needed ? complaint : Wother,
                _("symbol %s is used, but is not defined as a token"
                  " and has no rules; did you mean %s?"),
                quote_n (0, sym->tag),
                quote_n (1, best->tag));
      જો (feature_flag & feature_caret)
        location_caret_suggestion (sym->location, best->tag, stderr);
    }
  નહીંતર
    complain (&sym->location,
              sym->content->status == needed ? complaint : Wother,
              _("symbol %s is used, but is not defined as a token"
                " and has no rules"),
              quote (sym->tag));
}

શૂન્ય
પ્રતીક_location_as_lhs_set (પ્રતીક *sym, location loc)
{
  જો (!sym->location_of_lhs)
    {
      sym->location = loc;
      sym->location_of_lhs = true;
    }
}


/*-----------------------------------------------------------------.
| Set the TYPE_NAME associated with SYM.  Does nothing if passed 0 |
| as TYPE_NAME.                                                    |
`-----------------------------------------------------------------*/

શૂન્ય
પ્રતીક_type_set (પ્રતીક *sym, uniqstr type_name, location loc)
{
  જો (type_name)
    {
      tag_seen = true;
      જો (sym->content->type_name)
        complain_પ્રતીક_redeclared (sym, "%type",
                                    sym->content->type_loc, loc);
      નહીંતર
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

શૂન્ય
પ્રતીક_code_props_set (પ્રતીક *sym, code_props_type kind,
                       code_props અચલ *code)
{
  જો (sym->content->props[kind].code)
    complain_પ્રતીક_redeclared (sym, code_props_type_string (kind),
                                sym->content->props[kind].location,
                                code->location);
  નહીંતર
    sym->content->props[kind] = *code;
}


/*-----------------------------------------------------.
| Set the DESTRUCTOR or PRINTER associated with TYPE.  |
`-----------------------------------------------------*/

શૂન્ય
semantic_type_code_props_set (semantic_type *type,
                              code_props_type kind,
                              code_props અચલ *code)
{
  જો (type->props[kind].code)
    complain_semantic_type_redeclared (type, code_props_type_string (kind),
                                       type->props[kind].location,
                                       code->location);
  નહીંતર
    type->props[kind] = *code;
}

/*---------------------------------------------------.
| Get the computed %destructor or %printer for SYM.  |
`---------------------------------------------------*/

code_props *
પ્રતીક_code_props_get (પ્રતીક *sym, code_props_type kind)
{
  /* Per-symbol code props.  */
  જો (sym->content->props[kind].code)
    પાછા &sym->content->props[kind];

  /* Per-type code props.  */
  જો (sym->content->type_name)
    {
      code_props *code =
        &semantic_type_get (sym->content->type_name, ખાલી)->props[kind];
      જો (code->code)
        પાછા code;
    }

  /* Apply default code props's only to user-defined symbols.  */
  જો (પ્રતીક_is_user_defined (sym))
    {
      code_props *code = &semantic_type_get (sym->content->type_name ? "*" : "",
                                             ખાલી)->props[kind];
      જો (code->code)
        પાછા code;
    }
  પાછા &code_props_none;
}

/*-----------------------------------------------------------------.
| Set the PRECEDENCE associated with SYM.  Does nothing if invoked |
| with UNDEF_ASSOC as ASSOC.                                       |
`-----------------------------------------------------------------*/

શૂન્ય
પ્રતીક_precedence_set (પ્રતીક *sym, પૂર્ણાંક prec, assoc a, location loc)
{
  જો (a != undef_assoc)
    {
      sym_content *s = sym->content;
      જો (s->prec)
        complain_પ્રતીક_redeclared (sym, assoc_to_string (a),
                                    s->prec_loc, loc);
      નહીંતર
        {
          s->prec = prec;
          s->assoc = a;
          s->prec_loc = loc;
        }
    }

  /* Only terminals have a precedence. */
  પ્રતીક_class_set (sym, token_sym, loc, false);
}


/*------------------------------------.
| Set the CLASS associated with SYM.  |
`------------------------------------*/

સ્થિર શૂન્ય
complain_pct_type_on_token (location *loc)
{
  complain (loc, Wyacc,
            _("POSIX yacc reserves %%type to nonterminals"));
}

શૂન્ય
પ્રતીક_class_set (પ્રતીક *sym, પ્રતીક_class class, location loc, bool declaring)
{
  aver (class != unknown_sym);
  sym_content *s = sym->content;
  જો (class == pct_type_sym)
    {
      જો (s->class == token_sym)
        complain_pct_type_on_token (&loc);
      નહીંતર જો (s->class == unknown_sym)
        s->class = class;
    }
  નહીંતર જો (s->class != unknown_sym && s->class != pct_type_sym
           && s->class != class)
    complain_class_redeclared (sym, class, loc);
  નહીંતર
    {
      જો (class == token_sym && s->class == pct_type_sym)
        complain_pct_type_on_token (&sym->location);

      s->class = class;

      જો (declaring)
        {
          જો (s->status == declared)
            {
              complain (&loc, Wother,
                        _("symbol %s redeclared"), sym->tag);
              subcomplain (&sym->location, Wother,
                           _("previous declaration"));
            }
          નહીંતર
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

શૂન્ય
પ્રતીક_code_set (પ્રતીક *sym, પૂર્ણાંક code, location loc)
{
  પૂર્ણાંક *codep = &sym->content->code;
  જો (sym->content->class != token_sym)
    complain (&loc, complaint,
              _("nonterminals cannot be given a token code"));
  નહીંતર જો (*codep != CODE_UNDEFINED
           && *codep != code)
    complain (&loc, complaint, _("redefining code of token %s"),
              sym->tag);
  નહીંતર જો (code == INT_MAX)
    complain (&loc, complaint, _("code of token %s too large"),
              sym->tag);
  નહીંતર
    {
      *codep = code;
      /* User defined $end token? */
      જો (code == 0 && !eoftoken)
        {
          eoftoken = sym->content->પ્રતીક;
          eoftoken->content->સંખ્યા = 0;
        }
    }
}


/*----------------------------------------------------------.
| If SYM is not defined, report an error, and consider it a |
| nonterminal.                                              |
`----------------------------------------------------------*/

સ્થિર શૂન્ય
પ્રતીક_check_defined (પ્રતીક *sym)
{
  sym_content *s = sym->content;
  જો (s->class == unknown_sym || s->class == pct_type_sym)
    {
      complain_પ્રતીક_undeclared (sym);
      s->class = nterm_sym;
    }

  જો (s->સંખ્યા == સંખ્યા_UNDEFINED)
    s->સંખ્યા = s->class == token_sym ? ntokens++ : nnterms++;

  જો (s->class == token_sym
      && sym->tag[0] == '"'
      && !sym->is_alias)
    complain (&sym->location, Wdangling_alias,
              _("string literal %s not attached to a symbol"),
              sym->tag);

  માટે (પૂર્ણાંક i = 0; i < 2; ++i)
    પ્રતીક_code_props_get (sym, i)->is_used = true;

  /* Set the semantic type status associated to the current symbol to
     'declared' so that we could check semantic types unnecessary uses. */
  જો (s->type_name)
    {
      semantic_type *sem_type = semantic_type_get (s->type_name, ખાલી);
      જો (sem_type)
        sem_type->status = declared;
    }
}

સ્થિર શૂન્ય
semantic_type_check_defined (semantic_type *sem_type)
{
  /* <*> and <> do not have to be "declared".  */
  જો (sem_type->status == declared
      || !*sem_type->tag
      || STREQ (sem_type->tag, "*"))
    {
      માટે (પૂર્ણાંક i = 0; i < 2; ++i)
        જો (sem_type->props[i].kind != CODE_PROPS_NONE
            && ! sem_type->props[i].is_used)
          complain (&sem_type->location, Wother,
                    _("useless %s for type <%s>"),
                    code_props_type_string (i), sem_type->tag);
    }
  નહીંતર
    complain (&sem_type->location, Wother,
              _("type <%s> is used, but is not associated to any symbol"),
              sem_type->tag);
}

/*-------------------------------------------------------------------.
| Merge the properties (precedence, associativity, etc.) of SYM, and |
| its string-named alias STR; check consistency.                     |
`-------------------------------------------------------------------*/

સ્થિર શૂન્ય
પ્રતીક_merge_properties (પ્રતીક *sym, પ્રતીક *str)
{
  જો (str->content->type_name != sym->content->type_name)
    {
      જો (str->content->type_name)
        પ્રતીક_type_set (sym,
                         str->content->type_name, str->content->type_loc);
      નહીંતર
        પ્રતીક_type_set (str,
                         sym->content->type_name, sym->content->type_loc);
    }


  માટે (પૂર્ણાંક i = 0; i < CODE_PROPS_SIZE; ++i)
    જો (str->content->props[i].code)
      પ્રતીક_code_props_set (sym, i, &str->content->props[i]);
    નહીંતર જો (sym->content->props[i].code)
      પ્રતીક_code_props_set (str, i, &sym->content->props[i]);

  જો (sym->content->prec || str->content->prec)
    {
      જો (str->content->prec)
        પ્રતીક_precedence_set (sym, str->content->prec, str->content->assoc,
                               str->content->prec_loc);
      નહીંતર
        પ્રતીક_precedence_set (str, sym->content->prec, sym->content->assoc,
                               sym->content->prec_loc);
    }
}

શૂન્ય
પ્રતીક_make_alias (પ્રતીક *sym, પ્રતીક *str, location loc)
{
  જો (sym->content->class != token_sym)
    complain (&loc, complaint,
              _("nonterminals cannot be given a string alias"));
  નહીંતર જો (str->alias)
    complain (&loc, Wother,
              _("symbol %s used more than once as a literal string"), str->tag);
  નહીંતર જો (sym->alias)
    complain (&loc, Wother,
              _("symbol %s given more than one literal string"), sym->tag);
  નહીંતર
    {
      પ્રતીક_merge_properties (sym, str);
      sym_content_free (str->content);
      str->content = sym->content;
      str->content->પ્રતીક = str;
      str->is_alias = true;
      str->alias = sym;
      sym->alias = str;
    }
}


/*-------------------------------------------------------------------.
| Assign a symbol number, and write the definition of the token name |
| into FDEFINES.  Put in SYMBOLS.                                    |
`-------------------------------------------------------------------*/

સ્થિર શૂન્ય
પ્રતીક_pack (પ્રતીક *sym)
{
  aver (sym->content->સંખ્યા != સંખ્યા_UNDEFINED);
  જો (sym->content->class == nterm_sym)
    sym->content->સંખ્યા += ntokens;

  symbols[sym->content->સંખ્યા] = sym->content->પ્રતીક;
}

સ્થિર શૂન્ય
complain_code_redeclared (પૂર્ણાંક અંક, અચલ પ્રતીક *first, અચલ પ્રતીક *second)
{
  symbols_sort (&first, &second);
  complain (&second->location, complaint,
            _("code %d reassigned to token %s"),
            અંક, second->tag);
  subcomplain (&first->location, complaint,
               _("previous declaration for %s"),
               first->tag);
}

/*-------------------------------------------------.
| Put SYM in TOKEN_TRANSLATIONS if it is a token.  |
`-------------------------------------------------*/

સ્થિર શૂન્ય
પ્રતીક_translation (અચલ પ્રતીક *sym)
{
  જો (sym->content->class == token_sym && !sym->is_alias)
    {
      /* A token whose translation has already been set? */
      જો (token_translations[sym->content->code]
          != undeftoken->content->સંખ્યા)
        complain_code_redeclared
          (sym->content->code,
           symbols[token_translations[sym->content->code]], sym);
      નહીંતર
        token_translations[sym->content->code]
          = sym->content->સંખ્યા;
    }
}


/*---------------------------------------.
| Symbol and semantic type hash tables.  |
`---------------------------------------*/

/* Initial capacity of symbol and semantic type hash table.  */
#define HT_INITIAL_CAPACITY 257

સ્થિર અંતઃરેખ bool
hash_compare_પ્રતીક (અચલ પ્રતીક *m1, અચલ પ્રતીક *m2)
{
  /* Since tags are unique, we can compare the pointers themselves.  */
  પાછા UNIQSTR_EQ (m1->tag, m2->tag);
}

સ્થિર અંતઃરેખ bool
hash_compare_semantic_type (અચલ semantic_type *m1, અચલ semantic_type *m2)
{
  /* Since names are unique, we can compare the pointers themselves.  */
  પાછા UNIQSTR_EQ (m1->tag, m2->tag);
}

સ્થિર bool
hash_પ્રતીક_comparator (શૂન્ય અચલ *m1, શૂન્ય અચલ *m2)
{
  પાછા hash_compare_પ્રતીક (m1, m2);
}

સ્થિર bool
hash_semantic_type_comparator (શૂન્ય અચલ *m1, શૂન્ય અચલ *m2)
{
  પાછા hash_compare_semantic_type (m1, m2);
}

સ્થિર અંતઃરેખ size_t
hash_પ્રતીક (અચલ પ્રતીક *m, size_t tablesize)
{
  /* Since tags are unique, we can hash the pointer itself.  */
  પાછા ((uintptr_t) m->tag) % tablesize;
}

સ્થિર અંતઃરેખ size_t
hash_semantic_type (અચલ semantic_type *m, size_t tablesize)
{
  /* Since names are unique, we can hash the pointer itself.  */
  પાછા ((uintptr_t) m->tag) % tablesize;
}

સ્થિર size_t
hash_પ્રતીક_hasher (શૂન્ય અચલ *m, size_t tablesize)
{
  પાછા hash_પ્રતીક (m, tablesize);
}

સ્થિર size_t
hash_semantic_type_hasher (શૂન્ય અચલ *m, size_t tablesize)
{
  પાછા hash_semantic_type (m, tablesize);
}

/*-------------------------------.
| Create the symbol hash table.  |
`-------------------------------*/

શૂન્ય
symbols_new (શૂન્ય)
{
  પ્રતીક_table = hash_xinitialize (HT_INITIAL_CAPACITY,
                                   ખાલી,
                                   hash_પ્રતીક_hasher,
                                   hash_પ્રતીક_comparator,
                                   પ્રતીક_free);

  /* Construct the acceptsymbol symbol. */
  acceptsymbol = પ્રતીક_get ("$accept", empty_loc);
  acceptsymbol->content->class = nterm_sym;
  acceptsymbol->content->સંખ્યા = nnterms++;

  /* Construct the YYerror/"error" token */
  errtoken = પ્રતીક_get ("YYerror", empty_loc);
  errtoken->content->class = token_sym;
  errtoken->content->સંખ્યા = ntokens++;
  {
    પ્રતીક *alias = પ્રતીક_get ("error", empty_loc);
    પ્રતીક_class_set (alias, token_sym, empty_loc, false);
    પ્રતીક_make_alias (errtoken, alias, empty_loc);
  }

  /* Construct the YYUNDEF/"$undefined" token that represents all
     undefined literal tokens.  It is always symbol number 2.  */
  undeftoken = પ્રતીક_get ("YYUNDEF", empty_loc);
  undeftoken->content->class = token_sym;
  undeftoken->content->સંખ્યા = ntokens++;
  {
    પ્રતીક *alias = પ્રતીક_get ("$undefined", empty_loc);
    પ્રતીક_class_set (alias, token_sym, empty_loc, false);
    પ્રતીક_make_alias (undeftoken, alias, empty_loc);
  }

  semantic_type_table = hash_xinitialize (HT_INITIAL_CAPACITY,
                                          ખાલી,
                                          hash_semantic_type_hasher,
                                          hash_semantic_type_comparator,
                                          મુક્ત);
}


/*----------------------------------------------------------------.
| Find the symbol named KEY, and return it.  If it does not exist |
| yet, create it.                                                 |
`----------------------------------------------------------------*/

પ્રતીક *
પ્રતીક_from_uniqstr (અચલ uniqstr ચાવી, location loc)
{
  પ્રતીક probe;

  probe.tag = ચાવી;
  પ્રતીક *res = hash_lookup (પ્રતીક_table, &probe);

  જો (!res)
    {
      /* First insertion in the hash. */
      aver (!symbols_sorted);
      res = પ્રતીક_new (ચાવી, loc);
      hash_xinsert (પ્રતીક_table, res);
    }
  પાછા res;
}


/*-----------------------------------------------------------------------.
| Find the semantic type named KEY, and return it.  If it does not exist |
| yet, create it.                                                        |
`-----------------------------------------------------------------------*/

semantic_type *
semantic_type_from_uniqstr (અચલ uniqstr ચાવી, અચલ location *loc)
{
  semantic_type probe;

  probe.tag = ચાવી;
  semantic_type *res = hash_lookup (semantic_type_table, &probe);

  જો (!res)
    {
      /* First insertion in the hash. */
      res = semantic_type_new (ચાવી, loc);
      hash_xinsert (semantic_type_table, res);
    }
  પાછા res;
}


/*----------------------------------------------------------------.
| Find the symbol named KEY, and return it.  If it does not exist |
| yet, create it.                                                 |
`----------------------------------------------------------------*/

પ્રતીક *
પ્રતીક_get (અચલ અક્ષર *ચાવી, location loc)
{
  પાછા પ્રતીક_from_uniqstr (uniqstr_new (ચાવી), loc);
}


/*-----------------------------------------------------------------------.
| Find the semantic type named KEY, and return it.  If it does not exist |
| yet, create it.                                                        |
`-----------------------------------------------------------------------*/

semantic_type *
semantic_type_get (અચલ અક્ષર *ચાવી, અચલ location *loc)
{
  પાછા semantic_type_from_uniqstr (uniqstr_new (ચાવી), loc);
}


/*------------------------------------------------------------------.
| Generate a dummy nonterminal, whose name cannot conflict with the |
| user's names.                                                     |
`------------------------------------------------------------------*/

પ્રતીક *
dummy_પ્રતીક_get (location loc)
{
  /* Incremented for each generated symbol.  */
  સ્થિર પૂર્ણાંક dummy_ગણતરી = 0;
  અક્ષર buf[32];
  પૂર્ણાંક len = snprintf (buf, કદ buf, "$@%d", ++dummy_ગણતરી);
  assure (len < કદ buf);
  પ્રતીક *sym = પ્રતીક_get (buf, loc);
  sym->content->class = nterm_sym;
  પાછા sym;
}

bool
પ્રતીક_is_dummy (પ્રતીક અચલ *sym)
{
  પાછા sym->tag[0] == '@' || (sym->tag[0] == '$' && sym->tag[1] == '@');
}

/*-------------------.
| Free the symbols.  |
`-------------------*/

શૂન્ય
symbols_free (શૂન્ય)
{
  hash_free (પ્રતીક_table);
  hash_free (semantic_type_table);
  મુક્ત (symbols);
  મુક્ત (symbols_sorted);
  મુક્ત (semantic_types_sorted);
}


સ્થિર પૂર્ણાંક
પ્રતીક_cmp (શૂન્ય અચલ *a, શૂન્ય અચલ *b)
{
  પાછા location_cmp ((*(પ્રતીક * અચલ *)a)->location,
                       (*(પ્રતીક * અચલ *)b)->location);
}

/* Store in *SORTED an array of pointers to the symbols contained in
   TABLE, sorted by order of appearance (i.e., by location). */

સ્થિર શૂન્ય
table_sort (સંરચના hash_table *table, પ્રતીક ***sorted)
{
  aver (!*sorted);
  size_t ગણતરી = hash_get_n_entries (table);
  *sorted = xnmalloc (ગણતરી + 1, કદ **sorted);
  hash_get_entries (table, (શૂન્ય**)*sorted, ગણતરી);
  qsort (*sorted, ગણતરી, કદ **sorted, પ્રતીક_cmp);
  (*sorted)[ગણતરી] = ખાલી;
}

/*--------------------------------------------------------------.
| Check that all the symbols are defined.  Report any undefined |
| symbols and consider them nonterminals.                       |
`--------------------------------------------------------------*/

શૂન્ય
symbols_check_defined (શૂન્ય)
{
  table_sort (પ્રતીક_table, &symbols_sorted);
  /* semantic_type, like symbol, starts with a 'tag' field and then a
     'location' field.  And here we only deal with arrays/hashes of
     pointers, sizeof is not an issue.

     So instead of implementing table_sort (and symbol_cmp) once for
     each type, let's lie a bit to the typing system, and treat
     'semantic_type' as if it were 'symbol'. */
  table_sort (semantic_type_table, (પ્રતીક ***) &semantic_types_sorted);

  માટે (પૂર્ણાંક i = 0; symbols_sorted[i]; ++i)
    પ્રતીક_check_defined (symbols_sorted[i]);
  માટે (પૂર્ણાંક i = 0; semantic_types_sorted[i]; ++i)
    semantic_type_check_defined (semantic_types_sorted[i]);
}

/*------------------------------------------------------------------.
| Set TOKEN_TRANSLATIONS.  Check that no two symbols share the same |
| number.                                                           |
`------------------------------------------------------------------*/

સ્થિર શૂન્ય
symbols_token_translations_init (શૂન્ય)
{
  bool code_256_available_p = true;

  /* Find the highest token code, and whether 256, the POSIX preferred
     token code for the error token, is used.  */
  max_code = 0;
  માટે (પૂર્ણાંક i = 0; i < ntokens; ++i)
    {
      sym_content *sym = symbols[i]->content;
      જો (sym->code != CODE_UNDEFINED)
        {
          જો (sym->code > max_code)
            max_code = sym->code;
          જો (sym->code == 256)
            code_256_available_p = false;
        }
    }

  /* If 256 is not used, assign it to error, to follow POSIX.  */
  જો (code_256_available_p
      && errtoken->content->code == CODE_UNDEFINED)
    errtoken->content->code = 256;

  /* Set the missing codes. */
  જો (max_code < 256)
    max_code = 256;

  માટે (પૂર્ણાંક i = 0; i < ntokens; ++i)
    {
      sym_content *sym = symbols[i]->content;
      જો (sym->code == CODE_UNDEFINED)
        {
          IGNORE_TYPE_LIMITS_BEGIN
          જો (INT_ADD_WRAPV (max_code, 1, &max_code))
            complain (ખાલી, fatal, _("token number too large"));
          IGNORE_TYPE_LIMITS_END
          sym->code = max_code;
        }
      જો (sym->code > max_code)
        max_code = sym->code;
    }

  token_translations = xnmalloc (max_code + 1,
                                 કદ *token_translations);

  /* Initialize all entries for literal tokens to the internal token
     number for $undefined, which represents all invalid inputs.  */
  માટે (પૂર્ણાંક i = 0; i < max_code + 1; ++i)
    token_translations[i] = undeftoken->content->સંખ્યા;
  માટે (પૂર્ણાંક i = 0; symbols_sorted[i]; ++i)
    પ્રતીક_translation (symbols_sorted[i]);
}


/* Whether some symbol requires internationalization.  */
સ્થિર bool
has_translations (શૂન્ય)
{
  માટે (અચલ શૂન્ય *entry = hash_get_first (પ્રતીક_table);
       entry;
       entry = hash_get_next (પ્રતીક_table, entry))
    {
      અચલ પ્રતીક *sym = (અચલ પ્રતીક *) entry;
      જો (sym->translatable)
        પાછા true;
    }
  પાછા false;
}

/*----------------------------------------------------------------.
| Assign symbol numbers, and write definition of token names into |
| FDEFINES.  Set up vectors SYMBOL_TABLE, TAGS of symbols.        |
`----------------------------------------------------------------*/

શૂન્ય
symbols_pack (શૂન્ય)
{
  symbols = xcalloc (nsyms, કદ *symbols);
  માટે (પૂર્ણાંક i = 0; symbols_sorted[i]; ++i)
    પ્રતીક_pack (symbols_sorted[i]);

  /* Aliases leave empty slots in symbols, so remove them.  */
  {
    પૂર્ણાંક nsyms_old = nsyms;
    માટે (પૂર્ણાંક writei = 0, readi = 0; readi < nsyms_old; readi += 1)
      {
        જો (symbols[readi] == ખાલી)
          {
            nsyms -= 1;
            ntokens -= 1;
          }
        નહીંતર
          {
            symbols[writei] = symbols[readi];
            symbols[writei]->content->સંખ્યા = writei;
            writei += 1;
          }
      }
  }
  symbols = xnrealloc (symbols, nsyms, કદ *symbols);

  symbols_token_translations_init ();

  // If some user tokens are internationalized, the internal ones
  // should be too.
  જો (has_translations ())
    {
      અચલ bool eof_is_user_defined
        = !eoftoken->alias || STRNEQ (eoftoken->alias->tag, "$end");
      જો (!eof_is_user_defined)
        eoftoken->alias->translatable = true;
      undeftoken->alias->translatable = true;
      errtoken->alias->translatable = true;
    }
}

/*---------------------------------.
| Initialize relation graph nodes. |
`---------------------------------*/

સ્થિર શૂન્ય
init_prec_nodes (શૂન્ય)
{
  prec_nodes = xcalloc (nsyms, કદ *prec_nodes);
  માટે (પૂર્ણાંક i = 0; i < nsyms; ++i)
    {
      prec_nodes[i] = xmalloc (કદ *prec_nodes[i]);
      symgraph *s = prec_nodes[i];
      s->id = i;
      s->succ = 0;
      s->pred = 0;
    }
}

/*----------------.
| Create a link.  |
`----------------*/

સ્થિર symgraphlink *
symgraphlink_new (graphid id, symgraphlink *next)
{
  symgraphlink *res = xmalloc (કદ *res);
  res->id = id;
  res->next = next;
  પાછા res;
}


/*------------------------------------------------------------------.
| Register the second symbol of the precedence relation, and return |
| whether this relation is new.  Use only in register_precedence.   |
`------------------------------------------------------------------*/

સ્થિર bool
register_precedence_second_પ્રતીક (symgraphlink **first, graphid sym)
{
  જો (!*first || sym < (*first)->id)
    *first = symgraphlink_new (sym, *first);
  નહીંતર
    {
      symgraphlink *slist = *first;

      જ્યારે (slist->next && slist->next->id <= sym)
        slist = slist->next;

      જો (slist->id == sym)
        /* Relation already present. */
        પાછા false;

      slist->next = symgraphlink_new (sym, slist->next);
    }
  પાછા true;
}

/*------------------------------------------------------------------.
| Register a new relation between symbols as used. The first symbol |
| has a greater precedence than the second one.                     |
`------------------------------------------------------------------*/

શૂન્ય
register_precedence (graphid first, graphid snd)
{
  જો (!prec_nodes)
    init_prec_nodes ();
  register_precedence_second_પ્રતીક (&(prec_nodes[first]->succ), snd);
  register_precedence_second_પ્રતીક (&(prec_nodes[snd]->pred), first);
}


/*---------------------------------------.
| Deep clear a linked / adjacency list). |
`---------------------------------------*/

સ્થિર શૂન્ય
linkedlist_free (symgraphlink *ગાંઠ)
{
  જો (ગાંઠ)
    {
      જ્યારે (ગાંઠ->next)
        {
          symgraphlink *tmp = ગાંઠ->next;
          મુક્ત (ગાંઠ);
          ગાંઠ = tmp;
        }
      મુક્ત (ગાંઠ);
    }
}

/*----------------------------------------------.
| Clear and destroy association tracking table. |
`----------------------------------------------*/

સ્થિર શૂન્ય
assoc_free (શૂન્ય)
{
  માટે (પૂર્ણાંક i = 0; i < nsyms; ++i)
    {
      linkedlist_free (prec_nodes[i]->pred);
      linkedlist_free (prec_nodes[i]->succ);
      મુક્ત (prec_nodes[i]);
    }
  મુક્ત (prec_nodes);
}

/*---------------------------------------.
| Initialize association tracking table. |
`---------------------------------------*/

સ્થિર શૂન્ય
init_assoc (શૂન્ય)
{
  used_assoc = xcalloc (nsyms, કદ *used_assoc);
  માટે (graphid i = 0; i < nsyms; ++i)
    used_assoc[i] = false;
}

/*------------------------------------------------------------------.
| Test if the associativity for the symbols is defined and useless. |
`------------------------------------------------------------------*/

સ્થિર અંતઃરેખ bool
is_assoc_useless (પ્રતીક *s)
{
  પાછા s
      && s->content->assoc != undef_assoc
      && s->content->assoc != precedence_assoc
      && !used_assoc[s->content->સંખ્યા];
}

/*-------------------------------.
| Register a used associativity. |
`-------------------------------*/

શૂન્ય
register_assoc (graphid i, graphid j)
{
  જો (!used_assoc)
    init_assoc ();
  used_assoc[i] = true;
  used_assoc[j] = true;
}

/*--------------------------------------------------.
| Print a warning for unused precedence relations.  |
`--------------------------------------------------*/

શૂન્ય
print_precedence_warnings (શૂન્ય)
{
  જો (!prec_nodes)
    init_prec_nodes ();
  જો (!used_assoc)
    init_assoc ();
  માટે (પૂર્ણાંક i = 0; i < nsyms; ++i)
    {
      પ્રતીક *s = symbols[i];
      જો (s
          && s->content->prec != 0
          && !prec_nodes[i]->pred
          && !prec_nodes[i]->succ)
        {
          જો (is_assoc_useless (s))
            complain (&s->content->prec_loc, Wprecedence,
                      _("useless precedence and associativity for %s"), s->tag);
          નહીંતર જો (s->content->assoc == precedence_assoc)
            complain (&s->content->prec_loc, Wprecedence,
                      _("useless precedence for %s"), s->tag);
        }
      નહીંતર જો (is_assoc_useless (s))
        complain (&s->content->prec_loc, Wprecedence,
                  _("useless associativity for %s, use %%precedence"), s->tag);
    }
  મુક્ત (used_assoc);
  assoc_free ();
}
