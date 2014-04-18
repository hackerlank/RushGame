#pragma once
#include "../IO/VersionMapChunk.h"

namespace bly
{
class CTerrainMesh;
}

namespace Map
{

/// ������ͼ��UV.
class RegionCliffUV : public IO::DataChunk
{
	DeclareChunk(RegionCliffUV);
public:
	RegionCliffUV();
	virtual ~RegionCliffUV();

	/// д����
	void Write();
	/// �����ж�ȡ
	void Read();

	/// ��������.
	/// @param	CTerrainMesh * terrain
	/// @param	uint index	Region����
	/// @return	void
	void LoadFromTerrain(CTerrainMesh* terrain, uint index);

	enum
	{
		GRID_VERTEX_NUM = 4,
		REGION_WIDTH_IN_GRID = 16,
		REGION_GRID_NUM = REGION_WIDTH_IN_GRID * REGION_WIDTH_IN_GRID,
		REGION_UV_NUM = REGION_GRID_NUM * GRID_VERTEX_NUM
	};

private:
	D3DXVECTOR2 cliffuv[REGION_UV_NUM];
};

}// namespace Map
