engine_require "common/Script/ScriptLib"
engine_require "common/Script/ExportClass"
etc_com_require "DevConfig"

CoreClient={}
p=CoreClient

local function Run( FileName, uCyc )
	EnableCppMemLeakCheck(DevConfig.CheckMemLeak == 1)
	EnableExportClass(DevConfig.ExportClass == 1)
	
	CoreClient=nil
	Run=nil
	
	--����Client������
	local lClient=CBaseAppClient_Inst()
	
	--�Ѹö���¶����Ϸ�߼�
	g_App=lClient;
	g_ScriptApp=CBaseScriptApp_Inst()
	
	--������Ϸ�߼�����������ļ�
	local uCounter=0;
	
	local r=require
	local nr=function( modname )
		uCounter = uCounter + 1
		return r(modname)
	end
	
	_G.require=nr
	
	--�����������ļ�֮ǰ�Ȱ�ȫ�ֿռ�����Ķ�����ӵ�leak filter����ȥ
	--�������ǽ����ڴ�й¶������ȫ�ֿռ��ʱ����Щ����Ͳ������
	for k, v in pairs(_G) do
		AddCheckLeakFilterObj(v)
	end
	
	print("Main VM begin loading script files.")
	
	local uTime=GetProcessTime()	
	Require(FileName)	
	uTime = GetProcessTime() - uTime
	
	_G.require=r;	
	
	print( "Main VM loaded " .. tostring(uCounter) .. " script files in " .. tostring(uTime) .. " milliseconds." )
	
	StartUp()
	
	g_App:GetMainWnd():Show(true)
	
	lClient:Run()
	
	CleanUp()
	
	g_App=nil
	
	collectgarbage("collect")
	collectgarbage("collect")
	
	if DevConfig.CheckMemLeak == 1 then
		CheckCppObjMemLeak()
		TraverseMemCheckTable()
	end
end


CoreClient=
{
	Run = Run
}

