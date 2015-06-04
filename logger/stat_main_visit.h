
#ifndef STAT_MAIN_VISIT_H
#define STAT_MAIN_VISIT_H
#include "stat_base.h"
class Stat_Main_Visit : public Stat_Base {
	public:
		class Item {
			public:
				Item():user_pf(""),appversion(""),page_type(1),page_name(""),visit_num(0),visit_users(0) { }
				~Item() { }
			public:
				string user_pf;
				string appversion;
				int page_type;
				string page_name;
				int visit_num;
				int visit_users;
		};
	public:
		Stat_Main_Visit(Connection *db) :Stat_Base(db) { }
		virtual ~Stat_Main_Visit() { }
	protected:
		virtual int parse(const vector<Logger>& lgrs);
		virtual int indb(const string& dd, const string& hh, const string& mm, int type);
	private:
		map<string, Item> items_;
};
#endif

