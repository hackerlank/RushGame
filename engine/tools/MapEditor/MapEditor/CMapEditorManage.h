#pragma once

#include "TSingleton.h"

#ifndef NEW_MAPEDITOR
#define NEW_MAPEDITOR
#endif

namespace MapEditor
{
	class CMapMgr;
}

namespace sqr_tools
{
	class CEditContext;
	class CEditDataScene;
}

using namespace MapEditor;

class CMapEditorManage :public Singleton<CMapEditorManage>
{
public:
	CMapEditorManage(void);
	~CMapEditorManage(void);
	
private:
	CMapMgr*			m_Map;
	CEditContext*		m_pContext;
	CEditDataScene*		m_pDataScene;

public:
	void SetRenderScene(CEditContext* cont);
	CEditContext* GetRenderScene();
	CEditDataScene* GetDataScene( void );
	CMapMgr* GetMapMgr();
	//��ʼ������
	void InitMapEditorEngine();
	//��ʼ���ر���
	void InitMapEditorFrame();
	bool IsEdit;
};
