#include "stdafx.h"
#include "CCfgAllMagicCheck.h"
#include "CTxtTableFile.h"
#include "CCfgColChecker.inl"
#include "LoadSkillCommon.h"

CCfgBarrierMagicCheck::SetRowName CCfgBarrierMagicCheck::ms_setRowName;

bool CCfgBarrierMagicCheck::Check(const TCHAR* cfgFile)
{
	using namespace CfgChk;

	CTxtTableFile TableFile;
	SetTabFile(TableFile, "�ϰ�ħ��");
	if (!TableFile.Load(PATH_ALIAS_CFG.c_str(), cfgFile))
	{
		stringstream ExpStr;
		ExpStr << "���ñ� ����ʧ�ܣ���鿴�ļ���[" << cfgFile << "]�Ƿ���ȷ�����ļ��Ƿ����";
		GenExpInfo(ExpStr.str());
	}

	static const uint32 s_uTableFileWide = 7;
	if (TableFile.GetWidth() != s_uTableFileWide)
	{
		stringstream ExpStr;
		ExpStr << "���ñ� �������� ӦΪ: " << s_uTableFileWide << " �У�ʵΪ: " << TableFile.GetWidth() << " ��";
		GenExpInfo(ExpStr.str());
	}

	for(int32 i = 1; i < TableFile.GetHeight(); ++i)
	{
		SetLineNo(i);
		string strName;
		ReadItem(strName, szMagic_Name, CANEMPTY);
		trimend(strName);
		SetRowName::iterator iter = ms_setRowName.find(strName);
		if (iter != ms_setRowName.end())
		{
			stringstream ExpStr;
			ExpStr << " ���ñ�: " << g_sTabName << " �� " << i << " �е� " << szMagic_Name << "["<< strName << "]" << "�ظ�";
			GenExpInfo(ExpStr.str());
		}
		else
		{
			ms_setRowName.insert(strName);
		}

		// ��Ч�����
		string strFX = TableFile.GetString(i, szMagic_FxName);
		if (strFX != "")
		{
			vector<string> sFXTable = CCfgMagicOp::Split(strFX, ",");
			if (sFXTable.size() == 2)
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

void CCfgBarrierMagicCheck::EndCheck()
{
	ms_setRowName.clear();
}

bool CCfgBarrierMagicCheck::CheckExist(const string& strName)
{
	SetRowName::iterator iter = ms_setRowName.find(strName);
	if (iter == ms_setRowName.end())
	{
		stringstream ExpStr;
		ExpStr << " Υ��ħ������Լ��: �ϰ�ħ��" << " [" << strName << "] ������ ";
		CfgChk::GenExpInfo(ExpStr.str());
		return false;
	}
	return true;
}
