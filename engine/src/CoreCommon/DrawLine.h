#pragma once


#include "MathAlgorithm.h"
#include "CFindPathMallocObject.h"

namespace sqr
{
class CBarrierManager;
class CMapInfoManager;

class DrawLine
	:public ILineAction
	,public CFindPathMallocObject
{
public:
	DrawLine(CBarrierManager* pBarrierMgr, CMapInfoManager* pMapInfo);
	~DrawLine();
	bool Do( int32 x, int32 y );
	int32 GetLastX();
	int32 GetLastY();

private:
	int32 m_nX;
	int32 m_nY;  // ����ֱ�߲�ѯ������������꣬��������������ϰ���ʱ���ܹ�����ǰһ����Ч�����
	CBarrierManager*   m_BarrierMgr;
	CMapInfoManager*   m_MapInfo;
};

}

