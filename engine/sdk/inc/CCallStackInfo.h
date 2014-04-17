#pragma once

#include "ModuleDefs.h"
#include "CMallocObject.h"

namespace sqr
{
#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable:4275)	//dll linkage
#pragma warning(disable:4251)	//dll linkage
#endif

	//���������ֻ����GenErr��ʱ�򱣴��ջ�������ڴ��������κι�ϵ��
	class MODULE_API CCallStackInfo
		:public CMallocObject
	{
	public:
		const char* GetText() const;
		
		void WalkStack();

		//stBase��������һ���ջ��ʼ��¼��һ���Ǵӵ�0�㿪ʼ��stCount������¼�Ĳ���
		CCallStackInfo(bool bWalk = true, size_t stBase = 0, size_t stCount = 30);
		CCallStackInfo(void* buffer, size_t stBase = 0, size_t stCount = 30);
#ifdef _WIN32
		CCallStackInfo(PCONTEXT pContext, size_t stBase = 0, size_t stCount = 30);
#endif
		~CCallStackInfo(void);
	private:
		void GetStack( size_t uBegin, size_t uEnd );
#ifdef _WIN32
		void GetStack( PCONTEXT pContext, uint32 uBegin, uint32 uEnd );
		void FetchSymbol(void* pAddr);
#endif

		void CopySymbol(const char* szSymbol);

		size_t m_stNextSymbolPos;

		size_t m_stBegin;
		size_t m_stEnd;
		
		char*		m_ayStack;
		bool		m_bNewStack;
	};

#ifdef _WIN32
#pragma warning(pop)
#endif
}
