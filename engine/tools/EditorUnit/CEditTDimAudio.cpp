#include "stdafx.h"
#include "CEditTDimAudio.h"
#include "TSqrAllocator.inl"


CEditTDimAudioProp::CEditTDimAudioProp( uint32 ID, IEffectClassDesc* pEffectClassDesc )
:CTDimAudioProp(ID,pEffectClassDesc)
{
	SetUnitProp(this);
	// ����������ù���
	SetBaseUintData();
	SetUintPropData();
}

void CEditTDimAudioProp::SetUintPropData()
{
	// ��Ч��
	_RegCtrlData(E_CUEESTRING, NULL, NULL, "��Ч��", NULL, &m_szAudioName );
	_RegCtrlData(E_FLOAT, NULL, NULL, "���Ÿ���", NULL, &m_fPlayProbability );
	_RegCtrlData(E_BOOLValue, NULL, NULL, "�ӳٽ���", NULL, &m_bDelayStop );

	// advanced prop 
	// EFADV_SCALE   EMap<float, byte>
	_RegCtrlData(E_EBYTEMap, NULL, NULL, "��С", NULL, &m_Scale );

	// EFADV_OFFSET
	_RegCtrlData(E_EVectorMap, NULL, NULL, "�켣", NULL, &m_vecOffset );
	

}

int32 CEditTDimAudioProp::GetProp( EFFECT_PROP* pProp, int32 nBufLen )
{
	EVector<EFFECT_PROP> Prop;
	CEditUnitPropHelp::GetBaseProp( Prop );

	Prop.push_back( EFFECT_PROP( "��Ч��", sqr::CStringData( 2, m_szAudioName ) ) );
	Prop.push_back( EFFECT_PROP( "���Ÿ���", m_fPlayProbability ) );
	Prop.push_back( EFFECT_PROP( "�ӳٽ���", m_bDelayStop ) );

	nBufLen = min( nBufLen,(int32) Prop.size() );
	for ( int32 i = 0; i < nBufLen; ++i )pProp[i] = Prop[i];
	return nBufLen;
}

void CEditTDimAudioProp::SetProp( const EFFECT_PROP* pProp, int32 nBufLen )
{
	EVector<EFFECT_PROP> Prop( pProp, pProp + nBufLen );
	CEditUnitPropHelp::SetBaseProp( Prop );

	for ( size_t i = 0; i < Prop.size(); ++i )
	{
		if ( Prop[i].Name() == "��Ч��" )		m_szAudioName	= Prop[i].Data().Str();
		if ( Prop[i].Name() == "���Ÿ���" )		m_fPlayProbability	= Prop[i].Data().Float();
		if ( Prop[i].Name() == "�ӳٽ���" )		m_bDelayStop	= Prop[i].Data().Bool();
	}

	_UpdateParams();
}

uint32 CEditTDimAudioProp::GetAdvancedProp( ADVANCE_PROP* pOffset,   int32& nOffLen,
									   ADVANCE_PROP* pTexture,   int32& nTextLen,
									   ADVANCE_PROP* pColor,     int32& nColorLen,
									   ADVANCE_PROP* pScale,     int32& nScaleLen,
									   ADVANCE_PROP* pXYZcale,   int32& nXYZcaleLen,
									   ADVANCE_PROP* pUVRCenter, int32& nUVRCenter)
{
	CEditUnitPropHelp::GetAdvancedProp( pOffset, nOffLen, pTexture, nTextLen, pColor, nColorLen, pScale, nScaleLen, pXYZcale, nXYZcaleLen, pUVRCenter, nUVRCenter);

	return EFADV_SCALE|EFADV_OFFSET;
}

void CEditTDimAudioProp::SetAdvancedProp( const ADVANCE_PROP* pOffset,   int32 nOffLen,
									 const ADVANCE_PROP* pTexture,   int32 nTextLen,
									 const ADVANCE_PROP* pColor,     int32 nColorLen,
									 const ADVANCE_PROP* pScale,     int32 nScaleLen,
									 const ADVANCE_PROP* pXYZcale,   int32 nXYZcaleLen,
									 const ADVANCE_PROP* pUVRCenter, int32 nUVRCenter)
{
	CEditUnitPropHelp::SetAdvancedProp( pOffset, nOffLen, pTexture, 0, pColor, 0, pScale, nScaleLen, pXYZcale, nXYZcaleLen, pUVRCenter, nUVRCenter);
}