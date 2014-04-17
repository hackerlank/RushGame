#pragma once
#include "TQuadTree.h"
#include "QuadTreeFun.h"
#include "TQuadNode.inl"
#include "ExpHelper.h"

template<typename Traits>
TQuadTree<Traits>::TQuadTree(uint32 uWidth, uint32 uHeight, uint32 uMinSpliteSize)
: m_uMinSpliteSize(uMinSpliteSize)
, m_rcSceneBox(0, (float)uHeight, (float)uWidth, 0)
{
	const CFRect& rcBox = GetQuadRect(uWidth, uHeight);
	m_pRootNode = new ImpQuadNode_t(NULL, rcBox);
}

template<typename Traits>
TQuadTree<Traits>::~TQuadTree()
{
	delete m_pRootNode;
}

template<typename Traits>
const CFRect& TQuadTree<Traits>::GetSceneBox() const
{
	return m_rcSceneBox;
}

//--------------------------------------------------------------------

//��һ������size���ã�����size rect��ͬʱ����Ҫ����obj

template<typename Traits>
void TQuadTree<Traits>::InsertSizeRectToQuad(ImpQuadObject_t* pObject)
{
	const CFPos& pos = pObject->GetPosition();

	ImpQuadNode_t* pQuadNode;

	if (pObject->m_pViewRectNode)
	{
		pQuadNode = FindDownQuadNodeWithPos(pos, pObject->m_pViewRectNode);
	}
	else
	{
		pQuadNode = FindDownQuadNodeWithPos(pos, m_pRootNode);
	}

	//���Ƿ���Ҫ����ƽ��
	//����Ҫ��ֱ�ӹ���
	//��Ҫ������ƽ�棬��ԭ�Ƚڵ�����Ķ������¹���
	//�ٴ��ж��Ƿ���Ҫ����ƽ�棬
	bool bSplited = false;
	while (BeNeedQuadNodeSplited(pQuadNode, pObject))
	{
		//�����ӽڵ�
		bSplited = true;
		pQuadNode->CreateChildNodes();
		pQuadNode = FindDownQuadNodeWithPos(pos, pQuadNode);
	}

	//���������quad node��set��
	pQuadNode->AddObjectToNode(pObject);

	//����size rect����obj����һ�����
	const CFRect& rcSizeRect = pObject->GetSizeRect();
	ImpQuadNode_t* pSizeNode = FindUpQuadNodeWithRect(rcSizeRect, pQuadNode);
	pSizeNode->AddSizeRectToNode(pObject);

	//���view rect���ڣ�����ƽ������˻��֣�view rect���ܽ�����ƽ����
	if (pObject->m_pViewRectNode && bSplited)
	{
		const CFRect& rect = pObject->GetViewRect();
		if (BeInQuadNodeBox(rect, pQuadNode->m_rcQuadNodeBox))
		{
			pObject->m_pViewRectNode->RemoveViewRectFromNode(pObject);
			pQuadNode->AddViewRectToNode(pObject);
		}
	}
}

template<typename Traits>
void TQuadTree<Traits>::RemoveSizeRectFrQuad(ImpQuadObject_t* pObject)
{	
	ImpQuadNode_t* pSizeNode = pObject->m_pSizeRectNode;
	pSizeNode->RemoveSizeRectFromNode(pObject);

	ImpQuadNode_t* pNode = pObject->m_pObjectNode;
	pNode->RemoveObjectFromNode(pObject);

	ImpQuadNode_t* pParent =  pNode->m_pParentNode;

	if (pParent == NULL)
	{
		return;
	}

	bool bMergeNeeded = pParent->BeMergeChildNodesNeeded();

	if (bMergeNeeded)
	{
		pParent->MergeChildNodes();
	}
}

//----------------------------------------------------------------------------

//��һ������eye sight����

template<typename Traits>
void TQuadTree<Traits>::InsertViewRectToQuad(ImpQuadObject_t* pObject)
{
	const CFRect& rcObjRect = pObject->GetViewRect();

	//������view rect����С�ڵ�
	ImpQuadNode_t* pNode;
	if (pObject->m_pObjectNode)
	{
		pNode = FindUpQuadNodeWithRect(rcObjRect, pObject->m_pObjectNode);
	}
	else
	{
		pNode = FindDownQuadNodeWithRect(rcObjRect, m_pRootNode);
	}

	//������view rect����quad node��
	pNode->AddViewRectToNode(pObject);
}


template<typename Traits>
void TQuadTree<Traits>::RemoveViewRectFrQuad(ImpQuadObject_t* pObject)
{
	ImpQuadNode_t* pNode = pObject->m_pViewRectNode;
	pNode->RemoveViewRectFromNode(pObject);
}


//----------------------------------------------------------

template<typename Traits>
void TQuadTree<Traits>::InsertObjectToQuad(ImpQuadObject_t* pObject)
{
	const CFPos& pos = pObject->GetPosition();

	//�ҵ�object�����Ӧ��Ҷ�ӽڵ�
	ImpQuadNode_t* pQuadNode = pObject->m_pObjectNode;

	/*
	���obj node���ڣ���ʱ���node��ָ���ƶ�ǰ�Ľڵ��
	���ȴ���ǰ�ڵ������ҵ������µ�pos�Ľڵ㣬Ȼ���ڴӸýڵ�
	���²��ҵ��ܹ�����pos��Ҷ�ӽڵ�
	*/
	pQuadNode = FindUpQuadNodeWithPos(pos, pQuadNode);
	pQuadNode = FindDownQuadNodeWithPos(pos, pQuadNode);


	while (BeNeedQuadNodeSplited(pQuadNode, pObject))
	{
		//�����ӽڵ�
		pQuadNode->CreateChildNodes();
		pQuadNode = FindDownQuadNodeWithPos(pos, pQuadNode);
	}

	//���������quad node��set��
	pQuadNode->AddObjectToNode(pObject);

	ResetSizeRectInQuad(pObject, pos);
	ResetViewRectInQuad(pObject, pos);
}


template<typename Traits>
void TQuadTree<Traits>::RemoveObjectFrQuad(ImpQuadObject_t* pObject)
{
	//���Ȱ�obj��node�ڵ���ɾ��
	ImpQuadNode_t* pNode = pObject->m_pObjectNode;
	pNode->RemoveObjectFromNode(pObject);

	//�������ǰ�obj node����ΪpNode����Ϊ�������ƶ�֮�����ڵ��ʱ��Ҫ����
	//ԭ������pNode�Ľڵ����ϲ���
	pObject->m_pObjectNode = pNode;

	ImpQuadNode_t* pParent =  pNode->m_pParentNode;

	if (pParent == NULL)
	{
		return;
	}

	//Ȼ���ж�ɾ��obj֮���node�ĸ��ڵ��Ƿ���Ҫ�鲢�ӽڵ�
	bool bMergeNeeded = pParent->BeMergeChildNodesNeeded();

	if (bMergeNeeded)
	{
		//��obj nodeָ��parent�ڵ�
		//��Ϊ���������Ҫͨ������ڵ����ϲ��Ұ����µ�pos�Ľڵ�
		pObject->m_pObjectNode = pParent;

		pParent->MergeChildNodes();
	}

}


template<typename Traits>
void TQuadTree<Traits>::ResetSizeRectInQuad(ImpQuadObject_t* pObject, const CFPos& NewPos)
{
	ImpQuadNode_t* pSizeNode = pObject->m_pSizeRectNode;
	if (!pSizeNode)
		return;

	pObject->SetSizeRect(NewPos, GetSceneBox());

	const CFRect& NewRect = pObject->GetSizeRect();

	//����µ�size rectҲ��Ȼ��ԭ�ȵĽڵ���
	if (BeInQuadNodeBox(NewRect, pSizeNode->m_rcQuadNodeBox))
	{

		//size rect���ܽ����˸ýڵ���ӽڵ�
		ImpQuadNode_t* pChildNode = FindDownQuadNodeWithRect(NewRect, pSizeNode);
		if (pChildNode == pSizeNode)
		{
			return;
		}
		else
		{
			pSizeNode->RemoveSizeRectFromNode(pObject);
			pChildNode->AddSizeRectToNode(pObject);		
			return;
		}

	}
	else
	{
		pSizeNode->RemoveSizeRectFromNode(pObject);
		//���������ұ����ڵ����ϲ���֪���ܰ���סNewRect�Ľڵ�
		pSizeNode = FindUpQuadNodeWithRect(NewRect, pSizeNode);
		//Ȼ��Ӹýڵ����²���
		pSizeNode = FindDownQuadNodeWithRect(NewRect, pSizeNode);

		pSizeNode->AddSizeRectToNode(pObject);
		return;
	}	
}

template<typename Traits>
void TQuadTree<Traits>::ResetViewRectInQuad(ImpQuadObject_t* pObject, const CFPos& NewPos)
{
	ImpQuadNode_t* pViewNode = pObject->m_pViewRectNode;
	if (!pViewNode)
		return;

	pObject->SetViewRect(NewPos, GetSceneBox());

	const CFRect& NewRect = pObject->GetViewRect();

	//����µ�view rectҲ��Ȼ��ԭ�ȵĽڵ���
	if (BeInQuadNodeBox(NewRect, pViewNode->m_rcQuadNodeBox))
	{

		//view rect���ܽ����˸ýڵ���ӽڵ�
		ImpQuadNode_t* pChildNode = FindDownQuadNodeWithRect(NewRect, pViewNode);
		if (pChildNode == pViewNode)
		{
			return;
		}
		else
		{
			pViewNode->RemoveViewRectFromNode(pObject);
			pChildNode->AddViewRectToNode(pObject);		
			return;
		}

	}
	else
	{
		pViewNode->RemoveViewRectFromNode(pObject);
		//���������ұ����ڵ����ϲ���֪���ܰ���סNewRect�Ľڵ�
		pViewNode = FindUpQuadNodeWithRect(NewRect, pViewNode);
		//Ȼ��Ӹýڵ����²���
		pViewNode = FindDownQuadNodeWithRect(NewRect, pViewNode);

		pViewNode->AddViewRectToNode(pObject);
		return;
	}
}


//------------------------------------------------------------------
template<typename Traits>
typename TQuadTree<Traits>::ImpQuadNode_t* TQuadTree<Traits>::FindUpQuadNodeWithPos(const CFPos& pos, ImpQuadNode_t* pNode)
{
	while (pNode)
	{
		if (BeInQuadNodeBox(pos, pNode->m_rcQuadNodeBox))
		{
			return pNode;
		}
		pNode = pNode->m_pParentNode;
	}
	
	ostringstream strm;
	strm << "pos : (" << pos.x << ", " << pos.y << " )" << endl;
	GenErr("pos ������Χ", strm.str().c_str());
	return NULL;
}

template<typename Traits>
typename TQuadTree<Traits>::ImpQuadNode_t* TQuadTree<Traits>::SelectChildNodeWithPos(const CFPos& pos, ImpQuadNode_t* pNode)
{
	if(pNode->m_pNEChildNode)
	{
		if (pos.x > pNode->m_pNEChildNode->m_rcQuadNodeBox.left)
		{
			if (pos.y >= pNode->m_pNEChildNode->m_rcQuadNodeBox.bottom)
			{
				return pNode->m_pNEChildNode;
			}
			else
			{
				return pNode->m_pSEChildNode;
			}
		}
		else if(pos.x <= pNode->m_pNWChildNode->m_rcQuadNodeBox.right)
		{
			if (pos.y >= pNode->m_pNWChildNode->m_rcQuadNodeBox.bottom)
			{
				return pNode->m_pNWChildNode;
			}
			else
			{
				return pNode->m_pSWChildNode;
			}
		}

	}
	return NULL;
}

template<typename Traits>
typename TQuadTree<Traits>::ImpQuadNode_t* TQuadTree<Traits>::FindDownQuadNodeWithPos(const CFPos& pos, ImpQuadNode_t* pNode)
{
	ImpQuadNode_t* pSelectNode = pNode;
	ImpQuadNode_t* pChildNode = pNode;

	if (BeInQuadNodeBox(pos, pNode->m_rcQuadNodeBox))
	{
		if(pNode->m_pNEChildNode)
		{
			while(pSelectNode)
			{
				pChildNode = pSelectNode;
				pSelectNode = SelectChildNodeWithPos(pos, pSelectNode);
			}
			if(pChildNode != pNode)
				return pChildNode;
		}

		return pNode;
	}
	

	//����Ҳ����������ϲ���
	return FindUpQuadNodeWithPos(pos, pNode->m_pParentNode);
}

//-------------------------------------------------------------------

template<typename Traits>
typename TQuadTree<Traits>::ImpQuadNode_t* TQuadTree<Traits>::FindUpQuadNodeWithRect(const CFRect& rcRect, ImpQuadNode_t* pNode)
{
	while (pNode)
	{
		if (BeInQuadNodeBox(rcRect, pNode->m_rcQuadNodeBox))
		{
			return pNode;
		}
		pNode = pNode->m_pParentNode;
	}

	GenErr("view rect������Χ");
	return NULL;
}

template<typename Traits>
typename TQuadTree<Traits>::ImpQuadNode_t* TQuadTree<Traits>::SelectChildNodeWithRect(const CFRect& rcRect, ImpQuadNode_t* pNode)
{
	if(pNode->m_pNEChildNode)
	{
		if (rcRect.left > pNode->m_pNEChildNode->m_rcQuadNodeBox.left)
		{
			if (rcRect.bottom >= pNode->m_pNEChildNode->m_rcQuadNodeBox.bottom)
			{
				return pNode->m_pNEChildNode;
			}
			else if(rcRect.top < pNode->m_pNEChildNode->m_rcQuadNodeBox.bottom)
			{
				return pNode->m_pSEChildNode;
			}
		}
		else if(rcRect.right <= pNode->m_pNWChildNode->m_rcQuadNodeBox.right)
		{
			if (rcRect.bottom >= pNode->m_pNWChildNode->m_rcQuadNodeBox.bottom)
			{
				return pNode->m_pNWChildNode;
			}
			else if(rcRect.top < pNode->m_pNWChildNode->m_rcQuadNodeBox.bottom)
			{
				return pNode->m_pSWChildNode;
			}
		}
	}
	return NULL;
}

template<typename Traits>
typename TQuadTree<Traits>::ImpQuadNode_t* TQuadTree<Traits>::FindDownQuadNodeWithRect(const CFRect& rcRect, ImpQuadNode_t* pNode) 
{
	ImpQuadNode_t* pSelectNode = pNode;
	ImpQuadNode_t* pChildNode = pNode;

	if (BeInQuadNodeBox(rcRect, pNode->m_rcQuadNodeBox))
	{
		if(pNode->m_pNEChildNode)
		{
			while(pSelectNode)
			{
				pChildNode = pSelectNode;
				pSelectNode = SelectChildNodeWithRect(rcRect, pSelectNode);
			}
			if(pChildNode != pNode)
				return pChildNode;
		}
		
		return pNode;
	}


	//���node�����ڣ������ϲ���
	return FindUpQuadNodeWithRect(rcRect, pNode->m_pParentNode);
}


//-------------------------------------------------------------------
template<typename Traits>
bool TQuadTree<Traits>::BeNeedQuadNodeSplited(ImpQuadNode_t* pNode, ImpQuadObject_t* pObject)
{
	if (pNode->QuadNodeBoxWidth() <= m_uMinSpliteSize || 
		pNode->QuadNodeBoxHeight() <= m_uMinSpliteSize)
	{
		return false;
	}


	//�ڵ�û�ж���ֱ�ӹ��أ�����Ҫ�з�
	if (pNode->m_lstObject.empty())
	{
		return false;
	}
	else
	{
		float fXCenter = pNode->m_rcQuadNodeBox.left + 
			(pNode->m_rcQuadNodeBox.right - pNode->m_rcQuadNodeBox.left) / 2;

		const CFPos& pos = pObject->GetPosition();
		int iXPos = (int)pos.x / m_uMinSpliteSize;
		int iYPos = (int)pos.y / m_uMinSpliteSize;
		//���aoi�����ڽڵ���ε�right���������������棬����������ڵ�
		if (pos.x == pNode->m_rcQuadNodeBox.right || pos.x == fXCenter)
			iXPos = iXPos - 1;
	
		typename ListQuadObject_t::const_iterator iter = pNode->m_lstObject.begin();
		const CFPos& OtPos = (*iter)->GetPosition();
		int iOtXPos = (int)OtPos.x / m_uMinSpliteSize;
		int iOtYPos = (int)OtPos.y / m_uMinSpliteSize;

		//���aoi�����ڽڵ���ε�right���������������棬����������ڵ�
		if (OtPos.x == pNode->m_rcQuadNodeBox.right || OtPos.x == fXCenter)
			iOtXPos = iOtXPos - 1;
	
		if (iXPos == iOtXPos && iYPos == iOtYPos)
			return false;
		else
			return true;

		return false;
	}
}


