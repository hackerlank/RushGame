#pragma once
#include "../IO/VersionMapChunk.h"

namespace bly
{
class CTerrainMesh;
}

namespace Map
{

/// ������Ȩ��.
class RegionBlendAlpha : public IO::DataChunk
{
	DeclareChunk(RegionBlendAlpha);
public:
	RegionBlendAlpha();
	virtual ~RegionBlendAlpha();

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
	byte alpha1[REGION_VERTEX_NUM];	///< �ڶ���Ȩ��ֵ
	byte alpha2[REGION_VERTEX_NUM];	///< ������Ȩ��ֵ
};

}// namespace Map
