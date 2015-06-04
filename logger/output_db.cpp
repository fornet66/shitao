/**
 * output.c -- output to the standard output stream
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

#define _LARGEFILE_SOURCE
#define _LARGEFILE64_SOURCE
#define _FILE_OFFSET_BITS 64

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <inttypes.h>

#include "output_db.h"

#ifdef HAVE_LIBGEOIP
#include "geolocation.h"
#endif

#include "commons.h"
#include "error.h"
#include "gdns.h"
#include "settings.h"
#include "util.h"
#include "xmalloc.h"
#include "logger_handle.h"
#include <aiodbc/aiodbc_uac.h>

void
output_db (GLog * logger, const vector<Logger>& lgrs)
{
	Connection* db = aiodbc::start_connection("SHITAODB");
	if(db==NULL) throw SQLException("connect to SHITAODB error\n");
	LoggerHandle handle(db);
	handle.insert(lgrs);
}

