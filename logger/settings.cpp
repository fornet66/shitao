/**
 * settings.c -- goaccess configuration
 * Copyright (C) 2009-2014 by Gerardo Orellana <goaccess@prosoftcorp.com>
 * GoAccess - An Ncurses apache weblog analyzer & interactive viewer
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * A copy of the GNU General Public License is attached to this
 * source distribution for its full text.
 *
 * Visit http://goaccess.prosoftcorp.com for new releases.
 */

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "settings.h"

#include "commons.h"
#include "error.h"
#include "util.h"
#include "xmalloc.h"

static char **nargv;
static int nargc = 0;

/* Ignore the following options */
static const char *ignore_cmd_opts[] = {
  "help",
  "storage",
};

static int
in_ignore_cmd_opts (const char *val)
{
  size_t i;
  for (i = 0; i < ARRAY_SIZE (ignore_cmd_opts); i++) {
    if (strstr (val, ignore_cmd_opts[i]) != NULL)
      return 1;
  }
  return 0;
}

static char *
get_config_file_path (void)
{
  char *path = NULL;

  /* determine which config file to open, default or custom */
  if (conf.iconfigfile != NULL) {
    path = realpath (conf.iconfigfile, NULL);
    if (path == NULL)
      FATAL ("%s", strerror (errno));
  }
  return path;
}

/* clean command line arguments */
void
free_cmd_args (void)
{
  int i;
  if (nargc == 0)
    return;
  for (i = 0; i < nargc; i++)
    free (nargv[i]);
  free (nargv);
}

/* append extra value to argv */
static void
append_to_argv (int *argc, char ***argv, char *val)
{
  char **_argv = (char**)xrealloc (*argv, (*argc + 2) * sizeof (*_argv));
  _argv[*argc] = val;
  _argv[*argc + 1] = 0;
  (*argc)++;
  *argv = _argv;
}

/* parses configuration file to feed getopt_long */
int
parse_conf_file (int *argc, char ***argv)
{
  char line[MAX_LINE_CONF + 1];
  char *path = NULL, *val, *opt, *p;
  FILE *file;
  int i;
  size_t idx;

  /* assumes program name is on argv[0], though, it is not guaranteed */
  append_to_argv (&nargc, &nargv, xstrdup ((char *) *argv[0]));

  /* determine which config file to open, default or custom */
  path = get_config_file_path ();
  if (path == NULL)
    return ENOENT;

  /* could not open conf file, if so prompt conf dialog */
  if ((file = fopen (path, "r")) == NULL) {
    free (path);
    return ENOENT;
  }

  while (fgets (line, sizeof line, file) != NULL) {
    if (line[0] == '\n' || line[0] == '\r' || line[0] == '#')
      continue;

    /* key */
    idx = strcspn (line, " \t");
    if (strlen (line) == idx)
      FATAL ("Malformed config key at line: %s", line);

    /* make old config options backwards compatible by
     * substituting underscores with dashes */
    while ((p = strpbrk (line, "_")) != NULL)
      *p = '-';

    line[idx] = '\0';

    /* Ignore the following options when reading the config file */
    if (in_ignore_cmd_opts (line))
      continue;

    /* value */
    val = line + (idx + 1);
    idx = strspn (val, " \t");
    if (strlen (line) == idx)
      FATAL ("Malformed config value at line: %s", line);
    val = val + idx;
    val = trim_str (val);

    if (strcmp ("false", val) == 0)
      continue;

    /* set it as command line options */
    opt = (char*)xmalloc (snprintf (NULL, 0, "--%s", line) + 1);
    sprintf (opt, "--%s", line);

    append_to_argv (&nargc, &nargv, opt);
    if (strcmp ("true", val) != 0)
      append_to_argv (&nargc, &nargv, xstrdup (val));
  }

  /* give priority to command line arguments */
  for (i = 1; i < *argc; i++)
    append_to_argv (&nargc, &nargv, xstrdup ((char *) (*argv)[i]));

  *argc = nargc;
  *argv = (char **) nargv;

  fclose (file);

  free (path);
  return 0;
}

