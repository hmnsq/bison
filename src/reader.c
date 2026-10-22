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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ "system.h"

#அந்தர்பாவ <quote.h>
#அந்தர்பாவ <vasnprintf.h>

#அந்தர்பாவ "complain.h"
#அந்தர்பாவ "conflicts.h"
#அந்தர்பாவ "files.h"
#அந்தர்பாவ "fixits.h"
#அந்தர்பாவ "getargs.h"
#அந்தர்பாவ "gram.h"
#அந்தர்பாவ "muscle-tab.h"
#அந்தர்பாவ "reader.h"
#அந்தர்பாவ "symlist.h"
#அந்தர்பாவ "symtab.h"
#அந்தர்பாவ "scan-gram.h"
#அந்தர்பாவ "scan-code.h"

நிலையான வெறுமை prepare_percent_define_front_end_variables (வெறுமை);
நிலையான வெறுமை check_and_convert_grammar (வெறுமை);

நிலையான குறியீடு_list *grammar = வெற்று;
குறியீடு_list *start_symbols = வெற்று;
merger_list *merge_functions = வெற்று;

/* Was %union seen?  */
bool union_seen = false;

/* Should rules have a default precedence?  */
bool default_prec = true;


வெறுமை
grammar_start_symbols_add (குறியீடு_list *syms)
{
  /* Report and ignore duplicates.  Append the others to START_SYMBOLS.  */
  குறியீடு_list *last = குறியீடு_list_last (start_symbols);
  க்கு (குறியீடு_list *l = syms; l && l->content.sym; /* nothing */)
    {
      /* Is there a previous definition?  */
      குறியீடு_list *first = குறியீடு_list_find_குறியீடு (start_symbols, l->content.sym);
      எனில் (first)
        {
          duplicate_directive ("%start", first->sym_loc, l->sym_loc);
          குறியீடு_list *dupl = l;
          l = l->next;
          dupl->next = வெற்று;
          குறியீடு_list_free (dupl);
        }
      இல்லையெனில்
        {
          எனில் (last)
            {
              last->next = l;
              last = l;
            }
          இல்லையெனில்
            {
              last = l;
              start_symbols = last;
            }
          குறியீடு_list *next = l->next;
          l->next = வெற்று;
          l = next;
        }
    }
}



/*------------------------------------------------------------------------.
| Return the merger index for a merging function named NAME.  Records the |
| function, if new, in MERGER_LIST.                                       |
`------------------------------------------------------------------------*/

நிலையான முழு
get_merge_function (uniqstr name)
{
  எனில் (! glr_parser)
    திரும்பு 0;

  merger_list *syms;
  merger_list head;
  முழு n;

  head.next = merge_functions;
  க்கு (syms = &head, n = 1; syms->next; syms = syms->next, n += 1)
    எனில் (UNIQSTR_EQ (name, syms->next->name))
      நிறுத்து;
  எனில் (syms->next == வெற்று)
    {
      syms->next = xmalloc (அளவு syms->next[0]);
      syms->next->name = uniqstr_new (name);
      /* After all symbol type declarations have been parsed, packgram invokes
         record_merge_function_type to set the type.  */
      syms->next->sym = வெற்று;
      syms->next->next = வெற்று;
      merge_functions = head.next;
    }
  திரும்பு n;
}

/*-------------------------------------------------------------------.
| For the existing merging function with index MERGER, record that   |
| the result type is that of SYM, as required by the lhs (i.e., SYM) |
| of the rule whose %merge declaration is at DECLARATION_LOC.        |
`-------------------------------------------------------------------*/

நிலையான வெறுமை
record_merge_function_type (முழு merger, குறியீடு *sym, location declaration_loc)
{
  எனில் (merger <= 0)
    திரும்பு;

  uniqstr type
    = sym->content->type_name ? sym->content->type_name : uniqstr_new ("");

  merger_list *merge_function;
  முழு merger_find = 1;
  க்கு (merge_function = merge_functions;
       merge_function != வெற்று && merger_find != merger;
       merge_function = merge_function->next)
    merger_find += 1;
  aver (merge_function != வெற்று && merger_find == merger);
  எனில் (merge_function->sym && merge_function->sym->content->type_name)
    {
      எனில் (!UNIQSTR_EQ (merge_function->sym->content->type_name, type))
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
  இல்லையெனில்
    {
      merge_function->sym = sym;
      merge_function->type_declaration_loc = declaration_loc;
    }
}

/*--------------------------------------.
| Free all merge-function definitions.  |
`--------------------------------------*/

வெறுமை
free_merger_functions (வெறுமை)
{
  merger_list *L0 = merge_functions;
  போது (L0)
    {
      merger_list *L1 = L0->next;
      விடுவி (L0);
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
நிலையான குறியீடு_list *grammar_end = வெற்று;

/* Append SYM to the grammar.  */
நிலையான குறியீடு_list *
grammar_குறியீடு_append (குறியீடு *sym, location loc)
{
  குறியீடு_list *p = குறியீடு_list_sym_new (sym, loc);

  எனில் (grammar_end)
    grammar_end->next = p;
  இல்லையெனில்
    grammar = p;

  grammar_end = p;

  /* A null SYM stands for an end of rule; it is not an actual
     part of it.  */
  எனில் (sym)
    ++nritems;

  திரும்பு p;
}

நிலையான வெறுமை
assign_named_ref (குறியீடு_list *p, named_ref *name)
{
  குறியீடு *sym = p->content.sym;

  எனில் (name->id == sym->tag)
    {
      complain (&name->loc, Wother,
                _("duplicated symbol name for %s ignored"),
                quote (sym->tag));
      named_ref_free (name);
    }
  இல்லையெனில்
    p->named_ref = name;
}


/* The rule currently being defined, and the previous rule.
   CURRENT_RULE points to the first LHS of the current rule, while
   PREVIOUS_RULE_END points to the *end* of the previous rule (NULL).  */
நிலையான குறியீடு_list *current_rule = வெற்று;
நிலையான குறியீடு_list *previous_rule_end = வெற்று;


/*----------------------------------------------.
| Create a new rule for LHS in to the GRAMMAR.  |
`----------------------------------------------*/

வெறுமை
grammar_current_rule_begin (குறியீடு *lhs, location loc,
                            named_ref *lhs_name)
{
  /* Start a new rule and record its lhs.  */
  ++nrules;
  previous_rule_end = grammar_end;

  current_rule = grammar_குறியீடு_append (lhs, loc);
  எனில் (lhs_name)
    assign_named_ref (current_rule, named_ref_copy (lhs_name));

  /* Mark the rule's lhs as a nonterminal if not already so.  */
  எனில் (lhs->content->class == unknown_sym || lhs->content->class == pct_type_sym)
    குறியீடு_class_set (lhs, nterm_sym, empty_loc, false);
  இல்லையெனில் எனில் (lhs->content->class == token_sym)
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

நிலையான bool
குறியீடு_should_be_used (குறியீடு_list மாறிலி *s, bool *midrule_warning)
{
  எனில் (குறியீடு_code_props_get (s->content.sym, destructor)->code)
    திரும்பு true;
  எனில் ((s->midrule && s->midrule->action_props.is_மதிப்பு_used)
      || (s->midrule_parent_rule
          && (குறியீடு_list_n_get (s->midrule_parent_rule,
                                 s->midrule_parent_rhs_index)
              ->action_props.is_மதிப்பு_used)))
    {
      *midrule_warning = true;
      திரும்பு true;
    }
  திரும்பு false;
}

/*-----------------------------------------------------------------.
| Check that the rule R is properly defined.  For instance, there  |
| should be no type clash on the default action.  Possibly install |
| the default action.                                              |
`-----------------------------------------------------------------*/

நிலையான வெறுமை
grammar_rule_check_and_complete (குறியீடு_list *r)
{
  மாறிலி குறியீடு *lhs = r->content.sym;
  மாறிலி குறியீடு *first_rhs = r->next->content.sym;

  /* Type check.

     If there is an action, then there is nothing we can do: the user
     is allowed to shoot herself in the foot.

     Don't worry about the default action if $$ is untyped, since $$'s
     value can't be used.  */
  எனில் (!r->action_props.code && lhs->content->type_name)
    {
      /* If $$ is being set in default way, report if any type mismatch.  */
      எனில் (first_rhs)
        {
          எழுத்து மாறிலி *lhs_type = lhs->content->type_name;
          எழுத்து மாறிலி *rhs_type =
            first_rhs->content->type_name ? first_rhs->content->type_name : "";
          எனில் (!UNIQSTR_EQ (lhs_type, rhs_type))
            complain (&r->rhs_loc, Wother,
                      _("type clash on default action: <%s> != <%s>"),
                      lhs_type, rhs_type);
          இல்லையெனில்
            {
              /* Install the default action only for C++.  */
              மாறிலி bool is_cxx =
                STREQ (language->language, "c++")
                || (skeleton && (STREQ (skeleton, "glr.cc")
                                 || STREQ (skeleton, "glr2.cc")
                                 || STREQ (skeleton, "lalr1.cc")));
              எனில் (is_cxx)
                {
                  code_props_rule_action_init (&r->action_props, "{ $$ = $1; }",
                                               r->rhs_loc, r,
                                               /* name */ வெற்று,
                                               /* type */ வெற்று,
                                               /* is_predicate */ false);
                  code_props_translate_code (&r->action_props);
                }
            }
        }
      /* Warn if there is no default for $$ but we need one.  */
      இல்லையெனில்
        complain (&r->rhs_loc, Wother,
                  _("empty rule for typed nonterminal, and no action"));
    }

  /* For each start symbol, build the action of its start rule.  Use
     the same obstack as the one used by scan-code, which is in charge
     of actions. */
  மாறிலி bool multistart = start_symbols && start_symbols->next;
  எனில் (multistart && lhs == acceptsymbol)
    {
      மாறிலி குறியீடு *start = r->next->next->content.sym;
      எனில் (start->content->type_name)
        obstack_printf (obstack_for_actions,
                        "{ ]b4_accept""([%s%d])[; }",
                        start->content->class == nterm_sym ? "orig " : "",
                        start->content->எண்);
      இல்லையெனில்
        obstack_printf (obstack_for_actions,
                        "{ ]b4_accept[; }");
      code_props_rule_action_init (&r->action_props,
                                   obstack_finish0 (obstack_for_actions),
                                   r->rhs_loc, r,
                                   /* name */ வெற்று,
                                   /* type */ வெற்று,
                                   /* is_predicate */ false);
    }


  /* Check that symbol values that should be used are in fact used.
     Don't check the generated start rules.  It has no action, so some
     rhs symbols may appear unused, but the parsing algorithm ensures
     that %destructor's are invoked appropriately.  */
  எனில் (lhs != acceptsymbol)
    {
      முழு n = 0;
      க்கு (குறியீடு_list மாறிலி *l = r; l && l->content.sym; l = l->next, ++n)
        {
          bool midrule_warning = false;
          எனில் (!l->action_props.is_மதிப்பு_used
              && குறியீடு_should_be_used (l, &midrule_warning)
              /* The default action, $$ = $1, 'uses' both.  */
              && (r->action_props.code || (n != 0 && n != 1)))
            {
              warnings warn_flag = midrule_warning ? Wmidrule_மதிப்புகள் : Wother;
              எனில் (n)
                complain (&l->sym_loc, warn_flag, _("unused value: $%d"), n);
              இல்லையெனில்
                complain (&l->rhs_loc, warn_flag, _("unset value: $$"));
            }
        }
    }

  /* Check that %empty => empty rule.  */
  எனில் (r->percent_empty_loc.start.file
      && r->next && r->next->content.sym)
    {
      complain (&r->percent_empty_loc, complaint,
                _("%%empty on non-empty rule"));
      fixits_register (&r->percent_empty_loc, "");
    }

  /* Check that empty rule => %empty.  */
  எனில் (!(r->next && r->next->content.sym)
      && !r->midrule_parent_rule
      && !r->percent_empty_loc.start.file
      && warning_is_enabled (Wempty_rule))
    {
      complain (&r->rhs_loc, Wempty_rule, _("empty rule without %%empty"));
      எனில் (feature_flag & feature_caret)
        location_caret_suggestion (r->rhs_loc, "%empty", stderr);
      location loc = r->rhs_loc;
      loc.end = loc.start;
      fixits_register (&loc, " %empty ");
    }

  /* See comments in grammar_current_rule_prec_set for how POSIX
     mandates this complaint.  It's only for identifiers, so skip
     it for char literals and strings, which are always tokens.  */
  எனில் (r->ruleprec
      && r->ruleprec->tag[0] != '\'' && r->ruleprec->tag[0] != '"'
      && r->ruleprec->content->status != declared
      && !r->ruleprec->content->prec)
    complain (&r->rhs_loc, Wother,
              _("token for %%prec is not defined: %s"), r->ruleprec->tag);

  /* Check that the (main) action was not typed.  */
  எனில் (r->action_props.type)
    complain (&r->rhs_loc, Wother,
              _("only midrule actions can be typed: %s"), r->action_props.type);
}


/*-------------------------------------.
| End the currently being grown rule.  |
`-------------------------------------*/

வெறுமை
grammar_current_rule_end (location loc)
{
  /* Put an empty link in the list to mark the end of this rule  */
  grammar_குறியீடு_append (வெற்று, grammar_end->rhs_loc);
  current_rule->rhs_loc = loc;
}


/*-------------------------------------------------------------------.
| The previous action turns out to be a midrule action.  Attach it   |
| to the current rule, i.e., create a dummy symbol, attach it this   |
| midrule action, and append this dummy nonterminal to the current   |
| rule.                                                              |
`-------------------------------------------------------------------*/

வெறுமை
grammar_midrule_action (வெறுமை)
{
  /* Since the action was written out with this rule's number, we must
     give the new rule this number by inserting the new rule before
     it.  */

  /* Make a DUMMY nonterminal, whose location is that of the midrule
     action.  Create the MIDRULE.  */
  location dummy_loc = current_rule->action_props.location;
  குறியீடு *dummy = dummy_குறியீடு_get (dummy_loc);
  குறியீடு_type_set (dummy,
                   current_rule->action_props.type, current_rule->action_props.location);
  குறியீடு_list *midrule = குறியீடு_list_sym_new (dummy, dummy_loc);

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
                               /* name_ref */ வெற்று,
                               /* type */ வெற்று,
                               current_rule->action_props.is_predicate);
  code_props_none_init (&current_rule->action_props);

  midrule->expected_sr_conflicts = current_rule->expected_sr_conflicts;
  midrule->expected_rr_conflicts = current_rule->expected_rr_conflicts;
  current_rule->expected_sr_conflicts = -1;
  current_rule->expected_rr_conflicts = -1;

  எனில் (previous_rule_end)
    previous_rule_end->next = midrule;
  இல்லையெனில்
    grammar = midrule;

  /* End the dummy's rule.  */
  midrule->next = குறியீடு_list_sym_new (வெற்று, dummy_loc);
  midrule->next->next = current_rule;

  previous_rule_end = midrule->next;

  /* Insert the dummy nonterminal replacing the midrule action into
     the current rule.  Bind it to its dedicated rule.  */
  grammar_current_rule_குறியீடு_append (dummy, dummy_loc,
                                      action_name);
  grammar_end->midrule = midrule;
  midrule->midrule_parent_rule = current_rule;
  midrule->midrule_parent_rhs_index = குறியீடு_list_length (current_rule->next);
}

/* Set the precedence symbol of the current rule to PRECSYM. */

வெறுமை
grammar_current_rule_prec_set (குறியீடு *precsym, location loc)
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
  குறியீடு_class_set (precsym, token_sym, loc, false);
  எனில் (current_rule->ruleprec)
    duplicate_rule_directive ("%prec",
                              current_rule->ruleprec->location, loc);
  இல்லையெனில்
    current_rule->ruleprec = precsym;
}

/* Set %empty for the current rule. */

வெறுமை
grammar_current_rule_empty_set (location loc)
{
  /* If %empty is used and -Wno-empty-rule is not, then enable
     -Wempty-rule.  */
  எனில் (warning_is_unset (Wempty_rule))
    warning_argmatch ("empty-rule", 0, 0);
  எனில் (current_rule->percent_empty_loc.start.file)
    duplicate_rule_directive ("%empty",
                              current_rule->percent_empty_loc, loc);
  இல்லையெனில்
    current_rule->percent_empty_loc = loc;
}

/* Attach dynamic precedence DPREC to the current rule. */

வெறுமை
grammar_current_rule_dprec_set (முழு dprec, location loc)
{
  எனில் (! glr_parser)
    complain (&loc, Wother, _("%s affects only GLR parsers"),
              "%dprec");
  எனில் (dprec <= 0)
    complain (&loc, complaint, _("%s must be followed by positive number"),
              "%dprec");
  இல்லையெனில் எனில் (current_rule->dprec != 0)
    duplicate_rule_directive ("%dprec",
                              current_rule->dprec_loc, loc);
  இல்லையெனில்
    {
      current_rule->dprec = dprec;
      current_rule->dprec_loc = loc;
    }
}

/* Attach a merge function NAME with argument type TYPE to current
   rule. */

வெறுமை
grammar_current_rule_merge_set (uniqstr name, location loc)
{
  எனில் (! glr_parser)
    complain (&loc, Wother, _("%s affects only GLR parsers"),
              "%merge");
  எனில் (current_rule->merger != 0)
    duplicate_rule_directive ("%merge",
                              current_rule->merger_declaration_loc, loc);
  இல்லையெனில்
    {
      current_rule->merger = get_merge_function (name);
      current_rule->merger_declaration_loc = loc;
    }
}

/* Attach SYM to the current rule.  If needed, move the previous
   action as a midrule action.  */

வெறுமை
grammar_current_rule_குறியீடு_append (குறியீடு *sym, location loc,
                                    named_ref *name)
{
  எனில் (current_rule->action_props.code)
    grammar_midrule_action ();
  குறியீடு_list *p = grammar_குறியீடு_append (sym, loc);
  எனில் (name)
    assign_named_ref (p, name);
  எனில் (sym->content->status == undeclared || sym->content->status == used)
    sym->content->status = needed;
}

வெறுமை
grammar_current_rule_action_append (மாறிலி எழுத்து *action, location loc,
                                    named_ref *name, uniqstr type)
{
  எனில் (current_rule->action_props.code)
    grammar_midrule_action ();
  எனில் (type)
    complain (&loc, Wyacc,
              _("POSIX Yacc does not support typed midrule actions"));
  /* After all symbol declarations have been parsed, packgram invokes
     code_props_translate_code.  */
  code_props_rule_action_init (&current_rule->action_props, action, loc,
                               current_rule,
                               name, type,
                               /* is_predicate */ false);
}

வெறுமை
grammar_current_rule_predicate_append (மாறிலி எழுத்து *pred, location loc)
{
  எனில் (current_rule->action_props.code)
    grammar_midrule_action ();
  code_props_rule_action_init (&current_rule->action_props, pred, loc,
                               current_rule,
                               வெற்று, வெற்று,
                               /* is_predicate */ true);
}

/* Set the expected number of shift/reduce (reduce/reduce) conflicts
 * for the current rule.  If a midrule is encountered later, the count
 * is transferred to it and reset in the current rule to -1. */

வெறுமை
grammar_current_rule_expect_sr (முழு மொத்தம், location loc)
{
  (வெறுமை) loc;
  current_rule->expected_sr_conflicts = மொத்தம்;
}

வெறுமை
grammar_current_rule_expect_rr (முழு மொத்தம், location loc)
{
  எனில் (! glr_parser)
    complain (&loc, Wother, _("%s affects only GLR parsers"),
              "%expect-rr");
  இல்லையெனில்
    current_rule->expected_rr_conflicts = மொத்தம்;
}


/*---------------------------------------------.
| Build RULES and RITEM from what was parsed.  |
`---------------------------------------------*/

நிலையான வெறுமை
packgram (வெறுமை)
{
  முழு itemno = 0;
  ritem = xnmalloc (nritems + 1, அளவு *ritem);
  /* This sentinel is used by build_relations() in lalr.c.  */
  *ritem++ = 0;

  rule_எண் ruleno = 0;
  rules = xnmalloc (nrules, அளவு *rules);

  க்கு (குறியீடு_list *p = grammar; p; p = p->next)
    {
      குறியீடு_list *lhs = p;
      record_merge_function_type (lhs->merger, lhs->content.sym,
                                  lhs->merger_declaration_loc);
      /* If the midrule's $$ is set or its $n is used, remove the '$' from the
         symbol name so that it's a user-defined symbol so that the default
         %destructor and %printer apply.  */
      எனில் (lhs->midrule_parent_rule /* i.e., symbol_is_dummy (lhs->content.sym).  */
          && (lhs->action_props.is_மதிப்பு_used
              || (குறியீடு_list_n_get (lhs->midrule_parent_rule,
                                     lhs->midrule_parent_rhs_index)
                  ->action_props.is_மதிப்பு_used)))
        lhs->content.sym->tag += 1;

      grammar_rule_check_and_complete (lhs);

      rules[ruleno].code = ruleno;
      rules[ruleno].எண் = ruleno;
      rules[ruleno].lhs = lhs->content.sym->content;
      rules[ruleno].rhs = ritem + itemno;
      rules[ruleno].prec = வெற்று;
      rules[ruleno].dprec = lhs->dprec;
      rules[ruleno].merger = lhs->merger;
      rules[ruleno].precsym = வெற்று;
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
        க்கு (p = lhs->next; p->content.sym; p = p->next)
          {
            ++rule_length;

            /* Don't allow rule_length == INT_MAX, since that might
               cause confusion with strtol if INT_MAX == LONG_MAX.  */
            எனில் (rule_length == INT_MAX)
              complain (&rules[ruleno].location, fatal, _("rule is too long"));

            /* item_number = symbol_number.
               But the former needs to contain more: negative rule numbers. */
            ritem[itemno++] =
              குறியீடு_எண்_as_item_எண் (p->content.sym->content->எண்);
            /* A rule gets by default the precedence and associativity
               of its last token.  */
            எனில் (p->content.sym->content->class == token_sym && default_prec)
              rules[ruleno].prec = p->content.sym->content;
          }
      }

      /* If this rule has a %prec,
         the specified symbol's precedence replaces the default.  */
      எனில் (lhs->ruleprec)
        {
          rules[ruleno].precsym = lhs->ruleprec->content;
          rules[ruleno].prec = lhs->ruleprec->content;
        }

      /* An item ends by the rule number (negated).  */
      ritem[itemno++] = rule_எண்_as_item_எண் (ruleno);
      aver (itemno < ITEM_எண்_MAX);
      ++ruleno;
      aver (ruleno < RULE_எண்_MAX);
    }

  aver (itemno == nritems);

  எனில் (trace_flag & trace_sets)
    ritem_print (stderr);
}


/*--------------------------------------------------------------.
| Read in the grammar specification and record it in the format |
| described in gram.h.                                          |
`--------------------------------------------------------------*/

வெறுமை
reader (மாறிலி எழுத்து *gram)
{
  /* Set up symbol_table, semantic_type_table, and the built-in
     symbols.  */
  symbols_new ();

  gram_scanner_open (gram);
  parser_init ();
  gram_parse ();
  gram_scanner_close ();

  prepare_percent_define_front_end_variables ();

  எனில் (complaint_status  < status_complaint)
    check_and_convert_grammar ();
}

நிலையான வெறுமை
prepare_percent_define_front_end_variables (வெறுமை)
{
  /* Set %define front-end variable defaults.  */
  muscle_percent_define_default ("lr.keep-unreachable-state", "false");
  {
    /* IELR would be a better default, but LALR is historically the
       default.  */
    muscle_percent_define_default ("lr.type", "lalr");
    எழுத்து *lr_type = muscle_percent_define_get ("lr.type");
    எனில் (STRNEQ (lr_type, "canonical-lr"))
      muscle_percent_define_default ("lr.default-reduction", "most");
    இல்லையெனில்
      muscle_percent_define_default ("lr.default-reduction", "accepting");
    விடுவி (lr_type);
  }
  muscle_percent_define_default ("tool.xsltproc", "xsltproc");

  /* Check %define front-end variables.  */
  {
    நிலையான எழுத்து மாறிலி * மாறிலி மதிப்புகள்[] =
      {
       "lr.type", "lr""(0)", "lalr", "ielr", "canonical-lr", வெற்று,
       "lr.default-reduction", "most", "consistent", "accepting", வெற்று,
       வெற்று
      };
    muscle_percent_define_check_மதிப்புகள் (மதிப்புகள்);
  }
}

/* Find the first LHS which is not a dummy.  */

நிலையான குறியீடு *
find_start_குறியீடு (வெறுமை)
{
  குறியீடு_list *res = grammar;
  /* Skip all the possible dummy rules of the first rule.  */
  க்கு (; குறியீடு_is_dummy (res->content.sym); res = res->next)
    /* Skip the LHS, and then all the RHS of the dummy rule.  */
    க்கு (res = res->next; res->content.sym; res = res->next)
      தொடர்;
  திரும்பு res->content.sym;
}


/* Insert an initial rule, whose location is that of the first rule
   (not that of the start symbol):

   $accept: SWITCHING_TOKEN START $end.  */
நிலையான வெறுமை
create_start_rule (குறியீடு *swtok, குறியீடு *start)
{
  குறியீடு_list *initial_rule = குறியீடு_list_sym_new (acceptsymbol, empty_loc);
  initial_rule->rhs_loc = grammar->rhs_loc;
  குறியீடு_list *p = initial_rule;
  எனில் (swtok)
    {
      // Cannot create the action now, as the symbols have not yet
      // been assigned their number (by symbol_pack), which we need to
      // know the type name.  So the action is created in
      // grammar_rule_check_and_complete, which is run after
      // symbol_pack.
      p->next = குறியீடு_list_sym_new (swtok, empty_loc);
      p = p->next;
    }
  p->next = குறியீடு_list_sym_new (start, empty_loc);
  p = p->next;
  p->next = குறியீடு_list_sym_new (eoftoken, empty_loc);
  p = p->next;
  p->next = குறியீடு_list_sym_new (வெற்று, empty_loc);
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
குறியீடு *
switching_token (மாறிலி குறியீடு *start)
{
  எழுத்து buf[100];
  size_t len = அளவு buf;
  எழுத்து *name = asnprintf (buf, &len, "YY_PARSE_%s", குறியீடு_id_get (start));
  எனில் (!name)
    xalloc_die ();
  // Setting the location ensures deterministic symbol numbers.
  குறியீடு *res = குறியீடு_get (name, start->location);
  எனில் (name != buf)
    விடுவி (name);
  குறியீடு_class_set (res, token_sym, start->location, false);
  திரும்பு res;
}

/* Create the start rules in reverse order, since they are inserted at
   the top of the grammar.  That way the rules follow the order of
   declaration to %start.  */

நிலையான வெறுமை
create_multiple_start_rules (குறியீடு_list *start_syms)
{
  எனில் (start_syms)
    {
      create_multiple_start_rules (start_syms->next);
      ப்ரதிஜ்ஞா (start_syms->content_type == SYMLIST_குறியீடு);
      குறியீடு *start = start_syms->content.sym;
      குறியீடு *swtok = switching_token (start);
      create_start_rule (swtok, start);
    }
}

/* For each start symbol "foo", create the rule "$accept: YY_FOO
   foo $end". */
நிலையான வெறுமை
create_start_rules (வெறுமை)
{
  எனில் (!start_symbols)
    {
      குறியீடு *start = find_start_குறியீடு ();
      start_symbols = குறியீடு_list_sym_new (start, start->location);
    }

  மாறிலி bool several = start_symbols->next;
  எனில் (several)
    create_multiple_start_rules (start_symbols);
  இல்லையெனில்
    {
      குறியீடு *start = start_symbols->content.sym;
      create_start_rule (வெற்று, start);
    }
}

நிலையான வெறுமை
check_start_symbols (வெறுமை)
{
  மாறிலி bool multistart = start_symbols && start_symbols->next;
  // Sanity checks on the start symbols.
  க்கு (குறியீடு_list *list = start_symbols; list; list = list->next)
    {
      மாறிலி குறியீடு *start = list->content.sym;
      எனில் (start->content->class == unknown_sym)
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
      எனில் (!multistart && start->content->class == token_sym)
        complain (&start->location, complaint,
                  _("the start symbol %s is a token"),
                  start->tag);
    }
  எனில் (complaint_status == status_complaint)
    exit (EXIT_FAILURE);
}


/*-------------------------------------------------------------.
| Check the grammar that has just been read, and convert it to |
| internal form.                                               |
`-------------------------------------------------------------*/

நிலையான வெறுமை
check_and_convert_grammar (வெறுமை)
{
  /* Grammar has been read.  Do some checking.  */
  எனில் (nrules == 0)
    complain (வெற்று, fatal, _("no rules in the input grammar"));

  /* If the user did not define her EOFTOKEN, do it now. */
  எனில் (!eoftoken)
    {
      eoftoken = குறியீடு_get ("YYEOF", empty_loc);
      எனில் (eoftoken->content->class == nterm_sym)
        complain (&eoftoken->location, complaint,
                  _("rule given for %s, which is a token"),
                  eoftoken->tag);
      eoftoken->content->class = token_sym;
      eoftoken->content->எண் = 0;
      /* Value specified by POSIX.  */
      eoftoken->content->code = 0;
      {
        குறியீடு *alias = குறியீடு_get ("$end", empty_loc);
        குறியீடு_class_set (alias, token_sym, empty_loc, false);
        குறியீடு_make_alias (eoftoken, alias, empty_loc);
      }
    }

  /* Insert the initial rule(s).  */
  create_start_rules ();

  /* Report any undefined symbols and consider them nonterminals.  */
  symbols_check_defined ();

  எனில் (குறியீடு_எண்_MAXIMUM - nnterms < ntokens)
    complain (வெற்று, fatal, "too many symbols in input grammar (limit is %d)",
              குறியீடு_எண்_MAXIMUM);

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
  க்கு (குறியீடு_list *sym = grammar; sym; sym = sym->next)
    code_props_translate_code (&sym->action_props);

  /* Convert the grammar into the format described in gram.h.  */
  packgram ();

  /* The grammar as a symbol_list is no longer needed. */
  குறியீடு_list_free (grammar);
}
