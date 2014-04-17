#pragma once
#include "PatternCOR.h"

//�����߳�ר�õ�PatternCOR,������߼��߳�ʹ�õ�PatternCOR���ֿ�

namespace sqr
{
	class CSyncCOREventMgr : public CCOREventMgr
	{
	public:
		static CSyncCOREventMgr* Inst()
		{
			static CSyncCOREventMgr ls_Inst;
			return &ls_Inst;
		}
	};

	class CSyncPtCORSubject : public CPtCORSubject
	{
	public:
		CSyncPtCORSubject(uint32 uHighFreqEvent);
		virtual ~CSyncPtCORSubject();

		virtual bool Notify(uint32 uEventType);
		virtual bool Notify(uint32 uEventType,void* pArg);
	};

}
