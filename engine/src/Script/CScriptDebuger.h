#pragma once

#include "ScriptDef.h"

extern "C"
{
#include "lua/lua.h"
}

#include <set>
#include <string>


namespace sqr
{
	class CScriptDebugerMgr;
	class CScriptDbContainer;

	class CScriptDebuger
	{
		friend class CScriptDebugerMgr;

	private:
		//�����ĸ�����ֻ���ڽ���StepIn,StepOut,StepOver�Ĺ����вű��õ���ƽʱ���ᱻ���ʡ�
		//���Կ��ǽ����Ƿŵ�һ���ṹ������
		char*			m_pBuf;
		int32				m_nBreakStackLevel;		//�ϵ�Ķ�ջ���
		lua_State*			m_pPrevState;			//��һ��hook��������state�������ж��Ƿ�������µ�coroutine
		lua_State*			m_pBaseState;			//���Կ�ʼʱ��state��
		bool(CScriptDebuger::* m_pCurHook)(lua_State*,lua_Debug*);

		CScriptDbContainer*	m_pDbContainer;

		std::string			m_sLastVarName;


		enum{EInputLineLength=1024};

		char			m_szLineBuffer[EInputLineLength];
		char			m_szWordBuffer[EInputLineLength];
		
		bool PrintLine( lua_State *pState, uint32 uLevel, int32 nLine, bool bIsCurLine );
		
		const char* ReadWord(bool bNewLine=false);
		//-----------------------------------------------------
		// ���ű�ִ��������תʱ������ô˺���
		//-----------------------------------------------------
		static void HookProc( lua_State *pState, lua_Debug* pDebug );

		bool StepInHook( lua_State *pState,lua_Debug* pDebug );
		bool StepOutHook( lua_State *pState,lua_Debug* pDebug );
		bool StepOverHook( lua_State *pState,lua_Debug* pDebug );
		bool StepSwitchHook(lua_State *pState, lua_Debug* pDebug);

		void SetHook( lua_State* pState,bool bSetHook);
		void SetBaseState( lua_State* pState);

		bool m_bAtFileEnd;
		int m_nCurLevel;
		int m_nCurLine;

		void HandleHelp();
		void HandleLoad(lua_State* pState);
		void HandleList(lua_State* pState);
		void HandlePrint(lua_State* pState);
		void HandleFrame(lua_State* pState);
		void HandleShowCo(lua_State* pState);
		void HandleThread(lua_State* pState);
		void HandleInfo(lua_State* pState);
		void HandleDelBreak(lua_State* pState);
		void HandleAddBreak(lua_State* pState);

	public:	

		CScriptDebuger(lua_State* pState);
		void Release(lua_State* pState);

		void SetStepOver(lua_State* pState);
		void SetStepIn(lua_State* pState);
		void SetStepOut(lua_State* pState);
		void SetStepSwitch(lua_State* pState);
		
		void Debug(lua_State* pState);
	};
}
