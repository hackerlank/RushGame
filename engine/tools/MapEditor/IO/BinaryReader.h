#pragma once
#include "Serializer.h"

namespace sqr
{

SmartPointer(BinaryReader);

/// ���������л�.
class BinaryReader : public Serializer
{
public:
	/// 8λ�ַ�
	char ReadChar();
	/// 8λ�޷����ַ�
	byte ReadByte();
	/// 16λ������
	int16 ReadInt16();
	/// 16λ�޷��Ŷ�����
	uint16 ReadUInt16();
	/// 32λ����
	int32 ReadInt32();
	/// 32λ�޷�������
	uint32 ReadUInt32();
	/// �����ȸ�����
	float ReadFloat();
	/// ����ֵ
	bool ReadBool();
	/// �ַ���
	string ReadString();
	/// ���ݶ�
	void ReadData(void* data, size_t size);

	/// �������ݶ�
	void AbortData(size_t size);
};

}// namespace sqr
