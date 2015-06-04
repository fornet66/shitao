/**
 * parser.c -- web log parsing
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

/*
 * "_XOPEN_SOURCE" is required for the GNU libc to export "strptime(3)"
 * correctly.
 */
#define _LARGEFILE_SOURCE
#define _LARGEFILE64_SOURCE
#define _FILE_OFFSET_BITS 64

#define _XOPEN_SOURCE 700

#include <ctype.h>
#include <errno.h>

#include <arpa/inet.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#ifdef HAVE_LIBGEOIP
#include "geolocation.h"
#endif

#include "parser.h"

#include "browsers.h"
#include "app_logger.h"
#include "error.h"
#include "opesys.h"
#include "settings.h"
#include "util.h"
#include "xmalloc.h"
#include "logger.h"

extern vector<Logger> g_lgrs;
GLog *
init_log (void)
{
  GLog *glog = (GLog*)xmalloc (sizeof (GLog));
  memset (glog, 0, sizeof *glog);

  return glog;
}

GLogItem *
init_log_item (void)
{
  GLogItem *glog = (GLogItem*)xmalloc (sizeof (GLogItem));
  memset (glog, 0, sizeof *glog);

  glog->agent = NULL;
  glog->date = NULL;
  glog->host = NULL;
  glog->ref = NULL;
  glog->method = NULL;
  glog->protocol = NULL;
  glog->req = NULL;
  glog->status = NULL;
  glog->resp_size = 0LL;
  glog->serve_time = 0;

  return glog;
}

static void
free_logger (GLogItem * glog)
{
  if (glog->agent != NULL)
    free (glog->agent);
  if (glog->date != NULL)
    free (glog->date);
  if (glog->host != NULL)
    free (glog->host);
  if (glog->ref != NULL)
    free (glog->ref);
  if (glog->method != NULL)
    free (glog->method);
  if (glog->protocol != NULL)
    free (glog->protocol);
  if (glog->req != NULL)
    free (glog->req);
  if (glog->status != NULL)
    free (glog->status);
  free (glog);
}

#define BASE16_TO_10(x) (((x) >= '0' && (x) <= '9') ? ((x) - '0') : \
    (toupper((x)) - 'A' + 10))

static char *
decode_url (char *url)
{
  char *out, *ptr;
  const char *c;

  if ((url == NULL) || (*url == '\0'))
    return NULL;

  out = ptr = xstrdup (url);
  for (c = url; *c; c++) {
    if (*c != '%' || !isxdigit (c[1]) || !isxdigit (c[2]))
      *ptr++ = *c;
    else {
      *ptr++ = (BASE16_TO_10 (c[1]) * 16) + (BASE16_TO_10 (c[2]));
      c += 2;
    }
  }
  *ptr = 0;
  strip_newlines (out);

  return trim_str (out);
}

/* returns 1 if the request seems to be a static file */
static int
verify_static_content (char *req)
{
  char *nul = req + strlen (req), *ext = NULL;
  int elen = 0, i;

  if (strlen (req) < conf.static_file_max_len)
    return 0;

  for (i = 0; i < conf.static_file_idx; ++i) {
    ext = conf.static_files[i];
    if (ext == NULL || *ext == '\0')
      continue;

    elen = strlen (ext);
    if (!memcmp (nul - elen, ext, elen))
      return 1;
  }
  return 0;
}

static char *
parse_req (char *line, GLogItem * glog)
{
  const char *lookfor = NULL;
  char *req, *request, *req_l = NULL, *req_r = NULL;
  ptrdiff_t req_len;

  if ((lookfor = "OPTIONS ", req_l = strstr (line, lookfor)) != NULL ||
      (lookfor = "GET ", req_l = strstr (line, lookfor)) != NULL ||
      (lookfor = "HEAD ", req_l = strstr (line, lookfor)) != NULL ||
      (lookfor = "POST ", req_l = strstr (line, lookfor)) != NULL ||
      (lookfor = "PUT ", req_l = strstr (line, lookfor)) != NULL ||
      (lookfor = "DELETE ", req_l = strstr (line, lookfor)) != NULL ||
      (lookfor = "TRACE ", req_l = strstr (line, lookfor)) != NULL ||
      (lookfor = "CONNECT ", req_l = strstr (line, lookfor)) != NULL ||
      (lookfor = "PATCH", req_l = strstr (line, lookfor)) != NULL ||
      (lookfor = "options ", req_l = strstr (line, lookfor)) != NULL ||
      (lookfor = "get ", req_l = strstr (line, lookfor)) != NULL ||
      (lookfor = "head ", req_l = strstr (line, lookfor)) != NULL ||
      (lookfor = "post ", req_l = strstr (line, lookfor)) != NULL ||
      (lookfor = "put ", req_l = strstr (line, lookfor)) != NULL ||
      (lookfor = "delete ", req_l = strstr (line, lookfor)) != NULL ||
      (lookfor = "trace ", req_l = strstr (line, lookfor)) != NULL ||
      (lookfor = "connect ", req_l = strstr (line, lookfor)) != NULL ||
      (lookfor = "patch", req_l = strstr (line, lookfor)) != NULL) {
    /* didn't find it - weird  */
    if ((req_r = strstr (line, " HTTP/1.0")) == NULL &&
        (req_r = strstr (line, " HTTP/1.1")) == NULL)
      return alloc_string ("-");

    req_l += strlen (lookfor);
    req_len = req_r - req_l;

    /* make sure we don't have some weird requests */
    if (req_len <= 0)
      return alloc_string ("-");

    req = (char*)xmalloc (req_len + 1);
    strncpy (req, req_l, req_len);
    req[req_len] = 0;

  } else
    req = alloc_string (line);

  request = decode_url (req);
  if (request != NULL && *request != '\0') {
    free (req);
    return request;
  }

  return req;
}

static int
invalid_method (const char *token)
{
  const char *lookfor = NULL;

  return !((lookfor = "OPTIONS", !memcmp (token, lookfor, 7)) ||
           (lookfor = "GET", !memcmp (token, lookfor, 3)) ||
           (lookfor = "HEAD", !memcmp (token, lookfor, 4)) ||
           (lookfor = "POST", !memcmp (token, lookfor, 4)) ||
           (lookfor = "PUT", !memcmp (token, lookfor, 3)) ||
           (lookfor = "DELETE", !memcmp (token, lookfor, 6)) ||
           (lookfor = "TRACE", !memcmp (token, lookfor, 5)) ||
           (lookfor = "CONNECT", !memcmp (token, lookfor, 7)) ||
           (lookfor = "PATCH", !memcmp (token, lookfor, 5)) ||
           (lookfor = "options", !memcmp (token, lookfor, 7)) ||
           (lookfor = "get", !memcmp (token, lookfor, 3)) ||
           (lookfor = "head", !memcmp (token, lookfor, 4)) ||
           (lookfor = "post", !memcmp (token, lookfor, 4)) ||
           (lookfor = "put", !memcmp (token, lookfor, 3)) ||
           (lookfor = "delete", !memcmp (token, lookfor, 6)) ||
           (lookfor = "trace", !memcmp (token, lookfor, 5)) ||
           (lookfor = "connect", !memcmp (token, lookfor, 7)) ||
           (lookfor = "patch", !memcmp (token, lookfor, 5)));
}

static int
invalid_protocol (const char *token)
{
  const char *lookfor = NULL;

  return !((lookfor = "HTTP/1.0", !memcmp (token, lookfor, 8)) ||
           (lookfor = "HTTP/1.1", !memcmp (token, lookfor, 8)));
}

static char *
parse_string (char **str, char end, int cnt)
{
  int idx = 0;
  char *pch = *str, *p;
  do {
    if (*pch == end)
      idx++;
    if ((*pch == end && cnt == idx) || *pch == '\0') {
      size_t len = (pch - *str + 1);
      p = (char*)xmalloc (len);
      memcpy (p, *str, (len - 1));
      p[len - 1] = '\0';
      *str += len - 1;
      return trim_str (p);
    }
    /* advance to the first unescaped delim */
    if (*pch == '\\')
      pch++;
  } while (*pch++);
  return NULL;
}

static int
parse_format (GLogItem * glog, const char *fmt, const char *date_format,
              char *str)
{
  const char *p;
  double serve_secs;
  int special = 0;
  struct tm tm;
  unsigned long long bandw, serve_time;

  if (str == NULL || *str == '\0')
    return 1;

  memset (&tm, 0, sizeof (tm));

  /* iterate over the log format */
  for (p = fmt; *p; p++) {
    if (*p == '%') {
      special++;
      continue;
    }
    if (special && *p != '\0') {
      char *pch, *sEnd, *bEnd, *tkn = NULL, *end = NULL;
      errno = 0;
      bandw = 0;
      serve_time = 0;
      serve_secs = 0;

      switch (*p) {
         /* date */
       case 'd':
         if (glog->date)
           return 1;
         /* parse date format including dates containing spaces,
          * i.e., syslog date format (Jul 15 20:10:56) */
         tkn = parse_string (&str, p[1], count_matches (date_format, ' ') + (p[1]==' '?1:0));
         if (tkn == NULL)
           return 1;
         end = strptime (tkn, date_format, &tm);
         if (end == NULL || *end != '\0') {
           free (tkn);
           return 1;
         }
         glog->date = tkn;
         break;
         /* remote hostname (IP only) */
       case 'h':
         if (glog->host)
           return 1;
         tkn = parse_string (&str, p[1], 1);
         if (tkn == NULL)
           return 1;
         if (invalid_ipaddr (tkn)) {
           free (tkn);
           return 1;
         }
         glog->host = tkn;
         break;
         /* request method */
       case 'm':
         if (glog->method)
           return 1;
         tkn = parse_string (&str, p[1], 1);
         if (tkn == NULL)
           return 1;
         if (invalid_method (tkn)) {
           free (tkn);
           return 1;
         }
         glog->method = tkn;
         break;
         /* request not including method or protocol */
       case 'U':
         if (glog->req)
           return 1;
         tkn = parse_string (&str, p[1], 1);
         if (tkn == NULL || *tkn == '\0')
           return 1;
         if ((glog->req = decode_url (tkn)) == NULL)
           return 1;
         free (tkn);
         break;
         /* request protocol */
       case 'H':
         if (glog->protocol)
           return 1;
         tkn = parse_string (&str, p[1], 1);
         if (tkn == NULL)
           return 1;
         if (invalid_protocol (tkn)) {
           free (tkn);
           return 1;
         }
         glog->protocol = tkn;
         break;
         /* request, including method + protocol */
       case 'r':
         if (glog->req)
           return 1;
         tkn = parse_string (&str, p[1], 1);
         if (tkn == NULL)
           return 1;
         glog->req = parse_req (tkn, glog);
         free (tkn);
         break;
         /* Status Code */
       case 's':
         if (glog->status)
           return 1;
         tkn = parse_string (&str, p[1], 1);
         if (tkn == NULL)
           return 1;
         strtol (tkn, &sEnd, 10);
         if (tkn == sEnd || *sEnd != '\0' || errno == ERANGE) {
           free (tkn);
           return 1;
         }
         glog->status = tkn;
         break;
         /* size of response in bytes - excluding HTTP headers */
       case 'b':
         if (glog->resp_size)
           return 1;
         tkn = parse_string (&str, p[1], 1);
         if (tkn == NULL)
           return 1;
         bandw = strtol (tkn, &bEnd, 10);
         if (tkn == bEnd || *bEnd != '\0' || errno == ERANGE)
           bandw = 0;
         glog->resp_size = bandw;
         free (tkn);
         break;
         /* referrer */
       case 'R':
         if (glog->ref)
           return 1;
         tkn = parse_string (&str, p[1], 1);
         if (tkn == NULL)
           tkn = alloc_string ("-");
         if (tkn != NULL && *tkn == '\0') {
           free (tkn);
           tkn = alloc_string ("-");
         }
         glog->ref = tkn;
         break;
         /* user agent */
       case 'u':
         if (glog->agent)
           return 1;
         tkn = parse_string (&str, p[1], 1);
         if (tkn != NULL && *tkn != '\0') {
           /* Make sure the user agent is decoded (i.e.: CloudFront)
            * and replace all '+' with ' ' (i.e.: w3c) */
           glog->agent = char_replace (decode_url (tkn), '+', ' ');
           free (tkn);
           break;
         } else if (tkn != NULL && *tkn == '\0') {
           free (tkn);
           tkn = alloc_string ("-");
         }
         /* must be null */
         else {
           tkn = alloc_string ("-");
         }
         glog->agent = tkn;
         break;
         /* time taken to serve the request, in seconds */
       case 'T':
         if (glog->serve_time)
           return 1;
         /* ignore seconds if we have microseconds */
         if (strstr (fmt, "%D") != NULL)
           break;

         tkn = parse_string (&str, p[1], 1);
         if (tkn == NULL)
           return 1;
         if (strchr (tkn, '.') != NULL)
           serve_secs = strtod (tkn, &bEnd);
         else
           serve_secs = strtoull (tkn, &bEnd, 10);

         if (tkn == bEnd || *bEnd != '\0' || errno == ERANGE)
           serve_secs = 0;
         /* convert it to microseconds */
         if (serve_secs > 0)
           glog->serve_time = static_cast<unsigned long long>(serve_secs * SECS);
         else
           glog->serve_time = 0;
         conf.serve_usecs = 1;
         free (tkn);
         break;
         /* time taken to serve the request, in microseconds */
       case 'D':
         if (glog->serve_time)
           return 1;
         tkn = parse_string (&str, p[1], 1);
         if (tkn == NULL)
           return 1;
         serve_time = strtoull (tkn, &bEnd, 10);
         if (tkn == bEnd || *bEnd != '\0' || errno == ERANGE)
           serve_time = 0;
         glog->serve_time = serve_time;
         conf.serve_usecs = 1;
         free (tkn);
         break;
         /* everything else skip it */
       default:
         if ((pch = strchr (str, p[1])) != NULL)
           str += pch - str;
      }
      if ((str == NULL) || (*str == '\0'))
        return 0;
      special = 0;
    } else if (special && isspace (p[0])) {
      return 1;
    } else
      str++;
  }
  return 0;
}

static int
valid_line (char *line)
{
  /* invalid line */
  if ((line == NULL) || (*line == '\0'))
    return 1;
  /* ignore comments */
  if (*line == '#' || *line == '\n')
    return 1;

  return 0;
}

static void
count_invalid (GLog * logger)
{
  logger->invalid++;
}

static void
count_process (GLog * logger)
{
  logger->process++;
}

static int
process_date (GLogItem * glog, char *date, char* zone)
{
  /* make compiler happy */
  memset (date, 0, sizeof *date);
  memset (zone, 0, sizeof *zone);
  convert_date (date, zone, glog->date, conf.date_format, "%Y%m%d%H%M%S", DATE_LEN);
  if (date == NULL)
    return 1;
  return 0;
}

static int
exclude_ip (GLog * logger, GLogItem * glog)
{
  if (conf.ignore_ip_idx && ip_in_range (glog->host)) {
    logger->exclude_ip++;
    return 0;
  }
  return 1;
}

static int
exclude_crawler (GLogItem * glog)
{
  return conf.ignore_crawlers && is_crawler (glog->agent) ? 0 : 1;
}

/* process a line from the log and store it accordingly */
static int
process_log (GLog * logger, char *line, int test)
{
  GLogItem *glog;
  char date[DATE_LEN];
  char zone[ZONE_LEN];

  if (valid_line (line)) {
    count_invalid (logger);
    return 0;
  }

  count_process (logger);
  glog = init_log_item ();
  /* parse a line of log, and fill structure with appropriate values */
  if (parse_format (glog, conf.log_format, conf.date_format, line)) {
    count_invalid (logger);
	free_logger (glog);
	return 0;
  }

  /* must have the following fields */
  if (glog->host == NULL || glog->date == NULL || glog->req == NULL) {
    count_invalid (logger);
	free_logger (glog);
	return 0;
  }
  /* agent will be null in cases where %u is not specified */
  if (glog->agent == NULL)
    glog->agent = alloc_string ("-");

  /* testing log only */
  if (test) {
	free_logger (glog);
	return 0;
  }

  if (process_date (glog, date, zone)) {
    count_invalid (logger);
	free_logger (glog);
	return 0;
  }
  /* ignore host or crawlers */
  if (exclude_ip (logger, glog) == 0) {
	free_logger (glog);
	return 0;
  }
  if (exclude_crawler (glog) == 0) {
	free_logger (glog);
	return 0;
  }
  if (verify_static_content (glog->req)) {
    free_logger (glog);		// TODO
	return 0;
  }

  logger->resp_size += glog->resp_size;

  // output
  Logger lgr;
  lgr.date = string(date);
  lgr.zone = string(zone);
  lgr.host = glog->host!=NULL?glog->host:string("");
  lgr.method = glog->method!=NULL?glog->method:string("");
  lgr.protocol = glog->protocol!=NULL?glog->protocol:string("");
  lgr.status = glog->status!=NULL?glog->status:string("");
  lgr.bytes = glog->resp_size;
  lgr.timer = glog->serve_time;
  lgr.ref = glog->ref!=NULL?glog->ref:string("");
  lgr.agent = glog->agent!=NULL?glog->agent:string("");
  lgr.req = glog->req!=NULL?glog->req:string("");
  lgr.parse();
  g_lgrs.push_back(lgr);

  free_logger (glog);
  return 0;
}

/* entry point to parse the log line by line */
int
parse_log (GLog ** logger, char *tail, int n)
{
  FILE *fp = NULL;
  char line[LINE_BUFFER];
  int i = 0, test = -1 == n ? 0 : 1;

  if (conf.date_format == NULL || *conf.date_format == '\0')
    FATAL ("No date format was found on your conf file.");

  if (conf.log_format == NULL || *conf.log_format == '\0')
    FATAL ("No log format was found on your conf file.");

  if (tail != NULL) {
    if (process_log ((*logger), tail, test))
      return 1;
    return 0;
  }

  if (conf.ifile == NULL) {
    fp = stdin;
    (*logger)->piping = 1;
  }

  g_lgrs.reserve(100000);

  /* make sure we can open the log (if not reading from stdin) */
  if (!(*logger)->piping && (fp = fopen (conf.ifile, "r")) == NULL)
    FATAL ("%s", strerror (errno));

  while (fgets (line, LINE_BUFFER, fp) != NULL) {
    if (n >= 0 && i++ == n)
      break;
    if (process_log ((*logger), line, test)) {
      if (!(*logger)->piping)
        fclose (fp);
      return 1;
    }
  }
  /* definitely not portable! */
  if ((*logger)->piping)
    freopen ("/dev/tty", "r", stdin);

  if (!(*logger)->piping)
    fclose (fp);
  return 0;
}

