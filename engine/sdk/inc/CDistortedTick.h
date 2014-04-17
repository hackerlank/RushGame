#pragma once
#include "CDynamicObject.h"
#include "CDistortedTimeAllocator.h"

namespace sqr
{
	class CTick;
	class CDistortedTimeObj;
	class IDistortedTime;
	class CDistortedTickImp;

	class CTraitsServer;
	class CSyncTraitsServer;
	class CTraitsClient;
	class CTraitsStress;
	template<typename Traits>
	class TApp;

	class CDistortedTick
		:public virtual CDynamicObject
	{
		friend class CDistortedTimeObj;
		friend class CDistortedTickImp;
		friend class TApp<CTraitsServer>;
		friend class TApp<CSyncTraitsServer>;
		friend class TApp<CTraitsClient>;
		friend class TApp<CTraitsStress>;
	public:
		CDistortedTick();
		virtual	~CDistortedTick(void);

		void SetTickName(const char* szName);
		virtual void OnTick();

		bool Registered() const;
		uint32 GetInterval()const;
		uint32 GetLeftTime()const;

	private:
		void SetDistortedTime(IDistortedTime* pDistortedTime);

		CTick*	m_pTick;
		IDistortedTime*	m_pDistortedTime;				//����IDistortedTime�ӿ�ָ�������Ϊ����ע��tickʱ�����,��Ӧ����CDistortedTick�ĳ�Ա�����е���IDistortedTime�ϵĽӿ�.

		uint32	m_uKeyOfDistortedTickMap;

		uint32			m_uTickCycInRealTime;			//ע��tickʱ��ԭʼʱ����,���û��tickע����Ϊ-1
		bool			m_bIntervalNeedToBeRefreshed;	//��ע���tick��ʱ��ϵ�������仯ʱ�����ʣ��ʱ�䲢ע��һ���µ�tick,�����ñ�־λ��Ϊtrue.����tickʱ�䵽��ûָ�ԭ����tick���.
	};
}

