//---------------------------------------------------------------------------
// Comment      : �������޹صĶ�дIni�ļ���ʵ��
//              : ˳���д������ٶ�.
//              : ����ͬһ��ʵ������д���ɴ������
//---------------------------------------------------------------------------
// NOTE         : Use std libc function str*** instead of ms ext function 
//                lstr*** for platform compatibility reason.
//---------------------------------------------------------------------------
#include "stdafx.h"
#include "CIniFile.h"
#include "CPkgFile.h"
#include "ExpHelper.h"
#include "BaseHelper.h"
#include "CodeCvs.h"
//---------------------------------------------------------------------------

#ifdef _WIN32
#pragma warning(disable:4127)
#endif

static bool			IsBlank(char    cChar);
static void			NextLine(char*& lpBuffer);
static string		BuildSection(char*& lpBuffer);
static bool			BuildKey(char*& lpBuffer, map<string, string>& Section);



CIniFile::CIniFile()
{	
}

CIniFile::~CIniFile()
{
	Close();
}

// ����һ���µ� .Ini�ļ���Ϊд(����)��׼��
bool CIniFile::Create(const wchar_t* szFileName)
{
	Ast( NULL != szFileName);
	// �������Ϊ�գ��������.
	Close();

	this->fileName = szFileName;
	return true;
}

// ���ļ���ɨ��һ�飬Ϊ�Ժ�Ķ���������׼��(Get...)
bool CIniFile::Open(const wchar_t* alias, const wchar_t* szFileName)
{
	CPkgFile	File;
	char*	    pFileBuff;
	int32		nSize;
	bool        IsSuccess;

	if( !szFileName || !szFileName[0] )
		return false;

	// comp file name  strcmp
	if( this->fileName == szFileName )
		return true;

	// ���ͷ�
	Close();
	// open Ini file  , ���ڲ��γ�ȫ·��
	if (File.Open(alias, utf16_to_utf8(szFileName).c_str(), true, eRO_File) != eFE_SUCCESS)
		return false;

	// get file size
	if ((nSize = File.Size()) == 0)
		return false;

	// alloc memory
	pFileBuff = new char[nSize+1];

	// read data from file
	if (File.read(pFileBuff, nSize) == false)
	{
		SafeDelAry(pFileBuff);
		return false;
	}

	pFileBuff[nSize] = 0;

	// close the file
	File.Close();

	// save file name
	this->fileName = szFileName;

	// �����ڲ�����
	IsSuccess = Prepare(pFileBuff);
	if( !IsSuccess )
		Close();

	SafeDelAry(pFileBuff);

	return IsSuccess;
}

/*
*	�����ڲ�����
*/
bool CIniFile::Prepare(char* lpBuffer)
{
	char*   lpCursor = lpBuffer;
	char    cCursor = *lpCursor;

	string sSection;
	while( 0 != cCursor)
	{
		if( IsBlank(cCursor))
		{
			lpBuffer++;
			lpCursor = lpBuffer;
			cCursor = *lpCursor;
			continue;
		}
		if( '/' == cCursor)
		{
			NextLine(lpBuffer);
			lpCursor    = lpBuffer;
			cCursor     = *lpCursor;
			continue;
		}
		if( '[' == cCursor )
		{
			lpBuffer++;
			sSection = BuildSection(lpBuffer);
			if( sSection.empty() )
				return false;

			// ���Release Ybtx.exe��Ȼ����? [2/27/2009 xoyojank]
			//this->valueMap.insert(make_pair(sSection, KeyMap()));
			lpCursor    = ++lpBuffer;
			cCursor     = *lpCursor;
			continue;
		}

		//--------read key and value-----------------
		if( !BuildKey(lpBuffer, this->valueMap[sSection]))
			return false;
		lpCursor    = lpBuffer;
		cCursor     = *lpCursor;
		//-------------------------------------------

	}

	return true;  
}

// ��˳�������е����ݱ��浽�����ļ�
bool CIniFile::Save()
{	
	wstring szSavePath = CPkgFile::GetLoadPath(L"");
	return Save(szSavePath + this->fileName);
}

bool CIniFile::Save(const wstring& FileName)
{
#ifdef _WIN32 
	FILE* pFile = _wfopen( FileName.c_str(), L"wb" );
#else
	FILE* pFile = fopen( utf16_to_utf8(FileName).c_str(), "wb" );
#endif

	if( !pFile )
		return false;

	for( SectionMap::iterator itSection = this->valueMap.begin();
		itSection != this->valueMap.end(); ++itSection )
	{
		string sSection = "[" + itSection->first + "]\r\n";
		fwrite((void*)sSection.c_str(), sSection.size(), 1, pFile );
		for( KeyMap::iterator itKey = itSection->second.begin();
			itKey != itSection->second.end(); itKey++ )
		{
			string sKey = itKey->first + " = " + itKey->second + "\r\n";
			fwrite((void*)sKey.c_str(), sKey.size(), 1, pFile );
		}
	}
	fclose( pFile );
	return true;
}

// �ر��ļ�,�ͷ��ڴ�
void CIniFile::Close()
{
	this->fileName.clear();
	this->valueMap.clear();
}

// Comment      :   ���� �ո�(0x20)��tab(0x09), �س�(0x0d)�����з�(0x0a)
//              :   ������һЩ�Ż����紫��ָ�룬һ������������Լ��ٺ�������
bool IsBlank(char    cChar)
{
	if( 0x20 == cChar )
		return true;
	if( 0x09 == cChar )
		return true;
	if( 0x0d == cChar )
		return true;
	if( 0x0a == cChar )
		return true;
	return false;
}

//---------------------------------------------------------------------------
// Function     : ��Bufferָ���Ƶ���һ��, ���紦��ע��ʱ��Ҫ.
// Comment      : �����Bufferָ����һ�����ã��ڲ��޸ĺ��Ӱ���ⲿ.
//              : ������Buffer��βʱ��0�������ص�lpBuffer ָ�� 0
//              : �������н���ʱ
//---------------------------------------------------------------------------
void NextLine(char*& lpBuffer)
{
	Ast(NULL != lpBuffer);

	char    cChar = *lpBuffer;

	while( 0 != cChar )
	{
		if( 0x0d == cChar )
		{
			lpBuffer += 2;
			return;
		}
		cChar = *++lpBuffer;
	}
}

//---------------------------------------------------------------------------
// Function     :  ��ȡSection�����֣�����AddtoSectionList
//---------------------------------------------------------------------------
string BuildSection(char*& lpBuffer)
{
	Ast(NULL != lpBuffer);

	char*   lpCursor = lpBuffer;
	char    cChar = *lpBuffer;

	while( 1 )
	{
		if( 0 == cChar )
			return "";
		if( ']' == cChar)
		{
			*lpBuffer = 0;
			string sSection = lpCursor;
			*lpBuffer = ']';
			return sSection;
		}
		lpBuffer++;
		cChar = *lpBuffer;
	}
}

// �γ�KeyName, ��KeyValue
bool BuildKey(char*& lpBuffer, map<string, string>& sectionMap )
{
	Ast(NULL != lpBuffer);

	char    cChar = *lpBuffer;
	char*   lpCursor = lpBuffer;
	string  sKey;
	string  sValue;
	size_t     nLen;

	// Read Key
	while( 1 )
	{
		if( '=' == cChar)
		{
			*lpBuffer = 0;
			nLen = strlen( lpCursor );
			string sTemp = lpCursor;
			//modified by weck to eat space .
			for( size_t i = sTemp.size() - 1; i > 0 && IsBlank( sTemp[i] ); i-- )
				sTemp[i] = 0;

			sKey = sTemp.c_str();

			*lpBuffer = '=';
			//modified by weck to eat space .
			while ( *lpBuffer == '\t' || *lpBuffer == ' ' || *lpBuffer == '=')
			{
				lpBuffer++;
			}
			lpCursor = lpBuffer;
			cChar = *lpBuffer;
			break;
		}
		if( 0 == cChar )
			return false;
		// modified by weck
		// just linebreak or return indicate thtat the format is incorrect.
		// some people like to add space between key and '=' and value.
		if ( cChar == 0x0d || cChar ==0x0a )
			return false;
		/*
		if( IsBlank(cChar))
		return false;
		*/
		lpBuffer++;
		cChar = *lpBuffer;
	}

	// Read Value
	int Len = 0;
	while( 1 )
	{
		if( ( 0 == cChar ) || (0x0d == cChar) || (0x0a == cChar))
		{
			if(Len==0)
				return false;
			// ���˵��ǿո�
			while(1)
			{
				lpBuffer--;
				if( !IsBlank(*lpBuffer))
					break;
			}
			cChar = *++lpBuffer;

			*lpBuffer = 0;
			sValue = lpCursor;
			*lpBuffer = cChar;
			break;
		}
		++Len;
		lpBuffer++;
		cChar = *lpBuffer;
	}

	sectionMap.insert(make_pair(sKey, sValue));

	return true;
}

