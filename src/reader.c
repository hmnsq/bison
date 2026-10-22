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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব "system.h"

#অন্তর্ভাব <quote.h>
#অন্তর্ভাব <vasnprintf.h>

#অন্তর্ভাব "complain.h"
#অন্তর্ভাব "conflicts.h"
#অন্তর্ভাব "files.h"
#অন্তর্ভাব "fixits.h"
#অন্তর্ভাব "getargs.h"
#অন্তর্ভাব "gram.h"
#অন্তর্ভাব "muscle-tab.h"
#অন্তর্ভাব "reader.h"
#অন্তর্ভাব "symlist.h"
#অন্তর্ভাব "symtab.h"
#অন্তর্ভাব "scan-gram.h"
#অন্তর্ভাব "scan-code.h"

স্থির শূন্য prepare_percent_define_front_end_variables (শূন্য);
স্থির শূন্য check_and_convert_grammar (শূন্য);

স্থির প্রতীক_list *grammar = ফাঁকা;
প্রতীক_list *start_symbols = ফাঁকা;
merger_list *merge_functions = ফাঁকা;

/* Was %union seen?  */
bool union_seen = false;

/* Should rules have a default precedence?  */
bool default_prec = true;


শূন্য
grammar_start_symbols_add (প্রতীক_list *syms)
{
  /* Report and ignore duplicates.  Append the others to START_SYMBOLS.  */
  প্রতীক_list *last = প্রতীক_list_last (start_symbols);
  জন্য (প্রতীক_list *l = syms; l && l->content.sym; /* nothing */)
    {
      /* Is there a previous definition?  */
      প্রতীক_list *first = প্রতীক_list_find_প্রতীক (start_symbols, l->content.sym);
      যদি (first)
        {
          duplicate_directive ("%start", first->sym_loc, l->sym_loc);
          প্রতীক_list *dupl = l;
          l = l->পরবর্তী;
          dupl->পরবর্তী = ফাঁকা;
          প্রতীক_list_free (dupl);
        }
      অন্যথা
        {
          যদি (last)
            {
              last->পরবর্তী = l;
              last = l;
            }
          অন্যথা
            {
              last = l;
              start_symbols = last;
            }
          প্রতীক_list *পরবর্তী = l->পরবর্তী;
          l->পরবর্তী = ফাঁকা;
          l = পরবর্তী;
        }
    }
}



/*------------------------------------------------------------------------.
| Return the merger index for a merging function named NAME.  Records the |
| function, if new, in MERGER_LIST.                                       |
`------------------------------------------------------------------------*/

স্থির পূর্ণসংখ্যা
get_merge_function (uniqstr name)
{
  যদি (! glr_parser)
    ফেরত 0;

  merger_list *syms;
  merger_list শীর্ষ_3;
  পূর্ণসংখ্যা n;

  শীর্ষ_3.পরবর্তী = merge_functions;
  জন্য (syms = &শীর্ষ_3, n = 1; syms->পরবর্তী; syms = syms->পরবর্তী, n += 1)
    যদি (UNIQSTR_EQ (name, syms->পরবর্তী->name))
      বিরতি;
  যদি (syms->পরবর্তী == ফাঁকা)
    {
      syms->পরবর্তী = xmalloc (আকার syms->পরবর্তী[0]);
      syms->পরবর্তী->name = uniqstr_new (name);
      /* After all symbol type declarations have been parsed, packgram invokes
         record_merge_function_type to set the type.  */
      syms->পরবর্তী->sym = ফাঁকা;
      syms->পরবর্তী->পরবর্তী = ফাঁকা;
      merge_functions = শীর্ষ_3.পরবর্তী;
    }
  ফেরত n;
}

/*-------------------------------------------------------------------.
| For the existing merging function with index MERGER, record that   |
| the result type is that of SYM, as required by the lhs (i.e., SYM) |
| of the rule whose %merge declaration is at DECLARATION_LOC.        |
`-------------------------------------------------------------------*/

স্থির শূন্য
record_merge_function_type (পূর্ণসংখ্যা merger, প্রতীক *sym, location declaration_loc)
{
  যদি (merger <= 0)
    ফেরত;

  uniqstr type
    = sym->content->type_name ? sym->content->type_name : uniqstr_new ("");

  merger_list *merge_function;
  পূর্ণসংখ্যা merger_find = 1;
  জন্য (merge_function = merge_functions;
       merge_function != ফাঁকা && merger_find != merger;
       merge_function = merge_function->পরবর্তী)
    merger_find += 1;
  aver (merge_function != ফাঁকা && merger_find == merger);
  যদি (merge_function->sym && merge_function->sym->content->type_name)
    {
      যদি (!UNIQSTR_EQ (merge_function->sym->content->type_name, type))
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
  অন্যথা
    {
      merge_function->sym = sym;
      merge_function->type_declaration_loc = declaration_loc;
    }
}

/*--------------------------------------.
| Free all merge-function definitions.  |
`--------------------------------------*/

শূন্য
free_merger_functions (শূন্য)
{
  merger_list *L0 = merge_functions;
  যখন (L0)
    {
      merger_list *L1 = L0->পরবর্তী;
      মুক্ত (L0);
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
স্থির প্রতীক_list *grammar_end = ফাঁকা;

/* Append SYM to the grammar.  */
স্থির প্রতীক_list *
grammar_প্রতীক_append (প্রতীক *sym, location loc)
{
  প্রতীক_list *p = প্রতীক_list_sym_new (sym, loc);

  যদি (grammar_end)
    grammar_end->পরবর্তী = p;
  অন্যথা
    grammar = p;

  grammar_end = p;

  /* A null SYM stands for an end of rule; it is not an actual
     part of it.  */
  যদি (sym)
    ++nritems;

  ফেরত p;
}

স্থির শূন্য
assign_named_ref (প্রতীক_list *p, named_ref *name)
{
  প্রতীক *sym = p->content.sym;

  যদি (name->id == sym->tag)
    {
      complain (&name->loc, Wother,
                _("duplicated symbol name for %s ignored"),
                quote (sym->tag));
      named_ref_free (name);
    }
  অন্যথা
    p->named_ref = name;
}


/* The rule currently being defined, and the previous rule.
   CURRENT_RULE points to the first LHS of the current rule, while
   PREVIOUS_RULE_END points to the *end* of the previous rule (NULL).  */
স্থির প্রতীক_list *current_rule = ফাঁকা;
স্থির প্রতীক_list *previous_rule_end = ফাঁকা;


/*----------------------------------------------.
| Create a new rule for LHS in to the GRAMMAR.  |
`----------------------------------------------*/

শূন্য
grammar_current_rule_begin (প্রতীক *lhs, location loc,
                            named_ref *lhs_name)
{
  /* Start a new rule and record its lhs.  */
  ++nrules;
  previous_rule_end = grammar_end;

  current_rule = grammar_প্রতীক_append (lhs, loc);
  যদি (lhs_name)
    assign_named_ref (current_rule, named_ref_copy (lhs_name));

  /* Mark the rule's lhs as a nonterminal if not already so.  */
  যদি (lhs->content->class == unknown_sym || lhs->content->class == pct_type_sym)
    প্রতীক_class_set (lhs, nterm_sym, empty_loc, false);
  অন্যথা যদি (lhs->content->class == token_sym)
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

স্থির bool
প্রতীক_should_be_used (প্রতীক_list ধ্রুবক *s, bool *midrule_warning)
{
  যদি (প্রতীক_code_props_get (s->content.sym, destructor)->code)
    ফেরত true;
  যদি ((s->midrule && s->midrule->action_props.is_মান_used)
      || (s->midrule_parent_rule
          && (প্রতীক_list_n_get (s->midrule_parent_rule,
                                 s->midrule_parent_rhs_সূচক)
              ->action_props.is_মান_used)))
    {
      *midrule_warning = true;
      ফেরত true;
    }
  ফেরত false;
}

/*-----------------------------------------------------------------.
| Check that the rule R is properly defined.  For instance, there  |
| should be no type clash on the default action.  Possibly install |
| the default action.                                              |
`-----------------------------------------------------------------*/

স্থির শূন্য
grammar_rule_check_and_complete (প্রতীক_list *r)
{
  ধ্রুবক প্রতীক *lhs = r->content.sym;
  ধ্রুবক প্রতীক *first_rhs = r->পরবর্তী->content.sym;

  /* Type check.

     If there is an action, then there is nothing we can do: the user
     is allowed to shoot herself in the foot.

     Don't worry about the default action if $$ is untyped, since $$'s
     value can't be used.  */
  যদি (!r->action_props.code && lhs->content->type_name)
    {
      /* If $$ is being set in default way, report if any type mismatch.  */
      যদি (first_rhs)
        {
          অক্ষর ধ্রুবক *lhs_type = lhs->content->type_name;
          অক্ষর ধ্রুবক *rhs_type =
            first_rhs->content->type_name ? first_rhs->content->type_name : "";
          যদি (!UNIQSTR_EQ (lhs_type, rhs_type))
            complain (&r->rhs_loc, Wother,
                      _("type clash on default action: <%s> != <%s>"),
                      lhs_type, rhs_type);
          অন্যথা
            {
              /* Install the default action only for C++.  */
              ধ্রুবক bool is_cxx =
                STREQ (language->language, "c++")
                || (skeleton && (STREQ (skeleton, "glr.cc")
                                 || STREQ (skeleton, "glr2.cc")
                                 || STREQ (skeleton, "lalr1.cc")));
              যদি (is_cxx)
                {
                  code_props_rule_action_init (&r->action_props, "{ $$ = $1; }",
                                               r->rhs_loc, r,
                                               /* name */ ফাঁকা,
                                               /* type */ ফাঁকা,
                                               /* is_predicate */ false);
                  code_props_translate_code (&r->action_props);
                }
            }
        }
      /* Warn if there is no default for $$ but we need one.  */
      অন্যথা
        complain (&r->rhs_loc, Wother,
                  _("empty rule for typed nonterminal, and no action"));
    }

  /* For each start symbol, build the action of its start rule.  Use
     the same obstack as the one used by scan-code, which is in charge
     of actions. */
  ধ্রুবক bool multistart = start_symbols && start_symbols->পরবর্তী;
  যদি (multistart && lhs == acceptsymbol)
    {
      ধ্রুবক প্রতীক *start = r->পরবর্তী->পরবর্তী->content.sym;
      যদি (start->content->type_name)
        obstack_printf (obstack_for_actions,
                        "{ ]b4_accept""([%s%d])[; }",
                        start->content->class == nterm_sym ? "orig " : "",
                        start->content->সংখ্যা);
      অন্যথা
        obstack_printf (obstack_for_actions,
                        "{ ]b4_accept[; }");
      code_props_rule_action_init (&r->action_props,
                                   obstack_finish0 (obstack_for_actions),
                                   r->rhs_loc, r,
                                   /* name */ ফাঁকা,
                                   /* type */ ফাঁকা,
                                   /* is_predicate */ false);
    }


  /* Check that symbol values that should be used are in fact used.
     Don't check the generated start rules.  It has no action, so some
     rhs symbols may appear unused, but the parsing algorithm ensures
     that %destructor's are invoked appropriately.  */
  যদি (lhs != acceptsymbol)
    {
      পূর্ণসংখ্যা n = 0;
      জন্য (প্রতীক_list ধ্রুবক *l = r; l && l->content.sym; l = l->পরবর্তী, ++n)
        {
          bool midrule_warning = false;
          যদি (!l->action_props.is_মান_used
              && প্রতীক_should_be_used (l, &midrule_warning)
              /* The default action, $$ = $1, 'uses' both.  */
              && (r->action_props.code || (n != 0 && n != 1)))
            {
              warnings warn_flag = midrule_warning ? Wmidrule_মানসমূহ : Wother;
              যদি (n)
                complain (&l->sym_loc, warn_flag, _("unused value: $%d"), n);
              অন্যথা
                complain (&l->rhs_loc, warn_flag, _("unset value: $$"));
            }
        }
    }

  /* Check that %empty => empty rule.  */
  যদি (r->percent_empty_loc.start.file
      && r->পরবর্তী && r->পরবর্তী->content.sym)
    {
      complain (&r->percent_empty_loc, complaint,
                _("%%empty on non-empty rule"));
      fixits_register (&r->percent_empty_loc, "");
    }

  /* Check that empty rule => %empty.  */
  যদি (!(r->পরবর্তী && r->পরবর্তী->content.sym)
      && !r->midrule_parent_rule
      && !r->percent_empty_loc.start.file
      && warning_is_enabled (Wempty_rule))
    {
      complain (&r->rhs_loc, Wempty_rule, _("empty rule without %%empty"));
      যদি (feature_flag & feature_caret)
        location_caret_suggestion (r->rhs_loc, "%empty", stderr);
      location loc = r->rhs_loc;
      loc.end = loc.start;
      fixits_register (&loc, " %empty ");
    }

  /* See comments in grammar_current_rule_prec_set for how POSIX
     mandates this complaint.  It's only for identifiers, so skip
     it for char literals and strings, which are always tokens.  */
  যদি (r->ruleprec
      && r->ruleprec->tag[0] != '\'' && r->ruleprec->tag[0] != '"'
      && r->ruleprec->content->status != declared
      && !r->ruleprec->content->prec)
    complain (&r->rhs_loc, Wother,
              _("token for %%prec is not defined: %s"), r->ruleprec->tag);

  /* Check that the (main) action was not typed.  */
  যদি (r->action_props.type)
    complain (&r->rhs_loc, Wother,
              _("only midrule actions can be typed: %s"), r->action_props.type);
}


/*-------------------------------------.
| End the currently being grown rule.  |
`-------------------------------------*/

শূন্য
grammar_current_rule_end (location loc)
{
  /* Put an empty link in the list to mark the end of this rule  */
  grammar_প্রতীক_append (ফাঁকা, grammar_end->rhs_loc);
  current_rule->rhs_loc = loc;
}


/*-------------------------------------------------------------------.
| The previous action turns out to be a midrule action.  Attach it   |
| to the current rule, i.e., create a dummy symbol, attach it this   |
| midrule action, and append this dummy nonterminal to the current   |
| rule.                                                              |
`-------------------------------------------------------------------*/

শূন্য
grammar_midrule_action (শূন্য)
{
  /* Since the action was written out with this rule's number, we must
     give the new rule this number by inserting the new rule before
     it.  */

  /* Make a DUMMY nonterminal, whose location is that of the midrule
     action.  Create the MIDRULE.  */
  location dummy_loc = current_rule->action_props.location;
  প্রতীক *dummy = dummy_প্রতীক_get (dummy_loc);
  প্রতীক_type_set (dummy,
                   current_rule->action_props.type, current_rule->action_props.location);
  প্রতীক_list *midrule = প্রতীক_list_sym_new (dummy, dummy_loc);

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
                               /* name_ref */ ফাঁকা,
                               /* type */ ফাঁকা,
                               current_rule->action_props.is_predicate);
  code_props_none_init (&current_rule->action_props);

  midrule->expected_sr_conflicts = current_rule->expected_sr_conflicts;
  midrule->expected_rr_conflicts = current_rule->expected_rr_conflicts;
  current_rule->expected_sr_conflicts = -1;
  current_rule->expected_rr_conflicts = -1;

  যদি (previous_rule_end)
    previous_rule_end->পরবর্তী = midrule;
  অন্যথা
    grammar = midrule;

  /* End the dummy's rule.  */
  midrule->পরবর্তী = প্রতীক_list_sym_new (ফাঁকা, dummy_loc);
  midrule->পরবর্তী->পরবর্তী = current_rule;

  previous_rule_end = midrule->পরবর্তী;

  /* Insert the dummy nonterminal replacing the midrule action into
     the current rule.  Bind it to its dedicated rule.  */
  grammar_current_rule_প্রতীক_append (dummy, dummy_loc,
                                      action_name);
  grammar_end->midrule = midrule;
  midrule->midrule_parent_rule = current_rule;
  midrule->midrule_parent_rhs_সূচক = প্রতীক_list_দৈর্ঘ্য (current_rule->পরবর্তী);
}

/* Set the precedence symbol of the current rule to PRECSYM. */

শূন্য
grammar_current_rule_prec_set (প্রতীক *precsym, location loc)
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
  প্রতীক_class_set (precsym, token_sym, loc, false);
  যদি (current_rule->ruleprec)
    duplicate_rule_directive ("%prec",
                              current_rule->ruleprec->location, loc);
  অন্যথা
    current_rule->ruleprec = precsym;
}

/* Set %empty for the current rule. */

শূন্য
grammar_current_rule_empty_set (location loc)
{
  /* If %empty is used and -Wno-empty-rule is not, then enable
     -Wempty-rule.  */
  যদি (warning_is_unset (Wempty_rule))
    warning_argmatch ("empty-rule", 0, 0);
  যদি (current_rule->percent_empty_loc.start.file)
    duplicate_rule_directive ("%empty",
                              current_rule->percent_empty_loc, loc);
  অন্যথা
    current_rule->percent_empty_loc = loc;
}

/* Attach dynamic precedence DPREC to the current rule. */

শূন্য
grammar_current_rule_dprec_set (পূর্ণসংখ্যা dprec, location loc)
{
  যদি (! glr_parser)
    complain (&loc, Wother, _("%s affects only GLR parsers"),
              "%dprec");
  যদি (dprec <= 0)
    complain (&loc, complaint, _("%s must be followed by positive number"),
              "%dprec");
  অন্যথা যদি (current_rule->dprec != 0)
    duplicate_rule_directive ("%dprec",
                              current_rule->dprec_loc, loc);
  অন্যথা
    {
      current_rule->dprec = dprec;
      current_rule->dprec_loc = loc;
    }
}

/* Attach a merge function NAME with argument type TYPE to current
   rule. */

শূন্য
grammar_current_rule_merge_set (uniqstr name, location loc)
{
  যদি (! glr_parser)
    complain (&loc, Wother, _("%s affects only GLR parsers"),
              "%merge");
  যদি (current_rule->merger != 0)
    duplicate_rule_directive ("%merge",
                              current_rule->merger_declaration_loc, loc);
  অন্যথা
    {
      current_rule->merger = get_merge_function (name);
      current_rule->merger_declaration_loc = loc;
    }
}

/* Attach SYM to the current rule.  If needed, move the previous
   action as a midrule action.  */

শূন্য
grammar_current_rule_প্রতীক_append (প্রতীক *sym, location loc,
                                    named_ref *name)
{
  যদি (current_rule->action_props.code)
    grammar_midrule_action ();
  প্রতীক_list *p = grammar_প্রতীক_append (sym, loc);
  যদি (name)
    assign_named_ref (p, name);
  যদি (sym->content->status == undeclared || sym->content->status == used)
    sym->content->status = needed;
}

শূন্য
grammar_current_rule_action_append (ধ্রুবক অক্ষর *action, location loc,
                                    named_ref *name, uniqstr type)
{
  যদি (current_rule->action_props.code)
    grammar_midrule_action ();
  যদি (type)
    complain (&loc, Wyacc,
              _("POSIX Yacc does not support typed midrule actions"));
  /* After all symbol declarations have been parsed, packgram invokes
     code_props_translate_code.  */
  code_props_rule_action_init (&current_rule->action_props, action, loc,
                               current_rule,
                               name, type,
                               /* is_predicate */ false);
}

শূন্য
grammar_current_rule_predicate_append (ধ্রুবক অক্ষর *pred, location loc)
{
  যদি (current_rule->action_props.code)
    grammar_midrule_action ();
  code_props_rule_action_init (&current_rule->action_props, pred, loc,
                               current_rule,
                               ফাঁকা, ফাঁকা,
                               /* is_predicate */ true);
}

/* Set the expected number of shift/reduce (reduce/reduce) conflicts
 * for the current rule.  If a midrule is encountered later, the count
 * is transferred to it and reset in the current rule to -1. */

শূন্য
grammar_current_rule_expect_sr (পূর্ণসংখ্যা গুনতি, location loc)
{
  (শূন্য) loc;
  current_rule->expected_sr_conflicts = গুনতি;
}

শূন্য
grammar_current_rule_expect_rr (পূর্ণসংখ্যা গুনতি, location loc)
{
  যদি (! glr_parser)
    complain (&loc, Wother, _("%s affects only GLR parsers"),
              "%expect-rr");
  অন্যথা
    current_rule->expected_rr_conflicts = গুনতি;
}


/*---------------------------------------------.
| Build RULES and RITEM from what was parsed.  |
`---------------------------------------------*/

স্থির শূন্য
packgram (শূন্য)
{
  পূর্ণসংখ্যা itemno = 0;
  ritem = xnmalloc (nritems + 1, আকার *ritem);
  /* This sentinel is used by build_relations() in lalr.c.  */
  *ritem++ = 0;

  rule_সংখ্যা ruleno = 0;
  rules = xnmalloc (nrules, আকার *rules);

  জন্য (প্রতীক_list *p = grammar; p; p = p->পরবর্তী)
    {
      প্রতীক_list *lhs = p;
      record_merge_function_type (lhs->merger, lhs->content.sym,
                                  lhs->merger_declaration_loc);
      /* If the midrule's $$ is set or its $n is used, remove the '$' from the
         symbol name so that it's a user-defined symbol so that the default
         %destructor and %printer apply.  */
      যদি (lhs->midrule_parent_rule /* i.e., symbol_is_dummy (lhs->content.sym).  */
          && (lhs->action_props.is_মান_used
              || (প্রতীক_list_n_get (lhs->midrule_parent_rule,
                                     lhs->midrule_parent_rhs_সূচক)
                  ->action_props.is_মান_used)))
        lhs->content.sym->tag += 1;

      grammar_rule_check_and_complete (lhs);

      rules[ruleno].code = ruleno;
      rules[ruleno].সংখ্যা = ruleno;
      rules[ruleno].lhs = lhs->content.sym->content;
      rules[ruleno].rhs = ritem + itemno;
      rules[ruleno].prec = ফাঁকা;
      rules[ruleno].dprec = lhs->dprec;
      rules[ruleno].merger = lhs->merger;
      rules[ruleno].precsym = ফাঁকা;
      rules[ruleno].location = lhs->rhs_loc;
      rules[ruleno].useful = true;
      rules[ruleno].action = lhs->action_props.code;
      rules[ruleno].action_loc = lhs->action_props.location;
      rules[ruleno].is_predicate = lhs->action_props.is_predicate;
      rules[ruleno].expected_sr_conflicts = lhs->expected_sr_conflicts;
      rules[ruleno].expected_rr_conflicts = lhs->expected_rr_conflicts;

      /* Traverse the rhs.  */
      {
        size_t rule_দৈর্ঘ্য = 0;
        জন্য (p = lhs->পরবর্তী; p->content.sym; p = p->পরবর্তী)
          {
            ++rule_দৈর্ঘ্য;

            /* Don't allow rule_length == INT_MAX, since that might
               cause confusion with strtol if INT_MAX == LONG_MAX.  */
            যদি (rule_দৈর্ঘ্য == INT_MAX)
              complain (&rules[ruleno].location, fatal, _("rule is too long"));

            /* item_number = symbol_number.
               But the former needs to contain more: negative rule numbers. */
            ritem[itemno++] =
              প্রতীক_সংখ্যা_as_item_সংখ্যা (p->content.sym->content->সংখ্যা);
            /* A rule gets by default the precedence and associativity
               of its last token.  */
            যদি (p->content.sym->content->class == token_sym && default_prec)
              rules[ruleno].prec = p->content.sym->content;
          }
      }

      /* If this rule has a %prec,
         the specified symbol's precedence replaces the default.  */
      যদি (lhs->ruleprec)
        {
          rules[ruleno].precsym = lhs->ruleprec->content;
          rules[ruleno].prec = lhs->ruleprec->content;
        }

      /* An item ends by the rule number (negated).  */
      ritem[itemno++] = rule_সংখ্যা_as_item_সংখ্যা (ruleno);
      aver (itemno < ITEM_সংখ্যা_MAX);
      ++ruleno;
      aver (ruleno < RULE_সংখ্যা_MAX);
    }

  aver (itemno == nritems);

  যদি (trace_flag & trace_sets)
    ritem_print (stderr);
}


/*--------------------------------------------------------------.
| Read in the grammar specification and record it in the format |
| described in gram.h.                                          |
`--------------------------------------------------------------*/

শূন্য
reader (ধ্রুবক অক্ষর *gram)
{
  /* Set up symbol_table, semantic_type_table, and the built-in
     symbols.  */
  symbols_new ();

  gram_scanner_open (gram);
  parser_init ();
  gram_parse ();
  gram_scanner_close ();

  prepare_percent_define_front_end_variables ();

  যদি (complaint_status  < status_complaint)
    check_and_convert_grammar ();
}

স্থির শূন্য
prepare_percent_define_front_end_variables (শূন্য)
{
  /* Set %define front-end variable defaults.  */
  muscle_percent_define_default ("lr.keep-unreachable-state", "false");
  {
    /* IELR would be a better default, but LALR is historically the
       default.  */
    muscle_percent_define_default ("lr.type", "lalr");
    অক্ষর *lr_type = muscle_percent_define_get ("lr.type");
    যদি (STRNEQ (lr_type, "canonical-lr"))
      muscle_percent_define_default ("lr.default-reduction", "most");
    অন্যথা
      muscle_percent_define_default ("lr.default-reduction", "accepting");
    মুক্ত (lr_type);
  }
  muscle_percent_define_default ("tool.xsltproc", "xsltproc");

  /* Check %define front-end variables.  */
  {
    স্থির অক্ষর ধ্রুবক * ধ্রুবক মানসমূহ[] =
      {
       "lr.type", "lr""(0)", "lalr", "ielr", "canonical-lr", ফাঁকা,
       "lr.default-reduction", "most", "consistent", "accepting", ফাঁকা,
       ফাঁকা
      };
    muscle_percent_define_check_মানসমূহ (মানসমূহ);
  }
}

/* Find the first LHS which is not a dummy.  */

স্থির প্রতীক *
find_start_প্রতীক (শূন্য)
{
  প্রতীক_list *res = grammar;
  /* Skip all the possible dummy rules of the first rule.  */
  জন্য (; প্রতীক_is_dummy (res->content.sym); res = res->পরবর্তী)
    /* Skip the LHS, and then all the RHS of the dummy rule.  */
    জন্য (res = res->পরবর্তী; res->content.sym; res = res->পরবর্তী)
      চালু;
  ফেরত res->content.sym;
}


/* Insert an initial rule, whose location is that of the first rule
   (not that of the start symbol):

   $accept: SWITCHING_TOKEN START $end.  */
স্থির শূন্য
create_start_rule (প্রতীক *swtok, প্রতীক *start)
{
  প্রতীক_list *initial_rule = প্রতীক_list_sym_new (acceptsymbol, empty_loc);
  initial_rule->rhs_loc = grammar->rhs_loc;
  প্রতীক_list *p = initial_rule;
  যদি (swtok)
    {
      // Cannot create the action now, as the symbols have not yet
      // been assigned their number (by symbol_pack), which we need to
      // know the type name.  So the action is created in
      // grammar_rule_check_and_complete, which is run after
      // symbol_pack.
      p->পরবর্তী = প্রতীক_list_sym_new (swtok, empty_loc);
      p = p->পরবর্তী;
    }
  p->পরবর্তী = প্রতীক_list_sym_new (start, empty_loc);
  p = p->পরবর্তী;
  p->পরবর্তী = প্রতীক_list_sym_new (eoftoken, empty_loc);
  p = p->পরবর্তী;
  p->পরবর্তী = প্রতীক_list_sym_new (ফাঁকা, empty_loc);
  p = p->পরবর্তী;
  p->পরবর্তী = grammar;
  nrules += 1;
  nritems += 3 + !!swtok;
  grammar = initial_rule;
}

/* Fetch (or create) a token "YY_PARSE_foo" for start symbol "foo".

   We don't use the simple "YY_FOO" because (i) we might get clashes
   with some of our symbols (e.g., cast => YY_CAST), and (ii) upcasing
   introduces possible clashes between terminal FOO and nonterminal
   foo.  */
প্রতীক *
switching_token (ধ্রুবক প্রতীক *start)
{
  অক্ষর buf[100];
  size_t len = আকার buf;
  অক্ষর *name = asnprintf (buf, &len, "YY_PARSE_%s", প্রতীক_id_get (start));
  যদি (!name)
    xalloc_die ();
  // Setting the location ensures deterministic symbol numbers.
  প্রতীক *res = প্রতীক_get (name, start->location);
  যদি (name != buf)
    মুক্ত (name);
  প্রতীক_class_set (res, token_sym, start->location, false);
  ফেরত res;
}

/* Create the start rules in reverse order, since they are inserted at
   the top of the grammar.  That way the rules follow the order of
   declaration to %start.  */

স্থির শূন্য
create_multiple_start_rules (প্রতীক_list *start_syms)
{
  যদি (start_syms)
    {
      create_multiple_start_rules (start_syms->পরবর্তী);
      প্রতিজ্ঞা (start_syms->content_type == SYMLIST_প্রতীক);
      প্রতীক *start = start_syms->content.sym;
      প্রতীক *swtok = switching_token (start);
      create_start_rule (swtok, start);
    }
}

/* For each start symbol "foo", create the rule "$accept: YY_FOO
   foo $end". */
স্থির শূন্য
create_start_rules (শূন্য)
{
  যদি (!start_symbols)
    {
      প্রতীক *start = find_start_প্রতীক ();
      start_symbols = প্রতীক_list_sym_new (start, start->location);
    }

  ধ্রুবক bool several = start_symbols->পরবর্তী;
  যদি (several)
    create_multiple_start_rules (start_symbols);
  অন্যথা
    {
      প্রতীক *start = start_symbols->content.sym;
      create_start_rule (ফাঁকা, start);
    }
}

স্থির শূন্য
check_start_symbols (শূন্য)
{
  ধ্রুবক bool multistart = start_symbols && start_symbols->পরবর্তী;
  // Sanity checks on the start symbols.
  জন্য (প্রতীক_list *list = start_symbols; list; list = list->পরবর্তী)
    {
      ধ্রুবক প্রতীক *start = list->content.sym;
      যদি (start->content->class == unknown_sym)
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
      যদি (!multistart && start->content->class == token_sym)
        complain (&start->location, complaint,
                  _("the start symbol %s is a token"),
                  start->tag);
    }
  যদি (complaint_status == status_complaint)
    exit (EXIT_FAILURE);
}


/*-------------------------------------------------------------.
| Check the grammar that has just been read, and convert it to |
| internal form.                                               |
`-------------------------------------------------------------*/

স্থির শূন্য
check_and_convert_grammar (শূন্য)
{
  /* Grammar has been read.  Do some checking.  */
  যদি (nrules == 0)
    complain (ফাঁকা, fatal, _("no rules in the input grammar"));

  /* If the user did not define her EOFTOKEN, do it now. */
  যদি (!eoftoken)
    {
      eoftoken = প্রতীক_get ("YYEOF", empty_loc);
      যদি (eoftoken->content->class == nterm_sym)
        complain (&eoftoken->location, complaint,
                  _("rule given for %s, which is a token"),
                  eoftoken->tag);
      eoftoken->content->class = token_sym;
      eoftoken->content->সংখ্যা = 0;
      /* Value specified by POSIX.  */
      eoftoken->content->code = 0;
      {
        প্রতীক *alias = প্রতীক_get ("$end", empty_loc);
        প্রতীক_class_set (alias, token_sym, empty_loc, false);
        প্রতীক_make_alias (eoftoken, alias, empty_loc);
      }
    }

  /* Insert the initial rule(s).  */
  create_start_rules ();

  /* Report any undefined symbols and consider them nonterminals.  */
  symbols_check_defined ();

  যদি (প্রতীক_সংখ্যা_MAXIMUM - nnterms < ntokens)
    complain (ফাঁকা, fatal, "too many symbols in input grammar (limit is %d)",
              প্রতীক_সংখ্যা_MAXIMUM);

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
  জন্য (প্রতীক_list *sym = grammar; sym; sym = sym->পরবর্তী)
    code_props_translate_code (&sym->action_props);

  /* Convert the grammar into the format described in gram.h.  */
  packgram ();

  /* The grammar as a symbol_list is no longer needed. */
  প্রতীক_list_free (grammar);
}
