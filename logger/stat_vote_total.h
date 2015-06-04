
#ifndef STAT_VOTE_TOTAL_H
#define STAT_VOTE_TOTAL_H
#include "stat_base.h"
class Stat_Vote_Total : public Stat_Base {
	public:
		class Item {
			public:
				Item():login_users(0),vote_id(0),vote_visit_users(0),vote_users(0),vote_ratio(0),vote_comment_visit_users(0),vote_comment_users(0),vote_comment_visit_ratio(0),vote_comment_ratio(0) { }
				~Item() { }
			public:
				int login_users;
				int vote_id;
				int vote_visit_users;
				int vote_users;
				double vote_ratio;
				int vote_comment_visit_users;
				int vote_comment_users;
				double vote_comment_visit_ratio;
				double vote_comment_ratio;
		};
	public:
		Stat_Vote_Total(Connection *db) :Stat_Base(db) {login_users=0;}
		virtual ~Stat_Vote_Total() { }
	protected:
		virtual int parse(const vector<Logger>& lgrs);
		virtual int indb(const string& dd, const string& hh, const string& mm, int type);
	private:
		int login_users;
		map<int, Item> items_;
};
#endif

