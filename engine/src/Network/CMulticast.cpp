#include "stdafx.h"

#include "CMulticast.h"
#include "CSyncPipe.h"
#include "CAsynPipeHandler.h"
#include "ErrLogHelper.h"
#include "ExpHelper.h"
#include "TSqrAllocator.inl"
#include <iomanip>

CMulticast::~CMulticast()
{}

bool CMulticast::AddPipe(IPipe* pIPipe)
{
	CSyncPipe* pPipe=dynamic_cast<CSyncPipe*>( pIPipe );

	if( !pPipe )
		GenErr("This muticast can accept only Sync Pipe!");

	if ( !pPipe->IsConnected() )
		return false;

	pair<CSyncPipe*,size_t> PipeSlot( pPipe , m_deqPipe.size() );

	pair<PipeMap_t::iterator,bool> p = m_mapPipe.insert( PipeSlot );

	if( p.second )
	{
		Ver( pPipe->AddToMulticast(this) );
		m_deqPipe.push_back( make_pair( pPipe,&p.first->second) );
	}

	return p.second;
}

bool CMulticast::DelPipe(IPipe* pIPipe)
{
	CSyncPipe* pPipe=dynamic_cast<CSyncPipe*>( pIPipe );

	if( !pPipe )
		GenErr("This muticast can accept only Sync Pipe!");

	PipeMap_t::iterator it=m_mapPipe.find( pPipe );
	if( it == m_mapPipe.end() )
		return false;

	const size_t stIndex=it->second;	
	m_mapPipe.erase( it );

	pair<CSyncPipe*,size_t*>& LastPipeInDeque= m_deqPipe.back();
	*LastPipeInDeque.second=stIndex;

	m_deqPipe[stIndex] = LastPipeInDeque;
	m_deqPipe.pop_back();

	Ver( pPipe->DelFrMulticast(this) );
	return true;
}

void CMulticast::Release()
{
	for (PipeMap_t::iterator it = m_mapPipe.begin();
		it != m_mapPipe.end();++it)
	{
		CSyncPipe* pPipe = it->first;
		Ver( pPipe->DelFrMulticast(this) );
	}
	m_mapPipe.clear();
	m_deqPipe.clear();
	delete this;
}

void CMulticast::SendCmd(const void* pData,size_t stSize)
{
	if(!pData || stSize <= 0)
		return;

	for ( size_t i=0; i<m_deqPipe.size();)
	{
		CSyncPipe* pPipe = m_deqPipe[i].first;

		bool bSendBufIsFull = false;

		//��Щ������ר��Ϊ�첽����⿼�ǵģ���������Ӱ�죬�Ժ�Ӧ��ȥ��
		CAsynPipeHandler* pHandler = dynamic_cast<CAsynPipeHandler*>( pPipe->GetHandler() );
		if( pHandler )
			bSendBufIsFull = pHandler->OutBufIsFull();

		if ( !bSendBufIsFull )
		{
			if( pPipe->Send(pData, stSize) == stSize )
			{
				++ i ;	//�����֡ɾ����pipe��deq���һ��pipe�ᱻ�ƶ�����ǰλ�ã���ôi�Ͳ�Ӧ��ƫ��.
				continue;
			}
		}

		const float fSpeed=pPipe->GetDataFlowSpeed();

		if( fSpeed > 3 )
		{
			CAddress addr;
			pPipe->GetRemoteAddress(addr);

			ostringstream strm;
			strm<< "size of data: " << stSize << endl
				<< "remote addr: " << addr.GetAddress() << endl
				<< "flow speed of last 10 sends: " << setprecision(3) << fSpeed << "kb/s" <<endl;

			LogErr( "�ಥʱ���ͻ����������", strm.str() );
		}

		pPipe->ShutDown(true);
		//�����֡ɾ����pipe��deq���һ��pipe�ᱻ�ƶ�����ǰλ�ã���ôi�Ͳ�Ӧ��ƫ��.
	}
}


void CMulticast::SetLocalID(size_t uLocalId)
{
	m_uLocalId = uLocalId;
}


bool CMulticast::Empty()const
{
	return m_mapPipe.empty();
}

