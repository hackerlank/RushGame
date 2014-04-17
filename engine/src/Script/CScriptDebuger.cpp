#include "stdafx.h"
#include "CScriptDebuger.h"
#include "ScriptCommon.h"
#include "CScript.h"
#include "ScriptX.h"
#include "ThreadHelper.h"
#include "CScriptData.h"
#include "ExpHelper.h"
#include "ScriptDebugHelper.h"
#ifdef _WIN32
#include "Console.h"
#endif
#include <algorithm>
#include "TimeHelper.h"
#include "CScriptDebugerMgr.h"


namespace sqr
{
	int DebugBreak(lua_State* pState)
	{
		CScriptDebuger* pDebuger=_S( pState ).GetData()->m_pDebuger;
		if(!pDebuger)
			return 0;
		//pDebuger->SetStepOut(pState);//��DebugBreak���ش���ʼ�ϵ�

		pDebuger->Debug(pState);

		return 0;
	}
}


//////////////////////////////////////////////////////////////////////////////////////////
//ʵ��֤��������һ���µ�coroutine��ʱ�򣬻Ὣԭcoroutine�Ĺ��Ӻ����Զ����Ƶ���coroutine��
//��coroutineִ��yield��ʱ�򣬻Ὣ��coroutine�Ĺ��Ӻ������Ƶ���coroutine��
//////////////////////////////////////////////////////////////////////////////////////////


CScriptDebuger::CScriptDebuger(lua_State* pState)
:m_pBuf(NULL),m_nBreakStackLevel(-1),m_pPrevState(NULL),m_pBaseState(NULL),m_pCurHook(NULL)
, m_pDbContainer(NULL)
{
	m_szLineBuffer[0]=0;
	lua_sethook( pState, &CScriptDebuger::HookProc, LUA_MASKCALL | LUA_MASKRET | LUA_MASKLINE, 0 );
}

void CScriptDebuger::Release(lua_State* pState)
{
	lua_sethook( pState, 0, 0, 0 );

	delete this;
}

bool CScriptDebuger::PrintLine( lua_State *pState, uint32 uLevel, int32 nLine, bool bIsCurLine )
{
	lua_Debug ld;
	lua_getstack ( pState, uLevel, &ld );
	lua_getinfo ( pState, "S", &ld );


	if( ld.source[0] == '=' )
	{
		printf( "Source not available for c function.\n" );
		return false;
	}

	if( ld.source[0] != '@' )
	{
		//trunk

		const char* pPos=ld.source;

		for( int i=1 ;i< nLine ;++i )
		{
			pPos=strchr(pPos,'\n');
			if( !pPos )
			{
				printf("Line number %d out of range; (trunk) has %d lines.\n",i+1,i);
				return false;
			}
			++pPos;
		}

		const char* pEndPos=pPos;

		while( ((*pEndPos) != '\n') && ((*pEndPos) !='\0') )
			++pEndPos;

		printf( "%d ", nLine);
		if(bIsCurLine)
			printf( ">>" );
		printf( "\t" );
		fwrite( pPos, 1, pEndPos - pPos, stdout );
		printf( "\n" );

		return true;
	}

	return DebugShowLine( ld.source + 1, nLine, bIsCurLine );
}

const char* CScriptDebuger::ReadWord(bool bNewLine)
{
	if(bNewLine)
		m_pBuf=NULL;

	if( !m_pBuf )
	{
		if(!bNewLine)
			return NULL;

		fprintf(stderr,"(db) ");
		
		char szPrevLineBuffer[EInputLineLength];
		memcpy( szPrevLineBuffer , m_szLineBuffer, sizeof(szPrevLineBuffer) );

		char* p;

		if( !fgets(m_szLineBuffer,sizeof(m_szLineBuffer),stdin) )
			return NULL;

		p=m_szLineBuffer;

		while( *p == ' ' || *p == '\t' )
			p++;

		if( *p == '\n' || *p == '\0' )
		{			
			strncpy( m_szLineBuffer , szPrevLineBuffer, sizeof(m_szLineBuffer) );
			
			p=m_szLineBuffer;

			while( *p == ' ' || *p == '\t' )
				p++;
		}

		strncpy( m_szWordBuffer , m_szLineBuffer, sizeof(m_szWordBuffer) );

		m_pBuf=p;
	}

	
	char* pWord = m_pBuf;

	//������ǰ��
	while( *m_pBuf != ' ' && *m_pBuf != '\t' && *m_pBuf != '\n' && *m_pBuf != '\0' )
		m_pBuf++;

	//�Ե����пո���Ʊ��
	while( *m_pBuf == ' ' || *m_pBuf == '\t' )
	{
		*m_pBuf = '\0';
		m_pBuf++;
	}

	//�ж��ǵ������н�β���ǵ�������һ���������ʼλ��
	if( *m_pBuf == '\0' || *m_pBuf == '\n' )
	{
		*m_pBuf = '\0';
		m_pBuf = NULL;
	}

	return pWord;
}

/*
hook�ĵ��ù���:
���ú�����ʱ��HOOKCALL��HOOKLINE�Ⱥ��ں����ĵ�һ�д���һ��
�Ӻ������ص�ʱ��HOOKLINE��HOOKRETURN�Ⱥ��ں�����end������һ��
�ں���������ֱ�ӵ��ú���ʱ���������غ󲻻��ٲ�����ͣ��������ֱ�ӽ��������Ҫ���ݸ��ĺ�����
*/

void CScriptDebuger::HookProc( lua_State *pState, lua_Debug* pDebug )
{
	CScriptDebuger* pThis=_S( pState ).GetData()->m_pDebuger;

	CScriptDebugerMgr::Inst().CheckBreak(pState, pDebug);
		
	bool(CScriptDebuger::* pCurHook)(lua_State*,lua_Debug*)=pThis->m_pCurHook;
	if( !pCurHook )
		return;

	bool bResult=(pThis->*pCurHook)( pState, pDebug );

	pThis->m_pPrevState = pState;

	if( !bResult )
		return;

	pThis->Debug(pState);
}

/*
	��������������ͬ�̵߳������
	�˺�������������뺯������������û����û�к����ĵط����У����ͬ��ִ����һ�����
	line����
	�����������
	a = function()
		print("Hello World")
	end

	a()

	��ʱ��������a()�������ǵ��i����debugger�����a�������棬���ͣ����a���������һ�м�printǰ��
*/
bool CScriptDebuger::StepInHook( lua_State *pState,lua_Debug* pDebug )
{
	switch(pDebug->event)
	{
	case LUA_HOOKLINE:
		if(m_pBaseState != pState)
			return false;	
		return true;	
	case LUA_HOOKCALL:	
		if(m_pBaseState != pState)
			return false;			
		return false;
	case LUA_HOOKRET:
	case LUA_HOOKTAILRET:
		return false;
	default:
		GenErr("Invalid event in lua hook function");
		return false;
	}
}

/*
	��������������ͬ�̵߳������
	�˺��������ӵ�ǰ��������
	return ����
	�����������
	a = function()
		print("Hello World")
	end

	a()
	
	�����print���������ʱ��o�����������end�������������������ٰ�o���������a���������ͣ����a()��һ�д�

*/
bool CScriptDebuger::StepOutHook( lua_State *pState,lua_Debug* pDebug )
{
	switch(pDebug->event)
	{
	case LUA_HOOKCALL:
		return false;
	case LUA_HOOKLINE:
		if( m_pBaseState != pState )
		{		
			return false;
		}

		{
			//uint32 uRunningStackLevel=GetRunStackDepth(pState);
			//if( m_nBreakStackLevel == uRunningStackLevel + 1)
			//	return true;

		}
		return false;
	case LUA_HOOKTAILRET:
	case LUA_HOOKRET:
		if( m_pBaseState != pState )
		{		
			return false;
		}
		
		{
			int32 uRunningStackLevel=GetRunStackDepth(pState);
			if( m_nBreakStackLevel == uRunningStackLevel)
				return true;
			//Ast( m_nBreakStackLevel < uRunningStackLevel );
		}
		return false;
	default:
		GenErr("Invalid event in lua hook function");
		return false;
	}
}

/*
	ִ����һ����䣨������ͬ�߳������
	�˺�������ִ����һ�����
	line����
	�����������
	a()
	b()
	��ʱ�����a��������n����debugger������a()Ȼ�󷵻�a��һ����䣬��bǰ��

	����ں���end������n����������������ִ�иú�������һ��
*/
bool CScriptDebuger::StepOverHook( lua_State *pState,lua_Debug* pDebug )
{
	switch(pDebug->event)
	{
	case LUA_HOOKCALL:
		return false;
	case LUA_HOOKTAILRET:
	case LUA_HOOKRET:
		if(m_pBaseState != pState)
		{
			return false;
		}	
		
		//nothing
		//{
		//	uint32 uRunningStackLevel=GetRunStackDepth(pState);
		//	if(m_nBreakStackLevel==uRunningStackLevel)
		//		m_nBreakStackLevel=uRunningStackLevel;
		//	Ast( m_nBreakStackLevel < uRunningStackLevel );
		//}
		return false;
	case LUA_HOOKLINE:
		if( m_pBaseState != pState )
			return false;		
		{
			int32 uRunningStackLevel = GetRunStackDepth(pState);
			if( uRunningStackLevel > m_nBreakStackLevel)
			{				
				return false;
			}
		}
		printf("step over hook\n");
		return true;
	default:
		GenErr("Invalid event in lua hook function");
		return false;
	}
}

/*
	�л��߳�
	�˺��������ڲ�ͬ��coroutine�����л�
	line����
	�����������

	co = coroutine.create(function()
		for i = 0, 10 do
			coroutine.yield()
		end
	end)

	coroutine.resume(co)
	coroutine.resume(co)

	����ڵ�һ��resumeǰ�棬�����ʱ��s����debugger�����co�������棬ͣ��co������һ�д������л�����coroutine���߳�
	������е�yield���ٰ�s����debugger���л������̣߳�����ͣ���ڵڶ���resumeǰ�档

	��û��coroutine�л�����s�൱��ִ��i��䡣

	�������ע�⣬���ڲ�ͬ�̵߳Ĵ���ֻ��s����Ч������n��i��o��ֻ����ͬһ���߳����С�
*/
bool CScriptDebuger::StepSwitchHook(lua_State *pState, lua_Debug* pDebug)
{
	switch(pDebug->event)
	{
	case LUA_HOOKCALL:
	case LUA_HOOKTAILRET:
	case LUA_HOOKRET:
		return false;
	case LUA_HOOKLINE:
		{
			if(m_pBaseState != pState)
			{
				printf("the stack is not the same\n");
			}
		}
		return true;
	default:
		GenErr("Invalid event in lua hook function");
		return false;
	}
}

void CScriptDebuger::SetStepSwitch(lua_State* pState)
{
	//if(m_pCurHook==&CScriptDebuger::StepSwitchHook)
	//	return SetStepIn(pState);

	m_pPrevState = m_pBaseState = pState;
	m_pCurHook = &CScriptDebuger::StepSwitchHook;
	m_nBreakStackLevel=INT_MAX;					//�޶�ջ���������κ���������Զ�
}

void CScriptDebuger::SetStepOver(lua_State* pState)
{
	//if(m_pCurHook==&CScriptDebuger::StepOutHook)
	//	return SetStepIn(pState);
	//m_pPrevState=m_pBaseState=pState;
	m_pBaseState=pState;
	m_pCurHook=&CScriptDebuger::StepOverHook;
	m_nBreakStackLevel=GetRunStackDepth(pState); //ջ��������뵱ǰִ�������ͬ
}

void CScriptDebuger::SetStepIn(lua_State* pState)
{
	//m_pPrevState=m_pBaseState=pState;
	m_pBaseState=pState;
	m_pCurHook=&CScriptDebuger::StepInHook;
	m_nBreakStackLevel=INT_MAX;	//�޶�ջ���������κ���������Զ�
}

void CScriptDebuger::SetStepOut(lua_State* pState)
{
	if(m_pCurHook==&CScriptDebuger::StepOutHook)
		return SetStepOver(pState);
	//m_pPrevState=m_pBaseState=pState;
	m_pBaseState=pState;
	m_pCurHook=&CScriptDebuger::StepOutHook;
	m_nBreakStackLevel=GetRunStackDepth(pState); //ջ��������뵱ǰִ�������ͬ
}

static void GetDbChannelCo(lua_State* pState)
{
	lua_getglobal(pState, "g_DbChannelMgr");	
	Ast(!lua_isnil(pState, -1));				
	lua_getfield(pState, -1, "_m_AllCoInfo");	
	lua_remove(pState, -2);						
}

static void PrintDbChannelCo(lua_State* pState)
{
	int top = lua_gettop(pState);
	
	GetDbChannelCo(pState);

	//��suspend��coroutine��ӡ����
	int row = 0;
	printf("\n");
	lua_pushnil(pState);//stack:dbco,nil
	while (lua_next(pState, -2) != 0)//stack: dbco, key ,value
	{
		lua_State *pCoState = lua_tothread(pState, -2);
		if (LUA_YIELD != lua_status(pCoState))
		{
			row++;
			printf("��");
			printf("%d", row);
			printf("��coroutine: ");
			PrintFrame(lua_tothread(pState, -2), 2);
		}
		lua_pop(pState, 1);
	}
	if (row == 0)
	{
		printf("��ʱDbChannelMgrû�б�yield��coroutine��\n");
	}

	lua_settop(pState, top);
}

void CScriptDebuger::Debug(lua_State* pState)
{
	CScriptDebugerMgr::Inst().WaitForDebuger(this);

#ifdef _WIN32
	ShowConsole(true);	//����̨�������е������ʱ��һ���᷵��false
#endif
	
	HTHREAD hThread;
	GetCurrentThread(&hThread);
	EnableWatchThread(hThread, false);

	uint64 uDebugBeginTime = GetProcessTime();

	const char* szBuf;
	
	m_bAtFileEnd=false;
	m_nCurLevel = 0;
	m_nCurLine = GetCurLine( pState,m_nCurLevel );

	PrintFrame(pState,m_nCurLevel);
	PrintLine(pState,m_nCurLevel,m_nCurLine,true);

	for(;;)
	{
		szBuf = ReadWord(true);
		if( !szBuf )	//Control+C
			exit(0);
		
		if( strlen( szBuf) == 0 )
			continue;
		
		if( !strcmp( szBuf, "help") )
		{
			HandleHelp();
		}
		else if( !strcmp( szBuf, "quit" ) || !strcmp( szBuf, "exit" ) || !strcmp( szBuf, "q" ) || !strcmp(szBuf, "Q"))
		{
			exit(0);
		}
		else if( !strcmp( szBuf, "continue" ) || !strcmp( szBuf, "c" ) || !strcmp(szBuf, "C"))
		{
			m_pPrevState=m_pBaseState=NULL;
			m_pCurHook=NULL;
			break;
		}
		else if( !strcmp( szBuf, "load" ) || !strcmp( szBuf, "lo") )
		{
			HandleLoad(pState);
		}
		else if( !strcmp( szBuf, "next" ) || !strcmp( szBuf, "n" ) || !strcmp(szBuf, "N"))
		{
			SetStepOver(pState);
			break;
		}
		else if( !strcmp( szBuf, "nl" ) )
		{
			SetStepOver(pState);
			HandleList(pState);
			break;
		}
		else if( !strcmp( szBuf, "in" ) || !strcmp( szBuf, "i" ) || !strcmp(szBuf, "I"))
		{
			SetStepIn(pState);
			break;
		}
		else if( !strcmp( szBuf, "out" ) || !strcmp( szBuf, "o" ) || !strcmp(szBuf, "O"))
		{
			SetStepOut(pState);
			break;
		}
		else if( !strcmp( szBuf, "switch" ) || !strcmp( szBuf, "s" ) || !strcmp(szBuf, "S"))
		{
			SetStepSwitch(pState);
			break;
		}
		else if( !strcmp( szBuf, "list" ) || !strcmp( szBuf, "l" ) || !strcmp(szBuf, "L"))
		{
			HandleList(pState);
		}
		else if( !strcmp( szBuf, "backtrace" ) || !strcmp( szBuf, "bt" ) || !strcmp(szBuf, "BT"))
		{
			for( int i = 0; PrintFrame( pState,i); i++ );
		}
		else if( !strcmp( szBuf, "frame" ) || !strcmp( szBuf, "f" ) || !strcmp(szBuf, "F"))
		{
			HandleFrame(pState);
		}
		else if( !strcmp( szBuf, "printco") || !strcmp( szBuf, "pco" ))
		{
			PrintDbChannelCo(pState);
		}
		else if( !strcmp( szBuf, "showco") || !strcmp( szBuf, "sco" ))
		{
			HandleShowCo(pState);
		}
		else if( !strcmp( szBuf, "print" ) || !strcmp( szBuf, "p" ) )
		{
			HandlePrint(pState);
		}
		else if(!strcmp( szBuf, "info" ) )
		{
			HandleInfo(pState);
		}
		else if(!strcmp( szBuf, "thread" ))
		{
			HandleThread(pState);
		}
		else if(!strcmp(szBuf, "break") || !strcmp(szBuf, "b"))
		{
			HandleAddBreak(pState);
		}
		else if(!strcmp(szBuf, "delete") || !strcmp(szBuf, "d"))
		{
			HandleDelBreak(pState);
		}
		else
		{   
			fprintf( stderr, "����������󣡣���\n" );
		}
	}

	SetProcessBeginTime( GetProcessTime() - uDebugBeginTime + GetProcessBeginTime() );

	EnableWatchThread(hThread, true);

#ifdef _WIN32
	ShowConsole(false);	//����̨�������е������ʱ��һ���᷵��false
#endif

	CScriptDebugerMgr::Inst().CancelBlock(this);
}

void CScriptDebuger::HandleHelp()
{
	const char* szHelp=
		"backtrace/bt           �г����ö�ջ\n"
		"continue/c             ����ִ��\n"
		"next/n                 ִ����һ��\n"
		"in/i                   ����\n"
		"out/o                  ����\n"
		"switch/s               �л�coroutine\n"
		"list/l                 �г��������д���\n"
		"frame/f n              ת����n���ջ\n"
		"print/p v              ��ӡv��ֵ\n"
		"load/lo file           ������������벢ִ��ָ���ļ�\n"
		"printco/pco            ��ӡDbChannel��ǰ����coroutine\n"
		"showco/sco n           �����n��coroutineִ�ж�ջ\n"
		"info thread            ��ʾ�ű��߳����\n"
		"thread n               �л�����һ���̵߳�debuger\n"
		"break/b source:line    ���öϵ�\n"
		"info break             ��ʾ�ű����жϵ�\n"
		"delete/d n             ɾ��ĳ���ϵ�\n"
		"delete/d               ɾ�����жϵ�\n"
		"help                   ��ӡ����\n"
		;
			
	printf(szHelp);
}

void CScriptDebuger::HandleLoad(lua_State* pState)
{
	const char* szBuf = ReadWord();
	if(!szBuf)
	{
		lua_Debug ld;
		lua_getstack ( pState, m_nCurLevel, &ld );
		lua_getinfo ( pState, "S", &ld );

		if(*ld.source!='@')
		{
			fprintf( stderr, "Can only load lua file.\n" );
			return;
		}

		szBuf=(ld.source+1);
	}
	if(luaL_dofile( pState, szBuf ))
	{
		printf( "%s\n", lua_tostring( pState, -1 ));
		lua_pop( pState, 1);
	}
}


void CScriptDebuger::HandleList(lua_State* pState)
{
	const char* szBuf = ReadWord();

	int nLineCount=15; //һ����ʾ���������

	if(szBuf)
	{
		if(isdigit(*szBuf))
		{
			m_nCurLine=atoi(szBuf);
		}
		else
		{
			if(m_bAtFileEnd)
			{
				m_bAtFileEnd=false;
				m_nCurLine-=nLineCount;
			}
			else
			{
				m_nCurLine-=nLineCount*2;
			}

			if(m_nCurLine<=nLineCount/2)
				m_nCurLine=nLineCount/2;
		}
	}

	int nShowBeginLine=m_nCurLine-nLineCount/2; //��ʾ�������ʼλ��
	int nShowCounter=0;

	for( nShowCounter = 0; nShowCounter < nLineCount; nShowCounter++ )
	{
		int nShowLine=nShowBeginLine+nShowCounter;
		if(nShowLine>0)
		{
			if(!PrintLine( pState,m_nCurLevel,nShowLine,nShowLine==GetCurLine(pState,m_nCurLevel)))
			{
				m_bAtFileEnd=true;
				break;
			}
		}
	}
	m_nCurLine+=nShowCounter;
}

void CScriptDebuger::HandleFrame(lua_State* pState)
{
	const char* szBuf = ReadWord();
	bool bValidStackNumber=true;

	if(szBuf)
	{
		int nLevel = m_nCurLevel;

		if(isdigit(*szBuf))
		{
			nLevel=atoi( szBuf );

			lua_Debug ld;
			if( lua_getstack ( pState, nLevel, &ld ) )
			{
				m_nCurLevel = nLevel;
				m_nCurLine = GetCurLine( pState, m_nCurLevel );
				m_bAtFileEnd=false;
			}
			else
				bValidStackNumber=false;
		}
		else
			bValidStackNumber=false;
	}
	else
	{
		m_nCurLine = GetCurLine( pState, m_nCurLevel );
	}
	if(!bValidStackNumber)
	{
		fprintf(stderr,"Invalid stack number.\n");
		return;
	}
	PrintFrame( pState,m_nCurLevel );
	PrintLine( pState, m_nCurLevel,m_nCurLine,true);
}

void CScriptDebuger::HandleShowCo(lua_State* pState)
{
	const char* szBuf = ReadWord();
	if(szBuf)
	{
		int nLevel = 1;

		if(isdigit(*szBuf))
		{
			nLevel = atoi(szBuf);
			GetDbChannelCo(pState);
			lua_pushnil(pState);
			int num = 1;
			lua_State *pCoState = NULL;
			while (lua_next(pState, -2) != 0)
			{
				if(num == nLevel)
				{
					pCoState = lua_tothread(pState, -2);		
					break;
				}
				lua_pop(pState, 1);
				num++;
			}
			if(pCoState)
			{
				Debug(pCoState);
			}
		}
	}
}

static bool FindAndPrintInLocal(lua_State* pState, lua_Debug* pDebug, 
									 const string& sName, const list<string>& listField)
{
	int n = 1;
	const char* name = NULL;
	bool bFound = false;
	int nTop = lua_gettop(pState);
	while(( name = lua_getlocal( pState, pDebug, n++ ) ) != NULL )
	{
		bFound = (sName == name);
		if( bFound )
		{
			Print_lua( pState, listField );
			break;
		}
		else
		{
			lua_pop( pState, 1 );
		}
	}

	lua_settop(pState, nTop);
	return bFound;
}

static bool FindAndPrintInUpvalue(lua_State* pState, lua_Debug* pDebug, 
									 const string& sName, const list<string>& listField)
{
	bool bFound = false;
	const char* name = NULL;
	int nTop = lua_gettop(pState);

	lua_getinfo( pState, "f", pDebug );

	int n = 1;

	while( !bFound && ( name = lua_getupvalue( pState, -1, n++ ) ) != NULL )
	{
		bFound = (sName == name);
		if( bFound )
		{
			Print_lua( pState, listField );
			break;
		}
		else
		{
			lua_pop( pState, 1 );
		}
	}

	lua_settop(pState, nTop);

	return bFound;
}

static bool FindAndPrintInEnv(lua_State* pState, lua_Debug* pDebug, 
									   const string& sName, const list<string>& listField)
{
	bool bFound = false;
	
	int nTop = lua_gettop(pState);
	
	lua_getinfo( pState, "f", pDebug );

	lua_getfenv(pState, -1);

	lua_getfield(pState, -1, sName.c_str());
	if( ( bFound = lua_type( pState, -1 ) != LUA_TNIL ) )
		Print_lua( pState, listField );
	else
		lua_pop( pState, 1 );
	
	lua_settop(pState, nTop);
	return bFound;
}

static bool FindAndPrintInGlobal(lua_State* pState, lua_Debug* pDebug, 
								   const string& sName, const list<string>& listField)
{
	bool bFound = false;
	int nTop = lua_gettop(pState);

	lua_getglobal( pState, sName.c_str() );
	if( ( bFound = lua_type( pState, -1 ) != LUA_TNIL ) )
		Print_lua( pState, listField );
	else
		lua_pop( pState, 1 );

	lua_settop(pState, nTop);
	return bFound;
}

void CScriptDebuger::HandlePrint(lua_State* pState)
{
	const char* szBuf = ReadWord();

	string sVarName;

	if(!szBuf )
	{
		if(m_sLastVarName.empty())
		{
			printf("The history is empty.\n");
			return;
		}
		sVarName=m_sLastVarName;
	}
	else
	{
		sVarName=szBuf;
	}

	lua_Debug ld;
	if( lua_getstack ( pState, m_nCurLevel, &ld ) )
	{
		list<string> listField;

		vector<char> szTemp( sVarName.size() + 1, 0 );
		memcpy( &szTemp[0], sVarName.c_str(), sVarName.size() );

		const char* szSep = ".[]";

		char buffer[1024];
		strcpy(buffer, sVarName.c_str());

		char* token = strtok( buffer, szSep ); 
		while( token != NULL )
		{
			listField.push_back(token);
			token = strtok( NULL, szSep ); 
		}

		if (listField.empty())
		{
			printf("Input Invalid Parameter\n");
			return;
		}

		string sFirstName = *listField.begin();
		listField.erase( listField.begin() );

		bool bFound = false;

		do 
		{
			if(bFound = FindAndPrintInLocal(pState, &ld, sFirstName, listField))
				break;
			
			if(bFound = FindAndPrintInUpvalue(pState, &ld, sFirstName, listField))
				break;

			if (bFound = FindAndPrintInEnv(pState, &ld, sFirstName, listField))
				break;

			if (bFound = FindAndPrintInGlobal(pState, &ld, sFirstName, listField))
				break;

		} while (0);

		if( !bFound )
		{
			string strExe = "print(" + sVarName + ")";
			luaL_dostring( pState, strExe.c_str() );
		}
	}
	else
		printf( "Invalid stack number!\n" );

	m_sLastVarName=sVarName;
}

void CScriptDebuger::HandleThread(lua_State* pState)
{
	const char* szBuf = ReadWord();

	if(szBuf)
	{
		if(isdigit(*szBuf))
		{
			uint32 uId = (uint32)atoi( szBuf );
			CScriptDebugerMgr::Inst().SwitchDebuger(uId, this);
		}
		else
		{
			printf("Please Input Corrent Thread ID\n");
		}
	}
}

void CScriptDebuger::HandleInfo(lua_State* pState)
{
	const char* szBuf = ReadWord();
	if(!strcmp( szBuf, "thread" ))
	{
		CScriptDebugerMgr::Inst().PrintDebugerInfo();		
	}
	else if(!strcmp( szBuf, "break") )
	{
		CScriptDebugerMgr::Inst().ListBreaks();
	}
}

void CScriptDebuger::HandleAddBreak(lua_State* pState)
{
	const char* szBuf = ReadWord();
	if(szBuf)
	{
		CScriptDebugerMgr::Inst().AddBreak(szBuf);	
	}
}

void CScriptDebuger::HandleDelBreak(lua_State* pState)
{
	const char* szBuf = ReadWord();
	if(szBuf)
	{
		if(isdigit(*szBuf))
		{
			uint32 uBreak = (uint32)atoi(szBuf);
			CScriptDebugerMgr::Inst().DelBreak(uBreak);
		}
		else
		{
			printf("��������ȷ�Ķϵ���\n");
		}
	}
	else
	{
		CScriptDebugerMgr::Inst().DelBreaks();
	}
}

