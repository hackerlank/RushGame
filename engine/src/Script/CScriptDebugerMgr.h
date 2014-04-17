#pragma once 
#include "ThreadTypes.h"

struct lua_Debug;
struct lua_State;

namespace sqr
{

	class CScriptDebuger;

	class CScriptDebgerMgrData;

	class CScriptDbContainer;

	class CScriptBreakInfo;

	class CScriptDebugerMgr
	{
		friend class CScriptDebuger;
		
		friend int SaveDebugBreaks();
		friend int LoadDebugBreaks();

	public:		
		static CScriptDebugerMgr& Inst();

		void AddDebuger(CScriptDebuger* pDebuger);
		void RemoveDebuger(CScriptDebuger* pDebuger);
	
	private:
		CScriptDebugerMgr();
		~CScriptDebugerMgr();


		void WaitForDebuger(CScriptDebuger* pDebuger);	
		void CancelBlock(CScriptDebuger* pDebuger);

		void PrintDebugerInfo();

		void SwitchDebuger(uint32 uId, CScriptDebuger* pDebuger);

		CScriptDbContainer* GetContainer(CScriptDebuger* pDebuger);

		void AddBreak(const char* szBreack);
		void DelBreak(uint32 uBreak);
		void DelBreaks();
		void ListBreaks();
		void CheckBreak(lua_State* pState, lua_Debug* pDebug);

		CScriptBreakInfo* GetBreak(const string& strBreak);

		//����������������debug mgr�����ͽ�����ʱ����ļ��ж�ȡbreak��Ϣ�Ͱ�break��Ϣ�����ļ�
		//ʹ�����ǵĳ�����������ʱ��break����Ϣ����
		void LoadBreaks(const wchar_t* szFileName);
		void SaveBreaks();

	private:
		std::list<CScriptDbContainer*>								m_lstWaitingContainer;								
		std::map<uint32, CScriptDbContainer*>						m_mapId2Container;

		HLOCK				m_lock;
		HLOCK				m_BreakLock;

		bool					m_bRunning;
		bool					m_bDbFileLoad;

		static uint32			ms_uId;
		typedef map<uint32, CScriptBreakInfo*>					MapId2Break_t;
		MapId2Break_t				m_mapId2Break;

		typedef map<string, CScriptBreakInfo*>					MapStr2Break_t;
		MapStr2Break_t				m_mapBreak;

		
		wchar_t						m_szBreakFile[256];

		bool						m_bHasBreak;
	};

}

