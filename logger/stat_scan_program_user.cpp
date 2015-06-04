
#include "stat_scan_program_user.h"
int Stat_Scan_Program_User::parse(const vector<Logger>& lgrs) {
	set<string> active_set;
	map<string,set<string> > scan_set;
	for(size_t i=0; i<lgrs.size(); ++i) {
		const Logger& log = lgrs[i];
		char buf[256]; memset(buf, 0, sizeof(buf));
		snprintf(buf, sizeof(buf), "%s%d", log.pf.c_str(), log.program_id);
		string key = string(buf);
		if(items_.find(key)==items_.end()) {
			Item item;
			item.user_pf = log.pf;
			item.program_id = log.program_id;
			items_[key] = item;
		}
		if(log.php==PHP_scan) {
			items_[key].scan_num++;
			if(scan_set[key].count(log.did)==0) {scan_set[key].insert(log.did);items_[key].scan_users++;}
		}
		if(active_set.count(log.did)==0) {active_set.insert(log.did);active_users++;}
	}
	return 0;
}

int Stat_Scan_Program_User::indb(const string& dd, const string& hh, const string& mm, int type) {
	int r = 0;
	i_->prepare("insert into "+tableName_+" (stat_date,stat_hour,stat_min,stat_type,user_pf,program_index,program_id,program_type,play_style,play_channel,scan_num,scan_users,active_users,user_ratio) values (:f1,:f2,:f3,:f4,:f5,:f6,:f7,:f8,:f9,:f10,:f11,:f12,:f13,:f14)");
	map<string,Item>::iterator it;
	for(it=items_.begin();it!=items_.end();++it) {
		Item& tt=it->second;
		// tt.program_index=
		// tt.program_type=
		// tt.play_style=
		// tt.play_channel=
		tt.active_users=active_users;
		if(tt.active_users>0) tt.user_ratio=(double)tt.scan_users/tt.active_users;
		i_->clearParams();
		*i_<<dd<<hh<<mm<<type<<tt.user_pf<<tt.program_index<<tt.program_id<<tt.program_type<<tt.play_style<<tt.play_channel<<tt.scan_num<<tt.scan_users<<tt.active_users<<tt.user_ratio;
		r += i_->execute();
	}
	return r;
}

