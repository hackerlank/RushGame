#pragma once
#include "../IO/VersionMapChunk.h"

namespace Map
{

/// �����ļ�����.
/// ����ÿ���������ļ��е�ƫ��ֵ
class RegionIndex : public IO::DataChunk
{
	DeclareChunk(RegionIndex);
public:
	RegionIndex();
	virtual ~RegionIndex();

	/// д����
	void Write();
	/// �����ж�ȡ
	void Read();

	/// ��ʼ��region��Ŀ
	void Resize(uint regionNum);

	/// ����Region���ļ�ƫ��.
	/// @param	uint index	region����
	/// @param	uint offset	�ļ�ƫ��
	/// @return	void
	void SetOffset(uint index, uint offset);

private:
	std::vector<uint> indexList;
};

}// namespace Map
