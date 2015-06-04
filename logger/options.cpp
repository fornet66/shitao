/**
 * options.c -- functions related to parsing program options
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
#include <string.h>
#include <getopt.h>
#include <errno.h>

#include "options.h"

#include "error.h"
#include "settings.h"
#include "util.h"

static char short_options[] = "f:e:o:"
#ifdef HAVE_LIBGEOIP
  "g"
#endif
  "ahV";

/* *INDENT-OFF* */
struct option long_opts[] = {
  {"exclude-ip"           , required_argument , 0 , 'e' } ,
  {"help"                 , no_argument       , 0 , 'h' } ,
  {"log-file"             , required_argument , 0 , 'f' } ,
  {"version"              , no_argument       , 0 , 'V' } ,
  {"date-format"          , required_argument , 0 ,  0  } ,
  {"ignore-crawlers"      , no_argument       , 0 ,  0  } ,
  {"log-format"           , required_argument , 0 ,  0  } ,
  {"output-format"        , required_argument , 0 , 'o' } ,
  {"real-os"              , no_argument       , 0 ,  0  } ,
  {"static-file"          , required_argument , 0 ,  0  } ,
#ifdef HAVE_LIBGEOIP
  {"std-geoip"            , no_argument       , 0 , 'g' } ,
  {"geoip-city-data"      , required_argument , 0 ,  0  } ,
#endif
  {0, 0, 0, 0}
};

void
cmd_help (void)
{
  printf ("\napp_logger - %s\n\n", GO_VERSION);
  printf (
  "Usage: "
  "app_logger [ options ... ] -f log_file [...]\n"
  "The following options can also be supplied to the command:\n\n"

  /* Log & Date Format Options */
  "Log & Date Format Options\n\n"
  "  --date-format=<dateformat>  - Specify log date format.\n"
  "  --log-format=<logformat>    - Specify log format. Inner quotes need to\n"
  "                                be escaped.\n\n"

  /* File Options */
  "File Options\n\n"
  "  -f --log-file=<filename>    - Path to input log file.\n"

  /* Parse Options */
  "Parse Options\n\n"
  "  -e --exclude-ip=<IP>        - Exclude one or multiple IPv4/6, includes\n"
  "                                IP ranges. i.e., 192.168.0.1-192.168.0.10\n"
  "  -o --output-format=txt|db   - Output either a TXT file or into database.\n"
  "  --ignore-crawlers           - Ignore crawlers.\n"
  "  --real-os                   - Display real OS names. e.g, Windows XP,\n"
  "                                Snow Leopard.\n"
  "  --static-file=<extension>   - Add static file extension. e.g.: .mp3\n"
  "                                Extensions are case sensitive.\n"

/* GeoIP Options */
#ifdef HAVE_LIBGEOIP
  "GeoIP Options\n\n"
  "  -g --std-geoip              - Standard GeoIP database for less memory\n"
  "                                usage.\n"
  "  --geoip-city-data=<path>    - Specify path to GeoIP City database file.\n"
  "                                i.e., GeoLiteCity.dat\n\n"
#endif

/* Other Options */
  "Other Options\n\n"
  "  -h --help                   - This help.\n"
  "  -V --version                - Display version information and exit.\n"

  "For more details contact xienan@asiainfo.com\n"
  "app_logger Copyright (C) 2015-2016, by xienan"
  "\n\n"
  );
  exit (EXIT_FAILURE);
}

void
read_option_args (int argc, char **argv)
{
  int o, idx = 0;

#ifdef HAVE_LIBGEOIP
  conf.geo_db = GEOIP_MEMORY_CACHE;
#endif

  while ((o = getopt_long (argc, argv, short_options, long_opts, &idx)) >= 0) {
    if (-1 == o || EOF == o)
      break;
    switch (o) {
     case 'f':
       conf.ifile = optarg;
       break;
#ifdef HAVE_LIBGEOIP
     case 'g':
       conf.geo_db = GEOIP_STANDARD;
       break;
#endif
     case 'e':
       conf.ignore_ips[conf.ignore_ip_idx++] = optarg;
       break;
     case 'o':
       conf.output_format = optarg;
       break;
     case 'h':
       cmd_help ();
       break;
     case 'V':
       display_version ();
       exit (EXIT_SUCCESS);
       break;
     case 0:
       if (!strcmp ("log-format", long_opts[idx].name))
         conf.log_format = unescape_str (optarg);
       if (!strcmp ("date-format", long_opts[idx].name))
         conf.date_format = unescape_str (optarg);
       if (!strcmp ("static-file", long_opts[idx].name)) {
         if (conf.static_file_max_len < strlen (optarg))
           conf.static_file_max_len = strlen (optarg);
         conf.static_files[conf.static_file_idx++] = optarg;
       }
       if (!strcmp ("ignore-crawlers", long_opts[idx].name))
         conf.ignore_crawlers = 1;
       if (!strcmp ("real-os", long_opts[idx].name))
         conf.real_os = 1;
       /* specifies the path of the GeoIP City database file */
       if (!strcmp ("geoip-city-data", long_opts[idx].name))
         conf.geoip_city_data = optarg;
       break;
     case '?':
       exit (EXIT_FAILURE);
     default:
       exit (EXIT_FAILURE);
    }
  }

  for (idx = optind; idx < argc; idx++)
    cmd_help ();
}

