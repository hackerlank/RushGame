#pragma once
#include "IDistortedTime.h"
#include "CDistortedTimeAllocator.h"
#include "TIDPtrMap.h"

namespace sqr
{
	class CDistortedTick;
	class CDistortedTimeScene;

	class CDistortedTimeObj 
		: public IDistortedTime
		, public CDistortedTimeMallocObject
	{
		friend class CDistortedTimeScene;
	public:
		virtual uint64 GetDistortedProcessTime()const;

		void SetTimeGroup(uint32 uTimeGroup);
		uint32 GetTimeGroup()const;

		float GetTimeRatio()const;

		//��DistortedTick��ʱ��������Ϊ�������
		virtual void RefreshTickInterval(CDistortedTick* pTick)=0;
		template<typename TTimeSystem>
		void RefreshTickInterval(CDistortedTick* pTick);

		//void SetPosition(CPos& pos);	//���������ȫ����Ψһʱ��ϵ�����������û�õ�,��ʱΪ�Ժ�����չ��׼��

		void TransferTo(CDistortedTimeScene* pNewScene);

	protected:
		CDistortedTimeObj(CDistortedTimeScene* pScene);
		~CDistortedTimeObj();

		template<typename TTimeSystem>
		void RegistDistortedTick(CDistortedTick* pTick, uint32 uCyc);
		template<typename TTimeSystem>
		void UnregistDistortedTick(CDistortedTick* pTick);
		template<typename TTimeSystem>
		bool IntRegistDistortedTick(CDistortedTick* pTick, uint32 uCyc);
		template<typename TTimeSystem>
		bool IntUnregistDistortedTick(CDistortedTick* pTick);

		virtual void OnTimeRatioChanged(float fOldTimeRatio);
		template<typename TTimeSystem>
		void OnTimeRatioChanged(float fOldTimeRatio);

		bool TimeNeedToBeDistorted()const;
		uint64 CalDistortedTime(uint64 uCurTime, uint64 uLastRealTimeWhenTimeRatioChanged, uint64 uLastDistortedTimeWhenTimeRatioChanged, float fTimeRatio)const;

		uint64 GetDistortedProcessTime(float fTimeRatio)const;
		template<typename TTimeSystem>
		uint64 GetDistortedFrameTime(float fTimeRatio)const;

		typedef TIDPtrMap<CDistortedTick>	TickContainer;
		TickContainer						m_TickContainer;


		//�ֱ�ΪProcessTime��FrameTime��¼����ʵʱ���Ť��ʱ��
		uint64 m_uLastDistortedProcessTimeWhenTimeRatioChanged;	//��¼�ϴ�ʱ��ϵ���ı�ʱ��Ť��ʱ��
		uint64 m_uLastRealProcessTimeWhenTimeRatioChanged;			//��¼�ϴ�ʱ��ϵ���ı�ʱ����ʵʱ��
		uint64 m_uLastDistortedFrameTimeWhenTimeRatioChanged;	
		uint64 m_uLastRealFrameTimeWhenTimeRatioChanged;			

	private:
		uint32 m_uTimeGroup;

		CDistortedTimeScene* m_pDistortedTimeScene;
		typedef list<CDistortedTimeObj*, CDistortedTimeAllocator<CDistortedTimeObj*> >	ListDistortedTimeObj;
		ListDistortedTimeObj::iterator		m_itDistortedTimeObjList;


	};
}
