/*==============================================================*/
/* DBMS name:      MySQL 5.0                                    */
/* Created on:     2015/3/7 15:01:54                            */
/*==============================================================*/

/*==============================================================*/
/* Table: app_log                                               */
/*==============================================================*/
drop table app_log;
create table app_log
(
   uuid           		char(36) comment '用于主键，取值格式调用系统uuid',
   country              varchar(10),
   city                 varchar(30),
   access_time          char(14),
   zone                 varchar(10) comment 'ngix访问时区，一般使用标准时区(东8区)',
   user_host            varchar(20) comment '用户访问ip，但有可能取到网关的地址',
   http_method          varchar(10) comment 'GET,POST,RESPONSE等http标准方法',
   http_protocol        varchar(10),
   http_status          varchar(5),
   body_bytes_sent      int,
   response_time        int comment '页面响应时间，以秒为单位',
   http_referer         varchar(256),
   http_user_agent      varchar(100),
   http_x_forwarded_for varchar(20) comment '若user_host取得是代理服务器或网关地址，此字段代表用户真实IP',
   php_name             varchar(30) comment '对于collect.php(收藏)，里面的id属性要根据type属性去对应响应的id
            1、节目 2、宝贝 3、剧照 4、web 5、投票 6、抽奖 7、店铺 8、介绍 9、话题 10、评论',
   user_pf              varchar(20) comment 'pf={android|iPhone|iPhone_corp|web}',
   uid                  varchar(30),
   did                  varchar(30),
   appversion           varchar(15),
   user_source          varchar(20) comment '此字段用于登录用户来源，如weibo,qq',
   cookie               varchar(64),
   token				varchar(64),
   list_from_id         int comment '用于记录列表显示起始位置ID，适用于：
            activity_list.php(活动列表)
            collect_list.php(收藏列表)
            comment_list.php(评论列表)
            main.php(首页)
            passed_program_list.php(热播列表)
            preview_program_list.php(预告列表)
            program_comment_list.php(节目评论列表)
            program_detail.php(节目详情)
            topic_comment_list.php(话题列表)
            ',
   list_count           int comment '和上一个字段配合，用户list列表中的显示条数；对统计而言无多大意义',
   main_type            int comment '视淘主类型：
            1、节目 2、宝贝 3、剧照 4、web 5、投票 6、抽奖 7、店铺 8、介绍 9、话题 10、评论',
   program_id           int comment 'program_comment_list.php(节目评论列表),
            program_intro.php(节目介绍),
            program_detail.php(节目详情),',
   sound_id             int,
   item_id              int comment 'item_detail.php',
   still_id             int comment 'still_detail.php',
   comment_id           int comment '评论id,适用于comment_option.php,comment_detail.php',
   follow_id            int comment '对应外部商品id guess_status.php',
   lottery_id           int comment 'lottery_detail.php(中奖) 
            lottery_draw.php(抽奖)
            lottery_share.php(中奖分享)
            lottery_share_reward.php(分享获得抽奖)
            lottery_winners.php(中奖信息) ',
   store_id             int comment 'store_detail.php',
   vote_id              int comment 'vote_detail.php(投票详情) vote_option_add.php(投票) vote_query.php(投票查询)',
   topic_id				int,
   prize_id             int,
   collect_type         varchar(5) comment 'collect.php的ask_status字段，取值{add|del}',
   comment_action       varchar(5) comment 'action={add|del}',
   comment_option       int comment '1-顶 2-踩',
   guess_price          numeric(8,2),
   expires_in           int,
   lottery_phone        varchar(15),
   lottery_email        varchar(64),
   lottery_addr         varchar(256),
   redirect_url         varchar(256)
);

alter table app_log comment 'shitao app访问日志';

/*==============================================================*/
/* Table: stat_app_user                                              */
/*==============================================================*/
drop table stat_app_user;
create table stat_app_user
(
   stat_date            char(8) comment '统计日期，格式yyyymmdd',
   stat_hour            char(2) comment '按小时统计：取值00~23',
   stat_min             char(2) comment '按分钟统计，取值00~59',
   stat_type            int comment '0:每分钟统计 1:每小时统计 2:每天统计 10:截至当前分钟统计 11:截止当前小时统计',
   user_pf              varchar(20) comment '主要分ios和andriod',
   appversion           varchar(12),
   user_source          varchar(60),
   new_users            int,
   active_users         int,
   startup_num          int,
   total_num            int,
   active_ratio         numeric(5,2) comment '活跃次数/启动次数，对应第6个报表，以日期为单位',
   active_user_ratio    numeric(8,2) comment '老用户数/活跃用户数，对应第6个报表，以日期为单位'
);

alter table stat_app_user comment '此表记录app按用户的使用情况 本表按时间维度精确到分钟，按大维度统计时需要对相应字段求和(sum)，累计用';

/*==============================================================*/
/* Table: stat_collect_total                                         */
/*==============================================================*/
drop table stat_collect_total;
create table stat_collect_total
(
   stat_date            char(8) comment '统计日期，格式yyyymmdd',
   stat_hour            char(2) comment '按小时统计：取值00~23',
   stat_min             char(2) comment '按分钟统计，取值00~59',
   stat_type            int comment '0:每分钟统计 1:每小时统计 2:每天统计 10:截至当前分钟统计 11:截止当前小时统计',
   user_pf              varchar(20) comment '主要分ios和andriod',
   login_users          int,
   collect_users        int,
   collect_ratio        numeric(6,2),
   collect_num          int,
   program_collect_num  int,
   still_collect_num    int,
   item_collect_num     int
);

alter table stat_collect_total comment '此表记录收藏整体情况 本表适用于统计数据要求：报表编号13';

/*==============================================================*/
/* Table: stat_collect_detail                                        */
/*==============================================================*/
drop table stat_collect_detail;
create table stat_collect_detail
(
   stat_date            char(8) comment '统计日期，格式yyyymmdd',
   stat_hour            char(2) comment '按小时统计：取值00~23',
   stat_min             char(2) comment '按分钟统计，取值00~59',
   stat_type            int comment '0:每分钟统计 1:每小时统计 2:每天统计 10:截至当前分钟统计 11:截止当前小时统计',
   user_pf              varchar(20) comment '主要分ios和andriod',
   program_index        int comment '节目',
   program_id           int comment '节目单集',
   program_type         int,
   play_style           int,
   play_channel         int,
   stat_class           int default 0 comment '此字段是为了便于区分统计类型，以减少统计表数量 1:剧照 2:宝贝',
   stat_id              int comment '和stat_class配合使用，根据stat_class确定是剧照id还是宝贝id',
   collect_num          int,
   collect_users        int
);

alter table stat_collect_detail comment '此表记录按节目收藏详情 本表适用于统计数据要求：报表编号13的第二部分，精确到具';

/*==============================================================*/
/* Table: stat_comment_total                                         */
/*==============================================================*/
drop table stat_comment_total;
create table stat_comment_total
(
   stat_date            char(8) comment '统计日期，格式yyyymmdd',
   stat_hour            char(2) comment '按小时统计：取值00~23',
   stat_min             char(2) comment '按分钟统计，取值00~59',
   stat_type            int comment '0:每分钟统计 1:每小时统计 2:每天统计 10:截至当前分钟统计 11:截止当前小时统计',
   user_pf              varchar(20) comment '主要分ios和andriod',
   comment_visit_users  int,
   comment_users        int,
   like_users           int,
   dislike_users        int,
   su_users             int,
   login_users          int,
   comment_ratio        numeric(6,2)
);

alter table stat_comment_total comment '此表记录评论整体情况 本表适用于统计数据要求：报表编号15第一部分';

/*==============================================================*/
/* Table: stat_comment_detail                                        */
/*==============================================================*/
drop table stat_comment_detail;
create table stat_comment_detail
(
   stat_date            char(8) comment '统计日期，格式yyyymmdd',
   stat_hour            char(2) comment '按小时统计：取值00~23',
   stat_min             char(2) comment '按分钟统计，取值00~59',
   stat_type            int comment '0:每分钟统计 1:每小时统计 2:每天统计 10:截至当前分钟统计 11:截止当前小时统计',
   user_pf              varchar(20) comment '主要分ios和andriod',
   program_index        int,
   program_id           int,
   program_type         int,
   play_style           int,
   play_channel         int,
   comment_visit_users  int,
   comment_users        int,
   like_users           int,
   dislike_users        int,
   su_users             int
);

alter table stat_comment_detail comment '此表记录按节目评论详情 本表适用于统计数据要求：报表编号15的第二部分';

/*==============================================================*/
/* Table: stat_login                                          */
/*==============================================================*/
drop table stat_login;
create table stat_login
(
   stat_date            char(8) comment '统计日期，格式yyyymmdd',
   stat_hour            char(2) comment '按小时统计：取值00~23',
   stat_min             char(2) comment '按小时统计：取值00~59',
   stat_type            int comment '0:每分钟统计 1:每小时统计 2:每天统计 10:截至当前分钟统计 11:截止当前小时统计',
   user_pf              varchar(20) comment '主要分ios和andriod',
   active_num           int,
   login_num            int,
   login_ratio          numeric(6,2)
);

alter table stat_login comment '此表记录登录情况 本表适用于统计数据要求：报表编号12';

/*==============================================================*/
/* Table: stat_main_visit                                            */
/*==============================================================*/
drop table stat_main_visit;
create table stat_main_visit
(
   stat_date            char(8) comment '统计日期，格式yyyymmdd',
   stat_hour            char(2) comment '按小时统计：取值00~23',
   stat_min             char(2) comment '按分钟统计，取值00~59',
   stat_type            int comment '0:每分钟统计 1:每小时统计 2:每天统计 10:截至当前分钟统计 11:截止当前小时统计',
   user_pf              varchar(20) comment '主要分ios和andriod',
   appversion           varchar(12),
   page_type            int comment '0：一级界面 1：二级界面',
   page_name            varchar(60),
   visit_num            int,
   visit_users          int
);

alter table stat_main_visit comment '此表记录app主页面的访问情况 本表按时间维度精确到分钟，按大维度统计时需要对相应字段求和(sum)，累计用';

/*==============================================================*/
/* Table: stat_visit_detail                                          */
/*==============================================================*/
drop table stat_visit_detail;
create table stat_visit_detail
(
   stat_date            char(8) comment '统计日期，格式yyyymmdd',
   stat_hour            char(2) comment '按小时统计：取值00~23',
   stat_min             char(2) comment '按分钟统计，取值00~59',
   stat_type            int comment '0:每分钟统计 1:每小时统计 2:每天统计 10:截至当前分钟统计 11:截止当前小时统计',
   user_pf              varchar(20) comment '主要分ios和andriod',
   program_index        int,
   program_id           int,
   program_type         int,
   play_style           int,
   play_channel         int,
   stat_class           int comment '区分统计类别减少统计表数量：0: 剧照,1:宝贝,2:第三方宝贝 此字段决定stat_id的具体含义',
   stat_id              int comment '和stat_class配合使用，根据stat_class确定是剧照id还是宝贝id',
   visit_num            int,
   visit_users          int,
   order_num            int,
   order_users          int,
   order_money          numeric(8,2),
   pay_num              int,
   pay_users            int,
   pay_money            numeric(8,2)
);

alter table stat_visit_detail comment '此表记录app二级页面按节目访问详情本表适用于统计数据要求：报表编号10的第三';

/*==============================================================*/
/* Table: stat_scan_user                                             */
/*==============================================================*/
drop table stat_scan_user;
create table stat_scan_user
(
   stat_date            char(8) comment '统计日期，格式yyyymmdd',
   stat_hour            char(2) comment '按小时统计：取值00~23',
   stat_min             char(2) comment '按分钟统计，取值00~59',
   stat_type            int comment '0:每分钟统计 1:每小时统计 2:每天统计 10:截至当前分钟统计 11:截止当前小时统计',
   scan_num             int,
   scan_users           int,
   active_users         int,
   user_ratio           numeric(5,2) comment '扫描用户数/活跃用户数'
);

alter table stat_scan_user comment '此表记录使用scan功能的用户情况 本表适用于统计数据要求：报表编号8的第一部分(不按节目和剧集计算的用户数';

/*==============================================================*/
/* Table: stat_scan_program_user                                        */
/*==============================================================*/
drop table stat_scan_program_user;
create table stat_scan_program_user
(
   stat_date            char(8) comment '统计日期，格式yyyymmdd',
   stat_hour            char(2) comment '按小时统计：取值00~23',
   stat_min             char(2) comment '按分钟统计，取值00~59',
   stat_type            int comment '0:每分钟统计 1:每小时统计 2:每天统计 10:截至当前分钟统计 11:截止当前小时统计',
   user_pf              varchar(20) comment '主要分ios和andriod',
   program_index        int,
   program_id           int,
   program_type         int,
   play_style           int,
   play_channel         int,
   scan_num             int,
   scan_users           int,
   active_users         int,
   user_ratio           numeric(5,2) comment '扫描用户数/活跃用户数'
);

alter table stat_scan_program_user comment '此表记录app 找一找功能的使用情况 本表适用于统计数据要求：报表编号8的第二部分';

/*==============================================================*/
/* Table: stat_vote_total                                            */
/*==============================================================*/
drop table stat_vote_total;
create table stat_vote_total
(
   stat_date                 char(8) comment '统计日期，格式yyyymmdd',
   stat_hour                 char(2) comment '按小时统计：取值00~23',
   stat_min                  char(2) comment '按分钟统计，取值00~59',
   stat_type                 int comment '0:每分钟统计 1:每小时统计 2:每天统计 10:截至当前分钟统计 11:截止当前小时统计',
   login_users               int,
   vote_id                   int,
   vote_visit_users          int,
   vote_users                int,
   vote_ratio                numeric(6,2),
   vote_comment_visit_users  int,
   vote_comment_users        int,
   vote_comment_visit_ratio  numeric(6,2),
   vote_comment_ratio        numeric(6,2)
);

alter table stat_vote_total comment '此表记录投票整体情况 本表适用于统计数据要求：报表编号14';









/*==============================================================*/
/* Table: stat_lottery_total                                         */
/*==============================================================*/
drop table stat_lottery_total;
create table stat_lottery_total
(
   stat_date            char(8) comment '统计日期，格式yyyymmdd',
   stat_hour            char(2) comment '按小时统计：取值00~23',
   stat_min             char(2) comment '按分钟统计，取值00~59',
   stat_type            int comment '0:每分钟统计 1:每小时统计 2:每天统计 10:截至当前分钟统计 11:截止当前小时统计',
   user_pf              varchar(20) comment '主要分ios和andriod',
   user_source          varchar(20),
   lottery_id           int,
   visit_num            int,
   visit_users          int,
   lottery_num          int,
   lottery_users        int,
   get_users            int,
   exchange_users       int
);

alter table stat_lottery_total comment '此表记录抽奖整体情况 本表适用于统计数据要求：报表编号16';

/*==============================================================*/
/* Table: stat_download_program_user                                    */
/*==============================================================*/
drop table stat_download_program_user;
create table stat_download_program_user
(
   stat_date            char(8) comment '统计日期，格式yyyymmdd',
   stat_hour            char(2) comment '按小时统计：取值00~23',
   stat_min             char(2) comment '按分钟统计，取值00~59',
   stat_type            int comment '0:每分钟统计 1:每小时统计 2:每天统计 10:截至当前分钟统计 11:截止当前小时统计',
   user_pf              varchar(20) comment '主要分ios和andriod',
   user_source          varchar(60),
   program_index        int,
   program_id           int,
   program_type         int,
   play_style           int,
   play_channel         int,
   scan_num             int,
   download_users       int
);

alter table stat_download_program_user comment '此表记录app扫描下载的用户情况 本表适用于统计数据要求：报表编号7的第二部分';

/*==============================================================*/
/* Table: stat_download_user                                         */
/*==============================================================*/
drop table stat_download_user;
create table stat_download_user
(
   stat_date            char(8) comment '统计日期，格式yyyymmdd',
   stat_hour            char(2) comment '按小时统计：取值00~23',
   stat_min             char(2) comment '按分钟统计，取值00~59',
   stat_type            int comment '0:每分钟统计 1:每小时统计 2:每天统计 10:截至当前分钟统计 11:截止当前小时统计',
   scan_num             int,
   download_num         int,
   success_num          int,
   scan_users           int,
   click_users          int,
   download_users       int
);

alter table stat_download_user comment '此表记录app扫描下载的用户情况 本表适用于统计数据要求：报表编号7的第一部分(不按节目和剧集计算的用户数)';

/*==============================================================*/
/* Table: stat_program_user                                          */
/*==============================================================*/
drop table stat_program_user;
create table stat_program_user
(
   stat_date            char(8) comment '统计日期，格式yyyymmdd',
   stat_hour            char(2) comment '按小时统计：取值00~23',
   stat_min             char(2) comment '按分钟统计，取值00~59',
   stat_type            int comment '0:每分钟统计 1:每小时统计 2:每天统计 10:截至当前分钟统计 11:截止当前小时统计',
   user_pf              varchar(20) comment '主要分ios和andriod',
   appversion           varchar(12),
   user_source          varchar(60),
   program_index        int,
   program_id           int,
   program_type         int,
   play_style           int,
   play_channel         int,
   new_users            int,
   active_users         int,
   startup_num          int,
   total_num            int,
   active_ratio         numeric(5,2) comment '活跃次数/启动次数，对应第6个报表，以日期为单位',
   active_user_ratio    numeric(8,2) comment '老用户数/活跃用户数，对应第6个报表，以日期为单位'
);

alter table stat_program_user comment '此表记录app按节目的使用情况 本表按时间维度精确到分钟，按大维度统计时需要对相应字段求和(sum)，累计用';

/*==============================================================*/
/* Table: stat_share_total                                           */
/*==============================================================*/
drop table stat_share_total;
create table stat_share_total
(
   stat_date            char(8) comment '统计日期，格式yyyymmdd',
   stat_hour            char(2) comment '按小时统计：取值00~23',
   stat_min             char(2) comment '按分钟统计，取值00~59',
   stat_type            int comment '0:每分钟统计 1:每小时统计 2:每天统计 10:截至当前分钟统计 11:截止当前小时统计',
   user_pf              varchar(20) comment '主要分ios和andriod',
   click_num            int,
   share_channel        varchar(60),
   share_num            int,
   back_num             int,
   back_ratio           numeric(6,2)
);

alter table stat_share_total comment '此表记录分享整体情况 本表适用于统计数据要求：报表编号11';

/*==============================================================*/
/* Table: stat_share_detail                                          */
/*==============================================================*/
drop table stat_share_detail;
create table stat_share_detail
(
   stat_date            char(8) comment '统计日期，格式yyyymmdd',
   stat_hour            char(2) comment '按小时统计：取值00~23',
   stat_min             char(2) comment '按分钟统计，取值00~59',
   stat_type            int comment '0:每分钟统计 1:每小时统计 2:每天统计 10:截至当前分钟统计 11:截止当前小时统计',
   user_pf              varchar(20) comment '主要分ios和andriod',
   stat_type1           smallint default 0 comment '此字段是为了便于区分统计类型，以减少统计表数量
            0: 整体情况
            1:按分享页面统计 
            2:第三方宝贝',
   share_id             int,
   share_name           varchar(60),
   share_num            int,
   back_num             int,
   back_ratio           numeric(6,2)
);

alter table stat_share_detail comment '此表记录分享情况 本表适用于统计数据要求：报表编号11';

/*==============================================================*/
/* Table: stat_visit_program_total                                      */
/*==============================================================*/
drop table stat_visit_program_total;
create table stat_visit_program_total
(
   stat_date            char(8) comment '统计日期，格式yyyymmdd
            ',
   stat_hour            char(2) comment '按小时统计：取值00~23',
   stat_min             char(2) comment '按分钟统计，取值00~59',
   stat_type            int comment '0:每分钟统计 1:每小时统计 2:每天统计 10:截至当前分钟统计 11:截止当前小时统计',
   user_pf              varchar(20) comment '主要分ios和andriod',
   stat_type1           smallint default 0 comment '此字段是为了便于区分统计类型，以减少统计表数量
            缺省为0，表示只计算访问总量
            1，表示需要区分剧照,剧集等',
   program_id           int,
   program_item            int,
   program_type         smallint,
   play_style           smallint,
   play_channel         smallint,
   prog_detail_num      int,
   prog_detail_users    int,
   still_num            int,
   still_user           int,
   item_detail_num      int,
   item_detail_user     int,
   refer_num            int,
   refer_user           int,
   total_num            int,
   total_user           int
);

alter table stat_visit_program_total comment '此表记录app二级页面的综合访问情况 本表按时间维度精确到分钟，按大维度统计时需要对相应字段求和(sum)，';

