/* system.h: system-dependent declarations; include this first.

   Copyright 1996, 2005, 2006, 2007, 2008, 2013 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#ifndef HELLO_SYSTEM_H
# define HELLO_SYSTEM_H

/* Assume ANSI C89 headers are available.  */
# include <locale.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

/* Use POSIX headers.  If they are not available, we use the substitute
   provided by gnulib.  */
# include <getopt.h>
# include <unistd.h>

/* Unicode. */
#include <wchar.h>
#include <wctype.h>

/* CHAR_MIN */
#include <limits.h>

/* Internationalization.  */
# include "gettext.h"
# define _(str) gettext (str)
# define N_(str) gettext_noop (str)

/* Check for errors on write.  */
# include "closeout.h"

/* Usage and version priting.  */
# include "dirname.h"
# include "progname.h"

/* These enum values cannot possibly conflict with the option values
   ordinarily used by commands, including CHAR_MAX + 1, etc.  Avoid
   CHAR_MIN - 1, as it may equal -1, the getopt end-of-options value.  */
enum
{
  GETOPT_HELP_CHAR = (CHAR_MIN - 2),
  GETOPT_VERSION_CHAR = (CHAR_MIN - 3)
};

#define GETOPT_HELP_OPTION_DECL \
  "help", no_argument, NULL, GETOPT_HELP_CHAR
#define GETOPT_VERSION_OPTION_DECL \
  "version", no_argument, NULL, GETOPT_VERSION_CHAR

#define case_GETOPT_HELP_CHAR                   \
  case GETOPT_HELP_CHAR:                        \
    usage (EXIT_SUCCESS);                       \
    break;

#define emit_bug_reporting_address unused__emit_bug_reporting_address
#include "version-etc.h"
#undef emit_bug_reporting_address

#include "propername.h"
/* Define away proper_name (leaving proper_name_utf8, which affects far
   fewer programs), since it's not worth the cost of adding ~17KB to
   the x86_64 text size of every single program.  This avoids a 40%
   (almost ~2MB) increase in the on-disk space utilization for the set
   of the 100 binaries. */
#define proper_name(x) (x)

#define case_GETOPT_VERSION_CHAR(Program_name, Authors)                 \
  case GETOPT_VERSION_CHAR:                                             \
    version_etc (stdout, Program_name, PACKAGE_NAME, VERSION, Authors,  \
                 (char *) NULL);                                        \
    exit (EXIT_SUCCESS);                                                \
    break;

#define HELP_OPTION_DESCRIPTION \
  _("      --help     display this help and exit\n")
#define VERSION_OPTION_DESCRIPTION \
  _("      --version  output version information and exit\n")

static inline void
emit_mandatory_arg_note (void)
{
  fputs (_("\n\
Mandatory arguments to long options are mandatory for short options too.\n\
"), stdout);
}

/* Just like strncmp, but the second argument must be a literal string
   and you don't specify the length;  that comes from the literal.  */
#define STRNCMP_LIT(s, literal) \
  strncmp (s, "" literal "", sizeof (literal) - 1)

static inline void
emit_ancillary_info (void)
{
  printf (_("\n%s online help: <%s>\n"), PACKAGE_NAME, PACKAGE_URL);
  /* Don't output this redundant message for English locales.
     Note we still output for 'C' so that it gets included in the man page.  */
  const char *lc_messages = setlocale (LC_MESSAGES, NULL);
  if (lc_messages && STRNCMP_LIT (lc_messages, "en_"))
    {
      /* TRANSLATORS: Replace LANG_CODE in this URL with your language code
         <http://translationproject.org/team/LANG_CODE.html> to form one of
         the URLs at http://translationproject.org/team/.  Otherwise, replace
         the entire URL with your translation team's email address.  */
      printf (_("Report %s translation bugs to "
                "<http://translationproject.org/team/>\n"),
                last_component (program_name));
    }
  printf (_("For complete documentation, run: "
            "info %s '%s invocation'\n"), PACKAGE, last_component (program_name));
}

static inline void
emit_try_help (void)
{
  fprintf (stderr, _("Try '%s --help' for more information.\n"), program_name);
}

#endif /* HELLO_SYSTEM_H */
