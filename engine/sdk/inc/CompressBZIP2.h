#pragma once

#ifdef _WIN32
#pragma comment( lib, "libsbz2.lib" )
#endif

//ѹ�����߽�ѹ��������Ǳ�append��sOut�У������ע���Ƿ�Ҫ�ڵ������к���֮ǰ���sOut

namespace sqr
{

#define BZIP_MIN_DATA_SIZE	150

	class CMemData;

	void CompressBufferBZIP2( CMemData& buffOut,const char* szData,size_t stSize );

	bool DecompressBufferBZIP2( char* szOutBuffer,size_t stOrgDataSize,const char* szData,size_t stSize );

}
