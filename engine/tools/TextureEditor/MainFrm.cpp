// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "MainFrm.h"
#include "TextureEditor.h"
#include "FileTree.h"
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_COMMAND(ID_ALPHA_GEN, &CMainFrame::OnAlphaGen)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ״̬��ָʾ��
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
	m_xSize=200;
	m_ySize=200;	
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}
	// TODO: �������Ҫ��������ͣ������ɾ��������
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸� CREATESTRUCT cs ���޸Ĵ������
	// ��ʽ

	return TRUE;
}

// CMainFrame ��Ϣ�������
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	
	if (!m_wndSplitter.CreateStatic(this,1,2))
		return 0;
	if (!m_wndSplitter.CreateView(0,0,RUNTIME_CLASS(CFileTreeView),CSize(m_xSize,0),pContext))
		return 0;

	if (!m_wndSplitter2.CreateStatic(&m_wndSplitter,2, 1,
		WS_CHILD|WS_VISIBLE,m_wndSplitter.IdFromRowCol(0,1)))
		return 0;
	
	RECT rect;
	GetClientRect(&rect);
	int size=max(rect.bottom-rect.top-m_ySize,50);
	if (!m_wndSplitter2.CreateView(0,0,pContext->m_pNewViewClass,CSize(0, size),pContext))
		return 0;
	if (!m_wndSplitter2.CreateView(1,0,RUNTIME_CLASS(CFileListView),CSize(0,m_xSize),pContext))
		return 0;

	m_DrawView=(CView*)m_wndSplitter2.GetPane(0,0);
	CFileTreeView* tree=(CFileTreeView*)m_wndSplitter.GetPane(0,0);
	CFileListView* list=(CFileListView*)m_wndSplitter2.GetPane(1,0);
	list->SetFileIcon( "tex" );
	list->SetFileIcon( "bmp" );
	list->SetFileIcon( "tga" );
	list->SetFileIcon( "jpg" );
	if(tree)
		list->SetFileList(*tree);
	
	return 1;
}

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	if(m_wndSplitter2.m_hWnd)
		m_wndSplitter2.SetRowInfo(0,cy-m_ySize,0);
	CFrameWnd::OnSize(nType, cx, cy);	
}

void CMainFrame::OnSizing(UINT fwSide, LPRECT pRect)
{
	int y,my;
	RECT rect;
	m_wndSplitter2.GetRowInfo(0,y,my);
	GetClientRect(&rect);
	m_ySize=rect.bottom-rect.top-y;

	CFrameWnd::OnSizing(fwSide, pRect);
}

void CMainFrame::OnAlphaGen()
{
	// TODO: Add your command handler code here
	CDirSelectWnd dialog;
	string dir = dialog.GetDirector("", "��ѡ��ARGB��ʽtga�ļ��ı���Ŀ¼").GetBuffer();
	if (dir.empty())
		return;
	struct is_invalid
	{
		bool operator()(TCHAR c)
		{
			return (c > 0x80) || (c == ' ');
		}
	};
	size_t isInvalid = std::count_if(dir.begin(), dir.end(), is_invalid());	
	if (isInvalid)
	{
		MessageBox("·���в��ܰ��������ַ��Ϳո�", NULL, 0);
		return;
	}
	string command = "TexAlphaGen.bat ";
	command += dir;
	system(command.c_str());
}
