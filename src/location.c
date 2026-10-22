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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ <mbfile.h>
#ಅನ್ತರ್ಭಾವ <mbswidth.h>
#ಅನ್ತರ್ಭಾವ <quotearg.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>    /* fileno */
#ಅನ್ತರ್ಭಾವ <sys/ioctl.h>
#ಅನ್ತರ್ಭಾವ <sys/stat.h> /* fstat */
#ಅನ್ತರ್ಭಾವ <termios.h>

#ifdef WINSIZE_IN_PTEM
# include <sys/stream.h>
# include <sys/ptem.h>
#endif

#ಅನ್ತರ್ಭಾವ "complain.h"
#ಅನ್ತರ್ಭಾವ "getargs.h"
#ಅನ್ತರ್ಭಾವ "location.h"

location ಧ್ರುವ empty_loc = EMPTY_LOCATION_INIT;

/* The terminal width.  Not less than 40.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
columns (ಶೂನ್ಯ)
{
  ಧ್ರುವ ವರ್ಣ *cp = getenv ("COLUMNS");
  ಪೂರ್ಣಾಙ್ಕ res = 80;
  ಯದಿ (cp && *cp)
    {
      long l = strtol (cp, ರಿಕ್ತ, 10);
      res = 0 <= l && l <= INT_MAX ? l : INT_MAX;
    }
  ಅನ್ಯಥಾ
    {
#ifdef TIOCGWINSZ
      ಸಂರಚನಾ winsize ws;
      ಯದಿ (ioctl (STDERR_FILENO, TIOCGWINSZ, &ws) != -1
          && 0 < ws.ws_col && ws.ws_col == (size_t) ws.ws_col)
        res = ws.ws_col;
#endif
    }
  ಪ್ರತ್ಯಾಗಮ max_int (res, 40);
}

/* Available screen width.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ screen_width = 80;

/* The ellipsis symbol to use for this locale, and the number of
   screen-columns it uses.  */
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *ellipsis = "...";
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ ellipsize = 3;

/* If BUF is null, add BUFSIZE (which in this case must be less than
   INT_MAX) to COLUMN; otherwise, add mbsnwidth (BUF, BUFSIZE, 0) to
   COLUMN.  If an overflow occurs, return INT_MAX.  */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ
add_column_width (ಪೂರ್ಣಾಙ್ಕ column, ವರ್ಣ ಧ್ರುವ *buf, size_t bufsize)
{
  ಪೂರ್ಣಾಙ್ಕ width
    = buf ? mbsnwidth (buf, bufsize, 0)
    : INT_MAX <= bufsize ? INT_MAX
    : bufsize;
  ಪ್ರತ್ಯಾಗಮ column <= INT_MAX - width ? column + width : INT_MAX;
}

ಸ್ಥಿರ ಶೂನ್ಯ
boundary_compute (boundary *cur, ವರ್ಣ ಧ್ರುವ *token, size_t size)
{
  ಪೂರ್ಣಾಙ್ಕ ಸಾಲು = cur->ಸಾಲು;
  ಪೂರ್ಣಾಙ್ಕ column = cur->column;
  ಪೂರ್ಣಾಙ್ಕ byte = cur->byte;
  ವರ್ಣ ಧ್ರುವ *p0 = token;
  ವರ್ಣ ಧ್ರುವ *p = token;
  ವರ್ಣ ಧ್ರುವ *lim = token + size;

  ಪ್ರತಿ (p = token; p < lim; ++p)
    ವಿಕಲ್ಪ (*p)
      {
      ಪಕ್ಷ '\n':
        ಸಾಲು += ಸಾಲು < INT_MAX;
        column = 1;
        byte = 1;
        p0 = p + 1;
        ಭಙ್ಗ;

      ಪಕ್ಷ '\t':
        column = add_column_width (column, p0, p - p0);
        column = add_column_width (column, ರಿಕ್ತ, 8 - ((column - 1) & 7));
        p0 = p + 1;
        byte += byte < INT_MAX;
        ಭಙ್ಗ;

      ಸ್ವಭಾವ:
        byte += byte < INT_MAX;
        ಭಙ್ಗ;
      }
  column = add_column_width (column, p0, p - p0);

  cur->ಸಾಲು = ಸಾಲು;
  cur->column = column;
  cur->byte = byte;
}


/* Set *LOC and adjust scanner cursor to account for token TOKEN of
   size SIZE.  */

ಶೂನ್ಯ
location_compute (location *loc, boundary *cur, ವರ್ಣ ಧ್ರುವ *token, size_t size)
{
  loc->start = *cur;
  boundary_compute (cur, token, size);
  loc->end = *cur;

  ಯದಿ (loc->end.ಸಾಲು == INT_MAX && loc->start.ಸಾಲು != INT_MAX)
    complain (loc, Wother, _("line number overflow"));
  ಯದಿ (loc->end.column == INT_MAX && loc->start.column != INT_MAX)
    complain (loc, Wother, _("column number overflow"));
  /* TRANSLATORS: we are counting bytes, and there are too many.  */
  ಯದಿ (loc->end.byte == INT_MAX && loc->start.byte != INT_MAX)
    complain (loc, Wother, _("byte number overflow"));
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
boundary_print (boundary ಧ್ರುವ *b, FILE *out)
{
  ಪ್ರತ್ಯಾಗಮ fprintf (out, "%s:%d.%d@%d",
                  quotearg_n_style (3, escape_quoting_style, b->file),
                  b->ಸಾಲು, b->column, b->byte);
}

ಪೂರ್ಣಾಙ್ಕ
location_print (location loc, FILE *out)
{
  ಪೂರ್ಣಾಙ್ಕ res = 0;
  ಯದಿ (location_empty (loc))
    res += fprintf (out, "(empty location)");
  ಅನ್ಯಥಾ ಯದಿ (trace_flag & trace_locations)
    {
      res += boundary_print (&loc.start, out);
      res += fprintf (out, "-");
      res += boundary_print (&loc.end, out);
    }
  ಅನ್ಯಥಾ
    {
      aver (loc.start.file);
      aver (loc.end.file);
      ಪೂರ್ಣಾಙ್ಕ end_col = 0 != loc.end.column ? loc.end.column - 1 : 0;
      res += fprintf (out, "%s",
                      quotearg_n_style (3, escape_quoting_style, loc.start.file));
      ಯದಿ (0 < loc.start.ಸಾಲು)
        {
          res += fprintf (out, ":%d", loc.start.ಸಾಲು);
          ಯದಿ (0 < loc.start.column)
            res += fprintf (out, ".%d", loc.start.column);
        }
      ಯದಿ (loc.start.file != loc.end.file)
        {
          res += fprintf (out, "-%s",
                          quotearg_n_style (3, escape_quoting_style,
                                            loc.end.file));
          ಯದಿ (0 < loc.end.ಸಾಲು)
            {
              res += fprintf (out, ":%d", loc.end.ಸಾಲು);
              ಯದಿ (0 <= end_col)
                res += fprintf (out, ".%d", end_col);
            }
        }
      ಅನ್ಯಥಾ ಯದಿ (0 < loc.end.ಸಾಲು)
        {
          ಯದಿ (loc.start.ಸಾಲು < loc.end.ಸಾಲು)
            {
              res += fprintf (out, "-%d", loc.end.ಸಾಲು);
              ಯದಿ (0 <= end_col)
                res += fprintf (out, ".%d", end_col);
            }
          ಅನ್ಯಥಾ ಯದಿ (0 <= end_col && loc.start.column < end_col)
            res += fprintf (out, "-%d", end_col);
        }
    }

  ಪ್ರತ್ಯಾಗಮ res;
}


/* Persistent data used by location_caret to avoid reopening and rereading the
   same file all over for each error.  */
ಸ್ಥಿರ ಸಂರಚನಾ
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
  ಪೂರ್ಣಾಙ್ಕ ಸಾಲು_len;
  /* Given the initial column to display, the offset (number of
     characters to skip at the beginning of the line). */
  ಪೂರ್ಣಾಙ್ಕ skip;

  /* Available width to quote the source file.  Eight chars are
     consumed by the left-margin (with line number).  */
  ಪೂರ್ಣಾಙ್ಕ width;
} caret_info;

ಶೂನ್ಯ caret_init (ಶೂನ್ಯ)
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

ಶೂನ್ಯ
caret_free (ಶೂನ್ಯ)
{
  ಯದಿ (caret_info.file)
    {
      fclose (caret_info.file);
      caret_info.file = ರಿಕ್ತ;
    }
}

/* Open FILE for quoting, if needed, and if possible.  Return whether
   the file can quoted. */
ಸ್ಥಿರ bool
caret_set_file (ಧ್ರುವ ವರ್ಣ *file)
{
  /* If a different file than before, close and let the rest open
     the new one. */
  ಯದಿ (caret_info.pos.file && caret_info.pos.file != file)
    {
      caret_free ();
      caret_info.pos.file = ರಿಕ್ತ;
    }
  ಯದಿ (!caret_info.pos.file)
    {
      caret_info.pos.file = file;
      ಯದಿ ((caret_info.file = fopen (caret_info.pos.file, "rb")))
        {
          /* If the file is not regular (imagine #line 1 "/dev/stdin"
             in the input file for instance), don't try to quote the
             file.  Keep caret_info.pos.file set so that we don't try
             to open it again, but leave caret_info.file NULL so that
             we don't try to quote it. */
          ಸಂರಚನಾ stat buf;
          ಯದಿ (fstat (fileno (caret_info.file), &buf) == 0
              && buf.st_mode & S_IFREG)
            {
              caret_info.pos.ಸಾಲು = 1;
              mbf_init (caret_info.mbfile, caret_info.file);
            }
          ಅನ್ಯಥಾ
            caret_free ();
        }
    }
  ಪ್ರತ್ಯಾಗಮ !!caret_info.file;
}

/* Getc, but smash \r\n as \n.  */
ಸ್ಥಿರ ಶೂನ್ಯ
caret_getc_internal (mbchar_t *res)
{
  mbf_getc (*res, caret_info.mbfile);
  ಯದಿ (mb_iseq (*res, '\r'))
    {
      mbchar_t c;
      mbf_getc (c, caret_info.mbfile);
      ಯದಿ (mb_iseq (c, '\n'))
        mb_copy (res, &c);
      ಅನ್ಯಥಾ
        mbf_ungetc (c, caret_info.mbfile);
    }
}

#define caret_getc(Var) caret_getc_internal(&Var)

/* Move CARET_INFO (which has a valid FILE) to the line number LINE.
   Compute and cache that line's length in CARET_INFO.LINE_LEN.
   Return whether successful.  */
ಸ್ಥಿರ bool
caret_set_ಸಾಲು (ಪೂರ್ಣಾಙ್ಕ ಸಾಲು)
{
  /* If the line we want to quote is seekable (the same line as the previous
     location), just seek it. If it was a previous line, we lost track of it,
     so return to the start of file.  */
  ಯದಿ (ಸಾಲು < caret_info.pos.ಸಾಲು)
    {
      caret_info.pos.ಸಾಲು = 1;
      caret_info.offset = 0;
    }
  ಯದಿ (fseek (caret_info.file, caret_info.offset, SEEK_SET))
    ಪ್ರತ್ಯಾಗಮ false;

  /* If this is the same line as the previous one, we are done. */
  ಯದಿ (ಸಾಲು < caret_info.pos.ಸಾಲು)
    ಪ್ರತ್ಯಾಗಮ true;

  /* Advance to the line's position, keeping track of the offset.  */
  ಯಾವತ್ (caret_info.pos.ಸಾಲು < ಸಾಲು)
    {
      mbchar_t c;
      caret_getc (c);
      ಯದಿ (mb_iseof (c))
        /* Something is wrong, that line number does not exist.  */
        ಪ್ರತ್ಯಾಗಮ false;
      caret_info.pos.ಸಾಲು += mb_iseq (c, '\n');
    }
  caret_info.offset = ftell (caret_info.file);
  caret_info.pos.column = 1;
  /* Reset mbf's internal state.
     FIXME: should be done in mbfile.  */
  caret_info.mbfile.eof_seen = 0;

  /* Find the number of columns of this line.  */
  ಯಾವತ್ (true)
    {
      mbchar_t c;
      caret_getc (c);
      ಯದಿ (mb_iseof (c) || mb_iseq (c, '\n'))
        ಭಙ್ಗ;
      boundary_compute (&caret_info.pos, mb_ptr (c), mb_len (c));
    }
  caret_info.ಸಾಲು_len = caret_info.pos.column;
  /* Go back to the beginning of line.  */
  ಯದಿ (fseek (caret_info.file, caret_info.offset, SEEK_SET))
    ಪ್ರತ್ಯಾಗಮ false;
  /* Reset mbf's internal state.
     FIXME: should be done in mbfile.  */
  caret_info.mbfile.eof_seen = 0;
  caret_info.pos.column = 1;
  ಪ್ರತ್ಯಾಗಮ true;
}

/* Compute CARET_INFO.WIDTH and CARET_INFO.SKIP based on the fact that
   the first column to display in the current line is COL.  */
ಸ್ಥಿರ bool
caret_set_column (ಪೂರ್ಣಾಙ್ಕ col)
{
  /* Available width.  Eight chars are consumed by the left-margin
     (with line number).  */
  caret_info.width = screen_width - 8;
  caret_info.skip = 0;
  ಯದಿ (caret_info.width < caret_info.ಸಾಲು_len)
    {
      /* We cannot quote the whole line.  Make sure we can see the
         beginning of the location.  */
      caret_info.skip = caret_info.width < col ? col - 10 : 0;
    }
  /* If we skip the initial part, we insert "..." before.  */
  ಯದಿ (caret_info.skip)
    caret_info.width -= ellipsize;
  /* If the end of line does not fit, we also need to truncate the
     end, and leave "..." there.  */
  ಯದಿ (caret_info.width < caret_info.ಸಾಲು_len - caret_info.skip)
    caret_info.width -= ellipsize;
  ಪ್ರತ್ಯಾಗಮ true;
}

ಶೂನ್ಯ
location_caret (location loc, ಧ್ರುವ ವರ್ಣ *style, FILE *out)
{
  ಯದಿ (!(feature_flag & feature_caret))
    ಪ್ರತ್ಯಾಗಮ;
  ಯದಿ (!loc.start.ಸಾಲು)
    ಪ್ರತ್ಯಾಗಮ;
  ಯದಿ (!caret_set_file (loc.start.file))
    ಪ್ರತ್ಯಾಗಮ;
  ಯದಿ (!caret_set_ಸಾಲು (loc.start.ಸಾಲು))
    ಪ್ರತ್ಯಾಗಮ;
  ಯದಿ (!caret_set_column (loc.start.column))
    ಪ್ರತ್ಯಾಗಮ;

  ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ width = caret_info.width;
  ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ skip = caret_info.skip;

  /* Read the actual line.  Don't update the offset, so that we keep a pointer
     to the start of the line.  */
  {
    mbchar_t c;
    caret_getc (c);
    ಯದಿ (!mb_iseof (c))
      {
        /* The last column to highlight.  Only the first line of
           multiline locations are quoted, in which case the ending
           column is the end of line.

           We used to work with byte offsets, and that was much
           easier.  However, we went back to using (visual) columns to
           support truncating of long lines.  */
        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ col_end
          = loc.start.ಸಾಲು == loc.end.ಸಾಲು
          ? loc.end.column
          : caret_info.ಸಾಲು_len;
        /* Quote the file (at most the first line in the case of
           multiline locations).  */
        {
          fprintf (out, "%5d | %s", loc.start.ಸಾಲು, skip ? ellipsis : "");
          /* Whether we opened the style.  If the line is not as
             expected (maybe the file was changed since the scanner
             ran), we might reach the end before we actually saw the
             opening column.  */
          ಗಣನಾ { before, inside, after } state = before;
          ಯಾವತ್ (!mb_iseof (c) && !mb_iseq (c, '\n'))
            {
              // We might have already opened (and even closed!) the
              // style and yet have the equality of the columns if we
              // just saw zero-width characters.
              ಯದಿ (state == before
                  && caret_info.pos.column == loc.start.column)
                {
                  begin_use_class (style, out);
                  state = inside;
                }
              ಯದಿ (skip < caret_info.pos.column)
                mb_putc (c, out);
              boundary_compute (&caret_info.pos, mb_ptr (c), mb_len (c));
              caret_getc (c);
              ಯದಿ (state == inside
                  && (caret_info.pos.column == col_end
                      || width < caret_info.pos.column - skip))
                {
                  end_use_class (style, out);
                  state = after;
                }
              ಯದಿ (width < caret_info.pos.column - skip)
                {
                  fputs (ellipsis, out);
                  ಭಙ್ಗ;
                }
            }
          ಯದಿ (state == inside)
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
          ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = loc.start.column - 1 - skip + 1,
                 i_end = min_int (col_end - 1 - skip, width);
               i < i_end; ++i)
            putc ('~', out);
          end_use_class (style, out);
          putc ('\n', out);
        }
      }
  }
}

ಶೂನ್ಯ
location_caret_suggestion (location loc, ಧ್ರುವ ವರ್ಣ *s, FILE *out)
{
  ಯದಿ (!(feature_flag & feature_caret))
    ಪ್ರತ್ಯಾಗಮ;
  ಧ್ರುವ ವರ್ಣ *style = "fixit-insert";
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
  ಪ್ರತ್ಯಾಗಮ !loc.start.file && !loc.start.ಸಾಲು && !loc.start.column
    && !loc.end.file && !loc.end.ಸಾಲು && !loc.end.column;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ
str_to_int (ಧ್ರುವ ವರ್ಣ *s)
{
  long l = strtol (s, ರಿಕ್ತ, 10);
  ಪ್ರತ್ಯಾಗಮ l < 0 ? -1 : l <= INT_MAX ? l : INT_MAX;
}

ಶೂನ್ಯ
boundary_set_from_string (boundary *bound, ವರ್ಣ *str)
{
  /* Search backwards: the file name may contain '.'  or ':'.  */
  ವರ್ಣ *at = strrchr (str, '@');
  ಯದಿ (at)
    {
      *at = '\0';
      bound->byte = str_to_int (at + 1);
    }
  {
    ವರ್ಣ *dot = strrchr (str, '.');
    aver (dot);
    *dot = '\0';
    bound->column = str_to_int (dot + 1);
    ಯದಿ (!at)
      bound->byte = bound->column;
  }
  {
    ವರ್ಣ *colon = strrchr (str, ':');
    aver (colon);
    *colon = '\0';
    bound->ಸಾಲು = str_to_int (colon + 1);
  }
  bound->file = uniqstr_new (str);
}
