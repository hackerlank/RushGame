#pragma once


#include "ModuleDefs.h"

namespace sqr
{
	MODULE_API size_t GetAllMemUsage();  //��ȡ��ǰʹ�õ��ڴ�����
	MODULE_API size_t GetAllCapacity(); //��ȡ��ǰռ�õ��ڴ�����

	
	//nCheckLevel:1 ����й©���� 2��ӡ��ϸ��ջ
	MODULE_API void WatchLeakage_Begin(int nCheckLevel);
	MODULE_API void WatchLeakage_End();
#ifndef _WIN32
	MODULE_API void WatchLeakage_FetchAllSymbol();
#endif
	//�ڴ��ڴ�й¶���ҵ�����£���ӡ��ǰ�����ڴ�����ջ���
	MODULE_API void WatchLeakage_PrintMemInfo();

	//���ڴ�����ص�ʱ��дlog������ʲô������
	MODULE_API void WatchLeakage_MemLog(const char* szName);

	struct MODULE_API ProcessMemInfo
	{
	public:
		size_t stAvailPhysMem;  // ϵͳ��ǰʣ�������ڴ�
		size_t stAvailPageMem;  // ϵͳ��ǰʣ��ҳ�����ļ�
		size_t stProcessPhys;   // ����ռ�õ������ڴ�
		size_t stProcessPage;   // ����ռ�õ�ҳ�����ļ�
	};

	//��ȡ��ǰ���̵��ڴ���Ϣ
	MODULE_API void GetProcessMemInfo(ProcessMemInfo* pMemInfo);

}
