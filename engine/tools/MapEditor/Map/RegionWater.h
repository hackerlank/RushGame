#pragma once
#include "../IO/VersionMapChunk.h"

namespace bly
{
class CTerrainMesh;
}

namespace Map
{

/// ������Ϣ.
class RegionWater : public IO::DataChunk
{
	DeclareChunk(RegionWater);
public:
	RegionWater();
	virtual ~RegionWater();

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
		REGION_WATER_VERTEX_NUM = REGION_GRID_NUM * GRID_VERTEX_NUM
	};

private:
	struct WaterVertex
	{
		short height;
		uint color;
	};
	struct WaterGrid
	{
		byte index;	///< local grid index
		WaterVertex vertex[GRID_VERTEX_NUM];
	};
private:
	std::vector<WaterGrid> water;
};

}// namespace Map
