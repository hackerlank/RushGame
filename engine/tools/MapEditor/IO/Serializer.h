#pragma once
#include "CRefObject.h"
#include "Stream.h"

namespace sqr
{

SmartPointer(Serializer);

/// ���л��ӿ�.
/// ����ʵ�ֶ��������л�, �ı����л�, XML���л�, �ȵ�
/// @todo ӳ�䵽�ڴ�Map()/UnMap()
class Serializer : public CRefObject
{
public:
	/// ���캯��
	Serializer();
	/// ��������
	virtual ~Serializer();
	/// �������л���ص���
	void SetStream(const StreamPtr& s);
	/// �������������
	const StreamPtr& GetStream() const;
	/// �Ƿ���������
	bool HasStream();
	/// ���Ƿ��Ѿ�����ĩβ
	bool Eof();
	/// ������ʼ���л�
	virtual bool Open();
	/// �������л�, �ر���
	virtual void Close();
	/// �Ƿ��Ѿ���ʼ���л�
	bool IsOpen();

protected:
	StreamPtr stream;
	bool isOpen;
};

//------------------------------------------------------------------------------
inline void
Serializer::SetStream( const StreamPtr& s )
{
	Ast(s.isvalid());
	this->stream = s;
}

//------------------------------------------------------------------------------
inline const StreamPtr&
Serializer::GetStream() const
{
	return this->stream;
}

//------------------------------------------------------------------------------
inline bool
Serializer::HasStream()
{
	return this->stream.isvalid();
}

//------------------------------------------------------------------------------
inline bool
Serializer::Eof()
{
	Ast(this->HasStream());
	return this->stream->Eof();
}

//------------------------------------------------------------------------------
inline bool
Serializer::IsOpen()
{
	return this->isOpen;
}

}// namespace sqr

