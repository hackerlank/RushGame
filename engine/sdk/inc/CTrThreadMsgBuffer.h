#pragma once
#include "ThreadTypes.h"
#include "CTrMsgMallocAllocator.h"

namespace sqr
{
	enum ETrBallState
	{
		eTBS_Owned,				//ӵ����
		eTBS_NotOwned,				//û��ӵ����
		eTBS_None,
	};

	enum ETrBallMsg
	{
		eTBM_BallReturned,   //�򷵻�
		eTBM_BufferSwaped,		 //buffer�Ѿ�������
		eTBM_Quit,				 //�˳�
		eTBM_None,
	};

	class CTrMsgBufferSwapper;


	class CTrThreadMsgBuffer
	{
	public:
		friend class CTrMsgBufferSwapper;

		CTrThreadMsgBuffer(CTrMsgBufferSwapper* pMgr);
		virtual ~CTrThreadMsgBuffer();

		virtual void OnWakeRight() = 0;
		
		virtual bool FlushRightMsg();

		bool HandleAllLeftMsg();
	protected:
		virtual void IntSwapLeftBuffer() = 0;
		virtual void IntSwapRightBuffer() = 0;
		
		virtual bool IntLeftBufferEmpty() = 0;
		virtual bool IntRightBufferEmpty() = 0;

		virtual void IntOnLeftBufferSwaped() = 0;
		virtual void IntOnRightBufferSwaped() = 0;


	protected:
		void SwapRightBuffer();

		void SwapLeftBuffer();
		void FlushLeftMsg();

		void NotifyLeft(ETrBallMsg eMsg);
		void NotifyRight(ETrBallMsg eMsg);

		void SetIndex(size_t stIndex);
		size_t GetIndex() const;

		bool LeftBallOwnedByLeft();
		bool RightBallOwnedByRight();

		void AddControlMsg(ETrBallMsg eMsg);

		void HandleLeftControlMsg(ETrBallMsg eMsg);
		void HandleRightControlMsg(ETrBallMsg eMsg);
		
	protected:
		size_t	m_stIndex;
		CTrMsgBufferSwapper*	m_pMgr;
		
		//job��res��ball״̬
		ETrBallState		m_LeftMsgBallState;
		ETrBallState		m_RightMsgBallState;

		HSPINLOCK			m_Lock;

		//��Ϊres��Ϣ��ÿ���̵߳�������ģ�����������һ����Ϣ����ֻ�ô����Լ���msg
		typedef vector<ETrBallMsg, CTrMsgMallocAllocator<ETrBallMsg> >	VecMsg_t;
		VecMsg_t*	m_pWaitingLeftControlMsg;
		VecMsg_t*	m_pDoingLeftControlMsg;

		bool		m_bQuit;
	};

}
