// MakeTraditionLangMFC.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMakeTraditionLangMFCApp:
// �йش����ʵ�֣������ MakeTraditionLangMFC.cpp
//

class CMakeTraditionLangMFCApp : public CWinApp
{
public:
	CMakeTraditionLangMFCApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMakeTraditionLangMFCApp theApp;