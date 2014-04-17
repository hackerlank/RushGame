#pragma once
/*
	SQRGUIEffectHandler 
	GUI�ؼ�Ч���Ŀ������ӿڣ�����GUIЧ��
*/
#include "SQRGUIEffectBase.h"
#include "SQRGuiMallocObject.h"
#include "TSqrAllocator.inl"
#include "SQRWnd.h"

namespace sqr
{
	class SQRGUIEffectHandler
		: public virtual CDynamicObject
		, public SQRGuiMallocObject
	{
	public:
		SQRGUIEffectHandler();
		~SQRGUIEffectHandler();

		bool	AddGUIEffect(SQRWnd* wnd, SQRGUIEffectBase* eff);	// ���GUIЧ��
		void	DelGUIEffect(EGUIEffect eff);
		void	ExecuteEffects();									// ִ��Ч��
		SQRGUIEffectBase*	GetEffect(EGUIEffect eff);

	private:
		void	_ProcessEffect();									// ����ʧЧ��Ч��

	private:
		SQRGUIEffectBase* m_EffectUnion[eEff_Count];
	};

	inline SQRGUIEffectBase* SQRGUIEffectHandler::GetEffect(EGUIEffect eff)
	{
		if (eff >= 0 && eff < eEff_Count)
			return m_EffectUnion[eff];
		else
			return NULL;
		
	}
}