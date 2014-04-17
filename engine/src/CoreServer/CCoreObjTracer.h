#pragma once
#include "FindPathDefs.h"
#include "CoreObjectDefs.h"
#include "CDistortedTick.h"
#include "PatternRef.inl"
#include "CPos.h"
#include "TCoreObjAllocator.h"
#include "SyncPatternCOR.h"

namespace sqr
{
	class CSyncCoreObjectServer;
	class CObjPosObservee;

	//Tracer���ټ̳�CObjPosObserver,����tick��ʵ��. CObjPosObserver���ܸĳ���tick��ʵ��,��Ϊ���󶼾�ֹʱtick�Ͱ׷���
	//Tracer��Tick���Զ����ݵ�ǰ�����Զ������Tick���,����Խ�����ԽС.�����СΪ100ms,���Ϊ500ms
	class CCoreObjTracer
		:public CDistortedTick
		,public CCoreObjMallocObject
		,public CPtCORHandler
	{
		friend class CSyncCoreObjectServer;
	public:
		CCoreObjTracer(CSyncCoreObjectServer* pCoreObj);

		ETraceResult Trace(CSyncCoreObjectServer* pTarget, float fSpeed, EFindPathType eFindPathType, EBarrierType eBarrierType, float fReachDist);
		void Stop();
		
	private:
		~CCoreObjTracer();

		virtual void OnTick();
		virtual void OnCOREvent(CPtCORSubject* pSubject,uint32 uEvent,void* pArg);

		EMoveToResult Move(float fSpeed = 0);
		CSyncCoreObjectServer* GetTarget()const;
		void AdjustTickFreq();
		float GetSelfSpeed()const;
		float GetTargetSpeed()const;

		CSyncCoreObjectServer*	m_pCoreObj;
		float					m_fSpeed;
		EFindPathType			m_eFindPathType;
		EBarrierType			m_eBarrierType;
		float					m_fReachDist;
		TPtRefer<CCoreObjTracer, CSyncCoreObjectServer> m_Target;
		uint32					m_uCurTickFreq;
		CFPos					m_fTargetCurRealPos;
		uint32					m_uNextTickFreq;		//����������һ��tick����
	};
}

