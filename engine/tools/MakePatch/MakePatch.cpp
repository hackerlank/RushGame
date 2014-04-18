// MakePatch.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <sys/stat.h>
#include <sys/types.h>

#include "bzip2/bzlib.h"

#include "MakePatch.h"
#include "StringProcess.h"
#include "FileProcess.h"

#include "CBaseDiff.h"
#include "CAudioDiff.h"

#include "VersionManager.h"
#include "CMd5Algorithm.h"



#define  RELATIVE_DIR_INI  "/etc/gac/update.ini"

#define  MIN_SAME_SIZE  8
#define  BIN_DIFF_TRUNK_LEVEL  2
#define  PATH_LEN       4096



// �����ļ����Ⱥ�˳��
static int GetLevel(const char* FileName)
{
	char* ptr = const_cast<char*>(FileName);
	int Len = strlen(FileName);

	int i = 0;
	for (i=Len; i>=0; --i)
	{
		if (ptr[i] == '.')
		{
			ptr += i + 1;

			if ( ! stricmp(ptr, "xsb") )
				return 1;

			break;
		}
	}

	return 0; // ���紦��
}

// �������Diff�Ĵ�����
static int GetDiff(const char* FileName)
{
	char* ptr = const_cast<char*>(FileName);
	int Len = strlen(FileName);

	int i = 0;
	for (i=Len; i>=0; --i)
	{
		if (ptr[i] == '.')
		{
			ptr += i + 1;

			if ( ! stricmp(ptr, "xwb") )
				return 1;

			break;
		}
	}

	return 0; // Ĭ�ϴ���
}


// MakePackage Method

MakePatch::MakePatch()
	: m_nMode(0)
	, m_bCompress(true)
	, m_uFileCount(0)
	, m_pVersion(new ReleaseVersion)
{
	// Base ProcessorҪ��������ԭ����������ʽ������ҪһЩ��Base Processor�����Ԫ�����ļ�
	// ��Щ�ļ�����������
	m_szDiff[0] = new CBaseDiff;
	m_szDiff[1] = new CAudioDiff;

	fclose(fopen(TEMP_DATA_FILE, "wb"));
}

MakePatch::~MakePatch()
{
}


// ��ð汾�ŵ�string�������ȫ�ֵİ汾��Ϣ�ṹ
void MakePatch::GetPatchVersion(char* szPackDir)
{
	// ��ȡ��ǰĿ¼
	char szIniFileDir[PATH_LEN];
	memset(szIniFileDir, 0, PATH_LEN);

	EX_ReplaceChar(szPackDir, '\\', '/');
	strcpy(szIniFileDir, szPackDir);
	strcat_s(szIniFileDir, RELATIVE_DIR_INI);

	char szBuf[24];
	string strFileName(szIniFileDir);
	GetPrivateProfileString( "Setting", "Version", "", szBuf, sizeof(szBuf), strFileName.c_str() );

	int32 uSequelVersion = 0;
	int32 uBranchVersion = 0;
	int32 uRevision = 0;
	int32 uSVersion = 0;
	int ret = 0;
	ret = sscanf(szBuf, "%d.%d.%d.%d", &uSequelVersion, &uBranchVersion, &uRevision, &uSVersion );
	if ( ret != 4 )
		printf("���ذ汾�Ÿ�ʽ����ȷ������Update.ini�����ļ�");

	m_pVersion->uSequelVersion = uSequelVersion;
	m_pVersion->uBranchVersion = uBranchVersion;
	m_pVersion->uRevision = uRevision;
	m_pVersion->uSVersion = uSVersion;

	m_strVersion.assign(szBuf);
}



void MakePatch::SetPatchName(char* str)
{
	m_strPatchName.assign(str);
	m_strPatchName += "/patch_";
	m_strPatchName += m_strVersion;
	m_strPatchName += ".ybtx";
}



// Ѱ�ҷ����ı���ļ�
int MakePatch::GetPatchFile(string sNewDirName, string sOldDirName)
{
	HANDLE hFind;
	WIN32_FIND_DATA ffd;

	string szSearch = sNewDirName + "/*.*";

	ZeroMemory( &ffd, sizeof(WIN32_FIND_DATA) );
	hFind = FindFirstFile( szSearch.c_str(), &ffd );

	if( hFind == INVALID_HANDLE_VALUE )
		return 0;

	while( FindNextFile( hFind, &ffd ) )
	{
		if( _stricmp(ffd.cFileName, ".") == 0 || _stricmp(ffd.cFileName, "..") == 0 )
			continue;

		string sNewFileName = sNewDirName + "/" + ffd.cFileName;
		string sOldFileName = sOldDirName + "/" + ffd.cFileName;

		// ���Կͻ��˰汾Ψһ��ʶ�ļ�
		if ( strcmpi("WxSDLRevision.log", ffd.cFileName) == 0 )
			continue;

		if( ffd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY ) // Ŀ¼
		{
			GetPatchFile( sNewFileName, sOldFileName );
		}
		else // ��¼�ļ���Ϣ
		{
			struct _stat stat_new;
			if( _stat( sNewFileName.c_str(), &stat_new ) )
			{
				printf("�޷���ȡ %s �ļ���Ϣ\n", sNewFileName);
				exit(1);
			}

			struct _stat stat_old;
			if( _stat( sOldFileName.c_str(), &stat_old ) ) // �����ļ�
				m_szChangeFiles[GetLevel(sNewFileName.c_str())][sNewFileName] = "";
			else if( ( stat_new.st_mtime != stat_old.st_mtime ) || ( stat_new.st_size != stat_old.st_size ) )
				m_szChangeFiles[GetLevel(sNewFileName.c_str())][sNewFileName] = sOldFileName;
		}
	}

	FindClose( hFind );

	return 1;
}


void MakePatch::FetchPatchData()
{
	int nCount = 0;
	int i = 0 ;
	string OldUpdatePath;
	string NewUpdatePath;
	for (i=0; i<LEVEL_NUMBER; ++i)
	{
		StringMap mapChangeFile = m_szChangeFiles[i];
		for ( StringMap::iterator it = mapChangeFile.begin(); it != mapChangeFile.end(); ++it )
		{
			// ��ʱ���Update.ini�����£�Ϊ��ֹ��������������ǰ��update.ini�Ѹ��£��жϸ��µ����
			if ( strstr( it->first.c_str(), "Update.ini" ) != NULL)
			{
				OldUpdatePath = it->first;
				NewUpdatePath = it->second;
			}
			else
			{
				Execute(it->first.c_str(), it->second.c_str(), m_strVersion.c_str());
			}
		}
	}

	Execute(OldUpdatePath.c_str(), NewUpdatePath.c_str(), m_strVersion.c_str());
}


void MakePatch::GeneratePatch()
{
	// ��ȡ�洢��temp�ļ��е�����(���Ҫɾ��temp��ʱ�ļ�)
	FILE* fp = fopen(TEMP_DATA_FILE, "rb");
	uint32 uSrcSize = GetFileLength(fp);
	char* szSource = new char[uSrcSize];
	fread(szSource, sizeof(char), uSrcSize, fp);
	fclose(fp);

	char* szDest = NULL;
	uint32 uDestSize = 0;
	if (m_bCompress)
	{
		uDestSize = (int)(uSrcSize * 1.01 + 640);
		szDest = new char[uDestSize];

		if ( BZ2_bzBuffToBuffCompress(szDest, &uDestSize, szSource, uSrcSize, 9, 0, 30) != BZ_OK )
			printf("have some error in compress!\n");;
	}
	else
	{
		uDestSize = uSrcSize;
		szDest = szSource;
	}

	// д��ͷ�ļ���ʶ����
	PatchHead head;
	if ( m_bCompress )
		head.uFlag = 'YBBZ'; // bzip2ѹ��
	else
		head.uFlag = 'YBTX'; // ��ѹ��

	head.uVersion.uReserve       = 0;
	head.uVersion.uSequelVersion = m_pVersion->uSequelVersion;
	head.uVersion.uBranchVersion = m_pVersion->uBranchVersion;
	head.uVersion.uRevision      = m_pVersion->uRevision;
	head.uVersion.uSVersion		 = m_pVersion->uSVersion;

	head.uNonCompressSize = uSrcSize;
	head.uFileCount = m_uFileCount;

	// md5���ɼ���¼
	CMd5Algorithm digester;
	digester.Begin();
	digester.Append((uint8*)szDest, uDestSize);
	digester.End(head.md5Code);

	fp = fopen(m_strPatchName.c_str(), "wb");
	fwrite(&head, sizeof(PatchHead), 1, fp); // ��ѹʹ�ù��߱�ʶ

	// д����������
	fwrite(szDest, sizeof(char), uDestSize, fp);
	fclose(fp);

	delete[] szDest;

	remove(TEMP_DATA_FILE);
}

void MakePatch::SetType(char* type)
{
	string strType;
	string strBuf(type);
	int Len = strBuf.length();
	int Offset = 0;
	int i = 0;
	for ( i=0; i<Len; ++i )
	{
		if ( strBuf.at(i) == '|' )
		{
			m_setType.insert( strBuf.substr(Offset, i-Offset) );
			Offset = i + 1;
		}
	}

	m_setType.insert( strBuf.substr(Offset, i-Offset) );
}


void MakePatch::SetCompress(bool flag)
{
	m_bCompress = flag;
}

bool MakePatch::GetCompress()
{
	return m_bCompress;
}

void MakePatch::Execute(const char* szNewFile, const char* szOldFile, const char* VersionStr)
{
	char Buf[MAX_PATH_LEN];
	memset(Buf, 0, MAX_PATH_LEN);
	EX_GetRelativePath(Buf, szNewFile, VersionStr); // ��ȡ�ļ����·��
	string RelativePath(Buf);

	int nResult = 0;
	uint32 size = 0;

	FILE* FH = fopen(szNewFile, "rb");
	FILE* FW = fopen(TEMP_DATA_FILE, "ab");

	size = GetFileLength(FH);

	uint32 uFlag = 0;
	uint32 uFileLen = 0;
	uint32 uDiffType = 0;
	uint32 uDiffLen = 0;
	uint32 uPathLen = 0;
	if ( strcmp(szOldFile, "") == 0 ) // �����ļ�
	{
		printf("Add: %s\n", szNewFile);

		uFlag    = 'ADDF';                    // Patch���ͱ�ʶ
		uFileLen = size;                      // ���ļ����ݳ���
		uDiffLen = size;                      // Diff���ݳ���(�����ļ�����)
		uPathLen = RelativePath.size() + 1;   // ���·������

		fwrite((char*)&uFlag,     sizeof(uint32), 1, FW);
		fwrite((char*)&uFileLen,  sizeof(uint32), 1, FW);
		fwrite((char*)&uDiffType, sizeof(uint32), 1, FW);
		fwrite((char*)&uDiffLen,  sizeof(uint32), 1, FW);
		fwrite((char*)&uPathLen,  sizeof(uint32), 1, FW);

		char* szFileContent = new char[uFileLen];  // �ļ�����
		fread(szFileContent, uFileLen, 1, FH);

		fwrite(RelativePath.c_str(), uPathLen, 1, FW);
		fwrite(szFileContent, uDiffLen, 1, FW);

		delete[] szFileContent;

		++m_uFileCount;
	}
	else
	{
		int DiffIdx = GetDiff(szNewFile);
		if ( m_szDiff[DiffIdx]->MakeDiff(szNewFile, szOldFile) )
		{
			uFlag    = 'MODI';                            // Patch���ͱ�ʶ
			uDiffType = m_szDiff[DiffIdx]->GetDiffType(); // Patch Diff�������ͱ�ʶ
			uFileLen = size;                              // ���ļ����ݳ���
			uDiffLen = m_szDiff[DiffIdx]->GetAllSize();   // Diff���ݳ���
			uPathLen = RelativePath.size() + 1;           // ���·������

			fwrite((char*)&uFlag,     sizeof(uint32), 1, FW);
			fwrite((char*)&uFileLen,  sizeof(uint32), 1, FW);
			fwrite((char*)&uDiffType, sizeof(uint32), 1, FW);
			fwrite((char*)&uDiffLen,  sizeof(uint32), 1, FW);
			fwrite((char*)&uPathLen,  sizeof(uint32), 1, FW);

			fwrite(RelativePath.c_str(), uPathLen, 1, FW);
			m_szDiff[DiffIdx]->WriteDiffData(FW);

			printf("Modify: %s,  Size: %d\n", szNewFile, uDiffLen);

			++m_uFileCount;
		}
	}

	fclose(FH);
	fclose(FW);
}


void MakePatch::TestDecompress()
{
	//// ��ȡ��ѹ���֣���ʽ����ʱȥ���ò���
	//FH = fopen(sPatchName.c_str(), "rb");

	//fseek(FH, 0, SEEK_END);
	//size = ftell(FH);

	//uint32 NewSourceLen = size;
	//uint32 NewDestLen = CompressSize;
	//char *NewSource = new char[NewSourceLen];
	//char *NewDest = new char[NewDestLen];

	//fseek(FH, 12, SEEK_SET);
	//fread(NewSource, NewSourceLen, 1, FH);
	//fclose(FH);

	//if (nResult = BZ2_bzBuffToBuffDecompress(NewDest, &NewDestLen, NewSource, NewSourceLen, 0, 0) != BZ_OK)
	//	printf("have some error in decompress!\n");

	//string sTestFile(argv[3]);
	//sTestFile += "/test.data";
	//FH = fopen(sTestFile.c_str(), "wb");
	//fwrite((char*)&CompressFlag, sizeof(CompressFlag), 1, FH); // ��ѹʹ�ù��߱�ʶ
	//fwrite((char*)&PatchVersion, sizeof(PatchVersion), 1, FH); // ��ʶ�汾
	//fwrite((char*)&CompressSize, sizeof(CompressSize), 1, FH); // ��ѹʱ���������С
	//fwrite(NewDest, NewDestLen, 1, FH); // ��ѹʱ���������С
	//fclose(FH);
}

