#pragma once
#include "CCtrlBaseObserver.h"
#include "CBasePropCtrlImp.h"

//�������Եĸ��� ���ö�ʱ����Ϣ

namespace sqr_tools
{

class CAudioPropCtrlImp;
class CScenePropCtrlImp;
class CModelPropCtrlImp;
class CLightPropCtrlImp;

class CPropCtrlImp :public CCtrlBaseObserver
{
	REG_UI_IMP(CPropCtrlImp);
public:
	CPropCtrlImp(void* _handle);
	~CPropCtrlImp(void);

public:
	virtual DWORD	Filter(void);
	virtual void	ActiveNode( CRenderNode* pNode );
	virtual void	SetContext(CEditContext* pContext);
public:
	virtual int GetCurPropType();
	virtual int GetSelObjectType();

	virtual CAudioPropCtrlImp*	GetAudioPropImp();
	virtual CScenePropCtrlImp*	GetScenePropImp();
	virtual CModelPropCtrlImp*	GetModelPropImp();
	virtual CLightPropCtrlImp*	GetLightPropImp();
protected:
	PROPTYPE			m_CurPropType;
	CAudioPropCtrlImp*	m_AudioPropImp;
	CScenePropCtrlImp*	m_ScenePropImp;
	CModelPropCtrlImp*	m_ModelPropImp;
	CLightPropCtrlImp*	m_LightPropImp;

};

//!namespace
}