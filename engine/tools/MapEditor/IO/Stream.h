#pragma once
#include "CRefObject.h"

namespace sqr
{

SmartPointer(Stream);

/// ������.
/// ��ʱֻ�ṩһЩ�����Ľӿ�
/// ���������ļ���, �ڴ���, �ļ�������
class Stream : public CRefObject
{
public:
	/// ����ģʽ.
	enum AccessMode
	{
		ReadAccess,		///< �ɶ�
		WriteAccess,	///< ��д
		ReadWriteAccess	///< �ɶ���д
	};
	/// Seekʱ�����λ��.
	enum SeekOrigin
	{
		Begin,		///< ���Ŀ�ʼλ��
		Current,	///< ��ǰλ��
		End			///< ��ĩβ
	};
public:
	/// ���캯��
	Stream();
	/// ��������
	virtual ~Stream();
	/// ������Դλ��
	void SetPath(const string& u);
	/// ������Դλ��
	const string& GetPath() const;
	/// ���÷���ģʽ
	void SetAccessMode(AccessMode m);
	/// ���ط���ģʽ
	AccessMode GetAccessMode() const;
	/// ���Ƿ��Ѿ���
	bool IsOpen() const;

	/// ����
	virtual bool Open();
	/// �ر���
	virtual void Close();
	/// �Ƿ�ɶ�
	virtual bool CanRead() const;
	/// �Ƿ��д
	virtual bool CanWrite() const;
	/// �Ƿ����Ѱ��
	virtual bool CanSeek() const;
	/// ���ص�ǰλ��
	virtual index_t GetPosition() const;
	/// ����������
	virtual size_t GetSize() const;
	/// ��ȡһ���ֽڵ�����
	virtual size_t Read(void* data, size_t size);
	/// д��һ���ֽڵ�����
	virtual void Write(void* data, size_t size);
	/// Ѱ�ҵ�ָ��λ��
	virtual void Seek(int offset, SeekOrigin dir);
	/// �Ƿ��Ѿ�������ĩβ
	virtual bool Eof() const;
	/// ӳ�䵽�ڴ�
	virtual void* Map();
	/// ȡ��ӳ��
	virtual void UnMap();
	/// �Ƿ��Ѿ�ӳ��
	bool IsMapped() const;

protected:
	string path;			///< ��Դλ��
	bool isOpen;		///< �Ƿ��
	bool isMapped;
	AccessMode mode;	///< ����ģʽ
	//@todo �̰߳�ȫ���
};

//------------------------------------------------------------------------------
inline bool
Stream::IsOpen() const
{
	return this->isOpen;
}

//------------------------------------------------------------------------------
inline bool
Stream::IsMapped() const
{
	return this->isMapped;
}

//------------------------------------------------------------------------------
inline void
Stream::SetPath( const string& u )
{
	Ast(!this->IsOpen());
	this->path = u;
}

//------------------------------------------------------------------------------
inline const string&
Stream::GetPath() const
{
	return this->path;
}

//------------------------------------------------------------------------------
inline void
Stream::SetAccessMode( AccessMode m )
{
	this->mode = m;
}

//------------------------------------------------------------------------------
inline Stream::AccessMode
Stream::GetAccessMode() const
{
	return this->mode;
}

}// namespace sqr

