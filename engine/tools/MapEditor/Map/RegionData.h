#pragma once
#include "../IO/VersionMapChunk.h"

namespace bly
{
class CTerrainMesh;
}

namespace Map
{
class RegionHeader;
class RegionVertexHeight;
class RegionVertexColor;
class RegionBlendAlpha;
class RegionCliffUV;
class RegionLogicalHeight;
class RegionWater;
class RegionLightInfo;
class RegionBlockInfo;

/// Region���ݶ�.
class RegionData : public IO::DataChunk
{
	DeclareChunk(RegionData);
public:
	RegionData();
	virtual ~RegionData();

	/// д����
	void Write();
	/// �����ж�ȡ
	void Read();

	/// ��������.
	/// @param	CTerrainMesh * terrain
	/// @param	uint index	Region����
	/// @return	void
	void LoadFromTerrain(CTerrainMesh* terrain, uint index);

private:
	RegionHeader*		header;
	RegionVertexHeight* height;
	RegionVertexColor*	color;
	RegionBlendAlpha*	alpha;
	RegionCliffUV*		cliffuv;
	RegionLogicalHeight* logicH;
	RegionWater*		water;
	RegionLightInfo*	light;
	RegionBlockInfo*	block;

	std::vector<DataChunk*> chunkList;
};

}// namespace Map
