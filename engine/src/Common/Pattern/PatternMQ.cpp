#include "stdafx.h"
#include "PatternMQ.h"
#include "ExpHelper.h"
#include <iostream>
#include "ErrLogHelper.h"
#include "CMemPool.h"
#include "TSqrAllocObject.inl"
#include "TSqrAllocator.inl"


/*
Pattern MQ��Ϣ������ƣ���ǰ��ʵ�����ܹ���֤ͬһ��owner����Ϣʱ�򣬵���
���ܱ�֤������Ϣ��ʱ�����׻������ڱ����֧��������Ϣʱ�������������������������
Ϊ�˲��Ķ��ⲿ����������ǰ�Ľӿڶ�����
��CPtMQMsgMgr���棬���ڱ���һ����Ϣ���У���Ϣֱ�Ӽ��뵽��Ϣ�������棬�������ܱ�֤����
��Ϣ��ʱ��
��Ҫע��һ�����⣬����ownerɾ����ʱ��owner��Ӧ����Ϣ�ǲ����ɷ��ˣ���ǰ��������ownerɾ����
ʱ���Ӧ����Ϣ��ɾ���ģ����������ǲ�����ô���ˡ�������Ҳ������owner���汣��ÿһ����Ϣ
��iteratorָ�룬����̫���ڴ��ˡ�
���������ṩһ���м������proxy���������owner˫�����ã���ͬʱ����Ϣ���á�
��owner����һ����Ϣ��ʱ�����proxy�����ڣ������ȴ���һ��proxy����proxy�󶨵���Ϣ����
ͬʱ����proxy�����ü�����
��ownerɾ����ʱ�򣬰Ѷ�Ӧproxy�����Ӧ��ownerָ����Ϊnull���������ɷ���Ϣ��ʱ�������ж�
����Ϣ����proxy��owner�Ƿ�Ϊnull�������Ϊ�����ɷ�
û�ɷ�һ����Ϣ����Ӧ��proxy�����ü�����һ�����proxy�����ü���Ϊ0���Ҷ�Ӧ��owner������
��ô�Ͱ����proxyɾ��
*/

DefineAllocObject(CPtMQMallocObject);

namespace sqr
{
	class COMMON_API CPtMQMsgOwnerProxy
		:public CPtMQMallocObject
	{
	public:
		friend class CPtMQMsgOwner;
		friend class CPtMQMsgMgr;
		friend class CPtMQMsg;

		CPtMQMsgOwnerProxy(CPtMQMsgOwner* pOwner);

		void AddRef();
		void DelRef();
		void Release();

	private:
		~CPtMQMsgOwnerProxy();

	private:
		CPtMQMsgOwner*	m_pOwner;
		uint32			m_uCounter;
	};
}

CPtMQMsgOwnerProxy::CPtMQMsgOwnerProxy(CPtMQMsgOwner* pOwner)
: m_pOwner(pOwner)
, m_uCounter(0)
{
}

CPtMQMsgOwnerProxy::~CPtMQMsgOwnerProxy()
{
}

inline void CPtMQMsgOwnerProxy::AddRef()
{
	++m_uCounter;
}

inline void CPtMQMsgOwnerProxy::DelRef()
{
	--m_uCounter;
	if ( !m_pOwner && (m_uCounter == 0) )
		delete this;
}

CPtMQMsg::CPtMQMsg( CPtMQMsgOwner* pOwner )
{
	m_pOwnerProxy = pOwner->m_pProxy;
	m_pOwnerProxy->AddRef();
	pOwner->m_pMgr->m_queMsg.push(this);
}

CPtMQMsgOwner* CPtMQMsg::GetOwner()
{
	return m_pOwnerProxy->m_pOwner;
}

CPtMQMsg::~CPtMQMsg()
{
}

void *CPtMQMsg::operator new( size_t stSize, CPtMQMsgOwner* pOwner )throw()
{
	return pOwner->GetMgr()->m_pMemPool->Alloc(stSize);
}

CPtMQMsgOwner::CPtMQMsgOwner()
{
	m_pMgr = NULL;
	m_pProxy = new CPtMQMsgOwnerProxy( this );
}

CPtMQMsgOwner::~CPtMQMsgOwner()
{
	ClearMsg();

	delete m_pProxy;

	if( m_pMgr )
		--m_pMgr->m_uCounter;
}

void CPtMQMsgOwner::SetMgr(CPtMQMsgMgr* pMgr)
{
	m_pMgr = pMgr;

	Ast( m_pMgr );

	++m_pMgr->m_uCounter;
}

void CPtMQMsgOwner::ClearMsg()
{
	if( m_pProxy->m_uCounter == 0 )
		return;

	m_pProxy->m_pOwner = NULL;
	m_pProxy = new CPtMQMsgOwnerProxy( this );
}

CPtMQMsgMgr::CPtMQMsgMgr(size_t stMemPoolPageSize)
{
	m_uCounter = 0;
	m_pMemPool= new CMemPool( stMemPoolPageSize, CPtMQMallocObject::GetPool() );
}


CPtMQMsgMgr::~CPtMQMsgMgr()
{
	delete m_pMemPool;
}


bool CPtMQMsgMgr::FlushMsg()
{
	if (m_queMsg.empty())
		return false;

	for( ;; ) 
	{
		SQR_TRY
		{
			while( !m_queMsg.empty() )
			{
				CPtMQMsg* pMsg = m_queMsg.front();
				m_queMsg.pop();

				CPtMQMsgOwnerProxy* const pOwnerProxy = pMsg->m_pOwnerProxy;
				CPtMQMsgOwner* const pOwner = pOwnerProxy->m_pOwner;

				if (pOwner)
					pMsg->Dispatch(pOwner);

				pOwnerProxy->DelRef();
				delete pMsg;
			}
			break;
		}
		SQR_CATCH(exp)
		{
			LogExp(exp);
		}
		SQR_TRY_END;
	}

	m_pMemPool->Clear();
	return true;
}

bool CPtMQMsgMgr::Release()
{
	if ( m_uCounter > 0 )
		return false;

	delete this;
	return true;
}
