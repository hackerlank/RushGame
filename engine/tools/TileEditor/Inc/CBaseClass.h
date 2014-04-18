#ifndef _CBASECLASS_H_
#define _CBASECLASS_H_

#include "IBaseClass.h"
struct ObjectCreateStruct;

class CBaseClass : IBaseClass
{
public:
	CBaseClass(void);
	~CBaseClass(void);

	// ���󴴽���׼������Ҫ��Ϸ����EngineһЩ��Ϣ�����ObjectCreateStruct
	void OnPrecreate(ObjectCreateStruct& ocs, float fCreateFlag);
	// �Ƿ�ѡ�е�ͨ��
	void OnSelected(bool bSelected);
	// ����������Ϣ
//	void OnSend(CMessage*	pMsg);
	// ����������Ϣ
//	void OnReceive(CMessage*	pMsg);

	void SetClassIdx(uint32 nClassIdx)
	{
		m_nClassIdx = nClassIdx;
	}

	void SetObjectIdx(uint32 nObjectIdx)
	{
		m_nObjectIdx = nObjectIdx;
	}

	uint32 GetObjectIdx()
	{ 
		return m_nObjectIdx;
	}
	
	uint32 GetClassIdx()
	{ 
		return m_nClassIdx;
	}

protected:
	// �������������һ���࣬���Դ���������еõ�����������
	uint32	m_nClassIdx;

	// ����������Cache�е�����
	uint32	m_nObjectIdx;

};

#endif