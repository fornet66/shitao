/**
 * goaccess.c -- main log analyzer
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
#define _FILE_OFFSET_BITS    64

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <locale.h>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#ifdef HAVE_LIBGEOIP
#include "geolocation.h"
#endif

#include "commons.h"
#include "error.h"
#include "gdns.h"
#include "options.h"
#include "output_txt.h"
#include "output_db.h"
#include "parser.h"
#include "settings.h"
#include "util.h"
#include "logger.h"

vector<Logger> g_lgrs;
GConf conf = { 0 };

int active_gdns = 0;
static GLog *logger;

static void
house_keeping (void)
{
  /* REVERSE DNS THREAD */
  pthread_mutex_lock (&gdns_thread.mutex);
  /* kill dns pthread */
  active_gdns = 0;
  gdns_free_queue ();
  pthread_mutex_unlock (&gdns_thread.mutex);

  /* GEOLOCATION */
#ifdef HAVE_LIBGEOIP
  if (geo_location_data != NULL)
    GeoIP_delete (geo_location_data);
#endif

  /* LOGGER */
  free (logger);

  free_cmd_args ();
}

#ifdef HAVE_LIBGEOIP
static void
init_geoip (void)
{
  /* open custom GeoIP database */
  if (conf.geoip_city_data != NULL)
    geo_location_data = geoip_open_db (conf.geoip_city_data);
  /* fall back to legacy GeoIP database */
  else
    geo_location_data = GeoIP_new (conf.geo_db);
}
#endif

static void
set_locale (void)
{
  char *loc_ctype;

  loc_ctype = getenv ("LC_CTYPE");
  if (loc_ctype != NULL)
    setlocale (LC_CTYPE, loc_ctype);
  else if ((loc_ctype = getenv ("LC_ALL")))
    setlocale (LC_CTYPE, loc_ctype);
  else
    setlocale (LC_CTYPE, "");
}

static void
parse_cmd_line (int argc, char **argv)
{
  read_option_args (argc, argv);

  if (!isatty (STDOUT_FILENO) || conf.output_format != NULL)
    conf.output_txt = 1;
  if (conf.ifile != NULL && !isatty (STDIN_FILENO) && !conf.output_txt)
    cmd_help ();
  if (conf.ifile == NULL && isatty (STDIN_FILENO) && conf.output_format == NULL)
    cmd_help ();
}

static void
setup_signal_handlers (void)
{
  struct sigaction act;
  sigemptyset (&act.sa_mask);
  act.sa_flags = 0;
  act.sa_handler = sigsegv_handler;

  sigaction (SIGSEGV, &act, NULL);
}

int
main (int argc, char **argv)
{
  setup_signal_handlers ();

  char configfile[14] = "./shitao.conf";
  conf.iconfigfile = configfile;
  /* command line/config options */
  parse_conf_file (&argc, &argv);
  parse_cmd_line (argc, argv);

  /* setup to use the current locale */
  set_locale ();

#ifdef HAVE_LIBGEOIP
  init_geoip ();
#endif

  /* init logger */
  logger = init_log ();

  /* main processing event */
  time (&start_proc);
  if (parse_log (&logger, NULL, -1))
    FATAL ("Error while processing file");

  logger->offset = logger->process;

  /* no valid entries to process from the log */
  if ((logger->process == 0) || (logger->process == logger->invalid))
    FATAL ("Nothing valid to process.");

  /* init reverse lookup thread */
  gdns_init ();

  time (&end_proc);

  try {
	  /* stdout */
	  if (conf.output_txt) {
		if (conf.output_format && strcmp ("db", conf.output_format) == 0)
		  output_db (logger, g_lgrs);
		else
		  output_txt (logger, g_lgrs);
	  }
	  /*
	  else {
		  output_screen(logger);
	  }
	   */
  }
  catch(std::exception& e) {
	  printf("error occured: %s\n", e.what());
  }
  /* clean */
  house_keeping ();

  return EXIT_SUCCESS;
}

