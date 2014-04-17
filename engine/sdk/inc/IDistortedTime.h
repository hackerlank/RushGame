#pragma once
#ifdef _WIN32
#include "CRefObject.h"
#endif

namespace sqr
{
	class CDistortedTick;

	//��¶���ⲿ����Ľӿ�, ����ʱ��ϵͳ�ڲ����벻Ӧ�õ�������Ľӿ�
	class IDistortedTime
#ifdef _WIN32
		: public CRefObject
#endif
	{
	public:
		IDistortedTime(void){}
		virtual ~IDistortedTime(void){}
		virtual void RegistDistortedTick(CDistortedTick* pTick, uint32 uCyc)=0;
		virtual void UnregistDistortedTick(CDistortedTick* pTick)=0;
		virtual uint64 GetDistortedProcessTime()const=0;
		virtual uint64 GetDistortedFrameTime()const=0;
		virtual uint64 GetDistortedServerFrameTime()const=0;
	};

}

