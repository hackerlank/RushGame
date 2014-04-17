#include "stdafx.h"
#include "ScriptCoroutine.h"
#include "ScriptCommon.h"
#include "CScriptData.h"
#include "ScriptLog.h"
#include "ScriptDebugHelper.h"
#include "ErrLogHelper.h"

namespace sqr
{
	int BeforeResume(lua_State* pState)
	{
		lua_State* co = lua_tothread(pState, -1);

		CScriptData* pData=CScript::Inst()->GetData();
		//�ѵ�ǰ��running state���óɸ�coroutine
		pData->m_pRunningState = co;

		//�õ��л�coroutine֮ǰ��context stack
		void* pSaveStack = GetCurrentContextStack();

		//�����context stack
		pData->SetContextStack(pState, pSaveStack);

		//�õ��л�coroutine����context stack������������Ϊ��ǰ��
		void* pStack = pData->GetContextStack(co);
		SetCurrentContextStack(pStack);

		lua_pushlightuserdata(pState, pSaveStack);

		return 1;
	}

	int AfterResume(lua_State* pState)
	{
		lua_State* co = lua_tothread(pState, 1);
		void* pSaveStack = lua_touserdata(pState, 2);

		CScriptData* pData=CScript::Inst()->GetData();

		bool bYield = (lua_status(co) == LUA_YIELD);

		if(bYield)
		{
			//���coroutine��yield״̬����ô���Ǳ����coroutine�����context stack
			void* pCoStack = GetCurrentContextStack();
			pData->SetContextStack(co, pCoStack);
			SetCurrentContextStack(pSaveStack);
		}
		else //coroutine ������������ʧ��
		{
			pData->RemoveContextStack(co);
			SetCurrentContextStack(pSaveStack);
		}

		//�ѵ�ǰ��running state���óɵ��ø�coroutine��state
		pData->m_pRunningState = pState;

		return 0;
	}


}
