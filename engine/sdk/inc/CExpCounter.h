#pragma once 
#include "ModuleDefs.h"

namespace sqr
{
#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable:4275)	//dll linkage
#pragma warning(disable:4251)	//dll linkage
#endif

	/*
	�쳣������
	�ڵ�λʱ�����쳣��������һ����Χ�Ϳ�ʼ������ش���
	*/

	class MODULE_API CExpCounter
	{
	public:
		CExpCounter(uint64 uInterval, uint32 uNum = 5);
		~CExpCounter();

		//�������true����֤����Ҫ��ʼ�����쳣������		
		bool AddExpCounter(uint64 uTime);

	private:
		std::list<uint64>	m_lstOccurTime;  //�����쳣���ֵ�ʱ��
		uint64				m_uInterval;	 //��λʱ��
		uint32				m_uMaxExpNum;    //�ڵ�λʱ����������������쳣����  
	};

#ifdef _WIN32
#pragma warning(pop)
#endif
}

