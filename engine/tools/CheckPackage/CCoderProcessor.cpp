#include "stdafx.h"
#include "CCoderProcessor.h"
#include "CCoderNameHelper.h"
#include "SQRWndDef.h"
#include "CRefCoder.h"

CCoderProcessor::CCoderProcessor(FourCC ccType, uint16 NewVer,CFileFormatMgr::CHeadCoder* pCoder)
: CFileFormatMgr(ccType, NewVer, pCoder)
{

}

CCoderProcessor::~CCoderProcessor(void)
{
}

// ע����Ҫ�ռ������TRUNK
void CCoderProcessor::RegProcessTrunk(FourCC fourcc)
{
	// �������coder���Ե����ã�ʵ�����ռ�
	// �����Ҫ��res���Ե�
	m_CltType.insert(fourcc);
}

bool CCoderProcessor::RunProcess( const TCHAR* szFilename )
{
	m_fileName.clear();
	ChangeToSrcDir();

	if( !Load(L"", szFilename, NULL) )
		return false;

	set<string>::iterator itr = m_fileName.begin();
	while(itr != m_fileName.end())
	{
		m_pMgr->ProcessWith((*itr).c_str(), szFilename);
		++itr;
	}

	m_pMgr->PostProcessed(szFilename);
	return true;
}

void CCoderProcessor::_AutoDecode(const	CDataChunk& inData, void* handle)
{
	CDataChunk	FileData;
	CBufFile File(inData.GetBuffer(), inData.GetSize() );

	// ���ݴ���ʼ
	size_t Size = inData.GetSize();
	CDataChunk::ChunkHdr hdr = inData.GetHdr();
	while(Size > 0)
	{
		if (hdr.type == CDataChunk::DCT_AUTO)
		{
			File.read(&FileData.GetHdr(),sizeof(CDataChunk::ChunkHdr));
			Size -= sizeof(CDataChunk::ChunkHdr);
			if(Size>=FileData.GetSize())
			{
				ReadChunk(File, FileData);
				
				_AutoDecode(FileData, NULL);
				Size -= (uint32)FileData.GetSize();
			}
			else
				break;
		}
		else if ( m_CltType.find(inData.GetName()) != m_CltType.end() )
		{
			if ( inData.GetBuffer() == 0 || inData.GetSize() == 0 )
				break;

			string refName;
			CBufFile bufFile(inData.GetBuffer(), inData.GetSize());
			int32 size = bufFile.GetBufSize();
			vector<char> temp( size + 1, 0);
			bufFile.read(&temp[0], size);
			refName = &temp[0];
			m_fileName.insert(refName);
			break;
		}
		else
			break;
	}
}