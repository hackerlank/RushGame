#pragma once
#include "../IO/VersionMapChunk.h"
namespace bly
{
class CTerrainMesh;
}
namespace Map
{

/// ��ͼ�ļ�ͷ.
class MapHeader : public IO::DataChunk
{
	DeclareChunk(MapHeader);
public:
	/// ���캯����ʼ������
	MapHeader();
	/// ��������
	virtual ~MapHeader();

	/// д����
	void Write();
	/// �����ж�ȡ
	void Read();

	/// ����
	void LoadFromTerrainMesh(CTerrainMesh* terrain);

private:
	uint flag;
	uint16 width;	///< ��ͼ��(Region��λ)
	uint16 height;	///< ��ͼ��(Region��λ)
};

}// namespace Map
