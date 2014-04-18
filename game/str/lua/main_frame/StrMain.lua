engine_require "common/EngineExport"
engine_require "common/Loader/loader"

str_require "main_frame/MsgHelper"
str_require "main_frame/MsgDefs"

str_require "rpc/Gas2StrDef"
etc_str_require "StrConfig"
engine_require "common/Script/TimeHelper"
str_require "rpc/Db2Gac"

str_require "main_frame/ConnMgrStressHandler"
str_require "main_frame/ConnHelper"
str_require "main_frame/CharHelper"
str_require "main_frame/SleepHelper"
str_require "main_frame/LoginHelper"

str_require "test_unit/TestRun"

--ȫ�ֱ�����ʼ��
	
MaxConnNum = StrConfig.MaxUserNum 	--���������Ŀ 
BaseUserName = StrConfig.UserName  --Ĭ�ϵ�¼���ʺ�����

function StartUp()

	local CAppStressHandler = class(IAppStressHandler)
	function CAppStressHandler:OnClose()
		print("OnClose")
		
		if not TestRunEnabled then
			db()
			g_App:Quit()
		end

		TestRunEnabled = false
		
		g_App.m_bQuit = true
		
		if not next(RunningCaseTbl) then
			g_App:Quit()			
		else
			for _,casedata in pairs(RunningCaseTbl) do
				SyncDisConnect(casedata.m_Conn, casedata.m_UserName)
			end
		end
		
	end

	--Stress Handlerע��
	g_App:SetHandler(CAppStressHandler:new())
	
	--load rpc��Э���
	ConnMgrStress = g_App:GetConnMgr()

	StressHander = CConnMgrStressHandler:new()
	
	ScriptConnMgrStressHandler = CScriptConnMgrStressHandler:new()
	ScriptConnMgrStressHandler:RegistCall("gac_gas", "framework/rpc/Gac2GasProList","CConnStress")
	ScriptConnMgrStressHandler:RegistDef("gac_gas", "framework/rpc/Gas2GacProList","CConnStress")
	StressHander:SetScriptHandler(ScriptConnMgrStressHandler)
	
	ConnMgrStress:SetHandler(StressHander)
	
	ConnAddr = CAddress:new()
	ConnAddr:SetAddress(StrConfig.ServerAddress)
	ConnAddr:SetPort(StrConfig.ServerPort)

	local test_case_num = #(g_TestCaseFun)
	if test_case_num == 0 then
		print("No Test Case, We Will Exit!")
	end

	for i=1,MaxConnNum do
		local co = coroutine.create(TestRun)
		
		--���ѡ��һ��test case������
		local index = math.random(1, test_case_num)
		
		coroutine.resume(co, index,i)		
	end
end


function CleanUp()
	OutputTestResult()
end
