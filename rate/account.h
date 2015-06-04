
#ifndef __IBS_ACCOUNT_H__
#define __IBS_ACCOUNT_H__
#include "define.h"
namespace ibs {
	class PGA;
	class Account {
		friend class PGA;
		public:
			Account()
				:acct_id(0),
				sale_id(0),
				pay_type(0),
				contact_number(""),
				acct_status(0),
				fee_rule_id(0),
				create_time()
			{ }
			~Account() { }
			int acctId() const {return acct_id;}
			int saleId() const {return sale_id;}
			int payType() const {return pay_type;}
			const string& contactNumber() const {return contact_number;}
			int acctStatus() const {return acct_status;}
			int feeRuleId() const {return fee_rule_id;}
			const OSSDateTime& createTime() const {return create_time;}
		private:
			int acct_id;
			int sale_id;
			int pay_type;
			string contact_number;
			int acct_status;
			int fee_rule_id;
			OSSDateTime create_time;
	};
}
#endif	// __IBS_ACCOUNT_H__

