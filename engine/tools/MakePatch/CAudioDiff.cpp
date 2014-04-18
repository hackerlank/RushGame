#include "stdafx.h"
#include "AssertTreat.h"
#include "FileProcess.h"
#include "StringProcess.h"
#include "GenerateHash.h"
#include "CAudioDiff.h"

CAudioDiff::CAudioDiff()
	: m_DstWaveData(NULL)
	, m_Offset(0)
	, m_TotalNameLen(0)
	, m_uAllSize(0)
	, m_DataOffset(0)
	, m_InfoData(NULL)
{
}

CAudioDiff::~CAudioDiff()
{
}

// ����ֵΪд���ļ���С��max(uSrcSize, uDstSize)�������Ҫ�����д�룬uSrcSize��0����
static uint32 WriteXorContent(FILE* fp, uint32 uSrcSize, uint8* szSrc, uint32 uDstSize, uint8* szDst)
{
	uint32 i = 0;
	uint32 k = 0;
	uint8 szXorBuf[MAX_BUFFER];
	uint8* pSrcData = szSrc;
	uint8* pDstData = szDst;
	for ( i=0; i<uSrcSize && i<uDstSize; ++i )
	{
		szXorBuf[k] = *pDstData ^ *pSrcData;
		++k;
		++pDstData;
		++pSrcData;

		if ( k >= MAX_BUFFER )
		{
			fwrite(szXorBuf, sizeof(uint8), MAX_BUFFER, fp);
			k = 0;
		}
	}

	// д���Ѿ���ȡ������
	if ( k > 0 )
	{
		fwrite(szXorBuf, sizeof(uint8), k, fp);
		k = 0;
	}

	// ���Դ�ļ���Ŀ���ļ�ͬ����С���˽���
	if ( uSrcSize == uDstSize )
		return uSrcSize;

	uint32 uLeaveSize = abs(uSrcSize - uDstSize);
	if ( uSrcSize > uDstSize )
	{
		fwrite(pSrcData, sizeof(uint8), uLeaveSize, fp);
		return uSrcSize;
	}
	else
	{
		fwrite(pDstData, sizeof(uint8), uLeaveSize, fp);
		return uDstSize;
	}
}

// ������Ƶ����
void CAudioDiff::LoadAudioData(const char* FileName, WaveInfo* info)
{
	FILE* fp;
	fp = fopen(FileName, "rb");
	AstFile(fp, FileName);
	int FileLen = GetFileLength(fp);

	fread(&info->Head, sizeof(WAVEBANKHEADER), 1, fp);
	fseek(fp, info->Head.Segments[WAVEBANK_SEGIDX_BANKDATA].dwOffset, SEEK_SET);

	fread(&info->BankData, sizeof(WAVEBANKDATA), 1, fp);
	fseek(fp, info->Head.Segments[WAVEBANK_SEGIDX_ENTRYMETADATA].dwOffset, SEEK_SET);

	int uFileCount = info->BankData.dwEntryCount;
	info->MetaDataList = new WAVEBANKENTRY[uFileCount];

	int i = 0;
	for (i=0; i<uFileCount; ++i)
		fread(&info->MetaDataList[i], sizeof(WAVEBANKENTRY), 1, fp);

	fclose(fp);
}

// ��ȡ�ļ���������
void CAudioDiff::ReadData(FILE* fp, uint32 idx, uint32 uFileLen, WaveInfo* info)
{
	uint32 uHash = 0;
	// ����������
	uint32 uSrcDataOffset = info->Head.Segments[WAVEBANK_SEGIDX_ENTRYWAVEDATA].dwOffset;
	uint32 uSrcFileCount = info->BankData.dwEntryCount;

	fseek(fp, uSrcDataOffset + info->MetaDataList[idx].PlayRegion.dwOffset, SEEK_SET);
	uint32 uFileSize = info->MetaDataList[idx].PlayRegion.dwLength;

	// ��ȡ�ļ�����
	uint8* szBuf = new uint8[uFileSize];
	fread(szBuf, sizeof(uint8), uFileSize, fp);

	uHash = HashCode(szBuf, uFileSize);
	AudioNode* pData = new AudioNode;
	memset(pData, 0, sizeof(AudioNode));
	pair<FileDataMap_t::iterator, bool> it = m_mapSrcWaveData.insert(make_pair(uHash, pData));

	it.first->second->uFileNumber = idx;
	it.first->second->uFileSize = uFileSize;
	it.first->second->Content = szBuf;

	// ��ȡ�ļ���϶����
	if (idx < uSrcFileCount - 1)
		it.first->second->uSpaceSize = info->MetaDataList[idx+1].PlayRegion.dwOffset -
			( info->MetaDataList[idx].PlayRegion.dwOffset + info->MetaDataList[idx].PlayRegion.dwLength );
	else
		it.first->second->uSpaceSize = uFileLen - uSrcDataOffset -
			(info->MetaDataList[idx].PlayRegion.dwOffset + info->MetaDataList[idx].PlayRegion.dwLength);

	it.first->second->Space = new uint8[it.first->second->uSpaceSize];
	fread(it.first->second->Space, sizeof(uint8), it.first->second->uSpaceSize, fp);
}

// ���diff�󼴿��ͷŵ��ڴ�
void CAudioDiff::Clear()
{
	FileDataMap_t::iterator iter = m_mapSrcWaveData.begin();
	for (; iter != m_mapSrcWaveData.end(); ++iter)
		delete[] iter->second->Content;

	m_mapSrcWaveData.clear();

	delete[] m_SrcInfo.MetaDataList;
	delete[] m_DstInfo.MetaDataList;
}

bool CAudioDiff::MakeDiff(const char* szNewFile, const char* szOldFile)
{
	bool bNeedPatch = false;
	uint32 i = 0;

	// ��ȡ�ṹ��Ϣ
	ReadInfo(szNewFile, szOldFile);

	// ��Դ�ļ���Ŀ���ļ�����ʱ�����ļ�
	FILE* ofp = fopen(szOldFile, "rb");
	AstFile(ofp, szOldFile);
	uint32 uSrcFileLen = GetFileLength(ofp);

	FILE* nfp = fopen(szNewFile, "rb");
	AstFile(nfp, szNewFile);
	uint32 uDstFileLen = GetFileLength(nfp);

	FILE* tfp = fopen(AUDIO_TEMP_FILE, "wb");
	AstFile(tfp, AUDIO_TEMP_FILE);

	// ��ȡ���ݿ�
	uint32 uSrcDataOffset = m_SrcInfo.Head.Segments[WAVEBANK_SEGIDX_ENTRYWAVEDATA].dwOffset;
	uint8* SrcInfoData = new uint8[uSrcDataOffset];
	fread(SrcInfoData, sizeof(uint8), uSrcDataOffset, ofp);

	if ( uSrcDataOffset != m_DstInfo.Head.Segments[WAVEBANK_SEGIDX_ENTRYWAVEDATA].dwOffset )
	{
		bNeedPatch = true;
	}
	else
	{
		for (i=0; i<uSrcDataOffset; ++i)
		{
			if (m_InfoData[i] != SrcInfoData[i])
			{
				bNeedPatch = true;
				break;
			}
		}
	}

	delete[] SrcInfoData;

	// ��ȡԴ�ļ���������
	uint32 uSrcFileCount = m_SrcInfo.BankData.dwEntryCount;
	for (i=0; i<uSrcFileCount; ++i)
		ReadData(ofp, i, uSrcFileLen, &m_SrcInfo);

	// ��ȡĿ���ļ���������
	uint32 uDstFileCount = m_DstInfo.BankData.dwEntryCount;
	uint32 uDstDataOffset = m_DstInfo.Head.Segments[WAVEBANK_SEGIDX_ENTRYWAVEDATA].dwOffset;
	fseek(nfp, uDstDataOffset, SEEK_SET);
	uint32 uSpaceSize = 0;

	m_DstWaveData = new AudioNode[uDstFileCount];
	uint32 uContentOffset = uDstDataOffset;
	m_uAllSize = uDstDataOffset + (sizeof(AudioNode) - 8) * uDstFileCount;

	uint32 uHash = 0;
	uint32 uFileSize = 0;
	for (i=0; i<uDstFileCount; ++i)
	{
		fseek(nfp, uDstDataOffset + m_DstInfo.MetaDataList[i].PlayRegion.dwOffset, SEEK_SET);
		// ����ļ���С
		uFileSize = m_DstInfo.MetaDataList[i].PlayRegion.dwLength;
		uint8* szBuf = new uint8[uFileSize];
		fread(szBuf, sizeof(uint8), uFileSize, nfp);
		// ��Ϊ�ļ����ܻ��ж������(����)�������ж��ļ��Ƿ���ڱ���������hash
		// ��Ƶ����û��modify��ʽԭ��Ҳ�ڴˣ��޷��ж�һ���ļ���ԭ�ļ����޸Ļ�����ȫ��ͬ����һ�ļ�
		uHash = HashCode(szBuf, uFileSize);

		m_DstWaveData[i].uContentOffset = m_DstInfo.MetaDataList[i].PlayRegion.dwOffset;

		FileDataMap_t::iterator EndIter = m_mapSrcWaveData.end();
		FileDataMap_t::iterator it = m_mapSrcWaveData.find(uHash);
		if (it == EndIter) // ����hash���ܴ��ڳ�ͻ������Ӧ��ʹ���ļ���������hash�����ж�
		{
			m_DstWaveData[i].uFlag = 'addf'; // ֻҪ�������޸�hashֵ��ͬ�͵���addf(����modi���)
			m_DstWaveData[i].uFileSize = uFileSize;
			m_DstWaveData[i].uFileNumber = -1;
			m_DstWaveData[i].Content = szBuf;

			bNeedPatch = true;
		}
		else
		{
			m_DstWaveData[i].uFlag = 'inva';
			m_DstWaveData[i].uFileSize = uFileSize;
			m_DstWaveData[i].uFileNumber = it->second->uFileNumber;
			m_DstWaveData[i].Content = NULL;
		}

		// ������ݼ�δ����϶��С
		if ( i < uDstFileCount - 1 )
			uSpaceSize = m_DstInfo.MetaDataList[i+1].PlayRegion.dwOffset -
				(m_DstInfo.MetaDataList[i].PlayRegion.dwOffset + m_DstInfo.MetaDataList[i].PlayRegion.dwLength);
		else
			uSpaceSize = uDstFileLen - uDstDataOffset -
				(m_DstInfo.MetaDataList[i].PlayRegion.dwOffset + m_DstInfo.MetaDataList[i].PlayRegion.dwLength);

		m_DstWaveData[i].uSpaceSize = uSpaceSize;

		fwrite(&m_DstWaveData[i].uFlag, sizeof(uint32), 1, tfp);
		fwrite(&m_DstWaveData[i].uContentOffset, sizeof(uint32), 1, tfp);
		fwrite(&m_DstWaveData[i].uFileSize, sizeof(uint32), 1, tfp);
		fwrite(&m_DstWaveData[i].uSpaceSize, sizeof(uint32), 1, tfp);
		fwrite(&m_DstWaveData[i].uFileNumber, sizeof(uint32), 1, tfp);

		if (m_DstWaveData[i].uFlag == 'addf')
		{
			WriteXorContent( tfp, 0, NULL, m_DstWaveData[i].uFileSize, m_DstWaveData[i].Content );
			m_uAllSize += m_DstWaveData[i].uFileSize;
		}

		// ��¼�ļ���հײ���
		if (m_DstWaveData[i].uSpaceSize != 0)
		{
			m_DstWaveData[i].Space = new uint8[m_DstWaveData[i].uSpaceSize];
			fread(m_DstWaveData[i].Space, sizeof(uint8), m_DstWaveData[i].uSpaceSize, nfp);

			// ��¼���ݼ�Ŀ�϶�������
			if (m_DstWaveData[i].uFlag == 'addf')
				WriteXorContent( tfp, 0, NULL, m_DstWaveData[i].uSpaceSize, m_DstWaveData[i].Space );
			else
				WriteXorContent( tfp, it->second->uSpaceSize, it->second->Space,
					m_DstWaveData[i].uSpaceSize, m_DstWaveData[i].Space );

			delete[] m_DstWaveData[i].Space;
			m_DstWaveData[i].Space = NULL;
			m_uAllSize += m_DstWaveData[i].uSpaceSize;
		}

		delete[] szBuf;
	}

	fclose(ofp);
	fclose(nfp);
	fclose(tfp);

	// ����Ѿ������ͷŵ�����
	Clear();

	return bNeedPatch; // ��Ҫ�з���false�����
}


void CAudioDiff::ReadInfo(const char* szNewFile, const char* szOldFile)
{
	LoadAudioData(szOldFile, &m_SrcInfo);
	LoadAudioData(szNewFile, &m_DstInfo);

	m_DataOffset = m_DstInfo.Head.Segments[WAVEBANK_SEGIDX_ENTRYWAVEDATA].dwOffset;
	FILE *fp = fopen(szNewFile, "rb");
	AstFile(fp, szNewFile);
	m_InfoData = new uint8[m_DataOffset]; // ��wav�����������������
	fread(m_InfoData, sizeof(uint8), m_DataOffset, fp);
	fclose(fp);
	m_Offset = m_DataOffset;
}


uint32 CAudioDiff::GetAllSize() const
{
	return m_uAllSize;
}

uint32 CAudioDiff::GetDiffType() const
{
	return 'AUDI';
}


void CAudioDiff::WriteDiffData(void* fp)
{
	FILE* FW = (FILE*)fp;
	FILE* tfp = fopen(AUDIO_TEMP_FILE, "rb");

	fwrite(m_InfoData, sizeof(uint8), m_DataOffset, FW);
	delete[] m_InfoData;
	m_InfoData = NULL;

	int nSize = 0;
	uint8 szBuf[MAX_BUFFER];
	while ( (nSize = fread(szBuf, sizeof(uint8), MAX_BUFFER, tfp)) > 0 )
		fwrite(szBuf, sizeof(uint8), nSize, FW);

	fclose(tfp);
}


