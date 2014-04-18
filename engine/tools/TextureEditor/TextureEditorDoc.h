// TextureEditorDoc.h :  CTextureEditorDoc ��Ľӿ�
//

#include "texture.h"
#include "D3dd.h"
#pragma once

class CTextureEditorDoc : public CDocument
{
	CD3d*     m_D3d;
	CTexture  m_Text;
	float     m_Scale;

	CString   m_PathStr;

protected: // �������л�����
	CTextureEditorDoc();
	DECLARE_DYNCREATE(CTextureEditorDoc)

// ����
public:
	float GetScale()                 { return m_Scale;  }
	void  SetScale( float Scale )    { m_Scale = Scale; }

// ����
public:
	CPoint SetTexture();
	void   SetD3d( CD3d* D3d ){ m_D3d=D3d; }
	CString GetFormat();
	virtual void SetTitle(LPCTSTR lpszTitle);

// ��д
	public:
	virtual BOOL OnNewDocument();

// ʵ��
public:
	virtual ~CTextureEditorDoc();

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	afx_msg void OnFileAutobat();
	afx_msg void OnFileMaptrans();
	afx_msg void OnEditAddframe();
	afx_msg void OnExportTga();
	afx_msg void OnDDSConvert();
};


