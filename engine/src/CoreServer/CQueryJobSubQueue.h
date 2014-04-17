#pragma once
#include "ThreadTypes.h"


namespace sqr
{

	class CQueryJob;

	class CQueryJobSubQueue
	{
	public:
		CQueryJobSubQueue(void);
		~CQueryJobSubQueue(void);

		void Push( CQueryJob* pJob );
		CQueryJob* Pop();

	private:
		CQueryJob*				m_pIn;
		CQueryJob*				m_pOut;

		//��Ȼ�������������Ǽ���100%�����ײ�����Ի��ǳ�֮Ϊ��������
		HSPINLOCK				m_hLock;
	};

}

