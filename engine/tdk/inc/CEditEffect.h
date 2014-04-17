#pragma once
#include "IEffectUnit.h"
#include <set>
#include "CEffect.h"
#include "CEffectGroup.h"
namespace sqr
{
	class CEditUnitPropHelp;
}


class CEditEffectProp;
class CEditEffectManager;
class CEditEffect;

class CEditEffect :	public CEffect
{
	friend class CEditTrack;
	friend class EDITEFFECTKEY;
	friend class CEditEffectProp;
	
	typedef EMap< int, EFFECTKEY > sTrack;
public:
	CEditEffect( CEditEffectManager* pEffectMgr, CEffectProp* pEffectProp );
	//CEditEffect( CEditEffectManager* pEffectMgr );
	virtual ~CEditEffect(void);

	//const CEditEffect& operator= ( const CEditEffect& Effect );

	virtual float					GetCurFrame();	
	CEditEffectProp*				GetEditEffectProp();
	virtual void					OnLinked( CRenderable* pParentObj );
	virtual void					OnUnlinked( void );
	void							OnEffectUintLinked( EFFECTKEY* EditKey );

	CEffect::EFFECTKEY&				GetEffectKey(int ln,int pos);
	EMap< int, CEffect::EFFECTKEY >&							GetEffectKeys(int ln);
	void							DeleteEffectTrack(int ln);
	virtual void					SetUnitVisibleOnRender(int track,int key);

	virtual const AniResult			Render( const CMatrix& matWorld, uint32 uCurTime,  RenderObjStyle* pRORS = NULL );	//��Ⱦģ��	
	//���溯����ҪΪ���ƹ��ʱ��Ҫ������Ϣ���Ƹ��¹��
	void CopyTrackBindInfo(int source,int dest);
			void					UpdateLink(void);
private:
	CRenderable*	m_RenderAble;

};