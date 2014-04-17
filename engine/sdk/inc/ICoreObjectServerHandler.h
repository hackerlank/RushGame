#pragma once
#include "CDynamicObject.h"
#include "CoreObjectDefs.h"
#include "CPos.h"
#include "PatternRef.inl"

namespace sqr
{
	class ICoreObjectDictatorHandler;
	class ICoreObjectCalculatorHandler;
	class CCoreSceneServer;
	class CCoreObjectServer;

	class ICoreObjectServerHandler : public virtual CDynamicObject
	{
		friend class CCoreObjectServer;

	public:
		ICoreObjectServerHandler() { m_RefsByCoreObj.SetHolder(this); }

		//��ʼ�ƶ����Ѿ������ƶ�״̬���������껹û�б仯
		virtual void OnMoveBegan() {}
		//�ƶ�·���Ѹı�
		virtual void OnMovePathChanged() {}
		//�Ѿ�����Ŀ��㣬�Ѿ����ھ�ֹ״̬
		virtual void OnMoveEnded(EMoveEndedType, uint32 uMoveID) {}

		virtual void OnTraceEnded(ETraceEndedType) {}

		//���pOldScene��NULL,��˵��ת����ʧ�ܡ�ԭ����Ŀ�곡���Ѿ������١�
		//�ú���������֮ǰ��������ƶ�״̬����ֹͣ��OnMoveEnded������OnTransfered�ص�֮ǰ������
		//DestroyObject�����½����е�ת������Ϊ��ȡ����OnTransfered�ص�Ҳ�ᱻȡ����
		virtual void OnTransfered(CCoreSceneServer* pOldScene, const CFPos& OldPos) {}

		virtual void OnAoiMsgFromSelf(const void* pContext){}

		//��PostEvent���͸���ǰ������¼���
		virtual void OnEvent(const void* pData) {};

		virtual void OnCaughtViewSightOf(uint32 uObjGlobalId, uint32 uDimension) {}
		virtual void OnLostViewSightOf(uint32 uObjGlobalId, uint32 uDimension) {}

		virtual void OnDestroy() {}

		//BindObjͬ�����岻��Ҫ�����߼��߳�
		//virtual void OnBindToParent(uint32 uObjID, bool bRet) {}
		//virtual void OnBindToChild(uint32 uObjID, bool bRet) {}
		//virtual void OnDisbindFromParent(uint32 uObjID, bool bRet) {}
		//virtual void OnDisbindToChild(uint32 uObjID, bool bRet) {}

		virtual ICoreObjectDictatorHandler* CastToObjDicHandler()const
		{
			return NULL;
		}
		virtual ICoreObjectCalculatorHandler* CastToObjCalHandler()const
		{
			return NULL;
		}
		virtual void* GetTag() const
		{
			return NULL;
		}
		virtual ~ICoreObjectServerHandler(){}
	private:
		TPtRefee<ICoreObjectServerHandler, CCoreObjectServer> m_RefsByCoreObj;
	};
}


