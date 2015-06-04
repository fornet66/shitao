
#include "stat_app_user.h"
int Stat_App_User::parse(const vector<Logger>& lgrs) {
	map<string,set<string> > active_set;
	for(size_t i=0; i<lgrs.size(); ++i) {
		const Logger& log = lgrs[i];
		string key = log.pf+log.appversion+log.source;
		if(items_.find(key)==items_.end()) {
			Item item;
			item.user_pf = log.pf;
			item.appversion = log.appversion;
			item.user_source = log.source;
			items_[key] = item;
		}
		// if(select log.uid) {items_[key].new_users++;}
		if(active_set[key].count(log.did)==0) {active_set[key].insert(log.did);items_[key].active_users++;}
		// select count(*) from api_user items_[key].total_num=total;
	}
	return 0;
}

int Stat_App_User::indb(const string& dd, const string& hh, const string& mm, int type) {
	int r = 0;
	i_->prepare("insert into "+tableName_+" (stat_date,stat_hour,stat_min,stat_type,user_pf,appversion,user_source,new_users,active_users,startup_num,total_num,active_ratio,active_user_ratio) values (:f1,:f2,:f3,:f4,:f5,:f6,:f7,:f8,:f9,:f10,:f11,:f12,:f13)");
	map<string,Item>::iterator it;
	for(it=items_.begin();it!=items_.end();++it) {
		Item& tt=it->second;
		i_->clearParams();
		*i_<<dd<<hh<<mm<<type<<tt.user_pf<<tt.appversion<<tt.user_source<<tt.new_users<<tt.active_users<<tt.startup_num<<tt.total_num<<tt.active_ratio<<tt.active_user_ratio;
		r += i_->execute();
	}
	return r;
}

