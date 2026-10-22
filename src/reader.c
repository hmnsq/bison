/* Input parser for Bison

   Copyright (C) 1984, 1986, 1989, 1992, 1998, 2000-2003, 2005-2007,
   2009-2015, 2018-2022 Free Software Foundation, Inc.

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
#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ <quote.h>
#ಅನ್ತರ್ಭಾವ <vasnprintf.h>

#ಅನ್ತರ್ಭಾವ "complain.h"
#ಅನ್ತರ್ಭಾವ "conflicts.h"
#ಅನ್ತರ್ಭಾವ "files.h"
#ಅನ್ತರ್ಭಾವ "fixits.h"
#ಅನ್ತರ್ಭಾವ "getargs.h"
#ಅನ್ತರ್ಭಾವ "gram.h"
#ಅನ್ತರ್ಭಾವ "muscle-tab.h"
#ಅನ್ತರ್ಭಾವ "reader.h"
#ಅನ್ತರ್ಭಾವ "symlist.h"
#ಅನ್ತರ್ಭಾವ "symtab.h"
#ಅನ್ತರ್ಭಾವ "scan-gram.h"
#ಅನ್ತರ್ಭಾವ "scan-code.h"

ಸ್ಥಿರ ಶೂನ್ಯ prepare_percent_define_front_ಕೊನೆ_variables (ಶೂನ್ಯ);
ಸ್ಥಿರ ಶೂನ್ಯ check_and_convert_grammar (ಶೂನ್ಯ);

ಸ್ಥಿರ ಚಿಹ್ನೆ_list *grammar = ಖಾಲಿ;
ಚಿಹ್ನೆ_list *ಆರಂಭ_symbols = ಖಾಲಿ;
merger_list *ವಿಲೀನ_functions = ಖಾಲಿ;

/* Was %union seen?  */
bool union_ನೋಡಿದ = false;

/* Should rules have a default precedence?  */
bool default_prec = true;


ಶೂನ್ಯ
grammar_ಆರಂಭ_symbols_ಕೂಡಿಸು (ಚಿಹ್ನೆ_list *syms)
{
  /* Report and ignore duplicates.  Append the others to START_SYMBOLS.  */
  ಚಿಹ್ನೆ_list *ಕೊನೆಯ = ಚಿಹ್ನೆ_list_ಕೊನೆಯ (ಆರಂಭ_symbols);
  ಗಾಗಿ (ಚಿಹ್ನೆ_list *l = syms; l && l->content.sym; /* nothing */)
    {
      /* Is there a previous definition?  */
      ಚಿಹ್ನೆ_list *ಮೊದಲ = ಚಿಹ್ನೆ_list_ಕಂಡುಹಿಡಿ_ಚಿಹ್ನೆ (ಆರಂಭ_symbols, l->content.sym);
      ಆದರೆ (ಮೊದಲ)
        {
          duplicate_directive ("%start", ಮೊದಲ->sym_loc, l->sym_loc);
          ಚಿಹ್ನೆ_list *dupl = l;
          l = l->ಮುಂದಿನ;
          dupl->ಮುಂದಿನ = ಖಾಲಿ;
          ಚಿಹ್ನೆ_list_free (dupl);
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಆದರೆ (ಕೊನೆಯ)
            {
              ಕೊನೆಯ->ಮುಂದಿನ = l;
              ಕೊನೆಯ = l;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              ಕೊನೆಯ = l;
              ಆರಂಭ_symbols = ಕೊನೆಯ;
            }
          ಚಿಹ್ನೆ_list *ಮುಂದಿನ = l->ಮುಂದಿನ;
          l->ಮುಂದಿನ = ಖಾಲಿ;
          l = ಮುಂದಿನ;
        }
    }
}



/*------------------------------------------------------------------------.
| Return the merger index for a merging function named NAME.  Records the |
| function, if new, in MERGER_LIST.                                       |
`------------------------------------------------------------------------*/

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
get_ವಿಲೀನ_function (uniqstr name)
{
  ಆದರೆ (! glr_parser)
    ಹಿಂತಿರುಗಿ 0;

  merger_list *syms;
  merger_list ತಲೆ;
  ಪೂರ್ಣಾಂಕ n;

  ತಲೆ.ಮುಂದಿನ = ವಿಲೀನ_functions;
  ಗಾಗಿ (syms = &ತಲೆ, n = 1; syms->ಮುಂದಿನ; syms = syms->ಮುಂದಿನ, n += 1)
    ಆದರೆ (UNIQSTR_EQ (name, syms->ಮುಂದಿನ->name))
      ನಿಲ್ಲಿಸು;
  ಆದರೆ (syms->ಮುಂದಿನ == ಖಾಲಿ)
    {
      syms->ಮುಂದಿನ = xmalloc (ಗಾತ್ರ syms->ಮುಂದಿನ[0]);
      syms->ಮುಂದಿನ->name = uniqstr_new (name);
      /* After all symbol type declarations have been parsed, packgram invokes
         record_merge_function_type to set the type.  */
      syms->ಮುಂದಿನ->sym = ಖಾಲಿ;
      syms->ಮುಂದಿನ->ಮುಂದಿನ = ಖಾಲಿ;
      ವಿಲೀನ_functions = ತಲೆ.ಮುಂದಿನ;
    }
  ಹಿಂತಿರುಗಿ n;
}

/*-------------------------------------------------------------------.
| For the existing merging function with index MERGER, record that   |
| the result type is that of SYM, as required by the lhs (i.e., SYM) |
| of the rule whose %merge declaration is at DECLARATION_LOC.        |
`-------------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
ದಾಖಲೆ_ವಿಲೀನ_function_type (ಪೂರ್ಣಾಂಕ merger, ಚಿಹ್ನೆ *sym, location declaration_loc)
{
  ಆದರೆ (merger <= 0)
    ಹಿಂತಿರುಗಿ;

  uniqstr type
    = sym->content->type_name ? sym->content->type_name : uniqstr_new ("");

  merger_list *ವಿಲೀನ_function;
  ಪೂರ್ಣಾಂಕ merger_ಕಂಡುಹಿಡಿ = 1;
  ಗಾಗಿ (ವಿಲೀನ_function = ವಿಲೀನ_functions;
       ವಿಲೀನ_function != ಖಾಲಿ && merger_ಕಂಡುಹಿಡಿ != merger;
       ವಿಲೀನ_function = ವಿಲೀನ_function->ಮುಂದಿನ)
    merger_ಕಂಡುಹಿಡಿ += 1;
  aver (ವಿಲೀನ_function != ಖಾಲಿ && merger_ಕಂಡುಹಿಡಿ == merger);
  ಆದರೆ (ವಿಲೀನ_function->sym && ವಿಲೀನ_function->sym->content->type_name)
    {
      ಆದರೆ (!UNIQSTR_EQ (ವಿಲೀನ_function->sym->content->type_name, type))
        {
          complain (&declaration_loc, complaint,
                    _("result type clash on merge function %s: "
                      "<%s> != <%s>"),
                    quote (ವಿಲೀನ_function->name), type,
                    ವಿಲೀನ_function->sym->content->type_name);
          subcomplain (&ವಿಲೀನ_function->type_declaration_loc, complaint,
                       _("previous declaration"));
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ವಿಲೀನ_function->sym = sym;
      ವಿಲೀನ_function->type_declaration_loc = declaration_loc;
    }
}

/*--------------------------------------.
| Free all merge-function definitions.  |
`--------------------------------------*/

ಶೂನ್ಯ
free_merger_functions (ಶೂನ್ಯ)
{
  merger_list *L0 = ವಿಲೀನ_functions;
  ಆಗ (L0)
    {
      merger_list *L1 = L0->ಮುಂದಿನ;
      ಬಿಡುಗಡೆ (L0);
      L0 = L1;
    }
}


/*-------------------------------------------------------------------.
| Parse the input grammar into a one symbol_list structure.  Each    |
| rule is represented by a sequence of symbols: the left hand side   |
| followed by the contents of the right hand side, followed by a     |
| null pointer instead of a symbol to terminate the rule.  The next  |
| symbol is the lhs of the following rule.                           |
|                                                                    |
| All actions are copied out, labelled by the rule number they apply |
| to.                                                                |
`-------------------------------------------------------------------*/

/* The (currently) last symbol of GRAMMAR. */
ಸ್ಥಿರ ಚಿಹ್ನೆ_list *grammar_ಕೊನೆ = ಖಾಲಿ;

/* Append SYM to the grammar.  */
ಸ್ಥಿರ ಚಿಹ್ನೆ_list *
grammar_ಚಿಹ್ನೆ_append (ಚಿಹ್ನೆ *sym, location loc)
{
  ಚಿಹ್ನೆ_list *p = ಚಿಹ್ನೆ_list_sym_new (sym, loc);

  ಆದರೆ (grammar_ಕೊನೆ)
    grammar_ಕೊನೆ->ಮುಂದಿನ = p;
  ಇಲ್ಲದಿದ್ದರೆ
    grammar = p;

  grammar_ಕೊನೆ = p;

  /* A null SYM stands for an end of rule; it is not an actual
     part of it.  */
  ಆದರೆ (sym)
    ++nritems;

  ಹಿಂತಿರುಗಿ p;
}

ಸ್ಥಿರ ಶೂನ್ಯ
ನಿಯೋಜಿಸು_named_ref (ಚಿಹ್ನೆ_list *p, named_ref *name)
{
  ಚಿಹ್ನೆ *sym = p->content.sym;

  ಆದರೆ (name->id == sym->ಟ್ಯಾಗ್)
    {
      complain (&name->loc, Wother,
                _("duplicated symbol name for %s ignored"),
                quote (sym->ಟ್ಯಾಗ್));
      named_ref_free (name);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    p->named_ref = name;
}


/* The rule currently being defined, and the previous rule.
   CURRENT_RULE points to the first LHS of the current rule, while
   PREVIOUS_RULE_END points to the *end* of the previous rule (NULL).  */
ಸ್ಥಿರ ಚಿಹ್ನೆ_list *current_ನಿಯಮ = ಖಾಲಿ;
ಸ್ಥಿರ ಚಿಹ್ನೆ_list *ಹಿಂದಿನ_ನಿಯಮ_ಕೊನೆ = ಖಾಲಿ;


/*----------------------------------------------.
| Create a new rule for LHS in to the GRAMMAR.  |
`----------------------------------------------*/

ಶೂನ್ಯ
grammar_current_ನಿಯಮ_begin (ಚಿಹ್ನೆ *lhs, location loc,
                            named_ref *lhs_name)
{
  /* Start a new rule and record its lhs.  */
  ++nrules;
  ಹಿಂದಿನ_ನಿಯಮ_ಕೊನೆ = grammar_ಕೊನೆ;

  current_ನಿಯಮ = grammar_ಚಿಹ್ನೆ_append (lhs, loc);
  ಆದರೆ (lhs_name)
    ನಿಯೋಜಿಸು_named_ref (current_ನಿಯಮ, named_ref_copy (lhs_name));

  /* Mark the rule's lhs as a nonterminal if not already so.  */
  ಆದರೆ (lhs->content->class == unknown_sym || lhs->content->class == pct_type_sym)
    ಚಿಹ್ನೆ_class_ಗಣ (lhs, nterm_sym, ಖಾಲಿಗಳು_loc, false);
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (lhs->content->class == token_sym)
    complain (&loc, complaint, _("rule given for %s, which is a token"),
              lhs->ಟ್ಯಾಗ್);
}


/*----------------------------------------------------------------------.
| A symbol should be used if either:                                    |
|   1. It has a destructor.                                             |
|   2. The symbol is a midrule symbol (i.e., the generated LHS          |
|      replacing a midrule action) that was assigned to or used, as in  |
|      "exp: { $$ = 1; } { $$ = $1; }".                                 |
`----------------------------------------------------------------------*/

ಸ್ಥಿರ bool
ಚಿಹ್ನೆ_should_be_ಬಳಸಿದ (ಚಿಹ್ನೆ_list ಸ್ಥಿರಾಂಕ *s, bool *midrule_warning)
{
  ಆದರೆ (ಚಿಹ್ನೆ_ಸಂಕೇತ_props_get (s->content.sym, destructor)->ಸಂಕೇತ_2)
    ಹಿಂತಿರುಗಿ true;
  ಆದರೆ ((s->midrule && s->midrule->action_props.is_ಮೌಲ್ಯ_ಬಳಸಿದ)
      || (s->midrule_ಪೋಷಕ_ನಿಯಮ
          && (ಚಿಹ್ನೆ_list_n_get (s->midrule_ಪೋಷಕ_ನಿಯಮ,
                                 s->midrule_ಪೋಷಕ_rhs_ಸೂಚ್ಯಂಕ)
              ->action_props.is_ಮೌಲ್ಯ_ಬಳಸಿದ)))
    {
      *midrule_warning = true;
      ಹಿಂತಿರುಗಿ true;
    }
  ಹಿಂತಿರುಗಿ false;
}

/*-----------------------------------------------------------------.
| Check that the rule R is properly defined.  For instance, there  |
| should be no type clash on the default action.  Possibly install |
| the default action.                                              |
`-----------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
grammar_ನಿಯಮ_check_and_complete (ಚಿಹ್ನೆ_list *r)
{
  ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆ *lhs = r->content.sym;
  ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆ *ಮೊದಲ_rhs = r->ಮುಂದಿನ->content.sym;

  /* Type check.

     If there is an action, then there is nothing we can do: the user
     is allowed to shoot herself in the foot.

     Don't worry about the default action if $$ is untyped, since $$'s
     value can't be used.  */
  ಆದರೆ (!r->action_props.ಸಂಕೇತ_2 && lhs->content->type_name)
    {
      /* If $$ is being set in default way, report if any type mismatch.  */
      ಆದರೆ (ಮೊದಲ_rhs)
        {
          ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *lhs_type = lhs->content->type_name;
          ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *rhs_type =
            ಮೊದಲ_rhs->content->type_name ? ಮೊದಲ_rhs->content->type_name : "";
          ಆದರೆ (!UNIQSTR_EQ (lhs_type, rhs_type))
            complain (&r->rhs_loc, Wother,
                      _("type clash on default action: <%s> != <%s>"),
                      lhs_type, rhs_type);
          ಇಲ್ಲದಿದ್ದರೆ
            {
              /* Install the default action only for C++.  */
              ಸ್ಥಿರಾಂಕ bool is_cxx =
                STREQ (language->language, "c++")
                || (skeleton && (STREQ (skeleton, "glr.cc")
                                 || STREQ (skeleton, "glr2.cc")
                                 || STREQ (skeleton, "lalr1.cc")));
              ಆದರೆ (is_cxx)
                {
                  ಸಂಕೇತ_props_ನಿಯಮ_action_init (&r->action_props, "{ $$ = $1; }",
                                               r->rhs_loc, r,
                                               /* name */ ಖಾಲಿ,
                                               /* type */ ಖಾಲಿ,
                                               /* is_predicate */ false);
                  ಸಂಕೇತ_props_translate_ಸಂಕೇತ (&r->action_props);
                }
            }
        }
      /* Warn if there is no default for $$ but we need one.  */
      ಇಲ್ಲದಿದ್ದರೆ
        complain (&r->rhs_loc, Wother,
                  _("empty rule for typed nonterminal, and no action"));
    }

  /* For each start symbol, build the action of its start rule.  Use
     the same obstack as the one used by scan-code, which is in charge
     of actions. */
  ಸ್ಥಿರಾಂಕ bool multistart = ಆರಂಭ_symbols && ಆರಂಭ_symbols->ಮುಂದಿನ;
  ಆದರೆ (multistart && lhs == acceptsymbol)
    {
      ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆ *ಆರಂಭ = r->ಮುಂದಿನ->ಮುಂದಿನ->content.sym;
      ಆದರೆ (ಆರಂಭ->content->type_name)
        obstack_printf (obstack_for_actions,
                        "{ ]b4_accept""([%s%d])[; }",
                        ಆರಂಭ->content->class == nterm_sym ? "orig " : "",
                        ಆರಂಭ->content->ಸಂಖ್ಯೆ);
      ಇಲ್ಲದಿದ್ದರೆ
        obstack_printf (obstack_for_actions,
                        "{ ]b4_accept[; }");
      ಸಂಕೇತ_props_ನಿಯಮ_action_init (&r->action_props,
                                   obstack_finish0 (obstack_for_actions),
                                   r->rhs_loc, r,
                                   /* name */ ಖಾಲಿ,
                                   /* type */ ಖಾಲಿ,
                                   /* is_predicate */ false);
    }


  /* Check that symbol values that should be used are in fact used.
     Don't check the generated start rules.  It has no action, so some
     rhs symbols may appear unused, but the parsing algorithm ensures
     that %destructor's are invoked appropriately.  */
  ಆದರೆ (lhs != acceptsymbol)
    {
      ಪೂರ್ಣಾಂಕ n = 0;
      ಗಾಗಿ (ಚಿಹ್ನೆ_list ಸ್ಥಿರಾಂಕ *l = r; l && l->content.sym; l = l->ಮುಂದಿನ, ++n)
        {
          bool midrule_warning = false;
          ಆದರೆ (!l->action_props.is_ಮೌಲ್ಯ_ಬಳಸಿದ
              && ಚಿಹ್ನೆ_should_be_ಬಳಸಿದ (l, &midrule_warning)
              /* The default action, $$ = $1, 'uses' both.  */
              && (r->action_props.ಸಂಕೇತ_2 || (n != 0 && n != 1)))
            {
              warnings warn_flag = midrule_warning ? Wmidrule_ಮೌಲ್ಯಗಳು : Wother;
              ಆದರೆ (n)
                complain (&l->sym_loc, warn_flag, _("unused value: $%d"), n);
              ಇಲ್ಲದಿದ್ದರೆ
                complain (&l->rhs_loc, warn_flag, _("unset value: $$"));
            }
        }
    }

  /* Check that %empty => empty rule.  */
  ಆದರೆ (r->percent_ಖಾಲಿಗಳು_loc.ಆರಂಭ.file
      && r->ಮುಂದಿನ && r->ಮುಂದಿನ->content.sym)
    {
      complain (&r->percent_ಖಾಲಿಗಳು_loc, complaint,
                _("%%empty on non-empty rule"));
      fixits_ರಿಜಿಸ್ಟರ್ (&r->percent_ಖಾಲಿಗಳು_loc, "");
    }

  /* Check that empty rule => %empty.  */
  ಆದರೆ (!(r->ಮುಂದಿನ && r->ಮುಂದಿನ->content.sym)
      && !r->midrule_ಪೋಷಕ_ನಿಯಮ
      && !r->percent_ಖಾಲಿಗಳು_loc.ಆರಂಭ.file
      && warning_is_enabled (Wempty_ನಿಯಮ))
    {
      complain (&r->rhs_loc, Wempty_ನಿಯಮ, _("empty rule without %%empty"));
      ಆದರೆ (feature_flag & feature_caret)
        location_caret_suggestion (r->rhs_loc, "%empty", stderr);
      location loc = r->rhs_loc;
      loc.ಕೊನೆ = loc.ಆರಂಭ;
      fixits_ರಿಜಿಸ್ಟರ್ (&loc, " %empty ");
    }

  /* See comments in grammar_current_rule_prec_set for how POSIX
     mandates this complaint.  It's only for identifiers, so skip
     it for char literals and strings, which are always tokens.  */
  ಆದರೆ (r->ruleprec
      && r->ruleprec->ಟ್ಯಾಗ್[0] != '\'' && r->ruleprec->ಟ್ಯಾಗ್[0] != '"'
      && r->ruleprec->content->ಸ್ಥಿತಿ_2 != declared
      && !r->ruleprec->content->prec)
    complain (&r->rhs_loc, Wother,
              _("token for %%prec is not defined: %s"), r->ruleprec->ಟ್ಯಾಗ್);

  /* Check that the (main) action was not typed.  */
  ಆದರೆ (r->action_props.type)
    complain (&r->rhs_loc, Wother,
              _("only midrule actions can be typed: %s"), r->action_props.type);
}


/*-------------------------------------.
| End the currently being grown rule.  |
`-------------------------------------*/

ಶೂನ್ಯ
grammar_current_ನಿಯಮ_ಕೊನೆ (location loc)
{
  /* Put an empty link in the list to mark the end of this rule  */
  grammar_ಚಿಹ್ನೆ_append (ಖಾಲಿ, grammar_ಕೊನೆ->rhs_loc);
  current_ನಿಯಮ->rhs_loc = loc;
}


/*-------------------------------------------------------------------.
| The previous action turns out to be a midrule action.  Attach it   |
| to the current rule, i.e., create a dummy symbol, attach it this   |
| midrule action, and append this dummy nonterminal to the current   |
| rule.                                                              |
`-------------------------------------------------------------------*/

ಶೂನ್ಯ
grammar_midrule_action (ಶೂನ್ಯ)
{
  /* Since the action was written out with this rule's number, we must
     give the new rule this number by inserting the new rule before
     it.  */

  /* Make a DUMMY nonterminal, whose location is that of the midrule
     action.  Create the MIDRULE.  */
  location dummy_loc = current_ನಿಯಮ->action_props.location;
  ಚಿಹ್ನೆ *dummy = dummy_ಚಿಹ್ನೆ_get (dummy_loc);
  ಚಿಹ್ನೆ_type_ಗಣ (dummy,
                   current_ನಿಯಮ->action_props.type, current_ನಿಯಮ->action_props.location);
  ಚಿಹ್ನೆ_list *midrule = ಚಿಹ್ನೆ_list_sym_new (dummy, dummy_loc);

  /* Remember named_ref of previous action. */
  named_ref *action_name = current_ನಿಯಮ->action_props.named_ref;

  /* Make a new rule, whose body is empty, before the current one, so
     that the action just read can belong to it.  */
  ++nrules;
  ++nritems;
  /* Attach its location and actions to that of the DUMMY.  */
  midrule->rhs_loc = dummy_loc;
  ಸಂಕೇತ_props_ನಿಯಮ_action_init (&midrule->action_props,
                               current_ನಿಯಮ->action_props.ಸಂಕೇತ_2,
                               current_ನಿಯಮ->action_props.location,
                               midrule,
                               /* name_ref */ ಖಾಲಿ,
                               /* type */ ಖಾಲಿ,
                               current_ನಿಯಮ->action_props.is_predicate);
  ಸಂಕೇತ_props_none_init (&current_ನಿಯಮ->action_props);

  midrule->expected_sr_conflicts = current_ನಿಯಮ->expected_sr_conflicts;
  midrule->expected_rr_conflicts = current_ನಿಯಮ->expected_rr_conflicts;
  current_ನಿಯಮ->expected_sr_conflicts = -1;
  current_ನಿಯಮ->expected_rr_conflicts = -1;

  ಆದರೆ (ಹಿಂದಿನ_ನಿಯಮ_ಕೊನೆ)
    ಹಿಂದಿನ_ನಿಯಮ_ಕೊನೆ->ಮುಂದಿನ = midrule;
  ಇಲ್ಲದಿದ್ದರೆ
    grammar = midrule;

  /* End the dummy's rule.  */
  midrule->ಮುಂದಿನ = ಚಿಹ್ನೆ_list_sym_new (ಖಾಲಿ, dummy_loc);
  midrule->ಮುಂದಿನ->ಮುಂದಿನ = current_ನಿಯಮ;

  ಹಿಂದಿನ_ನಿಯಮ_ಕೊನೆ = midrule->ಮುಂದಿನ;

  /* Insert the dummy nonterminal replacing the midrule action into
     the current rule.  Bind it to its dedicated rule.  */
  grammar_current_ನಿಯಮ_ಚಿಹ್ನೆ_append (dummy, dummy_loc,
                                      action_name);
  grammar_ಕೊನೆ->midrule = midrule;
  midrule->midrule_ಪೋಷಕ_ನಿಯಮ = current_ನಿಯಮ;
  midrule->midrule_ಪೋಷಕ_rhs_ಸೂಚ್ಯಂಕ = ಚಿಹ್ನೆ_list_ಉದ್ದಳತೆ (current_ನಿಯಮ->ಮುಂದಿನ);
}

/* Set the precedence symbol of the current rule to PRECSYM. */

ಶೂನ್ಯ
grammar_current_ನಿಯಮ_prec_ಗಣ (ಚಿಹ್ನೆ *precsym, location loc)
{
  /* POSIX says that any identifier is a nonterminal if it does not
     appear on the LHS of a grammar rule and is not defined by %token
     or by one of the directives that assigns precedence to a token.
     We ignore this here because the only kind of identifier that
     POSIX allows to follow a %prec is a token and because assuming
     it's a token now can produce more logical error messages.
     Nevertheless, grammar_rule_check_and_complete does obey what we
     believe is the real intent of POSIX here: that an error be
     reported for any identifier that appears after %prec but that is
     not defined separately as a token.  */
  ಚಿಹ್ನೆ_class_ಗಣ (precsym, token_sym, loc, false);
  ಆದರೆ (current_ನಿಯಮ->ruleprec)
    duplicate_ನಿಯಮ_directive ("%prec",
                              current_ನಿಯಮ->ruleprec->location, loc);
  ಇಲ್ಲದಿದ್ದರೆ
    current_ನಿಯಮ->ruleprec = precsym;
}

/* Set %empty for the current rule. */

ಶೂನ್ಯ
grammar_current_ನಿಯಮ_ಖಾಲಿಗಳು_ಗಣ (location loc)
{
  /* If %empty is used and -Wno-empty-rule is not, then enable
     -Wempty-rule.  */
  ಆದರೆ (warning_is_unset (Wempty_ನಿಯಮ))
    warning_argmatch ("empty-rule", 0, 0);
  ಆದರೆ (current_ನಿಯಮ->percent_ಖಾಲಿಗಳು_loc.ಆರಂಭ.file)
    duplicate_ನಿಯಮ_directive ("%empty",
                              current_ನಿಯಮ->percent_ಖಾಲಿಗಳು_loc, loc);
  ಇಲ್ಲದಿದ್ದರೆ
    current_ನಿಯಮ->percent_ಖಾಲಿಗಳು_loc = loc;
}

/* Attach dynamic precedence DPREC to the current rule. */

ಶೂನ್ಯ
grammar_current_ನಿಯಮ_dprec_ಗಣ (ಪೂರ್ಣಾಂಕ dprec, location loc)
{
  ಆದರೆ (! glr_parser)
    complain (&loc, Wother, _("%s affects only GLR parsers"),
              "%dprec");
  ಆದರೆ (dprec <= 0)
    complain (&loc, complaint, _("%s must be followed by positive number"),
              "%dprec");
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (current_ನಿಯಮ->dprec != 0)
    duplicate_ನಿಯಮ_directive ("%dprec",
                              current_ನಿಯಮ->dprec_loc, loc);
  ಇಲ್ಲದಿದ್ದರೆ
    {
      current_ನಿಯಮ->dprec = dprec;
      current_ನಿಯಮ->dprec_loc = loc;
    }
}

/* Attach a merge function NAME with argument type TYPE to current
   rule. */

ಶೂನ್ಯ
grammar_current_ನಿಯಮ_ವಿಲೀನ_ಗಣ (uniqstr name, location loc)
{
  ಆದರೆ (! glr_parser)
    complain (&loc, Wother, _("%s affects only GLR parsers"),
              "%merge");
  ಆದರೆ (current_ನಿಯಮ->merger != 0)
    duplicate_ನಿಯಮ_directive ("%merge",
                              current_ನಿಯಮ->merger_declaration_loc, loc);
  ಇಲ್ಲದಿದ್ದರೆ
    {
      current_ನಿಯಮ->merger = get_ವಿಲೀನ_function (name);
      current_ನಿಯಮ->merger_declaration_loc = loc;
    }
}

/* Attach SYM to the current rule.  If needed, move the previous
   action as a midrule action.  */

ಶೂನ್ಯ
grammar_current_ನಿಯಮ_ಚಿಹ್ನೆ_append (ಚಿಹ್ನೆ *sym, location loc,
                                    named_ref *name)
{
  ಆದರೆ (current_ನಿಯಮ->action_props.ಸಂಕೇತ_2)
    grammar_midrule_action ();
  ಚಿಹ್ನೆ_list *p = grammar_ಚಿಹ್ನೆ_append (sym, loc);
  ಆದರೆ (name)
    ನಿಯೋಜಿಸು_named_ref (p, name);
  ಆದರೆ (sym->content->ಸ್ಥಿತಿ_2 == undeclared || sym->content->ಸ್ಥಿತಿ_2 == ಬಳಸಿದ)
    sym->content->ಸ್ಥಿತಿ_2 = needed;
}

ಶೂನ್ಯ
grammar_current_ನಿಯಮ_action_append (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *action, location loc,
                                    named_ref *name, uniqstr type)
{
  ಆದರೆ (current_ನಿಯಮ->action_props.ಸಂಕೇತ_2)
    grammar_midrule_action ();
  ಆದರೆ (type)
    complain (&loc, Wyacc,
              _("POSIX Yacc does not support typed midrule actions"));
  /* After all symbol declarations have been parsed, packgram invokes
     code_props_translate_code.  */
  ಸಂಕೇತ_props_ನಿಯಮ_action_init (&current_ನಿಯಮ->action_props, action, loc,
                               current_ನಿಯಮ,
                               name, type,
                               /* is_predicate */ false);
}

ಶೂನ್ಯ
grammar_current_ನಿಯಮ_predicate_append (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *pred, location loc)
{
  ಆದರೆ (current_ನಿಯಮ->action_props.ಸಂಕೇತ_2)
    grammar_midrule_action ();
  ಸಂಕೇತ_props_ನಿಯಮ_action_init (&current_ನಿಯಮ->action_props, pred, loc,
                               current_ನಿಯಮ,
                               ಖಾಲಿ, ಖಾಲಿ,
                               /* is_predicate */ true);
}

/* Set the expected number of shift/reduce (reduce/reduce) conflicts
 * for the current rule.  If a midrule is encountered later, the count
 * is transferred to it and reset in the current rule to -1. */

ಶೂನ್ಯ
grammar_current_ನಿಯಮ_expect_sr (ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, location loc)
{
  (ಶೂನ್ಯ) loc;
  current_ನಿಯಮ->expected_sr_conflicts = ಎಣಿಕೆ_2;
}

ಶೂನ್ಯ
grammar_current_ನಿಯಮ_expect_rr (ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, location loc)
{
  ಆದರೆ (! glr_parser)
    complain (&loc, Wother, _("%s affects only GLR parsers"),
              "%expect-rr");
  ಇಲ್ಲದಿದ್ದರೆ
    current_ನಿಯಮ->expected_rr_conflicts = ಎಣಿಕೆ_2;
}


/*---------------------------------------------.
| Build RULES and RITEM from what was parsed.  |
`---------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
packgram (ಶೂನ್ಯ)
{
  ಪೂರ್ಣಾಂಕ itemno = 0;
  ritem = xnmalloc (nritems + 1, ಗಾತ್ರ *ritem);
  /* This sentinel is used by build_relations() in lalr.c.  */
  *ritem++ = 0;

  ನಿಯಮ_ಸಂಖ್ಯೆ ruleno = 0;
  ನಿಯಮಗಳು = xnmalloc (nrules, ಗಾತ್ರ *ನಿಯಮಗಳು);

  ಗಾಗಿ (ಚಿಹ್ನೆ_list *p = grammar; p; p = p->ಮುಂದಿನ)
    {
      ಚಿಹ್ನೆ_list *lhs = p;
      ದಾಖಲೆ_ವಿಲೀನ_function_type (lhs->merger, lhs->content.sym,
                                  lhs->merger_declaration_loc);
      /* If the midrule's $$ is set or its $n is used, remove the '$' from the
         symbol name so that it's a user-defined symbol so that the default
         %destructor and %printer apply.  */
      ಆದರೆ (lhs->midrule_ಪೋಷಕ_ನಿಯಮ /* i.e., symbol_is_dummy (lhs->content.sym).  */
          && (lhs->action_props.is_ಮೌಲ್ಯ_ಬಳಸಿದ
              || (ಚಿಹ್ನೆ_list_n_get (lhs->midrule_ಪೋಷಕ_ನಿಯಮ,
                                     lhs->midrule_ಪೋಷಕ_rhs_ಸೂಚ್ಯಂಕ)
                  ->action_props.is_ಮೌಲ್ಯ_ಬಳಸಿದ)))
        lhs->content.sym->ಟ್ಯಾಗ್ += 1;

      grammar_ನಿಯಮ_check_and_complete (lhs);

      ನಿಯಮಗಳು[ruleno].ಸಂಕೇತ_2 = ruleno;
      ನಿಯಮಗಳು[ruleno].ಸಂಖ್ಯೆ = ruleno;
      ನಿಯಮಗಳು[ruleno].lhs = lhs->content.sym->content;
      ನಿಯಮಗಳು[ruleno].rhs = ritem + itemno;
      ನಿಯಮಗಳು[ruleno].prec = ಖಾಲಿ;
      ನಿಯಮಗಳು[ruleno].dprec = lhs->dprec;
      ನಿಯಮಗಳು[ruleno].merger = lhs->merger;
      ನಿಯಮಗಳು[ruleno].precsym = ಖಾಲಿ;
      ನಿಯಮಗಳು[ruleno].location = lhs->rhs_loc;
      ನಿಯಮಗಳು[ruleno].useful = true;
      ನಿಯಮಗಳು[ruleno].action = lhs->action_props.ಸಂಕೇತ_2;
      ನಿಯಮಗಳು[ruleno].action_loc = lhs->action_props.location;
      ನಿಯಮಗಳು[ruleno].is_predicate = lhs->action_props.is_predicate;
      ನಿಯಮಗಳು[ruleno].expected_sr_conflicts = lhs->expected_sr_conflicts;
      ನಿಯಮಗಳು[ruleno].expected_rr_conflicts = lhs->expected_rr_conflicts;

      /* Traverse the rhs.  */
      {
        size_t ನಿಯಮ_ಉದ್ದಳತೆ = 0;
        ಗಾಗಿ (p = lhs->ಮುಂದಿನ; p->content.sym; p = p->ಮುಂದಿನ)
          {
            ++ನಿಯಮ_ಉದ್ದಳತೆ;

            /* Don't allow rule_length == INT_MAX, since that might
               cause confusion with strtol if INT_MAX == LONG_MAX.  */
            ಆದರೆ (ನಿಯಮ_ಉದ್ದಳತೆ == ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ)
              complain (&ನಿಯಮಗಳು[ruleno].location, fatal, _("rule is too long"));

            /* item_number = symbol_number.
               But the former needs to contain more: negative rule numbers. */
            ritem[itemno++] =
              ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ_as_ಅಂಶ_ಸಂಖ್ಯೆ (p->content.sym->content->ಸಂಖ್ಯೆ);
            /* A rule gets by default the precedence and associativity
               of its last token.  */
            ಆದರೆ (p->content.sym->content->class == token_sym && default_prec)
              ನಿಯಮಗಳು[ruleno].prec = p->content.sym->content;
          }
      }

      /* If this rule has a %prec,
         the specified symbol's precedence replaces the default.  */
      ಆದರೆ (lhs->ruleprec)
        {
          ನಿಯಮಗಳು[ruleno].precsym = lhs->ruleprec->content;
          ನಿಯಮಗಳು[ruleno].prec = lhs->ruleprec->content;
        }

      /* An item ends by the rule number (negated).  */
      ritem[itemno++] = ನಿಯಮ_ಸಂಖ್ಯೆ_as_ಅಂಶ_ಸಂಖ್ಯೆ (ruleno);
      aver (itemno < ಅಂಶ_ಸಂಖ್ಯೆ_MAX);
      ++ruleno;
      aver (ruleno < ನಿಯಮ_ಸಂಖ್ಯೆ_MAX);
    }

  aver (itemno == nritems);

  ಆದರೆ (trace_flag & trace_sets)
    ritem_print (stderr);
}


/*--------------------------------------------------------------.
| Read in the grammar specification and record it in the format |
| described in gram.h.                                          |
`--------------------------------------------------------------*/

ಶೂನ್ಯ
reader (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *gram)
{
  /* Set up symbol_table, semantic_type_table, and the built-in
     symbols.  */
  symbols_new ();

  gram_scanner_open (gram);
  parser_init ();
  gram_parse ();
  gram_scanner_close ();

  prepare_percent_define_front_ಕೊನೆ_variables ();

  ಆದರೆ (complaint_ಸ್ಥಿತಿ  < ಸ್ಥಿತಿ_complaint)
    check_and_convert_grammar ();
}

ಸ್ಥಿರ ಶೂನ್ಯ
prepare_percent_define_front_ಕೊನೆ_variables (ಶೂನ್ಯ)
{
  /* Set %define front-end variable defaults.  */
  muscle_percent_define_default ("lr.keep-unreachable-state", "false");
  {
    /* IELR would be a better default, but LALR is historically the
       default.  */
    muscle_percent_define_default ("lr.type", "lalr");
    ಅಕ್ಷರ *lr_type = muscle_percent_define_get ("lr.type");
    ಆದರೆ (STRNEQ (lr_type, "canonical-lr"))
      muscle_percent_define_default ("lr.default-reduction", "most");
    ಇಲ್ಲದಿದ್ದರೆ
      muscle_percent_define_default ("lr.default-reduction", "accepting");
    ಬಿಡುಗಡೆ (lr_type);
  }
  muscle_percent_define_default ("tool.xsltproc", "xsltproc");

  /* Check %define front-end variables.  */
  {
    ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ * ಸ್ಥಿರಾಂಕ ಮೌಲ್ಯಗಳು[] =
      {
       "lr.type", "lr""(0)", "lalr", "ielr", "canonical-lr", ಖಾಲಿ,
       "lr.default-reduction", "most", "consistent", "accepting", ಖಾಲಿ,
       ಖಾಲಿ
      };
    muscle_percent_define_check_ಮೌಲ್ಯಗಳು (ಮೌಲ್ಯಗಳು);
  }
}

/* Find the first LHS which is not a dummy.  */

ಸ್ಥಿರ ಚಿಹ್ನೆ *
ಕಂಡುಹಿಡಿ_ಆರಂಭ_ಚಿಹ್ನೆ (ಶೂನ್ಯ)
{
  ಚಿಹ್ನೆ_list *res = grammar;
  /* Skip all the possible dummy rules of the first rule.  */
  ಗಾಗಿ (; ಚಿಹ್ನೆ_is_dummy (res->content.sym); res = res->ಮುಂದಿನ)
    /* Skip the LHS, and then all the RHS of the dummy rule.  */
    ಗಾಗಿ (res = res->ಮುಂದಿನ; res->content.sym; res = res->ಮುಂದಿನ)
      ಮುಂದುವರಿ;
  ಹಿಂತಿರುಗಿ res->content.sym;
}


/* Insert an initial rule, whose location is that of the first rule
   (not that of the start symbol):

   $accept: SWITCHING_TOKEN START $end.  */
ಸ್ಥಿರ ಶೂನ್ಯ
create_ಆರಂಭ_ನಿಯಮ (ಚಿಹ್ನೆ *swtok, ಚಿಹ್ನೆ *ಆರಂಭ)
{
  ಚಿಹ್ನೆ_list *initial_ನಿಯಮ = ಚಿಹ್ನೆ_list_sym_new (acceptsymbol, ಖಾಲಿಗಳು_loc);
  initial_ನಿಯಮ->rhs_loc = grammar->rhs_loc;
  ಚಿಹ್ನೆ_list *p = initial_ನಿಯಮ;
  ಆದರೆ (swtok)
    {
      // Cannot create the action now, as the symbols have not yet
      // been assigned their number (by symbol_pack), which we need to
      // know the type name.  So the action is created in
      // grammar_rule_check_and_complete, which is run after
      // symbol_pack.
      p->ಮುಂದಿನ = ಚಿಹ್ನೆ_list_sym_new (swtok, ಖಾಲಿಗಳು_loc);
      p = p->ಮುಂದಿನ;
    }
  p->ಮುಂದಿನ = ಚಿಹ್ನೆ_list_sym_new (ಆರಂಭ, ಖಾಲಿಗಳು_loc);
  p = p->ಮುಂದಿನ;
  p->ಮುಂದಿನ = ಚಿಹ್ನೆ_list_sym_new (eoftoken, ಖಾಲಿಗಳು_loc);
  p = p->ಮುಂದಿನ;
  p->ಮುಂದಿನ = ಚಿಹ್ನೆ_list_sym_new (ಖಾಲಿ, ಖಾಲಿಗಳು_loc);
  p = p->ಮುಂದಿನ;
  p->ಮುಂದಿನ = grammar;
  nrules += 1;
  nritems += 3 + !!swtok;
  grammar = initial_ನಿಯಮ;
}

/* Fetch (or create) a token "YY_PARSE_foo" for start symbol "foo".

   We don't use the simple "YY_FOO" because (i) we might get clashes
   with some of our symbols (e.g., cast => YY_CAST), and (ii) upcasing
   introduces possible clashes between terminal FOO and nonterminal
   foo.  */
ಚಿಹ್ನೆ *
switching_token (ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆ *ಆರಂಭ)
{
  ಅಕ್ಷರ buf[100];
  size_t len = ಗಾತ್ರ buf;
  ಅಕ್ಷರ *name = asnprintf (buf, &len, "YY_PARSE_%s", ಚಿಹ್ನೆ_id_get (ಆರಂಭ));
  ಆದರೆ (!name)
    xalloc_die ();
  // Setting the location ensures deterministic symbol numbers.
  ಚಿಹ್ನೆ *res = ಚಿಹ್ನೆ_get (name, ಆರಂಭ->location);
  ಆದರೆ (name != buf)
    ಬಿಡುಗಡೆ (name);
  ಚಿಹ್ನೆ_class_ಗಣ (res, token_sym, ಆರಂಭ->location, false);
  ಹಿಂತಿರುಗಿ res;
}

/* Create the start rules in reverse order, since they are inserted at
   the top of the grammar.  That way the rules follow the order of
   declaration to %start.  */

ಸ್ಥಿರ ಶೂನ್ಯ
create_ಅಪವರ್ತ್ಯ_ಆರಂಭ_ನಿಯಮಗಳು (ಚಿಹ್ನೆ_list *ಆರಂಭ_syms)
{
  ಆದರೆ (ಆರಂಭ_syms)
    {
      create_ಅಪವರ್ತ್ಯ_ಆರಂಭ_ನಿಯಮಗಳು (ಆರಂಭ_syms->ಮುಂದಿನ);
      ಪ್ರತಿಜ್ಞಾ (ಆರಂಭ_syms->content_type == SYMLIST_ಚಿಹ್ನೆ);
      ಚಿಹ್ನೆ *ಆರಂಭ = ಆರಂಭ_syms->content.sym;
      ಚಿಹ್ನೆ *swtok = switching_token (ಆರಂಭ);
      create_ಆರಂಭ_ನಿಯಮ (swtok, ಆರಂಭ);
    }
}

/* For each start symbol "foo", create the rule "$accept: YY_FOO
   foo $end". */
ಸ್ಥಿರ ಶೂನ್ಯ
create_ಆರಂಭ_ನಿಯಮಗಳು (ಶೂನ್ಯ)
{
  ಆದರೆ (!ಆರಂಭ_symbols)
    {
      ಚಿಹ್ನೆ *ಆರಂಭ = ಕಂಡುಹಿಡಿ_ಆರಂಭ_ಚಿಹ್ನೆ ();
      ಆರಂಭ_symbols = ಚಿಹ್ನೆ_list_sym_new (ಆರಂಭ, ಆರಂಭ->location);
    }

  ಸ್ಥಿರಾಂಕ bool several = ಆರಂಭ_symbols->ಮುಂದಿನ;
  ಆದರೆ (several)
    create_ಅಪವರ್ತ್ಯ_ಆರಂಭ_ನಿಯಮಗಳು (ಆರಂಭ_symbols);
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಚಿಹ್ನೆ *ಆರಂಭ = ಆರಂಭ_symbols->content.sym;
      create_ಆರಂಭ_ನಿಯಮ (ಖಾಲಿ, ಆರಂಭ);
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
check_ಆರಂಭ_symbols (ಶೂನ್ಯ)
{
  ಸ್ಥಿರಾಂಕ bool multistart = ಆರಂಭ_symbols && ಆರಂಭ_symbols->ಮುಂದಿನ;
  // Sanity checks on the start symbols.
  ಗಾಗಿ (ಚಿಹ್ನೆ_list *list = ಆರಂಭ_symbols; list; list = list->ಮುಂದಿನ)
    {
      ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆ *ಆರಂಭ = list->content.sym;
      ಆದರೆ (ಆರಂಭ->content->class == unknown_sym)
        {
          complain (&ಆರಂಭ->location, complaint,
                    _("the start symbol %s is undefined"),
                    ಆರಂಭ->ಟ್ಯಾಗ್);
          // I claim this situation is unreachable.  This is caught
          // before, and we get "symbol 'foo' is used, but is not
          // defined as a token and has no rules".
          abort ();
        }
      // If your only start symbol is a token, you're weird.
      ಆದರೆ (!multistart && ಆರಂಭ->content->class == token_sym)
        complain (&ಆರಂಭ->location, complaint,
                  _("the start symbol %s is a token"),
                  ಆರಂಭ->ಟ್ಯಾಗ್);
    }
  ಆದರೆ (complaint_ಸ್ಥಿತಿ == ಸ್ಥಿತಿ_complaint)
    ನಿರ್ಗಮಿಸು (EXIT_FAILURE);
}


/*-------------------------------------------------------------.
| Check the grammar that has just been read, and convert it to |
| internal form.                                               |
`-------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
check_and_convert_grammar (ಶೂನ್ಯ)
{
  /* Grammar has been read.  Do some checking.  */
  ಆದರೆ (nrules == 0)
    complain (ಖಾಲಿ, fatal, _("no rules in the input grammar"));

  /* If the user did not define her EOFTOKEN, do it now. */
  ಆದರೆ (!eoftoken)
    {
      eoftoken = ಚಿಹ್ನೆ_get ("YYEOF", ಖಾಲಿಗಳು_loc);
      ಆದರೆ (eoftoken->content->class == nterm_sym)
        complain (&eoftoken->location, complaint,
                  _("rule given for %s, which is a token"),
                  eoftoken->ಟ್ಯಾಗ್);
      eoftoken->content->class = token_sym;
      eoftoken->content->ಸಂಖ್ಯೆ = 0;
      /* Value specified by POSIX.  */
      eoftoken->content->ಸಂಕೇತ_2 = 0;
      {
        ಚಿಹ್ನೆ *alias = ಚಿಹ್ನೆ_get ("$end", ಖಾಲಿಗಳು_loc);
        ಚಿಹ್ನೆ_class_ಗಣ (alias, token_sym, ಖಾಲಿಗಳು_loc, false);
        ಚಿಹ್ನೆ_make_alias (eoftoken, alias, ಖಾಲಿಗಳು_loc);
      }
    }

  /* Insert the initial rule(s).  */
  create_ಆರಂಭ_ನಿಯಮಗಳು ();

  /* Report any undefined symbols and consider them nonterminals.  */
  symbols_check_defined ();

  ಆದರೆ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ_ಗರಿಷ್ಠ - nnterms < ntokens)
    complain (ಖಾಲಿ, fatal, "too many symbols in input grammar (limit is %d)",
              ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ_ಗರಿಷ್ಠ);

  nsyms = ntokens + nnterms;

  /* Assign the symbols their symbol numbers.  */
  symbols_pack ();

  check_ಆರಂಭ_symbols ();

  /* Scan rule actions after invoking symbol_check_alias_consistency
     (in symbols_pack above) so that token types are set correctly
     before the rule action type checking.

     Before invoking grammar_rule_check_and_complete (in packgram
     below) on any rule, make sure all actions have already been
     scanned in order to set 'used' flags.  Otherwise, checking that a
     midrule's $$ should be set will not always work properly because
     the check must forward-reference the midrule's parent rule.  For
     the same reason, all the 'used' flags must be set before checking
     whether to remove '$' from any midrule symbol name (also in
     packgram).  */
  ಗಾಗಿ (ಚಿಹ್ನೆ_list *sym = grammar; sym; sym = sym->ಮುಂದಿನ)
    ಸಂಕೇತ_props_translate_ಸಂಕೇತ (&sym->action_props);

  /* Convert the grammar into the format described in gram.h.  */
  packgram ();

  /* The grammar as a symbol_list is no longer needed. */
  ಚಿಹ್ನೆ_list_free (grammar);
}
