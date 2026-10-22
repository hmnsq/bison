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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ "system.h"

#અન્તર્ભાવ <quote.h>
#અન્તર્ભાવ <vasnprintf.h>

#અન્તર્ભાવ "complain.h"
#અન્તર્ભાવ "conflicts.h"
#અન્તર્ભાવ "files.h"
#અન્તર્ભાવ "fixits.h"
#અન્તર્ભાવ "getargs.h"
#અન્તર્ભાવ "gram.h"
#અન્તર્ભાવ "muscle-tab.h"
#અન્તર્ભાવ "reader.h"
#અન્તર્ભાવ "symlist.h"
#અન્તર્ભાવ "symtab.h"
#અન્તર્ભાવ "scan-gram.h"
#અન્તર્ભાવ "scan-code.h"

સ્થિર શૂન્ય prepare_percent_define_front_end_variables (શૂન્ય);
સ્થિર શૂન્ય check_and_convert_grammar (શૂન્ય);

સ્થિર પ્રતીક_list *grammar = ખાલી;
પ્રતીક_list *start_symbols = ખાલી;
merger_list *merge_functions = ખાલી;

/* Was %union seen?  */
bool union_seen = false;

/* Should rules have a default precedence?  */
bool default_prec = true;


શૂન્ય
grammar_start_symbols_add (પ્રતીક_list *syms)
{
  /* Report and ignore duplicates.  Append the others to START_SYMBOLS.  */
  પ્રતીક_list *last = પ્રતીક_list_last (start_symbols);
  માટે (પ્રતીક_list *l = syms; l && l->content.sym; /* nothing */)
    {
      /* Is there a previous definition?  */
      પ્રતીક_list *first = પ્રતીક_list_find_પ્રતીક (start_symbols, l->content.sym);
      જો (first)
        {
          duplicate_directive ("%start", first->sym_loc, l->sym_loc);
          પ્રતીક_list *dupl = l;
          l = l->next;
          dupl->next = ખાલી;
          પ્રતીક_list_free (dupl);
        }
      નહીંતર
        {
          જો (last)
            {
              last->next = l;
              last = l;
            }
          નહીંતર
            {
              last = l;
              start_symbols = last;
            }
          પ્રતીક_list *next = l->next;
          l->next = ખાલી;
          l = next;
        }
    }
}



/*------------------------------------------------------------------------.
| Return the merger index for a merging function named NAME.  Records the |
| function, if new, in MERGER_LIST.                                       |
`------------------------------------------------------------------------*/

સ્થિર પૂર્ણાંક
get_merge_function (uniqstr name)
{
  જો (! glr_parser)
    પાછા 0;

  merger_list *syms;
  merger_list head;
  પૂર્ણાંક n;

  head.next = merge_functions;
  માટે (syms = &head, n = 1; syms->next; syms = syms->next, n += 1)
    જો (UNIQSTR_EQ (name, syms->next->name))
      તોડો;
  જો (syms->next == ખાલી)
    {
      syms->next = xmalloc (કદ syms->next[0]);
      syms->next->name = uniqstr_new (name);
      /* After all symbol type declarations have been parsed, packgram invokes
         record_merge_function_type to set the type.  */
      syms->next->sym = ખાલી;
      syms->next->next = ખાલી;
      merge_functions = head.next;
    }
  પાછા n;
}

/*-------------------------------------------------------------------.
| For the existing merging function with index MERGER, record that   |
| the result type is that of SYM, as required by the lhs (i.e., SYM) |
| of the rule whose %merge declaration is at DECLARATION_LOC.        |
`-------------------------------------------------------------------*/

સ્થિર શૂન્ય
record_merge_function_type (પૂર્ણાંક merger, પ્રતીક *sym, location declaration_loc)
{
  જો (merger <= 0)
    પાછા;

  uniqstr type
    = sym->content->type_name ? sym->content->type_name : uniqstr_new ("");

  merger_list *merge_function;
  પૂર્ણાંક merger_find = 1;
  માટે (merge_function = merge_functions;
       merge_function != ખાલી && merger_find != merger;
       merge_function = merge_function->next)
    merger_find += 1;
  aver (merge_function != ખાલી && merger_find == merger);
  જો (merge_function->sym && merge_function->sym->content->type_name)
    {
      જો (!UNIQSTR_EQ (merge_function->sym->content->type_name, type))
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
  નહીંતર
    {
      merge_function->sym = sym;
      merge_function->type_declaration_loc = declaration_loc;
    }
}

/*--------------------------------------.
| Free all merge-function definitions.  |
`--------------------------------------*/

શૂન્ય
free_merger_functions (શૂન્ય)
{
  merger_list *L0 = merge_functions;
  જ્યારે (L0)
    {
      merger_list *L1 = L0->next;
      મુક્ત (L0);
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
સ્થિર પ્રતીક_list *grammar_end = ખાલી;

/* Append SYM to the grammar.  */
સ્થિર પ્રતીક_list *
grammar_પ્રતીક_append (પ્રતીક *sym, location loc)
{
  પ્રતીક_list *p = પ્રતીક_list_sym_new (sym, loc);

  જો (grammar_end)
    grammar_end->next = p;
  નહીંતર
    grammar = p;

  grammar_end = p;

  /* A null SYM stands for an end of rule; it is not an actual
     part of it.  */
  જો (sym)
    ++nritems;

  પાછા p;
}

સ્થિર શૂન્ય
assign_named_ref (પ્રતીક_list *p, named_ref *name)
{
  પ્રતીક *sym = p->content.sym;

  જો (name->id == sym->tag)
    {
      complain (&name->loc, Wother,
                _("duplicated symbol name for %s ignored"),
                quote (sym->tag));
      named_ref_free (name);
    }
  નહીંતર
    p->named_ref = name;
}


/* The rule currently being defined, and the previous rule.
   CURRENT_RULE points to the first LHS of the current rule, while
   PREVIOUS_RULE_END points to the *end* of the previous rule (NULL).  */
સ્થિર પ્રતીક_list *current_rule = ખાલી;
સ્થિર પ્રતીક_list *previous_rule_end = ખાલી;


/*----------------------------------------------.
| Create a new rule for LHS in to the GRAMMAR.  |
`----------------------------------------------*/

શૂન્ય
grammar_current_rule_begin (પ્રતીક *lhs, location loc,
                            named_ref *lhs_name)
{
  /* Start a new rule and record its lhs.  */
  ++nrules;
  previous_rule_end = grammar_end;

  current_rule = grammar_પ્રતીક_append (lhs, loc);
  જો (lhs_name)
    assign_named_ref (current_rule, named_ref_copy (lhs_name));

  /* Mark the rule's lhs as a nonterminal if not already so.  */
  જો (lhs->content->class == unknown_sym || lhs->content->class == pct_type_sym)
    પ્રતીક_class_set (lhs, nterm_sym, empty_loc, false);
  નહીંતર જો (lhs->content->class == token_sym)
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

સ્થિર bool
પ્રતીક_should_be_used (પ્રતીક_list અચલ *s, bool *midrule_warning)
{
  જો (પ્રતીક_code_props_get (s->content.sym, destructor)->code)
    પાછા true;
  જો ((s->midrule && s->midrule->action_props.is_કિંમત_used)
      || (s->midrule_parent_rule
          && (પ્રતીક_list_n_get (s->midrule_parent_rule,
                                 s->midrule_parent_rhs_index)
              ->action_props.is_કિંમત_used)))
    {
      *midrule_warning = true;
      પાછા true;
    }
  પાછા false;
}

/*-----------------------------------------------------------------.
| Check that the rule R is properly defined.  For instance, there  |
| should be no type clash on the default action.  Possibly install |
| the default action.                                              |
`-----------------------------------------------------------------*/

સ્થિર શૂન્ય
grammar_rule_check_and_complete (પ્રતીક_list *r)
{
  અચલ પ્રતીક *lhs = r->content.sym;
  અચલ પ્રતીક *first_rhs = r->next->content.sym;

  /* Type check.

     If there is an action, then there is nothing we can do: the user
     is allowed to shoot herself in the foot.

     Don't worry about the default action if $$ is untyped, since $$'s
     value can't be used.  */
  જો (!r->action_props.code && lhs->content->type_name)
    {
      /* If $$ is being set in default way, report if any type mismatch.  */
      જો (first_rhs)
        {
          અક્ષર અચલ *lhs_type = lhs->content->type_name;
          અક્ષર અચલ *rhs_type =
            first_rhs->content->type_name ? first_rhs->content->type_name : "";
          જો (!UNIQSTR_EQ (lhs_type, rhs_type))
            complain (&r->rhs_loc, Wother,
                      _("type clash on default action: <%s> != <%s>"),
                      lhs_type, rhs_type);
          નહીંતર
            {
              /* Install the default action only for C++.  */
              અચલ bool is_cxx =
                STREQ (language->language, "c++")
                || (skeleton && (STREQ (skeleton, "glr.cc")
                                 || STREQ (skeleton, "glr2.cc")
                                 || STREQ (skeleton, "lalr1.cc")));
              જો (is_cxx)
                {
                  code_props_rule_action_init (&r->action_props, "{ $$ = $1; }",
                                               r->rhs_loc, r,
                                               /* name */ ખાલી,
                                               /* type */ ખાલી,
                                               /* is_predicate */ false);
                  code_props_translate_code (&r->action_props);
                }
            }
        }
      /* Warn if there is no default for $$ but we need one.  */
      નહીંતર
        complain (&r->rhs_loc, Wother,
                  _("empty rule for typed nonterminal, and no action"));
    }

  /* For each start symbol, build the action of its start rule.  Use
     the same obstack as the one used by scan-code, which is in charge
     of actions. */
  અચલ bool multistart = start_symbols && start_symbols->next;
  જો (multistart && lhs == acceptsymbol)
    {
      અચલ પ્રતીક *start = r->next->next->content.sym;
      જો (start->content->type_name)
        obstack_printf (obstack_for_actions,
                        "{ ]b4_accept""([%s%d])[; }",
                        start->content->class == nterm_sym ? "orig " : "",
                        start->content->સંખ્યા);
      નહીંતર
        obstack_printf (obstack_for_actions,
                        "{ ]b4_accept[; }");
      code_props_rule_action_init (&r->action_props,
                                   obstack_finish0 (obstack_for_actions),
                                   r->rhs_loc, r,
                                   /* name */ ખાલી,
                                   /* type */ ખાલી,
                                   /* is_predicate */ false);
    }


  /* Check that symbol values that should be used are in fact used.
     Don't check the generated start rules.  It has no action, so some
     rhs symbols may appear unused, but the parsing algorithm ensures
     that %destructor's are invoked appropriately.  */
  જો (lhs != acceptsymbol)
    {
      પૂર્ણાંક n = 0;
      માટે (પ્રતીક_list અચલ *l = r; l && l->content.sym; l = l->next, ++n)
        {
          bool midrule_warning = false;
          જો (!l->action_props.is_કિંમત_used
              && પ્રતીક_should_be_used (l, &midrule_warning)
              /* The default action, $$ = $1, 'uses' both.  */
              && (r->action_props.code || (n != 0 && n != 1)))
            {
              warnings warn_flag = midrule_warning ? Wmidrule_કિંમતો : Wother;
              જો (n)
                complain (&l->sym_loc, warn_flag, _("unused value: $%d"), n);
              નહીંતર
                complain (&l->rhs_loc, warn_flag, _("unset value: $$"));
            }
        }
    }

  /* Check that %empty => empty rule.  */
  જો (r->percent_empty_loc.start.file
      && r->next && r->next->content.sym)
    {
      complain (&r->percent_empty_loc, complaint,
                _("%%empty on non-empty rule"));
      fixits_register (&r->percent_empty_loc, "");
    }

  /* Check that empty rule => %empty.  */
  જો (!(r->next && r->next->content.sym)
      && !r->midrule_parent_rule
      && !r->percent_empty_loc.start.file
      && warning_is_enabled (Wempty_rule))
    {
      complain (&r->rhs_loc, Wempty_rule, _("empty rule without %%empty"));
      જો (feature_flag & feature_caret)
        location_caret_suggestion (r->rhs_loc, "%empty", stderr);
      location loc = r->rhs_loc;
      loc.end = loc.start;
      fixits_register (&loc, " %empty ");
    }

  /* See comments in grammar_current_rule_prec_set for how POSIX
     mandates this complaint.  It's only for identifiers, so skip
     it for char literals and strings, which are always tokens.  */
  જો (r->ruleprec
      && r->ruleprec->tag[0] != '\'' && r->ruleprec->tag[0] != '"'
      && r->ruleprec->content->status != declared
      && !r->ruleprec->content->prec)
    complain (&r->rhs_loc, Wother,
              _("token for %%prec is not defined: %s"), r->ruleprec->tag);

  /* Check that the (main) action was not typed.  */
  જો (r->action_props.type)
    complain (&r->rhs_loc, Wother,
              _("only midrule actions can be typed: %s"), r->action_props.type);
}


/*-------------------------------------.
| End the currently being grown rule.  |
`-------------------------------------*/

શૂન્ય
grammar_current_rule_end (location loc)
{
  /* Put an empty link in the list to mark the end of this rule  */
  grammar_પ્રતીક_append (ખાલી, grammar_end->rhs_loc);
  current_rule->rhs_loc = loc;
}


/*-------------------------------------------------------------------.
| The previous action turns out to be a midrule action.  Attach it   |
| to the current rule, i.e., create a dummy symbol, attach it this   |
| midrule action, and append this dummy nonterminal to the current   |
| rule.                                                              |
`-------------------------------------------------------------------*/

શૂન્ય
grammar_midrule_action (શૂન્ય)
{
  /* Since the action was written out with this rule's number, we must
     give the new rule this number by inserting the new rule before
     it.  */

  /* Make a DUMMY nonterminal, whose location is that of the midrule
     action.  Create the MIDRULE.  */
  location dummy_loc = current_rule->action_props.location;
  પ્રતીક *dummy = dummy_પ્રતીક_get (dummy_loc);
  પ્રતીક_type_set (dummy,
                   current_rule->action_props.type, current_rule->action_props.location);
  પ્રતીક_list *midrule = પ્રતીક_list_sym_new (dummy, dummy_loc);

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
                               /* name_ref */ ખાલી,
                               /* type */ ખાલી,
                               current_rule->action_props.is_predicate);
  code_props_none_init (&current_rule->action_props);

  midrule->expected_sr_conflicts = current_rule->expected_sr_conflicts;
  midrule->expected_rr_conflicts = current_rule->expected_rr_conflicts;
  current_rule->expected_sr_conflicts = -1;
  current_rule->expected_rr_conflicts = -1;

  જો (previous_rule_end)
    previous_rule_end->next = midrule;
  નહીંતર
    grammar = midrule;

  /* End the dummy's rule.  */
  midrule->next = પ્રતીક_list_sym_new (ખાલી, dummy_loc);
  midrule->next->next = current_rule;

  previous_rule_end = midrule->next;

  /* Insert the dummy nonterminal replacing the midrule action into
     the current rule.  Bind it to its dedicated rule.  */
  grammar_current_rule_પ્રતીક_append (dummy, dummy_loc,
                                      action_name);
  grammar_end->midrule = midrule;
  midrule->midrule_parent_rule = current_rule;
  midrule->midrule_parent_rhs_index = પ્રતીક_list_length (current_rule->next);
}

/* Set the precedence symbol of the current rule to PRECSYM. */

શૂન્ય
grammar_current_rule_prec_set (પ્રતીક *precsym, location loc)
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
  પ્રતીક_class_set (precsym, token_sym, loc, false);
  જો (current_rule->ruleprec)
    duplicate_rule_directive ("%prec",
                              current_rule->ruleprec->location, loc);
  નહીંતર
    current_rule->ruleprec = precsym;
}

/* Set %empty for the current rule. */

શૂન્ય
grammar_current_rule_empty_set (location loc)
{
  /* If %empty is used and -Wno-empty-rule is not, then enable
     -Wempty-rule.  */
  જો (warning_is_unset (Wempty_rule))
    warning_argmatch ("empty-rule", 0, 0);
  જો (current_rule->percent_empty_loc.start.file)
    duplicate_rule_directive ("%empty",
                              current_rule->percent_empty_loc, loc);
  નહીંતર
    current_rule->percent_empty_loc = loc;
}

/* Attach dynamic precedence DPREC to the current rule. */

શૂન્ય
grammar_current_rule_dprec_set (પૂર્ણાંક dprec, location loc)
{
  જો (! glr_parser)
    complain (&loc, Wother, _("%s affects only GLR parsers"),
              "%dprec");
  જો (dprec <= 0)
    complain (&loc, complaint, _("%s must be followed by positive number"),
              "%dprec");
  નહીંતર જો (current_rule->dprec != 0)
    duplicate_rule_directive ("%dprec",
                              current_rule->dprec_loc, loc);
  નહીંતર
    {
      current_rule->dprec = dprec;
      current_rule->dprec_loc = loc;
    }
}

/* Attach a merge function NAME with argument type TYPE to current
   rule. */

શૂન્ય
grammar_current_rule_merge_set (uniqstr name, location loc)
{
  જો (! glr_parser)
    complain (&loc, Wother, _("%s affects only GLR parsers"),
              "%merge");
  જો (current_rule->merger != 0)
    duplicate_rule_directive ("%merge",
                              current_rule->merger_declaration_loc, loc);
  નહીંતર
    {
      current_rule->merger = get_merge_function (name);
      current_rule->merger_declaration_loc = loc;
    }
}

/* Attach SYM to the current rule.  If needed, move the previous
   action as a midrule action.  */

શૂન્ય
grammar_current_rule_પ્રતીક_append (પ્રતીક *sym, location loc,
                                    named_ref *name)
{
  જો (current_rule->action_props.code)
    grammar_midrule_action ();
  પ્રતીક_list *p = grammar_પ્રતીક_append (sym, loc);
  જો (name)
    assign_named_ref (p, name);
  જો (sym->content->status == undeclared || sym->content->status == used)
    sym->content->status = needed;
}

શૂન્ય
grammar_current_rule_action_append (અચલ અક્ષર *action, location loc,
                                    named_ref *name, uniqstr type)
{
  જો (current_rule->action_props.code)
    grammar_midrule_action ();
  જો (type)
    complain (&loc, Wyacc,
              _("POSIX Yacc does not support typed midrule actions"));
  /* After all symbol declarations have been parsed, packgram invokes
     code_props_translate_code.  */
  code_props_rule_action_init (&current_rule->action_props, action, loc,
                               current_rule,
                               name, type,
                               /* is_predicate */ false);
}

શૂન્ય
grammar_current_rule_predicate_append (અચલ અક્ષર *pred, location loc)
{
  જો (current_rule->action_props.code)
    grammar_midrule_action ();
  code_props_rule_action_init (&current_rule->action_props, pred, loc,
                               current_rule,
                               ખાલી, ખાલી,
                               /* is_predicate */ true);
}

/* Set the expected number of shift/reduce (reduce/reduce) conflicts
 * for the current rule.  If a midrule is encountered later, the count
 * is transferred to it and reset in the current rule to -1. */

શૂન્ય
grammar_current_rule_expect_sr (પૂર્ણાંક ગણતરી, location loc)
{
  (શૂન્ય) loc;
  current_rule->expected_sr_conflicts = ગણતરી;
}

શૂન્ય
grammar_current_rule_expect_rr (પૂર્ણાંક ગણતરી, location loc)
{
  જો (! glr_parser)
    complain (&loc, Wother, _("%s affects only GLR parsers"),
              "%expect-rr");
  નહીંતર
    current_rule->expected_rr_conflicts = ગણતરી;
}


/*---------------------------------------------.
| Build RULES and RITEM from what was parsed.  |
`---------------------------------------------*/

સ્થિર શૂન્ય
packgram (શૂન્ય)
{
  પૂર્ણાંક itemno = 0;
  ritem = xnmalloc (nritems + 1, કદ *ritem);
  /* This sentinel is used by build_relations() in lalr.c.  */
  *ritem++ = 0;

  rule_સંખ્યા ruleno = 0;
  rules = xnmalloc (nrules, કદ *rules);

  માટે (પ્રતીક_list *p = grammar; p; p = p->next)
    {
      પ્રતીક_list *lhs = p;
      record_merge_function_type (lhs->merger, lhs->content.sym,
                                  lhs->merger_declaration_loc);
      /* If the midrule's $$ is set or its $n is used, remove the '$' from the
         symbol name so that it's a user-defined symbol so that the default
         %destructor and %printer apply.  */
      જો (lhs->midrule_parent_rule /* i.e., symbol_is_dummy (lhs->content.sym).  */
          && (lhs->action_props.is_કિંમત_used
              || (પ્રતીક_list_n_get (lhs->midrule_parent_rule,
                                     lhs->midrule_parent_rhs_index)
                  ->action_props.is_કિંમત_used)))
        lhs->content.sym->tag += 1;

      grammar_rule_check_and_complete (lhs);

      rules[ruleno].code = ruleno;
      rules[ruleno].સંખ્યા = ruleno;
      rules[ruleno].lhs = lhs->content.sym->content;
      rules[ruleno].rhs = ritem + itemno;
      rules[ruleno].prec = ખાલી;
      rules[ruleno].dprec = lhs->dprec;
      rules[ruleno].merger = lhs->merger;
      rules[ruleno].precsym = ખાલી;
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
        માટે (p = lhs->next; p->content.sym; p = p->next)
          {
            ++rule_length;

            /* Don't allow rule_length == INT_MAX, since that might
               cause confusion with strtol if INT_MAX == LONG_MAX.  */
            જો (rule_length == INT_MAX)
              complain (&rules[ruleno].location, fatal, _("rule is too long"));

            /* item_number = symbol_number.
               But the former needs to contain more: negative rule numbers. */
            ritem[itemno++] =
              પ્રતીક_સંખ્યા_as_item_સંખ્યા (p->content.sym->content->સંખ્યા);
            /* A rule gets by default the precedence and associativity
               of its last token.  */
            જો (p->content.sym->content->class == token_sym && default_prec)
              rules[ruleno].prec = p->content.sym->content;
          }
      }

      /* If this rule has a %prec,
         the specified symbol's precedence replaces the default.  */
      જો (lhs->ruleprec)
        {
          rules[ruleno].precsym = lhs->ruleprec->content;
          rules[ruleno].prec = lhs->ruleprec->content;
        }

      /* An item ends by the rule number (negated).  */
      ritem[itemno++] = rule_સંખ્યા_as_item_સંખ્યા (ruleno);
      aver (itemno < ITEM_સંખ્યા_MAX);
      ++ruleno;
      aver (ruleno < RULE_સંખ્યા_MAX);
    }

  aver (itemno == nritems);

  જો (trace_flag & trace_sets)
    ritem_print (stderr);
}


/*--------------------------------------------------------------.
| Read in the grammar specification and record it in the format |
| described in gram.h.                                          |
`--------------------------------------------------------------*/

શૂન્ય
reader (અચલ અક્ષર *gram)
{
  /* Set up symbol_table, semantic_type_table, and the built-in
     symbols.  */
  symbols_new ();

  gram_scanner_open (gram);
  parser_init ();
  gram_parse ();
  gram_scanner_close ();

  prepare_percent_define_front_end_variables ();

  જો (complaint_status  < status_complaint)
    check_and_convert_grammar ();
}

સ્થિર શૂન્ય
prepare_percent_define_front_end_variables (શૂન્ય)
{
  /* Set %define front-end variable defaults.  */
  muscle_percent_define_default ("lr.keep-unreachable-state", "false");
  {
    /* IELR would be a better default, but LALR is historically the
       default.  */
    muscle_percent_define_default ("lr.type", "lalr");
    અક્ષર *lr_type = muscle_percent_define_get ("lr.type");
    જો (STRNEQ (lr_type, "canonical-lr"))
      muscle_percent_define_default ("lr.default-reduction", "most");
    નહીંતર
      muscle_percent_define_default ("lr.default-reduction", "accepting");
    મુક્ત (lr_type);
  }
  muscle_percent_define_default ("tool.xsltproc", "xsltproc");

  /* Check %define front-end variables.  */
  {
    સ્થિર અક્ષર અચલ * અચલ કિંમતો[] =
      {
       "lr.type", "lr""(0)", "lalr", "ielr", "canonical-lr", ખાલી,
       "lr.default-reduction", "most", "consistent", "accepting", ખાલી,
       ખાલી
      };
    muscle_percent_define_check_કિંમતો (કિંમતો);
  }
}

/* Find the first LHS which is not a dummy.  */

સ્થિર પ્રતીક *
find_start_પ્રતીક (શૂન્ય)
{
  પ્રતીક_list *res = grammar;
  /* Skip all the possible dummy rules of the first rule.  */
  માટે (; પ્રતીક_is_dummy (res->content.sym); res = res->next)
    /* Skip the LHS, and then all the RHS of the dummy rule.  */
    માટે (res = res->next; res->content.sym; res = res->next)
      ચાલુ;
  પાછા res->content.sym;
}


/* Insert an initial rule, whose location is that of the first rule
   (not that of the start symbol):

   $accept: SWITCHING_TOKEN START $end.  */
સ્થિર શૂન્ય
create_start_rule (પ્રતીક *swtok, પ્રતીક *start)
{
  પ્રતીક_list *initial_rule = પ્રતીક_list_sym_new (acceptsymbol, empty_loc);
  initial_rule->rhs_loc = grammar->rhs_loc;
  પ્રતીક_list *p = initial_rule;
  જો (swtok)
    {
      // Cannot create the action now, as the symbols have not yet
      // been assigned their number (by symbol_pack), which we need to
      // know the type name.  So the action is created in
      // grammar_rule_check_and_complete, which is run after
      // symbol_pack.
      p->next = પ્રતીક_list_sym_new (swtok, empty_loc);
      p = p->next;
    }
  p->next = પ્રતીક_list_sym_new (start, empty_loc);
  p = p->next;
  p->next = પ્રતીક_list_sym_new (eoftoken, empty_loc);
  p = p->next;
  p->next = પ્રતીક_list_sym_new (ખાલી, empty_loc);
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
પ્રતીક *
switching_token (અચલ પ્રતીક *start)
{
  અક્ષર buf[100];
  size_t len = કદ buf;
  અક્ષર *name = asnprintf (buf, &len, "YY_PARSE_%s", પ્રતીક_id_get (start));
  જો (!name)
    xalloc_die ();
  // Setting the location ensures deterministic symbol numbers.
  પ્રતીક *res = પ્રતીક_get (name, start->location);
  જો (name != buf)
    મુક્ત (name);
  પ્રતીક_class_set (res, token_sym, start->location, false);
  પાછા res;
}

/* Create the start rules in reverse order, since they are inserted at
   the top of the grammar.  That way the rules follow the order of
   declaration to %start.  */

સ્થિર શૂન્ય
create_multiple_start_rules (પ્રતીક_list *start_syms)
{
  જો (start_syms)
    {
      create_multiple_start_rules (start_syms->next);
      પ્રતિજ્ઞા (start_syms->content_type == SYMLIST_પ્રતીક);
      પ્રતીક *start = start_syms->content.sym;
      પ્રતીક *swtok = switching_token (start);
      create_start_rule (swtok, start);
    }
}

/* For each start symbol "foo", create the rule "$accept: YY_FOO
   foo $end". */
સ્થિર શૂન્ય
create_start_rules (શૂન્ય)
{
  જો (!start_symbols)
    {
      પ્રતીક *start = find_start_પ્રતીક ();
      start_symbols = પ્રતીક_list_sym_new (start, start->location);
    }

  અચલ bool several = start_symbols->next;
  જો (several)
    create_multiple_start_rules (start_symbols);
  નહીંતર
    {
      પ્રતીક *start = start_symbols->content.sym;
      create_start_rule (ખાલી, start);
    }
}

સ્થિર શૂન્ય
check_start_symbols (શૂન્ય)
{
  અચલ bool multistart = start_symbols && start_symbols->next;
  // Sanity checks on the start symbols.
  માટે (પ્રતીક_list *list = start_symbols; list; list = list->next)
    {
      અચલ પ્રતીક *start = list->content.sym;
      જો (start->content->class == unknown_sym)
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
      જો (!multistart && start->content->class == token_sym)
        complain (&start->location, complaint,
                  _("the start symbol %s is a token"),
                  start->tag);
    }
  જો (complaint_status == status_complaint)
    exit (EXIT_FAILURE);
}


/*-------------------------------------------------------------.
| Check the grammar that has just been read, and convert it to |
| internal form.                                               |
`-------------------------------------------------------------*/

સ્થિર શૂન્ય
check_and_convert_grammar (શૂન્ય)
{
  /* Grammar has been read.  Do some checking.  */
  જો (nrules == 0)
    complain (ખાલી, fatal, _("no rules in the input grammar"));

  /* If the user did not define her EOFTOKEN, do it now. */
  જો (!eoftoken)
    {
      eoftoken = પ્રતીક_get ("YYEOF", empty_loc);
      જો (eoftoken->content->class == nterm_sym)
        complain (&eoftoken->location, complaint,
                  _("rule given for %s, which is a token"),
                  eoftoken->tag);
      eoftoken->content->class = token_sym;
      eoftoken->content->સંખ્યા = 0;
      /* Value specified by POSIX.  */
      eoftoken->content->code = 0;
      {
        પ્રતીક *alias = પ્રતીક_get ("$end", empty_loc);
        પ્રતીક_class_set (alias, token_sym, empty_loc, false);
        પ્રતીક_make_alias (eoftoken, alias, empty_loc);
      }
    }

  /* Insert the initial rule(s).  */
  create_start_rules ();

  /* Report any undefined symbols and consider them nonterminals.  */
  symbols_check_defined ();

  જો (પ્રતીક_સંખ્યા_MAXIMUM - nnterms < ntokens)
    complain (ખાલી, fatal, "too many symbols in input grammar (limit is %d)",
              પ્રતીક_સંખ્યા_MAXIMUM);

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
  માટે (પ્રતીક_list *sym = grammar; sym; sym = sym->next)
    code_props_translate_code (&sym->action_props);

  /* Convert the grammar into the format described in gram.h.  */
  packgram ();

  /* The grammar as a symbol_list is no longer needed. */
  પ્રતીક_list_free (grammar);
}
