#include "stdafx.h"
#include "CCfgPropertyCheck.h"
#include "CTxtTableFile.h"
#include "CCfgColChecker.inl"
#include "CPropertyCfg.h"

CPropertyCfg::MapClassType	CPropertyCfg::ms_mapClassType;

struct LevelZone
{
	uint8 uMinLevel;
	uint8 uMaxLevel;
};

bool CCfgPropertyCheck::Check(const TCHAR* cfgFile)
{
	CPropertyCfg::InitMapClassType();
	CTxtTableFile TabFile;
	CfgChk::SetTabFile(TabFile, "�������Ա�");
	if (!TabFile.Load(PATH_ALIAS_CFG.c_str(), cfgFile))
		return false;
	
	static const uint32 s_uTableFileWide = 27;
	if (TabFile.GetWidth() != s_uTableFileWide)
	{
		stringstream ExpStr;
		ExpStr << "���ñ� �������� ӦΪ: " << s_uTableFileWide << " �У�ʵΪ: " << TabFile.GetWidth() << " ��";
		CfgChk::GenExpInfo(ExpStr.str());
	}

	for (int32 i = 1; i < TabFile.GetHeight(); ++i)
	{
		// ְҵ & �ȼ� ���
		string strClass = TabFile.GetString(i, "ְҵ");
		CPropertyCfg::MapClassType::iterator it = CPropertyCfg::ms_mapClassType.find(strClass);
		if (it == CPropertyCfg::ms_mapClassType.end())
		{
			stringstream ExpStr;
			ExpStr << "���ñ�[�������Ա�] "<< strClass <<" ������";
			CfgChk::GenExpInfo(ExpStr.str());
		}
		
		CCfgCalc* pLevel = new CCfgCalc(TabFile.GetString(i, "�ȼ�"));
		uint8 uLeftLevel, uRightLevel;
		if (pLevel->GetStringCount() != 2)
		{
			stringstream ExpStr;
			ExpStr << "���ñ�[�������Ա�] "<< strClass << " �ȼ�������д���󣬱�����������С�����ȼ�";
			CfgChk::GenExpInfo(ExpStr.str());
		}
		else
		{
			uLeftLevel = atoi(pLevel->GetString(0).c_str());
			uRightLevel = atoi(pLevel->GetString(1).c_str());
		}

		static string strReadingClass = "";
		typedef map<string, LevelZone> MapClassLevelZone;
		static MapClassLevelZone mapReadClass;
		MapClassLevelZone::iterator itr = mapReadClass.find(strClass);
		if (strClass != strReadingClass)
		{
			if (itr != mapReadClass.end())
			{
				stringstream ExpStr;
				ExpStr << "���ñ�[�������Ա�] "<< strClass <<" �Ѷ�ȡ������鿴�Ƿ��ظ���д";
				CfgChk::GenExpInfo(ExpStr.str());
			}
			else
			{
				LevelZone levelZone;
				levelZone.uMinLevel = uLeftLevel;
				levelZone.uMaxLevel = uRightLevel;
				mapReadClass.insert(make_pair(strClass, levelZone));
				strReadingClass = strClass;
			}

			if (uLeftLevel != 1)
			{
				stringstream ExpStr;
				ExpStr << "���ñ�[�������Ա�] "<< strClass <<" ��ʼ�ȼ�����Ϊ1";
				CfgChk::GenExpInfo(ExpStr.str());
			}
		}
		else
		{
			if (uLeftLevel != (*itr).second.uMaxLevel + 1)
			{
				stringstream ExpStr;
				ExpStr << "���ñ�[�������Ա�] "<< strClass <<" ��д�ĵȼ� ������ �� �ص�";
				CfgChk::GenExpInfo(ExpStr.str());
			}
			else
			{
				(*itr).second.uMaxLevel = uRightLevel;
			}
		}
		// ְҵ & �ȼ� END

		CCfgCalc* pMaxHP					= new CCfgCalc(TabFile.GetString(i, "��������"));
		CCfgCalc* pMaxMP					= new CCfgCalc(TabFile.GetString(i, "ħ������"));
		CCfgCalc* pDPS						= new CCfgCalc(TabFile.GetString(i, "DPS"));
		CCfgCalc* pMagicDamage				= new CCfgCalc(TabFile.GetString(i, "����"));
		CCfgCalc* pDefence					= new CCfgCalc(TabFile.GetString(i, "����ֵ"));
		CCfgCalc* pPhysicalDodge			= new CCfgCalc(TabFile.GetString(i, "������ֵ"));
		CCfgCalc* pStrikeValue				= new CCfgCalc(TabFile.GetString(i, "����ֵ"));
		CCfgCalc* pMagicDodge				= new CCfgCalc(TabFile.GetString(i, "�������ֵ"));
		CCfgCalc* pNatureResistance			= new CCfgCalc(TabFile.GetString(i, "��Ȼ��ֵ"));
		CCfgCalc* pDestructionResistance	= new CCfgCalc(TabFile.GetString(i, "�ƻ�����"));
		CCfgCalc* pEvilResistance			= new CCfgCalc(TabFile.GetString(i, "���ڿ���"));
		CCfgCalc* pAccuratenessValue		= new CCfgCalc(TabFile.GetString(i, "��׼ֵ"));			
		CCfgCalc* pMagicHitValue			= new CCfgCalc(TabFile.GetString(i, "��������"));
		CCfgCalc* pStrikeMultiValue			= new CCfgCalc(TabFile.GetString(i, "��������"));			
		CCfgCalc* pResilienceValue			= new CCfgCalc(TabFile.GetString(i, "����ֵ"));	
		CCfgCalc* pStrikeResistanceValue			= new CCfgCalc(TabFile.GetString(i, "�Ⱪֵ"));	
		CCfgCalc* pParryValue				= new CCfgCalc(TabFile.GetString(i, "�м�ֵ"));
		CCfgCalc* pNatureSmashValue			= new CCfgCalc(TabFile.GetString(i, "��Ȼ��ѹֵ"));
		CCfgCalc* pDestructionSmashValue	= new CCfgCalc(TabFile.GetString(i, "�ƻ���ѹֵ"));
		CCfgCalc* pEvilSmashValue			= new CCfgCalc(TabFile.GetString(i, "������ѹֵ"));
		CCfgCalc* pDefenceSmashValue		= new CCfgCalc(TabFile.GetString(i, "������ѹֵ"));
		pMaxHP->SetTypeExpression();
		pMaxMP->SetTypeExpression();
		pDPS->SetTypeExpression();
		pMagicDamage->SetTypeExpression();
		pDefence->SetTypeExpression();
		pPhysicalDodge->SetTypeExpression();
		pStrikeValue->SetTypeExpression();
		pMagicDodge->SetTypeExpression();
		pNatureResistance->SetTypeExpression();
		pDestructionResistance->SetTypeExpression();
		pEvilResistance->SetTypeExpression();
		pAccuratenessValue->SetTypeExpression();
		pMagicHitValue->SetTypeExpression();
		pStrikeMultiValue->SetTypeExpression();
		pStrikeResistanceValue->SetTypeExpression();
		pResilienceValue->SetTypeExpression();
		pParryValue->SetTypeExpression();
		pNatureSmashValue->SetTypeExpression();
		pDestructionSmashValue->SetTypeExpression();
		pEvilSmashValue->SetTypeExpression();
		pDefenceSmashValue->SetTypeExpression();

		delete pLevel;
		delete pMaxHP;
		delete pMaxMP;
		delete pDPS;
		delete pMagicDamage;
		delete pDefence;
		delete pPhysicalDodge;
		delete pStrikeValue;
		delete pMagicDodge;
		delete pNatureResistance;
		delete pDestructionResistance;
		delete pEvilResistance;
		delete pAccuratenessValue;
		delete pMagicHitValue;
		delete pStrikeMultiValue;
		delete pResilienceValue;
		delete pStrikeResistanceValue;
		delete pParryValue;
		delete pNatureSmashValue;
		delete pDestructionSmashValue;
		delete pEvilSmashValue;
		delete pDefenceSmashValue;
	}

	return true;
}

void CCfgPropertyCheck::EndCheck()
{
	CPropertyCfg::ms_mapClassType.clear();
}
