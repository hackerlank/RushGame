set autocommit=0;

select @minscid:=case when min(sc_uId) is null then 0 else min(sc_uId) end from tbl_scene where sc_sSceneName='����ֵ�';
update tbl_scene as sc,tbl_char_position as cp
set cp.sc_uId = @minscid
where sc.sc_sSceneName = '����ֵ�' and sc.sc_uId = cp.sc_uId;
update tbl_scene as sc,tbl_war_zone as wz
set wz.sc_uId = @minscid
where sc.sc_sSceneName = '����ֵ�' and sc.sc_uId = wz.sc_uId;
delete from tbl_scene where sc_sSceneName = '����ֵ�' and sc_uId != @minscid;

select @minscid:=case when min(sc_uId) is null then 0 else min(sc_uId) end from tbl_scene where sc_sSceneName='����˹��ԭ';
update tbl_scene as sc,tbl_char_position as cp
set cp.sc_uId = @minscid
where sc.sc_sSceneName = '����˹��ԭ' and sc.sc_uId = cp.sc_uId;
update tbl_scene as sc,tbl_war_zone as wz
set wz.sc_uId = @minscid
where sc.sc_sSceneName = '����˹��ԭ' and sc.sc_uId = wz.sc_uId;
delete from tbl_scene where sc_sSceneName = '����˹��ԭ' and sc_uId != @minscid;

select @minscid:=case when min(sc_uId) is null then 0 else min(sc_uId) end from tbl_scene where sc_sSceneName='���׵۹����ݾư�';
update tbl_scene as sc,tbl_char_position as cp
set cp.sc_uId = @minscid
where sc.sc_sSceneName = '���׵۹����ݾư�' and sc.sc_uId = cp.sc_uId;
update tbl_scene as sc,tbl_war_zone as wz
set wz.sc_uId = @minscid
where sc.sc_sSceneName = '���׵۹����ݾư�' and sc.sc_uId = wz.sc_uId;
delete from tbl_scene where sc_sSceneName = '���׵۹����ݾư�' and sc_uId != @minscid;

select @minscid:=case when min(sc_uId) is null then 0 else min(sc_uId) end from tbl_scene where sc_sSceneName='��ʥ��͢���ݾư�';
update tbl_scene as sc,tbl_char_position as cp
set cp.sc_uId = @minscid
where sc.sc_sSceneName = '��ʥ��͢���ݾư�' and sc.sc_uId = cp.sc_uId;
update tbl_scene as sc,tbl_war_zone as wz
set wz.sc_uId = @minscid
where sc.sc_sSceneName = '��ʥ��͢���ݾư�' and sc.sc_uId = wz.sc_uId;
delete from tbl_scene where sc_sSceneName = '��ʥ��͢���ݾư�' and sc_uId != @minscid;

select @minscid:=case when min(sc_uId) is null then 0 else min(sc_uId) end from tbl_scene where sc_sSceneName='��˹�۹����ݾư�';
update tbl_scene as sc,tbl_char_position as cp
set cp.sc_uId = @minscid
where sc.sc_sSceneName = '��˹�۹����ݾư�' and sc.sc_uId = cp.sc_uId;
update tbl_scene as sc,tbl_war_zone as wz
set wz.sc_uId = @minscid
where sc.sc_sSceneName = '��˹�۹����ݾư�' and sc.sc_uId = wz.sc_uId;
delete from tbl_scene where sc_sSceneName = '��˹�۹����ݾư�' and sc_uId != @minscid;

select @minscid:=case when min(sc_uId) is null then 0 else min(sc_uId) end from tbl_scene where sc_sSceneName='�����ƽԭ';
update tbl_scene as sc,tbl_char_position as cp
set cp.sc_uId = @minscid
where sc.sc_sSceneName = '�����ƽԭ' and sc.sc_uId = cp.sc_uId;
update tbl_scene as sc,tbl_war_zone as wz
set wz.sc_uId = @minscid
where sc.sc_sSceneName = '�����ƽԭ' and sc.sc_uId = wz.sc_uId;
delete from tbl_scene where sc_sSceneName = '�����ƽԭ' and sc_uId != @minscid;

select @minscid:=case when min(sc_uId) is null then 0 else min(sc_uId) end from tbl_scene where sc_sSceneName='���Ͽ�';
update tbl_scene as sc,tbl_char_position as cp
set cp.sc_uId = @minscid
where sc.sc_sSceneName = '���Ͽ�' and sc.sc_uId = cp.sc_uId;
update tbl_scene as sc,tbl_war_zone as wz
set wz.sc_uId = @minscid
where sc.sc_sSceneName = '���Ͽ�' and sc.sc_uId = wz.sc_uId;
delete from tbl_scene where sc_sSceneName = '���Ͽ�' and sc_uId != @minscid;

select @minscid:=case when min(sc_uId) is null then 0 else min(sc_uId) end from tbl_scene where sc_sSceneName='����ɽ��';
update tbl_scene as sc,tbl_char_position as cp
set cp.sc_uId = @minscid
where sc.sc_sSceneName = '����ɽ��' and sc.sc_uId = cp.sc_uId;
update tbl_scene as sc,tbl_war_zone as wz
set wz.sc_uId = @minscid
where sc.sc_sSceneName = '����ɽ��' and sc.sc_uId = wz.sc_uId;
delete from tbl_scene where sc_sSceneName = '����ɽ��' and sc_uId != @minscid;

select @minscid:=case when min(sc_uId) is null then 0 else min(sc_uId) end from tbl_scene where sc_sSceneName='ǰ��ɽ��';
update tbl_scene as sc,tbl_char_position as cp
set cp.sc_uId = @minscid
where sc.sc_sSceneName = 'ǰ��ɽ��' and sc.sc_uId = cp.sc_uId;
update tbl_scene as sc,tbl_war_zone as wz
set wz.sc_uId = @minscid
where sc.sc_sSceneName = 'ǰ��ɽ��' and sc.sc_uId = wz.sc_uId;
delete from tbl_scene where sc_sSceneName = 'ǰ��ɽ��' and sc_uId != @minscid;

select @minscid:=case when min(sc_uId) is null then 0 else min(sc_uId) end from tbl_scene where sc_sSceneName='ʷ������';
update tbl_scene as sc,tbl_char_position as cp
set cp.sc_uId = @minscid
where sc.sc_sSceneName = 'ʷ������' and sc.sc_uId = cp.sc_uId;
update tbl_scene as sc,tbl_war_zone as wz
set wz.sc_uId = @minscid
where sc.sc_sSceneName = 'ʷ������' and sc.sc_uId = wz.sc_uId;
delete from tbl_scene where sc_sSceneName = 'ʷ������' and sc_uId != @minscid;

select @minscid:=case when min(sc_uId) is null then 0 else min(sc_uId) end from tbl_scene where sc_sSceneName='����С��';
update tbl_scene as sc,tbl_char_position as cp
set cp.sc_uId = @minscid
where sc.sc_sSceneName = '����С��' and sc.sc_uId = cp.sc_uId;
update tbl_scene as sc,tbl_war_zone as wz
set wz.sc_uId = @minscid
where sc.sc_sSceneName = '����С��' and sc.sc_uId = wz.sc_uId;
delete from tbl_scene where sc_sSceneName = '����С��' and sc_uId != @minscid;

select @minscid:=case when min(sc_uId) is null then 0 else min(sc_uId) end from tbl_scene where sc_sSceneName='���Ӵ�';
update tbl_scene as sc,tbl_char_position as cp
set cp.sc_uId = @minscid
where sc.sc_sSceneName = '���Ӵ�' and sc.sc_uId = cp.sc_uId;
update tbl_scene as sc,tbl_war_zone as wz
set wz.sc_uId = @minscid
where sc.sc_sSceneName = '���Ӵ�' and sc.sc_uId = wz.sc_uId;
delete from tbl_scene where sc_sSceneName = '���Ӵ�' and sc_uId != @minscid;

select @minscid:=case when min(sc_uId) is null then 0 else min(sc_uId) end from tbl_scene where sc_sSceneName='ͨ�ƹ�';
update tbl_scene as sc,tbl_char_position as cp
set cp.sc_uId = @minscid
where sc.sc_sSceneName = 'ͨ�ƹ�' and sc.sc_uId = cp.sc_uId;
update tbl_scene as sc,tbl_war_zone as wz
set wz.sc_uId = @minscid
where sc.sc_sSceneName = 'ͨ�ƹ�' and sc.sc_uId = wz.sc_uId;
delete from tbl_scene where sc_sSceneName = 'ͨ�ƹ�' and sc_uId != @minscid;

select @minscid:=case when min(sc_uId) is null then 0 else min(sc_uId) end from tbl_scene where sc_sSceneName='˫����';
update tbl_scene as sc,tbl_char_position as cp
set cp.sc_uId = @minscid
where sc.sc_sSceneName = '˫����' and sc.sc_uId = cp.sc_uId;
update tbl_scene as sc,tbl_war_zone as wz
set wz.sc_uId = @minscid
where sc.sc_sSceneName = '˫����' and sc.sc_uId = wz.sc_uId;
delete from tbl_scene where sc_sSceneName = '˫����' and sc_uId != @minscid;

select @minscid:=case when min(sc_uId) is null then 0 else min(sc_uId) end from tbl_scene where sc_sSceneName='������';
update tbl_scene as sc,tbl_char_position as cp
set cp.sc_uId = @minscid
where sc.sc_sSceneName = '������' and sc.sc_uId = cp.sc_uId;
update tbl_scene as sc,tbl_war_zone as wz
set wz.sc_uId = @minscid
where sc.sc_sSceneName = '������' and sc.sc_uId = wz.sc_uId;
delete from tbl_scene where sc_sSceneName = '������' and sc_uId != @minscid;

select @minscid:=case when min(sc_uId) is null then 0 else min(sc_uId) end from tbl_scene where sc_sSceneName='��³��';
update tbl_scene as sc,tbl_char_position as cp
set cp.sc_uId = @minscid
where sc.sc_sSceneName = '��³��' and sc.sc_uId = cp.sc_uId;
update tbl_scene as sc,tbl_war_zone as wz
set wz.sc_uId = @minscid
where sc.sc_sSceneName = '��³��' and sc.sc_uId = wz.sc_uId;
delete from tbl_scene where sc_sSceneName = '��³��' and sc_uId != @minscid;

select @minscid:=case when min(sc_uId) is null then 0 else min(sc_uId) end from tbl_scene where sc_sSceneName='�������ԭ';
update tbl_scene as sc,tbl_char_position as cp
set cp.sc_uId = @minscid
where sc.sc_sSceneName = '�������ԭ' and sc.sc_uId = cp.sc_uId;
update tbl_scene as sc,tbl_war_zone as wz
set wz.sc_uId = @minscid
where sc.sc_sSceneName = '�������ԭ' and sc.sc_uId = wz.sc_uId;
delete from tbl_scene where sc_sSceneName = '�������ԭ' and sc_uId != @minscid;

select @minscid:=case when min(sc_uId) is null then 0 else min(sc_uId) end from tbl_scene where sc_sSceneName='ʨ�Ӻӹȵ�';
update tbl_scene as sc,tbl_char_position as cp
set cp.sc_uId = @minscid
where sc.sc_sSceneName = 'ʨ�Ӻӹȵ�' and sc.sc_uId = cp.sc_uId;
update tbl_scene as sc,tbl_war_zone as wz
set wz.sc_uId = @minscid
where sc.sc_sSceneName = 'ʨ�Ӻӹȵ�' and sc.sc_uId = wz.sc_uId;
delete from tbl_scene where sc_sSceneName = 'ʨ�Ӻӹȵ�' and sc_uId != @minscid;

select @minscid:=case when min(sc_uId) is null then 0 else min(sc_uId) end from tbl_scene where sc_sSceneName='ʥѩɽ';
update tbl_scene as sc,tbl_char_position as cp
set cp.sc_uId = @minscid
where sc.sc_sSceneName = 'ʥѩɽ' and sc.sc_uId = cp.sc_uId;
update tbl_scene as sc,tbl_war_zone as wz
set wz.sc_uId = @minscid
where sc.sc_sSceneName = 'ʥѩɽ' and sc.sc_uId = wz.sc_uId;
delete from tbl_scene where sc_sSceneName = 'ʥѩɽ' and sc_uId != @minscid;

select @minscid:=case when min(sc_uId) is null then 0 else min(sc_uId) end from tbl_scene where sc_sSceneName='���ֵ�';
update tbl_scene as sc,tbl_char_position as cp
set cp.sc_uId = @minscid
where sc.sc_sSceneName = '���ֵ�' and sc.sc_uId = cp.sc_uId;
update tbl_scene as sc,tbl_war_zone as wz
set wz.sc_uId = @minscid
where sc.sc_sSceneName = '���ֵ�' and sc.sc_uId = wz.sc_uId;
delete from tbl_scene where sc_sSceneName = '���ֵ�' and sc_uId != @minscid;

select @minscid:=case when min(sc_uId) is null then 0 else min(sc_uId) end from tbl_scene where sc_sSceneName='����1';
update tbl_scene as sc,tbl_char_position as cp
set cp.sc_uId = @minscid
where sc.sc_sSceneName = '����1' and sc.sc_uId = cp.sc_uId;
update tbl_scene as sc,tbl_war_zone as wz
set wz.sc_uId = @minscid
where sc.sc_sSceneName = '����1' and sc.sc_uId = wz.sc_uId;
delete from tbl_scene where sc_sSceneName = '����1' and sc_uId != @minscid;

select @minscid:=case when min(sc_uId) is null then 0 else min(sc_uId) end from tbl_scene where sc_sSceneName='���Ե�ͼ';
update tbl_scene as sc,tbl_char_position as cp
set cp.sc_uId = @minscid
where sc.sc_sSceneName = '���Ե�ͼ' and sc.sc_uId = cp.sc_uId;
update tbl_scene as sc,tbl_war_zone as wz
set wz.sc_uId = @minscid
where sc.sc_sSceneName = '���Ե�ͼ' and sc.sc_uId = wz.sc_uId;
delete from tbl_scene where sc_sSceneName = '���Ե�ͼ' and sc_uId != @minscid;

select @minscid:=case when min(sc_uId) is null then 0 else min(sc_uId) end from tbl_scene where sc_sSceneName='ˢ��';
update tbl_scene as sc,tbl_char_position as cp
set cp.sc_uId = @minscid
where sc.sc_sSceneName = 'ˢ��' and sc.sc_uId = cp.sc_uId;
update tbl_scene as sc,tbl_war_zone as wz
set wz.sc_uId = @minscid
where sc.sc_sSceneName = 'ˢ��' and sc.sc_uId = wz.sc_uId;
delete from tbl_scene where sc_sSceneName = 'ˢ��' and sc_uId != @minscid;

select @minscid:=case when min(sc_uId) is null then 0 else min(sc_uId) end from tbl_scene where sc_sSceneName='�������';
update tbl_scene as sc,tbl_char_position as cp
set cp.sc_uId = @minscid
where sc.sc_sSceneName = '�������' and sc.sc_uId = cp.sc_uId;
update tbl_scene as sc,tbl_war_zone as wz
set wz.sc_uId = @minscid
where sc.sc_sSceneName = '�������' and sc.sc_uId = wz.sc_uId;
delete from tbl_scene where sc_sSceneName = '�������' and sc_uId != @minscid;

select @minscid:=case when min(sc_uId) is null then 0 else min(sc_uId) end from tbl_scene where sc_sSceneName='�ر���Ե�ͼ';
update tbl_scene as sc,tbl_char_position as cp
set cp.sc_uId = @minscid
where sc.sc_sSceneName = '�ر���Ե�ͼ' and sc.sc_uId = cp.sc_uId;
update tbl_scene as sc,tbl_war_zone as wz
set wz.sc_uId = @minscid
where sc.sc_sSceneName = '�ر���Ե�ͼ' and sc.sc_uId = wz.sc_uId;
delete from tbl_scene where sc_sSceneName = '�ر���Ե�ͼ' and sc_uId != @minscid;

select @minscid:=case when min(sc_uId) is null then 0 else min(sc_uId) end from tbl_scene where sc_sSceneName='���׵۹�ְҵʥ��';
update tbl_scene as sc,tbl_char_position as cp
set cp.sc_uId = @minscid
where sc.sc_sSceneName = '���׵۹�ְҵʥ��' and sc.sc_uId = cp.sc_uId;
update tbl_scene as sc,tbl_war_zone as wz
set wz.sc_uId = @minscid
where sc.sc_sSceneName = '���׵۹�ְҵʥ��' and sc.sc_uId = wz.sc_uId;
delete from tbl_scene where sc_sSceneName = '���׵۹�ְҵʥ��' and sc_uId != @minscid;

select @minscid:=case when min(sc_uId) is null then 0 else min(sc_uId) end from tbl_scene where sc_sSceneName='��ʥ��ְ͢ҵʥ��';
update tbl_scene as sc,tbl_char_position as cp
set cp.sc_uId = @minscid
where sc.sc_sSceneName = '��ʥ��ְ͢ҵʥ��' and sc.sc_uId = cp.sc_uId;
update tbl_scene as sc,tbl_war_zone as wz
set wz.sc_uId = @minscid
where sc.sc_sSceneName = '��ʥ��ְ͢ҵʥ��' and sc.sc_uId = wz.sc_uId;
delete from tbl_scene where sc_sSceneName = '��ʥ��ְ͢ҵʥ��' and sc_uId != @minscid;

select @minscid:=case when min(sc_uId) is null then 0 else min(sc_uId) end from tbl_scene where sc_sSceneName='��˹�۹�ְҵʥ��';
update tbl_scene as sc,tbl_char_position as cp
set cp.sc_uId = @minscid
where sc.sc_sSceneName = '��˹�۹�ְҵʥ��' and sc.sc_uId = cp.sc_uId;
update tbl_scene as sc,tbl_war_zone as wz
set wz.sc_uId = @minscid
where sc.sc_sSceneName = '��˹�۹�ְҵʥ��' and sc.sc_uId = wz.sc_uId;
delete from tbl_scene where sc_sSceneName = '��˹�۹�ְҵʥ��' and sc_uId != @minscid;

select @minscid:=case when min(sc_uId) is null then 0 else min(sc_uId) end from tbl_scene where sc_sSceneName='�����ð����ս��ͼ';
update tbl_scene as sc,tbl_char_position as cp
set cp.sc_uId = @minscid
where sc.sc_sSceneName = '�����ð����ս��ͼ' and sc.sc_uId = cp.sc_uId;
update tbl_scene as sc,tbl_war_zone as wz
set wz.sc_uId = @minscid
where sc.sc_sSceneName = '�����ð����ս��ͼ' and sc.sc_uId = wz.sc_uId;
delete from tbl_scene where sc_sSceneName = '�����ð����ս��ͼ' and sc_uId != @minscid;

select @minscid:=case when min(sc_uId) is null then 0 else min(sc_uId) end from tbl_scene where sc_sSceneName='ǰ������';
update tbl_scene as sc,tbl_char_position as cp
set cp.sc_uId = @minscid
where sc.sc_sSceneName = 'ǰ������' and sc.sc_uId = cp.sc_uId;
update tbl_scene as sc,tbl_war_zone as wz
set wz.sc_uId = @minscid
where sc.sc_sSceneName = 'ǰ������' and sc.sc_uId = wz.sc_uId;
delete from tbl_scene where sc_sSceneName = 'ǰ������' and sc_uId != @minscid;

select @minscid:=case when min(sc_uId) is null then 0 else min(sc_uId) end from tbl_scene where sc_sSceneName='�������»���ƽԭ';
update tbl_scene as sc,tbl_char_position as cp
set cp.sc_uId = @minscid
where sc.sc_sSceneName = '�������»���ƽԭ' and sc.sc_uId = cp.sc_uId;
update tbl_scene as sc,tbl_war_zone as wz
set wz.sc_uId = @minscid
where sc.sc_sSceneName = '�������»���ƽԭ' and sc.sc_uId = wz.sc_uId;
delete from tbl_scene where sc_sSceneName = '�������»���ƽԭ' and sc_uId != @minscid;

select @minscid:=case when min(sc_uId) is null then 0 else min(sc_uId) end from tbl_scene where sc_sSceneName='������ֵ�';
update tbl_scene as sc,tbl_char_position as cp
set cp.sc_uId = @minscid
where sc.sc_sSceneName = '������ֵ�' and sc.sc_uId = cp.sc_uId;
update tbl_scene as sc,tbl_war_zone as wz
set wz.sc_uId = @minscid
where sc.sc_sSceneName = '������ֵ�' and sc.sc_uId = wz.sc_uId;
delete from tbl_scene where sc_sSceneName = '������ֵ�' and sc_uId != @minscid;

select @minscid:=case when min(sc_uId) is null then 0 else min(sc_uId) end from tbl_scene where sc_sSceneName='����ƽԭ1';
update tbl_scene as sc,tbl_char_position as cp
set cp.sc_uId = @minscid
where sc.sc_sSceneName = '����ƽԭ1' and sc.sc_uId = cp.sc_uId;
update tbl_scene as sc,tbl_war_zone as wz
set wz.sc_uId = @minscid
where sc.sc_sSceneName = '����ƽԭ1' and sc.sc_uId = wz.sc_uId;
delete from tbl_scene where sc_sSceneName = '����ƽԭ1' and sc_uId != @minscid;

select @minscid:=case when min(sc_uId) is null then 0 else min(sc_uId) end from tbl_scene where sc_sSceneName='�����Ͽ�';
update tbl_scene as sc,tbl_char_position as cp
set cp.sc_uId = @minscid
where sc.sc_sSceneName = '�����Ͽ�' and sc.sc_uId = cp.sc_uId;
update tbl_scene as sc,tbl_war_zone as wz
set wz.sc_uId = @minscid
where sc.sc_sSceneName = '�����Ͽ�' and sc.sc_uId = wz.sc_uId;
delete from tbl_scene where sc_sSceneName = '�����Ͽ�' and sc_uId != @minscid;


#���е���������ID����1����ID
update tbl_scene as s,ybtx.tbl_scene as tmp 
set s.sc_uId=tmp.sc_uId 
where s.sc_sSceneName=tmp.sc_sSceneName 
and (s.sc_sSceneName != '����ֵ�' 
or s.sc_sSceneName != '����˹��ԭ' 
or s.sc_sSceneName != '���׵۹����ݾư�' 
or s.sc_sSceneName != '��ʥ��͢���ݾư�' 
or s.sc_sSceneName != '��˹�۹����ݾư�' 
or s.sc_sSceneName != '�����ƽԭ' 
or s.sc_sSceneName != '���Ͽ�' 
or s.sc_sSceneName != '����ɽ��' 
or s.sc_sSceneName != 'ǰ��ɽ��' 
or s.sc_sSceneName != 'ʷ������' 
or s.sc_sSceneName != '����С��'
or s.sc_sSceneName != '���Ӵ�'
or s.sc_sSceneName != 'ͨ�ƹ�'
or s.sc_sSceneName != '˫����'
or s.sc_sSceneName != '������'
or s.sc_sSceneName != '��³��'
or s.sc_sSceneName != '�������ԭ'
or s.sc_sSceneName != 'ʨ�Ӻӹȵ�'
or s.sc_sSceneName != 'ʥѩɽ'
or s.sc_sSceneName != '���ֵ�'
or s.sc_sSceneName != '����1'
or s.sc_sSceneName != '���Ե�ͼ'
or s.sc_sSceneName != 'ˢ��'
or s.sc_sSceneName != '�������'
or s.sc_sSceneName != '�ر���Ե�ͼ'
or s.sc_sSceneName != '���׵۹�ְҵʥ��'
or s.sc_sSceneName != '��ʥ��ְ͢ҵʥ��'
or s.sc_sSceneName != '��˹�۹�ְҵʥ��'
or s.sc_sSceneName != '�����ð����ս��ͼ'
or s.sc_sSceneName != 'ǰ������'
or s.sc_sSceneName != '�������»���ƽԭ'
or s.sc_sSceneName != '������ֵ�'
or s.sc_sSceneName != '����ƽԭ1'
or s.sc_sSceneName != '�����Ͽ�');

commit;
