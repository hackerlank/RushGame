#pragma once
#include "TObjMoveState.h"
#include "TCoreObject.h"
#include "IPathFinding.h"
#include "CPixelPath.h"
#include "ExpHelper.h"
#include "BaseMath.h"
#include "CoreObjectCommon.h"
#include "CoreCommon.h"
#include <iomanip>
#include "CDistortedTimeObj.h"
#include "CLogCoreObject.h"
#include "CoreObjectCommon.h"
#include "CTimeCheckPoint.h"

template<typename Traits>
TObjMoveState<Traits>::TObjMoveState( ImpCoreObject_t* pObj,CPixelPath* pPath,float fSpeed, float fBeginDist)
:m_pObject( pObj )
,m_pPath( NULL )
,m_fSpeed( 0 )
,m_fMovedDist( 0 )
,m_fEndDist( 0 )
{
	IntSetNewPath( pPath,fBeginDist );
}


template<typename Traits>
TObjMoveState<Traits>::~TObjMoveState()
{
	m_pPath->Release();
	m_pPath=NULL;
	//cout << " DeleteTickingDerived  ~TObjMoveState" << endl;
	//TimeSystem_t::Inst()->UnRegister(this);
}

template<typename Traits>
void TObjMoveState<Traits>::SetNewPathDerived(CPixelPath* pPath,float fBeginDist)
{
	IntSetNewPath(pPath, fBeginDist);
}

template<typename Traits>
void TObjMoveState<Traits>::IntSetNewPath(CPixelPath* pPath,float fBeginDist)
{
	m_pPath->Release();
	m_pPath = pPath;
	m_fMovedDist = fBeginDist;

	ResetUpdatePosTcp();

	m_fEndDist = m_pPath->GetPathPixelLength();

	CFPos TempPos;
	m_uCurWayPoint=m_pPath->GetPixelPosOfPath(TempPos,fBeginDist);

	Ast( m_uCurWayPoint != uint16(-1) );
	Ast( fBeginDist <= m_fEndDist );

	/*if( static_cast<ImpCoreObject_t*>( m_pObject )->GetType() == eCOT_Follower )
	{
		CFPos Begin,End;

		m_pPath->GetBeginPixelPos(Begin);
		m_pPath->GetEndPixelPos(End);

		printf("DoMove BeginPos(%f,%f) EndPos(%f,%f) EndDist(%f)\n",Begin.x,Begin.y,End.x,End.y,m_fEndDist);
	}*/
}

template<typename Traits>
bool TObjMoveState<Traits>::NeedSkipFrameDerived()const
{
	return false;
}

template<typename Traits>
float TObjMoveState<Traits>::IntGetNewMoveDistDerived()const
{
	return m_fMovedDist + sqr::CalMoveStepInMicroSeconds( m_fSpeed, static_cast<const ImpObjMoveState_t*>(this)->GetMoveCycDerived() );
}

template<typename Traits>
float TObjMoveState<Traits>::GetNewMoveDist()const
{
	return static_cast<const ImpObjMoveState_t*>(this)->IntGetNewMoveDistDerived();
}

template<typename Traits>
uint32 TObjMoveState<Traits>::GetMoveCyc()const
{
	return static_cast<const ImpObjMoveState_t*>(this)->GetMoveCycDerived();
}

template<typename Traits>
void TObjMoveState<Traits>::SetMoveCyc(uint32 uMoveCyc)
{
	static_cast<ImpObjMoveState_t*>(this)->SetMoveCycDerived(uMoveCyc);
}

template<typename Traits>
void TObjMoveState<Traits>::CreateTickingDerived(uint32 uCyc)
{
}

template<typename Traits>
void TObjMoveState<Traits>::DeleteTickingDerived()
{
}

template<typename Traits>
void TObjMoveState<Traits>::OnTick()
{
	//���GetInterval��0��������֡���ܻ������ѭ��������һ��Ҫ�ж�
	if( static_cast<ImpObjMoveState_t*>(this)->NeedSkipFrameDerived() && static_cast<ImpObjMoveState_t*>(this)->GetInterval() )
	{
		return;
	}

	CFPos NewPixelPos;

	const float fNewMoveDist = GetNewMoveDist();
	//static CTimeCheckPoint tcpMoveTick;
	//cout << "step " << fNewMoveDist << " time " << tcpMoveTick.GetElapse() << " cyc " << GetMoveCyc() << " speed " << m_fSpeed << endl;
	//tcpMoveTick.SetCheckPoint();
	//if(fNewMoveDist > 10) cout << "%$@))(@#$****** big step " << endl << endl;

	CPos GridPos;
	PixelPos2GridPos(GridPos,m_pObject->GetPixelPos());

	if( fNewMoveDist > m_fEndDist )
	{
		m_fMovedDist = m_fEndDist;
	}
	else
	{
		m_fMovedDist = fNewMoveDist;
	}

	uint16 uNewWayPoint= m_pPath->GetPixelPosOfPath( NewPixelPos, m_fMovedDist );
	//cout << GetObject()->GetLocalID() << " movedist = " << m_fMovedDist << " pathlen = " << GetPathLength() << "\t" << NewPixelPos.x / eGridSpan << "," << NewPixelPos.y / eGridSpan << endl;

	//cout << endl << " fNewMoveDist = " << fNewMoveDist << endl;
	//cout << " pos = " << NewPixelPos.x << " " << NewPixelPos.y << endl;	
	Ast( uNewWayPoint != uint16(-1) );

	//printf("%d\n",(uint32)m_tcpThisPath.GetElapse());
	//m_tcpThisPath.SetCheckPoint();

	//֪ͨ�����࣬������һ����
	//���������Dictator����Mediator�����п��ܴ���StepFollower��������Ϣ
	//���������Director�����п��ܴ���StepMediator��������Ϣ
	const CFPos OldPixelPos=m_pObject->GetPixelPos();

	//cout << "CoreObjPos Diff: " << NewPixelPos.x - OldPixelPos.x << " " << NewPixelPos.y - OldPixelPos.y << endl;
	//const float fRatio = eGridSpanForObj / eGridSpan;
	//cout << "RefreshRenderObjectPos " << NewPixelPos.x / fRatio << " " << NewPixelPos.y / fRatio << endl;
	//cout << "RenderObjPos Diff: " << NewPixelPos.x / fRatio - OldPixelPos.x / fRatio << " " 
	//<< NewPixelPos.y / fRatio - OldPixelPos.y / fRatio << endl;

	//ostringstream strm;
	//strm << "ObjMoveState::OnTick " << NewPixelPos.x << "," << NewPixelPos.y;
	//static_cast<ImpCoreObject_t*>(m_pObject)->LogMsg(strm.str());

	m_pObject->IntSetPosition( NewPixelPos );
	static_cast<ImpCoreObject_t*>(m_pObject)->OnMoveSteppedDerived( OldPixelPos );

	if( m_fMovedDist == m_fEndDist )
	{
#ifdef LOG_COREOBJ_MOVE
		CFPos beginPathPos, endPathPos;
		m_pPath->GetBeginPixelPos(beginPathPos);
		m_pPath->GetEndPixelPos(endPathPos);
		ostringstream strm;
		strm << "MoveState end: " << setprecision(12) << m_pObject->GetPixelPos().x << "," << m_pObject->GetPixelPos().y << " CalcPos = " << NewPixelPos.x << "," << NewPixelPos.y << " fMovedDist = " << m_fMovedDist << " fEndDist = " << m_fEndDist << " path begin pos = " << beginPathPos.x << "," << beginPathPos.y << " path end pos = " << endPathPos.x << "," << endPathPos.y;
		const PathList& pathList = m_pPath->GetGridPath()->GetPath();
		for(uint32 u = 0; u < pathList.size(); ++u)
			strm << " " << pathList[u].x << "," << pathList[u].y;
		static_cast<ImpCoreObject_t*>(m_pObject)->LogMsg(strm.str());
#endif
		m_pObject->IntOnMoveEnded();
	}
	else
	{
		if( uNewWayPoint != m_uCurWayPoint )
		{
			m_uCurWayPoint=uNewWayPoint;
			static_cast<ImpCoreObject_t*>(m_pObject)->OnWayPointChangedDerived();
		}

		CheckLastStep();
	}
}

template<typename Traits>
typename TObjMoveState<Traits>::ImpCoreObject_t* TObjMoveState<Traits>::GetObject()const
{
	return m_pObject;
}

template<typename Traits>
float TObjMoveState<Traits>::GetSpeed()const
{
	return m_fSpeed;
}

template<typename Traits>
void TObjMoveState<Traits>::IntSetSpeedDerived(float fSpeed)
{
	m_fSpeed = fSpeed;

	uint32 uMoveCyc;
	if (fSpeed == 0)
	{
		uMoveCyc = 0;
	}
	else
	{
		uint32 uMoveTime = uint32 (g_uPlayerAoiPrecision*1000/m_fSpeed);	//��λ�����ƶ�����ʱ��
		uMoveCyc = min( uMoveTime, Traits::GetObjectMoveCyc() ) * 1000;
	}
	SetMoveCyc(uMoveCyc);
	//cout << "setspeed movecyc " << uMoveCyc << endl;

	if( GetMoveCyc() == 0 )
	{
		static_cast<ImpObjMoveState_t*>(this)->DeleteTickingDerived();
	}
	else
	{
		if( !CheckLastStep() )
		{
			static_cast<ImpObjMoveState_t*>(this)->CreateTickingDerived( GetMoveCyc()/1000 );
		}
	}
}

template<typename Traits>
bool TObjMoveState<Traits>::SetSpeed(float fSpeed)
{
	if( fSpeed == m_fSpeed )
		return false;

	//�����ٶȱ仯������ˢ�¶����λ�ã���������tcpStep,�����´μ���CurDist��ʱ�򣬻���ͬһ��ʱ���ȼ������ֲ�ͬ���ٶ��ƶ��ľ��롣
	UpdateCurPosByRealTime();
	
	static_cast<ImpObjMoveState_t*>(this)->IntSetSpeedDerived( fSpeed );
	return true;
}

template<typename Traits>
bool TObjMoveState<Traits>::CheckLastStep()
{
	const float fNewMoveDist = GetNewMoveDist();

	const float fPathLength = GetPathLength();

	uint32 uInterval;

	if( fNewMoveDist <= fPathLength )
		return false;

	//ʣ���·�����Ȳ���һ��Step
	const float fRealMovedDist = GetRealMovedDist();
	if( fRealMovedDist >= fPathLength )
	{
		//Tick�Ѿ��ͺ����أ�Ӧע��һ�����Ϊ0��Tickʹ�������ص���
		uInterval = 0;
	}
	else
	{
		//ע��һ������պôﵽĿ���tick
		uInterval = uint32((fPathLength-m_fMovedDist)*1000/(m_fSpeed*eGridSpanForObj)+0.5f);
	}

	static_cast<ImpObjMoveState_t*>(this)->CreateTickingDerived( uInterval );
	//TimeSystem_t::Inst()->Register( this, uInterval );
	return true;	
}

template<typename Traits>
float TObjMoveState<Traits>::GetMovedDist()const
{
	return m_fMovedDist;
}

template<typename Traits>
float TObjMoveState<Traits>::GetRealMovedDist()const
{
	return GetObject()->GetRealMovedDistDerived();
}

template<typename Traits>
float TObjMoveState<Traits>::IntGetRealMovedDist()const
{
	const uint32 uElapsedTime = static_cast<uint32>(m_pObject->GetDistortedTimeObj()->GetDistortedProcessTime()  - m_uThisPathExpiredTime);
	const float fRealDist=/*GetMovedDist() + */ m_fLastUpdateDist + sqr::CalMoveStep( GetSpeed(), uElapsedTime );
	return limit2( fRealDist, GetMovedDist(), GetPathLength() );
}

template<typename Traits>
void TObjMoveState<Traits>::GetRealCurPos(CFPos& PixelPos)
{
	float fIdealMovedDist = GetRealMovedDist();
	float fMaxMovedDist = GetPathLength();
	if(fIdealMovedDist > fMaxMovedDist)
		fIdealMovedDist  = fMaxMovedDist; 

	VerNe( GetPath()->GetPixelPosOfPath(PixelPos,fIdealMovedDist),uint16(-1) );
}

template<typename Traits>
bool TObjMoveState<Traits>::SetEndDist(float fEndDist)
{
	const float fLimittedEndDist=limit2( fEndDist, m_fMovedDist, m_pPath->GetPathPixelLength() );
	m_fEndDist = fLimittedEndDist;
	
	//ÿ������EndDist��ʱ����Ҫ�����һ��step�ǲ��Ǿ��ǵ�ǰ·�������һ��,��������һ�����ܵ�MoveState��һ��uMoveCyc��ִ��step
	CheckLastStep();	
	return fLimittedEndDist == fEndDist;
}

template<typename Traits>
bool TObjMoveState<Traits>::SetReachDist(float fReachDist)
{
	if(fReachDist < 0)
		GenErr("ReachDist < 0");

	return SetEndDist( m_pPath->GetPathPixelLength() - fReachDist );
}

template<typename Traits>
float TObjMoveState<Traits>::GetPathLength()const
{
	return m_fEndDist;
}

template<typename Traits>
bool TObjMoveState<Traits>::Reached()const
{
	return m_fEndDist == m_fMovedDist;
}


template<typename Traits>
const CPixelPath* TObjMoveState<Traits>::GetPath()const
{
	return m_pPath;
}


template<typename Traits>
void TObjMoveState<Traits>::GetCurWayPoint(CFPos& PixelPos)const
{
	Ver( m_pPath->GetWayPointPixelPosOfPath(PixelPos,m_uCurWayPoint+1) );
}

template<typename Traits>
void TObjMoveState<Traits>::UpdateCurPosByRealTime()
{
	float fIdealMovedDist = GetRealMovedDist();

	CFPos ChangePos;

	const float fMaxMoveDist = GetNewMoveDist();

	//��ȷ����Ĳ������ܳ���һ��step,��������aoi���㾫�Ȳ���,���һЩaoi�¼���ʧ
	if( fIdealMovedDist > fMaxMoveDist )
		fIdealMovedDist = fMaxMoveDist;

	VerNe( GetPath()->GetPixelPosOfPath(ChangePos,fIdealMovedDist),uint16(-1) );

#ifdef LOG_COREOBJ_MOVE
	ostringstream strm;
	strm << "UpdateCurPosByRealTime " << setprecision(12) << m_pObject->GetPixelPos().x << "," << m_pObject->GetPixelPos().y << " -> " << ChangePos.x << "," << ChangePos.y;
	static_cast<ImpCoreObject_t*>(m_pObject)->LogMsg(strm.str());
#endif

	m_pObject->IntSetPosition( ChangePos );

	//cout << GetObject()->GetLocalID() << " u movedist = " << fIdealMovedDist << " pathlen = " << GetPathLength() << "\t" << ChangePos.x / eGridSpan << "," << ChangePos.y / eGridSpan << endl;
	m_fMovedDist = fIdealMovedDist;
	
	ResetUpdatePosTcp();
}


template<typename Traits>
void TObjMoveState<Traits>::UpdateCurPosByMovedDist(float fMovedDist)
{
	//Ast(fMovedDist > 0);

//	Ast(fMovedDist <= m_fMovedDist);
	if(fMovedDist > m_fEndDist)
	{
		ostringstream strm;
#ifdef LOG_COREOBJ_MOVE
		strm << "updateCurPosByMovedDist fMovedDist > m_fEndDist " << fMovedDist << " > " << m_fEndDist;
		m_pObject->LogMsg(strm.str());
		return;
#else
		strm <<  fMovedDist << " > " << m_fEndDist;
		GenErr("updateCurPosByMovedDist fMovedDist > m_fEndDist ", strm.str());
#endif
	}

#ifdef LOG_COREOBJ_MOVE
	ostringstream strm;
	strm << "updateCurPosByMovedDist " << m_fMovedDist << "->" << fMovedDist;
	m_pObject->LogMsg(strm.str());
#endif
	CFPos ChangePos;
	//cout <<  << endl;
	VerNe( GetPath()->GetPixelPosOfPath(ChangePos,fMovedDist), uint16(-1) );
	Ver(m_pObject->IntSetPosition( ChangePos ));
	m_fMovedDist = fMovedDist;
}

template<typename Traits>
void TObjMoveState<Traits>::ResetUpdatePosTcp()
{
	m_fLastUpdateDist = m_fMovedDist;
	//m_tcpThisPath.SetCheckPoint();
	m_uThisPathExpiredTime = m_pObject->GetDistortedTimeObj()->GetDistortedProcessTime();
}
