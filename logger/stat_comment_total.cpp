
#include "stat_comment_total.h"
int Stat_Comment_Total::parse(const vector<Logger>& lgrs) {
	map<string,set<string> > visit_set;
	map<string,set<string> > comment_set;
	map<string,set<string> > like_set;
	map<string,set<string> > dislike_set;
	map<string,set<string> > su_set;
	map<string,set<string> > login_set;
	for(size_t i=0; i<lgrs.size(); ++i) {
		const Logger& log = lgrs[i];
		string key = log.pf;
		if(items_.find(key)==items_.end()) {
			Item item;
			item.user_pf = log.pf;
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
		if(login_set[key].count(log.uid)==0) {login_set[key].insert(log.uid);items_[key].login_users++;}
	}
	return 0;
}

int Stat_Comment_Total::indb(const string& dd, const string& hh, const string& mm, int type) {
	int r = 0;
	i_->prepare("insert into "+tableName_+" (stat_date,stat_hour,stat_min,stat_type,user_pf,comment_visit_users,comment_users,like_users,dislike_users,su_users,login_users,comment_ratio) values (:f1,:f2,:f3,:f4,:f5,:f6,:f7,:f8,:f9,:f10,:f11,:f12)");
	map<string,Item>::iterator it;
	for(it=items_.begin();it!=items_.end();++it) {
		Item& tt=it->second;
		if(tt.login_users>0) tt.comment_ratio=(double)tt.comment_users/tt.login_users;
		i_->clearParams();
		*i_<<dd<<hh<<mm<<type<<tt.user_pf<<tt.comment_visit_users<<tt.comment_users<<tt.like_users<<tt.dislike_users<<tt.su_users<<tt.login_users<<tt.comment_ratio;
		r += i_->execute();
	}
	return r;
}

