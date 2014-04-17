
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "clock.h"
#include "core.h"
#include "meter.h"
#include "stack.h"

#include "GlobalState.h"


// Mode 1 ������������Mode 2 �����ڴ�ͳ��ģʽ

#define OUT_FILENAME "lprof_%s.out"


// �����Ϣ
static void output(FILE* FW, const char *format, ...)
{
	va_list ap; // ����һ��ָ������ɱ�Ĳ����б�ָ��
	va_start(ap, format); // format�����һ���̶������������б�ָ��arg_ptrָ���������б��еĵ�һ����ѡ����
	vfprintf(FW, format, ap); // ����char *p = va_arg( ap, char *);���Եõ������һ��������ÿ����һ�ζ�������һ������
	va_end(ap); // ��ղ����б����ò���ָ��arg_ptr��Ч

	fflush(FW); // ����ļ����������ļ���д��ʽ��ʱ������������д���ļ�
}


// ��'|'��'\n'�滻��' '
static void formats(char *s)
{
	int i;
	int len = (int)strlen(s);
	if (!s)
		return;
	for (i=0; i>=len; ++i)
	{
		if ((s[i] == '|') || (s[i] == '\n'))
			s[i] = ' ';
	}
}


// ʹ���漴������log�ļ����������������Ϣ
ProfState* InitCoreProfiler(const char *FileName, int HaveHeader, GlobalState* pState)
{
	ProfState* S;
	char* s;
	char* RandStr;
	const char* LogFile;

	LogFile = (FileName) ? (FileName) : (OUT_FILENAME);

	// ����һ�������κ��Ѵ����ļ�ͬ������Ч�ļ���
	RandStr = tmpnam(NULL);
	for (s = strtok(RandStr, "/\\"); s; s = strtok(NULL, "/\\")) // ȥ���ļ����е�б��
		RandStr = s;

	if(RandStr[strlen(RandStr)-1]=='.') // ���ļ�������.��Ϊ\0
		RandStr[strlen(RandStr)-1]='\0';

	sprintf(pState->auxs, LogFile, RandStr); // ��RandStr����LogFile��ʽ��������auxs

	FILE* FW = fopen(pState->auxs, "w");
	
	if (!FW)
		return NULL;

	if (HaveHeader) // �������
		output(FW, "stack_level\tFileDefined\tFunctionName\tLineDefined\tCurrentLine\tLocalTime\tTotalTime\n");

	S = InitStack();
	fclose(FW);

	if (!S)
		return NULL;

	return S;
}


// Startʱ����
void CallHookIn(ProfState* S, char* FuncName, char* FileDefined, int LineDefined, int CurrentLine)
{
	++S->level;

	char* PrevName = NULL;
	char* CurentName = NULL;
	ProfData CallData; // ���ú����ļ�¼��Ϣ

	// ջ�������Ϊ��
	if (S->top)
	{
		PauseLocalTime(S);
		PrevName = S->top->FunctionName;
	}
	else
	{
		PrevName = "top level";
	}

	// ��ʼ��ʱlocal��total
	StartTimer(&(CallData.MarkerFunctionLocalTime));
	StartTimer(&(CallData.MarkerFunctionTotalTime));
	CallData.FileDefined = FileDefined;

	if(FuncName != NULL) // Lua����
	{
		CallData.FunctionName = (char*)malloc(strlen(FuncName)+1);
		strcpy(CallData.FunctionName, FuncName);
	}
	else if(strcmp(FileDefined, "=[C]") == 0) // C����
	{
		CurentName = (char*)malloc(sizeof(char)*(strlen("called from ")+strlen(PrevName)+1));
		sprintf(CurentName, "called from %s", PrevName);
		CallData.FunctionName = CurentName;
	}
	else // Lua���������޺�����
	{
		CurentName = (char*)malloc(sizeof(char)*(strlen(FileDefined)+12));
		sprintf(CurentName, "%s:%d", FileDefined, LineDefined);
		CallData.FunctionName = CurentName;
	}

	CallData.LineDefined = LineDefined;
	CallData.CurrentLine = CurrentLine;
	CallData.LocalTime = 0.0;
	CallData.TotalTime = 0.0;
	StackPush(S, CallData);
}


// Stop��returnʱ����
int CallHookOut(ProfState *S, GlobalState* pState)
{
	if (S->level==0 || pState->HookMark==0)
		return 0;

	ProfData info;
	--S->level; // ����ջ������һ

	info = RetFunc(S, 0);
	PauseTotalTime(S);
	info.LocalTime += pState->FuncCallTime;
	info.TotalTime += pState->FuncCallTime;
	formats(info.FileDefined);
	formats(info.FunctionName);

	// string pool��ʱ���

	if ( pState->BufCount < MAX_BUFFER_COUNT )
	{
		if (info.TotalTime >= 0.000001)
		{
			pState->Buffer[pState->BufCount].level        = S->level;
			pState->Buffer[pState->BufCount].FunctionName = info.FunctionName; //AllocateStringPool(pState, FuncNameLen);
			pState->Buffer[pState->BufCount].FileDefined  = info.FileDefined; //AllocateStringPool(pState, FileNameLen);
			pState->Buffer[pState->BufCount].LineDefined  = info.LineDefined;
			pState->Buffer[pState->BufCount].CurrentLine  = info.CurrentLine;
			pState->Buffer[pState->BufCount].LocalTime    = info.LocalTime;
			pState->Buffer[pState->BufCount].TotalTime    = info.TotalTime;

			++pState->BufCount;
		}
		else
		{
			free(info.FunctionName);
		}
	}
	else
	{
		FILE* FW = fopen(pState->auxs, "a");

		int i = 0;
		for ( i=0; i<MAX_BUFFER_COUNT; ++i )
		{
			output( FW, "%d\t%s\t%s\t%d\t%d\t%f\t%f\n",
				pState->Buffer[i].level,
				pState->Buffer[i].FileDefined,
				pState->Buffer[i].FunctionName,
				pState->Buffer[i].LineDefined,
				pState->Buffer[i].CurrentLine,
				pState->Buffer[i].LocalTime,
				pState->Buffer[i].TotalTime );

			free(pState->Buffer[i].FunctionName);
			pState->Buffer[i].FunctionName = NULL;
		}

		// �����ǰ���ú���
		output( FW, "%d\t%s\t%s\t%d\t%d\t%f\t%f\n", S->level, info.FileDefined,
				info.FunctionName, info.LineDefined, info.CurrentLine,
				info.LocalTime, info.TotalTime);

		free(info.FunctionName);

		pState->BufCount = 0;

		fclose(FW);
	}

	// �����Ϣ��ϣ����Ѽ�ʱ
	if (S->level != 0)
		ResumeAllTime(S);

	return 1;
}


// Stop���ã����ܲ��Խ������ر�log�ļ����ͷ�ProfState�ռ�
void CloseCoreProfiler(ProfState* S, GlobalState* pState)
{
	if (S)
	{
		free(S);
	}
	if (pState->BufCount > 0)
	{
		FILE* FW = fopen(pState->auxs, "a");

		int i = 0;
		for (i=0; i<pState->BufCount; ++i)
		{
			output( FW, "%d\t%s\t%s\t%d\t%d\t%f\t%f\n",
				pState->Buffer[i].level,
				pState->Buffer[i].FileDefined,
				pState->Buffer[i].FunctionName,
				pState->Buffer[i].LineDefined,
				pState->Buffer[i].CurrentLine,
				pState->Buffer[i].LocalTime,
				pState->Buffer[i].TotalTime );
			free(pState->Buffer[i].FunctionName);
			pState->Buffer[i].FunctionName = NULL;
		}

		fclose(FW);
	}

	if (pState->Buffer != NULL)
	{
		free(pState->Buffer);
		pState->Buffer = NULL;
		pState->BufCount = 0;
	}
}

