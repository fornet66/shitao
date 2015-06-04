
#ifndef __IBS_USAGE_H__
#define __IBS_USAGE_H__
#include "define.h"
#include "rule.h"
namespace ibs {
	class Usage {
		public:
			Usage()
				:sale_id(0),
				user_id(0),
				item_id(0),
				action(""),
				action_object(""),
				user_ip(""),
				click_url(""),
				action_time(),
				virtual_action_time(),
				uuid(""),
				cycle_id(""),
				acct_id(0),
				ori_fee(0),
				discnt_fee(0),
				real_fee(0),
				fee_rule_id(0),
				fee_dtl_id(0),
				subject_id(0),
				cal_time(),
				update_time()
			{ }
			~Usage() { }
			string toString() const {
				char buf[2048];
				snprintf(buf, sizeof(buf), "%d,%d,%d,%s,%s,%s,%s,%s,%s,%s,%s,%d,%d,%d,%d,%d,%d,%d,%d,%s",
						sale_id, user_id, item_id, action.c_str(), action_object.c_str(), user_ip.c_str(),
						click_url.c_str(), action_time.toString().c_str(),
						virtual_action_time.toString().c_str(),
						uuid.c_str(), cycle_id.c_str(), acct_id, ori_fee, discnt_fee,
						real_fee, fee_rule_id, fee_dtl_id, addup_id, subject_id,
						cal_time.toString().c_str());
				return string(buf);
			}
			string item() const {
				char buf[256];
				snprintf(buf,sizeof(buf),"%d", item_id);
				return string(buf);
			}
			string ia() const {
				char buf[256];
				snprintf(buf,sizeof(buf),"%d-%s", item_id, action.c_str());
				return string(buf);
			}
			string iao() const {
				char buf[256];
				snprintf(buf,sizeof(buf),"%d-%s-%s", item_id, action.c_str(), action_object.c_str());
				return string(buf);
			}
			void maketime() {
				virtual_action_time = action_time;
				OSSDateTime now = OSSDateTime::now();
				if(action_time.month()!=now.month())
					virtual_action_time = now;
			}
			void makerule(const Rule& rule) {
				fee_dtl_id = rule.dtlId();
				subject_id = rule.subjectId();
				addup_id = rule.addupId();
				ori_fee = rule.unitFee();
				discnt_fee = 0;
				real_fee = ori_fee-discnt_fee;
			}
		public:
			// cdr content
			int sale_id;
			int user_id;
			int item_id;
			string action;
			string action_object;
			string user_ip;
			string click_url;
			OSSDateTime action_time;
			OSSDateTime virtual_action_time;
			// rate content
			string uuid;
			string cycle_id;
			int acct_id;
			int ori_fee;
			int discnt_fee;
			int real_fee;
			int fee_rule_id;
			int fee_dtl_id;
			int addup_id;
			int subject_id;
			OSSDateTime cal_time;
			OSSDateTime update_time;
	};
}
#endif	// __IBS_USAGE_H__

