#include "stdafx.h"
#include "CCfgNPCFighterBaseDataCheck.h"
#include "CTxtTableFile.h"
#include "CCfgSkillCheck.h"
#include "CCfgColChecker.inl"
#include "CCfgNormalAttackCheck.h"
#include "CCfgSkillRuleCheck.h"

CCfgNPCFighterBaseDataCheck::MapNpcFightBaseData	CCfgNPCFighterBaseDataCheck::ms_mapNpcFightBaseData;
CCfgNPCFighterBaseDataCheck::MapName2SkillRule CCfgNPCFighterBaseDataCheck::ms_mapName2SkillRule;

bool CCfgNPCFighterBaseDataCheck::Check(const TCHAR* cfgFile)
{
	CTxtTableFile TableFile;
	Load(TableFile, cfgFile, "NpcFightBaseData_Server");
	string	sBornSkill, strNorAttackName;
	bool bSucc = true;
	for(int32 i = 1; i < TableFile.GetHeight(); ++i)
	{
		string sNpcName = TableFile.GetString(i,"����");
		string strSkillRule = TableFile.GetString(i, "���ܹ���");
		CheckOverlap("����", i);
		sBornSkill = TableFile.GetString(i, "�����ͷż���");
		strNorAttackName = TableFile.GetString(i, "��ͨ����");
		int32 iRandMaxAttackSpeed;
		iRandMaxAttackSpeed	= TableFile.GetInteger(i, "�����������ٶ�", -1);
		if (iRandMaxAttackSpeed < 0)
		{
			ostringstream ExpStr;
			ExpStr << "��NpcFightBaseData_Server�����ñ��" << i << "�е������������ٶ�С��0!";
			CfgChk::GenExpInfo(ExpStr.str());
		}
		CheckNpcBornSkill(sBornSkill,i);
		CheckNpcNormalAttack(strNorAttackName, i);
		if ( !MakeRuleMap(sNpcName, strSkillRule) )
			bSucc = false;
		
		CCfgNPCFighterBaseDataCheck* pCfg = new CCfgNPCFighterBaseDataCheck();
		pCfg->m_uAttackDist = TableFile.GetInteger(i, "�չ�����", 0);
		MapNpcFightBaseData::iterator iter = ms_mapNpcFightBaseData.find(sNpcName);
		if (iter != ms_mapNpcFightBaseData.end())
		{
			ostringstream ExpStr;
			ExpStr << "��NpcFightBaseData_Server�����ñ��" << i << "�е�Npc���Ѵ���!";
			CfgChk::GenExpInfo(ExpStr.str());
		}
		else
		{
			ms_mapNpcFightBaseData.insert(make_pair(sNpcName,pCfg));
		}
	}
	return bSucc;
}

void CCfgNPCFighterBaseDataCheck::CheckNpcNormalAttack(string& sAttackName, uint32 uLineNum)
{	
	if (!CCfgNormalAttackCheck::IsNPCNormalAttackNameValid(sAttackName))
	{
		ostringstream ExpStr;
		ExpStr << "��NpcFightBaseData_Server�����ñ��" << uLineNum << "�е�[" << uLineNum+1 << "]Npc�չ���Npc���չ����в�����!";
		CfgChk::GenExpInfo(ExpStr.str());
	}
}

void CCfgNPCFighterBaseDataCheck::CheckNpcBornSkill(string& strSkill, uint32 uLineNum)
{
	if(strSkill.empty())
		return;
	erase(strSkill, " ");
	erase(strSkill,"\"");
	replace(strSkill, "��", ",");
	int32 startPos = -1;
	string strTemp;
	while (true)
	{
		if(strSkill.empty())
			return;
		startPos = strSkill.find(',');
		if(-1 == startPos)
		{
			if (!CCfgSkillCheck::IsNPCSkillNameValid(strSkill.c_str()))
			{
				ostringstream ExpStr;
				ExpStr << "��NpcFightBaseData_Server�����ñ��" << uLineNum << "�е�[" << strSkill << "]���ܲ�����!";
				CfgChk::GenExpInfo(ExpStr.str());
			}
			return;
		}
		strTemp = strSkill.substr(0,startPos);
		if(strTemp.empty())
		{
			if(startPos < int32(strSkill.length())-1)
			{
				ostringstream ExpStr;
				ExpStr << "��NpcFightBaseData_Server�����ñ��" << uLineNum << "�еĳ����ͷż����и�ʽ���ԣ��������� ,, �����"<< endl;
				CfgChk::GenExpInfo(ExpStr.str());
			}
			else
				return;
		}
		if (!CCfgSkillCheck::IsNPCSkillNameValid(strTemp.c_str()))
		{
			ostringstream ExpStr;
			ExpStr << "��NpcFightBaseData_Server�����ñ��" << uLineNum << "�е�[" << strTemp << "]���ܲ�����!";
			CfgChk::GenExpInfo(ExpStr.str());
		}
		strSkill = strSkill.substr(startPos+1,strSkill.length());
	}
}

void CCfgNPCFighterBaseDataCheck::EndCheck()
{
	EndCheckOverlap();
}

bool CCfgNPCFighterBaseDataCheck::BeExist(string sNpcName)
{
	if (ms_mapNpcFightBaseData.find(sNpcName) != ms_mapNpcFightBaseData.end())
		return true;
	return false;
}

uint32 CCfgNPCFighterBaseDataCheck::GetAttackDistByName(const TCHAR* szName)
{
	string strNpcName(szName);
	MapNpcFightBaseData::iterator iter = ms_mapNpcFightBaseData.find(strNpcName);
	if (iter != ms_mapNpcFightBaseData.end())
	{
		return iter->second->m_uAttackDist;
	}
	return 0;
}

bool CCfgNPCFighterBaseDataCheck::MakeRuleMap(string& strNpcName, string& strSkillRule)
{
	if(strSkillRule.empty())
		return true;
	erase(strSkillRule, " ");
	erase(strSkillRule,"\"");
	replace(strSkillRule, "��", ",");
	replace(strSkillRule, "��","(");
	replace(strSkillRule, "��", ")");

	LstSkillRuleName lstSkillRuleName;

	bool bSucc = true;
	if (strSkillRule.find(",") == -1) //˵��ֻ��һ�����ܹ����Npc
	{
		if ( !CCfgSkillRuleCheck::CheckExist(strSkillRule) )
		{
			cout << "��NpcFightBaseData_Server�����ñ�Npc��Ϊ��" << strNpcName << "���ļ��ܹ���" << strSkillRule << "����SkillRuleBaseData_Server������" <<endl;
			bSucc = false;
		}
		else
		{
			//ms_mapName2SkillRule.insert(make_pair(strNpcName, strSkillRule));
			lstSkillRuleName.insert(strSkillRule);
			ms_mapName2SkillRule.insert(make_pair(strNpcName, lstSkillRuleName));
		}
	}
	else
	{
		uint32 uNumber = 0;
		//�������ж�����ܹ�������
		while (true)
		{
			if(strSkillRule.empty())
				break;
			string strSkillRuleNode;
			int32 uNext = strSkillRule.find(",");
			if (uNext != -1)
			{
				strSkillRuleNode = strSkillRule.substr(0, uNext);
				strSkillRule = strSkillRule.substr(uNext+1, strSkillRule.length());
			}
			else
			{
				strSkillRuleNode = strSkillRule;
				strSkillRule.clear();
			}
			Ast (!strSkillRuleNode.empty());
			int32 uStatrt = strSkillRuleNode.find("(");
			int32 uEnd = strSkillRuleNode.find(")");
			if (uStatrt == -1 || uEnd ==-1)
			{
				ostringstream strm;
				strm<<"NpcFightBaseData_Server���� Npc��"<< strNpcName<<" �ļ��ܹ�����д����"<< endl;
				GenErr(strm.str());
			}
			string strSkillRuleName = strSkillRuleNode.substr(0, uStatrt);
			string strNum = strSkillRuleNode.substr(uStatrt+1, uEnd-uStatrt-1);
			uNumber = uNumber + static_cast<uint32>(atoi(strNum.c_str()));
			if ( !CCfgSkillRuleCheck::CheckExist(strSkillRuleName) )
			{
				cout << "��NpcFightBaseData_Server�����ñ�Npc��Ϊ��" << strNpcName << "���ļ��ܹ���" << strSkillRuleName << "����SkillRuleBaseData_Server������" <<endl;
				bSucc = false;
			}
			else
			{
				lstSkillRuleName.insert(strSkillRuleName);
			}
		}
		if (uNumber != 100)
		{
			ostringstream strm;
			strm<<"NpcFightBaseData_Server���� Npc��"<< strNpcName<<" �ļ��ܹ����ܸ��ʲ�Ϊ100�����ʵ��"<< endl;
			GenErr(strm.str());
		}
		ms_mapName2SkillRule.insert(make_pair(strNpcName, lstSkillRuleName));
	}
	return bSucc;
}

bool CCfgNPCFighterBaseDataCheck::CheckSkillRule(string& strNpcName, string& strSkillRuleName )
{
	if (ms_mapName2SkillRule.find(strNpcName) != ms_mapName2SkillRule.end())
	{
		LstSkillRuleName& lstSkillRuleName = ms_mapName2SkillRule[strNpcName];
		if (lstSkillRuleName.find(strSkillRuleName) != lstSkillRuleName.end())
			return false;
	}
	return true;
}
