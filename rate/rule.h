
#ifndef __IBS_RULE_H__
#define __IBS_RULE_H__
#include "define.h"
namespace ibs {
	class PGA;
	class Rule {
		friend class PGA;
		public:
			enum Apply_Object {
				Apply_CDR = 1,
				Apply_BILL = 2
			};
			enum Ref_Rule {
				Ref_ITEM = 1,
				Ref_IA = 2,
				Ref_IAO = 3,
				Ref_NONE = 9
			};
			enum Addup_Rule {
				Addup_Fee = 1,
				Addup_Count = 2
			};
			Rule()
				:fee_rule_id(0),
				dtl_id(0),
				dtl_priority(0),
				apply_object(0),
				ref_rule(0),
				ref_object(""),
				addup_rule(0),
				addup_id(0),
				start_value(0),
				end_value(0),
				base_unit(0),
				unit_fee(0),
				subject_id(0)
			{ }
			~Rule() { }
			int feeRuleId() const {return fee_rule_id;}
			int dtlId() const {return dtl_id;}
			int dtlPriority() const {return dtl_priority;}
			int applyObject() const {return apply_object;}
			int refRule() const {return ref_rule;}
			const string& refObject() const {return ref_object;}
			int addupRule() const {return addup_rule;}
			int addupId() const {return addup_id;}
			int64 startValue() const {return start_value;}
			int64 endValue() const {return end_value;}
			int baseUnit() const {return base_unit;}
			int unitFee() const {return unit_fee;}
			int subjectId() const {return subject_id;}
		private:
			int fee_rule_id;
			int dtl_id;
			int dtl_priority;
			int apply_object;
			int ref_rule;
			string ref_object;
			int addup_rule;
			int addup_id;
			int64 start_value;
			int64 end_value;
			int base_unit;
			int unit_fee;
			int subject_id;
	};
	typedef map<int, vector<Rule> > RuleMap;
}
#endif	// __IBS_RULE_H__

