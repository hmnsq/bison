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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব "system.h"

#অন্তর্ভাব <mbfile.h>
#অন্তর্ভাব <mbswidth.h>
#অন্তর্ভাব <quotearg.h>
#অন্তর্ভাব <stdio.h>    /* fileno */
#অন্তর্ভাব <sys/ioctl.h>
#অন্তর্ভাব <sys/stat.h> /* fstat */
#অন্তর্ভাব <termios.h>

#ifdef WINSIZE_IN_PTEM
# include <sys/stream.h>
# include <sys/ptem.h>
#endif

#অন্তর্ভাব "complain.h"
#অন্তর্ভাব "getargs.h"
#অন্তর্ভাব "location.h"

location ধ্রুব empty_loc = EMPTY_LOCATION_INIT;

/* The terminal width.  Not less than 40.  */
স্থির পূর্ণাঙ্ক
columns (শূন্য)
{
  ধ্রুব বর্ণ *cp = getenv ("COLUMNS");
  পূর্ণাঙ্ক res = 80;
  যদি (cp && *cp)
    {
      long l = strtol (cp, রিক্ত, 10);
      res = 0 <= l && l <= INT_MAX ? l : INT_MAX;
    }
  অন্যথা
    {
#ifdef TIOCGWINSZ
      সংরচনা winsize ws;
      যদি (ioctl (STDERR_FILENO, TIOCGWINSZ, &ws) != -1
          && 0 < ws.ws_col && ws.ws_col == (size_t) ws.ws_col)
        res = ws.ws_col;
#endif
    }
  প্রত্যাগম max_int (res, 40);
}

/* Available screen width.  */
স্থির পূর্ণাঙ্ক screen_width = 80;

/* The ellipsis symbol to use for this locale, and the number of
   screen-columns it uses.  */
স্থির ধ্রুব বর্ণ *ellipsis = "...";
স্থির পূর্ণাঙ্ক ellipsize = 3;

/* If BUF is null, add BUFSIZE (which in this case must be less than
   INT_MAX) to COLUMN; otherwise, add mbsnwidth (BUF, BUFSIZE, 0) to
   COLUMN.  If an overflow occurs, return INT_MAX.  */

স্থির অন্তঃস্থ পূর্ণাঙ্ক
add_column_width (পূর্ণাঙ্ক column, বর্ণ ধ্রুব *buf, size_t bufsize)
{
  পূর্ণাঙ্ক width
    = buf ? mbsnwidth (buf, bufsize, 0)
    : INT_MAX <= bufsize ? INT_MAX
    : bufsize;
  প্রত্যাগম column <= INT_MAX - width ? column + width : INT_MAX;
}

স্থির শূন্য
boundary_compute (boundary *cur, বর্ণ ধ্রুব *token, size_t size)
{
  পূর্ণাঙ্ক সারি = cur->সারি;
  পূর্ণাঙ্ক column = cur->column;
  পূর্ণাঙ্ক byte = cur->byte;
  বর্ণ ধ্রুব *p0 = token;
  বর্ণ ধ্রুব *p = token;
  বর্ণ ধ্রুব *lim = token + size;

  প্রতি (p = token; p < lim; ++p)
    বিকল্প (*p)
      {
      পক্ষ '\n':
        সারি += সারি < INT_MAX;
        column = 1;
        byte = 1;
        p0 = p + 1;
        ভঙ্গ;

      পক্ষ '\t':
        column = add_column_width (column, p0, p - p0);
        column = add_column_width (column, রিক্ত, 8 - ((column - 1) & 7));
        p0 = p + 1;
        byte += byte < INT_MAX;
        ভঙ্গ;

      স্বভাব:
        byte += byte < INT_MAX;
        ভঙ্গ;
      }
  column = add_column_width (column, p0, p - p0);

  cur->সারি = সারি;
  cur->column = column;
  cur->byte = byte;
}


/* Set *LOC and adjust scanner cursor to account for token TOKEN of
   size SIZE.  */

শূন্য
location_compute (location *loc, boundary *cur, বর্ণ ধ্রুব *token, size_t size)
{
  loc->start = *cur;
  boundary_compute (cur, token, size);
  loc->end = *cur;

  যদি (loc->end.সারি == INT_MAX && loc->start.সারি != INT_MAX)
    complain (loc, Wother, _("line number overflow"));
  যদি (loc->end.column == INT_MAX && loc->start.column != INT_MAX)
    complain (loc, Wother, _("column number overflow"));
  /* TRANSLATORS: we are counting bytes, and there are too many.  */
  যদি (loc->end.byte == INT_MAX && loc->start.byte != INT_MAX)
    complain (loc, Wother, _("byte number overflow"));
}

স্থির পূর্ণাঙ্ক
boundary_print (boundary ধ্রুব *b, FILE *out)
{
  প্রত্যাগম fprintf (out, "%s:%d.%d@%d",
                  quotearg_n_style (3, escape_quoting_style, b->file),
                  b->সারি, b->column, b->byte);
}

পূর্ণাঙ্ক
location_print (location loc, FILE *out)
{
  পূর্ণাঙ্ক res = 0;
  যদি (location_empty (loc))
    res += fprintf (out, "(empty location)");
  অন্যথা যদি (trace_flag & trace_locations)
    {
      res += boundary_print (&loc.start, out);
      res += fprintf (out, "-");
      res += boundary_print (&loc.end, out);
    }
  অন্যথা
    {
      aver (loc.start.file);
      aver (loc.end.file);
      পূর্ণাঙ্ক end_col = 0 != loc.end.column ? loc.end.column - 1 : 0;
      res += fprintf (out, "%s",
                      quotearg_n_style (3, escape_quoting_style, loc.start.file));
      যদি (0 < loc.start.সারি)
        {
          res += fprintf (out, ":%d", loc.start.সারি);
          যদি (0 < loc.start.column)
            res += fprintf (out, ".%d", loc.start.column);
        }
      যদি (loc.start.file != loc.end.file)
        {
          res += fprintf (out, "-%s",
                          quotearg_n_style (3, escape_quoting_style,
                                            loc.end.file));
          যদি (0 < loc.end.সারি)
            {
              res += fprintf (out, ":%d", loc.end.সারি);
              যদি (0 <= end_col)
                res += fprintf (out, ".%d", end_col);
            }
        }
      অন্যথা যদি (0 < loc.end.সারি)
        {
          যদি (loc.start.সারি < loc.end.সারি)
            {
              res += fprintf (out, "-%d", loc.end.সারি);
              যদি (0 <= end_col)
                res += fprintf (out, ".%d", end_col);
            }
          অন্যথা যদি (0 <= end_col && loc.start.column < end_col)
            res += fprintf (out, "-%d", end_col);
        }
    }

  প্রত্যাগম res;
}


/* Persistent data used by location_caret to avoid reopening and rereading the
   same file all over for each error.  */
স্থির সংরচনা
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
  পূর্ণাঙ্ক সারি_len;
  /* Given the initial column to display, the offset (number of
     characters to skip at the beginning of the line). */
  পূর্ণাঙ্ক skip;

  /* Available width to quote the source file.  Eight chars are
     consumed by the left-margin (with line number).  */
  পূর্ণাঙ্ক width;
} caret_info;

শূন্য caret_init (শূন্য)
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

শূন্য
caret_free (শূন্য)
{
  যদি (caret_info.file)
    {
      fclose (caret_info.file);
      caret_info.file = রিক্ত;
    }
}

/* Open FILE for quoting, if needed, and if possible.  Return whether
   the file can quoted. */
স্থির bool
caret_set_file (ধ্রুব বর্ণ *file)
{
  /* If a different file than before, close and let the rest open
     the new one. */
  যদি (caret_info.pos.file && caret_info.pos.file != file)
    {
      caret_free ();
      caret_info.pos.file = রিক্ত;
    }
  যদি (!caret_info.pos.file)
    {
      caret_info.pos.file = file;
      যদি ((caret_info.file = fopen (caret_info.pos.file, "rb")))
        {
          /* If the file is not regular (imagine #line 1 "/dev/stdin"
             in the input file for instance), don't try to quote the
             file.  Keep caret_info.pos.file set so that we don't try
             to open it again, but leave caret_info.file NULL so that
             we don't try to quote it. */
          সংরচনা stat buf;
          যদি (fstat (fileno (caret_info.file), &buf) == 0
              && buf.st_mode & S_IFREG)
            {
              caret_info.pos.সারি = 1;
              mbf_init (caret_info.mbfile, caret_info.file);
            }
          অন্যথা
            caret_free ();
        }
    }
  প্রত্যাগম !!caret_info.file;
}

/* Getc, but smash \r\n as \n.  */
স্থির শূন্য
caret_getc_internal (mbchar_t *res)
{
  mbf_getc (*res, caret_info.mbfile);
  যদি (mb_iseq (*res, '\r'))
    {
      mbchar_t c;
      mbf_getc (c, caret_info.mbfile);
      যদি (mb_iseq (c, '\n'))
        mb_copy (res, &c);
      অন্যথা
        mbf_ungetc (c, caret_info.mbfile);
    }
}

#define caret_getc(Var) caret_getc_internal(&Var)

/* Move CARET_INFO (which has a valid FILE) to the line number LINE.
   Compute and cache that line's length in CARET_INFO.LINE_LEN.
   Return whether successful.  */
স্থির bool
caret_set_সারি (পূর্ণাঙ্ক সারি)
{
  /* If the line we want to quote is seekable (the same line as the previous
     location), just seek it. If it was a previous line, we lost track of it,
     so return to the start of file.  */
  যদি (সারি < caret_info.pos.সারি)
    {
      caret_info.pos.সারি = 1;
      caret_info.offset = 0;
    }
  যদি (fseek (caret_info.file, caret_info.offset, SEEK_SET))
    প্রত্যাগম false;

  /* If this is the same line as the previous one, we are done. */
  যদি (সারি < caret_info.pos.সারি)
    প্রত্যাগম true;

  /* Advance to the line's position, keeping track of the offset.  */
  যাবত্ (caret_info.pos.সারি < সারি)
    {
      mbchar_t c;
      caret_getc (c);
      যদি (mb_iseof (c))
        /* Something is wrong, that line number does not exist.  */
        প্রত্যাগম false;
      caret_info.pos.সারি += mb_iseq (c, '\n');
    }
  caret_info.offset = ftell (caret_info.file);
  caret_info.pos.column = 1;
  /* Reset mbf's internal state.
     FIXME: should be done in mbfile.  */
  caret_info.mbfile.eof_seen = 0;

  /* Find the number of columns of this line.  */
  যাবত্ (true)
    {
      mbchar_t c;
      caret_getc (c);
      যদি (mb_iseof (c) || mb_iseq (c, '\n'))
        ভঙ্গ;
      boundary_compute (&caret_info.pos, mb_ptr (c), mb_len (c));
    }
  caret_info.সারি_len = caret_info.pos.column;
  /* Go back to the beginning of line.  */
  যদি (fseek (caret_info.file, caret_info.offset, SEEK_SET))
    প্রত্যাগম false;
  /* Reset mbf's internal state.
     FIXME: should be done in mbfile.  */
  caret_info.mbfile.eof_seen = 0;
  caret_info.pos.column = 1;
  প্রত্যাগম true;
}

/* Compute CARET_INFO.WIDTH and CARET_INFO.SKIP based on the fact that
   the first column to display in the current line is COL.  */
স্থির bool
caret_set_column (পূর্ণাঙ্ক col)
{
  /* Available width.  Eight chars are consumed by the left-margin
     (with line number).  */
  caret_info.width = screen_width - 8;
  caret_info.skip = 0;
  যদি (caret_info.width < caret_info.সারি_len)
    {
      /* We cannot quote the whole line.  Make sure we can see the
         beginning of the location.  */
      caret_info.skip = caret_info.width < col ? col - 10 : 0;
    }
  /* If we skip the initial part, we insert "..." before.  */
  যদি (caret_info.skip)
    caret_info.width -= ellipsize;
  /* If the end of line does not fit, we also need to truncate the
     end, and leave "..." there.  */
  যদি (caret_info.width < caret_info.সারি_len - caret_info.skip)
    caret_info.width -= ellipsize;
  প্রত্যাগম true;
}

শূন্য
location_caret (location loc, ধ্রুব বর্ণ *style, FILE *out)
{
  যদি (!(feature_flag & feature_caret))
    প্রত্যাগম;
  যদি (!loc.start.সারি)
    প্রত্যাগম;
  যদি (!caret_set_file (loc.start.file))
    প্রত্যাগম;
  যদি (!caret_set_সারি (loc.start.সারি))
    প্রত্যাগম;
  যদি (!caret_set_column (loc.start.column))
    প্রত্যাগম;

  ধ্রুব পূর্ণাঙ্ক width = caret_info.width;
  ধ্রুব পূর্ণাঙ্ক skip = caret_info.skip;

  /* Read the actual line.  Don't update the offset, so that we keep a pointer
     to the start of the line.  */
  {
    mbchar_t c;
    caret_getc (c);
    যদি (!mb_iseof (c))
      {
        /* The last column to highlight.  Only the first line of
           multiline locations are quoted, in which case the ending
           column is the end of line.

           We used to work with byte offsets, and that was much
           easier.  However, we went back to using (visual) columns to
           support truncating of long lines.  */
        ধ্রুব পূর্ণাঙ্ক col_end
          = loc.start.সারি == loc.end.সারি
          ? loc.end.column
          : caret_info.সারি_len;
        /* Quote the file (at most the first line in the case of
           multiline locations).  */
        {
          fprintf (out, "%5d | %s", loc.start.সারি, skip ? ellipsis : "");
          /* Whether we opened the style.  If the line is not as
             expected (maybe the file was changed since the scanner
             ran), we might reach the end before we actually saw the
             opening column.  */
          গণনা { before, inside, after } state = before;
          যাবত্ (!mb_iseof (c) && !mb_iseq (c, '\n'))
            {
              // We might have already opened (and even closed!) the
              // style and yet have the equality of the columns if we
              // just saw zero-width characters.
              যদি (state == before
                  && caret_info.pos.column == loc.start.column)
                {
                  begin_use_class (style, out);
                  state = inside;
                }
              যদি (skip < caret_info.pos.column)
                mb_putc (c, out);
              boundary_compute (&caret_info.pos, mb_ptr (c), mb_len (c));
              caret_getc (c);
              যদি (state == inside
                  && (caret_info.pos.column == col_end
                      || width < caret_info.pos.column - skip))
                {
                  end_use_class (style, out);
                  state = after;
                }
              যদি (width < caret_info.pos.column - skip)
                {
                  fputs (ellipsis, out);
                  ভঙ্গ;
                }
            }
          যদি (state == inside)
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
          প্রতি (পূর্ণাঙ্ক i = loc.start.column - 1 - skip + 1,
                 i_end = min_int (col_end - 1 - skip, width);
               i < i_end; ++i)
            putc ('~', out);
          end_use_class (style, out);
          putc ('\n', out);
        }
      }
  }
}

শূন্য
location_caret_suggestion (location loc, ধ্রুব বর্ণ *s, FILE *out)
{
  যদি (!(feature_flag & feature_caret))
    প্রত্যাগম;
  ধ্রুব বর্ণ *style = "fixit-insert";
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
  প্রত্যাগম !loc.start.file && !loc.start.সারি && !loc.start.column
    && !loc.end.file && !loc.end.সারি && !loc.end.column;
}

স্থির অন্তঃস্থ পূর্ণাঙ্ক
str_to_int (ধ্রুব বর্ণ *s)
{
  long l = strtol (s, রিক্ত, 10);
  প্রত্যাগম l < 0 ? -1 : l <= INT_MAX ? l : INT_MAX;
}

শূন্য
boundary_set_from_string (boundary *bound, বর্ণ *str)
{
  /* Search backwards: the file name may contain '.'  or ':'.  */
  বর্ণ *at = strrchr (str, '@');
  যদি (at)
    {
      *at = '\0';
      bound->byte = str_to_int (at + 1);
    }
  {
    বর্ণ *dot = strrchr (str, '.');
    aver (dot);
    *dot = '\0';
    bound->column = str_to_int (dot + 1);
    যদি (!at)
      bound->byte = bound->column;
  }
  {
    বর্ণ *colon = strrchr (str, ':');
    aver (colon);
    *colon = '\0';
    bound->সারি = str_to_int (colon + 1);
  }
  bound->file = uniqstr_new (str);
}
