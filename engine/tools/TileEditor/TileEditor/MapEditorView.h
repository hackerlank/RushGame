// MapEditorView.h : CMapEditorView ��Ľӿ�
//


#pragma once


class CMapEditorView : public CView
{
protected: // �������л�����
	CMapEditorView();
	DECLARE_DYNCREATE(CMapEditorView)

// ����
public:
	CMapEditorDoc* GetDocument() const;

// ����
public:

// ��д
	public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// ʵ��
public:
	virtual ~CMapEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};

#ifndef _DEBUG  // MapEditorView.cpp �ĵ��԰汾
inline CMapEditorDoc* CMapEditorView::GetDocument() const
   { return reinterpret_cast<CMapEditorDoc*>(m_pDocument); }
#endif

