// NewSceneEditor.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "ScriptMgr.h"


// CNewSceneEditorApp:
// �йش����ʵ�֣������ NewSceneEditor.cpp
//

class CNewSceneEditorApp : public CWinApp
{
public:
	CNewSceneEditorApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CNewSceneEditorApp theApp;
extern const wstring PATH_ALIAS_SCENE_EDITOR;