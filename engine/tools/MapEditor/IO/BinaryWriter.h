#pragma once
#include "Serializer.h"

namespace sqr
{

SmartPointer(BinaryWriter);

/// ���������л�.
class BinaryWriter : public Serializer
{
public:
	/// 8λ�ַ�
	void WriteChar(char c);
	/// 8λ�޷����ַ�
	void WriteByte(byte c);
	/// 16λ������
	void WriteInt16(int16 s);
	/// 16λ�޷��Ŷ�����
	void WriteUInt16(uint16 s);
	/// 32λ����
	void WriteInt32(int32 i);
	/// 32λ�޷�������
	void WriteUInt32(uint32 i);
	/// �����ȸ�����
	void WriteFloat(float f);
	/// ����ֵ
	void WriteBool(bool b);
	/// �ַ���
	void WriteString(const string& s);
	/// ���ݶ�
	void WriteData(void* data, size_t size);
};

}// namespace sqr
