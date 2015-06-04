
#include "stat_collect_detail.h"
int Stat_Collect_Detail::parse(const vector<Logger>& lgrs) {
	map<string,set<string> > collect_set;
	for(size_t i=0; i<lgrs.size(); ++i) {
		const Logger& log = lgrs[i];
		if(log.php==PHP_collect) {
			char buf[256]; memset(buf, 0, sizeof(buf));
			snprintf(buf, sizeof(buf), "%s%d%d%d", log.pf.c_str(), log.program_id, log.type, log.type_id);
			string key = string(buf);
			if(items_.find(key)==items_.end()) {
				Item item;
				item.user_pf = log.pf;
				item.program_id = log.program_id;
				item.stat_class = log.type;
				item.stat_id = log.type_id;
				items_[key] = item;
			}
			items_[key].collect_num++;
			if(collect_set[key].count(log.did)==0) {collect_set[key].insert(log.did);items_[key].collect_users++;}
		}
	}
	return 0;
}

int Stat_Collect_Detail::indb(const string& dd, const string& hh, const string& mm, int type) {
	int r = 0;
	i_->prepare("insert into "+tableName_+" (stat_date,stat_hour,stat_min,stat_type,user_pf,program_index,program_id,program_type,play_style,play_channel,stat_class,stat_id,collect_num,collect_users) values (:f1,:f2,:f3,:f4,:f5,:f6,:f7,:f8,:f9,:f10,:f11,:f12,:f13,:f14)");
	map<string,Item>::iterator it;
	for(it=items_.begin();it!=items_.end();++it) {
		Item& tt=it->second;
		// tt.program_index=
		// tt.program_type=
		// tt.play_style=
		// tt.play_channel=
		i_->clearParams();
		*i_<<dd<<hh<<mm<<type<<tt.user_pf<<tt.program_index<<tt.program_id<<tt.program_type<<tt.play_style<<tt.play_channel<<tt.stat_class<<tt.stat_id<<tt.collect_num<<tt.collect_users;
		r += i_->execute();
	}
	return r;
}

