#include "stdafx.h"

#include <direct.h>
#include <tchar.h>
#include <process.h>

#include "CBTUpdater_Imp.h"
#include "CAsyncUpdater.h"
#include "IBTDownloadHandler.h"
#include "IAsyncUpdateHandler.h"
#include "CPatcher.h"
#include "CHTTPDownloader.h"
#include "global.h"



NAMESPACE_BEGIN(sqr)

static bool FileExists(LPCTSTR lpszFileName);


CBTUpdater_Imp::CBTUpdater_Imp()
	: m_http(new CHTTPDownloader)
{
	m_Bittorrent = CreateDownloader(); // ���BT���������
	pThis = this;
}

CBTUpdater_Imp::~CBTUpdater_Imp()
{
	if(&m_CriticalSection)
		DeleteCriticalSection(&m_CriticalSection);

	delete m_Bittorrent;
	delete m_http;
}

int CBTUpdater_Imp::BeginDownloadPatchThread(IAsyncUpdateHandler* updater, const TCHAR* Host, const TCHAR* File, const TCHAR* szCurPath)
{
	// ��¼�����Ϣ
/*	m_strHost = string(Host);
	m_strFile = string(File);
	m_strCurPath = string(szCurPath);
	m_pAsyncUpdateHandler = updater;

	//// ��ʼ���������߳�
	unsigned int tid; // ��ʶ�̵߳�ID
	uintptr_t BTThread;
	BTThread = _beginthreadex(NULL, 0, &CBTUpdater_Imp::CBdownload_patch, this, 0, &tid); // �����������Ǵ����߳����еĺ���

	if(!BTThread)
	{
		m_pAsyncUpdateHandler->ShowMessage("����BT�����߳�ʧ��!");
		return 0;
	}
*/

	return 1;
}

// BT����������
// *** �ú����İ汾��÷�ʽ��û���޸� ***
int CBTUpdater_Imp::Download()
{
/*
	// ��ȡ���������б��ļ�YbtxPatchList.data
	m_http->Download(m_strHost, m_strFile);
	string content(m_http->GetContent());

	// ��������Ŀ¼
	TCHAR *szDownDir = "\\download";
	TCHAR szSavePath[MAX_PATH_SIZE];
	GetCurrentDirectory(MAX_PATH_SIZE, szSavePath);
	strcat(szSavePath, szDownDir);
	_mkdir(szSavePath);

	TCHAR path[1024];
	HMODULE hModule = GetModuleHandle(NULL);
	if (hModule != NULL && GetModuleFileName(hModule, path, MAX_PATH) != 0)
	{
		m_Bittorrent->AddFirewallException(path, _T("YbtxBT_Downloader"));
	}

	m_Bittorrent->InitAgent("YbtxBT_1.0");

	// �˴���Ҫ��������汾�Ž��жԱȣ���ȷ��������Ҫ�����Ǽ����汾������Ҫ���صİ汾�Žṹѹ��pl
	PatchList pl;
	CreatePatchList(pl, content);
	// û����Ҫ���µĲ���
	if(pl.size() < 1)
	{
		m_Bittorrent->Shutdown();
		//m_pAsyncUpdateHandler->CallBackFinishPatch(UPDATED_FLAG);
		return 0;
	}

	// ������ظ��汾��Patch�ļ���ÿ��Patch��Ӧһ�������ļ�
	CPatcher patcher;
	PatchList::iterator it = pl.begin();
	for(; it!=pl.end(); ++it)
	{
		HANDLE hDownloader = bt_download(it->url, szSavePath);
		// ����������ѭ���ȴ���Patch�������
		int iPercent = 0;
		if (hDownloader == NULL)
		{
			m_pAsyncUpdateHandler->ShowMessage("BT���ز�����������������");
			return 1;
		}
		else
		{
			for(;;)
			{
				iPercent = m_Bittorrent->GetProgress(hDownloader);

				if (iPercent >= 100)
					break;
				else if (iPercent >= 0)
					m_pAsyncUpdateHandler->SetDownState(iPercent);
				else
					m_pAsyncUpdateHandler->ShowMessage("BT���ؽ���С��0��");

				Sleep(40);
			}

			m_pAsyncUpdateHandler->SetDownState(iPercent);
		}

		// �������һ����������һ������
		TCHAR buffer[MAX_PATH];
		memset(buffer, 0, sizeof(buffer));
		sprintf(buffer, "%s\\%s\\patch_%d.%d.%d.ybtx", m_strCurPath.c_str(), szDownDir, it->ver.byHiVersion, it->ver.byLoVersion, it->ver.wBuildVersion);
		patcher.CheckHeadInfo(m_pAsyncUpdateHandler, buffer);
		//m_pAsyncUpdateHandler->UpdateVersion();
		break;
	}

	m_Bittorrent->Shutdown();
	//m_pAsyncUpdateHandler->CallBackFinishPatch("updating");
*/
	return 0;
}

// ���ص���BT�ļ�
HANDLE CBTUpdater_Imp::bt_download(const string& url, const string& saveas)
{
	// ����url����ȡ��host��file
	string DownUrl(url);
	int iStart = DownUrl.find_first_of(':');
	iStart = iStart + 3; // ���� ://
	int iEnd = DownUrl.find_first_of('/', iStart);

	// ������������
	m_http->Download(DownUrl.substr(iStart, iEnd-iStart), DownUrl.substr(iEnd, DownUrl.size() - iEnd));
	string content(m_http->GetContent());

	HANDLE hDownloader = m_Bittorrent->DownFromBuffer((LPBYTE)content.data(), content.size(), saveas.c_str());

	return hDownloader;
}

unsigned int __stdcall CBTUpdater_Imp::CBdownload_patch(void* pClass)
{
	CBTUpdater_Imp* pCBTUpdater_Imp = static_cast<CBTUpdater_Imp*>(pClass);

	return pCBTUpdater_Imp->Download();
}


static bool FileExists(LPCTSTR lpszFileName)
{
	DWORD dwAttributes = GetFileAttributes(lpszFileName);

	if ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
		return true;
	else
		return false;
}


NAMESPACE_END

