#include "stdafx.h"
#include "CBaseMagicOp.h"
#include "CCfgColChecker.inl"
#include "CCfgAllStateCheck.h"
#include "CCfgAllMagicCheck.h"
#include "CCfgNormalAttackCheck.h"
#include "CCfgSkillCheck.h"
#include "CCfgProcessCheck.h"
#include "CSkillCfg.h"
#include "CCfgTempVarCheck.h"

bool CBaseMagicOp::CheckObject2MOP(EObjFilterType eObjFilterType)
{
	if(eObjFilterType == eOF_Position)
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ�����ڲ��ǶԵص��ħ�������ﲻ��Ϊ�ص�", eObjFilterType);
		return false;
	}
	return true;
}

// CheckArg2MOP
bool CValueMagicCond::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	pMagicOpArg->SetTypeExpression();
	return true;
}

bool CMagicCondWithEmptyArg::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (pMagicOpArg->GetTestString() != "")
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ������������Ϊ��", pMagicOpArg->GetString());
		return false;
	}
	return true;
}

bool CValueMagicOp::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	pMagicOpArg->SetTypeExpression();
	return true;
}

bool CValueMagicOp::CalcNeedTarget(CCfgCalc* pArg) const
{
	return pArg->CalcNeedTarget();
}

bool CCalInstantHurtMop::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if(pMagicOpArg->IsSingleNumber() && pMagicOpArg->GetIntValue() <= 0)
	{
		CfgChk::GenExpInfo("Υ��ħ����������Լ����ֵ�������0", pMagicOpArg->GetTestString());
		return false;
	}
	return true;
}

// CheckObject2MOP
bool CPosMagicOp::CheckObject2MOP(EObjFilterType eObjFilterType)
{
	if(eObjFilterType != eOF_Position)
	{
		stringstream str;
		CfgChk::GenExpInfo("Υ��ħ������Լ�����ڶԵص��ħ�����������Ϊ�ص�", eObjFilterType);
		return false;
	}
	return true;
}

bool CRelPositionMagicByDirAndDistanceMOP::CheckObject2MOP(EObjFilterType eObjFilterType)
{
	return true;
}

bool CRelPositionMagicRandomInAreaMOP::CheckObject2MOP(EObjFilterType eObjFilterType)
{
	return true;
}

bool CCreateTotemByDirAndDistanceMOP::CheckObject2MOP(EObjFilterType eObjFilterType)
{
	return true;
}

bool CFunctionMagicOpWithEmptyArg::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (pMagicOpArg->GetTestString() != "")
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ������������Ϊ��", pMagicOpArg->GetString());
		return false;
	}
	return true;
}

bool CCPosMagicOpWithEmptyArg::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (pMagicOpArg->GetTestString() != "")
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ������������Ϊ��", pMagicOpArg->GetString());
		return false;
	}
	return true;
}

bool CFunctionCancelableMagicOpWithEmptyArg::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (pMagicOpArg->GetTestString() != "")
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ������������Ϊ��", pMagicOpArg->GetString());
		return false;
	}
	return true;
}

bool CValueCancelableMagicOpWithEmptyArg::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (pMagicOpArg->GetTestString() != "")
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ������������Ϊ��", pMagicOpArg->GetString());
		return false;
	}
	return true;
}

// CheckArg2MOP
bool CIgnoreImmuneSetupSpecialStateMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (!CCfgMagicStateCheck::CheckExist(pMagicOpArg->GetString()))
		return false;
	return true;
}

bool CPassiveSetupMagicStateMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (!CCfgMagicStateCheck::CheckExist(pMagicOpArg->GetString()))
		return false;
	return true;
}

bool CSetupMagicStateMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (!CCfgMagicStateCheck::CheckExist(pMagicOpArg->GetString()))
		return false;
	return true;
}

bool CSetupMagicStateByMaxSkillLevelMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (!CCfgMagicStateCheck::CheckExist(pMagicOpArg->GetString()))
		return false;
	return true;
}

bool CSetupTriggerStateMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (!CCfgTriggerStateCheck::CheckExist(pMagicOpArg->GetString()))
		return false;
	return true;
}

bool CSetupDamageChangeStateMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (!CCfgDamageChangeStateCheck::CheckExist(pMagicOpArg->GetString()))
		return false;
	return true;
}

bool CSetupCumuliTriggerStateMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (!CCfgCumuliTriggerStateCheck::CheckExist(pMagicOpArg->GetString()))
		return false;
	return true;
}

bool CSetupMultiMagicStateMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if(pMagicOpArg->GetStringCount() != 2)
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ������װ���ħ��״̬�Ĳ�������Ϊ2��ֵ", pMagicOpArg->GetTestString());
		return false;
	}
	if (!CCfgMagicStateCheck::CheckExist(pMagicOpArg->GetString(0)))
		return false;
	return true;
}

bool CSetupOrEraseTriggerStateMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (!CCfgTriggerStateCheck::CheckExist(pMagicOpArg->GetString()))
		return false;
	return true;
}

bool CSetupOrEraseMagicStateMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (!CCfgMagicStateCheck::CheckExist(pMagicOpArg->GetString()))
		return false;
	return true;
}

bool CSetupSpecialStateMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (!CCfgSpecialStateCheck::CheckExist(pMagicOpArg->GetString()))
		return false;
	return true;
}

bool CSetupAureMagicMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (!CCfgAureMagicCheck::CheckExist(pMagicOpArg->GetString()))
		return false;
	return true;
}

bool CEraseAureMagicMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (!CCfgAureMagicCheck::CheckExist(pMagicOpArg->GetString()))
		return false;
	return true;
}

bool CEraseAureMagicOnCancellingMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (!CCfgAureMagicCheck::CheckExist(pMagicOpArg->GetString()))
		return false;
	return true;
}

bool CCancelMagicStateCascadeMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	stringstream str;
	if(pMagicOpArg->GetStringCount() != 2)
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ��������ħ��״̬���Ӳ��������Ĳ�������Ϊ2��ֵ", pMagicOpArg->GetTestString());
		return false;
	}
	if (!CCfgMagicStateCheck::CheckExist(pMagicOpArg->GetString(0)))
		return false;

	string sValue = pMagicOpArg->GetString(1);
	stringstream sTempValue;
	int32 iReader = atoi(sValue.c_str());
	sTempValue << iReader;
	if(sTempValue.str() != sValue)
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ��������ħ��״̬���Ӳ��������ĵ�2����������Ϊ�����������ֵ", pMagicOpArg->GetTestString());
		return false;
	}
	if(iReader < 1)
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ��������ħ��״̬���Ӳ��������ĵ�2����������С��", pMagicOpArg->GetTestString());
		return false;
	}
	return true;
}

bool CCancelMagicStateCascadeByEffectMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	CCancelMagicStateCascadeMOP pMop;
	return pMop.CheckArg2MOP(pMagicOpArg);
}

bool CResetMagicStateTimeMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	CSetupMagicStateMOP aMop;
	return aMop.CheckArg2MOP(pMagicOpArg);
}

bool CAddMagicStateTimeMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	stringstream str;
	if(pMagicOpArg->GetStringCount() != 2)
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ��������ħ��״̬ʱ������Ĳ�������Ϊ2��ֵ", pMagicOpArg->GetTestString());
		return false;
	}
	if (!CCfgMagicStateCheck::CheckExist(pMagicOpArg->GetString(0)))
		return false;

	string sValue = pMagicOpArg->GetString(1);
	stringstream sTempValue;
	int32 iReader = atoi(sValue.c_str());
	sTempValue << iReader;
	if(sTempValue.str() != sValue)
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ��������ħ��״̬ʱ������ĵ�2����������Ϊ�����������ֵ", pMagicOpArg->GetTestString());
		return false;
	}
	//if(iReader < 1)
	//{
	//	CfgChk::GenExpInfo("Υ��ħ������Լ��������ħ��״̬���Ӳ��������ĵ�2����������С��", pMagicOpArg->GetTestString());
	//	return false;
	//}
	return true;
}

bool CSetCumuliTriggerStateAccumulateMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	stringstream str;
	if(pMagicOpArg->GetStringCount() != 2)
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ��������ħ��״̬ʱ������Ĳ�������Ϊ2��ֵ", pMagicOpArg->GetTestString());
		return false;
	}
	if (!CCfgCumuliTriggerStateCheck::CheckExist(pMagicOpArg->GetString(0)))
		return false;

	string sValue = pMagicOpArg->GetString(1);
	stringstream sTempValue;
	int32 iReader = atoi(sValue.c_str());
	sTempValue << iReader;
	if(sTempValue.str() != sValue)
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ�������û��۴���״̬�ۻ�ֵ�ĵ�2����������Ϊ�����������ֵ", pMagicOpArg->GetTestString());
		return false;
	}
	if(iReader < 0)
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ�������û��۴���״̬�ۻ�ֵ�ĵ�2����������С��0", pMagicOpArg->GetTestString());
		return false;
	}
	return true;
}

bool CSetDamageChangeStateAccumulateMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	stringstream str;
	if(pMagicOpArg->GetStringCount() != 2)
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ��������ħ��״̬ʱ������Ĳ�������Ϊ2��ֵ", pMagicOpArg->GetTestString());
		return false;
	}
	if (!CCfgDamageChangeStateCheck::CheckExist(pMagicOpArg->GetString(0)))
		return false;

	string sValue = pMagicOpArg->GetString(1);
	stringstream sTempValue;
	int32 iReader = atoi(sValue.c_str());
	sTempValue << iReader;
	if(sTempValue.str() != sValue)
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ���������˺����״̬�ۻ�ֵ�ĵ�2����������Ϊ�����������ֵ", pMagicOpArg->GetTestString());
		return false;
	}
	if(iReader < 0)
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ���������˺����״̬�ۻ�ֵ�ĵ�2����������С��0", pMagicOpArg->GetTestString());
		return false;
	}
	return true;
}

bool CTriggerStateMessageEventMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (!CFunctionMagicOp::CheckArg2MOP(pMagicOpArg))
		return false;

	if (!CCfgTriggerStateCheck::CheckExist(pMagicOpArg->GetString()))
		return false;
	return true;
}

bool CRelMoveMagicAtShockwaveMagicPosMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	string sShockWaveMagicName = pMagicOpArg->GetString(0);
	if(!CCfgShockWaveMagicCheck::CheckExist(sShockWaveMagicName))
	{
		return false;
	}
	string sMoveMagicName = pMagicOpArg->GetString(1);
	if(sMoveMagicName.empty())
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ����λ��ħ��Ϊ��", pMagicOpArg->GetTestString());
		return false;
	}
	else if(!CCfgMoveMagicCheck::CheckExist(sMoveMagicName))
	{
		CfgChk::GenExpInfo("λ��ħ��������", pMagicOpArg->GetTestString());
		return false;
	}
	return true;
}

bool CRelMoveMagicMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	string sName = pMagicOpArg->GetString();
	if(sName.empty())
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ����λ��ħ��Ϊ��", pMagicOpArg->GetTestString());
		return false;
	}
	else if(!CCfgMoveMagicCheck::CheckExist(sName))
	{
		CfgChk::GenExpInfo("λ��ħ��������", pMagicOpArg->GetTestString());
		return false;
	}
	return true;
}

bool CCancelableRelMoveMagicMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	CRelMoveMagicMOP aMOP;
	return aMOP.CheckArg2MOP(pMagicOpArg);
}

bool CRelTransferMagicMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	string sName = pMagicOpArg->GetString();
	if(!CCfgTransMagicCheck::CheckExist(sName))
	{
		return false;
	}
	return true;
}

bool CRelBulletMagicMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	string sName = pMagicOpArg->GetString();
	if(!CCfgBulletMagicCheck::CheckExist(sName))
	{
		return false;
	}
	return true;
}

bool CDelayRelBulletMagicMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	string sName = pMagicOpArg->GetString();
	if(!CCfgBulletMagicCheck::CheckExist(sName))
	{
		return false;
	}
	return true;
}

bool CRelShockWaveMagicMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	string sName = pMagicOpArg->GetString();
	if(!CCfgShockWaveMagicCheck::CheckExist(sName))
	{
		return false;
	}
	return true;
}

bool CPosCreateNpcWithMasterMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	return true;
}

bool CRelPositionMagicMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	string sName = pMagicOpArg->GetString();
	if(!CCfgPositionMagicCheck::CheckExist(sName))
	{
		return false;
	}
	return true;
}

bool CRelPositionMagicAtTargetPosMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	CRelPositionMagicMOP aMop;
	return aMop.CheckArg2MOP(pMagicOpArg);
}

bool CRelShockWaveMagicAtTargetPosMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	CRelShockWaveMagicMOP aMop;
	return aMop.CheckArg2MOP(pMagicOpArg);
}

bool CRelTickMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if(pMagicOpArg->GetStringCount() != 2)
	{
		CfgChk::GenExpInfo("[ʩ����ʱħ��Ч��]ħ��������������Ϊ2����", pMagicOpArg->GetTestString().c_str());
		return false;
	}
	CCfgMagicEffCheck::CheckMagicEffExist(pMagicOpArg->GetString(1));
	CCfgCalc pCfgCacl(pMagicOpArg->GetString(0));
	/*if (pCfgCacl.GetDblValue() < 0)
	{
		CfgChk::GenExpInfo("[ʩ����ʱħ��Ч��]ħ�������ڶ�����������Ϊ������", pMagicOpArg->GetTestString().c_str());
		return false;
	}*/
	return true;
}

bool CCreateNPCMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (pMagicOpArg->GetTestString() == "")
	{
		CfgChk::GenExpInfo("[����NPC]ħ��������������Ϊ��");
		return false;
	}
	return true;
}

bool CDestoryNPCMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	return true;
}

bool CSetupUncancellableMagicStateMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (!CCfgMagicStateCheck::CheckExist(pMagicOpArg->GetString()))
		return false;
	return true;
}

bool CSetupOrEraseAureMagicMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (!CCfgAureMagicCheck::CheckExist(pMagicOpArg->GetString()))
		return false;
	return true;
}

bool CChangeAureMagicMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (!CCfgAureMagicCheck::CheckExist(pMagicOpArg->GetString()))
		return false;
	return true;
}

bool CCreateServantMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (pMagicOpArg->GetString() == "")
	{
		CfgChk::GenExpInfo("[��������]ħ��������������Ϊ��");
		return false;
	}
	return true;
}

bool CCreateServantWithTargetLevelFollowMasterMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (pMagicOpArg->GetString() == "")
	{
		CfgChk::GenExpInfo("[�������ﲢ����Ŀ��]ħ��������������Ϊ��");
		return false;
	}
	return true;
}

bool CCreatePassiveNPCMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (pMagicOpArg->GetString() == "")
	{
		CfgChk::GenExpInfo("[��������NPC]ħ��������������Ϊ��");
		return false;
	}
	return true;
}

bool CCreateNPCOnTargetPosMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (pMagicOpArg->GetString() == "")
	{
		CfgChk::GenExpInfo("[Ŀ��λ�ô���NPC]ħ��������������Ϊ��");
		return false;
	}
	return true;
}

bool CSetServantLeftTimeMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	pMagicOpArg->SetTypeExpression();
	return true;
}

bool CTauntMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	pMagicOpArg->SetTypeExpression();
	return true;
}

bool CNormalAttackProduceRagePoint::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	pMagicOpArg->SetTypeExpression();
	return true;
}

bool CBeDamagePreduceRagePoint::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	pMagicOpArg->SetTypeExpression();
	return true;
}

bool CRelNPCDirSkillMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (pMagicOpArg->GetStringCount() != 2)
	{
		CfgChk::GenExpInfo("[NPCʩ�ŷ�����]ħ����������������2��");
		return false;
	}
	string strSkillName = pMagicOpArg->GetString(0);
	if (!CCfgSkillCheck::IsNPCSkillNameValid(strSkillName.c_str()))
	{
		CfgChk::GenExpInfo("���ܲ�����", pMagicOpArg->GetTestString());
		return false;
	}
	CCfgCalc pCfgCalc(pMagicOpArg->GetString(1));
	pCfgCalc.SetTypeExpression();
	return true;
}

bool CTargetSetupMagicStateToSelfMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (!CCfgMagicStateCheck::CheckExist(pMagicOpArg->GetString()))
		return false;
	return true;
}

bool CEraseIncreaseStateMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (pMagicOpArg->GetTestString() != "")
	{
		pMagicOpArg->SetTypeExpression();
	}
	return true;
}

bool CEraseAllDecreaseStateMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (pMagicOpArg->GetTestString() != "")
	{
		pMagicOpArg->SetTypeExpression();
	}
	return true;
}

bool CResetSingleCoolDownTimeMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (!CCfgSkillCheck::IsNPCSkillNameValid(pMagicOpArg->GetString().c_str()))
	{
		CfgChk::GenExpInfo("���ܲ�����", pMagicOpArg->GetTestString());
		return false;
	}
	return true;
}

bool CTargetRelBulletMagicMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (!CCfgBulletMagicCheck::CheckExist(pMagicOpArg->GetString().c_str()))
	{
		CfgChk::GenExpInfo("�ӵ�ħ��������", pMagicOpArg->GetTestString());
		return false;
	}
	return true;
}

bool CTargetBurstSoulFXMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (!CCfgBulletMagicCheck::CheckExist(pMagicOpArg->GetString().c_str()))
	{
		CfgChk::GenExpInfo("�ӵ�ħ��������", pMagicOpArg->GetTestString());
		return false;
	}
	return true;
}

bool CBurstSoulFXMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (!CCfgBulletMagicCheck::CheckExist(pMagicOpArg->GetString().c_str()))
	{
		CfgChk::GenExpInfo("�ӵ�ħ��������", pMagicOpArg->GetTestString());
		return false;
	}
	return true;
}

bool CBurstSoulBallFXMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (!CCfgBulletMagicCheck::CheckExist(pMagicOpArg->GetString().c_str()))
	{
		CfgChk::GenExpInfo("�ӵ�ħ��������", pMagicOpArg->GetTestString());
		return false;
	}
	return true;
}

bool CChangeSceneMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (pMagicOpArg->GetStringCount() != 3)
	{
		CfgChk::GenExpInfo("[�س�]ħ��������������Ϊ3��");
		return false;
	}
	CCfgCalc pCfgCalcX(pMagicOpArg->GetString(1));
	pCfgCalcX.SetTypeExpression();
	CCfgCalc pCfgCalcY(pMagicOpArg->GetString(1));
	pCfgCalcY.SetTypeExpression();
	return true;
}

bool CReflectStateByDecreaseTypeMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (pMagicOpArg->GetString() == "")
	{
		CfgChk::GenExpInfo("[��������NPC]ħ��������������Ϊ��");
		return false;
	}
	return true;
}

bool CRelBattleArrayMagicMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	return true;
}

bool CRelPosBulletMagicMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	string sName = pMagicOpArg->GetString();
	if(!CCfgBulletMagicCheck::CheckExist(sName))
	{
		return false;
	}
	return true;
}

bool CNonCombatBehaviorMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	pMagicOpArg->SetTypeExpression();
	return true;
}

bool CRelPosMoveMagicMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	string sName = pMagicOpArg->GetString();
	if(sName.empty())
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ����λ��ħ��Ϊ��", pMagicOpArg->GetTestString());
		return false;
	}
	else if(!CCfgMoveMagicCheck::CheckExist(sName))
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ����λ��ħ��������", pMagicOpArg->GetTestString());
		return false;
	}
	return true;
}

bool CEraseMoveMagicMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	CRelMoveMagicMOP aMop;
	return aMop.CheckArg2MOP(pMagicOpArg);
}

bool CErasePositionMagicMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	CRelPositionMagicMOP aMop;
	return aMop.CheckArg2MOP(pMagicOpArg);
}

bool CEraseAllPositionMagicMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (pMagicOpArg->GetTestString() != "")
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ������������Ϊ��", pMagicOpArg->GetString());
		return false;
	}
	return true;
}

bool CReplacePositionMagicMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if(pMagicOpArg->GetStringCount() != 2)
	{
		CfgChk::GenExpInfo("[�滻�ص�ħ��]ħ��������������Ϊ2����", pMagicOpArg->GetTestString().c_str());
		return eHR_Fail;
	}

	if (!CCfgPositionMagicCheck::CheckExist(pMagicOpArg->GetString(0)))
		return false;
	if (!CCfgPositionMagicCheck::CheckExist(pMagicOpArg->GetString(1)))
		return false;

	return true;
}

bool CTestMagicStateMCOND::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (!CCfgMagicStateCheck::CheckExist(pMagicOpArg->GetString()))
		return false;
	return true;
}

bool CNotTestMagicStateAndSelfExistTalentMCOND::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if(pMagicOpArg->GetStringCount() != 2)
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ������[����ħ��״̬�����������츳]�Ĳ�������Ϊ2��ֵ", pMagicOpArg->GetTestString());
		return false;
	}
	if (!CCfgMagicStateCheck::CheckExist(pMagicOpArg->GetString(0)))
		return false;
	return true;
}

bool CTestNotInDamageChangeStateMCOND::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (!CCfgDamageChangeStateCheck::CheckExist(pMagicOpArg->GetString()))
		return false;
	return true;
}

bool CTestNotInMagicStateMCOND::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (!CCfgMagicStateCheck::CheckExist(pMagicOpArg->GetString()))
		return false;
	return true;
}

bool CTestNotInRepeatedMagicStateMCOND::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (!CCfgMagicStateCheck::CheckExist(pMagicOpArg->GetString()))
		return false;
	return true;
}

bool CTestNotInSpecialStateMCOND::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (!CCfgSpecialStateCheck::CheckExist(pMagicOpArg->GetString()))
		return false;
	return true;
}

bool CTestSelfMagicStateMCOND::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (!CCfgMagicStateCheck::CheckExist(pMagicOpArg->GetString()))
		return false;
	return true;
}

bool CTestTriggerStateMCOND::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (!CCfgTriggerStateCheck::CheckExist(pMagicOpArg->GetString()))
		return false;
	return true;
}

bool CTestSelfTriggerStateMCOND::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (!CCfgTriggerStateCheck::CheckExist(pMagicOpArg->GetString()))
		return false;
	return true;
}

bool CTestDamageChangeStateMCOND::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (!CCfgDamageChangeStateCheck::CheckExist(pMagicOpArg->GetString()))
		return false;
	return true;
}

bool CTestSelfDamageChangeStateMCOND::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (!CCfgDamageChangeStateCheck::CheckExist(pMagicOpArg->GetString()))
		return false;
	return true;
}

bool CTestSpecialStateMCOND::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (!CCfgSpecialStateCheck::CheckExist(pMagicOpArg->GetString()))
		return false;
	return true;
}

bool CTestSelfSpecialStateMCOND::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (!CCfgSpecialStateCheck::CheckExist(pMagicOpArg->GetString()))
		return false;
	return true;
}

bool CTestNotInStateByDecreaseTypeMCOND::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if(pMagicOpArg->GetTestString() == "") return true;
	if(!CCfgAllStateCheck::ExistDecreaseType(pMagicOpArg->GetString()))
	{
		stringstream str;
		CfgChk::GenExpInfo("Υ��ħ������Լ���������ڸ���Ч��״̬�����ͳ���ȡֵ��Χ", pMagicOpArg->GetTestString());
		return false;
	}

	return true;
}

bool CNotTestInStateByDecreaseTypeAndSelfExistTalentMCOND::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if(pMagicOpArg->GetStringCount() != 2)
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ������[���ڸ���״̬�����������츳]�Ĳ�������Ϊ2��ֵ", pMagicOpArg->GetTestString());
		return false;
	}
	if(!CCfgAllStateCheck::ExistDecreaseType(pMagicOpArg->GetString(0)))
	{
		stringstream str;
		CfgChk::GenExpInfo("Υ��ħ������Լ���������ڸ���Ч��״̬�����ͳ���ȡֵ��Χ", pMagicOpArg->GetTestString());
		return false;
	}

	return true;
}

bool CTestMagicStateCascadeGreaterOrEqualMCOND::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	stringstream str;
	if(pMagicOpArg->GetStringCount() != 2)
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ����ħ��״̬���Ӳ������ڵ��������Ĳ�������Ϊ2��ֵ", pMagicOpArg->GetTestString());
		return false;
	}
	if (!CCfgMagicStateCheck::CheckExist(pMagicOpArg->GetString(0)))
		return false;

	string sValue = pMagicOpArg->GetString(1);
	stringstream sTempValue;
	int32 iReader = atoi(sValue.c_str());
	sTempValue << iReader;
	if(sTempValue.str() != sValue)
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ����ħ��״̬���Ӳ������ڵ��������ĵ�2����������Ϊ�����������ֵ", pMagicOpArg->GetTestString());
		return false;
	}
	if(iReader < 1)
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ����ħ��״̬���Ӳ������ڵ��������ĵ�2����������С��0", pMagicOpArg->GetTestString());
		return false;
	}
	return true;
}

bool CTestAureMagicMCOND::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if(!CCfgAureMagicCheck::CheckExist(pMagicOpArg->GetString())) 
		return false;
	return true;
}

bool CProbabilityLesserCond::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	pMagicOpArg->SetTypeExpression();
	if(pMagicOpArg->IsSingleNumber())
	{
		float fProbability = float(pMagicOpArg->GetDblValue());
		if(fProbability < 0.0f || fProbability > 1.0f)
		{
			CfgChk::GenExpInfo("Υ��ħ������Լ��������С�ڳ���ȡֵ��Χ", pMagicOpArg->GetTestString());
			return false;
		}
	}
	return true;
}

bool CTestExpIsVestInSelfAndTriggerStateMCOND::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (!CCfgTriggerStateCheck::CheckExist(pMagicOpArg->GetString()))
		return false;
	return true;
}

bool CTestTargetLearnSkillMCOND::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (!CCfgSkillCheck::IsPlayerSkillNameValid(pMagicOpArg->GetString().c_str()))
		return false;
	return true;
}

bool CTestTargetSkillIsCoolDownMCOND::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (!CCfgSkillCheck::IsPlayerSkillNameValid(pMagicOpArg->GetString().c_str()))
		return false;
	return true;
}

bool CTestTargetInScene::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (pMagicOpArg->GetStringCount() != 2)
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ������������Ϊ2��");
		return false;
	}
	return true;	
}

bool CTestDebaseEnmityMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	return true;
}

bool CTestTargetLevelLesserOrEqualMCOND::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (pMagicOpArg->GetTestString() == "")
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ������������Ϊ��");
		return false;
	}
	int32 iDiffLevel = pMagicOpArg->GetIntValue();
	if (iDiffLevel<0)
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ������������С��0");
		return false;
	}
	return true;
}

bool CTestTargetLevelLesserOrEqualToExpOwnerMCOND::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	pMagicOpArg->SetTypeExpression();
	return true;
}

bool CRelPassiveSkillMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (!CCfgSkillCheck::IsPlayerSkillNameValid(pMagicOpArg->GetString().c_str()))
	{
		stringstream str;
		CfgChk::GenExpInfo("���ܲ�����", pMagicOpArg->GetTestString());
		return false;
	}
	else if (CCfgProcessCheck::CheckExist(pMagicOpArg->GetString()))
	{
		stringstream str;
		CfgChk::GenExpInfo("���ʩ�ű������ܲ���ʩ�����̼���", pMagicOpArg->GetTestString());
		return false;
	}
	else
	{
		return true;
	}
}

bool CRelNPCSkillMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (!CCfgSkillCheck::IsNPCSkillNameValid(pMagicOpArg->GetString().c_str()))
	{
		stringstream str;
		CfgChk::GenExpInfo("���ܲ�����", pMagicOpArg->GetTestString());
		return false;
	}
	else if (CCfgProcessCheck::CheckExist(pMagicOpArg->GetString()))
	{
		stringstream str;
		CfgChk::GenExpInfo("���ʩ�ű������ܲ���ʩ�����̼���", pMagicOpArg->GetTestString());
		return false;
	}
	else
	{
		return true;
	}	
}

bool CEraseSelfStateMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if(!CCfgAllStateCheck::CheckExistAllState(pMagicOpArg->GetString()))
	{
		stringstream str;
		CfgChk::GenExpInfo("״̬������:", pMagicOpArg->GetString());
		return false;
	}
	return true;
}

bool CEraseStateMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if(!CCfgAllStateCheck::CheckExistAllState(pMagicOpArg->GetString()))
	{
		stringstream str;
		CfgChk::GenExpInfo("״̬������:", pMagicOpArg->GetString());
		return false;
	}
	return true;
}

bool CEraseStateOnCancellingMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if(!CCfgAllStateCheck::CheckExistAllState(pMagicOpArg->GetString()))
	{
		stringstream str;
		CfgChk::GenExpInfo("״̬������:", pMagicOpArg->GetString());
		return false;
	}
	return true;
}

bool CEraseStateByDecreaseTypeMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if(!CCfgAllStateCheck::ExistDecreaseType(pMagicOpArg->GetString(0)))
	{
		stringstream str;
		CfgChk::GenExpInfo("Υ��ħ������Լ�����������Ч��״̬�����ͳ���ȡֵ��Χ", pMagicOpArg->GetTestString());
		return false;
	}

	if(pMagicOpArg->GetStringCount() >= 2)
	{
		if(pMagicOpArg->GetString(1) == "")
		{
			stringstream str;
			CfgChk::GenExpInfo("Υ��ħ������Լ�����������Ч��״̬�Ĵ�������Ϊ��", pMagicOpArg->GetTestString());
			return false;
		}


		CCfgCalc *calcCount;
		CfgChk::SetValue(calcCount, pMagicOpArg->GetString(1));
		if(calcCount->IsSingleNumber())
		{
			int32 iCount = calcCount->GetIntValue();
			if(iCount < -1 || iCount == 0)
			{
				delete calcCount;
				calcCount = NULL;
				stringstream str;
				CfgChk::GenExpInfo("Υ��ħ������Լ�����������Ч��״̬�Ĵ�������Ϊ-1�������������ֵ", pMagicOpArg->GetTestString());
				return false;
			}
		}
		delete calcCount;
		calcCount = NULL;
	}

	return true;
}

bool CTestWeaponTypeMCOND::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if(CSkillCfg::ms_mapAttackType.find(pMagicOpArg->GetString()) == CSkillCfg::ms_mapAttackType.end())
		return false;
	else
		return true;
}

bool CTestIsAvailableDeadBobyMCOND::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	stringstream str;
	if(pMagicOpArg->GetStringCount() != 2)
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ������Χ���Ƿ��п���ʬ�������Ĳ�������Ϊ2��ֵ", pMagicOpArg->GetTestString());
		return false;
	}
	if (!CCfgSpecialStateCheck::CheckExist(pMagicOpArg->GetString(0)))
		return false;
	if (!CCfgSpecialStateCheck::CheckExist(pMagicOpArg->GetString(1)))
		return false;

	return true;
}

bool CImmuneTaskTypeDamageMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if(!CCfgTempVarCheck::IsImmuneTaskAttackName(pMagicOpArg->GetString()))
	{
		stringstream sExp;
		sExp << "������������[" << pMagicOpArg->GetTestString() << "]<<��������map��\n";
		CfgChk::GenExpInfo(sExp.str());
		return false;
	}
	return true;
}

bool CChangeExpRateMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	pMagicOpArg->SetTypeExpression();
	if(!pMagicOpArg->IsSingleNumber() )
	{
		CfgChk::GenExpInfo("Υ��ħ����������Լ����ֵ����Ϊ����", pMagicOpArg->GetTestString());
		return false;
	}
	return true;
}

bool CChangeExpRateInFBMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	pMagicOpArg->SetTypeExpression();
	if(!pMagicOpArg->IsSingleNumber())
	{
		CfgChk::GenExpInfo("Υ��ħ����������Լ����ֵ����Ϊ����", pMagicOpArg->GetTestString());
		return false;
	}
	return true;
}

bool CChangeFetchRateMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	pMagicOpArg->SetTypeExpression();
	if(!pMagicOpArg->IsSingleNumber())
	{
		CfgChk::GenExpInfo("Υ��ħ����������Լ����ֵ����Ϊ����", pMagicOpArg->GetTestString());
		return false;
	}
	return true;
}

bool CNonCombatStateMagicOp::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (pMagicOpArg->GetString() == "")
	{
		CfgChk::GenExpInfo("Υ��ħ����������Լ������ս����Ϊ״̬������������Ϊ��", pMagicOpArg->GetTestString());
		return false;
	}
	return true;
}

bool CDoAllBodyActionMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (pMagicOpArg->GetString() == "")
	{
		CfgChk::GenExpInfo("Υ��ħ����������Լ��������ȫ����������������Ϊ��", pMagicOpArg->GetTestString());
		return false;
	}
	return true;
}

bool CUseNormalHorseMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if(pMagicOpArg->GetStringCount() != 2)
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ����ʹ����ͨ�����������Ϊ2��ֵ", pMagicOpArg->GetTestString());
		return false;
	}
	return true;
}

bool CUseBattleHorseMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if(pMagicOpArg->GetStringCount() != 2)
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ����ʹ��ս�������������Ϊ2��ֵ", pMagicOpArg->GetTestString());
		return false;
	}
	return true;
}

bool CCallBattleHorseMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if(pMagicOpArg->GetStringCount() != 2)
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ�����ٻ�ս�������������Ϊ2��ֵ", pMagicOpArg->GetTestString());
		return false;
	}
	return true;
}

bool CChangeCampMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (pMagicOpArg->GetString() == "")
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ�����ı���Ӫ��������Ϊ��");
		return false;
	}
	return true;
}

bool COnlyPlayFX::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (pMagicOpArg->GetTestString() != "") 
	{
		CCfgMagicEffCheck::CheckMagicEffExist(pMagicOpArg->GetTestString());
	}
	return true;
}

bool CLockAgileValueMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (pMagicOpArg->GetString() == "")
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ�����ı���Ӫ��������Ϊ��");
		return false;
	}
	return true;
}

bool CSoulLinkMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	pMagicOpArg->SetTypeExpression();
	return true;
}

bool CTestTargetLevelIsInScopeMCOND::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if(pMagicOpArg->GetStringCount() != 2)
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ����Ŀ��ȼ��Ƿ��������Ĳ�������Ϊ2��ֵ", pMagicOpArg->GetTestString());
		return false;
	}

	CCfgCalc* pMinLevel = NULL;
	CCfgCalc* pMaxLevel = NULL;
	CfgChk::SetValue(pMinLevel, pMagicOpArg->GetString(0));
	CfgChk::SetValue(pMaxLevel, pMagicOpArg->GetString(1));
	if(pMinLevel->IsSingleNumber())
	{
		int32 iLevel = pMinLevel->GetIntValue();
		if(iLevel < 0)
		{
			delete pMinLevel;
			pMinLevel = NULL;
			stringstream str;
			CfgChk::GenExpInfo("Υ��ħ������Լ����Ŀ��ȼ��Ƿ��������ĵ�һ����������С��0", pMagicOpArg->GetTestString());
			return false;
		}
	}

	if(pMaxLevel->IsSingleNumber())
	{
		int32 iLevel = pMaxLevel->GetIntValue();
		if(iLevel < 0)
		{
			delete pMaxLevel;
			pMaxLevel = NULL;
			stringstream str;
			CfgChk::GenExpInfo("Υ��ħ������Լ����Ŀ��ȼ��Ƿ��������ĵڶ�����������С��0", pMagicOpArg->GetTestString());
			return false;
		}
	}


	if(pMaxLevel->IsSingleNumber()&&pMinLevel->IsSingleNumber())
	{
		int32 iMinLevel = pMinLevel->GetIntValue();
		int32 iMaxLevel = pMaxLevel->GetIntValue();
		if (iMinLevel > iMaxLevel)
		{
			delete pMinLevel;
			pMinLevel = NULL;
			delete pMaxLevel;
			pMaxLevel = NULL;
			CfgChk::GenExpInfo("Υ��ħ������Լ����Ŀ��ȼ��Ƿ��������ĵ�һ���������ܴ��ڵڶ�������", pMagicOpArg->GetTestString());
			return false;
		}
	}

	delete pMinLevel;
	pMinLevel = NULL;
	delete pMaxLevel;
	pMaxLevel = NULL;
	return true;
}

bool CSetNoSingTimeAboutNextSkill::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	string strSkillName = pMagicOpArg->GetString();
	if (strSkillName != "ȫ��" && strSkillName != "ս������"
		&& !CCfgSkillCheck::IsPlayerSkillNameValid(pMagicOpArg->GetString().c_str()))
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ������һ������ʱ��Ϊ�ղ����ļ��ܲ�����", pMagicOpArg->GetTestString());
		return false;
	}
	return true;
}

bool CTestTargetInCastingProcess::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if(!pMagicOpArg->GetString().empty() && CSkillCfg::ms_mapAttackType[pMagicOpArg->GetString()] == 0)
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ������������Ϊ��������������ַ���", pMagicOpArg->GetString());
		return false;
	}
	return true;
}

bool CRelPositionMagicByDirAndDistanceMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	string magicName = pMagicOpArg->GetString(0);
	if(pMagicOpArg->GetStringCount() > 2)
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ�����������ܶ���2��ֵ", pMagicOpArg->GetTestString());
		return false;
	}

	int32 iDis;
	CfgChk::SetValue(iDis, pMagicOpArg->GetString(1));
	CfgChk::CheckUIntType(iDis);

	return true;
}

bool CRelPositionMagicRandomInAreaMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	string magicName = pMagicOpArg->GetString(0);
	if(pMagicOpArg->GetStringCount() != 3)
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ��������ӦΪ (�ص�ħ��������Χ������) 3��", pMagicOpArg->GetTestString());
		return false;
	}

	int32 iDis;
	CfgChk::SetValue(iDis, pMagicOpArg->GetString(1));
	CfgChk::CheckUIntType(iDis);
	int32 iNum;
	CfgChk::SetValue(iNum, pMagicOpArg->GetString(2));
	CfgChk::CheckUIntType(iNum);
	return true;
}

bool CCreateTotemByDirAndDistanceMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	string magicName = pMagicOpArg->GetString(0);
	if(pMagicOpArg->GetStringCount() > 2)
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ�����������ܶ���2��ֵ", pMagicOpArg->GetTestString());
		return false;
	}

	int32 iDis;
	CfgChk::SetValue(iDis, pMagicOpArg->GetString(1));
	CfgChk::CheckUIntType(iDis);

	return true;
}

bool CTestRidingAureMagicMCOND::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (!CCfgAureMagicCheck::CheckExist(pMagicOpArg->GetString()))
		return false;
	return true;
}

bool CTestBurstSoulTimesGreaterOrEqualMCOND::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	pMagicOpArg->SetTypeExpression();
	return true;
}

bool CTestContinuousBurstSoulTimesEqualMCOND::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	pMagicOpArg->SetTypeExpression();
	return true;
}

bool CEraseEnemyPositionMagicMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (pMagicOpArg->GetStringCount() != 2)
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ������������������Ӧ����2��", pMagicOpArg->GetTestString());
		return false;
	}
	string sName = pMagicOpArg->GetString(1);
	if(!CCfgPositionMagicCheck::CheckExist(sName))
		return false;
	return true;
}

bool CPursueKillMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (pMagicOpArg->GetStringCount() != 2)
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ������������������Ӧ����2��", pMagicOpArg->GetTestString());
		return false;
	}
	return true;
}

bool CSetupDynamicBarrierMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (!CCfgBarrierMagicCheck::CheckExist(pMagicOpArg->GetString()))
		return false;
	return true;
}

bool CCreateTotemMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (pMagicOpArg->GetTestString() == "")
	{
		CfgChk::GenExpInfo("[����NPC]ħ��������������Ϊ��");
		return false;
	}
	return true;
}
bool CPosCreateNPCMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (pMagicOpArg->GetTestString() == "")
	{
		CfgChk::GenExpInfo("[����NPC]ħ��������������Ϊ��");
		return false;
	}
	return true;
}
bool CCreateShadowMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (pMagicOpArg->GetTestString() == "")
	{
		CfgChk::GenExpInfo("[����NPC]ħ��������������Ϊ��");
		return false;
	}
	return true;
}
bool CPosCreateNPCLevelSameMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (pMagicOpArg->GetTestString() == "")
	{
		CfgChk::GenExpInfo("[����NPC]ħ��������������Ϊ��");
		return false;
	}
	return true;
}
bool CTargetPosCreateNPCMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (pMagicOpArg->GetTestString() == "")
	{
		CfgChk::GenExpInfo("[����NPC]ħ��������������Ϊ��");
		return false;
	}
	return true;
}

bool CDelayRelPositionMagicMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (pMagicOpArg->GetStringCount() != 2)
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ������������������Ӧ����2��", pMagicOpArg->GetTestString());
		return false;
	}
	return true;
}
bool CPosDelayCreateNPCMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (pMagicOpArg->GetStringCount() != 2)
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ������������������Ӧ����2��", pMagicOpArg->GetTestString());
		return false;
	}
	return true;
}
bool CTargetPosCreateNPCLevelSameMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (pMagicOpArg->GetTestString() == "")
	{
		CfgChk::GenExpInfo("[����NPC]ħ��������������Ϊ��");
		return false;
	}
	return true;
}
bool CDelayRelShockWaveMagicMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (!CCfgShockWaveMagicCheck::CheckExist(pMagicOpArg->GetString()))
	{
		return false;
	}
	return true;
}

bool CTestTargetHavePositionMagic::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	CRelPositionMagicMOP aMop;
	return aMop.CheckArg2MOP(pMagicOpArg);
}

bool CTestMainHandEquipMCOND::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (pMagicOpArg->GetTestString() == "")
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ�����������벻�ܿ�", pMagicOpArg->GetString());
		return false;
	}
	return true;
}

bool CTestAssistHandEquipMCOND::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (pMagicOpArg->GetTestString() == "")
	{
		return true;
	}
	CfgChk::GenExpInfo("Υ��ħ������Լ������������Ϊ��", pMagicOpArg->GetString());
	return false;
}

bool CTestIsExistPetMCOND::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	return true;
}

bool CTestIsNotExistPetMCOND::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	return true;
}

bool CTestTargetClassMCOND::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (pMagicOpArg->GetTestString() == "")
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ�����������벻�ܿ�", pMagicOpArg->GetString());
		return false;
	}
	return true;
}

bool CTestTargetIsNPCMCOND::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	return true;
}

bool CTestRidingMCOND::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (pMagicOpArg->GetTestString() == "")
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ�����������벻�ܿ�", pMagicOpArg->GetString());
		return false;
	}
	return true;
}

bool CTestTargetDistIsLesserThanMCOND::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	pMagicOpArg->SetTypeExpression();
	return true;
}

bool CTestTargetIsCamp::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	pMagicOpArg->SetTypeExpression();
	return true;
}

bool CChangePercentMoveSpeedMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	pMagicOpArg->SetTypeExpression();
	if(pMagicOpArg->IsConstValue() && pMagicOpArg->GetDblValue() <= -1.0)
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ�����ı��ٶȰٷֱȵĲ�����Ӧ��С�ڵ���-1", pMagicOpArg->GetTestString());
		return false;
	}
	return true;
}

bool CChangePercentWalkSpeedMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	pMagicOpArg->SetTypeExpression();
	if(pMagicOpArg->IsConstValue() && pMagicOpArg->GetDblValue() <= -1.0)
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ�����ı��ٶȰٷֱȵĲ�����Ӧ��С�ڵ���-1", pMagicOpArg->GetTestString());
		return false;
	}
	return true;
}

bool CChangePercentNormalAttackSpeedMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	pMagicOpArg->SetTypeExpression();
	if(pMagicOpArg->IsConstValue() && pMagicOpArg->GetDblValue() <= -1.0)
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ�����ı��ٶȰٷֱȵĲ�����Ӧ��С�ڵ���-1", pMagicOpArg->GetTestString());
		return false;
	}
	return true;
}

bool CCalValueMagicOp::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	pMagicOpArg->SetTypeExpression();
	if((pMagicOpArg->IsConstValue() && pMagicOpArg->GetDblValue() < 0.0) || pMagicOpArg->GetTestString().find("0-") != -1)
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ���������˺������Ĳ�����Ӧ��С��0", pMagicOpArg->GetTestString());
		return false;
	}
	return true;
}

bool CTalentChangeManaPointMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (pMagicOpArg->GetStringCount() != 2)
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ������������������Ӧ����2��", pMagicOpArg->GetTestString());
		return false;
	}
	CCfgCalc tempArp(pMagicOpArg->GetString(0));
	tempArp.SetTypeExpression();

	if(!CTalentHolder::ExistName(pMagicOpArg->GetString(1))) return false;
	return true;
}

bool CAddBurstSoulTimesToExpOwnerMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	pMagicOpArg->SetTypeExpression();
	return true;
}

bool CConsumeBurstSoulTimesMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	pMagicOpArg->SetTypeExpression();
	return true;
}

bool CBurstSoulPrizeToExpOwnerMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	pMagicOpArg->SetTypeExpression();
	return true;
}

bool CCBurstSoulExtraPrizeToExpOwnerMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if(!CCfgBurstSoulCheck::CheckExistType(pMagicOpArg->GetString()))
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ����[�Ծ���ӵ���߽����긽��]��������ȷ");
		return false;
	}
	return true;
}

bool CAddContinuousBurstTimesMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	pMagicOpArg->SetTypeExpression();
	return true;
}

bool CAddBurstSoulTimesMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	pMagicOpArg->SetTypeExpression();
	return true;
}

bool CDisplayMsgMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (pMagicOpArg->GetStringCount() != 2)
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ������������������Ӧ����2��", pMagicOpArg->GetTestString());
		return false;
	}

	if (pMagicOpArg->GetString(0) == "")
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ��������1���벻�ܿ�", pMagicOpArg->GetString());
		return false;
	}

	if (pMagicOpArg->GetString(1) == "")
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ��������2���벻�ܿ�", pMagicOpArg->GetString());
		return false;
	}
	return true;
}

bool CDestroyServantOnCancellingMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (pMagicOpArg->GetTestString() == "")
	{
		CfgChk::GenExpInfo("[����ʱ���ٳ���]ħ��������������Ϊ��");
		return false;
	}
	return true;
}

bool CDoWorldSkillMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (pMagicOpArg->GetStringCount() != 2)
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ������������������Ӧ����2��", pMagicOpArg->GetTestString());
		return false;
	}

	if (pMagicOpArg->GetString(0) == "")
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ��������1���벻�ܿ�", pMagicOpArg->GetString());
		return false;
	}

	if (pMagicOpArg->GetString(1) == "")
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ��������2���벻�ܿ�", pMagicOpArg->GetString());
		return false;
	}

	if (!CCfgSkillCheck::IsPlayerSkillNameValid(pMagicOpArg->GetString(0).c_str()))
	{
		stringstream str;
		CfgChk::GenExpInfo("���ܲ�����", pMagicOpArg->GetTestString());
		return false;
	}
	return true;
}

bool CDoCampSkillMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (pMagicOpArg->GetStringCount() != 2)
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ������������������Ӧ����2��", pMagicOpArg->GetTestString());
		return false;
	}

	if (pMagicOpArg->GetString(0) == "")
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ��������1���벻�ܿ�", pMagicOpArg->GetString());
		return false;
	}

	if (pMagicOpArg->GetString(1) == "")
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ��������2���벻�ܿ�", pMagicOpArg->GetString());
		return false;
	}

	if (!CCfgSkillCheck::IsPlayerSkillNameValid(pMagicOpArg->GetString(0).c_str()))
	{
		stringstream str;
		CfgChk::GenExpInfo("���ܲ�����", pMagicOpArg->GetTestString());
		return false;
	}
	return true;
}

bool CDoArmyCorpsSkillMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (pMagicOpArg->GetStringCount() != 2)
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ������������������Ӧ����2��", pMagicOpArg->GetTestString());
		return false;
	}

	if (pMagicOpArg->GetString(0) == "")
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ��������1���벻�ܿ�", pMagicOpArg->GetString());
		return false;
	}

	if (pMagicOpArg->GetString(1) == "")
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ��������2���벻�ܿ�", pMagicOpArg->GetString());
		return false;
	}

	if (!CCfgSkillCheck::IsPlayerSkillNameValid(pMagicOpArg->GetString(0).c_str()))
	{
		stringstream str;
		CfgChk::GenExpInfo("���ܲ�����", pMagicOpArg->GetTestString());
		return false;
	}
	return true;
}

bool CDoTongSkillMOP::CheckArg2MOP(CCfgCalc* pMagicOpArg)
{
	if (pMagicOpArg->GetStringCount() != 2)
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ������������������Ӧ����2��", pMagicOpArg->GetTestString());
		return false;
	}

	if (pMagicOpArg->GetString(0) == "")
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ��������1���벻�ܿ�", pMagicOpArg->GetString());
		return false;
	}

	if (pMagicOpArg->GetString(1) == "")
	{
		CfgChk::GenExpInfo("Υ��ħ������Լ��������2���벻�ܿ�", pMagicOpArg->GetString());
		return false;
	}

	if (!CCfgSkillCheck::IsPlayerSkillNameValid(pMagicOpArg->GetString(0).c_str()))
	{
		stringstream str;
		CfgChk::GenExpInfo("���ܲ�����", pMagicOpArg->GetTestString());
		return false;
	}
	return true;
}
EValueMagicOpType CCureMop::GetValueOpType() const
{
	return eVMOT_None;
}

EValueMagicOpType CCalculateMainHandPhysicsHurtMop::GetValueOpType() const
{
	return eVMOT_Physical;
}

EValueMagicOpType CCalculateAssistantPhyicsHurtMop::GetValueOpType() const
{
	return eVMOT_Physical;
}

EValueMagicOpType CCalculateMainHandPhysicsStrikeMop::GetValueOpType() const
{
	return eVMOT_Physical;
}

EValueMagicOpType CCalculateMainHandPhysicsNoStrikeHurtMop::GetValueOpType() const
{
	return eVMOT_Physical;
}

EValueMagicOpType CCalculateMainHandPhysicsOnlyHitAndStrikeHurtMop::GetValueOpType() const
{
	return eVMOT_Physical;
}

EValueMagicOpType CCalNatureMagHurtMop::GetValueOpType() const
{
	return eVMOT_Magic;
}

EValueMagicOpType CCalDestructionMagHurtMop::GetValueOpType() const
{
	return eVMOT_Magic;
}

EValueMagicOpType CCalEvilMagHurtMop::GetValueOpType() const
{
	return eVMOT_Magic;
}

EValueMagicOpType CCalNatureMagNoStrikeHurtMop::GetValueOpType() const
{
	return eVMOT_Magic;
}

EValueMagicOpType CCalDestructionMagNoStrikeHurtMop::GetValueOpType() const
{
	return eVMOT_Magic;
}

EValueMagicOpType CCalDestructionMagNoResistHurtMop::GetValueOpType() const
{
	return eVMOT_Magic;
}

EValueMagicOpType CCalEvilMagNoStrikeHurtMop::GetValueOpType() const
{
	return eVMOT_Magic;
}

EValueMagicOpType CCalDOTHurtMop::GetValueOpType() const
{
	return eVMOT_None;
}

EValueMagicOpType CCalDOTNoStrikeHurtMop::GetValueOpType() const
{
	return eVMOT_None;
}
