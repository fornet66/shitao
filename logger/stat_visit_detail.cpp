
#include "stat_visit_detail.h"
int Stat_Visit_Detail::parse(const vector<Logger>& lgrs) {
	map<string,set<string> > visit_set;
	for(size_t i=0; i<lgrs.size(); ++i) {
		const Logger& log = lgrs[i];
		if(log.program_id>0) {
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
			items_[key].visit_num++;
			if(visit_set[key].count(log.did)==0) {visit_set[key].insert(log.did);items_[key].visit_users++;}
		}
	}
	return 0;
}

int Stat_Visit_Detail::indb(const string& dd, const string& hh, const string& mm, int type) {
	int r = 0;
	i_->prepare("insert into "+tableName_+" (stat_date,stat_hour,stat_min,stat_type,user_pf,program_index,program_id,program_type,play_style,play_channel,stat_class,stat_id,visit_num,visit_users,order_num,order_users,order_money,pay_num,pay_users,pay_money) values (:f1,:f2,:f3,:f4,:f5,:f6,:f7,:f8,:f9,:f10,:f11,:f12,:f13,:f14,:f15,:f16,:f17,:f18,:f19,:f20)");
	map<string,Item>::iterator it;
	for(it=items_.begin();it!=items_.end();++it) {
		Item& tt=it->second;
		// tt.program_index=
		// tt.program_type=
		// tt.play_style=
		// tt.play_channel=
		i_->clearParams();
		*i_<<dd<<hh<<mm<<type<<tt.user_pf<<tt.program_index<<tt.program_id<<tt.program_type<<tt.play_style<<tt.play_channel<<tt.stat_class<<tt.stat_id<<tt.visit_num<<tt.visit_users<<tt.order_num<<tt.order_users<<tt.order_money<<tt.pay_num<<tt.pay_users<<tt.pay_money;
		r += i_->execute();
	}
	return r;
}

