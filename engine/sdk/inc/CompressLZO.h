#pragma once

namespace sqr
{
	class CMemData;

	//return false��ʾѹ����̫�ͣ�����ѹ����sOut���ݲ��ᱻ�޸�
	void CompressBufferLZO( CMemData& bufOut,const char* szData,size_t stSize );

	//return false������Ч
	bool DecompressBufferLZO( char* szOutBuffer,size_t stOrgDataSize,const char* szData,size_t stSize );

}
