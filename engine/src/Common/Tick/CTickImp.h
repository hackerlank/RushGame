//$Id: CTickImp.h 54290 2007-04-02 08:19:54Z shhuang $
#pragma once
#include "CommonDefs.h"
#include <deque>
#include <list>
#include "CTickMallocObject.h"
#include "CTickMallocAllocator.h"

class CTestSuiteTickMgr;

#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable:4251)
#endif

namespace sqr
{
	class CTickMgr;
	class CTickSlot;
	class CTick;
	
	class  COMMON_API CTickImp : public CTickMallocObject
	{
		friend class CTickMgr;
		friend class CTickSlot;
		friend class CTestSuiteTickMgr;

	public:
		uint32 GetInterval()const;

		CTickImp();
		~CTickImp();

		void SetTickName(const char* szName);
		const char* GetTickName()const;	

		uint32 GetLeftTime()const;

		void UnRegister();

	private:
		uint32						m_uQueuePos;
		uint32						m_uKeyOfTickMap;		//������CTickMgr��Infolist��λ�ã�����ʱɾ��

		uint16						m_uSlotPos;
		uint32						m_uInterval;		//����ʱ�䴦��һ��,��������λ
		uint32						m_uRing;			//���ֿ�ʼ����

		//���������˫�غ���
		//��TickInterval���ڵ���TickMgrIntervalʱ����¼����Tick�����ͺ�ĺ�����
		//��TickIntervalС��TickMgrIntervalʱ����¼����Tick��һ�ε��õĶ��������
		uint32						m_uLoan;
		CTickMgr*					m_pTickMgr;			//�����ĸ�mgr�ϣ������ʱ��Ϊ��

		typedef basic_string<char,std::char_traits<char>,CTickMallocAllocator<char> > TickName_t;
		TickName_t					m_sTickName;
		const char*					m_szClassName;

		void AddToSlot(uint16 uSlotPos,uint32 uRing);
		void DelFrSlot();
		CTickSlot*	GetSlot()const;

		void OnCaughtTickException(CTick* pTick);

		CTick*	m_pTick;
	};
}

#ifdef _WIN32
#pragma warning(pop)
#endif
