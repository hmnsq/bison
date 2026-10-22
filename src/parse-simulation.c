/* Parser simulator for unifying counterexample search

   Copyright (C) 2020-2022 Free Software Foundation, Inc.

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

#समावेश "parse-simulation.h"

#समावेश <gl_linked_list.h>
#समावेश <gl_xlist.h>
#समावेश <stdlib.h>

#समावेश "lssi.h"
#समावेश "nullable.h"

काष्ठा parse_state
{
  // Path of state-items the parser has traversed.
  काष्ठा si_chunk
  {
    // Elements newly added in this chunk.
    state_item_list contents;
    // Properties of the linked list this chunk represents.
    स्थिर state_item *शीर्ष_elt;
    स्थिर state_item *पुच्छ_elt;
    माप_प्रकार योग_आकार;
  } state_वस्तुएँ;
  // List of derivations of the symbols.
  काष्ठा deriv_chunk
  {
    derivation_list contents;
    स्थिर derivation *शीर्ष_elt;
    स्थिर derivation *पुच्छ_elt;
    माप_प्रकार योग_आकार;
  } derivs;
  काष्ठा parse_state *parent;
  पूर्णांक reference_गिनती;
  // Incremented during productions, decremented during reductions.
  पूर्णांक depth;
  // Whether the contents of the chunks should be prepended or
  // appended to the list the chunks represent.
  bool prepend;
  // Causes chunk contents to be freed when the reference count is
  // one. Used when only the chunk metadata will be needed.
  bool free_contents_early;
};


अटल व्योम
ps_si_prepend (parse_state *ps, स्थिर state_item *si)
{
  काष्ठा si_chunk *sic = &ps->state_वस्तुएँ;
  gl_list_add_first (sic->contents, si);
  sic->शीर्ष_elt = si;
  ++sic->योग_आकार;
  अगर (!sic->पुच्छ_elt)
    sic->पुच्छ_elt = si;
}

अटल व्योम
ps_si_append (parse_state *ps, स्थिर state_item *si)
{
  काष्ठा si_chunk *sic = &ps->state_वस्तुएँ;
  gl_list_add_last (sic->contents, si);
  sic->पुच्छ_elt = si;
  ++sic->योग_आकार;
  अगर (!sic->शीर्ष_elt)
    sic->शीर्ष_elt = si;
}

अटल व्योम
ps_derivs_prepend (parse_state *ps, derivation *d)
{
  काष्ठा deriv_chunk *dc = &ps->derivs;
  derivation_list_prepend (dc->contents, d);
  dc->शीर्ष_elt = d;
  ++dc->योग_आकार;
  अगर (!dc->पुच्छ_elt)
    dc->पुच्छ_elt = d;
}

अटल व्योम
ps_derivs_append (parse_state *ps, derivation *d)
{
  काष्ठा deriv_chunk *dc = &ps->derivs;
  derivation_list_append (dc->contents, d);
  dc->पुच्छ_elt = d;
  ++dc->योग_आकार;
  अगर (!dc->शीर्ष_elt)
    dc->शीर्ष_elt = d;
}

अटल पूर्णांक allocs = 0;
अटल पूर्णांक frees = 0;

अटल parse_state *
empty_parse_state (व्योम)
{
  parse_state *res = xcalloc (1, माप *res);
  res->state_वस्तुएँ.contents
    = gl_list_create_empty (GL_LINKED_LIST, शून्य, शून्य, शून्य, true);
  res->derivs.contents = derivation_list_new ();
  ++allocs;
  वापस res;
}

parse_state *
new_parse_state (स्थिर state_item *si)
{
  parse_state *res = empty_parse_state ();
  ps_si_append (res, si);
  ps_derivs_append (res, derivation_dot ());
  वापस res;
}

अटल parse_state *
copy_parse_state (bool prepend, parse_state *parent)
{
  parse_state *res = xmalloc (माप *res);
  *res = *parent;
  res->state_वस्तुएँ.contents
    = gl_list_create_empty (GL_LINKED_LIST, शून्य, शून्य, शून्य, true);
  res->derivs.contents = derivation_list_new ();
  res->parent = parent;
  res->prepend = prepend;
  res->reference_गिनती = 0;
  res->free_contents_early = false;
  parse_state_retain (parent);
  ++allocs;
  वापस res;
}

bool
parse_state_derivation_completed (स्थिर parse_state *ps)
{
  वापस ps->derivs.योग_आकार == 1;
}

derivation *
parse_state_derivation (स्थिर parse_state *ps)
{
  वापस (derivation *) ps->derivs.शीर्ष_elt;
}

स्थिर state_item *
parse_state_शीर्ष (स्थिर parse_state *ps)
{
  वापस ps->state_वस्तुएँ.शीर्ष_elt;
}

स्थिर state_item *
parse_state_पुच्छ (स्थिर parse_state *ps)
{
  वापस ps->state_वस्तुएँ.पुच्छ_elt;
}

पूर्णांक
parse_state_लंबाई (स्थिर parse_state *ps)
{
  वापस ps->state_वस्तुएँ.योग_आकार;
}

पूर्णांक
parse_state_depth (स्थिर parse_state *ps)
{
  वापस ps->depth;
}

व्योम
parse_state_retain (parse_state *ps)
{
  ++ps->reference_गिनती;
}

व्योम
parse_state_free_contents_early (parse_state *ps)
{
  ps->free_contents_early = true;
}

व्योम
free_parse_state (parse_state *original_ps)
{
  bool free_contents = true;
  parse_state *parent_ps = शून्य;
  क्रम (parse_state *ps = original_ps; ps && free_contents; ps = parent_ps)
    {
      --ps->reference_गिनती;
      free_contents = (ps->reference_गिनती == 1 && ps->free_contents_early)
        || (ps->reference_गिनती == 0 && !ps->free_contents_early);
      // need to keep the parse state around for visited hash set,
      // but its contents and parent can be freed
      अगर (free_contents)
        {
          अगर (ps->state_वस्तुएँ.contents)
            gl_list_free (ps->state_वस्तुएँ.contents);
          अगर (ps->derivs.contents)
            derivation_list_free (ps->derivs.contents);
        }
      parent_ps = ps->parent;
      अगर (ps->reference_गिनती <= 0)
        {
          मुक्त (ps);
          ++frees;
        }
    }
}

माप_प्रकार
parse_state_hasher (स्थिर parse_state *ps, माप_प्रकार max)
{
  स्थिर काष्ठा si_chunk *sis = &ps->state_वस्तुएँ;
  वापस ((state_item *) sis->शीर्ष_elt - state_वस्तुएँ +
          (state_item *) sis->पुच्छ_elt - state_वस्तुएँ + sis->योग_आकार) % max;
}

bool
parse_state_comparator (स्थिर parse_state *ps1, स्थिर parse_state *ps2)
{
  स्थिर काष्ठा si_chunk *sis1 = &ps1->state_वस्तुएँ;
  स्थिर काष्ठा si_chunk *sis2 = &ps2->state_वस्तुएँ;
  वापस sis1->शीर्ष_elt == sis2->शीर्ष_elt
    && sis1->पुच्छ_elt == sis2->पुच्छ_elt
    && sis1->योग_आकार == sis2->योग_आकार;
}


व्योम
parse_state_completed_steps (स्थिर parse_state *ps, पूर्णांक *shifts, पूर्णांक *productions)
{
  // traverse to the root parse_state,
  // which will have a list of all completed productions.
  स्थिर parse_state *मूल_ps = ps;
  जबतक (मूल_ps->parent)
    मूल_ps = मूल_ps->parent;

  state_item_list sis = मूल_ps->state_वस्तुएँ.contents;
  पूर्णांक गिनती = 0;

  state_item *last = शून्य;
  state_item *अग्र = शून्य;
  क्रम (gl_list_iterator_t it = gl_list_iterator (sis);
       state_item_list_अग्र (&it, &अग्र);
       )
    {
      अगर (last && last->state == अग्र->state)
        ++गिनती;
      last = अग्र;
    }
  *productions = गिनती;
  *shifts = मूल_ps->state_वस्तुएँ.योग_आकार - गिनती;
}

प्रकार व्योम (*chunk_append_fn) (gl_list_t, स्थिर व्योम *);

// A version of gl_list_add_last which has the chunk_append_fn
// signature.
अटल व्योम
list_add_last (gl_list_t list, स्थिर व्योम *elt)
{
  gl_list_add_last (list, elt);
}

// takes an array of n gl_lists and flattens them into two list
// based off of the index split
अटल व्योम
list_flatten_and_split (gl_list_t *list, gl_list_t *rets, पूर्णांक split, पूर्णांक n,
                        chunk_append_fn append_fn)
{
  पूर्णांक ret_सूचकांक = 0;
  पूर्णांक ret_array = 0;
  क्रम (पूर्णांक i = 0; i < n; ++i)
    {
      स्थिर व्योम *p = शून्य;
      gl_list_iterator_t it = gl_list_iterator (list[i]);
      जबतक (gl_list_iterator_अग्र (&it, &p, शून्य))
        अगर (p)
          {
            gl_list_t l = (gl_list_t) p;
            स्थिर व्योम *si = शून्य;
            gl_list_iterator_t it2 = gl_list_iterator (l);
            जबतक (gl_list_iterator_अग्र (&it2, &si, शून्य))
              {
                अगर (ret_सूचकांक++ == split)
                  ++ret_array;
                अगर (rets[ret_array])
                  append_fn (rets[ret_array], si);
              }
            gl_list_iterator_free (&it2);
          }
      gl_list_iterator_free (&it);
    }
}

अटल parse_state_list
parse_state_list_new (व्योम)
{
  वापस gl_list_create_empty (GL_LINKED_LIST, शून्य, शून्य,
                               (gl_listelement_dispose_fn)free_parse_state,
                               true);
}

अटल व्योम
parse_state_list_append (parse_state_list pl, parse_state *ps)
{
  parse_state_retain (ps);
  gl_list_add_last (pl, ps);
}

// Emulates a reduction on a parse state by popping some amount of
// derivations and state_items off of the parse_state and returning
// the result in ret. Returns the derivation of what's popped.
अटल derivation_list
parser_निकालो (parse_state *ps, पूर्णांक deriv_सूचकांक,
            पूर्णांक si_सूचकांक, parse_state *ret)
{
  // prepend sis, append sis, prepend derivs, append derivs
  gl_list_t chunks[4];
  क्रम (पूर्णांक i = 0; i < 4; ++i)
    chunks[i] = gl_list_create_empty (GL_LINKED_LIST, शून्य, शून्य, शून्य, true);
  क्रम (parse_state *pn = ps; pn != शून्य; pn = pn->parent)
    अगर (pn->prepend)
      {
        gl_list_add_last (chunks[0], pn->state_वस्तुएँ.contents);
        gl_list_add_last (chunks[2], pn->derivs.contents);
      }
    अन्यथा
      {
        gl_list_add_first (chunks[1], pn->state_वस्तुएँ.contents);
        gl_list_add_first (chunks[3], pn->derivs.contents);
      }
  derivation_list popped_derivs = derivation_list_new ();
  gl_list_t ret_chunks[4] = { ret->state_वस्तुएँ.contents, शून्य,
    ret->derivs.contents, popped_derivs
  };
  list_flatten_and_split (chunks, ret_chunks, si_सूचकांक, 2,
                          list_add_last);
  list_flatten_and_split (chunks + 2, ret_chunks + 2, deriv_सूचकांक, 2,
                          (chunk_append_fn)derivation_list_append);
  माप_प्रकार s_आकार = gl_list_आकार (ret->state_वस्तुएँ.contents);
  ret->state_वस्तुएँ.योग_आकार = s_आकार;
  अगर (s_आकार > 0)
    {
      ret->state_वस्तुएँ.पुच्छ_elt = gl_list_get_at (ret->state_वस्तुएँ.contents,
                                                  s_आकार - 1);
      ret->state_वस्तुएँ.शीर्ष_elt =
        gl_list_get_at (ret->state_वस्तुएँ.contents, 0);
    }
  अन्यथा
    {
      ret->state_वस्तुएँ.पुच्छ_elt = शून्य;
      ret->state_वस्तुएँ.शीर्ष_elt = शून्य;
    }
  माप_प्रकार d_आकार = gl_list_आकार (ret->derivs.contents);
  ret->derivs.योग_आकार = d_आकार;
  अगर (d_आकार > 0)
    {
      ret->derivs.पुच्छ_elt = gl_list_get_at (ret->derivs.contents,
                                             d_आकार - 1);
      ret->derivs.शीर्ष_elt = gl_list_get_at (ret->derivs.contents, 0);
    }
  अन्यथा
    {
      ret->derivs.पुच्छ_elt = शून्य;
      ret->derivs.शीर्ष_elt = शून्य;
    }
  क्रम (पूर्णांक i = 0; i < 4; ++i)
    gl_list_free (chunks[i]);
  वापस popped_derivs;
}

व्योम
parse_state_lists (parse_state *ps, state_item_list *sitems,
                   derivation_list *derivs)
{
  parse_state *अस्थायी = empty_parse_state ();
  माप_प्रकार si_आकार = ps->state_वस्तुएँ.योग_आकार;
  माप_प्रकार deriv_आकार = ps->derivs.योग_आकार;
  derivation_list dl = parser_निकालो (ps, si_आकार, deriv_आकार, अस्थायी);
  *sitems = अस्थायी->state_वस्तुएँ.contents;
  *derivs = अस्थायी->derivs.contents;
  // prevent the return lists from being freed
  अस्थायी->state_वस्तुएँ.contents = शून्य;
  अस्थायी->derivs.contents = शून्य;
  free_parse_state (अस्थायी);
  derivation_list_free (dl);
}

/**
 * Compute the parse states that result from taking a transition on
 * nullable symbols whenever possible from the given state_item.
 */
अटल व्योम
nullable_closure (parse_state *ps, state_item *si, parse_state_list state_list)
{
  parse_state *current_ps = ps;
  state_item_संख्या पूर्व_sin = si - state_वस्तुएँ;
  क्रम (state_item_संख्या sin = si->trans; sin != -1;
       पूर्व_sin = sin, sin = state_वस्तुएँ[sin].trans)
    {
      state_item *psi = &state_वस्तुएँ[पूर्व_sin];
      प्रतीक_संख्या sp = item_संख्या_as_प्रतीक_संख्या (*psi->item);
      अगर (ISTOKEN (sp) || !nullable[sp - ntokens])
        अवरोध;

      state_item *nsi = &state_वस्तुएँ[sin];
      current_ps = copy_parse_state (false, current_ps);
      ps_si_append (current_ps, nsi);
      ps_derivs_append (current_ps,
                        derivation_new (sp, derivation_list_new (),
                                        state_item_rule (nsi)));
      parse_state_list_append (state_list, current_ps);
    }
}

parse_state_list
simulate_transition (parse_state *ps)
{
  स्थिर state_item *si = ps->state_वस्तुएँ.पुच्छ_elt;
  प्रतीक_संख्या sym = item_संख्या_as_प्रतीक_संख्या (*si->item);
  // Transition on the same next symbol, taking nullable
  // symbols into account.
  parse_state_list परिणाम = parse_state_list_new ();
  state_item_संख्या si_अग्र = si->trans;
  // Check for disabled transition, shouldn't happen as any
  // state_items that lead to these should be disabled.
  अगर (si_अग्र < 0)
    वापस परिणाम;
  parse_state *अग्र_ps = copy_parse_state (false, ps);
  ps_si_append (अग्र_ps, &state_वस्तुएँ[si_अग्र]);
  ps_derivs_append (अग्र_ps, derivation_new_leaf (sym));
  parse_state_list_append (परिणाम, अग्र_ps);

  nullable_closure (अग्र_ps, &state_वस्तुएँ[si_अग्र], परिणाम);
  वापस परिणाम;
}

/**
 * Determine if the given symbols are equal or their first sets
 * intersect.
 */
अटल bool
compatible (प्रतीक_संख्या sym1, प्रतीक_संख्या sym2)
{
  अगर (sym1 == sym2)
    वापस true;
  अगर (ISTOKEN (sym1) && ISVAR (sym2))
    वापस bitset_test (FIRSTS (sym2), sym1);
  अन्यथा अगर (ISVAR (sym1) && ISTOKEN (sym2))
    वापस bitset_test (FIRSTS (sym1), sym2);
  अन्यथा अगर (ISVAR (sym1) && ISVAR (sym2))
    वापस !bitset_disjoint_p (FIRSTS (sym1), FIRSTS (sym2));
  अन्यथा
    वापस false;
}

parse_state_list
simulate_production (parse_state *ps, प्रतीक_संख्या compat_sym)
{
  parse_state_list परिणाम = parse_state_list_new ();
  स्थिर state_item *si = parse_state_पुच्छ (ps);
  अगर (si->prods)
    {
      bitset_iterator biter;
      state_item_संख्या sin;
      BITSET_FOR_EACH (biter, si->prods, sin, 0)
        {
          // Take production step only if lhs is not nullable and
          // if first rhs symbol is compatible with compat_sym
          state_item *अग्र = &state_वस्तुएँ[sin];
          item_संख्या *itm1 = अग्र->item;
          अगर (!compatible (*itm1, compat_sym) || !production_allowed (si, अग्र))
            जारी;
          parse_state *अग्र_ps = copy_parse_state (false, ps);
          ps_si_append (अग्र_ps, अग्र);
          parse_state_list_append (परिणाम, अग्र_ps);
          अगर (अग्र_ps->depth >= 0)
            ++अग्र_ps->depth;
          nullable_closure (अग्र_ps, अग्र, परिणाम);
        }
    }
  वापस परिणाम;
}

// simulates a reduction on the given parse state, conflict_item is the
// item associated with ps's conflict. symbol_set is a lookahead set this
// reduction must be compatible with
parse_state_list
simulate_reduction (parse_state *ps, पूर्णांक rule_len, bitset प्रतीक_set)
{
  parse_state_list परिणाम = parse_state_list_new ();

  पूर्णांक s_आकार = ps->state_वस्तुएँ.योग_आकार;
  पूर्णांक d_आकार = ps->derivs.योग_आकार;
  अगर (ps->depth >= 0)
    d_आकार--;                   // account for dot
  parse_state *new_मूल = empty_parse_state ();
  derivation_list popped_derivs =
    parser_निकालो (ps, d_आकार - rule_len,
                s_आकार - rule_len - 1, new_मूल);

  // update derivation
  state_item *si = (state_item *) ps->state_वस्तुएँ.पुच्छ_elt;
  स्थिर rule *r = item_rule (si->item);
  प्रतीक_संख्या lhs = r->lhs->संख्या;
  derivation *deriv = derivation_new (lhs, popped_derivs, state_item_rule (si));
  --new_मूल->depth;
  ps_derivs_append (new_मूल, deriv);

  अगर (s_आकार != rule_len + 1)
    {
      state_item *पुच्छ = (state_item *) new_मूल->state_वस्तुएँ.पुच्छ_elt;
      ps_si_append (new_मूल, &state_वस्तुएँ[पुच्छ->trans]);
      parse_state_list_append (परिणाम, new_मूल);
    }
  अन्यथा
    {
      // The head state_item is a production item, so we need to prepend
      // with possible source state-items.
      स्थिर state_item *शीर्ष_3 = ps->state_वस्तुएँ.शीर्ष_elt;
      state_item_list पूर्व = lssi_reverse_production (शीर्ष_3, प्रतीक_set);
      // TODO: better understand what causes this case.
      अगर (gl_list_आकार (पूर्व) == 0)
        {
          // new_root needs to have an RC of 1 to be freed correctly here.
          parse_state_retain (new_मूल);
          free_parse_state (new_मूल);
        }
      अन्यथा
        {
          state_item *psis = शून्य;
          क्रम (gl_list_iterator_t it = gl_list_iterator (पूर्व);
               state_item_list_अग्र (&it, &psis);
               )
            {
              // Prepend the result from the reverse production.
              parse_state *copy = copy_parse_state (true, new_मूल);
              ps_si_prepend (copy, psis);

              // Append the left hand side to the end of the parser state
              copy = copy_parse_state (false, copy);
              काष्ठा si_chunk *sis = &copy->state_वस्तुएँ;
              स्थिर state_item *पुच्छ = sis->पुच्छ_elt;
              ps_si_append (copy, &state_वस्तुएँ[पुच्छ->trans]);
              parse_state_list_append (परिणाम, copy);
              nullable_closure (copy, (state_item *) sis->पुच्छ_elt, परिणाम);
            }
        }
      gl_list_free (पूर्व);
    }
  वापस परिणाम;
}

parse_state_list
parser_prepend (parse_state *ps)
{
  parse_state_list res = parse_state_list_new ();
  स्थिर state_item *शीर्ष_3 = ps->state_वस्तुएँ.शीर्ष_elt;
  प्रतीक_संख्या prepend_sym =
    item_संख्या_as_प्रतीक_संख्या (*(शीर्ष_3->item - 1));
  bitset_iterator biter;
  state_item_संख्या sin;
  BITSET_FOR_EACH (biter, शीर्ष_3->revs, sin, 0)
  {
    parse_state *copy = copy_parse_state (true, ps);
    ps_si_prepend (copy, &state_वस्तुएँ[sin]);
    अगर (SI_TRANSITION (शीर्ष_3))
      ps_derivs_prepend (copy, derivation_new_leaf (prepend_sym));
    parse_state_list_append (res, copy);
  }
  वापस res;
}

व्योम
print_parse_state (parse_state *ps)
{
  खाता *out = मानक_त्रुटि;
  ख_लिखो (out, "(size %zu depth %d rc %d)\n",
          ps->state_वस्तुएँ.योग_आकार, ps->depth, ps->reference_गिनती);
  state_item_print (ps->state_वस्तुएँ.शीर्ष_elt, out, "");
  state_item_print (ps->state_वस्तुएँ.पुच्छ_elt, out, "");
  अगर (ps->derivs.योग_आकार > 0)
    derivation_print (ps->derivs.शीर्ष_elt, out, "");
  अ_दो ('\n', out);
}
