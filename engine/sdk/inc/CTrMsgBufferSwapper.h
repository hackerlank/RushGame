#pragma once
#include "ThreadTypes.h"
#include "CTrMsgMallocAllocator.h"

namespace sqr
{
	/*
		���߳���Ϣbuffer��������

		������Ҫ�����ģ�������µ��߳�ģ��

		��һ�����̣߳���Ҫ�����ַ�left msg�ʹ���right msg
		�ж�����̣߳���Ҫ��������left msg�������߳�Ͷ��right msg

		��Ϣ��Ͷ������ȷָ��Ŀ�꣬����������岢���ʺ���Ϣ��Ͷ�ݸ����⹤���̵߳�ģ�ͣ�����DbThread����

		���̺߳���Ӧ�Ĵ��߳�ʹ��һ��TrMsgBuffer
		ͬʱ���߳�ʹ��TrMsgBufferSwapper�����й�����Щ��������

		����ʹ��TrMsgBufferSwapper���������߳��������Ӧ����
		ʹ��TrMsgBuffer��ÿ���߳����������Ӧ�Ĵ���

		TrMsgBufferSwapper��Ҫ����
		��OnMainFrameUpdate��ÿ�����߳�ѭ����ʱ���Ҫ�����left msg�ŵ���Ӧ�Ĵ��̵߳�swap msg buffer
		��OnCanceled������Ӧ���߳�Ͷ�ݹ�������Ϣ��������Ӧ�Ĵ�����������right msg���У���ȡ���̹߳黹����

		TrMsgBuffer��Ҫ����
		��OnFrameUpdate�����߳�ÿ��ѭ����ʱ���Ҫ�����right msg�ŵ���Ӧ��swap msg buffer
		��OnCanceled������Ӧ���߳�Ͷ�ݹ�������Ϣ��������Ӧ�Ĵ���

		���̺߳ʹ��߳̽�������main��threadΪ��

		�����̷߳���
		1����main�������Waited Left Msg Buffer
		2����main��update��ʱ�򣬼��left msg��ball�Ƿ�ӵ��
		3�������ball���򽻻�Waited Left Msg Buffer��Swap Left Msg Buffer��
		   ͬʱ�������ó�NotOwned����thread����LeftMsgBufferSwaped��Ϣ
		4��thread��Canceled���ѣ�����LeftMsgBufferSwaped��Ϣ��
		   ����SwapLeftMsgBuffer��DoingLeftMsgBuffer������left msg��Ȼ���main����ReturnFromMinor��Ϣ
	    5��main��Canceled���ѣ�����ReturnFromMinor��Ϣ���������ó�Owned
		6���ڵ�2���У������ʱû��ball����ô�Ͳ�����ֱ��ballΪOwned״̬

		���߳�Ҳ��һ���Ĵ���ʽ

		���������ʹ��
		���ⲿʹ�õ�ʱ����������Ҫдһ��swapper��̳�CTrMsgBufferSwapper
		Ȼ���ڸ�����������ʵ�����̻߳��Ѻ���
		Ȼ��дһ����̳�TTrThreadMsgBuffer������ʵ��ʵ�ʵ���������
		���ڸ�����������ʵ�ִ��̺߳���
		TTrThreadMsgBuffer�����AllocLeftMsgMem��AddCommLeftMsg��AllocRightLeftMem��AddCommRightMsg
		������ʵ�����������������left��right msg�Լ�������Ӧ���ڴ�
	*/

	class CTrThreadMsgBuffer;

	class CTrMsgBufferSwapper
	{
		friend class CTrThreadMsgBuffer;

	public:
		//���߳�ÿ��ѭ������
		void FlushAllLeftMsg();

		//���̱߳����ѵ���
		void HandleAllRightMsg();
		
		//�����̵߳��û������߳�ʹ��
		virtual void OnWakeLeft() = 0;

		void NotifyRightToQuit();

	protected:
		CTrMsgBufferSwapper() ;
		virtual ~CTrMsgBufferSwapper();

		void AddTrBuffer(CTrThreadMsgBuffer* pTrBuffer);
		void RemoveTrBuffer(CTrThreadMsgBuffer* pTrBuffer);

		bool IsInSameThread();

		void AddControlMsg(uint32 uIndex, uint32 uMsg);

	protected:

		vector<CTrThreadMsgBuffer*>	m_vecTrBuffer;
		
		HTHREADID					m_hThread;

		HSPINLOCK					m_Lock;

		/*
			��Ϊmgr���渺�����left msg���У������������߳���Ϣ����Ͷ�ݵ�����
			��������ͳһʹ��һ����Ϣ���н��й���ÿһ����Ϣ������������
			һ����Ͷ���̵߳�id����һ������Ͷ�ݵ���Ϣ����
		*/
		typedef pair<uint32, uint32>	IndexMsgPair_t;
		typedef vector<IndexMsgPair_t, CTrMsgMallocAllocator<IndexMsgPair_t> >	VecMsg_t;

		VecMsg_t*	m_pWaitingRightControlMsg;
		VecMsg_t*	m_pDoingRightControlMsg;
	};

}
