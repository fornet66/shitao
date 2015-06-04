/**
 * commons.c -- holds different data types
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
#include <time.h>

#include "commons.h"
#include "error.h"
#include "settings.h"
#include "util.h"

/* processing time */
time_t end_proc;
time_t timestamp;
time_t start_proc;

/* calculate hits percentage */
float
get_percentage (unsigned long long total, unsigned long long hit)
{
  return ((float) (hit * 100) / (total));
}

void
display_version (void)
{
  fprintf (stdout, "shitao_alog - %s.\n", GO_VERSION);
  fprintf (stdout, "For more details contact xienan@asiainfo.com\n");
  fprintf (stdout, "Copyright (C) 2015-2016, by xienan\n");
}

