#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "LuaProfiler.h"
#include "GlobalState.h"

#include "clock.h"
#include "core.h"

static int StateID;

#define GLOBALSTATE_NAME "__GlobalState__"

//��Ϊ����profilerҪ֧�ֽű�����̣߳����ԾͲ�����ȫ�ֵ������ʾ��
//GlobalState  g_State;



// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

static const luaL_reg OpFunc[] = {
	{ "Start", profiler_init },
	{ "Stop", profiler_stop },
	{ NULL, NULL }
};


// ע��luaopen_profiler��C������Lua�У���ע��һЩ������Ϣ
int luaopen_profiler(lua_State *L)
{
	luaL_openlib(L, "profiler", OpFunc, 0); // ע�᷽��
	lua_pushliteral(L, "_COPYRIGHT"); // metatable._COPYRIGHT = "Copyright (C) 2003-2007 Kepler Project"
	lua_pushliteral(L, "Copyright (C) 2009 Ybtx Project");
	lua_settable(L, -3); // �����������ѹ���ִ���ɼ�ֵ��ɵı��ַѹ��ջ������ջ�е�����ֵ
	lua_pushliteral (L, "_DESCRIPTION");
	lua_pushliteral (L, "LuaProfiler is a program grow with Ybtx project for find bottlenecks.");
	lua_settable (L, -3);
	lua_pushliteral (L, "_NAME");
	lua_pushliteral (L, "LuaProfiler");
	lua_settable (L, -3);
	lua_pushliteral (L, "_VERSION");
	lua_pushliteral (L, "0.1.0");
	lua_settable (L, -3);

	return 1;
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


// �����Lua����һ��������Ҫ�Ĵ�Լʱ��
static double calcCallTime(lua_State *L)
{
	sqr::uint64 timer;
	char Code[] = "                  \
		function MesureFunc()        \
			local i                  \
                                     \
			local t = function()     \
			end                      \
                                     \
			i = 1                    \
			while (i < 100000) do    \
				t()                  \
				i = i + 1            \
				end                  \
			end                      \
                                     \
			MesureFunc()             \
			MesureFunc = nil         \
		";

	StartTimer(&timer);
	luaL_dostring(L, Code); // ����Lua����

	return GetSeconds(timer) / (double) 100000;
}


// ���û򷵻غ����¼�����������ͨ��CallHookIn��CallHookOutʵ��ʱ��ͳ��
static void CallHook(lua_State *L, lua_Debug *ar)
{
	int CurrentLine;
	lua_Debug PrevAr;
	ProfState* S = NULL;
	lua_pushlightuserdata(L, &StateID);
	lua_gettable(L, LUA_REGISTRYINDEX); // ����StateID����ȡregistry���е�table
	S = (ProfState*)lua_touserdata(L, -1);

	// �鿴level 1�Ƿ����
	if (lua_getstack(L, 1, &PrevAr) == 0)
	{
		CurrentLine = -1;
	}
	else
	{
		lua_getinfo(L, "l", &PrevAr); // ��ȡ��level����Ϣ�����洢�ھֲ�lua_Debug�ṹ��
		CurrentLine = PrevAr.currentline;
	}

	lua_getinfo(L, "nS", ar);

	if ( strstr(ar->source, "function") == NULL ) //  && strcmp(info->FileDefined, "=[C]") != 0
	{
		if (!ar->event) // ����0�����Ǻ�����ڣ�1���Ǻ�������
			CallHookIn(S, (char*)ar->name, (char*)ar->source, ar->linedefined, CurrentLine);
		else
		{
			lua_getglobal(L, GLOBALSTATE_NAME);
			GlobalState* pGlobalState = (GlobalState*)lua_touserdata(L, -1);
			lua_pop(L, 1);
			CallHookOut(S, pGlobalState);
		}
	}
}


// ע��hook�¼�������log�ļ������ѵ�ǰprofile״̬ѹ��registry��
int profiler_init(lua_State *L)
{
	lua_getglobal(L, GLOBALSTATE_NAME);
	//�����Ϊnil����֤���Ƕ��init�ˣ���ʱ��������stop��һ�εģ��������µ�
	if(!lua_isnil(L, -1))
	{
		profiler_stop(L);	
	}
	lua_pop(L, 1);

	// ��ʼ��ȫ�ֱ���
	GlobalState* pGlobalState = (GlobalState*)malloc(sizeof(GlobalState));
	lua_pushlightuserdata(L, pGlobalState);
	lua_setglobal(L, GLOBALSTATE_NAME);

	pGlobalState->BufCount = 0;
	pGlobalState->Buffer = (BufferData*)malloc(sizeof(BufferData) * MAX_BUFFER_COUNT);
	ProfState *S;
	const char* OutFile;

	pGlobalState->FuncCallTime = calcCallTime(L);
	OutFile = NULL;
	if (lua_gettop(L) == 1)
		OutFile = luaL_checkstring(L, -1); // ��Ϊstart��һ������ļ����Ŀ�ѡ����

	lua_sethook(L, (lua_Hook)CallHook, LUA_MASKCALL | LUA_MASKRET, 0);
	pGlobalState->HookMark = 1;

	if (!(S=InitCoreProfiler(OutFile, 1, pGlobalState))) // ����log�ļ��������������Ϣ�����س�ʼ����Ա��S
	{
		luaL_error(L, "LuaProfiler error: output file could not be opened!");
		lua_pushnil(L);
		return 0;
	}

	// �����ֵ�ԣ������ɱ�洢����LUA_REGISTRYINDEXΪ��������registry��ջ����
	lua_pushlightuserdata(L, &StateID);
	lua_pushlightuserdata(L, S);
	lua_settable(L, LUA_REGISTRYINDEX);

	CallHookIn(S, "start", "(C)", -1, -1);
	lua_pushboolean(L, 1); // Ϊ����ļ���ṩ����

	return 1;
}


// ���ܷ�������
int profiler_stop(lua_State *L)
{
	lua_getglobal(L, GLOBALSTATE_NAME);
	GlobalState* pGlobalState = (GlobalState*)lua_touserdata(L, -1);
	lua_pop(L, 1);
	lua_pushnil(L);
	lua_setglobal(L, GLOBALSTATE_NAME);

	ProfState *S;
	lua_sethook(L, NULL, 0, 0); // ����������Ϊ�գ�����ע�������¼�����
	lua_pushlightuserdata(L, &StateID);
	lua_gettable(L, LUA_REGISTRYINDEX);
	if(!lua_isnil(L, -1)) // ���metaltable����
	{
		S = (ProfState*)lua_touserdata(L, -1);
		// �ͷź������ܲ���ջ������ֹͣ���м�����ͬʱ���ջ����Ϣ
		while ( CallHookOut(S, pGlobalState) );
		CloseCoreProfiler(S, pGlobalState);
		lua_pushboolean(L, 1);
	}
	else
		lua_pushboolean(L, 0);

	pGlobalState->HookMark = 0;
	free(pGlobalState->Buffer);
	free(pGlobalState);
	return 1;
}

