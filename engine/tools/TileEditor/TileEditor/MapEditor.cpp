// MapEditor.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "MapEditor.h"
#include "MainFrm.h"

#include "MapEditorDoc.h"
#include "MapEditorView.h"
#include <Engine.h>
#include <SharedTexture.h>
#include <CTileEditApp.h>
#include <WireBox.h>
#include "NewMapDialog.h"
#include ".\mapeditor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMapEditorApp

BEGIN_MESSAGE_MAP(CMapEditorApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

// CMapEditorApp ����

CMapEditorApp::CMapEditorApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

CMapEditorApp::~CMapEditorApp()
{
	eg::CEngine::GetInstance()->Destroy();
}

// Ψһ��һ�� CMapEditorApp ����

CMapEditorApp theApp;

// CMapEditorApp ��ʼ��

BOOL CMapEditorApp::InitInstance()
{
	std::locale::global(std::locale(""));
	::SetCurrentDirectory("..\\..\\data");

	CWinApp::InitInstance();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)
	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CMapEditorDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ��ܴ���
		RUNTIME_CLASS(CMapEditorView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);
	// ������׼������DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// ��������������ָ����������
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();
//	m_pMainWnd->SetWindowText("Tang Map Editor Version .01 Powered by Bluely Engine");
	// �������ں�׺ʱ�ŵ��� DragAcceptFiles��
	//  �� SDI Ӧ�ó����У���Ӧ�� ProcessShellCommand  ֮����
	
	/////////////////////////////////////////////////////////////////////////
	eg::CEngine::GetInstance()->Create(((CMainFrame*)m_pMainWnd)->GetWorldView()->GetSafeHwnd(), m_pMainWnd->GetSafeHwnd(), ((CMainFrame*)m_pMainWnd)->GetBackBufferRect(),false,false);
	
	bly::CCamera * pCamera;
	bly::COperator * pOperator;
		
	// camera must be first
	pCamera = CSceneManager::GetInstance()->AddNodeCamera(STDFOV, STDPITCH, STDYAW, 50,10000,D3DXVECTOR3(2880.0f,2880.0f,0.0f), D3DXVECTOR3(0.0f,0.0f,480.0f));
	//pCamera->MoveToRTSByView(D3DXVECTOR3(REGION_SPACE/2.0f*64.0f,0.0f,REGION_SPACE/2.0f*64.0f));
	eg::CEngine::GetInstance()->SetActiveCamera(pCamera);
	
	// operator
	pOperator = CSceneManager::GetInstance()->AddNodeOperator(
		((CMainFrame*)(m_pMainWnd))->GetResSetView()->GetSafeHwnd());
	
	// XFile
	CTileEditApp::GetInstance()->SetOperator(pOperator);

	((CMainFrame*)m_pMainWnd)->InitMapEditor(1,1);
		
	((CMainFrame*)m_pMainWnd)->UpdateCamera();
	
	((CMainFrame*)m_pMainWnd)->GetResSetView()->AddEffectSetFromFile("..\\..\\data\\fx\\setting\\mapfx_local.are");
	((CMainFrame*)m_pMainWnd)->GetResSetView()->LoadLog();
			
	return TRUE;
}



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnViewStatusBar();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// �������жԻ����Ӧ�ó�������
void CMapEditorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CMapEditorApp ��Ϣ�������

void CMapEditorApp::BackToWorldView()
{
	((CMainFrame*)m_pMainWnd)->GetWorldView()->SetFocus();
}

BOOL CMapEditorApp::OnIdle(LONG lCount)
{
	if( ((CMainFrame*)m_pMainWnd)->GetWorldView() )
	{
		if ( !ut::CTimer::GetInstance()->IsStop() )
		{
			while ( ut::CTimer::GetInstance()->IsOverEF() ) 
			{
				this->Update();
				ut::CTimer::GetInstance()->Down();
			}
			ut::CFPSCounter::GetInstance()->RegisterFPS();
			this->Render();
			ut::CTimer::GetInstance()->Clear();
		}
	}
	CWinApp::OnIdle( lCount );
	return 1;
}

void CMapEditorApp::Render()
{
	eg::CEngine::GetInstance()->Render();
}

void CMapEditorApp::Update()
{
	eg::CEngine::GetInstance()->Update();
}