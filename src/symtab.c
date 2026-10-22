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

#अन्तर्भाव <config.h>
#अन्तर्भाव "symtab.h"

#अन्तर्भाव "system.h"

#अन्तर्भाव <assure.h>
#अन्तर्भाव <fstrcmp.h>
#अन्तर्भाव <hash.h>
#अन्तर्भाव <quote.h>

#अन्तर्भाव "complain.h"
#अन्तर्भाव "getargs.h"
#अन्तर्भाव "gram.h"
#अन्तर्भाव "intprops.h"

/** Undefined token code.  */
#define CODE_UNDEFINED (-1)

/* Undefined symbol number.  */
#define सङ्ख्या_UNDEFINED (-1)


स्थिर संरचना hash_table *चिह्नम्_table = रिक्त;
स्थिर संरचना hash_table *semantic_type_table = रिक्त;

/*----------------------------------------------------------------.
| Symbols sorted by tag.  Allocated by table_sort, after which no |
| more symbols should be created.                                 |
`----------------------------------------------------------------*/

स्थिर चिह्नम् **symbols_sorted = रिक्त;
स्थिर semantic_type **semantic_types_sorted = रिक्त;


/*------------------------.
| Distinguished symbols.  |
`------------------------*/

चिह्नम् *errtoken = रिक्त;
चिह्नम् *undeftoken = रिक्त;
चिह्नम् *eoftoken = रिक्त;
चिह्नम् *acceptsymbol = रिक्त;

/* Precedence relation graph. */
स्थिर symgraph **prec_nodes;

/* Store which associativity is used.  */
स्थिर bool *used_assoc = रिक्त;

bool tag_seen = false;


/* Whether SYM was defined by the user.  */

स्थिर bool
चिह्नम्_is_user_defined (चिह्नम् *sym)
{
  ध्रुव bool eof_is_user_defined
    = !eoftoken->alias || STRNEQ (eoftoken->alias->tag, "$end");
  पटिनिवत्त sym->tag[0] != '$'
    && (eof_is_user_defined || (sym != eoftoken && sym->alias != errtoken))
    && sym != errtoken && sym->alias != errtoken
    && sym != undeftoken && sym->alias != undeftoken;
}


/*--------------------------.
| Create a new sym_content. |
`--------------------------*/

स्थिर sym_content *
sym_content_new (चिह्नम् *s)
{
  sym_content *res = xmalloc (परिमाण *res);

  res->चिह्नम् = s;

  res->type_name = रिक्त;
  res->type_loc = empty_loc;
  अत्थाय (पूर्णाङ्क i = 0; i < CODE_PROPS_SIZE; ++i)
    code_props_none_init (&res->props[i]);

  res->सङ्ख्या = सङ्ख्या_UNDEFINED;
  res->prec_loc = empty_loc;
  res->prec = 0;
  res->assoc = undef_assoc;
  res->code = CODE_UNDEFINED;

  res->class = unknown_sym;
  res->status = undeclared;

  पटिनिवत्त res;
}

/*---------------------------------.
| Create a new symbol, named TAG.  |
`---------------------------------*/

स्थिर चिह्नम् *
चिह्नम्_new (uniqstr tag, location loc)
{
  चिह्नम् *res = xmalloc (परिमाण *res);
  uniqstr_assert (tag);

  /* If the tag is not a string (starts with a double quote), check
     that it is valid for Yacc. */
  सचे (tag[0] != '\"' && tag[0] != '\'' && strchr (tag, '-'))
    complain (&loc, Wyacc,
              _("POSIX Yacc forbids dashes in symbol names: %s"), tag);

  res->tag = tag;
  res->location = loc;
  res->translatable = false;
  res->location_of_lhs = false;
  res->alias = रिक्त;
  res->content = sym_content_new (res);
  res->is_alias = false;
  पटिनिवत्त res;
}

/*--------------------.
| Free a sym_content. |
`--------------------*/

स्थिर शून्य
sym_content_free (sym_content *sym)
{
  मुक्त (sym);
}


/*---------------------------------------------------------.
| Free a symbol and its associated content if appropriate. |
`---------------------------------------------------------*/

स्थिर शून्य
चिह्नम्_free (शून्य *ptr)
{
  चिह्नम् *sym = (चिह्नम् *)ptr;
  सचे (!sym->is_alias)
    sym_content_free (sym->content);
  मुक्त (sym);

}

/* If needed, swap first and second so that first has the earliest
   location (according to location_cmp).

   Many symbol features (e.g., token codes) are not assigned during
   parsing, but in a second step, via a traversal of the symbol table
   sorted on tag.

   However, error messages make more sense if we keep the first
   declaration first.
*/

स्थिर शून्य
symbols_sort (ध्रुव चिह्नम् **first, ध्रुव चिह्नम् **second)
{
  सचे (0 < location_cmp ((*first)->location, (*second)->location))
    {
      ध्रुव चिह्नम्* tmp = *first;
      *first = *second;
      *second = tmp;
    }
}

/* Likewise, for locations.  */

स्थिर शून्य
locations_sort (location *first, location *second)
{
  सचे (0 < location_cmp (*first, *second))
    {
      location tmp = *first;
      *first = *second;
      *second = tmp;
    }
}

वर्ण ध्रुव *
code_props_type_string (code_props_type kind)
{
  विकल्प (kind)
    {
    पक्ष destructor:
      पटिनिवत्त "%destructor";
    पक्ष printer:
      पटिनिवत्त "%printer";
    }
  abort ();
}


/*----------------------------------------.
| Create a new semantic type, named TAG.  |
`----------------------------------------*/

स्थिर semantic_type *
semantic_type_new (uniqstr tag, ध्रुव location *loc)
{
  semantic_type *res = xmalloc (परिमाण *res);

  uniqstr_assert (tag);
  res->tag = tag;
  res->location = loc ? *loc : empty_loc;
  res->status = undeclared;
  अत्थाय (पूर्णाङ्क i = 0; i < CODE_PROPS_SIZE; ++i)
    code_props_none_init (&res->props[i]);

  पटिनिवत्त res;
}


/*-----------------.
| Print a symbol.  |
`-----------------*/

#define चिह्नम्_INT_ATTR_PRINT(Attr)                     \
  सचे (s->content)                                       \
    fprintf (f, " %s = %d", #Attr, s->content->Attr)

#define चिह्नम्_STR_ATTR_PRINT(Attr)                     \
  सचे (s->content && s->content->Attr)                   \
    fprintf (f, " %s { %s }", #Attr, s->content->Attr)

#define चिह्नम्_CODE_PRINT(Attr)                                         \
  सचे (s->content && s->content->props[Attr].code)                       \
    fprintf (f, " %s { %s }", #Attr, s->content->props[Attr].code)

शून्य
चिह्नम्_print (चिह्नम् ध्रुव *s, FILE *f)
{
  सचे (s)
    {
      चिह्नम्_class c = s->content->class;
      fprintf (f, "%s: %s",
               c == unknown_sym    ? "unknown"
               : c == pct_type_sym ? "%type"
               : c == token_sym    ? "token"
               : c == nterm_sym    ? "nterm"
               : रिक्त, /* abort.  */
               s->tag);
      putc (' ', f);
      location_print (s->location, f);
      चिह्नम्_INT_ATTR_PRINT (code);
      चिह्नम्_INT_ATTR_PRINT (सङ्ख्या);
      चिह्नम्_STR_ATTR_PRINT (type_name);
      चिह्नम्_CODE_PRINT (destructor);
      चिह्नम्_CODE_PRINT (printer);
    }
  अञ्ञथा
    fputs ("<NULL>", f);
}

#undef चिह्नम्_ATTR_PRINT
#undef चिह्नम्_CODE_PRINT


/*----------------------------------.
| Whether S is a valid identifier.  |
`----------------------------------*/

स्थिर bool
is_identifier (uniqstr s)
{
  स्थिर वर्ण ध्रुव alphanum[26 + 26 + 1 + 10] =
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "_"
    "0123456789";
  सचे (!s || ! memchr (alphanum, *s, परिमाण alphanum - 10))
    पटिनिवत्त false;
  अत्थाय (++s; *s; ++s)
    सचे (! memchr (alphanum, *s, परिमाण alphanum))
      पटिनिवत्त false;
  पटिनिवत्त true;
}


/*-----------------------------------------------.
| Get the identifier associated to this symbol.  |
`-----------------------------------------------*/

uniqstr
चिह्नम्_id_get (चिह्नम् ध्रुव *sym)
{
  // There's one weird case: YYerror is the alias, and error is the
  // base symbol.  Return YYerror in that case.
  सचे (sym->alias && is_identifier (sym->alias->tag))
    पटिनिवत्त sym->alias->tag;
  अञ्ञथा सचे (is_identifier (sym->tag))
    पटिनिवत्त sym->tag;
  अञ्ञथा
    पटिनिवत्त रिक्त;
}


/*------------------------------------------------------------------.
| Complain that S's WHAT is redeclared at SECOND, and was first set |
| at FIRST.                                                         |
`------------------------------------------------------------------*/

स्थिर शून्य
complain_चिह्नम्_redeclared (चिह्नम् *s, ध्रुव वर्ण *what, location first,
                            location second)
{
  locations_sort (&first, &second);
  complain (&second, complaint, _("%s redeclaration for %s"), what, s->tag);
  subcomplain (&first, complaint, _("previous declaration"));
}

स्थिर शून्य
complain_semantic_type_redeclared (semantic_type *s, ध्रुव वर्ण *what, location first,
                                   location second)
{
  locations_sort (&first, &second);
  complain (&second, complaint, _("%s redeclaration for <%s>"), what, s->tag);
  subcomplain (&first, complaint, _("previous declaration"));
}

स्थिर शून्य
complain_class_redeclared (चिह्नम् *sym, चिह्नम्_class class, location second)
{
  complain (&second, complaint,
            class == token_sym
            ? _("symbol %s redeclared as a token")
            : _("symbol %s redeclared as a nonterminal"), sym->tag);
  सचे (!location_empty (sym->location))
    subcomplain (&sym->location, complaint, _("previous definition"));
}

स्थिर ध्रुव चिह्नम् *
चिह्नम्_from_uniqstr_fuzzy (ध्रुव uniqstr कुञ्चिका)
{
  aver (symbols_sorted);
#define FSTRCMP_THRESHOLD 0.6
  द्विगुण best_similarity = FSTRCMP_THRESHOLD;
  ध्रुव चिह्नम् *res = रिक्त;
  size_t गणनम् = hash_get_n_entries (चिह्नम्_table);
  अत्थाय (पूर्णाङ्क i = 0; i < गणनम्; ++i)
    {
      चिह्नम् *sym = symbols_sorted[i];
      सचे (STRNEQ (कुञ्चिका, sym->tag)
          && (sym->content->status == declared
              || sym->content->status == undeclared))
        {
          द्विगुण similarity = fstrcmp_bounded (कुञ्चिका, sym->tag, best_similarity);
          सचे (best_similarity < similarity)
            {
              res = sym;
              best_similarity = similarity;
            }
        }
    }
  पटिनिवत्त res;
}

स्थिर शून्य
complain_चिह्नम्_undeclared (ध्रुव चिह्नम् *sym)
{
  प्रतिज्ञा (sym->content->status != declared);
  ध्रुव चिह्नम् *best = चिह्नम्_from_uniqstr_fuzzy (sym->tag);
  सचे (best)
    {
      complain (&sym->location,
                sym->content->status == needed ? complaint : Wother,
                _("symbol %s is used, but is not defined as a token"
                  " and has no rules; did you mean %s?"),
                quote_n (0, sym->tag),
                quote_n (1, best->tag));
      सचे (feature_flag & feature_caret)
        location_caret_suggestion (sym->location, best->tag, stderr);
    }
  अञ्ञथा
    complain (&sym->location,
              sym->content->status == needed ? complaint : Wother,
              _("symbol %s is used, but is not defined as a token"
                " and has no rules"),
              quote (sym->tag));
}

शून्य
चिह्नम्_location_as_lhs_set (चिह्नम् *sym, location loc)
{
  सचे (!sym->location_of_lhs)
    {
      sym->location = loc;
      sym->location_of_lhs = true;
    }
}


/*-----------------------------------------------------------------.
| Set the TYPE_NAME associated with SYM.  Does nothing if passed 0 |
| as TYPE_NAME.                                                    |
`-----------------------------------------------------------------*/

शून्य
चिह्नम्_type_set (चिह्नम् *sym, uniqstr type_name, location loc)
{
  सचे (type_name)
    {
      tag_seen = true;
      सचे (sym->content->type_name)
        complain_चिह्नम्_redeclared (sym, "%type",
                                    sym->content->type_loc, loc);
      अञ्ञथा
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

शून्य
चिह्नम्_code_props_set (चिह्नम् *sym, code_props_type kind,
                       code_props ध्रुव *code)
{
  सचे (sym->content->props[kind].code)
    complain_चिह्नम्_redeclared (sym, code_props_type_string (kind),
                                sym->content->props[kind].location,
                                code->location);
  अञ्ञथा
    sym->content->props[kind] = *code;
}


/*-----------------------------------------------------.
| Set the DESTRUCTOR or PRINTER associated with TYPE.  |
`-----------------------------------------------------*/

शून्य
semantic_type_code_props_set (semantic_type *type,
                              code_props_type kind,
                              code_props ध्रुव *code)
{
  सचे (type->props[kind].code)
    complain_semantic_type_redeclared (type, code_props_type_string (kind),
                                       type->props[kind].location,
                                       code->location);
  अञ्ञथा
    type->props[kind] = *code;
}

/*---------------------------------------------------.
| Get the computed %destructor or %printer for SYM.  |
`---------------------------------------------------*/

code_props *
चिह्नम्_code_props_get (चिह्नम् *sym, code_props_type kind)
{
  /* Per-symbol code props.  */
  सचे (sym->content->props[kind].code)
    पटिनिवत्त &sym->content->props[kind];

  /* Per-type code props.  */
  सचे (sym->content->type_name)
    {
      code_props *code =
        &semantic_type_get (sym->content->type_name, रिक्त)->props[kind];
      सचे (code->code)
        पटिनिवत्त code;
    }

  /* Apply default code props's only to user-defined symbols.  */
  सचे (चिह्नम्_is_user_defined (sym))
    {
      code_props *code = &semantic_type_get (sym->content->type_name ? "*" : "",
                                             रिक्त)->props[kind];
      सचे (code->code)
        पटिनिवत्त code;
    }
  पटिनिवत्त &code_props_none;
}

/*-----------------------------------------------------------------.
| Set the PRECEDENCE associated with SYM.  Does nothing if invoked |
| with UNDEF_ASSOC as ASSOC.                                       |
`-----------------------------------------------------------------*/

शून्य
चिह्नम्_precedence_set (चिह्नम् *sym, पूर्णाङ्क prec, assoc a, location loc)
{
  सचे (a != undef_assoc)
    {
      sym_content *s = sym->content;
      सचे (s->prec)
        complain_चिह्नम्_redeclared (sym, assoc_to_string (a),
                                    s->prec_loc, loc);
      अञ्ञथा
        {
          s->prec = prec;
          s->assoc = a;
          s->prec_loc = loc;
        }
    }

  /* Only terminals have a precedence. */
  चिह्नम्_class_set (sym, token_sym, loc, false);
}


/*------------------------------------.
| Set the CLASS associated with SYM.  |
`------------------------------------*/

स्थिर शून्य
complain_pct_type_on_token (location *loc)
{
  complain (loc, Wyacc,
            _("POSIX yacc reserves %%type to nonterminals"));
}

शून्य
चिह्नम्_class_set (चिह्नम् *sym, चिह्नम्_class class, location loc, bool declaring)
{
  aver (class != unknown_sym);
  sym_content *s = sym->content;
  सचे (class == pct_type_sym)
    {
      सचे (s->class == token_sym)
        complain_pct_type_on_token (&loc);
      अञ्ञथा सचे (s->class == unknown_sym)
        s->class = class;
    }
  अञ्ञथा सचे (s->class != unknown_sym && s->class != pct_type_sym
           && s->class != class)
    complain_class_redeclared (sym, class, loc);
  अञ्ञथा
    {
      सचे (class == token_sym && s->class == pct_type_sym)
        complain_pct_type_on_token (&sym->location);

      s->class = class;

      सचे (declaring)
        {
          सचे (s->status == declared)
            {
              complain (&loc, Wother,
                        _("symbol %s redeclared"), sym->tag);
              subcomplain (&sym->location, Wother,
                           _("previous declaration"));
            }
          अञ्ञथा
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

शून्य
चिह्नम्_code_set (चिह्नम् *sym, पूर्णाङ्क code, location loc)
{
  पूर्णाङ्क *codep = &sym->content->code;
  सचे (sym->content->class != token_sym)
    complain (&loc, complaint,
              _("nonterminals cannot be given a token code"));
  अञ्ञथा सचे (*codep != CODE_UNDEFINED
           && *codep != code)
    complain (&loc, complaint, _("redefining code of token %s"),
              sym->tag);
  अञ्ञथा सचे (code == INT_MAX)
    complain (&loc, complaint, _("code of token %s too large"),
              sym->tag);
  अञ्ञथा
    {
      *codep = code;
      /* User defined $end token? */
      सचे (code == 0 && !eoftoken)
        {
          eoftoken = sym->content->चिह्नम्;
          eoftoken->content->सङ्ख्या = 0;
        }
    }
}


/*----------------------------------------------------------.
| If SYM is not defined, report an error, and consider it a |
| nonterminal.                                              |
`----------------------------------------------------------*/

स्थिर शून्य
चिह्नम्_check_defined (चिह्नम् *sym)
{
  sym_content *s = sym->content;
  सचे (s->class == unknown_sym || s->class == pct_type_sym)
    {
      complain_चिह्नम्_undeclared (sym);
      s->class = nterm_sym;
    }

  सचे (s->सङ्ख्या == सङ्ख्या_UNDEFINED)
    s->सङ्ख्या = s->class == token_sym ? ntokens++ : nnterms++;

  सचे (s->class == token_sym
      && sym->tag[0] == '"'
      && !sym->is_alias)
    complain (&sym->location, Wdangling_alias,
              _("string literal %s not attached to a symbol"),
              sym->tag);

  अत्थाय (पूर्णाङ्क i = 0; i < 2; ++i)
    चिह्नम्_code_props_get (sym, i)->is_used = true;

  /* Set the semantic type status associated to the current symbol to
     'declared' so that we could check semantic types unnecessary uses. */
  सचे (s->type_name)
    {
      semantic_type *sem_type = semantic_type_get (s->type_name, रिक्त);
      सचे (sem_type)
        sem_type->status = declared;
    }
}

स्थिर शून्य
semantic_type_check_defined (semantic_type *sem_type)
{
  /* <*> and <> do not have to be "declared".  */
  सचे (sem_type->status == declared
      || !*sem_type->tag
      || STREQ (sem_type->tag, "*"))
    {
      अत्थाय (पूर्णाङ्क i = 0; i < 2; ++i)
        सचे (sem_type->props[i].kind != CODE_PROPS_NONE
            && ! sem_type->props[i].is_used)
          complain (&sem_type->location, Wother,
                    _("useless %s for type <%s>"),
                    code_props_type_string (i), sem_type->tag);
    }
  अञ्ञथा
    complain (&sem_type->location, Wother,
              _("type <%s> is used, but is not associated to any symbol"),
              sem_type->tag);
}

/*-------------------------------------------------------------------.
| Merge the properties (precedence, associativity, etc.) of SYM, and |
| its string-named alias STR; check consistency.                     |
`-------------------------------------------------------------------*/

स्थिर शून्य
चिह्नम्_merge_properties (चिह्नम् *sym, चिह्नम् *str)
{
  सचे (str->content->type_name != sym->content->type_name)
    {
      सचे (str->content->type_name)
        चिह्नम्_type_set (sym,
                         str->content->type_name, str->content->type_loc);
      अञ्ञथा
        चिह्नम्_type_set (str,
                         sym->content->type_name, sym->content->type_loc);
    }


  अत्थाय (पूर्णाङ्क i = 0; i < CODE_PROPS_SIZE; ++i)
    सचे (str->content->props[i].code)
      चिह्नम्_code_props_set (sym, i, &str->content->props[i]);
    अञ्ञथा सचे (sym->content->props[i].code)
      चिह्नम्_code_props_set (str, i, &sym->content->props[i]);

  सचे (sym->content->prec || str->content->prec)
    {
      सचे (str->content->prec)
        चिह्नम्_precedence_set (sym, str->content->prec, str->content->assoc,
                               str->content->prec_loc);
      अञ्ञथा
        चिह्नम्_precedence_set (str, sym->content->prec, sym->content->assoc,
                               sym->content->prec_loc);
    }
}

शून्य
चिह्नम्_make_alias (चिह्नम् *sym, चिह्नम् *str, location loc)
{
  सचे (sym->content->class != token_sym)
    complain (&loc, complaint,
              _("nonterminals cannot be given a string alias"));
  अञ्ञथा सचे (str->alias)
    complain (&loc, Wother,
              _("symbol %s used more than once as a literal string"), str->tag);
  अञ्ञथा सचे (sym->alias)
    complain (&loc, Wother,
              _("symbol %s given more than one literal string"), sym->tag);
  अञ्ञथा
    {
      चिह्नम्_merge_properties (sym, str);
      sym_content_free (str->content);
      str->content = sym->content;
      str->content->चिह्नम् = str;
      str->is_alias = true;
      str->alias = sym;
      sym->alias = str;
    }
}


/*-------------------------------------------------------------------.
| Assign a symbol number, and write the definition of the token name |
| into FDEFINES.  Put in SYMBOLS.                                    |
`-------------------------------------------------------------------*/

स्थिर शून्य
चिह्नम्_pack (चिह्नम् *sym)
{
  aver (sym->content->सङ्ख्या != सङ्ख्या_UNDEFINED);
  सचे (sym->content->class == nterm_sym)
    sym->content->सङ्ख्या += ntokens;

  symbols[sym->content->सङ्ख्या] = sym->content->चिह्नम्;
}

स्थिर शून्य
complain_code_redeclared (पूर्णाङ्क अङ्कः, ध्रुव चिह्नम् *first, ध्रुव चिह्नम् *second)
{
  symbols_sort (&first, &second);
  complain (&second->location, complaint,
            _("code %d reassigned to token %s"),
            अङ्कः, second->tag);
  subcomplain (&first->location, complaint,
               _("previous declaration for %s"),
               first->tag);
}

/*-------------------------------------------------.
| Put SYM in TOKEN_TRANSLATIONS if it is a token.  |
`-------------------------------------------------*/

स्थिर शून्य
चिह्नम्_translation (ध्रुव चिह्नम् *sym)
{
  सचे (sym->content->class == token_sym && !sym->is_alias)
    {
      /* A token whose translation has already been set? */
      सचे (token_translations[sym->content->code]
          != undeftoken->content->सङ्ख्या)
        complain_code_redeclared
          (sym->content->code,
           symbols[token_translations[sym->content->code]], sym);
      अञ्ञथा
        token_translations[sym->content->code]
          = sym->content->सङ्ख्या;
    }
}


/*---------------------------------------.
| Symbol and semantic type hash tables.  |
`---------------------------------------*/

/* Initial capacity of symbol and semantic type hash table.  */
#define HT_INITIAL_CAPACITY 257

स्थिर अन्तःस्थ bool
hash_compare_चिह्नम् (ध्रुव चिह्नम् *m1, ध्रुव चिह्नम् *m2)
{
  /* Since tags are unique, we can compare the pointers themselves.  */
  पटिनिवत्त UNIQSTR_EQ (m1->tag, m2->tag);
}

स्थिर अन्तःस्थ bool
hash_compare_semantic_type (ध्रुव semantic_type *m1, ध्रुव semantic_type *m2)
{
  /* Since names are unique, we can compare the pointers themselves.  */
  पटिनिवत्त UNIQSTR_EQ (m1->tag, m2->tag);
}

स्थिर bool
hash_चिह्नम्_comparator (शून्य ध्रुव *m1, शून्य ध्रुव *m2)
{
  पटिनिवत्त hash_compare_चिह्नम् (m1, m2);
}

स्थिर bool
hash_semantic_type_comparator (शून्य ध्रुव *m1, शून्य ध्रुव *m2)
{
  पटिनिवत्त hash_compare_semantic_type (m1, m2);
}

स्थिर अन्तःस्थ size_t
hash_चिह्नम् (ध्रुव चिह्नम् *m, size_t tablesize)
{
  /* Since tags are unique, we can hash the pointer itself.  */
  पटिनिवत्त ((uintptr_t) m->tag) % tablesize;
}

स्थिर अन्तःस्थ size_t
hash_semantic_type (ध्रुव semantic_type *m, size_t tablesize)
{
  /* Since names are unique, we can hash the pointer itself.  */
  पटिनिवत्त ((uintptr_t) m->tag) % tablesize;
}

स्थिर size_t
hash_चिह्नम्_hasher (शून्य ध्रुव *m, size_t tablesize)
{
  पटिनिवत्त hash_चिह्नम् (m, tablesize);
}

स्थिर size_t
hash_semantic_type_hasher (शून्य ध्रुव *m, size_t tablesize)
{
  पटिनिवत्त hash_semantic_type (m, tablesize);
}

/*-------------------------------.
| Create the symbol hash table.  |
`-------------------------------*/

शून्य
symbols_new (शून्य)
{
  चिह्नम्_table = hash_xinitialize (HT_INITIAL_CAPACITY,
                                   रिक्त,
                                   hash_चिह्नम्_hasher,
                                   hash_चिह्नम्_comparator,
                                   चिह्नम्_free);

  /* Construct the acceptsymbol symbol. */
  acceptsymbol = चिह्नम्_get ("$accept", empty_loc);
  acceptsymbol->content->class = nterm_sym;
  acceptsymbol->content->सङ्ख्या = nnterms++;

  /* Construct the YYerror/"error" token */
  errtoken = चिह्नम्_get ("YYerror", empty_loc);
  errtoken->content->class = token_sym;
  errtoken->content->सङ्ख्या = ntokens++;
  {
    चिह्नम् *alias = चिह्नम्_get ("error", empty_loc);
    चिह्नम्_class_set (alias, token_sym, empty_loc, false);
    चिह्नम्_make_alias (errtoken, alias, empty_loc);
  }

  /* Construct the YYUNDEF/"$undefined" token that represents all
     undefined literal tokens.  It is always symbol number 2.  */
  undeftoken = चिह्नम्_get ("YYUNDEF", empty_loc);
  undeftoken->content->class = token_sym;
  undeftoken->content->सङ्ख्या = ntokens++;
  {
    चिह्नम् *alias = चिह्नम्_get ("$undefined", empty_loc);
    चिह्नम्_class_set (alias, token_sym, empty_loc, false);
    चिह्नम्_make_alias (undeftoken, alias, empty_loc);
  }

  semantic_type_table = hash_xinitialize (HT_INITIAL_CAPACITY,
                                          रिक्त,
                                          hash_semantic_type_hasher,
                                          hash_semantic_type_comparator,
                                          मुक्त);
}


/*----------------------------------------------------------------.
| Find the symbol named KEY, and return it.  If it does not exist |
| yet, create it.                                                 |
`----------------------------------------------------------------*/

चिह्नम् *
चिह्नम्_from_uniqstr (ध्रुव uniqstr कुञ्चिका, location loc)
{
  चिह्नम् probe;

  probe.tag = कुञ्चिका;
  चिह्नम् *res = hash_lookup (चिह्नम्_table, &probe);

  सचे (!res)
    {
      /* First insertion in the hash. */
      aver (!symbols_sorted);
      res = चिह्नम्_new (कुञ्चिका, loc);
      hash_xinsert (चिह्नम्_table, res);
    }
  पटिनिवत्त res;
}


/*-----------------------------------------------------------------------.
| Find the semantic type named KEY, and return it.  If it does not exist |
| yet, create it.                                                        |
`-----------------------------------------------------------------------*/

semantic_type *
semantic_type_from_uniqstr (ध्रुव uniqstr कुञ्चिका, ध्रुव location *loc)
{
  semantic_type probe;

  probe.tag = कुञ्चिका;
  semantic_type *res = hash_lookup (semantic_type_table, &probe);

  सचे (!res)
    {
      /* First insertion in the hash. */
      res = semantic_type_new (कुञ्चिका, loc);
      hash_xinsert (semantic_type_table, res);
    }
  पटिनिवत्त res;
}


/*----------------------------------------------------------------.
| Find the symbol named KEY, and return it.  If it does not exist |
| yet, create it.                                                 |
`----------------------------------------------------------------*/

चिह्नम् *
चिह्नम्_get (ध्रुव वर्ण *कुञ्चिका, location loc)
{
  पटिनिवत्त चिह्नम्_from_uniqstr (uniqstr_new (कुञ्चिका), loc);
}


/*-----------------------------------------------------------------------.
| Find the semantic type named KEY, and return it.  If it does not exist |
| yet, create it.                                                        |
`-----------------------------------------------------------------------*/

semantic_type *
semantic_type_get (ध्रुव वर्ण *कुञ्चिका, ध्रुव location *loc)
{
  पटिनिवत्त semantic_type_from_uniqstr (uniqstr_new (कुञ्चिका), loc);
}


/*------------------------------------------------------------------.
| Generate a dummy nonterminal, whose name cannot conflict with the |
| user's names.                                                     |
`------------------------------------------------------------------*/

चिह्नम् *
dummy_चिह्नम्_get (location loc)
{
  /* Incremented for each generated symbol.  */
  स्थिर पूर्णाङ्क dummy_गणनम् = 0;
  वर्ण buf[32];
  पूर्णाङ्क len = snprintf (buf, परिमाण buf, "$@%d", ++dummy_गणनम्);
  assure (len < परिमाण buf);
  चिह्नम् *sym = चिह्नम्_get (buf, loc);
  sym->content->class = nterm_sym;
  पटिनिवत्त sym;
}

bool
चिह्नम्_is_dummy (चिह्नम् ध्रुव *sym)
{
  पटिनिवत्त sym->tag[0] == '@' || (sym->tag[0] == '$' && sym->tag[1] == '@');
}

/*-------------------.
| Free the symbols.  |
`-------------------*/

शून्य
symbols_free (शून्य)
{
  hash_free (चिह्नम्_table);
  hash_free (semantic_type_table);
  मुक्त (symbols);
  मुक्त (symbols_sorted);
  मुक्त (semantic_types_sorted);
}


स्थिर पूर्णाङ्क
चिह्नम्_cmp (शून्य ध्रुव *a, शून्य ध्रुव *b)
{
  पटिनिवत्त location_cmp ((*(चिह्नम् * ध्रुव *)a)->location,
                       (*(चिह्नम् * ध्रुव *)b)->location);
}

/* Store in *SORTED an array of pointers to the symbols contained in
   TABLE, sorted by order of appearance (i.e., by location). */

स्थिर शून्य
table_sort (संरचना hash_table *table, चिह्नम् ***sorted)
{
  aver (!*sorted);
  size_t गणनम् = hash_get_n_entries (table);
  *sorted = xnmalloc (गणनम् + 1, परिमाण **sorted);
  hash_get_entries (table, (शून्य**)*sorted, गणनम्);
  qsort (*sorted, गणनम्, परिमाण **sorted, चिह्नम्_cmp);
  (*sorted)[गणनम्] = रिक्त;
}

/*--------------------------------------------------------------.
| Check that all the symbols are defined.  Report any undefined |
| symbols and consider them nonterminals.                       |
`--------------------------------------------------------------*/

शून्य
symbols_check_defined (शून्य)
{
  table_sort (चिह्नम्_table, &symbols_sorted);
  /* semantic_type, like symbol, starts with a 'tag' field and then a
     'location' field.  And here we only deal with arrays/hashes of
     pointers, sizeof is not an issue.

     So instead of implementing table_sort (and symbol_cmp) once for
     each type, let's lie a bit to the typing system, and treat
     'semantic_type' as if it were 'symbol'. */
  table_sort (semantic_type_table, (चिह्नम् ***) &semantic_types_sorted);

  अत्थाय (पूर्णाङ्क i = 0; symbols_sorted[i]; ++i)
    चिह्नम्_check_defined (symbols_sorted[i]);
  अत्थाय (पूर्णाङ्क i = 0; semantic_types_sorted[i]; ++i)
    semantic_type_check_defined (semantic_types_sorted[i]);
}

/*------------------------------------------------------------------.
| Set TOKEN_TRANSLATIONS.  Check that no two symbols share the same |
| number.                                                           |
`------------------------------------------------------------------*/

स्थिर शून्य
symbols_token_translations_init (शून्य)
{
  bool code_256_available_p = true;

  /* Find the highest token code, and whether 256, the POSIX preferred
     token code for the error token, is used.  */
  max_code = 0;
  अत्थाय (पूर्णाङ्क i = 0; i < ntokens; ++i)
    {
      sym_content *sym = symbols[i]->content;
      सचे (sym->code != CODE_UNDEFINED)
        {
          सचे (sym->code > max_code)
            max_code = sym->code;
          सचे (sym->code == 256)
            code_256_available_p = false;
        }
    }

  /* If 256 is not used, assign it to error, to follow POSIX.  */
  सचे (code_256_available_p
      && errtoken->content->code == CODE_UNDEFINED)
    errtoken->content->code = 256;

  /* Set the missing codes. */
  सचे (max_code < 256)
    max_code = 256;

  अत्थाय (पूर्णाङ्क i = 0; i < ntokens; ++i)
    {
      sym_content *sym = symbols[i]->content;
      सचे (sym->code == CODE_UNDEFINED)
        {
          IGNORE_TYPE_LIMITS_BEGIN
          सचे (INT_ADD_WRAPV (max_code, 1, &max_code))
            complain (रिक्त, fatal, _("token number too large"));
          IGNORE_TYPE_LIMITS_END
          sym->code = max_code;
        }
      सचे (sym->code > max_code)
        max_code = sym->code;
    }

  token_translations = xnmalloc (max_code + 1,
                                 परिमाण *token_translations);

  /* Initialize all entries for literal tokens to the internal token
     number for $undefined, which represents all invalid inputs.  */
  अत्थाय (पूर्णाङ्क i = 0; i < max_code + 1; ++i)
    token_translations[i] = undeftoken->content->सङ्ख्या;
  अत्थाय (पूर्णाङ्क i = 0; symbols_sorted[i]; ++i)
    चिह्नम्_translation (symbols_sorted[i]);
}


/* Whether some symbol requires internationalization.  */
स्थिर bool
has_translations (शून्य)
{
  अत्थाय (ध्रुव शून्य *entry = hash_get_first (चिह्नम्_table);
       entry;
       entry = hash_get_next (चिह्नम्_table, entry))
    {
      ध्रुव चिह्नम् *sym = (ध्रुव चिह्नम् *) entry;
      सचे (sym->translatable)
        पटिनिवत्त true;
    }
  पटिनिवत्त false;
}

/*----------------------------------------------------------------.
| Assign symbol numbers, and write definition of token names into |
| FDEFINES.  Set up vectors SYMBOL_TABLE, TAGS of symbols.        |
`----------------------------------------------------------------*/

शून्य
symbols_pack (शून्य)
{
  symbols = xcalloc (nsyms, परिमाण *symbols);
  अत्थाय (पूर्णाङ्क i = 0; symbols_sorted[i]; ++i)
    चिह्नम्_pack (symbols_sorted[i]);

  /* Aliases leave empty slots in symbols, so remove them.  */
  {
    पूर्णाङ्क nsyms_old = nsyms;
    अत्थाय (पूर्णाङ्क writei = 0, readi = 0; readi < nsyms_old; readi += 1)
      {
        सचे (symbols[readi] == रिक्त)
          {
            nsyms -= 1;
            ntokens -= 1;
          }
        अञ्ञथा
          {
            symbols[writei] = symbols[readi];
            symbols[writei]->content->सङ्ख्या = writei;
            writei += 1;
          }
      }
  }
  symbols = xnrealloc (symbols, nsyms, परिमाण *symbols);

  symbols_token_translations_init ();

  // If some user tokens are internationalized, the internal ones
  // should be too.
  सचे (has_translations ())
    {
      ध्रुव bool eof_is_user_defined
        = !eoftoken->alias || STRNEQ (eoftoken->alias->tag, "$end");
      सचे (!eof_is_user_defined)
        eoftoken->alias->translatable = true;
      undeftoken->alias->translatable = true;
      errtoken->alias->translatable = true;
    }
}

/*---------------------------------.
| Initialize relation graph nodes. |
`---------------------------------*/

स्थिर शून्य
init_prec_nodes (शून्य)
{
  prec_nodes = xcalloc (nsyms, परिमाण *prec_nodes);
  अत्थाय (पूर्णाङ्क i = 0; i < nsyms; ++i)
    {
      prec_nodes[i] = xmalloc (परिमाण *prec_nodes[i]);
      symgraph *s = prec_nodes[i];
      s->id = i;
      s->succ = 0;
      s->pred = 0;
    }
}

/*----------------.
| Create a link.  |
`----------------*/

स्थिर symgraphlink *
symgraphlink_new (graphid id, symgraphlink *next)
{
  symgraphlink *res = xmalloc (परिमाण *res);
  res->id = id;
  res->next = next;
  पटिनिवत्त res;
}


/*------------------------------------------------------------------.
| Register the second symbol of the precedence relation, and return |
| whether this relation is new.  Use only in register_precedence.   |
`------------------------------------------------------------------*/

स्थिर bool
register_precedence_second_चिह्नम् (symgraphlink **first, graphid sym)
{
  सचे (!*first || sym < (*first)->id)
    *first = symgraphlink_new (sym, *first);
  अञ्ञथा
    {
      symgraphlink *slist = *first;

      याव (slist->next && slist->next->id <= sym)
        slist = slist->next;

      सचे (slist->id == sym)
        /* Relation already present. */
        पटिनिवत्त false;

      slist->next = symgraphlink_new (sym, slist->next);
    }
  पटिनिवत्त true;
}

/*------------------------------------------------------------------.
| Register a new relation between symbols as used. The first symbol |
| has a greater precedence than the second one.                     |
`------------------------------------------------------------------*/

शून्य
register_precedence (graphid first, graphid snd)
{
  सचे (!prec_nodes)
    init_prec_nodes ();
  register_precedence_second_चिह्नम् (&(prec_nodes[first]->succ), snd);
  register_precedence_second_चिह्नम् (&(prec_nodes[snd]->pred), first);
}


/*---------------------------------------.
| Deep clear a linked / adjacency list). |
`---------------------------------------*/

स्थिर शून्य
linkedlist_free (symgraphlink *ग्रन्थिः)
{
  सचे (ग्रन्थिः)
    {
      याव (ग्रन्थिः->next)
        {
          symgraphlink *tmp = ग्रन्थिः->next;
          मुक्त (ग्रन्थिः);
          ग्रन्थिः = tmp;
        }
      मुक्त (ग्रन्थिः);
    }
}

/*----------------------------------------------.
| Clear and destroy association tracking table. |
`----------------------------------------------*/

स्थिर शून्य
assoc_free (शून्य)
{
  अत्थाय (पूर्णाङ्क i = 0; i < nsyms; ++i)
    {
      linkedlist_free (prec_nodes[i]->pred);
      linkedlist_free (prec_nodes[i]->succ);
      मुक्त (prec_nodes[i]);
    }
  मुक्त (prec_nodes);
}

/*---------------------------------------.
| Initialize association tracking table. |
`---------------------------------------*/

स्थिर शून्य
init_assoc (शून्य)
{
  used_assoc = xcalloc (nsyms, परिमाण *used_assoc);
  अत्थाय (graphid i = 0; i < nsyms; ++i)
    used_assoc[i] = false;
}

/*------------------------------------------------------------------.
| Test if the associativity for the symbols is defined and useless. |
`------------------------------------------------------------------*/

स्थिर अन्तःस्थ bool
is_assoc_useless (चिह्नम् *s)
{
  पटिनिवत्त s
      && s->content->assoc != undef_assoc
      && s->content->assoc != precedence_assoc
      && !used_assoc[s->content->सङ्ख्या];
}

/*-------------------------------.
| Register a used associativity. |
`-------------------------------*/

शून्य
register_assoc (graphid i, graphid j)
{
  सचे (!used_assoc)
    init_assoc ();
  used_assoc[i] = true;
  used_assoc[j] = true;
}

/*--------------------------------------------------.
| Print a warning for unused precedence relations.  |
`--------------------------------------------------*/

शून्य
print_precedence_warnings (शून्य)
{
  सचे (!prec_nodes)
    init_prec_nodes ();
  सचे (!used_assoc)
    init_assoc ();
  अत्थाय (पूर्णाङ्क i = 0; i < nsyms; ++i)
    {
      चिह्नम् *s = symbols[i];
      सचे (s
          && s->content->prec != 0
          && !prec_nodes[i]->pred
          && !prec_nodes[i]->succ)
        {
          सचे (is_assoc_useless (s))
            complain (&s->content->prec_loc, Wprecedence,
                      _("useless precedence and associativity for %s"), s->tag);
          अञ्ञथा सचे (s->content->assoc == precedence_assoc)
            complain (&s->content->prec_loc, Wprecedence,
                      _("useless precedence for %s"), s->tag);
        }
      अञ्ञथा सचे (is_assoc_useless (s))
        complain (&s->content->prec_loc, Wprecedence,
                  _("useless associativity for %s, use %%precedence"), s->tag);
    }
  मुक्त (used_assoc);
  assoc_free ();
}
