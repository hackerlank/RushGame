#include "stdafx.h"
#include "CPipeThread.h"
#include "CPipeThreadMgr.h"
#include "CSyncPipeReactor.h"
#include "TimeHelper.h"
#include "CPipeState_Busy.h"
#include "CSyncPipe.h"
#include "ExpHelper.h"
#include "PatternMQ.h"
#include "CPipeCallback.h"
#include "CMulticast.h"
#include "TSqrAllocator.inl"
#include "ErrLogHelper.h"


#ifndef _WIN32
#include <fcntl.h>
#endif


/*
Linux��ʹ��epoll������,windows����EventSelect����connect��listen,��overlapped������read��write

��һ��Socket�Լ�������fin�����ֽ��ܵ���һ��fin,��ô��Socket�Ϳ�������ͬ����������close

������Client�������ߣ������Ϳ��Լ���Server����TIMEWAIT_STAT��socket

���û�����ShutDown��ʱ�򲻲���OnConnectedFailed�ص�
���û�����ShutDown��ʱ�򲻲���OnDisconnected�ص�,��Ϊ��ShutDown����֮ǰ�������Ѿ����׶Ͽ�����������֪ͨ��
����GetEvent,�κκ������ö��������������¼��ص���һ���̶ȱ����ջ�����
ͬһ��Reactor�Ĳ�ͬ��Pipe����Ϣʱ���ǲ���֤�ģ�ͬһ��Pipe����Ϣʱ��һ����֤��
*/

CSyncPipeReactor::CSyncPipeReactor( bool bSyncCallback )
{
	m_uPipeNum=0;

#ifdef _WIN32
	m_pCallingBackPipe=NULL;
#endif

	if( !bSyncCallback )
		m_pMQMsgMgr= new CPtMQMsgMgr( 1024 );

	m_bHasEvent = false;

	m_uLastCheckTime = GetProcessTime();

#ifndef _WIN32
	m_fdEpoll=epoll_create(EPOLL_BACK_STORE_SIZE);
	if( -1 == m_fdEpoll )
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
	m_vecState.reserve(MAXIMUM_WAIT_OBJECTS-1);
	m_evCancelBlock=CreateEvent(NULL,FALSE,FALSE,NULL);
	if(!m_evCancelBlock)
		GenNetworkExp("CreateEvent");
	m_vecEvent.push_back(m_evCancelBlock);
#endif
}


CSyncPipeReactor::~CSyncPipeReactor(void)
{
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
#endif

	if( m_pMQMsgMgr )
	{
		m_pMQMsgMgr->Release();
		m_pMQMsgMgr=NULL;
	}
}

bool CSyncPipeReactor::Release()
{
	//���ڵ���GetEvent��Reactor��������Pipeע�������е�Reactor�����ܱ�ɾ����
	if( (m_uPipeNum>0) )
		return false;
	delete this;
	return true;
}

inline size_t CSyncPipeReactor::GetPipeNum()const
{
	return m_uPipeNum;
}


void CSyncPipeReactor::Register(IPipe* pPipe)
{
	CSyncPipe* pRealPipe=class_cast<CSyncPipe*>(pPipe);

	if( pRealPipe->m_pReactor )
		GenErr("IPipeReactor::Register: Pipe has been registered.");

	if( !pRealPipe->m_pHandler )
		GenErr("Handler is NULL when register to reactor!");

	pRealPipe->m_pReactor=this;
	if( m_pMQMsgMgr )
		pRealPipe->SetMgr(m_pMQMsgMgr);

	++m_uPipeNum;

	pRealPipe->m_pState->Register();
}


void CSyncPipeReactor::Unregister(IPipe* pPipe)
{
	CSyncPipe* pRealPipe = class_cast<CSyncPipe*>(pPipe);

	if( !pRealPipe->m_pReactor )
		GenErr( "IPipeReactor::Unregister: Pipe is not registered yet." );

	if( pRealPipe->m_pReactor!=this )
		GenErr( "IPipeReactor::Unregister: Pipe is not registered in this reactor." );

	//���û����䣬�մ�ĳ��Reactor����Unregister������Pipe����Ȼ�����յ��¼���
	pRealPipe->CPtMQMsgOwner::ClearMsg();

	--m_uPipeNum;

	pRealPipe->m_pState->Unregister();
	pRealPipe->m_pReactor=NULL;

}

#ifndef _WIN32
bool CSyncPipeReactor::ClearControlPipeData()
{
	//����Cancel��ר��PipePair	
	char szBuffer[32];

	bool bSignaled=false;

	for(;;)
	{
		int32 nResult=read( m_fdReadPipe,szBuffer,sizeof(szBuffer) );
		if ( nResult == -1 )
		{
			switch( SocketGetLastError() )
			{
			case EINTR:
				bSignaled = true;
			case EAGAIN:
				break;
			default:
				{
					ostringstream strm;
					strm<<"read m_fdReadPipe failed with error:"<<strerror(errno);
					GenErr("read m_fdReadPipe failed", strm.str());
				}
			}
		}
		else
		{
			if( nResult < sizeof(szBuffer) )
				break;
		}
	}

	return bSignaled;
}
#endif

uint32 CSyncPipeReactor::GetEvent(unsigned uWaitTime)
{
	m_uLastCheckTime = GetProcessTime();
#ifndef _WIN32

	uint32 uResult=0;

	epoll_event aryEvents[EPOLL_MAX_EVENT_NUM];

	const int nEventCount= epoll_wait( m_fdEpoll,aryEvents,EPOLL_MAX_EVENT_NUM,uWaitTime );

	if( -1==nEventCount )
	{
		int nErrorCode=errno;
		if( nErrorCode==EINTR )//received a signal
			return eGER_Signal;

		ostringstream strm;
		strm<<"epoll_wait failed with error:"<<strerror(nErrorCode)<<"("<<nErrorCode<<")";
		GenErr(strm.str());
	}

	for( int i=0 ; i<nEventCount ; )
	{
		SQR_TRY
		{
			for( ; i<nEventCount ; ++i )
			{
				epoll_event& e=aryEvents[i];

				if( !e.data.ptr )
				{
					//����Cancel��ר��PipePair
					uResult |= eGER_Canceled;

					if( ClearControlPipeData() )
						uResult |= eGER_Signal;
				}
				else
				{
					static_cast<CPipeState_Busy*>(e.data.ptr)->
						ProcessEvent(e.events&EPOLLIN,e.events&EPOLLOUT,e.events&EPOLLERR);
				}
			}
		}
		SQR_CATCH(exp)
		{
			++i;
			LogExp(exp);
		}
		SQR_TRY_END;
	}

	if( DispatchEvent() )
		uResult |= eGER_NetMsg;

	return uResult ? uResult : eGER_TimedOut;
#else
	//����Connect�ɿ���ֱ�Ӳ���OnConnectedFailed�¼���������ɷ���ֱ�ӽ�
	//�������MsgWaitForMultipleObjectsEx�������OnConnectFailed�¼����ӳ١�

	if( DispatchEvent() )
		return eGER_NetMsg;
	
	uint32 uResult=MsgWaitForMultipleObjectsEx((uint32)m_vecEvent.size(),
		&m_vecEvent[0],uWaitTime,QS_ALLINPUT,MWMO_ALERTABLE|MWMO_INPUTAVAILABLE );

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

			if( (uResult < uMsg)  &&  (uResult >= WAIT_OBJECT_0) )
			{
				uint32 uIndex=uResult-WAIT_OBJECT_0;
				WSANETWORKEVENTS NetworkEvents;
				CPipeState_Busy* pState=class_cast<CPipeState_Busy*>( m_vecState[uIndex-1]->GetPipeState() );

				if( WSAEnumNetworkEvents( pState->m_Socket , m_vecEvent[uIndex] , &NetworkEvents ) )
				{
					ostringstream strm;
					strm << "WSAEnumNetworkEvents failed with error code:" << SocketGetLastError();
					GenErr(strm.str());
				}
				long lEvents=NetworkEvents.lNetworkEvents;
				//����ֻ��connect��accept��Ϊ��Ҫͨ��eventselect�������������
				//Ҳֻ�ж��������¼���read��write�Ľ�ͨ��complection routine������
				SQR_TRY
				{
					pState->ProcessEvent( (lEvents&FD_ACCEPT)!=0 , (lEvents&FD_CONNECT)!=0 , (NetworkEvents.iErrorCode[FD_CONNECT_BIT])!=0 );
				}
				SQR_CATCH(exp)
				{
					LogExp(exp);
				}
				SQR_TRY_END;
			}
			else if( uResult == uMsg )
			{
				return eGER_SysMsg;
			}
			else
			{
				GenErr("MsgWaitForMultipleObjectsEx return invalid result.");
			}
		}
	}
	DispatchEvent();

	return eGER_NetMsg;
#endif
}

bool CSyncPipeReactor::DispatchEvent()
{
	if( !m_pMQMsgMgr)
	{
		bool bHasEvent = m_bHasEvent;
		m_bHasEvent= false;			
		return bHasEvent;
	}
	return m_pMQMsgMgr->FlushMsg();
}


IPipe* CSyncPipeReactor::CreateIPipe(IPipeHandler* pHandler,unsigned int uSendBufferSize,unsigned int uRecvBufferSize, uint8 uFlag)
{
	if( !pHandler )
		GenErr("Pipe must has a handler!");

	CSyncPipe* pPipe=new CSyncPipe(pHandler,uSendBufferSize,uRecvBufferSize, uFlag);

	Register(pPipe);
	return pPipe;
}


void CSyncPipeReactor::CancelBlock()
{
#ifdef _WIN32
	SetEvent(m_evCancelBlock);
#else
	uint8 uByte;
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


IMulticast* CSyncPipeReactor::CreateMulticast()
{
	CMulticast* m_pMulticast = new CMulticast;
	return m_pMulticast;
}

void CSyncPipeReactor::DestroyMulticast(IMulticast* pMulticast)
{
	pMulticast->Release();
}
