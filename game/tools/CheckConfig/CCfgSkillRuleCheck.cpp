#include "stdafx.h"
#include "CCfgSkillRuleCheck.h"
#include "CCfgColChecker.inl"
#include "CTxtTableFile.h"
#include "CCfgSkillCheck.h"
#include "CCfgProcessCheck.h"
#include "CCfgAllStateCheck.h"

CCfgSkillRuleCheck::MapSkillRuleName CCfgSkillRuleCheck::ms_mapSkillRuleName;
TypeName2SRTarget CCfgSkillRule::ms_mapSRTargetType;

bool CCfgSkillRuleCheck::Check(const TCHAR* cfgFile)
{
	using namespace CfgChk;
	
	bool bSucc = true;
	CTxtTableFile TableFile;
	SetTabFile(TableFile, "SkillRuleBaseData_Server");
	if (!TableFile.Load(PATH_ALIAS_CFG.c_str(), cfgFile))
	{
		stringstream ExpStr;
		ExpStr << " ���ñ� ����ʧ�ܣ���鿴�ļ���[" << cfgFile << "]�Ƿ���ȷ�����ļ��Ƿ����";
		GenExpInfo(ExpStr.str());
	}
	bool bNewSec = true;
	for(int32 i = 1; i < TableFile.GetHeight(); ++i)
	{
		SetLineNo(i);
		string strName;
		ReadItem(strName, "����",	CANEMPTY);
		if( strName.empty() )
		{
			bNewSec = true;
			continue;
		}

		string strSkillName, strTarget, strSkillArg;
		ReadItem(strSkillName, "��������",	CANEMPTY);
		ReadItem(strTarget,"ʩ��Ŀ��",CANEMPTY);
		if(strSkillName == "��ѡһ" || strSkillName == "��Χ�ص��ͷż���" || strSkillName == "����" 
			|| strSkillName == "���غ���" || strSkillName == "���������ͷ�" || strSkillName == "����Ч����"
			|| strSkillName == "���Ⱦ���" || strSkillName == "״̬�����ͷ�")
		{
			ReadItem(strSkillArg,"���ܲ���",MUSTFILL);
		}

		string	strBeginCond;
		ReadItem(strBeginCond,"��ʼ����",CANEMPTY);
		string	strContinueCond;
		ReadItem(strContinueCond,"���з�ʽ",CANEMPTY);
		int32 iSkillPage = 0;
		ReadItem(iSkillPage,"�׶�");
		if (iSkillPage != -1 && iSkillPage < 0)
		{
			stringstream ExpStr;
			ExpStr << "��" << i << "�е�" << "�׶�" << iSkillPage << "���Ϸ����������0��Ϊ-1";
			GenExpInfo(ExpStr.str());
		}
		if (!CreateRuleCondData(strName,strBeginCond,strContinueCond,strSkillName,iSkillPage))
		{
			bSucc = false;
		}
		
		string	strDistance;
		ReadItem(strDistance,"���ܾ���",CANEMPTY);
		if (!CreateRuleDistance(strName, strSkillName, strDistance))
		{
			bSucc = false;
		}

		trimend(strName);
		static CCfgSkillRuleCheck* pCfgSkillRuleCheck;
		if (bNewSec)
		{
			bNewSec = false;
			MapSkillRuleName::iterator iter = ms_mapSkillRuleName.find(strName);
			if (iter != ms_mapSkillRuleName.end())
			{
				stringstream ExpStr;
				ExpStr << "��" << i << "�е�" << "ȫ�ּ��ܲ���" << strName << "�ظ�";
				GenExpInfo(ExpStr.str());
			}
			else
			{
				pCfgSkillRuleCheck = new CCfgSkillRuleCheck();
				ms_mapSkillRuleName.insert(make_pair(strName, pCfgSkillRuleCheck));
			}
		}

		CCfgSkillRule* pCfgSkillRule = CreateCfgSkillRule(strName, strSkillName, strTarget, strSkillArg);
		if (!pCfgSkillRule)
		{
			bSucc = false;
		}
		else
		{
			pCfgSkillRuleCheck->m_vecChild.push_back(pCfgSkillRule);
		}
	}
	if (!CheckRuleSkill())
	{
		GenExpInfo("��SkillRuleBaseData_Server�����ñ���д����");
	}

	return bSucc;
}

void CCfgSkillRuleCheck::EndCheck()
{
	ClearMap(ms_mapSkillRuleName);
}

CCfgSkillRuleCheck::~CCfgSkillRuleCheck()
{
	ClearVector(m_vecChild);
}

bool CCfgSkillRuleCheck::CheckRuleSkill()
{
	bool bSuccess = true;
	MapSkillRuleName::const_iterator itr = ms_mapSkillRuleName.begin();
	for (; itr != ms_mapSkillRuleName.end(); ++itr)
	{
		const string sRuleName = (*itr).first;
		VecChild::const_iterator itr_lst = ((*itr).second)->m_vecChild.begin();
		for (; itr_lst != ((*itr).second)->m_vecChild.end(); ++itr_lst)
		{
			ESRTargetType eTargetType = (*itr_lst)->m_eSRTargetType;
			string strTempSkillName = (*itr_lst)->m_strSkillName;
			if (strTempSkillName == "�л��׶�" || strTempSkillName == "����ı�Ŀ��" || strTempSkillName == "����" 
				|| strTempSkillName == "���غ���" || strTempSkillName == "����Ч����" || strTempSkillName == "���Ⱦ���"
				|| strTempSkillName == "����ı��NpcĿ��")
			{
				continue;
			}
			else if (strTempSkillName == "��ѡһ" || strTempSkillName == "��Χ�ص��ͷż���" )
			{
				const CCfgSkillRule::SkillNameLst lstSkillName = (*itr_lst)->m_lstMultiArgs;
				CCfgSkillRule::SkillNameLst::const_iterator itr_skill = lstSkillName.begin();
				for (; itr_skill != lstSkillName.end(); ++itr_skill)
				{
					string sSkillName = *itr_skill;
					CCfgSkillCheck::MapSkill::iterator itr = CCfgSkillCheck::ms_mapSkill.find(sSkillName);
					if (itr == CCfgSkillCheck::ms_mapSkill.end())
					{
						cout<<"Npc���ܹ�����й���"<<sRuleName<<"���ͷŵļ��ܡ�"<<sSkillName<<"���ڡ�NpcSkill.ndf���в����ڣ�"<<endl;
						bSuccess = false;
					}
					if (!CheckRuleSkillTargetType(sRuleName, sSkillName, eTargetType))
						bSuccess = false;
				}
			}
			else if (strTempSkillName == "���������ͷ�" || strTempSkillName == "״̬�����ͷ�")
			{
				const CCfgSkillRule::SkillNameLst lstSkillName = (*itr_lst)->m_lstMultiArgs;
				CCfgSkillRule::SkillNameLst::const_iterator itr_skill = lstSkillName.end();
				string skillName = *(--itr_skill);
				CCfgSkillCheck::MapSkill::iterator itr = CCfgSkillCheck::ms_mapSkill.find(skillName);
				if (itr == CCfgSkillCheck::ms_mapSkill.end())
				{
					cout<<"Npc���ܹ�����й���"<<sRuleName<<"���ͷŵļ��ܡ�"<<strTempSkillName<<"���Ĳ�����" << skillName <<  "���ڡ�NpcSkill.ndf���в����ڣ�"<<endl;
					bSuccess = false;
				}

				if (strTempSkillName == "״̬�����ͷ�" )
				{
					string strStateName = *(--itr_skill);
					if(!CCfgMagicStateCheck::CheckExist(strStateName))
					{
						cout << "Npc���ܹ�����й���"<<sRuleName<<"������ħ��״̬����" << strTempSkillName << "���Ĳ�����" << strStateName << "����ħ��״̬���в�����" <<endl;
						bSuccess = false;
					}
				}
			}
			else
			{
				CCfgSkillCheck::MapSkill::iterator itr = CCfgSkillCheck::ms_mapSkill.find(strTempSkillName);
				if (itr == CCfgSkillCheck::ms_mapSkill.end())
				{
					cout<<"Npc���ܹ�����й���"<<sRuleName<<"���ͷŵļ��ܡ�"<<strTempSkillName<<"���ڡ�NpcSkill.ndf���в����ڣ�"<<endl;
					bSuccess = false;
				}
				if (!CheckRuleSkillTargetType(sRuleName, strTempSkillName, eTargetType))
					bSuccess = false;
			}
		}
	}
	
	return bSuccess;
}

bool CCfgSkillRuleCheck::CheckRuleSkillTargetType(const string sRuleName, string sSkillName, ESRTargetType eTargetType)
{
	bool bSuccess = true;
	if (CCfgSkillCheck::IsNPCSkillNameValid(sSkillName.c_str()))
	{
		CCfgSkillCheck::MapSkill::iterator itr = CCfgSkillCheck::ms_mapSkill.find(sSkillName);
		if (itr != CCfgSkillCheck::ms_mapSkill.end())
		{
			CCfgMagicEffCheck* pNpcMagicEff = (*itr).second->GetMagicEff();
			if (pNpcMagicEff && !pNpcMagicEff->CheckNpcSkillFilterObj2MagicEff(sRuleName, sSkillName, eTargetType))
				bSuccess = false;

			CCfgProcessCheck::MapProcess::iterator itr = CCfgProcessCheck::ms_mapProcess.find(sSkillName);
			if (itr != CCfgProcessCheck::ms_mapProcess.end())
			{
				CCfgMagicEffCheck* pCastSkillMagicEff = (*itr).second->GetMagicEff();
				if (pCastSkillMagicEff && !pCastSkillMagicEff->CheckNpcSkillFilterObj2MagicEff(sRuleName, sSkillName, eTargetType))
					bSuccess = false;
			}
		}
	}
	return bSuccess;
}

uint32 CCfgSkillRuleCheck::GetArgNum(ESRTargetType eSRTargetType)
{
	if (eSRTargetType > eSRTT_NoArg && eSRTargetType < eSRRT_OneArg)
	{
		return 0;
	}
	else if ( eSRTargetType > eSRRT_OneArg && eSRTargetType < eSRTT_TwoArg)
	{
		return 1;
	}
	else
		return 2;
}

bool CCfgSkillRuleCheck::CreateRuleCondData(string &strName,string &strBegin,string &strContiune,string &strSkillName, int32 iSkillPage)
{
	bool bRet1 = ParseRuleCond(strName,strBegin,eRCK_Begin,strSkillName,iSkillPage);
	bool bRet2 = ParseRuleCond(strName,strContiune,eRCK_Continue,strSkillName,iSkillPage);
	return bRet1 && bRet2;
}

bool CCfgSkillRuleCheck::ParseRuleCond(string &strName,string& strCond,ERuleCondKind eRuleKind,string &strSkillName, int32 iSkillPage)
{
	if(strCond.empty())
		return true;
	bool bSucc = true;
	erase(strCond, " ");
	erase(strCond,"\"");
	replace(strCond, "��", ",");
	replace(strCond, "��", "(");
	replace(strCond, "��", ")");
	int32 startPos = -1;
	string strNameTemp;
	string strArgTemp;
	string strTemp;
	while (true)
	{
		if(strCond.empty())
			return true;
		startPos = strCond.find(',');
		if(-1 == startPos)
		{
			int32 lbracketPos,rbracketPos;
			lbracketPos = strCond.find('(');
			rbracketPos = strCond.find(')');
			if (-1 == lbracketPos || -1 == rbracketPos)
			{
				ostringstream strm;
				strm<<"���ܹ���<"<<strName<<">����<"<<strSkillName<<">�ﴥ������������������ȱ�ٻ�ƥ��"<< endl;
				cout<<strm.str()<<endl;
				bSucc = false;
			}
			strNameTemp = strCond.substr(0,lbracketPos);
			strArgTemp	= strCond.substr(lbracketPos+1,rbracketPos-lbracketPos-1);
			if(!CCfgSkillRule::ms_mapSRTargetType.mapCondType.count(strNameTemp))
			{
				ostringstream strm;
				strm<<"���ܹ���<"<<strName<<">����<"<<strSkillName<<">�Ĺ�������<"<<strNameTemp<<">������!"<<endl;;
				cout<<strm.str()<<endl;
				bSucc = false;
			}
			return ParseRuleCondArg(strName,CCfgSkillRule::ms_mapSRTargetType.mapCondType[strNameTemp],strArgTemp,strSkillName,iSkillPage);
		}
		strTemp = strCond.substr(0,startPos);
		if(strTemp.empty())
		{
			if(startPos < int32(strCond.length())-1)
			{
				ostringstream strm;
				strm<<"���ܹ���<"<<strName<<">����<"<<strSkillName<<">�Ĺ�������<"<<strNameTemp<<">��ʽ���ԣ��������� ,, �����"<< endl;
				cout<<strm.str()<<endl;
				bSucc = false;
			}
			else
				return true;
		}
		else
		{
			int32 lbracketPos,rbracketPos;
			lbracketPos = strTemp.find('(');
			rbracketPos = strTemp.find(')');
			if (-1 == lbracketPos || -1 == rbracketPos)
			{
				ostringstream strm;
				strm<<"���ܹ���<"<<strName<<">����<"<<strSkillName<<">�Ĺ�������<"<<strNameTemp<<">��������������ȱ�ٻ�ƥ��"<< endl;
				cout<<strm.str()<<endl;
				bSucc = false;
			}
			strNameTemp = strTemp.substr(0,lbracketPos);
			strArgTemp	= strTemp.substr(lbracketPos+1,rbracketPos-lbracketPos-1);
			if(!CCfgSkillRule::ms_mapSRTargetType.mapCondType.count(strNameTemp))
			{
				ostringstream strm;
				strm<<"���ܹ���<"<<strName<<">����<"<<strSkillName<<">�Ĺ�������<"<<strNameTemp<<">����������!"<<endl;;
				cout<<strm.str()<<endl;
				bSucc = false;
			}
			bSucc = ParseRuleCondArg(strName,CCfgSkillRule::ms_mapSRTargetType.mapCondType[strNameTemp],strArgTemp,strSkillName,iSkillPage);
		}
		strCond = strCond.substr(startPos+1,strCond.length());
	}
	return bSucc;
}

bool CCfgSkillRuleCheck::ParseRuleCondArg(string &strName,ERCondType eRCType,string& strArgTemp,string &strSkillName,int32 iSkillPage)
{
	bool bSucc = true;
	if(strArgTemp.empty())
	{
		if(eRCT_EnterBattle != eRCType && eRCT_EnterPhase != eRCType
			&& eRCT_OnDeadCond != eRCType && eRCT_EnemyDead != eRCType)
		{
			ostringstream strm;
			strm<<"���ܹ���<"<<strName<<">����<"<<strSkillName<<">��������ȱ�ٲ���!"<<endl;;
			cout<<strm.str()<<endl;
			bSucc = false;
		}
	}
	else
	{
		if (eRCT_RangeTimingCond == eRCType)
		{
			uint32 uArgPos = strArgTemp.find(',');
			if(-1 == uArgPos)
			{
				ostringstream strm;
				strm<<"���ܹ���<"<<strName<<">����<"<<strSkillName<<">��Χ��ʱ������������Ӧ��Ϊ����!"<<endl;;
				cout<<strm.str()<<endl;
				bSucc = false;
			}
			string strOne = strArgTemp.substr(0,uArgPos);
			string strTwo = strArgTemp.substr(uArgPos+1,strArgTemp.length());
			if (strOne.empty())
			{
				ostringstream strm;
				strm<<"���ܹ���<"<<strName<<">����<"<<strSkillName<<">��Χ��ʱ����������һ������Ϊ��!"<<endl;;
				cout<<strm.str()<<endl;
				bSucc = false;
			}
			if (strTwo.empty())
			{
				ostringstream strm;
				strm<<"���ܹ���<"<<strName<<">����<"<<strSkillName<<">��Χ��ʱ���������ڶ�������Ϊ��!"<<endl;;
				cout<<strm.str()<<endl;
				bSucc = false;
			}
		}
		else if(eRCT_WanderTimeGreater == eRCType)
		{
			if (!CCfgSkillCheck::IsNPCSkillNameValid(strSkillName.c_str()))
			{
				ostringstream strm;
				strm<<"���ܹ���<"<<strName<<">����<"<<strSkillName<<">������!"<<endl;;
				cout<<strm.str()<<endl;
				bSucc = false;
			}
			if (CCfgSkillCheck::IsNPCSkillDoSkillRule(strSkillName.c_str()))
			{
				ostringstream strm;
				strm<<"���ܹ���<"<<strName<<">�ļ���<"<<strSkillName<<">���߼��ܹ����,�������ڴ�������!"<<endl;;
				cout<<strm.str()<<endl;
				bSucc = false;
			}
		}
	}
	
	if (iSkillPage == -1)
	{
		if (eRCType != eRCT_EnterWander && eRCType != eRCT_WanderTimeGreater && eRCType != eRCT_OnDeadCond)
		{
			ostringstream strm;
			strm<<"���ܹ���<"<<strName<<">�Ľ׶�<"<<iSkillPage<<">�����Ӧ��������������"<<endl;;
			cout<<strm.str()<<endl;
			bSucc = false;
		}
	}
	if (eRCType == eRCT_EnterWander || eRCType == eRCT_WanderTimeGreater || eRCType == eRCT_OnDeadCond)
	{
		if (iSkillPage != -1)
		{
			ostringstream strm;
			strm<<"���ܹ���<"<<strName<< ">���������������Ӧ�Ľ׶α���Ϊ-1, ������Ϊ<" << iSkillPage << ">" <<endl;;
			cout<<strm.str()<<endl;
			bSucc = false;
		}
	}
	
	return bSucc;
}

bool CCfgSkillRuleCheck::CreateRuleDistance(string& strName, string& strSkillName, string& strDistace)
{
	bool bSuccess = true;
	if (strDistace.empty())
	{
		ostringstream strm;
		strm<<"���ܹ���<"<<strName<<">����<"<<strSkillName<<">���ܾ��벻��Ϊ��!"<< endl;
		cout<<strm.str()<<endl;
		bSuccess = false;
	}
	erase(strDistace, " ");
	erase(strDistace,"\"");
	replace(strDistace, "��", ",");
	replace(strDistace, "��", "(");
	replace(strDistace, "��", ")");
	int32 lbracketPos,rbracketPos;
	int32 commaPos;
	lbracketPos = strDistace.find('(');
	rbracketPos = strDistace.find(')');
	commaPos = strDistace.find(',');
	if (-1 == lbracketPos || -1 == rbracketPos || -1 == commaPos)
	{
		ostringstream strm;
		strm<<"���ܹ���<"<<strName<<">����<"<<strSkillName<<">���ܾ����ʽ��д����!"<< endl;
		cout<<strm.str()<<endl;
		bSuccess = false;
	}
	string strOne = strDistace.substr(lbracketPos+1,commaPos-lbracketPos-1);
	string strTwo = strDistace.substr(commaPos+1,rbracketPos-commaPos-1);
	if (strOne.empty() || strTwo.empty())
	{
		ostringstream strm;
		strm<<"���ܹ���<"<<strName<<">����<"<<strSkillName<<">���ܾ���ȱ�ٲ���!"<< endl;
		cout<<strm.str()<<endl;
		bSuccess = false;
	}
	return bSuccess;
}

CCfgSkillRule* CCfgSkillRuleCheck::CreateCfgSkillRule(string strName, string strSkillName, string strTarget, string strSkillArg)
{
	bool bSuccess = true;
	if(strTarget.empty())
	{
		ostringstream strm;
		strm<<"���ܹ���<"<<strName<<">����<"<<strSkillName<<">���ͷ�Ŀ��Ϊ�գ�������!"<< endl;
		cout<<strm.str()<<endl;
		bSuccess = false;
	}

	string strSkillTargetName;
	uint32 uTargetArgs;
	erase(strTarget, " ");
	erase(strTarget,"\"");
	replace(strTarget, "��", ",");
	replace(strTarget, "��", "(");
	replace(strTarget, "��", ")");
	int32 lbracketPos, rbracketPos, commaPos;
	lbracketPos = strTarget.find('(');
	commaPos = strTarget.find(',');
	rbracketPos = strTarget.find(')');
	if (-1 == lbracketPos)
	{
		uTargetArgs = 0;
		strSkillTargetName = strTarget;
	}
	else
	{
		if (-1 == rbracketPos)
		{
			ostringstream strm;
			strm<<"���ܹ���<"<<strName<<">����<"<<strSkillName<<">�ͷ�Ŀ�� �������Ų�ƥ��!"<< endl;
			cout<<strm.str()<<endl;
			bSuccess = false;
		}

		string strRealTargetName = strTarget.substr(0,lbracketPos);
		strSkillTargetName = strRealTargetName;
		if (-1 == commaPos)
		{
			uTargetArgs = 1;
			string strArg = strTarget.substr(lbracketPos+1, rbracketPos-lbracketPos-1);
			if(strArg.empty())
			{
				ostringstream strm;
				strm<<"���ܹ���<"<<strName<<">����<"<<strSkillName<<">��Ŀ�����Ͳ���Ϊ�գ����ʵ!"<< endl;
				cout<<strm.str()<<endl;
				bSuccess = false;
			}
		}
		else
		{
			uTargetArgs = 2;
			string strArg1 = strTarget.substr(lbracketPos+1, commaPos-lbracketPos-1);
			string strArg2 = strTarget.substr(commaPos+1, rbracketPos-commaPos-1);
			if(strArg1.empty() || strArg2.empty())
			{
				ostringstream strm;
				strm<<"���ܹ���<"<<strName<<">����<"<<strSkillName<<">��Ŀ�����Ͳ���Ϊ�գ����ʵ!"<< endl;
				cout<<strm.str()<<endl;
				bSuccess = false;
			}
		}
	}
	ESRTargetType eSRTargetType = eSRTT_None;
	if (CCfgSkillRule::ms_mapSRTargetType.mapTargetType.find(strSkillTargetName) == CCfgSkillRule::ms_mapSRTargetType.mapTargetType.end())
	{
		ostringstream strm;
		strm<<"���ܹ���<"<<strName<<">����<"<<strSkillName<<">�����ڵ�Ŀ������<"<<strSkillTargetName<<"> ���ʵ��"<< endl;
		cout<<strm.str()<<endl;
		bSuccess = false;
	}
	else
	{
		eSRTargetType = CCfgSkillRule::ms_mapSRTargetType.mapTargetType[strSkillTargetName];
		uint32 uRealArgs = GetArgNum(eSRTargetType);
		if (uRealArgs != uTargetArgs)
		{
			ostringstream strm;
			strm<<"���ܹ���<"<<strName<<">����<"<<strSkillName<<">��Ŀ������<"<<strSkillTargetName<<">��������ʵ����Ҫ����<"
				<<uRealArgs<<"> ������д��<"<<uTargetArgs<<">��������"<<endl;
			cout<<strm.str()<<endl;
			bSuccess = false;
		}
	}
	
	if (bSuccess)
	{
		return new CCfgSkillRule(strName, strSkillName, eSRTargetType, strSkillArg);
	}
	return NULL;
}

bool CCfgSkillRuleCheck::CheckExist(string& strSkillRuleName)
{
	MapSkillRuleName::iterator iter = ms_mapSkillRuleName.find(strSkillRuleName);
	if (iter != ms_mapSkillRuleName.end())
	{
		return true;
	}
	return false;
}

CCfgSkillRule::CCfgSkillRule(string strName, string strSkillName, ESRTargetType eSRTargetType, string strSkillArg)
:m_strSkillName(strSkillName)
,m_eSRTargetType(eSRTargetType)
{
	if(strSkillName == "��ѡһ" || strSkillName == "��Χ�ص��ͷż���" || strSkillName == "����" 
		|| strSkillName == "���غ���" || strSkillName == "���������ͷ�" 
		||strSkillName == "����Ч����" || strSkillName == "���Ⱦ���" || strSkillName == "״̬�����ͷ�")
	{
		if(strSkillArg.empty())
		{
			ostringstream strm;
			strm<<"���ܹ���<"<<strName<<">����<"<<strSkillName<<">Ϊ��ѡһ���ܣ�ȱ�ټ��ܲ���!"<<endl;;
			cout<<strm.str()<<endl;
		}
		erase(strSkillArg, " ");
		erase(strSkillArg,"\"");
		replace(strSkillArg, "��", ",");
		replace(strSkillArg, "��", "(");
		replace(strSkillArg, "��", ")");
		erase(strSkillArg, "(");
		erase(strSkillArg, ")");
		int32 startPos = -1;
		string strTemp;
		while (true)
		{
			if(strSkillArg.empty())
				return;
			startPos = strSkillArg.find(',');
			if (-1 == startPos)
			{
				m_lstMultiArgs.push_back(strSkillArg);
				return;
			}
			strTemp = strSkillArg.substr(0,startPos);
			if(strTemp.empty())
			{
				if(startPos < int32(strSkillArg.length())-1)
				{
					ostringstream strm;
					strm<<"���ܹ���<"<<strName<<">����<"<<strSkillName<<">�ļ��ܲ�����ʽ���ԣ��������� ,, �����"<< endl;
					cout<<strm.str()<<endl;
				}
				else
					return;
			}
			m_lstMultiArgs.push_back(strTemp);
			strSkillArg = strSkillArg.substr(startPos+1,strSkillArg.length());
		}
	}
}

