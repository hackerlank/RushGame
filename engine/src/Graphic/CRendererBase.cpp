#include "stdafx.h"
#include "CRendererBase.h"
#include "CWindowTarget.h"

namespace sqr
{

//------------------------------------------------------------------------------
CRendererBase::CRendererBase()
{
	/// CRendererBase ��һ��Ҫ��CMainWindowTarget��
	//if(CMainWindowTarget::HasInst())
	//	CMainWindowTarget::GetInst()->InsertRenderer(this);
}

//------------------------------------------------------------------------------
CRendererBase::~CRendererBase()
{
	//if(CMainWindowTarget::HasInst())
	//	CMainWindowTarget::GetInst()->DeleteRenderer(this);
}


}//namespace sqr
