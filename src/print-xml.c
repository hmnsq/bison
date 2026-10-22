/* Print an xml on generated parser, for Bison,

   Copyright (C) 2007, 2009-2015, 2018-2022 Free Software Foundation,
   Inc.

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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "print-xml.h"

#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ <bitset.h>
#ಅನ್ತರ್ಭಾವ <stdarg.h>

#ಅನ್ತರ್ಭಾವ "closure.h"
#ಅನ್ತರ್ಭಾವ "complain.h"
#ಅನ್ತರ್ಭಾವ "conflicts.h"
#ಅನ್ತರ್ಭಾವ "execute.h"
#ಅನ್ತರ್ಭಾವ "files.h"
#ಅನ್ತರ್ಭಾವ "getargs.h"
#ಅನ್ತರ್ಭಾವ "gram.h"
#ಅನ್ತರ್ಭಾವ "lalr.h"
#ಅನ್ತರ್ಭಾವ "lr0.h"
#ಅನ್ತರ್ಭಾವ "muscle-tab.h"
#ಅನ್ತರ್ಭಾವ "path-join.h"
#ಅನ್ತರ್ಭಾವ "print.h"
#ಅನ್ತರ್ಭಾವ "reader.h"
#ಅನ್ತರ್ಭಾವ "reduce.h"
#ಅನ್ತರ್ಭಾವ "state.h"
#ಅನ್ತರ್ಭಾವ "symtab.h"
#ಅನ್ತರ್ಭಾವ "tables.h"

ಸ್ಥಿರ bitset no_reduce_ಗಣ;
ರಚನೆ escape_buf
{
  ಅಕ್ಷರ *ptr;
  size_t ಗಾತ್ರ_2;
};
ಎಣಿಕೆ { ಅಂಕೆ_escape_bufs = 3 };
ಸ್ಥಿರ ರಚನೆ escape_buf escape_bufs[ಅಂಕೆ_escape_bufs];


/*--------------------------------.
| Report information on a state.  |
`--------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
print_core (FILE *ಹೊರಹೋಗು, ಪೂರ್ಣಾಂಕ level, state *s)
{
  ಅಂಶ_ಸೂಚ್ಯಂಕ *sitems = s->ವಸ್ತುಗಳು;
  size_t snritems = s->nitems;

  /* Output all the items of a state, not only its kernel.  */
  ಆವರಣ (sitems, snritems);
  sitems = itemset;
  snritems = nitemset;

  ಆದರೆ (!snritems)
    {
      xml_puts (ಹೊರಹೋಗು, level, "<itemset/>");
      ಹಿಂತಿರುಗಿ;
    }

  xml_puts (ಹೊರಹೋಗು, level, "<itemset>");

  ಗಾಗಿ (size_t i = 0; i < snritems; i++)
    {
      bool printed = false;
      ಅಂಶ_ಸಂಖ್ಯೆ *sp1 = ritem + sitems[i];
      ನಿಯಮ ಸ್ಥಿರಾಂಕ *r = ಅಂಶ_ನಿಯಮ (sp1);
      ಅಂಶ_ಸಂಖ್ಯೆ *sp = r->rhs;

      /* Display the lookahead tokens?  */
      ಆದರೆ (ಅಂಶ_ಸಂಖ್ಯೆ_is_ನಿಯಮ_ಸಂಖ್ಯೆ (*sp1))
        {
          reductions *reds = s->reductions;
          ಪೂರ್ಣಾಂಕ red = state_reduction_ಕಂಡುಹಿಡಿ (s, r);
          /* Print item with lookaheads if there are. */
          ಆದರೆ (reds->lookaheads && red != -1)
            {
              xml_printf (ಹೊರಹೋಗು, level + 1,
                          "<item rule-number=\"%d\" dot=\"%d\">",
                          r->ಸಂಖ್ಯೆ, sp1 - sp);
              state_ನಿಯಮ_lookaheads_print_xml (s, r,
                                                     ಹೊರಹೋಗು, level + 2);
              xml_puts (ಹೊರಹೋಗು, level + 1, "</item>");
              printed = true;
            }
        }

      ಆದರೆ (!printed)
        xml_printf (ಹೊರಹೋಗು, level + 1,
                    "<item rule-number=\"%d\" dot=\"%d\"/>",
                    r->ಸಂಖ್ಯೆ,
                    sp1 - sp);
    }
  xml_puts (ಹೊರಹೋಗು, level, "</itemset>");
}


/*-----------------------------------------------------------.
| Report the shifts if DISPLAY_SHIFTS_P or the gotos of S on |
| OUT.                                                       |
`-----------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
print_transitions (state *s, FILE *ಹೊರಹೋಗು, ಪೂರ್ಣಾಂಕ level)
{
  transitions *trans = s->transitions;
  ಪೂರ್ಣಾಂಕ n = 0;
  ಪೂರ್ಣಾಂಕ i;

  ಗಾಗಿ (i = 0; i < trans->ಅಂಕೆ; i++)
    ಆದರೆ (!TRANSITION_IS_DISABLED (trans, i))
      {
        n++;
      }

  /* Nothing to report. */
  ಆದರೆ (!n)
    {
      xml_puts (ಹೊರಹೋಗು, level, "<transitions/>");
      ಹಿಂತಿರುಗಿ;
    }

  /* Report lookahead tokens and shifts.  */
  xml_puts (ಹೊರಹೋಗು, level, "<transitions>");

  ಗಾಗಿ (i = 0; i < trans->ಅಂಕೆ; i++)
    ಆದರೆ (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_IS_ಸರಿಸು (trans, i))
      {
        ಚಿಹ್ನೆ *sym = symbols[TRANSITION_ಚಿಹ್ನೆ (trans, i)];
        ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಟ್ಯಾಗ್ = sym->ಟ್ಯಾಗ್;
        state *s1 = trans->ಸ್ಥಿತಿಗಳು[i];

        xml_printf (ಹೊರಹೋಗು, level + 1,
                    "<transition type=\"shift\" symbol=\"%s\" state=\"%d\"/>",
                    xml_escape (ಟ್ಯಾಗ್), s1->ಸಂಖ್ಯೆ);
      }

  ಗಾಗಿ (i = 0; i < trans->ಅಂಕೆ; i++)
    ಆದರೆ (!TRANSITION_IS_DISABLED (trans, i)
        && !TRANSITION_IS_ಸರಿಸು (trans, i))
      {
        ಚಿಹ್ನೆ *sym = symbols[TRANSITION_ಚಿಹ್ನೆ (trans, i)];
        ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಟ್ಯಾಗ್ = sym->ಟ್ಯಾಗ್;
        state *s1 = trans->ಸ್ಥಿತಿಗಳು[i];

        xml_printf (ಹೊರಹೋಗು, level + 1,
                    "<transition type=\"goto\" symbol=\"%s\" state=\"%d\"/>",
                    xml_escape (ಟ್ಯಾಗ್), s1->ಸಂಖ್ಯೆ);
      }

  xml_puts (ಹೊರಹೋಗು, level, "</transitions>");
}


/*--------------------------------------------------------.
| Report the explicit errors of S raised from %nonassoc.  |
`--------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
print_errs (FILE *ಹೊರಹೋಗು, ಪೂರ್ಣಾಂಕ level, state *s)
{
  errs *errp = s->errs;
  bool ಎಣಿಕೆ_2 = false;
  ಪೂರ್ಣಾಂಕ i;

  ಗಾಗಿ (i = 0; i < errp->ಅಂಕೆ; ++i)
    ಆದರೆ (errp->symbols[i])
      ಎಣಿಕೆ_2 = true;

  /* Nothing to report. */
  ಆದರೆ (!ಎಣಿಕೆ_2)
    {
      xml_puts (ಹೊರಹೋಗು, level, "<errors/>");
      ಹಿಂತಿರುಗಿ;
    }

  /* Report lookahead tokens and errors.  */
  xml_puts (ಹೊರಹೋಗು, level, "<errors>");
  ಗಾಗಿ (i = 0; i < errp->ಅಂಕೆ; ++i)
    ಆದರೆ (errp->symbols[i])
      {
        ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಟ್ಯಾಗ್ = errp->symbols[i]->ಟ್ಯಾಗ್;
        xml_printf (ಹೊರಹೋಗು, level + 1,
                    "<error symbol=\"%s\">nonassociative</error>",
                    xml_escape (ಟ್ಯಾಗ್));
      }
  xml_puts (ಹೊರಹೋಗು, level, "</errors>");
}


/*-------------------------------------------------------------------.
| Report a reduction of RULE on LOOKAHEAD (which can be 'default').  |
| If not ENABLED, the rule is masked by a shift or a reduce (S/R and |
| R/R conflicts).                                                    |
`-------------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
print_reduction (FILE *ಹೊರಹೋಗು, ಪೂರ್ಣಾಂಕ level, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *lookahead,
                 ನಿಯಮ *r, bool enabled)
{
  ಆದರೆ (ನಿಯಮ_is_initial (r))
    xml_printf (ಹೊರಹೋಗು, level,
                "<reduction symbol=\"%s\" rule=\"accept\" enabled=\"%s\"/>",
                xml_escape (lookahead),
                enabled ? "true" : "false");
  ಇಲ್ಲದಿದ್ದರೆ
    xml_printf (ಹೊರಹೋಗು, level,
                "<reduction symbol=\"%s\" rule=\"%d\" enabled=\"%s\"/>",
                xml_escape (lookahead),
                r->ಸಂಖ್ಯೆ,
                enabled ? "true" : "false");
}


/*-------------------------------------------.
| Report on OUT the reduction actions of S.  |
`-------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
print_reductions (FILE *ಹೊರಹೋಗು, ಪೂರ್ಣಾಂಕ level, state *s)
{
  transitions *trans = s->transitions;
  reductions *reds = s->reductions;
  ನಿಯಮ *default_reduction = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ report = false;
  ಪೂರ್ಣಾಂಕ i, j;

  ಆದರೆ (reds->ಅಂಕೆ == 0)
    {
      xml_puts (ಹೊರಹೋಗು, level, "<reductions/>");
      ಹಿಂತಿರುಗಿ;
    }

  ಆದರೆ (yydefact[s->ಸಂಖ್ಯೆ] != 0)
    default_reduction = &ನಿಯಮಗಳು[yydefact[s->ಸಂಖ್ಯೆ] - 1];

  bitset_zero (no_reduce_ಗಣ);
  FOR_EACH_ಸರಿಸು (trans, i)
    bitset_ಗಣ (no_reduce_ಗಣ, TRANSITION_ಚಿಹ್ನೆ (trans, i));
  ಗಾಗಿ (i = 0; i < s->errs->ಅಂಕೆ; ++i)
    ಆದರೆ (s->errs->symbols[i])
      bitset_ಗಣ (no_reduce_ಗಣ, s->errs->symbols[i]->content->ಸಂಖ್ಯೆ);

  ಆದರೆ (default_reduction)
    report = true;

  ಆದರೆ (reds->lookaheads)
    ಗಾಗಿ (i = 0; i < ntokens; i++)
      {
        bool ಎಣಿಕೆ_2 = bitset_test (no_reduce_ಗಣ, i);

        ಗಾಗಿ (j = 0; j < reds->ಅಂಕೆ; ++j)
          ಆದರೆ (bitset_test (reds->lookaheads[j], i))
            {
              ಆದರೆ (! ಎಣಿಕೆ_2)
                {
                  ಆದರೆ (reds->ನಿಯಮಗಳು[j] != default_reduction)
                    report = true;
                  ಎಣಿಕೆ_2 = true;
                }
              ಇಲ್ಲದಿದ್ದರೆ
                {
                  report = true;
                }
            }
      }

  /* Nothing to report. */
  ಆದರೆ (!report)
    {
      xml_puts (ಹೊರಹೋಗು, level, "<reductions/>");
      ಹಿಂತಿರುಗಿ;
    }

  xml_puts (ಹೊರಹೋಗು, level, "<reductions>");

  /* Report lookahead tokens (or $default) and reductions.  */
  ಆದರೆ (reds->lookaheads)
    ಗಾಗಿ (i = 0; i < ntokens; i++)
      {
        bool defaulted = false;
        bool ಎಣಿಕೆ_2 = bitset_test (no_reduce_ಗಣ, i);

        ಗಾಗಿ (j = 0; j < reds->ಅಂಕೆ; ++j)
          ಆದರೆ (bitset_test (reds->lookaheads[j], i))
            {
              ಆದರೆ (! ಎಣಿಕೆ_2)
                {
                  ಆದರೆ (reds->ನಿಯಮಗಳು[j] != default_reduction)
                    print_reduction (ಹೊರಹೋಗು, level + 1, symbols[i]->ಟ್ಯಾಗ್,
                                     reds->ನಿಯಮಗಳು[j], true);
                  ಇಲ್ಲದಿದ್ದರೆ
                    defaulted = true;
                  ಎಣಿಕೆ_2 = true;
                }
              ಇಲ್ಲದಿದ್ದರೆ
                {
                  ಆದರೆ (defaulted)
                    print_reduction (ಹೊರಹೋಗು, level + 1, symbols[i]->ಟ್ಯಾಗ್,
                                     default_reduction, true);
                  defaulted = false;
                  print_reduction (ಹೊರಹೋಗು, level + 1, symbols[i]->ಟ್ಯಾಗ್,
                                   reds->ನಿಯಮಗಳು[j], false);
                }
            }
      }

  ಆದರೆ (default_reduction)
    print_reduction (ಹೊರಹೋಗು, level + 1,
                     "$default", default_reduction, true);

  xml_puts (ಹೊರಹೋಗು, level, "</reductions>");
}


/*--------------------------------------------------------------.
| Report on OUT all the actions (shifts, gotos, reductions, and |
| explicit errors from %nonassoc) of S.                         |
`--------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
print_actions (FILE *ಹೊರಹೋಗು, ಪೂರ್ಣಾಂಕ level, state *s)
{
  xml_puts (ಹೊರಹೋಗು, level, "<actions>");
  print_transitions (s, ಹೊರಹೋಗು, level + 1);
  print_errs (ಹೊರಹೋಗು, level + 1, s);
  print_reductions (ಹೊರಹೋಗು, level + 1, s);
  xml_puts (ಹೊರಹೋಗು, level, "</actions>");
}


/*----------------------------------.
| Report all the data on S on OUT.  |
`----------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
print_state (FILE *ಹೊರಹೋಗು, ಪೂರ್ಣಾಂಕ level, state *s)
{
  fputc ('\n', ಹೊರಹೋಗು);
  xml_printf (ಹೊರಹೋಗು, level, "<state number=\"%d\">", s->ಸಂಖ್ಯೆ);
  print_core (ಹೊರಹೋಗು, level + 1, s);
  print_actions (ಹೊರಹೋಗು, level + 1, s);
  ಆದರೆ (s->solved_conflicts_xml)
    {
      xml_puts (ಹೊರಹೋಗು, level + 1, "<solved-conflicts>");
      fputs (s->solved_conflicts_xml, ಹೊರಹೋಗು);
      xml_puts (ಹೊರಹೋಗು, level + 1, "</solved-conflicts>");
    }
  ಇಲ್ಲದಿದ್ದರೆ
    xml_puts (ಹೊರಹೋಗು, level + 1, "<solved-conflicts/>");
  xml_puts (ಹೊರಹೋಗು, level, "</state>");
}


/*-----------------------------------------.
| Print information on the whole grammar.  |
`-----------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
print_grammar (FILE *ಹೊರಹೋಗು, ಪೂರ್ಣಾಂಕ level)
{
  fputc ('\n', ಹೊರಹೋಗು);
  xml_puts (ಹೊರಹೋಗು, level, "<grammar>");
  grammar_ನಿಯಮಗಳು_print_xml (ಹೊರಹೋಗು, level);

  /* Terminals */
  xml_puts (ಹೊರಹೋಗು, level + 1, "<terminals>");
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < max_ಸಂಕೇತ + 1; i++)
    ಆದರೆ (token_translations[i] != undeftoken->content->ಸಂಖ್ಯೆ)
      {
        ಚಿಹ್ನೆ ಸ್ಥಿರಾಂಕ *sym = symbols[token_translations[i]];
        ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಟ್ಯಾಗ್ = sym->ಟ್ಯಾಗ್;
        ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *type = sym->content->type_name;
        ಪೂರ್ಣಾಂಕ precedence = sym->content->prec;
        assoc associativity = sym->content->assoc;
        xml_indent (ಹೊರಹೋಗು, level + 2);
        fprintf (ಹೊರಹೋಗು,
                 "<terminal symbol-number=\"%d\" token-number=\"%d\""
                 " name=\"%s\" type=\"%s\" usefulness=\"%s\"",
                 token_translations[i], i, xml_escape_n (0, ಟ್ಯಾಗ್),
                 type ? xml_escape_n (1, type) : "",
                 reduce_token_ಬಳಸದ_ಒಳ_grammar (token_translations[i])
                   ? "unused-in-grammar" : "useful");
        ಆದರೆ (precedence)
          fprintf (ಹೊರಹೋಗು, " prec=\"%d\"", precedence);
        ಆದರೆ (associativity != undef_assoc)
          fprintf (ಹೊರಹೋಗು, " assoc=\"%s\"", assoc_ಗಮ್ಯ_string (associativity) + 1);
        fputs ("/>\n", ಹೊರಹೋಗು);
      }
  xml_puts (ಹೊರಹೋಗು, level + 1, "</terminals>");

  /* Nonterminals */
  xml_puts (ಹೊರಹೋಗು, level + 1, "<nonterminals>");
  ಗಾಗಿ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ i = ntokens; i < nsyms + nuseless_nonterminals; i++)
    {
      ಚಿಹ್ನೆ ಸ್ಥಿರಾಂಕ *sym = symbols[i];
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಟ್ಯಾಗ್ = sym->ಟ್ಯಾಗ್;
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *type = sym->content->type_name;
      xml_printf (ಹೊರಹೋಗು, level + 2,
                  "<nonterminal symbol-number=\"%d\" name=\"%s\""
                  " type=\"%s\""
                  " usefulness=\"%s\"/>",
                  i, xml_escape_n (0, ಟ್ಯಾಗ್),
                  type ? xml_escape_n (1, type) : "",
                  reduce_nonterminal_useless_ಒಳ_grammar (sym->content)
                    ? "useless-in-grammar" : "useful");
    }
  xml_puts (ಹೊರಹೋಗು, level + 1, "</nonterminals>");
  xml_puts (ಹೊರಹೋಗು, level, "</grammar>");
}

ಶೂನ್ಯ
xml_indent (FILE *ಹೊರಹೋಗು, ಪೂರ್ಣಾಂಕ level)
{
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < level; i++)
    fputs ("  ", ಹೊರಹೋಗು);
}

ಶೂನ್ಯ
xml_puts (FILE *ಹೊರಹೋಗು, ಪೂರ್ಣಾಂಕ level, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *s)
{
  xml_indent (ಹೊರಹೋಗು, level);
  fputs (s, ಹೊರಹೋಗು);
  fputc ('\n', ಹೊರಹೋಗು);
}

ಶೂನ್ಯ
xml_printf (FILE *ಹೊರಹೋಗು, ಪೂರ್ಣಾಂಕ level, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *fmt, ...)
{
  va_list arglist;

  xml_indent (ಹೊರಹೋಗು, level);

  va_ಆರಂಭ (arglist, fmt);
  vfprintf (ಹೊರಹೋಗು, fmt, arglist);
  va_ಕೊನೆ (arglist);

  fputc ('\n', ಹೊರಹೋಗು);
}

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *
xml_escape_string (ರಚನೆ escape_buf *buf, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *str)
{
  size_t len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (str);
  size_t max_expansion = ಗಾತ್ರ "&quot;" - 1;

  ಆದರೆ (buf->ಗಾತ್ರ_2 <= max_expansion * len)
    {
      buf->ಗಾತ್ರ_2 = max_expansion * len + 1;
      buf->ptr = x2realloc (buf->ptr, &buf->ಗಾತ್ರ_2);
    }
  ಅಕ್ಷರ *p = buf->ptr;

  ಗಾಗಿ (; *str; str++)
    ಆಯ್ಕೆ (*str)
      {
      ಪೂರ್ವನಿಯೋಜಿತ: *p++ = *str; ನಿಲ್ಲಿಸು;
      ಸ್ಥಿತಿ '&': p = stpcpy (p, "&amp;" ); ನಿಲ್ಲಿಸು;
      ಸ್ಥಿತಿ '<': p = stpcpy (p, "&lt;"  ); ನಿಲ್ಲಿಸು;
      ಸ್ಥಿತಿ '>': p = stpcpy (p, "&gt;"  ); ನಿಲ್ಲಿಸು;
      ಸ್ಥಿತಿ '"': p = stpcpy (p, "&quot;"); ನಿಲ್ಲಿಸು;
      }

  *p = '\0';
  ಹಿಂತಿರುಗಿ buf->ptr;
}

ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *
xml_escape_n (ಪೂರ್ಣಾಂಕ n, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *str)
{
  ಹಿಂತಿರುಗಿ xml_escape_string (escape_bufs + n, str);
}

ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *
xml_escape (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *str)
{
  ಹಿಂತಿರುಗಿ xml_escape_n (0, str);
}

ಶೂನ್ಯ
print_xml (ಶೂನ್ಯ)
{
  FILE *ಹೊರಹೋಗು = xfopen (spec_xml_file, "w");

  fputs ("<?xml version=\"1.0\"?>\n\n", ಹೊರಹೋಗು);

  ಪೂರ್ಣಾಂಕ level = 0;
  xml_printf (ಹೊರಹೋಗು, level,
              "<bison-xml-report version=\"%s\" bug-report=\"%s\""
              " url=\"%s\">",
              xml_escape_n (0, ಆವೃತ್ತಿ_2),
              xml_escape_n (1, PACKAGE_BUGREPORT),
              xml_escape_n (2, PACKAGE_URL));

  fputc ('\n', ಹೊರಹೋಗು);
  xml_printf (ಹೊರಹೋಗು, level + 1, "<filename>%s</filename>",
              xml_escape (grammar_file));

  /* print grammar */
  print_grammar (ಹೊರಹೋಗು, level + 1);

  no_reduce_ಗಣ = bitset_create (ntokens, BITSET_FIXED);

  /* print automaton */
  fputc ('\n', ಹೊರಹೋಗು);
  xml_puts (ಹೊರಹೋಗು, level + 1, "<automaton>");
  ಗಾಗಿ (state_ಸಂಖ್ಯೆ i = 0; i < nstates; i++)
    print_state (ಹೊರಹೋಗು, level + 2, ಸ್ಥಿತಿಗಳು[i]);
  xml_puts (ಹೊರಹೋಗು, level + 1, "</automaton>");

  bitset_free (no_reduce_ಗಣ);

  xml_puts (ಹೊರಹೋಗು, 0, "</bison-xml-report>");

  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಅಂಕೆ_escape_bufs; ++i)
    ಬಿಡುಗಡೆ (escape_bufs[i].ptr);

  xfclose (ಹೊರಹೋಗು);
}


ಶೂನ್ಯ
print_html (ಶೂನ್ಯ)
{
  ಪ್ರತಿಜ್ಞಾ (xml_flag);

  ಅಕ್ಷರ *xml2html = xpath_join (pkgdatadir (), "xslt/xml2xhtml.xsl");
  ಅಕ್ಷರ *xsltproc = muscle_percent_define_get ("tool.xsltproc");
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *argv[11];
  ಪೂರ್ಣಾಂಕ i = 0;
  argv[i++] = xsltproc;
  argv[i++] = "-o";
  argv[i++] = spec_html_file;
  argv[i++] = xml2html;
  argv[i++] = spec_xml_file;
  argv[i++] = ಖಾಲಿ;
  aver (i <= ARRAY_CARDINALITY (argv));

  ಆದರೆ (trace_flag & trace_tools)
    {
      fputs ("running:", stderr);
      ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; argv[j]; ++j)
        fprintf (stderr, " %s", argv[j]);
      fputc ('\n', stderr);
    }

  ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2
    = execute (argv[0],
               argv[0], argv,
               /* directory */ ಖಾಲಿ,
               /* ignore_sigpipe */ false,
               /* null_stdin, null_stdout, null_stderr */ true, true, true,
               /* slave_process */ true, /* exit_on_error */ false,
               /* termsigp */ ಖಾಲಿ);
  ಆದರೆ (ಸ್ಥಿತಿ_2)
    complain (ಖಾಲಿ, complaint, _("%s failed with status %d"), argv[0], ಸ್ಥಿತಿ_2);
  ಬಿಡುಗಡೆ (xsltproc);
  ಬಿಡುಗಡೆ (xml2html);
}
