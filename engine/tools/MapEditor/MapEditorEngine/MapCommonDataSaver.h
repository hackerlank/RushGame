#pragma once
#include "TSingleton.h"
#include "CVector3.h"
namespace sqr
{
	class CTerrainMesh;
	class COperator;
	class SGrid;

	class CMapCommonDataSaver : public TSingleton<CMapCommonDataSaver>
	{
		COperator    *pOperator;
		CTerrainMesh *terrain;
	public:
		friend class TSingleton<CMapCommonDataSaver>;
		CMapCommonDataSaver();
		virtual ~CMapCommonDataSaver();

		void SetOperator(COperator *pOper);
		void SetTerrainMesh(CTerrainMesh *pTerrain);

		// ����env ������Ϣ
		void SaveEnvInfo(FILE * fp);

		// ���汳��ͼ��Ϣ
		void SaveBackPictureInfo(FILE * fp);

		// ����wave��Ϣ
		void SaveWaveInfo(FILE * fp);

		// ����ر�����Ȩ����Ϣ�������赲���ȼ�
		void SaveMainRoadWeightInfo(FILE * fp);

		/// ����ģ�������Լ��������԰汾��
		void SaveGridModeProVersion(FILE * fp);

		// ����ÿ��������Ϣ
		void SaveEachVertexInfo(FILE * fp, const float fPositionY, const CVector3f& vNormal, const DWORD oColor, const DWORD sColor, const short sLogicHeight);

		///��¼������������
		void SaveTerrainGridTexIndex(FILE * fp, const int nGridCnt, map<string, int16>& terrainTextureNames);

		// ����ÿ��������Ϣ
		void SaveEachGridInfo(FILE * fp, SGrid* pGrid, int& SceneLightCount, int& nSpotLightCount, bool bResize, bool bSplit);

	public:
		set<int> nLoginGridIndexs;
		DWORD    dwNewMapWidth;
	};
}
