#pragma once
#include "CPos.h"
#include "CTimeCheckPoint.h"


namespace sqr
{
	class CDirObjActiveState
	{
	public:
		CDirObjActiveState(void);
		~CDirObjActiveState(void);

		uint32	m_uTellStepCounter;		//������������ʵ��ÿ�̶�����step֮���ٷ���һ��step֪ͨ������ˡ�

		float	m_fLastSentMovedDist;		//���һ�������ٲö�������ƶ�����
		float	m_fUnsentMovedDist;			//Ŀǰδ���͸��ٲö�������ƶ�����

		//�ƶ���ʼ��ʱ��㣬���ڼ���timestamp
		CTimeCheckPoint		m_tcpBegin;
		CTimeCheckPoint		m_tcpLastTellStep;
	};
}