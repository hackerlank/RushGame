// MapEditor.h : MapEditor Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h�� 
#endif

#include "resource.h"       // ������

//#include <BluelyG.h>
//#include <BluelyC.h>
//#include <Timer.h>
//#include <FPSCounter.h>
//#include <Camera.h>
//#include <TerrainMesh.h>
//#include <Mouse.h>
//#include <SkyBox.h>
//#include <Operator.h>
//#include <GUI.h>
//#include <Object.h>

// CMapEditorApp:
// �йش����ʵ�֣������ MapEditor.cpp
//

class CMapEditorApp : public CWinApp
{
private:
    
	//////////////////////////////////////////////////////////////////////////
	
	// scene oriented 
	
	//bly::COperator * m_pOperator;
	//bly::CTerrainMesh * m_pTerrain;
	//bly::CCamera * m_pCamera;
	
public:
	//bly::CTerrainMesh * GetT() { return m_pTerrain; }
	//bly::COperator * GetO() { return m_pOperator; }
	//bly::CTerrainMesh * SetT() { return m_pTerrain; }
	//bly::COperator * GetO() { return m_pOperator; }

	void BackToWorldView();

public:
	CMapEditorApp();
	~CMapEditorApp();

private:
	void Render();
	void Update();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
};

extern CMapEditorApp theApp;
