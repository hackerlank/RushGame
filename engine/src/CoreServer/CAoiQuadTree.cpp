#include "stdafx.h"
#include "CAoiQuadTree.h"
#include "QuadTreeFun.h"
#include "CSynAoiDimObj.h"
#include "CAoiQuadNode.h"
#include "TQuadTree.inl"
#include "TSqrAllocator.inl"

template class TQuadTree<CTraitsAoiQuad>;

CAoiQuadTree::CAoiQuadTree(uint32 uWidth, uint32 uHeight, uint32 uMinSpliteSize)
: Parent_t(uWidth, uHeight, uMinSpliteSize)
{

}


CAoiQuadTree::~CAoiQuadTree()
{

}

//void CAoiQuadTree::InsertSizeRectToQuad(AoiObject* pObj)
//{
//	Parent_t::InsertSizeRectToQuad(pObj);
//}
//
//void CAoiQuadTree::InsertViewRectToQuad(AoiObject* pObj)
//{
//	Parent_t::InsertViewRectToQuad(pObj);	
//}
//
//void CAoiQuadTree::RemoveSizeRectFrQuad(AoiObject* pObj)
//{
//	Parent_t::RemoveSizeRectFrQuad(pObj);
//}
//
//void CAoiQuadTree::RemoveViewRectFrQuad(AoiObject* pObj)
//{
//	Parent_t::RemoveViewRectFrQuad(pObj);
//}

void CAoiQuadTree::ResetAoiObject(AoiObject* pObj, const CFPos& NewPos)
{
	//���Ȱ�aoi obj��quad��ɾ��
	RemoveObjectFrQuad(pObj);
	pObj->SetAoiPos(NewPos);
	//�ڰ�aoi obj����quad
	InsertObjectToQuad(pObj);
}

void CAoiQuadTree::SetAoiPos(AoiObject* pObject, const CFPos& NewPos)
{
	CAoiQuadNode* pNode = pObject->m_pObjectNode;
	CAoiQuadNode* pViewNode = pObject->m_pViewRectNode;

	if (pNode)
	{
		//�����������Ȼ�ڽڵ���
		if (BeInQuadNodeBox(NewPos, pNode->m_rcQuadNodeBox))
		{			
			pObject->SetAoiPos(NewPos);
			ResetSizeRectInQuad(pObject, NewPos);
			ResetViewRectInQuad(pObject, NewPos);
			return;
		}
		else
		{
			ResetAoiObject(pObject, NewPos);
			return;
		}
	}

	//���������aoi obj��ֻ��view rect
	if (pViewNode)
	{
		pObject->SetAoiPos(NewPos);
		ResetViewRectInQuad(pObject, NewPos);
		return;
	}

	//�������û�м����Ĳ������棬������ȻҪ��������������
	pObject->SetAoiPos(NewPos);
}

void CAoiQuadTree::SetAoiSizeRect(AoiObject* pObject)
{
	ResetSizeRectInQuad(pObject, pObject->GetPosition());
}

void CAoiQuadTree::SetAoiViewRect(AoiObject* pObject)
{
	ResetViewRectInQuad(pObject, pObject->GetPosition());
}

//--------------------------------------------------------------------------



void CAoiQuadTree::QueryObservee(VecObject_t& vecObj, const CFPos& pos, float fAoiRadius) 
{
	CFRect rcObjRect;

	GetRadiusRect(rcObjRect, pos, fAoiRadius, GetSceneBox());

	QueryObservee(vecObj, rcObjRect);

}

void CAoiQuadTree::QueryObservee(VecObject_t& vecObj, CFRect& rcObjRect) 
{
	CAoiQuadNode* pNode = FindDownQuadNodeWithRect(rcObjRect, m_pRootNode);

	//��pnode�ڵ㿪ʼ�������ҵ���rect�ཻ
	TravelQuadForObservee(pNode, vecObj, rcObjRect);
}


void CAoiQuadTree::QueryObservee(AoiObject* pObj, VecObject_t& vecObj) 
{
	CAoiQuadNode* pNode = pObj->m_pViewRectNode;

	TravelQuadForObservee(pNode, vecObj, pObj->GetViewRect());
}


void CAoiQuadTree::QueryObservee(AoiObject* pObj, VecObject_t& vecObj, const CFPos& NewPos) 
{
	CAoiQuadNode* pNode = pObj->m_pViewRectNode;

	CFRect OldRect, NewRect;

	OldRect = pObj->GetViewRect();

	GetRadiusRect(NewRect, NewPos, pObj->GetEyeSight(), GetSceneBox());

	TravelQuadForObservee(pNode, vecObj, OldRect, NewRect);
}

void CAoiQuadTree::QueryObservee(AoiObject* pObj, VecObject_t& vecObj, const CFRect& OldRect, const CFRect& NewRect)
{
	CAoiQuadNode* pNode = pObj->m_pViewRectNode;

	TravelQuadForObservee(pNode, vecObj, OldRect, NewRect);
}

void CAoiQuadTree::QueryObserver(VecObject_t& vecObj, CFRect& rcObjRect) 
{
	CAoiQuadNode* pNode = FindDownQuadNodeWithRect(rcObjRect, m_pRootNode);

	//��pnode�ڵ㿪ʼ�������ҵ���rect�ཻ
	TravelQuadForObserver(pNode, vecObj, rcObjRect);
}


void CAoiQuadTree::QueryObserver(AoiObject* pObj, VecObject_t& vecObj) 
{
	CAoiQuadNode* pNode = pObj->m_pSizeRectNode;

	TravelQuadForObserver(pNode, vecObj, pObj->GetSizeRect());
}


void CAoiQuadTree::QueryObserver(AoiObject* pObj, VecObject_t& vecObj, const CFPos& NewPos) 
{
	CAoiQuadNode* pNode = pObj->m_pSizeRectNode;

	CFRect OldRect, NewRect;

	OldRect = pObj->GetSizeRect();

	GetRadiusRect(NewRect, NewPos, pObj->GetSize(), GetSceneBox());

	TravelQuadForObserver(pNode, vecObj, OldRect, NewRect);
}

void CAoiQuadTree::QueryObserver(AoiObject* pObj, VecObject_t& vecObj, const CFRect& OldRect, const CFRect& NewRect)
{
	CAoiQuadNode* pNode = pObj->m_pSizeRectNode;

	TravelQuadForObserver(pNode, vecObj, OldRect, NewRect);
}
