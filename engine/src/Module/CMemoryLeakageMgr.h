#pragma once

/*
�ڴ棺
�ṩ�����ڴ������Ƶ�Ŀ�ģ�
�ṩ�����ڴ�й¶�ͽ����ڴ�����ͳ�ƵĹ���


����effective stl��˵����
����allocator�����ܸ���������⣬��ΪĳЩstl container����������allocator�������ڴ�(list,map,set)��
Ȼ������ʵ�ʲ��ԣ�
vc.net2003��stl�Լ�stlport4.3.3��list container���ڴ��������ȫʹ�ö��Ƶ�allocator�������ڴ��.
���Ʋ�������ͨ��rebind������ʵ��������ܡ�
*/

#include "ModuleDefs.h"
#include "ThreadTypes.h"
#include "CMallocObject.h"	

#ifndef _WIN32
#define MAX_PATH PATH_MAX
#endif

namespace sqr
{
	class CMemThreadMgr;
	/*

	�ڴ�й¶���ҵļ���Ҫ�㣺

	1.����������Ͷ�����CMallocObject����ʹ��TMallocAllocator��ʹ����Щ������ڴ����Ĳ�����ͳ��
	2.����CMemoryLeakageMgr֮�⣬���е�Mgr����ò�Ƶ��������壬ȫ����ֻ���첻�ͷţ���ҪΪ�˻رܵ�������ʱ������
	3.���ڹ��������ڴ�Ĺ������ڶ����߳�CMemThread�����У��������߳��ڵ�������������ʱ���Ѿ���ϵͳǿ�йرգ�������ʱ��
	  ��ӡ�ڴ�й©�Ĺ��ܣ�����ָ����CMemoryLeakageMgr����������ʱ��ִ�У�������ȷ��Main��������á�
    4.�����޷����õ�����������Ϊ���ʱ����ӡй©���ֶΣ�����RunModule�Լ�Module������Ӧ����ȫû�г��������ȫ�ֱ�����
	  ��̬������
    5.��Ҫʹ��cin,cout,cerr,ʹ��fprintf,fputs,puts,��Щc++����Ҳ�ǻ�����ڴ�ĳ��ڴ��ڵ�ȫ�ֶ���
	*/

	class CMemoryLeakageMgr
		:public CMallocObject
	{
	public:		
		static void Init(int nCheckLevel);
		static void Unit();
		static CMemoryLeakageMgr& Inst();
		//find leakage function		
		void WriteMemLog(const char* szLog);

#ifndef _WIN32
		void FetchAllSymbol();
#endif
		void DumpStack();		
	private:
		void DumpLeakageNum();

		CMemThreadMgr*	m_pThreadMgr;
		
		FILE*			m_fdLeakage;

		CMemoryLeakageMgr(int nCheckLevel);
		~CMemoryLeakageMgr();

		static CMemoryLeakageMgr	ms_Inst;


		wchar_t	 m_szMemFileName[MAX_PATH];
	};

}


