
#��¼���ݿ������ID��Ҫ��ƫ����


create table tbl_id_add_count
(
	ian_sBaseName					char(60) not null, #��ǰ���ݿ�
	ian_uId						tinyint	unsigned not null default 1,
	primary key(ian_sBaseName),
	unique key(ian_uId)
)engine=InnoDB;

create table tbl_army_info
(
	ian_uId						tinyint	unsigned	default 1,
	ac_uId						bigint	unsigned	default 0,
	ac_sName					varchar(100) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL,
	ac_uLevel					tinyint	unsigned	default 1,
	primary key(ac_uId)
)engine=InnoDB;

#�����Ҫ�������İ����Ϣ
create table tbl_tong_info
(
	ian_uId						tinyint	unsigned	default 1,
	t_uId						bigint	unsigned	default 0,
	t_sName						varchar(100) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL,
	t_uLevel					tinyint	unsigned	default 1,
	primary key(t_uId)
)engine=InnoDB;

create table tbl_char_info
(
	ian_uId						tinyint	unsigned	default 1,
	cs_uId						bigint	unsigned	default 0,
	c_sName						char(32) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL,
	cb_uLevel					tinyint	unsigned	default 1,
	primary key(cs_uId)
)engine=InnoDB;


create table tbl_user_info
(
	us_sName					varchar(100) not	null,
	us_uId						bigint unsigned not null,
	ub_uOnlineTime					bigint unsigned  not null	default	0,	 #�û���ĳ�����������ۼ�����ʱ��
	ub_uYuanBao					bigint	unsigned	not null  default 0, #����˺���ĳ���������ϵ�Ԫ��
	ub_uAgreedVer					smallint unsigned not null default 0,  #��ĳ�����������û��ϴ�ͬ���û�Э��İ汾��
	
	primary	key(us_sName)
)engine=InnoDB;

