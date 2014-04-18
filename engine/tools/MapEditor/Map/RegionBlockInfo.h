#pragma once
#include "../IO/VersionMapChunk.h"

namespace bly
{
class CTerrainMesh;
}

namespace Map
{

/// �赲��Ϣ.
class RegionBlockInfo : public IO::DataChunk
{
	DeclareChunk(RegionBlockInfo);
public:
	RegionBlockInfo();
	virtual ~RegionBlockInfo();

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
		REGION_WIDTH_IN_GRID = 16,
		REGION_GRID_NUM = REGION_WIDTH_IN_GRID*REGION_WIDTH_IN_GRID
	};
private:
	byte block[REGION_GRID_NUM];
};

}// namespace Map
