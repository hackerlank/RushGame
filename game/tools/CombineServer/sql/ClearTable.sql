set autocommit=0;

delete from tbl_sort_char_kill_boss_num;
delete from tbl_sort_char_kill_boss_num_by_camp;
delete from tbl_sort_char_resource;
delete from tbl_sort_char_resource_by_camp;
delete from tbl_sort_create_time;
delete from tbl_sort_dataosha_all_jointimes;
delete from tbl_sort_dataosha_all_jointimes_by_camp;
delete from tbl_sort_dataosha_all_wintimes;
delete from tbl_sort_dataosha_all_wintimes_by_camp;
delete from tbl_sort_dataosha_point;
delete from tbl_sort_dataosha_point_by_camp;
delete from tbl_sort_deadtimes;
delete from tbl_sort_deadtimes_by_camp;
delete from tbl_sort_fight_evaluation;
delete from tbl_sort_fight_evaluation_by_camp;
delete from tbl_sort_jifensai_join_times;
delete from tbl_sort_jifensai_join_times_by_camp;
delete from tbl_sort_jifensai_wintimes;
delete from tbl_sort_jifensai_wintimes_by_camp;
delete from tbl_sort_kill_npc_count;
delete from tbl_sort_kill_npc_count_by_camp;
delete from tbl_sort_kill_player_count;
delete from tbl_sort_kill_player_count_by_camp;
delete from tbl_sort_level;
delete from tbl_sort_level_by_camp;
delete from tbl_sort_money;
delete from tbl_sort_money_by_camp;
delete from tbl_sort_player_onlinetime;
delete from tbl_sort_player_onlinetime_by_camp;
delete from tbl_sort_resource_kill_num;
delete from tbl_sort_resource_kill_num_by_camp;
delete from tbl_sort_tong_exploit;
delete from tbl_sort_tong_exploit_by_camp;
delete from tbl_sort_tong_kill_boss_num;
delete from tbl_sort_tong_kill_boss_num_by_camp;
delete from tbl_sort_tong_kill_num;
delete from tbl_sort_tong_kill_num_by_camp;
delete from tbl_sort_tong_level;
delete from tbl_sort_tong_level_by_camp;
delete from tbl_sort_tong_money;
delete from tbl_sort_tong_money_by_camp;
delete from tbl_sort_tong_resource;
delete from tbl_sort_tong_resource_by_camp;
delete from tbl_sort_tong_resource_occupy_times;
delete from tbl_sort_tong_resource_occupy_times_by_camp;

#delete from tbl_cpu_status;
delete from tbl_long_string_value1;
delete from tbl_arg_common1;
delete from tbl_msg_common1;
delete from tbl_long_string_value2;
delete from tbl_arg_common2;
delete from tbl_msg_common2;
delete from tbl_long_string_value3;
delete from tbl_arg_common3;
delete from tbl_msg_common3;
delete from tbl_long_string_value4;
delete from tbl_arg_common4;
delete from tbl_msg_common4;
delete from tbl_long_string_value5;
delete from tbl_arg_common5;
delete from tbl_msg_common5;
delete from tbl_long_string_value6;
delete from tbl_arg_common6;
delete from tbl_msg_common6;
delete from tbl_long_string_value7;
delete from tbl_arg_common7;
delete from tbl_msg_common7;
delete from tbl_long_string_value8;
delete from tbl_arg_common8;
delete from tbl_msg_common8;
delete from tbl_long_string_value9;
delete from tbl_arg_common9;
delete from tbl_msg_common9;
delete from tbl_long_string_value10;
delete from tbl_arg_common10;
delete from tbl_msg_common10;
delete from tbl_long_string_value11;
delete from tbl_arg_common11;
delete from tbl_msg_common11;
delete from tbl_long_string_value12;
delete from tbl_arg_common12;
delete from tbl_msg_common12;
delete from tbl_long_string_value13;
delete from tbl_arg_common13;
delete from tbl_msg_common13;
delete from tbl_long_string_value14;
delete from tbl_arg_common14;
delete from tbl_msg_common14;
delete from tbl_long_string_value15;
delete from tbl_arg_common15;
delete from tbl_msg_common15;

delete from tbl_hot_update_gas;
delete from tbl_hot_update_gac;
delete from tbl_hot_update_db;
delete from tbl_hot_update_result;

delete from tbl_IBPay_detail_memory;
delete from tbl_user_draw_balance_memory;

delete from tbl_tong_rob_state;
delete from tbl_tong_rob_scene;

delete from tbl_char_server_lastleve;
delete from tbl_var_server;
delete from tbl_war_zone_sentry;
delete from tbl_war_zone_station;
delete from tbl_user_online;
delete from tbl_server;
delete from tbl_user_wait_queue;
delete from tbl_channel_id;
delete from tbl_channel_name;
delete from ONLINE_STAT;
delete from tbl_server_version;
delete from tbl_user_logout;
delete from tbl_npcshop_player_sell_item;
delete from tbl_player_sell_item_id;
delete from tbl_market_cfg_info;
delete from tbl_char_deleted;

#tbl_erating_detail_id��ÿ10������һ�Σ�Ҫ��ȡ�������ݸո���գ��ϲ�������id����0�ˣ������ںϲ�֮ǰ�Ȳ���һ���������Ͳ�����������Ϊ�յ��������ʱ�ȷ�������
insert into tbl_erating_detail_id values();

#������ֻ����������
delete from tbl_scene 
where sc_sSceneName != '����ֵ�' 
and sc_sSceneName != '����˹��ԭ' 
and sc_sSceneName != '���׵۹����ݾư�' 
and sc_sSceneName != '��ʥ��͢���ݾư�' 
and sc_sSceneName != '��˹�۹����ݾư�' 
and sc_sSceneName != '�����ƽԭ' 
and sc_sSceneName != '���Ͽ�' 
and sc_sSceneName != '����ɽ��' 
and sc_sSceneName != 'ǰ��ɽ��' 
and sc_sSceneName != 'ʷ������' 
and sc_sSceneName != '����С��'
and sc_sSceneName != '���Ӵ�'
and sc_sSceneName != 'ͨ�ƹ�'
and sc_sSceneName != '˫����'
and sc_sSceneName != '������'
and sc_sSceneName != '��³��'
and sc_sSceneName != '�������ԭ'
and sc_sSceneName != 'ʨ�Ӻӹȵ�'
and sc_sSceneName != 'ʥѩɽ'
and sc_sSceneName != '���ֵ�'
and sc_sSceneName != '����1'
and sc_sSceneName != '���Ե�ͼ'
and sc_sSceneName != 'ˢ��'
and sc_sSceneName != '�������'
and sc_sSceneName != '�ر���Ե�ͼ'
and sc_sSceneName != '���׵۹�ְҵʥ��'
and sc_sSceneName != '��ʥ��ְ͢ҵʥ��'
and sc_sSceneName != '��˹�۹�ְҵʥ��'
and sc_sSceneName != '�����ð����ս��ͼ'
and sc_sSceneName != 'ǰ������'
and sc_sSceneName != '�������»���ƽԭ'
and sc_sSceneName != '������ֵ�'
and sc_sSceneName != '����ƽԭ1'
and sc_sSceneName != '�����Ͽ�';

commit;
