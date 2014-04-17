#include "stdafx.h"
#include "CSyncPipeOutBuffer.h"
#include "TSqrAllocator.inl"
#include "Memory.h"
#include "ExpHelper.h"
#include "CompressLZO.h"
#include "PipeTypes.h"
#include "CCompressedDataHeader.h"
#include "CMemData.inl"
#include "CByteEncoder.inl"


CSyncPipeOutBuffer::CSyncPipeOutBuffer(size_t stOutBufferSize,bool bCompressData,bool bEncryptData)
:m_bufCompressedData( CNetworkSendBufferMallocObject::GetPool() )
,m_bCompressData ( bCompressData )
,m_bEncryptData( bEncryptData )
{
	m_stOutBufferSize=stOutBufferSize;
	m_stOutBufferSegNum=0;
	OutBufferClear();
}

CSyncPipeOutBuffer::~CSyncPipeOutBuffer(void)
{
	for(BufferList_t::iterator it=m_lstOutBuffer.begin();it!=m_lstOutBuffer.end();++it)
	{
		SqrDelete(*it, CNetworkSendBufferMallocObject::GetPool());
	}
}

void CSyncPipeOutBuffer::OutBufferClear()
{
	m_stOutHeadDataBegin=0;
	m_stOutTailDataEnd=0;
	
	for(BufferList_t::iterator it=m_lstOutBuffer.begin();it!=m_lstOutBuffer.end();++it)
	{
		SqrDelete(*it, CNetworkSendBufferMallocObject::GetPool());
	}

	m_lstOutBuffer.clear();
	m_lstOutBuffer.push_back((char*)SqrNew(WRITE_BUFFER_CHUNK_SIZE * sizeof(char), 
		CNetworkSendBufferMallocObject::GetPool()));
	m_stOutBufferSegNum=1;

	m_bufCompressedData.Clear();
}

#ifdef _WIN32
void CSyncPipeOutBuffer::PrepareSendData(WSABUF*& pBuffer,DWORD& dwBufferCount)
{
	if( !m_bCompressData )
	{
		if( m_stOutTailDataEnd>0 )
		{
			dwBufferCount=(DWORD)m_stOutBufferSegNum;
			pBuffer=new WSABUF[dwBufferCount];
			pBuffer[0].buf=m_lstOutBuffer.front()+m_stOutHeadDataBegin;

			BufferList_t::iterator it=++m_lstOutBuffer.begin();
			BufferList_t::iterator end=m_lstOutBuffer.end();

			if( it==end )
			{
				pBuffer[0].len=(u_long)(m_stOutTailDataEnd-m_stOutHeadDataBegin);
			}
			else
			{
				pBuffer[0].len=(u_long)(WRITE_BUFFER_CHUNK_SIZE-m_stOutHeadDataBegin);

				if(it==--end)
				{
					pBuffer[1].buf=*it;
					pBuffer[1].len=(u_long)m_stOutTailDataEnd;
				}
				else
				{
					for(size_t i=1;it!=end;++it,++i)
					{
						pBuffer[i].buf=*it;
						pBuffer[i].len=WRITE_BUFFER_CHUNK_SIZE;
					}
					pBuffer[m_stOutBufferSegNum-1].buf=*end;
					pBuffer[m_stOutBufferSegNum-1].len=(u_long)m_stOutTailDataEnd;
				}
			}
		}
		else
		{
			dwBufferCount=(uint32)(m_stOutBufferSegNum-1);
			pBuffer=new WSABUF[dwBufferCount];
			pBuffer[0].buf=m_lstOutBuffer.front()+m_stOutHeadDataBegin;
			pBuffer[0].len=(u_long)(WRITE_BUFFER_CHUNK_SIZE-m_stOutHeadDataBegin);

			BufferList_t::iterator it=++m_lstOutBuffer.begin();
			BufferList_t::iterator end=--m_lstOutBuffer.end();

			for(size_t i=1;it!=end;++it,++i)
			{
				pBuffer[i].buf=*it;
				pBuffer[i].len=WRITE_BUFFER_CHUNK_SIZE;
			}
		}
	}
	else
	{
		CompressDataIfNeeded();

		dwBufferCount=1;
		pBuffer=new WSABUF;
		pBuffer->buf=m_bufCompressedData.Data();
		pBuffer->len=u_long(m_bufCompressedData.Size());
		/*FILE* fp=fopen("send.txt","a");
		fwrite( pBuffer->buf,1,pBuffer->len,fp);
		fclose(fp);*/
	}
}
#endif




void CSyncPipeOutBuffer::CompressDataIfNeeded()
{
	if( !m_bufCompressedData.Empty() )
		return;

	const size_t stChunkDataSize = IntGetChunkDataSize();

	if( stChunkDataSize == 0 )
		return;

	const char* szChunkData = IntGetChunkData();

	//2 byte δѹ������
	//1 byte ����Ƿ�ѹ��
	//2 byte ѹ���󳤶ȣ����δѹ������û����2byte��
	
	//150��һ��������ֵ�����ȴﵽ150��ʱ��ѹ����һ���ܵ�70%��̫С�����ݰ�������ѹ�������Լ���cpu����
	if( stChunkDataSize < 150 )
	{
		m_bufCompressedData.ReSize( sizeof( CDataHeader ) + stChunkDataSize );

		CDataHeader* pHeader=m_bufCompressedData.AsPtr<CDataHeader>();
		
		pHeader->m_uCompressed = 0;
		pHeader->m_uDataLen = uint16(stChunkDataSize);
		
		if( m_bEncryptData )
		{
			char* p = reinterpret_cast<char*>( pHeader );

			//header�ĳ���Ϊ2������ִ������byte encode.
			*p=m_Encoder.Encode(*p);
			++p;
			*p=m_Encoder.Encode(*p);
			++p;
			
			char* const pEnd = p + stChunkDataSize;

			for( ; p<pEnd ; ++p,++szChunkData )
				*p=m_Encoder.Encode(*szChunkData);
		}
		else
		{
			m_bufCompressedData.Assign( sizeof( CDataHeader ) , szChunkData , stChunkDataSize );
		}

		//printf("not compress stChunkDataSize:%d m_sCompressedDataSize:%d\n",int(stChunkDataSize),int(m_bufCompressedData.Size()));
	}
	else
	{
		m_bufCompressedData.ReSize( sizeof( CCompressedDataHeader) );
		
		//�м���������Ҫ������������棬��Ϊѹ����֮��string�ڲ��������ڴ���ܶ��Ѿ����·�����
		CompressBufferLZO( m_bufCompressedData,szChunkData ,stChunkDataSize );

		CCompressedDataHeader* pHeader= m_bufCompressedData.AsPtr<CCompressedDataHeader>();

		pHeader->m_uCompressed = 1;
		pHeader->m_uDataLen = uint16(stChunkDataSize);

		Ast( m_bufCompressedData.Size() <= 0xffff );
		
		pHeader->m_uCompressedDataLen = uint16( m_bufCompressedData.Size()-sizeof( CCompressedDataHeader ) );
		
		if( m_bEncryptData )
			m_Encoder.EncodeBuffer( m_bufCompressedData.Data(),m_bufCompressedData.Size() );

		//printf("compress stChunkDataSize:%d stDataSize:%d m_sCompressedDataSize:%d\n"
		//	,int(stChunkDataSize),int(m_bufCompressedData.Size()-5),int(m_bufCompressedData.Size()));
	}

	IntPopChunkData( stChunkDataSize );
}



size_t CSyncPipeOutBuffer::OutBufferGetDataSize()const
{
	return (m_stOutBufferSegNum-1)*WRITE_BUFFER_CHUNK_SIZE+m_stOutTailDataEnd-m_stOutHeadDataBegin;
}


size_t CSyncPipeOutBuffer::OutBufferGetFreeSize()const
{
	return m_stOutBufferSize-OutBufferGetDataSize();
}

bool CSyncPipeOutBuffer::OutBufferIsFull()const
{
	return OutBufferGetFreeSize() == 0;
}


size_t CSyncPipeOutBuffer::OutBufferGetSize()const
{
	return m_stOutBufferSize;
}


size_t CSyncPipeOutBuffer::OutBufferPushData(const char* pData,size_t stDataSize)
{
	//��ȡд����������ʣ��ռ�
	size_t stLeftSize=m_stOutBufferSize-OutBufferGetDataSize();
	size_t stReturnValue;

	if( stLeftSize < stDataSize )
	{
		stReturnValue=stLeftSize;
		stDataSize=stLeftSize;
	}
	else
	{
		stReturnValue=stDataSize;
	}

	//��ѹ����ʱ�򣬲����������������м��ܣ�����Ҫ��ѹ���������ѹ��֮����м���
	const bool bEncryptDataHere = (!m_bCompressData) && m_bEncryptData;

	while(stDataSize>0)
	{
		size_t stCopySize;	//���һ������ֻ�ܷ�����ô�������

		if(WRITE_BUFFER_CHUNK_SIZE<=m_stOutTailDataEnd)
		{
			m_lstOutBuffer.push_back((char*)SqrNew(WRITE_BUFFER_CHUNK_SIZE * sizeof(char), 
				CNetworkSendBufferMallocObject::GetPool()));
			//m_lstOutBuffer.push_back(new char[WRITE_BUFFER_CHUNK_SIZE]);
			++m_stOutBufferSegNum;

			m_stOutTailDataEnd=0;
			stCopySize=WRITE_BUFFER_CHUNK_SIZE;
		}
		else
		{
			stCopySize=WRITE_BUFFER_CHUNK_SIZE-m_stOutTailDataEnd;	//���һ������ֻ�ܷ�����ô�������
		}

		if(stDataSize<stCopySize)
			stCopySize=stDataSize;

		char*const pWritePos=m_lstOutBuffer.back()+m_stOutTailDataEnd;
		memcpy(pWritePos,pData,stCopySize);
		if( bEncryptDataHere )
			m_Encoder.EncodeBuffer( pWritePos, stCopySize );

		pData+=stCopySize;
		stDataSize-=stCopySize;
		m_stOutTailDataEnd+=stCopySize;
	}
	return stReturnValue;
}


size_t CSyncPipeOutBuffer::IntGetChunkDataSize()
{
	if( m_stOutBufferSegNum > 1 )
		return WRITE_BUFFER_CHUNK_SIZE-m_stOutHeadDataBegin;
	return m_stOutTailDataEnd-m_stOutHeadDataBegin;
}


const char* CSyncPipeOutBuffer::IntGetChunkData()
{
	return m_lstOutBuffer.front()+m_stOutHeadDataBegin;
}


void CSyncPipeOutBuffer::IntPopChunkData(size_t stPopDataSize)
{
	//���ﲻ��Underflow���,�������д��û��Ͳ���underflow,����inbuffer��overflow��ͬ,
	//��ʹ����д��û��,����̫��Ҳ�����overflow,��Ϊ������������������

	while( (stPopDataSize>=WRITE_BUFFER_CHUNK_SIZE-m_stOutHeadDataBegin) && (m_stOutBufferSegNum>1))
	{
		SqrDelete(m_lstOutBuffer.front(), CNetworkSendBufferMallocObject::GetPool());
		m_lstOutBuffer.pop_front();
		--m_stOutBufferSegNum;
		stPopDataSize-=WRITE_BUFFER_CHUNK_SIZE-m_stOutHeadDataBegin;
		m_stOutHeadDataBegin=0;
	}
	
	m_stOutHeadDataBegin+=stPopDataSize;
	
	if(m_stOutBufferSegNum==1)
	{
		Ast(m_stOutHeadDataBegin<=m_stOutTailDataEnd);

		if(m_stOutHeadDataBegin==m_stOutTailDataEnd)
			m_stOutHeadDataBegin=m_stOutTailDataEnd=0;
	}
}



size_t CSyncPipeOutBuffer::OutBufferGetChunkDataSize()
{
	if( !m_bCompressData )
		return IntGetChunkDataSize();

	CompressDataIfNeeded();
	
	return m_bufCompressedData.Size();
}


const char* CSyncPipeOutBuffer::OutBufferGetChunkData()
{
	if( !m_bCompressData )
		return IntGetChunkData();

	CompressDataIfNeeded();
	
	return m_bufCompressedData.Data();
}


void CSyncPipeOutBuffer::OutBufferPopChunkData(size_t stPopDataSize)
{
	if( !m_bCompressData )
		return IntPopChunkData( stPopDataSize );

	Ast( stPopDataSize <= m_bufCompressedData.Size() );
	m_bufCompressedData.Erase( 0,stPopDataSize );

	//printf("PopChunkData m_bufCompressedData.Size():%d\n",int(m_bufCompressedData.Size()));
}


bool CSyncPipeOutBuffer::NoDataToSend()const
{
	return m_bufCompressedData.Empty() && m_stOutHeadDataBegin==0 && m_stOutTailDataEnd==0;
}

