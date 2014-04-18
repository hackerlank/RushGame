// MainFrm.h : CMainFrame ��Ľӿ�
//


#pragma once
class CMainFrame : public CFrameWnd
{	
protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// ����
public:

// ����
public:
	CView* GetShowView(){ return m_DrawView; }

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CMainFrame();

protected:  // �ؼ���Ƕ���Ա
	CStatusBar   m_wndStatusBar;
	CToolBar     m_wndToolBar;
	CSplitterWnd m_wndSplitter;
	CSplitterWnd m_wndSplitter2;

	int         m_xSize;
	int         m_ySize;

	CView*      m_DrawView;

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnAlphaGen();
};


