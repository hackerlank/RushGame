
#include "stdafx.h"
#include "DrawLine.h"
#include "BarrierManager.h"
#include "MapInfoManager.inl"


DrawLine::DrawLine(CBarrierManager* pBarrierMgr, CMapInfoManager* pMapInfo)
	: m_nX(-1)
	, m_nY(-1)
	, m_BarrierMgr(pBarrierMgr)
	, m_MapInfo(pMapInfo)
{
}

DrawLine::~DrawLine()
{
}

bool DrawLine::Do( int32 x, int32 y )
{
	bool ret = m_BarrierMgr->JumpBarrier(m_MapInfo->GetIndex(x, y));
	// ��ǰGridΪ�ɺ����ϰ��ȼ�(�����ܹ������ϰ��ȼ��ɸô�Ѱ·������EBarrier����)
	if(ret == true)
	{
		m_nX = x; // ����ǰGrid��¼Ϊ��ͨ�е�(HypoLineѰ·�õ���LastX, LastY)
		m_nY = y;
	}
	return ret;
}

int32 DrawLine::GetLastX()
{
	return m_nX;
}

int32 DrawLine::GetLastY()
{
	return m_nY;
}
