#pragma once
#include "../IO/VersionMapChunk.h"

namespace bly
{
class CTerrainMesh;
}

namespace Map
{

/// �����ļ�ͷ.
class RegionHeader : public IO::DataChunk
{
	DeclareChunk(RegionHeader);
public:
	RegionHeader();
	virtual ~RegionHeader();

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
	uint flag;	///< reserved
	uint16 x;	///< Region������
	uint16 z;	///< Region������
};

}// namespace Map
