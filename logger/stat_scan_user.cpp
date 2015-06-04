
#include "stat_scan_user.h"
int Stat_Scan_User::parse(const vector<Logger>& lgrs) {
	scan_num=0;
	scan_users=0;
	active_users=0;
	set<string> active_set;
	set<string> scan_set;
	for(size_t i=0; i<lgrs.size(); ++i) {
		const Logger& log = lgrs[i];
		if(log.php==PHP_scan) {
			scan_num++;
			if(scan_set.count(log.did)==0) {scan_set.insert(log.did);scan_users++;}
		}
		if(active_set.count(log.did)==0) {active_set.insert(log.did);active_users++;}
	}
	if(active_users>0) user_ratio=(double)scan_users/active_users;
	return 0;
}

int Stat_Scan_User::indb(const string& dd, const string& hh, const string& mm, int type) {
	int r = 0;
	i_->prepare("insert into "+tableName_+" (stat_date,stat_hour,stat_min,stat_type,scan_num,scan_users,active_users,user_ratio) values (:f1,:f2,:f3,:f4,:f5,:f6,:f7,:f8)");
	i_->clearParams();
	*i_<<dd<<hh<<mm<<type<<scan_num<<scan_users<<active_users<<user_ratio;
	r += i_->execute();
	return r;
}

