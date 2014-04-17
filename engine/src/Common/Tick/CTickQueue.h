#pragma once
#include "CTickMallocAllocator.h"

namespace sqr
{
	class CTickImp;
	
	class CTickQueue
		:public std::vector<CTickImp*, CTickMallocAllocator<CTickImp*> >
		,public CTickMallocObject
	{
	public:
		void Init(uint32 uRingSlot)
		{
			clear();
			reserve(64);
			m_uTickCount = 0;
			m_uRingSlot = uRingSlot;
		}

		uint32	m_uTickCount;	//��queue����tick�ĸ���
		uint32  m_uRingSlot;	//��queue����slot��λ��
	};
}
