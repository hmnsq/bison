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

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ WINSIZE_ಒಳ_PTEM
# include <sys/stream.h>
# include <sys/ptem.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "complain.h"
#ಅನ್ತರ್ಭಾವ "getargs.h"
#ಅನ್ತರ್ಭಾವ "location.h"

location ಸ್ಥಿರಾಂಕ ಖಾಲಿಗಳು_loc = ಖಾಲಿಗಳು_LOCATION_INIT;

/* The terminal width.  Not less than 40.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
columns (ಶೂನ್ಯ)
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cp = getenv ("COLUMNS");
  ಪೂರ್ಣಾಂಕ res = 80;
  ಆದರೆ (cp && *cp)
    {
      ದೀರ್ಘ l = strtol (cp, ಖಾಲಿ, 10);
      res = 0 <= l && l <= ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ ? l : ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ TIOCGWINSZ
      ರಚನೆ winsize ws;
      ಆದರೆ (ioctl (STDERR_FILENO, TIOCGWINSZ, &ws) != -1
          && 0 < ws.ws_ಲಂಬಸಾಲು && ws.ws_ಲಂಬಸಾಲು == (size_t) ws.ws_ಲಂಬಸಾಲು)
        res = ws.ws_ಲಂಬಸಾಲು;
#ಆದರೆ_ಅಂತ್ಯ
    }
  ಹಿಂತಿರುಗಿ max_int (res, 40);
}

/* Available screen width.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ screen_ಅಗಲ = 80;

/* The ellipsis symbol to use for this locale, and the number of
   screen-columns it uses.  */
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ellipsis = "...";
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ellipsize = 3;

/* If BUF is null, add BUFSIZE (which in this case must be less than
   INT_MAX) to COLUMN; otherwise, add mbsnwidth (BUF, BUFSIZE, 0) to
   COLUMN.  If an overflow occurs, return INT_MAX.  */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
ಕೂಡಿಸು_ಸ್ತಂಭ_ಅಗಲ (ಪೂರ್ಣಾಂಕ ಸ್ತಂಭ, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *buf, size_t bufsize)
{
  ಪೂರ್ಣಾಂಕ ಅಗಲ
    = buf ? mbsnwidth (buf, bufsize, 0)
    : ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ <= bufsize ? ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ
    : bufsize;
  ಹಿಂತಿರುಗಿ ಸ್ತಂಭ <= ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ - ಅಗಲ ? ಸ್ತಂಭ + ಅಗಲ : ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ;
}

ಸ್ಥಿರ ಶೂನ್ಯ
boundary_compute (boundary *cur, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *token, size_t ಗಾತ್ರ_2)
{
  ಪೂರ್ಣಾಂಕ ಸಾಲು = cur->ಸಾಲು;
  ಪೂರ್ಣಾಂಕ ಸ್ತಂಭ = cur->ಸ್ತಂಭ;
  ಪೂರ್ಣಾಂಕ byte = cur->byte;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *p0 = token;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *p = token;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *lim = token + ಗಾತ್ರ_2;

  ಗಾಗಿ (p = token; p < lim; ++p)
    ಆಯ್ಕೆ (*p)
      {
      ಸ್ಥಿತಿ '\n':
        ಸಾಲು += ಸಾಲು < ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ;
        ಸ್ತಂಭ = 1;
        byte = 1;
        p0 = p + 1;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ '\t':
        ಸ್ತಂಭ = ಕೂಡಿಸು_ಸ್ತಂಭ_ಅಗಲ (ಸ್ತಂಭ, p0, p - p0);
        ಸ್ತಂಭ = ಕೂಡಿಸು_ಸ್ತಂಭ_ಅಗಲ (ಸ್ತಂಭ, ಖಾಲಿ, 8 - ((ಸ್ತಂಭ - 1) & 7));
        p0 = p + 1;
        byte += byte < ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ;
        ನಿಲ್ಲಿಸು;

      ಪೂರ್ವನಿಯೋಜಿತ:
        byte += byte < ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ;
        ನಿಲ್ಲಿಸು;
      }
  ಸ್ತಂಭ = ಕೂಡಿಸು_ಸ್ತಂಭ_ಅಗಲ (ಸ್ತಂಭ, p0, p - p0);

  cur->ಸಾಲು = ಸಾಲು;
  cur->ಸ್ತಂಭ = ಸ್ತಂಭ;
  cur->byte = byte;
}


/* Set *LOC and adjust scanner cursor to account for token TOKEN of
   size SIZE.  */

ಶೂನ್ಯ
location_compute (location *loc, boundary *cur, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *token, size_t ಗಾತ್ರ_2)
{
  loc->ಆರಂಭ = *cur;
  boundary_compute (cur, token, ಗಾತ್ರ_2);
  loc->ಕೊನೆ = *cur;

  ಆದರೆ (loc->ಕೊನೆ.ಸಾಲು == ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ && loc->ಆರಂಭ.ಸಾಲು != ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ)
    complain (loc, Wother, _("line number overflow"));
  ಆದರೆ (loc->ಕೊನೆ.ಸ್ತಂಭ == ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ && loc->ಆರಂಭ.ಸ್ತಂಭ != ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ)
    complain (loc, Wother, _("column number overflow"));
  /* TRANSLATORS: we are counting bytes, and there are too many.  */
  ಆದರೆ (loc->ಕೊನೆ.byte == ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ && loc->ಆರಂಭ.byte != ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ)
    complain (loc, Wother, _("byte number overflow"));
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
boundary_print (boundary ಸ್ಥಿರಾಂಕ *b, FILE *ಹೊರಹೋಗು)
{
  ಹಿಂತಿರುಗಿ fprintf (ಹೊರಹೋಗು, "%s:%d.%d@%d",
                  quotearg_n_style (3, escape_quoting_style, b->file),
                  b->ಸಾಲು, b->ಸ್ತಂಭ, b->byte);
}

ಪೂರ್ಣಾಂಕ
location_print (location loc, FILE *ಹೊರಹೋಗು)
{
  ಪೂರ್ಣಾಂಕ res = 0;
  ಆದರೆ (location_ಖಾಲಿಗಳು (loc))
    res += fprintf (ಹೊರಹೋಗು, "(empty location)");
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (trace_flag & trace_locations)
    {
      res += boundary_print (&loc.ಆರಂಭ, ಹೊರಹೋಗು);
      res += fprintf (ಹೊರಹೋಗು, "-");
      res += boundary_print (&loc.ಕೊನೆ, ಹೊರಹೋಗು);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      aver (loc.ಆರಂಭ.file);
      aver (loc.ಕೊನೆ.file);
      ಪೂರ್ಣಾಂಕ ಕೊನೆ_ಲಂಬಸಾಲು = 0 != loc.ಕೊನೆ.ಸ್ತಂಭ ? loc.ಕೊನೆ.ಸ್ತಂಭ - 1 : 0;
      res += fprintf (ಹೊರಹೋಗು, "%s",
                      quotearg_n_style (3, escape_quoting_style, loc.ಆರಂಭ.file));
      ಆದರೆ (0 < loc.ಆರಂಭ.ಸಾಲು)
        {
          res += fprintf (ಹೊರಹೋಗು, ":%d", loc.ಆರಂಭ.ಸಾಲು);
          ಆದರೆ (0 < loc.ಆರಂಭ.ಸ್ತಂಭ)
            res += fprintf (ಹೊರಹೋಗು, ".%d", loc.ಆರಂಭ.ಸ್ತಂಭ);
        }
      ಆದರೆ (loc.ಆರಂಭ.file != loc.ಕೊನೆ.file)
        {
          res += fprintf (ಹೊರಹೋಗು, "-%s",
                          quotearg_n_style (3, escape_quoting_style,
                                            loc.ಕೊನೆ.file));
          ಆದರೆ (0 < loc.ಕೊನೆ.ಸಾಲು)
            {
              res += fprintf (ಹೊರಹೋಗು, ":%d", loc.ಕೊನೆ.ಸಾಲು);
              ಆದರೆ (0 <= ಕೊನೆ_ಲಂಬಸಾಲು)
                res += fprintf (ಹೊರಹೋಗು, ".%d", ಕೊನೆ_ಲಂಬಸಾಲು);
            }
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (0 < loc.ಕೊನೆ.ಸಾಲು)
        {
          ಆದರೆ (loc.ಆರಂಭ.ಸಾಲು < loc.ಕೊನೆ.ಸಾಲು)
            {
              res += fprintf (ಹೊರಹೋಗು, "-%d", loc.ಕೊನೆ.ಸಾಲು);
              ಆದರೆ (0 <= ಕೊನೆ_ಲಂಬಸಾಲು)
                res += fprintf (ಹೊರಹೋಗು, ".%d", ಕೊನೆ_ಲಂಬಸಾಲು);
            }
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (0 <= ಕೊನೆ_ಲಂಬಸಾಲು && loc.ಆರಂಭ.ಸ್ತಂಭ < ಕೊನೆ_ಲಂಬಸಾಲು)
            res += fprintf (ಹೊರಹೋಗು, "-%d", ಕೊನೆ_ಲಂಬಸಾಲು);
        }
    }

  ಹಿಂತಿರುಗಿ res;
}


/* Persistent data used by location_caret to avoid reopening and rereading the
   same file all over for each error.  */
ಸ್ಥಿರ ರಚನೆ
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
  size_t ಸ್ಥಾನಾಂತರ;
  /* Length of the current line.  */
  ಪೂರ್ಣಾಂಕ ಸಾಲು_len;
  /* Given the initial column to display, the offset (number of
     characters to skip at the beginning of the line). */
  ಪೂರ್ಣಾಂಕ skip;

  /* Available width to quote the source file.  Eight chars are
     consumed by the left-margin (with line number).  */
  ಪೂರ್ಣಾಂಕ ಅಗಲ;
} caret_info;

ಶೂನ್ಯ caret_init (ಶೂನ್ಯ)
{
  screen_ಅಗಲ = columns ();
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
  ಆದರೆ (caret_info.file)
    {
      fclose (caret_info.file);
      caret_info.file = ಖಾಲಿ;
    }
}

/* Open FILE for quoting, if needed, and if possible.  Return whether
   the file can quoted. */
ಸ್ಥಿರ bool
caret_ಗಣ_file (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file)
{
  /* If a different file than before, close and let the rest open
     the new one. */
  ಆದರೆ (caret_info.pos.file && caret_info.pos.file != file)
    {
      caret_free ();
      caret_info.pos.file = ಖಾಲಿ;
    }
  ಆದರೆ (!caret_info.pos.file)
    {
      caret_info.pos.file = file;
      ಆದರೆ ((caret_info.file = fopen (caret_info.pos.file, "rb")))
        {
          /* If the file is not regular (imagine #line 1 "/dev/stdin"
             in the input file for instance), don't try to quote the
             file.  Keep caret_info.pos.file set so that we don't try
             to open it again, but leave caret_info.file NULL so that
             we don't try to quote it. */
          ರಚನೆ stat buf;
          ಆದರೆ (fstat (fileno (caret_info.file), &buf) == 0
              && buf.st_ಬಹುಲಕ & S_IFREG)
            {
              caret_info.pos.ಸಾಲು = 1;
              mbf_init (caret_info.mbfile, caret_info.file);
            }
          ಇಲ್ಲದಿದ್ದರೆ
            caret_free ();
        }
    }
  ಹಿಂತಿರುಗಿ !!caret_info.file;
}

/* Getc, but smash \r\n as \n.  */
ಸ್ಥಿರ ಶೂನ್ಯ
caret_getc_ಆಂತರಿಕ (mbchar_t *res)
{
  mbf_getc (*res, caret_info.mbfile);
  ಆದರೆ (mb_iseq (*res, '\r'))
    {
      mbchar_t c;
      mbf_getc (c, caret_info.mbfile);
      ಆದರೆ (mb_iseq (c, '\n'))
        mb_copy (res, &c);
      ಇಲ್ಲದಿದ್ದರೆ
        mbf_ungetc (c, caret_info.mbfile);
    }
}

#ವ್ಯಾಖ್ಯಾನಿಸು caret_getc(Var) caret_getc_ಆಂತರಿಕ(&Var)

/* Move CARET_INFO (which has a valid FILE) to the line number LINE.
   Compute and cache that line's length in CARET_INFO.LINE_LEN.
   Return whether successful.  */
ಸ್ಥಿರ bool
caret_ಗಣ_ಸಾಲು (ಪೂರ್ಣಾಂಕ ಸಾಲು)
{
  /* If the line we want to quote is seekable (the same line as the previous
     location), just seek it. If it was a previous line, we lost track of it,
     so return to the start of file.  */
  ಆದರೆ (ಸಾಲು < caret_info.pos.ಸಾಲು)
    {
      caret_info.pos.ಸಾಲು = 1;
      caret_info.ಸ್ಥಾನಾಂತರ = 0;
    }
  ಆದರೆ (fseek (caret_info.file, caret_info.ಸ್ಥಾನಾಂತರ, SEEK_SET))
    ಹಿಂತಿರುಗಿ false;

  /* If this is the same line as the previous one, we are done. */
  ಆದರೆ (ಸಾಲು < caret_info.pos.ಸಾಲು)
    ಹಿಂತಿರುಗಿ true;

  /* Advance to the line's position, keeping track of the offset.  */
  ಆಗ (caret_info.pos.ಸಾಲು < ಸಾಲು)
    {
      mbchar_t c;
      caret_getc (c);
      ಆದರೆ (mb_iseof (c))
        /* Something is wrong, that line number does not exist.  */
        ಹಿಂತಿರುಗಿ false;
      caret_info.pos.ಸಾಲು += mb_iseq (c, '\n');
    }
  caret_info.ಸ್ಥಾನಾಂತರ = ftell (caret_info.file);
  caret_info.pos.ಸ್ತಂಭ = 1;
  /* Reset mbf's internal state.
     FIXME: should be done in mbfile.  */
  caret_info.mbfile.eof_ನೋಡಿದ = 0;

  /* Find the number of columns of this line.  */
  ಆಗ (true)
    {
      mbchar_t c;
      caret_getc (c);
      ಆದರೆ (mb_iseof (c) || mb_iseq (c, '\n'))
        ನಿಲ್ಲಿಸು;
      boundary_compute (&caret_info.pos, mb_ptr (c), mb_len (c));
    }
  caret_info.ಸಾಲು_len = caret_info.pos.ಸ್ತಂಭ;
  /* Go back to the beginning of line.  */
  ಆದರೆ (fseek (caret_info.file, caret_info.ಸ್ಥಾನಾಂತರ, SEEK_SET))
    ಹಿಂತಿರುಗಿ false;
  /* Reset mbf's internal state.
     FIXME: should be done in mbfile.  */
  caret_info.mbfile.eof_ನೋಡಿದ = 0;
  caret_info.pos.ಸ್ತಂಭ = 1;
  ಹಿಂತಿರುಗಿ true;
}

/* Compute CARET_INFO.WIDTH and CARET_INFO.SKIP based on the fact that
   the first column to display in the current line is COL.  */
ಸ್ಥಿರ bool
caret_ಗಣ_ಸ್ತಂಭ (ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು)
{
  /* Available width.  Eight chars are consumed by the left-margin
     (with line number).  */
  caret_info.ಅಗಲ = screen_ಅಗಲ - 8;
  caret_info.skip = 0;
  ಆದರೆ (caret_info.ಅಗಲ < caret_info.ಸಾಲು_len)
    {
      /* We cannot quote the whole line.  Make sure we can see the
         beginning of the location.  */
      caret_info.skip = caret_info.ಅಗಲ < ಲಂಬಸಾಲು ? ಲಂಬಸಾಲು - 10 : 0;
    }
  /* If we skip the initial part, we insert "..." before.  */
  ಆದರೆ (caret_info.skip)
    caret_info.ಅಗಲ -= ellipsize;
  /* If the end of line does not fit, we also need to truncate the
     end, and leave "..." there.  */
  ಆದರೆ (caret_info.ಅಗಲ < caret_info.ಸಾಲು_len - caret_info.skip)
    caret_info.ಅಗಲ -= ellipsize;
  ಹಿಂತಿರುಗಿ true;
}

ಶೂನ್ಯ
location_caret (location loc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *style, FILE *ಹೊರಹೋಗು)
{
  ಆದರೆ (!(feature_flag & feature_caret))
    ಹಿಂತಿರುಗಿ;
  ಆದರೆ (!loc.ಆರಂಭ.ಸಾಲು)
    ಹಿಂತಿರುಗಿ;
  ಆದರೆ (!caret_ಗಣ_file (loc.ಆರಂಭ.file))
    ಹಿಂತಿರುಗಿ;
  ಆದರೆ (!caret_ಗಣ_ಸಾಲು (loc.ಆರಂಭ.ಸಾಲು))
    ಹಿಂತಿರುಗಿ;
  ಆದರೆ (!caret_ಗಣ_ಸ್ತಂಭ (loc.ಆರಂಭ.ಸ್ತಂಭ))
    ಹಿಂತಿರುಗಿ;

  ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಅಗಲ = caret_info.ಅಗಲ;
  ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ skip = caret_info.skip;

  /* Read the actual line.  Don't update the offset, so that we keep a pointer
     to the start of the line.  */
  {
    mbchar_t c;
    caret_getc (c);
    ಆದರೆ (!mb_iseof (c))
      {
        /* The last column to highlight.  Only the first line of
           multiline locations are quoted, in which case the ending
           column is the end of line.

           We used to work with byte offsets, and that was much
           easier.  However, we went back to using (visual) columns to
           support truncating of long lines.  */
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_ಕೊನೆ
          = loc.ಆರಂಭ.ಸಾಲು == loc.ಕೊನೆ.ಸಾಲು
          ? loc.ಕೊನೆ.ಸ್ತಂಭ
          : caret_info.ಸಾಲು_len;
        /* Quote the file (at most the first line in the case of
           multiline locations).  */
        {
          fprintf (ಹೊರಹೋಗು, "%5d | %s", loc.ಆರಂಭ.ಸಾಲು, skip ? ellipsis : "");
          /* Whether we opened the style.  If the line is not as
             expected (maybe the file was changed since the scanner
             ran), we might reach the end before we actually saw the
             opening column.  */
          ಎಣಿಕೆ { before, inside, after } state = before;
          ಆಗ (!mb_iseof (c) && !mb_iseq (c, '\n'))
            {
              // We might have already opened (and even closed!) the
              // style and yet have the equality of the columns if we
              // just saw zero-width characters.
              ಆದರೆ (state == before
                  && caret_info.pos.ಸ್ತಂಭ == loc.ಆರಂಭ.ಸ್ತಂಭ)
                {
                  begin_use_class (style, ಹೊರಹೋಗು);
                  state = inside;
                }
              ಆದರೆ (skip < caret_info.pos.ಸ್ತಂಭ)
                mb_putc (c, ಹೊರಹೋಗು);
              boundary_compute (&caret_info.pos, mb_ptr (c), mb_len (c));
              caret_getc (c);
              ಆದರೆ (state == inside
                  && (caret_info.pos.ಸ್ತಂಭ == ಲಂಬಸಾಲು_ಕೊನೆ
                      || ಅಗಲ < caret_info.pos.ಸ್ತಂಭ - skip))
                {
                  ಕೊನೆ_use_class (style, ಹೊರಹೋಗು);
                  state = after;
                }
              ಆದರೆ (ಅಗಲ < caret_info.pos.ಸ್ತಂಭ - skip)
                {
                  fputs (ellipsis, ಹೊರಹೋಗು);
                  ನಿಲ್ಲಿಸು;
                }
            }
          ಆದರೆ (state == inside)
            {
              // The line is shorter than expected.
              ಕೊನೆ_use_class (style, ಹೊರಹೋಗು);
              state = after;
            }
          putc ('\n', ಹೊರಹೋಗು);
        }

        /* Print the carets with the same indentation as above.  */
        {
          fprintf (ಹೊರಹೋಗು, "      | %*s",
                   loc.ಆರಂಭ.ಸ್ತಂಭ - 1 - skip + (skip ? ellipsize : 0), "");
          begin_use_class (style, ಹೊರಹೋಗು);
          putc ('^', ಹೊರಹೋಗು);
          /* Underlining a multiline location ends with the first
             line.  */
          ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = loc.ಆರಂಭ.ಸ್ತಂಭ - 1 - skip + 1,
                 i_ಕೊನೆ = min_int (ಲಂಬಸಾಲು_ಕೊನೆ - 1 - skip, ಅಗಲ);
               i < i_ಕೊನೆ; ++i)
            putc ('~', ಹೊರಹೋಗು);
          ಕೊನೆ_use_class (style, ಹೊರಹೋಗು);
          putc ('\n', ಹೊರಹೋಗು);
        }
      }
  }
}

ಶೂನ್ಯ
location_caret_suggestion (location loc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s, FILE *ಹೊರಹೋಗು)
{
  ಆದರೆ (!(feature_flag & feature_caret))
    ಹಿಂತಿರುಗಿ;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *style = "fixit-insert";
  fprintf (ಹೊರಹೋಗು, "      | %*s",
           loc.ಆರಂಭ.ಸ್ತಂಭ - 1 - caret_info.skip
           + (caret_info.skip ? ellipsize : 0),
           "");
  begin_use_class (style, ಹೊರಹೋಗು);
  fputs (s, ಹೊರಹೋಗು);
  ಕೊನೆ_use_class (style, ಹೊರಹೋಗು);
  putc ('\n', ಹೊರಹೋಗು);
}

bool
location_ಖಾಲಿಗಳು (location loc)
{
  ಹಿಂತಿರುಗಿ !loc.ಆರಂಭ.file && !loc.ಆರಂಭ.ಸಾಲು && !loc.ಆರಂಭ.ಸ್ತಂಭ
    && !loc.ಕೊನೆ.file && !loc.ಕೊನೆ.ಸಾಲು && !loc.ಕೊನೆ.ಸ್ತಂಭ;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
str_ಗಮ್ಯ_int (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s)
{
  ದೀರ್ಘ l = strtol (s, ಖಾಲಿ, 10);
  ಹಿಂತಿರುಗಿ l < 0 ? -1 : l <= ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ ? l : ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ;
}

ಶೂನ್ಯ
boundary_ಗಣ_from_string (boundary *bound, ಅಕ್ಷರ *str)
{
  /* Search backwards: the file name may contain '.'  or ':'.  */
  ಅಕ್ಷರ *at = strrchr (str, '@');
  ಆದರೆ (at)
    {
      *at = '\0';
      bound->byte = str_ಗಮ್ಯ_int (at + 1);
    }
  {
    ಅಕ್ಷರ *dot = strrchr (str, '.');
    aver (dot);
    *dot = '\0';
    bound->ಸ್ತಂಭ = str_ಗಮ್ಯ_int (dot + 1);
    ಆದರೆ (!at)
      bound->byte = bound->ಸ್ತಂಭ;
  }
  {
    ಅಕ್ಷರ *colon = strrchr (str, ':');
    aver (colon);
    *colon = '\0';
    bound->ಸಾಲು = str_ಗಮ್ಯ_int (colon + 1);
  }
  bound->file = uniqstr_new (str);
}
