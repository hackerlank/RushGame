#pragma once
#include "../IO/VersionMapChunk.h"

namespace bly
{
class CTerrainMesh;
}

namespace Map
{

/// ���򻷾���.
class RegionLightInfo : public IO::DataChunk
{
	DeclareChunk(RegionLightInfo);
public:
	RegionLightInfo();
	virtual ~RegionLightInfo();

	/// д����
	void Write();
	/// �����ж�ȡ
	void Read();

private:

};

}// namespace Map
