engine_require "common/Script/ScriptLib"
engine_require "common/Script/ExportClass"
etc_com_require "DevConfig"

CoreStress={}
p=CoreStress


local function Run( FileName, uCyc )
	EnableExportClass(DevConfig.ExportClass == 1)

	CoreStress=nil
	Run=nil
	
	--����Client������
	local lStress=CBaseAppStress_Inst()
	
	--�Ѹö���¶����Ϸ�߼�
	g_App=lStress;
	
	--������Ϸ�߼�����������ļ�
	local uTime=GetProcessTime()
	
	Require(FileName)
	
	uTime = GetProcessTime() - uTime
	
	print( "script load time:" ..tostring(uTime) );
	
	StartUp()
	
	lStress:Run()
	
	CleanUp()
	
	g_App=nil
end


CoreStress=
{
	Run = Run
}

