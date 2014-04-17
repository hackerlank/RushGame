#pragma once

#include "ThreadTypes.h"
#include "CMemCallStackInfoSet.h"
#include "CMallocObject.h"

namespace sqr
{
	class CMemCallStackInfoMgr
		:public CMallocObject
	{
	public:
		void AddStack(CMemCallStackInfo*& pStackInfo, uint32 uMemSize);
		void DelStack(CMemCallStackInfo*& pStackInfo,uint32 uMemSize);

		static CMemCallStackInfoMgr& Inst();

		void Print();

		int32 nLastElem; // ��¼����Сһ��Top
		int32 nHadElem; // Top���Ѿ����ڵ�Ԫ�ظ���
	private:
		HLOCK					m_Lock;
		CMemCallStackInfoSet    m_setStack;

		CMemCallStackInfoMgr(void);
		~CMemCallStackInfoMgr(void);
	};
}
