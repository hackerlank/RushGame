#include "StdAfx.h"
#include "LightParameterCtrl/CLightParameterCtrlImp.h"

DEF_UI_IMP(CLightParameterCtrlImp);
CLightParameterCtrlImp::CLightParameterCtrlImp(void* param)
{
}

CLightParameterCtrlImp::~CLightParameterCtrlImp(void)
{
}

void CLightParameterCtrlImp::Update()
{
	CCtrlBase::Update();
}

void CLightParameterCtrlImp::ActiveNode( CRenderNode* pNode )
{
	//pNodeΪ��ǰ����ĵƹ�ڵ�
	if(curLight)
		Update();
}

