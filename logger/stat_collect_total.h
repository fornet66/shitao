
#ifndef STAT_COLLECT_TOTAL_H
#define STAT_COLLECT_TOTAL_H
#include "stat_base.h"
class Stat_Collect_Total : public Stat_Base {
	public:
		class Item {
			public:
				Item():user_pf(""),login_users(0),collect_users(0),collect_ratio(0),collect_num(0),
					prog_collect_num(0),still_collect_num(0),item_collect_num(0) { }
				~Item() { }
			public:
				string user_pf;
				int login_users;
				int collect_users;
				double collect_ratio;
				int collect_num;
				int prog_collect_num;
				int still_collect_num;
				int item_collect_num;
		};
	public:
		Stat_Collect_Total(Connection *db) :Stat_Base(db) { }
		virtual ~Stat_Collect_Total() { }
	protected:
		virtual int parse(const vector<Logger>& lgrs);
		virtual int indb(const string& dd, const string& hh, const string& mm, int type);
	private:
		map<string, Item> items_;
};
#endif

