#pragma once

#include "EffectBase.h"
#include "CEditEffect.h"
#include "CGraphic.h"
#include "IEffectClassDesc.h"
#include "CEffectManager.h"

namespace sqr
{
	class CEditUnitPropHelp;
}

class CEditEffectGroup;

class CEditEffectManager : public CEffectManager
{
	//map< string, map< string, CEditEffect* > >	m_pEffects;
	CEditEffectGroup*						m_pEffectGroup;
	map<string,vector<string>>				m_GroupPropsTree;
	bool									m_bLoadRes;

public:
	static	CEditEffectManager*		GetEditInst();	
	CEditEffectManager( );
	virtual ~CEditEffectManager(void);

	int					GetEffectGUIDNum();
	const GUID			GetEffectGUID( int Num );
	const char*			GetDLLName( const GUID Guid );
	const char*			GetShowString( const GUID Guid, DWORD Time );
	int					GetEffectUnitNum( const GUID Guid );
	string				GetEffectUnitName( const GUID Guid, int Num );
	CEditUnitPropHelp*	CreateEffectUnitPropHelp( const GUID Guid, int Num );
	void				DestroyEffectUnit( IEffectUnit* );
	void				DestroyEffectUnitProp( IEffectUnitProp* );
	void				ClearAll();
	void				GetEffectName( map< string, vector< string > >& EffectName );
	BOOL				CreateEffectGroup( string GroupName );
	CEditEffectProp*	CreateEffect( string GroupName, string Name );
	CEditEffectProp*	GetEditEffectProp( string GroupName, string Name );
	BOOL				ReNameGroup( string Old, string New );
	BOOL				ReNameEffect( string GroupName, string Old, string New );
	BOOL				DeleteGroup( string Name );
	BOOL				DeleteEffect( string GroupName, string Name );
	BOOL				MoveEffectToGroup( string Name, string SrcGroupName, string DestGroupName );

	int					LoadEffectGroups( string LoadName );
	//void				LoadSplitEffectGroups( const string& LoadName );
	int					SaveEffectGroups( string SaveName, CEditEffectGroup* pGroup = NULL );
	int					SaveEffectGroupsByChunk( string SaveName );

	//void				MoveEffectGroup(IEffectGroup* pEffectGroup);

	///�����ָ��������effect
	void				SplitEffectPackSave( const string& SaveName,CEditEffectGroup* pEditEffectGroup);
	void				ResetClassDesc();
	void				SetLoadRes( bool bLoadRes ) { m_bLoadRes = bLoadRes; }
	void				SetFramSynch(bool bResult);
	void				SetFramSynch(string strGroup,string strEffect,bool bResult);
	void				SetFramSynch(string strEffect,bool bResult);
	void				Render( uint32 CurTime );

	virtual HRESULT					CreateEffect( IEffectGroup* pEffectGroup, const char* szEffectName, IEffect** pEffect );
	virtual IEffectUnitProp*		CreateEffectUnitProp( const GUID Guid, int Num );
	static CEditUnitPropHelp*		EffectUnitPropCast_ToPropHelp(IEffectUnitProp* pProp);
	void							CreateEditEffectMap(IEffectGroup* pEffectGroup);

	//�����ṩ�ر������Ϊ��Ч��ɾ������Ч�鱣������ǰ�ļ���Ϣ
	void				ClearEffectGroup(string strName);
	void				ChangeGroupName(string strNew);
	CEditEffectGroup*			GetEditEffectGroup();
		
protected:
	virtual IEffectGroup*		CreateEffectGroupFun(const char* szFileName);
};
