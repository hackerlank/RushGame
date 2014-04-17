#pragma once
#include "CGraphicMallocObject.h"
namespace sqr
{

/// �����ַ����࣬���ڶ�������SOCKET����
class IDNAME : public CGraphicMallocObject
{
	union
	{
		char		m_Str[16];
		int64		m_Buf[2];
	};
	
public:
	IDNAME();
	IDNAME( const char* str );

	const IDNAME& operator= ( const IDNAME& BN );
	const IDNAME& operator= ( const char* str );
	bool operator== ( const IDNAME& BN ) const;
	bool operator!= ( const IDNAME& BN ) const;
	operator const char*() const;
};
}
