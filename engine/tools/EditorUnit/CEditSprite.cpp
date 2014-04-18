#include "stdafx.h"
#include "CEditSprite.h"
#include "TSqrAllocator.inl"


CEditSpriteProp::CEditSpriteProp( uint32 ID, IEffectClassDesc* pEffectClassDesc )
:CSpriteProp(ID,pEffectClassDesc)
{
	SetUnitProp(this);
	// ����������ù���
	SetBaseUintData();
	SetUintPropData();
}

void CEditSpriteProp::_UIDataUpdate()
{
	_UpdateParams();
}

void CEditSpriteProp::SetUintPropData()
{
	_RegCtrlData(E_INTEGER, NULL, NULL, "����ʱ��", NULL, &m_BirthTime );
	_RegCtrlData(E_INTEGER, NULL, NULL, "����ʱ��", NULL, &m_DeleteTime );
	_RegCtrlData(E_VECTOR, NULL, NULL, "��ת��", NULL, &m_vecRotate );
	_RegCtrlData(E_VECTOR, NULL, NULL, "����", NULL, &m_vecNormal );

	_RegCtrlData(E_FLOAT, NULL, NULL, "��ת��ʼ��", NULL, &m_fStartAngle );
	_RegCtrlData(E_FLOAT, NULL, NULL, "�����", NULL, &m_fWidthRatio );
	_RegCtrlData(E_FLOAT, NULL, NULL, "��С�ߴ�", NULL, &m_fMinSize );
	_RegCtrlData(E_FLOAT, NULL, NULL, "���ߴ�", NULL, &m_fMaxSize );

	CCombDataHelper* pCombHelper = (CCombDataHelper*)_RegCtrlData(E_COMBO, NULL, NULL, "����", NULL, &m_eFaceType);
	pCombHelper->PushItem("����", SPRF_NORMAL);
	pCombHelper->PushItem("��ͷ", SPRF_CAMERA);
	pCombHelper->PushItem("����", SPRF_CAMERANONEX);
	pCombHelper->PushItem("ƽ�з���", SPRF_WORLDNORPAR);
	pCombHelper->PushItem("�ر�", SPRF_TERRAIN);
	pCombHelper->PushItem("�̶�����", SPRF_NORMAL_STATIC);
	pCombHelper->PushItem("�ر���ת", SPRF_TERRAIN_ROTATE);
	
	pCombHelper = (CCombDataHelper*)_RegCtrlData(E_COMBO, NULL, NULL, "���ģʽ", NULL, &m_eBlendModel);
	pCombHelper->PushItem("�����", SPRB_NONE);
	pCombHelper->PushItem("��ˣ�����", SPRB_MULTIPLY);
	pCombHelper->PushItem("��ӣ�����", SPRB_ADD);
	pCombHelper->PushItem("��Ч��", SPRB_LIGHT);
	
	pCombHelper = (CCombDataHelper*)_RegCtrlData(E_COMBO, NULL, NULL, "���OP", NULL, &m_eTextOp);
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
	_RegCtrlData(E_BOOLValue, NULL, NULL, "��ʾ�߿�", NULL, &m_isShowFrame );
	_RegCtrlData(E_FLOAT, NULL, NULL, "���ƫ��", NULL, &m_fZBias );

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

int32 CEditSpriteProp::GetProp( EFFECT_PROP* pProp, int32 nBufLen )
{
	EVector<EFFECT_PROP> Prop;
	CEditUnitPropHelp::GetBaseProp( Prop );

	Prop.push_back( EFFECT_PROP( "����ʱ��",	m_BirthTime ) );
	Prop.push_back( EFFECT_PROP( "����ʱ��",	m_DeleteTime ) );

	Prop.push_back( EFFECT_PROP( "��ת��",		m_vecRotate ) );
	Prop.push_back( EFFECT_PROP( "����",		m_vecNormal ) );
	Prop.push_back( EFFECT_PROP( "��ת��ʼ��",	m_fStartAngle ) );
	Prop.push_back( EFFECT_PROP( "�����",		m_fWidthRatio ) );
	Prop.push_back( EFFECT_PROP( "��С�ߴ�",	m_fMinSize ) );
	Prop.push_back( EFFECT_PROP( "���ߴ�",	m_fMaxSize ) );
	Prop.push_back( EFFECT_PROP( "����",		CComboData( (BYTE)m_eFaceType, "����,��ͷ,����,ƽ�з���,�ر�,�̶�����,�ر���ת" ) ) );
	Prop.push_back( EFFECT_PROP( "���ģʽ",	CComboData( (BYTE)m_eBlendModel,  "�����,��ˣ�����,��ӣ�����,��Ч��" ) ) );
	Prop.push_back( EFFECT_PROP( "���OP",		CComboData( (BYTE)m_eTextOp,	  "������,����ɫ,1����,2����,4����,��ɫ,��ɫ��,2����ɫ,��ɫ,ƽ����ɫ,��ɫ���,��ͼ���" ) ) );
	Prop.push_back( EFFECT_PROP( "Z����",		CComboData( (BYTE)m_eZTest, "�ر�,��,ֻ����д" ) ) );
	Prop.push_back( EFFECT_PROP( "���",		CComboData( (BYTE)m_bLightEnable, "�ر�,��" ) ) );
	Prop.push_back( EFFECT_PROP( "�߿�",		CComboData( (BYTE)m_isShowFrame, "�ر�,��" ) ) );
	Prop.push_back( EFFECT_PROP( "���ƫ��",	m_fZBias ) );

	nBufLen = min( nBufLen,(int32) Prop.size() );
	for( int32 i = 0; i < nBufLen; ++i )pProp[i] = Prop[i];
	return nBufLen;
}

void CEditSpriteProp::SetProp( const EFFECT_PROP* pProp, int32 nBufLen )
{
	EVector<EFFECT_PROP> Prop( pProp, pProp + nBufLen );
	CEditUnitPropHelp::SetBaseProp( Prop );

	for( size_t i = 0; i < Prop.size(); ++i )
	{
		if( Prop[i].Name() == "����ʱ��" )			m_BirthTime = Prop[i].Data().Int();
		if( Prop[i].Name() == "����ʱ��" )			m_DeleteTime = Prop[i].Data().Int();
		if( Prop[i].Name() == "��ת��" )			m_vecRotate = Prop[i].Data().Vec();
		if( Prop[i].Name() == "����" )				m_vecNormal = Prop[i].Data().Vec();
		if( Prop[i].Name() == "��ת��ʼ��" )		m_fStartAngle  = Prop[i].Data().Float();
		if( Prop[i].Name() == "�����" )			m_fWidthRatio  = Prop[i].Data().Float();
		if( Prop[i].Name() == "��С�ߴ�" )			m_fMinSize  = Prop[i].Data().Float();
		if( Prop[i].Name() == "���ߴ�" )			m_fMaxSize  = Prop[i].Data().Float();
		if( Prop[i].Name() == "����" )				m_eFaceType = (SPR_FACE)Prop[i].Data().Index();
		if( Prop[i].Name() == "���ģʽ" )			m_eBlendModel = (SPR_BLENDMODE)Prop[i].Data().Index();
		if( Prop[i].Name() == "���OP" )			m_eTextOp = (SPR_BLENDOP)Prop[i].Data().Index();
		if( Prop[i].Name() == "Z����" )				m_eZTest = (Z_TEST)Prop[i].Data().Index();
		if( Prop[i].Name() == "���" )				m_bLightEnable = (Prop[i].Data().Index()!=0);
		if( Prop[i].Name() == "�߿�" )				m_isShowFrame = (Prop[i].Data().Index()!=0);
		if( Prop[i].Name() == "���ƫ��" )			m_fZBias = Prop[i].Data().Float();
	}
	_UpdateParams();
}
