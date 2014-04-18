// MapEditorView.cpp : CMapEditorView ���ʵ��
//

#include "stdafx.h"
#include "MapEditor.h"

#include "MapEditorDoc.h"
#include "MapEditorView.h"
#include ".\mapeditorview.h"
#include <Engine.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMapEditorView

IMPLEMENT_DYNCREATE(CMapEditorView, CView)

BEGIN_MESSAGE_MAP(CMapEditorView, CView)
END_MESSAGE_MAP()

// CMapEditorView ����/����

CMapEditorView::CMapEditorView()
{
	// TODO: �ڴ˴���ӹ������

}

CMapEditorView::~CMapEditorView()
{
}

BOOL CMapEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸� CREATESTRUCT cs ���޸Ĵ������
	// ��ʽ

	return CView::PreCreateWindow(cs);
}

// CMapEditorView ����

void CMapEditorView::OnDraw(CDC* /*pDC*/)
{
	CMapEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CMapEditorView ���

#ifdef _DEBUG
void CMapEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CMapEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMapEditorDoc* CMapEditorView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMapEditorDoc)));
	return (CMapEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CMapEditorView ��Ϣ�������

LRESULT CMapEditorView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���

	if ( bly::eg::CEngine::GetInstance()->WndProc(message, wParam, lParam) ) return 0;

	return CView::WindowProc(message, wParam, lParam);
}