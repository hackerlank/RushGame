// DynamicFileSel.cpp : implementation file
//

#include "stdafx.h"
#include "ScriptMgr.h"
#include "DynamicFileSel.h"
#include "SEPathMgr.h"
#include "CodeCvs.h"


// CDynamicFileSel dialog

IMPLEMENT_DYNAMIC(CDynamicFileSel, CDialog)

CDynamicFileSel::CDynamicFileSel(CWnd* pParent /*=NULL*/)
	: CDialog(CDynamicFileSel::IDD, pParent)
{

}

CDynamicFileSel::~CDynamicFileSel()
{
}

void CDynamicFileSel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST3, m_listName);
}


BEGIN_MESSAGE_MAP(CDynamicFileSel, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CDynamicFileSel::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDynamicFileSel::OnBnClickedButton2)
	ON_LBN_DBLCLK(IDC_LIST3, &CDynamicFileSel::OnLbnDblclkList3)
	ON_BN_CLICKED(IDC_BUTTON3, &CDynamicFileSel::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CDynamicFileSel::OnBnClickedButton4)
//	ON_BN_CLICKED(IDC_BUTTON5, &CDynamicFileSel::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CDynamicFileSel::OnBnClickedButton6)
END_MESSAGE_MAP()


// CDynamicFileSel message handlers

void CDynamicFileSel::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	if(g_eModeType == eMT_DYNAMIC && g_pCache->GetCurEditDynamicName() != "")
	{
		int result = MessageBox("��ǰ���ڱ༭ " + g_pCache->GetCurEditDynamicName() + "  �Ƿ񱣴�,�л��󽫶�ʧ�Ķ�����Ϣ", "��ʾ!!", MB_YESNOCANCEL);
		if (result == IDYES)
		{
			g_pCache->SaveDynamicFile();
		}
		else if(result == IDCANCEL)
		{
			return;
		}
	}

	int nIndex = m_listName.GetCurSel();
	if (nIndex != LB_ERR)
	{
		m_listName.GetText(nIndex,m_strSelName);
	}
	else
	{
		m_strSelName = "";
	}
	OnOK();
}

BOOL CDynamicFileSel::OnInitDialog()
{
	CDialog::OnInitDialog();
		
	UpdateNames();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}




//������õ�
CString CDynamicFileSel::GetSelName()
{
	return m_strSelName;
}
void CDynamicFileSel::OnBnClickedButton2()
{
	CString str ;
	GetDlgItem(IDC_NEWNAME)->GetWindowText(str);
	if (str == "")
	{
		MessageBox("���ֲ���Ϊ��");
		return;
	}
	char *ch = str.GetBuffer();
	for(int i=0; i<str.GetLength(); i++)
	{
		if(!( (ch[i]>='a'&&ch[i]<='z')||(ch[i]>='A'&&ch[i]<='Z')||(ch[i]>='0'&&ch[i]<='9') ))
		{
			MessageBox("����ֻ������ĸ���������");
			return;
		}
	}
	if(g_pCache->IsDynamicName(str) || g_pCache->IsDynamicFileName(str))
	{
		MessageBox("�Ѿ�������Ϊ " + str + " ���ļ�");
		return;
	}
	m_strSelName = str;
	if(g_eModeType == eMT_PATH_EDIT)
	{
		g_pCache->Exp_AddDynamicFile(str.GetBuffer(), "");
	}
	else
	{
		g_pCache->Exp_AddDynamicFile(str.GetBuffer(), str.GetBuffer());
	}
	m_listName.AddString(str);
}

void CDynamicFileSel::OnLbnDblclkList3()
{
	// TODO: Add your control notification handler code here
	OnBnClickedButton1();
}

void CDynamicFileSel::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	CString selName ;
	int nIndex = m_listName.GetCurSel();
	if (nIndex != LB_ERR)
	{
		m_listName.GetText(nIndex,selName);
	}
	
	if(IDYES != MessageBox(  "ȷ��Ҫɾ�� " + selName + " ��?", "��ʾ", MB_YESNO))
	{
		return;
	}
	CString fileName;
	g_pCache->GetDynamicFileName(selName, fileName);

	g_pCache->RemoveDynamicName(selName);
	m_listName.DeleteString(nIndex);
	if(g_eModeType == eMT_DYNAMIC) //ɾ��ԭ���ļ�����, ���ⱻ����
	{
		CString filePath = CSEPathMgr::Inst().GetDesignerPath() + "/scene/dynamic_file/" + fileName + ".lua";
		FILE* pFile  = fopen(filePath.GetBuffer(), "r");
		if(pFile) 
		{
			fclose(pFile);
			pFile  = fopen(filePath.GetBuffer(), "wb");
			if(pFile)
			{
				TCHAR szSave[CHAT_BUFFER_SIZE];
				strcpy(szSave, "");
				std::string tString = gbk_to_utf8(szSave);
				int length = tString.length();
				for(int i = 0; i < length; i++)
				{
					szSave[i] = tString[i];
				}
				szSave[length] = '\0';
				fwrite(szSave, strlen(szSave), 1, pFile);
				fclose(pFile);
			}
			filePath = CSEPathMgr::Inst().GetDesignerPath() + "/scene/dynamic_file/DynamicFileList.lua";
			pFile = fopen(filePath.GetBuffer(), "wb");
			if (pFile)
			{
				g_pCache->SaveDynamicFileList(pFile);
			}
		}

	}
	if (selName == g_pCache->GetCurEditDynamicName() && g_eModeType == eMT_DYNAMIC)
	{
		g_pCache->SetCurEditDynamicName("");
	}
}

void CDynamicFileSel::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
	CString newName ;
	GetDlgItem(IDC_NEWNAME)->GetWindowText(newName);
	if (newName == "")
	{
		MessageBox("���ֲ���Ϊ��");
		return;
	}
	else if(g_pCache-> IsDynamicName(newName))
	{
		MessageBox("�Ѿ�������Ϊ " + newName + " ���ļ�");
		return;
	}
	int nIndex = m_listName.GetCurSel();
	CString selName;
	if (nIndex == LB_ERR)
	{
		MessageBox("����ѡ��һ��Ҫ�޸ĵ���");
		return;
	}
	else
	{
		m_listName.GetText(nIndex,selName);
	}
	if(IDYES == MessageBox(  "ȷ��Ҫ�� " + selName + " �޸ĳ�" + newName + " ��?", "��ʾ", MB_YESNO))
	{
		CString fileName;
		g_pCache->GetDynamicFileName(selName, fileName);
		g_pCache->RemoveDynamicName(selName);
		m_listName.DeleteString(nIndex);

		g_pCache->Exp_AddDynamicFile(newName, fileName);
		m_listName.AddString(newName);
		if (g_eModeType == eMT_DYNAMIC)
		{
			if (selName == g_pCache->GetCurEditDynamicName())
			{
				g_pCache->SetCurEditDynamicName(newName);
			}
		}
		else if (g_eModeType == eMT_PATH_EDIT)
		{
			PPlaceNpc pNpc = g_pCache->FindNpc(selName);
			if (pNpc)
			{
				pNpc->name = newName;	
			}
		}
	}
}
void CDynamicFileSel::UpdateNames()
{
	m_listName.ResetContent();
	map<CString, CString>::iterator iter = g_pCache->m_mapDynamicFile.begin();
	for (; iter != g_pCache->m_mapDynamicFile.end(); ++iter)
	{
		m_listName.AddString(iter->first);
	}
}

//void CDynamicFileSel::OnBnClickedButton5()
//{
//	// TODO: Add your control notification handler code here
//	RunLuaFile("LoadWarZoneDynamicFileName()");
//	UpdateNames();
//}

void CDynamicFileSel::OnBnClickedButton6()
{
	// TODO: Add your control notification handler code here
	if (m_listName.GetCount() <= 0)
	{
		return;
	}
	CString strSearchName;
	GetDlgItem(IDC_NEWNAME)->GetWindowText(strSearchName);
	int iStart = m_listName.GetCurSel() + 1;
	if (iStart >= m_listName.GetCount())
	{
		iStart = 0;
	}
	int i = iStart;
	CString nameTemp;
	do 
	{
		m_listName.GetText(i,nameTemp);
		if (nameTemp.Find(strSearchName) != -1)
		{
			m_listName.SetCurSel(i);
			m_listName.SetFocus();
			return;
		}
		i++;
		if (i>= m_listName.GetCount())
		{
			i = 0;
		}
	} while(i != iStart);
}
