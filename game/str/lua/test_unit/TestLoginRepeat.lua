gac_gas_require "scene_mgr/SceneCfg"

local LoginSucMsg = MsgTbl.LoginSucMsg
local LoginFailedMsg = MsgTbl.LoginFailedMsg
local RepeatLoginMsg = MsgTbl.RepeatLoginMsg
local LoginWaitingMsg = MsgTbl.LoginWaitingMsg
local UpdateWaitingMsg = MsgTbl.UpdateWaitingMsg

local CreateRoleSucMsg = MsgTbl.CreateRoleSucMsg 
local CreateRoleFailedMsg = MsgTbl.CreateRoleFailedMsg
local GetCharListMsg = MsgTbl.GetCharListMsg
local CreatePlayerMsg = MsgTbl.CreatePlayerMsg

local ChangeSceneSucMsg = MsgTbl.ChangeSceneSucMsg
local ChangeSceneFailedMsg = MsgTbl.ChangeSceneFailedMsg
local RetEnterOtherServerMsg = MsgTbl.RetEnterOtherServerMsg
local ConnSucMsg = MsgTbl.ConnSucMsg
local ConnFailedMsg = MsgTbl.ConnFailedMsg
local DisConnMsg = MsgTbl.DisConnMsg
local ConnErrorMsg = MsgTbl.ConnErrorMsg

--[[
	����ѹ�����ԣ���������Ҫ���ԵĲ�ͬ�ͻ�����ͬһ���ʺŵ�¼�����
	������������caseҪ��ȫ��������д�������������Ĳ���������һ��
--]]

TestLoginRepeatCase = TestCase("TestLoginRepeat")

local LoginNum = 1
function TestLoginRepeatCase:SetUp(case_data)
	if LoginNum % 2 == 0 then
		case_data.m_UserName = LoginNum .. BaseUserName
	else
		case_data.m_UserName = (LoginNum + 1) .. BaseUserName
	end
	LoginNum = LoginNum + 1
end

local function GetWaitTime()
	return math.random(1, 10) * 1000
end

--��Ϊ���ŵ�״̬�ܶ࣬���������������state pattern

BaseState = class()

CheckUserState = class(BaseState)

function CheckUserState:Handle(case_data)
	local Conn = case_data.m_Conn
	local user_name = case_data.m_UserName
	
	CheckUser(Conn, user_name)
	
	local msg = WaitForMsg(LoginSucMsg, LoginWaitingMsg, RepeatLoginMsg, DisConnMsg)
	
	if msg == LoginSucMsg then
		case_data.m_RunningState = LoginSucState:new()
	elseif msg == LoginWaitingMsg then
		case_data.m_RunningState = LoginWaitingState:new()
	elseif msg == RepeatLoginMsg then
		case_data.m_RunningState = RepeatLoginState:new()
	else
		case_data.m_RunningState = DisConnState:new()
	end
end

local DisConnRet = -1

DisConnState = class(BaseState)

function DisConnState:Handle(case_data)
	case_data.m_Conn = nil
	TestSleep(GetWaitTime())
	return DisConnRet
end


RepeatLoginState = class(BaseState)

function RepeatLoginState:Handle(case_data)
	SyncDisConnect(case_data.m_Conn)
	case_data.m_Conn = nil
	
	case_data.m_RunningState = DisConnState:new()
end


LoginWaitingState = class(BaseState)

function LoginWaitingState:Handle(case_data)
	while true do
		local msg = WaitForMsg(LoginSucMsg, UpdateWaitingMsg, DisConnMsg)
		if msg == LoginSucMsg then
			case_data.m_RunningState = LoginSucState:new()
			return 
		elseif msg == DisConnMsg then
			case_data.m_RunningState = DisConnState:new()
			return 	
		end
	end
	return 
end

LoginSucState = class(BaseState)

function LoginSucState:Handle(case_data)
	local Conn = case_data.m_Conn

	GetCharList(Conn)
	local msg = WaitForMsg(GetCharListMsg, DisConnMsg)
	if msg == DisConnMsg then
		case_data.m_RunningState = DisConnState:new()
		return 
	end
	
	if #Conn.m_RoleList == 0 then
		CreateRole(Conn)
		local msg = WaitForMsg(CreateRoleSucMsg, DisConnMsg)
		if msg == DisConnMsg then
			case_data.m_RunningState = DisConnState:new()
			return
		end	
	else
		SelectChar(Conn)
		EnterGame(Conn, login_type)
	end
	
	local waited_msg = WaitForMsg(RetEnterOtherServerMsg, CreatePlayerMsg, DisConnMsg)
	if waited_msg == DisConnMsg then
		case_data.m_RunningState = DisConnState:new()
		return
	end

	if waited_msg == CreatePlayerMsg then
		case_data.m_RunningState = ChangeSceneState:new()
	else
		case_data.m_RunningState = LoginOtherState:new()
	end
end

LoginOtherState = class(BaseState)

function LoginOtherState:Handle(case_data)
	local Conn = case_data.m_Conn
	local sIP = Conn.m_IP 
	local Port = Conn.m_Port 
	local LoginType = Conn.m_LoginType
	local sKey = Conn.m_sKey 
	local LastMsgStr = Conn.m_LastMsgStr 
	
	local addr = CAddress:new()
	addr:SetAddress(sIP)
	addr:SetPort(tonumber(Port))	
	
	SyncDisConnect(Conn)
	case_data.m_Conn = nil	

	TestSleep(1000)

	Conn = SyncConnect(addr)
	case_data.m_Conn = Conn
	
	Gac2Gas:CharEnterOtherServer(Conn, LoginType, sKey, LastMsgStr)
	local msg = WaitForMsg(CreatePlayerMsg, DisConnMsg, RetEnterOtherServerMsg)
	
	if msg == CreatePlayerMsg then
		case_data.m_RunningState = ChangeSceneState:new()
	elseif msg == DisConnMsg then
		case_data.m_RunningState = DisConnState:new()	
	else
		--��ΪRetEnterOtherServerMsg�Ƿ��������첽�ģ��������ǿ��ܻ��յ���һ������֮ǰ���͵��г�������Ϣ
		case_data.m_RunningState = LoginOtherState:new()
	end
end

ChangeSceneState = class(BaseState)

local Main_Scene = {
	"����1",
	"���Ե�ͼ",
}

local AllSceneNum = #Main_Scene
assert((AllSceneNum) ~= 0)

local SceneNum  = 1
function ChangeSceneState:Handle(case_data)
	local Conn = case_data.m_Conn

	local scene_name = Main_Scene[SceneNum]
	SceneNum = SceneNum + 1
	if SceneNum > AllSceneNum then
		SceneNum = 1
	end
	
	Gac2Gas:GM_Execute( Conn, "$changescene(\""  .. scene_name .."\", 5, 5)" )
	
	local waited_msg = WaitForMsg(RetEnterOtherServerMsg, ChangeSceneSucMsg, DisConnMsg)
	if waited_msg == ChangeSceneSucMsg then
		SyncDisConnect(case_data.m_Conn)
		case_data.m_Conn = nil
		case_data.m_RunningState = DisConnState:new()
	elseif waited_msg == RetEnterOtherServerMsg then
		case_data.m_RunningState = LoginOtherState:new()
	else	
		case_data.m_RunningState = DisConnState:new()	
	end

end



function TestLoginRepeatCase:test_RepeatLogin(case_data)
	local Conn = SyncConnect(ConnAddr)
	case_data.m_Conn = Conn

	case_data.m_RunningState = CheckUserState:new()
	
	while true do		
		local ret = case_data.m_RunningState:Handle(case_data)
		if ret == DisConnRet then
			break
		end
	end

	SyncDisConnect(case_data.m_Conn)
	case_data.m_Conn = nil
end


function TestLoginRepeatCase:TearDown(case_data)
	SyncDisConnect(case_data.m_Conn)
	case_data.m_Conn = nil
end

