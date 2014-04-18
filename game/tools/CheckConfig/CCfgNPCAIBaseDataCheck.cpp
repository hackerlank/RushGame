#include "stdafx.h"
#include "CCfgNPCAIBaseDataCheck.h"
#include "CTxtTableFile.h"
#include "CCfgColChecker.inl"

CCfgNPCAIBaseDataCheck::MapAIBaseData		CCfgNPCAIBaseDataCheck::ms_mapAIBaseData;

bool CCfgNPCAIBaseDataCheck::Check(const TCHAR* cfgFile)
{
	CTxtTableFile TableFile;
	Load(TableFile, cfgFile, "NpcAIBaseData_Server");
	for(int32 i = 1; i < TableFile.GetHeight(); ++i)
	{
		CheckOverlap("Name", i);

		string sName = TableFile.GetString(i, "Name");
		uint32 uMinRandMoveTime, uMaxRandMoveTime;
		uMinRandMoveTime	= TableFile.GetInteger(i, "MinRandMoveTime", 0);
		uMaxRandMoveTime	= TableFile.GetInteger(i, "MaxRandMoveTime", 0);
		if (uMinRandMoveTime ==0 || uMaxRandMoveTime == 0)
		{
			ostringstream ExpStr;
			ExpStr << "��NpcAIBaseData_Server�����ñ��" << i << "�е�" << TableFile.GetString(i, "Name") << "�ġ�MinRandMoveTime����MaxRandMoveTime������������0��";
			CfgChk::GenExpInfo(ExpStr.str());
		}
		int32 uPatrolPointStayTime	= TableFile.GetInteger(i, "PatrolPointStayTime", -1);
		if (uPatrolPointStayTime < 0)
		{
			ostringstream ExpStr;
			ExpStr << "��NpcAIBaseData_Server�����ñ��" << i << "�е�" << TableFile.GetString(i, "Name") << "�ġ�PatrolPointStayTime��������Ǵ���0��������";
			CfgChk::GenExpInfo(ExpStr.str());
		}

		CCfgNPCAIBaseDataCheck* pCfg = new CCfgNPCAIBaseDataCheck();
		pCfg->m_uEyeSize = TableFile.GetInteger(i, "EyeSize", 0);

		MapAIBaseData::iterator iter = ms_mapAIBaseData.find(sName);
		if (iter != ms_mapAIBaseData.end())
		{
			ostringstream ExpStr;
			ExpStr << "��NpcFightBaseData_Server�����ñ�� " << i << " �е� "<<sName<<" �Ѿ�������д�ظ���";
			CfgChk::GenExpInfo(ExpStr.str());
		}
		else
		{
			ms_mapAIBaseData.insert(make_pair(sName, pCfg));
		}

	}
	return true;
}

void CCfgNPCAIBaseDataCheck::EndCheck()
{
	EndCheckOverlap();
}

bool CCfgNPCAIBaseDataCheck::BeExist(string sAIDataName)
{
	if (ms_mapAIBaseData.find(sAIDataName) != ms_mapAIBaseData.end())
		return true;
	return false;
}

uint32 CCfgNPCAIBaseDataCheck::GetEyeSizeByName(const TCHAR* szName)
{
	string sAIDataName(szName);
	MapAIBaseData::iterator iter = ms_mapAIBaseData.find(sAIDataName);
	if (iter != ms_mapAIBaseData.end())
	{
		return iter->second->m_uEyeSize;
	}
	else
	{
		return 0;
	}
}

