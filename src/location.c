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

#جے_تعریف_شدہ WINSIZE_وچ_PTEM
# include <sys/stream.h>
# include <sys/ptem.h>
#جے_انت

#ضمن "complain.h"
#ضمن "getargs.h"
#ضمن "location.h"

location ثابت خالی_تھاواں_loc = خالی_تھاواں_LOCATION_INIT;

/* The terminal width.  Not less than 40.  */
ساكن صحيح
columns (فراغ)
{
  ثابت حرف *cp = getenv ("COLUMNS");
  صحيح res = 80;
  إذا (cp && *cp)
    {
      لما l = strtol (cp, عدم, 10);
      res = 0 <= l && l <= ودھ_توں_ودھ_عدد ? l : ودھ_توں_ودھ_عدد;
    }
  وإلا
    {
#جے_تعریف_شدہ TIOCGWINSZ
      بنية winsize ws;
      إذا (ioctl (STDERR_FILENO, TIOCGWINSZ, &ws) != -1
          && 0 < ws.ws_خانہ && ws.ws_خانہ == (size_t) ws.ws_خانہ)
        res = ws.ws_خانہ;
#جے_انت
    }
  أرجع max_int (res, 40);
}

/* Available screen width.  */
ساكن صحيح screen_چوڑائی = 80;

/* The ellipsis symbol to use for this locale, and the number of
   screen-columns it uses.  */
ساكن ثابت حرف *ellipsis = "...";
ساكن صحيح ellipsize = 3;

/* If BUF is null, add BUFSIZE (which in this case must be less than
   INT_MAX) to COLUMN; otherwise, add mbsnwidth (BUF, BUFSIZE, 0) to
   COLUMN.  If an overflow occurs, return INT_MAX.  */

ساكن مضمن صحيح
جوڑو_کالم_چوڑائی (صحيح کالم, حرف ثابت *buf, size_t bufsize)
{
  صحيح چوڑائی
    = buf ? mbsnwidth (buf, bufsize, 0)
    : ودھ_توں_ودھ_عدد <= bufsize ? ودھ_توں_ودھ_عدد
    : bufsize;
  أرجع کالم <= ودھ_توں_ودھ_عدد - چوڑائی ? کالم + چوڑائی : ودھ_توں_ودھ_عدد;
}

ساكن فراغ
boundary_compute (boundary *cur, حرف ثابت *token, size_t ناپ)
{
  صحيح سطر = cur->سطر;
  صحيح کالم = cur->کالم;
  صحيح byte = cur->byte;
  حرف ثابت *p0 = token;
  حرف ثابت *p = token;
  حرف ثابت *lim = token + ناپ;

  لكل (p = token; p < lim; ++p)
    بدل (*p)
      {
      حالة '\n':
        سطر += سطر < ودھ_توں_ودھ_عدد;
        کالم = 1;
        byte = 1;
        p0 = p + 1;
        اكسر;

      حالة '\t':
        کالم = جوڑو_کالم_چوڑائی (کالم, p0, p - p0);
        کالم = جوڑو_کالم_چوڑائی (کالم, عدم, 8 - ((کالم - 1) & 7));
        p0 = p + 1;
        byte += byte < ودھ_توں_ودھ_عدد;
        اكسر;

      افتراضي:
        byte += byte < ودھ_توں_ودھ_عدد;
        اكسر;
      }
  کالم = جوڑو_کالم_چوڑائی (کالم, p0, p - p0);

  cur->سطر = سطر;
  cur->کالم = کالم;
  cur->byte = byte;
}


/* Set *LOC and adjust scanner cursor to account for token TOKEN of
   size SIZE.  */

فراغ
location_compute (location *loc, boundary *cur, حرف ثابت *token, size_t ناپ)
{
  loc->شروع = *cur;
  boundary_compute (cur, token, ناپ);
  loc->سرا = *cur;

  إذا (loc->سرا.سطر == ودھ_توں_ودھ_عدد && loc->شروع.سطر != ودھ_توں_ودھ_عدد)
    complain (loc, Wother, _("line number overflow"));
  إذا (loc->سرا.کالم == ودھ_توں_ودھ_عدد && loc->شروع.کالم != ودھ_توں_ودھ_عدد)
    complain (loc, Wother, _("column number overflow"));
  /* TRANSLATORS: we are counting bytes, and there are too many.  */
  إذا (loc->سرا.byte == ودھ_توں_ودھ_عدد && loc->شروع.byte != ودھ_توں_ودھ_عدد)
    complain (loc, Wother, _("byte number overflow"));
}

ساكن صحيح
boundary_print (boundary ثابت *b, FILE *باہرلا)
{
  أرجع fprintf (باہرلا, "%s:%d.%d@%d",
                  quotearg_n_style (3, escape_quoting_style, b->file),
                  b->سطر, b->کالم, b->byte);
}

صحيح
location_print (location loc, FILE *باہرلا)
{
  صحيح res = 0;
  إذا (location_خالی_تھاواں (loc))
    res += fprintf (باہرلا, "(empty location)");
  وإلا إذا (trace_flag & trace_locations)
    {
      res += boundary_print (&loc.شروع, باہرلا);
      res += fprintf (باہرلا, "-");
      res += boundary_print (&loc.سرا, باہرلا);
    }
  وإلا
    {
      aver (loc.شروع.file);
      aver (loc.سرا.file);
      صحيح سرا_خانہ = 0 != loc.سرا.کالم ? loc.سرا.کالم - 1 : 0;
      res += fprintf (باہرلا, "%s",
                      quotearg_n_style (3, escape_quoting_style, loc.شروع.file));
      إذا (0 < loc.شروع.سطر)
        {
          res += fprintf (باہرلا, ":%d", loc.شروع.سطر);
          إذا (0 < loc.شروع.کالم)
            res += fprintf (باہرلا, ".%d", loc.شروع.کالم);
        }
      إذا (loc.شروع.file != loc.سرا.file)
        {
          res += fprintf (باہرلا, "-%s",
                          quotearg_n_style (3, escape_quoting_style,
                                            loc.سرا.file));
          إذا (0 < loc.سرا.سطر)
            {
              res += fprintf (باہرلا, ":%d", loc.سرا.سطر);
              إذا (0 <= سرا_خانہ)
                res += fprintf (باہرلا, ".%d", سرا_خانہ);
            }
        }
      وإلا إذا (0 < loc.سرا.سطر)
        {
          إذا (loc.شروع.سطر < loc.سرا.سطر)
            {
              res += fprintf (باہرلا, "-%d", loc.سرا.سطر);
              إذا (0 <= سرا_خانہ)
                res += fprintf (باہرلا, ".%d", سرا_خانہ);
            }
          وإلا إذا (0 <= سرا_خانہ && loc.شروع.کالم < سرا_خانہ)
            res += fprintf (باہرلا, "-%d", سرا_خانہ);
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
  size_t وتھ;
  /* Length of the current line.  */
  صحيح سطر_len;
  /* Given the initial column to display, the offset (number of
     characters to skip at the beginning of the line). */
  صحيح skip;

  /* Available width to quote the source file.  Eight chars are
     consumed by the left-margin (with line number).  */
  صحيح چوڑائی;
} caret_info;

فراغ caret_init (فراغ)
{
  screen_چوڑائی = columns ();
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
caret_سیٹ_file (ثابت حرف *file)
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
              && buf.st_منوال & S_IFREG)
            {
              caret_info.pos.سطر = 1;
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
caret_getc_اندرونی (mbchar_t *res)
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

#تعریف_کرو caret_getc(Var) caret_getc_اندرونی(&Var)

/* Move CARET_INFO (which has a valid FILE) to the line number LINE.
   Compute and cache that line's length in CARET_INFO.LINE_LEN.
   Return whether successful.  */
ساكن bool
caret_سیٹ_سطر (صحيح سطر)
{
  /* If the line we want to quote is seekable (the same line as the previous
     location), just seek it. If it was a previous line, we lost track of it,
     so return to the start of file.  */
  إذا (سطر < caret_info.pos.سطر)
    {
      caret_info.pos.سطر = 1;
      caret_info.وتھ = 0;
    }
  إذا (fseek (caret_info.file, caret_info.وتھ, SEEK_SET))
    أرجع false;

  /* If this is the same line as the previous one, we are done. */
  إذا (سطر < caret_info.pos.سطر)
    أرجع true;

  /* Advance to the line's position, keeping track of the offset.  */
  طالما (caret_info.pos.سطر < سطر)
    {
      mbchar_t c;
      caret_getc (c);
      إذا (mb_iseof (c))
        /* Something is wrong, that line number does not exist.  */
        أرجع false;
      caret_info.pos.سطر += mb_iseq (c, '\n');
    }
  caret_info.وتھ = ftell (caret_info.file);
  caret_info.pos.کالم = 1;
  /* Reset mbf's internal state.
     FIXME: should be done in mbfile.  */
  caret_info.mbfile.eof_تکیا = 0;

  /* Find the number of columns of this line.  */
  طالما (true)
    {
      mbchar_t c;
      caret_getc (c);
      إذا (mb_iseof (c) || mb_iseq (c, '\n'))
        اكسر;
      boundary_compute (&caret_info.pos, mb_ptr (c), mb_len (c));
    }
  caret_info.سطر_len = caret_info.pos.کالم;
  /* Go back to the beginning of line.  */
  إذا (fseek (caret_info.file, caret_info.وتھ, SEEK_SET))
    أرجع false;
  /* Reset mbf's internal state.
     FIXME: should be done in mbfile.  */
  caret_info.mbfile.eof_تکیا = 0;
  caret_info.pos.کالم = 1;
  أرجع true;
}

/* Compute CARET_INFO.WIDTH and CARET_INFO.SKIP based on the fact that
   the first column to display in the current line is COL.  */
ساكن bool
caret_سیٹ_کالم (صحيح خانہ)
{
  /* Available width.  Eight chars are consumed by the left-margin
     (with line number).  */
  caret_info.چوڑائی = screen_چوڑائی - 8;
  caret_info.skip = 0;
  إذا (caret_info.چوڑائی < caret_info.سطر_len)
    {
      /* We cannot quote the whole line.  Make sure we can see the
         beginning of the location.  */
      caret_info.skip = caret_info.چوڑائی < خانہ ? خانہ - 10 : 0;
    }
  /* If we skip the initial part, we insert "..." before.  */
  إذا (caret_info.skip)
    caret_info.چوڑائی -= ellipsize;
  /* If the end of line does not fit, we also need to truncate the
     end, and leave "..." there.  */
  إذا (caret_info.چوڑائی < caret_info.سطر_len - caret_info.skip)
    caret_info.چوڑائی -= ellipsize;
  أرجع true;
}

فراغ
location_caret (location loc, ثابت حرف *style, FILE *باہرلا)
{
  إذا (!(feature_flag & feature_caret))
    أرجع;
  إذا (!loc.شروع.سطر)
    أرجع;
  إذا (!caret_سیٹ_file (loc.شروع.file))
    أرجع;
  إذا (!caret_سیٹ_سطر (loc.شروع.سطر))
    أرجع;
  إذا (!caret_سیٹ_کالم (loc.شروع.کالم))
    أرجع;

  ثابت صحيح چوڑائی = caret_info.چوڑائی;
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
        ثابت صحيح خانہ_سرا
          = loc.شروع.سطر == loc.سرا.سطر
          ? loc.سرا.کالم
          : caret_info.سطر_len;
        /* Quote the file (at most the first line in the case of
           multiline locations).  */
        {
          fprintf (باہرلا, "%5d | %s", loc.شروع.سطر, skip ? ellipsis : "");
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
                  && caret_info.pos.کالم == loc.شروع.کالم)
                {
                  begin_use_class (style, باہرلا);
                  state = inside;
                }
              إذا (skip < caret_info.pos.کالم)
                mb_putc (c, باہرلا);
              boundary_compute (&caret_info.pos, mb_ptr (c), mb_len (c));
              caret_getc (c);
              إذا (state == inside
                  && (caret_info.pos.کالم == خانہ_سرا
                      || چوڑائی < caret_info.pos.کالم - skip))
                {
                  سرا_use_class (style, باہرلا);
                  state = after;
                }
              إذا (چوڑائی < caret_info.pos.کالم - skip)
                {
                  fputs (ellipsis, باہرلا);
                  اكسر;
                }
            }
          إذا (state == inside)
            {
              // The line is shorter than expected.
              سرا_use_class (style, باہرلا);
              state = after;
            }
          putc ('\n', باہرلا);
        }

        /* Print the carets with the same indentation as above.  */
        {
          fprintf (باہرلا, "      | %*s",
                   loc.شروع.کالم - 1 - skip + (skip ? ellipsize : 0), "");
          begin_use_class (style, باہرلا);
          putc ('^', باہرلا);
          /* Underlining a multiline location ends with the first
             line.  */
          لكل (صحيح i = loc.شروع.کالم - 1 - skip + 1,
                 i_سرا = min_int (خانہ_سرا - 1 - skip, چوڑائی);
               i < i_سرا; ++i)
            putc ('~', باہرلا);
          سرا_use_class (style, باہرلا);
          putc ('\n', باہرلا);
        }
      }
  }
}

فراغ
location_caret_suggestion (location loc, ثابت حرف *s, FILE *باہرلا)
{
  إذا (!(feature_flag & feature_caret))
    أرجع;
  ثابت حرف *style = "fixit-insert";
  fprintf (باہرلا, "      | %*s",
           loc.شروع.کالم - 1 - caret_info.skip
           + (caret_info.skip ? ellipsize : 0),
           "");
  begin_use_class (style, باہرلا);
  fputs (s, باہرلا);
  سرا_use_class (style, باہرلا);
  putc ('\n', باہرلا);
}

bool
location_خالی_تھاواں (location loc)
{
  أرجع !loc.شروع.file && !loc.شروع.سطر && !loc.شروع.کالم
    && !loc.سرا.file && !loc.سرا.سطر && !loc.سرا.کالم;
}

ساكن مضمن صحيح
str_منزل_int (ثابت حرف *s)
{
  لما l = strtol (s, عدم, 10);
  أرجع l < 0 ? -1 : l <= ودھ_توں_ودھ_عدد ? l : ودھ_توں_ودھ_عدد;
}

فراغ
boundary_سیٹ_from_string (boundary *bound, حرف *str)
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
    bound->کالم = str_منزل_int (dot + 1);
    إذا (!at)
      bound->byte = bound->کالم;
  }
  {
    حرف *colon = strrchr (str, ':');
    aver (colon);
    *colon = '\0';
    bound->سطر = str_منزل_int (colon + 1);
  }
  bound->file = uniqstr_new (str);
}
