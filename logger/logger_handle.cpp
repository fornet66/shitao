
#include "logger_handle.h"
LoggerHandle::LoggerHandle(Connection *db)
	:db_(db),s_(0),i_(0)
{
	if(s_==0) {
		s_=new Statement;
		*s_=db->createStatement();
		s_->prepare("select uuid, country, city,"
				"access_time, zone, user_host, http_method, http_protocol, http_status, body_bytes_sent, response_time, http_referer, http_user_agent,"
				"http_x_forwarded_for, php_name, user_pf, uid, did, appversion, user_source, cookie, token, list_from_id,"
				"list_count, main_type, program_id, sound_id, item_id, still_id, comment_id, follow_id, lottery_id, store_id,"
				"vote_id, topic_id, prize_id, collect_type, comment_action, comment_option, guess_price, expires_in, lottery_phone, lottery_email, "
				"lottery_addr, redirect_url from app_log where access_time>=? and access_time<?");
	}
	if(i_==0) {
		i_=new Statement;
		*i_=db->createStatement();
		i_->prepare("insert into app_log (uuid,"
#ifdef HAVE_LIBGEOIP
				"country, city,"
#endif
				"access_time, zone, user_host, http_method, http_protocol, http_status, body_bytes_sent, response_time, http_referer, http_user_agent,"
				"http_x_forwarded_for, php_name, user_pf, uid, did, appversion, user_source, cookie, token, list_from_id,"
				"list_count, main_type, program_id, sound_id, item_id, still_id, comment_id, follow_id, lottery_id, store_id,"
				"vote_id, topic_id, prize_id, collect_type, comment_action, comment_option, guess_price, expires_in, lottery_phone, lottery_email, "
				"lottery_addr, redirect_url) values (?,"
#ifdef HAVE_LIBGEOIP
				"?,?,"
#endif
				"?,?,?,?,?,?,?,?,?,?,"
				"?,?,?,?,?,?,?,?,?,?,"
				"?,?,?,?,?,?,?,?,?,?,"
				"?,?,?,?,?,?,?,?,?,?,"
				"?,?)");
	}
}

LoggerHandle::~LoggerHandle() {
	if(s_!=0) {
		s_->close();
		delete s_;
		s_=0;
	}
	if(i_!=0) {
		i_->close();
		delete i_;
		i_=0;
	}
}

int LoggerHandle::select(const string& start, const string& end, vector<Logger>& lgrs) {
	lgrs.clear();
	lgrs.reserve(60 * 10000);
	s_->clearParams();
	*s_ << start << end;
	s_->execute();
	while(s_->next()) {
		Logger log;
		string fwd;
		int64 bytes, timer;
		*s_ >> log.uuid >> log.country >> log.city
			>>	log.date >>	log.zone >>	log.host >>	log.method >> log.protocol >> log.status >>	bytes >> timer >> log.ref >> log.agent
			>> fwd >> log.php >> log.pf >> log.uid >> log.did >> log.appversion >> log.source >> log.cookie >> log.token >> log.fromId
			>> log.count >> log.type >> log.program_id >> log.sound_id >> log.item_id >> log.still_id >> log.comment_id >> log.follow_id >> log.lottery_id >> log.store_id
			>> log.vote_id >> log.topic_id >> log.prize_id >> log.collect_type >> log.comment_action >> log.comment_option >> log.guess_price >> log.expires_in >> log.phone >> log.email
			>> log.address >> log.redirect_url;
		log.fwd = fwd;
		log.bytes = bytes;
		log.timer = timer;
		lgrs.push_back(log);
	}
	return lgrs.size();
}

int LoggerHandle::insert(const vector<Logger>& lgrs) {
	for(size_t i=0; i<lgrs.size(); ++i) {
		i_->clearParams();
		*i_ << lgrs[i].uuid;
#ifdef HAVE_LIBGEOIP
		char country[COUNTRY_LEN] = "";
		char city[CITY_LEN] = "";
		geoip_get_country (lgrs[i].host.c_str(), country);
		if (conf.geoip_city_data) {
			geoip_get_city (lgrs[i].host.c_str(), city);
		}
		*i_ <<	string(country)
			<<	string(city);
#endif
		*i_ <<	lgrs[i].date <<	lgrs[i].zone <<	lgrs[i].host <<	lgrs[i].method << lgrs[i].protocol << lgrs[i].status <<	static_cast<int64>(lgrs[i].bytes) << static_cast<int64>(lgrs[i].timer) << lgrs[i].ref << lgrs[i].agent
			<< string("") << lgrs[i].php << lgrs[i].pf << lgrs[i].uid << lgrs[i].did << lgrs[i].appversion << lgrs[i].source << lgrs[i].cookie << lgrs[i].token << lgrs[i].fromId
			<< lgrs[i].count << lgrs[i].type << lgrs[i].program_id << lgrs[i].sound_id << lgrs[i].item_id << lgrs[i].still_id << lgrs[i].comment_id << lgrs[i].follow_id << lgrs[i].lottery_id << lgrs[i].store_id
			<< lgrs[i].vote_id << lgrs[i].topic_id << lgrs[i].prize_id << lgrs[i].collect_type << lgrs[i].comment_action << lgrs[i].comment_option << lgrs[i].guess_price << lgrs[i].expires_in << lgrs[i].phone << lgrs[i].email
			<< lgrs[i].address << lgrs[i].redirect_url;
		i_->execute();
		if(i%1000==0) db_->commit();
	}
	db_->commit();
	return lgrs.size();
}

