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

#ضمن <config.h>
#ضمن "system.h"

#ضمن <quote.h>
#ضمن <vasnprintf.h>

#ضمن "complain.h"
#ضمن "conflicts.h"
#ضمن "files.h"
#ضمن "fixits.h"
#ضمن "getargs.h"
#ضمن "gram.h"
#ضمن "muscle-tab.h"
#ضمن "reader.h"
#ضمن "symlist.h"
#ضمن "symtab.h"
#ضمن "scan-gram.h"
#ضمن "scan-code.h"

ساكن فراغ prepare_percent_define_front_end_variables (فراغ);
ساكن فراغ check_and_convert_grammar (فراغ);

ساكن رمز_list *grammar = عدم;
رمز_list *start_symbols = عدم;
merger_list *merge_functions = عدم;

/* Was %union seen?  */
bool union_seen = false;

/* Should rules have a default precedence?  */
bool default_prec = true;


فراغ
grammar_start_symbols_add (رمز_list *syms)
{
  /* Report and ignore duplicates.  Append the others to START_SYMBOLS.  */
  رمز_list *last = رمز_list_last (start_symbols);
  لكل (رمز_list *l = syms; l && l->content.sym; /* nothing */)
    {
      /* Is there a previous definition?  */
      رمز_list *first = رمز_list_find_رمز (start_symbols, l->content.sym);
      إذا (first)
        {
          duplicate_directive ("%start", first->sym_loc, l->sym_loc);
          رمز_list *dupl = l;
          l = l->next;
          dupl->next = عدم;
          رمز_list_free (dupl);
        }
      وإلا
        {
          إذا (last)
            {
              last->next = l;
              last = l;
            }
          وإلا
            {
              last = l;
              start_symbols = last;
            }
          رمز_list *next = l->next;
          l->next = عدم;
          l = next;
        }
    }
}



/*------------------------------------------------------------------------.
| Return the merger index for a merging function named NAME.  Records the |
| function, if new, in MERGER_LIST.                                       |
`------------------------------------------------------------------------*/

ساكن صحيح
get_merge_function (uniqstr name)
{
  إذا (! glr_parser)
    إرجاع 0;

  merger_list *syms;
  merger_list head;
  صحيح n;

  head.next = merge_functions;
  لكل (syms = &head, n = 1; syms->next; syms = syms->next, n += 1)
    إذا (UNIQSTR_EQ (name, syms->next->name))
      اكسر;
  إذا (syms->next == عدم)
    {
      syms->next = xmalloc (حجم syms->next[0]);
      syms->next->name = uniqstr_new (name);
      /* After all symbol type declarations have been parsed, packgram invokes
         record_merge_function_type to set the type.  */
      syms->next->sym = عدم;
      syms->next->next = عدم;
      merge_functions = head.next;
    }
  إرجاع n;
}

/*-------------------------------------------------------------------.
| For the existing merging function with index MERGER, record that   |
| the result type is that of SYM, as required by the lhs (i.e., SYM) |
| of the rule whose %merge declaration is at DECLARATION_LOC.        |
`-------------------------------------------------------------------*/

ساكن فراغ
record_merge_function_type (صحيح merger, رمز *sym, location declaration_loc)
{
  إذا (merger <= 0)
    إرجاع;

  uniqstr type
    = sym->content->type_name ? sym->content->type_name : uniqstr_new ("");

  merger_list *merge_function;
  صحيح merger_find = 1;
  لكل (merge_function = merge_functions;
       merge_function != عدم && merger_find != merger;
       merge_function = merge_function->next)
    merger_find += 1;
  aver (merge_function != عدم && merger_find == merger);
  إذا (merge_function->sym && merge_function->sym->content->type_name)
    {
      إذا (!UNIQSTR_EQ (merge_function->sym->content->type_name, type))
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
  وإلا
    {
      merge_function->sym = sym;
      merge_function->type_declaration_loc = declaration_loc;
    }
}

/*--------------------------------------.
| Free all merge-function definitions.  |
`--------------------------------------*/

فراغ
free_merger_functions (فراغ)
{
  merger_list *L0 = merge_functions;
  بينما (L0)
    {
      merger_list *L1 = L0->next;
      حرر (L0);
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
ساكن رمز_list *grammar_end = عدم;

/* Append SYM to the grammar.  */
ساكن رمز_list *
grammar_رمز_append (رمز *sym, location loc)
{
  رمز_list *p = رمز_list_sym_new (sym, loc);

  إذا (grammar_end)
    grammar_end->next = p;
  وإلا
    grammar = p;

  grammar_end = p;

  /* A null SYM stands for an end of rule; it is not an actual
     part of it.  */
  إذا (sym)
    ++nritems;

  إرجاع p;
}

ساكن فراغ
assign_named_ref (رمز_list *p, named_ref *name)
{
  رمز *sym = p->content.sym;

  إذا (name->id == sym->tag)
    {
      complain (&name->loc, Wother,
                _("duplicated symbol name for %s ignored"),
                quote (sym->tag));
      named_ref_free (name);
    }
  وإلا
    p->named_ref = name;
}


/* The rule currently being defined, and the previous rule.
   CURRENT_RULE points to the first LHS of the current rule, while
   PREVIOUS_RULE_END points to the *end* of the previous rule (NULL).  */
ساكن رمز_list *current_rule = عدم;
ساكن رمز_list *previous_rule_end = عدم;


/*----------------------------------------------.
| Create a new rule for LHS in to the GRAMMAR.  |
`----------------------------------------------*/

فراغ
grammar_current_rule_begin (رمز *lhs, location loc,
                            named_ref *lhs_name)
{
  /* Start a new rule and record its lhs.  */
  ++nrules;
  previous_rule_end = grammar_end;

  current_rule = grammar_رمز_append (lhs, loc);
  إذا (lhs_name)
    assign_named_ref (current_rule, named_ref_copy (lhs_name));

  /* Mark the rule's lhs as a nonterminal if not already so.  */
  إذا (lhs->content->class == unknown_sym || lhs->content->class == pct_type_sym)
    رمز_class_set (lhs, nterm_sym, empty_loc, false);
  وإلا إذا (lhs->content->class == token_sym)
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

ساكن bool
رمز_should_be_used (رمز_list ثابت *s, bool *midrule_warning)
{
  إذا (رمز_code_props_get (s->content.sym, destructor)->code)
    إرجاع true;
  إذا ((s->midrule && s->midrule->action_props.is_قيمة_used)
      || (s->midrule_parent_rule
          && (رمز_list_n_get (s->midrule_parent_rule,
                                 s->midrule_parent_rhs_index)
              ->action_props.is_قيمة_used)))
    {
      *midrule_warning = true;
      إرجاع true;
    }
  إرجاع false;
}

/*-----------------------------------------------------------------.
| Check that the rule R is properly defined.  For instance, there  |
| should be no type clash on the default action.  Possibly install |
| the default action.                                              |
`-----------------------------------------------------------------*/

ساكن فراغ
grammar_rule_check_and_complete (رمز_list *r)
{
  ثابت رمز *lhs = r->content.sym;
  ثابت رمز *first_rhs = r->next->content.sym;

  /* Type check.

     If there is an action, then there is nothing we can do: the user
     is allowed to shoot herself in the foot.

     Don't worry about the default action if $$ is untyped, since $$'s
     value can't be used.  */
  إذا (!r->action_props.code && lhs->content->type_name)
    {
      /* If $$ is being set in default way, report if any type mismatch.  */
      إذا (first_rhs)
        {
          حرف ثابت *lhs_type = lhs->content->type_name;
          حرف ثابت *rhs_type =
            first_rhs->content->type_name ? first_rhs->content->type_name : "";
          إذا (!UNIQSTR_EQ (lhs_type, rhs_type))
            complain (&r->rhs_loc, Wother,
                      _("type clash on default action: <%s> != <%s>"),
                      lhs_type, rhs_type);
          وإلا
            {
              /* Install the default action only for C++.  */
              ثابت bool is_cxx =
                STREQ (language->language, "c++")
                || (skeleton && (STREQ (skeleton, "glr.cc")
                                 || STREQ (skeleton, "glr2.cc")
                                 || STREQ (skeleton, "lalr1.cc")));
              إذا (is_cxx)
                {
                  code_props_rule_action_init (&r->action_props, "{ $$ = $1; }",
                                               r->rhs_loc, r,
                                               /* name */ عدم,
                                               /* type */ عدم,
                                               /* is_predicate */ false);
                  code_props_translate_code (&r->action_props);
                }
            }
        }
      /* Warn if there is no default for $$ but we need one.  */
      وإلا
        complain (&r->rhs_loc, Wother,
                  _("empty rule for typed nonterminal, and no action"));
    }

  /* For each start symbol, build the action of its start rule.  Use
     the same obstack as the one used by scan-code, which is in charge
     of actions. */
  ثابت bool multistart = start_symbols && start_symbols->next;
  إذا (multistart && lhs == acceptsymbol)
    {
      ثابت رمز *start = r->next->next->content.sym;
      إذا (start->content->type_name)
        obstack_printf (obstack_for_actions,
                        "{ ]b4_accept""([%s%d])[; }",
                        start->content->class == nterm_sym ? "orig " : "",
                        start->content->العدد);
      وإلا
        obstack_printf (obstack_for_actions,
                        "{ ]b4_accept[; }");
      code_props_rule_action_init (&r->action_props,
                                   obstack_finish0 (obstack_for_actions),
                                   r->rhs_loc, r,
                                   /* name */ عدم,
                                   /* type */ عدم,
                                   /* is_predicate */ false);
    }


  /* Check that symbol values that should be used are in fact used.
     Don't check the generated start rules.  It has no action, so some
     rhs symbols may appear unused, but the parsing algorithm ensures
     that %destructor's are invoked appropriately.  */
  إذا (lhs != acceptsymbol)
    {
      صحيح n = 0;
      لكل (رمز_list ثابت *l = r; l && l->content.sym; l = l->next, ++n)
        {
          bool midrule_warning = false;
          إذا (!l->action_props.is_قيمة_used
              && رمز_should_be_used (l, &midrule_warning)
              /* The default action, $$ = $1, 'uses' both.  */
              && (r->action_props.code || (n != 0 && n != 1)))
            {
              warnings warn_flag = midrule_warning ? Wmidrule_قيم : Wother;
              إذا (n)
                complain (&l->sym_loc, warn_flag, _("unused value: $%d"), n);
              وإلا
                complain (&l->rhs_loc, warn_flag, _("unset value: $$"));
            }
        }
    }

  /* Check that %empty => empty rule.  */
  إذا (r->percent_empty_loc.start.file
      && r->next && r->next->content.sym)
    {
      complain (&r->percent_empty_loc, complaint,
                _("%%empty on non-empty rule"));
      fixits_register (&r->percent_empty_loc, "");
    }

  /* Check that empty rule => %empty.  */
  إذا (!(r->next && r->next->content.sym)
      && !r->midrule_parent_rule
      && !r->percent_empty_loc.start.file
      && warning_is_enabled (Wempty_rule))
    {
      complain (&r->rhs_loc, Wempty_rule, _("empty rule without %%empty"));
      إذا (feature_flag & feature_caret)
        location_caret_suggestion (r->rhs_loc, "%empty", stderr);
      location loc = r->rhs_loc;
      loc.end = loc.start;
      fixits_register (&loc, " %empty ");
    }

  /* See comments in grammar_current_rule_prec_set for how POSIX
     mandates this complaint.  It's only for identifiers, so skip
     it for char literals and strings, which are always tokens.  */
  إذا (r->ruleprec
      && r->ruleprec->tag[0] != '\'' && r->ruleprec->tag[0] != '"'
      && r->ruleprec->content->status != declared
      && !r->ruleprec->content->prec)
    complain (&r->rhs_loc, Wother,
              _("token for %%prec is not defined: %s"), r->ruleprec->tag);

  /* Check that the (main) action was not typed.  */
  إذا (r->action_props.type)
    complain (&r->rhs_loc, Wother,
              _("only midrule actions can be typed: %s"), r->action_props.type);
}


/*-------------------------------------.
| End the currently being grown rule.  |
`-------------------------------------*/

فراغ
grammar_current_rule_end (location loc)
{
  /* Put an empty link in the list to mark the end of this rule  */
  grammar_رمز_append (عدم, grammar_end->rhs_loc);
  current_rule->rhs_loc = loc;
}


/*-------------------------------------------------------------------.
| The previous action turns out to be a midrule action.  Attach it   |
| to the current rule, i.e., create a dummy symbol, attach it this   |
| midrule action, and append this dummy nonterminal to the current   |
| rule.                                                              |
`-------------------------------------------------------------------*/

فراغ
grammar_midrule_action (فراغ)
{
  /* Since the action was written out with this rule's number, we must
     give the new rule this number by inserting the new rule before
     it.  */

  /* Make a DUMMY nonterminal, whose location is that of the midrule
     action.  Create the MIDRULE.  */
  location dummy_loc = current_rule->action_props.location;
  رمز *dummy = dummy_رمز_get (dummy_loc);
  رمز_type_set (dummy,
                   current_rule->action_props.type, current_rule->action_props.location);
  رمز_list *midrule = رمز_list_sym_new (dummy, dummy_loc);

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
                               /* name_ref */ عدم,
                               /* type */ عدم,
                               current_rule->action_props.is_predicate);
  code_props_none_init (&current_rule->action_props);

  midrule->expected_sr_conflicts = current_rule->expected_sr_conflicts;
  midrule->expected_rr_conflicts = current_rule->expected_rr_conflicts;
  current_rule->expected_sr_conflicts = -1;
  current_rule->expected_rr_conflicts = -1;

  إذا (previous_rule_end)
    previous_rule_end->next = midrule;
  وإلا
    grammar = midrule;

  /* End the dummy's rule.  */
  midrule->next = رمز_list_sym_new (عدم, dummy_loc);
  midrule->next->next = current_rule;

  previous_rule_end = midrule->next;

  /* Insert the dummy nonterminal replacing the midrule action into
     the current rule.  Bind it to its dedicated rule.  */
  grammar_current_rule_رمز_append (dummy, dummy_loc,
                                      action_name);
  grammar_end->midrule = midrule;
  midrule->midrule_parent_rule = current_rule;
  midrule->midrule_parent_rhs_index = رمز_list_length (current_rule->next);
}

/* Set the precedence symbol of the current rule to PRECSYM. */

فراغ
grammar_current_rule_prec_set (رمز *precsym, location loc)
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
  رمز_class_set (precsym, token_sym, loc, false);
  إذا (current_rule->ruleprec)
    duplicate_rule_directive ("%prec",
                              current_rule->ruleprec->location, loc);
  وإلا
    current_rule->ruleprec = precsym;
}

/* Set %empty for the current rule. */

فراغ
grammar_current_rule_empty_set (location loc)
{
  /* If %empty is used and -Wno-empty-rule is not, then enable
     -Wempty-rule.  */
  إذا (warning_is_unset (Wempty_rule))
    warning_argmatch ("empty-rule", 0, 0);
  إذا (current_rule->percent_empty_loc.start.file)
    duplicate_rule_directive ("%empty",
                              current_rule->percent_empty_loc, loc);
  وإلا
    current_rule->percent_empty_loc = loc;
}

/* Attach dynamic precedence DPREC to the current rule. */

فراغ
grammar_current_rule_dprec_set (صحيح dprec, location loc)
{
  إذا (! glr_parser)
    complain (&loc, Wother, _("%s affects only GLR parsers"),
              "%dprec");
  إذا (dprec <= 0)
    complain (&loc, complaint, _("%s must be followed by positive number"),
              "%dprec");
  وإلا إذا (current_rule->dprec != 0)
    duplicate_rule_directive ("%dprec",
                              current_rule->dprec_loc, loc);
  وإلا
    {
      current_rule->dprec = dprec;
      current_rule->dprec_loc = loc;
    }
}

/* Attach a merge function NAME with argument type TYPE to current
   rule. */

فراغ
grammar_current_rule_merge_set (uniqstr name, location loc)
{
  إذا (! glr_parser)
    complain (&loc, Wother, _("%s affects only GLR parsers"),
              "%merge");
  إذا (current_rule->merger != 0)
    duplicate_rule_directive ("%merge",
                              current_rule->merger_declaration_loc, loc);
  وإلا
    {
      current_rule->merger = get_merge_function (name);
      current_rule->merger_declaration_loc = loc;
    }
}

/* Attach SYM to the current rule.  If needed, move the previous
   action as a midrule action.  */

فراغ
grammar_current_rule_رمز_append (رمز *sym, location loc,
                                    named_ref *name)
{
  إذا (current_rule->action_props.code)
    grammar_midrule_action ();
  رمز_list *p = grammar_رمز_append (sym, loc);
  إذا (name)
    assign_named_ref (p, name);
  إذا (sym->content->status == undeclared || sym->content->status == used)
    sym->content->status = needed;
}

فراغ
grammar_current_rule_action_append (ثابت حرف *action, location loc,
                                    named_ref *name, uniqstr type)
{
  إذا (current_rule->action_props.code)
    grammar_midrule_action ();
  إذا (type)
    complain (&loc, Wyacc,
              _("POSIX Yacc does not support typed midrule actions"));
  /* After all symbol declarations have been parsed, packgram invokes
     code_props_translate_code.  */
  code_props_rule_action_init (&current_rule->action_props, action, loc,
                               current_rule,
                               name, type,
                               /* is_predicate */ false);
}

فراغ
grammar_current_rule_predicate_append (ثابت حرف *pred, location loc)
{
  إذا (current_rule->action_props.code)
    grammar_midrule_action ();
  code_props_rule_action_init (&current_rule->action_props, pred, loc,
                               current_rule,
                               عدم, عدم,
                               /* is_predicate */ true);
}

/* Set the expected number of shift/reduce (reduce/reduce) conflicts
 * for the current rule.  If a midrule is encountered later, the count
 * is transferred to it and reset in the current rule to -1. */

فراغ
grammar_current_rule_expect_sr (صحيح تعداد_2, location loc)
{
  (فراغ) loc;
  current_rule->expected_sr_conflicts = تعداد_2;
}

فراغ
grammar_current_rule_expect_rr (صحيح تعداد_2, location loc)
{
  إذا (! glr_parser)
    complain (&loc, Wother, _("%s affects only GLR parsers"),
              "%expect-rr");
  وإلا
    current_rule->expected_rr_conflicts = تعداد_2;
}


/*---------------------------------------------.
| Build RULES and RITEM from what was parsed.  |
`---------------------------------------------*/

ساكن فراغ
packgram (فراغ)
{
  صحيح itemno = 0;
  ritem = xnmalloc (nritems + 1, حجم *ritem);
  /* This sentinel is used by build_relations() in lalr.c.  */
  *ritem++ = 0;

  rule_العدد ruleno = 0;
  rules = xnmalloc (nrules, حجم *rules);

  لكل (رمز_list *p = grammar; p; p = p->next)
    {
      رمز_list *lhs = p;
      record_merge_function_type (lhs->merger, lhs->content.sym,
                                  lhs->merger_declaration_loc);
      /* If the midrule's $$ is set or its $n is used, remove the '$' from the
         symbol name so that it's a user-defined symbol so that the default
         %destructor and %printer apply.  */
      إذا (lhs->midrule_parent_rule /* i.e., symbol_is_dummy (lhs->content.sym).  */
          && (lhs->action_props.is_قيمة_used
              || (رمز_list_n_get (lhs->midrule_parent_rule,
                                     lhs->midrule_parent_rhs_index)
                  ->action_props.is_قيمة_used)))
        lhs->content.sym->tag += 1;

      grammar_rule_check_and_complete (lhs);

      rules[ruleno].code = ruleno;
      rules[ruleno].العدد = ruleno;
      rules[ruleno].lhs = lhs->content.sym->content;
      rules[ruleno].rhs = ritem + itemno;
      rules[ruleno].prec = عدم;
      rules[ruleno].dprec = lhs->dprec;
      rules[ruleno].merger = lhs->merger;
      rules[ruleno].precsym = عدم;
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
        لكل (p = lhs->next; p->content.sym; p = p->next)
          {
            ++rule_length;

            /* Don't allow rule_length == INT_MAX, since that might
               cause confusion with strtol if INT_MAX == LONG_MAX.  */
            إذا (rule_length == INT_MAX)
              complain (&rules[ruleno].location, fatal, _("rule is too long"));

            /* item_number = symbol_number.
               But the former needs to contain more: negative rule numbers. */
            ritem[itemno++] =
              رمز_العدد_as_item_العدد (p->content.sym->content->العدد);
            /* A rule gets by default the precedence and associativity
               of its last token.  */
            إذا (p->content.sym->content->class == token_sym && default_prec)
              rules[ruleno].prec = p->content.sym->content;
          }
      }

      /* If this rule has a %prec,
         the specified symbol's precedence replaces the default.  */
      إذا (lhs->ruleprec)
        {
          rules[ruleno].precsym = lhs->ruleprec->content;
          rules[ruleno].prec = lhs->ruleprec->content;
        }

      /* An item ends by the rule number (negated).  */
      ritem[itemno++] = rule_العدد_as_item_العدد (ruleno);
      aver (itemno < ITEM_العدد_MAX);
      ++ruleno;
      aver (ruleno < RULE_العدد_MAX);
    }

  aver (itemno == nritems);

  إذا (trace_flag & trace_sets)
    ritem_print (stderr);
}


/*--------------------------------------------------------------.
| Read in the grammar specification and record it in the format |
| described in gram.h.                                          |
`--------------------------------------------------------------*/

فراغ
reader (ثابت حرف *gram)
{
  /* Set up symbol_table, semantic_type_table, and the built-in
     symbols.  */
  symbols_new ();

  gram_scanner_open (gram);
  parser_init ();
  gram_parse ();
  gram_scanner_close ();

  prepare_percent_define_front_end_variables ();

  إذا (complaint_status  < status_complaint)
    check_and_convert_grammar ();
}

ساكن فراغ
prepare_percent_define_front_end_variables (فراغ)
{
  /* Set %define front-end variable defaults.  */
  muscle_percent_define_default ("lr.keep-unreachable-state", "false");
  {
    /* IELR would be a better default, but LALR is historically the
       default.  */
    muscle_percent_define_default ("lr.type", "lalr");
    حرف *lr_type = muscle_percent_define_get ("lr.type");
    إذا (STRNEQ (lr_type, "canonical-lr"))
      muscle_percent_define_default ("lr.default-reduction", "most");
    وإلا
      muscle_percent_define_default ("lr.default-reduction", "accepting");
    حرر (lr_type);
  }
  muscle_percent_define_default ("tool.xsltproc", "xsltproc");

  /* Check %define front-end variables.  */
  {
    ساكن حرف ثابت * ثابت قيم[] =
      {
       "lr.type", "lr""(0)", "lalr", "ielr", "canonical-lr", عدم,
       "lr.default-reduction", "most", "consistent", "accepting", عدم,
       عدم
      };
    muscle_percent_define_check_قيم (قيم);
  }
}

/* Find the first LHS which is not a dummy.  */

ساكن رمز *
find_start_رمز (فراغ)
{
  رمز_list *res = grammar;
  /* Skip all the possible dummy rules of the first rule.  */
  لكل (; رمز_is_dummy (res->content.sym); res = res->next)
    /* Skip the LHS, and then all the RHS of the dummy rule.  */
    لكل (res = res->next; res->content.sym; res = res->next)
      استمر;
  إرجاع res->content.sym;
}


/* Insert an initial rule, whose location is that of the first rule
   (not that of the start symbol):

   $accept: SWITCHING_TOKEN START $end.  */
ساكن فراغ
create_start_rule (رمز *swtok, رمز *start)
{
  رمز_list *initial_rule = رمز_list_sym_new (acceptsymbol, empty_loc);
  initial_rule->rhs_loc = grammar->rhs_loc;
  رمز_list *p = initial_rule;
  إذا (swtok)
    {
      // Cannot create the action now, as the symbols have not yet
      // been assigned their number (by symbol_pack), which we need to
      // know the type name.  So the action is created in
      // grammar_rule_check_and_complete, which is run after
      // symbol_pack.
      p->next = رمز_list_sym_new (swtok, empty_loc);
      p = p->next;
    }
  p->next = رمز_list_sym_new (start, empty_loc);
  p = p->next;
  p->next = رمز_list_sym_new (eoftoken, empty_loc);
  p = p->next;
  p->next = رمز_list_sym_new (عدم, empty_loc);
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
رمز *
switching_token (ثابت رمز *start)
{
  حرف buf[100];
  size_t len = حجم buf;
  حرف *name = asnprintf (buf, &len, "YY_PARSE_%s", رمز_id_get (start));
  إذا (!name)
    xalloc_die ();
  // Setting the location ensures deterministic symbol numbers.
  رمز *res = رمز_get (name, start->location);
  إذا (name != buf)
    حرر (name);
  رمز_class_set (res, token_sym, start->location, false);
  إرجاع res;
}

/* Create the start rules in reverse order, since they are inserted at
   the top of the grammar.  That way the rules follow the order of
   declaration to %start.  */

ساكن فراغ
create_multiple_start_rules (رمز_list *start_syms)
{
  إذا (start_syms)
    {
      create_multiple_start_rules (start_syms->next);
      أكد (start_syms->content_type == SYMLIST_رمز);
      رمز *start = start_syms->content.sym;
      رمز *swtok = switching_token (start);
      create_start_rule (swtok, start);
    }
}

/* For each start symbol "foo", create the rule "$accept: YY_FOO
   foo $end". */
ساكن فراغ
create_start_rules (فراغ)
{
  إذا (!start_symbols)
    {
      رمز *start = find_start_رمز ();
      start_symbols = رمز_list_sym_new (start, start->location);
    }

  ثابت bool several = start_symbols->next;
  إذا (several)
    create_multiple_start_rules (start_symbols);
  وإلا
    {
      رمز *start = start_symbols->content.sym;
      create_start_rule (عدم, start);
    }
}

ساكن فراغ
check_start_symbols (فراغ)
{
  ثابت bool multistart = start_symbols && start_symbols->next;
  // Sanity checks on the start symbols.
  لكل (رمز_list *list = start_symbols; list; list = list->next)
    {
      ثابت رمز *start = list->content.sym;
      إذا (start->content->class == unknown_sym)
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
      إذا (!multistart && start->content->class == token_sym)
        complain (&start->location, complaint,
                  _("the start symbol %s is a token"),
                  start->tag);
    }
  إذا (complaint_status == status_complaint)
    exit (EXIT_FAILURE);
}


/*-------------------------------------------------------------.
| Check the grammar that has just been read, and convert it to |
| internal form.                                               |
`-------------------------------------------------------------*/

ساكن فراغ
check_and_convert_grammar (فراغ)
{
  /* Grammar has been read.  Do some checking.  */
  إذا (nrules == 0)
    complain (عدم, fatal, _("no rules in the input grammar"));

  /* If the user did not define her EOFTOKEN, do it now. */
  إذا (!eoftoken)
    {
      eoftoken = رمز_get ("YYEOF", empty_loc);
      إذا (eoftoken->content->class == nterm_sym)
        complain (&eoftoken->location, complaint,
                  _("rule given for %s, which is a token"),
                  eoftoken->tag);
      eoftoken->content->class = token_sym;
      eoftoken->content->العدد = 0;
      /* Value specified by POSIX.  */
      eoftoken->content->code = 0;
      {
        رمز *alias = رمز_get ("$end", empty_loc);
        رمز_class_set (alias, token_sym, empty_loc, false);
        رمز_make_alias (eoftoken, alias, empty_loc);
      }
    }

  /* Insert the initial rule(s).  */
  create_start_rules ();

  /* Report any undefined symbols and consider them nonterminals.  */
  symbols_check_defined ();

  إذا (رمز_العدد_MAXIMUM - nnterms < ntokens)
    complain (عدم, fatal, "too many symbols in input grammar (limit is %d)",
              رمز_العدد_MAXIMUM);

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
  لكل (رمز_list *sym = grammar; sym; sym = sym->next)
    code_props_translate_code (&sym->action_props);

  /* Convert the grammar into the format described in gram.h.  */
  packgram ();

  /* The grammar as a symbol_list is no longer needed. */
  رمز_list_free (grammar);
}
