#pragma once
#include "../IO/BinFileBase.h"

namespace IO
{
class VersionMapChunk;
}

namespace Map
{
class MapHeader;
class MapSceneInfo;
class MapSoundInfo;
class MapAmbientEffect;
class MapHDRInfo;

/// ��ͼ�ļ�.
class MapFile : public IO::BinFileBase
{
public:
	MapFile();
	virtual ~MapFile();

private:
	/// ��ʼ����Ӧ
	void OnInit();

private:
	IO::VersionMapChunk* versions;
	MapHeader* mapHeader;
	MapSceneInfo* sceneInfo;
	MapSoundInfo* soundInfo;
	MapAmbientEffect* ambientFx;
	MapHDRInfo* hdrInfo;
};

}// namespace Map

