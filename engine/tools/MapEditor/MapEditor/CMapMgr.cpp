#include "StdAfx.h"
#include "CMapMgr.h"
#include "CMapEditApp.h"
#include "CEditContext.h"
#include "CEditDataScene.h"
#include "CRenderGroup.h"
#include "CEditTerrainMgr.h"
#include "CEditWaterMgr.h"
//Old MapEditor
#include "LocalMapLoader.h"

using namespace MapEditor;

CMapMgr::CMapMgr(void)
{
}

CMapMgr::~CMapMgr(void)
{
}

void CMapMgr::Init(char* opFileName)
{
}

bool CMapMgr::CreateMap(int w,int h,bool useHMap)
{
	//���ɵر� ˮ������
	CMapEditApp::GetInst()->GetDataScene()->SetWidth(w/4);
	CMapEditApp::GetInst()->GetDataScene()->SetHeight(h/4);
	//CMapEditApp::GetInst()->GetDataScene()->CreateSceneMetaRegion();
	CMapEditApp::GetInst()->GetDataScene()->SetActivePos(0, 0);
	CMapEditApp::GetInst()->GetDataScene()->GetTerrainMgr()->GenerateTerrain(w/4,h/4);
	CMapEditApp::GetInst()->GetDataScene()->GetWaterMgr()->GenerateWater(w/4,h/4);
	//��ʼ������ϵͳ
	CLightSystem *pLightSystem = CMainWindowTarget::GetInst()->GetGroup()->GetLightSystem();
	pLightSystem->SetGlobalLightColor(0xffffffff,0);
	pLightSystem->SetGlobalLightColor(0xffff00ff,1);
	pLightSystem->SetGlobalShadowColor(0xffff0000);
	return true;
}

bool CMapMgr::ResizeMap(int dwWidth, int dwDepth,int offW,int offL)
{
	return false;
}
bool CMapMgr::LoadOldMap(void* fname)
{
	std::string strFileName=(char*)fname;
	//�滻�ļ���չ��.local
	int n = strFileName.find_last_of('.');
	string str0 = strFileName.substr(0,n);
	str0 += ".local";
	CLocalMapLoader* OldMapLoader=new CLocalMapLoader();
	OldMapLoader->NewLoadAsLocal(str0);
	delete OldMapLoader;
	return true;
}
void CMapMgr::SaveOldMap(void* filename)
{
}
