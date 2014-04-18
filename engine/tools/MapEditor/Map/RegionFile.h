#pragma once
#include "../IO/BinFileBase.h"

namespace Map
{
class RegionIndex;
class RegionData;

/// ��ͼ�ļ�.
class RegionFile : public IO::BinFileBase
{
public:
	RegionFile();
	virtual ~RegionFile();

	/// �����м����ļ�
	void Load();
	/// ���浽��
	void Save();

private:
	/// ��ʼ����Ӧ
	void OnInit();

private:
	RegionIndex* indices;
	std::vector<RegionData*> regionList;
};

}// namespace Map

