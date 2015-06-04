
#include "stat_comment_detail.h"
int Stat_Comment_Detail::parse(const vector<Logger>& lgrs) {
	map<string,set<string> > visit_set;
	map<string,set<string> > comment_set;
	map<string,set<string> > like_set;
	map<string,set<string> > dislike_set;
	map<string,set<string> > su_set;
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
		if(log.php==PHP_comment||log.php==PHP_comment_complaint||log.php==PHP_comment_detail
				||log.php==PHP_comment_list||log.php==PHP_comment_option) {
			if(visit_set[key].count(log.uid)==0) {visit_set[key].insert(log.uid);items_[key].comment_visit_users++;}
		}
		if(log.php==PHP_comment) {
			if(comment_set[key].count(log.uid)==0) {comment_set[key].insert(log.uid);items_[key].comment_users++;}
		}
		if(log.php==PHP_comment_option) {
			if(like_set[key].count(log.uid)==0) {like_set[key].insert(log.uid);items_[key].like_users++;}
			if(dislike_set[key].count(log.uid)==0) {dislike_set[key].insert(log.uid);items_[key].dislike_users++;}
		}
		if(log.php==PHP_comment_complaint) {
			if(su_set[key].count(log.uid)==0) {su_set[key].insert(log.uid);items_[key].su_users++;}
		}
	}
	return 0;
}

int Stat_Comment_Detail::indb(const string& dd, const string& hh, const string& mm, int type) {
	int r = 0;
	i_->prepare("insert into "+tableName_+" (stat_date,stat_hour,stat_min,stat_type,user_pf,program_index,program_id,program_type,play_style,play_channel,comment_visit_users,comment_users,like_users,dislike_users,su_users) values (:f1,:f2,:f3,:f4,:f5,:f6,:f7,:f8,:f9,:f10,:f11,:f12,:f13,:f14,:f15)");
	map<string,Item>::iterator it;
	for(it=items_.begin();it!=items_.end();++it) {
		Item& tt=it->second;
		// tt.program_index=
		// tt.program_type=
		// tt.play_style=
		// tt.play_channel=
		i_->clearParams();
		*i_<<dd<<hh<<mm<<type<<tt.user_pf<<tt.program_index<<tt.program_id<<tt.program_type<<tt.play_style<<tt.play_channel<<tt.comment_visit_users<<tt.comment_users<<tt.like_users<<tt.dislike_users<<tt.su_users;
		r += i_->execute();
	}
	return r;
}

