#include "stdafx.h"
#include "CEditParticleSystem.h"
#include "TSqrAllocator.inl"


CEditParticleSystemProp::CEditParticleSystemProp( uint32 ID, IEffectClassDesc* pEffectClassDesc )
:CParticleSystemProp(ID,pEffectClassDesc)
{
	SetUnitProp(this);
	// ����������ù���
	SetBaseUintData();
	SetUintPropData();
}

void CEditParticleSystemProp::_UIDataUpdate()
{
	_UpdateParams();
}

void CEditParticleSystemProp::SetUintPropData()
{
	_RegCtrlData(E_INTEGER, AutoUpdate, this, "����ʱ����", NULL, &m_nEmissionInterval );
	_RegCtrlData(E_INTEGER, AutoUpdate, this, "ÿ�η���������", NULL, &m_nParticlesPerEmission );
	_RegCtrlData(E_FLOAT, NULL, NULL, "��С�ߴ�", NULL, &m_fMinSize );
	_RegCtrlData(E_FLOAT, NULL, NULL, "���ߴ�", NULL, &m_fMaxSize );
	_RegCtrlData(E_VECTOR, NULL, NULL, "����ƽ�淨��", NULL, &m_vEmissionPlan );
	_RegCtrlData(E_VECTOR, NULL, NULL, "��С�ٶ�", NULL, &m_vMinVelocity );
	_RegCtrlData(E_VECTOR, NULL, NULL, "����ٶ�", NULL, &m_vMaxVelocity );
	_RegCtrlData(E_VECTOR, NULL, NULL, "���ٶ�", NULL, &m_vAcceleration );
	_RegCtrlData(E_FLOAT, NULL, NULL, "��С���ٶ�", NULL, &m_fMinAngularVelocity );
	_RegCtrlData(E_FLOAT, NULL, NULL, "�����ٶ�", NULL, &m_fMaxAngularVelocity );
	_RegCtrlData(E_INTEGER, AutoUpdate, this, "��С��������", NULL, &m_nMinLifeSpan );
	_RegCtrlData(E_INTEGER, AutoUpdate, this, "�����������", NULL, &m_nMaxLifeSpan );

	_RegCtrlData(E_FLOAT, NULL, NULL, "��������С�뾶", NULL, &m_fMinRadius );
	_RegCtrlData(E_FLOAT, NULL, NULL, "���������뾶", NULL, &m_fMaxRadius );
	_RegCtrlData(E_FLOAT, NULL, NULL, "���γ����", NULL, &m_fLongShortRadius );
	_RegCtrlData(E_FLOAT, NULL, NULL, "�����ܶȱ�", NULL, &m_fLongShortDRadius );
	_RegCtrlData(E_FLOAT, NULL, NULL, "�ǵ�ƽ��", NULL, &m_fCornerDivision );
	_RegCtrlData(E_FLOAT, NULL, NULL, "���淢�����ٶ�", NULL, &m_fVelocityPercent );


	CCombDataHelper* pCombHelper = (CCombDataHelper*)_RegCtrlData(E_COMBO, NULL, NULL, "����ϵͳ", NULL, &m_eFollowType);
	pCombHelper->PushItem("������", PSPC_Free);
	pCombHelper->PushItem("��ȫ����", PSPC_Follow);
	pCombHelper->PushItem("��ʼ�ٶȸ���", PSPC_AddSpeed);
	pCombHelper->PushItem("��̬����", PSPC_Static);
	pCombHelper->PushItem("���淨��", PSPC_Normal);

	pCombHelper = (CCombDataHelper*)_RegCtrlData(E_COMBO, NULL, NULL, "����������", NULL, &m_eEmissionType);
	pCombHelper->PushItem("��״", PSEMS_eSphere);
	pCombHelper->PushItem("Բ", PSEMS_eCircle);
	pCombHelper->PushItem("����", PSEMS_eRectangle);
	pCombHelper->PushItem("������ͽǵ�", PSEMS_eMulCorner);
	
	pCombHelper = (CCombDataHelper*)_RegCtrlData(E_COMBO, NULL, NULL, "�ٶ�����", NULL, &m_eVelType);
	pCombHelper->PushItem("���", PSVEL_eRANDOM);
	pCombHelper->PushItem("��ԭ�����", PSVEL_eFROMCENTER);
	pCombHelper->PushItem("����", PSVEL_eTOCENTER);
	
	pCombHelper = (CCombDataHelper*)_RegCtrlData(E_COMBO, NULL, NULL, "���ģʽ", NULL, &m_eBlendModel);
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
	
	pCombHelper = (CCombDataHelper*)_RegCtrlData(E_COMBO, NULL, NULL, "������ͼ����", NULL, &m_eTextDir);
	pCombHelper->PushItem("��ͷ", PSTD_eDirCamera);
	pCombHelper->PushItem("���ٶ�", PSTD_eDirVelocity);
	pCombHelper->PushItem("�����ٶȹ̶���", PSTD_eDirFaceToVelocity);
	pCombHelper->PushItem("ʼ�ճ���", PSTD_eDirAlwaysUP);
	pCombHelper->PushItem("����������", PSTD_eDirUpUpUp);
	
	_RegCtrlData(E_FLOAT, NULL, NULL, "�����", NULL, &m_fWidthRatio );
	_RegCtrlData(E_CHARPTR, NULL, NULL, "�ο�����", NULL, &m_sRefSkelatal );
	_RegCtrlData(E_FLOAT, NULL, NULL, "���ƫ��", NULL, &m_fZBias );
	_RegCtrlData(E_VECTOR, NULL, NULL, "�ü��淨����", NULL, &(m_ClipPlane.originPlane.m_Normal) );
	_RegCtrlData(E_EFloatMap, NULL, NULL, "�ü������", NULL, &m_ClipDistMap );

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

	// EFADV_EMIMINRAD
	_RegCtrlData(E_EFloatMap, NULL, NULL, "��������С�뾶", NULL, &m_EmissionMinRadius );

	// EFADV_EMIMAXRAD
	_RegCtrlData(E_EFloatMap, NULL, NULL, "���������뾶", NULL, &m_EmissionMaxRadius );
	
}

int32 CEditParticleSystemProp::GetProp( EFFECT_PROP* pProp, int32 nBufLen )
{
	EVector<EFFECT_PROP> Prop;
	CEditUnitPropHelp::GetBaseProp( Prop );

	Prop.push_back( EFFECT_PROP( "����ʱ����",	m_nEmissionInterval ) );
	Prop.push_back( EFFECT_PROP( "ÿ�η���������",	m_nParticlesPerEmission ) );
	Prop.push_back( EFFECT_PROP( "��С�ߴ�",		m_fMinSize ) );
	Prop.push_back( EFFECT_PROP( "���ߴ�",		m_fMaxSize ) );
	Prop.push_back( EFFECT_PROP( "����ƽ�淨��",	m_vEmissionPlan ) );
	Prop.push_back( EFFECT_PROP( "��С�ٶ�",		m_vMinVelocity ) );
	Prop.push_back( EFFECT_PROP( "����ٶ�",		m_vMaxVelocity ) );
	Prop.push_back( EFFECT_PROP( "���ٶ�",			m_vAcceleration ) );
	Prop.push_back( EFFECT_PROP( "��С���ٶ�",		m_fMinAngularVelocity ) );
	Prop.push_back( EFFECT_PROP( "�����ٶ�",		m_fMaxAngularVelocity ) );
	Prop.push_back( EFFECT_PROP( "��С��������",	m_nMinLifeSpan ) );
	Prop.push_back( EFFECT_PROP( "�����������",	m_nMaxLifeSpan ) );
	Prop.push_back( EFFECT_PROP( "��������С�뾶",	m_fMinRadius ) );
	Prop.push_back( EFFECT_PROP( "���������뾶",	m_fMaxRadius ) );
	Prop.push_back( EFFECT_PROP( "���γ����",		m_fLongShortRadius ) );
	Prop.push_back( EFFECT_PROP( "�����ܶȱ�",		m_fLongShortDRadius ) );
	Prop.push_back( EFFECT_PROP( "�ǵ�ƽ��",		m_fCornerDivision ) );
	Prop.push_back( EFFECT_PROP( "���淢�����ٶ�",	m_fVelocityPercent) );
	Prop.push_back( EFFECT_PROP( "����ϵͳ",		CComboData( (BYTE)m_eFollowType, "������,��ȫ����,��ʼ�ٶȸ���,��̬����,���淨��" ) ) );
	Prop.push_back( EFFECT_PROP( "����������",		CComboData( (BYTE)m_eEmissionType, "��״,Բ,����,������ͽǵ�" ) ) );
	Prop.push_back( EFFECT_PROP( "�ٶ�����",		CComboData( (BYTE)m_eVelType, "���,��ԭ�����,����" ) ) );
	Prop.push_back( EFFECT_PROP( "���ģʽ",		CComboData( (BYTE)m_eBlendModel, "�����,��ˣ�����,��ӣ�����" ) ) );
	Prop.push_back( EFFECT_PROP( "���OP",		CComboData( (BYTE)m_eTextOp,	  "������,����ɫ,1����,2����,4����,��ɫ,��ɫ��,2����ɫ,��ɫ,ƽ����ɫ,��ɫ���,��ͼ���" ) ) );
	Prop.push_back( EFFECT_PROP( "Z����",			CComboData( (BYTE)m_eZTest, "�ر�,��,ֻ����д" ) ) );
	Prop.push_back( EFFECT_PROP( "���",			CComboData( (BYTE)m_bLightEnable, "�ر�,��" ) ) );
	Prop.push_back( EFFECT_PROP( "������ͼ����",	CComboData( (BYTE)m_eTextDir, "��ͷ,���ٶ�,�����ٶȹ̶���,ʼ�ճ���,����������" ) ) );
	Prop.push_back( EFFECT_PROP( "�����",			m_fWidthRatio ) );
	Prop.push_back( EFFECT_PROP( "�ο�����",		sqr::CStringData( 0, (const char*)m_sRefSkelatal ) ) );
	Prop.push_back( EFFECT_PROP( "���ƫ��",		m_fZBias ) );
	Prop.push_back( EFFECT_PROP( "�ü��淨����", m_ClipPlane.originPlane.m_Normal ) );
	Prop.push_back( EFFECT_PROP( "�ü������", m_ClipDistMap ) ) ;

	nBufLen = min( nBufLen,(int32) Prop.size() );
	for( int32 i = 0; i < nBufLen; ++i )pProp[i] = Prop[i];
	return nBufLen;
}

void CEditParticleSystemProp::SetProp( const EFFECT_PROP* pProp, int32 nBufLen )
{
	EVector<EFFECT_PROP> Prop( pProp, pProp + nBufLen );
	CEditUnitPropHelp::SetBaseProp( Prop );

	for( size_t i = 0; i < Prop.size(); ++i )
	{
		if( Prop[i].Name() == "����ʱ����" )		m_nEmissionInterval = max( Prop[i].Data().Int(), 1 );
		if( Prop[i].Name() == "ÿ�η���������" )	m_nParticlesPerEmission = Prop[i].Data().Int();
		if( Prop[i].Name() == "��С�ߴ�" )			m_fMinSize  = Prop[i].Data().Float();
		if( Prop[i].Name() == "���ߴ�" )			m_fMaxSize  = Prop[i].Data().Float();
		if( Prop[i].Name() == "����ƽ�淨��" )		m_vEmissionPlan = Prop[i].Data().Vec();
		if( Prop[i].Name() == "��С�ٶ�" )			m_vMinVelocity = Prop[i].Data().Vec();
		if( Prop[i].Name() == "����ٶ�" )			m_vMaxVelocity = Prop[i].Data().Vec();
		if( Prop[i].Name() == "���ٶ�" )			m_vAcceleration = Prop[i].Data().Vec();
		if( Prop[i].Name() == "��С���ٶ�" )		m_fMinAngularVelocity = Prop[i].Data().Float();
		if( Prop[i].Name() == "�����ٶ�" )		m_fMaxAngularVelocity = Prop[i].Data().Float();
		if( Prop[i].Name() == "��С��������" )		m_nMinLifeSpan = Prop[i].Data().Int();
		if( Prop[i].Name() == "�����������" )		m_nMaxLifeSpan = Prop[i].Data().Int();
		if( Prop[i].Name() == "��������С�뾶" )	m_fMinRadius = Prop[i].Data().Float();
		if( Prop[i].Name() == "���������뾶" )	m_fMaxRadius = Prop[i].Data().Float();
		if( Prop[i].Name() == "���γ����" )		m_fLongShortRadius = Prop[i].Data().Float();
		if( Prop[i].Name() == "�����ܶȱ�" )		m_fLongShortDRadius = Prop[i].Data().Float();
		if( Prop[i].Name() == "�ǵ�ƽ��" )			m_fCornerDivision = Prop[i].Data().Float();
		if( Prop[i].Name() == "���淢�����ٶ�" )	m_fVelocityPercent = Prop[i].Data().Float();
		if( Prop[i].Name() == "����ϵͳ" )			m_eFollowType = (ePS_ParticlCoord)Prop[i].Data().Index();
		if( Prop[i].Name() == "����������" )		m_eEmissionType = (ePS_EMSType)Prop[i].Data().Index();
		if( Prop[i].Name() == "�ٶ�����" )			m_eVelType = (ePS_VELType)Prop[i].Data().Index();
		if( Prop[i].Name() == "���ģʽ" )			m_eBlendModel = (SPR_BLENDMODE)Prop[i].Data().Index();
		if( Prop[i].Name() == "���OP" )			m_eTextOp = (SPR_BLENDOP)Prop[i].Data().Index();
		if( Prop[i].Name() == "Z����" )				m_eZTest = (Z_TEST)Prop[i].Data().Index();
		if( Prop[i].Name() == "���" )				m_bLightEnable = (0!=Prop[i].Data().Index());
		if( Prop[i].Name() == "������ͼ����" )		m_eTextDir = (ePS_TextureDir)Prop[i].Data().Index();
		if( Prop[i].Name() == "�����" )			m_fWidthRatio  = Prop[i].Data().Float();
		if( Prop[i].Name() == "�ο�����" )			m_sRefSkelatal = Prop[i].Data().Str();
		if( Prop[i].Name() == "���ƫ��" )			m_fZBias = Prop[i].Data().Float();
		if (Prop[i].Name() == "�ü��淨����")		m_ClipPlane.originPlane.m_Normal = Prop[i].Data().Vec();
		if (Prop[i].Name() == "�ü������")			m_ClipDistMap = *Prop[i].Data().FloatList();
	}
	_UpdateParams();
}

void CEditParticleSystemProp::SetAdvancedProp( const ADVANCE_PROP* pOffset, int32 nOffLen,
										  const ADVANCE_PROP* pTexture,	int32 nTextLen,
										  const ADVANCE_PROP* pColor,		int32 nColorLen,
										  const ADVANCE_PROP* pScale,		int32 nScaleLen,
										  const ADVANCE_PROP* pXYZcale,	int32 nXYZcale,
										  const ADVANCE_PROP* pEmiMinRad,	int32 nEmissionMinRadius,
										  const ADVANCE_PROP* pEmiMaxRad,  int32 nEmissionMaxRadius)
{
	CEditUnitPropHelp::SetAdvancedProp( pOffset, nOffLen, pTexture, nTextLen, pColor, nColorLen, pScale, nScaleLen, pXYZcale, nXYZcale, pEmiMinRad, nEmissionMinRadius, pEmiMaxRad, nEmissionMaxRadius );
}
