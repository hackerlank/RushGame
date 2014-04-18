#include "stdafx.h"
#include "CEditShake.h"
#include "TSqrAllocator.inl"



CEditShakeProp::CEditShakeProp( uint32 ID, IEffectClassDesc* pEffectClassDesc )
:CShakeProp(ID,pEffectClassDesc)
{
	SetUnitProp(this);
	// ����������ù���
	SetBaseUintData();
	SetUintPropData();
}


void CEditShakeProp::SetUintPropData()
{
	_RegCtrlData(E_VECTOR, NULL, NULL, "�������", NULL, &m_vecDirCtrl );
	_RegCtrlData(E_VECTOR, NULL, NULL, "����Ƶ��", NULL, &m_vecFrequence );
	_RegCtrlData(E_FLOAT, NULL, NULL, "ǿ��", NULL, &m_fStrength );
	_RegCtrlData(E_BOOLValue, NULL, NULL, "Ӱ�����", NULL, &m_isCtrlParent );

	// advanced prop 
	// NULL	
	
}

int32 CEditShakeProp::GetProp( EFFECT_PROP* pProp, int32 nBufLen )
{
	EVector<EFFECT_PROP> Prop;
	CEditUnitPropHelp::GetBaseProp( Prop );
	Prop.push_back( EFFECT_PROP( "�������",		m_vecDirCtrl ) );
	Prop.push_back( EFFECT_PROP( "����Ƶ��",		m_vecFrequence ) );
	Prop.push_back( EFFECT_PROP( "ǿ��",			m_fStrength ) );
	Prop.push_back( EFFECT_PROP( "Ӱ�����",		m_isCtrlParent ) );
	nBufLen = min( nBufLen,(int32) Prop.size() );
	for( int32 i = 0; i < nBufLen; ++i )pProp[i] = Prop[i];
	return nBufLen;
}

void CEditShakeProp::SetProp( const EFFECT_PROP* pProp, int32 nBufLen )
{
	EVector<EFFECT_PROP> Prop( pProp, pProp + nBufLen );
	CEditUnitPropHelp::SetBaseProp( Prop );

	for( size_t i = 0; i < Prop.size(); ++i )
	{
		if( Prop[i].Name() == "�������" )			m_vecDirCtrl	= Prop[i].Data().Vec();
		if( Prop[i].Name() == "����Ƶ��" )			m_vecFrequence	= Prop[i].Data().Vec();
		if( Prop[i].Name() == "ǿ��" )				m_fStrength 	= Prop[i].Data().Float();
		if( Prop[i].Name() == "Ӱ�����" )			m_isCtrlParent 	= Prop[i].Data().Bool();
	}

	_UpdateParams();
}

uint32 CEditShakeProp::GetAdvancedProp( ADVANCE_PROP* pOffset,    int32& nOffLen,
								   ADVANCE_PROP* pTexture,   int32& nTextLen,
								   ADVANCE_PROP* pColor,     int32& nColorLen,
								   ADVANCE_PROP* pScale,     int32& nScaleLen, 
								   ADVANCE_PROP* pXYZcale,   int32& nXYZcaleLen,
								   ADVANCE_PROP* pUVRCenter, int32& nUVRCenter)
{
	CEditUnitPropHelp::GetAdvancedProp( pOffset, nOffLen, pTexture, nTextLen, pColor, nColorLen, pScale, nScaleLen, pXYZcale, nXYZcaleLen, pUVRCenter, nUVRCenter);

	return 0;
}

void CEditShakeProp::SetAdvancedProp( const ADVANCE_PROP* pOffset,   int32 nOffLen,
								 const ADVANCE_PROP* pTexture,  int32 nTextLen,
								 const ADVANCE_PROP* pColor,    int32 nColorLen,
								 const ADVANCE_PROP* pScale,    int32 nScaleLen,
								 const ADVANCE_PROP* pXYZcale,   int32 nXYZcaleLen,
								 const ADVANCE_PROP* pUVRCenter, int32 nUVRCenter)
{
	CEditUnitPropHelp::SetAdvancedProp( pOffset, nOffLen, pTexture, 0, pColor, 0, pScale, nScaleLen, pXYZcale, nXYZcaleLen, pUVRCenter, nUVRCenter);
}