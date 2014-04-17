
#include "stdafx.h"

#include "BaseTypes.h"
#include "CPatcher.h"
#include "CPatcherUpdateHandler.h"


using namespace sqr;

#define  PATCH_FILE  L"download/patch_0.0.13.ybtx"


//DEFINITION_OF_OPERATOR_NEW

void StartPatch()
{
	sqr::CPatcher patch;
	CPatcherUpdateHandler handler;
	// ����������������ѡ���������汾���뵱ǰ�汾�ŶԱȾ���
	patch.CheckHeadInfo(&handler, PATCH_FILE); // MemoryBoundCheck(1000);
}


int _tmain(int argc, _TCHAR* argv[])
{
	// �ڴ�Խ����
	//SetBoundCheckEnabled(true);

	// ��ʼ�ڴ�й¶����
	//SetFindLeakageEnabled(true);

	//if (IsFindLeakageEnabled())
	//	BeginWatchingLeakage();

	//CMapFileMgr::Inst().HandleMapFile("F:/ybtx/programmer/trunk/bin/Debug/ApplyPatch.map");
	//SetMemFileName("leak", "F:/ybtx/programmer/tools/dbti/var/Client/ver/ybtx_69179_c/0.6.1/bin/Release");

	// ��ʼ�ϲ���
	StartPatch();

	// �����ڴ�й¶����
	//if (IsFindLeakageEnabled())
	//	EndWatchingLeakage();
	//
	//SetFindLeakageEnabled(false);

	return 0;
}

