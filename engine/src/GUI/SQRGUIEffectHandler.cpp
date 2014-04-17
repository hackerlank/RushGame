#include "stdafx.h"
#include "SQRGUIEffectHandler.h"

SQRGUIEffectHandler::SQRGUIEffectHandler()
{
	for (size_t i = 0; i < eEff_Count; ++i)
		m_EffectUnion[i] = NULL;
}

SQRGUIEffectHandler::~SQRGUIEffectHandler()
{
	for (size_t i = 0; i < eEff_Count; ++i)
		if ( m_EffectUnion[i] )
			delete m_EffectUnion[i];
}

bool SQRGUIEffectHandler::AddGUIEffect(SQRWnd* wnd, SQRGUIEffectBase* eff)
{
	if (!eff)
		return false;
	EGUIEffect eff_type = eff->GetEffectType();
	// �����ǰ�����ڴ����ε�״̬����������µĸ����ε�״̬
	if ( eff_type < eEff_Count && eff_type >= 0 && !m_EffectUnion[eff_type] )
	{
		m_EffectUnion[eff_type] = eff;
		return true;
	}
	return false;
}

void SQRGUIEffectHandler::DelGUIEffect(EGUIEffect eff)
{
	if ( eff < eEff_Count && eff >= 0 && m_EffectUnion[eff] 
		&& (!m_EffectUnion[eff]->GetIsDel()) )
	{
		m_EffectUnion[eff]->EndEffect();

		if (m_EffectUnion[eff]->GetbeLock())
			m_EffectUnion[eff]->SetIsDel(true);
		else
		{
			delete m_EffectUnion[eff];
			m_EffectUnion[eff] = NULL;
		}
	}
}

void SQRGUIEffectHandler::ExecuteEffects()
{
	for (size_t i = 0; i < eEff_Count; ++i)
	{
		if (m_EffectUnion[i])
		{
			// ִ�и����ε�Ч��
			m_EffectUnion[i]->SetbeLock(true);
			m_EffectUnion[i]->ExecuteEffect();
			m_EffectUnion[i]->SetbeLock(false);
			// ����
			if ( m_EffectUnion[i]->GetIsDel()||m_EffectUnion[i]->GetIsDie() )
			{
				delete m_EffectUnion[i];
				m_EffectUnion[i] = NULL;
			}
		}
	}
}