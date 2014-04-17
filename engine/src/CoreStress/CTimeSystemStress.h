#pragma once

#include "TTimeSystem.h"

namespace sqr
{

	class CTimeSystemStress
		:public TTimeSystem<CTimeSystemStress>
	{
	private:
		friend class CConnStress;

		typedef TTimeSystem<CTimeSystemStress>	Parent_t;
		friend class TTimeSystem<CTimeSystemStress>;
		CTimeSystemStress(uint32 uBaseCyc);

		int32	m_nClientServerTimeDiff;	//client��server�ľ���ʱ�����������

		void SetGlobalTime(uint32 uGlobalTime);
	public:
		uint32 GetGlobalTime()const;
	};

}

