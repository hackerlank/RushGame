#include "stdafx.h"
#include "CTxtTableFile.h"
#include "CPropertyCfg.h"
#include "CCfgBloodCheck.h"
#include "CCfgColChecker.inl"

bool CCfgBloodCheck::Check(const TCHAR* cfgFile)
{
	CTxtTableFile TableFile;
	Load(TableFile, cfgFile, "BloodType_Client");
	for(int32 i = 1; i < TableFile.GetHeight(); ++i)
	{
		CheckOverlap("ְҵ", i);

		string strClassName = TableFile.GetString(i, "ְҵ");
		CPropertyCfg::MapClassType::iterator it = CPropertyCfg::ms_mapClassType.find(strClassName);
		if (it == CPropertyCfg::ms_mapClassType.end())
		{
			ostringstream ExpStr;
			ExpStr << "��BloodType_Client�����ñ��" << i << "�е�" << strClassName << "����ְҵ���ƣ�";
			CfgChk::GenExpInfo(ExpStr.str());
		}
	}
	return true;
}

void CCfgBloodCheck::EndCheck()
{
	EndCheckOverlap();
}
