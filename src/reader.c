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

#समावेश <config.h>
#समावेश "system.h"

#समावेश <quote.h>
#समावेश <vasnprintf.h>

#समावेश "complain.h"
#समावेश "conflicts.h"
#समावेश "files.h"
#समावेश "fixits.h"
#समावेश "getargs.h"
#समावेश "gram.h"
#समावेश "muscle-tab.h"
#समावेश "reader.h"
#समावेश "symlist.h"
#समावेश "symtab.h"
#समावेश "scan-gram.h"
#समावेश "scan-code.h"

अटल व्योम prepare_percent_define_front_end_variables (व्योम);
अटल व्योम check_and_convert_grammar (व्योम);

अटल प्रतीक_list *grammar = शून्य;
प्रतीक_list *start_symbols = शून्य;
merger_list *merge_functions = शून्य;

/* Was %union seen?  */
bool union_seen = false;

/* Should rules have a default precedence?  */
bool default_prec = true;


व्योम
grammar_start_symbols_add (प्रतीक_list *syms)
{
  /* Report and ignore duplicates.  Append the others to START_SYMBOLS.  */
  प्रतीक_list *last = प्रतीक_list_last (start_symbols);
  क्रम (प्रतीक_list *l = syms; l && l->content.sym; /* nothing */)
    {
      /* Is there a previous definition?  */
      प्रतीक_list *first = प्रतीक_list_find_प्रतीक (start_symbols, l->content.sym);
      अगर (first)
        {
          duplicate_directive ("%start", first->sym_loc, l->sym_loc);
          प्रतीक_list *dupl = l;
          l = l->अग्र;
          dupl->अग्र = शून्य;
          प्रतीक_list_free (dupl);
        }
      अन्यथा
        {
          अगर (last)
            {
              last->अग्र = l;
              last = l;
            }
          अन्यथा
            {
              last = l;
              start_symbols = last;
            }
          प्रतीक_list *अग्र = l->अग्र;
          l->अग्र = शून्य;
          l = अग्र;
        }
    }
}



/*------------------------------------------------------------------------.
| Return the merger index for a merging function named NAME.  Records the |
| function, if new, in MERGER_LIST.                                       |
`------------------------------------------------------------------------*/

अटल पूर्णांक
get_merge_function (uniqstr name)
{
  अगर (! glr_parser)
    वापस 0;

  merger_list *syms;
  merger_list शीर्ष_3;
  पूर्णांक n;

  शीर्ष_3.अग्र = merge_functions;
  क्रम (syms = &शीर्ष_3, n = 1; syms->अग्र; syms = syms->अग्र, n += 1)
    अगर (UNIQSTR_EQ (name, syms->अग्र->name))
      अवरोध;
  अगर (syms->अग्र == शून्य)
    {
      syms->अग्र = xmalloc (माप syms->अग्र[0]);
      syms->अग्र->name = uniqstr_new (name);
      /* After all symbol type declarations have been parsed, packgram invokes
         record_merge_function_type to set the type.  */
      syms->अग्र->sym = शून्य;
      syms->अग्र->अग्र = शून्य;
      merge_functions = शीर्ष_3.अग्र;
    }
  वापस n;
}

/*-------------------------------------------------------------------.
| For the existing merging function with index MERGER, record that   |
| the result type is that of SYM, as required by the lhs (i.e., SYM) |
| of the rule whose %merge declaration is at DECLARATION_LOC.        |
`-------------------------------------------------------------------*/

अटल व्योम
record_merge_function_type (पूर्णांक merger, प्रतीक *sym, location declaration_loc)
{
  अगर (merger <= 0)
    वापस;

  uniqstr type
    = sym->content->type_name ? sym->content->type_name : uniqstr_new ("");

  merger_list *merge_function;
  पूर्णांक merger_find = 1;
  क्रम (merge_function = merge_functions;
       merge_function != शून्य && merger_find != merger;
       merge_function = merge_function->अग्र)
    merger_find += 1;
  aver (merge_function != शून्य && merger_find == merger);
  अगर (merge_function->sym && merge_function->sym->content->type_name)
    {
      अगर (!UNIQSTR_EQ (merge_function->sym->content->type_name, type))
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

व्योम
free_merger_functions (व्योम)
{
  merger_list *L0 = merge_functions;
  जबतक (L0)
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
अटल प्रतीक_list *grammar_end = शून्य;

/* Append SYM to the grammar.  */
अटल प्रतीक_list *
grammar_प्रतीक_append (प्रतीक *sym, location loc)
{
  प्रतीक_list *p = प्रतीक_list_sym_new (sym, loc);

  अगर (grammar_end)
    grammar_end->अग्र = p;
  अन्यथा
    grammar = p;

  grammar_end = p;

  /* A null SYM stands for an end of rule; it is not an actual
     part of it.  */
  अगर (sym)
    ++nritems;

  वापस p;
}

अटल व्योम
assign_named_ref (प्रतीक_list *p, named_ref *name)
{
  प्रतीक *sym = p->content.sym;

  अगर (name->id == sym->tag)
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
अटल प्रतीक_list *current_rule = शून्य;
अटल प्रतीक_list *previous_rule_end = शून्य;


/*----------------------------------------------.
| Create a new rule for LHS in to the GRAMMAR.  |
`----------------------------------------------*/

व्योम
grammar_current_rule_begin (प्रतीक *lhs, location loc,
                            named_ref *lhs_name)
{
  /* Start a new rule and record its lhs.  */
  ++nrules;
  previous_rule_end = grammar_end;

  current_rule = grammar_प्रतीक_append (lhs, loc);
  अगर (lhs_name)
    assign_named_ref (current_rule, named_ref_copy (lhs_name));

  /* Mark the rule's lhs as a nonterminal if not already so.  */
  अगर (lhs->content->class == unknown_sym || lhs->content->class == pct_type_sym)
    प्रतीक_class_set (lhs, nterm_sym, empty_loc, false);
  अन्यथा अगर (lhs->content->class == token_sym)
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

अटल bool
प्रतीक_should_be_used (प्रतीक_list स्थिर *s, bool *midrule_warning)
{
  अगर (प्रतीक_code_props_get (s->content.sym, destructor)->code)
    वापस true;
  अगर ((s->midrule && s->midrule->action_props.is_मूल्य_used)
      || (s->midrule_parent_rule
          && (प्रतीक_list_n_get (s->midrule_parent_rule,
                                 s->midrule_parent_rhs_सूचकांक)
              ->action_props.is_मूल्य_used)))
    {
      *midrule_warning = true;
      वापस true;
    }
  वापस false;
}

/*-----------------------------------------------------------------.
| Check that the rule R is properly defined.  For instance, there  |
| should be no type clash on the default action.  Possibly install |
| the default action.                                              |
`-----------------------------------------------------------------*/

अटल व्योम
grammar_rule_check_and_complete (प्रतीक_list *r)
{
  स्थिर प्रतीक *lhs = r->content.sym;
  स्थिर प्रतीक *first_rhs = r->अग्र->content.sym;

  /* Type check.

     If there is an action, then there is nothing we can do: the user
     is allowed to shoot herself in the foot.

     Don't worry about the default action if $$ is untyped, since $$'s
     value can't be used.  */
  अगर (!r->action_props.code && lhs->content->type_name)
    {
      /* If $$ is being set in default way, report if any type mismatch.  */
      अगर (first_rhs)
        {
          अक्षर स्थिर *lhs_type = lhs->content->type_name;
          अक्षर स्थिर *rhs_type =
            first_rhs->content->type_name ? first_rhs->content->type_name : "";
          अगर (!UNIQSTR_EQ (lhs_type, rhs_type))
            complain (&r->rhs_loc, Wother,
                      _("type clash on default action: <%s> != <%s>"),
                      lhs_type, rhs_type);
          अन्यथा
            {
              /* Install the default action only for C++.  */
              स्थिर bool is_cxx =
                STREQ (language->language, "c++")
                || (skeleton && (STREQ (skeleton, "glr.cc")
                                 || STREQ (skeleton, "glr2.cc")
                                 || STREQ (skeleton, "lalr1.cc")));
              अगर (is_cxx)
                {
                  code_props_rule_action_init (&r->action_props, "{ $$ = $1; }",
                                               r->rhs_loc, r,
                                               /* name */ शून्य,
                                               /* type */ शून्य,
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
  स्थिर bool multistart = start_symbols && start_symbols->अग्र;
  अगर (multistart && lhs == acceptsymbol)
    {
      स्थिर प्रतीक *start = r->अग्र->अग्र->content.sym;
      अगर (start->content->type_name)
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
                                   /* name */ शून्य,
                                   /* type */ शून्य,
                                   /* is_predicate */ false);
    }


  /* Check that symbol values that should be used are in fact used.
     Don't check the generated start rules.  It has no action, so some
     rhs symbols may appear unused, but the parsing algorithm ensures
     that %destructor's are invoked appropriately.  */
  अगर (lhs != acceptsymbol)
    {
      पूर्णांक n = 0;
      क्रम (प्रतीक_list स्थिर *l = r; l && l->content.sym; l = l->अग्र, ++n)
        {
          bool midrule_warning = false;
          अगर (!l->action_props.is_मूल्य_used
              && प्रतीक_should_be_used (l, &midrule_warning)
              /* The default action, $$ = $1, 'uses' both.  */
              && (r->action_props.code || (n != 0 && n != 1)))
            {
              warnings warn_flag = midrule_warning ? Wmidrule_मूल्यावली : Wother;
              अगर (n)
                complain (&l->sym_loc, warn_flag, _("unused value: $%d"), n);
              अन्यथा
                complain (&l->rhs_loc, warn_flag, _("unset value: $$"));
            }
        }
    }

  /* Check that %empty => empty rule.  */
  अगर (r->percent_empty_loc.start.file
      && r->अग्र && r->अग्र->content.sym)
    {
      complain (&r->percent_empty_loc, complaint,
                _("%%empty on non-empty rule"));
      fixits_register (&r->percent_empty_loc, "");
    }

  /* Check that empty rule => %empty.  */
  अगर (!(r->अग्र && r->अग्र->content.sym)
      && !r->midrule_parent_rule
      && !r->percent_empty_loc.start.file
      && warning_is_enabled (Wempty_rule))
    {
      complain (&r->rhs_loc, Wempty_rule, _("empty rule without %%empty"));
      अगर (feature_flag & feature_caret)
        location_caret_suggestion (r->rhs_loc, "%empty", मानक_त्रुटि);
      location loc = r->rhs_loc;
      loc.end = loc.start;
      fixits_register (&loc, " %empty ");
    }

  /* See comments in grammar_current_rule_prec_set for how POSIX
     mandates this complaint.  It's only for identifiers, so skip
     it for char literals and strings, which are always tokens.  */
  अगर (r->ruleprec
      && r->ruleprec->tag[0] != '\'' && r->ruleprec->tag[0] != '"'
      && r->ruleprec->content->status != declared
      && !r->ruleprec->content->prec)
    complain (&r->rhs_loc, Wother,
              _("token for %%prec is not defined: %s"), r->ruleprec->tag);

  /* Check that the (main) action was not typed.  */
  अगर (r->action_props.type)
    complain (&r->rhs_loc, Wother,
              _("only midrule actions can be typed: %s"), r->action_props.type);
}


/*-------------------------------------.
| End the currently being grown rule.  |
`-------------------------------------*/

व्योम
grammar_current_rule_end (location loc)
{
  /* Put an empty link in the list to mark the end of this rule  */
  grammar_प्रतीक_append (शून्य, grammar_end->rhs_loc);
  current_rule->rhs_loc = loc;
}


/*-------------------------------------------------------------------.
| The previous action turns out to be a midrule action.  Attach it   |
| to the current rule, i.e., create a dummy symbol, attach it this   |
| midrule action, and append this dummy nonterminal to the current   |
| rule.                                                              |
`-------------------------------------------------------------------*/

व्योम
grammar_midrule_action (व्योम)
{
  /* Since the action was written out with this rule's number, we must
     give the new rule this number by inserting the new rule before
     it.  */

  /* Make a DUMMY nonterminal, whose location is that of the midrule
     action.  Create the MIDRULE.  */
  location dummy_loc = current_rule->action_props.location;
  प्रतीक *dummy = dummy_प्रतीक_get (dummy_loc);
  प्रतीक_type_set (dummy,
                   current_rule->action_props.type, current_rule->action_props.location);
  प्रतीक_list *midrule = प्रतीक_list_sym_new (dummy, dummy_loc);

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
                               /* name_ref */ शून्य,
                               /* type */ शून्य,
                               current_rule->action_props.is_predicate);
  code_props_none_init (&current_rule->action_props);

  midrule->expected_sr_conflicts = current_rule->expected_sr_conflicts;
  midrule->expected_rr_conflicts = current_rule->expected_rr_conflicts;
  current_rule->expected_sr_conflicts = -1;
  current_rule->expected_rr_conflicts = -1;

  अगर (previous_rule_end)
    previous_rule_end->अग्र = midrule;
  अन्यथा
    grammar = midrule;

  /* End the dummy's rule.  */
  midrule->अग्र = प्रतीक_list_sym_new (शून्य, dummy_loc);
  midrule->अग्र->अग्र = current_rule;

  previous_rule_end = midrule->अग्र;

  /* Insert the dummy nonterminal replacing the midrule action into
     the current rule.  Bind it to its dedicated rule.  */
  grammar_current_rule_प्रतीक_append (dummy, dummy_loc,
                                      action_name);
  grammar_end->midrule = midrule;
  midrule->midrule_parent_rule = current_rule;
  midrule->midrule_parent_rhs_सूचकांक = प्रतीक_list_लंबाई (current_rule->अग्र);
}

/* Set the precedence symbol of the current rule to PRECSYM. */

व्योम
grammar_current_rule_prec_set (प्रतीक *precsym, location loc)
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
  प्रतीक_class_set (precsym, token_sym, loc, false);
  अगर (current_rule->ruleprec)
    duplicate_rule_directive ("%prec",
                              current_rule->ruleprec->location, loc);
  अन्यथा
    current_rule->ruleprec = precsym;
}

/* Set %empty for the current rule. */

व्योम
grammar_current_rule_empty_set (location loc)
{
  /* If %empty is used and -Wno-empty-rule is not, then enable
     -Wempty-rule.  */
  अगर (warning_is_unset (Wempty_rule))
    warning_argmatch ("empty-rule", 0, 0);
  अगर (current_rule->percent_empty_loc.start.file)
    duplicate_rule_directive ("%empty",
                              current_rule->percent_empty_loc, loc);
  अन्यथा
    current_rule->percent_empty_loc = loc;
}

/* Attach dynamic precedence DPREC to the current rule. */

व्योम
grammar_current_rule_dprec_set (पूर्णांक dprec, location loc)
{
  अगर (! glr_parser)
    complain (&loc, Wother, _("%s affects only GLR parsers"),
              "%dprec");
  अगर (dprec <= 0)
    complain (&loc, complaint, _("%s must be followed by positive number"),
              "%dprec");
  अन्यथा अगर (current_rule->dprec != 0)
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

व्योम
grammar_current_rule_merge_set (uniqstr name, location loc)
{
  अगर (! glr_parser)
    complain (&loc, Wother, _("%s affects only GLR parsers"),
              "%merge");
  अगर (current_rule->merger != 0)
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

व्योम
grammar_current_rule_प्रतीक_append (प्रतीक *sym, location loc,
                                    named_ref *name)
{
  अगर (current_rule->action_props.code)
    grammar_midrule_action ();
  प्रतीक_list *p = grammar_प्रतीक_append (sym, loc);
  अगर (name)
    assign_named_ref (p, name);
  अगर (sym->content->status == undeclared || sym->content->status == used)
    sym->content->status = needed;
}

व्योम
grammar_current_rule_action_append (स्थिर अक्षर *action, location loc,
                                    named_ref *name, uniqstr type)
{
  अगर (current_rule->action_props.code)
    grammar_midrule_action ();
  अगर (type)
    complain (&loc, Wyacc,
              _("POSIX Yacc does not support typed midrule actions"));
  /* After all symbol declarations have been parsed, packgram invokes
     code_props_translate_code.  */
  code_props_rule_action_init (&current_rule->action_props, action, loc,
                               current_rule,
                               name, type,
                               /* is_predicate */ false);
}

व्योम
grammar_current_rule_predicate_append (स्थिर अक्षर *pred, location loc)
{
  अगर (current_rule->action_props.code)
    grammar_midrule_action ();
  code_props_rule_action_init (&current_rule->action_props, pred, loc,
                               current_rule,
                               शून्य, शून्य,
                               /* is_predicate */ true);
}

/* Set the expected number of shift/reduce (reduce/reduce) conflicts
 * for the current rule.  If a midrule is encountered later, the count
 * is transferred to it and reset in the current rule to -1. */

व्योम
grammar_current_rule_expect_sr (पूर्णांक गिनती, location loc)
{
  (व्योम) loc;
  current_rule->expected_sr_conflicts = गिनती;
}

व्योम
grammar_current_rule_expect_rr (पूर्णांक गिनती, location loc)
{
  अगर (! glr_parser)
    complain (&loc, Wother, _("%s affects only GLR parsers"),
              "%expect-rr");
  अन्यथा
    current_rule->expected_rr_conflicts = गिनती;
}


/*---------------------------------------------.
| Build RULES and RITEM from what was parsed.  |
`---------------------------------------------*/

अटल व्योम
packgram (व्योम)
{
  पूर्णांक itemno = 0;
  ritem = xnmalloc (nritems + 1, माप *ritem);
  /* This sentinel is used by build_relations() in lalr.c.  */
  *ritem++ = 0;

  rule_संख्या ruleno = 0;
  rules = xnmalloc (nrules, माप *rules);

  क्रम (प्रतीक_list *p = grammar; p; p = p->अग्र)
    {
      प्रतीक_list *lhs = p;
      record_merge_function_type (lhs->merger, lhs->content.sym,
                                  lhs->merger_declaration_loc);
      /* If the midrule's $$ is set or its $n is used, remove the '$' from the
         symbol name so that it's a user-defined symbol so that the default
         %destructor and %printer apply.  */
      अगर (lhs->midrule_parent_rule /* i.e., symbol_is_dummy (lhs->content.sym).  */
          && (lhs->action_props.is_मूल्य_used
              || (प्रतीक_list_n_get (lhs->midrule_parent_rule,
                                     lhs->midrule_parent_rhs_सूचकांक)
                  ->action_props.is_मूल्य_used)))
        lhs->content.sym->tag += 1;

      grammar_rule_check_and_complete (lhs);

      rules[ruleno].code = ruleno;
      rules[ruleno].संख्या = ruleno;
      rules[ruleno].lhs = lhs->content.sym->content;
      rules[ruleno].rhs = ritem + itemno;
      rules[ruleno].prec = शून्य;
      rules[ruleno].dprec = lhs->dprec;
      rules[ruleno].merger = lhs->merger;
      rules[ruleno].precsym = शून्य;
      rules[ruleno].location = lhs->rhs_loc;
      rules[ruleno].useful = true;
      rules[ruleno].action = lhs->action_props.code;
      rules[ruleno].action_loc = lhs->action_props.location;
      rules[ruleno].is_predicate = lhs->action_props.is_predicate;
      rules[ruleno].expected_sr_conflicts = lhs->expected_sr_conflicts;
      rules[ruleno].expected_rr_conflicts = lhs->expected_rr_conflicts;

      /* Traverse the rhs.  */
      {
        माप_प्रकार rule_लंबाई = 0;
        क्रम (p = lhs->अग्र; p->content.sym; p = p->अग्र)
          {
            ++rule_लंबाई;

            /* Don't allow rule_length == INT_MAX, since that might
               cause confusion with strtol if INT_MAX == LONG_MAX.  */
            अगर (rule_लंबाई == पूर्णांक_उच्च)
              complain (&rules[ruleno].location, fatal, _("rule is too long"));

            /* item_number = symbol_number.
               But the former needs to contain more: negative rule numbers. */
            ritem[itemno++] =
              प्रतीक_संख्या_as_item_संख्या (p->content.sym->content->संख्या);
            /* A rule gets by default the precedence and associativity
               of its last token.  */
            अगर (p->content.sym->content->class == token_sym && default_prec)
              rules[ruleno].prec = p->content.sym->content;
          }
      }

      /* If this rule has a %prec,
         the specified symbol's precedence replaces the default.  */
      अगर (lhs->ruleprec)
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

  अगर (trace_flag & trace_sets)
    ritem_print (मानक_त्रुटि);
}


/*--------------------------------------------------------------.
| Read in the grammar specification and record it in the format |
| described in gram.h.                                          |
`--------------------------------------------------------------*/

व्योम
reader (स्थिर अक्षर *gram)
{
  /* Set up symbol_table, semantic_type_table, and the built-in
     symbols.  */
  symbols_new ();

  gram_scanner_open (gram);
  parser_init ();
  gram_parse ();
  gram_scanner_close ();

  prepare_percent_define_front_end_variables ();

  अगर (complaint_status  < status_complaint)
    check_and_convert_grammar ();
}

अटल व्योम
prepare_percent_define_front_end_variables (व्योम)
{
  /* Set %define front-end variable defaults.  */
  muscle_percent_define_default ("lr.keep-unreachable-state", "false");
  {
    /* IELR would be a better default, but LALR is historically the
       default.  */
    muscle_percent_define_default ("lr.type", "lalr");
    अक्षर *lr_type = muscle_percent_define_get ("lr.type");
    अगर (STRNEQ (lr_type, "canonical-lr"))
      muscle_percent_define_default ("lr.default-reduction", "most");
    अन्यथा
      muscle_percent_define_default ("lr.default-reduction", "accepting");
    मुक्त (lr_type);
  }
  muscle_percent_define_default ("tool.xsltproc", "xsltproc");

  /* Check %define front-end variables.  */
  {
    अटल अक्षर स्थिर * स्थिर मूल्यावली[] =
      {
       "lr.type", "lr""(0)", "lalr", "ielr", "canonical-lr", शून्य,
       "lr.default-reduction", "most", "consistent", "accepting", शून्य,
       शून्य
      };
    muscle_percent_define_check_मूल्यावली (मूल्यावली);
  }
}

/* Find the first LHS which is not a dummy.  */

अटल प्रतीक *
find_start_प्रतीक (व्योम)
{
  प्रतीक_list *res = grammar;
  /* Skip all the possible dummy rules of the first rule.  */
  क्रम (; प्रतीक_is_dummy (res->content.sym); res = res->अग्र)
    /* Skip the LHS, and then all the RHS of the dummy rule.  */
    क्रम (res = res->अग्र; res->content.sym; res = res->अग्र)
      जारी;
  वापस res->content.sym;
}


/* Insert an initial rule, whose location is that of the first rule
   (not that of the start symbol):

   $accept: SWITCHING_TOKEN START $end.  */
अटल व्योम
create_start_rule (प्रतीक *swtok, प्रतीक *start)
{
  प्रतीक_list *initial_rule = प्रतीक_list_sym_new (acceptsymbol, empty_loc);
  initial_rule->rhs_loc = grammar->rhs_loc;
  प्रतीक_list *p = initial_rule;
  अगर (swtok)
    {
      // Cannot create the action now, as the symbols have not yet
      // been assigned their number (by symbol_pack), which we need to
      // know the type name.  So the action is created in
      // grammar_rule_check_and_complete, which is run after
      // symbol_pack.
      p->अग्र = प्रतीक_list_sym_new (swtok, empty_loc);
      p = p->अग्र;
    }
  p->अग्र = प्रतीक_list_sym_new (start, empty_loc);
  p = p->अग्र;
  p->अग्र = प्रतीक_list_sym_new (eoftoken, empty_loc);
  p = p->अग्र;
  p->अग्र = प्रतीक_list_sym_new (शून्य, empty_loc);
  p = p->अग्र;
  p->अग्र = grammar;
  nrules += 1;
  nritems += 3 + !!swtok;
  grammar = initial_rule;
}

/* Fetch (or create) a token "YY_PARSE_foo" for start symbol "foo".

   We don't use the simple "YY_FOO" because (i) we might get clashes
   with some of our symbols (e.g., cast => YY_CAST), and (ii) upcasing
   introduces possible clashes between terminal FOO and nonterminal
   foo.  */
प्रतीक *
switching_token (स्थिर प्रतीक *start)
{
  अक्षर buf[100];
  माप_प्रकार len = माप buf;
  अक्षर *name = asnprintf (buf, &len, "YY_PARSE_%s", प्रतीक_id_get (start));
  अगर (!name)
    xalloc_die ();
  // Setting the location ensures deterministic symbol numbers.
  प्रतीक *res = प्रतीक_get (name, start->location);
  अगर (name != buf)
    मुक्त (name);
  प्रतीक_class_set (res, token_sym, start->location, false);
  वापस res;
}

/* Create the start rules in reverse order, since they are inserted at
   the top of the grammar.  That way the rules follow the order of
   declaration to %start.  */

अटल व्योम
create_multiple_start_rules (प्रतीक_list *start_syms)
{
  अगर (start_syms)
    {
      create_multiple_start_rules (start_syms->अग्र);
      निश्चित (start_syms->content_type == SYMLIST_प्रतीक);
      प्रतीक *start = start_syms->content.sym;
      प्रतीक *swtok = switching_token (start);
      create_start_rule (swtok, start);
    }
}

/* For each start symbol "foo", create the rule "$accept: YY_FOO
   foo $end". */
अटल व्योम
create_start_rules (व्योम)
{
  अगर (!start_symbols)
    {
      प्रतीक *start = find_start_प्रतीक ();
      start_symbols = प्रतीक_list_sym_new (start, start->location);
    }

  स्थिर bool several = start_symbols->अग्र;
  अगर (several)
    create_multiple_start_rules (start_symbols);
  अन्यथा
    {
      प्रतीक *start = start_symbols->content.sym;
      create_start_rule (शून्य, start);
    }
}

अटल व्योम
check_start_symbols (व्योम)
{
  स्थिर bool multistart = start_symbols && start_symbols->अग्र;
  // Sanity checks on the start symbols.
  क्रम (प्रतीक_list *list = start_symbols; list; list = list->अग्र)
    {
      स्थिर प्रतीक *start = list->content.sym;
      अगर (start->content->class == unknown_sym)
        {
          complain (&start->location, complaint,
                    _("the start symbol %s is undefined"),
                    start->tag);
          // I claim this situation is unreachable.  This is caught
          // before, and we get "symbol 'foo' is used, but is not
          // defined as a token and has no rules".
          पात ();
        }
      // If your only start symbol is a token, you're weird.
      अगर (!multistart && start->content->class == token_sym)
        complain (&start->location, complaint,
                  _("the start symbol %s is a token"),
                  start->tag);
    }
  अगर (complaint_status == status_complaint)
    निकास (निकास_त्रुटि);
}


/*-------------------------------------------------------------.
| Check the grammar that has just been read, and convert it to |
| internal form.                                               |
`-------------------------------------------------------------*/

अटल व्योम
check_and_convert_grammar (व्योम)
{
  /* Grammar has been read.  Do some checking.  */
  अगर (nrules == 0)
    complain (शून्य, fatal, _("no rules in the input grammar"));

  /* If the user did not define her EOFTOKEN, do it now. */
  अगर (!eoftoken)
    {
      eoftoken = प्रतीक_get ("YYEOF", empty_loc);
      अगर (eoftoken->content->class == nterm_sym)
        complain (&eoftoken->location, complaint,
                  _("rule given for %s, which is a token"),
                  eoftoken->tag);
      eoftoken->content->class = token_sym;
      eoftoken->content->संख्या = 0;
      /* Value specified by POSIX.  */
      eoftoken->content->code = 0;
      {
        प्रतीक *alias = प्रतीक_get ("$end", empty_loc);
        प्रतीक_class_set (alias, token_sym, empty_loc, false);
        प्रतीक_make_alias (eoftoken, alias, empty_loc);
      }
    }

  /* Insert the initial rule(s).  */
  create_start_rules ();

  /* Report any undefined symbols and consider them nonterminals.  */
  symbols_check_defined ();

  अगर (प्रतीक_संख्या_MAXIMUM - nnterms < ntokens)
    complain (शून्य, fatal, "too many symbols in input grammar (limit is %d)",
              प्रतीक_संख्या_MAXIMUM);

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
  क्रम (प्रतीक_list *sym = grammar; sym; sym = sym->अग्र)
    code_props_translate_code (&sym->action_props);

  /* Convert the grammar into the format described in gram.h.  */
  packgram ();

  /* The grammar as a symbol_list is no longer needed. */
  प्रतीक_list_free (grammar);
}
