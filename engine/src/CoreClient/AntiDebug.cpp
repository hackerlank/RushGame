#include "stdafx.h"
#include "AntiDebug.h"
#include "CAppConfigClient.h"
#include "ExpHelper.h"
#include <shlobj.h>
#include <Psapi.h>
#include "StringHelper.h"

typedef struct _PROCESS_DEBUG_PORT_INFO 
{ 
	HANDLE DebugPort; 
}PROCESS_DEBUG_PORT_INFO;

typedef struct _PROCESS_BASIC_INFO
{
	DWORD ExitStatus;
	DWORD PebBaseAddress;
	DWORD AffinityMask;
	DWORD BasePriority;
	ULONG UniqueProcessId;
	ULONG InheritedFromUniqueProcessId;
} PROCESS_BASIC_INFO;

typedef long              NTSTATUS; 

enum PROCESS_INFO_CLASS
{
	ProcessBasicInfo = 0,
	ProcessDebugPort = 7
};

typedef NTSTATUS  (NTAPI *ZW_QUERY_INFORMATION_PROCESS)(IN HANDLE ProcessHandle, IN PROCESS_INFO_CLASS ProcessInformationClass, OUT PVOID ProcessInformation, IN ULONG ProcessInformationLength, OUT PULONG ReturnLength);
#define STATUS_SUCCESS    ((NTSTATUS)0L) 

//------------------------------------------------------------------------------
namespace sqr
{

	//------------------------------------------------------------------------------
	/**
	���IsDebuggerPresent().
	*/
	bool CheckDebugger()
	{
		__asm
		{
			mov eax, fs:[30h] ;EAX =  TEB.ProcessEnvironmentBlock
				inc eax
				inc eax
				mov eax, [eax]
			and eax,0x000000ff	;AL  =  PEB.BeingDebugged
				test eax, eax
				jne rt_label
				jmp rf_label
		}
rt_label:
		return true;
rf_label:
		return false;
	}

	//------------------------------------------------------------------------------
	/**
	ʹ��ntdll_NtQueryInformationProcess()����ѯProcessDebugPort����������ⷴ���ԡ�
	������̱����ԣ��䷵��ֵӦΪ0xffffffff��
	*/
	bool
		CheckDebugPort()
	{
		HMODULE hModule = GetModuleHandle("ntdll.dll"); 
		ZW_QUERY_INFORMATION_PROCESS ZwQueryInformationProcess; 
		ZwQueryInformationProcess = (ZW_QUERY_INFORMATION_PROCESS)GetProcAddress(hModule, "ZwQueryInformationProcess"); 
		if (ZwQueryInformationProcess == NULL) 
			return false;
		PROCESS_DEBUG_PORT_INFO ProcessInfo; 
		if (STATUS_SUCCESS != ZwQueryInformationProcess(GetCurrentProcess(),
			ProcessDebugPort, &ProcessInfo, sizeof(ProcessInfo), NULL)) 
			return false;
		if(ProcessInfo.DebugPort)
			return true;
		else
			return false;
	}

	//------------------------------------------------------------------------------
	/**
	�����CloseHandle()����һ����Ч�����Ϊ���������
	���޵�����ʱ�����᷵��һ��������룬���е���������ʱ��
	���ᴥ��һ��EXCEPTION_INVALID_HANDLE (0xc0000008)���쳣
	*/
	bool CheckClosehandle()
	{
		__try
		{
			CloseHandle(HANDLE(0x00001234));
			return false;
		}
		__except(1)
		{
			return true;
		}
	}

	//---------------------------------------------------------------------------
	bool
		GetParentProcessID(DWORD* outID)
	{
		HMODULE hModule = GetModuleHandle("ntdll.dll"); 
		ZW_QUERY_INFORMATION_PROCESS ZwQueryInformationProcess; 
		ZwQueryInformationProcess = (ZW_QUERY_INFORMATION_PROCESS)GetProcAddress(hModule, "ZwQueryInformationProcess"); 
		if (ZwQueryInformationProcess == NULL) 
			return false;
		PROCESS_BASIC_INFO BasicInfo;
		if (STATUS_SUCCESS != ZwQueryInformationProcess(GetCurrentProcess(),
			ProcessBasicInfo, &BasicInfo, sizeof(BasicInfo), NULL))
			return false;
		*outID = BasicInfo.InheritedFromUniqueProcessId;
		return true;
	}

	//---------------------------------------------------------------------------
	/**
	��⸸�����쳣.
	@todo �ַ�������
	*/
	bool ParentProcessIsGood(string& sParentProcessName)
	{
		DWORD dwParentID = 0;
		if ( !GetParentProcessID(&dwParentID) )
			return true;

		HANDLE hParent = OpenProcess(PROCESS_ALL_ACCESS, TRUE, dwParentID);
		if( !hParent )
			return true;

		char szProcessName[MAX_PATH];

		bool bResult=true;

		DWORD length = GetModuleBaseName(hParent, NULL, szProcessName, MAX_PATH);
		if ( 0 == length )
			goto out;

		sParentProcessName = szProcessName;
		sParentProcessName = tolower(sParentProcessName);

		if ( sParentProcessName.find("pixwin") != string::npos ||
			 sParentProcessName.find("nvperfhud") != string::npos ||
			 sParentProcessName.find("perfclient") != string::npos )
		{
			bResult = false;
		}

out:
		CloseHandle(hParent);
		return bResult;
	}

	//------------------------------------------------------------------------------
	void ConfirmNoDebuger()
	{
		if ( !CAppConfigClient::Inst()->IsAntiDebug() )
			return;

#ifdef _WIN32
		if( CheckDebugger() )
			GenErr("��෢�ֵ�����");

		if( CheckClosehandle() )
			GenErr("CloseHandle���ֵ�����");

		if( CheckDebugPort() )
			GenErr("QueryInformationProcess���ֵ�����");

		string sParentProcessName;
		if( !ParentProcessIsGood( sParentProcessName ) )
		{
			if( sParentProcessName.empty() )
				GenErr("�����ڸ�����");

			GenErr("���ֲ�����ĸ�����",sParentProcessName);
		}
#endif // _WIN32
	}

}// namespace sqr
