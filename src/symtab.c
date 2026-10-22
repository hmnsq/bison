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
#परिभाषय संहिता_UNDEFINED (-1)

/* Undefined symbol number.  */
#परिभाषय सङ्ख्या_UNDEFINED (-1)


स्थिर संरचना hash_सारणी *चिह्नम्_सारणी = रिक्त;
स्थिर संरचना hash_सारणी *semantic_type_सारणी = रिक्त;

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
स्थिर bool *प्रयुक्तम्_assoc = रिक्त;

bool चिह्नकम्_आलोकितम् = false;


/* Whether SYM was defined by the user.  */

स्थिर bool
चिह्नम्_is_user_defined (चिह्नम् *sym)
{
  ध्रुव bool eof_is_user_defined
    = !eoftoken->alias || STRNEQ (eoftoken->alias->चिह्नकम्, "$end");
  प्रत्यागम sym->चिह्नकम्[0] != '$'
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
  res->type_loc = रिक्तानि_loc;
  प्रति (पूर्णाङ्क i = 0; i < संहिता_PROPS_आकार; ++i)
    संहिता_props_none_init (&res->props[i]);

  res->सङ्ख्या = सङ्ख्या_UNDEFINED;
  res->prec_loc = रिक्तानि_loc;
  res->prec = 0;
  res->assoc = undef_assoc;
  res->संहिता = संहिता_UNDEFINED;

  res->class = unknown_sym;
  res->स्थितिः = undeclared;

  प्रत्यागम res;
}

/*---------------------------------.
| Create a new symbol, named TAG.  |
`---------------------------------*/

स्थिर चिह्नम् *
चिह्नम्_new (uniqstr चिह्नकम्, location loc)
{
  चिह्नम् *res = xmalloc (परिमाण *res);
  uniqstr_assert (चिह्नकम्);

  /* If the tag is not a string (starts with a double quote), check
     that it is valid for Yacc. */
  यदि (चिह्नकम्[0] != '\"' && चिह्नकम्[0] != '\'' && strchr (चिह्नकम्, '-'))
    complain (&loc, Wyacc,
              _("POSIX Yacc forbids dashes in symbol names: %s"), चिह्नकम्);

  res->चिह्नकम् = चिह्नकम्;
  res->location = loc;
  res->translatable = false;
  res->location_सम्बन्धि_lhs = false;
  res->alias = रिक्त;
  res->content = sym_content_new (res);
  res->is_alias = false;
  प्रत्यागम res;
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
  यदि (!sym->is_alias)
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
symbols_क्रमय (ध्रुव चिह्नम् **first, ध्रुव चिह्नम् **second)
{
  यदि (0 < location_cmp ((*first)->location, (*second)->location))
    {
      ध्रुव चिह्नम्* tmp = *first;
      *first = *second;
      *second = tmp;
    }
}

/* Likewise, for locations.  */

स्थिर शून्य
locations_क्रमय (location *first, location *second)
{
  यदि (0 < location_cmp (*first, *second))
    {
      location tmp = *first;
      *first = *second;
      *second = tmp;
    }
}

वर्ण ध्रुव *
संहिता_props_type_string (संहिता_props_type विधा)
{
  विकल्प (विधा)
    {
    पक्ष destructor:
      प्रत्यागम "%destructor";
    पक्ष printer:
      प्रत्यागम "%printer";
    }
  abort ();
}


/*----------------------------------------.
| Create a new semantic type, named TAG.  |
`----------------------------------------*/

स्थिर semantic_type *
semantic_type_new (uniqstr चिह्नकम्, ध्रुव location *loc)
{
  semantic_type *res = xmalloc (परिमाण *res);

  uniqstr_assert (चिह्नकम्);
  res->चिह्नकम् = चिह्नकम्;
  res->location = loc ? *loc : रिक्तानि_loc;
  res->स्थितिः = undeclared;
  प्रति (पूर्णाङ्क i = 0; i < संहिता_PROPS_आकार; ++i)
    संहिता_props_none_init (&res->props[i]);

  प्रत्यागम res;
}


/*-----------------.
| Print a symbol.  |
`-----------------*/

#परिभाषय चिह्नम्_INT_ATTR_PRINT(Attr)                     \
  यदि (s->content)                                       \
    fprintf (f, " %s = %d", #Attr, s->content->Attr)

#परिभाषय चिह्नम्_STR_ATTR_PRINT(Attr)                     \
  यदि (s->content && s->content->Attr)                   \
    fprintf (f, " %s { %s }", #Attr, s->content->Attr)

#परिभाषय चिह्नम्_संहिता_PRINT(Attr)                                         \
  यदि (s->content && s->content->props[Attr].संहिता)                       \
    fprintf (f, " %s { %s }", #Attr, s->content->props[Attr].संहिता)

शून्य
चिह्नम्_print (चिह्नम् ध्रुव *s, FILE *f)
{
  यदि (s)
    {
      चिह्नम्_class c = s->content->class;
      fprintf (f, "%s: %s",
               c == unknown_sym    ? "unknown"
               : c == pct_type_sym ? "%type"
               : c == token_sym    ? "token"
               : c == nterm_sym    ? "nterm"
               : रिक्त, /* abort.  */
               s->चिह्नकम्);
      putc (' ', f);
      location_print (s->location, f);
      चिह्नम्_INT_ATTR_PRINT (संहिता);
      चिह्नम्_INT_ATTR_PRINT (सङ्ख्या);
      चिह्नम्_STR_ATTR_PRINT (type_name);
      चिह्नम्_संहिता_PRINT (destructor);
      चिह्नम्_संहिता_PRINT (printer);
    }
  अन्यथा
    fputs ("<NULL>", f);
}

#अपरिभाषय चिह्नम्_ATTR_PRINT
#अपरिभाषय चिह्नम्_संहिता_PRINT


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
  यदि (!s || ! memchr (alphanum, *s, परिमाण alphanum - 10))
    प्रत्यागम false;
  प्रति (++s; *s; ++s)
    यदि (! memchr (alphanum, *s, परिमाण alphanum))
      प्रत्यागम false;
  प्रत्यागम true;
}


/*-----------------------------------------------.
| Get the identifier associated to this symbol.  |
`-----------------------------------------------*/

uniqstr
चिह्नम्_id_get (चिह्नम् ध्रुव *sym)
{
  // There's one weird case: YYerror is the alias, and error is the
  // base symbol.  Return YYerror in that case.
  यदि (sym->alias && is_identifier (sym->alias->चिह्नकम्))
    प्रत्यागम sym->alias->चिह्नकम्;
  अन्यथा यदि (is_identifier (sym->चिह्नकम्))
    प्रत्यागम sym->चिह्नकम्;
  अन्यथा
    प्रत्यागम रिक्त;
}


/*------------------------------------------------------------------.
| Complain that S's WHAT is redeclared at SECOND, and was first set |
| at FIRST.                                                         |
`------------------------------------------------------------------*/

स्थिर शून्य
complain_चिह्नम्_redeclared (चिह्नम् *s, ध्रुव वर्ण *what, location first,
                            location second)
{
  locations_क्रमय (&first, &second);
  complain (&second, complaint, _("%s redeclaration for %s"), what, s->चिह्नकम्);
  subcomplain (&first, complaint, _("previous declaration"));
}

स्थिर शून्य
complain_semantic_type_redeclared (semantic_type *s, ध्रुव वर्ण *what, location first,
                                   location second)
{
  locations_क्रमय (&first, &second);
  complain (&second, complaint, _("%s redeclaration for <%s>"), what, s->चिह्नकम्);
  subcomplain (&first, complaint, _("previous declaration"));
}

स्थिर शून्य
complain_class_redeclared (चिह्नम् *sym, चिह्नम्_class class, location second)
{
  complain (&second, complaint,
            class == token_sym
            ? _("symbol %s redeclared as a token")
            : _("symbol %s redeclared as a nonterminal"), sym->चिह्नकम्);
  यदि (!location_रिक्तानि (sym->location))
    subcomplain (&sym->location, complaint, _("previous definition"));
}

स्थिर ध्रुव चिह्नम् *
चिह्नम्_from_uniqstr_fuzzy (ध्रुव uniqstr कुञ्चिका)
{
  aver (symbols_sorted);
#परिभाषय FSTRCMP_देहली 0.6
  द्विगुण best_similarity = FSTRCMP_देहली;
  ध्रुव चिह्नम् *res = रिक्त;
  size_t गणनम् = hash_get_n_entries (चिह्नम्_सारणी);
  प्रति (पूर्णाङ्क i = 0; i < गणनम्; ++i)
    {
      चिह्नम् *sym = symbols_sorted[i];
      यदि (STRNEQ (कुञ्चिका, sym->चिह्नकम्)
          && (sym->content->स्थितिः == declared
              || sym->content->स्थितिः == undeclared))
        {
          द्विगुण similarity = fstrcmp_bounded (कुञ्चिका, sym->चिह्नकम्, best_similarity);
          यदि (best_similarity < similarity)
            {
              res = sym;
              best_similarity = similarity;
            }
        }
    }
  प्रत्यागम res;
}

स्थिर शून्य
complain_चिह्नम्_undeclared (ध्रुव चिह्नम् *sym)
{
  प्रतिज्ञा (sym->content->स्थितिः != declared);
  ध्रुव चिह्नम् *best = चिह्नम्_from_uniqstr_fuzzy (sym->चिह्नकम्);
  यदि (best)
    {
      complain (&sym->location,
                sym->content->स्थितिः == needed ? complaint : Wother,
                _("symbol %s is used, but is not defined as a token"
                  " and has no rules; did you mean %s?"),
                quote_n (0, sym->चिह्नकम्),
                quote_n (1, best->चिह्नकम्));
      यदि (feature_flag & feature_caret)
        location_caret_suggestion (sym->location, best->चिह्नकम्, stderr);
    }
  अन्यथा
    complain (&sym->location,
              sym->content->स्थितिः == needed ? complaint : Wother,
              _("symbol %s is used, but is not defined as a token"
                " and has no rules"),
              quote (sym->चिह्नकम्));
}

शून्य
चिह्नम्_location_as_lhs_समुच्चयः (चिह्नम् *sym, location loc)
{
  यदि (!sym->location_सम्बन्धि_lhs)
    {
      sym->location = loc;
      sym->location_सम्बन्धि_lhs = true;
    }
}


/*-----------------------------------------------------------------.
| Set the TYPE_NAME associated with SYM.  Does nothing if passed 0 |
| as TYPE_NAME.                                                    |
`-----------------------------------------------------------------*/

शून्य
चिह्नम्_type_समुच्चयः (चिह्नम् *sym, uniqstr type_name, location loc)
{
  यदि (type_name)
    {
      चिह्नकम्_आलोकितम् = true;
      यदि (sym->content->type_name)
        complain_चिह्नम्_redeclared (sym, "%type",
                                    sym->content->type_loc, loc);
      अन्यथा
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
चिह्नम्_संहिता_props_समुच्चयः (चिह्नम् *sym, संहिता_props_type विधा,
                       संहिता_props ध्रुव *संहिता)
{
  यदि (sym->content->props[विधा].संहिता)
    complain_चिह्नम्_redeclared (sym, संहिता_props_type_string (विधा),
                                sym->content->props[विधा].location,
                                संहिता->location);
  अन्यथा
    sym->content->props[विधा] = *संहिता;
}


/*-----------------------------------------------------.
| Set the DESTRUCTOR or PRINTER associated with TYPE.  |
`-----------------------------------------------------*/

शून्य
semantic_type_संहिता_props_समुच्चयः (semantic_type *type,
                              संहिता_props_type विधा,
                              संहिता_props ध्रुव *संहिता)
{
  यदि (type->props[विधा].संहिता)
    complain_semantic_type_redeclared (type, संहिता_props_type_string (विधा),
                                       type->props[विधा].location,
                                       संहिता->location);
  अन्यथा
    type->props[विधा] = *संहिता;
}

/*---------------------------------------------------.
| Get the computed %destructor or %printer for SYM.  |
`---------------------------------------------------*/

संहिता_props *
चिह्नम्_संहिता_props_get (चिह्नम् *sym, संहिता_props_type विधा)
{
  /* Per-symbol code props.  */
  यदि (sym->content->props[विधा].संहिता)
    प्रत्यागम &sym->content->props[विधा];

  /* Per-type code props.  */
  यदि (sym->content->type_name)
    {
      संहिता_props *संहिता =
        &semantic_type_get (sym->content->type_name, रिक्त)->props[विधा];
      यदि (संहिता->संहिता)
        प्रत्यागम संहिता;
    }

  /* Apply default code props's only to user-defined symbols.  */
  यदि (चिह्नम्_is_user_defined (sym))
    {
      संहिता_props *संहिता = &semantic_type_get (sym->content->type_name ? "*" : "",
                                             रिक्त)->props[विधा];
      यदि (संहिता->संहिता)
        प्रत्यागम संहिता;
    }
  प्रत्यागम &संहिता_props_none;
}

/*-----------------------------------------------------------------.
| Set the PRECEDENCE associated with SYM.  Does nothing if invoked |
| with UNDEF_ASSOC as ASSOC.                                       |
`-----------------------------------------------------------------*/

शून्य
चिह्नम्_precedence_समुच्चयः (चिह्नम् *sym, पूर्णाङ्क prec, assoc a, location loc)
{
  यदि (a != undef_assoc)
    {
      sym_content *s = sym->content;
      यदि (s->prec)
        complain_चिह्नम्_redeclared (sym, assoc_गन्तव्यम्_string (a),
                                    s->prec_loc, loc);
      अन्यथा
        {
          s->prec = prec;
          s->assoc = a;
          s->prec_loc = loc;
        }
    }

  /* Only terminals have a precedence. */
  चिह्नम्_class_समुच्चयः (sym, token_sym, loc, false);
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
चिह्नम्_class_समुच्चयः (चिह्नम् *sym, चिह्नम्_class class, location loc, bool declaring)
{
  aver (class != unknown_sym);
  sym_content *s = sym->content;
  यदि (class == pct_type_sym)
    {
      यदि (s->class == token_sym)
        complain_pct_type_on_token (&loc);
      अन्यथा यदि (s->class == unknown_sym)
        s->class = class;
    }
  अन्यथा यदि (s->class != unknown_sym && s->class != pct_type_sym
           && s->class != class)
    complain_class_redeclared (sym, class, loc);
  अन्यथा
    {
      यदि (class == token_sym && s->class == pct_type_sym)
        complain_pct_type_on_token (&sym->location);

      s->class = class;

      यदि (declaring)
        {
          यदि (s->स्थितिः == declared)
            {
              complain (&loc, Wother,
                        _("symbol %s redeclared"), sym->चिह्नकम्);
              subcomplain (&sym->location, Wother,
                           _("previous declaration"));
            }
          अन्यथा
            {
              sym->location = loc;
              s->स्थितिः = declared;
            }
        }
    }
}


/*----------------------------.
| Set the token code of SYM.  |
`----------------------------*/

शून्य
चिह्नम्_संहिता_समुच्चयः (चिह्नम् *sym, पूर्णाङ्क संहिता, location loc)
{
  पूर्णाङ्क *codep = &sym->content->संहिता;
  यदि (sym->content->class != token_sym)
    complain (&loc, complaint,
              _("nonterminals cannot be given a token code"));
  अन्यथा यदि (*codep != संहिता_UNDEFINED
           && *codep != संहिता)
    complain (&loc, complaint, _("redefining code of token %s"),
              sym->चिह्नकम्);
  अन्यथा यदि (संहिता == अधिकतम_पूर्णाङ्कः)
    complain (&loc, complaint, _("code of token %s too large"),
              sym->चिह्नकम्);
  अन्यथा
    {
      *codep = संहिता;
      /* User defined $end token? */
      यदि (संहिता == 0 && !eoftoken)
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
  यदि (s->class == unknown_sym || s->class == pct_type_sym)
    {
      complain_चिह्नम्_undeclared (sym);
      s->class = nterm_sym;
    }

  यदि (s->सङ्ख्या == सङ्ख्या_UNDEFINED)
    s->सङ्ख्या = s->class == token_sym ? ntokens++ : nnterms++;

  यदि (s->class == token_sym
      && sym->चिह्नकम्[0] == '"'
      && !sym->is_alias)
    complain (&sym->location, Wdangling_alias,
              _("string literal %s not attached to a symbol"),
              sym->चिह्नकम्);

  प्रति (पूर्णाङ्क i = 0; i < 2; ++i)
    चिह्नम्_संहिता_props_get (sym, i)->is_प्रयुक्तम् = true;

  /* Set the semantic type status associated to the current symbol to
     'declared' so that we could check semantic types unnecessary uses. */
  यदि (s->type_name)
    {
      semantic_type *sem_type = semantic_type_get (s->type_name, रिक्त);
      यदि (sem_type)
        sem_type->स्थितिः = declared;
    }
}

स्थिर शून्य
semantic_type_check_defined (semantic_type *sem_type)
{
  /* <*> and <> do not have to be "declared".  */
  यदि (sem_type->स्थितिः == declared
      || !*sem_type->चिह्नकम्
      || STREQ (sem_type->चिह्नकम्, "*"))
    {
      प्रति (पूर्णाङ्क i = 0; i < 2; ++i)
        यदि (sem_type->props[i].विधा != संहिता_PROPS_NONE
            && ! sem_type->props[i].is_प्रयुक्तम्)
          complain (&sem_type->location, Wother,
                    _("useless %s for type <%s>"),
                    संहिता_props_type_string (i), sem_type->चिह्नकम्);
    }
  अन्यथा
    complain (&sem_type->location, Wother,
              _("type <%s> is used, but is not associated to any symbol"),
              sem_type->चिह्नकम्);
}

/*-------------------------------------------------------------------.
| Merge the properties (precedence, associativity, etc.) of SYM, and |
| its string-named alias STR; check consistency.                     |
`-------------------------------------------------------------------*/

स्थिर शून्य
चिह्नम्_merge_properties (चिह्नम् *sym, चिह्नम् *str)
{
  यदि (str->content->type_name != sym->content->type_name)
    {
      यदि (str->content->type_name)
        चिह्नम्_type_समुच्चयः (sym,
                         str->content->type_name, str->content->type_loc);
      अन्यथा
        चिह्नम्_type_समुच्चयः (str,
                         sym->content->type_name, sym->content->type_loc);
    }


  प्रति (पूर्णाङ्क i = 0; i < संहिता_PROPS_आकार; ++i)
    यदि (str->content->props[i].संहिता)
      चिह्नम्_संहिता_props_समुच्चयः (sym, i, &str->content->props[i]);
    अन्यथा यदि (sym->content->props[i].संहिता)
      चिह्नम्_संहिता_props_समुच्चयः (str, i, &sym->content->props[i]);

  यदि (sym->content->prec || str->content->prec)
    {
      यदि (str->content->prec)
        चिह्नम्_precedence_समुच्चयः (sym, str->content->prec, str->content->assoc,
                               str->content->prec_loc);
      अन्यथा
        चिह्नम्_precedence_समुच्चयः (str, sym->content->prec, sym->content->assoc,
                               sym->content->prec_loc);
    }
}

शून्य
चिह्नम्_make_alias (चिह्नम् *sym, चिह्नम् *str, location loc)
{
  यदि (sym->content->class != token_sym)
    complain (&loc, complaint,
              _("nonterminals cannot be given a string alias"));
  अन्यथा यदि (str->alias)
    complain (&loc, Wother,
              _("symbol %s used more than once as a literal string"), str->चिह्नकम्);
  अन्यथा यदि (sym->alias)
    complain (&loc, Wother,
              _("symbol %s given more than one literal string"), sym->चिह्नकम्);
  अन्यथा
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
  यदि (sym->content->class == nterm_sym)
    sym->content->सङ्ख्या += ntokens;

  symbols[sym->content->सङ्ख्या] = sym->content->चिह्नम्;
}

स्थिर शून्य
complain_संहिता_redeclared (पूर्णाङ्क अङ्कः, ध्रुव चिह्नम् *first, ध्रुव चिह्नम् *second)
{
  symbols_क्रमय (&first, &second);
  complain (&second->location, complaint,
            _("code %d reassigned to token %s"),
            अङ्कः, second->चिह्नकम्);
  subcomplain (&first->location, complaint,
               _("previous declaration for %s"),
               first->चिह्नकम्);
}

/*-------------------------------------------------.
| Put SYM in TOKEN_TRANSLATIONS if it is a token.  |
`-------------------------------------------------*/

स्थिर शून्य
चिह्नम्_translation (ध्रुव चिह्नम् *sym)
{
  यदि (sym->content->class == token_sym && !sym->is_alias)
    {
      /* A token whose translation has already been set? */
      यदि (token_translations[sym->content->संहिता]
          != undeftoken->content->सङ्ख्या)
        complain_संहिता_redeclared
          (sym->content->संहिता,
           symbols[token_translations[sym->content->संहिता]], sym);
      अन्यथा
        token_translations[sym->content->संहिता]
          = sym->content->सङ्ख्या;
    }
}


/*---------------------------------------.
| Symbol and semantic type hash tables.  |
`---------------------------------------*/

/* Initial capacity of symbol and semantic type hash table.  */
#परिभाषय HT_INITIAL_क्षमता 257

स्थिर अन्तःस्थ bool
hash_compare_चिह्नम् (ध्रुव चिह्नम् *m1, ध्रुव चिह्नम् *m2)
{
  /* Since tags are unique, we can compare the pointers themselves.  */
  प्रत्यागम UNIQSTR_EQ (m1->चिह्नकम्, m2->चिह्नकम्);
}

स्थिर अन्तःस्थ bool
hash_compare_semantic_type (ध्रुव semantic_type *m1, ध्रुव semantic_type *m2)
{
  /* Since names are unique, we can compare the pointers themselves.  */
  प्रत्यागम UNIQSTR_EQ (m1->चिह्नकम्, m2->चिह्नकम्);
}

स्थिर bool
hash_चिह्नम्_comparator (शून्य ध्रुव *m1, शून्य ध्रुव *m2)
{
  प्रत्यागम hash_compare_चिह्नम् (m1, m2);
}

स्थिर bool
hash_semantic_type_comparator (शून्य ध्रुव *m1, शून्य ध्रुव *m2)
{
  प्रत्यागम hash_compare_semantic_type (m1, m2);
}

स्थिर अन्तःस्थ size_t
hash_चिह्नम् (ध्रुव चिह्नम् *m, size_t tablesize)
{
  /* Since tags are unique, we can hash the pointer itself.  */
  प्रत्यागम ((uintptr_t) m->चिह्नकम्) % tablesize;
}

स्थिर अन्तःस्थ size_t
hash_semantic_type (ध्रुव semantic_type *m, size_t tablesize)
{
  /* Since names are unique, we can hash the pointer itself.  */
  प्रत्यागम ((uintptr_t) m->चिह्नकम्) % tablesize;
}

स्थिर size_t
hash_चिह्नम्_hasher (शून्य ध्रुव *m, size_t tablesize)
{
  प्रत्यागम hash_चिह्नम् (m, tablesize);
}

स्थिर size_t
hash_semantic_type_hasher (शून्य ध्रुव *m, size_t tablesize)
{
  प्रत्यागम hash_semantic_type (m, tablesize);
}

/*-------------------------------.
| Create the symbol hash table.  |
`-------------------------------*/

शून्य
symbols_new (शून्य)
{
  चिह्नम्_सारणी = hash_xinitialize (HT_INITIAL_क्षमता,
                                   रिक्त,
                                   hash_चिह्नम्_hasher,
                                   hash_चिह्नम्_comparator,
                                   चिह्नम्_free);

  /* Construct the acceptsymbol symbol. */
  acceptsymbol = चिह्नम्_get ("$accept", रिक्तानि_loc);
  acceptsymbol->content->class = nterm_sym;
  acceptsymbol->content->सङ्ख्या = nnterms++;

  /* Construct the YYerror/"error" token */
  errtoken = चिह्नम्_get ("YYerror", रिक्तानि_loc);
  errtoken->content->class = token_sym;
  errtoken->content->सङ्ख्या = ntokens++;
  {
    चिह्नम् *alias = चिह्नम्_get ("error", रिक्तानि_loc);
    चिह्नम्_class_समुच्चयः (alias, token_sym, रिक्तानि_loc, false);
    चिह्नम्_make_alias (errtoken, alias, रिक्तानि_loc);
  }

  /* Construct the YYUNDEF/"$undefined" token that represents all
     undefined literal tokens.  It is always symbol number 2.  */
  undeftoken = चिह्नम्_get ("YYUNDEF", रिक्तानि_loc);
  undeftoken->content->class = token_sym;
  undeftoken->content->सङ्ख्या = ntokens++;
  {
    चिह्नम् *alias = चिह्नम्_get ("$undefined", रिक्तानि_loc);
    चिह्नम्_class_समुच्चयः (alias, token_sym, रिक्तानि_loc, false);
    चिह्नम्_make_alias (undeftoken, alias, रिक्तानि_loc);
  }

  semantic_type_सारणी = hash_xinitialize (HT_INITIAL_क्षमता,
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

  probe.चिह्नकम् = कुञ्चिका;
  चिह्नम् *res = hash_lookup (चिह्नम्_सारणी, &probe);

  यदि (!res)
    {
      /* First insertion in the hash. */
      aver (!symbols_sorted);
      res = चिह्नम्_new (कुञ्चिका, loc);
      hash_xinsert (चिह्नम्_सारणी, res);
    }
  प्रत्यागम res;
}


/*-----------------------------------------------------------------------.
| Find the semantic type named KEY, and return it.  If it does not exist |
| yet, create it.                                                        |
`-----------------------------------------------------------------------*/

semantic_type *
semantic_type_from_uniqstr (ध्रुव uniqstr कुञ्चिका, ध्रुव location *loc)
{
  semantic_type probe;

  probe.चिह्नकम् = कुञ्चिका;
  semantic_type *res = hash_lookup (semantic_type_सारणी, &probe);

  यदि (!res)
    {
      /* First insertion in the hash. */
      res = semantic_type_new (कुञ्चिका, loc);
      hash_xinsert (semantic_type_सारणी, res);
    }
  प्रत्यागम res;
}


/*----------------------------------------------------------------.
| Find the symbol named KEY, and return it.  If it does not exist |
| yet, create it.                                                 |
`----------------------------------------------------------------*/

चिह्नम् *
चिह्नम्_get (ध्रुव वर्ण *कुञ्चिका, location loc)
{
  प्रत्यागम चिह्नम्_from_uniqstr (uniqstr_new (कुञ्चिका), loc);
}


/*-----------------------------------------------------------------------.
| Find the semantic type named KEY, and return it.  If it does not exist |
| yet, create it.                                                        |
`-----------------------------------------------------------------------*/

semantic_type *
semantic_type_get (ध्रुव वर्ण *कुञ्चिका, ध्रुव location *loc)
{
  प्रत्यागम semantic_type_from_uniqstr (uniqstr_new (कुञ्चिका), loc);
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
  प्रत्यागम sym;
}

bool
चिह्नम्_is_dummy (चिह्नम् ध्रुव *sym)
{
  प्रत्यागम sym->चिह्नकम्[0] == '@' || (sym->चिह्नकम्[0] == '$' && sym->चिह्नकम्[1] == '@');
}

/*-------------------.
| Free the symbols.  |
`-------------------*/

शून्य
symbols_free (शून्य)
{
  hash_free (चिह्नम्_सारणी);
  hash_free (semantic_type_सारणी);
  मुक्त (symbols);
  मुक्त (symbols_sorted);
  मुक्त (semantic_types_sorted);
}


स्थिर पूर्णाङ्क
चिह्नम्_cmp (शून्य ध्रुव *a, शून्य ध्रुव *b)
{
  प्रत्यागम location_cmp ((*(चिह्नम् * ध्रुव *)a)->location,
                       (*(चिह्नम् * ध्रुव *)b)->location);
}

/* Store in *SORTED an array of pointers to the symbols contained in
   TABLE, sorted by order of appearance (i.e., by location). */

स्थिर शून्य
सारणी_क्रमय (संरचना hash_सारणी *सारणी, चिह्नम् ***sorted)
{
  aver (!*sorted);
  size_t गणनम् = hash_get_n_entries (सारणी);
  *sorted = xnmalloc (गणनम् + 1, परिमाण **sorted);
  hash_get_entries (सारणी, (शून्य**)*sorted, गणनम्);
  शीघ्रं_क्रमय (*sorted, गणनम्, परिमाण **sorted, चिह्नम्_cmp);
  (*sorted)[गणनम्] = रिक्त;
}

/*--------------------------------------------------------------.
| Check that all the symbols are defined.  Report any undefined |
| symbols and consider them nonterminals.                       |
`--------------------------------------------------------------*/

शून्य
symbols_check_defined (शून्य)
{
  सारणी_क्रमय (चिह्नम्_सारणी, &symbols_sorted);
  /* semantic_type, like symbol, starts with a 'tag' field and then a
     'location' field.  And here we only deal with arrays/hashes of
     pointers, sizeof is not an issue.

     So instead of implementing table_sort (and symbol_cmp) once for
     each type, let's lie a bit to the typing system, and treat
     'semantic_type' as if it were 'symbol'. */
  सारणी_क्रमय (semantic_type_सारणी, (चिह्नम् ***) &semantic_types_sorted);

  प्रति (पूर्णाङ्क i = 0; symbols_sorted[i]; ++i)
    चिह्नम्_check_defined (symbols_sorted[i]);
  प्रति (पूर्णाङ्क i = 0; semantic_types_sorted[i]; ++i)
    semantic_type_check_defined (semantic_types_sorted[i]);
}

/*------------------------------------------------------------------.
| Set TOKEN_TRANSLATIONS.  Check that no two symbols share the same |
| number.                                                           |
`------------------------------------------------------------------*/

स्थिर शून्य
symbols_token_translations_init (शून्य)
{
  bool संहिता_256_उपलब्धम्_p = true;

  /* Find the highest token code, and whether 256, the POSIX preferred
     token code for the error token, is used.  */
  max_संहिता = 0;
  प्रति (पूर्णाङ्क i = 0; i < ntokens; ++i)
    {
      sym_content *sym = symbols[i]->content;
      यदि (sym->संहिता != संहिता_UNDEFINED)
        {
          यदि (sym->संहिता > max_संहिता)
            max_संहिता = sym->संहिता;
          यदि (sym->संहिता == 256)
            संहिता_256_उपलब्धम्_p = false;
        }
    }

  /* If 256 is not used, assign it to error, to follow POSIX.  */
  यदि (संहिता_256_उपलब्धम्_p
      && errtoken->content->संहिता == संहिता_UNDEFINED)
    errtoken->content->संहिता = 256;

  /* Set the missing codes. */
  यदि (max_संहिता < 256)
    max_संहिता = 256;

  प्रति (पूर्णाङ्क i = 0; i < ntokens; ++i)
    {
      sym_content *sym = symbols[i]->content;
      यदि (sym->संहिता == संहिता_UNDEFINED)
        {
          IGNORE_TYPE_LIMITS_BEGIN
          यदि (INT_सङ्कलय_WRAPV (max_संहिता, 1, &max_संहिता))
            complain (रिक्त, fatal, _("token number too large"));
          IGNORE_TYPE_LIMITS_अन्तः
          sym->संहिता = max_संहिता;
        }
      यदि (sym->संहिता > max_संहिता)
        max_संहिता = sym->संहिता;
    }

  token_translations = xnmalloc (max_संहिता + 1,
                                 परिमाण *token_translations);

  /* Initialize all entries for literal tokens to the internal token
     number for $undefined, which represents all invalid inputs.  */
  प्रति (पूर्णाङ्क i = 0; i < max_संहिता + 1; ++i)
    token_translations[i] = undeftoken->content->सङ्ख्या;
  प्रति (पूर्णाङ्क i = 0; symbols_sorted[i]; ++i)
    चिह्नम्_translation (symbols_sorted[i]);
}


/* Whether some symbol requires internationalization.  */
स्थिर bool
has_translations (शून्य)
{
  प्रति (ध्रुव शून्य *प्रविष्टिः = hash_get_first (चिह्नम्_सारणी);
       प्रविष्टिः;
       प्रविष्टिः = hash_get_अग्र (चिह्नम्_सारणी, प्रविष्टिः))
    {
      ध्रुव चिह्नम् *sym = (ध्रुव चिह्नम् *) प्रविष्टिः;
      यदि (sym->translatable)
        प्रत्यागम true;
    }
  प्रत्यागम false;
}

/*----------------------------------------------------------------.
| Assign symbol numbers, and write definition of token names into |
| FDEFINES.  Set up vectors SYMBOL_TABLE, TAGS of symbols.        |
`----------------------------------------------------------------*/

शून्य
symbols_pack (शून्य)
{
  symbols = xcalloc (nsyms, परिमाण *symbols);
  प्रति (पूर्णाङ्क i = 0; symbols_sorted[i]; ++i)
    चिह्नम्_pack (symbols_sorted[i]);

  /* Aliases leave empty slots in symbols, so remove them.  */
  {
    पूर्णाङ्क nsyms_old = nsyms;
    प्रति (पूर्णाङ्क writei = 0, readi = 0; readi < nsyms_old; readi += 1)
      {
        यदि (symbols[readi] == रिक्त)
          {
            nsyms -= 1;
            ntokens -= 1;
          }
        अन्यथा
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
  यदि (has_translations ())
    {
      ध्रुव bool eof_is_user_defined
        = !eoftoken->alias || STRNEQ (eoftoken->alias->चिह्नकम्, "$end");
      यदि (!eof_is_user_defined)
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
  प्रति (पूर्णाङ्क i = 0; i < nsyms; ++i)
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
symgraphlink_new (graphid id, symgraphlink *अग्र)
{
  symgraphlink *res = xmalloc (परिमाण *res);
  res->id = id;
  res->अग्र = अग्र;
  प्रत्यागम res;
}


/*------------------------------------------------------------------.
| Register the second symbol of the precedence relation, and return |
| whether this relation is new.  Use only in register_precedence.   |
`------------------------------------------------------------------*/

स्थिर bool
पञ्जिकाकोशः_precedence_second_चिह्नम् (symgraphlink **first, graphid sym)
{
  यदि (!*first || sym < (*first)->id)
    *first = symgraphlink_new (sym, *first);
  अन्यथा
    {
      symgraphlink *slist = *first;

      यावत् (slist->अग्र && slist->अग्र->id <= sym)
        slist = slist->अग्र;

      यदि (slist->id == sym)
        /* Relation already present. */
        प्रत्यागम false;

      slist->अग्र = symgraphlink_new (sym, slist->अग्र);
    }
  प्रत्यागम true;
}

/*------------------------------------------------------------------.
| Register a new relation between symbols as used. The first symbol |
| has a greater precedence than the second one.                     |
`------------------------------------------------------------------*/

शून्य
पञ्जिकाकोशः_precedence (graphid first, graphid snd)
{
  यदि (!prec_nodes)
    init_prec_nodes ();
  पञ्जिकाकोशः_precedence_second_चिह्नम् (&(prec_nodes[first]->succ), snd);
  पञ्जिकाकोशः_precedence_second_चिह्नम् (&(prec_nodes[snd]->pred), first);
}


/*---------------------------------------.
| Deep clear a linked / adjacency list). |
`---------------------------------------*/

स्थिर शून्य
linkedlist_free (symgraphlink *ग्रन्थिः)
{
  यदि (ग्रन्थिः)
    {
      यावत् (ग्रन्थिः->अग्र)
        {
          symgraphlink *tmp = ग्रन्थिः->अग्र;
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
  प्रति (पूर्णाङ्क i = 0; i < nsyms; ++i)
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
  प्रयुक्तम्_assoc = xcalloc (nsyms, परिमाण *प्रयुक्तम्_assoc);
  प्रति (graphid i = 0; i < nsyms; ++i)
    प्रयुक्तम्_assoc[i] = false;
}

/*------------------------------------------------------------------.
| Test if the associativity for the symbols is defined and useless. |
`------------------------------------------------------------------*/

स्थिर अन्तःस्थ bool
is_assoc_useless (चिह्नम् *s)
{
  प्रत्यागम s
      && s->content->assoc != undef_assoc
      && s->content->assoc != precedence_assoc
      && !प्रयुक्तम्_assoc[s->content->सङ्ख्या];
}

/*-------------------------------.
| Register a used associativity. |
`-------------------------------*/

शून्य
पञ्जिकाकोशः_assoc (graphid i, graphid j)
{
  यदि (!प्रयुक्तम्_assoc)
    init_assoc ();
  प्रयुक्तम्_assoc[i] = true;
  प्रयुक्तम्_assoc[j] = true;
}

/*--------------------------------------------------.
| Print a warning for unused precedence relations.  |
`--------------------------------------------------*/

शून्य
print_precedence_warnings (शून्य)
{
  यदि (!prec_nodes)
    init_prec_nodes ();
  यदि (!प्रयुक्तम्_assoc)
    init_assoc ();
  प्रति (पूर्णाङ्क i = 0; i < nsyms; ++i)
    {
      चिह्नम् *s = symbols[i];
      यदि (s
          && s->content->prec != 0
          && !prec_nodes[i]->pred
          && !prec_nodes[i]->succ)
        {
          यदि (is_assoc_useless (s))
            complain (&s->content->prec_loc, Wprecedence,
                      _("useless precedence and associativity for %s"), s->चिह्नकम्);
          अन्यथा यदि (s->content->assoc == precedence_assoc)
            complain (&s->content->prec_loc, Wprecedence,
                      _("useless precedence for %s"), s->चिह्नकम्);
        }
      अन्यथा यदि (is_assoc_useless (s))
        complain (&s->content->prec_loc, Wprecedence,
                  _("useless associativity for %s, use %%precedence"), s->चिह्नकम्);
    }
  मुक्त (प्रयुक्तम्_assoc);
  assoc_free ();
}
