
#ifndef STAT_COMMENT_TOTAL_H
#define STAT_COMMENT_TOTAL_H
#include "stat_base.h"
class Stat_Comment_Total : public Stat_Base {
	public:
		class Item {
			public:
				Item():user_pf(""),comment_visit_users(0),comment_users(0),like_users(0),dislike_users(0),su_users(0),login_users(0),comment_ratio(0) { }
				~Item() { }
			public:
				string user_pf;
				int comment_visit_users;
				int comment_users;
				int like_users;
				int dislike_users;
				int su_users;
				int login_users;
				double comment_ratio;
		};
	public:
		Stat_Comment_Total(Connection *db) :Stat_Base(db) { }
		virtual ~Stat_Comment_Total() { }
	protected:
		virtual int parse(const vector<Logger>& lgrs);
		virtual int indb(const string& dd, const string& hh, const string& mm, int type);
	private:
		map<string, Item> items_;
};
#endif

