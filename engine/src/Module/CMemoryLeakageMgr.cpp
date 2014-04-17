#include "stdafx.h"
#include "CMemoryLeakageMgr.h"
#include "CMemThreadMgr.h"
#include "ExpHelper.h"
#include "ErrLogHelper.h"
#include "CSqrAllocPoolMgr.h"
#include "UtfCvs.h"
#include "ThreadHelper.h"

#ifndef _WIN32
#include <malloc.h>
#define _countof(array) (sizeof(array)/sizeof(array[0]))
#endif

//��鿴CMemoryLeakageMgr.h�����˵������

CMemoryLeakageMgr::CMemoryLeakageMgr(int nCheckLevel)
{
	m_pThreadMgr = NULL;

	//����������ٶ�new��m_pThreadMgr,��������κ�new�������ᵼ���ڴ�������ָ�����
	if( nCheckLevel>1 )
		m_pThreadMgr = new CMemThreadMgr;

	if( CSqrAllocPoolMgr::Inst().GetAllMemUsage() > 0 )
		GenErr( "Memory leaked before watch begin!" );
	
	m_fdLeakage = NULL;	

	if( !nCheckLevel )
		return;
	
	uint32 uVersion = GetCurrentRevision();

#ifdef _WIN32
	swprintf_s(m_szMemFileName, _countof(m_szMemFileName), L"../../var/mem%uv%u.log", GetCurPID(), uVersion);
#else
	swprintf(m_szMemFileName, _countof(m_szMemFileName), L"../../var/mem%uv%u.log", GetCurPID(), uVersion);
#endif

	m_szMemFileName[ _countof(m_szMemFileName) - 1 ] = L'\0';

#ifdef _WIN32
	m_fdLeakage = _wfopen(m_szMemFileName, L"w");
#else
	char* strMemFileName = U16_to_U8(m_szMemFileName);
	m_fdLeakage = fopen(strMemFileName, "w");
	FreeUtfCvsDest(strMemFileName);
#endif
	if( !m_fdLeakage )
	{
		wostringstream strm;
		strm<<"Open file "<<m_szMemFileName<<" failed.";
		GenErr( U16_to_U8(strm.str().c_str()) );
	}
}

CMemoryLeakageMgr::~CMemoryLeakageMgr()
{
	if ( m_pThreadMgr )
		DumpStack();
	
	DumpLeakageNum();

	delete m_pThreadMgr;
	m_pThreadMgr=NULL;
	
	if (m_fdLeakage)
	{
		fclose( m_fdLeakage );
		m_fdLeakage = NULL;
	}
}

static CMemoryLeakageMgr* gs_Inst=NULL;

void CMemoryLeakageMgr::Init( int nCheckLevel )
{
	gs_Inst = new CMemoryLeakageMgr(nCheckLevel);
}

void CMemoryLeakageMgr::Unit()
{
	delete gs_Inst;
	gs_Inst=NULL;
}

CMemoryLeakageMgr& CMemoryLeakageMgr::Inst()
{
	return *gs_Inst;
}


void CMemoryLeakageMgr::DumpLeakageNum()
{
	if( !m_fdLeakage )
		return;

	if (CSqrAllocPoolMgr::Inst().GetAllMemUsage() == 0)
		return;

	basic_ostringstream<char, char_traits<char>, TMallocAllocator<char> > strm;

	CSqrAllocPoolMgr::Inst().GetMemLeakage(strm);

	if ( strm.str().empty() )
		return;

	WriteMemLog(strm.str().c_str());

#ifndef _WIN32
	fprintf( stderr, "%s\n", strm.str().c_str() );
#else
	MessageBox( NULL , strm.str().c_str() , "�ڴ�й©" , MB_OK|MB_ICONERROR );
#endif
}

void CMemoryLeakageMgr::DumpStack()
{
	if( !m_pThreadMgr )
		return;
	m_pThreadMgr->AddDumpStackJob();
}


#ifndef _WIN32
void CMemoryLeakageMgr::FetchAllSymbol()
{
	if ( !m_pThreadMgr )
		return;

	CMemThreadMgr::Inst()->AddFetchAllSymbolJob();

	/*
	�����������ڵ�fetch symbol��Щ������һ���̴߳���������
	fetch symbol��ȫ����֮ǰ�������ǲ��ܹ������ǵ�GameGas��GameGac��̬���free����
	��Ȼ�ᵼ���Ҳ���symbol���������������������һ��semaphore��
	�����̰߳�fetch all symbol���֮�󣬼�����GameGas��GameGac������ڴ�й¶����
	ȫ���ҵ�symbol֮���������߳�����
	*/
	CMemThreadMgr::Inst()->WaitFetchAllSymbolEnded();
}
#endif


void CMemoryLeakageMgr::WriteMemLog(const char* szLog)
{
	fputs( szLog,m_fdLeakage);
}


