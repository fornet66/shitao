
#include <stdlib.h>
#include <aiuuid/aiuuid.h>
#include "logger.h"

void split(const string& src,vector<string>& dest,const char& sep) {
	string lsrc;
	dest.clear();
	// Add seperator at end
	if(src[src.size()-1] != sep)
		lsrc = src + sep;
	else
		lsrc = src;
	int start = 0,end = lsrc.size();
	while((end=lsrc.substr(start,lsrc.size()).find(sep))!=-1) {
		dest.push_back(lsrc.substr(start,end));
		start += end+1;
	}
}

Logger::Logger()
	:uuid(""),
	date(""),
	zone(""),
	host(""),
	method(""),
	protocol(""),
	status(""),
	bytes(0),
	timer(0),
	ref(""),
	agent(""),
	req(""),
	fwd(""),
	page(""),
	php(""),
	pf(""),
	uid(""),
	did(""),
	appversion(""),
	source(""),
	cookie(""),
	token(""),
	pushDeviceToken(""),
	fromId(0),
	count(0),
	type(0),
	type_id(0),
	program_id(0),
	sound_id(0),
	item_id(0),
	still_id(0),
	comment_id(0),
	follow_id(0),
	lottery_id(0),
	store_id(0),
	vote_id(0),
	topic_id(0),
	prize_id(0),
	collect_type(""),
	comment_action(""),
	comment_option(0),
	guess_price(0),
	expires_in(0),
	phone(""),
	email(""),
	address(""),
	redirect_url("")
{ }

Logger::~Logger() {
}

void Logger::parse() {
	uuid = aiuuid::get_uuid_str();
	page = req.substr(0, req.find('?'));
	php = page.substr(page.rfind('/')+1);
	vector<string> kv;
	split(req.substr(req.find('?')+1), kv);
	char sep = '=';
	for(size_t i=0; i<kv.size(); ++i) {
		int p = kv[i].find(sep);
		if(p!=-1) {
			string k = kv[i].substr(0,p);
			string v = kv[i].substr(p+1);
			if(php==PHP_comment_complaint&&k=="soruce") {k="";type=COMMENT;}		// 不清楚后续处理
			if(php==PHP_comment_detail) type = COMMENT;
			if(php==PHP_comment_option&&k=="type") {k="";type=COMMENT;comment_option=atoi(v.c_str());}
			if(php==PHP_guess_add) type=ITEM;
			if(php==PHP_guess_status) type=ITEM;
			if(php==PHP_item_detail) type=ITEM;
			if(php==PHP_login&&k=="id") uid=v;
			if(php==PHP_lottery_award&&k=="trueId") uid=v;
			if(php==PHP_lottery_detail) type=LOTTERY;
			if(php==PHP_lottery_draw) type=LOTTERY;
			if(php==PHP_lottery_share) type=LOTTERY;
			if(php==PHP_lottery_share_reward) type=LOTTERY;
			if(php==PHP_lottery_winners) type=LOTTERY;
			if(php==PHP_program_intro) type=PROGRAM;
			if(php==PHP_program_detail) type=PROGRAM;
			if(php==PHP_still_detail) type=STILL;
			if(php==PHP_store_detail) type=SALES;
			if(php==PHP_topic_comment_list) type=TOPIC;
			if(php==PHP_vote_option_add) type=VOTE;
			if(php==PHP_vote_query) type=VOTE;
			if(k=="pf") pf = v;
			if(k=="uid") uid = v;
			if(k=="did") did = v;
			if(k=="appversion") appversion = v;
			if(k=="source") source = v;
			if(k=="cookie") cookie = v;
			if(k=="token") token = v;
			if(k=="pushDeviceToken") pushDeviceToken = v;
			if(k=="fromId") fromId = atoi(v.c_str());
			if(k=="count") count = atoi(v.c_str());
			if(k=="type") type = atoi(v.c_str());
			if(k=="id") type_id = atoi(v.c_str());
			if(k=="soundId") sound_id = atoi(v.c_str());
			if(k=="prizeId") prize_id = atoi(v.c_str());
			if(k=="ask_status") collect_type = v;
			if(k=="action") comment_action = v;
			if(k=="price") guess_price = atof(v.c_str());
			if(k=="expires_in") expires_in = atoi(v.c_str());
			if(k=="phone") phone = v;
			if(k=="email") email = v;
			if(k=="address") address = v;
			if(k=="url") redirect_url=v;
			analysis();
		}
	}
}

void Logger::analysis() {
	switch (type) {
		case PROGRAM:
			program_id = type_id;		// program id
			break;
		case ITEM:
			item_id = type_id;			// item id
			break;
		case STILL:
			still_id = type_id;			// still id
			break;
		case WEB:
			follow_id = type_id;		// web id
			break;
		case VOTE:
			vote_id = type_id;			// vote id
			break;
		case LOTTERY:
			lottery_id = type_id;		// lottery id
			break;
		case SALES:
			store_id = type_id;			// store id
			break;
			/*
		case INTRO:
			intro_id = type_id;			// introduce id
			break;
			*/
		case TOPIC:
			topic_id = type_id;			// topic id
			break;
		case COMMENT:
			comment_id = type_id;		// comment id
			break;
	}
}

