
#include "CNode.h"



// ��õ����ƶ�����G
inline float CNode::GetG()
{
	return m_fG;
}

//// ��õ�ǰλ��
//inline const CPos& CNode::GetPos()
//{
//	return m_Pos;
//}
//
//// ��õ�ǰ�ڵ�ĸ��ڵ�
//inline CNode* CNode::GetFarther()
//{
//	return m_pFarther;
//}

// ���õ����ƶ�����
inline void CNode::SetG(float fG)
{
	m_fG = fG;
}
//
//// ���õ�ǰλ��
//inline void CNode::SetPos(CPos Pos)
//{
//	m_Pos = Pos;
//}
//
//// ���õ�ǰ�ڵ�ĸ��ڵ�
//inline void CNode::SetFarther(CNode *pFarther)
//{
//	m_pFarther = pFarther;
//}
//
//inline COpenList::OpenList_t::iterator* CNode::GetIter()
//{
//	return &m_iter;
//}
//
//// record the iterator in tree pool
//inline void CNode::SetIter(COpenList::OpenList_t::iterator& iter)
//{
//	m_iter = iter;
//}

inline bool CNode::IsOpen(uint32 nSearchId)
{
	return m_nSearchId == nSearchId && (m_nState & 0x80) != 0;
}

inline bool CNode::IsClose(uint32 nSearchId)
{
	return m_nSearchId == nSearchId && (m_nState & 0x80) == 0;
}

inline void CNode::Open(uint32 nSearchId)
{
	m_nSearchId = nSearchId;
	m_nState |= 0x80;
}

inline void CNode::Close(uint32 nSearchId)
{
	m_nSearchId = nSearchId;
	m_nState &= 0x7F;
}

inline void CNode::SetTargetId(uint16 nTargetId)
{
	m_nTargetId = nTargetId;
}

inline uint16 CNode::GetTargetId()
{
	return m_nTargetId;
}


inline void CNode::SetFrontDir(uint8 nFatherDir)
{
	m_nState &= 0xF0;
	m_nState |= nFatherDir;
}

inline uint8 CNode::GetFrontDir()
{
	return m_nState & 0xF; 
}


inline void CNode::SetFatherRegion(uint32 nSearchId)
{
	m_nSearchId = nSearchId;
	m_nState |= 0x40;
}

inline bool CNode::IsFatherRegion(uint32 nSearchId)
{
	return m_nSearchId == nSearchId && (m_nState & 0x40) != 0;
}

inline void CNode::SetFrontIndex(uint8 nIndex)
{
	m_nState &= 0xC0;
	m_nState |= nIndex;
}

inline uint8 CNode::GetFrontIndex()
{
	return m_nState & 0x3F; 
}
