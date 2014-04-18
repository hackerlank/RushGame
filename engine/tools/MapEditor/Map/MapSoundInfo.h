#pragma once
#include "../IO/VersionMapChunk.h"

namespace bly
{
class COperator;
}

namespace Map
{

/// ������Ϣ.
class MapSoundInfo : public IO::DataChunk
{
	DeclareChunk(MapSoundInfo);
public:
	MapSoundInfo();
	virtual ~MapSoundInfo();

	/// д����
	void Write();
	/// �����ж�ȡ
	void Read();

	/// ����
	void LoadFromOperator(COperator* o);

private:
	std::string backMusic;
	int interval;
};

}// namespace Map
