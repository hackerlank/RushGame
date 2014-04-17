#pragma once
#include "CPos.h"
#include "CDynamicObject.h"
#include "CoreObjectServerDefs.h"
#include "CoreObjectDefs.h"
#include "FindPathDefs.h"
#include "PatternRef.h"
#include "TCoreObjAllocator.h"

namespace sqr
{
	template<typename ValueType>
	class TIDPtrMap;
	class CPixelPath;
	class CSyncCoreObjectServer;
	class CCoreSceneServer;
	class CCoreObjectMediator;
	class CCoreObjectDictator;
	class IWatchHandler;
	class ICoreObjectServerHandler;
	class CAsynCorePos;
	class CAsynCoreQuadObject;

	class CCoreObjOnDestroyResult;
	class CCoreObjOnMoveEndedResult;
	class CCoreObjOnTraceEndedResult;
	class CCoreObjOnMoveBeganResult;
	class CCoreObjOnObserveeEnteredResult;
	class CCoreObjOnObserveeLeftResult;
	class CCoreObjWatchResult;
	class CCoreObjOnPosChangedResult;
	class CCoreObjOnTransferedResult;
	class CCoreObjOnBindToResult;
	class CCoreObjOnDisindToParentResult;
	class CCoreObjOnDisbindToChildResult;
	class CCoreObjOnDisbindToAllChildResult;
	class IObjPosObserverHandler;
	struct MovedDistArg;

	class IDistortedTime;
	class CDistortedTimeObj;
	class CBindObjectServer;

	class CCoreObjectServer
		:public virtual CDynamicObject
		,public CCoreObjMallocObject
	{
		typedef TIDPtrMap<CCoreObjectServer>	MapObject;
		friend class CCoreSceneServer;
		friend class CCoreObjectDictator;
		friend class CCoreObjectMediator;
		friend class CSyncCoreObjectServer;
		friend class CCoreObjOnDestroyResult;
		friend class CCoreObjOnMoveEndedResult;
		friend class CCoreObjOnTraceEndedResult;
		friend class CCoreObjOnMoveBeganResult;
		friend class CCoreObjOnObserveeEnteredResult;
		friend class CCoreObjOnObserveeLeftResult;
		friend class CCoreObjWatchResult;
		friend class CCoreReSetCoreSceneResult;
		friend class CCoreObjOnPosChangedResult;
		friend class CCoreObjOnTransferedResult;
		friend class CCorePixelPosSetResult;
		friend class CCoreObjOnBindToResult;
		friend class CCoreObjOnDisbindFromParentResult;
		friend class CCoreObjOnDisbindToChildResult;
		friend class CCoreObjOnDisbindToAllChildResult;

		enum EAsynObjLifeState
		{
			eAOLS_Running,
			eAOLS_Destroying,
			eAOLS_Transfering
		};

		struct AoiData
			:public CCoreObjMallocObject
		{
			float fSize;
			float fEyeSight;
			float fStealth;
			float fKeenness;
			uint32 uViewGroup;
		};

	public:
		static CCoreObjectServer* GetObjectServer(uint32 uObjGlobalID);
		uint32 GetGlobalID()const;
		uint32 GetLocalID()const;

		virtual EServerObjectType GetType()const=0;
		virtual CCoreObjectMediator* CastToObjMediator()=0;
		virtual CCoreObjectDictator* CastToObjDictator()=0;

		CCoreSceneServer* GetScene()const;

		void SetHandler(ICoreObjectServerHandler* pHandler);
		ICoreObjectServerHandler* GetHandler()const;

		IDistortedTime* GetDistortedTime();

		//����AOI���º���
		void SetViewEyeSight( float fEyeSight, uint32 uDimension );
		float GetViewEyeSight(uint32 uDimension )const;

		void SetViewSize( float fSize, uint32 uDimension );
		float GetViewSize(uint32 uDimension ) const;

		void SetViewStealth( float fStealth, uint32 uDimension );
		float GetViewStealth(uint32 uDimension )const;

		void SetViewKeenness(float Keenness, uint32 uDimension );
		float GetViewKeenness(uint32 uDimension )const;

		void SetViewGroup(uint32 uGroupID, uint32 uDimension );
		uint32 GetViewGroup(uint32 uDimension )const;


		virtual bool DisbindFromParent()	{return true;}
		virtual bool DisbindAllChildren()	{return true;}

		void PostAoiMsgToSelf(const void* pContext);
		void PostEvent(const void* pContext);

		void GetGridPos(CPos& GridPos)const;
		const CFPos& GetPixelPos()const;
		void GetPixelPos(CFPos& pos);
		ESetPosResult SetGridPos(const CPos& GridPos);
		virtual ESetPosResult SetPixelPos(const CFPos& PixelPos);

		virtual ESetSpeedResult SetSpeed(float fSpeed);
		float GetSpeed()const;
		bool IsMoving()const;
		
		uint32 GetLastMoveID()const;
		virtual EMoveToResult MoveTo(const CFPos& PixelPosDes,float fSpeed
			,EFindPathType eFindPathType,EBarrierType eBarrierType,float fReachDist);
		EMoveToResult MoveToLimit(const CFPos& PixelPosDes, float fSpeed, 
			EFindPathType eFindPathType, EBarrierType eBarrierType,float fReachDist, int32 nMaxStep);
		//�����ṩ����PixelPath��MovedDist������MoveTo�ӿ�.
		EMoveToResult MoveToInPath(const  CPixelPath* pPixelPath, float fSpeed, EFindPathType eFindPathType, EBarrierType eBarrierType, float fReachDist);
		//�����ṩ��ȡMovedDist�Ľӿ�
		float GetMovedDist()const;
		
		virtual EStopMovingResult StopMoving();

		void Watch(IWatchHandler* pHandler, CCoreObjectServer* pTarget, float fPixelDistThreshold);
		void StopWatching(IWatchHandler* pHandler);
		//trace�Ƕ�λΪ��moveΪ������һ�����߼����Զ�move�Ĺ���
		//����ڵ���MoveToʱ��ҪStopTracing,��Traceʱ����ҪStopMoving
		virtual void Trace(CCoreObjectServer* pTarget, float fSpeed
			, EFindPathType eFindPathType, EBarrierType eBarrierType, float fReachDist);
		virtual void StopTracing();

		bool Running()const;
		bool Destroying()const;
		bool Transfering()const;
		
		void QueryObject(vector<CCoreObjectServer*>& vecObj, float fRadius, const CFPos& = CFPos(0, 0)) const;
		
		void SetQuerySize(float fSize);  //����д������Ϊ������setsize����������İ����������������aoi����
		float GetQuerySize() const;


		bool IsTracing()const;

		virtual void LogMsg(const string& log);

	protected:
		CCoreObjectServer(ICoreObjectServerHandler* pHandler, CCoreSceneServer* pScene);
		virtual ~CCoreObjectServer(void);

		CCoreSceneServer* m_pScene;		

	private:
		static MapObject& GetObjectMap();

		bool BeginDestroyObject();
		bool BeginTransferObject(CCoreSceneServer* pScene, const CFPos& PixelPos);

		void TransferDistortedTimeObjTo(CCoreSceneServer* pScene);

		uint32 CreatePosData(const CFPos& pos);
		void ReleasePosData();

		void CreateQuadObject(const CFPos& pos);
		void ReleaseQuadObject();

		EMoveToResult CheckMoving(const CFPos& PixelPosDes,EFindPathType eFindPathType,EBarrierType eBarrierType,int32 nMaxStep=-1);
	
		void ReSetCoreScene(CCoreSceneServer* pCoreScene);

		void ResetMovedDistArg(const CPixelPath* pPath, float fReachDist);
		void IncreaseMoveToSessionID();
		void IncreaseTraceSessionID();
		
		CSyncCoreObjectServer* m_pSynCoreObj;
		uint32	m_uID;

		CAsynCorePos*			m_pPosData;
		CAsynCoreQuadObject*	m_pQuadObject;

		const AoiData& GetAoiData(uint32 uDimension) const;
		typedef map<uint32, AoiData,less<uint32>,TCoreObjAllocator<pair<uint32,AoiData> > > MapDim2AoiData_t;
		MapDim2AoiData_t	m_mapDim2AoiData;

		bool m_bIsMoving;

		static uint32 GetNextPosObserverHandlerID();
		typedef map<uint32, IObjPosObserverHandler*,less<uint32>, TCoreObjAllocator<pair<uint32,IObjPosObserverHandler*> > > MapPosObserverHandler;
		MapPosObserverHandler	m_mapPosObserverHandler;

		float m_fSpeed;

		uint32				m_uGPCNFlagRefCount;
		EAsynObjLifeState	m_eLifeState;

		typedef list<CCoreObjectServer*,TCoreSceneAllocator<CCoreObjectServer*> >				ListObject;
		ListObject::iterator							m_itObjectListInScene;

		bool			m_bIsTracing;
		MovedDistArg*	m_pMovedDistArg;
		uint32			m_uLastMoveID; 
		uint32			m_uTraceSessionID; 

		TPtRefer<CCoreObjectServer, ICoreObjectServerHandler> m_Handler;

		CFPos			m_Pos;
		bool			m_bPosSet;

		CDistortedTimeObj*		m_pDistortedTimeObj;

	};
}
