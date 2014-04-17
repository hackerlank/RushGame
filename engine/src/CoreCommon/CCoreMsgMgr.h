#pragma once
#include "ThreadTypes.h"
#include "CMemPool.h"
#include "TCoreMsgAllocator.h"

namespace sqr
{
	class CCoreMsg;
	class CCoreThreadMsg;

	class CCoreMsgMgr
	{
	public:
		void FlushMsg(void);
		void PostMsg(CCoreMsg* pMsg);

		void FlushThreadMsg(void);
		bool PostThreadMsg(CCoreThreadMsg* pMsg);
		
		static void Init();
		static void Unit();
		static CCoreMsgMgr& Inst();

		CMemPool& MemPool();
	
	protected:
		friend class CCoreMsg;

		static CCoreMsgMgr*		ms_pInst;

		CMemPool				m_MemPool;
		
		//�����߳�Ͷ�ݵ�������Ϣ��������������߳���������
		typedef queue<CCoreMsg*, deque<CCoreMsg*, TCoreMsgAllocator<CCoreMsg*> > >	QueMsg_t;
		QueMsg_t		m_queCoreMsg;


		//�������߳�Ͷ�ݵ������̷߳������������������
		HLOCK				m_slkThreadCoreMsg;
		typedef list<CCoreThreadMsg*, TCoreMsgAllocator<CCoreThreadMsg*> >	ListThreadMsg_t;
		ListThreadMsg_t	m_lstThreadCoreMsg;
		ListThreadMsg_t	m_lstDoingThreadMsg;	

	protected:
		CCoreMsgMgr(void);
		virtual ~CCoreMsgMgr(void);
	};

	inline CMemPool& CCoreMsgMgr::MemPool()
	{
		return m_MemPool;
	}
}
