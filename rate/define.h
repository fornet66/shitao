
#ifndef __IBS_DEFINE_H__
#define __IBS_DEFINE_H__
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <aibase/compiler.h>
#include <aibase/datetime.h>
#include <aiodbc/aiodbc.h>
#include <aiodbc/aiodbc_uac.h>
#include <aiuuid/aiuuid.h>
#include <ailogger/Log_Msg.h>
#include <ailogger/ailogger.h>
#define TMPFILESUFFIX ".tmp"
#define OUTFILESUFFIX ".out"
#define ERRFILESUFFIX ".err"
using namespace std;
using namespace aiodbc;
using namespace aiuuid;
using namespace ailogger;

namespace ibs {
	static void split(const string &source,vector<string> &desList,char sep=',') {
		string lsrc;
		desList.clear();
		if(source[source.size()-1]!=sep)
			lsrc=source+sep;
		else
			lsrc=source;
		int start=0,end=lsrc.size();
		while((end=lsrc.substr(start,lsrc.size()).find(sep))!=-1) {
			desList.push_back(lsrc.substr(start,end));
			start+=end+1;
		}
		return;
	}
}
#endif	// __IBS_DEFINE_H__

