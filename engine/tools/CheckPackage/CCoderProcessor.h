#pragma once
#include <string>
#include "iresprocessormgr.h"
#include "GraphicBase.h"
#include "CFileFormatMgr.h"
/*********************************************************
// ֻ�ʺ��ռ��ַ������͵��ļ�·��
**********************************************************/
class CCoderProcessor 
	: public CBaseResProcessor
	, public CFileFormatMgr
{
public:
	CCoderProcessor(FourCC ccType, uint16 NewVer,CFileFormatMgr::CHeadCoder* pCoder = NULL);
	virtual ~CCoderProcessor(void);

	virtual void			RegProcessTrunk(FourCC fourcc);
	virtual bool			RunProcess( const TCHAR* szFilename );
	virtual size_t			GetCountCanBeProcess() = 0;
	virtual unsigned long	GetTypeCanBeProcess( size_t nNum ) = 0;

protected:
	virtual void			_AutoDecode(const	CDataChunk& inData, void* handle);
protected:
	set<FourCC>				m_CltType;
	// �ռ������ļ�·����
	set<string>				m_fileName;
	// �������Դ�����·��resbin
	string					m_filePath;	
	// ��processor�ռ��ĵ��ļ�·����
	FourCC					m_fileType;
};