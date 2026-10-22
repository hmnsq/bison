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

#جيڪڏهن_وصف_ٿيل WINSIZE_اندريون_PTEM
# include <sys/stream.h>
# include <sys/ptem.h>
#جيڪڏهن_پورو

#ضمن "complain.h"
#ضمن "getargs.h"
#ضمن "location.h"

location ثابت خالي_جايون_loc = خالي_جايون_LOCATION_INIT;

/* The terminal width.  Not less than 40.  */
ساكن صحيح
columns (فراغ)
{
  ثابت حرف *cp = getenv ("COLUMNS");
  صحيح res = 80;
  إذا (cp && *cp)
    {
      ڊگهو l = strtol (cp, عدم, 10);
      res = 0 <= l && l <= وڌ_کان_وڌ_عدد ? l : وڌ_کان_وڌ_عدد;
    }
  وإلا
    {
#جيڪڏهن_وصف_ٿيل TIOCGWINSZ
      بنية winsize ws;
      إذا (ioctl (STDERR_FILENO, TIOCGWINSZ, &ws) != -1
          && 0 < ws.ws_کانو && ws.ws_کانو == (size_t) ws.ws_کانو)
        res = ws.ws_کانو;
#جيڪڏهن_پورو
    }
  أرجع max_int (res, 40);
}

/* Available screen width.  */
ساكن صحيح screen_ويڪر = 80;

/* The ellipsis symbol to use for this locale, and the number of
   screen-columns it uses.  */
ساكن ثابت حرف *ellipsis = "...";
ساكن صحيح ellipsize = 3;

/* If BUF is null, add BUFSIZE (which in this case must be less than
   INT_MAX) to COLUMN; otherwise, add mbsnwidth (BUF, BUFSIZE, 0) to
   COLUMN.  If an overflow occurs, return INT_MAX.  */

ساكن مضمن صحيح
جوڙيو_ڪالم_ويڪر (صحيح ڪالم, حرف ثابت *buf, size_t bufsize)
{
  صحيح ويڪر
    = buf ? mbsnwidth (buf, bufsize, 0)
    : وڌ_کان_وڌ_عدد <= bufsize ? وڌ_کان_وڌ_عدد
    : bufsize;
  أرجع ڪالم <= وڌ_کان_وڌ_عدد - ويڪر ? ڪالم + ويڪر : وڌ_کان_وڌ_عدد;
}

ساكن فراغ
boundary_compute (boundary *cur, حرف ثابت *token, size_t سائيز)
{
  صحيح سٽ = cur->سٽ;
  صحيح ڪالم = cur->ڪالم;
  صحيح byte = cur->byte;
  حرف ثابت *p0 = token;
  حرف ثابت *p = token;
  حرف ثابت *lim = token + سائيز;

  لكل (p = token; p < lim; ++p)
    بدل (*p)
      {
      حالة '\n':
        سٽ += سٽ < وڌ_کان_وڌ_عدد;
        ڪالم = 1;
        byte = 1;
        p0 = p + 1;
        اكسر;

      حالة '\t':
        ڪالم = جوڙيو_ڪالم_ويڪر (ڪالم, p0, p - p0);
        ڪالم = جوڙيو_ڪالم_ويڪر (ڪالم, عدم, 8 - ((ڪالم - 1) & 7));
        p0 = p + 1;
        byte += byte < وڌ_کان_وڌ_عدد;
        اكسر;

      افتراضي:
        byte += byte < وڌ_کان_وڌ_عدد;
        اكسر;
      }
  ڪالم = جوڙيو_ڪالم_ويڪر (ڪالم, p0, p - p0);

  cur->سٽ = سٽ;
  cur->ڪالم = ڪالم;
  cur->byte = byte;
}


/* Set *LOC and adjust scanner cursor to account for token TOKEN of
   size SIZE.  */

فراغ
location_compute (location *loc, boundary *cur, حرف ثابت *token, size_t سائيز)
{
  loc->شروعات = *cur;
  boundary_compute (cur, token, سائيز);
  loc->پڇاڙي = *cur;

  إذا (loc->پڇاڙي.سٽ == وڌ_کان_وڌ_عدد && loc->شروعات.سٽ != وڌ_کان_وڌ_عدد)
    complain (loc, Wother, _("line number overflow"));
  إذا (loc->پڇاڙي.ڪالم == وڌ_کان_وڌ_عدد && loc->شروعات.ڪالم != وڌ_کان_وڌ_عدد)
    complain (loc, Wother, _("column number overflow"));
  /* TRANSLATORS: we are counting bytes, and there are too many.  */
  إذا (loc->پڇاڙي.byte == وڌ_کان_وڌ_عدد && loc->شروعات.byte != وڌ_کان_وڌ_عدد)
    complain (loc, Wother, _("byte number overflow"));
}

ساكن صحيح
boundary_print (boundary ثابت *b, FILE *ٻاهريون)
{
  أرجع fprintf (ٻاهريون, "%s:%d.%d@%d",
                  quotearg_n_style (3, escape_quoting_style, b->file),
                  b->سٽ, b->ڪالم, b->byte);
}

صحيح
location_print (location loc, FILE *ٻاهريون)
{
  صحيح res = 0;
  إذا (location_خالي_جايون (loc))
    res += fprintf (ٻاهريون, "(empty location)");
  وإلا إذا (trace_flag & trace_locations)
    {
      res += boundary_print (&loc.شروعات, ٻاهريون);
      res += fprintf (ٻاهريون, "-");
      res += boundary_print (&loc.پڇاڙي, ٻاهريون);
    }
  وإلا
    {
      aver (loc.شروعات.file);
      aver (loc.پڇاڙي.file);
      صحيح پڇاڙي_کانو = 0 != loc.پڇاڙي.ڪالم ? loc.پڇاڙي.ڪالم - 1 : 0;
      res += fprintf (ٻاهريون, "%s",
                      quotearg_n_style (3, escape_quoting_style, loc.شروعات.file));
      إذا (0 < loc.شروعات.سٽ)
        {
          res += fprintf (ٻاهريون, ":%d", loc.شروعات.سٽ);
          إذا (0 < loc.شروعات.ڪالم)
            res += fprintf (ٻاهريون, ".%d", loc.شروعات.ڪالم);
        }
      إذا (loc.شروعات.file != loc.پڇاڙي.file)
        {
          res += fprintf (ٻاهريون, "-%s",
                          quotearg_n_style (3, escape_quoting_style,
                                            loc.پڇاڙي.file));
          إذا (0 < loc.پڇاڙي.سٽ)
            {
              res += fprintf (ٻاهريون, ":%d", loc.پڇاڙي.سٽ);
              إذا (0 <= پڇاڙي_کانو)
                res += fprintf (ٻاهريون, ".%d", پڇاڙي_کانو);
            }
        }
      وإلا إذا (0 < loc.پڇاڙي.سٽ)
        {
          إذا (loc.شروعات.سٽ < loc.پڇاڙي.سٽ)
            {
              res += fprintf (ٻاهريون, "-%d", loc.پڇاڙي.سٽ);
              إذا (0 <= پڇاڙي_کانو)
                res += fprintf (ٻاهريون, ".%d", پڇاڙي_کانو);
            }
          وإلا إذا (0 <= پڇاڙي_کانو && loc.شروعات.ڪالم < پڇاڙي_کانو)
            res += fprintf (ٻاهريون, "-%d", پڇاڙي_کانو);
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
  size_t وٿي;
  /* Length of the current line.  */
  صحيح سٽ_len;
  /* Given the initial column to display, the offset (number of
     characters to skip at the beginning of the line). */
  صحيح skip;

  /* Available width to quote the source file.  Eight chars are
     consumed by the left-margin (with line number).  */
  صحيح ويڪر;
} caret_info;

فراغ caret_init (فراغ)
{
  screen_ويڪر = columns ();
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
caret_سيٽ_file (ثابت حرف *file)
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
              && buf.st_گهڻائي & S_IFREG)
            {
              caret_info.pos.سٽ = 1;
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
caret_getc_اندروني (mbchar_t *res)
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

#وصف_ڪريو caret_getc(Var) caret_getc_اندروني(&Var)

/* Move CARET_INFO (which has a valid FILE) to the line number LINE.
   Compute and cache that line's length in CARET_INFO.LINE_LEN.
   Return whether successful.  */
ساكن bool
caret_سيٽ_سٽ (صحيح سٽ)
{
  /* If the line we want to quote is seekable (the same line as the previous
     location), just seek it. If it was a previous line, we lost track of it,
     so return to the start of file.  */
  إذا (سٽ < caret_info.pos.سٽ)
    {
      caret_info.pos.سٽ = 1;
      caret_info.وٿي = 0;
    }
  إذا (fseek (caret_info.file, caret_info.وٿي, SEEK_SET))
    أرجع false;

  /* If this is the same line as the previous one, we are done. */
  إذا (سٽ < caret_info.pos.سٽ)
    أرجع true;

  /* Advance to the line's position, keeping track of the offset.  */
  طالما (caret_info.pos.سٽ < سٽ)
    {
      mbchar_t c;
      caret_getc (c);
      إذا (mb_iseof (c))
        /* Something is wrong, that line number does not exist.  */
        أرجع false;
      caret_info.pos.سٽ += mb_iseq (c, '\n');
    }
  caret_info.وٿي = ftell (caret_info.file);
  caret_info.pos.ڪالم = 1;
  /* Reset mbf's internal state.
     FIXME: should be done in mbfile.  */
  caret_info.mbfile.eof_ڏٺل = 0;

  /* Find the number of columns of this line.  */
  طالما (true)
    {
      mbchar_t c;
      caret_getc (c);
      إذا (mb_iseof (c) || mb_iseq (c, '\n'))
        اكسر;
      boundary_compute (&caret_info.pos, mb_ptr (c), mb_len (c));
    }
  caret_info.سٽ_len = caret_info.pos.ڪالم;
  /* Go back to the beginning of line.  */
  إذا (fseek (caret_info.file, caret_info.وٿي, SEEK_SET))
    أرجع false;
  /* Reset mbf's internal state.
     FIXME: should be done in mbfile.  */
  caret_info.mbfile.eof_ڏٺل = 0;
  caret_info.pos.ڪالم = 1;
  أرجع true;
}

/* Compute CARET_INFO.WIDTH and CARET_INFO.SKIP based on the fact that
   the first column to display in the current line is COL.  */
ساكن bool
caret_سيٽ_ڪالم (صحيح کانو)
{
  /* Available width.  Eight chars are consumed by the left-margin
     (with line number).  */
  caret_info.ويڪر = screen_ويڪر - 8;
  caret_info.skip = 0;
  إذا (caret_info.ويڪر < caret_info.سٽ_len)
    {
      /* We cannot quote the whole line.  Make sure we can see the
         beginning of the location.  */
      caret_info.skip = caret_info.ويڪر < کانو ? کانو - 10 : 0;
    }
  /* If we skip the initial part, we insert "..." before.  */
  إذا (caret_info.skip)
    caret_info.ويڪر -= ellipsize;
  /* If the end of line does not fit, we also need to truncate the
     end, and leave "..." there.  */
  إذا (caret_info.ويڪر < caret_info.سٽ_len - caret_info.skip)
    caret_info.ويڪر -= ellipsize;
  أرجع true;
}

فراغ
location_caret (location loc, ثابت حرف *style, FILE *ٻاهريون)
{
  إذا (!(feature_flag & feature_caret))
    أرجع;
  إذا (!loc.شروعات.سٽ)
    أرجع;
  إذا (!caret_سيٽ_file (loc.شروعات.file))
    أرجع;
  إذا (!caret_سيٽ_سٽ (loc.شروعات.سٽ))
    أرجع;
  إذا (!caret_سيٽ_ڪالم (loc.شروعات.ڪالم))
    أرجع;

  ثابت صحيح ويڪر = caret_info.ويڪر;
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
        ثابت صحيح کانو_پڇاڙي
          = loc.شروعات.سٽ == loc.پڇاڙي.سٽ
          ? loc.پڇاڙي.ڪالم
          : caret_info.سٽ_len;
        /* Quote the file (at most the first line in the case of
           multiline locations).  */
        {
          fprintf (ٻاهريون, "%5d | %s", loc.شروعات.سٽ, skip ? ellipsis : "");
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
                  && caret_info.pos.ڪالم == loc.شروعات.ڪالم)
                {
                  begin_use_class (style, ٻاهريون);
                  state = inside;
                }
              إذا (skip < caret_info.pos.ڪالم)
                mb_putc (c, ٻاهريون);
              boundary_compute (&caret_info.pos, mb_ptr (c), mb_len (c));
              caret_getc (c);
              إذا (state == inside
                  && (caret_info.pos.ڪالم == کانو_پڇاڙي
                      || ويڪر < caret_info.pos.ڪالم - skip))
                {
                  پڇاڙي_use_class (style, ٻاهريون);
                  state = after;
                }
              إذا (ويڪر < caret_info.pos.ڪالم - skip)
                {
                  fputs (ellipsis, ٻاهريون);
                  اكسر;
                }
            }
          إذا (state == inside)
            {
              // The line is shorter than expected.
              پڇاڙي_use_class (style, ٻاهريون);
              state = after;
            }
          putc ('\n', ٻاهريون);
        }

        /* Print the carets with the same indentation as above.  */
        {
          fprintf (ٻاهريون, "      | %*s",
                   loc.شروعات.ڪالم - 1 - skip + (skip ? ellipsize : 0), "");
          begin_use_class (style, ٻاهريون);
          putc ('^', ٻاهريون);
          /* Underlining a multiline location ends with the first
             line.  */
          لكل (صحيح i = loc.شروعات.ڪالم - 1 - skip + 1,
                 i_پڇاڙي = min_int (کانو_پڇاڙي - 1 - skip, ويڪر);
               i < i_پڇاڙي; ++i)
            putc ('~', ٻاهريون);
          پڇاڙي_use_class (style, ٻاهريون);
          putc ('\n', ٻاهريون);
        }
      }
  }
}

فراغ
location_caret_suggestion (location loc, ثابت حرف *s, FILE *ٻاهريون)
{
  إذا (!(feature_flag & feature_caret))
    أرجع;
  ثابت حرف *style = "fixit-insert";
  fprintf (ٻاهريون, "      | %*s",
           loc.شروعات.ڪالم - 1 - caret_info.skip
           + (caret_info.skip ? ellipsize : 0),
           "");
  begin_use_class (style, ٻاهريون);
  fputs (s, ٻاهريون);
  پڇاڙي_use_class (style, ٻاهريون);
  putc ('\n', ٻاهريون);
}

bool
location_خالي_جايون (location loc)
{
  أرجع !loc.شروعات.file && !loc.شروعات.سٽ && !loc.شروعات.ڪالم
    && !loc.پڇاڙي.file && !loc.پڇاڙي.سٽ && !loc.پڇاڙي.ڪالم;
}

ساكن مضمن صحيح
str_منزل_int (ثابت حرف *s)
{
  ڊگهو l = strtol (s, عدم, 10);
  أرجع l < 0 ? -1 : l <= وڌ_کان_وڌ_عدد ? l : وڌ_کان_وڌ_عدد;
}

فراغ
boundary_سيٽ_from_string (boundary *bound, حرف *str)
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
    bound->ڪالم = str_منزل_int (dot + 1);
    إذا (!at)
      bound->byte = bound->ڪالم;
  }
  {
    حرف *colon = strrchr (str, ':');
    aver (colon);
    *colon = '\0';
    bound->سٽ = str_منزل_int (colon + 1);
  }
  bound->file = uniqstr_new (str);
}
