#include "stdafx.h"
#include "CFolObjFollowState.h"
#include "CFolMovement.h"
#include "CObjMoveStateClient.h"
#include "CoreCommon.h"
#include "ExpHelper.h"
#include "CPixelPath.h"
#include "CTimeSystemClient.h"
#include "CCoreObjectFollower.h"
#include "CoreObjectCommon.h"
#include "CoreObjectClientCommon.h"
#include "ICoreObjectFollowerHandler.h"
#include "CAppClient.h"
#include "IDistortedTime.h"
#include "CConnClient.h"
#include "PrtlGas2GacObjectDef.h"

#define Reference_Fol_Step_Cmd

#ifdef LOG_COREOBJ_MOVE
namespace sqr
{
	class CPrintMovedDist
		:public CTick
	{
	public:
		CPrintMovedDist(CFolObjFollowState* pFolState):m_pFolState(pFolState){}

		void OnTick()
		{
			CFolMovement* pCurMovent = m_pFolState->m_pCurMovement;
			if(!pCurMovent)
				return;
			ostringstream strm;
			strm << pCurMovent->GetRemoteMovedDist() << " " << pCurMovent->GetMovedDistWhenRemoteSuspended() << " " << pCurMovent->GetEstimatedServerTime() - pCurMovent->GetElapsedTime() << " " << pCurMovent->GetTimeErr() << endl;
			m_pFolState->m_pObj->LogMsg(strm.str());
		}

	private:
		CFolObjFollowState* m_pFolState;
	};
}
#endif

CFolObjFollowState::CFolObjFollowState(CFolMovement* pMm,float fSpeed,CCoreObjectFollower* pCoreObj)
:m_pCurMovement(pMm)
,m_fSpeedModifier(0)
,m_fRemoteLastPathMovedDist(0)
,m_pObj(pCoreObj)
,m_tcpNotifySpeedChanged(pCoreObj->GetDistortedTime()->GetDistortedProcessTime())
,m_tcpStep(m_tcpNotifySpeedChanged)
#ifdef LOG_COREOBJ_MOVE
,m_bNormalSpeed(true)
#endif
{
	//printf("init %f\n", fLastPathMovedDist);
	if(!pMm)
		GenErr("CFolObjFollowState must have a initial movement");

	if( m_pObj->GetCorrectionSpeedRatio() == 0 )
		return;	//��ȫ�ر��ٶ����书��				
#ifdef USE_RENDER_SLOT
	CRenderSlotOwner::Inst().Insert(this);
	//CAppClient::Inst()->RegisterTick(this,min( uint32(1000/fSpeed) , CTraitsClient::GetObjectMoveCyc() ) );
	//m_pObj->GetMovState()->CreateTickingDerived( m_pObj->GetMovState()->GetMoveCyc() );
#else
	CAppClient::Inst()->RegisterTick(this,min( uint32(1000/fSpeed) , CTraitsClient::GetObjectMoveCyc() ) );
#endif

#ifdef LOG_COREOBJ_MOVE
	m_pPrintTick = new CPrintMovedDist(this);
	CAppClient::Inst()->RegisterTick(m_pPrintTick, 200);
#endif
}

CFolObjFollowState::~CFolObjFollowState(void)
{
#ifdef LOG_COREOBJ_MOVE
	delete m_pPrintTick;
	m_pPrintTick = NULL;
#endif

#ifdef USE_RENDER_SLOT
	CRenderSlotOwner::Inst().Delete(this);
#else
	UnRegister();
#endif
	Ast( !m_pCurMovement );
}

float CFolObjFollowState::GetMovedDist(CObjMoveStateClient* pMovState)const
{
	if(!pMovState)
		return m_pCurMovement->GetPathLength();
	return pMovState->GetMovedDist();
}

//���follower����ͷ���˶�����ƶ��������,ֵΪ������˶������߽���-�ͻ��˶������߽��ȡ�
float CFolObjFollowState::GetTotalMovedDistError(CObjMoveStateClient* pMovState, uint32 uDelay)const
{
#ifdef Reference_Fol_Step_Cmd
	float fError=GetRemoteTotalMovedDist()-pMovState->GetMovedDist();
	
	//����Follower������ƶ��Ǳ������ӳ��˵ģ���˼�������ʱ��Ӧ�ð��ӳټ����ȥ��
	float fDelayDist=CalIdealDistByTime(uDelay);

	fError-=fDelayDist;

	return fError;
#endif

	float fRemoteTotalMovedDist = m_pCurMovement->GetRemoteMovedDist();
	for( list<CFolMovement*>::const_iterator it=m_lstPendingMovement.begin();it!=m_lstPendingMovement.end();++it )
	{
		fRemoteTotalMovedDist += (*it)->GetRemoteMovedDist();	
	}

	return  fRemoteTotalMovedDist - m_pObj->GetMovState()->GetMovedDist();
}

//����ָ��ʱ���ڣ�����˶����ڵ�ǰ�ƶ��ٶ��������ߵľ���
float CFolObjFollowState::CalIdealDistByTime(int32 nTime)const
{
#ifdef Reference_Fol_Step_Cmd
	return nTime*GetSpeed()*eGridSpanForObj/1000;
#endif
	return m_pCurMovement->CalIdealDistByTimeInMovement(nTime);
}

//��GetTotalMovedDistError��ȣ����������������ʱ������ʱ������
float CFolObjFollowState::GetEasyMovedDistError(CObjMoveStateClient* pMovState, uint32 uDelay)const
{
	float fError=GetEasyRemoteTotalMovedDist()-pMovState->GetMovedDist();
	
	//����Follower������ƶ��Ǳ������ӳ��˵ģ���˼�������ʱ��Ӧ�ð��ӳټ����ȥ��
	float fDelayDist=CalIdealDistByTime(uDelay);

	fError-=fDelayDist;
	//printf("GetEasyMovedDistError: fError %f\n", fError);
	return fError;
}

void CFolObjFollowState::ModifySpeed(float fSpeedModifier)
{
	//m_fSpeedModifier=0;
	//cout<<fSpeedModifier<<endl;
	m_fSpeedModifier=fSpeedModifier;
}

float CFolObjFollowState::GetRemoteTotalMovedDist()const
{
	float fTotalMovedDist;

	if(m_lstPendingMovement.empty())
	{
		//printf("empty %f\n", GetRemoteLastPathMovedDist());
		fTotalMovedDist=GetRemoteLastPathMovedDist();
	}
	else
	{

		fTotalMovedDist=m_pCurMovement->GetPathLength();

		for( list<CFolMovement*>::const_iterator it=m_lstPendingMovement.begin();it!=--m_lstPendingMovement.end();++it )
		{
			fTotalMovedDist+=(*it)->GetPathLength();
			//printf("pending path is %f\n", (*it)->GetPathLength());
		}

		fTotalMovedDist+=GetRemoteLastPathMovedDist();
	}
		
	return fTotalMovedDist;	//������ֵ�����ٲ����ȵ����ֵ
}

float CFolObjFollowState::GetEasyRemoteTotalMovedDist()const
{
	////�������̵�����ʱ��Ĳ�һ���������ͬ�ļ����TickҲ��������
	////0 �� TimeSystem����֡��֮����ô������˼������˾����ʱ��������

	uint32 uMaxTimeError=CTimeSystemClient::Inst()->GetBaseCyc();
	uint64 uCurTime = m_pObj->GetDistortedTime()->GetDistortedProcessTime();
	int32 nElapsedTime=int32(uCurTime - m_tcpStep)-uMaxTimeError;
	return GetRemoteTotalMovedDist()+CalIdealDistByTime(nElapsedTime);
}

float CFolObjFollowState::GetRemoteLastPathMovedDist()const
{
#ifdef Reference_Fol_Step_Cmd
	if(m_fRemoteLastPathMovedDist>=0)
	{
		return m_fRemoteLastPathMovedDist;
	}

	if(m_lstPendingMovement.empty())
	{
		//printf("pending empty, %f\n", m_pCurMovement->GetPathLength());
		return m_pCurMovement->GetPathLength();
	}

	//printf("else, %f\n", m_lstPendingMovement.back()->GetPathLength());
	return m_lstPendingMovement.back()->GetPathLength();
#endif

	CFolMovement* pMm = GetLastMovement();
	if(pMm->RemoteReached())
		return pMm->GetMovedDistWhenRemoteReached();

	if(pMm->GetMovedDistWhenRemoteSuspended() >= 0)
	{
		return pMm->GetMovedDistWhenRemoteSuspended();
	}
	
	return pMm->CalIdealDistByTimeInMovement(pMm->GetTimeErr() + int32(pMm->GetElapsedTime()));

}

CFPos CFolObjFollowState::GetServerObjectPixelPos()const
{
	float fRemoteLastPathMovedDist=GetRemoteLastPathMovedDist();
	CFolMovement* pMm= GetLastMovement();

	CFPos PixelPos;
	
	pMm->GetPixelPos(PixelPos,fRemoteLastPathMovedDist);

	return PixelPos;
}

CFolMovement* CFolObjFollowState::GetLastMovement()const
{
	return m_lstPendingMovement.empty() ? m_pCurMovement : m_lstPendingMovement.back();
}

void CFolObjFollowState::OnStepped(float fLastPathMovedDist)
{
	m_tcpStep = m_pObj->GetDistortedTime()->GetDistortedProcessTime();
	m_fRemoteLastPathMovedDist=fLastPathMovedDist;
}

bool CFolObjFollowState::RemoteObjectReached()const
{
#ifdef Reference_Fol_Step_Cmd
	return m_fRemoteLastPathMovedDist<0;
#endif
	return GetLastMovement()->RemoteReached();
}

bool CFolObjFollowState::RemoteObjectReachedCurPath()const
{
#ifdef Reference_Fol_Step_Cmd
	return !m_lstPendingMovement.empty() || RemoteObjectReached();
#endif
	return m_pCurMovement->RemoteReached();
}

//void CFolObjFollowState::SuspendMoving(const CGas2GacOC_Suspend_Follower_Moving* pCmd)
//{
//	GetLastMovement()->SetMovedDistWhenRemoteSuspended(pCmd->fMovedDist);
//	//Delete();
//}

//void CFolObjFollowState::ResumeMoving(uint32 uSuspendedTime)
//{
//	GetLastMovement()->SetMovedDistWhenRemoteSuspended(-1);
//	GetLastMovement()->AccumulateSuspendedTime(uSuspendedTime);
//	//InsertToStandardSlot();
//}

#ifdef USE_RENDER_SLOT
void CFolObjFollowState::AdjustSpeed(uint32 uMoveInterval)
#else
void CFolObjFollowState::AdjustSpeed()
#endif
{
	if( !m_pObj->GetMovState() )
		return;

	const float fRealSpeed=GetRealSpeed();
	const float fMaxDistError = eGridSpanForObj * fRealSpeed / 2;	//ÿ��4�׵��ƶ��ٶ���2��
	const float fMinDistError = eGridSpanForObj * fRealSpeed / 4;	//ÿ��4�׵��ƶ��ٶ���1��

	float fDelayDist = GetTotalMovedDistError(m_pObj->GetMovState(), m_pObj->GetMoveDelay());

	if( (-fDelayDist > fMaxDistError) && !RemoteObjectReachedCurPath() )
	{
		//�ͻ��˶����ߵ�̫�죬����ֹͣ������
		//�����SetSpeed��SetSpeed���п��ܴ����ص�������������Ҫ��
		SetSpeed(0);
#ifdef LOG_COREOBJ_MOVE
		//stall (fDelayDist, fMaxDistError)
		if(m_bNormalSpeed)
		{
			ostringstream strm;
			strm << "s(" << fDelayDist << "," << fMaxDistError << ")" << endl;
			m_pObj->LogMsg(strm.str());
		}
		m_bNormalSpeed = false;
#endif
		return;
	}
#ifdef Reference_Fol_Step_Cmd
	fDelayDist=GetEasyMovedDistError(m_pObj->GetMovState(), m_pObj->GetMoveDelay());
#endif

	if( fabs(fDelayDist) > fMinDistError )
	{
#ifdef USE_RENDER_SLOT
		float fSpeedModifier=fDelayDist/m_pObj->GetCorrectionSpeedRatio()/eGridSpanForObj;	//�������ʱ���������ٶ�
		//float fSpeedModifier=fDelayDist*1000000/uMoveInterval/eGridSpanForObj;	//�������ʱ���������ٶ�
#else
		float fSpeedModifier=fDelayDist/m_pObj->GetCorrectionSpeedRatio()/eGridSpanForObj;	//�������ʱ���������ٶ�
#endif

#ifdef LOG_COREOBJ_MOVE
		//speed adjust (fDelayDist,fMinDistError,fSpeedModifier,CurSpeed)
		if(m_bNormalSpeed)
		{
			ostringstream strm;
			strm << "sa(" << fDelayDist << "," << fMinDistError << "," << fSpeedModifier << "," << fRealSpeed <<
 ",";
			m_pObj->LogMsg(strm.str());
		}
		m_bNormalSpeed = false;
#endif
		ModifySpeed( fSpeedModifier );
	}
	else
	{
#ifdef LOG_COREOBJ_MOVE
		//restore speed
		if(!m_bNormalSpeed)
		{
			ostringstream strm;
			strm << "rs(" << GetRealSpeed() << "," << fDelayDist << "," << fMinDistError << ")" << m_pCurMovement->GetMovedDistWhenRemoteSuspended() << " remote " << m_pCurMovement->GetRemoteMovedDist() << endl;
			m_pObj->LogMsg(strm.str());
		}
		m_bNormalSpeed = true;
#endif
		ModifySpeed( 0 );
	}

	//if(GetRealSpeed() > GetIdealSpeed())
	//{
	//	cout << m_pObj->GetMovState()->GetMoveCyc() << " ";
	//	cout << "RealSpeed = " << GetRealSpeed() << " IdealSpeed() = " << GetIdealSpeed() << " RemoteMoveDistErr = " << this->GetTotalMovedDistError(m_pObj->GetMovState(), m_pObj->GetMoveDelay()) << " easyMoveDistErr = " << this->GetEasyMovedDistError(m_pObj->GetMovState(), m_pObj->GetMoveDelay()) << endl;
	//}
	//if(m_pObj->GetMovState()->GetSpeed() > GetRealSpeed())
	//	cout << "LastSpeed = " << m_pObj->GetMovState()->GetSpeed() << " curSpeed = " << GetRealSpeed() << " RemoteMoveDistErr = " << this->GetTotalMovedDistError(m_pObj->GetMovState(), m_pObj->GetMoveDelay()) << " easyMoveDistErr = " << this->GetEasyMovedDistError(m_pObj->GetMovState(), m_pObj->GetMoveDelay()) << endl;

	//�����SetSpeed��SetSpeed���п��ܴ����ص�������������Ҫ��
	//cout << "setspeed " << GetRealSpeed() << " " << GetSpeed() << " " << m_fSpeedModifier << endl;
	SetSpeed( GetRealSpeed() );
}

#ifdef USE_RENDER_SLOT
void CFolObjFollowState::OnUpdate(uint64 uLastFrameUsedTime)
{
	AdjustSpeed(uint32(uLastFrameUsedTime));
}
#else
void CFolObjFollowState::OnTick()
{
	AdjustSpeed();
}
#endif

void CFolObjFollowState::SetIdealSpeed(float fSpeed)
{
	//�����SetSpeed��SetSpeed���п��ܴ����ص�������������Ҫ��
	//if(!m_lstPendingMovement.empty())
	//{
	//	m_lstPendingMovement.back()->SetSpeed(fSpeed);
	//	return;
	//}

	if(m_pCurMovement)
		m_pCurMovement->SetSpeed(fSpeed);
	SetSpeed(fSpeed);
#ifdef USE_RENDER_SLOT
	AdjustSpeed(0);
#else
	AdjustSpeed();
#endif
}

float CFolObjFollowState::GetSpeed()const
{
	return m_pCurMovement?m_pCurMovement->GetSpeed():0.0f;
}

float CFolObjFollowState::GetRealSpeed()const
{
	float fRealSpeed=GetSpeed()+m_fSpeedModifier;
	if (fRealSpeed<0)
		return 0;
	return fRealSpeed;
}


void CFolObjFollowState::SetSpeed(float fSpeed)
{
	CObjMoveStateClient* pMovState = m_pObj->GetMovState();

	if( !pMovState) 
		return;

	float fOldSpeed=pMovState->GetSpeed();	

	//�����SetSpeed��SetSpeed���п��ܴ����ص�������������Ҫ��
#ifdef USE_RENDER_SLOT
	pMovState->IntSetSpeedDerived( fSpeed );
#else
	pMovState->SetSpeed( fSpeed );
#endif

	if( fOldSpeed == fSpeed )
		return;

	bool bNeedToNotify = false;

	if( abs(fSpeed)<0.0001f || abs( fSpeed - fOldSpeed ) / fSpeed > 0.25f )
	{
		bNeedToNotify = true;
	}
	else
	{
		uint64 uCurTime = m_pObj->GetDistortedTime()->GetDistortedProcessTime();
		if( uCurTime - m_tcpNotifySpeedChanged > 300 )
			bNeedToNotify = true;
	}

	if( !bNeedToNotify )
		return;

	ICoreObjectFollowerHandler* pHandler = m_pObj->GetHandler();
	if( !pHandler )
		return;

	m_tcpNotifySpeedChanged = m_pObj->GetDistortedTime()->GetDistortedProcessTime();

	SQR_TRY
	{
		pHandler->OnSpeedChanged();
	}
	SQR_CATCH( exp )
	{
		LogExp(exp);
	}
	SQR_TRY_END;
	
}
