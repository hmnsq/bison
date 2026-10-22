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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ "system.h"

#അന്തര്ഭാവ <mbfile.h>
#അന്തര്ഭാവ <mbswidth.h>
#അന്തര്ഭാവ <quotearg.h>
#അന്തര്ഭാവ <stdio.h>    /* fileno */
#അന്തര്ഭാവ <sys/ioctl.h>
#അന്തര്ഭാവ <sys/stat.h> /* fstat */
#അന്തര്ഭാവ <termios.h>

#ifdef WINSIZE_IN_PTEM
# include <sys/stream.h>
# include <sys/ptem.h>
#endif

#അന്തര്ഭാവ "complain.h"
#അന്തര്ഭാവ "getargs.h"
#അന്തര്ഭാവ "location.h"

location സ്ഥിരം empty_loc = EMPTY_LOCATION_INIT;

/* The terminal width.  Not less than 40.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
columns (ശൂന്യം)
{
  സ്ഥിരം അക്ഷരം *cp = getenv ("COLUMNS");
  പൂർണ്ണസംഖ്യ res = 80;
  എങ്കിൽ (cp && *cp)
    {
      long l = strtol (cp, ശൂന്യ, 10);
      res = 0 <= l && l <= INT_MAX ? l : INT_MAX;
    }
  അല്ലെങ്കിൽ
    {
#ifdef TIOCGWINSZ
      ഘടന winsize ws;
      എങ്കിൽ (ioctl (STDERR_FILENO, TIOCGWINSZ, &ws) != -1
          && 0 < ws.ws_col && ws.ws_col == (size_t) ws.ws_col)
        res = ws.ws_col;
#endif
    }
  തിരികെ max_int (res, 40);
}

/* Available screen width.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ screen_width = 80;

/* The ellipsis symbol to use for this locale, and the number of
   screen-columns it uses.  */
സ്ഥിതം സ്ഥിരം അക്ഷരം *ellipsis = "...";
സ്ഥിതം പൂർണ്ണസംഖ്യ ellipsize = 3;

/* If BUF is null, add BUFSIZE (which in this case must be less than
   INT_MAX) to COLUMN; otherwise, add mbsnwidth (BUF, BUFSIZE, 0) to
   COLUMN.  If an overflow occurs, return INT_MAX.  */

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
add_column_width (പൂർണ്ണസംഖ്യ column, അക്ഷരം സ്ഥിരം *buf, size_t bufsize)
{
  പൂർണ്ണസംഖ്യ width
    = buf ? mbsnwidth (buf, bufsize, 0)
    : INT_MAX <= bufsize ? INT_MAX
    : bufsize;
  തിരികെ column <= INT_MAX - width ? column + width : INT_MAX;
}

സ്ഥിതം ശൂന്യം
boundary_compute (boundary *cur, അക്ഷരം സ്ഥിരം *token, size_t size)
{
  പൂർണ്ണസംഖ്യ വരി = cur->വരി;
  പൂർണ്ണസംഖ്യ column = cur->column;
  പൂർണ്ണസംഖ്യ byte = cur->byte;
  അക്ഷരം സ്ഥിരം *p0 = token;
  അക്ഷരം സ്ഥിരം *p = token;
  അക്ഷരം സ്ഥിരം *lim = token + size;

  വേണ്ടി (p = token; p < lim; ++p)
    തിരഞ്ഞെടുപ്പ് (*p)
      {
      അവസ്ഥ '\n':
        വരി += വരി < INT_MAX;
        column = 1;
        byte = 1;
        p0 = p + 1;
        നിർത്തുക;

      അവസ്ഥ '\t':
        column = add_column_width (column, p0, p - p0);
        column = add_column_width (column, ശൂന്യ, 8 - ((column - 1) & 7));
        p0 = p + 1;
        byte += byte < INT_MAX;
        നിർത്തുക;

      സ്ഥിരസ്ഥിതി:
        byte += byte < INT_MAX;
        നിർത്തുക;
      }
  column = add_column_width (column, p0, p - p0);

  cur->വരി = വരി;
  cur->column = column;
  cur->byte = byte;
}


/* Set *LOC and adjust scanner cursor to account for token TOKEN of
   size SIZE.  */

ശൂന്യം
location_compute (location *loc, boundary *cur, അക്ഷരം സ്ഥിരം *token, size_t size)
{
  loc->start = *cur;
  boundary_compute (cur, token, size);
  loc->end = *cur;

  എങ്കിൽ (loc->end.വരി == INT_MAX && loc->start.വരി != INT_MAX)
    complain (loc, Wother, _("line number overflow"));
  എങ്കിൽ (loc->end.column == INT_MAX && loc->start.column != INT_MAX)
    complain (loc, Wother, _("column number overflow"));
  /* TRANSLATORS: we are counting bytes, and there are too many.  */
  എങ്കിൽ (loc->end.byte == INT_MAX && loc->start.byte != INT_MAX)
    complain (loc, Wother, _("byte number overflow"));
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
boundary_print (boundary സ്ഥിരം *b, FILE *out)
{
  തിരികെ fprintf (out, "%s:%d.%d@%d",
                  quotearg_n_style (3, escape_quoting_style, b->file),
                  b->വരി, b->column, b->byte);
}

പൂർണ്ണസംഖ്യ
location_print (location loc, FILE *out)
{
  പൂർണ്ണസംഖ്യ res = 0;
  എങ്കിൽ (location_empty (loc))
    res += fprintf (out, "(empty location)");
  അല്ലെങ്കിൽ എങ്കിൽ (trace_flag & trace_locations)
    {
      res += boundary_print (&loc.start, out);
      res += fprintf (out, "-");
      res += boundary_print (&loc.end, out);
    }
  അല്ലെങ്കിൽ
    {
      aver (loc.start.file);
      aver (loc.end.file);
      പൂർണ്ണസംഖ്യ end_col = 0 != loc.end.column ? loc.end.column - 1 : 0;
      res += fprintf (out, "%s",
                      quotearg_n_style (3, escape_quoting_style, loc.start.file));
      എങ്കിൽ (0 < loc.start.വരി)
        {
          res += fprintf (out, ":%d", loc.start.വരി);
          എങ്കിൽ (0 < loc.start.column)
            res += fprintf (out, ".%d", loc.start.column);
        }
      എങ്കിൽ (loc.start.file != loc.end.file)
        {
          res += fprintf (out, "-%s",
                          quotearg_n_style (3, escape_quoting_style,
                                            loc.end.file));
          എങ്കിൽ (0 < loc.end.വരി)
            {
              res += fprintf (out, ":%d", loc.end.വരി);
              എങ്കിൽ (0 <= end_col)
                res += fprintf (out, ".%d", end_col);
            }
        }
      അല്ലെങ്കിൽ എങ്കിൽ (0 < loc.end.വരി)
        {
          എങ്കിൽ (loc.start.വരി < loc.end.വരി)
            {
              res += fprintf (out, "-%d", loc.end.വരി);
              എങ്കിൽ (0 <= end_col)
                res += fprintf (out, ".%d", end_col);
            }
          അല്ലെങ്കിൽ എങ്കിൽ (0 <= end_col && loc.start.column < end_col)
            res += fprintf (out, "-%d", end_col);
        }
    }

  തിരികെ res;
}


/* Persistent data used by location_caret to avoid reopening and rereading the
   same file all over for each error.  */
സ്ഥിതം ഘടന
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
  പൂർണ്ണസംഖ്യ വരി_len;
  /* Given the initial column to display, the offset (number of
     characters to skip at the beginning of the line). */
  പൂർണ്ണസംഖ്യ skip;

  /* Available width to quote the source file.  Eight chars are
     consumed by the left-margin (with line number).  */
  പൂർണ്ണസംഖ്യ width;
} caret_info;

ശൂന്യം caret_init (ശൂന്യം)
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

ശൂന്യം
caret_free (ശൂന്യം)
{
  എങ്കിൽ (caret_info.file)
    {
      fclose (caret_info.file);
      caret_info.file = ശൂന്യ;
    }
}

/* Open FILE for quoting, if needed, and if possible.  Return whether
   the file can quoted. */
സ്ഥിതം bool
caret_set_file (സ്ഥിരം അക്ഷരം *file)
{
  /* If a different file than before, close and let the rest open
     the new one. */
  എങ്കിൽ (caret_info.pos.file && caret_info.pos.file != file)
    {
      caret_free ();
      caret_info.pos.file = ശൂന്യ;
    }
  എങ്കിൽ (!caret_info.pos.file)
    {
      caret_info.pos.file = file;
      എങ്കിൽ ((caret_info.file = fopen (caret_info.pos.file, "rb")))
        {
          /* If the file is not regular (imagine #line 1 "/dev/stdin"
             in the input file for instance), don't try to quote the
             file.  Keep caret_info.pos.file set so that we don't try
             to open it again, but leave caret_info.file NULL so that
             we don't try to quote it. */
          ഘടന stat buf;
          എങ്കിൽ (fstat (fileno (caret_info.file), &buf) == 0
              && buf.st_mode & S_IFREG)
            {
              caret_info.pos.വരി = 1;
              mbf_init (caret_info.mbfile, caret_info.file);
            }
          അല്ലെങ്കിൽ
            caret_free ();
        }
    }
  തിരികെ !!caret_info.file;
}

/* Getc, but smash \r\n as \n.  */
സ്ഥിതം ശൂന്യം
caret_getc_internal (mbchar_t *res)
{
  mbf_getc (*res, caret_info.mbfile);
  എങ്കിൽ (mb_iseq (*res, '\r'))
    {
      mbchar_t c;
      mbf_getc (c, caret_info.mbfile);
      എങ്കിൽ (mb_iseq (c, '\n'))
        mb_copy (res, &c);
      അല്ലെങ്കിൽ
        mbf_ungetc (c, caret_info.mbfile);
    }
}

#define caret_getc(Var) caret_getc_internal(&Var)

/* Move CARET_INFO (which has a valid FILE) to the line number LINE.
   Compute and cache that line's length in CARET_INFO.LINE_LEN.
   Return whether successful.  */
സ്ഥിതം bool
caret_set_വരി (പൂർണ്ണസംഖ്യ വരി)
{
  /* If the line we want to quote is seekable (the same line as the previous
     location), just seek it. If it was a previous line, we lost track of it,
     so return to the start of file.  */
  എങ്കിൽ (വരി < caret_info.pos.വരി)
    {
      caret_info.pos.വരി = 1;
      caret_info.offset = 0;
    }
  എങ്കിൽ (fseek (caret_info.file, caret_info.offset, SEEK_SET))
    തിരികെ false;

  /* If this is the same line as the previous one, we are done. */
  എങ്കിൽ (വരി < caret_info.pos.വരി)
    തിരികെ true;

  /* Advance to the line's position, keeping track of the offset.  */
  ആകുമ്പോൾ (caret_info.pos.വരി < വരി)
    {
      mbchar_t c;
      caret_getc (c);
      എങ്കിൽ (mb_iseof (c))
        /* Something is wrong, that line number does not exist.  */
        തിരികെ false;
      caret_info.pos.വരി += mb_iseq (c, '\n');
    }
  caret_info.offset = ftell (caret_info.file);
  caret_info.pos.column = 1;
  /* Reset mbf's internal state.
     FIXME: should be done in mbfile.  */
  caret_info.mbfile.eof_seen = 0;

  /* Find the number of columns of this line.  */
  ആകുമ്പോൾ (true)
    {
      mbchar_t c;
      caret_getc (c);
      എങ്കിൽ (mb_iseof (c) || mb_iseq (c, '\n'))
        നിർത്തുക;
      boundary_compute (&caret_info.pos, mb_ptr (c), mb_len (c));
    }
  caret_info.വരി_len = caret_info.pos.column;
  /* Go back to the beginning of line.  */
  എങ്കിൽ (fseek (caret_info.file, caret_info.offset, SEEK_SET))
    തിരികെ false;
  /* Reset mbf's internal state.
     FIXME: should be done in mbfile.  */
  caret_info.mbfile.eof_seen = 0;
  caret_info.pos.column = 1;
  തിരികെ true;
}

/* Compute CARET_INFO.WIDTH and CARET_INFO.SKIP based on the fact that
   the first column to display in the current line is COL.  */
സ്ഥിതം bool
caret_set_column (പൂർണ്ണസംഖ്യ col)
{
  /* Available width.  Eight chars are consumed by the left-margin
     (with line number).  */
  caret_info.width = screen_width - 8;
  caret_info.skip = 0;
  എങ്കിൽ (caret_info.width < caret_info.വരി_len)
    {
      /* We cannot quote the whole line.  Make sure we can see the
         beginning of the location.  */
      caret_info.skip = caret_info.width < col ? col - 10 : 0;
    }
  /* If we skip the initial part, we insert "..." before.  */
  എങ്കിൽ (caret_info.skip)
    caret_info.width -= ellipsize;
  /* If the end of line does not fit, we also need to truncate the
     end, and leave "..." there.  */
  എങ്കിൽ (caret_info.width < caret_info.വരി_len - caret_info.skip)
    caret_info.width -= ellipsize;
  തിരികെ true;
}

ശൂന്യം
location_caret (location loc, സ്ഥിരം അക്ഷരം *style, FILE *out)
{
  എങ്കിൽ (!(feature_flag & feature_caret))
    തിരികെ;
  എങ്കിൽ (!loc.start.വരി)
    തിരികെ;
  എങ്കിൽ (!caret_set_file (loc.start.file))
    തിരികെ;
  എങ്കിൽ (!caret_set_വരി (loc.start.വരി))
    തിരികെ;
  എങ്കിൽ (!caret_set_column (loc.start.column))
    തിരികെ;

  സ്ഥിരം പൂർണ്ണസംഖ്യ width = caret_info.width;
  സ്ഥിരം പൂർണ്ണസംഖ്യ skip = caret_info.skip;

  /* Read the actual line.  Don't update the offset, so that we keep a pointer
     to the start of the line.  */
  {
    mbchar_t c;
    caret_getc (c);
    എങ്കിൽ (!mb_iseof (c))
      {
        /* The last column to highlight.  Only the first line of
           multiline locations are quoted, in which case the ending
           column is the end of line.

           We used to work with byte offsets, and that was much
           easier.  However, we went back to using (visual) columns to
           support truncating of long lines.  */
        സ്ഥിരം പൂർണ്ണസംഖ്യ col_end
          = loc.start.വരി == loc.end.വരി
          ? loc.end.column
          : caret_info.വരി_len;
        /* Quote the file (at most the first line in the case of
           multiline locations).  */
        {
          fprintf (out, "%5d | %s", loc.start.വരി, skip ? ellipsis : "");
          /* Whether we opened the style.  If the line is not as
             expected (maybe the file was changed since the scanner
             ran), we might reach the end before we actually saw the
             opening column.  */
          എണ്ണൽ { before, inside, after } state = before;
          ആകുമ്പോൾ (!mb_iseof (c) && !mb_iseq (c, '\n'))
            {
              // We might have already opened (and even closed!) the
              // style and yet have the equality of the columns if we
              // just saw zero-width characters.
              എങ്കിൽ (state == before
                  && caret_info.pos.column == loc.start.column)
                {
                  begin_use_class (style, out);
                  state = inside;
                }
              എങ്കിൽ (skip < caret_info.pos.column)
                mb_putc (c, out);
              boundary_compute (&caret_info.pos, mb_ptr (c), mb_len (c));
              caret_getc (c);
              എങ്കിൽ (state == inside
                  && (caret_info.pos.column == col_end
                      || width < caret_info.pos.column - skip))
                {
                  end_use_class (style, out);
                  state = after;
                }
              എങ്കിൽ (width < caret_info.pos.column - skip)
                {
                  fputs (ellipsis, out);
                  നിർത്തുക;
                }
            }
          എങ്കിൽ (state == inside)
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
          വേണ്ടി (പൂർണ്ണസംഖ്യ i = loc.start.column - 1 - skip + 1,
                 i_end = min_int (col_end - 1 - skip, width);
               i < i_end; ++i)
            putc ('~', out);
          end_use_class (style, out);
          putc ('\n', out);
        }
      }
  }
}

ശൂന്യം
location_caret_suggestion (location loc, സ്ഥിരം അക്ഷരം *s, FILE *out)
{
  എങ്കിൽ (!(feature_flag & feature_caret))
    തിരികെ;
  സ്ഥിരം അക്ഷരം *style = "fixit-insert";
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
  തിരികെ !loc.start.file && !loc.start.വരി && !loc.start.column
    && !loc.end.file && !loc.end.വരി && !loc.end.column;
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
str_to_int (സ്ഥിരം അക്ഷരം *s)
{
  long l = strtol (s, ശൂന്യ, 10);
  തിരികെ l < 0 ? -1 : l <= INT_MAX ? l : INT_MAX;
}

ശൂന്യം
boundary_set_from_string (boundary *bound, അക്ഷരം *str)
{
  /* Search backwards: the file name may contain '.'  or ':'.  */
  അക്ഷരം *at = strrchr (str, '@');
  എങ്കിൽ (at)
    {
      *at = '\0';
      bound->byte = str_to_int (at + 1);
    }
  {
    അക്ഷരം *dot = strrchr (str, '.');
    aver (dot);
    *dot = '\0';
    bound->column = str_to_int (dot + 1);
    എങ്കിൽ (!at)
      bound->byte = bound->column;
  }
  {
    അക്ഷരം *colon = strrchr (str, ':');
    aver (colon);
    *colon = '\0';
    bound->വരി = str_to_int (colon + 1);
  }
  bound->file = uniqstr_new (str);
}
