#pragma once
#include "CTerrainMgr.h"

namespace sqr_tools
{
	class CEditLogicDataMgr;
	class CEditTerrainMgr : public CTerrainMgr
	{
	public:
		CEditTerrainMgr(CRenderGroup* pGroup,CEditLogicDataMgr* pLogicDataMgr);
		~CEditTerrainMgr(void);

		bool			SetRenderFlag(TerrainFlag::Mask mask);
		bool			isVisable(URegionID ID);

		void			SetLightMap(const URegionID ID, const char* filename);
		void			SetLightMap(const URegionID ID, ITexture* pText);

		virtual void	Render();
		
		bool			GenerateTerrain(int tileWidth,int tileHeight);
		void			WriteMapBuffer(const set<URegionID> regions);
	
	protected:
		CElementNode*	_CreateNode( URegionID ID );
		CEditLogicDataMgr* m_pLogicDataMgr;
		//���һ��Tile�Ķ�������(��׼ Ŀǰֻ��λ����Ϣ)
		static CVector3f	OffPos[64*64];
	public:
		static const string st_DefaultMapName;
	};
}