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

#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#define UKEY_BUFFER 	  2048
#define LINE_BUFFER 	  4096
#define BW_HASHTABLES   3
#define KEY_FOUND       1
#define KEY_NOT_FOUND  -1
#define REF_SITE_LEN    512

#include "commons.h"

typedef struct GLogItem_
{
  char *agent;
  char *date;
  char *host;
  char *ref;
  char *method;
  char *protocol;
  char *req;
  char *status;
  unsigned long long resp_size;
  unsigned long long serve_time;
} GLogItem;

typedef struct GLog_
{
  unsigned int exclude_ip;
  unsigned int invalid;
  unsigned int offset;
  unsigned int process;
  unsigned long long resp_size;
  unsigned short piping;
} GLog;

GLog *init_log (void);
GLogItem *init_log_item (void);
int parse_log (GLog ** logger, char *tail, int n);

#endif

