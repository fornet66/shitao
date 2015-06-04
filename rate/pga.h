
#ifndef __IBS_PGA_H__
#define __IBS_PGA_H__
#include "rule.h"
#include "account.h"
#include "addup.h"
#include "bill.h"
#include "usage.h"
#include "file.h"
namespace ibs {
	class PGA {
		public:
			PGA(const string& homePath);
			~PGA();
			int start();
			int stop();
		private:
			int init();
			int re_init(int in_yyyy);
			int getAccount(int sid, Account& account);
			int getAddup(int aid, int rule, const string& cycle, map<string,Addup>& addupMap);
			int updateAddup(const Addup& addup);
			int getBill(int aid, const string& cycle, vector<Bill>& billVec);
			int updateBill(const Bill& bill);
			int insertUsage(const Usage& usage);
			int calculate(Usage& usage);
		private:
			Connection* conn_;
			int stop_;
			int yyyy;
			string homePath_;
			Statement* acct_stat;
			Statement* addup_stat_s;
			Statement* addup_stat_i;
			Statement* addup_stat_u;
			Statement* bill_stat_s;
			Statement* bill_stat_i;
			Statement* bill_stat_u;
			Statement* usage_stat;
			RuleMap cdrRuleMap_;
			RuleMap billRuleMap_;
			file file_;
	};
}
#endif	// __IBS_PGA_H__

