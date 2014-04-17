#pragma once
#include "ExpHelper.h"

namespace sqr
{

/// ����ָ����.
/// ��ҪT��AddRef()��Release()����
template<class T>
class TRefPointer
{
public:
	/// ���캯��
	TRefPointer();
	/// ��C++ָ�빹��
	TRefPointer(T* p);
	/// ������ָ�빹��
	TRefPointer(const TRefPointer<T>& p);
	/// ��������
	~TRefPointer();
	/// ��ֵ
	void operator=(const TRefPointer<T>& rhs);
	/// ��ֵ
	void operator=(T* rhs);
	/// ���
	bool operator==(const TRefPointer<T>& rhs) const;
	/// ������
	bool operator!=(const TRefPointer<T>& rhs) const;
	/// ���
	bool operator==(const T* rhs) const;
	/// ����
	bool operator!=(const T* rhs) const;
	/// ��ȫ��->�����
	T* operator->() const;
	/// ��ȫ��ȡֵ�����
	T& operator*() const;
	/// ��ȫ��ָ��ת��
	operator T*() const;
	/// ����ȫ��, ת��Ϊ��������ָ��
	template<class U> const TRefPointer<U>& cast() const;
	/// ����Ƿ�Ϸ�
	bool isvalid() const;
	/// ֱ�ӷ���ָ�� (��ָ���Ast)
	T* get() const;
	/// ֱ�ӷ���ָ�� (��ָ��ֱ�ӷ���)
	T* get_unsafe() const;
	/// ����ָ���ַ (���&)
	T** address();

private:
	T* ptr;
};
/// ��������ָ�������ĺ궨��
#define SmartPointer(classname) \
	class classname; \
	typedef TRefPointer<classname> classname##Ptr

//------------------------------------------------------------------------------
template<class T>
T** TRefPointer<T>::address()
{
	return &this->ptr;
}

//------------------------------------------------------------------------------
template<class T>
T* TRefPointer<T>::get_unsafe() const
{
	return (this->ptr);
}
//------------------------------------------------------------------------------
template<class T>
T* TRefPointer<T>::get() const
{
	Ast(NULL != this->ptr);
	return this->ptr;
}
//------------------------------------------------------------------------------
template<class T>
bool TRefPointer<T>::isvalid() const
{
	return (NULL != this->ptr);
}
//------------------------------------------------------------------------------
template<class T>
template<class U> const TRefPointer<U>&
TRefPointer<T>::cast() const
{
	return *(TRefPointer<U>*)this;
}
//------------------------------------------------------------------------------
template<class T>
TRefPointer<T>::operator T*() const
{
	//Ast(NULL != this->ptr);
	return this->ptr;
}
//------------------------------------------------------------------------------
template<class T>
T& TRefPointer<T>::operator*() const
{
	Ast(NULL != this->ptr);
	return *this->ptr;
}
//------------------------------------------------------------------------------
template<class T>
T* TRefPointer<T>::operator->() const
{
	Ast(NULL != this->ptr);
	return this->ptr;
}
//------------------------------------------------------------------------------
template<class T>
bool TRefPointer<T>::operator!=( const T* rhs ) const
{
	return (this->ptr != rhs);
}
//------------------------------------------------------------------------------
template<class T>
bool TRefPointer<T>::operator==( const T* rhs ) const
{
	return (this->ptr == rhs);
}
//------------------------------------------------------------------------------
template<class T>
bool TRefPointer<T>::operator!=( const TRefPointer<T>& rhs ) const
{
	return (this->ptr != rhs.ptr);
}
//------------------------------------------------------------------------------
template<class T>
bool TRefPointer<T>::operator==( const TRefPointer<T>& rhs ) const
{
	return (this->ptr == rhs.ptr);
}
//------------------------------------------------------------------------------
template<class T>
void TRefPointer<T>::operator=( T* rhs )
{
	if (this->ptr != rhs)
	{
		if (NULL != this->ptr)
		{
			this->ptr->Release();
		}
		this->ptr = rhs;
		if (NULL != this->ptr)
		{
			this->ptr->AddRef();
		}
	}
}
//------------------------------------------------------------------------------
template<class T>
void TRefPointer<T>::operator=( const TRefPointer<T>& rhs )
{
	if (this->ptr != rhs.ptr)
	{
		if (NULL != this->ptr)
		{
			this->ptr->Release();
		}
		this->ptr = rhs.ptr;
		if (NULL != this->ptr)
		{
			this->ptr->AddRef();
		}
	}
}
//------------------------------------------------------------------------------
template<class T>
TRefPointer<T>::~TRefPointer()
{
	if (NULL != this->ptr)
	{
		this->ptr->Release();
		this->ptr = NULL;
	}
}
//------------------------------------------------------------------------------
template<class T>
TRefPointer<T>::TRefPointer( const TRefPointer<T>& p )
		: ptr(p.ptr)
{
	if (NULL != this->ptr)
	{
		this->ptr->AddRef();
	}
}
//------------------------------------------------------------------------------
template<class T>
TRefPointer<T>::TRefPointer( T* p )
		: ptr(p)
{
	if (NULL != this->ptr)
	{
		this->ptr->AddRef();
	}
}
//------------------------------------------------------------------------------
template<class T>
TRefPointer<T>::TRefPointer()
		: ptr(NULL)
{
	// empty
}

}// namespace sqr
