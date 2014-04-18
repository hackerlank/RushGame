#pragma once
#include "../IO/VersionMapChunk.h"

namespace bly
{
class CTerrainMesh;
}

namespace Map
{

/// ����ɫ.
class RegionVertexColor : public IO::DataChunk
{
	DeclareChunk(RegionVertexColor);
public:
	RegionVertexColor();
	virtual ~RegionVertexColor();

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
		REGION_WIDTH_IN_VERTEX = REGION_WIDTH_IN_GRID + 1,
		REGION_VERTEX_NUM = REGION_WIDTH_IN_VERTEX*REGION_WIDTH_IN_VERTEX
	};

private:
	/// 32λɫת16λɫ.
	static uint16 X888To565(uint32 c);

private:
	uint16 color[REGION_VERTEX_NUM];
};

}// namespace Map
