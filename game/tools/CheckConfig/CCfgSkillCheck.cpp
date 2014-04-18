#include "stdafx.h"
#include "CCfgSkillCheck.h"
#include "CTxtTableFile.h"
#include "CCfgColChecker.inl"
#include "LoadSkillCommon.h"
#include "CSkillCfg.h"
#include "CCfgProcessCheck.h"

CSkillCfg::MapTargetType	CSkillCfg::ms_mapTargetType;
CSkillCfg::MapCoolDownType	CSkillCfg::ms_mapCoolDownType;
CSkillCfg::MapAttackType	CSkillCfg::ms_mapAttackType;
CCfgSkillCheck::MapSkill	CCfgSkillCheck::ms_mapSkill;

using namespace CfgChk;

CCfgSkillCheck* CCfgSkillCheck::GetInst()
{
	static CCfgSkillCheck ms_SkillCheck;
	return &ms_SkillCheck;
}

bool CCfgSkillCheck::CheckPlayerSkillCfg(const TCHAR* cfgFile)
{
	CSkillCfg::InitMapTargetType();
	CSkillCfg::InitMapCoolDownType();
	CTxtTableFile TabFile;
	SetTabFile(TabFile, "��Ҽ��ܱ�");
	if (!TabFile.Load(PATH_ALIAS_CFG.c_str(), cfgFile))
	{
		stringstream ExpStr;
		ExpStr << " ���ñ� ����ʧ�ܣ���鿴�ļ���[" << cfgFile << "]�Ƿ���ȷ�����ļ��Ƿ����";
		GenExpInfo(ExpStr.str());
	}

	static const uint32 s_uTableFileWide = 20;
	if (TabFile.GetWidth() != s_uTableFileWide)
	{
		stringstream ExpStr;
		ExpStr << "���ñ� �������� ӦΪ: " << s_uTableFileWide << " �У�ʵΪ: " << TabFile.GetWidth() << " ��";
		GenExpInfo(ExpStr.str());
	}

	for( int32 i = 1; i < TabFile.GetHeight(); ++i )
	{
		SetLineNo(i);
		string strName, strMagicEff;
		ReadItem(strName, szSkill_Name, CANEMPTY);
		ReadItem(strMagicEff, szSkill_MagicEff, CANEMPTY);
		CCfgMagicEffCheck::CheckMagicEffExist(strMagicEff);
		trimend(strName);
		MapSkill::iterator iter = ms_mapSkill.find(strName);
		if (iter != ms_mapSkill.end())
		{
			stringstream ExpStr;
			ExpStr << " ���ñ�: " << g_sTabName << " �� " << i << " �е� " << szSkill_Name << "["<< strName << "]" << "�ظ�";
			GenExpInfo(ExpStr.str());
		}
		else
		{
			CCfgSkillCheck* pSkill = new CCfgSkillCheck;
			pSkill->m_bIsDoSkillRule = true;
			pSkill->m_strSkillName = strName;
			if (!strMagicEff.empty())
			{
				CCfgMagicEffCheck::MapMagicEff::iterator itr = CCfgMagicEffCheck::ms_mapMagicEff.find(strMagicEff);
				if(itr == CCfgMagicEffCheck::ms_mapMagicEff.end())
				{
					stringstream ExpStr;
					ExpStr << " ���ñ�: " << g_sTabName << " �� " << i << " �е� " << strName << " ��Ч��[" << strMagicEff << "]" << "������";
					GenExpInfo(ExpStr.str());
				}
				pSkill->m_pMagicEff = (*itr).second;
			}
			ms_mapSkill.insert(make_pair(strName, pSkill));
		}

		string sTempSelectType;
		ReadItem(sTempSelectType,	szSkill_SelectTargetType,	CANEMPTY,	"");
		string::size_type pos = sTempSelectType.find_first_of(',');
		if(pos == string::npos)
		{
			if(!CSkillCfg::ms_mapTargetType.count(sTempSelectType))
			{
				stringstream ExpStr;
				ExpStr << "���ñ�[�����ܱ�]��" << (i + 1) << "�еļ���,ѡ��Ŀ������[" << sTempSelectType << "]����";
				GenExpInfo(ExpStr.str());
			}
		}
		else
		{
			string sSelectType = sTempSelectType.substr(0, pos);
			if(!CSkillCfg::ms_mapTargetType.count(sSelectType))
			{
				stringstream ExpStr;
				ExpStr << "���ñ�[�����ܱ�]��" << (i + 1) << "�еļ���,ѡ��Ŀ������[" << sSelectType << "����";
				GenExpInfo(ExpStr.str());
			}
		}

		//����ӵ��ʱ�估�������
		ESkillCoolDownType eCoolDownType;
		ReadItem(eCoolDownType,		szSkill_Type,	CSkillCfg::ms_mapCoolDownType);
		string strActiveArg;
		ReadItem(strActiveArg,					szSkill_ActiveTimeAndCount,CANEMPTY);
		if (strActiveArg!= "")
		{
			CCfgCalc* pActiveArg = new CCfgCalc;
			CCfgCalc* pActiveTime = new CCfgCalc;
			pActiveArg->InputString(strActiveArg);
			if (pActiveArg->GetStringCount()>1)
			{
				if (eCoolDownType != eSCDT_TempSkill)
				{
					stringstream ExpStr;
					ExpStr << "���ñ�[�����ܱ�]��" << (i + 1) << "�еļ���[" << strName<< "]��" << szSkill_ActiveTimeAndCount << "����,ֻ����ʱ���ܲ��������";
					GenExpInfo(ExpStr.str());
				}
				else
				{
					CCfgCalc* pActiveCount = new CCfgCalc;
					pActiveCount->InputString(pActiveArg->GetString(1));
					if (pActiveCount->GetIntValue()<0)
					{
						stringstream ExpStr;
						ExpStr << "���ñ�[�����ܱ�]��" << (i + 1) << "�еļ���[" << strName<< "]��" << szSkill_ActiveTimeAndCount << "����,��������С��0";
						GenExpInfo(ExpStr.str());
					}
					delete pActiveCount;
				}
			}
			pActiveTime->InputString(pActiveArg->GetString(0));
			if ((float)pActiveTime->GetDblValue() < 0)
			{
				stringstream ExpStr;
				ExpStr << "���ñ�[�����ܱ�]��" << (i + 1) << "�еļ���[" << strName<< "]��" << szSkill_ActiveTimeAndCount << "����,ʱ�䲻��С��0";
				GenExpInfo(ExpStr.str());
			}
			delete pActiveArg;
			delete pActiveTime;
		}

		// ��Ч�����
		string strFX = TabFile.GetString(i, szSkill_CastEffect);
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



bool CCfgSkillCheck::CheckNPCSkillCfg(const TCHAR* cfgFile)
{
	CSkillCfg::InitMapAttackType();
	CTxtTableFile TabFile;
	SetTabFile(TabFile, "NPC�����ܱ�");
	if (!TabFile.Load(PATH_ALIAS_CFG.c_str(), cfgFile))
	{
		stringstream ExpStr;
		ExpStr << " ���ñ� ����ʧ�ܣ���鿴�ļ���[" << cfgFile << "]�Ƿ���ȷ�����ļ��Ƿ����";
		GenExpInfo(ExpStr.str());
	}

	static const uint32 s_uTableFileWide = 6;
	if (TabFile.GetWidth() != s_uTableFileWide)
	{
		stringstream ExpStr;
		ExpStr << "���ñ� �������� ӦΪ: " << s_uTableFileWide << " �У�ʵΪ: " << TabFile.GetWidth() << " ��";
		GenExpInfo(ExpStr.str());
	}

	for( int32 i = 1; i < TabFile.GetHeight(); ++i )
	{
		SetLineNo(i);
		string strName, strMagicEff, strCastAction;
		ReadItem(strName, szSkill_Name, CANEMPTY);
		ReadItem(strMagicEff, szSkill_MagicEff, CANEMPTY);
		ReadItem(strCastAction, szSkill_CastAction, CANEMPTY);

		if (!strCastAction.empty() && CCfgSkillCheck::GetInst()->m_setSkillAction.find(strCastAction) == CCfgSkillCheck::GetInst()->m_setSkillAction.end())
		{
			stringstream ExpStr;
			ExpStr << " ���ñ�: " << g_sTabName << " �� " << i << " �е� " << szSkill_CastAction << "["<< strCastAction << "]" << "��������ĳ��ֶ�����(birth, attack01-04, cast01-09, sf01-09, skill01-09)";
			GenExpInfo(ExpStr.str());
		}

		CCfgMagicEffCheck::CheckMagicEffExist(strMagicEff);
		trimend(strName);
		MapSkill::iterator iter = ms_mapSkill.find(strName);
		if (iter != ms_mapSkill.end())
		{
			stringstream ExpStr;
			ExpStr << " ���ñ�: " << g_sTabName << " �� " << i << " �е� " << szSkill_Name << "["<< strName << "]" << "�ظ�";
			GenExpInfo(ExpStr.str());
		}
		else
		{
			CCfgSkillCheck* pSkill = new CCfgSkillCheck;
			pSkill->m_strSkillName = strName;
			if (!strMagicEff.empty())
			{
				CCfgMagicEffCheck::MapMagicEff::iterator itr = CCfgMagicEffCheck::ms_mapMagicEff.find(strMagicEff);
				if(itr == CCfgMagicEffCheck::ms_mapMagicEff.end())
				{
					stringstream ExpStr;
					ExpStr << " ���ñ�: " << g_sTabName << " �� " << i << " �е� " << strName << " ��Ч��[" << strMagicEff << "]" << "������";
					GenExpInfo(ExpStr.str());
				}
				pSkill->m_pMagicEff = (*itr).second;
				ReadItem(pSkill->m_bIsDoSkillRule,"�Ƿ��߼��ܹ���",	CANEMPTY,	YES);
			}
			ms_mapSkill.insert(make_pair(strName, pSkill));
		}

		EAttackType eAttackType;
		ReadItem(eAttackType,	szSkill_AttackType,	CANEMPTY,	eATT_None,	CSkillCfg::ms_mapAttackType);
		if(eAttackType == eATT_FollowWeapon)
		{
			stringstream ExpStr;
			ExpStr << "���ñ�[" << g_sTabName << "]��" << (g_iLine + 1) << "�е�["
				<< szSkill_AttackType << "]�в�����[������������]";
			GenExpInfo(ExpStr.str());
		}

		// ��Ч�����
		string strFX = TabFile.GetString(i, szSkill_CastEffect);
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

bool CCfgSkillCheck::CheckIntObjSkillCfg(const TCHAR* cfgFile)
{
	CTxtTableFile TabFile;
	SetTabFile(TabFile, "IntObj�����ܱ�");
	if (!TabFile.Load(PATH_ALIAS_CFG.c_str(), cfgFile))
	{
		stringstream ExpStr;
		ExpStr << " ���ñ� ����ʧ�ܣ���鿴�ļ���[" << cfgFile << "]�Ƿ���ȷ�����ļ��Ƿ����";
		GenExpInfo(ExpStr.str());
	}

	static const uint32 s_uTableFileWide = 2;
	if (TabFile.GetWidth() != s_uTableFileWide)
	{
		stringstream ExpStr;
		ExpStr << "���ñ� �������� ӦΪ: " << s_uTableFileWide << " �У�ʵΪ: " << TabFile.GetWidth() << " ��";
		GenExpInfo(ExpStr.str());
	}

	for( int32 i = 1; i < TabFile.GetHeight(); ++i )
	{
		SetLineNo(i);
		string strName, strMagicEff;
		ReadItem(strName, szSkill_Name, CANEMPTY);
		ReadItem(strMagicEff, szSkill_MagicEff, CANEMPTY);
		CCfgMagicEffCheck::CheckMagicEffExist(strMagicEff);
		if (CCfgProcessCheck::CheckExist(strName))
		{
			stringstream ExpStr;
			ExpStr << " ���ñ�: " << g_sTabName << " �� " << i << " �е� " << szSkill_Name << "["<< strName << "]" << "���������̼���";
			GenExpInfo(ExpStr.str());
		}
		trimend(strName);
		MapSkill::iterator iter = ms_mapSkill.find(strName);
		if (iter != ms_mapSkill.end())
		{
			stringstream ExpStr;
			ExpStr << " ���ñ�: " << g_sTabName << " �� " << i << " �е� " << szSkill_Name << "["<< strName << "]" << "�ظ�";
			GenExpInfo(ExpStr.str());
		}
		else
		{
			CCfgSkillCheck* pSkill = new CCfgSkillCheck;
			pSkill->m_strSkillName = strName;
			if (!strMagicEff.empty())
			{
				CCfgMagicEffCheck::MapMagicEff::iterator itr = CCfgMagicEffCheck::ms_mapMagicEff.find(strMagicEff);
				pSkill->m_pMagicEff = (*itr).second;
			}
			ms_mapSkill.insert(make_pair(strName, pSkill));
		}
	}

	return true;
}

void CCfgSkillCheck::EndCheck()
{
	ClearMap(ms_mapSkill);
}

CCfgSkillCheck::CCfgSkillCheck()
:m_strSkillName("")
,m_pMagicEff(NULL)
{

	m_setSkillAction.insert("birth");
	m_setSkillAction.insert("attack01");
	m_setSkillAction.insert("attack02");
	m_setSkillAction.insert("attack03");
	m_setSkillAction.insert("attack04");
	m_setSkillAction.insert("cast01");
	m_setSkillAction.insert("cast02");
	m_setSkillAction.insert("cast03");
	m_setSkillAction.insert("cast04");
	m_setSkillAction.insert("cast05");
	m_setSkillAction.insert("cast06");
	m_setSkillAction.insert("cast07");
	m_setSkillAction.insert("cast08");
	m_setSkillAction.insert("cast09");
	m_setSkillAction.insert("sf01");
	m_setSkillAction.insert("sf02");
	m_setSkillAction.insert("sf03");
	m_setSkillAction.insert("sf04");
	m_setSkillAction.insert("sf05");
	m_setSkillAction.insert("sf06");
	m_setSkillAction.insert("sf07");
	m_setSkillAction.insert("sf08");
	m_setSkillAction.insert("sf09");
	m_setSkillAction.insert("skill01");
	m_setSkillAction.insert("skill02");
	m_setSkillAction.insert("skill03");
	m_setSkillAction.insert("skill04");
	m_setSkillAction.insert("skill05");
	m_setSkillAction.insert("skill06");
	m_setSkillAction.insert("skill07");
	m_setSkillAction.insert("skill08");
	m_setSkillAction.insert("skill09");

}

CCfgSkillCheck::~CCfgSkillCheck()
{
}

bool CCfgSkillCheck::IsPlayerSkillNameValid(const TCHAR* szName)
{
	string strName = szName;
	return ms_mapSkill.find(strName) != ms_mapSkill.end();
}

bool CCfgSkillCheck::IsNPCSkillNameValid(const TCHAR* szName)
{
	string strName = szName;
	return ms_mapSkill.find(strName) != ms_mapSkill.end();
}

bool CCfgSkillCheck::IsIntObjSkillNameValid(const TCHAR* szName)
{
	string strName = szName;
	return ms_mapSkill.find(strName) != ms_mapSkill.end();
}

bool CCfgSkillCheck::IsNPCSkillDoSkillRule(const TCHAR* szName)
{
	MapSkill::iterator iter = ms_mapSkill.find(szName);
	if (iter != ms_mapSkill.end())
	{
		return iter->second->m_bIsDoSkillRule;
	}
	return false;
}

