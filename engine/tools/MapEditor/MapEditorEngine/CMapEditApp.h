#pragma once
#include "TSingleton.h"
#include "ExsertStructDB.h"

namespace sqr_tools
{
	class CEditDataScene;
	class CEditContext;
	class CMapContext;
}

namespace MapEditor
{
	class CMapMgr;
}

using namespace MapEditor;

namespace sqr
{
	class CTerrainMesh;
	class COperator;
	class CWaterOperator;

	class CMapEditApp : public Singleton<CMapEditApp>
	{
	public:
		static bool NEWOLD;

		CMapEditApp();
		virtual ~CMapEditApp();

	private:
		COperator*		m_pOperator;
		CWaterOperator*	m_pWaterOperator;
		CTerrainMesh*	m_pTerrainMesh;
		CTerrainMesh*	m_pTransformTerrainMesh;//���䣬�ָ�ƶ����terrainMesh
		CEditDataScene*	m_pDataScene;
		CEditContext*	m_pRenderScene;
	public:
		void SetTransformTerrainMesh(CTerrainMesh * pTerrainMesh);
		void SetTerrainMesh(CTerrainMesh * pTerrainMesh);
		CTerrainMesh*		GetEditingMesh();
		CTerrainMesh*		GetTransformEditingMesh();
		COperator *			GetOperator();
		CWaterOperator*		GetWaterOperator();
		CTerrainMesh*		GetTerrain();
		CTerrainMesh*		GetWater();
	
		CEditDataScene*		GetDataScene(void);
		CEditContext*		GetRenderScene(void);
		bool				InitNewRenderScene();

		void				SetEditingMesh(sqr::EEDIT_MESH e);
		bool				GetIsEditingWater();

		string				savelog;
		bool				m_bSave;
		EEDIT_MESH			m_eEditMesh;		// ��־�ǵر���ˮ

	public://������	
		CMapMgr*			GetMapMgr();
		CMapMgr*			m_Map;
		void				SetRenderScene(CEditContext* cont);
	};

	inline void CMapEditApp::SetEditingMesh(sqr::EEDIT_MESH e)
	{
		m_eEditMesh = e;
	}

	inline bool CMapEditApp::GetIsEditingWater()
	{
		return m_eEditMesh == sqr::EEM_WATER;
	}
}
