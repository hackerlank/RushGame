#include "stdafx.h"
#include "CScriptDebugerMgr.h"
#include "CScriptDebuger.h"
#include "ThreadHelper.h"
#include "CScriptData.h"
#include "ErrLogHelper.h"
#include "CodeCvs.h"
#ifndef _WIN32
#	define _countof(array)	(sizeof(array)/sizeof(array[0]))
#endif

namespace sqr
{
	enum EDebugerType
	{
		eDebugerRunning = 0,
		eDebugerSuspended = 1,
		eDebugerNone = 2,
	};

	class CScriptDbContainer
	{
	public:
		CScriptDbContainer();
		~CScriptDbContainer();
		
		list<CScriptDbContainer*>::iterator*	m_pItContainer;
		HSEMAPHORE			m_smContainer;
		EDebugerType		m_Type;
		uint32				m_uId;
	};
}

CScriptDbContainer::CScriptDbContainer()
: m_pItContainer(NULL)
{
	CreateSemaphore(&m_smContainer, 0,INT_MAX);
}

CScriptDbContainer::~CScriptDbContainer()
{
	DestroySemaphore(&m_smContainer);
}

//--------------------------------------------------------------------

CScriptDebugerMgr::CScriptDebugerMgr()
: m_bRunning(false)
, m_bDbFileLoad(false)
, m_bHasBreak(false)
{
	CreateLock(&m_lock);
	CreateLock(&m_BreakLock);
}

CScriptDebugerMgr::~CScriptDebugerMgr()
{
	DestroyLock(&m_BreakLock);
	DestroyLock(&m_lock);
}

CScriptDebugerMgr& CScriptDebugerMgr::Inst()
{
	static CScriptDebugerMgr ls_Inst;
	return ls_Inst;
}

CScriptDbContainer* CScriptDebugerMgr::GetContainer(CScriptDebuger* pDebuger)
{
	return pDebuger->m_pDbContainer;
}

void CScriptDebugerMgr::AddDebuger(CScriptDebuger* pDebuger)
{
	if(GetContainer(pDebuger))
		GenErr("��debuger��container�Ѿ����ڣ������ٴ����");

	CScriptDbContainer* pContainer = new CScriptDbContainer;
	pContainer->m_Type = eDebugerNone;
	
	Lock(&m_lock);
	static uint32 uId = 0;
	pContainer->m_uId = uId;
	m_mapId2Container.insert(make_pair(uId, pContainer));
	++uId;
	Unlock(&m_lock);

	pDebuger->m_pDbContainer = pContainer;
}

void CScriptDebugerMgr::RemoveDebuger(CScriptDebuger* pDebuger)
{
	CScriptDbContainer* pContainer = GetContainer(pDebuger);
	if(!pContainer)
		GenErr("��debuger��container�Ѿ�ɾ���������ٴ�ɾ��");

	uint32 uId = pContainer->m_uId;

	Lock(&m_lock);
	m_mapId2Container.erase(uId);
	Unlock(&m_lock);

	Ast(!pContainer->m_pItContainer);
	
	delete pContainer;
	pDebuger->m_pDbContainer = NULL;
}

void CScriptDebugerMgr::WaitForDebuger(CScriptDebuger* pDebuger)
{
	bool bWaited = false;
	Lock(&m_lock);
	if(!m_bRunning)
	{
		m_bRunning = true;
		bWaited = true;
	}
	
	CScriptDbContainer* pContainer = GetContainer(pDebuger);

	//����ȵ��ˣ��Ͱ�debuger��type����Ϊrunning��ֱ���˳�
	//����typeΪsuspended��Ȼ�����һ���ȴ��������棬
	if(bWaited)
	{
		pContainer->m_Type = eDebugerRunning;
		Unlock(&m_lock);
		return;
	}
	else
	{
		pContainer->m_Type = eDebugerSuspended;
	}

	Ast(!pContainer->m_pItContainer);
	pContainer->m_pItContainer = new list<CScriptDbContainer*>::iterator;
	
	m_lstWaitingContainer.push_back(pContainer);
	*pContainer->m_pItContainer = --m_lstWaitingContainer.end();

	Unlock(&m_lock);

	GetSemaphore(&pContainer->m_smContainer);

	Lock(&m_lock);
	pContainer->m_Type = eDebugerRunning;
	Unlock(&m_lock);
}

void CScriptDebugerMgr::CancelBlock(CScriptDebuger* pDebuger)
{
	CScriptDbContainer* pContainer = GetContainer(pDebuger);
	
	Lock(&m_lock);
	pContainer->m_Type = eDebugerNone;
	//���Ϊ�գ������û�еȴ���Ҫ���е�db��
	if(m_lstWaitingContainer.empty())
	{
		m_bRunning = false;
		Unlock(&m_lock);
		return;
	}

	CScriptDbContainer* pOtherContainer= m_lstWaitingContainer.front();
	m_lstWaitingContainer.pop_front();

	Ast(pOtherContainer->m_pItContainer);
	delete pOtherContainer->m_pItContainer;
	pOtherContainer->m_pItContainer = NULL;
	
	pOtherContainer->m_Type = eDebugerRunning;

	Unlock(&m_lock);

	PutSemaphore(&pOtherContainer->m_smContainer);
}

void CScriptDebugerMgr::PrintDebugerInfo()
{
	Lock(&m_lock);
	printf("ID\t\t\tDebuger State\n");

	const char* szType[3] = {"��������������", "�������ѱ�����", "������û������"};

	map<uint32, CScriptDbContainer*>::iterator iter = m_mapId2Container.begin();
	while(iter != m_mapId2Container.end())
	{
		CScriptDbContainer* pContainer = iter->second;
		printf("%d\t\t\t%s\n", pContainer->m_uId, szType[(uint32)pContainer->m_Type]);
		++iter;
	}
	Unlock(&m_lock);
}

void CScriptDebugerMgr::SwitchDebuger(uint32 uId, CScriptDebuger* pDebuger)
{
	CScriptDbContainer* pContainer = GetContainer(pDebuger);
	//����л����߳�id�������ǵ�ǰ�ģ�ʲô��������ֱ���˳�
	if(pContainer->m_uId == uId)
		return;

	CScriptDbContainer* pOtContainer = NULL;
	
	Lock(&m_lock);
	map<uint32, CScriptDbContainer*>::iterator iter = m_mapId2Container.find(uId);
	if(iter == m_mapId2Container.end())
	{
		printf("������߳�id����������ȷ��ֵ\n");
		Unlock(&m_lock);
		return;
	}

	pOtContainer = iter->second;
	
	if(pOtContainer->m_Type == eDebugerNone)
	{
		printf("�л����߳̽ű���������û������\n");
		Unlock(&m_lock);
		return;
	}

	pContainer->m_Type = eDebugerSuspended;

	//�ѿ�ʼ���е�db����ȴ����У�Ȼ�����Ҫ���ѵ�db�ӵȴ���������ɾ��
	Ast(!pContainer->m_pItContainer);
	pContainer->m_pItContainer = new list<CScriptDbContainer*>::iterator;
	m_lstWaitingContainer.push_back(pContainer);
	*pContainer->m_pItContainer = --m_lstWaitingContainer.end();

	m_lstWaitingContainer.erase(*pOtContainer->m_pItContainer);
	Ast(pOtContainer->m_pItContainer);
	delete pOtContainer->m_pItContainer;
	pOtContainer->m_pItContainer = NULL;

	pOtContainer->m_Type = eDebugerRunning;
	
	Unlock(&m_lock);

	//����������db�����������������Լ���
	PutSemaphore(&pOtContainer->m_smContainer);

	GetSemaphore(&pContainer->m_smContainer);
}

//---------------------------------------------------------------
//���洦��break

namespace sqr
{
	class CScriptBreakInfo
	{
	public:
		CScriptBreakInfo(const string& strBreak, const string& strSource, uint32 uLine);
		string  m_strBreak;
		string	m_strSource;
		uint32	m_uLine;

		static bool ParseBreak(const string& strBreak, string& strSource, uint32& uLine);
	};
}

CScriptBreakInfo::CScriptBreakInfo(const string& strBreak, const string& strSource, uint32 uLine)
: m_strBreak(strBreak)
, m_strSource(strSource)
, m_uLine(uLine)
{

}

bool CScriptBreakInfo::ParseBreak(const string& strBreak, string& strSource, uint32& uLine)
{
	string::size_type uPos = strBreak.find(':');
	if(uPos == string::npos)
		return false;

	strSource = strBreak.substr(0, uPos);
	string strLine = strBreak.substr(uPos + 1);

	uLine = (uint32)atoi(strLine.c_str());
	return true;
}

CScriptBreakInfo* CScriptDebugerMgr::GetBreak(const string& strBreak)
{
	Lock(&m_BreakLock);
	MapStr2Break_t::const_iterator iter = m_mapBreak.find(strBreak);
	if(iter == m_mapBreak.end())
	{
		Unlock(&m_BreakLock);
		return NULL;
	}
	Unlock(&m_BreakLock);
	return iter->second;
}

uint32 CScriptDebugerMgr::ms_uId = 0;

//�����ʽ�ĸ�ʽΪ�ļ���:�к� a.lua:1
void CScriptDebugerMgr::AddBreak(const char *szBreak)
{
	string strBreak(szBreak);
	string strSource;
	uint32 uLine;

	if(!CScriptBreakInfo::ParseBreak(strBreak, strSource, uLine))
	{
		printf("�����break��ʽ����ȷ�������� (�ļ���:�к�) �ĸ�ʽ\n");
		return;
	}

	CScriptBreakInfo* pBreak = GetBreak(strBreak);
	if(pBreak)
		return;

	Lock(&m_BreakLock);
	
	m_bHasBreak = true;
	
	pBreak = new CScriptBreakInfo(strBreak, strSource, uLine);
	m_mapBreak.insert(make_pair(strBreak, pBreak));
	m_mapId2Break.insert(make_pair(ms_uId, pBreak));
	ms_uId++;
	Unlock(&m_BreakLock);

	//ÿ����һ��break�����Ǿ�Ҫд���ļ�����ֹ����ͻȻ������û�б���ϵ���Ϣ
	SaveBreaks();
}

void CScriptDebugerMgr::DelBreak(uint32 uBreak)
{
	Lock(&m_BreakLock);

	MapId2Break_t::iterator iter = m_mapId2Break.find(uBreak);
	if(iter == m_mapId2Break.end())
	{
		Unlock(&m_BreakLock);
		printf("��������ȷ��break���\n");
		return;		
	}

	CScriptBreakInfo* pBreak = iter->second;
	m_mapId2Break.erase(uBreak);
	m_mapBreak.erase(pBreak->m_strSource);
	delete pBreak;
	
	if(m_mapId2Break.empty())
		m_bHasBreak = false;

	Unlock(&m_BreakLock);

	SaveBreaks();
}

void CScriptDebugerMgr::DelBreaks()
{
	Lock(&m_BreakLock);

	m_mapId2Break.clear();

	for(MapStr2Break_t::iterator iter = m_mapBreak.begin(); iter != m_mapBreak.end(); ++iter)
	{
		delete iter->second;
	}

	m_mapBreak.clear();

	m_bHasBreak = false;

	Unlock(&m_BreakLock);

	SaveBreaks();
}

void CScriptDebugerMgr::ListBreaks()
{
	Lock(&m_BreakLock);

	if(m_mapId2Break.empty())
	{
		Unlock(&m_BreakLock);
		return;	
	}

	printf("���\t\t\t�ϵ�λ��\n");
	MapId2Break_t::iterator iter = m_mapId2Break.begin();
	while(iter != m_mapId2Break.end())
	{
		printf("%d \t\t\t%s\n", iter->first, iter->second->m_strBreak.c_str());
		++iter;
	}

	Unlock(&m_BreakLock);
}

void CScriptDebugerMgr::CheckBreak(lua_State* pState, lua_Debug* pDebug)
{
	//���һ��break��û�У���ô���ǾͲ������
	if(!m_bHasBreak)
		return;

	if(pDebug->event != LUA_HOOKLINE)
		return;

	Lock(&m_BreakLock);
	if(m_mapBreak.empty())
	{
		Unlock(&m_BreakLock);
		return;
	}
	Unlock(&m_BreakLock);

	lua_getinfo(pState, "S", pDebug);

	const char* szSource = pDebug->source;
	if(*szSource != '@')
		return;

	lua_getinfo(pState, "l", pDebug);
	uint32 uLine = (uint32)pDebug->currentline;

	string strSource(szSource);

	//����pdebug�õ���source��������·��������ֻ��Ҫ�����ļ����־�����
	//�����е�"\\"�滻��"/"��ͳһ����
	replace(strSource.begin(), strSource.end(), '\\', '/');
	string::size_type uPos = strSource.find_last_of('/');
	strSource = strSource.substr(uPos + 1);
	char buf[10];
	sprintf(buf, ":%u", uLine);
	strSource.append(buf);

	CScriptBreakInfo* pBreak = GetBreak(strSource);
	if(!pBreak)
		return;

	//�ҵ��ˣ��ͽ���ϵ��˹�
	CScriptDebuger* pDebuger=_S( pState ).GetData()->m_pDebuger;
	
	pDebuger->Debug(pState);
}

namespace sqr
{
	int SaveDebugBreaks()
	{
		CScriptDebugerMgr::Inst().SaveBreaks();
		return 0;
	}

	int LoadDebugBreaks()
	{
		wstring strFile;
		strFile.append(GetLogPath()).append(L"/ScriptBreak.log");

		CScriptDebugerMgr::Inst().LoadBreaks(strFile.c_str());
		return 0;
	}
}

void CScriptDebugerMgr::LoadBreaks(const wchar_t* szFileName)
{
	if(m_bDbFileLoad)
		return;

	wcsncpy(m_szBreakFile, szFileName, _countof(m_szBreakFile));
	m_szBreakFile[_countof(m_szBreakFile) - 1] = L'\0';

#ifdef _WIN32
	FILE* fp = _wfopen(m_szBreakFile, L"r");
#else	
	FILE* fp = fopen( utf16_to_utf8(m_szBreakFile).c_str(), "r" );
#endif

	if(!fp)
		return;

	while(true)
	{
		char buf[56];

		if (fgets(buf, sizeof(buf), fp) == NULL)
			break;

		for(int i = 0; i < sizeof(buf); i++)
		{
			if(buf[i] == '\n')
			{
				if(buf[i - 1] == '\r')
				{
					buf[i - 1] = '\0';
					break;
				}
				else
				{	
					buf[i] = '\0';
					break;
				}
			}
		}

		AddBreak(buf);		
	}

	fclose(fp);

	m_bDbFileLoad = true;
}

void CScriptDebugerMgr::SaveBreaks()
{
	if(!m_bDbFileLoad)
		return;

	if(m_mapBreak.empty())
		return;

#ifdef _WIN32
	FILE* fp = _wfopen(m_szBreakFile, L"w");
#else
	FILE* fp = fopen( utf16_to_utf8(m_szBreakFile).c_str(), "w" );
#endif

	MapStr2Break_t::iterator iter = m_mapBreak.begin();
	while(iter != m_mapBreak.end())
	{
		fprintf(fp, "%s\n", iter->first.c_str());
		++iter;
	}

	fflush(fp);
	fclose(fp);

	m_bDbFileLoad = false;
}
