/**
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

#ifndef SETTINGS_H_INCLUDED
#define SETTINGS_H_INCLUDED

#include <stdint.h>

#define MAX_LINE_CONF 512
#define MAX_EXTENSIONS 64
#define MAX_IGNORE_IPS 64

typedef enum
{
  COMBINED,
  VCOMBINED,
  COMMON,
  VCOMMON,
  W3C,
  CLOUDFRONT
} LOGTYPE;

/* predefined log dates */
typedef struct GPreConfDate_
{
  const char *apache;
  const char *w3c;
  const char *cloudfront;
} GPreConfDate;

/* predefined log formats */
typedef struct GPreConfLog_
{
  const char *combined;
  const char *vcombined;
  const char *common;
  const char *vcommon;
  const char *w3c;
  const char *cloudfront;
} GPreConfLog;

typedef struct GConfKeyword_
{
  const unsigned short key_id;
  const char *keyword;
} GConfKeyword;

typedef struct GConf_
{
  char *date_format;
  char *geoip_city_data;
  char *iconfigfile;
  char *ifile;
  char *ignore_ips[MAX_IGNORE_IPS];
  char *log_format;
  char *output_format;
  char *static_files[MAX_EXTENSIONS];
  int geo_db;
  int ignore_crawlers;
  int ignore_ip_idx;
  int output_txt;
  int real_os;
  int serve_usecs;
  int static_file_idx;
  size_t static_file_max_len;

} GConf;

extern GConf conf;

int parse_conf_file (int *argc, char ***argv);
void free_cmd_args (void);

#endif

