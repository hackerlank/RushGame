// SetHeightView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MapEditor.h"
#include "SetHeightView.h"
#include ".\setheightview.h"


// CSetHeightView

CSetHeightView::CSetHeightView()
	: CFormView(CSetHeightView::IDD)
{
}

CSetHeightView::~CSetHeightView()
{
}

void CSetHeightView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSetHeightView, CFormView)
	ON_BN_CLICKED(IDC_GAUSS, OnBnClickedGauss)
	ON_BN_CLICKED(IDC_LINEAR, OnBnClickedLinear)
END_MESSAGE_MAP()

// CSetHeightView ���

#ifdef _DEBUG
void CSetHeightView::AssertValid() const
{
	CFormView::AssertValid();
}

void CSetHeightView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


// CSetHeightView ��Ϣ�������

void CSetHeightView::PreSubclassWindow()
{
	// TODO: �ڴ����ר�ô����/����û���
//	radios.CheckRadio(this,IDC_LINEAR, IDC_GAUSS, IDC_GAUSS);
    CFormView::PreSubclassWindow();
}

BOOL CSetHeightView::OnCmdMsg(UINT nID, int nCode, void* pExtra,
							  AFX_CMDHANDLERINFO* pHandlerInfo)
{
//	m_radios.CheckRadio( this, nID );
	return CFormView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}		

void CSetHeightView::OnBnClickedGauss()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CSetHeightView::OnBnClickedLinear()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}