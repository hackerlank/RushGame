#pragma once
#include "CCoreObjectClient.h"
#include "TCoreObjAllocator.h"
#include "CVariantHolderDef.h"

#ifdef GetObject
#undef GetObject
#endif

namespace sqr
{
	class ICoreObjectFollowerHandler;

	class CFolObjFollowState;
	class CFolMovement;

	class CSlowObjCmd;
	class CDelayedObjCmdMgr;

	class CGas2GacOC_Create_Still_Object;
	class CGas2GacOC_Create_Moving_Object;
	class CGas2GacOC_Check_Object_ViewVariant;
	class CGas2GacOC_Sync_View_Variant_End;
	class CGas2GacOC_Init_View_Variant;
	class CGas2GacOC_Move_Follower_To;
	class CGas2GacOC_Change_Follower_Move_Target;
	class CGas2GacOC_Change_Follower_Move_Speed;
	class CGas2GacOC_Tell_Follower_Stepped_SmallDist;
	class CGas2GacOC_Tell_Follower_Stepped_MiddleDist;
	class CGas2GacOC_Tell_Follower_Stepped_BigDist;
	class CGas2GacOC_Tell_Follower_Stopped;
	//class CGas2GacOC_Suspend_Follower_Moving;
	//class CGas2GacOC_Resume_Follower_Moving;
	class CGas2GacOC_Tell_Follower_Reached;
	class CGas2GacOC_Enable_Follower_Delay;
	class CGas2GacOC_Set_Object_Pos;
	class CGas2GacOC_Sync_Object_Pos;
	class CGas2GacOC_Validate_Follower_Pos;

	class CGas2GacOC_Static_Sync_Variant_Small_Data;
	class CGas2GacOC_Static_Sync_Variant_Middle_Data;
	class CGas2GacOC_Static_Sync_Variant_Big_Data;
	class CGas2GacOC_Agile_Sync_Variant_Small_Data;
	class CGas2GacOC_Agile_Sync_Variant_Middle_Data;
	class CGas2GacOC_Agile_Sync_Variant_Big_Data;

	class CGas2GacOC_Static_View_Variant_Small_Data;
	class CGas2GacOC_Static_View_Variant_Middle_Data;
	class CGas2GacOC_Static_View_Variant_Big_Data;
	class CGas2GacOC_Agile_View_Variant_Small_Data;
	class CGas2GacOC_Agile_View_Variant_Middle_Data;
	class CGas2GacOC_Agile_View_Variant_Big_Data;

	class CGas2GacOC_Check_Object_SyncVariant;
	class CGas2GacOC_Init_Sync_Variant;
	class CGas2GacOC_Left_Sync_Aoi;
	class CGas2GacOC_Entered_Sync_Aoi;
	class CGas2GacOC_Sync_Dynamic_Barrier;
	class CGas2GacOC_Bind_To;
	class CGas2GacOC_Disbind_From_Parent;
	class CGas2GacOC_Disbind_All_Children;

	class CObjVarDefCfg;
	class CSyncVariantClientHolder;
	class CSyncVariantClient;
	class ICypherVariantHolderHandler;

	class CCoreSceneClient;


	class CCoreObjectFollower
		:public CCoreObjectClient
	{
		friend class TBindObject<CCoreObjectFollower>;
		typedef set<uint32, less<uint32>, TCoreObjAllocator<uint32> >	SetUInt32;
		typedef vector<CSyncVariantClientHolder*,TCoreObjAllocator<CSyncVariantClientHolder*> > VecSyncVariantClientHolder_t;
	public:
		static CCoreObjectFollower* GetObjectFollower(uint32 uObjGlobalID);
		static const map<uint32,CCoreObjectFollower*>& GetObjectFollowerMap();

		uint32 GetGlobalID(void)const;
		virtual bool IsPassive()const;
		bool IsMoving()const;
		bool GetDestOfCurPath(CFPos& pos);

		EClientObjectType GetType()const;

		uint32 GetMoveDelay()const;
		bool MoveDelayEnabled()const;

		ICoreObjectFollowerHandler* GetHandler()const;

		virtual void GetServerPixelPos(CFPos& PixelPos)const;

		static CObjVarDefCfg* GetVarDefCfg();
		const CSyncVariantClient* GetSyncVariant(ECodeTableIndex eIndex) const;
		const CSyncVariantClient* GetViewVariant(ECodeTableIndex eIndex) const;

		const CSyncVariantClientHolder* GetSyncVariantHolder(ECodeTableIndex eIndex) const;
		const CSyncVariantClientHolder* GetViewVariantHolder(ECodeTableIndex eIndex) const;

		ICypherVariantHolderHandler* GetSyncVariantHandler() const;
		ICypherVariantHolderHandler* GetViewVariantHandler() const;
		void SetSyncVariantHandler(ICypherVariantHolderHandler *pHandler, ECodeTableIndex eIndex);
		void SetViewVariantHandler(ICypherVariantHolderHandler *pHandler, ECodeTableIndex eIndex);

		bool IsInSyncAoi() const;

		bool HasBarrier()const;
		float GetBarrierSize()const;

		float Distance(const CCoreObjectFollower* pCoreObj)const;

		CPixelPath* GetClonedMovePath()const;

		void LogMsg(const string& log);

		//void AddPendingBindRelation(uint32 pParentID, uint32 pChildID);
		//void RemovePendingBindRelation(uint32 pParentID);
		CCoreObjectFollower* GetBindObjParent()const;
		CCoreObjectFollower* MoveFirstBindObjChild();
		CCoreObjectFollower* MoveNextBindObjChild();
		bool EndOfBindObjChild()const;
		CCoreObjectFollower*  GetCurBindObjChild()const;
		void SetViewVariantInitEnd();
		void SetSyncVariantInitEnd();

	protected:
		CCoreObjectFollower(const CGas2GacOC_Create_Still_Object* pCmd);
		CCoreObjectFollower(const CGas2GacOC_Create_Moving_Object* pCmd);
		~CCoreObjectFollower(void);

		virtual float GetRealMovedDistDerived()const;

		virtual void StopMovingWhenPassThrough();

		void OnEndMoveDerived();

		bool HasUnfinishedPassiveMovement()const;

		virtual bool BindTo(uint32 uParentLID);
		virtual bool BindTo(CCoreObjectFollower* pParent);
		virtual bool DisbindFromParent();
		virtual bool DisbindChild(uint32 uChildLID);
		virtual bool DisbindAllChildren();
		virtual bool IsBindingObj() const;

		void OnBindTo(uint32 uParentLID, bool bRet);
		void OnBeforeBindTo(uint32 uParentLID, bool bRet);
		void OnBeforeDisbindFromParent();
		void OnBeforeDisbindToChild(uint32 uChildLID);
		void OnBeforeDisbindToAllChild();

		virtual bool BindObjIntSetPosition(const CFPos& PixelPos);
		void OnSyncViewVariantEnd(uint32 uBindParentGID, uint32 uStaticVariantRealDecodeTimes, uint32 uAgileVariantRealDecodeTimes);
		//virtual void BindObjNotifyPosChangedIfNeed();
		//void NotifyPosChangedIfNeed()		{};

		TBindObject<CCoreObjectFollower>*			m_pBindObj;
		SetUInt32::iterator	m_itrBindObjChild;
	private:
		typedef vector<CPos> BarrierArea;

		friend class CFolMovement;
		friend class CConnClient;
		friend class CDelayedObjCmdMgr;
		friend class CSlowObjCmd;
		friend class CTestSuiteCoreObjectClient;
		friend class CCoreSceneClient;
		friend class CFolObjFollowState;

		template<typename CmdType>void CreateSyncVariant(const CmdType* pCmd);
		void DelSyncVariant();

		template<typename CmdType>friend class TFastObjCmd;
		template<typename CmdType>friend class TSlowObjCmd;

		void ConstructHelper();

		//�˺��������ɴ���������Ϣ�ĺ������ã����ܱ���Ϸ�߼��Ĵ�����ã��������ֻص�ʱ�����
		void FlushAllPassiveMovement();

		static float& GetCorrectionSpeedRatio();

		void GenErrAndLogInvalidPos(const CFPos& pos)const;

		CDelayedObjCmdMgr*	m_pDelayedCmdMgr;

		uint32	m_uGlobalID;
		bool	m_bEnableMoveDelay;
		uint32	m_uMoveDelay;	//�ƶ��ӳٺ�����

		//��Follower�������ƶ�״̬(OnMoveBegan��OnMoveEnded֮���״̬)ʱ�����ָ�����������Ϊ��
		CFolObjFollowState*		m_pFolState;

		//�ͻ�������Follower�����id�������ӳ�������DirectorҲ��һ��Follower��������Director����Ҳ��������С�
		typedef map<uint32,CCoreObjectFollower*>	MapObjFollower_t;
		static MapObjFollower_t	ms_mapObjFollower;

		static bool AddObjectFollower(CCoreObjectFollower* pObject);
		static bool DelObjectFollower(uint32 uObjGlobalID);


		void OnServerCommand(const CGas2GacOC_Set_Object_Pos* pCmd);
		void OnServerCommand(const CGas2GacOC_Sync_Object_Pos* pCmd);
		void OnServerCommand(const CGas2GacOC_Enable_Follower_Delay* pCmd);
		void OnServerCommand(const CGas2GacOC_Check_Object_ViewVariant* pCmd);
		void OnServerCommand(const CGas2GacOC_Sync_View_Variant_End* pCmd);
		void OnServerCommand(const CGas2GacOC_Check_Object_SyncVariant* pCmd);
		void OnServerCommand(const CGas2GacOC_Entered_Sync_Aoi* pCmd);
		void OnServerCommand(const CGas2GacOC_Left_Sync_Aoi* pCmd);
		void OnServerCommand(const CGas2GacOC_Init_View_Variant* pCmd);
		void OnServerCommand(const CGas2GacOC_Init_Sync_Variant* pCmd);


		//����ͬ��
		VecSyncVariantClientHolder_t	m_vecSyncVariantHolder;
		VecSyncVariantClientHolder_t	m_vecViewVariantHolder;

		static void GenVariantDecodeError(const char* szVariantTypeName,uint32 uEncodeTimes,uint32 uDecodeTimes);

		template<typename CmdType>
		void UpdateSyncVariant(const CmdType* pCmd, ECodeTableIndex eIndex);

		template<typename CmdType>
		void DecompressUpdateSyncVariant(const CmdType* pCmd);
		
		template<typename CmdType>
		void UpdateViewVariant(const CmdType* pCmd, ECodeTableIndex eIndex);

		template<typename CmdType>
		void DecompressUpdateViewVariant(const CmdType* pCmd);
		
		void OnServerCommand(const CGas2GacOC_Static_Sync_Variant_Small_Data* pCmd);
		void OnServerCommand(const CGas2GacOC_Static_Sync_Variant_Middle_Data* pCmd);
		void OnServerCommand(const CGas2GacOC_Static_Sync_Variant_Big_Data* pCmd);
		void OnServerCommand(const CGas2GacOC_Agile_Sync_Variant_Small_Data* pCmd);
		void OnServerCommand(const CGas2GacOC_Agile_Sync_Variant_Middle_Data* pCmd);
		void OnServerCommand(const CGas2GacOC_Agile_Sync_Variant_Big_Data* pCmd);

		void OnServerCommand(const CGas2GacOC_Static_View_Variant_Small_Data* pCmd);
		void OnServerCommand(const CGas2GacOC_Static_View_Variant_Middle_Data* pCmd);
		void OnServerCommand(const CGas2GacOC_Static_View_Variant_Big_Data* pCmd);
		void OnServerCommand(const CGas2GacOC_Agile_View_Variant_Small_Data* pCmd);
		void OnServerCommand(const CGas2GacOC_Agile_View_Variant_Middle_Data* pCmd);
		void OnServerCommand(const CGas2GacOC_Agile_View_Variant_Big_Data* pCmd);

		float				m_fBarrierSize;
		EBarrierType		m_eBarrierType;	
		void OnServerCommand(const CGas2GacOC_Sync_Dynamic_Barrier* pCmd);
		void ClearBarrier();
		void SetBarrier(float fBarrierSize, EBarrierType eBarrierType);
		void UpdateSceneBarrier(float fBarrierSize, EBarrierType eBarrierType);

		void IntSetPositionDerived(const CFPos& PixelPos);
		void TellFollowerStepped(float fMovedDist);

		void OnServerCommand(const CGas2GacOC_Bind_To* pCmd);
		void OnServerCommand(const CGas2GacOC_Disbind_From_Parent* pCmd);
		void OnServerCommand(const CGas2GacOC_Disbind_All_Children* pCmd);


		//Ϊ��ƽ����ԭ��Follower������ܵ����������Ҫ����һ�����ӳٴ�������ĺ����Ǿ����ӳ�֮�󱻵��õĺ�������������������ĺ���

		//���º����������ӳ�����
		void OnDelayedServerCommand(const CGas2GacOC_Move_Follower_To* pCmd);
		void OnDelayedServerCommand(const CGas2GacOC_Change_Follower_Move_Speed* pCmd);

		//���º����ǿ����ӳ�����
		void OnDelayedServerCommand(const CGas2GacOC_Change_Follower_Move_Target* pCmd);
		void OnDelayedServerCommand(const CGas2GacOC_Tell_Follower_Stepped_SmallDist* pCmd);
		void OnDelayedServerCommand(const CGas2GacOC_Tell_Follower_Stepped_MiddleDist* pCmd);
		void OnDelayedServerCommand(const CGas2GacOC_Tell_Follower_Stepped_BigDist* pCmd);
		void OnDelayedServerCommand(const CGas2GacOC_Tell_Follower_Stopped* pCmd);
		void OnDelayedServerCommand(const CGas2GacOC_Tell_Follower_Reached* pCmd);
		void OnDelayedServerCommand(const CGas2GacOC_Validate_Follower_Pos* pCmd);
		//void OnDelayedServerCommand(const CGas2GacOC_Suspend_Follower_Moving* pCmd);
		//void OnDelayedServerCommand(const CGas2GacOC_Resume_Follower_Moving* pCmd);

		//ֱ�ӽ����������ӷ��͹��������������������
		//���������������������롰�ӳ��������������Ȼ�����ӳ�������������ʵ���ʱ��������͸������OnDelayedServerCommand
		template<typename CmdType>
		void OnSlowServerCommand(const CmdType* pCmd);

		template<typename CmdType>
		void OnFastServerCommand(const CmdType* pCmd);

#ifdef USE_RENDER_SLOT
		virtual void OnSpeedAdjust(uint32 uMoveInterval);
#endif

		typedef multimap<uint32, uint32>			MapPendingBindRelation;
		typedef pair<MapPendingBindRelation::const_iterator, MapPendingBindRelation::const_iterator>	MapPendingBindRelationItr;
		static MapPendingBindRelation				m_mapPendingBindRelation;
		uint32										m_uPendingBindParentGID;
		bool					m_bEnteredSyncAoi;
		bool					m_bEnteredViewAoi;

		typedef queue<uint64>	QueMoveToBaseTime;
		QueMoveToBaseTime		m_queMoveToBaseTime;
		

		//���±�����Ϊ����ʹ��
		bool					m_bReceiveStopOrReachCmd;

	};

}
