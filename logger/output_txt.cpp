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

#include "output_txt.h"

#ifdef HAVE_LIBGEOIP
#include "geolocation.h"
#endif

#include "commons.h"
#include "error.h"
#include "gdns.h"
#include "settings.h"
#include "util.h"
#include "xmalloc.h"
#include "logger.h"

void
output_txt (GLog * logger, const vector<Logger>& lgrs)
{
	FILE* fp = stdout;
	struct tm* now_tm;
	timestamp = time (NULL);
	now_tm = localtime (&timestamp);
	char now[DATE_TIME];
	strftime (now, DATE_TIME, "%Y-%m-%d %H:%M:%S", now_tm);
	fprintf (fp, "Server Statistics - %s\n", now);
	fprintf (fp, "exclude_ip: %u\n", logger->exclude_ip);
	fprintf (fp, "invalid: %u\n", logger->invalid);
	fprintf (fp, "process: %u\n", logger->process);
	fprintf (fp, "resp_size:%s\n", filesize_str(logger->resp_size));
	for(size_t i=0; i<lgrs.size(); ++i) {
#ifdef HAVE_LIBGEOIP
		char country[COUNTRY_LEN] = "";
		char city[CITY_LEN] = "";
#endif
#ifdef HAVE_LIBGEOIP
		geoip_get_country (lgrs[i].host.c_str(), country);
		fprintf (fp, "{country %s}", country);
		if (conf.geoip_city_data) {
			geoip_get_city (lgrs[i].host.c_str(), city);
			fprintf (fp, "{city %s}", city);
		}
#endif
		fprintf (fp, "{agent %s}{log_date %s}{zone %s}{host %s}{method %s}{protocol %s}{status %s}{bandwidth %llu}{timer %llu}{php %s}{pf %s}{source %s}{uid %s}{appversion %s}{did %s}{cookie %s}{fromId %d}{count %d}{type %d}{program_id %d}{item_id %d}{still_id %d}{follow_id %d}{vote_id %d}{lottery_id %d}{sales_id %d}{topic_id %d}{comment_id %d}{prize_id %d}{action %s}{guess_price %.f}{expires_in %d}{phone %s}{email %s}{address %s}\n",
				lgrs[i].agent.c_str(),
				lgrs[i].date.c_str(),
				lgrs[i].zone.c_str(),
				lgrs[i].host.c_str(),
				lgrs[i].method.c_str(),
				lgrs[i].protocol.c_str(),
				lgrs[i].status.c_str(),
				lgrs[i].bytes,
				lgrs[i].timer,
				lgrs[i].php.c_str(),
				lgrs[i].pf.c_str(),
				lgrs[i].source.c_str(),
				lgrs[i].uid.c_str(),
				lgrs[i].appversion.c_str(),
				lgrs[i].did.c_str(),
				lgrs[i].cookie.c_str(),
				lgrs[i].fromId,
				lgrs[i].count,
				lgrs[i].type,
				lgrs[i].program_id,
				lgrs[i].item_id,
				lgrs[i].still_id,
				lgrs[i].follow_id,
				lgrs[i].vote_id,
				lgrs[i].lottery_id,
				lgrs[i].store_id,
				lgrs[i].topic_id,
				lgrs[i].comment_id,
				lgrs[i].prize_id,
				lgrs[i].comment_action.c_str(),
				lgrs[i].guess_price,
				lgrs[i].expires_in,
				lgrs[i].phone.c_str(),
				lgrs[i].email.c_str(),
				lgrs[i].address.c_str());
	}
}

