#pragma once
#include "TMemPoolAllocator.h"
#include "CMirrorMallocObject.h"

namespace sqr
{
	/*
		����rb����һ������֮�����ǻᴴ��һ��cmd������patch��ÿ��rb��ib������ʱ��
		���cmd�ͻ�ִ��
		����wb������Ӧ������������ɾ����ʱ�����ǻ������������ڵ�slot����һ��discarded�б�
		Ȼ�󽻻�wb,ib���ڽ���ib,rb����ʱ��rb�����cmd�ͻ�ִ�У�������ִ��ǰ�����Ȼ���
		��cmd�����Ӧ��slot�ǲ�����discarded�б����棬����ڣ���ô�Ͷ������cmd����ִ��
	*/

	class CTrDiscardedCmdMgr : public CMirrorMallocObject
	{
	public:
		CTrDiscardedCmdMgr();
		~CTrDiscardedCmdMgr();

		void AddDiscardedCmd(uint32 uSlot);
		
		void SwapWIDiscardedCmd();
		void SwapRIDiscardedCmd();

		bool IsDiscardedCmd(uint32 uSlot);
		void ClearDiscardedCmd();

	private:
		void CreateWBMem();
		void CreateIBMem();

	private:
		typedef TMemPoolAllocator<uint32>	SlotAlloc_t;
		typedef set<uint32, less<uint32>, SlotAlloc_t>	SlotSet_t;

		SlotAlloc_t*	m_pIBAlloc;
		SlotAlloc_t*	m_pRBAlloc;

		typedef deque<uint32, CMirrorMallocAllocator<uint32> >	QueCmd_t;
		QueCmd_t	m_queWBDiscardedCmd;
		SlotSet_t*				m_pIBDiscardedCmd;
		SlotSet_t*				m_pRBDiscardedCmd;

	};

}
