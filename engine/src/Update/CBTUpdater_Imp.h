#pragma once

#include "global.h"
#include "BaseTypes.h"
#include "HeadDataType.h"
#include "CAsyncUpdater.h"
#include "IBTDownloadHandler.h"



NAMESPACE_BEGIN(sqr)

class IAsyncUpdateHandler;
class CHTTPDownloader;

class CBTUpdater_Imp
{
public:
	CBTUpdater_Imp();
	~CBTUpdater_Imp();

	int Download();
	HANDLE bt_download(const string& url, const string& saveas); // ���ص���BT�ļ�

	// BT����
	static unsigned int __stdcall CBdownload_patch(void* pClass);
	int BeginDownloadPatchThread(IAsyncUpdateHandler* updater, const TCHAR* Host, const TCHAR* File, const TCHAR* szCurPath);

	// ���ݴ��ڿռ�ص���
	IAsyncUpdateHandler*  m_pAsyncUpdateHandler;
	IBTDownloadHandler*   m_Bittorrent;
	CHTTPDownloader*      m_http;

private:
	string  m_strHost;         // ���ط�����·��
	string  m_strFile;         // �����ļ���
	string  m_strCurPath;      // ��ǰ·��

	string  m_strTorrentUrl;   // ��ǰ������������
	string  m_strDstPath;      // Ŀ��·��
	string  m_strSaveAs;       // ���·��
	string  m_strDownloadPath; // ����·��

	//seed_style      m_seed_style;
	PatchList       m_downloaded_packet;
	string          m_strBtOption;

	CRITICAL_SECTION   m_CriticalSection;
	FILE*              m_logfile;
};

static CBTUpdater_Imp *pThis;

NAMESPACE_END

