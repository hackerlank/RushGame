#include "stdafx.h"
#include "CThreadAllocPool.h"
#include "dlmalloc.h"
#include <iomanip>
#include "CMemoryCookie.h"
#include "CMemCallStackInfo.h"
#include "CMemThreadMgr.h"
#include "ExpHelper.h"
#include "ThreadHelper.inl"
#include "CSqrAllocPool.h"
#include "MemoryHelper.h"



CThreadAllocPool::CThreadAllocPool(CSqrAllocPool* pPool)
{
	m_dlmspace = create_mspace( 0, 0 );
	m_nMemUsage = 0;
	
	CMemoryCookie* pCookie=static_cast<CMemoryCookie*>( mspace_malloc( m_dlmspace, sizeof(CMemoryCookie) ) );
	pCookie->m_pNext=NULL;

	m_pFreeQueueIn = pCookie;
	m_pFreeQueueOut = pCookie;
	m_pParentPool = pPool;

	m_bDelayTrim = false;	
}

CThreadAllocPool::~CThreadAllocPool(void)
{
	destroy_mspace( m_dlmspace );
}

void* CThreadAllocPool::GetMSpace()const
{
	return m_dlmspace;
}


static void PrintMemoryToStream( ostringstream& strm,void* pMem,size_t stSize )
{
	//����������ڴ��ǰ32���ֽڿ�ʼ��ӡ

	strm<< "��ʼ��ַǰ��32���ֽ�:" << endl;

	for( int i = -32 ;i< 0; ++i )
	{
		strm << hex <<setfill('0') << setw(2) << int( *( (uint8*)(pMem)+i ) )
			<< setw(1) << " ";
	}

	strm<< endl ;

	strm<< "��ʼ��ַ���" << dec << stSize << "���ֽ�:" << endl;

	for( int i = 0 ;i< int(stSize); ++i )
	{
		strm << hex <<setfill('0') << setw(2) << int( *( (uint8*)(pMem)+i ) )
			<< setw(1) << " ";
	}
}





//������ڴ��������8�ֽڶ��룬����һЩ����ĵײ�api������������������WSASend����

static const size_t gs_stAligment = 8;//8�ֽڶ���
//���ص�������8�ֽڶ���
#define ROUND_UP(size) (size_t)((size + gs_stAligment - 1) & ~(gs_stAligment - 1));

static const size_t gs_stCookieSize = ROUND_UP(sizeof(CMemoryCookie));
static const size_t gs_stCheckSize = 1;
static const size_t gs_stExtraSize = gs_stCookieSize + gs_stCheckSize;

void* CThreadAllocPool::Alloc(size_t stSize)
{
	HandleFreeQueue();

	stSize += gs_stExtraSize;

	uint8* const p= reinterpret_cast< uint8* >( mspace_malloc( GetMSpace(), stSize ) );
	
	if ( !p )
	{
#ifdef _WIN32
		void* ptr = VirtualAlloc(0, stSize, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
		if  (ptr == 0)
		{
			ostringstream strm;
			strm << "�����С: " << stSize<<" , "
				<< "�ڴ��: " << m_pParentPool->GetPoolName()<<" , "
				<< "�ڴ��ʹ��: " << GetMemUsage()<<" , "
				<< "�ڴ�ش�С: " << GetCapacity()<<" , "
				<< "�����ڴ��ʹ��: " << GetAllMemUsage()<<" , "
				<< "�����ڴ�ش�С: " << GetAllCapacity()<<" , "
				<< "VirtualAlloc ����ʧ�� ErrorID: " <<	GetLastError()
				<< endl;
			GenErr("�����ڴ�ʧ��", strm.str().c_str());
		}
		else
		{
			VirtualFree(ptr, 0, MEM_RELEASE);
			ostringstream strm;
			strm << "�����С: " << stSize<<" , "
				<< "�ڴ��: " << m_pParentPool->GetPoolName()<<" , "
				<< "�ڴ��ʹ��: " << GetMemUsage()<<" , "
				<< "�ڴ�ش�С: " << GetCapacity()<<" , "
				<< "�����ڴ��ʹ��: " << GetAllMemUsage()<<" , "
				<< "�����ڴ�ش�С: " << GetAllCapacity()<<" , "
				<< "dlmalloc �������"
				<< endl;
			GenErr("�����ڴ�ʧ��", strm.str().c_str());
		}
#else
		ostringstream strm;
		strm << "�����С: " << stSize<<" , "
			<< "�ڴ��: " << m_pParentPool->GetPoolName()<<" , "
			<< "�ڴ��ʹ��: " << GetMemUsage()<<" , "
			<< "�ڴ�ش�С: " << GetCapacity()<<" , "
			<< "�����ڴ��ʹ��: " << GetAllMemUsage()<<" , "
			<< "�����ڴ�ش�С: " << GetAllCapacity()<<" , "
			<< endl;
		GenErr("�����ڴ�ʧ��", strm.str().c_str());
#endif
	}

	Atomic_FetchAndAdd( &m_nMemUsage, (int32)stSize );

	CMemoryCookie *const pCookie = reinterpret_cast<CMemoryCookie*>(p);
	pCookie->m_stSize=stSize;
	pCookie->m_pPool=this;
	
	if ( m_pParentPool->m_bFindLeakage )
	{
		pCookie->m_pStackInfo = new CMemCallStackInfo();

		CMemThreadMgr::Inst()->AddMemMallocJob(pCookie);
	}
	else
	{
		pCookie->m_uCheckSum = pCookie->m_stSize ^ size_t(pCookie) ^ size_t(0xffffffffffffffff);
	}


	uint8* const pBottomCheck = p + stSize - gs_stCheckSize;

	*pBottomCheck = 0xbb;
	
	return p + gs_stCookieSize;
}


inline void CThreadAllocPool::ConfirmIsChildPool( CThreadAllocPool* pThreadPoolOfCookie )
{
	if( pThreadPoolOfCookie->m_pParentPool == m_pParentPool )
		return;

	ostringstream strm;
	strm<<"\""<<pThreadPoolOfCookie->m_pParentPool->GetPoolName()<<"\"�з�����ڴ汻�õ�\""
		<<m_pParentPool->GetPoolName()<<"\"���ͷ�!";

	GenErr( strm.str() );
}

void CThreadAllocPool::Dealloc(void* pMem)
{
	if( !pMem )
		return;

	uint8*const p = reinterpret_cast<uint8*>(pMem) - gs_stCookieSize;

	CMemoryCookie * const pCookie= reinterpret_cast<CMemoryCookie*>( p );

	CThreadAllocPool * const pPoolOfCookie = pCookie->m_pPool;
	
	const size_t stSize=pCookie->m_stSize;
	
	if ( m_pParentPool->m_bFindLeakage )
	{
		ConfirmIsChildPool( pPoolOfCookie );

		Atomic_FetchAndAdd( &pPoolOfCookie->m_nMemUsage, -(int32)stSize );

		if( !CMemThreadMgr::Inst() )
			ShowErrorMsgAndExit("RunModule����Module�д���ֱ�ӻ��ӱ����������ȫ�ֻ��߾�̬������Щ����������ڴ���������������֮��");

		CMemThreadMgr::Inst()->AddMemFreeJob(pCookie, m_pParentPool);
	}
	else
	{
		if( ( stSize ^ size_t(pCookie->m_uCheckSum) ^ size_t(pCookie) ) != size_t(0xffffffffffffffff) )
		{
			ostringstream strm;
			PrintMemoryToStream( strm , pMem , 128-32 );

			GenErr( "�ڴ泤��У��ʧ�ܣ�������Ұָ������ڴ汻д���������ͷŸ��ڴ�顣",strm.str() );
		}

		uint8* const pBottomCheck = p + stSize - gs_stCheckSize;

		bool const bWriteBelowBoundary = *pBottomCheck != 0xbb;

		ConfirmIsChildPool( pPoolOfCookie );
		
		Atomic_FetchAndAdd( &pPoolOfCookie->m_nMemUsage, -(int32)stSize );

		if( pPoolOfCookie == this )
		{
			mspace_free( GetMSpace(), pCookie );
		}
		else
		{
			pPoolOfCookie->PushToFreeQueue( pCookie );
		}

		if( bWriteBelowBoundary )
		{
			//�����﷢���ڴ������ʱ��,stSize��ֵ�ǿ��ŵģ���Ϊͨ����У�飩,�������ｫ�����ӳٵ��ڴ��ͷ��Ժ�

			size_t stPrintSize = stSize+32;
			if( stPrintSize > 512-32 )
				stPrintSize = 512-32;

			ostringstream strm;

			PrintMemoryToStream( strm , pMem , stPrintSize );

			DbgGenErr("�ڴ�����дԽ��" ,strm.str());
		}
	}
}


void* CThreadAllocPool::Realloc(void* pMem,size_t stNewSize)
{
	if( !pMem )
		return Alloc( stNewSize );

	if( stNewSize == 0 )
	{
		Dealloc( pMem );
		return NULL;
	}

	uint8*const oldp = reinterpret_cast<uint8*>(pMem) - gs_stCookieSize;

	CMemoryCookie *const pOldCookie= reinterpret_cast<CMemoryCookie*>( oldp );

	CThreadAllocPool*const pOldPool = pOldCookie->m_pPool;
	
	const size_t stOldSize=pOldCookie->m_stSize;
	
	if( (pOldPool != this) || m_pParentPool->m_bFindLeakage )
	{
		void*pNewMem = Alloc( stNewSize );
		memcpy( pNewMem, pMem, min(stOldSize-gs_stExtraSize,stNewSize) );
		Dealloc( pMem );
		return pNewMem;
	}

	if( ( stOldSize ^ size_t(pOldCookie->m_uCheckSum) ^ size_t(pOldCookie) ) != size_t(0xffffffffffffffff) )
	{
		ostringstream strm;
		PrintMemoryToStream( strm , pMem , 128-32 );

		GenErr( "�ڴ泤��У��ʧ�ܣ�������Ұָ������ڴ汻д���������ͷŸ��ڴ�顣",strm.str() );
	}

	ConfirmIsChildPool( pOldPool );
	
	uint8* const pBottomCheck = oldp + stOldSize - gs_stCheckSize;

	bool const bWriteBelowBoundary = *pBottomCheck != 0xbb;

	stNewSize += gs_stExtraSize;

	uint8*const newp=reinterpret_cast<uint8*>( mspace_realloc( GetMSpace(), oldp, stNewSize ) );

	if( bWriteBelowBoundary )
	{
		//�����﷢���ڴ������ʱ��,stSize��ֵ�ǿ��ŵģ���Ϊͨ����У�飩,�������ｫ�����ӳٵ��ڴ��ͷ��Ժ�

		size_t stPrintSize = stOldSize+32;
		if( stPrintSize > 512-32 )
			stPrintSize = 512-32;

		ostringstream strm;

		PrintMemoryToStream( strm , pMem , stPrintSize );

		DbgGenErr("�ڴ�����дԽ��" ,strm.str());
	}	

	if ( !newp )
	{
		ostringstream strm;
		strm << "���·����С: " << stNewSize<<" , "
			<< "�ڴ��: " << m_pParentPool->GetPoolName()<<" , "
			<< "�ڴ��ʹ��: " << GetMemUsage()<<" , "
			<< "�ڴ�ش�С: " << GetCapacity()<<" , "
			<< endl;
		GenErr("���·����ڴ�ʧ��", strm.str().c_str());
	}

	Atomic_FetchAndAdd( &m_nMemUsage, int32(stNewSize)-int32(stOldSize) );
	
	CMemoryCookie *const pNewCookie = reinterpret_cast<CMemoryCookie*>(newp);
	
	pNewCookie->m_stSize=stNewSize;
	
	pNewCookie->m_uCheckSum = pNewCookie->m_stSize ^ size_t(pNewCookie) ^ size_t(0xffffffffffffffff);

	uint8* const pNewBottomCheck = newp + stNewSize - gs_stCheckSize;

	*pNewBottomCheck = 0xbb;

	return newp + gs_stCookieSize;
}


void CThreadAllocPool::FreeFromMemThread( void* pMem )
{
	uint8*const p =static_cast<uint8*>(  pMem );

	CMemoryCookie * const pCookie= reinterpret_cast<CMemoryCookie*>( p);

	CThreadAllocPool * const pPoolOfCookie = pCookie->m_pPool;

	const size_t stSize=pCookie->m_stSize;

	uint8* const pBottomCheck = p + stSize - gs_stCheckSize;

	bool const bWriteBelowBoundary = *pBottomCheck != 0xbb;

	ConfirmIsChildPool( pPoolOfCookie );
	
	if( pPoolOfCookie == this )
	{
		mspace_free( GetMSpace(), pCookie );			
	}
	else
	{
		pPoolOfCookie->PushToFreeQueue( pCookie );
	}

	if( bWriteBelowBoundary )
	{
		//�����﷢���ڴ������ʱ��,stSize��ֵ�ǿ��ŵģ���Ϊͨ����У�飩,�������ｫ�����ӳٵ��ڴ��ͷ��Ժ�

		size_t stPrintSize = stSize+32;
		if( stPrintSize > 512-32 )
			stPrintSize = 512-32;

		ostringstream strm;

		PrintMemoryToStream( strm , pMem , stPrintSize );

		DbgGenErr("�ڴ�����дԽ��" ,strm.str());
	}
}


size_t CThreadAllocPool::GetMemSize(void* pMem)
{
	if( !pMem )
		return 0;

	uint8*const p = reinterpret_cast<uint8*>(pMem) - gs_stCookieSize;

	CMemoryCookie * const pCookie= reinterpret_cast<CMemoryCookie*>( p );

	return pCookie->m_stSize-gs_stExtraSize;
}

size_t CThreadAllocPool::GetMemUsage()const
{
	return m_nMemUsage;
}

size_t CThreadAllocPool::GetCapacity()const
{
	return mspace_footprint( GetMSpace() );
}


void CThreadAllocPool::PushToFreeQueue( CMemoryCookie* pCookie )
{
	pCookie->m_pNext=NULL;
	
	CMemoryCookie* pPrev=static_cast<CMemoryCookie*>
		( Atomic_SwapPtr( reinterpret_cast<void* volatile*>( &m_pFreeQueueIn ), pCookie ) );

	Ast( pPrev );

	pPrev->m_pNext = pCookie;
}

void CThreadAllocPool::HandleFreeQueue()
{
	if( m_bDelayTrim )
	{
		m_bDelayTrim=false;
		mspace_trim( m_dlmspace, 0 );

	}
	//��Զ����Ԥ�����һ��MemoryCookie��ɾ��
	
	CMemoryCookie* pCookie = m_pFreeQueueOut;
	CMemoryCookie* pNext = pCookie->m_pNext;

	if( !pNext )
		return;
	
	for(;;)
	{
		mspace_free( m_dlmspace, pCookie );

		pCookie = pNext;

		pNext = pCookie->m_pNext;

		if( !pNext )
		{
			m_pFreeQueueOut = pCookie;
			return;
		}		
	}
}

void CThreadAllocPool::DelayTrim()
{
	m_bDelayTrim = true;
}
