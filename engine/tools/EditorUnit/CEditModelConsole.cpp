#include "stdafx.h"
#include "CEditModelConsole.h"
#include "CSkeletalsMgr.h"
#include "TSqrAllocator.inl"


CEditModelConsoleProp::CEditModelConsoleProp( uint32 ID, IEffectClassDesc* pEffectClassDesc )
:CModelConsoleProp(ID,pEffectClassDesc)
{
	SetUnitProp(this);
	// ����������ù���
	SetBaseUintData();
	SetUintPropData();
}

void CEditModelConsoleProp::_UIDataUpdate()
{
	_UpdateParams();
	CCombDataHelper* pHelper0 = (CCombDataHelper*)_ResetCtrlData(E_COMBO, NULL, NULL, "piece0", NULL, &m_nPiece0);
	pHelper0->PushItem("None", 0);
	CCombDataHelper* pHelper1 = (CCombDataHelper*)_ResetCtrlData(E_COMBO, NULL, NULL, "piece1", NULL, &m_nPiece1);
	pHelper1->PushItem("None", 0);
	CCombDataHelper* pHelper2 = (CCombDataHelper*)_ResetCtrlData(E_COMBO, NULL, NULL, "piece2", NULL, &m_nPiece2);
	pHelper2->PushItem("None", 0);
	CCombDataHelper* pHelper3 = (CCombDataHelper*)_ResetCtrlData(E_COMBO, NULL, NULL, "piece3", NULL, &m_nPiece3);
	pHelper3->PushItem("None", 0);
	CCombDataHelper* pHelper4 = (CCombDataHelper*)_ResetCtrlData(E_COMBO, NULL, NULL, "piece4", NULL, &m_nPiece4);
	pHelper4->PushItem("None", 0);
	CCombDataHelper* pHelper5 = (CCombDataHelper*)_ResetCtrlData(E_COMBO, NULL, NULL, "piece5", NULL, &m_nPiece5);
	pHelper5->PushItem("None", 0);

	// PieceList
	for( size_t i = 0; m_pPieceGroup && i < m_pPieceGroup->GetPieceClassNum(); ++i )
	{
		pHelper0->PushItem(m_pPieceGroup->GetPieceClassName(i), i+1);
		pHelper1->PushItem(m_pPieceGroup->GetPieceClassName(i), i+1);
		pHelper2->PushItem(m_pPieceGroup->GetPieceClassName(i), i+1);
		pHelper3->PushItem(m_pPieceGroup->GetPieceClassName(i), i+1);
		pHelper4->PushItem(m_pPieceGroup->GetPieceClassName(i), i+1);
		pHelper5->PushItem(m_pPieceGroup->GetPieceClassName(i), i+1);
	}

	CCombDataHelper* pHelperAni = (CCombDataHelper*)_ResetCtrlData(E_COMBO, NULL, NULL, "������", NULL, &m_Animation);
	for( int32 j = 0; m_pPieceGroup && j < m_pPieceGroup->GetPieceClassAniNum(); ++j )
	{
		pHelperAni->PushItem(m_pPieceGroup->GetPieceClassAniName(j), j);
	}

	CCombDataHelper* pHelperSke = (CCombDataHelper*)_ResetCtrlData(E_COMBO, NULL, NULL, "�ӹ�������", NULL, &m_nSkIdx);
	pHelperSke->PushItem("None", 0);
	for( int32 i = 0; m_pSkeletalFrame && i < m_pSkeletalFrame->GetSkeletalTotalNum(); ++i )
	{
		pHelperSke->PushItem(m_pSkeletalFrame->GetSkeletalName(i), i+1);
	}
}

void CEditModelConsoleProp::_GetListNames(EString* PieceList, EString* AniList, EString* SKList)
{
	ESet<EString> PieceAniName;
	for( int32 j = 0; m_pPieceGroup && j < m_pPieceGroup->GetPieceClassAniNum(); ++j )
		PieceAniName.insert( m_pPieceGroup->GetPieceClassAniName( j ) );
	// PieceList
	for( size_t i = 0; m_pPieceGroup && i < m_pPieceGroup->GetPieceClassNum(); ++i )
	{
		(*PieceList) += ",";
		(*PieceList) += m_pPieceGroup->GetPieceClassName( i );;
	}
	if( (*PieceList).size() > 120 )	(*PieceList)[120] = 0;

	// AniList
	for( ESet<EString>::iterator it = PieceAniName.begin(); it != PieceAniName.end(); ++it )
	{
		if( it != PieceAniName.begin() )
			(*AniList) += ",";
		(*AniList) += *it;
	}
	for( int32 i = PieceAniName.size(); m_pAnimationGroup 
		&& i < m_pAnimationGroup->GetAnimationNum() + (int32)PieceAniName.size(); ++i )
	{
		if( i )
			(*AniList) += ",";
		(*AniList) += m_pAnimationGroup->GetAnimationName(i);
	}
	if( (*AniList).size() > 120 )		(*AniList)[120] = 0;

	// SkList
	for( int32 i = 0; m_pSkeletalFrame && i < m_pSkeletalFrame->GetSkeletalTotalNum(); ++i )
	{
		(*SKList) += ",";
		(*SKList) += m_pSkeletalFrame->GetSkeletalName(i);
	}
}

void CEditModelConsoleProp::_GetListNames(vector<EString>& PieceList, vector<EString>& AniList, vector<EString>& SKList)
{
	// PieceList
	PieceList.push_back("None");
	for( size_t i = 0; m_pPieceGroup && i < m_pPieceGroup->GetPieceClassNum(); ++i )
		PieceList.push_back(m_pPieceGroup->GetPieceClassName(i));

	// AniList
	for( int32 j = 0; m_pPieceGroup && j < m_pPieceGroup->GetPieceClassAniNum(); ++j )
		AniList.push_back(m_pPieceGroup->GetPieceClassAniName(j));

	// SkList
	for( int32 i = 0; m_pSkeletalFrame && i < m_pSkeletalFrame->GetSkeletalTotalNum(); ++i )
		SKList.push_back(m_pSkeletalFrame->GetSkeletalName(i));
}

void CEditModelConsoleProp::SetUintPropData()
{
	vector<EString> PieceList;
	vector<EString> AniList;
	vector<EString> SkList;
	_GetListNames(PieceList, AniList, SkList);
	
	// add end
	_RegCtrlData(E_INTEGER, NULL, NULL, "����ʱ��", NULL, &m_BirthTime );
	_RegCtrlData(E_INTEGER, NULL, NULL, "����ʱ��", NULL, &m_DeleteTime );
	_RegCtrlData(E_VECTOR, NULL, NULL, "��ת��", NULL, &m_vecRotate );
	_RegCtrlData(E_ESTRINGBUTTON, AutoUpdate, this, "MOD�ļ���", "ģ���ļ�(*.mod)|*.mod", &m_szARPName );
	
	CCombDataHelper* pCombHelper = (CCombDataHelper*)_RegCtrlData(E_COMBO, NULL, NULL, "piece0", NULL, &m_nPiece0);
	for ( size_t i=0; i<PieceList.size(); ++i )
		pCombHelper->PushItem(PieceList[i].c_str(), i);

	_RegCtrlData(E_INTEGER, NULL, NULL, "piece0��Ⱦ����", NULL, &m_nRS0 );
	
	pCombHelper = (CCombDataHelper*)_RegCtrlData(E_COMBO, NULL, NULL, "piece1", NULL, &m_nPiece1);
	for ( size_t i=0; i<PieceList.size(); ++i )
		pCombHelper->PushItem(PieceList[i].c_str(), i);
	
	_RegCtrlData(E_INTEGER, NULL, NULL, "piece1��Ⱦ����", NULL, &m_nRS1 );
	
	pCombHelper = (CCombDataHelper*)_RegCtrlData(E_COMBO, NULL, NULL, "piece2", NULL, &m_nPiece2);
	for ( size_t i=0; i<PieceList.size(); ++i )
		pCombHelper->PushItem(PieceList[i].c_str(), i);
	
	_RegCtrlData(E_INTEGER, NULL, NULL, "piece2��Ⱦ����", NULL, &m_nRS2 );
	
	pCombHelper = (CCombDataHelper*)_RegCtrlData(E_COMBO, NULL, NULL, "piece3", NULL, &m_nPiece3);
	for ( size_t i=0; i<PieceList.size(); ++i )
		pCombHelper->PushItem(PieceList[i].c_str(), i);
	
	_RegCtrlData(E_INTEGER, NULL, NULL, "piece3��Ⱦ����", NULL, &m_nRS3 );
	
	pCombHelper = (CCombDataHelper*)_RegCtrlData(E_COMBO, NULL, NULL, "piece4", NULL, &m_nPiece4);
	for ( size_t i=0; i<PieceList.size(); ++i )
		pCombHelper->PushItem(PieceList[i].c_str(), i);
	
	_RegCtrlData(E_INTEGER, NULL, NULL, "piece4��Ⱦ����", NULL, &m_nRS4 );
	
	pCombHelper = (CCombDataHelper*)_RegCtrlData(E_COMBO, NULL, NULL, "piece5", NULL, &m_nPiece5);
	for ( size_t i=0; i<PieceList.size(); ++i )
		pCombHelper->PushItem(PieceList[i].c_str(), i);
	
	_RegCtrlData(E_INTEGER, NULL, NULL, "piece5��Ⱦ����", NULL, &m_nRS5 );
	_RegCtrlData(E_ESTRINGBUTTON, AutoUpdate, this, "SKE�ļ���", "��Чģ�Ͷ����ļ�(*.agp)|*.agp", &m_szARAName );
	
	pCombHelper = (CCombDataHelper*)_RegCtrlData(E_COMBO, NULL, NULL, "������", NULL, &m_Animation);
	for ( size_t i=0; i<AniList.size(); ++i )
		pCombHelper->PushItem(AniList[i].c_str(), i);
	
	_RegCtrlData(E_FLOAT, NULL, NULL, "X��С�ߴ�", NULL, &m_fMinXSize );
	_RegCtrlData(E_FLOAT, NULL, NULL, "X���ߴ�", NULL, &m_fMaxXSize );
	_RegCtrlData(E_FLOAT, NULL, NULL, "Y��С�ߴ�", NULL, &m_fMinYSize );
	_RegCtrlData(E_FLOAT, NULL, NULL, "Y���ߴ�", NULL, &m_fMaxYSize );
	_RegCtrlData(E_FLOAT, NULL, NULL, "Z��С�ߴ�", NULL, &m_fMinZSize );
	_RegCtrlData(E_FLOAT, NULL, NULL, "Z���ߴ�", NULL, &m_fMaxZSize );
	
	pCombHelper = (CCombDataHelper*)_RegCtrlData(E_COMBO, NULL, NULL, "����", NULL, &m_eFaceType);
	pCombHelper->PushItem("ԭ��", SPRF_NORMAL);
	pCombHelper->PushItem("��ͷ", SPRF_CAMERA);
	pCombHelper->PushItem("��ͷ(��X��)", SPRF_CAMERANONEX);
	
	_RegCtrlData(E_VECTOR2F, NULL, NULL, "��ͼƽ��", NULL, &m_vecTextMoveVel );
	_RegCtrlData(E_FLOAT, NULL, NULL, "��ͼ��ת", NULL, &m_fTextAngularVel );
	_RegCtrlData(E_FLOAT, NULL, NULL, "��ͼ�����", NULL, &m_fTextMaxScale );
	_RegCtrlData(E_FLOAT, NULL, NULL, "��ͼ��С����", NULL, &m_fTextMinScale );
	_RegCtrlData(E_ESTRING, NULL, NULL, "����������", NULL, &m_szParentSk );
	
	pCombHelper = (CCombDataHelper*)_RegCtrlData(E_COMBO, NULL, NULL, "�ӹ�������", NULL, &m_nSkIdx);
	for ( size_t i=0; i<SkList.size(); ++i )
		pCombHelper->PushItem(SkList[i].c_str(), i);
	
	_RegCtrlData(E_VECTOR, NULL, NULL, "�ü��淨����", NULL, &(m_ClipPlane.originPlane.m_Normal) );
	_RegCtrlData(E_EFloatMap, NULL, NULL, "�ü������", NULL, &m_ClipDistMap );
	_RegCtrlData(E_BOOLValue, NULL, NULL, "�Ƿ�����Ӱ", NULL, &m_bEnableShadow );
	_RegCtrlData(E_EFloatMap, NULL, NULL, "����������", NULL, &m_mapRefract );


	// advanced prop 
	if( m_nSkIdx > 0 && m_pSkeletalFrame && m_nSkIdx <= m_pSkeletalFrame->GetSkeletalTotalNum() )
		m_szSk = m_pSkeletalFrame->GetSkeletalName(m_nSkIdx-1);

	// EFADV_SCALE   EMap<float, byte>
	_RegCtrlData(E_EBYTEMap, NULL, NULL, "��С", NULL, &m_Scale );

	// EFADV_OFFSET
	_RegCtrlData(E_EVectorMap, NULL, NULL, "�켣", NULL, &m_vecOffset );

	// EFADV_COLOR
	_RegCtrlData(E_ECOLORMap, NULL, NULL, "��ɫ", NULL, &m_Color );
	
	// EFADV_XYZCALE;
	_RegCtrlData(E_EVectorMap, NULL, NULL, "XYZ����", NULL, &m_XYZcale );
	
}

int32 CEditModelConsoleProp::GetProp( EFFECT_PROP* pProp, int32 nBufLen )
{
	EString PieceList = "None";
	EString AniList = "";
	EString SkList = "None";
	_GetListNames(&PieceList, &AniList, &SkList);
	// add end

	EVector<EFFECT_PROP> Prop;
	CEditUnitPropHelp::GetBaseProp( Prop );

	Prop.push_back( EFFECT_PROP( "����ʱ��",		m_BirthTime ) );
	Prop.push_back( EFFECT_PROP( "����ʱ��",		m_DeleteTime ) );

	Prop.push_back( EFFECT_PROP( "��ת��",			m_vecRotate ) );
	Prop.push_back( EFFECT_PROP( "MOD�ļ���",		sqr::CStringData( 1, m_szARPName ) ) );
	Prop.push_back( EFFECT_PROP( "piece0",			CComboData( (BYTE)m_nPiece0, PieceList.c_str() ) ) );
	Prop.push_back( EFFECT_PROP( "piece0��Ⱦ����",	m_nRS0 ) );
	Prop.push_back( EFFECT_PROP( "piece1",			CComboData( (BYTE)m_nPiece1, PieceList.c_str() ) ) );
	Prop.push_back( EFFECT_PROP( "piece1��Ⱦ����",	m_nRS1 ) );
	Prop.push_back( EFFECT_PROP( "piece2",			CComboData( (BYTE)m_nPiece2, PieceList.c_str() ) ) );
	Prop.push_back( EFFECT_PROP( "piece2��Ⱦ����",	m_nRS2 ) );
	Prop.push_back( EFFECT_PROP( "piece3",			CComboData( (BYTE)m_nPiece3, PieceList.c_str() ) ) );
	Prop.push_back( EFFECT_PROP( "piece3��Ⱦ����",	m_nRS3 ) );
	Prop.push_back( EFFECT_PROP( "piece4",			CComboData( (BYTE)m_nPiece4, PieceList.c_str() ) ) );
	Prop.push_back( EFFECT_PROP( "piece4��Ⱦ����",	m_nRS4 ) );
	Prop.push_back( EFFECT_PROP( "piece5",			CComboData( (BYTE)m_nPiece5, PieceList.c_str() ) ) );
	Prop.push_back( EFFECT_PROP( "piece5��Ⱦ����",	m_nRS5 ) );
	Prop.push_back( EFFECT_PROP( "SKE�ļ���",		sqr::CStringData( 1, m_szARAName ) ) );
	Prop.push_back( EFFECT_PROP( "������",			CComboData( (BYTE)m_Animation, AniList.c_str() ) ) );
	//Prop.push_back( EFFECT_PROP( "ͬ��ģ�Ͷ���",	CComboData( (BYTE)m_bSynch, "��ͬ��,ͬ��" ) ) );

	Prop.push_back( EFFECT_PROP( "X��С�ߴ�",		m_fMinXSize ) );
	Prop.push_back( EFFECT_PROP( "X���ߴ�",		m_fMaxXSize ) );
	Prop.push_back( EFFECT_PROP( "Y��С�ߴ�",		m_fMinYSize ) );
	Prop.push_back( EFFECT_PROP( "Y���ߴ�",		m_fMaxYSize ) );
	Prop.push_back( EFFECT_PROP( "Z��С�ߴ�",		m_fMinZSize ) );
	Prop.push_back( EFFECT_PROP( "Z���ߴ�",		m_fMaxZSize ) );

	Prop.push_back( EFFECT_PROP( "����",			CComboData( (BYTE)m_eFaceType, "ԭ��,��ͷ,��ͷ(��X��)" ) ) );

	Prop.push_back( EFFECT_PROP( "��ͼƽ��",		CVector3f( m_vecTextMoveVel.x, m_vecTextMoveVel.y, 0 ) ) );
	Prop.push_back( EFFECT_PROP( "��ͼ��ת",		m_fTextAngularVel ) );
	Prop.push_back( EFFECT_PROP( "��ͼ�����",	m_fTextMaxScale ) );
	Prop.push_back( EFFECT_PROP( "��ͼ��С����",	m_fTextMinScale ) );

	Prop.push_back( EFFECT_PROP( "����������",		sqr::CStringData( 0, m_szParentSk.c_str() ) ) );
	Prop.push_back( EFFECT_PROP( "�ӹ�������",		CComboData( (BYTE)m_nSkIdx, SkList.c_str() ) ) );

	Prop.push_back( EFFECT_PROP( "�ü��淨����",	m_ClipPlane.originPlane.m_Normal ) );
	Prop.push_back( EFFECT_PROP( "�ü������",		m_ClipDistMap ) ) ;

	Prop.push_back( EFFECT_PROP( "�Ƿ�����Ӱ",		CComboData( (BYTE)m_bEnableShadow, "��, ��" ) ) );

	Prop.push_back(EFFECT_PROP("����������",		m_mapRefract));

	if( m_nSkIdx > 0 && m_pSkeletalFrame && m_nSkIdx <= m_pSkeletalFrame->GetSkeletalTotalNum() )
		m_szSk = m_pSkeletalFrame->GetSkeletalName(m_nSkIdx-1);

	nBufLen = min( nBufLen,(int32) Prop.size() );
	for( int32 i = 0; i < nBufLen; ++i )
		pProp[i] = Prop[i];

	return nBufLen;
}

void CEditModelConsoleProp::SetProp( const EFFECT_PROP* pProp, int32 nBufLen )
{
	EVector<EFFECT_PROP> Prop( pProp, pProp + nBufLen );
	CEditUnitPropHelp::SetBaseProp( Prop );

	for( size_t i = 0; i < Prop.size(); ++i )
	{
		if( Prop[i].Name() == "����ʱ��" )			m_BirthTime = Prop[i].Data().Int();
		if( Prop[i].Name() == "����ʱ��" )			m_DeleteTime = Prop[i].Data().Int();
		if( Prop[i].Name() == "��ת��" )			m_vecRotate = Prop[i].Data().Vec();
		if( Prop[i].Name() == "MOD�ļ���" )			m_szARPName	= Prop[i].Data().Str();
		if( Prop[i].Name() == "piece0" )			m_nPiece0 = Prop[i].Data().Index();
		if( Prop[i].Name() == "piece0��Ⱦ����" )	m_nRS0 = max( 0, Prop[i].Data().Int() );
		if( Prop[i].Name() == "piece1" )			m_nPiece1 = Prop[i].Data().Index();
		if( Prop[i].Name() == "piece1��Ⱦ����" )	m_nRS1 = max( 0, Prop[i].Data().Int() );
		if( Prop[i].Name() == "piece2" )			m_nPiece2 = Prop[i].Data().Index();
		if( Prop[i].Name() == "piece2��Ⱦ����" )	m_nRS2 = max( 0, Prop[i].Data().Int() );
		if( Prop[i].Name() == "piece3" )			m_nPiece3 = Prop[i].Data().Index();
		if( Prop[i].Name() == "piece3��Ⱦ����" )	m_nRS3 = max( 0, Prop[i].Data().Int() );
		if( Prop[i].Name() == "piece4" )			m_nPiece4 = Prop[i].Data().Index();
		if( Prop[i].Name() == "piece4��Ⱦ����" )	m_nRS4 = max( 0, Prop[i].Data().Int() );
		if( Prop[i].Name() == "piece5" )			m_nPiece5 = Prop[i].Data().Index();
		if( Prop[i].Name() == "piece5��Ⱦ����" )	m_nRS5 = max( 0, Prop[i].Data().Int() );
		if( Prop[i].Name() == "SKE�ļ���" )			m_szARAName = Prop[i].Data().Str();
		if( Prop[i].Name() == "������" )			m_Animation = Prop[i].Data().Index();
		//if( Prop[i].Name() == "ͬ��ģ�Ͷ���" )		m_bSynch = Prop[i].Data().Index() != 0;

		if( Prop[i].Name() == "X��С�ߴ�" )			m_fMinXSize  = Prop[i].Data().Float();
		if( Prop[i].Name() == "X���ߴ�" )			m_fMaxXSize  = Prop[i].Data().Float();
		if( Prop[i].Name() == "Y��С�ߴ�" )			m_fMinYSize  = Prop[i].Data().Float();
		if( Prop[i].Name() == "Y���ߴ�" )			m_fMaxYSize  = Prop[i].Data().Float();
		if( Prop[i].Name() == "Z��С�ߴ�" )			m_fMinZSize  = Prop[i].Data().Float();
		if( Prop[i].Name() == "Z���ߴ�" )			m_fMaxZSize  = Prop[i].Data().Float();

		if( Prop[i].Name() == "����" )				m_eFaceType = (SPR_FACE)Prop[i].Data().Index();

		if( Prop[i].Name() == "��ͼƽ��" )			m_vecTextMoveVel  = Prop[i].Data().Vec();
		if( Prop[i].Name() == "��ͼ��ת" )			m_fTextAngularVel  = Prop[i].Data().Float();
		if( Prop[i].Name() == "��ͼ�����" )		m_fTextMaxScale = Prop[i].Data().Float();
		if( Prop[i].Name() == "��ͼ��С����" )		m_fTextMinScale = Prop[i].Data().Float();

		if( Prop[i].Name() == "����������" )		m_szParentSk = Prop[i].Data().Str();
		if( Prop[i].Name() == "�ӹ�������" )		m_nSkIdx = Prop[i].Data().Index();

		if (Prop[i].Name() == "�ü��淨����")		m_ClipPlane.originPlane.m_Normal = Prop[i].Data().Vec();
		if (Prop[i].Name() == "�ü������")			m_ClipDistMap = *Prop[i].Data().FloatList();

		if( Prop[i].Name() == "�Ƿ�����Ӱ" )		m_bEnableShadow = Prop[i].Data().Index() != 0;

		if (Prop[i].Name() == "����������")			m_mapRefract = *Prop[i].Data().FloatList();
	}

	_UpdateParams();
}

uint32 CEditModelConsoleProp::GetAdvancedProp( ADVANCE_PROP* pOffset,   int32& nOffLen,
										  ADVANCE_PROP* pTexture,   int32& nTextLen,
										  ADVANCE_PROP* pColor,     int32& nColorLen,
										  ADVANCE_PROP* pScale,     int32& nScaleLen, 
										  ADVANCE_PROP* pXYZcale,   int32& nXYZcaleLen,
										  ADVANCE_PROP* pUVRCenter, int32& nUVRCenter)
{
	CEditUnitPropHelp::GetAdvancedProp( pOffset, nOffLen, pTexture, nTextLen, pColor, nColorLen, pScale, nScaleLen, pXYZcale, nXYZcaleLen, pUVRCenter, nUVRCenter );

	return EFADV_SCALE|EFADV_OFFSET|EFADV_COLOR|EFADV_XYZCALE;
}

void CEditModelConsoleProp::SetAdvancedProp( const ADVANCE_PROP* pOffset,   int32 nOffLen,
										const ADVANCE_PROP* pTexture,   int32 nTextLen,
										const ADVANCE_PROP* pColor,     int32 nColorLen,
										const ADVANCE_PROP* pScale,     int32 nScaleLen,
										const ADVANCE_PROP* pXYZcale,   int32 nXYZcaleLen,
										const ADVANCE_PROP* pUVRCenter, int32 nUVRCenter)
{
	CEditUnitPropHelp::SetAdvancedProp( pOffset, nOffLen, pTexture, 0, pColor, nColorLen, pScale, nScaleLen, pXYZcale, nXYZcaleLen, pUVRCenter, nUVRCenter);
}