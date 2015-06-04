
#ifndef STAT_SCAN_USER_H
#define STAT_SCAN_USER_H
#include "stat_base.h"
class Stat_Scan_User : public Stat_Base {
	public:
		Stat_Scan_User(Connection *db) :Stat_Base(db) { }
		virtual ~Stat_Scan_User() { }
	protected:
		virtual int parse(const vector<Logger>& lgrs);
		virtual int indb(const string& dd, const string& hh, const string& mm, int type);
	private:
		int scan_num;
		int scan_users;
		int active_users;
		double user_ratio;
};
#endif

