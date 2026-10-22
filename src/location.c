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

#ضمن <config.h>
#ضمن "system.h"

#ضمن <mbfile.h>
#ضمن <mbswidth.h>
#ضمن <quotearg.h>
#ضمن <stdio.h>    /* fileno */
#ضمن <sys/ioctl.h>
#ضمن <sys/stat.h> /* fstat */
#ضمن <termios.h>

#که_تعریف_شوی WINSIZE_دننه_PTEM
# include <sys/stream.h>
# include <sys/ptem.h>
#د_که_پای

#ضمن "complain.h"
#ضمن "getargs.h"
#ضمن "location.h"

location ثابت تشه_loc = تشه_LOCATION_INIT;

/* The terminal width.  Not less than 40.  */
ساكن صحيح
columns (فراغ)
{
  ثابت حرف *cp = getenv ("COLUMNS");
  صحيح res = 80;
  إذا (cp && *cp)
    {
      اوږد l = strtol (cp, عدم, 10);
      res = 0 <= l && l <= لوی_عدد ? l : لوی_عدد;
    }
  وإلا
    {
#که_تعریف_شوی TIOCGWINSZ
      بنية winsize ws;
      إذا (ioctl (STDERR_FILENO, TIOCGWINSZ, &ws) != -1
          && 0 < ws.ws_ستون && ws.ws_ستون == (size_t) ws.ws_ستون)
        res = ws.ws_ستون;
#د_که_پای
    }
  أرجع max_int (res, 40);
}

/* Available screen width.  */
ساكن صحيح screen_پلنوالی = 80;

/* The ellipsis symbol to use for this locale, and the number of
   screen-columns it uses.  */
ساكن ثابت حرف *ellipsis = "...";
ساكن صحيح ellipsize = 3;

/* If BUF is null, add BUFSIZE (which in this case must be less than
   INT_MAX) to COLUMN; otherwise, add mbsnwidth (BUF, BUFSIZE, 0) to
   COLUMN.  If an overflow occurs, return INT_MAX.  */

ساكن مضمن صحيح
جمع_کړه_ستنه_پلنوالی (صحيح ستنه, حرف ثابت *buf, size_t bufsize)
{
  صحيح پلنوالی
    = buf ? mbsnwidth (buf, bufsize, 0)
    : لوی_عدد <= bufsize ? لوی_عدد
    : bufsize;
  أرجع ستنه <= لوی_عدد - پلنوالی ? ستنه + پلنوالی : لوی_عدد;
}

ساكن فراغ
boundary_compute (boundary *cur, حرف ثابت *token, size_t اندازه)
{
  صحيح کرښه = cur->کرښه;
  صحيح ستنه = cur->ستنه;
  صحيح byte = cur->byte;
  حرف ثابت *p0 = token;
  حرف ثابت *p = token;
  حرف ثابت *lim = token + اندازه;

  لكل (p = token; p < lim; ++p)
    بدل (*p)
      {
      حالة '\n':
        کرښه += کرښه < لوی_عدد;
        ستنه = 1;
        byte = 1;
        p0 = p + 1;
        اكسر;

      حالة '\t':
        ستنه = جمع_کړه_ستنه_پلنوالی (ستنه, p0, p - p0);
        ستنه = جمع_کړه_ستنه_پلنوالی (ستنه, عدم, 8 - ((ستنه - 1) & 7));
        p0 = p + 1;
        byte += byte < لوی_عدد;
        اكسر;

      افتراضي:
        byte += byte < لوی_عدد;
        اكسر;
      }
  ستنه = جمع_کړه_ستنه_پلنوالی (ستنه, p0, p - p0);

  cur->کرښه = کرښه;
  cur->ستنه = ستنه;
  cur->byte = byte;
}


/* Set *LOC and adjust scanner cursor to account for token TOKEN of
   size SIZE.  */

فراغ
location_compute (location *loc, boundary *cur, حرف ثابت *token, size_t اندازه)
{
  loc->پیل = *cur;
  boundary_compute (cur, token, اندازه);
  loc->پای_ټکی = *cur;

  إذا (loc->پای_ټکی.کرښه == لوی_عدد && loc->پیل.کرښه != لوی_عدد)
    complain (loc, Wother, _("line number overflow"));
  إذا (loc->پای_ټکی.ستنه == لوی_عدد && loc->پیل.ستنه != لوی_عدد)
    complain (loc, Wother, _("column number overflow"));
  /* TRANSLATORS: we are counting bytes, and there are too many.  */
  إذا (loc->پای_ټکی.byte == لوی_عدد && loc->پیل.byte != لوی_عدد)
    complain (loc, Wother, _("byte number overflow"));
}

ساكن صحيح
boundary_print (boundary ثابت *b, FILE *بهر)
{
  أرجع fprintf (بهر, "%s:%d.%d@%d",
                  quotearg_n_style (3, escape_quoting_style, b->file),
                  b->کرښه, b->ستنه, b->byte);
}

صحيح
location_print (location loc, FILE *بهر)
{
  صحيح res = 0;
  إذا (location_تشه (loc))
    res += fprintf (بهر, "(empty location)");
  وإلا إذا (trace_flag & trace_locations)
    {
      res += boundary_print (&loc.پیل, بهر);
      res += fprintf (بهر, "-");
      res += boundary_print (&loc.پای_ټکی, بهر);
    }
  وإلا
    {
      aver (loc.پیل.file);
      aver (loc.پای_ټکی.file);
      صحيح پای_ټکی_ستون = 0 != loc.پای_ټکی.ستنه ? loc.پای_ټکی.ستنه - 1 : 0;
      res += fprintf (بهر, "%s",
                      quotearg_n_style (3, escape_quoting_style, loc.پیل.file));
      إذا (0 < loc.پیل.کرښه)
        {
          res += fprintf (بهر, ":%d", loc.پیل.کرښه);
          إذا (0 < loc.پیل.ستنه)
            res += fprintf (بهر, ".%d", loc.پیل.ستنه);
        }
      إذا (loc.پیل.file != loc.پای_ټکی.file)
        {
          res += fprintf (بهر, "-%s",
                          quotearg_n_style (3, escape_quoting_style,
                                            loc.پای_ټکی.file));
          إذا (0 < loc.پای_ټکی.کرښه)
            {
              res += fprintf (بهر, ":%d", loc.پای_ټکی.کرښه);
              إذا (0 <= پای_ټکی_ستون)
                res += fprintf (بهر, ".%d", پای_ټکی_ستون);
            }
        }
      وإلا إذا (0 < loc.پای_ټکی.کرښه)
        {
          إذا (loc.پیل.کرښه < loc.پای_ټکی.کرښه)
            {
              res += fprintf (بهر, "-%d", loc.پای_ټکی.کرښه);
              إذا (0 <= پای_ټکی_ستون)
                res += fprintf (بهر, ".%d", پای_ټکی_ستون);
            }
          وإلا إذا (0 <= پای_ټکی_ستون && loc.پیل.ستنه < پای_ټکی_ستون)
            res += fprintf (بهر, "-%d", پای_ټکی_ستون);
        }
    }

  أرجع res;
}


/* Persistent data used by location_caret to avoid reopening and rereading the
   same file all over for each error.  */
ساكن بنية
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
  size_t دننه_واټن;
  /* Length of the current line.  */
  صحيح کرښه_len;
  /* Given the initial column to display, the offset (number of
     characters to skip at the beginning of the line). */
  صحيح skip;

  /* Available width to quote the source file.  Eight chars are
     consumed by the left-margin (with line number).  */
  صحيح پلنوالی;
} caret_info;

فراغ caret_init (فراغ)
{
  screen_پلنوالی = columns ();
  /* TRANSLATORS: This is used when a line is too long, and is
     displayed truncated.  Use an ellipsis appropriate for your
     language, remembering that "…" (U+2026 HORIZONTAL ELLIPSIS)
     sometimes misdisplays and that "..." (three ASCII periods) is a
     safer choice in some locales.  */
  ellipsis = _("...");
  ellipsize = mbswidth (ellipsis, 0);
}

فراغ
caret_free (فراغ)
{
  إذا (caret_info.file)
    {
      fclose (caret_info.file);
      caret_info.file = عدم;
    }
}

/* Open FILE for quoting, if needed, and if possible.  Return whether
   the file can quoted. */
ساكن bool
caret_سېټ_file (ثابت حرف *file)
{
  /* If a different file than before, close and let the rest open
     the new one. */
  إذا (caret_info.pos.file && caret_info.pos.file != file)
    {
      caret_free ();
      caret_info.pos.file = عدم;
    }
  إذا (!caret_info.pos.file)
    {
      caret_info.pos.file = file;
      إذا ((caret_info.file = fopen (caret_info.pos.file, "rb")))
        {
          /* If the file is not regular (imagine #line 1 "/dev/stdin"
             in the input file for instance), don't try to quote the
             file.  Keep caret_info.pos.file set so that we don't try
             to open it again, but leave caret_info.file NULL so that
             we don't try to quote it. */
          بنية stat buf;
          إذا (fstat (fileno (caret_info.file), &buf) == 0
              && buf.st_ډېرپېښ & S_IFREG)
            {
              caret_info.pos.کرښه = 1;
              mbf_init (caret_info.mbfile, caret_info.file);
            }
          وإلا
            caret_free ();
        }
    }
  أرجع !!caret_info.file;
}

/* Getc, but smash \r\n as \n.  */
ساكن فراغ
caret_getc_دننی (mbchar_t *res)
{
  mbf_getc (*res, caret_info.mbfile);
  إذا (mb_iseq (*res, '\r'))
    {
      mbchar_t c;
      mbf_getc (c, caret_info.mbfile);
      إذا (mb_iseq (c, '\n'))
        mb_copy (res, &c);
      وإلا
        mbf_ungetc (c, caret_info.mbfile);
    }
}

#تعریف_کړه caret_getc(Var) caret_getc_دننی(&Var)

/* Move CARET_INFO (which has a valid FILE) to the line number LINE.
   Compute and cache that line's length in CARET_INFO.LINE_LEN.
   Return whether successful.  */
ساكن bool
caret_سېټ_کرښه (صحيح کرښه)
{
  /* If the line we want to quote is seekable (the same line as the previous
     location), just seek it. If it was a previous line, we lost track of it,
     so return to the start of file.  */
  إذا (کرښه < caret_info.pos.کرښه)
    {
      caret_info.pos.کرښه = 1;
      caret_info.دننه_واټن = 0;
    }
  إذا (fseek (caret_info.file, caret_info.دننه_واټن, SEEK_SET))
    أرجع false;

  /* If this is the same line as the previous one, we are done. */
  إذا (کرښه < caret_info.pos.کرښه)
    أرجع true;

  /* Advance to the line's position, keeping track of the offset.  */
  طالما (caret_info.pos.کرښه < کرښه)
    {
      mbchar_t c;
      caret_getc (c);
      إذا (mb_iseof (c))
        /* Something is wrong, that line number does not exist.  */
        أرجع false;
      caret_info.pos.کرښه += mb_iseq (c, '\n');
    }
  caret_info.دننه_واټن = ftell (caret_info.file);
  caret_info.pos.ستنه = 1;
  /* Reset mbf's internal state.
     FIXME: should be done in mbfile.  */
  caret_info.mbfile.eof_کتل_شوی = 0;

  /* Find the number of columns of this line.  */
  طالما (true)
    {
      mbchar_t c;
      caret_getc (c);
      إذا (mb_iseof (c) || mb_iseq (c, '\n'))
        اكسر;
      boundary_compute (&caret_info.pos, mb_ptr (c), mb_len (c));
    }
  caret_info.کرښه_len = caret_info.pos.ستنه;
  /* Go back to the beginning of line.  */
  إذا (fseek (caret_info.file, caret_info.دننه_واټن, SEEK_SET))
    أرجع false;
  /* Reset mbf's internal state.
     FIXME: should be done in mbfile.  */
  caret_info.mbfile.eof_کتل_شوی = 0;
  caret_info.pos.ستنه = 1;
  أرجع true;
}

/* Compute CARET_INFO.WIDTH and CARET_INFO.SKIP based on the fact that
   the first column to display in the current line is COL.  */
ساكن bool
caret_سېټ_ستنه (صحيح ستون)
{
  /* Available width.  Eight chars are consumed by the left-margin
     (with line number).  */
  caret_info.پلنوالی = screen_پلنوالی - 8;
  caret_info.skip = 0;
  إذا (caret_info.پلنوالی < caret_info.کرښه_len)
    {
      /* We cannot quote the whole line.  Make sure we can see the
         beginning of the location.  */
      caret_info.skip = caret_info.پلنوالی < ستون ? ستون - 10 : 0;
    }
  /* If we skip the initial part, we insert "..." before.  */
  إذا (caret_info.skip)
    caret_info.پلنوالی -= ellipsize;
  /* If the end of line does not fit, we also need to truncate the
     end, and leave "..." there.  */
  إذا (caret_info.پلنوالی < caret_info.کرښه_len - caret_info.skip)
    caret_info.پلنوالی -= ellipsize;
  أرجع true;
}

فراغ
location_caret (location loc, ثابت حرف *style, FILE *بهر)
{
  إذا (!(feature_flag & feature_caret))
    أرجع;
  إذا (!loc.پیل.کرښه)
    أرجع;
  إذا (!caret_سېټ_file (loc.پیل.file))
    أرجع;
  إذا (!caret_سېټ_کرښه (loc.پیل.کرښه))
    أرجع;
  إذا (!caret_سېټ_ستنه (loc.پیل.ستنه))
    أرجع;

  ثابت صحيح پلنوالی = caret_info.پلنوالی;
  ثابت صحيح skip = caret_info.skip;

  /* Read the actual line.  Don't update the offset, so that we keep a pointer
     to the start of the line.  */
  {
    mbchar_t c;
    caret_getc (c);
    إذا (!mb_iseof (c))
      {
        /* The last column to highlight.  Only the first line of
           multiline locations are quoted, in which case the ending
           column is the end of line.

           We used to work with byte offsets, and that was much
           easier.  However, we went back to using (visual) columns to
           support truncating of long lines.  */
        ثابت صحيح ستون_پای_ټکی
          = loc.پیل.کرښه == loc.پای_ټکی.کرښه
          ? loc.پای_ټکی.ستنه
          : caret_info.کرښه_len;
        /* Quote the file (at most the first line in the case of
           multiline locations).  */
        {
          fprintf (بهر, "%5d | %s", loc.پیل.کرښه, skip ? ellipsis : "");
          /* Whether we opened the style.  If the line is not as
             expected (maybe the file was changed since the scanner
             ran), we might reach the end before we actually saw the
             opening column.  */
          تعداد { before, inside, after } state = before;
          طالما (!mb_iseof (c) && !mb_iseq (c, '\n'))
            {
              // We might have already opened (and even closed!) the
              // style and yet have the equality of the columns if we
              // just saw zero-width characters.
              إذا (state == before
                  && caret_info.pos.ستنه == loc.پیل.ستنه)
                {
                  begin_use_class (style, بهر);
                  state = inside;
                }
              إذا (skip < caret_info.pos.ستنه)
                mb_putc (c, بهر);
              boundary_compute (&caret_info.pos, mb_ptr (c), mb_len (c));
              caret_getc (c);
              إذا (state == inside
                  && (caret_info.pos.ستنه == ستون_پای_ټکی
                      || پلنوالی < caret_info.pos.ستنه - skip))
                {
                  پای_ټکی_use_class (style, بهر);
                  state = after;
                }
              إذا (پلنوالی < caret_info.pos.ستنه - skip)
                {
                  fputs (ellipsis, بهر);
                  اكسر;
                }
            }
          إذا (state == inside)
            {
              // The line is shorter than expected.
              پای_ټکی_use_class (style, بهر);
              state = after;
            }
          putc ('\n', بهر);
        }

        /* Print the carets with the same indentation as above.  */
        {
          fprintf (بهر, "      | %*s",
                   loc.پیل.ستنه - 1 - skip + (skip ? ellipsize : 0), "");
          begin_use_class (style, بهر);
          putc ('^', بهر);
          /* Underlining a multiline location ends with the first
             line.  */
          لكل (صحيح i = loc.پیل.ستنه - 1 - skip + 1,
                 i_پای_ټکی = min_int (ستون_پای_ټکی - 1 - skip, پلنوالی);
               i < i_پای_ټکی; ++i)
            putc ('~', بهر);
          پای_ټکی_use_class (style, بهر);
          putc ('\n', بهر);
        }
      }
  }
}

فراغ
location_caret_suggestion (location loc, ثابت حرف *s, FILE *بهر)
{
  إذا (!(feature_flag & feature_caret))
    أرجع;
  ثابت حرف *style = "fixit-insert";
  fprintf (بهر, "      | %*s",
           loc.پیل.ستنه - 1 - caret_info.skip
           + (caret_info.skip ? ellipsize : 0),
           "");
  begin_use_class (style, بهر);
  fputs (s, بهر);
  پای_ټکی_use_class (style, بهر);
  putc ('\n', بهر);
}

bool
location_تشه (location loc)
{
  أرجع !loc.پیل.file && !loc.پیل.کرښه && !loc.پیل.ستنه
    && !loc.پای_ټکی.file && !loc.پای_ټکی.کرښه && !loc.پای_ټکی.ستنه;
}

ساكن مضمن صحيح
str_منزل_int (ثابت حرف *s)
{
  اوږد l = strtol (s, عدم, 10);
  أرجع l < 0 ? -1 : l <= لوی_عدد ? l : لوی_عدد;
}

فراغ
boundary_سېټ_from_string (boundary *bound, حرف *str)
{
  /* Search backwards: the file name may contain '.'  or ':'.  */
  حرف *at = strrchr (str, '@');
  إذا (at)
    {
      *at = '\0';
      bound->byte = str_منزل_int (at + 1);
    }
  {
    حرف *dot = strrchr (str, '.');
    aver (dot);
    *dot = '\0';
    bound->ستنه = str_منزل_int (dot + 1);
    إذا (!at)
      bound->byte = bound->ستنه;
  }
  {
    حرف *colon = strrchr (str, ':');
    aver (colon);
    *colon = '\0';
    bound->کرښه = str_منزل_int (colon + 1);
  }
  bound->file = uniqstr_new (str);
}
