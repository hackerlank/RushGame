#include "stdafx.h"
#include "CConnMgrServer.h"
#include "CConnServer.h"
#include "BaseHelper.h"
#include "IConnMgrServerHandler.h"
#include "CAppConfigServer.h"
#include "CThreadWatcher.h"
#include "ExpHelper.h"
#include "ErrLogHelper.h"
#include "TSqrAllocator.inl"
#include "CCoreConnServerJob.h"
#include "CCoreServerJob.inl"

CConnMgrServer::CConnMgrServer()
{
	m_pHandler					= NULL;
	m_eServiceState				= eServiceState_Off;
	m_bDetectedDeadLoopEnabled	= false;
	m_uConnNum					= 0;
}

CConnMgrServer::~CConnMgrServer(void)
{
}

CConnMgrServer*& CConnMgrServer::IntInst()
{
	static CConnMgrServer* ls_pInst = NULL;;
	return ls_pInst;;
}

CConnMgrServer* CConnMgrServer::Inst()
{
	return IntInst();
}


void CConnMgrServer::SetHandler(IConnMgrServerHandler* pHandler)
{
	AdvDelRef(m_pHandler);
	m_pHandler=pHandler;
	AdvAddRef(m_pHandler);
}


IConnMgrServerHandler* CConnMgrServer::GetHandler()const
{
	return m_pHandler;
}

void CConnMgrServer::BeginService(const CAddress& addr,uint32 uSendBufferSize,uint32 uRecvBufferSize)
{
	//ֻ���ڷ��������׹رյ�����²�����������
	if (m_eServiceState != eServiceState_Off)
	{
		const string sCurState = sServiceStateMap.mServiceStateName[m_eServiceState];
		ostringstream strm;
		strm<<"CConnMgrServer��ǰ״̬�ǣ� "<<sCurState.c_str()<<"����״̬�ڲ��� BeginService ��"<<endl;
		GenErr(strm.str());
		return;
	}

	(new CCoreBeginServiceJob(addr, uSendBufferSize, uRecvBufferSize))->Add();
#ifndef _WIN32	
	SetLogCallBackFunc(&CConnMgrServer::SendErrMsgToConn);
#endif	
	m_eServiceState = eServiceState_Starting;
}

void CConnMgrServer::EndService()
{
	//���������ѹرջ������ڹرչ����в������ٴε���EndService
	if ( m_eServiceState == eServiceState_Off || m_eServiceState == eServiceState_Shutting )
	{
		const string sCurState = sServiceStateMap.mServiceStateName[m_eServiceState];
		ostringstream strm;
		strm<<"CConnMgrServer��ǰ״̬�ǣ� "<<sCurState.c_str()<<"����״̬�ڲ��� EndService ��"<<endl;
		GenErr(strm.str());
		return;
	}

	(new CCoreEndServiceJob())->Add();
#ifndef _WIN32	
	SetLogCallBackFunc(NULL);
#endif	
	m_eServiceState = eServiceState_Shutting;

	if( m_bDetectedDeadLoopEnabled )
		CThreadWatcher::Inst()->End();
}

void CConnMgrServer::OnBeginServiceSucceeded()
{
	Ast ( GetHandler() );

	m_eServiceState = eServiceState_On;
	GetHandler()->OnBeginServiceSucceeded();

	if( CAppConfigServer::Inst()->InfiniteLoopDetectionEnabled() && CatchErrorEnabled() )
	{
		m_bDetectedDeadLoopEnabled = true;
		CThreadWatcher::Inst()->Begin();
	}
}

void CConnMgrServer::OnBeginServiceFailed(const char* szErrMsg)
{
	Ast (GetHandler());
	GetHandler()->OnBeginServiceFailed(szErrMsg);
}

void CConnMgrServer::OnServiceEnded()
{
	Ast (GetHandler());
	m_eServiceState = eServiceState_Off;
	GetHandler()->OnServiceEnded();

	m_uConnNum = 0;
}


uint32 CConnMgrServer::Send(const void* pData,uint32 uSize)
{
	if (uSize == 0)
		return 0;
	(new CCoreSendMsgJob(pData, uSize))->Add();
	return uSize;
}

void CConnMgrServer::SendErrMsgToConn(const char* szError, CLogOwner* pOwner)
{
	if(pOwner == NULL)
	{
		(new CCoreSendErrMsgToAllConnJob(szError))->Add();
	}
	else
	{
		CConnServer* pConnServer = dynamic_cast<CConnServer*>(pOwner);
		if(!pConnServer)
			return;

		pConnServer->SendErrMsg(szError);
	}
}

uint32 CConnMgrServer::GetConnNum()const
{
	return m_uConnNum;
}

