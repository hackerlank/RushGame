require "common/Script/ScriptLib"

CoreServer={}
p=CoreServer

local function Run( FileName, uCyc )
	CoreServer=nil
	Run=nil

	--����Server������
	local lServer=CBaseAppServer_Inst()
	
	--�Ѹö���¶����Ϸ�߼�
	g_App=lServer;
	g_ScriptApp=CBaseScriptApp_Inst()
	
	--������Ϸ�߼�����������ļ�
	
	local uTime=GetProcessTime()
	require(FileName)
	uTime = GetProcessTime() - uTime
	print( "Load script used " .. tostring(uTime) .. " milliseconds." )
	
	StartUp()
	
	lServer:Run()
	
	CleanUp()  
	
	g_App=nil
	
	
	print("ȫ�ֶ�����Ŀ��" .. getNumOfObjTable() .. "    ���ݸ�lua������Ŀ��" .. getNumOfCppTable())
	--db()

end

CoreServer=
{
	Run = Run
}

