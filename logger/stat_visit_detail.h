
#ifndef STAT_VISIT_DETAIL_H
#define STAT_VISIT_DETAIL_H
#include "stat_base.h"
class Stat_Visit_Detail : public Stat_Base {
	public:
		class Item {
			public:
				Item():user_pf(""),program_index(0),program_id(0),program_type(0),play_style(0),play_channel(0),
					stat_class(0),stat_id(0),visit_num(0),visit_users(0),order_num(0),order_users(0),order_money(0),
					pay_num(0),pay_users(0),pay_money(0) { }
				~Item() { }
			public:
				string user_pf;
				int program_index;
				int program_id;
				int program_type;
				int play_style;
				int play_channel;
				int stat_class;
				int stat_id;
				int visit_num;
				int visit_users;
				int order_num;
				int order_users;
				int order_money;
				int pay_num;
				int pay_users;
				int pay_money;
		};
	public:
		Stat_Visit_Detail(Connection *db) :Stat_Base(db) { }
		virtual ~Stat_Visit_Detail() { }
	protected:
		virtual int parse(const vector<Logger>& lgrs);
		virtual int indb(const string& dd, const string& hh, const string& mm, int type);
	private:
		map<string,Item> items_;
};
#endif

