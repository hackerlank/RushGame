--�Ŷӵȴ�
--�������ʱ��ǵý�GasConfig.lua����LogouTime�ĳ�10000�������£���Ȼ�Ŷӵȴ���ʱ���ܳ�s

local TestLoginWaitCase = TestCase("TestLoginWait", true)  --���ǲ�ʹ��Ĭ�ϵĳ�ʼ��

local LoginSucMsg = MsgTbl.LoginSucMsg
local LoginWaitingMsg = MsgTbl.LoginWaitingMsg
local DelWaitingMsg = MsgTbl.DelWaitingMsg
local UpdateWaitingMsg = MsgTbl.UpdateWaitingMsg
local RepeatLoginMsg = MsgTbl.RepeatLoginMsg

local function GetRandWaitTime()
	local MinTime = 1
	local MaxTime = 10

	return math.random(MinTime,MaxTime) * 1000
end

function TestLoginWaitCase:SetUp(case_data)
	local Conn = SyncConnect(ConnAddr)
	case_data.m_Conn = Conn	
end

function TestLoginWaitCase:test_CancelWait(case_data)
	local Conn = case_data.m_Conn

	local user_name = case_data.m_UserName
	
	local msg = SyncCheckUser(Conn, user_name)
	--��������Ҫ���Ե��ǵ�¼�Ŷӣ�����ܹ���¼�ɹ�����ô�ȴ�һ��ʱ��֮���������
	if msg == LoginSucMsg or msg == RepeatLoginMsg then
		local waited_time = GetRandWaitTime()

		Conn = ReConnect(case_data, waited_time, ConnAddr)

		return
	end
	
	--ȡ���Ŷ�
	assert(IsCppBound(Conn))
	StressRpcToServer("CancelLoginWaiting", Conn)
	
	--������Ŷӵ�¼�ˣ����ǾͿ��Բ�����
	while true do
		local LastWaitingID = Conn.m_WaitingID
		assert(LastWaitingID)
		
		local msg =	WaitForMsg(UpdateWaitingMsg, DelWaitingMsg, LoginSucMsg)
		if msg == UpdateWaitingMsg then
			assert(Conn.m_WaitingID < LastWaitingID)
		else
			break
		end
	end
	
	local waited_time = GetRandWaitTime()
	Conn = ReConnect(case_data, waited_time, ConnAddr)
end

function TestLoginWaitCase:test_UpdateWaiting(case_data)
	local Conn = case_data.m_Conn
	local user_name = case_data.m_UserName
	
	local msg = SyncCheckUser(Conn, user_name)
	--��������Ҫ���Ե��ǵ�¼�Ŷӣ�����ܹ���¼�ɹ�����ô�ȴ�һ��ʱ��֮���������
	if msg == LoginSucMsg or msg == RepeatLoginMsg then
		local waited_time = GetRandWaitTime()
		Conn = ReConnect(case_data, waited_time, ConnAddr)
		return
	end
	
	while true do
		local LastWaitingID = Conn.m_WaitingID
		assert(LastWaitingID)
		local waited_msg = WaitForMsg(UpdateWaitingMsg, LoginSucMsg)
		if waited_msg == LoginSucMsg then
			local waited_time = GetRandWaitTime()
			Conn = ReConnect(case_data, waited_time, ConnAddr)
			return
		else
			local CurWaitingID = Conn.m_WaitingID
			assert(CurWaitingID < LastWaitingID)
		end
	end
end

function TestLoginWaitCase:TearDown(case_data)
	SyncDisConnect(case_data.m_Conn)
	case_data.m_Conn = nil
end

return TestLoginWaitCase