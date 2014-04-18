#include "stdafx.h"
#include "CCfgBaseCheck.h"
#include "CCfgColChecker.inl"

CCfgBaseCheck::SetRowName CCfgBaseCheck::ms_setRowName;

void CCfgBaseCheck::Load(CTxtTableFile& TxtTableFile, const TCHAR* cfgFile, string strTableName)
{
	EndCheckOverlap();
	CfgChk::SetTabFile(TxtTableFile, strTableName);
	if (!TxtTableFile.Load(PATH_ALIAS_CFG.c_str(), cfgFile))
	{
		stringstream ExpStr;
		ExpStr << " ���ñ�: " << strTableName << " ����ʧ�ܣ���鿴�ļ���[" << cfgFile << "]�Ƿ���ȷ�����ļ��Ƿ����";
		CfgChk::GenExpInfo(ExpStr.str());
	}
}

bool CCfgBaseCheck::CheckOverlap(string strColName, int32 nRowNum)
{
	CfgChk::SetLineNo(nRowNum);
	string strName;
	CfgChk::ReadItem(strName, strColName.c_str(), CANEMPTY);
	trimend(strName);
	SetRowName::iterator iter = ms_setRowName.find(strName);
	if (iter != ms_setRowName.end())
	{
		stringstream ExpStr;
		ExpStr << " ���ñ�: " << CfgChk::g_sTabName << " �� " << nRowNum << " �е� " << strColName << "["<< strName << "]" << "�ظ�";
		CfgChk::GenExpInfo(ExpStr.str());
	}
	else
	{
		ms_setRowName.insert(strName);
	}

	return true;
}

void CCfgBaseCheck::EndCheckOverlap()
{
	ms_setRowName.clear();
}	
