#include "stdafx.h"
#include "CRenderModel.h"

//#define USENEWTRANSMETHOD
//#define MixObjHeight 0//�����Ҫ�����������������ķ�ʽ�ƶ���������Ӱ���õĹ淶���� //200 //����������С�߶�
//
//#define MAX_ALPHA			255
//#define MIN_ALPHA			64


//------------------------------------------------------------------------------
CRenderModel::CRenderModel(ILogicHandler* pHandler)
: CRenderObject(NULL,pHandler)
, m_bCameraCollision(false) 
{
}

//------------------------------------------------------------------------------
CRenderModel::~CRenderModel()
{
}

bool  CRenderModel::Intersects( const CVector3f& Pos )
{
	if(m_bCameraCollision)
	{
		CAxisAlignedBox Temp = m_OrgAABB;
		Temp.transformBox(m_CenterMatrix);
		return Temp.intersects(Pos);
	}
	else
		return false;
}