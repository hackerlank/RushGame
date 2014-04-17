#pragma once
#include "CVector2.h"
#include "CVector3.h"

namespace sqr_tools
{
	class CBrushGridHepler
	{
		int32				m_uWidthInGrid;			// �����Ŀ�ȣ��Ը���Ϊ��λ
		int32				m_uHeightInGrid;		// �����ĸ߶ȣ��Ը���Ϊ��λ;
	public:
	
		CBrushGridHepler(void);
		virtual ~CBrushGridHepler(void);

		void SetSceneSize(const int32 width, const int32 height)
		{
			m_uWidthInGrid  = width;
			m_uHeightInGrid = height;
		}

		void GetBrushGrids(const CVector3f center, const int nBrushWidth, const int nBrushHeight, vector<CVector2I>& grids, vector<CVector3f>& vertexs);
		void GetBrushGridIndexsByGrid(const CVector3f center, const int nBrushWidth, const int nBrushHeight, const int unit, vector<CVector2I>& grids, vector<CVector3f>& vertexs);
	};

}
