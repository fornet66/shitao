
#ifndef __IBS_BILL_H__
#define __IBS_BILL_H__
#include <aiuuid/aiuuid.h>
#include "define.h"
namespace ibs {
	class PGA;
	class Bill {
		friend class PGA;
		public:
			Bill()
				:bill_id(""),
				acct_id(0),
				cycle_id(""),
				item_id(0),
				subject_id(0),
				ori_fee(0),
				balance(0),
				discnt_fee(0),
				adjust_fee(0),
				pay_status(0),
				update_time(OSSDateTime::now())
			{ }
			~Bill() { }
			bool isValid() const {return acct_id>0;}
			void makebill(const Usage& usage) {
				bill_id = get_uuid_str();
				acct_id = usage.acct_id;
				cycle_id = usage.cycle_id;
				item_id = usage.item_id;
				subject_id = usage.subject_id;
				ori_fee = usage.ori_fee;
				balance = usage.real_fee;
				discnt_fee = usage.discnt_fee;
				adjust_fee = 0;
				update_time = OSSDateTime::now();
			}
		private:
			string bill_id;
			int acct_id;
			string cycle_id;
			int item_id;
			int subject_id;
			int64 ori_fee;
			int64 balance;
			int64 discnt_fee;
			int64 adjust_fee;
			int pay_status;
			OSSDateTime update_time;
	};
}
#endif	// __IBS_BILL_H__

