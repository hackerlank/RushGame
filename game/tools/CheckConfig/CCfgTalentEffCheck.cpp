#include "stdafx.h"
#include "CCfgTalentEffCheck.h"
#include "CTxtTableFile.h"
#include "CCfgColChecker.inl"
#include "LoadSkillCommon.h"
#include "CSkillCfg.h"
#include "CPropertyCfg.h"
#include "CCfgMagicEffCheck.h"

CSkillCfg::MapTalentType CSkillCfg::ms_mapTalentType;
CSkillCfg::MapTalentRuleType CSkillCfg::ms_mapTalentRuleType;

bool CCfgTalentEffCheck::Check(const TCHAR* cfgFile)
{
	using namespace CfgChk;

	CSkillCfg::InitMapTalentType();
	CSkillCfg::InitMapTalentRuleType();
	CPropertyCfg::InitMapClassType();

	CTxtTableFile TableFile;
	Load(TableFile, cfgFile, "�츳Ч��");

	static const uint32 s_uTableFileWide = 5;
	if (TableFile.GetWidth() != s_uTableFileWide)
	{
		stringstream ExpStr;
		ExpStr << "���ñ� �������� ӦΪ: " << s_uTableFileWide << " �У�ʵΪ: " << TableFile.GetWidth() << " ��";
		GenExpInfo(ExpStr.str());
	}

	for(int32 i = 1; i < TableFile.GetHeight(); ++i)
	{
		CheckOverlap(szSkill_TalentName, i);

		ETalentType eTalentType;
		string strTypeArg, strMagicEff;
		ReadItem(eTalentType,	szSkill_TalentType, CSkillCfg::ms_mapTalentType);
		ReadItem(strTypeArg,	szSkill_TypeArg,	CANEMPTY);
		ReadItem(strMagicEff,	szSkill_MagicEff,	CANEMPTY);
		CCfgMagicEffCheck::CheckMagicEffExist(strMagicEff);
		switch (eTalentType)
		{
		case eTT_Class:
			{
				if (!CPropertyCfg::ms_mapClassType.count(strTypeArg))
				{
					stringstream ExpStr;
					ExpStr << "���ñ��" << (i + 1) << "�еļ���[" << TableFile.GetString(i, szSkill_TalentName) << "]���Ͳ���[" << strTypeArg << "]����";
					GenExpInfo(ExpStr.str());
				}
			}
			break;
		case eTT_Equip:
			{
				if (!CSkillCfg::ms_mapTalentRuleType.count(strTypeArg))
				{
					stringstream ExpStr;
					ExpStr << "���ñ��" << (i + 1) << "�еļ���[" << TableFile.GetString(i, szSkill_TalentName) << "]���Ͳ���[" << strTypeArg << "]����";
					GenExpInfo(ExpStr.str());
				}
			}
			break;
		default:
			break;
		}
	}
	return true;
}

void CCfgTalentEffCheck::EndCheck()
{
	CSkillCfg::ms_mapTalentType.clear();
	CSkillCfg::ms_mapTalentRuleType.clear();
	CPropertyCfg::ms_mapClassType.clear();
	EndCheckOverlap();
}
