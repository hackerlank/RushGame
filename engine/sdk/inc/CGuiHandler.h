#pragma once
//------------------------------------------------------------------------------
/**
@class CGuiHandler

GUI����Ч��֮����н�.

(C) 2009 ThreeOGCMan
*/
#include "GUIBase.h"
#include "TRefPointer.h"
#include "CSceneMallocObject.h"

//------------------------------------------------------------------------------
namespace sqr
{
	class CWnd;
	class IEffect;
	class IEffectManager;
	class IEffectGroup;
	class CRenderGroup;
	SmartPointer(CViewport);
	struct CGuiEffect : public SQRGuiMallocObject
	{
		CGuiEffect( uint endTime, IEffect* pEffect, EEffectRun eEffrun = eEffRun_Normal) 
				  : m_uEndTime(endTime),m_pEffect(pEffect), m_eEffRunMode(eEffrun){};
		uint		m_uEndTime;
		EEffectRun	m_eEffRunMode;
		IEffect*	m_pEffect;
	};

	class CGuiHandler : public IGUIHandler
	{
	public:
		CGuiHandler(IEffectManager* manager, const string& fxPath);
		~CGuiHandler();
		void OnEnableIme( bool b );
		/// ���ڴ�С�ı�
		void  OnWndSized(const CFRect& rect);

		/// ������Ч
		void  OnPlayFx( SQRWnd* pWnd, const char* fxName, EEffectRun eEffrun = eEffRun_Normal );
		void* OnPlayFxIme( SQRWnd* pWnd, const char* fxName, EEffectRun eEffrun = eEffRun_Normal );
		/// ָ����Ч·��
		void  OnPlayFxEx( SQRWnd* pWnd, const char* fxFileName, const char* fxName, EEffectRun eEffrun = eEffRun_Normal );
		void* OnPlayFxImeEx( SQRWnd* pWnd, const char* fxFileName, const char* fxName, EEffectRun eEffrun = eEffRun_Normal );
		/// ɾ����Ч
		void  OnClearFx( SQRWnd* pWnd, const char* fxName );
		void  OnClearFx( SQRWnd* pWnd );
		void  OnClearImeFx( SQRWnd* pWnd, const char* fxName );
		/// ����
		void OnUpdateFx(uint curTime, float zoom);
		/// �ض�����
		bool OnRenderFx( EffectKey& key, uint curTime, float zoom );

	private:
		typedef UIMap<EffectKey, CGuiEffect> EffectMap;
	private:
		IEffectManager*		effectManager;
		EffectMap			effectMap;
		EffectMap			ImeEffectMap;

		CRenderGroup*		renderGroup;
		CViewportPtr		viewport;
		SString				m_fxDefPath;
	};

}// namespace sqr
