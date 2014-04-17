#pragma once


namespace sqr
{

#define PIPEINFINITE	(-1)


//д������ʹ�õ�����dequeһ�µ����ݽṹ���洢�ģ�����deque��ÿһ��trunk��size
#define WRITE_BUFFER_CHUNK_SIZE	4096

	enum EGetEventResult
	{
		eGER_NetMsg		= 1,
#ifdef _WIN32
		eGER_SysMsg		= 2,
		eGER_Iocp		= 4,
#else
		eGER_Signal		= 2,
#endif
		eGER_Canceled	= 8,
		eGER_TimedOut	= 16,
	};

	enum EPipeCreationFlag
	{
		ePCF_SendWayEncryption=1,
		ePCF_RecvWayDecryption=2,
		ePCF_SendWayCompression=4,
		ePCF_RecvWayDecompression=8,
		ePCF_DataSentCallback=16		
	};

}

