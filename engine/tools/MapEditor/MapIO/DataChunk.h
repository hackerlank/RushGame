#pragma once
#include "../Util/FourCC.h"
#include "TRefPointer.h"

namespace IO
{
SmartPointer(BinarySerializer);

/// ���ݿ�.
class DataChunk
{
public:
	/// ���캯��
	DataChunk(const Util::FourCC& id);
	/// ��������
	virtual ~DataChunk();
	/// ȡ��Chunk id
	const Util::FourCC& GetID() const;
	/// Chunk���ݳ���(������FourCC��size����)
	uint Size();
	/// �������л���
	void SetSerializer(const BinarySerializerPtr& s);
	/// �������л���
	const BinarySerializerPtr& GetSerializer() const;

	/// д����
	virtual void Write();
	/// �����ж�ȡ
	/// @note ����ȡFourCC
	virtual void Read();

protected:
	Util::FourCC fourcc;
	uint size;
	BinarySerializerPtr serializer;
};

}// namespace IO
