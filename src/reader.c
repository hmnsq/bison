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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ "system.h"

#അന്തര്ഭാവ <quote.h>
#അന്തര്ഭാവ <vasnprintf.h>

#അന്തര്ഭാവ "complain.h"
#അന്തര്ഭാവ "conflicts.h"
#അന്തര്ഭാവ "files.h"
#അന്തര്ഭാവ "fixits.h"
#അന്തര്ഭാവ "getargs.h"
#അന്തര്ഭാവ "gram.h"
#അന്തര്ഭാവ "muscle-tab.h"
#അന്തര്ഭാവ "reader.h"
#അന്തര്ഭാവ "symlist.h"
#അന്തര്ഭാവ "symtab.h"
#അന്തര്ഭാവ "scan-gram.h"
#അന്തര്ഭാവ "scan-code.h"

സ്ഥിതം ശൂന്യം prepare_percent_define_front_end_variables (ശൂന്യം);
സ്ഥിതം ശൂന്യം check_and_convert_grammar (ശൂന്യം);

സ്ഥിതം ചിഹ്നം_list *grammar = ശൂന്യ;
ചിഹ്നം_list *start_symbols = ശൂന്യ;
merger_list *merge_functions = ശൂന്യ;

/* Was %union seen?  */
bool union_seen = false;

/* Should rules have a default precedence?  */
bool default_prec = true;


ശൂന്യം
grammar_start_symbols_add (ചിഹ്നം_list *syms)
{
  /* Report and ignore duplicates.  Append the others to START_SYMBOLS.  */
  ചിഹ്നം_list *last = ചിഹ്നം_list_last (start_symbols);
  വേണ്ടി (ചിഹ്നം_list *l = syms; l && l->content.sym; /* nothing */)
    {
      /* Is there a previous definition?  */
      ചിഹ്നം_list *first = ചിഹ്നം_list_find_ചിഹ്നം (start_symbols, l->content.sym);
      എങ്കിൽ (first)
        {
          duplicate_directive ("%start", first->sym_loc, l->sym_loc);
          ചിഹ്നം_list *dupl = l;
          l = l->next;
          dupl->next = ശൂന്യ;
          ചിഹ്നം_list_free (dupl);
        }
      അല്ലെങ്കിൽ
        {
          എങ്കിൽ (last)
            {
              last->next = l;
              last = l;
            }
          അല്ലെങ്കിൽ
            {
              last = l;
              start_symbols = last;
            }
          ചിഹ്നം_list *next = l->next;
          l->next = ശൂന്യ;
          l = next;
        }
    }
}



/*------------------------------------------------------------------------.
| Return the merger index for a merging function named NAME.  Records the |
| function, if new, in MERGER_LIST.                                       |
`------------------------------------------------------------------------*/

സ്ഥിതം പൂർണ്ണസംഖ്യ
get_merge_function (uniqstr name)
{
  എങ്കിൽ (! glr_parser)
    തിരികെ 0;

  merger_list *syms;
  merger_list head;
  പൂർണ്ണസംഖ്യ n;

  head.next = merge_functions;
  വേണ്ടി (syms = &head, n = 1; syms->next; syms = syms->next, n += 1)
    എങ്കിൽ (UNIQSTR_EQ (name, syms->next->name))
      നിർത്തുക;
  എങ്കിൽ (syms->next == ശൂന്യ)
    {
      syms->next = xmalloc (വലുപ്പം syms->next[0]);
      syms->next->name = uniqstr_new (name);
      /* After all symbol type declarations have been parsed, packgram invokes
         record_merge_function_type to set the type.  */
      syms->next->sym = ശൂന്യ;
      syms->next->next = ശൂന്യ;
      merge_functions = head.next;
    }
  തിരികെ n;
}

/*-------------------------------------------------------------------.
| For the existing merging function with index MERGER, record that   |
| the result type is that of SYM, as required by the lhs (i.e., SYM) |
| of the rule whose %merge declaration is at DECLARATION_LOC.        |
`-------------------------------------------------------------------*/

സ്ഥിതം ശൂന്യം
record_merge_function_type (പൂർണ്ണസംഖ്യ merger, ചിഹ്നം *sym, location declaration_loc)
{
  എങ്കിൽ (merger <= 0)
    തിരികെ;

  uniqstr type
    = sym->content->type_name ? sym->content->type_name : uniqstr_new ("");

  merger_list *merge_function;
  പൂർണ്ണസംഖ്യ merger_find = 1;
  വേണ്ടി (merge_function = merge_functions;
       merge_function != ശൂന്യ && merger_find != merger;
       merge_function = merge_function->next)
    merger_find += 1;
  aver (merge_function != ശൂന്യ && merger_find == merger);
  എങ്കിൽ (merge_function->sym && merge_function->sym->content->type_name)
    {
      എങ്കിൽ (!UNIQSTR_EQ (merge_function->sym->content->type_name, type))
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
  അല്ലെങ്കിൽ
    {
      merge_function->sym = sym;
      merge_function->type_declaration_loc = declaration_loc;
    }
}

/*--------------------------------------.
| Free all merge-function definitions.  |
`--------------------------------------*/

ശൂന്യം
free_merger_functions (ശൂന്യം)
{
  merger_list *L0 = merge_functions;
  ആകുമ്പോൾ (L0)
    {
      merger_list *L1 = L0->next;
      സ്വതന്ത്രം (L0);
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
സ്ഥിതം ചിഹ്നം_list *grammar_end = ശൂന്യ;

/* Append SYM to the grammar.  */
സ്ഥിതം ചിഹ്നം_list *
grammar_ചിഹ്നം_append (ചിഹ്നം *sym, location loc)
{
  ചിഹ്നം_list *p = ചിഹ്നം_list_sym_new (sym, loc);

  എങ്കിൽ (grammar_end)
    grammar_end->next = p;
  അല്ലെങ്കിൽ
    grammar = p;

  grammar_end = p;

  /* A null SYM stands for an end of rule; it is not an actual
     part of it.  */
  എങ്കിൽ (sym)
    ++nritems;

  തിരികെ p;
}

സ്ഥിതം ശൂന്യം
assign_named_ref (ചിഹ്നം_list *p, named_ref *name)
{
  ചിഹ്നം *sym = p->content.sym;

  എങ്കിൽ (name->id == sym->tag)
    {
      complain (&name->loc, Wother,
                _("duplicated symbol name for %s ignored"),
                quote (sym->tag));
      named_ref_free (name);
    }
  അല്ലെങ്കിൽ
    p->named_ref = name;
}


/* The rule currently being defined, and the previous rule.
   CURRENT_RULE points to the first LHS of the current rule, while
   PREVIOUS_RULE_END points to the *end* of the previous rule (NULL).  */
സ്ഥിതം ചിഹ്നം_list *current_rule = ശൂന്യ;
സ്ഥിതം ചിഹ്നം_list *previous_rule_end = ശൂന്യ;


/*----------------------------------------------.
| Create a new rule for LHS in to the GRAMMAR.  |
`----------------------------------------------*/

ശൂന്യം
grammar_current_rule_begin (ചിഹ്നം *lhs, location loc,
                            named_ref *lhs_name)
{
  /* Start a new rule and record its lhs.  */
  ++nrules;
  previous_rule_end = grammar_end;

  current_rule = grammar_ചിഹ്നം_append (lhs, loc);
  എങ്കിൽ (lhs_name)
    assign_named_ref (current_rule, named_ref_copy (lhs_name));

  /* Mark the rule's lhs as a nonterminal if not already so.  */
  എങ്കിൽ (lhs->content->class == unknown_sym || lhs->content->class == pct_type_sym)
    ചിഹ്നം_class_set (lhs, nterm_sym, empty_loc, false);
  അല്ലെങ്കിൽ എങ്കിൽ (lhs->content->class == token_sym)
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

സ്ഥിതം bool
ചിഹ്നം_should_be_used (ചിഹ്നം_list സ്ഥിരം *s, bool *midrule_warning)
{
  എങ്കിൽ (ചിഹ്നം_code_props_get (s->content.sym, destructor)->code)
    തിരികെ true;
  എങ്കിൽ ((s->midrule && s->midrule->action_props.is_മൂല്യം_used)
      || (s->midrule_parent_rule
          && (ചിഹ്നം_list_n_get (s->midrule_parent_rule,
                                 s->midrule_parent_rhs_index)
              ->action_props.is_മൂല്യം_used)))
    {
      *midrule_warning = true;
      തിരികെ true;
    }
  തിരികെ false;
}

/*-----------------------------------------------------------------.
| Check that the rule R is properly defined.  For instance, there  |
| should be no type clash on the default action.  Possibly install |
| the default action.                                              |
`-----------------------------------------------------------------*/

സ്ഥിതം ശൂന്യം
grammar_rule_check_and_complete (ചിഹ്നം_list *r)
{
  സ്ഥിരം ചിഹ്നം *lhs = r->content.sym;
  സ്ഥിരം ചിഹ്നം *first_rhs = r->next->content.sym;

  /* Type check.

     If there is an action, then there is nothing we can do: the user
     is allowed to shoot herself in the foot.

     Don't worry about the default action if $$ is untyped, since $$'s
     value can't be used.  */
  എങ്കിൽ (!r->action_props.code && lhs->content->type_name)
    {
      /* If $$ is being set in default way, report if any type mismatch.  */
      എങ്കിൽ (first_rhs)
        {
          അക്ഷരം സ്ഥിരം *lhs_type = lhs->content->type_name;
          അക്ഷരം സ്ഥിരം *rhs_type =
            first_rhs->content->type_name ? first_rhs->content->type_name : "";
          എങ്കിൽ (!UNIQSTR_EQ (lhs_type, rhs_type))
            complain (&r->rhs_loc, Wother,
                      _("type clash on default action: <%s> != <%s>"),
                      lhs_type, rhs_type);
          അല്ലെങ്കിൽ
            {
              /* Install the default action only for C++.  */
              സ്ഥിരം bool is_cxx =
                STREQ (language->language, "c++")
                || (skeleton && (STREQ (skeleton, "glr.cc")
                                 || STREQ (skeleton, "glr2.cc")
                                 || STREQ (skeleton, "lalr1.cc")));
              എങ്കിൽ (is_cxx)
                {
                  code_props_rule_action_init (&r->action_props, "{ $$ = $1; }",
                                               r->rhs_loc, r,
                                               /* name */ ശൂന്യ,
                                               /* type */ ശൂന്യ,
                                               /* is_predicate */ false);
                  code_props_translate_code (&r->action_props);
                }
            }
        }
      /* Warn if there is no default for $$ but we need one.  */
      അല്ലെങ്കിൽ
        complain (&r->rhs_loc, Wother,
                  _("empty rule for typed nonterminal, and no action"));
    }

  /* For each start symbol, build the action of its start rule.  Use
     the same obstack as the one used by scan-code, which is in charge
     of actions. */
  സ്ഥിരം bool multistart = start_symbols && start_symbols->next;
  എങ്കിൽ (multistart && lhs == acceptsymbol)
    {
      സ്ഥിരം ചിഹ്നം *start = r->next->next->content.sym;
      എങ്കിൽ (start->content->type_name)
        obstack_printf (obstack_for_actions,
                        "{ ]b4_accept""([%s%d])[; }",
                        start->content->class == nterm_sym ? "orig " : "",
                        start->content->സംഖ്യ);
      അല്ലെങ്കിൽ
        obstack_printf (obstack_for_actions,
                        "{ ]b4_accept[; }");
      code_props_rule_action_init (&r->action_props,
                                   obstack_finish0 (obstack_for_actions),
                                   r->rhs_loc, r,
                                   /* name */ ശൂന്യ,
                                   /* type */ ശൂന്യ,
                                   /* is_predicate */ false);
    }


  /* Check that symbol values that should be used are in fact used.
     Don't check the generated start rules.  It has no action, so some
     rhs symbols may appear unused, but the parsing algorithm ensures
     that %destructor's are invoked appropriately.  */
  എങ്കിൽ (lhs != acceptsymbol)
    {
      പൂർണ്ണസംഖ്യ n = 0;
      വേണ്ടി (ചിഹ്നം_list സ്ഥിരം *l = r; l && l->content.sym; l = l->next, ++n)
        {
          bool midrule_warning = false;
          എങ്കിൽ (!l->action_props.is_മൂല്യം_used
              && ചിഹ്നം_should_be_used (l, &midrule_warning)
              /* The default action, $$ = $1, 'uses' both.  */
              && (r->action_props.code || (n != 0 && n != 1)))
            {
              warnings warn_flag = midrule_warning ? Wmidrule_മൂല്യങ്ങൾ : Wother;
              എങ്കിൽ (n)
                complain (&l->sym_loc, warn_flag, _("unused value: $%d"), n);
              അല്ലെങ്കിൽ
                complain (&l->rhs_loc, warn_flag, _("unset value: $$"));
            }
        }
    }

  /* Check that %empty => empty rule.  */
  എങ്കിൽ (r->percent_empty_loc.start.file
      && r->next && r->next->content.sym)
    {
      complain (&r->percent_empty_loc, complaint,
                _("%%empty on non-empty rule"));
      fixits_register (&r->percent_empty_loc, "");
    }

  /* Check that empty rule => %empty.  */
  എങ്കിൽ (!(r->next && r->next->content.sym)
      && !r->midrule_parent_rule
      && !r->percent_empty_loc.start.file
      && warning_is_enabled (Wempty_rule))
    {
      complain (&r->rhs_loc, Wempty_rule, _("empty rule without %%empty"));
      എങ്കിൽ (feature_flag & feature_caret)
        location_caret_suggestion (r->rhs_loc, "%empty", stderr);
      location loc = r->rhs_loc;
      loc.end = loc.start;
      fixits_register (&loc, " %empty ");
    }

  /* See comments in grammar_current_rule_prec_set for how POSIX
     mandates this complaint.  It's only for identifiers, so skip
     it for char literals and strings, which are always tokens.  */
  എങ്കിൽ (r->ruleprec
      && r->ruleprec->tag[0] != '\'' && r->ruleprec->tag[0] != '"'
      && r->ruleprec->content->status != declared
      && !r->ruleprec->content->prec)
    complain (&r->rhs_loc, Wother,
              _("token for %%prec is not defined: %s"), r->ruleprec->tag);

  /* Check that the (main) action was not typed.  */
  എങ്കിൽ (r->action_props.type)
    complain (&r->rhs_loc, Wother,
              _("only midrule actions can be typed: %s"), r->action_props.type);
}


/*-------------------------------------.
| End the currently being grown rule.  |
`-------------------------------------*/

ശൂന്യം
grammar_current_rule_end (location loc)
{
  /* Put an empty link in the list to mark the end of this rule  */
  grammar_ചിഹ്നം_append (ശൂന്യ, grammar_end->rhs_loc);
  current_rule->rhs_loc = loc;
}


/*-------------------------------------------------------------------.
| The previous action turns out to be a midrule action.  Attach it   |
| to the current rule, i.e., create a dummy symbol, attach it this   |
| midrule action, and append this dummy nonterminal to the current   |
| rule.                                                              |
`-------------------------------------------------------------------*/

ശൂന്യം
grammar_midrule_action (ശൂന്യം)
{
  /* Since the action was written out with this rule's number, we must
     give the new rule this number by inserting the new rule before
     it.  */

  /* Make a DUMMY nonterminal, whose location is that of the midrule
     action.  Create the MIDRULE.  */
  location dummy_loc = current_rule->action_props.location;
  ചിഹ്നം *dummy = dummy_ചിഹ്നം_get (dummy_loc);
  ചിഹ്നം_type_set (dummy,
                   current_rule->action_props.type, current_rule->action_props.location);
  ചിഹ്നം_list *midrule = ചിഹ്നം_list_sym_new (dummy, dummy_loc);

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
                               /* name_ref */ ശൂന്യ,
                               /* type */ ശൂന്യ,
                               current_rule->action_props.is_predicate);
  code_props_none_init (&current_rule->action_props);

  midrule->expected_sr_conflicts = current_rule->expected_sr_conflicts;
  midrule->expected_rr_conflicts = current_rule->expected_rr_conflicts;
  current_rule->expected_sr_conflicts = -1;
  current_rule->expected_rr_conflicts = -1;

  എങ്കിൽ (previous_rule_end)
    previous_rule_end->next = midrule;
  അല്ലെങ്കിൽ
    grammar = midrule;

  /* End the dummy's rule.  */
  midrule->next = ചിഹ്നം_list_sym_new (ശൂന്യ, dummy_loc);
  midrule->next->next = current_rule;

  previous_rule_end = midrule->next;

  /* Insert the dummy nonterminal replacing the midrule action into
     the current rule.  Bind it to its dedicated rule.  */
  grammar_current_rule_ചിഹ്നം_append (dummy, dummy_loc,
                                      action_name);
  grammar_end->midrule = midrule;
  midrule->midrule_parent_rule = current_rule;
  midrule->midrule_parent_rhs_index = ചിഹ്നം_list_length (current_rule->next);
}

/* Set the precedence symbol of the current rule to PRECSYM. */

ശൂന്യം
grammar_current_rule_prec_set (ചിഹ്നം *precsym, location loc)
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
  ചിഹ്നം_class_set (precsym, token_sym, loc, false);
  എങ്കിൽ (current_rule->ruleprec)
    duplicate_rule_directive ("%prec",
                              current_rule->ruleprec->location, loc);
  അല്ലെങ്കിൽ
    current_rule->ruleprec = precsym;
}

/* Set %empty for the current rule. */

ശൂന്യം
grammar_current_rule_empty_set (location loc)
{
  /* If %empty is used and -Wno-empty-rule is not, then enable
     -Wempty-rule.  */
  എങ്കിൽ (warning_is_unset (Wempty_rule))
    warning_argmatch ("empty-rule", 0, 0);
  എങ്കിൽ (current_rule->percent_empty_loc.start.file)
    duplicate_rule_directive ("%empty",
                              current_rule->percent_empty_loc, loc);
  അല്ലെങ്കിൽ
    current_rule->percent_empty_loc = loc;
}

/* Attach dynamic precedence DPREC to the current rule. */

ശൂന്യം
grammar_current_rule_dprec_set (പൂർണ്ണസംഖ്യ dprec, location loc)
{
  എങ്കിൽ (! glr_parser)
    complain (&loc, Wother, _("%s affects only GLR parsers"),
              "%dprec");
  എങ്കിൽ (dprec <= 0)
    complain (&loc, complaint, _("%s must be followed by positive number"),
              "%dprec");
  അല്ലെങ്കിൽ എങ്കിൽ (current_rule->dprec != 0)
    duplicate_rule_directive ("%dprec",
                              current_rule->dprec_loc, loc);
  അല്ലെങ്കിൽ
    {
      current_rule->dprec = dprec;
      current_rule->dprec_loc = loc;
    }
}

/* Attach a merge function NAME with argument type TYPE to current
   rule. */

ശൂന്യം
grammar_current_rule_merge_set (uniqstr name, location loc)
{
  എങ്കിൽ (! glr_parser)
    complain (&loc, Wother, _("%s affects only GLR parsers"),
              "%merge");
  എങ്കിൽ (current_rule->merger != 0)
    duplicate_rule_directive ("%merge",
                              current_rule->merger_declaration_loc, loc);
  അല്ലെങ്കിൽ
    {
      current_rule->merger = get_merge_function (name);
      current_rule->merger_declaration_loc = loc;
    }
}

/* Attach SYM to the current rule.  If needed, move the previous
   action as a midrule action.  */

ശൂന്യം
grammar_current_rule_ചിഹ്നം_append (ചിഹ്നം *sym, location loc,
                                    named_ref *name)
{
  എങ്കിൽ (current_rule->action_props.code)
    grammar_midrule_action ();
  ചിഹ്നം_list *p = grammar_ചിഹ്നം_append (sym, loc);
  എങ്കിൽ (name)
    assign_named_ref (p, name);
  എങ്കിൽ (sym->content->status == undeclared || sym->content->status == used)
    sym->content->status = needed;
}

ശൂന്യം
grammar_current_rule_action_append (സ്ഥിരം അക്ഷരം *action, location loc,
                                    named_ref *name, uniqstr type)
{
  എങ്കിൽ (current_rule->action_props.code)
    grammar_midrule_action ();
  എങ്കിൽ (type)
    complain (&loc, Wyacc,
              _("POSIX Yacc does not support typed midrule actions"));
  /* After all symbol declarations have been parsed, packgram invokes
     code_props_translate_code.  */
  code_props_rule_action_init (&current_rule->action_props, action, loc,
                               current_rule,
                               name, type,
                               /* is_predicate */ false);
}

ശൂന്യം
grammar_current_rule_predicate_append (സ്ഥിരം അക്ഷരം *pred, location loc)
{
  എങ്കിൽ (current_rule->action_props.code)
    grammar_midrule_action ();
  code_props_rule_action_init (&current_rule->action_props, pred, loc,
                               current_rule,
                               ശൂന്യ, ശൂന്യ,
                               /* is_predicate */ true);
}

/* Set the expected number of shift/reduce (reduce/reduce) conflicts
 * for the current rule.  If a midrule is encountered later, the count
 * is transferred to it and reset in the current rule to -1. */

ശൂന്യം
grammar_current_rule_expect_sr (പൂർണ്ണസംഖ്യ എണ്ണം, location loc)
{
  (ശൂന്യം) loc;
  current_rule->expected_sr_conflicts = എണ്ണം;
}

ശൂന്യം
grammar_current_rule_expect_rr (പൂർണ്ണസംഖ്യ എണ്ണം, location loc)
{
  എങ്കിൽ (! glr_parser)
    complain (&loc, Wother, _("%s affects only GLR parsers"),
              "%expect-rr");
  അല്ലെങ്കിൽ
    current_rule->expected_rr_conflicts = എണ്ണം;
}


/*---------------------------------------------.
| Build RULES and RITEM from what was parsed.  |
`---------------------------------------------*/

സ്ഥിതം ശൂന്യം
packgram (ശൂന്യം)
{
  പൂർണ്ണസംഖ്യ itemno = 0;
  ritem = xnmalloc (nritems + 1, വലുപ്പം *ritem);
  /* This sentinel is used by build_relations() in lalr.c.  */
  *ritem++ = 0;

  rule_സംഖ്യ ruleno = 0;
  rules = xnmalloc (nrules, വലുപ്പം *rules);

  വേണ്ടി (ചിഹ്നം_list *p = grammar; p; p = p->next)
    {
      ചിഹ്നം_list *lhs = p;
      record_merge_function_type (lhs->merger, lhs->content.sym,
                                  lhs->merger_declaration_loc);
      /* If the midrule's $$ is set or its $n is used, remove the '$' from the
         symbol name so that it's a user-defined symbol so that the default
         %destructor and %printer apply.  */
      എങ്കിൽ (lhs->midrule_parent_rule /* i.e., symbol_is_dummy (lhs->content.sym).  */
          && (lhs->action_props.is_മൂല്യം_used
              || (ചിഹ്നം_list_n_get (lhs->midrule_parent_rule,
                                     lhs->midrule_parent_rhs_index)
                  ->action_props.is_മൂല്യം_used)))
        lhs->content.sym->tag += 1;

      grammar_rule_check_and_complete (lhs);

      rules[ruleno].code = ruleno;
      rules[ruleno].സംഖ്യ = ruleno;
      rules[ruleno].lhs = lhs->content.sym->content;
      rules[ruleno].rhs = ritem + itemno;
      rules[ruleno].prec = ശൂന്യ;
      rules[ruleno].dprec = lhs->dprec;
      rules[ruleno].merger = lhs->merger;
      rules[ruleno].precsym = ശൂന്യ;
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
        വേണ്ടി (p = lhs->next; p->content.sym; p = p->next)
          {
            ++rule_length;

            /* Don't allow rule_length == INT_MAX, since that might
               cause confusion with strtol if INT_MAX == LONG_MAX.  */
            എങ്കിൽ (rule_length == INT_MAX)
              complain (&rules[ruleno].location, fatal, _("rule is too long"));

            /* item_number = symbol_number.
               But the former needs to contain more: negative rule numbers. */
            ritem[itemno++] =
              ചിഹ്നം_സംഖ്യ_as_item_സംഖ്യ (p->content.sym->content->സംഖ്യ);
            /* A rule gets by default the precedence and associativity
               of its last token.  */
            എങ്കിൽ (p->content.sym->content->class == token_sym && default_prec)
              rules[ruleno].prec = p->content.sym->content;
          }
      }

      /* If this rule has a %prec,
         the specified symbol's precedence replaces the default.  */
      എങ്കിൽ (lhs->ruleprec)
        {
          rules[ruleno].precsym = lhs->ruleprec->content;
          rules[ruleno].prec = lhs->ruleprec->content;
        }

      /* An item ends by the rule number (negated).  */
      ritem[itemno++] = rule_സംഖ്യ_as_item_സംഖ്യ (ruleno);
      aver (itemno < ITEM_സംഖ്യ_MAX);
      ++ruleno;
      aver (ruleno < RULE_സംഖ്യ_MAX);
    }

  aver (itemno == nritems);

  എങ്കിൽ (trace_flag & trace_sets)
    ritem_print (stderr);
}


/*--------------------------------------------------------------.
| Read in the grammar specification and record it in the format |
| described in gram.h.                                          |
`--------------------------------------------------------------*/

ശൂന്യം
reader (സ്ഥിരം അക്ഷരം *gram)
{
  /* Set up symbol_table, semantic_type_table, and the built-in
     symbols.  */
  symbols_new ();

  gram_scanner_open (gram);
  parser_init ();
  gram_parse ();
  gram_scanner_close ();

  prepare_percent_define_front_end_variables ();

  എങ്കിൽ (complaint_status  < status_complaint)
    check_and_convert_grammar ();
}

സ്ഥിതം ശൂന്യം
prepare_percent_define_front_end_variables (ശൂന്യം)
{
  /* Set %define front-end variable defaults.  */
  muscle_percent_define_default ("lr.keep-unreachable-state", "false");
  {
    /* IELR would be a better default, but LALR is historically the
       default.  */
    muscle_percent_define_default ("lr.type", "lalr");
    അക്ഷരം *lr_type = muscle_percent_define_get ("lr.type");
    എങ്കിൽ (STRNEQ (lr_type, "canonical-lr"))
      muscle_percent_define_default ("lr.default-reduction", "most");
    അല്ലെങ്കിൽ
      muscle_percent_define_default ("lr.default-reduction", "accepting");
    സ്വതന്ത്രം (lr_type);
  }
  muscle_percent_define_default ("tool.xsltproc", "xsltproc");

  /* Check %define front-end variables.  */
  {
    സ്ഥിതം അക്ഷരം സ്ഥിരം * സ്ഥിരം മൂല്യങ്ങൾ[] =
      {
       "lr.type", "lr""(0)", "lalr", "ielr", "canonical-lr", ശൂന്യ,
       "lr.default-reduction", "most", "consistent", "accepting", ശൂന്യ,
       ശൂന്യ
      };
    muscle_percent_define_check_മൂല്യങ്ങൾ (മൂല്യങ്ങൾ);
  }
}

/* Find the first LHS which is not a dummy.  */

സ്ഥിതം ചിഹ്നം *
find_start_ചിഹ്നം (ശൂന്യം)
{
  ചിഹ്നം_list *res = grammar;
  /* Skip all the possible dummy rules of the first rule.  */
  വേണ്ടി (; ചിഹ്നം_is_dummy (res->content.sym); res = res->next)
    /* Skip the LHS, and then all the RHS of the dummy rule.  */
    വേണ്ടി (res = res->next; res->content.sym; res = res->next)
      തുടരുക;
  തിരികെ res->content.sym;
}


/* Insert an initial rule, whose location is that of the first rule
   (not that of the start symbol):

   $accept: SWITCHING_TOKEN START $end.  */
സ്ഥിതം ശൂന്യം
create_start_rule (ചിഹ്നം *swtok, ചിഹ്നം *start)
{
  ചിഹ്നം_list *initial_rule = ചിഹ്നം_list_sym_new (acceptsymbol, empty_loc);
  initial_rule->rhs_loc = grammar->rhs_loc;
  ചിഹ്നം_list *p = initial_rule;
  എങ്കിൽ (swtok)
    {
      // Cannot create the action now, as the symbols have not yet
      // been assigned their number (by symbol_pack), which we need to
      // know the type name.  So the action is created in
      // grammar_rule_check_and_complete, which is run after
      // symbol_pack.
      p->next = ചിഹ്നം_list_sym_new (swtok, empty_loc);
      p = p->next;
    }
  p->next = ചിഹ്നം_list_sym_new (start, empty_loc);
  p = p->next;
  p->next = ചിഹ്നം_list_sym_new (eoftoken, empty_loc);
  p = p->next;
  p->next = ചിഹ്നം_list_sym_new (ശൂന്യ, empty_loc);
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
ചിഹ്നം *
switching_token (സ്ഥിരം ചിഹ്നം *start)
{
  അക്ഷരം buf[100];
  size_t len = വലുപ്പം buf;
  അക്ഷരം *name = asnprintf (buf, &len, "YY_PARSE_%s", ചിഹ്നം_id_get (start));
  എങ്കിൽ (!name)
    xalloc_die ();
  // Setting the location ensures deterministic symbol numbers.
  ചിഹ്നം *res = ചിഹ്നം_get (name, start->location);
  എങ്കിൽ (name != buf)
    സ്വതന്ത്രം (name);
  ചിഹ്നം_class_set (res, token_sym, start->location, false);
  തിരികെ res;
}

/* Create the start rules in reverse order, since they are inserted at
   the top of the grammar.  That way the rules follow the order of
   declaration to %start.  */

സ്ഥിതം ശൂന്യം
create_multiple_start_rules (ചിഹ്നം_list *start_syms)
{
  എങ്കിൽ (start_syms)
    {
      create_multiple_start_rules (start_syms->next);
      പ്രതിജ്ഞാ (start_syms->content_type == SYMLIST_ചിഹ്നം);
      ചിഹ്നം *start = start_syms->content.sym;
      ചിഹ്നം *swtok = switching_token (start);
      create_start_rule (swtok, start);
    }
}

/* For each start symbol "foo", create the rule "$accept: YY_FOO
   foo $end". */
സ്ഥിതം ശൂന്യം
create_start_rules (ശൂന്യം)
{
  എങ്കിൽ (!start_symbols)
    {
      ചിഹ്നം *start = find_start_ചിഹ്നം ();
      start_symbols = ചിഹ്നം_list_sym_new (start, start->location);
    }

  സ്ഥിരം bool several = start_symbols->next;
  എങ്കിൽ (several)
    create_multiple_start_rules (start_symbols);
  അല്ലെങ്കിൽ
    {
      ചിഹ്നം *start = start_symbols->content.sym;
      create_start_rule (ശൂന്യ, start);
    }
}

സ്ഥിതം ശൂന്യം
check_start_symbols (ശൂന്യം)
{
  സ്ഥിരം bool multistart = start_symbols && start_symbols->next;
  // Sanity checks on the start symbols.
  വേണ്ടി (ചിഹ്നം_list *list = start_symbols; list; list = list->next)
    {
      സ്ഥിരം ചിഹ്നം *start = list->content.sym;
      എങ്കിൽ (start->content->class == unknown_sym)
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
      എങ്കിൽ (!multistart && start->content->class == token_sym)
        complain (&start->location, complaint,
                  _("the start symbol %s is a token"),
                  start->tag);
    }
  എങ്കിൽ (complaint_status == status_complaint)
    exit (EXIT_FAILURE);
}


/*-------------------------------------------------------------.
| Check the grammar that has just been read, and convert it to |
| internal form.                                               |
`-------------------------------------------------------------*/

സ്ഥിതം ശൂന്യം
check_and_convert_grammar (ശൂന്യം)
{
  /* Grammar has been read.  Do some checking.  */
  എങ്കിൽ (nrules == 0)
    complain (ശൂന്യ, fatal, _("no rules in the input grammar"));

  /* If the user did not define her EOFTOKEN, do it now. */
  എങ്കിൽ (!eoftoken)
    {
      eoftoken = ചിഹ്നം_get ("YYEOF", empty_loc);
      എങ്കിൽ (eoftoken->content->class == nterm_sym)
        complain (&eoftoken->location, complaint,
                  _("rule given for %s, which is a token"),
                  eoftoken->tag);
      eoftoken->content->class = token_sym;
      eoftoken->content->സംഖ്യ = 0;
      /* Value specified by POSIX.  */
      eoftoken->content->code = 0;
      {
        ചിഹ്നം *alias = ചിഹ്നം_get ("$end", empty_loc);
        ചിഹ്നം_class_set (alias, token_sym, empty_loc, false);
        ചിഹ്നം_make_alias (eoftoken, alias, empty_loc);
      }
    }

  /* Insert the initial rule(s).  */
  create_start_rules ();

  /* Report any undefined symbols and consider them nonterminals.  */
  symbols_check_defined ();

  എങ്കിൽ (ചിഹ്നം_സംഖ്യ_MAXIMUM - nnterms < ntokens)
    complain (ശൂന്യ, fatal, "too many symbols in input grammar (limit is %d)",
              ചിഹ്നം_സംഖ്യ_MAXIMUM);

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
  വേണ്ടി (ചിഹ്നം_list *sym = grammar; sym; sym = sym->next)
    code_props_translate_code (&sym->action_props);

  /* Convert the grammar into the format described in gram.h.  */
  packgram ();

  /* The grammar as a symbol_list is no longer needed. */
  ചിഹ്നം_list_free (grammar);
}
