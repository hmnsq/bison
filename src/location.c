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

#شامل <config.h>
#شامل "system.h"

#شامل <mbfile.h>
#شامل <mbswidth.h>
#شامل <quotearg.h>
#شامل <stdio.h>    /* fileno */
#شامل <sys/ioctl.h>
#شامل <sys/stat.h> /* fstat */
#شامل <termios.h>

#ifdef WINSIZE_IN_PTEM
# include <sys/stream.h>
# include <sys/ptem.h>
#endif

#شامل "complain.h"
#شامل "getargs.h"
#شامل "location.h"

location ثابت empty_loc = EMPTY_LOCATION_INIT;

/* The terminal width.  Not less than 40.  */
ایستا صحیح
columns (تهی)
{
  ثابت نویسه *cp = getenv ("COLUMNS");
  صحیح res = 80;
  اگر (cp && *cp)
    {
      long l = strtol (cp, پوچ, 10);
      res = 0 <= l && l <= INT_MAX ? l : INT_MAX;
    }
  وگرنه
    {
#ifdef TIOCGWINSZ
      ساختار winsize ws;
      اگر (ioctl (STDERR_FILENO, TIOCGWINSZ, &ws) != -1
          && 0 < ws.ws_col && ws.ws_col == (size_t) ws.ws_col)
        res = ws.ws_col;
#endif
    }
  بازگشت max_int (res, 40);
}

/* Available screen width.  */
ایستا صحیح screen_width = 80;

/* The ellipsis symbol to use for this locale, and the number of
   screen-columns it uses.  */
ایستا ثابت نویسه *ellipsis = "...";
ایستا صحیح ellipsize = 3;

/* If BUF is null, add BUFSIZE (which in this case must be less than
   INT_MAX) to COLUMN; otherwise, add mbsnwidth (BUF, BUFSIZE, 0) to
   COLUMN.  If an overflow occurs, return INT_MAX.  */

ایستا درونخطی صحیح
add_column_width (صحیح column, نویسه ثابت *buf, size_t bufsize)
{
  صحیح width
    = buf ? mbsnwidth (buf, bufsize, 0)
    : INT_MAX <= bufsize ? INT_MAX
    : bufsize;
  بازگشت column <= INT_MAX - width ? column + width : INT_MAX;
}

ایستا تهی
boundary_compute (boundary *cur, نویسه ثابت *token, size_t size)
{
  صحیح سطر = cur->سطر;
  صحیح column = cur->column;
  صحیح byte = cur->byte;
  نویسه ثابت *p0 = token;
  نویسه ثابت *p = token;
  نویسه ثابت *lim = token + size;

  برای (p = token; p < lim; ++p)
    انتخاب (*p)
      {
      حالت '\n':
        سطر += سطر < INT_MAX;
        column = 1;
        byte = 1;
        p0 = p + 1;
        بشکن;

      حالت '\t':
        column = add_column_width (column, p0, p - p0);
        column = add_column_width (column, پوچ, 8 - ((column - 1) & 7));
        p0 = p + 1;
        byte += byte < INT_MAX;
        بشکن;

      پیشفرض:
        byte += byte < INT_MAX;
        بشکن;
      }
  column = add_column_width (column, p0, p - p0);

  cur->سطر = سطر;
  cur->column = column;
  cur->byte = byte;
}


/* Set *LOC and adjust scanner cursor to account for token TOKEN of
   size SIZE.  */

تهی
location_compute (location *loc, boundary *cur, نویسه ثابت *token, size_t size)
{
  loc->start = *cur;
  boundary_compute (cur, token, size);
  loc->end = *cur;

  اگر (loc->end.سطر == INT_MAX && loc->start.سطر != INT_MAX)
    complain (loc, Wother, _("line number overflow"));
  اگر (loc->end.column == INT_MAX && loc->start.column != INT_MAX)
    complain (loc, Wother, _("column number overflow"));
  /* TRANSLATORS: we are counting bytes, and there are too many.  */
  اگر (loc->end.byte == INT_MAX && loc->start.byte != INT_MAX)
    complain (loc, Wother, _("byte number overflow"));
}

ایستا صحیح
boundary_print (boundary ثابت *b, FILE *out)
{
  بازگشت fprintf (out, "%s:%d.%d@%d",
                  quotearg_n_style (3, escape_quoting_style, b->file),
                  b->سطر, b->column, b->byte);
}

صحیح
location_print (location loc, FILE *out)
{
  صحیح res = 0;
  اگر (location_empty (loc))
    res += fprintf (out, "(empty location)");
  وگرنه اگر (trace_flag & trace_locations)
    {
      res += boundary_print (&loc.start, out);
      res += fprintf (out, "-");
      res += boundary_print (&loc.end, out);
    }
  وگرنه
    {
      aver (loc.start.file);
      aver (loc.end.file);
      صحیح end_col = 0 != loc.end.column ? loc.end.column - 1 : 0;
      res += fprintf (out, "%s",
                      quotearg_n_style (3, escape_quoting_style, loc.start.file));
      اگر (0 < loc.start.سطر)
        {
          res += fprintf (out, ":%d", loc.start.سطر);
          اگر (0 < loc.start.column)
            res += fprintf (out, ".%d", loc.start.column);
        }
      اگر (loc.start.file != loc.end.file)
        {
          res += fprintf (out, "-%s",
                          quotearg_n_style (3, escape_quoting_style,
                                            loc.end.file));
          اگر (0 < loc.end.سطر)
            {
              res += fprintf (out, ":%d", loc.end.سطر);
              اگر (0 <= end_col)
                res += fprintf (out, ".%d", end_col);
            }
        }
      وگرنه اگر (0 < loc.end.سطر)
        {
          اگر (loc.start.سطر < loc.end.سطر)
            {
              res += fprintf (out, "-%d", loc.end.سطر);
              اگر (0 <= end_col)
                res += fprintf (out, ".%d", end_col);
            }
          وگرنه اگر (0 <= end_col && loc.start.column < end_col)
            res += fprintf (out, "-%d", end_col);
        }
    }

  بازگشت res;
}


/* Persistent data used by location_caret to avoid reopening and rereading the
   same file all over for each error.  */
ایستا ساختار
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
  صحیح سطر_len;
  /* Given the initial column to display, the offset (number of
     characters to skip at the beginning of the line). */
  صحیح skip;

  /* Available width to quote the source file.  Eight chars are
     consumed by the left-margin (with line number).  */
  صحیح width;
} caret_info;

تهی caret_init (تهی)
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

تهی
caret_free (تهی)
{
  اگر (caret_info.file)
    {
      fclose (caret_info.file);
      caret_info.file = پوچ;
    }
}

/* Open FILE for quoting, if needed, and if possible.  Return whether
   the file can quoted. */
ایستا bool
caret_set_file (ثابت نویسه *file)
{
  /* If a different file than before, close and let the rest open
     the new one. */
  اگر (caret_info.pos.file && caret_info.pos.file != file)
    {
      caret_free ();
      caret_info.pos.file = پوچ;
    }
  اگر (!caret_info.pos.file)
    {
      caret_info.pos.file = file;
      اگر ((caret_info.file = fopen (caret_info.pos.file, "rb")))
        {
          /* If the file is not regular (imagine #line 1 "/dev/stdin"
             in the input file for instance), don't try to quote the
             file.  Keep caret_info.pos.file set so that we don't try
             to open it again, but leave caret_info.file NULL so that
             we don't try to quote it. */
          ساختار stat buf;
          اگر (fstat (fileno (caret_info.file), &buf) == 0
              && buf.st_mode & S_IFREG)
            {
              caret_info.pos.سطر = 1;
              mbf_init (caret_info.mbfile, caret_info.file);
            }
          وگرنه
            caret_free ();
        }
    }
  بازگشت !!caret_info.file;
}

/* Getc, but smash \r\n as \n.  */
ایستا تهی
caret_getc_internal (mbchar_t *res)
{
  mbf_getc (*res, caret_info.mbfile);
  اگر (mb_iseq (*res, '\r'))
    {
      mbchar_t c;
      mbf_getc (c, caret_info.mbfile);
      اگر (mb_iseq (c, '\n'))
        mb_copy (res, &c);
      وگرنه
        mbf_ungetc (c, caret_info.mbfile);
    }
}

#define caret_getc(Var) caret_getc_internal(&Var)

/* Move CARET_INFO (which has a valid FILE) to the line number LINE.
   Compute and cache that line's length in CARET_INFO.LINE_LEN.
   Return whether successful.  */
ایستا bool
caret_set_سطر (صحیح سطر)
{
  /* If the line we want to quote is seekable (the same line as the previous
     location), just seek it. If it was a previous line, we lost track of it,
     so return to the start of file.  */
  اگر (سطر < caret_info.pos.سطر)
    {
      caret_info.pos.سطر = 1;
      caret_info.offset = 0;
    }
  اگر (fseek (caret_info.file, caret_info.offset, SEEK_SET))
    بازگشت false;

  /* If this is the same line as the previous one, we are done. */
  اگر (سطر < caret_info.pos.سطر)
    بازگشت true;

  /* Advance to the line's position, keeping track of the offset.  */
  وقتی (caret_info.pos.سطر < سطر)
    {
      mbchar_t c;
      caret_getc (c);
      اگر (mb_iseof (c))
        /* Something is wrong, that line number does not exist.  */
        بازگشت false;
      caret_info.pos.سطر += mb_iseq (c, '\n');
    }
  caret_info.offset = ftell (caret_info.file);
  caret_info.pos.column = 1;
  /* Reset mbf's internal state.
     FIXME: should be done in mbfile.  */
  caret_info.mbfile.eof_seen = 0;

  /* Find the number of columns of this line.  */
  وقتی (true)
    {
      mbchar_t c;
      caret_getc (c);
      اگر (mb_iseof (c) || mb_iseq (c, '\n'))
        بشکن;
      boundary_compute (&caret_info.pos, mb_ptr (c), mb_len (c));
    }
  caret_info.سطر_len = caret_info.pos.column;
  /* Go back to the beginning of line.  */
  اگر (fseek (caret_info.file, caret_info.offset, SEEK_SET))
    بازگشت false;
  /* Reset mbf's internal state.
     FIXME: should be done in mbfile.  */
  caret_info.mbfile.eof_seen = 0;
  caret_info.pos.column = 1;
  بازگشت true;
}

/* Compute CARET_INFO.WIDTH and CARET_INFO.SKIP based on the fact that
   the first column to display in the current line is COL.  */
ایستا bool
caret_set_column (صحیح col)
{
  /* Available width.  Eight chars are consumed by the left-margin
     (with line number).  */
  caret_info.width = screen_width - 8;
  caret_info.skip = 0;
  اگر (caret_info.width < caret_info.سطر_len)
    {
      /* We cannot quote the whole line.  Make sure we can see the
         beginning of the location.  */
      caret_info.skip = caret_info.width < col ? col - 10 : 0;
    }
  /* If we skip the initial part, we insert "..." before.  */
  اگر (caret_info.skip)
    caret_info.width -= ellipsize;
  /* If the end of line does not fit, we also need to truncate the
     end, and leave "..." there.  */
  اگر (caret_info.width < caret_info.سطر_len - caret_info.skip)
    caret_info.width -= ellipsize;
  بازگشت true;
}

تهی
location_caret (location loc, ثابت نویسه *style, FILE *out)
{
  اگر (!(feature_flag & feature_caret))
    بازگشت;
  اگر (!loc.start.سطر)
    بازگشت;
  اگر (!caret_set_file (loc.start.file))
    بازگشت;
  اگر (!caret_set_سطر (loc.start.سطر))
    بازگشت;
  اگر (!caret_set_column (loc.start.column))
    بازگشت;

  ثابت صحیح width = caret_info.width;
  ثابت صحیح skip = caret_info.skip;

  /* Read the actual line.  Don't update the offset, so that we keep a pointer
     to the start of the line.  */
  {
    mbchar_t c;
    caret_getc (c);
    اگر (!mb_iseof (c))
      {
        /* The last column to highlight.  Only the first line of
           multiline locations are quoted, in which case the ending
           column is the end of line.

           We used to work with byte offsets, and that was much
           easier.  However, we went back to using (visual) columns to
           support truncating of long lines.  */
        ثابت صحیح col_end
          = loc.start.سطر == loc.end.سطر
          ? loc.end.column
          : caret_info.سطر_len;
        /* Quote the file (at most the first line in the case of
           multiline locations).  */
        {
          fprintf (out, "%5d | %s", loc.start.سطر, skip ? ellipsis : "");
          /* Whether we opened the style.  If the line is not as
             expected (maybe the file was changed since the scanner
             ran), we might reach the end before we actually saw the
             opening column.  */
          شمارش { before, inside, after } state = before;
          وقتی (!mb_iseof (c) && !mb_iseq (c, '\n'))
            {
              // We might have already opened (and even closed!) the
              // style and yet have the equality of the columns if we
              // just saw zero-width characters.
              اگر (state == before
                  && caret_info.pos.column == loc.start.column)
                {
                  begin_use_class (style, out);
                  state = inside;
                }
              اگر (skip < caret_info.pos.column)
                mb_putc (c, out);
              boundary_compute (&caret_info.pos, mb_ptr (c), mb_len (c));
              caret_getc (c);
              اگر (state == inside
                  && (caret_info.pos.column == col_end
                      || width < caret_info.pos.column - skip))
                {
                  end_use_class (style, out);
                  state = after;
                }
              اگر (width < caret_info.pos.column - skip)
                {
                  fputs (ellipsis, out);
                  بشکن;
                }
            }
          اگر (state == inside)
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
          برای (صحیح i = loc.start.column - 1 - skip + 1,
                 i_end = min_int (col_end - 1 - skip, width);
               i < i_end; ++i)
            putc ('~', out);
          end_use_class (style, out);
          putc ('\n', out);
        }
      }
  }
}

تهی
location_caret_suggestion (location loc, ثابت نویسه *s, FILE *out)
{
  اگر (!(feature_flag & feature_caret))
    بازگشت;
  ثابت نویسه *style = "fixit-insert";
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
  بازگشت !loc.start.file && !loc.start.سطر && !loc.start.column
    && !loc.end.file && !loc.end.سطر && !loc.end.column;
}

ایستا درونخطی صحیح
str_to_int (ثابت نویسه *s)
{
  long l = strtol (s, پوچ, 10);
  بازگشت l < 0 ? -1 : l <= INT_MAX ? l : INT_MAX;
}

تهی
boundary_set_from_string (boundary *bound, نویسه *str)
{
  /* Search backwards: the file name may contain '.'  or ':'.  */
  نویسه *at = strrchr (str, '@');
  اگر (at)
    {
      *at = '\0';
      bound->byte = str_to_int (at + 1);
    }
  {
    نویسه *dot = strrchr (str, '.');
    aver (dot);
    *dot = '\0';
    bound->column = str_to_int (dot + 1);
    اگر (!at)
      bound->byte = bound->column;
  }
  {
    نویسه *colon = strrchr (str, ':');
    aver (colon);
    *colon = '\0';
    bound->سطر = str_to_int (colon + 1);
  }
  bound->file = uniqstr_new (str);
}
