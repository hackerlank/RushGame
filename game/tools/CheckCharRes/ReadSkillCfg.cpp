#include "stdafx.h"
#include "ReadSkillCfg.h"
#include "CTableFile.h"
#include "LoadSkillCommon.h"


MapSkillCfgCheck CSkillCfgCheck::m_mapSkill;

bool CSkillCfgCheck::CheckConfig(const string& szFileName)
{   
	CTabFile TabFile; 
	if(!TabFile.Load(szFileName.c_str()))
	{
		cout << "�ļ� [" << szFileName << "] ������\n";
		return false;
	}

	for( int i = 1; i < TabFile.GetHeight(); i++ )
	{
		CSkillCfgCheck* pCfg = new CSkillCfgCheck;
		pCfg->m_uId						= i;
		pCfg->m_sName					= TabFile.GetString(i, szSkill_Name);
		pCfg->m_sBeginSkillAni			= DeQuot(TabFile.GetString(i, szSkill_ReachUpAction));
		pCfg->m_sSkillFX				= DeQuot(TabFile.GetString(i, szSkill_ReachUpEffect));
		
		pair<MapSkillCfgCheck::iterator, bool> pr = m_mapSkill.insert(make_pair(pCfg->m_sName, pCfg));
		if(!pr.second)
		{
			stringstream ExpStr;
			ExpStr << "��" << i << "�е�" << "����" << pCfg->m_sName << "�ظ�";
			GenErr(ExpStr.str());
		}

		PropResChecker::CheckSkillAllAnim(pCfg->m_sName, pCfg->m_sBeginSkillAni, "���ܿ�ʼ����");
		PropResChecker::CheckSkillAllEff(pCfg->m_sName, pCfg->m_sSkillFX, true, "������Ч");
	}
	return true;
}

MapCastingCfgCheck CCastingCfgCheck::m_mapCasting;

bool CCastingCfgCheck::CheckConfig(const string& szFileName)
{
	CTabFile TabFile;
	if(!TabFile.Load(szFileName.c_str()))
	{
		cout << "�ļ� [" << szFileName << "] ������\n";
		return false;
	}

	for( int i = 1; i < TabFile.GetHeight(); i++ )
	{
		CCastingCfgCheck* pCfg = new CCastingCfgCheck;
		pCfg->m_uId						= i;
		pCfg->m_sName					= TabFile.GetString(i, szCasting_Name);
		pCfg->m_sAni					= DeQuot(TabFile.GetString(i, szProcessAction));
		pCfg->m_sFX						= DeQuot(TabFile.GetString(i, szProcessEffect));

		pair<MapCastingCfgCheck::iterator, bool> pr = m_mapCasting.insert(make_pair(pCfg->m_sName, pCfg));
		if(!pr.second)
		{
			stringstream ExpStr;
			ExpStr << "��" << i << "�е�" << "����" << pCfg->m_sName << "�ظ�";
			GenErr(ExpStr.str());
		}

		PropResChecker::CheckSkillAllAnim(pCfg->m_sName, pCfg->m_sAni, "���̶���");
		PropResChecker::CheckSkillAllEff(pCfg->m_sName, pCfg->m_sFX, true, "������Ч");
	}
	return true;
}

MapMagicOpCfgCheck CMagicOpCfgCheck::m_mapMagicOp;

bool CMagicOpCfgCheck::CheckConfig(const string& szFileName)
{
	CTabFile TabFile;
	if(!TabFile.Load(szFileName.c_str()))
	{
		cout << "�ļ� [" << szFileName << "] ������\n";
		return false;
	}

	for( int i = 1; i < TabFile.GetHeight(); i++ )
	{
		CMagicOpCfgCheck* pCfg = new CMagicOpCfgCheck;
		pCfg->m_uId						= i;
		pCfg->m_sName					= TabFile.GetString(i, szMagicEff_Name);
		pCfg->m_sFX						= DeQuot(TabFile.GetString(i, szMagicEff_FXName));

		pair<MapMagicOpCfgCheck::iterator, bool> pr = m_mapMagicOp.insert(make_pair(pCfg->m_uId, pCfg));
		if(!pr.second)
		{
			stringstream ExpStr;
			ExpStr << "��" << i << "�е�" << "����" << pCfg->m_sName << "�ظ�";
			GenErr(ExpStr.str());
		}
		
		PropResChecker::CheckSkillAllEff(pCfg->m_sName, pCfg->m_sFX, false, "ħ��������Ч");
	}
	return true;
}

MapStateCfgCheck CStateCfgCheck::m_mapState;

bool CStateCfgCheck::CheckConfig(const string& szFileName, const string& szTypeName)
{
	CTabFile TabFile;
	if(!TabFile.Load(szFileName.c_str()))
	{
		cout << "�ļ� [" << szFileName << "] ������\n";
		return false;
	}

	for( int i = 1; i < TabFile.GetHeight(); i++ )
	{
		CStateCfgCheck* pCfg = new CStateCfgCheck;
		pCfg->m_uId						= i;
		pCfg->m_sName					= TabFile.GetString(i, "����");
		pCfg->m_sModel					= DeQuot(TabFile.GetString(i, "ģ��"));
		pCfg->m_sFX						= DeQuot(TabFile.GetString(i, "��Ӧ��Ч���"));

		pair<MapStateCfgCheck::iterator, bool> pr = m_mapState.insert(make_pair(pCfg->m_sName, pCfg));
		if(!pr.second)
		{
			stringstream ExpStr;
			ExpStr << "��" << i << "�е�" << szTypeName << "״̬" << pCfg->m_sName << "�ظ�";
			GenErr(ExpStr.str());
		}
		
		string sEffTitle = szTypeName + "״̬��Ч";
		PropResChecker::CheckSkillAllEff(pCfg->m_sName, pCfg->m_sFX, false, sEffTitle);
	}
	return true;
}

MapStanceSkillCfgCheck CStanceSkillCfgCheck::m_mapStanceSkill;

bool CStanceSkillCfgCheck::CheckConfig(const string& szFileName)
{
	CTabFile TabFile;
	if(!TabFile.Load(szFileName.c_str()))
	{
		cout << "�ļ� [" << szFileName << "] ������\n";
		return false;
	}

	for( int i = 1; i < TabFile.GetHeight(); i++ )
	{
		CStanceSkillCfgCheck* pCfg = new CStanceSkillCfgCheck;
		pCfg->m_uId						= i;
		pCfg->m_sName					= TabFile.GetString(i, "����");
		pCfg->m_sFX						= DeQuot(TabFile.GetString(i, "��Ч��"));

		pair<MapStanceSkillCfgCheck::iterator, bool> pr = m_mapStanceSkill.insert(make_pair(pCfg->m_sName, pCfg));
		if(!pr.second)
		{
			stringstream ExpStr;
			ExpStr << "��" << i << "�е�" << "��̬����" << pCfg->m_sName << "�ظ�";
			GenErr(ExpStr.str());
		}

		PropResChecker::CheckSkillAllEff(pCfg->m_sName, pCfg->m_sFX, false, "��̬������Ч");
	}
	return true;
}

MapBulletMagicCfgCheck CBulletMagicCfgCheck::m_mapBulletMagic;

bool CBulletMagicCfgCheck::CheckConfig(const string& szFileName)
{
	CTabFile TabFile;
	if(!TabFile.Load(szFileName.c_str()))
	{
		cout << "�ļ� [" << szFileName << "] ������\n";
		return false;
	}

	for( int i = 1; i < TabFile.GetHeight(); i++ )
	{
		CBulletMagicCfgCheck* pCfg = new CBulletMagicCfgCheck;
		pCfg->m_uId						= i;
		pCfg->m_sName					= TabFile.GetString(i, "����");
		pCfg->m_sModel					= DeQuot(TabFile.GetString(i, "ģ��"));
		pCfg->m_sFX						= DeQuot(TabFile.GetString(i, "������Ч�ļ���"));
		pCfg->m_sExplodeFX				= DeQuot(TabFile.GetString(i, "������Ч�ļ���"));

		pair<MapBulletMagicCfgCheck::iterator, bool> pr = m_mapBulletMagic.insert(make_pair(pCfg->m_sName, pCfg));
		if(!pr.second)
		{
			stringstream ExpStr;
			ExpStr << "��" << i << "�е�" << "��̬����" << pCfg->m_sName << "�ظ�";
			GenErr(ExpStr.str());
		}

		PropResChecker::CheckSkillAllEff(pCfg->m_sName, pCfg->m_sFX, false, "�ӵ�ħ��������Ч");
		PropResChecker::CheckSkillAllEff(pCfg->m_sName, pCfg->m_sExplodeFX, false, "�ӵ�ħ����ը��Ч");
	}
	return true;
}

MapMagicCfgCheck CMagicCfgCheck::m_mapMagic;

bool CMagicCfgCheck::CheckConfig(const string& szFileName, const string& szTypeName)
{
	CTabFile TabFile;
	if(!TabFile.Load(szFileName.c_str()))
	{
		cout << "�ļ� [" << szFileName << "] ������\n";
		return false;
	}

	for( int i = 1; i < TabFile.GetHeight(); i++ )
	{
		CMagicCfgCheck* pCfg = new CMagicCfgCheck;
		pCfg->m_uId						= i;
		pCfg->m_sName					= TabFile.GetString(i, "����");
		pCfg->m_sModel					= DeQuot(TabFile.GetString(i, szTypeName == "�����" ? "�ӵ�ģ��" : "ģ��"));
		pCfg->m_sFX						= DeQuot(TabFile.GetString(i, "��Ч��"));

		pair<MapMagicCfgCheck::iterator, bool> pr = m_mapMagic.insert(make_pair(pCfg->m_sName, pCfg));
		if(!pr.second)
		{
			stringstream ExpStr;
			ExpStr << "��" << i << "�е�" << szTypeName << "ħ��" << pCfg->m_sName << "�ظ�";
			GenErr(ExpStr.str());
		}

		string sEffTitle = szTypeName + "ħ����Ч";
		PropResChecker::CheckSkillAllEff(pCfg->m_sName, pCfg->m_sFX, false, sEffTitle);
	}
	return true;
}

bool CheckAllConfig()
{
	string pathSkill = g_SettingCfgPath + "/skill/";
	string skill = pathSkill + "Skill.txt";
	string stanceskill = pathSkill + "StanceSkill.txt";
	string magicEffect = pathSkill + "MagicEffect.txt";
	string castingProcess  = pathSkill + "CastingProcess.txt";
	string magicState = pathSkill + "MagicState.txt";
	string bulletMagic = pathSkill + "BulletMagic.txt";
	string moveMagic = pathSkill + "MoveMagic.txt";
	string transMagic = pathSkill + "TransferableMagic.txt";
	string shockWaveMagic = pathSkill + "ShockWaveMagic.txt";
	string positionMagic = pathSkill + "PositionMagic.txt";
	string totemMagic = pathSkill + "TotemMagic.txt";
	string triggerState = pathSkill + "TriggerState.txt";
	string damageState = pathSkill + "DamageChangeState.txt";
	string specialState = pathSkill + "SpecialState.txt";
	string normalAttack = pathSkill + "NormalAttack.txt";
	
	CSkillCfgCheck::CheckConfig(skill);
	CCastingCfgCheck::CheckConfig(castingProcess);
	CMagicOpCfgCheck::CheckConfig(magicEffect);
	CStateCfgCheck::CheckConfig(magicState, "ħ��");
	CStateCfgCheck::CheckConfig(triggerState, "������");
	CStateCfgCheck::CheckConfig(damageState, "�˺����");
	CStateCfgCheck::CheckConfig(specialState, "����");
	CStanceSkillCfgCheck::CheckConfig(stanceskill);
	CMagicCfgCheck::CheckConfig(moveMagic, "λ��");
	CBulletMagicCfgCheck::CheckConfig(bulletMagic);
	CMagicCfgCheck::CheckConfig(shockWaveMagic, "�����");
	CMagicCfgCheck::CheckConfig(transMagic, "����");
	CMagicCfgCheck::CheckConfig(positionMagic, "λ��");
	CMagicCfgCheck::CheckConfig(totemMagic, "ͼ��");

	return true;
}


