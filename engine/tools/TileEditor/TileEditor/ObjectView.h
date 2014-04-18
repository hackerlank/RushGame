#pragma once


#include "FileTree.h"

// CObjectView ������ͼ

class CObjectView : public CFormView
{
public:
	CObjectView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CObjectView();

public:
	enum { IDD = IDD_TEST };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void PostNcDestroy() { }

	DECLARE_MESSAGE_MAP()
public:
	void Init();
	CFileTree m_pathTree;
	CFileList m_fileList;

protected:
	virtual void PreSubclassWindow();
public:
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
};


