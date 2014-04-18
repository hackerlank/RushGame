#include "stdafx.h"

#include "ScriptX.h"
#include "ExpHelper.h"
#include "RegistStrShell.h"
#include "CScriptAppStress.h"
#include "CPathMgr.h"
#include "Module.h"
#include "CodeCvs.h"


using namespace sqr;

//static void PrintUsage();

static int GetNumFromArgs(int nArgCount,wchar_t* aryArg[],const wchar_t* szOption);

static void RegistStressNum(CScript& Script);

static int StressNum;
#ifdef _WIN32
static int StressMaxNum = MAXIMUM_WAIT_OBJECTS - 2;
#else
static int StressMaxNum = 62;
#endif

SQRENTRY int SqrMain(int argc, wchar_t* argv[])
{
	//�������ǵ������и�ʽΪ ���� -n Num ��ֻ��3�����������Բ������ԾͲ�������
	//if( argc != 3 )
	//{
	//	PrintUsage();
	//	return 0;
	//}

	StressNum = GetNumFromArgs(argc, argv, L"-n");


	if(StressNum == 0)
	{
		cerr << "ѹ������������Ŀ����Ϊ0" << endl;
		return 0;
	}

	SQR_TRY
	{
		CScriptAppStress Runner("etc/str/StrConfig.xml");
	
		CScript* pScript = Runner.GetMainVM();

		RegistStressShell(*pScript);
		RegistStressNum(*pScript);

		string str_path = pScript->GetLoadPath("str");
		string str_main = str_path + string("main_frame/StrMain");

		string sError=Runner.Run(str_main.c_str());

		if( !sError.empty() )
			GenErr(sError);
	}
	SQR_CATCH(exp)
	{
		clog << exp.ErrorTitle() << endl << exp.ErrorMsg() << endl;
		clog << exp.StackInfo() << endl;
	}
	SQR_TRY_END;
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////
//					����
/////////////////////////////////////////////////////////////////////////////////
//��ӡ������÷�
/*
static void PrintUsage()
{
	cerr<<"����: StressRunner :"<<endl
		<<"��ʽ: StressRunner -n Num "<<endl
		<<"���ӣ�StressRunner -n 100 "<<endl
		<<endl
		<<"  ��Ҫ���� "<<endl	
		<<"    -n Num     ����ѹ������������"<<endl
		<<endl;
}
*/
/////////////////////////////////////////////////////////////////////////////////
//					��������
/////////////////////////////////////////////////////////////////////////////////

//�������в�������ȡ��Ӧ��Ϣ
static int GetNumFromArgs(int nArgCount,wchar_t* aryArg[],const wchar_t* szOption)
{
	for(int i=1;i<nArgCount;++i)
	{
		const wchar_t* szArg=aryArg[i];

		if(wcsncmp(szOption,szArg,wcslen(szOption))!=0)
			continue;

		//�������ƥ���ˣ����ǽ�ȡ��������������α���������
		if(i + 1 < nArgCount)
		{
#ifdef _WIN32
			return _wtoi(aryArg[i + 1]);
#else
			return atoi( utf16_to_utf8(aryArg[i + 1]).c_str() );
#endif
		}
	}
	return 1;
}



//ע��ѹ��������Ҫ��������������ѹ������������Ŀ���Լ�ѹ����������
static void RegistStressNum(CScript& Script)
{
	REGIST_CONSTANT(Script, StressNum);
	REGIST_CONSTANT(Script, StressMaxNum);
}

