
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <aibase/datetime.h>
#include <aiodbc/aiodbc_uac.h>
#include "logger_handle.h"
#include "stat_app_user.h"
#include "stat_collect_total.h"
#include "stat_collect_detail.h"
#include "stat_comment_total.h"
#include "stat_comment_detail.h"
#include "stat_login.h"
#include "stat_main_visit.h"
#include "stat_visit_detail.h"
#include "stat_scan_user.h"
#include "stat_scan_program_user.h"
#include "stat_vote_total.h"
using namespace std;
using namespace aibase;
using namespace aiodbc;

enum Stat_Type {By_Minute=0, By_Hour=1, By_Day=2, By_Minute_Now=10, By_Hour_Now=11};

void do_stat(Connection *db, vector<Logger> &lgrs, string &dd, string &hh, string &mm, int type) {
	Stat_App_User app_user(db);
	app_user.tableName("stat_app_user");
	app_user.stat(lgrs,dd,hh,mm,type);
	db->commit();

	Stat_Collect_Total collect_total(db);
	collect_total.tableName("stat_collect_total");
	collect_total.stat(lgrs,dd,hh,mm,type);
	db->commit();

	Stat_Collect_Detail collect_detail(db);
	collect_detail.tableName("stat_collect_detail");
	collect_detail.stat(lgrs,dd,hh,mm,type);
	db->commit();

	Stat_Comment_Total comment_total(db);
	comment_total.tableName("stat_comment_total");
	comment_total.stat(lgrs,dd,hh,mm,type);
	db->commit();

	Stat_Comment_Detail comment_detail(db);
	comment_detail.tableName("stat_comment_detail");
	comment_detail.stat(lgrs,dd,hh,mm,type);
	db->commit();

	Stat_Login login(db);
	login.tableName("stat_login");
	login.stat(lgrs,dd,hh,mm,type);
	db->commit();

	Stat_Main_Visit main_visit(db);
	main_visit.tableName("stat_main_visit");
	main_visit.stat(lgrs,dd,hh,mm,type);
	db->commit();

	Stat_Visit_Detail visit_detail(db);
	visit_detail.tableName("stat_visit_detail");
	visit_detail.stat(lgrs,dd,hh,mm,type);
	db->commit();

	Stat_Scan_User scan_user(db);
	scan_user.tableName("stat_scan_user");
	scan_user.stat(lgrs,dd,hh,mm,type);
	db->commit();

	Stat_Scan_Program_User scan_program_user(db);
	scan_program_user.tableName("stat_scan_program_user");
	scan_program_user.stat(lgrs,dd,hh,mm,type);
	db->commit();

	Stat_Vote_Total vote_total(db);
	vote_total.tableName("stat_vote_total");
	vote_total.stat(lgrs,dd,hh,mm,type);
	db->commit();
}

int main (int argc, char** argv) {
	if(argc < 2) {
		printf("app_stat - 1.0.1\n");
		printf("\n");
		printf("Usage: app_stat options\n");
		printf("    app_stat yyyymmdd\n");
		printf("\n");
		return 0;
	}
	OSSDateTime time;
	OSSDateTime terminate;
	OSSDateTime start;
	OSSDateTime end;
	time=OSSDate(string(argv[1]));
	terminate=time.addDays(1);
	if(time>OSSDate("20160101")||time<OSSDate("20150101")) {
		printf("yyyymmdd format error\n\n");
		return 0;
	}
	Connection *db=start_connection("SHITAODB");
	if(db==NULL) {
		printf("can not connect to database\n");
		return 0;
	}
	vector<Logger> lgrs;
	LoggerHandle handle(db);
	// by minute
	for (start=time,end=start.addSecs(60);end<=terminate;start=start.addSecs(60),end=end.addSecs(60)) {
		int type = By_Minute;
		string dd = start.toString("%Y%m%d");
		string hh = start.toString("%H");
		string mm = start.toString("%M");
		string a = start.toString("%Y%m%d%H%M%S");
		string b = end.toString("%Y%m%d%H%M%S");
		handle.select(a, b, lgrs);
		do_stat(db, lgrs, dd, hh, mm, type);
		printf("do logger time scope [%s----%s] ...... ok\n", a.c_str(), b.c_str());
	}
	// by hour
	for (start=time,end=start.addSecs(3600);end<=terminate;start=start.addSecs(3600),end=end.addSecs(3600)) {
		int type = By_Hour;
		string dd = start.toString("%Y%m%d");
		string hh = start.toString("%H");
		string mm = "60";
		string a = start.toString("%Y%m%d%H%M%S");
		string b = end.toString("%Y%m%d%H%M%S");
		handle.select(a, b, lgrs);
		do_stat(db, lgrs, dd, hh, mm, type);
		printf("do logger time scope [%s----%s] ...... ok\n", a.c_str(), b.c_str());
	}
	// by day
	for (start=time,end=start.addDays(1);end<=terminate;start=start.addDays(1),end=end.addDays(1)) {
		int type = By_Day;
		string dd = start.toString("%Y%m%d");
		string hh = "24";
		string mm = "60";
		string a = start.toString("%Y%m%d%H%M%S");
		string b = end.toString("%Y%m%d%H%M%S");
		handle.select(a, b, lgrs);
		do_stat(db, lgrs, dd, hh, mm, type);
		printf("do logger time scope [%s----%s] ...... ok\n", a.c_str(), b.c_str());
	}
	// by minute now
	for (start=time,end=start.addSecs(60);end<=terminate;end=end.addSecs(60)) {
		int type = By_Minute_Now;
		string dd = start.toString("%Y%m%d");
		string hh = end.addSecs(-60).toString("%H");
		string mm = end.addSecs(-60).toString("%M");
		string a = start.toString("%Y%m%d%H%M%S");
		string b = end.toString("%Y%m%d%H%M%S");
		handle.select(a, b, lgrs);
		do_stat(db, lgrs, dd, hh, mm, type);
		printf("do logger time scope [%s----%s] ...... ok\n", a.c_str(), b.c_str());
	}
	// by hour now
	for (start=time,end=start.addSecs(3600);end<=terminate;end=end.addSecs(3600)) {
		int type = By_Hour_Now;
		string dd = start.toString("%Y%m%d");
		string hh = end.addSecs(-3600).toString("%H");
		string mm = "60";
		string a = start.toString("%Y%m%d%H%M%S");
		string b = end.toString("%Y%m%d%H%M%S");
		handle.select(a, b, lgrs);
		do_stat(db, lgrs, dd, hh, mm, type);
		printf("do logger time scope [%s----%s] ...... ok\n", a.c_str(), b.c_str());
	}
	return 0;
}

