#include "stdafx.h"
#include "CCfgAllMagicCheck.h"
#include "CTxtTableFile.h"
#include "CCfgColChecker.inl"
#include "LoadSkillCommon.h"

CCfgPositionMagicCheck::MapPositionMagic CCfgPositionMagicCheck::ms_mapPositionMagic;

CCfgPositionMagicCheck::CCfgPositionMagicCheck()
:m_strName("")
,m_pTouchMagicEff(NULL)
,m_bMutex(false)
{
}

CCfgPositionMagicCheck::~CCfgPositionMagicCheck()
{
}

bool CCfgPositionMagicCheck::Check(const TCHAR* cfgFile)
{
	using namespace CfgChk;

	CTxtTableFile TableFile;
	SetTabFile(TableFile, "λ��ħ��");
	if (!TableFile.Load(PATH_ALIAS_CFG.c_str(), cfgFile))
	{
		stringstream ExpStr;
		ExpStr << "���ñ� ����ʧ�ܣ���鿴�ļ���[" << cfgFile << "]�Ƿ���ȷ�����ļ��Ƿ����";
		GenExpInfo(ExpStr.str());
	}

	static const uint32 s_uTableFileWide = 10;
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
		MapPositionMagic::iterator iter = ms_mapPositionMagic.find(strName);
		if (iter != ms_mapPositionMagic.end())
		{
			stringstream ExpStr;
			ExpStr << " ���ñ�: " << g_sTabName << " �� " << i << " �е� " << szMagic_Name << "["<< strName << "]" << "�ظ�";
			GenExpInfo(ExpStr.str());
		}
		else
		{
			CCfgPositionMagicCheck* pPosition = new CCfgPositionMagicCheck;
			pPosition->m_strName = strName;
			if (!strTouchEff.empty() && CCfgMagicEffCheck::CheckMagicEffExist(strTouchEff))
			{
				CCfgMagicEffCheck::MapMagicEff::iterator itr = CCfgMagicEffCheck::ms_mapMagicEff.find(strTouchEff);
				pPosition->m_pTouchMagicEff = (*itr).second;
			}
			string strMutextype;
			ReadItem(strMutextype, szMagic_MutexType, CANEMPTY);
			if (strMutextype == "��")
				pPosition->m_bMutex = true;

			ms_mapPositionMagic.insert(make_pair(strName, pPosition));
		}
		
		string strDotEff, strFinalEff;
		ReadItem(strDotEff,		szMagic_DotMagicEff,	CANEMPTY);
		if (!strDotEff.empty())
		{
			size_t comma = strDotEff.find(',');
			SetItemTitle(szMagic_DotMagicEff);
			if(comma != string::npos)
			{
				string strDotMagicEff = strDotEff.substr(0, comma);
				CCfgMagicEffCheck::CheckMagicEffExist(strDotMagicEff);
			}
		}
	
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

		// ��Ч�����
		string strFX = TableFile.GetString(i, szMagic_FxName);
		if (strFX != "")
		{
			vector<string> sFXTable = CCfgMagicOp::Split(strFX, ",");
			if (sFXTable.size() == 2)
			{
				string strFXName	= sFXTable[1];
				if (strFXName == "")
				{
					stringstream ExpStr;
					ExpStr << "��" << i << "�е� ��Ч�� " << strFX << " ����, ���ź��������Ч��!";
					GenExpInfo(ExpStr.str());
				}
			}
			else
			{
				stringstream ExpStr;
				ExpStr << "��" << i << "�е� ��Ч�� " << strFX << " ����, ����Ϊ���Ÿ���������!";
				GenExpInfo(ExpStr.str());
			}
		}
	}
	return true;
}

void CCfgPositionMagicCheck::EndCheck()
{
	ClearMap(ms_mapPositionMagic);
}

bool CCfgPositionMagicCheck::CheckExist(const string& strName)
{
	MapPositionMagic::iterator iter = ms_mapPositionMagic.find(strName);
	if (iter == ms_mapPositionMagic.end())
	{
		stringstream ExpStr;
		ExpStr << " Υ��ħ������Լ��: λ��ħ��" << " [" << strName << "] ������ ";
		CfgChk::GenExpInfo(ExpStr.str());
		return false;
	}
	return true;
}
