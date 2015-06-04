
#include "stat_login.h"
int Stat_Login::parse(const vector<Logger>& lgrs) {
	map<string,set<string> > active_set;
	map<string,set<string> > login_set;
	for(size_t i=0; i<lgrs.size(); ++i) {
		const Logger& log = lgrs[i];
		if(log.php==PHP_main) {
			if(login_set[log.pf].count(log.did) == 0) {
				login_set[log.pf].insert(log.did);
				items_[log.pf].login_num++;
			}
		}
		if(active_set[log.pf].count(log.did) == 0) {
			active_set[log.pf].insert(log.did);
			items_[log.pf].active_num++;
		}
	}
	return 0;
}

int Stat_Login::indb(const string& dd, const string& hh, const string& mm, int type) {
	int r = 0;
	i_->prepare("insert into "+tableName_+" (stat_date,stat_hour,stat_min,stat_type,user_pf,active_num,login_num,login_ratio) values (:f1,:f2,:f3,:f4,:f5,:f6,:f7,:f8)");
	map<string,Item>::iterator it;
	for(it=items_.begin();it!=items_.end();++it) {
		Item& tt=it->second;
		if(tt.active_num>0) tt.login_ratio=(double)tt.login_num/tt.active_num;
		i_->clearParams();
		*i_<<dd<<hh<<mm<<type<<it->first<<tt.active_num<<tt.login_num<<tt.login_ratio;
		r += i_->execute();
	}
	return r;
}

