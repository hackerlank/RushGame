#include "stdafx.h"
#include "CCfgProcessCheck.h"
#include "CTxtTableFile.h"
#include "CCfgColChecker.inl"
#include "LoadSkillCommon.h"
#include "CSkillCfg.h"

CSkillCfg::MapCastingType CSkillCfg::ms_mapCastingType;
CSkillCfg::MapCastingInterruptType CSkillCfg::ms_mapCastingInterruptType;
CCfgProcessCheck::MapProcess	CCfgProcessCheck::ms_mapProcess;

CCfgProcessCheck::CCfgProcessCheck()
:m_strName("")
,m_pMagicEff(NULL)
,m_pSelfCancelEff(NULL)
,m_pObjCancelEff(NULL)
{
}

CCfgProcessCheck::~CCfgProcessCheck()
{
}

bool CCfgProcessCheck::Check(const TCHAR* cfgFile)
{
	using namespace CfgChk;

	CSkillCfg::InitMapCastingType();
	CSkillCfg::InitMapCastingInterruptType();

	CTxtTableFile TableFile;
	SetTabFile(TableFile, "����");
	if (!TableFile.Load(PATH_ALIAS_CFG.c_str(), cfgFile))
	{
		stringstream ExpStr;
		ExpStr << "���ñ� ����ʧ�ܣ���鿴�ļ���[" << cfgFile << "]�Ƿ���ȷ�����ļ��Ƿ����";
		GenExpInfo(ExpStr.str());
	}

	static const uint32 s_uTableFileWide = 12;
	if (TableFile.GetWidth() != s_uTableFileWide)
	{
		stringstream ExpStr;
		ExpStr << "���ñ� �������� ӦΪ: " << s_uTableFileWide << " �У�ʵΪ: " << TableFile.GetWidth() << " ��";
		GenExpInfo(ExpStr.str());
	}

	for(int32 i = 1; i < TableFile.GetHeight(); ++i)
	{
		SetLineNo(i);
		string strName, strMagicEff, strSelfCancelEff, strObjCancelEff;
		ReadItem(strName, szCasting_Name, CANEMPTY);
		ReadItem(strMagicEff,		szMagicEff,					CANEMPTY);
		ReadItem(strSelfCancelEff,	szSelfCancelableMagicEff,	CANEMPTY);
		ReadItem(strObjCancelEff,	szObjCancelableMagicEff,	CANEMPTY);
		trimend(strName);
		MapProcess::iterator iter = ms_mapProcess.find(strName);
		if (iter != ms_mapProcess.end())
		{
			stringstream ExpStr;
			ExpStr << " ���ñ�: " << g_sTabName << " �� " << i << " �е� " << szCasting_Name << "["<< strName << "]" << "�ظ�";
			GenExpInfo(ExpStr.str());
		}
		else
		{
			CCfgProcessCheck* pProcess = new CCfgProcessCheck;
			pProcess->m_strName = strName;
			if (!strMagicEff.empty())
			{
				size_t comma = strMagicEff.find(',');
				SetItemTitle(szMagicEff);
				string strFixMagicEff;
				if(comma != string::npos)
				{
					strFixMagicEff = strMagicEff.substr(0, comma);	
				}
				else
				{
					strFixMagicEff = strMagicEff;
				}
				if (CCfgMagicEffCheck::CheckMagicEffExist(strFixMagicEff))
				{
					CCfgMagicEffCheck::MapMagicEff::iterator itr = CCfgMagicEffCheck::ms_mapMagicEff.find(strFixMagicEff);
					pProcess->m_pMagicEff = (*itr).second;
				}
			}
			if (!strSelfCancelEff.empty() && CCfgMagicEffCheck::CheckMagicEffExist(strSelfCancelEff))
			{
				CCfgMagicEffCheck::MapMagicEff::iterator itr = CCfgMagicEffCheck::ms_mapMagicEff.find(strSelfCancelEff);
				pProcess->m_pSelfCancelEff = (*itr).second;
			}
			if (!strObjCancelEff.empty() && CCfgMagicEffCheck::CheckMagicEffExist(strObjCancelEff))
			{
				CCfgMagicEffCheck::MapMagicEff::iterator itr = CCfgMagicEffCheck::ms_mapMagicEff.find(strObjCancelEff);
				pProcess->m_pObjCancelEff = (*itr).second;
			}
			ms_mapProcess.insert(make_pair(strName, pProcess));
		}

		string strCastingType;
		ReadItem(strCastingType, szCasting_Type, CANEMPTY);
		trimend(strCastingType);
		if(!CSkillCfg::ms_mapCastingType.count(strCastingType))
		{
			stringstream ExpStr;
			ExpStr << "��" << i << "�е�" << "����ʩ�����ʹ���";
			GenExpInfo(ExpStr.str());
		}

		ECastingProcessType eCastingProcessType = CSkillCfg::ms_mapCastingType[strCastingType];
		float fChannelInterval = 1.0f;
		if (eCPT_Channel == eCastingProcessType)
		{
			CCfgCalc* magicEff;
			ReadMixedItem(magicEff,		szMagicEff,	CANEMPTY,	"");
			if(magicEff->GetStringCount() == 2)
			{
				SetValue(fChannelInterval, magicEff->GetString(1));
			}
		}
		if(fChannelInterval <= 0.0f)
		{
			GenExpInfo("Υ����Լ��������Ч�����ʱ�����Ϊ����\n");
		}
		else if(fChannelInterval / 1.0f != float(int32(fChannelInterval / 1.0f)) && fChannelInterval != 0.5f)
		{
			GenExpInfo("Υ����Լ��������Ч�����ʱ�����Ϊ������0.5\n");
		}

		string strCastingInterruptType;
		ReadItem(strCastingInterruptType, szCasting_InterruptType);
		trimend(strCastingInterruptType);
		if(!CSkillCfg::ms_mapCastingInterruptType.count(strCastingInterruptType))
		{
			stringstream ExpStr;
			ExpStr << "��" << i << "�е�" << "���̴�����ʹ���";
			GenExpInfo(ExpStr.str());
		}
		string strSelfCancelableMagicEff, strObjCancelableMagicEff, strFinalMagicEff;
		ReadItem(strSelfCancelableMagicEff,	szSelfCancelableMagicEff,	CANEMPTY);
		ReadItem(strObjCancelableMagicEff,	szObjCancelableMagicEff,	CANEMPTY);
		ReadItem(strFinalMagicEff,			szFinalMagicEff,			CANEMPTY);
		if (!strMagicEff.empty())
		{
			if((!strSelfCancelableMagicEff.empty() && strSelfCancelableMagicEff == strMagicEff)
				|| (!strObjCancelableMagicEff.empty() && strObjCancelableMagicEff == strMagicEff) 
				|| (!strFinalMagicEff.empty() && strFinalMagicEff == strMagicEff))
			{
				stringstream str;
				str << "\n��" << i << "��Υ����Լ����ͬһ�еĲ�ͬħ��Ч������ͬ��";
				GenExpInfo(str.str());
			}
		}
		if((!strSelfCancelableMagicEff.empty() && !strObjCancelableMagicEff.empty() && strSelfCancelableMagicEff == strObjCancelableMagicEff)
			|| (!strSelfCancelableMagicEff.empty() && !strFinalMagicEff.empty() && strSelfCancelableMagicEff == strFinalMagicEff) 
			|| (!strObjCancelableMagicEff.empty() && !strFinalMagicEff.empty() && strObjCancelableMagicEff == strFinalMagicEff))
		{
			stringstream str;
			str << "\n��" << i << "��Υ����Լ����ͬһ�еĲ�ͬħ��Ч������ͬ��";
			GenExpInfo(str.str());
		}

		// ��Ч�����
		string strFX = TableFile.GetString(i, szProcessEffect);
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

void CCfgProcessCheck::EndCheck()
{
	ClearMap(ms_mapProcess);
	CSkillCfg::ms_mapCastingType.clear();
}

bool CCfgProcessCheck::CheckExist(const string& strName)
{
	MapProcess::iterator iter = ms_mapProcess.find(strName);
	if (iter == ms_mapProcess.end())
	{
		//stringstream ExpStr;
		//ExpStr << " Υ��ħ������Լ��: ����" << " [" << strName << "] ������ ";
		//GenExpInfo(ExpStr.str());
		return false;
	}
	return true;
}
