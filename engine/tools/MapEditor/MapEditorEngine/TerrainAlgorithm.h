#pragma once
#include "TSingleton.h"
#include "CVector3.h"
#include "ExsertStructDB.h"

namespace sqr
{
	class CTerrainMesh;
	class SVertex;

	struct  ClientGridRgn
	{
		DWORD gAmbientColor;
		DWORD gPlayerAmbientColor;
		DWORD gDirLightDiffuse;
		DWORD gPlayerDiffuse;
		DWORD gFogColor;
	};

	class CTerrainAlgorithm : public TSingleton<CTerrainAlgorithm>
	{
	public:

		CTerrainAlgorithm();
		virtual ~CTerrainAlgorithm();

		// �������ж��㷨�߷���
		void	CalTerrainNormal();
		// �������ÿ�����㷨��
		void	CalculateVertexNormal(DWORD dwVertexIndex);
		//����grid����yֵ
		void ReCalculateGridsHeight(DWORD dwVertexIndex);
		///�ж϶���߶��Ƿ���Ч
		void AssertHeight(SVertex* pVertex);

		///����ɫ����
		///do
		void ModulateVertexDiffuseRGB(int nVertexIndex, DWORD dwBrushWidth, DWORD dwBrushDepth, BYTE byRed, BYTE byGreen, BYTE byBlue, int nVertexIndexHot);
		void ModulateVertexSpecularRGB(int nVertexIndex, DWORD dwBrushWidth, DWORD dwBrushDepth, BYTE byRed, BYTE byGreen, BYTE byBlue, int nVertexIndexHot);
		///undo
		void SetVertexDiffuseRGB(int nVertexIndex, DWORD dwColor);
		void SetVertexSpecularRGB(int nVertexIndex, DWORD dwColor);

		///alpha����
		void CutVertexAlpha(DWORD dwVertexIndex);

		void IncVertexAlphaX(sqr::EALPHA_TYPE eAlphaType, DWORD dwVertexIndex, DWORD dwBrushSize, int nVertexIndexHot);
		void IncVertexAlphaZ(sqr::EALPHA_TYPE eAlphaType, DWORD dwVertexIndex, DWORD dwBrushSize, int nVertexIndexHot);
		void IncVertexAlphaB(sqr::EALPHA_TYPE eAlphaType, DWORD dwVertexIndex, DWORD dwBrushWidth, DWORD dwBrushDepth, int nVertexIndexHot);

		void DecVertexAlphaX(sqr::EALPHA_TYPE eAlphaType, DWORD dwVertexIndex, DWORD dwBrushSize, int nVertexIndexHot);
		void DecVertexAlphaZ(sqr::EALPHA_TYPE eAlphaType, DWORD dwVertexIndex, DWORD dwBrushSize, int nVertexIndexHot);
		void DecVertexAlphaB(sqr::EALPHA_TYPE eAlphaType, DWORD dwVertexIndex, DWORD dwBrushWidth, DWORD dwBrushDepth, int nVertexIndexHot);

		void SetVertexAlpha(DWORD dwVertexIndex, D3DCOLOR cColor);

		///height manipulation
		void CutVertexHeight(DWORD dwVertexIndex, float fMinY);
		void SmoothVertexHeight(int nVertexIndex);

		void IncLinearVertexHeightX(DWORD dwVertexIndex, DWORD dwBrushSize, int nVertexIndexHot);
		void IncLinearVertexHeightZ(DWORD dwVertexIndex, DWORD dwBrushSize, int nVertexIndexHot);
		void IncLinearVertexHeightB(DWORD dwVertexIndex, DWORD dwBrushWidth, DWORD dwBrushDepth, int nVertexIndexHot);
		void IncGaussVertexHeight(DWORD dwVertexIndex, DWORD dwBrushWidth, DWORD dwBrushDepth, int nVertexIndexHot);

		void DecLinearVertexHeightX(DWORD dwVertexIndex, DWORD dwBrushSize, int nVertexIndexHot);
		void DecLinearVertexHeightZ(DWORD dwVertexIndex, DWORD dwBrushSize, int nVertexIndexHot);
		void DecLinearVertexHeightB(DWORD dwVertexIndex, DWORD dwBrushWidth, DWORD dwBrushDepth, int nVertexIndexHot);

		void DecGaussVertexHeight(DWORD dwVertexIndex, DWORD dwBrushWidth, DWORD dwBrushDepth,  int nVertexIndexHot);

		void TemplateIncCutVertexHeightByGauss(DWORD dwVertexIndex, DWORD dwBrushWidth, DWORD dwBrushDepth, int nVertexIndexHot);
		void TemplateIncCutVertexHeightByLinear(DWORD dwVertexIndex, DWORD dwBrushWidth, DWORD dwBrushDepth, int nVertexIndexHot);

		// undo
		void SetVertexHeight(DWORD dwVertexIndex, float fHeight);

		///�赲 block manipulation
		// do and undo
		void SetTileBlock(DWORD dwGridIndex, int nBlockType);

		//////////////////////////////////////////////////////////////////////////
		///���¼���
		CVector2f GetProjectCoord(CVector3f& pos, CVector3f& normal);
		void ReCalculateGridsLayerUV(const int nGridIndex, const int nLayer);
		void ReBackGridLayerUV(const int nGridIndex, const int nLayer);
		//////////////////////////////////////////////////////////////////////////

		///������С��ͼͼƬ��صļ���
		float GetApex();

		//��ʱӳ���Լ�С��ͼ����
		void SetMappedRegion( set<int>& regionunits );

		void GetMapCoordinate(const int nGridIndex, int& nX, int& nZ);
	

	};
}
