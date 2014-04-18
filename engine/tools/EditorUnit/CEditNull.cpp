#include "stdafx.h"
#include "CEditNull.h"
#include "TSqrAllocator.inl"


CEditNullProp::CEditNullProp( uint32 ID, IEffectClassDesc* pEffectClassDesc )
:CNullProp(ID,pEffectClassDesc)
{
	SetUnitProp(this);
	// ����������ù���
	SetBaseUintData();
	SetUintPropData();
}

void CEditNullProp::_UIDataUpdate()
{
	_UpdateParams();
}

void CEditNullProp::SetUintPropData()
{
	_RegCtrlData(E_BOOLValue, NULL, NULL, "λ��ͣ��", NULL, &m_bDelay );
	_RegCtrlData(E_VECTOR, NULL, NULL, "��ת��", NULL, &m_vecRotate );
	_RegCtrlData(E_BOOLValue, NULL, NULL, "���ٶȷ�����ת", NULL, &m_bFolowSpeed );
	
	CCombDataHelper* pCombHelper = (CCombDataHelper*)_RegCtrlData(E_COMBO, NULL, NULL, "λ��", NULL, &m_eFaceType );
	pCombHelper->PushItem("��ͨ", SNLF_NORMAL);
	pCombHelper->PushItem("�ر�߶�", SNLF_TERRAIN);
	pCombHelper->PushItem("�ر�����ת", SNLF_TERRAIN_ROTATE);
	
	// advanced prop 
	// EFADV_OFFSET
	_RegCtrlData(E_EVectorMap, NULL, NULL, "�켣", NULL, &m_vecOffset );
	
}

int32 CEditNullProp::GetProp( EFFECT_PROP* pProp, int32 nBufLen )
{
	EVector<EFFECT_PROP> Prop;
	CEditUnitPropHelp::GetBaseProp( Prop );

	Prop.push_back( EFFECT_PROP( "λ��ͣ��",		m_bDelay ) );
	Prop.push_back( EFFECT_PROP( "��ת��",			m_vecRotate ) );
	Prop.push_back( EFFECT_PROP( "���ٶȷ�����ת",	CComboData( (BYTE)m_bFolowSpeed, "��,��" ) ) );
	Prop.push_back( EFFECT_PROP( "λ��",			CComboData( (BYTE)m_eFaceType,"��ͨ,�ر�߶�,�ر�����ת" ) ) );
	nBufLen = min( nBufLen,(int32) Prop.size() );
	for( int32 i = 0; i < nBufLen; ++i )pProp[i] = Prop[i];
	return nBufLen;
}

void CEditNullProp::SetProp( const EFFECT_PROP* pProp, int32 nBufLen )
{
	EVector<EFFECT_PROP> Prop( pProp, pProp + nBufLen );
	CEditUnitPropHelp::SetBaseProp( Prop );

	for( size_t i = 0; i < Prop.size(); ++i )
	{
		if( Prop[i].Name() == "λ��ͣ��" )				m_bDelay = Prop[i].Data().Bool();
		if( Prop[i].Name() == "��ת��" )				m_vecRotate = Prop[i].Data().Vec();
		if( Prop[i].Name() == "���ٶȷ�����ת" )		m_bFolowSpeed = Prop[i].Data().Index() != 0;
		if( Prop[i].Name() == "λ��" )					m_eFaceType = (SNULL_FACE)Prop[i].Data().Index();
	}
	_UpdateParams();
}

uint32 CEditNullProp::GetAdvancedProp( ADVANCE_PROP* pOffset,   int32& nOffLen,
								  ADVANCE_PROP* pTexture,   int32& nTextLen,
								  ADVANCE_PROP* pColor,     int32& nColorLen,
								  ADVANCE_PROP* pScale,     int32& nScaleLen, 
								  ADVANCE_PROP* pXYZcale,   int32& nXYZcaleLen,
								  ADVANCE_PROP* pUVRCenter, int32& nUVRCenter)
{
	CEditUnitPropHelp::GetAdvancedProp( pOffset, nOffLen, pTexture, nTextLen, pColor, nColorLen, pScale, nScaleLen, pXYZcale, nXYZcaleLen, pUVRCenter, nUVRCenter);

	return EFADV_OFFSET;
}

void CEditNullProp::SetAdvancedProp( const ADVANCE_PROP* pOffset,   int32 nOffLen,
								const ADVANCE_PROP* pTexture,   int32 nTextLen,
								const ADVANCE_PROP* pColor,     int32 nColorLen,
								const ADVANCE_PROP* pScale,     int32 nScaleLen,
								const ADVANCE_PROP* pXYZcale,   int32 nXYZcaleLen,
								const ADVANCE_PROP* pUVRCenter, int32 nUVRCenter)
{
	CEditUnitPropHelp::SetAdvancedProp( pOffset, nOffLen, pTexture, 0, pColor, 0, pScale, nScaleLen, pXYZcale, nXYZcaleLen, pUVRCenter, nUVRCenter);
}
