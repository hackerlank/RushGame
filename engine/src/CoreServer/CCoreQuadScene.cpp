#include "stdafx.h"
#include "CCoreQuadScene.h"
#include "CCoreQuadNode.h"
#include "CoreCommon.h"
#include "CCoreObjectServer.h"
#include "CCoreQuadObject.h"
#include "QuadTreeFun.h"
#include "TQuadTree.inl"
#include "TSqrAllocator.inl"

template class TQuadTree<CTraitsCoreQuad>;

//-------------------------------------------------
CCoreQuadScene::CCoreQuadScene(uint32 uWidth, uint32 uHeight, uint32 uPrecision)
: Parent_t(uWidth, uHeight, 16 * uPrecision)
, m_uPrecision(uPrecision)
{

}


CCoreQuadScene::~CCoreQuadScene()
{

}

uint32 CCoreQuadScene::GetPrecision() const
{
	return m_uPrecision;
}

//void CCoreQuadScene::InsertSizeRectToQuad(CCoreQuadObject* pObj)
//{
//	Parent_t::InsertSizeRectToQuad(pObj);
//}
//
//void CCoreQuadScene::RemoveSizeRectFrQuad(CCoreQuadObject* pObj)
//{
//	Parent_t::RemoveSizeRectFrQuad(pObj);
//}
//
//void CCoreQuadScene::ResetSizeRectInQuad(CCoreQuadObject* pObj, const CFPos& NewPos)
//{
//	Parent_t::ResetSizeRectInQuad(pObj, NewPos);
//}

void CCoreQuadScene::SetObjectPos(CCoreQuadObject* pObject, const CFPos& NewPos)
{
	CCoreQuadNode* pNode = pObject->m_pObjectNode;
	if(!pNode)
		return;

	//�����������Ȼ�ڽڵ���
	if (BeInQuadNodeBox(NewPos, pNode->m_rcQuadNodeBox))
	{			
		pObject->SetPosition(NewPos);
		ResetSizeRectInQuad(pObject, NewPos);
		return;
	}
	else
	{
		RemoveObjectFrQuad(pObject);
		pObject->SetPosition(NewPos);
		InsertObjectToQuad(pObject);
		return;
	}
	
	pObject->SetPosition(NewPos);
}


void CCoreQuadScene::QueryObject(void (*pFunc)(void*, uint32), void* pVecObject,
								 const CFPos& pos, float fRadius)const
{
	CFRect rcObjRect;
	GetRadiusRect(rcObjRect, pos, fRadius, GetSceneBox());

	TravelQuadForObject(m_pRootNode, pFunc, pVecObject, pos, fRadius, rcObjRect);	
}

void CCoreQuadScene::QueryObject(void (*pFunc)(void*, uint32), void* pVecObject, CCoreQuadObject* pObject, 
								 const CFPos& OffsetPos, float fRadius) const
{
	CFPos pos = pObject->GetPosition();
	CFPos QueryPos = pos + OffsetPos;
	
	CFRect rect;
	GetRadiusRect(rect, QueryPos, fRadius, GetSceneBox());

	CCoreQuadNode* pNode = NULL;
	CCoreQuadNode* pSizeNode = pObject->m_pSizeRectNode;

	pNode = FindUpQuadNodeWithRect(rect, pSizeNode);
	pNode = FindDownQuadNodeWithRect(rect, pNode);
	
	TravelQuadForObject(pNode, pFunc, pVecObject, QueryPos, fRadius, rect);
}


void CCoreQuadScene::TravelDownQuadForObject(CCoreQuadNode* pNode, void (*pFunc)(void*, uint32), void* pVecObject,  
											const CFPos& pos, float fRadius, const CFRect& rect)
{

	if (pNode == NULL)
	{
		return;
	}

	//���view rect�ľ�����node��rect���ཻ
	if (!BeOverlappedQuadNode(rect, pNode->m_rcQuadNodeBox))
	{
		return;
	}

	ListQuadObject_t::const_iterator iter = pNode->m_lstSizeRect.begin();
	while(iter != pNode->m_lstSizeRect.end())
	{
		if(BeOverlappedQuadNode((*iter)->GetSizeRect(), rect))
		{
			CFPos OtPos = (*iter)->GetPosition();
			float fSize = (*iter)->GetSize();
			if((fRadius + fSize)>= pos.Dist(OtPos))
			{
				uint32 uObjID = (*iter)->m_uObjectID;
				pFunc(pVecObject, uObjID);			
			}
		}
		++iter;
	}

	if(!pNode->m_pNEChildNode)
		return;

	TravelDownQuadForObject(pNode->m_pNEChildNode, pFunc, pVecObject, pos, fRadius, rect);
	TravelDownQuadForObject(pNode->m_pNWChildNode, pFunc, pVecObject, pos, fRadius, rect);
	TravelDownQuadForObject(pNode->m_pSWChildNode, pFunc, pVecObject, pos, fRadius, rect);
	TravelDownQuadForObject(pNode->m_pSEChildNode, pFunc, pVecObject, pos, fRadius, rect);

}

void CCoreQuadScene::TravelUpQuadForObject(CCoreQuadNode* pNode, void (*pFunc)(void*, uint32), void* pVecObject, 
										  const CFPos& pos, float fRadius, const CFRect& rect)
{
	while(pNode)
	{
		ListQuadObject_t::const_iterator iter = pNode->m_lstSizeRect.begin();
		while(iter != pNode->m_lstSizeRect.end())
		{
			if(BeOverlappedQuadNode((*iter)->GetSizeRect(), rect))
			{
				CFPos OtPos = (*iter)->GetPosition();
				float fSize = (*iter)->GetSize();
				if((fRadius + fSize)>= pos.Dist(OtPos))
				{
					uint32 uObjID = (*iter)->m_uObjectID;
					pFunc(pVecObject, uObjID);
				}				
			}
			++iter;
		}
		pNode = pNode->m_pParentNode;
	}
}

void CCoreQuadScene::TravelQuadForObject(CCoreQuadNode* pNode, void (*pFunc)(void*, uint32), void* pVecObject, 
										const CFPos& pos, float fRadius, const CFRect& rect)
{
	//�������²���
	TravelDownQuadForObject(pNode, pFunc, pVecObject, pos, fRadius, rect);

	//Ȼ������
	TravelUpQuadForObject(pNode->m_pParentNode, pFunc, pVecObject, pos, fRadius, rect);
}
		
