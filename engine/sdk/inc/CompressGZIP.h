#pragma once

#ifdef _WIN32
#ifndef _DEBUG
#pragma comment( lib, "zlib.lib" )
#else
#pragma comment( lib, "zlibd.lib" )
#endif
#endif

//ѹ�����߽�ѹ��������Ǳ�append��sOut�У������ע���Ƿ�Ҫ�ڵ������к���֮ǰ���sOut

#define GZIP_MIN_DATA_SIZE	60

namespace sqr
{
	class CMemData;

	//return false��ʾѹ����̫�ͣ�����ѹ����sOut���ݲ��ᱻ�޸�
	void CompressBufferGZIP( CMemData& bufOut,const char* szData,size_t stSize );

	//return false������Ч
	bool DecompressBufferGZIP( char* szOutBuffer,size_t stOrgDataSize,const char* szData,size_t stSize );

}
