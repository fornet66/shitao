
#ifndef STAT_LOGIN_H
#define STAT_LOGIN_H
#include "stat_base.h"
class Stat_Login : public Stat_Base {
	public:
		class Item {
			public:
				Item():active_num(0),login_num(0),new_login(0),keep_login(0) { }
				~Item() { }
			public:
				int active_num;
				int login_num;
				int new_login;
				int keep_login;
				double login_ratio;
		};
	public:
		Stat_Login(Connection *db) :Stat_Base(db) { }
		virtual ~Stat_Login() { }
	protected:
		virtual int parse(const vector<Logger>& lgrs);
		virtual int indb(const string& dd, const string& hh, const string& mm, int type);
	private:
		map<string,Item> items_;
};
#endif

