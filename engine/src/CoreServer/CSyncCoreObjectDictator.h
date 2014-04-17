#pragma once

#include "CSyncCoreObjectServer.h"
#include "CCoreObjectServer.h"
#include "CVariantHolderDef.h"

namespace sqr
{
	class ISend;
	class IMulticast;

	class ISyncCoreObjectDictatorHandler;
	class CSynConnServer;
	class CSyncCoreSceneServer;
	//class CCypherRootVariant;
	class CObjVarDefCfg;
	class CCacheVariantServer;
	class CCacheVariantServerHolder;
	class CAoiCmdBase;
	class CAoiCmdConnAttached;

	struct CStepCallbackArg;
	struct CMoveBeganCallbackArg;
	struct CMoveSpeedChangedCallbackArg;
	struct CMovePathChangedCallbackArg;
	struct CMoveStoppedCallbackArg;
	struct CSetObjectPosCallbackArg;
	struct CSyncObjectPosCallbackArg;
	struct CSendCallbackArg;
	struct CIntEnableFollowerDelayCallbackArg;

	class CCoreObjectDictator;

	template<typename Traits>
	class TBindObject;

	class CSyncCoreObjectDictator
		:public CSyncCoreObjectServer
	{
		friend class CCoreObjectDictator;
		friend class CSyncCoreObjectServer;
		friend class CSyncCoreObjectMediator;
		friend class CAoiCmdConnAttachVeiwee;
		friend class CAoiCmdConnDetachVeiwee;
		friend class TBindObject<CSyncCoreObjectDictator>;
	public:
		typedef ISyncCoreObjectDictatorHandler	Handler_t;
		typedef vector<CCacheVariantServerHolder*,TCoreObjAllocator<CCacheVariantServerHolder*> > VecSyncVariantServerHolder_t;

		ISyncCoreObjectDictatorHandler* GetHandler()const;

		CCacheVariantServer* GetSyncVariant(ECodeTableIndex eIndex)const;
		CCacheVariantServer* GetViewVariant(ECodeTableIndex eIndex)const;

		CCacheVariantServerHolder* GetSyncVariantHolder(ECodeTableIndex eIndex)const;
		CCacheVariantServerHolder* GetViewVariantHolder(ECodeTableIndex eIndex)const;

		EServerObjectType GetType()const;
		ISend* GetIS(uint32 uRange)const;

		static CObjVarDefCfg* GetVarDefCfg();

		virtual void BroadCastForViewAoi(const void* pData,uint32 uSize)const;
		virtual void BroadCastForSyncAoi(const void* pData,uint32 uSize)const;
		void EnableFollowerDelay(bool bEnable);
		uint32 GetFollowerDelay()const;
		bool IsFollowerDelayEnabled()const;

		void SetBarrier(float fBarrierSize, EBarrierType eBarrierType);
		bool HasBarrier()const;
		bool GetBarrierType(EBarrierType& eBarrierType)const;
		uint8 GetBarrierSizeInGrid()const;	//��Ϊ�ϰ���������С��λΪ����,�ú������ص����ϰ��ĸ��Ӱ뾶
		float GetBarrierSize()const;

		virtual float Distance(const CSyncCoreObjectServer* pCoreObj)const;
		void LogMsg(const string& log);

		void BroadCastForViewAoiExtraConn(const void* pData,uint32 uSize,CSynConnServer* pExtraConn)const;
		void BroadCastForSyncAoiExtraConn(const void* pData,uint32 uSize,CSynConnServer* pExtraConn)const;
		void StepExtraConn(CSynConnServer* pExtraConn,float fMovedDist);
		void MoveBeganExtraConn(const CMoveBeganDerivedArgs& Args,CSynConnServer* pExtraConn);
		void MoveEndedExtraConn(CSynConnServer* pExtraConn);
		void MoveStoppedExtraConn(float fMovedDist,CSynConnServer* pExtraConn);
		void MovePathChangedExtraConn(const CMovePathChangedDerivedArgs& Args,CSynConnServer* pExtraConn);
		void MoveSpeedChangedExtraConn(const CMoveSpeedChangedDerivedArgs&,CSynConnServer* pExtraConn);
		void SyncPixelPosExtraConn(bool bForceSync,CSynConnServer* pExtraConn);
		void EnableFollowerDelayExtraConn(bool bEnable,CSynConnServer* pExtraConn);

		//����������������������Ϣ
		void TellClientSyncRelationChanged(CSyncCoreObjectMediator* pObjMediator,bool bSee, uint32 uDimension);
		void TellClientViewRelationChanged(CSyncCoreObjectMediator* pObjMediator,bool bSee, uint32 uDimension);

		void SyncBarrierExtraConn(CSynConnServer* pExtraConn)const;

		void OnSyncBindingRelation(uint32 uParentID, bool bRet);
		void OnSyncDisbindingParentRelation();
		void OnSyncDisbindingAllChildRelation();

		bool BindTo(uint32 uParentID);
		bool DisbindFromParent();
		bool DisbindChild(uint32 uChildID);
		bool DisbindAllChildren();
		bool IsBindingObj() const;

		//BindObj�����߳�����Ϊͬ�����ͻ��ˣ�ͬ��������Ѳ���Ҫ�����߼��߲㣻���Կ��Ǽ���bRetΪfalse�׳��쳣
		//BindObj�߼��߳��������������߳�
		void OnBindTo(uint32 uParentID, bool bRet);
		//ͬ�ϣ���Ŀǰ����ʱ����Ҫͬ�����ͻ��ˣ����Ժ�����Ϊ�գ����Կ��Ǽ���bRetΪfalse�׳��쳣
		void OnDisbindFromParent(bool bRet);
		void OnDisbindToChild(uint32 uChildID, bool bRet);
		void OnDisbindToAllChild(bool bRet);
		//���Ϻ���ר�õĸ�������
		void OnBindToAoiDone(uint32 uParentID, bool bRet);	
		//virtual void OnSyncBindingRelation(uint32 uParentID, bool bRet) {};

		//BindObj�������������õĻص����߼��̣߳��޸�AOI��ר��
		void OnDisbindParentSingle() {};
		void OnDisbindChildSingle() {};
		void OnBeforeBindTo(uint32 uParentID, bool bRet) {};	//�ͻ������Ԥ�󶨹�ϵҲҪ�ã����������ֹͣ�ƶ����߼��������̲߳��ظ�ֹͣ

		//BindObj�ͻ��ˣ�Ԥ�󶨹�ϵ��ר��
		void OnBeforeDisbindFromParent() {};
		void OnBeforeDisbindToChild(uint32 uChildID) {};
		void OnBeforeDisbindToAllChild() {};
		
		bool BindObjIntSetPosition(const CFPos& PixelPos);
		void BindObjNotifyPosChangedIfNeed();
	protected:
		friend class CSyncCoreSceneServer;
		friend class CCacheVariantServerHolder;
		typedef vector<CPos> BarrierArea;

		friend class CAoiViewObjHandler;
		friend class CAoiSyncerObjHandler;
		friend class CAoiSynceeObjHandler;

		CSyncCoreObjectDictator(uint32 uObjID, CSyncCoreSceneServer* pScene,const CFPos& PixelPos,uint32 uVarDefID);
		~CSyncCoreObjectDictator(void);
		
		//Dictator�����Mediator���󴴽��ͻ��˶���
		void SendCreateStillObjCmd(EStillObjFlag eStillObjFlag,CSynConnServer* pConn);

		virtual void SendCreateObjCmdForConn(CSynConnServer* pConn);
		void CreateFollowerForConnection(CSynConnServer* pConn, uint32 uSessionID);
		void SyncViewVariantHolder(CSynConnServer* pConn, bool bSyncStaticView, bool bSyncAgileView);
		void SyncSyncVariantHolder(CSynConnServer* pConn, bool bSyncStaticSync, bool bSyncAgileSync);
		void DestroyObjForConnection(CSynConnServer* pConn);

		void EnumConnection(const set<CSyncCoreObjectMediator*>& setMedObj,void* pArg,void (*)(CSynConnServer*,void*),CSynConnServer* pExtraConn)const;
		
		//--------��������Ϊ��ʵ��CSyncCoreObjectServer���麯��----------
		void OnMoveSteppedDerived(const CFPos&);
		void OnMoveBeganDerived(const CMoveBeganDerivedArgs& Args);
		void OnEndMoveDerived();
		void OnMoveStoppedDerived(float fStopDist);
		void OnMovePathChangedDerived(const CMovePathChangedDerivedArgs& Args);
		void OnMoveSpeedChangedDerived(const CMoveSpeedChangedDerivedArgs& Args);
		void OnPixelPosSetDerived(bool bSuperPosition);
		//********��������Ϊ��ʵ��CSyncCoreObjectServer���麯��

		//����AOI��Ұ���Զ������ͻ��˷���������Ϣ(�������,�߼����).Ϊ�˱��������Ұ��������Ϣ���߼���Ϣ֮�䲻�����һ���뿪��Ұ��������Ϣ,�Ͳ�����OnSync/ViewRelationChanged�ص���ֱ�ӷ���������Ϣ,ֻ�ܽ����߼��߳���ͳһ������������Ϣ�ķ���ʱ��
		void OnSyncRelationChanged(CSyncCoreObjectMediator* pObjMediator,bool bSee, uint32 uDimension);
		void OnViewRelationChanged(CSyncCoreObjectServer* pObjServer,bool bSee, uint32 uDimension);
		
		virtual void IntEnableFollowerDelay(bool bEnable);

		inline uint32 GetVarDefID() const
		{
			return m_uVarDefID;
		}

		void OnMsgFromViewer(const void* pContext);
		//void OnMsgToVieweeHandled(CSyncCoreObjectServer* pObj, const void* pContext, uint32 uDimension);

		void PostMsgToSyncee(const void* pContext);
		void OnMsgFromSyncee(const void* pContext);
		void OnMsgToSynceeHandled(const void* pContext);

		virtual void IntSetPositionDerived(const CFPos& PixelPos);

	private:
		uint32 m_uVarDefID;

		IMulticast* m_pSyncMulticast;
		IMulticast* m_pViewMulticast;

		EBarrierType	m_eBarrierType;	
		float		m_fBarrierSize;
		bool		m_bEnableFollowerDelay;
		VecSyncVariantServerHolder_t m_vecSyncVariantHolder;
		VecSyncVariantServerHolder_t m_vecViewVariantHolder;

		void MulticastSendCmd(IMulticast*pMulticast, const void* pData, uint32 uSize)const;

		virtual void SyncBarrier()const;
		bool UpdateSceneBarrier(EBarrierType eBarrierType);
		void ClearBarrier();
		
		static void TellStepCallback(CSynConnServer* pConn,CStepCallbackArg* pArg);
		static void TellMoveBeganCallback(CSynConnServer* pConn,CMoveBeganCallbackArg *pArg);
		static void TellMoveEndedCallback(CSynConnServer* pConn,CSyncCoreObjectDictator *pArg);
		static void TellMoveStoppedCallback(CSynConnServer* pConn,CMoveStoppedCallbackArg *pArg);
		static void TellMovePathChangedCallback(CSynConnServer* pConn,CMovePathChangedCallbackArg *pArg);
		static void TellMoveSpeedChangedCallback(CSynConnServer* pConn,CMoveSpeedChangedCallbackArg *pArg);
		static void SetObjectPosCallback(CSynConnServer* pConn,CSetObjectPosCallbackArg* pArg);
		static void SyncObjectPosCallback(CSynConnServer* pConn,CSyncObjectPosCallbackArg* pArg);
		static void SendCallback(CSynConnServer* pConn,CSendCallbackArg* pArg);
		static void IntEnableFollowerDelayCallback(CSynConnServer* pConn,CIntEnableFollowerDelayCallbackArg* pArg);
		static void MulticastCmdForAoi(const void* pData,uint32 uSize, IMulticast* pMulticast);
		
		void BroadCastForAoiExtraConn(const void* pData,uint32 uSize,CSynConnServer* pExtraConn, IMulticast* pMulticast)const;

		void IntBeginTransferObject(CSyncCoreSceneServer* pScene , const CFPos& PixelPos);

		TBindObject<CSyncCoreObjectDictator>*			m_pBindObj;
#ifdef LOG_COREOBJ_MOVE
		static uint32& GetChangeFolMoveTargetSessionID();
		//uint32	m_uChangeFolMoveTargetCount;
#endif
	};
}
