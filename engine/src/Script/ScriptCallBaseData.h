#pragma once
#include "CScriptAllocator.h"

namespace sqr
{

	//�������������Ϣ�Ľṹ
	struct CallBaseData : public CScriptMallocObject
	{
		CallBaseData() : m_pResult(NULL), m_uBufSize(0), m_nReturnNum(0), m_bHasVariant(false), m_bHasArgList(false), m_bRetArgList(false){}

		typedef vector<CBaseType*, CScriptAllocator<CBaseType*> >	VecType_t;

		VecType_t			m_vecParam;
		CBaseType*			m_pResult;
		size_t				m_uBufSize;
		int32				m_nReturnNum;
		bool                m_bHasVariant;     //�����б����Ƿ����CVariant����
		bool                m_bHasArgList;     //�����б����Ƿ���ڿɱ����
		bool                m_bRetArgList;     //����ֵ�Ƿ�Ϊ�ɱ���� ��������callback
	};

}

