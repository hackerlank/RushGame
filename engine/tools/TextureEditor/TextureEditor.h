// TextureEditor.h : TextureEditor Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h�� 
#endif

#include "resource.h"       // ������


// CTextureEditorApp:
// �йش����ʵ�֣������ TextureEditor.cpp
//

class CDocManagerEx : public CDocManager
{		
public:
	CDocManagerEx(){};
	virtual ~CDocManagerEx(){};
	
	virtual BOOL DoPromptFileName(CString& fileName, UINT nIDSTitle,
		DWORD lFlags, BOOL bOpenFileDialog, CDocTemplate* pTemplate)
	{
		CFileDialog dlgFile(bOpenFileDialog);
		static TCHAR SaveStr[]="��ͼ�ļ� DDS�ļ�  (*.dds)\0*.dds\0";
		static TCHAR OpenStr[]="��ͼ�ļ� (*.tex)\0*.tex\0BMP�ļ�  (*.bmp)\0*.bmp\0TGA�ļ�  (*.tga)\0*.tga\0JPG�ļ�  (*.jpg)\0*.jpg\0DDS�ļ�  (*.dds)\0*.dds\0";
		
		CString title;
		VERIFY(title.LoadString(nIDSTitle));
		
		dlgFile.m_ofn.Flags |= lFlags|OFN_NOCHANGEDIR;
		dlgFile.m_ofn.nMaxCustFilter++;		
		dlgFile.m_ofn.lpstrFilter = bOpenFileDialog?OpenStr:SaveStr;
		dlgFile.m_ofn.lpstrTitle = title;
		dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);
			
		INT_PTR nResult = dlgFile.DoModal();
		fileName.ReleaseBuffer();
		if ( 0 == bOpenFileDialog )
		{
			// �����û�ѡ�����չ����������������չ��	
			CString	strFileExt;
			int		nFilterIndex	= dlgFile.m_ofn.nFilterIndex;

			strFileExt	= ".dds";
	
			// ȥ��ԭ����չ��������û�ѡ�����չ��
			int DotPos = fileName.Find( ".", max( fileName.GetLength() - 4, 0 ) ); 
			if (  DotPos >= 0 )
			{
				fileName.Delete(DotPos, 10);
			}
			fileName.Append(strFileExt);
		}
		
		

		return nResult == IDOK;
	}
};

class CTextureEditorApp : public CWinApp
{
public:
	CTextureEditorApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CTextureEditorApp theApp;
