/* hello.c -- print a greeting message and exit.

   Copyright 1992, 1995, 1996, 1997, 1998, 1999, 2000, 2001, 2002, 2005,
   2006, 2007, 2008, 2010, 2011, 2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include <config.h>
#include "system.h"
#include "errno.h"
#include "error.h"
#include "progname.h"
#include "propername.h"
#include "xalloc.h"

/* The official name of this program (e.g., no `g' prefix).  */
#define PROGRAM_NAME "hello"

#define AUTHORS \
  proper_name ("The King"), \
  proper_name ("others")

static const struct option longopts[] = {
  {"greeting", required_argument, NULL, 'g'},
  {"traditional", no_argument, NULL, 't'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0}
};

/* Forward declarations.  */
static void usage (int status);

int
main (int argc, char *argv[])
{
  int optc;
  const char *greeting_msg;
  wchar_t *mb_greeting;
  size_t len;

  set_program_name (argv[0]);

  /* Set locale via LC_ALL.  */
  setlocale (LC_ALL, "");

#if ENABLE_NLS
  /* Set the text message domain.  */
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
#endif

  /* Having initialized gettext, get the default message. */
  greeting_msg = _("Hello, world!");

  /* Even exiting has subtleties.  On exit, if any writes failed, change
     the exit status.  The /dev/full device on GNU/Linux can be used for
     testing; for instance, hello >/dev/full should exit unsuccessfully.
     This is implemented in the Gnulib module "closeout".  */
  atexit (close_stdout);

  while ((optc = getopt_long (argc, argv, "g:t", longopts, NULL)) != -1)
    switch (optc)
      {
      case 'g':
	greeting_msg = optarg;
	break;
      case 't':
	greeting_msg = _("hello, world");
	break;
      /* --help and --version exit immediately, per GNU coding standards.  */
      case_GETOPT_HELP_CHAR;
      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
      default:
	usage (EXIT_FAILURE);
      }

  if (optind < argc)
    {
      /* Print error message and exit.  */
      error (0, 0, "%s: %s", _("extra operand"), argv[optind]);
      usage (EXIT_FAILURE);
    }

  len = mbsrtowcs(NULL, &greeting_msg, 0, NULL);
  if (len == (size_t)-1)
    error (EXIT_FAILURE, errno, _("conversion to a multibyte string failed"));
  mb_greeting = xmalloc((len + 1) * sizeof(wchar_t));
  mbsrtowcs(mb_greeting, &greeting_msg, len + 1, NULL);

  /* Print greeting message and exit. */
  wprintf (L"%ls\n", mb_greeting);
  free(mb_greeting);

  exit (EXIT_SUCCESS);
}


/* Print help info.  This long message is split into
   several pieces to help translators be able to align different
   blocks and identify the various pieces.  */

static void
usage (int status)
{
  if (status != EXIT_SUCCESS)
    emit_try_help ();
  else
    {
      /* TRANSLATORS: --help output 1 (synopsis)
         no-wrap */
      printf (_("\
Usage: %s OPTION...\n"), program_name);

      /* TRANSLATORS: --help output 2 (brief description)
         no-wrap */
      fputs (_("\
Print a friendly, customizable greeting.\n"), stdout);

      emit_mandatory_arg_note ();

      /* TRANSLATORS: --help output 3: options 2/2
         no-wrap */
      fputs (_("\
  -t, --traditional       use traditional greeting\n\
  -g, --greeting=TEXT     use TEXT as the greeting message\n"), stdout);

      fputs (HELP_OPTION_DESCRIPTION, stdout);
      fputs (VERSION_OPTION_DESCRIPTION, stdout);

      emit_ancillary_info ();
    }
  exit (status);
}
