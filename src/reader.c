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

स्थिर शून्य prepare_percent_define_front_अन्तः_variables (शून्य);
स्थिर शून्य check_and_convert_grammar (शून्य);

स्थिर चिह्नम्_list *grammar = रिक्त;
चिह्नम्_list *start_symbols = रिक्त;
merger_list *merge_functions = रिक्त;

/* Was %union seen?  */
bool union_आलोकितम् = false;

/* Should rules have a default precedence?  */
bool default_prec = true;


शून्य
grammar_start_symbols_सङ्कलय (चिह्नम्_list *syms)
{
  /* Report and ignore duplicates.  Append the others to START_SYMBOLS.  */
  चिह्नम्_list *चरमः = चिह्नम्_list_चरमः (start_symbols);
  प्रति (चिह्नम्_list *l = syms; l && l->content.sym; /* nothing */)
    {
      /* Is there a previous definition?  */
      चिह्नम्_list *first = चिह्नम्_list_find_चिह्नम् (start_symbols, l->content.sym);
      यदि (first)
        {
          duplicate_directive ("%start", first->sym_loc, l->sym_loc);
          चिह्नम्_list *dupl = l;
          l = l->अग्र;
          dupl->अग्र = रिक्त;
          चिह्नम्_list_free (dupl);
        }
      अन्यथा
        {
          यदि (चरमः)
            {
              चरमः->अग्र = l;
              चरमः = l;
            }
          अन्यथा
            {
              चरमः = l;
              start_symbols = चरमः;
            }
          चिह्नम्_list *अग्र = l->अग्र;
          l->अग्र = रिक्त;
          l = अग्र;
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
  merger_list शीर्ष;
  पूर्णाङ्क n;

  शीर्ष.अग्र = merge_functions;
  प्रति (syms = &शीर्ष, n = 1; syms->अग्र; syms = syms->अग्र, n += 1)
    यदि (UNIQSTR_EQ (name, syms->अग्र->name))
      भङ्ग;
  यदि (syms->अग्र == रिक्त)
    {
      syms->अग्र = xmalloc (परिमाण syms->अग्र[0]);
      syms->अग्र->name = uniqstr_new (name);
      /* After all symbol type declarations have been parsed, packgram invokes
         record_merge_function_type to set the type.  */
      syms->अग्र->sym = रिक्त;
      syms->अग्र->अग्र = रिक्त;
      merge_functions = शीर्ष.अग्र;
    }
  प्रत्यागम n;
}

/*-------------------------------------------------------------------.
| For the existing merging function with index MERGER, record that   |
| the result type is that of SYM, as required by the lhs (i.e., SYM) |
| of the rule whose %merge declaration is at DECLARATION_LOC.        |
`-------------------------------------------------------------------*/

स्थिर शून्य
लेखः_merge_function_type (पूर्णाङ्क merger, चिह्नम् *sym, location declaration_loc)
{
  यदि (merger <= 0)
    प्रत्यागम;

  uniqstr type
    = sym->content->type_name ? sym->content->type_name : uniqstr_new ("");

  merger_list *merge_function;
  पूर्णाङ्क merger_find = 1;
  प्रति (merge_function = merge_functions;
       merge_function != रिक्त && merger_find != merger;
       merge_function = merge_function->अग्र)
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
      merger_list *L1 = L0->अग्र;
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
स्थिर चिह्नम्_list *grammar_अन्तः = रिक्त;

/* Append SYM to the grammar.  */
स्थिर चिह्नम्_list *
grammar_चिह्नम्_append (चिह्नम् *sym, location loc)
{
  चिह्नम्_list *p = चिह्नम्_list_sym_new (sym, loc);

  यदि (grammar_अन्तः)
    grammar_अन्तः->अग्र = p;
  अन्यथा
    grammar = p;

  grammar_अन्तः = p;

  /* A null SYM stands for an end of rule; it is not an actual
     part of it.  */
  यदि (sym)
    ++nritems;

  प्रत्यागम p;
}

स्थिर शून्य
नियोजय_named_ref (चिह्नम्_list *p, named_ref *name)
{
  चिह्नम् *sym = p->content.sym;

  यदि (name->id == sym->चिह्नकम्)
    {
      complain (&name->loc, Wother,
                _("duplicated symbol name for %s ignored"),
                quote (sym->चिह्नकम्));
      named_ref_free (name);
    }
  अन्यथा
    p->named_ref = name;
}


/* The rule currently being defined, and the previous rule.
   CURRENT_RULE points to the first LHS of the current rule, while
   PREVIOUS_RULE_END points to the *end* of the previous rule (NULL).  */
स्थिर चिह्नम्_list *current_नियमः = रिक्त;
स्थिर चिह्नम्_list *previous_नियमः_अन्तः = रिक्त;


/*----------------------------------------------.
| Create a new rule for LHS in to the GRAMMAR.  |
`----------------------------------------------*/

शून्य
grammar_current_नियमः_begin (चिह्नम् *lhs, location loc,
                            named_ref *lhs_name)
{
  /* Start a new rule and record its lhs.  */
  ++nrules;
  previous_नियमः_अन्तः = grammar_अन्तः;

  current_नियमः = grammar_चिह्नम्_append (lhs, loc);
  यदि (lhs_name)
    नियोजय_named_ref (current_नियमः, named_ref_copy (lhs_name));

  /* Mark the rule's lhs as a nonterminal if not already so.  */
  यदि (lhs->content->class == unknown_sym || lhs->content->class == pct_type_sym)
    चिह्नम्_class_समुच्चयः (lhs, nterm_sym, रिक्तानि_loc, false);
  अन्यथा यदि (lhs->content->class == token_sym)
    complain (&loc, complaint, _("rule given for %s, which is a token"),
              lhs->चिह्नकम्);
}


/*----------------------------------------------------------------------.
| A symbol should be used if either:                                    |
|   1. It has a destructor.                                             |
|   2. The symbol is a midrule symbol (i.e., the generated LHS          |
|      replacing a midrule action) that was assigned to or used, as in  |
|      "exp: { $$ = 1; } { $$ = $1; }".                                 |
`----------------------------------------------------------------------*/

स्थिर bool
चिह्नम्_should_be_प्रयुक्तम् (चिह्नम्_list ध्रुव *s, bool *midrule_warning)
{
  यदि (चिह्नम्_संहिता_props_get (s->content.sym, destructor)->संहिता)
    प्रत्यागम true;
  यदि ((s->midrule && s->midrule->action_props.is_मानम्_प्रयुक्तम्)
      || (s->midrule_parent_नियमः
          && (चिह्नम्_list_n_get (s->midrule_parent_नियमः,
                                 s->midrule_parent_rhs_सूचकांक)
              ->action_props.is_मानम्_प्रयुक्तम्)))
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
grammar_नियमः_check_and_complete (चिह्नम्_list *r)
{
  ध्रुव चिह्नम् *lhs = r->content.sym;
  ध्रुव चिह्नम् *first_rhs = r->अग्र->content.sym;

  /* Type check.

     If there is an action, then there is nothing we can do: the user
     is allowed to shoot herself in the foot.

     Don't worry about the default action if $$ is untyped, since $$'s
     value can't be used.  */
  यदि (!r->action_props.संहिता && lhs->content->type_name)
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
                  संहिता_props_नियमः_action_init (&r->action_props, "{ $$ = $1; }",
                                               r->rhs_loc, r,
                                               /* name */ रिक्त,
                                               /* type */ रिक्त,
                                               /* is_predicate */ false);
                  संहिता_props_translate_संहिता (&r->action_props);
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
  ध्रुव bool multistart = start_symbols && start_symbols->अग्र;
  यदि (multistart && lhs == acceptsymbol)
    {
      ध्रुव चिह्नम् *start = r->अग्र->अग्र->content.sym;
      यदि (start->content->type_name)
        obstack_printf (obstack_for_actions,
                        "{ ]b4_accept""([%s%d])[; }",
                        start->content->class == nterm_sym ? "orig " : "",
                        start->content->सङ्ख्या);
      अन्यथा
        obstack_printf (obstack_for_actions,
                        "{ ]b4_accept[; }");
      संहिता_props_नियमः_action_init (&r->action_props,
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
      प्रति (चिह्नम्_list ध्रुव *l = r; l && l->content.sym; l = l->अग्र, ++n)
        {
          bool midrule_warning = false;
          यदि (!l->action_props.is_मानम्_प्रयुक्तम्
              && चिह्नम्_should_be_प्रयुक्तम् (l, &midrule_warning)
              /* The default action, $$ = $1, 'uses' both.  */
              && (r->action_props.संहिता || (n != 0 && n != 1)))
            {
              warnings warn_flag = midrule_warning ? Wmidrule_मानानि : Wother;
              यदि (n)
                complain (&l->sym_loc, warn_flag, _("unused value: $%d"), n);
              अन्यथा
                complain (&l->rhs_loc, warn_flag, _("unset value: $$"));
            }
        }
    }

  /* Check that %empty => empty rule.  */
  यदि (r->percent_रिक्तानि_loc.start.file
      && r->अग्र && r->अग्र->content.sym)
    {
      complain (&r->percent_रिक्तानि_loc, complaint,
                _("%%empty on non-empty rule"));
      fixits_पञ्जिकाकोशः (&r->percent_रिक्तानि_loc, "");
    }

  /* Check that empty rule => %empty.  */
  यदि (!(r->अग्र && r->अग्र->content.sym)
      && !r->midrule_parent_नियमः
      && !r->percent_रिक्तानि_loc.start.file
      && warning_is_enabled (Wempty_नियमः))
    {
      complain (&r->rhs_loc, Wempty_नियमः, _("empty rule without %%empty"));
      यदि (feature_flag & feature_caret)
        location_caret_suggestion (r->rhs_loc, "%empty", stderr);
      location loc = r->rhs_loc;
      loc.अन्तः = loc.start;
      fixits_पञ्जिकाकोशः (&loc, " %empty ");
    }

  /* See comments in grammar_current_rule_prec_set for how POSIX
     mandates this complaint.  It's only for identifiers, so skip
     it for char literals and strings, which are always tokens.  */
  यदि (r->ruleprec
      && r->ruleprec->चिह्नकम्[0] != '\'' && r->ruleprec->चिह्नकम्[0] != '"'
      && r->ruleprec->content->स्थितिः != declared
      && !r->ruleprec->content->prec)
    complain (&r->rhs_loc, Wother,
              _("token for %%prec is not defined: %s"), r->ruleprec->चिह्नकम्);

  /* Check that the (main) action was not typed.  */
  यदि (r->action_props.type)
    complain (&r->rhs_loc, Wother,
              _("only midrule actions can be typed: %s"), r->action_props.type);
}


/*-------------------------------------.
| End the currently being grown rule.  |
`-------------------------------------*/

शून्य
grammar_current_नियमः_अन्तः (location loc)
{
  /* Put an empty link in the list to mark the end of this rule  */
  grammar_चिह्नम्_append (रिक्त, grammar_अन्तः->rhs_loc);
  current_नियमः->rhs_loc = loc;
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
  location dummy_loc = current_नियमः->action_props.location;
  चिह्नम् *dummy = dummy_चिह्नम्_get (dummy_loc);
  चिह्नम्_type_समुच्चयः (dummy,
                   current_नियमः->action_props.type, current_नियमः->action_props.location);
  चिह्नम्_list *midrule = चिह्नम्_list_sym_new (dummy, dummy_loc);

  /* Remember named_ref of previous action. */
  named_ref *action_name = current_नियमः->action_props.named_ref;

  /* Make a new rule, whose body is empty, before the current one, so
     that the action just read can belong to it.  */
  ++nrules;
  ++nritems;
  /* Attach its location and actions to that of the DUMMY.  */
  midrule->rhs_loc = dummy_loc;
  संहिता_props_नियमः_action_init (&midrule->action_props,
                               current_नियमः->action_props.संहिता,
                               current_नियमः->action_props.location,
                               midrule,
                               /* name_ref */ रिक्त,
                               /* type */ रिक्त,
                               current_नियमः->action_props.is_predicate);
  संहिता_props_none_init (&current_नियमः->action_props);

  midrule->expected_sr_conflicts = current_नियमः->expected_sr_conflicts;
  midrule->expected_rr_conflicts = current_नियमः->expected_rr_conflicts;
  current_नियमः->expected_sr_conflicts = -1;
  current_नियमः->expected_rr_conflicts = -1;

  यदि (previous_नियमः_अन्तः)
    previous_नियमः_अन्तः->अग्र = midrule;
  अन्यथा
    grammar = midrule;

  /* End the dummy's rule.  */
  midrule->अग्र = चिह्नम्_list_sym_new (रिक्त, dummy_loc);
  midrule->अग्र->अग्र = current_नियमः;

  previous_नियमः_अन्तः = midrule->अग्र;

  /* Insert the dummy nonterminal replacing the midrule action into
     the current rule.  Bind it to its dedicated rule.  */
  grammar_current_नियमः_चिह्नम्_append (dummy, dummy_loc,
                                      action_name);
  grammar_अन्तः->midrule = midrule;
  midrule->midrule_parent_नियमः = current_नियमः;
  midrule->midrule_parent_rhs_सूचकांक = चिह्नम्_list_दीर्घता (current_नियमः->अग्र);
}

/* Set the precedence symbol of the current rule to PRECSYM. */

शून्य
grammar_current_नियमः_prec_समुच्चयः (चिह्नम् *precsym, location loc)
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
  चिह्नम्_class_समुच्चयः (precsym, token_sym, loc, false);
  यदि (current_नियमः->ruleprec)
    duplicate_नियमः_directive ("%prec",
                              current_नियमः->ruleprec->location, loc);
  अन्यथा
    current_नियमः->ruleprec = precsym;
}

/* Set %empty for the current rule. */

शून्य
grammar_current_नियमः_रिक्तानि_समुच्चयः (location loc)
{
  /* If %empty is used and -Wno-empty-rule is not, then enable
     -Wempty-rule.  */
  यदि (warning_is_unset (Wempty_नियमः))
    warning_argmatch ("empty-rule", 0, 0);
  यदि (current_नियमः->percent_रिक्तानि_loc.start.file)
    duplicate_नियमः_directive ("%empty",
                              current_नियमः->percent_रिक्तानि_loc, loc);
  अन्यथा
    current_नियमः->percent_रिक्तानि_loc = loc;
}

/* Attach dynamic precedence DPREC to the current rule. */

शून्य
grammar_current_नियमः_dprec_समुच्चयः (पूर्णाङ्क dprec, location loc)
{
  यदि (! glr_parser)
    complain (&loc, Wother, _("%s affects only GLR parsers"),
              "%dprec");
  यदि (dprec <= 0)
    complain (&loc, complaint, _("%s must be followed by positive number"),
              "%dprec");
  अन्यथा यदि (current_नियमः->dprec != 0)
    duplicate_नियमः_directive ("%dprec",
                              current_नियमः->dprec_loc, loc);
  अन्यथा
    {
      current_नियमः->dprec = dprec;
      current_नियमः->dprec_loc = loc;
    }
}

/* Attach a merge function NAME with argument type TYPE to current
   rule. */

शून्य
grammar_current_नियमः_merge_समुच्चयः (uniqstr name, location loc)
{
  यदि (! glr_parser)
    complain (&loc, Wother, _("%s affects only GLR parsers"),
              "%merge");
  यदि (current_नियमः->merger != 0)
    duplicate_नियमः_directive ("%merge",
                              current_नियमः->merger_declaration_loc, loc);
  अन्यथा
    {
      current_नियमः->merger = get_merge_function (name);
      current_नियमः->merger_declaration_loc = loc;
    }
}

/* Attach SYM to the current rule.  If needed, move the previous
   action as a midrule action.  */

शून्य
grammar_current_नियमः_चिह्नम्_append (चिह्नम् *sym, location loc,
                                    named_ref *name)
{
  यदि (current_नियमः->action_props.संहिता)
    grammar_midrule_action ();
  चिह्नम्_list *p = grammar_चिह्नम्_append (sym, loc);
  यदि (name)
    नियोजय_named_ref (p, name);
  यदि (sym->content->स्थितिः == undeclared || sym->content->स्थितिः == प्रयुक्तम्)
    sym->content->स्थितिः = needed;
}

शून्य
grammar_current_नियमः_action_append (ध्रुव वर्ण *action, location loc,
                                    named_ref *name, uniqstr type)
{
  यदि (current_नियमः->action_props.संहिता)
    grammar_midrule_action ();
  यदि (type)
    complain (&loc, Wyacc,
              _("POSIX Yacc does not support typed midrule actions"));
  /* After all symbol declarations have been parsed, packgram invokes
     code_props_translate_code.  */
  संहिता_props_नियमः_action_init (&current_नियमः->action_props, action, loc,
                               current_नियमः,
                               name, type,
                               /* is_predicate */ false);
}

शून्य
grammar_current_नियमः_predicate_append (ध्रुव वर्ण *pred, location loc)
{
  यदि (current_नियमः->action_props.संहिता)
    grammar_midrule_action ();
  संहिता_props_नियमः_action_init (&current_नियमः->action_props, pred, loc,
                               current_नियमः,
                               रिक्त, रिक्त,
                               /* is_predicate */ true);
}

/* Set the expected number of shift/reduce (reduce/reduce) conflicts
 * for the current rule.  If a midrule is encountered later, the count
 * is transferred to it and reset in the current rule to -1. */

शून्य
grammar_current_नियमः_expect_sr (पूर्णाङ्क गणनम्, location loc)
{
  (शून्य) loc;
  current_नियमः->expected_sr_conflicts = गणनम्;
}

शून्य
grammar_current_नियमः_expect_rr (पूर्णाङ्क गणनम्, location loc)
{
  यदि (! glr_parser)
    complain (&loc, Wother, _("%s affects only GLR parsers"),
              "%expect-rr");
  अन्यथा
    current_नियमः->expected_rr_conflicts = गणनम्;
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

  नियमः_सङ्ख्या ruleno = 0;
  नियमाः = xnmalloc (nrules, परिमाण *नियमाः);

  प्रति (चिह्नम्_list *p = grammar; p; p = p->अग्र)
    {
      चिह्नम्_list *lhs = p;
      लेखः_merge_function_type (lhs->merger, lhs->content.sym,
                                  lhs->merger_declaration_loc);
      /* If the midrule's $$ is set or its $n is used, remove the '$' from the
         symbol name so that it's a user-defined symbol so that the default
         %destructor and %printer apply.  */
      यदि (lhs->midrule_parent_नियमः /* i.e., symbol_is_dummy (lhs->content.sym).  */
          && (lhs->action_props.is_मानम्_प्रयुक्तम्
              || (चिह्नम्_list_n_get (lhs->midrule_parent_नियमः,
                                     lhs->midrule_parent_rhs_सूचकांक)
                  ->action_props.is_मानम्_प्रयुक्तम्)))
        lhs->content.sym->चिह्नकम् += 1;

      grammar_नियमः_check_and_complete (lhs);

      नियमाः[ruleno].संहिता = ruleno;
      नियमाः[ruleno].सङ्ख्या = ruleno;
      नियमाः[ruleno].lhs = lhs->content.sym->content;
      नियमाः[ruleno].rhs = ritem + itemno;
      नियमाः[ruleno].prec = रिक्त;
      नियमाः[ruleno].dprec = lhs->dprec;
      नियमाः[ruleno].merger = lhs->merger;
      नियमाः[ruleno].precsym = रिक्त;
      नियमाः[ruleno].location = lhs->rhs_loc;
      नियमाः[ruleno].useful = true;
      नियमाः[ruleno].action = lhs->action_props.संहिता;
      नियमाः[ruleno].action_loc = lhs->action_props.location;
      नियमाः[ruleno].is_predicate = lhs->action_props.is_predicate;
      नियमाः[ruleno].expected_sr_conflicts = lhs->expected_sr_conflicts;
      नियमाः[ruleno].expected_rr_conflicts = lhs->expected_rr_conflicts;

      /* Traverse the rhs.  */
      {
        size_t नियमः_दीर्घता = 0;
        प्रति (p = lhs->अग्र; p->content.sym; p = p->अग्र)
          {
            ++नियमः_दीर्घता;

            /* Don't allow rule_length == INT_MAX, since that might
               cause confusion with strtol if INT_MAX == LONG_MAX.  */
            यदि (नियमः_दीर्घता == अधिकतम_पूर्णाङ्कः)
              complain (&नियमाः[ruleno].location, fatal, _("rule is too long"));

            /* item_number = symbol_number.
               But the former needs to contain more: negative rule numbers. */
            ritem[itemno++] =
              चिह्नम्_सङ्ख्या_as_item_सङ्ख्या (p->content.sym->content->सङ्ख्या);
            /* A rule gets by default the precedence and associativity
               of its last token.  */
            यदि (p->content.sym->content->class == token_sym && default_prec)
              नियमाः[ruleno].prec = p->content.sym->content;
          }
      }

      /* If this rule has a %prec,
         the specified symbol's precedence replaces the default.  */
      यदि (lhs->ruleprec)
        {
          नियमाः[ruleno].precsym = lhs->ruleprec->content;
          नियमाः[ruleno].prec = lhs->ruleprec->content;
        }

      /* An item ends by the rule number (negated).  */
      ritem[itemno++] = नियमः_सङ्ख्या_as_item_सङ्ख्या (ruleno);
      aver (itemno < ITEM_सङ्ख्या_MAX);
      ++ruleno;
      aver (ruleno < नियमः_सङ्ख्या_MAX);
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

  prepare_percent_define_front_अन्तः_variables ();

  यदि (complaint_स्थितिः  < स्थितिः_complaint)
    check_and_convert_grammar ();
}

स्थिर शून्य
prepare_percent_define_front_अन्तः_variables (शून्य)
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
    स्थिर वर्ण ध्रुव * ध्रुव मानानि[] =
      {
       "lr.type", "lr""(0)", "lalr", "ielr", "canonical-lr", रिक्त,
       "lr.default-reduction", "most", "consistent", "accepting", रिक्त,
       रिक्त
      };
    muscle_percent_define_check_मानानि (मानानि);
  }
}

/* Find the first LHS which is not a dummy.  */

स्थिर चिह्नम् *
find_start_चिह्नम् (शून्य)
{
  चिह्नम्_list *res = grammar;
  /* Skip all the possible dummy rules of the first rule.  */
  प्रति (; चिह्नम्_is_dummy (res->content.sym); res = res->अग्र)
    /* Skip the LHS, and then all the RHS of the dummy rule.  */
    प्रति (res = res->अग्र; res->content.sym; res = res->अग्र)
      अनुवर्त;
  प्रत्यागम res->content.sym;
}


/* Insert an initial rule, whose location is that of the first rule
   (not that of the start symbol):

   $accept: SWITCHING_TOKEN START $end.  */
स्थिर शून्य
create_start_नियमः (चिह्नम् *swtok, चिह्नम् *start)
{
  चिह्नम्_list *initial_नियमः = चिह्नम्_list_sym_new (acceptsymbol, रिक्तानि_loc);
  initial_नियमः->rhs_loc = grammar->rhs_loc;
  चिह्नम्_list *p = initial_नियमः;
  यदि (swtok)
    {
      // Cannot create the action now, as the symbols have not yet
      // been assigned their number (by symbol_pack), which we need to
      // know the type name.  So the action is created in
      // grammar_rule_check_and_complete, which is run after
      // symbol_pack.
      p->अग्र = चिह्नम्_list_sym_new (swtok, रिक्तानि_loc);
      p = p->अग्र;
    }
  p->अग्र = चिह्नम्_list_sym_new (start, रिक्तानि_loc);
  p = p->अग्र;
  p->अग्र = चिह्नम्_list_sym_new (eoftoken, रिक्तानि_loc);
  p = p->अग्र;
  p->अग्र = चिह्नम्_list_sym_new (रिक्त, रिक्तानि_loc);
  p = p->अग्र;
  p->अग्र = grammar;
  nrules += 1;
  nritems += 3 + !!swtok;
  grammar = initial_नियमः;
}

/* Fetch (or create) a token "YY_PARSE_foo" for start symbol "foo".

   We don't use the simple "YY_FOO" because (i) we might get clashes
   with some of our symbols (e.g., cast => YY_CAST), and (ii) upcasing
   introduces possible clashes between terminal FOO and nonterminal
   foo.  */
चिह्नम् *
switching_token (ध्रुव चिह्नम् *start)
{
  वर्ण buf[100];
  size_t len = परिमाण buf;
  वर्ण *name = asnprintf (buf, &len, "YY_PARSE_%s", चिह्नम्_id_get (start));
  यदि (!name)
    xalloc_die ();
  // Setting the location ensures deterministic symbol numbers.
  चिह्नम् *res = चिह्नम्_get (name, start->location);
  यदि (name != buf)
    मुक्त (name);
  चिह्नम्_class_समुच्चयः (res, token_sym, start->location, false);
  प्रत्यागम res;
}

/* Create the start rules in reverse order, since they are inserted at
   the top of the grammar.  That way the rules follow the order of
   declaration to %start.  */

स्थिर शून्य
create_गुणजः_start_नियमाः (चिह्नम्_list *start_syms)
{
  यदि (start_syms)
    {
      create_गुणजः_start_नियमाः (start_syms->अग्र);
      प्रतिज्ञा (start_syms->content_type == SYMLIST_चिह्नम्);
      चिह्नम् *start = start_syms->content.sym;
      चिह्नम् *swtok = switching_token (start);
      create_start_नियमः (swtok, start);
    }
}

/* For each start symbol "foo", create the rule "$accept: YY_FOO
   foo $end". */
स्थिर शून्य
create_start_नियमाः (शून्य)
{
  यदि (!start_symbols)
    {
      चिह्नम् *start = find_start_चिह्नम् ();
      start_symbols = चिह्नम्_list_sym_new (start, start->location);
    }

  ध्रुव bool several = start_symbols->अग्र;
  यदि (several)
    create_गुणजः_start_नियमाः (start_symbols);
  अन्यथा
    {
      चिह्नम् *start = start_symbols->content.sym;
      create_start_नियमः (रिक्त, start);
    }
}

स्थिर शून्य
check_start_symbols (शून्य)
{
  ध्रुव bool multistart = start_symbols && start_symbols->अग्र;
  // Sanity checks on the start symbols.
  प्रति (चिह्नम्_list *list = start_symbols; list; list = list->अग्र)
    {
      ध्रुव चिह्नम् *start = list->content.sym;
      यदि (start->content->class == unknown_sym)
        {
          complain (&start->location, complaint,
                    _("the start symbol %s is undefined"),
                    start->चिह्नकम्);
          // I claim this situation is unreachable.  This is caught
          // before, and we get "symbol 'foo' is used, but is not
          // defined as a token and has no rules".
          abort ();
        }
      // If your only start symbol is a token, you're weird.
      यदि (!multistart && start->content->class == token_sym)
        complain (&start->location, complaint,
                  _("the start symbol %s is a token"),
                  start->चिह्नकम्);
    }
  यदि (complaint_स्थितिः == स्थितिः_complaint)
    निर्गच्छ (EXIT_FAILURE);
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
      eoftoken = चिह्नम्_get ("YYEOF", रिक्तानि_loc);
      यदि (eoftoken->content->class == nterm_sym)
        complain (&eoftoken->location, complaint,
                  _("rule given for %s, which is a token"),
                  eoftoken->चिह्नकम्);
      eoftoken->content->class = token_sym;
      eoftoken->content->सङ्ख्या = 0;
      /* Value specified by POSIX.  */
      eoftoken->content->संहिता = 0;
      {
        चिह्नम् *alias = चिह्नम्_get ("$end", रिक्तानि_loc);
        चिह्नम्_class_समुच्चयः (alias, token_sym, रिक्तानि_loc, false);
        चिह्नम्_make_alias (eoftoken, alias, रिक्तानि_loc);
      }
    }

  /* Insert the initial rule(s).  */
  create_start_नियमाः ();

  /* Report any undefined symbols and consider them nonterminals.  */
  symbols_check_defined ();

  यदि (चिह्नम्_सङ्ख्या_अधिकतमम् - nnterms < ntokens)
    complain (रिक्त, fatal, "too many symbols in input grammar (limit is %d)",
              चिह्नम्_सङ्ख्या_अधिकतमम्);

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
  प्रति (चिह्नम्_list *sym = grammar; sym; sym = sym->अग्र)
    संहिता_props_translate_संहिता (&sym->action_props);

  /* Convert the grammar into the format described in gram.h.  */
  packgram ();

  /* The grammar as a symbol_list is no longer needed. */
  चिह्नम्_list_free (grammar);
}
