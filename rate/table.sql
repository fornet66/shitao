
drop table acct_info;
CREATE TABLE ACCT_INFO  (
  ACCT_ID             number(9) NOT NULL ,
  ACCT_NAME           VARCHAR2(100),
  SALE_ID             number(9) NOT NULL ,
  SALES_NAME          VARCHAR2(100),
  PAY_TYPE            number(1),
  ACCT_SCORE          number(9),
  CONTACT_NUMBER      VARCHAR2(20),
  CREDIT_VALUE        number(9),
  CREDIT_RULE_ID      number(9),
  ACCT_STATUS         number(1) not null,
  FEE_RULE_ID         number(5) not null,
  OPER_ID             number(5),
  CREATE_TIME         date NOT NULL ,
  UPDATE_TIME         date NOT NULL ,
  PRIMARY KEY (ACCT_ID)
) ;
insert into acct_info values (88888,'testacct',11111,'testsale',1,0,'13012345678',0,0,'1',999,1,sysdate,sysdate);

drop table acc_addup_2014;
CREATE TABLE ACC_ADDUP_2014  (
  ACCT_ID             number(9) NOT NULL ,
  CYCLE_ID            CHAR(6) NOT NULL ,
  FEE_RULE_ID         number(9) NOT NULL ,
  SUBJECT_ID          number(6) NOT NULL ,
  ADDUP_ID            number(5) NOT NULL ,
  ADDUP_VALUE         number(15) not null,
  UNIT_TYPE           number(5),
  ACTIVE_TIME         date,
  INACTIVE_TIME       date,
  UPDATE_TIME         date,
  PRIMARY KEY (ACCT_ID,CYCLE_ID,FEE_RULE_ID,SUBJECT_ID,ADDUP_ID)
) ;

drop table acc_bill_2014;
CREATE TABLE ACC_BILL_2014  (
  BILL_ID             VARCHAR2(64) NOT NULL ,
  ACCT_ID             number(9) NOT NULL ,
  CYCLE_ID            CHAR(6) NOT NULL ,
  ITEM_ID             VARCHAR2(64) NOT NULL ,
  SUBJECT_ID          number(6) NOT NULL ,
  ORI_FEE             number(15) NOT NULL ,
  BALANCE             number(15) NOT NULL ,
  DISCNT_FEE          number(15) NOT NULL ,
  ADJUST_FEE          number(15) NOT NULL ,
  PAY_STATUS          CHAR(1) NOT NULL ,
  UPDATE_TIME         date NOT NULL ,
  OPERATOR            VARCHAR2(20) NOT NULL 
) ;

drop table acc_cdr_2014;
CREATE TABLE ACC_CDR_2014  (
  CDR_ID              varchar2(64) NOT NULL ,
  CYCLE_ID            CHAR(6) NOT NULL ,
  ACTION_TIME         date NOT NULL ,
  ACCT_ID             number(9) NOT NULL ,
  SALE_ID             number(9) NOT NULL ,
  USER_ID             number(9) NOT NULL ,
  ITEM_ID             number(9) NOT NULL ,
  ACTION              CHAR(2) NOT NULL ,
  ACTION_OBJECT       varchar2(8),
  USER_IP             varchar2(15),
  CLICK_URL           varchar2(256),
  ORI_FEE             number(15),
  DISCNT_FEE          number(15),
  REAL_FEE            number(15),
  FEE_RULE_ID         number(5),
  FEE_DTL_ID          number(5),
  ADDUP_ID            number(5),
  SUBJECT_ID          number(6),
  CAL_TIME            date,
  UPDATE_TIME         date,
  PRIMARY KEY (CDR_ID)
) ;


drop table acc_fee_rule;
CREATE TABLE ACC_FEE_RULE  (
  FEE_RULE_ID         number(5) NOT NULL ,
  FEE_RULE_DESC       VARCHAR2(256),
  FEE_RULE_TYPE       CHAR(1),
  EXEC_TYPE           number(2),
  VALID_FLAG           number(1),
  PRIMARY KEY (FEE_RULE_ID)
)
;
insert into acc_fee_rule values (999,'testrule',1,1,1);

drop table acc_fee_rule_dtl;
CREATE TABLE ACC_FEE_RULE_DTL  (
  FEE_RULE_ID         number(5) NOT NULL ,
  DTL_ID              number(5) NOT NULL ,
  DTL_PRIORITY        number(5) NOT NULL ,
  APPLY_OBJECT        number(5) NOT NULL ,
  REF_RULE            number(5) NOT NULL ,
  REF_OBJECT          VARCHAR2(128) NOT NULL ,
  ADDUP_RULE          number(5) NOT NULL ,
  ADDUP_ID            number(5) NOT NULL ,
  START_VALUE         number(15) NOT NULL ,
  END_VALUE           number(15) NOT NULL ,
  BASE_UNIT           number(5) NOT NULL ,
  UNIT_FEE            number(9) NOT NULL ,
  SUBJECT_ID          number(6) NOT NULL ,
  VALID_FLAG          number(1) NOT NULL ,
  PRIMARY KEY (FEE_RULE_ID,DTL_ID)
) ;

insert into acc_fee_rule_dtl values(999,1,1,1,1,'1111',0,0,0,0,0,5000,210100,1);
commit;

