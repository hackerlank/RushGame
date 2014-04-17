#pragma once


//����Core����Ļ���
#include "CPos.h"
#include "CDynamicObject.h"
#include "FindPathDefs.h"
#include "CoreObjectDefs.h"
#include "CCoreObjMallocObject.h"
#include "TCoreSceneAllocator.h"


namespace sqr
{
	//********************************************************************************************
	//
	//		ע�⣺�����ǿͻ������ж���ͷ�������ж���Ĺ��������ģ�棬�ǳ����࣬���ܶ�����������
	//	    ���ڿͻ��˶���ͷ���˶����ǲ���Ҫ������ʱ����Ϊ��ͬ�Ļ������ʹ�õģ�Ϊ������˼·�����
	//
	//			    + + + + + + + +��Ҫ���������������κ��麯��+ + + + + + + +
	//
	//*********************************************************************************************

#ifdef GetObject
#undef GetObject
#endif

	template<typename ValueType>
	class TIDPtrMap;

	class CPixelPath;
	class CMedObjFollowState;
	class CTestSuiteCoreObjectServer;

	template<typename Traits>
	class TCoreScene;

	template<typename Traits>
	class TObjMoveState;

	template<typename Traits>
	class TTimeSystem;

	template < class HolderType, class PointeeHolderType >
	class TPtRefee;

	template<typename Traits>
	class TBaseCOEvent;

	class CCoreObjectClient;
	class CSyncCoreObjectServer;

	class IDistortedTime;
	class CDistortedTimeObj;

	template<typename Traits>
	class TCoreObject
		:public virtual CDynamicObject
		,public CCoreObjMallocObject
	{
		typedef typename Traits::CoreObject_t			ImpCoreObject_t;
		typedef typename Traits::ObjMoveState_t			ImpObjMoveState_t;
		typedef typename Traits::CoreObjectHandler_t	ImpHandler_t;
		typedef typename Traits::CoreScene_t			ImpCoreScene_t;
		typedef typename Traits::MetaScene_t			ImpMetaScene_t;
		typedef typename Traits::TimeSystem_t			ImpTimeSystem_t;
		typedef TTimeSystem<Traits>						TimeSystem_t;

		friend class CCoreObjectClient;
		friend class CSyncCoreObjectServer;
		//friend class TBindObject<ImpCoreObject_t>;

	public:
		//--------���к������ⲿ�����--------

		static ImpCoreObject_t* GetObject(uint32 uLocalID);

		ImpCoreScene_t* GetScene()const;

		void SetHandler(ImpHandler_t* pHandler);
		ImpHandler_t* GetHandler()const;

		void GetGridPos(CPos& GridPos)const;

		const CFPos& GetPixelPos()const;
		void GetPixelPos(CFPos& pos);
		uint32	GetLocalID()const;

		float GetSpeed()const;

		IDistortedTime* GetDistortedTime()const;

		bool IsBindingObj() const {return false;}

		//********����ĺ������ⲿ�����********

	protected:

		friend class CTestSuiteCoreObjectServer;
		//----------���к���ר�����������------------

		friend class TCoreScene<Traits>;
		friend class TObjMoveState<Traits>;
		friend class CMedObjFollowState;
		template<typename>
		friend class TBaseCOEvent;

		TCoreObject(ImpHandler_t* pHandler,const CFPos& PixelPos,ImpCoreScene_t* pScene);
		~TCoreObject();

		/*
		@.Int��Ϊǰ׺�ĺ����Զ����״̬��ֵ�����������޸ģ������ڱ���������״̬���ն���ʹ��Intϵ�к������޸Ķ���״̬
		@.��Intϵ�к������ڸ��û��Դ�������״̬�Ķ���ֱ�Ӳ�������Щ�������ж϶���ĵ�ǰ����״̬�������ɹ����ߴ�����
		@.Derived��Ϊ��׺�ĺ�������������д���Ա㷢����������
		@.Int��Ϊǰ׺��ͬʱDerived��Ϊ��׺�ĺ������������������޸ĸ������ֵ(����Visible��Observer)��Ҳ������ɷ�����
		������ȹ��ܡ���Ҫ�ǳ�ע����ǣ������͵ĺ�����Ӧ���������ഥ���κλص�������
		*/

		ESetSpeedResult IntSetSpeed(float fSpeed);
		//���������MoveTo�����Ƿ�ִ�У�Follower�������ǲ��ᱻִ�У�Director����������Ƿ�������״̬�������ܷ�ִ�У�
		EMoveToResult IntMoveTo(const CFPos& PixelPosDes,float fSpeed,EFindPathType eFindPathType,
			EBarrierType eBarrierType,float fReachDist, float fMovedDist, CPixelPath* pPixelPath, int32 nMaxStep=-1);

		EStopMovingResult IntStopMoving();
		//�������λ�ñ��ⲿ���޸�ʱ��������Ը��ݶ����״̬�����Ƿ����λ���޸ģ�������SetPixelPos�����������������õ��á�
		ESetPosResult IntSetPixelPos(const CFPos& PixelPos);


		//����һ����ָ����㵽ָ��Ŀ�����ƶ���Ϊ,DoMove�Լ��ǲ����޸Ķ����λ�õġ�
		//ֻ���ƶ��еĶ�������ٶȡ�
		//uPixelSpeed Pixel per second
		//fBeginDist�Ӿ������ָ�������λ�ÿ�ʼ�ƶ�����Ҫ���ڴ����ƶ��е�Follower�������ֶ����ڴ���ʱ�̾ʹ��������ƶ�·�����м��ĳ��λ�á�
		//fReachDist�ƶ��������յ���ô�����ʱֹͣ��
		//����false��ʾĿ����뵱ǰ���غ�
		EMoveToResult DoMove(const CFPos& PixelPosBegin,const CFPos& PixelPosEnd,float fSpeed
			,EFindPathType eFindPathType,EBarrierType eBarrierType,float fBeginDist, CPixelPath* pPath, int32 nMaxStep=-1);
		void DoMove(CPixelPath* pPath,float fSpeed,float fBeginDist);
		//����false��ʾԭ�����Ѿ�����ֹͣ״̬
		bool IntStopDoingMove();
		bool IntIsDoingMove()const;
		//return false��ʾ��Χ����������С	//�����޸���ֵ
		void RawSetPixelPos(const CFPos& PixelPos);
		bool IntSetPositionSingle(const CFPos& PixelPos);
		bool IntGetCurPixelWayPoint(CFPos& PixelPos)const;


		//�����������������޸Ķ����λ�ã�dictator������޸�visible��λ�ã�mediator������޸�Observer��λ��
		//IntSetPositionDerived����ֱ���趨�����λ�ñ仯
		void IntSetPositionDerived(const CFPos& PixelPos);	//return false��ʾ������ͼ��Χ

		ImpObjMoveState_t* GetMovState()const;

		bool IntSetPosition(const CFPos& PixelPos);
		//ESetPosResult IntSetPixelPos( const CFPos& PixelPos );

		float GetRealMovedDistDerived()const;

		CDistortedTimeObj* GetDistortedTimeObj()const;
		//******����ĺ���ר�����������

	private:
		//-----------���к���ר��TObjMoveState������-----------

		void IntOnMoveEnded();


		//����˶���Ӧ��Step������ʱ���ƶ����ȸ��߿ͻ��˶���
		//�ͻ��˶���Ӧ��Step������ʱ�����Լ����ƶ��ٶ�

		//******����ĺ���ר��TObjMoveState������



		//------------��������ר���Լ�ʹ��----------------
		uint32				m_uID;
		ImpHandler_t*		m_pHandler;
		ImpObjMoveState_t*	m_pMovState;
		CFPos				m_Pos;


		typedef TPtRefee< ImpCoreObject_t , TBaseCOEvent<Traits> >	RefeeByEvent_t;
		RefeeByEvent_t*	m_pRefByEvent;

		typedef TIDPtrMap<ImpCoreObject_t>			MapObject_t;
		typedef list<ImpCoreObject_t*,TCoreSceneAllocator<ImpCoreObject_t*> >				ListObject_t;

		static MapObject_t& GetObjectMap();

		typename ListObject_t::iterator		m_itObjectListInScene;

		void OnWayPointChangedDerived();

		CDistortedTimeObj*		m_pDistortedTimeObj;
		//********���������ר���Լ�ʹ��****************
	};
}
