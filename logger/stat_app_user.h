
#ifndef STAT_APP_USER_H
#define STAT_APP_USER_H
#include "stat_base.h"
class Stat_App_User : public Stat_Base {
	public:
		class Item {
			public:
				Item():user_pf(""),appversion(""),user_source(""),new_users(0),active_users(0),
					startup_num(0),total_num(0),active_ratio(0),active_user_ratio(0) { }
				~Item() { }
			public:
				string user_pf;
				string appversion;
				string user_source;
				int new_users;
				int active_users;
				int startup_num;
				int total_num;
				double active_ratio;
				double active_user_ratio;
		};
	public:
		Stat_App_User(Connection *db) :Stat_Base(db) { }
		virtual ~Stat_App_User() { }
	protected:
		virtual int parse(const vector<Logger>& lgrs);
		virtual int indb(const string& dd, const string& hh, const string& mm, int type);
	private:
		map<string,Item> items_;
};
#endif

