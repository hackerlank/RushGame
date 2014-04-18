
#pragma once

/////////////////////////////////////////////////////
// ��������������CView�࣬һ�������ļ�Ŀ¼����һ�������ļ��б�
// ר�������ļ��Ĳ鿴
//

//FLODER �ļ�Ŀ¼��
struct FLODER
{
	FLODER()
	{
		memset(Path,0,1024);
		pChild=NULL;
		ChildNum=0;
		pFloder=NULL;
	};
	~FLODER()
	{
		if(pChild)
		{
			for(int i=0;i<ChildNum;i++)
			{
				delete pChild[i];
			}
			delete []pChild;
			pChild=NULL;
			ChildNum=0;
		}
		if(pFloder)
			pFloder->Release();
	};

	LPSHELLFOLDER pFloder;
	HTREEITEM     Item;
	TCHAR          Path[1024];

	int           ChildNum;
	FLODER**      pChild;
};

class CFileList : public CListCtrl
{
// Construction
public:
	CFileList();
	CImageList m_Image;
	int m_TextIndex;
	int m_BmpIndex;

// Attributes
private:
	TCHAR m_Path[1024];
	BOOL m_bInit;

	map< string, UINT > m_FileExt;

// Operations
public:
	BOOL ResetList(LPCTSTR path);
	LPCTSTR GetPath(){ return m_Path; };
	BOOL Create(CWnd* parent);

	//�Ǽ��ļ�ͼ��ͺ�׺
	BOOL SetFileIcon( LPCTSTR extname );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileList)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFileList();

	// Generated message map functions
protected:
	//{{AFX_MSG(CFileList)
	afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CFileTree window

class CFileTree : public CTreeCtrl
{
// Construction
public:
	CFileTree();

// Attributes
public:

// Operations
public:
	LPCTSTR GetSelectedPath();
	BOOL    SetCurPath( LPCTSTR Path );
	BOOL    Create(CWnd* parent);

	//���ù������ļ��б�
	void SetFileList(CFileList* filelist){m_FileList=filelist;};
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFloderTree)
	//}}AFX_VIRTUAL

private:
	CImageList m_Image;                      //�ļ���ͼ���б�
	FLODER     m_desktop;                    //���棨��Ŀ¼��
	FLODER*    m_SelectFloder;               //��ǰѡ���Ŀ¼
	CFileList* m_FileList;                   //���������ļ��б�
	
	BOOL InitChild(FLODER& floder,int order=1);
// Implementation
public:
	virtual ~CFileTree();

	// Generated message map functions
protected:
	//{{AFX_MSG(CFileTree)
	afx_msg void OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

class CFileTreeView : public CView
{
protected:
	CFileTreeView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CFileTreeView)

public:
	void SetFileList(CFileList* filelist){ m_pathtree.SetFileList(filelist);}

private:
	CFileTree m_pathtree;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPathTreeView)
	protected:
	virtual void OnDraw(CDC* pDC){};      // overridden to draw this view
	//}}AFX_VIRTUAL

protected:
	virtual ~CFileTreeView();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPathTreeView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

class CFileListView : public CView
{
protected:
	CFileListView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CFileListView)

// Attributes
private:
	CFileList m_filelist;

// Operations
public:
	void SetFileList(CFileTreeView& filetree){ filetree.SetFileList(&m_filelist);}
	BOOL SetFileIcon( LPCTSTR extname ){ return m_filelist.SetFileIcon( extname );}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileListView)
	protected:
	virtual void OnDraw(CDC* pDC){};      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CFileListView();

protected:
	//{{AFX_MSG(CFileListView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

class CDirSelectWnd : public CDialog
{
	CString		m_CurPath;
	CFileTree	m_Tree;
	CButton		m_OK;
	CButton		m_Cancel;

	BOOL		m_ReturnID;
	CFont		m_Font;
	
public:
	CString GetDirector( const CString InitDir, CString Title = "ѡ��Ŀ¼" );
// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
	virtual void OnCancel();
};
