// GUIEditor.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "GUIEditor.h"
#include "GUIEditorDlg.h"
#include ".\guieditor.h"
#include "PathBrowser\CEditorConfig.h"
//#include "../src/gui/SQRGUIManager.h"

const wstring PATH_ALIAS_GUIEDITOR = L"";

BEGIN_MESSAGE_MAP(CGUIEditorApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CGUIEditorApp ����

CGUIEditorApp::CGUIEditorApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
	EnableCatchError(true);
}


// Ψһ��һ�� CGUIEditorApp ����

CGUIEditorApp theApp;


// CGUIEditorApp ��ʼ��

BOOL CGUIEditorApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControls()�����򣬽��޷��������ڡ�
	InitCommonControls();

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

	//m_MainDlg.Create(CGUIEditorDlg::GetDialogID(), CWnd::GetDesktopWindow() );
	CGUIEditorDlg* pDlg = new CGUIEditorDlg();
	pDlg->Create( IDD_GUIEDITOR_DIALOG_1024, ::CWnd::GetDesktopWindow() );//���ε�ѡȡ1024��800�Ի���
	m_pMainWnd = pDlg;

	::SetCursor( ::LoadCursor( NULL, IDC_ARROW ) );
	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	// ����������Ӧ�ó������Ϣ�á�
	return TRUE;
}

//------------------------------------------------------------------------------
CGUIEditorApp::~CGUIEditorApp()
{
	SafeDelete(m_pMainWnd);
}