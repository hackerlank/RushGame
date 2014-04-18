set autocommit=0;

#��¼���з�������ĳ���û����ĵ�һ���û�ID�����IDΪ�ϲ����������Ч�û�ID���Լ�ĳЩ���ݣ���ʼ��Ϊ0�������ںϲ������¼����û�������
insert ignore into medi_database.tbl_user_info select us_sName,us_uId,0,0,0 from tbl_user_static;

update 
medi_database.tbl_user_info as tmp, 
tbl_user_static as us,
tbl_user_basic as ub 
set 
tmp.ub_uOnlineTime = tmp.ub_uOnlineTime+ub.ub_uOnlineTime,
tmp.ub_uYuanBao = tmp.ub_uYuanBao+ub.ub_uYuanBao
where 
tmp.us_sName = us.us_sName 
and us.us_uId = ub.us_uId;

update 
medi_database.tbl_user_info as tmp, 
tbl_user_static as us,
tbl_user_basic as ub 
set 
tmp.ub_uAgreedVer = ub.ub_uAgreedVer
where 
tmp.us_sName = us.us_sName
and us.us_uId = ub.us_uId
and ub.ub_uAgreedVer > tmp.ub_uAgreedVer;

commit;
