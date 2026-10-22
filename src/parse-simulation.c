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

#अन्तर्भाव <config.h>

#अन्तर्भाव "parse-simulation.h"

#अन्तर्भाव <gl_linked_list.h>
#अन्तर्भाव <gl_xlist.h>
#अन्तर्भाव <stdlib.h>

#अन्तर्भाव "lssi.h"
#अन्तर्भाव "nullable.h"

संरचना parse_state
{
  // Path of state-items the parser has traversed.
  संरचना si_chunk
  {
    // Elements newly added in this chunk.
    state_item_list contents;
    // Properties of the linked list this chunk represents.
    ध्रुव state_item *शीर्ष_elt;
    ध्रुव state_item *पुच्छ_elt;
    size_t योग_आकार;
  } state_वस्तूनि;
  // List of derivations of the symbols.
  संरचना deriv_chunk
  {
    derivation_list contents;
    ध्रुव derivation *शीर्ष_elt;
    ध्रुव derivation *पुच्छ_elt;
    size_t योग_आकार;
  } derivs;
  संरचना parse_state *parent;
  पूर्णाङ्क reference_गणनम्;
  // Incremented during productions, decremented during reductions.
  पूर्णाङ्क गभीरता;
  // Whether the contents of the chunks should be prepended or
  // appended to the list the chunks represent.
  bool prepend;
  // Causes chunk contents to be freed when the reference count is
  // one. Used when only the chunk metadata will be needed.
  bool free_contents_early;
};


स्थिर शून्य
ps_si_prepend (parse_state *ps, ध्रुव state_item *si)
{
  संरचना si_chunk *sic = &ps->state_वस्तूनि;
  gl_list_सङ्कलय_first (sic->contents, si);
  sic->शीर्ष_elt = si;
  ++sic->योग_आकार;
  यदि (!sic->पुच्छ_elt)
    sic->पुच्छ_elt = si;
}

स्थिर शून्य
ps_si_append (parse_state *ps, ध्रुव state_item *si)
{
  संरचना si_chunk *sic = &ps->state_वस्तूनि;
  gl_list_सङ्कलय_चरमः (sic->contents, si);
  sic->पुच्छ_elt = si;
  ++sic->योग_आकार;
  यदि (!sic->शीर्ष_elt)
    sic->शीर्ष_elt = si;
}

स्थिर शून्य
ps_derivs_prepend (parse_state *ps, derivation *d)
{
  संरचना deriv_chunk *dc = &ps->derivs;
  derivation_list_prepend (dc->contents, d);
  dc->शीर्ष_elt = d;
  ++dc->योग_आकार;
  यदि (!dc->पुच्छ_elt)
    dc->पुच्छ_elt = d;
}

स्थिर शून्य
ps_derivs_append (parse_state *ps, derivation *d)
{
  संरचना deriv_chunk *dc = &ps->derivs;
  derivation_list_append (dc->contents, d);
  dc->पुच्छ_elt = d;
  ++dc->योग_आकार;
  यदि (!dc->शीर्ष_elt)
    dc->शीर्ष_elt = d;
}

स्थिर पूर्णाङ्क allocs = 0;
स्थिर पूर्णाङ्क frees = 0;

स्थिर parse_state *
रिक्तानि_parse_state (शून्य)
{
  parse_state *res = xcalloc (1, परिमाण *res);
  res->state_वस्तूनि.contents
    = gl_list_create_रिक्तानि (GL_LINKED_LIST, रिक्त, रिक्त, रिक्त, true);
  res->derivs.contents = derivation_list_new ();
  ++allocs;
  प्रत्यागम res;
}

parse_state *
new_parse_state (ध्रुव state_item *si)
{
  parse_state *res = रिक्तानि_parse_state ();
  ps_si_append (res, si);
  ps_derivs_append (res, derivation_dot ());
  प्रत्यागम res;
}

स्थिर parse_state *
copy_parse_state (bool prepend, parse_state *parent)
{
  parse_state *res = xmalloc (परिमाण *res);
  *res = *parent;
  res->state_वस्तूनि.contents
    = gl_list_create_रिक्तानि (GL_LINKED_LIST, रिक्त, रिक्त, रिक्त, true);
  res->derivs.contents = derivation_list_new ();
  res->parent = parent;
  res->prepend = prepend;
  res->reference_गणनम् = 0;
  res->free_contents_early = false;
  parse_state_retain (parent);
  ++allocs;
  प्रत्यागम res;
}

bool
parse_state_derivation_completed (ध्रुव parse_state *ps)
{
  प्रत्यागम ps->derivs.योग_आकार == 1;
}

derivation *
parse_state_derivation (ध्रुव parse_state *ps)
{
  प्रत्यागम (derivation *) ps->derivs.शीर्ष_elt;
}

ध्रुव state_item *
parse_state_शीर्ष (ध्रुव parse_state *ps)
{
  प्रत्यागम ps->state_वस्तूनि.शीर्ष_elt;
}

ध्रुव state_item *
parse_state_पुच्छ (ध्रुव parse_state *ps)
{
  प्रत्यागम ps->state_वस्तूनि.पुच्छ_elt;
}

पूर्णाङ्क
parse_state_दीर्घता (ध्रुव parse_state *ps)
{
  प्रत्यागम ps->state_वस्तूनि.योग_आकार;
}

पूर्णाङ्क
parse_state_गभीरता (ध्रुव parse_state *ps)
{
  प्रत्यागम ps->गभीरता;
}

शून्य
parse_state_retain (parse_state *ps)
{
  ++ps->reference_गणनम्;
}

शून्य
parse_state_free_contents_early (parse_state *ps)
{
  ps->free_contents_early = true;
}

शून्य
free_parse_state (parse_state *original_ps)
{
  bool free_contents = true;
  parse_state *parent_ps = रिक्त;
  प्रति (parse_state *ps = original_ps; ps && free_contents; ps = parent_ps)
    {
      --ps->reference_गणनम्;
      free_contents = (ps->reference_गणनम् == 1 && ps->free_contents_early)
        || (ps->reference_गणनम् == 0 && !ps->free_contents_early);
      // need to keep the parse state around for visited hash set,
      // but its contents and parent can be freed
      यदि (free_contents)
        {
          यदि (ps->state_वस्तूनि.contents)
            gl_list_free (ps->state_वस्तूनि.contents);
          यदि (ps->derivs.contents)
            derivation_list_free (ps->derivs.contents);
        }
      parent_ps = ps->parent;
      यदि (ps->reference_गणनम् <= 0)
        {
          मुक्त (ps);
          ++frees;
        }
    }
}

size_t
parse_state_hasher (ध्रुव parse_state *ps, size_t max)
{
  ध्रुव संरचना si_chunk *sis = &ps->state_वस्तूनि;
  प्रत्यागम ((state_item *) sis->शीर्ष_elt - state_वस्तूनि +
          (state_item *) sis->पुच्छ_elt - state_वस्तूनि + sis->योग_आकार) % max;
}

bool
parse_state_comparator (ध्रुव parse_state *ps1, ध्रुव parse_state *ps2)
{
  ध्रुव संरचना si_chunk *sis1 = &ps1->state_वस्तूनि;
  ध्रुव संरचना si_chunk *sis2 = &ps2->state_वस्तूनि;
  प्रत्यागम sis1->शीर्ष_elt == sis2->शीर्ष_elt
    && sis1->पुच्छ_elt == sis2->पुच्छ_elt
    && sis1->योग_आकार == sis2->योग_आकार;
}


शून्य
parse_state_completed_चरणाः (ध्रुव parse_state *ps, पूर्णाङ्क *shifts, पूर्णाङ्क *productions)
{
  // traverse to the root parse_state,
  // which will have a list of all completed productions.
  ध्रुव parse_state *मूलम्_ps = ps;
  यावत् (मूलम्_ps->parent)
    मूलम्_ps = मूलम्_ps->parent;

  state_item_list sis = मूलम्_ps->state_वस्तूनि.contents;
  पूर्णाङ्क गणनम् = 0;

  state_item *चरमः = रिक्त;
  state_item *अग्र = रिक्त;
  प्रति (gl_list_iterator_t it = gl_list_iterator (sis);
       state_item_list_अग्र (&it, &अग्र);
       )
    {
      यदि (चरमः && चरमः->state == अग्र->state)
        ++गणनम्;
      चरमः = अग्र;
    }
  *productions = गणनम्;
  *shifts = मूलम्_ps->state_वस्तूनि.योग_आकार - गणनम्;
}

प्रकारनिर्देश शून्य (*chunk_append_fn) (gl_list_t, ध्रुव शून्य *);

// A version of gl_list_add_last which has the chunk_append_fn
// signature.
स्थिर शून्य
list_सङ्कलय_चरमः (gl_list_t list, ध्रुव शून्य *elt)
{
  gl_list_सङ्कलय_चरमः (list, elt);
}

// takes an array of n gl_lists and flattens them into two list
// based off of the index split
स्थिर शून्य
list_flatten_and_विभज (gl_list_t *list, gl_list_t *rets, पूर्णाङ्क विभज, पूर्णाङ्क n,
                        chunk_append_fn append_fn)
{
  पूर्णाङ्क ret_सूचकांक = 0;
  पूर्णाङ्क ret_array = 0;
  प्रति (पूर्णाङ्क i = 0; i < n; ++i)
    {
      ध्रुव शून्य *p = रिक्त;
      gl_list_iterator_t it = gl_list_iterator (list[i]);
      यावत् (gl_list_iterator_अग्र (&it, &p, रिक्त))
        यदि (p)
          {
            gl_list_t l = (gl_list_t) p;
            ध्रुव शून्य *si = रिक्त;
            gl_list_iterator_t it2 = gl_list_iterator (l);
            यावत् (gl_list_iterator_अग्र (&it2, &si, रिक्त))
              {
                यदि (ret_सूचकांक++ == विभज)
                  ++ret_array;
                यदि (rets[ret_array])
                  append_fn (rets[ret_array], si);
              }
            gl_list_iterator_free (&it2);
          }
      gl_list_iterator_free (&it);
    }
}

स्थिर parse_state_list
parse_state_list_new (शून्य)
{
  प्रत्यागम gl_list_create_रिक्तानि (GL_LINKED_LIST, रिक्त, रिक्त,
                               (gl_listelement_dispose_fn)free_parse_state,
                               true);
}

स्थिर शून्य
parse_state_list_append (parse_state_list pl, parse_state *ps)
{
  parse_state_retain (ps);
  gl_list_सङ्कलय_चरमः (pl, ps);
}

// Emulates a reduction on a parse state by popping some amount of
// derivations and state_items off of the parse_state and returning
// the result in ret. Returns the derivation of what's popped.
स्थिर derivation_list
parser_निष्कासय (parse_state *ps, पूर्णाङ्क deriv_सूचकांक,
            पूर्णाङ्क si_सूचकांक, parse_state *ret)
{
  // prepend sis, append sis, prepend derivs, append derivs
  gl_list_t chunks[4];
  प्रति (पूर्णाङ्क i = 0; i < 4; ++i)
    chunks[i] = gl_list_create_रिक्तानि (GL_LINKED_LIST, रिक्त, रिक्त, रिक्त, true);
  प्रति (parse_state *pn = ps; pn != रिक्त; pn = pn->parent)
    यदि (pn->prepend)
      {
        gl_list_सङ्कलय_चरमः (chunks[0], pn->state_वस्तूनि.contents);
        gl_list_सङ्कलय_चरमः (chunks[2], pn->derivs.contents);
      }
    अन्यथा
      {
        gl_list_सङ्कलय_first (chunks[1], pn->state_वस्तूनि.contents);
        gl_list_सङ्कलय_first (chunks[3], pn->derivs.contents);
      }
  derivation_list popped_derivs = derivation_list_new ();
  gl_list_t ret_chunks[4] = { ret->state_वस्तूनि.contents, रिक्त,
    ret->derivs.contents, popped_derivs
  };
  list_flatten_and_विभज (chunks, ret_chunks, si_सूचकांक, 2,
                          list_सङ्कलय_चरमः);
  list_flatten_and_विभज (chunks + 2, ret_chunks + 2, deriv_सूचकांक, 2,
                          (chunk_append_fn)derivation_list_append);
  size_t s_आकार = gl_list_आकार (ret->state_वस्तूनि.contents);
  ret->state_वस्तूनि.योग_आकार = s_आकार;
  यदि (s_आकार > 0)
    {
      ret->state_वस्तूनि.पुच्छ_elt = gl_list_get_at (ret->state_वस्तूनि.contents,
                                                  s_आकार - 1);
      ret->state_वस्तूनि.शीर्ष_elt =
        gl_list_get_at (ret->state_वस्तूनि.contents, 0);
    }
  अन्यथा
    {
      ret->state_वस्तूनि.पुच्छ_elt = रिक्त;
      ret->state_वस्तूनि.शीर्ष_elt = रिक्त;
    }
  size_t d_आकार = gl_list_आकार (ret->derivs.contents);
  ret->derivs.योग_आकार = d_आकार;
  यदि (d_आकार > 0)
    {
      ret->derivs.पुच्छ_elt = gl_list_get_at (ret->derivs.contents,
                                             d_आकार - 1);
      ret->derivs.शीर्ष_elt = gl_list_get_at (ret->derivs.contents, 0);
    }
  अन्यथा
    {
      ret->derivs.पुच्छ_elt = रिक्त;
      ret->derivs.शीर्ष_elt = रिक्त;
    }
  प्रति (पूर्णाङ्क i = 0; i < 4; ++i)
    gl_list_free (chunks[i]);
  प्रत्यागम popped_derivs;
}

शून्य
parse_state_lists (parse_state *ps, state_item_list *sitems,
                   derivation_list *derivs)
{
  parse_state *अस्थायी = रिक्तानि_parse_state ();
  size_t si_आकार = ps->state_वस्तूनि.योग_आकार;
  size_t deriv_आकार = ps->derivs.योग_आकार;
  derivation_list dl = parser_निष्कासय (ps, si_आकार, deriv_आकार, अस्थायी);
  *sitems = अस्थायी->state_वस्तूनि.contents;
  *derivs = अस्थायी->derivs.contents;
  // prevent the return lists from being freed
  अस्थायी->state_वस्तूनि.contents = रिक्त;
  अस्थायी->derivs.contents = रिक्त;
  free_parse_state (अस्थायी);
  derivation_list_free (dl);
}

/**
 * Compute the parse states that result from taking a transition on
 * nullable symbols whenever possible from the given state_item.
 */
स्थिर शून्य
nullable_संवृतिः (parse_state *ps, state_item *si, parse_state_list state_list)
{
  parse_state *current_ps = ps;
  state_item_सङ्ख्या पूर्व_sin = si - state_वस्तूनि;
  प्रति (state_item_सङ्ख्या ज्या = si->trans; ज्या != -1;
       पूर्व_sin = ज्या, ज्या = state_वस्तूनि[ज्या].trans)
    {
      state_item *psi = &state_वस्तूनि[पूर्व_sin];
      चिह्नम्_सङ्ख्या sp = item_सङ्ख्या_as_चिह्नम्_सङ्ख्या (*psi->item);
      यदि (ISTOKEN (sp) || !nullable[sp - ntokens])
        भङ्ग;

      state_item *nsi = &state_वस्तूनि[ज्या];
      current_ps = copy_parse_state (false, current_ps);
      ps_si_append (current_ps, nsi);
      ps_derivs_append (current_ps,
                        derivation_new (sp, derivation_list_new (),
                                        state_item_नियमः (nsi)));
      parse_state_list_append (state_list, current_ps);
    }
}

parse_state_list
simulate_transition (parse_state *ps)
{
  ध्रुव state_item *si = ps->state_वस्तूनि.पुच्छ_elt;
  चिह्नम्_सङ्ख्या sym = item_सङ्ख्या_as_चिह्नम्_सङ्ख्या (*si->item);
  // Transition on the same next symbol, taking nullable
  // symbols into account.
  parse_state_list परिणाम = parse_state_list_new ();
  state_item_सङ्ख्या si_अग्र = si->trans;
  // Check for disabled transition, shouldn't happen as any
  // state_items that lead to these should be disabled.
  यदि (si_अग्र < 0)
    प्रत्यागम परिणाम;
  parse_state *अग्र_ps = copy_parse_state (false, ps);
  ps_si_append (अग्र_ps, &state_वस्तूनि[si_अग्र]);
  ps_derivs_append (अग्र_ps, derivation_new_पर्णम् (sym));
  parse_state_list_append (परिणाम, अग्र_ps);

  nullable_संवृतिः (अग्र_ps, &state_वस्तूनि[si_अग्र], परिणाम);
  प्रत्यागम परिणाम;
}

/**
 * Determine if the given symbols are equal or their first sets
 * intersect.
 */
स्थिर bool
compatible (चिह्नम्_सङ्ख्या sym1, चिह्नम्_सङ्ख्या sym2)
{
  यदि (sym1 == sym2)
    प्रत्यागम true;
  यदि (ISTOKEN (sym1) && ISVAR (sym2))
    प्रत्यागम bitset_test (FIRSTS (sym2), sym1);
  अन्यथा यदि (ISVAR (sym1) && ISTOKEN (sym2))
    प्रत्यागम bitset_test (FIRSTS (sym1), sym2);
  अन्यथा यदि (ISVAR (sym1) && ISVAR (sym2))
    प्रत्यागम !bitset_disjoint_p (FIRSTS (sym1), FIRSTS (sym2));
  अन्यथा
    प्रत्यागम false;
}

parse_state_list
simulate_production (parse_state *ps, चिह्नम्_सङ्ख्या compat_sym)
{
  parse_state_list परिणाम = parse_state_list_new ();
  ध्रुव state_item *si = parse_state_पुच्छ (ps);
  यदि (si->prods)
    {
      bitset_iterator biter;
      state_item_सङ्ख्या ज्या;
      BITSET_FOR_EACH (biter, si->prods, ज्या, 0)
        {
          // Take production step only if lhs is not nullable and
          // if first rhs symbol is compatible with compat_sym
          state_item *अग्र = &state_वस्तूनि[ज्या];
          item_सङ्ख्या *itm1 = अग्र->item;
          यदि (!compatible (*itm1, compat_sym) || !production_allowed (si, अग्र))
            अनुवर्त;
          parse_state *अग्र_ps = copy_parse_state (false, ps);
          ps_si_append (अग्र_ps, अग्र);
          parse_state_list_append (परिणाम, अग्र_ps);
          यदि (अग्र_ps->गभीरता >= 0)
            ++अग्र_ps->गभीरता;
          nullable_संवृतिः (अग्र_ps, अग्र, परिणाम);
        }
    }
  प्रत्यागम परिणाम;
}

// simulates a reduction on the given parse state, conflict_item is the
// item associated with ps's conflict. symbol_set is a lookahead set this
// reduction must be compatible with
parse_state_list
simulate_reduction (parse_state *ps, पूर्णाङ्क नियमः_len, bitset चिह्नम्_समुच्चयः)
{
  parse_state_list परिणाम = parse_state_list_new ();

  पूर्णाङ्क s_आकार = ps->state_वस्तूनि.योग_आकार;
  पूर्णाङ्क d_आकार = ps->derivs.योग_आकार;
  यदि (ps->गभीरता >= 0)
    d_आकार--;                   // account for dot
  parse_state *new_मूलम् = रिक्तानि_parse_state ();
  derivation_list popped_derivs =
    parser_निष्कासय (ps, d_आकार - नियमः_len,
                s_आकार - नियमः_len - 1, new_मूलम्);

  // update derivation
  state_item *si = (state_item *) ps->state_वस्तूनि.पुच्छ_elt;
  ध्रुव नियमः *r = item_नियमः (si->item);
  चिह्नम्_सङ्ख्या lhs = r->lhs->सङ्ख्या;
  derivation *deriv = derivation_new (lhs, popped_derivs, state_item_नियमः (si));
  --new_मूलम्->गभीरता;
  ps_derivs_append (new_मूलम्, deriv);

  यदि (s_आकार != नियमः_len + 1)
    {
      state_item *पुच्छ = (state_item *) new_मूलम्->state_वस्तूनि.पुच्छ_elt;
      ps_si_append (new_मूलम्, &state_वस्तूनि[पुच्छ->trans]);
      parse_state_list_append (परिणाम, new_मूलम्);
    }
  अन्यथा
    {
      // The head state_item is a production item, so we need to prepend
      // with possible source state-items.
      ध्रुव state_item *शीर्ष = ps->state_वस्तूनि.शीर्ष_elt;
      state_item_list पूर्व = lssi_reverse_production (शीर्ष, चिह्नम्_समुच्चयः);
      // TODO: better understand what causes this case.
      यदि (gl_list_आकार (पूर्व) == 0)
        {
          // new_root needs to have an RC of 1 to be freed correctly here.
          parse_state_retain (new_मूलम्);
          free_parse_state (new_मूलम्);
        }
      अन्यथा
        {
          state_item *psis = रिक्त;
          प्रति (gl_list_iterator_t it = gl_list_iterator (पूर्व);
               state_item_list_अग्र (&it, &psis);
               )
            {
              // Prepend the result from the reverse production.
              parse_state *copy = copy_parse_state (true, new_मूलम्);
              ps_si_prepend (copy, psis);

              // Append the left hand side to the end of the parser state
              copy = copy_parse_state (false, copy);
              संरचना si_chunk *sis = &copy->state_वस्तूनि;
              ध्रुव state_item *पुच्छ = sis->पुच्छ_elt;
              ps_si_append (copy, &state_वस्तूनि[पुच्छ->trans]);
              parse_state_list_append (परिणाम, copy);
              nullable_संवृतिः (copy, (state_item *) sis->पुच्छ_elt, परिणाम);
            }
        }
      gl_list_free (पूर्व);
    }
  प्रत्यागम परिणाम;
}

parse_state_list
parser_prepend (parse_state *ps)
{
  parse_state_list res = parse_state_list_new ();
  ध्रुव state_item *शीर्ष = ps->state_वस्तूनि.शीर्ष_elt;
  चिह्नम्_सङ्ख्या prepend_sym =
    item_सङ्ख्या_as_चिह्नम्_सङ्ख्या (*(शीर्ष->item - 1));
  bitset_iterator biter;
  state_item_सङ्ख्या ज्या;
  BITSET_FOR_EACH (biter, शीर्ष->revs, ज्या, 0)
  {
    parse_state *copy = copy_parse_state (true, ps);
    ps_si_prepend (copy, &state_वस्तूनि[ज्या]);
    यदि (SI_TRANSITION (शीर्ष))
      ps_derivs_prepend (copy, derivation_new_पर्णम् (prepend_sym));
    parse_state_list_append (res, copy);
  }
  प्रत्यागम res;
}

शून्य
print_parse_state (parse_state *ps)
{
  FILE *बहिर्गमनम् = stderr;
  fprintf (बहिर्गमनम्, "(size %zu depth %d rc %d)\n",
          ps->state_वस्तूनि.योग_आकार, ps->गभीरता, ps->reference_गणनम्);
  state_item_print (ps->state_वस्तूनि.शीर्ष_elt, बहिर्गमनम्, "");
  state_item_print (ps->state_वस्तूनि.पुच्छ_elt, बहिर्गमनम्, "");
  यदि (ps->derivs.योग_आकार > 0)
    derivation_print (ps->derivs.शीर्ष_elt, बहिर्गमनम्, "");
  putc ('\n', बहिर्गमनम्);
}
