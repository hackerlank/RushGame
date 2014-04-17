#include "stdafx.h"
#include "CRenderSystem.h"
#include "CGraphic.h"
#include "IEffectMgrModule.h"
#include "CRenderObject.h"
#include "CRenderDialog.h"
#include "CObjectDialog.h"
#include "CRenderSpace.h"
#include "CRenderScene.h"
#include "CMetaGridMgr.h"
#include "CMetaSceneClient.h"
#include "TWhole.inl"
#include "CCamera.h"
#include "CSceneConfig.h"
#include "IAudioModule.h"
//#include "CCameraTick.h"
#include "CRenderGroup.h"
#include "IGUIManager.h"
#include "CWaterMgr.h"
#include "CTerrain.h"
#include "IProcessCallBack.h"
#include "CAppClient.h"
#include "CodeCvs.h"
#include "ErrLogHelper.h"
#include "CHardwareBufferMgr.h"
#include "CSqrAllocPool.h"
#include "SQRGUIMallocObject.h"
#include "CSceneMallocObject.h"
#include "MemoryHelper.h"
#include "StringHelper.h"
#include "CTPSCameraCtrller.h"
#include "CHardwareCapacity.h"
#include "CTextureQualityMgr.h"
#include "CRenderPipeline.h"
#include "TSqrAllocator.inl"
#include "CTerrainMgr.h"
#include "CWaterMgr.h"
#include "CSkyMgr.h"
#include "CCameraConfig.h"
#include "PkgStream.h"
#include "CPkgFile.h"
#include "IProcessCallBack.h"
#include "CEffectLoader.h"

#include "mmsystem.h"
//template class TWhole<CRenderSystem,CRenderScene>;

//FILE* gs_File=NULL;

CRenderSystem::CRenderSystem()
:m_isAutoLimitFps(true),m_bFreeCamera(false),m_bCursorSel(true)
,m_isShowFps(false),m_isShowDebug(0),m_isShowASynLoad(0),m_isRender(true),
m_pProcessHandler(NULL)
{
	//SQR_TRY
	//{
	//	///shader�����ļ�·��
	//	char dir[1024];
	//	::GetCurrentDirectory(1024, dir);
	//	CSceneConfig* config = CSceneConfig::GetInst();
	//	if( NULL == new CGraphic(eRCT_DX9))
	//		GenErr("��Ⱦϵͳ����ʧ��");			

	//	if (NULL != config)
	//	{
	//		RenderSettings settings;
	//		settings.SetWindowHandle(hWnd);
	//		settings.SetWidth(config->GetWindowWidth());
	//		settings.SetHeight(config->GetWindowHeight());
	//		if (settings.GetWidth() != 0)
	//		{
	//			settings.SetOrgRatio( float(settings.GetHeight()) / float(settings.GetWidth()) );
	//		}
	//		settings.SetColorDepth(config->GetColorDepth());
	//		settings.SetVSync(config->IsVSynchronize());
	//		settings.SetAntiAliasQuality((EAntiAliasQuality)config->GetAntiAliasQuality());
	//		m_bFreeCamera = config->IsFreeCameraEnabled();
	//		
	//		if (!CMainWindowTarget::GetInst()->Initialize(settings))
	//			GenErr("��Ⱦϵͳ����ʧ��");
	//		
	//		CTerrainMgr::RegiserVertexDecl(); //ע��ر��ʽ
	//		CWaterRegionMesh::InitIndexBuffer();

	//		settings.SetWindowed(!config->IsFullScreen());
	//		CMainWindowTarget::GetInst()->ChangeRenderSettings(settings);

	//		CGraphic::GetInst()->InitShader(config->GetShaderConfigPath(),(ShaderVer)config->GetMaxShaderVer());
	//		CRenderGroup*	pGroup = CRenderGroupMgr::GetInst()->CreateGroup();
	//		CMainWindowTarget::GetInst()->SetRenderGroup(pGroup);
	//		pGroup->Init(MFF_NPC_SHADOW);

	//	
	//		CCamera* pCam;

	//		m_isShowFps = config->IsShowFps();
	//		m_hWnd	= hWnd;
	//		m_pMainPlayer = NULL;
	//		m_fIncRenderObj = 4;
	//		m_fIncGUI = 4;
	//		m_fIncPlayer = 4;
	//		m_RenderObjAlpha = 255;
	//		m_GUIAlpha = 255;
	//		m_MainPlayerAlpha = 255;

	//		pCam = CMainWindowTarget::GetInst()->GetCamera();
	//		if (config->IsLockRatio())
	//			CMainWindowTarget::GetInst()->LockAspectRatio(float(config->GetWindowWidth())/float(config->GetWindowHeight()));
	//		CMainWindowTarget::GetInst()->GetGroup()->EnableFog(true);

	//		SQR_TRY
	//		{
	//			IAudioSystem* audioSystem = GetAudioModule()->GetAudioSystem();
	//			if (audioSystem->Open())
	//				audioSystem->LoadSoundBankFromXml("sound/config.xml");
	//			else
	//				GfkDrvLog("AudioSystem��ʧ��");
	//		}
	//		SQR_CATCH(exp)
	//		{
	//			ostringstream strm;
	//			strm << "��Ч�ⴴ��ʧ��" << endl
	//				<<exp.ErrorTitle()<<endl
	//				<< exp.ErrorMsg() << endl;

	//			GfkLogErr(strm.str().c_str());
	//		}
	//		SQR_TRY_END;
	//	}
	//	else
	//	{
	//		if (!CMainWindowTarget::GetInst()->Initialize(RenderSettings()))
	//			GenErr("��Ⱦϵͳ�Ҳ��������ļ�");
	//		CMainWindowTarget::GetInst()->SetRenderGroup(CRenderGroupMgr::GetInst()->CreateGroup());
	//	}
	//	// window resolutions
	//	m_resWidths = CMainWindowTarget::GetInst()->GetResolutionWidths();

	//	if ( !( GetIEffectMgrModule()->CreateEffectMgr() ) )
	//		GenErr("��Чϵͳ���ܳ�ʼ�����");

	//	//������ͼ����	
	//	if(CHardwareCapacity::GetInst()->GetVideoMem() != 0)
	//	{
	//		UINT QLevel = config->GetTexQualityLevel();
	//		SString Info = "";
	//		if( QLevel == 0 && CHardwareCapacity::GetInst()->GetVideoMem() < config->GetTextHightMem() )
	//		{
	//			++QLevel;
	//			format(Info,"������Ч�Դ�(%uMB)���ͣ����޷���������������",CHardwareCapacity::GetInst()->GetVideoMem());
	//		}

	//		if( QLevel == 1 && CHardwareCapacity::GetInst()->GetVideoMem() < config->GetTextMidMem() )
	//		{
	//			++QLevel;
	//			format(Info,"������Ч�Դ�(%uMB)���ͣ����޷�������������������",CHardwareCapacity::GetInst()->GetVideoMem());
	//		}

	//		if(Info.size()>0)
	//			::MessageBox(hWnd,Info.c_str(),"��ʾ",0);
	//		CTextureQualityMgr::GetInst()->SetQualityLevel(QLevel);
	//	}
	//	else
	//		CTextureQualityMgr::GetInst()->SetQualityLevel(config->GetTexQualityLevel()); 		
	//	//--------------------------
	//	CGraphic::GetInst()->SetEffectLevel(config->GetEffectLevel());
	//	IEffectManager::GetInst()->InitClassDesc();
	//}
	//SQR_CATCH(exp)
	//{
	//	GfkLogExp(exp);
	//	ostringstream strm;
	//	strm << "������Ϣ:" << endl
	//		 << exp.ErrorMsg() << endl
	//		 << "�뽫����Ϣ��ͼ,���͵���̳��bugר�����������ǽ����������кܴ�"
	//		 << "�İ�������л����Ӷ�����µ�֧�֡�";
	//	/*ostringstream strm;
	//	strm<<"    ����ȷ���Ƿ���ȷ��װ����Ϸ��װ���Դ���dx9c�ļ����µ�DXSetup���������װ���������⣬ԭ��"
	//		<<"���������ǵ���Ϸδ�ܺܺõؼ��������Կ���Ӳ�����á�Ŀǰ�����õ���һЩ�Կ���Ӳ�����ܣ���ĳЩ�ʼǱ�"
	//		<<"�Կ��Լ����ϵĲ�֧��dx9c���Կ�����û�еģ�2004����ǰ������е��Կ�������޷�������Ϸ�������"<<endl
	//		<<"    ����Կ�������������Ҫ�Ƚϳ���ʱ�䣬����γ�������������Ϸ�������⻹û�н�������������"
	//		<<"��ʾ�ǳ���Ǹ-_-��������ܰ��Կ�����ϸ�ͺ���Ϣ���͵���̳��bugר�����������ǽ����������кܴ�"
	//		<<"�İ�������л����Ӷ�����µ�֧�֡�";*/
	//	ShowErrorMsgAndExit(strm.str().c_str());
	//}
	//SQR_TRY_END;
}

CRenderSystem::~CRenderSystem(void)
{
	SQR_TRY
	{
		if(CSceneConfig::GetInst())
		{
			GetAudioModule()->GetAudioSystem()->Close();
		}

		CEffectLoader::Destroy();
		IEffectManager::Destroy();	
		CGraphic::Destroy();
	}
	SQR_CATCH(exp)
	{
		GfkLogExp(exp);
		ShowErrorMsgAndExit("CRenderSystem delete fail, the client will be shut");
	}
	SQR_TRY_END;
}

#define GfkErrText "��Ⱦϵͳ����ʧ��"
void CRenderSystem::InitRenderSystem(HWND hWnd)
{
	SQR_TRY
	{
		CSceneConfig* config = CSceneConfig::GetInst();
		if (NULL != config)
		{
			RenderSettings settings;
			settings.SetWindowHandle(hWnd);
			settings.SetWidth(config->GetWindowWidth());
			settings.SetHeight(config->GetWindowHeight());
			if (settings.GetWidth() != 0)
			{
				settings.SetOrgRatio( float(settings.GetHeight()) / float(settings.GetWidth()) );
			}
			settings.SetColorDepth(config->GetColorDepth());
			settings.SetVSync(config->IsVSynchronize());
			settings.SetAntiAliasQuality((EAntiAliasQuality)config->GetAntiAliasQuality());
			m_bFreeCamera = config->IsFreeCameraEnabled();
			if (!CMainWindowTarget::GetInst()->Initialize(settings))
				GenErr(GfkErrText);
			
			CTerrainMgr::RegiserVertexDecl(); //ע��ر��ʽ
			CWaterRegionMesh::InitIndexBuffer();

			settings.SetWindowed(!config->IsFullScreen());
			CMainWindowTarget::GetInst()->ChangeRenderSettings(settings);
			CGraphic::GetInst()->InitShader(config->GetShaderConfigPath(),(ShaderVer)config->GetMaxShaderVer());

			CRenderGroup* pGroup = new CRenderGroup;
			CMainWindowTarget::GetInst()->SetRenderGroup(pGroup);

			SQR_TRY
			{
				pGroup->SetShadowType(EShadowType(CSceneConfig::GetInst()->GetShadowType()));
			}
			SQR_CATCH(exp)
			{
				exp.AppendType("��Ӱ����������ʧ��");
				GfkLogExp(exp);
			}
			SQR_TRY_END;
			SafeRelease(pGroup);

			CCamera* pCam;

			m_isShowFps = config->IsShowFps();
			m_hWnd	= hWnd;
			m_pMainPlayer = NULL;
			m_fIncGUI = 4;
			m_bShowObj = true;
			m_fIncPlayer = 4;
			m_RenderObjAlpha = 255;
			m_GUIAlpha = 255;
			m_MainPlayerAlpha = 255;

			pCam = CMainWindowTarget::GetInst()->GetCamera();
			if (config->IsLockRatio())
				CMainWindowTarget::GetInst()->LockAspectRatio(float(config->GetWindowWidth())/float(config->GetWindowHeight()));
			CMainWindowTarget::GetInst()->GetGroup()->EnableFog(true);
			
			SQR_TRY
			{
				IAudioSystem* audioSystem = GetAudioModule()->GetAudioSystem();
				if (audioSystem->Open())
				{
					audioSystem->LoadSoundBankFromXml("sound/config.xml");
				}
			}
			SQR_CATCH(exp)
			{
				ostringstream strm;
				strm << "��Ч�ⴴ��ʧ��" << endl
					<<exp.ErrorTitle()<<endl
					<< exp.ErrorMsg() << endl;

				GfkLogErr(strm.str().c_str());
			}
			SQR_TRY_END;
		}
		else
		{
			if (!CMainWindowTarget::GetInst()->Initialize(RenderSettings()))
				GenErr("��Ⱦϵͳ�Ҳ��������ļ�");
			CMainWindowTarget::GetInst()->SetRenderGroup(new CRenderGroup);
		}
		// window resolutions
		m_resWidths = CMainWindowTarget::GetInst()->GetResolutionWidths();

		if ( !( GetIEffectMgrModule()->CreateEffectMgr() ) )
			GenErr("��Чϵͳ���ܳ�ʼ�����");	
		//--------------------------
		CGraphic::GetInst()->SetEffectLevel(config->GetEffectLevel());
		CGraphic::GetInst()->SetCameraShake(config->GetCameraShake());
		IEffectManager::GetInst()->InitClassDesc();
		CEffectLoader::Create();
		CEffectLoader::GetInst()->SetGroupHandle(hWnd);

		//���ÿ�������
		SetRefractEnabled(config->IsRefractEnabled());
		//����ȫ������
		SetBloomEnabled(config->IsBloomEnabled());
	}
	SQR_CATCH(exp)
	{
		ostringstream strm;
		string ErrMsg = exp.ErrorTitle();

		strm<<"�����Կ��޷�����DirectX9c,������Կ��������������Խ��������"<<endl
			<<"ͬʱ������������Ϸ��װĿ¼�µ�dx9c�ļ����µ�DXSetup����";

		if(ErrMsg != GfkErrText)
		{			
			GfkLogExp(exp);
		}

		/*ostringstream strm;
		strm<<"    ����ȷ���Ƿ���ȷ��װ����Ϸ��װ���Դ���dx9c�ļ����µ�DXSetup���������װ���������⣬ԭ��"
			<<"���������ǵ���Ϸδ�ܺܺõؼ��������Կ���Ӳ�����á�Ŀǰ�����õ���һЩ�Կ���Ӳ�����ܣ���ĳЩ�ʼǱ�"
			<<"�Կ��Լ����ϵĲ�֧��dx9c���Կ�����û�еģ�2004����ǰ������е��Կ�������޷�������Ϸ�������"<<endl
			<<"    ����Կ�������������Ҫ�Ƚϳ���ʱ�䣬����γ�������������Ϸ�������⻹û�н�������������"
			<<"��ʾ�ǳ���Ǹ-_-��������ܰ��Կ�����ϸ�ͺ���Ϣ���͵���̳��bugר�����������ǽ����������кܴ�"
			<<"�İ�������л����Ӷ�����µ�֧�֡�";*/
		ShowErrorMsgAndExit(strm.str().c_str());
	}
	SQR_TRY_END;
}

IGraphic* CRenderSystem::Inst_Graphic()
{
	return CGraphic::GetInst();
}

void CRenderSystem::UpdateAudioSystem()
{
	// update audio engine
	if(IAudioSystem::HasInst())
		IAudioSystem::GetInst()->Update();
}

void	CRenderSystem::Refresh()
{
	if ( FAILED( CMainWindowTarget::GetInst()->IsRenderActive() ) )
		return;

	CMainWindowTarget::GetInst()->Render();
}

void CRenderSystem::SetClearColor(uint32 r,uint32 g,uint32 b)
{
	CMainWindowTarget::GetInst()->SetClearColor( CColor(255,(uint8)r,(uint8)g,(uint8)b) );
}

void CRenderSystem::Clear(void)
{
	CMainWindowTarget::GetInst()->Clear();
}

CObjectDialog* CRenderSystem::CreateObjectDialog()
{
	return new CObjectDialog();
}

void CRenderSystem::DestroyObjectDialog(CObjectDialog* pWnd)
{
	delete pWnd;
}

void CRenderSystem::ShowFpsSwitch()
{
	m_isShowFps ++;
	if(m_isShowFps>2)
		m_isShowFps = 0;
}

void CRenderSystem::ShowPlayerSwitch(void)
{
	m_fIncPlayer = -m_fIncPlayer;
}

void CRenderSystem::ShowASynLoadSwitch(void)
{
	++m_isShowASynLoad;
	if (m_isShowASynLoad>2)
		m_isShowASynLoad = 0;
	if(IGUIManager::HasInst())
		IGUIManager::GetInst()->ShowWndArea(!IGUIManager::GetInst()->isShowWndArea());
}

void CRenderSystem::ShowDebugSwitch()
{
	++m_isShowDebug;
	if (m_isShowDebug>2)
		m_isShowDebug = 0;
}
void CRenderSystem::ShowRenderObjSwitch(void)
{
	m_bShowObj = !m_bShowObj;
}

void CRenderSystem::ShowGUISwith(void)
{
	m_fIncGUI = -m_fIncGUI;
}

void CRenderSystem::LockAspectSwitch(void)
{
	CCamera* pCam = CMainWindowTarget::GetInst()->GetCamera();
	RECT rect;
	GetClientRect( m_hWnd, &rect );
	float AspRat = float(rect.right - rect.left)/float(rect.bottom - rect.top);
	if (CMainWindowTarget::GetInst()->isLockAspectRatio())
	{
		CMainWindowTarget::GetInst()->LockAspectRatio(0.0f);
		pCam->setAspectRatio(AspRat);
	}
	else
		CMainWindowTarget::GetInst()->LockAspectRatio(AspRat);
}

void CRenderSystem::SwitchHidenMode()
{
	IGUIManager::GetInst()->SwitchHidenMode();
}

void CRenderSystem::SetIsRender(bool isRender)
{
	m_isRender = isRender;
	if(m_isRender)
	{
		CMainWindowTarget::GetInst()->GetGroup()->SetRenderQueueFlag(m_RenderFlag);
		if(CMainWindowTarget::HasInst())
			CMainWindowTarget::GetInst()->InsertRenderer(IGUIManager::GetInst());

	}
	else
	{
		m_RenderFlag = CMainWindowTarget::GetInst()->GetGroup()->GetRenderQueueFlag();
		RenderQueueFlag SceneFlag;
		CMainWindowTarget::GetInst()->GetGroup()->SetRenderQueueFlag(SceneFlag);
		if(CMainWindowTarget::HasInst())
			CMainWindowTarget::GetInst()->DeleteRenderer(IGUIManager::GetInst());
	}

}

//------------------------------------------------------------------------------
bool CRenderSystem::ChangeRenderSettings( RenderSettings* settings )
{
	bool suc = false;
	SQR_TRY
	{
		CSceneConfig* config = CSceneConfig::GetInst();
		config->SetFullScreen(!settings->IsWindowed());
		config->SetWindowWidth(settings->GetWidth());
		config->SetWindowHeight(settings->GetHeight());
		config->SetAntiAliasQuality(settings->GetAntiAliasQuality());
		config->SetColorDepth(settings->GetColorDepth());
		config->SetVSynchronize(settings->IsVSync());
		config->SaveConfig();
		suc = SUCCEEDED(CMainWindowTarget::GetInst()->ChangeRenderSettings(*settings));
	}
	SQR_CATCH(exp)
	{
		exp.AppendMsg("ChangeRenderSetting failed");
		GfkLogExp(exp);
	}
	SQR_TRY_END
	return suc;
	
}

void CRenderSystem::SetEffectLevel(float level)
{
	CGraphic::GetInst()->SetEffectLevel(level);
	CSceneConfig::GetInst()->SetEffectLevel(level);
}

float CRenderSystem::GetEffectLevel(void)
{
	return CGraphic::GetInst()->GetEffectLevel();
}

void CRenderSystem::SetCameraShake(bool b)
{
	CGraphic::GetInst()->SetCameraShake(b);
	CSceneConfig::GetInst()->SetCameraShake(b);
}

bool CRenderSystem::GetCameraShake(void)
{
	return CGraphic::GetInst()->GetCameraShake();
}

bool CRenderSystem::IsSelectByCursor(void)
{
	return m_bCursorSel;
}

void CRenderSystem::SetSelectByCursor(bool isSel)
{
	m_bCursorSel = isSel;
}


UINT CRenderSystem::GetTexQualityLevel(void)
{
	return CTextureQualityMgr::GetInst()->GetQualityLevel();
}

void CRenderSystem::SetTexQualityLevel( UINT QualityLevel )
{
	static bool stbFirstChange = true;
	if(CHardwareCapacity::GetInst()->GetVideoMem() != 0 && stbFirstChange)
	{
		string Info = "";
		if( QualityLevel == 0 && CHardwareCapacity::GetInst()->GetVideoMem()  < CSceneConfig::GetInst()->GetTextHightMem() )
		{
			format(Info,"���Ǽ�⵽������Ч�Դ����Ϊ%uMB,���鲻Ҫ��������������",CHardwareCapacity::GetInst()->GetVideoMem());
		}

		if( QualityLevel == 1 && CHardwareCapacity::GetInst()->GetVideoMem()  < CSceneConfig::GetInst()->GetTextMidMem() )
		{
			format(Info,"���Ǽ�⵽������Ч�Դ����Ϊ%uMB,���鲻Ҫ������������������",CHardwareCapacity::GetInst()->GetVideoMem());
		}

		if(Info.size()>0)
		{
			stbFirstChange = false;
			::MessageBox(CMainWindowTarget::GetInst()->GetHwnd(),Info.c_str(),"��ʾ",0);
		}
	}

	if(  CTextureQualityMgr::GetInst()->GetQualityLevel() != QualityLevel )
	{
		class TempProcess : public IProcessCallBack
		{
		//public:
		//	TempProcess(){};
		//	~TempProcess(){};
			void Clear()
			{
				if ( CRenderSystem::GetInst()->GetProcessCallBack() )
					CRenderSystem::GetInst()->GetProcessCallBack()->DoProcess( 0.0f );
				m_fProcess = 0.0f;
			}

			void DoProcess( float process )
			{				
				process *= 100.0f;
				if ((m_fProcess==100.0f || process != 100.0f)&&(process - m_fProcess < 1.0f || process<0.0f))
					return;

				m_fProcess = process;
				if ( CRenderSystem::GetInst()->GetProcessCallBack() )
					CRenderSystem::GetInst()->GetProcessCallBack()->DoProcess( m_fProcess );
			}

		private:
			float m_fProcess;
		};
		
		TempProcess tProcess;
		//CRenderSystem::GetInst()->SetProcessCallBack(&tProcess);
		CTextureQualityMgr::GetInst()->SetQualityLevel(QualityLevel);
		CSceneConfig::GetInst()->SetTexQualityLevel(QualityLevel);

		RenderQueueFlag SceneFlag = CMainWindowTarget::GetInst()->GetGroup()->GetRenderQueueFlag();
		RenderQueueFlag NullFlag;
		CMainWindowTarget::GetInst()->GetGroup()->SetRenderQueueFlag(NullFlag);
		CTextureQualityMgr::GetInst()->Update(&tProcess);
		CMainWindowTarget::GetInst()->GetGroup()->SetRenderQueueFlag(SceneFlag);
		//CRenderSystem::GetInst()->SetProcessCallBack(0);
	}
}

UINT	CRenderSystem::GetBackBufferNum(void)
{
	return CMainWindowTarget::GetInst()->GetBackBufferNum();
}

void	CRenderSystem::SetBackBufferNum(UINT Num)
{
	CMainWindowTarget::GetInst()->SetBackBufferNum(Num);
}

//------------------------------------------------------------------------------
void CRenderSystem::SetShadowType( EShadowType type )
{
	SQR_TRY
	{
		CMainWindowTarget::GetInst()->GetGroup()->SetShadowType(type);
		CSceneConfig::GetInst()->SetShadowType(type);
	}
	SQR_CATCH(exp)
	{
		exp.AppendType("��Ӱ����������ʧ��");
		GfkLogExp(exp);
	}
	SQR_TRY_END;
}

//------------------------------------------------------------------------------
bool CRenderSystem::SetWaterType( EWaterType type )
{
	CWaterMgr* waterManager = (CWaterMgr*)CMainWindowTarget::GetInst()->GetGroup()->GetBatch(ID_WATER_FILTER);
	if(!waterManager)
		return false;
	CSceneConfig::GetInst()->SetWaterType(type);
	return waterManager->SetWaterType(type);
}

//------------------------------------------------------------------------------
void CRenderSystem::SetUseShaderTerrain( bool b )
{
	CSceneConfig::GetInst()->SetTerrainQuality(b&&CGpuProgramMgr::GetInst()->GetShaderVer()>=Shader_V_2?1:0);
}

//------------------------------------------------------------------------------
bool CRenderSystem::SetBloomEnabled( bool b )
{
	b &= CGpuProgramMgr::GetInst()->GetShaderVer() > Shader_V_1;
	CBloomTarget* pipeline = CMainWindowTarget::GetInst()->GetTargetPipeline();

	SQR_TRY
	{
		pipeline->SetTurnOn("Bloom", b);
	}
	SQR_CATCH(exp)
	{
		exp.AppendMsg("����ȫ������ʧ��");
		b = false;
		pipeline->SetTurnOn("Bloom", b);
		GfkLogExp(exp);
	}
	SQR_TRY_END
	
	CSceneConfig::GetInst()->SetBloomEnabled(b);

	return b;
}

//------------------------------------------------------------------------------
bool
CRenderSystem::IsRefractEnabled() const
{
	return CMainWindowTarget::GetInst()->GetTargetPipeline()->IsTurnOn("Refract");
}

//------------------------------------------------------------------------------
bool
CRenderSystem::SetRefractEnabled( bool b )
{
	b &= CGpuProgramMgr::GetInst()->GetShaderVer() > Shader_V_1;
	CBloomTarget* pipeline = CMainWindowTarget::GetInst()->GetTargetPipeline();

	SQR_TRY
	{
		pipeline->SetTurnOn("Refract", b);
	}
	SQR_CATCH(exp)
	{
		exp.AppendMsg("��������Ч������ʧ��");
		b = false;
		pipeline->SetTurnOn("Refract", b);
		GfkLogExp(exp);
	}
	SQR_TRY_END
	
	CSceneConfig::GetInst()->SetRefractEnabled(b);

	return b;
}

uint32	CRenderSystem::GetRenderCyc(void) const
{
	return CSceneConfig::GetInst()->GetRenderCyc();
}

void	CRenderSystem::SetRenderCyc(uint32 cyc)
{
	CSceneConfig::GetInst()->SetRenderCyc(cyc);
}


//------------------------------------------------------------------------------
void
CRenderSystem::SetAudioMute( bool mute )
{
	if( !GetAudioModule()->HasAudioSystem() )
	{
		GfkLogErr("AudioSystem is NULL");
		return;
	}

	SQR_TRY
	{
		IAudioSystem* audioSystem = GetAudioModule()->GetAudioSystem();
		if (mute)
		{
			audioSystem->SetGlobalVariable("MusicVolume",   0.0f);
			audioSystem->SetGlobalVariable("EffectVolume",  0.0f);
			audioSystem->SetGlobalVariable("AmbientVolume", 0.0f);
		}
		else
		{
			audioSystem->SetGlobalVariable("MusicVolume",   GetAudioVolume("MusicVolume"));
			audioSystem->SetGlobalVariable("EffectVolume",  GetAudioVolume("EffectVolume"));
			audioSystem->SetGlobalVariable("AmbientVolume", GetAudioVolume("AmbientVolume"));
		}
	}
	SQR_CATCH(exp)
	{
		GfkLogExp(exp);
	}
	SQR_TRY_END;
}

//------------------------------------------------------------------------------
void CRenderSystem::SetAudioVolume( const char* category, float volume )
{
	IAudioSystem* audioSystem = GetAudioModule()->GetAudioSystem();
	CSceneConfig::GetInst()->SetAudioVolume(category, volume);
	audioSystem->SetGlobalVariable(category, volume);
}

//------------------------------------------------------------------------------
void CRenderSystem::SetVoiceSwitch( bool bOpen )
{
	CSceneConfig::GetInst()->SetVoiceSwitch(bOpen);
}

//------------------------------------------------------------------------------
float CRenderSystem::GetAudioVolume( const char* category )
{
	return CSceneConfig::GetInst()->GetAudioVolume(category);
}

//------------------------------------------------------------------------------
bool CRenderSystem::GetVoiceSwitch() const
{
	return CSceneConfig::GetInst()->GetVoiceSwitch();
}

//------------------------------------------------------------------------------
void CRenderSystem::GetRenderSettings( RenderSettings& settings ) const
{
	settings = CMainWindowTarget::GetInst()->GetRenderSettings();
}

bool CRenderSystem::IsInWindowState()
{
	return CMainWindowTarget::GetInst()->GetRenderSettings().IsWindowed();
}
//------------------------------------------------------------------------------
EShadowType CRenderSystem::GetShadowType() const
{
	return CMainWindowTarget::GetInst()->GetGroup()->GetShadowType();
}

//------------------------------------------------------------------------------
EWaterType CRenderSystem::GetWaterType() const
{
	CWaterMgr* waterManager = (CWaterMgr*)CMainWindowTarget::GetInst()->GetGroup()->GetBatch(ID_WATER_FILTER);
	if(!waterManager)
		return WT_Low;
	return waterManager->GetWaterType();
}

//------------------------------------------------------------------------------
bool CRenderSystem::IsUseShaderTerrain() const
{
	return CSceneConfig::GetInst()->GetTerrainQuality()>0;
}

//------------------------------------------------------------------------------
bool CRenderSystem::IsBloomEnabled() const
{
	return CMainWindowTarget::GetInst()->GetTargetPipeline()->IsTurnOn("Bloom");
}

//------------------------------------------------------------------------------

/**
����, ��ʱ��Ϊ�ļ�������
*/
const char* CRenderSystem::Snapshot()
{
	time_t T;
	struct tm * timenow;
	time ( &T );
	timenow = localtime ( &T ); 

	wchar_t filename[MAX_PATH];
	_snwprintf(filename, MAX_PATH, L"%s%04d_%02d_%02d_%02d_%02d_%02d.png",
		CSceneConfig::GetInst()->GetScreenshotPath(),
		timenow->tm_year + 1900, timenow->tm_mon + 1, timenow->tm_mday,
		timenow->tm_hour, timenow->tm_min, timenow->tm_sec);

	wstring fullname = CMainWindowTarget::GetInst()->DoScreenShot(CPkgFile::GetLoadPath(L""),filename);

	static string filename_utf8;
	filename_utf8 = utf16_to_utf8(fullname);

	return filename_utf8.c_str();
}		

float CRenderSystem::GetUIZoom(void)
{
	return IGUIManager::GetInst()->GetRootWnd()->GetOrgZoom();
}

void  CRenderSystem::SetUIZoom(float zoom)
{
	zoom = limit2(zoom,0.68f,1.32f);
	CSceneConfig::GetInst()->SetGuiZoom(zoom);
	IGUIManager::GetInst()->GetRootWnd()->SetOrgZoom(zoom);
}

//------------------------------------------------------------------------------
size_t
CRenderSystem::GetNumResWidths() const
{
	return m_resWidths.size();
}

//------------------------------------------------------------------------------
uint
CRenderSystem::GetResWidthAt( index_t i ) const
{
	Ast(i < m_resWidths.size());
	return m_resWidths[i];
}

void		CRenderSystem::SetProcessCallBack( IProcessCallBack* pProcessCall )
{
	m_pProcessHandler = pProcessCall;
}

IProcessCallBack*	CRenderSystem::GetProcessCallBack( void )
{
	return m_pProcessHandler;
}

void	CRenderSystem::SetShowFog( bool isFog )
{
	if( isFog )
		CMainWindowTarget::GetInst()->GetGroup()->SetFogParams(FMT_LINEAR,	1000.0f,	2000.0f);
	else
		CMainWindowTarget::GetInst()->GetGroup()->SetFogParams(FMT_NONE);
}
