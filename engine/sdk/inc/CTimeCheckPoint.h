#pragma once
#include "CDynamicObject.h"
#include "CMallocObject.h"

namespace sqr
{

	class MODULE_API CTimeCheckPoint
		:public virtual CDynamicObject
		,public CMallocObject
	{
	public:
		CTimeCheckPoint();

		void SetCheckPoint();
		void SetCheckPoint(uint32 uPassedTime);

		void SetBaseTime(uint64 uBaseTime);
		uint64 GetBaseTime()const;

		uint64 GetElapse()const;	//millisecond
		uint64 GetHDElapse()const;	//microsecond

	private:
		uint64			m_uBaseTime;
		//�̻߳��ڲ�ͬ��cpu�����У�һЩBIOS����HAL��bug�ᵼ��ÿ��cpu�ϵ�counter�Ľ��Ȳ�һ������ȡ��counter���ܱ�֮ǰ��counterС��
		//�����������ȷ�����ȡ��ʱ��һ������ǰ���ȡ��ʱ��
		mutable uint64	m_uLastQueryTime;

#ifdef _WIN32
		uint64 GetTime()const;
#else
		static uint64 GetTime();
#endif
	};
	
}
