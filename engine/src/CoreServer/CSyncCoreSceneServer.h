#pragma once
#include "TCoreScene.h"
#include "CSyncTraitsServer.h"
#include "ISend.h"
#include "TIDPtrMap.h"


namespace sqr
{
	template<class T> struct TRect;
	typedef TRect<int>		CIRect;

	class IMulticast;
	class CSyncMetaSceneServer;

	class ICoreObjectMediatorHandler;
	class ICoreObjectDictatorHandler;
	class ICoreObjectCalculatorHandler;

	class CCoreGridServer;
	class CConnServer;

	class CSyncCoreObjectServer;
	class CSyncCoreObjectServer;
	class CSyncCoreObjectMediator;
	class CSyncCoreObjectDictator;
	class CCoreObjectCalculator;

	class CObjVarDef;

	class CAsyncContext;

	class CAsyAoiViewObj;
	class CAsyAoiScene;
	class ICoreGridServerHandler;

	class CSyncCoreQuadScene;

	class CCoreSendSceneCmdJob;

	class CSyncDistortedTimeSceneServer;
	class CSyncDistortedTimeObjServer;
	class CDistortedTimeObj;

	class CDimensionInfo;

	class CSyncCoreSceneServer
		:public TCoreScene<CSyncTraitsServer>
		,public ISend
	{
		typedef vector<CSyncCoreSceneServer*,TCoreSceneAllocator<CSyncCoreSceneServer*> > IDVecSynCoreSceneServer;
	public:
		ISend* GetIS()const;

		void CreateObjectMediator(uint32 uObjID, const CFPos& PixelPos,uint32 uVarDefID, uint32 uSlot, float fDirectorMaxSpeed);
		void CreateObjectDictator(uint32 uObjID, const CFPos& PixelPos,uint32 uVarDefID, uint32 uSlot);
		void CreateObjectCalculator(uint32 uObjID, const CFPos& PixelPos, uint32 uSlot);

		CSyncDistortedTimeObjServer* CreateDistortedTimeObj();
		void DestroyDistortedTimeObj(CDistortedTimeObj* pObj);
		void SetTimeDistortedRatio(float fTimeDistortedRatio);
		float GetTimeDistortedRatio()const;

		//return false��ʾ�����Ѿ���������ɾ��״̬��
		bool DestroyObject(CSyncCoreObjectServer* pObject);

		CSyncMetaSceneServer* GetMetaScene()const;

		bool TransferObject( CSyncCoreObjectServer* pObj, CSyncCoreSceneServer* pScene, const CFPos& PixelPos );

		bool Destroying()const;
		bool IsPixelValid(const CFPos& PixelPos)const;

		uint32 GetGlobalID()const {return m_uID;}
		static CSyncCoreSceneServer* GetSynCoreSceneServer(uint32 uID);

	private:
		typedef TCoreScene<CSyncTraitsServer>	ParentScene_t;
		friend class CSyncCoreObjectMediator;
		friend class CSyncCoreObjectDictator;
		friend class CSyncCoreObjectServer;
		friend class TCoreSceneMgr<CSyncTraitsServer>;
		friend class CSyncCoreSceneMgrServer;
		friend class TPart<ImpCoreScene_t,ImpCoreSceneMgr_t>;
		friend class TPart<ImpCoreScene_t,ImpMetaScene_t>;
		friend class TCoreScene<CSyncTraitsServer>;
		friend class CSyncMetaSceneServer;
		friend class CCoreSendSceneCmdJob;
		friend class CAoiSceneHandler;


		IMulticast*				m_pMulticast;

		uint32 Send(const void* pData,uint32 uSize);

		CSyncCoreSceneServer(uint uID, CSyncMetaSceneServer* pSceneServer, CSyncCoreSceneMgrServer* pMgr
			, const char* szSyncAoiFileName, CDimensionInfo* DimInfo);
		~CSyncCoreSceneServer(void);

		//uint32	m_uViewAoiRadius;

		CAsyAoiScene *m_pAoiScene;

		CAsyAoiScene* GetAoiScene() const;

		bool HaltAoiScene();

		void CreateAoiObj(CSyncCoreObjectServer* pObj);
		void DestroyAoiObj(CAsyAoiViewObj* pAoiObj);

		void MulticastSendCmd(IMulticast*pMulticast, const void* pData, uint32 uSize);
		void AddToMulticast(CSynConnServer*);
		void DelFromMulticast(CSynConnServer*);

		//return false��ʾ�����Ѿ�����ɾ��״̬
		bool BeginDestroyScene();
		//��������ᱻAoi Scene OnHalted��Ϣ������
		void EndDestroyScene();

		//�����Լ�ɾ���Լ����Ժ�֪ͨ��������������������ʱ���������У������Ƿ����ж���ɾ���ɾ��ˣ�������򳡾�����Ҳ���Կ�ʼ�����ˡ�
		void OnObjectDestroyed();
		void SetDynamicBarrierInfo(uint32 uOwnerID, const DynamicBarrierArea& dynamicBarrier, EBarrierType eBarrierType);


		void ConfirmObjectCreateCondition(const CFPos& PixelPos);
		void ConfirmObjectCreateSceneState(CSyncCoreObjectServer* pObj);

		void ClearObjectInDestroyingSceneList();

		void CreateQuadScene(uint32 uSlot);
		void ReleaseQuadScene();

		static IDVecSynCoreSceneServer& GetIDVecCCoreSCeneServer();

	private:
		list<CSyncCoreObjectServer*,TCoreSceneAllocator<CSyncCoreObjectServer*> >	m_listObjectInDestroyingScene;

		CSyncCoreQuadScene*						m_pQuadScene;

		uint32									m_uID;
		bool									m_bDestroying;
		CSyncDistortedTimeSceneServer*			m_pDistortedTimeScene;

	};
}
