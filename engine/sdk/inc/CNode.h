
#ifndef	_Node_H
#define	_Node_H
//---------------------------------------------------------------------------
#ifdef _WIN32

#pragma warning(disable: 4275)
#pragma warning(disable: 4251)

#endif

namespace sqr
{
namespace BaseDataStruct
{
class CNode
{
protected:
	CNode* m_pNext;
	CNode* m_pPrev;

public:
	CNode(void);
	virtual ~CNode() {};
	CNode* GetNext(void);
	CNode* GetPrev(void);
	void InsertBefore(CNode* pNode);
	void InsertAfter(CNode* pNode);
	void Clear();
	void Remove(void);
	bool IsLinked(void);

	friend class CList;
};
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ����:	CNode
// ����:	����
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
inline CNode::CNode(void)
{
	m_pNext = NULL;
	m_pPrev = NULL;
}
//---------------------------------------------------------------------------
// ����:	GetNext
// ����:	��һ�����
// ����:	void
// ����:	CNode*
//---------------------------------------------------------------------------
inline CNode* CNode::GetNext(void)
{
	//if(m_pNext == NULL)
	//	return NULL;
	if (m_pNext->m_pNext)
		return m_pNext;
	return NULL;
}
//---------------------------------------------------------------------------
// ����:	GetPrev
// ����:	ǰһ�����
// ����:	void
// ����:	CNode*
//---------------------------------------------------------------------------
inline CNode* CNode::GetPrev(void)
{
	if (m_pPrev->m_pPrev)
		return m_pPrev;
	return NULL;
}
//----------------------------------------------------------------------------
// ����:	InsertBefore
// ����:	��ǰ�����һ�����
// ����:	CNode*
// ����:	void
//---------------------------------------------------------------------------
inline void CNode::InsertBefore(CNode *pNode)
{
	//KASSERT(m_pPrev);
	if (!pNode || !m_pPrev)
		return;

	pNode->m_pPrev = m_pPrev;
	pNode->m_pNext = this;
	m_pPrev->m_pNext = pNode;
	m_pPrev = pNode;
}
//---------------------------------------------------------------------------
// ����:	InsertAfter
// ����:	�ں������һ�����
// ����:	CNode*
// ����:	void
//---------------------------------------------------------------------------
inline void CNode::InsertAfter(CNode *pNode)
{
	//KASSERT(m_pNext);
	if (!pNode || !m_pNext)
		return;
	pNode->m_pPrev = this;
	pNode->m_pNext = m_pNext;
	m_pNext->m_pPrev = pNode;
	m_pNext = pNode;
}
// ��սڵ��ָ��
inline void CNode::Clear()
{
	m_pPrev = NULL;
	m_pNext = NULL;
}

//---------------------------------------------------------------------------
// ����:	Remove the node
// ����:	��������ɾ��������
// ����:	void
// ����:	CNode*
//---------------------------------------------------------------------------
inline void CNode::Remove(void)
{
	//KASSERT(m_pPrev);
	//KASSERT(m_pNext);
	if (m_pPrev)
		m_pPrev->m_pNext = m_pNext;
	if (m_pNext)
		m_pNext->m_pPrev = m_pPrev;
	m_pPrev = NULL;
	m_pNext = NULL;
}
//---------------------------------------------------------------------------
// ����:	IsLinked
// ����:	����ڵ��Ƿ�����
// ����:	void
// ����:	bool
//---------------------------------------------------------------------------
inline bool CNode::IsLinked(void)
{
	return (m_pPrev && m_pNext);
}
//--------------------------------------------------------------------------------
}
}

::BaseDataStruct;

#endif
