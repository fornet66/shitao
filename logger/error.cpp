/**
 * error.c -- error handling
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

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <execinfo.h>

#include "error.h"
#include "commons.h"

void
sigsegv_handler (int sig)
{
  FILE *fp = stderr;
  char **messages;
  size_t size, i;
  void *trace_stack[TRACE_SIZE];

  fprintf (fp, "\n=== shitao_alog %s crashed by signal %d ===\n", GO_VERSION, sig);

  size = backtrace (trace_stack, TRACE_SIZE);
  messages = backtrace_symbols (trace_stack, size);

  fprintf (fp, "\n-- STACK TRACE:\n\n");

  for (i = 0; i < size; i++)
    fprintf (fp, "\t%zu %s\n", i, messages[i]);

  fprintf (fp, "\nPlease report the crash opening an issue on GitHub:");
  fprintf (fp, "\nhttps://github.com/allinurl/goaccess/issues\n\n");
  exit (EXIT_FAILURE);
}

