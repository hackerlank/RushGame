#ifndef ___BITSET___
#define ___BITSET___
#include "CMathMallocObject.h"

namespace sqr
{
template< int32 nSize, typename IntType = uint64 >
class TBitset : public CMathMallocObject
{
public:

	char _Buf[ ( nSize - 1 )/8 + 1 ];

	// param1: Ҫ��ȡλֵ��λ��
	// param2: Ҫ��ö���λ
	// ret: ����ֵΪ��ö���λ����ֵ
	uint32 GetBit( int pos, int Num )
	{
		IntType n = *( (IntType*)( _Buf + ( pos >> 3 ) ) );
		n = n >> ( pos&0x07 );
		n = n & ( ( 1 << Num ) - 1 );

		return (uint32)n;
	}

	// param1: Ҫ����λֵ��λ��
	// param2: Ҫ���ö���λ
	// param3: Ҫ���õ�����
	void SetBit( int pos, int Num, uint32 v )
	{
		IntType  m = ( ( 1 << Num ) - 1 ) << ( pos&0x07 );
		IntType& n = *( (IntType*)( _Buf + (pos>>3) ) );

		n = ( n&(~m) )|( ( ( (IntType)v ) << (pos&0x07) )&m );
	}
};
}

#endif

