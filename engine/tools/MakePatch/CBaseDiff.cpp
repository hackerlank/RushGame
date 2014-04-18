
#include "stdafx.h"

#include <sys/stat.h>
#include <sys/types.h>

#include "CBaseDiff.h"

#include "FileProcess.h"
#include "StringProcess.h"
#include "AssertTreat.h"



CBaseDiff::CBaseDiff()
	: m_uAllSize(0)
	//, m_szDiff(NULL)
{
}


CBaseDiff::~CBaseDiff()
{
}


// �Ƚ������ļ�
bool CBaseDiff::MakeDiff(const char* szNewFile, const char* szOldFile)
{
	uint32 uNewSize  = 0;
	uint32 uOldSize  = 0;
	uint32 uCurSize  = 0;
	uint32 uClipSize = 0;
	uint32 uSize     = 0;
	uint32 uOffset   = 0;
	uint32 uSame     = 0;
	uint32 uSkip     = 0;
	bool bContinueFlag = false; // true������Buffer���Ӿ�Buffer

	char szNewContent[MAX_BUFFER];
	char szOldContent[MAX_BUFFER];

	FILE* nfp = fopen(szNewFile, "rb");
	uNewSize = GetFileLength( nfp );
	FILE* ofp = fopen(szOldFile, "rb");
	uOldSize = GetFileLength( ofp );
	
	//m_szDiff = new char[uNewSize+8]; // �ٽ�������ļ������С����8byte
	//char* pDiff = m_szDiff;
	
	m_szDiffFile = "MakeYBTXPatchTemp.tmp";
	FILE* pfDiffFile = fopen(m_szDiffFile.c_str(), "wb");
	if (!pfDiffFile)
	{
		printf("can not create temp file %s\n", m_szDiffFile.c_str());
		fclose(nfp);
		fclose(ofp);
		return false;
	}


	fseek( pfDiffFile, 0, SEEK_SET );


	//char* pRetSize = NULL;
	long posRetSize = 0;

	int j = 0;
	int i = 0;
	char szBuf[MIN_SAME_SIZE];
	while ( j < uNewSize && j < uOldSize )
	{
		if (uOldSize - j < MAX_BUFFER)
			uCurSize=fread(szNewContent, sizeof(char), uOldSize - j, nfp);
		else
			uCurSize=fread(szNewContent, sizeof(char), MAX_BUFFER, nfp);
		fread(szOldContent, sizeof(char), uCurSize, ofp);

		for ( i=0; i<uCurSize; ++i, ++uSkip )
		{
			if ( ! bContinueFlag ) // countine��ǩ����Ϊtrue����ԭ��8�ֽ���ͬ���û�����
			{
				if (szNewContent[i] == szOldContent[i]) // �Թ���ͬ����
					continue;

				//memcpy(pDiff, (char*)&uSkip, sizeof(uint32));
				//pDiff += sizeof(uint32);
				fwrite( &uSkip, sizeof(uint32), 1, pfDiffFile );


				//pRetSize = pDiff; // ��¼ClipSize��ַ���Ժ����
				posRetSize = ftell(pfDiffFile);

				//memcpy(pDiff, (char*)&uClipSize, sizeof(uint32));
				//pDiff += sizeof(uint32);
				fwrite(&uClipSize, sizeof(uint32), 1, pfDiffFile);

				uSame = 0;
				bContinueFlag = true;
			}

			// ��¼��ͬ���֣������8�ֽ�������ͬ������Ϊ��ǰclip����
			while( uSame < MIN_SAME_SIZE && i < uCurSize )
			{
				szBuf[uSame] = szNewContent[i];
				++uSame;

				// ���¾��ļ��в�ͬʱ����¼��ǰclip��һ����(8�ֽ�����)
				if( szNewContent[i] != szOldContent[i] )
				{
					//memcpy(pDiff, szBuf, uSame);
					//pDiff += uSame;
					fwrite(szBuf, 1, uSame, pfDiffFile);

					uSame = 0;
				}

				++i;
				++uClipSize;
			}

			// 8�ֽ���ͬ�����¿�ʼ�ۼƲ�ͬ����
			if ( uSame >= MIN_SAME_SIZE || j + i >= uNewSize )
			{
				uClipSize -= uSame;
				//*( (uint32*)pRetSize ) = uClipSize;				
				fseek(pfDiffFile, posRetSize, SEEK_SET);
				fwrite(&uClipSize, sizeof(uClipSize), 1, pfDiffFile);
				fseek(pfDiffFile, 0, SEEK_END);


				uSize += uClipSize + sizeof(uint32) * 2;
				--i;           // ��Ϊ��������ѭ����һ�ξͻ����++i����
				uSkip = uSame; // �ٽ���������ļ��Ⱦ��ļ������һ��uSkip����С��8
				--uSkip;       // ��Ϊi������һλ��skipҲ��Ҫ��֮����һλ
				uClipSize = 0;
				bContinueFlag = false;
			}
		}

		j += uCurSize;
	}

	// ���Ŀ���ļ���Դ�ļ���ʣ������ݾ���ΪPatch���ݴ���
	if ( uNewSize > uOldSize )
	{
		uint32 uLeaveSize = uNewSize - uOldSize;
		char* szLeaveBuf = new char[uLeaveSize];

		fread(szLeaveBuf, sizeof(char), uLeaveSize, nfp);

		// �������պ�8λ��ͬ��clipͷ��û�д���
		if (uClipSize == 0)
		{
			uClipSize += uLeaveSize;
			//memcpy(pDiff, (char*)&uSkip, sizeof(uint32));
			//pDiff += sizeof(uint32);
			fwrite(&uSkip, sizeof(uint32), 1, pfDiffFile);


			//memcpy(pDiff, (char*)&uClipSize, sizeof(uint32));
			//pDiff += sizeof(uint32);
			fwrite(&uClipSize, sizeof(uint32), 1, pfDiffFile);
		}
		else
		{
			if (uSame != 0)
			{
				//memcpy(pDiff, szBuf, uSame);
				//pDiff += uSame;
				fwrite(szBuf, 1, uSame, pfDiffFile);
				uSame = 0;
			}

			uClipSize += uLeaveSize;
			//*( (uint32*)pRetSize ) = uClipSize;
			fseek(pfDiffFile, posRetSize, SEEK_SET);
			fwrite(&uClipSize, sizeof(uClipSize), 1, pfDiffFile);
			fseek(pfDiffFile, 0, SEEK_END);
		}

		// ���ļ��Ⱦ��ļ�������ֲ����(������ASCII 0���)��ֱ�Ӽ�¼ԭֵ
		//memcpy(pDiff, szLeaveBuf, uLeaveSize);
		fwrite(szLeaveBuf, 1, uLeaveSize, pfDiffFile);

		uSize += uClipSize + sizeof(uint32) * 2;

		delete[] szLeaveBuf;
	}

	fclose(nfp);
	fclose(ofp);

	fclose(pfDiffFile);

	m_uAllSize = uSize;

	if (uSize==0) // �¾��ļ�������ͬ������Patch
	{

		remove(m_szDiffFile.c_str());
		m_szDiffFile.clear();

		//delete[] m_szDiff;
		//m_szDiff = NULL;
		
		if (uNewSize != uOldSize)
		{
			return true;
		}

		return false;
	}

	return true;
}

uint32 CBaseDiff::GetAllSize() const
{
	return m_uAllSize;
}

uint32 CBaseDiff::GetDiffType() const
{
	return 'BASE';
}

void CBaseDiff::WriteDiffData(void* fp)
{
	if (m_szDiffFile.empty())
	{
		printf("the diffFile temp file not exist!\n");
		return;
	}
	FILE* fDiffFile = fopen(m_szDiffFile.c_str(), "rb");
	if (!fDiffFile)
	{
		printf( "can not open file: %s\n", m_szDiffFile.c_str() );
		return;
	}

	const int BUFF_LEN = 64*1024; // 64K
	char* pTemp = new char[BUFF_LEN];
	if (!pTemp)
	{
		printf("can not new memory!\n");
		fclose(fDiffFile);
		return;
	}

	//fwrite(m_szDiff, sizeof(char), m_uAllSize, (FILE*)fp);
	size_t Read = 0;
	while (Read = fread(pTemp, sizeof(char), BUFF_LEN, fDiffFile))
	{
		fwrite(pTemp, sizeof(char), Read, (FILE*)fp);		
	}
	if (pTemp)
	{
		delete[] pTemp;
		pTemp = NULL;
	}
	fclose(fDiffFile);


	//if ( m_szDiff != NULL )
	//{
	//	delete[] m_szDiff;
	//	m_szDiff = NULL;
	//}

	if (!m_szDiffFile.empty())
	{
		remove(m_szDiffFile.c_str());
		m_szDiffFile.clear();
	} 
}

