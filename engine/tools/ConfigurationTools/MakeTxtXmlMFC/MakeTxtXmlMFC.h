// MakeTxtXmlMFC.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMakeTxtXmlMFCApp:
// �йش����ʵ�֣������ MakeTxtXmlMFC.cpp
//

class CMakeTxtXmlMFCApp : public CWinApp
{
public:
	CMakeTxtXmlMFCApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMakeTxtXmlMFCApp theApp;