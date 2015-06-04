
#include "stat_base.h"

int Stat_Base::stat(vector<Logger> &lgrs, const string& dd, const string& hh, const string& mm, int type) {
	this->parse(lgrs);
	this->clean(dd, hh, mm, type);
	return this->indb(dd, hh, mm, type);
}

int Stat_Base::clean(const string& dd, const string& hh, const string&mm, int type) {
	d_->prepare("delete from "+tableName_+" where stat_date=? and stat_hour=? and stat_min=? and stat_type=?");
	d_->clearParams();
	*d_ << dd << hh << mm << type;
	return d_->execute();
}

