#pragma once
//------------------------------------------------------------------------------
/**
    @class CGuiFxPlayer

	GUI����Ч��֮����н�.

    (C) 2009 ThreeOGCMan
*/
#include "GUIBase.h"
#include "TRefPointer.h"

//------------------------------------------------------------------------------
namespace sqr
{
class SQRWnd;
class IEffect;
class IEffectManager;
class IEffectGroup;
class CRenderGroup;
SmartPointer(CViewport);

class CGuiFxPlayer : public IGUIHandler
{
public:
	CGuiFxPlayer(IEffectManager* manager, const string& fxPath);
	~CGuiFxPlayer();
	void OnEnableIme( bool bEnable ) {};
	/// ������Ч
	void OnPlayFx( SQRWnd* pWnd, const char* fxName, EEffectRun eEffrun = eEffRun_Normal );
	/// ����������Ч
	void* OnPlayFxIme( SQRWnd* pWnd, const char* fxName, EEffectRun eEffrun = eEffRun_Normal );
	/// ������Ч
	void  OnPlayFxEx( SQRWnd* pWnd, const char* fxFileName, const char* fxName, EEffectRun eEffrun = eEffRun_Normal ){};
	/// ����������Ч
	void* OnPlayFxImeEx( SQRWnd* pWnd, const char* fxFileName, const char* fxName, EEffectRun eEffrun = eEffRun_Normal ){ return NULL; };
	/// ������Ч
	void OnClearFx( SQRWnd* pWnd, const char* fxName );
	/// ������Ч
	void OnClearFx( SQRWnd* pWnd );
	/// ����������Ч
	void OnClearImeFx( SQRWnd* pWnd, const char* fxName );

	/// ����
	void OnUpdateFx(uint curTime, float zoom);
	void OnWndSized(const CFRect& rect) {}
	bool OnRenderFx( EffectKey& key, uint curTime, float zoom );
	void SetBePreView(bool pre);
	
private:
	struct EffectUnit
	{
		EffectUnit()
			: window(NULL) {}
		EffectUnit(IEffect* fx, SQRWnd* wnd)
			: effect(fx), window(wnd) {}
		EffectUnit(IEffect* fx, SQRWnd* wnd, const char* nm)
			: effect(fx), window(wnd) 
		{
			fx_name = (UIString)(nm);
		}
	public:
		IEffect* effect;
		SQRWnd* window;
		UIString fx_name;
	};
private:
	IEffectManager* effectManager;
	IEffectGroup* effectGroup;

	list<EffectUnit> effects;
	list<EffectUnit> effectsIme;

	CRenderGroup* renderGroup;
	CViewportPtr viewport;

	bool	m_PreView;
};

}// namespace sqr
