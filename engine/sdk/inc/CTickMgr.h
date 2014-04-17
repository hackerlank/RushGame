#pragma once

#include "CommonDefs.h"
#include "BaseTypes.h"
#include "CTick.h"
#include "TTickHandler.h"
#include "CTickMallocAllocator.h"
#include "TIDPtrMap.h"

#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable:4251)
#endif


namespace sqr
{
	template<typename,template<typename>class>
	class TObjectBank;
	class CExpCounter;

	class CTickQueue;
	class CTickSlot;
	class CTick;
	
	class COMMON_API CTickMgr
		:public CTick
	{
		friend class CTickImp;
		friend class CTestSuiteTickMgr;
		friend class CTickSlot;
		typedef std::vector< CTickSlot*, CTickMallocAllocator<CTickSlot*> > VecTickSlot_t;
	public:
		//uTickCyc Tick������=���ٺ���Tickһ��
		explicit CTickMgr(uint32 uTickCyc,uint16 uSlotNum);
		~CTickMgr();
		uint32 GetInterval()const;
		void OnTick();

		//uInterval Tick������=���ٺ���Tickһ��
		void Register(CTick*,uint32 uInterval);
		void UnRegister(CTick*);

		//�õ���ǰע��tick����ϸ��Ϣ��tick���֣�tick����
		void GetRegisterTickInfo(string& strInfo);
		//�õ���ǰע��tick��������
		size_t GetRegisterTickNum();

	private:
		CTickImp * m_pLastTickImp; //�ڵ���OnTickǰ����Ҫ���õ�Tickָ�뱣�����������Լ��Ƿ���OnTick�б�ɾ��
		uint32 m_uRing; //������
		uint32 m_uCurPos; //��ǰ�ڼ�����
		uint32 m_uInterval; //����ʱ�䣬��һ���������ú�������λ��

		VecTickSlot_t m_vecSlot;

		typedef TObjectBank<CTickQueue, CTickMallocAllocator>	QueueBank_t;
		QueueBank_t*					m_pTickQueueBank;

		typedef TObjectBank<CTickImp, CTickMallocAllocator>		ImpBank_t;
		ImpBank_t*						m_pTickImpBank;
		
		typedef TIDPtrMap<CTickImp>	MapTickImp_t;
		MapTickImp_t	m_mapRegisteredTick;

		//�ڵ���OnTick���棬���浱ǰ��queue����ֹע��tick֮���queueΪ���˱�ɾ��
		CTickQueue* m_pLastTickQueue; 
									   
		void PrintAndUnregisterTick();
		void OnCaughtTickException(CTick* pTick);

		inline bool CallTick(CTickImp* pTick);
	};
}

#ifdef _WIN32
#pragma warning(pop)
#endif

