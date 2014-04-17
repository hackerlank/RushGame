#pragma once
/*
* FILE: CThread.h
* DESC: Intercurrent operation function family
* DATE: Jul.21.2004
*/

#include "ModuleDefs.h"
#include "ThreadTypes.h"

namespace sqr
{
	//return false  ��ʾ������ϵͳ�źŻ���APC
	MODULE_API bool Sleep(unsigned uMilliSecond);

	// Default priority of a thread is eTP_Lowest
	// to a game running in full speed. a low thread priority is good.
	MODULE_API void CreateThread(HTHREAD* phThread,unsigned long nStackSize,THREADPROC lpStartRoutineAddress,void* lpParameter);

	MODULE_API void DetachThread(HTHREAD* phThread);

	MODULE_API void ExitThread(unsigned long dwExitCode);

	MODULE_API bool LowerCurThreadPriority();

	MODULE_API void TerminateThread(HTHREAD* HTHREAD, unsigned long dwExitCode);

#ifdef GetCurrentThread
#undef GetCurrentThread
#endif

	MODULE_API void GetCurrentThread(HTHREAD* phThread);

	MODULE_API void JoinThread(HTHREAD* phThread);

	MODULE_API HTHREADID GetCurTID();

	MODULE_API HPID GetCurPID();

	// Lock
	MODULE_API void CreateLock(HLOCK* pLock);

	MODULE_API void DestroyLock(HLOCK* pLock);

	MODULE_API void Lock(HLOCK* pLock);

	MODULE_API void Unlock(HLOCK* pLock);



#undef CreateSemaphore
	// Semaphore
	MODULE_API void CreateSemaphore(HSEMAPHORE* phSemaphore, int32 nInitCount, int32 nMaxCount);

	MODULE_API void PutSemaphore(HSEMAPHORE* phSemaphore);

	//����ֵ˵��
	//0 �ɹ�
	//1 ��ʱ
	//-1 ��Ч�ź���
	MODULE_API int32 GetSemaphore(HSEMAPHORE* phSemaphore,unsigned uMilliSecond);
	MODULE_API int32 GetSemaphore(HSEMAPHORE* phSemaphore);
	MODULE_API void DestroySemaphore(HSEMAPHORE* phSemaphore);


	//spinlock
	//���浽Ҫ��SpinLock��?
	//1.�ܶ��ֳ�ͬʱ��������ǧ��Ҫ��spinlock
	//2.spinlock���������ڳ�̬�������������������߳̽�����Ϣ�Ķ��С�
	MODULE_API void  CreateSpinLock(HSPINLOCK* phSpinLock);
	MODULE_API void  SpinLock(HSPINLOCK* phSpinLock);
	MODULE_API void  SpinUnlock(HSPINLOCK* phSpinLock);
	MODULE_API void  DestroySpinLock(HSPINLOCK* phSpinLock);

	//thread local storage
	MODULE_API void TLS_CreateKey(HTLSKEY* key);
	MODULE_API void TLS_DestroyKey(HTLSKEY key);
	MODULE_API void TLS_SetValue(HTLSKEY key, void* pData);
	MODULE_API void* TLS_GetValue(HTLSKEY key);

	MODULE_API void WatchThread(HTHREAD hThread,const char* szThreadName,volatile uint64* pTimeVar,uint64 uAllowedDelay,uint32 uAllowedTimes,uint64 uSlowValue);
	MODULE_API void DoNotWatchThread(HTHREAD hThread);
	MODULE_API void EnableWatchThread(HTHREAD hThread, bool bEnabled);

	MODULE_API void InitAltSigStack();
	MODULE_API void UnitAltSigStack();
	MODULE_API void SetAltSigStackEnabled(bool bEnabled);

	MODULE_API void SetThreadName(const char* szName);
	MODULE_API const char* GetThreadName();
}

