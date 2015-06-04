
#include <fstream>
#include "pga.h"
namespace ibs {
	PGA::PGA(const string& homePath)
		:conn_(0),
		stop_(0),
		yyyy(0),
		homePath_(homePath),
		acct_stat(0),
		addup_stat_s(0),
		addup_stat_i(0),
		addup_stat_u(0),
		bill_stat_s(0),
		bill_stat_i(0),
		bill_stat_u(0),
		usage_stat(0)
	{ }

	PGA::~PGA() {
		delete acct_stat; acct_stat=0;
		delete addup_stat_s; addup_stat_s=0;
		delete addup_stat_i; addup_stat_i=0;
		delete addup_stat_u; addup_stat_u=0;
		delete bill_stat_s; bill_stat_s=0;
		delete bill_stat_i; bill_stat_i=0;
		delete bill_stat_u; bill_stat_u=0;
		delete usage_stat; usage_stat=0;
		stop_connection("IBSDB");
	}

	int PGA::init() {
		conn_ = start_connection("IBSDB");
		if(conn_==NULL) {
			OSS_DEBUG((LM_ERROR,"database connection error\n"));
			return -1;
		}
		Statement stat = conn_->createStatement();
		stat.prepare("select a.fee_rule_id,b.dtl_id,b.dtl_priority,"
				"b.apply_object,b.ref_rule,b.ref_object,b.addup_rule,b.addup_id,"
				"b.start_value,b.end_value,b.base_unit,b.unit_fee,b.subject_id "
				"from acc_fee_rule a, acc_fee_rule_dtl b "
				"where a.fee_rule_id=b.fee_rule_id and a.valid_flag=1 and b.valid_flag=1 "
				"and b.apply_object=1 order by dtl_priority asc");
		stat.clearParams();
		stat.execute();
		while(stat.next()) {
			Rule rule;
			stat>>rule.fee_rule_id>>rule.dtl_id>>rule.dtl_priority
				>>rule.apply_object>>rule.ref_rule>>rule.ref_object>>rule.addup_rule>>rule.addup_id
				>>rule.start_value>>rule.end_value>>rule.base_unit>>rule.unit_fee>>rule.subject_id;
			cdrRuleMap_[rule.fee_rule_id].push_back(rule);
		}
		ACE_DEBUG((LM_INFO, "load [%d] acc_fee_rule cdr data ...\n", cdrRuleMap_.size()));
		stat.prepare("select a.fee_rule_id,b.dtl_id,b.dtl_priority,"
				"b.apply_object,b.ref_rule,b.ref_object,b.addup_rule,b.addup_id,"
				"b.start_value,b.end_value,b.base_unit,b.unit_fee,b.subject_id "
				"from acc_fee_rule a, acc_fee_rule_dtl b "
				"where a.fee_rule_id=b.fee_rule_id and a.valid_flag=1 and b.valid_flag=1 "
				"and b.apply_object=2 order by dtl_priority asc");
		stat.clearParams();
		stat.execute();
		while(stat.next()) {
			Rule rule;
			stat>>rule.fee_rule_id>>rule.dtl_id>>rule.dtl_priority
				>>rule.apply_object>>rule.ref_rule>>rule.ref_object>>rule.addup_rule>>rule.addup_id
				>>rule.start_value>>rule.end_value>>rule.base_unit>>rule.unit_fee>>rule.subject_id;
			billRuleMap_[rule.fee_rule_id].push_back(rule);
		}
		ACE_DEBUG((LM_INFO, "load [%d] acc_fee_rule bill data ...\n", billRuleMap_.size()));
		file_.init(homePath_);
		return 0;
	}

	int PGA::re_init(int in_yyyy) {
		if(yyyy!=in_yyyy) {
			yyyy=in_yyyy;
			char sql[1024];
			delete addup_stat_s;
			addup_stat_s = new Statement();
			*addup_stat_s = conn_->createStatement();
			snprintf(sql, sizeof(sql), "select acct_id,cycle_id,fee_rule_id,subject_id,addup_id,addup_value,"
					"unit_type,active_time,inactive_time,update_time "
					"from acc_addup_%d where acct_id=:f1 and fee_rule_id=:f2 and "
					"(cycle_id=:f3 or cycle_id='000000')", yyyy);
			addup_stat_s->prepare(sql);
			delete addup_stat_u;
			addup_stat_u = new Statement();
			*addup_stat_u = conn_->createStatement();
			snprintf(sql, sizeof(sql), "update acc_addup_%d set addup_value=addup_value+:f1,update_time=:f2 "
					"where acct_id=:f3 and cycle_id=:f4 and fee_rule_id=:f5 and addup_id=:f6", yyyy);
			addup_stat_u->prepare(sql);
			delete addup_stat_i;
			addup_stat_i = new Statement();
			*addup_stat_i = conn_->createStatement();
			snprintf(sql, sizeof(sql), "insert into acc_addup_%d (acct_id,cycle_id,fee_rule_id,subject_id,addup_id,"
					"addup_value,unit_type,active_time,inactive_time,update_time) values "
					"(:f1,:f2,:f3,:f4,:f5,:f6,:f7,:f8,:f9,:f10)",yyyy);
			addup_stat_i->prepare(sql);

			delete bill_stat_s;
			bill_stat_s = new Statement();
			*bill_stat_s = conn_->createStatement();
			snprintf(sql, sizeof(sql), "select bill_id,acct_id,cycle_id,item_id,subject_id,ori_fee,balance,"
					"discnt_fee,adjust_fee,pay_status,update_time "
					"from acc_bill_%d "
					"where acct_id=:f1 and cycle_id=:f2", yyyy);
			ACE_DEBUG((LM_DEBUG,"sql = %s\n",sql));
			bill_stat_s->prepare(sql);
			delete bill_stat_u;
			bill_stat_u = new Statement();
			*bill_stat_u = conn_->createStatement();
			snprintf(sql, sizeof(sql), "update acc_bill_%d set ori_fee=ori_fee+:f1,balance=balance+:f2,"
					"discnt_fee=discnt_fee+:f3,update_time=:f4 "
					"where acct_id=:f5 and cycle_id=:f6 and item_id=:f7 and subject_id=:f8", yyyy);
			bill_stat_u->prepare(sql);
			delete bill_stat_i;
			bill_stat_i = new Statement();
			*bill_stat_i = conn_->createStatement();
			snprintf(sql, sizeof(sql), "insert into acc_bill_%d (bill_id,acct_id,cycle_id,item_id,subject_id,"
					"ori_fee,balance,discnt_fee,adjust_fee,pay_status,update_time,operator) values "
					"(:f1,:f2,:f3,:f4,:f5,:f6,:f7,:f8,:f9,:f10,:f11,'SYS')", yyyy);
			bill_stat_i->prepare(sql);

			delete usage_stat;
			usage_stat = new Statement();
			*usage_stat = conn_->createStatement();
			snprintf(sql, sizeof(sql), "insert into acc_cdr_%d (cdr_id,cycle_id,action_time,acct_id,sale_id,"
					"user_id,item_id,action,action_object,user_ip,click_url,ori_fee,discnt_fee,real_fee,"
					"fee_rule_id,fee_dtl_id,addup_id,subject_id,cal_time,update_time) values "
					"(:f1,:f2,:f3,:f4,:f5,:f6,:f7,:f8,:f9,:f10,:f11,:f12,:f13,:f14,:f15,:f16,:f17,:f18,:f19,:f20)");
			usage_stat->prepare(sql);
		}
		return 0;
	}

	int PGA::getAccount(int sid, Account& account) {
		if(acct_stat == 0) {
			acct_stat = new Statement();
			*acct_stat = conn_->createStatement();
			acct_stat->prepare("select acct_id,sale_id,pay_type,contact_number,acct_status,fee_rule_id,create_time "
					"from acct_info where sale_id=:f1 and acct_status='1'");
		}
		acct_stat->clearParams();
		*acct_stat<<sid;
		acct_stat->execute();
		while(acct_stat->next()) {
			*acct_stat>>account.acct_id>>account.sale_id>>account.pay_type>>account.contact_number
				>>account.acct_status>>account.fee_rule_id>>account.create_time;
		}
		return account.acct_id;
	}

	int PGA::getAddup(int aid, int rule, const string& cycle, map<string,Addup>& addupMap) {
		addupMap.clear();
		addup_stat_s->clearParams();
		*addup_stat_s<<aid<<rule<<cycle;
		addup_stat_s->execute();
		while(addup_stat_s->next()) {
			Addup addup;
			*addup_stat_s>>addup.acct_id>>addup.cycle_id>>addup.fee_rule_id>>addup.subject_id>>addup.addup_id
				>>addup.addup_value>>addup.unit_type>>addup.active_time>>addup.inactive_time>>addup.update_time;
			char key[16];
			snprintf(key, sizeof(key), "%s-%d", addup.cycleId().c_str(), addup.addupId());
			addupMap.insert(map<string,Addup>::value_type(string(key), addup));
		}
		return addupMap.size();
	}

	int PGA::updateAddup(const Addup& addup) {
		if(addup.isValid()==false) return 0;
		addup_stat_u->clearParams();
		*addup_stat_u<<addup.addup_value<<addup.update_time<<addup.acct_id
			<<addup.cycle_id<<addup.fee_rule_id<<addup.addup_id;
		int r = addup_stat_u->execute();
		if(r==0) {
			addup_stat_i->clearParams();
			*addup_stat_i<<addup.acct_id<<addup.cycle_id<<addup.fee_rule_id<<addup.subject_id<<addup.addup_id
				<<addup.addup_value<<addup.unit_type<<addup.active_time<<addup.inactive_time<<addup.update_time;
			r = addup_stat_i->execute();
		}
		return r;
	}

	int PGA::getBill(int aid, const string& cycle, vector<Bill>& billVec) {
		billVec.clear();
	}

	int PGA::updateBill(const Bill& bill) {
		if(bill.isValid()==false) return 0;
		bill_stat_u->clearParams();
		*bill_stat_u<<bill.ori_fee<<bill.balance<<bill.discnt_fee<<bill.update_time
			<<bill.acct_id<<bill.cycle_id<<bill.item_id<<bill.subject_id;
		int r = bill_stat_u->execute();
		if(r == 0) {
			bill_stat_i->clearParams();
			*bill_stat_i<<bill.bill_id<<bill.acct_id<<bill.cycle_id<<bill.item_id<<bill.subject_id
				<<bill.ori_fee<<bill.balance<<bill.discnt_fee<<bill.adjust_fee<<bill.pay_status<<bill.update_time;
			r = bill_stat_i->execute();
		}
		return r;
	}

	int PGA::insertUsage(const Usage& usage) {
		usage_stat->clearParams();
		*usage_stat<<usage.uuid<<usage.cycle_id<<usage.action_time<<usage.acct_id<<usage.sale_id
			<<usage.user_id<<usage.item_id<<usage.action<<usage.action_object<<usage.user_ip
			<<usage.click_url<<usage.ori_fee<<usage.discnt_fee
			<<usage.real_fee<<usage.fee_rule_id<<usage.fee_dtl_id<<usage.addup_id<<usage.subject_id
			<<usage.cal_time<<usage.update_time;
		return usage_stat->execute();
	}

	int PGA::calculate(Usage& usage) {
		int r = -1;
		usage.maketime();
		usage.uuid = get_uuid_str();
		usage.cal_time = OSSDateTime::now();
		usage.update_time = OSSDateTime::now();
		usage.cycle_id = usage.virtual_action_time.toString("%Y%m");
		re_init(usage.virtual_action_time.year());
		// get account
		Account account;
		getAccount(usage.sale_id, account);
		usage.acct_id = account.acctId();
		usage.fee_rule_id = account.feeRuleId();
		//map<string, Bill> billMap;
		//getBill(usage.acct_id, billMap);
		vector<Rule> cdrrules;
		RuleMap::const_iterator cdrit = cdrRuleMap_.find(usage.fee_rule_id);
		if(cdrit==cdrRuleMap_.end()) {
			usage.fee_rule_id = 0;
			cdrit = cdrRuleMap_.find(0);
		}
		map<string, Addup> addupMap;
		if(cdrit!=cdrRuleMap_.end()) {
			getAddup(usage.acct_id, usage.fee_rule_id, usage.cycle_id, addupMap);
			cdrrules = cdrit->second;
		}
		ACE_DEBUG((LM_DEBUG,"uuid    (%s)\n",usage.uuid.c_str()));
		ACE_DEBUG((LM_DEBUG,"acct    (%d)\n",usage.acct_id));
		ACE_DEBUG((LM_DEBUG,"rule    (%d)\n",usage.fee_rule_id));
		ACE_DEBUG((LM_DEBUG,"cycle   (%s)\n",usage.cycle_id.c_str()));
		ACE_DEBUG((LM_DEBUG,"virtual (%s)\n",usage.virtual_action_time.toString().c_str()));
		ACE_DEBUG((LM_DEBUG,"cal     (%s)\n",usage.cal_time.toString().c_str()));
		ACE_DEBUG((LM_DEBUG,"update  (%s)\n",usage.update_time.toString().c_str()));
		for(size_t i=0; i<cdrrules.size(); ++i) {
			Bill bill_todo;
			Addup addup_todo;
			const Rule& rule = cdrrules[i];
			if((rule.refRule()==Rule::Ref_ITEM && rule.refObject()==usage.item())
					|| (rule.refRule()==Rule::Ref_IA && rule.refObject()==usage.ia())
					|| (rule.refRule()==Rule::Ref_IAO && rule.refObject()==usage.iao())
					|| (rule.refRule()==Rule::Ref_NONE)) {
				if(rule.addupRule()==Rule::Addup_Count) {
					Addup ref;
					char addup_id[16];
					snprintf(addup_id, sizeof(addup_id), "%s-%d", usage.cycle_id.c_str(), rule.addupId());
					map<string, Addup>::const_iterator it = addupMap.find(string(addup_id));
					if(it != addupMap.end()) {
						ref = it->second;
					}
					if(ref.addupValue()>=rule.startValue()&&ref.addupValue()<rule.endValue()) {
						usage.makerule(rule);
						addup_todo.makeaddup(usage);
						bill_todo.makebill(usage);
					}
				}
				else {
					usage.makerule(rule);
					bill_todo.makebill(usage);
				}
				ACE_DEBUG((LM_DEBUG,"dtl     (%d)\n",usage.fee_dtl_id));
				ACE_DEBUG((LM_DEBUG,"subject (%d)\n",usage.subject_id));
				ACE_DEBUG((LM_DEBUG,"ori     (%d)\n",usage.ori_fee));
				ACE_DEBUG((LM_DEBUG,"discnt  (%d)\n",usage.discnt_fee));
				ACE_DEBUG((LM_DEBUG,"real    (%d)\n",usage.real_fee));
				updateAddup(addup_todo);
				updateBill(bill_todo);
				insertUsage(usage);
				r = 0;
				break;
			}
		}
		vector<Rule> billrules;
		RuleMap::const_iterator billit = billRuleMap_.find(usage.fee_rule_id);
		if(billit==billRuleMap_.end()) billit = billRuleMap_.find(0);
		if(billit!=billRuleMap_.end()) billrules = billit->second;
		for(size_t i=0; i<billrules.size(); ++i) {
			// TODO
		}
		return r;
	}

	int PGA::start() {
		if(this->init()!=0) return -1;
		while(true) {
			if(stop_==1) {
				ACE_DEBUG((LM_INFO, "manual stopping ibs\n"));
				break;
			}
			while(file_.lock("useraction.*cdr")) {
				int CDR_MAX_LENGTH=2048;
				char buf[CDR_MAX_LENGTH];
				ifstream tmp;
				ofstream out;
				ofstream err;
				string tmpfile = file_.tmpFile();
				string outfile = file_.outFile();
				string errfile = file_.errFile();
				tmp.open(tmpfile.c_str());
				while(tmp.getline(buf, CDR_MAX_LENGTH)) {
					vector<string> columns;
					split(string(buf), columns);
					if(columns.size()!=8) {
						if(!err.is_open()) {
							err.clear();
							err.open(errfile.c_str());
						}
						err << string(buf) << endl;
						continue;
					}
					Usage usg;
					usg.action_time = OSSDateTime(columns[0]);
					usg.user_id = atoi(columns[1].c_str());
					usg.item_id = atoi(columns[2].c_str());
					usg.sale_id = atoi(columns[3].c_str());
					usg.action = columns[4];
					usg.action_object = columns[5];
					usg.user_ip = columns[6];
					usg.click_url = columns[7];
					ACE_DEBUG((LM_DEBUG,"time    (%s)\n",usg.action_time.toString().c_str()));
					ACE_DEBUG((LM_DEBUG,"user    (%d)\n",usg.user_id));
					ACE_DEBUG((LM_DEBUG,"item    (%d)\n",usg.item_id));
					ACE_DEBUG((LM_DEBUG,"sale    (%d)\n",usg.sale_id));
					ACE_DEBUG((LM_DEBUG,"action  (%s)\n",usg.action.c_str()));
					ACE_DEBUG((LM_DEBUG,"object  (%s)\n",usg.action_object.c_str()));
					ACE_DEBUG((LM_DEBUG,"ip      (%s)\n",usg.user_ip.c_str()));
					ACE_DEBUG((LM_DEBUG,"url     (%s)\n",usg.click_url.c_str()));
					if(calculate(usg) == 0) {
						conn_->commit();
						if(!out.is_open()) {
							out.clear();
							out.open(outfile.c_str());
						}
						out << usg.toString() << endl;
					}
					else {
						if(!err.is_open()) {
							err.clear();
							err.open(errfile.c_str());
						}
						err << string(buf) << endl;
					}
				}
				if(tmp.is_open()) tmp.close();
				if(out.is_open()) out.close();
				if(err.is_open()) err.close();
				file_.unlock();
			}
			ACE_DEBUG((LM_INFO,"there is no file to do, sleep 3s ...\n"));
			sleep(3);
		}
		return 0;
	}

	int PGA::stop() {
		stop_ = 1;
		return 0;
	}
}

