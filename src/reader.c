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

ಸ್ಥಿರ ಶೂನ್ಯ prepare_percent_define_front_end_variables (ಶೂನ್ಯ);
ಸ್ಥಿರ ಶೂನ್ಯ check_and_convert_grammar (ಶೂನ್ಯ);

ಸ್ಥಿರ ಚಿಹ್ನೆ_list *grammar = ರಿಕ್ತ;
ಚಿಹ್ನೆ_list *start_symbols = ರಿಕ್ತ;
merger_list *merge_functions = ರಿಕ್ತ;

/* Was %union seen?  */
bool union_seen = false;

/* Should rules have a default precedence?  */
bool default_prec = true;


ಶೂನ್ಯ
grammar_start_symbols_add (ಚಿಹ್ನೆ_list *syms)
{
  /* Report and ignore duplicates.  Append the others to START_SYMBOLS.  */
  ಚಿಹ್ನೆ_list *last = ಚಿಹ್ನೆ_list_last (start_symbols);
  ಪ್ರತಿ (ಚಿಹ್ನೆ_list *l = syms; l && l->content.sym; /* nothing */)
    {
      /* Is there a previous definition?  */
      ಚಿಹ್ನೆ_list *first = ಚಿಹ್ನೆ_list_find_ಚಿಹ್ನೆ (start_symbols, l->content.sym);
      ಯದಿ (first)
        {
          duplicate_directive ("%start", first->sym_loc, l->sym_loc);
          ಚಿಹ್ನೆ_list *dupl = l;
          l = l->next;
          dupl->next = ರಿಕ್ತ;
          ಚಿಹ್ನೆ_list_free (dupl);
        }
      ಅನ್ಯಥಾ
        {
          ಯದಿ (last)
            {
              last->next = l;
              last = l;
            }
          ಅನ್ಯಥಾ
            {
              last = l;
              start_symbols = last;
            }
          ಚಿಹ್ನೆ_list *next = l->next;
          l->next = ರಿಕ್ತ;
          l = next;
        }
    }
}



/*------------------------------------------------------------------------.
| Return the merger index for a merging function named NAME.  Records the |
| function, if new, in MERGER_LIST.                                       |
`------------------------------------------------------------------------*/

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
get_merge_function (uniqstr name)
{
  ಯದಿ (! glr_parser)
    ಪ್ರತ್ಯಾಗಮ 0;

  merger_list *syms;
  merger_list head;
  ಪೂರ್ಣಾಙ್ಕ n;

  head.next = merge_functions;
  ಪ್ರತಿ (syms = &head, n = 1; syms->next; syms = syms->next, n += 1)
    ಯದಿ (UNIQSTR_EQ (name, syms->next->name))
      ಭಙ್ಗ;
  ಯದಿ (syms->next == ರಿಕ್ತ)
    {
      syms->next = xmalloc (ಪರಿಮಾಣ syms->next[0]);
      syms->next->name = uniqstr_new (name);
      /* After all symbol type declarations have been parsed, packgram invokes
         record_merge_function_type to set the type.  */
      syms->next->sym = ರಿಕ್ತ;
      syms->next->next = ರಿಕ್ತ;
      merge_functions = head.next;
    }
  ಪ್ರತ್ಯಾಗಮ n;
}

/*-------------------------------------------------------------------.
| For the existing merging function with index MERGER, record that   |
| the result type is that of SYM, as required by the lhs (i.e., SYM) |
| of the rule whose %merge declaration is at DECLARATION_LOC.        |
`-------------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
record_merge_function_type (ಪೂರ್ಣಾಙ್ಕ merger, ಚಿಹ್ನೆ *sym, location declaration_loc)
{
  ಯದಿ (merger <= 0)
    ಪ್ರತ್ಯಾಗಮ;

  uniqstr type
    = sym->content->type_name ? sym->content->type_name : uniqstr_new ("");

  merger_list *merge_function;
  ಪೂರ್ಣಾಙ್ಕ merger_find = 1;
  ಪ್ರತಿ (merge_function = merge_functions;
       merge_function != ರಿಕ್ತ && merger_find != merger;
       merge_function = merge_function->next)
    merger_find += 1;
  aver (merge_function != ರಿಕ್ತ && merger_find == merger);
  ಯದಿ (merge_function->sym && merge_function->sym->content->type_name)
    {
      ಯದಿ (!UNIQSTR_EQ (merge_function->sym->content->type_name, type))
        {
          complain (&declaration_loc, complaint,
                    _("result type clash on merge function %s: "
                      "<%s> != <%s>"),
                    quote (merge_function->name), type,
                    merge_function->sym->content->type_name);
          subcomplain (&merge_function->type_declaration_loc, complaint,
                       _("previous declaration"));
        }
    }
  ಅನ್ಯಥಾ
    {
      merge_function->sym = sym;
      merge_function->type_declaration_loc = declaration_loc;
    }
}

/*--------------------------------------.
| Free all merge-function definitions.  |
`--------------------------------------*/

ಶೂನ್ಯ
free_merger_functions (ಶೂನ್ಯ)
{
  merger_list *L0 = merge_functions;
  ಯಾವತ್ (L0)
    {
      merger_list *L1 = L0->next;
      ಮುಕ್ತ (L0);
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
ಸ್ಥಿರ ಚಿಹ್ನೆ_list *grammar_end = ರಿಕ್ತ;

/* Append SYM to the grammar.  */
ಸ್ಥಿರ ಚಿಹ್ನೆ_list *
grammar_ಚಿಹ್ನೆ_append (ಚಿಹ್ನೆ *sym, location loc)
{
  ಚಿಹ್ನೆ_list *p = ಚಿಹ್ನೆ_list_sym_new (sym, loc);

  ಯದಿ (grammar_end)
    grammar_end->next = p;
  ಅನ್ಯಥಾ
    grammar = p;

  grammar_end = p;

  /* A null SYM stands for an end of rule; it is not an actual
     part of it.  */
  ಯದಿ (sym)
    ++nritems;

  ಪ್ರತ್ಯಾಗಮ p;
}

ಸ್ಥಿರ ಶೂನ್ಯ
assign_named_ref (ಚಿಹ್ನೆ_list *p, named_ref *name)
{
  ಚಿಹ್ನೆ *sym = p->content.sym;

  ಯದಿ (name->id == sym->tag)
    {
      complain (&name->loc, Wother,
                _("duplicated symbol name for %s ignored"),
                quote (sym->tag));
      named_ref_free (name);
    }
  ಅನ್ಯಥಾ
    p->named_ref = name;
}


/* The rule currently being defined, and the previous rule.
   CURRENT_RULE points to the first LHS of the current rule, while
   PREVIOUS_RULE_END points to the *end* of the previous rule (NULL).  */
ಸ್ಥಿರ ಚಿಹ್ನೆ_list *current_rule = ರಿಕ್ತ;
ಸ್ಥಿರ ಚಿಹ್ನೆ_list *previous_rule_end = ರಿಕ್ತ;


/*----------------------------------------------.
| Create a new rule for LHS in to the GRAMMAR.  |
`----------------------------------------------*/

ಶೂನ್ಯ
grammar_current_rule_begin (ಚಿಹ್ನೆ *lhs, location loc,
                            named_ref *lhs_name)
{
  /* Start a new rule and record its lhs.  */
  ++nrules;
  previous_rule_end = grammar_end;

  current_rule = grammar_ಚಿಹ್ನೆ_append (lhs, loc);
  ಯದಿ (lhs_name)
    assign_named_ref (current_rule, named_ref_copy (lhs_name));

  /* Mark the rule's lhs as a nonterminal if not already so.  */
  ಯದಿ (lhs->content->class == unknown_sym || lhs->content->class == pct_type_sym)
    ಚಿಹ್ನೆ_class_set (lhs, nterm_sym, empty_loc, false);
  ಅನ್ಯಥಾ ಯದಿ (lhs->content->class == token_sym)
    complain (&loc, complaint, _("rule given for %s, which is a token"),
              lhs->tag);
}


/*----------------------------------------------------------------------.
| A symbol should be used if either:                                    |
|   1. It has a destructor.                                             |
|   2. The symbol is a midrule symbol (i.e., the generated LHS          |
|      replacing a midrule action) that was assigned to or used, as in  |
|      "exp: { $$ = 1; } { $$ = $1; }".                                 |
`----------------------------------------------------------------------*/

ಸ್ಥಿರ bool
ಚಿಹ್ನೆ_should_be_used (ಚಿಹ್ನೆ_list ಧ್ರುವ *s, bool *midrule_warning)
{
  ಯದಿ (ಚಿಹ್ನೆ_code_props_get (s->content.sym, destructor)->code)
    ಪ್ರತ್ಯಾಗಮ true;
  ಯದಿ ((s->midrule && s->midrule->action_props.is_ಮೌಲ್ಯ_used)
      || (s->midrule_parent_rule
          && (ಚಿಹ್ನೆ_list_n_get (s->midrule_parent_rule,
                                 s->midrule_parent_rhs_index)
              ->action_props.is_ಮೌಲ್ಯ_used)))
    {
      *midrule_warning = true;
      ಪ್ರತ್ಯಾಗಮ true;
    }
  ಪ್ರತ್ಯಾಗಮ false;
}

/*-----------------------------------------------------------------.
| Check that the rule R is properly defined.  For instance, there  |
| should be no type clash on the default action.  Possibly install |
| the default action.                                              |
`-----------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
grammar_rule_check_and_complete (ಚಿಹ್ನೆ_list *r)
{
  ಧ್ರುವ ಚಿಹ್ನೆ *lhs = r->content.sym;
  ಧ್ರುವ ಚಿಹ್ನೆ *first_rhs = r->next->content.sym;

  /* Type check.

     If there is an action, then there is nothing we can do: the user
     is allowed to shoot herself in the foot.

     Don't worry about the default action if $$ is untyped, since $$'s
     value can't be used.  */
  ಯದಿ (!r->action_props.code && lhs->content->type_name)
    {
      /* If $$ is being set in default way, report if any type mismatch.  */
      ಯದಿ (first_rhs)
        {
          ವರ್ಣ ಧ್ರುವ *lhs_type = lhs->content->type_name;
          ವರ್ಣ ಧ್ರುವ *rhs_type =
            first_rhs->content->type_name ? first_rhs->content->type_name : "";
          ಯದಿ (!UNIQSTR_EQ (lhs_type, rhs_type))
            complain (&r->rhs_loc, Wother,
                      _("type clash on default action: <%s> != <%s>"),
                      lhs_type, rhs_type);
          ಅನ್ಯಥಾ
            {
              /* Install the default action only for C++.  */
              ಧ್ರುವ bool is_cxx =
                STREQ (language->language, "c++")
                || (skeleton && (STREQ (skeleton, "glr.cc")
                                 || STREQ (skeleton, "glr2.cc")
                                 || STREQ (skeleton, "lalr1.cc")));
              ಯದಿ (is_cxx)
                {
                  code_props_rule_action_init (&r->action_props, "{ $$ = $1; }",
                                               r->rhs_loc, r,
                                               /* name */ ರಿಕ್ತ,
                                               /* type */ ರಿಕ್ತ,
                                               /* is_predicate */ false);
                  code_props_translate_code (&r->action_props);
                }
            }
        }
      /* Warn if there is no default for $$ but we need one.  */
      ಅನ್ಯಥಾ
        complain (&r->rhs_loc, Wother,
                  _("empty rule for typed nonterminal, and no action"));
    }

  /* For each start symbol, build the action of its start rule.  Use
     the same obstack as the one used by scan-code, which is in charge
     of actions. */
  ಧ್ರುವ bool multistart = start_symbols && start_symbols->next;
  ಯದಿ (multistart && lhs == acceptsymbol)
    {
      ಧ್ರುವ ಚಿಹ್ನೆ *start = r->next->next->content.sym;
      ಯದಿ (start->content->type_name)
        obstack_printf (obstack_for_actions,
                        "{ ]b4_accept""([%s%d])[; }",
                        start->content->class == nterm_sym ? "orig " : "",
                        start->content->ಸಂಖ್ಯೆ);
      ಅನ್ಯಥಾ
        obstack_printf (obstack_for_actions,
                        "{ ]b4_accept[; }");
      code_props_rule_action_init (&r->action_props,
                                   obstack_finish0 (obstack_for_actions),
                                   r->rhs_loc, r,
                                   /* name */ ರಿಕ್ತ,
                                   /* type */ ರಿಕ್ತ,
                                   /* is_predicate */ false);
    }


  /* Check that symbol values that should be used are in fact used.
     Don't check the generated start rules.  It has no action, so some
     rhs symbols may appear unused, but the parsing algorithm ensures
     that %destructor's are invoked appropriately.  */
  ಯದಿ (lhs != acceptsymbol)
    {
      ಪೂರ್ಣಾಙ್ಕ n = 0;
      ಪ್ರತಿ (ಚಿಹ್ನೆ_list ಧ್ರುವ *l = r; l && l->content.sym; l = l->next, ++n)
        {
          bool midrule_warning = false;
          ಯದಿ (!l->action_props.is_ಮೌಲ್ಯ_used
              && ಚಿಹ್ನೆ_should_be_used (l, &midrule_warning)
              /* The default action, $$ = $1, 'uses' both.  */
              && (r->action_props.code || (n != 0 && n != 1)))
            {
              warnings warn_flag = midrule_warning ? Wmidrule_ಮೌಲ್ಯಗಳು : Wother;
              ಯದಿ (n)
                complain (&l->sym_loc, warn_flag, _("unused value: $%d"), n);
              ಅನ್ಯಥಾ
                complain (&l->rhs_loc, warn_flag, _("unset value: $$"));
            }
        }
    }

  /* Check that %empty => empty rule.  */
  ಯದಿ (r->percent_empty_loc.start.file
      && r->next && r->next->content.sym)
    {
      complain (&r->percent_empty_loc, complaint,
                _("%%empty on non-empty rule"));
      fixits_register (&r->percent_empty_loc, "");
    }

  /* Check that empty rule => %empty.  */
  ಯದಿ (!(r->next && r->next->content.sym)
      && !r->midrule_parent_rule
      && !r->percent_empty_loc.start.file
      && warning_is_enabled (Wempty_rule))
    {
      complain (&r->rhs_loc, Wempty_rule, _("empty rule without %%empty"));
      ಯದಿ (feature_flag & feature_caret)
        location_caret_suggestion (r->rhs_loc, "%empty", stderr);
      location loc = r->rhs_loc;
      loc.end = loc.start;
      fixits_register (&loc, " %empty ");
    }

  /* See comments in grammar_current_rule_prec_set for how POSIX
     mandates this complaint.  It's only for identifiers, so skip
     it for char literals and strings, which are always tokens.  */
  ಯದಿ (r->ruleprec
      && r->ruleprec->tag[0] != '\'' && r->ruleprec->tag[0] != '"'
      && r->ruleprec->content->status != declared
      && !r->ruleprec->content->prec)
    complain (&r->rhs_loc, Wother,
              _("token for %%prec is not defined: %s"), r->ruleprec->tag);

  /* Check that the (main) action was not typed.  */
  ಯದಿ (r->action_props.type)
    complain (&r->rhs_loc, Wother,
              _("only midrule actions can be typed: %s"), r->action_props.type);
}


/*-------------------------------------.
| End the currently being grown rule.  |
`-------------------------------------*/

ಶೂನ್ಯ
grammar_current_rule_end (location loc)
{
  /* Put an empty link in the list to mark the end of this rule  */
  grammar_ಚಿಹ್ನೆ_append (ರಿಕ್ತ, grammar_end->rhs_loc);
  current_rule->rhs_loc = loc;
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
  location dummy_loc = current_rule->action_props.location;
  ಚಿಹ್ನೆ *dummy = dummy_ಚಿಹ್ನೆ_get (dummy_loc);
  ಚಿಹ್ನೆ_type_set (dummy,
                   current_rule->action_props.type, current_rule->action_props.location);
  ಚಿಹ್ನೆ_list *midrule = ಚಿಹ್ನೆ_list_sym_new (dummy, dummy_loc);

  /* Remember named_ref of previous action. */
  named_ref *action_name = current_rule->action_props.named_ref;

  /* Make a new rule, whose body is empty, before the current one, so
     that the action just read can belong to it.  */
  ++nrules;
  ++nritems;
  /* Attach its location and actions to that of the DUMMY.  */
  midrule->rhs_loc = dummy_loc;
  code_props_rule_action_init (&midrule->action_props,
                               current_rule->action_props.code,
                               current_rule->action_props.location,
                               midrule,
                               /* name_ref */ ರಿಕ್ತ,
                               /* type */ ರಿಕ್ತ,
                               current_rule->action_props.is_predicate);
  code_props_none_init (&current_rule->action_props);

  midrule->expected_sr_conflicts = current_rule->expected_sr_conflicts;
  midrule->expected_rr_conflicts = current_rule->expected_rr_conflicts;
  current_rule->expected_sr_conflicts = -1;
  current_rule->expected_rr_conflicts = -1;

  ಯದಿ (previous_rule_end)
    previous_rule_end->next = midrule;
  ಅನ್ಯಥಾ
    grammar = midrule;

  /* End the dummy's rule.  */
  midrule->next = ಚಿಹ್ನೆ_list_sym_new (ರಿಕ್ತ, dummy_loc);
  midrule->next->next = current_rule;

  previous_rule_end = midrule->next;

  /* Insert the dummy nonterminal replacing the midrule action into
     the current rule.  Bind it to its dedicated rule.  */
  grammar_current_rule_ಚಿಹ್ನೆ_append (dummy, dummy_loc,
                                      action_name);
  grammar_end->midrule = midrule;
  midrule->midrule_parent_rule = current_rule;
  midrule->midrule_parent_rhs_index = ಚಿಹ್ನೆ_list_length (current_rule->next);
}

/* Set the precedence symbol of the current rule to PRECSYM. */

ಶೂನ್ಯ
grammar_current_rule_prec_set (ಚಿಹ್ನೆ *precsym, location loc)
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
  ಚಿಹ್ನೆ_class_set (precsym, token_sym, loc, false);
  ಯದಿ (current_rule->ruleprec)
    duplicate_rule_directive ("%prec",
                              current_rule->ruleprec->location, loc);
  ಅನ್ಯಥಾ
    current_rule->ruleprec = precsym;
}

/* Set %empty for the current rule. */

ಶೂನ್ಯ
grammar_current_rule_empty_set (location loc)
{
  /* If %empty is used and -Wno-empty-rule is not, then enable
     -Wempty-rule.  */
  ಯದಿ (warning_is_unset (Wempty_rule))
    warning_argmatch ("empty-rule", 0, 0);
  ಯದಿ (current_rule->percent_empty_loc.start.file)
    duplicate_rule_directive ("%empty",
                              current_rule->percent_empty_loc, loc);
  ಅನ್ಯಥಾ
    current_rule->percent_empty_loc = loc;
}

/* Attach dynamic precedence DPREC to the current rule. */

ಶೂನ್ಯ
grammar_current_rule_dprec_set (ಪೂರ್ಣಾಙ್ಕ dprec, location loc)
{
  ಯದಿ (! glr_parser)
    complain (&loc, Wother, _("%s affects only GLR parsers"),
              "%dprec");
  ಯದಿ (dprec <= 0)
    complain (&loc, complaint, _("%s must be followed by positive number"),
              "%dprec");
  ಅನ್ಯಥಾ ಯದಿ (current_rule->dprec != 0)
    duplicate_rule_directive ("%dprec",
                              current_rule->dprec_loc, loc);
  ಅನ್ಯಥಾ
    {
      current_rule->dprec = dprec;
      current_rule->dprec_loc = loc;
    }
}

/* Attach a merge function NAME with argument type TYPE to current
   rule. */

ಶೂನ್ಯ
grammar_current_rule_merge_set (uniqstr name, location loc)
{
  ಯದಿ (! glr_parser)
    complain (&loc, Wother, _("%s affects only GLR parsers"),
              "%merge");
  ಯದಿ (current_rule->merger != 0)
    duplicate_rule_directive ("%merge",
                              current_rule->merger_declaration_loc, loc);
  ಅನ್ಯಥಾ
    {
      current_rule->merger = get_merge_function (name);
      current_rule->merger_declaration_loc = loc;
    }
}

/* Attach SYM to the current rule.  If needed, move the previous
   action as a midrule action.  */

ಶೂನ್ಯ
grammar_current_rule_ಚಿಹ್ನೆ_append (ಚಿಹ್ನೆ *sym, location loc,
                                    named_ref *name)
{
  ಯದಿ (current_rule->action_props.code)
    grammar_midrule_action ();
  ಚಿಹ್ನೆ_list *p = grammar_ಚಿಹ್ನೆ_append (sym, loc);
  ಯದಿ (name)
    assign_named_ref (p, name);
  ಯದಿ (sym->content->status == undeclared || sym->content->status == used)
    sym->content->status = needed;
}

ಶೂನ್ಯ
grammar_current_rule_action_append (ಧ್ರುವ ವರ್ಣ *action, location loc,
                                    named_ref *name, uniqstr type)
{
  ಯದಿ (current_rule->action_props.code)
    grammar_midrule_action ();
  ಯದಿ (type)
    complain (&loc, Wyacc,
              _("POSIX Yacc does not support typed midrule actions"));
  /* After all symbol declarations have been parsed, packgram invokes
     code_props_translate_code.  */
  code_props_rule_action_init (&current_rule->action_props, action, loc,
                               current_rule,
                               name, type,
                               /* is_predicate */ false);
}

ಶೂನ್ಯ
grammar_current_rule_predicate_append (ಧ್ರುವ ವರ್ಣ *pred, location loc)
{
  ಯದಿ (current_rule->action_props.code)
    grammar_midrule_action ();
  code_props_rule_action_init (&current_rule->action_props, pred, loc,
                               current_rule,
                               ರಿಕ್ತ, ರಿಕ್ತ,
                               /* is_predicate */ true);
}

/* Set the expected number of shift/reduce (reduce/reduce) conflicts
 * for the current rule.  If a midrule is encountered later, the count
 * is transferred to it and reset in the current rule to -1. */

ಶೂನ್ಯ
grammar_current_rule_expect_sr (ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, location loc)
{
  (ಶೂನ್ಯ) loc;
  current_rule->expected_sr_conflicts = ಎಣಿಕೆ;
}

ಶೂನ್ಯ
grammar_current_rule_expect_rr (ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, location loc)
{
  ಯದಿ (! glr_parser)
    complain (&loc, Wother, _("%s affects only GLR parsers"),
              "%expect-rr");
  ಅನ್ಯಥಾ
    current_rule->expected_rr_conflicts = ಎಣಿಕೆ;
}


/*---------------------------------------------.
| Build RULES and RITEM from what was parsed.  |
`---------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
packgram (ಶೂನ್ಯ)
{
  ಪೂರ್ಣಾಙ್ಕ itemno = 0;
  ritem = xnmalloc (nritems + 1, ಪರಿಮಾಣ *ritem);
  /* This sentinel is used by build_relations() in lalr.c.  */
  *ritem++ = 0;

  rule_ಸಂಖ್ಯೆ ruleno = 0;
  rules = xnmalloc (nrules, ಪರಿಮಾಣ *rules);

  ಪ್ರತಿ (ಚಿಹ್ನೆ_list *p = grammar; p; p = p->next)
    {
      ಚಿಹ್ನೆ_list *lhs = p;
      record_merge_function_type (lhs->merger, lhs->content.sym,
                                  lhs->merger_declaration_loc);
      /* If the midrule's $$ is set or its $n is used, remove the '$' from the
         symbol name so that it's a user-defined symbol so that the default
         %destructor and %printer apply.  */
      ಯದಿ (lhs->midrule_parent_rule /* i.e., symbol_is_dummy (lhs->content.sym).  */
          && (lhs->action_props.is_ಮೌಲ್ಯ_used
              || (ಚಿಹ್ನೆ_list_n_get (lhs->midrule_parent_rule,
                                     lhs->midrule_parent_rhs_index)
                  ->action_props.is_ಮೌಲ್ಯ_used)))
        lhs->content.sym->tag += 1;

      grammar_rule_check_and_complete (lhs);

      rules[ruleno].code = ruleno;
      rules[ruleno].ಸಂಖ್ಯೆ = ruleno;
      rules[ruleno].lhs = lhs->content.sym->content;
      rules[ruleno].rhs = ritem + itemno;
      rules[ruleno].prec = ರಿಕ್ತ;
      rules[ruleno].dprec = lhs->dprec;
      rules[ruleno].merger = lhs->merger;
      rules[ruleno].precsym = ರಿಕ್ತ;
      rules[ruleno].location = lhs->rhs_loc;
      rules[ruleno].useful = true;
      rules[ruleno].action = lhs->action_props.code;
      rules[ruleno].action_loc = lhs->action_props.location;
      rules[ruleno].is_predicate = lhs->action_props.is_predicate;
      rules[ruleno].expected_sr_conflicts = lhs->expected_sr_conflicts;
      rules[ruleno].expected_rr_conflicts = lhs->expected_rr_conflicts;

      /* Traverse the rhs.  */
      {
        size_t rule_length = 0;
        ಪ್ರತಿ (p = lhs->next; p->content.sym; p = p->next)
          {
            ++rule_length;

            /* Don't allow rule_length == INT_MAX, since that might
               cause confusion with strtol if INT_MAX == LONG_MAX.  */
            ಯದಿ (rule_length == INT_MAX)
              complain (&rules[ruleno].location, fatal, _("rule is too long"));

            /* item_number = symbol_number.
               But the former needs to contain more: negative rule numbers. */
            ritem[itemno++] =
              ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ_as_item_ಸಂಖ್ಯೆ (p->content.sym->content->ಸಂಖ್ಯೆ);
            /* A rule gets by default the precedence and associativity
               of its last token.  */
            ಯದಿ (p->content.sym->content->class == token_sym && default_prec)
              rules[ruleno].prec = p->content.sym->content;
          }
      }

      /* If this rule has a %prec,
         the specified symbol's precedence replaces the default.  */
      ಯದಿ (lhs->ruleprec)
        {
          rules[ruleno].precsym = lhs->ruleprec->content;
          rules[ruleno].prec = lhs->ruleprec->content;
        }

      /* An item ends by the rule number (negated).  */
      ritem[itemno++] = rule_ಸಂಖ್ಯೆ_as_item_ಸಂಖ್ಯೆ (ruleno);
      aver (itemno < ITEM_ಸಂಖ್ಯೆ_MAX);
      ++ruleno;
      aver (ruleno < RULE_ಸಂಖ್ಯೆ_MAX);
    }

  aver (itemno == nritems);

  ಯದಿ (trace_flag & trace_sets)
    ritem_print (stderr);
}


/*--------------------------------------------------------------.
| Read in the grammar specification and record it in the format |
| described in gram.h.                                          |
`--------------------------------------------------------------*/

ಶೂನ್ಯ
reader (ಧ್ರುವ ವರ್ಣ *gram)
{
  /* Set up symbol_table, semantic_type_table, and the built-in
     symbols.  */
  symbols_new ();

  gram_scanner_open (gram);
  parser_init ();
  gram_parse ();
  gram_scanner_close ();

  prepare_percent_define_front_end_variables ();

  ಯದಿ (complaint_status  < status_complaint)
    check_and_convert_grammar ();
}

ಸ್ಥಿರ ಶೂನ್ಯ
prepare_percent_define_front_end_variables (ಶೂನ್ಯ)
{
  /* Set %define front-end variable defaults.  */
  muscle_percent_define_default ("lr.keep-unreachable-state", "false");
  {
    /* IELR would be a better default, but LALR is historically the
       default.  */
    muscle_percent_define_default ("lr.type", "lalr");
    ವರ್ಣ *lr_type = muscle_percent_define_get ("lr.type");
    ಯದಿ (STRNEQ (lr_type, "canonical-lr"))
      muscle_percent_define_default ("lr.default-reduction", "most");
    ಅನ್ಯಥಾ
      muscle_percent_define_default ("lr.default-reduction", "accepting");
    ಮುಕ್ತ (lr_type);
  }
  muscle_percent_define_default ("tool.xsltproc", "xsltproc");

  /* Check %define front-end variables.  */
  {
    ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ * ಧ್ರುವ ಮೌಲ್ಯಗಳು[] =
      {
       "lr.type", "lr""(0)", "lalr", "ielr", "canonical-lr", ರಿಕ್ತ,
       "lr.default-reduction", "most", "consistent", "accepting", ರಿಕ್ತ,
       ರಿಕ್ತ
      };
    muscle_percent_define_check_ಮೌಲ್ಯಗಳು (ಮೌಲ್ಯಗಳು);
  }
}

/* Find the first LHS which is not a dummy.  */

ಸ್ಥಿರ ಚಿಹ್ನೆ *
find_start_ಚಿಹ್ನೆ (ಶೂನ್ಯ)
{
  ಚಿಹ್ನೆ_list *res = grammar;
  /* Skip all the possible dummy rules of the first rule.  */
  ಪ್ರತಿ (; ಚಿಹ್ನೆ_is_dummy (res->content.sym); res = res->next)
    /* Skip the LHS, and then all the RHS of the dummy rule.  */
    ಪ್ರತಿ (res = res->next; res->content.sym; res = res->next)
      ಅನುವರ್ತ;
  ಪ್ರತ್ಯಾಗಮ res->content.sym;
}


/* Insert an initial rule, whose location is that of the first rule
   (not that of the start symbol):

   $accept: SWITCHING_TOKEN START $end.  */
ಸ್ಥಿರ ಶೂನ್ಯ
create_start_rule (ಚಿಹ್ನೆ *swtok, ಚಿಹ್ನೆ *start)
{
  ಚಿಹ್ನೆ_list *initial_rule = ಚಿಹ್ನೆ_list_sym_new (acceptsymbol, empty_loc);
  initial_rule->rhs_loc = grammar->rhs_loc;
  ಚಿಹ್ನೆ_list *p = initial_rule;
  ಯದಿ (swtok)
    {
      // Cannot create the action now, as the symbols have not yet
      // been assigned their number (by symbol_pack), which we need to
      // know the type name.  So the action is created in
      // grammar_rule_check_and_complete, which is run after
      // symbol_pack.
      p->next = ಚಿಹ್ನೆ_list_sym_new (swtok, empty_loc);
      p = p->next;
    }
  p->next = ಚಿಹ್ನೆ_list_sym_new (start, empty_loc);
  p = p->next;
  p->next = ಚಿಹ್ನೆ_list_sym_new (eoftoken, empty_loc);
  p = p->next;
  p->next = ಚಿಹ್ನೆ_list_sym_new (ರಿಕ್ತ, empty_loc);
  p = p->next;
  p->next = grammar;
  nrules += 1;
  nritems += 3 + !!swtok;
  grammar = initial_rule;
}

/* Fetch (or create) a token "YY_PARSE_foo" for start symbol "foo".

   We don't use the simple "YY_FOO" because (i) we might get clashes
   with some of our symbols (e.g., cast => YY_CAST), and (ii) upcasing
   introduces possible clashes between terminal FOO and nonterminal
   foo.  */
ಚಿಹ್ನೆ *
switching_token (ಧ್ರುವ ಚಿಹ್ನೆ *start)
{
  ವರ್ಣ buf[100];
  size_t len = ಪರಿಮಾಣ buf;
  ವರ್ಣ *name = asnprintf (buf, &len, "YY_PARSE_%s", ಚಿಹ್ನೆ_id_get (start));
  ಯದಿ (!name)
    xalloc_die ();
  // Setting the location ensures deterministic symbol numbers.
  ಚಿಹ್ನೆ *res = ಚಿಹ್ನೆ_get (name, start->location);
  ಯದಿ (name != buf)
    ಮುಕ್ತ (name);
  ಚಿಹ್ನೆ_class_set (res, token_sym, start->location, false);
  ಪ್ರತ್ಯಾಗಮ res;
}

/* Create the start rules in reverse order, since they are inserted at
   the top of the grammar.  That way the rules follow the order of
   declaration to %start.  */

ಸ್ಥಿರ ಶೂನ್ಯ
create_multiple_start_rules (ಚಿಹ್ನೆ_list *start_syms)
{
  ಯದಿ (start_syms)
    {
      create_multiple_start_rules (start_syms->next);
      ಪ್ರತಿಜ್ಞಾ (start_syms->content_type == SYMLIST_ಚಿಹ್ನೆ);
      ಚಿಹ್ನೆ *start = start_syms->content.sym;
      ಚಿಹ್ನೆ *swtok = switching_token (start);
      create_start_rule (swtok, start);
    }
}

/* For each start symbol "foo", create the rule "$accept: YY_FOO
   foo $end". */
ಸ್ಥಿರ ಶೂನ್ಯ
create_start_rules (ಶೂನ್ಯ)
{
  ಯದಿ (!start_symbols)
    {
      ಚಿಹ್ನೆ *start = find_start_ಚಿಹ್ನೆ ();
      start_symbols = ಚಿಹ್ನೆ_list_sym_new (start, start->location);
    }

  ಧ್ರುವ bool several = start_symbols->next;
  ಯದಿ (several)
    create_multiple_start_rules (start_symbols);
  ಅನ್ಯಥಾ
    {
      ಚಿಹ್ನೆ *start = start_symbols->content.sym;
      create_start_rule (ರಿಕ್ತ, start);
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
check_start_symbols (ಶೂನ್ಯ)
{
  ಧ್ರುವ bool multistart = start_symbols && start_symbols->next;
  // Sanity checks on the start symbols.
  ಪ್ರತಿ (ಚಿಹ್ನೆ_list *list = start_symbols; list; list = list->next)
    {
      ಧ್ರುವ ಚಿಹ್ನೆ *start = list->content.sym;
      ಯದಿ (start->content->class == unknown_sym)
        {
          complain (&start->location, complaint,
                    _("the start symbol %s is undefined"),
                    start->tag);
          // I claim this situation is unreachable.  This is caught
          // before, and we get "symbol 'foo' is used, but is not
          // defined as a token and has no rules".
          abort ();
        }
      // If your only start symbol is a token, you're weird.
      ಯದಿ (!multistart && start->content->class == token_sym)
        complain (&start->location, complaint,
                  _("the start symbol %s is a token"),
                  start->tag);
    }
  ಯದಿ (complaint_status == status_complaint)
    exit (EXIT_FAILURE);
}


/*-------------------------------------------------------------.
| Check the grammar that has just been read, and convert it to |
| internal form.                                               |
`-------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
check_and_convert_grammar (ಶೂನ್ಯ)
{
  /* Grammar has been read.  Do some checking.  */
  ಯದಿ (nrules == 0)
    complain (ರಿಕ್ತ, fatal, _("no rules in the input grammar"));

  /* If the user did not define her EOFTOKEN, do it now. */
  ಯದಿ (!eoftoken)
    {
      eoftoken = ಚಿಹ್ನೆ_get ("YYEOF", empty_loc);
      ಯದಿ (eoftoken->content->class == nterm_sym)
        complain (&eoftoken->location, complaint,
                  _("rule given for %s, which is a token"),
                  eoftoken->tag);
      eoftoken->content->class = token_sym;
      eoftoken->content->ಸಂಖ್ಯೆ = 0;
      /* Value specified by POSIX.  */
      eoftoken->content->code = 0;
      {
        ಚಿಹ್ನೆ *alias = ಚಿಹ್ನೆ_get ("$end", empty_loc);
        ಚಿಹ್ನೆ_class_set (alias, token_sym, empty_loc, false);
        ಚಿಹ್ನೆ_make_alias (eoftoken, alias, empty_loc);
      }
    }

  /* Insert the initial rule(s).  */
  create_start_rules ();

  /* Report any undefined symbols and consider them nonterminals.  */
  symbols_check_defined ();

  ಯದಿ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ_MAXIMUM - nnterms < ntokens)
    complain (ರಿಕ್ತ, fatal, "too many symbols in input grammar (limit is %d)",
              ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ_MAXIMUM);

  nsyms = ntokens + nnterms;

  /* Assign the symbols their symbol numbers.  */
  symbols_pack ();

  check_start_symbols ();

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
  ಪ್ರತಿ (ಚಿಹ್ನೆ_list *sym = grammar; sym; sym = sym->next)
    code_props_translate_code (&sym->action_props);

  /* Convert the grammar into the format described in gram.h.  */
  packgram ();

  /* The grammar as a symbol_list is no longer needed. */
  ಚಿಹ್ನೆ_list_free (grammar);
}
