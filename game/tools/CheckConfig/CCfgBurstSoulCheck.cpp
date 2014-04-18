#include "stdafx.h"
#include "CCfgAllMagicCheck.h"
#include "CTxtTableFile.h"
#include "CCfgColChecker.inl"
#include "LoadSkillCommon.h"

CCfgBurstSoulCheck::SetRowName CCfgBurstSoulCheck::ms_setRowName;
CCfgBurstSoulCheck::MapBurstSoulType CCfgBurstSoulCheck::ms_mapBurstSoulType;

bool CCfgBurstSoulCheck::Check(const TCHAR* cfgFile)
{
	using namespace CfgChk;

	InitMapBurstSoulType();
	CTxtTableFile TableFile;
	SetTabFile(TableFile, "����");
	if (!TableFile.Load(PATH_ALIAS_CFG.c_str(), cfgFile))
	{
		stringstream ExpStr;
		ExpStr << "���ñ� ����ʧ�ܣ���鿴�ļ���[" << cfgFile << "]�Ƿ���ȷ�����ļ��Ƿ����";
		GenExpInfo(ExpStr.str());
	}

	static const uint32 s_uTableFileWide = 7;
	if (TableFile.GetWidth() != s_uTableFileWide)
	{
		stringstream ExpStr;
		ExpStr << "���ñ� �������� ӦΪ: " << s_uTableFileWide << " �У�ʵΪ: " << TableFile.GetWidth() << " ��";
		GenExpInfo(ExpStr.str());
	}

	CCfgCalc* pCountCacl = NULL;
	CCfgCalc* pProbabilityCacl = NULL;
	for(int32 i = 1; i < TableFile.GetHeight(); ++i)
	{
		SetLineNo(i);
		string strName;
		ReadItem(strName, szBurstSoul_DoubleBurst, CANEMPTY);
		trimend(strName);
		SetRowName::iterator iter = ms_setRowName.find(strName);
		if (iter != ms_setRowName.end())
		{
			stringstream ExpStr;
			ExpStr << " ���ñ�: " << g_sTabName << " �� " << i << " �е� " << szBurstSoul_DoubleBurst << "["<< strName << "]" << "�ظ�";
			GenExpInfo(ExpStr.str());
		}
		else
		{
			ms_setRowName.insert(strName);
		}

		string strMagicEff;
		ReadItem(strMagicEff, szSkill_MagicEff, CANEMPTY);
		CCfgMagicEffCheck::CheckMagicEffExist(strMagicEff);

		string	strBurstSoulType;
		ReadItem(strBurstSoulType,	szBurstSoul_Type);
		if(!ms_mapBurstSoulType[strBurstSoulType])
		{
			stringstream ExpStr;
			ExpStr << " �������Ͳ�����: " << strBurstSoulType ;
			CfgChk::GenExpInfo(ExpStr.str());
		}
		ReadItem(pCountCacl, szBurstSoul_Count);
		pCountCacl->SetTypeExpression();
		ReadItem(pProbabilityCacl, szBurstSoul_Probability);
		pProbabilityCacl->SetTypeExpression();
		delete pCountCacl;
		delete pProbabilityCacl;

		string strBulletMagicName;
		ReadItem(strBulletMagicName,	szBurstSoul_BulletMagicName);
		if (!CCfgBulletMagicCheck::CheckExist(strBulletMagicName))
		{
			CfgChk::GenExpInfo("�����ӵ�������, �鿴�ӵ�ħ����\n");
		}
	}
	return true;
}

void CCfgBurstSoulCheck::EndCheck()
{
	ms_setRowName.clear();
}

bool CCfgBurstSoulCheck::CheckExist(const string& strName)
{
	SetRowName::iterator iter = ms_setRowName.find(strName);
	if (iter == ms_setRowName.end())
	{
		stringstream ExpStr;
		ExpStr << " Υ��ħ������Լ��: ����" << " [" << strName << "] ������ ";
		CfgChk::GenExpInfo(ExpStr.str());
		return false;
	}
	return true;
}

bool CCfgBurstSoulCheck::CheckExistType(const string& strType)
{
	if(!ms_mapBurstSoulType[strType])
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ����[�Ծ���ӵ���߽����긽��]��������ȷ");
		return false;
	}
	return true;
}
