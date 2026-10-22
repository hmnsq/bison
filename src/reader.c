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

ساكن فراغ prepare_percent_define_front_سرا_variables (فراغ);
ساكن فراغ check_and_convert_grammar (فراغ);

ساكن علامت_list *grammar = عدم;
علامت_list *شروع_symbols = عدم;
merger_list *ملاؤ_functions = عدم;

/* Was %union seen?  */
bool union_تکیا = false;

/* Should rules have a default precedence?  */
bool default_prec = true;


فراغ
grammar_شروع_symbols_جوڑو (علامت_list *syms)
{
  /* Report and ignore duplicates.  Append the others to START_SYMBOLS.  */
  علامت_list *آخری = علامت_list_آخری (شروع_symbols);
  لكل (علامت_list *l = syms; l && l->content.sym; /* nothing */)
    {
      /* Is there a previous definition?  */
      علامت_list *پہلا = علامت_list_لبھو_علامت (شروع_symbols, l->content.sym);
      إذا (پہلا)
        {
          duplicate_directive ("%start", پہلا->sym_loc, l->sym_loc);
          علامت_list *dupl = l;
          l = l->اگلا;
          dupl->اگلا = عدم;
          علامت_list_free (dupl);
        }
      وإلا
        {
          إذا (آخری)
            {
              آخری->اگلا = l;
              آخری = l;
            }
          وإلا
            {
              آخری = l;
              شروع_symbols = آخری;
            }
          علامت_list *اگلا = l->اگلا;
          l->اگلا = عدم;
          l = اگلا;
        }
    }
}



/*------------------------------------------------------------------------.
| Return the merger index for a merging function named NAME.  Records the |
| function, if new, in MERGER_LIST.                                       |
`------------------------------------------------------------------------*/

ساكن صحيح
get_ملاؤ_function (uniqstr name)
{
  إذا (! glr_parser)
    أرجع 0;

  merger_list *syms;
  merger_list سر;
  صحيح n;

  سر.اگلا = ملاؤ_functions;
  لكل (syms = &سر, n = 1; syms->اگلا; syms = syms->اگلا, n += 1)
    إذا (UNIQSTR_EQ (name, syms->اگلا->name))
      اكسر;
  إذا (syms->اگلا == عدم)
    {
      syms->اگلا = xmalloc (حجم syms->اگلا[0]);
      syms->اگلا->name = uniqstr_new (name);
      /* After all symbol type declarations have been parsed, packgram invokes
         record_merge_function_type to set the type.  */
      syms->اگلا->sym = عدم;
      syms->اگلا->اگلا = عدم;
      ملاؤ_functions = سر.اگلا;
    }
  أرجع n;
}

/*-------------------------------------------------------------------.
| For the existing merging function with index MERGER, record that   |
| the result type is that of SYM, as required by the lhs (i.e., SYM) |
| of the rule whose %merge declaration is at DECLARATION_LOC.        |
`-------------------------------------------------------------------*/

ساكن فراغ
ریکارڈ_ملاؤ_function_type (صحيح merger, علامت *sym, location declaration_loc)
{
  إذا (merger <= 0)
    أرجع;

  uniqstr type
    = sym->content->type_name ? sym->content->type_name : uniqstr_new ("");

  merger_list *ملاؤ_function;
  صحيح merger_لبھو = 1;
  لكل (ملاؤ_function = ملاؤ_functions;
       ملاؤ_function != عدم && merger_لبھو != merger;
       ملاؤ_function = ملاؤ_function->اگلا)
    merger_لبھو += 1;
  aver (ملاؤ_function != عدم && merger_لبھو == merger);
  إذا (ملاؤ_function->sym && ملاؤ_function->sym->content->type_name)
    {
      إذا (!UNIQSTR_EQ (ملاؤ_function->sym->content->type_name, type))
        {
          complain (&declaration_loc, complaint,
                    _("result type clash on merge function %s: "
                      "<%s> != <%s>"),
                    quote (ملاؤ_function->name), type,
                    ملاؤ_function->sym->content->type_name);
          subcomplain (&ملاؤ_function->type_declaration_loc, complaint,
                       _("previous declaration"));
        }
    }
  وإلا
    {
      ملاؤ_function->sym = sym;
      ملاؤ_function->type_declaration_loc = declaration_loc;
    }
}

/*--------------------------------------.
| Free all merge-function definitions.  |
`--------------------------------------*/

فراغ
free_merger_functions (فراغ)
{
  merger_list *L0 = ملاؤ_functions;
  طالما (L0)
    {
      merger_list *L1 = L0->اگلا;
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
ساكن علامت_list *grammar_سرا = عدم;

/* Append SYM to the grammar.  */
ساكن علامت_list *
grammar_علامت_append (علامت *sym, location loc)
{
  علامت_list *p = علامت_list_sym_new (sym, loc);

  إذا (grammar_سرا)
    grammar_سرا->اگلا = p;
  وإلا
    grammar = p;

  grammar_سرا = p;

  /* A null SYM stands for an end of rule; it is not an actual
     part of it.  */
  إذا (sym)
    ++nritems;

  أرجع p;
}

ساكن فراغ
سونپو_named_ref (علامت_list *p, named_ref *name)
{
  علامت *sym = p->content.sym;

  إذا (name->id == sym->ٹیگ)
    {
      complain (&name->loc, Wother,
                _("duplicated symbol name for %s ignored"),
                quote (sym->ٹیگ));
      named_ref_free (name);
    }
  وإلا
    p->named_ref = name;
}


/* The rule currently being defined, and the previous rule.
   CURRENT_RULE points to the first LHS of the current rule, while
   PREVIOUS_RULE_END points to the *end* of the previous rule (NULL).  */
ساكن علامت_list *current_قاعدہ = عدم;
ساكن علامت_list *پچھلا_قاعدہ_سرا = عدم;


/*----------------------------------------------.
| Create a new rule for LHS in to the GRAMMAR.  |
`----------------------------------------------*/

فراغ
grammar_current_قاعدہ_begin (علامت *lhs, location loc,
                            named_ref *lhs_name)
{
  /* Start a new rule and record its lhs.  */
  ++nrules;
  پچھلا_قاعدہ_سرا = grammar_سرا;

  current_قاعدہ = grammar_علامت_append (lhs, loc);
  إذا (lhs_name)
    سونپو_named_ref (current_قاعدہ, named_ref_copy (lhs_name));

  /* Mark the rule's lhs as a nonterminal if not already so.  */
  إذا (lhs->content->class == unknown_sym || lhs->content->class == pct_type_sym)
    علامت_class_سیٹ (lhs, nterm_sym, خالی_تھاواں_loc, false);
  وإلا إذا (lhs->content->class == token_sym)
    complain (&loc, complaint, _("rule given for %s, which is a token"),
              lhs->ٹیگ);
}


/*----------------------------------------------------------------------.
| A symbol should be used if either:                                    |
|   1. It has a destructor.                                             |
|   2. The symbol is a midrule symbol (i.e., the generated LHS          |
|      replacing a midrule action) that was assigned to or used, as in  |
|      "exp: { $$ = 1; } { $$ = $1; }".                                 |
`----------------------------------------------------------------------*/

ساكن bool
علامت_should_be_ورتیا (علامت_list ثابت *s, bool *midrule_warning)
{
  إذا (علامت_کوڈ_props_get (s->content.sym, destructor)->کوڈ)
    أرجع true;
  إذا ((s->midrule && s->midrule->action_props.is_قدر_ورتیا)
      || (s->midrule_ماپا_قاعدہ
          && (علامت_list_n_get (s->midrule_ماپا_قاعدہ,
                                 s->midrule_ماپا_rhs_اشاریہ)
              ->action_props.is_قدر_ورتیا)))
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
grammar_قاعدہ_check_and_complete (علامت_list *r)
{
  ثابت علامت *lhs = r->content.sym;
  ثابت علامت *پہلا_rhs = r->اگلا->content.sym;

  /* Type check.

     If there is an action, then there is nothing we can do: the user
     is allowed to shoot herself in the foot.

     Don't worry about the default action if $$ is untyped, since $$'s
     value can't be used.  */
  إذا (!r->action_props.کوڈ && lhs->content->type_name)
    {
      /* If $$ is being set in default way, report if any type mismatch.  */
      إذا (پہلا_rhs)
        {
          حرف ثابت *lhs_type = lhs->content->type_name;
          حرف ثابت *rhs_type =
            پہلا_rhs->content->type_name ? پہلا_rhs->content->type_name : "";
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
                  کوڈ_props_قاعدہ_action_init (&r->action_props, "{ $$ = $1; }",
                                               r->rhs_loc, r,
                                               /* name */ عدم,
                                               /* type */ عدم,
                                               /* is_predicate */ false);
                  کوڈ_props_translate_کوڈ (&r->action_props);
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
  ثابت bool multistart = شروع_symbols && شروع_symbols->اگلا;
  إذا (multistart && lhs == acceptsymbol)
    {
      ثابت علامت *شروع = r->اگلا->اگلا->content.sym;
      إذا (شروع->content->type_name)
        obstack_printf (obstack_for_actions,
                        "{ ]b4_accept""([%s%d])[; }",
                        شروع->content->class == nterm_sym ? "orig " : "",
                        شروع->content->نمبر);
      وإلا
        obstack_printf (obstack_for_actions,
                        "{ ]b4_accept[; }");
      کوڈ_props_قاعدہ_action_init (&r->action_props,
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
      لكل (علامت_list ثابت *l = r; l && l->content.sym; l = l->اگلا, ++n)
        {
          bool midrule_warning = false;
          إذا (!l->action_props.is_قدر_ورتیا
              && علامت_should_be_ورتیا (l, &midrule_warning)
              /* The default action, $$ = $1, 'uses' both.  */
              && (r->action_props.کوڈ || (n != 0 && n != 1)))
            {
              warnings warn_flag = midrule_warning ? Wmidrule_اقدار : Wother;
              إذا (n)
                complain (&l->sym_loc, warn_flag, _("unused value: $%d"), n);
              وإلا
                complain (&l->rhs_loc, warn_flag, _("unset value: $$"));
            }
        }
    }

  /* Check that %empty => empty rule.  */
  إذا (r->percent_خالی_تھاواں_loc.شروع.file
      && r->اگلا && r->اگلا->content.sym)
    {
      complain (&r->percent_خالی_تھاواں_loc, complaint,
                _("%%empty on non-empty rule"));
      fixits_رجسٹر (&r->percent_خالی_تھاواں_loc, "");
    }

  /* Check that empty rule => %empty.  */
  إذا (!(r->اگلا && r->اگلا->content.sym)
      && !r->midrule_ماپا_قاعدہ
      && !r->percent_خالی_تھاواں_loc.شروع.file
      && warning_is_enabled (Wempty_قاعدہ))
    {
      complain (&r->rhs_loc, Wempty_قاعدہ, _("empty rule without %%empty"));
      إذا (feature_flag & feature_caret)
        location_caret_suggestion (r->rhs_loc, "%empty", stderr);
      location loc = r->rhs_loc;
      loc.سرا = loc.شروع;
      fixits_رجسٹر (&loc, " %empty ");
    }

  /* See comments in grammar_current_rule_prec_set for how POSIX
     mandates this complaint.  It's only for identifiers, so skip
     it for char literals and strings, which are always tokens.  */
  إذا (r->ruleprec
      && r->ruleprec->ٹیگ[0] != '\'' && r->ruleprec->ٹیگ[0] != '"'
      && r->ruleprec->content->حالت != declared
      && !r->ruleprec->content->prec)
    complain (&r->rhs_loc, Wother,
              _("token for %%prec is not defined: %s"), r->ruleprec->ٹیگ);

  /* Check that the (main) action was not typed.  */
  إذا (r->action_props.type)
    complain (&r->rhs_loc, Wother,
              _("only midrule actions can be typed: %s"), r->action_props.type);
}


/*-------------------------------------.
| End the currently being grown rule.  |
`-------------------------------------*/

فراغ
grammar_current_قاعدہ_سرا (location loc)
{
  /* Put an empty link in the list to mark the end of this rule  */
  grammar_علامت_append (عدم, grammar_سرا->rhs_loc);
  current_قاعدہ->rhs_loc = loc;
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
  location dummy_loc = current_قاعدہ->action_props.location;
  علامت *dummy = dummy_علامت_get (dummy_loc);
  علامت_type_سیٹ (dummy,
                   current_قاعدہ->action_props.type, current_قاعدہ->action_props.location);
  علامت_list *midrule = علامت_list_sym_new (dummy, dummy_loc);

  /* Remember named_ref of previous action. */
  named_ref *action_name = current_قاعدہ->action_props.named_ref;

  /* Make a new rule, whose body is empty, before the current one, so
     that the action just read can belong to it.  */
  ++nrules;
  ++nritems;
  /* Attach its location and actions to that of the DUMMY.  */
  midrule->rhs_loc = dummy_loc;
  کوڈ_props_قاعدہ_action_init (&midrule->action_props,
                               current_قاعدہ->action_props.کوڈ,
                               current_قاعدہ->action_props.location,
                               midrule,
                               /* name_ref */ عدم,
                               /* type */ عدم,
                               current_قاعدہ->action_props.is_predicate);
  کوڈ_props_none_init (&current_قاعدہ->action_props);

  midrule->expected_sr_conflicts = current_قاعدہ->expected_sr_conflicts;
  midrule->expected_rr_conflicts = current_قاعدہ->expected_rr_conflicts;
  current_قاعدہ->expected_sr_conflicts = -1;
  current_قاعدہ->expected_rr_conflicts = -1;

  إذا (پچھلا_قاعدہ_سرا)
    پچھلا_قاعدہ_سرا->اگلا = midrule;
  وإلا
    grammar = midrule;

  /* End the dummy's rule.  */
  midrule->اگلا = علامت_list_sym_new (عدم, dummy_loc);
  midrule->اگلا->اگلا = current_قاعدہ;

  پچھلا_قاعدہ_سرا = midrule->اگلا;

  /* Insert the dummy nonterminal replacing the midrule action into
     the current rule.  Bind it to its dedicated rule.  */
  grammar_current_قاعدہ_علامت_append (dummy, dummy_loc,
                                      action_name);
  grammar_سرا->midrule = midrule;
  midrule->midrule_ماپا_قاعدہ = current_قاعدہ;
  midrule->midrule_ماپا_rhs_اشاریہ = علامت_list_لمائی (current_قاعدہ->اگلا);
}

/* Set the precedence symbol of the current rule to PRECSYM. */

فراغ
grammar_current_قاعدہ_prec_سیٹ (علامت *precsym, location loc)
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
  علامت_class_سیٹ (precsym, token_sym, loc, false);
  إذا (current_قاعدہ->ruleprec)
    duplicate_قاعدہ_directive ("%prec",
                              current_قاعدہ->ruleprec->location, loc);
  وإلا
    current_قاعدہ->ruleprec = precsym;
}

/* Set %empty for the current rule. */

فراغ
grammar_current_قاعدہ_خالی_تھاواں_سیٹ (location loc)
{
  /* If %empty is used and -Wno-empty-rule is not, then enable
     -Wempty-rule.  */
  إذا (warning_is_unset (Wempty_قاعدہ))
    warning_argmatch ("empty-rule", 0, 0);
  إذا (current_قاعدہ->percent_خالی_تھاواں_loc.شروع.file)
    duplicate_قاعدہ_directive ("%empty",
                              current_قاعدہ->percent_خالی_تھاواں_loc, loc);
  وإلا
    current_قاعدہ->percent_خالی_تھاواں_loc = loc;
}

/* Attach dynamic precedence DPREC to the current rule. */

فراغ
grammar_current_قاعدہ_dprec_سیٹ (صحيح dprec, location loc)
{
  إذا (! glr_parser)
    complain (&loc, Wother, _("%s affects only GLR parsers"),
              "%dprec");
  إذا (dprec <= 0)
    complain (&loc, complaint, _("%s must be followed by positive number"),
              "%dprec");
  وإلا إذا (current_قاعدہ->dprec != 0)
    duplicate_قاعدہ_directive ("%dprec",
                              current_قاعدہ->dprec_loc, loc);
  وإلا
    {
      current_قاعدہ->dprec = dprec;
      current_قاعدہ->dprec_loc = loc;
    }
}

/* Attach a merge function NAME with argument type TYPE to current
   rule. */

فراغ
grammar_current_قاعدہ_ملاؤ_سیٹ (uniqstr name, location loc)
{
  إذا (! glr_parser)
    complain (&loc, Wother, _("%s affects only GLR parsers"),
              "%merge");
  إذا (current_قاعدہ->merger != 0)
    duplicate_قاعدہ_directive ("%merge",
                              current_قاعدہ->merger_declaration_loc, loc);
  وإلا
    {
      current_قاعدہ->merger = get_ملاؤ_function (name);
      current_قاعدہ->merger_declaration_loc = loc;
    }
}

/* Attach SYM to the current rule.  If needed, move the previous
   action as a midrule action.  */

فراغ
grammar_current_قاعدہ_علامت_append (علامت *sym, location loc,
                                    named_ref *name)
{
  إذا (current_قاعدہ->action_props.کوڈ)
    grammar_midrule_action ();
  علامت_list *p = grammar_علامت_append (sym, loc);
  إذا (name)
    سونپو_named_ref (p, name);
  إذا (sym->content->حالت == undeclared || sym->content->حالت == ورتیا)
    sym->content->حالت = needed;
}

فراغ
grammar_current_قاعدہ_action_append (ثابت حرف *action, location loc,
                                    named_ref *name, uniqstr type)
{
  إذا (current_قاعدہ->action_props.کوڈ)
    grammar_midrule_action ();
  إذا (type)
    complain (&loc, Wyacc,
              _("POSIX Yacc does not support typed midrule actions"));
  /* After all symbol declarations have been parsed, packgram invokes
     code_props_translate_code.  */
  کوڈ_props_قاعدہ_action_init (&current_قاعدہ->action_props, action, loc,
                               current_قاعدہ,
                               name, type,
                               /* is_predicate */ false);
}

فراغ
grammar_current_قاعدہ_predicate_append (ثابت حرف *pred, location loc)
{
  إذا (current_قاعدہ->action_props.کوڈ)
    grammar_midrule_action ();
  کوڈ_props_قاعدہ_action_init (&current_قاعدہ->action_props, pred, loc,
                               current_قاعدہ,
                               عدم, عدم,
                               /* is_predicate */ true);
}

/* Set the expected number of shift/reduce (reduce/reduce) conflicts
 * for the current rule.  If a midrule is encountered later, the count
 * is transferred to it and reset in the current rule to -1. */

فراغ
grammar_current_قاعدہ_expect_sr (صحيح گنتی, location loc)
{
  (فراغ) loc;
  current_قاعدہ->expected_sr_conflicts = گنتی;
}

فراغ
grammar_current_قاعدہ_expect_rr (صحيح گنتی, location loc)
{
  إذا (! glr_parser)
    complain (&loc, Wother, _("%s affects only GLR parsers"),
              "%expect-rr");
  وإلا
    current_قاعدہ->expected_rr_conflicts = گنتی;
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

  قاعدہ_نمبر ruleno = 0;
  قاعدے = xnmalloc (nrules, حجم *قاعدے);

  لكل (علامت_list *p = grammar; p; p = p->اگلا)
    {
      علامت_list *lhs = p;
      ریکارڈ_ملاؤ_function_type (lhs->merger, lhs->content.sym,
                                  lhs->merger_declaration_loc);
      /* If the midrule's $$ is set or its $n is used, remove the '$' from the
         symbol name so that it's a user-defined symbol so that the default
         %destructor and %printer apply.  */
      إذا (lhs->midrule_ماپا_قاعدہ /* i.e., symbol_is_dummy (lhs->content.sym).  */
          && (lhs->action_props.is_قدر_ورتیا
              || (علامت_list_n_get (lhs->midrule_ماپا_قاعدہ,
                                     lhs->midrule_ماپا_rhs_اشاریہ)
                  ->action_props.is_قدر_ورتیا)))
        lhs->content.sym->ٹیگ += 1;

      grammar_قاعدہ_check_and_complete (lhs);

      قاعدے[ruleno].کوڈ = ruleno;
      قاعدے[ruleno].نمبر = ruleno;
      قاعدے[ruleno].lhs = lhs->content.sym->content;
      قاعدے[ruleno].rhs = ritem + itemno;
      قاعدے[ruleno].prec = عدم;
      قاعدے[ruleno].dprec = lhs->dprec;
      قاعدے[ruleno].merger = lhs->merger;
      قاعدے[ruleno].precsym = عدم;
      قاعدے[ruleno].location = lhs->rhs_loc;
      قاعدے[ruleno].useful = true;
      قاعدے[ruleno].action = lhs->action_props.کوڈ;
      قاعدے[ruleno].action_loc = lhs->action_props.location;
      قاعدے[ruleno].is_predicate = lhs->action_props.is_predicate;
      قاعدے[ruleno].expected_sr_conflicts = lhs->expected_sr_conflicts;
      قاعدے[ruleno].expected_rr_conflicts = lhs->expected_rr_conflicts;

      /* Traverse the rhs.  */
      {
        size_t قاعدہ_لمائی = 0;
        لكل (p = lhs->اگلا; p->content.sym; p = p->اگلا)
          {
            ++قاعدہ_لمائی;

            /* Don't allow rule_length == INT_MAX, since that might
               cause confusion with strtol if INT_MAX == LONG_MAX.  */
            إذا (قاعدہ_لمائی == ودھ_توں_ودھ_عدد)
              complain (&قاعدے[ruleno].location, fatal, _("rule is too long"));

            /* item_number = symbol_number.
               But the former needs to contain more: negative rule numbers. */
            ritem[itemno++] =
              علامت_نمبر_as_شے_نمبر (p->content.sym->content->نمبر);
            /* A rule gets by default the precedence and associativity
               of its last token.  */
            إذا (p->content.sym->content->class == token_sym && default_prec)
              قاعدے[ruleno].prec = p->content.sym->content;
          }
      }

      /* If this rule has a %prec,
         the specified symbol's precedence replaces the default.  */
      إذا (lhs->ruleprec)
        {
          قاعدے[ruleno].precsym = lhs->ruleprec->content;
          قاعدے[ruleno].prec = lhs->ruleprec->content;
        }

      /* An item ends by the rule number (negated).  */
      ritem[itemno++] = قاعدہ_نمبر_as_شے_نمبر (ruleno);
      aver (itemno < شے_نمبر_MAX);
      ++ruleno;
      aver (ruleno < قاعدہ_نمبر_MAX);
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

  prepare_percent_define_front_سرا_variables ();

  إذا (complaint_حالت  < حالت_complaint)
    check_and_convert_grammar ();
}

ساكن فراغ
prepare_percent_define_front_سرا_variables (فراغ)
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
    ساكن حرف ثابت * ثابت اقدار[] =
      {
       "lr.type", "lr""(0)", "lalr", "ielr", "canonical-lr", عدم,
       "lr.default-reduction", "most", "consistent", "accepting", عدم,
       عدم
      };
    muscle_percent_define_check_اقدار (اقدار);
  }
}

/* Find the first LHS which is not a dummy.  */

ساكن علامت *
لبھو_شروع_علامت (فراغ)
{
  علامت_list *res = grammar;
  /* Skip all the possible dummy rules of the first rule.  */
  لكل (; علامت_is_dummy (res->content.sym); res = res->اگلا)
    /* Skip the LHS, and then all the RHS of the dummy rule.  */
    لكل (res = res->اگلا; res->content.sym; res = res->اگلا)
      استمر;
  أرجع res->content.sym;
}


/* Insert an initial rule, whose location is that of the first rule
   (not that of the start symbol):

   $accept: SWITCHING_TOKEN START $end.  */
ساكن فراغ
create_شروع_قاعدہ (علامت *swtok, علامت *شروع)
{
  علامت_list *initial_قاعدہ = علامت_list_sym_new (acceptsymbol, خالی_تھاواں_loc);
  initial_قاعدہ->rhs_loc = grammar->rhs_loc;
  علامت_list *p = initial_قاعدہ;
  إذا (swtok)
    {
      // Cannot create the action now, as the symbols have not yet
      // been assigned their number (by symbol_pack), which we need to
      // know the type name.  So the action is created in
      // grammar_rule_check_and_complete, which is run after
      // symbol_pack.
      p->اگلا = علامت_list_sym_new (swtok, خالی_تھاواں_loc);
      p = p->اگلا;
    }
  p->اگلا = علامت_list_sym_new (شروع, خالی_تھاواں_loc);
  p = p->اگلا;
  p->اگلا = علامت_list_sym_new (eoftoken, خالی_تھاواں_loc);
  p = p->اگلا;
  p->اگلا = علامت_list_sym_new (عدم, خالی_تھاواں_loc);
  p = p->اگلا;
  p->اگلا = grammar;
  nrules += 1;
  nritems += 3 + !!swtok;
  grammar = initial_قاعدہ;
}

/* Fetch (or create) a token "YY_PARSE_foo" for start symbol "foo".

   We don't use the simple "YY_FOO" because (i) we might get clashes
   with some of our symbols (e.g., cast => YY_CAST), and (ii) upcasing
   introduces possible clashes between terminal FOO and nonterminal
   foo.  */
علامت *
switching_token (ثابت علامت *شروع)
{
  حرف buf[100];
  size_t len = حجم buf;
  حرف *name = asnprintf (buf, &len, "YY_PARSE_%s", علامت_id_get (شروع));
  إذا (!name)
    xalloc_die ();
  // Setting the location ensures deterministic symbol numbers.
  علامت *res = علامت_get (name, شروع->location);
  إذا (name != buf)
    حرر (name);
  علامت_class_سیٹ (res, token_sym, شروع->location, false);
  أرجع res;
}

/* Create the start rules in reverse order, since they are inserted at
   the top of the grammar.  That way the rules follow the order of
   declaration to %start.  */

ساكن فراغ
create_مضرب_شروع_قاعدے (علامت_list *شروع_syms)
{
  إذا (شروع_syms)
    {
      create_مضرب_شروع_قاعدے (شروع_syms->اگلا);
      أكد (شروع_syms->content_type == SYMLIST_علامت);
      علامت *شروع = شروع_syms->content.sym;
      علامت *swtok = switching_token (شروع);
      create_شروع_قاعدہ (swtok, شروع);
    }
}

/* For each start symbol "foo", create the rule "$accept: YY_FOO
   foo $end". */
ساكن فراغ
create_شروع_قاعدے (فراغ)
{
  إذا (!شروع_symbols)
    {
      علامت *شروع = لبھو_شروع_علامت ();
      شروع_symbols = علامت_list_sym_new (شروع, شروع->location);
    }

  ثابت bool several = شروع_symbols->اگلا;
  إذا (several)
    create_مضرب_شروع_قاعدے (شروع_symbols);
  وإلا
    {
      علامت *شروع = شروع_symbols->content.sym;
      create_شروع_قاعدہ (عدم, شروع);
    }
}

ساكن فراغ
check_شروع_symbols (فراغ)
{
  ثابت bool multistart = شروع_symbols && شروع_symbols->اگلا;
  // Sanity checks on the start symbols.
  لكل (علامت_list *list = شروع_symbols; list; list = list->اگلا)
    {
      ثابت علامت *شروع = list->content.sym;
      إذا (شروع->content->class == unknown_sym)
        {
          complain (&شروع->location, complaint,
                    _("the start symbol %s is undefined"),
                    شروع->ٹیگ);
          // I claim this situation is unreachable.  This is caught
          // before, and we get "symbol 'foo' is used, but is not
          // defined as a token and has no rules".
          abort ();
        }
      // If your only start symbol is a token, you're weird.
      إذا (!multistart && شروع->content->class == token_sym)
        complain (&شروع->location, complaint,
                  _("the start symbol %s is a token"),
                  شروع->ٹیگ);
    }
  إذا (complaint_حالت == حالت_complaint)
    باہر_نکلو (EXIT_FAILURE);
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
      eoftoken = علامت_get ("YYEOF", خالی_تھاواں_loc);
      إذا (eoftoken->content->class == nterm_sym)
        complain (&eoftoken->location, complaint,
                  _("rule given for %s, which is a token"),
                  eoftoken->ٹیگ);
      eoftoken->content->class = token_sym;
      eoftoken->content->نمبر = 0;
      /* Value specified by POSIX.  */
      eoftoken->content->کوڈ = 0;
      {
        علامت *alias = علامت_get ("$end", خالی_تھاواں_loc);
        علامت_class_سیٹ (alias, token_sym, خالی_تھاواں_loc, false);
        علامت_make_alias (eoftoken, alias, خالی_تھاواں_loc);
      }
    }

  /* Insert the initial rule(s).  */
  create_شروع_قاعدے ();

  /* Report any undefined symbols and consider them nonterminals.  */
  symbols_check_defined ();

  إذا (علامت_نمبر_ودھ_توں_ودھ - nnterms < ntokens)
    complain (عدم, fatal, "too many symbols in input grammar (limit is %d)",
              علامت_نمبر_ودھ_توں_ودھ);

  nsyms = ntokens + nnterms;

  /* Assign the symbols their symbol numbers.  */
  symbols_pack ();

  check_شروع_symbols ();

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
  لكل (علامت_list *sym = grammar; sym; sym = sym->اگلا)
    کوڈ_props_translate_کوڈ (&sym->action_props);

  /* Convert the grammar into the format described in gram.h.  */
  packgram ();

  /* The grammar as a symbol_list is no longer needed. */
  علامت_list_free (grammar);
}
