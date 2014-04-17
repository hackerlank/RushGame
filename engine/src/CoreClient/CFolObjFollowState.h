#pragma once
#include "FindPathDefs.h"
#include "CPos.h"
#include "TObjMoveState.h"
#include "CTraitsClient.h"
#include "TimeHelper.h"
#include "CLogCoreObject.h"

#ifdef USE_RENDER_SLOT
#include "CRenderSlot.h"
#endif


namespace sqr
{
	/*
	CFolMovement�ฺ���������ƶ�����

	�������������·���ϣ���ʹ��·������ƽ���취��
	����ĸ߶Ȳ��������޸ĵģ�Ѱ·��Ϊֱ���ö�������¸߶������С�

	ȥ������˶�����ͻ��˷��͵�step����,�����������ַ��������Ӧ����. 
	1.ÿ���ͻ��˵�����fol�����Լ�����ͷ����ʱ���Ĳ���,�Ӷ�������˴����ĺͷ����֮���λ�����,�������ͻ���ͬ��ʱ���Ƶ����ԭ����5����ߵ�1��. 
	2.�����Med�������Ӷ�Dir������step��Ϣ�ļ��,�������dir��ʱ����step,Med��֪ͨfolֹͣ�ƶ�,ֱ��dir�ָ�����step��Ϣ.
	*/
	class CFolMovement;
	class CCoreObjectFollower;
	class CGas2GacOC_Suspend_Follower_Moving;

#ifdef LOG_COREOBJ_MOVE
	class CPrintMovedDist;
#endif

	enum EObjectHeight;

	class CFolObjFollowState
#ifdef USE_RENDER_SLOT
		:public CRenderSlot
#else
		:public CTick
#endif
	{
		friend class CCoreObjectFollower;
	public:
		CFolObjFollowState(CFolMovement* pMm,float fSpeed,CCoreObjectFollower* pObj);
		~CFolObjFollowState(void);
		
		void SetIdealSpeed(float fSpeed);	//������ٶȣ�δ�����ٶ������ԭʼ�ٶ�

		float GetRealSpeed()const;
		void ModifySpeed(float fSpeedModifier);

		CFolMovement*			m_pCurMovement;			//���·��������Ϊ��
		list<CFolMovement*>		m_lstPendingMovement;

		uint64					m_tcpNotifySpeedChanged;	//���ڿ���OnSpeedChangedƵ�ʲ�Ҫ����

		CFolMovement* GetLastMovement()const;

		//fRealMovedDist,��TObjMoveState��õĶ���ǰ�ƶ�״̬��MovedDist
		float GetMovedDist(CObjMoveStateClient* pMovState)const;
		//float GetTotalMovedDistError()const;

		float GetTotalMovedDistError(CObjMoveStateClient* pMovState, uint32 uDelay)const;
		float GetEasyMovedDistError(CObjMoveStateClient* pMovState, uint32 uDelay)const;

		float GetRemoteLastPathMovedDist()const;
		
		//�յ��ķ�������ǰλ��
		float GetRemoteTotalMovedDist()const;

		//Ԥ��ķ�������ǰλ��
		float GetEasyRemoteTotalMovedDist()const;

		CFPos GetServerObjectPixelPos()const;

		bool RemoteObjectReachedCurPath()const;
		bool RemoteObjectReached()const;

		void OnStepped(float fLastPathMovedDist);

		float CalIdealDistByTime(int32 nTime)const;

#ifdef LOG_COREOBJ_MOVE
		CFPos				m_ServerReachedPos;
		float				m_fServerReachedDist;
		float				m_fReachedDist;
#endif

	private:
		//void SuspendMoving(const CGas2GacOC_Suspend_Follower_Moving* pCmd);
		//void ResumeMoving(uint32 uSuspendedTime);
#ifdef USE_RENDER_SLOT
		void OnUpdate(uint64 uLastFrameUsedTime);
		void AdjustSpeed(uint32 uMoveInterval);
#else
		void OnTick();	//�ƶ��ٶ�ƥ��
		void AdjustSpeed();
#endif
		void SetSpeed(float fSpeed);
		float GetSpeed()const;

		//����˶��������һ��·���ϵ��ƶ����ȡ����������ȷ������˶����
		//��ǰλ�ã���Ϊƽ�������ݡ������ֵС��0˵������˶����Ѿ���������һ��·����
		float				m_fRemoteLastPathMovedDist;
		float				m_fSpeedModifier;		//�ٶ������������ڸ������߽��ȵĲ���ʹ�ͻ������߽��Ⱦ�����������������߽��ȡ�
		uint64				m_tcpStep;

		CCoreObjectFollower*	m_pObj;

#ifdef LOG_COREOBJ_MOVE
		friend class CPrintMovedDist;
		bool		m_bNormalSpeed;
		CPrintMovedDist*		m_pPrintTick;
#endif
	};

}
