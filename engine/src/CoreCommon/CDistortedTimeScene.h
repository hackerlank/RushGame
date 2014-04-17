#pragma once
#include "CDistortedTimeAllocator.h"

namespace sqr
{
	class CDistortedTimeObj;

	class CDistortedTimeScene
		: public CDistortedTimeMallocObject
	{
	public:
		CDistortedTimeScene(void);
		virtual ~CDistortedTimeScene();

		void Release();
		void DestroyDistortedTimeObj(CDistortedTimeObj* pObj);

		void SetTimeGroup(uint32 uTimeGroup);
		uint32 GetTimeGroup()const;
		void SetTimeDistortedRatio(float fTimeDistortedRatio);
		float GetTimeDistortedRatio()const;

		void AddDistortedTimeObj(CDistortedTimeObj* pObj);
		void DelDistortedTimeObj(CDistortedTimeObj* pObj);

		void OnDistortedTimeObjDestroyed();

	private:
		typedef list<CDistortedTimeObj*, CDistortedTimeAllocator<CDistortedTimeObj*> >	DistortedTimeObjContainer;
		DistortedTimeObjContainer			m_DistortedTimeObjContainer;

		float	m_fTimeDistortedRatio;	//ʱ��ϵ��
		uint32	m_uTimeGroup;			//ʱ����,ֻ����Ų���ͬ��DistortedTimeObj�Ż���ʱ��ϵ��Ӱ��
		bool	m_bInDestroying;
	};
}
