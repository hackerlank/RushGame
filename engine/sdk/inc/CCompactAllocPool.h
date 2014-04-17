#pragma once
#include "ISqrAllocPool.h"
#include "CMallocObject.h"

namespace sqr
{
	//���̲߳���footprint�����¼�ڴ���С�ķ������������ⲿÿ�δ����ڴ��С

	class MODULE_API CCompactAllocPool
		: public CMallocObject
		, public ISqrAllocPool
	{
	public:
		CCompactAllocPool(size_t stCapacity, const char* szPoolName);

		~CCompactAllocPool();

		void* Alloc(size_t stSize);
		void Dealloc(void* pMem,size_t stSize);
		void* Realloc(void* pMem,size_t stOldSize,size_t stNewSize);

		size_t GetMemUsage()const;
		size_t GetCapacity()const;

		const char* GetPoolName()const;
		void Trim();

	private:
		bool CheckLeakageEnabled()const;

		char		m_szPoolName[256];
		
		void*		m_pSpace;

		size_t		m_stMemUsage;

		bool		m_bDelayTrim;
	};

}

