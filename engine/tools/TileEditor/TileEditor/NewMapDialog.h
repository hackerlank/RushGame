#pragma once


// CNewMapDialog �Ի���

class CNewMapDialog : public CDialog
{
	DECLARE_DYNAMIC(CNewMapDialog)

public:
	CNewMapDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNewMapDialog();
	
// �Ի�������
	enum { IDD = IDD_NEWMAP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	DWORD m_dwWidth;
	DWORD m_dwDepth;
	afx_msg void OnBnClickedOk2();
};
