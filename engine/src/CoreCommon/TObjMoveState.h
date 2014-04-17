#pragma once
//$id$
//������ƶ�״̬�࣬���𱣴��ƶ�״̬�����ݣ��Լ�ִ���ƶ�����

#include "CDistortedTick.h"
#include "CRenderSlot.h"
#include "TimeHelper.h"
#include "CCoreObjMallocObject.h"

#ifdef GetObject
#undef GetObject
#endif

namespace sqr
{	
	template<class T> struct TPos;
	typedef TPos<int32> CPos;
	typedef TPos<float> CFPos;

	class CPixelPath;

	template<typename Traits>
	class TCoreObject;

	template<typename Traits>
	class TObjMoveState
		:public Traits::Tick_t
		,public CCoreObjMallocObject
	{
		typedef typename Traits::CoreObject_t		ImpCoreObject_t;
		typedef typename Traits::ObjMoveState_t		ImpObjMoveState_t;
		typedef typename Traits::TimeSystem_t		TimeSystem_t;

		friend class TCoreObject<Traits>;
	public:
		ImpCoreObject_t* GetObject()const;

		const CPixelPath* GetPath()const;
		
		void GetCurWayPoint(CFPos& PixelPos)const;
		float GetPathLength()const;

		//return false��ʾMovedDist�Ѿ�����EndDist
		bool SetEndDist(float fEndDist);
		bool SetReachDist(float fReachDist);

		float GetMovedDist()const;
		float GetRealMovedDist()const;
		//��ʵϵͳʱ�������������,�ó�����Ŀǰ��׼������
		void GetRealCurPos(CFPos& PixelPos);

		uint32 GetMoveCyc()const;
		void SetMoveCyc(uint32 uMoveCyc);

		bool SetSpeed(float fSpeed);
		float GetSpeed()const;
	protected:
		TObjMoveState(ImpCoreObject_t* pObj,CPixelPath* pPath,float fSpeed,float fBeginDist);
		virtual ~TObjMoveState();

		bool NeedSkipFrameDerived()const;
		void CreateTickingDerived(uint32 uCyc);
		void DeleteTickingDerived();
		void OnTick();

		void IntSetSpeedDerived(float fSpeed);

		float IntGetNewMoveDistDerived()const;

		void IntSetNewPath(CPixelPath* pPath,float fBeginDist);
		void SetNewPathDerived(CPixelPath* pPath,float fBeginDist);

	private:		
		bool CheckLastStep();

		void ResetUpdatePosTcp();

		bool Reached()const;

		float GetNewMoveDist()const;

		//�˺���ʹ�õ�����ʵϵͳʱ������������㣬��Tick���߼�ʱ������ȫ��ͬ�ģ�
		//��Ӧ��ֻ�����ڽ���ĳ���ƶ�������������㣬��Ӧ���������ƶ��Ĺ�����ʹ�á�
		void UpdateCurPosByRealTime();
		//������߼��̷߳�������MoveTo����ChangeMovePath֮ǰ
		//��Ҫ�����߼��̶߳����MovedDist�����������̶߳��������
		void UpdateCurPosByMovedDist(float fMovedDist);

		float IntGetRealMovedDist()const;

		ImpCoreObject_t*	m_pObject;
		CPixelPath*			m_pPath;
		float				m_fSpeed;
		float			m_fMovedDist;
		float			m_fEndDist;
		//uint32			m_uMoveCyc;
		float			m_fLastUpdateDist;
		uint16			m_uCurWayPoint;

		//CTimeCheckPoint		m_tcpThisPath;		//�ƶ�����ʼʱ��
		uint64			m_uThisPathExpiredTime;		//�ƶ�����ʼʱ��

	};
}


