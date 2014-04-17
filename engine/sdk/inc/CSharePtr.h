/////////////////////////////////////////////////////////////////////////
//
//	Copyright (C) 2004 LiChunXia
//	��ӭ���ʹ��
//
//  ����Ҫ�����������ü������ܣ����ü����Ͷ��󹲴���
//////////////////////////////////////////////////////////////////////////

#ifndef _CSHAREPTR_H_
#define _CSHAREPTR_H_

template <class T>
class CSharePtr
{
private:
	/*
	 *  ��ֹ�������壿
	 */
	CSharePtr()
	{
	}

public:
	/*
	 * ʹ����������
	 */
	CSharePtr(T* otherObj)
	{
		//if ( otherObj )
		//{
		m_pObj  = otherObj;
		m_pnRef = new int(1);
		//}
	}

	~CSharePtr()
	{
		Release();
	}

	/*
	 *   �������ü���
	 */
	void Release()
	{
		if ( m_pnRef && --*m_pnRef == 0 )
		{
			if ( m_pObj )
				delete m_pObj;
			m_pObj = NULL;

			delete m_pnRef;
			m_pnRef = NULL;
		}
	}

	/*
	 * �ж��Ƿ�Ϊ��
	 */
	bool IsNull()
	{
		return ( m_pObj == NULL );
	}

	/*
	 * ���²���һ������
	 */
	void Reset(T* otherObj = 0)
	{
		//if(otherObj)
		//{
		Release();

		m_pObj = otherObj;
		m_pnRef  = new int(1);
		//}
	}


	/*
	 * ��������
	 */
	CSharePtr(const CSharePtr<T>& other)
	{
		m_pObj  = other.m_pObj;
		m_pnRef = other.m_pnRef;

		++(*m_pnRef);
	}

	/*
	 * ��ֵ�����
	 */
	CSharePtr<T>& operator = (const CSharePtr<T> &other)
	{
		if ( this == &other || this->m_pObj == other.m_pObj )
			return *this;

		Release();

		m_pObj  = other.m_pObj;
		m_pnRef = other.m_pnRef;

		++(*m_pnRef);

		return *this;
	}

	bool operator == (const T * otherObj) const
	{
		return m_pObj == otherObj;
	}

	bool operator == ( const CSharePtr<T>& other ) const
	{
		return m_pObj == other.m_pObj;
	}

	bool operator != ( const CSharePtr<T>& other ) const
	{
		return m_pObj != other.m_pObj;
	}

	T& operator * () const
	{
		return *m_pObj;
	}

	T* operator -> () const
	{
		return m_pObj;
	}

	operator T * () const
	{
		return m_pObj;
	}

	/*T * CSharePtr() const
	{
		return m_pObj;
	}*/

	T* Get() const
	{
		return m_pObj;
	}

	long UseCount() const
	{
		return *m_pnRef;
	}

	bool IsUnique() const
	{
		return (*m_pnRef == 1);
	}

private:
	T*   m_pObj;
	int* m_pnRef;
};

#endif