#include "stdafx.h"
#include "CCfgNPCResCommonCheck.h"
#include "CCfgColChecker.inl"

CCfgNPCResCommonCheck::SetNpcName CCfgNPCResCommonCheck::ms_setNpcName;

void CCfgNPCResCommonCheck::Check(const TCHAR* sNpcName, float fBaseSize, bool bAdhereTerrain, const TCHAR* sAniFileName)
{
	if (sNpcName == NULL)
	{
		stringstream ExpStr;
		ExpStr << "�ڡ�NpcRes_Common�����С�NpcName�����Ϊ�գ����ʵ��";
		CfgChk::GenExpInfo(ExpStr.str());
	}

	if (fBaseSize <= 0)
	{
		stringstream ExpStr;
		ExpStr << "�ڡ�NpcRes_Common�����нС�" << sNpcName<< "����Npc��Scaling����BaseSize��������붼����0�����ʵ��";
		CfgChk::GenExpInfo(ExpStr.str());
	}

	SetNpcName::iterator iter = ms_setNpcName.find(sNpcName);
	if (iter != ms_setNpcName.end())
	{
		stringstream ExpStr;
		ExpStr << "�ڡ�NpcRes_Common�����нС�" << sNpcName << "����Npc�ظ������ʵ��" ;
		CfgChk::GenExpInfo(ExpStr.str());
	}
	else
	{
		ms_setNpcName.insert(sNpcName);
	}
}

void CCfgNPCResCommonCheck::EndCheck()
{
	ms_setNpcName.clear();
}

bool CCfgNPCResCommonCheck::BeExist(string sNpcName)
{
	if (ms_setNpcName.find(sNpcName) != ms_setNpcName.end())
		return true;
	return false;
}


