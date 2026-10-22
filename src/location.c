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

#अन्तर्भाव <config.h>
#अन्तर्भाव "system.h"

#अन्तर्भाव <mbfile.h>
#अन्तर्भाव <mbswidth.h>
#अन्तर्भाव <quotearg.h>
#अन्तर्भाव <stdio.h>    /* fileno */
#अन्तर्भाव <sys/ioctl.h>
#अन्तर्भाव <sys/stat.h> /* fstat */
#अन्तर्भाव <termios.h>

#ifdef WINSIZE_IN_PTEM
# include <sys/stream.h>
# include <sys/ptem.h>
#endif

#अन्तर्भाव "complain.h"
#अन्तर्भाव "getargs.h"
#अन्तर्भाव "location.h"

location ध्रुव empty_loc = EMPTY_LOCATION_INIT;

/* The terminal width.  Not less than 40.  */
स्थिर पूर्णाङ्क
columns (शून्य)
{
  ध्रुव वर्ण *cp = getenv ("COLUMNS");
  पूर्णाङ्क res = 80;
  यदि (cp && *cp)
    {
      long l = strtol (cp, रिक्त, 10);
      res = 0 <= l && l <= INT_MAX ? l : INT_MAX;
    }
  अन्यथा
    {
#ifdef TIOCGWINSZ
      संरचना winsize ws;
      यदि (ioctl (STDERR_FILENO, TIOCGWINSZ, &ws) != -1
          && 0 < ws.ws_col && ws.ws_col == (size_t) ws.ws_col)
        res = ws.ws_col;
#endif
    }
  प्रत्यागम max_int (res, 40);
}

/* Available screen width.  */
स्थिर पूर्णाङ्क screen_width = 80;

/* The ellipsis symbol to use for this locale, and the number of
   screen-columns it uses.  */
स्थिर ध्रुव वर्ण *ellipsis = "...";
स्थिर पूर्णाङ्क ellipsize = 3;

/* If BUF is null, add BUFSIZE (which in this case must be less than
   INT_MAX) to COLUMN; otherwise, add mbsnwidth (BUF, BUFSIZE, 0) to
   COLUMN.  If an overflow occurs, return INT_MAX.  */

स्थिर अन्तःस्थ पूर्णाङ्क
add_column_width (पूर्णाङ्क column, वर्ण ध्रुव *buf, size_t bufsize)
{
  पूर्णाङ्क width
    = buf ? mbsnwidth (buf, bufsize, 0)
    : INT_MAX <= bufsize ? INT_MAX
    : bufsize;
  प्रत्यागम column <= INT_MAX - width ? column + width : INT_MAX;
}

स्थिर शून्य
boundary_compute (boundary *cur, वर्ण ध्रुव *token, size_t size)
{
  पूर्णाङ्क ओळ = cur->ओळ;
  पूर्णाङ्क column = cur->column;
  पूर्णाङ्क byte = cur->byte;
  वर्ण ध्रुव *p0 = token;
  वर्ण ध्रुव *p = token;
  वर्ण ध्रुव *lim = token + size;

  प्रति (p = token; p < lim; ++p)
    विकल्प (*p)
      {
      पक्ष '\n':
        ओळ += ओळ < INT_MAX;
        column = 1;
        byte = 1;
        p0 = p + 1;
        भङ्ग;

      पक्ष '\t':
        column = add_column_width (column, p0, p - p0);
        column = add_column_width (column, रिक्त, 8 - ((column - 1) & 7));
        p0 = p + 1;
        byte += byte < INT_MAX;
        भङ्ग;

      स्वभाव:
        byte += byte < INT_MAX;
        भङ्ग;
      }
  column = add_column_width (column, p0, p - p0);

  cur->ओळ = ओळ;
  cur->column = column;
  cur->byte = byte;
}


/* Set *LOC and adjust scanner cursor to account for token TOKEN of
   size SIZE.  */

शून्य
location_compute (location *loc, boundary *cur, वर्ण ध्रुव *token, size_t size)
{
  loc->start = *cur;
  boundary_compute (cur, token, size);
  loc->end = *cur;

  यदि (loc->end.ओळ == INT_MAX && loc->start.ओळ != INT_MAX)
    complain (loc, Wother, _("line number overflow"));
  यदि (loc->end.column == INT_MAX && loc->start.column != INT_MAX)
    complain (loc, Wother, _("column number overflow"));
  /* TRANSLATORS: we are counting bytes, and there are too many.  */
  यदि (loc->end.byte == INT_MAX && loc->start.byte != INT_MAX)
    complain (loc, Wother, _("byte number overflow"));
}

स्थिर पूर्णाङ्क
boundary_print (boundary ध्रुव *b, FILE *out)
{
  प्रत्यागम fprintf (out, "%s:%d.%d@%d",
                  quotearg_n_style (3, escape_quoting_style, b->file),
                  b->ओळ, b->column, b->byte);
}

पूर्णाङ्क
location_print (location loc, FILE *out)
{
  पूर्णाङ्क res = 0;
  यदि (location_empty (loc))
    res += fprintf (out, "(empty location)");
  अन्यथा यदि (trace_flag & trace_locations)
    {
      res += boundary_print (&loc.start, out);
      res += fprintf (out, "-");
      res += boundary_print (&loc.end, out);
    }
  अन्यथा
    {
      aver (loc.start.file);
      aver (loc.end.file);
      पूर्णाङ्क end_col = 0 != loc.end.column ? loc.end.column - 1 : 0;
      res += fprintf (out, "%s",
                      quotearg_n_style (3, escape_quoting_style, loc.start.file));
      यदि (0 < loc.start.ओळ)
        {
          res += fprintf (out, ":%d", loc.start.ओळ);
          यदि (0 < loc.start.column)
            res += fprintf (out, ".%d", loc.start.column);
        }
      यदि (loc.start.file != loc.end.file)
        {
          res += fprintf (out, "-%s",
                          quotearg_n_style (3, escape_quoting_style,
                                            loc.end.file));
          यदि (0 < loc.end.ओळ)
            {
              res += fprintf (out, ":%d", loc.end.ओळ);
              यदि (0 <= end_col)
                res += fprintf (out, ".%d", end_col);
            }
        }
      अन्यथा यदि (0 < loc.end.ओळ)
        {
          यदि (loc.start.ओळ < loc.end.ओळ)
            {
              res += fprintf (out, "-%d", loc.end.ओळ);
              यदि (0 <= end_col)
                res += fprintf (out, ".%d", end_col);
            }
          अन्यथा यदि (0 <= end_col && loc.start.column < end_col)
            res += fprintf (out, "-%d", end_col);
        }
    }

  प्रत्यागम res;
}


/* Persistent data used by location_caret to avoid reopening and rereading the
   same file all over for each error.  */
स्थिर संरचना
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
  पूर्णाङ्क ओळ_len;
  /* Given the initial column to display, the offset (number of
     characters to skip at the beginning of the line). */
  पूर्णाङ्क skip;

  /* Available width to quote the source file.  Eight chars are
     consumed by the left-margin (with line number).  */
  पूर्णाङ्क width;
} caret_info;

शून्य caret_init (शून्य)
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

शून्य
caret_free (शून्य)
{
  यदि (caret_info.file)
    {
      fclose (caret_info.file);
      caret_info.file = रिक्त;
    }
}

/* Open FILE for quoting, if needed, and if possible.  Return whether
   the file can quoted. */
स्थिर bool
caret_set_file (ध्रुव वर्ण *file)
{
  /* If a different file than before, close and let the rest open
     the new one. */
  यदि (caret_info.pos.file && caret_info.pos.file != file)
    {
      caret_free ();
      caret_info.pos.file = रिक्त;
    }
  यदि (!caret_info.pos.file)
    {
      caret_info.pos.file = file;
      यदि ((caret_info.file = fopen (caret_info.pos.file, "rb")))
        {
          /* If the file is not regular (imagine #line 1 "/dev/stdin"
             in the input file for instance), don't try to quote the
             file.  Keep caret_info.pos.file set so that we don't try
             to open it again, but leave caret_info.file NULL so that
             we don't try to quote it. */
          संरचना stat buf;
          यदि (fstat (fileno (caret_info.file), &buf) == 0
              && buf.st_mode & S_IFREG)
            {
              caret_info.pos.ओळ = 1;
              mbf_init (caret_info.mbfile, caret_info.file);
            }
          अन्यथा
            caret_free ();
        }
    }
  प्रत्यागम !!caret_info.file;
}

/* Getc, but smash \r\n as \n.  */
स्थिर शून्य
caret_getc_internal (mbchar_t *res)
{
  mbf_getc (*res, caret_info.mbfile);
  यदि (mb_iseq (*res, '\r'))
    {
      mbchar_t c;
      mbf_getc (c, caret_info.mbfile);
      यदि (mb_iseq (c, '\n'))
        mb_copy (res, &c);
      अन्यथा
        mbf_ungetc (c, caret_info.mbfile);
    }
}

#define caret_getc(Var) caret_getc_internal(&Var)

/* Move CARET_INFO (which has a valid FILE) to the line number LINE.
   Compute and cache that line's length in CARET_INFO.LINE_LEN.
   Return whether successful.  */
स्थिर bool
caret_set_ओळ (पूर्णाङ्क ओळ)
{
  /* If the line we want to quote is seekable (the same line as the previous
     location), just seek it. If it was a previous line, we lost track of it,
     so return to the start of file.  */
  यदि (ओळ < caret_info.pos.ओळ)
    {
      caret_info.pos.ओळ = 1;
      caret_info.offset = 0;
    }
  यदि (fseek (caret_info.file, caret_info.offset, SEEK_SET))
    प्रत्यागम false;

  /* If this is the same line as the previous one, we are done. */
  यदि (ओळ < caret_info.pos.ओळ)
    प्रत्यागम true;

  /* Advance to the line's position, keeping track of the offset.  */
  यावत् (caret_info.pos.ओळ < ओळ)
    {
      mbchar_t c;
      caret_getc (c);
      यदि (mb_iseof (c))
        /* Something is wrong, that line number does not exist.  */
        प्रत्यागम false;
      caret_info.pos.ओळ += mb_iseq (c, '\n');
    }
  caret_info.offset = ftell (caret_info.file);
  caret_info.pos.column = 1;
  /* Reset mbf's internal state.
     FIXME: should be done in mbfile.  */
  caret_info.mbfile.eof_seen = 0;

  /* Find the number of columns of this line.  */
  यावत् (true)
    {
      mbchar_t c;
      caret_getc (c);
      यदि (mb_iseof (c) || mb_iseq (c, '\n'))
        भङ्ग;
      boundary_compute (&caret_info.pos, mb_ptr (c), mb_len (c));
    }
  caret_info.ओळ_len = caret_info.pos.column;
  /* Go back to the beginning of line.  */
  यदि (fseek (caret_info.file, caret_info.offset, SEEK_SET))
    प्रत्यागम false;
  /* Reset mbf's internal state.
     FIXME: should be done in mbfile.  */
  caret_info.mbfile.eof_seen = 0;
  caret_info.pos.column = 1;
  प्रत्यागम true;
}

/* Compute CARET_INFO.WIDTH and CARET_INFO.SKIP based on the fact that
   the first column to display in the current line is COL.  */
स्थिर bool
caret_set_column (पूर्णाङ्क col)
{
  /* Available width.  Eight chars are consumed by the left-margin
     (with line number).  */
  caret_info.width = screen_width - 8;
  caret_info.skip = 0;
  यदि (caret_info.width < caret_info.ओळ_len)
    {
      /* We cannot quote the whole line.  Make sure we can see the
         beginning of the location.  */
      caret_info.skip = caret_info.width < col ? col - 10 : 0;
    }
  /* If we skip the initial part, we insert "..." before.  */
  यदि (caret_info.skip)
    caret_info.width -= ellipsize;
  /* If the end of line does not fit, we also need to truncate the
     end, and leave "..." there.  */
  यदि (caret_info.width < caret_info.ओळ_len - caret_info.skip)
    caret_info.width -= ellipsize;
  प्रत्यागम true;
}

शून्य
location_caret (location loc, ध्रुव वर्ण *style, FILE *out)
{
  यदि (!(feature_flag & feature_caret))
    प्रत्यागम;
  यदि (!loc.start.ओळ)
    प्रत्यागम;
  यदि (!caret_set_file (loc.start.file))
    प्रत्यागम;
  यदि (!caret_set_ओळ (loc.start.ओळ))
    प्रत्यागम;
  यदि (!caret_set_column (loc.start.column))
    प्रत्यागम;

  ध्रुव पूर्णाङ्क width = caret_info.width;
  ध्रुव पूर्णाङ्क skip = caret_info.skip;

  /* Read the actual line.  Don't update the offset, so that we keep a pointer
     to the start of the line.  */
  {
    mbchar_t c;
    caret_getc (c);
    यदि (!mb_iseof (c))
      {
        /* The last column to highlight.  Only the first line of
           multiline locations are quoted, in which case the ending
           column is the end of line.

           We used to work with byte offsets, and that was much
           easier.  However, we went back to using (visual) columns to
           support truncating of long lines.  */
        ध्रुव पूर्णाङ्क col_end
          = loc.start.ओळ == loc.end.ओळ
          ? loc.end.column
          : caret_info.ओळ_len;
        /* Quote the file (at most the first line in the case of
           multiline locations).  */
        {
          fprintf (out, "%5d | %s", loc.start.ओळ, skip ? ellipsis : "");
          /* Whether we opened the style.  If the line is not as
             expected (maybe the file was changed since the scanner
             ran), we might reach the end before we actually saw the
             opening column.  */
          गणना { before, inside, after } state = before;
          यावत् (!mb_iseof (c) && !mb_iseq (c, '\n'))
            {
              // We might have already opened (and even closed!) the
              // style and yet have the equality of the columns if we
              // just saw zero-width characters.
              यदि (state == before
                  && caret_info.pos.column == loc.start.column)
                {
                  begin_use_class (style, out);
                  state = inside;
                }
              यदि (skip < caret_info.pos.column)
                mb_putc (c, out);
              boundary_compute (&caret_info.pos, mb_ptr (c), mb_len (c));
              caret_getc (c);
              यदि (state == inside
                  && (caret_info.pos.column == col_end
                      || width < caret_info.pos.column - skip))
                {
                  end_use_class (style, out);
                  state = after;
                }
              यदि (width < caret_info.pos.column - skip)
                {
                  fputs (ellipsis, out);
                  भङ्ग;
                }
            }
          यदि (state == inside)
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
          प्रति (पूर्णाङ्क i = loc.start.column - 1 - skip + 1,
                 i_end = min_int (col_end - 1 - skip, width);
               i < i_end; ++i)
            putc ('~', out);
          end_use_class (style, out);
          putc ('\n', out);
        }
      }
  }
}

शून्य
location_caret_suggestion (location loc, ध्रुव वर्ण *s, FILE *out)
{
  यदि (!(feature_flag & feature_caret))
    प्रत्यागम;
  ध्रुव वर्ण *style = "fixit-insert";
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
  प्रत्यागम !loc.start.file && !loc.start.ओळ && !loc.start.column
    && !loc.end.file && !loc.end.ओळ && !loc.end.column;
}

स्थिर अन्तःस्थ पूर्णाङ्क
str_to_int (ध्रुव वर्ण *s)
{
  long l = strtol (s, रिक्त, 10);
  प्रत्यागम l < 0 ? -1 : l <= INT_MAX ? l : INT_MAX;
}

शून्य
boundary_set_from_string (boundary *bound, वर्ण *str)
{
  /* Search backwards: the file name may contain '.'  or ':'.  */
  वर्ण *at = strrchr (str, '@');
  यदि (at)
    {
      *at = '\0';
      bound->byte = str_to_int (at + 1);
    }
  {
    वर्ण *dot = strrchr (str, '.');
    aver (dot);
    *dot = '\0';
    bound->column = str_to_int (dot + 1);
    यदि (!at)
      bound->byte = bound->column;
  }
  {
    वर्ण *colon = strrchr (str, ':');
    aver (colon);
    *colon = '\0';
    bound->ओळ = str_to_int (colon + 1);
  }
  bound->file = uniqstr_new (str);
}
