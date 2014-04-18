#include "stdafx.h"
#include "CNpcEnmityTargetFilterDataCheck.h"
#include "CTxtTableFile.h"
#include "CCfgColChecker.inl"
//#include "CCfgSkillCheck.h"
#include "CCfgNpcServerBaseData.h"
#include "CScriptAppServer.h"

CNpcEnmityTargetFilterDataCheck::CNpcEnmityTargetFilterDataCheck()
{

}

CNpcEnmityTargetFilterDataCheck::~CNpcEnmityTargetFilterDataCheck()
{

}

void CNpcEnmityTargetFilterDataCheck::GetNpcArgs(SetAllNpc& lstNpcArgs, string& strNpcArgs)
{
	if (strNpcArgs.empty())
		return;
	erase(strNpcArgs, " ");
	erase(strNpcArgs,"\"");
	replace(strNpcArgs, "��", ",");
	while(true)
	{
		uint32 upoint = strNpcArgs.find(",");
		if (upoint != -1)
		{
			string sNpcName = strNpcArgs.substr(0, upoint);
			lstNpcArgs.insert(sNpcName);
			strNpcArgs = strNpcArgs.substr(upoint+1, strNpcArgs.length());
		}
		else
		{
			lstNpcArgs.insert(strNpcArgs);
			break;
		}
	}
}

bool CNpcEnmityTargetFilterDataCheck::Check()
{
	string strFiler = CScriptAppServer::Inst()->GetCfgFilePath("");
	strFiler = strFiler + "/npc/NpcEnmiyTargetFilter.txt";
	CTxtTableFile TableFile;
	Load(TableFile, strFiler.c_str(), "NpcEnmiyTargetFilter");

	SetAllNpc setAllNpc;

	for(int32 i = 1; i < TableFile.GetHeight(); ++i)
	{
		string sNpcName				= TableFile.GetString(i,"Name");
		string sTargetType			= TableFile.GetString(i, "TargetType");
		string sNpcArgs				= TableFile.GetString(i, "TargetArg");
		
		if (setAllNpc.find(sNpcName) != setAllNpc.end())
		{
			stringstream ExpStr;
			ExpStr << "�ڡ�NpcEnmiyTargetFilter������Npc:��"<<sNpcName<<"����д�ظ������ʵ��";
			CfgChk::GenExpInfo(ExpStr.str());
		}
		else
			setAllNpc.insert(sNpcName);

		if (!CCfgNpcServerBaseData::FindNpc(sNpcName))
		{
			stringstream ExpStr;
			ExpStr << "�ڡ�NpcEnmiyTargetFilter������Npc:��"<<sNpcName<<"���ڡ�Npc_Common�����в����ڣ����ʵ��";
			CfgChk::GenExpInfo(ExpStr.str());
			continue;
		}
		if (sTargetType == "����Ŀ��" || sTargetType == "�������" || sTargetType == "����Npc")
		{
			continue;
		}
		else if (sTargetType == "ָ��Npc")
		{
			SetAllNpc lstNpcArgs;
			GetNpcArgs(lstNpcArgs, sNpcArgs);
			for (SetAllNpc::iterator iter = lstNpcArgs.begin(); iter != lstNpcArgs.end(); iter ++)
			{
				bool bFind = CCfgNpcServerBaseData::FindNpc(*iter);
				if (!bFind)
				{
					stringstream ExpStr;
					ExpStr << "�ڡ�NpcEnmiyTargetFilter������Npc:��"<<sNpcName<<"���Ĳ���Npc��"<<(*iter)<<"���ڡ�Npc_Common�����в����ڣ����ʵ��";
					CfgChk::GenExpInfo(ExpStr.str());
				}
			}
		}
		else
		{
			stringstream ExpStr;
			ExpStr << "�ڡ�NpcEnmiyTargetFilter������Npc:��"<<sNpcName<<"����Ŀ�����͡�"<<sTargetType<<"�������ڣ����ʵ��";
			CfgChk::GenExpInfo(ExpStr.str());
		}
	}
	return true;
}
