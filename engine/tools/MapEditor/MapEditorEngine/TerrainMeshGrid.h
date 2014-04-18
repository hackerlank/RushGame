#pragma once
#include "TRefPointer.h"
#include "OffLineBaker/CVertexColor.h"
#include "ExsertStructDB.h"

namespace sqr
{
	SmartPointer(CTObject);

	class CRiver;
	class CRiverWave;

	class SGrid
	{
	public:
		SGrid();
		~SGrid(void);

	public:
		DWORD		dwGridIndex;
		DWORD		dwVertexIndex[4];

		CVector2f	uv[2][4];
		CVector2f	uvOld[2][4];
		STextureNodeInfo nodeInfo;

		CVector3f	vCenter;
		vector< CTObjectPtr>			vecCenterObjects; // is this the center gird(in Model way)?
		int			nBlockType;
		int			nGridSeekRoadWeight;
		bool		bVisible;
		bool		bSide;
		bool		bReverse;
		CRiver	   *pRiver;
		CRiverWave *pWave;
		bool		bInHideRegion;
		bool		bEffectByVertexColor;	// �����Ƿ��ܶ���ɫӰ��
		bool		bCliffLayerOne;			// ��һ���Ƿ���Ҫ������ͼ
		bool		bGridHide;
		DWORD		dwBufferIndex;			// buffer����
		
		BYTE		subGridTypes[4];

		/*			---------
					|2  |3	|
					---------
					|0	|1  |
					---------
		*/
		map<int, int> subGridIndex;			// first subgridIndex, second 0---3
		int			m_nDivideRegionIndex;	// ���������ĸ�ϸ������
		int			m_nRegionIndex;			// ���������ĸ�����
		bool		m_bSetExactBlock;

		map< ModelGroupIndex, ModelGroupColorMap > m_ObjectColorMap;
		map< ModelGroupIndex, ModelGroupColorMap > m_DepathObjColorMap;

		///�ر��������
		int8		nMaterialIndex;
		bool		bMatNotEffectByTileSets;///ǿ�����øø��Ӳ���ͼ�ذ�����Ӱ�죬Ĭ��Ϊ��Ӱ��
			
		int8		bSubRenderUnitIndex;///�ڴ������е��ĸ����֣���ʱ�Ƿ�4������

	public:
		ModelGroupColorMap* GetModelColorMap(ModelGroupIndex modelGroupIndex);
		void ClearModelColorMap();
		void ClearGridInfo();
	};
}
