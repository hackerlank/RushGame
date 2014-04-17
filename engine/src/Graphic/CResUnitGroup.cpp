#include "stdafx.h"
#include "CResUnitGroup.h"

uint32	CResUnitGroup::AddNodeRef()
{
	++m_NodeRef;
	if(m_bInRecycle)
	{
		GfkLogErr("Group�쳣�޸�","�����˻���վ�еĽڵ�");
		Reclaim();
		Release();
	}
	return m_NodeRef;
}

void	CResUnitGroup::NodeRelease()
{
	if ( --m_NodeRef == 0 && GetRefCount() == 0 )
		FreeRes();
};	

void	CResUnitGroup::Free()
{
	if(m_NodeRef == 0)
		FreeRes();
}

int32	CResUnitGroup::AddRef(void)
{ 
	return GraphicRes::AddRef(); 
}