drop database if exists medi_database;
create database medi_database;
use medi_database;

#��¼���ݿ������ID��Ҫ��ƫ����

create table tbl_base_Id
(
	bi_sBaseName					char(60) not null,	
	bi_uId						tinyint unsigned not null default 1,
	
	primary key(bi_sBaseName),
	unique key(bi_uId)
)engine=myisam;

create table tbl_id_add_count
(
	ian_sBaseName					char(60) not null, #��ǰ���ݿ�

	#army_corps.sql
	tac_uId						bigint	unsigned	default 0, #tbl_army_corps

	#association.sql
#	fc_uId						bigint	unsigned	default 0, #tbl_friends_class
	om_uID						bigint	unsigned	default 0, #tbl_offline_msg
	fg_uId						bigint	unsigned	default 0, #tbl_friends_group
	sm_uId						bigint	unsigned	default 0, #tbl_system_message

	#battle.sql
	bs_uId						bigint	unsigned	default 0, #tbl_battle_shape
	ba_uId						bigint	unsigned	default 0, #tbl_battle_array

	#erating.sql
	tid_uDetailID				bigint	unsigned	default 0, #tbl_erating_detail_id

	#gm_command.sql
	gc_uID						bigint	unsigned	default 0, #tbl_gm_command
	gcl_uID						bigint	unsigned	default 0, #tbl_gm_command_log
	ac_uID						bigint	unsigned	default 0, #tbl_action_closed

	#item.sql
	ii_uId						bigint	unsigned	default 0, #tbl_item_id
	is_uid						bigint	unsigned	default 0, #tbl_item_static	
	ibp_uId         			bigint	unsigned 	default 0, #tbl_item_boxitem_pickup
	sh_uId						bigint	unsigned	default 0, #tbl_stone_hole
	cds_uId						bigint	unsigned	default 0, #tbl_collectivity_depot

	#liveskill.sql
	cs_uSID						bigint	unsigned	default 0, #tbl_commerce_skill

	#log.sql
	le_uId						bigint	unsigned	default 0, #tbl_log_event
	
	#mail.sql
	ms_uID						bigint	unsigned	default 0, #tbl_mail

	#market.sql
	mos_uId						bigint	unsigned	default 0, #tbl_market_order_static	
	npsi_nId					bigint	unsigned	default 0, #tbl_npcshop_player_sell_item
	cmo_uId						bigint	unsigned	default 0, #tbl_contract_manufacture_order_static

	#pet.sql
	ps_uId						bigint	unsigned	default 0, #tbl_pet_static	
	
	#scene.sql
	#sc_uId						bigint	unsigned	default 0, #tbl_scene
	
	#scene_fb.sql
	tar_uId						bigint	unsigned	default 0, #tbl_action_room
	tat_uId						bigint	unsigned	default 0, #tbl_action_team

	#team.sql
	t_uId						bigint	unsigned	default 0, #tbl_team_static

	#tong.sql
	tong_uId					bigint	unsigned	default 0, #tbl_tong
	lt_uId						bigint	unsigned	default 0, #tbl_log_tong
	rt_uId						bigint	unsigned	default 0, #tbl_request_tong
	bt_uId						bigint	unsigned	default 0, #tbl_building_tong
	tic_uId						bigint	unsigned	default 0, #tbl_tong_item_create
	tts_uId						bigint	unsigned	default 0, #tbl_tong_tech_static

	#toolsmall.sql
	tbd_uDetailID				bigint	unsigned	default 0, #tbl_toolsmall_buy_detail

	#user_log.sql
	ua_uErrorCode				bigint	unsigned	default 0, #tbl_user_advice	

	#war_zone.sql	
	wz_uId						bigint	unsigned	default 0, #tbl_war_zone
		
	primary key(ian_sBaseName)
)engine=myisam;

#�����Ҫ�������İ����Ϣ
create table tbl_tong_info
(
	t_uId						bigint	unsigned	default 0,
	t_sName						varchar(100)	not null,
	primary key(t_uId)
)engine=myisam;

create table tbl_char_info
(
	cs_uId						bigint	unsigned	default 0,
	c_sName						varchar(100)	not null,
	primary key(cs_uId)
)engine=myisam;

create table tbl_chardel_info
(
	cs_uId						bigint	unsigned	default 0,
	cd_sName					varchar(100)	not null,
	primary key(cs_uId)
)engine=myisam;

create table tbl_user_info
(
	us_sName					varchar(100) not	null,
	us_uId						bigint unsigned not null,
	ub_uOnlineTime					bigint unsigned  not null	default	0,	 #�û���ĳ�����������ۼ�����ʱ��
	ub_uYuanBao					bigint	unsigned	not null  default 0, #����˺���ĳ���������ϵ�Ԫ��
	ub_uAgreedVer					smallint unsigned not null default 0,  #��ĳ�����������û��ϴ�ͬ���û�Э��İ汾��
	
	primary	key(us_sName)
)engine=myisam;

#������з�������log��¼����ֹ����û���
create table tbl_log_user_id
(
	lus_sName					varchar(100) not	null,
	lus_uId						bigint unsigned not null,
	
	primary	key(lus_sName)
)engine=myisam;
