local ChangeSceneSucMsg = MsgTbl.ChangeSceneSucMsg
local ChangeSceneFailedMsg = MsgTbl.ChangeSceneFailedMsg
local RetEnterOtherServerMsg = MsgTbl.RetEnterOtherServerMsg
local ReadyGoFbMsg = MsgTbl.ReadyGoFbMsg
local SetLevelMsg = MsgTbl.SetLevelMsg
local ClearActionTimesMsg = MsgTbl.ClearActionTimesMsg
local ExitMatchGameMsg = MsgTbl.ExitMatchGameMsg
local ActionAllReadyMsg = MsgTbl.ActionAllReadyMsg

local gameName = "����ػ�������"


local TestMatchGameFbCase = TestCase("TestMatchGameFb")


local function PrintInfo(...)
	print(...)
end

local function LoopFun(case_data)
	local UserName = case_data.m_UserName
	case_data.m_IsGiveUp = false
	
	WaitForMsg(ReadyGoFbMsg)
	--PrintInfo(4,UserName,case_data.m_aa)
	local randnum = math.random(1,100)
	if randnum > 20 then
		StressRpcToServer("AgreeJoinFbAction",case_data.m_Conn, gameName)
	else
		case_data.m_IsGiveUp = true
		StressRpcToServer("NotJoinFbAction",case_data.m_Conn, gameName)
	end
	--PrintInfo(5,UserName,case_data.m_aa,case_data.m_IsGiveUp)
	
	local waitedmsg = WaitForMsg(ActionAllReadyMsg, ExitMatchGameMsg, RetEnterOtherServerMsg, ChangeSceneSucMsg)
	if waitedmsg == RetEnterOtherServerMsg then
		local res,num = LoginOtherServer(case_data)
		if not res and num == 1 then
			LoginOtherServer(case_data)
			return false
		end
	elseif waitedmsg == ActionAllReadyMsg then
		StressRpcToServer("AutoJoinActio",case_data.m_Conn, case_data.m_Conn.m_ServerId, case_data.m_Conn.m_RoomId, case_data.m_Conn.m_ActionName)
		local msg = WaitForMsg(RetEnterOtherServerMsg, ChangeSceneSucMsg)
		if msg == RetEnterOtherServerMsg then
			local res,num = LoginOtherServer(case_data)
			if not res and num == 1 then
				LoginOtherServer(case_data)
				return false
			end
		end
	elseif waitedmsg == ExitMatchGameMsg then
		if case_data.m_IsGiveUp then
			PrintInfo(UserName,"��������,�����븱��")
			return false
		else
			if not LoopFun(case_data) then
				return false
			end
		end
	end
	return true
end

function TestMatchGameFbCase:test_MatchGameFb(case_data)
	local isAutoTeam = false--�Ƿ�Ҫ�Զ���ӵĿ���
	local UserName = case_data.m_UserName
	
	PrintInfo(UserName, "ǰ���ı�������:", gameName)
	if not case_data.m_aa then
		case_data.m_aa = 0
	end
	case_data.m_aa = case_data.m_aa + 1
	--PrintInfo(1,UserName,case_data.m_aa)
	StressRpcToServer("GM_Execute",case_data.m_Conn, "$addlevel(80)" )
	WaitForMsg(SetLevelMsg)
	--PrintInfo(2,UserName,case_data.m_aa)
	StressRpcToServer("GM_Execute",case_data.m_Conn, "$clearlimitcount()" )
	WaitForMsg(ClearActionTimesMsg)
	--PrintInfo(3,UserName,case_data.m_aa)
	StressRpcToServer("JoinMatchGame",case_data.m_Conn, gameName, isAutoTeam)
	
	if not LoopFun(case_data) then
		return
	end
	
	PrintInfo(UserName,"�յ�������Ϣ,���븱��")
	
	--TestSleep(10000)
	--������
	--StressRpcToServer("GM_Execute", case_data.m_Conn, "$changescene(\"����1\", 5, 5)" )
	StressRpcToServer("NotJoinFbAction",case_data.m_Conn,gameName)
	
	PrintInfo(UserName,"�˳�����1")
	local waited_msg = WaitForMsg(ExitMatchGameMsg,RetEnterOtherServerMsg, ChangeSceneSucMsg)
	PrintInfo(UserName,"1�˳�����",waited_msg)
	if waited_msg == RetEnterOtherServerMsg then
		local res = LoginOtherServer(case_data)
	end
	PrintInfo(UserName,"�˳�����2")
	local waited_msg = WaitForMsg(ExitMatchGameMsg,RetEnterOtherServerMsg, ChangeSceneSucMsg)
	PrintInfo(UserName,"2�˳�����",waited_msg)
	if waited_msg == RetEnterOtherServerMsg then
		local res = LoginOtherServer(case_data)
	end
	
	PrintInfo(UserName,"��ȫ�˳�����")
end

return TestMatchGameFbCase
