#include "stdafx.h"
#include "CCfgMagicOp.h"
#include "CCfgMagicEffCheck.h"
#include "CCfgColChecker.inl"
#include "LoadSkillCommon.h"
#include "CSkillCfg.h"

CSkillCfg::MapObjFilterType CSkillCfg::ms_mapObjFilterType;

CCfgMagicOp::CCfgMagicOp(int32 nID)
:m_nID(nID)
{
	using namespace CfgChk;
	ReadItem(m_strMagicOpName,		szMagicEff_MagicOp);
	ReadItem(m_strMOPType,			szMagicEff_MOPType);
	m_bIsCondition = (m_strMOPType == "����") ? true : false;
	ReadItem(m_pMagicOp,			szMagicEff_MagicOp,			CCfgMagicEffCheck::ms_mapMagicOp);
	ReadMixedItem(m_MagicOpArg,		szMagicEff_MOPParam,		CANEMPTY);

	stringstream ExpStr;
	if (!m_pMagicOp)
	{
		string strMagicOp;
		ReadItem(strMagicOp, szMagicEff_MagicOp, CANEMPTY);
		GenExpInfo("�¼�ħ������δ����!", strMagicOp);
	}
	string strFilterPipe;
	ReadItem(strFilterPipe,			szMagicEff_FilterPipe,		CANEMPTY);
	trimend(strFilterPipe);
	if (!strFilterPipe.empty())
	{
		vector<string> vecFilter = CCfgMagicOp::Split(strFilterPipe, "|");
		vector<string>::iterator it_Filter = vecFilter.begin();
		for(; it_Filter != vecFilter.end(); ++it_Filter)
		{
			vector<string> vecFilterMultiObj = Split(*it_Filter, ",");
			vector<string>::iterator it_FilterMultiObj = vecFilterMultiObj.begin();
			MagicEffFilter* pMagicEffFilter = new MagicEffFilter;
			pMagicEffFilter->m_eObjectFilter = CSkillCfg::ms_mapObjFilterType[*it_FilterMultiObj];

			if (vecFilterMultiObj.size() == 2)
			{
				++it_FilterMultiObj;
				pMagicEffFilter->m_FilterPara = new CCfgCalc;
				pMagicEffFilter->m_FilterPara->InputString(*it_FilterMultiObj);
			}
			else
			{
				if (vecFilterMultiObj.size() != 1)
				{
					SafeDelete(pMagicEffFilter);
					ExpStr << m_strMagicOpName << endl;
					GenExpInfo("ħ��Ч����ɸѡ�ܵ�������������!", ExpStr.str());
				}
			}
			m_vecFilterPipe.push_back(pMagicEffFilter);
		}
	}
	else
	{
		ExpStr << m_strMagicOpName << endl;
		GenExpInfo("ħ��Ч����ɸѡ�ܵ�����Ϊ�� ", ExpStr.str());
	}
}

CCfgMagicOp::~CCfgMagicOp()
{
	SafeDelete(m_MagicOpArg);
}

vector<string> CCfgMagicOp::Split(const string& src, string delimit)  
{  
	vector<string> v;
	if( src.empty() || delimit.empty() )
	{
		v.push_back("");
		v.push_back("");
		return v;
	}

	int32 m,n;
	m=src.find_first_of("\"");  
	n=src.find_last_of("\"");  
	string str=src.substr(m>=0?m+1:0,n-1>0?n-1:src.size());  

	basic_string<TCHAR>::size_type delim_len = delimit.size();
	size_t index = std::string::npos, last_search_position = 0;  
	while((index=str.find(delimit, last_search_position)) != std::string::npos)  
	{  
		if(index == last_search_position)  
			v.push_back("");  
		else  
			v.push_back( str.substr(last_search_position, index - last_search_position) );  
		last_search_position = index + delim_len;  
	}  
	string last_one = str.substr(last_search_position);  
	v.push_back( last_one.empty()? "":last_one );
	return v;  
}

bool CCfgMagicOp::CheckMagicOpAvail()
{
	CfgChk::SetLineNo(m_nID);
	if(!CheckRadiusMaxNum2Object()) 
		return false;

	CfgChk::SetItemTitle(szMagicEff_FilterPipe);
	if(!CheckFilterObj2MOPType()) 
		return false;

	if (!m_pMagicOp)
	{
		CfgChk::GenExpInfo("���Ȳ�ȫδ��ӵ���ħ������!");
		return false;
	}

	vector<MagicEffFilter*>::iterator it = m_vecFilterPipe.begin();
	if(m_vecFilterPipe.size() == 1 && !m_pMagicOp->CheckObject2MOP((*it)->m_eObjectFilter)) 
		return false;

	CfgChk::SetItemTitle(szMagicEff_MOPParam);
	if(!m_pMagicOp->CheckArg2MOP(m_MagicOpArg))
		return false;

	return true;
}

bool CCfgMagicOp::CheckRadiusMaxNum2Object()
{
	//ɸѡ�뾶��Ŀ�����޼��
	VecMagicEffFilter::const_iterator it = m_vecFilterPipe.begin();
	for (; it != m_vecFilterPipe.end(); ++it)
	{
		switch((*it)->m_eObjectFilter)
		{
		case eOF_Self:
		case eOF_Target:
		case eOF_Position:
		case eOF_Pet:
		case eOF_NotCtrlSummon:
		case eOF_Master:
		case eOF_DeadBody:
		case eOF_ExceptSelf:
		case eOF_ExceptTarget:
		case eOF_IsVestInSelf:
		case eOF_ExpIsVestInSelf:
		case eOF_LatestTarget:
		case eOF_ExceptDeadBoby:
			if ((*it)->m_FilterPara)
			{
				CfgChk::SetItemTitle(szMagicEff_FilterPipe);
				stringstream str;
				str << "Υ��ɸѡ�ܵ���д����Ŀ��ɸѡ����Ϊ[" << (*it)->m_eObjectFilter << "]��ħ��������Ӧ���в���";
				CfgChk::GenExpInfo(str.str(), (*it)->m_FilterPara->GetTestString());
				return false;
			}
			break;
		case eOF_Teammates:
		case eOF_Raidmates:
		case eOF_Tongmates:
		case eOF_AroundFriend:
		case eOF_AroundEnemy:
		case eOF_Amount:
		case eOF_FilterRandom:
			if(!(*it)->m_FilterPara ||
				((*it)->m_FilterPara && (*it)->m_FilterPara->IsSingleNumber() && (*it)->m_FilterPara->GetIntValue() <= 0))
			{
				CfgChk::SetItemTitle(szMagicEff_FilterPipe);
				stringstream str;
				str << "Υ��ɸѡ�ܵ���д����Ŀ��ɸѡ����Ϊ[" << (*it)->m_eObjectFilter << "]��ħ�����������в��������Ҵ��ڵ���0";
				CfgChk::GenExpInfo(str.str(), (*it)->m_FilterPara->GetTestString());
				return false;
			}
			break;
		case eOF_InTriggerState:
		case eOF_InMagicState:
		case eOF_NotInMagicState:
		case eOF_OutSpecialState:
		case eOF_PosMagicTarget:
		case eOF_Class:
			if(!(*it)->m_FilterPara)
			{
				CfgChk::SetItemTitle(szMagicEff_FilterPipe);
				stringstream str;
				str << "Υ��ɸѡ�ܵ���д����Ŀ��ɸѡ����Ϊ[" << (*it)->m_eObjectFilter << "]��ħ�����������в���";
				CfgChk::GenExpInfo(str.str(), (*it)->m_FilterPara->GetTestString());
				return false;
			}
			break;
		case eOF_LeastHP:
		case eOF_FilterPlayer:
		case eOF_ExpOwner:
		case eOF_LockedEnemyTarget:
		case eOF_FilterNPC:
		case eOF_ShockWaveEff1:
		case eOF_ShockWaveEff2:
		case eOF_ShockWaveEff3:
		case eOF_ShockWaveEff4:
		case eOF_ShockWaveEff5:
		case eOF_FilterResult:
			break;
		default:
			if(!(*it)->m_FilterPara)
			{
				CfgChk::SetItemTitle(szMagicEff_FilterPipe);
				stringstream str;
				str << "Υ��ɸѡ�ܵ���д����[" << m_nID << "]�е�ɸѡ�ܵ�������";
				CfgChk::GenExpInfo(str.str());
				return false;
			}
			break;
		}
	}
	return true;
}

bool CCfgMagicOp::CheckFilterObj2MOPType()
{
	//��ʱ������Ŀ���ٴ�ɸѡ����ֻ��Ϊ�����Ŀ�� //Ŀ��Ϊ2009-04-27�ӵģ�����Ϊ��֮ǰ����ֻ��Ϊ�����ԭ������
	if(m_strMOPType == "��ʱ")
	{
		vector<MagicEffFilter*>::iterator it = m_vecFilterPipe.begin();
		if(m_vecFilterPipe.size() != 1 || ((*it)->m_eObjectFilter != eOF_Self && (*it)->m_eObjectFilter != eOF_Target))
		{
			CfgChk::GenExpInfo("Υ��ħ������Լ��������ʱ�����ﲻ��Ϊ�����Ŀ�������ֵ", (*it)->m_eObjectFilter);
			return false;
		}
	}
	return true;
}

EValueMagicOpType CCfgMagicOp::GetOpValueMagicOpType()
{
	if(m_pMagicOp->GetBaseType() == eMOPBT_MagicOP)
	{
		CMagicOp* pMagicOp = class_cast<CMagicOp*>(m_pMagicOp);
		if (pMagicOp->GetMagicOpType() == eMOT_Value)
		{
			CValueMagicOp* pValueMagicOp = class_cast<CValueMagicOp*>(pMagicOp);
			return pValueMagicOp->GetValueOpType();
		}
	}
	return eVMOT_End;
}