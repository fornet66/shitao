
#include "stat_collect_total.h"
int Stat_Collect_Total::parse(const vector<Logger>& lgrs) {
	map<string,set<string> > login_set;
	map<string,set<string> > collect_set;
	for(size_t i=0; i<lgrs.size(); ++i) {
		const Logger& log = lgrs[i];
		string key = log.pf;
		if(items_.find(key)==items_.end()) {
			Item item;
			item.user_pf = log.pf;
			items_[key] = item;
		}
		if(log.php==PHP_collect||log.php==PHP_collect_list) {
			if(collect_set[key].count(log.uid)==0) {collect_set[key].insert(log.uid);items_[key].collect_users++;}
			if(log.type!=0) items_[key].collect_num++;
			if(log.type==PROGRAM) items_[key].prog_collect_num++;
			if(log.type==STILL) items_[key].still_collect_num++;
			if(log.type==ITEM) items_[key].item_collect_num++;
		}
		if(login_set[key].count(log.uid)==0) {login_set[key].insert(log.uid);items_[key].login_users++;}
	}
	return 0;
}

int Stat_Collect_Total::indb(const string& dd, const string& hh, const string& mm, int type) {
	int r = 0;
	i_->prepare("insert into "+tableName_+" (stat_date,stat_hour,stat_min,stat_type,user_pf,login_users,collect_users,collect_ratio,collect_num,program_collect_num,still_collect_num,item_collect_num) values (:f1,:f2,:f3,:f4,:f5,:f6,:f7,:f8,:f9,:f10,:f11,:f12)");
	map<string,Item>::iterator it;
	for(it=items_.begin();it!=items_.end();++it) {
		Item& tt=it->second;
		if(tt.login_users>0) tt.collect_ratio=(double)tt.collect_users/tt.login_users;
		i_->clearParams();
		*i_<<dd<<hh<<mm<<type<<tt.user_pf<<tt.login_users<<tt.collect_users<<tt.collect_ratio<<tt.collect_num<<tt.prog_collect_num<<tt.still_collect_num<<tt.item_collect_num;
		r += i_->execute();
	}
	return r;
}

