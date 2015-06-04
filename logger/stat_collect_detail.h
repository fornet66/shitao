
#ifndef STAT_COLLECT_DETAIL_H
#define STAT_COLLECT_DETAIL_H
#include "stat_base.h"
class Stat_Collect_Detail : public Stat_Base {
	public:
		class Item {
			public:
				Item():user_pf(""),program_index(0),program_id(0),program_type(0),play_style(0),
					play_channel(0),stat_class(0),stat_id(0),collect_num(0),collect_users(0) { }
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
				int collect_num;
				int collect_users;
		};
	public:
		Stat_Collect_Detail(Connection *db) :Stat_Base(db) { }
		virtual ~Stat_Collect_Detail() { }
	protected:
		virtual int parse(const vector<Logger>& lgrs);
		virtual int indb(const string& dd, const string& hh, const string& mm, int type);
	private:
		map<string, Item> items_;
};
#endif

