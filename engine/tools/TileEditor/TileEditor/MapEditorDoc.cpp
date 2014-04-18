// MapEditorDoc.cpp :  CMapEditorDoc ���ʵ��
//

#include "stdafx.h"
#include "MapEditor.h"
#include <CTileEditApp.h>
#include "MapEditorDoc.h"
#include ".\mapeditordoc.h"
#include <Engine.h>
#include <SharedTexture.h>
#include <WireBox.h>
#include "NewMapDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMapEditorDoc

IMPLEMENT_DYNCREATE(CMapEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CMapEditorDoc, CDocument)
END_MESSAGE_MAP()


// CMapEditorDoc ����/����

CMapEditorDoc::CMapEditorDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CMapEditorDoc::~CMapEditorDoc()
{
}

BOOL CMapEditorDoc::OnNewDocument()
{
	CString s = this->GetTitle();
	if ( this->GetTitle() != "�ޱ���" && this->GetTitle() != "Untitled" )
		CTileEditApp::GetInstance()->GetTerrainMesh()->SaveAsLocal((const char*)this->GetTitle());

	if (!CDocument::OnNewDocument())
		return FALSE;

	static bool bFirst = true;
	
	if ( !bFirst )
	{
		CNewMapDialog d;
		d.DoModal();
		this->SetTitle("������");
	}
	
	if ( bFirst ) bFirst = false;
	
	return TRUE;
}

// CMapEditorDoc ���л�

void CMapEditorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
	}
}


// CMapEditorDoc ���

#ifdef _DEBUG
void CMapEditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMapEditorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMapEditorDoc ����

BOOL CMapEditorDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if ( this->GetTitle() != "�ޱ���" && this->GetTitle() != "Untitled" )
		CTileEditApp::GetInstance()->GetTerrainMesh()->SaveAsLocal((const char*)this->GetTitle());
	
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
		
	CTileEditApp::GetInstance()->GetTerrainMesh()->LoadAsLocal(lpszPathName);
	
	// TODO:  �ڴ������ר�õĴ�������
	
	return TRUE;
}

BOOL CMapEditorDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	CTileEditApp::GetInstance()->GetTerrainMesh()->SaveAsLocal(lpszPathName);
	return true;
}
