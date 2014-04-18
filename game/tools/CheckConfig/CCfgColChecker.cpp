#include "stdafx.h"
#include "CCfgColChecker.inl"
#include "CAppServer.h"
#include "TimeHelper.h"
#include "ErrLogHelper.h"
#include "StringHelper.h"

namespace CfgChk
{

	bool g_bIsCheckingCfg = false;
	CTxtTableFile* g_pTabFile = NULL;
	string g_sTabName = "";
	string g_sSrcTabLineName = "";
	string g_sMagicEffName = "";
	int32 g_iLine = 0;					//�к�
	string g_sTitle = "";				//�б���
	EErrorReactType g_uErrReact = eERT_AtEnd;			//����Ӧ 0:�����쳣 1:�����쳣 2:���쳣
	bool g_bExistError = false;
	uint64 g_uTimeForTest = 0;
	bool g_bCheckInReading = true;		//Ĭ�϶���ʱ�����

	bool	GT(const float x, const float a) {return x > a;}
	bool	GE(const float x, const float a) {return x >= a;}
	bool	LT(const float x, const float a) {return x < a;}
	bool	LE(const float x, const float a) {return x <= a;}
	bool	EQ(const float x, const float a) {return x == a;}
	bool	NE(const float x, const float a) {return x != a;}
	bool	IsRate(const float x, const float a) {return x >= 0.0f && x <= 1.0f;}

	bool	ExistError() {return g_bExistError;}
	bool	ExpIsForbidden() {return g_uErrReact == eERT_Never;}
	bool	ExpAtOnce() {return g_uErrReact == eERT_AtOnce;}
	bool	ExpAtEnd() {return g_uErrReact == eERT_AtEnd;}

void GenExpInfo(const string& sExp)
{
	g_bExistError = true;
	if(g_bIsCheckingCfg)
	{
		cout << sExp << "\n";
		if(ExpAtOnce())
			CfgLogErr(sExp.c_str());
	}
	else
	{
		CfgLogErr(sExp.c_str());
	}
}

void SetTabFile(CTxtTableFile& TabFile, const string& TabName) 
{
	uint64 uCurTime = GetProcessTime();
	cout << "\t��ʱ" << uCurTime - g_uTimeForTest << "����\n[" << uCurTime << "]��ȡ����⼼�����ñ�[" << TabName << "]��ʼ";
	g_uTimeForTest = uCurTime;
	g_bIsCheckingCfg = true;
	g_pTabFile = &TabFile;
	g_sTabName = TabName;
	g_sSrcTabLineName.clear();
	g_sMagicEffName.clear();
	g_iLine = 0;
	//g_bCheckingInReading = true;
	g_sTitle.clear();

}

void ResetTab(CTxtTableFile& TabFile, const string& TabName)
{
	g_bIsCheckingCfg = true;
	g_pTabFile = &TabFile;
	g_sTabName.clear();
	g_sSrcTabLineName.clear();
	g_sMagicEffName.clear();
	g_sTabName = TabName;
	g_iLine = 0;
	//g_bCheckingInReading = true;
	g_sTitle.clear();
}

void SetTabName(const string& TabName)
{
	uint64 uCurTime = GetProcessTime();
	cout << "\t��ʱ" << uCurTime - g_uTimeForTest << "����\n[" << uCurTime << "]��ϵ��⼼�����ñ�[" << TabName << "]��ʼ";
	g_uTimeForTest = uCurTime;
	g_bIsCheckingCfg = true;
	g_pTabFile = NULL;
	g_sSrcTabLineName.clear();
	g_sMagicEffName.clear();
	g_sTabName = TabName;
	g_iLine = 0;
	//g_bCheckingInReading = false;
	g_sTitle.clear();

}

void SetTabNull()
{
	g_bIsCheckingCfg = false;
	g_pTabFile = NULL;
	g_sSrcTabLineName.clear();
	g_sMagicEffName.clear();
	g_sTabName.clear();
	g_iLine = 0;
	g_sTitle.clear();
}

void SetLineNo(int32 iLine)
{
	g_iLine = iLine;
}

void SetItemTitle(const TCHAR* sTitle)
{
	g_sTitle = sTitle;
}



//SetValue()
void SetValue(int32& tReader, const string& sValue)
{
	stringstream sTempValue;
	tReader = atoi(sValue.c_str());
	sTempValue << tReader;
	if(sTempValue.str() != sValue)
	{
		GenExpInfo("Υ����Լ���������в���Ϊ�����������ֵ", sValue);
		//return false;
	}
}

void SetValue(float& tReader, const string& sValue)
{
	//stringstream sTempValue;
	tReader = float(atof(sValue.c_str()));
	//sTempValue << tReader;

	if(g_bCheckInReading)
	{
		if(tReader == 0.0f)
		{
			if(/*sTempValue.str()*/ "0" != sValue)
			{
				GenExpInfo("Υ����Լ�����������в���Ϊ����ֵ", sValue);
				//return false;
			}
		}
	}
}

void SetValue(bool& tReader, const string& sValue)
{
	if(sValue == "��" || sValue == "��")
	{
		tReader = true;
	}
	else if(sValue == "��" || sValue == "��")
	{
		tReader = false;
	}
	else
	{
		GenExpInfo("Υ����Լ�����������в���ȡ�Ǻͷ������ֵ", sValue);
	}
}

void SetValue(string& tReader, const string& sValue)
{
	tReader = sValue;
}

void SetValue(CCfgCalc*& pCalc, const string& sValue, bool bIsSingle)
{
	//���￼��Ҫ��Ҫ�ж�NULL�������������Ҫ�ڸ����ñ��캯�����CCfgCalc*��Ա��ΪNULL
	pCalc = new CCfgCalc;
	pCalc->InputString(sValue);
	if(bIsSingle) pCalc->SetTypeExpression();
}

//CheckType()
void CheckUIntType(int32& tReader)
{
	if(g_bCheckInReading)
	{
		if(tReader < 0)
		{
			GenExpInfo("Υ����Լ�����Ǹ������в���ȡ����", tReader);
			//return false;
		}
	}
}

//CheckField()
void CheckField(string& sValue, FIELD_CHECKER_STRING pFunFieldChecker)
{
	if(g_bCheckInReading)
	{
		if(pFunFieldChecker)
		{
			if(!pFunFieldChecker(sValue))
			{
				GenExpInfo("Υ����Լ�����ַ�������ȡֵ��Χ", sValue);
			}
		}
	}
}

//ReadItem()
void ReadItem(uint32& tReader, const TCHAR* sTitle,
			  FIELD_CHECKER pFunFieldChecker, uint32 fFieldCheckerParam)
{
	ReadItem(tReader, sTitle, false, 0, pFunFieldChecker, fFieldCheckerParam);
}

void ReadItem(uint32& tReader, const TCHAR* sTitle, bool bCanEmpty, 
							  uint32 tDefault, FIELD_CHECKER pFunFieldChecker, uint32 fFieldCheckerParam)
{
	SetItemTitle(sTitle);
	string sValue = g_pTabFile->GetString(g_iLine, sTitle);
	CheckEmpty(tReader, sValue, bCanEmpty, tDefault);
	if(!sValue.empty())
	{
		int32 tTempReader;
		SetValue(tTempReader, sValue);
		CheckUIntType(tTempReader);
		tReader = tTempReader;
	}
	CheckField(tReader, pFunFieldChecker, fFieldCheckerParam);
}

void ReadItem(string& tReader, const TCHAR* sTitle, 
			  FIELD_CHECKER_STRING pFunFieldChecker)
{
	SetItemTitle(sTitle);
	ReadItem(tReader, sTitle, false, "", pFunFieldChecker);
}

void ReadItem(string& tReader, const TCHAR* sTitle, bool bCanEmpty,
			  const string& tDefault, FIELD_CHECKER_STRING pFunFieldChecker)
{
	SetItemTitle(sTitle);
	string sValue = g_pTabFile->GetString(g_iLine, sTitle);
	size_t l = sValue.size();
	if(l > 1 && sValue[0] == '"' && sValue[l - 1] == '"') sValue = sValue.substr(1, l - 2);
	trimend(sValue);
	CheckEmpty(tReader, sValue, bCanEmpty, tDefault);
	if(!sValue.empty())
	{
		SetValue(tReader, sValue);
	}
	CheckField(tReader, pFunFieldChecker);
}

//typedef CCfgCalc* CalcType;
void ReadItem(CCfgCalc*& tReader, const TCHAR* sTitle, FIELD_CHECKER pFunFieldChecker,
			  float fFieldCheckerParam)
{
	ReadItem(tReader, sTitle, false, 0.0f, pFunFieldChecker, fFieldCheckerParam);
}

void ReadItem(CCfgCalc*& tReader, const TCHAR* sTitle, bool bCanEmpty, 
			  float tDefault, FIELD_CHECKER pFunFieldChecker, float fFieldCheckerParam)
{
	SetItemTitle(sTitle);
	string sValue = g_pTabFile->GetString(g_iLine, sTitle);
	trimend(sValue);
	CheckEmpty(tReader, sValue, bCanEmpty, tDefault);
	if(!sValue.empty())
	{
		SetValue(tReader, sValue);
	}
	if(pFunFieldChecker && tReader->IsSingleNumber())
	{
		float fTemp = float(tReader->GetDblValue());
		CheckField(fTemp, pFunFieldChecker, fFieldCheckerParam);
	}
}

void ReadMixedItem(CCfgCalc*& tReader, const TCHAR* sTitle, bool bCanEmpty, 
			  const string& tDefault)
{
	SetItemTitle(sTitle);
	string sValue = g_pTabFile->GetString(g_iLine, sTitle);
	trimend(sValue);
	CheckEmpty(tReader, sValue, bCanEmpty, tDefault);
	if(!sValue.empty())
	{
		SetValue(tReader, sValue, false);
	}
}


}//end namespace spaceCfgColChecke

