#include "stdafx.h"
#include "ThreadHelper.h"
#include "CAsynPipeReactor.h"
#include "CAsynPipeInBuffer.h"
#include "CAsynPipe.h"
#include "CPipeCallback.h"
#include "ExpHelper.h"
#include "TimeHelper.h"
#include "CAsynMulticast.h"
#include "CPipeThreadMgr.h"
#include "CPipeTrMsgBufferSwapper.h"
#include "ErrLogHelper.h"
#include "TSqrAllocator.inl"
#include "CPipeJob.inl"

#ifndef _WIN32
#include <fcntl.h>
#endif

/*
  Linux��ʹ��epoll������,windows����EventSelect����connect��listen,��overlapped������read��write

  ��һ��Socket�Լ�������fin�����ֽ��ܵ���һ��fin,��ô��Socket�Ϳ�������ͬ����������close

  ������Client�������ߣ������Ϳ��Լ���Server����TIMEWAIT_STAT��socket

  ����GetEvent,�κκ������ö��������������¼��ص���һ���̶ȱ����ջ�����
  ͬһ��Reactor�Ĳ�ͬ��Pipe����Ϣʱ���ǲ���֤�ģ�ͬһ��Pipe����Ϣʱ��һ����֤��
*/
uint32 CAsynPipeReactor::m_suThreadNum;

CAsynPipeReactor::CAsynPipeReactor()
	:m_uPipeNum(0)
								  //,m_bUserCanceled(false)
{
	if ( CPipeThreadMgr::Inst() )
		GenErr("AsyncPipeReactor can only have one instance!");

	CPipeThreadMgr::Init(m_suThreadNum, this);
	
#ifndef _WIN32
	m_fdEpoll=epoll_create(EPOLL_BACK_STORE_SIZE);
	if(-1 == m_fdEpoll)
	{
		ostringstream strm;
		strm<<"epoll_create failed with error:"<<strerror(errno);
		GenErr(strm.str());
	}
	int aryfd[2];
	if( -1 == pipe(aryfd) )
	{
		ostringstream strm;
		strm<<"pipe failed with error:"<<strerror(errno);
		GenErr(strm.str());
	}
	m_fdReadPipe=aryfd[0];
	m_fdWritePipe=aryfd[1];

	if( -1 == fcntl( m_fdReadPipe,F_SETFL,O_NONBLOCK ) )
	{
		ostringstream strm;
		strm<<"fcntl m_fdReadPipe failed with error:"<<strerror(errno);
		GenErr(strm.str());
	}

	if( -1 == fcntl( m_fdWritePipe,F_SETFL,O_NONBLOCK ) )
	{
		ostringstream strm;
		strm<<"fcntl m_fdWritePipe failed with error:"<<strerror(errno);
		GenErr(strm.str());
	}

	epoll_event ee;
	ee.data.ptr=NULL;
	ee.events=EPOLLIN;
	if(-1==epoll_ctl(m_fdEpoll,EPOLL_CTL_ADD,m_fdReadPipe,&ee))
	{
		ostringstream strm;
		strm<<"epoll_ctl add m_fdReadPipe failed with error:"<<strerror(errno);
		GenErr(strm.str());
	}
	

#else
	m_vecEvent.reserve(MAXIMUM_WAIT_OBJECTS);
	m_evCancelBlock=CreateEvent(NULL, FALSE, FALSE, NULL);
	if(!m_evCancelBlock)
		GenNetworkExp("CreateEvent Failed");
	m_vecEvent.push_back(m_evCancelBlock);
	
	for (size_t index = 0; index < m_suThreadNum; ++index)
	{
		HANDLE pThreadHandler = CreateEvent(NULL, FALSE, FALSE, NULL);
		m_vecThreadMsgHandler.push_back(pThreadHandler);
		m_vecEvent.push_back(pThreadHandler);
	}
#endif
}

CAsynPipeReactor::~CAsynPipeReactor()
{
	CPipeThreadMgr::Inst()->Halt();

	while( true )
	{
		if(CPipeThreadMgr::Inst()->Halted())
			break;
		
		GetEvent(uint32(-1));

		CPipeTrMsgBufferSwapper::Inst()->HandleAllRightMsg();
	}

	CPipeThreadMgr::Unit();
#ifndef _WIN32
	epoll_event ee;
	if( -1 == epoll_ctl(m_fdEpoll,EPOLL_CTL_DEL,m_fdReadPipe,&ee) )
	{
		ostringstream strm;
		strm<<"epoll_ctl del m_fdReadPipe failed with error:"<<strerror(errno);
		GenErr(strm.str());
	}	
	if( -1 == close( m_fdReadPipe ) )
	{
		ostringstream strm;
		strm<<"close m_fdReadPipe failed with error:"<<strerror(errno);
		GenErr(strm.str());
	}
	if( -1 == close( m_fdWritePipe ) )
	{
		ostringstream strm;
		strm<<"close m_fdWritePipe failed with error:"<<strerror(errno);
		GenErr(strm.str());
	}
	if( -1 == close( m_fdEpoll ) )
	{
		ostringstream strm;
		strm<<"close m_fdEpoll failed with error:"<<strerror(errno);
		GenErr(strm.str());
	}

#else
	CloseHandle(m_evCancelBlock);
	for (size_t index = 0; index < m_suThreadNum; ++index)
	{
		HANDLE pThreadHandler = m_vecThreadMsgHandler[index];
		CloseHandle(pThreadHandler);
	}
#endif
}

bool CAsynPipeReactor::Release()
{
	if ( m_uPipeNum>0 )
		return false;
	delete this;
	return true;
}

void CAsynPipeReactor::SetThreadNum(uint32 uThreadNum)
{
	m_suThreadNum = uThreadNum;
}

inline size_t CAsynPipeReactor::GetPipeNum()const
{
	return m_uPipeNum;
}

void CAsynPipeReactor::Register(IPipe* pPipe)
{
	CAsynPipe* pRealPipe = class_cast<CAsynPipe*>(pPipe);

	if(pRealPipe->m_pReactor)
		GenErr("IPipeReactor::Register: Pipe has been Registered.");
	
	pRealPipe->m_pReactor = this;

	pRealPipe->m_eState = eAPS_Connected;

	++m_uPipeNum;
}


void CAsynPipeReactor::Unregister(IPipe* pPipe)
{
	CAsynPipe* pRealPipe = class_cast<CAsynPipe*>(pPipe);

	if( !pRealPipe->m_pReactor )
		GenErr("IPipeReactor::Unregister: Pipe is not registered yet.");

	if( pRealPipe->m_pReactor!=this )
		GenErr("IPipeReactor::Unregister: Pipe is not registered in this reactor.");

	--m_uPipeNum;

	pRealPipe->m_pReactor=NULL;
}


uint32 CAsynPipeReactor::GetEvent(unsigned uWaitTime)
{
	CPipeTrMsgBufferSwapper::Inst()->FlushAllLeftMsg();

#ifndef _WIN32
	epoll_event aryEvents[EPOLL_MAX_EVENT_NUM];

	const int nEventCount=epoll_wait(m_fdEpoll,aryEvents,EPOLL_MAX_EVENT_NUM,uWaitTime);
	
	if( -1== nEventCount )
	{
		int nErrorCode=errno;
		if(nErrorCode==EINTR)//received a signal
			return eGER_Signal;
		ostringstream strm;
		strm<<"epoll_wait failed with error:"<<strerror(nErrorCode)<<"("<<nErrorCode<<")";
		GenErr(strm.str());
	}

	uint32 uResult = 0;

	char szBuffer[256];

	int32 nReadResult;

	do
	{
		nReadResult=read( m_fdReadPipe,szBuffer,sizeof(szBuffer) );

		if ( nReadResult == -1 )
		{
			switch( SocketGetLastError() )
			{
			case EINTR:
				uResult |= eGER_Signal;
			case EAGAIN:
				goto out;
			default:
				{
					ostringstream strm;
					strm<<"read m_fdReadPipe failed with error:"<<strerror(errno);
					GenErr(strm.str());
				}
			}
		}
		else
		{
			for( int i = 0; i < nReadResult; ++i )
			{
				const uint8 ch = (uint8)szBuffer[i];
				if (ch == (uint8)-1)
				{
					uResult |= eGER_Canceled;
				}
				else
				{
					uResult |= eGER_NetMsg;
				}
			}
		}
	}
	while(	nReadResult >= sizeof(szBuffer) );
out:

	if( uResult & eGER_NetMsg )
		CPipeTrMsgBufferSwapper::Inst()->HandleAllRightMsg();

	return uResult;
#else
	const uint32 uResult=MsgWaitForMultipleObjectsEx((uint32)m_vecEvent.size(),&m_vecEvent[0],
													 uWaitTime,QS_ALLINPUT, MWMO_ALERTABLE|MWMO_INPUTAVAILABLE );

	switch( uResult )
	{
	case WAIT_OBJECT_0:
		return eGER_Canceled;
	case WAIT_TIMEOUT:
		return eGER_TimedOut;
	case WAIT_IO_COMPLETION:
		return eGER_Iocp;
	case WAIT_FAILED:
		{
			ostringstream strm;
			strm<<"MsgWaitForMultipleObjectsEx failed and SocketGetLastError return:"<<SocketGetLastError();
			GenErr(strm.str());
		}
	default:
		{
			const uint32 uMsg=WAIT_OBJECT_0+m_vecEvent.size();
			if( uResult == uMsg  )
			{
				return eGER_SysMsg;
			}
			else if (uResult < uMsg || uResult > WAIT_OBJECT_0)
			{
				CPipeTrMsgBufferSwapper::Inst()->HandleAllRightMsg();
				return eGER_NetMsg;
			}
			else
			{
				GenErr("MsgWaitForMultipleObjectsEx return invalid result.");
			}
		}
	}

	return eGER_NetMsg;
#endif
}

IPipe* CAsynPipeReactor::CreateIPipe(IPipeHandler* pHandler,unsigned int uSendBufferSize,unsigned int uRecvBufferSize, uint8 uFlag)
{
	if( !pHandler )
		GenErr("Pipe must has a handler!");

	CAsynPipe* pPipe=new CAsynPipe(pHandler,uSendBufferSize,uRecvBufferSize);

	CPipeThread* pThread= CPipeThreadMgr::Inst()->GetThread(pPipe->GetThreadId());

	uRecvBufferSize/=100;

	if( (uFlag & ePCF_RecvWayDecompression) != 0 )
	{
		//���ջ���������ȷ��ͻ�����һ��chunk����ѹ����������ܵ�sizeҪ�󣬷����޷��������޷�����һ��������ѹ������
		//����ʹ��1.5��Ϊһ���������ݡ�����gzip��ѹ�����������ݲ����ܱ�ԭ���ݴ�1.5����

		const unsigned uDecompressedMaxSize=unsigned( WRITE_BUFFER_CHUNK_SIZE*1.5 );
		if( uRecvBufferSize < uDecompressedMaxSize )
			uRecvBufferSize = uDecompressedMaxSize;
	}
	else
	{
		if( uRecvBufferSize < 128 )
			uRecvBufferSize = 128;
	}

	++pThread->m_stPipeNum;

	(new(pThread) CPipeCreateJob(pPipe->GetLocalID(), uFlag, pThread,uSendBufferSize,uRecvBufferSize))
		->Add(pThread);

	Register(pPipe);
	return pPipe;
}


void CAsynPipeReactor::CancelBlock()
{
#ifdef _WIN32
	SetEvent(m_evCancelBlock);
#else
	uint8 uByte = uint8(-1);
 again:
	if( -1== write(m_fdWritePipe,&uByte,sizeof(uByte) ) )
	{
		int nResult=SocketGetLastError();
		switch(nResult)
		{
		case EAGAIN:
			return;
		case EINTR:
			goto again;
		default:
			{
				ostringstream strm;
				strm<<"write m_fdWritePipe failed with error:"<<SocketGetLastError();
				GenErr(strm.str());
			}
		}
	}	
#endif
}



IMulticast* CAsynPipeReactor::CreateMulticast()
{
	CAsynMulticast* pMulticast = new CAsynMulticast;

	size_t stSize = CPipeThreadMgr::Inst()->GetThreadSize();
	for(size_t i = 0; i < stSize; i++)
	{
		CPipeThread* pPool = CPipeThreadMgr::Inst()->GetThread(i);
		(new(pPool) CAddMulticastJob(pMulticast->GetLocalID(), pPool))->Add(pPool);
	}
	return pMulticast;
}

void CAsynPipeReactor::DestroyMulticast(IMulticast* pMulticast)
{
	pMulticast->Release();
}

void CAsynPipeReactor::PushPipeThreadMsg(uint32 uThreadId)
{
#ifdef _WIN32
	SetEvent(m_vecThreadMsgHandler[uThreadId]);
#else
	uint8 uByte = uint8(uThreadId);
 again1:
	if( -1== write(m_fdWritePipe,&uByte,sizeof(uByte) ) )
	{
		int nResult=SocketGetLastError();
		switch(nResult)
		{
		case EAGAIN:
			return;
		case EINTR:
			goto again1;
		default:
			{
				ostringstream strm;
				strm<<"write m_vecThreadEpollEvent[uThreadId].fdWritePipe failed with error:"<<SocketGetLastError();
				GenErr(strm.str());
			}
		}
	}	
#endif
}

