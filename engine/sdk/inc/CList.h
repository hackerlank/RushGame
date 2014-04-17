
#ifndef	_CList_H
#define	_CList_H

//---------------------------------------------------------------------------
#include "CNode.h"
//---------------------------------------------------------------------------

namespace sqr
{
namespace BaseDataStruct
{
class CList
{
protected:
	CNode m_ListHead; // ͷ�ڵ㣨���ǵ�һ���ڵ㣩
	CNode m_ListTail; // β�ڵ㣨�������Ľڵ㣩
public:
	CList(void);
	CNode* GetHead(void); // ȡ�õ�һ���ڵ�
	CNode* GetTail(void); // ȡ�����һ���ڵ�
	void AddHead(CNode *pNode); // ����ǰ������һ���ڵ�
	void AddTail(CNode *pNode); // �����������һ���ڵ�
	CNode* RemoveHead(void); // ɾ����һ���ڵ�
	CNode* RemoveTail(void); // ɾ�����һ���ڵ�
	bool IsEmpty(void); // �Ƿ��Ǹ��յ�����
	long GetNodeCount(void);
};
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ����:	CList
// ����:	����
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
inline CList::CList(void)
{
	m_ListHead.m_pNext = &m_ListTail;
	m_ListTail.m_pPrev = &m_ListHead;
}
//-------------------------------------------------------------------------
// ����:	IsEmpty
// ����:	�Ƿ�Ϊ��
// ����:	void
// ����:	bool
//---------------------------------------------------------------------------
inline bool CList::IsEmpty(void)
{
	return (m_ListHead.GetNext() == NULL);
}
//-------------------------------------------------------------------------
// ����:	GetHead
// ����:	ȡ�������ͷ
// ����:	void
// ����:	CNode*
//---------------------------------------------------------------------------
inline CNode* CList::GetHead(void)
{
	return m_ListHead.GetNext();
}
//-------------------------------------------------------------------------
// ����:	GetTail
// ����:	ȡ�������β
// ����:	void
// ����:	CNode*
//---------------------------------------------------------------------------
inline CNode* CList::GetTail(void)
{
	return m_ListTail.GetPrev();
}
//-------------------------------------------------------------------------
// ����:	AddHead
// ����:	��ͷ������һ���ڵ�
// ����:	CNode*
// ����:	bool
//---------------------------------------------------------------------------
inline void CList::AddHead(CNode *pNode)
{
	m_ListHead.InsertAfter(pNode);
}
//-------------------------------------------------------------------------
// ����:	AddTail
// ����:	��ĩβ����һ���ڵ�
// ����:	CNode*
// ����:	void
//---------------------------------------------------------------------------
inline void CList::AddTail(CNode *pNode)
{
	m_ListTail.InsertBefore(pNode);
}
//-------------------------------------------------------------------------
// ����:	RemoveHead
// ����:	ɾ����һ���ڵ�
// ����:	void
// ����:	CNode*
//---------------------------------------------------------------------------
inline CNode* CList::RemoveHead(void)
{
	CNode* pNode = m_ListHead.GetNext();
	if (pNode)
		pNode->Remove();
	return pNode;
}
//-------------------------------------------------------------------------
// ����:	RemoveTail
// ����:	ɾ�����һ���ڵ�
// ����:	void
// ����:	CNode*
//---------------------------------------------------------------------------
inline CNode* CList::RemoveTail(void)
{
	CNode* pNode = m_ListTail.GetPrev();
	if (pNode)
		pNode->Remove();
	return pNode;
}
//-------------------------------------------------------------------------
// ����:	GetNodeCount
// ����:	ȡ�ýڵ����
// ����:	void
// ����:	long
//---------------------------------------------------------------------------
inline long CList::GetNodeCount(void)
{
	long nNode = 0;
	CNode* pNode = GetHead();
	while (pNode)
	{
		pNode = pNode->GetNext();
		nNode++;
	}
	return nNode;
}
//-------------------------------------------------------------------------
}
}

::BaseDataStruct;
#endif
