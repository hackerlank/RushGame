#include "stdafx.h"
#include "CCfgAllStateCheck.h"
#include "CTxtTableFile.h"
#include "CCfgColChecker.inl"
#include "LoadSkillCommon.h"
#include "CCfgMagicEffCheck.h"

CCfgMagicStateCheck::SetRowName CCfgMagicStateCheck::ms_setRowName;
CCfgAllStateCheck::MapState CCfgAllStateCheck::ms_mapState;

bool CCfgMagicStateCheck::Check(const TCHAR* cfgFile)
{
	using namespace CfgChk;

	CTxtTableFile TxtTableFile;
	SetTabFile(TxtTableFile, "ħ��״̬");
	if (!TxtTableFile.Load(PATH_ALIAS_CFG.c_str(), cfgFile))
	{
		stringstream ExpStr;
		ExpStr << "���ñ� ����ʧ�ܣ���鿴�ļ���[" << cfgFile << "]�Ƿ���ȷ�����ļ��Ƿ����";
		GenExpInfo(ExpStr.str());
	}

	static const uint32 s_uTableFileWide = 16;
	if (TxtTableFile.GetWidth() != s_uTableFileWide)
	{
		stringstream ExpStr;
		ExpStr << "���ñ� �������� ӦΪ: " << s_uTableFileWide << " �У�ʵΪ: " << TxtTableFile.GetWidth() << " ��";
		GenExpInfo(ExpStr.str());
	}

	for(int32 i = 1; i < TxtTableFile.GetHeight(); ++i)
	{
		SetLineNo(i);
		string strName,	strCancelEff, strDotEff, strFinalEff, strCascadeType, strCascadeMax;
		bool bPersistent;
		ReadItem(strName,			szMagicState_Name,				CANEMPTY);
		ReadItem(bPersistent,		szState_Persistent,				CANEMPTY,	NO);
		ReadItem(strCancelEff,		szMagicState_CancelableMagicEff,CANEMPTY);
		ReadItem(strDotEff,			szMagicState_DotMagicEff,		CANEMPTY);
		ReadItem(strFinalEff,		szMagicState_FinalMagicEff,		CANEMPTY);
		ReadItem(strCascadeType,	szMagicState_CascadeType,		CANEMPTY);
		ReadItem(strCascadeMax,		szMagicState_CascadeMax,		CANEMPTY);
		trimend(strName);


		//2
		if (!strDotEff.empty())
		{
			size_t comma = strDotEff.find(',');
			SetItemTitle(szMagicState_DotMagicEff);
			if(comma != string::npos)
			{
				string strTemp = strDotEff;
				strDotEff = strDotEff.substr(0, comma);
				CCfgMagicEffCheck::CheckMagicEffExist(strDotEff);

				float fDotInterval = 1.0f;
				SetValue(fDotInterval, strTemp.substr(comma + 1));
				if(fDotInterval <= 0.0f)
				{
					GenExpInfo("Υ����Լ�������Ч��ʱ�����Ϊ����\n");
				}
				else if(fDotInterval / 1.0f != float(int32(fDotInterval / 1.0f)) && fDotInterval != 0.5f)
				{
					GenExpInfo("Υ����Լ�������Ч��ʱ�����Ϊ������0.5\n");
				}
			}
			else
			{
				CCfgMagicEffCheck::CheckMagicEffExist(strDotEff);
			}
		}

		//3
		if( (!strCancelEff.empty() && !strDotEff.empty() && strCancelEff == strDotEff) ||
			(!strFinalEff.empty() && !strDotEff.empty() && strFinalEff == strDotEff) ||
			(!strCancelEff.empty() && !strFinalEff.empty() && strCancelEff == strFinalEff) )
		{
			stringstream str;
			str << "\n��" << i << "��Υ����Լ����ͬһ�еĲ�ͬħ��Ч������ͬ��";
			GenExpInfo(str.str());
		}

		//4
		string strModel;
		ReadItem(strModel,			szTplState_Model,			CANEMPTY);
		if(!strModel.empty() && "Ψһ" != strCascadeType)
		{
			SetItemTitle(szMagicState_CascadeType);
			GenExpInfo("����ģ�͵�ħ��״̬�ﲻ��ΪΨһ�����ȡֵ", strCascadeType);
		}

		//5
		stringstream ExpStr;
		CCfgCalc* calcFX= new CCfgCalc;
		CCfgCalc* calcCascadeMax = new CCfgCalc;
		ReadMixedItem(calcFX,		szTplState_FXID,			CANEMPTY,	"");
		calcFX->SetStringSplitter(';');
		ReadItem(calcCascadeMax,	szMagicState_CascadeMax,		GE,			-1);
		CCfgCalc* calcTime = new CCfgCalc;
		ReadItem(calcTime,				szMagicState_Time,				GE,			-1);
		if(calcFX->GetStringCount() > 1)
		{
			if(!calcCascadeMax->IsSingleNumber())
			{
				ExpStr << "�ֲ���Ч�ĵ������޲���[" << calcCascadeMax->GetTestString() << "�����Ǳ��ʽ";
				GenExpInfo(ExpStr.str(), calcFX->GetTestString());
			}
			else if((uint32)calcCascadeMax->GetIntValue() != (uint32)calcFX->GetStringCount())
			{
				ExpStr << "�ֲ���Ч���ַ�����[" << calcFX->GetStringCount() << "]��������޲���[" << calcCascadeMax->GetIntValue() << "]�����";
				GenExpInfo(ExpStr.str(), calcFX->GetTestString());
			}
		}

		//6
		string strReplaceType;
		ReadItem(strReplaceType,	szMagicState_ReplaceRuler,	CANEMPTY);
		SetItemTitle(szMagicState_ReplaceRuler);
		if(strReplaceType == "��" && calcCascadeMax->IsSingleNumber() && calcCascadeMax->GetIntValue() > 1)
		{
			ExpStr.clear();
			ExpStr << "���Ӳ�����Ϊ1��ħ��״̬�滻������Ϊ��\n";
			GenExpInfo(ExpStr.str(), strReplaceType);
		}

		delete calcFX;
		delete calcCascadeMax;
		delete calcTime;

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
			pState->m_eType = eBST_State;
			pState->m_bPersistent = bPersistent;
			pState->m_strCascadeType = strCascadeType;
			pState->m_strCascadeMax = strCascadeMax;
			if (!strCancelEff.empty() && CCfgMagicEffCheck::CheckMagicEffExist(strCancelEff))
			{
				CCfgMagicEffCheck::MapMagicEff::iterator itr = CCfgMagicEffCheck::ms_mapMagicEff.find(strCancelEff);
				pState->m_pCancelEff = (*itr).second;
			}
			if (!strDotEff.empty() && CCfgMagicEffCheck::CheckMagicEffExist(strDotEff))
			{
				CCfgMagicEffCheck::MapMagicEff::iterator itr = CCfgMagicEffCheck::ms_mapMagicEff.find(strDotEff);
				pState->m_pDotEff = (*itr).second;
			}
			if (!strFinalEff.empty() && CCfgMagicEffCheck::CheckMagicEffExist(strFinalEff))
			{
				CCfgMagicEffCheck::MapMagicEff::iterator itr = CCfgMagicEffCheck::ms_mapMagicEff.find(strFinalEff);
				pState->m_pFinalEff = (*itr).second;
			}
			CCfgAllStateCheck::ms_mapState.insert(make_pair(strName, pState));
		}

		// ��Ч�����
		CCfgCalc* pCascadeMax = NULL;
		ReadItem(pCascadeMax, szMagicState_CascadeMax);
		pCascadeMax->SetTypeExpression();
		int32 nCascadeMax = pCascadeMax->GetIntValue();
		delete pCascadeMax;

		string strFX = TxtTableFile.GetString(i, szTplState_FXID);
		if (strFX != "")
		{
			vector<string> sMultilayerFX = CCfgMagicOp::Split(strFX, ";");
			if (sMultilayerFX.size() == 1)
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
			else
			{
				for (int i = 0; i < nCascadeMax; ++i)
				{
					if (sMultilayerFX[i] != "")
					{
						vector<string> sFXTable = CCfgMagicOp::Split(sMultilayerFX[i], ",");
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
			}
		}
	}
	return true;
}

void CCfgMagicStateCheck::EndCheck()
{
	ms_setRowName.clear();
}

bool CCfgMagicStateCheck::CheckExist(const string& strName)
{
	SetRowName::iterator iter = ms_setRowName.find(strName);
	if (iter == ms_setRowName.end())
	{
		stringstream ExpStr;
		ExpStr << " Υ��ħ������Լ��: ħ��״̬" << " [" << strName << "] ������ ";
		CfgChk::GenExpInfo(ExpStr.str());
		return false;
	}
	return true;
}
