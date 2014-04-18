#include "StdAfx.h"
#include "CEditPostProcessManager.h"
#include "CRenderTexture.h"
#include "CPostEffect.h"
#include "CPostEffectPass.h"
#include "CScreenAlignedQuad.h"
#include "CRenderPipeline.h"
#include "TSqrAllocator.inl"

#include "CEditRenderPipeline.h"
#include "CGraphic.h"
#include "CCamera.h"
#include "CWindowTarget.h"
#include "CRenderGroup.h"
#include "CScreenAlignedQuad.h"


//ע�� ���� �����ӵ�EditUnit������·����Ϣ ��Щͷ�ļ� û������SDK/INC�� �����Ҳ��� 
//���Ǹ���ʱ���� ��Ҫ�����ߺ�������ļ���CPP��λ��

namespace sqr
{

CEditPostProcessManager::CEditPostProcessManager(CPostProcessManager* pPostProcessMgr)
: m_pPostProcessMgr(pPostProcessMgr)
{
	
}

CEditPostProcessManager::~CEditPostProcessManager(void)
{	
}

CEditPostProcessManager* CEditPostProcessManager::GetInst()
{
	return static_cast<CEditPostProcessManager*>(CPostProcessManager::GetInst());
}

void CEditPostProcessManager::Setup()
{
	m_pPostProcessMgr->Setup();
}

void CEditPostProcessManager::Discard()
{
	m_pPostProcessMgr->Discard();
}

bool CEditPostProcessManager::IsEffectEnabled(const string& name)
{
	return m_pPostProcessMgr->IsEffectEnabled(name);
}

void CEditPostProcessManager::SetEffectEnabled(const string& name, bool enable)
{
	m_pPostProcessMgr->SetEffectEnabled(name,enable);
}

void CEditPostProcessManager::DoPostProcess()
{
	CEditRenderPipeline::GetInst()->DisableBasicParamCount();
	CEditRenderPipeline::GetInst()->DisableTexCount();

	//�����༭��������Ч
	CRenderPipeline::GetInst()->_SetRenderStyle(RS_ZWRITEENABLE,FALSE);
	CRenderPipeline::GetInst()->_SetRenderStyle(RS_ZENABLE,FALSE);
	GVector<CPostEffectPtr>::iterator it,eit = postEffects.end();
	for( it = postEffects.begin(); it!=eit; ++it )
	{
		(*it)->Apply();
	}
	//������������Ч
	m_pPostProcessMgr->DoPostProcess();

	CEditRenderPipeline::GetInst()->EnalbeTexCount();
	CEditRenderPipeline::GetInst()->EnableBasicParamCount();
}

CRenderTexture*  CEditPostProcessManager::GetDefaultRenderTarget()
{
	return m_pPostProcessMgr->GetDefaultRenderTarget();
}

//���������ܷ��� by fyz
bool CEditPostProcessManager::AddEffect(CPostEffectPtr effect)
{
	if(effect==NULL ||effect->GetName()==NULL)
		return false;
	string name=effect->GetName();
	if(!FindEffect(name))
		postEffects.push_back(effect);
	return true;
}

void CEditPostProcessManager::DelEffect(const string& name)
{
	GVector<CPostEffectPtr>::iterator it=postEffects.begin();
	while(it!=postEffects.end())
	{
		if(strcmp( (*it)->GetName(),name.c_str())==0)
		{
			CPostEffectPtr tmp=(*it);
			postEffects.erase(it);
			tmp=NULL;
			return ;
		}
		it++;
	}
	return ;
}

bool CEditPostProcessManager::FindEffect(const string& name)
{
	//GVector<CPostEffectPtr>			postEffects;
	GVector<CPostEffectPtr>::iterator it=postEffects.begin();
	while(it!=postEffects.end())
	{
		if(strcmp( (*it)->GetName(),name.c_str())==0)
			return true;
		it++;
	}
	return false;
}

CPostEffectPtr CEditPostProcessManager::GetEffect(const string& name)
{
	GVector<CPostEffectPtr>::iterator it=postEffects.begin();
	while(it!=postEffects.end())
	{
		if(strcmp( (*it)->GetName(),name.c_str())==0)
			return (*it);
		it++;
	}
	return NULL;
}

}
