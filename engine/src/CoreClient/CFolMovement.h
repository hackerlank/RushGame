#pragma once
#include "FindPathDefs.h"
#include "CPos.h"


namespace sqr
{

	enum EFolMovementType{eFMT_MoveTo,eFMT_ChangePath};

	class CCoreSceneClient;
	class CPixelPath;
	class CServerTimeGuesser;


	class CFolMovement
	{
	public:
		EFolMovementType	m_eType;

		CPixelPath* ClonePixelPath()const;

		bool IsModified()const;

		const CPixelPath* GetOriginPath()const;

		CFPos GetPathBeginPixelPos()const;		//���ԭʼ·�������
		CFPos GetPathEndPixelPos()const;			//���ԭʼ·�����յ�

		CFPos GetMovementBeginPixelPos()const;	//���ʵ��·�������	(���ܱ��ع�·��������;���·�����޸�)
		CFPos GetMovementEndPixelPos()const;		//���ʵ��·�����յ�	(���ܱ��ع�·��������;���·�����޸�)

		float GetSpeed()const{return m_fSpeed;}
		void SetSpeed(float fSpeed){m_fSpeed=fSpeed;}
		void CutPath(float fChangeDist);
		bool PathIsCut()const;		//·�����жϣ�Ҳ����˵��������������ChangePath
		bool GetPixelPos(CFPos& PixelPos,float fDist)const;
		
		//��㵽�յ���ƶ����벻һ������GetPathLength(),��Ϊ·�����п����߲���ġ�
		float GetPathLength()const;

		static CFolMovement* Create(CCoreSceneClient* pScene,const CFPos& PixelPosBegin,const CFPos& PixelPosEnd
			,EFolMovementType eType,float fSpeed,EFindPathType eFindPathType,EBarrierType eBarrierType,float fEndDist, uint64 uServerBaseTime);
		static CFolMovement* Create(CCoreSceneClient* pScene,const int16* pPathData, uint16 uPathDataLen,const CFPos& PixelPosBegin,const CFPos& PixelPosEnd
			,EFolMovementType eType,float fSpeed,float fEndDist, uint64 uServerBaseTime);

		~CFolMovement();
	
		uint64 GetEstimatedServerTime()const;
		uint64 GetElapsedTime()const;

		float GetMovedDistWhenRemoteReached()const;
		void SetMovedDistWhenRemoteReached(float fMovedDistWhenRemoteReached);
		float GetMovedDistWhenRemoteSuspended()const;
		void SetMovedDistWhenRemoteSuspended(float fMovedDistWhenRemoteSuspended);
		bool RemoteReached()const;
		float GetRemoteMovedDist()const;

		int32 GetTimeErr()const;
		float CalIdealDistByTimeInMovement(int32 nTime)const;
		void AccumulateSuspendedTime(uint32 uSuspendedTime);
		//debug
		bool	m_bChangePath;
	private:
		CFolMovement(CCoreSceneClient* pScene,const CFPos& PixelPosBegin,const CFPos& PixelPosEnd
			,EFolMovementType eType,float fSpeed,EFindPathType eFindPathType,EBarrierType eBarrierType,float fEndDist, uint64 uServerBaseTime);

		
		CFolMovement::CFolMovement(CCoreSceneClient* pScene,const int16* pPathData, uint16 uPathDataLen, const CFPos& PixelPosBegin ,const CFPos& PixelPosEnd
			,EFolMovementType eType,float fSpeed,float fEndDist, uint64 uServerBaseTime);


		CPixelPath*			m_pPath;
		float				m_fPathLength;	//����ֵ�ض�С�ڵ���m_pPath->GetPathPixelLength()
		bool				m_bPathIsCut;
		float				m_fSpeed;
		CServerTimeGuesser*	m_EstimatedServerTime;
		float				m_fMovedDistWhenRemoteSuspended;
		float				m_fMovedDistWhenRemoteReached;
		uint32				m_uTotalSuspendedTime;
	};

}