local SetLevelMsg = MsgTbl.SetLevelMsg

local TestConsignmentCase = TestCase("TestConsignment")

local function PrintInfo(...)
	print(...)
end

function TestConsignmentCase:test_Consignment(case_data)
	local UserName = case_data.m_UserName
	
	if not case_data.m_aa then
		case_data.m_aa = 0
	end
	case_data.m_aa = case_data.m_aa + 1
	--PrintInfo(1,UserName,case_data.m_aa)
	StressRpcToServer("GM_Execute", case_data.m_Conn, "$addlevel(80)" )
	WaitForMsg(SetLevelMsg)
	--PrintInfo(2,UserName,case_data.m_aa)
	PrintInfo(UserName,"��ȫ�˳�����.............1")
end

return TestConsignmentCase

--
--	{"CSMBuyOrder",    "I"},					--���򶩵���Ʒ�� ����������ID
--	{"CSMSearchBuyOrder",   "IIIsIIiII" },	--��ѯ�����б���������Ʒ���ͣ���Ʒ���ƣ���ʼ�ȼ�����ֹ�ȼ�������ʽ��ҳ��
--	{"CSMSearchSellOrder",   "sIIIIIIiIiib" },	--��ѯ�����б���������Ʒ���ƣ���ʼ�ȼ�����ֹ�ȼ����Ƿ���ã�����ʽ��ҳ����1-��Ʒ��2-��װ��ս��ħ��
--	{"CSMGetOrderByItemType","IIsIIIIIIiIiib"},	--ͬ�ϣ����ϱ߶�������������壬��Ʒ���
--	{"CSMGetOrderByItemAttr","IIsssssIIIIIIiIiib"},--ͬ�ϣ������Ҳ�������壬���������� parentNodeText������������ ����������
--	{"CSMGetOrderBySomeItemType","IIsIIIIIIiIiib"},--��ѯ�����б���������壬 ĳ������Ʒ�Ĵ������֣�ĳһ���ִ�������ļ�����Ʒ��,��Ʒ���ƣ���ʼ�ȼ�����ֹ�ȼ����Ƿ���ã�����ʽ��ҳ��
--	{"CSMGetOrderByExactItemAttr","IIIsIIIIIIiIiib"},--��ѯ�����б���������壬��Ʒ���ͣ�����������������ĳ������,��Ʒ���ƣ���ʼ�ȼ�����ֹ�ȼ����Ƿ���ã�����ʽ��ҳ��
--	{"CSMSearchCharBuyOrder",   "" },		--��Ҳ�ѯ���˹����б�
--	{"CSMSearchCharSellOrder",   "II"},		--��Ҳ�ѯ���˳����������������ʽ��ҳ��
--	{"CSMAddBuyOrder",   "sIdI" },						--�������չ���������������Ʒ������Ŀ���۸�ʱ��
--	{"CSMAddSellOrder",   "iIIdIb"},						--�����ӳ��۶�����������RoomIndex��pos����Ŀ���۸�ʱ��,�Ƿ񼤻��ס�۸��ܣ�true��false��
--	{"CSMCancelBuyOrder",   "I" },					--���ȡ���չ�����������������ID
--	{"CSMCancelSellOrder",   "I"},					--���ȡ�����۶���������������ID
--	{"CSMTakeAttachment",    "I"},					--�����ȡ������ ����������ID
--	{"CSMSellGoods2Order",       "IiI"},					--֧������,����������ID��RoomIndex��pos
--	{"CSMFastSearchBuyOrderItem",       ""},					--֧������,����������ID��RoomIndex��pos
--	{"GetCSMRememberPrice",	"Iss"},					--�õ�����ϴγ���ĳ����Ʒ�ļ۸񣬲���:���no����Ʒ���ͣ���Ʒ����
--	{"CSMGetOrderBySeveralSortItem", "IIIsIIIIIIiIiib"},
--	{"CSMGetTopPriceBuyOrderByItemName","s"},
--	{"CSMGetAveragePriceByItemName","s"},
--	{"SetMoneyType",       "I"},	  --���ð��������ѡ�еĻ�������