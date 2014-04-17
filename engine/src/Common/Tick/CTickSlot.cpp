#include "stdafx.h"
#include "ExpHelper.h"
#include "CTickSlot.h"
#include "CTickImp.h"
#include "CTickMgr.h"
#include "CTickQueue.h"
#include "TObjectBank.inl"
#include "TSqrAllocator.inl"

void CTickSlot::AddTick( CTickImp* pTickImp )
{
	CTickQueue* pTickQueue = GetTickQueue( pTickImp->m_uRing );
	
	if( !pTickQueue )
	{
		QueueBank_t* pBank=pTickImp->m_pTickMgr->m_pTickQueueBank;

		pTickQueue= pBank->PopOrNew();
		
		m_TickQueueMap.push_back( make_pair( pTickImp->m_uRing,pTickQueue ) );
		pTickQueue->Init((uint32)m_TickQueueMap.size() - 1);
	}

	pTickQueue->push_back( pTickImp );
	pTickQueue->m_uTickCount++;  //��queue��tick������һ
	pTickImp->m_uQueuePos = uint32(pTickQueue->size()-1);
}

void CTickSlot::DelTick( CTickImp* pTickImp )
{
	CTickQueue* pTickQueue = GetTickQueue( pTickImp->m_uRing );
	pTickQueue->at( pTickImp->m_uQueuePos ) = NULL;
	pTickQueue->m_uTickCount--;
	
	if (pTickQueue->m_uTickCount == 0)
	{
		//�������last tick queue���������tick��ɾ�����ڵ�ǰ��queue������е�
		//��ʱ�����ǲ���ɾ�����queue����Ϊ����tick֮���Զ���ɾ��
		if (pTickQueue == pTickImp->m_pTickMgr->m_pLastTickQueue)
		{
			return;
		}

		//���tick�ļ���Ϊ0�ˣ���ô���ǲ�����Ҫ���queue��
		DelTickQueue(pTickQueue, pTickImp->m_pTickMgr->m_pTickQueueBank);
	}
}

void CTickSlot::ClearTick()
{
	const size_t stSize = m_TickQueueMap.size();

	for( size_t i=0;i<stSize;++i )
	{
		CTickQueue* pTickQueue= m_TickQueueMap[i].second;
		
		CTickQueue::iterator itEnd=pTickQueue->end();

		for( CTickQueue::iterator it=pTickQueue->begin() ; it!=itEnd; ++it )
		{
			CTickImp* pTickImp=*it;

			if( !pTickImp )
				continue;

			DelTick( pTickImp );
		}

		delete pTickQueue;
	}

	m_TickQueueMap.clear();
}

CTickQueue* CTickSlot::GetTickQueue( uint32 uRing )
{
	const size_t stSize=m_TickQueueMap.size();
	
	for( size_t i=0; i< stSize; ++i )
	{
		pair< uint32,CTickQueue*> Pair=m_TickQueueMap[i];
		if( Pair.first == uRing )
		{
			Pair.second->m_uRingSlot = (uint32)i;
			return Pair.second;
		}
	}
	return NULL;
}

void CTickSlot::DelTickQueue( CTickQueue* pTickQueue, QueueBank_t* pBank )
{
	uint32 uRingSlot = pTickQueue->m_uRingSlot;

	//��Ϊ��ɾ����slot��������queue��ʱ�򣬿�����ǰ��ĳ�������Ѿ��Ѹ�queueǰ���queueɾ����
	//�������queue��λ�þͲ���ԭ�����õ�ringslot����ģ�����������Ҫ�Ӹ�ringslot��ǰ����

	uint32 uQueueMapSize = (uint32)m_TickQueueMap.size();
	Ast(uQueueMapSize > 0); 

	if (uRingSlot >= uQueueMapSize)
	{
		uRingSlot = uQueueMapSize - 1;
	}

	for (uint32 i = uRingSlot; i >= 0; i--)
	{
		pair< uint32,CTickQueue*> Pair=m_TickQueueMap[i];
		if (Pair.second == pTickQueue)
		{
			m_TickQueueMap.erase( m_TickQueueMap.begin()+ i);
			pBank->Push( pTickQueue );
			return;
		}
	}

}

