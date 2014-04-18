// FontConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GUIEditor.h"
#include "LanguageConfigDlg.h"
#include "CLanguageConfigEdit.h"

// CLanguageConfigDlg dialog

IMPLEMENT_DYNAMIC(CLanguageConfigDlg, CDialog)

CLanguageConfigDlg::CLanguageConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLanguageConfigDlg::IDD, pParent)
{

}

CLanguageConfigDlg::~CLanguageConfigDlg()
{
}

void CLanguageConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LANGUAGE_LIST, m_combLanguageList);
	DDX_Control(pDX, IDC_FONTLIST, m_combFontList);
	DDX_Control(pDX, IDC_LANGUAGE_NAME, m_editLanguage);
	DDX_Control(pDX, IDC_FONT_NAME, m_editFontName);
	DDX_Control(pDX, IDC_FONT_SIZE, m_editFontSize);
	DDX_Control(pDX, IDC_FONT_FILE, m_editFontFile);
}


BEGIN_MESSAGE_MAP(CLanguageConfigDlg, CDialog)
	ON_BN_CLICKED(IDC_ADD_LANGUAGE, &CLanguageConfigDlg::OnBnClickedAddLanguage)
	ON_BN_CLICKED(IDC_ADDFONT, &CLanguageConfigDlg::OnBnClickedAddFont)
	ON_BN_CLICKED(ID_GENERATE_LANGUAGE_CONFIG, &CLanguageConfigDlg::OnBnClickedGenerateConfigFile)
	ON_BN_CLICKED(ID_CANCEL_CONFIG_LANGUAGE, &CLanguageConfigDlg::OnBnClickedCancelConfigLanguage)
	ON_CBN_SELCHANGE(IDC_LANGUAGE_LIST, &CLanguageConfigDlg::OnCbnSelChangeLanguageList)
END_MESSAGE_MAP()


// CLanguageConfigDlg message handlers

void CLanguageConfigDlg::OnBnClickedAddLanguage()
{
	CString	strLanguage;
	m_editLanguage.GetWindowText(strLanguage);
	if (strLanguage.GetLength() == 0)
	{
		MessageBox(L"��������������");
		return;
	}
	
	addLanguage(utf16_to_mbcs((LPCWSTR)strLanguage));

	m_editLanguage.SetWindowText(L"");
	m_combFontList.ResetContent();
}

void CLanguageConfigDlg::OnBnClickedAddFont()
{
	int32 nLanIndex = m_combLanguageList.GetCurSel();
	if ( nLanIndex == -1 )
	{
		MessageBox(L"����ѡ��һ�����ԡ�");
		return;
	}

	CString strFontFile;
	CString strFontName;
	CString strFontSize;
	m_editFontFile.GetWindowText(strFontFile);
	if ( strFontFile.GetLength() == 0 )
	{
		MessageBox(L"�����������ļ���");
		return;
	}
	m_editFontName.GetWindowText(strFontName);
	if ( strFontName.GetLength() == 0 )
	{
		MessageBox(L"��������������");
		return;
	}
	m_editFontSize.GetWindowText(strFontSize);
	if ( strFontSize.GetLength() == 0 )
	{
		MessageBox(L"�����������С");
		return;
	}

	addFont(nLanIndex, utf16_to_utf8((LPCWSTR)strFontFile), utf16_to_utf8((LPCWSTR)strFontName), utf16_to_utf8((LPCWSTR)strFontSize));

	m_editFontFile.SetWindowText(L"");
	m_editFontName.SetWindowText(L"");
	m_editFontSize.SetWindowText(L"");
}

void CLanguageConfigDlg::addLanguage( const string &szLanguage )
{
	if ( !sqr::CLanguageConfigEdit::GetEditInst()->AddLanguage(szLanguage) )
	{
		MessageBox( (utf8_to_utf16(szLanguage) + L"������������Ѿ����ڣ��������������ơ�").c_str() );
		return;
	}

	combAddLanguage(szLanguage);
}

void CLanguageConfigDlg::addFont(int32 nLanIndex, const string &szFontFile, 
								 const string &szFontName, const string &szFontSize)
{
	if ( !sqr::CLanguageConfigEdit::GetEditInst()->AddFont(nLanIndex, szFontFile, szFontName, szFontSize) )
	{
		MessageBox(L"�������ʧ�ܣ����޸�����������ԡ�");
		return;
	}

	updateControl();
}

void CLanguageConfigDlg::combAddLanguage( const string &szLanguage )
{
	m_combLanguageList.SetCurSel(m_combLanguageList.AddString(utf8_to_utf16(szLanguage).c_str()));
}

void CLanguageConfigDlg::combAddFont(int32 id, const string &szFontName, const string &szFontFile)
{
	wstringstream ss;
	ss << id;
	m_combFontList.SetCurSel(m_combFontList.AddString((ss.str() + L" - " + utf8_to_utf16(szFontName) + L" - " + utf8_to_utf16(szFontName)).c_str()) );
}

void CLanguageConfigDlg::OnBnClickedGenerateConfigFile()
{
	if ( !sqr::CLanguageConfigEdit::GetEditInst()->GenerateConfigFile() )
	{
		MessageBox(L"��û�н��й���������,����ʧ��,��������ú����ԡ�");
		return;
	}
	
	OnOK();
}

void CLanguageConfigDlg::OnBnClickedCancelConfigLanguage()
{
	OnCancel();
}

BOOL CLanguageConfigDlg::OnInitDialog()
{
	if ( !CDialog::OnInitDialog())
		return FALSE;

	// ���Ѿ��������������ļ���������������ļ���ȡ���ò��ڶԻ�������Ӷ�Ӧ��Ϣ
	if ( CLanguageConfig::GetInst()->LoadConfig() )
	{
		loadConfigFile();
	}

	return TRUE;
}

INT_PTR CLanguageConfigDlg::DoModal()
{
	INT_PTR re = CDialog::DoModal();
	
	return IDOK;
}

void CLanguageConfigDlg::loadConfigFile()
{
	for ( int32 nLanguageIndex = 0; nLanguageIndex < CLanguageConfig::GetInst()->GetLanCount(); ++nLanguageIndex )
	{
		string szLanguage = CLanguageConfig::GetInst()->GetLanName(nLanguageIndex);
		combAddLanguage(szLanguage);
	}

	updateControl();
}

void CLanguageConfigDlg::OnCbnSelChangeLanguageList()
{
	updateControl();
}

void CLanguageConfigDlg::updateControl()
{
	// ���������б��
	int32 nLanIndex = m_combLanguageList.GetCurSel();

	m_combFontList.ResetContent();

	CLanguageConfig::GDIFontInfoVector  fontMap;
	CLanguageConfig::GetInst()->GetGDIFontInfoVector(nLanIndex, fontMap);
	CLanguageConfig::GDIFontInfoVector::iterator it = fontMap.begin();
	int32 index = 0;
	for ( ; it != fontMap.end(); ++it )
	{
		string szFont	  = utf16_to_mbcs(it->first);
		string szFontFile = it->second;
		combAddFont(index++, szFontFile, szFont);
	}
}	