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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ "system.h"

#અન્તર્ભાવ <mbfile.h>
#અન્તર્ભાવ <mbswidth.h>
#અન્તર્ભાવ <quotearg.h>
#અન્તર્ભાવ <stdio.h>    /* fileno */
#અન્તર્ભાવ <sys/ioctl.h>
#અન્તર્ભાવ <sys/stat.h> /* fstat */
#અન્તર્ભાવ <termios.h>

#ifdef WINSIZE_IN_PTEM
# include <sys/stream.h>
# include <sys/ptem.h>
#endif

#અન્તર્ભાવ "complain.h"
#અન્તર્ભાવ "getargs.h"
#અન્તર્ભાવ "location.h"

location અચલ empty_loc = EMPTY_LOCATION_INIT;

/* The terminal width.  Not less than 40.  */
સ્થિર પૂર્ણાંક
columns (શૂન્ય)
{
  અચલ અક્ષર *cp = getenv ("COLUMNS");
  પૂર્ણાંક res = 80;
  જો (cp && *cp)
    {
      long l = strtol (cp, ખાલી, 10);
      res = 0 <= l && l <= INT_MAX ? l : INT_MAX;
    }
  નહીંતર
    {
#ifdef TIOCGWINSZ
      સંરચના winsize ws;
      જો (ioctl (STDERR_FILENO, TIOCGWINSZ, &ws) != -1
          && 0 < ws.ws_col && ws.ws_col == (size_t) ws.ws_col)
        res = ws.ws_col;
#endif
    }
  પાછા max_int (res, 40);
}

/* Available screen width.  */
સ્થિર પૂર્ણાંક screen_width = 80;

/* The ellipsis symbol to use for this locale, and the number of
   screen-columns it uses.  */
સ્થિર અચલ અક્ષર *ellipsis = "...";
સ્થિર પૂર્ણાંક ellipsize = 3;

/* If BUF is null, add BUFSIZE (which in this case must be less than
   INT_MAX) to COLUMN; otherwise, add mbsnwidth (BUF, BUFSIZE, 0) to
   COLUMN.  If an overflow occurs, return INT_MAX.  */

સ્થિર અંતઃરેખ પૂર્ણાંક
add_column_width (પૂર્ણાંક column, અક્ષર અચલ *buf, size_t bufsize)
{
  પૂર્ણાંક width
    = buf ? mbsnwidth (buf, bufsize, 0)
    : INT_MAX <= bufsize ? INT_MAX
    : bufsize;
  પાછા column <= INT_MAX - width ? column + width : INT_MAX;
}

સ્થિર શૂન્ય
boundary_compute (boundary *cur, અક્ષર અચલ *token, size_t size)
{
  પૂર્ણાંક લીટી = cur->લીટી;
  પૂર્ણાંક column = cur->column;
  પૂર્ણાંક byte = cur->byte;
  અક્ષર અચલ *p0 = token;
  અક્ષર અચલ *p = token;
  અક્ષર અચલ *lim = token + size;

  માટે (p = token; p < lim; ++p)
    પસંદગી (*p)
      {
      સ્થિતિ '\n':
        લીટી += લીટી < INT_MAX;
        column = 1;
        byte = 1;
        p0 = p + 1;
        તોડો;

      સ્થિતિ '\t':
        column = add_column_width (column, p0, p - p0);
        column = add_column_width (column, ખાલી, 8 - ((column - 1) & 7));
        p0 = p + 1;
        byte += byte < INT_MAX;
        તોડો;

      મૂળભૂત:
        byte += byte < INT_MAX;
        તોડો;
      }
  column = add_column_width (column, p0, p - p0);

  cur->લીટી = લીટી;
  cur->column = column;
  cur->byte = byte;
}


/* Set *LOC and adjust scanner cursor to account for token TOKEN of
   size SIZE.  */

શૂન્ય
location_compute (location *loc, boundary *cur, અક્ષર અચલ *token, size_t size)
{
  loc->start = *cur;
  boundary_compute (cur, token, size);
  loc->end = *cur;

  જો (loc->end.લીટી == INT_MAX && loc->start.લીટી != INT_MAX)
    complain (loc, Wother, _("line number overflow"));
  જો (loc->end.column == INT_MAX && loc->start.column != INT_MAX)
    complain (loc, Wother, _("column number overflow"));
  /* TRANSLATORS: we are counting bytes, and there are too many.  */
  જો (loc->end.byte == INT_MAX && loc->start.byte != INT_MAX)
    complain (loc, Wother, _("byte number overflow"));
}

સ્થિર પૂર્ણાંક
boundary_print (boundary અચલ *b, FILE *out)
{
  પાછા fprintf (out, "%s:%d.%d@%d",
                  quotearg_n_style (3, escape_quoting_style, b->file),
                  b->લીટી, b->column, b->byte);
}

પૂર્ણાંક
location_print (location loc, FILE *out)
{
  પૂર્ણાંક res = 0;
  જો (location_empty (loc))
    res += fprintf (out, "(empty location)");
  નહીંતર જો (trace_flag & trace_locations)
    {
      res += boundary_print (&loc.start, out);
      res += fprintf (out, "-");
      res += boundary_print (&loc.end, out);
    }
  નહીંતર
    {
      aver (loc.start.file);
      aver (loc.end.file);
      પૂર્ણાંક end_col = 0 != loc.end.column ? loc.end.column - 1 : 0;
      res += fprintf (out, "%s",
                      quotearg_n_style (3, escape_quoting_style, loc.start.file));
      જો (0 < loc.start.લીટી)
        {
          res += fprintf (out, ":%d", loc.start.લીટી);
          જો (0 < loc.start.column)
            res += fprintf (out, ".%d", loc.start.column);
        }
      જો (loc.start.file != loc.end.file)
        {
          res += fprintf (out, "-%s",
                          quotearg_n_style (3, escape_quoting_style,
                                            loc.end.file));
          જો (0 < loc.end.લીટી)
            {
              res += fprintf (out, ":%d", loc.end.લીટી);
              જો (0 <= end_col)
                res += fprintf (out, ".%d", end_col);
            }
        }
      નહીંતર જો (0 < loc.end.લીટી)
        {
          જો (loc.start.લીટી < loc.end.લીટી)
            {
              res += fprintf (out, "-%d", loc.end.લીટી);
              જો (0 <= end_col)
                res += fprintf (out, ".%d", end_col);
            }
          નહીંતર જો (0 <= end_col && loc.start.column < end_col)
            res += fprintf (out, "-%d", end_col);
        }
    }

  પાછા res;
}


/* Persistent data used by location_caret to avoid reopening and rereading the
   same file all over for each error.  */
સ્થિર સંરચના
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
  પૂર્ણાંક લીટી_len;
  /* Given the initial column to display, the offset (number of
     characters to skip at the beginning of the line). */
  પૂર્ણાંક skip;

  /* Available width to quote the source file.  Eight chars are
     consumed by the left-margin (with line number).  */
  પૂર્ણાંક width;
} caret_info;

શૂન્ય caret_init (શૂન્ય)
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

શૂન્ય
caret_free (શૂન્ય)
{
  જો (caret_info.file)
    {
      fclose (caret_info.file);
      caret_info.file = ખાલી;
    }
}

/* Open FILE for quoting, if needed, and if possible.  Return whether
   the file can quoted. */
સ્થિર bool
caret_set_file (અચલ અક્ષર *file)
{
  /* If a different file than before, close and let the rest open
     the new one. */
  જો (caret_info.pos.file && caret_info.pos.file != file)
    {
      caret_free ();
      caret_info.pos.file = ખાલી;
    }
  જો (!caret_info.pos.file)
    {
      caret_info.pos.file = file;
      જો ((caret_info.file = fopen (caret_info.pos.file, "rb")))
        {
          /* If the file is not regular (imagine #line 1 "/dev/stdin"
             in the input file for instance), don't try to quote the
             file.  Keep caret_info.pos.file set so that we don't try
             to open it again, but leave caret_info.file NULL so that
             we don't try to quote it. */
          સંરચના stat buf;
          જો (fstat (fileno (caret_info.file), &buf) == 0
              && buf.st_mode & S_IFREG)
            {
              caret_info.pos.લીટી = 1;
              mbf_init (caret_info.mbfile, caret_info.file);
            }
          નહીંતર
            caret_free ();
        }
    }
  પાછા !!caret_info.file;
}

/* Getc, but smash \r\n as \n.  */
સ્થિર શૂન્ય
caret_getc_internal (mbchar_t *res)
{
  mbf_getc (*res, caret_info.mbfile);
  જો (mb_iseq (*res, '\r'))
    {
      mbchar_t c;
      mbf_getc (c, caret_info.mbfile);
      જો (mb_iseq (c, '\n'))
        mb_copy (res, &c);
      નહીંતર
        mbf_ungetc (c, caret_info.mbfile);
    }
}

#define caret_getc(Var) caret_getc_internal(&Var)

/* Move CARET_INFO (which has a valid FILE) to the line number LINE.
   Compute and cache that line's length in CARET_INFO.LINE_LEN.
   Return whether successful.  */
સ્થિર bool
caret_set_લીટી (પૂર્ણાંક લીટી)
{
  /* If the line we want to quote is seekable (the same line as the previous
     location), just seek it. If it was a previous line, we lost track of it,
     so return to the start of file.  */
  જો (લીટી < caret_info.pos.લીટી)
    {
      caret_info.pos.લીટી = 1;
      caret_info.offset = 0;
    }
  જો (fseek (caret_info.file, caret_info.offset, SEEK_SET))
    પાછા false;

  /* If this is the same line as the previous one, we are done. */
  જો (લીટી < caret_info.pos.લીટી)
    પાછા true;

  /* Advance to the line's position, keeping track of the offset.  */
  જ્યારે (caret_info.pos.લીટી < લીટી)
    {
      mbchar_t c;
      caret_getc (c);
      જો (mb_iseof (c))
        /* Something is wrong, that line number does not exist.  */
        પાછા false;
      caret_info.pos.લીટી += mb_iseq (c, '\n');
    }
  caret_info.offset = ftell (caret_info.file);
  caret_info.pos.column = 1;
  /* Reset mbf's internal state.
     FIXME: should be done in mbfile.  */
  caret_info.mbfile.eof_seen = 0;

  /* Find the number of columns of this line.  */
  જ્યારે (true)
    {
      mbchar_t c;
      caret_getc (c);
      જો (mb_iseof (c) || mb_iseq (c, '\n'))
        તોડો;
      boundary_compute (&caret_info.pos, mb_ptr (c), mb_len (c));
    }
  caret_info.લીટી_len = caret_info.pos.column;
  /* Go back to the beginning of line.  */
  જો (fseek (caret_info.file, caret_info.offset, SEEK_SET))
    પાછા false;
  /* Reset mbf's internal state.
     FIXME: should be done in mbfile.  */
  caret_info.mbfile.eof_seen = 0;
  caret_info.pos.column = 1;
  પાછા true;
}

/* Compute CARET_INFO.WIDTH and CARET_INFO.SKIP based on the fact that
   the first column to display in the current line is COL.  */
સ્થિર bool
caret_set_column (પૂર્ણાંક col)
{
  /* Available width.  Eight chars are consumed by the left-margin
     (with line number).  */
  caret_info.width = screen_width - 8;
  caret_info.skip = 0;
  જો (caret_info.width < caret_info.લીટી_len)
    {
      /* We cannot quote the whole line.  Make sure we can see the
         beginning of the location.  */
      caret_info.skip = caret_info.width < col ? col - 10 : 0;
    }
  /* If we skip the initial part, we insert "..." before.  */
  જો (caret_info.skip)
    caret_info.width -= ellipsize;
  /* If the end of line does not fit, we also need to truncate the
     end, and leave "..." there.  */
  જો (caret_info.width < caret_info.લીટી_len - caret_info.skip)
    caret_info.width -= ellipsize;
  પાછા true;
}

શૂન્ય
location_caret (location loc, અચલ અક્ષર *style, FILE *out)
{
  જો (!(feature_flag & feature_caret))
    પાછા;
  જો (!loc.start.લીટી)
    પાછા;
  જો (!caret_set_file (loc.start.file))
    પાછા;
  જો (!caret_set_લીટી (loc.start.લીટી))
    પાછા;
  જો (!caret_set_column (loc.start.column))
    પાછા;

  અચલ પૂર્ણાંક width = caret_info.width;
  અચલ પૂર્ણાંક skip = caret_info.skip;

  /* Read the actual line.  Don't update the offset, so that we keep a pointer
     to the start of the line.  */
  {
    mbchar_t c;
    caret_getc (c);
    જો (!mb_iseof (c))
      {
        /* The last column to highlight.  Only the first line of
           multiline locations are quoted, in which case the ending
           column is the end of line.

           We used to work with byte offsets, and that was much
           easier.  However, we went back to using (visual) columns to
           support truncating of long lines.  */
        અચલ પૂર્ણાંક col_end
          = loc.start.લીટી == loc.end.લીટી
          ? loc.end.column
          : caret_info.લીટી_len;
        /* Quote the file (at most the first line in the case of
           multiline locations).  */
        {
          fprintf (out, "%5d | %s", loc.start.લીટી, skip ? ellipsis : "");
          /* Whether we opened the style.  If the line is not as
             expected (maybe the file was changed since the scanner
             ran), we might reach the end before we actually saw the
             opening column.  */
          ગણના { before, inside, after } state = before;
          જ્યારે (!mb_iseof (c) && !mb_iseq (c, '\n'))
            {
              // We might have already opened (and even closed!) the
              // style and yet have the equality of the columns if we
              // just saw zero-width characters.
              જો (state == before
                  && caret_info.pos.column == loc.start.column)
                {
                  begin_use_class (style, out);
                  state = inside;
                }
              જો (skip < caret_info.pos.column)
                mb_putc (c, out);
              boundary_compute (&caret_info.pos, mb_ptr (c), mb_len (c));
              caret_getc (c);
              જો (state == inside
                  && (caret_info.pos.column == col_end
                      || width < caret_info.pos.column - skip))
                {
                  end_use_class (style, out);
                  state = after;
                }
              જો (width < caret_info.pos.column - skip)
                {
                  fputs (ellipsis, out);
                  તોડો;
                }
            }
          જો (state == inside)
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
          માટે (પૂર્ણાંક i = loc.start.column - 1 - skip + 1,
                 i_end = min_int (col_end - 1 - skip, width);
               i < i_end; ++i)
            putc ('~', out);
          end_use_class (style, out);
          putc ('\n', out);
        }
      }
  }
}

શૂન્ય
location_caret_suggestion (location loc, અચલ અક્ષર *s, FILE *out)
{
  જો (!(feature_flag & feature_caret))
    પાછા;
  અચલ અક્ષર *style = "fixit-insert";
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
  પાછા !loc.start.file && !loc.start.લીટી && !loc.start.column
    && !loc.end.file && !loc.end.લીટી && !loc.end.column;
}

સ્થિર અંતઃરેખ પૂર્ણાંક
str_to_int (અચલ અક્ષર *s)
{
  long l = strtol (s, ખાલી, 10);
  પાછા l < 0 ? -1 : l <= INT_MAX ? l : INT_MAX;
}

શૂન્ય
boundary_set_from_string (boundary *bound, અક્ષર *str)
{
  /* Search backwards: the file name may contain '.'  or ':'.  */
  અક્ષર *at = strrchr (str, '@');
  જો (at)
    {
      *at = '\0';
      bound->byte = str_to_int (at + 1);
    }
  {
    અક્ષર *dot = strrchr (str, '.');
    aver (dot);
    *dot = '\0';
    bound->column = str_to_int (dot + 1);
    જો (!at)
      bound->byte = bound->column;
  }
  {
    અક્ષર *colon = strrchr (str, ':');
    aver (colon);
    *colon = '\0';
    bound->લીટી = str_to_int (colon + 1);
  }
  bound->file = uniqstr_new (str);
}
