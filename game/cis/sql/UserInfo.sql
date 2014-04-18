drop database if exists yb_cis;
create database if not exists yb_cis;

use yb_cis;
############################
#
# �˺���Ϣ������
#
############################

# �����
create table tbl_activity_type
(
 at_ActiveID int unsigned not null,  # �ID
 at_AwardItem varchar(128) not null, # ������Ʒ
 at_AwardType int unsigned not null, # ��������
 at_MutexID int unsigned not null,   # ����ID
 at_StartTime datetime not null,     # ��ʼʱ��
 at_EndTime datetime not null,       # ����ʱ��

 primary key(at_ActiveID)
 )engine=innodb DEFAULT CHARSET=latin1;

# �û��˺���Ϣ
create table tbl_user_info
(
 ui_UserID int unsigned not null,    # �˺�ID
 at_ActiveID int unsigned not null,  # �ID

 foreign key(at_ActiveID) references tbl_activity_type(at_ActiveID) on update cascade
 )engine=innodb DEFAULT CHARSET=latin1;

# ������ȡ��ʷ
create table tbl_award_history
(
 ah_HistoryID bigint unsigned not null auto_increment, # ������ȡ��ʷID
 ui_UserID int unsigned not null,                      # �˺�ID
 ah_ServerID int unsigned not null,                    # �Ѿ���ȡ�˽�Ʒ�ķ�����ID

 primary key(ah_HistoryID)
 )engine=innodb DEFAULT CHARSET=latin1;


create index idx_History on tbl_award_history(ui_UserID, ah_ServerID);



create table tbl_user_static
(
	us_uId					int	unsigned	not	null auto_increment,
	us_sName				varchar(96)		not	null,
	us_uVipType				tinyint unsigned	not	null default 0,	#vip��ʶ;0-��vip,1-vip
	
	primary	key(us_uId),
	unique key(us_sName)
)engine=innodb;

create table tbl_char_static
(
	cs_uId					bigint unsigned		not	null auto_increment,
	us_uId					int	unsigned			not	null,
	cs_uSex					tinyint	unsigned	not	null,	#�Ա�
	cs_uClass				tinyint	unsigned	not	null,	#ְҵ
	cs_uCamp				tinyint	unsigned	not	null,			#��Ӫ
	cs_dtCreateDate 		datetime not null					, #��ɫ����ʱ��
	cs_uServerId			int unsigned not null,				  #��������id
	cs_uState				tinyint unsigned not null default 1,    	#��ɫ״̬
	
	primary	key(cs_uId),
	foreign	key(us_uId)	references tbl_user_static(us_uId) on	update cascade
)engine=innodb;


create table tbl_char
(
	cs_uId					bigint	unsigned	not	null,
	c_sName					char(60) not null,
	c_sNameFormer			char(80) not null,
	
	primary	key(cs_uId),
	unique key(c_sName),
	foreign	key(cs_uId)	references tbl_char_static(cs_uId) on update cascade on delete cascade
)engine=innodb;


