#include "stdafx.h"
#include "CAoiThread.h"
#include "ThreadHelper.h"
#include "CAoiThreadMgr.h"
#include "CAoiJob.h"
#include "TimeHelper.h"
#include "ExpHelper.h"
#include <time.h>
#include "ErrLogHelper.h"
#include "CAppConfigServer.h"
#include "CThreadWatcher.h"
#include "CAoiResult.inl"
#include "CAoi2LogicThreadMsgBuffer.h"
#include "TraceHelper.h"

CAoiThread::CAoiThread(CAoiThreadMgr* pMgr): m_pThreadMgr(pMgr)
{
	CreateThread(&m_hThread,1024,&CAoiThread::Execute,this);

}

CAoiThread::~CAoiThread(void)
{
	JoinThread(&m_hThread);
}

namespace sqr
{
	static void OnAoiErrLogCallBack(const char* szError, CLogOwner* )
	{
		(new CAoiTellErrMsgResult(szError))->Add();
	}
}

void CAoiThread::Execute(void* pParam)
{
	return static_cast<CAoiThread*>(pParam)->DoExecute();
}

void CAoiThread::DoExecute()
{
	SetAltSigStackEnabled(true);

#ifdef _WIN32
	if(CatchErrorEnabled())
		_set_se_translator(TransESHFun);
#endif

#ifndef _WIN32
	SetLogCallBackFunc(OnAoiErrLogCallBack);
#endif

	volatile uint64 uTime=GetProcessTime();

	const char* szThreadName = "AoiThread";
	SetThreadName(szThreadName);
	AddWatchCpuThreadID(szThreadName,GetCurTID());
	WatchThread( m_hThread , szThreadName, &uTime, 30*1000, 2,1000 );

	SQR_TRY
	{
		LowerCurThreadPriority();

		for(;;)
		{
			uTime=GetProcessTime();
			
			switch( GetSemaphore( &m_pThreadMgr->m_smJobQueue , 200 ) )
			{
			case 1:
				continue;
			case 0:
				//���д�������Ż�������ͨ����Cancel֮ǰ����һЩԭ�ӱ������趨��������Щ������Ҫ����
				//����ÿ�ζ��������е���Ϣ����������Ҫ���ö������������ʵ�����˷ѵ�
				CAoi2LogicThreadMsgBuffer::Inst()->HandleAllLeftMsg();
				m_pThreadMgr->m_bEndThreads = m_pThreadMgr->HandleAllLeftMsg();

				CAoi2LogicThreadMsgBuffer::Inst()->FlushRightMsg();
				m_pThreadMgr->FlushRightMsg();
				break;
			default:
				{
					ostringstream strm;
					strm<<"GetSemaphore failed with error \""<<strerror(errno)<<"\"";
					GenErr("CAoiThread::DoExecute GetSemaphore Error", strm.str());
				}
			}

			if(m_pThreadMgr->m_bEndThreads)
				break;

		}

	}

	SQR_CATCH(exp)
	{
		LogExp(exp);
	}
	SQR_TRY_END;

	DoNotWatchThread( m_hThread );

#ifndef _WIN32
	SetLogCallBackFunc(NULL);
#endif

	SetAltSigStackEnabled(false);
}
