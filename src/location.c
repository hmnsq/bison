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

location स्थिरांक empty_loc = EMPTY_LOCATION_INIT;

/* The terminal width.  Not less than 40.  */
स्थिर पूर्णांक
columns (शून्य)
{
  स्थिरांक अक्षर *cp = दो_पर्या ("COLUMNS");
  पूर्णांक res = 80;
  जर (cp && *cp)
    {
      दीर्घ l = म_से_दीर्घ (cp, रिक्त, 10);
      res = 0 <= l && l <= पूर्णांक_उच्च ? l : पूर्णांक_उच्च;
    }
  अन्यथा
    {
#ifdef TIOCGWINSZ
      संरचना winsize ws;
      जर (ioctl (STDERR_FILENO, TIOCGWINSZ, &ws) != -1
          && 0 < ws.ws_col && ws.ws_col == (माप_प्रकार) ws.ws_col)
        res = ws.ws_col;
#endif
    }
  परत max_int (res, 40);
}

/* Available screen width.  */
स्थिर पूर्णांक screen_width = 80;

/* The ellipsis symbol to use for this locale, and the number of
   screen-columns it uses.  */
स्थिर स्थिरांक अक्षर *ellipsis = "...";
स्थिर पूर्णांक ellipsize = 3;

/* If BUF is null, add BUFSIZE (which in this case must be less than
   INT_MAX) to COLUMN; otherwise, add mbsnwidth (BUF, BUFSIZE, 0) to
   COLUMN.  If an overflow occurs, return INT_MAX.  */

स्थिर ओळीत पूर्णांक
add_column_width (पूर्णांक column, अक्षर स्थिरांक *buf, माप_प्रकार bufsize)
{
  पूर्णांक width
    = buf ? mbsnwidth (buf, bufsize, 0)
    : पूर्णांक_उच्च <= bufsize ? पूर्णांक_उच्च
    : bufsize;
  परत column <= पूर्णांक_उच्च - width ? column + width : पूर्णांक_उच्च;
}

स्थिर शून्य
boundary_compute (boundary *cur, अक्षर स्थिरांक *token, माप_प्रकार size)
{
  पूर्णांक ओळ = cur->ओळ;
  पूर्णांक column = cur->column;
  पूर्णांक byte = cur->byte;
  अक्षर स्थिरांक *p0 = token;
  अक्षर स्थिरांक *p = token;
  अक्षर स्थिरांक *lim = token + size;

  साठी (p = token; p < lim; ++p)
    निवड (*p)
      {
      स्थिती '\n':
        ओळ += ओळ < पूर्णांक_उच्च;
        column = 1;
        byte = 1;
        p0 = p + 1;
        थांबा;

      स्थिती '\t':
        column = add_column_width (column, p0, p - p0);
        column = add_column_width (column, रिक्त, 8 - ((column - 1) & 7));
        p0 = p + 1;
        byte += byte < पूर्णांक_उच्च;
        थांबा;

      पूर्वनिर्धारित:
        byte += byte < पूर्णांक_उच्च;
        थांबा;
      }
  column = add_column_width (column, p0, p - p0);

  cur->ओळ = ओळ;
  cur->column = column;
  cur->byte = byte;
}


/* Set *LOC and adjust scanner cursor to account for token TOKEN of
   size SIZE.  */

शून्य
location_compute (location *loc, boundary *cur, अक्षर स्थिरांक *token, माप_प्रकार size)
{
  loc->start = *cur;
  boundary_compute (cur, token, size);
  loc->end = *cur;

  जर (loc->end.ओळ == पूर्णांक_उच्च && loc->start.ओळ != पूर्णांक_उच्च)
    complain (loc, Wother, _("line number overflow"));
  जर (loc->end.column == पूर्णांक_उच्च && loc->start.column != पूर्णांक_उच्च)
    complain (loc, Wother, _("column number overflow"));
  /* TRANSLATORS: we are counting bytes, and there are too many.  */
  जर (loc->end.byte == पूर्णांक_उच्च && loc->start.byte != पूर्णांक_उच्च)
    complain (loc, Wother, _("byte number overflow"));
}

स्थिर पूर्णांक
boundary_print (boundary स्थिरांक *b, खाता *out)
{
  परत ख_लिखो (out, "%s:%d.%d@%d",
                  quotearg_n_style (3, escape_quoting_style, b->file),
                  b->ओळ, b->column, b->byte);
}

पूर्णांक
location_print (location loc, खाता *out)
{
  पूर्णांक res = 0;
  जर (location_empty (loc))
    res += ख_लिखो (out, "(empty location)");
  अन्यथा जर (trace_flag & trace_locations)
    {
      res += boundary_print (&loc.start, out);
      res += ख_लिखो (out, "-");
      res += boundary_print (&loc.end, out);
    }
  अन्यथा
    {
      aver (loc.start.file);
      aver (loc.end.file);
      पूर्णांक end_col = 0 != loc.end.column ? loc.end.column - 1 : 0;
      res += ख_लिखो (out, "%s",
                      quotearg_n_style (3, escape_quoting_style, loc.start.file));
      जर (0 < loc.start.ओळ)
        {
          res += ख_लिखो (out, ":%d", loc.start.ओळ);
          जर (0 < loc.start.column)
            res += ख_लिखो (out, ".%d", loc.start.column);
        }
      जर (loc.start.file != loc.end.file)
        {
          res += ख_लिखो (out, "-%s",
                          quotearg_n_style (3, escape_quoting_style,
                                            loc.end.file));
          जर (0 < loc.end.ओळ)
            {
              res += ख_लिखो (out, ":%d", loc.end.ओळ);
              जर (0 <= end_col)
                res += ख_लिखो (out, ".%d", end_col);
            }
        }
      अन्यथा जर (0 < loc.end.ओळ)
        {
          जर (loc.start.ओळ < loc.end.ओळ)
            {
              res += ख_लिखो (out, "-%d", loc.end.ओळ);
              जर (0 <= end_col)
                res += ख_लिखो (out, ".%d", end_col);
            }
          अन्यथा जर (0 <= end_col && loc.start.column < end_col)
            res += ख_लिखो (out, "-%d", end_col);
        }
    }

  परत res;
}


/* Persistent data used by location_caret to avoid reopening and rereading the
   same file all over for each error.  */
स्थिर संरचना
{
  /* Raw input file.  */
  खाता *file;
  /* Input file as a stream of multibyte characters.  */
  mb_file_t mbfile;
  /* The position within the last file we quoted.  If POS.FILE is non
     NULL, but FILE is NULL, it means this file is special and should
     not be quoted. */
  boundary pos;
  /* Offset in FILE of the current line (i.e., where line POS.LINE
     starts).  */
  माप_प्रकार offset;
  /* Length of the current line.  */
  पूर्णांक ओळ_len;
  /* Given the initial column to display, the offset (number of
     characters to skip at the beginning of the line). */
  पूर्णांक skip;

  /* Available width to quote the source file.  Eight chars are
     consumed by the left-margin (with line number).  */
  पूर्णांक width;
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
  जर (caret_info.file)
    {
      ख_बंद (caret_info.file);
      caret_info.file = रिक्त;
    }
}

/* Open FILE for quoting, if needed, and if possible.  Return whether
   the file can quoted. */
स्थिर bool
caret_set_file (स्थिरांक अक्षर *file)
{
  /* If a different file than before, close and let the rest open
     the new one. */
  जर (caret_info.pos.file && caret_info.pos.file != file)
    {
      caret_free ();
      caret_info.pos.file = रिक्त;
    }
  जर (!caret_info.pos.file)
    {
      caret_info.pos.file = file;
      जर ((caret_info.file = ख_खोलो (caret_info.pos.file, "rb")))
        {
          /* If the file is not regular (imagine #line 1 "/dev/stdin"
             in the input file for instance), don't try to quote the
             file.  Keep caret_info.pos.file set so that we don't try
             to open it again, but leave caret_info.file NULL so that
             we don't try to quote it. */
          संरचना stat buf;
          जर (ख_स्थिति (fileno (caret_info.file), &buf) == 0
              && buf.st_mode & S_IFREG)
            {
              caret_info.pos.ओळ = 1;
              mbf_init (caret_info.mbfile, caret_info.file);
            }
          अन्यथा
            caret_free ();
        }
    }
  परत !!caret_info.file;
}

/* Getc, but smash \r\n as \n.  */
स्थिर शून्य
caret_getc_internal (mbchar_t *res)
{
  mbf_getc (*res, caret_info.mbfile);
  जर (mb_iseq (*res, '\r'))
    {
      mbchar_t c;
      mbf_getc (c, caret_info.mbfile);
      जर (mb_iseq (c, '\n'))
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
caret_set_ओळ (पूर्णांक ओळ)
{
  /* If the line we want to quote is seekable (the same line as the previous
     location), just seek it. If it was a previous line, we lost track of it,
     so return to the start of file.  */
  जर (ओळ < caret_info.pos.ओळ)
    {
      caret_info.pos.ओळ = 1;
      caret_info.offset = 0;
    }
  जर (ख_जाओ (caret_info.file, caret_info.offset, शुरू_से))
    परत false;

  /* If this is the same line as the previous one, we are done. */
  जर (ओळ < caret_info.pos.ओळ)
    परत true;

  /* Advance to the line's position, keeping track of the offset.  */
  पर्यंत (caret_info.pos.ओळ < ओळ)
    {
      mbchar_t c;
      caret_getc (c);
      जर (mb_iseof (c))
        /* Something is wrong, that line number does not exist.  */
        परत false;
      caret_info.pos.ओळ += mb_iseq (c, '\n');
    }
  caret_info.offset = ख_बताओ (caret_info.file);
  caret_info.pos.column = 1;
  /* Reset mbf's internal state.
     FIXME: should be done in mbfile.  */
  caret_info.mbfile.eof_seen = 0;

  /* Find the number of columns of this line.  */
  पर्यंत (true)
    {
      mbchar_t c;
      caret_getc (c);
      जर (mb_iseof (c) || mb_iseq (c, '\n'))
        थांबा;
      boundary_compute (&caret_info.pos, mb_ptr (c), mb_len (c));
    }
  caret_info.ओळ_len = caret_info.pos.column;
  /* Go back to the beginning of line.  */
  जर (ख_जाओ (caret_info.file, caret_info.offset, शुरू_से))
    परत false;
  /* Reset mbf's internal state.
     FIXME: should be done in mbfile.  */
  caret_info.mbfile.eof_seen = 0;
  caret_info.pos.column = 1;
  परत true;
}

/* Compute CARET_INFO.WIDTH and CARET_INFO.SKIP based on the fact that
   the first column to display in the current line is COL.  */
स्थिर bool
caret_set_column (पूर्णांक col)
{
  /* Available width.  Eight chars are consumed by the left-margin
     (with line number).  */
  caret_info.width = screen_width - 8;
  caret_info.skip = 0;
  जर (caret_info.width < caret_info.ओळ_len)
    {
      /* We cannot quote the whole line.  Make sure we can see the
         beginning of the location.  */
      caret_info.skip = caret_info.width < col ? col - 10 : 0;
    }
  /* If we skip the initial part, we insert "..." before.  */
  जर (caret_info.skip)
    caret_info.width -= ellipsize;
  /* If the end of line does not fit, we also need to truncate the
     end, and leave "..." there.  */
  जर (caret_info.width < caret_info.ओळ_len - caret_info.skip)
    caret_info.width -= ellipsize;
  परत true;
}

शून्य
location_caret (location loc, स्थिरांक अक्षर *style, खाता *out)
{
  जर (!(feature_flag & feature_caret))
    परत;
  जर (!loc.start.ओळ)
    परत;
  जर (!caret_set_file (loc.start.file))
    परत;
  जर (!caret_set_ओळ (loc.start.ओळ))
    परत;
  जर (!caret_set_column (loc.start.column))
    परत;

  स्थिरांक पूर्णांक width = caret_info.width;
  स्थिरांक पूर्णांक skip = caret_info.skip;

  /* Read the actual line.  Don't update the offset, so that we keep a pointer
     to the start of the line.  */
  {
    mbchar_t c;
    caret_getc (c);
    जर (!mb_iseof (c))
      {
        /* The last column to highlight.  Only the first line of
           multiline locations are quoted, in which case the ending
           column is the end of line.

           We used to work with byte offsets, and that was much
           easier.  However, we went back to using (visual) columns to
           support truncating of long lines.  */
        स्थिरांक पूर्णांक col_end
          = loc.start.ओळ == loc.end.ओळ
          ? loc.end.column
          : caret_info.ओळ_len;
        /* Quote the file (at most the first line in the case of
           multiline locations).  */
        {
          ख_लिखो (out, "%5d | %s", loc.start.ओळ, skip ? ellipsis : "");
          /* Whether we opened the style.  If the line is not as
             expected (maybe the file was changed since the scanner
             ran), we might reach the end before we actually saw the
             opening column.  */
          गणना { before, inside, after } state = before;
          पर्यंत (!mb_iseof (c) && !mb_iseq (c, '\n'))
            {
              // We might have already opened (and even closed!) the
              // style and yet have the equality of the columns if we
              // just saw zero-width characters.
              जर (state == before
                  && caret_info.pos.column == loc.start.column)
                {
                  begin_use_class (style, out);
                  state = inside;
                }
              जर (skip < caret_info.pos.column)
                mb_putc (c, out);
              boundary_compute (&caret_info.pos, mb_ptr (c), mb_len (c));
              caret_getc (c);
              जर (state == inside
                  && (caret_info.pos.column == col_end
                      || width < caret_info.pos.column - skip))
                {
                  end_use_class (style, out);
                  state = after;
                }
              जर (width < caret_info.pos.column - skip)
                {
                  ख_माला_दो (ellipsis, out);
                  थांबा;
                }
            }
          जर (state == inside)
            {
              // The line is shorter than expected.
              end_use_class (style, out);
              state = after;
            }
          अ_दो ('\n', out);
        }

        /* Print the carets with the same indentation as above.  */
        {
          ख_लिखो (out, "      | %*s",
                   loc.start.column - 1 - skip + (skip ? ellipsize : 0), "");
          begin_use_class (style, out);
          अ_दो ('^', out);
          /* Underlining a multiline location ends with the first
             line.  */
          साठी (पूर्णांक i = loc.start.column - 1 - skip + 1,
                 i_end = min_int (col_end - 1 - skip, width);
               i < i_end; ++i)
            अ_दो ('~', out);
          end_use_class (style, out);
          अ_दो ('\n', out);
        }
      }
  }
}

शून्य
location_caret_suggestion (location loc, स्थिरांक अक्षर *s, खाता *out)
{
  जर (!(feature_flag & feature_caret))
    परत;
  स्थिरांक अक्षर *style = "fixit-insert";
  ख_लिखो (out, "      | %*s",
           loc.start.column - 1 - caret_info.skip
           + (caret_info.skip ? ellipsize : 0),
           "");
  begin_use_class (style, out);
  ख_माला_दो (s, out);
  end_use_class (style, out);
  अ_दो ('\n', out);
}

bool
location_empty (location loc)
{
  परत !loc.start.file && !loc.start.ओळ && !loc.start.column
    && !loc.end.file && !loc.end.ओळ && !loc.end.column;
}

स्थिर ओळीत पूर्णांक
str_to_int (स्थिरांक अक्षर *s)
{
  दीर्घ l = म_से_दीर्घ (s, रिक्त, 10);
  परत l < 0 ? -1 : l <= पूर्णांक_उच्च ? l : पूर्णांक_उच्च;
}

शून्य
boundary_set_from_string (boundary *bound, अक्षर *str)
{
  /* Search backwards: the file name may contain '.'  or ':'.  */
  अक्षर *at = म_खोजप (str, '@');
  जर (at)
    {
      *at = '\0';
      bound->byte = str_to_int (at + 1);
    }
  {
    अक्षर *dot = म_खोजप (str, '.');
    aver (dot);
    *dot = '\0';
    bound->column = str_to_int (dot + 1);
    जर (!at)
      bound->byte = bound->column;
  }
  {
    अक्षर *colon = म_खोजप (str, ':');
    aver (colon);
    *colon = '\0';
    bound->ओळ = str_to_int (colon + 1);
  }
  bound->file = uniqstr_new (str);
}
