#include "stdafx.h"
#include "CThreadWatcherMap.h"
#include "ExpHelper.h"
#include "ErrLogHelper.h"
#include "TimeHelper.h"
#include "CLock.inl"

CThreadLoopInfo::CThreadLoopInfo(const char* szThreadName,volatile uint64* pTimeVar,uint64 uAllowedDelay,uint32 uAllowedTimes,uint64 uSlowValue)
{
	m_uAllowedTimes				= uAllowedTimes;
	m_uSlowValue				= uSlowValue;
	m_uAllowedDelay				= uAllowedDelay;
	m_pTimeVar					= pTimeVar;
	m_uWakeSignalCounter		= 0;
	m_uLogStackSignalCounter	= 0;
	m_bEnabled					= true;

	strncpy( m_szThreadName , szThreadName, sizeof( m_szThreadName ) );
}

CThreadWatcherMap* CThreadWatcherMap::ms_pThreadMap=NULL;

void CThreadWatcherMap::Init()
{
	if( ms_pThreadMap )
		GenErr("CThreadWatcherMap already initialized.");

	ms_pThreadMap=new CThreadWatcherMap;
}

void CThreadWatcherMap::Unit()
{
	delete ms_pThreadMap;
	ms_pThreadMap=NULL;
}


CThreadWatcherMap* CThreadWatcherMap::Inst()
{
	return ms_pThreadMap;
}

CThreadWatcherMap::CThreadWatcherMap()
{
}

CThreadWatcherMap::~CThreadWatcherMap()
{
}

uint64 CThreadWatcherMap::GetMaxAllowedDelay()
{
	//�˺���ͬʱҲ����������ѭ���ۼƴ�����ʱ��������CThreadWatcher����ʹ��
	return 60 * 1000;
}

void CThreadWatcherMap::AddThread( HTHREAD hThread,const char* szThreadName,volatile uint64* pTimeVar,uint64 uAllowedDelay,uint32 uAllowedTimes,uint64 uSlowValue )
{
	if( uAllowedDelay > GetMaxAllowedDelay() )
	{
		if( uAllowedTimes != 0 )
		{
			ostringstream strm;
			strm<<"��ѭ���ж�ʱ�䳬��"<<GetMaxAllowedDelay()<<"���룬������ѭ���Ĵ���ֻ����0�Ρ�";
			GenErr(strm.str());
		}
	}

	m_lkVarMap.Lock();
	Ver( m_mapVar.insert( make_pair( hThread,new CThreadLoopInfo(szThreadName,pTimeVar,uAllowedDelay,uAllowedTimes,uSlowValue) ) ).second );
	m_lkVarMap.Unlock();
}

void CThreadWatcherMap::DelThread( HTHREAD hThread )
{
	m_lkVarMap.Lock();

	VarMap_t::iterator it = m_mapVar.find( hThread );
	
	if( it == m_mapVar.end() )
	{
		m_lkVarMap.Unlock();
		GenErr("hThread is not added to the thread watcher.");
	}
	
	delete it->second;

	m_mapVar.erase( it );

	m_lkVarMap.Unlock();
}

void CThreadWatcherMap::EnableWatch(HTHREAD hThread, bool bEnabled)
{
	m_lkVarMap.Lock();

	VarMap_t::iterator it = m_mapVar.find( hThread );

	if( it != m_mapVar.end() )
	{
		CThreadLoopInfo* pInfo = it->second;
		if (bEnabled)
		{
			//���enable������ǿ�Ƹ���ʱ�䣬��ֹ�ͺ�
			*(pInfo->m_pTimeVar) = GetProcessTime();
		}
		pInfo->m_bEnabled = bEnabled;
	}

	m_lkVarMap.Unlock();
}

void CThreadWatcherMap::Lock()
{
	m_lkVarMap.Lock();
}

void CThreadWatcherMap::Unlock()
{
	m_lkVarMap.Unlock();
}

