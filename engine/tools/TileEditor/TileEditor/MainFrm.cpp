// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "MapEditor.h"

#include "MainFrm.h"
#include ".\mainfrm.h"

#include <Engine.h>
#include <SharedTexture.h>
#include <CTileEditApp.h>
#include <WireBox.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
//	ON_COMMAND(ID_VIEW_HIDE, OnViewHide)
	ON_COMMAND(ID_EXPORTASCLIENT, OnExportasclient)
	ON_COMMAND(ID_EXPORTASSERVER, OnExportasserver)
	ON_WM_CLOSE()
	ON_COMMAND(ID_SV, OnSv)
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
}

CMainFrame::~CMainFrame()
{
}

void CMainFrame::InitMapEditor(DWORD dwWidth, DWORD dwDepth)
{
	rd::CRenderDevice * pRender = rd::CRenderDevice::GetInstance();
	
	bly::CTerrainMesh * pTerrain;
	
	// terrain
	DWORD dwTextureHandleDefault = CSharedTexture::GetTexture("..\\Tools\\TileEditor\\Res\\DefaultForTileEditor.tga")->GetTextureHandle();
	pTerrain = CSceneManager::GetInstance()->AddNodeTerrainMesh(dwWidth,dwDepth,dwTextureHandleDefault);
	
	// XFile
	CTileEditApp::GetInstance()->SetTerrainMesh(pTerrain);
	
	D3DXVECTOR3 vec;
	vec.x = CTileEditApp::GetInstance()->GetTerrainMesh()->GetWidth()/2.0f * GRID_SPACE;
	vec.y = 0.0f;
	vec.z = CTileEditApp::GetInstance()->GetTerrainMesh()->GetDepth()/2.0f * GRID_SPACE;
	eg::CEngine::GetInstance()->GetActiveCamera()->MoveToRTSByView(vec);
	
	// reload operator
	CTileEditApp::GetInstance()->GetOperator()->InitProcessParams();
	
	// start timer
	ut::CTimer::GetInstance()->Start();
	ut::CTimer::GetInstance()->ResumeUpdate();
	
	//////////////////////////////////////////////////////////////////////////
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	
	//if (!m_wndStatusBar.Create(this) ||
	//	!m_wndStatusBar.SetIndicators(indicators,
	//	  sizeof(indicators)/sizeof(UINT)))
	//{
	//	TRACE0("δ�ܴ���״̬��\n");
	//	return -1;      // δ�ܴ���
	//}
	// TODO: �������Ҫ��������ͣ������ɾ��������
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	
	if(!m_wndEditBar.Create( this, IDD_PANDLG, WS_CHILD|CBRS_RIGHT,IDD_PANDLG ) )
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	
	if(!m_wndEditPan.Create(WS_CHILD|TCS_SINGLELINE,CRect(5,5,200,400),&m_wndEditBar,1000))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	
	m_wndEditPan.AddToList(&m_resSetView,0," ");
	m_wndEditPan.SetMinTabWidth(64);
	
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


void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);
	
	CRect rect;
	if(m_wndEditBar.GetSafeHwnd()&&m_wndEditPan.GetSafeHwnd())
	{
		m_wndEditBar.GetClientRect(&rect);
		m_wndEditPan.SetWindowPos(NULL,0,0,rect.Width(),rect.Height(),SWP_NOZORDER);
	}
	
	// TODO: Add your message handler code here
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	if (pContext != NULL && pContext->m_pNewViewClass != NULL)
	{
		if ( ( m_worldView = CreateView(pContext, AFX_IDW_PANE_FIRST) ) == NULL)
			return FALSE;
	}
	return TRUE;

}

void CMainFrame::UpdateCamera()
{
	RECT rc;
	this->GetWorldView()->GetClientRect(&rc);
	float f = float(rc.right-rc.left)/float(rc.bottom - rc.top);
	eg::CEngine::GetInstance()->GetActiveCamera()->SetAspect(f);
}

void CMainFrame::OnViewHide()
{
//	this->SetHide();
	
//	this->UpdateCamera();

	// TODO: �ڴ���������������
}

void CMainFrame::OnExportasclient()
{
	std::string str = this->GetActiveDocument()->GetTitle();
	CTileEditApp::GetInstance()->GetTerrainMesh()->SaveAsClient(str.c_str());
}

void CMainFrame::OnExportasserver()
{
	std::string str = this->GetActiveDocument()->GetTitle();
	CTileEditApp::GetInstance()->GetTerrainMesh()->SaveAsServer(str.c_str());
}

void CMainFrame::OnClose()
{
	//int i  = ::MessageBox(NULL,"�Ƿ񱣴浱ǰ�༭ͼ�ذ�?","ϵͳ��ʾ",MB_OKCANCEL);
	//
	//if ( i == IDOK )
	//{
	//	this->OnSv();
	//}
	
	CFrameWnd::OnClose();
}
void CMainFrame::OnSv()
{
	m_resSetView.OnSv();
}
