cfg_load "fb_game/AreaFb_Common"

local ChangeSceneSucMsg = MsgTbl.ChangeSceneSucMsg
local ChangeSceneFailedMsg = MsgTbl.ChangeSceneFailedMsg
local RetEnterOtherServerMsg = MsgTbl.RetEnterOtherServerMsg

--[[
local AreaFbTbl = {}
for _,name in pairs(AreaFb_Common:GetKeys()) do
	table.insert(AreaFbTbl, name)
end
--]]
local AreaFbTbl = {
"��ѩ֮��",
"����ԭҰ",
}

local AreaFbNum = #AreaFbTbl

TestFbSceneCase = TestCase("TestFbScene")

function TestFbSceneCase:test_FbScene(case_data)
	local FbName = AreaFbTbl[math.random(1, AreaFbNum)]
	local IsHard = math.random(1, 2) == 1
	--print(FbName)
	Gac2Gas:AgreedJoinAreaFb(case_data.m_Conn,FbName,IsHard)
	
	local waited_msg = WaitForMsg(RetEnterOtherServerMsg, ChangeSceneSucMsg)
	if waited_msg == RetEnterOtherServerMsg then
		local res = LoginOtherServer(case_data)
		if not res then
			return
		end
	end
	--print("���븱��")
	--���ʱ���Ѿ�����������
	
	--������
	Gac2Gas:GM_Execute( case_data.m_Conn, "$changescene(\"����1\", 5, 5)" )
	
	waited_msg = WaitForMsg(RetEnterOtherServerMsg, ChangeSceneSucMsg)
	if waited_msg == RetEnterOtherServerMsg then
		local res = LoginOtherServer(case_data)
		if not res then
			return
		end
	end
	--print"�˳�����"
end