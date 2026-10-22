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

#अन्तर्भाव <config.h>
#अन्तर्भाव "system.h"

#अन्तर्भाव <quote.h>
#अन्तर्भाव <vasnprintf.h>

#अन्तर्भाव "complain.h"
#अन्तर्भाव "conflicts.h"
#अन्तर्भाव "files.h"
#अन्तर्भाव "fixits.h"
#अन्तर्भाव "getargs.h"
#अन्तर्भाव "gram.h"
#अन्तर्भाव "muscle-tab.h"
#अन्तर्भाव "reader.h"
#अन्तर्भाव "symlist.h"
#अन्तर्भाव "symtab.h"
#अन्तर्भाव "scan-gram.h"
#अन्तर्भाव "scan-code.h"

स्थिर शून्य prepare_percent_define_front_end_variables (शून्य);
स्थिर शून्य check_and_convert_grammar (शून्य);

स्थिर चिन्ह_list *grammar = रिक्त;
चिन्ह_list *start_symbols = रिक्त;
merger_list *merge_functions = रिक्त;

/* Was %union seen?  */
bool union_seen = false;

/* Should rules have a default precedence?  */
bool default_prec = true;


शून्य
grammar_start_symbols_add (चिन्ह_list *syms)
{
  /* Report and ignore duplicates.  Append the others to START_SYMBOLS.  */
  चिन्ह_list *last = चिन्ह_list_last (start_symbols);
  प्रति (चिन्ह_list *l = syms; l && l->content.sym; /* nothing */)
    {
      /* Is there a previous definition?  */
      चिन्ह_list *first = चिन्ह_list_find_चिन्ह (start_symbols, l->content.sym);
      यदि (first)
        {
          duplicate_directive ("%start", first->sym_loc, l->sym_loc);
          चिन्ह_list *dupl = l;
          l = l->next;
          dupl->next = रिक्त;
          चिन्ह_list_free (dupl);
        }
      अन्यथा
        {
          यदि (last)
            {
              last->next = l;
              last = l;
            }
          अन्यथा
            {
              last = l;
              start_symbols = last;
            }
          चिन्ह_list *next = l->next;
          l->next = रिक्त;
          l = next;
        }
    }
}



/*------------------------------------------------------------------------.
| Return the merger index for a merging function named NAME.  Records the |
| function, if new, in MERGER_LIST.                                       |
`------------------------------------------------------------------------*/

स्थिर पूर्णाङ्क
get_merge_function (uniqstr name)
{
  यदि (! glr_parser)
    प्रत्यागम 0;

  merger_list *syms;
  merger_list head;
  पूर्णाङ्क n;

  head.next = merge_functions;
  प्रति (syms = &head, n = 1; syms->next; syms = syms->next, n += 1)
    यदि (UNIQSTR_EQ (name, syms->next->name))
      भङ्ग;
  यदि (syms->next == रिक्त)
    {
      syms->next = xmalloc (परिमाण syms->next[0]);
      syms->next->name = uniqstr_new (name);
      /* After all symbol type declarations have been parsed, packgram invokes
         record_merge_function_type to set the type.  */
      syms->next->sym = रिक्त;
      syms->next->next = रिक्त;
      merge_functions = head.next;
    }
  प्रत्यागम n;
}

/*-------------------------------------------------------------------.
| For the existing merging function with index MERGER, record that   |
| the result type is that of SYM, as required by the lhs (i.e., SYM) |
| of the rule whose %merge declaration is at DECLARATION_LOC.        |
`-------------------------------------------------------------------*/

स्थिर शून्य
record_merge_function_type (पूर्णाङ्क merger, चिन्ह *sym, location declaration_loc)
{
  यदि (merger <= 0)
    प्रत्यागम;

  uniqstr type
    = sym->content->type_name ? sym->content->type_name : uniqstr_new ("");

  merger_list *merge_function;
  पूर्णाङ्क merger_find = 1;
  प्रति (merge_function = merge_functions;
       merge_function != रिक्त && merger_find != merger;
       merge_function = merge_function->next)
    merger_find += 1;
  aver (merge_function != रिक्त && merger_find == merger);
  यदि (merge_function->sym && merge_function->sym->content->type_name)
    {
      यदि (!UNIQSTR_EQ (merge_function->sym->content->type_name, type))
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
  अन्यथा
    {
      merge_function->sym = sym;
      merge_function->type_declaration_loc = declaration_loc;
    }
}

/*--------------------------------------.
| Free all merge-function definitions.  |
`--------------------------------------*/

शून्य
free_merger_functions (शून्य)
{
  merger_list *L0 = merge_functions;
  यावत् (L0)
    {
      merger_list *L1 = L0->next;
      मुक्त (L0);
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
स्थिर चिन्ह_list *grammar_end = रिक्त;

/* Append SYM to the grammar.  */
स्थिर चिन्ह_list *
grammar_चिन्ह_append (चिन्ह *sym, location loc)
{
  चिन्ह_list *p = चिन्ह_list_sym_new (sym, loc);

  यदि (grammar_end)
    grammar_end->next = p;
  अन्यथा
    grammar = p;

  grammar_end = p;

  /* A null SYM stands for an end of rule; it is not an actual
     part of it.  */
  यदि (sym)
    ++nritems;

  प्रत्यागम p;
}

स्थिर शून्य
assign_named_ref (चिन्ह_list *p, named_ref *name)
{
  चिन्ह *sym = p->content.sym;

  यदि (name->id == sym->tag)
    {
      complain (&name->loc, Wother,
                _("duplicated symbol name for %s ignored"),
                quote (sym->tag));
      named_ref_free (name);
    }
  अन्यथा
    p->named_ref = name;
}


/* The rule currently being defined, and the previous rule.
   CURRENT_RULE points to the first LHS of the current rule, while
   PREVIOUS_RULE_END points to the *end* of the previous rule (NULL).  */
स्थिर चिन्ह_list *current_rule = रिक्त;
स्थिर चिन्ह_list *previous_rule_end = रिक्त;


/*----------------------------------------------.
| Create a new rule for LHS in to the GRAMMAR.  |
`----------------------------------------------*/

शून्य
grammar_current_rule_begin (चिन्ह *lhs, location loc,
                            named_ref *lhs_name)
{
  /* Start a new rule and record its lhs.  */
  ++nrules;
  previous_rule_end = grammar_end;

  current_rule = grammar_चिन्ह_append (lhs, loc);
  यदि (lhs_name)
    assign_named_ref (current_rule, named_ref_copy (lhs_name));

  /* Mark the rule's lhs as a nonterminal if not already so.  */
  यदि (lhs->content->class == unknown_sym || lhs->content->class == pct_type_sym)
    चिन्ह_class_set (lhs, nterm_sym, empty_loc, false);
  अन्यथा यदि (lhs->content->class == token_sym)
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

स्थिर bool
चिन्ह_should_be_used (चिन्ह_list ध्रुव *s, bool *midrule_warning)
{
  यदि (चिन्ह_code_props_get (s->content.sym, destructor)->code)
    प्रत्यागम true;
  यदि ((s->midrule && s->midrule->action_props.is_मूल्य_used)
      || (s->midrule_parent_rule
          && (चिन्ह_list_n_get (s->midrule_parent_rule,
                                 s->midrule_parent_rhs_index)
              ->action_props.is_मूल्य_used)))
    {
      *midrule_warning = true;
      प्रत्यागम true;
    }
  प्रत्यागम false;
}

/*-----------------------------------------------------------------.
| Check that the rule R is properly defined.  For instance, there  |
| should be no type clash on the default action.  Possibly install |
| the default action.                                              |
`-----------------------------------------------------------------*/

स्थिर शून्य
grammar_rule_check_and_complete (चिन्ह_list *r)
{
  ध्रुव चिन्ह *lhs = r->content.sym;
  ध्रुव चिन्ह *first_rhs = r->next->content.sym;

  /* Type check.

     If there is an action, then there is nothing we can do: the user
     is allowed to shoot herself in the foot.

     Don't worry about the default action if $$ is untyped, since $$'s
     value can't be used.  */
  यदि (!r->action_props.code && lhs->content->type_name)
    {
      /* If $$ is being set in default way, report if any type mismatch.  */
      यदि (first_rhs)
        {
          वर्ण ध्रुव *lhs_type = lhs->content->type_name;
          वर्ण ध्रुव *rhs_type =
            first_rhs->content->type_name ? first_rhs->content->type_name : "";
          यदि (!UNIQSTR_EQ (lhs_type, rhs_type))
            complain (&r->rhs_loc, Wother,
                      _("type clash on default action: <%s> != <%s>"),
                      lhs_type, rhs_type);
          अन्यथा
            {
              /* Install the default action only for C++.  */
              ध्रुव bool is_cxx =
                STREQ (language->language, "c++")
                || (skeleton && (STREQ (skeleton, "glr.cc")
                                 || STREQ (skeleton, "glr2.cc")
                                 || STREQ (skeleton, "lalr1.cc")));
              यदि (is_cxx)
                {
                  code_props_rule_action_init (&r->action_props, "{ $$ = $1; }",
                                               r->rhs_loc, r,
                                               /* name */ रिक्त,
                                               /* type */ रिक्त,
                                               /* is_predicate */ false);
                  code_props_translate_code (&r->action_props);
                }
            }
        }
      /* Warn if there is no default for $$ but we need one.  */
      अन्यथा
        complain (&r->rhs_loc, Wother,
                  _("empty rule for typed nonterminal, and no action"));
    }

  /* For each start symbol, build the action of its start rule.  Use
     the same obstack as the one used by scan-code, which is in charge
     of actions. */
  ध्रुव bool multistart = start_symbols && start_symbols->next;
  यदि (multistart && lhs == acceptsymbol)
    {
      ध्रुव चिन्ह *start = r->next->next->content.sym;
      यदि (start->content->type_name)
        obstack_printf (obstack_for_actions,
                        "{ ]b4_accept""([%s%d])[; }",
                        start->content->class == nterm_sym ? "orig " : "",
                        start->content->संख्या);
      अन्यथा
        obstack_printf (obstack_for_actions,
                        "{ ]b4_accept[; }");
      code_props_rule_action_init (&r->action_props,
                                   obstack_finish0 (obstack_for_actions),
                                   r->rhs_loc, r,
                                   /* name */ रिक्त,
                                   /* type */ रिक्त,
                                   /* is_predicate */ false);
    }


  /* Check that symbol values that should be used are in fact used.
     Don't check the generated start rules.  It has no action, so some
     rhs symbols may appear unused, but the parsing algorithm ensures
     that %destructor's are invoked appropriately.  */
  यदि (lhs != acceptsymbol)
    {
      पूर्णाङ्क n = 0;
      प्रति (चिन्ह_list ध्रुव *l = r; l && l->content.sym; l = l->next, ++n)
        {
          bool midrule_warning = false;
          यदि (!l->action_props.is_मूल्य_used
              && चिन्ह_should_be_used (l, &midrule_warning)
              /* The default action, $$ = $1, 'uses' both.  */
              && (r->action_props.code || (n != 0 && n != 1)))
            {
              warnings warn_flag = midrule_warning ? Wmidrule_मूल्ये : Wother;
              यदि (n)
                complain (&l->sym_loc, warn_flag, _("unused value: $%d"), n);
              अन्यथा
                complain (&l->rhs_loc, warn_flag, _("unset value: $$"));
            }
        }
    }

  /* Check that %empty => empty rule.  */
  यदि (r->percent_empty_loc.start.file
      && r->next && r->next->content.sym)
    {
      complain (&r->percent_empty_loc, complaint,
                _("%%empty on non-empty rule"));
      fixits_register (&r->percent_empty_loc, "");
    }

  /* Check that empty rule => %empty.  */
  यदि (!(r->next && r->next->content.sym)
      && !r->midrule_parent_rule
      && !r->percent_empty_loc.start.file
      && warning_is_enabled (Wempty_rule))
    {
      complain (&r->rhs_loc, Wempty_rule, _("empty rule without %%empty"));
      यदि (feature_flag & feature_caret)
        location_caret_suggestion (r->rhs_loc, "%empty", stderr);
      location loc = r->rhs_loc;
      loc.end = loc.start;
      fixits_register (&loc, " %empty ");
    }

  /* See comments in grammar_current_rule_prec_set for how POSIX
     mandates this complaint.  It's only for identifiers, so skip
     it for char literals and strings, which are always tokens.  */
  यदि (r->ruleprec
      && r->ruleprec->tag[0] != '\'' && r->ruleprec->tag[0] != '"'
      && r->ruleprec->content->status != declared
      && !r->ruleprec->content->prec)
    complain (&r->rhs_loc, Wother,
              _("token for %%prec is not defined: %s"), r->ruleprec->tag);

  /* Check that the (main) action was not typed.  */
  यदि (r->action_props.type)
    complain (&r->rhs_loc, Wother,
              _("only midrule actions can be typed: %s"), r->action_props.type);
}


/*-------------------------------------.
| End the currently being grown rule.  |
`-------------------------------------*/

शून्य
grammar_current_rule_end (location loc)
{
  /* Put an empty link in the list to mark the end of this rule  */
  grammar_चिन्ह_append (रिक्त, grammar_end->rhs_loc);
  current_rule->rhs_loc = loc;
}


/*-------------------------------------------------------------------.
| The previous action turns out to be a midrule action.  Attach it   |
| to the current rule, i.e., create a dummy symbol, attach it this   |
| midrule action, and append this dummy nonterminal to the current   |
| rule.                                                              |
`-------------------------------------------------------------------*/

शून्य
grammar_midrule_action (शून्य)
{
  /* Since the action was written out with this rule's number, we must
     give the new rule this number by inserting the new rule before
     it.  */

  /* Make a DUMMY nonterminal, whose location is that of the midrule
     action.  Create the MIDRULE.  */
  location dummy_loc = current_rule->action_props.location;
  चिन्ह *dummy = dummy_चिन्ह_get (dummy_loc);
  चिन्ह_type_set (dummy,
                   current_rule->action_props.type, current_rule->action_props.location);
  चिन्ह_list *midrule = चिन्ह_list_sym_new (dummy, dummy_loc);

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
                               /* name_ref */ रिक्त,
                               /* type */ रिक्त,
                               current_rule->action_props.is_predicate);
  code_props_none_init (&current_rule->action_props);

  midrule->expected_sr_conflicts = current_rule->expected_sr_conflicts;
  midrule->expected_rr_conflicts = current_rule->expected_rr_conflicts;
  current_rule->expected_sr_conflicts = -1;
  current_rule->expected_rr_conflicts = -1;

  यदि (previous_rule_end)
    previous_rule_end->next = midrule;
  अन्यथा
    grammar = midrule;

  /* End the dummy's rule.  */
  midrule->next = चिन्ह_list_sym_new (रिक्त, dummy_loc);
  midrule->next->next = current_rule;

  previous_rule_end = midrule->next;

  /* Insert the dummy nonterminal replacing the midrule action into
     the current rule.  Bind it to its dedicated rule.  */
  grammar_current_rule_चिन्ह_append (dummy, dummy_loc,
                                      action_name);
  grammar_end->midrule = midrule;
  midrule->midrule_parent_rule = current_rule;
  midrule->midrule_parent_rhs_index = चिन्ह_list_length (current_rule->next);
}

/* Set the precedence symbol of the current rule to PRECSYM. */

शून्य
grammar_current_rule_prec_set (चिन्ह *precsym, location loc)
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
  चिन्ह_class_set (precsym, token_sym, loc, false);
  यदि (current_rule->ruleprec)
    duplicate_rule_directive ("%prec",
                              current_rule->ruleprec->location, loc);
  अन्यथा
    current_rule->ruleprec = precsym;
}

/* Set %empty for the current rule. */

शून्य
grammar_current_rule_empty_set (location loc)
{
  /* If %empty is used and -Wno-empty-rule is not, then enable
     -Wempty-rule.  */
  यदि (warning_is_unset (Wempty_rule))
    warning_argmatch ("empty-rule", 0, 0);
  यदि (current_rule->percent_empty_loc.start.file)
    duplicate_rule_directive ("%empty",
                              current_rule->percent_empty_loc, loc);
  अन्यथा
    current_rule->percent_empty_loc = loc;
}

/* Attach dynamic precedence DPREC to the current rule. */

शून्य
grammar_current_rule_dprec_set (पूर्णाङ्क dprec, location loc)
{
  यदि (! glr_parser)
    complain (&loc, Wother, _("%s affects only GLR parsers"),
              "%dprec");
  यदि (dprec <= 0)
    complain (&loc, complaint, _("%s must be followed by positive number"),
              "%dprec");
  अन्यथा यदि (current_rule->dprec != 0)
    duplicate_rule_directive ("%dprec",
                              current_rule->dprec_loc, loc);
  अन्यथा
    {
      current_rule->dprec = dprec;
      current_rule->dprec_loc = loc;
    }
}

/* Attach a merge function NAME with argument type TYPE to current
   rule. */

शून्य
grammar_current_rule_merge_set (uniqstr name, location loc)
{
  यदि (! glr_parser)
    complain (&loc, Wother, _("%s affects only GLR parsers"),
              "%merge");
  यदि (current_rule->merger != 0)
    duplicate_rule_directive ("%merge",
                              current_rule->merger_declaration_loc, loc);
  अन्यथा
    {
      current_rule->merger = get_merge_function (name);
      current_rule->merger_declaration_loc = loc;
    }
}

/* Attach SYM to the current rule.  If needed, move the previous
   action as a midrule action.  */

शून्य
grammar_current_rule_चिन्ह_append (चिन्ह *sym, location loc,
                                    named_ref *name)
{
  यदि (current_rule->action_props.code)
    grammar_midrule_action ();
  चिन्ह_list *p = grammar_चिन्ह_append (sym, loc);
  यदि (name)
    assign_named_ref (p, name);
  यदि (sym->content->status == undeclared || sym->content->status == used)
    sym->content->status = needed;
}

शून्य
grammar_current_rule_action_append (ध्रुव वर्ण *action, location loc,
                                    named_ref *name, uniqstr type)
{
  यदि (current_rule->action_props.code)
    grammar_midrule_action ();
  यदि (type)
    complain (&loc, Wyacc,
              _("POSIX Yacc does not support typed midrule actions"));
  /* After all symbol declarations have been parsed, packgram invokes
     code_props_translate_code.  */
  code_props_rule_action_init (&current_rule->action_props, action, loc,
                               current_rule,
                               name, type,
                               /* is_predicate */ false);
}

शून्य
grammar_current_rule_predicate_append (ध्रुव वर्ण *pred, location loc)
{
  यदि (current_rule->action_props.code)
    grammar_midrule_action ();
  code_props_rule_action_init (&current_rule->action_props, pred, loc,
                               current_rule,
                               रिक्त, रिक्त,
                               /* is_predicate */ true);
}

/* Set the expected number of shift/reduce (reduce/reduce) conflicts
 * for the current rule.  If a midrule is encountered later, the count
 * is transferred to it and reset in the current rule to -1. */

शून्य
grammar_current_rule_expect_sr (पूर्णाङ्क मोजणी, location loc)
{
  (शून्य) loc;
  current_rule->expected_sr_conflicts = मोजणी;
}

शून्य
grammar_current_rule_expect_rr (पूर्णाङ्क मोजणी, location loc)
{
  यदि (! glr_parser)
    complain (&loc, Wother, _("%s affects only GLR parsers"),
              "%expect-rr");
  अन्यथा
    current_rule->expected_rr_conflicts = मोजणी;
}


/*---------------------------------------------.
| Build RULES and RITEM from what was parsed.  |
`---------------------------------------------*/

स्थिर शून्य
packgram (शून्य)
{
  पूर्णाङ्क itemno = 0;
  ritem = xnmalloc (nritems + 1, परिमाण *ritem);
  /* This sentinel is used by build_relations() in lalr.c.  */
  *ritem++ = 0;

  rule_संख्या ruleno = 0;
  rules = xnmalloc (nrules, परिमाण *rules);

  प्रति (चिन्ह_list *p = grammar; p; p = p->next)
    {
      चिन्ह_list *lhs = p;
      record_merge_function_type (lhs->merger, lhs->content.sym,
                                  lhs->merger_declaration_loc);
      /* If the midrule's $$ is set or its $n is used, remove the '$' from the
         symbol name so that it's a user-defined symbol so that the default
         %destructor and %printer apply.  */
      यदि (lhs->midrule_parent_rule /* i.e., symbol_is_dummy (lhs->content.sym).  */
          && (lhs->action_props.is_मूल्य_used
              || (चिन्ह_list_n_get (lhs->midrule_parent_rule,
                                     lhs->midrule_parent_rhs_index)
                  ->action_props.is_मूल्य_used)))
        lhs->content.sym->tag += 1;

      grammar_rule_check_and_complete (lhs);

      rules[ruleno].code = ruleno;
      rules[ruleno].संख्या = ruleno;
      rules[ruleno].lhs = lhs->content.sym->content;
      rules[ruleno].rhs = ritem + itemno;
      rules[ruleno].prec = रिक्त;
      rules[ruleno].dprec = lhs->dprec;
      rules[ruleno].merger = lhs->merger;
      rules[ruleno].precsym = रिक्त;
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
        प्रति (p = lhs->next; p->content.sym; p = p->next)
          {
            ++rule_length;

            /* Don't allow rule_length == INT_MAX, since that might
               cause confusion with strtol if INT_MAX == LONG_MAX.  */
            यदि (rule_length == INT_MAX)
              complain (&rules[ruleno].location, fatal, _("rule is too long"));

            /* item_number = symbol_number.
               But the former needs to contain more: negative rule numbers. */
            ritem[itemno++] =
              चिन्ह_संख्या_as_item_संख्या (p->content.sym->content->संख्या);
            /* A rule gets by default the precedence and associativity
               of its last token.  */
            यदि (p->content.sym->content->class == token_sym && default_prec)
              rules[ruleno].prec = p->content.sym->content;
          }
      }

      /* If this rule has a %prec,
         the specified symbol's precedence replaces the default.  */
      यदि (lhs->ruleprec)
        {
          rules[ruleno].precsym = lhs->ruleprec->content;
          rules[ruleno].prec = lhs->ruleprec->content;
        }

      /* An item ends by the rule number (negated).  */
      ritem[itemno++] = rule_संख्या_as_item_संख्या (ruleno);
      aver (itemno < ITEM_संख्या_MAX);
      ++ruleno;
      aver (ruleno < RULE_संख्या_MAX);
    }

  aver (itemno == nritems);

  यदि (trace_flag & trace_sets)
    ritem_print (stderr);
}


/*--------------------------------------------------------------.
| Read in the grammar specification and record it in the format |
| described in gram.h.                                          |
`--------------------------------------------------------------*/

शून्य
reader (ध्रुव वर्ण *gram)
{
  /* Set up symbol_table, semantic_type_table, and the built-in
     symbols.  */
  symbols_new ();

  gram_scanner_open (gram);
  parser_init ();
  gram_parse ();
  gram_scanner_close ();

  prepare_percent_define_front_end_variables ();

  यदि (complaint_status  < status_complaint)
    check_and_convert_grammar ();
}

स्थिर शून्य
prepare_percent_define_front_end_variables (शून्य)
{
  /* Set %define front-end variable defaults.  */
  muscle_percent_define_default ("lr.keep-unreachable-state", "false");
  {
    /* IELR would be a better default, but LALR is historically the
       default.  */
    muscle_percent_define_default ("lr.type", "lalr");
    वर्ण *lr_type = muscle_percent_define_get ("lr.type");
    यदि (STRNEQ (lr_type, "canonical-lr"))
      muscle_percent_define_default ("lr.default-reduction", "most");
    अन्यथा
      muscle_percent_define_default ("lr.default-reduction", "accepting");
    मुक्त (lr_type);
  }
  muscle_percent_define_default ("tool.xsltproc", "xsltproc");

  /* Check %define front-end variables.  */
  {
    स्थिर वर्ण ध्रुव * ध्रुव मूल्ये[] =
      {
       "lr.type", "lr""(0)", "lalr", "ielr", "canonical-lr", रिक्त,
       "lr.default-reduction", "most", "consistent", "accepting", रिक्त,
       रिक्त
      };
    muscle_percent_define_check_मूल्ये (मूल्ये);
  }
}

/* Find the first LHS which is not a dummy.  */

स्थिर चिन्ह *
find_start_चिन्ह (शून्य)
{
  चिन्ह_list *res = grammar;
  /* Skip all the possible dummy rules of the first rule.  */
  प्रति (; चिन्ह_is_dummy (res->content.sym); res = res->next)
    /* Skip the LHS, and then all the RHS of the dummy rule.  */
    प्रति (res = res->next; res->content.sym; res = res->next)
      अनुवर्त;
  प्रत्यागम res->content.sym;
}


/* Insert an initial rule, whose location is that of the first rule
   (not that of the start symbol):

   $accept: SWITCHING_TOKEN START $end.  */
स्थिर शून्य
create_start_rule (चिन्ह *swtok, चिन्ह *start)
{
  चिन्ह_list *initial_rule = चिन्ह_list_sym_new (acceptsymbol, empty_loc);
  initial_rule->rhs_loc = grammar->rhs_loc;
  चिन्ह_list *p = initial_rule;
  यदि (swtok)
    {
      // Cannot create the action now, as the symbols have not yet
      // been assigned their number (by symbol_pack), which we need to
      // know the type name.  So the action is created in
      // grammar_rule_check_and_complete, which is run after
      // symbol_pack.
      p->next = चिन्ह_list_sym_new (swtok, empty_loc);
      p = p->next;
    }
  p->next = चिन्ह_list_sym_new (start, empty_loc);
  p = p->next;
  p->next = चिन्ह_list_sym_new (eoftoken, empty_loc);
  p = p->next;
  p->next = चिन्ह_list_sym_new (रिक्त, empty_loc);
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
चिन्ह *
switching_token (ध्रुव चिन्ह *start)
{
  वर्ण buf[100];
  size_t len = परिमाण buf;
  वर्ण *name = asnprintf (buf, &len, "YY_PARSE_%s", चिन्ह_id_get (start));
  यदि (!name)
    xalloc_die ();
  // Setting the location ensures deterministic symbol numbers.
  चिन्ह *res = चिन्ह_get (name, start->location);
  यदि (name != buf)
    मुक्त (name);
  चिन्ह_class_set (res, token_sym, start->location, false);
  प्रत्यागम res;
}

/* Create the start rules in reverse order, since they are inserted at
   the top of the grammar.  That way the rules follow the order of
   declaration to %start.  */

स्थिर शून्य
create_multiple_start_rules (चिन्ह_list *start_syms)
{
  यदि (start_syms)
    {
      create_multiple_start_rules (start_syms->next);
      प्रतिज्ञा (start_syms->content_type == SYMLIST_चिन्ह);
      चिन्ह *start = start_syms->content.sym;
      चिन्ह *swtok = switching_token (start);
      create_start_rule (swtok, start);
    }
}

/* For each start symbol "foo", create the rule "$accept: YY_FOO
   foo $end". */
स्थिर शून्य
create_start_rules (शून्य)
{
  यदि (!start_symbols)
    {
      चिन्ह *start = find_start_चिन्ह ();
      start_symbols = चिन्ह_list_sym_new (start, start->location);
    }

  ध्रुव bool several = start_symbols->next;
  यदि (several)
    create_multiple_start_rules (start_symbols);
  अन्यथा
    {
      चिन्ह *start = start_symbols->content.sym;
      create_start_rule (रिक्त, start);
    }
}

स्थिर शून्य
check_start_symbols (शून्य)
{
  ध्रुव bool multistart = start_symbols && start_symbols->next;
  // Sanity checks on the start symbols.
  प्रति (चिन्ह_list *list = start_symbols; list; list = list->next)
    {
      ध्रुव चिन्ह *start = list->content.sym;
      यदि (start->content->class == unknown_sym)
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
      यदि (!multistart && start->content->class == token_sym)
        complain (&start->location, complaint,
                  _("the start symbol %s is a token"),
                  start->tag);
    }
  यदि (complaint_status == status_complaint)
    exit (EXIT_FAILURE);
}


/*-------------------------------------------------------------.
| Check the grammar that has just been read, and convert it to |
| internal form.                                               |
`-------------------------------------------------------------*/

स्थिर शून्य
check_and_convert_grammar (शून्य)
{
  /* Grammar has been read.  Do some checking.  */
  यदि (nrules == 0)
    complain (रिक्त, fatal, _("no rules in the input grammar"));

  /* If the user did not define her EOFTOKEN, do it now. */
  यदि (!eoftoken)
    {
      eoftoken = चिन्ह_get ("YYEOF", empty_loc);
      यदि (eoftoken->content->class == nterm_sym)
        complain (&eoftoken->location, complaint,
                  _("rule given for %s, which is a token"),
                  eoftoken->tag);
      eoftoken->content->class = token_sym;
      eoftoken->content->संख्या = 0;
      /* Value specified by POSIX.  */
      eoftoken->content->code = 0;
      {
        चिन्ह *alias = चिन्ह_get ("$end", empty_loc);
        चिन्ह_class_set (alias, token_sym, empty_loc, false);
        चिन्ह_make_alias (eoftoken, alias, empty_loc);
      }
    }

  /* Insert the initial rule(s).  */
  create_start_rules ();

  /* Report any undefined symbols and consider them nonterminals.  */
  symbols_check_defined ();

  यदि (चिन्ह_संख्या_MAXIMUM - nnterms < ntokens)
    complain (रिक्त, fatal, "too many symbols in input grammar (limit is %d)",
              चिन्ह_संख्या_MAXIMUM);

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
  प्रति (चिन्ह_list *sym = grammar; sym; sym = sym->next)
    code_props_translate_code (&sym->action_props);

  /* Convert the grammar into the format described in gram.h.  */
  packgram ();

  /* The grammar as a symbol_list is no longer needed. */
  चिन्ह_list_free (grammar);
}
