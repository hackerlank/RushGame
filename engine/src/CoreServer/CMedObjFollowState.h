#pragma once
//$id$
//�ٲö����ڸ���״̬ʱ��״̬��Ϣ

#include "CPos.h"
#include "FindPathDefs.h"
#include "TimeHelper.h"
#include "TCoreObjAllocator.h"
//#include "CDistortedTick.h"

namespace sqr
{
	class CSyncCoreObjectMediator;

	class CGac2GasOC_Move_Mediator_To;
	class CGac2GasOC_Change_Mediator_Move_Target;
	class CGac2GasOC_Change_Mediator_Move_Speed;
	class CGac2GasOC_Tell_Mediator_Stepped;
	class CGac2GasOC_Tell_Mediator_Stopped;

	class CPixelPath;

	class CMedObjFollowState
		:public CCoreObjMallocObject
		//,public CDistortedTick
	{

	public:
		CMedObjFollowState(CSyncCoreObjectMediator* pObject,const CGac2GasOC_Move_Mediator_To* pCmd);
		~CMedObjFollowState();

		void SetNewPath(const CGac2GasOC_Change_Mediator_Move_Target* pCmd);

		const CFPos& GetBeginPixelPos()const;
		const CFPos& GetEndPixelPos()const;
		//const WayPointDeque& GetWayPointDeq()const;

		EFindPathType GetFindPathType()const;
		EBarrierType GetBarrierType()const;

		float GetSpeed()const;
		bool SetSpeed(const CGac2GasOC_Change_Mediator_Move_Speed* pCmd);

		float GetMovedDist()const;
		float GetEndDist()const;

		//��ʵϵͳʱ�������������,�ó�����Ŀǰ��׼������
		void GetRealCurPos(CFPos& PixelPos);
		float GetRealMovedDist()const;
		float CalcDirMovedDist()const;

		int32 GetCheatScore()const;

		void Step(float fNewMovedDist);

		const CPixelPath* GetPath()const;
		uint16 GetPixelPosOfPath(CFPos& PixelPos,float fDist)const;

	private:
		enum ESpeedCheckThreshold		{ eSCT_Standard = 10 };		//���׷�������
		enum ECheatScoreAdjustAmplitude { eCSAA_TickOnce = 1 };		//ÿ�ε������׷����ķ���
		enum ECheatScoreAdjustFrequency	{ eCSAF_TickOnce = 1000 };	//�������׷�����Ƶ��

		//void RegistDirStepCheckTick(uint32 uInterval);
		//uint32 EstimateDirStepFreq();
		//void OnTick();
		//void SuspendFollowerMoving();
		//void ResumeFollowerMoving();

		CSyncCoreObjectMediator*	m_pCoreObj;

		float				m_fSpeed;
		float				m_fMovedDist;
		float				m_fEndDist;

		CFPos				m_BeginPixelPos;
		CFPos				m_EndPixelPos;

		EFindPathType		m_eFindPathType;
		EBarrierType		m_eBarrierType;

		uint64				m_uLastStepTimePoint;	//�ϴ�step��ʱ���
		int32				m_iCheatScore;
		uint32				m_uClientTimeStamp;

		CPixelPath*			m_pPath;

		float				m_fBeginDist;
		uint64				m_uThisPathExpiredTime;
		float				m_fDirBeginDist;
		uint64				m_uDirThisPathExpiredTime;

		//bool				m_bDirStepped;
		//uint64				m_uMoveSuspendedTime;


	};

}

