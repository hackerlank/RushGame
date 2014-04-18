#include "stdafx.h"
#include "CCfgAllMagicCheck.h"
#include "CTxtTableFile.h"
#include "CCfgColChecker.inl"
#include "LoadSkillCommon.h"

CCfgAureMagicCheck::MapAureMagic CCfgAureMagicCheck::ms_mapAureMagic;

CCfgAureMagicCheck::CCfgAureMagicCheck()
:m_strName("")
,m_bPersistent(false)
,m_pMagicEff(NULL)
{
}

CCfgAureMagicCheck::~CCfgAureMagicCheck()
{
}

bool CCfgAureMagicCheck::Check(const TCHAR* cfgFile)
{
	using namespace CfgChk;

	CTxtTableFile TableFile;
	SetTabFile(TableFile, "��̬����");
	if (!TableFile.Load(PATH_ALIAS_CFG.c_str(), cfgFile))
	{
		stringstream ExpStr;
		ExpStr << "���ñ� ����ʧ�ܣ���鿴�ļ���[" << cfgFile << "]�Ƿ���ȷ�����ļ��Ƿ����";
		GenExpInfo(ExpStr.str());
	}

	static const uint32 s_uTableFileWide = 8;
	if (TableFile.GetWidth() != s_uTableFileWide)
	{
		stringstream ExpStr;
		ExpStr << "���ñ� �������� ӦΪ: " << s_uTableFileWide << " �У�ʵΪ: " << TableFile.GetWidth() << " ��";
		GenExpInfo(ExpStr.str());
	}

	for(int32 i = 1; i < TableFile.GetHeight(); ++i)
	{
		SetLineNo(i);
		string strName, strType, strStanceType, strMagicEff;
		ReadItem(strName, szMagic_Name, CANEMPTY);
		ReadItem(strStanceType, szMagic_StanceType, CANEMPTY);

		ReadItem(strMagicEff, szSkill_MagicEff, CANEMPTY);
		CCfgMagicEffCheck::CheckMagicEffExist(strMagicEff);

		bool bNeedSaveToDB;
		ReadItem(bNeedSaveToDB,	szAureMagic_SaveToDB, CANEMPTY,	NO);

		trimend(strName);
		MapAureMagic::iterator iter = ms_mapAureMagic.find(strName);
		if (iter != ms_mapAureMagic.end())
		{
			stringstream ExpStr;
			ExpStr << " ���ñ�: " << g_sTabName << " �� " << i << " �е� " << szMagic_Name << "["<< strName << "]" << "�ظ�";
			GenExpInfo(ExpStr.str());
		}
		else
		{	
			CCfgAureMagicCheck* pAure = new CCfgAureMagicCheck;
			pAure->m_strName = strName;
			pAure->m_bNeedSaveToDB = bNeedSaveToDB;
			pAure->m_bPersistent = (strStanceType == "��̬" || strStanceType == "������̬") ? true : false;
			if (!strMagicEff.empty() && CCfgMagicEffCheck::CheckMagicEffExist(strMagicEff))
			{
				CCfgMagicEffCheck::MapMagicEff::iterator itr = CCfgMagicEffCheck::ms_mapMagicEff.find(strMagicEff);
				pAure->m_pMagicEff = (*itr).second;
			}
			ms_mapAureMagic.insert(make_pair(strName, pAure));
		}

		// ��Ч�����
		string strFX = TableFile.GetString(i, szMagic_FxName);
		if (strFX != "")
		{
			vector<string> sFXTable = CCfgMagicOp::Split(strFX, ",");
			if (sFXTable.size() == 2 || sFXTable.size() == 3)
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

void CCfgAureMagicCheck::EndCheck()
{
	ClearMap(ms_mapAureMagic);
}

bool CCfgAureMagicCheck::CheckExist(const string& strName)
{
	MapAureMagic::iterator iter = ms_mapAureMagic.find(strName);
	if (iter == ms_mapAureMagic.end())
	{
		stringstream ExpStr;
		ExpStr << " Υ��ħ������Լ��: ��̬����" << " [" << strName << "] ������ ";
		CfgChk::GenExpInfo(ExpStr.str());
		return false;
	}
	return true;
}
