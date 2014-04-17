#pragma once

#include "TBinaryHeap.h"
#include "CNode.h"
#include "CFindPathMallocObject.h"
#include "CFindPathAllocator.h"

namespace sqr
{

class CAStarData
	:public CFindPathMallocObject
{
public:
	typedef CFindPathAllocator<float>	BHKeyAlloc;
	typedef CFindPathAllocator<CNode*>	BHValueAlloc;

	typedef TBinaryHeap<float, CNode*, less<float>, BHKeyAlloc, BHValueAlloc >	MinHeap;

	CAStarData(uint32 nWidth, uint32 nHeight, uint32 nAllNodeNum);
	~CAStarData(void);

	void UpdateSize(uint32 nWidth, uint32 nHeight, uint32 nAllNodeNum);
	void ReSet();

	inline CNode* ReturnBestNode();

	//����A*Ѱ·�ĺ���
	inline CNode* GetNodeAddress(int x, int y);
	inline int GetNodeOffset(int iDir);
	inline uint32 GetIndex(int x, int y);
	inline void GetPos(int32& x, int32& y, CNode* pNode);
	inline CNode* GetFront(CNode* pNode);

	//������ͨ����Ѱ·�ĺ���
	inline CNode* GetNode(uint32 nIndex);
	inline uint32 GetIndex(CNode* pNode);

	int*   GetOffsetArray();
	uint32 GetSearchId();
	MinHeap& GetHeapF();
private:
	CNode* m_pNodeArray;
	uint32	m_nSearchId;
	uint32 m_nWidth;
	uint32 m_nHeight;
	uint32 m_nAllNodeNum;

	static int ms_nOffset[16];
	int m_nNodeOffset[8];		//Ѱ·8�������Ӧ�ĸ��� �� m_pNodeArray �е�ƫ��, ��ms_nOffset ˳���Ӧ, ʡȥ�˷�����
	MinHeap	m_heapF;
};

}

