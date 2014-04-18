local LoginSucMsg = MsgTbl.LoginSucMsg
local DisConnMsg = MsgTbl.DisConnMsg
local CreatePlayerMsg = MsgTbl.CreatePlayerMsg
local LoginWaitingMsg = MsgTbl.LoginWaitingMsg
local UpdateWaitingMsg = MsgTbl.UpdateWaitingMsg
local RepeatLoginMsg = MsgTbl.RepeatLoginMsg
local RetEnterOtherServerMsg = MsgTbl.RetEnterOtherServerMsg

local login_key_num = 0

function CheckUser(Conn, UserName)
	assert(IsCppBound(Conn))
	assert(UserName)
	
	Conn.m_UserName = UserName
	
	local Password_to_MD5 = Conn.m_UserName .. " 111111"	
	--MD5
	local MD5Digester = CMd5Digester:new()
	MD5Digester:Begin()
	MD5Digester:Append(Password_to_MD5, string.len(Password_to_MD5))
	local PasswordMD5 = CreateCBufAcr( CreateCBuf(33) )
	MD5Digester:GetMD5Base16Str(PasswordMD5)
	local strPasswordMD5 = PasswordMD5:ToString()
	
	login_key_num = login_key_num + 1		
	Gac2Gas:CheckUser(Conn, Conn.m_UserName, strPasswordMD5, tostring(login_key_num),"","")
end


--�����¼
function LoginOtherServer(case_data)
	local Conn = case_data.m_Conn
	local sIP = Conn.m_IP 
	local Port = Conn.m_Port 
	local LoginType = Conn.m_LoginType
	local sKey = Conn.m_sKey 
	local LastMsgStr = Conn.m_LastMsgStr 
	
	local addr = CAddress:new()
	addr:SetAddress(sIP)
	addr:SetPort(tonumber(Port))	
	
	Conn = ReConnect(case_data, 1000, addr)
	assert(IsCppBound(Conn))
	local tempTime = GetProcessTime()
	Gac2Gas:CharEnterOtherServer(Conn, LoginType, sKey, LastMsgStr)
	if case_data.tempTime then
		--print(case_data.m_UserName,"������Ϣ���õ�ʱ��:",GetProcessTime()-case_data.tempTime)
	end
	local waited_msg = WaitForMsg(CreatePlayerMsg, DisConnMsg, RetEnterOtherServerMsg)
	if waited_msg == DisConnMsg then
		print(case_data.m_UserName,"���г�����ʱ��Ͽ�����")
		return false
	elseif waited_msg == RetEnterOtherServerMsg then
		print(case_data.m_UserName,"�ж����,��Ϊ�ӳ�,�յ����л�ԭ������Ϣ.")
		return false, 1
	else
		return true
	end
end

--login �����İ�װ
function Login(case_data, login_type)
	local Conn = case_data.m_Conn
	local user_name = case_data.m_UserName
	
	while true do
		local waited_msg = SyncCheckUser(Conn, user_name)
		if waited_msg == LoginSucMsg then
			break;
		elseif waited_msg == LoginWaitingMsg then
			while true do
				local msg = WaitForMsg(LoginSucMsg, UpdateWaitingMsg)
				if msg == LoginSucMsg then
					break
				end
			end			
			break
		else
			TestSleep(2000)
		end
	end

	--������ɫ��������Ϸ
	SyncGetCharList(Conn)
	if #Conn.m_RoleList == 0 then
		SyncCreateRole(Conn)
	else
		SelectChar(Conn)
		EnterGame(Conn, login_type)
	end
	
	--����ȵ�����CreatePlayerMsg��Ϣ��֤����ͬ����¼�ģ��������ǿ����¼
	--��������¼����
	local waited_msg = WaitForMsg(RetEnterOtherServerMsg, CreatePlayerMsg)
	if waited_msg == CreatePlayerMsg then
		return
	else
		local res = LoginOtherServer(case_data)
		if not res then
			return
		end
	end
	
	
end

--Ĭ�ϵ�������¼���̣���������������ǲ��Ը���¼�йصģ����Ƕ��������溯�������е�¼���˳�
function DefaultLoginFlow(case_data)
	local user_name = case_data.m_UserName
	
	local Conn = SyncConnect(ConnAddr)		
	print(user_name, Conn)
	case_data.m_Conn = Conn	
	Login(case_data, 0)
end

function DefaultLogoutFlow(case_data)
	SyncDisConnect(case_data.m_Conn)
	case_data.m_Conn = nil
end
