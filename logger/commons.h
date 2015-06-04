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

#ifndef COMMONS_H_INCLUDED
#define COMMONS_H_INCLUDED

#ifdef HAVE_LIBGEOIP
#include <GeoIP.h>
#endif

#include <time.h>

#include "error.h"

#define GO_VERSION 		"1.0.1"

#define INT_TO_PTR(i) ((void *) (long) (i))
#define PTR_TO_INT(p) ((int) (long) (p))

/* Processing time */
extern time_t end_proc;
extern time_t timestamp;
extern time_t start_proc;

#ifdef HAVE_LIBGEOIP
#define TOTAL_MODULES    12
#else
#define TOTAL_MODULES    11
#endif

#define DATE_TIME        20

#define REQ_PROTO_LEN     9
#define REQ_METHOD_LEN    8

typedef enum
{
  REQUEST,
  REQUEST_METHOD,
  REQUEST_PROTOCOL
} GReqMeta;

typedef enum MODULES
{
  VISITORS,
  REQUESTS,
  REQUESTS_STATIC,
  NOT_FOUND,
  HOSTS,
  OS,
  BROWSERS,
  REFERRERS,
  REFERRING_SITES,
  KEYPHRASES,
#ifdef HAVE_LIBGEOIP
  GEO_LOCATION,
#endif
  STATUS_CODES
} GModule;

typedef struct GSubItem_
{
  GModule module;
  char *data;
  int hits;
  unsigned long long bw;
  struct GSubItem_ *prev;
  struct GSubItem_ *next;
} GSubItem;

typedef struct GSubList_
{
  int size;
  struct GSubItem_ *head;
  struct GSubItem_ *tail;
} GSubList;

typedef struct GHolderItem_
{
  char *data;
  char *method;
  char *protocol;
  GSubList *sub_list;
  int hits;
  unsigned long long bw;
  unsigned long long usecs;
} GHolderItem;

typedef struct GHolder_
{
  GHolderItem *items;           /* data                             */
  GModule module;               /* current module                   */
  int idx;                      /* first level index                */
  int holder_size;              /* total num of items (first level) */
  int sub_items_size;           /* total number of sub items        */
} GHolder;



float get_percentage (unsigned long long total, unsigned long long hit);
void display_version (void);

#endif

