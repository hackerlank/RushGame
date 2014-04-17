#pragma once
#include "CMallocObject.h"
#include "CStackFrameSet.h"

namespace sqr
{
	class CStackFrame;

	class CStackFrameMgr
		: public CMallocObject
	{
	public:
		CStackFrame* AddFrame(void*);
		void DelFrame(CStackFrame*);

#ifndef _WIN32
		//����ǰ���д��ڵ�Frame������ȡ��������Frame��
		void FetchAllSymbol();
#endif

		static CStackFrameMgr& Inst();
	private:
		CStackFrameInfoSet	m_setStack;

		CStackFrameMgr(void);
		~CStackFrameMgr(void);
	};
}
