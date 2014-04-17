#pragma once
#include "CFourCC.h"
namespace sqr
{
/*
��װ��һ���ڴ��,����һ��ָ��ָ����ڴ�,�Լ�����ڴ�Ĵ�С eg:[name][size][data].
@��ע
����һ����һ���ڴ���װ����.
��ͬʱҲ����������ڴ��Ĵ�С(size)�Լ��򵥵ı�ʾ��(name)
@ע��
Ĭ�ϵ� DataChunk �Ķ�������ʱ<��>���ͷ��κ��ڴ棬����ͨ���ͷź�����������ͷš�
@��ע
������������������ʱ���ͷ��ڴ棩��Ϊ���������������������ط���ʱ��õ����ߵ�Ч�ʣ�
��������Ҳ��֤��DataChunk�İ�ȫ�Ŀ�������ֵ����
eg:����Է��Ľ�DataChunk�Ķ�����Ϊ�����ķ���ֵ�����ص��������ڴ汻�ͷŵ���
������Ҫ�ͷ������ڴ��ʱ������Ե���DataChunk::free
@��ע
�������Ҫһ��������������ʱ���ͷ��ڴ��DataChunk������SDataChunk���� -> ������δ֧�� ->Ҳ�������ò���

*/
class CDataChunk
{
public:
	struct ChunkHdr
	{
		ChunkHdr()
			: dwName(0)
			, ver(0)
			, type(0)
			, stSize(0)
		{
		}

		FourCC	dwName;
		uint16	ver;
		uint16	type;
		size_t	stSize;
	};

	enum EDataChunkTypeMask
	{
		DCT_DEPEND		= 0x0001,
		DCT_AUTO		= 0x0002,
		DCT_LEAF		= 0x0003,
		DCT_REFERENCE	= 0x0004,
	};

public:
	CDataChunk();
	CDataChunk( void* pData,size_t fSize );
	~CDataChunk();

	//char*		allocate( size_t size, const char* ptr = NULL );//���·�������
	char* 		SetBuffer( char* ptr,size_t size );
	//void		free();
	char* 		GetBuffer( void );
	const char* GetBuffer( void ) const;
	size_t		GetSize( void )const;
	FourCC		GetName( void )const;
	DWORD		SetName( FourCC fName );
	DWORD		SetName( const std::string& s );
	uint16		GetVersion(void) const;
	void		SetVersion(uint16 ver);
	void		SetType(uint16 type);
	uint16		GetType() const;

	ChunkHdr&	GetHdr(void) const;

	CDataChunk& operator = (CDataChunk& fData );

protected:
	mutable ChunkHdr	m_Header;
	mutable char* 		m_pBuffer;
	mutable char* 		m_pPos;
	mutable	char* 		m_pEnd;
private:
};
}
