
#ifndef STAT_SCAN_PROGRAM_USER_H
#define STAT_SCAN_PROGRAM_USER_H
#include "stat_base.h"
class Stat_Scan_Program_User : public Stat_Base {
	public:
		class Item {
			public:
				Item():user_pf(""),program_index(0),program_id(0),program_type(0),play_style(0),play_channel(0),scan_num(0),scan_users(0),active_users(0),user_ratio(0) { }
				~Item() { }
			public:
				string user_pf;
				int program_index;
				int program_id;
				int program_type;
				int play_style;
				int play_channel;
				int scan_num;
				int scan_users;
				int active_users;
				double user_ratio;
		};
	public:
		Stat_Scan_Program_User(Connection *db) :Stat_Base(db) { }
		virtual ~Stat_Scan_Program_User() { }
	protected:
		virtual int parse(const vector<Logger>& lgrs);
		virtual int indb(const string& dd, const string& hh, const string& mm, int type);
	private:
		int active_users;
		map<string,Item> items_;
};
#endif

