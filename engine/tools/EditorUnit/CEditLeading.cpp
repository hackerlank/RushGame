#include "stdafx.h"
#include "CEditLeading.h"
#include "TSqrAllocator.inl"


CEditLeadingProp::CEditLeadingProp( uint32 ID, IEffectClassDesc* pEffectClassDesc )
:CLeadingProp(ID,pEffectClassDesc)
{
	SetUnitProp(this);
	// ����������ù���
	SetBaseUintData();
	SetUintPropData();
}

void CEditLeadingProp::SetUintPropData()
{
	_RegCtrlData(E_FLOAT, NULL, NULL, "��ʼ��", NULL, &m_fStartAngle);
	_RegCtrlData(E_FLOAT, NULL, NULL, "��ת�ٶ�", NULL, &m_fRotSpeed);
	_RegCtrlData(E_FLOAT, NULL, NULL, "UV���ٶ�", NULL, &m_fUVSpeed);
	_RegCtrlData(E_INTEGER, NULL, NULL, "��Ƭ��", NULL, &m_nPieceCount);
	_RegCtrlData(E_INTEGER, NULL, NULL, "�ֶ���", NULL, &m_nDouCount);
	_RegCtrlData(E_FLOAT, NULL, NULL, "�����ٷֱ�", NULL, &m_fDouPercent);
	_RegCtrlData(E_FLOAT, NULL, NULL, "�������", NULL, &m_fDouInterval);
	_RegCtrlData(E_INTEGER, NULL, NULL, "��Ծ�ٶ�", NULL, &m_JumpSpeed);
	_RegCtrlData(E_INTEGER, NULL, NULL, "UV���ٶ�", NULL, &m_JumpNum);
	
	CCombDataHelper* pCombHelper = (CCombDataHelper*)_RegCtrlData(E_COMBO, NULL, NULL, "����", NULL, &m_eFaceType);
	pCombHelper->PushItem("����",SPRF_NORMAL);
	pCombHelper->PushItem("��ͷ",SPRF_CAMERA);
	pCombHelper->PushItem("����",SPRF_CAMERANONEX);
	pCombHelper->PushItem("ƽ�з���",SPRF_WORLDNORPAR);
	pCombHelper->PushItem("�ر�",SPRF_TERRAIN);
	pCombHelper->PushItem("�̶�����",SPRF_NORMAL_STATIC);
	pCombHelper->PushItem("�ر���ת",SPRF_TERRAIN_ROTATE);
	
	pCombHelper = (CCombDataHelper*)_RegCtrlData(E_COMBO, NULL, NULL, "���ӷ�ʽ", NULL, &m_eLType);
	pCombHelper->PushItem("��������", LPLK_IMM);
	pCombHelper->PushItem("��������", LPLK_JUMP);
	pCombHelper->PushItem("��Ծ����", LPLK_JUMPONCE);

	pCombHelper = (CCombDataHelper*)_RegCtrlData(E_COMBO, NULL, NULL, "��ʼ��", NULL, &m_eSSType);
	pCombHelper->PushItem("����", LPSS_Player);
	pCombHelper->PushItem("�б�", LPSS_LIST);
	pCombHelper->PushItem("����", LPSS_LINE);

	pCombHelper = (CCombDataHelper*)_RegCtrlData(E_COMBO, NULL, NULL, "���ģʽ",NULL, &m_eBlendModel);
	pCombHelper->PushItem("�����", SPRB_NONE);
	pCombHelper->PushItem("��ˣ�����", SPRB_MULTIPLY);
	pCombHelper->PushItem("��ӣ�����", SPRB_ADD);
	pCombHelper->PushItem("��Ч��", SPRB_LIGHT);

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

int CEditLeadingProp::GetProp( EFFECT_PROP* pProp, int nBufLen )
{
	EVector<EFFECT_PROP> Prop;
	CEditUnitPropHelp::GetBaseProp( Prop );

	Prop.push_back( EFFECT_PROP( "��ʼ��",			m_fStartAngle	) );
	Prop.push_back( EFFECT_PROP( "��ת�ٶ�",		m_fRotSpeed		) );
	Prop.push_back( EFFECT_PROP( "UV���ٶ�",		m_fUVSpeed		) );
	Prop.push_back( EFFECT_PROP( "��Ƭ��",			m_nPieceCount	) );
	Prop.push_back( EFFECT_PROP( "�ֶ���",			m_nDouCount		) );
	Prop.push_back( EFFECT_PROP( "�����ٷֱ�",		m_fDouPercent	) );
	Prop.push_back( EFFECT_PROP( "�������",		m_fDouInterval	) );
	Prop.push_back( EFFECT_PROP( "��Ծ�ٶ�",		m_JumpSpeed		) );
	Prop.push_back( EFFECT_PROP( "��Ծ����",		m_JumpNum		) );
	Prop.push_back( EFFECT_PROP( "����",		CComboData( (BYTE)m_eFaceType, "����,��ͷ,����,ƽ�з���,�ر�,�̶�����,�ر���ת" ) ) );
	Prop.push_back( EFFECT_PROP( "���ӷ�ʽ",	CComboData( (BYTE)m_eLType,  "��������, ��������,��Ծ����" ) ) );
	Prop.push_back( EFFECT_PROP( "��ʼ��",		CComboData( (BYTE)m_eSSType,  "����,�б�,����" ) ) );

	Prop.push_back( EFFECT_PROP( "���ģʽ",	CComboData( (BYTE)m_eBlendModel,  "�����,��ˣ�����,��ӣ�����,��Ч��" ) ) );
	Prop.push_back( EFFECT_PROP( "���OP",		CComboData( (BYTE)m_eTextOp,	  "������,����ɫ,1����,2����,4����,��ɫ,��ɫ��,2����ɫ,��ɫ,ƽ����ɫ,��ɫ���,��ͼ���" ) ) );
	Prop.push_back( EFFECT_PROP( "Z����",		CComboData( (BYTE)m_eZTest, "�ر�,��,ֻ����д" ) ) );
	Prop.push_back( EFFECT_PROP( "���",		CComboData( (BYTE)m_bLightEnable, "�ر�,��" ) ) );
	Prop.push_back( EFFECT_PROP( "���ƫ��",	m_fZBias ) );

	nBufLen = min( nBufLen,(int) Prop.size() );
	for( int i = 0; i < nBufLen; ++i )
		pProp[i] = Prop[i];

	return nBufLen;
}

void CEditLeadingProp::SetProp( const EFFECT_PROP* pProp, int nBufLen )
{
	EVector<EFFECT_PROP> Prop( pProp, pProp + nBufLen );
	CEditUnitPropHelp::SetBaseProp( Prop );

	for( int i = 0; i < (int)Prop.size(); ++i )
	{
		if( Prop[i].Name() == "��ʼ��" )			m_fStartAngle	= Prop[i].Data().Float();
		if( Prop[i].Name() == "��ת�ٶ�" )			m_fRotSpeed		= Prop[i].Data().Float();
		if( Prop[i].Name() == "UV���ٶ�" )			m_fUVSpeed		= Prop[i].Data().Float();
		if( Prop[i].Name() == "��Ƭ��" )			m_nPieceCount	= Prop[i].Data().DWord();
		if( Prop[i].Name() == "�ֶ���" )			m_nDouCount		= Prop[i].Data().DWord();
		if( Prop[i].Name() == "�����ٷֱ�" )		m_fDouPercent	= Prop[i].Data().Float();
		if( Prop[i].Name() == "�������" )			m_fDouInterval	= Prop[i].Data().Float();
		if( Prop[i].Name() == "��Ծ�ٶ�" )			m_JumpSpeed		= Prop[i].Data().DWord();
		if( Prop[i].Name() == "��Ծ����" )			m_JumpNum		= Prop[i].Data().DWord();
		if( Prop[i].Name() == "����" )				m_eFaceType = (SPR_FACE)Prop[i].Data().Index();
		if( Prop[i].Name() == "���ӷ�ʽ" )			m_eLType		= (eLP_LKType)Prop[i].Data().Index();
		if( Prop[i].Name() == "��ʼ��" )			m_eSSType		= (eLP_StartScr)Prop[i].Data().Index();
		if( Prop[i].Name() == "���ģʽ" )			m_eBlendModel	= (SPR_BLENDMODE)Prop[i].Data().Index();
		if( Prop[i].Name() == "���OP" )			m_eTextOp		= (SPR_BLENDOP)Prop[i].Data().Index();
		if( Prop[i].Name() == "Z����" )				m_eZTest		= (Z_TEST)Prop[i].Data().Index();
		if( Prop[i].Name() == "���" )				m_bLightEnable	= Prop[i].Data().Index() != 0;
		if( Prop[i].Name() == "���ƫ��" )			m_fZBias		= Prop[i].Data().Float();
	}

	_UpdateParams();
}