#pragma once
#include "CDynamicObject.h"
#include "GraphicBase.h"

namespace sqr
{
/// ���νṹ�Ľ��
class CRenderSpaceNode;
class CCamera;

class CRenderSpace : public virtual CDynamicObject
{
public:
	typedef	set<CRenderSpaceNode*>	SpaceNodeList;
	SpaceNodeList					m_listNode;
public:
	CRenderSpace();
	virtual ~CRenderSpace();
	virtual void	AddChild( CRenderSpaceNode* pChild, eLinkType linkType, const char* szAttachName );
	virtual void	DelChild( CRenderSpaceNode* pChild );
	virtual void	GetAttachMatrix( CMatrix& matLink, uint32 uChildLinkID );

	//OnPreRenderEnd �� GetWillRenderҪ����ɾ��
	virtual bool	GetWillRender();
	
	virtual void	OnPreRender( uint32 uCurTime, CCamera* pCamera );
	virtual void	Render(void);

	virtual void	RenderEx( CCamera* pCamera, uint32 uCurTime );
	virtual void	DestroyRenderChild(CRenderSpaceNode* pObj);
};
}
