#include "stdafx.h"
#include "CEditCameraFilter.h"
#include "TSqrAllocator.inl"

CEditCameraFilterProp::CEditCameraFilterProp( uint32 ID, IEffectClassDesc* pEffectClassDesc )
:CCameraFilterProp(ID,pEffectClassDesc)
{
	SetUnitProp(this);
	// ����������ù���
	SetBaseUintData();
	SetUintPropData();
}

CEditCameraFilterProp::~CEditCameraFilterProp(void)
{
}

void CEditCameraFilterProp::SetUintPropData()
{
	_RegCtrlData(E_INTEGER, NULL, NULL, "����ʱ��", NULL, &m_BirthTime);
	_RegCtrlData(E_INTEGER, NULL, NULL, "����ʱ��", NULL, &m_DeleteTime);
	_RegCtrlData(E_BOOLValue, NULL, NULL, "������ģ��", NULL, &m_bFilterParent);

	CCombDataHelper* pHelper = (CCombDataHelper*)_RegCtrlData(E_COMBO, NULL, NULL, "��������", NULL, &m_eCFilterTp );
	pHelper->PushItem("ƽ��", CAMERAFILTER_RATIO_NONE);
	pHelper->PushItem("���ֱ���", CAMERAFILTER_RATIO_CLAMP);
	
	pHelper = (CCombDataHelper*)_RegCtrlData(E_COMBO, NULL, NULL, "���ģʽ", NULL, &m_eBlendModel);
	pHelper->PushItem("�����", SPRB_NONE);
	pHelper->PushItem("��ˣ�����", SPRB_MULTIPLY);
	pHelper->PushItem("��ӣ�����", SPRB_ADD);

	pHelper = (CCombDataHelper*)_RegCtrlData(E_COMBO, NULL, NULL, "Z����", NULL, &m_eZTest);
	pHelper->PushItem("�ر�", Z_NONE);
	pHelper->PushItem("��", Z_WRITE);
	pHelper->PushItem("ֻ����д", Z_TESTONLY);


	_RegCtrlData(E_FLOAT, NULL, NULL, "Ӱ�췶Χ���x", NULL, &m_xScale);
	_RegCtrlData(E_FLOAT, NULL, NULL, "Ӱ�췶Χ�߶�y", NULL, &m_yScale);
	_RegCtrlData(E_FLOAT, NULL, NULL, "Ӱ�췶Χ���z", NULL, &m_zScale);
	// advanced prop 
	// EFADV_TEXTURE
	_RegCtrlData(E_EEStrMap, NULL, NULL, "��ͼ", NULL, &m_TextName);
	// EFADV_COLOR
	_RegCtrlData(E_ECOLORMap, NULL, NULL, "��ɫ", NULL, &m_Color);
}

int32 CEditCameraFilterProp::GetProp( EFFECT_PROP* pProp, int32 nBufLen )
{
	EVector<EFFECT_PROP> Prop;
	CEditUnitPropHelp::GetBaseProp( Prop );

	Prop.push_back( EFFECT_PROP( "����ʱ��",		m_BirthTime ) );
	Prop.push_back( EFFECT_PROP( "����ʱ��",		m_DeleteTime ) );
	Prop.push_back( EFFECT_PROP( "������ģ��",		m_bFilterParent ) );
	Prop.push_back( EFFECT_PROP( "��������",		CComboData( (BYTE)m_eCFilterTp, "ƽ��,���ֱ���" ) ) );
	Prop.push_back( EFFECT_PROP( "���ģʽ",		CComboData( (BYTE)m_eBlendModel,  "�����,��ˣ�����,��ӣ�����" ) ) );
	Prop.push_back( EFFECT_PROP( "Z����",			CComboData( (BYTE)m_eZTest, "�ر�,��,ֻ����д" ) ) );
	Prop.push_back( EFFECT_PROP( "Ӱ�췶Χ���x",	m_xScale ) );
	Prop.push_back( EFFECT_PROP( "Ӱ�췶Χ�߶�y",	m_yScale ) );
	Prop.push_back( EFFECT_PROP( "Ӱ�췶Χ���z",	m_zScale ) );

	nBufLen = min( nBufLen,(int32) Prop.size() );
	for( int32 i = 0; i < nBufLen; i++ )pProp[i] = Prop[i];
	return nBufLen;
}

void CEditCameraFilterProp::SetProp( const EFFECT_PROP* pProp, int32 nBufLen )
{
	EVector<EFFECT_PROP> Prop( pProp, pProp + nBufLen );
	CEditUnitPropHelp::SetBaseProp( Prop );

	for( size_t i = 0; i < Prop.size(); ++i )
	{
		if( Prop[i].Name() == "����ʱ��" )			m_BirthTime		= Prop[i].Data().Int();
		if( Prop[i].Name() == "����ʱ��" )			m_DeleteTime	= Prop[i].Data().Int();
		if( Prop[i].Name() == "������ģ��" )		m_bFilterParent	= Prop[i].Data().Bool();
		if( Prop[i].Name() == "��������" )			m_eCFilterTp	= (CAMERAFILTER_RATIO)Prop[i].Data().Index();
		if( Prop[i].Name() == "���ģʽ" )			m_eBlendModel	= (SPR_BLENDMODE)Prop[i].Data().Index();
		if( Prop[i].Name() == "Z����" )				m_eZTest		= (Z_TEST)Prop[i].Data().Index();
		if( Prop[i].Name() == "Ӱ�췶Χ���x" )		m_xScale		= fabs( Prop[i].Data().Float() );
		if( Prop[i].Name() == "Ӱ�췶Χ�߶�y" )		m_yScale		= fabs( Prop[i].Data().Float() );
		if( Prop[i].Name() == "Ӱ�췶Χ���z" )		m_zScale		= fabs( Prop[i].Data().Float() );
	}
	_UpdateParams();
}

uint32 CEditCameraFilterProp::GetAdvancedProp( ADVANCE_PROP* pOffset,      int32& nOffLen,
										  ADVANCE_PROP* pTexture,   int32& nTextLen,
										  ADVANCE_PROP* pColor,     int32& nColorLen,
										  ADVANCE_PROP* pScale,     int32& nScaleLen,
										  ADVANCE_PROP* pXYZcale,   int32& nXYZcaleLen,
										  ADVANCE_PROP* pUVRCenter, int32& nUVRCenter)
{
	CEditUnitPropHelp::GetAdvancedProp( pOffset, nOffLen, pTexture, nTextLen, pColor, nColorLen, pScale, nScaleLen ,pXYZcale, nXYZcaleLen, pUVRCenter, nUVRCenter);

	return EFADV_TEXTURE|EFADV_COLOR;
}