// AreaEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ScriptMgr.h"
#include "AreaEditDlg.h"


// CAreaEditDlg dialog

IMPLEMENT_DYNAMIC(CAreaEditDlg, CDialog)

CAreaEditDlg::CAreaEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAreaEditDlg::IDD, pParent)
{

}

CAreaEditDlg::~CAreaEditDlg()
{
}

void CAreaEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_comboMusic);
	DDX_Control(pDX, IDC_AREA_STATE, m_listCtrlAreaState);
	DDX_Control(pDX, IDC_EDIT1, m_editAreaName);
	DDX_Control(pDX, IDC_IS_FB, m_comboxIsFb);
	DDX_Control(pDX, IDC_PLAYER_NUM, m_editMaxPlayerNum);
	DDX_Control(pDX, IDC_MAX_PLAYER_NUM, m_editRatingPlayerNum);
}


BEGIN_MESSAGE_MAP(CAreaEditDlg, CDialog)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_AREA_STATE, &CAreaEditDlg::OnLvnItemchangedAreaState)
	ON_BN_CLICKED(IDC_MODIFY, &CAreaEditDlg::OnBnClickedModify)
	ON_BN_CLICKED(IDC_DELETE_AREA, &CAreaEditDlg::OnBnClickedDeleteArea)
	ON_CBN_SELCHANGE(IDC_IS_FB, &CAreaEditDlg::OnCbnSelchangeIsFb)
END_MESSAGE_MAP()


// CAreaEditDlg message handlers

BOOL CAreaEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_comboMusic.AddString("��");
	for (uint i = 0; i < g_pMainDlg->m_vecMusicNames.size(); i ++)
	{
		m_comboMusic.AddString( g_pMainDlg->m_vecMusicNames[i]);
	}
	m_comboMusic.SetCurSel(0);
	CString NameList[5] = {"������","����","����","�������","�����"};
	CWnd*  WndList[5] = {
		static_cast<CWnd*>(&m_editAreaName)
		,static_cast<CWnd*>(&m_comboMusic)
		,static_cast<CWnd*>(&m_comboxIsFb)
		,static_cast<CWnd*>(&m_editMaxPlayerNum)
		,static_cast<CWnd*>(&m_editRatingPlayerNum)
	};
	CRect rt;
	for (int i = 0; i < sizeof(WndList)/sizeof(CWnd*); ++i)
	{
		m_listCtrlAreaState.InsertColumn(i,NameList[i]);
		WndList[i]->GetWindowRect(&rt);
		m_listCtrlAreaState.SetColumnWidth(i, rt.Width() + 4);
	}
	CCache::AreaData_Iter iter = g_pCache->m_mapAreaData.begin();
	int index = 0;
	PAreaData pAreaData;
	CString strPlayerNum; 
	for(; iter != g_pCache->m_mapAreaData.end(); iter++)
	{	
		pAreaData = iter->second;
		m_listCtrlAreaState.InsertItem(index,  pAreaData->name);
		m_listCtrlAreaState.SetItemText(index,1, pAreaData->music);
		m_listCtrlAreaState.SetItemText(index,2, pAreaData->isFb ? "��" : "��");
		if (pAreaData->isFb)
		{
			strPlayerNum.Format("%d",pAreaData->maxPlayerNum);
			m_listCtrlAreaState.SetItemText(index,3, strPlayerNum);
		}
		else
		{
			m_listCtrlAreaState.SetItemText(index,3, "");
		}
		if(pAreaData->ratingPlayerNum > 0)
		{
			strPlayerNum.Format("%d",pAreaData->ratingPlayerNum);
			m_listCtrlAreaState.SetItemText(index,4, strPlayerNum);
		}
		else
		{
			m_listCtrlAreaState.SetItemText(index,4, "");
		}
		index ++;
	}
	m_comboxIsFb.SetCurSel(0);
	OnCbnSelchangeIsFb();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CAreaEditDlg::OnLvnItemchangedAreaState(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	int index = pNMLV->iItem; 
	if (index >= 0 && index < m_listCtrlAreaState.GetItemCount())
	{
		PAreaData pAreaData = g_pCache->GetAreaData(m_listCtrlAreaState.GetItemText(index,0));
		m_editAreaName.SetWindowText(pAreaData->name);
		m_comboMusic.SelectString(-1, pAreaData->music);
		m_comboxIsFb.SelectString(-1, pAreaData->isFb ? "��" : "��");
		CString strPlayerNum;
		if (pAreaData->isFb)
		{
			strPlayerNum.Format("%d",pAreaData->maxPlayerNum);
			m_editMaxPlayerNum.SetWindowText(strPlayerNum);
		}
		else
		{
			m_editMaxPlayerNum.SetWindowText("");
		}
		if(pAreaData->ratingPlayerNum > 0)
		{
			strPlayerNum.Format("%d",pAreaData->ratingPlayerNum);
			m_editRatingPlayerNum.SetWindowText(strPlayerNum);
		}
		else
		{
			m_editRatingPlayerNum.SetWindowText("");
		}

		OnCbnSelchangeIsFb();
	}
	*pResult = 0;
}

void CAreaEditDlg::OnBnClickedModify()
{
	// TODO: Add your control notification handler code here
	int index = m_listCtrlAreaState.GetSelectionMark();
	if(index < 0)
	{
		MessageBox("��ѡ��һ��Ҫ�޸ĵ�����.");
		return;
	}
	CString newAreaName; 
	m_editAreaName.GetWindowText(newAreaName);
	if (newAreaName == "")
	{
		MessageBox("����д������.");
		return;
	}
	if (newAreaName != "��·" && newAreaName.Find("��·") >= 0)
	{
		MessageBox("���������ܺ� ��·, ", MB_OK);
		return;
	}
	CString oldAreaName = m_listCtrlAreaState.GetItemText(index,0);
	if (newAreaName != oldAreaName)
	{
		if(g_pCache->GetAreaData(newAreaName))
		{
			MessageBox("��д���������Ѿ�����.");
			return;
		}
		if ( ! g_pMainDlg->AreaRename(oldAreaName, newAreaName))
		{
			return;
		}
	}

	CString musicName;
	m_comboMusic.GetWindowText(musicName);
	if( musicName == "")
	{
		musicName = "��";
	}
	g_pCache->SetAreaMusic(newAreaName, musicName);
	

	CString isFB, maxPlayerNum, ratingPlayerNum;
	int num;
	m_comboxIsFb.GetWindowText(isFB);
	if (isFB == "��")
	{
		m_editMaxPlayerNum.GetWindowText(maxPlayerNum);
		if (maxPlayerNum == "")
		{
			MessageBox("��������");
			return;
		}
		num = atoi(maxPlayerNum.GetBuffer());
		if (num < 1)
		{
			MessageBox("�����������1");
			return;
		}
		g_pCache->SetAreaFbState(newAreaName, true);
		g_pCache->SetAreaMaxPlayerNum(newAreaName, num);
	}
	else
	{
		g_pCache->SetAreaFbState(newAreaName, false);
		g_pCache->SetAreaMaxPlayerNum(newAreaName, 0);
	}
	
	m_editRatingPlayerNum.GetWindowText(ratingPlayerNum);
	if(ratingPlayerNum == "")
	{
		ratingPlayerNum = "0";
	}

	num = atoi(ratingPlayerNum.GetBuffer());
	g_pCache->SetAreaRatingPlayerNum(newAreaName, num);

	m_listCtrlAreaState.SetItemText(index,0, newAreaName);
	m_listCtrlAreaState.SetItemText(index,1, musicName);
	m_listCtrlAreaState.SetItemText(index,2, isFB);
	if (isFB == "��")
	{
		m_listCtrlAreaState.SetItemText(index,3, maxPlayerNum);
	}
	else
	{
		m_listCtrlAreaState.SetItemText(index,3, "");
	}
	if(ratingPlayerNum == "0")
	{
		ratingPlayerNum = "";
	}
	m_listCtrlAreaState.SetItemText(index,4, ratingPlayerNum);
	
}

void CAreaEditDlg::OnBnClickedDeleteArea()
{
	// TODO: Add your control notification handler code here
	int index = m_listCtrlAreaState.GetSelectionMark();
	if(index < 0)
	{
		MessageBox("��ѡ��һ��Ҫ�޸ĵ�����.");
		return;
	}
	CString areaName = m_listCtrlAreaState.GetItemText(index,0);
	CString msg = "ȷ��Ҫɾ������ '" + areaName + "' ��? ɾ���ᶪʧ����������ص���Ϣ.";
	if(MessageBox(msg, "ȷ��ɾ��", MB_OKCANCEL) == IDOK)
	{
		if( ! g_pMainDlg->DelArea(areaName))
		{
			return;
		}
		m_listCtrlAreaState.DeleteItem(index);
	}
}
void CAreaEditDlg::OnCbnSelchangeIsFb()
{
	CString strIsFb;
	m_comboxIsFb.GetWindowText(strIsFb);
	if (strIsFb == "��")
	{
		m_editMaxPlayerNum.EnableWindow(true);

	}
	else
	{
		m_editMaxPlayerNum.EnableWindow(false);
		m_editMaxPlayerNum.SetWindowText("");
	}
}
