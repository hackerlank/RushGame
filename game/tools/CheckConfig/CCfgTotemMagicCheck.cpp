#include "stdafx.h"
#include "CCfgAllMagicCheck.h"
#include "CTxtTableFile.h"
#include "CCfgColChecker.inl"
#include "LoadSkillCommon.h"

CCfgTotemMagicCheck::MapTotemMagic CCfgTotemMagicCheck::ms_mapTotemMagic;

CCfgTotemMagicCheck::CCfgTotemMagicCheck()
:m_strName("")
,m_pTouchMagicEff(NULL)
{
}

CCfgTotemMagicCheck::~CCfgTotemMagicCheck()
{
}

bool CCfgTotemMagicCheck::Check(const TCHAR* cfgFile)
{
	using namespace CfgChk;

	CTxtTableFile TableFile;
	SetTabFile(TableFile, "ͼ��ħ��");
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

	for(int32 i = 1; i < TableFile.GetHeight(); ++i)
	{
		SetLineNo(i);
		string strName, strTouchEff;
		ReadItem(strName, szMagic_Name, CANEMPTY);
		ReadItem(strTouchEff, szMagic_TouchMagicEff, CANEMPTY);
		CCfgMagicEffCheck::CheckMagicEffExist(strTouchEff);
		trimend(strName);
		MapTotemMagic::iterator iter = ms_mapTotemMagic.find(strName);
		if (iter != ms_mapTotemMagic.end())
		{
			stringstream ExpStr;
			ExpStr << " ���ñ�: " << g_sTabName << " �� " << i << " �е� " << szMagic_Name << "["<< strName << "]" << "�ظ�";
			GenExpInfo(ExpStr.str());
		}
		else
		{
			CCfgTotemMagicCheck* pTotem = new CCfgTotemMagicCheck;
			pTotem->m_strName = strName;
			if (!strTouchEff.empty() && CCfgMagicEffCheck::CheckMagicEffExist(strTouchEff))
			{
				CCfgMagicEffCheck::MapMagicEff::iterator itr = CCfgMagicEffCheck::ms_mapMagicEff.find(strTouchEff);
				pTotem->m_pTouchMagicEff = (*itr).second;
			}
			ms_mapTotemMagic.insert(make_pair(strName, pTotem));
		}

		string strDotEff, strFinalEff;
		ReadItem(strDotEff,		szMagic_DotMagicEff,	CANEMPTY);
		CCfgMagicEffCheck::CheckMagicEffExist(strDotEff);
		ReadItem(strFinalEff,	szMagic_FinalMagicEff,	CANEMPTY);
		CCfgMagicEffCheck::CheckMagicEffExist(strFinalEff);

		if ( (!strTouchEff.empty() && !strDotEff.empty() && strTouchEff == strDotEff) ||
			(!strTouchEff.empty() && !strFinalEff.empty() && strTouchEff == strFinalEff) ||
			(!strDotEff.empty() && !strFinalEff.empty() && strDotEff == strFinalEff) )
		{
			stringstream str;
			str << "\n��" << i << "��Υ����Լ����ͬһ�еĲ�ͬħ��Ч������ͬ��";
			GenExpInfo(str.str());
		}
	}
	return true;
}

void CCfgTotemMagicCheck::EndCheck()
{
	ClearMap(ms_mapTotemMagic);
}

bool CCfgTotemMagicCheck::CheckExist(const string& strName)
{
	MapTotemMagic::iterator iter = ms_mapTotemMagic.find(strName);
	if (iter == ms_mapTotemMagic.end())
	{
		stringstream ExpStr;
		ExpStr << " Υ��ħ������Լ��: ͼ��ħ��" << " [" << strName << "] ������ ";
		CfgChk::GenExpInfo(ExpStr.str());
		return false;
	}
	return true;
}
