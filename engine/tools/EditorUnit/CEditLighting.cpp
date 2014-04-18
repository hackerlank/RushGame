#include "stdafx.h"
#include "CEditLighting.h"
#include "TSqrAllocator.inl"


CEditLightningProp::CEditLightningProp( uint32 ID, IEffectClassDesc* pEffectClassDesc )
:CLightningProp(ID,pEffectClassDesc)
{
	SetUnitProp(this);
	// ����������ù���
	SetBaseUintData();
	SetUintPropData();
}

void CEditLightningProp::_UIDataUpdate()
{
	_UpdateParams();
}

void CEditLightningProp::SetUintPropData()
{
	_RegCtrlData(E_VECTOR, NULL, NULL, "��ת��", NULL, &m_vecRotate);
	_RegCtrlData(E_VECTOR, NULL, NULL, "����", NULL, &m_vecNormal);
	_RegCtrlData(E_FLOAT, NULL, NULL, "��ת��ʼ��", NULL, &m_fStartAngle);
	_RegCtrlData(E_FLOAT, NULL, NULL, "�����", NULL, &m_fWidthRatio);
	_RegCtrlData(E_FLOAT, NULL, NULL, "��С�ߴ�", NULL, &m_fMinSize);
	_RegCtrlData(E_FLOAT, NULL, NULL, "���ߴ�", NULL, &m_fMaxSize);
	_RegCtrlData(E_VECTOR, NULL, NULL, "���", NULL, &m_vecSrc);
	_RegCtrlData(E_FLOAT, NULL, NULL, "��С�𶯷���", NULL, &m_fDitherMin);
	_RegCtrlData(E_FLOAT, NULL, NULL, "����𶯷���", NULL, &m_fDitherMax);
	_RegCtrlData(E_INTEGER, NULL, NULL, "�ֶ���Ŀ", NULL, &m_nSubNum);
	
	CCombDataHelper* pCombHelper = (CCombDataHelper* )_RegCtrlData(E_COMBO, NULL, NULL, "����", NULL, &m_eFaceType);
	pCombHelper->PushItem("����", SPRF_NORMAL);
	pCombHelper->PushItem("��ͷ", SPRF_CAMERA);
	pCombHelper->PushItem("����", SPRF_CAMERANONEX);
	
	pCombHelper = (CCombDataHelper* )_RegCtrlData(E_COMBO, NULL, NULL, "���ģʽ", NULL, &m_eBlendModel);
	pCombHelper->PushItem("�����", SPRB_NONE);
	pCombHelper->PushItem("��ˣ�����", SPRB_MULTIPLY);
	pCombHelper->PushItem("��ӣ�����", SPRB_ADD);
	pCombHelper->PushItem("��Ч��", SPRB_LIGHT);

	pCombHelper = (CCombDataHelper* )_RegCtrlData(E_COMBO, NULL, NULL, "Z����", NULL, &m_eZTest);
	pCombHelper->PushItem("�ر�", Z_NONE);
	pCombHelper->PushItem("��", Z_WRITE);
	pCombHelper->PushItem("ֻ����д", Z_TESTONLY);
	
	_RegCtrlData(E_BOOLValue, NULL, NULL, "���", NULL, &m_bLightEnable);
	_RegCtrlData(E_FLOAT, NULL, NULL, "���ƫ��", NULL, &m_fZBias);

	// advanced prop 
	// EFADV_OFFSET
	_RegCtrlData(E_EVectorMap, NULL, NULL, "�켣", NULL, &m_vecOffset);

	// EFADV_TEXTURE
	_RegCtrlData(E_EEStrMap, NULL, NULL, "��ͼ", NULL, &m_TextName);

	// EFADV_COLOR   
	_RegCtrlData(E_ECOLORMap, NULL, NULL, "��ɫ", NULL, &m_Color);

	// EFADV_SCALE   EMap<float, byte>
	_RegCtrlData(E_EBYTEMap, NULL, NULL, "��С", NULL, &m_Scale);

	// EFADV_XYZCALE;
	_RegCtrlData(E_EVectorMap, NULL, NULL, "XYZ����", NULL, &m_XYZcale);

	// EFADV_XYZCALE;
	_RegCtrlData(E_EVectorMap, NULL, NULL, "������ת����", NULL, &m_UVRCenter);
	
}

int CEditLightningProp::GetProp( EFFECT_PROP* pProp, int nBufLen )
{
	EVector<EFFECT_PROP> Prop;
	CEditUnitPropHelp::GetBaseProp( Prop );

	Prop.push_back( EFFECT_PROP( "��ת��",		m_vecRotate ) );
	Prop.push_back( EFFECT_PROP( "����",		m_vecNormal ) );
	Prop.push_back( EFFECT_PROP( "��ת��ʼ��",	m_fStartAngle ) );
	Prop.push_back( EFFECT_PROP( "�����",		m_fWidthRatio ) );
	Prop.push_back( EFFECT_PROP( "��С�ߴ�",	m_fMinSize ) );
	Prop.push_back( EFFECT_PROP( "���ߴ�",	m_fMaxSize ) );
	Prop.push_back( EFFECT_PROP( "���",		m_vecSrc ) );
	Prop.push_back( EFFECT_PROP( "��С�𶯷���",m_fDitherMin ) );
	Prop.push_back( EFFECT_PROP( "����𶯷���",m_fDitherMax ) );
	Prop.push_back( EFFECT_PROP( "�ֶ���Ŀ",	m_nSubNum ) );
	Prop.push_back( EFFECT_PROP( "����",		CComboData( (BYTE)m_eFaceType, "����,��ͷ,����" ) ) );
	Prop.push_back( EFFECT_PROP( "���ģʽ",	CComboData( (BYTE)m_eBlendModel,  "�����,��ˣ�����,��ӣ�����,��Ч��" ) ) );
	Prop.push_back( EFFECT_PROP( "Z����",		CComboData( (BYTE)m_eZTest, "�ر�,��,ֻ����д" ) ) );
	Prop.push_back( EFFECT_PROP( "���",		CComboData( (BYTE)m_bLightEnable, "�ر�,��" ) ) );
	Prop.push_back( EFFECT_PROP( "���ƫ��",	m_fZBias ) );

	nBufLen = min( nBufLen,(int) Prop.size() );
	for( int i = 0; i < nBufLen; i++ )pProp[i] = Prop[i];
	return nBufLen;
}

void CEditLightningProp::SetProp( const EFFECT_PROP* pProp, int nBufLen )
{
	EVector<EFFECT_PROP> Prop( pProp, pProp + nBufLen );
	CEditUnitPropHelp::SetBaseProp( Prop );

	for( int i = 0; i < (int)Prop.size(); ++i )
	{
		if( Prop[i].Name() == "��ת��" )			m_vecRotate = Prop[i].Data().Vec();
		if( Prop[i].Name() == "����" )				m_vecNormal = Prop[i].Data().Vec();
		if( Prop[i].Name() == "��ת��ʼ��" )		m_fStartAngle  = Prop[i].Data().Float();
		if( Prop[i].Name() == "�����" )			m_fWidthRatio  = Prop[i].Data().Float();
		if( Prop[i].Name() == "��С�ߴ�" )			m_fMinSize  = Prop[i].Data().Float();
		if( Prop[i].Name() == "���ߴ�" )			m_fMaxSize  = Prop[i].Data().Float();
		if( Prop[i].Name() == "���" )				m_vecSrc  = Prop[i].Data().Vec();
		if( Prop[i].Name() == "��С�𶯷���" )		m_fDitherMin  = Prop[i].Data().Float();
		if( Prop[i].Name() == "����𶯷���" )		m_fDitherMax  = max(Prop[i].Data().Float(), 1);
		if( Prop[i].Name() == "�ֶ���Ŀ" )			m_nSubNum  = max( Prop[i].Data().Int(), 1 );
		if( Prop[i].Name() == "����" )				m_eFaceType = (SPR_FACE)Prop[i].Data().Index();
		if( Prop[i].Name() == "���ģʽ" )			m_eBlendModel = (SPR_BLENDMODE)Prop[i].Data().Index();
		if( Prop[i].Name() == "Z����" )				m_eZTest = (Z_TEST)Prop[i].Data().Index();
		if( Prop[i].Name() == "���" )				m_bLightEnable = Prop[i].Data().Index() != 0;
		if( Prop[i].Name() == "���ƫ��" )			m_fZBias = Prop[i].Data().Float();
	}
	_UpdateParams();
}