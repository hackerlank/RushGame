#include "stdafx.h"
#include "CCfgAllStateCheck.h"
#include "CTxtTableFile.h"
#include "CCfgColChecker.inl"
#include "LoadSkillCommon.h"

CCfgTriggerStateCheck::SetRowName CCfgTriggerStateCheck::ms_setRowName;

bool CCfgTriggerStateCheck::Check(const TCHAR* cfgFile)
{
	using namespace CfgChk;

	CTxtTableFile TxtTableFile;
	SetTabFile(TxtTableFile, "������״̬");
	if (!TxtTableFile.Load(PATH_ALIAS_CFG.c_str(), cfgFile))
	{
		stringstream ExpStr;
		ExpStr << "���ñ� ����ʧ�ܣ���鿴�ļ���[" << cfgFile << "]�Ƿ���ȷ�����ļ��Ƿ����";
		GenExpInfo(ExpStr.str());
	}

	static const uint32 s_uTableFileWide = 18;
	if (TxtTableFile.GetWidth() != s_uTableFileWide)
	{
		stringstream ExpStr;
		ExpStr << "���ñ� �������� ӦΪ: " << s_uTableFileWide << " �У�ʵΪ: " << TxtTableFile.GetWidth() << " ��";
		GenExpInfo(ExpStr.str());
	}

	for(int32 i = 1; i < TxtTableFile.GetHeight(); ++i)
	{
		SetLineNo(i);
		string strName,	strCancelEff, strTriggerEvent;
		bool bPersistent;
		CCfgCalc* pCalcTime = new CCfgCalc;
		ReadItem(strName,		szTriggerState_Name,		CANEMPTY);
		ReadItem(bPersistent,	szState_Persistent,			CANEMPTY,	NO);
		ReadItem(pCalcTime,		szDamageChangeState_Time,			GE,			-1);
		ReadItem(strTriggerEvent, szDamageChangeState_TriggerEvent, CANEMPTY);
		ReadItem(strCancelEff,	szTriggerState_CancelableMagicEff,	CANEMPTY);
		CCfgMagicEffCheck::CheckMagicEffExist(strCancelEff);
		trimend(strName);
		SetRowName::iterator iter = ms_setRowName.find(strName);
		if (iter != ms_setRowName.end())
		{
			stringstream ExpStr;
			ExpStr << " ���ñ�: " << g_sTabName << " �� " << i << " �е� " << szMagicState_Name << "["<< strName << "]" << "�ظ�";
			GenExpInfo(ExpStr.str());
		}
		else
		{
			ms_setRowName.insert(strName);
			CCfgAllStateCheck* pState = new CCfgAllStateCheck;
			pState->m_strName = strName;
			pState->m_eType = eBST_Damage;
			pState->m_bPersistent = bPersistent;
			pState->m_bNeedSaveToDB =
				pCalcTime->IsSingleNumber() && pCalcTime->GetDblValue() > 10
				&& (strTriggerEvent != "����װ��ɱ��" && strTriggerEvent != "��װ������") ? true : false;
			if (!strCancelEff.empty() && CCfgMagicEffCheck::CheckMagicEffExist(strCancelEff))
			{
				CCfgMagicEffCheck::MapMagicEff::iterator itr = CCfgMagicEffCheck::ms_mapMagicEff.find(strCancelEff);
				pState->m_pCancelEff = (*itr).second;
			}
			CCfgAllStateCheck::ms_mapState.insert(make_pair(strName, pState));
		}
		delete pCalcTime;

		CCfgCalc* pCalc = NULL;
		ReadMixedItem(pCalc, szTplState_Scale, CANEMPTY, "");
		if (!pCalc->GetTestString().empty() && pCalc->GetValueCount() > 2)
		{
			GenExpInfo("���ʽ��������2", pCalc->GetTestString());
		}
		delete pCalc;

		string strSkillType, strAttackType;
		ReadItem(strSkillType, szTriggerState_SkillType, CANEMPTY);
		ReadItem(strAttackType, szTriggerState_AttackType, CANEMPTY);
		if(strSkillType.empty() && !strAttackType.empty())
		{
			GenExpInfo("Υ����Լ������������Ϊ��ʱ�����������", strAttackType);
		}

		string strTriggerEff;
		ReadItem(strTriggerEff, szTriggerState_TriggerEff,			CANEMPTY);
		CCfgMagicEffCheck::CheckMagicEffExist(strTriggerEff);
		if(!strTriggerEff.empty() && !strCancelEff.empty() && strTriggerEff == strCancelEff)
		{
			stringstream str;
			str << "\n��" << i << "��Υ����Լ����ͬһ�еĲ�ͬħ��Ч������ͬ��";
			GenExpInfo(str.str());
		}

		string strTargetChange;
		ReadItem(strTargetChange,	szTriggerState_ChangeTrigger,	CANEMPTY);
		if (strTargetChange.empty())		// �����Ĭ��Ϊӵ����
			strTargetChange = "ӵ����";
		if(strTriggerEff.empty() && !strCancelEff.empty() && strTargetChange != "ӵ����")
		{
			GenExpInfo("Υ����Լ������ֻ�пɳ���Ч���Ĵ�����״̬�ﲻ����ӵ���������ֵ", strTargetChange);
		}

		// ��Ч�����
		string strFX = TxtTableFile.GetString(i, szTplState_FXID);
		if (strFX != "")
		{
			vector<string> sFXTable = CCfgMagicOp::Split(strFX, ",");
			if (sFXTable.size() == 2 || sFXTable.size() == 3)
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

void CCfgTriggerStateCheck::EndCheck()
{
	ms_setRowName.clear();
}

bool CCfgTriggerStateCheck::CheckExist(const string& strName)
{
	SetRowName::iterator iter = ms_setRowName.find(strName);
	if (iter == ms_setRowName.end())
	{
		stringstream ExpStr;
		ExpStr << " Υ��ħ������Լ��: ������״̬" << " [" << strName << "] ������ ";
		CfgChk::GenExpInfo(ExpStr.str());
		return false;
	}
	return true;
}
