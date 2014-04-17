#pragma once

#include "CNetworkMallocObject.h"
#include "CByteDecoder.h"


class CSyncPipeInBuffer
	:public CNetworkMallocObject
{
public:
	explicit CSyncPipeInBuffer(size_t stInBufferSize,bool bProcessCompressedData,bool bDecryptData );
	~CSyncPipeInBuffer(void);

	char* InBufferGetWritePos();
	
	size_t InBufferGetLeftSpaceSize()const;

	//eTADR_Nothing �������������ݣ�����û���������ݣ����Բ��ô���AddNetDataEvent;�ֻ��߽�ѹ���ݳ��Ȳ����������ܲ���������
	//eTADR_NewData��Ҫ����NewDataEvent
	//eTADR_Error��Ҫ����disconnect
	enum EProcessRecvDataResult{ePRDR_Nothing,ePRDR_NewData,ePRDR_Error};
#ifdef _WIN32
	EProcessRecvDataResult InBufferTellAddedDataPosAndSize(char* pBeginPos,size_t stNewDataSize);
#endif
	EProcessRecvDataResult InBufferTellAddedDataSize(size_t stNewDataSize);

	size_t InBufferGetSize()const;

	char* InBufferGetData()const;

	size_t InBufferGetDataSize()const;

	bool InBufferPopData(size_t stPopDataSize);

	void InBufferClear();

	bool InBufferIsFull()const;

private:
	
	class CTinyBuffer
	{
	public:
		char*				m_szBuffer;
		size_t				m_stBufferSize;
		size_t				m_stDataSize;

		void Init(size_t stBufferSize);
		void Init();
		void Unit();
		char* GetWritePos()const;
		size_t GetFreeSize()const;
	};

	EProcessRecvDataResult DecompressedData();

	bool DecodeBuffer( CTinyBuffer& Buffer , size_t stNewDataSize );
private:
	

	CTinyBuffer		m_Buffer;
	CTinyBuffer		m_CompressedBuffer;

	const size_t	m_stMaxInBufferSize;	

	const bool	m_bDecompressData:1;
	const bool	m_bDecryptData:1;

	
	CByteDecoder m_Decoder;
	
	bool			m_bRecvHandShaken;
	
};


