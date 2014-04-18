#pragma once
//------------------------------------------------------------------------------
/**
	@class CMapBaker

	LightMap������.
	����ÿ��4x4����, ÿ�ΰѵ��κ��������Ӱ��Ⱦ��Ⱦ��RTT

	(C) 2009 ThreeOGCMan
*/
#include "CRefObject.h"

#include "BakerMgr.h"

#define OLD_BAKER false

namespace sqr
{
	class CMapEditCamera;
	class CRenderGroup;
	class CTerrainMesh;
	class IProgressCallback;

	SmartPointer(CMapBaker);

	class CMapBaker : public sqr::CRefObject
	{
	public:
		enum
		{
			LIGHT_MAP_SIZE = 512,
			REGION_SIZE_IN_PIXEL = 1024,
			TILE_WIDTH_IN_REGION = 4,
			LIGHT_MAP_SIZE_IN_PIXEL = REGION_SIZE_IN_PIXEL * TILE_WIDTH_IN_REGION,
		};
	public:
		CMapBaker();
		virtual ~CMapBaker();

		/// ���ý�����
		void SetProcessor();
		/// ������Ϣ�ص�
		void SetProgressCallback(IProgressCallback* callback);
		/// ����Zƫ��
		void SetZOffset(float f);

		/// ��ʼ��
		void Setup();
		/// ����
		void Generate();
		/// �����������
		void GenerateByTileRegions(set<int> regions);
		/// �ͷ���Դ
		void Discard();

		//
		void SetBakerMgr(CBakerMgrPtr bakermgr);

	private:
		CTerrainMesh* terrainMesh;
		CMapEditCamera* camera;
		CRenderGroup* renderGroup;

		float zOffset;
		float terrainOffset;
		IProgressCallback* progress;

		CBakerMgrPtr BakerMgr;
	};

	//------------------------------------------------------------------------------
	inline void
	CMapBaker::SetProgressCallback( IProgressCallback* callback )
	{
		this->progress = callback;
	}

	//------------------------------------------------------------------------------
	inline void
	CMapBaker::SetZOffset( float f )
	{
		this->zOffset = f;
	}
}