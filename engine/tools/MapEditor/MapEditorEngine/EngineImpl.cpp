#include "stdafx.h"
#include "EngineImpl.h"
#include "CCameraManager.h"
#include "CMapEditCamera.inl"
#include "CRecordCamera.inl"
#include "Render.h"
#include "SceneRegionMgr.h"
#include "MapGUIManager.h"

#include "TerrainAdhereRenderer.h"
#include "CWindowTarget.h"
#include "EditGuiMgr\EditGuiMgr.h"
#include "GUIRenderer.h"
#include "IApplication.h"
#include "BaseHelper.h"
#include "EditStateOpertiaonMgr.h"

namespace sqr
{
	CEngineImpl::CEngineImpl()
		: m_bInitialized(false)
		, m_bMultipleView(false)
		, m_bWndActive(false)
	{
		m_guiRenderer = NULL;
	}

	CEngineImpl::~CEngineImpl()
	{
		if( CEditGuiMgr::HasInst() )
			CEditGuiMgr::Destroy();

		CCameraManager::Destroy();

		CTerrainAdhereRenderer::Destroy();
		CSceneRegionMgr::Destroy();
		CMapGUIManager::Destroy();
		CRenderDevice::Destroy();
		CSceneManager::Destroy();

		SafeDelete(m_guiRenderer);
	}

	bool CEngineImpl::Init(bool bMyGUI,bool bMultipleView )
	{
		m_bMultipleView  = bMultipleView;
		CSceneManager::Create();
		CMapGUIManager::Create();
		CRenderDevice::Create();
		CRenderDevice::GetInst()->Init();
		CSceneRegionMgr::Create();
		//��GUI��Ⱦ���뵽��RT��
		m_guiRenderer = new CGUIRenderer();
		m_guiRenderer->SetEngineImpl(this);
		if(CMainWindowTarget::HasInst())
			CMainWindowTarget::GetInst()->InsertRenderer(m_guiRenderer);
		
		//�����ر��е��赲���߼��ߵ���Ⱦ ��������RT��
		CTerrainAdhereRenderer::Create();
		CEditStateOpertiaonMgr::Create();
		CMapGUIManager::GetInst()->CreateMapGUI();

		m_bInitialized = true;
		
		SQR_TRY
		{
#ifdef USE_GUI_DIALOG
			if( !CEditGuiMgr::HasInst() )
				CEditGuiMgr::Create();
//
//			RECT viewrc;
//			GetClientRect(m_hWnd,&viewrc);
//			CEditGuiMgr::GetInst()->CreateGuiRootWnd(viewrc);
#else
			return true;
#endif
		}
		SQR_CATCH(exp)
		{
			::MessageBox(NULL, exp.ErrorMsg(), "��ʾ", MB_ICONWARNING);
		}
		SQR_TRY_END;

		return true;
	}
}