--��������test case����һ��table����
g_TestCaseFun = {}

local TestCaseTbl = {
	--"test_unit/TestLogin",--���Ե�¼,ֻ�ǲ��Ե�¼
	--"test_unit/TestLogoutAndLogin",--ѭ��(��¼,�˳�,��¼)
	--"test_unit/TestLoginWait",--�����Ŷӵ�¼
	--"test_unit/TestChangeScene",--�����г���
	--"test_unit/TestLoginRepeat",--���Զ���
	--"test_unit/TestLoginFlow",--Ŀǰû��Ӧ��.ѭ��(��¼��Ϸ,�г���,����,����)
	"test_unit/TestMatchGameFb",--���Խ�������
	--"test_unit/TestConsignment",--����������
	--"test_unit/TestPurchasing",--��������
}

for i=1, #(TestCaseTbl) do
	g_TestCaseFun[i] = str_require(TestCaseTbl[i])
end
