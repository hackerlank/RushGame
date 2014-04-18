#pragma once
#include "../IO/VersionMapChunk.h"

namespace bly
{
class COperator;
}

namespace Map
{

/// HDR��Ϣ.
class MapHDRInfo : public IO::DataChunk
{
	DeclareChunk(MapHDRInfo);
public:
	MapHDRInfo();
	virtual ~MapHDRInfo();

	/// д����
	void Write();
	/// �����ж�ȡ
	void Read();

	/// ����
	void LoadFromOperator(COperator* o);

private:
	float	fBrightPass;		///<��ͨ�� 0~1
	float	fGaussMultiplier;	///<����ǿ�� 0~2
	float	fGaussMean;			///<����ƫ�� -1~+1
	float	fGaussStdDev;		///<�����ںͳ̶�  0~1
	float	fExposure;			///<��������0~2
	float	fSample;			///<��϶�
};

}// namespace Map
