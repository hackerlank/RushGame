#include "stdafx.h"
#include "CEditFlareSprite.h"
#include "TSqrAllocator.inl"


CEditFlareSpriteProp::CEditFlareSpriteProp( uint32 ID, IEffectClassDesc* pEffectClassDesc )
:CFlareSpriteProp(ID,pEffectClassDesc)
{
	SetUnitProp(this);
	// ����������ù���
	SetBaseUintData();
	SetUintPropData();
}

void CEditFlareSpriteProp::_UIDataUpdate()
{
	_UpdateParams();
}

void CEditFlareSpriteProp::SetUintPropData()
{
	_RegCtrlData(E_VECTOR, NULL, NULL, "��ת��", NULL, &m_vecRotate);
	_RegCtrlData(E_VECTOR, NULL, NULL, "����", NULL, &m_vecNormal);
	_RegCtrlData(E_FLOAT, AutoUpdate, this, "���ɼ���", NULL, &m_fMaxAngle);
	_RegCtrlData(E_BYTE, AutoUpdate, this, "��СAlpha", NULL, &m_byMinAlpha);
	_RegCtrlData(E_BYTE, AutoUpdate, this, "���Alpha", NULL, &m_byMaxAlpha);
	_RegCtrlData(E_FLOAT, NULL, NULL, "��С�ߴ�", NULL, &m_fMinSize);
	_RegCtrlData(E_FLOAT, NULL, NULL, "���ߴ�", NULL, &m_fMaxSize);
	
	CCombDataHelper* pHelper = (CCombDataHelper*)_RegCtrlData(E_COMBO, NULL, NULL, "����", NULL, &m_eFaceType);
	pHelper->PushItem("����", SPRF_NORMAL );
	pHelper->PushItem("��ͷ", SPRF_CAMERA );
	pHelper->PushItem("����", SPRF_CAMERANONEX );
	
	pHelper = (CCombDataHelper*)_RegCtrlData(E_COMBO, NULL, NULL, "���ģʽ", NULL, &m_eBlendModel );
	pHelper->PushItem("�����", SPRB_NONE);
	pHelper->PushItem("��ˣ�����", SPRB_MULTIPLY);
	pHelper->PushItem("��ӣ�����", SPRB_ADD);
	pHelper->PushItem("��Ч��", SPRB_LIGHT);

	pHelper = (CCombDataHelper*)_RegCtrlData(E_COMBO, NULL, NULL, "Z����", NULL, &m_eZTest);
	pHelper->PushItem("�ر�", Z_NONE);
	pHelper->PushItem("��", Z_WRITE);;
	pHelper->PushItem("ֻ����д", Z_TESTONLY);;

	
	_RegCtrlData(E_BOOLValue, NULL, NULL, "���", NULL, &m_bLightEnable);
	
	// advanced prop 
	// EFADV_COLOR
	_RegCtrlData(E_ECOLORMap, NULL, NULL, "��ɫ", NULL, &m_Color);

	// EFADV_TEXTURE
	_RegCtrlData(E_EEStrMap, NULL, NULL, "��ͼ", NULL, &m_TextName);

	// EFADV_OFFSET
	_RegCtrlData(E_EVectorMap, NULL, NULL, "�켣", NULL, &m_vecOffset);
}

int CEditFlareSpriteProp::GetProp( EFFECT_PROP* pProp, int nBufLen )
{
	EVector<EFFECT_PROP> Prop;
	CEditUnitPropHelp::GetBaseProp( Prop );

	Prop.push_back( EFFECT_PROP( "��ת��",		m_vecRotate ) );
	Prop.push_back( EFFECT_PROP( "����",		m_vecNormal ) );
	Prop.push_back( EFFECT_PROP( "���ɼ���",	m_fMaxAngle*180.0f/3.1415f ) );
	Prop.push_back( EFFECT_PROP( "��СAlpha",	(int)m_byMinAlpha ) );
	Prop.push_back( EFFECT_PROP( "���Alpha",	(int)m_byMaxAlpha ) );
	Prop.push_back( EFFECT_PROP( "��С�ߴ�",	m_fMinSize ) );
	Prop.push_back( EFFECT_PROP( "���ߴ�",	m_fMaxSize ) );
	Prop.push_back( EFFECT_PROP( "����",		CComboData( (BYTE)m_eFaceType, "����,��ͷ,����" ) ) );
	Prop.push_back( EFFECT_PROP( "���ģʽ",	CComboData( (BYTE)m_eBlendModel,  "�����,��ˣ�����,��ӣ�����,��Ч��" ) ) );
	Prop.push_back( EFFECT_PROP( "Z����",		CComboData( (BYTE)m_eZTest, "�ر�,��,ֻ����д" ) ) );
	Prop.push_back( EFFECT_PROP( "���",		CComboData( (BYTE)m_bLightEnable, "�ر�,��" ) ) );

	nBufLen = min( nBufLen,(int) Prop.size() );
	for( int i = 0; i < nBufLen; ++i )
		pProp[i] = Prop[i];

	return nBufLen;
}

void CEditFlareSpriteProp::SetProp( const EFFECT_PROP* pProp, int nBufLen )
{
	EVector<EFFECT_PROP> Prop( pProp, pProp + nBufLen );
	CEditUnitPropHelp::SetBaseProp( Prop );

	for( int i = 0; i < (int)Prop.size(); ++i )
	{
		if( Prop[i].Name() == "��ת��" )			m_vecRotate = Prop[i].Data().Vec();
		if( Prop[i].Name() == "����" )				m_vecNormal = Prop[i].Data().Vec();
		if( Prop[i].Name() == "���ɼ���" )		m_fMaxAngle = Prop[i].Data().Float()*3.1415f/180.0f;
		if( Prop[i].Name() == "��СAlpha" )			m_byMinAlpha = (BYTE)Prop[i].Data().Int();
		if( Prop[i].Name() == "���Alpha" )			m_byMaxAlpha = (BYTE)Prop[i].Data().Int();
		if( Prop[i].Name() == "��С�ߴ�" )			m_fMinSize  = Prop[i].Data().Float();
		if( Prop[i].Name() == "���ߴ�" )			m_fMaxSize  = Prop[i].Data().Float();
		if( Prop[i].Name() == "����" )				m_eFaceType = (SPR_FACE)Prop[i].Data().Index();
		if( Prop[i].Name() == "���ģʽ" )			m_eBlendModel = (SPR_BLENDMODE)Prop[i].Data().Index();
		if( Prop[i].Name() == "Z����" )				m_eZTest = (Z_TEST)Prop[i].Data().Index();
		if( Prop[i].Name() == "���" )				m_bLightEnable = !!Prop[i].Data().Index();
	}
	_UpdateParams();
}

uint32 CEditFlareSpriteProp::GetAdvancedProp( ADVANCE_PROP* pOffset,   int32& nOffLen,
										 ADVANCE_PROP* pTexture,   int32& nTextLen,
										 ADVANCE_PROP* pColor,     int32& nColorLen,
										 ADVANCE_PROP* pScale,     int32& nScaleLen, 
										 ADVANCE_PROP* pXYZcale,   int32& nXYZcaleLen,
										 ADVANCE_PROP* pUVRCenter, int32& nUVRCenter)
{
	CEditUnitPropHelp::GetAdvancedProp( pOffset, nOffLen, pTexture, nTextLen, pColor, nColorLen, pScale, nScaleLen, pXYZcale, nXYZcaleLen, pUVRCenter, nUVRCenter);

	return EFADV_COLOR|EFADV_TEXTURE|EFADV_OFFSET;
}

void CEditFlareSpriteProp::SetAdvancedProp( const ADVANCE_PROP* pOffset,   int32 nOffLen,
									   const ADVANCE_PROP* pTexture,   int32 nTextLen,
									   const ADVANCE_PROP* pColor,     int32 nColorLen,
									   const ADVANCE_PROP* pScale,     int32 nScaleLen,
									   const ADVANCE_PROP* pXYZcale,   int32 nXYZcaleLen,
									   const ADVANCE_PROP* pUVRCenter, int32 nUVRCenter)
{
	CEditUnitPropHelp::SetAdvancedProp( pOffset, nOffLen, pTexture, nTextLen, pColor, nColorLen, pScale, 0, pXYZcale, nXYZcaleLen, pUVRCenter, nUVRCenter);
}