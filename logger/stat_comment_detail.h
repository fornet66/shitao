
#ifndef STAT_COMMENT_DETAIL_H
#define STAT_COMMENT_DETAIL_H
#include "stat_base.h"
class Stat_Comment_Detail : public Stat_Base {
	public:
		class Item {
			public:
				Item():user_pf(""),program_index(0),program_id(0),program_type(0),play_style(0),play_channel(0),comment_visit_users(0),comment_users(0),like_users(0),dislike_users(0),su_users(0) { }
				~Item() { }
			public:
				string user_pf;
				int program_index;
				int program_id;
				int program_type;
				int play_style;
				int play_channel;
				int comment_visit_users;
				int comment_users;
				int like_users;
				int dislike_users;
				int su_users;
		};
	public:
		Stat_Comment_Detail(Connection *db) :Stat_Base(db) { }
		virtual ~Stat_Comment_Detail() { }
	protected:
		virtual int parse(const vector<Logger>& lgrs);
		virtual int indb(const string& dd, const string& hh, const string& mm, int type);
	private:
		map<string, Item> items_;
};
#endif

