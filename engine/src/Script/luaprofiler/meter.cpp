
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "clock.h"
#include "meter.h"
#include "stack.h"


#include "GlobalState.h"


// ����ֲ�ʱ��
static void ComputeLocalTime(ProfData* e)
{
	ASSERT(e, "local time null");
	e->LocalTime += GetSeconds(e->MarkerFunctionLocalTime);
}


// ������ʱ��
static void ComputeTotalTime(ProfData* e)
{
	ASSERT(e, "total time null");
	e->TotalTime += GetSeconds(e->MarkerFunctionTotalTime);
}

// Ϊ��ǰ��������ʱ�䣬CallFunc
void PauseLocalTime(ProfState* S)
{
	ComputeLocalTime(S->top);
}


// ��ͣ��ʱ�䣬CallHookOut
void PauseTotalTime(ProfState* S)
{
	ProfData* aux;

	ASSERT(S->top, "pause_TotalTime: stack_top null");

	// ȡ������ջ��������ʱ����ӣ��õ���ִ��ʱ��
	aux = S->top;
	while (aux)
	{
		ComputeTotalTime(aux);
		aux = aux->next;
	}
}


// �ָ�local�����ļ�ʱ
void ResumeLocalTime(ProfState* S)
{
	ASSERT(S->top, "resume_LocalTime: stack_top null");
	StartTimer(&(S->top->MarkerFunctionLocalTime));
}


// �ָ�total�����ļ�ʱ
void ResumeTotalTime(ProfState* S)
{
	ProfData* aux = NULL;

	ASSERT(S->top, "resume_TotalTime: stack_top null");

	// �ָ��������к����ļ�ʱ
	aux = S->top;
	while (aux)
	{
		StartTimer(&(aux->MarkerFunctionTotalTime));
		aux = aux->next;
	}
}


// �ָ�local��total�����ļ�ʱ��CallHookOut
void ResumeAllTime(ProfState* S)
{
	ASSERT(S->top, "resume_function: stack_top null");

	ResumeLocalTime(S);
	ResumeTotalTime(S);
}


// ��������
ProfData RetFunc(ProfState* S, int IsResume)
{
	ProfData RetData; // ���غ����ļ�¼��Ϣ

	ASSERT(S->top, "leave_function: stack_top null");

	RetData = StackPop(S);

	ComputeTotalTime(&RetData);
	if (RetData.TotalTime < 0.000001)
		RetData.LocalTime = 0.0;
	else
		ComputeLocalTime(&RetData);

	if (IsResume) // Ŀǰ�����ֵ��0
		ResumeLocalTime(S);

	return RetData;
}


