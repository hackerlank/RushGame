#include "stdafx.h"
#include "CExpCounter.h"

CExpCounter::CExpCounter(uint64 uInterval, uint32 uNum)
: m_uInterval(uInterval)
, m_uMaxExpNum(uNum)
{

}

CExpCounter::~CExpCounter()
{

}


bool CExpCounter::AddExpCounter(uint64 uTime)
{
	if (m_lstOccurTime.empty())
	{
		m_lstOccurTime.push_back(uTime);
		return false;
	}

	uint64 uFirstTime = m_lstOccurTime.front();

	//�����η����쳣��ʱ�������������һ��ʱ����ȴ��ڼ��ʱ�䣬���ǰѵ�һ�ε�ɾ��
	if (uTime - uFirstTime > m_uInterval)
	{
		m_lstOccurTime.pop_front(); 
		m_lstOccurTime.push_back(uTime);
		return false;
	}

	m_lstOccurTime.push_back(uTime);

	//��������������Ŀ��������������Ŀ��˵���ڵ�λʱ���ڲ����Ĵ���������ֵ�ˣ�Ҫ����
	//��ش�������
	if (m_lstOccurTime.size() > m_uMaxExpNum)
	{
		m_lstOccurTime.pop_front();
		return true;
	}

	return false;
}

