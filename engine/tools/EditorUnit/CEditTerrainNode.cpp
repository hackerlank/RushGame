#include "stdafx.h"
#include "CEditTerrainNode.h"
#include "CEditTerrainMgr.h"
#include "CElementManager.h"

CEditTerrainNode::CEditTerrainNode(CEditTerrainMgr* pParent , URegionID id)
: TerrainNode(pParent,id) 
{
}

CEditTerrainNode::~CEditTerrainNode()
{
}

void CEditTerrainNode::CreateBuffer()
{
	//�������
	int a=0;
}

void CEditTerrainNode::DestroyBuffer()
{
}