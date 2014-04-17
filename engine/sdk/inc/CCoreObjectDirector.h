#pragma once

//$id$
//CCoreObjectDirector,������ʵ�����ǵ��ɿͻ��˿��ƵĶ����λ��ͬ������

#include "CCoreObjectFollower.h"


namespace sqr
{
	class CCoreSceneClient;
	class CDirObjActiveState;
	class ICoreObjectDirectorHandler;

	class CGas2GacOC_Create_Still_Object;
	class CGas2GacOC_Create_Moving_Object;
	class CGas2GacOC_Set_Director_Active;
	class CGas2GacOC_Set_Dir_Max_Speed_And_Move_To;
	class CGas2GacOC_Set_Dir_Max_Speed_Zero;
	class CGas2GacOC_Set_Dir_Max_Speed;
	class CGas2GacOCI_Director_Create_End;

	class CRenderScene;
	class CCoreObjectDirector
		:public CCoreObjectFollower
	{
	public:
		ICoreObjectDirectorHandler* GetHandler()const;
		EClientObjectType GetType()const;

		static CCoreObjectDirector* Inst();

		bool ControlCamera(bool bControl);

		bool IsPassive()const;
		bool IsMoving()const;
		CPixelPath* GetClonedMovePath()const;

		//Follower�������4��������Զ����not allowed
		ESetSpeedResult SetSpeed(float fSpeed);
		EStopMovingResult StopMoving();

		EMoveToResult MoveToLimit(const CFPos& PixelPosDes,float fSpeed,EFindPathType eFindPathType, EBarrierType eBarrierType,float fReachDist, int32 nMaxStep);
		EMoveToResult MoveTo(const CFPos& PixelPosDes,float fSpeed,EFindPathType eFindPathType,EBarrierType eBarrierType,float fReachDist);

		ESetPosResult SetPixelPos(const CFPos& PixelPos);
		ESetPosResult SetGridPos(const CPos& GridPos);
		virtual void GetServerPixelPos(CFPos& PixelPos)const;

		float GetMaxSpeed()const;
	protected:
		CCoreObjectDirector(const CGas2GacOC_Create_Still_Object* pCmd);
		~CCoreObjectDirector(void);

	private:
		friend class CConnClient;
		friend class CCoreSceneClient;

		bool	m_bWaitingToBeActive;	//�ȴ���������״̬
		bool	m_bActive;
		CDirObjActiveState*		m_pActState;
		void RefreshCameraDest(const CFPos& posCMemoryCookie);

		float	m_fDirectorMaxSpeed;

		void TellMediatorActivated();
		void TellMedMaxSpeedSet(uint32 uSessionID)const;

		template<typename CmdType>
		void SendCmdToMediator(CmdType* pCmd)const;

		void OnServerCommand(const CGas2GacOC_Set_Dir_Max_Speed_And_Move_To* pCmd);
		void OnServerCommand(const CGas2GacOC_Set_Director_Active* pCmd);
		void OnServerCommand(const CGas2GacOC_Set_Dir_Max_Speed_Zero* pCmd);
		void OnServerCommand(const CGas2GacOC_Set_Dir_Max_Speed* pCmd);
		void OnServerCommand(const CGas2GacOCI_Director_Create_End* pCmd);

		void StopMovingWhenPassThrough();
		void SwitchActiveMode();
		void IntSetPositionDerived(const CFPos& PixelPos);

		void OnMoveSteppedDerived(const CFPos&);
		void OnMoveBeganDerived(const CMoveBeganDerivedArgs& Args);
		void OnEndMoveDerived();
		void OnMoveStoppedDerived(float fStopDist);
		void OnMovePathChangedDerived(const CMovePathChangedDerivedArgs& Args);
		void OnMoveSpeedChangedDerived(const CMoveSpeedChangedDerivedArgs& Args);

		static uint64& GetLastDistortedProcessTime();	//��¼�ϴ��˳�����ʱ��Ť��ʱ��,Ϊ�˱�֤�л����¸�������Ť��ʱ��һ��
		static uint64& GetLastProcessTime();	//��¼�ϴ��˳�����ʱ����ʵʱ��
		static uint64& GetLastDistortedFrameTime();
		static uint64& GetLastFrameTime();

		bool CheckSpeed(float fSpeed)const;
	};
}
