#pragma once
#include "TCoreObject.h"
#include "CoreObjectServerDefs.h"
#include "CSyncTraitsServer.h"
#include "CPos.h"
#include "PatternRef.inl"
#include "TCoreObjAllocator.h"
#include "SyncPatternCOR.h"

namespace sqr
{
	class CPixelPath;
	class CCoreSceneServer;
	class CCoreObjectServer;
	class CSyncCoreObjectServer;
	class CSyncCoreObjectMediator;
	class CSyncCoreObjectDictator;
	class CObjTransferState;
	class CObjDestoryState;
	struct CMoveBeganDerivedArgs;
	struct CMovePathChangedDerivedArgs;
	struct CMoveSpeedChangedDerivedArgs;
	
	class CAoiViewObjHandler;
	class CAsyAoiViewObj;

	class CObjPosObserver;
	class CObjPosObservee;
	class CCoreObjTracer;
	class ISyncObjPosObserverHandler;
	class CSyncWatchHandler;

	class CSyncCorePos;
	class CSyncCoreQuadObject;

	class CCoreObjCreateQuadObjJob;
	class CCoreObjTransferObjJob;
	class CCoreObjDestoryObjJob;
	class CCoreObjMoveToJob;
	class CCoreObjMoveToLimitJob;
	class CCoreObjTraceJob;
	class CCoreObjEndTransferJob;
	class CCoreObjEndDestroyJob;
	class CCoreObjBindToJob;
	class CCoreObjDisbindFromParentJob;
	class CCoreObjDisbindToChildJob;
	class CCoreObjDisbindToAllChildJob;

	enum EServerObjectLifeState
	{
		eSOLS_Running,
		eSOLS_Destroying,
		eSOLS_Transfering,
		eSOLS_AoiDestroying,
	};

	class CSyncCoreObjectServer
		:public TCoreObject<CSyncTraitsServer>
		,public CSyncPtCORSubject
	{
		typedef TCoreObject<CSyncTraitsServer>	ParentObject_t;
		friend class TCoreObject<CSyncTraitsServer>;
		friend class TObjMoveState<CSyncTraitsServer>;
		friend class TCoreScene<CSyncTraitsServer>;

		friend class CAoiViewObjHandler;
		friend class CAoiSyncerObjHandler;
		friend class CAoiSynceeObjHandler;

		friend class CCoreSceneServer;
		friend class CCoreObjectServer;

		friend class CObjPosObserver;
		friend class CCoreObjTracer;
		friend class CSyncCoreObjectMediator;
		friend class CSyncCoreObjectDictator;

		//friend class CCoreObjBindToJob;
		//friend class CCoreObjDisbindFromParentJob;
		//friend class CCoreObjDisbindToChildJob;
		//friend class CCoreObjDisbindToAllChildJob;
		//friend class CAoiBindToCmd;

	public:
		uint32 GetGlobalID()const;

		virtual EServerObjectType GetType()const=0;

		void AddGPCNFlagRef();
		void DelGPCNFlagRef();

		static CSyncCoreObjectServer* GetObject( uint32 uObjGlobalID );

		static CSyncCoreObjectServer* GetObjectServer( uint32 uObjGlobalID );
		
		void SetSize( float fSize, uint32 uDimension = 0 );
		float GetSize(uint32 uDimension = 0 )const;

		virtual void SetEyeSight( float fEyeSight, uint32 uDimension = 0 );
		virtual float GetEyeSight(uint32 uDimension = 0)const;

		void SetStealth( float fStealth, uint32 uDimension = 0 );
		float GetStealth(uint32 uDimension = 0)const;

		void SetKeenness(float Keenness, uint32 uDimension = 0);
		float GetKeenness(uint32 uDimension = 0)const;

		void SetViewGroup(uint32 uGroupID, uint32 uDimension = 0);
		uint32 GetViewGroup(uint32 uDimension = 0)const;

		void PostAoiMsgToSelf(const void* pContext);

		void PostEvent(const void* pContext);

		ESetPosResult SetGridPos(const CPos& GridPos);
		virtual ESetPosResult SetPixelPos(const CFPos& PixelPos);
		virtual ESetSpeedResult SetSpeed(float fSpeed);
		virtual bool IsMoving()const;
		
		virtual EMoveToResult MoveTo(const CFPos& PixelPosDes,float fSpeed
			,EFindPathType eFindPathType,EBarrierType eBarrierType,float fReachDist,float fMovedDist, CPixelPath* pPixelPath);
		EMoveToResult MoveToLimit(const CFPos& PixelPosDes, float fSpeed, 
			EFindPathType eFindPathType, EBarrierType eBarrierType,float fReachDist,float fMovedDist, CPixelPath* pPixelPath, int32 nMaxStep);
		virtual EStopMovingResult StopMoving();
		

		void Watch(uint32 uHandlerID, CSyncCoreObjectServer* pTarget, float fPixelDistThreshold);
		void StopWatching(uint32 uHandlerID);
		void Trace(CSyncCoreObjectServer* pTarget, float fSpeed
			, EFindPathType eFindPathType, EBarrierType eBarrierType, float fReachDist);
		void StopTracing();

		//����ת��
		CSyncCoreObjectMediator* CastToObjMediator();
		CSyncCoreObjectDictator* CastToObjDictator();
		const CSyncCoreObjectMediator* CastToObjMediator()const;
		const CSyncCoreObjectDictator* CastToObjDictator()const;

		bool Running()const;
		bool Destroying()const;
		bool Transfering()const;
		
		virtual void LogMsg(const string& log);


		void SetQuerySize(float fSize);  //����д������Ϊ������setsize����������İ����������������aoi����
		float GetQuerySize() const;

		virtual void GetRealCurPos(CFPos& PixelPos);
		virtual float Distance(const CSyncCoreObjectServer* pCoreObj)const;

		void IncreaseMoveToSessionID();
		void IncreaseTraceSessionID();
		uint32 GetLastMoveID()const;
		uint32 GetTraceSessionID()const;

		void EndTracing();
		void IntStopTracing();

	public:
		friend class CSyncCoreSceneServer;
		friend class CCoreQuadScene;
		typedef vector<CSyncCoreObjectServer*,TCoreObjAllocator<CSyncCoreObjectServer*> > IDVecSynCoreObjectServer;
		static IDVecSynCoreObjectServer& GetSynCoreObjServerVec();

		uint32 CreateID();
		bool DestroyID();

		bool HaltAoiObject();
		void OnAoiObjectHalted();
		void SetAoiObjectHalt();
		bool BeginDestroyObject();
		
		bool BeginTransferObject(CSyncCoreSceneServer* pScene , const CFPos& PixelPos);
		

		//��TCoreObject���õ�
		inline CSyncCoreSceneServer* IntGetScene()const
		{
			return m_pScene;
		}

		void InsertToScene();
		void RemoveFrScene();

		void OnObserveePosChanged();
		void OnObserverPosChanged();

		void CreatePosData(uint32 uSlot, const CFPos& pos);
		void ReleasePosData();

		void CreateQuadObject(uint32 uSlot, const CFPos& pos);
		void ReleaseQuadObject();

		CObjPosObservee* GetPosObservee()const;

		void ReSetCoreScene(CSyncCoreSceneServer* pSynCoreScene);

		void SetFlag( EServerObjectFlag eFlag );
		EServerObjectFlag GetFlag()const;
		void SetGPCNFlag(bool bSet);
		bool GetGPCNFlag()const;

		void EndDestroyObject();
		void EndTransferObject();
		virtual void IntBeginTransferObject(CSyncCoreSceneServer* pScene,const CFPos& PixelPos);
		virtual void IntBeginDestroyObject();
		virtual void IntEndTransferObject();	

		void ForceUpDateQuadTreePos();

	protected:
		CSyncCoreObjectServer(uint32 uObjID, CSyncCoreSceneServer* pScene, const CFPos& PixelPos);
		virtual ~CSyncCoreObjectServer(void);

		void NotifyPosChangedIfNeed();
		void NotifyPosChangedIfNeedSingle();
		void NotifySpeedChanged();

		void IntPostMsgToSelf(const void* pContext);
		
		//�����ܿ����Ķ�������Ϣ
		void IntPostMsgToViewee(const void* pContext, uint32 uDimension);
		//���ܿ����ҵĶ�������Ϣ
		void IntPostMsgToViewer(const void* pContext, uint32 uDimension);


		//���к�����TCoreObject��ģ���麯������CSyncCoreObjectServer��ʼ����Ҳ�������ͨ���麯��
		virtual void OnMoveSteppedDerived(const CFPos&);
		virtual void OnMoveBeganDerived(const CMoveBeganDerivedArgs& Args);
		virtual void OnEndMoveDerived();
		virtual void OnMoveStoppedDerived(float fStopDist);
		virtual void OnMovePathChangedDerived(const CMovePathChangedDerivedArgs& Args);
		virtual void OnMoveSpeedChangedDerived(const CMoveSpeedChangedDerivedArgs& Args);
		virtual void OnPixelPosSetDerived(bool bSuperPosition);		
		
		virtual void IntSetPositionDerived(const CFPos& PixelPos);

		virtual bool BindObjIntSetPosition(const CFPos& PixelPos) {return true;}
		virtual void BindObjNotifyPosChangedIfNeed() {}

		//����ĺ�����TCoreObject��ģ���麯������CSyncCoreObjectServer��ʼ����Ҳ�������ͨ���麯��

		virtual void OnViewRelationChanged(CSyncCoreObjectServer* pObjServer,bool bSee, uint32 uDimension);

		//�������ҵĶ�����ҷ��͵���Ϣ
		virtual void OnMsgFromViewer(const void* pContext);
		//�����ҿ����Ķ�����ҷ��͵���Ϣ
		virtual void OnMsgFromViewee(const void* pContext);

		//�����ҿ����Ķ��������Ϣ�������֮�����
		virtual void OnMsgToVieweeHandled(const void* pContext);
		//���п����ҵĶ��������Ϣ�������֮�����
		virtual void OnMsgToViewerHandled(const void* pContext);

		virtual void OnMsgFromSelf(const void* pContext);

		const CAsyAoiViewObj* GetAoiObject() const;
		CAsyAoiViewObj* GetAoiObject();		

	private:
		CSyncCoreSceneServer*	m_pScene;

		uint32				m_uGPCNFlagRefCount;
		EServerObjectFlag	m_eFlag;

		CObjTransferState*	m_pTransferState;
		CObjDestoryState*	m_pDestoryState;

		EServerObjectLifeState m_eLifeState;

		typedef map<uint32, ISyncObjPosObserverHandler*,less<uint32>,
			TCoreObjAllocator<pair<uint32,ISyncObjPosObserverHandler*> > > MapPosObserverHandler;
		MapPosObserverHandler m_mapPosObserverHandler;

		typedef map<ISyncObjPosObserverHandler*, CObjPosObserver*,less<ISyncObjPosObserverHandler*>,
			TCoreObjAllocator<pair<ISyncObjPosObserverHandler*,CObjPosObserver*> > > MapPosObserver;
		MapPosObserver m_mapPosObserver;	//һ��CoreObj����ͬʱ��ض��Ŀ��
		CCoreObjTracer* m_pTracer;
		CObjPosObservee* m_pPosObservee;		//��CoreObj�����˼�صĶ���	
		TPtRefee<CSyncCoreObjectServer, CCoreObjTracer> m_RefsByTracer;

		bool	m_bIsTracing;
		uint32	m_uLastMoveID;
		uint32	m_uTraceSessionID;

		TPtRefer<CSyncCoreObjectServer, CAsyAoiViewObj>		m_AoiObj;
		CSyncCorePos*			m_pPosData;
		CSyncCoreQuadObject*	m_pQuadObject;
		TPtRefee<CSyncCoreObjectServer, CAoiViewObjHandler> m_RefsByAoiObjHandler;
	};
}
