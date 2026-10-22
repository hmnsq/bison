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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ "system.h"

#அந்தர்பாவ <mbfile.h>
#அந்தர்பாவ <mbswidth.h>
#அந்தர்பாவ <quotearg.h>
#அந்தர்பாவ <stdio.h>    /* fileno */
#அந்தர்பாவ <sys/ioctl.h>
#அந்தர்பாவ <sys/stat.h> /* fstat */
#அந்தர்பாவ <termios.h>

#ifdef WINSIZE_IN_PTEM
# include <sys/stream.h>
# include <sys/ptem.h>
#endif

#அந்தர்பாவ "complain.h"
#அந்தர்பாவ "getargs.h"
#அந்தர்பாவ "location.h"

location மாறிலி empty_loc = EMPTY_LOCATION_INIT;

/* The terminal width.  Not less than 40.  */
நிலையான முழு
columns (வெறுமை)
{
  மாறிலி எழுத்து *cp = getenv ("COLUMNS");
  முழு res = 80;
  எனில் (cp && *cp)
    {
      long l = strtol (cp, வெற்று, 10);
      res = 0 <= l && l <= INT_MAX ? l : INT_MAX;
    }
  இல்லையெனில்
    {
#ifdef TIOCGWINSZ
      கட்டமைப்பு winsize ws;
      எனில் (ioctl (STDERR_FILENO, TIOCGWINSZ, &ws) != -1
          && 0 < ws.ws_col && ws.ws_col == (size_t) ws.ws_col)
        res = ws.ws_col;
#endif
    }
  திரும்பு max_int (res, 40);
}

/* Available screen width.  */
நிலையான முழு screen_width = 80;

/* The ellipsis symbol to use for this locale, and the number of
   screen-columns it uses.  */
நிலையான மாறிலி எழுத்து *ellipsis = "...";
நிலையான முழு ellipsize = 3;

/* If BUF is null, add BUFSIZE (which in this case must be less than
   INT_MAX) to COLUMN; otherwise, add mbsnwidth (BUF, BUFSIZE, 0) to
   COLUMN.  If an overflow occurs, return INT_MAX.  */

நிலையான உள்வரி முழு
add_column_width (முழு column, எழுத்து மாறிலி *buf, size_t bufsize)
{
  முழு width
    = buf ? mbsnwidth (buf, bufsize, 0)
    : INT_MAX <= bufsize ? INT_MAX
    : bufsize;
  திரும்பு column <= INT_MAX - width ? column + width : INT_MAX;
}

நிலையான வெறுமை
boundary_compute (boundary *cur, எழுத்து மாறிலி *token, size_t size)
{
  முழு வரி = cur->வரி;
  முழு column = cur->column;
  முழு byte = cur->byte;
  எழுத்து மாறிலி *p0 = token;
  எழுத்து மாறிலி *p = token;
  எழுத்து மாறிலி *lim = token + size;

  க்கு (p = token; p < lim; ++p)
    தேர்வு (*p)
      {
      நிலை '\n':
        வரி += வரி < INT_MAX;
        column = 1;
        byte = 1;
        p0 = p + 1;
        நிறுத்து;

      நிலை '\t':
        column = add_column_width (column, p0, p - p0);
        column = add_column_width (column, வெற்று, 8 - ((column - 1) & 7));
        p0 = p + 1;
        byte += byte < INT_MAX;
        நிறுத்து;

      இயல்பு:
        byte += byte < INT_MAX;
        நிறுத்து;
      }
  column = add_column_width (column, p0, p - p0);

  cur->வரி = வரி;
  cur->column = column;
  cur->byte = byte;
}


/* Set *LOC and adjust scanner cursor to account for token TOKEN of
   size SIZE.  */

வெறுமை
location_compute (location *loc, boundary *cur, எழுத்து மாறிலி *token, size_t size)
{
  loc->start = *cur;
  boundary_compute (cur, token, size);
  loc->end = *cur;

  எனில் (loc->end.வரி == INT_MAX && loc->start.வரி != INT_MAX)
    complain (loc, Wother, _("line number overflow"));
  எனில் (loc->end.column == INT_MAX && loc->start.column != INT_MAX)
    complain (loc, Wother, _("column number overflow"));
  /* TRANSLATORS: we are counting bytes, and there are too many.  */
  எனில் (loc->end.byte == INT_MAX && loc->start.byte != INT_MAX)
    complain (loc, Wother, _("byte number overflow"));
}

நிலையான முழு
boundary_print (boundary மாறிலி *b, FILE *out)
{
  திரும்பு fprintf (out, "%s:%d.%d@%d",
                  quotearg_n_style (3, escape_quoting_style, b->file),
                  b->வரி, b->column, b->byte);
}

முழு
location_print (location loc, FILE *out)
{
  முழு res = 0;
  எனில் (location_empty (loc))
    res += fprintf (out, "(empty location)");
  இல்லையெனில் எனில் (trace_flag & trace_locations)
    {
      res += boundary_print (&loc.start, out);
      res += fprintf (out, "-");
      res += boundary_print (&loc.end, out);
    }
  இல்லையெனில்
    {
      aver (loc.start.file);
      aver (loc.end.file);
      முழு end_col = 0 != loc.end.column ? loc.end.column - 1 : 0;
      res += fprintf (out, "%s",
                      quotearg_n_style (3, escape_quoting_style, loc.start.file));
      எனில் (0 < loc.start.வரி)
        {
          res += fprintf (out, ":%d", loc.start.வரி);
          எனில் (0 < loc.start.column)
            res += fprintf (out, ".%d", loc.start.column);
        }
      எனில் (loc.start.file != loc.end.file)
        {
          res += fprintf (out, "-%s",
                          quotearg_n_style (3, escape_quoting_style,
                                            loc.end.file));
          எனில் (0 < loc.end.வரி)
            {
              res += fprintf (out, ":%d", loc.end.வரி);
              எனில் (0 <= end_col)
                res += fprintf (out, ".%d", end_col);
            }
        }
      இல்லையெனில் எனில் (0 < loc.end.வரி)
        {
          எனில் (loc.start.வரி < loc.end.வரி)
            {
              res += fprintf (out, "-%d", loc.end.வரி);
              எனில் (0 <= end_col)
                res += fprintf (out, ".%d", end_col);
            }
          இல்லையெனில் எனில் (0 <= end_col && loc.start.column < end_col)
            res += fprintf (out, "-%d", end_col);
        }
    }

  திரும்பு res;
}


/* Persistent data used by location_caret to avoid reopening and rereading the
   same file all over for each error.  */
நிலையான கட்டமைப்பு
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
  முழு வரி_len;
  /* Given the initial column to display, the offset (number of
     characters to skip at the beginning of the line). */
  முழு skip;

  /* Available width to quote the source file.  Eight chars are
     consumed by the left-margin (with line number).  */
  முழு width;
} caret_info;

வெறுமை caret_init (வெறுமை)
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

வெறுமை
caret_free (வெறுமை)
{
  எனில் (caret_info.file)
    {
      fclose (caret_info.file);
      caret_info.file = வெற்று;
    }
}

/* Open FILE for quoting, if needed, and if possible.  Return whether
   the file can quoted. */
நிலையான bool
caret_set_file (மாறிலி எழுத்து *file)
{
  /* If a different file than before, close and let the rest open
     the new one. */
  எனில் (caret_info.pos.file && caret_info.pos.file != file)
    {
      caret_free ();
      caret_info.pos.file = வெற்று;
    }
  எனில் (!caret_info.pos.file)
    {
      caret_info.pos.file = file;
      எனில் ((caret_info.file = fopen (caret_info.pos.file, "rb")))
        {
          /* If the file is not regular (imagine #line 1 "/dev/stdin"
             in the input file for instance), don't try to quote the
             file.  Keep caret_info.pos.file set so that we don't try
             to open it again, but leave caret_info.file NULL so that
             we don't try to quote it. */
          கட்டமைப்பு stat buf;
          எனில் (fstat (fileno (caret_info.file), &buf) == 0
              && buf.st_mode & S_IFREG)
            {
              caret_info.pos.வரி = 1;
              mbf_init (caret_info.mbfile, caret_info.file);
            }
          இல்லையெனில்
            caret_free ();
        }
    }
  திரும்பு !!caret_info.file;
}

/* Getc, but smash \r\n as \n.  */
நிலையான வெறுமை
caret_getc_internal (mbchar_t *res)
{
  mbf_getc (*res, caret_info.mbfile);
  எனில் (mb_iseq (*res, '\r'))
    {
      mbchar_t c;
      mbf_getc (c, caret_info.mbfile);
      எனில் (mb_iseq (c, '\n'))
        mb_copy (res, &c);
      இல்லையெனில்
        mbf_ungetc (c, caret_info.mbfile);
    }
}

#define caret_getc(Var) caret_getc_internal(&Var)

/* Move CARET_INFO (which has a valid FILE) to the line number LINE.
   Compute and cache that line's length in CARET_INFO.LINE_LEN.
   Return whether successful.  */
நிலையான bool
caret_set_வரி (முழு வரி)
{
  /* If the line we want to quote is seekable (the same line as the previous
     location), just seek it. If it was a previous line, we lost track of it,
     so return to the start of file.  */
  எனில் (வரி < caret_info.pos.வரி)
    {
      caret_info.pos.வரி = 1;
      caret_info.offset = 0;
    }
  எனில் (fseek (caret_info.file, caret_info.offset, SEEK_SET))
    திரும்பு false;

  /* If this is the same line as the previous one, we are done. */
  எனில் (வரி < caret_info.pos.வரி)
    திரும்பு true;

  /* Advance to the line's position, keeping track of the offset.  */
  போது (caret_info.pos.வரி < வரி)
    {
      mbchar_t c;
      caret_getc (c);
      எனில் (mb_iseof (c))
        /* Something is wrong, that line number does not exist.  */
        திரும்பு false;
      caret_info.pos.வரி += mb_iseq (c, '\n');
    }
  caret_info.offset = ftell (caret_info.file);
  caret_info.pos.column = 1;
  /* Reset mbf's internal state.
     FIXME: should be done in mbfile.  */
  caret_info.mbfile.eof_seen = 0;

  /* Find the number of columns of this line.  */
  போது (true)
    {
      mbchar_t c;
      caret_getc (c);
      எனில் (mb_iseof (c) || mb_iseq (c, '\n'))
        நிறுத்து;
      boundary_compute (&caret_info.pos, mb_ptr (c), mb_len (c));
    }
  caret_info.வரி_len = caret_info.pos.column;
  /* Go back to the beginning of line.  */
  எனில் (fseek (caret_info.file, caret_info.offset, SEEK_SET))
    திரும்பு false;
  /* Reset mbf's internal state.
     FIXME: should be done in mbfile.  */
  caret_info.mbfile.eof_seen = 0;
  caret_info.pos.column = 1;
  திரும்பு true;
}

/* Compute CARET_INFO.WIDTH and CARET_INFO.SKIP based on the fact that
   the first column to display in the current line is COL.  */
நிலையான bool
caret_set_column (முழு col)
{
  /* Available width.  Eight chars are consumed by the left-margin
     (with line number).  */
  caret_info.width = screen_width - 8;
  caret_info.skip = 0;
  எனில் (caret_info.width < caret_info.வரி_len)
    {
      /* We cannot quote the whole line.  Make sure we can see the
         beginning of the location.  */
      caret_info.skip = caret_info.width < col ? col - 10 : 0;
    }
  /* If we skip the initial part, we insert "..." before.  */
  எனில் (caret_info.skip)
    caret_info.width -= ellipsize;
  /* If the end of line does not fit, we also need to truncate the
     end, and leave "..." there.  */
  எனில் (caret_info.width < caret_info.வரி_len - caret_info.skip)
    caret_info.width -= ellipsize;
  திரும்பு true;
}

வெறுமை
location_caret (location loc, மாறிலி எழுத்து *style, FILE *out)
{
  எனில் (!(feature_flag & feature_caret))
    திரும்பு;
  எனில் (!loc.start.வரி)
    திரும்பு;
  எனில் (!caret_set_file (loc.start.file))
    திரும்பு;
  எனில் (!caret_set_வரி (loc.start.வரி))
    திரும்பு;
  எனில் (!caret_set_column (loc.start.column))
    திரும்பு;

  மாறிலி முழு width = caret_info.width;
  மாறிலி முழு skip = caret_info.skip;

  /* Read the actual line.  Don't update the offset, so that we keep a pointer
     to the start of the line.  */
  {
    mbchar_t c;
    caret_getc (c);
    எனில் (!mb_iseof (c))
      {
        /* The last column to highlight.  Only the first line of
           multiline locations are quoted, in which case the ending
           column is the end of line.

           We used to work with byte offsets, and that was much
           easier.  However, we went back to using (visual) columns to
           support truncating of long lines.  */
        மாறிலி முழு col_end
          = loc.start.வரி == loc.end.வரி
          ? loc.end.column
          : caret_info.வரி_len;
        /* Quote the file (at most the first line in the case of
           multiline locations).  */
        {
          fprintf (out, "%5d | %s", loc.start.வரி, skip ? ellipsis : "");
          /* Whether we opened the style.  If the line is not as
             expected (maybe the file was changed since the scanner
             ran), we might reach the end before we actually saw the
             opening column.  */
          எண்ணிக்கை { before, inside, after } state = before;
          போது (!mb_iseof (c) && !mb_iseq (c, '\n'))
            {
              // We might have already opened (and even closed!) the
              // style and yet have the equality of the columns if we
              // just saw zero-width characters.
              எனில் (state == before
                  && caret_info.pos.column == loc.start.column)
                {
                  begin_use_class (style, out);
                  state = inside;
                }
              எனில் (skip < caret_info.pos.column)
                mb_putc (c, out);
              boundary_compute (&caret_info.pos, mb_ptr (c), mb_len (c));
              caret_getc (c);
              எனில் (state == inside
                  && (caret_info.pos.column == col_end
                      || width < caret_info.pos.column - skip))
                {
                  end_use_class (style, out);
                  state = after;
                }
              எனில் (width < caret_info.pos.column - skip)
                {
                  fputs (ellipsis, out);
                  நிறுத்து;
                }
            }
          எனில் (state == inside)
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
          க்கு (முழு i = loc.start.column - 1 - skip + 1,
                 i_end = min_int (col_end - 1 - skip, width);
               i < i_end; ++i)
            putc ('~', out);
          end_use_class (style, out);
          putc ('\n', out);
        }
      }
  }
}

வெறுமை
location_caret_suggestion (location loc, மாறிலி எழுத்து *s, FILE *out)
{
  எனில் (!(feature_flag & feature_caret))
    திரும்பு;
  மாறிலி எழுத்து *style = "fixit-insert";
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
  திரும்பு !loc.start.file && !loc.start.வரி && !loc.start.column
    && !loc.end.file && !loc.end.வரி && !loc.end.column;
}

நிலையான உள்வரி முழு
str_to_int (மாறிலி எழுத்து *s)
{
  long l = strtol (s, வெற்று, 10);
  திரும்பு l < 0 ? -1 : l <= INT_MAX ? l : INT_MAX;
}

வெறுமை
boundary_set_from_string (boundary *bound, எழுத்து *str)
{
  /* Search backwards: the file name may contain '.'  or ':'.  */
  எழுத்து *at = strrchr (str, '@');
  எனில் (at)
    {
      *at = '\0';
      bound->byte = str_to_int (at + 1);
    }
  {
    எழுத்து *dot = strrchr (str, '.');
    aver (dot);
    *dot = '\0';
    bound->column = str_to_int (dot + 1);
    எனில் (!at)
      bound->byte = bound->column;
  }
  {
    எழுத்து *colon = strrchr (str, ':');
    aver (colon);
    *colon = '\0';
    bound->வரி = str_to_int (colon + 1);
  }
  bound->file = uniqstr_new (str);
}
