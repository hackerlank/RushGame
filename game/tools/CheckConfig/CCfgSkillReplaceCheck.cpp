#include "stdafx.h"
#include "CCfgSkillReplaceCheck.h"
#include "CTxtTableFile.h"
#include "LoadSkillCommon.h"
#include "CCfgColChecker.inl"
#include "CCfgAllStateCheck.h"
#include "CCfgSkillCheck.h"

CCfgSkillReplaceCheck::MultiMapCSkillReplace CCfgSkillReplaceCheck::ms_multimapSkillReplace;

bool CCfgSkillReplaceCheck::Check(const TCHAR* cfgFile)
{
	using namespace CfgChk;

	CTxtTableFile TableFile;
	SetTabFile(TableFile, "�滻����");
	if (!TableFile.Load(PATH_ALIAS_CFG.c_str(), cfgFile))
	{
		stringstream ExpStr;
		ExpStr << "���ñ� ����ʧ�ܣ���鿴�ļ���[" << cfgFile << "]�Ƿ���ȷ�����ļ��Ƿ����";
		GenExpInfo(ExpStr.str());
	}

	static const uint32 s_uTableFileWide = 3;
	if (TableFile.GetWidth() != s_uTableFileWide)
	{
		stringstream ExpStr;
		ExpStr << "���ñ� �������� ӦΪ: " << s_uTableFileWide << " �У�ʵΪ: " << TableFile.GetWidth() << " ��";
		GenExpInfo(ExpStr.str());
	}

	for(int32 i = 1; i < TableFile.GetHeight(); ++i)
	{
		SetLineNo(i);
		CCfgSkillReplaceCheck* pSkillReplace = new CCfgSkillReplaceCheck;
		ReadItem(pSkillReplace->m_sSkillName,			szSkillReplace_SkillName);
		ReadItem(pSkillReplace->m_sStateName,			szSkillReplace_StateName);
		ReadItem(pSkillReplace->m_sReplaceSkillName,	szSkillReplace_ReplaceName);

		//���
		if(!CCfgMagicStateCheck::CheckExist(pSkillReplace->m_sStateName))
		{
			stringstream ExpStr;
			ExpStr << "���ñ�[�����滻]��" << (i + 1) << "�еĵ�����״̬" << pSkillReplace->m_sStateName.c_str() << "������";
			GenExpInfo(ExpStr.str());
		}
		if (!CCfgSkillCheck::IsPlayerSkillNameValid(pSkillReplace->m_sSkillName.c_str()))
		{
			stringstream ExpStr;
			ExpStr << "���ñ�[�����滻]��" << (i + 1) << "�еĵļ���" << pSkillReplace->m_sSkillName.c_str() << "������";
			GenExpInfo(ExpStr.str());
		}
		if (!CCfgSkillCheck::IsPlayerSkillNameValid(pSkillReplace->m_sReplaceSkillName.c_str()))
		{
			stringstream ExpStr;
			ExpStr << "���ñ�[�����滻]��" << (i + 1) << "�е��滻����" << pSkillReplace->m_sReplaceSkillName.c_str() << "������";
			GenExpInfo(ExpStr.str());
		}

		//�滻�����в������ظ���
		MultiMapCSkillReplace::iterator it = ms_multimapSkillReplace.begin();
		MultiMapCSkillReplace::iterator itEnd = ms_multimapSkillReplace.end();
		for (;it!=itEnd;it++)
		{
			if (strcmp((*it).second->GetStateName().c_str(), pSkillReplace->m_sStateName.c_str())==0)
			{
				stringstream ExpStr;
				ExpStr << "���ñ�[�����滻]��" << (i + 1) << "�еĵ�����״̬" << pSkillReplace->m_sStateName.c_str() << "�ظ�";
				GenExpInfo(ExpStr.str());
			}
			if (strcmp((*it).second->GetReplaceSkillName().c_str(), pSkillReplace->m_sReplaceSkillName.c_str())==0)
			{
				stringstream ExpStr;
				ExpStr << "���ñ�[�����滻]��" << (i + 1) << "�е��滻����" << pSkillReplace->m_sReplaceSkillName.c_str() << "�ظ�";
				GenExpInfo(ExpStr.str());
			}
		}
		ms_multimapSkillReplace.insert(pair<string, CCfgSkillReplaceCheck*>(pSkillReplace->m_sSkillName,pSkillReplace));
	}
	return true;
}

void CCfgSkillReplaceCheck::EndCheck()
{
	ClearMap(ms_multimapSkillReplace);
}
