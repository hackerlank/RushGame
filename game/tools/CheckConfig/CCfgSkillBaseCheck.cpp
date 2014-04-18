#include "stdafx.h"
#include "CCfgSkillBaseCheck.h"
#include "CCfgColChecker.inl"

CCfgSkillBaseCheck::SetRowName CCfgSkillBaseCheck::ms_setRowName;
string CCfgSkillBaseCheck::m_strTableName;

void CCfgSkillBaseCheck::Load(CTxtTableFile& TxtTableFile, const TCHAR* cfgFile, string strTableName)
{
	EndCheckOverlap();
	CfgChk::SetTabFile(TxtTableFile, strTableName);
	m_strTableName = strTableName;
	if (!TxtTableFile.Load(PATH_ALIAS_CFG.c_str(), cfgFile))
	{
		stringstream ExpStr;
		ExpStr << " ���ñ�: " << strTableName << " ����ʧ�ܣ���鿴�ļ���[" << cfgFile << "]�Ƿ���ȷ�����ļ��Ƿ����";
		CfgChk::GenExpInfo(ExpStr.str());
	}
}

bool CCfgSkillBaseCheck::CheckOverlap(string strColName, int32 nRowNum)
{
	CfgChk::SetLineNo(nRowNum);
	string strName;
	CfgChk::ReadItem(strName, strColName.c_str(), CANEMPTY);
	trimend(strName);
	SetRowName::iterator iter = ms_setRowName.find(strName);
	if (iter != ms_setRowName.end())
	{
		stringstream ExpStr;
		ExpStr << " ���ñ�: " << m_strTableName << " �� " << nRowNum << " �е� " << strColName << "["<< strName << "]" << "�ظ�";
		CfgChk::GenExpInfo(ExpStr.str());
	}
	else
	{
		ms_setRowName.insert(strName);
	}

	return true;
}

void CCfgSkillBaseCheck::EndCheckOverlap()
{
	ms_setRowName.clear();
}

bool CCfgSkillBaseCheck::CheckExist(const string& strMagicName)
{
	SetRowName::iterator iter = ms_setRowName.find(strMagicName);
	if (iter == ms_setRowName.end())
	{
		stringstream ExpStr;
		ExpStr << " Υ��ħ������Լ��: " << m_strTableName << " [" << strMagicName << "] ������ ";
		CfgChk::GenExpInfo(ExpStr.str());
		return false;
	}
	return true;
}
