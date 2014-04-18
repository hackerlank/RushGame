// TextureEditorView.h : CTextureEditorView ��Ľӿ�
//

#pragma once


class CTextureEditorView : public CView, UpdateRes
{
protected: // �������л�����
	CTextureEditorView();
	DECLARE_DYNCREATE(CTextureEditorView)

// ����
public:
	CTextureEditorDoc* CTextureEditorView::GetDocument() const
   { return reinterpret_cast<CTextureEditorDoc*>(m_pDocument); }

   CD3d       m_d3d;
   CMenu      m_Menu;
   CWnd       m_show;
   CPoint     m_Start;
   CPoint     m_IniP;
   LPVERBUF   m_pVB;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC){};  // ��д�Ի��Ƹ���ͼ
	void Draw();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual LRESULT RestoreRes();
	virtual LRESULT ReleaseRes();
protected:

// ʵ��
public:
	virtual ~CTextureEditorView();

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnTimer(UINT nIDEvent);
};

