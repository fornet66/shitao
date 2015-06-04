
#include "stat_vote_total.h"
int Stat_Vote_Total::parse(const vector<Logger>& lgrs) {
	set<string> login_set;
	map<int,set<string> > visit_set;
	map<int,set<string> > vote_set;
	map<int,set<string> > comment_visit_set;
	map<int,set<string> > comment_set;
	for(size_t i=0; i<lgrs.size(); ++i) {
		const Logger& log = lgrs[i];
		int key = log.vote_id;
		if(log.php==PHP_vote_detail||log.php==PHP_vote_query||log.php==PHP_vote_option_add) {
			if(key!=0 && items_.find(key)==items_.end()) {
				Item item;
				item.vote_id = log.vote_id;
				items_[key] = item;
			}
		}
		if(log.php==PHP_vote_detail||log.php==PHP_vote_query) {
			if(visit_set[key].count(log.uid)==0) {visit_set[key].insert(log.uid);items_[key].vote_visit_users++;}
		}
		if(log.php==PHP_vote_option_add) {
			if(vote_set[key].count(log.uid)==0) {vote_set[key].insert(log.uid);items_[key].vote_users++;}
		}
		if(log.php==PHP_comment&&log.type==VOTE&&log.vote_id!=0) {
			if(comment_set[key].count(log.uid)==0) {comment_set[key].insert(log.uid);items_[key].vote_comment_users++;}
		}
		if(login_set.count(log.uid)==0) {login_set.insert(log.uid);login_users++;}
	}
	return 0;
}

int Stat_Vote_Total::indb(const string& dd, const string& hh, const string& mm, int type) {
	int r = 0;
	i_->prepare("insert into "+tableName_+" (stat_date,stat_hour,stat_min,stat_type,login_users,vote_id,vote_visit_users,vote_users,vote_ratio,vote_comment_visit_users,vote_comment_users,vote_comment_visit_ratio,vote_comment_ratio) values (:f1,:f2,:f3,:f4,:f5,:f6,:f7,:f8,:f9,:f10,:f11,:f12,:f13)");
	map<int,Item>::iterator it;
	for(it=items_.begin();it!=items_.end();++it) {
		Item& tt=it->second;
		tt.login_users=login_users;
		if(tt.vote_visit_users>0) tt.vote_ratio=(double)tt.vote_users/tt.vote_visit_users;
		i_->clearParams();
		*i_<<dd<<hh<<mm<<type<<tt.login_users<<tt.vote_id<<tt.vote_visit_users<<tt.vote_users<<tt.vote_ratio<<tt.vote_comment_visit_users<<tt.vote_comment_users<<tt.vote_comment_visit_ratio<<tt.vote_comment_ratio;
		r += i_->execute();
	}
	return r;
}

