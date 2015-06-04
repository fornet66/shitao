
#ifndef STAT_BASE_H
#define STAT_BASE_H
#include "logger.h"
class Stat_Base {
	public:
		Stat_Base(Connection *db)
			:db_(db),i_(0),d_(0)
		{
			i_=new Statement;
			*i_=db_->createStatement();
			d_=new Statement;
			*d_=db_->createStatement();
		}
		virtual ~Stat_Base()
		{
			if(i_!=0) {
				i_->close();
				delete i_;
				i_=0;
			}
			if(d_!=0) {
				d_->close();
				delete d_;
				d_=0;
			}
		}
		int stat(vector<Logger> &lgrs, const string& dd, const string& hh, const string& mm, int type);
		void tableName(const string& table) {tableName_ = table;}
	protected:
		int clean(const string& dd, const string& hh, const string&mm, int type);
		virtual int parse(const vector<Logger> &lgrs) = 0;
		virtual int indb(const string& dd, const string& hh, const string& mm, int type) = 0;
	protected:
		Connection *db_;
		Statement *i_, *d_;
		string tableName_;
};
#endif

