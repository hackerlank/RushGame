#include "stdafx.h"
#include "CCfgAllMagicCheck.h"
#include "CTxtTableFile.h"
#include "CCfgColChecker.inl"
#include "LoadSkillCommon.h"
#include "CMoveMagicCfg.h"
#include "CSkillCfg.h"

CMoveMagicCfg::MapMoveArgLimit CMoveMagicCfg::ms_mapMoveArgLimit;
CMoveMagicCfg::MapMoveArg CMoveMagicCfg::ms_mapMoveArg;
CSkillCfg::MapOperateObjectType CSkillCfg::ms_mapOperateObjectType;
CCfgMoveMagicCheck::MapMoveMagic CCfgMoveMagicCheck::ms_mapMoveMagic;

CCfgMoveMagicCheck::CCfgMoveMagicCheck()
:m_strName("")
,m_pCancelMagicEff(NULL)
{
}

CCfgMoveMagicCheck::~CCfgMoveMagicCheck()
{
}

bool CCfgMoveMagicCheck::Check(const TCHAR* cfgFile)
{
	using namespace CfgChk;

	CMoveMagicCfg::InitMapMoveArgLimit();
	CMoveMagicCfg::InitMapMoveArg();
	CSkillCfg::InitMapOperateObjectType();

	CTxtTableFile TableFile;
	SetTabFile(TableFile, "�ƶ�ħ��");
	if (!TableFile.Load(PATH_ALIAS_CFG.c_str(), cfgFile))
	{
		stringstream ExpStr;
		ExpStr << "���ñ� ����ʧ�ܣ���鿴�ļ���[" << cfgFile << "]�Ƿ���ȷ�����ļ��Ƿ����";
		GenExpInfo(ExpStr.str());
	}

	static const uint32 s_uTableFileWide = 16;
	if (TableFile.GetWidth() != s_uTableFileWide)
	{
		stringstream ExpStr;
		ExpStr << "���ñ� �������� ӦΪ: " << s_uTableFileWide << " �У�ʵΪ: " << TableFile.GetWidth() << " ��";
		GenExpInfo(ExpStr.str());
	}

	for(int32 i = 1; i < TableFile.GetHeight(); ++i)
	{
		SetLineNo(i);
		string strName;
		ReadItem(strName, szMagic_Name, CANEMPTY);
		trimend(strName);

		string strMagicEff, strCancelEff;
		ReadItem(strMagicEff,	szMagic_MagicEff,			CANEMPTY);
		CCfgMagicEffCheck::CheckMagicEffExist(strMagicEff);
		ReadItem(strCancelEff,	szMagic_CancelableMagicEff,	CANEMPTY);
		CCfgMagicEffCheck::CheckMagicEffExist(strCancelEff);
		MapMoveMagic::iterator iter = ms_mapMoveMagic.find(strName);
		if (iter != ms_mapMoveMagic.end())
		{
			stringstream ExpStr;
			ExpStr << " ���ñ�: " << g_sTabName << " �� " << i << " �е� " << szMagic_Name << "["<< strName << "]" << "�ظ�";
			GenExpInfo(ExpStr.str());
		}
		else
		{
			CCfgMoveMagicCheck* pMoveMagic = new CCfgMoveMagicCheck;
			pMoveMagic->m_strName = strName;
			if (!strCancelEff.empty() && CCfgMagicEffCheck::CheckMagicEffExist(strCancelEff))
			{
				CCfgMagicEffCheck::MapMagicEff::iterator itr = CCfgMagicEffCheck::ms_mapMagicEff.find(strCancelEff);
				pMoveMagic->m_pCancelMagicEff = (*itr).second;
			}
			ms_mapMoveMagic.insert(make_pair(strName, pMoveMagic));
		}

		if(!strMagicEff.empty() && !strCancelEff.empty() && strMagicEff == strCancelEff)
		{
			stringstream str;
			str << "\n��" << i << "��Υ����Լ����ͬһ�еĲ�ͬħ��Ч������ͬ��";
			GenExpInfo(str.str());
		}

		CCfgCalc* pMoveArg = new CCfgCalc;
		string strMoveArg;
		ReadItem(strMoveArg,					szMagic_MoveArg);
		pMoveArg->InputString(strMoveArg);
		if(pMoveArg->GetStringCount()>1)
		{
			if (pMoveArg->GetStringCount()<3)
			{
				stringstream ExpStr;
				ExpStr << "���ñ��" << (i + 1) << "�еļ���[" << TableFile.GetString(i, szMagic_Name) << "�ƶ���������3��";
				GenExpInfo(ExpStr.str());
			}

			string strMoveArgLimit = pMoveArg->GetString(1);
			if (!CMoveMagicCfg::ms_mapMoveArgLimit.count(strMoveArgLimit))
			{
				stringstream ExpStr;
				ExpStr << "���ñ��" << (i + 1) << "�еļ���[" << TableFile.GetString(i, szMagic_Name) << "]�ƶ���������[" << strMoveArgLimit << "]����";
				GenExpInfo(ExpStr.str());
			}	
		}

		string strMoveArgType = pMoveArg->GetString(0);
		if (!CMoveMagicCfg::ms_mapMoveArg.count(strMoveArgType))
		{
			stringstream ExpStr;
			ExpStr << "���ñ��" << (i + 1) << "�еļ���[" << TableFile.GetString(i, szMagic_Name) << "]�ƶ�����[" << strMoveArgType << "]����";
			GenExpInfo(ExpStr.str());
		}
		delete pMoveArg;

		string strOpObject;
		ReadItem(strOpObject,		szMagic_OperateObject);
		CCfgCalc* pOperateObject = new CCfgCalc;
		pOperateObject->InputString(strOpObject);
		string strOperateObject = pOperateObject->GetString(0);
		if (!CSkillCfg::ms_mapOperateObjectType.count(strOperateObject))
		{
			stringstream ExpStr;
			ExpStr << "���ñ��" << (i + 1) << "�еļ���[" << TableFile.GetString(i, szMagic_Name) << "]���ö���[" << strOperateObject << "]����";
			GenExpInfo(ExpStr.str());
		}
		
		if (pOperateObject->GetStringCount()>1)
		{
			string strAreaOperateObject = pOperateObject->GetString(1);
			if (!CSkillCfg::ms_mapOperateObjectType.count(strAreaOperateObject))
			{
				stringstream ExpStr;
				ExpStr << "���ñ��" << (i + 1) << "�еļ���[" << TableFile.GetString(i, szMagic_Name) << "]���ö���[" << strAreaOperateObject << "]����";
				GenExpInfo(ExpStr.str());
			}
		}
		delete pOperateObject;

		// ��Ч�����
		string strFX = TableFile.GetString(i, szMagic_ProcessFxName);
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

void CCfgMoveMagicCheck::EndCheck()
{
	CMoveMagicCfg::ms_mapMoveArg.clear();
	CMoveMagicCfg::ms_mapMoveArgLimit.clear();
	CSkillCfg::ms_mapOperateObjectType.clear();
	ClearMap(ms_mapMoveMagic);
}

bool CCfgMoveMagicCheck::CheckExist(const string& strName)
{
	MapMoveMagic::iterator iter = ms_mapMoveMagic.find(strName);
	if (iter == ms_mapMoveMagic.end())
	{
		stringstream ExpStr;
		ExpStr << " Υ��ħ������Լ��: �ƶ�ħ��" << " [" << strName << "] ������ ";
		CfgChk::GenExpInfo(ExpStr.str());
		return false;
	}
	return true;
}
