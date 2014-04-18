#include "stdafx.h"
#include "CEditPolyTrail.h"
#include "TSqrAllocator.inl"


CEditPolyTrailProp::CEditPolyTrailProp( uint32 ID, IEffectClassDesc* pEffectClassDesc )
:CPolyTrailProp(ID,pEffectClassDesc)
{
	SetUnitProp(this);
	// ����������ù���
	SetBaseUintData();
	SetUintPropData();
}

void CEditPolyTrailProp::_UIDataUpdate()
{
	_UpdateParams();
}

void CEditPolyTrailProp::SetUintPropData()
{
	_RegCtrlData(E_VECTOR, NULL, NULL, "��ת��", NULL, &m_vecRotate );
	_RegCtrlData(E_FLOAT, NULL, NULL, "�켣��С���", NULL, &m_MinTrailWidth );
	_RegCtrlData(E_FLOAT, NULL, NULL, "�켣�����", NULL, &m_MaxTrailWidth );
	_RegCtrlData(E_INTEGER, NULL, NULL, "�켣��Ԫ��������", NULL, &m_UnitLifeSpan );
	_RegCtrlData(E_INTEGER, NULL, NULL, "�켣����˥���ٶ�", NULL, &m_DeclTime );
	_RegCtrlData(E_INTEGER, NULL, NULL, "Բ����", NULL, &m_Smooth );

	CCombDataHelper* pCombHelper = (CCombDataHelper*)_RegCtrlData(E_COMBO, NULL, NULL, "���ģʽ", NULL, &m_eBlendModel);
	pCombHelper->PushItem("�����", SPRB_NONE);
	pCombHelper->PushItem("��ˣ�����", SPRB_MULTIPLY);
	pCombHelper->PushItem("��ӣ�����", SPRB_ADD);

	pCombHelper = (CCombDataHelper*)_RegCtrlData(E_COMBO, NULL, NULL, "���OP", NULL, &m_eTextOp);
	pCombHelper->PushItem("������", SPRO_SELECTARG1);
	pCombHelper->PushItem("����ɫ", SPRO_SELECTARG2);
	pCombHelper->PushItem("1����", SPRO_MODULATE);
	pCombHelper->PushItem("2����", SPRO_MODULATE2X);
	pCombHelper->PushItem("4����", SPRO_MODULATE4X);
	pCombHelper->PushItem("��ɫ", SPRO_ADD);
	pCombHelper->PushItem("��ɫ��", SPRO_ADDSIGNED);
	pCombHelper->PushItem("2����ɫ", SPRO_ADDSIGNED2X);
	pCombHelper->PushItem("��ɫ", SPRO_SUBTRACT);
	pCombHelper->PushItem("ƽ����ɫ", SPRO_ADDSMOOTH);
	pCombHelper->PushItem("��ɫ���", SPRO_BLENDDIFFUSEALPHA);
	pCombHelper->PushItem("��ͼ���", SPRO_BLENDTEXTUREALPHA);
	
	pCombHelper = (CCombDataHelper*)_RegCtrlData(E_COMBO, NULL, NULL, "Z����", NULL, &m_eZTest);
	pCombHelper->PushItem("�ر�", Z_NONE);
	pCombHelper->PushItem("��", Z_WRITE);
	pCombHelper->PushItem("ֻ����д", Z_TESTONLY);
	
	_RegCtrlData(E_BOOLValue, NULL, NULL, "���", NULL, &m_bLightEnable );
	
	pCombHelper = (CCombDataHelper*)_RegCtrlData(E_COMBO, NULL, NULL, "����", NULL, &m_eStrip);
	pCombHelper->PushItem("��������", PTT_BLOCK);
	pCombHelper->PushItem("������", PTT_TRAIL);
	pCombHelper->PushItem("��ͼ����", PTT_STRECTH);
	
	_RegCtrlData(E_BOOLValue, NULL, NULL, "������չ", NULL, &m_bSingleSide );
	_RegCtrlData(E_CHARPTR, NULL, NULL, "�ο�����", NULL, &m_sRefSkelatal );
	_RegCtrlData(E_FLOAT, NULL, NULL, "������", NULL, &m_fRefractIndex );

	// advanced prop 
	// EFADV_OFFSET
	_RegCtrlData(E_EVectorMap, NULL, NULL, "�켣", NULL, &m_vecOffset );

	// EFADV_TEXTURE
	_RegCtrlData(E_EEStrMap, NULL, NULL, "��ͼ", NULL, &m_TextName );

	// EFADV_COLOR   
	_RegCtrlData(E_ECOLORMap, NULL, NULL, "��ɫ", NULL, &m_Color );

	// EFADV_SCALE   EMap<float, byte>
	_RegCtrlData(E_EBYTEMap, NULL, NULL, "��С", NULL, &m_Scale );

	// EFADV_XYZCALE;
	_RegCtrlData(E_EVectorMap, NULL, NULL, "XYZ����", NULL, &m_XYZcale );


	// EFADV_XYZCALE;
	_RegCtrlData(E_EVectorMap, NULL, NULL, "������ת����", NULL, &m_UVRCenter );
	
}

int32 CEditPolyTrailProp::GetProp( EFFECT_PROP* pProp, int32 nBufLen )
{
	EVector<EFFECT_PROP> Prop;
	CEditUnitPropHelp::GetBaseProp( Prop );

	Prop.push_back( EFFECT_PROP( "��ת��",				m_vecRotate ) );
	Prop.push_back( EFFECT_PROP( "�켣��С���",		m_MinTrailWidth ) );
	Prop.push_back( EFFECT_PROP( "�켣�����",		m_MaxTrailWidth ) );
	Prop.push_back( EFFECT_PROP( "�켣��Ԫ��������",	m_UnitLifeSpan ) );
	Prop.push_back( EFFECT_PROP( "�켣����˥���ٶ�",	m_DeclTime ) );
	Prop.push_back( EFFECT_PROP( "Բ����",				m_Smooth) );
	Prop.push_back( EFFECT_PROP( "���ģʽ",			CComboData( (BYTE)m_eBlendModel,  "�����,��ˣ�����,��ӣ�����" ) ) );
	Prop.push_back( EFFECT_PROP( "���OP",				CComboData( (BYTE)m_eTextOp,	  "������,����ɫ,1����,2����,4����,��ɫ,��ɫ��,2����ɫ,��ɫ,ƽ����ɫ,��ɫ���,��ͼ���" ) ) );
	Prop.push_back( EFFECT_PROP( "Z����",				CComboData( (BYTE)m_eZTest, "�ر�,��,ֻ����д" ) ) );
	Prop.push_back( EFFECT_PROP( "���",				CComboData( (BYTE)m_bLightEnable, "�ر�,��" ) ) );
	Prop.push_back( EFFECT_PROP( "����",				CComboData( (BYTE)m_eStrip , "��������,������,��ͼ����" ) ) );
	Prop.push_back( EFFECT_PROP( "��չ",				CComboData( (BYTE)m_bSingleSide , "˫��,����" ) ) );
	Prop.push_back( EFFECT_PROP( "�ο�����",			sqr::CStringData( 0, (const char*)m_sRefSkelatal ) ) );
	Prop.push_back(EFFECT_PROP("������",		m_fRefractIndex));

	nBufLen = min( nBufLen,(int32) Prop.size() );
	for( int32 i = 0; i < nBufLen; ++i )pProp[i] = Prop[i];
	return nBufLen;
}

void CEditPolyTrailProp::SetProp( const EFFECT_PROP* pProp, int32 nBufLen )
{
	EVector<EFFECT_PROP> Prop( pProp, pProp + nBufLen );
	CEditUnitPropHelp::SetBaseProp( Prop );

	for( size_t i = 0; i < Prop.size(); ++i )
	{
		if( Prop[i].Name() == "��ת��" )			m_vecRotate = Prop[i].Data().Vec();
		if( Prop[i].Name() == "�켣��С���" )		m_MinTrailWidth = Prop[i].Data().Float();
		if( Prop[i].Name() == "�켣�����" )		m_MaxTrailWidth = Prop[i].Data().Float();
		if( Prop[i].Name() == "�켣��Ԫ��������" )	m_UnitLifeSpan = Prop[i].Data().Int();
		if( Prop[i].Name() == "�켣����˥���ٶ�" )m_DeclTime = max( Prop[i].Data().Int() , 1 );
		if( Prop[i].Name() == "Բ����" )			m_Smooth = max( Prop[i].Data().Int() , 1 );
		if( Prop[i].Name() == "���ģʽ" )			m_eBlendModel = (SPR_BLENDMODE)Prop[i].Data().Index();
		if( Prop[i].Name() == "���OP" )			m_eTextOp = (SPR_BLENDOP)Prop[i].Data().Index();
		if( Prop[i].Name() == "Z����" )				m_eZTest = (Z_TEST)Prop[i].Data().Index();
		if( Prop[i].Name() == "���" )				m_bLightEnable = (0!=Prop[i].Data().Index());
		if( Prop[i].Name() == "����" )				m_eStrip = (POLY_TRAIL_TYPE)Prop[i].Data().Index();
		if( Prop[i].Name() == "��չ" )				m_bSingleSide = (0!=Prop[i].Data().Index());
		if( Prop[i].Name() == "�ο�����" )			m_sRefSkelatal = Prop[i].Data().Str();
		if (Prop[i].Name() == "������")			m_fRefractIndex = Prop[i].Data().Float();
	}

	_UpdateParams();
}

void CEditPolyTrailProp::SetAdvancedProp( const ADVANCE_PROP* pOffset,  int32 nOffLen,
									 const ADVANCE_PROP* pTexture, int32 nTextLen,
									 const ADVANCE_PROP* pColor,   int32 nColorLen,
									 const ADVANCE_PROP* pScale,   int32 nScaleLen )
{
	CEditUnitPropHelp::SetAdvancedProp( pOffset, nOffLen, pTexture, nTextLen, pColor, nColorLen, pScale, nScaleLen );
	m_OrgScale = m_MinTrailWidth + ( m_MaxTrailWidth - m_MinTrailWidth )*GetMapValue( m_Scale, 0.0f )/255.0f;
}
