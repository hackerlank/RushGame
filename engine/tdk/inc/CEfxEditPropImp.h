#pragma once
#include "CCtrlBaseObserver.h"

class CEditTrack;
class CEditEffectManager;
class CEditEffectProp;

namespace sqr_tools
{
	// ������Ч�༭�ĺ��ģ����ݿɱ༭��Ч���ɶ�Ӧ�ؼ� 
	class CEfxEditPropImp 
		: public CCtrlBaseObserver
	{
		REG_UI_IMP(CEfxEditPropImp);
	public:
		CEfxEditPropImp(void* param);
		~CEfxEditPropImp(void);

		virtual uint32		GetTracksNum();
		virtual void		GetEditTrack( int Num, void** editTrack );
		virtual void*		GetEditEffectProp();

		// over ride
		virtual DWORD	Filter(void);
		virtual void	ActiveNode( CRenderNode* pNode );

		//------------------------------------------------------------------
	private:
		virtual void _InitEditEffectManger();
		virtual void _UpdateCLR();

	private:
		CEditEffectProp*		m_pEditEffectProp;			// ����
		CEditEffectManager*		m_pEffectManager;
	};
}