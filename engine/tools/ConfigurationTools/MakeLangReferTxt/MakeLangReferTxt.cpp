// MakeLangReferTxt.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "common.h"
#include "CMakeReferRelation.h"
#include "ConsoleInfo.h"
#include "CSplitInfo.h"
#include "CExpLog.h"

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_CTYPE,"");
	_set_invalid_parameter_handler(ReportInvaidParamentFun);
	_set_se_translator(ReportESHFun);
	CSplitInfo::GetInst()->Init(argc, argv);
	if(!CSplitInfo::GetInst()->GetBeInternational())
		return 0;
	BeginToPrint("MakeLangReferTxt");
	CConsoleInfo::GetInst()->PrintFunction("��������ͨ�ù�ϵ���ñ�");
	FileTreeWalk( gbk_to_utf16(CSplitInfo::GetInst()->GetLangSrcPath()).c_str(), CMakeReferRelation::CreateRefer, NULL );
	CConsoleInfo::GetInst()->PrintFunction("����Obj Itemͨ�ù�ϵ���ñ�");
	CMakeReferRelation::GetInst()->CreateObjAndItemTwoWay();
	CMakeReferRelation::GetInst()->WriteReferInfoToTxt();
	EndToPrint();
	return 0;
}

