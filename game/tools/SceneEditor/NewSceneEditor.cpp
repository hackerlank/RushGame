// NewSceneEditor.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "CVector3.h"
#include "ScriptMgr.h"
#include "NewSceneEditor.h"
#include "NewSceneEditorDlg.h"
#include "IAudioModule.h"
#include "IAudioSystem.h"
#include "ModeChoiceDlg.h"
#include "SEPathMgr.h"
#include "CPkgFile.h"
#include "CodeCvs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CNewSceneEditorApp

BEGIN_MESSAGE_MAP(CNewSceneEditorApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

const wstring PATH_ALIAS_SCENE_EDITOR = L"res";

// CNewSceneEditorApp ����

CNewSceneEditorApp::CNewSceneEditorApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CNewSceneEditorApp ����

CNewSceneEditorApp theApp;


// CNewSceneEditorApp ��ʼ��


BOOL CNewSceneEditorApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	char curPath[1024];
	::GetCurrentDirectory(1024,curPath);
	CString str(curPath);
	CPkgFile::AddLoadPath(PATH_ALIAS_SCENE_EDITOR.c_str(), gbk_to_utf16((LPCSTR)str).c_str());

	str = CSEPathMgr::Inst().GetArtistPath();
	CPkgFile::AddLoadPath(PATH_ALIAS_SCENE_EDITOR.c_str(), gbk_to_utf16((LPCSTR)str).c_str());
	
	CModeChoiceDlg modeDlg;
	if (modeDlg.DoModal() == IDOK)
	{
		Init();
		g_eModeType = modeDlg.GetMode();
		CNewSceneEditorDlg dlg;
		m_pMainWnd = &dlg;
		INT_PTR nResponse = dlg.DoModal();
		if (nResponse == IDOK)
		{
			// TODO: �ڴ˴����ô����ʱ�á�ȷ�������ر�
			//  �Ի���Ĵ���
		}
		else if (nResponse == IDCANCEL)
		{
			// TODO: �ڴ˷��ô����ʱ�á�ȡ�������ر�
			//  �Ի���Ĵ���
		}
		ReleaseGlobalPtr();
		GetAudioModule()->GetAudioSystem()->Close();
	}
	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}
