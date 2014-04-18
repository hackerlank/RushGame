#include "stdafx.h"
#include "CCfgAllStateCheck.h"
#include "CCfgColChecker.inl"
#include "CTxtTableFile.h"
#include "LoadSkillCommon.h"

CCfgSpecialStateCheck::SetRowName CCfgSpecialStateCheck::ms_setRowName;

bool CCfgSpecialStateCheck::Check(const TCHAR* cfgFile)
{
	using namespace CfgChk;

	CTxtTableFile TxtTableFile;
	SetTabFile(TxtTableFile, "����״̬");
	if (!TxtTableFile.Load(PATH_ALIAS_CFG.c_str(), cfgFile))
	{
		stringstream ExpStr;
		ExpStr << "���ñ� ����ʧ�ܣ���鿴�ļ���[" << cfgFile << "]�Ƿ���ȷ�����ļ��Ƿ����";
		GenExpInfo(ExpStr.str());
	}

	static const uint32 s_uTableFileWide = 10;
	if (TxtTableFile.GetWidth() != s_uTableFileWide)
	{
		stringstream ExpStr;
		ExpStr << "���ñ� �������� ӦΪ: " << s_uTableFileWide << " �У�ʵΪ: " << TxtTableFile.GetWidth() << " ��";
		GenExpInfo(ExpStr.str());
	}

	for(int32 i = 1; i < TxtTableFile.GetHeight(); ++i)
	{
		SetLineNo(i);
		string strName;
		bool bPersistent;
		ReadItem(strName, szSpecialState_Name, CANEMPTY);
		ReadItem(bPersistent, szState_Persistent, CANEMPTY, NO);
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
			pState->m_eType = eBST_Special;
			pState->m_bPersistent = bPersistent;
			CCfgAllStateCheck::ms_mapState.insert(make_pair(strName, pState));
		}

		CCfgCalc* pCalc = NULL;
		ReadMixedItem(pCalc, szTplState_Scale, CANEMPTY, "");
		if (!pCalc->GetTestString().empty() && pCalc->GetValueCount() > 2)
		{
			GenExpInfo("���ʽ��������2", pCalc->GetTestString());
		}
		delete pCalc;

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

void CCfgSpecialStateCheck::EndCheck()
{
	ms_setRowName.clear();
}

bool CCfgSpecialStateCheck::CheckExist(const string& strName)
{
	SetRowName::iterator iter = ms_setRowName.find(strName);
	if (iter == ms_setRowName.end())
	{
		stringstream ExpStr;
		ExpStr << " Υ��ħ������Լ��: ����״̬" << " [" << strName << "] ������ ";
		CfgChk::GenExpInfo(ExpStr.str());
		return false;
	}
	return true;
}
