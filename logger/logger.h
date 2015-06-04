
#ifndef LOGGER_H_INCLUDED
#define LOGGER_H_INCLUDED
#include <stdio.h>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <aiodbc/aiodbc.h>
using std::map;
using std::set;
using std::string;
using std::vector;
using aiodbc::Connection;
using aiodbc::Statement;
using aiodbc::SQLException;

const char* const PHP_access="access.php";
const char* const PHP_activity_list="activity_list.php";
const char* const PHP_collect="collect.php";
const char* const PHP_collect_list="collect_list.php";
const char* const PHP_comment="comment.php";
const char* const PHP_comment_complaint="comment_complaint.php";
const char* const PHP_comment_detail="comment_detail.php";
const char* const PHP_comment_list="comment_list.php";
const char* const PHP_comment_option="comment_option.php";
const char* const PHP_guess_add="guess_add.php";
const char* const PHP_guess_status="guess_status.php";
const char* const PHP_item_detail="item_detail.php";
const char* const PHP_item_list="item_list.php";
const char* const PHP_login="login.php";
const char* const PHP_lottery_award="lottery_award.php";
const char* const PHP_lottery_detail="lottery_detail.php";
const char* const PHP_lottery_draw="lottery_draw.php";
const char* const PHP_lottery_prize="lottery_prize.php";
const char* const PHP_lottery_pv="lottery_pv.php";
const char* const PHP_lottery_share="lottery_share.php";
const char* const PHP_lottery_share_reward="lottery_share_reward.php";
const char* const PHP_lottery_winners="lottery_winners.php";
const char* const PHP_main="main.php";
const char* const PHP_passed_program_list="passed_program_list.php";
const char* const PHP_preview_program_list="preview_program_list.php";
const char* const PHP_program_comment_list="program_comment_list.php";
const char* const PHP_program_intro="program_intro.php";
const char* const PHP_program_detail="program_detail.php";
const char* const PHP_push_reset="push_reset.php";
const char* const PHP_scan="scan.php";
const char* const PHP_sign_out="sign_out.php";
const char* const PHP_still_detail="still_detail.php";
const char* const PHP_still_list="still_list.php";
const char* const PHP_store_detail="store_detail.php";
const char* const PHP_topic_comment_list="topic_comment_list.php";
const char* const PHP_user="user.php";
const char* const PHP_vote_detail="vote_detail.php";
const char* const PHP_vote_option_add="vote_option_add.php";
const char* const PHP_vote_query="vote_query.php";
const char* const PHP_redirect="redirect.php";
enum TYPE { PROGRAM = 1, ITEM = 2, STILL = 3, WEB = 4, VOTE = 5,
	LOTTERY = 6, SALES = 7, INTRO = 8, TOPIC = 9, COMMENT = 10 };

void split(const string& src,vector<string>& dest,const char& sep='&');
class Logger {
	public:
		Logger();
		~Logger();
		void parse();
		void analysis();
	public:
		string uuid;
		string country;
		string city;
		string date;
		string zone;
		string host;
		string method;
		string protocol;
		string status;
		unsigned long long bytes;
		unsigned long long timer;
		string ref;
		string agent;
		string req;
		string fwd;

		// shitao
		string page;					// page with path
		string php;						// php page
		string pf;						// platform
		string uid;						// user id
		string did;						// device id
		string appversion;				// shitao app version
		string source;					// weibo qq
		string cookie;					// cookie
		string token;					// token
		string pushDeviceToken;			// ??
		int fromId;						// fromId
		int count;						// count
		int type;						// type
		int type_id;					// real type id in url
		int program_id;					// program id
		int sound_id;					// sound id
		int item_id;					// item id
		int still_id;					// still id
		int comment_id;					// comment id
		int follow_id;					// web id
		int lottery_id;					// lottery id
		int store_id;					// store id
		int vote_id;					// vote id
		int topic_id;					// topic id
		int prize_id;					// prize id
		string collect_type;			// add or delete
		string comment_action;			// add or delete
		int comment_option;				// ding or cai
		double guess_price;				// guess price
		int expires_in;					// expires
		string phone;
		string email;
		string address;
		string redirect_url;
};
#endif

