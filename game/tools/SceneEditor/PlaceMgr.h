#pragma once
#include "NewSceneEditor.h"
#include "PlaceDataSheet.h"
#include "NpcPage.h"
#include "ObjPage.h"
#include "TrapPage.h"



//����ڷ�npc,obj,trap����
class CPlaceMgr
{
private:
	CPlaceMgr(void);
public:
	static CPlaceMgr& Inst();
	~CPlaceMgr(void);
	
	bool Init(CWnd* pwnd);
	void NormalShow();
	void NormalShow(int index);
	void Hide();
	void ShowBossPage(int iColonyId);
	void ShowServantPage(int iColonyId);

	BOOL IsShow();

	CElement* GetPlaceElement();
	
	//�ڶ�̬·���༭ģʽ��, �ڷ���·��ʱ��Ϊ�� ͳһ��ʹ�� GetPlaceNpc ���·��(��ʵ��npc����·��)
	//ͨ�� SetPathName ���� m_pageNpc �д���һ��ѡ���npc;
	void SetPathName(const CString&  pathName); 
private:
	CPlaceDataSheet*	m_pSheet;
	CNpcPage			m_pageNpc;
	CObjPage			m_pageObj;
	CTrapPage			m_pageTrap;
};