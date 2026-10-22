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

ساكن فراغ prepare_percent_define_front_پڇاڙي_variables (فراغ);
ساكن فراغ check_and_convert_grammar (فراغ);

ساكن نشان_list *grammar = عدم;
نشان_list *شروعات_symbols = عدم;
merger_list *ملايو_functions = عدم;

/* Was %union seen?  */
bool union_ڏٺل = false;

/* Should rules have a default precedence?  */
bool default_prec = true;


فراغ
grammar_شروعات_symbols_جوڙيو (نشان_list *syms)
{
  /* Report and ignore duplicates.  Append the others to START_SYMBOLS.  */
  نشان_list *آخري = نشان_list_آخري (شروعات_symbols);
  لكل (نشان_list *l = syms; l && l->content.sym; /* nothing */)
    {
      /* Is there a previous definition?  */
      نشان_list *پهريون = نشان_list_ڳوليو_نشان (شروعات_symbols, l->content.sym);
      إذا (پهريون)
        {
          duplicate_directive ("%start", پهريون->sym_loc, l->sym_loc);
          نشان_list *dupl = l;
          l = l->ايندڙ;
          dupl->ايندڙ = عدم;
          نشان_list_free (dupl);
        }
      وإلا
        {
          إذا (آخري)
            {
              آخري->ايندڙ = l;
              آخري = l;
            }
          وإلا
            {
              آخري = l;
              شروعات_symbols = آخري;
            }
          نشان_list *ايندڙ = l->ايندڙ;
          l->ايندڙ = عدم;
          l = ايندڙ;
        }
    }
}



/*------------------------------------------------------------------------.
| Return the merger index for a merging function named NAME.  Records the |
| function, if new, in MERGER_LIST.                                       |
`------------------------------------------------------------------------*/

ساكن صحيح
get_ملايو_function (uniqstr name)
{
  إذا (! glr_parser)
    أرجع 0;

  merger_list *syms;
  merger_list سر;
  صحيح n;

  سر.ايندڙ = ملايو_functions;
  لكل (syms = &سر, n = 1; syms->ايندڙ; syms = syms->ايندڙ, n += 1)
    إذا (UNIQSTR_EQ (name, syms->ايندڙ->name))
      اكسر;
  إذا (syms->ايندڙ == عدم)
    {
      syms->ايندڙ = xmalloc (حجم syms->ايندڙ[0]);
      syms->ايندڙ->name = uniqstr_new (name);
      /* After all symbol type declarations have been parsed, packgram invokes
         record_merge_function_type to set the type.  */
      syms->ايندڙ->sym = عدم;
      syms->ايندڙ->ايندڙ = عدم;
      ملايو_functions = سر.ايندڙ;
    }
  أرجع n;
}

/*-------------------------------------------------------------------.
| For the existing merging function with index MERGER, record that   |
| the result type is that of SYM, as required by the lhs (i.e., SYM) |
| of the rule whose %merge declaration is at DECLARATION_LOC.        |
`-------------------------------------------------------------------*/

ساكن فراغ
رڪارڊ_ملايو_function_type (صحيح merger, نشان *sym, location declaration_loc)
{
  إذا (merger <= 0)
    أرجع;

  uniqstr type
    = sym->content->type_name ? sym->content->type_name : uniqstr_new ("");

  merger_list *ملايو_function;
  صحيح merger_ڳوليو = 1;
  لكل (ملايو_function = ملايو_functions;
       ملايو_function != عدم && merger_ڳوليو != merger;
       ملايو_function = ملايو_function->ايندڙ)
    merger_ڳوليو += 1;
  aver (ملايو_function != عدم && merger_ڳوليو == merger);
  إذا (ملايو_function->sym && ملايو_function->sym->content->type_name)
    {
      إذا (!UNIQSTR_EQ (ملايو_function->sym->content->type_name, type))
        {
          complain (&declaration_loc, complaint,
                    _("result type clash on merge function %s: "
                      "<%s> != <%s>"),
                    quote (ملايو_function->name), type,
                    ملايو_function->sym->content->type_name);
          subcomplain (&ملايو_function->type_declaration_loc, complaint,
                       _("previous declaration"));
        }
    }
  وإلا
    {
      ملايو_function->sym = sym;
      ملايو_function->type_declaration_loc = declaration_loc;
    }
}

/*--------------------------------------.
| Free all merge-function definitions.  |
`--------------------------------------*/

فراغ
free_merger_functions (فراغ)
{
  merger_list *L0 = ملايو_functions;
  طالما (L0)
    {
      merger_list *L1 = L0->ايندڙ;
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
ساكن نشان_list *grammar_پڇاڙي = عدم;

/* Append SYM to the grammar.  */
ساكن نشان_list *
grammar_نشان_append (نشان *sym, location loc)
{
  نشان_list *p = نشان_list_sym_new (sym, loc);

  إذا (grammar_پڇاڙي)
    grammar_پڇاڙي->ايندڙ = p;
  وإلا
    grammar = p;

  grammar_پڇاڙي = p;

  /* A null SYM stands for an end of rule; it is not an actual
     part of it.  */
  إذا (sym)
    ++nritems;

  أرجع p;
}

ساكن فراغ
سونپيو_named_ref (نشان_list *p, named_ref *name)
{
  نشان *sym = p->content.sym;

  إذا (name->id == sym->ٽيگ)
    {
      complain (&name->loc, Wother,
                _("duplicated symbol name for %s ignored"),
                quote (sym->ٽيگ));
      named_ref_free (name);
    }
  وإلا
    p->named_ref = name;
}


/* The rule currently being defined, and the previous rule.
   CURRENT_RULE points to the first LHS of the current rule, while
   PREVIOUS_RULE_END points to the *end* of the previous rule (NULL).  */
ساكن نشان_list *current_قاعدو = عدم;
ساكن نشان_list *اڳيون_قاعدو_پڇاڙي = عدم;


/*----------------------------------------------.
| Create a new rule for LHS in to the GRAMMAR.  |
`----------------------------------------------*/

فراغ
grammar_current_قاعدو_begin (نشان *lhs, location loc,
                            named_ref *lhs_name)
{
  /* Start a new rule and record its lhs.  */
  ++nrules;
  اڳيون_قاعدو_پڇاڙي = grammar_پڇاڙي;

  current_قاعدو = grammar_نشان_append (lhs, loc);
  إذا (lhs_name)
    سونپيو_named_ref (current_قاعدو, named_ref_copy (lhs_name));

  /* Mark the rule's lhs as a nonterminal if not already so.  */
  إذا (lhs->content->class == unknown_sym || lhs->content->class == pct_type_sym)
    نشان_class_سيٽ (lhs, nterm_sym, خالي_جايون_loc, false);
  وإلا إذا (lhs->content->class == token_sym)
    complain (&loc, complaint, _("rule given for %s, which is a token"),
              lhs->ٽيگ);
}


/*----------------------------------------------------------------------.
| A symbol should be used if either:                                    |
|   1. It has a destructor.                                             |
|   2. The symbol is a midrule symbol (i.e., the generated LHS          |
|      replacing a midrule action) that was assigned to or used, as in  |
|      "exp: { $$ = 1; } { $$ = $1; }".                                 |
`----------------------------------------------------------------------*/

ساكن bool
نشان_should_be_استعمال_ٿيل (نشان_list ثابت *s, bool *midrule_warning)
{
  إذا (نشان_ڪوڊ_props_get (s->content.sym, destructor)->ڪوڊ)
    أرجع true;
  إذا ((s->midrule && s->midrule->action_props.is_قدر_استعمال_ٿيل)
      || (s->midrule_والد_قاعدو
          && (نشان_list_n_get (s->midrule_والد_قاعدو,
                                 s->midrule_والد_rhs_اشاريو)
              ->action_props.is_قدر_استعمال_ٿيل)))
    {
      *midrule_warning = true;
      أرجع true;
    }
  أرجع false;
}

/*-----------------------------------------------------------------.
| Check that the rule R is properly defined.  For instance, there  |
| should be no type clash on the default action.  Possibly install |
| the default action.                                              |
`-----------------------------------------------------------------*/

ساكن فراغ
grammar_قاعدو_check_and_complete (نشان_list *r)
{
  ثابت نشان *lhs = r->content.sym;
  ثابت نشان *پهريون_rhs = r->ايندڙ->content.sym;

  /* Type check.

     If there is an action, then there is nothing we can do: the user
     is allowed to shoot herself in the foot.

     Don't worry about the default action if $$ is untyped, since $$'s
     value can't be used.  */
  إذا (!r->action_props.ڪوڊ && lhs->content->type_name)
    {
      /* If $$ is being set in default way, report if any type mismatch.  */
      إذا (پهريون_rhs)
        {
          حرف ثابت *lhs_type = lhs->content->type_name;
          حرف ثابت *rhs_type =
            پهريون_rhs->content->type_name ? پهريون_rhs->content->type_name : "";
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
                  ڪوڊ_props_قاعدو_action_init (&r->action_props, "{ $$ = $1; }",
                                               r->rhs_loc, r,
                                               /* name */ عدم,
                                               /* type */ عدم,
                                               /* is_predicate */ false);
                  ڪوڊ_props_translate_ڪوڊ (&r->action_props);
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
  ثابت bool multistart = شروعات_symbols && شروعات_symbols->ايندڙ;
  إذا (multistart && lhs == acceptsymbol)
    {
      ثابت نشان *شروعات = r->ايندڙ->ايندڙ->content.sym;
      إذا (شروعات->content->type_name)
        obstack_printf (obstack_for_actions,
                        "{ ]b4_accept""([%s%d])[; }",
                        شروعات->content->class == nterm_sym ? "orig " : "",
                        شروعات->content->عدد);
      وإلا
        obstack_printf (obstack_for_actions,
                        "{ ]b4_accept[; }");
      ڪوڊ_props_قاعدو_action_init (&r->action_props,
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
      لكل (نشان_list ثابت *l = r; l && l->content.sym; l = l->ايندڙ, ++n)
        {
          bool midrule_warning = false;
          إذا (!l->action_props.is_قدر_استعمال_ٿيل
              && نشان_should_be_استعمال_ٿيل (l, &midrule_warning)
              /* The default action, $$ = $1, 'uses' both.  */
              && (r->action_props.ڪوڊ || (n != 0 && n != 1)))
            {
              warnings warn_flag = midrule_warning ? Wmidrule_قدرون : Wother;
              إذا (n)
                complain (&l->sym_loc, warn_flag, _("unused value: $%d"), n);
              وإلا
                complain (&l->rhs_loc, warn_flag, _("unset value: $$"));
            }
        }
    }

  /* Check that %empty => empty rule.  */
  إذا (r->percent_خالي_جايون_loc.شروعات.file
      && r->ايندڙ && r->ايندڙ->content.sym)
    {
      complain (&r->percent_خالي_جايون_loc, complaint,
                _("%%empty on non-empty rule"));
      fixits_رجسٽر (&r->percent_خالي_جايون_loc, "");
    }

  /* Check that empty rule => %empty.  */
  إذا (!(r->ايندڙ && r->ايندڙ->content.sym)
      && !r->midrule_والد_قاعدو
      && !r->percent_خالي_جايون_loc.شروعات.file
      && warning_is_enabled (Wempty_قاعدو))
    {
      complain (&r->rhs_loc, Wempty_قاعدو, _("empty rule without %%empty"));
      إذا (feature_flag & feature_caret)
        location_caret_suggestion (r->rhs_loc, "%empty", stderr);
      location loc = r->rhs_loc;
      loc.پڇاڙي = loc.شروعات;
      fixits_رجسٽر (&loc, " %empty ");
    }

  /* See comments in grammar_current_rule_prec_set for how POSIX
     mandates this complaint.  It's only for identifiers, so skip
     it for char literals and strings, which are always tokens.  */
  إذا (r->ruleprec
      && r->ruleprec->ٽيگ[0] != '\'' && r->ruleprec->ٽيگ[0] != '"'
      && r->ruleprec->content->حالت != declared
      && !r->ruleprec->content->prec)
    complain (&r->rhs_loc, Wother,
              _("token for %%prec is not defined: %s"), r->ruleprec->ٽيگ);

  /* Check that the (main) action was not typed.  */
  إذا (r->action_props.type)
    complain (&r->rhs_loc, Wother,
              _("only midrule actions can be typed: %s"), r->action_props.type);
}


/*-------------------------------------.
| End the currently being grown rule.  |
`-------------------------------------*/

فراغ
grammar_current_قاعدو_پڇاڙي (location loc)
{
  /* Put an empty link in the list to mark the end of this rule  */
  grammar_نشان_append (عدم, grammar_پڇاڙي->rhs_loc);
  current_قاعدو->rhs_loc = loc;
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
  location dummy_loc = current_قاعدو->action_props.location;
  نشان *dummy = dummy_نشان_get (dummy_loc);
  نشان_type_سيٽ (dummy,
                   current_قاعدو->action_props.type, current_قاعدو->action_props.location);
  نشان_list *midrule = نشان_list_sym_new (dummy, dummy_loc);

  /* Remember named_ref of previous action. */
  named_ref *action_name = current_قاعدو->action_props.named_ref;

  /* Make a new rule, whose body is empty, before the current one, so
     that the action just read can belong to it.  */
  ++nrules;
  ++nritems;
  /* Attach its location and actions to that of the DUMMY.  */
  midrule->rhs_loc = dummy_loc;
  ڪوڊ_props_قاعدو_action_init (&midrule->action_props,
                               current_قاعدو->action_props.ڪوڊ,
                               current_قاعدو->action_props.location,
                               midrule,
                               /* name_ref */ عدم,
                               /* type */ عدم,
                               current_قاعدو->action_props.is_predicate);
  ڪوڊ_props_none_init (&current_قاعدو->action_props);

  midrule->expected_sr_conflicts = current_قاعدو->expected_sr_conflicts;
  midrule->expected_rr_conflicts = current_قاعدو->expected_rr_conflicts;
  current_قاعدو->expected_sr_conflicts = -1;
  current_قاعدو->expected_rr_conflicts = -1;

  إذا (اڳيون_قاعدو_پڇاڙي)
    اڳيون_قاعدو_پڇاڙي->ايندڙ = midrule;
  وإلا
    grammar = midrule;

  /* End the dummy's rule.  */
  midrule->ايندڙ = نشان_list_sym_new (عدم, dummy_loc);
  midrule->ايندڙ->ايندڙ = current_قاعدو;

  اڳيون_قاعدو_پڇاڙي = midrule->ايندڙ;

  /* Insert the dummy nonterminal replacing the midrule action into
     the current rule.  Bind it to its dedicated rule.  */
  grammar_current_قاعدو_نشان_append (dummy, dummy_loc,
                                      action_name);
  grammar_پڇاڙي->midrule = midrule;
  midrule->midrule_والد_قاعدو = current_قاعدو;
  midrule->midrule_والد_rhs_اشاريو = نشان_list_ڊيگهائي (current_قاعدو->ايندڙ);
}

/* Set the precedence symbol of the current rule to PRECSYM. */

فراغ
grammar_current_قاعدو_prec_سيٽ (نشان *precsym, location loc)
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
  نشان_class_سيٽ (precsym, token_sym, loc, false);
  إذا (current_قاعدو->ruleprec)
    duplicate_قاعدو_directive ("%prec",
                              current_قاعدو->ruleprec->location, loc);
  وإلا
    current_قاعدو->ruleprec = precsym;
}

/* Set %empty for the current rule. */

فراغ
grammar_current_قاعدو_خالي_جايون_سيٽ (location loc)
{
  /* If %empty is used and -Wno-empty-rule is not, then enable
     -Wempty-rule.  */
  إذا (warning_is_unset (Wempty_قاعدو))
    warning_argmatch ("empty-rule", 0, 0);
  إذا (current_قاعدو->percent_خالي_جايون_loc.شروعات.file)
    duplicate_قاعدو_directive ("%empty",
                              current_قاعدو->percent_خالي_جايون_loc, loc);
  وإلا
    current_قاعدو->percent_خالي_جايون_loc = loc;
}

/* Attach dynamic precedence DPREC to the current rule. */

فراغ
grammar_current_قاعدو_dprec_سيٽ (صحيح dprec, location loc)
{
  إذا (! glr_parser)
    complain (&loc, Wother, _("%s affects only GLR parsers"),
              "%dprec");
  إذا (dprec <= 0)
    complain (&loc, complaint, _("%s must be followed by positive number"),
              "%dprec");
  وإلا إذا (current_قاعدو->dprec != 0)
    duplicate_قاعدو_directive ("%dprec",
                              current_قاعدو->dprec_loc, loc);
  وإلا
    {
      current_قاعدو->dprec = dprec;
      current_قاعدو->dprec_loc = loc;
    }
}

/* Attach a merge function NAME with argument type TYPE to current
   rule. */

فراغ
grammar_current_قاعدو_ملايو_سيٽ (uniqstr name, location loc)
{
  إذا (! glr_parser)
    complain (&loc, Wother, _("%s affects only GLR parsers"),
              "%merge");
  إذا (current_قاعدو->merger != 0)
    duplicate_قاعدو_directive ("%merge",
                              current_قاعدو->merger_declaration_loc, loc);
  وإلا
    {
      current_قاعدو->merger = get_ملايو_function (name);
      current_قاعدو->merger_declaration_loc = loc;
    }
}

/* Attach SYM to the current rule.  If needed, move the previous
   action as a midrule action.  */

فراغ
grammar_current_قاعدو_نشان_append (نشان *sym, location loc,
                                    named_ref *name)
{
  إذا (current_قاعدو->action_props.ڪوڊ)
    grammar_midrule_action ();
  نشان_list *p = grammar_نشان_append (sym, loc);
  إذا (name)
    سونپيو_named_ref (p, name);
  إذا (sym->content->حالت == undeclared || sym->content->حالت == استعمال_ٿيل)
    sym->content->حالت = needed;
}

فراغ
grammar_current_قاعدو_action_append (ثابت حرف *action, location loc,
                                    named_ref *name, uniqstr type)
{
  إذا (current_قاعدو->action_props.ڪوڊ)
    grammar_midrule_action ();
  إذا (type)
    complain (&loc, Wyacc,
              _("POSIX Yacc does not support typed midrule actions"));
  /* After all symbol declarations have been parsed, packgram invokes
     code_props_translate_code.  */
  ڪوڊ_props_قاعدو_action_init (&current_قاعدو->action_props, action, loc,
                               current_قاعدو,
                               name, type,
                               /* is_predicate */ false);
}

فراغ
grammar_current_قاعدو_predicate_append (ثابت حرف *pred, location loc)
{
  إذا (current_قاعدو->action_props.ڪوڊ)
    grammar_midrule_action ();
  ڪوڊ_props_قاعدو_action_init (&current_قاعدو->action_props, pred, loc,
                               current_قاعدو,
                               عدم, عدم,
                               /* is_predicate */ true);
}

/* Set the expected number of shift/reduce (reduce/reduce) conflicts
 * for the current rule.  If a midrule is encountered later, the count
 * is transferred to it and reset in the current rule to -1. */

فراغ
grammar_current_قاعدو_expect_sr (صحيح ڳڻپ, location loc)
{
  (فراغ) loc;
  current_قاعدو->expected_sr_conflicts = ڳڻپ;
}

فراغ
grammar_current_قاعدو_expect_rr (صحيح ڳڻپ, location loc)
{
  إذا (! glr_parser)
    complain (&loc, Wother, _("%s affects only GLR parsers"),
              "%expect-rr");
  وإلا
    current_قاعدو->expected_rr_conflicts = ڳڻپ;
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

  قاعدو_عدد ruleno = 0;
  قاعدا = xnmalloc (nrules, حجم *قاعدا);

  لكل (نشان_list *p = grammar; p; p = p->ايندڙ)
    {
      نشان_list *lhs = p;
      رڪارڊ_ملايو_function_type (lhs->merger, lhs->content.sym,
                                  lhs->merger_declaration_loc);
      /* If the midrule's $$ is set or its $n is used, remove the '$' from the
         symbol name so that it's a user-defined symbol so that the default
         %destructor and %printer apply.  */
      إذا (lhs->midrule_والد_قاعدو /* i.e., symbol_is_dummy (lhs->content.sym).  */
          && (lhs->action_props.is_قدر_استعمال_ٿيل
              || (نشان_list_n_get (lhs->midrule_والد_قاعدو,
                                     lhs->midrule_والد_rhs_اشاريو)
                  ->action_props.is_قدر_استعمال_ٿيل)))
        lhs->content.sym->ٽيگ += 1;

      grammar_قاعدو_check_and_complete (lhs);

      قاعدا[ruleno].ڪوڊ = ruleno;
      قاعدا[ruleno].عدد = ruleno;
      قاعدا[ruleno].lhs = lhs->content.sym->content;
      قاعدا[ruleno].rhs = ritem + itemno;
      قاعدا[ruleno].prec = عدم;
      قاعدا[ruleno].dprec = lhs->dprec;
      قاعدا[ruleno].merger = lhs->merger;
      قاعدا[ruleno].precsym = عدم;
      قاعدا[ruleno].location = lhs->rhs_loc;
      قاعدا[ruleno].useful = true;
      قاعدا[ruleno].action = lhs->action_props.ڪوڊ;
      قاعدا[ruleno].action_loc = lhs->action_props.location;
      قاعدا[ruleno].is_predicate = lhs->action_props.is_predicate;
      قاعدا[ruleno].expected_sr_conflicts = lhs->expected_sr_conflicts;
      قاعدا[ruleno].expected_rr_conflicts = lhs->expected_rr_conflicts;

      /* Traverse the rhs.  */
      {
        size_t قاعدو_ڊيگهائي = 0;
        لكل (p = lhs->ايندڙ; p->content.sym; p = p->ايندڙ)
          {
            ++قاعدو_ڊيگهائي;

            /* Don't allow rule_length == INT_MAX, since that might
               cause confusion with strtol if INT_MAX == LONG_MAX.  */
            إذا (قاعدو_ڊيگهائي == وڌ_کان_وڌ_عدد)
              complain (&قاعدا[ruleno].location, fatal, _("rule is too long"));

            /* item_number = symbol_number.
               But the former needs to contain more: negative rule numbers. */
            ritem[itemno++] =
              نشان_عدد_as_شي_عدد (p->content.sym->content->عدد);
            /* A rule gets by default the precedence and associativity
               of its last token.  */
            إذا (p->content.sym->content->class == token_sym && default_prec)
              قاعدا[ruleno].prec = p->content.sym->content;
          }
      }

      /* If this rule has a %prec,
         the specified symbol's precedence replaces the default.  */
      إذا (lhs->ruleprec)
        {
          قاعدا[ruleno].precsym = lhs->ruleprec->content;
          قاعدا[ruleno].prec = lhs->ruleprec->content;
        }

      /* An item ends by the rule number (negated).  */
      ritem[itemno++] = قاعدو_عدد_as_شي_عدد (ruleno);
      aver (itemno < شي_عدد_MAX);
      ++ruleno;
      aver (ruleno < قاعدو_عدد_MAX);
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

  prepare_percent_define_front_پڇاڙي_variables ();

  إذا (complaint_حالت  < حالت_complaint)
    check_and_convert_grammar ();
}

ساكن فراغ
prepare_percent_define_front_پڇاڙي_variables (فراغ)
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
    ساكن حرف ثابت * ثابت قدرون[] =
      {
       "lr.type", "lr""(0)", "lalr", "ielr", "canonical-lr", عدم,
       "lr.default-reduction", "most", "consistent", "accepting", عدم,
       عدم
      };
    muscle_percent_define_check_قدرون (قدرون);
  }
}

/* Find the first LHS which is not a dummy.  */

ساكن نشان *
ڳوليو_شروعات_نشان (فراغ)
{
  نشان_list *res = grammar;
  /* Skip all the possible dummy rules of the first rule.  */
  لكل (; نشان_is_dummy (res->content.sym); res = res->ايندڙ)
    /* Skip the LHS, and then all the RHS of the dummy rule.  */
    لكل (res = res->ايندڙ; res->content.sym; res = res->ايندڙ)
      استمر;
  أرجع res->content.sym;
}


/* Insert an initial rule, whose location is that of the first rule
   (not that of the start symbol):

   $accept: SWITCHING_TOKEN START $end.  */
ساكن فراغ
create_شروعات_قاعدو (نشان *swtok, نشان *شروعات)
{
  نشان_list *initial_قاعدو = نشان_list_sym_new (acceptsymbol, خالي_جايون_loc);
  initial_قاعدو->rhs_loc = grammar->rhs_loc;
  نشان_list *p = initial_قاعدو;
  إذا (swtok)
    {
      // Cannot create the action now, as the symbols have not yet
      // been assigned their number (by symbol_pack), which we need to
      // know the type name.  So the action is created in
      // grammar_rule_check_and_complete, which is run after
      // symbol_pack.
      p->ايندڙ = نشان_list_sym_new (swtok, خالي_جايون_loc);
      p = p->ايندڙ;
    }
  p->ايندڙ = نشان_list_sym_new (شروعات, خالي_جايون_loc);
  p = p->ايندڙ;
  p->ايندڙ = نشان_list_sym_new (eoftoken, خالي_جايون_loc);
  p = p->ايندڙ;
  p->ايندڙ = نشان_list_sym_new (عدم, خالي_جايون_loc);
  p = p->ايندڙ;
  p->ايندڙ = grammar;
  nrules += 1;
  nritems += 3 + !!swtok;
  grammar = initial_قاعدو;
}

/* Fetch (or create) a token "YY_PARSE_foo" for start symbol "foo".

   We don't use the simple "YY_FOO" because (i) we might get clashes
   with some of our symbols (e.g., cast => YY_CAST), and (ii) upcasing
   introduces possible clashes between terminal FOO and nonterminal
   foo.  */
نشان *
switching_token (ثابت نشان *شروعات)
{
  حرف buf[100];
  size_t len = حجم buf;
  حرف *name = asnprintf (buf, &len, "YY_PARSE_%s", نشان_id_get (شروعات));
  إذا (!name)
    xalloc_die ();
  // Setting the location ensures deterministic symbol numbers.
  نشان *res = نشان_get (name, شروعات->location);
  إذا (name != buf)
    حرر (name);
  نشان_class_سيٽ (res, token_sym, شروعات->location, false);
  أرجع res;
}

/* Create the start rules in reverse order, since they are inserted at
   the top of the grammar.  That way the rules follow the order of
   declaration to %start.  */

ساكن فراغ
create_گڻو_شروعات_قاعدا (نشان_list *شروعات_syms)
{
  إذا (شروعات_syms)
    {
      create_گڻو_شروعات_قاعدا (شروعات_syms->ايندڙ);
      أكد (شروعات_syms->content_type == SYMLIST_نشان);
      نشان *شروعات = شروعات_syms->content.sym;
      نشان *swtok = switching_token (شروعات);
      create_شروعات_قاعدو (swtok, شروعات);
    }
}

/* For each start symbol "foo", create the rule "$accept: YY_FOO
   foo $end". */
ساكن فراغ
create_شروعات_قاعدا (فراغ)
{
  إذا (!شروعات_symbols)
    {
      نشان *شروعات = ڳوليو_شروعات_نشان ();
      شروعات_symbols = نشان_list_sym_new (شروعات, شروعات->location);
    }

  ثابت bool several = شروعات_symbols->ايندڙ;
  إذا (several)
    create_گڻو_شروعات_قاعدا (شروعات_symbols);
  وإلا
    {
      نشان *شروعات = شروعات_symbols->content.sym;
      create_شروعات_قاعدو (عدم, شروعات);
    }
}

ساكن فراغ
check_شروعات_symbols (فراغ)
{
  ثابت bool multistart = شروعات_symbols && شروعات_symbols->ايندڙ;
  // Sanity checks on the start symbols.
  لكل (نشان_list *list = شروعات_symbols; list; list = list->ايندڙ)
    {
      ثابت نشان *شروعات = list->content.sym;
      إذا (شروعات->content->class == unknown_sym)
        {
          complain (&شروعات->location, complaint,
                    _("the start symbol %s is undefined"),
                    شروعات->ٽيگ);
          // I claim this situation is unreachable.  This is caught
          // before, and we get "symbol 'foo' is used, but is not
          // defined as a token and has no rules".
          abort ();
        }
      // If your only start symbol is a token, you're weird.
      إذا (!multistart && شروعات->content->class == token_sym)
        complain (&شروعات->location, complaint,
                  _("the start symbol %s is a token"),
                  شروعات->ٽيگ);
    }
  إذا (complaint_حالت == حالت_complaint)
    ٻاهر_نڪرو (EXIT_FAILURE);
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
      eoftoken = نشان_get ("YYEOF", خالي_جايون_loc);
      إذا (eoftoken->content->class == nterm_sym)
        complain (&eoftoken->location, complaint,
                  _("rule given for %s, which is a token"),
                  eoftoken->ٽيگ);
      eoftoken->content->class = token_sym;
      eoftoken->content->عدد = 0;
      /* Value specified by POSIX.  */
      eoftoken->content->ڪوڊ = 0;
      {
        نشان *alias = نشان_get ("$end", خالي_جايون_loc);
        نشان_class_سيٽ (alias, token_sym, خالي_جايون_loc, false);
        نشان_make_alias (eoftoken, alias, خالي_جايون_loc);
      }
    }

  /* Insert the initial rule(s).  */
  create_شروعات_قاعدا ();

  /* Report any undefined symbols and consider them nonterminals.  */
  symbols_check_defined ();

  إذا (نشان_عدد_وڌ_کان_وڌ - nnterms < ntokens)
    complain (عدم, fatal, "too many symbols in input grammar (limit is %d)",
              نشان_عدد_وڌ_کان_وڌ);

  nsyms = ntokens + nnterms;

  /* Assign the symbols their symbol numbers.  */
  symbols_pack ();

  check_شروعات_symbols ();

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
  لكل (نشان_list *sym = grammar; sym; sym = sym->ايندڙ)
    ڪوڊ_props_translate_ڪوڊ (&sym->action_props);

  /* Convert the grammar into the format described in gram.h.  */
  packgram ();

  /* The grammar as a symbol_list is no longer needed. */
  نشان_list_free (grammar);
}
