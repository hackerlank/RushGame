local ConnSucMsg = MsgTbl.ConnSucMsg
local count = 0
--��ͬ����ʽ����һ������
function SyncConnect(Addr)
	assert(Addr)
	--��������

	local Conn = ConnMgrStress:Connect(Addr)
	Conn.m_Co = coroutine.running()
	
	WaitForMsg(ConnSucMsg)
	
	return Conn
end

--�Ͽ�һ������
function SyncDisConnect(Conn, UserName)
	if not IsCppBound(Conn) then
		return	
	end
	if UserName then
		count = count + 1
		print(count,UserName)
	end
	Conn:ShutDown()
	return
end

--���ߣ��ȴ�һ��ʱ������
function ReConnect(case_data, time, addr)
	local Conn = case_data.m_Conn
	
	SyncDisConnect(Conn)
	case_data.m_Conn = nil
	
	TestSleep(time)
	
	Conn = SyncConnect(addr)
	case_data.m_Conn = Conn
	
	return Conn
end
