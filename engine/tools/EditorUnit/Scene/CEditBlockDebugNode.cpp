#include "StdAfx.h"
#include "CEditBlockDebugNode.h"
#include "CRenderScene.h"
#include "CEditDataScene.h"
#include "CEditLogicDataMgr.h"

CEditBlockDebugNode::CEditBlockDebugNode(ERenderObjStyle style , CRenderScene* pScene)
	: CSceneBlockDebugNode(style,pScene)
{
	m_uRange = 128;//���������и�bug������m_pRenderScene->GetCameraDest()�ڵر��ֵʼ����һ���ģ������赲��ʾ������camera�ƶ���
}

CEditBlockDebugNode::~CEditBlockDebugNode(void)
{
}

EBarrierType CEditBlockDebugNode::GetBarrierType( CPos pos )
{
	CEditLogicDataMgr* pEditLogicDataMgr = (CEditLogicDataMgr*)m_pRenderScene->GetDataScene()->GetLogicDataMgr();
	int8 type = pEditLogicDataMgr->GetBlockType(pos.x, pos.y);
	return (EBarrierType)type;
}