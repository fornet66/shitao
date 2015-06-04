
#ifndef __IBS_ADDUP_H__
#define __IBS_ADDUP_H__
#include "define.h"
#include "usage.h"
namespace ibs {
	class PGA;
	class Addup {
		friend class PGA;
		public:
			Addup()
				:acct_id(0),
				cycle_id(""),
				fee_rule_id(0),
				subject_id(0),
				addup_id(0),
				addup_value(0),
				unit_type(0),
				active_time(),
				inactive_time(),
				update_time(OSSDateTime::now())
			{ }
			~Addup() { }
			const string& cycleId() const {return cycle_id;}
			int addupId() const {return addup_id;}
			int64 addupValue() const {return addup_value;}
			bool isValid() const {return acct_id>0;}
			void makeaddup(const Usage& usage) {
				acct_id = usage.acct_id;
				cycle_id = usage.cycle_id;
				fee_rule_id = usage.fee_rule_id;
				subject_id = usage.subject_id;
				addup_id = usage.addup_id;
				addup_value = 1;
				unit_type = 0;
				active_time = OSSDateTime(OSSDate(usage.cycle_id+"01"),OSSTime("000000"));
				inactive_time = OSSDateTime(active_time.addMonths(1).addDays(-1).date(),OSSTime("235959"));
				update_time = OSSDateTime::now();
			}
		private:
			int acct_id;
			string cycle_id;
			int fee_rule_id;
			int subject_id;
			int addup_id;
			int64 addup_value;
			int unit_type;
			OSSDateTime active_time;
			OSSDateTime inactive_time;
			OSSDateTime update_time;
	};
}
#endif	// __IBS_ADDUP_H__

