#pragma once

#include "CVector2.h"
#include "CVector3.h"

namespace sqr_tools
{
	//�ر�߶Ȳ����㷨��
	class CEditLogicDataMgr;
	class CTerrainOperate
	{
	public:
		CTerrainOperate(void);
		~CTerrainOperate(void);

		//����
		virtual void SetLogicDataMgr(CEditLogicDataMgr* logicDataMgr);
		//����
		virtual CVector3f CalculateVertexNormal(uint32 uX, uint32 uY,int widthGrid,int heightGridt);
		virtual float SmoothVertexHeight(uint32 uX, uint32 uY,int widthGrid,int heightGrid,float fStrength); //���� ��ͼ���(����) ǿ��
		virtual float AlignVertexHeight(uint32 uX, uint32 uY, float fMinY);
		virtual float CulGaussVertexHeight(uint32 uX, uint32 uY, uint32 uHotX, uint32 uHotY, const DWORD dwBrushWidth, const DWORD dwBrushDepth, const float fStrength,bool isInc=true);
		virtual float CulLineVertexHeightX(uint32 uX, uint32 uY, uint32 uHotX, uint32 uHotY, const DWORD dwBrushWidth, const DWORD dwBrushDepth, const float fStrength,bool isInc=true);
		virtual float CulLineVertexHeightZ(uint32 uX, uint32 uY, uint32 uHotX, uint32 uHotY, const DWORD dwBrushWidth, const DWORD dwBrushDepth, const float fStrength,bool isInc=true);
		virtual float CulLineVertexHeightB(uint32 uX, uint32 uY, uint32 uHotX, uint32 uHotY, const DWORD dwBrushWidth, const DWORD dwBrushDepth, const float fStrength,bool isInc=true);
		//
		virtual bool IsNeighborPoint(int p1,int p2,int w);

	protected:
		CEditLogicDataMgr* m_LogicDataMgr;
	};

	inline void CTerrainOperate::SetLogicDataMgr(CEditLogicDataMgr* logicDataMgr)
	{	m_LogicDataMgr=logicDataMgr;	}

//!namespace
}
