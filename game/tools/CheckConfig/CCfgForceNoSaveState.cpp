#include "stdafx.h"
#include "CCfgForceNoSaveState.h"
#include "CCfgColChecker.inl"
#include "LoadSkillCommon.h"
#include "CCfgAllStateCheck.h"

namespace sqr
{
	extern const wstring PATH_ALIAS_CFG;
}

CForceNoSaveStateCfgServer::SetRowName CForceNoSaveStateCfgServer::ms_setRowName;

bool CForceNoSaveStateCfgServer::Check(const TCHAR* cfgFile)
{
	using namespace CfgChk;

	CTxtTableFile TabFile;
	SetTabFile(TabFile, "ǿ�ƴ治�����ݿ��״̬");
	if (!TabFile.Load(PATH_ALIAS_CFG.c_str(), cfgFile)) return false;

	string sName;
	for(int32 i=1; i<TabFile.GetHeight(); ++i)
	{
		SetLineNo(i);
		ReadItem(sName,	szGlobalState_Name);
		string sSave;
		ReadItem(sSave,	szState_ForceSaveOrForceNoSave);

		CCfgAllStateCheck* pGlobalCfg = CCfgAllStateCheck::GetStateByName(sName);
		if(pGlobalCfg)
		{
			if(sSave == "���ߴ�" || sSave == "��ʱ��" )
			{
				pGlobalCfg->m_bNeedSaveToDB = true;
			}
			else if (sSave == "����")
			{
				pGlobalCfg->m_bNeedSaveToDB = false;
			}
			else
			{
				stringstream ExpStr;
				ExpStr << " ���ñ�: " << g_sTabName << " �� " << i << " �е� " << szState_ForceSaveOrForceNoSave << "["<< sSave << "]" << "�����Ϲ���";
				GenExpInfo(ExpStr.str());
			}
		}
		SetRowName::iterator iter = ms_setRowName.find(sName);
		if (iter != ms_setRowName.end())
		{
			stringstream ExpStr;
			ExpStr << " ���ñ�: " << g_sTabName << " �� " << i << " �е� " << szMagicState_Name << "["<< sName << "]" << "�ظ�";
			GenExpInfo(ExpStr.str());
		}
		else
		{
			ms_setRowName.insert(sName);
		}
	}

	return true;
}

void CForceNoSaveStateCfgServer::EndCheck()
{
}

