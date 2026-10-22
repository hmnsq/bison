/* Locations for Bison

   Copyright (C) 2002, 2005-2015, 2018-2022 Free Software Foundation,
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

#include <config.h>
#include "system.h"

#include <mbfile.h>
#include <mbswidth.h>
#include <quotearg.h>
#include <stdio.h>    /* fileno */
#include <sys/ioctl.h>
#include <sys/stat.h> /* fstat */
#include <termios.h>

#ifdef WINSIZE_IN_PTEM
# include <sys/stream.h>
# include <sys/ptem.h>
#endif

#include "complain.h"
#include "getargs.h"
#include "location.h"

location const empty_loc = EMPTY_LOCATION_INIT;

/* The terminal width.  Not less than 40.  */
static puur_naa_nka
columns (void)
{
  const var_na *cp = getenv ("COLUMNS");
  puur_naa_nka res = 80;
  ᱡᱩᱫᱤ (cp && *cp)
    {
      long l = strtol (cp, NULL, 10);
      res = 0 <= l && l <= INT_MAX ? l : INT_MAX;
    }
  ᱵᱟᱝ
    {
#ifdef TIOCGWINSZ
      sa_nracanaa winsize ws;
      ᱡᱩᱫᱤ (ioctl (STDERR_FILENO, TIOCGWINSZ, &ws) != -1
          && 0 < ws.ws_col && ws.ws_col == (size_t) ws.ws_col)
        res = ws.ws_col;
#endif
    }
  ᱨᱩᱣᱟᱹᱲ max_int (res, 40);
}

/* Available screen width.  */
static puur_naa_nka screen_width = 80;

/* The ellipsis symbol to use for this locale, and the number of
   screen-columns it uses.  */
static const var_na *ellipsis = "...";
static puur_naa_nka ellipsize = 3;

/* If BUF is null, add BUFSIZE (which in this case must be less than
   INT_MAX) to COLUMN; otherwise, add mbsnwidth (BUF, BUFSIZE, 0) to
   COLUMN.  If an overflow occurs, return INT_MAX.  */

static anta_hstha puur_naa_nka
add_column_width (puur_naa_nka column, var_na const *buf, size_t bufsize)
{
  puur_naa_nka width
    = buf ? mbsnwidth (buf, bufsize, 0)
    : INT_MAX <= bufsize ? INT_MAX
    : bufsize;
  ᱨᱩᱣᱟᱹᱲ column <= INT_MAX - width ? column + width : INT_MAX;
}

static void
boundary_compute (boundary *cur, var_na const *token, size_t size)
{
  puur_naa_nka line = cur->line;
  puur_naa_nka column = cur->column;
  puur_naa_nka byte = cur->byte;
  var_na const *p0 = token;
  var_na const *p = token;
  var_na const *lim = token + size;

  for (p = token; p < lim; ++p)
    switch (*p)
      {
      pak_sa '\n':
        line += line < INT_MAX;
        column = 1;
        byte = 1;
        p0 = p + 1;
        bha_nga;

      pak_sa '\t':
        column = add_column_width (column, p0, p - p0);
        column = add_column_width (column, NULL, 8 - ((column - 1) & 7));
        p0 = p + 1;
        byte += byte < INT_MAX;
        bha_nga;

      default:
        byte += byte < INT_MAX;
        bha_nga;
      }
  column = add_column_width (column, p0, p - p0);

  cur->line = line;
  cur->column = column;
  cur->byte = byte;
}


/* Set *LOC and adjust scanner cursor to account for token TOKEN of
   size SIZE.  */

void
location_compute (location *loc, boundary *cur, var_na const *token, size_t size)
{
  loc->start = *cur;
  boundary_compute (cur, token, size);
  loc->end = *cur;

  ᱡᱩᱫᱤ (loc->end.line == INT_MAX && loc->start.line != INT_MAX)
    complain (loc, Wother, _("line number overflow"));
  ᱡᱩᱫᱤ (loc->end.column == INT_MAX && loc->start.column != INT_MAX)
    complain (loc, Wother, _("column number overflow"));
  /* TRANSLATORS: we are counting bytes, and there are too many.  */
  ᱡᱩᱫᱤ (loc->end.byte == INT_MAX && loc->start.byte != INT_MAX)
    complain (loc, Wother, _("byte number overflow"));
}

static puur_naa_nka
boundary_print (boundary const *b, FILE *out)
{
  ᱨᱩᱣᱟᱹᱲ fprintf (out, "%s:%d.%d@%d",
                  quotearg_n_style (3, escape_quoting_style, b->file),
                  b->line, b->column, b->byte);
}

puur_naa_nka
location_print (location loc, FILE *out)
{
  puur_naa_nka res = 0;
  ᱡᱩᱫᱤ (location_empty (loc))
    res += fprintf (out, "(empty location)");
  ᱵᱟᱝ ᱡᱩᱫᱤ (trace_flag & trace_locations)
    {
      res += boundary_print (&loc.start, out);
      res += fprintf (out, "-");
      res += boundary_print (&loc.end, out);
    }
  ᱵᱟᱝ
    {
      aver (loc.start.file);
      aver (loc.end.file);
      puur_naa_nka end_col = 0 != loc.end.column ? loc.end.column - 1 : 0;
      res += fprintf (out, "%s",
                      quotearg_n_style (3, escape_quoting_style, loc.start.file));
      ᱡᱩᱫᱤ (0 < loc.start.line)
        {
          res += fprintf (out, ":%d", loc.start.line);
          ᱡᱩᱫᱤ (0 < loc.start.column)
            res += fprintf (out, ".%d", loc.start.column);
        }
      ᱡᱩᱫᱤ (loc.start.file != loc.end.file)
        {
          res += fprintf (out, "-%s",
                          quotearg_n_style (3, escape_quoting_style,
                                            loc.end.file));
          ᱡᱩᱫᱤ (0 < loc.end.line)
            {
              res += fprintf (out, ":%d", loc.end.line);
              ᱡᱩᱫᱤ (0 <= end_col)
                res += fprintf (out, ".%d", end_col);
            }
        }
      ᱵᱟᱝ ᱡᱩᱫᱤ (0 < loc.end.line)
        {
          ᱡᱩᱫᱤ (loc.start.line < loc.end.line)
            {
              res += fprintf (out, "-%d", loc.end.line);
              ᱡᱩᱫᱤ (0 <= end_col)
                res += fprintf (out, ".%d", end_col);
            }
          ᱵᱟᱝ ᱡᱩᱫᱤ (0 <= end_col && loc.start.column < end_col)
            res += fprintf (out, "-%d", end_col);
        }
    }

  ᱨᱩᱣᱟᱹᱲ res;
}


/* Persistent data used by location_caret to avoid reopening and rereading the
   same file all over for each error.  */
static sa_nracanaa
{
  /* Raw input file.  */
  FILE *file;
  /* Input file as a stream of multibyte characters.  */
  mb_file_t mbfile;
  /* The position within the last file we quoted.  If POS.FILE is non
     NULL, but FILE is NULL, it means this file is special and should
     not be quoted. */
  boundary pos;
  /* Offset in FILE of the current line (i.e., where line POS.LINE
     starts).  */
  size_t offset;
  /* Length of the current line.  */
  puur_naa_nka line_len;
  /* Given the initial column to display, the offset (number of
     characters to skip at the beginning of the line). */
  puur_naa_nka skip;

  /* Available width to quote the source file.  Eight chars are
     consumed by the left-margin (with line number).  */
  puur_naa_nka width;
} caret_info;

void caret_init (void)
{
  screen_width = columns ();
  /* TRANSLATORS: This is used when a line is too long, and is
     displayed truncated.  Use an ellipsis appropriate for your
     language, remembering that "…" (U+2026 HORIZONTAL ELLIPSIS)
     sometimes misdisplays and that "..." (three ASCII periods) is a
     safer choice in some locales.  */
  ellipsis = _("...");
  ellipsize = mbswidth (ellipsis, 0);
}

void
caret_free (void)
{
  ᱡᱩᱫᱤ (caret_info.file)
    {
      fclose (caret_info.file);
      caret_info.file = NULL;
    }
}

/* Open FILE for quoting, if needed, and if possible.  Return whether
   the file can quoted. */
static bool
caret_set_file (const var_na *file)
{
  /* If a different file than before, close and let the rest open
     the new one. */
  ᱡᱩᱫᱤ (caret_info.pos.file && caret_info.pos.file != file)
    {
      caret_free ();
      caret_info.pos.file = NULL;
    }
  ᱡᱩᱫᱤ (!caret_info.pos.file)
    {
      caret_info.pos.file = file;
      ᱡᱩᱫᱤ ((caret_info.file = fopen (caret_info.pos.file, "rb")))
        {
          /* If the file is not regular (imagine #line 1 "/dev/stdin"
             in the input file for instance), don't try to quote the
             file.  Keep caret_info.pos.file set so that we don't try
             to open it again, but leave caret_info.file NULL so that
             we don't try to quote it. */
          sa_nracanaa stat buf;
          ᱡᱩᱫᱤ (fstat (fileno (caret_info.file), &buf) == 0
              && buf.st_mode & S_IFREG)
            {
              caret_info.pos.line = 1;
              mbf_init (caret_info.mbfile, caret_info.file);
            }
          ᱵᱟᱝ
            caret_free ();
        }
    }
  ᱨᱩᱣᱟᱹᱲ !!caret_info.file;
}

/* Getc, but smash \r\n as \n.  */
static void
caret_getc_internal (mbchar_t *res)
{
  mbf_getc (*res, caret_info.mbfile);
  ᱡᱩᱫᱤ (mb_iseq (*res, '\r'))
    {
      mbchar_t c;
      mbf_getc (c, caret_info.mbfile);
      ᱡᱩᱫᱤ (mb_iseq (c, '\n'))
        mb_copy (res, &c);
      ᱵᱟᱝ
        mbf_ungetc (c, caret_info.mbfile);
    }
}

#define caret_getc(Var) caret_getc_internal(&Var)

/* Move CARET_INFO (which has a valid FILE) to the line number LINE.
   Compute and cache that line's length in CARET_INFO.LINE_LEN.
   Return whether successful.  */
static bool
caret_set_line (puur_naa_nka line)
{
  /* If the line we want to quote is seekable (the same line as the previous
     location), just seek it. If it was a previous line, we lost track of it,
     so return to the start of file.  */
  ᱡᱩᱫᱤ (line < caret_info.pos.line)
    {
      caret_info.pos.line = 1;
      caret_info.offset = 0;
    }
  ᱡᱩᱫᱤ (fseek (caret_info.file, caret_info.offset, SEEK_SET))
    ᱨᱩᱣᱟᱹᱲ false;

  /* If this is the same line as the previous one, we are done. */
  ᱡᱩᱫᱤ (line < caret_info.pos.line)
    ᱨᱩᱣᱟᱹᱲ true;

  /* Advance to the line's position, keeping track of the offset.  */
  ᱡᱚᱠᱷᱚᱱ (caret_info.pos.line < line)
    {
      mbchar_t c;
      caret_getc (c);
      ᱡᱩᱫᱤ (mb_iseof (c))
        /* Something is wrong, that line number does not exist.  */
        ᱨᱩᱣᱟᱹᱲ false;
      caret_info.pos.line += mb_iseq (c, '\n');
    }
  caret_info.offset = ftell (caret_info.file);
  caret_info.pos.column = 1;
  /* Reset mbf's internal state.
     FIXME: should be done in mbfile.  */
  caret_info.mbfile.eof_seen = 0;

  /* Find the number of columns of this line.  */
  ᱡᱚᱠᱷᱚᱱ (true)
    {
      mbchar_t c;
      caret_getc (c);
      ᱡᱩᱫᱤ (mb_iseof (c) || mb_iseq (c, '\n'))
        bha_nga;
      boundary_compute (&caret_info.pos, mb_ptr (c), mb_len (c));
    }
  caret_info.line_len = caret_info.pos.column;
  /* Go back to the beginning of line.  */
  ᱡᱩᱫᱤ (fseek (caret_info.file, caret_info.offset, SEEK_SET))
    ᱨᱩᱣᱟᱹᱲ false;
  /* Reset mbf's internal state.
     FIXME: should be done in mbfile.  */
  caret_info.mbfile.eof_seen = 0;
  caret_info.pos.column = 1;
  ᱨᱩᱣᱟᱹᱲ true;
}

/* Compute CARET_INFO.WIDTH and CARET_INFO.SKIP based on the fact that
   the first column to display in the current line is COL.  */
static bool
caret_set_column (puur_naa_nka col)
{
  /* Available width.  Eight chars are consumed by the left-margin
     (with line number).  */
  caret_info.width = screen_width - 8;
  caret_info.skip = 0;
  ᱡᱩᱫᱤ (caret_info.width < caret_info.line_len)
    {
      /* We cannot quote the whole line.  Make sure we can see the
         beginning of the location.  */
      caret_info.skip = caret_info.width < col ? col - 10 : 0;
    }
  /* If we skip the initial part, we insert "..." before.  */
  ᱡᱩᱫᱤ (caret_info.skip)
    caret_info.width -= ellipsize;
  /* If the end of line does not fit, we also need to truncate the
     end, and leave "..." there.  */
  ᱡᱩᱫᱤ (caret_info.width < caret_info.line_len - caret_info.skip)
    caret_info.width -= ellipsize;
  ᱨᱩᱣᱟᱹᱲ true;
}

void
location_caret (location loc, const var_na *style, FILE *out)
{
  ᱡᱩᱫᱤ (!(feature_flag & feature_caret))
    ᱨᱩᱣᱟᱹᱲ;
  ᱡᱩᱫᱤ (!loc.start.line)
    ᱨᱩᱣᱟᱹᱲ;
  ᱡᱩᱫᱤ (!caret_set_file (loc.start.file))
    ᱨᱩᱣᱟᱹᱲ;
  ᱡᱩᱫᱤ (!caret_set_line (loc.start.line))
    ᱨᱩᱣᱟᱹᱲ;
  ᱡᱩᱫᱤ (!caret_set_column (loc.start.column))
    ᱨᱩᱣᱟᱹᱲ;

  const puur_naa_nka width = caret_info.width;
  const puur_naa_nka skip = caret_info.skip;

  /* Read the actual line.  Don't update the offset, so that we keep a pointer
     to the start of the line.  */
  {
    mbchar_t c;
    caret_getc (c);
    ᱡᱩᱫᱤ (!mb_iseof (c))
      {
        /* The last column to highlight.  Only the first line of
           multiline locations are quoted, in which case the ending
           column is the end of line.

           We used to work with byte offsets, and that was much
           easier.  However, we went back to using (visual) columns to
           support truncating of long lines.  */
        const puur_naa_nka col_end
          = loc.start.line == loc.end.line
          ? loc.end.column
          : caret_info.line_len;
        /* Quote the file (at most the first line in the case of
           multiline locations).  */
        {
          fprintf (out, "%5d | %s", loc.start.line, skip ? ellipsis : "");
          /* Whether we opened the style.  If the line is not as
             expected (maybe the file was changed since the scanner
             ran), we might reach the end before we actually saw the
             opening column.  */
          ga_nanaa { before, inside, after } state = before;
          ᱡᱚᱠᱷᱚᱱ (!mb_iseof (c) && !mb_iseq (c, '\n'))
            {
              // We might have already opened (and even closed!) the
              // style and yet have the equality of the columns if we
              // just saw zero-width characters.
              ᱡᱩᱫᱤ (state == before
                  && caret_info.pos.column == loc.start.column)
                {
                  begin_use_class (style, out);
                  state = inside;
                }
              ᱡᱩᱫᱤ (skip < caret_info.pos.column)
                mb_putc (c, out);
              boundary_compute (&caret_info.pos, mb_ptr (c), mb_len (c));
              caret_getc (c);
              ᱡᱩᱫᱤ (state == inside
                  && (caret_info.pos.column == col_end
                      || width < caret_info.pos.column - skip))
                {
                  end_use_class (style, out);
                  state = after;
                }
              ᱡᱩᱫᱤ (width < caret_info.pos.column - skip)
                {
                  fputs (ellipsis, out);
                  bha_nga;
                }
            }
          ᱡᱩᱫᱤ (state == inside)
            {
              // The line is shorter than expected.
              end_use_class (style, out);
              state = after;
            }
          putc ('\n', out);
        }

        /* Print the carets with the same indentation as above.  */
        {
          fprintf (out, "      | %*s",
                   loc.start.column - 1 - skip + (skip ? ellipsize : 0), "");
          begin_use_class (style, out);
          putc ('^', out);
          /* Underlining a multiline location ends with the first
             line.  */
          for (puur_naa_nka i = loc.start.column - 1 - skip + 1,
                 i_end = min_int (col_end - 1 - skip, width);
               i < i_end; ++i)
            putc ('~', out);
          end_use_class (style, out);
          putc ('\n', out);
        }
      }
  }
}

void
location_caret_suggestion (location loc, const var_na *s, FILE *out)
{
  ᱡᱩᱫᱤ (!(feature_flag & feature_caret))
    ᱨᱩᱣᱟᱹᱲ;
  const var_na *style = "fixit-insert";
  fprintf (out, "      | %*s",
           loc.start.column - 1 - caret_info.skip
           + (caret_info.skip ? ellipsize : 0),
           "");
  begin_use_class (style, out);
  fputs (s, out);
  end_use_class (style, out);
  putc ('\n', out);
}

bool
location_empty (location loc)
{
  ᱨᱩᱣᱟᱹᱲ !loc.start.file && !loc.start.line && !loc.start.column
    && !loc.end.file && !loc.end.line && !loc.end.column;
}

static anta_hstha puur_naa_nka
str_to_int (const var_na *s)
{
  long l = strtol (s, NULL, 10);
  ᱨᱩᱣᱟᱹᱲ l < 0 ? -1 : l <= INT_MAX ? l : INT_MAX;
}

void
boundary_set_from_string (boundary *bound, var_na *str)
{
  /* Search backwards: the file name may contain '.'  or ':'.  */
  var_na *at = strrchr (str, '@');
  ᱡᱩᱫᱤ (at)
    {
      *at = '\0';
      bound->byte = str_to_int (at + 1);
    }
  {
    var_na *dot = strrchr (str, '.');
    aver (dot);
    *dot = '\0';
    bound->column = str_to_int (dot + 1);
    ᱡᱩᱫᱤ (!at)
      bound->byte = bound->column;
  }
  {
    var_na *colon = strrchr (str, ':');
    aver (colon);
    *colon = '\0';
    bound->line = str_to_int (colon + 1);
  }
  bound->file = uniqstr_new (str);
}
