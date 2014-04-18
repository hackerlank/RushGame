#include "stdafx.h"
#include "CCfgNormalAttackCheck.h"
#include "CTxtTableFile.h"
#include "CCfgColChecker.inl"
#include "LoadSkillCommon.h"
#include "CSkillCfg.h"

CCfgNormalAttackCheck::SetPlayerNAName	CCfgNormalAttackCheck::ms_setPlayerNAName;
CCfgNormalAttackCheck::SetNPCNAName		CCfgNormalAttackCheck::ms_setNPCNAName;
CCfgNormalAttackCheck::MapNormalAttack	CCfgNormalAttackCheck::ms_mapNormalAttack;

bool CCfgNormalAttackCheck::CheckPlayerNormalAttackCfg(const TCHAR* cfgFile)
{
	using namespace CfgChk;

	CTxtTableFile TableFile;
	SetTabFile(TableFile, "��ͨ����");
	if (!TableFile.Load(PATH_ALIAS_CFG.c_str(), cfgFile))
	{
		stringstream ExpStr;
		ExpStr << "���ñ� ����ʧ�ܣ���鿴�ļ���[" << cfgFile << "]�Ƿ���ȷ�����ļ��Ƿ����";
		GenExpInfo(ExpStr.str());
	}

	static const uint32 s_uTableFileWide = 5;
	if (TableFile.GetWidth() != s_uTableFileWide)
	{
		stringstream ExpStr;
		ExpStr << "���ñ� �������� ӦΪ: " << s_uTableFileWide << " �У�ʵΪ: " << TableFile.GetWidth() << " ��";
		GenExpInfo(ExpStr.str());
	}

	for( int32 i = 1; i < TableFile.GetHeight(); ++i )
	{
		SetLineNo(i);
		string strName, strMagicEff;
		ReadItem(strName, szNT_Name, CANEMPTY);
		ReadItem(strMagicEff, szSkill_MagicEff, CANEMPTY);
		CCfgMagicEffCheck::CheckMagicEffExist(strMagicEff);
		trimend(strName);
		SetPlayerNAName::iterator iter = ms_setPlayerNAName.find(strName);
		if (iter != ms_setPlayerNAName.end())
		{
			stringstream ExpStr;
			ExpStr << " ���ñ�: " << g_sTabName << " �� " << i << " �е� " << szNT_Name << "["<< strName << "]" << "�ظ�";
			GenExpInfo(ExpStr.str());
		}
		else
		{
			ms_setPlayerNAName.insert(strName);
			CCfgNormalAttackCheck* pNormalAttack = new CCfgNormalAttackCheck;
			pNormalAttack->m_strName = strName;
			if (!strMagicEff.empty() && CCfgMagicEffCheck::CheckMagicEffExist(strMagicEff))
			{
				CCfgMagicEffCheck::MapMagicEff::iterator itr = CCfgMagicEffCheck::ms_mapMagicEff.find(strMagicEff);
				pNormalAttack->m_pMagicEff = (*itr).second;
			}
			ms_mapNormalAttack.insert(make_pair(strName, pNormalAttack));
		}
	}

	return true;
}

bool CCfgNormalAttackCheck::CheckNPCNormalAttackCfg(const TCHAR* cfgFile)
{
	using namespace CfgChk;

	CTxtTableFile TableFile;
	SetTabFile(TableFile, "NPC��ͨ����");
	if (!TableFile.Load(PATH_ALIAS_CFG.c_str(), cfgFile))
	{
		stringstream ExpStr;
		ExpStr << "���ñ� ����ʧ�ܣ���鿴�ļ���[" << cfgFile << "]�Ƿ���ȷ�����ļ��Ƿ����";
		GenExpInfo(ExpStr.str());
	}

	static const uint32 s_uTableFileWide = 5;
	if (TableFile.GetWidth() != s_uTableFileWide)
	{
		stringstream ExpStr;
		ExpStr << "���ñ� �������� ӦΪ: " << s_uTableFileWide << " �У�ʵΪ: " << TableFile.GetWidth() << " ��";
		GenExpInfo(ExpStr.str());
	}

	for( int32 i = 1; i < TableFile.GetHeight(); ++i )
	{
		SetLineNo(i);
		string strName, strMagicEff;
		EAttackType	eAttackType;
		ReadItem(strName, szNT_Name, CANEMPTY);
		ReadItem(strMagicEff, szSkill_MagicEff, CANEMPTY);
		ReadItem(eAttackType, szSkill_AttackType, CSkillCfg::ms_mapAttackType);
		
		CCfgMagicEffCheck::CheckMagicEffExist(strMagicEff);
		trimend(strName);
		SetNPCNAName::iterator iter = ms_setNPCNAName.find(strName);
		if (iter != ms_setNPCNAName.end())
		{
			stringstream ExpStr;
			ExpStr << " ���ñ�: " << g_sTabName << " �� " << i << " �е� " << szMagicState_Name << "["<< strName << "]" << "�ظ�";
			GenExpInfo(ExpStr.str());
		}
		else
		{
			ms_setNPCNAName.insert(strName);
			CCfgNormalAttackCheck* pNormalAttack = new CCfgNormalAttackCheck;
			pNormalAttack->m_strName = strName;
			if (!strMagicEff.empty() && CCfgMagicEffCheck::CheckMagicEffExist(strMagicEff))
			{
				CCfgMagicEffCheck::MapMagicEff::iterator itr = CCfgMagicEffCheck::ms_mapMagicEff.find(strMagicEff);
				pNormalAttack->m_pMagicEff = (*itr).second;
				EValueMagicOpType eValueMagicOpType = pNormalAttack->m_pMagicEff->GetFirstOpValueMagicOpType();
				switch(eAttackType)
				{
				case eATT_Puncture:
				case eATT_Chop:
				case eATT_BluntTrauma:
					{
						if (eValueMagicOpType == eVMOT_Magic)
						{
							stringstream ExpStr;
							ExpStr << "���ñ�[" << g_sTabName << "]��" << (g_iLine + 1) << "�е�["
								<< szSkill_AttackType << "]�͵�һ���˺�����Ч�����Ͳ�һ��, �������������,�˺��Ƿ�����";
							GenExpInfo(ExpStr.str());
						}
						break;
					}
				case eATT_Nature:
				case eATT_Destroy:
				case eATT_Evil:
					{
						if (eValueMagicOpType == eVMOT_Physical)
						{
							stringstream ExpStr;
							ExpStr << "���ñ�[" << g_sTabName << "]��" << (g_iLine + 1) << "�е�["
								<< szSkill_AttackType << "]�͵�һ���˺�����Ч�����Ͳ�һ��, ���������Ƿ�����,�˺��������";
							GenExpInfo(ExpStr.str());
						}
						break;
					}
				case eATT_None:
				default:
					break;
				}
			}
			ms_mapNormalAttack.insert(make_pair(strName, pNormalAttack));
		}
		if (eAttackType == eATT_FollowWeapon)
		{
			stringstream ExpStr;
			ExpStr << "���ñ�[" << g_sTabName << "]��" << (g_iLine + 1) << "�е�["
				<< szSkill_AttackType << "]�в�����[������������]";
			GenExpInfo(ExpStr.str());
		}
	}

	return true;
}

void CCfgNormalAttackCheck::EndCheck()
{
	ms_setPlayerNAName.clear();
	ms_setNPCNAName.clear();
	ClearMap(ms_mapNormalAttack);
}

bool CCfgNormalAttackCheck::IsPlayerNormalAttackNameValid(const string& strName)
{
	SetPlayerNAName::iterator iter = ms_setPlayerNAName.find(strName);
	if (iter == ms_setPlayerNAName.end())
	{
		stringstream ExpStr;
		ExpStr << " Υ��ħ������Լ��: ��ͨ����" << " [" << strName << "] ������ ";
		CfgChk::GenExpInfo(ExpStr.str());
		return false;
	}
	return true;
}

bool CCfgNormalAttackCheck::IsNPCNormalAttackNameValid(const string& strName)
{
	SetNPCNAName::iterator iter = ms_setNPCNAName.find(strName);
	if (iter == ms_setNPCNAName.end())
	{
		stringstream ExpStr;
		ExpStr << " Υ��ħ������Լ��: NPC��ͨ����" << " [" << strName << "] ������ ";
		CfgChk::GenExpInfo(ExpStr.str());
		return false;
	}
	return true;
}
