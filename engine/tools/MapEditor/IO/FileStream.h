#pragma once
#include "Stream.h"

namespace sqr
{
SmartPointer(FileStream);

/// �ļ���
class FileStream :  public Stream
{
public:
	/// ���캯��
	FileStream();
	/// ��������
	virtual ~FileStream();
	/// �Ƿ�ɶ�
	bool CanRead() const;
	/// �Ƿ��д
	bool CanWrite() const;
	/// �Ƿ���Բ���
	bool CanSeek() const;
	/// ���ص�ǰλ��
	index_t GetPosition() const;
	/// ����������
	size_t GetSize() const;
	/// ����
	bool Open();
	/// �ر���
	void Close();
	/// ��ȡһ���ֽڵ�����
	size_t Read(void* data, size_t size);
	/// д��һ���ֽڵ�����
	void Write(void* data, size_t size);
	/// Ѱ�ҵ�ָ��λ��
	void Seek(int offset, SeekOrigin dir);
	/// �Ƿ��Ѿ�������ĩβ
	bool Eof() const;
	/// ӳ�䵽�ڴ�
	void* Map();
	/// ȡ��ӳ��
	void UnMap();

private:
	HANDLE handle;
	void* mappedData;
};


//------------------------------------------------------------------------------
inline bool
FileStream::CanRead() const
{
	return (this->mode != WriteAccess);
}

//------------------------------------------------------------------------------
inline bool
FileStream::CanWrite() const
{
	return (this->mode != ReadAccess);
}

//------------------------------------------------------------------------------
inline bool
FileStream::CanSeek() const
{
	return true;
}

}// namespace sqr
