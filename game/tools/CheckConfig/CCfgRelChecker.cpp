#include "stdafx.h"
#include "CCfgRelChecker.h"
#include "CCfgMagicEffCheck.h"
#include "CCfgColChecker.inl"
#include "CCfgMagicOp.h"
#include "CCfgAllMagicCheck.h"
#include "CCfgAllStateCheck.h"
#include "CCfgSkillCheck.h"
#include "CCfgNormalAttackCheck.h"
#include "CCfgProcessCheck.h"

CCfgRelChecker::MapPingPongObjPersistent CCfgRelChecker::ms_mapPingPongObjPersistent;

bool CCfgRelChecker::ResetMap()
{
	ms_mapPingPongObjPersistent["��װ�����ħ��״̬"]	= eOPT_Inexistence;
	ms_mapPingPongObjPersistent["��װ�����������״̬"]	= eOPT_Inexistence;
	ms_mapPingPongObjPersistent["��װ�������̬"]		= eOPT_Inexistence;
	return true;
}

bool CCfgRelChecker::CheckRelationAvail()
{
	bool bRet = true;

	//ħ��Ч����ϵԼ�����Ƕ���������
	CfgChk::SetTabName("ħ��Ч��");
	for(CCfgMagicEffCheck::MapMagicEff::iterator itr =  CCfgMagicEffCheck::ms_mapMagicEff.begin(); 
		itr != CCfgMagicEffCheck::ms_mapMagicEff.end(); ++itr)
	{
		CfgChk::g_sMagicEffName = itr->second->GetMagicEffName();
		ResetMap();
		itr->second->CheckMagicOpRelationAvail(CCfgMagicOp::CheckPingPongObjPersistent);

		//��⺬���⳷��������Ч������Ϊ״̬�ɳ���Ч��
		itr->second->CheckMustCancelEffOfState();
	}

	//���ܱ��ϵԼ��
	CfgChk::SetTabName("����");
	for(CCfgSkillCheck::MapSkill::iterator itr = CCfgSkillCheck::ms_mapSkill.begin();
		itr != CCfgSkillCheck::ms_mapSkill.end(); ++itr)
	{
		if((*itr).second && (!(*itr).second->CheckRelationAvail())) bRet = false;
	}

	//�չ����ϵԼ��
	CfgChk::SetTabName("�չ�");
	for(CCfgNormalAttackCheck::MapNormalAttack::iterator itr = CCfgNormalAttackCheck::ms_mapNormalAttack.begin();
		itr != CCfgNormalAttackCheck::ms_mapNormalAttack.end(); ++itr)
	{
		if((*itr).second && (!(*itr).second->CheckRelationAvail())) bRet = false;
	}

	//���̱��ϵԼ��
	CfgChk::SetTabName("����");
	for(CCfgProcessCheck::MapProcess::iterator itr = CCfgProcessCheck::ms_mapProcess.begin();
		itr != CCfgProcessCheck::ms_mapProcess.end(); ++itr)
	{
		if((*itr).second && (!(*itr).second->CheckRelationAvail())) bRet = false;
	}

	//״̬���ϵԼ��
	CfgChk::SetTabName("״̬");
	for(CCfgAllStateCheck::MapState::iterator itr = CCfgAllStateCheck::ms_mapState.begin();
		itr != CCfgAllStateCheck::ms_mapState.end(); ++itr)
	{
		if(!(*itr).second->CheckRelationAvail()) bRet = false;
	}

	//��̬���ܱ��ϵԼ��
	CfgChk::SetTabName("��̬����");
	for(CCfgAureMagicCheck::MapAureMagic::iterator itr = CCfgAureMagicCheck::ms_mapAureMagic.begin();
		itr != CCfgAureMagicCheck::ms_mapAureMagic.end(); ++itr)
	{
		if(!(*itr).second->CheckRelationAvail()) bRet = false;
	}

	//λ��ħ�����ϵԼ��
	CfgChk::SetTabName("λ��ħ��");
	for(CCfgPositionMagicCheck::MapPositionMagic::iterator itr = CCfgPositionMagicCheck::ms_mapPositionMagic.begin();
		itr != CCfgPositionMagicCheck::ms_mapPositionMagic.end(); ++itr)
	{
		if(!(*itr).second->CheckRelationAvail()) bRet = false;
	}

	//λ��ħ�����ϵԼ��
	CfgChk::SetTabName("λ��ħ��");
	for (CCfgMoveMagicCheck::MapMoveMagic::iterator itr = CCfgMoveMagicCheck::ms_mapMoveMagic.begin();
		itr != CCfgMoveMagicCheck::ms_mapMoveMagic.end(); ++itr)
	{
		if (!(*itr).second->CheckRelationAvail()) bRet = false;
	}

	//ͼ��ħ�����ϵԼ��
	CfgChk::SetTabName("ͼ��ħ��");
	for(CCfgTotemMagicCheck::MapTotemMagic::iterator itr = CCfgTotemMagicCheck::ms_mapTotemMagic.begin();
		itr != CCfgTotemMagicCheck::ms_mapTotemMagic.end(); ++itr)
	{
		if(!(*itr).second->CheckRelationAvail()) bRet = false;
	}

	CfgChk::SetTabNull();
	return bRet;
}

bool CCfgSkillCheck::CheckRelationAvail()
{
	if(GetMagicEff())
	{
		CfgChk::g_sSrcTabLineName = GetSkillName();
		CfgChk::g_sMagicEffName = GetMagicEff()->GetMagicEffName();
		return GetMagicEff()->CheckMagicOpRelationAvail(CCfgMagicOp::CheckForbidCancellableValueChangedMop);
	}
	return true;
}

bool CCfgNormalAttackCheck::CheckRelationAvail()
{
	if(GetMagicEff())
	{
		CfgChk::g_sSrcTabLineName = GetName();
		CfgChk::g_sMagicEffName = GetMagicEff()->GetMagicEffName();
		return GetMagicEff()->CheckMagicOpRelationAvail(CCfgMagicOp::CheckForbidCancellableValueChangedMop);
	}
	return true;
}

bool CCfgProcessCheck::CheckRelationAvail()
{
	if(GetSelfCancelEff())
	{
		CfgChk::g_sMagicEffName = GetSelfCancelEff()->GetMagicEffName();
		if (!GetSelfCancelEff()->CheckMagicOpRelationAvail(CCfgMagicOp::CheckNoSingleFilterSelf2Eff)) return false;
		if (!GetSelfCancelEff()->CheckMagicOpRelationAvail(CCfgMagicOp::CheckCancelableMopSetupAureMagicNotSave)) return false;
		if (!GetSelfCancelEff()->CheckMagicOpRelationAvail(CCfgMagicOp::CheckCancelableMopSetupStateNotSave)) return false;
	}
	if (GetObjCancelEff())
	{
		CfgChk::g_sMagicEffName = GetObjCancelEff()->GetMagicEffName();
		if (!GetObjCancelEff()->CheckMagicOpRelationAvail(CCfgMagicOp::CheckNoSingleFilterObj2Eff))	return false;
		if (!GetObjCancelEff()->CheckMagicOpRelationAvail(CCfgMagicOp::CheckCancelableMopSetupAureMagicNotSave)) return false;
		if (!GetObjCancelEff()->CheckMagicOpRelationAvail(CCfgMagicOp::CheckCancelableMopSetupStateNotSave)) return false;
	}

	return true;
}

bool CCfgAllStateCheck::CheckRelationAvail()
{
	if (GetType() == eBST_State)
	{
		if((GetCascadeType() == "����" || GetCascadeType() == "����") && GetCascadeMax() != "1")
		{
			if(GetCancelEff() && !GetCancelEff()->CheckMagicOpRelationAvail(CCfgMagicOp::CheckFilterObj2CentralMS)) return false;
			if(GetDotEff() && !GetDotEff()->CheckMagicOpRelationAvail(CCfgMagicOp::CheckFilterObj2CentralMS)) return false;
			if(GetFinalEff() && !GetFinalEff()->CheckMagicOpRelationAvail(CCfgMagicOp::CheckFilterObj2CentralMS)) return false;
		}
		if(GetCancelEff())
		{
			if(!GetCancelEff()->CheckMagicOpRelationAvail(CCfgMagicOp::CheckNoSingleFilterObj2Eff)) return false;
			if(!GetCancelEff()->CheckMagicOpRelationAvail(CCfgMagicOp::CheckFilterObj2CalcNeedTargetMOPInMS)) return false;
			CfgChk::g_sMagicEffName = GetCancelEff()->GetMagicEffName();
			if(GetPersistent())
			{
				CfgChk::g_sSrcTabLineName = GetName();
				if(!GetCancelEff()->CheckMagicOpRelationAvail(CCfgMagicOp::CheckNonPersistentObj)) return false;
			}
			if (!GetCancelEff()->CheckMagicOpRelationAvail(CCfgMagicOp::CheckCancelableMopSetupAureMagicNotSave)) return false;
		}
		if(GetDotEff())
		{
			CfgChk::g_sMagicEffName = GetDotEff()->GetMagicEffName();
			if(!GetDotEff()->CheckMagicOpRelationAvail(CCfgMagicOp::CheckFilterObj2CalcNeedTargetMOPInMS)) return false;
		}
		if(GetFinalEff())
		{
			CfgChk::g_sMagicEffName = GetFinalEff()->GetMagicEffName();
			if(!GetFinalEff()->CheckMagicOpRelationAvail(CCfgMagicOp::CheckFilterObj2CalcNeedTargetMOPInMS)) return false;
		}
		return true;
	}
	else if (GetType() == eBST_Trigger || GetType() == eBST_Damage)
	{
		if(GetCancelEff())
		{
			if(!GetCancelEff()->CheckMagicOpRelationAvail(CCfgMagicOp::CheckCascade2CancelableMagicEff)) return false;
			if(GetNeedSaveToDB() && !GetCancelEff()->CheckMagicOpRelationAvail(CCfgMagicOp::CheckMOPType2CancelableMagicEff)) return false;

			if(!GetCancelEff()->CheckMagicOpRelationAvail(CCfgMagicOp::CheckNoSingleFilterObj2Eff)) return false;
			if(!GetCancelEff()->CheckMagicOpRelationAvail(CCfgMagicOp::CheckCancellableMopArgIsNotConst)) return false;

			CfgChk::g_sMagicEffName = GetCancelEff()->GetMagicEffName();
			if(GetPersistent())
			{
				CfgChk::g_sSrcTabLineName = GetName();
				GetCancelEff()->CheckMagicOpRelationAvail(CCfgMagicOp::CheckNonPersistentObj);
			}
			if (!GetCancelEff()->CheckMagicOpRelationAvail(CCfgMagicOp::CheckCancelableMopSetupAureMagicNotSave)) return false;
		}
		return true;
	}
	else
	{
		return true;
	}
}

bool CCfgAureMagicCheck::CheckRelationAvail()
{
	if(GetMagicEff())
	{
		CfgChk::g_sMagicEffName = GetMagicEff()->GetMagicEffName();
		if(GetPersistent())
		{
			CfgChk::g_sSrcTabLineName = GetName();
			GetMagicEff()->CheckMagicOpRelationAvail(CCfgMagicOp::CheckNonPersistentObj);
		}
		if(!GetMagicEff()->CheckMagicOpRelationAvail(CCfgMagicOp::CheckNoSingleFilterObj2Eff)) return false;
		if(!GetMagicEff()->CheckMagicOpRelationAvail(CCfgMagicOp::CheckCancelableMopSetupStateNotSave)) return false;
	}
	return true;
}

bool CCfgPositionMagicCheck::CheckRelationAvail()
{
	if(GetTouchMagicEff())
	{
		CfgChk::g_sMagicEffName = GetTouchMagicEff()->GetMagicEffName();
		if(!GetTouchMagicEff()->CheckMagicOpRelationAvail(CCfgMagicOp::CheckNoSingleFilterObj2Eff)) return false;
		if(!GetTouchMagicEff()->CheckMagicOpRelationAvail(CCfgMagicOp::CheckCancelableMopSetupStateNotSave)) return false;
		if(!GetTouchMagicEff()->CheckMagicOpRelationAvail(CCfgMagicOp::CheckPosMagicSetupStateType)) return false;
		if (IsMutex())
		{
			if(!GetTouchMagicEff()->CheckMagicOpRelationAvail(CCfgMagicOp::CheckMutexPosMagicSetupStateType)) return false;
		}
	}

	return true;
}

bool CCfgMoveMagicCheck::CheckRelationAvail()
{
	if(GetCancelMagicEff())
	{
		CfgChk::g_sMagicEffName = GetCancelMagicEff()->GetMagicEffName();
		if (!GetCancelMagicEff()->CheckMagicOpRelationAvail(CCfgMagicOp::CheckCancelableMopSetupAureMagicNotSave)) return false;
		if (!GetCancelMagicEff()->CheckMagicOpRelationAvail(CCfgMagicOp::CheckCancelableMopSetupStateNotSave)) return false;
	}

	return true;
}

bool CCfgTotemMagicCheck::CheckRelationAvail()
{
	if(GetTouchMagicEff())
	{
		if(!GetTouchMagicEff()->CheckMagicOpRelationAvail(CCfgMagicOp::CheckNoSingleFilterObj2Eff)) return false;
	}

	return true;
}

bool CCfgMagicEffCheck::CheckMagicOpRelationAvail(MOP_MATCH_CHECKER pFunChecker)
{
	VecChild::iterator itrChild = m_vecChild.begin();
	for (; itrChild != m_vecChild.end(); ++ itrChild)
	{
		if(!(*itrChild)->CheckMagicOpRelationAvail(pFunChecker))
			return false;
	}
	return true;
}

bool CCfgMagicEffCheck::CheckMustCancelEffOfState()
{
	CBaseMagicOp* pBaseMagicOp = NULL;
	VecChild::iterator itrChild = m_vecChild.begin();
	for (; itrChild != m_vecChild.end(); ++ itrChild)
	{
		CBaseMagicOp* pBaseMagicOp = (*itrChild)->GetBaseMagicOp();
		if (pBaseMagicOp->CheckIsCancelEffOfState())
		{
			if (!CCfgAllStateCheck::IsCancelEffOfState(this))
			{
				stringstream str;
				str << "\n���ñ�[ħ��Ч��]��" << ((*itrChild)->GetID() + 1) << "��Υ��Լ����ħ��Ч��[" << CfgChk::g_sMagicEffName << 
					"]��ʹ�����ƽ�ֹ�չ������ܲ��������Ա�����ħ��״̬�Ŀɳ���Ч��\n";
				CfgChk::GenExpInfo(str.str());
				return false;
			}
		}
	}
	return true;
}

bool CCfgMagicOp::CheckMagicOpRelationAvail(MOP_MATCH_CHECKER pFunChecker)
{
	if(!pFunChecker(this))
		return false;
	return true;
}

bool CCfgMagicOp::CheckFilterObj2CentralMS(CCfgMagicOp* pMagicOp)
{
	vector<MagicEffFilter*>::iterator it = pMagicOp->m_vecFilterPipe.begin();
	if((*it)->m_eObjectFilter == eOF_Self)
	{
		stringstream str;
		str << "���ñ�[ħ��Ч��]��" << (pMagicOp->m_nID + 1) << "��Υ��ħ��״̬��ϵ����Լ������������Ϊ���е�ħ��״̬������ħ������ɸѡĿ��Ϊ�������[" << (*it)->m_eObjectFilter << "]��ħ��Ч��\n";
		CfgChk::GenExpInfo(str.str());
		return false;
	}

	if(pMagicOp->m_bIsCondition)
	{
		stringstream str;
		str << "���ñ�[ħ��Ч��]��" << (pMagicOp->m_nID + 1) << "��Υ��ħ��״̬��ϵ����Լ������������Ϊ���е�ħ��״̬������ħ������\n";
		CfgChk::GenExpInfo(str.str());
		return false;
	}

	if(pMagicOp->m_strMOPType == "��ʱ")
	{
		stringstream str;
		str << "���ñ�[ħ��Ч��]��" << (pMagicOp->m_nID + 1) << "��Υ��ħ��״̬��ϵ����Լ������������Ϊ���е�ħ��״̬��������ʱħ������\n";
		CfgChk::GenExpInfo(str.str());
		return false;
	}

	return true;
}

bool CCfgMagicOp::CheckCascade2CancelableMagicEff(CCfgMagicOp* pMagicOp)
{
	if(pMagicOp->m_strMagicOpName == "��װħ��״̬")
	{
		CCfgAllStateCheck::MapState::iterator itr = CCfgAllStateCheck::ms_mapState.find(pMagicOp->m_MagicOpArg->GetString());
		if ((*itr).second->GetCascadeType() != "Ψһ")
		{
			stringstream str;
			str << "���ñ�[ħ��Ч��]��" << (pMagicOp->m_nID + 1) << "��Υ��ħ��״̬��ϵ����Լ�����ɳ���Ч���ﰲװ��ħ��״̬�ĵ������ͱ���ΪΨһ\n";
			CfgChk::GenExpInfo(str.str());
			return false;
		}
	}
	return true;
}

bool CCfgMagicOp::CheckMOPType2CancelableMagicEff(CCfgMagicOp* pMagicOp)
{
	if(pMagicOp->m_pMagicOp && pMagicOp->m_pMagicOp->GetBaseType() == eMOPBT_MagicOP && 
		class_cast<CMagicOp*>(pMagicOp->m_pMagicOp)->GetMagicOpType() != eMOT_Function
		&& pMagicOp->m_strMOPType != "��ʱ")
	{
		stringstream str;
		str << "���ñ�[ħ��Ч��]��" << (pMagicOp->m_nID + 1) << "��Υ��ħ��״̬��ϵ����Լ������Ҫ�洢�����ݿ�Ĵ��������˺����״̬�Ŀɳ���Ч����ķ���������ʱħ����������Ϊ������\n";
		CfgChk::GenExpInfo(str.str());
		return false;
	}
	return true;
}


bool CCfgMagicOp::CheckFilterObj2CancelableMagicEff(CCfgMagicOp* pMagicOp)
{
	//vector<MagicEffFilter*>::iterator it = pMagicOp->m_vecFilterPipe.begin();
	//if ((*it)->m_eObjectFilter == eOF_Self)
	//{
	//	stringstream str;
	//	str << "���ñ�[ħ��Ч��]��" << (pMagicOp->m_nID + 1) << "��Υ��״̬��ϵ����Լ����״̬������ħ������ɸѡĿ��Ϊ�������[" << (*it)->m_eObjectFilter << "]��ħ��Ч��\n";
	//	CfgChk::GenExpInfo(str.str());
	//	return false;
	//}
	return true;
}

bool CCfgMagicOp::CheckNoSingleFilterSelf2Eff(CCfgMagicOp* pCfgMagicOp)
{
	vector<MagicEffFilter*>::iterator it = pCfgMagicOp->m_vecFilterPipe.begin();
	CBaseMagicOp* pMagicOp = pCfgMagicOp->m_pMagicOp;
	if(pMagicOp && pMagicOp->GetBaseType() == eMOPBT_MagicOP && 
		/*class_cast<CMagicOp*>(pMagicOp)->GetMagicOpType() == eMOT_Value && */
		class_cast<CMagicOp*>(pMagicOp)->Cancelable() &&
		(*it)->m_eObjectFilter != eOF_Self)
	{
		stringstream str;
		str << "���ñ�[ħ��Ч��]��" << (pCfgMagicOp->m_nID + 1) << "��Υ��[" << CfgChk::g_sTabName << "]��ϵ����Լ�����ɳ���Ч���ﲻ�����ٴ�ɸѡĿ��Ϊ[" << (*it)->m_eObjectFilter << "]�ķ���ʱ�ɳ���ħ������\n";
		CfgChk::GenExpInfo(str.str());
		return false;
	}

	return true;
}

//��ʱ������
bool CCfgMagicOp::CheckNoSingleFilterObj2Eff(CCfgMagicOp* pCfgMagicOp)
{
	vector<MagicEffFilter*>::iterator it = pCfgMagicOp->m_vecFilterPipe.begin();
	CBaseMagicOp* pMagicOp = pCfgMagicOp->m_pMagicOp;
	if(pMagicOp && pMagicOp->GetBaseType() == eMOPBT_MagicOP && 
		/*class_cast<CMagicOp*>(pMagicOp)->GetMagicOpType() == eMOT_Value && */
		class_cast<CMagicOp*>(pMagicOp)->Cancelable() &&
		pCfgMagicOp->m_strMOPType != "��ʱ")
	{
		if ((*it)->m_eObjectFilter != eOF_Target)
		{
			stringstream str;
			str << "���ñ�[ħ��Ч��]��" << (pCfgMagicOp->m_nID + 1) << "��Υ��[" << CfgChk::g_sTabName << "]��ϵ����Լ�����ɳ���Ч���ﲻ�����ٴ�ɸѡĿ��Ϊ[" << (*it)->m_eObjectFilter << "]�ķ���ʱ�ɳ���ħ������\n";
			CfgChk::GenExpInfo(str.str());
			return false;
		}
		if (pCfgMagicOp->m_vecFilterPipe.size() > 1)
		{
			stringstream str;
			str << "���ñ�[ħ��Ч��]��" << (pCfgMagicOp->m_nID + 1) << "��Υ��[" << CfgChk::g_sTabName << "]��ϵ����Լ�����ɳ���Ч���ﲻ����ɸѡ�ܵ�'|'�ķ���ʱ�ɳ���ħ������\n";
			CfgChk::GenExpInfo(str.str());
			return false;
		}
	}

	return true;
}

bool CCfgMagicOp::CheckNoSingleAndSelfFilterObj2Eff(CCfgMagicOp* pMagicOp)
{
	//vector<MagicEffFilter*>::iterator it = pMagicOp->m_vecFilterPipe.begin();
	//CBaseMagicOp* pMagicOp = pMagicOp->m_pMagicOp;
	//if(pMagicOp && pMagicOp->GetBaseType() == eMOPBT_MagicOP && 
	//	/*class_cast<CMagicOp*>(pMagicOp)->GetMagicOpType() == eMOT_Value && */
	//	class_cast<CMagicOp*>(pMagicOp)->Cancelable() &&
	//	(*it)->m_eObjectFilter != eOF_Target && 
	//	(*it)->m_eObjectFilter != eOF_Self && 
	//	pMagicOp->m_strMOPType != "��ʱ")
	//{
	//	stringstream str;
	//	str << "���ñ�[ħ��Ч��]��" << (pMagicOp->m_nID + 1) << "��Υ��[" << CfgChk::g_sTabName << "]��ϵ����Լ�����ɳ���Ч���ﲻ�����ٴ�ɸѡĿ��Ϊ[" << (*it)->m_eObjectFilter << "]�ķ���ʱ�ɳ���ħ������\n";
	//	CfgChk::GenExpInfo(str.str());
	//	return false;
	//}

	return true;
}

bool CCfgMagicOp::CheckFilterObj2CalcNeedTargetMOPInMS(CCfgMagicOp* pCfgMagicOp)
{
	vector<MagicEffFilter*>::iterator it = pCfgMagicOp->m_vecFilterPipe.begin();
	for(;it != pCfgMagicOp->m_vecFilterPipe.end(); ++it)
	{
		if ((*it)->m_eObjectFilter != eOF_Target)
		{
			CBaseMagicOp* pMagicOp = pCfgMagicOp->m_pMagicOp;
			if(pMagicOp->GetBaseType() == eMOPBT_MagicOP
				&& (class_cast<CMagicOp*>(pMagicOp))->GetMagicOpType() == eMOT_Value
				&& (class_cast<CValueMagicOp*>(pMagicOp))->CalcNeedTarget(pCfgMagicOp->m_MagicOpArg))
			{
				stringstream str;
				str << "���ñ�[ħ��Ч��]��" << (pCfgMagicOp->m_nID + 1)
					<< "��[" << CfgChk::g_sMagicEffName << "]Υ��ħ��״̬��ϵ����Լ����ħ��״̬��Ч��������ħ������ɸѡĿ��Ϊ["
					<< (*it)->m_eObjectFilter << "]�ļ��㲿����ҪĿ���ħ������["
					<< pCfgMagicOp->m_strMagicOpName << "]����ҪĿ���ħ����������["
					<< pCfgMagicOp->m_MagicOpArg->GetTestString() << "]\n";
				CfgChk::GenExpInfo(str.str());
				return false;
			}
		}
	}
	return true;
}

bool CCfgMagicOp::CheckNoCond(CCfgMagicOp* pMagicOp)
{
	//if(pMagicOp->m_pMagicOp->GetBaseType() == eMOPBT_MagicCondition)
	//{
	//	stringstream str;
	//	str << "���ñ�[ħ��Ч��]��" << (pMagicOp->m_nID + 1)
	//		<< "��Υ��ħ��״̬��ϵ����Լ����ħ��״̬��Ч��������ħ������["
	//		<< pMagicOp->m_strMagicOpName << "]\n";
	//		CfgChk::GenExpInfo(str.str());
	//		return false;
	//}
	return true;
}

bool CCfgMagicOp::CheckHasNoCond(CCfgMagicOp* pMagicOp)
{
	//return pMagicOp->m_pMagicOp->GetBaseType() != eMOPBT_MagicCondition;
	return true;
}

bool CCfgMagicOp::CheckHasNoValueChangedOp(CCfgMagicOp* pMagicOp)
{
	//if(pMagicOp->m_pMagicOp->GetBaseType() == eMOPBT_MagicOP)
	//{
	//	CMagicOp* pMagicOp = static_cast<CMagicOp*>(pMagicOp->m_pMagicOp);
	//	if(pMagicOp->GetMagicOpType() == eMOT_Value)
	//	{
	//		return false;
	//	}
	//}
	return true;
}

bool CCfgMagicOp::CheckCancellableMopArgIsNotConst(CCfgMagicOp* pCfgMagicOp)
{
	switch(pCfgMagicOp->m_pMagicOp->GetBaseType())
	{
	case eMOPBT_MagicOP:
		{
			CMagicOp* pMagicOp = static_cast<CMagicOp*>(pCfgMagicOp->m_pMagicOp);
			if(pMagicOp->Cancelable() && !pCfgMagicOp->m_MagicOpArg->IsConstValue())
			{
				stringstream str;
				str << "���ñ�[" << CfgChk::g_sTabName << "]�Ŀɳ���Ч����[" << pCfgMagicOp->m_nID << "]�еĿɳ�������[" << pCfgMagicOp->m_strMagicOpName << 
					"ħ����������[" << pCfgMagicOp->m_MagicOpArg->GetTestString() << "]����Ϊ�ɱ�ֵ\n";
				CfgChk::GenExpInfo(str.str());
				return false;
			}
		}
		break;
	default:
		break;
	}
	return true;
}

bool CCfgMagicOp::CheckCanExecuteWithTargetIsDead(CCfgMagicOp* pMagicOp)
{
	//���ڽ�ɫ����ʱ��״̬��ִ��Ч����������ƿ���ȥ�ˣ���ҪС���������ܰ�װĿ������ʱִ����Ч��ħ������������ı����ֵ
	//switch(pMagicOp->m_pMagicOp->GetBaseType())
	//{
	//case eMOPBT_MagicOP:
	//	if(!pMagicOp->m_pMagicOp->CanExecuteWithTargetIsDead())
	//	{
	//		stringstream str;
	//		str << "ʱ�䲻�����޵�����״̬[" << CfgChk::g_sTabName << "]��Ч����[" << pMagicOp->m_nID << "]�еĲ���[" << pMagicOp->m_strMagicOpName << 
	//			"]����ΪĿ������ʱ����ִ�е�ħ������\n";
	//		CfgChk::GenExpInfo(str.str());
	//		return false;
	//	}
	//case eMOPBT_MagicCondition:
	//	return true;
	//}
	return true;
}

bool CCfgMagicOp::CheckTempMopForState(CCfgMagicOp* pMagicOp)
{
	//if(pMagicOp->m_strMOPType == "��ʱ")
	//{
	//	stringstream str;
	//	str << "״̬[" << CfgChk::g_sTabName << "]��Ч����������Ч���⣩��[" << pMagicOp->m_uId << "]�в�������ʱ����[" << pMagicOp->m_strMagicOpName <<"]\n";
	//	CfgChk::GenExpInfo(str.str());
	//	return false;
	//}
	return false;
}

bool CCfgMagicEffCheck::CheckNpcSkillFilterObj2MagicEff(const string sRuleName, string sSkillName, ESRTargetType eSRTargetType)
{
	VecChild::iterator itrChild = m_vecChild.begin();
	for (; itrChild != m_vecChild.end(); ++ itrChild)
	{
		if(!(*itrChild)->CheckNpcSkillFilterObj2MagicEff(sRuleName, sSkillName, eSRTargetType))
			return false;
	}
	return true;
}

bool CCfgMagicOp::CheckNpcSkillFilterObj2MagicEff(const string sRuleName, string sSkillName, ESRTargetType eSRTargetType)
{
	bool bRet = true;
	vector<MagicEffFilter*>::iterator it = m_vecFilterPipe.begin();
	switch(eSRTargetType)
	{
	case eSRTT_P_EnemyPostion:
	case eSRTT_P_SelfPosition:
	case eSRTT_P_RandomEnemyPostion:
	case eSRTT_P_SelfDirection:
	case eSRTT_P_SelfReverseDirection:
	case eSRTT_P_RandomFriendPos:
	case eSRTT_P_MaxHpEnemyPos:
	case eSRTT_P_MinHpEnemyPos:
	case eSRTT_P_MaxMpEnemyPos:
	case eSRTT_P_MinMpEnemyPos:
	case eSRTT_P_RandomDirection:
	case eSRTT_P_NearestEnemyPos:
	case eSRTT_P_FarthestEnemyPos:
	case eSRTT_P_NearestFriendPos:
	case eSRTT_P_FarthestFriendPos:
	case eSRTT_P_RandomUnEnmityPos:
	case eSRTT_P_RandomUnServantPos:
	case eSRTT_P_RamdomUnEnmityUnServantPos:
	case eSRTT_P_MaxHpFriendPos:
	case eSRTT_P_MinHpFriendPos:
	case eSRTT_P_MaxMpFriendPos:
	case eSRTT_P_MinMpFriendPos:
	case eSRTT_P_AroundPos:
	case eSRTT_P_RandomPos:
		{
			// ֮����"����"Ҳ�������ڣ�����Ϊ�ص�����ħ���������ж�
			if(!((*it)->m_eObjectFilter == eOF_Position || (*it)->m_eObjectFilter == eOF_Self || (*it)->m_eObjectFilter == eOF_FilterResult))
			{
				cout<<endl;
				cout<<"Npc���ܹ�����й���"<<sRuleName<<"���ͷŵļ��ܡ�"<<sSkillName<<"��ִ�е�ħ��������"
					<<m_strMagicOpName<<"�������Ŀ��ģ����Ǽ��ܹ����������ǵص㣡��߻�������"<<endl;
				return false;
			}
		}
		break;
	default:
		{
			if((*it)->m_eObjectFilter == eOF_Position)
			{
				cout<<endl;
				cout<<"Npc���ܹ�����й���"<<sRuleName<<"���ͷŵļ��ܡ�"<<sSkillName<<"��ִ�е�ħ��������"
					<<m_strMagicOpName<<"������Եص�ģ����Ǽ��ܹ�����������Ŀ�꣡��߻�������"<<endl;
				return false;	
			}
		}
		break;
	}

	return bRet;
}

bool CCfgMagicOp::CheckForbidCancellableValueChangedMop(CCfgMagicOp* pCfgMagicOp)
{
	CBaseMagicOp* pBaseMagicOp = pCfgMagicOp->m_pMagicOp;
	if(pBaseMagicOp->GetBaseType() == eMOPBT_MagicOP && pCfgMagicOp->m_strMOPType != "��ʱ")
	{
		CMagicOp* pMagicOp = class_cast<CMagicOp*>(pBaseMagicOp);
		if(pMagicOp->GetMagicOpType() == eMOT_Value && pMagicOp->Cancelable())
		{
			stringstream str;
			str << "���ñ�[ħ��Ч��]��" << (pCfgMagicOp->m_nID + 1) << "��Υ����ֵ����ϵԼ����" <<
				CfgChk::g_sTabName << "[" << CfgChk::g_sSrcTabLineName << "]��ֱ��ħ��Ч��[" << CfgChk::g_sMagicEffName << 
				"]�����иı�ֵ�ķ���ʱ�ɳ�������[" << pCfgMagicOp->m_strMagicOpName << "]\n";
			CfgChk::GenExpInfo(str.str());
			return false;
		}
	}
	return true;
}

bool CCfgMagicOp::CheckNonPersistentObj(CCfgMagicOp* pCfgMagicOp)
{
	CBaseMagicOp* pBaseMagicOp = pCfgMagicOp->m_pMagicOp;
	if(pBaseMagicOp->GetBaseType() == eMOPBT_MagicOP && pCfgMagicOp->m_strMOPType != "��ʱ")
	{
		CMagicOp* pMagicOp = class_cast<CMagicOp*>(pBaseMagicOp);
		if(pMagicOp->GetMagicOpType() == eMOT_Function && pMagicOp->Cancelable())
		{
			CFunctionCancelableMagicOp* pFunMop = class_cast<CFunctionCancelableMagicOp*>(pMagicOp);
			if(pFunMop->IsSetupNestedObj())
			{
				string sMagicOpName = pCfgMagicOp->m_strMagicOpName;
				if(sMagicOpName == "��װ��̬")
				{	
					string strAureName = pCfgMagicOp->m_MagicOpArg->GetTestString();
					CCfgAureMagicCheck::MapAureMagic::iterator itr = CCfgAureMagicCheck::ms_mapAureMagic.find(strAureName);
					if(!(*itr).second->GetPersistent())
					{
						stringstream str;
						str << "���ñ�[ħ��Ч��]��" << (pCfgMagicOp->m_nID + 1) << "��Υ�����ö����ϵԼ�������õ�" <<
							CfgChk::g_sTabName << "[" << CfgChk::g_sSrcTabLineName << "]��ֱ��ħ��Ч��[" << CfgChk::g_sMagicEffName << 
							"]���ܰ�װ�����õ���̬ħ�����⻷��[" <<  pCfgMagicOp->m_MagicOpArg->GetTestString() << "]\n";
						CfgChk::GenExpInfo(str.str());
						return false;						
					}
				}
				else/* if(sMagicOpName == "���ΰ�װħ��״̬")*/
				{
					uint32 l = 1;
					string sStateName;
					if(pCfgMagicOp->m_MagicOpArg->GetStrType() == eST_MutliString)
					{
						l = pCfgMagicOp->m_MagicOpArg->GetStringCount();
					}
					for(uint32 i = 0; i < l; ++i)
					{
						if(i == 0 && l == 1)
						{
							sStateName = pCfgMagicOp->m_MagicOpArg->GetString();
						}
						else
						{
							sStateName = pCfgMagicOp->m_MagicOpArg->GetString(i);
						}
						if(atoi(sStateName.c_str()) > 0)
							continue;
						CCfgAllStateCheck::MapState::iterator itr = CCfgAllStateCheck::ms_mapState.find(sStateName);
						if(!(*itr).second->GetPersistent())
						{
							stringstream str;
							str << "���ñ�[ħ��Ч��]��" << (pCfgMagicOp->m_nID + 1) << "��Υ�����ö����ϵԼ�������õ�" <<
								CfgChk::g_sTabName << "[" << CfgChk::g_sSrcTabLineName << "]��ֱ��ħ��Ч��[" << CfgChk::g_sMagicEffName << 
								"]���ܰ�װ������״̬[" <<  sStateName << "]\n";
							CfgChk::GenExpInfo(str.str());
							return false;						
						}
					}
					
				}
			}
		}
	}
	return true;
}

bool CCfgMagicOp::CheckPingPongObjPersistent(CCfgMagicOp* pCfgMagicOp)
{
	CBaseMagicOp* pBaseMagicOp = pCfgMagicOp->m_pMagicOp;
	if(pBaseMagicOp->GetBaseType() == eMOPBT_MagicOP && pCfgMagicOp->m_strMOPType != "��ʱ")
	{
		CMagicOp* pMagicOp = class_cast<CMagicOp*>(pBaseMagicOp);
		if(pMagicOp->GetMagicOpType() == eMOT_Function)
		{
			CCfgRelChecker::MapPingPongObjPersistent::iterator itr = CCfgRelChecker::ms_mapPingPongObjPersistent.find(pCfgMagicOp->m_strMagicOpName);
			if(itr != CCfgRelChecker::ms_mapPingPongObjPersistent.end())
			{
				bool bPersistent;
				if(pCfgMagicOp->m_strMagicOpName == "��װ�������̬")
				{
					string strAureName = pCfgMagicOp->m_MagicOpArg->GetString();
					CCfgAureMagicCheck::MapAureMagic::iterator itr = CCfgAureMagicCheck::ms_mapAureMagic.find(strAureName);
					bPersistent = (*itr).second->GetPersistent();
				}
				else
				{
					string strStateName = pCfgMagicOp->m_MagicOpArg->GetString();
					CCfgAllStateCheck::MapState::iterator itr = CCfgAllStateCheck::ms_mapState.find(strStateName);
					bPersistent = (*itr).second->GetPersistent();
				}
				switch(itr->second)
				{
				case CCfgRelChecker::eOPT_Inexistence:
					itr->second = bPersistent ? CCfgRelChecker::eOPT_Persistent : CCfgRelChecker::eOPT_Inpersistent;
					break;
				case CCfgRelChecker::eOPT_Persistent:
					if(!bPersistent)
					{
						stringstream str;
						str << "���ñ�[ħ��Ч��]��" << (pCfgMagicOp->m_nID + 1) << "��Υ����װ����������һ����Լ����ħ��Ч��[" << 
							CfgChk::g_sMagicEffName << "]���������ϵ�ħ������[" <<  pCfgMagicOp->m_strMagicOpName << "]�Ұ�װ�Ķ��������Բ�һ��\n";
						CfgChk::GenExpInfo(str.str());
						return false;						
					}
					break;
				case CCfgRelChecker::eOPT_Inpersistent:
					if(bPersistent)
					{
						stringstream str;
						str << "���ñ�[ħ��Ч��]��" << (pCfgMagicOp->m_nID + 1) << "��Υ����װ����������һ����Լ����ħ��Ч��[" << 
							CfgChk::g_sMagicEffName << "]���������ϵ�ħ������[" <<  pCfgMagicOp->m_strMagicOpName << "]�Ұ�װ�Ķ��������Բ�һ��\n";
						CfgChk::GenExpInfo(str.str());
						return false;						
					}
					break;
				default:
					{
						stringstream str;
						str << "���ñ�[ħ��Ч��]��" << (pCfgMagicOp->m_nID + 1) << "��ħ��Ч��[" << CfgChk::g_sMagicEffName << 
							"]Υ����װ����������һ����Լ����ms_mapPingPongObjPersistent��secondȡ��EObjPersistentType�����ֵ[" << 
							itr->second << "]\n";
						CfgChk::GenExpInfo(str.str());
						return false;						
					}
				}
			}
		}
	}
	return true;
}

bool CCfgMagicOp::CheckCancelableMopSetupAureMagicNotSave(CCfgMagicOp* pMagicOp)
{
	if(pMagicOp->m_strMagicOpName == "��װ��̬" || pMagicOp->m_strMagicOpName == "��װ�������̬")
	{
		string strAureName = pMagicOp->m_MagicOpArg->GetTestString();
		CCfgAureMagicCheck::MapAureMagic::iterator itr = CCfgAureMagicCheck::ms_mapAureMagic.find(strAureName);
		if((*itr).second->GetNeedSaveToDB())
		{
			stringstream str;
			str << "\n���ñ�[ħ��Ч��]��" << (pMagicOp->m_nID + 1) << "��Υ��Լ�����ɳ���Ч��[" << CfgChk::g_sMagicEffName << 
				"]��װ����̬(��⻷)[" <<  pMagicOp->m_MagicOpArg->GetTestString() << "]����������ǿ�Ʋ����\n";
			CfgChk::GenExpInfo(str.str());
			return false;						
		}
	}
	return true;
}

bool CCfgMagicOp::CheckCancelableMopSetupStateNotSave(CCfgMagicOp* pMagicOp)
{
	if( pMagicOp->m_strMagicOpName == "��װħ��״̬" || pMagicOp->m_strMagicOpName == "������װħ��״̬" ||
		pMagicOp->m_strMagicOpName == "�������߰�װħ��״̬" || pMagicOp->m_strMagicOpName == "��װ������״̬" ||
		pMagicOp->m_strMagicOpName == "��װ�˺����״̬" || pMagicOp->m_strMagicOpName == "��װ���۴���״̬" ||
		pMagicOp->m_strMagicOpName == "��װ����״̬" || pMagicOp->m_strMagicOpName == "����߼��ܵȼ���װħ��״̬")
	{
		string strStateName = pMagicOp->m_MagicOpArg->GetTestString();
		CCfgAllStateCheck::MapState::iterator itr = CCfgAllStateCheck::ms_mapState.find(strStateName);
		if ((*itr).second->GetNeedSaveToDB())
		{
			stringstream str;
			str << "\n���ñ�[ħ��Ч��]��" << (pMagicOp->m_nID + 1) << "��Υ��Լ������״̬�Ŀɳ���Ч��[" << CfgChk::g_sMagicEffName << 
				"]��װ��״̬[" <<  pMagicOp->m_MagicOpArg->GetTestString() << "]����������ǿ�Ʋ����\n";
			CfgChk::GenExpInfo(str.str());
			return false;
		}
	}
	return true;
}

bool CCfgMagicOp::CheckPosMagicSetupStateType(CCfgMagicOp* pMagicOp)
{
	if( pMagicOp->m_strMagicOpName == "��װħ��״̬" || pMagicOp->m_strMagicOpName == "������װħ��״̬" ||
		pMagicOp->m_strMagicOpName == "�������߰�װħ��״̬" || pMagicOp->m_strMagicOpName == "����߼��ܵȼ���װħ��״̬")
	{
		string strStateName = pMagicOp->m_MagicOpArg->GetTestString();
		CCfgAllStateCheck::MapState::iterator itr = CCfgAllStateCheck::ms_mapState.find(strStateName);
		if ((*itr).second->GetCascadeType() == "Ψһ")
		{
			stringstream str;
			str << "�����桿���ñ�[ħ��Ч��]��" << (pMagicOp->m_nID + 1) << "��Υ��Լ�����ص�ħ���Ĵ���Ч��[" << CfgChk::g_sMagicEffName << 
				"]��װ��״̬[" <<  pMagicOp->m_MagicOpArg->GetTestString() << "]Ⱥ���������ΪΨһ\n";
			CfgLogErr(str.str());
			//ֻ���治����false
			return true;
		}
	}
	return true;
}

bool CCfgMagicOp::CheckMutexPosMagicSetupStateType(CCfgMagicOp* pMagicOp)
{
	if( pMagicOp->m_strMagicOpName == "��װħ��״̬" || pMagicOp->m_strMagicOpName == "������װħ��״̬" ||
		pMagicOp->m_strMagicOpName == "�������߰�װħ��״̬" || pMagicOp->m_strMagicOpName == "����߼��ܵȼ���װħ��״̬")
	{
		string strStateName = pMagicOp->m_MagicOpArg->GetTestString();
		CCfgAllStateCheck::MapState::iterator itr = CCfgAllStateCheck::ms_mapState.find(strStateName);
		if ((*itr).second->GetCascadeType() == "����")
		{
			stringstream str;
			str << "�����桿���ñ�[ħ��Ч��]��" << (pMagicOp->m_nID + 1) << "��Υ��Լ��������ص�ħ���Ĵ���Ч��[" << CfgChk::g_sMagicEffName << 
				"]��װ��״̬[" <<  pMagicOp->m_MagicOpArg->GetTestString() << "]Ⱥ���������Ϊ����\n";
			CfgChk::GenExpInfo(str.str());
			return false;
		}
	}
	return true;
}
