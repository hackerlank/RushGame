#pragma once
//------------------------------------------------------------------------------
/**
	@class CPostProcessManager

	���������.
	��������Ч���Ŀ��غ�RenderTarget����������.
	����Ч��֮����Թ�����ʱ��RenderTarget, �Խ�ʡ��Դռ��
	Ч��֮������˳�򲻿ɸı�
	���õ�Ч����Ҫ��pass���, �ͷ�RTT������
*/
#include "TSingleton.h"
#include "TRefPointer.h"
#include "RTTFormat.h"
#include "CShaderConstParams.h"


namespace sqr
{
	enum PostProcessShaderParam
	{
		// ---- VS PARAMS
		PPSP_INV_VIEW_SIZE_VS			= ACT_CUSTOM_PARAMS_LIST,
		
		// ---- PS PARAMS
		PPSP_INV_VIEW_SIZE_PS			= 0,
		PPSP_BRIGHT_PASS_THRESHOLD_PS	= 1,
		PPSP_EXPOSURE_PS				= 2,
		PPSP_SCALER_PS					= 3,
		PPSP_HBLOOM_WEIGHTS_PS			= 4,
		PPSP_HBLOOM_OFFSETS_PS			= 7,
		PPSP_VBLOOM_WEIGHTS_PS			= 10,
		PPSP_VBLOOM_OFFSETS_PS			= 13,
		PPSP_DOWN_SAMPLE_OFFSETS_PS		= 16, // use 16 slots, so next param will be "32"
	};

	SmartPointer(CRenderTexture);
	SmartPointer(CPostEffect);
	SmartPointer(CPostEffectPass);
	SmartPointer(CPostEffectLoader);
	class CRenderGroup;

	class CPostProcessManager 
		: public Singleton<CPostProcessManager>
		, public CGraphicMallocObject
	{
		friend class Singleton<CPostProcessManager>;	
	public:
		CPostProcessManager();
		~CPostProcessManager();
		static const GString ConfigFile;

	public:
		/// ��ʼ������Ч��
		virtual void Setup();
		/// �ͷ���Դ
		virtual void Discard();

		/// ����ָ��Ч���Ƿ�����
		virtual bool IsEffectEnabled(const string& name);
		/// ����/����ָ��Ч��
		virtual void SetEffectEnabled(const string& name, bool enable);

		/// ��Ⱦ��Ĭ�ϵ�RenderTarget
		//void Begin(CRenderGroup* pRenderGroup);
		/// ���к���
		virtual void DoPostProcess();

		/// ����Ĭ�ϵ�RTT
		virtual CRenderTexture* GetDefaultRenderTarget();

	private:
		/// �Ƿ��к���Ч������
		bool HasPostEffectEnabled() const;
		/// ����pass��
		void UpdatePassChain();
	//private:
	protected:
		GVector<CPostEffectPtr>			postEffects;
		CPostEffectPassPtr				nullPass;		///< ��װdefault RTT�õ�
		GSet<GString>					enabledEffects;
		CRenderTexture*					defaultRTT;
		CPostEffectLoaderPtr			effectLoader;
	};

	//------------------------------------------------------------------------------
	inline bool
	CPostProcessManager::HasPostEffectEnabled() const
	{
		return (!this->enabledEffects.empty());
	}

	//------------------------------------------------------------------------------
	inline CRenderTexture*
	CPostProcessManager::GetDefaultRenderTarget()
	{
		return this->defaultRTT;
	}

	//------------------------------------------------------------------------------
	inline bool
	CPostProcessManager::IsEffectEnabled( const string& name )
	{
		return (this->enabledEffects.end() != this->enabledEffects.find(name.c_str()));
	}

}// namespace sqr
