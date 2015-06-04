
#include "stat_main_visit.h"
int Stat_Main_Visit::parse(const vector<Logger>& lgrs) {
	map<string,set<string> > visit_set;
	for(size_t i=0; i<lgrs.size(); ++i) {
		const Logger& log = lgrs[i];
		string key = log.pf+log.appversion+log.php;
		if(items_.find(key)==items_.end()) {
			Item item;
			item.user_pf = log.pf;
			item.appversion = log.appversion;
			item.page_name = log.php;
			if(log.php==PHP_main||log.php==PHP_passed_program_list||log.php==PHP_scan
					||log.php==PHP_preview_program_list||log.php==PHP_activity_list
					||log.php==PHP_collect_list) {
				item.page_type = 0;
			}
			items_[key] = item;
		}
		items_[key].visit_num++;
		if(visit_set[key].count(log.did)==0) {visit_set[key].insert(log.did);items_[key].visit_users++;}
	}
	return 0;
}

int Stat_Main_Visit::indb(const string& dd, const string& hh, const string& mm, int type) {
	int r = 0;
	i_->prepare("insert into "+tableName_+" (stat_date,stat_hour,stat_min,stat_type,user_pf,appversion,page_type,page_name,visit_num,visit_users) values (:f1,:f2,:f3,:f4,:f5,:f6,:f7,:f8,:f9,:f10)");
	map<string,Item>::iterator it;
	for(it=items_.begin();it!=items_.end();++it) {
		Item& tt=it->second;
		i_->clearParams();
		*i_<<dd<<hh<<mm<<type<<tt.user_pf<<tt.appversion<<tt.page_type<<tt.page_name<<tt.visit_num<<tt.visit_users;
		r += i_->execute();
	}
	return r;
}

