#include "stdafx.h"
#include "CEditDynamicLight.h"
#include "TSqrAllocator.inl"


CEditDynamicLightProp::CEditDynamicLightProp( uint32 ID, IEffectClassDesc* pEffectClassDesc )
:CDynamicLightProp(ID,pEffectClassDesc)
{
	SetUnitProp(this);
	// ����������ù���
	SetBaseUintData();
	SetUintPropData();
}

CEditDynamicLightProp::~CEditDynamicLightProp(void)
{

}

void CEditDynamicLightProp::_UIDataUpdate()
{
	_UpdateParams();
}

void CEditDynamicLightProp::SetUintPropData()
{
	_RegCtrlData(E_FLOAT,  NULL, NULL, "��Χ", NULL, &m_fRange);
	_RegCtrlData(E_FLOAT, AutoUpdate, this, "��Сǿ��", NULL, &m_fMinStrong);
	_RegCtrlData(E_FLOAT, AutoUpdate, this, "���ǿ��", NULL, &m_fMaxStrong);

	// advanced prop 
	// EFADV_SCALE   EMap<float, byte>
	_RegCtrlData(E_EBYTEMap, NULL, NULL, "��С", NULL, &m_Scale);
	// EFADV_OFFSET
	_RegCtrlData(E_EVectorMap, NULL, NULL, "�켣", NULL, &m_vecOffset);
	// EFADV_COLOR
	_RegCtrlData(E_ECOLORMap, NULL, NULL, "��ɫ", NULL, &m_Color);
}

int32 CEditDynamicLightProp::GetProp( EFFECT_PROP* pProp, int32 nBufLen )
{
	EVector<EFFECT_PROP> Prop;
	CEditUnitPropHelp::GetBaseProp( Prop );

	Prop.push_back( EFFECT_PROP( "��Χ",				m_fRange ) );
	Prop.push_back( EFFECT_PROP( "��Сǿ��",			m_fMinStrong ) );
	Prop.push_back( EFFECT_PROP( "���ǿ��",			m_fMaxStrong ) );

	nBufLen = min( nBufLen,(int32) Prop.size() );
	for( int32 i = 0; i < nBufLen; ++i )pProp[i] = Prop[i];
	return nBufLen;
}

void CEditDynamicLightProp::SetProp( const EFFECT_PROP* pProp, int32 nBufLen )
{
	EVector<EFFECT_PROP> Prop( pProp, pProp + nBufLen );
	CEditUnitPropHelp::SetBaseProp( Prop );

	for( size_t i = 0; i < Prop.size(); ++i )
	{
		if( Prop[i].Name() == "��Χ" )				m_fRange = Prop[i].Data().Float();
		if( Prop[i].Name() == "��Сǿ��" )			m_fMinStrong = max( 1.0f, Prop[i].Data().Float() );
		if( Prop[i].Name() == "���ǿ��" )			m_fMaxStrong = Prop[i].Data().Float();
	}
	_UpdateParams();
}

uint32 CEditDynamicLightProp::GetAdvancedProp( ADVANCE_PROP* pOffset,   int32& nOffLen,
										  ADVANCE_PROP* pTexture,   int32& nTextLen,
										  ADVANCE_PROP* pColor,     int32& nColorLen,
										  ADVANCE_PROP* pScale,     int32& nScaleLen, 
										  ADVANCE_PROP* pXYZcale,   int32& nXYZcaleLen,
										  ADVANCE_PROP* pUVRCenter, int32& nUVRCenter)
{
	CEditUnitPropHelp::GetAdvancedProp( pOffset, nOffLen, pTexture, nTextLen, pColor, nColorLen, pScale, nScaleLen, pXYZcale, nXYZcaleLen, pUVRCenter, nUVRCenter );

	return EFADV_SCALE|EFADV_OFFSET|EFADV_COLOR;
}

void CEditDynamicLightProp::SetAdvancedProp( const ADVANCE_PROP* pOffset,   int32 nOffLen,
										const ADVANCE_PROP* pTexture,   int32 nTextLen,
										const ADVANCE_PROP* pColor,     int32 nColorLen,
										const ADVANCE_PROP* pScale,     int32 nScaleLen,
										const ADVANCE_PROP* pXYZcale,   int32 nXYZcaleLen,
										const ADVANCE_PROP* pUVRCenter, int32 nUVRCenter)
{
	CEditUnitPropHelp::SetAdvancedProp( pOffset, nOffLen, pTexture, 0, pColor, nColorLen, pScale, nScaleLen, pXYZcale, nXYZcaleLen, pUVRCenter, nUVRCenter);
}