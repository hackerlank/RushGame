#pragma once

#include "TMetaScene.h"
#include "CFindPathMgrTemplate.h"
#include "CTraitsClient.h"
#include "CColor.h"
#include "CSceneManagerClient.h"

namespace sqr
{
	class CTestSuiteMetaSceneClient;
	class CTestSuiteCoreObjectClient;
	class IMetaSceneClientHandler;

	class CoreSceneManagerClient
		: public CSceneManagerClient
	{
	public:
		CoreSceneManagerClient(const SString& SceneName, CTreeScene* pTreeScene , uint32 Wdith , uint32 Height, uint32 RecycleSize );;
	protected:
		IMap*		CreateIMap(uint32 uWidthInRegion, uint32 uHeightInRegion);
	};

	class CMetaSceneClient
		:public TMetaScene<CTraitsClient>
	{
	private:
		typedef TMetaScene<CTraitsClient>		ParentMetaScene_t;
		friend class CRenderScene;
		friend class TMetaSceneMgr<CTraitsClient>;
		friend class TPart<ImpMetaScene_t,ImpMetaSceneMgr_t>;
		friend class CTestSuiteCoreObjectClient;

		CMetaSceneClient( const char* szSceneName, const uint32 sceneId, CMetaSceneMgrClient* pMgr );
		~CMetaSceneClient();

	public:
		bool						m_bUsedByCore;			//��������ʹ�õı�ǣ����Ϊtrue����ʹshellҪ���ͷŸ�metascene��ʵ����Ҳ�����ͷš�
		uint32						m_uUsedCountByShell;	//�߼���ʹ�ü���
	
		CTreeScene*					m_pTreeScene;
		CoreSceneManagerClient		m_SceneManager;
	};
}