#pragma once
#include "../IO/VersionMapChunk.h"

namespace bly
{
class CTerrainMesh;
}

namespace Map
{

/// ������Ч.
class MapAmbientEffect : public IO::DataChunk
{
	DeclareChunk(MapAmbientEffect);
public:
	MapAmbientEffect();
	virtual ~MapAmbientEffect();

	/// д����
	void Write();
	/// �����ж�ȡ
	void Read();

private:

};

}// namespace Map
