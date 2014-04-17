#pragma once

#include "CPos.h"
#include "CFindPathMallocObject.h"

NAMESPACE_BEGIN(sqr)

#pragma pack(push,1)

class CNode
	:public CFindPathMallocObject
{
public:

	//���ú���
	inline float GetG();
	inline void SetG(float fG);
	inline bool IsOpen(uint32 nSearchId);
	inline void Open(uint32 nSearchId);
	inline bool IsClose(uint32 nSearchId);
	inline void Close(uint32 nSearchId);

	inline void SetTargetId(uint16 nTargetId);
	inline uint16 GetTargetId();
	//A*ʹ�õĺ���
	inline void SetFrontDir(uint8 nFatherDir);
	inline uint8 GetFrontDir();

	//��ͨ����Ѱ·ʹ�õĺ���
	inline void SetFatherRegion(uint32 nSearchId);
	inline bool IsFatherRegion(uint32 nSearchId);
	inline void SetFrontIndex(uint8 nIndex);
	inline uint8 GetFrontIndex();
private:

	float   m_fG;        // �ƶ���ֵ
	uint8	m_nState;	//  	A* Ѱ·:	��λ�� ��һ��bit Ϊ 1 ��ʾopen, 0 ��ʾclose,  �ϵ͵�4��bit����Ѱ·����һ���ڵ� ����, (�м�3λ����)
						// ��ͨ����Ѱ·:  ��λ�� ��һ��bit Ϊ 1 ��ʾopen, 0 ��ʾclose, �ڶ���bit ��ʾ�Ƿ�Ϊ����ͨ����, �ϵ͵�6��bit����Ѱ·����һ���ڵ���linklist������
	
	uint16	m_nTargetId;
	uint32  m_nSearchId;	//��ǰ�ڵ��Ӧ�Ĳ�ѯid, �����ж��Ƿ񱾴�Ѱ·̽�����Ľڵ�
};

#pragma pack(pop)

NAMESPACE_END


