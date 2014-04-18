#pragma once
#include "DataChunk.h"

namespace IO
{

/// �汾��.
class VersionChunk : public DataChunk
{
public:
	/// ���캯��
	VersionChunk();
	/// ��������
	virtual ~VersionChunk();
	/// д����
	void Write();
	/// �����ж�ȡ
	void Read();
	/// ���ð汾��
	void SetVersion(uint v);
	/// ��ð汾��
	uint GetVersion() const;

private:
	uint version;
};

}// namespace IO

